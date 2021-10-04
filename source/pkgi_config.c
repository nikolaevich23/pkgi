#include "pkgi_config.h"
#include "pkgi.h"

static char* skipnonws(char* text, char* end)
{
    while (text < end && *text != ' ' && *text != '\n' && *text != '\r')
    {
        text++;
    }
    return text;
}

static char* skipws(char* text, char* end)
{
    while (text < end && (*text == ' ' || *text == '\n' || *text == '\r'))
    {
        text++;
    }
    return text;
}

static DbSort parse_sort(const char* value, DbSort sort)
{
    if (pkgi_stricmp(value, "title") == 0)
    {
        return SortByTitle;
    }
    else if (pkgi_stricmp(value, "region") == 0)
    {
        return SortByRegion;
    }
    else if (pkgi_stricmp(value, "name") == 0)
    {
        return SortByName;
    }
    else if (pkgi_stricmp(value, "size") == 0)
    {
        return SortBySize;
    }
    else
    {
        return sort;
    }
}

static DbSortOrder parse_order(const char* value, DbSortOrder order)
{
    if (pkgi_stricmp(value, "asc") == 0)
    {
        return SortAscending;
    }
    else if (pkgi_stricmp(value, "desc") == 0)
    {
        return SortDescending;
    }
    else
    {
        return order;
    }
}

static DbSortOrder parse_filter(char* value, uint32_t filter)
{
    uint32_t result = 0;

    char* start = value;
    for (;;)
    {
        char ch = *value;
        if (ch == 0 || ch == ',')
        {
            *value = 0;
            if (pkgi_stricmp(start, "ASA") == 0)
            {
                result |= DbFilterRegionASA;
            }
            else if (pkgi_stricmp(start, "EUR") == 0)
            {
                result |= DbFilterRegionEUR;
            }
            else if (pkgi_stricmp(start, "JPN") == 0)
            {
                result |= DbFilterRegionJPN;
            }
            else if (pkgi_stricmp(start, "USA") == 0)
            {
                result |= DbFilterRegionUSA;
            }
            else
            {
                return filter;
            }
            if (ch == 0)
            {
                break;
            }
            value++;
            start = value;
        }
        else
        {
            value++;
        }
    }

    return result;
}

void pkgi_load_config(Config* config, char* refresh_url, uint32_t refresh_len)
{
    refresh_url[0] = 0;
    config->sort = SortByName;
    config->order = SortAscending;
    config->filter = DbFilterAll;
    config->version_check = 1;
    config->dl_mode_background = 0;
    config->music = 1;
    config->content = 0;
    config->allow_refresh = 0;

    char data[4096];
    char path[256];
    pkgi_snprintf(path, sizeof(path), "%s/config.txt", pkgi_get_config_folder());
    LOG("config location: %s", path);

    int loaded = pkgi_load(path, data, sizeof(data) - 1);
    if (loaded > 0)
    {
        data[loaded] = '\n';

        LOG("config.txt loaded, parsing");
        char* text = data;
        char* end = data + loaded + 1;

        if (loaded > 3 && (uint8_t)text[0] == 0xef && (uint8_t)text[1] == 0xbb && (uint8_t)text[2] == 0xbf)
        {
            text += 3;
        }

        while (text < end)
        {
            char* key = text;

            text = skipnonws(text, end);
            if (text == end) break;

            *text++ = 0;

            text = skipws(text, end);
            if (text == end) break;

            char* value = text;

            text = skipnonws(text, end);
            if (text == end) break;

            *text++ = 0;

            text = skipws(text, end);

            if (pkgi_stricontains(key, "url"))
            {
                for (int i = 0; i < MAX_CONTENT_TYPES; i++)
                    if (pkgi_stricmp(key+3, pkgi_content_tag(i)) == 0)
                    {
                        pkgi_strncpy(refresh_url + refresh_len*i, refresh_len, value);
                        config->allow_refresh = 1;
                    }
            }
            else if (pkgi_stricmp(key, "sort") == 0)
            {
                config->sort = parse_sort(value, SortByName);
            }
            else if (pkgi_stricmp(key, "order") == 0)
            {
                config->order = parse_order(value, SortAscending);
            }
            else if (pkgi_stricmp(key, "filter") == 0)
            {
                config->filter = parse_filter(value, DbFilterAll);
            }
            else if (pkgi_stricmp(key, "no_version_check") == 0)
            {
                config->version_check = 0;
            }
            else if (pkgi_stricmp(key, "dl_mode_background") == 0)
            {
                config->dl_mode_background = 1;
            }
            else if (pkgi_stricmp(key, "no_music") == 0)
            {
                config->music = 0;
            }
            else if (pkgi_stricmp(key, "content") == 0)
            {
                config->content = (uint8_t)pkgi_strtoll(value);
            }
        }
    }
    else
    {
        LOG("config.txt �� ����� ���� �������� �� ���������� �� ���������");
    }
    if (config->content == 0)
    {
        config->filter |= DbFilterAllContent;
    }
    else
    {
        config->filter |= (128 << config->content);
    }
}

