/*******************************************************************************
 *                              US212A
 *                            Module: Video
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-15 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     video_menu.c
 * \brief    video �Ĳ˵��б�ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/15
 *******************************************************************************/
#include  "video.h"
#include  "video_menu_cfg.h"

/******************************************************************************/
/*!
 * \par  Description:
 *	  video �Ƿ񴴽������б���ʾ��
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \ingroup      video_menu
 * \note
 *******************************************************************************/
static app_result_e ask_for_creat_playlist(void)
{
    app_result_e result;
    dialog_com_data_t ask_dialog_data;
    string_desc_t ask_create_str;

    uint8 old_disk = g_video_var.path.file_path.dirlocation.disk;

    //Ϊ��Ӧ���γ���Ϣ������ʱ�����̷�
    g_video_var.path.file_path.dirlocation.disk = DISK_H;

    //�Ի����Ƿ񴴽������б�
    ask_create_str.data.id = S_IS_CREATE_PLAYLIST;
    ask_create_str.language = UNICODEID;
    //�Ի�������
    ask_dialog_data.dialog_type = DIALOG_BUTTON_INCLUDE;
    ask_dialog_data.icon_id = -1;
    //�ַ���
    ask_dialog_data.string_desc = &ask_create_str;

    //��������
    ask_dialog_data.button_type = BUTTON_YESNOCANCEL;
    /*! Ĭ�ϼ������ */
    ask_dialog_data.active_default = 0x01;

    result = gui_dialog(DIALOG_ASK, &ask_dialog_data);

    g_video_var.path.file_path.dirlocation.disk = old_disk;
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �л��̷�����
 * \param[in]    disk_flag Ҫ�л����̷�
 * \param[out]   none
 * \return       the result
 * \ingroup      video_menu
 * \note         �����Ҫ���̷�����Ҫ�رպ�̨����(�Ƿ�ֻ��Ҫ�ر�music?),���³�ʼ��
 *               �ļ�ѡ����
 *******************************************************************************/
static app_result_e change_disk(uint8 disk_flag)
{
    //    fsel_error_e fsel_error;
    app_result_e result = RESULT_VIDEO_FILELIST;

    if (g_video_var.path.file_path.dirlocation.disk != disk_flag)
    {
        //�رպ�̨����
        music_close_engine();
        //�˳��ļ�ѡ����
        fsel_exit();
        //�л��̷���ʼ��location��Ĭ��ֵ
        libc_memset(&g_video_var.path.file_path, 0, sizeof(musfile_location_u));
        g_video_var.path.file_path.plist_location.disk = disk_flag;

        result = video_disk_initial();

        if (result == RESULT_NULL)
        {
            //��ʼ���ɹ������벥���б����
            result = RESULT_VIDEO_FILELIST;
        }
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ����Ŀ¼�˵���ִ�к���
 * \param[in]    param ��ʵ������
 * \param[out]   none
 * \return       the result
 * \ingroup      video_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_view_main_disk(void *param)
{
    return change_disk(DISK_C);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �忨Ŀ¼�˵���ִ�к���
 * \param[in]    param ��ʵ������
 * \param[out]   none
 * \return       the result
 * \ingroup      video_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_view_card_disk(void *param)
{
    app_result_e result;

    if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
    {
        if (get_card_plist() == FALSE)
        {
            result = ask_for_creat_playlist();
            if (result == RESULT_DIALOG_YES)
            {
                //ȷ�ϵ����
                g_video_var.path.file_path.dirlocation.disk = DISK_H;
                result = RESULT_CREATE_LIST_APP;
            }
            else if (result == RESULT_DIALOG_NO)
            {
                //ȡ�����
                result = change_disk(DISK_H);
            }
            else
            {
                ;//do nothing ϵͳ��Ϣ�˳�
            }
        }
        else
        {
            result = change_disk(DISK_H);
        }
    }
    else
    {
        //��ʾ��
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
        //���½���ͼƬ�ļ��б�
        result = RESULT_VIDEO_FILELIST;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �˵��������� �п�����ʱ�Ż���ָó���
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \retval       app_result_e, �˵��ؼ����صĽṹ��Ϣ
 * \ingroup      video_menu
 * \note
 *******************************************************************************/
app_result_e video_menu(void)
{
    app_result_e result;

    menu_com_data_t menu_data;

    //��ڲ˵�
    menu_data.app_id = APP_ID_VIDEO;
    menu_data.menu_entry = VIDEO_CARD_MENU_ENTRY;
    menu_data.path_id = 1;
    menu_data.menu_func = NULL;
    menu_data.browse_mode = 0;

    //��Ҫ��ʾ�˵���
    result = gui_menulist(MENULIST, &menu_data);
    if (result == RESULT_REDRAW)
    {
        result = RESULT_MAIN_APP;
    }
    return result;
}
