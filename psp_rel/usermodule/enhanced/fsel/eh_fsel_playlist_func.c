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
 * \file     *.c
 * \brief    �����б���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_fsel.h"

extern uint8 fsel_temp_buf[SECTOR_SIZE];
//#define fsel_temp_buf  fsel_temp_buf

extern pdir_layer_t pdir_layer_buffer;
extern pfile_offset_t pfile_offset_buffer;

static bool set_into_layer(uint8 list_layer, uint16 layer_list_offset);

static bool get_items_info(plist_item_info_t *item_info, plist_type_e type);
static uint16 get_object_index(uint16 index_start_offset, uint16 index);
static plist_f_info_t * get_file_info(uint16 file_offset);

extern plist_tree_t* get_tree_byno(uint16 sector_start, uint16 tree_no);

extern bool fsel_read_sector_data(uint16 sector_offset) __FAR__;
extern bool plist_get_next(uint8 *strfile);

extern uint16 plist_get_fileNo_byIndex(uint16 index_start_offset, uint16 start_index, uint16 t_file_index) __FAR__;

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
static uint16 get_object_index(uint16 index_start_offset, uint16 index)
{
    uint16 sector_offset;
    uint16 buf_offset;

    sector_offset = index_start_offset + index / 256;
    buf_offset = (index * 2) % SECTOR_SIZE;

    fsel_read_sector_data(sector_offset);

    return *(uint16 *) (fsel_temp_buf + buf_offset);

}

