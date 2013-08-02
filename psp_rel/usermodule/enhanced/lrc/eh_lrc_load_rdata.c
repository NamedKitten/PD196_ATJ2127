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
handle lrc_handle;
uint16 time_lab_count; //ʱ���ǩ����

#ifdef _VC_DEBUG
uint8 LRC_CONTENT_ADDR[0x800];
uint8 LRC_LABLE_ADDR[0x800];
uint8 LRC_CONTENT_VM[0x2000];
uint8 LRC_LABLE_VM[0x2000];
#else

#ifdef PC
uint8 LRC_CONTENT_ADDR[0x800];
uint8 LRC_LABLE_ADDR[0x800];
//uint8 LRC_CONTENT_VM[0x2000];
//uint8 LRC_LABLE_VM[0x2000];
#endif

#endif

