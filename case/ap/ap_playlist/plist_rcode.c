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
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

const uint8 m3u_ext_buf[4] = "M3U";

/******************************************************************************/
/*
 * \par  Description: д���ļ���������֤��������
 *
 * \param[in]     data_buf--д�����ݵĵ�ַ
 len--���ݳ���(�ֽ�)

 * \param[out]

 * \return        TRUE OR FALSE

 * \note   ��¼��ǰ�ļ�λ��
 *******************************************************************************/
//д���ļ���������֤�������룬��¼�ļ�λ��
bool save_to_file(uint8 *data_buf, uint32 len)
{
    uint16 sectors;
    uint16 i;

    sectors = (uint16) ((len - 1) / SECTOR_SIZE) + 1;

#if 0
    //������д��
    if(0==vfs_file_write(ap_vfs_mount,data_buf,SECTOR_SIZE*sectors,ap_obj_handle))
    {
        return FALSE;
    }
#else
    // ������д��
    for (i = 0; i < sectors; i++)
    {
        if (0 == vfs_file_write(ap_vfs_mount, data_buf, SECTOR_SIZE, ap_obj_handle))
        {
            return FALSE;
        }
        data_buf += SECTOR_SIZE;
    }

#endif
    ap_pos_file_offset += sectors;
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ��VRAM��ȡ����������
 *
 * \param[in]    dest--VM�ĵ�ַ
 source--���ݵĵ�ַ
 len--�������ֽ���
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool read_from_vram(uint8 *dest, uint8 *source, uint32 len)
{
    len = (len - 1) / SECTOR_SIZE + 1;

#if 0
    //vram�������ƣ�ÿ��ֻ��512
    while (len != 0)
    {
        sys_vm_read(dest, source, SECTOR_SIZE);
        source += SECTOR_SIZE;
        dest += SECTOR_SIZE;
        len--;
    }

#else
    //��������
    sys_vm_read(dest, source, len * SECTOR_SIZE);

#endif
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: д�����������ݵ�VRAM
 *
 * \param[in]    dest--VM�ĵ�ַ
 source--���ݵĵ�ַ
 byte--д���ֽڸ���
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool write_to_vram(uint8 *dest, uint8 *source, uint32 len)
{
    len = (len - 1) / SECTOR_SIZE + 1;

#if 0
    //vramд�����ƣ�ÿ��ֻд512
    while (len != 0)
    {
        sys_vm_write(source, dest);
        source += SECTOR_SIZE;
        dest += SECTOR_SIZE;
        len--;
    }

#else

    //������д
    sys_vm_write_multi(source, dest, len * SECTOR_SIZE);

#endif

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ����debug
 *
 * \param[in]    count--������
 debug_value--����ָ��ֵdebug
 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void debug_count_flag(uint32 count, uint32 debug_value)
{
    if (count == debug_value)
    {
        debug_value = 0;
    }

    return;
}

/*
 void handle_timers()
 {

 }
 */

//���ˣ�gui��Ϣ�����Ѿ��������
//��ȡap˽����Ϣ��ϵͳ��Ϣ������ת��Ϊ˽����Ϣ�ٷ��أ���������

app_result_e plist_get_app_msg(void)
{
    private_msg_t private_msg;
    input_gui_msg_t gui_msg;
    app_result_e result = RESULT_NULL;

    if (get_gui_msg(&gui_msg) == TRUE)
    {
        if (INPUT_MSG_KEY == gui_msg.type)
        {
            if ((gui_msg.data.kmsg.val == KEY_LOCK) || (gui_msg.data.kmsg.val == KEY_UNLOCK))
            {
                gui_keylock(TRUE);
            }
        }
    }
    g_sys_counter.battery_counter = 100;//ÿ�ν��붼����͵�
    if (get_app_msg(&private_msg) == TRUE)
    {
        if (private_msg.msg.type == MSG_EARPHONE_IN)
        {
            /*���������ߣ�����*/
            change_antenna_state(TRUE);
        }
        else if (private_msg.msg.type == MSG_EARPHONE_OUT)
        {
            /*���������ߣ��γ�*/
            change_antenna_state(FALSE);
        }
        else
        {
        }
    }

    //����10ms�����������
    cpu_release_count++;
    if ((cpu_release_count % 5) == 00)
    {
        //��ʱ�ͷ�cpu
        sys_os_time_dly(1);
    }

    return result;
}
