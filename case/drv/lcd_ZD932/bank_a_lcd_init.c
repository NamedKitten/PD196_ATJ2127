/*******************************************************************************
 *                              US212A
 *                            Module: lcd driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-22 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     lcd_init.c
 * \brief    lcd initģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               lcd init
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "lcd_inner.h"
/*! \cond LCD_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *	  LCD��ʼ��װ��
 * \param[in]   none
 * \param[out]  none
 * \return      ��ʼ���Ƿ�ɹ�
 * \retval      1: ��ʼ���ɹ�
 * \retval      0: ��ʼ���ɹ�
 * \ingroup     lcd_install
 * \note
 *******************************************************************************/
int lcd_init(void *null0, void *null1, void *null2)
{
    trans_mode = 0;
    lcd_draw_mode = 0; //LCD����ʾģʽ
    reent_flag = 0; //Ƭѡ��־
    return 0;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  LCDж��
 * \param[in]   none
 * \param[out]  none
 * \return      ж���Ƿ�ɹ�
 * \retval      1: ��ʼ���ɹ�
 * \retval      0: ��ʼ���ɹ�
 * \ingroup     lcd_install
 * \note
 *******************************************************************************/
int lcd_exit(void *null0, void *null1, void *null2)
{

    //	sys_free_dma_chan((uint32)dma_chan);
    return 0;
}
/*! \endcond */

#ifndef PC
module_init(lcd_init)
module_exit(lcd_exit)
#endif

