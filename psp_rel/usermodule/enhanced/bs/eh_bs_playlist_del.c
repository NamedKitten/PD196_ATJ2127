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
 * \file     eh_bs_playlist_del.c
 * \brief    �����б����
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �ļ�ϵͳ��VRAM
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_bs.h"

//��֤ʹ�����±���ʱ��û�з�����bank
static uint16 del_file_index _BANK_DATA_ATTR_;
static uint16 next_file_index _BANK_DATA_ATTR_;
static bool del_tree_flag _BANK_DATA_ATTR_;

static plist_item_info_t temp_plist_item_info _BANK_DATA_ATTR_;

extern uint8 bs_temp_buf[SECTOR_SIZE];

extern bool bs_read_sector_data(uint16 sector_offset) __FAR__;

extern uint16 bs_get_file_index_byno(uint16 sector_start,uint16 index_offset,uint16 t_file_no) __FAR__; //��bank
/******************************************************************************/
/*
 * \par  Description:��ȡplaylist����ļ��ṹ��Ϣ
 *
 * \param[in]    file_offset �ļ������0~N

 * \param[out]

 * \return       �ļ���Ϣ�ṹ

 * \note
 *******************************************************************************/
static plist_f_info_t * bs_plist_get_file_info(uint16 file_offset)
{
    uint16 buf_offset;
    uint16 sector_offset; //��һ������Ϊͷ

    sector_offset = plist_info_offset + file_offset / (SECTOR_SIZE / PLIST_FILE_SIZE);
    buf_offset = (file_offset * PLIST_FILE_SIZE) % SECTOR_SIZE;

    bs_read_sector_data(sector_offset);

    return (plist_f_info_t*) (bs_temp_buf + buf_offset);
}

/******************************************************************************/
/*
 * \par  Description: ��ȡָ����TREE��Ϣ
 *
 * \param[in]    tree_start_offset--tree��ŵ�����ƫ��(0~N)
 tree_offset--��ȡ��treeλ��ƫ��(0~N)

 * \param[out]

 * \return       TREE��Ϣ

 * \note
 *******************************************************************************/
