/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     pic_main.c
 * \brief    picture��ģ�飬������̳�ʼ�����˳�������������
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "picture.h"
#include "picture_preview.h"
#include "music_common.h"
#include "pic_menu_cfg.h"

// ȫ�ֱ�������

/*! \brief pictureӦ��VM����*/
g_picture_var_t g_picture_var;

/*! \brief ͨ��VM����*/
comval_t g_comval;

/*! \brief picture �����ļ�ϵͳ��mount ID*/
int32 picture_mount;

#ifndef PC
/*! \brief pictureʹ�õĶ�ջָ�� */
OS_STK *ptos = (OS_STK *) AP_PICTURE_STK_POS;
#endif

/*! \brief pictureʹ�õĽ������ȼ� */
INT8U prio = AP_PICTURE_PRIO;

/*! \brief ����codec��ͼƬwindow */
wregion_t region_in;

/*! \brief ʵ�ʽ����õ�ͼƬwindow */
wregion_t region_out;

/*! \brief ͼƬ�����߳̾�� */
void *picture_handle;

/*! \brief ����codec��ͼƬ������Ϣ */
ColorLcmInfor g_picture_userinfor;

/*! \brief Ӧ�ý���ģʽ*/
app_param_e g_picture_enter_mode;

/*! \brief �Զ����Ż������ʾ�ļ����Ķ�ʱ��ID */
int8 timer_pic_play;

/*! \brief �Զ������л��ļ���־ */
uint8 g_picture_change_file;

/*! \brief �Զ����ű�־ */
bool g_picture_auto_play;

/*! \brief �ļ�����Ҫ������ʾ��־ */
bool g_picture_scroll_flag;

/*! \brief picture��һ������ */
scene_picture_e g_picture_scene_next;

/*! \brief picture��һ������*/
scene_picture_e g_picture_scene_prev;

/*! \brief ��Ҫ��������music�����־ */
bool g_need_resume_engine;

/*! \brief ͼƬ������Ƿ���б�־ */
bool g_mmm_id_free;

/*! \brief �ļ��������� */
uint8 g_picture_filename[LIST_ITEM_BUFFER_LEN];

/*! \brief �ļ�ѡ�����Ƿ��ʼ�� */
bool is_fsel_init;

/*! \brief �Ƿ�ر�music��̨ */
uint8 g_stop_music;

/*! \brief �ļ����� */
uint16 g_file_total;

/*! \brief �����ļ�����*/
uint16 g_file_err;

/*! \brief �Ƿ�ɾ���ļ�*/
uint8  g_del_file;

/*! \brief Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������ */
app_timer_t picture_app_timer_vector[APP_TIMER_TOTAL];

/*! \brief ������Դ�ļ�(common style)�� */
const uint8 com_themes[5][13] =
{ "common.sty", "cm_green.sty", "cm_gray.sty", "cm_pink.sty", "cm_red.sty" };

/*! \brief picture��Դ�ļ��� */
const uint8 picture_sty[] =
{ "picture.sty" };

/*! \brief picture�˵������ļ��� */
const uint8 picture_mcg[] =
{ "picture.mcg" };

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��̨�������ļ����̷�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_main
 * \note
 *******************************************************************************/
static uint8 _get_music_disk_type(void)
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
 *	  picture ��ȡӦ��VMȫ�ֱ���,��ʼ��ȫ�ֱ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_main
 * \note
 *******************************************************************************/
