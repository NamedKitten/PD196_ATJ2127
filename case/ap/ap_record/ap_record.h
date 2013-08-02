/********************************************************************************
 *                            Module: music_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      leiming    2011-10-14          1.0             build this file
 ********************************************************************************/

#ifndef _AP_RECORD_H_
#define _AP_RECORD_H_

#include "psp_includes.h"
#include "case_include.h"       //display.h������common_ui.h��
#include "enhanced.h"
#include "Mmm_mr.h"

#include "setting_common.h"
#include "record_res.h"
#include "record_sty.h"

//record debug macro

/*!������ƵӦ�õ�����ջ�����߳����ȼ�*/
#define     AP_RECORD_STK_POS     AP_FRONT_LOW_STK_POS
#define     AP_RECORD_PRIO        AP_FRONT_LOW_PRIO

//#define     FILE_NUM_MAX          999       //¼���ļ�������
#define     FILE_NUM_MAX          200       //¼���ļ�������
#define     REC_SPACE_LOW         200        //���̿ռ�����С����
//#define     SETDISKOKFLAG_DEF     0x01

#define     REC_SCENE_TIMER_INTERVAL  125   //¼��timer��ʱʱ����ms
#define     WAV_WRITE_BACK_TIMES      300   //¼��������д��wav��ʱдvram����ʱ������ֵ
#define     WAV_FILE_SIZE_MAX       (0x400000-1)	//wav�ļ�size���Ƶ�2G��sectors
#define     MP3_FILE_SIZE_MAX	    (0x400000-1)	//mp3�ļ�size���Ƶ�2G��sectors
#define     MAGIC_RECORD            0x00   //¼��ȫ�ֱ���ħ����

#define     VM_AP_RECORD_WAV	    VM_AP_RECORD+0x200  //�ϵ�ָ������ݼ�¼��ַ

#define     NR_LEVEL_SHIFT 0            //���뼶��Ϊ1-6��Խ��Խ�������Ƽ�ֵ3
#define     RESERVE_LEVEL_SHIFT 4       //��������
#define     AGC_LEVEL_SHIFT 8           //AGC����Ϊ1-16����λ1000��Խ������Խ�죬�Ƽ�ֵ8��
#define     STABLE_PERIOD_SHIFT 16      //1-255, Խ��Խ�ʺ��ȶ�������������֮ԽСԽ�ʺϱ仯��Ļ������Ƽ�ֵ100
#define     DENOISELEVEL(a,b,c,d)	\
            (((a) << NR_LEVEL_SHIFT) | ((b) << RESERVE_LEVEL_SHIFT) \
            | ((c) << AGC_LEVEL_SHIFT) | ((d) << STABLE_PERIOD_SHIFT))

/**********record������ʾԪ�ر�ʶ****************/
#define up_recordPauseIcon      0x00000400    //���¼��pause icon
#define up_recordProgress       0x00000200    //¼����������ʾ
#define up_recordTime           0x00000020    //¼��now time
#define up_recordFMFreq         0x00000010    //FMƵ����ʾtxt
#define up_recordFileTxt        0x00000008    //�ļ�����ʾtxt
#define up_recordPlayIcon       0x00000004    //¼��״̬��ʾͼ��
#define up_recordHeadBar        0x00000002    //��ʾ¼��������
#define up_recordBackGDPic      0x00000001    //����
//-----------------------------���ˢ�±�־
#define clr_recordPauseIcon     0xfffffbff    //���¼��pause icon
#define clr_recordProgress      0xfffffdff    //���¼����������ʾ
#define clr_recordTime          0xffffffdf    //���¼��now time
#define clr_recordFMFreq        0xffffffef    //���FMƵ����ʾtxt
#define clr_recordFileTxt       0xfffffff7    //����ļ�����ʾtxt
#define clr_recordPlayIcon      0xfffffffb    //���¼��״̬��ʾͼ��
#define clr_recordHeadBar       0xfffffffd    //���¼��������
#define clr_recordBackGDPic     0xfffffffe    //�������
//********************************************************************************

