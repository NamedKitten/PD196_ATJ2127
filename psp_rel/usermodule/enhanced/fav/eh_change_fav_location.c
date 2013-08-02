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
 * \file     *.c
 * \brief    ת�����ղؼ�location
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"
#include "eh_rdata_ex.h"

static uint8 change_fav_buffer[SECTOR_SIZE] _BANK_DATA_ATTR_;

#define MUSIC_EXT_NUM 13

//����const data ���к�׺����
static const uint8 support_ext[][4] =
{ "MP1", "MP2", "MP3", "WMA", "WMV", "ASF", "APE", "FLA", "OGG", "AAC", "AAX", "M4A", "AA " };

static const id3_type_e support_id3_type[] =
{ ID3_TYPE_MP3, ID3_TYPE_MP3, ID3_TYPE_MP3, ID3_TYPE_WMA, ID3_TYPE_WMA, ID3_TYPE_WMA, ID3_TYPE_APE, ID3_TYPE_FLAC,
        ID3_TYPE_OGG, ID3_TYPE_AAC, ID3_TYPE_AAX, ID3_TYPE_AAX, ID3_TYPE_AA };
/******************************************************************************/
/*!
 * \par  Description:
 * \id3_type_e eh_change_checktype(char * extname)
 * \��ȡ��׺��
 * \param[in]    extname��׺��buffer  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
static id3_type_e chg_change_checktype(uint8 * extname)
{
    uint8 cnt;
    id3_type_e ret;
    //ת����ǰ��׺��(ת�ɴ�д)
    extname[0] = ((extname[0] >= 'a') && (extname[0] <= 'z')) ? (extname[0] - 'a' + 'A') : extname[0];
    extname[1] = ((extname[1] >= 'a') && (extname[1] <= 'z')) ? (extname[1] - 'a' + 'A') : extname[1];
    extname[2] = ((extname[2] >= 'a') && (extname[2] <= 'z')) ? (extname[2] - 'a' + 'A') : extname[2];
    //�ȽϺ�׺
    for (cnt = 0; cnt < MUSIC_EXT_NUM; cnt++)
    {
        if (libc_strncmp(extname, support_ext[cnt], sizeof(support_ext[cnt])) == 0)
        {
            ret = support_id3_type[cnt];
            break;
        }
    }
    if (cnt == MUSIC_EXT_NUM)
    {
        ret = ID3_TYPE_END;
    }
    return ret;
}

/******************************************************************************/
/*
 * \par  Description:������locationת���ղؼе�location
 *
 * \param[in]    source_location Դlocation
 * \param[out]   dest_location �ղؼе�location
 * \param[in]    source_type Դlocation����
 * \param[in]    id3_info_buf ID3��Ϣ�ṹ

 * \return        TRUE or FALSE

 * \note
 *******************************************************************************/
bool fsel_change_fav_location(void *source_location, flist_location_t *dest_location, fsel_type_e source_type,
        id3_info_t *id3_info_buf)
{
    uint8 type_case;
    id3_type_e file_id3_type;

    bool ret_val = TRUE;

    if ((source_location == NULL) || (dest_location == NULL) || (id3_info_buf == NULL))
    {
        return FALSE;
    }

    type_case = 1;
    switch (source_type)
    {
        case FSEL_TYPE_LISTFAVOR1:
        case FSEL_TYPE_LISTFAVOR2:
        case FSEL_TYPE_LISTFAVOR3:
        {
            type_case = 0;
            libc_memcpy(dest_location, source_location, sizeof(flist_location_t));
            break;
        }

        case FSEL_TYPE_COMMONDIR:
        case FSEL_TYPE_DISKSEQUNCE:
        {
            file_location_t *location_p;
            location_p = source_location;
            libc_memcpy(&dest_location->dir_layer_info, &location_p->dir_layer_info, sizeof(pdir_layer_t));
            break;
        }

        default:
        {
            plist_location_t *location_p;
            location_p = source_location;
            libc_memcpy(&dest_location->dir_layer_info, &location_p->dir_layer_info, sizeof(pdir_layer_t));
            break;
        }

    }

    if (0 == type_case)
    {
        return TRUE;
    }

    libc_memcpy(dest_location, source_location, 22);

    //ȡ������Ϣ
    id3_info_buf->tit2_buffer = dest_location->title; //�����ļ�����buffer
    id3_info_buf->tpe1_buffer = 0; //��������buffer
    id3_info_buf->talb_buffer = 0; //����ר��buffer
    id3_info_buf->genre_buffer = 0; //��������buffer
    id3_info_buf->drm_buffer = 0; //����drm��buffer
    id3_info_buf->track_buffer = 0; //��������buffer
    id3_info_buf->tit2_length = USERPL_TITLE_MAX; //�ļ����ⳤ��
    id3_info_buf->tpe1_length = 0; //���߳���
    id3_info_buf->talb_length = 0; //ר������
    id3_info_buf->genre_length = 0; //���ɳ���
    id3_info_buf->drm_length = 0; //drm����
    id3_info_buf->track_length = 0; //���쳤��
    id3_info_buf->apic_flag = 0; //��ȡͼƬƫ�Ʊ��
    id3_info_buf->ldb_flag = 0; //��ȡldb���ƫ�Ʊ��


    file_id3_type = chg_change_checktype(dest_location->filename);

    vfs_file_dir_offset(vfs_mount, change_fav_buffer, change_fav_buffer + 128, 0);

    vfs_file_dir_offset(vfs_mount, &dest_location->dir_layer_info, &dest_location->cluster_no, 1);
    vfs_get_name(vfs_mount, dest_location->title, USERPL_TITLE_MAX / 2);

    vfs_file_dir_offset(vfs_mount, change_fav_buffer, change_fav_buffer + 128, 1);

    if (file_id3_type != ID3_TYPE_END)
    {
        //���붨���ڶ�ջ�ϣ���Ϊ��ȡID3��Ҫʹ��B1��B2
        pdir_layer_t tmp_pdir_layer_buffer;
        pfile_offset_t tmp_pfile_offset_buffer;

        vfs_file_dir_offset(vfs_mount, &tmp_pdir_layer_buffer, &tmp_pfile_offset_buffer, 0);

        vfs_file_dir_offset(vfs_mount, &dest_location->dir_layer_info, &dest_location->cluster_no, 1);
        ret_val = get_id3_info(id3_info_buf, NULL, file_id3_type);

        vfs_file_dir_offset(vfs_mount, &tmp_pdir_layer_buffer, &tmp_pfile_offset_buffer, 1);
    }

    return ret_val;

}

