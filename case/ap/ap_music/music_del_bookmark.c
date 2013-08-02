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

/******************************************************************************/
/*!
 * \par  Description:
 *  void del_bookmark_by_index(uint16 index, uint32 bm_fp)
 *	  ������ǩ����ǩ�ļ���������ɾ��֮
 * \param[in]  index ��ǩ���
 * \param[out] bm_fp ��ǩ�ļ����
 * \return     none
 * \ingroup    music_bookmark
 * \note
 * \    �ýӿ��������ط�����õ������ɾ��music�ļ�ʱ����˳��ɾ����ǩ�ļ�������location
        �õ���ǩ��ţ�������ǩ��ʱ���������紴������ǩ�ļ���Ҳ��ɾ��
 *******************************************************************************/
void del_bookmark_by_index(uint16 index, uint32 bm_fp)
{
    //�������ݽṹ
    mbmk_index_t* bmk_index_item;
    mfile_bm_items_t* bm_data_ptr;

    //�������һ������������
    //mbmk_index_t last_index_item;
    //�����ɾ�������������
    mbmk_index_t del_index_item;

    //uint16 last_index = bm_head.last_index;

    //��λ����ǰɾ���ļ���������
    bmk_index_item = read_bmk_index(index, bm_fp);
    //�����ɾ�������������
    libc_memcpy(&del_index_item, bmk_index_item, sizeof(mbmk_index_t));
    //���ø���������Ч
    libc_memset(bmk_index_item, 0xff, sizeof(mbmk_index_t));

    if (del_index_item.prev != 0xffff)
    {
        //�޸�ɾ����ǰ���ĺ��
        bmk_index_item = read_bmk_index(del_index_item.prev, bm_fp);
        bmk_index_item->next = del_index_item.next;
        write_bmk_index(del_index_item.prev, bm_fp);
    }
    else
    {
        //�޸�ͷָ��
        bm_head.first_index = del_index_item.next;
    }

    if (del_index_item.next != 0xffff)
    {
        //�޸�ɾ�����̵�ǰ��
        bmk_index_item = read_bmk_index(del_index_item.next, bm_fp);
        bmk_index_item->prev = del_index_item.prev;
        write_bmk_index(del_index_item.next, bm_fp);
    }
    else
    {
        //�޸�βָ��
        bm_head.last_index = del_index_item.prev;
    }

    //�������������
    bm_data_ptr = read_bmk_item(index, bm_fp);
    libc_memset(bm_data_ptr, 0xff, sizeof(mfile_bm_items_t));
    write_sector(index / 2 + BM_HEADER_SECTOR_NUM, bm_fp);

    //����ͷ��Ϣ
    bm_head.total_index--;
    bm_head.del_total++;
    return;

}
/******************************************************************************/
/*!
 * \par  Description:
 *  void del_bookmark_file(file_location_t *plocation)
 *  ɾ���ļ�ʱ,ɾ����Ӧ����ǩ�ļ���
 * \param[in]
 * \param[out]
 * \return     void
 * \retval
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/
void music_del_bookmark(file_location_t *plocation, uint16 cur_index, uint32 bm_fp)
{
    //��ǩ�ļ�ͷ��Ϣ
    music_bm_head *bm_head_ptr;
    uint16 index = 0xffff;

    bmk_cache_sector = 0xffff;

    //��ȡ��һ��������ֵ
    read_sector(0, bm_fp);

    bm_head_ptr = (music_bm_head *) bm_buf;

    libc_memcpy(&bm_head, bm_head_ptr, sizeof(music_bm_head));

    //������ǩ��¼��
    if (bm_head.total_index != 0xffff)
    {
        if (plocation != NULL)
        {
            //��Ҫ����������������
            //��һ��������ǰ16�ֽ�����ǩ�ļ�ͷ
            index = find_index(plocation->cluster_no, plocation->dir_entry, g_bookmark_fp, 0);
        }
        else
        {
            index = cur_index;
        }
        //�ҵ�������
        if (index != 0xffff)
        {
            del_bookmark_by_index(index, bm_fp);
            read_sector(0, bm_fp);
            libc_memcpy(bm_head_ptr, &bm_head, sizeof(music_bm_head));
            write_sector(0, bm_fp);
        }
    }
}

/*! \endcond */

