/*******************************************************************************
 *                              US212A
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     config_main.c
 * \brief    ���ػ��Ĵ���
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "ap_config.h"

/*ȫ�ֱ�������*/
//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT     1

/*!
 *  \brief
 *  app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
 */
app_timer_t conf_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];
//ϵͳ����
setting_comval_t g_setting_comval;
//configӦ�ñ���
g_config_var_t g_config_var;
//wav¼�������д�ļ�ͷ�ṹ��
record_writehead_t g_writehead;
//�Ƿ��һ���ϵ��־
bool first_boot = FALSE;
//���������ö���
cfg_key_var_t g_cfg_key;
//���ͼƬid
uint8 charge_frame_id = 0;
//���������
uint8 backlight_cnt = 0;
//�����־
backlight_type_e backlight_flag = BACKLIGHT_ON;
//��ʱ��id
int8 timer_id;
//�����־
bool charge_full_flag = FALSE;
//u�����ݸ��±�־
uint32 update_flag = 0;

/*main��Ҫ���������ֵ����ϵͳ��Ա����*/
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS;
#endif
INT8U prio = AP_FRONT_LOW_PRIO;


/******************************************************************************/
/*!
 * \par  Description:
 *  ϵͳ��Ϣ������
 * \param[in]    Ӧ��˽����Ϣָ��
 * \param[out]   ��Ϣ���
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
app_result_e config_msg_dispatch(private_msg_t *private_msg)
{
    app_result_e result = RESULT_NULL;

    switch (private_msg->msg.type)
    {
        /* USB B�߲�����Ϣ */
        case MSG_USB_STICK:
        {
            //��ʾUSB ѡ��Ի���
            result = gui_usbconnect();
            if (result == RESULT_USB_TRANS)
            {
                //���� udisk ap
                msg_apps_t msg;

                if (g_setting_comval.g_comval.online_device == 0)
                {
                    msg.content.data[0] = APP_ID_UDISK;
                }
                else
                {
                    msg.content.data[0] = APP_ID_MTP;
                }
                msg.type = MSG_CREAT_APP; //��process manager���ʹ�������������Ϣ
                msg.content.data[1] = (uint8) PARAM_FROM_MAINMENU;
                send_async_msg(APP_ID_MANAGER, &msg);

                result = RESULT_APP_QUIT;
            }
            else
            {
                ; //QAC need
            }
        }
        break;

        /* USB B�߰γ���Ϣ */
        case MSG_USB_UNSTICK:
        {
            //ֻ��USBѡ��Ի����У����߷� udisk Ӧ�ó�粥������Ӧ����USB������������udisk Ӧ���Լ�����
            //�����USBѡ��Ի�����
            if(usb_selecting_state == TRUE)
            {
                //���� RESULT_USB_UNSTICK ���ɣ�ui_usbconnect �� RESULT_USB_UNSTICK תΪ RESULT_REDRAW ���˳��Ի���
                result = RESULT_USB_UNSTICK;
            }

            g_sys_counter.battery_counter = 100;//ǿ�ƶ�ȡ���״̬�͵�ص���
        }
        break;

        /* RTC��ʱ������Ϣ */
        case MSG_RTCALARM:
        {
            //��tools ����
            msg_apps_t msg;

            result = com_alarm_msg_dispatch();
            if (result == RESULT_ALARMFM)
            {
                msg.content.data[0] = APP_ID_RADIO;
            }
            else if (result == RESULT_FMRECORD)
            {
                msg.content.data[0] = APP_ID_RECORD;
            }
            else if (result == RESULT_ALARM)
            {
                msg.content.data[0] = APP_ID_ALARM;
            }
            else
            {
                break;
            }
            msg.type = MSG_CREAT_APP; //��process manager���ʹ�������������Ϣ
            msg.content.data[1] = (uint8) PARAM_FROM_ALARM;
            send_async_msg(APP_ID_MANAGER, &msg);

            result = RESULT_APP_QUIT;
        }
        break;

        /* ϵͳ�͵���Ϣ */
        case MSG_LOW_POWER:
        {
            style_infor_t style_infor;

            //��ʾ�͵�ͼ��
            style_infor.style_id = LOW_POWER_BG;
            style_infor.type = UI_COM;
            ui_show_picbox(&style_infor, NULL);

            //��ʾ�͵��ַ���
            style_infor.style_id = LOW_POWER_TEXT;
            style_infor.type = UI_AP;
            ui_show_textbox(&style_infor, NULL, TEXTBOX_DRAW_NORMAL);

            result = RESULT_ERROR;
        }
        break;

        /* ����� ADAPTOR ������Ϣ */
        case MSG_ADAPTOR_IN:
        {
            //������磬����������UI
            key_chargeset(CHARGE_START, ChargeCurrent250mA, BATFULL_LEVEL1);

            g_sys_counter.battery_counter = 100;//ǿ�ƶ�ȡ���״̬�͵�ص���
        }
        break;

        /*���������ߣ�����*/
        case MSG_EARPHONE_IN:
        {
            change_antenna_state(TRUE);
        }
        break;

        /*���������ߣ��γ�*/
        case MSG_EARPHONE_OUT:
        {
            change_antenna_state(FALSE);
        }
        break;

        default:
        break;
    }

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  ��ȡӦ��VMȫ�ֱ���,��ʼ��ȫ�ֱ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
static void _read_var(void)
{
    uint8 i;
    uint16 temp_ap_file_num[MAX_APP_NUM];

    //��ȡcommon VM����������Ϣ
    sys_vm_read(&g_setting_comval, VM_AP_SETTING, sizeof(g_setting_comval));
    //��ʼ��ϵͳ��VM����
    if (g_setting_comval.g_comval.magic != MAGIC_SETTING)
    {
        g_setting_comval.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&g_setting_comval);
        sys_vm_write(&g_setting_comval, VM_AP_SETTING);
    }

    com_set_sys_comval(&g_setting_comval.g_comval);

    sys_vm_read(&g_config_var, VM_AP_CONFIG, sizeof(g_config_var));
    //��ʼ��config��VM����
    if (g_config_var.magic != MAGIC_CONFIG)
    {
        //��һ���ϵ�
        first_boot = TRUE;
        g_config_var.magic = MAGIC_CONFIG;
        g_config_var.ap_id = APP_ID_MAINMENU;
        //��ȡ��������
        com_get_config_struct(CFG_KEY_ARRAY, (uint8 *) &g_cfg_key, sizeof(g_cfg_key));
        for (i = 0; i < g_cfg_key.total; i++)
        {
            g_config_var.keyID[i] = (uint8) g_cfg_key.keyID[i];
        }
        sys_vm_write(&g_config_var, VM_AP_CONFIG);
        //����Ӧ���ļ�����дΪ0
        libc_memset(temp_ap_file_num, 0, sizeof(temp_ap_file_num));
        sys_vm_write(&temp_ap_file_num, VM_FILE_NUM);
    }
    //��ȡ¼��������Ϣ
    sys_vm_read(&g_writehead, VM_AP_RECORD_WAV, sizeof(record_writehead_t));
    //u�����ݸ��±�־
    sys_vm_read(&update_flag, UDISK_VM, 4);
}


