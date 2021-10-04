#include "pkgi.h"
#include "pkgi_db.h"
#include "pkgi_menu.h"
#include "pkgi_config.h"
#include "pkgi_dialog.h"
#include "pkgi_download.h"
#include "pkgi_utils.h"
#include "pkgi_style.h"
#include "pkgi_utf8.h"

#include <stddef.h>

#define content_filter(c)   (c ? 1 << (7 + c) : DbFilterAllContent)

typedef enum  {
    StateError,
    StateRefreshing,
    StateUpdateDone,
    StateMain,
    StateTerminate
} State;

static State state;

static uint32_t first_item;
static uint32_t selected_item;

static int search_active;

static char refresh_url[MAX_CONTENT_TYPES][256];

static Config config;
static Config config_temp;

static int font_height;
static int avail_height;
static int bottom_y;

static char search_text[256];
static char error_state[256];

static void reposition(void);

static const char* pkgi_get_ok_str(void)
{
    return pkgi_ok_button() == PKGI_BUTTON_X ? PKGI_UTF8_X : PKGI_UTF8_O;
}

static const char* pkgi_get_cancel_str(void)
{
    return pkgi_cancel_button() == PKGI_BUTTON_O ? PKGI_UTF8_O : PKGI_UTF8_X;
}

static void pkgi_refresh_thread(void)
{
    LOG("starting update");

    if (pkgi_menu_result() == MenuResultRefresh)
    {
        pkgi_db_update((char*) &refresh_url, sizeof(refresh_url[0]), error_state, sizeof(error_state));
    }

    if (pkgi_db_reload(error_state, sizeof(error_state)))
    {
        first_item = 0;
        selected_item = 0;
        state = StateUpdateDone;
    }
    else
    {
        state = StateError;
    }
    
    pkgi_thread_exit();
}

static int install(const char* content)
{
    LOG("���������...");
    pkgi_dialog_start_progress("���������", "���������� ���������...", -1);

    char titleid[10];
    pkgi_memcpy(titleid, content + 7, 9);
    titleid[9] = 0;

    pkgi_dialog_allow_close(0);
    int ok = pkgi_install(titleid);
    pkgi_dialog_allow_close(1);

    if (!ok)
    {
        pkgi_dialog_error("��������� �� �������");
        return 0;
    }

    LOG("��������� ��������� �������");

    return 1;
}

static void pkgi_download_thread(void)
{
    DbItem* item = pkgi_db_get(selected_item);

    LOG("����� ������ ��������");

    // �������� �������� ��� ������� �������� ����������� ���� ��������
    pkgi_sleep(300);

    pkgi_lock_process();
    if (pkgi_download(item, config.dl_mode_background))
    {
        if (!config.dl_mode_background)
        {
            install(item->content);
            pkgi_dialog_message(item->name, "�������� ���������!");
        }
        else
        {
            pkgi_dialog_message(item->name, "������ ��������� � ������� (����� ����� ������������)");
        }
        LOG("�������� ���������!");
    }
    pkgi_unlock_process();

    if (pkgi_dialog_is_cancelled())
    {
        pkgi_dialog_close();
    }

    item->presence = PresenceUnknown;
    state = StateMain;

    pkgi_thread_exit();
}

static uint32_t friendly_size(uint64_t size)
{
    if (size > 10ULL * 1000 * 1024 * 1024)
    {
        return (uint32_t)(size / (1024 * 1024 * 1024));
    }
    else if (size > 10 * 1000 * 1024)
    {
        return (uint32_t)(size / (1024 * 1024));
    }
    else if (size > 10 * 1000)
    {
        return (uint32_t)(size / 1024);
    }
    else
    {
        return (uint32_t)size;
    }
}

static const char* friendly_size_str(uint64_t size)
{
    if (size > 10ULL * 1000 * 1024 * 1024)
    {
        return "��";
    }
    else if (size > 10 * 1000 * 1024)
    {
        return "��";
    }
    else if (size > 10 * 1000)
    {
        return "��";
    }
    else
    {
        return "�";
    }
}

