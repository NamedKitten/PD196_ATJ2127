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
 * \file     fix_ui_string.c
 * \brief    �̻������޸�
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date     2012-1-12
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond */
//˵����
//1��������ص�����ͨ�� DMA ���䣬��ô��Ҫ���ֽ���ǰ�����ֽ��ں�
//2��������ص�����ͨ�� EXTMEM ���䣬��ô��Ҫ���ֽ���ǰ�����ֽ��ں�

#if (USE_MULTI_FONTLIB != 0)
/******************************************************************************/
/*!
 * \par  Description:
 *    �ַ��Ӻڰ�ת��Ϊ��ɫ�����浽��������
 * \param[in]    psoure_buf���ַ���ģ��������33 ���ֽڣ�16*16��buf[32]��ʾ��ȣ�
 * \param[in]    buf_region����ʾ������Ԥ�����򣨸߶�С��height_max��
 *               ��������������Ϊ���Ϊwidth���߶�Ϊheight_max + buffer_len / width������
 * \param[in]    char_disp���ַ�����ģ����ʾģʽ
 * \param[in]    start_col���ַ�����ģ����ʾ��ʼλ��
 * \param[out]   none
 * \return       none
 * \note         start_col ���ܴ��ڵ����ַ����
 *******************************************************************************/
