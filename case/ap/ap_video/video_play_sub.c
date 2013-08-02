/*******************************************************************************
 *                              US212A
 *                            Module: VIDEO_PLAY_SUB
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-2-3 17:00:44           1.0              build this file
 *******************************************************************************/
/*!
 * \file     video_play_sub.c
 * \brief    ��Ƶ������ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2012-2-3
 *******************************************************************************/

#include "video.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    �����Ƶ�ϵ���Ϣ
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void video_clear_breakinfor(void)
{
    g_video_var.break_point.bpbyte = 0;
    g_video_var.break_point.framecnt = 0;
    g_video_var.break_point.time = 0;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    �˳���Ƶ���ţ��ͷ���Ƶ�м����ռ�ÿռ�
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void video_decoder_free(void)
{
    int32 dec_result;

    dec_result = mmm_vp_cmd(g_video_handle, MMM_VP_CLOSE, 0);
    if (dec_result == MMM_CMD_ERR)
    {
        while (1)
        {
            ;//todo
        }
    }
    sys_free_mmm(FALSE);
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ��Ƶ�л��ļ�����
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 *******************************************************************************/
bool video_change_file(void)
{
    bool result = FALSE;
    uint8 *file_name = g_video_var.path.file_path.dirlocation.filename;

    //��ʱ��ϵ�
    video_clear_breakinfor();

    //�ͷ���Ƶ�м���ռ�
    video_decoder_free();

    if ((g_video_change_file & up_video_playprev) == up_video_playprev)
    {
        result = fsel_get_prevfile(file_name);
    }
    else
    {
        result = fsel_get_nextfile(file_name);
    }

    if (result == TRUE)
    {
        fsel_get_location((void *) &g_video_var.path.file_path, g_video_var.path.file_source);
    }
    g_video_change_file = (uint8)(g_video_change_file & clr_video_changefile);
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��Ƶ�ļ���Ϣ����������Ϣ������תģʽ
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \ingroup      video_play
 * \note         
 * \li  ���ļ���ʽ�����򷵻�FALSE�����򷵻�TRUE
 *******************************************************************************/
bool video_get_file_information(void)
{
    bool ret_val = FALSE;
    int32 dec_result;
    mmm_vp_file_info_t fileinfo;

    /* ��ʼ��ͼƬ������*/
    dec_result = mmm_vp_cmd(g_video_handle, MMM_VP_SET_FILE, (uint32) (&g_video_userinfor));
    if (dec_result == MMM_CMD_OK)
    {
        /*��ȡ��Ƶ������Ϣ*/
        dec_result = mmm_vp_cmd(g_video_handle, MMM_VP_GET_MEDIA_INFO, (unsigned int) &fileinfo);

        if (fileinfo.file_len != 0)
        {
            if (fileinfo.type == VIDEO_AMV)
            {
                g_video_draw_mode = DRAW_MODE_V_PIC_DEF;
                g_region_progressbar.y = g_region_adjust.amv_prog_y;
                g_region_volumebar.y = g_region_adjust.amv_vol_y;
            }
            else
            {
                g_video_draw_mode = DRAW_MODE_AVI_DEF;
                g_region_progressbar.y = g_region_adjust.avi_prog_y;
                g_region_volumebar.y = g_region_adjust.avi_vol_y;
            }
            g_total_time = fileinfo.total_time;
            
            //������Ƶˢ��ģʽ
            lcd_set_draw_mode(g_video_draw_mode);
            ret_val = TRUE;
        }

    }
    return ret_val;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ��Ƶ�����ʼ��
 * \param[in]    need_free_mmm  ��Ƶ�м���Ƿ��Ѽ��ر�־������Ѽ��أ��˳�ʱ��ж��
 * \param[out]   void
 * \return       bool
 * \ingroup      video_play
 * \note         
 * \li  ���ļ���ʽ�����򷵻�FALSE�����򷵻�TRUE
 *******************************************************************************/
decode_status_e video_start_decode(bool *need_free_mmm)
{
    int32 dec_result;
#ifdef PC
	return DECODE_NO_ERR;
#endif
    sys_bank_flush(AP_BANK_FRONT_ENHANCED_1);
    sys_bank_flush(AP_BANK_FRONT_ENHANCED_2);
    if (-1 == sys_load_mmm("mmm_vp.al", FALSE))
    {
        *need_free_mmm = FALSE;
        return DECODE_LOAD_ERR;
    }

    //���߳̾��
    dec_result = mmm_vp_cmd(&g_video_handle, MMM_VP_OPEN, 0);
    if (g_video_handle == NULL)
    {
        //���ļ��������
        *need_free_mmm = TRUE;
        return DECODE_OPEN_ERR;
    }

    //�����ļ���Ϣ
    if (FALSE == video_get_file_information())
    { 
        *need_free_mmm = TRUE;
        return DECODE_FILE_ERR;
    }
    else
    {
        g_video_cur_window = VIDEO_PROGRESSBAR;
        g_video_prev_window = VIDEO_FULLSCREEN;
        g_video_timer_flag |= up_video_fullscreen_timer;
        g_video_timer_flag |= up_video_filename_timer;
        g_video_timer_flag |= up_video_progress_timer;
    }
    *need_free_mmm = TRUE;
    return DECODE_NO_ERR;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video ��ȡ˽����Ϣ�Ĵ���
 * \param[in]    msg video �Ĵ���ĸ���˽����Ϣ�Ļص�����
 * \param[out]   none
 * \return       Ӧ�����Ͷ�Ӧ���ļ��������ߵ�̨����
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e video_msg_callback(private_msg_t *pri_msg)
{
    app_result_e result = RESULT_NULL;
    engine_type_e engine_type;
    //msg_reply_t m_reply;
    /*ͬ����Ϣ����*/
    switch (pri_msg->msg.type)
    {
        case MSG_SD_OUT:
        if (g_video_var.path.file_path.dirlocation.disk == DISK_H)
        {
            if(com_get_gui_direction() == GUI_DIRECT_NORMAL)
            {
                gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
            }
            else
            {
                gui_dialog_msg(DIALOG_MSG_V, DIALOG_INFOR_WAIT, S_CARD_OUT);
            }

            //���ڷ��ʿ����ļ����˻ص���Ӧ��
            engine_type = get_engine_type();

            if (engine_type == ENGINE_MUSIC)
            {
                music_close_engine();
            }
            result = RESULT_CARD_OUT_ERROR;
        }
        else
        {
            //���ų�����ע����Ϣ
            if(g_video_scene_next == SCENE_PLAY)
            {
                if(g_video_cur_window != VIDEO_PROGRESSBAR)
                {                    
                    result = RESULT_REDRAW;
                }
                else
                {
                    //ֱ�Ӹ��¿�ͼ��
                    g_video_paint_flag = VIDEO_PAINT_ALL;
                    g_display_bitmap |= up_videoheaderbar;
                }
            }
        }
        break;

        case MSG_SD_IN:
        //���ų�����ע����Ϣ
        if (g_video_scene_next == SCENE_PLAY)
        {
            if(g_video_cur_window != VIDEO_PROGRESSBAR)
            {                
                result = RESULT_REDRAW;
            }
            else
            {
                //ֱ�Ӹ��¿�ͼ��            
                g_video_paint_flag = VIDEO_PAINT_ALL;
                g_display_bitmap |= up_videoheaderbar;
            }

        }
        break;

        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;

        default:
        result = com_message_box(pri_msg->msg.type);
        break;
    }

    if (result == RESULT_REDRAW)
    {
        video_set_full_flag()
        g_video_cur_window = VIDEO_PROGRESSBAR;
        g_video_prev_window = VIDEO_FULLSCREEN;
        //�ػ���棬ǿ������Ϊ����״̬
        g_video_play_status = VIDEO_PLAY;
        //����
        ui_clear_screen(NULL);
    }

    //�ָ�ˢ��ģʽ
    lcd_set_draw_mode(g_video_draw_mode);
    return result;

}


