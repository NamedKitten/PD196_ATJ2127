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
#ifndef   _PICTURE_H
#define   _PICTURE_H

#include  "psp_includes.h"
#include  "case_include.h"
#include  "picture_res.h"
#include  "picture_sty.h"
#include  "mmm_id.h"

#define FOR_TEST
/*!����ͼƬӦ�õ�����ջ�����߳����ȼ�*/
#define    AP_PICTURE_STK_POS  AP_FRONT_LOW_STK_POS
#define    AP_PICTURE_PRIO     AP_FRONT_LOW_PRIO

/*! \brief Ӧ���Լ���Ҫ����ʱ������ */
#define APP_TIMER_COUNT		1
#define APP_TIMER_TOTAL     (COMMON_TIMER_COUNT + APP_TIMER_COUNT)

#define DISK_BASE           DISK_C

//#define AUTO_CHANGE_DISK    //�Ƿ��Զ��л��̷�

//playflag����
/*!ͼƬ��ת����*/
#define    PLAYFLAG_AUTO_ROTATION              0x00000001
/*!ͼƬ����ͼ����*/
#define    PLAYFLAG_PREVIEW_DECODE             0x00000002
/*!ͼƬ����������*/
#define    PLAYFLAG_BYPASS_MODE                0x00000004
/*!ͼƬ����֧����ʱ����*/
#define    PLAYFLAG_DELAY_MODE                 0x00000008

/*!ͼƬ����ת����*/
#define    PLAYFLAG_NO_ROTATION                0xfffffffe
/*!ͼƬ��ͼ����*/
#define    PLAYFLAG_NORMAL_DECODE              0xfffffffd
/*!ͼƬС��������*/
#define    PLAYFLAG_FULL_MODE                  0xfffffffb
/*!ͼƬ����ʱ����*/
#define    PLAYFLAG_NODELAY_MODE               0xfffffff7

//returnflag����
/*!ͼƬ�Ƿ�ѡ��*/
#define    RETURNFLAG_ROTATION                 0x00000001
/*!ͼƬ�Ƿ��֡*/
#define    RETURNFLAG_MULTIFRAME               0x00000002



#define VALUE_PREVIEW_MODULE_ON 0x03      //��ѯ����ͼ�Ƿ�  
#define VALUE_PREVIEW_ON        0x02      //������ͼ��
#define VALUE_PREVIEW_OFF       0xfd      //������ͼ��  
#define VALUE_PREVIEW_SUPPORT   0x01      //�Ƿ�֧������ͼ

//ͼƬ�л��ļ���־λ
#define up_picture_changenext   0x00
#define up_picture_changeprev   0x01
#define up_picture_changefile   0x02
#define up_picture_playnext     (up_picture_changefile | up_picture_changenext)
#define up_picture_playprev     (up_picture_changefile | up_picture_changeprev)
#define clr_picture_changefile 0xfd


/*����pictureӦ�õ�VMȫ�ֱ���*/
/*!
 * \brief
 *  picture��VMȫ�ֱ���.
 */
typedef struct
{
    /*!VM ����ħ��*/
    uint16 magic;
    /*!ͼƬ����ͼ���ܿ��� bit 0:�Ƿ�֧������ͼ bit1:����ͼ����*/
    uint8 thumbnail_flag;
    /*!ͼƬ�Զ���ת���ܿ��� 0:�� 1:��*/
    uint8 revolve_flag;
    /*!ͼƬ��չ����bit map*/
    uint32 file_ext_bitmap;
    /*ͼƬ�ļ��Ĳ���·����Ϣ*/
    file_path_info_t path;
    /*��һ�ν���ͼƬ��Ӧ��*/
    uint8 prev_enter_mode;
} g_picture_var_t;

/*�������˵��ĳ���*/
/*!
 *  \brief
 *  scene type
 *  \li SCENE_EXIT : �˳�
 *  \li SCENE_FILELIST : picture�ļ��б�
 *  \li SCENE_OPTIONMENU : �˵�ѡ���б���
 *  \li SCENE_PLAY : ���ų���
 */