void fixf_black_char_to_color(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, uint8 start_col)
{
#ifdef SUPPORT_READ_CMD//֧�ֶ���
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
    temp_width = (font_width + 7)/8;
    
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

    for (i = 0, k = (uint8)buf_region->y; i < buf_region->height; i++, k++)//loop for height
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
            
            src_data = (uint8)(psoure_buf[index] & shift_mask);
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
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �����ַ��ĵ��������Լ��Ƿ�͸��������Ļ�ϻ������ַ���Ҫ��ʾ�Ĳ��֡�֧�ֲ������ַ���ʾ
 * \param[in]   psoure_buf���ַ���ģ������
 * \param[in]   char_disp���ַ�����ģ����ʾģʽ
 * \param[in]   start_col���ַ�����ģ����ʾ��ʼλ��
 * \param[out]  none
 * \return      none
 * \note        start_col ���ܴ��ڵ����ַ����
 *******************************************************************************/
void fixf_char_fill(uint8 *psoure_buf, char_disp_e char_disp, uint8 start_col)
{
#ifndef SUPPORT_READ_CMD//��֧�ֶ���
    region_t point_region;//��㴰��
    uint16 temp_data;
    uint16 temp_width;
    uint8 trans_font[2];
    uint8 i, j;
    uint8 src_data;//0��ʾ�հ׵㣬1��ʾ�ֵ�
    uint8 font_width;
    uint8 real_start, real_end;
    uint8 shift_mask;
    uint8 index;

    //��ʼ��������
    font_width = psoure_buf[fix_font_size - 1];
    temp_width = (font_width + 7)/8;

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

    //��ʼ�ֵ���ɫ
    temp_data = (uint16) dc.pen_color;
#ifdef PC
    trans_font[0] = (uint8) temp_data;//���ֽ�
    trans_font[1] = (uint8) ((temp_data & 0xff00) >> 8);//���ֽ�
#else
    trans_font[0] = (uint8) ((temp_data & 0xff00) >> 8);//ͨ��EXTMEM���䣬���ֽ��ȷ���
    trans_font[1] = (uint8) temp_data;//���ֽں���
#endif

    //��ʼ��㴰��
    point_region.x = region.x;
    point_region.y = region.y;
    point_region.width = 1;
    point_region.height = 1;
    
    for (i = 0; i < fix_font_height_actual; i++)//loop for height
    {
        index = ((i + fix_font_height_up_margin) * temp_width) + (real_start/8);
        shift_mask = (0x80 >> (real_start % 8));
        for (j = real_start; j <= real_end; j++)//loop for width
        {
            if ((j % 8) == 0)
            {
                index = ((i + fix_font_height_up_margin) * temp_width) + (j/8);
                shift_mask = 0x80;
            }
            
            src_data = (uint8)(psoure_buf[index] & shift_mask);
            shift_mask >>= 1;

            //�ֵ㣬ֱ�Ӵ����ʾ
            if (src_data != 0)
            {
                char_fill_hook(&point_region, trans_font);
            }

            point_region.x++;
#ifdef PC
            lcd_set_window(&point_region);
#endif
        }
        point_region.x -= (real_end - real_start + 1);
        point_region.y++;
#ifdef PC
        lcd_set_window(&point_region);
#endif
    }

    //ָ��ָ����һ���ַ����Ͻǣ�ͬʱwidth��Ӧ����
    region.x += (real_end - real_start + 1);
    region.width -= (real_end - real_start + 1);
#endif
}

#ifdef SUPPORT_READ_CMD//֧�ֶ���

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾ�����ַ���
 * \param[in]    desc���ַ���������
 * \param[in]    str_region�������ַ�����ʾ����
 * \param[in]    mode���ַ����ģʽ
 * \param[out]   none
 * \return       �����ַ�����ʾ����ָ��
 * \note         1��Ҫ�󴫽������ַ����������������������ʾ�ı��롣
 *               2����ʾ����������������ʾ�����ַ������ַ���������
 *               3��str_region->height����ΪUNICODE_HEIGHT_DEF��
 *               4��desc->result ���ص����ַ�����ʾ��ֹԭ��
 *******************************************************************************/
uint8 *fixf_put_string_line(string_desc_t *desc, region_t *str_region, char_fill_mode_e mode)
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

    //����������ʾģʽ
    //���ݻ�������С����ʾģʽ���ַ������ܻ�ֶ����ʾ
    //ÿ����ʾ���Ȼ��嵽��ʾbuffer����һ����DMA��GRAM�У�������ʾЧ��
    while (1)
    {
        //���ַ�����������������
        //���㻺����Ԥ������
        buf_region.x = 0;
        buf_region.y = 0;
        buf_region.height = fix_font_height_actual;
        if (fix_font_height_actual != 0)
        {
            buf_region.width = ((display_buf_len / 2) / fix_font_height_actual);
        }
        else
        {
            buf_region.width = DISP_MAX_WIDTH;//for QAC ��0 warning
        }
        if (buf_region.width > DISP_MAX_WIDTH)//���160�����軺����Ϊ16*160*2=5KB
        {
            buf_region.width = DISP_MAX_WIDTH;
        }
        if (buf_region.width > (str_region->width - total_width))//���������պ��������ǣ�ʣ�ࣩregion
        {
            buf_region.width = (str_region->width - total_width);
            remain_region_flag = FALSE;
        }

        //��ȡ�ַ�������ͼƬ�����贰
        region.x = str_region->x + total_width;
        region.y = str_region->y;
        region.width = buf_region.width;
        region.height = buf_region.height;
        read_str_bgpic(display_buffer, &region, mode);

        //����һ�β����ַ���
        this_width = 0;//�ۼ�ÿ��ʵ�ʻ��������
        while (1)
        {
            //�ַ��������Ѿ����
            if (length == 0)
            {
                draw_over = TRUE;
                desc->result = STRING_TAIL_LEN_ZERO;
                break;
            }
            //��ȡ��һ���ַ�����Ϊ�����������з�����ͨ�ַ�
            char_type = get_next_char_type(str, length, language, &char_bytes);
            if ((char_type == CHAR_EOF) || (char_type == CHAR_NOPEND))//���������߲���������ʾ
            {
                draw_over = TRUE;
                desc->result = STRING_TAIL_EOF_NOPEND;
                break;
            }
            else if (char_type == CHAR_UNI_FLAG)
            {
                str += 2;
                length -= 2;
                continue;
            }
            else if (char_type == CHAR_LR)//���з�
            {
                str += char_bytes;
                draw_over = TRUE;
                desc->result = STRING_TAIL_CHAR_LR;
                break;
            }
            else
            {
                //do nothing for QAC
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
            fixf_black_char_to_color(fix_p_char_point, &buf_region, CHAR_DISP_NORMAL, 0);
            //�ַ�����ۼ�
            this_width += fontwidth;
            //�����Ƶ���һ���ֵ����Ͻ�
            buf_region.x += fontwidth;

            //ָ�����
            str += char_bytes;
            length -= char_bytes;
        }

        //ֻ���������ص���ʾʱ����Ҫ�������������ʾ
        if (this_width > 0)
        {
            //��ʾ�����ַ���
            //�����ַ�����ʾ����
            set_lcd_param_hook(&region, LCD_PARAM_STRING);
            //���ͻ��������ݵ�DMA��
            if (dma_buffer_data_hook(buf_region.width * buf_region.height, buf_region.width) == FALSE)
            {
                desc->result = STRING_TAIL_OTHER_ERR;
                break;//dma����ʧ��
            }

            //�ۼ� total_width
            total_width += this_width;
        }

        //�ַ�����ʾ����
        if (draw_over == TRUE)
        {
            //���� regionΪʣ����ʾ����
            region.x = str_region->x + total_width;
            region.y = str_region->y;
            region.width = str_region->width - total_width;
            break;
        }
    }

    return str;
}

#else//��֧�ֶ���

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾ�����ַ���
 * \param[in]    desc���ַ���������
 * \param[in]    str_region�������ַ�����ʾ����
 * \param[in]    mode���ַ����ģʽ
 * \param[out]   none
 * \return       �����ַ�����ʾ����ָ��
 * \note         1��Ҫ�󴫽������ַ����������������������ʾ�ı��롣
 *               2����ʾ����������������ʾ�����ַ������ַ���������
 *               3��str_region->height����ΪUNICODE_HEIGHT_DEF��
 *               4��desc->result ���ص����ַ�����ʾ��ֹԭ��
 *******************************************************************************/
uint8 *fixf_put_string_line(string_desc_t *desc, region_t *str_region, char_fill_mode_e mode)
{
    char_type_e (*get_next_char_type)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
    uint8 *str;
    uint16 length;
    int8 language;
    uint8 char_bytes;
    char_type_e char_type;
    uint8 fontwidth;

    //����������ʼ��
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

    //ֱ�Ӵ��ģʽ
    libc_memcpy(&region, str_region, sizeof(region_t));

    //����ַ����������ַ���
    while (1)
    {
        //�ַ��������Ѿ����
        if (length == 0)
        {
            desc->result = STRING_TAIL_LEN_ZERO;
            break;
        }
        //��ȡ��һ���ַ�����Ϊ�����������з�����ͨ�ַ�
        char_type = get_next_char_type(str, length, language, &char_bytes);
        if ((char_type == CHAR_EOF) || (char_type == CHAR_NOPEND))//���������߲���������ʾ
        {
            desc->result = STRING_TAIL_EOF_NOPEND;
            break;
        }
        else if (char_type == CHAR_UNI_FLAG)
        {
            str += 2;
            length -= 2;
            continue;
        }
        else if (char_type == CHAR_LR)//���з�
        {
            str += char_bytes;
            desc->result = STRING_TAIL_CHAR_LR;
            break;
        }
        else
        {
            //do nothing for QAC
        }

        //����nor �����ü��ֿ�������ֿ�
        char_bytes = get_next_char_point_hook(str, language, fix_p_char_point);
        fontwidth = fix_p_char_point[fix_font_size - 1];

        //�����ʾ���򣨿ռ䣩�Ƿ��㹻��ʾ 
        if ((fontwidth > region.width)//��ǰ�ַ���ʾ���� region ����
         || ((region.x + fontwidth) > (region_win.x + region_win.width)))//��ǰ�ַ���ʾ��������X��
        {
            desc->result = STRING_TAIL_REGION_ERR;
            break;
        }

        //ֱ�Ӵ��ģʽ����LCD�޷�����ʱʹ��
        fixf_char_fill(fix_p_char_point, CHAR_DISP_NORMAL, 0);

        //ָ�����
        str += char_bytes;
        length -= char_bytes;
    }

    return str;
}

#endif//#ifdef SUPPORT_READ_CMD

#endif
/*! \endcond */
