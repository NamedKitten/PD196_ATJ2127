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

#include "app_music_setmenu.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_open_engine(mengine_enter_mode_e start_mode)
 * \װ������,���������ڣ����ٰ�װ
 * \param[in]    bool  para1
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_open_engine(mengine_enter_mode_e start_mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t temp_reply;
    engine_type_e engine_type;

    engine_type = get_engine_type();

    if (engine_type == ENGINE_NULL)
    {
        //����Ϣ����
        msg.content.data[0] = APP_ID_MENGINE;
        msg.content.data[1] = (uint8)(start_mode | g_file_path_info.file_path.plist_location.disk);
        //��Ϣ����(����Ϣ����)
        msg.type = MSG_CREAT_APP_SYNC;
        //����ͬ����Ϣ
        bret = send_sync_msg(APP_ID_MANAGER, &msg, &temp_reply, 0);
    }
    else if (engine_type == ENGINE_MUSIC)
    {
        //�����Ѵ��ڣ����ٰ�װ
        bret = TRUE;
    }
    else
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_clear_error(void)
 * \��������
 * \param[in]    void para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_playing_paint.c
 * \note
 */
/*******************************************************************************/
bool music_clear_error(void)
{
    bool bret = TRUE;
    msg_apps_t msg;

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_CLEAR_ERR_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_eror_deal(play_status_e status_prev)
 * \������ʾ���л�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       app_result_e
 * \retval       none
 * \ingroup      music_playing_paint.c
 * \note
 */
/*******************************************************************************/
app_result_e play_eror_deal(play_status_e status_prev)
{
    bool ret_val;
    file_location_t* locatptr;//ָ��location��ָ��
    msg_apps_t msg;
    uint16 str_id;
    app_result_e err_ret = RESULT_NULL;

    locatptr = (file_location_t*) &(g_file_path_info.file_path.flist_location);

    //��������
    //music_clear_error();

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_CLEAR_ERR_SYNC;
    //����ͬ����Ϣ
    send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);

    g_error_num++;

    if (g_file_path_info.file_path.dirlocation.disk == DISK_H)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            //��ⲻ������
            return music_deal_sd_out();
        }
    }

    if (g_error_num <= (locatptr->file_total))
    {
        switch (g_music_status.err_status)
        {
            case EG_ERR_DRM_INFO:
            str_id = S_LICENSE_FAIL;
            break;

            case EG_ERR_OPEN_FILE:
            str_id = S_FILE_OPEN_ERROR;
            break;

            case EG_ERR_NOT_SUPPORT:
            str_id = S_FILE_NO_SUPPORT;
            break;

            default:
            str_id = S_FORMAT_ERROR;
            break;
        }

        //��ʾ����
        err_ret = gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, str_id);

        if (g_switch_prev == 1)
        {
            //�л�����һ��
            ret_val = music_play_prev();
        }
        else
        {
            //�л�����һ��
            ret_val = music_play_next();
        }

        //�л�����Ч�ļ���֮ǰ�ǲ���״̬
        if ((status_prev == PlaySta) && (ret_val == TRUE))
        {
            //����
            music_play();
        }
        music_get_status(&g_music_status);//��ȡ״̬��Ϣ
        SetFullInterface()
    }
    if ((locatptr->file_total) == 0)
    {
        //�Ƿ��̨����music
        engine_type_e engine_type = get_engine_type();

        //��ʾ����
        err_ret = gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_NO_FILE);

        if (engine_type == ENGINE_MUSIC)
        {
            //����ر�
            music_close_engine();
        }

        err_ret = RESULT_MAIN_APP;
    }

    if (err_ret == RESULT_REDRAW)
    {
        err_ret = RESULT_NULL;
    }
    return err_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_volume(uint8 volume)
 * \��������
 * \param[in]    volume �����ȼ�
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_set_volume(uint8 volume)
{
    bool bret;
    bret = music_send_msg(&volume, NULL, MSG_MENGINE_SET_VOLUME_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _scene_play_seteq(uint8 eq_type)
 * \����eq
 * \param[in]    eq_type  para1
 * \param[out]   none
 * \return       void the result
 * \retval       void
 * \retval       none
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
void _scene_play_seteq(uint8 eq_type)
{

    if (g_setting_comval.music_comval.srs != VALUE_SRS_OFF)
    {
        //srs����
        if (g_setting_comval.music_comval.srs == VALUE_WOWHD_ON)
        {
            set_scene_srswowhd_callback();
        }
        else
        {
            //���ÿؼ�
            set_scene_srsuser_callback();
        }
    }
    else if (g_setting_comval.music_comval.fullsound != VALUE_FULLSOUND_OFF)
    {
        //fullsound����
        set_scene_fullsoundon_callback();

    }
    else if (g_setting_comval.music_comval.variable_playback != 0)
    {
        //�����ٶ�����
        set_scene_speed_callback((int16) g_setting_comval.music_comval.variable_playback);
    }
    else
    {
        //eq ����
        switch (eq_type)
        {
            //eq off
            case VALUE_EQ_OFF:
            set_scene_eqnormal_callback();
            break;
            //eq rock
            case VALUE_EQ_ROCK:
            set_scene_eqrock_callback();
            break;
            //ea funk
            case VALUE_EQ_FUNK:
            set_scene_eqfunk_callback();
            break;
            //eq hip hop
            case VALUE_EQ_HIP_HOP:
            set_scene_eqhiphop_callback();
            break;
            //eq jazz
            case VALUE_EQ_JAZZ:
            set_scene_eqjazz_callback();
            break;
            //eq classical
            case VALUE_EQ_CLASSICAL:
            set_scene_eqclassic_callback();
            break;
            //eq teckno
            case VALUE_EQ_TECHNO:
            set_scene_eqtechno_callback();
            break;
            //eq custom
            case VALUE_EQ_CUSTOM:
            set_scene_equser_callback();
            break;

            default:
            set_scene_eqnormal_callback();
            break;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_init(file_path_info_t* path_locat music_play_set_e play_set)
 * \���ų�����ʼ��
 * \param[in]    paly_set���벥�ų�������� para1
 *               typedef struct
 *							 {
 *							 	fsel_type_e file_sorce;
 *							 	uint16 reserved;
 *							 	musfile_location_u file_path;
 *							 }file_path_info_t
 * \param[in]    path_locat�ļ�·����Ϣָ�� para2
 *							 typedef enum
 *							 {
 *							 	PLAY_START //�����ļ���ʼ����
 *							 	PLAY_RESUME //����ͣ�ָ�����
 *							 	PLAY_KEEP //���ֲ���/��ͣ״̬
 *							 }music_play_set_e;
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       ......
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool _scene_play_init(file_path_info_t* path_locat, music_play_set_e play_set)
{
    bool ret_val = TRUE;

    file_location_t* locatptr;//ָ��location��ָ��

    id3_type_e cur_type;//��ǰ���ֵ�����

    engine_type_e engine_type;

    msg_apps_t msg;

    fsel_type_e comp_type = path_locat->file_source;
    locatptr = (file_location_t*) &(path_locat->file_path.dirlocation);

    cur_type = play_scene_checktype(locatptr->filename);

    //��δ��music����֮ǰ��⵱ǰ�ļ������Ƿ�Ϊaudible������sys�ļ�
    //������Ϊ�̷����⴦��Ƚ��鷳
    if ((cur_type == ID3_TYPE_AAX) || (cur_type == ID3_TYPE_AA))
    {
        g_audible_now = TRUE;
        if (play_set != PLAY_KEEP)
        {
            music_load_sysfile();
        }
    }
    else
    {
        g_audible_now = FALSE;
    }

    //���ص�radio���ų���
    msg.type = MSG_KILL_APP_SYNC; //MSG_KILL_APP; //ע��radio��̨

    engine_type = get_engine_type();

    if (engine_type == ENGINE_RADIO)
    {
        msg.content.data[0] = APP_ID_FMENGINE;
        //����ͬ����Ϣ
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    g_need_open_pa = FALSE;

    if (engine_type != ENGINE_MUSIC)
    {
        //������
        if ((play_set == PLAY_START) || (play_set == PLAY_AUDIBLE_RESUME))
        {
            music_open_engine(ENTER_START);
        }
        else
        {
            g_need_open_pa = TRUE;
            if (g_enter_mode == PARAM_BACKTO_PLAYING)
            {
                music_open_engine(ENTER_RESUME);
            }
            else
            {
                music_open_engine(ENTER_RESUME_PAUSE);
            }
        }

        //set eq
        _scene_play_seteq(g_setting_comval.music_comval.eq_setting);

        //set volume
        music_set_volume(g_setting_comval.g_comval.volume_current);
    }

    //֮����Ҫ����ΪPLAY_KEEP,����Ϊǰ�����ֶ������¿�ʼ���ţ���Ҫ����
    //setting�е�ѭ��ģʽ�������á�������PLAY_KEEP,����������ţ��򲻸�
    //�ı�ѭ��ģʽ;�����standby������ҲӦ�ø�������Ĳ����Լ����ã�����
    //Ӧ����Ӧ������
    if (play_set != PLAY_KEEP)
    {
        //Ϊ������δ��������֮ǰ ��settingӦ������ѭ��ģʽ��eq�����
        //set loop&shuffle mode
        if (g_audible_now == FALSE)
        {
            if (g_shuffle_tmporary == FALSE)
            {
                music_set_playmode(g_setting_comval.music_comval.music_repeat_mode
                        | g_setting_comval.music_comval.music_shuffle);
            }
            else
            {
                //��ʱshuffle
                music_set_playmode(FSEL_MODE_NORMAL | 0x10);
            }
        }
        else
        {
            //ǿ��ȫ��ѭ��
            music_set_playmode(FSEL_MODE_LOOPALL);
        }
    }

    if (play_set == PLAY_START)
    {
        path_locat->file_source = g_browser_filepath.file_source;

        //�����ļ�·��
        ret_val = music_set_filepath(path_locat);

        //ֻ����music_set_filepath�Ƿ�ɹ�
        if(ret_val == TRUE)
        {
            music_play();
        }
        //��ʼ����ʱ����״̬Ϊ����״̬
        g_music_status.play_status = PlaySta;
    }
    else if (play_set == PLAY_AUDIBLE_RESUME)
    {
        //�����ļ�·��
        ret_val = music_set_filepath(path_locat);

        //ֻ����music_set_filepath�Ƿ�ɹ�
        if(ret_val == TRUE)
        {
            //ѡ���������
            music_play_bookmark(&g_audible_bkinfo);
        }

        //�������
        comp_type = g_browser_filepath.file_source;

        //�ϵ㲥��ʱ����״̬Ϊ����״̬
        g_music_status.play_status = PlaySta;
    }
    else
    {
        //����ԭ��״̬
        //��ǰ���ŵ���������Ŀ��
        music_get_filepath(path_locat);//��ȡ�ļ�·��

        //�鿴��ʱ�̷��Ƿ����
        if(locatptr->disk == DISK_H)
        {
            if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
            {
                //��ⲻ������
                music_deal_sd_out();
                return FALSE;
            }
        }

        //����ԭ�в���״̬ʱ,��Ҫ��ʼ���ļ�ѡ����Ϊԭ��״̬
        if (path_locat->file_path.dirlocation.file_total != 0)
        {
            comp_type = g_browser_filepath.file_source;
            //����standby�����ĸ�����֮ǰ����֪�������ļ�����
            //���Ҫ�ٴ�ȷ��
            cur_type = play_scene_checktype(locatptr->filename);
            if ((cur_type == ID3_TYPE_AAX) || (cur_type == ID3_TYPE_AA))
            {
                g_audible_now = TRUE;
            }
        }
        else
        {
            //��ʾ����
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_NO_FILE);
            music_close_engine();
            ret_val = FALSE;
        }
    }

    if(ret_val == TRUE)
    {
        if ((path_locat->file_source != comp_type) || (locatptr->disk != g_browser_filepath.file_path.dirlocation.disk))
        {
            fsel_exit();
            music_file_init(path_locat);
        }

        //ͬ��location��Ϣ
        libc_memcpy(&g_browser_filepath, path_locat, sizeof(file_path_info_t));

        //������ʾ
        //g_file.total = locatptr->file_total;
        //g_file.num = locatptr->file_num;

        //������ʾˢ��timer ��λ��ǰʱ�䡢��ʵ���ʾ��ʶ1s
        time_frush_id = set_app_timer(APP_TIMER_ATTRB_UI, 250, play_flag_update);

        //���ſ�������
        if (locatptr->disk == DISK_H)
        {
            change_card_state(CARD_STATE_CARD_PLAY);
        }

        if (g_audible_now == TRUE)
        {
            //��Ҫ����audible�����ڶϵ�
            g_music_config.audible_bk_flag = TRUE;
        }

        //����Ϊ����״̬
        change_app_state(APP_STATE_PLAYING);
#ifdef PRINT_BANK_INFO
        print_label = 0x12345678;
#endif

    }

    return ret_val;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_exit(void)
 * \�˳����ų���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool _scene_play_exit(void)
{
    //������ʱ��ȡ��
    kill_app_timer(scroll_frush_id);
    scroll_frush_id = -1;

    kill_app_timer(time_frush_id);
    time_frush_id = -1;

    //����Ϊ�ǲ���״̬
    change_app_state(APP_STATE_NO_PLAY);

#ifdef PRINT_BANK_INFO
    print_label = 0;
#endif
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint6 music_scene_playing(app_param_e enter_mode file_path_info_t* path_locat music_play_set_e paly_set)
 * \���ų�������
 * \param[in]    enter_mode����ģʽ  para1
 *               typedef enum
 *							 {
 *							 	ENTER_FROM_DESKTOP
 *							 	ENTER_FROM_BROWSER
 *							 	ENTER_FROM_PLAYNOW
 *							 	ENTER_FROM_UI_MENU
 *							 	MODEMAX
 *							 }app_param_e;
 * \param[in]    paly_set���벥�ų�������� para2
 *               typedef struct
 *							 {
 *							 	fsel_type_e file_sorce;
 *							 	uint16 reserved;
 *							 	musfile_location_u file_path;
 *							 }file_path_info_t
 * \param[in]    path_locat�ļ�·����Ϣָ�� para3
 *							 typedef enum
 *							 {
 *							 	PLAY_START //�����ļ���ʼ����
 *							 	PLAY_RESUME //����ͣ�ָ�����
 *							 	PLAY_KEEP //���ֲ���/��ͣ״̬
 *							 }music_play_set_e;
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       ......
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/

app_result_e music_scene_playing(app_param_e enter_mode, file_path_info_t* path_locat, music_play_set_e paly_set)
{
    bool result;
    app_result_e ret_vals;

    style_infor_t style_infor;

    //������ʼ�� ��ʼ���ļ�ѡ��������ȡUI����ʾ�����ݵ�
    result = _scene_play_init(path_locat, paly_set);

    if (result == FALSE)
    {
        return RESULT_SCENE_EXIT;
    }
    //ˢ�½����ʶ
    SetFullInterface()

    //g_paint_flag = TRUE;
    //��ȡר��ͼƬ��ʾ����
    style_infor.style_id = ALBUM_ART_REGION;
    style_infor.type = UI_AP;
    ui_get_textbox_attrb(&style_infor, &album_art_info.region, 0);

    //���������Ĳ���
    g_com_paint_cb.func = play_paint_albumart;
    g_com_paint_cb.arg = &album_art_info;

    //����ѭ��
    g_error_num = 0;
    g_check_mengine_status = TRUE;
    g_total_time = 0;
    g_speed_counter = 0;
    g_paint_flag = TRUE;
    g_lyric_multi_screen = FALSE;

    //ͨ��������Դ���޸���Ĭ��ѡ��
    if (g_setting_comval.music_comval.music_from != VALUE_FROM_DEFAULT)
    {
        music_playlist_update_history(path_locat, g_setting_comval.music_comval.music_from);
        g_setting_comval.music_comval.music_from = VALUE_FROM_DEFAULT;
    }

    ret_vals = _playing_loop_deal(path_locat);
    //���ó����˳�����
    _scene_play_exit();
    return ret_vals;
}
