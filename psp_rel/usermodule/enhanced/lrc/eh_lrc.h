/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     eh_lrc.h
 * \brief    ���ݽṹ�ͳ�������
 * \author   liminxian
 * \version 1.0
 * \date  2011/09/15
 *******************************************************************************/

#ifndef _eh_lrc_h

#define _eh_lrc_h

#include "eh_module.h"
#include "eh_rdata_ex.h"

#define MINUTE_VALID  2   //����Ч����
#define SECOND_VALID  3  //�뼶��Чλ
#define MS_VALID  3  //10MS����Чλ
#define MAX_MINUTE    99
#define MAX_LAB_COUNT 800  //7k�ռ�

#ifndef _VC_DEBUG

#ifndef PC
#define LRC_LABLE_ADDR      0x9fc34000 //��RDATA_AL(JPEG)(8k)����ʱ����
#endif

#define LRC_CONTENT_VM      VM_LYRIC_INFO//0x0000c000 //8k (case\inc, VM_LYRIC_INFO)
#define LRC_INFO_VM         (LRC_CONTENT_VM+0x2000)   //0.5k
#define LRC_LABLE_VM        (LRC_INFO_VM+0x200)       //7.5k
#define LRC_CONTENT_MAX      LRC_INFO_VM		//vm max address
#endif

#define LRC_TIME_MAX        0xff000000 //��ǩ����ʱ��
typedef enum
{
    READ_ZERO = 0, //�����ַ�������\0
    READ_NEXT, //���¸��ӿ�
    READ_LAB_START, // ������ʵĲ�ֹһ����ǩʱ����ʼ���
    READ_DATA_END, // ���굱ǰ������ݽ���
    READ_INVALID_LAB, //����ǩʱ���쳣
    READ_FILE_END, //�����ļ�β
    READ_FILE_ERROR
//���ļ�������
} ret_read_e;//

typedef enum
{
    LRC_FIND_NEXT, LRC_FIND_CUR, LRC_FIND_PREV
} lrc_find_e;

typedef struct
{
    uint8 p_ms; //  0.01s
    uint8 sec; //  ��
    uint8 min; // ��
    uint8 reserve;//
    uint16 offset_byte; //����ڸ�ʿ��е��ֽ�ƫ��
    uint16 lrc_len; //��ʵĳ���
} lrc_lable_t;

//extern global
//load lrc
extern handle lrc_handle;
extern uint16 time_lab_count; //ʱ���ǩ����

//get lrc
extern uint16 lrc_cur_num; //��ʵ�ҳ��
extern uint32 lrc_prev_time; //ǰһ��ʱ��
extern uint32 lrc_cur_time; //��ǰҳ��ʵ�ʱ��
extern uint32 lrc_next_time; //��һҳ��ʵ�ʱ��
extern uint16 lrc_lab_count; //ʱ���ǩ����

#ifdef _VC_DEBUG
extern uint8 LRC_CONTENT_ADDR[0x800];
extern uint8 LRC_LABLE_ADDR[0x800];
extern uint8 LRC_CONTENT_VM[0x2000];
extern uint8 LRC_LABLE_VM[0x2000];
#else

#ifdef PC
extern uint8 LRC_CONTENT_ADDR[0x800];
extern uint8 LRC_LABLE_ADDR[0x800];
//extern uint8 LRC_CONTENT_VM[0x2000];
//extern uint8 LRC_LABLE_VM[0x2000];
#endif

#endif

#endif

