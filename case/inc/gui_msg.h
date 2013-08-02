/*******************************************************************************
 *                              us211A
 *                            Module: Applib
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-11-27 22:03     1.0             build this file 
*******************************************************************************/
/*!
 * \file     gui_msg.h
 * \brief    gui ��Ϣ����ģ��������ݽṹ���궨���
 * \author   lzcai
 * \version  1.0
 * \date     2011-11-27
*******************************************************************************/

#ifndef _gui_msg_H_
#define _gui_msg_H_

#include "psp_includes.h"

/*!
 * \brief
 *  input_msg_type_e ��������Ϣ���ͣ�Ҳ�� gui ��Ϣ����
 */
typedef enum
{
    /*! ��ͨ���� */
    INPUT_MSG_KEY       = 0x00,
    /*! ������ */
    INPUT_MSG_TOUCH     = 0x01,
    /*! g-sensor */
    INPUT_MSG_GSENSOR   = 0x02,
} input_msg_type_e;

/*!
 * \brief
 *  key_value_e �߼�����ö������
 */
typedef enum
{
    /*! �հ��� */
    KEY_NULL        = 0x00,
    /*! ����/��ͣ���� */
    KEY_PLAY        = 0x05,
    /*! ��һ��󣩰��� */
    KEY_PREV        = 0x04,
    /*! ��һ��ң����� */
    KEY_NEXT        = 0x06,
    /*! �������ڿ�ݰ��� */
    KEY_VOL         = 0x0a,
    /*! �����ӿ�ݰ��� */
    KEY_VADD        = 0x09,
    /*! ��������ݰ��� */
    KEY_VSUB        = 0x07,
    /*! �˵�/ѡ��� */
    KEY_MODE        = 0x03,
    /*! ������ */
    KEY_LOCK        = 0x0c,
    /*! ��������ֻ�ڲ���������Ҫ */
    KEY_UNLOCK      = 0x0d,
    /*! ���������ְ�����������ס״̬�£�������ͨ������ת��Ϊ�ð��� */
    KEY_LOCKHOLD    = 0x7f,
    
    KEY_RETURN      = 0x80,
    KEY_POWER       = 0x81,
    KEY_UP          = 0x82,
    KEY_DOWN        = 0x83,
} key_value_e;

/*!
 * \brief
 *  key_type_e ��������
 */
typedef enum
{
    /*! û�а���*/
    KEY_TYPE_NULL       = 0x0000,
    /*! ��������*/
    KEY_TYPE_DOWN       = 0x2000,
    /*! ��������*/
    KEY_TYPE_LONG       = 0x1000,
    /*! ��������*/
    KEY_TYPE_HOLD       = 0x0800,
    /*! �����̰�����*/
    KEY_TYPE_SHORT_UP   = 0x0400,
    /*! ������������*/
    KEY_TYPE_LONG_UP    = 0x0200,
    /*! �������ֵ���*/
    KEY_TYPE_HOLD_UP    = 0x0100,
    /*! ���ⰴ������ */
    KEY_TYPE_ALL        = 0x3f00,
} key_type_e;


/*!
 * \brief  
 *  key_event_t �����¼��ṹ��
 */
typedef struct
{
    /*! �������߼���ֵ*/
    key_value_e val;
    uint8 reserve;
    /*! ��������*/
    key_type_e type;
} key_event_t;

/*!
 * \brief  
 *  tp_type_e ��������
 */
typedef enum
{
    /*! ��������*/
    TP_TYPE_DOWN        = 0x0,
    /*! ��������*/
    TP_TYPE_UP          = 0x1,
    /*! �����ƶ�*/
    TP_TYPE_MOVE        = 0x2,
} tp_type_e;

/*!
 * \brief  
 *  touch_event_t �����¼��ṹ�壬ѹ��Ϊ24bit
 */
typedef struct
{
    /*! ������X���꣬10bitȡֵ��Χ��0~1023*/
    uint16 x;
    /*! ������Y���꣬10bitȡֵ��Χ��0~1023*/
    uint16 y;
    /*! �������ͣ�4bit��Ч */
    tp_type_e type;
} touch_event_t;

/*!
 * \brief  
 *  input_gui_msg_t ��������Ϣ���ֳ� gui ��Ϣ�ṹ��
 */
typedef struct
{
    /*! ��Ϣ����*/
    input_msg_type_e type;
    /*! ��Ϣ����*/
    union
    {
        /*! �����¼�*/
        key_event_t kmsg;
        /*! �����¼�*/
        touch_event_t tmsg;
    }data;
} input_gui_msg_t;

#endif
