#include "pkgi_db.h"
#include "pkgi_config.h"
#include "pkgi_utils.h"
#include "pkgi_sha256.h"
#include "pkgi.h"
#include "pkgi_download.h"

#include <stddef.h>

#define MAX_DB_SIZE (8*1024*1024)
#define MAX_DB_ITEMS 32768
#define MAX_DB_COLUMNS 32

#define EXTDB_ID_LENGTH 110
#define EXTDB_ID_SHA256 "\x7d\x24\x89\x6f\x50\xf2\xb2\x3b\x7f\xbd\x12\xc4\x7c\x67\x93\xcd\xb5\x92\x55\x7c\x1c\x09\xaf\xf3\x25\xf5\x46\x5a\x35\x7f\xc9\x64"

static char db_data[MAX_DB_SIZE];
static uint32_t db_total;
static uint32_t db_size;

static DbItem db[MAX_DB_ITEMS];
static uint32_t db_count;

static DbItem* db_item[MAX_DB_ITEMS];
static uint32_t db_item_count;

typedef enum {
    ColumnContentId,
    ColumnContentType,
    ColumnName,
    ColumnDescription,
    ColumnRap,
    ColumnUrl,
    ColumnSize,
    ColumnChecksum,
    ColumnUnknown
} ColumnType;

typedef struct {
    ColumnType type;
    const char* text_id;
    const char* data;
} ColumnEntry;

typedef struct {
    char delimiter;
    uint8_t total_columns;
    ColumnType* type;
    ColumnEntry* data;
} dbFormat;

static ColumnEntry entries[] =
{
    { ColumnContentId, "contentid", "" },
    { ColumnContentType, "type", "" },
    { ColumnName, "name", "" },
    { ColumnDescription, "description", "" },
    { ColumnRap, "rap", "" },
    { ColumnUrl, "url", "" },
    { ColumnSize, "size", "" },
    { ColumnChecksum, "checksum", "" },
};

static const ColumnType default_format[] =
{
    ColumnContentId,
    ColumnContentType,
    ColumnName,
    ColumnDescription,
    ColumnRap,
    ColumnUrl,
    ColumnSize,
    ColumnChecksum
};

static const ColumnType external_format[] =
{
    ColumnUnknown,
    ColumnUnknown,
    ColumnName,
    ColumnUrl,
    ColumnRap,
    ColumnContentId,
    ColumnUnknown,
    ColumnUnknown,
    ColumnSize,
    ColumnChecksum
};

static uint8_t hexvalue(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }
    else if (ch >= 'a' && ch <= 'f')
    {
        return ch - 'a' + 10;
    }
    else if (ch >= 'A' && ch <= 'F')
    {
        return ch - 'A' + 10;
    }
    return 0;
}

static uint8_t* pkgi_hexbytes(const char* digest, uint32_t length)
{
    uint8_t* result = (uint8_t*)digest;

    for (uint32_t i = 0; i < length; i++)
    {
        char ch1 = digest[2 * i];
        char ch2 = digest[2 * i + 1];
        if (ch1 == 0 || ch2 == 0)
        {
            return NULL;
        }

        result[i] = hexvalue(ch1) * 16 + hexvalue(ch2);
    }

    return result;
}

static char* generate_contentid(void)
{
    char* cid = (char*)pkgi_malloc(37);
    pkgi_snprintf(cid, 36, "X00000-X%08d_00-0000000000000000", db_count);
    return cid;
}

