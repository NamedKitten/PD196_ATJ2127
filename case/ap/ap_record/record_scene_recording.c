/*******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_paint.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      leiming       2011-10-27          v1.0
*******************************************************************************/
/*include file of this application, ��Ӧ�õ�ͷ�ļ�*/
#include "ap_record.h"


/******************************************************************************/
/*!
 * \par  Description:
 * \bool record_auto_start(void)
 * \�Զ���ʼ¼��
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval
 * \retval       ......
 * \ingroup      record_scene_recording.c
 * \note
 */
/*******************************************************************************/
bool record_auto_start(void)
{
    //ֹͣ��ǰ¼��
    _recording_stop();

    //�رյ�ǰ¼��
    _recording_close();

    g_scene_param.paint_flag = TRUE;
    rec_free_time = g_scene_param.free_time;
    SetFullInterface()

    //�ر��м��
    sys_free_mmm(0);

    //�Զ������ļ���Ŀ��1
    g_file_total++;
    
    //��ʼ��һ��¼��
    /*set record path , check disk info, check filenum, create filename*/
    if (!_record_file_setup())
    {
        return FALSE;
    }

    /*variables init*/
    rec_free_space = g_fs_param.free_space; //����¼����ʼǰ��free space
    rec_free_time = g_scene_param.free_time; //����¼����ʼǰ��free time
    g_scene_param.record_result = RESULT_NULL;
    rewrite_times = 0;

    sys_bank_flush(AP_BANK_FRONT_ENHANCED_1);
    sys_bank_flush(AP_BANK_FRONT_ENHANCED_2);

    /*����¼��codec�����*/
#ifndef PC
    if (-1 == sys_load_mmm("mmm_mr.al", 0))
    {
        return FALSE;
    }
#endif

    //g_mr_param.mr_status.status = MMM_MR_ENGINE_STOPPED;//���Ƿ�ȥ��
    _recording_start();

    return TRUE;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \void alarm_fm_record_proc(void)
 * \��ʱ¼����ʱ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \retval       ......
 * \ingroup      record_scene_recording.c
 * \note
 */
/*******************************************************************************/
void alarm_fm_record_proc(void)
{
    alarm_rec_time--;
    if (alarm_rec_time == 0)
    {
        g_record_timeout = TRUE;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \static void _check_disk_info(void)
 * \���¼��״̬�������Ƿ�����¼���ļ��Ƿ񳬹�����
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       none
 * \retval       ......
 * \ingroup      record_scene_recording.c
 * \note
 */
/*******************************************************************************/
static void _check_disk_info(void)
{
    int result;

#if 0
    result = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_SINGLE_THREAD, 0);
#endif

    /*��ȡ�м��״̬*/
    result = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_GET_STATUS, (unsigned int) &g_mr_param.mr_status);
    if (result < 0)
    {
        goto check_disk_exit;
    }

    if (MMM_MR_ENGINE_ERROR == g_mr_param.mr_status.status)
    {
        if (REC_PLAY_STATUS == g_scene_param.record_status)
        {
            _recording_stop();
            _recording_close();
        }
        if (DISK_H == g_record_vars.path.file_path.dirlocation.disk)
        {
            if (sys_detect_disk(DRV_GROUP_STG_CARD) != 0)
            {
                //�޿���ʾ
                gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
                fsel_exit();
                g_record_vars.path.file_path.dirlocation.disk = DISK_C;//�Զ��л���flash
                need_fsel_init = TRUE;
            }
        }

        if(g_entry_mode == PARAM_FROM_MAINMENU)
        {
            g_scene_param.record_result = RESULT_RECORD_MEMU;
        }
        else if(g_entry_mode == PARAM_FROM_RADIO)
        {
            g_scene_param.record_result = RESULT_RADIO_PLAY;
        }
        else
        {
            g_scene_param.record_result = RESULT_RECORD_MEMU;    
        }
        goto check_disk_exit;
    }

    /*��ȡ���̿ռ���Ϣ*/
    g_fs_param.free_space = rec_free_space - (uint32) g_mr_param.mr_status.write_pages;
    g_scene_param.free_time = rec_free_time - (uint32) (g_mr_param.mr_status.time / 1000); /*codec���ص�ʱ�䵥λ��ms*/

    //if (g_fs_param.free_space <= 0)
    if ((g_scene_param.free_time <= 0) || (g_fs_param.free_space <= 0))
    {
        /*¼�ƹ����У��ռ䲻�㣬��ʾ������*/
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, STR_DISK_FULL);

        if (REC_PLAY_STATUS == g_scene_param.record_status)
        {
            _recording_stop();
            _recording_close();
            g_scene_param.record_result = RESULT_SCENE_EXIT;
        }

        /*��ʾ�ļ��ѱ���*/
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, SAVED2);

        //ȷ�ϱ���¼���ļ����ȹر�¼��
        //_recording_close();

        if (g_entry_mode == PARAM_FROM_MAINMENU)
        {
            //�����desktop����ap����¼��һ�׺󣬷��ص�menu����
            g_scene_param.record_result = RESULT_RECORD_MEMU;
        }
        else if (g_entry_mode == PARAM_FROM_RADIO)
        {
            //�����fm����ap�����˳�Ӧ�÷��ص�fm
            //g_scene_param.record_result = RESULT_SCENE_EXIT;
            g_scene_param.record_result = RESULT_RADIO_PLAY;
        }
        else
        {
            ;//qac need
        }
        goto check_disk_exit;
    }

    //�Զ�����
    if (MMM_MR_ENGINE_STOPPED == g_mr_param.mr_status.status)
    {
        if ((g_record_vars.ai_source == AI_SOURCE_LINEIN) && (g_record_vars.track_mode == TRACK_ON))
        {
            record_auto_start();
            goto check_disk_exit;
        }
    }

    if (REC_FORMAT_WAV == g_record_vars.file_type)
    {
        //wav�ļ�����2G�Զ��л��ļ�����
        if (g_mr_param.mr_status.write_pages >= WAV_FILE_SIZE_MAX)
        {
            record_auto_start();
            goto check_disk_exit;
        }
    }
    else
    {
        //mp3�ļ�����2G�Զ��л��ļ�����
        if (g_mr_param.mr_status.write_pages >= MP3_FILE_SIZE_MAX)
        {
            record_auto_start();
            goto check_disk_exit;
        }
    }
    check_disk_exit: return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _mktime(uint32 time_sec,time_timebox_t *time_buf)
 * \��֯ʱ�����ݽṹ
 * \param[in]    time_sec ʱ����ֵ  para1
 * \param[in]    time_buf ���ʱ��ṹ��buffer
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      recoed_scene_recording.c
 * \note
 */
