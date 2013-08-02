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
 * \file     ui_char_to_unicode.c
 * \brief    ����ת����unicode
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ������ת����unicode
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���SD�ļ�ϵͳ�ӿں���sys_sd_fread��sys_sd_fseek
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    �����ַ���תunicode�ַ���
 * \param[in]   src��Դ�ַ���ָ��
 * \param[in]   len��Ҫת���������ַ�������
 * \param[out]  dest��Ŀ���ַ���ָ��
 * \return      �Ƿ�ת���ɹ�
 * \retval      TRUE ת���ɹ�
 * \retval      FALSE ת��ʧ��
 * \ingroup     string
 * \note        Ŀ���ַ������������û���֤�����������������С��Ҫ���Ͻ�������
 *******************************************************************************/
bool char_to_unicode(uint8 *dest, uint8 *src, uint16 len)
{
    int s_cnt = 0;
    int d_cnt = 0;
    uint16 font_code;

    while (1)
    {
        if ((s_cnt >= len) || (src[s_cnt] == 0))//��β���߽�����
        {
            break;
        }
        
        if (((uint8) src[s_cnt] >= 0x80) && (language_2byte == TRUE))
        {
            font_code = src[s_cnt + 1];
            font_code = font_code << 8;
            font_code |= src[s_cnt];
            s_cnt += 2;
        }
        else
        {
            font_code = src[s_cnt];
            s_cnt++;
        }
        //ת��ΪUnicode���룬С��0x80����������ת��
        if (font_code >= 0x80)
        {
            font_code = romf_multi_char_to_unicode(font_code);
        }
        
        dest[d_cnt] = *((uint8*) &font_code); //���ֽ�
        d_cnt++;
        dest[d_cnt] = *((uint8*) &font_code + 1); //���ֽ�
        d_cnt++;
    }
    dest[d_cnt] = 0;
    d_cnt++;
    dest[d_cnt] = 0;
    return TRUE;
}
/*! \endcond */