int update_database(const char* update_url, const char* path, char* error, uint32_t error_size)
{
    db_total = 0;
    db_size = 0;
    LOG("�������� ���������� �� %s", update_url);

    pkgi_http* http = pkgi_http_get(update_url, NULL, 0);
    if (!http)
    {
        pkgi_snprintf(error, error_size, "�� ������� ��������� ������ ��\n%s", update_url);
        return 0;
    }
    else
    {
        int64_t length;
        if (!pkgi_http_response_length(http, &length))
        {
            pkgi_snprintf(error, error_size, "�� ������� ��������� ������ ��\n%s", update_url);
        }
        else
        {
            if (length > (int64_t)sizeof(db_data) - 1)
            {
                pkgi_snprintf(error, error_size, "������ ������� �����... ��������� ������� ����� ����� ������ PKGi!");
            }
            else if (length != 0)
            {
                db_total = (uint32_t)length;
            }

            error[0] = 0;

            for (;;)
            {
                uint32_t want = (uint32_t)min64(1 << 16, sizeof(db_data) - 1 - db_size);
                int read = pkgi_http_read(http, db_data + db_size, want);
                if (read == 0)
                {
                        break;
                }
                else if (read < 0)
                {
                    pkgi_snprintf(error, error_size, "������ HTTP 0x%08x", read);
                    db_size = 0;
                    break;
                }
                db_size += read;
            }

            if (error[0] == 0 && db_size == 0)
            {
                pkgi_snprintf(error, error_size, "������ ���� ... ��������� ������ ��");
            }
        }

        pkgi_http_close(http);

        if (db_size == 0)
        {
            return 0;
        }
        else
        {
            pkgi_save(path, db_data, db_size);
        }
    }
    return 1;
}

int load_database(uint8_t db_id)
{
    uint8_t column = 0;
    dbFormat dbf = { ',', 8, (ColumnType*)default_format, entries };

    char path[256];
    pkgi_snprintf(path, sizeof(path), "%s/dbformat.txt", pkgi_get_config_folder());

    int loaded = pkgi_load(path, db_data, sizeof(db_data) - 1);
    if (loaded > 0) {
        char* ptr = db_data;
        char* end = db_data + loaded + 1;
        column = 0;
        ColumnType types[MAX_DB_COLUMNS];

        LOG("�������� ������� �� %s", path);

        dbf.delimiter = *ptr++;

        if (ptr < end && *ptr == '\r')
        {
            ptr++;
        }
        if (ptr < end && *ptr == '\n')
        {
            ptr++;
        }

        while (ptr < end && *ptr)
        {
            const char* column_name = ptr;
            types[column] = ColumnUnknown;

            while (ptr < end && *ptr != dbf.delimiter && *ptr != '\n' && *ptr != '\r' && column < MAX_DB_COLUMNS)
            {
                ptr++;
            }
            *ptr++ = 0;

            int j;
            for (j = 0; j < 8; j++) {
                if (pkgi_stricmp(entries[j].text_id, column_name) == 0) {
                    types[column] = entries[j].type;
                }
            }
        
            column++;
        }
        dbf.total_columns = column;
        dbf.type = types;
    }

    pkgi_snprintf(path, sizeof(path), "%s/pkgi%s.txt", pkgi_get_config_folder(), pkgi_content_tag(db_id));

    LOG("�������� ���� ������ �� %s", path);
    
    loaded = pkgi_load(path, db_data+db_size, sizeof(db_data) - 1);
    if (loaded > 0)
    {
        uint8_t check[SHA256_DIGEST_SIZE];

        sha256((uint8_t*)db_data+db_size, EXTDB_ID_LENGTH, check, 0);

        if (pkgi_memequ(EXTDB_ID_SHA256, check, SHA256_DIGEST_SIZE))
        {
            dbf.delimiter = '\t';
            dbf.total_columns = 10;
            dbf.type = (ColumnType*) external_format;
        }
    }
    else
    {
        return 0;
    }

    LOG("������ ��������� (%d ����)", loaded);

    db_size += loaded;
    db_data[db_size] = '\n';
    char* ptr = db_data + db_size - loaded;
    char* end = db_data + db_size + 1;

    if (db_size > 3 && (uint8_t)ptr[0] == 0xef && (uint8_t)ptr[1] == 0xbb && (uint8_t)ptr[2] == 0xbf)
    {
        ptr += 3;
    }

    while (ptr < end && *ptr)
    {
        column = 0;
        while (ptr < end && column < dbf.total_columns)
        {
            const char* content = ptr;

            while (ptr < end && *ptr != dbf.delimiter && *ptr != '\n' && *ptr != '\r')
            {
                ptr++;
            }
            *ptr++ = 0;

            dbf.data[dbf.type[column]].data = content;
            column++;
        }

        if (column == dbf.total_columns && pkgi_validate_url(dbf.data[ColumnUrl].data))
        {
            uint32_t ctype = (uint32_t)pkgi_strtoll(dbf.data[ColumnContentType].data);
            // contentid �� ����� ���� ������, ������� �������� ���
            db[db_count].content = (dbf.data[ColumnContentId].data[0] == 0 ? generate_contentid() : dbf.data[ColumnContentId].data);
            db[db_count].type = pkgi_get_content_type(ctype == 0 ? db_id : ctype);
            db[db_count].name = dbf.data[ColumnName].data;
            db[db_count].description = dbf.data[ColumnDescription].data;
            db[db_count].rap = pkgi_hexbytes(dbf.data[ColumnRap].data, PKGI_RAP_SIZE);
            db[db_count].url = dbf.data[ColumnUrl].data;
            db[db_count].size = pkgi_strtoll(dbf.data[ColumnSize].data);
            db[db_count].digest = pkgi_hexbytes(dbf.data[ColumnChecksum].data, SHA256_DIGEST_SIZE);
            db_item[db_count] = db + db_count;
            db_count++;
        }

        if (db_count == MAX_DB_ITEMS)
        {
            break;
        }

        if (ptr < end && *ptr == '\r')
        {
            ptr++;
        }
        if (ptr < end && *ptr == '\n')
        {
            ptr++;
        }
    }

    LOG("������ ��������, ����� �������: %u", (db_count - db_item_count));

    db_item_count = db_count;

    return 1;
}

