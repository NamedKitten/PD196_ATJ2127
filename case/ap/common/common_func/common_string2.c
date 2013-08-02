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
 * \file     _common_string2.c
 * \brief    �ַ����������ܽӿ�ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ���������ӵ㣬�ȵȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               psp
 * \version  1.0
 * \date     2011-9-20
*******************************************************************************/

#include "common_func.h"

uint8 tmp_buffer[52] _BANK_DATA_ATTR_;

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    Ϊ������ӵ�ţ�����Ϊ xxxxxxxx.yyy ��ʽ��
 * \param[in]    short_name 8+3����
 * \param[out]   short_name �����Ѽӵ�Ŷ��ļ���
 * \return       none
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    ����1���������ļ�����.��
    //��ȡ���ļ����������� g_filename ��
    if((g_filename[0] != 0xff) || (g_filename[1] != 0xfe))//���룬Ϊ�����ļ���
    {
        com_dot_to_shortname(g_filename);
    }
    else//Unicode�룬�����������ļ���
    {
        Unicode���ļ�������
    }
 * \endcode
 * \note
 * \li  �û�Ӧ�ñ�֤ short_name �ռ䲻���� 13 �ֽڣ������п��ܲ����ڴ��������
*******************************************************************************/
void com_dot_to_shortname(uint8 *short_name)
{
    uint8 i = 7, result;
    
    //���˵��ո�
    while(short_name[i] == ' ')
    {
        i--;
    }
    
    //��������ӵ��
    result = i + 1;
    libc_memcpy(tmp_buffer, short_name, result);
    tmp_buffer[result] = '.';
    result++;
    libc_memcpy(tmp_buffer + result, short_name + 8, 3);
    result += 3;
    tmp_buffer[result] = '\0';
    result++;
    
    //�������
    libc_memcpy(short_name, tmp_buffer, result);
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ����������ļ���ת��ΪUnicode�����ļ�����
 * \param[in]    file_name ��������ļ���
 * \param[out]   file_name ���Unicode�����ļ���
 * \return       none
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    ����1������������ļ���ת��ΪUnicode�����ļ���
    const char const_rec_name[] = "rec_001.wav";
    uint8 new_rec_name[26];
    
    libc_memcpy(new_rec_name, const_rec_name, 12);
    com_ansi_to_unicode(new_rec_name);
    //���ţ��Ϳ���ʹ�� new_rec_name ����������Ϊ rec_001.wav ���ļ���
 * \endcode
 * \note
 * \li  ���ڴ����ļ�ǰ���ļ���ת��ΪUnicode���룬����Ӧ exFat �ļ�ϵͳ��
 * \li  �û����Լ���֤ file_name �����Unicode�����ļ��������� Ӣ�������ļ���
 *      rec_001.wav��ת��Unicode�����ļ�������Ҫ��������СΪ 26 ���ֽڣ���
 *      Unicode�����־�ַ�0xfffe 2�ֽ� + 11���ַ� 2 * 11�ֽ� + ������0x0 0x0 2�ֽڡ�
 * \li  �����ڸ�����������С��52�ֽڣ����ļ������ܳ��� 24 ���ַ����ȡ�
*******************************************************************************/
void com_ansi_to_unicode(uint8 *file_name)
{
    uint16 ansi_len = (uint16)libc_strlen(file_name);
    uint16 i;
    
    //�����ƶ�2�ֽڣ�ǰ����� 0xfffe
    for(i = ansi_len; i > 0; i--)
    {
        file_name[(i - 1) + 2] = file_name[i - 1];
    }
    file_name[0] = 0xff;
    file_name[1] = 0xfe;
    file_name[ansi_len + 2] = 0x0;
    
    //���ļ���ת��ΪUnicode���� com_name_buffer
    ui_char_to_unicode(tmp_buffer, file_name + 2, ansi_len);
    
    //����Unicode����
    libc_memcpy(file_name + 2, tmp_buffer, ansi_len*2 + 2);
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���ļ�������Ϊ"����..."��ʽ��
 * \param[in]    long_name �ļ���������
 * \param[in]    ellipsis_len ����ļ�������������
 * \param[out]   ellipsis_name ����ļ���������
 * \return       uint16 ���������ַ�������ֽڳ���
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    ����1�����ļ�������Ϊ������...����ʽ
    com_ellipsis_to_longname(g_filename, com_name_buffer, 52);
 * \endcode
 * \note
 * \li  ��� longstr Ϊ UNICODE ���룬�û�Ӧ�ñ�֤��ͷΪ 0xfffe ��־��
 * \li  ������������...������ʾ�������Ϊ ELLIPSIS_MAX_LEN ����84�����ص㡣
*******************************************************************************/
uint16 com_ellipsis_to_longname(uint8 *long_name, uint8 *ellipsis_name, uint16 ellipsis_len)
{
    string_desc_t ellipsis_desc;
    uint16 true_bytes;
    
    libc_memcpy(ellipsis_name, long_name, ellipsis_len);
    
    ellipsis_desc.data.str = ellipsis_name;
    ellipsis_desc.length = -1;
    if((ellipsis_name[0] == 0xff) && (ellipsis_name[1] == 0xfe))
    {
        ellipsis_name[ellipsis_len - 2] = 0;
        ellipsis_name[ellipsis_len - 1] = 0;
        ellipsis_desc.language = UNICODEDATA;
        true_bytes = (uint16)(int)ui_ellipsis_to_longstr(&ellipsis_desc, ELLIPSIS_MAX_LEN, NULL);
    }
    else
    {
        ellipsis_name[ellipsis_len - 1] = 0;
        ellipsis_desc.language = ANSIDATAAUTO;
        true_bytes = (uint16)(int)ui_ellipsis_to_longstr(&ellipsis_desc, ELLIPSIS_MAX_LEN, NULL);
    }
    
    return true_bytes;
}

//����ת��Ϊʮ�������ַ���itoa_hex
uint8 itoa_hex(uint32 value, uint8 *tmp_buf)
{
    uint8 zero_flag = TRUE;
    uint8 byte_value, byte_high, byte_low;
    uint8 ret_len, j;
    
    //���ǰ����0x
    ret_len = 0;
    tmp_buf[ret_len] = '0';
    tmp_buf[ret_len+1] = 'x';
    ret_len += 2;
    
    //4���ֽ�ת��Ϊ˫ʮ�������ַ�
    for(j = 0; j < 4; j++)
    {
        byte_value = *((uint8 *)(&value) + (3-j));
        byte_high = (byte_value >> 4);
        byte_low = (byte_value & 0x0f);
        if((zero_flag == TRUE) && (byte_value == 0) && (j < 3))//����ʾǰ��0
        {
            continue;
        }
        
        tmp_buf[ret_len] = (byte_high >= 10)? (byte_high - 10 + 'a'): (byte_high + '0');
        tmp_buf[ret_len+1] = (byte_low >= 10)? (byte_low - 10 + 'a'): (byte_low + '0');
        ret_len += 2;
        zero_flag = FALSE;
    }
    
    return ret_len;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ʾ��ӡ������Ϣ��UI�ϣ�������Ϣ��ʾΪ�ڵװ��֡�
 * \param[in]    x ��ӡ��ϢX����
 * \param[in]    y ��ӡ��ϢY����
 * \param[in]    format ��ӡ��ʽ
 * \param[in]    value ��ӡ������
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    ����1����ӡ��ʽΪ errno = value��value Ϊ 10 ����
    com_dump_debug(0, 40, "errno = %d", fsel_errno);
 * \endcode
 * \note
 * \li  format ��֧��һ�� %x �� %d��ǰ�߱�ʾ16���ƣ����߱�ʾ10���ơ�
 * \li  format �����Ϊ 32 �ֽڣ�����ֻ֧�� ANSI ���롣
*******************************************************************************/
void com_dump_debug(uint16 x, uint16 y, uint8 *format, uint32 value)
{
    string_desc_t debug_str;
    region_t debug_region;
    uint16 format_len, remain_len;
    uint16 ret_len, i;
    uint8  ch_prev, ch_now;
    uint8  value_len;
    
    format_len = (uint16)libc_strlen(format);
    if(format_len > 32)
    {
        format_len = 32;
    }
    
    //ת��Ϊdebug
    ret_len = 0;
    ch_prev = 0x0;
    for(i = 0; i < format_len; i++)
    {
        ch_now = format[i];
        if(ch_prev == '%')
        {
            switch(ch_now)
            {
            case 'x':
            case 'd':
                if(ch_now == 'x')
                {
                    value_len = itoa_hex(value, tmp_buffer + ret_len);
                }
                else
                {
                    value_len = libc_itoa(value, tmp_buffer + ret_len, 0);
                }
                ret_len += value_len;
                                
                remain_len = format_len - (i+1);
                if((remain_len > 0) && ((ret_len + remain_len) <= 52))//for QAM
                {
                    libc_memcpy(tmp_buffer + ret_len, format + (i+1), remain_len);
                    ret_len += remain_len;
                }
                break;
                
            default:
                if(ret_len >= 52)
                {
                    return ;//for QAM
                }
                tmp_buffer[ret_len] = ch_prev;
                ret_len++;
                ch_prev = 0x0;
                break;
            }
            
            if((ch_now == 'x') || (ch_now == 'd'))
            {
                break;//Ҫ��formatΪ��%x��%d�����������˿��˳�
            }
        }
        else if(ch_now == '%')
        {
            ch_prev = ch_now;
            continue;
        }
        else
        {
            ch_prev = 0x0;
        }
        if(ret_len >= 52)
        {
            return ;//for QAM
        }
        tmp_buffer[ret_len] = ch_now;
        ret_len++;
    }
    
    if(ret_len >= 52)
    {
        return ;//for QAM
    }
    tmp_buffer[ret_len] = 0x0;
    
    //��ӡdebug��Ϣ
    debug_str.data.str = tmp_buffer;
    debug_str.language = ANSIDATAAUTO;
    debug_str.length = ret_len;
    
    debug_region.x = x;
    debug_region.y = y;
    debug_region.height = 16;
    debug_region.width = (DISPLAY_LENGTH - x);
    
    ui_set_pen_color(0xffff);
    ui_set_backgd_color(0x0000);
    //��͸��ģʽ�������������ַ���ȡ���������Ҷ�������
    ui_show_string(&debug_str, &debug_region, CHAR_FILL_COLOR | DISP_LONGSTR_NO_RECT | DISP_ARABIC_DISABLE);
}

/*! \endcond */
