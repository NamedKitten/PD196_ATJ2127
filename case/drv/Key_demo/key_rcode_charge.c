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
 * \file     key_rcode_charge.c
 * \brief    charge flow control
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

#include "key_inner.h"

#define  BATFAST_VEL     0x60//50 mA����� 3.6V��Ϊ�͵���1.5���ӣ�250 mA*5 = 350 mA*3.5��

uint8 TheCharge; //����ʶ��0,stop��1��start
uint8 ChargeCurrent; //������ѡ��
uint8 TrickleTimeCounter = 0; //������������30�����
uint8 BatteryFullFlag = 0; //������ʶ
uint8 LowestCurrentCnt = 0;
uint8 CurrentSetFlag = 0;
uint8 BatteryRefVol; //����ѹֵ
uint8 LowCurrentChargeCounter;
uint8 ChargeCurrent_backup;
uint8 FastChargeFlag;
uint8 FastChargeCounter;

/******************************************************************************/
/*!
 * \par  Description:
 *    battery full check
 * \param[in]    none
 * \param[out]   uint8
 * \return       the result
 * \retval           0 battery is not full
 * \retval           1 battery is full
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
uint8 BatteryFullCheck(void)
{
    g_rtc_counter = 0;
    
    //���ڽӽ������ʱ�ż�������ֱ�ӷ���
    if ((TrickleTimeCounter != 100) && (act_readb(BATADC_DATA) < BatteryRefVol))
    {
        return 0;
    }
    if (TrickleTimeCounter < 5) //5min������
    {
        //3.2.1.1.1.1.1.1 δ�ӳ䣬�ӳ��ʱ���ۼ�1���˳�
        TrickleTimeCounter++;
        return 0;
    }

    if ((LowCurrentChargeCounter == 0) && (act_readb(CHG_CTL) & 0x20))//������ǰ���ȹرճ����ƣ�ȷ�������������ȷ��
    {
        act_writeb((act_readb(CHG_CTL) & 0xdf), CHG_CTL);
        TrickleTimeCounter = 100; //delay
        g_rtc_counter = 2950; //�ȴ�(3000-2950)*20ms=1s����м�⣬���û�����������������
        return 0;
    }
    
    if ((LowCurrentChargeCounter > 0) || (act_readb(BATADC_DATA) < BatteryRefVol))//δ�������磬�ָ����
    {
        if(LowCurrentChargeCounter == 0)
        {
            ChargeCurrent_backup = act_readb(CHG_CTL) & 0x0f;//������ͣ������
            //��50mAʹ�ܳ�磬�ȴ�200ms
            act_writeb(0x32, CHG_CTL);
        }
        
        //3.2.1.1.1.1.1.2.1.1 ����򿪳���·����ʼ���ӳ䶨ʱ�����˳�
        LowCurrentChargeCounter++;
        if(LowCurrentChargeCounter >= 10)
        {
            uint8 tmp1;
            tmp1 = (act_readb(CHG_CTL) & 0xf0) | 0x0f;
            act_writeb((ChargeCurrent_backup | 0xf0) & tmp1, CHG_CTL);
            
            TrickleTimeCounter = 0; //delay
            LowCurrentChargeCounter = 0;
        }
        else
        {
            g_rtc_counter = 2999;//20ms��ʱ
        }
        
        return 0;
    }
    else
    {
        BatteryFullFlag = 1;
        TrickleTimeCounter = 0;
        stop_charge_exit();
        return 1;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *    charge flow deal
 * \param[in]    none
 * \param[out]   none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * �����紦������ᱻ��ʱ����ѯ�ġ�
 *******************************************************************************/