int pkgi_db_update(const char* update_url, uint32_t update_len, char* error, uint32_t error_size)
{
    char path[256];
    int i;

    for (i = 0; i < MAX_CONTENT_TYPES; i++)
    {
        const char* tmp_url = update_url + update_len*i;

        if (tmp_url[0] != 0)
        {
            pkgi_snprintf(path, sizeof(path), "%s/pkgi%s.txt", pkgi_get_config_folder(), pkgi_content_tag(i));
            update_database(tmp_url, path, error, error_size);
        }
    }

    return 1;
}

int pkgi_db_reload(char* error, uint32_t error_size)
{
    db_total = 0;
    db_size = 0;
    db_count = 0;
    db_item_count = 0;

    char path[256];
    int i;

    for (i = 0; i < MAX_CONTENT_TYPES; i++)
    {
        pkgi_snprintf(path, sizeof(path), "%s/pkgi%s.txt", pkgi_get_config_folder(), pkgi_content_tag(i));

        if (pkgi_get_size(path) > 0)
        {
            load_database(i);
        }
    }

    LOG("��������� ���������� ���� ������, ����� �������: %u", db_count);

    if (db_count == 0)
    {
        pkgi_snprintf(error, error_size, "������: pkgi.txt ����(�) ����������� ��� �������� ���� config.txt");
        return 0;
    }
    return 1;
}

static void swap(uint32_t a, uint32_t b)
{
    DbItem* temp = db_item[a];
    db_item[a] = db_item[b];
    db_item[b] = temp;
}

static int matches(GameRegion region, ContentType content, uint32_t filter)
{
    return ((region == RegionASA && (filter & DbFilterRegionASA))
        || (region == RegionEUR && (filter & DbFilterRegionEUR))
        || (region == RegionJPN && (filter & DbFilterRegionJPN))
        || (region == RegionUSA && (filter & DbFilterRegionUSA))
        || (region == RegionUnknown))

        && ((content == ContentGame && (filter & DbFilterContentGame))
        || (content == ContentRUS && (filter & DbFilterContentRUS))
        || (content == ContentPS2 && (filter & DbFilterContentPS2))
        || (content == ContentPS1 && (filter & DbFilterContentPS1))
        || (content == ContentMinis && (filter & DbFilterContentMinis))
        || (content == ContentDLC && (filter & DbFilterContentDLC))
        || (content == ContentTheme && (filter & DbFilterContentTheme))
        || (content == ContentAvatar && (filter & DbFilterContentAvatar))
        || (content == ContentDemo && (filter & DbFilterContentDemo))
        || (content == ContentManager && (filter & DbFilterContentManager))
        || (content == ContentApp && (filter & DbFilterContentApp))
        || (content == ContentCheat && (filter & DbFilterContentCheat))
        || (content == ContentUnknown));
}

