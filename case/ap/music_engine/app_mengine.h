/********************************************************************************
 *                            Module: music_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-05 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_MENGINE_H__
#define __APP_MENGINE_H__

#include "psp_includes.h"
#include "case_include.h"
#include "enhanced.h"

#include "music_common.h"
#include "setting_common.h"

/*!�����������������ջ�����߳����ȼ�*/
#define    AP_MENGINE_STK_POS  AP_BACK_LOW_STK_POS
#define    AP_MENGINE_PRIO     AP_BACK_LOW_PRIO

#define INTRO_INTERVAL 11000

//���ֲ���ģʽ
typedef enum
{
    //��������
    PLAY_NORMAL,
    //����
    PLAY_RESUME,
    //�������
    PLAY_FFON,
    //���˲���
    PLAY_FBON,
    //����Ҫ����
    PLAY_NO_PLAY
} play_mode_e;

//����ֹͣģʽ
typedef enum
{
    //����ֹͣ
    STOP_NORMAL,
    //��ͣ
    STOP_PAUSE
} stop_mode_e;

typedef enum
{
    NORMAL_SWITCH_NEXT = 0x0,
    NORMAL_SWITCH_PREV = 0x1,
    FORCE_SWITCH_NEXT  = 0x2,
    FORCE_SWITCH_PREV  = 0x3,
    END_SWITCH         = 0x4
} switch_mode_e;

typedef enum
{
    SWITCH_NO_ERR,
    SWITCH_ERR_STOP,
    SWITCH_ERR_SWITCH,
    SWITCH_ERR_PLAY
} switch_result_e;

//for audible

typedef struct
{
    // С�����к�
    unsigned char DeviceId_[20];
    //�����ļ�,sys�ļ�����
    unsigned char ActivationRecord_[560];
    //pos�ļ�����
    unsigned char playbackPosFilebuffer[16];
    //�ܵ��½���Ŀ
    unsigned int m_cChapters_;
    // �½�ʱ��
    unsigned int SongChapters_[200];

    //�ܵ�ͼƬ��Ŀ
    unsigned int m_cImages_;
    //ͼƬʱ��
    unsigned int imageTime_[200];
    //ͼƬλ��
    unsigned int imagePos_[200];
    // ͼƬ��С
    unsigned int imageSize_[200];

    //�½��ı�λ��
    unsigned int textPos_[200];

} audible_para_t;


/*!
 * \brief
 *  ������Ϣ�ṹ���������
 */ 
typedef struct
{
    /******************************************************************
     *          ����״̬��Ϣ
     *******************************************************************/
    mengine_status_t eg_status;

    /******************************************************************
     *          �����ļ���ʱ���bit����Ϣ
     *******************************************************************/
    mengine_playinfo_t eg_playinfo;

    /******************************************************************
     *          �м�������ļ���Ϣ
     *******************************************************************/
    mmm_mp_file_info_t eg_file_info;

    /******************************************************************

     *          ������Ϣ(��������)
     *******************************************************************/
    mengine_config_t eg_config;

} mengine_info_t;

//globle variable
//�����м�����
extern void *mp_handle;

//����ļ�ϵͳid
extern int32 file_sys_id;

//����ģʽ
extern mengine_enter_mode_e mengine_enter_mode;

//������Ϣ
extern mengine_info_t mengine_info;

//���������ʼʱ��
extern uint32 g_intro_start;

//���music�ļ���׺����bitmap����
extern uint32 file_type_cfg;//

//ɾ����ǰ�ļ���ʶ
extern uint32 g_del_curfile;

//�ı��ļ�·����Ϣ��ʶ
extern uint32 g_change_path_flag;

//��ʶaudbile�ļ�
extern uint16 g_is_audible;

//audible �½��л���ʱ��
extern int8 g_chapter_timer;

//audible ͼƬ��ⶨʱ��
extern int8 g_image_timer;

//pos file buffer
extern uint8 g_audible_pos[16];

//�ļ���buffer
extern uint8 g_file_name[64];

//audbile info ptr
extern audible_para_t *g_audible_info_p;

//sent to decoder audbile info ptr
extern audible_para_t *g_send_audible_p;

//sd �ļ���
//extern uint8 g_sd_file_name[12];

//�ļ�·����Դ��Ϣ����,�ı�·����Ϣʱʹ��
extern fsel_type_e g_path_type;

//ϵͳȫ�ֱ���
//extern setting_comval_t g_setting_comval;
//extern comval_t g_comval;

//�м��״̬��ⶨʱ��
extern int8 g_status_timer;

//�м��״̬����־λ
extern uint8 g_check_status_flag;

//music engine��ʱ����
extern uint8 g_delay_time;

//audibleר��ͼƬ��Ŀ
extern uint16 g_image_num;

//�ؼ�����ֵ
extern app_result_e g_mengine_result;

//�������ñ����ṹ��ָ��
extern mengine_config_t *g_eg_cfg_p;

//����״̬�����ṹ��ָ��
extern mengine_status_t *g_eg_status_p;

