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
 * \brief    �ղؼ���ӣ�ɾ������ղ���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"
#include "eh_rdata_ex.h"

//#ifdef USE_83NAME_CREAT
//static const char userpl_name[USERPL_MAX][12]=
//{   "USERPL1 PL ","USERPL2 PL ","USERPL3 PL "};
//#else
//static const char userpl_name[USERPL_MAX][12] =
//{ "USERPL1.PL", "USERPL2.PL", "USERPL3.PL" };
//#endif

static uint8 fav_opt_temp_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;
static uint16 fav_opt_read_record _BANK_DATA_ATTR_;
//static uint32 fav_opt_file_size _BANK_DATA_ATTR_;
//static uint8 fav_opt_write_add_flag _BANK_DATA_ATTR_;

userpl_file_t *fav_opt_user_pl_p _BANK_DATA_ATTR_;
userpl_head_t fav_opt_user_pl_head _BANK_DATA_ATTR_;
uint16 fav_opt_existfile _BANK_DATA_ATTR_;
uint8 fav_obj_open_mode _BANK_DATA_ATTR_;

extern handle fav_opt_open_userpl(uint8 sn,uint8 *buf,uint8 creat_flag) __FAR__;
extern bool fav_opt_judge_exist(flist_location_t *location,uint8 *buf,handle t_handle) __FAR__;
extern bool fav_opt_update_head(uint8 *buf,handle t_handle) __FAR__;
extern bool fav_opt_delete_sub(uint16 del_file_offset,uint8 *buf,handle t_handle) __FAR__;
//static bool fav_update_last(void);
extern bool fav_opt_set_pos(uint16 num, uint8 *buf, handle t_handle);

/******************************************************************************/
/*
 * \par  Description:�ر��ļ�
 *
 * \param[in]
 * \param[out]
 * \return

 * \note
 *******************************************************************************/
void fav_opt_close_userpl(handle t_handle)
{
    if (0xee != fav_obj_open_mode)
    {
        vfs_file_close(vfs_mount, t_handle);
    }

}

/******************************************************************************/
/*
 * \par  Description: ���ղؼ���ָ������λ������
 *
 * \param[in]    offset--����ƫ��
 buf-- ����buffer
 w_flag--��д���ݱ��
 t_handle--�ļ����

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//���ղؼ���ָ���ļ����ڵ�����
bool fav_opt_read_data(uint16 offset, uint8 *buf, uint8 w_flag, handle t_handle)
{
    if (offset != fav_opt_read_record)
    {
        if (w_flag != 0)
        {
            if (0xee == fav_obj_open_mode)
            {
                //��дVRAM��obj_handle���VM��ַ
                sys_vm_write(buf, t_handle + fav_opt_read_record * SECTOR_SIZE);
            }
            else
            {
                vfs_file_seek(vfs_mount, 0 - SECTOR_SIZE, SEEK_CUR, t_handle);
                vfs_file_write(vfs_mount, buf, SECTOR_SIZE, t_handle);
            }

        }

        if (0xee == fav_obj_open_mode)
        {
            sys_vm_read(buf, t_handle + offset * SECTOR_SIZE, SECTOR_SIZE);
        }
        else
        {
            vfs_file_seek(vfs_mount, offset * SECTOR_SIZE, SEEK_SET, t_handle);
            vfs_file_read(vfs_mount, buf, SECTOR_SIZE, t_handle);
        }

        fav_opt_read_record = offset;
    }

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description: ���ղؼ���ָ���ļ�����Ϣ
 *
 * \param[in]    offset--�ļ�ƫ��
 buf-- ����buffer
 w_flag--��д���ݱ��
 t_handle--�ļ����

 * \param[out]

 * \return       userpl_file_t

 * \note
 *******************************************************************************/
userpl_file_t *fav_opt_read_file(uint16 offset, uint8 *buf, uint8 w_flag, handle t_handle)
{
    uint16 temp_offset;

    temp_offset = offset / USERPL_PRE_SEC + USERPL_HEAD_SEC;//add head's sectors
    fav_opt_read_data(temp_offset, buf, w_flag, t_handle);
    return (userpl_file_t*) (buf + (offset % USERPL_PRE_SEC) * USERPL_FILE_SIZE);
}

