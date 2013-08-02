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
 * \file     eh_bs_entry.c
 * \brief    ����ӿ������
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               �ӽӿ����
 * \par      EXTERNALIZED FUNCTIONS:
 *               �ļ�ϵͳ
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "../eh_bs.h"

uint8 bs_temp_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;

extern bool bs_dir_enter(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow);
extern bool bs_plist_enter(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow);
extern bool bs_uspl_enter(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow);

extern bool bs_dir_get_items(brow_decode_t *decode_param, void *file_list);
extern bool bs_plist_get_items(brow_decode_t *decode_param, void *file_list);
extern bool bs_uspl_get_items(brow_decode_t *decode_param, void *file_list);

extern bool bs_dir_get_location(file_location_t *location, fsel_type_e type);
extern bool bs_plist_get_location(plist_location_t *location, fsel_type_e type);
extern bool bs_uspl_get_location(flist_location_t*location, fsel_type_e type);

extern bool bs_dir_set_location(file_location_t *location, fsel_type_e type);
extern bool bs_plist_set_location(plist_location_t *location, fsel_type_e type);
extern bool bs_uspl_set_location(flist_location_t*location, fsel_type_e type);

extern bool bs_dir_delete(brow_del_e type, uint16 list_no, file_location_t *location);
extern bool bs_plist_delete(brow_del_e type, uint16 list_no, plist_location_t *location);
extern bool bs_uspl_delete(brow_del_e type, uint16 list_no, flist_location_t *location);

extern bool bs_dir_set_file(file_location_t *location, fsel_type_e type);
extern bool bs_plist_set_file(plist_location_t *location, fsel_type_e type);
extern bool bs_uspl_set_file(flist_location_t*location, fsel_type_e type);

extern bool bs_plist_get_albums(brow_decode_t *decode_param, void *album_list);

typedef bool (*fsel_bs_enter_f)(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow);
typedef bool (*fsel_bs_get_items_f)(brow_decode_t *decode_param, void *file_list);
typedef bool (*fsel_bs_get_location_f)(void *location, fsel_type_e type);
typedef bool (*fsel_bs_del_f)(brow_del_e type, uint16 list_no, void *location);
typedef bool (*fsel_bs_set_location_f)(void *location, fsel_type_e type);
typedef bool (*fsel_browser_set_file_f)(void *location, fsel_type_e type);

static const fsel_bs_enter_f fsel_bs_enter_func[3] =
{ bs_dir_enter, bs_plist_enter, bs_uspl_enter };
static const fsel_bs_get_items_f fsel_bs_get_items_func[3] =
{ bs_dir_get_items, bs_plist_get_items, bs_uspl_get_items };
static const fsel_bs_get_location_f fsel_bs_get_location_func[3] =
{ (void *) bs_dir_get_location, (void *) bs_plist_get_location, (void *) bs_uspl_get_location };
static const fsel_bs_del_f fsel_bs_del_func[3] =
{ (void *) bs_dir_delete, (void *) bs_plist_delete, (void *) bs_uspl_delete };
static const fsel_bs_set_location_f fsel_bs_set_location_func[3] =
{ (void *) bs_dir_set_location, (void *) bs_plist_set_location, (void *) bs_uspl_set_location };

static const fsel_browser_set_file_f fsel_browser_set_file_func[3] =
{ (void *) bs_dir_set_file, (void *) bs_plist_set_file, (void *) bs_uspl_set_file };

