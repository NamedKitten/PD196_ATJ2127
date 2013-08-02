/********************************************************************************
 *                            Module: radio_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang    2011-09-13 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_RADIO_H__
#define __APP_RADIO_H__

#include "psp_includes.h"
#include "case_include.h"
#include "radio_sty.h"
#include "radio_res.h"
#include "setting_common.h"
#include "fmengine.h"

/*!����FM  Ӧ�õ�����ջ�����߳����ȼ�*/
#define    AP_RADIO_STK_POS  AP_FRONT_LOW_STK_POS
#define    AP_RADIO_PRIO     AP_FRONT_LOW_PRIO

/* ����Ԥ���̨�б�����֧�ֵı����̨�����迪����Ӧ��Сbuffer*/
#define    MAX_STATION_COUNT    30

/* �����������СƵ��*/
#define    MIN_US     0x55cc                //87500 = 0x155cc
#define    MIN_JP      0x28E0               //76000 = 0x128E0
#define    MIN_EU      0x55cc               //87500 = 0x155cc
#define    MIN_USER    0x55cc   //0x28E0             //�û���̨��СƵ��87.5MHz
/* ������������Ƶ��*/
#define    MAX_US         0xa5e0                //108000
#define    MAX_JP         0x5f90                //90000
#define    MAX_EU         0xa5e0               //108000
#define    MAX_USER      0xa5e0              //�û���̨���Ƶ��108MHz
/* �����û���̨�б��ļ���ģʽ*/
#define FS_OPEN_NORMAL		0x01	//����seek
#define FS_OPEN_NOSEEK		0x00	//������seek
#define FS_OPEN_MODIFY		0x02    //֧��׷��д
/* �����ļ�������������С*/
#define SECTOR_SIZE     512

/* ����dialog ѡ����ѡ��*/
#define  BUTTON_YESNO_YES     0
#define  BUTTON_YESNO_NO       1

/* �û��б��ļ���ÿ��Ƶ���ִ����ֽڳ��ȣ���093900*/
#define FREQ_LENTH 6
/* �û��б��ļ���ÿ����̨���ִ����ֽڳ��ȣ���CH01*/
#define STANUM_LENTH   4

/* ��̨�б���ÿ���б���ȣ���05   FM  107.10 */
#define LIST_STR_LENGTH    16

/*���岥�ų���ˢ��bitmap */
#define PAINT_ALL                           0xff
#define PAINT_VOL                           0x01
#define PAINT_FREQ                         0x02
#define PAINT_STATION_INFOR        0x04
#define CLEAR_PAINT_ALL                0x7f
#define CLEAR_PAINT_VOL                0xfe
#define CLEAR_PAINT_FREQ              0xfd
#define CLEAR_PAINT_INFOR            0xfb

/*����play  ����Ƶ��ͼƬ����*/
#define PIC_X_F1START       29                           //����λ��Ϊ2  λʱ����ʼx  ����
#define PIC_X_F1START_1    24                          //����λ��Ϊ3  λʱ����ʼx  ����
#define PIC_Y_F1START       43

/* ����Ƶ������ͼƬ֮�����������ֵ*/
#define PIC_X_INC1      11
#define PIC_X_INC2      11
#define PIC_X_INC3      12
#define PIC_X_INC4      5
#define PIC_X_INC5      11

/*������״Ƶ�������*/
#define FREQ_BAR_HEIGHT     103
#define FREQ_BAR_X           8

/* ֧�ֵ�Ӧ������*/
#define    MAX_APP_NUM    11

/* ʹ�ö��������ļ�*/
//#define    USE_83NAME_CREAT

//����radio ���Ž���ģʽ���������˳�ʱ�ķ��س���
typedef enum
{
    /*  radio ���˵�ѡ���ֶ���г�����, ����radio ���˵�*/
    ENTER_PLAY_FROM_MAIN = 1,
    /* ��̨�б�ѡ��Ƶ�ʲ��Ž��룬���ص�̨�б�*/
    ENTER_PLAY_FROM_LIST,
    /* ���ڲ��ţ���һ�β��Ž��룬����mainmenu Ӧ��*/
    ENTER_PLAY_FROM_NOWPLAY,
} radio_playwin_mode_e;

//radio AP��������
typedef enum
{
    /* FM ������˵�����*/
    FM_SCENE_MAINMENU = 1,
    /* FM Ԥ���̨���û���̨�����������֧��30����̨��*/
    FM_SCENE_STALIST,
    /* FM ���ų���*/
    FM_SCENE_PLAYING,
    /* FM �Ӳ˵�����������FM���Ž���͵�̨�б����Ӳ˵� */
    FM_SCENE_OPTMENU,
    /* �����˳�*/
    FM_SCENE_EXIT,
} radio_scene_e;

