/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     pic_filelist.c
 * \brief    Picture��filelist����ģ�飬����common_ui�Ŀؼ�����ʾ�ļ��б���Ϣ
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "picture.h"

/*ȫ�ֱ�������*/
/*��������*/
/******************************************************************************/
/*!
 * \par  Description:
 *	  ͼƬ�ļ��б�option�ص�����,�÷���ֵ�ڳ���ѭ���������ж�
 * \param[in]    path_info ��ǰ�ļ���path_info,���ﲻ��Ҫ
 * \param[in]    active    ��ǰ�ļ����б���ţ����ﲻ��Ҫ
 * \param[out]   path_info
 * \return       app_result_e
 * \retval       RESULT_PIC_OPTIONMENU ���ص�option�˵�����
 * \ingroup      pic_filelist
 * \note
 *******************************************************************************/
static app_result_e pic_option_callback(file_path_info_t *path_info, uint16 active)
{
    return RESULT_PIC_OPTIONMENU;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ͼƬ�ļ��б�����ֱ�ӵ���common��filelist�ؼ�
 * \param[in]    void
 * \param[out]   none
 * \return       the result filelist�ؼ����ؽ��
 * \ingroup      pic_filelist
 * \note
 *******************************************************************************/
app_result_e pic_filelist(void)
{
    app_result_e result;
    dir_com_data_t pic_dir_data;

    if (g_picture_var.path.file_path.dirlocation.filename[0] == 0)
    {
        //�Ӹ�Ŀ¼��ʼ���
        pic_dir_data.browse_mode = 0;
    }
    else
    {
        //���ϴ�����˳��ĵط���ʼ���
        pic_dir_data.browse_mode = 1;
    }

    if(g_del_file == TRUE)
    {
        g_del_file = FALSE;
        pic_dir_data.del_back = TRUE;
    }

    //pic�ļ��б����˵���
    pic_dir_data.menulist_cnt = 0;
    pic_dir_data.root_layer = 0;
    //option���ص�����
    pic_dir_data.list_option_func = pic_option_callback;
    //�˵��б�Ϊ��
    pic_dir_data.menulist = NULL;

    result = gui_directory(DIRLIST, &g_picture_var.path, &pic_dir_data);
    if (result == RESULT_REDRAW)
    {
        //��filelist��return�����ж��Ƿ���ڿ���
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            result = RESULT_MAIN_APP;
        }
        else
        {
            result = RESULT_PIC_MENU;
        }
    }
    else if (result == RESULT_DIR_ERROR_ENTER_DIR)
    {
        //�ؼ�����
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        result = RESULT_MAIN_APP;
    }
    else if (result == RESULT_DIR_ERROR_NO_FILE)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_PIC_FILE);
    }
    else
    {

    }
    return result;
}




