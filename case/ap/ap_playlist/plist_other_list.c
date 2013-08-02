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
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

static const uint16 others_file_max[PL_LIB_MAX] =
{ 0, 0, PLIST_VIDEO_MAX, PLIST_PICTURE_MAX, PLIST_EBOOK_MAX, M3U_LIST_MAX };

static void sort_all_file(char *str_buf);
static void get_others_info(uint32 ext_name);
static void store_others_info(uint16 file_num);
static void get_ebook_info(uint32 ext_name);

extern void init_file_index(void) __FAR__;
extern void copy_deal_string(char *dest,char * source,uint8 land_id,uint8 size) __FAR__;
extern void list_sort(char *base, uint16 num, uint16 width,void *depond_buf,uint8 cmp_type,uint8 cmp_len) __FAR__;
extern void build_ex_index(uint16 *ex_buf) __FAR__;

extern bool plist_fsel_get_nextfile (char *strfile) __FAR__;

//���˿ո�,ת��Ϊ xx.yyy��ʽ
//�����ַ����ȣ�������������
static void deal_if_83name(uint8 *nbuf)
{
    uint8 i, j;
    uint8 *name_buf = nbuf;

    if ((name_buf[0] == 0xff) && (name_buf[1] == 0xfe))
    {
        return;
    }

    //��չ��������1���ֽ�
    for (i = 11; i > 8; i--)
    {
        name_buf[i] = name_buf[i - 1];
    }

    //�����ļ����ո�
    for (i = 7; i > 0; i--)
    {
        if (name_buf[i] != 0x20)
        {
            break;
        }
    }

    //�ӵ�
    i++;
    name_buf[i] = 0x2e; //��"."

    //������չ���ո�
    for (j = 9; j < 12; j++)
    {
        if (name_buf[j] != 0x20)
        {
            i++;
            name_buf[i] = name_buf[j];
        }
    }

    if (name_buf[i] != 0x2e)
    {
        i++;
    }

    name_buf[i] = 0x00;

    return;
}

/******************************************************************************/
/*
 * \par  Description: ��ȡ�ҵ��ĵ������ļ���Ϣ(�ļ������ļ���С���ļ������ȣ�
 *                             ·������չ����)
 *
 * \param[in]    ext_name--��չ��

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void get_ebook_info(uint32 ext_name)
{
    uint8 filename_length;
    plist_ebook_info_t ebook_info;
    filename_length = (uint8) vfs_get_name(ap_vfs_mount, &ebook_info.file_name, ID3_ITEM_SIZE);
    vfs_file_get_size(ap_vfs_mount, &ebook_info.file_length, (void*) NULL, 1);
    ebook_info.filename_lenth = (char) filename_length;
    *(uint32*) (other_f_p->filename) = ext_name;
    vfs_file_dir_offset(ap_vfs_mount, &other_f_p->dir_layer_info, &other_f_p->cluster_no, 0);
    other_f_p->flag = 1;
    libc_memcpy(other_f_p, &ebook_info, sizeof(plist_ebook_info_t));
    deal_if_83name(other_f_p->title_string);
    return;
}

/******************************************************************************/
/*
 * \par  Description: ��ȡ�ҵ����ļ���Ϣ(·������չ����)
 *
 * \param[in]    ext_name--��չ��

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void get_others_info(uint32 ext_name)
{
    uint8 i, len;
    len = (uint8) vfs_get_name(ap_vfs_mount, other_f_p->title_string, (ID3_ITEM_SIZE * 2) / 2);

    if (lib_num == 5)
    {
        //m3u.lib
        //m3u�б��ļ�����ʾ��׺��
        if ((uint8) other_f_p->title_string[0] != 0xff)
        {
            //����
            other_f_p->title_string[8] = 0x20;
            other_f_p->title_string[9] = 0x20;
            other_f_p->title_string[10] = 0x20;
        }
        else
        {
            //����
            len = (len - 1) << 1;
            for (i = len - 8; i < len; i = i + 2)
            {
                if ((uint8) other_f_p->title_string[i] == 0x2e)
                {
                    other_f_p->title_string[i] = 0x00;
                    break;
                }
            }
        }

    }

    deal_if_83name(other_f_p->title_string);

    *(uint32*) (other_f_p->filename) = ext_name;
    vfs_file_dir_offset(ap_vfs_mount, &other_f_p->dir_layer_info, &other_f_p->cluster_no, 0);
    other_f_p->flag = 1;
    return;
}
/******************************************************************************/
/*
 * \par  Description: ��������õ��ļ���Ϣ��������Ϣ
 *
 * \param[in]    file_num--��ǰ�Ѵ�ŵ��ļ�����

 * \param[out]

 * \return

 * \note        ���ж�buffer�Ƿ����ˣ���д��
 *******************************************************************************/