//radio AP option�ӳ�������
typedef enum
{
    /* ��ʼ����δ����option */
    NO_ENTER_SUB = 0,
    /* ��̨�б��µ�option �Ӳ˵�*/
    SUBS_OPT_STATION,
    /* ���Ž����µ�option �Ӳ˵�*/
    SUBS_OPT_PLAY
} radio_opt_subscene_e;

//radio AP Ԥ���̨�б�Ľ���ģʽ
typedef enum
{
    /* ����������ʾ��ȷ����ʼ����*/
    ENTER_NORMAL = 1,
    /* ������룬ȷ����ѡ�񱣴�λ��*/
    ENTER_ADD_STATION,
    /* ɾ�����룬ȷ����ѡ����ɾ����̨*/
    ENTER_DEL_STATION
} radio_list_entry_e;

//��̨�б���ʾģʽ
typedef enum
{
    /* Ԥ���̨�б�*/
    STATIONLIST_SAVE = 0,
    /* �û���̨�б�*/
    STATIONLIST_USER,
} radio_listtype_e;

//�û���̨�б�Ľ���ģʽ
typedef enum
{
    /* ����Ƶ�ʽ����������û���̨�б���ʾ*/
    ALL_FREQ = 1,
    /* ���ļ�ͷ��ʼ��ָ�������ŵĵ�̨����*/
    INDEX_FROM_START,
    /* ��ǰ��̨����һ����̨���������������ڲ�ʹ��*/
    NEXT_FROM_CUR,
} userlist_parse_e;

//�û���̨�б�ı��뷽ʽ
typedef enum
{
    /* ��������������*/
    ENCODE_MUL = 1,
    /* unicode ����*/
    ENCODE_UNICODE,
} userlist_encode_e;

//����voice �����
typedef enum
{
    //Ĭ�ϳ�ʼ״̬��δ����voice
    NO_ERTER_VOICE = 0,
    //��radio ���˵�ѡ��FM ¼�ƽ���voice
    FROM_RADIO_MAINMENU,
    //��radio ��option �Ӳ˵�ѡ��FM ¼�ƽ���voice
    FROM_OPTION_MENU
} enter_voice_mode_e;

//radio ui���ò���
typedef struct
{
    /* ħ����*/
    uint16 magic;
    /* Ԥ���б�( ��ͨƵ��)*/
    uint16 fmstation_us[MAX_STATION_COUNT];
    /* Ԥ���б�( �ձ�Ƶ��)*/
    uint16 fmstation_jp[MAX_STATION_COUNT];
    /* Ԥ���б�( ŷ��Ƶ��)*/
    uint16 fmstation_eu[MAX_STATION_COUNT];
    /* ��ǰ���ŵĵ�̨��Ϣ*/
    fm_play_status_t FMStatus;
    /* ��ǰ���Ų���, Band_MAX ��ʾ��ǰ����Ϊ�û���̨*/
    radio_band_e band_mode;
    /* �����������ڣ������˷��ص�radioUI �����*/
    enter_voice_mode_e enter_voice;
    /* ����ģ����õ���̨ģʽ( Ĭ��ģʽ��ͨ������������)	*/
    FM_SeekMode_e seek_mode;
} radio_config_t;

/* radio �û���̨�ṹ*/
typedef struct
{
    /* �û���̨�б�Ƶ�㣬�����±�Ϊ��̨��-1*/
    uint16 fmstation_user[MAX_STATION_COUNT];
    /* ��ǰ���ŵ��û���̨���� */
    char station_name[40];
} radio_userlist_t;

/* ��̨�б���ƽṹ*/
typedef struct
{
    /*! ��ǰ�б�ÿҳ���� */
    uint8 one_page_count;
    /*! ��ǰ�б������� */
    uint8 total;
    /*! ��ǰ��ʾ���б�������� */
    uint8 top;
    /*! ��ǰ��ʾ���б�β����� */
    uint8 bottom;
    /*! ��ǰ������λ�� */
    uint8 list_no;
    /*! ��ǰ���������� */
    uint8 active;
    /*! ֮ǰ���������� */
    uint8 old;
    uint8 reserved;
    /*! �б������ģʽ���μ� list_update_mode_e ���� */
    list_update_mode_e update_mode;
    /*! �б�UI����ģʽ���μ� list_draw_mode_e ���� */
    list_draw_mode_e draw_mode;
} radiolist_control_t;