/******************************************************************************/
/*!
 * \par  Description:
 *  applib��ʼ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
bool _app_init(void)
{
    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_CONFIG, APP_TYPE_GUI);

    //��ʼ����ʱ��
    init_app_timers(conf_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init(config_msg_dispatch);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    configӦ�õ���ں���
 * \param[in]    0-����������ֵ���ػ�����16bit����ػ�ʱ��ǰ̨Ӧ��ID����16bit��ʾ��̨�����״̬
 * \param[out]   none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    private_msg_t private_msg;
    app_result_e result = RESULT_NULL;
    date_t init_data;
    time_t init_time;
    bool sd_in_flag;
    bool usb_in_flag;
    bool alarm_on_flag;
    uint8* key_table_addr;
    input_gui_msg_t gui_msg;

    //Ӧ�ñ�����ʼ��
    _read_var();
    //applib initial
    _app_init();
    //����
    if (((uint32)argc & 0xff) == SWITCH_ON)
    {
        //��װ��������
        sys_drv_install(DRV_GROUP_KEY, 0, "key.drv");
        //���ð���ֵ
        key_table_addr = key_getkeytabaddress();
        libc_memcpy(key_table_addr, g_config_var.keyID, sizeof(g_config_var.keyID));
        //��װ��ʾ����
        sys_drv_install(DRV_GROUP_LCD, 0, "drv_lcd.drv");
        sys_drv_install(DRV_GROUP_UI, 0, "drv_ui.drv");
        //��������
        ui_set_language(g_setting_comval.g_comval.language_id);
        //��Աȶ�
        com_set_contrast(g_setting_comval.g_comval.lightness);
        //��װaudio����
        sys_drv_install(DRV_GROUP_AUDIO_DEVICE, 0, "aud_dev.drv");
        //��������
        com_set_sound_volume(g_setting_comval.g_comval.volume_current);
        //���ð�����
        com_set_sound_keytone(g_setting_comval.g_comval.key_tone);

        //�ж��Ƿ���磬���ǣ���ʼ������2012-1-1��ʱ��00:00:00
        if ((act_readl(RTC_CTL) & 0x0010) == 0)
        {
            init_data.year = 2012;
            init_data.month = 1;
            init_data.day = 1;
            sys_set_date(&init_data);

            init_time.hour = 0;
            init_time.minute = 0;
            init_time.second = 0;
            sys_set_time(&init_time);
        }

        //��ʾ��������
        if(g_config_var.ap_id != APP_ID_MUSIC)
        {
//            draw_logo(SWITCH_ON);
        }
        //���ղ�����˽����Ϣ
        ui_res_open("common.sty", UI_COM);
        sd_in_flag = FALSE;
        usb_in_flag = FALSE;
        alarm_on_flag = FALSE;
        while (get_app_msg(&private_msg) == TRUE)
        {
            if (private_msg.msg.type == MSG_SD_IN)
            {
                sd_in_flag = TRUE;
                continue;
            }
            if (private_msg.msg.type == MSG_USB_STICK)
            {
                usb_in_flag = TRUE;
                continue;
            }
            if (private_msg.msg.type == MSG_RTCALARM)
            {
                alarm_on_flag = TRUE;
                sys_os_time_dly(50);//��ʱ500ms����֤����������Ϣ�ѷ��͵�ϵͳ��Ϣ������
                continue;
            }
            result = config_msg_dispatch(&private_msg);
            if ((result == RESULT_APP_QUIT) || (result == RESULT_ERROR))
            {
                break;
            }
        }
        //ȷ�Ͻ���u��
        if (usb_in_flag == TRUE)
        {
            private_msg.msg.type = MSG_USB_STICK;
            result = config_msg_dispatch(&private_msg);
        }
        //ȷ��alarm��Ϣ����
        else if(alarm_on_flag == TRUE)
        {
            private_msg.msg.type = MSG_RTCALARM;
            result = config_msg_dispatch(&private_msg);
        }
        else
        {
            ;//QAC need
        }
        ui_res_close(UI_COM);

        if (sd_in_flag == TRUE)
        {
            //��Ϊǰ���ȡ��Ϣʱ��sd card�������Ϣȡ�����ˣ����½�sd card�������Ϣ���ͻ�ϵͳ��Ϣ����
            uint16 msgtype;

            msgtype = MSG_SD_IN;
            sys_mq_send(MQ_ID_SYS, &msgtype);
        }

        //¼��������д
        if (g_writehead.rewrite_need == TRUE)
        {
            write_head();
            g_writehead.rewrite_need = FALSE;
            sys_vm_write(&g_writehead, VM_AP_RECORD_WAV);
        }

        if (result == RESULT_APP_QUIT)
        {
            ;//��config_msg_dispatch�������Ѿ���������ǰ̨AP��ֱ���˳�����
        }
        else if (result == RESULT_ERROR)
        {
            goto switch_off_here;
        }
        else
        {
            get_gui_msg(&gui_msg);
            if (gui_msg.data.kmsg.val == KEY_PLAY)
            {
                //this_filter_key_value = KEY_PLAY;
                com_filter_key_hold();
            }
            //����ap
            _creat_application(PARAM_ENTER_NORMAL);
        }
    }
    //nandflash reclaim ����
    else if(((uint32)argc & 0xff) == NANDFLASH_RECLAIM)
    {
        config_reclaim(argc);
    }
    else//�����ǹػ�����
    {
        if ((((uint32) argc >> 8) & 0xff) == APP_ID_RADIO)
        {
            g_config_var.ap_id = APP_ID_RADIO;
            g_config_var.engine_state = (uint8) (((uint32) argc >> 16) & 0xff);
        }
        else if ((((uint32) argc >> 8) & 0xff) == APP_ID_MUSIC)
        {
            g_config_var.ap_id = APP_ID_MUSIC;
            g_config_var.engine_state = (uint8) (((uint32) argc >> 16) & 0xff);
        }
        else if((((uint32) argc >> 8) & 0xff) == APP_ID_VIDEO)
        {
            g_config_var.ap_id = APP_ID_VIDEO;
            g_config_var.engine_state = (uint8) (((uint32) argc >> 16) & 0xff);
        }
        else
        {
            g_config_var.ap_id = APP_ID_MAINMENU;
        }
        //���½��ػ�ʱ�����0
        g_setting_comval.g_comval.sleep_timer = 0;
        sys_vm_write(&g_setting_comval, VM_AP_SETTING);
        sys_vm_write(&g_config_var, VM_AP_CONFIG);

        switch_off_here: //����ػ�����
        //����ػ�ʱusb�����ϣ���������ʾ����
        if ((act_readb(SYSTEM_VOL) & 0x80) != 0)
        {
            //if ((uint32)key_chargeget(0) == CHARGE_CHARGING)
            {
                _config_charging();
            }
        }
        else
        {
            switch_off();
        }
    }

    if (first_boot == TRUE)
    {
        _config_format_disk();
    }
    //����˵���¼
    com_clear_all_history(PATH_HISTORY_MENU);

    //ִ��applib���ע������
    applib_quit();

    return result;
}
