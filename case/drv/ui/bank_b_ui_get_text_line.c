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
 * \file     ui_get_text_line.c
 * \brief    �ı��ַ������ȴ���ͳ��ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               �����ַ��ı������ͣ����ı��ַ������г��ȴ���ͳ�ƣ������ı����еȹ���
 * \par      EXTERNALIZED FUNCTIONS:
 *              none
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

#define SEP_WORD_COUNT      18//19 ȥ�����и����

//���ʷָ�����
static const uint8 sep_word[SEP_WORD_COUNT] = 
{ 
    0x00, //������
    0x20, //�ո�
    0x21, //��̾��!
    0x22, //˫����"
    //0x27, //���и���� ���и����Ӧ����д
    0x2c, //����,
    0x2e, //���.
    0x2f, //б��/
    0x3a, //ð��:
    0x3b, //�ֺ�;
    0x3c, //��������<
    0x3d, //���ں�=
    0x3e, //��������>
    0x3f, //�ʺ�?
    0x40, //�����ַ����@
    0x5b, //��������[
    0x5d, //��������]
    0x7b, //������{
    0x7d, //�һ�����}
};

/*! \cond */

//�ж��Ƿ�word�ַ���Ŀǰ֧��Ӣ�ĵ���
static bool check_word_char(uint8 *str, int8 language)
{
    uint16 tmp_code;
    uint8 i;
    
    if (language == UNICODELANGUAGE)
    {
        tmp_code = *str + ((uint16) *(str + 1) << 8);
    }
    else
    {
        tmp_code = *str;
    }
    
    if (tmp_code >= 0x80)//��ASCII��
    {
        return FALSE;
    }
    
    for (i = 0; i < SEP_WORD_COUNT; i++)
    {
        if (sep_word[i] == (uint8)tmp_code)
        {
            return FALSE;//�ָ���
        }
    }
    
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��ǰλ���ַ�����--utf8����
 * \param[in]    str����ǰ�ַ�����ʼλ��
 * \param[in]    length����ǰ��Ч�ֽ���
 * \param[in]    language���ַ���������������
 * \param[out]   byte_cnt�����ص�ǰ��Ч�ַ����ֽ���
 * \return       char_type_e�������ַ�����
 * \note         ������ַ���һ�����ḻ�ĸ������Ϊ������ʾ���ַ���Ԫ��
 *******************************************************************************/
char_type_e get_next_char_type_utf8(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)
{
    uint8 tmp_char;

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
        if (length >= 3)
        {
            if ((*str == 0xef) && (*(str + 1) == 0xbb) && (*(str + 2) == 0xbf))
            {
                *byte_cnt = 3;
                return CHAR_UNI_FLAG;
            }
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

    //��ͨ�ַ��ж�
    tmp_char = *str;
    if ((tmp_char < 0x80) && (length >= 1))//0h0#######
    {
        *byte_cnt = 1;
        return CHAR_NORMAL;
    }
    else if ((tmp_char < 0xe0) && (length >= 2)) //0h110#####
    {
        *byte_cnt = 2;
        return CHAR_NORMAL;
    }
    else if ((tmp_char < 0xf0) && (length >= 3)) //0h1110####
    {
        *byte_cnt = 3;
        return CHAR_NORMAL;
    }
    else
    {
        //do nothing for QAC
    }

    *byte_cnt = 0;
    return CHAR_NOPEND;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��ǰ�ַ��ĵ���
 * \param[in]   str:��ǰ�ַ�ָ��
 * \param[in]   language����ǰ�ַ���������
 * \param[out]  pbuffer:�����ַ������buffer
 * \return      ��ǰ�ַ����ֽ�����2�ֽڻ�1�ֽ�
 * \note
 *******************************************************************************/
static uint8 s_get_next_char_point_hook(uint8 *str, int8 language, uint8 *pbuffer)
{
    uint16 uni_code = 0;
    uint8 pt_step = 0;

    if (language == UTF_8DATA)
    {
        if (*str < 0x80)//0h0#######
        {
            uni_code = *str;
            pt_step = 1;
        }
        else if (*str < 0xe0) //0h110#####
        {
            uni_code = ((uint16) (*str) & 0x1f) << 6;
            uni_code += ((uint16) (*(str + 1)) & 0x3f);
            pt_step = 2;
        }
        else if (*str < 0xf0) //0h1110####
        {
            uni_code = ((uint16) (*str) & 0xf) << 12;
            uni_code += ((uint16) (*(str + 1)) & 0x3f) << 6;
            uni_code += ((uint16) (*(str + 2)) & 0x3f);
            pt_step = 3;
        }
        else
        {
            //do nothing for QAC
        }
    }
    else if ((language == UNICODELANGUAGE) || ((*str> ASCIIDISCODE) && (language_2byte == TRUE)))
    {
        uni_code = *str + ((uint16) (*(str + 1)) << 8);
        pt_step = 2;
    }
    else
    {
        uni_code = *str;
        pt_step = 1;
    }

    //ת��ΪUnicode���룬Unicode���Ժ�С��0x80����������ת��
    if (((language != UNICODELANGUAGE) && (language != UTF_8DATA)) && (uni_code >= ASCIIDISCODE))
    {
        uni_code = romf_multi_char_to_unicode(uni_code);
    }
    //add for thai languange show by liutingting
    if ((uni_code >= 0x0E01) && (uni_code <= 0x0E7F))
    {
        thai_point(str, language, uni_code, pbuffer, &pt_step);
    }
    else
    {
#if (USE_MULTI_FONTLIB == 0)
        romf_get_next_char_point(uni_code, pbuffer);
#else
        fixf_get_next_char_point(uni_code, pbuffer);
#endif
    }
    return pt_step;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ������󵽻�������
 * \param[in]   point_buf ���󻺳���
 * \param[in]   pbuffer ��ǰ�ַ�����
 * \param[in]   pos ��ǰλ��
 * \param[in]   max_width �������п��
 * \return      none
 * \note
 *******************************************************************************/
static void save_char_point(uint8 *point_buf, uint8 *pbuffer, uint16 pos, uint16 max_width)
{
    uint8 i, j;
    uint8 src_data;//0��ʾ�հ׵㣬1��ʾ�ֵ�
    uint8 temp_width;//1��ʾ16*8��ģ��2��ʾ16��16��ģ
    uint8 font_width;
    uint8 *point_buf_base;
    uint8 shift_mask;
    uint8 index;

    //��ʼ��������
    font_width = pbuffer[fix_font_size - 1];
    temp_width = (font_width + 7)/8;

    for (i = 0; i < fix_font_height_actual; i++)//loop for height
    {
        point_buf_base = point_buf + (i * ((max_width + 7) / 8));
        for (j = 0; j <= (font_width - 1); j++)//loop for char width
        {
            if ((j % 8) == 0)
            {
                index = ((i + fix_font_height_up_margin) * temp_width) + (j/8);
                shift_mask = 0x80;
            }
            
            src_data = (uint8)(pbuffer[index] & shift_mask);
            shift_mask >>= 1;

            if (src_data != 0)
            {
                *(point_buf_base + ((pos + j)/8)) |= (uint8)(0x80 >> ((pos + j)%8));
            }
        }
    }
}

/*! \endcond */

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ�ַ�������
 * \param[in]    str_desc���ַ����������ṹ��ָ�룬�����ı����й��ܵ�
 * \param[in]    line_mode���д���ģʽ
 * \param[in]    param���������
 * \param[out]   none
 * \return       text_end_mode_e �� text_end_mode_e ����
 * \ingroup      string
 * \par          exmaple code
 * \code
    string_desc_t str_desc;
    uint8 page_mode;
    bool page_miss;
    
    str_desc.data.str = str_buf;
    str_desc.language = UNICODEDATA;
    str_desc.length = -1;
    page_miss = (bool)(uint32)ui_get_text_line(&str_desc, page_mode);
 * \endcode
 * \note
 *******************************************************************************/
text_end_mode_e get_text_line(string_desc_t *str_desc, uint16 line_mode, void *param)
{
    char_type_e (*get_next_char_type)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
    uint8 *str, *str_col;
    uint16 length;
    int8 language;
    uint8 char_bytes;
    uint16 str_width = 0;//�ַ������ȣ����ص㣩
    uint16 char_width;
    char_type_e char_type;
    uint16 word_index = 0;//���ʿ�ʼλ��
    uint16 word_count = 0;//��ǰ�е�����
    uint16 word_state = 0;//0--���ʼ���У�1--����ɨ�赥��
    uint16 col = 0;
    uint16 lines_cnt = 0;//������
    text_end_mode_e end_mode = FALSE;
    bool scan_over = FALSE;//��ǰ��ɨ���Ƿ����
    textline_param_t *textline_param;
    
#if (USE_MULTI_FONTLIB != 0)
    //�л������ֿ�
    fix_switch_fontlib(line_mode);
#endif
    
    if((line_mode & FUNC_MODE_OPTION) == FUNC_GET_CHAR_POINTS)
    {
        textline_param = (textline_param_t *)param;
        libc_memset(textline_param->point_buf, 0x00, ((str_desc->argv + 7)/8)*fix_font_height_actual);
    }

    //����������ʼ��
    str = str_desc->data.str;
    length = str_desc->length;
    language = str_desc->language;
    if ((language == UNICODEDATA) || (language == UNICODELANGUAGE))
    {
        language = UNICODELANGUAGE;
        get_next_char_type = get_next_char_type_unicode;
    }
    else if (language == UTF_8DATA)
    {
        get_next_char_type = get_next_char_type_utf8;
    }
    else
    {
        get_next_char_type = get_next_char_type_ansi;
    }
    
    str_col = str;
    //�����ַ�����ȡ�ַ�������
    while (1)
    {
        scan_string:
        //�ַ��������Ѿ����
        if (length == 0)
        {
            lines_cnt++;
            end_mode = TEXT_END_PAGE_MISS;
            goto text_line_ret;
        }
        //��ȡ��һ���ַ�����Ϊ�����������з�����ͨ�ַ�
        char_type = get_next_char_type(str, length, language, &char_bytes);
        switch(char_type)
        {
        case CHAR_EOF://������
            lines_cnt++;
            end_mode = TEXT_END_NUL;
            goto text_line_ret;

        case CHAR_NOPEND://���ݲ���
            lines_cnt++;
            end_mode = TEXT_END_PAGE_MISS;
            goto text_line_ret;

        case CHAR_UNI_FLAG://Unicode �ַ�����־ С��Unicode
            str += char_bytes;
            col += char_bytes;
            length -= char_bytes;
            goto scan_string;//����ɨ��

        case CHAR_LR://���з�
            if( ((line_mode & FUNC_MODE_OPTION) == FUNC_GET_LINE_CHARS)
             || ((line_mode & FUNC_MODE_OPTION) == FUNC_GET_CHAR_POINTS) )
            {
                //�����˿��У���β���з���Ҫ��������˵�������Ļ��з�������
                if ((line_mode & FILTER_BLANK_OPTION) == FILTER_BLANK_DISABLE)
                {
                    str += char_bytes;
                    col += char_bytes;
                    length -= char_bytes;
                }

                end_mode = TEXT_END_LR;
                goto text_line_ret;
            }
            else
            {
                str += char_bytes;
                col += char_bytes;
                length -= char_bytes;
                scan_over = TRUE;
            }
            break;

        default://��ͨ�ַ�����
            //�ִʼ��
            if ((line_mode & DIVIDE_WORD_OPTION) == DIVIDE_WORD_ENABLE)
            {
                //���ڷִ�
                if (word_state == 1)
                {
                    //������word�ַ�����ʾ���ʽ���
                    if(check_word_char(str, language) == FALSE)
                    {
                        word_count++;
                        word_state = 0;
                    }
                }
                //δ��ʼ�ִ�
                else
                {
                    //����word�ַ�����ʾ���ʿ�ʼ
                    if (check_word_char(str, language) == TRUE)
                    {
                        word_index = col;
                        str_col = str;
                        word_state = 1;
                    }
                    else//�����ַ��������ָ���������word����
                    {
                        word_count++;
                    }
                }
            }
            break;
        }
        
        //ɨ���һ��
        if(scan_over == TRUE)
        {
            lines_cnt++;
            str_width = 0;
            col = 0;
            word_count = 0;
            word_state = 0;
            scan_over = FALSE;
            continue;
        }

        //����nor �����ü��ֿ�������ֿ�
        char_bytes = s_get_next_char_point_hook(str, language, fix_p_char_point);

        //�ַ�������ۼ�
        char_width = fix_p_char_point[fix_font_size - 1];
        if ((str_width + char_width) > str_desc->argv)//�Ѿ�������󳤶ȣ������ټ���
        {
            //�ع�һ������
            if (  ((line_mode & DIVIDE_WORD_OPTION) == DIVIDE_WORD_ENABLE) //�ִʼ��
               && (word_state == 1)//���ڷִ�
               && (word_count > 0))//��ǰ���Ѿ��ֳ��������ʳ��������ⳬ�������޷��ִ� bug
            {
                length += (col - word_index);
                col = word_index;
                str = str_col;
            }
            end_mode = TEXT_END_OVER;
            if( ((line_mode & FUNC_MODE_OPTION) == FUNC_GET_LINE_CHARS)
             || ((line_mode & FUNC_MODE_OPTION) == FUNC_GET_CHAR_POINTS) )
            {
                goto text_line_ret;
            }
            else
            {
                scan_over = TRUE;
            }
        }
        
        if(scan_over == FALSE)
        {
            if((line_mode & FUNC_MODE_OPTION) == FUNC_GET_CHAR_POINTS)//�������
            {
                save_char_point(textline_param->point_buf, fix_p_char_point, str_width, str_desc->argv);
            }
            str += char_bytes;
            length -= char_bytes;
            col += char_bytes;
            str_width += char_width;
        }
        else
        {
            lines_cnt++;
            str_width = 0;
            col = 0;
            word_count = 0;
            word_state = 0;
            scan_over = FALSE;
        }
    }
    
    text_line_ret://���ؽ��
    if((line_mode & FUNC_MODE_OPTION) == FUNC_GET_LINE_CHARS)
    {
        str_desc->result = col;//���ص�ǰ���ֽ���
    }
    else if((line_mode & FUNC_MODE_OPTION) == FUNC_GET_CHAR_POINTS)
    {
        textline_param->true_width = str_width;
        textline_param->true_height = fix_font_height_actual;
        str_desc->result = col;//���ص�ǰ���ֽ���
    }
    else
    {
        str_desc->result = lines_cnt;//�����ַ���������
    }

    return end_mode;
}

/*! \endcond */
