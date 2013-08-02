/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-20 9:42     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_string.c
 * \brief    �ַ����������ܽӿ�ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ������ʽ���ַ�����ϣ��ȵȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               psp
 * \version  1.0
 * \date     2011-9-20
*******************************************************************************/

#include "common_func.h"

static bool __symbol_search(uint8 *str, uint16 len, int8 language, uint16 *scan_len);

//�����������ݳ���Ϊ124�ֽڣ�ͬ UI �����ڲ�������һ��
//��������Ϊ192�ֽڣ���Ϊ����ANSI������������ʱ������ת�������UNICODE�ַ�
#define PATTERN_BUF_LEN  192
#define SUB_BUF_LEN      128
//�ַ������ϻ�����
uint8 pattern_buf[PATTERN_BUF_LEN] _BANK_DATA_ATTR_;
uint8 sub_buf[SUB_BUF_LEN] _BANK_DATA_ATTR_;

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    �ַ�����ʽ����������ƴ�Ӵ����
 * \param[in]    pattern �ַ���ģʽ�����ַ��� ID ���ڣ�ͨ�� pattern->argv ���ݣ�
 *                        ���� [%s] ���滻Ϊ�����ַ���ָ�����ָ����ַ�����
 * \param[in]    strings �����滻���ַ������������飬�����code�����������������
 * \param[out]   pattern ���ظ�ʽ��������������������� pattern->data.str������
                         ������ pattern->lang_id ָ����
 * \return       bool
 * \retval           �����Ƿ���ɹ���TRUE��ʾ�ɹ���FALSE��ʾʧ�ܡ�
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    ����1�����ļ������뵽ģ���ַ����У������ַ������ݣ�����ɾ�����ļ�����...
    string_desc_t pattern, input_str;
    uint8 pattern_buffer[PATTERN_BUF_LEN];
    
    pattern.argv = S_DELETING;
    pattern.data.str = pattern_buffer;
    pattern.length = PATTERN_BUF_LEN;
    pattern.language = UNICODEDATA;
    
    input_str.data.str = file_name;
    input_str.language = UNICODEDATA;
    input_str.length = -1;//Unicode�ַ���������Ϊ����ȡ������д����Ҫ���ַ���ĩβ�н�����\0\0
    
    com_string_format(&pattern, &input_str);
 * \endcode
 * \note
 * \li  �ַ������Ͻ������������ pattern ��������С��ҪԤ�����������������ص���
 * \li  ��� strings ���������ַ���Ϊ UNICODEDATA ����ô�ᱻ�ض�Ϊ 128 �ֽڡ�
 * \li  ��� strings ���������ַ�����Ϊ������룬��pattern->language ����ָ��Ϊ
 *      ��ǰ�������ͣ�Ҳ����ָ��ΪUNICODEDATA��
 * \li  ��� strings ���������ַ����� UNICODEDATA ����ô����� pattern->language
 *      ָ��ΪUNICODEDATA���Ա�֤��������ܹ�������ʾ��
*******************************************************************************/
bool com_string_format(string_desc_t *pattern, string_desc_t *strings)
{
    string_desc_t parrern_str;
    uint8 *copy_str;
    uint16 copy_len;
    uint16 max_len;
    uint16 len;
    uint16 total, index, result_index;
    uint16 str_index = 0;
    uint16 result;
    uint8  pt_len;
    bool   ret = FALSE;
    uint8 *result_str = pattern->data.str;
    int8   result_lang = pattern->language;
    bool   is_ansi;
    
    parrern_str.data.str = pattern_buf;
    parrern_str.length = PATTERN_BUF_LEN;
    ui_get_multi_string_unicode(pattern->argv, &parrern_str);
    if(result_lang == UNICODEDATA)
    {
        total = len = (uint16)(int)libc_strlenuni(pattern_buf);
        max_len = pattern->length - 2;
        pt_len = 8;
    }
    else
    {
        ui_unicode_to_char(pattern_buf, parrern_str.length, result_lang);
        total = len = (uint16)libc_strlen(pattern_buf);
        max_len = pattern->length - 1;
        pt_len = 4;
    }
    
    index = 0;
    result_index = 0;
    while(1)
    {
        if(__symbol_search(pattern_buf + index, len, result_lang, &result) == FALSE)
        {
            if((result_index + result) >= max_len)
            {
                libc_memcpy(result_str + result_index, pattern_buf + index, max_len - result_index);
                result_index = max_len;
            }
            else
            {
                libc_memcpy(result_str + result_index, pattern_buf + index, result);
                result_index += result;
            }
            
            break;//ɨ����ϣ��˳�
        }
        
        //���� pattern �����ַ���
        if((result_index + result) >= max_len)
        {
            libc_memcpy(result_str + result_index, pattern_buf + index, max_len - result_index);
            result_index = max_len;
            break;//buffer��������ֹ����
        }
        libc_memcpy(result_str + result_index, pattern_buf + index, result);
        result_index += result;
        
        //�����ַ���ת��
        is_ansi = FALSE;
        if(strings[str_index].language == UNICODEID)
        {
            //��������ַ���תΪUnicode ����
            parrern_str.data.str = sub_buf;
            parrern_str.length = SUB_BUF_LEN;
            ui_get_multi_string_unicode(strings[str_index].data.id, &parrern_str);
        }
        else if(strings[str_index].language == UNICODEDATA)
        {
            //����SUB_BUF_LEN�ֽڣ���� strings[str_index].data.str ��Ч���ȴ���SUB_BUF_LEN-2����ô�ͻᱻ�ص�����
            libc_memcpy(sub_buf, strings[str_index].data.str, SUB_BUF_LEN);
        }
        else if(strings[str_index].language == UTF_8DATA)
        {
            //UTF-8����ת��ΪUNICODE���룬ĩβ����ӽ�����
            ui_utf8_to_unicode(strings[str_index].data.str, sub_buf, &copy_len);
        }
        else
        {
            is_ansi = TRUE;
        }
        
        //���������ַ���
        if(result_lang == UNICODEDATA)
        {
            //�ϲ��ַ���ԴΪ��������ֽ������������Ϳ��ܻᵼ��������ƥ����ʾ����
            if(is_ansi == TRUE)
            {
                copy_len = (uint16)libc_strlen(strings[str_index].data.str);
                ui_char_to_unicode(sub_buf, strings[str_index].data.str, copy_len);
            }
            copy_str = sub_buf;
            copy_len = (uint16)(int)libc_strlenuni(copy_str);
        }
        else
        {
            //�ϲ��ַ���ԴΪ��������ֽ������������Ϳ��ܻᵼ��������ƥ����ʾ����
            if(is_ansi == TRUE)
            {
                copy_str = strings[str_index].data.str;
            }
            else
            {
                ui_unicode_to_char(sub_buf, SUB_BUF_LEN, result_lang);
                copy_str = sub_buf;
            }
            copy_len = (uint16)libc_strlen(copy_str);
        }
        
        if((result_index + copy_len) >= max_len)
        {
            libc_memcpy(result_str + result_index, copy_str, max_len - result_index);
            result_index = max_len;
            break;//buffer��������ֹ����
        }
        libc_memcpy(result_str + result_index, copy_str, copy_len);
        result_index += copy_len;
        
        len = len - result - pt_len;
        index = total - len;
        str_index++;
    }
    
    result_str[result_index] = 0;
    if(result_lang == UNICODEDATA)
    {
        result_str[result_index + 1] = 0;
    }
    pattern->length = result_index;

    return ret;
}

/*! \endcond */

/*! \cond */

//����ָ������[%s]
static bool __symbol_search(uint8 *str, uint16 len, int8 language, uint16 *scan_len)
{
    uint16 i;
    uint16 *str_16;
    
    if(language == UNICODEDATA)
    {
        if(len < 8)
        {
            *scan_len = len;
            return FALSE;
        }
        str_16 = (uint16 *)str;
        for(i = 0; i <= ((len/2) - 4); i++)
        {   
            if(  (str_16[i] == '[')
               &&(str_16[i + 1] == '%')
               &&(str_16[i + 2] == 's')
               &&(str_16[i + 3] == ']') )
            {
                *scan_len = i*2;
                return TRUE;
            }
        }
    }
    else
    {
        if(len < 4)
        {
            *scan_len = len;
            return FALSE;
        }
        for(i = 0; i <= (len - 4); i++)
        {   
            if(  (str[i] == '[')
               &&(str[i + 1] == '%')
               &&(str[i + 2] == 's')
               &&(str[i + 3] == ']') )
            {
                *scan_len = i;
                return TRUE;
            }
        }
    }
    
    *scan_len = len;
    return FALSE;
}

/*! \endcond */
