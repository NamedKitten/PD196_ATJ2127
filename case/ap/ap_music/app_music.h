/********************************************************************************
 *                            Module: music_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-13 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_MUSIC_H__
#define __APP_MUSIC_H__

#include "psp_includes.h"
#include "case_include.h"	//display.h������common_ui.h��
#include "music_sty.h"
#include "music_res.h"
#include "music_common.h"

#include "setting_common.h"
#include "enhanced.h"

/*!��������Ӧ�õ�����ջ�����߳����ȼ�*/
#define    AP_MUSIC_STK_POS  AP_FRONT_LOW_STK_POS
#define    AP_MUSIC_PRIO     AP_FRONT_LOW_PRIO

//����music��������ղؼ�ʱ�Ķ�ʱ������ID
#define    APP_TIMER_TAG_FAVOR          0x01
#define    APP_TIMER_TAG_BOOKMARK       0x02

//����music�б��ӳ����������Ŀؼ�״̬��
#define    MUSIC_LISTSCENE_LIST         0x0
#define    MUSIC_LISTSCENE_MENU         0x1
#define    MUSIC_LISTSCENE_ALBUMLIST    0x2
#define    MUSIC_LISTSCENE_EXIT         0x3

//�������ģʽ
#define    MUSIC_BROWSER_ROOT_LAYER     0x0
#define    MUSIC_BROWSER_DEST_LAYER     0x1

//#define PRINT_BANK_INFO

//id3�ļ��� buffer����
#define Id3BufSIZE 92

//id3ר��/����buffer����
#define ArtBufSIZE 52

//����׺������
#define MusicExtNum 11

/*!
 * \brief
 *  ������ap����msuic��ģʽ
 */
typedef enum
{
    /*! ��������ѡ��music ͼ�� */
    ENTER_FROM_DESKTOP,
    /*! ��browserѡ����music�ļ� */
    ENTER_FROM_BROWSER,
    /*! ��������ѡ�������ڲ���ͼ�� */
    ENTER_FROM_PLAYNOW,
    /*! ������ap��option�˵�ѡ�������ڲ��� */
    ENTER_FROM_UI_MENU
} music_enter_mode_e;

/*!
 * \brief
 *  ����msuic���ų���ģʽ
 */
typedef enum
{
    /*! �����ļ���ʼ���� */
    PLAY_START,
    /*! audible�ϵ㲥�� */
    PLAY_AUDIBLE_RESUME,
    /*! ���ֲ���/��ͣ״̬ */
    PLAY_KEEP
} music_play_set_e;

/*!
 * \brief
 *  music_ui��������
 */
typedef enum
{
    /*! ���ų��� */
    SCENE_PLAYING,
    /*! �˵�/�б��� */
    SCENE_LSTMENU,
    /*! ���ó��� */
    SCENE_SETMENU,
    /*! �˳����� */
    SCENE_EXIT
} music_scene_e;

/*!
 * \brief
 *  ����ID3������������
 */
typedef struct
{
    /*! �����ַ������� */
    uint8 index;
    /*! �Ƿ�Ϊ��ʼ������ */
    uint8 init;
    /*! �Ƿ�ˢ����ʾ�����ڹ�����ʼ��ʱ��Ч */
    uint8 update;
    /*! �������� */
    uint8 direct;
} scroll_vars_t;



/*!
 * \brief
 *  music ui���ò���
 */
typedef struct
{
    /*! ħ�� */
    uint16 magic;
    /*! id3��ȡ���� */
    bool id3_support;
    /*! �����ʾ���� */
    bool lrc_support;
    /*! album art��ʾ���� */
    bool album_art_support;
    /*! �Ƿ����audible �ϵ� */
    uint8 audible_bk_flag;
    /*! �����ٶ�*/
    uint16 playspeed;
    /*! ab��������*/
    uint16 ab_count;
    /*! ab�������*/
    uint16 ab_gap;
    /*! audible���½�ģʽ */
    audible_switch_mode_e section_mode;
    /*! ��browserѡ����� */
    uint8 browser_sel;
    /*! ���볡�� */
    music_scene_e enter_scene;
    /*! ������������ */
    music_play_set_e play_set;
    /*! ��ר��ͼƬ�б���� */
    uint8 albumlist_sel;

    uint8 reserved[3];
} music_config_t;

/*!
 * \brief
 *  ��¼��ǰ�ļ�ѡ�����ĳ�ʼ������
 */
typedef struct
{
    uint8 disk_type;
    uint8 list_type;
} fsel_module_t;


//for globle variables

//globle variable
//��ŵ�ǰ��������
extern music_scene_e g_music_scene;

//����music�ķ�ʽ
extern app_param_e g_enter_mode;

//����play�Ĳ���
extern music_play_set_e g_play_set;

//��ʾ��ʶ
extern bool g_paint_flag;

//album art �����ʶ
extern uint8 g_album_art;

//��ȡ�ļ���Ϣ��ʶ bit0:��ȡ�ļ�ID3��Ϣ bit1:��ȡ�ļ�·����Ϣ
extern uint8 g_getfile_info;

//��ʴ��ڱ�ʶ
extern bool g_lyric_flag;

//���ſ��ƶ�ʱ��id
extern int8 time_frush_id;

//��¼������ʱ��״̬,�μ�id3_deal_status_e����
extern uint8 g_scroll_status;

//ID3������ʱ��
extern int8 scroll_frush_id;

//����Ƿ���ʱshuffle
extern uint8 g_shuffle_tmporary;

//��ǵ�ǰ�Ƿ���audible
extern uint8 g_audible_now;

//��־ab�����滻
extern bool g_ab_switch;

//��ǰ���ŵ��ļ�����
//extern uint16 g_file_total;