/*******************************************************************************/
void _mktime(uint32 time_sec, time_timebox_t *time_buf)
{
    time_buf->hour = (uint16)(time_sec / 3600);
    time_buf->minute = (uint8)((time_sec % 3600) / 60);
    time_buf->second = (uint8)(time_sec % 60);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \��������ʾ
 * \param[in]    pdisplay_bitmap ˢ�±�־λ
 * \param[in]    para TRUE ��ʼ�� FALSE ����ˢ��
 * \param[in]    type 0 timebox 1 progressbar
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      recoed_scene_recording.c
 * \note
 */
/*******************************************************************************/
void _record_show_time_progress(uint32 *pdisplay_bitmap, uint8 para, uint8 type)
{
    uint8 draw_mode;
    style_infor_t style;
    timebox_private_t timebox_data;
    progressbar_private_t progressbar_data;

    style.type = UI_AP;

    //��ʾtimebox
    if (type == 0)
    {
        style.style_id = STY_RECORD_TIME;

        timebox_data.times[0].time_type = 'T';
        timebox_data.times[1].time_type = 'T';

        _mktime((uint32)(g_mr_param.mr_status.time / 1000), &(timebox_data.times[0].union_time.time));

        _mktime(g_scene_param.free_time, &(timebox_data.times[1].union_time.time));

        if (timebox_data.times[0].union_time.time.hour != 0)
        {
            timebox_data.times[0].mode = TIME_DISP_HHMMSS;
        }
        else
        {
            timebox_data.times[0].mode = TIME_DISP_MMSS;
        }

        if (timebox_data.times[1].union_time.time.hour != 0)
        {
            timebox_data.times[1].mode = TIME_DISP_HHMMSS;
        }
        else
        {
            timebox_data.times[1].mode = TIME_DISP_MMSS;
        }

        if(para == TRUE)
        {
            draw_mode = TIMEBOX_DRAW_ALL;
        }
        else
        {
            draw_mode = TIMEBOX_DRAW_TIME;
        }
        ui_show_timebox((void*) &style, (void*) &timebox_data, draw_mode);

        /*����ʾ��־*/
        *pdisplay_bitmap &= clr_recordTime;
    }
    else
    {
        style.style_id = STY_RECORD_PROGRESS;

        if (rec_free_time > 0xffff)
        {
            progressbar_data.total = (rec_free_time / 1000);
            progressbar_data.value = (uint32)(g_mr_param.mr_status.time / 1000 / 1000);
        }
        else
        {
            progressbar_data.total = (rec_free_time);
            progressbar_data.value = (uint32)(g_mr_param.mr_status.time / 1000);
        }

        if(para == TRUE)
        {
            draw_mode = PROGRESS_DRAW_ALL;
        }
        else
        {
            draw_mode = PROGRESS_DRAW_PROGRESS;
        }

        ui_show_progressbar((void*) &style, (void*) &progressbar_data, draw_mode);

        /*����ʾ��־*/
        *pdisplay_bitmap &= clr_recordProgress;
    }
}


/******************************************************************************/
/*!
 * \par  Description:
 * \void _scene_record_proc_timer(void)
 * \record timer������,ˢ��ʱ����ʾ�����¼��������Ϣ
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       none
 * \retval       ......
 * \ingroup      record_scene_recording.c
 * \note
 */
/*******************************************************************************/
void _scene_record_proc_timer(void)
{
    uint32 temp_val;

    //����¼��ʱ��ˢ�±�־
    if (get_screensave_state() == FALSE)
    {
        //g_scene_param.paint_flag = TRUE;
        g_scene_param.display_bitmap |= up_recordTime;
        g_scene_param.display_bitmap |= up_recordProgress;
    }

    //��������Ϣ
    _check_disk_info();

    //¼���ϵ籣��
    {
        rewrite_times++;
        if (rewrite_times >= WAV_WRITE_BACK_TIMES)
        {
            temp_val = (uint32) g_mr_param.mr_status.time / 1000 / 60; //¼��ʱ��ת��Ϊ����
            if (temp_val >= (uint32) (256 * vram_wr_cnt))
            {
                //����RTC_ACCESS�Ĵ���ֻ��8bit���ã�1���Ӽ�1λ�����Ҳֻ�ܼ�¼255���ӣ����Գ���255��дһ��vram
                //¼������дvramʱ��ϳ����ᵼ��¼����������˾�������дvram�Ĵ���
                vram_wr_cnt++;
                g_writehead.file_bytes = (uint32) g_mr_param.mr_status.write_pages * 512;
                sys_vm_write(&g_writehead, VM_AP_RECORD_WAV);
            }

            //RTC_ACCESS�Ĵ�����¼¼���ķ�����
            temp_val = temp_val % 256;
            act_writel(temp_val, RTC_ACCESS);
            act_writel(0xA596, RTC_REGUPDATA);
            while (act_readl(RTC_REGUPDATA) != 0x5A69)
            {
                ;//do nothing
            }

            rewrite_times = 0;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e record_scene_recording(void)
 * \record����������
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       ......
 * \ingroup      record_scene_recording.c
 * \note
 */
/*******************************************************************************/
app_result_e record_scene_recording(void)
{
    bool bl_ret;
    app_result_e app_ret = RESULT_NULL;

    bl_ret = _scene_recording_init();
    g_scene_param.paint_flag = TRUE;
    rec_free_time = g_scene_param.free_time;
    SetFullInterface()

    if (bl_ret == FALSE)
    {
        if (g_entry_mode == PARAM_FROM_RADIO)
        {
            //�����fm����ap�����˳�Ӧ�÷��ص�fm
            return RESULT_RADIO_PLAY;
        }
        return RESULT_RECORD_MEMU;
    }

    change_app_state(APP_STATE_PLAYING);
    if(g_entry_mode == PARAM_FROM_RADIO)
    {
        change_app_state(APP_STATE_PLAYING_ALWAYS);
    }
    while (1)
    {
        if (g_scene_param.paint_flag == TRUE)
        {
            /*���Ƴ�������,����g_display_bitmap�ж���ı�־λ��ˢ��ͼ��*/
            _record_scene_paint(&g_scene_param.display_bitmap);
            g_scene_param.paint_flag = FALSE;
        }

        if((g_scene_param.display_bitmap & up_recordTime) != 0)
        {
            _record_show_time_progress(&(g_scene_param.display_bitmap), FALSE, 0);
        }

        if((g_scene_param.display_bitmap & up_recordProgress) != 0)
        {
            _record_show_time_progress(&(g_scene_param.display_bitmap), FALSE, 1);
        }

        app_ret = _scene_record_get_msg();

        //������g_scene_param.record_result != RESULT_NULL���˳�ѭ��
        if (g_scene_param.record_result != RESULT_NULL)
        {
            app_ret = g_scene_param.record_result;
        }

        //��Ϣ����Ĵ���
        if (app_ret == RESULT_REDRAW)
        {
            g_scene_param.paint_flag = TRUE;
            SetFullInterface()
            if (g_scene_param.record_status == REC_PAUSE_STATUS)
            {
                g_scene_param.display_bitmap &= clr_recordPlayIcon;
                g_scene_param.display_bitmap |= up_recordPauseIcon;
            }
        }
        else if (app_ret != RESULT_NULL)
        {
            break;
        }
        else
        {
            ; //QAC need
        }

        //��ʱ¼����ʱ���ж�
        if (g_record_timeout == TRUE)
        {
            app_ret = RESULT_ALARM_QUIT;
            break;
        }
        /*����10ms�������񽻻�*/
        sys_os_time_dly(1);
    }/*end of while*/

    /*���ó����˳�����*/
    _scene_recording_exit();
    change_app_state(APP_STATE_NO_PLAY);

    return app_ret;
}

