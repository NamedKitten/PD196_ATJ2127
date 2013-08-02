/*******************************************************************************
 *                              US212A
 *                            Module: TOOLS_MAIN
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      liuweizhan    2011-12-12 17:03:16           1.0              build this file
 *******************************************************************************/
/*!
 * \file     tools_menu.c
 * \brief
 * \author   liuweizhan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2011-12-12
 *******************************************************************************/
#include "ap_tools.h"

const uint8 am[3] =
{ 'A', 'M', 0 };
const uint8 pm[3] =
{ 'P', 'M', 0 };
/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵�� : stopwatch��timer�жϴ�����
 *      �ص�˵�� : ����RCODE
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ����
 * \note: 
 *
 ******************************************************************************
 */

void StopWatch_timer_proc(void)
{
    if(CtcCntEnable != 0)
    {
        sw_realtime.Ms++;
        sw_realtime.Second = sw_realtime.Second + sw_realtime.Ms / 100;
        sw_realtime.Ms = sw_realtime.Ms % 100;
        sw_realtime.Minute = sw_realtime.Minute + sw_realtime.Second / 60;
        sw_realtime.Second = sw_realtime.Second % 60;
        sw_realtime.Hour = sw_realtime.Hour + sw_realtime.Minute / 60;
        sw_realtime.Minute = sw_realtime.Minute % 60;
    }
}


/******************************************************************************/
/*!
 * \par  Description:
 * \
 * \����gui��Ϣ
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup
 * \note
 */
/*******************************************************************************/
app_result_e tool_privmsg_deal(private_msg_t *private_msg)
{
    app_result_e result = RESULT_NULL;
    //msg_reply_t m_reply;
    /*ͬ����Ϣ����*/

    switch (private_msg->msg.type)
    {
        case MSG_SD_OUT:
        if (g_dir_flag == TRUE)
        {
            //���ڷ��ʿ����ļ����˻ص���Ӧ��
            if (get_engine_type() == ENGINE_MUSIC)
            {
                music_close_engine();
            }
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
            result = RESULT_MAIN_APP;
            g_dir_flag = FALSE;
        }
        break;
        
        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;

        default:
        result = com_message_box(private_msg->msg.type);
        break;
    }

    return result;
}



