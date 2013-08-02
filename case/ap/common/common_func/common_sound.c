/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-5-18 11:44     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_sound.c
 * \brief    �����������ӿ�ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ���˶԰���������Ӧ���������Э������
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-5-18
*******************************************************************************/

#include "common_func.h"

#define KEY_TONE_VOLUME     12//�������̶�Ϊ PA 12

//#define AUDIO_DEVICE_PA_DDV

/*! \cond COMMON_API */

/******************************************************************************/
/*!                     
 * \par  Description:
 *    ���ð�����ʹ�����
 * \param[in]    kt_enable �������Ƿ�ʹ�ܣ�0��ʾ��ֹ��1��ʾʹ��
 * \param[out]   none    
 * \return       bool
 * \retval           TRUE ���ð������ɹ�
 * \retval           FALSE ���ð�����ʧ�� 
 * \note 
*******************************************************************************/
bool com_set_sound_keytone(uint8 kt_enable)
{
    soundout_state_e cur_state;
    bool last_sound_flag, sound_flag;
    bool dac_flag;
    
    //��ȡ����ǰ����״̬
    cur_state = (soundout_state_e)get_soundout_state();
    if((get_keytone_enable() == TRUE) || (cur_state != SOUND_OUT_STOP))
    {
        last_sound_flag = TRUE;
    }
    else
    {
        last_sound_flag = FALSE;
    }
    
    //��ȡ��Ƶ���dac״̬
    if((cur_state == SOUND_OUT_START) || (cur_state == SOUND_OUT_RESUME))
    {
        dac_flag = TRUE;
    }
    else
    {
        dac_flag = FALSE;
    }
    
    if(kt_enable != 0)
    {
        //enable key tone
        g_app_info_state.sound_state |= (uint8)SOUND_BIT_KT_ENABLE;
    }
    else
    {
        //disable key tone
        g_app_info_state.sound_state &= (uint8)SOUND_BIT_KT_DISABLE;
    }
    
    //��ȡ���ú�����״̬
    if((get_keytone_enable() == TRUE) || ((soundout_state_e)get_soundout_state() != SOUND_OUT_STOP))
    {
        sound_flag = TRUE;
    }
    else
    {
        sound_flag = FALSE;
    }
    
    //�ر�dac
    if((dac_flag == FALSE) && (kt_enable == 0))
    {
        disable_dac();
        key_speakcheck(0);//��ֹspeaker���
    }
    
    //����״̬�ӹص�������pa
    if((last_sound_flag == FALSE) && (sound_flag == TRUE))
    {
        //��pa
#ifdef AUDIO_DEVICE_PA_DDV
        enable_pa_ddv(0, 0);
#else
        enable_pa(0, 0);
#endif
    }
    //����״̬�ӿ����أ��ر�pa
    else if((last_sound_flag == TRUE) && (sound_flag == FALSE))
    {
        //�ر�pa
#ifdef AUDIO_DEVICE_PA_DDV
        disable_pa_ddv(0, 0);
#else
        disable_pa();
#endif
    }
    else
    {
        //do nothing for QAC
    }
    
    //��dac�������ð�����pa
    if((dac_flag == FALSE) && (kt_enable != 0))
    {
        key_speakcheck(1);//ʹ��speaker���
        enable_dac(2, 44);//û����Ƶ�����dac�ǹص��ģ���Ҫ����dac
        set_pa_volume(KEY_TONE_VOLUME);
        
        //���ð���������
        set_key_tone_vol(24);
        set_key_tone_freq(2);
        set_key_tone_period(2);
    }
    
    return TRUE;
}

