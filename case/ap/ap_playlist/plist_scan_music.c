/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    ������д�ļ��ĸ���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "plist.h"
#include "rdata_ex.h"

//music playlist
static const char no_artist[] = "Unknown Artist";
static const char no_album[] = "Unknown Album";
static const char no_genre[] = "Unknown Genre";

static void get_music_info(uint32 ext_name);
static void deal_music_id3info(void);
static void store_music_info(uint16 file_num);

extern id3_type_e check_id3_type(uint32 ext_name) __FAR__;
extern bool unicode_to_char(char *str,uint16 len,uint8 encode) __FAR__;
extern void copy_deal_string(char *dest,char * source,uint8 land_id,uint8 size) __FAR__;
extern bool plist_fsel_get_nextfile (char *strfile) __FAR__;

/******************************************************************************/
/*
 * \par  Description: ��ȡ�ļ�����Ϣ(ID3,������Ϣ)
 *
 * \param[in]    ext_name--��չ��

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void get_music_info(uint32 ext_name)
{
    id3_type_e id3_type;

    id3_type = check_id3_type(ext_name);
    if (id3_type < ID3_TYPE_END)
    {
        if (FALSE != get_id3_info(&id3_file_info, NULL, id3_type))
        {
            music_f_p->id3_tag_flag = 1;

            if ((id3_file_info.track_num != 0) && (id3_file_info.track_num != UNICODE_FLAG))
            {
                music_f_p->track_num = id3_file_info.track_num;
            }

        }
        else
        {
            music_f_p->id3_tag_flag = 0;
        }

    }
    else
    {
        music_f_p->id3_tag_flag = 0;
    }

    *(uint32*) (music_f_p->filename) = ext_name;
    vfs_file_dir_offset(ap_vfs_mount, &music_f_p->dir_layer_info, &music_f_p->cluster_no, 0);

    deal_music_id3info();

    music_f_p->flag = 1;

}

//���˿ո�,ת��Ϊ xx.yyy��ʽ
//�����ַ����ȣ�������������
static void deal_if_83name(uint8 *name_buf)
{

#if 0  //����id3��ȡʱ����title���˴β���Ҫ����
    uint8 i,j;

    if((name_buf[0]==0xff) && (name_buf[1]==0xfe))
    {
        return;
    }

    //��չ��������1���ֽ�
    for(i=11;i>8;i--)
    {
        name_buf[i]=name_buf[i-1];
    }

    //�����ļ����ո�
    for(i=7;i>0;i--)
    {
        if(name_buf[i]!=0x20)
        {
            break;
        }
    }

    //�ӵ�
    i++;
    name_buf[i]=0x2e; //��"."

    //������չ���ո�
    for(j=9;j<12;j++)
    {
        if(name_buf[j]!=0x20)
        {
            i++;
            name_buf[i]= name_buf[j];
        }
    }

    if(name_buf[i]!=0x2e)
    {
        i++;
    }

    name_buf[i]=0x00;

#endif

    return;
}

/******************************************************************************/
/*
 * \par  Description: �Ի�ȡ����ID3��Ϣ���д���
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void deal_music_id3info(void)
{

    //�˴��ж�ID3��Ϣ�Ƿ�Ϊ�տ���ȥ������Ϊ�ڻ�ȡid3�����Զ�����лл
    if ((music_f_p->title_string[0] == 0) || (music_f_p->id3_tag_flag == 0))
    {
        vfs_get_name(ap_vfs_mount, music_f_p->title_string, (ID3_ITEM_SIZE * 2) / 2);
        deal_if_83name((uint8*) &music_f_p->title_string);
    }

    if ((music_f_p->artist_string[0] == 0) || (music_f_p->id3_tag_flag == 0))
    {
        libc_memcpy(music_f_p->artist_string, no_artist, sizeof(no_artist));
    }

    if ((music_f_p->album_string[0] == 0) || (music_f_p->id3_tag_flag == 0))
    {
        libc_memcpy(music_f_p->album_string, no_album, sizeof(no_album));
    }

    if ((music_f_p->gener_string[0] == 0) || (music_f_p->id3_tag_flag == 0))
    {
        libc_memcpy(music_f_p->gener_string, no_genre, sizeof(no_genre));
    }

    if (music_f_p->id3_tag_flag == 0)
    {
        music_f_p->track_num = 0;
    }
    else
    {

        if (music_f_p->track_num == UNICODE_FLAG)
        {
            unicode_to_char(music_f_p->track_string, 8, language_id);
            music_f_p->track_num = 0;
        }

        if ((music_f_p->track_num == 0) && (music_f_p->track_string[0] != 0))
        {
            uint8 i;
            uint8 j = 1;
            for (i = 0; i < 4; i++)
            {
                if ((music_f_p->track_string[i] == 0) || (music_f_p->track_string[i] == '/'))
                {
                    break;
                }
                if ((music_f_p->track_string[i] < 0x30) || (music_f_p->track_string[i] > 0x39))
                {
                    break;
                }
                else
                {
                    music_f_p->track_string[i] -= 0x30;
                }
            }
            while (i > 0)
            {
                i--;
                music_f_p->track_num += music_f_p->track_string[i] * j;
                j *= 10;
            }
        }

        if (music_f_p->track_num == 0)
        {
            music_f_p->track_num = TRACK_NULL;
        }

    }

}

/******************************************************************************/
/*
 * \par  Description: �洢��Ϣ��RAM��VM��
 *
 * \param[in]    file_num--��ǰ�Ѵ�ŵ��ļ���Ϣ����

 * \param[out]

 * \return

 * \note    ���ж�buffer�Ƿ����ˣ���д��
 *******************************************************************************/
