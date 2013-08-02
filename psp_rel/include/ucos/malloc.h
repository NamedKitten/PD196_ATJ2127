/********************************************************************************
 *                               USDK(1100)
 *                            Module: IRQ
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     malloc.h
 * \brief    ���ڴ����
 * \author   wuyueqian
 * \version 1.0
 * \date   2011-9-10
 *******************************************************************************/
#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <typeext.h>
#include <os_cpu.h>

#define LIBC_INNER_MEM_SIZE (512)
#define HEAD_ADDR 0x9fc18e00

/*���ڵ�ַ����*/
#define LIBC_MY_BYTE_ALIGN(x) (((((x) + 4)-1)/4) * 4)
#define SIZE_MASK 0xfff00000
#define SIZE_POS 20
#define SRAM_MASK 0x3ffff

#ifndef _ASSEMBLER_

/*���ٵ�bank data������ջ����ʾ*/
/*bank data�����ڶ�̬���ڴ�*/
struct libc_alloc_struct {
	/*���ڴ�����ʼ��ַ*/
	uint32 address;
	/*���ڴ��Ĵ�С��ռ��12bit����18bit��ָ���������ݿ����ʼ��ַ�����û�����룬��ָ��0*/
	uint32 size_and_next;
	/*ָ����һ���ڴ��*/
	struct libc_alloc_struct *next;
};

extern int malloc(void *pmem_addr, unsigned int size, void *null2);
extern int free(void *pmem_addr, void *null1, void*null2);

#endif /*_ASSEMBLER_*/

#endif /*__IRQ_H__*/

