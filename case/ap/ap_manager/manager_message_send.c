/*******************************************************************************
 *                              us212A
 *                            Module: Manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-27 20:38     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_message_send.c
 * \brief    applib ��Ϣͨ�Ż��� �����ࣨbank�� �ӿ�ʵ��
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ��һ�׻�����Ϣͨ�Ż��ƣ���Ҫ������Ϣ���ͻ��ơ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               ���� libc �� psp �Ľӿڣ��Լ�applib ���̹���ģ��ӿ�ʵ����Ϣ���ơ�
 * \version 1.0
 * \date  2011-9-27
 *******************************************************************************/

#include "applib.h"
/******************************************************************************/
/*!
 * \par  Description:
 *    ���첽��ʽ����Ӧ��˽����Ϣ
 * \param[in]    app_id��ͬ������Ӧ��˽����Ϣ��Ŀ��Ӧ��ID
 * \param[in]    msg��ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[out]   none
 * \return       bool
 * \retval           ���ط����첽��Ϣ�Ƿ�ɹ���true��ʾ�ɹ���false��ʾʧ��
 * \par          exmaple code
 * \code
 *               msg.type = MSG_KILL_APP;
 *               msg.content.data[0] = APP_ID_MUSIC;
 *               result = send_async_msg(APP_ID_MANAGER, &msg);
 *               if(result == TRUE)
 *               {}
 * \endcode
 * \note
 *******************************************************************************/
bool send_async_msg(uint8 app_id, msg_apps_t *msg)
{
    private_msg_t cur_send_pmsg;
    mq_id_e mq_id;

    cur_send_pmsg.sem = NULL;
    cur_send_pmsg.reply = NULL;

    //����Ŀ��Ӧ��
    if ((app_id == APP_ID_MENGINE) || (app_id == APP_ID_FMENGINE))
    {
        mq_id = MQ_ID_EGN;
    }
    else
    {
        mq_id = MQ_ID_DESK;
    }

    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));

    //������Ϣ��������
    if ((int) sys_mq_send((uint32) (mq_id), (void *) &cur_send_pmsg) == -1)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �Թ㲥��ʽ��Ҳ�����첽���ͣ�����Ӧ��˽����Ϣ
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[out]   none
 * \return       bool
 * \retval           ���ع㲥��Ϣ�Ƿ�ɹ���true��ʾ�ɹ���false��ʾʧ��
 * \par          exmaple code
 * \code
 *               msg.type = MSG_APP_QUIT;
 *               result = broadcast_msg(&msg);
 *               if(result == TRUE)
 *               {}
 * \endcode
 * \note
 *******************************************************************************/
bool broadcast_msg(msg_apps_t *msg)
{
    app_info_t *app;
    bool ret = FALSE;
    uint8 i;

    //�㲥����Ϣ������ǰ̨Ӧ�ã���ʵ��ֻ��1��
    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if ((app->used == 1) && (app->app_type == APP_TYPE_GUI))
        {
            ret = send_async_msg(app->app_id, msg);
            if(ret == FALSE)
            {
                return FALSE;
            }
        }
    }

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ��ͬ����Ϣ������Ҫ�ͷ��ź�������
 * \param[in]    private_msg��ָ����յ���Ӧ��˽����Ϣ����ͬ����ʽ���ͣ�
 * \param[out]   none
 * \return       bool
 * \retval           ����Ӧ��ͬ����Ϣ�Ƿ�ɹ���true��ʾ�ɹ���false��ʾʧ��
 * \note
 *******************************************************************************/
bool reply_sync_msg(private_msg_t *private_msg)
{
    libc_sem_post(private_msg->sem);

    return TRUE;
}

