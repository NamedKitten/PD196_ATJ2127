/*******************************************************************************
 *                              US212A
 *                            Module: BROWSER_SCENE_MENU
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-3-29 9:47:16           1.0              build this file
 *******************************************************************************/
/*!
 * \file     browser_scene_menu.c
 * \brief    browser�˵�ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2012-3-29
 *******************************************************************************/
#include "browser.h"
#include "browser_menu_cfg.h"

/*! \brief recordĿ¼���ļ������� */
static const uint8 record_name[] =
{
    0xff, 0xfe,  //flag
    'R',  0x00,
    'E',  0x00,
    'C',  0x00,
    'O',  0x00,
    'R',  0x00,
    'D',  0x00,
    0x00, 0x00  //end flag
};

file_path_info_t temp_file_path _BANK_DATA_ATTR_;


extern bool _enter_record(void)
__FAR__;
/******************************************************************************/
/*!
 * \par  Description:
 *	  �л��̷������³�ʼ���ļ�ѡ����
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
static app_result_e _browser_filelist_init(uint8 diskflag)
{
    fsel_error_e fsel_error;
    app_result_e result = RESULT_BROWSER_FILELIST;

    uint8 *cur_disk = &(g_browser_var.path.file_path.dirlocation.disk);

    if (*cur_disk != diskflag)
    {
        //�ر���������
        music_close_engine();

        //�˳���ǰ�ļ�ѡ����
        fsel_exit();

        //��ʼ��locationΪ��Чlocation
        libc_memset(&g_browser_var.path.file_path, 0, sizeof(musfile_location_u));

        //�����̷�
        *cur_disk = diskflag;

        //��ʼ��pictureӦ�õ��ļ�ѡ����
        fsel_error = browser_fsel_initial(*cur_disk);

        if (fsel_error == FSEL_NO_ERR)
        {
            result = RESULT_BROWSER_FILELIST;
        }
        else
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
            result = RESULT_MAIN_APP;
            g_browser_scene_next = SCENE_EXIT;
        }
    }

    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  ����Ŀ¼�˵��ص����� ��ʼ���ļ�ѡ����
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_main_disk(void *param)
{
    return _browser_filelist_init(DISK_C);
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  �忨Ŀ¼�˵��ص����� ��ʼ���ļ�ѡ����
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_card_disk(void *param)
{
    if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
    {
        return _browser_filelist_init(DISK_H);
    }
    else
    {
        //��ʾ��
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);

        //���½����ļ��б�
        return RESULT_BROWSER_FILELIST;
    }
}
#if 0
/******************************************************************************/
/*!
 * \par  Description:
 *	  ����ɾ���ļ�����
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
static void _delete_all_file(dir_brow_t *browser_dir_browser, file_path_info_t *path_info)
{
    bool result;
    scanfile_result_e browser_result;

    //��ʾ����ɾ���ļ��ĶԻ���
    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_DEL_FILE_ING);

    //�ļ��б��������ʼ��
    fsel_browser_scanfile_init(browser_dir_browser);

    //�����ļ��в�ɾ���ļ������ļ���
    while (1)
    {
        browser_result = fsel_browser_scanfile(path_info, browser_dir_browser, SCANFILE_TYPE_DELETE);
        if (browser_result == SCANFILE_RESULT_FILE)//�����ļ�

        {
            //ɾ����ǰ�ļ�
            result = fsel_browser_delete(BROW_DEL_FILE, 0, &(path_info->file_path));
        }
        else//�����ļ��У��������ļ��� ,���ļ���, �ļ�������

        {
            //����ļ���Ϊ�գ�ɾ��֮
            if (browser_result == SCANFILE_RESULT_PARENTDIR)
            {
                //���ص���Ŀ¼�������Ŀ¼����ɾ��
                result = fsel_browser_delete(BROW_DEL_DIR, 1, &(path_info->file_path));
                //���µ�ǰ���ļ�Ŀ¼����
                browser_dir_browser->dir_total--;
            }

            if (browser_result == SCANFILE_RESULT_SELF)//�ļ�������

            {
                //ɾ�������ļ���ϣ���ʾ��ɾ���������˳�Ӧ��
                deleted_file_display("RECORD");
                return;
            }
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser �رպ�̨������ʾ����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 * \li   ��ȫ��ɾ���˵���ʾ�Ƿ�رպ�̨����
 *******************************************************************************/
