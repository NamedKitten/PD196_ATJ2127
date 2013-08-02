/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       cailizhen     2012-10-16 9:27     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_putstring_sub_2.c
 * \brief    �ַ�����ʾ�׶� bank c �ӿ�
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *           
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-10-16
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond */

/******************************************************************************/
/*!
 * \par  Description:
 *    ���ص�ǰ�ַ�������ͼƬ����ʼ��ȫ�ֽṹ�� text_bg_desc 
 * \param[in]    id������ͼƬID
 * \param[in]    x������ͼƬ��X����
 * \param[in]    y������ͼƬ��Y����
 * \param[in]    mode��ROW_PRIORITY_DEF ��ʾ�����ȣ�COL_PRIORITY_DEF ��ʾ������
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void load_bgpic_sd(uint16 id, uint16 x, uint16 y, uint8 mode)
{
    res_open_infor_t *res_ptr;
    res_entry_t res_entry;
    
    //��ȡͼƬ��Դ��Ϣ
    res_ptr = res_ptr_select(id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(id) * RESHEADITEM));
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));

    text_bg_desc.back_id = id;
    text_bg_desc.back_x = x;
    text_bg_desc.back_y = y;
    text_bg_desc.width = res_entry.width;
    text_bg_desc.height = res_entry.height;
    if(mode == ROW_PRIORITY_DEF)
    {
        text_bg_desc.offset_col = res_entry.offset;
    }
    else
    {
        text_bg_desc.offset_col = res_entry.offset + (res_entry.width * res_entry.height * 2);
        //4�ֽڶ���
        if((text_bg_desc.offset_col % 4) != 0)
        {
            text_bg_desc.offset_col = (text_bg_desc.offset_col & 0xfffffffc) + 4;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾ���������ַ���ǰ���ַ��������޸ģ�����ʵ�ְ�������ת�Դ�������ʾ�ȹ���
 * \param[in]   string_infor�������ַ���������
 * \param[in]   param���������
 * \param[out]  string_infor������ַ���������
 * \return      none
 * \note
 *******************************************************************************/
void bankaf_put_string_line_arabic_hook(string_desc_t *string_infor, uint16 disp_mode, uint16 param)
{
    libc_memcpy(arabic_string_buf, string_infor->data.str, (uint32)param);
    //���ַ�����ת
    arabic_do_turn(arabic_string_buf, 0, param);
    arabic_string_buf[param] = 0;
    arabic_string_buf[param + 1] = 0;
    string_infor->data.str = arabic_string_buf;
    string_infor->length = param;
    //arabic_stat = ARABIC_LINE_TURN; //��ʱ״̬�����޸�
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ָ���������ϴ��һ���ַ�,�������򲿷ֲ���ʾ-������ʽ
 * \param[in]   str ��ǰ�ַ���ָ��
 * \param[in]   language �ַ���������
 * \param[in]   char_region Ҫ�����������
 * \param[in]   disp_mode �ַ���ʾģʽ
 * \param[in]   start_col Ҫ��Ӻ��ֵ���һ�п�ʼ���
 * \param[in]   mode CHAR_FILL_SDIMAGE ģʽ����Ч��ROW_PRIORITY_DEF ��ʾ�����ȣ�COL_PRIORITY_DEF ��ʾ������
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void char_out_rect(uint8 *str, int8 language, region_t *char_region, uint16 disp_mode, uint8 start_col, uint8 mode)
{
    uint8 end;
    
    if (char_region->width == 0)
    {
        return;
    }
    
    get_next_char_point_hook(str, language, fix_p_char_point);
    
    //CHAR_FILL_SDIMAGE����SD��ȡ��������ֱ�ӵ����ַ���
    if((disp_mode & DISP_CHAR_FILL_OPTION) == CHAR_FILL_SDIMAGE)
    {
        if (need_arabic_deal(disp_mode) == TRUE)
        {
            end = fix_p_char_point[fix_font_size - 1] - 1;
            put_char_sd(fix_p_char_point, char_region, CHAR_DISP_RIGHT2LEFT, end - start_col, mode);
        }
        else
        {
            put_char_sd(fix_p_char_point, char_region, CHAR_DISP_LEFT2RIGHT, start_col, mode);
        }
    }
    else//����ģʽ
    {
        libc_memcpy(&region, char_region, sizeof(region_t));
    
        if (need_arabic_deal(disp_mode) == TRUE)
        {
            end = fix_p_char_point[fix_font_size - 1] - 1;
#if (USE_MULTI_FONTLIB == 0)
            romf_put_char(fix_p_char_point, disp_mode & DISP_CHAR_FILL_OPTION, CHAR_DISP_RIGHT2LEFT, end - start_col);
#else
            fixf_put_char(fix_p_char_point, disp_mode & DISP_CHAR_FILL_OPTION, CHAR_DISP_RIGHT2LEFT, end - start_col);
#endif
        }
        else
        {
#if (USE_MULTI_FONTLIB == 0)
            romf_put_char(fix_p_char_point, disp_mode & DISP_CHAR_FILL_OPTION, CHAR_DISP_LEFT2RIGHT, start_col);
#else
            fixf_put_char(fix_p_char_point, disp_mode & DISP_CHAR_FILL_OPTION, CHAR_DISP_LEFT2RIGHT, start_col);
#endif
        }
    }
}

/*! \endcond */
