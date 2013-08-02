/*******************************************************************************
 *                              US212A
 *                            Module: Video
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     video
 * \brief    video��filelist����ģ�飬����common_ui�Ŀؼ�����ʾ�ļ��б���Ϣ
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "video.h"

/*ȫ�ֱ�������*/

/*��������*/

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��Ƶ�ļ��б�option�ص�����,�÷���ֵ�ڳ���ѭ���������ж�
 * \param[in]    path_info ��ǰ�ļ���path_info,���ﲻ��Ҫ
 * \param[in]    active    ��ǰ�ļ����б���ţ����ﲻ��Ҫ
 * \param[out]   path_info
 * \return       app_result_e
 * \retval       RESULT_VIDEO_MENU ���ص�option�˵�����
 * \ingroup      video_filelist
 * \note
 *******************************************************************************/
static app_result_e _video_option_callback(file_path_info_t *path_info, uint16 active)
{
    return RESULT_VIDEO_OPTIONMENU;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��Ƶ�ļ��б�ģ��
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MAIN_APP  �ص���Ӧ��
 * \retval       RESULT_VIDEO_MENU ����video�˵�����
 * \retval       RESULT_VIDEO_OPTIONMENU ����video option�˵�����
 * \ingroup      video_filelist
 * \note
 *******************************************************************************/
app_result_e video_filelist(void)
{

    app_result_e result;
    dir_com_data_t video_dir_data;

    if (g_video_var.path.file_path.dirlocation.filename[0] == 0)
    {
        video_dir_data.browse_mode = 0;
    }
    else
    {
        video_dir_data.browse_mode = 1;
    }

    if(g_del_file == TRUE)
    {
        g_del_file = FALSE;
        video_dir_data.del_back = TRUE;
    }

    ui_clear_screen(NULL);
    //�ļ��б����˵���
    video_dir_data.menulist_cnt = 0;
    video_dir_data.root_layer = 0;
    //option���ص�����
    video_dir_data.list_option_func = _video_option_callback;
    //�˵��б�Ϊ��
    video_dir_data.menulist = NULL;

    result = gui_directory(DIRLIST, &g_video_var.path, &video_dir_data);

    switch(result)
    {
        case RESULT_REDRAW:
        //��filelist��return�����ж��Ƿ���ڿ���
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            result = RESULT_MAIN_APP;
        }
        else
        {
            result = RESULT_VIDEO_MENU;
        }  
        break;

        case RESULT_DIR_ERROR_ENTER_DIR:
        //�ؼ�����
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        break;

        case RESULT_DIR_ERROR_NO_FILE:
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_VIDEO_FILE);
        break;

        default:
        break;
    }
    
    return result;
}


