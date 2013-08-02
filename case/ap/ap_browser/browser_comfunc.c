/*******************************************************************************
 *                              US212A
 *                            Module: BROWSER_COMFUNC
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-3-7 9:16:42           1.0              build this file
 *******************************************************************************/
/*!
 * \file     browser_comfunc.c
 * \brief    browser����ģ�麯��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2012-3-7
 *******************************************************************************/
#include "browser.h"

#define MAX_SUPPORT_STG_DEVICE 3

/*! \brief ֧����������洢���ʵ��̷� */
static const uint8 driver_type[MAX_SUPPORT_STG_DEVICE] =
{ DISK_C, DISK_H, DISK_U };

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser�ļ�ѡ������ʼ��
 * \param[in]    uint8 disk_flag �̷�
 * \param[out]   none
 * \return       fsel_error_e �ļ�ѡ������ʼ������ֵ
 * \ingroup      browser_comfunc
 * \note
 * \li  �����ļ�ѡ�����ӿڳ�ʼ���������ز������
 *******************************************************************************/
fsel_error_e browser_fsel_initial(uint8 disk_flag)
{
    uint8 i;
    fsel_param_t browser_fsel_param;
    fsel_error_e fsel_error = FSEL_NO_ERR;

    browser_fsel_param.fsel_type = FSEL_TYPE_COMMONDIR;
    browser_fsel_param.fsel_mode = FSEL_MODE_BROWSER;
    browser_fsel_param.brow_type = FSEL_BROW_ALL;
    browser_fsel_param.disk = disk_flag;
    browser_fsel_param.file_type_bit = ALLFILE_BITMAP;

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
        browser_mount = fsel_init(&browser_fsel_param, 0);

        fsel_error = fsel_get_error();
    }
    return fsel_error;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser��ʼ���ļ�ѡ����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e ������
 * \ingroup      browser_comfunc
 * \note
 * \li  ���ڶ�browser_fsel_initial()��һ����װ�����ؽ����app_result_e����
 *******************************************************************************/
app_result_e browser_disk_initial(void)
{
    fsel_error_e fsel_error;
    app_result_e result;
    uint8 disk_flag = g_browser_var.path.file_path.dirlocation.disk;

    //��ʼ��browserӦ�õ��ļ�ѡ����
    fsel_error = browser_fsel_initial(disk_flag);

    if (fsel_error == FSEL_NO_ERR)
    {
        result = RESULT_NULL;
    }
    else if ((fsel_error == FSEL_ERR_STG) || (fsel_error == FSEL_ERR_FS))
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);

        if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
        {
            //���ڿ���,�Ƚ��̷�����Ϊ�Ƿ�ֵ�Ա����³�ʼ���ļ�ѡ����
            g_browser_var.path.file_path.dirlocation.disk = 0xff;
            result = RESULT_BROWSER_MENU;
        }
        else
        {
            //����main ap
            result = RESULT_MAIN_APP;
        } 

    }
    else
    {
        result = RESULT_MAIN_APP;
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
 * \ingroup      browser
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

