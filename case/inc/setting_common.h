/*******************************************************************************
 *                              us212A
 *                            Module: SETTING AP
 *                 Copyright(c) 2003-20012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       johnsen     2011-9-2 9:18     1.0             build this file 
*******************************************************************************/
/*!
 * \file     setting_common.h
 * \brief    setting Ӧ�ù������ݽṹ
 * \author   johnsen
 * \version  1.0
 * \date     2011-9-9
*******************************************************************************/

#ifndef _SETTING_COMMON_H
#define _SETTING_COMMON_H

#include "psp_includes.h"
#include "enhanced.h"
#include "applib.h"
#include "case_type.h"
#include "common_func.h"

#define VALUE_MUSIC_REPEAT_OFF 0x00 //��ֵ��music AP����
#define VALUE_MUSIC_REPEAT_ONE 0x01
#define VALUE_MUSIC_REPEAT_ALL 0x02
#define VALUE_MUSIC_REPEAT_INTRO 0x03

#define VALUE_MUSIC_SHUFFLE_OFF 0x00  //��ֵ��music AP����
#define VALUE_MUSIC_SHUFFLE_ON 0x10

#define VALUE_FULLSOUND_OFF    0x00    //��ֵ��music AP����
#define VALUE_FULLSOUND_ON     0x01

#define VALUE_EQ_OFF     0x00    //��ֵ��music AP����
#define VALUE_EQ_ROCK    0x01
#define VALUE_EQ_FUNK    0x02
#define VALUE_EQ_HIP_HOP 0x03
#define VALUE_EQ_JAZZ    0x04
#define VALUE_EQ_CLASSICAL    0x05
#define VALUE_EQ_TECHNO       0x06
#define VALUE_EQ_CUSTOM       0x07

#define VALUE_SRS_OFF 0x0
#define VALUE_WOWHD_ON 0x01
#define VALUE_USER_SRS_ON 0x2

#define COMMAND_EQ_NORMAL 0
#define COMMAND_EQ  0x07
#define COMMAND_FULLSOUND 0x07
#define COMMAND_SRS 0x4f
#define COMMAND_VPS 0x77

#define VALUE_FROM_DEFAULT    0x04      //��ֵ��music AP����
#define VALUE_FROM_ALL_SONGS  0x00  
#define VALUE_FROM_ALBUM      0x01
#define VALUE_FROM_ARTIST     0x02
#define VALUE_FROM_GENRE      0x03

#define VALUE_SHUT_DOWN_TIMER_OFF 0x00
#define VALUE_SHUT_DOWN_TIMER_ON 0x01

#define VALUE_SHUT_DOWN_TIME_MIN 1
#define VALUE_SHUT_DOWN_TIME_MAX 99
#define VALUE_SHUT_DOWN_TIME_STEP 1

#define VALUE_BUTTON_YESNO_YES  0
#define VALUE_BUTTON_YESNO_NO   1

#define VALUE_LIGHTNESS_MAX  11
#define VALUE_LIGHTNESS_MIN  0
#define VALUE_LIGHTNESS_STEP 1
#define VALUE_LIGHTNESS_MAX_IN_DRV 15   //��ֵ������ʾ����һ�£�����������

#define VALUE_KEY_TONE_ON  1
#define VALUE_KEY_TONE_OFF 0
/*!
 * \brief
 *  comval_t ϵͳ����������Ͻṹ��
 */
