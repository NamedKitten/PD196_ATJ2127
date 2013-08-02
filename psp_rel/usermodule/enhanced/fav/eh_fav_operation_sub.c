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
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"
#include "eh_rdata_ex.h"

#ifdef USE_83NAME_CREAT
static const char userpl_name[USERPL_MAX][12]=
{   "USERPL1 PL ","USERPL2 PL ","USERPL3 PL "};
#else
static const char userpl_name[USERPL_MAX][12] =
{ "USERPL1.PL", "USERPL2.PL", "USERPL3.PL" };
#endif

extern userpl_file_t *fav_opt_user_pl_p _BANK_DATA_ATTR_;
extern userpl_head_t fav_opt_user_pl_head _BANK_DATA_ATTR_;
extern uint16 fav_opt_existfile _BANK_DATA_ATTR_;
extern uint8 fav_obj_open_mode _BANK_DATA_ATTR_;

extern bool fav_opt_read_data(uint16 offset,uint8 *buf,uint8 w_flag,handle t_handle) __FAR__;
extern userpl_file_t *fav_opt_read_file(uint16 offset, uint8 *buf, uint8 w_flag, handle t_handle) __FAR__;

/******************************************************************************/
/*
 * \par  Description: �ж��ļ��Ƿ������б���
 *
 * \param[in]    location--�ļ���location
 buf-- buffer��ַ
 t_handle-- ���

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fav_opt_judge_exist(flist_location_t *location, uint8 *buf, handle t_handle)
{
    //    uint32 t_file_offset;
    uint16 temp_total;
    uint16 temp_offset;
    uint16 temp;
    bool ret_val = FALSE;

    if (fav_opt_user_pl_head.file_num == 0)
    {
        return FALSE;
    }

    temp = 0;
    temp_offset = fav_opt_user_pl_head.first_offset;
    temp_total = fav_opt_user_pl_head.file_num;

    while (temp < temp_total)
    {
        fav_opt_user_pl_p = (userpl_file_t*) fav_opt_read_file(temp_offset, buf, 0, t_handle);

        if (0 == libc_memcmp(&location->cluster_no, &fav_opt_user_pl_p->location.cluster_no, 8))
        {
            fav_opt_existfile = temp_offset;
            ret_val = TRUE;
            break;
        }
        temp_offset = fav_opt_user_pl_p->next_offset;
        temp++;
    }

    return ret_val;

}

/******************************************************************************/
/*
 * \par  Description: �����ղؼ�ͷ��Ϣ
 *
 * \param[in]    buf--ʹ�õĻ���buffer
 t_handle--���

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fav_opt_update_head(uint8 *buf, handle t_handle)
{
    //��д���һ������,�޸�����ͷ
    fav_opt_read_data(0, buf, 1, t_handle);
    libc_memcpy(buf, &fav_opt_user_pl_head, sizeof(userpl_head_t));
    fav_opt_read_data(1, buf, 1, t_handle);
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ��λ��ѡ����ļ�
 *
 * \param[in]    num-- ѡ�к� 1~N
 buf-- buffer��ַ
 t_handle-- ���
 * \param[out]

 * \return       TRUE OR FALSE

 * \note        ����ֵ fav_opt_existfile
 *******************************************************************************/
bool fav_opt_set_pos(uint16 num, uint8 *buf, handle t_handle)
{
    uint16 i;
    uint16 temp_file_offset;
    userpl_file_t *tmp_user_pl;

    if (num > fav_opt_user_pl_head.file_num)
    {
        return FALSE;
    }
    temp_file_offset = fav_opt_user_pl_head.first_offset;
    i = 1;
    tmp_user_pl = fav_opt_read_file(temp_file_offset, buf, 0, t_handle);
    while (i != num)
    {
        if (tmp_user_pl->flag != 1)
        {
            return FALSE;
        }
        temp_file_offset = tmp_user_pl->next_offset;
        tmp_user_pl = fav_opt_read_file(temp_file_offset, buf, 0, t_handle);
        i++;
    }
    fav_opt_existfile = temp_file_offset;
    return TRUE;
}
/******************************************************************************/
/*
 * \par  Description: ���ղؼ���ɾ���ļ�
 *
 * \param[in]
 del_file_offset--ɾ���ļ���offset
 buf-- buffer��ַ
 t_handle-- ���

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/

bool fav_opt_delete_sub(uint16 del_file_offset, uint8 *buf, handle t_handle)
{
    uint16 temp_prev_offset;
    uint16 temp_next_offset;
    uint16 temp_del_offset;
    uint16 temp_last_next;

    if (fav_opt_user_pl_head.file_num == 0)
    {
        return FALSE;
    }

    fav_opt_user_pl_p = fav_opt_read_file(fav_opt_user_pl_head.last_offset, buf, 0, t_handle);
    temp_last_next = fav_opt_user_pl_p->next_offset;//�ȼ�¼

    temp_del_offset = del_file_offset; //֮ǰ����ʱ�Ѷ�fav_opt_existfile��ֵ

    fav_opt_user_pl_p = fav_opt_read_file(temp_del_offset, buf, 0, t_handle);
    fav_opt_user_pl_head.file_num--; //������һ
    fav_opt_user_pl_p->flag = 2; //��ɾ�����

    if (temp_del_offset == fav_opt_user_pl_head.first_offset)
    {
        //ɾ�����ǵ�һ��
        if (0 != fav_opt_user_pl_head.file_num)//
        {
            //��ֹһ���ļ�,ɾ���������Ч������β
            fav_opt_user_pl_head.first_offset = fav_opt_user_pl_p->next_offset;
            fav_opt_user_pl_p->next_offset = temp_last_next;
            fav_opt_user_pl_p->prev_offset = fav_opt_user_pl_head.last_offset;
        }
        else
        {
            temp_del_offset = 0xffff;
        }
    }
    else if (temp_del_offset == fav_opt_user_pl_head.last_offset)
    {
        //ɾ���������һ��
        fav_opt_user_pl_head.last_offset = fav_opt_user_pl_p->prev_offset;
        temp_del_offset = 0xffff;
    }
    else
    {
        //ɾ���м��
        temp_prev_offset = fav_opt_user_pl_p->prev_offset;
        temp_next_offset = fav_opt_user_pl_p->next_offset;
        fav_opt_user_pl_p->next_offset = temp_last_next;

        //������һ��ָ��
        fav_opt_user_pl_p = fav_opt_read_file(temp_prev_offset, buf, 1, t_handle);
        fav_opt_user_pl_p->next_offset = temp_next_offset;
        //������һ��ָ��
        fav_opt_user_pl_p = fav_opt_read_file(temp_next_offset, buf, 1, t_handle);
        fav_opt_user_pl_p->prev_offset = temp_prev_offset;

    }

    //���������ļ���ָ��
    if (temp_del_offset != 0xffff)
    {
        fav_opt_user_pl_p = fav_opt_read_file(fav_opt_user_pl_head.last_offset, buf, 1, t_handle);
        fav_opt_user_pl_p->next_offset = temp_del_offset;

        fav_opt_user_pl_p = fav_opt_read_file(temp_last_next, buf, 1, t_handle);
        fav_opt_user_pl_p->prev_offset = temp_del_offset;
    }
    else
    {
        if (0 == fav_opt_user_pl_head.file_num)
        {
            fav_opt_user_pl_head.last_offset = fav_opt_user_pl_head.first_offset;
        }
    }

    //��д���һ������,�޸�����ͷ
    fav_opt_update_head(buf, t_handle);

    return TRUE;
}
