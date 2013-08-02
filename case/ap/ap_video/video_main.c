/*******************************************************************************
 *                              US212A
 *                            Module: Video
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     Video
 * \brief    Video ��main����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "video.h"
#include "video_menu_cfg.h"
#include "music_common.h"

//ȫ�ֱ�������

/*! \brief videoӦ��VM����*/
g_video_var_t g_video_var;

/*! \brief ͨ��VM����*/
comval_t g_comval;

/*! \brief video �����ļ�ϵͳ��mount ID*/
int32 video_mount;

/*! \brief ��Ƶ�ļ���ʱ��*/
uint32 g_total_time;

/*! \brief ����ˢ�½������Ķ�ʱ��ID��1s��ʱ*/
int8 timer_flush_progress;

/*! \brief ���ڹ�����ʾ�ļ����Ķ�ʱ��ID��80ms��ʱ*/
int8 timer_flush_filename;

/*! \brief ����ȫ��̬�봰��̬�л��Ķ�ʱ��ID, 5s��ʱ*/
int8 timer_flush_screen;

/*! \brief ���ڿ��ƶ�ʱ���Ŀ�����ر� bit0: ȫ����ʱ�� bit1:�ļ�����ʱ�� bit2:��������ʱ��*/
uint8 g_video_timer_flag;

/*! \brief ˢ��λ��־����*/
uint32 g_display_bitmap;

/*! \brief videoӦ�ý���ģʽ*/
app_param_e g_video_enter_mode;

/*! \brief ��Ƶ������*/
void* g_video_handle;

/*! \brief ��Ƶ����״̬*/
video_play_status_e g_video_play_status;

/*! \brief ��Ƶ����״̬����*/
video_play_status_e g_video_play_status_backup;

/*! \brief ��Ƶ��ǰ����״̬*/
video_window_e g_video_cur_window;

/*! \brief ��Ƶ֮ǰ����״̬*/
video_window_e g_video_prev_window;

/*! \brief ˢ�±�־����ʶ�Ƿ�ˢ��*/
uint8 g_video_paint_flag;

/*! \brief Ӧ�û�ȡ��Ƶ������Ϣ�ı���*/
mmm_vp_userparam_t g_video_userinfor;

/*! \brief Ӧ�ÿ��������ڱ���*/
ctlinfor_t g_video_ctlinfor;

/*! \brief ����������*/
wregion_t g_region_fullscreen;

/*! \brief ʱ�������������*/
wregion_t g_region_progressbar;

/*! \brief ������������*/
wregion_t g_region_volumebar;

/*! \brief ���������������߶ȵ�������*/
adjust_region_t g_region_adjust;

/*! \brief �Զ������л��ļ���־ bit0: 0:����һ�� 1:����һ�� bit1:�Ƿ��л�����*/
uint8 g_video_change_file;

/*! \brief videoˢ��ģʽ */
uint8 g_video_draw_mode;

/*! \brief video��������ٶȿ��� */
uint8 g_video_speed_control;

/*! \brief videoʱ����� */
uint8 g_video_timer_counter;

/*! \brief �ļ��������� */
uint8 g_video_filename[LIST_ITEM_BUFFER_LEN];

/*! \brief �ļ����Ƿ������ʾ��־ */
bool need_scroll_filename;

/*! \brief  Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������*/
app_timer_t video_app_timer_vector[APP_TIMER_TOTAL];

/*! \brief videoҪ����ĳ���*/
scene_video_e g_video_scene_next;

/*! \brief video��һ�ε��ȵĳ���*/
scene_video_e g_video_scene_prev;

/*! \brief video�ļ���������*/
uint16 g_file_total;

/*! \brief video�����ļ�����*/
uint16 g_file_err;

/*! \brief video�Ƿ�ɾ���ļ�*/
uint8 g_del_file;

uint8 g_fast_back_end;
#ifndef PC
/*! \brief videoʹ�õĶ�ջָ�� */
OS_STK *ptos = (OS_STK *) AP_VIDEO_STK_POS;
#endif
/*! \brief videoʹ�õĽ������ȼ� */
INT8U prio = AP_VIDEO_PRIO;

