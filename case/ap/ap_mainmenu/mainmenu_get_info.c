/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mainmenu_get_info.c
 * \brief    ��ȡID3��Ϣ��Ҫʹ�õĺ���
 * \author   zhangxs
 * \version  1.0
 * \date  2011/10/08
 *******************************************************************************/
#include  "main_menu.h"

//����׺������
#define MusicExtNum 11

//����const data ���к�׺����
const uint8 support_ext[][4] =
{
    "MP1", //mp3
    "MP2", //mp3
    "MP3", //mp3
    "WMA", //wma
    "APE", //ape
    "FLA", //flac
    "OGG", //ogg
    "AAC", //aa
    "AAX", //aax
    "M4A", //aax
    "AA " //aa
};

const id3_type_e support_id3_type[] =
{
    ID3_TYPE_MP3, //mp1
    ID3_TYPE_MP3, //mp2
    ID3_TYPE_MP3, //mp3
    ID3_TYPE_WMA, //wma
    ID3_TYPE_APE, //ape
    ID3_TYPE_FLAC, //fla
    ID3_TYPE_OGG, //ogg
    ID3_TYPE_AAC, //aac
    ID3_TYPE_AAX, //aax
    ID3_TYPE_AAC, //m4a
    ID3_TYPE_AA //aa
};

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_initid3buf(void)
 * \��ȡ��׺��
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      mainmenu_get_info.c
 * \note
 */
/*******************************************************************************/
void play_scene_initid3buf(void)
{
    //����buffer
    libc_memset(g_title_buf, 0, Id3BufSize);
    g_id3_info.tit2_buffer = g_title_buf;
    //����buffer�ͳ���
    g_id3_info.tpe1_buffer = NULL;
    //ר��buffer
    g_id3_info.talb_buffer = NULL;
    //����buffer
    g_id3_info.genre_buffer = NULL;
    //drm��buffer
    g_id3_info.drm_buffer = NULL;
    //����buffer
    g_id3_info.track_buffer = NULL;

    //�ļ����ⳤ��
    g_id3_info.tit2_length = Id3BufSize;
    //���߳���
    g_id3_info.tpe1_length = 0;
    //ר������
    g_id3_info.talb_length = 0;
    //���ɳ���
    g_id3_info.genre_length = 0;
    //drm����
    g_id3_info.drm_length = 0;
    //���쳤��
    g_id3_info.track_length = 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \id3_type_e play_scene_checktype(char * extname)
 * \��ȡ��׺��
 * \param[in]    extname��׺��buffer  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      mainmenu_get_info.c
 * \note
 */
/*******************************************************************************/
id3_type_e play_scene_checktype(uint8 * extname)
{
    uint8 cnt;
    id3_type_e ret;
    //ת����ǰ��׺��(ת�ɴ�д)
    extname[0] = ((extname[0] >= 'a') && (extname[0] <= 'z')) ? (extname[0] - 'a' + 'A') : extname[0];
    extname[1] = ((extname[1] >= 'a') && (extname[1] <= 'z')) ? (extname[1] - 'a' + 'A') : extname[1];
    extname[2] = ((extname[2] >= 'a') && (extname[2] <= 'z')) ? (extname[2] - 'a' + 'A') : extname[2];
    //�ȽϺ�׺
    for (cnt = 0; cnt < MusicExtNum; cnt++)
    {
        if (libc_strncmp(extname, support_ext[cnt], sizeof(support_ext[cnt])) == 0)
        {
            ret = support_id3_type[cnt];
            break;
        }
    }
    if (cnt == MusicExtNum)
    {
        ret = ID3_TYPE_END;
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void void play_scene_getid3(file_path_info_t* path_info)
 * \��ȡid3��Ϣ
 * \param[in]    path_info�ļ���·����Ϣ  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      mainmenu_get_info.c
 * \note
 */
/*******************************************************************************/
void play_scene_getid3(file_path_info_t* path_info)
{
    id3_type_e music_type;
    //bool have_no_id3 = FALSE;
    uint8* nameptr = path_info->file_path.dirlocation.filename;
    //��ʼ������
    play_scene_initid3buf();
    //��ȡ��ǰ�ļ�����
    music_type = play_scene_checktype(nameptr);
    //��λ���ļ�
    //fsel_set_location(&(path_info->file_path.plist_location), path_info->file_source);
    if (fsel_browser_set_file(&(path_info->file_path.plist_location), path_info->file_source) == FALSE)
    {
        return;
    }

    //��ȡid3��Ϣ
    get_id3_info(&g_id3_info, NULL, music_type);

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _get_music_info(void)
 * \��ȡid3��Ϣ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu_get_info.c
 * \note
 */
/*******************************************************************************/
void _get_music_info(void)
{
    //��ȡ�ļ�·��
    music_get_filepath(&g_file_path_info);
    
    //��ȡid3��Ϣ
    play_scene_getid3(&g_file_path_info); 	
}

