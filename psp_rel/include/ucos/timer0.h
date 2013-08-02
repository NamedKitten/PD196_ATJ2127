/********************************************************************************
 *                              USDK(1100)
 *                            Module: TIMER
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     timer.h
 * \brief    ϵͳ��ʱ��ͷ�ļ�
 * \author   open source
 * \version 1.0
 * \date  2011-9-10
 *******************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

#include <ucos/list.h>
#include <ucos/interrupt.h>

#ifndef _ASSEMBLER_

/*!
 * \brief
 *      ��ʱ������ṹ����.
 */
struct timer_list {
	/*! ������˫������*/
	struct list_head entry;//null
	/*! ����ʱ��*/
	unsigned long expires;//null
	/*! ��ʱ���ص�����*/
	void (*function)(void);//�û�����
	/*! period*/
	unsigned long count;//�û�����
};

extern void add_timer(struct timer_list * timer);
extern int del_timer(struct timer_list * timer);

extern void init_timers(void);
extern struct list_head timer_header;

#endif /*_ASSEMBLER_*/
#endif 	/*__TIMER_H__*/
