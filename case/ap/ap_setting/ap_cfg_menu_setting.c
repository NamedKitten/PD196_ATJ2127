/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ap_cfg_menu_setting.c
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ap_setting.h"
#include "ap_setting_callback.h"

#define ENTRY_TOTAL 4
#define ITEM_TOTAL 129


/* define variable, �������� */
/*������ڲ˵�*/
const conf_item_head_t entrymenu =
{ "ENTRY MENU  ", ENTRY_TOTAL };

const conf_menu_entry_t entry[ENTRY_TOTAL] =
{
    { MENU_ENTRY_ID_MAIN, S_SETTING_TITLE, S_STORAGE_MSC },
    { MENU_ENTRY_ID_MAIN_MTP, S_SETTING_TITLE, S_STORAGE_MTP },
    { MENU_ENTRY_ID_SHUT_DOWN_ON, S_SHUT_DOWN_TIMER_TITLE, S_SHUT_DOWN_ON_TITLE },
    { MENU_ENTRY_ID_SHUT_DOWN_OFF, S_SHUT_DOWN_TIMER_TITLE, S_SHUT_DOWN_OFF_TITLE }
};

/*����Ҷ�Ӳ˵���*/
const conf_item_head_t item_head =\
{ "MENU ITEM   ", ITEM_TOTAL };

