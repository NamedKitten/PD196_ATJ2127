/*
 *******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_control.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *              leiming    2011-10-27         v1.0                 create this file
 *******************************************************************************
 */
/*include file of this application, ��Ӧ�õ�ͷ�ļ�*/
#include "ap_record.h"


/*������д���ٵ�ʱ���������ͷ�ļ�*/
//#include  <card.h>
//#include  <sysdef.h>

//record bitrate:8000ΪACT�����ʣ�
//32000, 64000, 128000, 192000Ϊmp3������
//512000, 768000, 1024000, 1536000Ϊwav pcm������
static const int rec_bit_rate[11] =
{ 8000, 32000, 64000, 128000, 192000, 256000, 512000, 768000, 1024000, 1536000, -1 };

static const int rec_sample_rate[11] =
{ 8000, 8000, 16000, 32000, 32000, 16000, 16000, 24000, 32000, 48000, -1 };

static const int rec_channel[11] =
{ SINGLE_CH, SINGLE_CH, SINGLE_CH, DUAL_CH, DUAL_CH, SINGLE_CH, DUAL_CH, DUAL_CH, DUAL_CH, DUAL_CH, -1 };

extern void _scene_record_proc_timer(void);

/******************************************************************************/
/*!
 * \par  Description:
 * \static app_result_e _recording_set_audio_param(void)
 * \����wav��ʽ¼����audio����
 * \param[in]    void
 * \param[out]   none
 * \return       int result
 * \retval       none
 * \ingroup      record_control.c
 * \note
 */
/*******************************************************************************/
static app_result_e _recording_set_audio_param(void)
{
    int mr_ret;

    //����audio param����
    if (g_record_vars.file_type == REC_FORMAT_WAV)
    {
        g_mr_param.mr_audio_param.encode_mode = g_record_vars.encode_type; //����ADPCM����PCM
    }
    else if (g_record_vars.file_type == REC_FORMAT_MP3)
    {
        g_mr_param.mr_audio_param.encode_mode = MP3;
    }
    else
    {
        g_mr_param.mr_audio_param.encode_mode = ACT;
    }

    if ((g_record_vars.ai_source == AI_SOURCE_MIC) || (g_record_vars.ai_source == AI_SOURCE_I2S))
    {
        if (g_record_vars.avr_mode == AVR_ON)
        {
            g_mr_param.mr_audio_param.vad_mode = 1; //ģʽ:����¼��
            g_mr_param.mr_audio_param.vad_delay = g_record_vars.track_delay * 1000; //set avr delay
            g_mr_param.mr_audio_param.vad_threshold = g_record_vars.track_level_avr; //set avr level
        }
        else//AVRʱ���رշ���
        {
            g_mr_param.mr_audio_param.vad_mode = 0; //ģʽ:����¼��
            g_mr_param.mr_audio_param.vad_delay = 0; //avr delay set 0
        }

        Codec_set_brat((uint8)g_record_vars.bitrate);
        Codec_set_adcgain(2);//test code
        
    }
    else
    {
        if (g_record_vars.track_mode == TRACK_ON)
        {
            g_mr_param.mr_audio_param.vad_mode = 2; //¼��ģʽ:�Զ�����
            g_mr_param.mr_audio_param.vad_delay = g_record_vars.track_delay * 1000; //avr delay set 0
            g_mr_param.mr_audio_param.vad_threshold = g_record_vars.track_level_breaksong;
        }
        else //AVRʱ���رշ���
        {
            g_mr_param.mr_audio_param.vad_delay = 0;
            g_mr_param.mr_audio_param.vad_mode = 0; //¼��ģʽ:����¼��
        }
    }

    g_mr_param.mr_audio_param.bitrate = rec_bit_rate[g_record_vars.bitrate]; //set record bitrate
    g_mr_param.mr_audio_param.sample_rate = rec_sample_rate[g_record_vars.bitrate]; //set record sample rate
    g_mr_param.mr_audio_param.channel = rec_channel[g_record_vars.bitrate]; //set record channel num

    //����¼��������/ͨ����/������
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_AUDIO_PARAM, (unsigned int) &g_mr_param.mr_audio_param);
    if (mr_ret == -1)
    {
        return RESULT_SCENE_EXIT; //�˳�����
    }

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _recording_start(void)
 * \��ʼ¼��
 * \param[in]    void
 * \param[out]   none
 * \return       int result
 * \retval       none
 * \ingroup      record_control.c
 * \note
 */
/*******************************************************************************/
app_result_e _recording_start(void)
{
    int mr_ret;
    app_result_e app_ret;
    mmm_mr_fs_para_t fs_para; //codec fs param

    //open recording file
    mr_ret = (int) mmm_mr_cmd(&g_mr_param.mr_handle, MMM_MR_OPEN, 0);
    if (mr_ret == -1)
    {
        return RESULT_SCENE_EXIT; //�˳�����
    }
    
    Codec_init(0);

    //�ȹرհ�����
    if (g_entry_mode != PARAM_FROM_RADIO)
    {
        com_set_sound_record(TRUE, 0);
    }
    else
    {
        com_set_sound_record(TRUE, 1);
    }

    g_mr_param.mr_ain_setting.digital_gain = 0;
    g_mr_param.mr_ain_setting.listening_flag = 0;

    //����������
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_AIN_OPEN, (unsigned int) &g_mr_param.mr_ain_setting);
    if (mr_ret == -1)
    {
        //�ر�¼���м��
        mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_CLOSE, 0);
        return RESULT_SCENE_EXIT; //�˳�����
    }

    //����¼���ļ�
    fs_para.fs_type = 0; /*! 0: vfs, 1: sd file system */
    fs_para.file_mount_id = (void*) g_fs_param.vfs_handle;
#if 1
    fs_para.file_handle = (void*) g_fs_param.fhandle;
#endif
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_SET_FILE, (unsigned int) &fs_para); //����¼���ļ�

    if (mr_ret == -1)
    {
        //�ر���������
        mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_AIN_CLOSE, 0);
        //�ر�¼���м��
        mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_CLOSE, 0);
        return RESULT_SCENE_EXIT; //�˳�����
    }

    //set (mp3/wav) audio param
    app_ret = _recording_set_audio_param();
    if (app_ret == RESULT_SCENE_EXIT)
    {
        _recording_close(); //clear file,close ain,close record
        return RESULT_SCENE_EXIT; //�˳�����
    }

    //��ACT��ʽ��¼��ԴΪMICʱ��������
    if ((g_record_vars.ai_source == 0) && (g_record_vars.file_type != REC_FORMAT_ACT))
    {
        //�趨����ȼ�
        mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_SET_DENOISE, g_record_vars.noise_cancel_level);
    }

    //start record
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_START, 0);
    if (mr_ret == -1)
    {
        _recording_close(); //clear file,close ain,close record
        return RESULT_SCENE_EXIT; //�˳�����
    }

    g_scene_param.record_status = REC_PLAY_STATUS;
    //scene timer init
    g_scene_param.timer_id = set_app_timer(APP_TIMER_ATTRB_UI, REC_SCENE_TIMER_INTERVAL,
            (timer_proc) _scene_record_proc_timer);
    if (g_scene_param.timer_id == -1)
    {
        return RESULT_SCENE_EXIT;
    }

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _recording_pause(void)
 * \¼��pause����
 * \param[in]    void
 * \param[out]   none
 * \return       int
 * \retval       error
 * \ingroup      record_control.c
 * \note
 */
/*******************************************************************************/
app_result_e _recording_pause(void)
{
    int mr_ret;

    //stop timer
    stop_app_timer(g_scene_param.timer_id);

    //ȡ��ǰ¼��ʱ��
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_GET_STATUS, (unsigned int) &g_mr_param.mr_status); //ȡ������ʱ��

    //����pause����
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_PAUSE, 0);
    if (mr_ret == -1)
    {
        //return error
        return RESULT_SCENE_EXIT; //�˳�����
    }

    g_scene_param.record_status = REC_PAUSE_STATUS;

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _recording_resume(void)
 * \¼��resume����
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       none
 * \ingroup      ap_record_control.c
 * \note
 */
/*******************************************************************************/
app_result_e _recording_resume(void)
{
    int mr_ret;

    //����record resume
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_RESUME, 0);
    if (mr_ret == -1)
    {
        return RESULT_SCENE_EXIT; //�˳�����
    }

    //restart record timer
    if (!restart_app_timer(g_scene_param.timer_id))
    {
        return RESULT_ERROR;
    }

    g_scene_param.record_status = REC_PLAY_STATUS;
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _recording_close(void)
 * \�ر�¼��
 * \param[in]    void
 * \param[out]   none
 * \return       app_result_e
 * \retval       none
 * \ingroup      record_control.c
 * \note
 */
/*******************************************************************************/
app_result_e _recording_close(void)
{
    bool ret;
    uint32 save_freq;

    save_freq = sys_adjust_clk(FREQ_108M, 0);
    //�ر�¼���ļ�
    mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_CLEAR_FILE, 0);
    if (vfs_file_close(g_fs_param.vfs_handle, g_fs_param.fhandle) == 0)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
    }

    //�ر���������
    mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_AIN_CLOSE, 0);

    //�ر�¼���м��
    mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_CLOSE, 0);

    sys_adjust_clk(save_freq, 0);

    //�ж��Ƿ�wav¼����wav¼������д�ļ�ͷ
    if ((g_record_vars.file_type == REC_FORMAT_WAV) || (g_record_vars.file_type == REC_FORMAT_ACT))
    {
        ret = _record_write_back_filehead();
        if (ret == FALSE)
        {
            return RESULT_MAIN_APP;
        }
    }

    //��vm�еĵ����д��־���
    sys_vm_read(&g_writehead, VM_AP_RECORD_WAV, sizeof(record_writehead_t));
    g_writehead.rewrite_need = FALSE;
    sys_vm_write(&g_writehead, VM_AP_RECORD_WAV);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _recording_stop(void)
 * \�ر�¼��
 * \param[in]    void
 * \param[out]   none
 * \return       app_result_e
 * \retval       none
 * \ingroup      record_control.c
 * \note
 */
/*******************************************************************************/
app_result_e _recording_stop(void)
{
    int mr_ret;
    /*������д���ٲο�*/
    /*
     if(DISK_H == g_record_vars.path.file_path.dirlocation.disk)
     {
     card_update();
     g_card_para = 0x07;
     }
     */
    /* kill timer */
    if (g_scene_param.timer_id != -1)
    {
        kill_app_timer(g_scene_param.timer_id);
        g_scene_param.timer_id = -1;
    }

    g_scene_param.record_status = REC_STOP_STATUS;

    //¼��״̬�£���ֹͣ¼��
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_STOP, 0);
    if (mr_ret < 0)
    {
        return RESULT_SCENE_EXIT;
    }

    //�ָ�������
    if (g_entry_mode != PARAM_FROM_RADIO)
    {
        com_set_sound_record(FALSE, 0);
    }
    else
    {
        com_set_sound_record(FALSE, 1);
    }

    //��ȡ¼��ʱ��
    /*��ȡ�м��״̬*/
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_GET_STATUS, (unsigned int) &g_mr_param.mr_status);

    if (mr_ret < 0)
    {
        return RESULT_ERROR;
    }
    
    return RESULT_NULL;
}

