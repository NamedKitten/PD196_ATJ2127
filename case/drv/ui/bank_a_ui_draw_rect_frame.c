/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-09 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_draw_rect_frame.c
 * \brief    ����������Ч�������ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ����ָ������߿������Ч������䡣
 * \par      EXTERNALIZED FUNCTIONS:
 *               ֱ�ӵ���������亯��fill_rectʵ�ֱ߿���䡣
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *	  ����������Ч��������߿�
 * \param[in]    frame_region����Ҫ��������ָ��
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/

void draw_rect_frame(region_t * frame_region)
{
    libc_memcpy(&region, frame_region, sizeof(region_t));
    //���ϱ�
    region.height = 1;
    romf_fill_rect(&region, (uint16) (dc.pen_color & 0xffff));

    //���±�
    region.y = frame_region->y + frame_region->height;
    romf_fill_rect(&region, (uint16) (dc.pen_color & 0xffff));

    libc_memcpy(&region, frame_region, sizeof(region_t));

    //�����
    region.width = 1;
    romf_fill_rect(&region, (uint16) (dc.pen_color & 0xffff));

    //���ұ�
    region.x = frame_region->x + frame_region->width - 1;
    romf_fill_rect(&region, (uint16) (dc.pen_color & 0xffff));

    return;
}
/*! \endcond */