/******************************************************************************/
/*
 * \par  Description:�����ļ�ѡ������Ŀ¼
 *
 * \param[in]    type--��������
 CD_UP     �ı䵱ǰĿ¼����һ����Ŀ¼��Ŀ¼��ָ��ָ��Ŀ¼��ʼλ�ã�
 CD_BACK   �ı䵱ǰĿ¼����һ����Ŀ¼��Ŀ¼��ָ��ָ��֮ǰCD��ȥ����Ŀ¼��
 CD_ROOT   �ı䵱ǰĿ¼����Ŀ¼��
 CD_SUB    �ı䵱ǰĿ¼����ǰĿ¼���Ӧ����Ŀ¼����ʱ����

 list_no--�б��1~N��

 * \param[out]   dir_brow--����Ŀ¼��Ϣ

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_browser_enter_dir(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow)
{
    fsel_error = FSEL_NO_ERR;
    return fsel_bs_enter_func[fsel_module_type](type, list_no, dir_brow);
}

/******************************************************************************/
/*
 * \par  Description:���ʱ����ȡ�б������ɸ���Ϣ��
 *
 * \param[in]    decode_param -- ������Ʒ�ʽ����ǰ����������Ϣ�������

 * \param[out]   file_list--���ػ�ȡ������Ϣ�����ݣ���file_browse_t������

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_browser_get_items(brow_decode_t *decode_param, file_brow_t *file_list)
{
    fsel_error = FSEL_NO_ERR;

    if ((decode_param == NULL) || (file_list == NULL))
    {
        return FALSE;
    }

    if ((decode_param->top == 0) || (decode_param->num == 0))
    {
        return FALSE;
    }

    return fsel_bs_get_items_func[fsel_module_type](decode_param, file_list);
}

/******************************************************************************/
/*
 * \par  Description:�����б�ר��ͼƬ���ʱ����ȡ�б������ɸ���Ϣ��
 *
 * \param[in]    decode_param -- ������Ʒ�ʽ����ǰ����������Ϣ�������

 * \param[out]   album_list--���ػ�ȡ������Ϣ�����ݣ���file_browse_t������

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_browser_get_albums(brow_decode_t *decode_param, album_brow_t *album_list)
{
    fsel_error = FSEL_NO_ERR;

    if (fsel_module_type == FSEL_MODULE_PLIST)
    {
        return bs_plist_get_albums(decode_param, album_list);
    }

    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: ���ʱ��ѡ����ǰ�б�ĳ���ļ����б��Ŀ¼��
 *
 * \param[in]    list_no--�б��1~N��

 * \param[out]

 * \return       TRUE OR FALSE

 * \note         ��Ҫ��fsel_browser_get_location�������ȵ��ýӿڣ�ʹ�ļ�ѡ�����ȶ�λ
 ��list_no=0xffff��ѡ��ǰ�б��������ļ�����
 *******************************************************************************/
