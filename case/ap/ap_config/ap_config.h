/*******************************************************************************
 *                              us212a
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
/*
 * \file     ap_config.h
 * \brief    configӦ�õ�ͷ�ļ�
 * \author   zhangxs
 * \version 1.0
 * \date  2011/09/04
 *******************************************************************************/
#ifndef _AP_CONFIG_H
#define _AP_CONFIG_H

#include  "config_res.h"
#include  "config_sty.h"
#include  "psp_includes.h"
#include  "case_include.h"

#define SWITCH_ON           0
#define SWITCH_OFF          1
#define NANDFLASH_RECLAIM   2

#define     VM_AP_RECORD_WAV	    VM_AP_RECORD+0x200
//ǰ̨Ӧ������
#define    MAX_APP_NUM    14
//����Ӧ�ú����Ĵ��ζ���
#define    PARAM_ENTER_NORMAL   0
#define    PARAM_ENTER_ALARM    1
//����Ӧ�ú����Ĵ��ζ���
#define    GUI_OUTOF_STANDBY      0
#define    ALARM_OUTOF_STANDBY    1

typedef enum
{
    BACKLIGHT_OFF, BACKLIGHT_ON
} backlight_type_e;

typedef struct
{
    uint16 magic;       //VM ����ħ��
    uint8 ap_id;        //�ػ���Ӧ��
    uint8 engine_state; //����״̬
    uint8 keyID[16];    //������������
} g_config_var_t;

typedef struct
{
    uint16 active;     //�˲���û�õ���ֻ�Ǳ�����config.bin�е����ݽṹһ��
    uint16 total;      //��������
    uint16 keyID[16];  //������������
} cfg_key_var_t;

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

extern record_writehead_t g_writehead;
extern uint8 charge_frame_id;
extern uint8 backlight_cnt;
extern backlight_type_e backlight_flag;
extern int8 timer_id;
extern bool charge_full_flag;

extern bool write_head(void);
extern void _config_charging(void);
extern void _creat_application(uint8 param);
extern void draw_logo(uint8 param);
extern void switch_off(void);
extern void config_reclaim(int argc);
extern bool _config_format_disk(void);
#endif

