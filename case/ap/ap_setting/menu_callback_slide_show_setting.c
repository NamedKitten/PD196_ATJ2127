/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_slide_show_setting.C
 * History:
 *      <johnsen>    <2011-9-25 20:43>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ap_setting.h"
#include "menu_callback_slide_show_setting.h"

//һ���˵���-�õ�Ƭ��ӳ����
//�õ�Ƭ��ӳ����|-ÿ�Żõ�Ƭ��ʱ��
//              |-�ظ�����|-��
//              |         |-��
//              |-�������|-��
//              |         |-��
////////////////////


/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڵ���ÿ��ͼƬ��ʾʱ��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_time_per_slide_callback(void *menu_title_void)
{
    //����ʱ��
    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item;
    app_result_e result;
    
    s_param_item.unit_id = S_SLIDE_TIME_UNIT;
    s_param_item.min = VALUE_SLIDE_TIME_MIN;
    s_param_item.max = VALUE_SLIDE_TIME_MAX;
    s_param_item.max_number = 2;
    s_param_item.step = VALUE_SLIDE_TIME_STEP;
    s_param_item.value = (int16) g_setting_vars.g_comval.slide_time;
    s_param_item.value_str = NULL;
    s_param_item.cycle = TRUE;
    s_param_item.adjust_func = NULL;
    s_param_item.callback = NULL;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 1;
    s_param_private.items = &s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;

    result = gui_set_parameter(PARAM_SIMPLE, &s_param_com);
    if(result == RESULT_CONFIRM)//ȷ�����ò�������
    {
        g_setting_vars.g_comval.slide_time = (uint8) s_param_com.private_data->items->value;
        return (app_result_e) RESULT_REDRAW; //������һ��
    }
    else
    {
        return result;
    }
    
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ�õ�ǰͼƬ����ѭ��ģʽѡ��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ����ͼƬ����ѭ��ģʽѡ��string ID
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e get_slide_repeat_mode(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_slide_repeat_mode;
    menu_title_slide_repeat_mode = (menu_title_action_t*) title_action;
    if (g_setting_vars.g_comval.slide_repeat_mode == VALUE_SLIDE_REPEAT_OFF)
    {
        active_id = S_OFF;
    }
    else
    {
        active_id = S_ON;
    }
    menu_title_slide_repeat_mode->menu_entry = -1; //���������
    menu_title_slide_repeat_mode->str_id = active_id; //ָ��active��
    menu_title_slide_repeat_mode->ret_str = NULL;
    menu_title_slide_repeat_mode->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڹر�ͼƬѭ������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_slide_repeat_off_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.slide_repeat_mode = VALUE_SLIDE_REPEAT_OFF;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڴ�ͼƬѭ������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_slide_repeat_on_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.slide_repeat_mode = VALUE_SLIDE_REPEAT_ON;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ�õ�ǰͼƬ�������ģʽѡ��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ����ͼƬ�������ģʽѡ��string ID
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e get_slide_shuffle_mode(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_slide_shuffle_mode;
    menu_title_slide_shuffle_mode = (menu_title_action_t*) title_action;
    if (g_setting_vars.g_comval.slide_shuffle == VALUE_SLIDE_SHUFFLE_OFF)
    {
        active_id = S_OFF;
    }
    else
    {
        active_id = S_ON;
    }
    menu_title_slide_shuffle_mode->menu_entry = -1; //���������
    menu_title_slide_shuffle_mode->str_id = active_id; //ָ��active��
    menu_title_slide_shuffle_mode->ret_str = NULL;
    menu_title_slide_shuffle_mode->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڹر�ͼƬ�������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_slide_shuffle_off_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.slide_shuffle = VALUE_SLIDE_SHUFFLE_OFF;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڴ�ͼƬ�������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_slide_shuffle_on_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.slide_shuffle = VALUE_SLIDE_SHUFFLE_ON;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}
