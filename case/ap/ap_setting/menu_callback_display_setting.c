/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_display_setting.C
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ap_setting.h"
#include "menu_callback_display_setting.h"

const uint16 theme_text_table[THEME_TOTAL] =
{ S_THEME_BLUE, S_THEME_GREEN, S_THEME_GRAY, S_THEME_PINK, S_THEME_RED };

const uint16 backlight_timer_text_table[LIGHT_TIMER_TOTAL] =
{ S_LIGHT_TIME_ALWAYS, S_LIGHT_TIME_10S, S_LIGHT_TIME_20S, S_LIGHT_TIME_30S };

const uint16 screen_saver_mode_text_table[SCREEN_SAVER_TOTAL] =
{ S_WITHOUT_SCREEN_SAVER, S_SCREEN_SAVER_CLOCK, S_SCREEN_SAVER_PHOTOS, S_SCREEN_OFF, S_SCREEN_SAVER_DEMO };

const uint8 theme_setting_res_1[5][13] =
{ "setting.sty", "setgreen.sty", "set_gray.sty", "set_pink.sty", "set_red.sty" };

const uint8 themes_com_1[5][13] =
{ 
    "common.sty", //Ĭ�Ϸ��������ģ��������ɫ���
    "cm_green.sty", //���������������д+������������ɫ���
    "cm_gray.sty", //��ɫ���
    "cm_pink.sty", //�ۺ�ɫ���
    "cm_red.sty" //��ɫ���
};
//һ���˵�-��ʾ����
//��ʾ����|-����
//        |-����|-��ɫ
//        |     |-��ɫ
//        |     |-��ɫ
//        |     |-�ۺ�ɫ
//        |     |-��ɫ
//        |-�����ʱ��|-10��
//        |           |-20��
//        |           |-30��
//        |           |-ʼ������
//        |-��Ļ����|-��
//                  |-����ʱ��
//                  |-���ͼ��
//                  |-�ر���Ļ
//                  |-��ʾģʽ
////////////////////

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������������������Ļ����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ����
 * \note: ע������˵��
 *
 ******************************************************************************
 */