static void _pic_read_var(void)
{
    int result;
    setting_comval_t temp_val;
    engine_type_e engine_type = get_engine_type();

    /*��ȡcommon VM����������Ϣ*/
    result = sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));

    /*��ʼ��ϵͳ��VM����*/
    if (temp_val.g_comval.magic != MAGIC_SETTING)
    {
        temp_val.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&temp_val);
        sys_vm_write(&temp_val, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_val.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    //��ȡphoto VM����������Ϣ
    result = sys_vm_read(&g_picture_var, VM_AP_PICTURE, sizeof(g_picture_var));

    //��һ�ν���AP����ʼ��VM����
    if (g_picture_var.magic != MAGIC_PICTURE)
    {
        g_picture_var.magic = MAGIC_PICTURE;

        //��ȡ����ֵ���Ƿ�֧������ͼ
        g_picture_var.thumbnail_flag = (uint8) com_get_config_default(PHOTO_AP_ID_THUMBNAIL);
        g_picture_var.revolve_flag = (uint8) com_get_config_default(PHOTO_AP_ID_REVOLVE);
        libc_memset(&g_picture_var.path.file_path, 0, sizeof(musfile_location_u));
        g_picture_var.path.file_path.dirlocation.disk = DISK_BASE;
        g_picture_var.prev_enter_mode = PARAM_FROM_MAINMENU;
    }

    if (g_picture_enter_mode == PARAM_FROM_PLAYLIST)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            //��ⲻ�����̣����ʼ����Ĭ���̷�
            g_picture_var.path.file_path.dirlocation.disk = DISK_BASE;
        }
    }
    else
    {
        g_picture_var.path.file_path.dirlocation.disk = DISK_C;
    }

    if (g_picture_enter_mode == PARAM_FROM_BROWSER)
    {
        //��browser��recode����
        sys_vm_read(&g_picture_var.path, VM_AP_BROWSER, sizeof(file_path_info_t));
    }

    //����������ȡ��ǰ�����ļ���·����Ϣ
    if (engine_type == ENGINE_MUSIC)
    {
        //�����̷�
        g_picture_var.path.file_path.dirlocation.disk = _get_music_disk_type();
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    picture Ӧ�÷���ֵ������������Ӧ�ķ���ֵת������Ϣ����
 * \param[in]    result app_result_e ������������ֵ
 * \param[out]   none
 * \return       the result
 * \ingroup      pic_main
 * \note
 * \li   RESULT_RETURN_BROWSER ����browserӦ��
 * \li   RESULT_NOWPLAYING/RESULT_LASTPLAY     ����music/radio��Ӧ��ǰ̨UI����
 * \li   RESULT_CREATE_LIST_APP ���벥���б�ap����ͼƬ�����б�
 * \li   RESULT_APP_QUIT        ��common������¼��᷵�ظ÷���ֵ��picture����������
 * \li   ��������ֵ             ����main
 *******************************************************************************/
static app_result_e _deal_picture_result(app_result_e pic_result)
{
    msg_apps_t msg;
    bool need_send_msg = TRUE;
    engine_type_e engine_type = get_engine_type();

    //��process manager���ʹ�������������Ϣ
    msg.type = MSG_CREAT_APP;
    msg.content.data[1] = (uint8) PARAM_FROM_PICTURE;

    switch (pic_result)
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

        }
        break;

        case RESULT_CREATE_LIST_APP:
        msg.content.data[0] = APP_ID_PLAYLIST;
        //ֻ���µ�ǰ�̷���playlist
        if (g_picture_var.path.file_path.dirlocation.disk == DISK_C)
        {
            msg.content.data[1] = (PLIST_DISK_C | PARAM_FROM_PICTURE);
        }
        else
        {
            msg.content.data[1] = (PLIST_DISK_H | PARAM_FROM_PICTURE);
        }
        //�������ģʽ
        g_picture_var.prev_enter_mode = g_picture_enter_mode;
        break;

        case RESULT_APP_QUIT:
        need_send_msg = FALSE;
        //�������ģʽ
        g_picture_var.prev_enter_mode = g_picture_enter_mode;
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
    return pic_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    picture Ӧ�õĳ�ʼ��
 * \param[in]    void
 * \param[out]   void
 * \return       the result
 * \retval       1  success
 * \retval       0  fail
 * \ingroup      pic_main
 * \note
 *******************************************************************************/
app_result_e _pic_app_init(void)
{
    app_result_e result = RESULT_NULL;

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_PICTURE, APP_TYPE_GUI);

    //��ʼ����ʱ��
    init_app_timers(picture_app_timer_vector, APP_TIMER_TOTAL);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init(pic_msg_callback);

    //��ʼ��common��������ʱ�������������ڳ�ʼ������ʱ�������
    sys_timer_init();

    //��ʼui��Դ�ļ�
    ui_res_open(com_themes[g_comval.theme], UI_COM);

    ui_res_open(picture_sty, UI_AP);

    //��ǰ̨Ӧ�ò˵����ýű�
    com_open_confmenu_file(picture_mcg);

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
#endif

    result = pic_data_init();
    if (result != RESULT_NULL)
    {
        is_fsel_init = FALSE;
        g_picture_scene_next = SCENE_EXIT;
        return result;
    }

    result = pic_disk_initial();
    if (result == RESULT_MAIN_APP)
    {
        g_picture_scene_next = SCENE_EXIT;
        return result;
    }
    else if(result == RESULT_PIC_MENU)
    {
        g_picture_scene_next = SCENE_MENU;
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
 *    picture Ӧ�õ��˳�����
 * \param[in]    void
 * \param[out]   void
 * \return       the result
 * \retval       1  success
 * \retval       0  fail
 * \ingroup      pic_main
 * \note
 *******************************************************************************/
bool _pic_app_deinit(void)
{
    //�ļ�ѡ�����˳�
    if (is_fsel_init == TRUE)
    {
        fsel_exit();
    }
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
 *    picture ����comval VM����
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      picture
 * \note
 *******************************************************************************/
static void _pic_save_comval(void)
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
 *	  picture��һ��Ҫ���ȵĳ���
 * \param[in]    app_result_e ����ֵ
 * \param[out]   �����ķ���ֵ
 * \return       void
 * \ingroup      pic_main
 * \note
 * \li  picture��5������:�ļ��б�/����ͼ����,option�˵���������ų�����
 �˵�����(�п�ʱ����)
 *******************************************************************************/
static void _pic_select_next_scene(app_result_e *result)
{
    app_result_e ui_result = *result;
    g_picture_scene_prev = g_picture_scene_next;

    if (ui_result == RESULT_DIR_ERROR_NO_FILE)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
        {
            //���ڿ���
            ui_result = RESULT_PIC_MENU;
        }
        else
        {
            //����main ap
            ui_result = RESULT_MAIN_APP;
        }
    }
    else if( ui_result == RESULT_CARD_OUT_ERROR)
    {
        if(g_picture_enter_mode == PARAM_FROM_BROWSER)
        {
            ui_result = RESULT_RETURN_BROWSER;
        }
        else
        {
            ui_result = RESULT_MAIN_APP;
        }    
    }
    else
    {
        ;//nothing
    }
    
    switch (ui_result)
    {
        case RESULT_PIC_MENU:
        g_picture_scene_next = SCENE_MENU;
        break;

        case RESULT_PIC_OPTIONMENU:
        g_picture_scene_next = SCENE_OPTIONMENU;
        break;

        case RESULT_PIC_FILELIST:
        if ((g_picture_var.thumbnail_flag & VALUE_PREVIEW_MODULE_ON) == VALUE_PREVIEW_MODULE_ON)
        {
            g_picture_scene_next = SCENE_PREVIEW;
        }
        else
        {
            g_picture_scene_next = SCENE_FILELIST;
        }
        break;

        case RESULT_PICTURE_PLAY:
        g_picture_scene_next = SCENE_PLAY;
        break;

        default:
        g_picture_scene_next = SCENE_EXIT;
        break;
    }
    *result = ui_result;
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture Ӧ�õ���ں����ͳ���������ģ��
 * \param[in]    argc �ɽ��̹���������Ĳ�������ʾ�Ӻ���ģʽ����
 * \param[in]    argv ���ò���
 * \param[out]   none
 * \return       the result ����Ӧ�÷���ֵ�Ѿ��������ﷵ��ֵ���Ժ���
 * \ingroup      pic_main
 * \note
 * \li   main�����ڽ���ʱ����argc�жϽ���ģʽ
 * \li   �ڳ�ʼ�����֮��ͽ��볡������ѭ��
 * \li   �˳�����ѭ����Է���ֵ�����жϣ���������Ӧ��Ӧ��
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    DC dc_status;
    app_result_e result;

    //��λ�֪����ģʽ?
    g_picture_enter_mode = (app_param_e) argc;

    //����VM����
    _pic_read_var();

    ui_get_DC_status(&dc_status);
    ui_set_pen_color(Color_WHITE_def);
    ui_set_backgd_color(Color_BLACK_def);

    result = _pic_app_init();

    /*����������*/
    while (g_picture_scene_next != SCENE_EXIT)
    {
        switch (g_picture_scene_next)
        {
            case SCENE_FILELIST:
            result = pic_filelist();
            break;

            case SCENE_OPTIONMENU:
            result = pic_option_menulist();
            break;

            case SCENE_PLAY:
            result = pic_play();
            break;

            case SCENE_PREVIEW:
            result = pic_preview();
            break;

            case SCENE_MENU:
            result = pic_menu();

            default:
            g_picture_scene_next = SCENE_EXIT; //exit
            break;
        }

        //ѡ����һ�����ȳ���
        _pic_select_next_scene(&result);
    }

    ui_set_pen_color(dc_status.pen_color);
    ui_set_backgd_color(dc_status.backgd_color);

    //������ֵת��Ϊ��Ϣ�������̹�����
    _deal_picture_result(result);
    //����AP VM����
    sys_vm_write(&g_picture_var, VM_AP_PICTURE);
    //����ϵͳ VM����
    _pic_save_comval();
    //applib���˳�����
    _pic_app_deinit();

    return result;
}

