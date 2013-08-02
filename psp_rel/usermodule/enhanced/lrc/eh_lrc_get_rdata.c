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
 * \file     eh_lrc_rdata.c
 * \brief    ��ʳ�פ����
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
uint16 lrc_cur_num; //��ʵ�ҳ��
uint32 lrc_prev_time; //ǰһ��ʱ��
uint32 lrc_cur_time; //��ǰҳ��ʵ�ʱ��
uint32 lrc_next_time; //��һҳ��ʵ�ʱ��

uint16 lrc_lab_count; //ʱ���ǩ����


