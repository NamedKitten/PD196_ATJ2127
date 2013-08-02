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
 * \file     bank_b_ui_show_textbox.c
 * \brief   ��ʾtextbox��ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ��textbox�ؼ����ݵĽ�������ʾtextbox
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���ϵͳSD�ļ�ϵͳ�Ľӿں�����������ʾtextbox
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//textbox attribute bits location
#define TEXTBOX_SHOW_EN                 (0x0001 << 0)
#define TEXTBOX_BG_OPTION_SHIFT         (1)
#define TEXTBOX_BG_OPTION               (0x0001 << TEXTBOX_BG_OPTION_SHIFT)
#define BG_OPTION_COLOR                 (0x0000 << TEXTBOX_BG_OPTION_SHIFT)
#define BG_OPTION_PIC                   (0x0001 << TEXTBOX_BG_OPTION_SHIFT)
#define TEXTBOX_FONT_OPTION_SHIFT       (2)
#define TEXTBOX_FONT_OPTION             (0x0003 << TEXTBOX_FONT_OPTION_SHIFT)
#define FONT_OPTION_SMALL               (0x0000 << TEXTBOX_FONT_OPTION_SHIFT)
#define FONT_OPTION_MIDDLE              (0x0001 << TEXTBOX_FONT_OPTION_SHIFT)
#define FONT_OPTION_LARGE               (0x0002<< TEXTBOX_FONT_OPTION_SHIFT)
#define TEXTBOX_ARABIC_CANCEL_EN        (0x0001 << 4)
#define TEXTBOX_DISP_OPTION_SHIFT       (5)
#define TEXTBOX_DISP_OPTION             (0x0003 << TEXTBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_MLINE               (0x0000 << TEXTBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_SCROLL              (0x0001 << TEXTBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_NORECT              (0x0002 << TEXTBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_ELLIPSIS            (0x0003 << TEXTBOX_DISP_OPTION_SHIFT)
#define TEXTBOX_ALIGNMENT_OPTION_SHIFT  (7)
#define TEXTBOX_ALIGNMENT_OPTION        (0x0003 << TEXTBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_LEFT           (0x0000 << TEXTBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_CENTER         (0x0001 << TEXTBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_RIGHT          (0x0002 << TEXTBOX_ALIGNMENT_OPTION_SHIFT)

uint16 check_display_mode(uint16 attrib);

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾtextbox
 * \param[in]    textbox_style ָ��textbox��style�ṹ�����
 * \param[in]    textbox_data Ӧ�ô��ݵ�textbox��˽�����ݽṹ��ָ��
 * \param[in]    mode ��ʾtextbox��ģʽ
 * \param[out]   none
 * \return       ui_result_e ���͵Ĳ������μ�ui_result_e����
 * \retval       UI_NO_ERR ������ʾtextbox
 * \retval       UI_NEED_SCROLL ��textbox���ַ�����Ҫ������ʾ
 * \retval       other ��ʾ�쳣
 * \ingroup      ui_controls
 * \par          exmaple code
 * \code
    ����1����ʾ�û�ָ�����봮�����������룬Ҳ������Unicode��
    style_infor_t textbox_style;
    textbox_private_t textbox_param;
    
    textbox_style.style_id = TEXTBOX_ID1;
    textbox_style.type = UI_AP;
    
    textbox_param.str_id = -1;
    textbox_param.lang_id = g_comval.lang_id;
    textbox_param.str_value = "text";
    ui_show_textbox(&textbox_style, &textbox_param, TEXTBOX_DRAW_NORMAL);
    
    ����2����ʾ�û�ָ����������ַ���
    style_infor_t textbox_style;
    textbox_private_t textbox_param;
    
    textbox_style.style_id = TEXTBOX_ID1;
    textbox_style.type = UI_AP;
    
    textbox_param.str_id = S_STRING_ID1;
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_value = NULL;
    ui_show_textbox(&textbox_style, &textbox_param, TEXTBOX_DRAW_NORMAL);
    
    ����3����ʾtextbox��ѡ��Ķ�������ַ���
    style_infor_t textbox_style;
    
    textbox_style.style_id = TEXTBOX_ID1;
    textbox_style.type = UI_AP;
    
    ui_show_textbox(&textbox_style, NULL, TEXTBOX_DRAW_NORMAL);
    
    ����4��������ʾ�ַ�������textbox�ĳ��ַ���ģʽ��������Ϊ��������
    ��ʼ����ʾ�������£�
    style_infor_t textbox_style;
    textbox_private_t textbox_param;
    ui_result_e result;
    
    textbox_style.style_id = TEXTBOX_ID1;
    textbox_style.type = UI_AP;
    
    textbox_param.str_id = -1;
    textbox_param.lang_id = UNICODEDATA;
    textbox_param.str_value = g_filename;
    result = ui_show_textbox(&textbox_style, &textbox_param, TEXTBOX_DRAW_NORMAL);
    if(result == UI_NEED_SCROLL)
    {
        ����������ʱ��
    }
    
    �ַ���������ʾ������Ӧ�ü���ʱ��ISR�У�
    style_infor_t textbox_style;
    
    textbox_style.style_id = TEXTBOX_ID1;
    textbox_style.type = UI_AP;
    
    ui_show_textbox(&textbox_style, NULL, TEXTBOX_DRAW_SCROLL);
 * \endcode
 * \note
 * \li  ��textbox_data->str_value != NULL����������ʾstr_valueָ������ݣ���ʱ������ָ��data->lang_idΪ���뻹��unicode��
 * \li  textbox_data->str_value == NULL,��textbox_data->str_id��=-1����������ʾstr_id��ָ����ַ�����
 * \li  ��textbox_data->str_id == -1,����ʾTextBox��idָ�����ַ�����
 * \li  ���ַ�����Ҫ����������UI_NEED_SCROLL��
 *******************************************************************************/
