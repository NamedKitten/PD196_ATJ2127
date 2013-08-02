/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_vram_operate.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/

#include  "ebook.h"

/********************************************************************************
 * Description :��ǩ�ļ��ĳ�ʼ��
 *
 * Arguments  :
 *              status:
 * Returns     :
 *               TRUE or FALSE
 * Notes       :
 *            ��ǩ�ṹ��:
 *             ��VM_BMK_INFO        0x00100000��ʼ�ĵ�һ��sector�����:
 *                 byte:0-47F:�洢vram_bmk_info_t��Ϣ�����֧��MAX_BMK_FILE���ļ���Ϣ
 *                 byte:480-508:����
 *                 byte:508-512:0x55aaaa55��ʾ��ǩ��vram���ѳ�ʼ������
 *             ��ǩ��Ϣ��ʼ�洢��һ��sector�����:
 *                 byte:0-255:������ǩ��Ϣ��ṹ�ο�bkmark_node_t,��ౣ��16����ǩ
 *                 byte:256-463:����
 *                 byte:464-512:�洢decode_bmk_info_t ��Ϣ
 *                 byte:504-507:���һҳ���ڵ�ƫ��
 *             ����sector�����:ÿ�ĸ�byte�洢��Ӧҳ��������ƫ��
 ********************************************************************************/
uint16 _vram_init_bmk_info(void)
{
    uint32 headinfo_magic;
    uint16 filename_length;
    
    uint8 i = 0;
    libc_memset(bmk_bufFile, 0x0, FILESIZE);
    cur_BufSector = -1;
    draw_page_num = page_count_flag = TRUE;
    page_totalnum = 0;
    bmk_last_pagenum = 0;
    temp_count = 0;
    bookmark_total = 0xFF;
    decode_offset = 0;
    filename_length = vfs_get_name(g_ebook_mount_id, BOOKMARK_filename, MAX_FILENAME_LENGTH);
    
    _get_filename(BOOKMARK_filename,filename_length);   //��ȡ�ļ������ڽ�����ʾ

    sys_vm_read(bmk_bufFile, VM_BMK_INFO, FILESIZE);
    libc_memcpy(&headinfo_magic, &bmk_bufFile[FILESIZE - 4], 4);

    while (1)
    {
        //����ļ���������֧�ֵ����������ħ�����ԵĻ����³�ʼ��vram������
        if ( (i >= MAX_BMK_FILE ) || (headinfo_magic != PAGE_MAGIC) )
        {
            libc_memset(bmk_bufFile, 0xFF, FILESIZE);
            i = 0;
        }
        libc_memcpy(&vram_bmk_info, &bmk_bufFile[i * 12], 12);
        //�����ǰ�Ľṹ��Ϣ���ԣ������³�ʼ���ṹ��Ϣ
        if ((vram_bmk_info.cluster_no == 0x0) || (vram_bmk_info.cluster_no == 0xFFFFFFFF))
        {
            _vram_init_bmk_headinfo(i);
            return 0;
        }
        if ((vram_bmk_info.cluster_no == g_ebook_vars.path.file_path.dirlocation.cluster_no)
                && (vram_bmk_info.dir_entry == g_ebook_vars.path.file_path.dirlocation.dir_entry))
        {
            break;
        }
        else
        {
            i++;
        }
    }
    //��ȡ��ǩ��Ϣ���ж��Ƿ��뵱ǰ����ǩ��Ϣһ�£������һ��
    //��Ҫ������Ҫ��ʼ��
    sys_vm_read(bmk_bufFile, VM_BMK_INFO + vram_bmk_info.startadd, FILESIZE);
    libc_memcpy(&decode_bmk_info, &bmk_bufFile[FILESIZE - 0x30], 0x30);
    if (libc_memcmp(decode_bmk_info.file_name, BOOKMARK_filename, filename_length) !=0 )
    {
        i++;
        sys_vm_read(bmk_bufFile, VM_BMK_INFO, FILESIZE);
        _vram_init_bmk_headinfo(i);
        return 0;
    }
    if (decode_bmk_info.decode_flag == PAGE_MAGIC)
    {
        page_count_flag = FALSE;
    }
    if (decode_bmk_info.totalpage == 0xFFFFFFFF)
    {
        page_totalnum = 0;
        page_count_flag = TRUE;
        cur_BufSector = 0;
        temp_count = 0;
        return 0;
    }
    decode_offset = page_count_file.file_offset = decode_bmk_info.decode_offset;
    page_totalnum = decode_bmk_info.totalpage;
    //���ҳ�����㻹û����ɣ���Ҫ�����洢����һ��sector�����ݶ�ȡ������
    //�Ա���������Ժ�����sector��д
    if ((page_count_flag == TRUE) && (page_totalnum > 1))
    {
        _vram_read_bmkinfo();
        bmk_last_pagenum = page_totalnum;
    }
    return 0;
}

