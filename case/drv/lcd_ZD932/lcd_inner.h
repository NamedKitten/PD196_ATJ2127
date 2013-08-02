/*******************************************************************************
 *                              us212A
 *                            Module: ui����
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-9 16:23     1.0             build this file
 *******************************************************************************/
/*!
 * \file     lcd_driver.h
 * \brief    lcd��������ģ��������ݽṹ���궨�壬�ӿ�������
 * \author   liutinging
 * \version 1.0
 * \date  2011-9-9
 *******************************************************************************/

#ifndef _LCD_INNER_H
#define _LCD_INNER_H

#include "psp_includes.h"
#include "lcd_driver.h"

#ifdef _FPGA_

/*! LCDģ��ʵ�ʵķֱ��ʵ�ʵ�ʳ��� */
#define LCD_WIDTH 132
/*! LCDģ��ֱ��ʵ�ʵ�ʿ�� */
#define LCD_HEIGHT 162

/*! 132X162 ��LCD������ʾʱ�������LCDģ�鳤�ȵ���ʼƫ�� */
#define LCD_WIDTH_OFFSET  2
/*! 32X162 ��LCD������ʾʱ�������LCDģ���ȵ���ʼƫ�� */
#define LCD_HEIGHT_OFFSET 1

#else
/*! LCDģ��ʵ�ʵķֱ��ʵ�ʵ�ʳ��� */
#define LCD_WIDTH 128
/*! LCDģ��ֱ��ʵ�ʵ�ʿ�� */
#define LCD_HEIGHT 64//160 //modify--vzhan

/*! 132X162 ��LCD������ʾʱ�������LCDģ�鳤�ȵ���ʼƫ�� */
#define LCD_WIDTH_OFFSET  0
/*! 32X162 ��LCD������ʾʱ�������LCDģ���ȵ���ʼƫ�� */
#define LCD_HEIGHT_OFFSET 0

#endif

#define LCD_CMD_STANDBY         0x10
#define LCD_CMD_EXIT_SLEEP      0x11
#define LCD_CMD_DISPLAY_OFF     0x28
#define LCD_CMD_DISPLAY_ON      0x29
#define LCD_CMD_SET_X           0x2a
#define LCD_CMD_SET_Y           0x2b
#define LCD_CMD_WRITE           0x2c
#define LCD_CMD_READ            0x2e
#define LCD_CMD_SETMODE         0x36
//display mode

//�ڲ�����
void store_ce(void)__FAR__;
void restore_ce(void)__FAR__;
void store_ce_gio(void)__FAR__;
void restore_ce_gio(void)__FAR__;
void write_data(uint8 *data_buf, uint16 pix_cnt)__FAR__;
void write_command(uint8 cmd)__FAR__;
void set_mfp_to_emif(void)__FAR__;
void set_mfp_to_lcd(void)__FAR__;
void dma_set_RAM_clk(uint8 dma_ram)__FAR__;
void lcd_hardware_init(void)__FAR__;

#ifdef __WELCOME__
void welcome_delay_us(uint8 dly_us);
void welcome_delay_ms(uint8 dly_ms);
#endif//__WELCOME__

extern region_t region_setwin;
#endif //_LCD_INNER_H
