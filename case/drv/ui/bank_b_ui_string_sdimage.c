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
 * \file     ui_string_sdimage.c
 * \brief    ʵ���ַ�����ʾ
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2012-02-12
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾ�����ַ���
 * \param[in]    desc���ַ���������
 * \param[in]    str_region�������ַ�����ʾ����
 * \param[in]    mode��ROW_PRIORITY_DEF ��ʾ�����ȣ�COL_PRIORITY_DEF ��ʾ������
 * \param[out]   none
 * \return       �����ַ�����ʾ����ָ��
 * \note         1��Ҫ�󴫽������ַ����������������������ʾ�ı��롣
 *               2����ʾ����������������ʾ�����ַ������ַ���������
 *               3��str_region->height����ΪUNICODE_HEIGHT_DEF��
 *               4��desc->result ���ص����ַ�����ʾ��ֹԭ��
 *******************************************************************************/
uint8 *put_string_line_sdimage(string_desc_t *desc, region_t *str_region, uint8 mode)
{
    char_type_e (*get_next_char_type)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint8 *str;
    uint16 length;
    int8 language;
    uint8 char_bytes;
    region_t buf_region;
    uint16 this_width;//����ʵ�ʻ������ַ������
    uint16 total_width;//�ַ�������ʾ���򳤶�
    char_type_e char_type;
    uint8 fontwidth;
    bool draw_over = FALSE;
    bool remain_region_flag = TRUE;
    bool scan_over = FALSE;
    uint8 start_row, end_row;
    uint16 buf_region1_width, buf_region1_height;

    //����������ʼ��
    total_width = 0;
    str = desc->data.str;
    length = desc->length;
    language = desc->language;
    if (language == UNICODELANGUAGE)
    {
        get_next_char_type = get_next_char_type_unicode;
    }
    else
    {
        get_next_char_type = get_next_char_type_ansi;
    }

    //���ݻ�������С����ʾģʽ���ַ������ܻ�ֶ����ʾ
    //ÿ����ʾ���Ȼ��嵽��ʾbuffer����һ����DMA��GRAM�У�������ʾЧ��
    while (1)
    {
        if(mode == ROW_PRIORITY_DEF)//�����ȣ�һ�λ��崦�������
        {
            start_row = (uint8)(str_region->y - text_bg_desc.back_y);
            end_row = (uint8)(start_row + str_region->height);
            //���ַ�����������������
            //���㻺����Ԥ������
            buf_region.x = str_region->x - text_bg_desc.back_x;
            buf_region.y = fix_font_height_actual - end_row;
            buf_region.height = str_region->height;
            buf_region.width = str_region->width;
            remain_region_flag = FALSE;
            
            //���õ�ǰ��������
            libc_memcpy(&region, str_region, sizeof(region_t));
        }
        else//������
        {
            buf_region1_height = text_bg_desc.height;
            if(buf_region1_height > 0)//for QAC
            {
                if(display_buf_len > (5*1024))
                {
                    display_buf_len = 5*1024;//buffer ���Ϊ5k
                }
                buf_region1_width = ((display_buf_len / 2) / buf_region1_height);
            }
            else
            {
                return NULL;
            }
            if (buf_region1_width > (str_region->width - total_width))//���������պ��������ǣ�ʣ�ࣩregion
            {
                buf_region1_width = (str_region->width - total_width);
                remain_region_flag = FALSE;
            }
    
            //���ַ�����������������
            //���㻺����Ԥ������
            buf_region.x = str_region->x - text_bg_desc.back_x;
            buf_region.y = str_region->y - text_bg_desc.back_y;
            buf_region.height = text_bg_desc.height;
            buf_region.width = buf_region1_width;
    
            //��ȡ�ַ�������ͼƬ�����贰
            region.x = str_region->x + total_width;
            region.y = text_bg_desc.back_y;
            region.width = buf_region1_width;
            region.height = buf_region1_height;
            read_str_bgpic(display_buffer, &buf_region, CHAR_FILL_SDIMAGE);
            buf_region.x = 0;//ָ�����У���ʼ�����ַ���
        }

        //����һ�β����ַ���
        this_width = 0;//�ۼ�ÿ��ʵ�ʻ��������
        while (1)
        {
            scan_string_other:
            //�ַ��������Ѿ����
            if (length == 0)
            {
                draw_over = TRUE;
                desc->result = STRING_TAIL_LEN_ZERO;
                break;
            }
            //��ȡ��һ���ַ�����Ϊ�����������з�����ͨ�ַ�
            char_type = get_next_char_type(str, length, language, &char_bytes);
            switch(char_type)
            {
            case CHAR_EOF:
            case CHAR_NOPEND://���������߲���������ʾ
                draw_over = TRUE;
                desc->result = STRING_TAIL_EOF_NOPEND;
                scan_over = TRUE;
                break;

            case CHAR_UNI_FLAG://Unicode ��־�� С��Unicode
                str += 2;
                length -= 2;
                goto scan_string_other;

            case CHAR_LR://���з�
                str += char_bytes;
                draw_over = TRUE;
                desc->result = STRING_TAIL_CHAR_LR;
                scan_over = TRUE;
                break;

            default:
                //do nothing for QAC
                break;
            }
            
            if(scan_over == TRUE)
            {
                break;//�˳��ַ���ɨ��
            }

            //����nor �����ü��ֿ�������ֿ�
            char_bytes = get_next_char_point_hook(str, language, fix_p_char_point);
            fontwidth = fix_p_char_point[fix_font_size - 1];

            //�����ʾ���򣨿ռ䣩�Ƿ��㹻��ʾ
            if ((region.x + this_width + fontwidth) > (region_win.x + region_win.width))//��������X��
            {
                draw_over = TRUE;
                desc->result = STRING_TAIL_REGION_ERR;
                break;
            }

            //�ټ�黺�����Ƿ񹻴�ţ�ͬʱҲ����˵�ǰ region �����Ƿ񹻴�ţ�
            if ((this_width + fontwidth) > buf_region.width)
            {
                //���û��ʣ�� region�����ɽ�����ʾ����
                if (remain_region_flag == FALSE)
                {
                    draw_over = TRUE;
                    desc->result = STRING_TAIL_REGION_ERR;
                }
                break;
            }

            //�Ȱ��ַ����嵽��ʾ��������
            if(mode == ROW_PRIORITY_DEF)//������
            {
                black_char_to_color_sd(fix_p_char_point, &buf_region, CHAR_DISP_NORMAL, 0, start_row, end_row, 0);
            }
            else//������
            {
                black_char_to_color_sd(fix_p_char_point, &buf_region, CHAR_DISP_NORMAL,0,0,fix_font_height_actual,1);
            }
            //�ַ�����ۼ�
            this_width += fontwidth;
            //�����Ƶ���һ���ֵ����Ͻ�
            buf_region.x += fontwidth;

            //ָ�����
            str += char_bytes;
            length -= char_bytes;
        }

        
        if (this_width > 0)
        {
            //ֻ���������ص���ʾʱ����Ҫ�������������ʾ
            if (mode == COL_PRIORITY_DEF)//��������Ҫ��ʾ����������������ʾ
            {
                //��ʾ�����ַ���
                //�����ַ�����ʾ����
                set_lcd_param_hook(&region, LCD_PARAM_SCROLL);
    
                //���ͻ��������ݵ�DMA��
                if (dma_buffer_data_hook(buf_region.width * buf_region.height, buf_region.width) == FALSE)
                {
                    desc->result = STRING_TAIL_OTHER_ERR;
                    break;//dma����ʧ��
                }
            }

            //�ۼ� total_width
            total_width += this_width;
        }

        //�ַ�����ʾ����
        if (draw_over == TRUE)
        {
            if(mode == ROW_PRIORITY_DEF)
            {
                str_region->x += total_width;
                
                //�����ȣ������������Ѿ���ȡ���ˣ����������ﴦ�������ַ�
                if(desc->result == STRING_TAIL_REGION_ERR)
                {
                    region.width = str_region->width - total_width;
                    if(region.width > 0)//��ʣ�಻�����ַ�������Ҫ����
                    {
                        char_out_rect(str, language, str_region, CHAR_FILL_SDIMAGE, 0, 0);
                    }
                }
            }
            else
            {
                //���� regionΪʣ����ʾ����
                region.x = str_region->x + total_width;
                region.y = text_bg_desc.back_y;
                region.width = str_region->width - total_width;
            }
            break;
        }
    }

    return str;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��������߽�������ص�
 * \param[in]    remain_region�������߽�������ص�����
 * \param[out]   none
 * \return       none
 * \note         
 *******************************************************************************/
void clear_remain_pixel_sdimage(region_t *remain_region)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    region_t buf_region;

    //���ַ�����������������
    //���㻺����Ԥ������
    buf_region.x = remain_region->x - text_bg_desc.back_x;
    buf_region.y = remain_region->y - text_bg_desc.back_y;
    buf_region.height = text_bg_desc.height;
    buf_region.width = remain_region->width;

    //��ȡ�ַ�������ͼƬ�����贰
    libc_memcpy(&region, remain_region, sizeof(region_t));
    region.height = text_bg_desc.height;
    read_str_bgpic(display_buffer, &buf_region, CHAR_FILL_SDIMAGE);

    //�����ַ�����ʾ����
    set_lcd_param_hook(&region, LCD_PARAM_SCROLL);
    //���ͻ��������ݵ�DMA��
    dma_buffer_data_hook(buf_region.width * buf_region.height, buf_region.width);
}

/*! \endcond */
