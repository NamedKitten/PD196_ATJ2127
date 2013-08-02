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
 * \file     _rom_ui_string.c
 * \brief    ROM����ģ��--�ַ�����ʾ���֣�С������
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2012-1-12
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond */
//˵����
//1��������ص�����ͨ�� DMA ���䣬��ô��Ҫ���ֽ���ǰ�����ֽ��ں�
//2��������ص�����ͨ�� EXTMEM ���䣬��ô��Ҫ���ֽ���ǰ�����ֽ��ں�

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
void romf_black_char_to_color(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, uint8 start_col)
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

    //��ʼ��������
    font_width = psoure_buf[32];
    temp_width = 1;
    if (font_width > 8)
    {
        temp_width = 2;
    }
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

    for (i = 0, k = (uint8)buf_region->y; i < UNICODE_HEIGHT_DEF; i++, k++)//loop for height
    {
        display_buffer = display_buffer_base + (k * buf_region->width + buf_region->x);
        for (j = real_start; j <= real_end; j++)//loop for char width
        {
            if (j > 7)
            {
                src_data = (uint8)(psoure_buf[i * temp_width + 1] & (0x80 >> (j - 8)));
            }
            else
            {
                src_data = (uint8)(psoure_buf[i * temp_width] & (0x80 >> j));
            }

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
void romf_char_fill(uint8 *psoure_buf, char_disp_e char_disp, uint8 start_col)
{
    region_t point_region;//��㴰��
    uint16 temp_data;
    uint16 temp_width;
    uint8 trans_font[2];
    uint8 i, j;
    uint8 src_data;//0��ʾ�հ׵㣬1��ʾ�ֵ�
    uint8 font_width;
    uint8 real_start, real_end;

    //��ʼ��������
    font_width = psoure_buf[32];
    temp_width = 1;
    if (font_width > 8)
    {
        temp_width = 2;
    }
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

    for (i = 0; i < UNICODE_HEIGHT_DEF; i++)//loop for height
    {
        for (j = real_start; j <= real_end; j++)//loop for width
        {
            if (j > 7)
            {
                src_data = (uint8) (psoure_buf[i * temp_width + 1] & (0x80 >> (j - 8)));
            }
            else
            {
                src_data = (uint8) (psoure_buf[i * temp_width] & (0x80 >> j));
            }

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
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ�����죩��ǰ�ַ����α�������
 * \param[in]    display_buffer���ַ�����ʾ������
 * \param[in]    bgpic_region����ǰ�ַ�������ʾ���ڣ�Ҳ���ַ�����������
 * \param[in]    mode���ַ����ģʽ
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void romf_read_str_bgpic(uint8 *display_buffer, region_t *bgpic_region, char_fill_mode_e mode)
{
    uint16 *display_buffer_uint16;
    uint16 backgd_color = (uint16) (dc.backgd_color & 0xffff);
    uint16 row, col;

    //ʹ�ñ�����ɫ��䣨���߼����룬�ɹ̻���
    if (mode == CHAR_FILL_COLOR)
    {
        for (row = 0; row < bgpic_region->height; row++)
        {
            display_buffer_uint16 = (uint16 *) display_buffer + (row * bgpic_region->width);
            for (col = 0; col < bgpic_region->width; col++)
            {
                *(display_buffer_uint16 + col) = backgd_color;
            }
        }
    }

    //��ȡ�����죩�������� hook ����
    read_str_bgpic_hook(display_buffer, bgpic_region, mode);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��ǰλ���ַ�����--Unicode ����
 * \param[in]    str����ǰ�ַ�����ʼλ��
 * \param[in]    length����ǰ��Ч�ֽ���
 * \param[in]    language���ַ���������������
 * \param[out]   byte_cnt�����ص�ǰ��Ч�ַ����ֽ���
 * \return       char_type_e�������ַ�����
 * \note         ������ַ���һ�����ḻ�ĸ������Ϊ������ʾ���ַ���Ԫ��
 *******************************************************************************/
char_type_e romf_get_next_char_type_unicode(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)
{
    uint16 uni_code, next_code;

    if (length < 2)
    {
        *byte_cnt = 0;
        return CHAR_NOPEND;
    }
    else
    {
        uni_code = *str + ((uint16) (*(str + 1)) << 8);
        if (uni_code == 0x00)
        {
            *byte_cnt = 2;
            return CHAR_EOF;
        }
        else if (uni_code == 0xfeff)
        {
            *byte_cnt = 2;
            return CHAR_UNI_FLAG;
        }
        else if (uni_code == 0x0a)
        {
            *byte_cnt = 2;
            return CHAR_LR;//linux
        }
        else
        {
            //do nothing for QAC
        }

        if (length >= 4)
        {
            next_code = *(str + 2) + ((uint16) (*(str + 3)) << 8);
            if ((uni_code == 0x0d) && (next_code == 0x0a))
            {
                *byte_cnt = 4;
                return CHAR_LR;//windows
            }
            else if (uni_code == 0x0d)
            {
                *byte_cnt = 2;
                return CHAR_LR;//mac
            }
            else
            {
                //do nothing for QAC
            }
        }
        else
        {
            if (uni_code == 0x0d)
            {
                *byte_cnt = 0;
                return CHAR_NOPEND;
            }
        }
    }

    return next_char_unicode_hook(str, length, language, byte_cnt);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��ǰλ���ַ�����--�������
 * \param[in]    str����ǰ�ַ�����ʼλ��
 * \param[in]    length����ǰ��Ч�ֽ���
 * \param[in]    language���ַ���������������
 * \param[out]   byte_cnt�����ص�ǰ��Ч�ַ����ֽ���
 * \return       char_type_e�������ַ�����
 * \note         ������ַ���һ�����ḻ�ĸ������Ϊ������ʾ���ַ���Ԫ��
 *******************************************************************************/
char_type_e romf_get_next_char_type_ansi(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)
{
    if (*str == 0x00)
    {
        *byte_cnt = 0;
        return CHAR_EOF;
    }
    else if (*str == 0x0a)
    {
        *byte_cnt = 1;
        return CHAR_LR;//linux
    }
    else
    {
        //do nothing for QAC
    }
    
    if (length >= 2)
    {
        if ((*str == 0x0d) && (*(str + 1) == 0x0a))
        {
            *byte_cnt = 2;
            return CHAR_LR;//windows
        }
        else if (*str == 0x0d)
        {
            *byte_cnt = 1;
            return CHAR_LR;//mac
        }
        else
        {
            //do nothing for QAC
        }
    }
    else
    {
        if (*str == 0x0d)
        {
            *byte_cnt = 0;
            return CHAR_NOPEND;
        }
    }

    return next_char_ansi_hook(str, length, language, byte_cnt);
}

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
void romf_get_next_char_point(uint16 uni_code, uint8 *pbuffer)
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
            libc_memcpy(pbuffer, font_cache_char_addr(cache_index), 33);
        }
        //û�ҵ�������ģ�����ֿ��ļ���ȡ����䵽��ģ������
        else
        {
            romf_get_font_point(uni_code, pbuffer);
            libc_memcpy(font_cache_char_addr(cache_index), pbuffer, 33);
        }
    }
    //û����ģ��������ÿ�ζ���Ҫֱ�Ӷ�ȡ��ģ
    else
    {
        romf_get_font_point(uni_code, pbuffer);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �������ַ�ת����unicode����
 * \param[in]    code_rename���ַ���ֵ
 * \param[out]   none
 * \return       ��ǰ�ַ���unicode����ֵ
 * \note         ֻ��Ҫ�Դ��ڻ����0x80������ת����С��0x80Ҳ����ת�����˷Ѷ�flash��
 *******************************************************************************/
uint16 romf_multi_char_to_unicode(uint16 code_rename)
{
    uint16 unicode;

    /*��ʼ��������Ե�Unicode��ת����ľ��*/
    sys_sd_fseek(MBtoUniTab_fp, SEEK_SET, (uint32) (code_rename << 1));
    sys_sd_fread(MBtoUniTab_fp, &unicode, 2);

    return unicode;
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
void romf_get_font_point(uint16 code_rename, uint8 *picbuf)
{
    sys_sd_fseek(FontLib_fp, SEEK_SET, (uint32) (code_rename * FONT_ONE_SIZE));
    sys_sd_fread(FontLib_fp, picbuf, FONT_ONE_SIZE);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾ�����ַ���֧�ֲ������ַ���ʾ
 * \param[in]    psoure_buf���ַ���ģ��������33 ���ֽڣ�16*16��buf[32]��ʾ��ȣ�
 * \param[in]    mode���ַ����ģʽ
 * \param[in]    char_disp���ַ�����ģ����ʾģʽ
 * \param[in]    start_col���ַ�����ģ����ʾ��ʼλ��
 * \param[out]   none
 * \return       none
 * \note         1��start_col ���ܴ��ڵ����ַ���ȡ�
 *               2������������� char_region ������ ȫ�� region �С�
 *******************************************************************************/
bool romf_put_char(uint8 *psoure_buf, char_fill_mode_e mode, char_disp_e char_disp, uint8 start_col)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    region_t buf_region;

    //ֱ�Ӵ��ģʽ����LCD�޷�����ʱʹ��
    if (mode == CHAR_FILL_POINT)
    {
        char_fill(psoure_buf, char_disp, start_col);
    }
    //�Ȱ��ַ����嵽��ʾ�������У���DMA��ʾ����
    else
    {
        //���㻺����Ԥ������
        buf_region.x = 0;
        buf_region.y = 0;
        buf_region.height = UNICODE_HEIGHT_DEF;
        buf_region.width = region.width;

        //��ȡ�ַ�������ͼƬ�����贰
        read_str_bgpic(display_buffer, &region, mode);

        black_char_to_color(psoure_buf, &buf_region, char_disp, start_col);
        //�����ַ�����ʾ����
        set_lcd_param_hook(&region, LCD_PARAM_STRING);
        //���ͻ��������ݵ�DMA��
        if (dma_buffer_data_hook(buf_region.width * buf_region.height, buf_region.width) == FALSE)
        {
            return FALSE;//dma����ʧ��
        }
    }

    return TRUE;
}

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
uint8 *romf_put_string_line(string_desc_t *desc, region_t *str_region, char_fill_mode_e mode)
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

    //ֱ�Ӵ��ģʽ
    if (mode == CHAR_FILL_POINT)
    {
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
            char_bytes = get_next_char_point_hook(str, language, char_point_buffer);
            fontwidth = char_point_buffer[32];

            //�����ʾ���򣨿ռ䣩�Ƿ��㹻��ʾ
            if ((fontwidth > region.width)//��ǰ�ַ���ʾ���� region ����
             && ((region.x + fontwidth) > (region_win.x + region_win.width)))//��ǰ�ַ���ʾ��������X��
            {
                desc->result = STRING_TAIL_REGION_ERR;
                break;
            }//2012-10-30 17:04 �˴���bug��������ʾԽ�磬Ӧ�� && ��Ϊ ||���޸�Ϊ fixf_put_string_line

            //ֱ�Ӵ��ģʽ����LCD�޷�����ʱʹ��
            char_fill(char_point_buffer, CHAR_DISP_NORMAL, 0);

            //ָ�����
            str += char_bytes;
            length -= char_bytes;
        }
    }
    //����������ʾģʽ
    //���ݻ�������С����ʾģʽ���ַ������ܻ�ֶ����ʾ
    //ÿ����ʾ���Ȼ��嵽��ʾbuffer����һ����DMA��GRAM�У�������ʾЧ��
    else
    {
        while (1)
        {
            //���ַ�����������������
            //���㻺����Ԥ������
            buf_region.x = 0;
            buf_region.y = 0;
            buf_region.height = UNICODE_HEIGHT_DEF;
            buf_region.width = ((display_buf_len / 2) / UNICODE_HEIGHT_DEF);
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
                char_bytes = get_next_char_point_hook(str, language, char_point_buffer);
                fontwidth = char_point_buffer[32];

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
                black_char_to_color(char_point_buffer, &buf_region, CHAR_DISP_NORMAL, 0);
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
    }

    return str;
}
/*! \endcond */

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ�ַ�������
 * \param[in]    desc���ַ���������
 * \param[in]    max_length���ַ�����󳤶ȣ���ɨ�赽�ַ������ȴ��ڸó�������ֹ����
 * \param[out]   result���������ָ�룬����Ϊ��
 * \return       �ַ������ȣ����ص㣩
 * \ingroup      string
 * \note         1��������������д����������
 *               2��desc->result ���ؼ����ַ���������ֹԭ��
 *******************************************************************************/
uint16 romf_get_string_length(string_desc_t *desc, uint16 max_length, str_getlen_result_t *result)
{
    char_type_e (*get_next_char_type)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
    uint8 *str;
    uint16 length;
    int8 language;
    uint8 char_bytes;
    uint16 str_width = 0;//�ַ������ȣ����ص㣩
    char_type_e char_type;

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
    if (result != NULL)
    {
        result->true_length = 0;
        result->true_bytes = 0;
    }

    //�����ַ�����ȡ�ַ�������
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
            if (result != NULL)
            {
                result->true_bytes += 2;
            }
            continue;
        }
        else if (char_type == CHAR_LR)//���з�
        {
            desc->result = STRING_TAIL_CHAR_LR;
            break;
        }
        else
        {
            //do nothing for QAC
        }

        //����nor �����ü��ֿ�������ֿ�
        char_bytes = get_next_char_point_hook(str, language, char_point_buffer);

        //�ַ�������ۼ�
        str_width += char_point_buffer[32];
        if (str_width > max_length)//�Ѿ�������󳤶ȣ������ټ���
        {
            desc->result = STRING_TAIL_REGION_ERR;
            break;
        }

        //ָ�����
        str += char_bytes;
        length -= char_bytes;

        if (result != NULL)
        {
            result->true_length += char_point_buffer[32];
            result->true_bytes += char_bytes;
        }
    }

    return str_width;
}
/*! \endcond */
