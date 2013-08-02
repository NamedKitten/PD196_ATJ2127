/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_time_date_setting.C
 * History:
 *      <johnsen>    <2011-9-25 20:43>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ap_setting.h"
#include "menu_callback_date_setting.h"

const uint8 day_table[12] =
{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

//һ���˵���-���ں�ʱ��
//���ں�ʱ��|-ʱ������|-����ʱ���ʽ|-12Сʱ��
//          |         |             |-24Сʱ��
//          |         |-����ʱ��
//          |-��������|-�������ڸ�ʽ|-MM-DD-YYYY
//                    |             |-DD-MM-YYYY
//                    |             |-YYYY-MM-DD
//                    |-��������
////////////////////

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ�õ�ǰ���ڸ�ʽѡ��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : �������ڸ�ʽѡ��string ID
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e get_date_format_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_date;
    menu_title_date = (menu_title_action_t*) title_action;
    if (g_setting_vars.g_comval.date_format == VALUE_DD_MM_YYYY)
    {
        active_id = S_DATE_FORMAT_DD_MM_YYYY;
    }
    else if (g_setting_vars.g_comval.date_format == VALUE_MM_DD_YYYY)
    {
        active_id = S_DATE_FORMAT_MM_DD_YYYY;
    }
    else
    {
        active_id = S_DATE_FORMAT_YYYY_MM_DD;
    }
    menu_title_date->menu_entry = -1; //���������
    menu_title_date->str_id = active_id; //ָ��active��
    menu_title_date->ret_str = NULL;
    menu_title_date->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص����������������ڸ�ʽΪdd-mm-yyyy��ʽ
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_date_format_day_first_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.date_format = VALUE_DD_MM_YYYY;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص����������������ڸ�ʽΪmm-dd-yyyy��ʽ
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_date_format_month_first_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.date_format = VALUE_MM_DD_YYYY;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص����������������ڸ�ʽΪyyyy-mm-dd��ʽ
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_date_format_year_first_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.date_format = VALUE_YYYY_MM_DD;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڵ������ա������ֵ
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
adjust_result_e date_adjust_func(parambox_one_t *params, uint8 active, bool inc)
{
    parambox_one_t *day_param = &(params[date_dd_location]);
    parambox_one_t *month_param = &(params[date_mm_location]);
    parambox_one_t *year_param = &(params[date_yy_location]);
    uint8 is_leap;
    uint8 days_max;
    is_leap = (uint8) (((((year_param->value) % 4) == 0) && (((year_param->value) % 100) != 0))
            || (((year_param->value) % 400) == 0));
    days_max = day_table[month_param->value - 1];
    if ((is_leap != 0) && (month_param->value == 2))
    {
        days_max = 29;
    }

    //�յ����������ֵ����Ϊ��Сֵ 1
    if (day_param->value > days_max)
    {
        //�����ղ���
        if (active == date_dd_location)
        {
            if (inc != 0)
            {
                day_param->value = 1;
            }
            else
            {
                day_param->value = (int16) days_max;
            }
        }
        //�����»������
        else
        {
            day_param->value = (int16) days_max;
        }
        return ADJUST_RESULT_ALL;
    }

    return ADJUST_RESULT_NULL;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�����������������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_date_callback(void *menu_title_void)
{
    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item[3];
    app_result_e result;
    
    sys_get_date(&date);
    if (g_setting_vars.g_comval.date_format == VALUE_DD_MM_YYYY)
    {
        date_dd_location = 0;
        date_mm_location = 1;
        date_yy_location = 2;
    }
    else if (g_setting_vars.g_comval.date_format == VALUE_MM_DD_YYYY)
    {
        date_dd_location = 1;
        date_mm_location = 0;
        date_yy_location = 2;
    }
    else //g_setting_vars.g_comval.date_format==VALUE_YYYY_MM_DD
    {
        date_dd_location = 2;
        date_mm_location = 1;
        date_yy_location = 0;
    }
    s_param_item[date_dd_location].min = VALUE_DATE_DD_MIN;
    s_param_item[date_dd_location].max = VALUE_DATE_DD_MAX;
    s_param_item[date_dd_location].step = VALUE_DATE_DD_STEP;
    s_param_item[date_dd_location].value = (int16) date.day;
    s_param_item[date_dd_location].cycle = TRUE;
    s_param_item[date_dd_location].adjust_func = date_adjust_func;
    s_param_item[date_dd_location].callback = NULL;
    s_param_item[date_dd_location].unit_id = S_DATE_DD;
    s_param_item[date_dd_location].max_number = 2;
    s_param_item[date_dd_location].value_str = NULL;

    s_param_item[date_mm_location].min = VALUE_DATE_MM_MIN;
    s_param_item[date_mm_location].max = VALUE_DATE_MM_MAX;
    s_param_item[date_mm_location].step = VALUE_DATE_MM_STEP;
    s_param_item[date_mm_location].value = (int16) date.month;
    s_param_item[date_mm_location].cycle = TRUE;
    s_param_item[date_mm_location].adjust_func = date_adjust_func;
    s_param_item[date_mm_location].callback = NULL;
    s_param_item[date_mm_location].unit_id = S_DATE_MM;
    s_param_item[date_mm_location].max_number = 2;
    s_param_item[date_mm_location].value_str = NULL;

    s_param_item[date_yy_location].min = VALUE_DATE_YY_MIN;
    s_param_item[date_yy_location].max = VALUE_DATE_YY_MAX;
    s_param_item[date_yy_location].step = VALUE_DATE_YY_STEP;
    s_param_item[date_yy_location].value = (int16) ((date.year-2000) %(VALUE_DATE_YY_MAX+1));
    s_param_item[date_yy_location].cycle = TRUE;
    s_param_item[date_yy_location].adjust_func = date_adjust_func;
    s_param_item[date_yy_location].callback = NULL;
    s_param_item[date_yy_location].unit_id = S_DATE_YY;
    s_param_item[date_yy_location].max_number = 2;
    s_param_item[date_yy_location].value_str = NULL;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 3;
    s_param_private.items = s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;

    result = gui_set_parameter(PARAM_DATE, &s_param_com);
    if(result == RESULT_CONFIRM)//ȷ�����ò�������
    {
        date.year = 2000 + (uint16) s_param_com.private_data->items[date_yy_location].value;
        date.month = (uint8) s_param_com.private_data->items[date_mm_location].value;
        date.day = (uint8) s_param_com.private_data->items[date_dd_location].value;
#ifndef PC
        sys_set_date(&date);
#endif
        return (app_result_e) RESULT_REDRAW; //RESULT_MENU_CUR; //������һ��
    }
    else
    {
        return result;
    }
}
