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

const uint8 fw_file_name[] =
{
    0xff,0xfe,
    'U', 0x00,
    'P', 0x00,
    'G', 0x00,
    'R', 0x00,
    'A', 0x00,
    'D', 0x00,
    'E', 0x00,
    '.', 0x00,
    'H', 0x00,
    'E', 0x00,
    'X', 0x00,
    0x00,0x00
};

const uint8 fw_ascfile_name[] = {"UPGRADE HEX"};

const uint8 fw_file_newname[] =
{
    0xff,0xfe,
    'U', 0x00,
    'P', 0x00,
    'G', 0x00,
    'R', 0x00,
    'A', 0x00,
    'D', 0x00,
    'E', 0x00,
    '~', 0x00,
    '.', 0x00,
    'H', 0x00,
    'E', 0x00,
    'X', 0x00,
    0x00,0x00
};

const uint8 lfi_name[] = {"FWIMAGE FW "};
const uint8 nand_id_name[] = {"FLASH_IDBIN"};

//const uint8 sys_folder_name[] = "SYSTEM     ";
/******************************************************************************/
/*!
 * \par  Description:
 *	  �������ֽ�У���
 * \param[in]    buf   data buffer address
 * \param[in]    len   data length, 2byte uint
 * \param[out]   none
 * \return       the checksum
 * \retval
 * \note
 *******************************************************************************/