bool fsel_browser_select(uint16 list_no)
{
    fsel_error = FSEL_NO_ERR;
    if ((list_no == 0xffff) || (list_no <= (bs_file_total + bs_dir_total)))
    {
        bs_select_listno = list_no;
    }
    else
    {
        return FALSE;
    }

    /*
     if(fsel_module_type==FSEL_MODULE_PLIST)
     {
     list_layer_no[dir_layer]=list_no;

     }
     */
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ���ʱ�������б���Ż�locationɾ����ǰ�б��µ��ļ�
 *
 * \param[in]    type     -- ȡֵΪ  FSEL_BROW_DEL_FILE  //ɾ��ָ�����ļ�
 list_no  -- �б��1~N��
 location -- �ļ���location�ṹ��û���ļ����ʱ��list_no=0��ɾ����ʹ�ã�

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
bool fsel_browser_delete(brow_del_e type, uint16 list_no, void *location)
{
    fsel_error = FSEL_NO_ERR;
    if ((list_no == 0) && (location == NULL))
    {
        return FALSE;
    }

    return fsel_bs_del_func[fsel_module_type](type, list_no, location);
}

/******************************************************************************/
/*
 * \par  Description:  ȡ��ǰѡ���ļ����б�λ��
 *
 * \param[in]    location -- �洢��ǰѡ���ļ���λ����Ϣ
 type --location���ͣ�no used��
 * \param[out]

 * \return       TRUE OR FALSE

 * \note   ���˽ӿ�ǰ�����ȵ�fsel_browser_select(uint16 list_no)��ȷ������
 *******************************************************************************/
bool fsel_browser_get_location(void *location, fsel_type_e type)
{
    fsel_error = FSEL_NO_ERR;

    if ((0x00 == bs_select_listno) || (NULL == location))
    {
        return FALSE;
    }

    return fsel_bs_get_location_func[fsel_module_type](location, type);
}

/******************************************************************************/
/*
 * \par  Description: ���õ�ǰ������ļ�ѡ��λ��
 *
 * \param[in]    location -- ����ļ���λ����Ϣ
 type --location���ͣ�no used��

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_browser_set_location(void *location, fsel_type_e type)
{
    fsel_error = FSEL_NO_ERR;

    if (location == NULL)
    {
        return FALSE;
    }

    return fsel_bs_set_location_func[fsel_module_type](location, type);
}

/******************************************************************************/
/*
 * \par  Description: ����bitmap����ͳ�Ƶ�ǰĿ¼ �޸�location��file_total��file_numֵ
 *
 * \param[in]    location -- ѡ���ļ���λ����Ϣ
 bitmap  --  ѡ���ļ�������bitmap

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_browser_modify_location(file_location_t *location, uint32 bitmap)
{
    uint16 t_file_no = 0;
    uint16 t_total = 0;
    uint8 dir_type = DIR_HEAD;

    fsel_error = FSEL_NO_ERR;
    //��bs_temp_buf����

    if (location == NULL)
    {
        return FALSE;
    }

    vfs_file_dir_offset(vfs_mount, &bs_temp_buf[128], &bs_temp_buf[16], 0);

    while (0 != vfs_dir(vfs_mount, dir_type, NULL, bitmap))
    {
        t_total++;
        if (0 == t_file_no)
        {
            //��bs_temp_buf���� 8�ֽ�
            vfs_file_dir_offset(vfs_mount, NULL, &bs_temp_buf, 0);
            if (libc_memcmp(&location->cluster_no, bs_temp_buf, 8) == 0)
            {
                t_file_no = t_total;
            }
        }
        dir_type = DIR_NEXT;
    }

    vfs_file_dir_offset(vfs_mount, &bs_temp_buf[128], &bs_temp_buf[16], 1);

    if ((t_file_no != 0) && (t_total != 0))
    {
        location->file_total = t_total;
        location->file_num = t_file_no;
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

/******************************************************************************/
/*
 * \par  Description: ��λ���ļ��ڴ��̵�Ŀ¼��λ��
 *
 * \param[in]    location -- �ļ���λ����Ϣ
 type --location���ͣ�no used��

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_browser_set_file(void *location, fsel_type_e type)
{
    if (NULL == location)
    {
        return FALSE;
    }
    return fsel_browser_set_file_func[fsel_module_type](location, type);

}

/******************************************************************************/
/*
 * \par  Description: dir �ļ����ļ���
 *
 * \param[in]    dir_type --dir����
 object_type --��������

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//dir �ӿڷ�װ
bool bs_dir_function(uint8 dir_type, uint32 object_type)
{
    //   uint8 name[4];

    //��Ҫ�л�Ŀ¼�����ڽӿ�ǰ׼����
    cur_ext_name = vfs_dir(vfs_mount, (uint32) dir_type, NULL, object_type);
    if (0 == cur_ext_name)
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ��λ��ָ����dirλ��
 *
 * \param[in]    dir_num--from (1~N)
 object_type --��������
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//��λ��ָ����Ŀ¼��λ��
bool bs_dir_set_dirpos(uint32 object_type, uint16 dir_num)
{
    //    uint8 name[4];
    uint8 t_dir_type;

    //��Ҫ�л�Ŀ¼�����ڽӿ�ǰ׼����

    if (dir_num == 0)
    {
        return FALSE;
    }

    t_dir_type = DIR_HEAD;

    for (; dir_num > 0; dir_num--)
    {
        if (FALSE == bs_dir_function(t_dir_type, object_type))
        {
            fsel_error = FSEL_ERR_OUTOF;
            return FALSE;
        }
        t_dir_type = DIR_NEXT;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ��VMָ��������
 *
 * \param[in]    sector_offset--����ƫ�ƣ�0~N

 * \param[out]   NULL

 * \return       NULL

 * \note �򿪵��ļ���֧������seek
 *******************************************************************************/
bool bs_read_sector_vram(uint16 sector_offset)
{
    if (save_read_offset != sector_offset)
    {
        if ((FALSE != write_data_flag) && (save_read_offset != 0xffff))
        {
            //��дVRAM��obj_handle���VM��ַ
            sys_vm_write(bs_temp_buf, obj_handle + save_read_offset * SECTOR_SIZE);
        }

        sys_vm_read(bs_temp_buf, obj_handle + sector_offset * SECTOR_SIZE, SECTOR_SIZE);
        save_read_offset = sector_offset;

    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ���ļ�ָ��������
 *
 * \param[in]    sector_offset--����ƫ�ƣ�0~N

 * \param[out]   NULL

 * \return       NULL

 * \note �򿪵��ļ���֧������seek
 *******************************************************************************/
bool bs_read_sector_data(uint16 sector_offset)
{
    uint16 seek_type;
    int16 seek_sector;

    if (0xee == obj_open_mode)
    {
        return bs_read_sector_vram(sector_offset);
    }

    if (save_read_offset != sector_offset)
    {
        if ((save_read_offset == 0xffff) || (save_read_offset > sector_offset))
        { //��һ�ζ� �� �����ʱʹ��
            seek_type = SEEK_SET;
            seek_sector = sector_offset;
        }
        else
        {
            //��ǰ��ʱʹ��
            seek_type = SEEK_CUR;
            //seek_sector��ֵ����Ϊ0�����Ϊ����������
            seek_sector = sector_offset - save_read_offset - 1;//���ļ�ָ����ʵΪsave_read_offset+1
        }

        if (write_data_flag && (save_read_offset != 0xffff))
        {
            vfs_file_seek(vfs_mount, save_read_offset * SECTOR_SIZE, SEEK_SET, obj_handle);
            vfs_file_write(vfs_mount, bs_temp_buf, SECTOR_SIZE, obj_handle);
        }

        if ((seek_sector != 0) || (seek_type == SEEK_SET))//���ǽ��Ŷ���һ�������Ͳ���Ҫseek��
        {
            vfs_file_seek(vfs_mount, seek_sector * SECTOR_SIZE, seek_type, obj_handle);
        }
        vfs_file_read(vfs_mount, bs_temp_buf, SECTOR_SIZE, obj_handle);
        save_read_offset = sector_offset;

    }

    return TRUE;
}

