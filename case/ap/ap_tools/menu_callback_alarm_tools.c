/*
 *******************************************************************************
 *                              USDK
 *                             Module: TOOLS AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_alarm_tools.C
 * History:
 *      <johnsen>    <2011-9-25 20:43>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */

#include "ap_tools.h"

extern const uint8 am[6];
//{ 'A', 0, 'M', 0, 0, 0 };
extern const uint8 pm[6];
//{ 'P', 0, 'M', 0, 0, 0 };
static const char card_driver_name[] = "card.drv";
static const char flash_driver_name[] = "nand.drv";


/*
 *****************************************************************************
 * \par  Description:
 *  ��ͨ˵��     : ���ص���������ȡ�����ӿ��صĲ˵����
 * \param[in]    ����
 * \param[out]   : ���ز˵����
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_alarm1_onoff_callback(void * title_action)
{
    menu_title_action_t* menu_title_shut_down;
    menu_title_shut_down = (menu_title_action_t*) title_action;
    if (g_alarm_vars.g_alarm.enable == 0)
    {
        menu_title_shut_down->str_id = S_ALARM1_OFF;
    }
    else
    {
        menu_title_shut_down->str_id = S_ALARM1_ON;
    }
    menu_title_shut_down->menu_entry = -1; //�������
    menu_title_shut_down->ret_str = NULL;
    menu_title_shut_down->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 * ��ͨ˵��      : ���� alarm1 ʹ��Ϊ"��"
 * \param[in]    ����
 * \param[out]   : RESULT_MENU_PARENT
 * \retval       ��������һ��
 * \note:
 *
 ******************************************************************************
 */
