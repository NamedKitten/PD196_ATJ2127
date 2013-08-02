/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-20 9:44     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _message_box.c
 * \brief    common Ĭ��Ӧ����Ϣ����
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ�ֶ�Ĭ��Ӧ����Ϣ��ͳһ��������ͳһ�������û�ʹ�á�
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2011-9-20
*******************************************************************************/

#include "common_ui.h"

extern void deal_antenna_detect(bool in_out) __FAR__;

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    gui ����¼� �� ϵͳ��Ϣ������
 * \param[in]    msg_type ���������Ϣ���¼�������
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_NULL ���ؿս����AP�ɲ������κδ���
 * \retval           RESULT_REDRAW �����ػ�UI��APһ����Ҫ���� redraw ����UI
 * \retval           other ֱ�ӷ�����Ϣ���¼�������Ľ��������μ� app_result_e ����
 * \ingroup      misc_func
 * \par          exmaple code
 * \code
    ����1���ڰ�����Ϣ���¼���ѭ���е��ã������ݰ����¼�
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    bool ret;
    
    ret = get_gui_msg(&input_msg);
    if(ret == TRUE)
    {
        if(com_key_mapping(&input_msg, &gui_event, key_map_list) == TRUE)
        {
            switch(gui_event)
            {
            case EVENT_DO_SOMETHING:
                break;
               
            default:
                result = com_message_box(gui_event);
                break;
            }
        }
    }
    else
    {
        gui ��Ϣ���¼���������ϣ����������Դ���Ӧ��˽����Ϣ��
    }
    
    ����2����Ӧ��˽����Ϣ�����У�����Ĭ��ϵͳ��Ϣ
    private_msg_t private_msg;
    msg_apps_type_e msg_type
    app_result_e result;
    
    if(get_app_msg(&private_msg) == TRUE)
    {
        msg_type = private_msg.msg.type;
        switch(msg_type)
        {
        case XXX://����Ӧ��˽����Ϣ������һЩӦ����Ҫ���⴦���ϵͳ��Ϣ
            break;
            
        default:
            result = com_message_box(msg_type);
            break;
        }
    }
 * \endcode
 * \note 
 * \li  com_message_box ����Ĭ�ϵķ�ʽ�� gui ����¼���ϵͳ��Ϣ ���д���
 * \li  ����û�Ҫ��ĳ�� gui ����¼���ϵͳ��Ϣ�������⴦����ôӦ���� gui �¼��ַ�
 *      ��Ӧ��˽����Ϣ�ַ��� case ��֧�����ȴ������
