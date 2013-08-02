/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang    2011-09-22          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

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
 * \bool radio_close_engine(void)
 * \ж������
 * \param[in]    void
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

    msg.content.data[0] = (uint8) g_radio_config.band_mode;

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
 * \��ʼ��FM  ģ��
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

    msg.content.data[0] = (uint8) g_radio_config.band_mode;
    msg.content.data[1] = (uint8) (g_radio_config.FMStatus.freq & 0xff);
    msg.content.data[2] = (uint8) (g_radio_config.FMStatus.freq >> 8);

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
 * \bool radio_modual_standby(void)
 * \FM ģ�����Standby
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_modual_standby(void)
{
    bool bret = TRUE;
    msg_apps_t msg;

    //FM ģ�����standby
    msg.type = MSG_FMENGINE_STANDBY;
    //�����첽��Ϣ
    bret = send_async_msg(APP_ID_FMENGINE, &msg);
    return bret;
}

/*******************************************************************************
 *                                          ������Ϣ���
 ********************************************************************************/

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_band(radio_band_e mode)
 * \���õ�̨����
 * \param[in]    mode
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_band(radio_band_e mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = (uint8) mode;

    msg.type = MSG_FMENGINE_SETBAND;
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
 * \bool radio_set_throd(uint8 val)
 * \������̨����
 * \param[in]    val ��̨����ֵ
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_throd(uint8 val)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = val;

    msg.type = MSG_FMENGINE_SETTHRED;
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
bool radio_set_volume(uint8 vol)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_SET_VOLUME;
    msg.content.addr = &vol;//g_comval.volume_current;

    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;

    }
    return bret;
}

/*******************************************************************************
 *                                          ��ȡ��Ϣ���
 ********************************************************************************/

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_get_status(uint8 mode)
 * \��ȡ��ǰ����״̬��Ϣ
 * \param[in]     mode
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note   mode ��ȡ״̬�ڵ�:   0-- ��������ʱȡ
 * \note   1-- Ӳ��seek ������ȡ
 */
/*******************************************************************************/
bool radio_get_status(uint8 mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_STATUS;
    msg.content.data[0] = mode;
    msg_reply.content = &g_engine_status;

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
 * \bool radio_get_band(void)
 * \��ȡ��ǰ������Ϣ
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_band(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_BAND;
    msg_reply.content = &g_engine_status.FM_CurBand;

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
 * \bool radio_get_intensity(void)
 * \��ȡ��ǰ�ź�ǿ��
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_intensity(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_INTENSITY;
    msg_reply.content = &g_engine_status.FM_SignalStg;

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
 * \bool radio_get_antenna(void)
 * \��ȡ��ǰ����״̬
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess    �����Ѳ���
 * \retval           0 failed     ��ȡʧ�ܻ�����δ����
 * \note
 */
/*******************************************************************************/
bool radio_get_antenna(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;
    uint8 status;

    msg.type = MSG_FMENGINE_GET_ANTENNA;
    msg_reply.content = &status;

    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if ((msg_reply.type == MSG_REPLY_FAILED) || (status == 0))
    {
        bret = FALSE;

    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_get_stereo(void)
 * \��ȡ��ǰ������״̬
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_stereo(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_STEREO;
    msg_reply.content = &g_engine_status.FM_Stereo_Status;

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
 * \bool radio_get_vol_status(void)
 * \��ȡ��ǰ����״̬������or  ��ͣ
 * \param[in]    none
 * \param[out]   none
 * \return       status
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_vol_status(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_PLAYSTATUS;
    msg_reply.content = &g_engine_status.FM_VolStatus;
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
 * \bool radio_get_volume(void)
 * \��ȡ��ǰ����ֵ
 * \param[in]    none
 * \param[out]   none
 * \return       status
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_volume(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_VOLUME;
    msg_reply.content = &g_comval.volume_current;
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
 * \bool radio_get_modual_status(Module_Status_e* status)
 * \��ȡ��ǰģ��״̬
 * \param[in]    none
 * \param[out]   none
 * \return       status
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
 * \void alarmfm_in_playing(void)
 * \alarmfm ����
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval           
 * \retval           
 * \note
 */
/*******************************************************************************/
void alarmfm_in_playing(void)
{
    alarm_vars_t temp_alarm_val;
    if (g_radio_from_alarm != 0)
    {
        sys_vm_read(&temp_alarm_val, VM_ALARM, sizeof(alarm_vars_t));
        //��ʡ�ռ䣬ֻȡ��FM�й���Ϣ
        libc_memcpy(&alarmfm_end_time, &temp_alarm_val.g_aufm.end_time, sizeof(time_t)); //��ʱFM�Ľ���ʱ��
        g_radio_config.FMStatus.freq = temp_alarm_val.g_aufm.frequency; //FMƵ��
        //alarmfm_volume = temp_alarm_val.g_aufm.volume; //FM����
        g_comval.volume_current = temp_alarm_val.g_aufm.volume; //FM����
        radio_set_freq(g_radio_config.FMStatus.freq);
        radio_set_volume(g_comval.volume_current);//alarmfm_volume);
    }
}
