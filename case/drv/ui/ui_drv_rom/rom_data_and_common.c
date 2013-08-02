/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       cailizhen     2012-1-12 9:42     1.0             build this file
 *******************************************************************************/
/*!
 * \file     _rom_data_and_common.c
 * \brief    ROM����ģ��������ܺ��������ݣ�С������
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2012-1-12
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//��Դ��Ϣ�ṹ��
res_open_infor_t g_ap_res;
res_open_infor_t g_com_res;
//�ֿ��ļ����
sd_handle FontLib_fp;
//�������ת������
sd_handle MBtoUniTab_fp;

//dc���
DC dc;
//��ǰUIԪ����ʾ����
region_t region;
//��ʾ������
region_t region_win;

#ifndef PC
uint32 display_buf_addr; //��ʾbuffer�ĵ�ַ
uint32 font_cache_addr; //��ģ��������ַ
#endif
uint16 display_buf_len; //��ʾbuffer�Ĵ�С
uint16 font_cache_len; //��ģ��������С��Ĭ��֧��1KB
uint16 font_cache_header;//��ģ������ͷ����С������4�ֽڶ��룬Ĭ��Ϊ64B
bool font_cache_flag;//ʹ����ģ��������־

uint8 char_point_buffer[33]; //��ģ���� buffer

//������ROM���õĺ�����Ӧ�ĺ���ָ�����
__char_fill_hook char_fill_hook;
__dma_buffer_data_hook dma_buffer_data_hook;
__read_str_bgpic read_str_bgpic;
__read_str_bgpic_hook read_str_bgpic_hook;
__get_next_char_point_hook get_next_char_point_hook;
__next_char_ansi_hook next_char_ansi_hook;
__next_char_unicode_hook next_char_unicode_hook;
__set_lcd_param_hook set_lcd_param_hook;

//ROM�ӿڣ�UI�����ڵ��ⲿ�ӿڣ�
__put_sd_image put_sd_image;
__black_char_to_color black_char_to_color;
__char_fill char_fill;
__get_next_char_type_unicode get_next_char_type_unicode;
__get_next_char_type_ansi get_next_char_type_ansi;
__search_cache search_cache;
__put_char put_char;

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ������������ģ�������б����
 * \param[in]    uni_code��Unicode���룬ֻ֧��Unicode�ֿ⻺��
 * \param[out]   cache_index�������б����
 * \return       ������ģ�Ƿ��ѻ���
 * \note         ˵��������Ĭ�� 1KB����������˵����
 *               1������ռ��ܹ� 1KB��ǰ64�ֽ����ڹ�����960�ֽ����ڻ�����ģ���ɻ���29����ͬ�����ַ���
 *               2��ͷ4�ֽڱ�ʾ��־��"font" = 102, 111, 110, 116 = 0x746E6F66�����û�иñ�־�����ʼ����
 *               3����5�ֽ����ڿ����滻��ģ��ţ���������ͻ���Ҳ�����ģ������ģ��������ʱ�����յ�5�ֽ�
 *                  �ı��루0-28�����ǻ��沢�����б�Ȼ���5�ֽ�ѭ����������6�ֽڲ�����Ч��
 *               4����ģ��Unicode�����ţ������ŵ� 58B ��ŵ�ǰ��������ģ�б����ڲ�ѯ��
 *               5������font_cache_addr��font_cache_len��font_cache_header �ȱ��������ܻ�Ӱ�쵽font��
 *                  ��������Ҫ�� FONT_CACHE_MAGIC �������
 *               6���û��Լ���֤ʵ�ʿ�����ģ��������С������ max_char �ַ���ģ���� 960 >= max_char*33��
 *******************************************************************************/
