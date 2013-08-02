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

#ifndef _ALARM_H_
#define _ALARM_H_
#include "alarm_res.h"
#include "alarm_sty.h"
#include "case_include.h"
#include "psp_includes.h"
#include "enhanced.h"
#include "mmm_id.h"
#include "alarm_common.h"

/*VM �滮 ��ʱʹ��*/
//#define VM_ALARM	  0x3000
//#define MAGIC_ALARM 0x55AA

/*ʱ���ʽ*/
#define VALUE_12HOURS_FORMAT 0x00
#define VALUE_24HOURS_FORMAT 0x01
/*���Ӳ���ʱ�䡪��5s*/
#define  ALARM_PLAY_TIME  5000*6

//��������
/*ϵͳ����VM����*/
extern comval_t g_comval;
/*ALARM VM����*/
extern alarm_alarm_t g_alarm;
//extern setting_comval_t g_setting_comval;
/*����״̬��Ϣ*/
extern mengine_status_t g_music_status;
/*���Ӷ���ID*/
extern uint8 alarm_pid;
/*���Ӷ���Ч��*/
extern bool g_alarm_pid;
/*��ʱ�˳���־*/
extern bool g_alarm_timeout;
/*���ӳ�ʱ����*/
extern uint8 g_timer_60;
/*��ʱ��id*/
/*0.5s��ʱ��*/
extern int8 g_halfsec_timer;
/*30s��ʱ��*/
extern int8 g_tirtysec_timer;

//��������
extern app_result_e alarm_scene_playing(void);
extern app_result_e playing_loop_deal(void);
extern bool alarm_play(void);
extern bool music_close_engine(void);
extern void play_eror_deal(void);
extern void play_scene_paint(void);
extern app_result_e _scene_play_guimsg(msg_apps_type_e cur_event);
#endif
