/*******************************************************************************
 *                              US212A
 *                            Module: Key driver
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       reagan     2011-9-2 14:45    1.0             build this file
 *******************************************************************************/
/*!
 * \file     key_scan.c
 * \brief    key scan and message deal
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

#include "../key_inner.h"

#define KEY_MSG_DEPTH   8//gui ��Ϣ�������
#define NO_KEY KEY_NULL
#define DOWN_KEY_TIMER  3//����������Ϣʱ��Ϊ60ms
#define LONG_KEY_TIMER  5//����������Ϣʱ��Ϊ 60ms + (5-1)*HOLD_KEY_TIMER = 700ms
#define HOLD_KEY_TIMER  8//��������hold��Ϣ���Ϊ160ms

extern unsigned int g_rtc_counter;
extern unsigned int key_count; //��ǰ������Ϣ�����Ĵ���
extern unsigned int key_value; //��ǰ������ֵ

/******************************************************************************/
/*!
 * \par  Description:
 *	  Post key package message to OS
 * \param[in]    *input_msg
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \retval           0
 * \retval           1
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * ��װ������Ϣ�������͸���Ϣ����
 *******************************************************************************/
bool post_key_msg(input_gui_msg_t *input_msg)
{
    uint32 key_data;

    //��Ϣѹ��
    key_data = (uint32) input_msg->type;//byte 0
    if (input_msg->type == INPUT_MSG_KEY)
    {
        key_data |= ((uint32) input_msg->data.kmsg.val << 8);//byte 1
        key_data |= ((uint32) input_msg->data.kmsg.type << 16);//byte 2-3
    }
    else if (input_msg->type == INPUT_MSG_TOUCH)
    {
        key_data |= ((uint32) input_msg->data.tmsg.x << 8);//bit[8-17]
        key_data |= ((uint32) input_msg->data.tmsg.y << 18);//bit[18-27]
        key_data |= ((uint32) input_msg->data.tmsg.type << 28);//bit[28-31]
    }
    else
    {

    }

    if ((int) sys_mq_send(MQ_ID_GUI, (void *) &key_data) == 0)
    {
        //libc_print("key", key_data);
        return TRUE;
    }

    //��Ϣ��������������false
    return FALSE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  Key message deal
 * \param[in]    key
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * �����̰���������������Ӧ������Ϣ
 *******************************************************************************/
void PutSysMsg(unsigned int key)
{
    input_gui_msg_t input_msg;
    input_msg.type = INPUT_MSG_KEY;
    if (key == key_value)
    {

        //g_rtc_counter = 0;
        key_count++;

        /*��ס����ʱ��160ms����һ����ͬ������Ϣ*/
        if (key_count == LONG_KEY_TIMER)
        {
            //key = (unsigned int) (key | AP_KEY_LONG); //=0.7s,����������Ϣ
            input_msg.data.kmsg.val = key_value;
            input_msg.data.kmsg.type = KEY_TYPE_LONG;
        }
        else if (key_count > LONG_KEY_TIMER)
        {
            //key = (unsigned int) (key | AP_KEY_HOLD); //>0.7s,ÿ160ms��Key hold��Ϣ
            input_msg.data.kmsg.val = key_value;
            input_msg.data.kmsg.type = KEY_TYPE_HOLD;
        }
        else
        {
            return;
        }
        //return key;   //putmsg

        post_key_msg(&input_msg);
        return;

    }
    switch (key)
    {
        case Msg_KeyShortUp:
        //gui��Ϣ�����Ѷ���HOLD��Ϣ������ȡ��һ���ٷ� SHORT_UP ��������֤ SHORT_UP ��Ϣ�ܹ����ͳɹ�
        if(key_count >= LONG_KEY_TIMER + KEY_MSG_DEPTH)
        {
            //�Ƚ���gui��Ϣ����֤gui��Ϣ���з���
            uint32 key_data;
            sys_mq_receive(MQ_ID_GUI, (void *)&key_data);
        }
        key_count = 0;
        //g_rtc_counter = 0;
        /*key_value������ı�*/
        input_msg.data.kmsg.val = key_value;
        input_msg.data.kmsg.type = KEY_TYPE_SHORT_UP;
        key_value = KEY_NULL;

        break;
        case Msg_KeyHold:
        key_count = 0;
        //g_rtc_counter = 0;
        /*key_value������ı�*/
        input_msg.data.kmsg.val = KEY_LOCK;
        input_msg.data.kmsg.type = KEY_TYPE_DOWN;
        key_value = KEY_NULL;
        break;
        case Msg_KeyUnHold:
        key_count = 0;
        //g_rtc_counter = 0;
        /*key_value������ı�*/
        input_msg.data.kmsg.val = KEY_UNLOCK;
        input_msg.data.kmsg.type = KEY_TYPE_DOWN;
        key_value = KEY_NULL;
        break;
        default: /*�°�������*/
        //g_rtc_counter = 0;
        key_value = key;
        input_msg.data.kmsg.val = key;
        input_msg.data.kmsg.type = KEY_TYPE_DOWN;
        key_count = 1;
        break;
    }
    //puts key msg in here

    post_key_msg(&input_msg);
    return;
}

