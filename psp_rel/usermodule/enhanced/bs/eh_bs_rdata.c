/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     eh_bs_rdata.c
 * \brief    ���ģ�鳣פ����
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"

/*********************************
 *** rdata  define
 *********************************/

// common
//uint8 dir_layer;
uint16 bs_select_listno; // ѡ���list���
uint16 bs_select_fileno; // ѡ����ļ����
// disk mode
//uint8  bs_file_type;     //������ļ�����
uint16 bs_dir_total; //��ǰĿ¼���ļ��е�����
uint16 bs_file_total; //��ǰĿ¼���ļ�������
// favorite mode
uint16 bs_uspl_cur;
// playlist mode
uint16 list_layer_no[4];
plist_tree_t plist_cur_tree;
bs_dir_location_t g_bs_dir_loc;