const conf_menu_item_t item[ITEM_TOTAL] =
{
/*һ���˵���-����ģʽ*/
    { S_PLAY_MODE, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,0,0}, /*����ģʽ*/
/*����ģʽ-������Դ�������˵���*/
    { S_MUSIC_FROM, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,1,0}, /*������Դ��...*/
/*����ģʽ-������Դ��ʽ�������˵���*/
 /*...��Դ�������ļ�*/
    { S_FROM_ALL_SONGS, 0, set_music_from_all_songs_callback, NULL, option_callback, NORMAL_MENU_ITEM ,2,0},
/*...��Դ�ڴ��ݳ���*/
    { S_FROM_ARTIST, 0, set_music_from_artist_callback, NULL, option_callback, NORMAL_MENU_ITEM ,3,0},
/*...��Դ�ڴ�ר��*/
    { S_FROM_ALBUM, 0, set_music_from_album_callback, NULL, option_callback, NORMAL_MENU_ITEM ,4,0},
/*...��Դ�ڴ�����*/
    { S_FROM_GENRE, 0, set_music_from_genre_callback, NULL, option_callback, NORMAL_MENU_ITEM ,5,0},
/*����ģʽ-�ظ����ţ������˵���*/
/*�ظ�����*/
    {
        S_REPEAT, S_MUSIC_REPEAT_MODE, get_music_repeat_mode_id_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,6,0
    },
/*����ģʽ-�ظ����ŷ�ʽ�������˵���*/
/*�ظ����Ź�*/
    { S_REPEAT_OFF, S_MUSIC_REPEAT_OFF, set_music_repeat_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,7,0 },
    {
        S_REPEAT_ONE, 0, set_music_repeat_one_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,8,0
    }, /*�����ظ�*/
    { S_REPEAT_ALL, 0, set_music_repeat_all_callback, NULL, option_callback, RAIDO_MENU_ITEM ,9,0 }, /*ȫ���ظ�*/
    { S_REPEAT_INTRO, 0, set_music_intro_callback, NULL, option_callback, RAIDO_MENU_ITEM ,134,0 }, /*ȫ���ظ�*/
/*����ģʽ-������ţ������˵���*/
/*�������*/
    { S_SHUFFLE, S_MUSIC_SHUFFLE_OPTION, get_music_shuflle_option, NULL, option_callback, NORMAL_MENU_ITEM ,10,0 },
/*����ģʽ-������ŷ�ʽ�������˵���*/
/*������Ź�*/
    { S_OFF, S_MUSIC_SHUFFLE_OFF, set_music_shuffle_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,11,0 },
/*������ſ�*/
    { S_ON, S_MUSIC_SHUFFLE_ON, set_music_shuffle_on_callback, NULL, option_callback, RAIDO_MENU_ITEM ,12,0 },
/*һ���˵���-��������*/
    { S_SOUND_SETTING, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,13,0 }, /*��������*/
/*��������-fullsound�������˵���*/
    {
        S_FULLSOUND, 0, get_fullsound_option_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,14,0
    }, /*fullsound*/
/*��������-fullsoundѡ������˵���*/
    {
        S_OFF, S_FULLSOUND_OFF, fullsound_off_sure, set_eq_normal_callback,
        option_callback, RAIDO_MENU_ITEM ,15,0
    }, /*fullsound��*/
    {
        S_ON, S_FULLSOUND_ON, fullsound_on_sure, set_eq_fullsoundon_callback,
        option_callback, RAIDO_MENU_ITEM ,16,0
    }, /*fullsound��*/
/*��������-�������������˵���*/
    { S_EQUALIZER, 0, get_eq_option_callback, NULL, option_callback, NORMAL_MENU_ITEM ,17,0 }, /*������*/
/*��������-������ѡ������˵���*/
    { S_OFF, S_EQ_OFF, set_eq_normal_sure, set_eq_normal_callback, option_callback, RAIDO_MENU_ITEM ,18,0 }, /*�����*/
    { S_EQ_ROCK, 0, set_eq_rock_sure, set_eq_rock_callback, option_callback, RAIDO_MENU_ITEM ,19,0 }, /*ҡ��*/
    { S_EQ_FUNK, 0, set_eq_funk_sure, set_eq_funk_callback, option_callback, RAIDO_MENU_ITEM ,20,0 }, /*�ſ�*/
/*ϣ�ջ���*/
    { S_EQ_HIP_HOP, 0, set_eq_hip_hop_sure, set_eq_hiphop_callback, option_callback, RAIDO_MENU_ITEM ,21,0 },
    { S_EQ_JAZZ, 0, set_eq_jazz_sure, set_eq_jazz_callback, option_callback, RAIDO_MENU_ITEM ,22,0 }, /*��ʿ*/
/*�ŵ�*/
    {
        S_EQ_CLASSICAL, 0, set_eq_classical_sure, set_eq_classical_callback,
        option_callback, RAIDO_MENU_ITEM ,23,0
    },
/*������*/
    { S_EQ_TECHNO, 0, set_eq_techno_sure, set_eq_techno_callback, option_callback, RAIDO_MENU_ITEM ,24,0 },
 /*�Զ���*/
    { S_EQ_CUSTOM, 0, set_eq_custom_sure, set_eq_custom_callback, option_callback, RAIDO_MENU_ITEM ,25,0 },
/*��������-srs�������˵���*/
    { S_SRS, 0, get_srs_option_callback, NULL, option_callback, NORMAL_MENU_ITEM ,26,0 }, /*srs*/
/*��������-srsѡ������˵���*/
    /*srs��*/
    { S_OFF, S_SRS_OFF, set_srs_off_sure, set_eq_normal_callback, option_callback, RAIDO_MENU_ITEM ,27,0 }, 
    /*wowhd��*/
    { S_WOWHD, 0, set_wowhd_on_sure, NULL, option_callback, RAIDO_MENU_ITEM ,28,0 }, 
    /*user srs��*/
    { S_USER_SRS, 0, set_user_srs_on_sure, set_user_srs_on_callback, option_callback, RAIDO_MENU_ITEM ,29,0 }, 
/*��������-�������ƣ������˵���*/
    {
        S_VOLUME_LIMIT, 0, set_volume_limit_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,30,0
    }, /*��������*/
/*һ���˵���-��ʱ�ػ�*/
    {
        S_SHUT_DOWN_TIMER_TITLE, 0, get_shut_down_entry, NULL,
        option_callback, NORMAL_MENU_ITEM ,31,0
    }, /*��ʱ�ػ�*/
/*��ʱ�ػ�ѡ������˵���*/
/*�رն�ʱ�ػ�*/
    { S_OFF, S_SHUT_DOWN_TIMER_OFF, shut_down_timer_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,32,0 },
/*���ö�ʱ�ػ�*/
    { S_SHUT_DOWN_SETTING, 0, shut_down_time_adjust_callback, NULL, option_callback, RAIDO_MENU_ITEM ,33,0 },
/*һ���˵���-��ʾ����*/
    { S_DISPLAY_SETTING, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,34,0 }, /*��ʾ����*/
/*��ʾ����-���ȣ������˵���*/
    {
        S_BRIGHTNESS_SETTING, 0, set_brightness_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,35,0
    }, /*���ȵ���*/
/*��ʾ����-���⣨�����˵���*/
    { S_THEME, 0, get_theme_option_callback, NULL, option_callback, NORMAL_MENU_ITEM ,36,0 }, /*����*/
/*��ʾ����-����ѡ������˵���*/
    { S_THEME_BLUE, 0, set_theme_blue_callback, NULL, option_callback, RAIDO_MENU_ITEM ,37,0 }, /*��ɫ����*/
    { S_THEME_GREEN, 0, set_theme_green_callback, NULL, option_callback, RAIDO_MENU_ITEM ,38,0 }, /*��ɫ����*/
    { S_THEME_GRAY, 0, set_theme_gray_callback, NULL, option_callback, RAIDO_MENU_ITEM ,39,0 }, /*��ɫ����*/
    { S_THEME_PINK, 0, set_theme_pink_callback, NULL, option_callback, RAIDO_MENU_ITEM ,40,0 }, /*�ۺ�ɫ����*/
    { S_THEME_RED, 0, set_theme_red_callback, NULL, option_callback, RAIDO_MENU_ITEM ,41,0 }, /*��ɫ����*/
/*��ʾ����-�����ʱ���������˵���*/
/*�����ʱ��*/
    { S_BACKLIGHT_TIMER, 0, get_backlight_timer_option_callback, NULL, option_callback, NORMAL_MENU_ITEM ,42,0 },
/*��ʾ����-�����ʱ��ѡ������˵���*/
    {
        S_LIGHT_TIME_10S, 0, set_light_time_10s_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,43,0
    }, /*����10��*/
    {
        S_LIGHT_TIME_20S, 0, set_light_time_20s_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,44,0
    }, /*����20��*/
    {
        S_LIGHT_TIME_30S, 0, set_light_time_30s_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,45,0
    }, /*����30��*/
/*ʼ������*/
    { S_LIGHT_TIME_ALWAYS, 0, set_light_time_always_callback, NULL, option_callback, RAIDO_MENU_ITEM ,46,0 },
/*��ʾ����-��Ļ�����������˵���*/
    {
        S_SCREEN_SAVER, 0, get_screen_saver_option_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,47,0
    }, /*��Ļ����*/
/*��ʾ����-��Ļ����ѡ������˵���*/
/*�ر���Ļ����*/
    { S_WITHOUT_SCREEN_SAVER, 0, set_screen_saver_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,48,0 },
/*����ʱ��*/
    { S_SCREEN_SAVER_CLOCK, 0, set_screen_saver_clock_callback, NULL, option_callback, RAIDO_MENU_ITEM ,49,0 },
/*���ͼ��*/
    { S_SCREEN_SAVER_PHOTOS, 0, set_screen_saver_photos_callback, NULL, option_callback, RAIDO_MENU_ITEM ,50,0 },
    { S_SCREEN_OFF, 0, set_screen_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,51,0 }, /*�ر���Ļ*/
/*��ʾģʽ*/
    { S_SCREEN_SAVER_DEMO, 0, set_screen_saver_demo_callback, NULL, option_callback, RAIDO_MENU_ITEM ,52,0 },
/*һ���˵���-���ں�ʱ��*/
    { S_DATE_AND_TIME, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,53,0 }, /*���ں�ʱ��*/
/*���ں�ʱ��-ʱ�����ã������˵���*/
    { S_TIME_SETTING, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,54,0}, /*ʱ������*/
/*���ں�ʱ��-ʱ������-����ʱ���ʽ�������˵���*/
    {
        S_SET_TIME_FORMAT, 0, get_time_format_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,55,0
    }, /*����ʱ���ʽ*/
/*���ں�ʱ��-ʱ������-����ʱ���ʽ-ʱ���ʽѡ��ļ��˵���*/
/*����Ϊ12Сʱ��*/
    { S_TIME_FORMAT_12H, 0, set_time_format_12h_callback, NULL, option_callback, RAIDO_MENU_ITEM ,56,0 },
/*����Ϊ24Сʱ��*/
    { S_TIME_FORMAT_24H, 0, set_time_format_24h_callback, NULL, option_callback, RAIDO_MENU_ITEM ,57,0 },
/*���ں�ʱ��-ʱ������-����ʱ�䣨�����˵���*/
    { S_SET_TIME, 0, set_time_callback, NULL, option_callback, NORMAL_MENU_ITEM ,58,0 }, /*����ʱ��*/
/*���ں�ʱ��-�������ã������˵���*/
    { S_DATE_SETTING, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,59,0 }, /*��������*/
/*���ں�ʱ��-�������ø�ʽ�������˵���*/
    {
        S_SET_DATE_FORMAT, 0, get_date_format_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,60,0
    }, /*�������ڸ�ʽ*/
/*���ں�ʱ��-��������-���ڸ�ʽ-���ڸ�ʽѡ��ļ��˵���*/
/*��������Ϊdd-mm-yyyy��ʽ*/
    {
        S_DATE_FORMAT_DD_MM_YYYY, 0, set_date_format_day_first_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,61,0
    },
/*��������Ϊmm-dd-yyyy��ʽ*/
    {
        S_DATE_FORMAT_MM_DD_YYYY, 0, set_date_format_month_first_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,62 ,0
    },
/*��������Ϊyyyy-mm-dd��ʽ*/
    {
        S_DATE_FORMAT_YYYY_MM_DD, 0, set_date_format_year_first_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,63 ,0
    },
/*���ں�ʱ��-�������ڣ������˵���*/
    { S_SET_DATE, 0, set_date_callback, NULL, option_callback, NORMAL_MENU_ITEM ,64,0 }, /*��������*/
/*һ���˵���-�õ�Ƭ��ӳ����*/
    { S_SLIDE_SHOW_SETTING, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,65,0}, /*�õ�Ƭ��ӳ����*/
/*�õ�Ƭ��ӳ����-ÿ�Żõ�Ƭ��ʱ�䣨�����˵���*/
/*ÿ�Żõ�Ƭʱ��*/
    { S_TIME_PER_SLIDE, 0, set_time_per_slide_callback, NULL, option_callback, NORMAL_MENU_ITEM ,66,0 },
/*�õ�Ƭ��ӳ����-�ظ����ţ������˵���*/
/*�õ��ظ�����*/
    { S_REPEAT, S_SLIDE_REPEAT_MODE, get_slide_repeat_mode, NULL, option_callback, NORMAL_MENU_ITEM ,67,0 },
/*�õ�Ƭ��ӳ����-�ظ�����ѡ������˵���*/
/*�õ�repeat��*/
    { S_OFF, S_SLIDE_REPEAT_OFF, set_slide_repeat_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,68,0 },
/*�õ�repeat��*/
    { S_ON, S_SLIDE_REPEAT_ON, set_slide_repeat_on_callback, NULL, option_callback, RAIDO_MENU_ITEM ,69,0 },
/*�õ�Ƭ��ӳ����-������ţ������˵���*/
/*�õ��������*/
    { S_SHUFFLE, S_SLIDE_SHUFFLE_OPTION, get_slide_shuffle_mode, NULL, option_callback, NORMAL_MENU_ITEM ,70,0 },
/*�õ�Ƭ��ӳ����-�������ѡ������˵���*/
/*�õ�shuffle��*/
    { S_OFF, S_SLIDE_SHUFFLE_OFF, set_slide_shuffle_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,71,0 },
/*�õ�shuffle��*/
    { S_ON, S_SLIDE_SHUFFLE_ON, set_slide_shuffle_on_callback, NULL, option_callback, RAIDO_MENU_ITEM ,72,0 },
/*һ���˵���-language/����*/
    { S_LANGUAGE, 0, get_language_id_callback, NULL, option_callback, NORMAL_MENU_ITEM ,73,0 }, /*language/����*/
/*language/����-����ѡ������˵���*/
    {
        S_LANGUAGE_CZECH, SN_LANGUAGE_CZECH, set_czech_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,74,0
    }, /*�ݿ���*/
    {
        S_LANGUAGE_GREEK, SN_LANGUAGE_GREEK, set_greek_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,75,0
    }, /*ϣ����*/
    {
        S_LANGUAGE_ENGLISH, SN_LANGUAGE_ENGLISH, set_english_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,76,0
    }, /*Ӣ��*/
    {
        S_LANGUAGE_DENISH, SN_LANGUAGE_DENISH, set_denish_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,77,0
    }, /*������*/
    {
        S_LANGUAGE_GERMANY, SN_LANGUAGE_GERMANY, set_germany_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,78,0
    }, /*����*/
    {
        S_LANGUAGE_SPANISH, SN_LANGUAGE_SPANISH, set_spanish_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,79,0
    }, /*��������*/
    {
        S_LANGUAGE_FRENCH, SN_LANGUAGE_FRENCH, set_french_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,80,0
    }, /*����*/
    {
        S_LANGUAGE_ITALIAN, SN_LANGUAGE_ITALIAN, set_italian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,81,0
    }, /*�������*/
    {
        S_LANGUAGE_HUNGARIAN, SN_LANGUAGE_HUNGARIAN, set_hungarian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,82,0
    }, /*��������*/
    {
        S_LANGUAGE_DUTCH, SN_LANGUAGE_DUTCH, set_dutch_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,83,0
    }, /*������*/
    {
        S_LANGUAGE_NORWEGIAN, SN_LANGUAGE_NORWEGIAN, set_norwegian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,84,0
    }, /*Ų����*/
    {
        S_LANGUAGE_POLAND, SN_LANGUAGE_POLAND, set_poland_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,85,0
    }, /*������*/
    {
        S_LANGUAGE_PORTUGUESE_EUROPEAN, SN_LANGUAGE_PORTUGUESE_EUROPEAN, set_portuguese_european_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,86,0
    }, /*��������-ŷ��*/
    {
        S_LANGUAGE_PORTUGUESE_BRAZILIAN, SN_LANGUAGE_PORTUGUESE_BRAZILIAN, set_portuguese_brazilian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,87,0
    }, /*��������-����*/
    {
        S_LANGUAGE_RUSSIAN, SN_LANGUAGE_RUSSIAN, set_russian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,88,0
    }, /*����*/
    {
        S_LANGUAGE_SLOVAK, SN_LANGUAGE_SLOVAK, set_slovak_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,89,0
    }, /*˹�工����*/
    {
        S_LANGUAGE_FINNISH, SN_LANGUAGE_FINNISH, set_finnish_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,90,0
    }, /*������*/
    {
        S_LANGUAGE_SWEDISH, SN_LANGUAGE_SWEDISH, set_swedish_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,91,0
    }, /*�����*/
    {
        S_LANGUAGE_TURKEY, SN_LANGUAGE_TURKEY, set_turkey_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,92,0
    }, /*��������*/
    {
        S_LANGUAGE_SIMPLIFIED_CHINESE, SN_LANGUAGE_SIMPLIFIED_CHINESE, set_simplified_chinese_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,93,0
    },/*��������*/
    {
        S_LANGUAGE_TRAD_CHINESE, SN_LANGUAGE_TRAD_CHINESE, set_trad_chinese_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,94,0
    }, /*��������*/
    {
        S_LANGUAGE_JAPANESE, SN_LANGUAGE_JAPANESE, set_japanese_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,95,0
    }, /*����*/
    {
        S_LANGUAGE_KOREAN, SN_LANGUAGE_KOREAN, set_korean_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,96,0
    }, /*����*/
    {
        S_LANGUAGE_HEBREW, SN_LANGUAGE_HEBREW, set_hebrew_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,97,0
    }, /*ϣ������*/
    {
        S_LANGUAGE_ARABIC, SN_LANGUAGE_ARABIC, set_arabic_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,98,0
    }, /*��������*/
    {
        S_LANGUAGE_THAI, SN_LANGUAGE_THAI, set_thai_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,99,0
    }, /*̩��*/
    {
        S_LANGUAGE_ROMANIAN, SN_LANGUAGE_ROMANIAN, set_romanian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,100,0
    }, /*������*/
    {
        S_LANGUAGE_INDONESIAN, SN_LANGUAGE_INDONESIAN, set_indonesian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,101,0
    }, /*ӡ����*/
    {
        S_LANGUAGE_CROATIAN, SN_LANGUAGE_CROATIAN, set_croatian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,102,0
    }, /*���޵�����*/
    {
        S_LANGUAGE_SLOVENIAN, SN_LANGUAGE_SLOVENIAN, set_slovenian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,103,0
    }, /*˹����������*/


/*һ���˵���-����������ѡ��*/
/*����������ѡ��*/
    {
        S_PC_CONNECT_OPTION, 0, get_pc_connect_information_id_callback, NULL,
        option_callback, NORMAL_MENU_ITEM,110 ,0
    },
/*����������ѡ��-ѡ��������˵���*/
    { S_STORAGE_MSC, 0, set_storage_msc_callback, NULL, option_callback, RAIDO_MENU_ITEM ,111,0 }, /*����Ϊmsc*/
    { S_STORAGE_MTP, 0, set_storage_mtp_callback, NULL, option_callback, RAIDO_MENU_ITEM ,112,0 }, /*����Ϊmtp*/
    { S_EXPLAIN_MSC, 0, explain_msc_callback, NULL, option_callback, NORMAL_MENU_ITEM ,113,0 }, /*����msc*/
    { S_EXPLAIN_MTP, 0, explain_mtp_callback, NULL, option_callback, NORMAL_MENU_ITEM ,114,0 }, /*����mtp*/
/*һ���˵���-��Ϣ*/
    { S_INFORMATION, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,115,0 }, /*��Ϣ*/
/*��Ϣ-��������Ϣ�������˵���*/
/*��������Ϣ*/
    { S_PLAYER_INFORMATION, 0, show_player_information_callback, NULL, option_callback, NORMAL_MENU_ITEM ,116,0 },
/*��Ϣ-���̿ռ���Ϣ�������˵���*/
/*���̿ռ�*/
    { S_DISK_INFORMATION, 0, show_disk_information_callback, NULL, option_callback, NORMAL_MENU_ITEM ,117,0 },
/*��Ϣ-֧���ļ��������˵���*/
    { S_SUPPORT_FILES, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,118,0 }, /*֧���ļ�����*/
/*��Ϣ-֧���ļ�-�ļ����ͣ������˵���*/
    { S_MUSIC, 0, show_music_type_callback, NULL, option_callback, NORMAL_MENU_ITEM ,119 ,0}, /*��������*/
    { S_PICTURE, 0, show_picture_type_callback, NULL, option_callback, NORMAL_MENU_ITEM ,120,0 }, /*ͼƬ����*/
    { S_VIDEO, 0, show_video_type_callback, NULL, option_callback, NORMAL_MENU_ITEM ,121,0 }, /*��Ƶ����*/
/*һ���˵���-������Ϣ*/
/*������Ϣ*/
    { S_LEGAL_INFORMATION, 0, show_legal_information_callback, NULL, option_callback, NORMAL_MENU_ITEM ,122,0 },
/*һ���˵���-��ʽ���豸*/
/*��ʽ���豸*/
    { S_FORMAT_DEVICE, 0, get_format_device_entry, NULL, option_callback, NORMAL_MENU_ITEM ,123,0 },
/*��ʽ���豸-��ʽ��ѡ������˵���*/
    { S_FORMAT_DISK, 0, format_disk_callback, NULL, option_callback, NORMAL_MENU_ITEM ,124,0 }, /*��ʽ������*/
    { S_FORMAT_CARD, 0, format_card_callback, NULL, option_callback, NORMAL_MENU_ITEM ,125,0 }, /*��ʽ������*/
    { S_DONT_FORMAT, 0, dont_format_callback, NULL, option_callback, NORMAL_MENU_ITEM ,126,0}, /*����ʽ��*/
/*һ���˵���-�����趨*/
/*�����趨*/
    {
        S_FACTORY_SETTING, 0, dialog_restore_factory_setting_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,127,0
    },
/*һ���˵���-CD��װ����*/
    { S_AUTORUN_SETTING, 0, get_autorun_setting_callback, NULL, option_callback, NORMAL_MENU_ITEM ,128,0 },
/*CD��װ����-�趨ѡ������˵���*/
/*��ֹ�Զ�����*/
    { S_DISABLE, S_DISABLE_AUTORUN, disable_autorun_callback, NULL, option_callback, RAIDO_MENU_ITEM ,129,0 },
/*ʹ���Զ�����*/
    { S_ENABLE, S_ENABLE_AUTORUN, enable_autorun_callback, NULL, option_callback, RAIDO_MENU_ITEM ,130,0 },
/*������*/
    {S_KEY_TONE, 0, get_key_tone_setting_callback, NULL, option_callback, NORMAL_MENU_ITEM ,131,0},
    {S_ON, SN_KEY_TONE_ON, enable_key_tone_setting_callback, NULL, option_callback, RAIDO_MENU_ITEM ,132,0},
    {S_OFF, SN_KEY_TONE_OFF, disable_key_tone_setting_callback, NULL, option_callback, RAIDO_MENU_ITEM ,133,0}
};



#ifdef PC
menu_cb_func _get_menu_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].menu_func;
        }
    }

    return NULL;
}

menu_cb_leaf _get_callback_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].callback;
        }
    }

    return NULL;
}

menu_cb_option _get_menu_option(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].menu_option;
        }
    }

    return NULL;
}
#endif

