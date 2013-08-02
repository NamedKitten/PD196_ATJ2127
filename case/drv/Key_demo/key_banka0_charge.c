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
 * \file     key_banka_charge.c
 * \brief    Get and set charge state
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/
#include "key_inner.h"

extern uint8 LowCurrentChargeCounter;
extern uint8 FastChargeFlag;

#define     BATLOW_VEL      0x44    //3.0V�����͵�
#define     BATNON_VEL      0x5d    //3.6V,����޵��
/******************************************************************************/
/*!
 * \par  Description:
 *    Get charge state
 * \param[in]      none reserve 0
 * \param[in]      none reserve 0
 * \param[in]      none reserve 0
 * \param[out]   charge state
 * \return       the result
 * \retval           0 sucess
 * \retval           1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
charge_state_e key_inner_chargeget(void* null0, void* null1, void* null2)
{
    //uint8 setting = (uint8)(param1);
    charge_state_e ret = CHARGE_NONE;

    //get charge status
    if (TheCharge == 1)
    {
        ret = CHARGE_CHARGING;

    }
    else
    {
        if (BatteryFullFlag == 1)
        {
            ret = CHARGE_FULL;
        }
        else if (BatteryFullFlag == 2)
        {
            ret = CHARGE_NOBATTERY;
        }
        else
        {
            
        }
    }

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Set charge
 * \param[in]      setting CHARGE_START:start charging CHARGE_STOP:stop charging
 * \param[in]      current  choose from charge_current_e
 * \param[in]      param3   choose from battery_full_t
 * \param[out]   charge state
 * \return       the result
 * \retval           0 sucess
 * \retval           1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
uint8 key_inner_chargeset(charge_control_e setting, charge_current_e current, battery_full_t param3)
{
    uint8 ret = 0;

    if (setting == CHARGE_START)
    {
        //start charge,set charge flag
        TheCharge = 1;
        ChargeCurrent = current;
        BatteryRefVol = param3;
        BatteryFullFlag = 0;
        g_rtc_counter = 2900; //���ٽ��������
        CurrentSetFlag = 0; //�����û��������ñ�ʶ
        LowCurrentChargeCounter = 0;
        FastChargeFlag = 0;
        //3.����ص�ѹ�Ƿ�С��3.0V,�ж��Ƿ���Ҫ������޵��
        if (act_readb(BATADC_DATA) < BATLOW_VEL)
        {
            //3.1��С��3.0V������������򿪳�����50ma��磬delay 500ms
            act_writeb((((act_readb(CHG_CTL) & 0xf0) | 0x02) | 0x10), CHG_CTL);
            act_writeb(act_readb(SYSTEM_VOL) | 0x60, SYSTEM_VOL);
            act_writeb((act_readb(CHG_CTL) | 0x20), CHG_CTL);
            sys_mdelay(500);

            //3.1.1����ص�ѹ�Ƿ�С��3.6V
            if (act_readb(BATADC_DATA) < BATNON_VEL)
            {
                g_rtc_counter = 0; //���ٽ��������
            }
            else
            {
                //no battery
                stop_charge_exit();
                BatteryFullFlag = 2; //��Ϊ�޵�ر�־
            }
        }
    }
    else //CHARGE_STOP
    {
        //stop charge, clr charge flag
        //TheCharge = 0;
        stop_charge_exit();
    }

    return ret;
}

