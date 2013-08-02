/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-27 20:38     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _message_bank_send.c
 * \brief    ��Ϣͨ�Ź���ģ��ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ��һ�׻�����Ϣͨ�Ź���ӿڣ������첽���ͣ�ͬ�����ͣ��㲥��Ϣ����������Ϣ�ȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel��libc��applib ���̹���ӿڣ��ȵȡ�
 * \version  1.0
 * \date     2011-9-27
*******************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

static app_info_t *__app_running_get_app_info(uint8 app_id);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ͬ����ʽ����Ӧ��˽����Ϣ��
 * \param[in]    app_id ͬ������Ӧ��˽����Ϣ��Ŀ��Ӧ��ID
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[in]    reply ָ��ͬ����ϢӦ��ṹ��
 * \param[in]    timeout ��ʱ����ʱ�䣬0 ��ʾһֱ�ȴ�����ʱ����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ����ͬ����Ϣ�ɹ���
 * \retval           FALSE ����ͬ����Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    ����1������ʽ����ͬ����Ϣ��ֱ���Է����� reply_sync_msg Ӧ��
    msg_apps_t msg;
    msg_reply_t reply;
    bool ret;
    
    msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
    reply.content = (void *)file_path;
    ret = send_sync_msg(APP_ID_MANAGER, &msg, &reply, 0);
    if(ret == TRUE)
    {
        if(reply.type == MSG_REPLY_SUCCESS)
        {
            ��ȷ���أ���ȡ�������ļ�·��
        }
        else
        {
            ���󷵻�
        }
    }
    else
    {
        ��������
    }
 * \endcode
 * \note 
 * \li  ���ش���ԭ��������Ҳ�������Ӧ�ã�����ͬ����Ϣ���Լ���ͬ����Ϣ�����˵�����Ϣ�����������ȡ�
 * \li  ע����Ϣ���ݻ����� msg.content.addr �� Ӧ�𻺳��� reply.content �Ŀռ䶼�ǿ�����Ϣ���ͷ��ġ�
 *      ���ڷ���ͬ����Ϣ���ȴ����أ�������� send_sync_msg �Ĵ������� control bank�Σ���ô�����
 *      ���������Կ���bank data�С�
*******************************************************************************/
bool send_sync_msg(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
{
    private_msg_t cur_send_pmsg;
    const app_info_t *target_app_info;
    msg_reply_t temp_reply;
    uint8 i;
    os_event_t *sem;//ͬ����Ϣͬ���ź���ָ��
    bool retvalue = TRUE;

    //����ʱ�ȴ��ź�����ͬʱֻ����1���̷߳���ͬ����Ϣ
    libc_sem_wait(thread_mutex, 0);

    //����Ŀ��Ӧ��
    target_app_info = __app_running_get_app_info(app_id);
    if(NULL == target_app_info)
    {
        libc_sem_post(thread_mutex);
        return FALSE;
    }
    
    //���ܷ���ͬ����Ϣ��Ӧ���Լ�
    if(target_app_info == g_this_app_info)
    {
        libc_sem_post(thread_mutex);
        return FALSE;
    }

#ifdef SUPPORT_FILTER_GRAB  
    //����Ƿ񱻹��˵�
    if(target_app_info->filter_all_msg == TRUE)
    {
        libc_sem_post(thread_mutex);
        return FALSE;
    }
    for(i = 0; i < MAX_FILTER_MSG; i++)
    {
        if(target_app_info->msg_filtered[i] == msg->type)
        {
            libc_sem_post(thread_mutex);
            return FALSE;
        }
    }
#endif
    
    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));
    libc_sem_init(&sem, 0);
    cur_send_pmsg.sem = sem;
    if(reply == NULL)
    {
        cur_send_pmsg.reply = &temp_reply;
    }
    else
    {
        cur_send_pmsg.reply = reply;
    }

    //������Ϣ��������
    if(sys_mq_send((uint32)(target_app_info->mq_id), (void *)&cur_send_pmsg) == -1)
    {
        libc_sem_destroy(&sem);
        libc_sem_post(thread_mutex);
        return FALSE;
    }
    
    //�ȴ�ͬ����Ϣ��Ӧ
    if(libc_sem_wait(sem, timeout) < 0)
    {
        retvalue = FALSE;
    }

    libc_sem_destroy(&sem);
    libc_sem_post(thread_mutex);
    return retvalue;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���첽��ʽ����Ӧ��˽����Ϣ��
 * \param[in]    app_id ͬ������Ӧ��˽����Ϣ��Ŀ��Ӧ��ID
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[out]   none
 * \return       bool
 * \retval           TRUE �����첽��Ϣ�ɹ���
 * \retval           FALSE �����첽��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    ����1���첽������Ϣ������ȴ���������
    msg_apps_t msg;
    bool ret;
    
    msg.type = MSG_KILL_APP;
    msg.content.data[0] = APP_ID_MUSIC;
    ret = send_async_msg(APP_ID_MANAGER, &msg);
    if(ret == FALSE)
    {
        ��������
    }
 * \endcode
 * \note 
 * \li  ���ش���ԭ��������Ҳ�������Ӧ�ã��첽��Ϣ�����˵�����Ϣ�����������ȡ�
 * \li  ˽����Ϣ�첽���ͷ�ʽ�����ֻ�ܸ���4���ֽڵ���Ϣ���ݣ��������˫��Լ��code��Ϊ��Ϣ���ݣ���
*******************************************************************************/
bool send_async_msg(uint8 app_id, msg_apps_t *msg)
{
    const app_info_t *target_app_info;
    private_msg_t cur_send_pmsg;
    uint8 i;

    //����Ŀ��Ӧ��
    target_app_info = __app_running_get_app_info(app_id);
    if(target_app_info == NULL)
    {
        return FALSE;
    }

#ifdef SUPPORT_FILTER_GRAB
    //����Ƿ񱻹��˵�
    if(target_app_info->filter_all_msg == TRUE)
    {
        return FALSE;
    }
    for(i = 0; i < MAX_FILTER_MSG; i++)
    {
        if(target_app_info->msg_filtered[i] == msg->type)
        {
            return FALSE;
        }
    }
#endif
    
    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));

    //������Ϣ��������
    if(sys_mq_send((uint32)(target_app_info->mq_id), (void *)&cur_send_pmsg) == -1)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    Ӧ��ͬ����Ϣ������Ҫ�ͷ��ź�������
 * \param[in]    private_msg ָ����յ���Ӧ��˽����Ϣ����ͬ����ʽ���ͣ�
 * \param[out]   none
 * \return       bool
 * \retval           ����Ӧ��ͬ����Ϣ�Ƿ�ɹ���true��ʾ�ɹ���false��ʾʧ��
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    ����1��Ӧ��ͬ����Ϣ MSG_MENGINE_GET_FILEPATH_SYNC
    libc_memcpy((file_path_info_t *)private_msg.reply.content.addr, &tmp_file_path, sizeof(file_path_info_t));
    private_msg.reply.type = MSG_REPLY_SUCCESS;
    reply_sync_msg(&private_msg);
 * \endcode
 * \note 
*******************************************************************************/

bool reply_sync_msg(private_msg_t *private_msg)
{
    libc_sem_post(private_msg->sem);

    return TRUE;
}


/*! \endcond */

/*! \cond */

//����Ӧ��ID��ȡӦ����Ϣ
static app_info_t *__app_running_get_app_info(uint8 app_id)
{
    app_info_t *app;
    uint8 i;
    
    if(app_id >= APP_ID_MAX)
    {
        return NULL;
    }

    for(i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if((app->used == 1) && (app->app_id == app_id))
        {
            return app;
        }
    }

    return NULL;
}

/*! \endcond */