int pkgi_check_free_space(uint64_t size)
{
    uint64_t free = pkgi_get_free_space();
    if (size > free + 1024 * 1024)
    {
        char error[256];
        pkgi_snprintf(error, sizeof(error), "��������� %u %s ���������� �����, �������� %u %s",
            friendly_size(size), friendly_size_str(size),
            friendly_size(free), friendly_size_str(free)
        );

        pkgi_dialog_error(error);
        return 0;
    }

    return 1;
}

static void pkgi_friendly_size(char* text, uint32_t textlen, int64_t size)
{
    if (size <= 0)
    {
        text[0] = 0;
    }
    else if (size < 1000LL)
    {
        pkgi_snprintf(text, textlen, "%u " PKGI_UTF8_B, (uint32_t)size);
    }
    else if (size < 1000LL * 1000)
    {
        pkgi_snprintf(text, textlen, "%.2f " PKGI_UTF8_KB, size / 1024.f);
    }
    else if (size < 1000LL * 1000 * 1000)
    {
        pkgi_snprintf(text, textlen, "%.2f " PKGI_UTF8_MB, size / 1024.f / 1024.f);
    }
    else
    {
        pkgi_snprintf(text, textlen, "%.2f " PKGI_UTF8_GB, size / 1024.f / 1024.f / 1024.f);
    }
}

static const char* content_type_str(ContentType content)
{
    switch (content)
    {
    case ContentGame: return "Game";
    case ContentRUS: return "RUS";
    case ContentPS2: return "PS2";
    case ContentPS1: return "PS1";
    case ContentMinis: return "miniS";
    case ContentDLC: return "DLC";
    case ContentTheme: return "Theme";
    case ContentAvatar: return "Avatar";
    case ContentDemo: return "Demo";
    case ContentManager: return "Manager";
    case ContentApp: return "App";
    case ContentCheat: return "Cheat";
    default: return "Unknown";
    }
}
static const char* content_type_str2(ContentType content)
{
    switch (content)
    {
    case ContentGame: return "GAM";
    case ContentRUS: return "RUS";
    case ContentPS2: return "PS2";
    case ContentPS1: return "PS1";
    case ContentMinis: return "MNS";
    case ContentDLC: return "DLC";
    case ContentTheme: return "THM";
    case ContentAvatar: return "AVA";
    case ContentDemo: return "DEM";
    case ContentManager: return "MGR";
    case ContentApp: return "APP";
    case ContentCheat: return "CHT";
    default: return "UNK";
    }
}

