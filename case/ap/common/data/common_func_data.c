/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-ext in rom
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-10-12 11:04     1.0             build this file 
*******************************************************************************/
/*!
 * \file     common_func_data.c
 * \brief    common func ȫ������
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2011-10-12
*******************************************************************************/

#include "common_func.h"

/*!
 *  \brief
 *  confmenu_fp �˵������ļ����
 */
sd_handle confmenu_fp;

/*!
 *  \brief
 *  confmenu_offset �˵�������ڲ˵��б���ʼ��ַ
 */
uint32 confmenu_offset;

/*!
 *  \brief
 *  this_filter_key_value ���˵���������������Ҫ���˵İ���
 */
uint8 this_filter_key_value;

/*!
 *  \brief
 *  g_keytone_backup ����¼��ʱ������ʹ����񱸷�
 */
bool g_keytone_backup;

/*!
 *  \brief
 *  dir_history_valid �ļ����·��������Ч���
 */
bool dir_history_valid;

/*!
 *  \brief
 *  menu_history_valid �˵��б�������Ч���
 */
bool menu_history_valid;

/*!
 *  \brief
 *  dir_history_addr �ļ���������仺����
 */
uint8 *dir_history_addr;

/*!
 *  \brief
 *  menu_history_addr �˵��б����仺����
 */
uint8 *menu_history_addr;
