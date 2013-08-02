/*******************************************************************************
 *                              us212A
 *                            Module: manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-10-8 11:21     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_config.c
 * \brief    Ӧ�����ýű������ӿ�ʵ��
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ����һ�׼򵥵�Ӧ�����ýӿڣ�������ȡ������ȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               ֱ�ӵ���sd �ļ�ϵͳ�ӿڽ��ж�λ�Ͷ�ȡ��
 * \version 1.0
 * \date  2011-10-8
 *******************************************************************************/

#include "common_func.h"
// config �ű��ļ�����
const char cfg_file[] = "config.bin";
/******************************************************************************/
/*!
 * \par  Description:
 *	  ��Ӧ�����ýű��ļ�������ȡ���������ļ�ͷ
 * \param[in]    config_file��Ӧ�����ýű��ļ�����
 * \param[out]   none
 * \return       bool
 * \retval           ���ش򿪳ɹ����true��ʾ�ɹ���false��ʾʧ��
 * \note
 *******************************************************************************/
bool open_config_file(void)
{
    uint8 head[16];

    config_fp = sys_sd_fopen(cfg_file);
    if (config_fp == NULL)
    {
        return FALSE;
    }

    if (sys_sd_fread(config_fp, head, 16) == -1)
    {
        return FALSE;
    }

    if (libc_memcmp(head, "cfg", 3) != 0)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ȡӦ�����ýű��ļ������ò���Ĭ��ֵ
 * \param[in]    config_id�����ò���ID
 * \param[out]   none
 * \return       uint16
 * \retval           ���ض�ȡ���Ĳ���Ĭ��ֵ
 * \note         ��������ֵ�Ͳ����������û���֤ config_id ��ȷ
 *******************************************************************************/
uint16 get_config_default(uint16 config_id)
{
    config_index_t temp_index;
    uint16 default_value;

    //��ȡ�����������ṹ��
    if (sys_sd_fseek(config_fp, SEEK_SET, 16 + config_id * sizeof(config_index_t)) == -1)
    {
        return 0;
    }
    if (sys_sd_fread(config_fp, &temp_index, sizeof(config_index_t)) == -1)
    {
        return 0;
    }

    //��ȡ����������
    if (sys_sd_fseek(config_fp, SEEK_SET, temp_index.offset) == -1)
    {
        return 0;
    }
    if (sys_sd_fread(config_fp, &default_value, sizeof(uint16)) == -1)
    {
        return 0;
    }

    return default_value;
}
