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
 * \file     fix_ui_string_2.c
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

/*! \cond UI_DRIVER_API */

#if (USE_MULTI_FONTLIB != 0)
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
uint16 fixf_get_string_length(string_desc_t *desc, uint16 max_length, str_getlen_result_t *result)
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
        char_bytes = get_next_char_point_hook(str, language, fix_p_char_point);

        //�ַ�������ۼ�
        str_width += fix_p_char_point[fix_font_size - 1];
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
            result->true_length += fix_p_char_point[fix_font_size - 1];
            result->true_bytes += char_bytes;
        }
    }

    return str_width;
}

//�л������ֿ�
void fix_switch_fontlib(uint16 disp_mode)
{
#ifndef PC
    uint32 *font_cache_buffer = font_cache_addr;
#else
    uint32 *font_cache_buffer = GET_REAL_ADDR(font_cache_addr);
#endif

#if ((USE_MULTI_FONTLIB & 0x01) != 0)
    if((disp_mode & DISP_FONT_OPTION) == DISP_FONT_SMALL)
    {
        //�л���С�����ֿ�
        if(fix_current_font != 0)
        {
            libc_memcpy(FontLib_fp, &fix_font_files[0], sizeof(sd_file_t));
            fix_current_font = 0;
            fix_font_height = FONTLIB_1_HEIGHT;
            fix_font_height_actual = FONTLIB_1_HEIGHT_ACTUAL;
            fix_font_height_up_margin = (fix_font_height - fix_font_height_actual)/2;
            fix_font_height_line = fix_font_height_actual + (fix_font_height_actual/4);
            fix_font_width = FONTLIB_1_WIDTH;
            fix_font_size = FONTLIB_1_SIZE;
            *font_cache_buffer = 0x0;//���font������
        }
        return ;
    }
#endif

#if ((USE_MULTI_FONTLIB & 0x04) != 0)
    if((disp_mode & DISP_FONT_OPTION) == DISP_FONT_LARGE)
    {
        //�л����������ֿ�
        if(fix_current_font != 2)
        {
            libc_memcpy(FontLib_fp, &fix_font_files[2], sizeof(sd_file_t));
            fix_current_font = 2;
            fix_font_height = FONTLIB_3_HEIGHT;
            fix_font_height_actual = FONTLIB_3_HEIGHT_ACTUAL;
            fix_font_height_up_margin = (fix_font_height - fix_font_height_actual)/2;
            fix_font_height_line = fix_font_height_actual + (fix_font_height_actual/4);
            fix_font_width = FONTLIB_3_WIDTH;
            fix_font_size = FONTLIB_3_SIZE;
            *font_cache_buffer = 0x0;//���font������
        }
        return ;
    }
#endif

    //�л����������ֿ�
    if(fix_current_font != 1)
    {
        libc_memcpy(FontLib_fp, &fix_font_files[1], sizeof(sd_file_t));
        fix_current_font = 1;
        fix_font_height = FONTLIB_2_HEIGHT;
        fix_font_height_actual = FONTLIB_2_HEIGHT_ACTUAL;
        fix_font_height_up_margin = (fix_font_height - fix_font_height_actual)/2;
        fix_font_height_line = fix_font_height_actual + (fix_font_height_actual/4);
        fix_font_width = FONTLIB_2_WIDTH;
        fix_font_size = FONTLIB_2_SIZE;
        *font_cache_buffer = 0x0;//���font������
    }
    return ;
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
bool fixf_put_char(uint8 *psoure_buf, char_fill_mode_e mode, char_disp_e char_disp, uint8 start_col)
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
        fixf_char_fill(psoure_buf, char_disp, start_col);
    }
    //�Ȱ��ַ����嵽��ʾ�������У���DMA��ʾ����
    else
    {
        //�Ȱ��ַ����嵽��ʾ�������У���DMA��ʾ����
        //���㻺����Ԥ������
        buf_region.x = 0;
        buf_region.y = 0;
        buf_region.height = fix_font_height_actual;
        buf_region.width = region.width;
    
        //��ȡ�ַ�������ͼƬ�����贰
        read_str_bgpic(display_buffer, &region, mode);
    
        fixf_black_char_to_color(psoure_buf, &buf_region, char_disp, start_col);
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
#endif
    
/*! \endcond */
