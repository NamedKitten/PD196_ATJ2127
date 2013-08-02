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
 * \file     eh_bs_ebook_deal.c
 * \brief    ebook�����ļ�
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ɾ��ebookʱ��ɾ�����Ӧ����ǩ�ļ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               �ļ�ϵͳ
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_bs.h"

static uint8 bs_ebook_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;

static const uint8 hex_str[8] =
{ 0x2E, 0x0, 0x42, 0x0, 0x4D, 0x0, 0x4B, 0x0 };//.bmk�ַ�
static const uint8 bmk_str[3] =
{ 0x42, 0x4D, 0x4B };//".bmk"�ַ�

/********************************************************************************
 * Description :��ȡ��Ҫ��������ǩ����
 *
 * Arguments  :
 * Returns     :
 *                ��
 * Notes       :
 *			  �ַ����ֵĳ��ȱ����ap_play_list�д�����ǩ�ĳ���һ��������
 *			  ����Ӧ�������´�����һ�����Ȳ�ͬ����ǩ�ļ���
 ********************************************************************************/
static bool _get_bmk_name(uint8 *name_buf, uint16 filename_length)
{
    //�ж��ļ����Ƿ�Ϊ����
    if ((name_buf[0] == 0xFF) && (name_buf[1] == 0xFE))
    {
        if ((filename_length > 7) && (filename_length <= FILENAME_LENGTH))
        {
            //��ȡ��Ҫ������ǩ�ĳ���
            libc_memcpy(&name_buf[filename_length * 2 - 10], hex_str, 8);
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if ((filename_length > 4) && (filename_length <= (FILENAME_LENGTH * 2)))
        {
            //��ȡ��Ҫ������ǩ�Ķ���
            libc_memcpy(&name_buf[filename_length - 3], bmk_str, 3);
        }
        else
        {
            return FALSE;
        }

    }

    return TRUE;
}

/********************************************************************************
 * Description :ɾ��ebook����ǩ
 *
 * Arguments  :
 * Returns    :
 *                ��
 * Notes      :
 *			  �ַ����ֵĳ��ȱ����ap_play_list�д�����ǩ�ĳ���һ��������
 *			  ����Ӧ�������´�����һ�����Ȳ�ͬ����ǩ�ļ�
 ********************************************************************************/
bool bs_del_ebookbmk(void)
{
    bool result;
    uint8 *bmk_buf;
    uint16 name_length;

    if (fsel_init_val.disk == DISK_C)
    {
        //nand ���ʣ�VRAM�ϵ���ǩ������
        return FALSE;
    }

    //    ʹ��bs_ebookbmk_buf��256~511�ռ�
    bmk_buf = (uint8 *) (bs_ebook_buf + 256);

    //���ݵ�ǰλ�õ�ebook���������Ӧ��ǩ���ļ�����
    name_length = vfs_get_name(vfs_mount, bmk_buf, FILENAME_LENGTH);
    //��ȡ��ǩ�ļ���
    result = _get_bmk_name(bmk_buf, name_length);

    if (FALSE != result)
    {
        vfs_file_dir_offset(vfs_mount, bs_ebook_buf, bs_ebook_buf + 128, 0);
        //��λ����Ŀ¼,BMKĬ���ڸ�Ŀ¼
        vfs_cd(vfs_mount, CD_ROOT, 0);
        result = vfs_file_dir_remove(vfs_mount, bmk_buf, FS_FILE);

        vfs_file_dir_offset(vfs_mount, bs_ebook_buf, bs_ebook_buf + 128, 1);

    }

    return result;

}

