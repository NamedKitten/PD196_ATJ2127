/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2012-03-02        1.0              create this file
 *******************************************************************************/

#include "app_mengine.h"

static mmm_mp_status_t mmm_status _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _mengine_start_sound(uint8 cur_volume)
 * \��ʼ����ʱ����������ص�����
 * \param[in]    cur_volume
 * \param[out]   none
 * \return       bool
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
static bool _mengine_start_sound(uint8 cur_volume)
{
    //param1 audio codec, param2 sample rate
    mengine_info.eg_config.volume = cur_volume;
    enable_dac(10, 44);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _mengine_stop_sound(uint8 cur_volume)
 * \ֹͣ����ʱ����������ص�����
 * \param[in]    cur_volume
 * \param[out]   none
 * \return       bool
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
static bool _mengine_stop_sound(void)
{
    disable_dac();
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _mengine_check_sd_in(void)
 * \���Ϊ���̲����ļ�����⿨�Ƿ����
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval       1 ����
 * \retval       0 ������
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
static bool _mengine_check_sd_in(void)
{
    bool bret = TRUE;
    if (g_eg_cfg_p->location.dirlocation.disk == DISK_H)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            bret = FALSE;
        }
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _get_file_info(uint8 normal_file)
 * \�����ļ�����ȡ�ļ���Ϣ
 * \param[in]    normal_file ����ͨmusic�ļ�����SD���ļ�(֧���������ֲ���)
 * \param[out]   none
 * \return       int �����ֵ
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
static int _get_file_info(uint8 normal_file)
{
    int result;

    mmm_mp_fs_para_t set_file_param;

    if (normal_file == TRUE)
    {
        //normal file
        set_file_param.fs_type = 0;
        set_file_param.file_name = NULL;
        //��������
#ifdef PC
        //mengine_config_t *eg_cfg = &mengine_info.eg_config;
        set_file_param.file_name = mengine_info.eg_config.location.plist_location.filename;
#endif
    }
    else
    {
        //sd file
        set_file_param.fs_type = 1;
        set_file_param.file_name = (char*) g_file_name;
    }

    //�̷�
    set_file_param.file_mount_id = (void*) file_sys_id;

    //����set file����
    result = mmm_mp_cmd(mp_handle, MMM_MP_SET_FILE, (unsigned int) &set_file_param);

    if (result == 0)
    {
        //��ȡ�ļ���ʽ����ʱ�䡢�����ʵ���Ϣ
        result = mmm_mp_cmd(mp_handle, MMM_MP_MEDIA_INFO, (unsigned int) &mengine_info.eg_file_info);
        Codec_set_sample((uint16)mengine_info.eg_file_info.sample_rate);
//        Codec_set_dacgain(2);//test code
    }

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _file_switch_info(void)
 * \�л���һ�����ļ���Ϣ��ȡ
 * \param[in]    play_mode_e para1
 * \param[in]    last_mode:

 * \param[out]   none
 * \return       the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
bool _file_switch_info(void)
{
    //��ȡ�ļ���Ϣ���粥����ʱ��
    _set_file();

    //ͨ��SET_FILE���������Ƿ���ȷ������FILE_SWITCH_INFO,�����Ĵ����
    //��Ҫ��������
    //if(g_eg_status_p->err_status != EG_ERR_NONE)
    //{
    g_eg_status_p->err_status = EG_ERR_NONE;
    //}

    //�ر��ļ�
    mmm_mp_cmd(mp_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);

    if (g_is_audible == TRUE)
    {
        _check_pos_time();
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _set_file(void)
 * \���ò����ļ���������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
bool _set_file(void)
{
    int result;

    //���ȼ�⿨�ļ��Ƿ����
    if (_mengine_check_sd_in() == FALSE)
    {
        return FALSE;
    }

    //�ļ�ϵͳ����id
    if (g_eg_cfg_p->fsel_type == FSEL_TYPE_SDFILE)
    {
        result = _get_file_info(FALSE);
    }
    else
    {
        //�������ʱ�־
        g_eg_playinfo_p->cur_lyric = 0;
        if (g_is_audible == FALSE)
        {
            vfs_get_name(file_sys_id, g_file_name, 32);
            g_eg_playinfo_p->cur_lyric = lyric_open(g_file_name);
        }
        result = _get_file_info(TRUE);
    }

    if (result != 0)
    {
        mengine_save_errno_no();
        return FALSE;
    }

    return TRUE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _play(play_mode_e play_mode)
 * \��ʼ��������
 * \param[in]    play_mode ����ģʽ para1
 * \param[in]    play_mode_e:
 *							 \PLAY_NORMAL ������ʼ����
 *							 \PLAY_RESUME �ϵ�����
 *							 \PLAY_FFON   �������
 *							 \PLAY_FBON   ���˲���
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 * \li   ע������ļ�SET_FILEʧ�ܣ����ﲢ��ֱ�ӹرվ��,������ͨ��״̬����ȡ����
 �ţ�Ȼ���ٹرվ�������ֱ�ӵ���CLEAR_FILE�������������
 */
/*******************************************************************************/
bool _play(play_mode_e play_mode)
{
    int result;

    bool bret = TRUE;

    //ֹͣ or ��ͣʱ ���Ͳ�������
    if ((mengine_info.eg_status.play_status == StopSta) || (mengine_info.eg_status.play_status == PauseSta))
    {
        Codec_init(0);
        bret = _set_file();
        if (bret == FALSE)
        {
            goto end_play;
        }

        com_set_sound_out(TRUE, SOUND_OUT_RESUME, _mengine_start_sound);

        //�п��ܴӲ�֧�ֲ��ŵ������л���֧�ֲ��ŵ����͵�EQ,�����Ҫ����
        mengine_set_real_eq(TRUE);

        //for audible
        if (g_is_audible == TRUE)
        {
            libc_memcpy(g_send_audible_p->playbackPosFilebuffer, g_audible_pos, sizeof(g_audible_pos));
            _deal_audible_sysfile();
        }

        if (play_mode == PLAY_FFON)
        {
            //�������
            result = mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int) 4);
            mengine_info.eg_status.play_status = PlayFast;//���ò���״̬

        }
        else if (play_mode == PLAY_FBON)
        {
            //���˲���
            mengine_info.eg_config.bk_infor.bp_time_offset = -5000;
            mengine_info.eg_config.bk_infor.bp_file_offset = 0;
            mengine_info.eg_config.bk_infor.bp_info_ext = 0;
            mmm_mp_cmd(mp_handle, MMM_MP_SET_BREAK_POINT, (uint32)(&(mengine_info.eg_config.bk_infor)));

            result = mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int) -4);
            mengine_info.eg_status.play_status = PlayFast;//���ò���״̬

        }
        else if (play_mode == PLAY_RESUME)
        {
            //�ϵ�����,���öϵ�
            mmm_mp_cmd(mp_handle, MMM_MP_SET_BREAK_POINT, (uint32)(&(mengine_info.eg_config.bk_infor)));
            mengine_info.eg_status.play_status = PlaySta;//���ò���״̬
        }
        else
        {
            mengine_info.eg_status.play_status = PlaySta;//���ò���״̬
        }
        
        //���Ͳ�������
        result = mmm_mp_cmd(mp_handle, MMM_MP_PLAY, (uint32) NULL);
        if (result != 0)
        {
            bret = FALSE;
        }
        else
        {
                        
            if (g_is_audible == TRUE)
            {
                _set_chapter_timer();
                _set_imag_timer();
                if (mengine_info.eg_config.section_mod == SWITCH_SECTION)
                {
                    _check_audible_chapter();
                }
            }
            else
            {
                if (libc_memcmp(mengine_info.eg_config.location.dirlocation.filename, "AAC", 4) == 0)
                {
                    //��ȡ�ļ���ʽ����ʱ�䡢�����ʵ���Ϣ
                    result = mmm_mp_cmd(mp_handle, MMM_MP_MEDIA_INFO, (uint32) & mengine_info.eg_file_info);
                }
            }
            bret = TRUE;
        }    
                
        //�������,���ü�ʱ��ʼʱ��
        if (mengine_info.eg_config.repeat_mode == FSEL_MODE_INTRO)
        {            
            mmm_mp_cmd(mp_handle, MMM_MP_GET_STATUS, (unsigned int) &mmm_status);
            g_intro_start = (uint32) mmm_status.cur_time;
        }        
       
    }
    end_play: if (bret == FALSE)
    {
        mengine_save_errno_no();
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _stop(stop_mode_e stop_mode)
 * \ֹͣ��������
 * \param[in]    stop_mode ֹͣģʽ para1
 * \param[in]    stop_mode_e:
 * 							 \STOP_NORMAL ����ֹͣ
 * 							 \STOP_PAUSE  ��ͣ����
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
bool _stop(stop_mode_e stop_mode)
{
    bool bret = TRUE;
    int result = 0;

    //mengine_config_t *cfg_ptr = &mengine_info.eg_config;
    mengine_status_t *eg_status = &mengine_info.eg_status;

    //ȡ������� & ab���� �ص���������״̬
    switch (eg_status->play_status)
    {
        case PlayAB://ab����
        if (eg_status->ab_status == PlayAB_Do)
        {
            mmm_mp_cmd(mp_handle, MMM_MP_CLEAR_AB, (unsigned int) NULL);
        }
        eg_status->ab_status = PlayAB_Null;
        eg_status->play_status = PlaySta;
        break;

        case PlayFast://�����
        mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int) 0);
        eg_status->play_status = PlaySta;
        break;

        default:
        break;
    }
    //�������ŲŻ�ֹͣ
    if (eg_status->play_status == PlaySta)
    {
        //����ϵ�
        mmm_mp_cmd(mp_handle, MMM_MP_GET_BREAK_POINT, (unsigned int) &(mengine_info.eg_config.bk_infor));
        //ֹͣ����
        result = mmm_mp_cmd(mp_handle, MMM_MP_STOP, (unsigned int) NULL);
        if (result != 0)
        {
            bret = FALSE;
        }
        else
        {
            //��ͣ����
            if (stop_mode == STOP_PAUSE)
            {
                eg_status->play_status = PauseSta;
            }
            else
            {
                //����ֹͣ
                eg_status->play_status = StopSta;
                mengine_info.eg_config.file_cnt.num = 1;
            }
            bret = TRUE;
        }
        //for audible file
        if (g_is_audible == TRUE)
        {
            _deal_audible_posfile(1);
            _clr_chapter_timer();
            _clr_imag_timer();
        }

        if (bret == FALSE)
        {
            mengine_save_errno_no();
        }

        //�ر���Ƶ���
        com_set_sound_out(FALSE, SOUND_OUT_PAUSE, _mengine_stop_sound);
    }

    //ֹͣ�ر��ļ�
    mmm_mp_cmd(mp_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);

    fsel_set_mode(mengine_info.eg_config.repeat_mode | mengine_info.eg_config.shuffle_flag);//����ѭ��ģʽ

//    Codec_exit();

    return bret;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool _next_or_prev(switch_mode_e switch_mode)
 * \�л�������
 * \param[in]    switch_mode �л�ģʽ
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
static bool _next_or_prev(switch_mode_e switch_mode)
{
    uint8 loop_mode;
    bool bret = TRUE;

    uint8 direct = (uint8)(switch_mode & 0x01);
    uint8 force_mod = (uint8)((switch_mode & 0x02) >> 1);

    mengine_config_t *cfg_ptr = &mengine_info.eg_config;

    mengine_info.eg_status.err_status = EG_ERR_NONE;

    loop_mode = cfg_ptr->shuffle_flag;//ѭ��ģʽshuffle

    //ǿ���л�
    if (force_mod == TRUE)
    {
        //loop_mode = cfg_ptr->shuffle_flag;//ѭ��ģʽ����Ϊȫ��ѭ��
        loop_mode |= FSEL_MODE_LOOPALL;
        fsel_set_mode(loop_mode);
    }

    if (g_change_path_flag == 1)
    {
        //�Ƿ����·������
        bret = change_locat_deal();
    }

    //����и���location���Ƿ�ɹ�
    if (bret == FALSE)
    {
        goto _switch_end;
    }

    if (direct == 0)
    {
        if ((g_del_curfile == 1) && (cfg_ptr->fsel_type <= FSEL_TYPE_DISKSEQUNCE))
        {
            //g_del_curfile = 0;
            //Ŀ¼ģʽ��Ҫ����ɨ�����
            fsel_set_typebit(file_type_cfg, FALSE);
        }

        if (g_del_curfile == 1)
        {
            cfg_ptr->location.flist_location.file_num++;
            if (cfg_ptr->location.flist_location.file_num > cfg_ptr->location.flist_location.file_total)
            {
                cfg_ptr->location.flist_location.file_num = 1;
            }
            g_del_curfile = 0;
            //��ȡ��һ��
            bret = fsel_get_byno((void*) cfg_ptr->location.plist_location.filename,
                    cfg_ptr->location.flist_location.file_num);
        }
        else
        {
            //��ȡ��һ��
            bret = fsel_get_nextfile((void*) cfg_ptr->location.plist_location.filename);
        }
    }
    else
    {
        //��ȡ��һ��
        bret = fsel_get_prevfile((void*) cfg_ptr->location.plist_location.filename);
    }

    //��ȡ��ǰ������location
    if (bret == TRUE)
    {
        fsel_get_location(&(cfg_ptr->location.plist_location), cfg_ptr->fsel_type);

        mengine_info.eg_playinfo.cur_file_switch |= 0x01;
        //for audible file
        if (g_is_audible == TRUE)
        {
            _deal_audible_posfile(0);
            g_image_num = 0xffff;
        }
        else
        {
            //��ͨ�����л�ʱ��ǰʱ��������
            mengine_info.eg_playinfo.cur_time = 0;
        }
    }
    else
    {
        //�ָ����һ�ײ��Ÿ�����Ŀ¼��
        fsel_get_prevfile((void*) cfg_ptr->location.plist_location.filename);
    }

    _switch_end:
    //ǿ���л�
    if (force_mod == TRUE)
    {
        //loop_mode = cfg_ptr->shuffle_flag;//�ָ�ѭ��ģʽ
        loop_mode |= (uint8) cfg_ptr->repeat_mode;
        fsel_set_mode(loop_mode);
    }

    //����ϵ���Ϣ
    libc_memset(&cfg_ptr->bk_infor, 0, sizeof(mmm_mp_bp_info_t));

    return bret;

}
/******************************************************************************/
/*!
 * \par  Description:
 * \ʵ���ļ��л��������������β����һ�������˵�ͷ����һ������
 * \param[in]    stop_mode_e stop_mode ֹͣģʽ STOP_NORMAL/STOP_PAUSE
 * \param[in]    switch_mode_e force_mode �л�ģʽ
 * \li NORMAL_SWITCH_NEXT ��������һ��
 * \li NORMAL_SWITCH_PREV ��������һ��
 * \li FORCE_SWITCH_NEXT  ǿ������һ��
 * \li FORCE_SWITCH_NEXT  ǿ������һ��
 * \li END_SWITCH         ���ŵ��ļ�β����һ��
 * \param[in]    play_mode_e play_mode ����ģʽ
 * \param[out]   none
 * \return       switch_result_e
 * \retval       SWITCH_NO_ERR success
 * \retval       other failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
switch_result_e mengine_file_switch(stop_mode_e stop_mode, switch_mode_e switch_mode, play_mode_e play_mode)
{
    bool ret;
    switch_result_e switch_result = SWITCH_NO_ERR;

    //ֹͣ��ǰ��������
    ret = _stop(stop_mode);

    if (ret == FALSE)
    {
        switch_result = SWITCH_ERR_STOP;
        goto switch_end;
    }
    //��⿨���Ƿ����
    ret = _mengine_check_sd_in();

    if (ret == FALSE)
    {
        switch_result = SWITCH_ERR_STOP;
        goto switch_end;
    }

    //�л�������
    ret = _next_or_prev(switch_mode);
    if (ret == FALSE)
    {
        //�л�ʧ��ʱ����Ƿ񲥷ŵ����һ�׸���
        if (switch_mode == END_SWITCH)
        {
            if (play_mode == PLAY_FFON)
            {
                play_mode = PLAY_NO_PLAY;
            }
            else
            {
                //ǿ���л�����һ�׸�����ֹͣ���ţ�ֻ�ǻ�ȡ��Ϣ
                _next_or_prev(FORCE_SWITCH_NEXT);
                play_mode = PLAY_NO_PLAY;
            }
        }
        else
        {
            switch_result = SWITCH_ERR_SWITCH;
            goto switch_end;
        }
    }

    //�Ƿ񲥷Ÿ���
    if (play_mode != PLAY_NO_PLAY)
    {
        //ret = _set_file();

        ret = _play(play_mode);

        if (ret == FALSE)
        {
            switch_result = SWITCH_ERR_PLAY;
        }
    }
    else
    {
        //��ȡ�ļ���Ϣ
        _file_switch_info();
    }
    switch_end: return switch_result;
}

