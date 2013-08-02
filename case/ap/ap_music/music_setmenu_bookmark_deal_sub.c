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


/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 handle_bookmark(uint8 active, uint8 mode, uint16 cur_index)
 * \�����ǩ�����ɾ����ǩ�ɾ����ǩ��ͽ���Ӧ����д0xffffff:wq
 * \param[in]  active ��ǰ������ param1
 * \param[in]  mode ��� or ɾ�� param2
 * \param[in]  cur_index ��ǰ������ param3
 * \param[out]
 * \return     uint8
 * \retval     TRUE �ɹ�
 * \retval     FALSE ʧ��
 * \ingroup     music_bookmark
 * \note
 *******************************************************************************/
uint8 handle_bookmark(uint8 active, uint8 mode, uint16 cur_index)
{
    uint32 time;
    mengine_playinfo_t play_info;
    music_bm_item bm_item;
    //uint16 sec_no, offset;

    //�������ݽṹ
    mbmk_index_t* bmk_index_ptr;
    //�������ṹ
    mfile_bm_items_t* bm_data_ptr;

    file_location_t *plocation = (file_location_t *) &g_file_path_info.file_path;

    bmk_cache_sector = 0xffff;

    if (mode == MODE_ADD_BOOKMARK)
    {
        music_get_playinfo(&play_info);
        //����ʱ��ת��Ϊ��Ϊ��λ
        time = (play_info.cur_time / 1000);

        bm_item.break_time.hour = (uint8)(time / 3600);
        bm_item.break_time.minute = (uint8)((time % 3600) / 60);
        bm_item.break_time.second = (uint8)(time % 60);

        //��ȡ�ϵ���Ϣ
        music_get_bkinfo(&bm_item.break_param);
    }
    else if (mode == MODE_DEL_BOOKMARK) //ɾ����ǩ���ǽ�ĳ����Ϊ0xffffff
    {
        libc_memset(&bm_item.break_time, 0xff, sizeof(time_t));
    }
    else //select bookmark
    {
        ;//do nothing
    }

    g_bookmark_fp = open_bookmark();

    //��⵱ǰ���ŵ�location����ǩ��location�Ƿ�ƥ��
    //��λ����������ȡ��������
    bmk_index_ptr = read_bmk_index(cur_index, g_bookmark_fp);

    music_get_filepath(&g_file_path_info);

    if ((plocation->cluster_no != bmk_index_ptr->bmk_clus_no) || (plocation->dir_entry != bmk_index_ptr->bmk_dir_eny))
    {
        //������ǩ�����κβ���
        close_bookmark(g_bookmark_fp);
        return FALSE;
    }

    //��λ��������
    bm_data_ptr = read_bmk_item(cur_index, g_bookmark_fp);

    if (mode == MODE_SELECT_BOOKMARK)
    {
        //�ȹر���ǩ�ļ�����Ϊ�ϵ㲥�Ŷ���audible��Ҫ����pos�ļ�
        //�ļ�ϵͳֻ֧��һ���ļ�modify
        close_bookmark(g_bookmark_fp);
        
        libc_memcpy(&bm_item, &(bm_data_ptr->mfile_bmk_items[active]), sizeof(music_bm_item));

        if ((bm_item.break_time.hour == 0xff) && (bm_item.break_time.minute == 0xff) && (bm_item.break_time.second
                == 0xff))
        {
            return FALSE;
        }

        music_play_bookmark(&bm_item.break_param);
    }
    else
    {
        libc_memcpy(&(bm_data_ptr->mfile_bmk_items[active]), &bm_item, sizeof(music_bm_item));
        write_sector(cur_index / 2 + BM_HEADER_SECTOR_NUM, g_bookmark_fp);
        close_bookmark(g_bookmark_fp);
    }
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 get_bookmark_time(uint16 index, time_t *bm_item_buf)
 * \��ȡ��ǩ��ʱ���ǩ
 * \param[in]  index ��ǰ������ param1
 * \param[in]  bm_item_buf ������ǩʱ���buffer param2
 * \param[out]
 * \return     uint8
 * \retval     TRUE �ɹ�
 * \retval     FALSE ʧ��
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/
uint8 get_bookmark_time(uint16 index, time_t *bm_item_buf, uint32 bm_fp)
{
    uint8 i;

    mfile_bm_items_t* bm_data_ptr = (mfile_bm_items_t*) (bm_buf);

    bmk_cache_sector = 0xffff;

    bm_data_ptr = read_bmk_item(index, bm_fp);

    // ��ȡ��ǩ����
    for (i = 0; i < MAX_BOOKMARKS; i++)
    {
        libc_memcpy(&bm_item_buf[i], &(bm_data_ptr->mfile_bmk_items[i].break_time), sizeof(time_t));
    }
    return TRUE;
}