typedef enum
{
    SCENE_EXIT = 0,
    SCENE_FILELIST,
    SCENE_OPTIONMENU,
    SCENE_PLAY,
    SCENE_PREVIEW,
    SCENE_MENU
} scene_picture_e;

/*!
 *  \brief
 *  ��������ʼ������״̬
 */
typedef enum
{
    DECODE_NO_ERR = 0,
    DECODE_LOAD_ERR,
    DECODE_OPEN_ERR,
    DECODE_FILE_ERR,
    DECODE_UNSUPPORT_ERR
}decode_status_e;


/*����picture������*/
/*!
 *  \brief
 */
typedef struct
{
    /*! �Ƿ�֧������ͼ��ʾ */
    bool thumbnail_support;
}picture_config_t;

/* ȫ�ֱ������� */

/*! \brief pictureӦ��VM����*/
extern g_picture_var_t g_picture_var;

/*! \brief ͨ��VM����*/
extern comval_t g_comval;

/*! \brief picture �����ļ�ϵͳ��mount ID*/
extern int32 picture_mount;

/*! \brief ����codec��ͼƬwindow */
extern wregion_t region_in;

/*! \brief ʵ�ʽ����õ�ͼƬwindow */
extern wregion_t region_out;

/*! \brief ͼƬ�����߳̾�� */
extern void *picture_handle;

/*! \brief ����codec��ͼƬ������Ϣ */
extern ColorLcmInfor g_picture_userinfor;

/*! \brief Ӧ�ý���ģʽ*/
extern app_param_e g_picture_enter_mode;

/*! \brief �Զ����Ż������ʾ�ļ����Ķ�ʱ��ID */
extern int8 timer_pic_play;

/*! \brief �Զ������л��ļ���־ */
extern uint8 g_picture_change_file;

/*! \brief �Զ����ű�־ */
extern bool  g_picture_auto_play;

/*! \brief �ļ�����Ҫ������ʾ��־ */
extern bool  g_picture_scroll_flag;

/*! \brief picture��һ������ */
extern scene_picture_e g_picture_scene_next;

/*! \brief picture��һ������*/
extern scene_picture_e g_picture_scene_prev;

/*! \brief ��Ҫ��������music�����־ */
extern bool g_need_resume_engine;

/*! \brief ͼƬ������Ƿ���б�־ */
extern bool g_mmm_id_free;

/*! \brief �ļ��������� */
extern uint8 g_picture_filename[LIST_ITEM_BUFFER_LEN];

/*! \brief �ļ�ѡ�����Ƿ��ʼ�� */
extern bool is_fsel_init;

/*! \brief �Ƿ�ر�music��̨ */
extern uint8 g_stop_music;

/*! \brief �ļ����� */
extern uint16 g_file_total;

/*! \brief �����ļ�����*/
extern uint16 g_file_err;

/*! \brief �Ƿ�ɾ���ļ�*/
extern uint8  g_del_file;


extern app_result_e pic_filelist(void);

extern app_result_e pic_option_menulist(void);

extern app_result_e pic_play(void);

extern app_result_e pic_menu(void);

extern app_result_e pic_msg_callback(private_msg_t *msg)
__FAR__;

extern bool pic_get_file_information(void);

extern bool pic_change_file(void);

extern bool pic_decode_init(uint8 mode) __FAR__;

extern app_result_e pic_play_proc_key_event(msg_apps_type_e gui_event);

extern decode_status_e pic_decode(void) __FAR__;

extern void pic_decoder_free(void) __FAR__;

extern bool pic_check_decode(void) __FAR__;

extern void music_close_engine(void) __FAR__;

extern bool music_resume_engine(void);

extern bool music_pause_engine(void);

extern fsel_error_e pic_fsel_initial(uint8 disk_flag);

extern app_result_e pic_disk_initial(void);

extern void auto_play_flag(void);

extern void scroll_play_flag(void);

extern app_result_e pic_data_init(void);

extern bool pic_check_engine(image_type_t image_type) __FAR__;

extern  decode_status_e _pic_set_file(void);

#endif

