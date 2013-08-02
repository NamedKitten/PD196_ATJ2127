/*******************************************************************************
 *                              US212A
 *                            Module: TOOLS_MAIN
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      liuweizhan    2011-12-12 17:03:16           1.0              build this file
 *******************************************************************************/
/*!
 * \file     tools_main.c
 * \brief
 * \author   liuweizhan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2011-12-12
 *******************************************************************************/
#include "ap_tools.h"

/*main��Ҫ���������ֵ����������ʱ��� __start �����ã���ϵͳ��Ա���䣩*/
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS; //�߳�ջ��ָ��
#endif
uint8 prio = AP_FRONT_LOW_PRIO;  //�������ȼ�

#ifdef PC
extern menu_cb_func _get_menu_func(uint16 str_id);
extern menu_cb_leaf _get_callback_func(uint16 str_id);
extern menu_cb_option _get_menu_option(uint16 str_id);
#endif

/*ϵͳ����VM����*/
comval_t g_comval;
/*ALARM VM����*/
alarm_vars_t  g_alarm_vars;
/*���ڹ�������*/
date_t date;
/*ʱ�乫������*/
//time_t time;
/*alarm set���˳��ص�����*/
uint8 alarm_set_flag = 1;
/*alarmfm set���˳��ص�����*/
uint8 fmrec_set_flag = 1;

//for stopwatch
/*stopwatch��ʱ����*/
StopWatch_time_t sw_realtime;
/*����stopwatch��5���ʱ����*/
StopWatch_time_t swtime[5];
/*stopwatch��ʱ״̬*/
sw_status_e  sw_status;
//uint8  sw_itemnum;
/*stopwatch��ʾ��־*/
uint8  sw_showflag;
/*stopwatch��ʱʹ�ܱ���*/
uint8 CtcCntEnable = 0;

//0.5s��ʱ��
int8 g_halfsec_timer = 0xff;
//MSG_SD_OUTʹ�ã�GUI_DIRECTORY����
bool g_dir_flag = FALSE;

/*��ʱ������*/
#define APP_TIMER_COUNT 2
app_timer_t tools_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡVM����
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     TOOLS
 * \note
 *******************************************************************************/
static void _tools_read_var(void)
{
    setting_comval_t temp_val;
    date_t cur_date;
    time_t cur_time;
    uint8 time_format_tmp;
    //ϵͳ������ȡ
    sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));
    /*��ʼ��ϵͳ��VM����*/
#ifndef PC
    if (temp_val.g_comval.magic != MAGIC_SETTING)