/******************************************************************************/
/*!                     
 * \par  Description:
 *    ��������������
 * \param[in]    volume ��������ֵ
 * \param[out]   none    
 * \return       bool
 * \retval           TRUE ���������ɹ�
 * \retval           FALSE ��������ʧ�� 
 * \note 
*******************************************************************************/
bool com_set_sound_volume(uint8 volume)
{
    bool bret = TRUE;
    
    //���浱ǰ����ֵ
    set_current_volume(volume);
    
    //ֻ�����ڲ������ֻ�����ʱ����Ҫ��������
    if(get_engine_state() == ENGINE_STATE_PLAYING)
    {
        msg_apps_t msg;
        engine_type_e type;
    
        type = get_engine_type();
        if(type == ENGINE_MUSIC)
        {
            //��������
            msg.content.addr = &volume; 
            //��Ϣ����(����Ϣ����)
            msg.type = MSG_MENGINE_SET_VOLUME_SYNC; 
            //����ͬ����Ϣ
            bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
        }
        else if(type == ENGINE_RADIO)
        {
            //��������
            msg.content.addr = &volume; 
            //��Ϣ����(����Ϣ����)
            msg.type = MSG_FMENGINE_SET_VOLUME; 
            //����ͬ����Ϣ
            bret = send_sync_msg(APP_ID_FMENGINE, &msg, NULL, 0);
        }
        else
        {
            //do nothing for QAC
        }
    }
    
    return bret;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ������ر���Ƶ�����
 * \param[in]    sound_out �Ƿ���Ƶ�����TRUEΪ�ǣ�FALSEΪ��
 * \param[in]    state �������״̬���� soundout_state_e ����
 * \param[in]    sound_func ������ֹͣ�豸��������ص�����������Ϊ NULL
 * \param[out]   none 
 * \return       bool
 * \retval           TRUE ������ر���Ƶ����ɹ���
 * \retval           FALSE ������ر���Ƶ���ʧ�ܡ�
 * \ingroup      common_func
 * \note 
 * \li  �ر���Ƶ���ʱ�����������ʹ�ܣ�����а������������á�
*******************************************************************************/
bool com_set_sound_out(bool sound_out, soundout_state_e state, void * sound_func)
{
    bool last_sound_flag, sound_flag;
    
    sound_out = sound_out;
    //��ȡ����ǰ����״̬
    if((get_keytone_enable() == TRUE) || ((soundout_state_e)get_soundout_state() != SOUND_OUT_STOP))
    {
        last_sound_flag = TRUE;
    }
    else
    {
        last_sound_flag = FALSE;
    }
    
    //������Ƶ���״̬
    g_app_info_state.sound_state &= (uint8)SOUND_BIT_SOUND_OUT_CLR;
    g_app_info_state.sound_state |= (uint8)state;
    
    //�ر���Ƶ������ڹر�pa��dac
    //�ر���Ƶ��������а����������ð�����pa
    if((state == SOUND_OUT_STOP) || (state == SOUND_OUT_PAUSE))
    {
        if(sound_func != NULL)
        {
            sound_stop_func __stop_sound = (sound_stop_func)sound_func;
            __stop_sound();
        }
        
        if(get_keytone_enable() == TRUE)
        {
            enable_dac(2, 44);//����Ƶ����˳���dac�ǹص��ģ���Ҫ����dac
            set_pa_volume(KEY_TONE_VOLUME);
            
            //���ð���������
            set_key_tone_vol(24);
            set_key_tone_freq(2);
            set_key_tone_period(2);
        }
        else//�Ѿ�������Ҫ������������Խ�ֹspeaker���
        {
            key_speakcheck(0);//��ֹspeaker���
        }
    }
    
    //��ȡ���ú�����״̬
    if((get_keytone_enable() == TRUE) || ((soundout_state_e)get_soundout_state() != SOUND_OUT_STOP))
    {
        sound_flag = TRUE;
    }
    else
    {
        sound_flag = FALSE;
    }
    
    //����״̬�ӹص�������pa
    if((last_sound_flag == FALSE) && (sound_flag == TRUE) && (state == SOUND_OUT_START))
    {
        //��pa
#ifdef AUDIO_DEVICE_PA_DDV
        enable_pa_ddv(0, 0);
#else
        enable_pa(0, 0);
#endif
    }
    //����״̬�ӿ����أ��ر�pa
    else if((last_sound_flag == TRUE) && (sound_flag == FALSE) && (state == SOUND_OUT_STOP))
    {
        //�ر�pa
#ifdef AUDIO_DEVICE_PA_DDV
        disable_pa_ddv(0, 0);
#else
        disable_pa();
#endif
    }
    else
    {
        //do nothing for QAC
    }
    
    //������Ƶ������ڴ�pa��dac
    //������Ƶ��������õ�ǰ����ֵ
    if((state == SOUND_OUT_START) || (state == SOUND_OUT_RESUME))
    {
        if(get_keytone_enable() == TRUE)
        {
            disable_dac();//�Ӱ�����������Ƶ�����Ҫ��dac�ǹص��ģ���Ҫ�ر�dac
        }
        else//֮ǰû��ʹ��speaker�������Ҫ�ڴ���ʹ�ܣ�����AP�п���dac
        {
            key_speakcheck(1);//ʹ��speaker���
        }
        
        if(sound_func != NULL)
        {
            sound_start_func __start_sound = (sound_start_func)sound_func;
            __start_sound(get_current_volume());
        }
    }
        
    return TRUE;
}

extern comval_t *sys_comval;

/******************************************************************************/
/*!                     
 * \par  Description:
 *    ����¼��ʹ�����
 * \param[in]    on_off ����¼�����˳�¼����TRUE��ʾ���룬FALSE��ʾ�˳�
 * \param[in]    type ¼�����ͣ�0��ʾ��ͨ¼����1��ʾ fm ¼��
 * \param[out]   none    
 * \return       bool
 * \retval           TRUE ����¼���ɹ�
 * \retval           FALSE ����¼��ʧ�� 
 * \note 
*******************************************************************************/
bool com_set_sound_record(bool on_off, uint8 type)
{
    if(on_off == TRUE)
    {
        if(type == 0)
        {
            if(get_keytone_enable() == TRUE)
            {
                com_set_sound_keytone(FALSE);
                g_keytone_backup = TRUE;
            }
            else
            {
                g_keytone_backup = FALSE;
            }
        }
    }
    else
    {
        if(type == 0)
        {
            if(g_keytone_backup == TRUE)
            {
                com_set_sound_keytone(TRUE);
                g_keytone_backup = FALSE;
            }
        }
    }
    
    return TRUE;
}

/*! \endcond */