static void pkgi_do_main(pkgi_input* input)
{
    int col_titleid = 0;
    int col_content = col_titleid + pkgi_text_width("PCSE00000") + PKGI_MAIN_COLUMN_PADDING;
    int col_installed = col_content + pkgi_text_width("RUS") + PKGI_MAIN_COLUMN_PADDING;
    int col_name = col_installed + pkgi_text_width(PKGI_UTF8_INSTALLED) + PKGI_MAIN_COLUMN_PADDING;

    uint32_t db_count = pkgi_db_count();
    
    if (input)
    {
        if (input->active & pkgi_cancel_button()) {
            input->pressed &= ~pkgi_cancel_button();
            if (pkgi_msg_dialog(MDIALOG_YESNO, MEXIT))
                state = StateTerminate;
        }

        if (input->active & PKGI_BUTTON_SELECT) {
            input->pressed &= ~PKGI_BUTTON_SELECT;

            pkgi_msg_dialog(MDIALOG_OK, M1ABOUT PKGI_VERSION M2ABOUT);
        }

        if (input->active & PKGI_BUTTON_L2)
        {
            config.filter ^= content_filter(config.content);

            if (config.content == 0)
                config.content = MAX_CONTENT_TYPES;

            config.content--;
            config.filter ^= content_filter(config.content);

            pkgi_db_configure(search_active ? search_text : NULL, &config);
            reposition();
            db_count = pkgi_db_count();
        }

        if (input->active & PKGI_BUTTON_R2)
        {
            config.filter ^= content_filter(config.content);

            config.content++;
            if (config.content == MAX_CONTENT_TYPES)
                config.content = 0;

            config.filter ^= content_filter(config.content);

            pkgi_db_configure(search_active ? search_text : NULL, &config);
            reposition();
            db_count = pkgi_db_count();
        }

        if (input->active & PKGI_BUTTON_UP)
        {
            if (selected_item == first_item && first_item > 0)
            {
                first_item--;
                selected_item = first_item;
            }
            else if (selected_item > 0)
            {
                selected_item--;
            }
            else if (selected_item == 0)
            {
                selected_item = db_count - 1;
                uint32_t max_items = avail_height / (font_height + PKGI_MAIN_ROW_PADDING) - 1;
                first_item = db_count > max_items ? db_count - max_items - 1 : 0;
            }
        }

        if (input->active & PKGI_BUTTON_DOWN)
        {
            uint32_t max_items = avail_height / (font_height + PKGI_MAIN_ROW_PADDING) - 1;
            if (selected_item == db_count - 1)
            {
                selected_item = first_item = 0;
            }
            else if (selected_item == first_item + max_items)
            {
                first_item++;
                selected_item++;
            }
            else
            {
                selected_item++;
            }
        }
        
        if (input->active & PKGI_BUTTON_LT)
        {
            uint32_t max_items = avail_height / (font_height + PKGI_MAIN_ROW_PADDING) - 1;
            if (first_item < max_items)
            {
                first_item = 0;
            }
            else
            {
                first_item -= max_items;
            }
            if (selected_item < max_items)
            {
                selected_item = 0;
            }
            else
            {
                selected_item -= max_items;
            }
        }

        if (input->active & PKGI_BUTTON_RT)
        {
            uint32_t max_items = avail_height / (font_height + PKGI_MAIN_ROW_PADDING) - 1;
            if (first_item + max_items < db_count - 1)
            {
                first_item += max_items;
                selected_item += max_items;
                if (selected_item >= db_count)
                {
                    selected_item = db_count - 1;
                }
            }
        }
    }
    
    int y = font_height*3/2 + PKGI_MAIN_HLINE_EXTRA;
    int line_height = font_height + PKGI_MAIN_ROW_PADDING;
    for (uint32_t i = first_item; i < db_count; i++)
    {
        DbItem* item = pkgi_db_get(i);

        if (i == selected_item)
        {
            pkgi_draw_fill_rect_z(0, y, PKGI_FONT_Z, VITA_WIDTH, font_height + PKGI_MAIN_ROW_PADDING - 1, PKGI_COLOR_SELECTED_BACKGROUND);
        }
        uint32_t color = PKGI_COLOR_TEXT;

        char titleid[10];
        pkgi_memcpy(titleid, item->content + 7, 9);
        titleid[9] = 0;

        if (item->presence == PresenceUnknown)
        {
            item->presence = pkgi_is_incomplete(titleid) ? PresenceIncomplete : pkgi_is_installed(titleid) ? PresenceInstalled : PresenceMissing;
        }

        char size_str[64];
        pkgi_friendly_size(size_str, sizeof(size_str), item->size);
        int sizew = pkgi_text_width(size_str);

        pkgi_clip_set(0, y, VITA_WIDTH, line_height);
        pkgi_draw_text(col_titleid, y, color, titleid);
        const char* ct;
        ct = content_type_str2(item->type);
        pkgi_draw_text(col_content, y, color, ct);

        if (item->presence == PresenceIncomplete)
        {
            pkgi_draw_text(col_installed, y, color, PKGI_UTF8_PARTIAL);
        }
        else if (item->presence == PresenceInstalled)
        {
            pkgi_draw_text(col_installed, y, color, PKGI_UTF8_INSTALLED);
        }
        pkgi_draw_text(VITA_WIDTH - PKGI_MAIN_SCROLL_WIDTH - PKGI_MAIN_SCROLL_PADDING - sizew, y, color, size_str);
        pkgi_clip_remove();

        pkgi_clip_set(col_name, y, VITA_WIDTH - PKGI_MAIN_SCROLL_WIDTH - PKGI_MAIN_SCROLL_PADDING - PKGI_MAIN_COLUMN_PADDING - sizew - col_name, line_height);
        pkgi_draw_text_ttf(0, 0, PKGI_FONT_Z, color, item->name);
        pkgi_clip_remove();

        y += font_height + PKGI_MAIN_ROW_PADDING;
        if (y > VITA_HEIGHT - (font_height + PKGI_MAIN_HLINE_EXTRA))
        {
            break;
        }
        else if (y + font_height > VITA_HEIGHT - (font_height + PKGI_MAIN_HLINE_EXTRA))
        {
            line_height = (VITA_HEIGHT - (font_height + PKGI_MAIN_HLINE_EXTRA)) - (y + 1);
            if (line_height < PKGI_MAIN_ROW_PADDING)
            {
                break;
            }
        }
    }

    if (db_count == 0)
    {
        const char* text = "������ ����!";

        int w = pkgi_text_width(text);
        pkgi_draw_text((VITA_WIDTH - w) / 2, VITA_HEIGHT / 2, PKGI_COLOR_TEXT, text);
    }

    // scroll-bar
    if (db_count != 0)
    {
        uint32_t max_items = (avail_height + font_height + PKGI_MAIN_ROW_PADDING - 1) / (font_height + PKGI_MAIN_ROW_PADDING) - 1;
        if (max_items < db_count)
        {
            uint32_t min_height = PKGI_MAIN_SCROLL_MIN_HEIGHT;
            uint32_t height = max_items * avail_height / db_count;
            uint32_t start = first_item * (avail_height - (height < min_height ? min_height : 0)) / db_count;
            height = max32(height, min_height);
            pkgi_draw_fill_rect_z(VITA_WIDTH - PKGI_MAIN_SCROLL_WIDTH - 1, font_height + PKGI_MAIN_HLINE_EXTRA + start, PKGI_FONT_Z, PKGI_MAIN_SCROLL_WIDTH, height, PKGI_COLOR_SCROLL_BAR);
        }
    }

    if (input && (input->pressed & pkgi_ok_button()))
    {
        input->pressed &= ~pkgi_ok_button();

        DbItem* item = pkgi_db_get(selected_item);

        if ((item->presence == PresenceInstalled) && pkgi_msg_dialog(MDIALOG_YESNO, MINST))
        {
            LOG("[%.9s] %s - ��� �����������", item->content + 7, item->name);
            item->presence = PresenceMissing;
        }

        if (item->presence == PresenceIncomplete || (item->presence == PresenceMissing && pkgi_check_free_space(item->size)))
        {
            LOG("[%.9s] %s - ������ ���������", item->content + 7, item->name);
            pkgi_dialog_start_progress("����������...", "����������...", 0);
            pkgi_start_thread("download_thread", &pkgi_download_thread);
        }
    }
    else if (input && (input->pressed & PKGI_BUTTON_T))
    {
        input->pressed &= ~PKGI_BUTTON_T;

        config_temp = config;

        pkgi_menu_start(search_active, &config);
    }
    else if (input && (input->active & PKGI_BUTTON_S))
    {
        input->pressed &= ~PKGI_BUTTON_S;

        DbItem* item = pkgi_db_get(selected_item);
        char item_info[256];

        const char* region;
        switch (pkgi_get_region(item->content))
        {
        case RegionASA: region = "ASA"; break;
        case RegionEUR: region = "EUR"; break;
        case RegionJPN: region = "JPN"; break;
        case RegionUSA: region = "USA"; break;
        default: region = "???"; break;
        }

        pkgi_snprintf(item_info, sizeof(item_info), "ID: %s\nRAP: (%s)       SHA-256: (%s)\n������: %s    �������: %s", 
            item->content,
            (item->rap ? PKGI_UTF8_CHECK_ON : PKGI_UTF8_CHECK_OFF),
            (item->digest ? PKGI_UTF8_CHECK_ON : PKGI_UTF8_CHECK_OFF),
            region,
            content_type_str(item->type));

        pkgi_download_icon(item->content);
        pkgi_dialog_details(item->name, item_info, item->description);
    }
}

