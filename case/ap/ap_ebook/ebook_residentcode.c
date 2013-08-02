/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_pageoperate.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/

#include  "ebook.h"

/********************************************************************************
 * Description :��ȡ��ǰҳ����ƫ��
 *
 * Arguments  :
 *               ��
 * Notes       :
 *
 ********************************************************************************/
void _get_page_offset(uint32 pagenum)
{
    uint32 cur_sector, cur_sector_offset;
    uint32 temp_index;
    curpagenum = pagenum;
    if (curpagenum == 0)
    {
        file_offset = 0;
        return;
    }
    cur_sector = (curpagenum - 1) >> 7;                //  curpagenum/128;
    cur_sector_offset = (curpagenum - 1) & 0x7F;
    if (page_count_flag == FALSE)
    {
        //ҳ�������Ѿ����,�����ǰҳ������ǩ�ļ��е������뻺���������ͬ
        //���ȡ����buf�е����ݣ��������¶�ȡһ������������
        if (cur_sector == (uint32) cur_BufSector)//��ʾ��ǰbuf�е���������������,-1��ʾ��ǰbuf�е����ݲ����á�
        {
            libc_memcpy(&file_offset, &bmk_bufFile[cur_sector_offset * 4], 0x04);
        }
        else
        {
            //��ȡ��ǩ��Ϣ
            _seek_and_read((cur_sector + 1) * FILE_SECTOR, FILE_SECTOR, bmk_bufFile);   

            //��ȡ��ǩ���ҳ��ƫ������ֵ�� file_offset
            libc_memcpy(&file_offset, &bmk_bufFile[cur_sector_offset * 4], 0x04);  
            
            cur_BufSector = (int32) cur_sector;
        }
    }
    else
    {
        //����ҳ����Ϣʱ�������ǰҳ�����ڻ�д����ǩ�ļ��е����һ��ҳ��
        //��ӵ�ǰ�����buf�ж�ȡ���ݣ�����Ļ�����ǩ�ļ���Ӧλ�ö�ȡ����
        if (curpagenum >= bmk_last_pagenum)
        {
            if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
            {
                temp_index = ((curpagenum - 1) % 128) * 4;
            }
            else
            {
                temp_index = (curpagenum - bmk_last_pagenum) * 4;
            }
            libc_memcpy(&file_offset, &bmk_bufFile[temp_index], 0x04);
        }
        else
        {
            _seek_and_read(FILE_SECTOR + (curpagenum - 1) * 4, 0x04, (uint8*) &file_offset);
        }
        cur_BufSector = -1;
    }
}


/********************************************************************************
 * Description :����������ҳ��
 *
 * Arguments  :
 *               ��
 * Notes       :
 *
 ********************************************************************************/

void _count_page_num(void)
{
    uint32 text_offset;
    uint32 last_offset, tmp_offset;
    //uint32 cur_sector, cur_sector_offset;
    if (page_count_flag == FALSE)
    {
        return;
    }
    draw_page_num = TRUE;
    last_offset = page_count_file.file_offset = decode_offset;
    decode_bmk_info.decode_offset = decode_offset;
    _decode_one_page(&page_count_decode, &page_count_file, &text_offset);
    //������ص���0xFFFFFFFF�������һ��ƫ�Ʋ��û�д����ǩ�ļ���
    if (text_offset == V_U32_INVALID)
    {
        decode_offset = V_U32_INVALID;
    }
    else
    {
        decode_offset += text_offset;
        temp_count++;
        page_totalnum++;
    }
    if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
    {
        _vram_write_bmkinfo();
        return;
    }
    //��������ļ�β����ҳ����Ϣ�ͽ�����PAGE_MAGIC��д���ļ���
    if (decode_offset >= page_count_file.file_length)
    {
        _seek_and_write(FILE_SECTOR + (page_totalnum - temp_count) * 4, temp_count * 4, bmk_bufFile);
        _seek_and_read(0, FILE_SECTOR, bmk_bufFile);
        tmp_offset = PAGE_MAGIC;
        libc_memcpy(&bmk_bufFile[FILE_SECTOR - 16], &tmp_offset, 0x04);
        libc_memcpy(&bmk_bufFile[FILE_SECTOR - 12], &page_totalnum, 0x04);
        libc_memcpy(&bmk_bufFile[FILE_SECTOR - 8], &last_offset, 0x04);
        _seek_and_write(0, FILE_SECTOR, bmk_bufFile);
        page_count_flag = FALSE;
        cur_BufSector = -1;
        return;
    }

    if ((temp_count < 129) && (temp_count > 0))
    {
        libc_memcpy(&bmk_bufFile[(temp_count - 1) * 4], &decode_offset, 0x04);
    }
    //��ҳ����Ϣ��һ������ʱ�������ݻ�д��ǩ�ļ�
    if ((page_totalnum % 128) == 0)
    {
        bmk_last_pagenum = page_totalnum;
        _seek_and_write(FILE_SECTOR + (page_totalnum - temp_count) * 4, temp_count * 4, bmk_bufFile);
        temp_count = 0;
        libc_memset(bmk_bufFile, 0x0, FILESIZE);
    }
}

