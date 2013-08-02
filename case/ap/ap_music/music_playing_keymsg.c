/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-14          1.0              create this file
 *******************************************************************************/
#include "app_music.h"

#include "app_music_playing.h"

const key_map_t playing_key_map_list[] =
{
    /*! ����next��� �¼� */
    {{ KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_FFOREWORD },
    /*! ����prev���� �¼� */
    {{ KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_FBACKWORD },
    /*! �̰���next�л���һ��or ab�����¼� */
    {{ KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_NEXTANDAB },
    /*! �̰���prev�л���һ���¼� */
    {{ KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PREVSONG },
    /*! �̰�option�¼� */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_OPTION },
    /*!�̰�play����/��ͣ�¼� */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PAUSE },
    /*! �̰�KEY_RETURN ����ת��Ϊ �����¼� */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    /*! ������־ */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

const uint32 low_speed_level[] =
{ 1000, 2000, 4000, 5000, 6000 };

const uint32 high_speed_level[] =
{ 4000, 12000, 24000, 48000, 96000};
/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_apoint(void)
 * \���ø���a��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
static bool music_set_apoint(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_SET_APOINT_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_bpoint(void)
 * \���ø���b��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
static bool music_set_bpoint(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_SET_BPOINT_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_clear_ab(void)
 * \���ab����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_clear_ab(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_CLEAR_AB_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_start_ab(void)
 * \����ab����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_start_ab(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_START_AB_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_stop(void)
 * \ֹͣ��������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_stop(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_STOP_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_pause(void)
 * \��ͣ��������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_pause(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_PAUSE_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_resume(void)
 * \�ָ���������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_resume(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_RESUME_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_fast_forward(void)
 * \���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_fast_forward(void)
{
    bool bret;
    //��Ϣ����(����Ϣ����)���
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_FFWD_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_fast_backward(void)
 * \����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_fast_backward(void)
{
    bool bret;
    //��Ϣ����(����Ϣ����)����
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_FBWD_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ����music���Ź����п���˵ȼ� ��5���ɵ�
 * \param[in]    none
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_play
 * \note
 * \li   �ȼ�Խ�󣬿���˼��Խ��
 */
/*******************************************************************************/
bool music_fastplay_step(void)
{
    bool bret;
    uint32 fastplay_step;

    uint8 level = g_speed_counter / 5;
    if (level > 4)
    {
        level = 4;
    }

    if (g_total_time < 600000)
    {
        fastplay_step = low_speed_level[level];
    }
    else
    {

        fastplay_step = high_speed_level[level];
    }

    bret = music_send_msg(&fastplay_step, NULL, MSG_MENGINE_SET_FFBSTEP_SYNC);

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_cancel_ffb(void)
 * \ȡ�������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_cancel_ffb(void)
{
    bool bret;
    //��Ϣ����(����Ϣ����)ȡ�������
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_CANCEL_FFB_SYNC);
    
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_fast_keyup(void)
 * \�������˵���Ϣ
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval       true up��Ϣ
 * \retval       false ���������
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
bool _scene_play_fast_keyup(void)
{
    input_gui_msg_t ui_msg;

    bool key_ret = FALSE;

    while (get_gui_msg(&ui_msg) == TRUE)
    {
        if ((ui_msg.data.kmsg.type & KEY_TYPE_SHORT_UP) == 0)
        {
            continue;
        }
        else
        {
            key_ret = TRUE;
            break;
        }
    }
    return key_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 play_key_prev(mengine_status_t* cur_status)
 * \����prev ������Ϣ
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keymsg.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_prev(mengine_status_t* cur_status)
{

    if (cur_status->ab_status != PlayAB_Null)
    {
        //clear AB
        music_clear_ab();
        //�ȴ�����a��
        music_start_ab();
        g_display_bitmap = g_display_bitmap | up_musicplaystatus;//����״̬
        g_paint_flag = TRUE;//��Ҫ���»��ƽ���
    }
    else
    {
        //�����ʱȡ�������
        if (cur_status->play_status == PlayFast)
        {
            music_cancel_ffb();
            g_display_bitmap |= ( up_musicplaystatus | up_musictimeinfo | up_musicprogress);//����״̬
            g_paint_flag = TRUE;//��Ҫ���»��ƽ���
        }
        else //if (cur_status->play_status == PlaySta)
        {
            music_play_prev();//���������л�����һ��
            //Ϊ������ʹ��
            g_switch_prev = 1;
            //SetSwitchInterface()//�л�����
            //g_getid3_info = TRUE;//��Ҫ���»�ȡid3��Ϣ
        }
        /*else
         {
         ;//����״̬����keep
         }*/
    }

    //����Ϣ���У����������İ�����Ϣ
    sys_mq_flush(MQ_ID_GUI);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 play_key_next(mengine_status_t* cur_status)
 * \����next������Ϣ
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keymsg.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_next(mengine_status_t* cur_status)
{

    //ͣ�������
    if (cur_status->play_status == PlayFast)
    {
        music_cancel_ffb();
        g_display_bitmap |= ( up_musicplaystatus | up_musictimeinfo | up_musicprogress);//����״̬
        g_paint_flag = TRUE;//��Ҫ���»��ƽ���
    }
    else
    {
        switch (cur_status->ab_status)
        {
            case PlayAB_Null:
            music_play_next();//���������л�����һ��          
            //g_getid3_info = TRUE;//��Ҫ���»�ȡid3��Ϣ
            break;

            case PlayAB_A:
            music_set_apoint();
            g_display_bitmap = g_display_bitmap | up_musicplaystatus;//����״̬
            g_paint_flag = TRUE;//��Ҫ���»��ƽ���
            break;

            case PlayAB_B:
            music_set_bpoint();
            g_display_bitmap = g_display_bitmap | up_musicplaystatus;//����״̬
            g_paint_flag = TRUE;//��Ҫ���»��ƽ���
            break;

            case PlayAB_Do:
            //clear AB
            music_clear_ab();
            g_display_bitmap = g_display_bitmap | up_musicplaystatus;//����״̬
            g_paint_flag = TRUE;//��Ҫ���»��ƽ���
            break;

            default:
            break;
        }
    }
    //����Ϣ���У����������İ�����Ϣ
    sys_mq_flush(MQ_ID_GUI);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 play_key_play(mengine_status_t* cur_status)
 * \����next������Ϣ
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keymsg.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_play(mengine_status_t* cur_status)
{
    if ((cur_status->play_status == PauseSta) || (cur_status->play_status == StopSta))
    {
        //play
        music_resume();
    }
    else if ((cur_status->play_status == PlaySta) || (cur_status->play_status == PlayAB))
    {
        if (cur_status->ab_status != PlayAB_Null)
        {
            //clear AB
            music_clear_ab();
        }
        //pause
        music_pause();
    }
    else
    {
        ;//nothing
    }
    g_display_bitmap = g_display_bitmap | up_musicplaystatus;//����״̬
    g_paint_flag = TRUE;//��Ҫ���»��ƽ���
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 play_key_return(void)
 * \����return������Ϣ
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keymsg.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_return(void)
{
    uint16 retval;
    switch (g_enter_mode)
    {
        //��browerѡ�����ļ�����//���ò����ļ�������Ϣ������
        case PARAM_FROM_RECORD:
        case PARAM_FROM_BROWSER:
        //send message create browser
        retval = RESULT_RETURN_BROWSER;
        break;
        
        case PARAM_BACKTO_PLAYING:
        case PARAM_BACKTO_PLAYED:
        //��main menuѡ����musicͼ��
        case PARAM_FROM_MAINMENU:
        //��ȡ�ļ�·��
        music_get_filepath(&g_file_path_info);
        if (g_file_path_info.file_source < FSEL_TYPE_SDFILE)
        {
            if(g_music_config.browser_sel == TRUE)
            {
                //����Ǵ�browserѡ��,�����·���browser
                retval = RESULT_RETURN_BROWSER;
            }
            else
            {
                //���򷵻�main ap,����standby�����ϵ��Ѳ���
                //�Ӵ�������ѡ��
                retval = RESULT_MAIN_APP;
            }
        }
        else
        {
            retval = RESULT_LISTMENU_LIST;
        }
        break;

        default://�������
        retval = RESULT_LISTMENU_LIST;
        break;
    }
    return retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _scene_play_gui(msg_gui_t *gui_msg)
 * \����gui��Ϣ
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_guimsg(input_gui_msg_t *gui_msg)
{
    //gui ��Ϣ��Ӧgui �¼�
    msg_apps_type_e cur_event;

    //������Ϣ������ֵ
    app_result_e key_result = RESULT_IGNORE;

    //��ǰ���ֲ���״̬
    mengine_status_t *music_status = &g_music_status;

    if ((g_music_status.play_status == PlayFast) && ((gui_msg->data.kmsg.type & KEY_TYPE_SHORT_UP) != 0))
    {
        //�����״̬�յ��κ�up��Ϣ��ת��Ϊ����������¼�
        cur_event = EVENT_PLAYING_NEXTANDAB;
    }
    else
    {
        //δ�ҵ���Ч��GUIӳ��
        if (com_key_mapping(gui_msg, &cur_event, playing_key_map_list) == FALSE)
        {
            //�˳���ǰGUI�¼�ѭ��
            goto msg_end;
        }
    }

    //����gui�¼�
    switch (cur_event)
    {
        //������Ϣ���
        case EVENT_PLAYING_FFOREWORD:
        //������Ϣ����
        case EVENT_PLAYING_FBACKWORD:
        if (music_status->play_status != PlayFast)
        {
            g_speed_counter = 0;
            
            //���ó�ʼ����
            music_fastplay_step(); 

            //���ÿ����״̬
            if (cur_event == EVENT_PLAYING_FFOREWORD)
            {
                music_fast_forward();
            }
            else
            {
                music_fast_backward();
            }
            g_display_bitmap |= up_musicplaystatus;
            g_paint_flag = TRUE;
        }
        else
        {
            if (_scene_play_fast_keyup() == TRUE)
            {
                music_cancel_ffb();
                g_display_bitmap |= ( up_musicplaystatus | up_musictimeinfo | up_musicprogress);
                g_paint_flag = TRUE;
            }
            else
            {
                if ((g_speed_counter % 5) == 0)
                {
                    music_fastplay_step();
                }
            }
        }
        break;

        //������Ϣ��һ��
        case EVENT_PLAYING_PREVSONG:
        key_result = play_key_prev(music_status);
        break;

        //������Ϣ��һ��/AB����
        case EVENT_PLAYING_NEXTANDAB:
        key_result = play_key_next(music_status);
        break;

        //option����
        case EVENT_PLAYING_OPTION:
        if (g_audible_now == TRUE)
        {
            key_result = RESULT_SETMENU_AUDIBLE;
        }
        else
        {
            key_result = RESULT_SETMENU_MUSIC;
        }
        break;

        //������ͣ
        case EVENT_PLAYING_PAUSE:
        key_result = play_key_play(music_status);
        break;

        //���ذ���
        case EVENT_PLAYING_RETURN:
        key_result = play_key_return();
        break;

        default:
        stop_app_timer(scroll_frush_id);
        stop_app_timer(time_frush_id);

        key_result = com_message_box(cur_event);
        //�ָ�����ˢ�¶�ʱ��
        if ((key_result == RESULT_IGNORE) || (key_result == RESULT_NULL))
        {
            key_result = RESULT_REDRAW;
        }
        restart_app_timer(scroll_frush_id);
        //1s��ʱ����������
        restart_app_timer(time_frush_id);
        break;
    }
    if (key_result == RESULT_REDRAW)
    {
        //ˢ�½����ʶ
        SetFullInterface()
        g_paint_flag = TRUE;//��Ҫ���»��ƽ���
        key_result = RESULT_NULL;
    }

    if (g_paint_flag == TRUE)
    {
        //��ȡ��ǰ����״̬
        //music_get_status(&g_music_status);
        g_check_mengine_status = TRUE;
    }
    msg_end: return key_result;
}

