/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     driver_manager.h
 * \brief    ��������
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef _DRIVER_H
#define _DRIVER_H

#include <typeext.h>
#include <attr.h>

#ifndef _ASSEMBLER_

/*5110�Ŀ�������������һ����������������λ�ÿ��ܻ᲻һ��*/
/*��Ϊ���ݿ���������ÿ���ļ�ϵͳҲ�п��ܳ����ڲ�ͬ�Ŀռ�*/
/*�������ִ洢�����������ļ�ϵͳ������������drv�ļ�*/
#define MODE_NORMAL 0
#define MODE_SPECIAL 1

typedef struct {
	unsigned char file_type;
	unsigned char drv_type;
	unsigned char Major_version;
	unsigned char minor_version;
	unsigned char magic[4];
	unsigned int text_offset;
	unsigned int text_length;
	unsigned int text_addr;
	unsigned int data_offset;
	unsigned int data_length;
	unsigned int data_addr;
	unsigned int bss_length;
	unsigned int bss_addr;
	unsigned int drv_init_entry;
	unsigned int drv_exit_entry;
	unsigned int drv_banka_file_offset;
	unsigned int drv_bankb_file_offset;
	unsigned int drv_bankc_file_offset;
	unsigned int drv_op_entry;
} drv_head_t;

typedef struct {
	uint32 bank_a_offset;
	uint32 bank_b_offset;
	uint32 bank_c_offset;
	uint32 exit_entry;
} drv_bank_info_t;

typedef struct {
	unsigned int bank_a_offset;
	unsigned int bank_b_offset;
} sys_bank_info_t;

#define MAX_DRV_GROUP_TOTAL 15
#define SUPPORT_DRV_GROUP_TOTAL 13  //��ʱֻ��11�����,���ٿ���ʡdrv_bank_info_t��Ӧ�����ݿռ�
#define API_TOTAL_ENTRY (MAX_DRV_GROUP_TOTAL+1+1)//sys+libc   vfs������һ���ӿ�
typedef enum {
	DRV_GROUP_STG_BASE = 0,
	DRV_GROUP_STG_CARD,
	DRV_GROUP_STG_UHOST,
	DRV_GROUP_FAT32,
	DRV_GROUP_FAT16,
	DRV_GROUP_EXFAT,
	DRV_GROUP_UI,
	DRV_GROUP_LCD,
	DRV_GROUP_FM,
	DRV_GROUP_KEY,//TOUCH&GSENSOR
	DRV_GROUP_I2C,
	DRV_GROUP_AUDIO_DEVICE,
	DRV_GROUP_SYS = 15,
} drv_type_t;

/*���¶���init �� exit �Ľӿڣ�����������*/
/*����ͨ��CMD������*/
/*void*  (*vfs_init)(vfs_mount_t* p_vfs_mount, void* null, void* null, void* null);*/
/*void* (*vfs_exit)(vfs_mount_t* p_vfs_mount, void* null1, void* null2, void* null3);*/

typedef int (*drv_entry_i)(void *param1, void *param2, void *param3);

extern int drv_install(uint8 drv_type, uint8 work_mode, const char* drv_name) __FAR__;
extern int drv_uninstall(unsigned char drv_type, void *null1, void* null2) __FAR__;
extern int get_drv_install_info(uint8 drv_type, void *null1, void* null2) __FAR__;

extern int detect_disk_fix(uint8 drv_type, void *null1, void* null2) __FAR__;
extern int detect_uhost_in(void* null0, void* null1, void* null2) __FAR__;
extern int detect_card(void* null0, void* null1, void* null2) __FAR__;
extern int8 detect_card_in(void* null0, void* null1, void* null2) __FAR__;
extern int detect_usb_device_in(void* null0, void* null1, void* null2) __FAR__;

#endif

#endif /*_DRIVER_H */

