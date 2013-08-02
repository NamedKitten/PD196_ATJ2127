/*******************************************************************************
 *                              US212A
 *                            Module: VIDEO_COMFUNC
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-3-6 16:03:23           1.0              build this file
 *******************************************************************************/
/*!
 * \file     video_comfunc.c
 * \brief    video��������ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2012-3-6
 *******************************************************************************/
#include "video.h"

#define MAX_SUPPORT_STG_DEVICE 3

static const uint8 driver_type[MAX_SUPPORT_STG_DEVICE] =
{ DISK_C, DISK_H, DISK_U };

/******************************************************************************/
/*!
 * \par  Description:
 *    video��ʼ���ļ�ѡ����
 * \param[in]    disk_flag Ҫ��ʼ�����ļ�ѡ�����̷�
 * \param[out]   none
 * \return       fsel_error_e ��enhanced��ȡ���ĳ�ʼ���������ֵ
 * \retval         FSEL_NO_ERR ��ʼ���ɹ�
 * \retval         other value ��ʼ��ʧ��
 * \ingroup      video_comfunc
 * \note
 *******************************************************************************/
fsel_error_e video_fsel_initial(uint8 disk_flag)
{
    uint8 i;
    fsel_param_t video_fsel_param;
    fsel_error_e fsel_error = FSEL_NO_ERR;

    //��ʼ��videoӦ�õ��ļ�ѡ����
    video_fsel_param.fsel_type = g_video_var.path.file_source;
    //vidoeѭ��ģʽ
    video_fsel_param.fsel_mode = g_video_var.repeat_mode;
    video_fsel_param.brow_type = FSEL_BROW_ALL;
    video_fsel_param.disk = disk_flag;
    video_fsel_param.file_type_bit = g_video_var.file_ext_bitmap;

    for (i = 0; i < MAX_SUPPORT_STG_DEVICE; i++)
    {
        if (disk_flag == driver_type[i])
        {
            break;
        }
    }

    if (i == MAX_SUPPORT_STG_DEVICE)
    {
        fsel_error = FSEL_ERR_INPUT;
    }
    else
    {
        video_mount = fsel_init(&video_fsel_param, 0);

        fsel_error = fsel_get_error();
    }
    return fsel_error;

}
#if 0
/******************************************************************************/
/*!
 * \par  Description:
 *    ѯ���Ƿ񴴽������б�
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e �Ի��򷵻�ֵ
 * \retval       RESULT_DIALOG_YES ȷ��
 * \retval       RESULT_REDRAW     ȡ��ѡ��
 * \ingroup      video_comfuc
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
    ask_dialog_data.button_type = BUTTON_YESNO;
    /*! Ĭ�ϼ������ */
    ask_dialog_data.active_default = 0x01;

    result = gui_dialog(DIALOG_ASK, &ask_dialog_data);

    g_video_var.path.file_path.dirlocation.disk = old_disk;
    return result;
}
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *    video��ʼ���ļ�ѡ��������video_fsel_init()��װ
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e
 * \retval       RESULT_NULL   ��ʼ���ɹ�
 * \retval       ����ֵ        ��ʼ��ʧ��
 * \ingroup      video_comfuc
 * \note
 *******************************************************************************/
app_result_e video_disk_initial(void)
{
    fsel_error_e fsel_error;
    app_result_e result;
    uint8 disk_flag = g_video_var.path.file_path.dirlocation.disk;

    //��ʼ��browserӦ�õ��ļ�ѡ����
    fsel_error = video_fsel_initial(disk_flag);

    switch(fsel_error)
    {
        case FSEL_NO_ERR:
        result = RESULT_NULL;
        break;

        case FSEL_ERR_STG:
        case FSEL_ERR_FS:
        case FSEL_ERR_LIST_NOEXIST:
        if(fsel_error == FSEL_ERR_LIST_NOEXIST)
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_PLAYLIST);    
        }
        else
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);    
        }
        
        if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
        {
            //���ڿ���,�Ƚ��̷�����Ϊ�Ƿ�ֵ�Ա����³�ʼ���ļ�ѡ����
            g_video_var.path.file_path.dirlocation.disk = 0xff;
            result = RESULT_VIDEO_MENU;
        }
        else
        {
            //����main ap
            result = RESULT_MAIN_APP;
        } 
        break;

        default:
        result = RESULT_MAIN_APP;
        break;       
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ֪ͨ��̨�����˳�
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      video_comfunc
 * \note
 *******************************************************************************/
void music_close_engine(void)
{
    msg_apps_t msg;
    engine_type_e engine_type;

    engine_type = get_engine_type();

    if (engine_type == ENGINE_MUSIC)
    {
        //��process manager����ϢҪ���̨�˳�
        msg.type = MSG_KILL_APP_SYNC;

        msg.content.data[0] = APP_ID_MENGINE;

        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
    return;
}