app_result_e set_alarm1_off_callback(void *menu_title_void)
{
    g_alarm_vars.g_alarm.enable = 0;

    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 * ��ͨ˵��      : ���� alarm1 ʹ��Ϊ"��"
 * \param[in]    ����
 * \param[out]   : RESULT_MENU_PARENT
 * \retval       ��������һ��
 * \note:
 *
 ******************************************************************************
 */
app_result_e set_alarm1_on_callback(void *menu_title_void)
{
    g_alarm_vars.g_alarm.enable = 1;

    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 * ��ͨ˵��      : ���ص���������ȡ��alarm1�������õĲ˵����
 * \param[in]    ����
 * \param[out]   : ���ز˵����
 * \retval       ��������һ��
 * \note:
 *
 ******************************************************************************
 */
app_result_e set_alarm1_cyc_callback(void * title_action)
{
    menu_title_action_t* menu_title_shut_down;
    menu_title_shut_down = (menu_title_action_t*) title_action;
    if (g_alarm_vars.g_alarm.cycle_flag == 0)
    {
        menu_title_shut_down->str_id = S_ALARM1_CYC_ONCE;
    }
    else if (g_alarm_vars.g_alarm.cycle_flag == 1)
    {
        menu_title_shut_down->str_id = S_ALARM1_CYC_DAYS;
    }
    else
    {
        menu_title_shut_down->str_id = S_ALARM1_CYC_WORK;
    }

    menu_title_shut_down->menu_entry = -1;
    menu_title_shut_down->ret_str = NULL;
    menu_title_shut_down->source_id = -1;
    return (app_result_e) RESULT_MENU_SON;
}

/*
 *****************************************************************************
 * \par  Description:
 * ��ͨ˵��      : ����alarm1 ������Ϊ����
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note:
 *
 ******************************************************************************
 */
app_result_e alarm1_once_callback(void *menu_title_void)
{
    app_result_e result;

    g_alarm_vars.g_alarm.cycle_flag = 0;
    result = calendar_main(FROM_ALARM);
    if (result == RESULT_REDRAW)
    {
        result = RESULT_MENU_PARENT; //���ظ�Ŀ¼
    }

    return result;
}

/*
 *****************************************************************************
 * \par  Description:
 * ��ͨ˵��      : ����alarm1 ������Ϊÿ��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note:
 *
 ******************************************************************************
 */
app_result_e alarm1_days_callback(void *menu_title_void)
{
    g_alarm_vars.g_alarm.cycle_flag = 1;

    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 * ��ͨ˵��      : ����alarm1 ������Ϊ������
 * \param[in]    ����
 * \param[out]   : RESULT_MENU_PARENT
 * \retval       ��������һ��
 * \note:
 *
 ******************************************************************************
 */
app_result_e alarm1_work_callback(void *menu_title_void)
{
    g_alarm_vars.g_alarm.cycle_flag = 2;

    return (app_result_e) RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���ص������������� alarm1 ʱ��
 * \param[in]    void
 * \param[out]   none
 * \return       RESULT_REDRAW
 * \retval       none
 * \ingroup      menu_callback_alarm_tools.c
 * \note
 *******************************************************************************/
app_result_e set_alarm1_time_callback(void *menu_title_void)
{
    app_result_e result;

    result = set_alarm_time_callback(&g_alarm_vars.g_alarm.start_time);

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���ص������������� alarm_fm ��ʼʱ��
 * \param[in]    void
 * \param[out]   none
 * \return       RESULT_REDRAW
 * \retval       none
 * \ingroup      menu_callback_alarm_tools.c
 * \note
 *******************************************************************************/
app_result_e set_fmtime_on_callback(void *menu_title_void)
{
    time_t tmp_time;
    app_result_e result;

    libc_memcpy(&tmp_time, &g_alarm_vars.g_aufm.start_time, sizeof(time_t));
    //set_alarm_time_callback(&g_alarm_vars.g_aufm.start_time);
    result = set_alarm_time_callback(&tmp_time);
    //�Ƚ�ʱ���Ƿ����
    if (libc_memcmp(&tmp_time, &g_alarm_vars.g_aufm.end_time, sizeof(time_t)) != 0)
    {
        //����ȵ���r
        libc_memcpy(&g_alarm_vars.g_aufm.start_time, &tmp_time, sizeof(time_t));
    }
    else
    {
        //������
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_SETTIME_FAIL);
    }
    return result; //������һ��
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ���ص������������� alarm_fm ����ʱ��
 * \param[in]    void
 * \param[out]   none
 * \return       RESULT_REDRAW
 * \retval       none
 * \ingroup      menu_callback_alarm_tools.c
 * \note
 *******************************************************************************/
app_result_e set_fmtime_off_callback(void *menu_title_void)
{
    time_t tmp_time;
    app_result_e result;

    libc_memcpy(&tmp_time, &g_alarm_vars.g_aufm.end_time, sizeof(time_t));
    //set_alarm_time_callback(&g_alarm_vars.g_aufm.end_time);
    result = set_alarm_time_callback(&tmp_time);
    //�Ƚ�ʱ���Ƿ����
    if (libc_memcmp(&tmp_time, &g_alarm_vars.g_aufm.start_time, sizeof(time_t)) != 0)
    {
        //����ȵ���r
        libc_memcpy(&g_alarm_vars.g_aufm.end_time, &tmp_time, sizeof(time_t));
    }
    else
    {
        //������
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_SETTIME_FAIL);
    }
    return result; //������һ��
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���ص��������ڵ���am/pm ��ʾ�ַ���
 * \param[in]    void
 * \param[out]   none
 * \return       ADJUST_RESULT_NULL
 * \retval       none
 * \ingroup      menu_callback_alarm_tools.c
 * \note
 *******************************************************************************/
adjust_result_e ampm_adjust_func(parambox_one_t *params, uint8 active, bool inc)
{
    if (params[POSITION_AMPM].value == 0)
    {
        params[POSITION_AMPM].value_str = &am;
    }
    else
    {
        params[POSITION_AMPM].value_str = &pm;
    }
    return ADJUST_RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ����time������ʾ
 * \param[in]    time_t *t_time
 * \param[out]   none
 * \return       RESULT_REDRAW
 * \retval       none
 * \ingroup      menu_callback_alarm_tools.c
 * \note
 *******************************************************************************/
app_result_e set_alarm_time_callback(time_t *t_time)
{
    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item[3];
    uint8 amflag;
    app_result_e result;
    time_t alarmtime;

    alarmtime.hour = t_time->hour;
    alarmtime.minute = t_time->minute;
    //sys_get_time(&alarmtime);

    if (alarmtime.hour < 12)
    {
        amflag = 1;
    }
    else
    {
        amflag = 0;
    }
    if (g_comval.time_format == VALUE_12HOURS_FORMAT)
    {
        s_param_item[0].min = VALUE_TIME_HH_12H_MIN;
    }
    else
    {
        s_param_item[0].min = VALUE_TIME_HH_24H_MIN;
    }
    if (g_comval.time_format == VALUE_12HOURS_FORMAT)
    {
        s_param_item[0].max = VALUE_TIME_HH_12H_MAX;
    }
    else
    {
        s_param_item[0].max = VALUE_TIME_HH_24H_MAX;
    }
    s_param_item[0].step = VALUE_TIME_HH_STEP;
    if (g_comval.time_format == VALUE_12HOURS_FORMAT)
    {
        alarmtime.hour = alarmtime.hour % 12;
        if (alarmtime.hour == 0)
        {
            alarmtime.hour = 12;
        }
    }
    else
    {
        alarmtime.hour = alarmtime.hour % 24;
    }
    s_param_item[0].value = (int16) alarmtime.hour;
    s_param_item[0].cycle = TRUE;
    s_param_item[0].adjust_func = NULL;
    s_param_item[0].callback = NULL;
    s_param_item[0].unit_id = S_TIME_HH;
    s_param_item[0].max_number = 2;
    s_param_item[0].value_str = NULL;

    s_param_item[1].min = VALUE_TIME_MM_MIN;
    s_param_item[1].max = VALUE_TIME_MM_MAX;
    s_param_item[1].step = VALUE_TIME_MM_STEP;
    s_param_item[1].value = (int16) alarmtime.minute;
    s_param_item[1].cycle = TRUE;
    s_param_item[1].adjust_func = NULL;
    s_param_item[1].callback = NULL;
    s_param_item[1].unit_id = S_TIME_MM;
    s_param_item[1].max_number = 2;
    s_param_item[1].value_str = NULL;

    s_param_item[POSITION_AMPM].min = VALUE_TIME_AM;
    s_param_item[POSITION_AMPM].max = VALUE_TIME_PM;
    s_param_item[POSITION_AMPM].step = VALUE_TIME_AMPM_STEP;
    if (amflag == 1)
    {
        s_param_item[POSITION_AMPM].value = VALUE_TIME_AM;
    }
    else
    {
        s_param_item[POSITION_AMPM].value = VALUE_TIME_PM;
    }
    s_param_item[POSITION_AMPM].cycle = TRUE;
    s_param_item[POSITION_AMPM].adjust_func = ampm_adjust_func;
    s_param_item[POSITION_AMPM].callback = NULL;
    s_param_item[POSITION_AMPM].unit_id = S_TIME_AMPM;
    s_param_item[POSITION_AMPM].max_number = 5;
    if (amflag == 1)
    {
        s_param_item[POSITION_AMPM].value_str = am;
    }
    else
    {
        s_param_item[POSITION_AMPM].value_str = pm;
    }

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    if (g_comval.time_format == VALUE_24HOURS_FORMAT)
    {
        s_param_private.param_cnt = 2;
    }
    else
    {
        s_param_private.param_cnt = 3;
    }
    s_param_private.items = (parambox_one_t *) s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;
    if (g_comval.time_format == VALUE_24HOURS_FORMAT)
    {
        result = gui_set_parameter(PARAM_TIME_24, &s_param_com);
    }
    else
    {
        result = gui_set_parameter(PARAM_TIME_12, &s_param_com);
    }
    if (result == RESULT_CONFIRM)//ȷ�����ò�������
    {
        alarmtime.minute = (uint8) s_param_com.private_data->items[1].value;
        if (g_comval.time_format == VALUE_12HOURS_FORMAT)
        {
            if ((s_param_com.private_data->items[POSITION_AMPM].value == VALUE_TIME_PM)
                    && (s_param_com.private_data->items[0].value == 12)) //PM 12�� ��������
            {
                alarmtime.hour = 12;
            }
            else if ((s_param_com.private_data->items[POSITION_AMPM].value == VALUE_TIME_AM)
                    && (s_param_com.private_data->items[0].value == 12))
            {
                alarmtime.hour = 0;

            }
            else if ((s_param_com.private_data->items[POSITION_AMPM].value == VALUE_TIME_PM)
                    && (s_param_com.private_data->items[0].value != 12))
            {
                alarmtime.hour = 12 + (uint8) s_param_com.private_data->items[0].value;
            }
            else
            {
                alarmtime.hour = (uint8) s_param_com.private_data->items[0].value;

            }
        }
        else
        {
            alarmtime.hour = (uint8) s_param_com.private_data->items[0].value;
        }
        //    alarmtime.second = 0;
        t_time->hour = alarmtime.hour;
        t_time->minute = alarmtime.minute;
        t_time->second = 0;

        return (app_result_e) RESULT_REDRAW; //������һ��
    }
    else
    {
        return result;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���ص������������� alarm1/ alarm_fm ����
 * \param[in]    void
 * \param[out]   none
 * \return       RESULT_REDRAW
 * \retval       none
 * \ingroup      menu_callback_alarm_tools.c
 * \note
 *******************************************************************************/
app_result_e set_alarm_volume_callback(uint8 *v_volume)
{
    app_result_e set_result;

    set_result = gui_volumebar(v_volume, &sys_comval->volume_limit, SET_VOLUME_ALARM);
    if ((set_result == RESULT_CONFIRM) || (set_result == RESULT_REDRAW))
    {
        set_result = RESULT_REDRAW;
    }
    return set_result;
}

app_result_e alarm1_volume_callback(void *menu_title_void)
{
    app_result_e result;

    result = set_alarm_volume_callback(&g_alarm_vars.g_alarm.volume);

    return result;
}

app_result_e fm_volume_callback(void *menu_title_void)
{
    app_result_e result;

    result = set_alarm_volume_callback(&g_alarm_vars.g_aufm.volume);

    return result;
}
/*
 *****************************************************************************
 * \par  Description:
 *  ��ͨ˵��     : ���ص���������ȡ�� auto_fm ��\�ر� �Ĳ˵����
 * \param[in]    ����
 * \param[out]   : RESULT_MENU_SON
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_fm_onoff_callback(void * title_action)
{
    menu_title_action_t* menu_title_shut_down;
    menu_title_shut_down = (menu_title_action_t*) title_action;
    if (g_alarm_vars.g_aufm.enable == 0)
    {
        menu_title_shut_down->str_id = S_AUFM_OFF;
    }
    else
    {
        menu_title_shut_down->str_id = S_AUFM_ON;
    }
    menu_title_shut_down->menu_entry = -1; //�������
    menu_title_shut_down->ret_str = NULL;
    menu_title_shut_down->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 * ��ͨ˵��      : ���� alarm_fm ʹ��Ϊ"��"
 * \param[in]    ����
 * \param[out]   : RESULT_MENU_PARENT
 * \retval       ��������һ��
 * \note:
 *
 ******************************************************************************
 */
app_result_e set_fm_off_callback(void *menu_title_void)
{
    g_alarm_vars.g_aufm.enable = 0;

    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 * ��ͨ˵��      : ���� alarm_fm ʹ��Ϊ"��"
 * \param[in]    ����
 * \param[out]   : RESULT_MENU_PARENT
 * \retval       ��������һ��
 * \note:
 *
 ******************************************************************************
 */
app_result_e set_fm_on_callback(void *menu_title_void)
{
    g_alarm_vars.g_aufm.enable = 1;

    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *  ��ͨ˵��     : ���ص���������ȡ�� auto_fm�������� �Ĳ˵����
 * \param[in]    ����
 * \param[out]   : ���ز˵����
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_fm_cyc_callback(void * title_action)
{
    menu_title_action_t* menu_title_shut_down;
    menu_title_shut_down = (menu_title_action_t*) title_action;
    if (g_alarm_vars.g_aufm.cycle_flag == 0)
    {
        menu_title_shut_down->str_id = S_AUFM_CYC_ONCE;
    }
    else if (g_alarm_vars.g_aufm.cycle_flag == 1)
    {
        menu_title_shut_down->str_id = S_AUFM_CYC_DAYS;
    }
    else if (g_alarm_vars.g_aufm.cycle_flag == 2)
    {
        menu_title_shut_down->str_id = S_AUFM_CYC_WORK;
    }
    else
    {
    }

    menu_title_shut_down->menu_entry = -1; //�������
    menu_title_shut_down->ret_str = NULL;
    menu_title_shut_down->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *  ��ͨ˵��     : ����auto_fm���� Ϊ����
 * \param[in]    ����
 * \param[out]   : RESULT_MENU_PARENT
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_fm_once_callback(void *menu_title_void)
{
    app_result_e result;

    g_alarm_vars.g_aufm.cycle_flag = 0;
    result = calendar_main(FROM_ALARMFM);
    if (result == RESULT_REDRAW)
    {
        result = RESULT_MENU_PARENT; //���ظ�Ŀ¼
    }
    return result;
}
/*
 *****************************************************************************
 * \par  Description:
 *  ��ͨ˵��     : ����auto_fm���� Ϊÿ��
 * \param[in]    ����
 * \param[out]   : RESULT_MENU_PARENT
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_fm_days_callback(void *menu_title_void)
{
    g_alarm_vars.g_aufm.cycle_flag = 1;

    return (app_result_e) RESULT_MENU_PARENT;
}
/*
 *****************************************************************************
 * \par  Description:
 *  ��ͨ˵��     : ����auto_fm���� Ϊ������
 * \param[in]    ����
 * \param[out]   : RESULT_MENU_PARENT
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_fm_work_callback(void *menu_title_void)
{
    g_alarm_vars.g_aufm.cycle_flag = 2;

    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *  ��ͨ˵��     : ���ص���������ȡ��auto_rec���صĲ˵����
 * \param[in]    ����
 * \param[out]   : ���ز˵����
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e fmrec_onoff_callback(void * title_action)
{
    menu_title_action_t* menu_title_shut_down;
    menu_title_shut_down = (menu_title_action_t*) title_action;
    if (g_alarm_vars.g_aufm.fmrec_enable == 0)
    {
        menu_title_shut_down->str_id = S_RECORD_OFF;
    }
    else
    {
        menu_title_shut_down->str_id = S_RECORD_ON;
    }
    menu_title_shut_down->menu_entry = -1; //�������
    menu_title_shut_down->ret_str = NULL;
    menu_title_shut_down->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 * ��ͨ˵��      : ���� fm¼��ʹ��Ϊ"��"
 * \param[in]    ����
 * \param[out]   : RESULT_MENU_PARENT
 * \retval       ��������һ��
 * \note:
 *
 ******************************************************************************
 */
app_result_e fmrec_off_callback(void *menu_title_void)
{
    g_alarm_vars.g_aufm.fmrec_enable = 0;

    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 * ��ͨ˵��      : ���� fm¼��ʹ��Ϊ"��"
 * \param[in]    ����
 * \param[out]   : RESULT_MENU_PARENT
 * \retval       ��������һ��
 * \note:
 *
 ******************************************************************************
 */
app_result_e fmrec_on_callback(void *menu_title_void)
{
    g_alarm_vars.g_aufm.fmrec_enable = 1;

    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 * ��ͨ˵��      : ���ص������������� fm ��Ƶ��
 * \param[in]    ����
 * \param[out]   : RESULT_REDRAW
 * \retval       ���ػ�UI����
 * \note:
 *
 ******************************************************************************
 */
app_result_e set_fm_fq_callback(void *menu_title_void)
{
    app_result_e result;

    result = set_fmfreq_main();
    if ((result == RESULT_REDRAW) || (result == RESULT_IGNORE))
    {
        result = RESULT_REDRAW;
    }

    return result;
}