bool romf_search_cache(uint16 uni_code, uint8 *cache_index)
{
#ifndef PC
    uint8 *font_cache_buffer = (uint8 *) font_cache_addr;
#else
    uint8 *font_cache_buffer = GET_REAL_ADDR(font_cache_addr);
#endif
    uint16 *table = (uint16 *)(font_cache_buffer + 6);//�뱣֤ font_cache_buffer half_word ����
    uint8 max_char;
    uint8 i;

    //����֧���ַ���ģ��������ȥ��ͷ6�ֽڣ�ÿ2�ֽڱ���һ���ַ�����
    max_char = (uint8)((font_cache_header - 6) / 2);

    //����Ƿ���Ч������
    if (*((uint32 *) font_cache_buffer) != FONT_CACHE_MAGIC)//"font" = 0x746E6F66
    {
        for (i = 0; i < max_char; i++)
        {
            table[i] = 0xfeff;
        }
        font_cache_buffer[FONT_CYCLE_INDEX] = 0;
        *cache_index = 0;
        table[0] = uni_code;//������б�ID����ģ�������ⲿ���
        *((uint32 *) font_cache_buffer) = FONT_CACHE_MAGIC;//"font"

        return FALSE;
    }

    for (i = 0; i < max_char; i++)
    {
        //�ҵ���ģ���ɹ�����
        if (table[i] == uni_code)
        {
            *cache_index = i;
            return TRUE;
        }
        //�ҵ���Ч��ģ����ʾû�л��棬����ʧ��
        if (table[i] == 0xfeff)
        {
            *cache_index = i;
            table[i] = uni_code;//������б�ID����ģ�������ⲿ���
            return FALSE;
        }
    }

    //��������ȡ�滻���
    *cache_index = font_cache_buffer[FONT_CYCLE_INDEX];
    table[*cache_index] = uni_code;//������б�ID����ģ�������ⲿ���
    //ѭ�������滻���
    font_cache_buffer[FONT_CYCLE_INDEX]++;
    if (font_cache_buffer[FONT_CYCLE_INDEX] >= max_char)
    {
        font_cache_buffer[FONT_CYCLE_INDEX] = 0;
    }
    return FALSE;
}
/*! \endcond */

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡdc��Ϣ
 * \param[in]    none
 * \param[out]   pdc������DC��Ϣ�Ľṹ��ָ��
 * \return       none
 * \ingroup      settings
 * \note
 *******************************************************************************/
void romf_get_DC_status(PDC pdc, void*null2, void *null3)
{
    pdc->pen_color = dc.pen_color;
    pdc->backgd_color = dc.backgd_color;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���û�����ɫ
 * \param[in]    color��Ҫ���õĻ�����ɫֵ
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \note
 *******************************************************************************/
void romf_set_pen_color(COLORREF color, void *null2, void *null3)
{
    dc.pen_color = color;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���ñ�����ɫ
 * \param[in]    color��Ҫ���õı�����ɫֵ
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \note
 *******************************************************************************/
void romf_set_backgd_color(COLORREF color, void*null2, void *null3)
{
    dc.backgd_color = color;
}
/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��ǰ��Ļ����
 * \param[in]    none
 * \param[out]   x����ȡ��X����
 * \param[out]   y����ȡ��Y����
 * \return       none
 * \note
 *******************************************************************************/
void romf_get_text_pos(uint16 *x, uint16 *y)
{
    *x = region.x;
    *y = region.y;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �����ַ������ǰ����
 * \param[in]    x��Ҫ���õ�X����
 * \param[in]    y��Ҫ���õ�Y����
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void romf_set_text_pos(uint16 x, uint16 y)
{
    region.x = x;
    region.y = y;
}
/*! \endcond */

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��������ַ�����unicode����
 * \param[in]    id����������ַ���ID
 * \param[in]    type����Դ����
 * \param[in]    string_code_infor��ָ���������ַ����ṹ�����
 * \return       none
 * \ingroup      string
 * \note         desc->result �� desc->length ��������Ч�ֽ�����
 *******************************************************************************/
void romf_get_unicode_data(uint16 id, string_desc_t *desc)
{
    res_langid_entry_t res_langid_entry;
    res_open_infor_t *res_ptr;
    uint16 real_len;

    /*���ݴ���Դ�ļ������ͣ���ָ��ָ��ͬ��ȫ�ֱ���*/
    res_ptr = res_ptr_select(id);
    // ָ��ָ���ַ����ļ��е�λ��
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->string_entry_offset
            + (uint32) (REAL_RES_ID( id)) *sizeof(res_langid_entry_t));
    sys_sd_fread(res_ptr->res_fp, &res_langid_entry, sizeof(res_langid_entry_t)); //res_fp->&res_entry
    /*��ȡ�ַ������ݣ�unicode����*/
    //ע�⣺res_langid_entry.length �Ѿ������˽��������ڡ�
    if (res_langid_entry.length > desc->length)
    {
        //ע�⣺desc->length ��Ϊż��
        desc->result = desc->length = real_len = desc->length - 2;
        //�Լ���ӽ�����
        desc->data.str[desc->length] = 0;
        desc->data.str[desc->length + 1] = 0;
    }
    else
    {
        real_len = res_langid_entry.length;
        desc->result = desc->length = real_len - 2;//��ȥ������
    }
    //��ȡUnicode����
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_langid_entry.offset);
    sys_sd_fread(res_ptr->res_fp, desc->data.str, (uint32) real_len);
}
/*! \endcond */
