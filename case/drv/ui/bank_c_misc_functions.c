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
 * \file     misc_functions.c
 * \brief    �������ܺ������������������� font ��������־��������Ļģʽ��������ʾbuffer���ȵ�
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ�������������� font ��������־��������Ļģʽ��������ʾbuffer���ȹ��ܡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ����
 * \param[in]    clrregion����Ҫ���������ָ��,�μ�region_t����
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \par          exmaple code
 * \code
    ����1: ���ָ������
    region_t region;
    
    region.x = 0;
    region.y = 10;
    region.width = 20;
    region.height = 40;
    ui_set_backgd_color(0x0000);//��ɫ
    ui_clear_screen(&region);
    
    ����2: ��ȫ��
    ui_set_backgd_color(0x0000);//��ɫ
    ui_clear_screen(NULL);
 * \endcode
 * \note
 *******************************************************************************/
void clear_screen(region_t *clrregion, void *null2, void *null3)
{
//    if (clrregion == NULL)
//    {
//        romf_fill_rect(&region_win, (uint16) (dc.backgd_color & 0xffff));
//    }
//    else//��ui�����ڲ���ֱ��ʹ�� romf_fill_rect �ӿڽ��оֲ����������Լ���bank �л�
//    {
//        romf_fill_rect(clrregion, (uint16) (dc.backgd_color & 0xffff));
//    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���� font ��������־
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \note
 *******************************************************************************/
void flush_font_buffer(void *null1, void *null2, void *null3)
{
#ifndef PC
    uint32 *font_cache_buffer = font_cache_addr;
#else
    uint32 *font_cache_buffer = GET_REAL_ADDR(font_cache_addr);
#endif
    if(*font_cache_buffer == FONT_CACHE_MAGIC)
    {
        *font_cache_buffer = 0x0;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ������Ļģʽ������������
 * \param[in]    screen_mode����Ҫ���õ���Ļģʽ��0��ʾ������1��ʾ����
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \par          exmaple code
 * \code
    ����1: ����ΪĬ��ģʽ��������ģʽ
    ui_set_screen_direction(0);
    
    ����2: ����Ϊ��תģʽ��������ģʽ
    ui_set_screen_direction(1);
 * \endcode
 * \note
 *******************************************************************************/
void set_screen_direction(uint8 screen_mode, void *null2, void *null3)
{
    if (screen_mode == 1)
    {
        rotate_flag = TRUE;
        region_win.x = 0;
        region_win.y = 0;
        region_win.width = DISPLAY_HEIGHT;
        region_win.height = DISPLAY_LENGTH;
    }
    else
    {
        rotate_flag = FALSE;
        region_win.x = 0;
        region_win.y = 0;
        region_win.width = DISPLAY_LENGTH;
        region_win.height = DISPLAY_HEIGHT;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ������ʾbuffer
 * \param[in]    LCD_RAM_INDEX��ʾUI 1K buffer,JPEG_RAM_INDEX��ʾJPEG 8K buffer
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \par          exmaple code
 * \code
    ����1: ����UI 1K buffer
    ui_set_display_buf(LCD_RAM_INDEX);
    
    ����2: ����JPEG 8K buffer
    ui_set_display_buf(JPEG_RAM_INDEX);
 * \endcode
 * \note
 *******************************************************************************/
void set_display_buf(uint8 mode, void *null2, void *null3)
{
    if (mode == JPEG_RAM_INDEX)
    {
        display_buf_addr = JPEG_BUF_ADDR;
        display_buf_len = JPEG_BUF_LEN;
        lcd_buffer_type = JPEG_RAM_INDEX;
        font_cache_addr = JPEG_BUF_ADDR + JPEG_BUF_LEN;//��6KB buffer��1KB/2KB
        font_cache_len = 2048;
        //ͷ����С��ѡ��̻�����Ĭ���ֿ�ʱ��1k������Ϊ64�ֽڣ�2k������Ϊ122�ֽ�
        font_cache_header = ((font_cache_len - 6) / (MAX_CHARPOINTE_SIZE + 2)) * 2 + 6;
        font_cache_flag = TRUE;
        lcd_mcu_set_JRAM_clk();
    }
    else
    {
        display_buf_addr = LCD_BUF_ADDR;
        display_buf_len = LCD_BUF_LEN;
        lcd_buffer_type = LCD_RAM_INDEX;
        //lcd_restore_JRAM_clk();
        font_cache_flag = FALSE;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ������ʾbuffer
 * \param[in]    mode ��� display.h �� ��������ѡ�� ˵��
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \par          exmaple code
 * \code
    ����1: ���ý��������б�˵�
    ui_set_lang_option(LANG_LIST_MENU_ENTER);
    
    ����2: �����˳������б�˵�
    ui_set_lang_option(LANG_LIST_MENU_EXIT);
 * \endcode
 * \note
 *******************************************************************************/
void set_lang_option(uint16 mode, void *null2, void *null3)
{
    lang_option = mode;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡSD��ͼƬ��Դ���浽buffer��
 * \param[in]    id��Ҫ��ȡ��ͼƬ��Դid
 * \param[out]   buffer:����ͼƬ��Դ���ݵ�buffer
 * \param[out]   res_size:����ͼƬ��Դ�ߴ���Ϣ�Ľṹ��ָ��
 * \return       none
 * \ingroup      picture
 * \note
 *******************************************************************************/
void read_pic_to_buffer(uint16 id, uint8 *buffer, res_size_t *res_size)
{
    res_open_infor_t *res_ptr;
    res_entry_t res_entry;

    //��ȡͼƬ��Դ��Ϣ
    res_ptr = res_ptr_select(id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(id) * RESHEADITEM));
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));

    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_entry.offset);
    sys_sd_fread(res_ptr->res_fp, buffer, res_entry.length);

    res_size->width = res_entry.width;
    res_size->height = res_entry.height;
}
//modify--vzhan
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾ�����ַ���
 * \param[in]    desc���ַ���������
 * \param[in]    str_region�������ַ�����ʾ����
 * \param[in]    mode���ַ����ģʽ
 * \param[out]   none
 * \return       �����ַ�����ʾ����ָ��
 * \note         1��Ҫ�󴫽������ַ����������������������ʾ�ı��롣
 *               2����ʾ����������������ʾ�����ַ������ַ���������
 *               3��str_region->height����ΪUNICODE_HEIGHT_DEF��
 *               4��desc->result ���ص����ַ�����ʾ��ֹԭ��
 *******************************************************************************/
uint8 *fixft_put_string_line(string_desc_t *desc, region_t *str_region, char_fill_mode_e mode)
{
    char_type_e (*get_next_char_type)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
    uint8 *str;
    uint16 length;
    int8 language;
    uint8 char_bytes;
    char_type_e char_type;
    uint8 fontwidth;

    //����������ʼ��
    str = desc->data.str;
    length = desc->length;
    language = desc->language;
    if (language == UNICODELANGUAGE)
    {
        get_next_char_type = get_next_char_type_unicode;
    }
    else
    {
        get_next_char_type = get_next_char_type_ansi;
    }

    //ֱ�Ӵ��ģʽ
    libc_memcpy(&region, str_region, sizeof(region_t));

    //����ַ����������ַ���
    while (1)
    {
        //�ַ��������Ѿ����
        if (length == 0)
        {
            desc->result = STRING_TAIL_LEN_ZERO;
            break;
        }
        //��ȡ��һ���ַ�����Ϊ�����������з�����ͨ�ַ�
        char_type = get_next_char_type(str, length, language, &char_bytes);
        if ((char_type == CHAR_EOF) || (char_type == CHAR_NOPEND))//���������߲���������ʾ
        {
            desc->result = STRING_TAIL_EOF_NOPEND;
            break;
        }
        else if (char_type == CHAR_UNI_FLAG)
        {
            str += 2;
            length -= 2;
            continue;
        }
        else if (char_type == CHAR_LR)//���з�
        {
            str += char_bytes;
            desc->result = STRING_TAIL_CHAR_LR;
            break;
        }
        else
        {
            //do nothing for QAC
        }

        //����nor �����ü��ֿ�������ֿ�
        char_bytes = get_next_char_point_hook(str, language, char_point_buffer);
        fontwidth = char_point_buffer[32];

        //�����ʾ���򣨿ռ䣩�Ƿ��㹻��ʾ 
        if ((fontwidth > region.width)//��ǰ�ַ���ʾ���� region ����
         || ((region.x + fontwidth) > (region_win.x + region_win.width)))//��ǰ�ַ���ʾ��������X��
        {
            desc->result = STRING_TAIL_REGION_ERR;
            break;
        }

        //ֱ�Ӵ��ģʽ����LCD�޷�����ʱʹ��
        char_fill(char_point_buffer, CHAR_DISP_NORMAL, 0);

        //ָ�����
        str += char_bytes;
        length -= char_bytes;
    }

    return str;
}


/*! \endcond */
