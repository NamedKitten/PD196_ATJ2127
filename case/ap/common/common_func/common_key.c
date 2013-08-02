/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-20 9:42     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_key.c
 * \brief    ������������������g-sensor�������������⴦��ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ����������Ϣ���ˣ����⴦�������������������ȵȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               applib
 * \version  1.0
 * \date     2011-9-20
*******************************************************************************/

#include "common_func.h"
#include "applib.h"

extern comval_t *sys_comval;
extern void com_set_contrast(uint8 lightness) __FAR__;

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    �����������⴦����Ϊ gui ��Ϣ����Ļص�������
 * \param[in]    input_msg ��ȡ���� gui��Ϣָ��
 * \param[out]   input_msg ���ܻ��input_msg�����޸ģ����罫val�޸�ΪKEY_HOLD���ⰴ��
 * \return       bool
 * \retval           TRUE ��ȡ���İ�����Ч��û�б����˵���
 * \retval           FALSE ��ȡ���İ�����Ч�������˵��ˡ�
 * \ingroup      common_func
 * \note         
*******************************************************************************/
bool com_gui_msg_hook(input_gui_msg_t *input_msg)
{
    bool ret;
    
    //0.������������˵���������
    if(get_filter_key() != KEY_NULL)
    {
        //ֱ����⵽����̧�𣨻��������������£�����������
        if(input_msg->data.kmsg.val == get_filter_key())
        {
            //����LONG��HOLD����
            if(input_msg->data.kmsg.type != KEY_TYPE_SHORT_UP)
            {
                return FALSE;
            }
            else//������ɣ������ǲ���ӦSHORT_UP����
            {
                change_filter_key(KEY_NULL);
                return FALSE;
            }
        }
        else//����������������ɣ���������
        {
            change_filter_key(KEY_NULL);
        }
    }
    
    //���µ�ǰ����ֵ
    this_filter_key_value = input_msg->data.kmsg.val;
    
    //1.1�����⴦�ڵ���״̬���ָ��������ȣ����޸ı���״̬
    if(get_backlight_state() == BACKLIGHT_STATE_DARK)
    {
        com_set_contrast(sys_comval->lightness);
        change_backlight_state(BACKLIGHT_STATE_NORMAL);
    }
    
    //1.2��������Ļ����ģʽ���˳���Ļ����ģʽ
    if(get_screensave_state() == TRUE)
    {
        msg_apps_t msg;
        
        //����ǰAP���� exit_screen_save �첽��Ϣ���� com_message_box ���˳�����
        msg.type = MSG_APP_EXIT_SCREEN_SAVE;
        send_async_msg(g_this_app_info->app_id, &msg);
        
        ret = FALSE;
    }
    //1.3����״̬�£�����������������
    else
    {
        //2.�������������ڰ�������ʱ����������
        if(input_msg->data.kmsg.type == KEY_TYPE_DOWN)
        {
            soundout_state_e state = (soundout_state_e)get_soundout_state();
            if(((state == SOUND_OUT_STOP) || (state == SOUND_OUT_PAUSE))
            && (get_keytone_enable() == TRUE))
            {
                //û��Ƶ����Ұ�����ʹ�ܣ�����������
                start_key_tone(0);
            }
        }
        
        //3.����������ֻ��������ס״̬�°�����ͨ����
        //���⣺config, playlist, udisk, ��Ӧ�ò���ⰴ����
        if((g_this_app_info->app_id != APP_ID_CONFIG)
        && (g_this_app_info->app_id != APP_ID_PLAYLIST)
        && (g_this_app_info->app_id != APP_ID_UDISK))
        {
            if( (get_keylock_state() == TRUE )
              &&(input_msg->data.kmsg.val != KEY_UNLOCK) )
            {
                //������ס״̬�£�������ͨ������ת��Ϊ�ð������Թ�Ӧ�ò�ѡ���Դ���
                input_msg->data.kmsg.val = KEY_LOCKHOLD;
                //���˵���ǰ����������������ֻ֤��Ӧһ�Ρ�������ס����ʾ
                com_filter_key_hold();
            }
        }
        
        ret = TRUE;
    }
    
    //4.����رռ�ʱ����Ļ������ʱ��ʡ��ػ���ʱ���������ڲ��ż�ʱ��0
    g_sys_counter.light_counter = 0;
    g_sys_counter.screen_saver_counter = 0;
    g_sys_counter.poweroff_counter = 0;
    g_sys_counter.playing_counter = 0;
    
    return ret;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���˵���ǰ������������������ֵͬ������Ϣ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 * \li  �ýӿ����ڹ��˵�ǰ����ֵ�ĺ���������������˵�����ָ����������ֱ�ӵ���
 *      change_filter_key ���á�
 * \li  �ڼ�⵽��ǰ����̧����Ϣ��������������Ϣ���� com_gui_msg_hook �Զ����ý����
 * \li  �����ʱû�а������£��ýӿڵ��ò�����������İ�������Ч����
*******************************************************************************/
void com_filter_key_hold(void)
{
    change_filter_key(this_filter_key_value);
}

/*! \endcond */
