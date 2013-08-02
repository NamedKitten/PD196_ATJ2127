/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    ������д�ļ��ĸ���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               Ĭ�ϴ����ļ�ʹ�ó���
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

//ϵͳĬ�ϴ����ļ�ʹ�ó���

#ifdef USE_83NAME_CREAT
static const uint8 BOOKMARK_NAME[12] = "MUSICBMKBMK";
#else
static const uint8 BOOKMARK_NAME[] = "MUSICBMK.BMK";
#endif

//����ebook����ǩʹ��
static const uint8 str[3] =
{ 0x42, 0x4D, 0x4B };
static const uint8 hex_str[8] =
{ 0x2E, 0x0, 0x42, 0x0, 0x4D, 0x0, 0x4B, 0x0 };

#ifdef USE_83NAME_CREAT
static const uint8 EBOOK_LIST_NAME[12]="EBOOK   LIB";
#else
static const uint8 EBOOK_LIST_NAME[] = "EBOOK.LIB";
#endif

uint32 g_bookmark_fp _BANK_DATA_ATTR_;

/******************************************************************************/
/*
 * \par  Description: mbyte to wchar
 *
 * \param[in]    dest��src

 * \param[out]

 * \return  UNICODE ��������������ʶ���ͽ�����

 * \note    ASCת��UNICODE���Զ������ʶ���ͽ�����
 *******************************************************************************/
static uint16 mbyte_to_wchar(uint8 *dest, uint8 *src)
{
    uint16 num = 0;

#ifdef USE_83NAME_CREAT

    while(*src!=0x00)
    {
        *dest=*src;
        dest++;
        src++;
        num++;
    }

    dest=0x00;

#else

    *dest = 0xff;
    *(dest + 1) = 0xfe;
    dest += 2;

    while (*src != 0x00)
    {
        *dest = *src;
        *(dest + 1) = 0x00;
        dest += 2;
        src++;
        num++;
    }

    *dest = 0x00;
    *(dest + 1) = 0x00;

#endif

    return num;

}

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
    if (drv_sn == 0)
    {
        g_bookmark_fp = VM_BOOKMARK_START + sector_num;
        sys_vm_write(temp_plist_buf, g_bookmark_fp);
    }
    else
    {
        vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, fp);
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
    if (drv_sn == 0)
    {
        g_bookmark_fp = VM_BOOKMARK_START + sector_num;
        sys_vm_read(temp_plist_buf, g_bookmark_fp, SECTOR_SIZE);
    }
    else
    {
        vfs_file_read(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, fp);
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

    if (drv_sn == 0)
    {
        raw_read_sector(0, g_bookmark_fp);
        bm_head_ptr = (music_bm_head *) temp_plist_buf;
        if (bm_head_ptr->magic != MAGIC_BOOKMARK)
        {
            libc_memset(temp_plist_buf, 0xff, SECTOR_SIZE);
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
        vfs_cd(ap_vfs_mount, CD_ROOT, 0);
        mbyte_to_wchar(temp_plist_buf, (uint8 *) BOOKMARK_NAME);        
        //���ļ�
        g_bookmark_fp = vfs_file_open(ap_vfs_mount, temp_plist_buf, R_NORMAL_SEEK);
        if (g_bookmark_fp == 0)
        {
            result = FALSE;
        }
        else
        {
            vfs_file_read(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, g_bookmark_fp);
            vfs_file_close(ap_vfs_mount, g_bookmark_fp);
            bm_head_ptr = (music_bm_head *) temp_plist_buf;
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
        vfs_file_dir_remove(ap_vfs_mount, temp_plist_buf, FS_FILE);
        bound_val = (BM_HEADER_SECTOR_NUM + BM_DATA_SECTOR_NUM);
        file_len = (bound_val << 9);
        //�����ļ�
        bm_fp = vfs_file_create(ap_vfs_mount, temp_plist_buf, file_len);
        if (0 == bm_fp)
        {
            //error to create file
            retval = FALSE;
        }
        else
        {
#ifdef HIDE_FILE_OPT        
            vfs_file_attralter(ap_vfs_mount, ATTR_HIDDEN, NULL, 0);
#endif            
            libc_memset(temp_plist_buf, 0xff, SECTOR_SIZE);
            bm_head_ptr = (music_bm_head *) temp_plist_buf;
            bm_head_ptr->magic = MAGIC_BOOKMARK;
            vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, bm_fp);
            bm_head_ptr->magic = 0xffff;
            for (i = 1; i < (BM_HEADER_SECTOR_NUM + BM_DATA_SECTOR_NUM); i++)
            {
                vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, bm_fp);
            }
            vfs_file_close(ap_vfs_mount, bm_fp);
        }
    }

    return retval;
}


