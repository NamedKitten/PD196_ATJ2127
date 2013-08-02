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
 * \file     ui_thai_point.c
 * \brief    ��ȡ̩���ַ����ĵ���
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ��̩���ַ����Ĵ�����ȡ����
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ��û�ȡ����Ľӿں���get_font_point
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ̩���ַ����ĵ���
 * \param[in]   str ��ǰ�ַ�ָ��,�Է��ʽ��Ӻ�����ַ�
 * \param[in]   language ��ǰ�ַ�����������
 * \param[in]   source_code ��ǰ�ַ��ı���ֵ
 * \param[in]   pt_step ������һ���ַ�ƫ��
 * \param[out]  char_buf ���洦���ĵ������ݵ�bufָ��
 * \param[out]  pt_step ���ص�ǰ�ص��ַ����ֽ���
 * \return      ���ص�ǰ�ص��ַ����ֽ���
 * \note
 *******************************************************************************/
uint8 thai_point(uint8 * str, int8 language, uint16 source_code, uint8* char_buf, uint8 *pt_step)
{
    uint16 thai_code = 0;
    uint8 width1, width2, i, j;
    uint8 char_bytes;
#ifndef PC
    uint8 *thai_point_buf = 0;
    if(sys_malloc(&thai_point_buf, MAX_CHARPOINTE_SIZE) == -1)//����ռ�ʧ��
    {
#ifdef UI_DEBUG
        bool loop = TRUE;
        while (1)//������ѭ��
        {
            loop = TRUE;
        }
#endif
    }
#else
    uint8 thai_point_buf[MAX_CHARPOINTE_SIZE];
#endif
    
#if (USE_MULTI_FONTLIB == 0)
    romf_get_font_point(source_code, fix_p_char_point); //ȡ����
#else
    fixf_get_font_point(source_code, fix_p_char_point); //ȡ����
#endif
    char_bytes = *pt_step;

    while (1)
    {
        str = str + char_bytes;//ָ����һ���ַ�
        if (language == UTF_8DATA)
        {
            if (*str < 0x80)//0h0#######
            {
                thai_code = *str;
                char_bytes = 1;
            }
            else if (*str < 0xe0) //0h110#####
            {
                thai_code = ((uint16) (*str) & 0x1f) << 6;
                thai_code += ((uint16) (*(str + 1)) & 0x3f);
                char_bytes = 2;
            }
            else if (*str < 0xf0) //0h1110####
            {
                thai_code = ((uint16) (*str) & 0xf) << 12;
                thai_code += ((uint16) (*(str + 1)) & 0x3f) << 6;
                thai_code += ((uint16) (*(str + 2)) & 0x3f);
                char_bytes = 3;
            }
            else
            {
                //do nothing for QAC
            }
        }
        else if (language == UNICODELANGUAGE)
        {
            thai_code = *str + ((uint16) (*(str + 1)) << 8);
            char_bytes = 2;
        }
        else
        {
            thai_code = *str;
            char_bytes = 1;
        }
        if (thai_code == 0x00)
        {
            break;
        }

        //ת��ΪUnicode���룬Unicode���Ժ�С��0x80����������ת��
        if (((language != UNICODELANGUAGE) && (language != UTF_8DATA)) && (thai_code >= ASCIIDISCODE))
        {
            thai_code = romf_multi_char_to_unicode(thai_code);
        }
        if ((thai_code == 0x0E31) || ((thai_code <= 0x0E3A) && (thai_code >= 0x0E34)) 
        || ((thai_code <= 0x0E4E) && (thai_code >= 0x0E47)))
        {
            *pt_step = *pt_step + char_bytes;
            width1 = fix_p_char_point[fix_font_size - 1];
            if (width1 <= (fix_font_width/2))
            {
                width1 = fix_font_width/2;
            }
            else
            {
                width1 = fix_font_width;
            }
#if (USE_MULTI_FONTLIB == 0)
            romf_get_font_point(thai_code, thai_point_buf);
#else
            fixf_get_font_point(thai_code, thai_point_buf);
#endif
            width2 = thai_point_buf[fix_font_size - 1];
            if (width2 <= (fix_font_width/2))
            {
                width2 = fix_font_width/2;
            }
            else
            {
                width2 = fix_font_width;
            }
            if (width1 > width2)
            {
                for (i = 0, j = 0; i < thai_point_buf[fix_font_size - 1]; i++)
                {
                    fix_p_char_point[j] |= thai_point_buf[i];
                    if (((j + width1) < fix_font_size) && ((i + width2) < fix_font_size))//for QAC & QAM
                    {
                        fix_p_char_point[j + width1] |= thai_point_buf[i + width2];
                    }
                    j += 2;
                }
            }
            else
            {
                for (i = 0; i < fix_p_char_point[fix_font_size - 1]; i++)
                {
                    thai_point_buf[i] |= fix_p_char_point[i];
                    thai_point_buf[i + width2] |= fix_p_char_point[i + width1];
                }
                libc_memcpy(fix_p_char_point, thai_point_buf, fix_font_size);
            }
        }
        else
        {
            break;
        }
    }

    // ����
    libc_memcpy(char_buf, fix_p_char_point, fix_font_size);
    
#ifndef PC
    sys_free(&thai_point_buf);
#endif
    return *pt_step;
}
/*! \endcond */