static void pkgi_do_refresh(void)
{
    char text[256];

    uint32_t updated;
    uint32_t total;
    pkgi_db_get_update_status(&updated, &total);

    if (total == 0)
    {
        pkgi_snprintf(text, sizeof(text), "����������... %.2f ��", (uint32_t)updated / 1024.f);
    }
    else
    {
        pkgi_snprintf(text, sizeof(text), "����������... %u%%", updated * 100U / total);
    }

    int w = pkgi_text_width(text);
    pkgi_draw_text((VITA_WIDTH - w) / 2, VITA_HEIGHT / 2, PKGI_COLOR_TEXT, text);
}

static void pkgi_do_head(void)
{
    char title[256];
    pkgi_snprintf(title, sizeof(title), "PKGi PS3 RUS MOD v%s", PKGI_VERSION);
    pkgi_draw_text(0, 0, PKGI_COLOR_TEXT_HEAD, title);

    pkgi_draw_fill_rect(0, font_height, VITA_WIDTH, PKGI_MAIN_HLINE_HEIGHT, PKGI_COLOR_HLINE);

    char battery[256];
    pkgi_snprintf(battery, sizeof(battery), "CPU: %u""\xa9""C RSX: %u""\xa9""C", pkgi_get_temperature(0), pkgi_get_temperature(1));

    uint32_t color;
    if (pkgi_temperature_is_high())
    {
        color = PKGI_COLOR_BATTERY_LOW;
    }
    else
    {
        color = PKGI_COLOR_BATTERY_CHARGING;
    }

    int rightw = pkgi_text_width(battery);
    pkgi_draw_text(VITA_WIDTH - PKGI_MAIN_HLINE_EXTRA - rightw, 0, color, battery);

    if (search_active)
    {
        char text[256];
        int left = pkgi_text_width(search_text) + PKGI_MAIN_TEXT_PADDING;
        int right = rightw + PKGI_MAIN_TEXT_PADDING;

        pkgi_snprintf(text, sizeof(text), "�����: %s", search_text);

        pkgi_clip_set(left, 0, VITA_WIDTH - right - left, font_height + PKGI_MAIN_HLINE_EXTRA);
        pkgi_draw_text((VITA_WIDTH - pkgi_text_width(text)) / 2, 0, PKGI_COLOR_TEXT_TAIL, text);
        pkgi_clip_remove();
    }
}

