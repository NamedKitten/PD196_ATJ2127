/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_fileoperate.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/

#include  "ebook.h"

/********************************************************************************
 * Description :�ļ���Ϣ��ʼ������Ҫ�����ļ�location��Ϣ��ȡ���ļ���
 *              ��ȡ�ļ���С������������Ϣ
 * Arguments  :
 *              enter_mode:��ǰ��������(����ebook.h�е�ebook_scene_e)
 * Returns     :
 *                  ��������ֵ

 * Notes       :
 *
 ********************************************************************************/

uint16 _init_file_info(void)
{
    uint32          unicode_flag;
//    int8            language;
    
    if (fsel_set_location(&g_ebook_vars.path.file_path.dirlocation, FSEL_TYPE_LISTEBOOK) == FALSE)//����ʧ��
    {
        if( fsel_get_byno(  g_ebook_vars.path.file_path.dirlocation.filename,
                            g_ebook_vars.path.file_path.dirlocation.file_num) == FALSE) //���»�ȡ��һ���ļ�
        {
            //��ȡ��ǰģʽ�µĵ�һ���ɲ����ļ�
            if (fsel_get_nextfile(g_ebook_vars.path.file_path.dirlocation.filename) == FALSE)
            {
                //�Ҳ����ɲ����ļ�
                return S_FILE_NO_FILE;
            }        
        }

        //��ȡ��ǰ�ļ���location��Ϣ
        fsel_get_location(&g_ebook_vars.path.file_path.dirlocation, FSEL_TYPE_LISTEBOOK);
    }
    
    view_file.file_handle = vfs_file_open(g_ebook_mount_id, (void*) (0), R_NORMAL_SEEK);
    if (view_file.file_handle == (uint32) NULL)
    {
        //       _show_single_dialog(S_FILE_OPEN_ERROR);
        return S_FILE_OPEN_ERROR;
    }

    //��ȡ�ļ�ͷ4���ֽ��б���ļ������������뻹��unicode
    vfs_file_read(g_ebook_mount_id, &unicode_flag, 4, view_file.file_handle);
    
    //�����ļ����ܳ��Ⱥ���������
    view_file.file_length = _get_file_length(view_file.file_handle);
    if (view_file.file_length == 0)
    {
        return S_FORMAT_ERROR;
    }
    _vram_limit_file_length();
    
    view_file.file_sectors = (view_file.file_length + (512 -1) ) / 512 ;  // + (512 -1) ��Ϊ�˽�λ


    view_file.fseek = _seek_file_sector;
    view_file.fread = _read_file_sector;
    view_file.file_offset = file_offset = 0;
    
    //��λ���ļ�ͷ
    _seek_file_sector(0, view_file.file_handle);
    
    if ((unicode_flag & 0xFFFF) == UNICODE_FLAG)
    {
        view_file.language = UNICODEDATA;
    }
    else if ((unicode_flag & 0xFFFFFF) == UTF8_FLAG)
    {
        view_file.language = UTF_8DATA;
    }
    else
    {   
        //��ȡ��һ�����������ݣ�����ʶ�𡣸���unicode_flag����Ϊʵ�ʶ�ȡ������
        unicode_flag = vfs_file_read(g_ebook_mount_id, bmk_bufFile, FILE_SECTOR, view_file.file_handle); 
        
        /*******************   ��Ҫ�ж��ǲ�����BOM utf-8  ******************/
        if(Is_utf8_nobom(bmk_bufFile,FILE_SECTOR)==TRUE)
        {
            view_file.language = UTF_8DATA;
        }
        else
        {
            view_file.language = (int8) g_comval.language_id;
        } 
    }
    
    
    libc_memcpy(&page_count_file, &view_file, sizeof(view_file_t));

    //���ļ����Ⱥ��ļ��ڴ��̵�ƫ���жϸ��ļ��Ƿ�Ϊ�ϴδ򿪹����ļ�
    if ((view_file.file_length == g_ebook_vars.file_length) && (cur_file_diroffset
            == g_ebook_vars.path.file_path.dirlocation.dir_entry))
    {
        curpagenum = g_ebook_vars.page_num;
    }
    else
    {
        curpagenum = 0;
    }
    g_ebook_vars.file_length = view_file.file_length;
    if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
    {
        return _vram_init_bmk_info();
    }
    
    //��ʼ����ǩ�ļ�
    return _init_bmk_info();
    
}

