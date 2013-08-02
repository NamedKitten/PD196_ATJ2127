/*******************************************************************************
 *                              us211A
 *                            Module: case
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-10-24 16:20     1.0             build this file
 *******************************************************************************/
/*!
 * \file     vm_def.h
 * \brief    ��������Ӧ����ռVM�ռ�궨��
 * \author   zhangxs
 * \version 1.0
 * \date  2011-10-24
 *******************************************************************************/
#ifndef _VM_FWSP_DEF_H
#define _VM_FWSP_DEF_H

#define VM_FWSP_START 0x00080000
#define FS_VM VM_FWSP_START
#define FS_VM_LEN 0x2000
#define UDISK_VM (FS_VM+FS_VM_LEN)
#define UDISK_VM_LEN 0x200
#define UDISK_ENCRYPT_VM (UDISK_VM+UDISK_VM_LEN)
#define UDISK_ENCRYPT_VM_LEN 0x200
#define UDISKB_START_ADDR_OFFSET 0 //������B����ʼ��ַ�ڼ�����vram����Ϣ�е�ƫ��
#define UDISKC_START_ADDR_OFFSET 4 //������C����ʼ��ַ�ڼ�����vram����Ϣ�е�ƫ��
#define UDISKA_CAP_OFFSET        8 //������A�������ڼ�����vram����Ϣ�е�ƫ��    
#define UDISKB_CAP_OFFSET        12 //������B�������ڼ�����vram����Ϣ�е�ƫ��    
#define UDISKC_CAP_OFFSET        16 //������C�������ڼ�����vram����Ϣ�е�ƫ��    
#endif
