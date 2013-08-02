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
 * \file     ui_copy_arabic.c
 * \brief    arabicת��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *              ʵ��Arabic�ַ���copy
 * \par      EXTERNALIZED FUNCTIONS:
 *              none
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond */ 
/******************************************************************************/
/*!
 * \par  Description:
 *    �������ַ���copy�����б���ת������д����ȡ�
 * \param[in]   desc��Դ�ַ���������ָ��
 * \param[out]  desc��ת�����ַ���������ָ��
 * \return      none
 * \note        desc->argv ��ʾ��������С��������תUnicode��ʱʹ�á�
 *******************************************************************************/
void arabic_uni_join(string_desc_t *desc)
{
    //����ת��ΪUnicode�룬�Ա���а�������д����
    if (desc->language != UNICODEDATA)
    {
        arabic_char_to_unicode(desc);
        desc->language = UNICODEDATA;
    }
    
    //��������д����
    if (language_id == ARABIC)
    {
        arabic_do_join(desc);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ������������ת��Ϊunicode
 * \param[in]   desc��Դ�ַ���������ָ��
 * \param[out]  desc��ת�����ַ���������ָ��
 * \return      ���ؽ���ַ�����Ч�ֽ��������洢�� desc->length �С�
 * \note        desc->argv ��ʾ��������С��ʹ��arabic_string_buf ����ת��
 *******************************************************************************/
uint16 arabic_char_to_unicode(string_desc_t *desc)
{
    uint8 *scr, *dest;
    uint16 length = desc->length;
    uint16 real_len;
    uint16 temp_code;
    uint16 i;

    //��ͳ����Ч�ַ���
    scr = desc->data.str;
    real_len = 0;
    for (i = 0; i < length; i++)
    {
        if (*scr == 0x0)
        {
            break;
        }
        real_len++;
        scr++;
    }

    //����ַ�����������ת������
    if ((real_len * 2) > (desc->argv - 2))
    {
        real_len = (desc->argv - 2) / 2;
    }

    //Unicode����ת��
    char_to_unicode(arabic_string_buf, desc->data.str, real_len);
    libc_memcpy(desc->data.str, arabic_string_buf, real_len * 2 + 2);
    desc->length = real_len * 2;
    return real_len * 2;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    arabic_ascii_turn
 * \param[in]   desc���ַ���������ָ��
 * \param[out]  none
 * \return      none
 * \note        ֧�������Unicodeת��
 *******************************************************************************/
void arabic_ascii_turn(string_desc_t *desc)
{
    uint8 *str = desc->data.str;
    uint16 length = desc->length;

    uint16 Glyph;
    uint16 Glyph_prev = 0;
    uint8 start, end;
    uint8 i = 0;
    uint8 char_count;
    //uint8 char_is_arabic = 0;

    while (1)
    {
        if (i >= length)//�ַ���ת�����
        {
            break;
        }

        Glyph = ((uint16)str[i + 1]  << 8) + str[i];
        if (Glyph == 0x0)//���������ַ���ת�����
        {
            break;
        }

        char_count = 0;
        start = i;
        while ((i <= length) && (Glyph < 0x80) //ASCII��
            && (((Glyph >= 0x41) && (Glyph <= 0x5a)) //��д��ĸ
             || ((Glyph >= 0x61) && (Glyph <= 0x7a)) //Сд��ĸ
             || ((Glyph >= 0x30) && (Glyph <= 0x39)) //����
             || (Glyph == 0x27) || (Glyph == 0x2d) || (Glyph == 0x2e) || (Glyph == 0x20)))//���������ַ�
        {
            char_count++;
            Glyph_prev = Glyph;
            //��ȡ��һ���ַ�
            i += 2;//���ܻ����length������whileѭ�������б�����
            Glyph = ((uint16)str[i + 1]  << 8) + str[i];
        }
        
        if (char_count > 0)
        {
            i -= 2;
        }
        end = i;

        if ((Glyph_prev == 0x20) || (Glyph_prev == 0x27) || (Glyph_prev == 0x2d) || (Glyph_prev == 0x2e))
        {
            char_count--;
            end -= 2;
        }
        //���ַ���������Ҫ�û��ַ�˳��
        if (char_count > 1)
        {
            arabic_do_turn(str, start, end + 2);//end�Խ���λ�ô���
        }
        i += 2;//���¸��ַ�����ɨ��
    }
}

void arabic_do_turn(uint8 *str, uint16 start, uint16 end)
{
    uint8 temp_byte;

    if (end > 0)
    {
        end -= 2;//ָ�����һ����Ч�ַ�
    }
    while (end > start)
    {
        temp_byte = str[end];
        str[end] = str[start];
        str[start] = temp_byte;
        temp_byte = str[end + 1];
        str[end + 1] = str[start + 1];
        str[start + 1] = temp_byte;

        end -= 2;
        start += 2;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    is_arabic_hebrew_string
 * \param[in]   desc���ַ���������ָ��
 * \param[out]  none
 * \return      uint8 1��ʾ�������2��ʾϣ�����0��ʾ��������
 * \note        ֧�������Unicodeת��
 *******************************************************************************/
uint8 is_arabic_hebrew_string(string_desc_t *desc)
{
    uint8 *str = desc->data.str;
    uint16 length = desc->length;

    uint16 Glyph;
    uint8 i = 0;

    while (1)
    {
        if (i >= length)//�ַ���ת�����
        {
            break;
        }

        Glyph = ((uint16)str[i + 1]  << 8) + str[i];
        if (Glyph == 0x0)
        {
            break;
        }
        if (is_arabic_char(Glyph) == TRUE)
        {
            return 1;
        }
        if (is_hebrew_char(Glyph) == TRUE)
        {
            return 2;
        }
        
        i = i + 2;
    }
    return 0;
}

/*! \endcond */
