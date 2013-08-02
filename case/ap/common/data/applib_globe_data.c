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
#include "case_type.h"

/*!
 *  \brief
 *  g_app_info_vector ȫ��Ӧ����Ϣ�ṹ������
 */
app_info_t g_app_info_vector[MAX_APP_COUNT];

/*!
 *  \brief
 *  g_app_info_state ȫ��ϵͳ��ǰ״̬�ṹ��
 */
app_info_state_t g_app_info_state;

/*!
 *  \brief
 *  g_app_last_state ȫ��ϵͳ���״̬�ṹ��
 */
app_last_state_t g_app_last_state;

/*!
 *  \brief
 *  g_sys_counter ϵͳ��ʱ���ṹ��
 */
sys_counter_t g_sys_counter;

/*!
 *  \brief
 *  config_fp ���ýű��ļ����
 */
sd_handle config_fp;

/*!
 *  \brief
 *  g_bat_sample ��ص�������ֵ������7��ȡƽ��ֵ
 */
uint8 g_bat_sample[BAT_SAMPLE_TIME];

/*!
 *  \brief
 *  g_bat_index ��ص����������
 */
uint8 g_bat_index;
