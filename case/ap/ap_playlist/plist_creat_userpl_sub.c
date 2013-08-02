/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    ������д�ļ��ĸ���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               Ĭ�ϴ����ļ�ʹ�ó���
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

static uint8 temp_name_buf[8]_BANK_DATA_ATTR_;//USERPL_TITLE_MAX
userpl_head_t fav_opt_user_pl_head _BANK_DATA_ATTR_;
uint16 fav_save_read_offset _BANK_DATA_ATTR_; //�������offset

/******************************************************************************/
/*
 * \par  Description: ���˿ո�,ת��Ϊ xx.yyy��ʽ
 *
 * \param[in]  name_buf

 * \param[out]

 * \return      name_len

 * \note
 *******************************************************************************/
//���˿ո�,ת��Ϊ xx.yyy��ʽ
//static uint8 deal_if_83name(uint8 *name_buf)
//{
//    uint8 i, j;
//
//    //��չ��������1���ֽ�
//    for (i = 11; i > 8; i--)
//    {
//        name_buf[i] = name_buf[i - 1];
//    }
//
//    //�����ļ����ո�
//    for (i = 7; i > 0; i--)
//    {
//        if (name_buf[i] != 0x20)
//        {
//            break;
//        }
//    }
//
//    //�ӵ�
//    i++;
//    name_buf[i] = 0x2e; //��"."
//
//    //������չ���ո�
//    for (j = 9; j < 12; j++)
//    {
//        if (name_buf[j] != 0x20)
//        {
//            i++;
//            name_buf[i] = name_buf[j];
//        }
//    }
//
//    if (name_buf[i] != 0x2e)
//    {
//        i++;
//    }
//
//    name_buf[i] = 0x00;
//
//    return i + 1;
//}

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
static void fav_opt_read_data(uint16 sector_offset, uint8 *buf, uint8 w_flag, handle t_handle)
{
    if (fav_save_read_offset != sector_offset)
    {
        if (drv_sn == 0)
        {
            //����
            if (1 == w_flag)
            {
                //��дVRAM��obj_handle���VM��ַ
                sys_vm_write(buf, t_handle + fav_save_read_offset * SECTOR_SIZE);
            }
            sys_vm_read(buf, t_handle + (sector_offset * SECTOR_SIZE), SECTOR_SIZE);
        }
        else
        {
            if (1 == w_flag)
            {
                vfs_file_seek(ap_vfs_mount, fav_save_read_offset * SECTOR_SIZE, SEEK_SET, t_handle);
                vfs_file_write(ap_vfs_mount, buf, SECTOR_SIZE, t_handle);
            }

            vfs_file_seek(ap_vfs_mount, sector_offset * SECTOR_SIZE, SEEK_SET, t_handle);
            vfs_file_read(ap_vfs_mount, buf, SECTOR_SIZE, t_handle);
        }

        fav_save_read_offset = sector_offset;
    }

}

/******************************************************************************/
/*
 * \par  Description: ���ղؼж�Ӧ�ļ���Ϣ
 *
 * \param[in]  t_file_offset--�ļ���λ��ƫ�����0~n

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static userpl_file_t *fav_opt_read_file(uint16 t_file_offset, uint8 *buf, uint8 w_flag, uint32 t_handle)
{
    uint16 sector_offset;
    uint16 buffer_offset;

    sector_offset = t_file_offset / USERPL_PRE_SEC + USERPL_HEAD_SEC;//add head sector
    buffer_offset = (t_file_offset % USERPL_PRE_SEC) * USERPL_FILE_SIZE;

    fav_opt_read_data(sector_offset, buf, w_flag, t_handle);
    return (userpl_file_t*) (buf + buffer_offset);

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
static void fav_opt_update_head(uint8 *buf, handle t_handle)
{
    //��д���һ������,�޸�����ͷ
    fav_opt_read_data(0, buf, 1, t_handle);
    libc_memcpy(buf, &fav_opt_user_pl_head, sizeof(userpl_head_t));
    fav_opt_read_data(1, buf, 1, t_handle);
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
    userpl_file_t *fav_opt_user_pl_p;

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

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: �����ղؼ���Ϣ�������ڵ��ļ�����ɾ��
 *
 * \param[in]   buf,uspl_hdl

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void reflash_userpl_table(uint8 *buf, uint32 uspl_hdl)
{
    userpl_file_t *file_info_p;
    flist_location_t *flist_loca_p;
    uint16 uspl_file_total;
    uint16 uspl_cur_offset;
    uint16 uspl_next_offset;
    uint16 name_len;
    bool ret_val;
    uint8 del_flag = 0;

    libc_memcpy(&fav_opt_user_pl_head, buf, sizeof(userpl_head_t));
    uspl_file_total = fav_opt_user_pl_head.file_num;
    uspl_cur_offset = fav_opt_user_pl_head.first_offset;
    uspl_next_offset = fav_opt_user_pl_head.first_offset;
    fav_save_read_offset = 0;

    while (uspl_file_total > 0)
    {
        uspl_file_total--;
        file_info_p = fav_opt_read_file(uspl_cur_offset, buf, 1, uspl_hdl);

        if (file_info_p->flag != 1)
        {
            break;
        }

        uspl_next_offset = file_info_p->next_offset;

        flist_loca_p = (flist_location_t*) &file_info_p->location;
        ret_val = vfs_file_dir_offset(ap_vfs_mount, &flist_loca_p->dir_layer_info, &flist_loca_p->cluster_no, 1);

        if (FALSE != ret_val)
        {

            //�Ƚ�ȥ��׺��
            name_len = vfs_get_name(ap_vfs_mount, temp_name_buf, 0);

            if ((3 == name_len) && (0 == libc_memcmp(temp_name_buf, flist_loca_p->filename, 4)))
            {
                //�ļ�����
                uspl_cur_offset = uspl_next_offset;
                continue;
            }

            //            name_len = vfs_get_name(ap_vfs_mount, temp_name_buf, USERPL_TITLE_MAX / 2);
            //            if (*(uint16*) temp_name_buf == 0xfeff)
            //            {
            //                name_len = name_len << 1;
            //            }
            //            else
            //            {
            //                //�����ղؼд��title��ʽ�� xxx.yyy
            //                name_len = (uint16)deal_if_83name(temp_name_buf);
            //            }
            //
            //            if (0 == libc_memcmp(temp_name_buf, flist_loca_p->title, (uint32) name_len))
            //            {
            //                //�ļ�����
            //                uspl_cur_offset = uspl_next_offset;
            //                continue;
            //            }
        }

        del_flag = 1;
        fav_opt_delete_sub(uspl_cur_offset, buf, uspl_hdl);
        uspl_cur_offset = uspl_next_offset;

    }

    if (1 == del_flag)
    {
        //��д���һ������,�޸�����ͷ
        fav_opt_update_head(buf, uspl_hdl);
    }

}

