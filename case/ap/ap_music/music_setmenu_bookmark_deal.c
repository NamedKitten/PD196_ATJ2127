/*******************************************************************************
 *                              US212A
 *                            Module: BOOKMARK
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-9-25 17:17:29           1.0              build this file
 *******************************************************************************/
/********************************************************************************
 * \note    ��ǩ�ļ�����:
 * \note�ʼ4�ֽ� ħ��  4bytes
 * \notelast_access_index 2bytes
 * \notetotal_index       2bytes
 * \notecluster_no        4bytes
 * \notedir_entry         4bytes
 *********************************************************************************/

#include "app_music.h"
#include "app_music_bookmark.h"

//��.xdata�Σ����ļ�.xdata������bank��

uint8 bm_buf[BM_BUFF_SIZE] _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \���ݴ����cluster_no��dir_entry,����ǩ������Ѱ�Ҷ�Ӧ�����������
 * \param[in]    cluster_no �ļ�����Ŀ¼��غ�
 * \param[in]    dir_entry  �ļ�ƫ��
 * \param[in]    bm_fp      ��ǩ���
 * \param[in]    mode       ����ģʽ mode = 1,��������ɾ������������Ի���
 * \param[out]   none
 * \return       index      �����ҵ������������
 * \retval           0xffff δ�ҵ���Ӧ�����
 * \retval           other  ���������
 * \ingroup      music_bookmark
 * \note
 */