#endif
    {
        temp_val.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&temp_val);
        sys_vm_write(&temp_val, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_val.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    /*��ȡalarm VM����������Ϣ*/
    sys_vm_read(&g_alarm_vars, VM_ALARM, sizeof(g_alarm_vars));
    /*��ʼ��alarm VM����*/
#ifndef PC
    if (g_alarm_vars.magic != MAGIC_ALARM)
#endif
    {
        //����VM����
        g_alarm_vars.magic = MAGIC_ALARM;
        g_alarm_vars.timer_flag = 0;
        sys_get_time(&cur_time);
        sys_get_date(&cur_date);

        //alarm VM����
        g_alarm_vars.g_alarm.enable = 0;
        g_alarm_vars.g_alarm.alarm_count = 0;
        g_alarm_vars.g_alarm.cycle_flag = 1;
        libc_memset(g_alarm_vars.g_alarm.cycle.select_week.week_day, 0, 7);
        g_alarm_vars.g_alarm.cycle.one_time.year = cur_date.year;
        g_alarm_vars.g_alarm.cycle.one_time.month = cur_date.month;
        g_alarm_vars.g_alarm.cycle.one_time.day = cur_date.day;
        g_alarm_vars.g_alarm.start_time.hour = cur_time.hour;
        g_alarm_vars.g_alarm.start_time.minute = cur_time.minute;
        g_alarm_vars.g_alarm.start_time.second = 0;
        g_alarm_vars.g_alarm.volume = 15;

        g_alarm_vars.g_alarm.ring_flag = 0;
        libc_memset(&g_alarm_vars.g_alarm.path, 0, sizeof(file_path_info_t)); //��ʼ�� alarm path ��Ϣ
        g_alarm_vars.g_alarm.path.file_source = FSEL_TYPE_COMMONDIR;
        g_alarm_vars.g_alarm.path.file_path.dirlocation.disk = DISK_H;

        //alarm_fm VM����
        g_alarm_vars.g_aufm.enable = 0;
        g_alarm_vars.g_aufm.cycle_flag = 1;
        libc_memset(g_alarm_vars.g_aufm.cycle.select_week.week_day, 0, 7);
        g_alarm_vars.g_aufm.cycle.one_time.year = cur_date.year;
        g_alarm_vars.g_aufm.cycle.one_time.month = cur_date.month;
        g_alarm_vars.g_aufm.cycle.one_time.day = cur_date.day;
        g_alarm_vars.g_aufm.start_time.hour = cur_time.hour;
        g_alarm_vars.g_aufm.start_time.minute = cur_time.minute;
        g_alarm_vars.g_aufm.start_time.second = 0;
        g_alarm_vars.g_aufm.end_time.hour = g_alarm_vars.g_aufm.start_time.hour;
        if(g_alarm_vars.g_aufm.start_time.minute > 56)//����ʱ��������
        {
            g_alarm_vars.g_aufm.end_time.hour ++;
            time_format_tmp = g_comval.time_format;
            g_comval.time_format = VALUE_24HOURS_FORMAT;
            if(g_alarm_vars.g_aufm.end_time.hour > 23)
            {
                g_alarm_vars.g_aufm.end_time.hour = 0;
            }
            g_comval.time_format = time_format_tmp;
        }
        g_alarm_vars.g_aufm.end_time.minute = (g_alarm_vars.g_aufm.start_time.minute + 3)%60;
        g_alarm_vars.g_aufm.end_time.second = 0;
        g_alarm_vars.g_aufm.frequency = 0x55cc; //Ĭ��Ƶ��87.5
        g_alarm_vars.g_aufm.volume = 15;
        g_alarm_vars.g_aufm.fmrec_enable = 0;

        sys_vm_write(&g_alarm_vars, VM_ALARM);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ap�����ʼ��
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     TOOLS
 * \note
 *******************************************************************************/
static bool _tools_app_init(void)
{
    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_TOOLS, APP_TYPE_GUI);

    //��ʼ����ʱ��
    init_app_timers(tools_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init(tool_privmsg_deal);

    sys_timer_init();

    //��ʼui��Դ�ļ�
    ui_res_open("common.sty", UI_COM);
    ui_res_open("tools.sty", UI_AP);

    //��ǰ̨Ӧ�ò˵����ýű�
    com_open_confmenu_file("tools.mcg");

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
#endif

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ap�˳�����
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     TOOLS
 * \note
 *******************************************************************************/
static bool _tools_app_deinit(void)
{
    //�ر�Ӧ�ò˵����ýű�
    com_close_confmenu_file();

    sys_timer_exit();

    //�ر�ui��Դ�ļ�
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    //ִ��applib���ע������
    applib_quit();
    return TRUE;
}
uint8 _detect_usb_in (void)
{
    if(((act_readb(LINESTATUS) & 0x18) == 0) &&
        ((act_readb(CHG_CTL) & 0x80) != 0))
/*
    if(((act_readb(DPDMCTRL) & 0x40) != 0) &&
        ((act_readb(CHG_CTL) & 0x80) != 0))*/
    {
        return 1;
    }
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    tools Ӧ�÷���ֵ������������Ӧ�ķ���ֵת������Ϣ����
 * \param[in]    result app_result_e ������������ֵ
 * \param[out]   none
 * \return       the result
 * \ingroup      TOOLS
 * \note
 *******************************************************************************/
static app_result_e _deal_tools_result(app_result_e tools_result)
{
    msg_apps_t msg;
    //engine_state_e engine_status;
    engine_type_e engine_type;
    bool need_send_msg = TRUE;

    //engine_status = get_engine_state();
    //��process manager���ʹ�������������Ϣ
    switch (tools_result)
    {
        case RESULT_MAIN_APP:
        msg.content.data[0] = APP_ID_MAINMENU;
        msg.content.data[1] = PARAM_FROM_TOOLS;
        break;

        case RESULT_DATAEXCH:
        msg.content.data[0] = APP_ID_ALARM;
        msg.content.data[1] = PARAM_FROM_TOOLS;
        break;

        case RESULT_UDISK:
        //engine_type = get_engine_type();
        //if(engine_type != ENGINE_NULL)
        //{
        //    msg.type = MSG_KILL_APP_SYNC;
        //    if (engine_type == ENGINE_MUSIC)
        //    {
        //        msg.content.data[0] = APP_ID_MENGINE;
        //    }
        //    else// if (engine_type == ENGINE_RADIO)
        //    {
        //        msg.content.data[0] = APP_ID_FMENGINE;
        //    }
        //    //����ͬ����Ϣ ɱ������
        //    send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
        //}
        msg.content.data[0] = APP_ID_UDISK;
        msg.content.data[1] = PARAM_FROM_TOOLS;
        break;

        case RESULT_AUTO_UPGRADE:
        msg.content.data[0] = APP_ID_UPGRADE;
        msg.content.data[1] = PARAM_FROM_TOOLS;
        break;

        case RESULT_APP_QUIT:
        default:
        need_send_msg = FALSE;
        break;
    }

    if (need_send_msg == TRUE)
    {
        private_msg_t private_msg;

        if(tools_result == RESULT_UDISK)
        {
            //����manager����,����Ҫ��engine
            msg.type = MSG_USB_TRANS;
            //��process manager���ʹ���������Ϣ ͬ����Ϣ�����첽��Ϣ
            send_async_msg(APP_ID_MANAGER, &msg);

            //�ȴ�AP�˳���Ϣ MSG_APP_QUIT ���ٷ��� RESULT_APP_QUIT��Ӧ��ֱ���˳�
            while(1)
            {
                //����10ms�����������
                sys_os_time_dly(1);
                if((get_app_msg(&private_msg) == TRUE) && (private_msg.msg.type == MSG_APP_QUIT))
                {
                    tools_result = RESULT_APP_QUIT;
                    break;
                }
            }
        }
        else
        {
            msg.type = MSG_CREAT_APP;
            //��process manager���ʹ���������Ϣ ͬ����Ϣ�����첽��Ϣ
            send_async_msg(APP_ID_MANAGER, &msg);
        }
    }
    return tools_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Tools Ӧ�õ���ں����ͳ���������ģ��
 * \param[in]    argc �ɽ��̹���������Ĳ�������ʾ�Ӻ���ģʽ����
 * \param[in]    argv ���ò���
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e result;
    scene_tools_e next_scene;

    _tools_app_init();

    _tools_read_var();

    next_scene = SCENE_MENULIST;

    while (next_scene != SCENE_EXIT)
    {
        switch (next_scene)
        {
            case SCENE_MENULIST:
            result = tools_menulist();
            switch (result)
            {
                case RESULT_CALENDAR:
                next_scene = SCENE_CALENDAR;
                break;
                case RESULT_STOPWATCH:
                next_scene = SCENE_STOPWATCH;
                break;
                case RESULT_DATAEXCH:
                //result = RESULT_MAIN_APP; //��ʱ�˳���mainmanu
                next_scene = SCENE_EXIT;
                break;
                case RESULT_UDISK:
                if (_detect_usb_in() == 0)
                {
                    break;
                }
                next_scene = SCENE_EXIT;
                break;
                case RESULT_APP_QUIT:
                next_scene = SCENE_EXIT;
                break;

                case RESULT_AUTO_UPGRADE:
                next_scene = SCENE_EXIT;
                break;

                default:
                result = RESULT_MAIN_APP;
                next_scene = SCENE_EXIT;
                break;
            }
            break;

            case SCENE_CALENDAR:
            result = calendar_main(FROM_MENU);
            if(result == RESULT_TOOLS_MENULIST)
            {
                next_scene = SCENE_MENULIST;
            }
            else if(result == RESULT_APP_QUIT)
            {
                next_scene = SCENE_EXIT;
            }
            else
            {
                //for QAC
            }
            break;

            case SCENE_STOPWATCH:
            result = stopwatch_main();
            if(result == RESULT_TOOLS_MENULIST)
            {
                next_scene = SCENE_MENULIST;
            }
            else if(result == RESULT_APP_QUIT)
            {
                next_scene = SCENE_EXIT;
            }
            else
            {
                //for QAC
            }
            break;

            default:
            next_scene = SCENE_EXIT;
            break;
        }
    }

    _deal_tools_result(result);

    _tools_app_deinit();

    return result;//dummy
}
/*! \endcond */