typedef struct
{
    /*! magic��ֵΪ 0x55AA ʱ��Ч */
    uint16 magic;
    
    //��ʾ�����
    /*! �������ȵȼ� */
    uint8 lightness;
    /*! ���� 0����ɫ 1����ɫ 2����ɫ 3���ۺ�ɫ 4����ɫ */
    uint8 theme;
    //+4bytes
    /*! ��Ļ���� 0: �� 1: ����ʱ�� 2�����ͼ�� 3���ر���Ļ 4����ʾģʽ */
    uint8 screen_saver_mode;
    /*! ����ʱ�� ��0.5��Ϊ��λ��0��ʾ���� */
    uint8 light_timer;
    /*! ��Ļ����ʱ�� ��0.5��Ϊ��λ��0��ʾ����Ļ���� */
    uint8 screen_saver_timer;
    /*! ʡ��ػ�ʱ�� ��0.5��Ϊ��λ��0��ʾ��ʡ��ػ� */
    uint8 poweroff_timer;
    //+8bytes
    /*! ��ʱ�������ڲ��Ž��� ��0.5��Ϊ��λ��0��ʾ���뷵�� */
    uint8 playing_timer;
    /*! ��ʱ�ػ�9˯�ߣ�ʱ�� �Է���Ϊ��λ��0��ʾ����ʱ�ػ� */
    uint8 sleep_timer;
    
    //���ں�ʱ��
    /*! ʱ���ʽ 0��12Сʱ�� 1��24Сʱ�� */
    uint8 time_format;
    /*! ���ڸ�ʽ 0��DD_MM_YYYY 1��MM_DD_YYYY 2��YYYY_MM_DD */
    uint8 date_format;
    //+12bytes
    /*! ��������, ��ֵ����ʾ�������� */
    uint8 language_id;
    /*! ����������ѡ�� 0��MSC 1��MTP */
    uint8 online_device;
    /*! CD��װ���� autorun֧��ѡ�� 0 ��autorun��1�� ֧��autorun */
    uint8 autorun_set;
    /*! ֧�ֿ�ѡ��  0:��֧��  1:֧�� */
    uint8 support_card;
    //+16bytes
    /*! �������� */
    uint8 volume_limit;
    /*! ��ǰ����ֵ */
    uint8 volume_current;
    //ͼƬ����
    /*! �õ�Ƭ���ʱ�� */
    uint8 slide_time;
    /*! �õ�Ƭ�ظ�ģʽ */
    uint8 slide_repeat_mode;
    //+20bytes 
    /*! �õ�Ƭ shuffle ���� */
    uint8 slide_shuffle;
    /*! ��Ļ�䰵���ص���Ļʱ�䣬��0.5��Ϊ��λ��0��ʾ�޴˹��� */
    uint8 dark_lightoff_timer;
    /*! �������ص���Ļʱ�䣬�Է���Ϊ��λ��0��ʾ�޴˹��� */
    uint8 screen_lightoff_timer;
    /*!����������*/
    uint8 key_tone;
} comval_t;

/*!
 * \brief
 *  music_comval_t �������ñ�����Ͻṹ��
 */
typedef struct
{
    /*! ������Դ */
    uint8 music_from;
    /*! ����ѭ����ʽ */
    uint8 music_repeat_mode;
    /*! ����������ظ����� */
    uint8 music_shuffle;
    /*! fullsound ��Ч���� */
    uint8 fullsound;
    /*! srs ��Ч���� */
    uint8 srs;
    /*! eqģʽ�趨 */
    uint8 eq_setting;
    /*! ���ٲ��� */
    int8  variable_playback;
    uint8 reserved;
    /*! eq�û������� */
    int8 eq_parameter[12];
    /*! srs�û������� */
    int8 srs_parameter[12];
} music_comval_t;

/*!
 * \brief
 *  setting_comval_t ����Ӧ��ȫ�ֲ����ṹ��
 */
typedef struct
{
    /*! ϵͳ����������Ͻṹ�� */
    comval_t g_comval;
    /*! �������ñ�����Ͻṹ�� */
    music_comval_t music_comval;
} setting_comval_t;

/*!
 * \brief
 *  browser��VMȫ�ֱ���
 */
typedef struct browser_var_t
{
    //browser��·����Ϣ
    file_path_info_t path;
    uint8 prev_enter_mode;
    //browserħ��
    uint16 magic;
    //uint16 reserved;
} browser_var_t;


#endif //_SETTING_COMMON_H
