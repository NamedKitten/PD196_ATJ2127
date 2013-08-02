/*******************************************************************************
 *                              US212A
 *                            Module: BROWSER_SCENE_BROWSING
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-8-24 9:40:29           1.0             build this file
 *******************************************************************************/
/*!
 * \file     browser_scene_browsing.c
 * \brief    browser�б���ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2011-8-24
 *******************************************************************************/
#include "browser.h"
#include "browser_menu_cfg.h"

#define BROWSER_START_INITIAL_LAYER 0   //�ӳ�ʼĿ¼��ʼ���
#define BROWSER_START_DEST_LAYER 1   //��Ŀ��㼶��ʼ���

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

static const uint8 record_short_name[] = "RECORD     ";
/******************************************************************************/
/*!
 * \par  Description:
 *    browser ��ʾ��Ϣ��ʾ
 * \param[in]    none
 * \param[out]   none
 * \return       bool �Ƿ�ɹ�����recordĿ¼
 * \retval       TRUE ����recordĿ¼
 * \retval       FALSE ����ʧ��
 * \ingroup      browser_scene_browsing
 * \note
 * \li   Ϊ��߲���Ч�ʣ�������ʹ����fsel�ӿں�������ָ���ļ���Ŀ¼��
 * 	     Ȼ��ʹ��bs�ӿڻ�ȡrecordĿ¼�����Ϣ
 *******************************************************************************/
bool _enter_record(void)
{
    bool result = TRUE;
    uint16 str_id;
    dir_brow_t brow;

    brow.name_len = 0;

    fsel_enter_dir(CD_ROOT, NULL);

    //���ҳ���Ŀ¼
    if (fsel_enter_dir(CD_SUB, (char *)record_name) == TRUE)
    {
        fsel_browser_enter_dir(CUR_DIR, 0, &brow);
        if ((brow.dir_total + brow.file_total) == 0)
        {
            result = FALSE;
        }
    }
    else
    {
        //�Ҳ���������RECORDĿ¼��Ѱ��RECORD���ļ���Ŀ¼
        if (fsel_enter_dir(CD_SUB, (char *)record_short_name) == TRUE)
        {
            fsel_browser_enter_dir(CUR_DIR, 0, &brow);
            if ((brow.dir_total + brow.file_total) == 0)
            {
                result = FALSE;
            }
        }
        else
        {
            result = FALSE;
        }
    }

    if (result == FALSE)
    {
        if (g_browser_enter_mode == PARAM_FROM_RADIO)
        {
            str_id = S_NO_RADIO_FILE;
        }
        else
        {
            str_id = S_NO_RECORD_FILE;
        }
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, str_id);
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser �б�������ģ��
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_browsing
 * \note
 * \li   ����ѡ���ļ���Ӧ�ķ���ֵ
 * \li   ���س��������ȼ�����ֵ
 *******************************************************************************/
app_result_e browser_scene_filelist(void)
{
    uint16 str_id;
    app_result_e result;
    dir_com_data_t browser_dir_data;

    //Ƕ�ײ˵��ṹ
    list_menu_t menu_item_insert;

    //Ĭ�ϵ�browser�ļ��б���û��Ƕ�ײ˵���
    browser_dir_data.root_layer = 0;
    browser_dir_data.menulist = NULL;
    browser_dir_data.menulist_cnt = 0;

    if (g_browser_enter_mode == PARAM_FROM_MAINMENU)
    {
        browser_dir_data.browse_mode = BROWSER_START_INITIAL_LAYER;
    }
    else if ((g_browser_enter_mode == PARAM_FROM_RECORD) || (g_browser_enter_mode == PARAM_FROM_RADIO))
    {
        retry:
        //��recordĿ¼����һ��Ŀ¼��ʼ���
        browser_dir_data.root_layer = 1;
        browser_dir_data.browse_mode = BROWSER_START_INITIAL_LAYER;
        if (_enter_record() == FALSE)
        {
            return RESULT_BROWSER_EXIT;
        }

        //��ʾǶ�׵Ĳ˵���"ȫ��ɾ��"
        menu_item_insert.app_id = APP_ID_BROWSER;
        menu_item_insert.layer = 1;
        menu_item_insert.list_menu = BROWSER_MENU_ENTRY_DEL_ALL;
        browser_dir_data.menulist = &menu_item_insert;
        browser_dir_data.menulist_cnt = 1;
    }
    else
    {
        //������ap����browserӦ�õ��ļ��б����
        browser_dir_data.browse_mode = BROWSER_START_DEST_LAYER;

        //֮ǰ�Ǵ�radio��record����
        if ((g_browser_var.prev_enter_mode == PARAM_FROM_RECORD) || (g_browser_var.prev_enter_mode == PARAM_FROM_RADIO))
        {
            //���֮ǰ���Ǵ�mainmenu���룬����record��FM���룬��
            //���ļ��б���ʾ"ȫ��ɾ��"�˵���
            browser_dir_data.root_layer = 1;
            //Ƕ�ײ˵�����browser ap
            menu_item_insert.app_id = APP_ID_BROWSER;
            //Ƕ�ײ˵����ļ��б�layerΪ1
            menu_item_insert.layer = 1;
            //Ƕ�ײ˵�����ڲ˵����
            menu_item_insert.list_menu = BROWSER_MENU_ENTRY_DEL_ALL;
            browser_dir_data.menulist = &menu_item_insert;
            browser_dir_data.menulist_cnt = 1;
        }
    }

    //�ļ��б�option���ص�����
    //browser_dir_data.list_option_func = _browser_show_information;
    browser_dir_data.list_option_func = browser_menu_option;

    result = gui_directory(DIRLIST, &g_browser_var.path, &browser_dir_data);

    if (result == RESULT_REDRAW)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            result = RESULT_BROWSER_EXIT;
        }
        else
        {
            result = RESULT_BROWSER_MENU;
        }
    }
    else if (result == RESULT_DIR_ERROR_NO_FILE)
    {
        if (g_browser_var.path.file_path.dirlocation.disk == DISK_BASE)
        {
            str_id = S_DISK_NOFILE;
        }
        else
        {
            str_id = S_CARD_NOFILE;
        }
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, str_id);
    }
    else if (result == RESULT_DIR_ERROR_ENTER_DIR)
    {
        //gui_directory()�ؼ�����
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        result = RESULT_BROWSER_EXIT;
    }
    else if (result == RESULT_DIR_ERROR_SETLOC)
    {
        //��ǰ�ļ�location����ʧ�ܣ���ȡ��ǰĿ¼�µ�һ���ļ�
        goto retry;
    }
    else
    {

    }

    return result;
}

