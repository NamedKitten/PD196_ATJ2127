/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-13          1.0              create this file
 *******************************************************************************/
#include "app_music.h"

#include "app_music_playing.h"

#include "app_music_menu_config.h"

//globle variable
//��ŵ�ǰ��������
music_scene_e g_music_scene;

//����music�ķ�ʽ
app_param_e g_enter_mode;

//����play�Ĳ���
music_play_set_e g_play_set;

//��ʾ��ʶ
bool g_paint_flag;

//album art �����ʶ
uint8 g_album_art;

//��ȡ�ļ���Ϣ��ʶ bit0:��ȡ�ļ�ID3��Ϣ bit1:��ȡ�ļ�·����Ϣ
uint8 g_getfile_info;

//��ʴ��ڱ�ʶ
bool g_lyric_flag;

//���ſ�������ʱ��id
int8 time_frush_id = 0xff;

//��¼������ʱ��״̬,�μ�id3_deal_status_e����
uint8 g_scroll_status;

//ID3������ʱ��
int8 scroll_frush_id = 0xff;

//����Ƿ���ʱshuffle
uint8 g_shuffle_tmporary = FALSE;

//��ǵ�ǰ�Ƿ���audible
uint8 g_audible_now = FALSE;

//��־ab�����滻
bool g_ab_switch;

//��ǰ�ļ���������Žṹ��
musfile_cnt_t g_file;

//�������������
uint16 g_error_num = 0;

//��ǰ��
uint16 g_switch_prev = 0;
//���id3��Ϣ
id3_info_t g_id3_info;

//���⻺����
uint8 g_title_buf[Id3BufSIZE];

//���ֻ�����
uint8 g_artist_buf[ArtBufSIZE];

//ר��������
uint8 g_album_buf[ArtBufSIZE];

//��������
scroll_content_t g_scroll_content[3];

//�������Ʋ�������
scroll_vars_t g_scroll_vars;

//region_t scroll_region;

//��ǰ���Ÿ������ļ��� �����album artʹ��
uint8 g_song_filename[64];

//ר��ͼƬ��ʾ��Ϣ
album_picinfo_t album_art_info;

//ˢ�±�־λ����
uint32 g_display_bitmap;

//�ļ�·����Դ��Ϣ���棬����ʱ����
file_path_info_t g_file_path_info;

//�����·����Ϣ�����ʱ��
file_path_info_t g_browser_filepath;

//����ļ�ϵͳid
int32 g_music_mount_id = -1;

//��ǰ����״̬��Ϣ
mengine_status_t g_music_status;

//��ǰ����ʱ��ͱ�������Ϣ
mengine_playinfo_t g_play_info;

//audible�Ķϵ���Ϣ
mmm_mp_bp_info_t g_audible_bkinfo;

//music ui����������Ϣ
music_config_t g_music_config;

//ϵͳȫ�ֱ���
setting_comval_t g_setting_comval;

//��ѯ����״̬��־λ
uint8 g_check_mengine_status;

//��ʱ������
uint8 g_timer_counter;

//��ǰ������ʱ��
uint32 g_total_time;

//��ǰ����ʱ��
uint32 g_cur_time;

//ǰһ�μ�¼����ʱ��
uint32 g_prev_time;

//����nandflash����ʾ��VM����ַ�������������ʣ�Ϊ�����ַ
uint32 g_bookmark_fp;

//��������ʼ���
uint8 g_speed_counter;

//�����ʾ������ʾ��־λ
bool g_lyric_multi_screen = FALSE;

//�ָ�Ĭ������eq��־
uint8 g_eq_restore;

//�Ƿ���Ҫ��PA��־
bool g_need_open_pa;

//����״̬���Ƿ���ȱ�־
bool g_scroll_deal;

//�ļ�ѡ����ģ������
fsel_module_t g_fsel_module;

//ǰ��������ɫ
DC g_dc_status;

//��ʱ��������λ��LCD��1k buffer
#ifndef PC
uint8 *g_temp_buffer_p = (uint8 *)LCD_BUF_ADDR;
#else
uint8 g_temp_buffer[1024];
uint8 *g_temp_buffer_p;
#endif

#ifdef PRINT_BANK_INFO
//��ӡ��־λ
int print_label;
#endif

//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT		2
/*!
 *  \brief
 *  app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
 */
app_timer_t music_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

#ifndef PC
OS_STK *ptos = (OS_STK *) AP_MUSIC_STK_POS;
#endif

