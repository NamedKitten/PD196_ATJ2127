/*
 *******************************************************************************
 *                ACTOS AP
 *        system setting ap of sdk 3.0
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: main.c,v 1.7 2009/05/22 11:29:01 vicky Exp $
 *******************************************************************************
 */
#include "case_include.h"
#include "psp_includes.h"
#include "ap_alarm.h"

/*ϵͳ����VM����*/
comval_t g_comval;
//setting_comval_t g_setting_comval;
/*ALARM VM����*/
alarm_alarm_t g_alarm;
/*ALARM_FM VM����*/
alarm_fm_t g_alarmfm;
/**/
uint8 timer_flag;
/*��ʱ����snoozeʱ��*/
time_t stimer_tmp;
/*����״̬��Ϣ*/
mengine_status_t g_music_status;

/*���Ӷ���ID*/
uint8 alarm_pid = 0;
/*���Ӷ���Ч��*/
bool g_alarm_pid = TRUE;
/*��ʱ�˳���־*/
bool g_alarm_timeout = FALSE;

/*��ʱ��id*/
/*0.5s��ʱ��*/
int8 g_halfsec_timer = 0xff;
/*30s��ʱ��*/
int8 g_tirtysec_timer = 0xff;
/*���ӳ�ʱ����*/
uint8 g_timer_60 = 0;

//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT        2
/*!
 *  \brief
 *  app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
 */
app_timer_t alarm_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS;
#endif

INT8U prio = AP_FRONT_LOW_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 * \
 * \����gui��Ϣ
 * \param[in]    gui_msg  para1
 * \param[out]   none
 * \return       app_result_e result
 * \retval
 * \ingroup      alarm_main.c
 * \note
 */
