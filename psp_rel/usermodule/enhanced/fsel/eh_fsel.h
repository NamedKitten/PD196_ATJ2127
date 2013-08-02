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
 * \file     eh_fsel.h
 * \brief    ���ݽṹ�ͳ�������
 * \author   liminxian
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#ifndef _eh_fsel_h

#define _eh_fsel_h

#include "enhanced.h"
#include "eh_module.h"
#include "eh_rdata_ex.h"

//�������λ���飬�������λ�����ڵĳ�Ա��������һ�����е���һ��

#define SET_GROUP_MAX   64
#define SET_MEMBER_MAX  256
#define SET_PREV_MAX    16

extern uint16 shuffle_total;//shuffle���ܸ���
extern uint8 group_map[SET_GROUP_MAX / 8];//���飬��64��
extern uint8 member_map[SET_MEMBER_MAX / 8];//���ڳ�Ա����256
extern uint8 member_avg;//��Աƽ��ֵ����ֵ
extern uint8 member_avg_group;//ƽ��ֵ��Ա�������ʼ
extern uint8 group_max;//��Ч�����
extern uint8 member_max;//��ǰ�������Ա����
extern uint8 use_member_flag;//ʹ������shuffle����
extern uint8 cur_group;//��ǰ����ţ�1��2��������
extern uint16 cur_member;//��ǰ���ڵĳ�Ա�ţ�1��2��������
extern uint16 filtrate_shuffle_no; //shuffle���˲�����ţ������ظ�

extern uint16 play_num_list[SET_PREV_MAX];
extern uint8 cur_save_pt; //��ǰ���ָ��
//extern uint8  cur_play_pos; //��ǰ����ָ��
//extern uint8  cur_save_count; //���ݱ���ĸ���
extern uint16 dir_no;

extern uint8 cur_save_prev_count;
extern uint8 cur_save_next_count;
extern uint8 cur_overturn_flag; //��ת���


bool init_shuffle(uint16 num_total);
uint16 shuffle_next_file(void);
uint16 shuffle_prev_file(void);

void init_fsel_sysdata(void);
extern uint32 fsel_open_object(uint8 open_mode) __FAR__;
extern bool fsel_close_object(void) __FAR__;

#endif