static void pkgi_do_tail(void)
{
    pkgi_draw_fill_rect_z(0, bottom_y - font_height/2, PKGI_FONT_Z, VITA_WIDTH, PKGI_MAIN_HLINE_HEIGHT, PKGI_COLOR_HLINE);

    uint32_t count = pkgi_db_count();
    uint32_t total = pkgi_db_total();

    char text[256];
    if (count == total)
    {
        pkgi_snprintf(text, sizeof(text), "�����: %u", count);
    }
    else
    {
        pkgi_snprintf(text, sizeof(text), "�����: %u (%u)", count, total);
    }
    pkgi_draw_text(0, bottom_y, PKGI_COLOR_TEXT_TAIL, text);

    char size[64];
    pkgi_friendly_size(size, sizeof(size), pkgi_get_free_space());

    char free_str[64];
    pkgi_snprintf(free_str, sizeof(free_str), "��������: %s", size);

    int rightw = pkgi_text_width(free_str);
    pkgi_draw_text(VITA_WIDTH - PKGI_MAIN_HLINE_EXTRA - rightw, bottom_y, PKGI_COLOR_TEXT_TAIL, free_str);

    int left = pkgi_text_width(text) + PKGI_MAIN_TEXT_PADDING;
    int right = rightw + PKGI_MAIN_TEXT_PADDING;

    if (pkgi_menu_is_open())
    {
        pkgi_snprintf(text, sizeof(text), "%s �����  " PKGI_UTF8_T " ���������  %s ������", pkgi_get_ok_str(), pkgi_get_cancel_str());
    }
    else
    {
        pkgi_snprintf(text, sizeof(text), "%s �������  " PKGI_UTF8_T " ����  " PKGI_UTF8_S " ������  %s �����", pkgi_get_ok_str(), pkgi_get_cancel_str());
    }

    pkgi_clip_set(left, bottom_y, VITA_WIDTH - right - left, VITA_HEIGHT - bottom_y);
    pkgi_draw_text_z((VITA_WIDTH - pkgi_text_width(text)) / 2, bottom_y, PKGI_FONT_Z, PKGI_COLOR_TEXT_TAIL, text);
    pkgi_clip_remove();
}