*******************************************************************************/
app_result_e com_message_box(msg_apps_type_e msg_type)
{
    app_result_e result = RESULT_NULL;
    
    switch(msg_type)
    {
    /************************** ��ݼ����� ******************************/
    /* ���� RETURN ������ mainmenu Ӧ���¼� */
    case EVENT_RETURN_MAINMENU:
        {
            //�ڷ� mainmenu AP��Ӧ
            if(g_this_app_info->app_id != APP_ID_MAINMENU)
            {
                //�˳���ǰAP���ص� MainMenu AP
                result = RESULT_MAIN_APP;
            }
        }
        break;
        
    /* ���� VOL+ �� VOL- �����������ڽ����¼� */
    case EVENT_ENTER_VOLUMEBAR:
        {
            result = gui_volumebar(&sys_comval->volume_current, &sys_comval->volume_limit, SET_VOLUME_VALUE);
        }
        break;
        
    /* ���� LOCK �������أ����밴���������¼� */
    case EVENT_ENTER_KEYLOCK:
        {
            result = gui_keylock(TRUE);
        }
        break;
    
    /* ���� LOCK �������أ����밴���������¼� */
    case EVENT_ENTER_KEYLOCK_UN:
        {
            result = gui_keylock(TRUE);
        }
        break;
    
    /*! ������ס״̬�£������ⰴ�����밴���������¼� */ 
    case EVENT_ENTER_KEYLOCK_HOLD:
        {
            result = gui_keylock(FALSE);
        }
        break;
        
    /* ���� PLAY ������ػ�����ȷ���¼� */
    case EVENT_ENTER_SHUTOFF_ASK:
        {
            result = gui_shut_off();
        }
        break;
    
    /************************** ϵͳ��Ϣ���� ******************************/
    /* ������Ļ�����¼� */
    case MSG_APP_ENTER_SCREEN_SAVE:
        {
            gui_screen_save_enter(sys_comval->screen_saver_mode);
        }
        break;
    
    /* ������Ļ�����¼����ص����� */
    case MSG_APP_ENTER_SCREEN_SAVE2:
        {
            if(get_screensave_state() == TRUE)
            {
                //�ص�����ƣ��ص�����ǰ��������Ч������
                ui_set_backgd_color(0x0000);//��Ϊȫ����
                ui_clear_screen(NULL);
                lcd_backlight_on_off(FALSE);
                lcd_standby_screen(FALSE);
                change_backlight_state(BACKLIGHT_STATE_OFF);
                
                //����Ƶ��Ϊ 3M
                g_ss_clk_backup = (uint8)sys_adjust_clk(FREQ_3M, 0);
            }
            else
            {
                gui_screen_save_enter(SCRREN_SCREEN_OFF);
            }
        }
        break;
    
    /* ������Ļ�����¼� */
    case MSG_APP_EXIT_SCREEN_SAVE:
        {
            result = gui_screen_save_exit();
            com_filter_key_hold();//���˺�������
        }
        break;
    
    /* �ص����ڲ��Ž��� */
    case MSG_APP_BACKTO_PLAYING:
        {
            msg_apps_t msg;
            
            if(g_ss_delay_flag == TRUE)//delay�˳�����
            {
                gui_screen_save_exit();
                g_ss_delay_flag = FALSE;
            }
            
            if (get_engine_type() == ENGINE_MUSIC)
            {
                if(g_this_app_info->app_id == APP_ID_MUSIC)
                {
                    break;//�쳣���� music Ӧ����Ӧ�ô������Ϣ
                }
                msg.content.data[0] = APP_ID_MUSIC;
            }
            else
            {
                if(g_this_app_info->app_id == APP_ID_RADIO)
                {
                    break;//�쳣���� radio Ӧ����Ӧ�ô������Ϣ
                }
                msg.content.data[0] = APP_ID_RADIO;
            }
            msg.content.data[1] = PARAM_BACKTO_PLAYING;
            msg.type = MSG_CREAT_APP;
            send_async_msg(APP_ID_MANAGER, &msg);
            
            result = RESULT_APP_QUIT;
        }
        break;
    
    /* ϵͳ�͵���Ϣ */
    case MSG_LOW_POWER:
        {
            private_msg_t private_msg;
            msg_apps_t msg;
            style_infor_t style_infor;

            //��ʾ�͵�ͼ��
            style_infor.style_id = LOW_POWER_BG;
            style_infor.type = UI_COM;
            ui_show_picbox(&style_infor, NULL);
            
            //��ʾ�͵��ַ���
            style_infor.style_id = LOW_POWER_TEXT;
            style_infor.type = UI_COM;
            ui_show_textbox(&style_infor, NULL, TEXTBOX_DRAW_NORMAL);
            
            //�ȴ�2����
            com_app_sleep(4, NULL);
            
            //�㲥�ػ���Ϣ
            msg.type = MSG_POWER_OFF;
            broadcast_msg(&msg);
            
            //�ȴ�AP�˳���Ϣ MSG_APP_QUIT ���ٷ��� RESULT_APP_QUIT��Ӧ��ֱ���˳�
            while(1)
            {
                //����10ms�����������
                sys_os_time_dly(1); 
                if((get_app_msg(&private_msg) == TRUE) && (private_msg.msg.type == MSG_APP_QUIT))
                {
                    result = RESULT_APP_QUIT;
                    break;
                }
            }
        }
        break;
    
    /* ���������Ϣ */
    case MSG_FULL_CHARGE:
        {
            style_infor_t style_infor;

            //��ʾ�͵�ͼ��
            style_infor.style_id = FULL_CHARGE_BG;
            style_infor.type = UI_COM;
            ui_show_picbox(&style_infor, NULL);
            
            //��ʾ�͵��ַ���
            style_infor.style_id = FULL_CHARGE_TEXT;
            style_infor.type = UI_COM;
            ui_show_textbox(&style_infor, NULL, TEXTBOX_DRAW_NORMAL);
            
            //�ȴ�2����
            com_app_sleep(4, NULL);
            result = RESULT_REDRAW;
        }
        break;

    /* ��������Ϣ */
    case MSG_SD_IN:
        {
            //����������ʹ��ʱ�ټ����������
        }
        break;
        
    /* ���γ���Ϣ*/
    case MSG_SD_OUT:
        {
            //�������ʹ�ÿ�����ʾ���γ�
        }
        break;
    
    /* U�̲���*/
    case MSG_UH_IN:
        {
            //����������ʹ��ʱ�ټ����������
        }
        break;
        
    /* U�̰γ�*/
    case MSG_UH_OUT:
        {
            //�������ʹ��U �̣���ʾU �̰γ�
        }
        break;
        
    /* USB B�߲�����Ϣ */
    case MSG_USB_STICK:
        {       
            //��ʾ USB ѡ��Ի���
            result = gui_usbconnect();
            
            g_sys_counter.battery_counter = 100;//ǿ�ƶ�ȡ���״̬�͵�ص���
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
        
    /* ����� ADAPTOR ������Ϣ */
    case MSG_ADAPTOR_IN:
        {
            //������磬����������UI
            key_chargeset(CHARGE_START, ChargeCurrent250mA, BATFULL_LEVEL1);
            
            g_sys_counter.battery_counter = 100;//ǿ�ƶ�ȡ���״̬�͵�ص���
        }
        break;
        
    /* ����� ADAPTOR �γ���Ϣ */
    case MSG_ADAPTOR_OUT:
        {
            g_sys_counter.battery_counter = 100;//ǿ�ƶ�ȡ���״̬�͵�ص���
        }
        break;
            
    /* RTC��ʱ������Ϣ */
    case MSG_RTCALARM:
        {
            //��tools ����
            msg_apps_t msg;
    
            result = com_alarm_msg_dispatch();
            if(result == RESULT_ALARMFM)
            {
                if(g_this_app_info->app_id == APP_ID_RADIO)
                {
                    break;
                }
                msg.content.data[0] = APP_ID_RADIO;
            }
            else if(result == RESULT_FMRECORD)
            {
                if(g_this_app_info->app_id == APP_ID_RECORD)
                {
                    break;
                }
                msg.content.data[0] = APP_ID_RECORD;
            }
            else if(result == RESULT_ALARM)
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
        
    /* ���������ߣ�������Ϣ */
    case MSG_EARPHONE_IN:
        {
            deal_antenna_detect(TRUE);
        }
        break;
        
    /* ���������ߣ��γ���Ϣ */
    case MSG_EARPHONE_OUT:
        {
            deal_antenna_detect(FALSE);
            if(g_this_app_info->app_id == APP_ID_RADIO)//ֻ�� RADIO ������ʾ
            {
                gui_dialog_msg (DIALOG_MSG, DIALOG_INFOR_WAIT, S_EARPHONE_OUT);
                result = RESULT_REDRAW;
            }
        }
        break;
        
    /************************** ����Ӧ��˽����Ϣ���� ******************************/
    case MSG_APP_QUIT:
        {
            result = RESULT_APP_QUIT;
        }
        break;

    default:
        break;
    }
    
    return result;
}

/*! \endcond */
