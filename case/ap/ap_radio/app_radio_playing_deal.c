/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-11-14          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

extern app_result_e scene_play_search(FM_SeekDir_e dir);

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 get_index_from_num(uint8 num)
 * \�û���̨�и��ݵ�̨�Ż�ȡ����
 * \param[in]    �û���̨��̨��
 * \param[in]
 * \param[out]   none
 * \return       ������
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
uint8 get_index_from_num(uint8 num)
{
    uint8 index;

    for (index = 0; index < MAX_STATION_COUNT; index++)
    {
        if (g_userlist_table[index] == num)
        {
            break;
        }
    }

    if (index == MAX_STATION_COUNT)
    {
        index = 0xff;
    }
    return index;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  select_station(FM_SeekDir_e dir)
 * \�л�����һ������һ���ѱ����̨
 * \param[in]    dir �л�����
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e select_station(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_IGNORE;
    /* ��ǰ����Ϊ�û���̨ģʽ*/
    if (g_radio_config.band_mode == Band_MAX)
    {
        if (dir == DIR_UP)
        {
            //�Ǳ����̨�е���һ�������̨
            if (g_userlist_index == 0xff)
            {
                g_userlist_index = 0;
            }
            else if (g_userlist_index < (g_userlist_total - 1))
            {
                g_userlist_index++;
            }
            else
            {
                //���ص�һ��
                g_userlist_index = 0;
            }
        }
        else
        {
            //�Ǳ����̨�е���һ�������̨
            if (g_userlist_index == 0xff)
            {
                g_userlist_index = 0;
            }
            else if (g_userlist_index > 0)
            {
                g_userlist_index--;
            }
            else
            {
                //��һ������
                g_userlist_index = g_userlist_total - 1;
            }
        }
        //��̨��
        g_radio_config.FMStatus.station = g_userlist_table[g_userlist_index];
        //ͨ����̨�Ż�ȡƵ��ֵ
        if ((g_radio_config.FMStatus.station > 0) && (g_radio_config.FMStatus.station <= MAX_STATION_COUNT))
        {
            g_radio_config.FMStatus.freq = g_user_station.fmstation_user[g_radio_config.FMStatus.station - 1];
        }
        //�û���̨ˢ�µ�̨����
        paint_flag |= PAINT_STATION_INFOR;
    }
    /* ��ǰ���ŷ��û�ģʽ*/
    else
    {
        //Ԥ���б������ѱ����̨
        if (g_radio_config.FMStatus.station_count > 0)
        {
            if (dir == DIR_UP)
            {
                if (g_radio_config.FMStatus.station < MAX_STATION_COUNT)
                {
                    g_radio_config.FMStatus.station++;
                }
                else
                {
                    //���ص�һ��
                    g_radio_config.FMStatus.station = 1;
                }
                g_radio_config.FMStatus.station = get_real_station_num(g_radio_config.FMStatus.station, DIR_UP);
            }
            else
            {
                if (g_radio_config.FMStatus.station > 1)
                {
                    g_radio_config.FMStatus.station--;
                }
                else if (g_radio_config.FMStatus.station == 0)
                {
                    //�Ǳ����̨�е���һ�������̨
                    g_radio_config.FMStatus.station = 1;
                }
                else
                {
                    //��һ������
                    g_radio_config.FMStatus.station = MAX_STATION_COUNT;
                }
                g_radio_config.FMStatus.station = get_real_station_num(g_radio_config.FMStatus.station, DIR_DOWN);
            }
            //ͨ����̨�Ż�ȡƵ��ֵ
            g_radio_config.FMStatus.freq = *(pfreq + g_radio_config.FMStatus.station - 1);
        }
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e change_freq(FM_SeekDir_e dir)
 * \�л�����һ������һ��Ƶ��
 * \param[in]    dir �л�����
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e change_freq(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_IGNORE;
    uint8 space;
    uint8 num;

    space = 100;
    if ((g_radio_config.band_mode == Band_Europe) || (g_radio_config.band_mode == Band_MAX))
    {
        //�û�ģʽ��ŷ��Ƶ��,  ��50k  ����
        space = 50;
    }
    //if (g_radio_config.band_mode > Band_MAX)
    //{
    //    g_radio_config.band_mode = Band_China_US;
    //}

    if (dir == DIR_UP)
    {
        if (g_radio_config.FMStatus.freq < max_freq[g_radio_config.band_mode])
        {
            g_radio_config.FMStatus.freq += space;
        }
        else
        {
            //���Ƶ���л�����С
            g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        }
    }
    else
    {
        if (g_radio_config.FMStatus.freq > min_freq[g_radio_config.band_mode])
        {
            g_radio_config.FMStatus.freq -= space;
        }
        else
        {
            //��СƵ���л������
            g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
        }
    }

    //�ж��л����Ƶ���Ƿ��ڵ�ǰ�����ŵ��б���
    num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);

    //�л����Ƶ����δ����Ƶ��
    if (num == 0xff)
    {
        if (g_radio_config.band_mode == Band_MAX)
        {
            g_userlist_index = 0xff;
            //�û���̨ˢ�µ�̨����
            paint_flag |= PAINT_STATION_INFOR;
        }
        g_radio_config.FMStatus.station = 0;
    }
    //�л����Ƶ�����ѱ���Ƶ��
    else
    {
        if (g_radio_config.band_mode == Band_MAX)
        {
            //���ݵ�̨�Ż�ȡ����
            g_userlist_index = get_index_from_num(num);
            if (g_userlist_index == 0xff)
            {
                //û�ҵ���������Ϊδ����
                num = 0;
            }
            //�û���̨ˢ�µ�̨����
            paint_flag |= PAINT_STATION_INFOR;
        }
        g_radio_config.FMStatus.station = num;
    }

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_playstatus(void)
 * \radio ���ų���������ͣ�л�����
 * \param[in]    none
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e deal_playstatus(void)
{
    app_result_e ret = RESULT_IGNORE;
    bool status;

    //��ȡ��ǰ����״̬
    status = radio_get_vol_status();

    if (g_engine_status.FM_VolStatus == STATUS_PLAY)
    {
        //����->  ��ͣ
        radio_set_mute(SetMUTE);
    }
    else
    {
        //��ͣ-> ����
        radio_set_mute(releaseMUTE);
    }
    //ˢ������ͼ��
    paint_flag |= PAINT_VOL;
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e playwin_key_return(radio_playwin_mode_e mode)
 * \radio ���ų���return ������
 * \param[in]    mode
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e playwin_key_return(radio_playwin_mode_e mode)
{
    app_result_e ret = RESULT_IGNORE;

    if (mode == ENTER_PLAY_FROM_NOWPLAY)
    {
        //����mainmenu ap
        ret = RESULT_MAIN_APP;
    }
    else if (mode == ENTER_PLAY_FROM_LIST)
    {
        if (g_radio_listtype == STATIONLIST_USER)
        {
            //�û��б����
            ret = RESULT_RADIO_USER_LIST;
        }
        else
        {
            //Ԥ���б����
            g_stalist_entry = ENTER_NORMAL;
            ret = RESULT_RADIO_STATION_LIST;
        }
    }
    else
    {
        //�������������radio  ���˵�
        ret = RESULT_RADIO_MAINMENU;
#ifdef ALARMFM
        if (g_radio_from_alarm != 0)
        {
            ret = RESULT_ALARM_QUIT;
        }
#endif
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_play_guimsg(msg_apps_type_e cur_event, radio_playwin_mode_e mode)
 * \radio ���ų���gui��Ϣ����
 * \param[in]    cur_event  ������������Ϣ�¼�
 * \param[in]    mode  ���벥�ų�����ģʽ, �����˷��ص��ĸ�����
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e deal_play_guimsg(msg_apps_type_e cur_event, radio_playwin_mode_e mode)
{
    app_result_e result = RESULT_IGNORE;
    bool set_freq = FALSE;
    bool need_play = FALSE;

    switch (cur_event)
    {
        //��һ�������̨
        case EVENT_RADIO_NEXT_PRESET:
        result = select_station(DIR_UP);
        set_freq = TRUE;
        break;

        //��һ�������̨
        case EVENT_RADIO_PREV_PRESET:
        result = select_station(DIR_DOWN);
        set_freq = TRUE;
        break;

        //�л�����һ��Ƶ��
        case EVENT_RADIO_NEXT_FREQ:
        result = change_freq(DIR_UP);
        set_freq = TRUE;
        break;

        //�л�����һ��Ƶ��
        case EVENT_RADIO_PREV_FREQ:
        result = change_freq(DIR_DOWN);
        set_freq = TRUE;
        break;

        //�����Զ���̨
        case EVENT_RADIO_SEEK_NEXT:
        result = scene_play_search(DIR_UP);
        set_freq = TRUE;
        need_play = TRUE;
        break;

        //��ǰ���Զ���̨
        case EVENT_RADIO_SEEK_PREV:
        result = scene_play_search(DIR_DOWN);
        set_freq = TRUE;
        need_play = TRUE;
        break;

        //���벥�ų�����option �˵�
        case EVENT_RADIO_OPTION:
        result = RESULT_RADIO_OPTION_PLAYWIN;
        break;

        //������ͣ�л�
        case EVENT_RADIO_PLAY_MUTE:
        result = deal_playstatus();
        break;

        //���ذ���
        case EVENT_RADIO_RETURN:
        result = playwin_key_return(mode);
        break;

        default:
        //�ȼ���Ϣ����
        if (timer_flush_radioinfo != -1)
        {
            kill_app_timer(timer_flush_radioinfo);
            timer_flush_radioinfo = -1;
        }
        result = com_message_box(cur_event);

        if ((result == RESULT_IGNORE) || (result == RESULT_NULL) || (result == RESULT_CONFIRM))
        {
            result = RESULT_REDRAW;
        }
        else if (result == RESULT_ALARMFM) //���Ź�������fm alarm��Ϣ
        {
            g_radio_from_alarm = 1;
            alarmfm_in_playing();
            result = RESULT_REDRAW;
        }
        else
        {
            ; //qac
        }
        break;
    }
    if (result == RESULT_REDRAW)
    {
        paint_flag |= (uint8) PAINT_ALL;
    }
    if (set_freq == TRUE)
    {
        //ˢ��Ƶ�������ʾ
        paint_flag |= PAINT_FREQ;
        //����Ƶ�㲥��
        radio_set_freq(g_radio_config.FMStatus.freq);

    }
    if (need_play == TRUE)
    {
        //���Զ���̨���ͷž�������
        if (get_antenna_state() == TRUE)
        {
            //�����Ӷ���, �Ŵ�PA
            //Ϊ�����ѵ�̨��,���ڰ����԰���,���¶�������쳣�����޷��ͷž���,�˴�����common�ӿ��ж϶�������״̬
            radio_set_mute(releaseMUTE);
        }
    }
    return result;
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
