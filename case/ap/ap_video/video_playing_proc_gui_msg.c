/*******************************************************************************
 *                              US212A
 *                            Module: VIDEO_PLAYING_PROC_GUI_MSG
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-12-8 9:03:17           1.0              build this file
 *******************************************************************************/
/*!
 * \file     video_playing_proc_gui_msg.c
 * \brief    video���Ž���԰����¼�ֵ����Ӧģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 * 				 �ڰ���״̬���ް����¼�����˱�ģ�鲻�ᱻ���ã�������bank����
 * \version 1.0
 * \date    2011-12-8
 *******************************************************************************/
#include "video.h"

/*! \brief ���������¼���Ϣӳ���*/
const key_map_t playing_key_map_list[] =
{
    /*! ����next��� �¼� */
    {{ KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_VIDEO_PLAYING_FFORWORD },
    /*! ����prev���� �¼� */
    {{ KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD},   EVENT_VIDEO_PLAYING_FBACKWORD },
    /*! �̰�next�л���һ��or����ȫ�� */
    {{ KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_VIDEO_PLAYING_NEXT },
    /*! �̰�prev�л���һ����or����ȫ��  */
    {{ KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_VIDEO_PLAYING_PREV },
    /*! �̰�option�¼� */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_VIDEO_PLAYING_OPTION },
    /*!�̰�play����/��ͣ�¼� */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_VIDEO_PLAYING_PAUSE },
    /*!�̰�vol�����¼� */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_VIDEO_PLAYING_RETURN },
    /*!vol�¼� */
    {{ KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_VIDEO_PLAYING_VOLUME },
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

static const uint8 video_volume_tbl[32]=
{
    40, 40, 39, 39, 38, 38, 37, 36, 35, 34,
    33, 32, 31, 30, 28, 27, 26, 25, 24, 23,
    22, 20, 19, 17, 16, 14, 12,10, 8, 6, 4, 
    0
};

/******************************************************************************/
/*!
 * \par  Description:
 *	  video �޸�����
 * \param[in]    is_key_next �����Ӽ�
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
static void _video_change_volume(uint8 is_key_next)
{
    uint8 volum_set;
    uint8 tab_index;
    
    //��������
    if (is_key_next == TRUE)
    {
        if (g_comval.volume_current < g_comval.volume_limit)
        {
            g_comval.volume_current++;
            g_display_bitmap |= up_videovoladd;
        }
    }
    else
    {
        if (g_comval.volume_current > 0)
        {
            g_comval.volume_current--;
            g_display_bitmap |= up_videovolsub;
        }
    }
    //�ֲ�ˢ�£�ͬʱ����ȫ����ʱ��
    g_video_paint_flag = VIDEO_PAINT_PARTIAL;
    g_video_timer_flag |= up_video_fullscreen_timer;

    //ת������
    tab_index = (31 - g_comval.volume_current);
    volum_set = (video_volume_tbl[tab_index]& 0x3f);

    //����Ӳ������
    set_pa_volume((uint32)volum_set);

    //���浱ǰ����ֵ
    set_current_volume(g_comval.volume_current);
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video nex������
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e _video_play_key_next(void)
{
    if (g_video_cur_window == VIDEO_FULLSCREEN)
    {
        g_video_cur_window = VIDEO_PROGRESSBAR;
        g_video_timer_flag |= up_video_fullscreen_timer;
    }
    else if (g_video_cur_window == VIDEO_VOLUMEBAR)
    {
        _video_change_volume(TRUE);
    }
    else
    {
        if (g_video_play_status == VIDEO_FF)
        {
            //�����������Ϊ0
            g_video_speed_control = 0;
            g_video_timer_counter = 0;
            //���²��ű�ʶ
            g_video_play_status = g_video_play_status_backup;
            g_display_bitmap |= up_videoplaystatus | up_videospeed;
        }
        else
        {
            //����һ��
            g_video_change_file = up_video_playnext;
        }
    }
    return RESULT_NULL;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video prev������
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e _video_play_key_prev(void)
{
    if (g_video_cur_window == VIDEO_FULLSCREEN)
    {
        g_video_cur_window = VIDEO_PROGRESSBAR;
        g_video_timer_flag |= up_video_fullscreen_timer;
    }
    else if (g_video_cur_window == VIDEO_VOLUMEBAR)
    {
        _video_change_volume(FALSE);
    }
    else
    {
        if (g_video_play_status == VIDEO_FB)
        {
            //��������Ϊ0
            g_video_speed_control = 0;
            g_video_timer_counter = 0;
            g_video_play_status = g_video_play_status_backup;
            g_display_bitmap |= up_videoplaystatus | up_videospeed;
            if(g_fast_back_end == TRUE)
            {
                g_video_change_file = up_video_playprev;
                g_fast_back_end = FALSE;
            }
        }
        else
        {
            //����һ��
            g_video_change_file = up_video_playprev;
        }
    }
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video play������
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e _video_play_key_play(void)
{
    if (g_video_cur_window != VIDEO_PROGRESSBAR)
    {
        g_video_cur_window = VIDEO_PROGRESSBAR;
        g_video_timer_flag |= up_video_fullscreen_timer;
    }
    else
    {
        g_video_paint_flag = VIDEO_PAINT_PARTIAL;
        g_display_bitmap |= up_videoplaystatus;
        if (g_video_play_status == VIDEO_PAUSE)
        {
            g_video_play_status = VIDEO_PLAY;
        }
        else
        {
            //��ȫ������״̬ת����ͣ״̬
            g_video_play_status = VIDEO_PAUSE;
            //��ͣ״̬��PROGRESSBARģʽ��ֹͣȫ����ʱ��
            g_video_timer_flag |= clr_video_fullscreen_timer;
        }
    }
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video �ȴ�����̧����
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
static bool _video_wait_key_up(void)
{
    input_gui_msg_t ui_msg;

    while (get_gui_msg(&ui_msg) == TRUE)
    {
        if ((ui_msg.data.kmsg.type & KEY_TYPE_SHORT_UP) == 0)
        {
            continue;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video ����˴���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e _video_play_key_fastplay(uint8 is_key_next)
{

    if (g_video_cur_window == VIDEO_VOLUMEBAR)
    {
        _video_change_volume(is_key_next);
    }
    else
    {
        if ((g_video_play_status != VIDEO_FF) && (g_video_play_status != VIDEO_FB))
        {
            g_video_play_status_backup = g_video_play_status;
            if (is_key_next == TRUE)
            {
                g_video_play_status = VIDEO_FF;
            }
            else
            {
                g_video_play_status = VIDEO_FB;
            }
            //���ó�ʼ�����������
            g_video_speed_control = 1;
            g_video_cur_window = VIDEO_PROGRESSBAR;
            g_display_bitmap |= up_videoplaystatus | up_videotimeinfor | up_videospeed;
            g_video_timer_flag |= clr_video_fullscreen_timer;
        }

        //��Ƶ�����ʱΪ��ֹ������ٶȹ���������Ϣ���������޷�
        //���յ�KEY_UP��Ϣ��Ӧ��������������KEY_HOLD��Ϣ�Ա�֤
        //���Խػ�KEY_UP��Ϣ�����������
        if (_video_wait_key_up() == TRUE)
        {
            g_video_speed_control = 0;
            g_video_timer_counter = 0;
            g_video_play_status = g_video_play_status_backup;
            g_display_bitmap |= up_videoplaystatus | up_videospeed;
        }

    }
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video ����������
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e _video_play_key_vol(void)
{
    //��Ҫ�����贰
    g_video_cur_window = VIDEO_VOLUMEBAR;
    g_display_bitmap |= up_videovolume | up_videoBackGDPic;
    g_video_paint_flag = VIDEO_PAINT_PARTIAL;

    g_video_timer_flag |= up_video_fullscreen_timer;

    //���˵�������VOL������Ϣ
    com_filter_key_hold();
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ����video GUI��Ϣ
 * \param[in]   cur_event:�����¼�ֵ
 * \param[out]  none
 * \return      the result�������¼�������ֵ
 * \ingroup    VIDEO
 * \note
 *******************************************************************************/
app_result_e video_playing_proc_gui_msg(input_gui_msg_t *ui_msg)
{
    //����״̬��Ļ���������Σ��Ƿ���Ҫת��
    app_result_e ui_result = RESULT_NULL;
    msg_apps_type_e cur_event;

    if (com_key_mapping(ui_msg, &cur_event, playing_key_map_list) == TRUE)
    {
        switch (cur_event)
        {
            case EVENT_VIDEO_PLAYING_NEXT:
            ui_result = _video_play_key_next();
            break;

            case EVENT_VIDEO_PLAYING_PREV:
            ui_result = _video_play_key_prev();
            break;

            case EVENT_VIDEO_PLAYING_PAUSE:
            ui_result = _video_play_key_play();
            break;

            case EVENT_VIDEO_PLAYING_FBACKWORD:
            ui_result = _video_play_key_fastplay(FALSE);
            break;

            case EVENT_VIDEO_PLAYING_FFORWORD:
            ui_result = _video_play_key_fastplay(TRUE);
            break;

            case EVENT_VIDEO_PLAYING_VOLUME:
            ui_result = _video_play_key_vol();
            break;

            case EVENT_VIDEO_PLAYING_RETURN:
            ui_result = RESULT_VIDEO_RETURN;
            break;

            case EVENT_VIDEO_PLAYING_OPTION:
            ui_result = RESULT_VIDEO_OPTIONMENU;
            break;

            case EVENT_ENTER_KEYLOCK:
            //������Ϣ����
            change_keylock_state(TRUE);
            break;
            case EVENT_ENTER_KEYLOCK_UN:
            //������Ϣ����
            change_keylock_state(FALSE);
            break;
            case EVENT_ENTER_KEYLOCK_HOLD:
            if (g_video_cur_window != VIDEO_PROGRESSBAR)
            {
                g_video_cur_window = VIDEO_PROGRESSBAR;
            }
            g_display_bitmap |= up_videoheaderbar;
            g_video_paint_flag = VIDEO_PAINT_ALL;
            break;

            default:
            //��Ƶ�����Ӧ�ȼ���Ϣ���Ƿ�Ӧ������ͣ��ǰ����??
            ui_result = com_message_box(cur_event);
            if (ui_result == RESULT_REDRAW)
            {
                video_set_full_flag()
                g_video_cur_window = VIDEO_PROGRESSBAR;
                g_video_prev_window = VIDEO_FULLSCREEN;
                //�ػ���棬ǿ������Ϊ����״̬
                g_video_play_status = VIDEO_PLAY;
                ui_clear_screen(NULL);

            }
            //�ָ�ˢ��ģʽ
            lcd_set_draw_mode(g_video_draw_mode);
            break;
        }

        if ((g_video_cur_window != VIDEO_FULLSCREEN) && (g_video_play_status == VIDEO_PLAY))
        {
            g_video_timer_flag |= up_video_fullscreen_timer;
        }
    }


    return ui_result;
}



