/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-28 14:42     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _app_manager_bank.c
 * \brief    Ӧ�ã����̣�����ģ��ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ��Ӧ�ã����̣�����ӿڣ�����Ӧ��ע��ͳ�ʼ����Ӧ��ע���ȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel��libc��common_config���ȵ�
 * \version  1.0
 * \date     2011-9-28
*******************************************************************************/

#include "applib.h"

static bool __app_running_set_app_info(uint8 app_id, const app_info_t *app_info);

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    Ӧ���� applib ģ���ϵ�ע��ͳ�ʼ����
 * \param[in]    app_id Ӧ��ID���� APP_ID_MUSIC��APP_ID_MANAGER��APP_ID_MENGINE ��
 * \param[in]    type Ӧ�����ͣ��μ� app_type_e ����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ��ʾע��ͳ�ʼ���ɹ���
 * \retval           FALSE ��ʾע��ͳ�ʼ��ʧ�ܡ�
 * \ingroup      app_manager
 * \par          exmaple code
 * \code
    ����1��ǰ̨Ӧ�� music AP ���� applib ģ��ע��ͳ�ʼ��
    applib_init(APP_ID_MUSIC, APP_TYPE_GUI);
    
    ����2����̨����Ӧ�� music engine AP ���� applib ģ��ע��ͳ�ʼ��
    applib_init(APP_ID_MENGINE, APP_TYPE_CONSOLE);
    
    ����3�����̹���Ӧ�� manager AP ���� applib ģ��ע��ͳ�ʼ��
    applib_init(APP_ID_MANAGER, APP_TYPE_PMNG);
 * \endcode
 * \note
 * \li  ��Ӧ�� main ������ڴ����ã��ڴ�֮ǰ������������� applib �ӿڡ�
*******************************************************************************/
bool applib_init(uint8 app_id, app_type_e type)
{
    app_info_t *cur_app_info;

    if(app_id >= APP_ID_MAX)
    {
        return FALSE;
    }
    
    //��ȡ app_info �ṹ��ָ��
    cur_app_info = &g_app_info_vector[type];
    
    //��������ֹ�������
    sys_os_sched_lock();

    //��ʼ��Ӧ����Ϣ
    libc_memset(cur_app_info, 0x0, sizeof(app_info_t));
    cur_app_info->used = 1;//��־�ѱ�����ʹ��
    cur_app_info->app_id = app_id;
    cur_app_info->app_type = (uint8)type;
    cur_app_info->soft_watchdog_timeout_expires = 0;
    if(type == APP_TYPE_PMNG)
    {
        cur_app_info->mq_id = (uint8)MQ_ID_MNG;
    }
    else if(type == APP_TYPE_GUI)
    {
        cur_app_info->mq_id = (uint8)MQ_ID_DESK;
        g_app_info_state.app_state = APP_STATE_NO_PLAY;//ǰ̨APע�ᣬ��ʼ��Ϊ NO_PLAY ״̬
    }
    else
    {
        cur_app_info->mq_id = (uint8)MQ_ID_EGN;
        
        if(app_id == APP_ID_MENGINE)
        {
            g_app_info_state.type = ENGINE_MUSIC;
        }
        else if(app_id == APP_ID_FMENGINE)
        {
            g_app_info_state.type = ENGINE_RADIO;
        }
        else//�����ܵ�������
        {
            g_app_info_state.type = ENGINE_NULL;
        }
    }
    
    //�����������������
    sys_os_sched_unlock();

    g_this_app_info = cur_app_info;

    //��ʼ���̻߳����ź���
    libc_sem_init(&thread_mutex, 1);

    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    Ӧ���� applib ģ���ϵ�ע������
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ��ʾע���ɹ���
 * \retval           FALSE ��ʾע��ʧ�ܡ�
 * \ingroup      app_manager
 * \par          exmaple code
 * \code
    ����1��Ӧ�ý��� applib ģ��ע��
    applib_quit();
 * \endcode
 * \note
 * \li  ��Ӧ�� main �����˳������ã��ڴ�֮�󣬲���������� applib �ӿڡ�
*******************************************************************************/
bool applib_quit(void)
{
    app_info_t cur_app_info;
    
    //�����̻߳����ź���
    libc_sem_destroy(&thread_mutex);
    
    //��¼��һ���˳���ǰ̨AP���Ա㷵��
    if(g_this_app_info->app_type == APP_TYPE_GUI)
    {
        g_app_last_state.last_app = g_this_app_info->app_id;
    }
    
    //�޸ĺ�̨��������Ϊ NONE
    if(g_this_app_info->app_type == APP_TYPE_CONSOLE)
    {
        g_app_info_state.type = ENGINE_NULL;
        g_app_info_state.state = ENGINE_STATE_NULL;
    }
    
    //�޸� app_info �ṹ��Ϊδ��ʹ��
    libc_memcpy(&cur_app_info, g_this_app_info, sizeof(app_info_t));
    cur_app_info.used = 0;
    cur_app_info.app_id = APP_ID_MAX;
    cur_app_info.soft_watchdog_timeout_expires = 0;
    __app_running_set_app_info(g_this_app_info->app_id, &cur_app_info);
    
    return TRUE;
}

/*! \endcond */

/*! \cond */

//����Ӧ��ID����Ӧ����Ϣ
static bool __app_running_set_app_info(uint8 app_id, const app_info_t *app_info)
{
    app_info_t *app;
    uint8 i;
    
    if(app_id >= APP_ID_MAX)
    {
        return FALSE;
    }

    //���� app_id ��Ӧ app_info_t 
    app = NULL;
    for(i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if((app->used == 1) && (app->app_id == app_id))
        {
            break;
        }
    }
    
    if(NULL == app)
    {
        return FALSE;
    }

    sys_os_sched_lock();
    libc_memcpy(app, app_info, sizeof(app_info_t));
    sys_os_sched_unlock();

    return TRUE;
}

/*! \endcond */
