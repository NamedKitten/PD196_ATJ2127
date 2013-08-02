/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-22          1.0              create this file
 *******************************************************************************/
/*!
 * \file     music_control.c
 * \brief    music��פ����������Ƶ��ʹ�õĴ������ڸ��ļ�
 * \author   fiona.yang
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date    2011-09-22
 *******************************************************************************/
#include "app_music.h"

#include "app_music_playing.h"

#if 0
/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_send_sync_msg(msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
 * \music ui�����淢����Ϣ����
 * \param[in]    msg ��Ϣ����ָ��
 * \param[in]    reply  ��Ϣ��ִָ��
 * \param[out]   timeout ��ʱ����
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 * \li  ����applib��send_sync_msg�Ͷ�ʱ������ͬ��UI bank,Ϊ��Сbank�л������ú���
        �򻯺��ʵ�ַ���music��פ����,Ҳ���Բ�ʹ�øô���
 */
/*******************************************************************************/
bool music_send_sync_msg(msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
{
    private_msg_t cur_send_pmsg;

    msg_reply_t temp_reply;
    //uint8 i;
    os_event_t *sem;//ͬ����Ϣͬ���ź���ָ��
    bool retvalue = TRUE;

    //��ȡ��̨�������ͣ����mengine�����ڣ�����ʧ��
    engine_type_e engine_type = get_engine_type();

    if(engine_type != ENGINE_MUSIC)
    {
        return FALSE;
    }

    //����ʱ�ȴ��ź�����ͬʱֻ����1���̷߳���ͬ����Ϣ
    libc_sem_wait(thread_mutex, 0);

    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));
    libc_sem_init(&sem, 0);
    cur_send_pmsg.sem = sem;
    if(reply == NULL)
    {
        cur_send_pmsg.reply = &temp_reply;
    }
    else
    {
        cur_send_pmsg.reply = reply;
    }

    //������Ϣ��������
    if(sys_mq_send(MQ_ID_EGN, (void *)&cur_send_pmsg) == -1)
    {
        libc_sem_destroy(&sem);
        libc_sem_post(thread_mutex);
        return FALSE;
    }

    //�ȴ�ͬ����Ϣ��Ӧ
    if(libc_sem_wait(sem, timeout) < 0)
    {
        retvalue = FALSE;
    }

    libc_sem_destroy(&sem);
    libc_sem_post(thread_mutex);
    return retvalue;
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_param(void *param, uint32 msg_type)
 * \��ȡ������ز�������
 * \param[in]    input_param �������ָ��
 * \param[in]    msg_type  ��Ϣ����
 * \param[out]   output_param �������ָ��
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_send_msg(void *input_param, void *output_param, uint32 msg_type)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;

    //�����������ĵ�ַ
    msg.content.addr = input_param;

    //��Ż�ȡ�����ĵ�ַ
    reply.content = output_param;

    //��Ϣ����(����Ϣ����)
    msg.type = msg_type;

#if 0
    //����ͬ����Ϣ
    bret = music_send_sync_msg(&msg, &reply, 0);
#else
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
#endif

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_filepath(file_path_info_t *filepath)
 * \��ȡ��ǰ�����ļ���·����Ϣ
 * \param[in]    filepath����ļ�·��buffer��ָ��  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_filepath(file_path_info_t *filepath)
{
    return music_send_msg(NULL, filepath, MSG_MENGINE_GET_FILEPATH_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_fileinfo(mmm_mp_file_info_t* fileinfo)
 * \��ȡ��ǰ���ŵ��ļ���ʽ����ʱ�䡢�����ʵ���Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_fileinfo(mmm_mp_file_info_t* fileinfo)
{
    return music_send_msg(NULL, fileinfo, MSG_MENGINE_GET_FILEINFO_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_status(mengine_status_t* play_status)
 * \��ȡ��ǰ����״̬
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_status(mengine_status_t* play_status)
{
    return music_send_msg(NULL, play_status, MSG_MENGINE_GET_STATUS_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playinfo(mengine_playinfo_t* play_info)
 * \��ȡ��ǰ���ŵ�ʱ��ͱ�������Ϣ
 * \param[in]    play_info  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_playinfo(mengine_playinfo_t* play_info)
{
    return music_send_msg(NULL, play_info, MSG_MENGINE_GET_PLAYINFO_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_bkinfo(mmm_mp_bp_info_t* curbk_info)
 * \��ȡ��ǰ���ŵĶϵ���Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_bkinfo(mmm_mp_bp_info_t* curbk_info)
{
    return music_send_msg(NULL, curbk_info, MSG_MENGINE_GET_BKINFO_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playmode(uint8* playmode)
 * \��ȡѭ��ģʽ&shuffleģʽ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_playmode(uint8* playmode)
{
    return music_send_msg(NULL, playmode, MSG_MENGINE_GET_PLAYMODE_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play(void)
 * \��ʼ��������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_play(void)
{
    return music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_SYNC);

}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_filepath(file_path_info_t *filepath)
 * \���ò����ļ���·����Ϣ
 * \param[in]    filepath����ļ�·��buffer��ָ��  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_set_filepath(file_path_info_t *filepath)
{
    if(filepath->file_path.dirlocation.disk == DISK_H)
    {
        if(sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            music_deal_sd_out();
            return FALSE;
        }
    }
    return music_send_msg(filepath, NULL, MSG_MENGINE_SET_FILEPATH_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play_next(void)
 * \������һ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_play_next(void)
{
    bool bret;
    uint32 msg_type;

    if ((g_audible_now == TRUE)
        && (g_music_config.section_mode == SWITCH_SECTION)
        && (g_music_status.err_status == EG_ERR_NONE))
    {
        //��Ϣ����(����Ϣ����)������һ��
        msg_type = MSG_MENGINE_CAHPTER_NEXT_SYNC;
        //ֻ��Ҫ�����ļ����
        g_display_bitmap |= up_musictracks;
    }
    else
    {
        msg_type = MSG_MENGINE_PLAY_NEXT_SYNC;
        //�����л��ļ�ˢ�±�־λ
        SetSwitchInterface()
    }
    //����ͬ����Ϣ
    bret = music_send_msg(NULL, NULL, msg_type);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play_prev(void)
 * \������һ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_play_prev(void)
{
    bool bret;
    uint32 msg_type;

    if ((g_audible_now == TRUE)
        && (g_music_config.section_mode == SWITCH_SECTION)
        && (g_music_status.err_status == EG_ERR_NONE))
    {
        //��Ϣ����(����Ϣ����)������һ��
        msg_type = MSG_MENGINE_CHAPTER_PREV_SYNC;
        //ֻ��Ҫ�����ļ����
        g_display_bitmap |= up_musictracks;
    }
    else
    {
        msg_type = MSG_MENGINE_PLAY_PREV_SYNC;
        //�����л��ļ�ˢ�±�־λ
        SetSwitchInterface()
    }

    //����ͬ����Ϣ
    bret = music_send_msg(NULL, NULL, msg_type);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_flag_update(void)
 * \1�붨ʱ���ò��ű�ʶ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
void play_flag_update(void)
{
    g_timer_counter++;
    g_check_mengine_status = TRUE;
    if (g_timer_counter == 4)
    {
        g_timer_counter = 0;
        g_display_bitmap |= up_musictimeinfo;//����ʱ��
        g_display_bitmap |= up_musicprogress;//������

        if (((g_music_status.play_status != StopSta) && (g_music_status.play_status != PauseSta)) && (g_lyric_flag
                == TRUE))
        {
            g_display_bitmap |= up_lyricflag;//���
        }
        //g_paint_flag = TRUE;//��Ҫ���»��ƽ���
        if (g_music_status.ab_status != PlayAB_Null)
        {
            g_display_bitmap |= up_musicplaystatus;//����״̬
            g_ab_switch = !g_ab_switch;
        }

        if (g_music_status.play_status == PlayFast)
        {
            g_speed_counter++;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool play_scroll_deal(void)
 * \��������
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval       0 ʧ����Ҫ��������/��������
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool play_scroll_deal(void)
{
    bool result;

    scroll_param_t scroll_param;
    style_infor_t style_infor, style_icon;

    //��ȡ��������
    style_infor.style_id = ID3_SCROLL_TEXTBOX;
    style_infor.type = UI_AP;

    //�Դ�ֱ������ʽ��ʾPICBOX��Ҫ��Ϊ��֡������֡0����Ϊ����ͼ��
    if (g_audible_now == TRUE)
    {
        style_icon.style_id = ID3_AUDIBLE_PICBOX;
    }
    else
    {
        style_icon.style_id = ID3_MUSIC_PICBOX;
    }
    style_icon.type = UI_AP;

    //�����������
    scroll_param.direct = g_scroll_vars.direct;
    scroll_param.init = g_scroll_vars.init;
    scroll_param.update = g_scroll_vars.update;
    scroll_param.active = g_scroll_vars.index;
    scroll_param.id3_text = &style_infor;
    scroll_param.id3_icon = &style_icon;

    //���ù���
    result = (bool)(int32) ui_scroll_string_ext(g_scroll_content, &scroll_param);

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scroll_update(void)
 * \������ʱ��������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
void play_scroll_update(void)
{
    g_scroll_deal = TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void id3_scroll_deal(void)
 * \ID3����״̬������
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
void id3_scroll_deal(void)
{
    switch (g_scroll_status)
    {
        //case ID3_DEAL_INIT:
        //play_scroll_init();
        //break;

        case ID3_DEAL_SCROLL:
        if (g_scroll_vars.update == TRUE)
        {
            g_scroll_vars.update = FALSE;
            modify_app_timer(scroll_frush_id, 80);
        }

        if (play_scroll_deal() == FALSE)
        {
            if (g_scroll_vars.init == TRUE)
            {
                //������ֺ�������Ҫ�����������޸�Ϊ����ģʽ
                g_scroll_vars.init = FALSE;
                //�޸�Ϊ��ֱ����
                g_scroll_vars.direct = ID3_SCROLL_VERTICAL;
            }
            else
            {
                //��ǰģʽ��������������3s��ʱ���л�����ģʽ
                modify_app_timer(scroll_frush_id, 3000);
                g_scroll_status = ID3_DEAL_SWITCH;
            }
        }
        else
        {
            g_scroll_vars.init = FALSE;
        }
        break;

        case ID3_DEAL_SWITCH:
        if (g_scroll_vars.direct == ID3_SCROLL_VERTICAL)
        {
            g_scroll_vars.index++;

            if (g_scroll_vars.index > 2)
            {
                g_scroll_vars.index = 0;
            }

            //��һ���������
            g_scroll_vars.direct = ID3_SCROLL_HORIZONTAL;
            g_scroll_vars.init = TRUE;
        }
        else
        {
            //�޸�Ϊ��ֱ����
            g_scroll_vars.direct = ID3_SCROLL_VERTICAL;
        }
        modify_app_timer(scroll_frush_id, 80);
        g_scroll_status = ID3_DEAL_SCROLL;
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _scene_play_sysmsg(void)
 * \����ϵͳ��Ϣ
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_sysmsg(void)
{
    //get message
    private_msg_t music_msg;
    app_result_e msg_result = RESULT_IGNORE;
    if (get_app_msg(&music_msg) == TRUE)
    {
        msg_result = music_message_deal(&music_msg);
    }
    return msg_result;
}
