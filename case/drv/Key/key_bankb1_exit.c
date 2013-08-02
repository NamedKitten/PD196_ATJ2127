/*******************************************************************************
 *                              US212A
 *                            Module: Key driver
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       reagan     2011-9-2 14:45    1.0             build this file
 *******************************************************************************/
/*!
 * \file     key_exit.c
 * \brief    key driver exit functions
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

#include "key_inner.h"

/******************************************************************************/
/*!
 * \par  Description:
 *	  KEY����ж�أ�ע���жϷ���
 * \param[in]    none
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \ingroup      key_install
 * \note
 * \li  �ýӿ�������ж��ʱ��ϵͳ�Զ����á�
 *******************************************************************************/
void key_exit(void)
{

    sys_del_irq_timer1(time_irq);

}
module_exit(key_exit)
