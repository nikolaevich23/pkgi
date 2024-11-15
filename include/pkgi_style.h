#pragma once

#define VITA_WIDTH  848
#define VITA_HEIGHT 512

#define PKGI_COLOR(R, G, B)		(((R)<<16) | ((G)<<8) | (B))
#define RGBA_COLOR(C, ALPHA)	((C<<8) | ALPHA)


#define PKGI_UTF8_O "\xaa" // "\xfa" // "\xe2\x97\x8b" // 0x25cb
#define PKGI_UTF8_X "\xab" // "\xfb" // "\xe2\x95\xb3" // 0x2573
#define PKGI_UTF8_T "\xac" // "\xfc" // "\xe2\x96\xb3" // 0x25b3
#define PKGI_UTF8_S "\xad" // "\xfd" // "\xe2\x96\xa1" // 0x25a1


#define PKGI_UTF8_INSTALLED "\x04" //"\xe2\x97\x8f" // 0x25cf
#define PKGI_UTF8_PARTIAL   "\x09" //"\xe2\x97\x8b" // 0x25cb

#define PKGI_UTF8_B  "�"
#define PKGI_UTF8_KB "��" // "\xe3\x8e\x85" // 0x3385
#define PKGI_UTF8_MB "��" // "\xe3\x8e\x86" // 0x3386
#define PKGI_UTF8_GB "��" // "\xe3\x8e\x87" // 0x3387

#define PKGI_UTF8_CLEAR "\xaf" // 0x00d7

#define PKGI_UTF8_SORT_ASC  "\x1e" //"\xe2\x96\xb2" // 0x25b2
#define PKGI_UTF8_SORT_DESC "\x1f" //"\xe2\x96\xbc" // 0x25bc

#define PKGI_UTF8_CHECK_ON  "\x04"//"\xe2\x97\x8f" // 0x25cf
#define PKGI_UTF8_CHECK_OFF "\x09"//"\xe2\x97\x8b" // 0x25cb

#define PKGI_COLOR_DIALOG_BACKGROUND    PKGI_COLOR(48, 48, 48)
#define PKGI_COLOR_MENU_BORDER          PKGI_COLOR(80, 80, 255)
#define PKGI_COLOR_MENU_BACKGROUND      PKGI_COLOR(48, 48, 48)
#define PKGI_COLOR_TEXT_MENU            PKGI_COLOR(255, 255, 255)
#define PKGI_COLOR_TEXT_MENU_SELECTED   PKGI_COLOR(0, 255, 0)
#define PKGI_COLOR_TEXT                 PKGI_COLOR(255, 255, 255)
#define PKGI_COLOR_TEXT_HEAD            PKGI_COLOR(255, 255, 255)
#define PKGI_COLOR_TEXT_TAIL            PKGI_COLOR(255, 255, 255)
#define PKGI_COLOR_TEXT_DIALOG          PKGI_COLOR(255, 255, 255)
#define PKGI_COLOR_TEXT_ERROR           PKGI_COLOR(255, 50, 50)
#define PKGI_COLOR_TEXT_SHADOW          PKGI_COLOR(0, 0, 0)
#define PKGI_COLOR_HLINE                PKGI_COLOR(200, 200, 200)
#define PKGI_COLOR_SCROLL_BAR           PKGI_COLOR(255, 255, 255)
#define PKGI_COLOR_BATTERY_LOW          PKGI_COLOR(255, 50, 50)
#define PKGI_COLOR_BATTERY_CHARGING     PKGI_COLOR(50, 255, 50)
#define PKGI_COLOR_SELECTED_BACKGROUND  PKGI_COLOR(60, 60, 60)
#define PKGI_COLOR_PROGRESS_BACKGROUND  PKGI_COLOR(128, 128, 128)
#define PKGI_COLOR_PROGRESS_BAR         PKGI_COLOR(128, 255, 0)

#define PKGI_ANIMATION_SPEED 4000 // px/second

#define PKGI_FONT_Z      1000
#define PKGI_FONT_WIDTH  10
#define PKGI_FONT_HEIGHT 16
#define PKGI_FONT_SHADOW 2

#define PKGI_MAIN_COLUMN_PADDING    10
#define PKGI_MAIN_HLINE_EXTRA       5
#define PKGI_MAIN_ROW_PADDING       2
#define PKGI_MAIN_HLINE_HEIGHT      2
#define PKGI_MAIN_TEXT_PADDING      5
#define PKGI_MAIN_SCROLL_WIDTH      2
#define PKGI_MAIN_SCROLL_PADDING    2
#define PKGI_MAIN_SCROLL_MIN_HEIGHT 50

#define PKGI_DIALOG_TEXT_Z  800
#define PKGI_DIALOG_HMARGIN 100
#define PKGI_DIALOG_VMARGIN 150
#define PKGI_DIALOG_PADDING 30
#define PKGI_DIALOG_WIDTH (VITA_WIDTH - 2*PKGI_DIALOG_HMARGIN+80)
#define PKGI_DIALOG_HEIGHT (VITA_HEIGHT - 2*PKGI_DIALOG_VMARGIN)

#define PKGI_DIALOG_PROCESS_BAR_HEIGHT  10
#define PKGI_DIALOG_PROCESS_BAR_PADDING 10
#define PKGI_DIALOG_PROCESS_BAR_CHUNK   200

#define PKGI_MENU_Z            900
#define PKGI_MENU_TEXT_Z       800
#define PKGI_MENU_WIDTH        200 //150 ������ ������
#define PKGI_MENU_HEIGHT       472 //480 ������ ����
#define PKGI_MENU_LEFT_PADDING 14  //20 ������ ������� ���� �����
#define PKGI_MENU_TOP_PADDING  40  //40 ������ ������� ���� ������
