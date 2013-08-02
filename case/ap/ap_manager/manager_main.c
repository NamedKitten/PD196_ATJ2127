/*******************************************************************************
 *                              US212A
 *                            Module: manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_main.c
 * \brief    manager��main����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "manager.h"
#include  <card.h>
#include  <sysdef.h>

#ifndef PC
OS_STK *ptos = (OS_STK *) AP_PROCESS_MANAGER_STK_POS;
#endif
INT8U prio = AP_PROCESS_MANAGER_PRIO + 1;

extern app_result_e manager_message_loop(void)__FAR__;
extern bool globe_data_init(void) __FAR__;
extern uint16 get_config_default(uint16 config_id);
void system_config(void);

typedef void (*handler_ker)(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ�õ���ں���
 * \param[in]    δʹ��
 * \param[out]   none
 * \return       ��Զ�����˳�
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    system_config();

    //��ʼ��globeȫ�����ݣ����������ýű� config.bin
    globe_data_init();

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_MANAGER, APP_TYPE_PMNG);

    //��ʼ�� applib ��Ϣģ��
    sys_mq_flush(MQ_ID_MNG);

    //����configӦ��
    sys_exece_ap("config.ap", 0, 0);
    
    manager_message_loop();
    return 0;
}


/******************************************************************************/
/*!
 * \par  Description:
 *    system config
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void system_config(void)
{
    /*for card configuration.  xiaomaky.  2013/2/19 13:49:32.*/
    /* 
    bit0: 0--1_line, 1--4_line; 
    bit1(sdvccout power): 0--direct, 1--GPIO; 
    bit2(DAT3 pull high): 0--hardware pull, 1--GPIO pull;
    bit3(write_fast):0--not continuous, 1--continuous write;
    */
#ifndef PC
	g_card_para = 0x07;
#endif
    
#ifndef PC
    handler_ker detect_card_isr_ker = (handler_ker)GLOBAL_DETECT_CARD_ISR_ADDR;//ָ��kernel�̻��ӿ� detect_card_isr
#endif
    
    /* for GPIOA19 card-detect.  need to enable PU as soon for be stable.   */
    act_writel((~(0x01 << 19)) & act_readl(GPIO_AOUTEN), GPIO_AOUTEN);  /* disable output.  */
    act_writel((0x01 << 19) | act_readl(GPIO_AINEN), GPIO_AINEN);       /* enable input.  */
    act_writel((~(0x01 << 19)) & act_readl(GPIO_APDEN), GPIO_APDEN);    /* disable 50K PD.  */
    act_writel((0x01 << 19) | act_readl(GPIO_APUEN), GPIO_APUEN);      /* 50K PU Enable, ����û�п�ʱĬ��Ϊ��.  */

#ifndef PC    
    sys_set_irq_timer1(detect_card_isr_ker, 20);
#endif

#ifndef PC 
    /* for uart-gpio, 0x9fc19a88--uart_tx_gpio_cfg_mask,  0x9fc19a8c--uart_tx_gpio_cfg_value.  */
    GLOBAL_UART_TX_GPIO_CFG_MASK = 0xFFFF1FFF;//GPIO A21
    GLOBAL_UART_TX_GPIO_CFG_VALUE = 0x00006000; //GPIO A21
//    GLOBAL_UART_TX_GPIO_CFG_MASK = 0xFFFFFFC7;//GPIO A17
//    GLOBAL_UART_TX_GPIO_CFG_VALUE = 0x00000020; //GPIO A17
#endif

    /*ϵͳ��Ƶʱ�����ٵ���VCC��ѹ��ϵͳ��ʼ��Ϊ3.0V��Ӧ�ÿ��Ը������󣬽�����������ʵ�λ�����磬Ϊ��EJTAG���ԣ���Ϊ3.3V.*/
    act_writel((0xFFFFFF8F&act_readl(VOUT_CTL))|0x40, VOUT_CTL);
    
    /*for EMI setting.  */
//    act_writel(0x5c92 | (0xffff0000 & act_readl(MCUPLL_DEBUG)), MCUPLL_DEBUG);      /* 0x5c12\0x5c92\0x5d12\0x5d92.   */
//    act_writel(act_readl(MCUPLL_DEBUG)&0xffffbfff, MCUPLL_DEBUG);          /*disable MCUPLL-EMI setting.  */
//    GLOBAL_KERNEL_BACKDOOR_FLAG = GLOBAL_KERNEL_BACKDOOR_FLAG | 0x01;   /* bit0----if is 1, for pass EMI, not change nand_pad_drv according to frequency;   */
//    GLOBAL_KERNEL_BACKDOOR_FLAG = GLOBAL_KERNEL_BACKDOOR_FLAG | 0x10;   /* bit4----if is 1, for pass EMI, Nand/Card clock is fixed HOSC, not according to MCUPLL;    */
//    GLOBAL_KERNEL_BACKDOOR_FLAG = GLOBAL_KERNEL_BACKDOOR_FLAG | 0x20;   /* bit5----if is 1, for pass EMI, UDisk AP set clock and USB LIB not;  */
    
    
    //��Ƶ����Ϊ24M
    sys_adjust_clk(FREQ_24M, 0);
}
