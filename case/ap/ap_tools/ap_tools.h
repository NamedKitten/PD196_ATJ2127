/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ap_setting.h
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */

#ifndef _TOOLS_H_
#define _TOOLS_H_
#include "tools_res.h"
#include "tools_sty.h"
#include "case_include.h"
#include "psp_includes.h"
#include "enhanced.h"
#include "mmm_id.h"
#include "alarm_common.h"


/*������ڲ˵����*/
#define MENU_ENTRY_ID_MAIN        0x00
#define MENU_ENTRY_ID_MAIN_NOCARD 0x01

#define MAX_YEAR                  2098     /*��������������*/
#define MIN_YEAR                  2000     /*����������С���*/

#define VALUE_12HOURS_FORMAT 0x00  //12Сʱ��
#define VALUE_24HOURS_FORMAT 0x01  //24Сʱ��

#define VALUE_TIME_HH_12H_MIN 1  //12Сʱ����Сֵ
#define VALUE_TIME_HH_24H_MIN 0  //24Сʱ����Сֵ
#define VALUE_TIME_HH_12H_MAX 12  //12Сʱ�����ֵ
#define VALUE_TIME_HH_24H_MAX 23  //24Сʱ�����ֵ
#define VALUE_TIME_HH_STEP 1  //Сʱ���ڲ���

#define VALUE_TIME_MM_MIN 0  //������Сֵ
#define VALUE_TIME_MM_MAX 59  //�������ֵ
#define VALUE_TIME_MM_STEP 1  //���ӵ��ڲ���

#define VALUE_TIME_AM 0  //����AM
#define VALUE_TIME_PM 1  //����PM
#define VALUE_TIME_AMPM_STEP 1  //AMPM����
#define POSITION_AMPM 2  //

#define VALUE_DD_MM_YYYY	0x00  //���ڸ�ʽ����������
#define VALUE_MM_DD_YYYY    0x01  //���ڸ�ʽ����������
#define VALUE_YYYY_MM_DD    0x02  //���ڸ�ʽ����������

#define FROM_MENU	  0  //�Ӳ˵���������
#define FROM_ALARM    1  //�����ӽ�������
#define FROM_ALARMFM  2  //�Ӷ�ʱFM��������

/*ȡ����ֵ*/
#define abs(x)  (((x)>0) ? (x) : -(x))
/*����ת��������*/
#define baseYear   1901 //
#define baseMonth  1
#define baseDate   1
#define baseIndex  0
#define baseChineseYear   4597//4598-1
#define baseChineseMonth   11
#define baseChineseDate    11

/*TOOLS����ö��*/
typedef enum
{
    //�˵�����
    SCENE_MENULIST = 0, 
    //��������
    SCENE_CALENDAR, 
    //�����
    SCENE_STOPWATCH, 
    //���ӳ���
    //SCENE_ALARM, 
    //���ݽ�������
    //SCENE_DATA_EXCHANGE, 
    //�˳�����
    SCENE_EXIT
} scene_tools_e;

/*���ڽṹ��*/
typedef struct
{
    uint16 year;
    uint8 month;
    uint8 day;
} DateVarType;

/*���״̬*/
typedef enum
{   
    //�ر�״̬
    CloseMode = 0,
    //��ʱ״̬
    RunMode = 1,
    //ֹͣ״̬
    StopMode = 2 
} sw_status_e;
//#define  CloseMode    0
//#define  RunMode      1
//#define  StopMode     2


/*��������*/
extern comval_t g_comval; //ϵͳ����VM����
extern alarm_vars_t  g_alarm_vars; //ALARM VM����
extern uint8 alarm_set_flag;
extern uint8 fmrec_set_flag;
extern int8 g_halfsec_timer; //5s��ʱ��
extern date_t date;
//extern time_t time;

//for stopwatch
extern StopWatch_time_t sw_realtime;
extern StopWatch_time_t swtime[5];
extern sw_status_e  sw_status;
//extern uint8  sw_itemnum;
extern uint8  sw_showflag;
extern uint8  CtcCntEnable;
//MSG_SD_OUTʹ�ã�GUI_DIRECTORY����
extern bool g_dir_flag;

/*��������*/
extern app_result_e tools_menulist(void);
extern app_result_e calendar_menu_entry(void *menu_title_void);
extern app_result_e stopwatch_menu_entry(void *menu_title_void);
extern app_result_e dataexch_menu_entry(void *menu_title_void);
extern app_result_e fwupdata_menu_entry(void *menu_title_void);

extern app_result_e calendar_main(uint8 flag);
extern app_result_e stopwatch_main(void);

extern app_result_e option_callback(void *menu_title_void);
extern app_result_e tool_privmsg_deal(private_msg_t *private_msg) __FAR__;
extern void music_close_engine(void) __FAR__; //��rcode����
//calendar callback
extern uint8 isLeapYear(uint16 y);
extern uint8 daysInMonth_Internal(uint16 year, uint8 month);

//stopwatch callback
extern void StopWatch_timer_proc(void);

//alarm callback
app_result_e set_alarm_time_callback(time_t *t_time);
app_result_e set_alarm_volume_callback(uint8 *v_volume);
app_result_e alarm_menu_entry(void *menu_title_void);
app_result_e fmrec_menu_entry(void *menu_title_void);
void alarm1_exit_callback(void *menu_title_void);
void fmrec_exit_callback(void *menu_title_void);
app_result_e alarm_ring_callback(uint8 disk_flag);
app_result_e browser_filelist_init(uint8 diskflag);
//for alarm
app_result_e set_alarm1_onoff_callback(void *menu_title_void);
app_result_e set_alarm1_off_callback(void *menu_title_void);
app_result_e set_alarm1_on_callback(void *menu_title_void);
app_result_e set_alarm1_time_callback(void *menu_title_void);
app_result_e set_alarm1_cyc_callback(void * title_action);
app_result_e alarm1_once_callback(void *menu_title_void);
app_result_e alarm1_days_callback(void *menu_title_void);
app_result_e alarm1_work_callback(void *menu_title_void);
app_result_e set_alarm1_ring_callback(void * title_action);
app_result_e alarm1_ring_sd_callback(void *menu_title_void);
app_result_e alarm1_ring_flash_callback(void *menu_title_void);
app_result_e alarm1_ring_card_callback(void *menu_title_void);
app_result_e alarm1_volume_callback(void *menu_title_void);
//for fmrec
app_result_e set_fm_onoff_callback(void *menu_title_void);
app_result_e set_fm_off_callback(void *menu_title_void);
app_result_e set_fm_on_callback(void *menu_title_void);
app_result_e set_fmtime_callback(void *menu_title_void);
app_result_e set_fmtime_on_callback(void *menu_title_void);
app_result_e set_fmtime_off_callback(void *menu_title_void);
app_result_e set_fm_cyc_callback(void * title_action);
app_result_e set_fm_once_callback(void *menu_title_void);
app_result_e set_fm_days_callback(void *menu_title_void);
app_result_e set_fm_work_callback(void *menu_title_void);
app_result_e fm_volume_callback(void *menu_title_void);
app_result_e set_fm_fq_callback(void *menu_title_void);
app_result_e fmrec_onoff_callback(void * title_action);
app_result_e fmrec_off_callback(void *menu_title_void);
app_result_e fmrec_on_callback(void *menu_title_void);
app_result_e set_fmfreq_main(void);
//for autoupgrade
extern app_result_e autoupgrade_menu_entry(void *menu_title_void);



#endif