static void store_music_info(uint16 file_num)
{
    if (file_num != 0)
    {
        if (file_num == PLIST_SAVE_MAX) //֧��2048����RAM
        {
            write_to_vram(title_vram_pt, (uint8*) TITLE_DATA_ADDR, TITLE_BUF_SIZE);
            write_to_vram(album_vram_pt, (uint8*) ALBUM_DATA_ADDR, ALBUM_BUF_SIZE);
            write_to_vram(artist_vram_pt, (uint8*) ARTIST_DATA_ADDR, ARTIST_BUF_SIZE);
            //            write_to_vram(genre_vram_pt, (uint8*) GENRE_DATA_ADDR, GENRE_BUF_SIZE);
            write_to_vram(track_vram_pt, (uint8*) TRACK_DATA_ADDR, TRACK_BUF_SIZE);//2K

            title_vram_pt = title_vram_pt + TITLE_BUF_SIZE;
            album_vram_pt = album_vram_pt + ALBUM_BUF_SIZE;
            artist_vram_pt = artist_vram_pt + ARTIST_BUF_SIZE;
            //            genre_vram_pt = genre_vram_pt + GENRE_BUF_SIZE;
            track_vram_pt = track_vram_pt + TRACK_BUF_SIZE;

            title_data_pt = (uint8*) TITLE_DATA_ADDR;
            album_data_pt = (uint8*) ALBUM_DATA_ADDR;
            artist_data_pt = (uint8*) ARTIST_DATA_ADDR;
            //            genre_data_pt = (uint8*) GENRE_DATA_ADDR;
            track_data_pt = (uint8*) TRACK_DATA_ADDR;

        }
        else
        {

            title_data_pt += TITLE_CMP_LENGTH;
            artist_data_pt += ARTIST_CMP_LENGTH;
            album_data_pt += ALBUM_CMP_LENGTH;
            //            genre_data_pt += GENRE_CMP_LENGTH;
            track_data_pt += TRACK_CMP_LENGTH;//ֻ����1bytes


        }

        //GENRE�ر���buffer����
        if (0 == (file_num % PLIST_GENRE_MAX))//֧��1024����RAM
        {
            write_to_vram(genre_vram_pt, (uint8*) GENRE_DATA_ADDR, GENRE_BUF_SIZE);
            genre_vram_pt = genre_vram_pt + GENRE_BUF_SIZE;
            genre_data_pt = (uint8*) GENRE_DATA_ADDR;
        }
        else
        {
            genre_data_pt += GENRE_CMP_LENGTH;
        }

        if ((file_num % (FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE)) == 0)
        {
            save_to_file((uint8*) FILE_INFO_ADDR, FILE_INFO_BUF_SIZE);
            fileinfo_data_pt = (uint8*) FILE_INFO_ADDR;
        }
        else
        {
            fileinfo_data_pt += PLIST_FILE_SIZE;
        }
    }

    music_f_p->prev_offset = file_num - 1;
    music_f_p->next_offset = file_num + 1;

    copy_deal_string(title_data_pt, music_f_p->title_string, language_id, CMPSTR_MAX_LEN1);
    copy_deal_string(artist_data_pt, music_f_p->artist_string, language_id, CMPSTR_MAX_LEN2);
    copy_deal_string(album_data_pt, music_f_p->album_string, language_id, CMPSTR_MAX_LEN2);
    copy_deal_string(genre_data_pt, music_f_p->gener_string, language_id, CMPSTR_MAX_LEN2);
    libc_memcpy(fileinfo_data_pt, music_f_p, sizeof(plist_f_info_t));
    //   *(uint16*) track_data_pt=music_f_p->track_num;
    *track_data_pt = (uint8) music_f_p->track_num;//ֻ�����8λ

}

