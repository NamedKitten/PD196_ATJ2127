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
 * \file     eh_bs_playlist.c
 * \brief    �����б����
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �ļ�ϵͳ
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

//#include "eh_bs.h"
#include "../eh_bs.h"

extern plist_tree_t plist_cur_tree;
extern uint8 bs_temp_buf[SECTOR_SIZE];

//extern uint16 save_read_offset; //����������ַ
//extern uint8   write_data_flag;   //��д���ݱ��


//bool bs_get_item_info(plist_item_info_t *item_info,plist_type_e type);

//static void bs_update_last_data(uint16 w_flag);

static bool bs_get_title_info(file_brow_t*name_struct, uint16 t_file_index);

uint16 bs_get_object_index(uint16 index_start_offset, uint16 index);
plist_f_info_t * bs_get_file_info(uint16 file_offset);
plist_tree_t *bs_read_tree_data(uint16 tree_start_offset, uint16 tree_offset);
plist_tree_t* bs_get_tree_info_byno(uint16 sector_start, uint16 tree_offset, uint16 dir_num, uint8 change_offset_flag);
bool bs_get_display_info(uint8*name_buf, uint8 name_len, uint8 type, uint16 t_file_index);
uint16 bs_get_file_index_byno(uint16 sector_start, uint16 index_offset, uint16 t_file_no);

extern plist_tree_t *bs_read_tree_data(uint16 tree_start_offset,uint16 tree_offset) __FAR__;
extern bool bs_read_sector_data(uint16 sector_offset) __FAR__;

/******************************************************************************/
/*
 * \par  Description: ������ʾ����Ż�ȡ�ļ���file_index
 *
 * \param[in]    sector_start---file_index �б������ƫ�� 0~N
 index_offset---��ʼfile_index���б��е�ƫ�� 0~N
 t_file_no---��ʾ����� 1~N

 * \param[out]

 * \return       file_index

 * \note
 *******************************************************************************/
uint16 bs_get_file_index_byno(uint16 sector_start, uint16 index_offset, uint16 t_file_no)
{
    uint16 sector_offset;
    uint16 buf_offset;
    uint16 *buf_pt;

    sector_offset = sector_start + index_offset / (SECTOR_SIZE / 2);
    buf_offset = (index_offset * 2) % SECTOR_SIZE;

    buf_pt = (uint16 *) (bs_temp_buf + buf_offset);
    //    vfs_file_seek(vfs_mount,sector_offset*SECTOR_SIZE,SEEK_SET,obj_handle);
    //    vfs_file_read(vfs_mount,bs_temp_buf,SECTOR_SIZE, obj_handle);
    bs_read_sector_data(sector_offset);

    //    while(t_file_no-->0)
    for (; t_file_no > 0; t_file_no--)
    {

        while (1)
        {
            if (buf_pt >= (uint16*) (bs_temp_buf + SECTOR_SIZE))
            {
                sector_offset++;
                buf_pt = (uint16*) bs_temp_buf;
                //vfs_file_read(vfs_mount,bs_temp_buf,SECTOR_SIZE, obj_handle);
                bs_read_sector_data(sector_offset);
            }
            if (*buf_pt != PLIST_INVALID_INDEX)
            {
                break;
            }
            buf_pt++;

        }

        buf_pt++;

    }

    return *(buf_pt - 1) & 0x1fff;
}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]    file_offset �ļ�����š�0~N

 * \param[out]

 * \return       �ļ���Ϣ�ṹ

 * \note
 *******************************************************************************/
plist_f_info_t * bs_get_file_info(uint16 file_offset)
{
    uint16 buf_offset;
    uint16 sector_offset; //��һ������Ϊͷ

    sector_offset = plist_info_offset + file_offset / (SECTOR_SIZE / 256);
    buf_offset = (file_offset * 256) % SECTOR_SIZE;
    bs_read_sector_data(sector_offset);

    return (plist_f_info_t*) (bs_temp_buf + buf_offset);
}