/*******************************************************************************/
app_result_e alarm_privmsg_deal(private_msg_t *private_msg)
{
    app_result_e result = RESULT_NULL;

    /*ͬ����Ϣ����*/
    switch (private_msg->msg.type)
    {
        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;

        default:
        result = com_message_box(private_msg->msg.type);
        break;
    }//end of switch

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \��ȡVM������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool result
 * \retval       1 sucess
 * \retval       0 failed
 * \ingroup      alarm_main.c
 * \note
 */
/*******************************************************************************/
bool _load_cfg(void)
{
    alarm_vars_t temp_alarm_val;
    bool result;
    setting_comval_t temp_val;
    result = TRUE;
    //ϵͳ������ȡ
    sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));
    if (temp_val.g_comval.magic != MAGIC_SETTING)
    {
        temp_val.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&temp_val);
        sys_vm_write(&temp_val, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_val.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    //alarm vm������ȡ
    sys_vm_read(&temp_alarm_val, VM_ALARM, sizeof(alarm_vars_t));
    //��ֹ����δ��ʼ�����󱣴�
    timer_flag = temp_alarm_val.timer_flag;
    if (temp_alarm_val.magic != MAGIC_ALARM)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        result = FALSE;
    }
    libc_memcpy(&g_alarm, &temp_alarm_val.g_alarm, sizeof(alarm_alarm_t));
    libc_memcpy(&g_alarmfm, &temp_alarm_val.g_aufm, sizeof(alarm_fm_t)); //����fmʱ��

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \����VM������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \ingroup      alarm_main.c
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{
    alarm_vars_t temp_alarm_val;
    sys_vm_read(&temp_alarm_val, VM_ALARM, sizeof(alarm_vars_t));
    //����ı��alarm
    libc_memcpy(&temp_alarm_val.g_alarm, &g_alarm, sizeof(alarm_alarm_t));
    temp_alarm_val.timer_flag = timer_flag;
    libc_memcpy(&temp_alarm_val.snooze_time, &stimer_tmp, sizeof(time_t));
    sys_vm_write(&temp_alarm_val, VM_ALARM);
    //sys_vm_write(&g_setting_comval, VM_AP_SETTING);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_init(void)
 * \alarm ap�ĳ�ʼ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool result
 * \retval       1 sucess
 * \retval       0 failed
 * \ingroup      alarm_main.c
 * \note
 */
/*******************************************************************************/
bool _app_init(void)
{
    bool app_init_ret = TRUE;

    //��ȡ������Ϣ (����vm��Ϣ)
    if (_load_cfg() == FALSE)
    {
        app_init_ret = FALSE;
    }
    //����ui��Դ�ļ�
    ui_res_open("alarm.sty", UI_AP);
    ui_res_open("common.sty", UI_COM);

    //#ifdef PC
    //    load_menulist_simfunc(get_menu_func, get_callback_func, get_menu_option);
    //#endif

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_ALARM, APP_TYPE_GUI);

    applib_message_init(alarm_privmsg_deal);
    //��ʼ����ʱ��
    init_app_timers(alarm_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);
    //������init_app_timers �������
    sys_timer_init();

    return app_init_ret;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \alarm ap�˳�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       1 sucess
 * \retval       0 failed
 * \ingroup      alarm_main.c
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    //����ϵͳ��ʱ��
    sys_timer_exit();

    //ִ��applib���ע������
    applib_quit();

    //�ر�Ӧ�ò˵����ýű�
    com_close_confmenu_file();

    //�ر���Դ�ļ�
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    //save config дvram
    _save_cfg();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    alarm ap����ֵ������������Ӧ�ķ���ֵת������Ϣ����
 * \param[in]    app_result_e result������������ֵ
 * \param[in]    app_result_e enter_ap ap����ģʽ
 * \param[out]   none
 * \return       the result
 * \ingroup      alarm_main.c
 * \note
 *******************************************************************************/
static app_result_e _deal_result(app_result_e result, uint8 enter_ap, uint8 engine_sta)
{
    msg_apps_t msg;
    engine_state_e engine_status;
    engine_type_e engine_type;
    bool need_send_msg = TRUE;

    engine_status = get_engine_state();
    engine_type = get_engine_type();
    //�������δ��engine״̬
    if ((engine_status != ENGINE_STATE_NULL) && (engine_type == ENGINE_MUSIC))
    {
        if (result != RESULT_APP_QUIT)
            music_close_engine();
    }

    //��process manager���ʹ�������������Ϣ
    msg.type = MSG_CREAT_APP;
    msg.content.data[1] = PARAM_FROM_MAINMENU;
    switch (result)
    {
        case RESULT_ALARM_QUIT:
        if (enter_ap < APP_ID_MAX)
        {
            msg.content.data[0] = enter_ap;
            //��������alarm����mainmenu
            if (enter_ap == APP_ID_CONFIG)
            {
                msg.content.data[0] = APP_ID_MAINMENU;
            }
        }

        if (get_last_app() == APP_ID_VIDEO)
        {
            if (g_app_info_state.video_state == 1)
            {
                msg.content.data[0] = APP_ID_VIDEO;
                msg.content.data[1] = PARAM_BACKTO_PLAYING;
            }
        }

        if (enter_ap == APP_ID_MUSIC)
        {
            if (engine_sta == ENGINE_STATE_PLAYING)
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYING;
            }
            else //if(g_app_last_state.last_state == ENGINE_STATE_PAUSE)
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYED;
            }
        }
        else if (enter_ap == APP_ID_RADIO)
        {
            msg.content.data[1] = PARAM_FROM_CONFIG;
        }
        else
        {
            ;//nothing
        }
        break;

        case RESULT_APP_QUIT:
        need_send_msg = FALSE;
        break;
        //Ĭ���˳���mainmenu
        default:
        msg.content.data[0] = APP_ID_MAINMENU;
        break;
    }//end of switch (result)

    if (need_send_msg == TRUE)
    {
        //��process manager���ʹ���������Ϣ �첽��Ϣ
        send_async_msg(APP_ID_MANAGER, &msg);
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc  const char *argv[])
 * \alarm ap ������
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval
 * \ingroup      alarm_main.c
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e result;
    time_t alarmtime_temp; //����snoozeʱ��
    uint8 time_format_tmp;
    uint8 flag;
    engine_state_e engine_status;
    engine_type_e engine_type;
    //app_param_e enter_mode = (app_param_e)argc;
    uint8 engine_sta = ENGINE_STATE_NULL; //��¼����ʱengine״̬
    uint8 enter_mode = g_app_last_state.last_app; //��¼�Ӻ�ap����

    //����engine״̬����app id
    engine_status = get_engine_state();
    engine_type = get_engine_type();

    if (engine_status != ENGINE_STATE_NULL)
    {
        if (engine_type == ENGINE_MUSIC)
        {
            enter_mode = APP_ID_MUSIC;
        }
        else if (engine_type == ENGINE_RADIO)
        {
            enter_mode = APP_ID_RADIO;
        }
        else
        {
            //QAC
        }
        engine_sta = engine_status;
    }

    //applib��ʼ��
    _app_init();

    result = alarm_scene_playing();

    //�ް�����ʱ�˳�
    if (g_alarm_timeout == TRUE)
    {
        g_alarm.alarm_count++;
        if (g_alarm.alarm_count < 3)
        {
            //���������ж�+5m
            alarmtime_temp.hour = g_alarm.start_time.hour;
            alarmtime_temp.minute = g_alarm.start_time.minute;
            alarmtime_temp.minute = alarmtime_temp.minute + 5* g_alarm .alarm_count;
            //����ʱ��������
            if (alarmtime_temp.minute > 59)
            {
                alarmtime_temp.minute -= 60;
                alarmtime_temp.hour++;
                time_format_tmp = g_comval.time_format;
                g_comval.time_format = VALUE_24HOURS_FORMAT;
                if (alarmtime_temp.hour > 23)
                {
                    alarmtime_temp.hour = 0;
                }
                g_comval.time_format = time_format_tmp;
            }
            //libc_print("hr1",alarmtime_temp.hour);
            //libc_print("mn1",alarmtime_temp.minute);
            /*����ʱ��Ƚϣ�ȷ������alarm*/
            if (g_alarmfm.enable)
            {
                time_t time_tmp;
                sys_get_time(&time_tmp); //��ȡ��ǰϵͳʱ��
                flag = compare_curtime(&time_tmp, &g_alarmfm.start_time, &alarmtime_temp, 3);
                if (flag == 1)
                {
                    timer_flag = 0;
                    sys_set_alarm_time(&g_alarmfm.start_time);
                }
                else //if(flag == 2)
                {
                    timer_flag = 1;
                    libc_memcpy(&stimer_tmp, &alarmtime_temp, sizeof(time_t));
                    sys_set_alarm_time(&alarmtime_temp);
                }
            }
            else //if(g_alarmfm.enable == 0)
            {
                timer_flag = 1;
                libc_memcpy(&stimer_tmp, &alarmtime_temp, sizeof(time_t));
                sys_set_alarm_time(&alarmtime_temp);
            }
            // ����ʱʱ������
            //sys_set_alarm_time(&alarmtime_temp);
        }
        else //3�ι���Ĵ���
        {
            g_alarm.alarm_count = 0;
            /*����ʱ��Ƚϣ�ȷ������alarm*/
            if (g_alarmfm.enable)
            {
                time_t time_tmp;
                sys_get_time(&time_tmp); //��ȡ��ǰϵͳʱ��
                flag = compare_curtime(&time_tmp, &g_alarmfm.start_time, &g_alarm.start_time, 3);
                //flag = (uint8)CompareAndSet_timer_flag(&g_alarmfm, &g_alarm);
                if (flag == 1)
                {
                    timer_flag = 0;
                    sys_set_alarm_time(&g_alarmfm.start_time);
                }
                else //if (flag == 2)
                {
                    timer_flag = 1;
                    sys_set_alarm_time(&g_alarm.start_time);
                }
            }
            else //if(g_alarmfm.enable == 0)
            {
                timer_flag = 1;
                sys_set_alarm_time(&g_alarm.start_time);
            }
            // ��ԭʱ�����
            //sys_set_alarm_time(&g_alarm.start_time);
        }
    }
    else //�ǳ�ʱ�˳��Ĵ���
    {
        g_alarm.alarm_count = 0;
    }
    set_current_volume(g_comval.volume_current);//�ָ�ϵͳ����
    _deal_result(result, enter_mode, engine_sta);
    //applibж��
    _app_deinit();

    return result;
}

