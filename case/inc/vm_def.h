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
#ifndef _VM_DEF_H
#define _VM_DEF_H

/* AP VRAM ���ݣ�0x00000000 - 0x00008000��֧��32��AP��1KB�� */
#define  VM_AP_SETTING          0x00000000
#define  VM_AP_MAINMENU         0x00000400
#define  VM_AP_VIDEO            0x00000800
#define  VM_AP_PICTURE          0x00000C00  
#define  VM_AP_BROWSER          0x00001000
#define  VM_AP_MUSIC            0x00001400
#define  VM_AP_MUSIC_AUDBK      0x00001600
#define  VM_AP_MENGINE          0x00001800
#define  VM_AP_EBOOK            0x00001c00
#define  VM_AP_RADIO            0x00002000
#define  VM_AP_FMENGINE         0x00002400
#define  VM_AP_RECORD           0x00002800
#define  VM_AUD_SYS             0x00002c00//����audible sys�ļ� 1K
#define  VM_AP_USER1            0x00003000//����ר��
#define  VM_AP_CONFIG           0x00003400

#define  VM_FILE_NUM            0x00003800
#define  VM_ALARM               0x00003A00
#define  VM_AP_DSC              0x00003c00

//��ǩ��ʼ��ַ��VM����1Mλ�ÿ�ʼ
#define  VM_BOOKMARK_START      0x00100000

/* AP VRAM ��������־������ͳһΪ 0x55AA */
#define  MAGIC_SETTING          0x55AA
#define  MAGIC_USER1            0x55AA
#define  MAGIC_MUSIC            0x55AA
#define  MAGIC_MENGINE          0x55AA
#define  MAGIC_CONFIG           0x55AA
#define  MAGIC_BROWSER          0x55AA
#define  MAGIC_PICTURE          0x55AA
#define  MAGIC_VIDEO            0x55AA
#define  MAGIC_ALARM            0x55AA
#define  MAGIC_FMRECORD         0x55AA
#define MAGIC_BOOKMARK          0x55AA
#define  MAGIC_AUDBK            0x55AA

/* �ļ����·����¼��ÿ�δ��������б�ʱ�������4~8KB��0x00008000 - 0x0000a000 */
#define  VM_DIR_HISTORY         0x00008000
/* �˵�·����¼��ÿ�ο������������4~8KB��0x0000a000 - 0x0000c000 */
#define  VM_MENU_HISTORY        0x0000a000

/* ENHANCED �ĸ����ʾʱ��ʱ���������(��ʾ���ʱռ��) ,16KB,0x0000c000 - 0x00010000*/
#define  VM_LYRIC_INFO          0x0000c000

/* PLAYLIST ��������Ϣ����(�������ʱռ��),256KB,0x00010000 - 0x00050000*/
#define  VM_PLAYLIST_INFO       0x00010000

//for album art info 23K, ����32K����������չ��0x00050000 - 0x00058000
#define  VM_ALBUMART_INFO       0x00050000

//for nand ����U�̣�����ղؼ���Ϣ������100K,0x0e0000 - 0x100000,��card��host���ղؼ���Ȼ���ڴ�����
#define  VM_FAVORITE_INFO   0x0e0000

#define  VM_BMK_INFO	    0x00200000
#endif
