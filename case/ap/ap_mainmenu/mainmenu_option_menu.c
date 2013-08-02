/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     Option Menu list
 * \brief    MainMenu�����˵��б�ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "main_menu.h"

const uint16 theme_text_table[] =
{ BLUE_S, GREEN_S, GRAY_S, PINK_S, RED_S };

const uint8 themes_ap2[5][13] =
{
    "mainmenu.sty", "mainmenu.sty",  "mainmenu.sty",  "mainmenu.sty",  "mainmenu.sty"
};
const uint8 themes_com2[5][13] =
{
    "common.sty", "cm_green.sty",  "cm_gray.sty",  "cm_pink.sty",  "cm_red.sty"
};

/******************************************************************************/
/*!
 * \par  Description:
 *	  Option�˵��б�ѡ���ģʽ�л�
 * \param[in]    0, �޲���ģʽ��1�����ڲ���ģʽ��2����һ�β���ģʽ
 * \param[out]   none
 * \return       the result
 * \retval       app_result_e, �˵��ؼ����صĽṹ��Ϣ
 * \ingroup      module name
 * \par          exmaple code
 * \code
 * \note
 *******************************************************************************/
app_result_e option_menulist(void)
{
    app_result_e result = 0;
    /*��ʼ���˵��ؼ�*/
    menu_com_data_t op_menu;
    op_menu.app_id = APP_ID_MAINMENU;
    /*���岻ͬģʽ�µ���ڲ˵�*/

    if (get_engine_state() == ENGINE_STATE_PLAYING)
    {
        op_menu.menu_entry = PLAYING_MENU;
    }
    else if (get_engine_state() == ENGINE_STATE_PAUSE)
    {
        op_menu.menu_entry = LASTPLAY_MENU;
    }
    else
    {
        op_menu.menu_entry = NOPLAY_MENU;
    }

    op_menu.path_id = 0;
    op_menu.menu_func = NULL;
    /*����˵�˽����Ϣ*/
    result = gui_menulist(MENULIST_OPTION, &op_menu);
    return result;
}

/*!�������ڲ��Ų˵�ִ�к���*/
app_result_e menu_func_playing(void *param)
{
    msg_apps_t temp_msg;

    g_mainmenu_var.cfg_var.active_item = 0xff; //���ڲ���
    temp_msg.type = MSG_CREAT_APP;
    if (get_engine_type() == ENGINE_MUSIC)
    {
        temp_msg.content.data[0] = APP_ID_MUSIC;
    }
    else
    {
        temp_msg.content.data[0] = APP_ID_RADIO;
    }
    temp_msg.content.data[1] = PARAM_BACKTO_PLAYING;
    send_async_msg(APP_ID_MANAGER, &temp_msg);

    return RESULT_APP_QUIT;
}

/*!������һ�β��Ų˵���ִ�к���*/
app_result_e menu_func_last_play(void *param)
{
    msg_apps_t temp_msg;

    g_mainmenu_var.cfg_var.active_item = 0xff; //���ڲ���
    temp_msg.type = MSG_CREAT_APP;
    if (get_engine_type() == ENGINE_MUSIC)
    {
        temp_msg.content.data[0] = APP_ID_MUSIC;
    }
    else
    {
        temp_msg.content.data[0] = APP_ID_RADIO;
    }
    temp_msg.content.data[1] = PARAM_BACKTO_PLAYED;
    send_async_msg(APP_ID_MANAGER, &temp_msg);

    return RESULT_APP_QUIT;
}

/*!��������˵���active��Ĳ�ѯ����*/
app_result_e menu_func_theme(void *title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title;

    menu_title = (menu_title_action_t*) title_action;
    active_id = theme_text_table[g_setting_comval.g_comval.theme];
    menu_title->menu_entry = 0xffff; //���������
    menu_title->str_id = active_id; //ָ��active��
    menu_title->ret_str = NULL;
    menu_title->source_id = 0xffff;
    return RESULT_MENU_SON; //������һ��
}

app_result_e menu_func_return(void)
{
    return RESULT_MENU_PARENT; //������һ��
}

