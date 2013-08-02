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
 * \file     eh_bs_favorite.c
 * \brief    �ղؼ����
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               �ղؼ�����ӿڴ���
 * \par      EXTERNALIZED FUNCTIONS:
 *               �ļ�ϵͳ
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_bs.h"

static userpl_file_t *bs_user_pl _BANK_DATA_ATTR_;
static flist_location_t *bs_file_location _BANK_DATA_ATTR_;

//static bool bs_uspl_read_data(uint16 offset);
//static bool bs_uspl_next(file_brow_t *file_brow);
static bool bs_uspl_set_pos(uint16 num);
static userpl_file_t* bs_uspl_read_info(uint16 t_file_offset);

extern uint8 bs_temp_buf[SECTOR_SIZE];

extern bool bs_read_sector_data(uint16 sector_offset) __FAR__;

/******************************************************************************/
/*
 * \par  Description:��ָ��ƫ�ƺŵ��ļ���Ϣ
 *
 * \param[in]    t_file_offset--�ļ���λ��ƫ�����0~n

 * \param[out]

 * \return       �ļ���Ϣ��bufferָ��

 * \note
 *******************************************************************************/
static userpl_file_t* bs_uspl_read_info(uint16 t_file_offset)
{
    uint16 sector_offset;
    uint16 buffer_offset;

    sector_offset = t_file_offset / USERPL_PRE_SEC + USERPL_HEAD_SEC;//add head sector
    buffer_offset = (t_file_offset % USERPL_PRE_SEC) * USERPL_FILE_SIZE;
    bs_read_sector_data(sector_offset);

    return (userpl_file_t*) (bs_temp_buf + buffer_offset);
}

/******************************************************************************/
/*
 * \par  Description: ��λ��ѡ����ļ�
 *
 * \param[in]    num-- ѡ�к� 1~N

 * \param[out]

 * \return       TRUE OR FALSE

 * \note        ����ֵ bs_uspl_cur �� uspl_file_offset
 *******************************************************************************/