#define SetFullInterface()             \
{                                      \
    g_scene_param.display_bitmap=g_scene_param.display_bitmap|   \
                    up_recordBackGDPic |   \
                    up_recordHeadBar |   \
                    up_recordPlayIcon |   \
                    up_recordFileTxt |     \
                    up_recordFMFreq |   \
                    up_recordTime| \
                    up_recordProgress; \
}

/*¼��������Ϣ*/
typedef enum
{
    /*¼������ : ¼������*/
    SCENE_UI_REC,
    /*¼������ : ¼���˵�*/
    SCENE_MENU_REC,
    /*¼������ : �˳�����*/
    SCENE_EXIT,
    /*¼������ : ����*/
    SCENE_MAX
} record_scene_e;

/*¼��״̬ö��*/
typedef enum
{
    /*¼��״̬ : ����¼��״̬*/
    REC_PLAY_STATUS,
    /*¼��״̬ : ��ͣ״̬*/
    REC_PAUSE_STATUS,
    /*¼��״̬ : ֹͣ״̬*/
    REC_STOP_STATUS,
/*¼��״̬ : ����״̬*/
//REC_ERROR
} record_status_e;

/*¼��������*/
typedef enum
{
    /*¼�������ʣ�8KHZ*/
    RATE_8KHZ = 0,
    /*¼�������ʣ�16KHZ*/
    RATE_16KHZ = 1,
    /*¼�������ʣ�24KHZ*/
    RATE_24KHZ = 2,
    /*¼�������ʣ�32KHZ*/
    RATE_32KHZ = 3,
/*¼�������ʣ���Чֵ*/
//RATE_MAX = 4,
} record_sample_rate_e;

/*¼��������*/
typedef enum
{
    /*¼�������ʣ�8KBPS*/
    BIT_8KBPS = 0,
    /*¼�������ʣ�32KBPS*/
    BIT_32KBPS = 1,
    /*¼�������ʣ�64KBPS*/
    BIT_64KBPS = 2,
    /*¼�������ʣ�128KBPS*/
    BIT_128KBPS = 3,
    /*¼�������ʣ�192KBPS*/
    BIT_192KBPS = 4,
    /*¼�������ʣ�256KBPS*/
    BIT_256KBPS = 5,
    /*¼�������ʣ�256KBPS*/
    BIT_512KBPS = 6,
    /*¼�������ʣ�64KBPS*/
    BIT_768KBPS = 7,
    /*¼�������ʣ�128KBPS*/
    BIT_1024KBPS = 8,
    /*¼�������ʣ�192KBPS*/
    BIT_1536KBPS = 9,
/*¼�������ʣ���Чֵ*/
//BIT_MAX = 5,
} record_bitrate_e;

/*¼���ļ���ʽ*/
typedef enum
{
    /*¼����ʽ ��wav¼��*/
    REC_FORMAT_WAV = 0,
    /*¼����ʽ ��mp3¼��*/
    REC_FORMAT_MP3 = 1,
    /*¼����ʽ ��ACT¼��*/
    REC_FORMAT_ACT = 2,
/*¼����ʽ ����Ч¼����ʽ*/
//REC_FORMAT_INVALID = 2,
} record_file_type_e;

/*¼��ͨ��*/
typedef enum
{
    /*¼��ͨ�� ����ͨ��*/
    SINGLE_CH = 1,
    /*¼��ͨ�� ��˫ͨ��*/
    DUAL_CH = 2,
} record_channelnum_e;

/*��������*/
typedef enum
{
    /*��ͨģʽ*/
    TRACK_OFF = 0,
    /*�Զ�����*/
    TRACK_ON = 1,
} record_track_mode_e;

/*avr����*/
typedef enum
{
    /*��ͨģʽ*/
    AVR_OFF = 0,
    /*�Զ�����*/
    AVR_ON = 1,
} record_avr_mode_e;