//���沥����Ϣ�ṹ��ָ��
extern mengine_playinfo_t *g_eg_playinfo_p;

//˽����Ϣȫ�ֱ���
extern private_msg_t g_cur_msg;


//functions

extern app_result_e mengine_control_block(void)
__FAR__;

extern bool change_locat_deal(void)
__FAR__;

extern app_result_e mengine_message_done(private_msg_t* cur_msg)
__FAR__;

extern bool mengine_file_init(void)
__FAR__;

//extern void _end_switch(play_mode_e last_mode)
//__FAR__;

extern bool _set_file(void)
__FAR__;

extern bool _play(play_mode_e play_mode)
__FAR__;

extern bool _stop(stop_mode_e stop_mode)
__FAR__;

//extern bool _next(bool force_mod)
//__FAR__;

//extern bool _prev(bool force_mod)
//__FAR__;

extern eg_err_e _convert_error_no(mmm_mp_err_t err_no)
__FAR__;

extern void _error_timeout_handle(void)
__FAR__;

//for audible

extern void _deal_audible_posfile(uint8 mode)
__FAR__;

extern void _deal_audible_sysfile(void)
__FAR__;

extern void _clr_chapter_timer(void)
__FAR__;

extern void _clr_imag_timer(void)
__FAR__;

extern void _set_chapter_timer(void)
__FAR__;

extern void _set_imag_timer(void)
__FAR__;

extern void _check_audible_chapter(void)
__FAR__;

extern void _check_audible_image(void)
__FAR__;

extern void _adjust_big_little_endian(uint8 *data_address, uint8 data_length);

extern void _check_pos_time(void)
__FAR__;

extern void _chapter_switch_force(bool next_prev)
__FAR__;

extern app_result_e mengine_audible_getimage(void* msg_ptr)
__FAR__;

extern uint32 _volume_deal(uint8 vol_in, bool set_mode)
__FAR__;

extern void mengine_save_errno_no(void)
__FAR__;
//for event
//extern app_result_e mengine_get_config(void* msg_ptr) __FAR__;

extern app_result_e mengine_get_playmode(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_filepath(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_fileinfo(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_status(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_playinfo(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_bkinfo(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_eq(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_volume(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_section_mode(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_playmode(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_ab_count(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_ab_gap(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_ab_gap(void* msg_ptr)
__FAR__;

extern app_result_e mengine_start_ab(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_apoint(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_bpoint(void* msg_ptr)
__FAR__;

extern app_result_e mengine_clear_ab(void* msg_ptr)
__FAR__;

extern app_result_e mengine_play(void* msg_ptr)
__FAR__;

extern app_result_e mengine_stop(void* msg_ptr)
__FAR__;

extern app_result_e mengine_pause(void* msg_ptr)
__FAR__;

extern app_result_e mengine_resume(void* msg_ptr)
__FAR__;

extern app_result_e mengine_fast_forward(void* msg_ptr)
__FAR__;

extern app_result_e mengine_fast_backward(void* msg_ptr)
__FAR__;

extern app_result_e mengine_fastplay_step(void *msg_ptr);

extern app_result_e mengine_cancel_ffb(void* msg_ptr)
__FAR__;

extern app_result_e mengine_play_next(void* msg_ptr)
__FAR__;

extern app_result_e mengine_play_prev(void* msg_ptr)
__FAR__;

extern app_result_e mengine_play_bookmark(void* msg_ptr)
__FAR__;

extern app_result_e mengine_musui_delete_file(void* msg_ptr)
__FAR__;

extern app_result_e mengine_other_delete_file(void* msg_ptr)
__FAR__;

extern app_result_e mengine_filepath_deal(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_filepath(void* msg_ptr)
__FAR__;

extern app_result_e mengine_change_filepath(void* msg_ptr)
__FAR__;

extern app_result_e mengine_del_favorite_deal(void* msg_ptr)
__FAR__;

extern app_result_e mengine_add_favorite_deal(void* msg_ptr)
__FAR__;

extern app_result_e mengine_clear_error(void* msg_ptr)
__FAR__;

extern app_result_e mengine_chapter_next(void* msg_ptr)
__FAR__;

extern app_result_e mengine_chapter_prev(void* msg_ptr)
__FAR__;

extern app_result_e mengine_audible_bookmark(void* msg_ptr)
__FAR__;

extern app_result_e mengine_message_done_bank(private_msg_t* cur_msg)
__FAR__;

extern void mengine_set_real_eq(uint8 mode)
__FAR__;

extern app_result_e mengine_get_engine_info(void* msg_ptr)
__FAR__;

extern void mengine_reply_msg(void* msg_ptr, bool ret_vals)
__FAR__;

extern app_result_e mengine_audible_get_section_info(void* msg_ptr);

extern app_result_e mengine_standby_play(void* msg_ptr);

extern switch_result_e mengine_file_switch(stop_mode_e stop_mode, switch_mode_e force_mode, play_mode_e play_mode)
__FAR__;

extern void mengine_save_audible_breakinfo(void) __FAR__;

extern bool _file_switch_info(void);

#endif //__APP_MENGINE_H__