/******************************************************************************/
/*
 * \par  Description:����ļ����ղؼ�
 *
 * \param[in]    location--�ļ���location����
 target_fsel_type--Ŀ���ղؼ�

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_favorite_add(flist_location_t *location, fsel_type_e target_fsel_type)
{
    uint16 temp_prev;
    uint16 temp_next;
    handle t_handle;
    bool err_flag;

    if (location == NULL)
    {
        return FALSE;
    }

    err_flag = TRUE;
    fav_opt_read_record = 0xffff;

    //Ŀ���ж�
    if ((target_fsel_type >= FSEL_TYPE_LISTFAVOR1) && (target_fsel_type <= FSEL_TYPE_LISTFAVOR3))
    {
        target_fsel_type = target_fsel_type - FSEL_TYPE_LISTFAVOR1;
    }
    else
    {
        fsel_error = FSEL_ERR_INPUT;
        return FALSE;
    }

    t_handle = fav_opt_open_userpl(target_fsel_type, fav_opt_temp_buf, 1);

    if (t_handle == 0)
    {
        fsel_error = FSEL_ERR_FAIL;
        return FALSE;
    }

    if (FALSE != fav_opt_judge_exist(location, fav_opt_temp_buf, t_handle))
    {
        //�ļ������ղؼ���
        fav_opt_close_userpl(t_handle);
        fsel_error = FSEL_ERR_EXIST;
        return FALSE;
    }

    fav_opt_user_pl_p = (userpl_file_t *) fav_opt_temp_buf;

    if (fav_opt_user_pl_head.file_num < USERPL_FILE_MAX)
    {
        //�ղؼ�δ�����������ļ�׷��
        fav_opt_user_pl_head.file_num++;
        if (1 == fav_opt_user_pl_head.file_num)
        {
            //ԭ�ղؼ�Ϊ�գ���ͷ����
            temp_next = fav_opt_user_pl_head.first_offset;
            temp_prev = temp_next;
        }
        else
        {
            fav_opt_user_pl_p = (userpl_file_t *) fav_opt_read_file(fav_opt_user_pl_head.last_offset, fav_opt_temp_buf,
                    0, t_handle);
            temp_next = fav_opt_user_pl_p->next_offset;
            temp_prev = fav_opt_user_pl_head.last_offset;
        }

        //���item
        //����ǰ�ղؼ������¼�¼
        fav_opt_user_pl_p = (userpl_file_t *) fav_opt_read_file(temp_next, fav_opt_temp_buf, 0, t_handle);

        libc_memcpy(&fav_opt_user_pl_p->location, location, sizeof(flist_location_t));
        fav_opt_user_pl_p->prev_offset = temp_prev;
        fav_opt_user_pl_head.last_offset = temp_next;

        if (fav_opt_user_pl_p->flag == 0) //,��Ϊ2���ʾ��ɾ����
        {
            //��ʾ֮ǰδ�ù�
            temp_next++;
            fav_opt_user_pl_p->next_offset = temp_next;
        }

        fav_opt_user_pl_p->flag = 1;

        //����cover_offset
        fav_opt_user_pl_head.cover_offset = fav_opt_user_pl_head.first_offset;

    }
    else
    {
        //�ղؼ�������ֱ�Ӹ���
        fav_opt_user_pl_p = (userpl_file_t *) fav_opt_read_file(fav_opt_user_pl_head.cover_offset, fav_opt_temp_buf, 0,
                t_handle);
        fav_opt_user_pl_head.cover_offset = fav_opt_user_pl_p->next_offset;
        if (fav_opt_user_pl_head.cover_offset >= USERPL_FILE_MAX)
        {
            fav_opt_user_pl_head.cover_offset = fav_opt_user_pl_head.first_offset;
        }
        libc_memcpy(&fav_opt_user_pl_p->location, location, sizeof(flist_location_t));
    }

    //��д���һ������,�޸�����ͷ
    fav_opt_update_head(fav_opt_temp_buf, t_handle);
    fav_opt_close_userpl(t_handle);

    return err_flag;
}

/******************************************************************************/
/*
 * \par  Description: ����ղؼв���
 *
 * \param[in]    target_fsel_type--������Ŀ���ղؼ�

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_favorite_clear(fsel_type_e target_fsel_type)
{
    uint16 temp;
    uint16 temp_total;
    uint16 temp_offset;
    //    userpl_head_t *upl_h;
    handle t_handle;
    fav_opt_read_record = 0xffff;

    //��������вŻ�����չ��ܡ�

    if ((target_fsel_type >= FSEL_TYPE_LISTFAVOR1) && (target_fsel_type <= FSEL_TYPE_LISTFAVOR3))
    {
        target_fsel_type = target_fsel_type - FSEL_TYPE_LISTFAVOR1;
    }
    else
    {
        fsel_error = FSEL_ERR_INPUT;
        return FALSE;
    }

    t_handle = fav_opt_open_userpl(target_fsel_type, fav_opt_temp_buf, 0);

    if (0 == t_handle)
    {
        fsel_error = FSEL_ERR_FAIL;
        return FALSE;
    }

    if (0 == fav_opt_user_pl_head.file_num)
    {
        fav_opt_close_userpl(t_handle);
        return FALSE;
    }

    temp_total = fav_opt_user_pl_head.file_num;
    fav_opt_user_pl_head.file_num = 0;
    temp_offset = fav_opt_user_pl_head.first_offset;
    temp = 0;
    fav_opt_user_pl_head.first_offset = 0;
    fav_opt_user_pl_head.last_offset = 0;
    fav_opt_user_pl_head.cover_offset = 0;

    while (temp < temp_total)
    {
        fav_opt_user_pl_p = (userpl_file_t*) fav_opt_read_file(temp_offset, fav_opt_temp_buf, 1, t_handle);

        temp_offset = fav_opt_user_pl_p->next_offset;
        fav_opt_user_pl_p->flag = 2; //��ɾ�����
        temp++;
    }

    //��д���һ������,�޸�����ͷ
    fav_opt_update_head(fav_opt_temp_buf, t_handle);
    fav_opt_close_userpl(t_handle);

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description: ���ļ����ղؼ���ɾ��
 *
 * \param[in]    target_fsel_type--������Ŀ���ղؼ�
 list_no--ѡ�����
 location--�ļ���location
 * \param[out]

 * \return           TRUE OR FALSE

 * \note            ��list_no��Ϊ0��ʹ��list_noΪ����ɾ��
 ��list_noΪ0��ʹ��location��ɾ���ļ�
 *******************************************************************************/
