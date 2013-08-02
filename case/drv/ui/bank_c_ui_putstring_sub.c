/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       cailizhen     2012-10-16 9:27     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_putstring_sub.c
 * \brief    �ַ�������׶� bank c �ӿ�
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *           
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-10-16
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond */

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾ�����ַ���֧�ֲ������ַ���ʾ
 * \param[in]    psoure_buf���ַ���ģ��������33 ���ֽڣ�16*16��buf[32]��ʾ��ȣ�
 * \param[in]    mode���ַ����ģʽ
 * \param[in]    char_disp���ַ�����ģ����ʾģʽ
 * \param[in]    start_col���ַ�����ģ����ʾ��ʼλ��
 * \param[in]    mode��ROW_PRIORITY_DEF ��ʾ�����ȣ�COL_PRIORITY_DEF ��ʾ������
 * \param[out]   none
 * \return       none
 * \note         1��start_col ���ܴ��ڵ����ַ���ȡ�
 *               2������������� char_region ������ ȫ�� region �С�
 *******************************************************************************/
void put_char_sd(uint8 *psoure_buf, region_t *char_region, char_disp_e char_disp, uint8 start_col, uint8 mode)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    region_t buf_region;
    uint8 start_row, end_row;
    
    if(mode == ROW_PRIORITY_DEF)
    {
        start_row = (uint8)(char_region->y - text_bg_desc.back_y);
        end_row = (uint8)(start_row + char_region->height);
        //���㻺����Ԥ������
        buf_region.x = char_region->x - region.x;
        buf_region.y = fix_font_height_actual - end_row;
        buf_region.height = char_region->height;
        buf_region.width = char_region->width;
        
        black_char_to_color_sd(psoure_buf, &buf_region, char_disp, start_col, start_row, end_row, mode);
    }
    else
    {
        //���㻺����Ԥ������
        buf_region.x = char_region->x - text_bg_desc.back_x;
        buf_region.y = char_region->y - text_bg_desc.back_y;
        buf_region.height = text_bg_desc.height;
        buf_region.width = char_region->width;
        
        region.x = char_region->x;
        region.y = text_bg_desc.back_y;
        region.width = char_region->width;
        region.height = text_bg_desc.height;
        
        //��ȡ�ַ�������ͼƬ�����贰
        read_str_bgpic(display_buffer, &buf_region, CHAR_FILL_SDIMAGE);
        buf_region.x = 0;//ָ�����У���ʼ�����ַ���
        
        black_char_to_color_sd(psoure_buf, &buf_region, char_disp, start_col, 0, fix_font_height_actual, mode);
        
        //�����ַ�����ʾ����
        set_lcd_param_hook(&region, LCD_PARAM_SCROLL);
        //���ͻ��������ݵ�DMA��
        if (dma_buffer_data_hook(buf_region.width * buf_region.height, buf_region.width) == FALSE)
        {
            return FALSE;//dma����ʧ��
        }
    }
}


/******************************************************************************/
/*!
 * \par  Description:
 *    �ַ��Ӻڰ�ת��Ϊ��ɫ�����浽��������
 * \param[in]    psoure_buf���ַ���ģ��������33 ���ֽڣ�16*16��buf[32]��ʾ��ȣ�
 * \param[in]    buf_region����ʾ������Ԥ�����򣨸߶�С��height_max��
 *               ��������������Ϊ���Ϊwidth���߶�Ϊheight_max + buffer_len / width������
 * \param[in]    char_disp���ַ�����ģ����ʾģʽ
 * \param[in]    start_col���ַ�����ģ����ʾ��ʼλ��
 * \param[in]    start_row���ַ���ʼ�У����ڴ�ֱ����
 * \param[in]    end_row���ַ������У����ڴ�ֱ����
 * \param[in]    mode��ROW_PRIORITY_DEF ��ʾ�����ȣ�COL_PRIORITY_DEF ��ʾ������
 * \param[out]   none
 * \return       none
 * \note         start_col ���ܴ��ڵ����ַ����
 *******************************************************************************/
