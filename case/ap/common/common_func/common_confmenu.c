/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-16 9:47     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_confmenu.c
 * \brief    �����ò˵��ű��ļ������ӿ�ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ����һ�׿����ò˵��ű��ļ������ӿڣ������򿪣��رգ���ȡ�˵�ͷ����ȡ�˵���ȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel sd_filesystem���ȵȡ�
 * \version  1.0
 * \date     2011-9-16
*******************************************************************************/

#include "common_func.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    �򿪿����ò˵��ű��ļ���
 * \param[in]    file_name �����ò˵��ű��ļ�����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ���ļ��ɹ���
 * \retval           FALSE ���ļ�ʧ�ܡ�
 * \ingroup      common_func
 * \note 
 * \li  �ýӿ��ڽ���ǰ̨APʱ���ã�Ȼ�����˳�ǰ̨APʱ�ٵ��� com_close_confmenu_file
 *      �رգ���AP�������������ڣ��ýű����ǿ���ֱ�ӷ��ʵġ�
*******************************************************************************/
bool com_open_confmenu_file(const char* file_name)
{
    uint8 header[16];
    
    confmenu_fp = sys_sd_fopen(file_name);
    if(confmenu_fp == NULL)
    {
        return FALSE;
    }
    
    if(sys_sd_fread(confmenu_fp, header, sizeof(header)) == -1)
    {
        return FALSE;
    }
    confmenu_offset = ((uint16)header[13] << 8) + header[12];   //���ֽ���ǰ
    
    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    �رտ����ò˵��ű��ļ���
 * \param[in]    none
 * \param[out]   none    
 * \return       bool
 * \retval           TRUE �ر��ļ��ɹ���
 * \retval           FALSE �ر��ļ�ʧ�ܡ�
 * \ingroup      common_func
 * \note 
 * \li  �ýӿ�Ӧ�����˳�ǰ̨APʱ���á�
*******************************************************************************/
bool com_close_confmenu_file(void)
{
    if(sys_sd_fclose(confmenu_fp) == -1)
    {
        return FALSE;
    }
    
    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ȡ�˵�ͷ���ݡ�
 * \param[in]    menu_id �˵�ͷID
 * \param[out]   confmenu_title �˵�ͷ�ṹ�����ݻ�����ָ��
 * \return       bool
 * \retval           TRUE ��ȡ�˵�ͷ�ɹ���
 * \retval           FALSE ��ȡ�˵�ͷʧ�ܡ�
 * \ingroup      common_func
 * \note 
*******************************************************************************/
bool com_get_confmenu_title(uint8 menu_id, menu_title_data_t* confmenu_title)
{
    sys_sd_fseek(confmenu_fp, SEEK_SET, confmenu_offset + menu_id * sizeof(menu_title_data_t));
    return (bool)(sys_sd_fread(confmenu_fp, confmenu_title, sizeof(menu_title_data_t)) != -1);
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ȡ�˵������ݡ�
 * \param[in]    confmenu_title �˵�ͷ���ݽṹָ��
 * \param[in]    item_index Ҫ��ȡ�ĵ�ǰ�˵������
 * \param[out]   confmenu_item �˵���ṹ�建����ָ��
 * \return       bool
 * \retval           TRUE ��ȡ�˵���ɹ���
 * \retval           FALSe ��ȡ�˵���ʧ�ܡ�
 * \ingroup      common_func
 * \note 
*******************************************************************************/
bool com_get_confmenu_item(menu_title_data_t* confmenu_title, uint16 item_index, menu_item_data_t *confmenu_item)
{
    sys_sd_fseek(confmenu_fp, SEEK_SET, confmenu_title->offset + sizeof(menu_item_data_t) * item_index);
    return (bool)(sys_sd_fread(confmenu_fp, confmenu_item, sizeof(menu_item_data_t)) != -1);
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ȡ�˵����ڵ�ǰ���˵��ж�Ӧ�������š�
 * \param[in]    confmenu_title �˵�ͷ���ݽṹָ��
 * \param[in]    item_str Ҫ�����Ĳ˵�����ַ���ID
 * \param[out]   none
 * \return       uint8
 * \retval           ���ز˵����ڲ˵��е������ţ�-1��ʾ����ʧ�ܡ�
 * \ingroup      common_func
 * \note 
 * \li  �ýӿ�����ȷ�� RADIO �˵��� select ���ΪRADIO �˵�Ҫ�����˵��� active
 *      ��Ϊ select ��������ò˵�ģʽ�£��˵��������ǿɱ�ģ������������ڳ���
 *      ��ָ��������ֻ�ܼ�ӵ�ָ���˵�����ַ���ID��ͨ���ַ���IDȥƥ����š�
*******************************************************************************/
uint16 com_get_confmenu_active(menu_title_data_t* confmenu_title, uint16 key_str)
{
    menu_item_data_t temp_data;
    uint16 active;
    
    if(key_str == V_U16_INVALID)
    {
        return V_U16_INVALID;
    }
    
    for(active = 0; active < confmenu_title->count; active++)
    {
        sys_sd_fseek(confmenu_fp, SEEK_SET, confmenu_title->offset + sizeof(menu_item_data_t) * active);
        if(sys_sd_fread(confmenu_fp, &temp_data, sizeof(menu_item_data_t)) == -1)
        {
            return V_U16_INVALID;
        }
        
        if(temp_data.str_id == key_str)
        {
            return active;
        }
    }
    
    return V_U16_INVALID;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ȡ�˵����ڵ�ǰ���˵��ж�Ӧ�������š�
 * \param[in]    confmenu_title �˵�ͷ���ݽṹָ��
 * \param[in]    item_str Ҫ�����Ĳ˵�����ַ���ID
 * \param[out]   none
 * \return       uint8
 * \retval           ���ز˵����ڲ˵��е������ţ�-1��ʾ����ʧ�ܡ�
 * \ingroup      common_func
 * \note 
 * \li  �ýӿ�����ȷ�� RADIO �˵��� select ���ΪRADIO �˵�Ҫ�����˵��� active
 *      ��Ϊ select ��������ò˵�ģʽ�£��˵��������ǿɱ�ģ������������ڳ���
 *      ��ָ��������ֻ�ܼ�ӵ�ָ���˵�����ַ���ID��ͨ���ַ���IDȥƥ����š�
*******************************************************************************/
uint16 com_get_menu_active(uint8 menu_id, uint16 key_str)
{
    menu_title_data_t temp_title;
    menu_item_data_t temp_data;
    uint16 active;
    
    com_get_confmenu_title(menu_id, &temp_title);
    
    if(key_str == V_U16_INVALID)
    {
        return V_U16_INVALID;
    }
    
    for(active = 0; active < temp_title.count; active++)
    {
        sys_sd_fseek(confmenu_fp, SEEK_SET, temp_title.offset + sizeof(menu_item_data_t) * active);
        if(sys_sd_fread(confmenu_fp, &temp_data, sizeof(menu_item_data_t)) == -1)
        {
            return V_U16_INVALID;
        }
        
        if(temp_data.str_id == key_str)
        {
            return active;
        }
    }
    
    return V_U16_INVALID;
}

/*! \endcond */
