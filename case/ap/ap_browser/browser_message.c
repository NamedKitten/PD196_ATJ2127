/*******************************************************************************
 *                              US212A
 *                            Module: BROWSER_PROC_MSG
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-10-25 16:16:47           1.0              build this file
 *******************************************************************************/
/*!
 * \file     browser_message.c
 * \brief    browser��Ϣ����ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2011-10-25
 *******************************************************************************/
#include "browser.h"

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser˽����Ϣ��ϵͳ��Ϣ����ص�����
 * \param[in]    �յ���˽����Ϣ��ϵͳ��Ϣ
 * \param[out]   none
 * \return       app_result_e
 * \ingroup      browser
 * \note
 *******************************************************************************/
app_result_e browser_msg_callback(private_msg_t *pri_msg)
{
    app_result_e result = RESULT_NULL;
    engine_type_e engine_type;
    //msg_reply_t m_reply;
    /*ͬ����Ϣ����*/
    switch (pri_msg->msg.type)
    {
        case MSG_SD_OUT:
        if (g_browser_var.path.file_path.dirlocation.disk == DISK_H)
        {
            //���ڷ��ʿ����ļ����˻ص���Ӧ��
            engine_type = get_engine_type();

            if (engine_type == ENGINE_MUSIC)
            {
                music_close_engine();
            }
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
            result = RESULT_MAIN_APP;
        }
        break;

        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;

        default:
        result = com_message_box(pri_msg->msg.type);
        break;
    }
    return result;
}