/******************************************************************************/
/*
 * \par  Description: ɨ���ļ�
 *
 * \param[in]

 * \param[out]

 * \return        �ļ�����

 * \note
 *******************************************************************************/
uint16 scan_music_file(void)
{
    uint32 temp;
    uint32 ext_name;
    //    uint16 music_num;
    //    uint8  scan_flag=0;
    uint16 debug_num = 900;

    music_f_p = (void*) file_buffer;
    id3_file_info.tit2_buffer = music_f_p->title_string;
    id3_file_info.tit2_length = ID3_ITEM_SIZE * 2;
    id3_file_info.tpe1_buffer = music_f_p->artist_string;
    id3_file_info.tpe1_length = ID3_ITEM_SIZE;
    id3_file_info.talb_buffer = music_f_p->album_string;
    id3_file_info.talb_length = ID3_ITEM_SIZE;
    id3_file_info.genre_buffer = music_f_p->gener_string;
    id3_file_info.genre_length = ID3_ITEM_SIZE;

    id3_file_info.track_buffer = music_f_p->track_string;
    id3_file_info.track_length = 8;

    id3_file_info.drm_length = 0;
    id3_file_info.drm_buffer = 0;

#ifdef ENHANCED_FIND_MODE
    while(FALSE!=fsel_get_nextfile((char*)&ext_name))
#else
    while (FALSE != plist_fsel_get_nextfile((char*) &ext_name))
#endif
    {
        if (debug_num == ap_plist_file_total)
        {
            debug_num = 0;
        }

        if (0 == (ap_plist_file_total & 0x07))
        {
            plist_get_app_msg();
        }

        libc_memset(music_f_p, 0x00, sizeof(plist_f_info_t));
        get_music_info(ext_name);
        store_music_info(ap_plist_file_total);
        ap_plist_file_total++;
        if (ap_plist_file_total >= PLIST_MUSIC_MAX)
        {
            break;
        }
    }

    if (ap_plist_file_total > PLIST_SAVE_MAX)
    {
        temp = (ap_plist_file_total % PLIST_SAVE_MAX) * TITLE_CMP_LENGTH;
        write_to_vram(title_vram_pt, (uint8*) TITLE_DATA_ADDR, temp);
        temp = (ap_plist_file_total % PLIST_SAVE_MAX) * ARTIST_CMP_LENGTH;
        write_to_vram(artist_vram_pt, (uint8*) ARTIST_DATA_ADDR, temp);
        write_to_vram(album_vram_pt, (uint8*) ALBUM_DATA_ADDR, temp);
        //        write_to_vram(genre_vram_pt, (uint8*) GENRE_DATA_ADDR, temp);
    }

    if (ap_plist_file_total > PLIST_GENRE_MAX)
    {
        temp = (ap_plist_file_total % PLIST_GENRE_MAX) * GENRE_CMP_LENGTH;
        write_to_vram(genre_vram_pt, (uint8*) GENRE_DATA_ADDR, temp);
    }

    if (ap_plist_file_total > 0)
    {
        temp = ap_plist_file_total % (FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE);
        if (temp == 0)
        {
            //�պ�д��
            temp = FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE;
        }
        save_to_file((uint8*) FILE_INFO_ADDR, temp * PLIST_FILE_SIZE);

        //    ap_plist_file_total=music_num;
    }

    plist_get_app_msg();

    return ap_plist_file_total;

}

