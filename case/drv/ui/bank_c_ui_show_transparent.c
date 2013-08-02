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
 * \file     ui_show_transparent.c
 * \brief    ��ʾ͸��ͼ��
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *               ʵ����͸��ͼ�����ʾ���������֣��������־Ͳ�����Ҫ�����ڱ�����ɫ�����Ǵ�ɫ�ġ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-7-30
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

void turn_pixel(uint8 *pixel_buffer, region_t *turn_region);
void pic_overlay(uint8 *pixel_result, uint8 *pic_source, region_t *b_region, region_t *p_region, \
transparent_pic_t *p_trs_pic);
bool put_image(region_t *img_region);

/*! \cond */

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾ͸��ͼ��
 * \param[in]    p_trs_pic ͸��ͼ����ʾ���ƽṹ��
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \par          exmaple code
 * \code
 * \endcode
 * \note
 *******************************************************************************/
void show_pic_transparent(transparent_pic_t *p_trs_pic)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint8 *display_buffer_tmp = display_buffer + 512;
    res_open_infor_t *back_ptr, *res_ptr;
    res_entry_t back_entry, res_entry;
    region_t back_region, pic_region;
    uint32 back_data_col, back_data_len, pic_data_len;

    //����ͼƬ
    back_ptr = res_ptr_select(p_trs_pic->back_id);
    sys_sd_fseek(back_ptr->res_fp, SEEK_SET, \
    back_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(p_trs_pic->back_id) * RESHEADITEM));
    sys_sd_fread(back_ptr->res_fp, &back_entry, sizeof(res_entry_t));
    
    //���㰴�����ȴ洢ͼƬ����ƫ��λ��
    if(p_trs_pic->direct == 0)
    {
        back_data_col = back_entry.offset + (back_entry.width * back_entry.height * 2);
        //4�ֽڶ���
        if((back_data_col % 4) != 0)
        {
            back_data_col = (back_data_col & 0xfffffffc) + 4;
        }
        back_data_col += (p_trs_pic->pic_x - p_trs_pic->back_x)*back_entry.height*2;
    }
    else
    {
        back_data_col = back_entry.offset;
        back_data_col += (p_trs_pic->pic_y - p_trs_pic->back_y)*back_entry.width*2;
    }
    
    //����ͼƬ
    res_ptr = res_ptr_select(p_trs_pic->pic_id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, \
    res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(p_trs_pic->pic_id) * RESHEADITEM));
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
    //ǰ������ͼƬ�Ĵ�С
    pic_data_len = res_entry.width * res_entry.height * 2;
    pic_region.x = p_trs_pic->pic_x;
    pic_region.y = p_trs_pic->pic_y;
    pic_region.width = res_entry.width;
    pic_region.height = res_entry.height;

    /*�贰��������*/
    if(p_trs_pic->direct == 0)
    {
        back_region.x = p_trs_pic->pic_x;
        back_region.y = p_trs_pic->back_y;
        back_region.width = res_entry.width;
        back_region.height = back_entry.height;//����߶Ȳ�һ��
    }
    else
    {
        back_region.x = p_trs_pic->back_x;
        back_region.y = p_trs_pic->pic_y;
        back_region.width = back_entry.width;//�����Ȳ�һ��
        back_region.height = res_entry.height;
    }
    
    //����ͼƬ��ȡ���ݴ�СΪ�����С
    back_data_len = back_region.width * back_region.height * 2;
    
    //��ȡ����ͼƬ����
    sys_sd_fseek(back_ptr->res_fp, SEEK_SET, back_data_col);
    sys_sd_fread(back_ptr->res_fp, display_buffer, back_data_len);
    
    //����ͼƬ����ת��
    if(p_trs_pic->direct == 0)//�������Ҫ����任
    {
        turn_pixel(display_buffer, &back_region);
    }
    
    //��ȡ����ͼƬ����
    if(p_trs_pic->clear == FALSE)//͸��ͼ�꣬����ͼ���͸������
    {
        sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_entry.offset);
        sys_sd_fread(res_ptr->res_fp, display_buffer_tmp, pic_data_len);
        
        //����ͼƬ���ӵ�����ͼƬ
        pic_overlay(display_buffer, display_buffer_tmp, &back_region, &pic_region, p_trs_pic);
    }

    //��ʾ����ok����ͼƬ
    put_image(&back_region);
}

//����ͼ�갴�������û�Ϊ��������
void turn_pixel(uint8 *pixel_buffer, region_t *turn_region)
{
    uint16 *pixel_16 = (uint16 *)pixel_buffer;
    uint16 *pixel_tmp = (uint16 *)(pixel_buffer + 512);//��С�޶���512�ֽڣ������ͼ��֧��Ϊ16*16��32*8��
    uint16 x, y;
    
    for(x = 0; x < turn_region->width; x++)
    {
        for(y = 0; y < turn_region->height; y++)
        {
            pixel_tmp[y*turn_region->width + x] = pixel_16[x*turn_region->height + y];
        }
    }
    
    libc_memcpy(pixel_16, pixel_tmp, 512);
}

//��͸��ͼ����ӵ�����������
void pic_overlay(uint8 *pixel_result, uint8 *pic_source, region_t *b_region, region_t *p_region, \
transparent_pic_t *p_trs_pic)
{
    uint16 x, y;
    uint16 *pixel_result_16 = (uint16 *)pixel_result;
    uint16 *pic_source_16 = (uint16 *)pic_source;
    
    if(p_trs_pic->direct == 0)//�п��ܻ�ƫ��
    {
        uint16 rel_y = p_region->y - b_region->y;
        
        for(y = 0; y < p_region->height; y++)
        {
            for(x = 0; x < p_region->width; x++)
            {
                if(pic_source_16[y * p_region->width + x] != p_trs_pic->back_color)
                {
                    pixel_result_16[(rel_y + y) *b_region->width + x] = pic_source_16[y * p_region->width + x];
                }
            }
        }
    }
    else//�п��ܻ�ƫ��
    {
        uint16 rel_x = p_region->x - b_region->x;
        
        for(y = 0; y < p_region->height; y++)
        {
            for(x = 0; x < p_region->width; x++)
            {
                if(pic_source_16[y * p_region->width + x] != p_trs_pic->back_color)
                {
                    pixel_result_16[y *b_region->width + (rel_x + x)] = pic_source_16[y * p_region->width + x];
                }
            }
        }
    }
}

//���ͼƬ���ݵ� lcd ��
bool put_image(region_t *img_region)
{
    uint32 count;
    bool result = TRUE;

    //����ͼƬ��ʾģʽ������
    set_lcd_param_hook(img_region, LCD_PARAM_IMAGE);

    count = ((uint32) img_region->width) * ((uint32) img_region->height) * 2;
    //DMA��GRAM��
    if (dma_buffer_data_hook(count / 2, img_region->width) == FALSE)
    {
        result = FALSE;
    }

    return result;
}

/*! \endcond */