/*!��������˵�����ɫ���ִ�к���*/
app_result_e menu_func_theme_blue(void *param)
{
    menu_title_action_t* menu_title;
    uint16 active_id;

    menu_title = (menu_title_action_t*) param;

    if (g_setting_comval.g_comval.theme != 0)
    {
        g_setting_comval.g_comval.theme = 0;
        active_id = theme_text_table[g_setting_comval.g_comval.theme];
        menu_title->str_id = active_id; //ָ��active��
        ui_res_close(UI_AP);
        ui_res_open(themes_ap2[g_setting_comval.g_comval.theme], UI_AP);
        ui_res_close(UI_COM);
        ui_res_open(themes_com2[g_setting_comval.g_comval.theme], UI_COM);
        return RESULT_MENU_REDRAW_PARENT;
    }
    return RESULT_MENU_PARENT;
}

/*!��������˵�����ɫ���ִ�к���*/
app_result_e menu_func_theme_green(void *param)
{
    menu_title_action_t* menu_title;
    uint16 active_id;

    menu_title = (menu_title_action_t*) param;

    if (g_setting_comval.g_comval.theme != 1)
    {
        g_setting_comval.g_comval.theme = 1;
        active_id = theme_text_table[g_setting_comval.g_comval.theme];
        menu_title->str_id = active_id; //ָ��active��
        ui_res_close(UI_AP);
        ui_res_open(themes_ap2[g_setting_comval.g_comval.theme], UI_AP);
        ui_res_close(UI_COM);
        ui_res_open(themes_com2[g_setting_comval.g_comval.theme], UI_COM);
        return RESULT_MENU_REDRAW_PARENT;
    }
    return RESULT_MENU_PARENT;
}

/*!��������˵��Ļ�ɫ���ִ�к���*/
app_result_e menu_func_theme_gray(void *param)
{
    menu_title_action_t* menu_title;
    uint16 active_id;

    menu_title = (menu_title_action_t*) param;

    if (g_setting_comval.g_comval.theme != 2)
    {
        g_setting_comval.g_comval.theme = 2;
        active_id = theme_text_table[g_setting_comval.g_comval.theme];
        menu_title->str_id = active_id; //ָ��active��
        ui_res_close(UI_AP);
        ui_res_open(themes_ap2[g_setting_comval.g_comval.theme], UI_AP);
        ui_res_close(UI_COM);
        ui_res_open(themes_com2[g_setting_comval.g_comval.theme], UI_COM);
        return RESULT_MENU_REDRAW_PARENT;
    }
    return RESULT_MENU_PARENT;
}

/*!��������˵��ķۺ�ɫ���ִ�к���*/
app_result_e menu_func_theme_pink(void *param)
{
    menu_title_action_t* menu_title;
    uint16 active_id;

    menu_title = (menu_title_action_t*) param;

    if (g_setting_comval.g_comval.theme != 3)
    {
        g_setting_comval.g_comval.theme = 3;
        active_id = theme_text_table[g_setting_comval.g_comval.theme];
        menu_title->str_id = active_id; //ָ��active��
        ui_res_close(UI_AP);
        ui_res_open(themes_ap2[g_setting_comval.g_comval.theme], UI_AP);
        ui_res_close(UI_COM);
        ui_res_open(themes_com2[g_setting_comval.g_comval.theme], UI_COM);
        return RESULT_MENU_REDRAW_PARENT;
    }
    return RESULT_MENU_PARENT;
}

/*!��������˵��ĺ�ɫ���ִ�к���*/
app_result_e menu_func_theme_red(void *param)
{
    menu_title_action_t* menu_title;
    uint16 active_id;

    menu_title = (menu_title_action_t*) param;

    if (g_setting_comval.g_comval.theme != 4)
    {
        g_setting_comval.g_comval.theme = 4;
        active_id = theme_text_table[g_setting_comval.g_comval.theme];
        menu_title->str_id = active_id; //ָ��active��
        ui_res_close(UI_AP);
        ui_res_open(themes_ap2[g_setting_comval.g_comval.theme], UI_AP);
        ui_res_close(UI_COM);
        ui_res_open(themes_com2[g_setting_comval.g_comval.theme], UI_COM);
        return RESULT_MENU_REDRAW_PARENT;
    }
    return RESULT_MENU_PARENT;
}

