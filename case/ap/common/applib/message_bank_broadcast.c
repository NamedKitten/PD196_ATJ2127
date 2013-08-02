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
 * \file     _message_bank_broadcast.c
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

#ifdef SUPPORT_FILTER_GRAB

static bool __app_running_set_app_info(uint8 app_id, const app_info_t *app_info);

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    �Թ㲥��ʽ��Ҳ�����첽���ͣ�����Ӧ��˽����Ϣ��
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[out]   none
 * \return       bool
 * \retval           TRUE �㲥��Ϣ�ɹ���
 * \retval           FALSE �㲥��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    ����1���㲥 MSG_POWER_OFF �ػ���Ϣ
    msg_apps_t msg;
    
    msg.type = MSG_POWER_OFF;
    broadcast_msg(&msg);
 * \endcode
 * \note 
 * \li  �㲥Ӧ����Ϣ���ֻ�ܸ���4���ֽڵ���Ϣ���ݣ��������˫��Լ��code��Ϊ��Ϣ���ݣ���
*******************************************************************************/
bool broadcast_msg(msg_apps_t *msg)
{
    const app_info_t *grab_msg_owner = NULL;
    app_info_t *app;
    uint8 i, j;

    //����ץȡ�˸���Ϣ��Ӧ��
    for(i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        for(j = 0; j < MAX_GRAB_MSG; j++)
        {
            if((app->used == 1) && (app->msg_grabed[i] == msg->type))
            {
                grab_msg_owner = app;
                break;
            }
        }
    }

    //��Ӧ��ץȡ�˸���Ϣ�������͸���Ӧ�ü���
    if(grab_msg_owner != NULL)
    {
        send_async_msg(grab_msg_owner->app_id, msg);
    }
    //���򣬹㲥����Ϣ������Ӧ��
    else
    {
        for(i = 0; i < MAX_APP_COUNT; i++)
        {
            app = &g_app_info_vector[i];
            if(app->used == 1)
            {
                send_async_msg(app->app_id, msg);
            }
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���Ӧ��ץȡ��Ϣ�����ڹ㲥������Ϣʱ��ռ����Ϣ��
 * \param[in]    msg_type Ӧ��Ҫץȡ��Ӧ����Ϣ����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ���ץȡ��Ϣ�ɹ���
 * \retval           FALSE ���ץȡ��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \note 
*******************************************************************************/
bool grab_msg(msg_apps_type_e msg_type)
{
    app_info_t app_info;
    uint8 i;

    //�ȶ��Ѿ�ץȡ����Ϣ��Ӧ���ͷŵ�����Ϣ
    release_msg(msg_type);

    //Ѱ�Һ��ʵ�λ�����ץȡ��Ϣ
    for(i = 0; i < MAX_GRAB_MSG; i++)
    {
        if(g_this_app_info->msg_grabed[i] == V_U16_INVALID)
        {
            libc_memcpy(&app_info, g_this_app_info, sizeof(app_info_t));
            app_info.msg_grabed[i] = msg_type;
            __app_running_set_app_info(app_info.app_id, &app_info);
            return TRUE;
        }
    }

    return FALSE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ɾ������ӵ�Ӧ��ץȡ��Ϣ��
 * \param[in]    msg_type Ӧ��Ҫɾ����ץȡӦ����Ϣ����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ɾ��ץȡ��Ϣ�ɹ���
 * \retval           FALSE ɾ��ץȡ��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \note 
*******************************************************************************/
bool release_msg(msg_apps_type_e msg_type)
{
    app_info_t *app;
    app_info_t  app_info;
    uint8 i, j;

    //����Ӧ����Ϣ�ṹ������
    for(i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        for(j = 0; j < MAX_GRAB_MSG; j++)
        {
            if((app->used == 1) && (app->msg_grabed[j] == msg_type))
            {
                libc_memcpy(&app_info, app, sizeof(app_info_t));
                app_info.msg_grabed[i] = V_U16_INVALID;
                __app_running_set_app_info(app_info.app_id, &app_info);
            }
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���Ӧ�ù�����Ϣ���Թ㲥��Ϣʱ���˵�����Ϣ�������䷢�͵�Ӧ��˽����Ϣ���С�
 * \param[in]    msg_type Ӧ��Ҫ��ӵĹ���Ӧ����Ϣ����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ��ӹ�����Ϣ�ɹ���
 * \retval           FALSE ��ӹ�����Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \note 
*******************************************************************************/
bool filter_msg(msg_apps_type_e msg_type)
{
    app_info_t app_info;
    uint8 i;

    for(i = 0; i < MAX_FILTER_MSG; i++)
    {
        if(g_this_app_info->msg_filtered[i] == V_U16_INVALID)
        {
            libc_memcpy(&app_info, g_this_app_info, sizeof(app_info_t));
            app_info.msg_filtered[i] = msg_type;
            __app_running_set_app_info(app_info.app_id, &app_info);
            return TRUE;
        }
    }

    return FALSE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ɾ�������˵�Ӧ����Ϣ��
 * \param[in]    msg_type Ӧ��Ҫɾ���Ĺ���Ӧ����Ϣ����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ɾ��������Ϣ�ɹ���
 * \retval           FALSE ɾ��������Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \note 
*******************************************************************************/
bool unfilter_msg(msg_apps_type_e msg_type)
{
    app_info_t app_info;
    uint8 i;

    for(i = 0; i < MAX_FILTER_MSG; i++)
    {
        if(g_this_app_info->msg_filtered[i] == msg_type)
        {
            libc_memcpy(&app_info, g_this_app_info, sizeof(app_info_t));
            app_info.msg_filtered[i] = V_U16_INVALID;
            __app_running_set_app_info(app_info.app_id, &app_info);
            return TRUE;
        }
    }

    return FALSE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���˵����й㲥��Ӧ����Ϣ��
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE �������й㲥��Ӧ����Ϣ�ɹ���
 * \retval           FALSE �������й㲥ʧ�ܡ�
 * \ingroup      applib_msg
 * \note 
*******************************************************************************/
bool filter_all_msg(void)
{
    app_info_t app_info;
    
    libc_memcpy(&app_info, g_this_app_info, sizeof(app_info_t));
    app_info.filter_all_msg  = TRUE;
    __app_running_set_app_info(app_info.app_id, &app_info);

    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ������˵����й㲥��Ӧ����Ϣ�����������ͨ��filter_msg��ӵĹ�����Ϣ��
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ����������й㲥��Ӧ����Ϣ�ɹ���
 * \retval           FALSE ����������й㲥��Ӧ����Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \note 
*******************************************************************************/
bool unfilter_all_msg(void)
{
    app_info_t app_info;

    libc_memcpy(&app_info, g_this_app_info, sizeof(app_info_t));
    app_info.filter_all_msg  = FALSE;
    libc_memset(app_info.msg_filtered, V_U16_INVALID, sizeof(app_info.msg_filtered));
    __app_running_set_app_info(app_info.app_id, &app_info);
    
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

#else

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    �Թ㲥��ʽ��Ҳ�����첽���ͣ�����Ӧ��˽����Ϣ��
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[out]   none
 * \return       bool
 * \retval           TRUE �㲥��Ϣ�ɹ���
 * \retval           FALSE �㲥��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    ����1���㲥 MSG_POWER_OFF �ػ���Ϣ
    msg_apps_t msg;
    
    msg.type = MSG_POWER_OFF;
    broadcast_msg(&msg);
 * \endcode
 * \note 
 * \li  �㲥Ӧ����Ϣ���ֻ�ܸ���4���ֽڵ���Ϣ���ݣ��������˫��Լ��code��Ϊ��Ϣ���ݣ���
*******************************************************************************/
bool broadcast_msg(msg_apps_t *msg)
{
    app_info_t *app;
    uint8 i;

    //�㲥����Ϣ������Ӧ��
    for(i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if(app->used == 1)
        {
            send_async_msg(app->app_id, msg);
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    �Թ㲥��ʽ��Ҳ�����첽���ͣ�����Ӧ��˽����Ϣ��
 * \param[in]    msg ָ��Ҫ���͵�Ӧ����Ϣ�ṹ��
 * \param[out]   none
 * \return       bool
 * \retval           TRUE �㲥��Ϣ�ɹ���
 * \retval           FALSE �㲥��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    ����1���㲥ϵͳ��Ϣ���� get_sys_msg ʵ��
 * \endcode
 * \note 
 * \li  �㲥Ӧ����Ϣ���ֻ�ܸ���4���ֽڵ���Ϣ���ݣ��������˫��Լ��code��Ϊ��Ϣ���ݣ���
 * \li  �ýӿڲ����ϵͳ��Ϣ�㲥�� manager ap ˽����Ϣ�����С�
*******************************************************************************/
bool broadcast_msg_sys(msg_apps_t *msg)
{
    app_info_t *app;
    uint8 i;
    
    if(msg->type == MSG_RECLAIM)//nandflash reclaim��ֻ���͸� manager AP
    {
        if(g_this_app_info->app_id != APP_ID_UDISK)//U �������²�����
        {
            send_async_msg(APP_ID_MANAGER, msg);
        }
        return TRUE;
    }

    //�㲥����Ϣ������Ӧ��
    for(i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if((app->used == 1) && (app->app_id != APP_ID_MANAGER))
        {
            send_async_msg(app->app_id, msg);
        }
    }

    return TRUE;
}

/*! \endcond */

#endif
