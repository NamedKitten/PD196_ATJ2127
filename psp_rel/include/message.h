/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     message.h
 * \brief    ��Ϣ�����ӿ�
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define MSG_SYS_RTC2HZ               0x0080
#define MSG_SYS_BAT_V_CHG            0x0081
#define MSG_SYS_LOW_POWER            0x0082
#define MSG_SYS_RTCALARM             0x0083
#define MSG_SYS_POWER_OFF            0x0084  /*! �ػ���Ϣ, �ȴ�ǰ̨Ӧ�úͺ�̨Ӧ���˳������� config Ӧ�ý���ػ����� */
#define MSG_SYS_RECLAIM              0x0085

#define MSG_SYS_USB_STICK            0x0090
#define MSG_SYS_USB_UNSTICK          0x0091

#define MSG_SYS_ADAPTOR_IN           0x00a0  /*! ����� ADAPTOR ������Ϣ */
#define MSG_SYS_ADAPTOR_OUT          0x00a1  /*! ����� ADAPTOR �γ���Ϣ */

#define MSG_SYS_SD_IN                0x00b0  /*�忨״̬*/
#define MSG_SYS_SD_OUT               0x00b1  /*����״̬*/

#define MSG_SYS_UH_IN				 0x00c0	 /*U�̲���*/
#define MSG_SYS_UH_OUT				 0x00c1	 /*U�̰γ�*/

#ifndef _ASSEMBLER_

extern int mq_send(uint8 queue_id, void *msg, void* null2);
extern int mq_send_fix(uint8 queue_id, void *msg, void* null2);
extern int mq_receive(uint8 queue_id, void *msg, void* null2);
extern int mq_flush(uint8 queue_id, void* null1, void* null2);
#endif

#endif

