/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-20 9:42     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _sys_counter.c
 * \brief    common ϵͳ��ʱ�����ܽӿ�ʵ��
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ��һЩӦ�ò��ʱ��ع������ܽӿڡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2011-9-20
*******************************************************************************/

#include "common_ui.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ϵͳ��ʱ��ʱ���������̣�������Ļ���⣬������ʡ��ػ���˯�߹ػ����ص�����
 *    ���ŵȡ�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  ϵͳ��ʱ���ܣ��������⣬������ʡ��ػ���˯�߹ػ���app���ߣ���ǰ̨AP���м�ʱ��
 *      ����ǰ̨AP����һ����ʱ����ר��������������ܼ�ʱ���ļ�ʱ���� get_app_msg ��
 *      ���� handle_timers ȥ����⣬һ����ʱ�ѵ���������Ӧ��ʱ���ܡ�
 * \li  ��Ļ������ȫ�� common ģ����ƣ������û����ģ��û�����ͨ�� get_backlight_state 
 *      ����ȡ����״̬��
 * \li  ��Ļ����ͬ����ȫ�� common ģ����ƣ������û����ġ�����Ҫ������Ļ����ʱ���ȷ���
 *      �첽��Ϣ MSG_ENTER_SCREEN_SAVE ����ǰAP���� com_message_box �н���������
 * \li  ʡ��ػ���˯�ߣ���ʱ���ػ����ǹ㲥 MSG_POWER_OFF ��Ϣ��manager ap ���յ�����Ϣ
 *      �󣬷��� MSG_APP_QUIT ��ǰ̨�ͺ�̨AP�Լ��˳���
 * \li  �������ڲ��Ž���ͨ����"�������ڲ��ŵ�ģʽ"������Ӧ��ǰ̨AP���߷����첽��Ϣ back_to_playing
 *      ��ǰ̨AP�Լ��ص����ڲ��ų������������ڲ��Ž�����ҪAP�����ڲ��ų���Э�������ڽ��볡��ʱ 
 *      change_app_state(APP_STATE_PLAYING)�����˳�����ʱ change_app_state(APP_STATE_NO_PLAY)��
*******************************************************************************/
void sys_counter_handle(void)
{
    msg_apps_t msg;
    uint8 cur_light_timer = sys_comval->light_timer;
    uint8 cur_save_timer = sys_comval->screen_saver_timer;
    uint8 cur_poff_timer = sys_comval->poweroff_timer;
    uint8 cur_sleep_timer = sys_comval->sleep_timer;
    uint8 cur_playing_timer = sys_comval->playing_timer;
    uint8 cur_ss_mode = sys_comval->screen_saver_mode;
    uint8 cur_dark_lightoff_timer = sys_comval->dark_lightoff_timer;
    uint8 cur_screen_lightoff_timer = sys_comval->screen_lightoff_timer;
    
    /* �����������ȼ�ʱ */
    //����ʱ�俪��ֻ��ͨ���˵�ѡ�񣬼��а����������� com_gui_msg_hook �аѼ�ʱ����0��
    //�������������ظ�������Ҳ�����
    //if((cur_light_timer != 0)
    //&& (cur_light_timer != g_app_last_state.last_light_timer) )
    //{
    //    g_sys_counter.light_counter = 0;
    //}

    /* ����䰵��ʱ�� */
    if((cur_light_timer != 0)
    && (g_app_info_state.backlight_state == BACKLIGHT_STATE_NORMAL)//���⴦������״̬��
    && (g_app_info_state.app_state != APP_STATE_PLAYING_ALWAYS))//�����У������賣������
    {
        g_sys_counter.light_counter++;
        if(g_sys_counter.light_counter >= cur_light_timer)
        {
            //������������޸ı���״̬
            com_set_contrast(0);//�����
            g_app_info_state.backlight_state = BACKLIGHT_STATE_DARK;
        }
    }
    
    /* ������Ļ������ʱ */
    //if((cur_ss_mode != 0)
    //&& (cur_ss_mode != g_app_last_state.last_screen_save_mode) )
    //{
    //    g_sys_counter.screen_saver_counter = 0;
    //}
    
    /* ��Ļ������ʱ�� */
    if((cur_save_timer != 0) 
    && (cur_ss_mode != 0)
    && (g_app_info_state.screensave_state == FALSE)//û�д�������״̬��
    && (g_app_info_state.app_state != APP_STATE_PLAYING_ALWAYS))//�����У������賣������
    {
        g_sys_counter.screen_saver_counter++;
        if(g_sys_counter.screen_saver_counter >= cur_save_timer)
        {
            //����ǰAP���� enter_screen_save �첽��Ϣ���� com_message_box �н�������
            msg.type = MSG_APP_ENTER_SCREEN_SAVE;
            send_async_msg(g_this_app_info->app_id, &msg);
        }
    }
    
    /* ����ص���ʱ����ʵ��Ϊ����ص����������ģʽ */
    if((cur_dark_lightoff_timer != 0)//��������䰵��һ��ʱ���Զ��ص����⹦��
    && (g_app_info_state.backlight_state == BACKLIGHT_STATE_DARK)//����䰵״̬
    && (cur_ss_mode == SCREEN_NULL))//û����������ģʽ
    {
        g_sys_counter.light_counter++;
        if(g_sys_counter.light_counter >= (cur_light_timer + cur_dark_lightoff_timer))
        {
            //����ǰAP���� enter_screen_save �첽��Ϣ���� com_message_box �н�������
            msg.type = MSG_APP_ENTER_SCREEN_SAVE2;
            send_async_msg(g_this_app_info->app_id, &msg);
        }
    }
    if((cur_screen_lightoff_timer != 0)//����������һ��ʱ���Զ��ص����⹦��
    && (g_app_info_state.screensave_state == TRUE)//��������״̬
    && (g_ss_mode != SCRREN_SCREEN_OFF)//��ǰ����ģʽ���ǹص�����
    && (g_app_info_state.backlight_state != BACKLIGHT_STATE_OFF))//��δ�ص�����
    {
        g_sys_counter.screen_saver_counter++;
        if(g_sys_counter.screen_saver_counter >= (cur_save_timer + cur_screen_lightoff_timer*120))
        {
            //����ǰAP���� enter_screen_save �첽��Ϣ���� com_message_box �н�������
            msg.type = MSG_APP_ENTER_SCREEN_SAVE2;
            send_async_msg(g_this_app_info->app_id, &msg);
        }
    }
    
    /* ����ʡ��ػ���ʱ */
    if((  (g_app_last_state.last_state == ENGINE_STATE_PLAYING)
        ||(g_app_last_state.last_app_state == APP_STATE_PLAYING))
    && (  (g_app_info_state.state != ENGINE_STATE_PLAYING)
        &&(g_app_info_state.app_state == APP_STATE_NO_PLAY)) )
    {
        g_sys_counter.poweroff_counter = 0;
    }
    
    /* ʡ��ػ���ʱ�� */
    if((cur_poff_timer != 0)
    && (g_app_info_state.cable_state == CABLE_STATE_CABLE_NULL)//USB����û�в���
    && (g_app_info_state.state != ENGINE_STATE_PLAYING)//��̨���治���ڲ���״̬
    && (g_app_info_state.app_state == APP_STATE_NO_PLAY))//ǰ̨Ӧ�ò����ڲ��� UI ״̬
    {
        g_sys_counter.poweroff_counter++;
        if(g_sys_counter.poweroff_counter >= cur_poff_timer)
        {
            //�㲥�ػ���Ϣ
            msg.type = MSG_POWER_OFF;
            broadcast_msg(&msg);
        }
    }
    
    /* ����˯�߹ػ���ʱ */
    if((cur_sleep_timer != 0)
    && (cur_sleep_timer != g_app_last_state.last_sleep_timer))
    {
        g_sys_counter.sleep_counter = 0;
    }
    
    /* ˯�߹ػ���ʱ�� */
    if((cur_sleep_timer != 0) 
    && (g_app_info_state.cable_state == CABLE_STATE_CABLE_NULL))//USB����û�в���
    {
        g_sys_counter.sleep_counter++;
        if(g_sys_counter.sleep_counter >= (cur_sleep_timer * 120))
        {
            //�㲥�ػ���Ϣ
            msg.type = MSG_POWER_OFF;
            broadcast_msg(&msg);
        }
    }
    
    /* �����������ڲ��ż�ʱ */
    if((  (g_app_last_state.last_state == ENGINE_STATE_PLAYING)
        ||(g_app_last_state.last_app_state == APP_STATE_PLAYING))
    && (  (g_app_info_state.state != ENGINE_STATE_PLAYING)
        &&(g_app_info_state.app_state == APP_STATE_NO_PLAY)) )
    {
        g_sys_counter.playing_counter = 0;
    }
    
    /* �������ڲ��ż�ʱ�� */
    if((cur_playing_timer != 0) 
    && (g_app_info_state.screensave_state == FALSE)//ϵͳ��������״̬��
    && (g_app_info_state.state == ENGINE_STATE_PLAYING)//��̨���治���ڲ���״̬
    && (g_app_info_state.app_state == APP_STATE_NO_PLAY))//ǰ̨Ӧ�ò����ڲ��� UI ״̬
    {
        g_sys_counter.playing_counter++;
        if(g_sys_counter.playing_counter >= cur_playing_timer)
        {
            //����ǰAP���� back_to_playing �첽��Ϣ
            msg.type = MSG_APP_BACKTO_PLAYING;
            send_async_msg(g_this_app_info->app_id, &msg);
        }
    }
    
    /* ��ȡ��ص����������͵磻����Ϊ 10 �� */
    g_sys_counter.battery_counter++;
    if(g_sys_counter.battery_counter >= 20)
    {
        uint8 tmp_battery_value;    //��ǰ��ص���
        uint8 tmp_charge_state;    //��ǰ���״̬�����޳�磬���ڳ�磬����������޵��
        bool bat_now = FALSE;
        msg.type = MSG_NULL;
            
        if(g_sys_counter.battery_counter >= 100)//���������״̬��������־
        {
            //��������7��
            g_bat_index = BAT_SAMPLE_TIME;
            bat_now = TRUE;
        }
        if(g_bat_index >= BAT_SAMPLE_TIME)
        {
            g_sys_counter.battery_counter = 0;
        	
            if(g_app_info_state.cable_state == CABLE_STATE_CABLE_NULL)//USB����û�в���
            {
                tmp_charge_state = CHARGE_NONE;
            }
            else
            {
                tmp_charge_state = (uint8)(int)key_chargeget(0);
            }
            tmp_battery_value = com_get_battery_grade(bat_now);
            
            if((g_app_info_state.charge_state != CHARGE_FULL) 
            && (tmp_charge_state == CHARGE_FULL))//��һ�μ�⵽�����������ʾ
            {
                //������������㲥���������Ϣ
                msg.type = MSG_FULL_CHARGE;
            }
            else if((tmp_charge_state == CHARGE_NONE) 
            && (tmp_battery_value == 0))//��ع��磬�Ҽ�⵽�͵�
            {
                if(g_app_info_state.cable_state == CABLE_STATE_CABLE_NULL)//USB����û�в���
                {
                    //��⵽�͵磬�㲥�͵���Ϣ
                    msg.type = MSG_LOW_POWER;
                }
            }
            else
            {
                //do nothing for QAC
            }
            
            g_app_info_state.charge_state = tmp_charge_state;
            g_app_info_state.bat_value = tmp_battery_value;
            if(msg.type != MSG_NULL)//�ȸ�ȫ�ֵ��״̬�������ٷ�����Ϣ
            {
                broadcast_msg_sys(&msg);//�㲥�������͵���Ϣ�������͸� manager ap
            }
        }
        else
        {
            g_bat_sample[g_bat_index] = act_readb(BATADC_DATA);//0-6bit��Ч
            g_bat_index++;
        }
    }
    
    /* �������״̬ */
    g_app_last_state.last_state = g_app_info_state.state;
    g_app_last_state.last_app_state = g_app_info_state.app_state;
    //g_app_last_state.last_light_timer = cur_light_timer;
    //g_app_last_state.last_screen_save_mode = cur_ss_mode;
    g_app_last_state.last_sleep_timer = cur_sleep_timer;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ϵͳ״̬������ʱ���������̣���������״̬�����µȡ�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
*******************************************************************************/
void sys_status_handle(void)
{
    /* ����״̬������ */
    g_battery_display++;//��ض�̬���������������Ϊ���������ˢ�� gui_headbar ��ص�����ʾ����
    if(g_headbar_update == HEADBAR_UPDATE_INIT)
    {
        gui_headbar(HEADBAR_UPDATE_INIT);
        g_headbar_update = HEADBAR_UPDATE_CHANGE;
    }
    else
    {
        gui_headbar_handle();
    }
}

/*! \endcond */

/*! \cond */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ʾģʽ��Ļ������ʱ���������̣����������л���ʾͼƬ
 * \param[in]    none
 * \param[out]   none    
 * \return       none
 * \note 
*******************************************************************************/
void demo_mode_handle(void)
{
    picbox_private_t ss_pic;
    style_infor_t style_infor;
    
    ss_pic.pic_id = V_U16_INVALID;
    ss_pic.frame_id = g_ss_demo_mode_phase;
    style_infor.style_id = DEMO_MODE_PIC;
    style_infor.type = UI_COM;
    ui_show_picbox(&style_infor, &ss_pic);
    g_ss_demo_mode_phase++;
    if(g_ss_demo_mode_phase >= g_ss_demo_frame_cnt)
    {
        g_ss_demo_mode_phase = 0;
    }
}

/*! \endcond */
