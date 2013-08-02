/*******************************************************************************
 *                              US212A
 *                            Module: MUSIC_CREATE_BOOKMARK
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-11-9 16:45:04           1.0              build this file
 *******************************************************************************/

#include "app_music.h"
#include "app_music_bookmark.h"
#include "music_common.h"

//��.xdata�Σ����ļ�.xdata������bank��
static uint8 bm_buf[BM_BUFF_SIZE] _BANK_DATA_ATTR_;

static const uint8 BOOKMARK_NAME[] =
{
    0xff, 0xfe, //unicode flag
    0x4D, 0x00, //M
    0x55, 0x00, //U
    0x53, 0x00, //S
    0x49, 0x00, //I
    0x43, 0x00, //C
    0x42, 0x00, //B
    0x4D, 0x00, //M
    0x4B, 0x00, //K
    0x2E, 0x00, //.
    0x42, 0x00, //B
    0x4D, 0x00, //M
    0x4B, 0x00, //K
    0x00, 0x00 //end flag
};
/******************************************************************************/
/*!
 * \par  Description:
 * \void raw_write_sector(uint32 sector_num, handle fp)
 * \дһ�����������ݵ��ļ�
 * \param[in]   sector_num ������� param1
 * \param[in]   fp �ļ���� param2
 * \param[out]  none
 * \return      none
 * \retval      none
 * \ingroup     music_bookmark
 * \note
 *******************************************************************************/
static void raw_write_sector(uint32 sector_num, handle fp)
{
    sector_num <<= 9;
    if (g_browser_filepath.file_path.plist_location.disk == DISK_C)
    {
        g_bookmark_fp = VM_BOOKMARK_START + sector_num;
        sys_vm_write(bm_buf, g_bookmark_fp);
    }
    else
    {
        vfs_file_write(g_music_mount_id, bm_buf, BM_BUFF_SIZE, fp);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void raw_read_sector(uint32 sector_num, handle fp)
 * \��һ������������
 * \param[in]   sector_num ������� param1
 * \param[in]   fp �ļ���� param2
 * \param[out]  none
 * \return      none
 * \ingroup     music_bookmark
 * \note
 *******************************************************************************/
static void raw_read_sector(uint32 sector_num, handle fp)
{
    sector_num <<= 9;
    if (g_browser_filepath.file_path.plist_location.disk == DISK_C)
    {
        g_bookmark_fp = VM_BOOKMARK_START + sector_num;
        sys_vm_read(bm_buf, g_bookmark_fp, BM_BUFF_SIZE);
    }
    else
    {
        vfs_file_read(g_music_mount_id, bm_buf, BM_BUFF_SIZE, fp);
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void bool check_bookmark_exit_sub(void)
 * \�����ǩ�ļ��Ƿ����
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \retval
 * \ingroup     music_bookmark
 * \note
 * \    ����Ҫд��VM����ǩ�ļ�������жϲ����ڣ���������������д��
        ���̵���ǩ�ļ�������ļ������ڣ��򷵻�FALSE,�ɽ������ĳ��򴴽�֮ 
 *******************************************************************************/
static bool check_bookmark_exit_sub(void)
{
    music_bm_head *bm_head_ptr;
    uint8 i;
    bool result = TRUE;

    if (g_browser_filepath.file_path.plist_location.disk == DISK_C)
    {
        raw_read_sector(0, g_bookmark_fp);
        bm_head_ptr = (music_bm_head *) bm_buf;
        if (bm_head_ptr->magic != MAGIC_BOOKMARK)
        {
            libc_memset(bm_buf, 0xff, BM_BUFF_SIZE);
            bm_head_ptr->magic = MAGIC_BOOKMARK;
            raw_write_sector(0, g_bookmark_fp);
            bm_head_ptr->magic = 0xffff;
            for (i = 1; i < (BM_HEADER_SECTOR_NUM + BM_DATA_SECTOR_NUM); i++)
            {
                raw_write_sector(i, g_bookmark_fp);
            }
        }
    }
    else
    {
        //��λ����Ŀ¼
        vfs_cd(g_music_mount_id, CD_ROOT, 0);
        //���ļ�
        g_bookmark_fp = vfs_file_open(g_music_mount_id, BOOKMARK_NAME, R_NORMAL_SEEK);
        if (g_bookmark_fp == 0)
        {
            result = FALSE;
        }
        else
        {
            vfs_file_read(g_music_mount_id, bm_buf, BM_BUFF_SIZE, g_bookmark_fp);
            vfs_file_close(g_music_mount_id, g_bookmark_fp);
            bm_head_ptr = (music_bm_head *) bm_buf;
            if (bm_head_ptr->magic != MAGIC_BOOKMARK)
            {
                result = FALSE;
            }
        }
    }

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  bool check_bookmark_exist(void)
 *	  �����ǩ�ļ��ֻ�����ڣ��粻���ڣ���ᴴ���ļ�
 * \param[in]   handle *fp ����򿪳ɹ������ش򿪵ľ��
 * \param[out] none
 * \return     bool
 * \retval     TRUE �ļ����� or �����ɹ�
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/
bool check_bookmark_exist(void)
{
    uint32 bm_fp;
    uint32 file_len;
    uint16 i;
    uint16 bound_val;
    music_bm_head *bm_head_ptr;
    bool retval = TRUE;

    if (FALSE == check_bookmark_exit_sub())
    {
        //���ļ�ʧ��
        vfs_file_dir_remove(g_music_mount_id, BOOKMARK_NAME, FS_FILE);
        bound_val = (BM_HEADER_SECTOR_NUM + BM_DATA_SECTOR_NUM);
        file_len = (bound_val << 9);
        //�����ļ�
        bm_fp = vfs_file_create(g_music_mount_id, BOOKMARK_NAME, file_len);
        if (0 == bm_fp)
        {
            //error to create file
            retval = FALSE;
        }
        else
        {
            vfs_file_attralter(g_music_mount_id, ATTR_HIDDEN, NULL, 0);
            libc_memset(bm_buf, 0xff, BM_BUFF_SIZE);
            bm_head_ptr = (music_bm_head *) bm_buf;
            bm_head_ptr->magic = MAGIC_BOOKMARK;
            vfs_file_write(g_music_mount_id, bm_buf, BM_BUFF_SIZE, bm_fp);
            bm_head_ptr->magic = 0xffff;
            for (i = 1; i < (BM_HEADER_SECTOR_NUM + BM_DATA_SECTOR_NUM); i++)
            {
                vfs_file_write(g_music_mount_id, bm_buf, BM_BUFF_SIZE, bm_fp);
            }
            vfs_file_close(g_music_mount_id, bm_fp);
        }
    }

    return retval;
}