const char* pkgi_content_tag(ContentType content)
{
    switch (content)
    {
    case ContentGame: return "_games";
    case ContentRUS: return "_russian";
    case ContentPS2: return "_ps2";
    case ContentPS1: return "_ps1";
    case ContentMinis: return "_minis";
    case ContentDLC: return "_dlcs";
    case ContentTheme: return "_themes";
    case ContentAvatar: return "_avatars";
    case ContentDemo: return "_demos";
    case ContentManager: return "_managers";
    case ContentApp: return "_apps";
    case ContentCheat: return "_cheats";
    default: return "";
    }
}

static const char* sort_str(DbSort sort)
{
    switch (sort)
    {
    case SortByTitle: return "title";
    case SortByRegion: return "region";
    case SortByName: return "name";
    case SortBySize: return "size";
    default: return "";
    }
}

static const char* order_str(DbSortOrder order)
{
    switch (order)
    {
    case SortAscending: return "asc";
    case SortDescending: return "desc";
    default: return "";
    }
}

void pkgi_save_config(const Config* config, const char* update_url, uint32_t update_len)
{
    char data[4096];
    int len = 0;

    for (int i = 0; i < MAX_CONTENT_TYPES; i++)
    {
        const char* tmp_url = update_url + update_len*i;
        if (update_url && tmp_url[0] != 0)
        {
            len += pkgi_snprintf(data + len, sizeof(data) - len, "url%s %s\n", pkgi_content_tag(i), tmp_url);
        }
    }
    len += pkgi_snprintf(data + len, sizeof(data) - len, "content %d\n", config->content);
    len += pkgi_snprintf(data + len, sizeof(data) - len, "sort %s\n", sort_str(config->sort));
    len += pkgi_snprintf(data + len, sizeof(data) - len, "order %s\n", order_str(config->order));
    len += pkgi_snprintf(data + len, sizeof(data) - len, "filter ");
    const char* sep = "";
    if (config->filter & DbFilterRegionASA)
    {
        len += pkgi_snprintf(data + len, sizeof(data) - len, "%sASA", sep);
        sep = ",";
    }
    if (config->filter & DbFilterRegionEUR)
    {
        len += pkgi_snprintf(data + len, sizeof(data) - len, "%sEUR", sep);
        sep = ",";
    }
    if (config->filter & DbFilterRegionJPN)
    {
        len += pkgi_snprintf(data + len, sizeof(data) - len, "%sJPN", sep);
        sep = ",";
    }
    if (config->filter & DbFilterRegionUSA)
    {
        len += pkgi_snprintf(data + len, sizeof(data) - len, "%sUSA", sep);
        sep = ",";
    }
    len += pkgi_snprintf(data + len, sizeof(data) - len, "\n");

    if (!config->version_check)
    {
        len += pkgi_snprintf(data + len, sizeof(data) - len, "no_version_check 1\n");
    }

    if (config->dl_mode_background)
    {
        len += pkgi_snprintf(data + len, sizeof(data) - len, "dl_mode_background 1\n");
    }

    if (!config->music)
    {
        len += pkgi_snprintf(data + len, sizeof(data) - len, "no_music 1\n");
    }

    char path[256];
    pkgi_snprintf(path, sizeof(path), "%s/config.txt", pkgi_get_config_folder());

    if (pkgi_save(path, data, len))
    {
        LOG("config.txt �������");
    }
    else
    {
        LOG("�� ������� ��������� config.txt");
    }
}