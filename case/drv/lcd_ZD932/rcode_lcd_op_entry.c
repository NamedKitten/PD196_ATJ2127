/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-20 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     lcd_op_entry.c
 * \brief    lcd�����ܽӿڶ���
 * \author   liutingting
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "lcd_driver.h"

//uint32 ce_bak; //����Ƭѡ��Ϣ
uint32 memclk_ctl1; //����memory ʱ��״̬
uint32 irq_status;
uint8 pic_mode;
uint8 dma_chan;
uint8 trans_mode;
uint8 contrast_grade; //���⼶��
uint8 lcd_draw_mode; //LCD����ʾģʽ
int8 reent_flag = 0; //Ƭѡ��־
//��ǰUIԪ����ʾ����
region_t region_setwin;//set window //modify--vzhan

typedef void*(*lcd_op_func)(void*, void*, void*);

typedef struct
{
    lcd_op_func set_window;
    lcd_op_func set_contrast;
    lcd_op_func set_draw_mode;
    lcd_op_func standby_screen;
    lcd_op_func backlight_on_off;
    lcd_op_func update_screen;
    lcd_op_func buff_data_trans;
    lcd_op_func get_buff_data;
    lcd_op_func invert_buff_data_trans;
    lcd_op_func controller_init;
    lcd_op_func dma_set_counter;
    lcd_op_func dma_start_trans;
    lcd_op_func dma_set_src_addr;
    lcd_op_func mcu_set_JRAM_clk;
    lcd_op_func restore_JRAM_clk;
} lcd_driver_operations;

lcd_driver_operations lcd_driver_op =
{      
    (lcd_op_func) set_window, (lcd_op_func) set_contrast, (lcd_op_func) set_draw_mode, (lcd_op_func) standby_screen,
    (lcd_op_func) backlight_on_off, (lcd_op_func) update_screen, (lcd_op_func) buff_data_trans,
    (lcd_op_func) get_buff_data, (lcd_op_func) invert_buff_data_trans, (lcd_op_func) controller_init,
    (lcd_op_func) dma_set_counter, (lcd_op_func) dma_start_trans, (lcd_op_func) dma_set_src_addr,
    (lcd_op_func) mcu_set_JRAM_clk, (lcd_op_func) restore_JRAM_clk 
};