/******************************************************************************/
/*
 * \par  Description: ������ʾ���б���б�ṹ���ȡ��Ϣ
 *
 * \param[in]    name_buf--��Ϣbuf
 name_len--��Ϣ����
 type--��ʾ����Ϣ�б��� PLIST_TITLE,PLIST_ALBUM��
 t_file_index--�ļ����

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_get_display_info(uint8*name_buf, uint8 name_len, uint8 type, uint16 t_file_index)
{
    plist_f_info_t *plist_f_p;
    uint16 str_len;
    uint8 *string;

    plist_f_p = bs_get_file_info(t_file_index);
    str_len = ID3_ITEM_SIZE;

    switch (type)
    {
        //        case PLIST_TITLE:
        //        case PLIST_BOOK:
        case PLIST_ARTIST:
        case PLIST_AUTHOR:
        {
            string = &plist_f_p->artist_string[0];
            break;
        }
        case PLIST_ALBUM:
        {
            string = &plist_f_p->album_string[0];
            break;
        }
        case PLIST_GENRE:
        {
            string = &plist_f_p->gener_string[0];
            break;
        }
        default:
        {
            string = &plist_f_p->title_string[0];
            str_len = ID3_ITEM_SIZE * 2;
            break;
        }

    }

    if (str_len > (name_len - 2))
    {
        str_len = name_len - 2;
    }

    libc_memcpy(name_buf, string, (uint32) str_len);
    name_buf[str_len] = 0x00;
    name_buf[str_len + 1] = 0x00;

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: �����ļ�����Ż�ȡ�ļ���title
 *
 * \param[in]    name_struct--�����Ϣbuf�ṹ
 t_file_index--�ļ����

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool bs_get_title_info(file_brow_t*name_struct, uint16 t_file_index)
{
    plist_f_info_t *plist_f_p;
    uint16 str_len;

    plist_f_p = bs_get_file_info(t_file_index);

    str_len = ID3_ITEM_SIZE * 2;

    if ((name_struct->name_len - 2) < str_len)
    {
        str_len = name_struct->name_len - 2;
        name_struct->name_buf[str_len] = 0x00;
        name_struct->name_buf[str_len + 1] = 0x00;
    }

    libc_memcpy(name_struct->name_buf, plist_f_p->title_string, (uint32) str_len);
    libc_memcpy(name_struct->ext, plist_f_p->filename, 4);

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]   tree_start_offset--tree����ʼ����ƫ�� 0~N
 tree_offset --tree ƫ�ƣ�0~N

 * \param[out]

 * \return        tree��Ϣ�ṹָ��

 * \note
 *******************************************************************************/
plist_tree_t *bs_read_tree_data(uint16 tree_start_offset, uint16 tree_offset)
{
    uint16 buf_offset;
    uint16 sector_offset;

    if (tree_offset != 0)
    {
        sector_offset = tree_start_offset + tree_offset / (SECTOR_SIZE / sizeof(plist_tree_t));
    }
    else
    {
        sector_offset = tree_start_offset;
    }

    buf_offset = sizeof(plist_tree_t) * (tree_offset % (SECTOR_SIZE / sizeof(plist_tree_t)));

    bs_read_sector_data(sector_offset);

    return (plist_tree_t *) (bs_temp_buf + buf_offset);
}

/******************************************************************************/
/*
 * \par  Description: ��ȡindex��
 *
 * \param[in]    index_start_offset--index�б������ƫ��
 index--��� 0~N

 * \param[out]

 * \return       object_index 0~N

 * \note
 *******************************************************************************/
uint16 bs_get_object_index(uint16 index_start_offset, uint16 index)
{
    uint16 sector_offset;
    uint16 buf_offset;

    sector_offset = index_start_offset + index / 256;
    buf_offset = (index * 2) % SECTOR_SIZE;

    bs_read_sector_data(sector_offset);

    return *(uint16 *) (bs_temp_buf + buf_offset);

}

/******************************************************************************/
/*
 * \par  Description: �������һ������
 *
 * \param[in]    w_flag--д���־

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
/*
 static void bs_update_last_data(uint16 w_flag)
 {
 if(w_flag==1 ||save_read_offset!=0xffff)
 {
 vfs_file_seek(vfs_mount,save_read_offset*SECTOR_SIZE,SEEK_SET,obj_handle);
 vfs_file_write(vfs_mount,bs_temp_buf,SECTOR_SIZE,obj_handle);
 save_read_offset=0xffff;
 }
 return;
 }
 */

