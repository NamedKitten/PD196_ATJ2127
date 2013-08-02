/*
********************************************************************************
*                       US212A
*                  I2C  head file
*                                                                         
*                (c) Copyright 2002-2003, Actions Co,Ld.                       
*                        All Right Reserved                               
*
* File   : I2C.h                                                   
* By     : Mikeyang
* Version: 1> v1.00     first version     2003-11-20 9:14
********************************************************************************
********************************************************************************
*/
#ifndef _I2C_H
#define _I2C_H

#include "psp_includes.h"


/* ģ��IIC ����GPIO ���ú�*/
#define  IIC_SCL_BIT         13                       //22     //13     //15
#define  IIC_SDA_BIT        12   //21                       //21    //12    //16
#define  GPIO_SCL_BIT    (0x00000001<<IIC_SCL_BIT)
#define  GPIO_SDA_BIT    (0x00000001<<IIC_SDA_BIT)


/*IIC GPIO ����ṹ */
typedef struct
{
    uint32 reg_in;                            //����ʹ�ܼĴ���
    uint32 reg_out;                         //���ʹ�ܼĴ���
    uint32 reg_data;                       //���ݼĴ���
    uint32 reg_bit;	                         //bit λ
}gpio_init_cfg_t;


#endif /*_I2C_H */

