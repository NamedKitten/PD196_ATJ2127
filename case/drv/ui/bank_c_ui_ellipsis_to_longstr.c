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
const uint8 ellipsis_str[4] = "...";
const uint8 ellipsis_str_uni[6] = { '.', 0, '.', 0, '.', 0 };

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ���ַ�������Ϊ"����..."��ʽ��
 * \param[in]    desc �ַ���������
 * \param[in]    max_len �ַ�����ʾ��󳤶�
 * \param[out]   getlen_result �������ָ�룬����Ϊ��
 * \param[out]   desc ����������ַ���
 * \return       uint16 ���������ַ�������ֽڳ���
 * \ingroup      string
 * \note
 *******************************************************************************/
uint16 ellipsis_to_longstr(string_desc_t *desc, uint16 max_len, str_getlen_result_t *getlen_result)
{
    string_desc_t tmp_desc;
    str_getlen_result_t tmp_result;
    uint16 ellipsis_len, longstr_len, tmp_len;
    uint16 true_bytes, true_length;
    
    uint8 *longstr = desc->data.str;
    uint16 length = desc->length;
    int8 language = desc->language;

    tmp_desc.data.str = ellipsis_str;
    tmp_desc.length = 3;
    tmp_desc.language = ANSIDATAAUTO;
#if (USE_MULTI_FONTLIB == 0)
    ellipsis_len = romf_get_string_length(&tmp_desc, 128, NULL);
#else
    ellipsis_len = fixf_get_string_length(&tmp_desc, 128, NULL);
#endif

    longstr_len = max_len - ellipsis_len;
    
    if(language == UNICODEDATA)
    {
        language = UNICODELANGUAGE;
    }
    tmp_desc.data.str = longstr;
    tmp_desc.language = language;
    tmp_desc.length = length;

    //��ȡ�����֡��ַ���
#if (USE_MULTI_FONTLIB == 0) 
    tmp_len = romf_get_string_length(&tmp_desc, longstr_len, &tmp_result);
#else
    tmp_len = fixf_get_string_length(&tmp_desc, longstr_len, &tmp_result);
#endif
    true_bytes = tmp_result.true_bytes;
    true_length = tmp_result.true_length;
    if (tmp_len > longstr_len)
    {
        //�ܳ��ȴ���max_len����Ҫ����Ϊ������...����ʽ
#if (USE_MULTI_FONTLIB == 0)
        tmp_len = romf_get_string_length(&tmp_desc, max_len, NULL);
#else
        tmp_len = fixf_get_string_length(&tmp_desc, max_len, NULL);
#endif
        if (tmp_len > max_len)
        {
            if (language == UNICODELANGUAGE)
            {
                libc_memcpy(longstr + true_bytes, ellipsis_str_uni, 6);
                true_bytes += 6;
                longstr[true_bytes] = 0;
                longstr[true_bytes + 1] = 0;
            }
            else
            {
                libc_memcpy(longstr + true_bytes, ellipsis_str, 3);
                true_bytes += 3;
                longstr[true_bytes] = 0;
            }
            true_length += ellipsis_len;
        }
    }
    
    if(getlen_result != NULL)
    {
        getlen_result->true_bytes = true_bytes;
        getlen_result->true_length = true_length;
    }

    return true_bytes;
}
/*! \endcond */
