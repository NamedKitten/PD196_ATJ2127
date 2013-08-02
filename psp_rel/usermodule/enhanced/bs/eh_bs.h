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
 * \file     eh_bs.h
 * \brief    ���ݽṹ�ͳ�������
 * \author   liminxian
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#ifndef _eh_bs_h

#define _eh_bs_h

#include "eh_module.h"
#include "eh_rdata_ex.h"

//extern global

extern uint16 bs_select_listno; // ѡ���list ��
// disk mode
//extern uint8  bs_file_type;  //������ļ�����
extern uint16 bs_dir_total; //��ǰĿ¼���ļ��е�����

extern uint16 bs_file_total; //��ǰĿ¼���ļ�������
// favorite mode
extern uint16 bs_uspl_cur;

// playlist mode
extern uint16 list_layer_no[4];

extern uint32 bs_open_object(uint8 open_mode) __FAR__;
extern bool bs_close_object(void) __FAR__;
extern uint32 bs_open_other(fsel_type_e type,uint8 open_mode) __FAR__;

#endif

