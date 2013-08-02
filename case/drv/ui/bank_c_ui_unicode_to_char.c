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
 * \file     ui_unicode_to_char.c
 * \brief    unicodeת��������
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ��unicode�ַ���ת���������ַ���
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���SD�ļ�ϵͳ�ӿں���sys_sd_fread��sys_sd_fseek
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

const char UniToMBTab[][12] =
{
    "FTBL_GB.$$$", /*��������codepage table*/
    "FTBL_B5.$$$", /*��������codepage table*/
    "1252L.TBL", /*Ӣ��,��ŷ��ϵ codepage table*/
    "FTBL_JP.$$$", /*���� codepage table*/
    "FTBL_KR.$$$", /*���� codepage table*/
    "1251L.TBL", /*������ϵ*/
    "1254L.TBL", /*��������ϵ*/
    "1255L.TBL", /*ϣ������*/
    "874L.TBL", /*̩��*/
    "1250L.TBL", /*��ŷ����ŷ��ϵ*/
    "1256L.TBL", /*��������ϵ*/
    "1253L.TBL" /*ϣ����*/
};

bool switch_to_char(uint8 *str, int len, sd_handle fp, uint8 latin_flag);

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    �����������࣬��unicode��Ӧ�Ķ������ת�������ʵ��unicode�������ת��
 * \param[in]    str���ַ�����ַ
 * \param[in]    len��Ҫת����unicode�ַ�������
 * \param[in]    lang_id����������
 * \param[out]   str��ת������ַ���ָ��
 * \return       �Ƿ�ת���ɹ�
 * \retval       TRUE ת���ɹ�
 * \retval       FALSE ��ת����ʧ��
 * \ingroup      string
 * \note
 *******************************************************************************/
bool unicode_to_char(uint8 *str, uint16 len, uint8 lang_id)
{
    sd_handle UnitoMBTab_fp;
    uint8 result;
    uint8 table_index = 0;
    uint8 latin_flag = 1; /*������ϵ��־��*/
    switch (lang_id)
    {
        case CHINESE_SIMPLIFIED:
        table_index = 0;
        latin_flag = 0;
        break;

        case CHINESE_TRADITIONAL:
        table_index = 1;
        latin_flag = 0;
        break;

        case ENGLISH:
        case FRENCH:
        case GERMAN:
        case ITALIAN:
        case DUTCH:
        case NORWEGIAN:
        case PORTUGUESE_EUROPEAN:
        case PORTUGUESE_BRAZILIAN:
        case SPANISH:
        case SWEDISH:
        case FINNISH:
        case INDONESIAN:
        table_index = 2;
        break;

        case JAPANESE:
        table_index = 3;
        latin_flag = 0;
        break;

        case KOREAN:
        table_index = 4;
        break;

        /*case CZECH:
         case POLISH:
         table_index = 5;
         break;*/

        case RUSSIAN:
        table_index = 5;
        break;

        case TURKISH:
        table_index = 6;
        break;

        case HEBREW:
        table_index = 7;
        break;

        case THAI:
        table_index = 8;
        latin_flag = 0;
        break;

        case CZECH:
        case POLISH:
        case DANISH:
        case HUNGARIAN:
        case SLOVAK:
        case ROMANIAN:
        table_index = 9;
        break;

        case ARABIC:
        table_index = 10;
        break;

        case GREEK:
        table_index = 11;
        break;

        default:
        table_index = 0;
        break;
    }
    /*��unicode to char ת����*/
    UnitoMBTab_fp = sys_sd_fopen(UniToMBTab[table_index]);
    if (UnitoMBTab_fp == NULL)
    {
        return FALSE;
    }
    result = switch_to_char(str, len, UnitoMBTab_fp, latin_flag);
    sys_sd_fclose(UnitoMBTab_fp);
    return TRUE;
}
/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    unicode�ַ���ת�����ַ���
 * \param[in]   str���ַ�����ַ
 * \param[in]   len��Ҫת����unicode�ַ�������
 * \param[in]   fp��unicode�������ת����ľ��
 * \param[in]   latin_flag���Ƿ�������ϵ�ı�־
 * \param[out]   str��ת������ַ���ָ��
 * \return      �Ƿ�ת���ɹ�
 * \note
 *******************************************************************************/
bool switch_to_char(uint8 *str, int len, sd_handle fp, uint8 latin_flag)
{
    int s_cnt;
    int d_cnt;
    uint16 font_code;

    for (d_cnt = 0, s_cnt = 0; (s_cnt < len) && ((str[s_cnt] != 0) || (str[s_cnt + 1] != 0));s_cnt += 2)
    {
        font_code = str[s_cnt + 1];
        font_code = font_code << 8;
        font_code |= str[s_cnt];
        //��ASCII���ת��
        if (font_code > 127)
        {
            //��ת������Ŀ���ַ����������ʾ
            sys_sd_fseek(fp, SEEK_SET, (uint32) (font_code << 1));
            sys_sd_fread(fp, &font_code, 2);

            str[d_cnt] = *((uint8*) &font_code); //��λ      Z80��С�� ��51�Ǵ��
            d_cnt++;
            //��������ϵ�������������ַ�����ʾ
            if ((latin_flag == 0) && (font_code > 0x7f)) //���ֽ������ת��
            {
                str[d_cnt] = *((uint8*) &font_code + 1); //��λ
                d_cnt++;
            }
        }
        else
        {
            str[d_cnt] = str[s_cnt]; //ֻȡ��λ,��õ��Ƕ�Ӧ�ַ���������
            d_cnt++;
        }
    }
    str[d_cnt] = 0;

    return TRUE;
}
/*! \endcond */