/******************************************************************************/
/*
 * \par  Description: ��ȡ����ITEM��Ϣ
 *
 * \param[in]       type--ָ������

 * \param[out]      item_info--����

 * \return          TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool get_items_info(plist_item_info_t *item_info, plist_type_e type)
{
    plist_item_t *plist_item_p;
    fsel_read_sector_data(0);
    plist_item_p = (plist_item_t*) (fsel_temp_buf + sizeof(plist_head_t));
    plist_item_p += ((uint8) type & 0x0f);
    libc_memcpy(item_info, &plist_item_p->son_tree_num, sizeof(plist_item_info_t));
    return TRUE;

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
static plist_f_info_t * get_file_info(uint16 file_offset)
{
    uint16 buf_offset;
    uint16 sector_offset; //

    sector_offset = plist_info_offset + file_offset / (SECTOR_SIZE / PLIST_FILE_SIZE);
    buf_offset = (file_offset * PLIST_FILE_SIZE) % SECTOR_SIZE;

    fsel_read_sector_data(sector_offset);

    return (plist_f_info_t*) (fsel_temp_buf + buf_offset);
}
/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]    location--�ϵ���Ϣ�ṹ
 type--location����(no used)

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
bool plist_get_location(plist_location_t *location, fsel_type_e type)
{
    type = type;

    location->disk = fsel_init_val.disk;
    location->list_layer = dir_layer;
    location->file_total = file_total;
    location->file_num = file_no;

    location->file_index = plist_cur_file_index;
    location->list_type = plist_type_selector;

    //    libc_memcpy(location->list_layer_no,plist_list_layerno,sizeof(plist_list_layerno));
    location->layer_list_offset = plist_layer_offset;

    *(uint32*) &location->filename = cur_ext_name;

    vfs_file_dir_offset(vfs_mount, &location->dir_layer_info, &location->cluster_no, 0);

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]    location--�ϵ���Ϣ�ṹ
 type--location����(no used)

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool plist_set_location(plist_location_t *location, fsel_type_e type)
{
    plist_f_info_t *plist_f_p;
    bool ret_val = TRUE;

    type = type;

    if (0 == fsel_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    if (NULL == location)//�˴������ж�filename�Ƿ�Ϊ��,��ΪҪȡlist_type
    {
        ret_val = FALSE;
        plist_type_selector = PLIST_TITLE;
        //        plist_list_layerno[0]=0;
    }
    else
    {
        plist_type_selector = location->list_type;
    }

    get_items_info(&plist_item_info, (plist_type_selector & 0x0f));
    dir_layer = 0;
    //    plist_list_layerno[0]=0;
    plist_layer_offset = 0;
    plist_start_sort_index = 0;
    file_total = plist_file_total;
    file_no = 0;

    if ((FALSE == ret_val) || (0x00 == *(uint32*) &location->filename))
    {
        //Ĭ���ڵ�һ��
        //        fsel_close_object();
        //        return FALSE;
        ret_val = FALSE;
        goto exit_func;
    }

    if (fsel_init_val.fsel_type == FSEL_TYPE_M3ULIST)
    {
        if (location->list_layer == 1)
        {
            //m3u �ӱ�
            dir_layer = 1;
            plist_layer_offset = location->layer_list_offset; //set loaction �ɹ��󣬸�ֵ�᲻���ı�
            m3u_curlist_index = plist_layer_offset & 0x7f;
            //���»�ȡ��Ϣ
            fsel_close_object();
            fsel_open_object(R_NORMAL_SEEK);
            ret_val = TRUE;
        }
        else
        {
            ret_val = FALSE;
        }
    }
    else
    {
        ret_val = set_into_layer(location->list_layer, location->layer_list_offset);
    }

    if (FALSE != ret_val)
    {
        //����ʱ����ֻ��Ҫ��ʼλ�ã�plist_start_sort_index���͸�����file_total���Ϳ���
        file_total = location->file_total;
        plist_f_p = get_file_info(location->file_index);
        if ((plist_f_p->flag == 2) || (0 != libc_memcmp(&location->cluster_no, &plist_f_p->cluster_no, 8)))
        {
            ret_val = FALSE;
        }
        else
        {
            ret_val = vfs_file_dir_offset(vfs_mount, &plist_f_p->dir_layer_info, &plist_f_p->cluster_no, 1);
        }
    }

    if (ret_val == FALSE)
    {
        file_no = 0;
    }
    else
    {
        //��ֵ����cur_ext_name
        libc_memcpy(&cur_ext_name, location->filename, 4);
        plist_cur_file_index = location->file_index;

        if (m3u_curlist_index != 0xffff)
        {
            //m3u ���� ˢ��file_no,file_total

            /**********************************
             M3U���̣�ǰ̨��������location�е�file_num��file_total�ǻ�������listͳ�Ƶ�
             �ں�̨�ٷֿ�MUSIC �� audible ���ű��ţ�����location��ǰ̨��ʾ
             ***********************************/

            file_total = plist_file_total;
            file_no = plist_get_fileNo_byIndex(plist_item_info.file_index_offset, 0, plist_cur_file_index);
            location->file_total = file_total;
            location->file_num = file_no;
        }
        else
        {
            file_no = location->file_num;
        }
    }

    exit_func: fsel_close_object();

    //�����ϵ㲥�ţ����³�ʼ�������
    if (TRUE == shuffle_sw)
    {
        init_shuffle(file_total);
        filtrate_shuffle_no = file_no;
    }

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static plist_tree_t *read_tree_data(uint16 tree_start_offset, uint16 tree_offset)
{
    uint16 buf_offset;
    uint16 sector_offset;

    if (tree_offset != 0)
    {
        //        sector_offset=tree_start_offset+(tree_offset-1)/256;
        sector_offset = tree_start_offset + (tree_offset * sizeof(plist_tree_t)) / SECTOR_SIZE;
    }
    else
    {
        sector_offset = tree_start_offset;
    }

    fsel_read_sector_data(sector_offset);

    //    buf_offset=(tree_offset*2)/SECTOR_SIZE;
    buf_offset = (tree_offset * sizeof(plist_tree_t)) % SECTOR_SIZE;
    return (plist_tree_t*) (fsel_temp_buf + buf_offset);
}

/******************************************************************************/
/*
 * \par  Description:�����б�����
 *
 * \param[in]    list_layer--���
 --���ڲ��λ��ƫ�ƣ�0~N��

 * \param[out]

 * \return       TRUE OR FALSE

 * \note        �����ֵdir_layer��plist_layer_offset��plist_start_sort_index
 *******************************************************************************/
//·�����ò��ɹ���Ĭ��Ϊ���и�������
static bool set_into_layer(uint8 list_layer, uint16 layer_list_offset)
{
    //    uint8 i=0;
    plist_tree_t *plist_tree_p;

    if (list_layer > 0)
    {

        plist_tree_p = read_tree_data(plist_item_info.son_tree_offset[list_layer - 1], layer_list_offset);
        if (plist_tree_p->flag == 1)
        {
            plist_start_sort_index = get_object_index(plist_item_info.sort_index_offset, plist_tree_p->file_index);
        }
        else
        {
            return FALSE;
        }

    }

    dir_layer = list_layer;
    plist_layer_offset = layer_list_offset;
    //    libc_memcpy(plist_list_layerno,list_layer_no,sizeof(plist_list_layerno));

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
bool plist_del_one(uint16 del_num)
{
    if (0 == fsel_open_object(OPEN_MODIFY))
    {
        return FALSE;
    }
    //add del code

    fsel_close_object();
    return FALSE;
}