void _vram_init_bmk_headinfo(uint8 file_index)
{
    uint32 tempadd;
    uint32 headinfo_magic;
    uint32 next_vram_bmk_startadd;
    vram_bmk_info.cluster_no = g_ebook_vars.path.file_path.dirlocation.cluster_no;
    vram_bmk_info.dir_entry = g_ebook_vars.path.file_path.dirlocation.dir_entry;
    tempadd = (view_file.file_length / 64);
    tempadd = ((tempadd / FILESIZE) + 4) * FILESIZE;
    if (file_index != 0)
    {
        libc_memcpy(&vram_bmk_info.startadd, &bmk_bufFile[file_index * 12 + 8], 4);
    }
    else
    {
        vram_bmk_info.startadd = FILESIZE;
    }
    next_vram_bmk_startadd = vram_bmk_info.startadd + tempadd;
    //���дvram���Ĵ�С�Ѿ�����������ַ�������vram�������³�ʼ��
    if (next_vram_bmk_startadd >= VRAM_END_ADDRESS)
    {
        libc_memset(bmk_bufFile, 0xFF, FILESIZE);
        vram_bmk_info.startadd = FILESIZE;
        next_vram_bmk_startadd = vram_bmk_info.startadd + tempadd;
        file_index = 0;
    }
    libc_memcpy(&bmk_bufFile[(file_index + 1) * 12 + 8], &next_vram_bmk_startadd, 4);
    libc_memcpy(&bmk_bufFile[file_index * 12], &vram_bmk_info, sizeof(vram_bmk_info));
    headinfo_magic = PAGE_MAGIC;
    libc_memcpy(&bmk_bufFile[FILESIZE - 4], &headinfo_magic, 4);
    _vram_write_data(0);

    libc_memset(bmk_bufFile, 0xff, FILESIZE);
    libc_memcpy(decode_bmk_info.file_name, BOOKMARK_filename, 32);
    libc_memcpy(&bmk_bufFile[FILESIZE - 0x30], &decode_bmk_info, sizeof(decode_bmk_info));
    _vram_write_data(vram_bmk_info.startadd);
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

void _vram_wb_bmk_headinfo(void)
{
    uint8 need_write = FALSE;
    uint32 offset_sector;
    if (page_count_flag == TRUE)
    {
        offset_sector = vram_bmk_info.startadd + FILESIZE + ((page_totalnum - 1) / 128) * FILESIZE;
        _vram_write_data(offset_sector);
    }
    //���ҳ�����㻹û��ɣ�����Ҫ��д�ؼ���Ϣ
    sys_vm_read(bmk_bufFile, VM_BMK_INFO +vram_bmk_info.startadd, FILESIZE);
    if (page_count_flag == TRUE)
    {
        decode_bmk_info.totalpage = page_totalnum;
        decode_bmk_info.decode_offset = decode_offset;
        decode_bmk_info.decode_flag = 0xFFFFFFFF;
        libc_memcpy(&bmk_bufFile[FILESIZE - 0x30], &decode_bmk_info, 0x30);
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
        _vram_write_data(vram_bmk_info.startadd);
    }
}

/********************************************************************************
 * Description :������õ�����ǩ��Ϣд��VRAM����ÿ�ζ���һ��sectorд
 *
 * Arguments  :
 *              ��
 * Returns     :
 *                  ��
 * Notes       :
 *
 ********************************************************************************/
void _vram_write_bmkinfo(void)
{
    uint32 offset_sector;
    //��������ļ�β����ҳ����Ϣ�ͽ�����PAGE_MAGIC��д���ļ���
    if (decode_offset >= page_count_file.file_length)
    {
        offset_sector = vram_bmk_info.startadd + FILESIZE + ((page_totalnum - 1) / 128) * FILESIZE;
        _vram_write_data(offset_sector);

        sys_vm_read(bmk_bufFile, VM_BMK_INFO + vram_bmk_info.startadd, FILESIZE);
        decode_bmk_info.decode_flag = PAGE_MAGIC;
        decode_bmk_info.totalpage = page_totalnum;
        libc_memcpy(&bmk_bufFile[FILESIZE - 0x30], &decode_bmk_info, sizeof(decode_bmk_info));
        _vram_write_data(vram_bmk_info.startadd);
        page_count_flag = FALSE;
        cur_BufSector = -1;
        return;
    }
    if (page_totalnum > 0)
    {
        libc_memcpy(&bmk_bufFile[((page_totalnum - 1) % 128) * 4], &decode_offset, 0x04);
    }
    //��ҳ����Ϣ��һ������ʱ�������ݻ�д��ǩ�ļ�
    if ((page_totalnum % 128) == 0)
    {
        bmk_last_pagenum = page_totalnum;
        offset_sector = vram_bmk_info.startadd + FILESIZE + ((page_totalnum - 1) / 128) * FILESIZE;
        _vram_write_data(offset_sector);
        temp_count = 0;
        libc_memset(bmk_bufFile, 0x0, FILESIZE);
    }
}
/********************************************************************************
 * Description :��vram����ĳ��λ�ö�ȡһ��sector������
 *
 * Arguments  :
 *              ��
 * Returns     :
 *                  ��
 * Notes       :
 *
 ********************************************************************************/

void _vram_read_bmkinfo(void)
{
    uint32 offset_sector;
    offset_sector = vram_bmk_info.startadd + FILESIZE + ((page_totalnum - 1) / 128) * FILESIZE;
    sys_vm_read(bmk_bufFile, VM_BMK_INFO + offset_sector, FILESIZE);
}
/********************************************************************************
 * Description :��vram����ĳ��λ��д��һ��sector������
 *
 * Arguments  :
 *              ��
 * Returns     :
 *                  ��
 * Notes       :
 *
 ********************************************************************************/

void _vram_write_data(uint32 address_offset)
{
    sys_vm_write(bmk_bufFile, VM_BMK_INFO + address_offset);
}
void _vram_limit_file_length(void)
{
    if ((g_ebook_vars.path.file_path.dirlocation.disk == DISK_C) && (view_file.file_length > MAX_FILE_LENGTH))
    {
        view_file.file_length = MAX_FILE_LENGTH;
    }
}
