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
 * \file     key_rcode_scan.c
 * \brief    key scan and message deal
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

#include "key_inner.h"

#define KEY_MSG_DEPTH   8//gui ��Ϣ�������
#define NO_KEY KEY_NULL
#define DOWN_KEY_TIMER  3//����������Ϣʱ��Ϊ60ms
#define LONG_KEY_TIMER  5//����������Ϣʱ��Ϊ 60ms + (5-1)*HOLD_KEY_TIMER = 700ms
#define HOLD_KEY_TIMER  8//��������hold��Ϣ���Ϊ160ms
unsigned char newkey = NO_KEY;
unsigned char oldkey = NO_KEY;
unsigned char tmpkey = NO_KEY;
unsigned char tmp_count, hold_count;
unsigned char wait_keyup = 0;

//��һ��byte�̶�Ϊplay�����������߿ذ���˳�򣬴�С������
unsigned char key_map[16];

#ifdef lock_key
unsigned char TheLastHoldKey = Msg_KeyUnHold;
unsigned char RTCHold_Counter=0;
unsigned char TheFirstHold=0;
#endif

unsigned int g_rtc_counter = 0;
unsigned int key_count = 0; //��ǰ������Ϣ�����Ĵ���
unsigned int key_value = KEY_NULL; //��ǰ������ֵ
int time_irq;
unsigned char g_speakonoff;
unsigned char g_debounce_earphone = 0;

bool g_earphone_status = FALSE;

//ADC data

#define ADC_KEY_NUM  (4)

const unsigned char Adc_data[ADC_KEY_NUM] =
{ 0x06, 0x0a, 0x1a, 0x23 };

/******************************************************************************/
/*!
 * \par  Description:
 *    charge loop
 * \param[in]    none
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * ��紦��������������缰����ѹ���ã��Լ��������
 *******************************************************************************/