uint16 check_sum16(uint16 *buf, UINT32 len)
{
    uint32 index;
    uint16 sum = 0;

    for (index = 0; index < len; index++)
    {
        sum += buf[index];
    }

    return sum;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  �������ֽ�У���
 * \param[in]    buf   data buffer address
 * \param[in]    len   data length, 4byte uint
 * \param[out]   none
 * \return       the checksum
 * \retval
 * \note
 *******************************************************************************/
uint32 check_sum32(uint32 *buf, uint32 len)
{
    uint32 index;
    uint32 sum = 0;

    for (index = 0; index < len; index++)
    {
        sum += buf[index];
    }

    return sum;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ȡ�̼������ļ���д��̼���
 * \param[in]    file_handle           �ļ����
 * \return       upgrade_status_e
 * \retval       д�����ʱ����ֵ
 * \note
 *******************************************************************************/
upgrade_status_e _firmware_file_write(uint32 file_handle)
{
    bool ret_val;
    uint16 sector_num;
    LFIHead_t *lfi_head;
    uint16 check_sum;
    uint32 write_addr;

    uint16 sector_total = (uint16)((g_fw_length + 511) / 512);

    lfi_head = (LFIHead_t *) g_data_buffer;

    //׼������FW
    vfs_file_seek(g_file_sys_id, g_fw_offset, SEEK_SET, file_handle);

    //��ȡ������FWǰ16K����
    ret_val = _read_file(16 * 1024, file_handle);

    if (ret_val == FALSE)
    {
        //����ʧ��
        return FW_UPGRADE_FILE_ERR;
    }

    //��ȡUSB���к�
    sys_get_fw_info(lfi_head->UsbSetupInfo, FW_INFO_USB_ID_ADDR, FW_INFO_USB_ID_LEN);

    //��ȡ����̷���Ϣ
    sys_get_fw_info((uint32) & (lfi_head->CapInfo.vm_disk_cap), FW_INFO_CAPINFO_ADDR, FW_INFO_CAPINFO_LEN);

    //����У���
    check_sum = check_sum16((uint16*) g_data_buffer, (sizeof(LFIHead_t) - 2) / 2);

    //д��ͷ��У���
    lfi_head->Headchecksum = check_sum;

    write_addr = 0;

    //д��16K����
    ret_val = ud_sector_write(g_data_buffer, write_addr, 16 * 1024);

    if (ret_val == FALSE)
    {
        return FW_UPGRADE_WRITE_ERR;
    }
    else
    {
        write_addr += 16 * 1024;
    }

    sector_total -= 32;

    while (sector_total > 0)
    {
        if (sector_total > 32)
        {
            sector_num = 32;
        }
        else
        {
            sector_num = sector_total;
        }

        ret_val = _read_file(sector_num * 512, file_handle);
        if (ret_val == FALSE)
        {
            //����ʧ��
            return FW_UPGRADE_FILE_ERR;
        }

        ret_val = ud_sector_write(g_data_buffer, write_addr, sector_num << 9);
        if (ret_val == FALSE)
        {
            return FW_UPGRADE_WRITE_ERR;
        }

        write_addr += sector_num << 9;
        sector_total -= sector_num;
    }
    return FW_UPGRADE_NO_ERR;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  У��д��̼����������Ƿ���ȷ
 * \param[in]    none
 * \return       upgrade_status_e
 * \retval       У��������ֵ
 * \note
 *******************************************************************************/
upgrade_status_e _firmware_file_check(void)
{
    LFIHead_t *lfi_head;
    LFIHead_dir_t *lfi_dir;
    uint32 check_sum;
    uint32 index;
    uint8 *buf;
    uint16 sector_num;
    uint16 sector_total;
    uint32 addr;

    lfi_head = (LFIHead_t *) g_data_buffer;
    lfi_dir = (LFIHead_dir_t *) g_data_buffer;

    //����ǰ8K
    ud_sector_read(g_data_buffer, 0, 0x2000);

    //����ͷ��У���
    check_sum = check_sum16((uint16*) g_data_buffer, (sizeof(LFIHead_t) - 2) / 2);
    if (check_sum != lfi_head->Headchecksum)
    {
        return FW_UPGRADE_CHECK_ERR;
    }

    //SD���ļ�У��������8K�ռ�
    buf = (uint8 *) (g_data_buffer + 0x2000);

    _upgrade_refreash_progress(9);

    for (index = 0; index < 240; index++)
    {
        if (0 == lfi_dir->diritem[index].filename[0])
        {
            break;
        }

        sector_total = (uint16)(lfi_dir->diritem[index].length / 0x200);
        addr = (lfi_dir->diritem[index].offset << 9);
        check_sum = 0;

        while (sector_total != 0)
        {
            if (sector_total > 16)
            {
                sector_num = 16;
            }
            else
            {
                sector_num = sector_total;
            }

            if (TRUE != ud_sector_read(buf, addr, sector_num << 9))
            {
                return FW_UPGRADE_CHECK_ERR;
            }

            check_sum = check_sum + check_sum32((uint32*) buf, sector_num * 512 / 4);

            addr += (sector_num << 9);
            sector_total -= sector_num;
        }

        if (lfi_dir->diritem[index].checksum != check_sum)
        {
            return FW_UPGRADE_CHECK_ERR;
        }

    }

    return FW_UPGRADE_NO_ERR;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  �̼���������
 * \param[in]    none
 * \return       upgrade_status_e
 * \retval       �������ֵ
 * \note
 *******************************************************************************/
upgrade_status_e _upgrade_firmware(void)
{
    uint32 file_len;
    uint32 ret_val;
    AFI_DIR_t *lfi_dir;
    AFI_DIR_t *nandid_dir;
    //file handle
    handle file_handle;
    upgrade_status_e upgrade_result;

    vfs_cd(g_file_sys_id, CD_ROOT, 0);

    file_handle = vfs_file_open(g_file_sys_id, fw_file_name, R_NORMAL_SEEK);

    if (file_handle == 0)
    {
        file_handle = vfs_file_open(g_file_sys_id, fw_ascfile_name, R_NORMAL_SEEK);
        if (file_handle == 0)
        {
            return FW_UPGRADE_FILE_ERR;
        }
    }

    vfs_file_get_size(g_file_sys_id, &file_len, file_handle, 0);

    g_decrypt.FileLength = (long) file_len;

    while (1)
    {
        //��ȡ��һ����������
        ret_val = _read_file_translate(0, file_handle, &g_decrypt, (uint32) g_decrypt.InOutLen);
        if (ret_val == FALSE)
        {
            //����ʧ��
            return FW_UPGRADE_FILE_ERR;
        }

        //�������������ƫ��
        g_firmware_offset = (uint32)(DECRYPT_USR_BUF_LEN - g_decrypt.InOutLen);

        //��ȡLFI���ļ���ƫ��
        lfi_dir = check_key_offset_exist(g_decrypt.pInOutBuffer, (uint32) g_decrypt.InOutLen, lfi_name, 11, 0);
        nandid_dir = check_key_offset_exist(g_decrypt.pInOutBuffer, (uint32) g_decrypt.InOutLen, nand_id_name, 11, 0);

        if ((lfi_dir != NULL) && (nandid_dir != NULL))
        {
            g_fw_offset = lfi_dir->offset + g_firmware_offset;
            g_fw_length = lfi_dir->length;
            g_flashid_offset = nandid_dir->offset + g_firmware_offset;
            g_flashid_length = nandid_dir->length;
            break;
        }
    }

    //дbrec
    upgrade_result = _firmware_brec_write(g_flashid_offset, g_flashid_length, file_handle);
    if (upgrade_result != FW_UPGRADE_NO_ERR)
    {
        return upgrade_result;
    }

    //д�̼�
    upgrade_result = _firmware_file_write(file_handle);
    if (upgrade_result != FW_UPGRADE_NO_ERR)
    {
        return upgrade_result;
    }

    upgrade_result = _firmware_file_check();
    if (upgrade_result != FW_UPGRADE_NO_ERR)
    {
        return upgrade_result;
    }

    //���һ��д�������ˢ�£������������ˢ��
    _upgrade_refreash_progress(10);

    vfs_file_close(g_file_sys_id, file_handle);

    //�������̼��ļ�
    vfs_file_dir_remove(g_file_sys_id, fw_file_newname, FS_FILE);

    file_handle = vfs_file_open(g_file_sys_id, fw_file_name, OPEN_MODIFY);
    if (file_handle == 0)
    {
        file_handle = vfs_file_open(g_file_sys_id, fw_ascfile_name, OPEN_MODIFY);
        if (file_handle == 0)
        {
            return FW_UPGRADE_FILE_ERR;
        }
    }

    vfs_file_rename(g_file_sys_id, fw_file_newname, file_handle, 0);
    vfs_file_close(g_file_sys_id, file_handle);

    return upgrade_result;
}