bool set_lightness_callback(int16 value)
{
    g_setting_vars.g_comval.lightness = (uint8) value;
    /*���õ�Ӳ��*/
    com_set_contrast((uint8) value);
    return TRUE;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������������Ļ����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_brightness_callback(void *menu_title_void)
{
    //��������
    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item;
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    app_result_e result;
    uint8 backup_brightness;
    
    backup_brightness = g_setting_vars.g_comval.lightness;
    s_param_item.unit_id = -1;
    s_param_item.min = VALUE_LIGHTNESS_MIN;
    s_param_item.max = VALUE_LIGHTNESS_MAX;
    s_param_item.max_number = 2;
    s_param_item.step = VALUE_LIGHTNESS_STEP;
    s_param_item.value = (int16) g_setting_vars.g_comval.lightness;
    s_param_item.value_str = NULL;
    s_param_item.cycle = FALSE;
    s_param_item.adjust_func = NULL;
    s_param_item.callback = set_lightness_callback;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 1;
    s_param_private.items = &s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;

    result = gui_set_parameter(SLIDER_SIMPLE, &s_param_com);
    if(result == RESULT_CONFIRM)//ȷ�����ò�������
    {    
        g_setting_vars.g_comval.lightness = (uint8) s_param_com.private_data->items->value;
        
        string_desc.data.id = S_SET_LIGHTNESS_END;
        string_desc.language = UNICODEID;
        
        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        dialog_com.icon_id = V_U16_INVALID;
        dialog_com.string_desc = &string_desc;
        gui_dialog(DIALOG_MSG, &dialog_com);
        return (app_result_e) RESULT_REDRAW; //������һ��
    }
    else
    {
        g_setting_vars.g_comval.lightness = backup_brightness;
        set_lightness_callback((int16)g_setting_vars.g_comval.lightness);
        return result;
    }
    
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ�õ�ǰ����ѡ���string ID��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : �����ַ���ID�ţ��Ա����¼��˵�ȡ�ü�����
 * \retval       ��������һ���˵�
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e get_theme_option_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_theme_option;
    menu_title_theme_option = (menu_title_action_t*) title_action;
    active_id = theme_text_table[g_setting_vars.g_comval.theme];
    menu_title_theme_option->menu_entry = -1; //���������
    menu_title_theme_option->str_id = active_id; //ָ��active��
    menu_title_theme_option->ret_str = NULL;
    menu_title_theme_option->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�������������������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ����
 * \note: ע������˵��
 *
 ******************************************************************************
 */
void switch_to_new_theme(uint8 theme)
{

    ui_res_close(UI_AP);
    ui_res_close(UI_COM);
    res_filename = (uint8 *) theme_setting_res_1[theme];
    ui_res_open(res_filename, UI_AP);
    res_filename = (uint8 *) themes_com_1[theme];
    ui_res_open(res_filename, UI_COM);
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������������ɫ����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_theme_blue_callback(void *menu_title_void)
{
    menu_title_action_t* menu_title_theme_option;
    uint16 active_id;

    g_setting_vars.g_comval.theme = VALUE_THEME_BLUE;

    menu_title_theme_option = (menu_title_action_t*) menu_title_void;
    active_id = theme_text_table[g_setting_vars.g_comval.theme];
    menu_title_theme_option->str_id = active_id; //ָ��active��

    //�л�����
    switch_to_new_theme(g_setting_vars.g_comval.theme);
    return (app_result_e) RESULT_MENU_REDRAW_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������������ɫ����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_theme_green_callback(void *menu_title_void)
{
    menu_title_action_t* menu_title_theme_option;
    uint16 active_id;

    g_setting_vars.g_comval.theme = VALUE_THEME_GREEN;

    menu_title_theme_option = (menu_title_action_t*) menu_title_void;
    active_id = theme_text_table[g_setting_vars.g_comval.theme];
    menu_title_theme_option->str_id = active_id; //ָ��active��

    //    g_setting_vars.g_comval.theme = VALUE_THEME_GREEN;
    //�л�����
    switch_to_new_theme(g_setting_vars.g_comval.theme);
    return (app_result_e) RESULT_MENU_REDRAW_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص������������û�ɫ����
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_theme_gray_callback(void *menu_title_void)
{
    menu_title_action_t* menu_title_theme_option;
    uint16 active_id;

    g_setting_vars.g_comval.theme = VALUE_THEME_GRAY;

    menu_title_theme_option = (menu_title_action_t*) menu_title_void;
    active_id = theme_text_table[g_setting_vars.g_comval.theme];
    menu_title_theme_option->str_id = active_id; //ָ��active��

    //    g_setting_vars.g_comval.theme = VALUE_THEME_GRAY;
    //�л�����
    switch_to_new_theme(g_setting_vars.g_comval.theme);
    return (app_result_e) RESULT_MENU_REDRAW_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص������������÷ۺ�ɫ����
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_theme_pink_callback(void *menu_title_void)
{
    menu_title_action_t* menu_title_theme_option;
    uint16 active_id;

    g_setting_vars.g_comval.theme = VALUE_THEME_PINK;

    menu_title_theme_option = (menu_title_action_t*) menu_title_void;
    active_id = theme_text_table[g_setting_vars.g_comval.theme];
    menu_title_theme_option->str_id = active_id; //ָ��active��

    //    g_setting_vars.g_comval.theme = VALUE_THEME_PINK;
    //�л�����
    switch_to_new_theme(g_setting_vars.g_comval.theme);
    return (app_result_e) RESULT_MENU_REDRAW_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص������������ú�ɫ����
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_theme_red_callback(void *menu_title_void)
{
    menu_title_action_t* menu_title_theme_option;
    uint16 active_id;

    g_setting_vars.g_comval.theme = VALUE_THEME_RED;

    menu_title_theme_option = (menu_title_action_t*) menu_title_void;
    active_id = theme_text_table[g_setting_vars.g_comval.theme];
    menu_title_theme_option->str_id = active_id; //ָ��active��

    //    g_setting_vars.g_comval.theme = VALUE_THEME_RED;
    //�л�����
    switch_to_new_theme(g_setting_vars.g_comval.theme);
    return (app_result_e) RESULT_MENU_REDRAW_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ�õ�ǰ����ѡ���string ID��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : �����ַ���ID�ţ��Ա����¼��˵�ȡ�ü�����
 * \retval       ��������һ���˵�
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e get_backlight_timer_option_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_backlight_timer;
    menu_title_backlight_timer = (menu_title_action_t*) title_action;
    /*light_timer��0.5��Ϊ��λ��0��ʾ���ⳣ��*/
    active_id = backlight_timer_text_table[g_setting_vars.g_comval.light_timer / 20];
    menu_title_backlight_timer->menu_entry = -1; //���������
    menu_title_backlight_timer->str_id = active_id; //ָ��active��
    menu_title_backlight_timer->ret_str = NULL;
    menu_title_backlight_timer->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص������������ñ���ʱ��Ϊ10��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_light_time_10s_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.light_timer = 20; //�԰���Ϊ��λ
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص������������ñ���ʱ��Ϊ20��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_light_time_20s_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.light_timer = 40; //�԰���Ϊ��λ
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص������������ñ���ʱ��Ϊ30��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_light_time_30s_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.light_timer = 60; //�԰���Ϊ��λ
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص������������ñ���ʱ�䳣��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_light_time_always_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.light_timer = 0; //0��ʾ���ⳣ��
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ�õ�ǰ��Ļ����ѡ���string ID��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : �����ַ���ID�ţ��Ա����¼��˵�ȡ�ü�����
 * \retval       ��������һ���˵�
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e get_screen_saver_option_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_screen_saver;
    menu_title_screen_saver = (menu_title_action_t*) title_action;
    active_id = screen_saver_mode_text_table[g_setting_vars.g_comval.screen_saver_mode];
    menu_title_screen_saver->menu_entry = -1; //���������
    menu_title_screen_saver->str_id = active_id; //ָ��active��
    menu_title_screen_saver->ret_str = NULL;
    menu_title_screen_saver->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص������������ùر���Ļ����
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_screen_saver_off_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.screen_saver_mode = VALUE_SCREEN_SAVER_OFF;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�����������������ʱ����Ļ����
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_screen_saver_clock_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.screen_saver_mode = VALUE_SCREEN_SAVER_CLOCK;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص����������������ͼ����Ļ����
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_screen_saver_photos_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.screen_saver_mode = VALUE_SCREEN_SAVER_PHOTOS;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص������������ùر���Ļ
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_screen_off_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.screen_saver_mode = VALUE_SCREEN_OFF;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������������ʾģʽΪ��Ļ����
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_screen_saver_demo_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.screen_saver_mode = VALUE_SCREEN_SAVER_DEMO;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}
