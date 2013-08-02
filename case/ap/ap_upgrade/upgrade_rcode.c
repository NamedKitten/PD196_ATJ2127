/*******************************************************************************
 *                              US212A
 *                            Module: UPGRADE_FIRMWARE
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-10-19 14:00:41           1.0              build this file
 *******************************************************************************/
/*!
 * \file     upgrade_firmware.c
 * \brief    �̼�����ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2012-10-19
 *******************************************************************************/
#include "ap_upgrade.h"
#include "audio_device.h"

const uint8 upgrade_percent_str[11][5] =
{ "0%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "100%" };

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ȡLFI�������ļ���ƫ��
 * \param[in]    buf   data buffer address
 * \param[in]    buffer_len   buffer length
 * \param[in]    key_word �ؼ����ַ���
 * \param[in]    key_len  �ؼ����ַ�������
 * \param[in]    offset  ƫ��
 * \return       AFI_DIR_t* Ŀ¼��ָ��
 * \retval
 * \note
 *******************************************************************************/
AFI_DIR_t *check_key_offset_exist(char *buffer, uint32 buffer_len, const uint8 *key_word, uint8 key_len, uint8 offset)
{
    uint32 str_len = 0;

    while (str_len < buffer_len)
    {
        if (libc_memcmp(&buffer[str_len + offset], key_word, (uint32) key_len) == 0)
        {
            return (AFI_DIR_t *) (&buffer[str_len]);
        }
        else
        {
            str_len += 32;
        }
    }

    return NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ȡ�ļ����ݲ�����
 * \param[in]    mode           0 ��ʼ�� 1��������
 * \param[in]    decrypt_rw     ���ݽṹ��
 * \param[in]    read_len       ��ȡ�����ݳ���
 * \param[in]    fp                     �ļ����
 * \return       the read len
 * \note
 *******************************************************************************/
uint32 _read_file_translate(uint8 mode, uint32 fp, PCFWDecrypt_struct *decrypt_rw, uint32 read_len)
{
    uint32 ret_val = 0;

    //��ȡ�ļ�
    read_len = vfs_file_read(g_file_sys_id, decrypt_rw->pInOutBuffer, read_len, fp);

    if (mode == 0)
    {
        ret_val = (uint32) fw_decrypt_init(decrypt_rw);
    }
    else
    {
        fw_decrypt_run(decrypt_rw->pInOutBuffer, read_len, decrypt_rw->pGLBuffer);
    }

    if (ret_val != 0)
    {
        return 0;
    }
    else
    {
        return read_len;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ȡ�ļ����ݲ�����
 * \param[in]    read_len       ��ȡ�����ݳ���
 * \param[in]    fp             �ļ����
 * \return       the result
 * \note
 *******************************************************************************/
bool _read_file(uint32 read_len, uint32 fp)
{
    uint8 i, read_cnt;
    uint32 read_bytes;
    uint8 *data_buffer = g_data_buffer;

    read_cnt = (uint8)((read_len + 2047) / 2048);

    upgrade_get_app_msg();

    for (i = 0; i < read_cnt; i++)
    {
        if (read_len >= 2048)
        {
            read_bytes = 2048;
        }
        else
        {
            read_bytes = read_len;
        }

        read_bytes = _read_file_translate(1, fp, &g_decrypt, read_bytes);
        if (read_bytes == 0)
        {
            return FALSE;
        }

        read_len -= read_bytes;

        libc_memcpy(data_buffer, g_decrypt.pInOutBuffer, read_bytes);

        data_buffer += read_bytes;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��װ�ײ�д�߼������ӿ�
 * \param[in]    pbuf           д������buffer
 * \param[in]    addr           д���߼�����ַ
 * \param[in]    len            д�볤��
 * \return       the result
 * \retval       TRUE succeed  FALSE fail
 * \note
 *******************************************************************************/
bool ud_sector_write(void *pbuf, uint32 addr, uint32 len)
{
    int32 ret_val;
    ud_sector_param_t ud_sec_param;

    if ((addr & 0x1ff) != 0)
    {
        //��512�ֽڶ���
        return FALSE;
    }

    ud_sec_param.file_offset = (addr >> 9);
    ud_sec_param.sec_num = (len + 0x1ff) >> 9;
    ud_sec_param.sram_addr = (uint32) pbuf;

    ret_val = (int32) base_fw_write(ud_sec_param.file_offset, ud_sec_param.sram_addr, ud_sec_param.sec_num);
    if (ret_val == -1)
    {
        return FALSE;
    }
    else
    {
        g_write_sector += ud_sec_param.sec_num;
        return TRUE;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  ��װ�ײ���߼������ӿ�
 * \param[in]    pbuf           ��������buffer
 * \param[in]    addr           �����߼�����ַ
 * \param[in]    len            ���볤��
 * \return       the result
 * \retval       0 succeed  -1 fail
 * \note
 *******************************************************************************/
bool ud_sector_read(void *pbuf, uint32 addr, uint32 len)
{
    int32 ret_val;
    ud_sector_param_t ud_sec_param;

    if ((addr & 0x1ff) != 0)
    {
        //��512�ֽڶ���
        return FALSE;
    }

    ud_sec_param.file_offset = addr >> 9;
    ud_sec_param.sec_num = (len + 0x1ff) >> 9;
    ud_sec_param.sram_addr = (uint32) pbuf;

    ret_val = (int32) base_fw_read(ud_sec_param.file_offset, ud_sec_param.sram_addr, ud_sec_param.sec_num);
    if (ret_val == -1)
    {
        return FALSE;
    }
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��װ�ײ�brec�߼������ӿ�
 * \param[in]    pbuf           ��������buffer
 * \param[in]    addr           �����߼�����ַ
 * \param[in]    len            ���볤��
 * \return       the result
 * \retval       0 succeed  -1 fail
 * \note
 *******************************************************************************/
bool brec_sector_write(void *pbuf, uint32 addr, uint32 len)
{
    int32 ret_val;
    ud_sector_param_t ud_sec_param;

    if ((addr & 0x1ff) != 0)
    {
        //��512�ֽڶ���
        return FALSE;
    }

    ud_sec_param.file_offset = addr >> 0x9;
    ud_sec_param.sec_num = (len + 0x1ff) >> 9;
    ud_sec_param.sram_addr = (uint32) pbuf;

    ret_val = (int32) base_brec_write(ud_sec_param.file_offset, ud_sec_param.sram_addr, ud_sec_param.sec_num);
    if (ret_val == -1)
    {
        return FALSE;
    }
    else
    {
        g_write_sector += ud_sec_param.sec_num;
        return TRUE;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ���¹̼����������е���ʾ
 * \param[in]    value     ������ʾ��ֵ
 * \return       none
 * \note
 *******************************************************************************/
void _upgrade_refreash_progress(uint8 value)
{
    style_infor_t upgrade_sty;
    progressbar_private_t progressbar_param;
    textbox_private_t textbox_param;

    upgrade_sty.type = UI_AP;

    // ��ʾ�ٷֱ�
    upgrade_sty.style_id = UPGRADE_PERCENT;
    textbox_param.lang_id = ANSIDATAAUTO;
    textbox_param.str_value = (uint8 *) &upgrade_percent_str[value];
    ui_show_textbox(&upgrade_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

    // ��ʾ������
    upgrade_sty.style_id = UPGRADE_PROGRESS;
    progressbar_param.total = 120;
    progressbar_param.value = value * 12;
    ui_show_progressbar(&upgrade_sty, &progressbar_param, PROGRESS_DRAW_PROGRESS);
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ʱ����ʾ���������ڸ��¹̼����������е���ʾ
 * \param[in]    none
 * \return       none
 * \note
 *******************************************************************************/
void _upgrade_proc_timer(void)
{
    uint8 old_precent_count;
    uint32 sector_total;

    old_precent_count = g_precent_count;

    sector_total = (uint32)((g_fw_length + g_brec_length) >> 9);

    if (sector_total == 0)
    {
        return;
    }

    g_precent_count = (uint8)((9 * g_write_sector) / sector_total);

    if (g_precent_count == 0)
    {
        //��ʼ����ȡ�׶�����Ϊ10%
        g_precent_count = 1;
    }

    if (old_precent_count != g_precent_count)
    {
        _upgrade_refreash_progress(g_precent_count);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	 �̼����������л�ȡ��Ϣ�ӿ�
 * \param[in]    none
 * \return       app_result_e �ᱻ����
 * \note
 *******************************************************************************/
app_result_e upgrade_get_app_msg(void)
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
    if ((cpu_release_count % 5) == 0)
    {
        //��ʱ�ͷ�cpu
        sys_os_time_dly(1);
    }

    return result;
}