/*¼��codec��������*/
typedef struct
{
    /*recorder codec handle*/
    void* mr_handle;
    /*¼��audio����*/
    mmm_mr_audio_param_t mr_audio_param;
    /*¼���������ò���*/
    mmm_mr_ain_setting_t mr_ain_setting;
    /*¼��ʵʱ״̬����*/
    mmm_mr_status_t mr_status;
} record_mr_param_t;

/*¼��ȫ�ֱ���*/
typedef struct
{
    /*ħ�������ж�vm�����Ƿ���Ч*/
    uint16 maigc;
    /*¼��������*/
    record_sample_rate_e sample_rate;
    /*¼�������� ��0/1/2/3/4/5 ��Ӧ 32/64/128/192/256bps.(ͨ��BitRate_Tab[]ת����module��Ľӿڲ���)*/
    record_bitrate_e bitrate;
    record_bitrate_e mp3_bitrate;
    record_bitrate_e wav_bitrate;
    record_bitrate_e act_bitrate;
    /*¼���ļ���ʽ : WAV/MP3*/
    record_file_type_e file_type;
    /*¼��input����*/
    uint8 gain_input_linein;
    /*¼��adc����*/
    uint8 gain_adc_linein;
    /*¼��input����*/
    uint8 gain_input_mic;
    /*¼��adc����*/
    uint8 gain_adc_mic;
    /*¼��ͨ��*/
    record_channelnum_e channel_num;
    /*¼������ģʽ��0,�Զ�������1,�رշ���*/
    record_track_mode_e track_mode;
    record_avr_mode_e avr_mode;
    /*¼��avr����*/
    uint8 track_level_avr;
    /*¼��avr�Զ���������*/
    uint8 track_level_breaksong;
    /*¼��avr��ʱʱ��,0~0xff*/
    uint8 track_delay;
    /*¼�����뼶��*/ 
    uint32 noise_cancel_level;
    /*��ǰ���õ�¼��Դ:0-MIC;1-FM;2-LINEIN;3-NULL*/
    mmm_ai_type_t ai_source;
    /*¼�������ʽ:0-PCM;1-ADPCM;2-MP3*/
    mmm_mr_encode_mode_t encode_type;
    /*¼���ļ�location*/
    file_path_info_t path;
    /*Ŀǰû��ʹ��*/
    uint16 file_num;
    /*һ��¼��(MIC/LINE-IN)�ļ����*/
    uint16 rec_num;
    /*FM¼���ļ����*/
    uint16 fmrec_num;
    /*����һ��¼��(MIC/LINE-IN)�ļ����*/
    uint16 rec_num_card;
    /*����FM¼���ļ����*/
    uint16 fmrec_num_card;
} record_vars_t;

/*¼���ļ���ز�������*/
typedef struct
{
    /*¼���ļ�mount id*/
    int32 vfs_handle;
    /*ʣ����̿ռ�,������*/
    uint32 free_space;
    /*fhandle�ļ����*/
    handle fhandle;
    /*¼���ļ�������8+3��ʽ*/
    //uint8 rec_filename[12];
    /*¼�����ļ���*/
    uint8 rec_filename[28];
    /*��ʾ¼���ļ���������ȥ�ո��ʽ*/
    uint8 show_filename[13];
    /*¼���ļ�����*/
    int8 file_num;
    /*flash��ʽ����ʶ*/
    uint8 disk_flag;
} record_fs_param_t;

/*¼��������������*/
typedef struct
{
    /*¼���ĳ�����Ϣ*/
    record_scene_e record_scene;
    /*¼����״̬��Ϣ*/
    record_status_e record_status;
    /*FMƵ��*/
    uint8 fm_freq[8];
    /*��ʾˢ�±�־*/
    bool paint_flag;
    /*ʣ��¼����ʱ��*/
    uint32 free_time;
    /*��ʾˢ��bitmap*/
    uint32 display_bitmap;
    /*��ʱ�����*/
    int8 timer_id;
    /*�����жϴ������������˳�recording����*/
    app_result_e record_result;
} record_scene_param_t;

