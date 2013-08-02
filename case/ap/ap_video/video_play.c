/*******************************************************************************
 *                              US212A
 *                            Module: video
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     video
 * \brief    video��play����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/22
 *******************************************************************************/
#include  "video.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    video ��⿨���Ƿ���ں���
 * \param[in]    void
 * \param[out]   void
 * \return       app_resule_e the result
 * \ingroup      video_play
 * \note
 * \li   �����ǰʹ���ǿ��̶����̲����ڣ�����RESULT_CARD_OUT_ERROR
 *******************************************************************************/
app_result_e video_check_card_out(void)
{
    app_result_e ui_result = RESULT_NULL;
    if (g_video_var.path.file_path.dirlocation.disk == DISK_H)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            gui_dialog_msg(DIALOG_MSG_V, DIALOG_INFOR_WAIT, S_CARD_OUT);
            ui_result = RESULT_CARD_OUT_ERROR;
        }
    }
    return ui_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    video ���ų�������״̬��⺯��
 * \param[in]    void
 * \param[out]   void
 * \return       app_resule_e the result
 * \ingroup      video_play
 * \note
 * \li   ����ѭ���в��ϵ��øú�����ʵ����Ƶ���룬����˹���
 * \li   ͨ�����ݲ�ͬ�Ĳ���ֵ(cur_step),ʵ����Ƶ������ٶȿɵ�
 *******************************************************************************/
