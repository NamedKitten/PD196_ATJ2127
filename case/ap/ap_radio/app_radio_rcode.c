/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      mikeyang    2011-11-9 16:45:04           1.0              build this file
 *******************************************************************************/

#include "app_radio.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \RadioUI  ��ȡ˽����Ϣ�Ĵ���
 * \param[in]    msg  ˽����Ϣ�ṹ
 * \param[out]   none
 * \return  app_result_e �����Ϣ
 * \note RadioUI �Ĵ���ĸ���˽����Ϣ�Ļص�����
 *******************************************************************************/
app_result_e radio_app_msg_callback(private_msg_t *pri_msg)
{
    app_result_e ret_result = RESULT_NULL;
    engine_type_e engine_type;

    switch (pri_msg->msg.type)
    {
        case MSG_APP_QUIT:
        ret_result = RESULT_APP_QUIT;
        break;

        /* �ص����ڲ��Ž��� */
        case MSG_APP_BACKTO_PLAYING:
        if (g_ss_delay_flag == TRUE)//delay�˳�����
        {
            gui_screen_save_exit();
            g_ss_delay_flag = FALSE;
        }

        //��ȡ��ǰ��̨��������
        engine_type = get_engine_type();
        if (engine_type == ENGINE_MUSIC)
        {
            //��ǰ��̨Ϊmusic, ��ص�����
            ret_result = RESULT_MUSIC;
        }
        else
        {
            //�ص�radio���ų�����, ������Ƶ��,��������
            need_set_freq = FALSE;
            ret_result = RESULT_RADIO_START_PLAY;
        }
        break;

        default:
        ret_result = com_message_box(pri_msg->msg.type);
        if (ret_result == RESULT_ALARMFM) //���Ź�������fm alarm��Ϣ
        {
            g_radio_from_alarm = 1;
            alarmfm_in_playing();
            ret_result = RESULT_REDRAW;
        }
        break;
    }

    return ret_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void init_point_freq(void)
 * \��ʼ��ָ�򲥷�Ƶ�ʱ��ָ��
 * \param[in]    void
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void init_point_freq(void)
{
    /* ��ǰ����Ϊ�û���̨ģʽ*/
    if (g_radio_config.band_mode == Band_MAX)
    {
        pfreq = &g_user_station.fmstation_user[0];
    }
    /* ��ǰ����ΪԤ���̨ģʽ*/
    else
    {
        if (g_radio_config.band_mode == Band_Japan)
        {
            pfreq = &g_radio_config.fmstation_jp[0];
        }
        else if (g_radio_config.band_mode == Band_Europe)
        {
            pfreq = &g_radio_config.fmstation_eu[0];
        }
        else
        {
            pfreq = &g_radio_config.fmstation_us[0];
        }
    }
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \uint8 get_station_count(void)
 * \param[in]    none
 * \param[out]   none
 * \return    saved station number
 * \retval
 * \retval
 * \note   ��ȡ��ǰģʽ( ����) �µı����̨����
 */
/*************************************************************************************************/
uint8 get_station_count(void)
{
    uint16 *ptr = pfreq;
    uint8 num, i;

    num = 0;
    for (i = 0; i < MAX_STATION_COUNT; i++)
    {
        //if(*ptr !=0)
        if ((*ptr <= max_freq[g_menu_band]) && (*ptr >= min_freq[g_menu_band]))
        {
            num++;
        }
        ptr++;
    }
    return num;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 whether_in_list( uint16  freq, uint16 *p_freq)
 * \�ж�ָ��Ƶ�ʵĵ�̨�Ƿ��ڵ�ǰ�ı����б���
 * \param[in]    freq
 * \param[in]    p_freq
 * \param[out]   none
 * \return       ��ǰƵ����ָ�����б��з��ص�̨�ţ����򷵻�0xff
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
uint8 whether_in_list(uint16 freq, uint16 *p_freq)
{
    uint8 ret;
    uint16 *ptr;
    uint8 num;
    uint16 tmp;

    /* ��ǰ���ε�̨�б�*/
    ptr = p_freq;

    for (num = 0; num < MAX_STATION_COUNT; num++)
    {
        tmp = *(ptr + num);
        if (tmp == freq)
        {
            //���б������ҵ�
            break;
        }
    }

    //���б���δ�ҵ�
    if (num == MAX_STATION_COUNT)
    {
        ret = 0xff;
    }
    else
    {
        ret = num + 1;
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e show_dialog(uint16 id, uint8 dialog_type, uint8 default_icon)
 * \RadioUI ʹ�õ�dialog �Ӻ���
 * \param[in]    id  ��ʾ���ַ���id��dialog_type  �Ի�������; default_icon:Ĭ����
 * \param[out]   none
 * \return   app_result_e �����Ϣ
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e show_dialog(uint16 id, uint8 dialog_type, uint8 default_icon)
{
    app_result_e result;
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    uint16 style_id;

    string_desc.data.id = id;
    string_desc.language = UNICODEID;

    dialog_com.dialog_type = dialog_type;
    if (dialog_type != DIALOG_BUTTON_INCLUDE)
    {
        dialog_com.button_type = BUTTON_NO_BUTTON;
        style_id = DIALOG_MSG;
    }
    else
    {
        //Ŀǰ����radioUI ��ֻʹ�õ��Ǻͷ�ť
        dialog_com.button_type = BUTTON_YESNO;
        dialog_com.active_default = default_icon;
        style_id = DIALOG_ASK;
    }
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    result = gui_dialog(style_id, &dialog_com);
    return result;
}

/******************************************************************************
 *   ���ֳ���������ڳ�פ��
 *******************************************************************************/
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

    //����, ����Ƶ���л������е�����
    //set_pa_volume(0);
    radio_set_mute(SetMUTE);
    msg.type = MSG_FMENGINE_SETFREQ_SYNC;
    msg.content.data[0] = (uint8)(freq & 0xff);
    msg.content.data[1] = (uint8)((freq & 0xff00) >> 8);
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    //�������������Ϊ��ǰ����
    //set_pa_volume(g_comval.volume_current);
    radio_set_mute(releaseMUTE);
    return bret;
}

#if 0  //��פrcode�ռ䲻��, �˺�����bank
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
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_get_seekflag(void)
 * \��ȡӲ����̨�Ƿ���ɱ�־
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess   ����Ӳ����̨���
 * \retval           0 failed     ��ȡʧ�ܻ�Ӳ����̨��δ���
 * \note
 */
/*******************************************************************************/
bool radio_get_seekflag(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_AUTO_SEEK_OVER;
    msg_reply.content = &g_engine_status.STC_flag;

    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if ((msg_reply.type == MSG_REPLY_FAILED) || (g_engine_status.STC_flag == HARDSEEK_DOING))
    {
        bret = FALSE;

    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_softseek(uint16 freq)
 * \���������̨Ƶ��
 * \param[in]    freq, dir
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note   �����̨:   �����ǰƵ��Ϊ��̨������TRUE
 */
/*******************************************************************************/
bool radio_set_softseek(uint16 freq, uint8 dir)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_SET_SOFTSEEK;
    msg.content.data[0] = (uint8)(freq & 0xff);
    msg.content.data[1] = (uint8)((freq & 0xff00) >> 8);
    msg.content.data[2] = dir;
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
 * \bool radio_set_hardseek(uint16 freq, uint8 dir)
 * \��������Ӳ����̨
 * \param[in]    freq-- Ӳ����̨��ʼƵ��
 * \param[in]    dir --- ��̨����
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess   ����Ӳ����̨�ɹ�
 * \retval           0 failed
 * \note  ��̨����˵��
 * bit0   ~~~  ���ϻ�������̨��0:   UP;    1: DOWN
 * bit1   ~~~  �Ƿ�߽���ơ�     0:   ����;    1:  ������
 */
/*******************************************************************************/
bool radio_set_hardseek(uint16 freq, uint8 dir)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_SET_HARDSEEK;
    msg.content.data[0] = (uint8)(freq & 0xff);
    msg.content.data[1] = (uint8)((freq & 0xff00) >> 8);
    msg.content.data[2] = dir;
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
 * \bool radio_break_hardseek(void)
 * \�˳�Ӳ����̨����
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess   �˳�Ӳ����̨�ɹ�
 * \retval           0 failed
 * \note   ������Ӳ����̨��������Ӧ������ֹӲ����̨
 */
/*******************************************************************************/
bool radio_break_hardseek(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_SET_SEEKBREAK;

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
 * \��ȡ��ǰƵ����Ϣ
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_freq(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_FREQ;
    msg_reply.content = &g_engine_status.FM_CurrentFreq;

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
 * \��ȡ��̨�б�����һ������һ���ѱ����̨�ĵ�̨��
 * \param[in]     num ��ʼ��̨��  dir   ����
 * \param[out]   ��õĵ�̨��
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
uint8 get_real_station_num(uint8 num, FM_SeekDir_e dir)
{
    uint16 freq;
    uint8 i;

    //ԭʼƵ��ֵ
    //freq = *(pfreq+g_radio_config.FMStatus.station-1);

    for (i = 0; i < MAX_STATION_COUNT; i++)
    {
        freq = *(pfreq + num - 1);
        if ((freq >= min_freq[g_radio_config.band_mode]) && (freq <= max_freq[g_radio_config.band_mode]))
        {
            //�ҵ���Ч̨���˳�
            g_radio_config.FMStatus.station = num;
        }
        else
        {
            if (dir == DIR_UP)
            {
                num++;
                if (num > MAX_STATION_COUNT)
                {
                    num = 1;
                }
            }
            else
            {
                num--;
                if (num == 0)
                {
                    num = MAX_STATION_COUNT;
                }
            }
        }
    }
    return g_radio_config.FMStatus.station;
}

/*! \endcond */