/*******************************************************************************/
uint16 find_index(uint32 cluster_no, uint32 dir_entry, uint32 bm_fp, uint8 mode)
{
    uint8 search_mode;
    uint16 i;
    uint16 file_index = 0xffff;
    uint16 loop_cnt;

    //�������ݽṹ
    mbmk_index_t* bmk_index_item;

    bmk_index_item = (mbmk_index_t*) (bm_buf);

    search_mode = 0;

    retry: read_sector(0, bm_fp);

    bmk_index_item += 2;

    loop_cnt = bm_head.total_index + bm_head.del_total;

    for (i = 2; i < (loop_cnt + 2); i++)
    {
        if (search_mode == 0)
        {
            if ((cluster_no == bmk_index_item->bmk_clus_no) && (dir_entry == bmk_index_item->bmk_dir_eny))
            {
                //��ȡ�������������ַ
                file_index = i - 2;
                break;
            }
        }
        else
        {
            if ((0xffffffff == bmk_index_item->bmk_clus_no) && (0xffffffff == bmk_index_item->bmk_dir_eny))
            {
                //��ǰ���ɾ��״̬����������øÿ�����
                file_index = i - 2;
                bm_head.del_total--;
                break;
            }
        }

        bmk_index_item++;

        if ((i % BM_SECTOR_PER_SORT_INDEX) == 0)
        {
            file_read(bm_buf, SEC_SIZE_USE, bm_fp);
            bmk_index_item = (mbmk_index_t*) (bm_buf);
            bmk_cache_sector++;
        }
    }

    if ((file_index == 0xffff) && (mode == 1) && (bm_head.del_total != 0))
    {
        //search only once
        mode = 0;
        search_mode = 1;
        goto retry;
    }

    return file_index;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \�����ҵ������,������������д����ǩ������
 * \param[in]    cluster_no �ļ�����Ŀ¼��غ�
 * \param[in]    dir_entry  �ļ�ƫ��
 * \param[in]    index      ��ǩ���
 * \param[in]    bm_fp      ��ǩ�ļ����
 * \param[out]   none
 * \return       none
 * \ingroup      music_bookmark
 * \note
 */
/*******************************************************************************/
static void write_index(uint32 cluster_no, uint32 dir_entry, uint16 index, uint32 bm_fp)
{
    //�������ݽṹ
    mbmk_index_t* bmk_index_item;

    //��֮ǰ���һ��������ĺ����Ϊ��index
    if (bm_head.last_index != 0xffff)
    {
        bmk_index_item = read_bmk_index(bm_head.last_index, bm_fp);
        bmk_index_item->next = index;
        write_bmk_index(bm_head.last_index, bm_fp);
    }

    //���ø�indexǰ��ָ�����һ��������
    bmk_index_item = read_bmk_index(index, bm_fp);
    //����������
    bmk_index_item->bmk_clus_no = cluster_no;
    bmk_index_item->bmk_dir_eny = dir_entry;
    bmk_index_item->prev = bm_head.last_index;
    bmk_index_item->next = 0xffff;
    //���������ݻ�д���ļ�
    write_bmk_index(index, bm_fp);
    bm_head.total_index++;

    //�⼸�仰�����ᵽǰ��ȥ
    if (bm_head.last_index == 0xffff)
    {
        //��ʼ��ͷָ���βָ��
        bm_head.last_index = bm_head.first_index = index;
    }
    else
    {
        //���õ�ǰ������βָ��
        bm_head.last_index = index;
    }
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \�����ҵ������,д����ǩ����������
 * \param[in]    index      ��ǩ���
 * \param[in]    name_str   ��ǩ�ļ���
 * \param[in]    bm_fp      ��ǩ�ļ����
 * \param[out]   none
 * \return       none
 * \ingroup      music_bookmark
 * \note
 */
/*******************************************************************************/

static bool write_bmk_item(uint16 index, uint8 *name_str, uint32 bm_fp)
{
    uint32 i;
    uint16 sector_no;
    bool is_write_bmk = FALSE;
    uint8 data_offset = (uint8)(index % 2);

    //�������ṹ��
    mfile_bm_items_t* bm_data_ptr = (mfile_bm_items_t*) (bm_buf);

    //�鿴��ǰ��index�����������Ƿ�ƥ��

    //δ�ҵ������ҵ��˵��ļ�����ƥ��ʱ��Ҫ���ļ���д��
    //������������������

    sector_no = BM_HEADER_SECTOR_NUM + (index >> 1);
    //��λ��������
    read_sector(sector_no, bm_fp);
    bm_data_ptr += data_offset;

    //�Ƚ��ļ���,δ�ҵ�ʱ�ļ���ͬ����ƥ��
    i = (uint32) libc_strlen(name_str);
    if (libc_memcmp(bm_data_ptr->mfile_name, name_str, i) != 0)
    {
        //���������
        libc_memset(bm_data_ptr->mfile_bmk_items, 0xff, MAX_BOOKMARKS * sizeof(music_bm_item));
        //д���ļ���
        libc_memcpy(bm_data_ptr->mfile_name, name_str, i);
        write_sector(sector_no, bm_fp);
#if 0
        //TODO:�Ҿ������ﻹ�����⣬�������һ�������ļ������Ч��?
        sector_no++;
        if (sector_no < (BM_HEADER_SECTOR_NUM + BM_DATA_SECTOR_NUM))
        {
            libc_memset(bm_buf, 0xff, BM_BUFF_SIZE);
            write_sector(sector_no, bm_fp);
        }
#endif
        is_write_bmk = TRUE;
    }
    return is_write_bmk;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 get_bookmark_index(uint32 cluster_no, uint32 dir_entry,uint8* name_str)
 * \��ȡ��ǰ��ǩ���ڵ�index
 * \param[in]  bm_fp��ǩ��� param1
 * \param[in]  cluster_no��ǩ���ڵĴغ� param2
 * \param[in]  dir_entry��ǩ���ڵ�Ŀ¼�� param3
 * \param[out]
 * \return     uint16
 * \retval     ��ǩ���
 * \ingroup    music_bookmark
 *******************************************************************************/
uint16 get_bookmark_index(uint32 cluster_no, uint32 dir_entry, uint8* name_str, uint32 bm_fp)
{
    music_bm_head *bm_head_ptr;
    bool is_write_bmk = FALSE;

    uint16 index;

    bmk_cache_sector = 0xffff;

    read_sector(0, bm_fp);

    bm_head_ptr = (music_bm_head *) bm_buf;

    //��ȡ��ǰ��¼������
    libc_memcpy(&bm_head, bm_head_ptr, sizeof(music_bm_head));

    if (bm_head_ptr->total_index != 0xffff)
    {
        //��Ҫ����������������
        index = find_index(cluster_no, dir_entry, bm_fp, 1);
    }
    else
    {
        //��¼��Ϊ0
        bm_head.total_index = 0;
        bm_head.del_total = 0;
        index = 0xffff;
        is_write_bmk = TRUE;
    }

    //δ�ҵ���Ӧ��������
    if (index == 0xffff)
    {
        if (bm_head.total_index >= MAX_MUSIC_FILE_NUMS)
        {
            //���õ�ǰ��ǩ������Ϊ��ɾ����ǩ�������
            index = bm_head.first_index;

            //��ǩ��������ɾ��������ӽ�����ǩ��
            music_del_bookmark(NULL, bm_head.first_index, bm_fp);

            bmk_cache_sector = 0xffff;

            //����һ��ɾ���������1
            bm_head.del_total--;
        }
        else
        {
            index = bm_head.total_index;
        }

        is_write_bmk = TRUE;

        //���㵱ǰ���������������ĵ�ַ
        write_index(cluster_no, dir_entry, index, bm_fp);
    }

    //δ�ҵ������ҵ��˵��ļ�����ƥ��ʱ��Ҫ���ļ���д��
    //������������������
    if (write_bmk_item(index, name_str, bm_fp) == TRUE)
    {
        is_write_bmk = TRUE;
    }

    if (is_write_bmk == TRUE)
    {
        //��Ҫ���»�дͷ��Ϣ
        //�л�����һ����������
        read_sector(0, bm_fp);
        //������ǩ�ļ�ͷ��Ϣ
        libc_memcpy(bm_head_ptr, &bm_head, sizeof(music_bm_head));
        //����ǩ�ļ�ͷ��Ϣ����д��
        write_sector(0, bm_fp);
    }
    return index;

}

/*! \endcond */