static bool bs_uspl_set_pos(uint16 num)
{
    uint16 i;
    //    uint16 temp;

    if (0 == bs_file_total)
    {
        return FALSE;
    }

    if (num > bs_file_total)
    {
        return FALSE;
    }

    save_read_offset = 0xffff;
    uspl_file_offset = uspl_first_offset;
    i = 1;
    bs_user_pl = bs_uspl_read_info(uspl_file_offset);
    while (i != num)
    {
        if (bs_user_pl->flag != 1)
        {
            return FALSE;
        }
        uspl_file_offset = bs_user_pl->next_offset;
        bs_user_pl = bs_uspl_read_info(uspl_file_offset);
        i++;
    }
    bs_uspl_cur = num;
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: �������ļ���
 *
 * \param[in]    type--��������
 list_no--ѡ��

 * \param[out]   dir_brow--������Ϣ

 * \return       TRUE OR FALSE

 * \note        ��Ϊ�ղؼ�ֻ��һ�㣬���Ըýӿ�ֻ������ȷ����
 *******************************************************************************/
bool bs_uspl_enter(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow)
{
    type = type;

    if (0 == bs_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }
    if (dir_brow != NULL)
    {
        dir_brow->dir_total = bs_dir_total;
        dir_brow->file_total = bs_file_total;
        dir_brow->layer_no = 0;
    }

    bs_select_listno = 0;
    bs_close_object();
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ��ȡ��ʾ��Ϣ
 *
 * \param[in]    decode_param--������ʾ����

 * \param[out]   file_list--��������Ϣ

 * \return       TRUE OR FALSE

 * \note    ����ֵ bs_uspl_cur �� uspl_file_offset
 *******************************************************************************/
bool bs_uspl_get_items(brow_decode_t *decode_param, void *file_list)
{
    file_brow_t *file_name;
    uint16 i;
    uint16 count;
    uint8 len;
    uint8 out_flag = 0; //������Χ���

    if (0 == bs_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    if (decode_param->top > bs_file_total)
    {
        bs_close_object();
        return FALSE;
    }

    save_read_offset = 0xffff;
    file_name = (file_brow_t *) file_list;
    count = decode_param->num;
    i = decode_param->top;

    //�ȶ�λ��ʼλ��
    if (FALSE == bs_uspl_set_pos(decode_param->top))
    {
        bs_close_object();
        return FALSE;
    }

    while (count > 0)
    {
        if (out_flag == 0)
        {
            bs_user_pl = bs_uspl_read_info(uspl_file_offset);
            if ((file_name->name_len - 2) > (USERPL_TITLE_MAX - 2))
            {
                len = USERPL_TITLE_MAX - 2;
            }
            else
            {
                len = file_name->name_len - 2;
            }

            libc_memcpy(file_name->name_buf, bs_user_pl->location.title, (uint32) len);
            libc_memcpy(file_name->ext, bs_user_pl->location.filename, 4);
            file_name->name_buf[len] = 0x00;
            file_name->name_buf[len + 1] = 0x00;

            if (i == bs_file_total)
            {
                out_flag = 1;
            }
            else
            {
                uspl_file_offset = bs_user_pl->next_offset;
            }

        }
        else
        {
            *(uint32*) (&file_name->ext) = 0x00;
            file_name->name_buf[0] = 0x00;
            file_name->name_buf[1] = 0x00;
        }

        file_name++;
        count--;
        i++;
    }

    bs_uspl_cur = count;

    bs_close_object();

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ��������ϵ�
 *
 * \param[in]    location--�ļ��ṹ
 type--location������(no used)

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_uspl_set_location(flist_location_t*location, fsel_type_e type)
{
    bool ret_val = TRUE;

    type = type;

    if (0 == bs_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    bs_select_listno = 0;
    bs_file_total = uspl_file_total;
    uspl_file_offset = uspl_first_offset;
    bs_dir_total = 0;

    if ((NULL == location) || (0x00 == *(uint32*) location->filename))
    {
        bs_close_object();
        return FALSE;
    }

    if (0 == libc_memcmp(location->filename, "ROOT", 4))
    {
        bs_close_object();
        return TRUE;
    }

    if ((0x00 == location->file_num) || (location->file_num > bs_file_total))
    {
        ret_val = FALSE;
    }

    if (FALSE != ret_val)
    {
        ret_val = bs_uspl_set_pos(location->file_num);
        if (FALSE != ret_val)
        {
            bs_user_pl = bs_uspl_read_info(uspl_file_offset);
            bs_file_location = (flist_location_t *) &bs_user_pl->location;

            if (0 != libc_memcmp(&location->cluster_no, &bs_file_location->cluster_no, 8))
            {
                ret_val = FALSE;
            }
            else
            {
                bs_select_listno = location->file_num;
                //��ֵ����cur_ext_name
                libc_memcpy(&cur_ext_name, location->filename, 4);
            }
        }
    }
    bs_close_object();
    return ret_val;

}

/******************************************************************************/
/*
 * \par  Description: ��ȡ��ʾ�ļ��Ķϵ�
 *
 * \param[in]       type--�ϵ�����(no used)

 * \param[out]      location--��ϵ���Ϣ

 * \return          TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_uspl_get_location(flist_location_t*location, fsel_type_e type)
{
    type = type;

    if (0 == bs_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    if (0 == bs_file_total)
    {
        bs_close_object();
        return FALSE;
    }

    save_read_offset = 0xffff;

    if (bs_select_listno != 0)
    {
        bs_uspl_set_pos(bs_select_listno);
        bs_user_pl = bs_uspl_read_info(uspl_file_offset);
        libc_memcpy(location, &bs_user_pl->location, sizeof(flist_location_t));

        location->file_total = bs_file_total;
        location->file_num = bs_select_listno;
    }
    else
    {
        libc_memcpy(location->filename, "ROOT", 4);

    }

    bs_close_object();
    return TRUE;
}

