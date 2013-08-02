/*******************************************************************************
 *                              US212A
 *                            Module: BROWSER_MAIN
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-10-25 11:28:13           1.0              build this file
 *******************************************************************************/
/*!
 * \file     browser_main.c
 * \brief    browser��ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2011-10-25
 *******************************************************************************/
/* ��Ӧ�õ�ͷ�ļ� */
#include "browser.h"
#include "browser_menu_cfg.h"

/*! \brief browserӦ��VM����*/
browser_var_t g_browser_var;

/*! \brief ͨ��VM����*/
comval_t g_comval;

/*! \brief browser��һ������ */
scene_browser_e g_browser_scene_next;

/*! \brief browser �����ļ�ϵͳ��mount ID*/
int32 browser_mount;

/*! \brief Ӧ�ý���ģʽ*/
app_param_e g_browser_enter_mode;

/*! \brief �Ƿ��Ǵ�����ap����browser*/
bool g_return_browser;

/*! \brief browser�ļ��� */
uint8 browser_file_name[LIST_ITEM_BUFFER_LEN];

/*! \brief Ӧ�ÿռ����ʱ������ */
app_timer_t browser_app_timer_vector[APP_TIMER_TOTAL];

#ifndef WIN32
OS_STK *ptos = (OS_STK *) AP_BROWSER_STK_POS;
#endif
INT8U prio = AP_BROWSER_PRIO;

/*! \brief ������Դ�ļ�(common style)�� */
const uint8 com_themes[5][13] =
{ "common.sty", "cm_green.sty", "cm_gray.sty", "cm_pink.sty", "cm_red.sty" };

/*! \brief browser��Դ�ļ��� */
const uint8 browser_sty[] =
{ "browser.sty" };

/*! \brief browser�˵������ļ��� */
const uint8 browser_mcg[] =
{ "browser.mcg" };

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser ��ȡ��̨�����̷�
 * \param[in]    none
 * \param[out]   none
 * \return       disk_type
 * \ingroup      browser_main
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
 *	  browser ��ȡӦ��VMȫ�ֱ���,��ʼ��ȫ�ֱ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      browser_main
 * \note
 * \li  �����̷���ʼ��Ĭ���̷��趨����Ҫ�����Ƿ��Ǵ�����ap���أ���̨�Ƿ񲥸�
 *      �����Ƿ���ڣ�������Щ�����趨�̷�������Ĭ������Ϊ���̡�
 *******************************************************************************/
