/*******************************************************************************
 *                              ������д��Ŀ��
 *                            Module: ������дģ����
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
/*
 * \file     ��������ʵ��д�ļ�����
 * \brief    ������д�ļ��ĸ���
 * \author   ������д�ļ�������
 * \version 1.0
 * \date  2009/5/4
 *******************************************************************************/
#ifndef    _VIDEO_H
#define    _VIDEO_H

#include  "psp_includes.h"
#include  "case_include.h"
#include  "video_res.h"
#include  "video_sty.h"
#include  "mmm_vp.h"
#include  "video_display.h"
#include "audio_device.h"

/*!������ƵӦ�õ�����ջ�����߳����ȼ�*/
#define    AP_VIDEO_STK_POS   AP_FRONT_LOW_STK_POS
#define    AP_VIDEO_PRIO     AP_FRONT_LOW_PRIO
//#define PC
/*!����AP�ı��*/


/*��Ӧ���Լ���Ҫ����ʱ������*/
#define APP_TIMER_COUNT		2
#define APP_TIMER_TOTAL     (COMMON_TIMER_COUNT + APP_TIMER_COUNT)

#define DISK_BASE           DISK_C

//#define AUTO_CHANGE_DISK  //�Ƿ�֧���Զ��л��̷�

//��Ƶ��������ٶȶ���
#define VIDEO_FFB_LEVEL_0   1
#define VIDEO_FFB_LEVEL_1   5
#define VIDEO_FFB_LEVEL_2   9

//��Ƶˢ�¶���
#define VIDEO_PAINT_NONE     0   //��ˢ��
#define VIDEO_PAINT_PARTIAL  1   //����ˢ��
#define VIDEO_PAINT_ALL      2   //ȫˢ��
//��Ƶѭ��ģʽ����
#define VIDEO_REPEAT_NORMAL  FSEL_MODE_NORMAL
#define VIDEO_REPEAT_ONE     FSEL_MODE_LOOPONE
#define VIDEO_REPEAT_ALL     FSEL_MODE_LOOPALL

//��Ƶ���Ž����־λ����
#define    up_videoBackGDPic       0x00000001   //��ʾ��Ƶ���ű���
#define    up_videoheaderbar       0x00000002   //headerbar
#define    up_videofileinfor       0x00000004   //�ļ���
#define    up_videoplaystatus      0x00000008   //����״̬
#define    up_videoprogress        0x00000010   //������
#define    up_videotimeinfor       0x00000020   //ʱ�䣨��ǰʱ��+��ʱ�䣩
#define    up_videovolume          0x00000040   //������
#define    up_videovoladd          0x00000080   //����������
#define    up_videovolsub          0x00000100   //����������
#define    up_videospeed           0x00000200   //ˢ�¿�������ٶȱ�ʶ
#define    up_videofullscreen      0x00000400   //ȫ����ʶ
//---------------------------------------
#define    clr_videoBackGDPic      0xfffffffe
#define    clr_videoheaderbar      0xfffffffd
#define    clr_videofileinfor      0xfffffffb
#define    clr_videoplaystatus     0xfffffff7
#define    clr_videoprogress       0xffffffef
#define    clr_videotimeinfor      0xffffffdf
#define    clr_videovolume         0xffffffbf
#define    clr_videovoladd         0xffffff7f
#define    clr_videovolsub         0xfffffeff
#define    clr_videospeed          0xfffffdff
#define    clr_videofullscreen     0xfffffbff

/**********************����ȫ����ˢ�±�־λ********************/
#define video_set_full_flag()          \
{                                      \
    g_display_bitmap=g_display_bitmap|   \
                    up_videoBackGDPic |   \
                    up_videoheaderbar |   \
                    up_videofileinfor |   \
                    up_videoplaystatus | \
                    up_videoprogress |   \
                    up_videotimeinfor; \
}
/**********************����״̬������ˢ�±�־λ*****************/
#define video_set_playbar_flag()             \
{                                        \
    g_display_bitmap=g_display_bitmap|   \
                    up_videoheaderbar |   \
                    up_videofileinfor |   \
                    up_videoplaystatus |   \
                    up_videoprogress| \
                    up_videotimeinfor; \
}
/**********************��������������ˢ�±�־λ*****************/
#define video_set_volumebar_flag()             \
{                                        \
    g_display_bitmap=g_display_bitmap| \
                    up_videovolume; \
}