void charge_loop(void)
{
    if (((act_readb(SYSTEM_VOL) & 0x80) != 0) && (TheCharge == 1)) //ʹ��SYSTEM_VOL�б�USB����
    {
        if (TrickleTimeCounter == 100)//�رճ�磬������,100��Ϊ��־λ
        {
            BatteryFullCheck();//g_rtc_counter��BatteryFullCheck������
        }
        else
        {
            chargedeal();//g_rtc_counter��chargedeal������
        }
    }
    else
    {
        stop_charge_exit();
        g_rtc_counter = 2900;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    earphone key check
 * \param[in]    unsigned char adc
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
 * ����������״̬
 *******************************************************************************/

void RTC_EarphoneCheck(unsigned char adc)
{

    if (g_speakonoff == 0)
    {
        g_debounce_earphone = 0;
        return;
    }
    if ((adc > 0x2B) && (adc < 0x34))
    {
        //earphone in
        g_debounce_earphone++;
        if (g_debounce_earphone > 3)
        {
            g_debounce_earphone = 0;
            act_writel(act_readl(GPIO_ADAT) & 0xfffffffd, GPIO_ADAT);//enable speak control mute
        }

    }
    else if (adc > 0x39)
    {
        g_debounce_earphone = 0;
        act_writel(act_readl(GPIO_ADAT) | 0x02, GPIO_ADAT);//disable speak control mute
    }
    else
    {
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    void post_Earphone_msg(unsigned char adc)
 * \param[in]    unsigned char adc
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
 * ���������Ϣ����
 *******************************************************************************/
void post_Earphone_msg(unsigned char adc)
{
    bool status;
    bool need_send = FALSE;
    //private_msg_t cur_send_pmsg;
    uint16 msg;
    static uint8 count1 = 0;
    static uint8 count2 = 0;

    if ((adc > 0x2B) && (adc < 0x34))
    {
        count1++;
        count2 = 0;
        //������⵽20  ��
        if (count1 > 20)
        {
            count1 = 0;

            //��ǰ��⵽�ж���
            status = TRUE;

            if (status != g_earphone_status) //��������
            {
                g_earphone_status = TRUE;
                need_send = TRUE;
                //cur_send_pmsg.msg.type = MSG_EARPHONE_IN;
                msg = MSG_EARPHONE_IN;
            }
        }

    }
    else if (adc > 0x39)
    {
        count2++;
        count1 = 0;

        if (count2 > 20)
        {
            count2 = 0;
            //��ǰ��⵽�޶���
            status = FALSE;
            if (status != g_earphone_status) //�����γ�
            {
                g_earphone_status = FALSE;
                need_send = TRUE;
                //cur_send_pmsg.msg.type = MSG_EARPHONE_OUT;
                msg = MSG_EARPHONE_OUT;
            }
        }
    }
    else
    {
    }

    if (need_send == TRUE)
    {
        //�跢�Ͷ��������Ϣ
        //sys_mq_send(MQ_ID_DESK, (void *)&cur_send_pmsg);                 //���͵�ǰ̨˽����Ϣ����
        sys_mq_send(MQ_ID_SYS, &msg); //���͵�ϵͳ��Ϣ����
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Hold key check
 * \param[in]    unsigned char adc
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
 * ���hold��״̬������״̬�仯������Ӧ��hold��Ϣ
 *******************************************************************************/
#ifdef lock_key
void RTC_HoldCheck(unsigned char adc)
{
    if((adc>MIN_HOLD_ADC)&&(adc<MAX_HOLD_ADC))
    {
        //hold on
        TheFirstHold++;
        if(TheFirstHold>3)
        {
            TheFirstHold = 0;
            if(TheLastHoldKey != Msg_KeyHold )
            {
                PutSysMsg(Msg_KeyHold); //����ֵ��Ϣ
                TheLastHoldKey = Msg_KeyHold;
            }
        }

    }
    else if(adc>MAX_HOLD_ADC) //no key

    {
        if(TheLastHoldKey == Msg_KeyHold )
        {
            PutSysMsg(Msg_KeyUnHold); //����ֵ��Ϣ
            TheLastHoldKey = Msg_KeyUnHold;
            TheFirstHold = 0;
        }
    }
    else
    {

    }
}
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *    key scan of times
 * \param[in]    none
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
 * ����ɨ���Լ������ѯ��ÿ��20msɨ��һ�Σ������Ѿ�����ȥ������
 *******************************************************************************/
void key_scan(void)
{

    unsigned char adcdat;
    unsigned char key_val, i;

    //��籣��DC5V����
    if ((act_readb(SYSTEM_VOL) & 0x80) != 0)
    {
        g_rtc_counter++;
        //60s
        if (g_rtc_counter == 3000)
        {
            charge_loop();

        }
    }
    else
    {
        stop_charge_exit();
    }

    key_val = KEY_NULL;

    adcdat = act_readb(LRADC1_DATA) & 0x3f;

    for (i = 0; i < ADC_KEY_NUM; i++)
    {
        if (adcdat < Adc_data[i])
        {
            key_val = key_map[i + 1]; //first key map for play

            break;
        }
    }
    if ((act_readl(SYSTEM_CTL_RTCVDD) & 0x80000000) != 0) //bit6��7��Ϊ0������/�̰�play��ͳһ��play��ת��APȥ����
    {
        key_val = key_map[0];
        adcdat = 0;//skip hold state

    }

    if (key_val == KEY_NULL)
    {
        RTC_EarphoneCheck(adcdat);
        post_Earphone_msg(adcdat);
    }

#ifdef lock_key
    RTCHold_Counter++;
    if(RTCHold_Counter>4)
    {
        RTCHold_Counter = 0;
        RTC_HoldCheck(adcdat);
    }
#endif

    if (key_val != oldkey) //�������ɨ��ֵ�������ϴΰ���ֵ
    {
        if (key_val == tmpkey) //������μ�ֵ����ǰ��ɨ��ֵ
        {
            tmp_count++; //�ۼ�ɨ�����

            if (tmp_count > DOWN_KEY_TIMER)
            {
                tmp_count = DOWN_KEY_TIMER;
            }

            if (tmp_count == DOWN_KEY_TIMER) //ɨ�����Σ���ͬһ��ֵ��ȷ���а�������
            {
                if (key_val != NO_KEY) //�������ɨ�貶���ֵ��Ϊ��
                {
                    oldkey = tmpkey;
                    wait_keyup = 1;
                    PutSysMsg(oldkey);
                }
                else //���μ�ֵΪNO_KEY
                {
                    hold_count = 0;
                    oldkey = NO_KEY;
                    tmpkey = NO_KEY;

                    if (wait_keyup == 1) //�ȴ�KEYUP
                    {
                        wait_keyup = 0;
                        tmp_count = 0; //��ɨ�����
                        PutSysMsg(Msg_KeyShortUp); //��keyup��Ϣ
                    }
                }
            }
        }
        else //�������ɨ��ֵ������ǰ��ɨ��ֵ�������ü�ֵ�����¼���
        {
            tmpkey = key_val;
            if (!wait_keyup)
            {
                tmp_count = 0;
            }
        }
    }
    else //������μ�ֵ�����ϴΰ���ֵ,�ۼ�8��(160ms)����һ�ΰ�����Ϣ
    {
        if (key_val != NO_KEY) //
        {
            hold_count++;

            if (hold_count == HOLD_KEY_TIMER)
            {
                hold_count = 0;
                PutSysMsg(oldkey); //����ֵ��Ϣ
            }
        }
        else
        {
            tmpkey = key_val;
            hold_count = 0;
            if (!wait_keyup)
            {
                tmp_count = 0;
            }
        }
    }

}

