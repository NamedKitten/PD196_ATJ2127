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
 * \file     eh_bs_playlist_func.c
 * \brief    �����б����
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �ļ�ϵͳ��VRAM
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

//#include "eh_bs.h"
#include "../eh_bs.h"

extern plist_tree_t plist_cur_tree;
//extern uint16 save_read_offset; //����������ַ
//extern uint8   write_data_flag;   //��д���ݱ��
extern uint8 bs_temp_buf[SECTOR_SIZE];

static bool bs_get_total(void);
static bool bs_plist_enter_son(uint16 dir_num);
static bool bs_plist_enter_parent(void);
//static plist_tree_t* bs_get_tree_info(uint16 sector_start,uint16 tree_offset);
static bool bs_set_into_layer(uint8 list_layer, uint16 list_layer_offset);

extern plist_tree_t* bs_get_tree_info_byno(uint16 sector_start, uint16 tree_offset, uint16 dir_num,
        uint8 change_offset_flag);
extern bool bs_get_display_info(uint8*name_buf, uint8 name_len, uint8 type, uint16 t_file_index);
extern plist_tree_t *bs_read_tree_data(uint16 tree_start_offset, uint16 tree_offset);

//extern uint16 bs_get_object_index(uint16 index_start_offset,uint16 index);
extern uint16 bs_get_file_index_byno(uint16 sector_start, uint16 index_offset, uint16 t_file_no);
//extern plist_f_info_t * bs_get_file_info(uint16 file_offset);
//extern bool bs_get_item_info(plist_item_info_t *item_info,plist_type_e type);

extern bool bs_uspl_delete_func(brow_del_e type,uint16 list_no ,uint8 *file_dir_info) __FAR__;
extern bool bs_plist_delete_func(brow_del_e type,uint16 list_no ,uint8 *file_dir_info) __FAR__;
extern bool bs_m3ufile_delete_func(brow_del_e type, uint16 list_no, uint8 *file_dir_info) __FAR__;

extern bool bs_read_sector_data(uint16 sector_offset) __FAR__;
extern bool bs_del_ebookbmk(void) __FAR__;

void bs_m3u_get_total(uint16 list_no);

/******************************************************************************/
/*
 * \par  Description: ��ȡָ�����͵�ITEM��Ϣ
 *
 * \param[in]     item_info --���ITEM��Ϣ
 type--����

 * \param[out]

 * \return        NULL

 * \note
 *******************************************************************************/
