/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    ������д�ļ��ĸ���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "plist.h"
#include "rdata_ex.h"

bool unicode_to_char(char *str, uint16 len, uint8 encode);
/******************************************************************************/
/*
 * \par  Description: ����ļ���ID3����
 *
 * \param[in]    ext_name--��չ��

 * \param[out]

 * \return      ID3������

 * \note
 *******************************************************************************/
id3_type_e check_id3_type(uint32 ext_name)
{
    id3_type_e id3_type;
    switch (ext_name)
    {
        case MP3_VAL:
        case MP1_VAL:
        case MP2_VAL:
        {
            id3_type = ID3_TYPE_MP3;
            break;
        }
        case WMA_VAL:
        case ASF_VAL:
        case WMV_VAL:
        {
            id3_type = ID3_TYPE_WMA;
            break;
        }
        case OGG_VAL:
        {
            id3_type = ID3_TYPE_OGG;
            break;
        }
        case APE_VAL:
        {
            id3_type = ID3_TYPE_APE;
            break;
        }
        case FLAC_VAL:
        {
            id3_type = ID3_TYPE_FLAC;
            break;
        }
        case AA_VAL:
        {
            id3_type = ID3_TYPE_AA;
            break;
        }
        case AAX_VAL:
        case M4A_VAL:
        {
            id3_type = ID3_TYPE_AAX;
            break;
        }
        case AAC_VAL:
        {
            id3_type = ID3_TYPE_AAC;
            break;
        }

        default:
        {
            id3_type = ID3_TYPE_END;
            break;
        }
    }

    return id3_type;
}

/******************************************************************************/
/*
 * \par  Description: unicodeת���ɶ�Ӧ������
 *
 * \param[in]    str--unicode�ַ�
 len--����
 encode--��������
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool unicode_to_char(char *str, uint16 len, uint8 encode)
{
    ui_unicode_to_char(str, (uint32) len, (uint32) encode);
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: unicodeת���ɶ�Ӧ������
 *
 * \param[in]    str--unicode�ַ�
 len--����
 encode--��������
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool char_to_unicode(uint8 *dest, uint8 *src, uint16 len)
{
    ui_char_to_unicode(dest, src, len);
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: utf8 ת���ɶ�Ӧ������
 *
 * \param[in]    str--unicode�ַ�
 len--����
 encode--��������

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool utf8_to_char(char *str, uint16 len, uint8 encode)
{
    int16 unicode_len;

    ui_utf8_to_unicode(str, temp_plist_buf, &unicode_len);
    ui_unicode_to_char(temp_plist_buf, (uint32) unicode_len, (uint32) encode);
    libc_memcpy(str, temp_plist_buf, (uint32) len);

    return TRUE;
}
/******************************************************************************/
/*
 * \par  Description:   �����ַ���������UNICODE��UTF8��ת���ɶ�Ӧ������
 *
 * \param[in]    dest--Ŀ���ַ���
 source--Դ�ַ���
 land_id--����id
 size--�ַ�����

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
//������Ϣ�ַ���
void copy_deal_string(char *dest, char * source, uint8 land_id, uint8 size)
{
    if (*((uint16 *) (source)) == UNICODE_FLAG)
    {
        //����UNICODE��ת�����ٴ洢
        libc_memcpy(temp_plist_buf, source + 2, size * 2);
        //ת��������
        unicode_to_char(temp_plist_buf, (uint16) (size * 2), land_id);
        libc_memcpy(dest, temp_plist_buf, (uint32) size);
    }
    else if (((uint8) source[0] == UTF8_FLAG_0) && ((uint8) source[1] == UTF8_FLAG_1) && ((uint8) source[2]
            == UTF8_FLAG_2))
    {
        //����UTF8��ת�����ٴ洢
        libc_memcpy(temp_plist_buf, source + 3, size * 2);
        //utf8ת��������
        utf8_to_char(temp_plist_buf, (uint16) (size * 3), land_id);
        libc_memcpy(dest, temp_plist_buf, (uint32) size);
    }
    else
    {
        libc_memcpy(dest, source, (uint32) size);
    }
}