/********************************************************************************
 * Description :��λ��ĳ��λ�ã�Ȼ���ڸ�λ�ö�ȡһ����С������
 * Arguments  :
 *              seek_offset:��Ҫseek����λ��
 *              read_size:��Ҫ��ȡ�����ݴ�С
 *              databuf:�洢���ݵ�buf
 * Returns     :
 *                TRUE or FALSE
 * Notes       :
 *
 ********************************************************************************/

bool _seek_and_read(uint32 seek_offset, uint32 read_size, uint8 *databuf)
{
    uint32 offset_sector;
    if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
    {
        offset_sector = vram_bmk_info.startadd + seek_offset;
        sys_vm_read(databuf, VM_BMK_INFO + offset_sector, read_size);
        return 1;
    }
    vfs_file_seek(g_ebook_mount_id, seek_offset, SEEK_SET, bmk_modify_handle);
    if (vfs_file_read(g_ebook_mount_id, databuf, read_size, bmk_modify_handle) == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
/********************************************************************************
 * Description :��λ��ĳ��λ�ã�Ȼ������λ��дһ����С������
 * Arguments  :
 *              seek_offset:��Ҫseek����λ��
 *              write_size:��Ҫд�����ݴ�С
 *              databuf:�洢���ݵ�buf
 * Returns     :
 *                TRUE or FALSE
 * Notes       :
 *
 ********************************************************************************/
bool _seek_and_write(uint32 seek_offset, uint32 write_size, uint8 *databuf)
{
    vfs_file_seek(g_ebook_mount_id, seek_offset, SEEK_SET, bmk_modify_handle);
    return ((int) vfs_file_write(g_ebook_mount_id, databuf, write_size, bmk_modify_handle) == 0);
}
/********************************************************************************
 * Description :�������Ķ�������Ϣ������
 *
 * Arguments  :
 *            sta:
 *              enter_mode:
 *               ��
 * Notes       :
 *
 ********************************************************************************/
app_result_e _deal_key_msg(dir_control_t *mlst_ctl, inlet_type_e inlet_type)
{
    app_result_e result = RESULT_NULL;
    input_gui_msg_t ui_msg; //gui ��Ϣ
    private_msg_t private_msg; //˽����Ϣ
    //��ȡgui��Ϣ
    //��ui��Ϣ
    if (get_gui_msg(&ui_msg) == TRUE)
    {
        if (ui_msg.type == INPUT_MSG_KEY)
        {
            result = _deal_msg_result(&ui_msg, mlst_ctl, inlet_type);
        }
    }
    else
    { /*����ap˽����Ϣ*/
        if (get_app_msg(&private_msg) == TRUE)
        {
            result = _ebook_scene_msg_dispatch(&private_msg);

            if (result == RESULT_REDRAW)
            {
                need_draw = TRUE;
                numboxflag = TRUE;
                if (inlet_type == NORMAL_TYPE)
                {
                    _get_page_offset(curpagenum);
                    view_decode.valid = FALSE;
                    draw_auto_flag = TRUE;   
                }
                result = RESULT_NULL;
            }
        }
        else
        {
            result = RESULT_NULL;
        }
    }
    return result;
}

//��Ϣ�ַ�����
app_result_e _ebook_scene_msg_dispatch(private_msg_t *private_msg)
{
    app_result_e result = RESULT_NULL;
    //msg_reply_t m_reply;
    /*ͬ����Ϣ����*/
    switch (private_msg->msg.type)
    {
        case MSG_SD_OUT:
        if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_H)
        {
            _show_single_dialog(S_CARD_OUT, DIALOG_INFOR_WAIT);
            _close_text_file(0);
            libc_memset(&g_ebook_vars.path.file_path.dirlocation, 0x0, sizeof(file_location_t));
            g_ebook_vars.path.file_path.dirlocation.disk = DISK_C;
            device_status &= 0x02;
            _close_engine();
            fsel_exit();
            _ebook_filesel_init();
            if ((device_status & 0x03) == 0x03)
            {
                result = RESULT_MAIN_APP;
            }
            else
            {
                enter_mode = ENTER_FROM_DESKTOP;
                result = RESULT_EBK_FILELIST;
            }
        }
        break;

        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;
        default:
        result = com_message_box(private_msg->msg.type);
        break;
    }
    return result;
}

