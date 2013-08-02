/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_putstring.c
 * \brief    �ַ���������ʾģ��
 * \author   liutingting
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ָ��λ����ʾ�ַ���
 * \param[in]    str_desc��Ҫ��ʾ���ַ����ṹ��ָ�룬�μ�string_desc_t����
 * \param[in]    str_region����ʾ�ַ���������,�μ�region_t����
 * \param[in]    mode���ַ�������ʾģʽ���μ�display.h���ַ�����ʾģʽ��ʽ����
 * \param[out]   none
 * \return       uint8���͵Ĳ�������ʾ�Ƿ�ɹ�
 * \retval       UI_NO_ERR ������ʾ
 * \retval       UI_NEED_SCROLL ��Ҫ������ʾ
 * \ingroup      string
 * \par          exmaple code
 * \code
    string_desc_t desc;
    region_t kregion;
    uint16 disp_mode;
    
    desc.data.str = str;
    desc.length = (uint16) libc_strlen(str);
    desc.language = language;
    result = (ui_result_e) show_string(&desc, &kregion, disp_mode);
 * \endcode
 * \note
 *******************************************************************************/
uint8 show_string(string_desc_t *str_desc, region_t *str_region, uint16 mode)
{
    string_desc_t string_infor;

    libc_memset(string_buf, 0, STRING_BUFFER_LEN);
    libc_memcpy(&string_infor, str_desc, sizeof(string_desc_t));
    arabic_stat = ARABIC_INIT;
    if (str_desc->language == UNICODEID)
    {
        uint16 str_id = string_infor.data.id;

        string_infor.data.str = string_buf;
        string_infor.length = STRING_BUFFER_LEN;
        string_infor.language = UNICODEDATA;
        romf_get_unicode_data(str_id, &string_infor);
    }
    else if (str_desc->language == UTF_8DATA)
    {
        int16 true_len;
        
        //��ʱ���� arabic_string_buf ת��
        libc_memcpy(arabic_string_buf, string_infor.data.str, (uint32)string_infor.length);
        arabic_string_buf[string_infor.length] = 0;
        utf8_to_unicode(arabic_string_buf, (uint16 *)string_buf, &true_len);
        string_infor.data.str = string_buf;
        string_infor.length = (uint16)true_len;
        string_infor.language = UNICODEDATA;
    }
    else
    {
        uint16 true_len;

        true_len = string_infor.length;
        if (true_len > STRING_BUFFER_LEN)//һ��Ϊ-1��Ҫ���н�������������ܻ������
        {
            if (string_infor.language == UNICODEDATA)
            {
                true_len = (uint16)libc_strlenuni(string_infor.data.str);
            }
            else
            {
                true_len = (uint16)libc_strlen(string_infor.data.str);
            }
            if (true_len > STRING_BUFFER_LEN)
            {
                true_len = STRING_BUFFER_LEN;
            }
        }
        libc_memcpy(string_buf, string_infor.data.str, (uint32)true_len);
        string_infor.data.str = string_buf;
        string_infor.length = true_len;
    }
    
    //����ѡ��˵��У��˵����ַ������⴦��
    if((mode & DISP_LANG_LIST_MENU) != 0)
    {
        uint8 lang;
        lang = is_arabic_hebrew_string(&string_infor);
        
        //��ͨ�����°�������ϣ�����ﴦ��
        if(need_arabic_deal(mode) == FALSE)
        {
            if(lang != 0)//��������ϣ������
            {
                //���а�������д��������ת�ַ���
                if(lang == 1)//��������
                {
                    arabic_do_join(&string_infor);
                }
                arabic_do_turn(string_infor.data.str, 0, string_infor.length);
            }
        }
        //ϣ�������°����������ͨ���Դ���
        else if(language_id == HEBREW)
        {
            if(lang == 1)//��������
            {
                //���а�������д����
                arabic_do_join(&string_infor);
            }
            else if(lang == 0)//��ͨ����
            {
                arabic_stat = ARABIC_LINE_TURN;//����ֱ����ʾ
            }
            else
            {
                //for QAC
            }
        }
        //������������ͨ���Դ���
        else
        {
            if(lang == 0)//��ͨ����
            {
                arabic_stat = ARABIC_LINE_TURN;//����ֱ����ʾ
            }
        }
    }
    
    //��������ϣ���������⴦��
    if(need_arabic_deal(mode) == TRUE)
    {
        //1. ��������ϣ������Ҫ���� UNICODE ��д����
        if(arabic_stat == ARABIC_INIT)
        {
            string_infor.argv = STRING_BUFFER_LEN;
            arabic_uni_join(&string_infor);
            
            //��������ϣ�������Ѿ����� UNICODE ��д����
            arabic_stat = ARABIC_UNI_JOIN;
        }
        
        //2. ��������ϣ������Ҫ�������ķ�ת����
        if(arabic_stat == ARABIC_UNI_JOIN)
        {
            string_infor.argv = STRING_BUFFER_LEN;
            arabic_ascii_turn(&string_infor);
            
            //��������ϣ�������Ѿ��������ķ�ת����
            arabic_stat = ARABIC_ASCII_TURN;
        }
    }

    //ת���������ͣ�תΪ�ڲ��������ԣ�UNICODELANGUAGE������ֵ
    if (string_infor.language == UNICODEDATA)
    {
        string_infor.language = UNICODELANGUAGE;
    }
    
    scroll_flag = FALSE;//��put_string���ж��Ƿ���Ҫ����
    //ע��put_string�ı��������ܲ���������
    put_string(&string_infor, str_region, mode);

    //��Ҫ���������й�����ش���
    if (scroll_flag == TRUE)
    {
        libc_memset(scroll_buf, 0, STRING_BUFFER_LEN);
        libc_memcpy(scroll_buf, string_infor.data.str, (uint32)string_infor.length);
        scroll_str.data.str = scroll_buf;
        scroll_str.language = string_infor.language;
        scroll_str.length = string_infor.length;

        return UI_NEED_SCROLL;
    }

    return UI_NO_ERR;
}
/*! \endcond */


