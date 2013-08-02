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

/******************************************************************************/
/*
 * \par  Description: mbyte to wchar
 *
 * \param[in]    dest��src

 * \param[out]

 * \return  UNICODE ��������������ʶ���ͽ�����

 * \note    ASCת��UNICODE���Զ������ʶ���ͽ�����
 *******************************************************************************/
static uint16 mbyte_to_wchar(uint8 *dest, uint8 *src)
{
    uint16 num = 0;

#ifdef USE_83NAME_CREAT

    while(*src!=0x00)
    {
        *dest=*src;
        dest++;
        src++;
        num++;
    }

    dest=0x00;

#else

    *dest = 0xff;
    *(dest + 1) = 0xfe;
    dest += 2;

    while (*src != 0x00)
    {
        *dest = *src;
        *(dest + 1) = 0x00;
        dest += 2;
        src++;
        num++;
    }

    *dest = 0x00;
    *(dest + 1) = 0x00;

#endif

    return num;

}

/******************************************************************************/
/*
 * \par  Description: ��VRAM�ղؼ�
 *
 * \param[in]    file_name--�ղؼе��ļ���
 buf--ʹ�õĻ���buffer
 creat_flag--�����ղؼ��ļ���־
 * \param[out]

 * \return       �ļ���� ����0--�ɹ���0--ʧ��

 * \note         ����fav_opt_user_pl_head��Ϣ
 *******************************************************************************/
handle fav_opt_open_userpl_vram(uint8 sn, uint8 *buf, uint8 creat_flag)
{
    uint8 temp;
    uint32 uspl_vm_addr;
    userpl_head_t *upl_h;

    upl_h = (userpl_head_t *) buf;
    uspl_vm_addr = VM_USERPL_INFO + sn * USERPL_VM_SIZE;
    sys_vm_read(buf, uspl_vm_addr, SECTOR_SIZE);

    if ((buf[510] != 0x55) || (buf[511] != 0xAA))
    {
        //���ʽ���ԣ��ؽ�
        goto creat_file;
    }

    if (0 != libc_memcmp(upl_h->file_name, userpl_name[sn], 7))
    {
        //���ʽ���ԣ��ؽ�
        creat_file:

        if (0 == creat_flag)
        {
            return 0;
        }

        libc_memset(buf, 0, SECTOR_SIZE);
        upl_h = (userpl_head_t*) buf;
        libc_memcpy(upl_h->file_name, userpl_name[sn], 7);
        upl_h->file_num = 0;
        upl_h->first_offset = 0;
        upl_h->last_offset = 0;

        //�����ʶ��
        buf[510] = 0x55;
        buf[511] = 0xAA;

        sys_vm_write(buf, uspl_vm_addr);

        libc_memset(buf, 0, SECTOR_SIZE);
        for (temp = 0; temp < (USERPL_FILE_MAX / USERPL_PRE_SEC) + USERPL_HEAD_SEC; temp++)
        {
            uspl_vm_addr = uspl_vm_addr + SECTOR_SIZE;
            sys_vm_write(buf, uspl_vm_addr);
        }

        //����ͷ��Ϣ
        sys_vm_read(buf, VM_USERPL_INFO + sn * USERPL_VM_SIZE, SECTOR_SIZE);
    }

    fav_obj_open_mode = 0xee;//�����VM�ľ��

    libc_memcpy(&fav_opt_user_pl_head, upl_h, sizeof(userpl_head_t));

    return (VM_USERPL_INFO + sn * USERPL_VM_SIZE);

}

/******************************************************************************/
/*
 * \par  Description: ���ղؼ�
 *
 * \param[in]    file_name--�ղؼе��ļ���
 buf--ʹ�õĻ���buffer
 creat_flag--�����ղؼ��ļ���־
 * \param[out]

 * \return       �ļ���� ����0--�ɹ���0--ʧ��

 * \note         ����fav_opt_user_pl_head��Ϣ
 *******************************************************************************/
handle fav_opt_open_userpl(uint8 sn, uint8 *buf, uint8 creat_flag)
{
    handle t_handle;
    userpl_head_t *upl_h;
    uint32 temp;
    uint8 *file_name;

    if (fsel_init_val.disk == DISK_C)
    {
        return fav_opt_open_userpl_vram(sn, buf, creat_flag);
    }

    //card or host ��֧

    upl_h = (userpl_head_t*) buf;
    file_name = (uint8 *) userpl_name[sn];
    fav_obj_open_mode = OPEN_MODIFY;

    open_uspl:

    //�������·��
    vfs_file_dir_offset(vfs_mount, buf, buf + 128, 0);
    //Ĭ�Ͻ����Ŀ¼
    vfs_cd(vfs_mount, CD_ROOT, 0);

    mbyte_to_wchar(buf + 256, (uint8 *) file_name);
    t_handle = vfs_file_open(vfs_mount, buf + 256, OPEN_MODIFY);

    if (t_handle == 0)
    {
        if (0 == creat_flag)
        {
            vfs_file_dir_offset(vfs_mount, buf, buf + 128, 1);
            return t_handle;
        }

        t_handle = vfs_file_create(vfs_mount, buf + 256, 0);

        vfs_file_dir_offset(vfs_mount, buf, buf + 128, 1);

        if (0 == t_handle)
        {
            return t_handle;
        }

        libc_memset(buf, 0, SECTOR_SIZE);
        libc_memset(&fav_opt_user_pl_head, 0, sizeof(userpl_head_t));

        libc_memcpy(fav_opt_user_pl_head.file_name, file_name, 8);
        fav_opt_user_pl_head.file_num = 0;
        fav_opt_user_pl_head.first_offset = 0;
        fav_opt_user_pl_head.last_offset = 0;

        libc_memcpy(buf, &fav_opt_user_pl_head, sizeof(userpl_head_t));

        //��ӱ�ʶʶ��
        buf[510] = 0x55;
        buf[511] = 0xAA;

        //д��1������
        vfs_file_write(vfs_mount, buf, SECTOR_SIZE, t_handle);

        libc_memset(buf, 0, SECTOR_SIZE);

        for (temp = 0; temp < (USERPL_FILE_MAX / USERPL_PRE_SEC) + USERPL_HEAD_SEC; temp++)
        {
            vfs_file_write(vfs_mount, buf, SECTOR_SIZE, t_handle);
        }

        vfs_file_close(vfs_mount, t_handle);

        vfs_file_dir_offset(vfs_mount, buf, buf + 128, 0);
        //Ĭ�Ͻ����Ŀ¼
        vfs_cd(vfs_mount, CD_ROOT, 0);
        mbyte_to_wchar(buf + 256, (uint8 *) file_name);
        t_handle = vfs_file_open(vfs_mount, buf + 256, OPEN_MODIFY);

        vfs_file_dir_offset(vfs_mount, buf, buf + 128, 1);

        if (0 == t_handle)
        {
            return 0;
        }
    }
    else
    {
        //�ָ�·��
        vfs_file_dir_offset(vfs_mount, buf, buf + 128, 1);

        vfs_file_read(vfs_mount, buf, SECTOR_SIZE, t_handle);

        if ((buf[510] != 0x55) || (buf[511] != 0xAA))
        {
            vfs_file_close(vfs_mount, t_handle);
            mbyte_to_wchar(buf + 256, (uint8 *) file_name);
            vfs_file_dir_remove(vfs_mount, buf + 256, FS_FILE);
            t_handle = 0;
            goto open_uspl;
        }

        libc_memcpy(&fav_opt_user_pl_head, upl_h, sizeof(userpl_head_t));
    }

    return t_handle;

}

