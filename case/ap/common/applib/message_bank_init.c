/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-11-9 17:29     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _message_bank_init.c
 * \brief    ��Ϣͨ�Ź���ģ��ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ��һ�׻�����Ϣͨ�Ź���ӿڣ������첽���ͣ�ͬ�����ͣ��㲥��Ϣ����������Ϣ�ȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel��libc��applib ���̹���ӿڣ��ȵȡ�
 * \version  1.0
 * \date     2011-11-9
*******************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ʼ�� applib ��Ϣͨ�Ź���ģ�顣
 * \param[in]    msg_dispatch Ӧ����Ϣ�ַ���������ǰ̨Ӧ������ common ģ��ע��ú���
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ��ʼ����Ϣͨ�Ź���ģ��ɹ���
 * \retval           FALSE ��ʼ����Ϣͨ�Ź���ģ��ʧ��
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    ����1��ǰ̨AP����
    extern app_result_e app_msg_dispatch(private_msg_t *msg);
    
    applib_message_init(app_msg_dispatch);
    
    ����2�����̹��� �� ��̨AP����
    applib_message_init(NULL);
 * \endcode
 * \note 
*******************************************************************************/
bool applib_message_init(app_msg_dispatch msg_dispatch)
{
    if(g_this_app_info->app_type == APP_TYPE_GUI)
    {
        //��� gui ��Ϣ����
        sys_mq_flush(MQ_ID_GUI);
        
        //ע��ǰ̨Ӧ����Ϣ�ַ��������Թ� common ģ�����
        g_this_app_msg_dispatch = msg_dispatch;
    }
    
    //��� Ӧ����Ϣ����
    sys_mq_flush(g_this_app_info->mq_id);
    
    return TRUE;
}

/*! \endcond */