static int lower(const DbItem* a, const DbItem* b, DbSort sort, DbSortOrder order, uint32_t filter)
{
    GameRegion reg_a = pkgi_get_region(a->content);
    GameRegion reg_b = pkgi_get_region(b->content);

    int cmp = 0;
    if (sort == SortByTitle)
    {
        cmp = pkgi_stricmp(a->content + 7, b->content + 7) < 0;
    }
    else if (sort == SortByRegion)
    {
        cmp = reg_a == reg_b ? pkgi_stricmp(a->content + 7, b->content + 7) < 0 : reg_a < reg_b;
    }
    else if (sort == SortByName)
    {
        cmp = pkgi_stricmp(a->name, b->name) < 0;
    }
    else if (sort == SortBySize)
    {
        cmp = a->size < b->size;
    }

    int matches_a = matches(reg_a, a->type, filter);
    int matches_b = matches(reg_b, b->type, filter);

    if (matches_a == matches_b)
    {
        return order == SortAscending ? cmp : !cmp;
    }
    else if (matches_a)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void heapify(uint32_t n, uint32_t index, DbSort sort, DbSortOrder order, uint32_t filter)
{
    uint32_t largest = index;
    uint32_t left = 2 * index + 1;
    uint32_t right = 2 * index + 2;

    if (left < n && lower(db_item[largest], db_item[left], sort, order, filter))
    {
        largest = left;
    }

    if (right < n && lower(db_item[largest], db_item[right], sort, order, filter))
    {
        largest = right;
    }

    if (largest != index)
    {
        swap(index, largest);
        heapify(n, largest, sort, order, filter);
    }
}

void pkgi_db_configure(const char* search, const Config* config)
{
    uint32_t search_count;
    if (!search)
    {
        search_count = db_count;
    }
    else
    {
        uint32_t write = 0;
        for (uint32_t read = 0; read < db_count; read++)
        {
            if (pkgi_stricontains(db_item[read]->name, search))
            {
                if (write < read)
                {
                    swap(read, write);
                }
                write++;
            }
        }
        search_count = write;
    }

    if (search_count == 0)
    {
        db_item_count = 0;
        return;
    }

    for (int i = search_count / 2 - 1; i >= 0; i--)
    {
        heapify(search_count, i, config->sort, config->order, config->filter);
    }

    for (int i = search_count - 1; i >= 0; i--)
    {
        swap(i, 0);
        heapify(i, 0, config->sort, config->order, config->filter);
    }

    if (config->filter == DbFilterAll)
    {
        db_item_count = search_count;
    }
    else
    {
        uint32_t low = 0;
        uint32_t high = search_count - 1;
        while (low <= high)
        {
            // ��� ������� �� ������������� ��-�� MAX_DB_ITEMS
            uint32_t middle = (low + high) / 2;

            GameRegion region = pkgi_get_region(db_item[middle]->content);
            if (matches(region, db_item[middle]->type, config->filter))
            {
                low = middle + 1;
            }
            else
            {
                if (middle == 0)
                {
                    break;
                }
                high = middle - 1;
            }
        }
        db_item_count = low;
    }
}

void pkgi_db_get_update_status(uint32_t* updated, uint32_t* total)
{
    *updated = db_size;
    *total = db_total;
}

uint32_t pkgi_db_count(void)
{
    return db_item_count;
}

uint32_t pkgi_db_total(void)
{
    return db_count;
}

DbItem* pkgi_db_get(uint32_t index)
{
    return index < db_item_count ? db_item[index] : NULL;
}

GameRegion pkgi_get_region(const char* content)
{
    switch (content[9])
    {
    case 'A':
    case 'K':
    case 'H':
        return RegionASA;

    case 'E':
        return RegionEUR;

    case 'J':
        return RegionJPN;

    case 'U':
        return RegionUSA;
	}

	switch (content[0])
	{
	case 'A':
	case 'K':
	case 'H':
		return RegionASA;

	case 'E':
		return RegionEUR;

	case 'J':
		return RegionJPN;

	case 'U':
		return RegionUSA;

	default:
		return RegionUnknown;
	}
}

ContentType pkgi_get_content_type(uint32_t content)
{
    if (content < MAX_CONTENT_TYPES)
        return (ContentType)content;

    return ContentUnknown;
}
