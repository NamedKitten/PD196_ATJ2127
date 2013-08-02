/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>       <time>           <version >             <desc>
 *       betty     2011-09-07 16:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_showlistbox.c
 * \brief    display  listbox
 * \author   betty
 * \par      GENERAL DESCRIPTION:
 *               ʵ��lsitbox�ؼ����ݵĽ�������ʾlistbox
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���ϵͳSD�ļ�ϵͳ�Ľӿں�����������ʾlistbox
 * \version 1.0
 * \date  2011/09/07
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//listbox attribute bits location
#define LISTBOX_LIST_SHOW_EN        (0x0001 << 0)
#define LISTBOX_TITLE_SHOW_EN       (0x0001 << 1)
#define LISTBOX_SLIDER_SHOW_EN      (0x0001 << 2)
#define LISTBOX_RATIO_SHOW_EN       (0x0001 << 3)

#define TITLE_ALIGNMENT_OPTION_SHIFT    (0)
#define TITLE_ALIGNMENT_OPTION          (0x0003 << TITLE_ALIGNMENT_OPTION_SHIFT)
#define TITLE_ALIGNMENT_OPTION_LEFT     (0x0000 << TITLE_ALIGNMENT_OPTION_SHIFT)
#define TITLE_ALIGNMENT_OPTION_CENTER   (0x0001 << TITLE_ALIGNMENT_OPTION_SHIFT)
#define TITLE_ALIGNMENT_OPTION_RIGHT    (0x0002 << TITLE_ALIGNMENT_OPTION_SHIFT)
#define TITLE_FONT_OPTION_SHIFT         (2)
#define TITLE_FONT_OPTION               (0x0003 << TITLE_FONT_OPTION_SHIFT)
#define TITLE_FONT_OPTION_SMALL         (0x0000 << TITLE_FONT_OPTION_SHIFT)
#define TITLE_FONT_OPTION_MIDDLE        (0x0001 << TITLE_FONT_OPTION_SHIFT)
#define TITLE_FONT_OPTION_LARGE         (0x0002 << TITLE_FONT_OPTION_SHIFT)
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾlistbox
 * \param[in]   listbox_style ָ��listbox��style�ṹ�����
 * \param[in]   listbox_data Ӧ�ô��ݵ�listbox��˽�����ݽṹ��ָ��
 * \param[in]   mode listbox��ˢ��ģʽ���μ�list_draw_mode_e����
 * \param[out]  none
 * \return      ui_result_e ���͵Ĳ������μ�ui_result_e����
 * \retval      UI_NO_ERR ������ʾlistbox
 * \retval      UI_NEED_SCROLL listbox�ļ�������Ҫ������ʾ
 * \retval      other ��ʾ�쳣
 * \ingroup     ui_controls
 * \par         exmaple code
 * \code
    ����1���� gui_directory �� gui_menulist �ȷ�װ�������������ǲο� gui_menulist ��д
    style_infor_t listbox_style;
    listbox_private_t listbox_param;
    uint8 index;
    
    listbox_style.style_id = LISTBOX_ID1;//��ģ��û�� title ������Ҫ��д title ��
    listbox_style.type = UI_AP;
    
    listbox_param.item_valid = 6;
    listbox_param.old = listbox_param.active = 0;
    listbox_param.list_no = g_list_no;
    listbox_param.list_total = g_list_total;
    
    for(index = 0; index < listbox_param.item_valid; index++)
    {
    listbox_param.items[index].data.id = g_menu_items[index].str_id;
    listbox_param.items[index].language = UNICODEID;
    listbox_param.items[index].argv = ITEMS_BACK_TAG_ISNOT_SELECT;
    }
    
    result = ui_show_listbox(&listbox_style, &listbox_param, LIST_DRAW_ALL);
    if(result == UI_NEED_SCROLL)
    {
    ��������Ҫ�����������ù�����ʱ��
    }
    
    ����2��������ʾ�������ַ���
    style_infor_t listbox_style;
    
    listbox_style.style_id = LISTBOX_ID1;
    listbox_style.type = UI_AP;
    
    ui_show_listbox(&listbox_style, NULL, LIST_DRAW_TITLE_SCROLL);
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e show_listbox(style_infor_t *listbox_style, listbox_private_t *listbox_data, list_draw_mode_e mode)
{
    listbox_t listbox;
    DC oldDC_sav;
    res_open_infor_t *style_ptr;
    uint32 listbox_entry_addr;
    ui_result_e result = UI_NO_ERR;
    uint8 i;

    if (mode == LIST_DRAW_ACTIVE_SCROLL)
    {
        scroll_mode_t scroll_mode;
        scroll_mode.disp_mode = 0;
        scroll_mode.mode = FALSE;
        scroll_string(NULL, NULL, scroll_mode);
        return UI_NO_ERR;
    }
    if (listbox_data == NULL)
    {
        return UI_PARAM_ERR;
    }
    /*���ݴ���Դ�ļ������ͣ���ָ��ָ��ͬ��ȫ�ֱ���*/
    if (listbox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }

    get_ctrl_addr(LISTBOX, &listbox_entry_addr, style_ptr); //��ȡlistbox�ؼ���UI�����ļ��еĿ�ʼλ��
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, listbox_entry_addr + (uint32) (listbox_style->style_id & 0x0fff)
            * sizeof(listbox_t));
    sys_sd_fread(style_ptr->res_fp, &listbox, sizeof(listbox_t));
    //����ʾlistbox
    if ((listbox.attrib & LISTBOX_LIST_SHOW_EN) != LISTBOX_LIST_SHOW_EN)
    {
        return UI_NO_DISPLAY;
    }

    //��ȡ list_item �ṹ��
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, listbox.item_addr);
    sys_sd_fread(style_ptr->res_fp, &list_item, sizeof(listbox_item_t));

    if ((mode == LIST_DRAW_ALL) && (listbox.back_id != (uint16) - 1))
    {
        u_show_pic(listbox.back_id, listbox.back_x, listbox.back_y);
    }

    romf_get_DC_status(&oldDC_sav, 0, 0);
    if ((listbox.attrib & LISTBOX_TITLE_SHOW_EN) == LISTBOX_TITLE_SHOW_EN)
    {
        show_list_title(listbox.title_addr, &(listbox_data->title), mode, style_ptr);
    }
    if ((listbox.attrib & LISTBOX_SLIDER_SHOW_EN) == LISTBOX_SLIDER_SHOW_EN)
    {
        show_list_slider(listbox.slider_addr, listbox_data, mode, style_ptr);
    }
    if ((listbox.attrib & LISTBOX_RATIO_SHOW_EN) == LISTBOX_RATIO_SHOW_EN)
    {
        show_list_ratio(listbox.ratio_addr, listbox_data, mode, style_ptr);
    }

    //ˢ��ȫ���˵������ʾ�Ǽ�������ص��ϼ�����
    if ((mode == LIST_DRAW_ALL) || (mode == LIST_DRAW_LIST))
    {
        for (i = 0; i < listbox_data->item_valid; i++)
        {
            if (i == listbox_data->active)
            {
                result = (ui_result_e) show_list_item(&listbox, listbox_data, i, 1, style_ptr);
            }
            else
            {
                show_list_item(&listbox, listbox_data, i, 0, style_ptr);
            }
        }
        
        //�������
        for (; i < listbox.num_one_page; i++)
        {
            show_list_item(&listbox, listbox_data, i, 0xff, style_ptr);
        }
    }
    //ֻˢ�»�����ʾ�Ǽ��������ʾ������
    else
    {
        show_list_item(&listbox, listbox_data, listbox_data->old, 0, style_ptr);
        result = (ui_result_e) show_list_item(&listbox, listbox_data, listbox_data->active, 1, style_ptr);
    }
    //�ָ�������ɫ
    romf_set_pen_color(oldDC_sav.pen_color, 0, 0);
    romf_set_backgd_color(oldDC_sav.backgd_color, 0, 0);
    return result;
}
/*! \endcond */
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾlistbox ��slider
 * \param[in]   addr��listbox��slider style��Ϣ����Դ�ļ��е�ƫ�Ƶ�ַ
 * \param[in]   listbox_data��Ӧ�ô��ݵ�listbox��˽�����ݽṹ��ָ��
 * \param[in]   mode��listbox��ˢ��ģʽ
 * \param[in]   style_ptr����ǰ��Դ��Ϣ�ṹ��ָ��
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_list_slider(uint32 addr, listbox_private_t *listbox_data, list_draw_mode_e mode, res_open_infor_t *style_ptr)
{
    listbox_slider_t list_slider;
    uint16 cur_pixel;

    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, addr);
    sys_sd_fread(style_ptr->res_fp, &list_slider, sizeof(listbox_slider_t));
    //ֻ��ȫ��ˢ��ʱ������Ҫˢ�»���������
    if (mode == LIST_DRAW_ALL)
    {
        u_show_pic(list_slider.slider_back_id, list_slider.slider_back_x, list_slider.slider_back_y);
        prev_pixel = 0;
    }
    else
    {
        u_show_pic(list_slider.slider_clear_id, list_slider.slider_point_x, list_slider.slider_point_y + prev_pixel);
    }
    if (listbox_data->list_total > 1)
    {
        cur_pixel = listbox_data->list_no * list_slider.slider_length / (listbox_data->list_total - 1);
    }
    else
    {
        cur_pixel = 0;
    }
    u_show_pic(list_slider.slider_point_id, list_slider.slider_point_x, list_slider.slider_point_y + cur_pixel);
    prev_pixel = cur_pixel;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾlistbox ��title
 * \param[in]   addr��listbox��title style��Ϣ����Դ�ļ��е�ƫ�Ƶ�ַ
 * \param[in]   title_str��Ӧ�ô��ݵ�listbox title��˽�����ݽṹ�����
 * \param[in]   mode��listbox��ˢ��ģʽ
 * \param[in]   style_ptr����ǰ��Դ��Ϣ�ṹ��ָ��
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_list_title(uint32 addr, string_desc_t *title_str, list_draw_mode_e mode, res_open_infor_t *style_ptr)
{
    listbox_title_t list_title;
    region_t title_region;
    uint16 disp_mode = 0;

    if (mode != LIST_DRAW_ALL)
    {
        return;
    }
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, addr);
    sys_sd_fread(style_ptr->res_fp, &list_title, sizeof(listbox_title_t));
    //atrib<11-12>0��ʾС����,1��ʾ�к����壬2��ʾ�����壬Ĭ���к�����
    if ((list_title.attrib & TITLE_FONT_OPTION) == TITLE_FONT_OPTION_SMALL)
    {
        disp_mode |= DISP_FONT_SMALL;
    }
    else if ((list_title.attrib & TITLE_FONT_OPTION) == TITLE_FONT_OPTION_LARGE)
    {
        disp_mode |= DISP_FONT_LARGE;
    }
    else
    {
        disp_mode |= DISP_FONT_NORMAL; //Ĭ���к�����
    }

    romf_set_pen_color(list_title.str_color, 0, 0);
    //��ʾ��������
    title_region.x = list_title.str_x;
    title_region.y = list_title.str_y;
    title_region.width = list_title.str_width;
    title_region.height = list_title.str_height;
    if ((list_title.attrib & TITLE_ALIGNMENT_OPTION) == TITLE_ALIGNMENT_OPTION_LEFT)
    {
        disp_mode |= DISP_ALIGN_LEFT;
    }
    else if ((list_title.attrib & TITLE_ALIGNMENT_OPTION) == TITLE_ALIGNMENT_OPTION_RIGHT)
    {
        disp_mode |= DISP_ALIGN_RIGHT;
    }
    else
    {
        disp_mode |= DISP_ALIGN_MEDIACY;
    }
    show_string(title_str, &title_region, disp_mode);
}
/*! \endcond */
