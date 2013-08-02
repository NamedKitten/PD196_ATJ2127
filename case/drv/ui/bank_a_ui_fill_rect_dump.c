/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       cailizhen     2012-1-12 9:42     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_fill_rect_dump.c
 * \brief    ��ָ��ͼ���ظ��������
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2012-2-17
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ָ����������ָ����ɫ��������������ʾSD BMPͼƬ
 * \param[in]    fill_region��ָ��Ҫ��������
 * \param[in]    color�������ɫ
 * \param[out]   none
 * \return       ��������Ƿ�ɹ�
 * \note         ��һЩͼ�λ�������������Ϸ���У�����Ƿǳ������Ľӿڣ�����Ҳ�̻���ȥ��
 *******************************************************************************/
bool fill_rect_dump(region_t *fill_region, uint16 pic_id, fill_dump_direction_e direction)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint32 cnt, buf_cnt;
    uint32 count;
    uint32 pic_len;
    uint16 i;
    bool result = TRUE;

    //����ͼƬ���ԣ��������������
    res_open_infor_t *res_ptr;
    res_entry_t res_entry;
    display_buffer_deal();
    //��ȡͼƬ��Դ��Ϣ
    res_ptr = res_ptr_select(pic_id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(pic_id) * RESHEADITEM));
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));

    pic_len = res_entry.width * res_entry.height * 2;

    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_entry.offset);
    sys_sd_fread(res_ptr->res_fp, display_buffer, pic_len);

    //���³�ʼ��DMAΪRGBģʽ
    lcd_controller_init(RGB_MODE);

    if (direction == DUMP_LEFT2RIGHT)//ͼƬ
    {
        fill_region->height = res_entry.height;
#ifndef PC
        if (rotate_flag == FALSE)
        {//�����ֱ�����Ͻ��䣬ֻ��Ӧ�������ص��ȣ������ص�����ʾ�����
            lcd_set_draw_mode(DRAW_MODE_H_SCROLL_DEF); //��������ģʽ�������ݽ����п�������������ģʽ����ˢ��
        }
        else
        {
            lcd_set_draw_mode(DRAW_MODE_V_DEF); //���ú���ģʽ
        }
#else
        lcd_set_draw_mode(DRAW_MODE_H_DEF); //��������ģʽ
#endif
    }
    else if (direction == DUMP_DOWN2UP)
    {
        fill_region->width = res_entry.width;
#ifndef PC
        if (rotate_flag == FALSE)
        {
            lcd_set_draw_mode(DRAW_MODE_H_DEF); //��������ģʽ
        }
        else
        {
            lcd_set_draw_mode(DRAW_MODE_V_DEF); //���ú���ģʽ
        }
#else
        lcd_set_draw_mode(DRAW_MODE_H_DEF); //��������ģʽ
#endif
    }
    else
    {
        //do nothing for QAC
    }

    lcd_set_window(fill_region);

    //��ʼ�����������

    //��ʼ�����ݴ�С����������ַ�����������
    cnt = ((uint32) fill_region->width) * ((uint32) fill_region->height) * 2;
    //���Ҫ������������С�������ʼ����ô�໺����
    buf_cnt = display_buf_len;
    if (cnt < buf_cnt)
    {
        buf_cnt = cnt;//�����ʼ���պø����������Ļ�����
    }
    if (pic_len != 0)
    {
        buf_cnt = buf_cnt - (buf_cnt % pic_len);
        for (i = 1; i < (buf_cnt / pic_len); i++)
        {
            libc_memcpy(display_buffer + pic_len * i, display_buffer, pic_len);
        }
    }
    //�ֶ�����
    while (cnt > 0)
    {
        //��ȡ����
        if (cnt >= buf_cnt)
        {
            count = buf_cnt;
        }
        else
        {
            count = cnt;
        }

        //DMA��GRAM��
        if (dma_buffer_data_hook(count / 2, fill_region->width) == FALSE)
        {
            result = FALSE;
            break;
        }

        //����ʣ�����ݴ�С���´ζ�ȡ��ַ
        cnt = cnt - count;
    }

    return result;
}
/*! \endcond */