static void pkgi_do_error(void)
{
    pkgi_draw_text((VITA_WIDTH - pkgi_text_width(error_state)) / 2, VITA_HEIGHT / 2, PKGI_COLOR_TEXT_ERROR, error_state);
}

static void reposition(void)
{
    uint32_t count = pkgi_db_count();
    if (first_item + selected_item < count)
    {
        return;
    }

    uint32_t max_items = (avail_height + font_height + PKGI_MAIN_ROW_PADDING - 1) / (font_height + PKGI_MAIN_ROW_PADDING) - 1;
    if (count > max_items)
    {
        uint32_t delta = selected_item - first_item;
        first_item = count - max_items;
        selected_item = first_item + delta;
    }
    else
    {
        first_item = 0;
        selected_item = 0;
    }
}

static void pkgi_update_check_thread(void)
{
    LOG("�������� ���������� PKGi �� %s", PKGI_UPDATE_URL);

    pkgi_http* http = pkgi_http_get(PKGI_UPDATE_URL, NULL, 0);
    if (!http)
    {
        LOG("HTTP-������ � %s �� ������", PKGI_UPDATE_URL);
        pkgi_thread_exit();
    }

    int64_t sz;
    pkgi_http_response_length(http, &sz);

    char buffer[8192];
    uint32_t size = 0;

    while (size < sizeof(buffer) - 1)
    {
        int read = pkgi_http_read(http, buffer + size, sizeof(buffer) - 1 - size);
        if (read < 0)
        {
            size = 0;
            break;
        }
        else if (read == 0)
        {
            break;
        }
        size += read;
    }

    if (size != 0)
    {
        LOG("�������� %u ����", size);
    }
    buffer[size] = 0;

    pkgi_http_close(http);

    static const char find[] = "\"name\":\"PKGi PS3 RUS MOD v";
    const char* start = pkgi_strstr(buffer, find);
    if (!start)
    {
        LOG("��� �� �������");
        pkgi_thread_exit();
    }

    LOG("��� �������");
    start += sizeof(find) - 1;

    char* end = pkgi_strstr(start, "\"");
    if (!end)
    {
        LOG("�� ������� ����� �����");
        pkgi_thread_exit();
    }

    *end = 0;
    LOG("��������� ������ %s", start);

    if (pkgi_stricmp(PKGI_VERSION, start) == 0)
    {
        LOG("���������� �� ����������");
        pkgi_thread_exit();
    }

	start = pkgi_strstr(end+1, "\"browser_download_url\":\"");
	if (!start)
	{
		LOG("URL ��� �������� �� ������");
        pkgi_thread_exit();
	}

	start += 24;
	end = pkgi_strstr(start, "\"");
	if (!end)
	{
		LOG("URL ��� �������� �� ������");
        pkgi_thread_exit();
	}

	*end = 0;
	LOG("URL �������� %s", start);

    DbItem update_item = {
        .content = "UP0001-NP00PKGI3_00-0000000000000000",
        .name    = "PKGi PS3 RUS MOD Update",
        .url     = start,
    };

    pkgi_dialog_start_progress(update_item.name, "����������...", 0);
    
    if (pkgi_download(&update_item, 0) && install(update_item.content))
    {
        pkgi_dialog_message(update_item.name, "������� ������� ���������� PKGi PS3 RUS MOD");
        LOG("���������� �������!");
    }

    pkgi_thread_exit();
}