//��Ƶ�л��ļ���־λ
#define up_video_changenext   0x00
#define up_video_changeprev   0x01
#define up_video_changefile   0x02
#define up_video_playnext     (up_video_changefile | up_video_changenext)
#define up_video_playprev     (up_video_changefile | up_video_changeprev)
#define clr_video_changefile 0xfd

#define up_video_fullscreen_timer 0x01
#define up_video_filename_timer   0x02
#define up_video_progress_timer   0x04

#define clr_video_fullscreen_timer 0x08
#define clr_video_filename_timer   0x10
#define clr_video_progress_timer   0x20

/*�������˵�Ӧ�õ�VMȫ�ֱ���*/
/*!
 * \brief
 *  video��VMȫ�ֱ���.
 */
typedef struct
{
    /*! VM������ħ��ֵ*/
    uint16 magic;
    /*! ��һ�ν�����Ƶ��Ӧ��*/
    uint8 prev_enter_mode;
    /*! ����*/
    uint8 repeat_mode;
    /*! ��Ƶ��չ����bit map*/
    uint32 file_ext_bitmap;
    /*! ��Ƶ�ļ��Ĳ���·����Ϣ*/
    file_path_info_t path;
    /*! ��Ƶ���Ŷϵ���Ϣ*/
    videobpinfor_t break_point;
    /*! ��Ƶ���Ŷϵ�λ����Ϣ*/
    uint32 bk_cluster_no;
    uint32 bk_dir_entry;
} g_video_var_t;

/*�������˵��ĳ���*/
/*!
 *  \brief
 *  scene type
 *  \li SCENE_EXIT : �˳�
 *  \li SCENE_FILELIST : video�ļ��б�
 *  \li SCENE_OPTIONMENU : �˵�ѡ���б���
 *  \li SCENE_PLAY : ���ų���
 */
typedef enum
{
    SCENE_EXIT = 0, SCENE_FILELIST, SCENE_OPTIONMENU, SCENE_PLAY, SCENE_MENU
} scene_video_e;

/*���岥�ų�����ui��Ϣ�¼�*/
/*!
 *  \brief
 */
typedef enum
{
    VIDEO_VOLUMEBAR = 0, VIDEO_PROGRESSBAR, VIDEO_FULLSCREEN
} video_window_e;

typedef enum
{
    DECODE_NO_ERR = 0, DECODE_LOAD_ERR, DECODE_OPEN_ERR, DECODE_FILE_ERR
} decode_status_e;

/*������Ƶ����ģʽ*/
/*!
 *  \brief
 *  Ӧ�����Ͷ���
 *  \li VIDEO_PLAY :  ����״̬
 *  \li VIDEO_PAUSE : ��ͣ����״̬
 *  \li VIDEO_STOP :  ֹͣ����״̬
 *  \li VIDEO_STOP :  ���״̬
 *  \li VIDEO_STOP :  ����״̬
 */
typedef enum
{
    VIDEO_PLAY = 0, VIDEO_PAUSE, VIDEO_FF, VIDEO_FB, VIDEO_STOP
} video_play_status_e;

typedef struct
{
    uint16 avi_prog_y;
    uint16 amv_prog_y;
    uint16 avi_vol_y;
    uint16 amv_vol_y;
} adjust_region_t;

/*!�ⲿ��������*/

/*! \brief videoӦ��VM����*/
extern g_video_var_t g_video_var;

/*! \brief ͨ��VM����*/
extern comval_t g_comval;

/*! \brief video �����ļ�ϵͳ��mount ID*/
extern int32 video_mount;

/*! \brief ��Ƶ�ļ���ʱ��*/
extern uint32 g_total_time;

/*! \brief ����ˢ�½������Ķ�ʱ��ID��1s��ʱ*/
extern int8 timer_flush_progress; 

/*! \brief ���ڹ�����ʾ�ļ����Ķ�ʱ��ID��80ms��ʱ*/
extern int8 timer_flush_filename; 

