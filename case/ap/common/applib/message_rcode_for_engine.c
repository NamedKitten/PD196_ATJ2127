/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-11-21 16:06     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _message_rcode_for_engine.c
 * \brief    ��Ϣͨ�Ź���ģ��ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ��һ�׻�����Ϣͨ�Ź���ӿڣ������첽���ͣ�ͬ�����ͣ��㲥��Ϣ����������Ϣ�ȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel��libc��applib ���̹���ӿڣ��ȵȡ�
 * \version  1.0
 * \date     2011-11-21
*******************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ����Ӧ��˽����Ϣ����Щ�����ڽ���ϵͳ��Ϣʱת���ģ���ר����̨����ʹ�õľ���汾��
 * \param[in]    none
 * \param[out]   private_msg�����ؽ��յ���Ӧ��˽����Ϣ
 * \return       bool
 * \retval           TRUE ����Ӧ��˽����Ϣ�ɹ���
 * \retval           FALSE ����Ӧ��˽����Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    ����1����̨����Ӧ��˽����Ϣ����
    private_msg_t private_msg;
    app_result_e result;
    bool ret;
    
    ret = get_app_msg(&private_msg);
    if(ret == TRUE)
    {
        result = msg_callback(&private_msg);
        if(result != RESULT_NULL)
        {
            �ǿշ���ֵ����
        }
    }
 * \endcode
 * \note 
*******************************************************************************/
bool get_app_msg_for_engine(private_msg_t *private_msg)
{
    //��ʱ��ɨ��
    if(g_app_timer_count > 0)
    {
        handle_timers();
    }

    //���� Ӧ��˽����Ϣ
    if(sys_mq_receive((uint32)(g_this_app_info->mq_id), (void *)private_msg) == 0)
    {
        return TRUE;
    }
    
    //��Ϣ�����ѿգ�����false
    return FALSE;
}
#if 0
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
bool reply_sync_msg(private_msg_t *private_msg)//ǿ���ӣ�LD����ʱʹ�ô˷�������
{
    libc_sem_post(private_msg->sem);

    return TRUE;
}
#endif
/*! \endcond */
