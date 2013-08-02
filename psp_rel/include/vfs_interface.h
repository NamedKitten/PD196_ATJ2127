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
 * \file     vfs_interface.h
 * \brief    �����ļ�ϵͳ�ӿ�
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __VFS_INTERFACE_H__
#define __VFS_INTERFACE_H__

#include <storage_device.h>
#include <vfs.h>
#include <attr.h>
#include <typeext.h>

#define FORMAT_FAT32 0
#define FORMAT_FAT16 1
#define FORMAT_EXFAT 2

//����CD����Ĳ�������
#define  CD_UP			'\\'
#define  CD_BACK		'|'
#define  CD_ROOT		':'
#define  CD_SUB			'/'

//����DIR����Ĳ�������
#define  DIR_NEXT			0x80
#define  DIR_PREV			0x81
#define  DIR_HEAD			0x82
#define  DIR_TAIL			0x83

//����DIR ��չ����ӳ��
#define  EXT_BITMAP_BUFF		0x80000000
#define  EXT_NAME_MP3	        (0x80000000>>1)
#define  EXT_NAME_WMA	        (0x80000000>>2)
#define  EXT_NAME_WAV	        (0x80000000>>3)
#define  EXT_NAME_AAC	        (0x80000000>>4)
#define  EXT_NAME_FLAC        	(0x80000000>>5)
#define  EXT_NAME_APE	        (0x80000000>>6)
#define  EXT_NAME_AVI	        (0x80000000>>7)
#define  EXT_NAME_TXT	        (0x80000000>>8)
#define  EXT_NAME_JPG	        (0x80000000>>9)
#define  EXT_NAME_JPEG        	(0x80000000>>10)
#define  EXT_NAME_BMP	        (0x80000000>>11)
#define  EXT_NAME_GIF	        (0x80000000>>12)
#define  EXT_NAME_LRC	        (0x80000000>>13)
#define  EXT_NAME_ASF	        (0x80000000>>14)
#define  EXT_NAME_OGG	        (0x80000000>>15)
#define  EXT_NAME_WMV	        (0x80000000>>16)
#define  EXT_NAME_AMV	        (0x80000000>>17)
#define  EXT_NAME_AAX	        (0x80000000>>18)
#define  EXT_NAME_AA 	        (0x80000000>>19)
#define  EXT_NAME_MP1	        (0x80000000>>20)
#define  EXT_NAME_MP2	        (0x80000000>>21)
#define  EXT_NAME_M4A	        (0x80000000>>22)
#define  EXT_NAME_POS	        (0x80000000>>23)
#define  EXT_NAME_ACT	        (0x80000000>>24)   /* ACT is added by fix FS_ROM rom_ext_name.  2013/3/13 14:42:24. */
#define  EXT_NAME_ALL			(0x80000000>>29)
#define  EXT_NAME_ALL_FILE 		(0x80000000>>30)
#define  EXT_NAME_ALL_DIR 		(0x80000000>>31)

//����OPEN�ļ��ķ�ʽ
#define  R_NORMAL_SEEK		0x00
#define  R_FAST_SEEK		0x01
#define  OPEN_MODIFY		0x02
#define  LARGE_FILE_SEEK	0x03
#define  OPEN_RECOVER		0x04


//�����ļ���Ŀ¼����������
#define  FS_DIR				0x00
#define  FS_FILE			0x01


//����ʱ�����������
#define  FS_TIME_CREATE		0X00
#define  FS_TIME_MODIFY		0x01

typedef struct {
	uint16 year;
	uint8 month;
	uint8 day;
	uint8 hour;
	uint8 minute;
	uint8 second;
} file_time_t;

//�����ļ�SEEK�ķ�ʽ
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2


//�����ȡ�����ռ������
#define  GET_TOTAL_SPACE			0x00
#define  GET_SPARE_SPACE			0x01

//�����ļ�����
#define         ATTR_NORMAL         0x00    // normal file
#define         ATTR_READONLY       0x01    // file is readonly
#define         ATTR_HIDDEN         0x02    // file is hidden
#define         ATTR_SYSTEM         0x04    // file is a system file
#define         ATTR_VOLUME         0x08    // entry is a volume label
#define         ATTR_LONG_FILENAME  0x0f    // this is a long filename entry
#define         ATTR_DIRECTORY      0x10    // entry is a directory name
#define         ATTR_ARCHIVE        0x20    // file is new or modified

