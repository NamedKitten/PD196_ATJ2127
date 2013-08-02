/*******************************************************************************
 *                              US212A
 *                            Module: FM driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       Mikeyang     2011-09-20 10:00     1.0             build this file
 *******************************************************************************/

#include "fm_interface.h"
#include "fm_drv.h"

gpio_init_cfg_t gpio_i2c[];

/******************************************************************************
 *  FM ��ģ����������ռ����˵��
 *  1.  ������Ϊdrv  ���أ���ǳ�פ�������������һ��
 *      ���Է�ΪBANKA,  BANKB �������BANK �ռ�
 *  2.  ����FM ������פ��������ݣ�ϵͳδ�滮�̶��ڴ棬
 *       �Ҽ���FM  ��������(��̨FM  ����ʱ����̨BASAL ����Ƶ����
 *       �ȿռ���п���)�����ݹ滮
 *  FM ������פ����ռ� SRAM_AP_BACK_CODEC_RCODE_ADDR  0x29000��4K
 *  FM ������פ���ݿռ� AP_BACK_DATA 1.5k ��ǰ��0.75k ��FM ����AP
 *                                                                                     ����0.75K ��FM ����
 *******************************************************************************
 **/

/* I2C ����ָ��*/
gpio_init_cfg_t *I2C_gpio = (gpio_init_cfg_t *) gpio_i2c;

/* I2C ����ģ��GPIO ��Ϣ�ṹ*/
gpio_init_cfg_t gpio_i2c[2] =
{
     //SCL==GPIO_A15
    { GPIO_AINEN, GPIO_AOUTEN, GPIO_ADAT, GPIO_SCL_BIT },
    //SDA==GPIO_A16
    { GPIO_AINEN, GPIO_AOUTEN, GPIO_ADAT, GPIO_SDA_BIT } 
};

/* I2C ������ʱ*/
uint8 delay_i2c = PARAM_DELAY_I2C;

/*FM ��������ӿں���*/
fm_driver_operations fm_drv_op =
{ (fm_op_func) sFM_Init, (fm_op_func) sFM_Standby, (fm_op_func) sFM_SetFreq,
        (fm_op_func) sFM_GetStatus, (fm_op_func) sFM_Mute, (fm_op_func) sFM_Search,
        (fm_op_func) sFM_HardSeek, (fm_op_func)sFM_SetBand,
        (fm_op_func)sFM_SetThrod, (fm_op_func)sFM_BreakSeek,
        (fm_op_func)sFM_GetHardSeekflag, (fm_op_func)sFM_GetBand,
        (fm_op_func)sFM_GetFreq, (fm_op_func)sFM_GetIntsity,
        (fm_op_func)sFM_GetAnten, (fm_op_func)sFM_GetStereo,
};

