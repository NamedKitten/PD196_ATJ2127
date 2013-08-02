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
 * \file     ui_draw_3Drect.c
 * \brief    ����������Ч�������ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ���˾�����ɫЧ��ָ���������䡣
 * \par      EXTERNALIZED FUNCTIONS:
 *               ֱ�ӵ���������亯��fill_rectʵ��ָ���������䡣
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *	  ʵ������������Ч��������
 * \param[in]    D3region��ָ��Ҫ��������
 * \param[in]    type��Ҫ����Ч������
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void draw_3D_rect(region_t * D3region, uint8 type)
{
    COLORREF pen_color_bak;
    pen_color_bak = dc.pen_color;//���浱ǰ������ɫ

    switch (type)
    {
        case D3_NORMAL_DEF://��ѡ�п�
        dc.pen_color = SelectedColor_def;
        draw_rect_frame(D3region);
        break;

        case D3_KEYUP_DEF://��ͻ��Ч��
        //�Ȼ����������
        dc.pen_color = BackLightColor_def;
        libc_memcpy(&region, D3region, sizeof(region_t));
        region.height = 1;
        romf_fill_rect(&region, BackLightColor_def);

        libc_memcpy(&region, D3region, sizeof(region_t));
        region.width = 1;
        region.x = D3region->x + D3region->width;
        romf_fill_rect(&region, BackLightColor_def);

        //�ٻ���������
        dc.pen_color = FaceLightColor_def;
        libc_memcpy(&region, D3region, sizeof(region_t));
        region.height = 1;
        romf_fill_rect(&region, FaceLightColor_def);

        libc_memcpy(&region, D3region, sizeof(region_t));
        region.width = 1;
        romf_fill_rect(&region, FaceLightColor_def);
        break;

        case D3_KEYDOWN_DEF://�Ӱ���Ч��
        //�Ȼ����������
        dc.pen_color = BackLightColor_def;
        libc_memcpy(&region, D3region, sizeof(region_t));
        region.height = 1;
        romf_fill_rect(&region, BackLightColor_def);

        libc_memcpy(&region, D3region, sizeof(region_t));
        region.width = 1;
        romf_fill_rect(&region, BackLightColor_def);

        //�ٻ���������
        dc.pen_color = FaceLightColor_def;
        libc_memcpy(&region, D3region, sizeof(region_t));
        region.height = 1;
        region.y = D3region->y + D3region->height;
        romf_fill_rect(&region, FaceLightColor_def);

        libc_memcpy(&region, D3region, sizeof(region_t));
        region.width = 1;
        region.x = D3region->x + D3region->width;
        romf_fill_rect(&region, FaceLightColor_def);
        break;

        default:
        break;
    }

    dc.pen_color = pen_color_bak;//�ָ�������ɫ
    return;
}
/*! \endcond */