//�������֧��Ŀ¼�㼶
#define  MAX_DIR_LAYER		9

#define  LONG_NAME_TAG		0xfeff

//����Ŀ¼�㼶�ṹ���ڴ�ռ�����
typedef struct {
	uint8 buffer[MAX_DIR_LAYER*8+1];
} pdir_layer_t;

typedef struct {
	uint8 buffer[MAX_DIR_LAYER*8+2];
} pdir_layer_ext_t;

typedef struct {
	uint8 buffer[8];
} pfile_offset_t;


//�����ļ�ϵͳ��������
#define  ERR_PHY_R          		0x02   //���̶�����   
#define  ERR_PHY_W          		0x03   //����д����  
#define  ERR_NO_FORMAT          	0x04   //����δ��ʽ��   
#define  ERR_WRITE_PROTECTED    	0x05   //����д����     
#define  ERR_FILE_OVER_OFFSET		0x06   //�ļ����������ļ��߽�   
#define  ERR_DIR_OVER_OFFSET    	0x07   //Ŀ¼��������Ŀ¼�߽�    
#define  ERR_FILE_NO_EXIST    		0x08   //�ļ�������   
#define  ERR_DIR_NO_EXIST    		0x09   //Ŀ¼������       
#define  ERR_FILE_OP_SPACE    		0x10   //��ʾ�ļ�����ʱû�д��̿ռ�,����д����   
#define  ERR_DIR_OP_SPACE    		0x11   //Ŀ¼����ʱû�д��̿ռ�,������չĿ¼,�½���Ŀ¼   
#define  ERR_FILE_OP_ROOT_FULL		0x12   //Ŀ¼������       
#define  ERR_DEL_DIR_NO_EMPTY		0x13   //ɾ��Ŀ¼ʱ����,��ʾɾ����Ŀ¼�ǿ�        
#define  ERR_DIRENTRY_ATTR			0x14
#define  ERR_DIR_TYPE               0x15  
#define  ERR_FHANDLE_FULL			0x16
#define  ERR_DIR_EXIST              0x17   //Ŀ¼�Ѵ���(����Ŀ¼ʱ)
#define  ERR_FILE_EXIST             0x18   //�ļ��Ѵ��ڣ������ļ�ʱ��
#define  ERR_VAILD_PARA				0x19   //�Ƿ��Ĳ���

typedef enum {
    VFS_CD = 0,
    VFS_DIR,
    VFS_GET_NAME,
    VFS_FILE_DIR_OFFSET,
    VFS_MAKE_DIR,
    VFS_FILE_OPEN,
    VFS_FILE_CREATE,
    VFS_FILE_CLOSE,
    VFS_FILE_DIR_REMOVE,
    VFS_FILE_GET_SIZE,
    VFS_GET_TIME,
    VFS_FILE_SEEK,
    VFS_FILE_TELL,
    VFS_FILE_READ,
    VFS_FILE_WRITE,
    VFS_GET_SPACE,
    VFS_GET_ERR_INFO,
    VFS_FILE_DIR_EXIST,
    VFS_FILE_CUT,
    VFS_FILE_ATTRALTER,
    VFS_FILE_RENAME,
    VFS_CREATEVOLUME,
    VFS_SET_TIME,
    VFS_SET_PRIVATE_DATA,
    VFS_CLEAR_PRIVATE_DATA,
    VFS_FILE_MOVE,
    VFS_DIR_FILE,
    VFS_JUMP_DIR
}vfs_cmd_e;

extern void * vfs_op_entry(void *vfs_mount_index,void *param1, void *param2, void *param3,  vfs_cmd_e cmd) __FAR__;