/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ָ��λ����ʾ�ַ���
 * \param[in]    str_desc Ҫ��ʾ���ַ����ṹ��ָ�룬�μ�string_desc_t����
 * \param[in]    str_region ��ʾ�ַ���������,�μ�region_t����
 * \param[in]    mode �ַ�������ʾģʽ���μ�display.h���ַ�����ʾģʽ��ʽ����
 * \param[out]   none
 * \return       uint8* ��ʾ�����ַ���ָ��
 * \ingroup      
 * \note
 *******************************************************************************/
uint8 *put_string(string_desc_t *string_infor, region_t *str_region, uint16 mode)
{
    string_desc_t string_backup;
    uint8 *nextchar;
    uint8 *cur_str;
    uint16 str_len;
    region_t cur_region;//������ʾ����
    region_t kregion;//�������ַ���ʾ����
    str_getlen_result_t getlen_result;
    uint16 length;
    uint16 adjust_x = 0;//��������ϣ����x�������
    uint16 adjust_w = 0;//��������ϣ����x�������
    uint16 length_bk;
    
#if (USE_MULTI_FONTLIB != 0)
    //�л������ֿ�
    fix_switch_fontlib(mode);
#endif
    
    //��ʼ��ԭʼ����
    kregion.y = cur_region.y = str_region->y;
    kregion.height = cur_region.height = fix_font_height_actual;
    cur_str = string_infor->data.str;
    length = string_infor->length;
    
    //��ʾһ���ַ�����������ʾ���򣨰���������ʾ����Ͳ������ַ���ʾ����
    put_str_again: 
#if (USE_MULTI_FONTLIB == 0)
    str_len = romf_get_string_length(string_infor, str_region->width, &getlen_result);
#else
    str_len = fixf_get_string_length(string_infor, str_region->width, &getlen_result);
#endif
    cur_region.width = getlen_result.true_length;
    kregion.width = str_region->width - getlen_result.true_length;
    
    //��ʡ�Ժ���ʾ�ĳ����ַ�����Ҫ��ʡ�ԺŴ����������¼��㳤��
    if ((str_len > str_region->width) && ((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_ELLIPSIS))
    {
        string_infor->length = ellipsis_to_longstr(string_infor, cur_region.width, &getlen_result);
        //�����ַ�������
        str_len = getlen_result.true_length;
        cur_region.width = getlen_result.true_length;
        kregion.width = str_region->width - getlen_result.true_length;
    }

    //���ַ�������������������ַ���
    if (str_len <= str_region->width)
    {
        // ������ʾ
        if ((mode & DISP_ALIGN_OPTION) == DISP_ALIGN_MEDIACY)
        {
            cur_region.x = str_region->x + (str_region->width - str_len) / 2;
        }
        // ������ʾ
        else if ((mode & DISP_ALIGN_OPTION) == DISP_ALIGN_RIGHT)
        {
            if (need_arabic_deal(mode) == TRUE)
            {
                cur_region.x = str_region->x;
                adjust_x = str_region->x + str_len;
                adjust_w = str_region->width - str_len;
            }
            else
            {
                cur_region.x = (str_region->x + str_region->width) - str_len;
            }
        }
        // ������ʾ
        else
        {
            if (need_arabic_deal(mode) == TRUE)
            {
                cur_region.x = (str_region->x + str_region->width) - str_len;
                adjust_x = str_region->x;
                adjust_w = str_region->width - str_len;
            }
            else
            {
                cur_region.x = str_region->x;
            }
        }
    }
    //���ַ�������RECT�͹�����ʽ��Ҫ��ʾ�������ַ�
    else
    {
        if ((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_SCROLL)
        {
            scroll_flag = TRUE;//��Ҫ����
        }
        
        // ������ʾ
        if ((mode & DISP_ALIGN_OPTION) == DISP_ALIGN_MEDIACY)
        {
            //����ʾ�������ַ��Լ�������ʾ����Ҫ������ʾ
            if (((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_NO_RECT) 
             || ((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_MULTI_ROW))
            {
                cur_region.x = str_region->x + (str_region->width - getlen_result.true_length) / 2;
            }
            else
            {
                //��ͬ�ھ��������ʾ
                goto over_left_right;
            }
        }
        // ���������ʾ
        else
        {
            over_left_right: 
            if (need_arabic_deal(mode) == TRUE)
            {
                cur_region.x = (str_region->x + str_region->width) - getlen_result.true_length;
                kregion.x = cur_region.x - (str_region->width - getlen_result.true_length);
                adjust_x = str_region->x;
                adjust_w = str_region->width - getlen_result.true_length;
            }
            else
            {
                cur_region.x = str_region->x;
                kregion.x = str_region->x + getlen_result.true_length;
            }
        }
    }

    //����Ҫ������ʾ������Ϊ�˴����ַ����������Ƿ�������
    if((mode & DISP_DISPLAY_DISABLE) != 0)
    {
        nextchar = NULL;
        goto putstring_end;
    }
    
    //���а�������ת��ʵ�ִ�������ʾ��
    libc_memcpy(&string_backup, string_infor, sizeof(string_desc_t));
    ramf_put_string_line_hook(string_infor, mode, getlen_result.true_bytes);
    
    if((mode & DISP_DIVIDE_WORD_ENABLE) != 0)//�Զ��ִ�
    {
        uint16 line_mode;
        
        length_bk = string_infor->length;
        string_infor->argv = cur_region.width;
        line_mode = DIVIDE_WORD_ENABLE | (mode & DISP_FONT_OPTION);
        get_text_line(string_infor, line_mode, NULL);
        string_infor->length = string_infor->result;
    }
    
    if((mode & DISP_CHAR_FILL_OPTION) == CHAR_FILL_SDIMAGE)//��SD��ȡ��������ֱ�ӵ����ַ���
    {
        //��ʾ�����ַ���
        nextchar = put_string_line_sdimage(string_infor, &cur_region, 1);
    }
    else
    {
        //��ʾ�����ַ���
#if (USE_MULTI_FONTLIB == 0)
        nextchar = romf_put_string_line(string_infor, &cur_region, mode & DISP_CHAR_FILL_OPTION);
#else
        nextchar = fixf_put_string_line(string_infor, &cur_region, mode & DISP_CHAR_FILL_OPTION);
#endif
    }
    if((mode & DISP_DIVIDE_WORD_ENABLE) != 0)//�Զ��ִ�
    {
        string_infor->length = length_bk;
    }
    arabic_stat = ARABIC_ASCII_TURN;
    
    //put_string_line ��Ҫ����regionΪʣ�����򣬰�������ϣ������Ҫadjust
    if (need_arabic_deal(mode) == TRUE)
    {
        region.x = adjust_x;
        region.width = adjust_w;
        nextchar = string_backup.data.str + (nextchar - string_infor->data.str);
    }
    if (str_len > str_region->width)
    {
        string_infor->result = STRING_TAIL_REGION_ERR;
    }
    
    //��������
    //��ʾ�������ַ�
    if (((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_RECT) || ((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_SCROLL))
    {
        // �����������ַ�
        if ((string_infor->result == STRING_TAIL_REGION_ERR) && (kregion.width > 0))//ʣ���Ȳ�Ϊ0����ʾ�������ַ�
        {
            char_out_rect(nextchar, string_infor->language, &kregion, mode, 0, 1);
        }
    }
    else if ((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_MULTI_ROW)
    {
        switch (string_infor->result)
        {
            /*! �������з� */
            case STRING_TAIL_CHAR_LR:
            /*! ��ʾ����Խ�� */
            case STRING_TAIL_REGION_ERR:
            string_infor->length = (uint16) (string_infor->length - (nextchar - string_infor->data.str));
            string_infor->data.str = cur_str = nextchar;
            cur_region.y += fix_font_height_line;//������ʾ
            if ((cur_region.y + fix_font_height_actual) <= (str_region->y + str_region->height))
            {
                goto put_str_again;
            }
            break;

            /*! �ַ�����Ч�������� */
            case STRING_TAIL_LEN_ZERO:
            /*! �����ַ�����ֹ������δ������ */
            case STRING_TAIL_EOF_NOPEND:
            /*! �������� */
            case STRING_TAIL_OTHER_ERR:
            break;

            default:
            break;
        }
    }
    else
    {
        //do nothing for QAC
    }
    
    putstring_end:
    string_infor->data.str = cur_str;
    string_infor->length = length;
    return nextchar;
}
/*! \endcond */