plist_tree_t *bs_plist_read_tree_data(uint16 tree_start_offset, uint16 tree_offset)
{
    uint16 buf_offset;
    uint16 sector_offset;

    sector_offset = tree_start_offset + (tree_offset * sizeof(plist_tree_t)) / SECTOR_SIZE;
    buf_offset = (tree_offset * sizeof(plist_tree_t)) % SECTOR_SIZE;
    bs_read_sector_data(sector_offset);

    return (plist_tree_t*) (bs_temp_buf + buf_offset);
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
static bool bs_plist_get_item_info(plist_item_info_t *item_info, plist_type_e type)
{
    plist_item_t *plist_item_p;
    bs_read_sector_data(0);
    plist_item_p = (plist_item_t*) (bs_temp_buf + sizeof(plist_head_t));
    plist_item_p += ((uint8) type & 0x0f);
    libc_memcpy(item_info, &plist_item_p->son_tree_num, sizeof(plist_item_info_t));
    return TRUE;

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
static uint16 bs_plist_get_object_index(uint16 index_start_offset, uint16 index)
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
 * \par  Description: ɾ��ָ����index��������ɾ��ǰ��index
 *
 * \param[in]    index_start_offset--index����ʼ����ƫ��0~N
 index--λ��ƫ��0~N

 * \param[out]

 * \return       index 0~N

 * \note
 *******************************************************************************/
//����ɾ��ǰ��index
static uint16 bs_plist_del_object_index(uint16 index_start_offset, uint16 index)
{
    uint16 sector_offset;
    uint16 buf_offset;
    uint16 object_index;

    sector_offset = index_start_offset + (index << 1) / SECTOR_SIZE;
    buf_offset = (index << 1) % SECTOR_SIZE;

    bs_read_sector_data(sector_offset);
    object_index = *(uint16 *) (bs_temp_buf + buf_offset);
    *(uint16 *) (bs_temp_buf + buf_offset) = PLIST_INVALID_INDEX;

    return object_index;
}

/******************************************************************************/
/*
 * \par  Description: ɾ��ָ��tree�����ظ�tree��ƫ��
 *
 * \param[in]    tree_start_offset--tree��ŵ���ʼ����ƫ��(0~N)
 tree_offset--��ǰ��ʼ��tree��λ��ƫ��(0~N)
 type--ָ������
 * \param[out]

 * \return       ��tree��ƫ��

 * \note
 *******************************************************************************/
//����object ��tree ƫ��
//TYPE=1�����ظ���ƫ�ƣ�TYPE=0�������ӵ�ƫ��
static uint16 bs_plist_del_object_tree(uint16 tree_start_offset, uint16 tree_offset, uint8 type)
{
    uint16 t_object_offset;
    uint16 t_prev_offset;
    uint16 t_next_offset;
    //    uint16 sector_offset;
    plist_tree_t *temp_tree;

    temp_tree = bs_plist_read_tree_data(tree_start_offset, tree_offset);

    if (temp_tree->flag == 1)
    {
        if (type == 1)
        {
            t_object_offset = temp_tree->parent_offset;
        }
        else
        {
            t_object_offset = temp_tree->son_offset;
        }

        if (temp_tree->file_total == 1)
        {
            //ɾ������tree
            temp_tree->flag = 2;
            t_prev_offset = temp_tree->prev_offset;
            t_next_offset = temp_tree->next_offset;

            if (t_prev_offset != 0xffff)
            {
                temp_tree = bs_plist_read_tree_data(tree_start_offset, t_prev_offset);
                temp_tree->next_offset = t_next_offset;
            }
            if (t_next_offset != 0xffff)
            {
                temp_tree = bs_plist_read_tree_data(tree_start_offset, t_next_offset);
                temp_tree->prev_offset = t_prev_offset;
            }

            del_tree_flag = TRUE;
        }
        else
        {
            //ֻ��������1
            temp_tree->file_total--;
            if (temp_tree->file_index == del_file_index)
            {
                //��ɾ�����ǵ�һ�����滻file_index
                temp_tree->file_index = next_file_index;
            }
            if (TRUE == del_tree_flag)// &&temp_tree->son_num>0
            {
                //���¼���ɾ��tree,�ϼ�son_num�ĸ�����һ
                temp_tree->son_num--;
            }

            del_tree_flag = FALSE;
        }
        //        }
    }
    else
    {
        t_object_offset = PLIST_INVALID_INDEX;
    }

    return t_object_offset;

}

/******************************************************************************/
/*
 * \par  Description: ������ʾ��ţ���ȡ�ļ���index
 *
 * \param[in]    index_start_offset--index �Ĵ����ʼ����ƫ��(0~N)
 start_index--��ʼ���ź�ƫ��(0~N)
 t_file_no--�ļ���file_no(1~N)
 * \param[out]

 * \return       index(0~N)

 * \note
 *******************************************************************************/
//get index
//static uint16 bs_plist_get_index_byno(uint16 index_start_offset,uint16 start_index,uint16 t_file_no)
//{
//    uint16 sector_offset;
//    uint16 buf_offset;
//    uint16 *buf_pt;
//
//    sector_offset =index_start_offset + (start_index<<1)/SECTOR_SIZE;
//    buf_offset=(start_index<<1)/SECTOR_SIZE;
//
//    buf_pt=(uint16*)(bs_temp_buf+buf_offset);
//    bs_read_sector_data(sector_offset);
//
//    while(0<t_file_no--)
//    {
//
//        while(1)
//        {
//            if(buf_pt>=(uint16 *)(bs_temp_buf+SECTOR_SIZE))
//            {
//                sector_offset++;
//                bs_read_sector_data(sector_offset);
//                buf_pt=(uint16 *)bs_temp_buf;
//            }
//            if(*buf_pt!=PLIST_INVALID_INDEX)
//            {
//                break;
//            }
//            buf_pt++;
//
//        }
//        buf_pt++;
//
//    }
//
//    return  *(buf_pt-1);
//}

/******************************************************************************/
/*
 * \par  Description: �ж��ļ��Ƿ������б���
 *
 * \param[in]    file_dir_info--�ļ���Ŀ¼��λ����Ϣ

 * \param[out]

 * \return       TRUE OR FALSE

 * \note        �ҵ���ֵdel_file_index
 *******************************************************************************/
static bool bs_plist_judge_exist(uint8 *file_dir_info)
{
    uint16 temp, temp_index;
    plist_f_info_t *plist_f_info_p;
    bool ret_val = TRUE;

    del_file_index = PLIST_INVALID_INDEX;
    if (0 == plist_file_total)
    {
        return FALSE;
    }
    temp = 0;
    temp_index = 0;

    //temp ��Ч����
    //temp_index file_index  ����4000
    while ((temp < plist_file_total) && (temp_index < 4000))
    {
        plist_f_info_p = bs_plist_get_file_info(temp_index); //
        if (1 == plist_f_info_p->flag)
        {

            if (0 == libc_memcmp(file_dir_info, &plist_f_info_p->cluster_no, 8))
            {
                del_file_index = temp_index;
                break;
            }
            else
            {
                temp++;
            }
        }
        temp_index++;
    }

    if ((temp == plist_file_total) || (temp_index == 4000))
    {

        ret_val = FALSE;
    }

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description: ����item���б��ļ���
 *
 * \param[in]    item_info--item��Ϣ
 type--ָ������
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool bs_plist_update_item_info(plist_item_info_t *item_info, plist_type_e type)
{
    plist_item_t *plist_item_p;

    bs_read_sector_data(0);
    plist_item_p = (plist_item_t *) (bs_temp_buf + sizeof(plist_head_t));
    plist_item_p += ((uint8) type & 0x0f);
    libc_memcpy(&plist_item_p->son_tree_num, item_info, sizeof(plist_item_info_t));

    return TRUE;

}
/******************************************************************************/
/*
 * \par  Description: ����file_indexɾ���б���ĳ���ļ�
 *
 * \param[in]    t_file_index (0~N)

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//ɾ���б��е�һ���ļ��������б�
static bool bs_plist_del_by_index(uint16 t_file_index)
{
    uint8 i, count;
    uint16 j;
    uint16 t_sort_index;
    uint16 t_index_parent;
    plist_f_info_t *plist_f_p;
    plist_head_t * plist_head_p;

    plist_f_p = bs_plist_get_file_info(t_file_index);

    plist_f_p->flag = 2;

    i = 0;
    count = 4;

    while (count > 0)
    {
        count--;
        bs_plist_get_item_info(&temp_plist_item_info, i);

        if (temp_plist_item_info.file_index_offset == 0x0000)
        {
            //picture��vedio��ebook�����Ϊ0x00
            i++;
            continue;//break;
            //�˴�����ʹ��break����Ϊaudible����title ��artist
        }

        t_sort_index = bs_plist_del_object_index(temp_plist_item_info.sort_index_offset, t_file_index);

        next_file_index = PLIST_INVALID_INDEX;
        j = 1;
        while (next_file_index == PLIST_INVALID_INDEX)
        {
            next_file_index = bs_plist_get_object_index(temp_plist_item_info.file_index_offset, t_sort_index + j);
            j++;
        }

        bs_plist_del_object_index(temp_plist_item_info.file_index_offset, t_sort_index);

        if (temp_plist_item_info.index_parent_offset != 0x00)
        {
            t_index_parent = bs_plist_del_object_index(temp_plist_item_info.index_parent_offset, t_file_index);
        }
        else
        {
            t_index_parent = PLIST_INVALID_INDEX;
        }

        if (t_index_parent == PLIST_INVALID_INDEX)
        {
            i++;
            continue;
        }

        j = 3;
        del_tree_flag = FALSE;
        while (j > 0)
        {
            j--;
            if (temp_plist_item_info.son_tree_num[j] > 0)
            {
                t_index_parent = bs_plist_del_object_tree(temp_plist_item_info.son_tree_offset[j], t_index_parent, 1);

                if (TRUE == del_tree_flag)
                {
                    temp_plist_item_info.son_tree_num[j]--;
                }

                if (t_index_parent == PLIST_INVALID_INDEX)
                {
                    break;
                }
            }

        }

        bs_plist_update_item_info(&temp_plist_item_info, i);
        i++;
    }

    //�����ļ�����
    bs_read_sector_data(0);
    plist_head_p = (plist_head_t *) bs_temp_buf;
    plist_file_total--;
    plist_head_p->file_total = plist_file_total;
    bs_read_sector_data(1); //updata 0 sector

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ��ѡ�е��ļ����б���ɾ��(�����ӿ�)
 *
 * \param[in]        type--ɾ�����ͣ�(ֻ֧��ɾ�������ļ�)
 list_no--ѡ�����(1~N)
 file_dir_info--�ļ���Ŀ¼��λ����Ϣ(����or���)
 * \param[out]

 * \return           TRUE OR FALSE

 * \note            ��list_no��Ϊ0��ʹ��list_noΪ����ɾ����file_dir_info Ϊ���
 ��list_noΪ0��ʹ��file_dir_info��ɾ���ļ���file_dir_info Ϊ����
 ɾ��ʹ�ñ���plist_file_total
 *******************************************************************************/
bool bs_plist_delete_func(brow_del_e type, uint16 list_no, uint8 *file_dir_info)
{
    bool ret_val = TRUE;

    save_read_offset = 0xffff;

    if (0 != list_no)
    {
        plist_f_info_t * tem_plist_f_p;
        //���ٴ���ռ�����Ҫ��bank
        del_file_index = bs_get_file_index_byno(plist_item_info.file_index_offset, plist_start_sort_index, list_no);
        //      del_file_index=bs_plist_get_index_byno(plist_item_info.file_index_offset,plist_start_sort_index,list_no);

        if (NULL != file_dir_info)
        {
            tem_plist_f_p = bs_plist_get_file_info(del_file_index);
            libc_memcpy(file_dir_info, &tem_plist_f_p->cluster_no, 8);
        }

    }
    else
    {
        if (NULL == file_dir_info)
        {
            ret_val = FALSE;
        }
        else
        {
            //�ҵ��ḳֵdel_file_index
            ret_val = bs_plist_judge_exist(file_dir_info);
        }

    }

    //ADD CODE
    if (FALSE != ret_val)
    {
        write_data_flag = TRUE;
        ret_val = bs_plist_del_by_index(del_file_index);
        write_data_flag = FALSE;
    }

    if (FALSE != ret_val)
    {
        if ((obj_open_type <= FSEL_TYPE_LISTEBOOK) && (fsel_init_val.disk == DISK_C))//ֻ��ɾ��nand�Ÿ�������
        {
            //����main_menu����ʾ����

            //#define  VM_FILE_NUM     0x00003800
            uint16* tmp_cp;
            tmp_cp = (uint16*) bs_temp_buf;
            sys_vm_read(bs_temp_buf, VM_MAINMENU_CNT_INFO, SECTOR_SIZE); //
            tmp_cp[(obj_open_type - FSEL_TYPE_PLAYLIST) % MAX_APP_NUM] = plist_file_total;
            sys_vm_write(bs_temp_buf, VM_MAINMENU_CNT_INFO);

        }
    }

    return ret_val;
}

