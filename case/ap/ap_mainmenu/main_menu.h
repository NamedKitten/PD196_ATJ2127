/*******************************************************************************
 *                              us212a
 *                            Module: Mainmenu
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
/*
 * \file     main_menu.h
 * \brief    mainmneuӦ�õ�ͷ�ļ�
 * \author   zhangxs
 * \version 1.0
 * \date  2011/09/04
 *******************************************************************************/
#ifndef _MAIN_MENU_H
#define _MAIN_MENU_H

#include  "mainmenu_res.h"
#include  "mainmenu_sty.h"
#include  "psp_includes.h"
#include  "case_include.h"

/*!�������˵�Ӧ�õ�����ջ�����߳����ȼ�*/
#define    AP_MAINMENU_STK_POS  0
#define    AP_MAINMENU_PRIO     6
/*!����option��ڲ˵��ı��*/
#define    NOPLAY_MENU         0
#define    PLAYING_MENU        1
#define    LASTPLAY_MENU       2

/*!����MAINMENU ħ��ֵ*/
#define MAGIC_MAINMENU 0x55aa
#define    MAX_APP_NUM    14
#define  FRAME_NUM  1
//id3 buffer����
#define Id3BufSize 52
#define    MAX_INFO_LEN    50
//#define  MAGIC_COMVAL   0x55aa
/*�������˵�Ӧ�õ�VMȫ�ֱ���*/
/*!
 * \brief
 *  main_menu��VMȫ�ֱ���.
 */

typedef struct
{
    uint16 active_item;//�AP�˵�����
    uint16 total_item; //AP�˵�����
    uint16 ap_id[MAX_APP_NUM]; //֧��Ӧ�ñ�ŵ�����
} cfg_mainmenu_var_t;

typedef struct
{
    uint16 magic; //VM ����ħ��
    cfg_mainmenu_var_t cfg_var;
} g_mainmenu_var_t;

/*�������˵��ĳ���*/
/*!
 *  \brief
 *  scene type
 *  \li SCENE_EXIT : �˳�
 *  \li SCENE_MAINMENU : ���˵�Ӧ���б���
 *  \li SCENE_OPTIONMENU : �˵�ѡ���б���
 */
typedef enum
{
    /*!�˳�*/
    SCENE_EXIT = 0,
    /*!���˵�Ӧ���б���*/
    SCENE_MAINMENU,
    /*!�˵�ѡ���б���*/
    SCENE_OPTIONMENU
} scene_mainmenu_e;

/*����Ӧ������*/
/*!
 *  \brief
 *  Ӧ�����Ͷ���
 *  \li MUSIC_TYPE : ������
 *  \li VIDEO_TYPE : ��Ƶ��
 *  \li PIC_TYPE : ͼƬ��
 *  \li TXT_TYPE : �ı���
 *  \li FM_TYPE : ��������
 */
typedef enum
{
    /*!������*/
    MUSIC_TYPE = 0,
    /*!��Ƶ��*/
    VIDEO_TYPE,
    /*!ͼƬ��*/
    PIC_TYPE,
    /*!�ı���*/
    TXT_TYPE,
    /*!��������*/
    FM_TYPE
} ap_type_e;

typedef enum
{
    /*!������Ϣ*/
    NORMAL = 0,
    /*!��һ����Ϣ*/
    NEXTITEM,
    /*!��һ����Ϣ*/
    PREVITEM,
    /*!���µײ�*/
    BOTTOM
} mainmenu_ui_result_e;

typedef enum
{
    FILE_NO_CHANGE = 0, FILE_CHANGE, FORMAT_ERR
} file_status_e;

/*! �ⲿ��������*/
extern g_mainmenu_var_t g_mainmenu_var;
extern setting_comval_t g_setting_comval;
extern uint16 g_active_index;
extern uint16 g_item_amount;
extern uint16 ap_pic_box[];
extern uint16 ap_pic_box_bg[];

//extern uint16 ap_pic_bottom_box[];
extern uint16 ap_text_box[][MAX_APP_NUM + 1];
extern uint16 ap_info_box[][MAX_APP_NUM + 1];
extern uint16 ap_string_box[];
extern uint16 ap_string_info_box[];
extern uint16 ap_file_amount[];
extern uint8 ap_string_infor[];
extern uint8 g_title_buf[];
extern int32 g_music_mount_id;
extern file_path_info_t g_file_path_info;
extern id3_info_t g_id3_info;
extern mengine_status_t g_music_status;
extern bool need_check_status;
extern bool need_fsel_init;
extern uint8 g_frame_num;
extern mainmenu_ui_result_e g_direction;
extern int8 scroll_timer;
extern int8 status_timer;
extern uint32 g_fm_feq;
extern uint16 g_ap_file_num[];
extern uint8 headbar_update;
extern uint8 g_check_mengine_status;
/*!�ⲿ��������*/
extern app_result_e mainmenu_desktop(void) __FAR__;
extern app_result_e option_menulist(void) __FAR__;
extern app_result_e mainmenu_gui_msg_handle(msg_apps_type_e gui_event) __FAR__;
/*!�˵�ѡ�������*/
extern app_result_e menu_func_playing(void *param) __FAR__;
extern app_result_e menu_func_last_play(void *param) __FAR__;
extern app_result_e menu_func_theme(void *param) __FAR__;
extern app_result_e menu_func_theme_blue(void *param) __FAR__;
extern app_result_e menu_func_theme_green(void *param) __FAR__;
extern app_result_e menu_func_theme_gray(void *param) __FAR__;
extern app_result_e menu_func_theme_pink(void *param) __FAR__;
extern app_result_e menu_func_theme_red(void *param) __FAR__;
extern app_result_e menu_func_return(void) __FAR__;
extern app_result_e mainmenu_app_msg_callback(private_msg_t *pri_msg) __FAR__;
extern void _get_music_info(void) __FAR__;
extern void _init_ap_display_infor(void) __FAR__;
extern void _paint_desktop(mainmenu_ui_result_e ui_result) __FAR__;
extern bool music_get_filepath(file_path_info_t *filepath) __FAR__;
extern bool music_close_engine(void) __FAR__;
extern void item_scroll_handle(void) __FAR__;
#endif