/********************************************************************************
 * Description :��ȡ��ǩ�ļ��ĵ�һ����������Ϣ
 *
 * Arguments   :
 *             :
 * Returns     :
 *               TRUE or FALSE
 * Notes       :
 *            ��ǩ�ṹ��:
 *             sector 0�����:
 *                 byte:0-256:������ǩ��Ϣ��ṹ�ο�bkmark_node_t,��ౣ��16����ǩ
 *                 byte:596-599:ҳ��������ϵ�ħ�����;
 *                 byte:500-503:��ǰ�ļ�����ҳ��
 *                 byte:504-507:���һҳ���ڵ�ƫ��
 *             ����sector�����:ÿ�ĸ�byte�洢��Ӧҳ��������ƫ��
 ********************************************************************************/

bool _get_bmk_firstsec_info(void)
{
    uint32 cur_direction, file_length;
    file_length = _get_file_length(bmk_modify_handle);
    //�����ȡ����ǩ�ļ����Ȳ�����һ��������˵��û�д洢��
    //ҳ����Ϣ�����Ի��ü�����ʼ����ǩ�ļ�
    if (file_length <= 512)
    {
        curpagenum = 0;
        libc_memset(bmk_bufFile, 0xFF, FILESIZE);
        _seek_and_write(0, FILESIZE, bmk_bufFile);
        cur_BufSector = 0;
        return 1;
    }
    _seek_and_read(0, FILESIZE, bmk_bufFile);
    libc_memcpy(&cur_direction, &bmk_bufFile[512 - 16], 4);
    //�����һ����������ĸ��ֽ�Ϊ0x55aaaa55�Ļ���˵���ļ�ҳ���Ѿ��������
    //����Ļ��򽫴�����λ�ü�������ҳ��
    if (cur_direction == PAGE_MAGIC)
    {
        page_count_flag = FALSE;
    }
    libc_memcpy(&page_totalnum, &bmk_bufFile[512 - 12], 4);
    if (page_totalnum == 0xFFFFFFFF)
    {
        page_totalnum = 0;
        page_count_flag = TRUE;
        cur_BufSector = 0;
        temp_count = 0;
        return 1;
    }
    if (page_count_flag == TRUE)
    {
        libc_memcpy(&page_count_file.file_offset, &bmk_bufFile[512 - 8], 4);
        bmk_last_pagenum = page_totalnum;
    }
    return 1;
}
/********************************************************************************
 * Description :��ǩ�ļ��ĳ�ʼ��
 *
 * Arguments  :
 *              status:
 * Returns     :
 *               TRUE or FALSE
 * Notes       :
 *            ��ǩ�ṹ��:
 *             sector 0�����:
 *                 byte:0-256:������ǩ��Ϣ��ṹ�ο�bkmark_node_t,��ౣ��16����ǩ
 *                 byte:596-599:ҳ��������ϵ�ħ�����;
 *                 byte:500-503:��ǰ�ļ�����ҳ��
 *                 byte:504-507:���һҳ���ڵ�ƫ��
 *             ����sector�����:ÿ�ĸ�byte�洢��Ӧҳ��������ƫ��
 ********************************************************************************/
uint16 _init_bmk_info(void)
{
    uint16 filename_length;
    uint32 tmp_page_total;
    libc_memset(bmk_bufFile, 0x0, FILESIZE);
    
    cur_BufSector = -1;
    draw_page_num = page_count_flag = TRUE;
    page_totalnum = 0;
    bmk_last_pagenum = 0;
    temp_count = 0;
    bookmark_total = 0xFF;
    filename_length = vfs_get_name(g_ebook_mount_id, BOOKMARK_filename, MAX_FILENAME_LENGTH);

    _get_filename(BOOKMARK_filename,filename_length);   //��ȡ�ļ������ڽ�����ʾ
    
    //��ȡ��ǩ�ļ���
    if (_get_bmk_name(filename_length) == 0)
    {
        return S_FILE_OPEN_ERROR;
    }
    bmk_modify_handle = vfs_file_open(g_ebook_mount_id, BOOKMARK_filename, OPEN_MODIFY);
    if ((uint32) NULL == bmk_modify_handle)
    {
        //��ǩ�ļ��ĳ��ȼ���Ϊ��������
        tmp_page_total = view_file.file_length / 128;
        if((view_file.file_length % 128) != 0)
        {
            tmp_page_total++;
        }
        if( (tmp_page_total % 128) != 0)
        {
            tmp_page_total = (tmp_page_total & 0xffffff00) + 128;
        }
        // ��ǩ�ļ������ڣ�������ǩ�ļ�
        _show_single_dialog(S_WAITING, DIALOG_INFOR_IMMEDIATE);
        bmk_modify_handle = vfs_file_create(g_ebook_mount_id, BOOKMARK_filename, 512 + tmp_page_total * 4);
        if (bmk_modify_handle != (uint32) NULL)
        {
            //���ļ���������Ϊ����
            //      vfs_file_attralter(g_ebook_mount_id, ATTR_HIDDEN, bmk_modify_handle, 0);
            libc_memset(bmk_bufFile, 0xFF, FILESIZE);
            _seek_and_write(0, FILESIZE, bmk_bufFile);
            curpagenum = 0;
            cur_BufSector = 0;
        }
        else
        {
            return S_FILE_OPEN_ERROR;
        }
    }
    else
    {
        _get_bmk_firstsec_info();
    }
    decode_offset = page_count_file.file_offset;
    return 0;
}

