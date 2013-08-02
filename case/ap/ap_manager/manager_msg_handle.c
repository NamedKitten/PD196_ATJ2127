/*******************************************************************************
 *                              US212A
 *                            Module: Manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_msg_handle.c
 * \brief    ��Ϣ����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "manager.h"

extern void _get_app_name(char *namebuf, uint8 ap_id);
extern void manager_msg_callback_sub(private_msg_t *pri_msg);

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��Ϣ����
 * \param[in]    ˽����Ϣָ��
 * \param[out]   none
 * \return       none
 * \retval       
 * \retval       
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void manager_msg_callback(private_msg_t *pri_msg)
{
    uint8 ap_name[12];
    int32 waitpid_ret;
    msg_apps_t async_msg;
    uint8 ap_param;
    uint8 ap_ret;

    switch (pri_msg->msg.type)
    {
        //����ǰ̨Ӧ��
        case MSG_CREAT_APP:
        //���ܴ�����ǰ��ǰ̨ap
        if (g_app_info_vector[1].app_id != pri_msg->msg.content.data[0])
        {
            waitpid_ret = libc_waitpid(&ap_ret, 0);
            if ((waitpid_ret >= 0) && (waitpid_ret != WAITPID_NO_PROCESS_EXIT))
            {
                _get_app_name(ap_name, pri_msg->msg.content.data[0]);
                ap_param = pri_msg->msg.content.data[1];
                sys_free_ap(FALSE);
                sys_exece_ap(ap_name, 0, (int32) ap_param);
            }
        }
        break;
        
        //������̨Ӧ��
        case MSG_CREAT_APP_SYNC:
        //Ӧ��Լ�������һ����̨���ڣ���Ҫ�Ȱѵ�ǰ��̨ɱ����Ȼ���ٴ�����Ҫ�ĺ�̨
        if (g_app_info_vector[2].used == 0)
        {
            _get_app_name(ap_name, pri_msg->msg.content.data[0]);
            ap_param = pri_msg->msg.content.data[1];
            sys_free_ap(TRUE);
            if (sys_exece_ap(ap_name, 1, (int32) ap_param) >= 0)
            {
                pri_msg->reply->type = MSG_REPLY_SUCCESS;
            }
            else
            {
                pri_msg->reply->type = MSG_REPLY_FAILED;
            }
            reply_sync_msg(pri_msg);
        }
        break;

        case MSG_KILL_APP_SYNC:
        //�����̨���ڲŴ���
        if (g_app_info_vector[2].used != 0)
        {
            async_msg.type = MSG_APP_QUIT;
            send_async_msg(pri_msg->msg.content.data[0], &async_msg);
            while (1)
            {
                waitpid_ret = libc_waitpid(&ap_ret, 0);
                if ((waitpid_ret >= 0) && (waitpid_ret != WAITPID_NO_PROCESS_EXIT))
                {
                    pri_msg->reply->type = MSG_REPLY_SUCCESS;
                    reply_sync_msg(pri_msg);
                    break;
                }
            }
        }
        else//��̨�Ѿ���ɱ����ֱ�ӷ��سɹ�������ǰ̨�ղ�����Ӧ������
        {
            pri_msg->reply->type = MSG_REPLY_SUCCESS;
            reply_sync_msg(pri_msg);
        }
        break;
        
        //ֹͣwatchdog ���Ź�ģ����ʱɾ��
        case MSG_STOP_HARD_WATCHDG_SYNC:
        break;
        
        //��������watchdog ���Ź�ģ����ʱɾ��
        case MSG_RESTART_HARD_WATCHDG_SYNC:
        break;

        default:
        manager_msg_callback_sub(pri_msg);
        break;
    }

}