//add by liuwzh
//for alarm_fm
/* radio����ģʽ */
extern uint8 g_radio_from_alarm;
/* alarm_fm�Ľ���ʱ�� */
extern time_t alarmfm_end_time; 
/* alarm_fmƵ�� */
//uint16 alarmfm_frequency;
/* alarm_fm���� */
extern uint8 alarmfm_volume;

//ȫ�ֱ�������
extern radio_scene_e g_radio_scene;
extern radio_config_t g_radio_config;
extern uint16 *pfreq;
extern uint16 *pfreq_list;
extern bool first_wait;
extern uint8 ui_icon_count;
extern radio_band_e g_menu_band;
extern uint16 Auto_tab[MAX_STATION_COUNT];
extern int8 timer_auto_seek;
extern int8 timer_flush_radioinfo;
extern Engine_Status_t g_engine_status;
extern comval_t g_comval;

extern const uint16 max_freq[4];
extern const uint16 min_freq[4];
extern uint8 g_radiolist_item[LIST_NUM_ONE_PAGE_MAX][LIST_STR_LENGTH];
extern uint8 Freqtab[7];

extern radio_listtype_e g_radio_listtype;
extern radio_list_entry_e g_stalist_entry;
extern radio_playwin_mode_e g_playwin_mode;
extern radio_opt_subscene_e g_option_type;
extern uint8 from_where;
extern bool need_reinstall;
extern bool need_set_freq;

extern uint8 paint_flag;
extern uint8 FMFreNumOld;
extern uint16 startfreq;
extern listbox_private_t radiolist_private;
extern radiolist_control_t list_control;

extern uint8 g_userlist_index;
extern uint8 g_userlist_total;
extern userlist_encode_e encode_mode;
extern radio_userlist_t g_user_station;
extern const char default_name[7];
extern const uint16 default_freq[3];
extern bool g_use_default;
extern uint8 g_userlist_table[MAX_STATION_COUNT];

//for user station parse
extern uint32 file_total_byte;
extern char drv_type;
extern uint32 vfs_mount_radio;
extern handle fp_radio;
extern uint8 g_first_open_engine;

//��������
extern bool radio_open_engine(void)
__FAR__;
extern bool radio_get_modual_status(Module_Status_e* status)
__FAR__;
extern bool radio_set_mute(FM_MUTE_e mode)
__FAR__;
extern bool radio_modual_init(void)
__FAR__;
extern app_result_e show_dialog(uint16 id, uint8 dialog_type, uint8 default_icon)
__FAR__;
extern app_result_e radio_app_msg_callback(private_msg_t *pri_msg)
__FAR__;
extern bool radio_set_band(radio_band_e mode);
extern bool radio_set_freq(uint16 freq)
__FAR__;
extern bool radio_set_hardseek(uint16 freq, uint8 dir)
__FAR__;
extern bool radio_set_softseek(uint16 freq, uint8 dir)
__FAR__;
extern bool radio_get_seekflag(void)
__FAR__;
extern bool radio_get_freq(void)
__FAR__;
extern bool radio_get_antenna(void)
__FAR__;	
extern bool radio_get_vol_status(void);
extern bool radio_break_hardseek(void)
__FAR__;
extern app_result_e deal_soft_autoseek(FM_SeekDir_e dir);
extern app_result_e deal_hard_autoseek(FM_SeekDir_e dir);

extern void init_point_freq(void)
__FAR__;
extern uint8 get_index_from_num(uint8 num)
__FAR__;
extern uint8 get_station_count(void)
__FAR__;
extern bool freq_change(uint8* buf, uint16 freq, uint8 mode);
extern uint8 whether_in_list(uint16 freq, uint16 *p_freq)
__FAR__;
extern uint8 get_real_station_num(uint8 num, FM_SeekDir_e dir)
__FAR__;

extern app_result_e show_saved_station(radio_list_entry_e entry_mode, uint8 from);
extern app_result_e show_user_station(void);
extern bool radio_parse_userlist(char* station_name, userlist_parse_e mode, uint8 num);

extern app_result_e radioUI_mainmenu(void);
extern app_result_e radioUI_scene_playing(radio_playwin_mode_e mode);
extern app_result_e radioUI_scene_stationlist(radio_listtype_e list_type, radio_list_entry_e entry_mode, uint8 from);
extern app_result_e radioUI_scene_optionlist(radio_opt_subscene_e mode);
extern app_result_e radio_AutoSearch_scene(FM_SeekMode_e mode, FM_SeekDir_e dir);
extern void clear_preset_list(radio_band_e band);

extern bool radio_engine_init(void);
extern void show_freq_in_play(void);
extern void show_station_num(void);
extern app_result_e scene_play_sysmsg(void);
extern void alarmfm_in_playing(void)
__FAR__;
#endif //__APP_RADIO_H__