#define vfs_cd(a,b,c)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(uint32)(b), (void*)(c), (void*)(0), VFS_CD)
#define vfs_dir(a,b,c,d)  (uint32)vfs_op_entry((void*)(a), (void*)(uint32)(b), (void*)(c), (void*)(d), VFS_DIR)
#define vfs_get_name(a,b,c)  (uint16)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(uint32)(c), (void*)(0), VFS_GET_NAME)
#define vfs_file_dir_offset(a,b,c,d)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(c), (void*)(uint32)(d), VFS_FILE_DIR_OFFSET)
#define vfs_make_dir(a,b)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(0), (void*)(0), VFS_MAKE_DIR)
#define vfs_file_open(a,b,c)  (uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(uint32)(c), (void*)(0), VFS_FILE_OPEN)
#define vfs_file_create(a,b,c)  (uint32)vfs_op_entry((void*)(a), (void*)(b),(void*)(c), (void*)(0), VFS_FILE_CREATE)
#define vfs_file_close(a,b)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(0), (void*)(0), VFS_FILE_CLOSE)
#define vfs_file_dir_remove(a,b,c)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(uint32)(c), (void*)(0), VFS_FILE_DIR_REMOVE)
#define vfs_file_get_size(a,b,c,d) (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(c), (void*)(uint32)(d), VFS_FILE_GET_SIZE)
#define vfs_get_time(a,b,c,d)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(c), (void*)(uint32)(d), VFS_GET_TIME)
#define vfs_file_seek(a,b,c,d)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(uint32)(c), (void*)(d), VFS_FILE_SEEK)
#define vfs_file_tell(a,b,c)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(c), (void*)(0), VFS_FILE_TELL)
#define vfs_file_read(a,b,c,d)  (uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(c), (void*)(d), VFS_FILE_READ)
#define vfs_file_write(a,b,c,d)  (uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(c), (void*)(d), VFS_FILE_WRITE)
#define vfs_get_space(a,b,c)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(uint32)(c), (void*)(0), VFS_GET_SPACE)
#define vfs_get_err_info(a)  (uint8)(uint32)vfs_op_entry((void*)(a), (void*)(0), (void*)(0), (void*)(0), VFS_GET_ERR_INFO)
#define vfs_file_dir_exist(a,b,c)  (uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(uint32)(c), (void*)(0), VFS_FILE_DIR_EXIST)
#define vfs_file_cut(a,b,c,d)  (uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(c), (void*)(d), (void*)(0), VFS_FILE_CUT)
#define vfs_file_attralter(a,b,c,d)  (uint8)(uint32)vfs_op_entry((void*)(a), (void*)(uint32)(b), (void*)(c), (void*)(uint32)(d), VFS_FILE_ATTRALTER)
#define vfs_file_rename(a,b,c,d)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(c), (void*)(uint32)(d), VFS_FILE_RENAME)
#define vfs_create_volume(a,b)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(0), (void*)(0), VFS_CREATEVOLUME)
#define vfs_set_time(a,b,c,d)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(c), (void*)(uint32)(d), VFS_SET_TIME)
#define vfs_file_move(a,b,c,d)  (bool)(uint32)vfs_op_entry((void*)(a), (void*)(b), (void*)(c), (void*)(uint32)(d), VFS_FILE_MOVE)
#define vfs_dir_current_entry_file(a,b,c,d) (uint32)vfs_op_entry((void*)(a), (void*)(uint32)(b), (void*)(c), (void*)(uint32)(d), VFS_DIR_FILE)
#define vfs_jump_to_direntry(a,b,c,d) (bool)(uint32)vfs_op_entry((void*)(a), (void*)(uint32)(b), (void*)(c), (void*)(d), VFS_JUMP_DIR)

/*sys_op*/
extern int format_disk(uint8 dry_type, uint8 partition_num, uint8 fat_type);
extern int mount_fs(unsigned char drv_type, uint8 disk, uint8 work_mode);
extern int unmount_fs(unsigned char vfs_mount_index, void* null1, void* null2);
extern uint8 get_fat_type_after_mount(unsigned char vfs_mount_index,
		void* null1, void* null2);
extern int parse_dbr(unsigned char drv_type, uint8 partition_num) __FAR__;
extern int parse_dbr_exfat(unsigned char drv_type, uint8 disk) __FAR__;





#endif



