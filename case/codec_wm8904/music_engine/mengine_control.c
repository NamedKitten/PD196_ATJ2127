/*******************************************************************************
 *                              US211A
 *                            Module: music engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     fiona.yang     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_status_deal(void)
 * \����״̬����,����ǰ���ֲ���״̬
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
void mengine_status_deal(void)
{
    mmm_mp_status_t mmm_status;
    bool bret = TRUE;

    //��ȡ��ǰ����״̬
    mmm_mp_cmd(mp_handle, MMM_MP_GET_STATUS, (unsigned int) &mmm_status);
            
    //��ŵ�ǰ�Ĳ���ʱ��ͱ�����
    if ((g_eg_status_p->play_status != PauseSta) && (g_eg_status_p->play_status != StopSta))
    {
        g_eg_playinfo_p->cur_time = (uint32) mmm_status.cur_time;
    }

    switch (mmm_status.status)
    {
        case MMM_MP_ENGINE_PLAYING://��������
        if (g_eg_status_p->play_status == PlayAB)/*ab ����״̬*/
        {
            //��������,�ص���������״̬
            if ((g_eg_status_p->ab_status == PlayAB_Do) && (mmm_status.ab_play_flag == AB_PLAY_CLEAR))
            {
                g_eg_status_p->ab_status = PlayAB_A;
            }
        }
        else if (g_eg_status_p->play_status == PlaySta)//��������
        {
            //������������л�����
            if ((g_eg_cfg_p->repeat_mode == FSEL_MODE_INTRO) && ((uint32) mmm_status.cur_time >= (g_intro_start
                    + INTRO_INTERVAL)))
            {
                mengine_file_switch(STOP_NORMAL, END_SWITCH, PLAY_NORMAL);
            }

            //�쳣�������
            if (g_eg_status_p->fast_status != FBPlay_Null)
            {
                mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int) 0);
                g_eg_status_p->fast_status = FBPlay_Null;
            }
        }
        else
        {
            //����˲�����������
            ;//do nothing
        }
        break;

        case MMM_MP_ENGINE_STOPPED://ֹͣ

        //sd �ļ�����ֻ��Ҫֹͣ
        if (g_eg_cfg_p->fsel_type == FSEL_TYPE_SDFILE)
        {
            _stop(STOP_NORMAL);//ֹͣ����,���ļ��ر�
            break;
        }

        //�������ŵ�β or �����β or ���˵�ͷֹͣ
        if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayFast)
                || (g_eg_status_p->play_status == PlayAB))
        {
            //�������ŵ�β
            if (g_eg_status_p->fast_status == FBPlay_Null)
            {
                mengine_file_switch(STOP_NORMAL, END_SWITCH, PLAY_NORMAL);
            }
            else if (g_eg_status_p->fast_status == FFPlaySta)
            {
                if (g_is_audible != TRUE)
                {
                    //�����β
                    mengine_file_switch(STOP_NORMAL, END_SWITCH, PLAY_FFON);
                }
                else
                {
                    //���ļ����Ϊ�Ѳ�����ϣ����֮ǰ�Ķϵ���Ϣ
                    //pos�ļ���15�ֽڱ�ʾ�ļ��Ƿ񲥷����
                    g_audible_info_p->playbackPosFilebuffer[15] = 0x01;
                }
            }
            else
            {
                if (g_is_audible != TRUE)
                {
                    //��audible����֧�ִ�һ�׸������˵�����һ�׸���
                    mengine_file_switch(STOP_NORMAL, FORCE_SWITCH_PREV, PLAY_FBON);
                }
            }
        }
        break;

        case MMM_MP_ENGINE_ERROR://����

        //ǰ̨����music UI����ǰ̨��������״̬�����ִ���ֱ������һ��
        if ((g_app_info_state.app_state != APP_STATE_PLAYING) || (g_app_info_state.screensave_state == TRUE))
        {
            //ǰ̨����music ui,��ֱ������һ��
            _error_timeout_handle();
        }
        else
        {
            //���Ȼ�ȡ����ţ���Ϊstop���������ô����
            mengine_save_errno_no();

            //����ȴ�ǰ̨��ӦUI״̬
            bret = _stop(STOP_NORMAL);//ֹͣ����

            //�ָ���Ĭ��״̬,��ֹ�����������ʽ�����ļ��ٴο����
            g_eg_status_p->play_status = StopSta;
            g_eg_status_p->fast_status = FBPlay_Null;
            g_eg_status_p->ab_status = PlayAB_Null;
        }
        break;

        default:
        break;
    }
    if ((g_eg_status_p->play_status == StopSta) || (g_eg_status_p->play_status == PauseSta))
    {
        g_app_info_state.state = ENGINE_STATE_PAUSE;
    }
    else
    {
        g_app_info_state.state = ENGINE_STATE_PLAYING;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void _check_megine_status(void)
 * \�������״̬��ʱ�����������ñ�־λ
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
static void _check_megine_status(void)
{
    g_check_status_flag = TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_control_block(void)
 * \���湦�ܴ�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_control_block(void)
{
    g_mengine_result = RESULT_NULL;

//    Codec_init(0);
    //250ms��ѯһ���м��״̬
    g_status_timer = set_app_timer(APP_TIMER_ATTRB_CONTROL, 250, _check_megine_status);
    while (1)
    {
        //��ѯ��ǰ����״̬
        //if ((g_check_status_flag == TRUE) || (mengine_info.eg_status.play_status == PlayFast))
        if (g_check_status_flag == TRUE)
        {
            g_check_status_flag = FALSE;
            mengine_status_deal();
        }

        //��ȡ��������ϸ
        if (get_app_msg_for_engine(&g_cur_msg) == TRUE)
        {
            switch (g_cur_msg.msg.type)
            {
                /*��ȡ��Ϣ���*/
                case MSG_MENGINE_GET_PLAYMODE_SYNC://��ȡѭ��ģʽ+shuffle
                mengine_get_playmode(&g_cur_msg);
                break;

                case MSG_MENGINE_GET_FILEPATH_SYNC://��ȡ�ļ�location��Ϣ
                mengine_get_filepath(&g_cur_msg);
                break;

                case MSG_MENGINE_GET_FILEINFO_SYNC://��ȡ�ļ���ʽ��b/s����ʱ�����Ϣ
                mengine_get_fileinfo(&g_cur_msg);
                break;

                case MSG_MENGINE_GET_STATUS_SYNC://��ȡ��ǰ����״̬
                mengine_get_status(&g_cur_msg);
                break;

                case MSG_MENGINE_GET_PLAYINFO_SYNC://��ȡ��ǰ������Ϣ
                mengine_get_playinfo(&g_cur_msg);
                break;

                default:
                g_mengine_result = mengine_message_done_bank(&g_cur_msg);
                break;
            }
        }

        if (g_mengine_result == RESULT_APP_QUIT)
        {
            if (g_is_audible == TRUE)
            {
                mengine_save_audible_breakinfo();
            }
            //�˳�Ӧ�ùرմ���
            _stop(STOP_NORMAL);
            break;
        }

        //����10ms �����񽻻�
        sys_os_time_dly(g_delay_time);
    }
    
    Codec_exit();
    
    kill_app_timer(g_status_timer);
    return g_mengine_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_reply_msg(void* msg_ptr, bool ret_vals)
 * \�ظ�ͬ����Ϣ
 * \param[in]    void  msg_ptr ��Ϣ�ṹ��
 * \param[in]    bool  ret_vals ����¼������� TRUE �ɹ� FALSE ʧ��
 * \param[out]   none
 * \return       app_result_E
 * \retval       RESULT_IGNOR ����
 * \ingroup      mengine_control.c
 * \note  �ظ���Ϣʱ������ret_vals�Ľ��������Ӧ��ɹ���ʧ��
 */
/*******************************************************************************/
void mengine_reply_msg(void* msg_ptr, bool ret_vals)
{
    //��Ϣָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    if (ret_vals == TRUE)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }

    //�ظ�ͬ����Ϣ(�����ź���)
    //reply_sync_msg(msg_ptr);
    libc_sem_post(data_ptr->sem);

    return;

    //return RESULT_IGNORE;
}