int main(int argc, const char* argv[])
{
    pkgi_start();

    pkgi_load_config(&config, (char*) &refresh_url, sizeof(refresh_url[0]));
    if (config.music)
    {
        pkgi_start_music();
    }
    
    pkgi_dialog_init();
    
    font_height = pkgi_text_height("M");
    avail_height = VITA_HEIGHT - 2 * (font_height + PKGI_MAIN_HLINE_EXTRA);
    bottom_y = VITA_HEIGHT - PKGI_MAIN_ROW_PADDING;

    state = StateRefreshing;
    pkgi_start_thread("refresh_thread", &pkgi_refresh_thread);

    pkgi_texture background = pkgi_load_image_buffer(background, png);

    if (config.version_check)
    {
        pkgi_start_thread("update_thread", &pkgi_update_check_thread);
    }

    pkgi_input input = {0, 0, 0, 0};
    while (pkgi_update(&input) && (state != StateTerminate))
    {
        pkgi_draw_background(background);

        if (state == StateUpdateDone)
        {
            pkgi_db_configure(NULL, &config);
            state = StateMain;
        }

        pkgi_do_head();
        switch (state)
        {
        case StateError:
            pkgi_do_error();
            // �������� ���� ��������, ���� ���� ������ ���, � URL-������ ��������
            if (!pkgi_menu_is_open() && config.allow_refresh)
            {
                config_temp = config;
                pkgi_menu_start(search_active, &config);
            }            
            break;

        case StateRefreshing:
            pkgi_do_refresh();
            break;

        case StateMain:
            pkgi_do_main(pkgi_dialog_is_open() || pkgi_menu_is_open() ? NULL : &input);
            break;

        default:
            // ������� �� ������, ������ ����� �������� ����������
            break;
        }

        pkgi_do_tail();

        if (pkgi_dialog_is_open())
        {
            pkgi_do_dialog(&input);
        }

        if (pkgi_dialog_input_update())
        {
            search_active = 1;
            pkgi_dialog_input_get_text(search_text, sizeof(search_text));
            pkgi_db_configure(search_text, &config);
            reposition();
        }

        if (pkgi_menu_is_open())
        {
            if (pkgi_do_menu(&input))
            {
                Config new_config;
                pkgi_menu_get(&new_config);
                if (config_temp.sort != new_config.sort ||
                    config_temp.order != new_config.order ||
                    config_temp.filter != new_config.filter)
                {
                    config_temp = new_config;
                    pkgi_db_configure(search_active ? search_text : NULL, &config_temp);
                    reposition();
                }
                else if (config_temp.music != new_config.music)
                {
                    config_temp = new_config;
                    (config_temp.music ? pkgi_start_music() : pkgi_stop_music());
                }
            }
            else
            {
                MenuResult mres = pkgi_menu_result();
                if (mres == MenuResultSearch)
                {
                    pkgi_dialog_input_text("Search", search_text);
                }
                else if (mres == MenuResultSearchClear)
                {
                    search_active = 0;
                    search_text[0] = 0;
                    pkgi_db_configure(NULL, &config);
                }
                else if (mres == MenuResultCancel)
                {
                    if (config_temp.sort != config.sort || config_temp.order != config.order || config_temp.filter != config.filter)
                    {
                        pkgi_db_configure(search_active ? search_text : NULL, &config);
                        reposition();
                    }
                    if (config_temp.music != config.music)
                    {
                        (config.music ? pkgi_start_music() : pkgi_stop_music());
                    }
                }
                else if (mres == MenuResultAccept)
                {
                    pkgi_menu_get(&config);
                    pkgi_save_config(&config, (char*) &refresh_url, sizeof(refresh_url[0]));
                }
                else if (mres == MenuResultRefresh)
                {
                    state = StateRefreshing;
                    pkgi_start_thread("refresh_thread", &pkgi_refresh_thread);
                }
            }
        }

        pkgi_swap();
    }

    LOG("���������");
    pkgi_free_texture(background);
    pkgi_end();
	return 0;
}