/******************************************************************************/
/*
 * \par  Description: browser��ȡ��ʾ�б���Ϣ
 *
 * \param[in]    decode_param--��ʾ����
 file_list--��ʾbuf

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
bool bs_plist_get_items(brow_decode_t *decode_param, void *file_list)
{
    file_brow_t *file_name;
    uint8 count;
    uint8 out_flag;//��Ϣbuf����0��־
    uint16 i;
    //    uint8 out_flag=0;
    plist_tree_t *plist_tree_p;
    uint16 index_offset;
    uint16 t_file_index;

    save_read_offset = 0xffff;
    out_flag = 0;

    if (0 == bs_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    file_name = (file_brow_t *) file_list;
    count = decode_param->num;
    i = decode_param->top; //����ֵΪ1

    if (i > (bs_dir_total + bs_file_total))
    {
        bs_close_object();
        return FALSE;
    }

    if (bs_dir_total != 0)
    {
        //        while(count--)
        for (; count > 0; count--)
        {
            if (out_flag == 1)
            {
                file_name->name_buf[0] = 0x00;
                //                file_name->name_buf[1] = 0x00;
                *(uint32*) &file_name->ext = 0x00;
            }
            else
            {
                plist_tree_p = bs_get_tree_info_byno(plist_item_info.son_tree_offset[dir_layer],
                        plist_cur_tree.son_offset, i, 0);
                if (NULL != plist_tree_p)
                {
                    bs_get_display_info(file_name->name_buf, file_name->name_len, plist_tree_p->type,
                            plist_tree_p->file_index);
                    *(uint32*) &file_name->ext = EH_DIR_FLAG;
                    if ((i + 1) > bs_dir_total)
                    {
                        out_flag = 1;
                    }
                }
            }
            i++;
            file_name++;
        }
    }
    else if (bs_file_total != 0)
    {
        if (m3u_curlist_index != 0xffff)
        {
            index_offset = 0;
        }
        else if (bs_file_total == plist_file_total)
        {
            index_offset = 0;//
        }
        else
        {
            index_offset = bs_get_object_index(plist_item_info.sort_index_offset, plist_cur_tree.file_index);
        }
        //        while(count--)
        for (; count > 0; count--)
        {
            if (out_flag == 1)
            {
                file_name->name_buf[0] = 0x00;
                //                file_name->name_buf[1] = 0x00;
                *(uint32*) &file_name->ext = 0x00;
            }
            else
            {
                t_file_index = bs_get_file_index_byno(plist_item_info.file_index_offset, index_offset, i);
                bs_get_title_info(file_name, t_file_index);
                if ((i + 1) > bs_file_total)
                {
                    out_flag = 1;
                }
            }
            i++;
            file_name++;
        }
    }
    else
    {
        bs_close_object();
        return FALSE;//���ļ����ļ���
    }

    bs_close_object();
    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]    sector_start--tree 's sector offset,
 tree_offset-- 0~N
 dir_num-- 1~N
 change_offset_flag--�ı�plist_layer_offset��ֵ���

 * \param[out]   tree's data

 * \return

 * \note
 *******************************************************************************/
plist_tree_t* bs_get_tree_info_byno(uint16 sector_start, uint16 tree_offset, uint16 dir_num, uint8 change_offset_flag)
{
    //    uint16 sector_offset;
    //    uint16 buf_offset;
    plist_tree_t *plist_tree_p;

    plist_tree_p = 0;
    //    while(dir_num--)
    for (; dir_num > 0; dir_num--)
    {
        while (1)
        {
            plist_tree_p = bs_read_tree_data(sector_start, tree_offset);
            if (plist_tree_p->flag == 1)
            {
                break;
            }
            tree_offset++;
        }
        tree_offset++;
    }

    if (change_offset_flag == 1)
    {
        plist_layer_offset = tree_offset - 1;//��¼������tree��ƫ��
    }
    return plist_tree_p;

}

