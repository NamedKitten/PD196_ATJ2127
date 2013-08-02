/*******************************************************************************
 *                              us212A
 *                            Module: applib-message(send)
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-11-10 11:26     1.0             build this file 
*******************************************************************************/
/*!
 * \file     applib_data.c
 * \brief    applib ģ��ȫ�ֱ�������
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               applib ģ��ȫ�ֱ�������
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2011-11-10
*******************************************************************************/

#include "applib.h"

/*!
 *  \brief
 *  thread_mutex �̻߳����ź���ָ��
 */
os_event_t *thread_mutex;

/*!
 *  \brief
 *  g_this_app_info ��ǰӦ����Ϣ�ṹ��ָ��
 */
const app_info_t *g_this_app_info;

/*!
 *  \brief
 *  g_app_timer_vector Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
 */
app_timer_t *g_app_timer_vector;

/*!
 *  \brief
 *  g_app_timer_count Ӧ����ʱ����Ŀ��Ϊ 0 ��ʾӦ�ò���Ҫʹ�ö�ʱ��
 */
uint8 g_app_timer_count = 0;

/*!
 *  \brief
 *  g_this_app_timer_tag ��ʱ��������־��AP����Ϊ0
 */
uint8 g_this_app_timer_tag;

/*!
 *  \brief
 *  g_this_app_msg_dispatch Ӧ��˽����Ϣ�ַ�����
 */
app_msg_dispatch g_this_app_msg_dispatch;

