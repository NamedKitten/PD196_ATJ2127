/********************************************************************************
 *                            Module: music_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-13 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_MUSIC_PLAYING_H__
#define __APP_MUSIC_PLAYING_H__

#include "psp_includes.h"
#include "case_include.h"
#include "music_common.h"

//play������ʾԪ�ر�ʶ
#define up_musicshuffle      0x00000800   //shuffle
#define up_musicprogress     0x00000400   //������
#define up_musicloopmode     0x00000200   //ѭ��ģʽ
#define up_musiceqmode       0x00000100   //eqģʽ
#define up_musictimeinfo     0x00000080   //����ʱ��
#define up_musicinformation  0x00000040   //id3��Ϣ
#define up_musicplaystatus   0x00000020   //����״̬
#define up_lyricflag         0x00000010   //���
#define up_musicalbumart     0x00000008   //ר��ͼƬ
#define up_musictracks       0x00000004   //��Ŀ��Ϣ(���������)
#define up_musicheadbar      0x00000002   //������
#define up_musicBackGDPic    0x00000001    //����
//-----------------------------���ˢ�±�־
#define clr_musicshuffle    0xfffff7ff    //���shuffle
#define clr_musicprogress    0xfffffbff   //���������
#define clr_musicloopmode    0xfffffdff   //���ѭ��ģʽ
#define clr_musiceqmode      0xfffffeff   //���eq��Ϣ
#define clr_musictimeinfo    0xffffff7f   //�������ʱ��
#define clr_musicinformation 0xffffffbf   //���id3��Ϣ
#define clr_musicplaystatus  0xffffffdf   //�������״̬��AB����
#define clr_lyricflag        0xffffffef   //��������Ϣ
#define clr_musicalbumart    0xfffffff7   //���ר����Ϣ
#define clr_musictracks      0xfffffffb   //�����Ŀ��Ϣ
#define clr_musicheadbar     0xfffffffd    //���������
#define clr_musicBackGDPic   0xfffffffe    //�������
/**********************����ˢ��ȫ�������********************/
#define SetFullInterface()             \
{                                      \
    g_display_bitmap=g_display_bitmap|   \
                    up_musicshuffle |   \
                    up_musicprogress |   \
                    up_musicloopmode |   \
                    up_musiceqmode |     \
                    up_musictimeinfo |   \
                    up_musicinformation| \
                    up_musicplaystatus | \
                    up_musicalbumart |   \
                    up_musictracks |     \
                    up_musicheadbar |    \
                    up_musicBackGDPic;   \
}

/**********************����ˢ���л������������*****************/
#define SetSwitchInterface()             \
{                                        \
    g_display_bitmap=g_display_bitmap|   \
                    up_musicshuffle |   \
                    up_musicprogress |   \
                    up_musictimeinfo |   \
                    up_musicinformation| \
                    up_musicplaystatus | \
                    up_musicalbumart |   \
                    up_musictracks;    \
}

//��ȡ�ļ���Ϣ��־λ
#define GET_FILEINFO_ID3       0x01
#define GET_FILEINFO_FILEPATH  0x02
#define CLR_FILEINFO_ID3       0xfe
#define CLR_FILEINFO_FILEPATH  0xfd


typedef enum
{
    Music_NON,
    Music_MP1,
    Music_MP2,
    Music_MP3,
    Music_WMA,
    Music_WMV,
    Music_AIF,
    Music_APE,
    Music_ASF,
    Music_FLA,
    Music_OGG,
    Music_MPC,
    Music_M4A,
    Music_AAC,
    Music_AA
} music_type_e;

typedef enum
{
    ID3_DEAL_INIT,
    ID3_DEAL_SCROLL,
    ID3_DEAL_SWITCH
} id3_deal_status_e;

//for functions

extern void play_scene_getid3(file_path_info_t* path_info);

extern id3_type_e play_scene_checktype(uint8 * extname);

extern void play_scene_paint(uint32 *display_flag);

extern app_result_e _scene_play_guimsg(input_gui_msg_t *gui_msg);

extern app_result_e play_key_prev(mengine_status_t* cur_status);

extern app_result_e play_key_next(mengine_status_t* cur_status);

extern app_result_e play_key_play(mengine_status_t* cur_status);

extern app_result_e play_key_return(void);

extern void play_scroll_init(void) __FAR__;

extern bool play_scroll_deal(void) __FAR__;

extern void id3_scroll_deal(void) __FAR__;

extern void play_scroll_update(void) __FAR__;

//extern void play_scroll_restart(void);

//extern void _show_now_time(uint8 para, style_infor_t * style);

//extern void _show_progress(uint8 para, style_infor_t * style);

extern void _show_time_progress(uint8 para, style_infor_t * style, uint8 type);

extern app_result_e _playing_loop_deal(file_path_info_t* path_locat);

extern app_result_e play_eror_deal(play_status_e status_prev);

extern void music_playlist_update_history(file_path_info_t* path_locat, uint8 dest_type) __FAR__;

extern void play_paint_albumart(album_picinfo_t *album_picinfo);
#endif //__APP_MUSIC_PLAYING_H__