//��ǰ���ŵ��ļ����
//extern uint16 g_file_num;

extern musfile_cnt_t g_file;

//�������������
extern uint16 g_error_num;

//��ǰ��
extern uint16 g_switch_prev;

//���id3��Ϣ
extern id3_info_t g_id3_info;

//���⻺����
extern uint8 g_title_buf[Id3BufSIZE];

//���ֻ�����
extern uint8 g_artist_buf[ArtBufSIZE];

//ר��������
extern uint8 g_album_buf[ArtBufSIZE];

//��������
extern scroll_content_t g_scroll_content[3];

//�������Ʋ�������
extern scroll_vars_t g_scroll_vars;

//��ǰ���Ÿ������ļ��� �����album artʹ��
extern uint8 g_song_filename[64];

//ר��ͼƬ��ʾ��Ϣ
extern album_picinfo_t album_art_info;

//ˢ�±�־λ����
extern uint32 g_display_bitmap;

//�ļ�·����Դ��Ϣ���棬����ʱ����
extern file_path_info_t g_file_path_info;

//�����·����Ϣ�����ʱ��
extern file_path_info_t g_browser_filepath;

//����ļ�ϵͳid
extern int32 g_music_mount_id;

//��ǰ����״̬��Ϣ
extern mengine_status_t g_music_status;

//��ǰ����ʱ��ͱ�������Ϣ
extern mengine_playinfo_t g_play_info;

//audible�Ķϵ���Ϣ
extern mmm_mp_bp_info_t g_audible_bkinfo;

//music ui����������Ϣ
extern music_config_t g_music_config;

//ϵͳȫ�ֱ���
extern setting_comval_t g_setting_comval;

//��ѯ����״̬��־λ
extern uint8 g_check_mengine_status;

//��ʱ������
extern uint8 g_timer_counter;

//��ǰ������ʱ��
extern uint32 g_total_time;

//��ǰ����ʱ��
extern uint32 g_cur_time;

//ǰһ�μ�¼����ʱ��
extern uint32 g_prev_time;

//����nandflash����ʾ��VM����ַ�������������ʣ�Ϊ�����ַ
extern uint32 g_bookmark_fp;

//��������ʼ���
extern uint8 g_speed_counter;

//�����ʾ������ʾ��־λ
extern bool g_lyric_multi_screen;

//�ָ�Ĭ������eq��־
extern uint8 g_eq_restore;

//�Ƿ���Ҫ��PA��־
extern bool g_need_open_pa;

//����״̬���Ƿ���ȱ�־
extern bool g_scroll_deal;

//��ʱ��������ַָ�룬λ��lcd��1k buffer
extern uint8 *g_temp_buffer_p;

//�ļ�ѡ����ģ������
extern fsel_module_t g_fsel_module;

#ifdef PRINT_BANK_INFO
extern int print_label;
#endif

//for function scene deal

extern app_result_e music_scene_playing(app_param_e enter_mode, file_path_info_t* path_locat,
music_play_set_e play_set);

extern app_result_e music_scene_listmenu(bool menu_list, file_path_info_t* browser_path);

extern app_result_e music_scene_setmenu(bool music_audible);

//for function favorite

extern uint8 deal_file_name(uint8 *source_str, uint8 *dest_str);

extern void get_music_id3_title(uint8 *buffer, uint8 buffer_len, uint8 *ext_name);

extern app_result_e music_message_deal(private_msg_t *msg)
__FAR__;

extern bool music_file_init(file_path_info_t* init_path_locat);

extern bool music_open_engine(mengine_enter_mode_e start_mode);

extern bool music_close_engine(void);

//for function rcode
extern bool music_get_filepath(file_path_info_t *filepath)
__FAR__;

extern bool music_get_fileinfo(mmm_mp_file_info_t* fileinfo)
__FAR__;

extern bool music_get_status(mengine_status_t* play_status)
__FAR__;

extern bool music_get_playinfo(mengine_playinfo_t* play_info)
__FAR__;

extern bool music_get_bkinfo(mmm_mp_bp_info_t* curbk_info)
__FAR__;

extern bool music_get_playmode(uint8* playmode)
__FAR__;

extern bool music_play(void)
__FAR__;

extern bool music_send_msg(void *input_param, void *output_param, uint32 msg_type)
__FAR__;

extern bool music_set_eq(mmm_mp_eq_info_t *eq_val);

extern bool music_set_volume(uint8 volume);

//extern bool music_set_section_mode(uint8 section_mod) __FAR__;

extern bool music_set_playmode(uint8 playmode);

extern bool music_set_ab_count(uint16 count);

extern bool music_set_ab_gap(uint16 gaptime);

extern bool music_start_ab(void);

extern bool music_set_filepath(file_path_info_t *filepath)
__FAR__;

extern bool music_play_next(void)
__FAR__;

extern bool music_play_prev(void)
__FAR__;

extern bool music_play_bookmark(mmm_mp_bp_info_t *bookmark)
__FAR__;

extern bool music_delete_file(void)
__FAR__;

extern bool music_change_filepath(file_path_info_t *filepath)
__FAR__;

extern bool music_clear_error(void)
__FAR__;

extern app_result_e _scene_play_sysmsg(void)
__FAR__;

//extern app_result_e _scene_deal_guimsg(input_gui_msg_t *gui_msg);

extern void play_flag_update(void)
__FAR__;

extern bool check_bookmark_exist(void);

extern void music_load_sysfile(void);

extern app_result_e music_deal_sd_out(void)__FAR__;

extern void _scene_result_deal(app_result_e result_val);

extern bool music_get_section_info(musfile_cnt_t *file_info);

extern void _scene_play_seteq(uint8 eq_type);

extern bool music_standby_play(void);

#endif //__APP_MUSIC_H__