/*! \brief ������Դ�ļ�(common style)�� */
const uint8 com_themes[5][13] =
{ "common.sty", "cm_green.sty", "cm_gray.sty", "cm_pink.sty", "cm_red.sty" };

/*! \brief video��Դ�ļ��� */
const uint8 video_sty[] =
{ "video.sty" };

/*! \brief video�˵������ļ� */
const uint8 video_mcg[] =
{ "video.mcg" };

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��̨�������ļ����̷�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_main
 * \note
 *******************************************************************************/
uint8 music_get_disk_type(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;

    file_path_info_t music_file_path;

    //��ȡ��ǰ�����ļ���·����Ϣ
    reply.content = &music_file_path;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);

    return music_file_path.file_path.dirlocation.disk;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  Video ��ȡӦ��VMȫ�ֱ���,��ʼ��ȫ�ֱ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_main
 * \note
 *******************************************************************************/
static void _video_read_var(void)
{
    int result;
    engine_type_e engine_type;
    setting_comval_t temp_val;

    //��ȡcommon VM����������Ϣ
    result = sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));

    //��ʼ��ϵͳ��VM����
    if (temp_val.g_comval.magic != MAGIC_SETTING)
    {
        com_get_default_setting(&temp_val);
        sys_vm_write(&temp_val, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_val.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    //��ȡmusic VM����������Ϣ
    result = sys_vm_read(&g_video_var, VM_AP_VIDEO, sizeof(g_video_var));

    //��һ�ν���AP����ʼ��VM����
    if (g_video_var.magic != MAGIC_VIDEO)
    {
        g_video_var.magic = MAGIC_VIDEO;
        libc_memset(&g_video_var.path.file_path, 0, sizeof(musfile_location_u));
        g_video_var.path.file_path.flist_location.disk = DISK_BASE;
        g_video_var.file_ext_bitmap = VIDEO_BITMAP;
        g_video_var.repeat_mode = VIDEO_REPEAT_NORMAL;
        g_video_var.prev_enter_mode = PARAM_FROM_MAINMENU;
        libc_memset(&g_video_var.break_point, 0, sizeof(g_video_var.break_point));
    }

    if ((g_video_enter_mode == PARAM_FROM_PLAYLIST)
        || (g_video_enter_mode == PARAM_BACKTO_PLAYING)
        || (g_video_enter_mode == PARAM_BACKTO_PLAYED))
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            //��ⲻ�����̣����ʼ����Ĭ���̷�
            g_video_var.path.file_path.dirlocation.disk = DISK_BASE;
        }
    }
    else
    {
        g_video_var.path.file_path.dirlocation.disk = DISK_BASE;
    }

    if (g_video_enter_mode == PARAM_FROM_BROWSER)
    {
        sys_vm_read(&g_video_var.path, VM_AP_BROWSER, sizeof(file_path_info_t));
    }

    //����������ȡ��ǰ�����ļ���·����Ϣ
    engine_type = get_engine_type();
    if (engine_type == ENGINE_MUSIC)
    {
        //�����̷�
        g_video_var.path.file_path.flist_location.disk = music_get_disk_type();
    }

    g_app_info_state.video_state = 0; 
}

/******************************************************************************/
/*!
 * \par  Description:
 *    video Ӧ�÷���ֵ������������Ӧ�ķ���ֵת������Ϣ����
 * \param[in]    result app_result_e ������������ֵ
 * \param[out]   none
 * \return       the result
 * \ingroup      video_main
 * \note
 *******************************************************************************/