void chargedeal(void)
{
    uint8 tmp1;
    
    g_rtc_counter = 0;
    FastChargeCounter++;

    //3.2����ص�ѹ����3.0V���ж��Ƿ����趨�û�������
    if (CurrentSetFlag == 1)
    {
        //3.2.1�����趨�û����������status�Ƿ�Ϊ0
        if ((act_readb(CHG_DET) & 0x01) == 0)
        {
            //3.2.1.1 status=0���жϳ������Ƿ����25ma
            if ((act_readb(CHG_CTL) & 0x0f) > ChargeCurrent25mA)
            {
                //3.2.1.1.1 ����>25mA��˵�������������㣬�𼶼�С50ma������
                LowestCurrentCnt = 0; //�͵��������0
                tmp1 = (act_readb(CHG_CTL) & 0x0f) - 1;
                act_writeb((act_readb(CHG_CTL) & 0xf0) | tmp1, CHG_CTL);

                BatteryFullCheck();
            }
            else
            {
                //3.2.1.1.2 ����<25mA���͵��������һ
                LowestCurrentCnt++;

                //3.2.1.1.2.1 �жϵ͵�������Ƿ�>3
                if (LowestCurrentCnt > 3)
                {
                    //������>3��������ع��䱻�������߸�����������
                    //�ó��������ֹͣ���˳�
                    BatteryFullFlag = 1;
                    stop_charge_exit();
                }
                else
                {
                    //������<3
                    //ת��3.2.1.1.1.1������ص�ѹ>4.2V
                    BatteryFullCheck();
                    //goto BatFullCheck;
                }
            }
        }
        else
        {
            if((FastChargeFlag == 1) && (FastChargeCounter >= 5))
            {
                //�����û������� 
                tmp1 = (act_readb(CHG_CTL) & 0xf0) | 0x0f;
                act_writeb((ChargeCurrent | 0xf0) & tmp1, CHG_CTL);
                FastChargeFlag = 0;
            }
                
            //3.2.1.2 status!=0,��ת3.2.1.1.1.1������ѹ(BATADC)�Ƿ�>4.2V
            //goto BatFullCheck;
            BatteryFullCheck();
        }
    }
    else
    {
        //3.2.2��δ�趨�û�����������ص�ѹ�Ƿ�<4.2V
        if ((LowCurrentChargeCounter > 0) || (act_readb(BATADC_DATA) < BatteryRefVol))
        {
            //�����������50mAʹ�ܳ�磬�ȴ�200ms
            if(LowCurrentChargeCounter == 0)
            {
                act_writeb(0x32, CHG_CTL);
                act_writeb((act_readb(CHG_ASSISTANT) | 0x01), CHG_ASSISTANT);//4.23V
            }
            
            LowCurrentChargeCounter++;
            if(LowCurrentChargeCounter >= 10)
            {
                //�͵��磬������ٳ��״̬��������Ϊ 350 ma
                if(act_readb(BATADC_DATA) < BATFAST_VEL)
                {
                    //�����û������� 350 ma
                    tmp1 = (act_readb(CHG_CTL) & 0xf0) | 0x0f;
                    act_writeb(0xf8 & tmp1, CHG_CTL);
                    FastChargeCounter = 0;
                    FastChargeFlag = 1;//���ٳ���־
                }
                else
                {
                    //�����û�������
                    tmp1 = (act_readb(CHG_CTL) & 0xf0) | 0x0f;
                    act_writeb((ChargeCurrent | 0xf0) & tmp1, CHG_CTL);
                }
                
                LowCurrentChargeCounter = 0;
                CurrentSetFlag = 1; //�����û��������ñ�ʶ
            }
            else
            {
                g_rtc_counter = 2999;//�ȴ�20ms���½���
            }
        }
        else
        {
            //3.2.2.2 ��ѹ>4.2V��˵���������,�ó��������־
            BatteryFullFlag = 1;
            stop_charge_exit();
        }
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *    charge stop deal
 * \param[in]    none
 * \param[out]   none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *
 *******************************************************************************/
void stop_charge_exit(void)
{
    act_writeb(act_readb(CHG_CTL) & 0xDF, CHG_CTL);//ֹͣ���
    TheCharge = 0;
    CurrentSetFlag = 0;
    LowestCurrentCnt = 0;
}