/******************************************************************************/
/*
 * \par  Description:��ȡ����Ķϵ�
 *
 * \param[in]    location--�ļ�location
 type--location�����ͣ�no used��

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_plist_get_location(plist_location_t *location, fsel_type_e type)
{
    uint16 t_sort_index;
    plist_f_info_t *plist_file_p;
    plist_tree_t *plist_tree_p;
    uint16 t_file_index = 0xffff;
    uint16 temp_layer_offset;
    uint8 temp = 0;

    type = type;
    save_read_offset = 0xffff;
    temp_layer_offset = plist_layer_offset;

    if ((bs_select_listno == 0) && (dir_layer == 0))
    {
        location->list_type = plist_type_selector;
        libc_memcpy(location->filename, "ROOT", 4);
        return TRUE;
    }

    if (fsel_init_val.fsel_type == FSEL_TYPE_M3ULIST)
    {
        if (dir_layer == 0)
        {
            //ѡ��M3U�б�
            if (bs_select_listno > plist_file_total)
            {
                return FALSE;
            }
            else
            {
                if (0 != bs_open_object(R_NORMAL_SEEK))
                {
                    //��ȡ�ӱ����
                    m3u_curlist_index = bs_get_file_index_byno(plist_item_info.file_index_offset, 0, bs_select_listno);
                    bs_close_object();
                }

                if (m3u_curlist_index != 0xffff)
                {
                    bs_select_listno = 1;
                    dir_layer = 1;
                }
            }

        }

        if ((m3u_curlist_index == 0xffff) || (dir_layer != 1))
        {
            return FALSE;
        }

        //������֧����
        plist_cur_tree.flag = 0;
        temp_layer_offset = m3u_curlist_index;
        plist_type_selector = PLIST_M3U;
        dir_layer = 1;
    }

    if (0 == bs_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    if (bs_select_listno == 0xffff)
    {
        //ѡ������ĵ�ǰ���и���
        bs_select_listno = 1;
        temp = 1;
    }

    if (0x01 == plist_cur_tree.flag)
    {
        //��tree�ṹ

        if (1 == temp)
        {
            //select current list all file
            t_file_index = plist_cur_tree.file_index;
            location->file_num = 1;
            location->file_total = plist_cur_tree.file_total;

        }
        else if (0 != bs_dir_total)
        {
            //ѡ���б���,ȡ�б��µĵ�һ���ļ�location,����plist_layer_offset��ֵ

            //�ӿ��ڻ����plist_layer_offsetֵ
            dir_layer++;
            plist_tree_p = bs_get_tree_info_byno(plist_item_info.son_tree_offset[dir_layer - 1],
                    plist_cur_tree.son_offset, bs_select_listno, 1);

            if (NULL != plist_tree_p)
            {
                location->file_num = 1;
                location->file_total = plist_tree_p->file_total;
                t_file_index = plist_tree_p->file_index;
            }
        }
        else
        {
            //            //��0���ȡ����

            //            if(0!=bs_dir_total)
            //    		{
            //    			//ѡ���б���,ȡ�б��µĵ�һ���ļ�location,����plist_layer_offset��ֵ
            //
            //              //ȷ��ѡ���б��ƫ��ֵ
            //              //�ӿ��ڻ����plist_layer_offsetֵ
            //
            //                dir_layer++;
            //
            //                plist_tree_p=bs_get_tree_info_byno(plist_item_info.son_tree_offset[dir_layer-1],plist_cur_tree.son_offset,bs_select_listno,1);
            //
            //                location->file_num=1;
            //                if(0==temp)
            //                {
            //                    location->file_total=plist_tree_p->file_total;
            //                }
            //                else
            //                {
            //                    //select all
            //                    location->file_total=plist_file_total;
            //                }
            //                t_file_index=plist_tree_p->file_index;
            //    		}
            //            else
            //            {
            //ѡ���ļ�����
            location->file_num = bs_select_listno;
            location->file_total = bs_file_total;

            if (bs_file_total == plist_file_total)//ALL?
            {
                t_sort_index = 0;//START OFFSET
            }
            else
            {
                t_sort_index = bs_get_object_index(plist_item_info.sort_index_offset, plist_cur_tree.file_index);
            }
            //ȥѡ���ļ���file_index
            t_file_index = bs_get_file_index_byno(plist_item_info.file_index_offset, t_sort_index, bs_select_listno);
            //            }
        }

    }
    else
    {
        //��Ŀ¼����dir_layer=0
        //��û����tree�ṹ
        if ((bs_file_total == plist_file_total) || (m3u_curlist_index != 0xffff))
        {
            //all song ��m3u�б�
            if (plist_file_total == 0)
            {
                //m3u�б�����,û���ļ���ѡ,����ʧ��;�˻��ϼ�
                t_file_index = 0xffff;
                m3u_curlist_index = 0xffff;
                dir_layer = 0;
            }
            else
            {
                t_file_index = bs_get_file_index_byno(plist_item_info.file_index_offset, 0, bs_select_listno);
                location->file_num = bs_select_listno;
                location->file_total = bs_file_total;
            }
        }
        else
        {
            //ѡ���Ŀ¼�µ��ӱ�
            location->file_num = 1;
            dir_layer = 1; //dir_layer++
            plist_tree_p = bs_get_tree_info_byno(plist_item_info.son_tree_offset[0], 0, bs_select_listno, 1);
            if (plist_tree_p != NULL)
            {
                t_file_index = plist_tree_p->file_index;

                if (1 == temp)
                {
                    //select all
                    location->file_total = plist_file_total;
                }
                else
                {
                    location->file_total = plist_tree_p->file_total;
                }
            }
        }

    }

    location->list_type = plist_type_selector;
    location->disk = fsel_init_val.disk;
    location->list_layer = dir_layer;
    //    libc_memcpy(location->list_layer_no,list_layer_no,4);
    location->layer_list_offset = plist_layer_offset;

    plist_layer_offset = temp_layer_offset;

    if (t_file_index != 0xffff)
    {
        location->file_index = t_file_index;
        plist_file_p = bs_get_file_info(t_file_index);

        if (m3u_curlist_index != 0xffff)
        {
            m3ulist_f_info_t *f_tp = (m3ulist_f_info_t*) plist_file_p;
            location->layer_list_offset = location->layer_list_offset | (f_tp->m3u_index[0] & 0x80);//ȡAUDIBLE���
        }

        libc_memcpy(&location->cluster_no, &plist_file_p->cluster_no, 8);
        libc_memcpy(&location->dir_layer_info, &plist_file_p->dir_layer_info, sizeof(pdir_layer_t));
        libc_memcpy(location->filename, plist_file_p->filename, 4);

        bs_close_object();
        return TRUE;
    }
    else
    {

        bs_close_object();
        return FALSE;
    }

}

