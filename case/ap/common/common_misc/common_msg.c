/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-5-11 15:07     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _miscellaneous.c
 * \brief    common Ӧ��˽����Ϣ������
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               Ӧ��˽����Ϣ������
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-5-11
*******************************************************************************/

#include "common_ui.h"

#define  PLUGINSTATUS    (1<<6)

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ǰ̨Ӧ��˽����Ϣ��������
 * \param[in]    private_msg ��ȡ����˽����Ϣָ��
 * \param[out]   none 
 * \return       none
 * \ingroup      misc_func
 * \note 
*******************************************************************************/
void com_app_msg_hook(private_msg_t *private_msg)
{
    msg_apps_type_e msg_type = private_msg->msg.type;
    
    switch(msg_type)
    {
    case MSG_SD_IN:
        if(get_card_state() == CARD_STATE_CARD_NULL)//ֻ����CARD_NULL״̬�²���Ҫ����
        {
            change_card_state(CARD_STATE_CARD_IN);
        }
        change_card_plist(FALSE);
        change_card_update(FALSE);
        g_ss_vip_event = TRUE;
        break;
    case MSG_SD_OUT:
        change_card_state(CARD_STATE_CARD_NULL);
        change_card_plist(FALSE);
        change_card_update(FALSE);
        g_ss_vip_event = TRUE;
        break;
        
    case MSG_UH_IN:
    case MSG_UH_OUT:
        g_ss_vip_event = TRUE;
        break;
        
    case MSG_USB_STICK:
        /* wait 50ms for USB-connect stably. */
        sys_os_time_dly(20);
        
        if((act_readb(DPDMCTRL)&PLUGINSTATUS) == 0)//ADAPTOR�����߲���
        {
            private_msg->msg.type = MSG_ADAPTOR_IN;//��ΪMSG_ADAPTOR_IN��Ϣ
            change_cable_state(CABLE_STATE_ADAPTOR_IN);
        }
        else
        {
            change_cable_state(CABLE_STATE_CABLE_IN);
        }
    
        g_ss_vip_event = TRUE;
        break;
    case MSG_USB_UNSTICK:
        if(get_cable_state() == CABLE_STATE_ADAPTOR_IN)//ADAPTOR�����߰γ�
        {
            private_msg->msg.type = MSG_ADAPTOR_OUT;//��ΪMSG_ADAPTOR_OUT��Ϣ
        }
        change_cable_state(CABLE_STATE_CABLE_NULL);
        g_ss_vip_event = TRUE;
        break;
        
    case MSG_POWER_OFF:
    case MSG_LOW_POWER:
    case MSG_RTCALARM:
        g_ss_vip_event = TRUE;
        break;
    
    default:
        break;
    }
    
    if(g_ss_vip_event == TRUE)
    {
        //�����⴦�ڵ���״̬���ָ��������ȣ����޸ı���״̬
        if(get_backlight_state() == BACKLIGHT_STATE_DARK)
        {
            com_set_contrast(sys_comval->lightness);
            change_backlight_state(BACKLIGHT_STATE_NORMAL);
        }
        
        //��������Ļ����ģʽ���˳���Ļ����ģʽ
        if(get_screensave_state() == TRUE)
        {
            msg_apps_t msg;
            
            //����Ӧ�˳���Ļ����ģʽ������Ӧ��ǰ��Ϣ����͵������Ϊ�˳���Ļ����ģʽ��Ϣ��Ȼ���ط�һ�ε�ǰ��Ϣ
            //1.���� exit_screen_save �첽��Ϣ���� com_message_box ���˳�����
            private_msg->msg.type = MSG_APP_EXIT_SCREEN_SAVE;
            
            //2.�ط�һ�ε�ǰ��Ϣ
            msg.type = msg_type;
            send_async_msg(g_this_app_info->app_id, &msg);
        }
        else
        {
            g_ss_vip_event = FALSE;
        }
        
        //����رռ�ʱ����Ļ������ʱ��ʡ��ػ���ʱ���������ڲ��ż�ʱ ��0
        g_sys_counter.light_counter = 0;
        g_sys_counter.screen_saver_counter = 0;
        g_sys_counter.poweroff_counter = 0;
        g_sys_counter.playing_counter = 0;
    }
}

/*! \endcond */
