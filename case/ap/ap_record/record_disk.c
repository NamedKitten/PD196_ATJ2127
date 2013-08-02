/*
 *******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_file.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *              leiming    2011-10-27          v1.0                 create this file
 *******************************************************************************
 */
/*include file of this application, ��Ӧ�õ�ͷ�ļ�*/
#include "ap_record.h"

uint8 wav_buf[512] _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _record_fs_init(void)
 * \װ�ش����������ļ�ϵͳ
 * \param[in]    void
 * \param[out]   none
 * \return       int result
 * \retval       TRUE/FALSE
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
bool _record_fs_init(uint8 diskno)
{
    bool ret_result = TRUE;
    fsel_param_t init_param;//�ļ�ѡ������ʼ������

    /*װ���ļ�ϵͳ*/
    //if (g_fs_param.vfs_handle < 0)
    if (need_fsel_init == TRUE)
    {
        init_param.disk = diskno;
        init_param.fsel_type = FSEL_TYPE_COMMONDIR;
        init_param.file_type_bit = RECORD_BITMAP; //ֻ����mp3��wav
        init_param.fsel_mode = FSEL_MODE_NORMAL;
        init_param.brow_type = FSEL_BROW_FILE;

        //�ļ�ѡ������ʼ��
        if (DISK_C == init_param.disk)
        {
            g_record_vars.path.file_path.dirlocation.disk = DISK_C;
        }
        else if (DISK_H == init_param.disk)
        {
            if (sys_detect_disk(DRV_GROUP_STG_CARD) != 0) //�޿�
            {
                gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
                g_record_vars.path.file_path.dirlocation.disk = DISK_C;
                init_param.disk = DISK_C;
            }
            else
            {
                g_record_vars.path.file_path.dirlocation.disk = DISK_H;
            }
        }
        else
        {
            ; //QAC need
        }

        g_fs_param.vfs_handle = fsel_init(&init_param, 0);

        if (g_fs_param.vfs_handle < 0)
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
            ret_result = FALSE;
            goto func_exit;
        }
        need_fsel_init = FALSE;
    }

    /*������ʣ������*/
    if (!vfs_get_space((void*) g_fs_param.vfs_handle, (void*) &g_fs_param.free_space, GET_SPARE_SPACE))
    {
        ret_result = FALSE;
        goto func_exit;
    }

    if (g_fs_param.free_space <= REC_SPACE_LOW)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, STR_DISK_FULL);
        ret_result = FALSE;
    }
    else
    {
        //��־�����Ѹ�ʽ��
        //g_record_vars.path.file_path.dirlocation.disk = diskno;
        //g_fs_param.disk_flag |= (uint8) SETDISKOKFLAG_DEF;
    }
    func_exit: return ret_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _record_write_back_filehead(void )
 * \wav¼��֮���дwav�ļ�ͷ
 * \param[in]    void
 * \param[out]   none
 * \return       int result
 * \retval       TRUE/FALSE
 * \ingroup      record_disk.c
 * \note         wav�ļ�����Ҫ�˲�����MP3�ļ�����Ҫ��д
 */
/*******************************************************************************/
bool _record_write_back_filehead(void)
{
    bool result;
    uint32 file_len;
    handle fhandle;
    uint32 ret_bytes;
    uint32 total_time, tmp_time;

    //���´�¼���ļ�
    fhandle = vfs_file_open(g_fs_param.vfs_handle, g_fs_param.rec_filename, R_NORMAL_SEEK);
    if (fhandle == 0)
    {
        return FALSE;
    }

    //��ȡ�ļ�����
    result = vfs_file_get_size(g_fs_param.vfs_handle, &file_len, fhandle, 0);
    if (result == FALSE)
    {
        return FALSE;
    }

    //SEEK���ļ�ͷ
    result = vfs_file_seek(g_fs_param.vfs_handle, 0, SEEK_SET, fhandle);
    if (result == FALSE)
    {
        return FALSE;
    }

    //��һPAGE����
    ret_bytes = vfs_file_read(g_fs_param.vfs_handle, wav_buf, 512, fhandle);
    if (ret_bytes == 0)
    {
        return FALSE;
    }

    //��дwav�ļ�ͷ
    if (g_record_vars.file_type == REC_FORMAT_WAV)
    {
        file_len = file_len - 8;
        libc_memcpy(&wav_buf[0x04], &file_len, 4);
        file_len = file_len + 8 - 512;
        libc_memcpy(&wav_buf[0x1fc], &file_len, 4);
    }
    else
    {
        //�����ļ����ȼ��㵱ǰ����ʱ��,ms��λ
        total_time = (((file_len - 512) / 512) * 510 + ((file_len - 512) % 512));
        tmp_time = total_time / 1000;
        wav_buf[259] = (uint8)(tmp_time % 60); //��
        tmp_time = tmp_time / 60;
        wav_buf[260] = (uint8)(tmp_time % 60); //��
        wav_buf[261] = (uint8)(tmp_time / 60); //ʱ
    }

    //SEEK���ļ�ͷ
    result = vfs_file_seek(g_fs_param.vfs_handle, 0, SEEK_SET, fhandle);
    if (result == FALSE)
    {
        return FALSE;
    }

    ret_bytes = vfs_file_write(g_fs_param.vfs_handle, wav_buf, 512, fhandle);
    if (ret_bytes == 0)
    {
        return FALSE;
    }

    vfs_file_close(g_fs_param.vfs_handle, g_fs_param.fhandle);

    return TRUE;
}