app_result_e video_check_play_status(void)
{
    app_result_e ret_val = RESULT_NULL;
    int32 dec_result;
    uint8 cur_step = g_video_speed_control * 4;
    //    cur_step = 1;
#ifdef PC
    return RESULT_NULL;
#endif
    if (g_fast_back_end == TRUE)
    {
        return RESULT_NULL;
    }

    /*���������״̬�Ľ������*/
    switch (g_video_play_status)
    {
        case VIDEO_PLAY:
        /*��һ��VA*/
        dec_result = mmm_vp_cmd(g_video_handle, MMM_VP_PLAY, 0);
        break;

        case VIDEO_FF:
        /*���4��VA*/
        dec_result = mmm_vp_cmd(g_video_handle, MMM_VP_FAST_FORWARD, cur_step);
        break;

        case VIDEO_FB:
        /*����8��VA*/
        dec_result = mmm_vp_cmd(g_video_handle, MMM_VP_FAST_BACKWARD, cur_step);
        break;

        default:
        dec_result = MMM_CMD_OK;
        break;
    }

    if (dec_result != MMM_CMD_OK)
    {
        //���ִ������ȼ�⿨�Ƿ����
        ret_val = video_check_card_out();

        if (ret_val == RESULT_CARD_OUT_ERROR)
        {
            return ret_val;
        }

        switch (dec_result)
        {

            case MMM_CMD_FILEEND: //���ļ�β
            g_video_change_file = up_video_playnext;
            break;

            case MMM_CMD_FILEHEAD: //���ļ�ͷ
            g_fast_back_end = TRUE;
            //g_video_change_file = up_video_playprev;
            break;

            case MMM_CMD_ERR://�������
            //����������ָ�ʽ����
            g_video_change_file |= up_video_changefile;
            gui_dialog_msg(DIALOG_MSG_V, DIALOG_INFOR_WAIT, S_FORMAT_ERROR);
            g_file_err++;
            break;

            default:
            break;
        }
    }

    //ע�ⲻҪ�ı�������ˢ�±�־λ
    if ((g_video_play_status == VIDEO_FF) || (g_video_play_status == VIDEO_FB))
    {
        if (video_prev_time != (g_video_var.break_point.time / 1000))
        {
            g_video_paint_flag = VIDEO_PAINT_PARTIAL;
            g_display_bitmap |= up_videotimeinfor | up_videoprogress;
        }
    }
    return ret_val;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  play�����ļ���������ʱ��ִ�к���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 * \li  ֻ���ñ�־λ������ѭ��ִ��ˢ������
 *******************************************************************************/
void filename_flag_update(void)
{
    if ((g_video_cur_window == VIDEO_PROGRESSBAR) && (need_scroll_filename == TRUE))
    {
        g_display_bitmap |= up_videofileinfor;
        g_video_paint_flag = 1;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  play���沥��ʱ���ʱ���������ʱ��ִ�к���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 * \li  ֻ���ñ�־λ������ѭ��ִ��ˢ������
 *******************************************************************************/
void play_flag_update(void)
{
    g_display_bitmap |= (up_videoprogress | up_videotimeinfor | up_videoheaderbar);
    g_video_paint_flag = 1;
    if ((g_video_play_status == VIDEO_FB) || (g_video_play_status == VIDEO_FF))
    {
        g_video_timer_counter++;
        if (g_video_timer_counter == VIDEO_FFB_LEVEL_1)
        {
            g_video_speed_control = 2;
            g_display_bitmap |= up_videospeed;
        }
        else if (g_video_timer_counter == VIDEO_FFB_LEVEL_2)
        {
            g_video_speed_control = 3;
            g_display_bitmap |= up_videospeed;
        }
        else
        {
        }
    }
    g_battery_display++;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ȫ����ʱ���ص�����
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 * \li  5s��ʱʱ�䵽����ʱ�����������������ʾ״̬תΪȫ����Ƶ����
 *      ��Ҫֹͣ��ȫ��״̬ˢ�¼�ʱ��
 *******************************************************************************/
void video_full_display(void)
{
    //ֹͣ����ˢ�¼�ʱ��
    g_video_timer_flag |= clr_video_fullscreen_timer;

    if (g_video_play_status == VIDEO_PAUSE)
    {
        if (g_video_cur_window == VIDEO_VOLUMEBAR)
        {
            g_video_cur_window = VIDEO_PROGRESSBAR;
            g_video_paint_flag = VIDEO_PAINT_ALL;
            //���������ͽ�������ʱ��
            g_video_timer_flag |= up_video_filename_timer;
            g_video_timer_flag |= up_video_progress_timer;
        }
    }
    else
    {
        g_video_cur_window = VIDEO_FULLSCREEN;
        g_video_timer_flag |= clr_video_filename_timer;
        g_video_timer_flag |= clr_video_progress_timer;
        //ȫ��ʱֻ�������ͼ����������ʾ
        g_display_bitmap = 0;
        g_display_bitmap |= up_videoBackGDPic;
        g_video_paint_flag = VIDEO_PAINT_PARTIAL;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ⴰ���Ƿ����仯
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 * \li  ���Ҫ��ʾ�Ĵ��ں͵�ǰ���ڲ�һ�£���Ҫ���������Ƶ�м��������ˢ������
 *******************************************************************************/
static void _video_check_window(void)
{
    if (g_video_cur_window != g_video_prev_window)
    {
        //��Ҫ����ȫ����PLAYBAR��VOLUMEBAR���ֽ����ת��
        if (g_video_cur_window == VIDEO_PROGRESSBAR)
        {
            mmm_vp_cmd(g_video_handle, MMM_VP_PROGRESSBAR, (uint32) & g_video_userinfor);
            g_video_timer_flag |= up_video_filename_timer;
            g_video_timer_flag |= up_video_progress_timer;
            g_video_timer_flag |= up_video_fullscreen_timer;
            g_video_paint_flag = VIDEO_PAINT_ALL;
        }
        else if (g_video_cur_window == VIDEO_VOLUMEBAR)
        {
            mmm_vp_cmd(g_video_handle, MMM_VP_VOLUMEBAR, (uint32) & g_video_userinfor);
            if (g_video_prev_window == VIDEO_PROGRESSBAR)
            {
                //��Ҫ��ȥ�����banner,�������ȫ����ʱ��
                //���ڲ���״̬�����VOLUMEBAR-->FULLSCREEN
                //������ͣ״̬�����VOLUMEBAR-->PROGRESSBAR
                g_video_timer_flag |= up_video_fullscreen_timer;
            }
            g_video_timer_flag |= clr_video_filename_timer;
            g_video_timer_flag |= clr_video_progress_timer;
        }
        else
        {
            mmm_vp_cmd(g_video_handle, MMM_VP_NOPROGRESS, (uint32) & g_video_userinfor);
        }
        g_video_prev_window = g_video_cur_window;
    }
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ������ֹͣ��ʱ��
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 * \li  video�ڲ��Ź�������������ʱ��������ѭ���и���g_video_timer_flag��־λ
 *      ���û������Ӧ�ı�־λ
 *******************************************************************************/
void _video_deal_timer(void)
{
    uint8 i;
    int8 timer_id[] =
    { timer_flush_screen, timer_flush_filename, timer_flush_progress }; //bit0,1,2

    for (i = 0; i < 3; i++)
    {
        if ((g_video_timer_flag & (1 << i)) != 0)
        {
            restart_app_timer(timer_id[i]);
            g_video_timer_flag &= (uint8)(~(1 << i));
        }
    }

    for (i = 3; i < 6; i++)
    {
        if ((g_video_timer_flag & (1 << i)) != 0)
        {
            stop_app_timer(timer_id[i - 3]);
            g_video_timer_flag &= (uint8)(~(1 << i));
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video ���ų���������
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       ����ֵ��video_scene_exit()���һ������
 * \ingroup      video_play
 * \note
 *******************************************************************************/
app_result_e video_play(void)
{
    //������Ϣ����
    input_gui_msg_t ui_msg;
    private_msg_t pri_msg;
    decode_status_e dec_status;

    bool result = FALSE;
    bool need_free_mmm = FALSE;
    bool need_play = TRUE;

    app_result_e ui_result = RESULT_NULL;

    while (1)
    {
        if ((g_video_change_file & up_video_changefile) != 0)
        {
            if (video_change_file() == TRUE)
            {
                need_play = TRUE;
            }
            else
            {
                ui_result = RESULT_VIDEO_RETURN;
                need_free_mmm = FALSE;
                goto dec_end;
            }
        }
        if (need_play == TRUE)
        {
            need_play = FALSE;
            ui_clear_screen(NULL);
            g_video_play_status = VIDEO_PLAY;
            g_file_total = g_video_var.path.file_path.dirlocation.file_total;
            dec_status = video_start_decode(&need_free_mmm);
            if (dec_status == DECODE_NO_ERR)
            {
                vfs_get_name(video_mount, g_video_filename, LIST_ITEM_BUFFER_LEN / 2);
                read_filename_to_buffer(g_video_filename, LIST_ITEM_BUFFER_LEN);
                g_file_err = 0;
                g_video_paint_flag = VIDEO_PAINT_ALL;
            }
            else
            {
                //���ִ������ȼ�⿨�Ƿ����
                ui_result = video_check_card_out();

                if (ui_result == RESULT_CARD_OUT_ERROR)
                {
                    goto dec_end;
                }

                if (dec_status == DECODE_FILE_ERR)
                {
                    gui_dialog_msg(DIALOG_MSG_V, DIALOG_INFOR_WAIT, S_FORMAT_ERROR);
                    if (g_file_err >= g_file_total)
                    {
                        ui_result = RESULT_VIDEO_RETURN;
                        goto dec_end;
                    }
                    else
                    {
                        //�л�����
                        g_file_err++;
                        g_video_change_file |= up_video_changefile;
                        continue;
                    }
                }
                else
                {
                    ui_result = RESULT_VIDEO_RETURN;
                    goto dec_end;
                }

            }
        }

        _video_deal_timer();

        //��ȡ�ʹ���gui��Ϣ
        result = get_gui_msg(&ui_msg);
        if (result == TRUE)
        {
            ui_result = video_playing_proc_gui_msg(&ui_msg);
        }
        else
        {
            //����ap˽����Ϣ
            result = get_app_msg(&pri_msg);
            if (result == TRUE)
            {
                ui_result = video_msg_callback(&pri_msg);
                //�ָ�ˢ��ģʽ
                lcd_set_draw_mode(g_video_draw_mode);
            }

        }

        if ((ui_result == RESULT_NULL) || (ui_result == RESULT_REDRAW))
        {
            //�ǲ��Ž�����Ч״̬��Ҫ������״̬
            ui_result = video_check_play_status();

            if (ui_result != RESULT_NULL)
            {
                //������Ч�ļ�,����
                goto dec_end;
            }
        }
        else
        {
            //����Ч״̬��Ҫ�˳����Ž���ѭ��
            //�ر��ļ����
            goto dec_end;
        }

        //��⵱ǰ��״̬������ˢ�±�־�Ͷ�ʱ��
        _video_check_window();

        if (g_video_paint_flag != VIDEO_PAINT_NONE)
        {
            if (g_video_paint_flag == VIDEO_PAINT_PARTIAL)
            {
                //���沿�ָ���
                video_refresh_screen(FALSE);
            }
            else
            {
                video_set_full_flag()
                video_refresh_screen(TRUE);
            }
            g_video_paint_flag = VIDEO_PAINT_NONE;
        }

        sys_os_time_dly(1);
    }

    dec_end: if (need_free_mmm == TRUE)
    {
        video_decoder_free();
    }

    //�ָ�RGBˢ��ģʽ
    lcd_controller_init(RGB_MODE);

    return ui_result;
}