static app_result_e ask_for_close_mengine(void)
{
    app_result_e result;
    dialog_com_data_t ask_dialog_data;
    string_desc_t ask_create_str;

    //�Ի����Ƿ񴴽������б�
    ask_create_str.data.id = S_CLOSE_MUSIC_DEL;
    ask_create_str.language = UNICODEID;
    //�Ի�������
    ask_dialog_data.dialog_type = DIALOG_BUTTON_INCLUDE;
    ask_dialog_data.icon_id = -1;
    //�ַ���
    ask_dialog_data.string_desc = &ask_create_str;

    //��������
    ask_dialog_data.button_type = BUTTON_YESNO;
    /*! Ĭ�ϼ������ */
    ask_dialog_data.active_default = 0x01;

    result = gui_dialog(DIALOG_ASK, &ask_dialog_data);

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser ɾ��recordĿ¼����Ŀ¼����ʾ����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
static app_result_e _ask_for_del_all(void)
{
    app_result_e result;
    dialog_com_data_t ask_dialog_data;
    string_desc_t ask_create_str;

    //�Ի����Ƿ�ɾ���ļ�
    if ((g_browser_var.prev_enter_mode == PARAM_FROM_RADIO) || (g_browser_enter_mode == PARAM_FROM_RADIO))
    {
        ask_create_str.data.id = S_DEL_RADIO_FILE;
    }
    else
    {
        ask_create_str.data.id = S_DEL_RECORD_FILE;
    }

    ask_create_str.language = UNICODEID;
    //�Ի�������
    ask_dialog_data.dialog_type = DIALOG_BUTTON_INCLUDE;
    ask_dialog_data.icon_id = -1;
    //�ַ���
    ask_dialog_data.string_desc = &ask_create_str;

    //��������
    ask_dialog_data.button_type = BUTTON_YESNO;
    //Ĭ�ϼ������
    ask_dialog_data.active_default = 0x01;

    result = gui_dialog(DIALOG_ASK, &ask_dialog_data);

    return result;
}
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *	  ����ɾ���ļ�����
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
static app_result_e _delete_all_file(file_path_info_t *path_info)
{
    app_result_e result;
    del_com_data_t del_com;

    libc_memcpy(browser_file_name, record_name, sizeof(record_name));

    //����common�ؼ�ɾ��֮
    del_com.del_no = 0;
    del_com.filename = browser_file_name;
    del_com.del_playing = FALSE;
    del_com.del_func = NULL;
    //����ɾ���ڵ�
    del_com.del_self = FALSE;
    result = gui_delete(path_info, &del_com);

    switch (result)
    {
        //�ɹ�ɾ��
        case RESULT_DELETE_DIRLIST_NOSELF:
        result = RESULT_BROWSER_EXIT;
        break;

        //�޷�ɾ��
        case RESULT_ERROR:
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DEL_FAILED);
        //���½���recordĿ¼
        _enter_record();
        result = RESULT_REDRAW;
        break;

        default:
        break;
    }

    return result;

}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browserȫ��ɾ��Ҷ�Ӳ˵����
 * \param[in]    param  no use
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_del_all(void *param)
{
    app_result_e ui_result;
    bool result;

    dir_brow_t brow;

    //����ȡĿ¼��
    brow.name_len = 0;
    //��ʼ������Ŀ¼����
    result = fsel_browser_enter_dir(CUR_DIR, 0, &brow);

    if (result == TRUE)
    {
        //�ɹ������Ŀ¼
        ui_result = _delete_all_file(&g_browser_var.path);
    }
    else
    {
        ui_result = RESULT_DIR_ERROR_ENTER_DIR;
    }
    return ui_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser���ڲ���Ҷ�Ӳ˵����
 * \param[in]    param  no use
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_playing(void * param)
{
    return RESULT_NOWPLAYING;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser��һ�β���Ҷ�Ӳ˵����
 * \param[in]    param  no use
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_last_play(void * param)
{
    return RESULT_LASTPLAY;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browserɾ��Ҷ�Ӳ˵����
 * \param[in]    param  no use
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_del_file(void *param)
{
    bool ret_val;
    app_result_e result;
    del_com_data_t del_com;
    dir_brow_t brow;
    brow_decode_t decode_param;
    file_brow_t file_list;

    //�ļ���ŵ��ڵ�ǰ���ļ���������ż�ȥǶ�ײ˵�����
    uint16 list_num = dir_control.list_no - dir_menu_title.count;

    brow.name_buf = browser_file_name;
    brow.name_len = LIST_ITEM_BUFFER_LEN;

    //ȷ��ɾ��ģʽ:�ļ��л��ļ�
    fsel_browser_enter_dir(CUR_DIR, 0, &brow);

    //�����ǰ�ļ����С��Ŀ¼����������ɾ���ļ��м����ļ�
    if (list_num <= brow.dir_total)
    {
        del_com.del_mode = DEL_MODE_LIST;

        fsel_browser_select(list_num);

        fsel_browser_get_location(&(g_browser_var.path.file_path.dirlocation), g_browser_var.path.file_source);

        libc_memcpy(&temp_file_path, &g_browser_var.path, sizeof(file_path_info_t));

    }
    else
    {
        del_com.del_mode = DEL_MODE_FILE;
    }

    //��ȡҪɾ��Ŀ¼���ļ���
    fsel_browser_select(list_num);
    decode_param.option = 1;
    decode_param.num = 1;
    decode_param.top = list_num;
    file_list.name_buf = browser_file_name;
    file_list.name_len = LIST_ITEM_BUFFER_LEN;
    fsel_browser_get_items(&decode_param, &file_list);

    //��λ��ɾ���ļ��л��ļ�
    if (del_com.del_mode == DEL_MODE_LIST)
    {
        //���뵱ǰĿ¼
        ret_val = fsel_browser_enter_dir(SON_DIR, list_num, &brow);
        if(ret_val == FALSE)
        {
            //�����˼�Ŀ¼���޷�ɾ��
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DEL_FAILED);
            return RESULT_REDRAW;
        }
    }
    else
    {
        //��λ����ɾ���ļ�
        fsel_browser_get_location(&g_browser_var.path.file_path, g_browser_var.path.file_source);
        fsel_browser_set_file(&g_browser_var.path.file_path, g_browser_var.path.file_source);
    }

    //����common�ؼ�ɾ��֮
    del_com.del_no = list_num;
    del_com.filename = browser_file_name;
    del_com.del_playing = FALSE;
    del_com.del_func = NULL;
    //����ɾ���ڵ�
    del_com.del_self = TRUE;
    result = gui_delete(&g_browser_var.path, &del_com);

    if (del_com.del_mode == DEL_MODE_LIST)
    {
        //���ɾ��ģʽΪɾ���ļ��У���������û�в������򷵻���һ��Ŀ¼
        if (result == RESULT_REDRAW)
        {
            //��Ϊ֮ǰ���뵱ǰ��Ŀ¼������Ҫ���·���
            fsel_browser_enter_dir(PARENT_DIR, 0, &brow);
        }
        else if (result == RESULT_ERROR)
        {
            //�����˼�Ŀ¼���޷�ɾ��
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DEL_FAILED);
            fsel_browser_set_location(&(temp_file_path.file_path.dirlocation), temp_file_path.file_source);
            result = RESULT_REDRAW;
        }
        else
        {
            ;//nothing
        }
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser option�������Ĳ˵��ؼ���option���ص�����
 * \param[in]    param  no use
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_option_option(void)
{
    //������һ��
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    browser ��ʾ��Ϣ��ʾ
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_browsing
 * \note
 *******************************************************************************/
app_result_e _browser_show_information(void)
{
    return gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_OPTION_DESC);
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser ui_directory()�ؼ�������ui_menulist()�ؼ���������Ϊ�ļ��б��option
 *	          ���ص�����������
 * \param[in]    path_info  ui_directory()�ؼ����ݵ�path_info
 * \param[in]    active     ��Ӧ�ļ��������
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 * \li  �ú���Ϊui_directory()��option���ص�����
 *******************************************************************************/
app_result_e browser_menu_option(file_path_info_t *path_info, uint16 active)
{
    app_result_e result;
    engine_state_e play_sta;
    //��ʼ���˵��ؼ�
    menu_com_data_t op_menu;

    //һ���ļ��б�����Ǵ�1��ʼ������ֻ�г���
    //Ƕ�ײ˵�ʱ�Ż�ֵΪ0
    if (active == 0)
    {
        //����record�ĵ�һ���˵����ֻ������ʾ��
        return _browser_show_information();
    }

    play_sta = get_engine_state();

    //���岻ͬģʽ�µ���ڲ˵�
    if (play_sta == ENGINE_STATE_NULL)
    {
        op_menu.menu_entry = BROWSER_MENU_ENTRY_NOPLAY;
    }
    else if (play_sta == ENGINE_STATE_PLAYING)
    {
        op_menu.menu_entry = BROWSER_MENU_ENTRY_PLAYING;
    }
    else
    {
        op_menu.menu_entry = BROWSER_MENU_ENTRY_LASTPLAY;
    }

    op_menu.app_id = APP_ID_BROWSER;
    //һ��ap�����ж���˵�������Ϊÿ���˵��������������·������
    op_menu.path_id = 1;
    op_menu.menu_func = NULL;
    op_menu.browse_mode = 0;

    result = gui_menulist(MENULIST_OPTION, &op_menu);

    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser �˵���������ģ��
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_menu
 * \note
 * \li  ��ʾ����Ŀ¼�Ͳ忨Ŀ¼
 *******************************************************************************/
app_result_e browser_scene_menu(void)
{
    app_result_e result;

    menu_com_data_t browser_menu;

    browser_menu.app_id = APP_ID_BROWSER;

    //browser���д���ڲ˵�
    browser_menu.menu_entry = BROWSER_MENU_ENTRY_CARD;
    browser_menu.path_id = 0;
    browser_menu.menu_func = NULL;
    browser_menu.browse_mode = 0;

    //��Ҫ��ʾ�˵���
    result = gui_menulist(MENULIST, &browser_menu);
    if (result == RESULT_REDRAW)
    {
        result = RESULT_BROWSER_EXIT;
    }
    return result;
}

