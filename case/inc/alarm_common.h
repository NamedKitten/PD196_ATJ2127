/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : setting_common.h
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
#ifndef _ALARM_COMMON_H
#define _ALARM_COMMON_H

#include "psp_includes.h"
#include "enhanced.h"
#include "applib.h"
#include "case_type.h"
#include "common_func.h"

#define  ALARM_FUN //�궨��alarm����
#define  ALARMFM   //�궨��ALARMFM����
/*���ṹ��*/
typedef struct
{    
    uint8 Ms;    
    uint8 Second;    
    uint8 Minute;    
    uint8 Hour;
} StopWatch_time_t;
/*����ѭ������*/
typedef struct
{
    uint16 year;
    uint8 month;
    uint8 day;
} cycle_one_time_s;

/*����ѭ������*/
typedef struct
{
    uint8 week_day[7];
} cycle_select_week_s;

/*����ѭ����ʽ*/
typedef struct
{
    cycle_one_time_s  one_time;
    cycle_select_week_s  select_week;//Reserved
} cycle_t;

/*ALARM�ṹ��*/
typedef struct
{
    //uint16 magic;
    uint8  enable;
    time_t start_time;  //����1��ʱ������
    uint8  cycle_flag;  //�������ڵ����ñ�ʶ��0-���Σ�1-ÿ�죬2-������
    cycle_t cycle;      //�������ڵ����÷�ʽ
    uint8  ring_flag;   //0-���� 1-flash 2-card
    file_path_info_t path;  //��Ӧ����ʱ���������ֵ�·��
    uint8  volume;          //�������ֲ���ʱ�������Ĵ�С
    uint8  alarm_count; //Snooze��־������
} alarm_alarm_t;

/*ALARMFM�ṹ��*/
typedef struct
{
    //uint16 magic;
    uint8  enable; //ʹ��
    time_t start_time;  //��ʱFM��ʱ������
    time_t end_time;    //��ʱFM��ʱ������
    uint8  cycle_flag;  //����ѭ����ʶ��0-���Σ�1-ÿ�죬2-������
    cycle_t cycle;      //ѭ���ṹ
    uint16 frequency;   //FMƵ��
    uint8  volume;      //FM����
    uint8  fmrec_enable;   //¼��ʹ�ܱ�־
    //uint8  timer_flag;
} alarm_fm_t;

/*Reserved*/
typedef struct
{
    //uint16 magic;
    uint8  enable;  //��ʱ¼���Ŀ��ر�ʶ
    time_t start_time; // ��ʼ¼����ʱ��
    time_t end_time;   // ����¼����ʱ��
    uint8  rcycle_flag;  //��ʱ¼���������ñ�ʶ��0 -���Σ�1-ÿ�죬2-������
    cycle_t cycle; //��ʱ¼�����ڵ����÷�ʽ
    uint8  rec_type;    //
} alarm_record_t;


typedef struct
{
    uint16 magic;  //MAGIC��־
    uint8  timer_flag; //��־��������
    alarm_alarm_t g_alarm;
    alarm_fm_t   g_aufm;
    time_t  snooze_time; //����snoozeʱ��
    //alarm_record_t g_arecord; //Reserved
}alarm_vars_t;

#endif //_ALARM_COMMON_H

