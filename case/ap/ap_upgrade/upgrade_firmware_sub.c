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

/******************************************************************************/
/*!
 * \par  Description:
 *	  ����flash ����
 * \param[in]    argBuff nand id
 * \param[in]    nandid_offset nandid��ƫ��
 * \param[in]    nandid_size   nandid���С
 * \param[in]    fp  ���
 * \return       uint32
 * \retval       flash����
 * \note
 *******************************************************************************/
uint32 _firmware_search_id(uint8 *argBuff, uint32 nandid_offset, uint32 nandid_size, uint32 fp)
{
    uint32 file_offset;

    uint32 i, flash_type = 0xffff;
    uint32 item_size, item_cnt, item_offset, search_flag;
    uint8 naidBuff[8];

    uint8 flash_id_item[128];

    item_size = 128;
    item_cnt = nandid_size;
    item_cnt = (item_cnt - 512) / item_size; /* first sector is table head */
    search_flag = 1;
    file_offset = 0;

    vfs_file_seek(g_file_sys_id, nandid_offset, SEEK_SET, fp);

    while (item_cnt != 0)
    {
        if ((file_offset % 0x4000) == 0)
        {
            _read_file(16 * 1024, fp);
            if (file_offset == 0)
            {
                item_offset = 512; //skip table head
            }
            else
            {
                item_offset = 0;
            }
        }

        libc_memcpy(flash_id_item, g_data_buffer + item_offset, item_size);
        libc_memcpy(naidBuff, flash_id_item, 8);

        i = 0;
        while (i < 8)
        {
            if (naidBuff[i] != 0xff)
            {
                if (argBuff[i] == naidBuff[i])
                {
                    search_flag = 0;
                }
                else
                {
                    search_flag = 1;
                    break;
                }
            }
            i = i + 1;
        }

        if (search_flag == 0)
        {
            break;
        }

        item_cnt--;
        item_offset += item_size;
        file_offset += item_size;
    }

    if ((search_flag == 0) && ((flash_id_item[22] & 0x08) != 0))
    {
        //nand flash need read retry
        if ((flash_id_item[0] == 0xAD) && ((flash_id_item[5] & 0x07) == 0x3))
        {
            //hynix 26nm MLC flash
            flash_type = 0xF645;
        }
        else if (flash_id_item[0] == 0x98)
        {
            //Toshiba 24nm MLC flash
            flash_type = 0xF646;
        }
        else if ((flash_id_item[0] == 0xAD) && ((flash_id_item[5] & 0x07) == 0x4))
        {
            //hynix 20nm MLC flash
            flash_type = 0xF647;
        }
        else if ((flash_id_item[0] == 0xEC) && ((flash_id_item[5] & 0x07) == 0x4))
        {
            //Samsung 21nm MLC flash
            flash_type = 0xF648;
        }
        else if ((flash_id_item[0] == 0x2c) && (flash_id_item[1] == 0x64) && (flash_id_item[2] == 0x44))
        {
            //Micron 20nm MLC flash
            flash_type = 0xF649;
        }
        else if ((flash_id_item[0] == 0x45) && (flash_id_item[5] == 0x57))
        {
            //Sandisk 19nm MLC flash
            flash_type = 0xF650;
        }
        else
        {
            ;//nothing for QAC
        }
    }
    else if (search_flag == 0)
    {
        flash_type = 0xF644;
    }
    else
    {
        ;//nothing for QAC
    }

    if (flash_type != 0xffff)
    {
        libc_memcpy(g_flash_id, flash_id_item, 64);
    }

    return flash_type;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ����brecλ��
 * \param[in]    flash_type flash����
 * \param[in]    cmp_len ƥ���ַ�������
 * \return       AFI_DIR_t ��Ӧ��brecĿ¼��ָ��
 * \note
 *******************************************************************************/
AFI_DIR_t *_firmware_search_brec(uint32 flash_type, uint32 cmp_len)
{
    uint32 i = 0;
    AFI_DIR_t *afi_dir = NULL;

    while (i < cmp_len)
    {
        afi_dir = check_key_offset_exist(g_data_buffer + i, cmp_len, (const uint8 *) (&flash_type), 4, 24);
        if (afi_dir != NULL)
        {
            if (libc_memcmp(afi_dir->filename, "BREC", 4) == 0)
            {
                break;
            }
            else
            {
                ;//����ƥ��
            }
        }
        else
        {
            break;
        }

        i = (uint32)((uint8 *) afi_dir - g_data_buffer + 32);
    }

    return afi_dir;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ����flash ����
 * \param[in]    nandid_offset nandid��ƫ��
 * \param[in]    nandid_size   nandid���С
 * \param[in]    file_handle  ���
 * \return       upgrade_status_e
 * \retval       ����״̬
 * \note
 *******************************************************************************/
upgrade_status_e _firmware_brec_write(uint32 nandid_offset, uint32 nandid_size, uint32 file_handle)
{
    bool ret_val;
    AFI_DIR_t *afi_dir;
    uint8 *nand_ptr;
    uint8 nand_id[8];
    uint32 flash_type;
    uint32 brec_offset;
    uint32 sector_total;
    uint32 sector_num;
    uint32 write_addr = 0;

    nand_ptr = (uint8 *) base_get_flash_id();

    libc_memcpy(nand_id, nand_ptr, 8);

    flash_type = _firmware_search_id(nand_id, nandid_offset, nandid_size, file_handle);

    if (flash_type == 0xffff)
    {
        return FW_UPGRADE_CHECK_ERR;
    }

    vfs_file_seek(g_file_sys_id, 0, SEEK_SET, file_handle);

    //ֻ��ǰ2K������
    _read_file(2048, file_handle);

    afi_dir = _firmware_search_brec(flash_type, 2048);

    if (afi_dir != NULL)
    {
        brec_offset = afi_dir->offset + g_firmware_offset;

        g_brec_length = afi_dir->length;
    }
    else
    {
        return FW_UPGRADE_CHECK_ERR;
    }

    vfs_file_seek(g_file_sys_id, brec_offset, SEEK_SET, file_handle);

    //��ȡ16k��brec����
    ret_val = _read_file(16 * 1024, file_handle);

    *(uint32 *) (g_data_buffer + 8) = (uint32)((g_fw_length + 511) / 512);
    libc_memcpy((g_data_buffer + 128), g_flash_id, 64);

    sector_total = (uint32)((g_brec_length + 511) / 512);

    ret_val = brec_sector_write(g_data_buffer, write_addr, 16 * 1024);

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

        ret_val = brec_sector_write(g_data_buffer, write_addr, sector_num << 9);
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
 *	  �����л���ǰbank�����ڴ�
 * \param[in]    none
 * \return       none
 * \note
 *******************************************************************************/
uint32 _firmware_load_bank(void)
{
    return TRUE;
}