static void store_others_info(uint16 file_num)
{
    if (file_num != 0)
    {
        if (file_num == PLIST_SAVE_MAX)
        {
            write_to_vram(title_vram_pt, (uint8 *) TITLE_DATA_ADDR, TITLE_BUF_SIZE);//32k
            title_vram_pt = title_vram_pt + TITLE_BUF_SIZE;
            title_data_pt = (uint8 *) TITLE_DATA_ADDR;
        }
        else
        {
            title_data_pt += TITLE_CMP_LENGTH;
        }

        if ((file_num % (FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE)) == 0)
        {
            save_to_file((uint8*) FILE_INFO_ADDR, FILE_INFO_BUF_SIZE);
            fileinfo_data_pt = (uint8*) FILE_INFO_ADDR;
        }
        else
        {

            fileinfo_data_pt += PLIST_FILE_SIZE;

        }
    }

    other_f_p->prev_offset = file_num - 1;
    other_f_p->next_offset = file_num + 1;

    copy_deal_string(title_data_pt, other_f_p->title_string, language_id, CMPSTR_MAX_LEN1);
    libc_memcpy(fileinfo_data_pt, other_f_p, sizeof(plist_f_info_t));

    return;
}

/******************************************************************************/
/*
 * \par  Description:ɨ�� �����ϵ��ļ�
 *
 * \param[in]

 * \param[out]

 * \return        �ļ�������

 * \note
 *******************************************************************************/
uint16 scan_others_info(void)
{
    uint32 temp;
    uint32 ext_name;
    //    uint16 music_num;

    other_f_p = (plist_f_info_t *) file_buffer;

#ifdef ENHANCED_FIND_MODE
    while(FALSE!=fsel_get_nextfile((char*)&ext_name))
#else
    while (FALSE != plist_fsel_get_nextfile((char*) &ext_name))
#endif
    {
        if (0 == (ap_plist_file_total & 0x07))
        {
            plist_get_app_msg();
        }
        libc_memset(other_f_p, 0x00, sizeof(plist_f_info_t));
        //��չ��Ϊ������ʱ�Ĵ���
        if (lib_num == 0x04)//TEXT_BITMAP
        {
            get_ebook_info(ext_name);
        }
        else
        {
            get_others_info(ext_name);
        }
        store_others_info(ap_plist_file_total);
        ap_plist_file_total++;
        if (ap_plist_file_total >= others_file_max[lib_num])
        {
            break;
        }
    }

    if (ap_plist_file_total > PLIST_SAVE_MAX)
    {
        temp = (ap_plist_file_total % PLIST_SAVE_MAX) * TITLE_CMP_LENGTH;
        write_to_vram(title_vram_pt, (uint8*) TITLE_DATA_ADDR, temp);
    }

    if (ap_plist_file_total > 0)
    {
        temp = ap_plist_file_total % (FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE);
        if (temp == 0)
        {
            temp = FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE;
        }
        save_to_file((uint8 *) FILE_INFO_ADDR, temp * PLIST_FILE_SIZE);
    }
    plist_get_app_msg();
    return ap_plist_file_total;
}

/******************************************************************************/
/*
 * \par  Description: �����б�
 *
 * \param[in]

 * \param[out]

 * \return        NULL

 * \note
 *******************************************************************************/
void sort_others_list(void)
{
    uint8 *str_buf;

    //clear buffer
    libc_memset(file_index_buf, 0, FILE_INFO_BUF_SIZE);

    sort_file_total = ap_plist_file_total;

    plist_get_app_msg();
    if (sort_file_total > PLIST_SAVE_MAX)
    {
        str_buf = (uint8 *) (TRACK_DATA_ADDR - TITLE_VRAM_SIZE);//64k;
        read_from_vram(str_buf, (uint8 *) TITLE_VRAM_ADDR, sort_file_total * TITLE_CMP_LENGTH);
    }
    else
    {
        str_buf = (uint8 *) TITLE_DATA_ADDR;
    }

    sort_all_file(str_buf);
    plist_get_app_msg();

}

/******************************************************************************/
/*
 * \par  Description: �����ļ���
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void sort_all_file(char *str_buf)
{
    init_file_index();
    list_sort((void*) file_index_buf, sort_file_total, 2, str_buf, SORT_CMP_STRING, CMPSTR_MAX_LEN1); //64+8k

    //������title����������buffer����Ϊ��title������������
    libc_memset(sort_index_buf, 0, INDEX_BUFFER_SIZE);

    plist_get_app_msg();

    build_ex_index(sort_index_buf); //8+8k

    ap_file_index_offset[0] = ap_pos_file_offset;
    save_to_file((void*) file_index_buf, sort_file_total * 2);

    ap_sort_index_offset[0] = ap_pos_file_offset;
    save_to_file((void*) sort_index_buf, sort_file_total * 2);

    return;
}

