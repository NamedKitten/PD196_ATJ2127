/*******************************************************************************
 *                              US212A
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     config_wav_head.c
 * \brief    config �ϵ�ָ�¼���ļ�
 * \author   zhangxs
 * \version  1.0
 * \date     2011/9/05
 *******************************************************************************/
#include  "ap_config.h"

/*wav head��д����buf*/
uint8 wav_buf[512] _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 *  ¼���ϵ�ָ�
 * \param[in]    none
 * \param[out]   none
 * \return       true �ɹ���false ʧ��
 * \ingroup      config
 * \note
 *******************************************************************************/
bool write_head(void)
{
    handle fhandle;
    uint32 ret_bytes;
    fsel_param_t init_param;//�ļ�ѡ������ʼ������
    int32 vfs_mount;
    uint32 file_len;
    int result;
    uint8 rtc_access_val;
    uint32 total_time, tmp_time;

    //��RTC_ACCESS�Ĵ��������Ѿ�¼���ķ�����
    rtc_access_val = (uint8) act_readl(RTC_ACCESS);
    file_len = g_writehead.file_bytes + rtc_access_val * 60* g_writehead .bitrate / 8;

    if (file_len == 0)
    {
        return FALSE;
    }

    /*װ��disk��fs����*/
    init_param.disk = g_writehead.disk_no;
    init_param.fsel_type = FSEL_TYPE_COMMONDIR;
    init_param.file_type_bit = ALLFILE_BITMAP; //��ʾ�����ļ���Ŀ¼
    init_param.fsel_mode = FSEL_MODE_NORMAL;

    //�ļ�ѡ������ʼ��
    vfs_mount = fsel_init(&init_param, 0);

    if (vfs_mount < 0)
    {
        return FALSE;
    }

    //��ȡpath��file��Ϣ
    if (!vfs_file_dir_offset(vfs_mount, &g_writehead.ptr_layer, &g_writehead.ptr_file_offset, 1))
    {
        return FALSE;
    }

    fhandle = vfs_file_open(vfs_mount, g_writehead.rec_filename, OPEN_RECOVER);
    if (fhandle == 0)
    {
        return FALSE;
    }

    //ͨ��write�������ļ�size
    vfs_file_write(vfs_mount, NULL, file_len, fhandle);

    vfs_file_close(vfs_mount, fhandle);

    if ((g_writehead.rec_format == 0) || (g_writehead.rec_format == 2)) //wav or act
    {
        //���´��ļ�����ʱ�ļ��ѱ��ָ�
        fhandle = vfs_file_open(vfs_mount, g_writehead.rec_filename, OPEN_MODIFY);
        if (fhandle == 0)
        {
            return FALSE;
        }

        //SEEK���ļ�ͷ
        result = vfs_file_seek(vfs_mount, 0, SEEK_SET, fhandle);
        if (result == FALSE)
        {
            return FALSE;
        }

        //��һPAGE����
        ret_bytes = vfs_file_read(vfs_mount, wav_buf, 512, fhandle);
        if (ret_bytes == 0)
        {
            return FALSE;
        }

        //��дwav�ļ�ͷ
        if (g_writehead.rec_format == 0)
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
        result = vfs_file_seek(vfs_mount, 0, SEEK_SET, fhandle);
        if (result == FALSE)
        {
            return FALSE;
        }

        ret_bytes = vfs_file_write(vfs_mount, wav_buf, 512, fhandle);
        if (ret_bytes == 0)
        {
            return FALSE;
        }

        vfs_file_close(vfs_mount, fhandle);

    }
    fsel_exit();
    return TRUE;
}