bool bs_get_item_info(plist_item_info_t *item_info, plist_type_e type)
{
    plist_item_t *plist_item_p;

    bs_read_sector_data(0);
    plist_item_p = (plist_item_t *) (bs_temp_buf + sizeof(plist_head_t));
    plist_item_p += ((uint8) type & 0x0f);
    libc_memcpy(item_info, &plist_item_p->son_tree_num, sizeof(plist_item_info_t));
    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description: �����ӱ�
 *
 * \param[in]    ѡ���list number 1��N

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool bs_plist_enter_son(uint16 dir_num)
{
    plist_tree_t *plist_tree_p;

    //M3U  ���⴦��
    if (fsel_init_val.fsel_type != FSEL_TYPE_M3ULIST)
    {
        if ((dir_num == 0) || (bs_dir_total == 0))
        {
            //ֻ���ļ���û���б�
            return FALSE;
        }
    }

    if (dir_num > (bs_dir_total + bs_file_total))
    {
        return FALSE;
    }

    dir_layer++;
    //M3U  ���⴦��
    if (fsel_init_val.fsel_type == FSEL_TYPE_M3ULIST)
    {
        if (dir_layer == 1)
        {
            m3u_curlist_index = bs_get_file_index_byno(plist_item_info.file_index_offset, 0, dir_num);
            if (m3u_curlist_index != 0xffff)
            {
                //m3u �ӱ����´��ļ���ˢ������
                bs_close_object();
                bs_open_object(R_NORMAL_SEEK);
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        plist_tree_p = bs_get_tree_info_byno(plist_item_info.son_tree_offset[dir_layer - 1], plist_cur_tree.son_offset,
                dir_num, 1);
        libc_memcpy(&plist_cur_tree, plist_tree_p, sizeof(plist_tree_t));
        bs_get_total();
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: �˵�����
 *
 * \param[in]    NULL

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool bs_plist_enter_parent(void)
{
    plist_tree_t *plist_tree_p;

    if (dir_layer == 0)
    {
        return FALSE;
    }

    dir_layer--;
    if (dir_layer == 0)
    {
        plist_layer_offset = 0;
        libc_memset(&plist_cur_tree, 0, sizeof(plist_tree_t));
        if (m3u_curlist_index != 0xffff)
        {
            m3u_curlist_index = 0xffff;
            //���´��ļ�����ȡM3U�б��ļ�����
            bs_close_object();
            bs_open_object(R_NORMAL_SEEK);
        }
        bs_get_total();
        return TRUE;
    }

    if (plist_cur_tree.parent_offset != 0xffff)
    {
        plist_layer_offset = plist_cur_tree.parent_offset;
        plist_tree_p = bs_read_tree_data(plist_item_info.son_tree_offset[dir_layer - 1], plist_cur_tree.parent_offset);
        libc_memcpy(&plist_cur_tree, plist_tree_p, sizeof(plist_tree_t));
        bs_get_total();
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

/******************************************************************************/
/*
 * \par  Description:�����б�
 *
 * \param[in]    type--�������ͣ����� �Ӽ� or ���� or ��ǰ����
 list_no--���� list number 1��N

 * \param[out]   dir_brow   --�ļ��в���

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_plist_enter(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow)
{
    bool ret_val = FALSE;

    save_read_offset = 0xffff;

    if (type == ROOT_DIR)
    {
        //����M3U�б���ʾ�����
        m3u_curlist_index = 0xffff;//��OPENǰ��ֵ
        dir_layer = 0;
    }

    if (0 == bs_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    switch (type)
    {
        //���½�����ǰĿ¼
        case CUR_DIR:
        ret_val = bs_get_total();
        break;

        //������Ŀ¼
        case SON_DIR:
        ret_val = bs_plist_enter_son(list_no);
        break;

        //���ظ�Ŀ¼
        case PARENT_DIR:
        ret_val = bs_plist_enter_parent();
        m3u_curlist_index = 0xffff;
        break;

        //�����Ŀ¼
        case ROOT_DIR:

        //        m3u_curlist_index = 0xffff;
        //        if(fsel_init_val.fsel_type == FSEL_TYPE_M3ULIST)
        //        {
        //            //���´��ļ�,�����б�������
        //            bs_close_object();
        //            bs_open_object(R_NORMAL_SEEK);
        //        }
        bs_get_item_info(&plist_item_info, plist_type_selector);
        dir_layer = 0;
        bs_select_listno = 0;
        //    plist_list_layerno[0]=0;
        plist_layer_offset = 0;
        plist_start_sort_index = 0;
        libc_memset(&plist_cur_tree, 0x00, sizeof(plist_tree_t));

        bs_dir_total = plist_item_info.son_tree_num[0];

        if (0 == bs_dir_total)
        {
            bs_file_total = plist_file_total;//title
        }
        else
        {
            bs_file_total = 0;
        }
        ret_val = TRUE;
        //��Ӵ���
        break;

        default:
        ret_val = FALSE;
        break;

    }

    if (dir_brow != NULL)
    {

        dir_brow->dir_total = bs_dir_total;
        dir_brow->file_total = bs_file_total;
        dir_brow->layer_no = dir_layer;

        if ((FALSE != ret_val) && (dir_brow->name_len > 0))
        {
            if ((0x00 == dir_layer) || (m3u_curlist_index != 0xffff))
            {
                dir_brow->name_buf[0] = 0x00;
            }
            else
            {
                bs_get_display_info(dir_brow->name_buf, dir_brow->name_len, plist_cur_tree.type,
                        plist_cur_tree.file_index);
            }
        }

        //M3U  ���⴦��
        //,dir_layer =0,ʱbs_file_total ��Ϊ bs_dir_total ֵ����
        if (fsel_init_val.fsel_type == FSEL_TYPE_M3ULIST)
        {
            if (0 == dir_layer)
            {
                dir_brow->dir_total = dir_brow ->file_total;
                dir_brow ->file_total = 0;
            }
        }
    }

    bs_select_listno = 0;

    bs_close_object();

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description:��ȡ����ļ�������
 *
 * \param[in]    NULL

 * \param[out]

 * \return      TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool bs_get_total(void)
{
    //M3U  ���⴦��
    if (fsel_init_val.fsel_type == FSEL_TYPE_M3ULIST)
    {
        if (dir_layer == 1)
        {
            return TRUE;
        }
    }

    if (0 == dir_layer)
    {
        bs_dir_total = plist_item_info.son_tree_num[0];
    }
    else
    {
        bs_dir_total = plist_cur_tree.son_num;
    }

    if (bs_dir_total == 0)
    {
        if (plist_cur_tree.flag == 1)
        {
            bs_file_total = plist_cur_tree.file_total;
        }
        else
        {
            bs_file_total = plist_file_total;
        }
    }
    else
    {
        bs_file_total = 0;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:��ȡ �����TREE
 *
 * \param[in]    sector_start--TREE����ʼ����ƫ��0��N
 tree_offset--TREE ��ƫ��λ�� 0��N

 * \param[out]

 * \return      plist_tree_t*

 * \note
 *******************************************************************************/
/*
 static plist_tree_t* bs_get_tree_info(uint16 sector_start,uint16 tree_offset)
 {
 return bs_read_tree_data(sector_start,tree_offset);
 }
 */

/******************************************************************************/
/*
 * \par  Description:���öϵ����Ϣ
 *
 * \param[in]    list_layer--����
 list_layer_offset--���һ���TREEƫ��

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//·�����ò��ɹ���Ĭ��Ϊ���и�������
static bool bs_set_into_layer(uint8 list_layer, uint16 list_layer_offset)
{
    //    uint8 i=0;
    plist_tree_t *plist_tree_p;

    if (list_layer > 0)
    {
        plist_tree_p = bs_read_tree_data(plist_item_info.son_tree_offset[list_layer - 1], list_layer_offset);
        if (plist_tree_p->flag != 1)
        {
            return FALSE;
        }
        plist_layer_offset = list_layer_offset;
        libc_memcpy(&plist_cur_tree, plist_tree_p, sizeof(plist_tree_t));

    }
    dir_layer = list_layer;
    //    libc_memcpy(plist_list_layerno,list_layer_no,sizeof(plist_list_layerno));

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:��������Ķϵ�
 *
 * \param[in]    location--�ļ�location
 type--location�����ͣ�no used��

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_plist_set_location(plist_location_t *location, fsel_type_e type)
{
    //    plist_f_info_t *plist_f_p;
    bool ret_val = TRUE;

    type = type;

    if (0 == bs_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    if (NULL == location)//�˴������ж�filename�Ƿ�Ϊ��,��ΪҪȡlist_type
    {
        ret_val = FALSE;
        plist_type_selector = PLIST_TITLE;
    }
    else
    {
        plist_type_selector = location->list_type;
    }

    bs_get_item_info(&plist_item_info, (plist_type_selector & 0x0f));
    dir_layer = 0;
    bs_select_listno = 0;
    //    plist_list_layerno[0]=0;
    plist_layer_offset = 0;
    plist_start_sort_index = 0;
    bs_dir_total = plist_item_info.son_tree_num[0];
    libc_memset(&plist_cur_tree, 0x00, sizeof(plist_tree_t));

    if (0 == bs_dir_total)
    {
        bs_file_total = plist_file_total;//title
    }
    else
    {
        bs_file_total = 0;
    }

    if ((FALSE == ret_val) || (0x00 == *(uint32*) &location->filename))
    {
        //Ĭ���ڵ�0��
        bs_close_object();
        return FALSE;
    }

    if (0 == libc_memcmp(location->filename, "ROOT", 4))
    {
        bs_close_object();
        return TRUE;
    }

    if ((fsel_init_val.fsel_type == FSEL_TYPE_M3ULIST) && (location->list_layer == 1))
    {
        //m3u �ӱ�
        dir_layer = 1;
        m3u_curlist_index = location->layer_list_offset & 0x7f;//�� audible���
        //���»�ȡ��Ϣ
        bs_close_object();
        bs_open_object(R_NORMAL_SEEK);
        ret_val = TRUE;
    }
    else
    {
        ret_val = bs_set_into_layer(location->list_layer, location->layer_list_offset);
    }
    if (FALSE != ret_val)
    {
        if (location->file_num <= bs_file_total)
        {
            bs_select_listno = location->file_num;
        }
        else
        {
            bs_select_listno = 0;
        }
        //��ֵ����cur_ext_name
        libc_memcpy(&cur_ext_name, location->filename, 4);
    }

    bs_close_object();
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ��ѡ�е��ļ����б���ɾ��
 *
 * \param[in]        type--ɾ�����ͣ�(ֻ֧��ɾ�������ļ�)
 list_no--ѡ�����(1~N)
 location--�ļ���location
 * \param[out]

 * \return           TRUE OR FALSE

 * \note            ��list_no��Ϊ0��ʹ��list_noΪ����ɾ��
 ��list_noΪ0��ʹ��location��ɾ���ļ�
 *******************************************************************************/
bool bs_plist_delete(brow_del_e type, uint16 list_no, plist_location_t *location)
{
    bool ret_val = TRUE;
    uint8 tmp_fsel_type;
    uint8 count = 3;
    uint8 tmp_file_dir_info[8];//ͬ������ʹ��

    if (0 == bs_open_object(OPEN_MODIFY))
    {
        return FALSE;
    }

    if (list_no == 0)
    {
        if (NULL != location)
        {
            libc_memcpy(tmp_file_dir_info, &location->cluster_no, 8);
        }
        else
        {
            ret_val = FALSE;
        }
    }

    if (ret_val != FALSE)
    {
        if (m3u_curlist_index != 0xffff)
        {
            ret_val = bs_m3ufile_delete_func(type, list_no, tmp_file_dir_info);
            plist_file_total--;
        }
        else
        {
            ret_val = bs_plist_delete_func(type, list_no, tmp_file_dir_info);
        }

    }

    bs_close_object();

    if (FALSE != ret_val)
    {
        bs_file_total--;

        if ((fsel_init_val.fsel_type == FSEL_TYPE_PLAYLIST) || (m3u_curlist_index != 0xffff))
        {
            //M3U�ӱ�MUSIC�����б����Ҫ�������ղؼ�ɾ��
            tmp_fsel_type = FSEL_TYPE_LISTFAVOR1;
            while (count > 0)
            {
                count--;
                if (0 != bs_open_other(tmp_fsel_type, OPEN_MODIFY))
                {
                    bs_uspl_delete_func(FS_FILE, 0, tmp_file_dir_info);
                    bs_close_object();
                }
                tmp_fsel_type++;
            }
        }

        if ((fsel_init_val.fsel_type == FSEL_TYPE_PLAYLIST) || (fsel_init_val.fsel_type == FSEL_TYPE_LISTAUDIBLE))
        {
            if (0 != bs_open_other(FSEL_TYPE_M3ULIST, OPEN_MODIFY))
            {
                bs_m3ufile_delete_func(FS_FILE, 0, tmp_file_dir_info);
                bs_close_object();
            }
        }

        if (m3u_curlist_index != 0xffff)//m3uɾ���ļ�
        {
            if (0 != bs_open_other(FSEL_TYPE_PLAYLIST, OPEN_MODIFY))
            {
                bs_plist_delete_func(FS_FILE, 0, tmp_file_dir_info);
                bs_close_object();
            }

            if (0 != bs_open_other(FSEL_TYPE_LISTAUDIBLE, OPEN_MODIFY))
            {
                bs_plist_delete_func(FS_FILE, 0, tmp_file_dir_info);
                bs_close_object();
            }
        }

        //���´���ɾ����ɾ��ֻ��8�ֽھͿ�����
        vfs_file_dir_offset(vfs_mount, NULL, bs_temp_buf, 0);
        vfs_file_dir_offset(vfs_mount, NULL, tmp_file_dir_info, 1);

        if (FSEL_TYPE_LISTEBOOK == fsel_init_val.fsel_type)
        {
            //ebookӦ����ɾ����ǩ
            bs_del_ebookbmk();
        }

        vfs_file_dir_remove(vfs_mount, NULL, FS_FILE);
        vfs_file_dir_offset(vfs_mount, NULL, bs_temp_buf, 1);
    }

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description: ��λ���ļ��ڴ��̵�Ŀ¼��λ��
 *
 * \param[in]    location -- �ļ���λ����Ϣ
 type --location���ͣ�no used��

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_plist_set_file(plist_location_t *location, fsel_type_e type)
{
    type = type;
    return vfs_file_dir_offset(vfs_mount, &location->dir_layer_info, &location->cluster_no, 1);
}

//***************************************************
//list_no,0~n
//void bs_m3u_get_total(uint16 list_no)
//{
//    handle handle_pt;
//    m3udat_head_t *m3u_dat_pt;
//
//    list_no--;
//    m3u_curlist_index = list_no;
//    bs_dir_total = 0;
//    plist_info_offset = (M3U_DAT_FILEOFFSET/SECTOR_SIZE);
//
//    vfs_file_dir_offset(vfs_mount, NULL, &lib_pfile_offset, 1);
//    handle_pt = vfs_file_open(vfs_mount, NULL, R_NORMAL_SEEK);
//    if(handle_pt!=0)
//    {
//        vfs_file_read(vfs_mount, bs_temp_buf, SECTOR_SIZE, handle_pt);
//        m3u_dat_pt = (m3udat_head_t*)(bs_temp_buf + M3U_DAT_HEADOFFSET);
//        bs_file_total = m3u_dat_pt->list_file_num[list_no];
//        plist_file_total = m3u_dat_pt->dat_file_total;
//        vfs_file_close(vfs_mount,handle_pt);
//    }
//    else
//    {
//        bs_file_total = 0;
//    }
//
//}