/******************************************************************************/
/*
 * \par  Description: ��ȡebook.lib���ļ������֣�Ȼ�󴴽���Ӧ����ǩ�ļ�
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void creat_bmk(void)
{
    uint32 bmk_modify_handle, file_length;
    uint8 i, j, filename_length;//count
    uint32 tmp_page_total;
    for (i = 0; i < 2; i++)
    {
        libc_memcpy(&file_buffer[(i + 1) * 0x50], &temp_plist_buf[i * PLIST_FILE_SIZE], 0x10 * 5);
    }
    for (i = 0; i < 2; i++)
    {
        temp_count++;
        if (temp_count > ebook_total)
        {
            return;
        }
        if (file_buffer[(i + 1) * 0x50] != 0)
        {
            libc_memset(file_buffer, 0x0, 0x10 * 5);
            //byte 0-63�洢�ļ���
            libc_memcpy(file_buffer, &file_buffer[(i + 1) * 0x50], 64);
            //byte 68�洢�ļ����ĳ���
            filename_length = file_buffer[((i + 1) * 0x50) + 68];
            if ((file_buffer[0] == 0xFF) && (file_buffer[1] == 0xFE))
            {
                if ((filename_length > 5) && (filename_length <= FILENAME_LENGTH))
                {
                    libc_memcpy(&file_buffer[(filename_length * 2) - 10], hex_str, 8);
                }
                else
                {
                    return;
                }
            }
            else
            {
                if (filename_length > 3)
                {
                    for (j = 8; j > 0; j--)
                    {
                        if (file_buffer[j] == 0x2e)
                        {
                            break;
                        }
                    }
                    libc_memset(&file_buffer[j], 0x20, 9 - j);
                    libc_memcpy(&file_buffer[8], str, 3);
                    file_buffer[11] = 0x0;
                }
                else
                {
                    return;
                }
            }
            //�˴������޸�תΪ��������Ϊ�Ǹ���ȡ�����ļ���������Ӧ����ǩ
            bmk_modify_handle = vfs_file_dir_exist(ap_vfs_mount, file_buffer, 0x01);
            if (bmk_modify_handle == (uint32) NULL)
            {
                //byte 64-67�洢�ļ�����
                libc_memcpy(&file_length, &file_buffer[(i + 1) * 0x50 + 64], 0x04);

                //��ǩ�ļ��ĳ��ȼ���Ϊ��������
                tmp_page_total = file_length / 128;
                if ((file_length % 128) != 0)
                {
                    tmp_page_total++;
                }
                if ((tmp_page_total % 128) != 0)
                {
                    tmp_page_total = (tmp_page_total & 0xffffff00) + 128;
                }

                //����һ�����ȵ���ǩ�ļ���������һ������������ȫ��ʼ��Ϊ0xFF
                bmk_modify_handle = vfs_file_create(ap_vfs_mount, file_buffer, 512 + tmp_page_total * 4);

                if (0 != bmk_modify_handle)
                {
                    libc_memset(temp_plist_buf, 0xFF, 0x200);
                    vfs_file_seek(ap_vfs_mount, 0, SEEK_SET, bmk_modify_handle);
                    vfs_file_write(ap_vfs_mount, temp_plist_buf, 0x200, bmk_modify_handle);
                    vfs_file_close(ap_vfs_mount, bmk_modify_handle);
                }
            }

        }
    }
}
/******************************************************************************/
/*
 * \par  Description: �ڸ�Ŀ¼�´������е��������ǩ�ļ���ÿ���ļ���Ӧһ����ǩ�ļ�
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void creat_ebook_bmk(void)
{
    uint16 i;
    uint32 ebook_lib;
    //    uint8 read_sector;

    if (0 == drv_sn)
    {
        //nand���ʣ��������VM����,�˴�������
        return;
    }

    vfs_cd(ap_vfs_mount, CD_ROOT, 0);
    mbyte_to_wchar(temp_plist_buf, (uint8 *) EBOOK_LIST_NAME);
    ebook_lib = vfs_file_open(ap_vfs_mount, temp_plist_buf, R_NORMAL_SEEK);
    if (0x00 == ebook_lib)
    {
        return;
    }
    vfs_file_read(ap_vfs_mount, temp_plist_buf, 512, ebook_lib);
    libc_memcpy(&ebook_total, &temp_plist_buf[8], 2);
    temp_count = 0;
    if (ebook_total != 0)
    {
        for (i = 0; i < ((ebook_total - 1) / 2 + 1); i++)
        {
            libc_memset(temp_plist_buf, 0x0, 512);
            vfs_file_seek(ap_vfs_mount, (i + 1) * 512, SEEK_SET, ebook_lib);
            vfs_file_read(ap_vfs_mount, temp_plist_buf, 512, ebook_lib);
            creat_bmk();

            if (0 == (i & 0x03))
            {
                plist_get_app_msg();
            }
        }
    }

    vfs_file_close(ap_vfs_mount, ebook_lib);
    return;
}

