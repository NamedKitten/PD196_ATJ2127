/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-11-14          1.0              create this file
 *******************************************************************************/

#include "ap_alarm.h"



const key_map_t playing_key_map_list[] =
{
    /*! �̰���next�л���һ��or ab�����¼� */
    /*{{ KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },*/
    /*! �̰���prev�л���һ���¼� */
    /*{{ KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },*/
    /*! �̰�option�¼� */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    /*!�̰�play����/��ͣ�¼� */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    /*! �̰�KEY_RETURN ����ת��Ϊ �����¼� */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/******************************************************************************/
/*!
 * \par  Description:
 * \  �ر�music����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
bool music_close_engine(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //msg_reply_t temp_reply;
    //ɱ��music ����
    msg.type = MSG_KILL_APP_SYNC;
    msg.content.data[0] = APP_ID_MENGINE;
    //msg.content.data[1] = 0x00;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  ������Ϣ, ֹͣ����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
bool music_stop(void)
{
    bool bret = TRUE;
    msg_apps_t msg;

    //��Ϣ����(����Ϣ����)ֹͣ��������
    msg.type = MSG_MENGINE_STOP_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  ��ȡ��ǰ����״̬
 * \param[in]    mengine_status_t *play_status
 * \param[out]   none
 * \return       bool the result
 * \retval       1 sucess
 * \retval       0 failed
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
bool music_get_status(mengine_status_t* play_status)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;
    //��ȡ��ǰ����״̬��
    reply.content = play_status;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_GET_STATUS_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    return������Ϣ����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 *******************************************************************************/
app_result_e play_key_return(void)
{
    //ֹͣ����
    music_stop();
    //�ر�engine
    music_close_engine();
    
    return RESULT_ALARM_QUIT;//RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  ����gui��Ϣ
 * \param[in]    msg_apps_type_e cur_event
 * \param[out]   none
 * \return       app_result_e result
 * \retval       none
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_guimsg(msg_apps_type_e cur_event)
{
    //������Ϣ������ֵ
    app_result_e key_result = RESULT_IGNORE;
    //��ǰ���ֲ���״̬
    //mengine_status_t *music_status = &g_music_status;
    //gui�¼�
    if(cur_event == EVENT_PLAYING_RETURN)
    {
        key_result = play_key_return();
    }

    return key_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  ����music��˽����Ϣ��ϵͳ��Ϣ
 * \param[in]    private_msg_t
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e alarm_message_deal(private_msg_t *msg)
{
    app_result_e result = RESULT_IGNORE;

    switch (msg->msg.type)
    {
        case MSG_SD_OUT:
        //music_get_filepath(&g_file_path_info);
        if (g_alarm.path.file_path.plist_location.disk == DISK_H)
        {
            music_close_engine();
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
            result = RESULT_ALARM_QUIT;
        }
        break;

        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;

        default:
        result = com_message_box(msg->msg.type);
        break;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  ����gui��Ϣ
 * \param[in]    void para
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_sysmsg(void)
{
    private_msg_t alarm_msg;
    app_result_e msg_result = RESULT_IGNORE;
    if (get_app_msg(&alarm_msg) == TRUE)
    {
        msg_result = alarm_message_deal(&alarm_msg);
    }
    return msg_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  �������ȡ��ǰ״̬��Ϣ �ж��Ƿ���� �Ƿ��л�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool result
 * \retval       none
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
bool _playing_check_status(void)
{    
    music_get_status(&g_music_status);//��ȡ״̬��Ϣ
    if (g_music_status.err_status != EG_ERR_NONE)
    {
        play_eror_deal(); //�����˳�����
        
        return FALSE;
    }
    if (g_music_status.play_status == StopSta)  // ѭ������
    {
        alarm_play();
    }
    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    alarm��ʱ����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       none
 * \retval       
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 *******************************************************************************/
void alarm_timeout(void)
{
    //ֹͣ����
    music_stop();
    //�ر�engine
    music_close_engine();
    
    g_alarm_timeout =TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  ���ų���ѭ������
 * \param[in]    void
 * \param[out]   none
 * \return       app_result_e result
 * \retval       none
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e playing_loop_deal(void)
{
    //��ȡgui��Ϣ
    input_gui_msg_t gui_msg;
    //gui ��Ϣ��Ӧgui �¼�
    msg_apps_type_e gui_event;
    style_infor_t style;
    //picbox_private_t picbox_param;
    bool result;

    app_result_e ret_vals = RESULT_IGNORE;
    //��ʹ����now_play�����ر���
    change_app_state(APP_STATE_PLAYING_ALWAYS);

    style.type = UI_AP;
    
    ui_show_pic(P_ALARM_BG,0,0);
    while (1)
    {
        //�������ȡ��ǰ״̬��Ϣ
        result = _playing_check_status();
        if ((result != TRUE))
        {
            ret_vals = RESULT_ALARM_QUIT;
            break;
        }

        //ˢ��alarm����
        if (g_halfsec_timer == -1)
        {
            g_halfsec_timer = set_app_timer(APP_TIMER_ATTRB_UI, 500, play_scene_paint);
        }
        //����30s��ʱ
        if (g_tirtysec_timer == -1)
        {
            g_tirtysec_timer = set_app_timer(APP_TIMER_ATTRB_UI, ALARM_PLAY_TIME, alarm_timeout);
        }

        //��ȡ�ʹ���GUI��Ϣ��˽����Ϣ
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {
            if (com_key_mapping(&gui_msg, &gui_event, playing_key_map_list) == TRUE)
            {
                ret_vals = _scene_play_guimsg(gui_event);
            }
        }
        else
        {
            //һ��ѭ������һ�����͵���Ϣ
            ret_vals = _scene_play_sysmsg();
        }
        if(g_alarm_timeout == TRUE)
        {
            ret_vals = RESULT_ALARM_QUIT;
        }

        if ((ret_vals != RESULT_IGNORE) && (ret_vals != RESULT_NULL))
        {
            break;
        }
        //����10ms �����񽻻�
        sys_os_time_dly(2);
    }//end of while(1)
    //������ʱ��ȡ��
    if(g_halfsec_timer != -1)
    {
        g_halfsec_timer = -1;
        kill_app_timer(g_halfsec_timer);
    }
    if(g_tirtysec_timer != -1)
    {
        g_tirtysec_timer = -1;
        kill_app_timer(g_tirtysec_timer);
    }
    
    change_app_state(APP_STATE_NO_PLAY);
    return ret_vals;
}