ui_result_e show_textbox(style_infor_t *textbox_style, textbox_private_t *textbox_data, textbox_draw_mode_e mode)
{
    textbox_t textbox;
    region_t kregion;
    DC oldDC_sav;
    res_open_infor_t *style_ptr;
    uint32 textbox_entry_addr;
    uint8 *str = NULL;
    uint16 res_id;
    uint16 disp_mode = 0;
    uint16 textout_mode = 0;
    ui_result_e result;
    bool ret_val;
    int8 language;
    string_desc_t desc;
    scroll_mode_t scroll_mode;
    
    romf_get_DC_status(&oldDC_sav, 0, 0);
    
    if ((mode == TEXTBOX_DRAW_SCROLL) || (mode == TEXTBOX_DRAW_ITEM_SCROLL))
    {
        scroll_mode.mode = FALSE;
        ret_val = scroll_string(NULL, NULL, scroll_mode);
        romf_set_pen_color(oldDC_sav.pen_color, 0, 0);
        romf_set_backgd_color(oldDC_sav.backgd_color, 0, 0);
        if(ret_val == TRUE)
        {
            return UI_NEED_SCROLL;
        }
        else
        {
            return UI_SCROLL_OVER;
        }
    }

    /*���ݴ���Դ�ļ������ͣ���ָ��ָ��ͬ��ȫ�ֱ���*/
    if (textbox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    //��ȡAP���ݵ�˽������
    if (textbox_data == NULL)
    {
        language = UNICODEID;
        res_id = (uint16) - 1;
        str = NULL;
    }
    else
    {
        language = textbox_data->lang_id;
        str = textbox_data->str_value;
        res_id = textbox_data->str_id;
    }
    get_ctrl_addr(TEXTBOX, &textbox_entry_addr, style_ptr);
    //��ȡ��ǰtextbox��style��Ϣ
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, textbox_entry_addr + (uint32) (textbox_style->style_id & 0x0fff)
            * sizeof(textbox_t));
    sys_sd_fread(style_ptr->res_fp, &textbox, sizeof(textbox_t));
    //����ʾtextbox
    if ((textbox.attrib & TEXTBOX_SHOW_EN) != TEXTBOX_SHOW_EN)
    {
        return UI_NO_DISPLAY;
    }
    //�����ַ�����ʾ����
    kregion.x = textbox.x;
    kregion.y = textbox.y;
    kregion.width = textbox.width;
    kregion.height = textbox.height;
    
    if((textbox_data != NULL) && ((textbox_data->color & 0xffff0000) == 0x5a5a0000))//�û��Զ�����ɫ
    {
        romf_set_pen_color(textbox_data->color & 0x0000ffff, 0, 0);//��16λΪ��Ч��ɫֵ
    }
    else
    {
        romf_set_pen_color(textbox.text_color, 0, 0);
    }
    if ((textbox.attrib & TEXTBOX_BG_OPTION) == BG_OPTION_PIC)
    {
        textout_mode = CHAR_FILL_NORMAL; //�б���ͼƬ������͸��ģʽ
    }
    else
    {
        textout_mode = CHAR_FILL_COLOR; //������ɫ�����Ƿ�͸��ģʽ,Ĭ��Ϊ��͸������ģʽ
        romf_set_backgd_color(textbox.back_id, 0, 0);
    }

    //��textbox������ת���ɶ�Ӧ����ʾģʽ
    disp_mode = check_display_mode(textbox.attrib);
    disp_mode |= textout_mode;
    
    //�������ز���ʼ������ͼƬ���Ѿ���ʼ��ȫ�ֱ���
    if (mode == TEXTBOX_DRAW_ID3_VERTICAL)
    {
        if(textout_mode != CHAR_FILL_COLOR)//͸��ģʽ���б���ͼƬ
        {
            //���ز���ʼ������ͼƬ
            load_bgpic_sd(textbox.back_id, textbox.back_x, textbox.back_y, 0);
            disp_mode &= ~DISP_CHAR_FILL_OPTION;
            disp_mode |= CHAR_FILL_SDIMAGE;
        }
        libc_memcpy(&(scroll_desc.scroll_region), &kregion, sizeof(region_t));
        scroll_desc.disp_mode = disp_mode;
        scroll_desc.pen_color = textbox.text_color;
        scrollhead_height = 0;//��ʼ��ֱID3����ȫ�ֱ���
        return UI_NEED_SCROLL;
    }
    //ID3ˮƽ�������أ�������ʾ
    if (mode == TEXTBOX_DRAW_ID3_HORIZONTAL)
    {
        disp_mode |= DISP_DISPLAY_DISABLE;
    }
    else
    {
        //�������
        if (textout_mode == CHAR_FILL_NORMAL)
        {
            if (textbox.back_id != (uint16) - 1)
            {
                u_show_pic(textbox.back_id, textbox.back_x, textbox.back_y);
            }
        }
        else
        {
            //clear_screen(&kregion, 0, 0);
            romf_fill_rect(&kregion, (uint16) (dc.backgd_color & 0xffff));
        }
    }
    
    //������ʾ�û�ָ�����ַ���
    if (str != NULL)
    {
        if (mode == TEXTBOX_DRAW_ITEM)//����+����ģʽ
        {
            uint16 title_length;
            
            //����ʾ���� res_id
            desc.length = -1;
            desc.language = UNICODEID;
            desc.data.id = res_id;
            title_length = get_string_length_id(res_id, disp_mode);
            show_string(&desc, &kregion, disp_mode);

            //�ٽ�������ʾ���� str
            desc.data.str = str;
            desc.length = -1;
            if(language == UTF_8DATA)
            {
                desc.length = (uint16)libc_strlen(str);
            }
            desc.language = language;
            if (need_arabic_deal(disp_mode) == TRUE)
            {
                kregion.x = textbox.x;
                kregion.width = textbox.width - title_length;
            }
            else
            {
                kregion.x = textbox.x + title_length;
                kregion.width = textbox.width - title_length;
            }
            kregion.y = textbox.y;
            kregion.height = textbox.height;
            result = (ui_result_e) show_string(&desc, &kregion, disp_mode);
        }
        else//����ģʽ TEXTBOX_DRAW_NORMAL TEXTBOX_DRAW_NOSCROLL
        {
            desc.data.str = str;
            desc.length = -1;
            if(language == UTF_8DATA)
            {
                desc.length = (uint16)libc_strlen(str);
            }
            desc.language = language;
            result = (ui_result_e) show_string(&desc, &kregion, disp_mode);
        }
    }
    //�����ʾ�û�ָ���Ķ�������ַ���
    else if (res_id != (uint16) - 1)
    {
        desc.data.id = res_id;
        desc.language = UNICODEID;
        result = (ui_result_e) show_string(&desc, &kregion, disp_mode);
    }
    //��ʾtextbox��ָ���Ķ�������ַ���
    else if (textbox.id != (uint16) - 1)
    {
        desc.data.id = textbox.id;
        desc.language = UNICODEID;
        result = (ui_result_e) show_string(&desc, &kregion, disp_mode);
    }
    else//�����������������ַ���
    {
        if (textbox.back_id != (uint16) - 1)
        {
            u_show_pic(textbox.back_id, textbox.back_x, textbox.back_y);
        }
        result = UI_PARAM_ERR;
    }
    //�ָ�������ɫ
    romf_set_pen_color(oldDC_sav.pen_color, 0, 0);
    romf_set_backgd_color(oldDC_sav.backgd_color, 0, 0);
    disp_mode &= ~DISP_DISPLAY_DISABLE;
    
    //��ʼ�������� TEXTBOX_DRAW_ITEM TEXTBOX_DRAW_NORMAL
    if ((result == UI_NEED_SCROLL) && (mode != TEXTBOX_DRAW_NOSCROLL))
    {
        if(textout_mode != CHAR_FILL_COLOR)//͸��ģʽ���б���ͼƬ
        {
            //���ز���ʼ������ͼƬ
            load_bgpic_sd(textbox.back_id, textbox.back_x, textbox.back_y, 1);
            disp_mode &= ~DISP_CHAR_FILL_OPTION;
            disp_mode |= CHAR_FILL_SDIMAGE;
        }
        
        //��ʼ���ַ��������������������ַ���ĩβ���2�ո�
        scroll_mode.mode = TRUE;
        scroll_mode.disp_mode = disp_mode;
        scroll_desc.back_id = textbox.back_id;//�����ڼ�¼������ɫ
        scroll_desc.back_x = textbox.back_x;//��ʱû��
        scroll_desc.back_y = textbox.back_y;//��ʱû��
        scroll_desc.pen_color = textbox.text_color;
        scroll_string(&scroll_str, &kregion, scroll_mode);
    }
    
    return result;
}
/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ����textbox�������ж�textbox����ʾģʽ
 * \param[in]    attrib��Ҫ��ʾ��textbox��attrib������Ϣ
 * \param[out]   none
 * \return       uint16���͵Ĳ�����tetxbox����ʾģʽ
 * \ingroup      ui driver
 * \note
 *******************************************************************************/