static app_result_e _deal_video_result(app_result_e video_result)
{
    msg_apps_t msg;
    engine_state_e engine_status;
    engine_type_e engine_type;
    bool need_send_msg = TRUE;

    engine_status = get_engine_state();
    engine_type = get_engine_type();

    //��process manager���ʹ�������������Ϣ
    msg.type = MSG_CREAT_APP;
    msg.content.data[1] = (uint8) PARAM_FROM_VIDEO;

    if(video_result == RESULT_CARD_OUT_ERROR)
    {
        //��ⲻ������
        video_clear_breakinfor();      
        if(g_video_enter_mode == PARAM_FROM_BROWSER)
        {
            video_result = RESULT_RETURN_BROWSER;
        }
        else
        {
            video_result = RESULT_MAIN_APP;
        }
    }

    switch (video_result)
    {
        case RESULT_RETURN_BROWSER:
        msg.content.data[0] = APP_ID_BROWSER;
        break;

        case RESULT_NOWPLAYING:
        msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYING;
        if (engine_type == ENGINE_MUSIC)
        {
            msg.content.data[0] = APP_ID_MUSIC;           
        }
        else if (engine_type == ENGINE_RADIO)
        {
            msg.content.data[0] = APP_ID_RADIO;
        }
        else
        {
            ;//nothing
        }
        break;

        case RESULT_LASTPLAY:
        msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYED;
        if (engine_type == ENGINE_MUSIC)
        {
            msg.content.data[0] = APP_ID_MUSIC;

        }
        else if (engine_type == ENGINE_RADIO)
        {
            msg.content.data[0] = APP_ID_RADIO;
        }
        else
        {
            ;//nothing
        }
        break;

        case RESULT_CREATE_LIST_APP:
        msg.content.data[0] = APP_ID_PLAYLIST;
        //ֻ���µ�ǰ�̷���playlist
        if (g_video_var.path.file_path.dirlocation.disk == DISK_C)
        {
            msg.content.data[1] = (PLIST_DISK_C | PARAM_FROM_VIDEO);
        }
        else
        {
            msg.content.data[1] = (PLIST_DISK_H | PARAM_FROM_VIDEO);
        }
        //�������ģʽ
        g_video_var.prev_enter_mode = g_video_enter_mode;
        break;

        case RESULT_APP_QUIT:
        need_send_msg = FALSE;
        //�������ģʽ
        g_video_var.prev_enter_mode = g_video_enter_mode;        
        break;

        default:
        //Ĭ�ϵ���������ֵ������main
        msg.content.data[0] = APP_ID_MAINMENU;
        break;
    }

    if (TRUE == need_send_msg)
    {
        //��process manager���ʹ���������Ϣ ͬ����Ϣ�����첽��Ϣ
        send_async_msg(APP_ID_MANAGER, &msg);
    }
    return video_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    video ��ɲ��ſ��Ʋ�����ʼ���Լ���������ѡ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_main
 * \note
 *******************************************************************************/
void _video_data_init(void)
{
    uint8 other_enter_mode = 0;
    //��ʼ����Ƶ������������
    style_infor_t video_style;
    video_style.type = UI_AP;

    //��ȡ�������������
    video_style.style_id = STY_VIDOE_FULLSCREEN_REGION;
    ui_get_textbox_attrb(&video_style, &g_region_fullscreen, 0);

    video_style.style_id = STY_VIDEO_PROGRESS_REGION;
    ui_get_textbox_attrb(&video_style, &g_region_progressbar, 0);

    video_style.style_id = STY_VIDEO_VOLUME_REGION;
    ui_get_textbox_attrb(&video_style, &g_region_volumebar, 0);

    //����amv��avi��ͼ��ʼ�㲢����ͬ���������Ҫ��������
    //       |------------------|
    // avi-> |----------------->|
    // ��ʼ��|----------------->|
    //       |----------------->|
    //       |------------------|
    //       |------------------|
    g_region_adjust.avi_prog_y = g_region_progressbar.y;
    g_region_adjust.avi_vol_y = g_region_volumebar.y;

    //       |------------------|
    //       |------------------|
    //       |----------------->|
    //       |----------------->|
    // amv-> |----------------->|
    // ��ʼ��|------------------|
    g_region_adjust.amv_prog_y = g_region_fullscreen.h - g_region_progressbar.y - g_region_progressbar.h;
    g_region_adjust.amv_vol_y = g_region_fullscreen.h - g_region_volumebar.y - g_region_volumebar.h;

    //mount_ID �м���ݴ˲����ļ��������ļ�
    g_video_userinfor.mountindex = (uint32) video_mount;

    //�ϵ����
    g_video_userinfor.pbreakinfor = &g_video_var.break_point;

    //����������
    g_video_userinfor.pregion1 = &g_region_fullscreen;
    g_video_userinfor.pregion2 = &g_region_progressbar;
    g_video_userinfor.pregion3 = &g_region_volumebar;

    //��ʼ�����ſ��Ʋ���
    g_video_ctlinfor.fadeout = 0;
    g_video_ctlinfor.softvolume = 0;
    g_video_ctlinfor.progressbar = 0;
    g_video_ctlinfor.volumebar = 0;
    g_video_userinfor.plcminfor = &g_video_ctlinfor;

    //�Ӳ����б���룬ѡ��֮ǰ�����ģʽ
    if ((g_video_enter_mode != PARAM_FROM_MAINMENU) && (g_video_enter_mode != PARAM_FROM_BROWSER))
    {
        if((g_video_enter_mode != PARAM_BACKTO_PLAYED)
            && (g_video_enter_mode != PARAM_BACKTO_PLAYING))
        {
            other_enter_mode = 1; 
        } 
        else
        {
            other_enter_mode = 2;
        }
        g_video_enter_mode = g_video_var.prev_enter_mode;
    }

    //����������룬���ȸ������޿���ѡ�����˵����������б���
    if (g_video_enter_mode == PARAM_FROM_MAINMENU)
    {

        this_headbar_icon_id = HEADBAR_ICON_VIDEO;
        g_video_var.path.file_source = FSEL_TYPE_LISTVIDEO;
        g_video_var.path.file_path.plist_location.list_type = PLIST_TITLE;

        if ((sys_detect_disk(DRV_GROUP_STG_CARD) == -1) || (other_enter_mode == 1))
        {
            g_video_scene_next = SCENE_FILELIST;
        }
        else
        {
            g_video_scene_next = SCENE_MENU;
        }
    }
    else
    {
        //����ģʽ���룬ֱ�ӽ��벥�ų���
        g_video_scene_next = SCENE_PLAY;
        this_headbar_icon_id = HEADBAR_ICON_BROWSER_V;
    }

    //standby�������ţ�ֱ�ӽ��벥�ų���
    if(other_enter_mode == 2)
    {
        g_video_scene_next = SCENE_PLAY;    
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video Ӧ�õĳ�ʼ��
 * \param[in]    void
 * \param[out]   void
 * \return       app_result_e
 * \retval       RESULT_NULL    success
 * \retval       other value    fail
 * \ingroup      video_main
 * \note
 *******************************************************************************/
app_result_e _video_app_init(void)
{
    app_result_e result;

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_VIDEO, APP_TYPE_GUI);

    //��ʼ����ʱ��
    init_app_timers(video_app_timer_vector, APP_TIMER_TOTAL);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init(video_msg_callback);

    //��ʼ��common��������ʱ�������������ڳ�ʼ������ʱ�������
    sys_timer_init();

    //��ʼui��Դ�ļ�
    ui_res_open(com_themes[g_comval.theme], UI_COM);

    ui_res_open(video_sty, UI_AP);

    //��ǰ̨Ӧ�ò˵����ýű�
    com_open_confmenu_file(video_mcg);

    //��ʼ��ȫ�ֱ���
    _video_data_init();

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
#endif

    //��ʼ���ļ�ѡ����
    result = video_disk_initial();
    if (result == RESULT_MAIN_APP)
    {
        g_video_scene_next = SCENE_EXIT;
        return result;
    }
    else if(result == RESULT_VIDEO_MENU)
    {
        g_video_scene_next = SCENE_MENU;
    }
    else
    {
        ;//nothing
    }    
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video Ӧ�õ��˳�
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \retval       1    success
 * \retval       0    fail
 * \ingroup      video_main
 * \note
 *******************************************************************************/
bool _video_app_deinit(void)
{
    //�ļ�ѡ�����˳�
    fsel_exit();

    //�ر�Ӧ�ò˵����ýű�
    com_close_confmenu_file();

    //ж��common��ʱ����������
    sys_timer_exit();

    //�ر�ui��Դ�ļ�
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    //ִ��applib���ע������
    applib_quit();
    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    video����comval VM����,����ʹ��setting_comval_t�������Ľṹ��
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_main
 * \note
 *******************************************************************************/
static void _video_save_comval(void)
{
#ifndef PC
    setting_comval_t temp_val;
    sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&temp_val.g_comval, &g_comval, sizeof(comval_t));
    sys_vm_write(&temp_val, VM_AP_SETTING);
#else
    setting_comval_t temp_val;
    char temp_buffer[512];
    sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&temp_val.g_comval, &g_comval, sizeof(comval_t));
    libc_memcpy(temp_buffer, &temp_val, sizeof(setting_comval_t));
    sys_vm_write(temp_buffer, VM_AP_SETTING);
#endif

}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video��һ��Ҫ���ȵĳ���
 * \param[in]    app_result_e ����ֵ
 * \param[out]   �����ķ���ֵ
 * \return       void
 * \ingroup      video_main
 * \note
 * \li  video��4������:�ļ��б���,option�˵���������ų����Ͳ˵�����(�п�ʱ����)
 *******************************************************************************/
static void _video_select_next_scene(app_result_e *ui_result)
{
    app_result_e result = *ui_result;
    g_video_scene_prev = g_video_scene_next;

    if (result == RESULT_DIR_ERROR_NO_FILE)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
        {
            //���ڿ���
            result = RESULT_VIDEO_MENU;
        }
        else
        {
            result = RESULT_MAIN_APP;
        }
    }

    switch (result)
    {
        case RESULT_VIDEO_MENU:
        g_video_scene_next = SCENE_MENU;
        break;

        case RESULT_VIDEO_OPTIONMENU:
        g_video_scene_next = SCENE_OPTIONMENU;
        break;

        case RESULT_VIDEO_FILELIST:
        g_video_scene_next = SCENE_FILELIST;
        break;

        case RESULT_MOVIE_PLAY:
        g_video_scene_next = SCENE_PLAY;
        break;

        default:
        g_video_scene_next = SCENE_EXIT;
        break;
    }

    *ui_result = result;
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video Ӧ�õ���ں����ͳ���������ģ��
 * \param[in]    argc �ɽ��̹���������Ĳ�������ʾ�Ӻ���ģʽ����
 * \param[in]    argv ���ò���
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      video_main
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    DC dc_status;
    app_result_e result;

    g_video_enter_mode = (app_param_e) argc;

    /* ����VM���� */
    _video_read_var();

    ui_get_DC_status(&dc_status);
    ui_set_pen_color(Color_WHITE_def);
    ui_set_backgd_color(Color_BLACK_def);

    result = _video_app_init();

    /*����������*/
    while (g_video_scene_next != SCENE_EXIT)
    {
        switch (g_video_scene_next)
        {
            case SCENE_FILELIST:
            result = video_filelist();
            break;

            case SCENE_OPTIONMENU:
            result = video_option_menulist();
            break;

            case SCENE_PLAY:
            result = video_scene_playing();
            break;

            case SCENE_MENU:
            result = video_menu();

            default:
            g_video_scene_next = SCENE_EXIT; //exit
            break;
        }
        _video_select_next_scene(&result);
    }

    ui_set_pen_color(dc_status.pen_color);
    ui_set_backgd_color(dc_status.backgd_color);

    _deal_video_result(result);
    //����VM����
    fsel_get_location((void *) &g_video_var.path.file_path.flist_location, FSEL_TYPE_PLAYLIST);
    sys_vm_write(&g_video_var, VM_AP_VIDEO);
    _video_save_comval();
    _video_app_deinit();

    return result;
}