/********************************************************************************
 * Description :��ȡ��ǰ�򿪵��ļ��ܳ���
 * Arguments  :
 *              mount_id:�̷�ʶ��ţ���Ҫ�ǻ�ȡfsel_init�ķ���ֵ
 *              file_handle:
 * Returns     :
 *                �ļ����ܳ���
 * Notes       :
 *
 ********************************************************************************/
uint32 _get_file_length(uint32 file_handle)
{
    uint32 file_length;
    vfs_file_get_size(g_ebook_mount_id, &file_length, file_handle, 0);
    return file_length;
}
/********************************************************************************
 * Description :��λҪ��ȡ���ļ�λ�ã�������Ϊ��λ
 * Arguments  :
 *              sector_offset:�������
 * Returns     :
 *                TRUE or FALSE
 * Notes       :
 *
 ********************************************************************************/
bool _seek_file_sector(uint32 sector_offset, uint32 file_handle)
{
    return vfs_file_seek(g_ebook_mount_id, sector_offset * 512, SEEK_SET, file_handle);
}
/********************************************************************************
 * Description :��ȡһ�����������ݵ�ĳһ����������
 * Arguments  :
 *              viewfilebuf:�����ȡ���ݵ�buf
 *              file_handle:��Ҫ��ȡ���ļ��ľ��
 * Returns     :
 *                TRUE or FALSE
 * Notes       :
 *
 ********************************************************************************/
bool _read_file_sector(uint8 *viewfilebuf, uint32 file_handle)
{
    if (vfs_file_read(g_ebook_mount_id, viewfilebuf, FILE_SECTOR, file_handle) == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
/********************************************************************************
 * Description :�ر��Ѵ򿪵��ļ��ļ����
 *
 * Arguments  :
 *              ��
 * Returns     :
 *                  ��
 * Notes       :
 *
 ********************************************************************************/

void _close_text_file(uint8 update_flag)
{
    if (bmk_modify_handle != 0)
    {
        if (update_flag == TRUE)
        {
            _writeback_bmk_info();
        }
        vfs_file_close(g_ebook_mount_id, bmk_modify_handle);
        bmk_modify_handle = 0x0;
        g_ebook_vars.play_flag = 0;
    }
    if (view_file.file_handle != 0)
    {
        if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
        {
            _vram_wb_bmk_headinfo();
        }
        vfs_file_close(g_ebook_mount_id, view_file.file_handle);

        view_file.file_handle = 0x0;
    }
}
/********************************************************************************
 * Description :��д��һ��ä���������Ҫ��������ݵ���ǩ�ļ���
 *
 * Arguments  :
 *              ��
 * Returns     :
 *                  ��
 * Notes       :
 *
 ********************************************************************************/

void _writeback_bmk_info(void)
{
    uint8 need_write = FALSE;
    if (page_count_flag == TRUE)
    {
        _seek_and_write(FILE_SECTOR + (page_totalnum - temp_count) * 4, temp_count * 4, bmk_bufFile);
    }
    _seek_and_read(0, FILE_SECTOR, bmk_bufFile);
    //���ҳ�����㻹û��ɣ�����Ҫ��д�ؼ���Ϣ
    if (page_count_flag == TRUE)
    {
        libc_memcpy(&bmk_bufFile[500], &page_totalnum, 0x04);
        libc_memcpy(&bmk_bufFile[504], &decode_offset, 0x04);
        bmk_last_pagenum = page_totalnum;
        need_write = TRUE;
    }
    //�����ǩ��Ϣ�б䶯������Ҫ���»�д
    if ((bmk_modify_flag == TRUE) && (bookmark_total <= 16))
    {
        libc_memcpy(bmk_bufFile, &bkmarks, MAX_BKMARK_NUM * 16);
        need_write = TRUE;
        bmk_modify_flag = FALSE;
    }
    if (need_write == TRUE)
    {
        _seek_and_write(0, FILE_SECTOR, bmk_bufFile);
    }
}