INT8U prio = AP_MUSIC_PRIO;
/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \��ȡ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{
    //ϵͳ������ȡ
    sys_vm_read(&g_setting_comval, VM_AP_SETTING, sizeof(setting_comval_t));
    if (g_setting_comval.g_comval.magic != MAGIC_SETTING)
    {
        //com_get_default_setting(&g_setting_comval);
        g_setting_comval.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&g_setting_comval);
        sys_vm_write(&g_setting_comval, VM_AP_SETTING);
    }
    com_set_sys_comval(&g_setting_comval.g_comval);
    //AP_MUSIC_PRIO music ������ȡ
    sys_vm_read(&g_music_config, VM_AP_MUSIC, sizeof(music_config_t));

    if (g_music_config.magic != MAGIC_MUSIC)
    {
        g_music_config.magic = MAGIC_MUSIC;
        //�Ƿ����audible �ϵ�
        g_music_config.audible_bk_flag = 0;
        //�����ٶ�
        g_music_config.playspeed = 0;

        //ab��������
        g_music_config.ab_count = 5;
        //ab�������
        g_music_config.ab_gap = 1;
        //audible���½�ģʽ
        g_music_config.section_mode = 0;
        //���볡��
        g_music_config.enter_scene = SCENE_LSTMENU;
        //������������
        g_music_config.play_set = PLAY_START;

        //�Ƿ��browser����
        g_music_config.browser_sel = FALSE;

        //����get config��ok �����ȸ���ֵ
        //id3��ȡ����
        g_music_config.id3_support = (uint8) com_get_config_default(MUSIC_ID3_SUPPORT);
        //�����ʾ
        g_music_config.lrc_support = (uint8) com_get_config_default(MUSIC_LRC_SUPPORT);
        //album art��ʾ
        g_music_config.album_art_support = (uint8) com_get_config_default(MUSIC_ALBUM_ART_SUPPORT);

        g_music_config.albumlist_sel = FALSE;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{
    /*����VM����*/
    sys_vm_write(&g_music_config, VM_AP_MUSIC);
    sys_vm_write(&g_setting_comval, VM_AP_SETTING);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _check_bkinfo(void)
 * \���ϵ���Ч�Բ����ó�ʼ��Ĭ���̷�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 * \li   �ú�����standby����ʱ���ã������ж�music engine��¼�Ķϵ���Ϣ�Ƿ���Ч
 music ui�ݴ����ó�ʼ��Ĭ���̷�����Ӧ������
 */
/*******************************************************************************/
void _check_bkinfo(void)
{
    uint8 disk_type;
    mengine_config_t temp_config;

    sys_vm_read(&temp_config, VM_AP_MENGINE, sizeof(mengine_config_t));

    disk_type = temp_config.location.dirlocation.disk;

    if (disk_type == DISK_H)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            disk_type = DISK_C;
        }
    }

    g_browser_filepath.file_path.plist_location.disk = disk_type;
    g_file_path_info.file_path.plist_location.disk = disk_type;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_init(void)
 * \��ȡ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 * \li  ��ʼ���ļ�ѡ����ʱ����Ĭ���̷�Ϊ���̣��ļ�����ΪCOMMONDIR
 * \li  �����̨��music���Ż��browser�Ƚ��룬ͬ��location
 * \li  music uiĬ��û���̷����䣬��playlist������˳����̷�д��browser VM����
 */
/*******************************************************************************/
bool _app_init(void)
{
    bool app_init_ret;

    engine_type_e engine_type;

    //��ǰ���ֵ�����
    id3_type_e cur_type;

    //��ȡ������Ϣ (����vm��Ϣ)
    _load_cfg();

    //����ui��Դ�ļ�
    ui_res_open("music.sty", UI_AP);
    ui_res_open("common.sty", UI_COM);

    com_open_confmenu_file("music.mcg");

#ifdef PC
    g_temp_buffer_p = g_temp_buffer;
    load_menulist_simfunc(get_menu_func, get_callback_func, get_menu_option);
#endif

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_MUSIC, APP_TYPE_GUI);

    applib_message_init(music_message_deal);

    //��ʼ����ʱ��
    init_app_timers(music_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //������init_app_timers �������
    sys_timer_init();

    ui_get_DC_status(&g_dc_status);
    ui_set_pen_color(Color_WHITE_def);
    ui_set_backgd_color(Color_BLACK_def);

    //��ʼ���ļ�ѡ����
    g_file_path_info.file_path.plist_location.disk = DISK_C;

    //��ʼ���ļ�ѡ����ΪĿ¼ģʽ
    g_file_path_info.file_source = FSEL_TYPE_COMMONDIR;

    //����������ȡ��ǰ�����ļ���·����Ϣ
    engine_type = get_engine_type();
    if (engine_type == ENGINE_MUSIC)
    {
        //��ȡ�ļ�·��
        music_get_filepath(&g_file_path_info);

        //ͬ������ļ���locationΪ��̨���ŵ�location
        libc_memcpy(&g_browser_filepath, &g_file_path_info, sizeof(file_path_info_t));
    }

    //�̷�����һ��
    g_browser_filepath.file_path.plist_location.disk = g_file_path_info.file_path.plist_location.disk;

    //��browserѡ���˸��������vram�ж���·����Ϣ
    if ((g_enter_mode == PARAM_FROM_BROWSER) || (g_enter_mode == PARAM_FROM_PLAYLIST) || (g_enter_mode
            == PARAM_FROM_RECORD))
    {
        sys_vm_read(&g_browser_filepath, VM_AP_BROWSER, sizeof(file_path_info_t));

        if (g_enter_mode == PARAM_FROM_PLAYLIST)
        {
            //����ϵ��־
            g_music_config.audible_bk_flag = FALSE;

            //ʹ·����Ϣ������Ч״̬���ָ�Ĭ��״̬
            libc_memset(&(g_browser_filepath.file_path.dirlocation.filename), 0, 4);
        }

        //��brwoserѡ���˸�������
        libc_memcpy(&g_file_path_info, &g_browser_filepath, sizeof(file_path_info_t));
    }
    else
    {
        if ((g_enter_mode == PARAM_BACKTO_PLAYED) || (g_enter_mode == PARAM_BACKTO_PLAYING))
        {
            if (engine_type == ENGINE_NULL)
            {
                //���ϵ���Ч�Բ����ó�ʼ��Ĭ���̷�
                _check_bkinfo();
            }
        }
    }

    //���ݺ�׺������ļ�����music/audible
    cur_type = play_scene_checktype(g_file_path_info.file_path.dirlocation.filename);

    if ((cur_type == ID3_TYPE_AAX) || (cur_type == ID3_TYPE_AA))
    {
        g_audible_now = TRUE;
    }
    else
    {
        g_audible_now = FALSE;
    }

    //�ļ�ѡ������ʼ��
    app_init_ret = music_file_init(&g_file_path_info);

    if (app_init_ret == TRUE)
    {
        //�����ǩ�ļ��Ƿ����,�粻�����򴴽�
        check_bookmark_exist();

        //���û�ȡ�ļ�·����Ϣ��־λ
        g_getfile_info |= GET_FILEINFO_FILEPATH;
    }
    return app_init_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \�˳�app�Ĺ��ܺ��� ����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    //save config дvram
    //�ļ�ѡ�����˳�
    fsel_exit();

    //����ϵͳ��ʱ��
    sys_timer_exit();

    //ִ��applib���ע������
    applib_quit();

    //�ر�Ӧ�ò˵����ýű�
    com_close_confmenu_file();

    //�ر���Դ�ļ�
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    ui_set_pen_color(g_dc_status.pen_color);
    ui_set_backgd_color(g_dc_status.backgd_color);

    //save config дvram
    _save_cfg();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _scene_dispatch(music_enter_mode_e enter_mode)
 * \����������
 * \param[in]    enter_mode ����ģʽ ���ݽ���ģʽѡ���һ������ĳ���
 * \param[out]   none
 * \return       app_result_e the result music���ݸ÷���ֵȷ����һ��Ҫ������ǰ̨ap
 * \retval       RESULT_MAIN_APP ����main ap
 * \retval       RESULT_RETURN_BROWSER ����browser ap
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
app_result_e _scene_dispatch(app_param_e enter_mode)
{
    app_result_e scene_result;
    //TRUE:menu;FALSE:list
    bool list_menu = TRUE;
    //TRUE:music set;FALSE:audible set
    bool music_audible = TRUE;

    switch (g_enter_mode)
    {
        //��main menuѡ����musicͼ��
        case PARAM_FROM_MAINMENU:
        g_music_scene = SCENE_LSTMENU;
        break;

        //��browerѡ�����ļ�����//���ò����ļ�������Ϣ������
        case PARAM_FROM_RECORD:
        case PARAM_FROM_BROWSER:
        g_play_set = PLAY_START;
        g_music_config.browser_sel = TRUE;
        g_music_scene = SCENE_PLAYING;
        break;

        //��main menuѡ����playnowͼ��
        //������ap��option�˵�ѡ�������ڲ���/�ϴβ���
        case PARAM_BACKTO_PLAYING:
        case PARAM_BACKTO_PLAYED:
        g_play_set = PLAY_KEEP;
        g_music_scene = SCENE_PLAYING;
        break;

        //�������
        default:
        g_music_scene = SCENE_LSTMENU;
        break;
    }

    //��������ѭ��
    while (g_music_scene != SCENE_EXIT)
    {
        switch (g_music_scene)
        {
            //����listmenu����
            case SCENE_LSTMENU:

            scene_result = music_scene_listmenu(list_menu, &g_browser_filepath);

            switch (scene_result)
            {
                //ѡ���ļ� ���벥�ų���
                case RESULT_PLAYING_START:
                case RESULT_PLAYING_SHUFFLE:
                g_music_scene = SCENE_PLAYING;
                g_play_set = PLAY_START;
                if (scene_result == RESULT_PLAYING_SHUFFLE)
                {
                    g_shuffle_tmporary = TRUE;
                }
                else
                {
                    g_shuffle_tmporary = FALSE;
                }
                //playlistѡ���ļ�
                g_music_config.browser_sel = FALSE;
                break;

                //���벥�ų��� ���ı�״̬
                case RESULT_PLAYING_KEEP:
                g_music_scene = SCENE_PLAYING;
                g_play_set = PLAY_KEEP;
                break;

                //ѡ��audible����
                case RESULT_AUDIBLE_RESUME:
                g_music_scene = SCENE_PLAYING;
                g_play_set = PLAY_AUDIBLE_RESUME;
                if (g_file_path_info.file_source < FSEL_TYPE_SDFILE)
                {
                    g_music_config.browser_sel = TRUE;
                }
                else
                {
                    //playlistѡ���ļ�
                    g_music_config.browser_sel = FALSE;
                }
                break;

                //����listmenu�Ĳ˵��б�
                case RESULT_CARD_OUT_ERROR:
                g_music_scene = SCENE_LSTMENU;
                //ǿ���޸�music�̷�
                g_browser_filepath.file_path.dirlocation.disk = DISK_C;
                g_file_path_info.file_path.dirlocation.disk = DISK_C;
                list_menu = TRUE;
                break;

                //��������˳���������
                default:
                g_music_scene = SCENE_EXIT;
                break;
            }
            break;

            //���벥�ų���
            case SCENE_PLAYING:

            scene_result = music_scene_playing(g_enter_mode, &g_file_path_info, g_play_set);

            switch (scene_result)
            {
                //����listmenu���ļ��б�
                case RESULT_LISTMENU_LIST:
                if (g_play_set == PLAY_AUDIBLE_RESUME)
                {
                    //��audible�����˵����룬ֱ�ӷ��ز˵��б�
                    g_music_scene = SCENE_LSTMENU;
                    list_menu = TRUE;
                }
                else
                {
                    //���������ȷ����б����
                    g_music_scene = SCENE_LSTMENU;
                    list_menu = FALSE;
                }

                break;

                //����listmenu�Ĳ˵��б�
                case RESULT_CARD_OUT_ERROR:
                case RESULT_LISTMENU_MENU:
                g_music_scene = SCENE_LSTMENU;
                list_menu = TRUE;
                break;

                //����audible�����ò˵�
                case RESULT_SETMENU_AUDIBLE:
                g_music_scene = SCENE_SETMENU;
                music_audible = FALSE;
                break;

                //����music�����ò˵�
                case RESULT_SETMENU_MUSIC:
                g_music_scene = SCENE_SETMENU;
                music_audible = TRUE;
                break;

                //��������˳���������
                default:
                g_music_scene = SCENE_EXIT;
                break;
            }
            break;

            //����setmenu����
            case SCENE_SETMENU:

            scene_result = music_scene_setmenu(music_audible);

            switch (scene_result)
            {
                case RESULT_PLAYING_SCENE:
                case RESULT_PLAYING_KEEP:
                //���벥�ų��� ���ı�״̬ record���ڲ���/��һ�β���
                g_music_scene = SCENE_PLAYING;
                g_play_set = PLAY_KEEP;
                break;

                case RESULT_CARD_OUT_ERROR:
                case RESULT_LISTMENU_MENU:
                //����listmenu�Ĳ˵��б�
                g_music_scene = SCENE_LSTMENU;
                list_menu = TRUE;
                break;
                default:
                //�˳���������
                g_music_scene = SCENE_EXIT;
                break;
            }
            break;

            default:
            break;
        }
    }
    return scene_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc  const char *argv[])
 * \app��ں���
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{

    app_result_e retval;
    g_enter_mode = argc;//����music��ģʽ

    //��ʼ��
    if (_app_init() == TRUE)
    {
        retval = _scene_dispatch(argc);//����������
    }
    else
    {
        //��ʾ����
        //gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        retval = RESULT_MAIN_APP;
    }
    //���������ȴ�����
    _scene_result_deal(retval);
    _app_deinit();//�˳�
    return retval;
}