/*! \brief ����ȫ��̬�봰��̬�л��Ķ�ʱ��ID, 5s��ʱ*/
extern int8 timer_flush_screen; 

/*! \brief ���ڿ��ƶ�ʱ���Ŀ�����ر� bit0: ȫ����ʱ�� bit1:�ļ�����ʱ�� bit2:��������ʱ��*/
extern uint8 g_video_timer_flag;

/*! \brief ˢ��λ��־����*/
extern uint32 g_display_bitmap;

/*! \brief videoӦ�ý���ģʽ*/
extern app_param_e g_video_enter_mode;

/*! \brief ��Ƶ������*/
extern void* g_video_handle;

/*! \brief ��Ƶ����״̬*/
extern video_play_status_e g_video_play_status;

/*! \brief ��Ƶ����״̬����*/
extern video_play_status_e g_video_play_status_backup;

/*! \brief ��Ƶ��ǰ����״̬*/
extern video_window_e g_video_cur_window;

/*! \brief ��Ƶ֮ǰ����״̬*/
extern video_window_e g_video_prev_window;

/*! \brief ˢ�±�־����ʶ�Ƿ�ˢ��*/
extern uint8 g_video_paint_flag;

/*! \brief Ӧ�û�ȡ��Ƶ������Ϣ�ı���*/
extern mmm_vp_userparam_t g_video_userinfor;

/*! \brief Ӧ�ÿ��������ڱ���*/
extern ctlinfor_t g_video_ctlinfor;

/*! \brief ����������*/
extern wregion_t g_region_fullscreen;

/*! \brief ʱ�������������*/
extern wregion_t g_region_progressbar;

/*! \brief ������������*/
extern wregion_t g_region_volumebar;

/*! \brief ���������������߶ȵ�������*/
extern adjust_region_t g_region_adjust;

/*! \brief �Զ������л��ļ���־ bit0: 0:����һ�� 1:����һ�� bit1:�Ƿ��л�����*/
extern uint8 g_video_change_file;

/*! \brief videoˢ��ģʽ */
extern uint8 g_video_draw_mode;

/*! \brief video��������ٶȿ��� */
extern uint8 g_video_speed_control;

/*! \brief videoʱ����� */
extern uint8 g_video_timer_counter;

/*! \brief �ļ��������� */
extern uint8 g_video_filename[LIST_ITEM_BUFFER_LEN];

/*! \brief �ļ����Ƿ������ʾ��־ */
extern bool need_scroll_filename;

/*! \brief videoҪ����ĳ���*/
extern scene_video_e g_video_scene_next;

/*! \brief video��һ�ε��ȵĳ���*/
extern scene_video_e g_video_scene_prev;

/*! \brief video�ļ���������*/
extern uint16 g_file_total;

/*! \brief video�����ļ�����*/
extern uint16 g_file_err;

/*! \brief video�Ƿ�ɾ���ļ�*/
extern uint8 g_del_file;

/*! \brief video���˵�ͷ��־*/
extern uint8 g_fast_back_end;

/*!�ⲿ��������*/

extern app_result_e video_menu(void);
extern app_result_e video_filelist(void);
extern app_result_e video_option_menulist(void);
extern app_result_e video_scene_playing(void);
extern app_result_e video_play(void)__FAR__;

extern app_result_e video_msg_callback(private_msg_t *msg)__FAR__;


extern void video_refresh_screen(uint8 mode)__FAR__;
extern bool video_get_file_information(void);
extern app_result_e video_playing_proc_gui_msg(input_gui_msg_t *ui_msg)__FAR__;

extern void filename_flag_update(void);
extern void play_flag_update(void);
extern void video_full_display(void);

extern void video_decoder_free(void) __FAR__;
extern bool video_change_file(void) __FAR__;
extern void video_clear_breakinfor(void);
extern decode_status_e video_start_decode(bool *need_free_mmm) __FAR__; 

extern void music_close_engine(void)
__FAR__;
extern void close_engine(void);
extern uint8 music_get_disk_type(void);
extern fsel_error_e video_fsel_initial(uint8 disk_flag);
extern app_result_e video_disk_initial(void);
extern app_result_e video_select_disk(void);

#endif