/*wav¼�������д�ļ�ͷ�ṹ��*/
typedef struct
{
    /*����*/
    uint8 rec_filename[28];
    /*�ļ�ƫ����Ϣ*/
    pfile_offset_t ptr_file_offset;
    /*��¼�ļ���С*/
    uint32 file_bytes;
    /*¼��������*/
    uint32 bitrate;
    /*Ŀ¼��Ϣ,73byte,��Ҫ������*/
    pdir_layer_t ptr_layer;
    /*�̷���Ϣ*/
    uint8 disk_no;
    /*¼����ʽ��0-wav, 1-mp3*/
    uint8 rec_format;
    /*��Ҫ��д��־*/
    bool rewrite_need;
} record_writehead_t;

extern uint32 rec_free_space; //��λ��sectors
extern uint32 rec_free_time;
extern uint16 rewrite_times; //������д�Ķ�ʱ������������0

/***********ȫ�ֱ���extern*****************/
/*ϵͳ��������*/
extern comval_t g_comval;

/*record��ȫ�ֱ���*/
extern record_vars_t g_record_vars;

/*ap ���ģʽ*/
extern app_param_e g_entry_mode;

/*record codec params*/
extern record_mr_param_t g_mr_param;

/*¼��������������*/
extern record_scene_param_t g_scene_param;

/*¼���ļ���ز�������*/
extern record_fs_param_t g_fs_param;

/*wav¼�������д�ļ�ͷ�ṹ��*/
extern record_writehead_t g_writehead;

extern bool need_fsel_init;
//add by liuwzh for alarm record
extern uint8 g_record_from_alarm;
extern uint32 alarm_rec_time;
extern uint16 fmrec_frequency; //FMƵ��
extern uint8 fmrec_volume; //FM����
extern bool g_record_timeout;
extern uint8 vram_wr_cnt;
//����¼���ļ���ŵļ���
extern uint16 g_rec_num;
//����fm¼���ļ���ŵļ���
extern uint16 g_fmrec_num;
//��һ�ν�¼��ADC������־
extern uint8 g_adc_flag;
//�Ƿ��һ�ν���¼��Ӧ��
extern uint8 g_first_enter_flag;
//record�ļ����ļ�����
extern uint16 g_file_total;
//��ʱFM��ʱ��
extern int8 g_alarm_timer;


/*¼����������*/
extern app_result_e record_scene_recording(void); //record����������

/*¼���˵���������*/
extern app_result_e record_scene_menu(void);

/*record������UI��ʾ����*/
extern void _record_scene_paint(uint32* pdisplay_bitmap);

/*װ�ش����������ļ�ϵͳ*/
extern bool _record_fs_init(uint8 diskno);

/*¼��֮ǰ��Ŀ¼���飬���̿ռ���飬¼���ļ�����*/
extern bool _record_file_setup(void);

/*wav�ļ�¼����Ϻ��д�ļ�ͷ*/
extern bool _record_write_back_filehead(void);

/*��ʼ¼��*/
extern app_result_e _recording_start(void) __FAR__;

/*��ͣ¼��*/
extern app_result_e _recording_pause(void) __FAR__;

/*�ָ�¼��*/
extern app_result_e _recording_resume(void) __FAR__;

/*ֹͣ¼��*/
extern app_result_e _recording_stop(void) __FAR__;

/*�ر�¼��*/
extern app_result_e _recording_close(void) __FAR__;

/*¼����Ϣ����*/
extern app_result_e record_message_deal(private_msg_t *msg) __FAR__;
extern app_result_e _scene_record_get_msg(void) __FAR__;
extern void _record_show_time_progress(uint32 *pdisplay_bitmap, uint8 para, uint8 type);

/*¼��������ʼ�����˳�����*/
extern bool _scene_recording_init(void);
extern bool _scene_recording_exit(void);

extern void _radio_get_freq(void);
extern bool radio_modual_init(void);
extern bool radio_modual_reinstall(void);



/*FM��ش���*/
extern bool radio_alarm_init(void);
extern bool radio_close_engine(void);
#endif