static void _browser_read_var(void)
{
    int result;
    setting_comval_t temp_val;
    engine_type_e engine_type;

    //��ȡcommon VM����������Ϣ
    result = sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));

    //��ʼ��ϵͳ��VM����
    if (temp_val.g_comval.magic != MAGIC_SETTING)
    {
        temp_val.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&temp_val);
        sys_vm_write(&temp_val, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_val.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    // ��ȡbrowser VM��������
    result = sys_vm_read(&g_browser_var, VM_AP_BROWSER, sizeof(browser_var_t));

    // ��һ�ν���AP,��ʼ��VM����
    if (g_browser_var.magic != MAGIC_BROWSER)
    {
        g_browser_var.magic = MAGIC_BROWSER;
        libc_memset(&g_browser_var.path.file_path, 0, sizeof(musfile_location_u));
        g_browser_var.path.file_path.dirlocation.disk = DISK_BASE;
        g_browser_var.prev_enter_mode = PARAM_FROM_MAINMENU;
    }

    if ((g_browser_enter_mode == PARAM_FROM_MAINMENU) || (g_browser_enter_mode == PARAM_FROM_RADIO)
            || (g_browser_enter_mode == PARAM_FROM_RECORD))
    {
        //����������룬��Ĭ�ϳ�����
        g_browser_var.path.file_path.dirlocation.disk = DISK_BASE;
        g_return_browser = FALSE;
    }
    else
    {
        //�������μ���Ƿ���ڿ���
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            //��ⲻ�����̣����ʼ����Ĭ���̷�
            g_browser_var.path.file_path.dirlocation.disk = DISK_BASE;
        }
        g_return_browser = TRUE;
    }

    //һ��Ҫǿ��д��COMMONDIR
    g_browser_var.path.file_source = FSEL_TYPE_COMMONDIR;

    //����������ȡ��ǰ�����ļ���·����Ϣ
    engine_type = get_engine_type();
    if (engine_type == ENGINE_MUSIC)
    {
        //�����̷�
        g_browser_var.path.file_path.dirlocation.disk = _get_music_disk_type();
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser ��ʼ������ĳ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      browser_main
 * \note
 * \li   ��⵽��������˵�����,��������б���
 *******************************************************************************/
static void _browser_data_init(void)
{
    if (g_return_browser == FALSE)
    {
        //�����������browser��Ҫ���ж��Ƿ��п��̣������
        //����menu�ٳ���
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            g_browser_scene_next = SCENE_FILELIST;
        }
        else
        {
            g_browser_scene_next = SCENE_MENU;
        }
    }
    else
    {
        //������ap����browserֱ�ӽ���filelist����
        g_browser_scene_next = SCENE_FILELIST;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *    browser Ӧ�õĳ�ʼ��
 * \param[in]    void
 * \param[out]   void
 * \return       app_result_e
 * \retval       RESULT_NULL  ��ʼ���ɹ�
 * \retval       ����ֵ       �ļ�ѡ������ʼ��ʧ�ܣ�����main ap
 * \ingroup      browser_main
 * \note
 *******************************************************************************/
app_result_e _browser_app_init(void)
{
    app_result_e result = TRUE;

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_BROWSER, APP_TYPE_GUI);

    //��ʼ����ʱ��
    init_app_timers(browser_app_timer_vector, APP_TIMER_TOTAL);

    //this_headbar_icon_id = HEADBAR_ICON_BROWSER;

    //��ʼ�� applib ��Ϣģ��
    applib_message_init(browser_msg_callback);

    //��ʼ��common��������ʱ�������������ڳ�ʼ������ʱ�������
    sys_timer_init();

    //��ʼui��Դ�ļ�
    ui_res_open(com_themes[g_comval.theme], UI_COM);

    ui_res_open(browser_sty, UI_AP);

    //��ǰ̨Ӧ�ò˵����ýű�
    com_open_confmenu_file(browser_mcg);

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
#endif

    //ȷ����ʼ������ĳ���
    _browser_data_init();

    //��ʼ���ļ�ѡ����
    result = browser_disk_initial();
    if (result != RESULT_NULL)
    {
        g_browser_scene_next = SCENE_EXIT;
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    browser Ӧ�õ��˳�
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \retval       1    success
 * \retval       0    fail
 * \ingroup      browser_main
 * \note
 *******************************************************************************/
bool _browser_app_deinit(void)
{
    //�ļ�ѡ�����˳�
    fsel_exit();

    //ж��common��ʱ��
    sys_timer_exit();

    //�ر�Ӧ�ò˵����ýű�
    com_close_confmenu_file();

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
 *    browser����comval VM����
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      browser_main
 * \note
 * \li  ����ģ��������������ö�ȡ�Ļ�����Ϊ512�ֽڣ��п�����ģ���������в���ȷ
 *      ��˲���buffer��ȡ��ʽ
 *******************************************************************************/
static void _browser_save_comval(void)
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
 *	  browser�������ģ��
 * \param[in]    none
 * \param[out]   none
 * \return       the result app_result_e
 * \retval       ����ѡ����ļ���׺����Ϣ��������ap
 * \ingroup      browser_main
 * \note
 * \li  �ڸ�ģ��������ӦӦ�õ�bitmap�����ļ��������ļ�����
 * \li  ��ѡ���browser���غͽ�������ap֮ǰ����Ҫ��¼��ǰ·��
 *      ʹ��prev_enter_mode��¼��һ�ν���ģʽ
 *      ʹ��enter_mode��¼��ǰ����ģʽ
 *      main/record/radion <==> browser <==> music/picture/video/ebook
 *******************************************************************************/
static app_result_e _browser_proc_result(app_result_e result)
{
    //Ӧ����Ϣ�ṹʵ��
    msg_apps_t msg;
    uint32 bit_map;
    bool need_modify_location = FALSE;
    bool need_send_msg = TRUE;
    engine_type_e engine_type = get_engine_type();

    //if (g_return_browser == FALSE)
    //{
        //g_return_browserΪFALSE�����Ǵ�������Ƚ��룬��Ҫ����
        //��ν����ģʽ
    //    g_browser_var.prev_enter_mode = g_browser_enter_mode;
    //}
    //else

    if (g_return_browser == TRUE)
    {
        //������Ǵ��������FM¼��������룬���Ǵ�����ap����browser
        //��Ҫ֪����һ������browser����ڲ���
        g_browser_enter_mode = g_browser_var.prev_enter_mode;

        //�Ժ󷵻�·������main menu
        g_browser_var.prev_enter_mode = PARAM_FROM_MAINMENU;
    }

    if (result == RESULT_BROWSER_EXIT)
    {
        if (g_browser_enter_mode == PARAM_FROM_RECORD)
        {
            result = RESULT_RECORD_MEMU;
        }
        else if(g_browser_enter_mode == PARAM_FROM_RADIO)
        {
            //�˳���FMӦ��
            result = RESULT_RADIO_MAINMENU;
        }
        else
        {
            //�˳�����Ӧ��
            result = RESULT_MAIN_APP;
        }
    }

    //��process manager���ʹ�������������Ϣ
    msg.type = MSG_CREAT_APP;
    //��Ϣ����data[1]Ϊ���ݸ�Ҫ������ap����ڲ�������������browser����
    msg.content.data[1] = (uint8) PARAM_FROM_BROWSER;

    switch (result)
    {
        case RESULT_AUDIBLE_PLAY:
        msg.content.data[0] = APP_ID_MUSIC;
        bit_map = AUDIBLE_BITMAP;
        need_modify_location = TRUE;
        g_browser_var.prev_enter_mode = g_browser_enter_mode;
        break;

        case RESULT_MUSIC_PLAY:
        msg.content.data[0] = APP_ID_MUSIC;
        if (g_browser_enter_mode != PARAM_FROM_MAINMENU)
        {
            //��record��radio����ʱ����Ĭ�ϳ�record���룬��Ϊ������music ap
            //��Ϊһ�£�����Ҫ���֡�ֻ��Ҫ�ڷ���browserʱ��browser�����ж�
            //����
            msg.content.data[1] = PARAM_FROM_RECORD;
        }
        bit_map = MUSIC_BITMAP;
        need_modify_location = TRUE;
        g_browser_var.prev_enter_mode = g_browser_enter_mode;
        break;

        case RESULT_MOVIE_PLAY:
        msg.content.data[0] = APP_ID_VIDEO;
        bit_map = VIDEO_BITMAP;
        need_modify_location = TRUE;
        g_browser_var.prev_enter_mode = g_browser_enter_mode;
        break;

        case RESULT_PICTURE_PLAY:
        msg.content.data[0] = APP_ID_PICTURE;
        bit_map = PICTURE_BITMAP;
        need_modify_location = TRUE;
        g_browser_var.prev_enter_mode = g_browser_enter_mode;
        break;

        case RESULT_EBOOK_PLAY:
        msg.content.data[0] = APP_ID_EBOOK;
        bit_map = TEXT_BITMAP;
        need_modify_location = TRUE;
        g_browser_var.prev_enter_mode = g_browser_enter_mode;
        break;

        case RESULT_RADIO_MAINMENU:
        msg.content.data[0] = APP_ID_RADIO;
        break;

        case RESULT_RECORD_MEMU:
        msg.content.data[0] = APP_ID_RECORD;
        break;

        case RESULT_NOWPLAYING:
        msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYING;
        if (engine_type == ENGINE_MUSIC)
        {
            msg.content.data[0] = APP_ID_MUSIC;
            msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYING;
        }
        else
        {
            msg.content.data[0] = APP_ID_RADIO;

        }
        break;

        case RESULT_LASTPLAY:
        msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYED;
        if (engine_type == ENGINE_MUSIC)
        {
            msg.content.data[0] = APP_ID_MUSIC;
        }
        else
        {
            msg.content.data[0] = APP_ID_RADIO;
        }
        break;

        case RESULT_APP_QUIT:
        //��commonĬ�ϴ�����¼�common�Ѿ�������Ϣ��apֱ���˳�����
        need_send_msg = FALSE;
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

    if (TRUE == need_modify_location)
    {
        //browser���ļ��������ļ�����Ӧ
        //fsel_browser_set_location(&(g_browser_var.path.file_path.dirlocation), FSEL_TYPE_COMMONDIR);
        fsel_browser_modify_location(&(g_browser_var.path.file_path.dirlocation), bit_map);
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser��һ��Ҫ���ȵĳ���
 * \param[in]    app_result_e ����ֵ
 * \param[out]   �����ķ���ֵ
 * \return       void
 * \ingroup      browser_main
 * \note
 * \li  browser����������:�˵��������б���������޿����룬��ֻ���б���
 *******************************************************************************/
void _browser_select_next_scene(app_result_e *result)
{
    app_result_e ui_result = *result;
    if (ui_result == RESULT_DIR_ERROR_NO_FILE)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
        {
            //���ڿ���
            ui_result = RESULT_BROWSER_MENU;
        }
        else
        {
            ui_result = RESULT_MAIN_APP;
        }
    }

    switch (ui_result)
    {
        case RESULT_BROWSER_MENU:
        g_browser_scene_next = SCENE_MENU;
        break;

        case RESULT_BROWSER_FILELIST:
        g_browser_scene_next = SCENE_FILELIST;
        break;

        default:
        g_browser_scene_next = SCENE_EXIT;
        break;
    }
    *result = ui_result;
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser Ӧ�õ���ں����ͳ���������ģ��
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_main
 * \note
 * \li   main�����ڽ���ʱ����argc�жϽ���ģʽ
 * \li   �ڳ�ʼ�����֮��ͽ��볡������ѭ��
 * \li   �˳�����ѭ����Է���ֵ�����жϣ���������Ӧ��Ӧ��
 *******************************************************************************/
int main(int argc, char *argv[])
{
    app_result_e result;

    g_browser_enter_mode = (app_param_e) argc;

    //����VM����
    _browser_read_var();

    //applib�ȳ�ʼ��
    result = _browser_app_init();

    // ����ѭ��
    while (g_browser_scene_next != SCENE_EXIT)
    {
        switch (g_browser_scene_next)
        {
            case SCENE_FILELIST:
            result = browser_scene_filelist();
            break;

            case SCENE_MENU:
            result = browser_scene_menu();
            break;

            default:
            g_browser_scene_next = SCENE_EXIT;
            break;
        }

        //ѡ����һ�����ȳ���
        _browser_select_next_scene(&result);
    }

    //����ؼ�����������ֵ��ת������Ϣ����
    _browser_proc_result(result);

    //����AP VM����
    sys_vm_write(&g_browser_var, VM_AP_BROWSER);

    //����ϵͳ VM����
    _browser_save_comval();

    //applib���˳�����
    _browser_app_deinit();
    return result;
}