uint16 check_display_mode(uint16 attrib)
{
    uint16 disp_mode = 0;

    //���ַ�����ʾģʽ
    if ((attrib & TEXTBOX_DISP_OPTION) == DISP_OPTION_MLINE)
    {
        disp_mode |= DISP_LONGSTR_MULTI_ROW;
    }
    else if ((attrib & TEXTBOX_DISP_OPTION) == DISP_OPTION_SCROLL)
    {
        disp_mode |= DISP_LONGSTR_SCROLL;
    }
    else if ((attrib & TEXTBOX_DISP_OPTION) == DISP_OPTION_NORECT)
    {
        disp_mode |= DISP_LONGSTR_NO_RECT;
    }
    else if ((attrib & TEXTBOX_DISP_OPTION) == DISP_OPTION_ELLIPSIS)
    {
        disp_mode |= DISP_LONGSTR_ELLIPSIS;
    }
    else
    {
        disp_mode |= DISP_LONGSTR_RECT;//Ĭ����ʾ�������ַ�
    }

    //����ģʽ
    if ((attrib & TEXTBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_CENTER)
    {
        disp_mode |= DISP_ALIGN_MEDIACY;
    }
    else if ((attrib & TEXTBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_RIGHT)
    {
        disp_mode |= DISP_ALIGN_RIGHT;
    }
    else
    {
        disp_mode |= DISP_ALIGN_LEFT;//Ĭ�������
    }

    //atrib<2-3>0��ʾС����,1��ʾ�к����壬2��ʾ�����壬Ĭ���к�����
    if ((attrib & TEXTBOX_FONT_OPTION) == FONT_OPTION_SMALL)
    {
        disp_mode |= DISP_FONT_SMALL;
    }
    else if ((attrib & TEXTBOX_FONT_OPTION) == FONT_OPTION_LARGE)
    {
        disp_mode |= DISP_FONT_LARGE;
    }
    else
    {
        disp_mode |= DISP_FONT_NORMAL; //Ĭ���к�����
    }

    //ȡ���������Ҷ���
    if ((attrib & TEXTBOX_ARABIC_CANCEL_EN) == TEXTBOX_ARABIC_CANCEL_EN)
    {
        disp_mode |= DISP_ARABIC_DISABLE;
    }

    return disp_mode;
}
/*! \endcond */

/*! \cond */

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ��������ַ�����Դ���ַ�������
 * \param[in]   str_id ��������ַ�����ԴID
 * \param[in]   disp_mode �ַ�����ʾģʽ
 * \param[out]  none
 * \return      uint16 �ַ�������
 * \note
 *******************************************************************************/
uint16 get_string_length_id(uint16 str_id, uint16 disp_mode)
{
    string_desc_t string_infor;
    uint16 str_len;
    
#if (USE_MULTI_FONTLIB != 0)
    //�л������ֿ�
    fix_switch_fontlib(disp_mode);
#endif

    libc_memset(string_buf, 0, STRING_BUFFER_LEN);
    
    string_infor.data.str = string_buf;
    string_infor.length = STRING_BUFFER_LEN;
    string_infor.language = UNICODEDATA;
    romf_get_unicode_data(str_id, &string_infor);
    
    //��������ϣ���������⴦��
    if(need_arabic_deal(0) == TRUE)
    {
        //��������ϣ������Ҫ���� UNICODE ��д����
        string_infor.argv = STRING_BUFFER_LEN;
        arabic_uni_join(&string_infor);
    }
    
    string_infor.language = UNICODELANGUAGE;

#if (USE_MULTI_FONTLIB == 0)    
    str_len = romf_get_string_length(&string_infor, 128, NULL);
#else
    str_len = fixf_get_string_length(&string_infor, 128, NULL);
#endif

    return str_len;
}

/*! \endcond */
