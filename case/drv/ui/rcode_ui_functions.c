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
 * \file     ui_functions.c
 * \brief    ������Ļ������Ϣ��ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ����lcd��ص�һЩ����
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ��ǰλ���ַ�����--UNICODE����
 * \param[in]    str����ǰ�ַ���ָ��
 * \param[in]    language���ַ���������������
 * \param[out]   byte_cnt�����ص�ǰ��Ч�ַ����ֽ���
 * \return       char_type_e�������ַ�����
 * \note         ������ַ���һ�����ḻ�ĸ������Ϊ������ʾ���ַ���Ԫ��
 *******************************************************************************/
char_type_e ramf_next_char_unicode_hook(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)
{
    *byte_cnt = 2;
    return CHAR_NORMAL;
}
/******************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ��ǰλ���ַ�����--�������
 * \param[in]    str����ǰ�ַ���ָ��
 * \param[in]    language���ַ���������������
 * \param[out]   byte_cnt�����ص�ǰ��Ч�ַ����ֽ���
 * \return       char_type_e�������ַ�����
 * \note         ������ַ���һ�����ḻ�ĸ������Ϊ������ʾ���ַ���Ԫ��
 *******************************************************************************/
char_type_e ramf_next_char_ansi_hook(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)
{
    if((*str >= 0x80) && (language_2byte == TRUE))
    {
        if(length < 2)//���ݲ��㣬���� NOPEND
        {
            return CHAR_NOPEND;
        }
        else if(*(str + 1) == 0)//���ݴ��󣬷��� NOPEND���������н�������������ǰ2�ֽ��ַ����������
        {
            return CHAR_NOPEND;
        }
        else
        {
            *byte_cnt = 2;
        }
    }
    else
    {
        *byte_cnt = 1;
    }
    return CHAR_NORMAL;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��ǰ�ַ��ĵ���
 * \param[in]   str:��ǰ�ַ�ָ��
 * \param[in]   language����ǰ�ַ���������
 * \param[out]  pbuffer:�����ַ������buffer
 * \return      ��ǰ�ַ����ֽ�����2�ֽڻ�1�ֽ�
 * \note
 *******************************************************************************/
uint8 ramf_get_next_char_point_hook(uint8 *str, int8 language, uint8 *pbuffer)
{
    uint16 uni_code = 0;
    uint8 pt_step;

    if ((language == UNICODELANGUAGE) || ((*str> ASCIIDISCODE) && (language_2byte == TRUE)))
    {
        uni_code = *str + ((uint16) (*(str + 1)) << 8);
        pt_step = 2;
    }
    else
    {
        uni_code = *str;
        pt_step = 1;
    }

    //ת��ΪUnicode���룬Unicode���Ժ�С��0x80����������ת��
    if ((language != UNICODELANGUAGE) && (uni_code >= ASCIIDISCODE))
    {
        uni_code = romf_multi_char_to_unicode(uni_code);
    }
    //add for thai languange show by liutingting
    if ((uni_code >= 0x0E01) && (uni_code <= 0x0E7F))
    {
        thai_point(str, language, uni_code, pbuffer, &pt_step);
    }
    else
    {
#if (USE_MULTI_FONTLIB == 0)
        romf_get_next_char_point(uni_code, pbuffer);
#else
        fixf_get_next_char_point(uni_code, pbuffer);
#endif
    }
    return pt_step;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡָ���������Ļ����
 * \param[in]   bgpic_region:Ҫ��ȡ��������Ϣ
 * \param[in]   mode���ַ���ʾģʽ
 * \param[out]  display_buffer:������Ļ���ݵ�buffer
 * \return      none
 * \note
 *******************************************************************************/
void ramf_read_str_bgpic_hook(uint8 *display_buffer, region_t *bgpic_region, char_fill_mode_e mode)
{
/*    //modify--vzhan
    uint32 pixel_cnt, pixel_offset;
    display_buffer_deal();
    //��GRAM��ȡ�ַ�������
    if (mode == CHAR_FILL_NORMAL)
    {
#ifndef PC
        if (rotate_flag == FALSE)
        {
            lcd_set_draw_mode(DRAW_MODE_H_DEF); //��������ģʽ
        }
        else
        {
            lcd_set_draw_mode(DRAW_MODE_V_DEF); //���ú���ģʽ
        }
#else
        lcd_set_draw_mode(DRAW_MODE_H_DEF); //��������ģʽ
#endif

        //�贰,��ȡ����
        lcd_set_window(bgpic_region);

        //��ȡGRAM����
        pixel_cnt = (uint32) bgpic_region->width * bgpic_region->height;
        lcd_get_buff_data((uint8 *) display_buffer, pixel_cnt);
    }
    //��flash��SD����Ҫ�������ȴ�ţ���ȡ�ַ�������
    else if (mode == CHAR_FILL_SDIMAGE)
    {
        res_open_infor_t *res_ptr = res_ptr_select(text_bg_desc.back_id);
    
        pixel_cnt = (uint32) bgpic_region->width * bgpic_region->height * 2;
        //ע�������ȶ�ȡ����ʱ��x����Ϊ0����ǰӦ�����������
        pixel_offset = (uint32) bgpic_region->height * bgpic_region->x * 2;
        sys_sd_fseek(res_ptr->res_fp, SEEK_SET, text_bg_desc.offset_col + pixel_offset);
        sys_sd_fread(res_ptr->res_fp, display_buffer, pixel_cnt);
    }
    else
    {
        //for QAC
    }*/
    //CHAR_FILL_COLOR �Ѿ���romf_read_str_bgpic������
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ����LCD��ģʽ����ʾ����
 * \param[in]   win_region:Ҫ��ʾ��������Ϣ
 * \param[in]   mode���������ͣ�0��ʾͼƬ��1��ʾ�ַ���
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void ramf_set_lcd_param_hook(region_t *win_region, uint8 mode)
{
    //���³�ʼ��DMAΪRGBģʽ
    lcd_controller_init(RGB_MODE);
#ifndef PC
    if (rotate_flag == FALSE)
    {
        if(mode == LCD_PARAM_SCROLL)
        {
            lcd_set_draw_mode(DRAW_MODE_H_SCROLL_DEF); //���������ַ�������ģʽ
        }
        else
        {
            lcd_set_draw_mode(DRAW_MODE_H_DEF); //��������ģʽ
        }
    }
    else
    {
        if(mode == LCD_PARAM_SCROLL)
        {
    	    lcd_set_draw_mode(DRAW_MODE_V_SCROLL_DEF); //���ú����ַ�������ģʽ
    	}
    	else
    	{
            lcd_set_draw_mode(DRAW_MODE_V_DEF); //���ú���ģʽ
        }
    }
#else
    if(mode == LCD_PARAM_SCROLL)
    {
        lcd_set_draw_mode(DRAW_MODE_H_SCROLL_DEF); //���������ַ�������ģʽ
    }
    else
    {
        lcd_set_draw_mode(DRAW_MODE_H_DEF); //��������ģʽ
    }
#endif
    
    lcd_set_window(win_region);
    display_buffer_deal();
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ����ʾbuffer�е�����ͨ��DMA����
 * \param[in]   pixel_cnt:Ҫ��ʾ�����ص�ĸ���
 * \param[in]   data_width��ÿ�е����ص���
 * \param[out]  none
 * \return      DMA�����Ƿ�ɹ����ɹ�����TRUE��ʧ�ܷ���FALSE
 * \note
 *******************************************************************************/
bool ramf_dma_buffer_data_hook(uint32 pixel_cnt, uint16 data_width)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
	lcd_buff_data_trans(display_buffer,0);
	return TRUE;

//    //DMA��GRAM��
//    lcd_dma_set_counter(pixel_cnt, data_width);
//    //��ʼDMA����
//    return (bool) (uint32) lcd_dma_start_trans(lcd_buffer_type);
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ָ���������ϴ��һ�����ص�
 * \param[in]   point_region:���ص���������
 * \param[in]   trans_font��Ҫ��������ص������
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void ramf_char_fill_hook(region_t *point_region, uint8 *trans_font)
{
    lcd_set_window(point_region);
    lcd_buff_data_trans(trans_font, 1);
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
void ramf_put_string_line_hook(string_desc_t *string_infor, uint16 disp_mode, uint16 param)
{
    if ((need_arabic_deal(disp_mode) == TRUE) && (arabic_stat == ARABIC_ASCII_TURN))
    {
        bankaf_put_string_line_arabic_hook(string_infor, disp_mode, param);
    }
}

/*! \endcond */

/*! \cond */

void display_buffer_deal(void)
{
    if (lcd_buffer_type == JPEG_RAM_INDEX)
    {
#ifndef PC
        if (sys_get_drv_install_info(DRV_GROUP_STG_BASE) > 0)
        {
            base_op_entry(1, 0, 0, BASE_UPDATE);
        }
#endif
        //lcd_mcu_set_JRAM_clk();
    }

}
//modify--vzhan
bool u_show_pic(uint16 id, uint16 x, uint16 y)
{
    res_open_infor_t *res_ptr;
    res_entry_t res_entry;
    region_t res_region;

    //��ȡͼƬ��Դ��Ϣ
    res_ptr = res_ptr_select(id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(id) * RESHEADITEM));
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));

    /*�贰��������*/
    res_region.x = x;
    res_region.y = y;
    res_region.width = res_entry.width;
    res_region.height = res_entry.height;

    /*��ȡͼƬ���ݲ�ˢ��*/
    return u_put_sd_image(res_ptr->res_fp, res_entry.offset, &res_region);
}

/*! \endcond */
