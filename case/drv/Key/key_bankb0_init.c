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
 * \file     key_init.c
 * \brief    key driver initial functions
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

//#include "gpio_opr.h"
#include "key_inner.h"

/******************************************************************************/
/*!
 * \par  Description:
 *	  KEY����װ�أ�������ʼ���������ж�������װ�ȡ�
 * \param[in]    none
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \ingroup      key_install
 * \note
 * \li  �ýӿ�������װ��ʱ��ϵͳ�Զ����á�
 *******************************************************************************/
int key_init(void)
{
    unsigned int tmp;

    //��ʼ���߿ذ��� LRADC1 ���ú���Ҫ�ȴ�10ms���ܶ�����ȷ�����ݣ�
    //�������ȫ0,����ŵ�welcome���г�ʼ��������Ҫ�ȴ�10ms
    tmp = act_readl(PMUADC_CTL);
    tmp |= (unsigned int) (0x1 << 5); //enable LRADC1/3/4/5
    act_writel(tmp, PMUADC_CTL);
    sys_mdelay(20);
    //Ĭ��Play1s������ʹ��Ĭ��ֵ
    time_irq = sys_set_irq_timer1(key_scan, 0x02);

    for (tmp = 0; tmp < 16; tmp++)
    {
        key_map[tmp] = tmp;
    }

    g_rtc_counter = 2900;
    act_writeb(act_readb(CHG_CTL) & 0xDF, CHG_CTL);//ֹͣ���
    TheCharge = 0;
    act_writel(act_readl(GPIO_AOUTEN) |0x02, GPIO_AOUTEN);//enable speak control
    act_writel(act_readl(GPIO_ADAT) &0xfffffffd, GPIO_ADAT);//enable speak control mute
    g_speakonoff = 0;
    act_writel(act_readl(GPIO_ADAT) &0xfffffffd, GPIO_ADAT);//enable speak control mute
    
    return 0;
}

module_init(key_init)
