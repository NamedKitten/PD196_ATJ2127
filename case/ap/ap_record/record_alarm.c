/********************************************************************************
 *                               us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wzliu         2011-10-13         v1.0              create this file
 ********************************************************************************/
 /*!
 * \file     record_alarm.c
 * \brief    ��ʱfm¼������غ���
 * \author   wzliu
 * \version  1.0
 * \date  2011/10/13
 *******************************************************************************/
#include "ap_record.h"
#include "fm_interface.h"
#include "fmengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_freq(uint16 freq)
 * \ͬ������Ƶ�ʲ���
 * \param[in]    freq  ���õ�Ƶ��ֵ( AP ��)
 * \param[out]   none
 * \return
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_freq(uint16 freq)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_SETFREQ_SYNC;
    msg.content.data[0] = (uint8) (freq & 0xff);
    msg.content.data[1] = (uint8) ((freq & 0xff00) >> 8);
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_volume(uint8 vol)
 * \��������ֵ
 * \param[in]     vol
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess   ���������ɹ�
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_volume(uint8 volume)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_SET_VOLUME;
    msg.content.addr = &volume;

    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_mute(FM_MUTE_e mode)
 * \���þ�����������
 * \param[in]    mode
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_mute(FM_MUTE_e mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = (uint8) mode;
    msg.content.data[1] = (uint8) g_comval.volume_current;

    msg.type = MSG_FMENGINE_SETMUTE;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;

    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_open_engine(void)
 * \װ��FM ��̨�������
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 * \note
 */
/*******************************************************************************/
bool radio_open_engine(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = APP_ID_FMENGINE;
    msg.content.data[1] = 0;
    //����radio ��̨�������
    msg.type = MSG_CREAT_APP_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MANAGER, &msg, &msg_reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_modual_init(void)
 * \��ʼ��FM  ģ��
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed--- ͬ����Ϣ����ʧ�ܣ���ģ���ʼ��ʧ��
 * \note
 */
/*******************************************************************************/
bool radio_modual_init(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = Band_China_US;

    //��ʼ��FM ģ��
    msg.type = MSG_FMENGINE_INIT;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_modual_reinstall(void)
 * \��������FMĬ��ȫ�ֱ�����ֵ
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed--- ͬ����Ϣ����ʧ�ܣ���ģ���ʼ��ʧ��
 * \note
 */
/*******************************************************************************/
bool radio_modual_reinstall(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    //����ֵ
    uint16 freq = 0x1247;

    msg.content.data[0] = (uint8) 0x02;
    msg.content.data[1] = (uint8) (freq & 0xff);
    msg.content.data[2] = (uint8) (freq >> 8);

    //��ʼ��FM ģ��
    msg.type = MSG_FMENGINE_RESINTALL;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;

    }
    return bret;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_close_engine(void)
 * \ж������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note  radioUI ��һ�㲻��Ҫ���������ж�أ���Ԥ���˽ӿ�
 * \        FM �����ж��һ�����������̷���ж������
 */
/*******************************************************************************/
bool radio_close_engine(void)
{
    bool bret = TRUE;    
    msg_apps_t msg;
    
    radio_modual_reinstall();  //fm��ʼ�����ݱ�������˴��������³�ʼ����
    
    //ɱ��radio ��̨����
    msg.type = MSG_KILL_APP_SYNC;
    msg.content.data[0] = APP_ID_FMENGINE;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_get_modual_status(Module_Status_e* status)
 * \��ȡ��ǰģ��״̬
 * \param[in]    none
 * \param[out]   Module_Status_e* status
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_modual_status(Module_Status_e* status)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_MODUAL_STATUS;
    msg_reply.content = status;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_engine_init(void)
 * \�����ʼ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return        success/fail
 * \retval   True:   �����Ѿ���װ��ģ���Ѿ���ʼ��
 * \retval   False:  ��װ����ʧ�ܻ��ʼ��ģ��ʧ��
 * \note
 */
/*******************************************************************************/
bool radio_engine_init(void)
{
    engine_type_e engine_type;
    msg_apps_t msg;
    bool result = TRUE;
    bool need_init = FALSE;

    //��ȡ��ǰ��̨��������
    engine_type = get_engine_type();
    msg.type = MSG_KILL_APP_SYNC;    //ע��ԭ�з�radio ��̨
    if (engine_type == ENGINE_MUSIC)
    {
        msg.content.data[0] = APP_ID_MENGINE;
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0); //����ͬ����Ϣ
    }

    //��ǰδ��װ���棬�谲װ
    if (engine_type != ENGINE_RADIO)
    {
        if (get_antenna_state() == FALSE)
        {
            //��ʾ��������
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, INSERT_EARPHONE);
            return FALSE;
        }
        
        result = radio_open_engine();
        if (result == TRUE)
        {
            need_init = TRUE; //���ʼ��ģ��
        }
    }
    //��ǰ�Ѱ�װ���棬���ж�ģ���Ƿ��Ѿ���ʼ��
    //¼�����̿��ܻὫfm ������ ��פ���ݿռ串��,  ����, ֱ�ӷ�һ���ʼ������
    else
    {
        //radio_get_modual_status(&status);
        //if (status != MODULE_WORK)
        {
            need_init = TRUE;
        }
    }

    if (need_init == TRUE)
    {
        // ģ���ʼ��֮ǰ��mute
        //radio_set_mute(SetMUTE);
        //��ʼ��ģ��
        result = radio_modual_init();
        //result = TRUE;    //���Խ׶�
#if 1     //���Խ׶�
        if (!result)
        {//ģ���ʼ��ʧ�ܣ���ʾӲ������
            //show_dialog(FMERROR, DIALOG_INFOR_WAIT, 0);
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, MDIROUT);
        }
        else
#endif
        {
            //��ʼ������mute��ˢ�½���󣬿�ʼ����
            radio_set_mute(SetMUTE); //releaseMUTE
            //first_wait = TRUE;
        }
    }
    else
    {
        ;//first_wait = FALSE;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_alarm_init(void)
 * \fm��ʼ��
 * \param[in]    none
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
bool radio_alarm_init(void)
{
    if (!radio_engine_init())
    {
        return FALSE; //��ʼ��ʧ�ܷ���
    }
    //��ʼ������mute��ˢ�½���󣬿�ʼ����
    if (!radio_set_mute(SetMUTE)) //releaseMUTE
    {
        return FALSE;
    }
    if (!radio_set_freq(fmrec_frequency))
    {
        return FALSE;
    }
    if (!radio_set_volume(fmrec_volume))
    {
        return FALSE;
    }
    if (!radio_set_mute(releaseMUTE))
    {
        return FALSE;
    }
    /*
     //���Ӷ�����Ϊ����������
     show_dialog(USE_ANT, DIALOG_INFOR_WAIT, 0);

     if(timer_auto_seek != -1)
     {
     kill_app_timer(timer_auto_seek);
     }
     //���ö�ʱ�������ݲ�����
     timer_auto_seek = set_app_timer(APP_TIMER_ATTRB_UI, 300, radio_auto_proc);
     */
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _radio_get_freq(void)
 * \��ȡ��ǰƵ����Ϣ
 * \param[in]     none
 * \param[out]   none
 * \return       none
 * \note
 */
/*******************************************************************************/
void _radio_get_freq(void)
{
    msg_apps_t msg;
    msg_reply_t msg_reply;
    uint32 temp_freq;
    uint32 tmp;
    uint16 engine_freq;

    msg.type = MSG_FMENGINE_GET_FREQ;
    msg_reply.content = &engine_freq;

    //����ͬ����Ϣ
    send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    //    if (msg_reply.type == MSG_REPLY_FAILED)
    //    {
    //        ;
    //    }

    temp_freq = (uint32)engine_freq + 0x10000; //��ȡ������fmƵ����Ҫ+0x10000
    //��дfmƵ��������ʾ
    tmp = temp_freq / 1000;
    if (tmp < 100)
    {
        libc_itoa(tmp, g_scene_param.fm_freq, 2);
        g_scene_param.fm_freq[2] = '.';
        tmp = (temp_freq - tmp * 1000) / 100;
        libc_itoa(tmp, &g_scene_param.fm_freq[3], 1);
        g_scene_param.fm_freq[4] = 'F';
        g_scene_param.fm_freq[5] = 'M';
        g_scene_param.fm_freq[6] = 0;
    }
    else
    {
        libc_itoa(tmp, g_scene_param.fm_freq, 3);
        g_scene_param.fm_freq[3] = '.';
        tmp = (temp_freq - tmp * 1000) / 100;
        libc_itoa(tmp, &g_scene_param.fm_freq[4], 1);
        g_scene_param.fm_freq[5] = 'F';
        g_scene_param.fm_freq[6] = 'M';
        g_scene_param.fm_freq[7] = 0;
    }
}