bool fsel_favorite_delete(fsel_type_e target_fsel_type, uint16 list_no, flist_location_t *location)
{
    bool ret_val = TRUE;
    handle t_handle;
    fav_opt_read_record = 0xffff;
    //Ŀ���ж�

    if ((target_fsel_type >= FSEL_TYPE_LISTFAVOR1) && (target_fsel_type <= FSEL_TYPE_LISTFAVOR3))
    {
        target_fsel_type = target_fsel_type - FSEL_TYPE_LISTFAVOR1;
    }
    else
    {
        fsel_error = FSEL_ERR_INPUT;
        return FALSE;
    }

    t_handle = fav_opt_open_userpl(target_fsel_type, fav_opt_temp_buf, 1);

    if (t_handle == 0)
    {
        fsel_error = FSEL_ERR_FAIL;
        return FALSE;
    }

    if (list_no != 0)
    {
        ret_val = fav_opt_set_pos(list_no, fav_opt_temp_buf, t_handle);
    }
    else
    {
        if (NULL == location)
        {
            ret_val = FALSE;
        }
        else
        {
            ret_val = fav_opt_judge_exist(location, fav_opt_temp_buf, t_handle);
        }
    }

    if (FALSE != ret_val)
    {
        ret_val = fav_opt_delete_sub(fav_opt_existfile, fav_opt_temp_buf, t_handle);
    }
    else
    {
        //�ļ������ղؼ���
        fsel_error = FSEL_ERR_NO_EXIST;
    }

    fav_opt_close_userpl(t_handle);
    return ret_val;

}

/******************************************************************************/
/*
 * \par  Description: ��ȡ�ղؼ�����
 *
 * \param[in]    target_fsel_type--������Ŀ���ղؼ�

 * \param[out]

 * \return       �ղؼ��ļ�����

 * \note
 *******************************************************************************/
uint16 fsel_favorite_get_total(fsel_type_e target_fsel_type)
{
    handle t_handle;
    //Ŀ���ж�
    if ((target_fsel_type >= FSEL_TYPE_LISTFAVOR1) && (target_fsel_type <= FSEL_TYPE_LISTFAVOR3))
    {
        target_fsel_type = target_fsel_type - FSEL_TYPE_LISTFAVOR1;
    }
    else
    {
        return 0;
    }

    t_handle = fav_opt_open_userpl(target_fsel_type, fav_opt_temp_buf, 1);
    fav_opt_close_userpl(t_handle);

    return fav_opt_user_pl_head.file_num;
}

