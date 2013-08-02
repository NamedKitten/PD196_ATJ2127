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
 * \file     ui_show_sliderbar.c
 * \brief    ��ʾSliderBar�ĺ���ģ��
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *               ʵ��sliderbar�ؼ����ݵĽ�������ʾsliderbar
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���ϵͳSD�ļ�ϵͳ�Ľӿں�����������ʾsliderbar
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//sliderbar attribute bits location
#define SLIDERBAR_SHOW_EN                (0x0001 << 0)
#define SLIDERBAR_SHOW_TITLE_EN          (0x0001 << 1)
#define SLIDERBAR_SHOW_NUM_EN            (0x0001 << 2)
#define SLIDERBAR_SHOW_UNIT_EN           (0x0001 << 3)
#define SLIDERBAR_SHOW_SLIDER_EN         (0x0001 << 4)
#define SLIDERBAR_LISTDIRECT_OPTION_SHIFT    (5)
#define SLIDERBAR_LISTDIRECT_OPTION      (0x0001 << SLIDERBAR_LISTDIRECT_OPTION_SHIFT)
#define SLIDERBAR_LISTDIRECT_HORIZONTAL  (0x0000 << SLIDERBAR_LISTDIRECT_OPTION_SHIFT)
#define SLIDERBAR_LISTDIRECT_VERTICAL    (0x0001 << SLIDERBAR_LISTDIRECT_OPTION_SHIFT)
#define SLIDERBAR_SHOW_TAGPIC_EN         (0x0001 << 6)
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾsliderbar
 * \param[in]   parambox_style ָ��parambox��style�ṹ�����
 * \param[in]   parambox_data Ӧ�ô��ݵ�parambox��˽�����ݽṹ��ָ��
 * \param[in]   mode parambox��ˢ��ģʽ,�μ�parambox_draw_mode_e����
 * \param[out]  none
 * \return      ui_result_e ���͵Ĳ������μ�ui_result_e����
 * \ingroup     ui_controls
 * \par         exmaple code
 * \code
    �� gui_set_parameter ��װ������ʵ����ֱ�Ӵ� gui_set_parameter ����
    style_infor_t parambox_style;
    parambox_private_t private_data;
    parambox_one_t param_item;
    
    parambox_style.style_id = SLIDERBAR_ID1;
    parambox_style.type = UI_AP;
    
    param_item.unit_id = V_U16_INVALID;
    param_item.min = 0;
    param_item.max = 40;
    param_item.step = 1;
    param_item.value = 24;
    param_item.cycle = FALSE;
    param_item.max_number = 2;
    param_item.value_str = NULL;
    param_item.adjust_func = NULL;
    param_item.callback = volumn_cb;
    
    private_data.back_id = V_U16_INVALID;
    private_data.icon_id = P_SPEAKER;
    private_data.title_id = V_U16_INVALID;
    private_data.param_cnt = 1;
    private_data.items = &param_item;
    private_data.active = 0;
    private_data.sign_flag = 0;
    private_data.scale = 0;//��sliderģ�壬�������
    
    ui_show_sliderbar(&parambox_style, &private_data, PARAMBOX_DRAW_ALL);
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e show_sliderbar(style_infor_t *parambox_style, parambox_private_t *parambox_data, parambox_draw_mode_e mode)
{
    sliderbar_t sliderbar;
    DC oldDC_sav;
    uint32 sliderbar_entry_addr;
    res_open_infor_t *style_ptr;
    uint8 i;
    uint8 type;
    /*���ݴ���Դ�ļ������ͣ���ָ��ָ��ͬ��ȫ�ֱ���*/
    if (parambox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    //��ȡparambox����Դ�ļ��е���ʼ��ַ
    get_ctrl_addr(SLIDERBAR, &sliderbar_entry_addr, style_ptr);
    //��ȡ��ǰparambox��style��Ϣ
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, sliderbar_entry_addr + (uint32) (parambox_style->style_id & 0x0fff)
            * sizeof(sliderbar_t));
    sys_sd_fread(style_ptr->res_fp, &sliderbar, sizeof(sliderbar_t));

    //����ʾsliderbar
    if ((sliderbar.attrib & SLIDERBAR_SHOW_EN) != SLIDERBAR_SHOW_EN)
    {
        return UI_NO_DISPLAY;
    }
    //����DC
    romf_get_DC_status(&oldDC_sav, 0, 0);
    //��ʼ��ȫ������
    if (mode == PARAMBOX_DRAW_ALL)
    {
        //��ʾsliderbar�ı�����������ʾ�û�ָ���ı���ͼƬ
        if (parambox_data->back_id != (uint16) - 1)
        {
            u_show_pic(parambox_data->back_id, sliderbar.back_x, sliderbar.back_y);
        }
        else if (sliderbar.back_id != (uint16) - 1)
        {
            u_show_pic(sliderbar.back_id, sliderbar.back_x, sliderbar.back_y);
        }
        else
        {
            //do nothing for QAC
        }
        
        //��ʾ��־ͼ
        if ((sliderbar.attrib & SLIDERBAR_SHOW_TAGPIC_EN) == SLIDERBAR_SHOW_TAGPIC_EN)
        {
            if (parambox_data->icon_id != (uint16) - 1)
            {
                u_show_pic(parambox_data->icon_id, sliderbar.icon_x, sliderbar.icon_y);
            }
            else if (sliderbar.icon_id != (uint16) - 1)
            {
                u_show_pic(sliderbar.icon_id, sliderbar.icon_x, sliderbar.icon_y);
            }
            else
            {
                //do nothing for QAC
            }
        }
        
        //��ʾ�ָ���
        if(sliderbar.addition_pic != (uint16) - 1)
        {
            u_show_pic(sliderbar.addition_pic, sliderbar.addition_x, sliderbar.addition_y);
        }
        
        //��ʾ�����ַ���
        if ((sliderbar.attrib & SLIDERBAR_SHOW_TITLE_EN) == SLIDERBAR_SHOW_TITLE_EN)
        {
            show_sliderbar_title(sliderbar.title_addr, parambox_data->title_id, style_ptr);
        }
        
        //��ʾ����ֵ�б�
        for (i = 0; i < parambox_data->param_cnt; i++)
        {
            if ((sliderbar.attrib & SLIDERBAR_SHOW_UNIT_EN) == SLIDERBAR_SHOW_UNIT_EN)
            {
                show_sliderbar_unit(&sliderbar, parambox_data, i, style_ptr);
            }
            
            if (i == parambox_data->active)
            {
                type = 1;
            }
            else
            {
                type = 0;
            }
            if ((sliderbar.attrib & SLIDERBAR_SHOW_NUM_EN) == SLIDERBAR_SHOW_NUM_EN)
            {
                show_sliderbar_num(&sliderbar, parambox_data, i, type, mode, style_ptr);
            }
            if ((sliderbar.attrib & SLIDERBAR_SHOW_SLIDER_EN) == SLIDERBAR_SHOW_SLIDER_EN)
            {
                show_sliderbar_slider(&sliderbar, parambox_data, i, type, mode, style_ptr);
            }
        }
    }
    else if (mode == PARAMBOX_DRAW_PARAM)
    {
        if ((sliderbar.attrib & SLIDERBAR_SHOW_NUM_EN) == SLIDERBAR_SHOW_NUM_EN)
        {
            show_sliderbar_num(&sliderbar, parambox_data, parambox_data->old, 0, mode, style_ptr);
            show_sliderbar_num(&sliderbar, parambox_data, parambox_data->active, 1, mode, style_ptr);
        }
        if ((sliderbar.attrib & SLIDERBAR_SHOW_SLIDER_EN) == SLIDERBAR_SHOW_SLIDER_EN)
        {
            show_sliderbar_slider(&sliderbar, parambox_data, parambox_data->old, 0, mode, style_ptr);
            show_sliderbar_slider(&sliderbar, parambox_data, parambox_data->active, 1, mode, style_ptr);
        }
    }
    else if (mode == PARAMBOX_DRAW_VALUE)
    {
        if ((sliderbar.attrib & SLIDERBAR_SHOW_NUM_EN) == SLIDERBAR_SHOW_NUM_EN)
        {
            show_sliderbar_num(&sliderbar, parambox_data, parambox_data->active, 1, mode, style_ptr);
        }
        if ((sliderbar.attrib & SLIDERBAR_SHOW_SLIDER_EN) == SLIDERBAR_SHOW_SLIDER_EN)
        {
            show_sliderbar_slider(&sliderbar, parambox_data, parambox_data->active, 1, mode, style_ptr);
        }
    }
    else
    {
        //do nothing for QAC
    }
    //�ָ�������ɫ
    romf_set_pen_color(oldDC_sav.pen_color, 0, 0);
    romf_set_backgd_color(oldDC_sav.backgd_color, 0, 0);
    return UI_NO_ERR;
}
/*! \endcond */