void black_char_to_color_sd(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, \
        uint8 start_col, uint8 start_row, uint8 end_row, uint8 mode)
{
#ifndef PC
    uint16 *display_buffer_base = (uint16 *) display_buf_addr;
#else
    uint16 *display_buffer_base = GET_REAL_ADDR(display_buf_addr);
#endif
    uint16 *display_buffer;//������Ϊ16bit������565С����
    uint16 pen_color = (uint16) (dc.pen_color & 0xffff);
    uint8 i, j, k;
    uint8 src_data;//0��ʾ�հ׵㣬1��ʾ�ֵ�
    uint8 temp_width;//1��ʾ16*8��ģ��2��ʾ16��16��ģ
    uint8 font_width;
    uint8 real_start, real_end;
    uint8 shift_mask;
    uint8 index;

    //��ʼ��������
    font_width = psoure_buf[fix_font_size - 1];
    temp_width = ((font_width + 7) / 8);
    
    //ȷ����������
    if (char_disp == CHAR_DISP_NORMAL)
    {
        real_start = 0;
        real_end = font_width - 1;
    }
    else if (char_disp == CHAR_DISP_LEFT2RIGHT)
    {
        real_start = start_col;
        real_end = start_col + (uint8)(region.width - 1);
    }
    else//CHAR_DISP_RIGHT2LEFT
    {
        real_start = start_col - (uint8)(region.width - 1);
        real_end = start_col;
    }
    
    if(mode == ROW_PRIORITY_DEF)
    {
        for (i = start_row, k = (uint8)buf_region->y; i < end_row; i++, k++)//loop for height
        {
            display_buffer = display_buffer_base + (k * buf_region->width + buf_region->x);
            index = ((i + fix_font_height_up_margin) * temp_width) + (real_start/8);
            shift_mask = (uint8)(0x80 >> (real_start % 8));
            for (j = real_start; j <= real_end; j++)//loop for char width
            {
                if ((j % 8) == 0)
                {
                    index = ((i + fix_font_height_up_margin) * temp_width) + (j/8);
                    shift_mask = 0x80;
                }

                src_data = (uint8)(psoure_buf[index] & (shift_mask));
                shift_mask >>= 1;
    
                //�հ׵㴦��
                if (src_data == 0)
                {
                    //��������ת��
                    display_buffer++;
                }
                //�ֵ㴦��
                else
                {
                    //��������ɫ����
                    *display_buffer = pen_color;
                    display_buffer++;
                }
            }
        }
    }
    else
    {
        for (j = real_start, k = (uint8)buf_region->x; j <= real_end; j++, k++)//loop for char width
        {
            display_buffer = display_buffer_base + (k * buf_region->height + buf_region->y);
            shift_mask = (uint8)(0x80 >> (j%8));	
            index = ((start_row + fix_font_height_up_margin) * temp_width) + (j/8);
            for (i = start_row; i < end_row; i++)//loop for height
            {
                src_data = (uint8)(psoure_buf[index] & shift_mask);
                index += temp_width;
    
                //�հ׵㴦��
                if (src_data == 0)
                {
                    //��������ת��
                    display_buffer++;
                }
                //�ֵ㴦��
                else
                {
                    //��������ɫ����
                    *display_buffer = pen_color;
                    display_buffer++;
                }
            }
        }
    }
}

#if (USE_MULTI_FONTLIB != 0)
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡָ��Unicode �����ַ���ģ
 * \param[in]    uni_code��Unicode �ַ�����
 * \param[in]    pbuffer���ַ���ģ������
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void fixf_get_next_char_point(uint16 uni_code, uint8 *pbuffer)
{
    uint8 cache_index;

    //��ȡ�ַ�����
    //����ģ�����������û�����Ƽ��ٶ�ȡ��ģ����
    //�̻����� for QAC��if(font_cache_flag) �ȼ��� if(font_cache_flag != FALSE) �����ȼ��� if(font_cache_flag == TRUE)
    if (font_cache_flag != FALSE)
    {
        //�ҵ�������ģ��ֱ�Ӷ�ȡ
        if (search_cache(uni_code, &cache_index) == TRUE)
        {
            libc_memcpy(pbuffer, fix_font_cache_char_addr(cache_index), fix_font_size);
        }
        //û�ҵ�������ģ�����ֿ��ļ���ȡ����䵽��ģ������
        else
        {
            fixf_get_font_point(uni_code, pbuffer);
            libc_memcpy(fix_font_cache_char_addr(cache_index), pbuffer, fix_font_size);
        }
    }
    //û����ģ��������ÿ�ζ���Ҫֱ�Ӷ�ȡ��ģ
    else
    {
        fixf_get_font_point(uni_code, pbuffer);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���ֿ��ļ��ж�ȡ�ַ��ĵ���
 * \param[in]    code_rename���ַ���ֵ
 * \param[out]   picbuf�������ַ������buffer��ַ
 * \return       none
 * \note
 *******************************************************************************/
void fixf_get_font_point(uint16 code_rename, uint8 *picbuf)
{
    sys_sd_fseek(FontLib_fp, SEEK_SET, (uint32) (code_rename * fix_font_size));
    sys_sd_fread(FontLib_fp, picbuf, fix_font_size);
}
#endif

/*! \endcond */
