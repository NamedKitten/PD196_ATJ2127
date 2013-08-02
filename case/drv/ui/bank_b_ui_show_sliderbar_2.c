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
 * \file     ui_show_sliderbar_2.c
 * \brief    ��ʾsliderbar��slider��ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ��slider��ؼ����ݵĽ�������ʾslider
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���ϵͳSD�ļ�ϵͳ�Ľӿں�����������ʾslider
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//sliderbar slider attribute bits location
#define SLIDER_DIRECTION_OPTION_SHIFT       (0)
#define SLIDER_DIRECTION_OPTION             (0x0001 << SLIDER_DIRECTION_OPTION_SHIFT)
#define DIRECTION_OPTION_HORIZONTAL         (0x0000 << SLIDER_DIRECTION_OPTION_SHIFT)
#define DIRECTION_OPTION_VERTICAL           (0x0001 << SLIDER_DIRECTION_OPTION_SHIFT)
#define SLIDER_STYLE_OPTION_SHIFT           (1)
#define SLIDER_STYLE_OPTION                 (0x0003 << SLIDER_STYLE_OPTION_SHIFT)
#define STYLE_OPTION_FILL                   (0x0000 << SLIDER_STYLE_OPTION_SHIFT)
#define STYLE_OPTION_CURSOR                 (0x0001 << SLIDER_STYLE_OPTION_SHIFT)
#define STYLE_OPTION_BOTH                   (0x0002 << SLIDER_STYLE_OPTION_SHIFT)
#define SLIDER_SCALE_OPTION_SHIFT           (3)
#define SLIDER_SCALE_OPTION                 (0x0001 << SLIDER_SCALE_OPTION_SHIFT)
#define SCALE_OPTION_NORMAL                 (0x0000 << SLIDER_SCALE_OPTION_SHIFT)
#define SCALE_OPTION_SCALE                  (0x0001 << SLIDER_SCALE_OPTION_SHIFT)
#define SLIDER_SYMMETRY_OPTION_SHIFT        (4)
#define SLIDER_SYMMETRY_OPTION              (0x0001 << SLIDER_SYMMETRY_OPTION_SHIFT)
#define SYMMETRY_OPTION_NORMAL              (0x0000 << SLIDER_SYMMETRY_OPTION_SHIFT)
#define SYMMETRY_OPTION_SYMMETRY            (0x0001 << SLIDER_SYMMETRY_OPTION_SHIFT)
#define SLIDER_INACTIVE_BG_OPTION_SHIFT     (5)
#define SLIDER_INACTIVE_BG_OPTION           (0x0001 << SLIDER_INACTIVE_BG_OPTION_SHIFT)
#define SLIDER_INACTIVE_BG_OPTION_SPIC      (0x0000 << SLIDER_INACTIVE_BG_OPTION_SHIFT)
#define SLIDER_INACTIVE_BG_OPTION_MPIC      (0x0001 << SLIDER_INACTIVE_BG_OPTION_SHIFT)
#define SLIDER_ACTIVE_BG_OPTION_SHIFT       (6)
#define SLIDER_ACTIVE_BG_OPTION             (0x0001 << SLIDER_ACTIVE_BG_OPTION_SHIFT)
#define SLIDER_ACTIVE_BG_OPTION_SPIC        (0x0000 << SLIDER_ACTIVE_BG_OPTION_SHIFT)
#define SLIDER_ACTIVE_BG_OPTION_MPIC        (0x0001 << SLIDER_ACTIVE_BG_OPTION_SHIFT)
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾsliderbar��slider
 * \param[in]   sliderbar��sliderbar�ؼ����ݽṹ�����
 * \param[in]   parambox_data��Ӧ�ô��ݵ�sliderbar��˽�����ݽṹ��ָ��
 * \param[in]   itemnum���ڼ���������
 * \param[in]   type����ǰ���Ƿ�Ϊ������
 * \param[in]   mode��sliderbar��ˢ��ģʽ
 * \param[in]   style_ptr����ǰ��Դ��Ϣ�ṹ��ָ��
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_sliderbar_slider(sliderbar_t *sliderbar, parambox_private_t *parambox_data, uint16 itemnum, uint8 type,
        parambox_draw_mode_e mode, res_open_infor_t *style_ptr)
{
    sliderbar_slider_t sliderbar_slider;
    res_entry_t res_entry;
    res_open_infor_t *res_ptr;
    region_t fill_region;
    int16 slider_cur_offset, tmp_slider_prev_offset;
    uint16 scale = 0;
    int16 i;
    int16 temp_block = 0;
    uint16 slider_back_id;
    uint16 back_x;
    uint16 prog_x;
    uint16 fill_x = 0;
    uint16 fill_y = 0;
    uint16 fill_id;
    uint8 prog_flag = 0;
    uint8 prog_direct_flag = 1;
    uint8 fill_mode;
    uint16 scale_value;
    uint16 one_width = sliderbar->list_width / sliderbar->param_cnt;
    
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, sliderbar->slider_addr);
    sys_sd_fread(style_ptr->res_fp, &sliderbar_slider, sizeof(sliderbar_slider_t));
    back_x = sliderbar_slider.slider_back_x + (one_width*itemnum);
    prog_x = sliderbar_slider.prog_x + (one_width*itemnum);
    fill_x = sliderbar_slider.slider_fill_x + (one_width*itemnum);
    
    //ֻ�е�ǰ����ֵ�Ż��õ� prev ֵ
    if (mode == PARAMBOX_DRAW_VALUE)
    {
        tmp_slider_prev_offset = slider_prev_offset;
    }
    else
    {
        if (type == 1)//���� sliderbar �״���0
        {
            slider_prev_offset = 0;
        }
        tmp_slider_prev_offset = 0;//ˢȫ������
    }
    
    //ˢ����������
    if ((mode == PARAMBOX_DRAW_ALL) || (mode == PARAMBOX_DRAW_PARAM))
    {
        //�Ǽ�����slider����ͼƬ
        if (type == 0)
        {
            if ((sliderbar_slider.attrib & SLIDER_INACTIVE_BG_OPTION) == SLIDER_INACTIVE_BG_OPTION_SPIC)
            {
                slider_back_id = sliderbar_slider.slider_back_id;
            }
            else
            {
                slider_back_id = sliderbar_slider.slider_back_id + itemnum;
            }
        }
        //������slider����ͼƬ
        else
        {
            if ((sliderbar_slider.attrib & SLIDER_ACTIVE_BG_OPTION) == SLIDER_ACTIVE_BG_OPTION_SPIC)
            {
                slider_back_id = sliderbar_slider.slider_back_act_id;
            }
            else
            {

                slider_back_id = sliderbar_slider.slider_back_act_id + itemnum;
            }
        }
        res_ptr = res_ptr_select(slider_back_id);
        //��ȡͼƬ�����Ϣ
        sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (slider_back_id & 0x7fff)
                * RESHEADITEM);
        sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
        u_show_pic(slider_back_id, back_x, sliderbar_slider.slider_back_y);
    }
    
    //��������
    if ((sliderbar_slider.attrib & SLIDER_SCALE_OPTION) == SCALE_OPTION_SCALE)
    {
        //����̶�ֵ
        scale = (sliderbar_slider.length - 1) / (parambox_data->items[itemnum].max - parambox_data->items[itemnum].min);
        if (scale <= 1)//�̶�����Ϊ2
        {
            scale = parambox_data->scale;
            scale_value = (uint16)(parambox_data->items[itemnum].max - parambox_data->items[itemnum].min);
            scale_value *= parambox_data->scale;
            if(sliderbar_slider.length > 1)//for QAC
            {
                scale_value /= (sliderbar_slider.length - 1);
            }
        }
        else
        {
            scale_value = (uint16)parambox_data->items[itemnum].step;
        }
        //���㵱ǰֵ
        if ((sliderbar_slider.attrib & SLIDER_SYMMETRY_OPTION) == SYMMETRY_OPTION_SYMMETRY)
        {
            slider_cur_offset = (int16) parambox_data->items[itemnum].value;
            if ((sliderbar_slider.attrib & SLIDER_DIRECTION_OPTION) == DIRECTION_OPTION_HORIZONTAL) //����ˢ��
            {
                fill_x += (sliderbar_slider.length - 1) / 2;
            }
            else//����ˢ��
            {
                sliderbar_slider.slider_start_y -= (sliderbar_slider.length - 1) / 2;
            }
        }
        else
        {
            slider_cur_offset = (int16) (parambox_data->items[itemnum].value - parambox_data->items[itemnum].min);
        }
        if ((slider_cur_offset % scale_value) != 0)
        {
            slider_cur_offset = (int16)((slider_cur_offset / scale_value * scale) + scale);
        }
        else
        {
            slider_cur_offset = (int16)(slider_cur_offset / scale_value * scale);
        }
    }
    else//û�п̶Ƚ�����
    {
        //���㵱ǰֵ
        if ((sliderbar_slider.attrib & SLIDER_SYMMETRY_OPTION) == SYMMETRY_OPTION_SYMMETRY)
        {
            slider_cur_offset = (int16) parambox_data->items[itemnum].value;
            if ((sliderbar_slider.attrib & SLIDER_DIRECTION_OPTION) == DIRECTION_OPTION_HORIZONTAL) //����ˢ��
            {
                fill_x += sliderbar_slider.length / 2;
            }
            else//����ˢ��
            {
                sliderbar_slider.slider_start_y -= sliderbar_slider.length / 2;
            }
        }
        else
        {
            slider_cur_offset = (int16) (parambox_data->items[itemnum].value - parambox_data->items[itemnum].min);
        }
        
        if(parambox_data->items[itemnum].value == parambox_data->items[itemnum].max)
        {
            if ((sliderbar_slider.attrib & SLIDER_SYMMETRY_OPTION) == SYMMETRY_OPTION_SYMMETRY)
            {
                slider_cur_offset = (int16)(sliderbar_slider.length / 2);
            }
            else
            {
                slider_cur_offset = (int16)(sliderbar_slider.length);
            }
        }
        else
        {
            slider_cur_offset = (int16)(slider_cur_offset * sliderbar_slider.length);
            slider_cur_offset = (int16)(slider_cur_offset / \
            (parambox_data->items[itemnum].max - parambox_data->items[itemnum].min));
        }
    }
        
    //��Ҫˢ�α�
    if (((sliderbar_slider.attrib & SLIDER_STYLE_OPTION) == STYLE_OPTION_CURSOR) || ((sliderbar_slider.attrib
            & SLIDER_STYLE_OPTION) == STYLE_OPTION_BOTH))
    {
        if ((sliderbar_slider.attrib & SLIDER_DIRECTION_OPTION) == DIRECTION_OPTION_HORIZONTAL) //����ˢ��
        {
            u_show_pic(sliderbar_slider.prog_id, prog_x + slider_cur_offset, sliderbar_slider.slider_start_y);
            if (mode == PARAMBOX_DRAW_VALUE)
            {
                u_show_pic(sliderbar_slider.unprog_id, prog_x + tmp_slider_prev_offset, \
                    sliderbar_slider.slider_start_y);
            }
        }
        else //����
        {
            u_show_pic(sliderbar_slider.prog_id, prog_x, sliderbar_slider.slider_start_y - slider_cur_offset);
            if (mode == PARAMBOX_DRAW_VALUE)
            {
                u_show_pic(sliderbar_slider.unprog_id, prog_x, \
                    sliderbar_slider.slider_start_y - tmp_slider_prev_offset);
            }
        }
    }
    if (type == 0)
    {
        fill_id = sliderbar_slider.slider_fill_id;
    }
    else
    {
        fill_id = sliderbar_slider.slider_fill_act_id;
    }
    //��Ҫˢ����
    if (((sliderbar_slider.attrib & SLIDER_STYLE_OPTION) == STYLE_OPTION_FILL) || ((sliderbar_slider.attrib
            & SLIDER_STYLE_OPTION) == STYLE_OPTION_BOTH))
    {
        temp_block = slider_cur_offset - tmp_slider_prev_offset;
        if (temp_block < 0)
        {
            temp_block = -temp_block;
            if (tmp_slider_prev_offset <= 0)
            {
                prog_flag = 1;
                prog_direct_flag = 0;//�������
            }
            else
            {
                prog_flag = 0;
                prog_direct_flag = 1;//����ݼ�
            }
        }
        else if (temp_block > 0)
        {
            if (tmp_slider_prev_offset >= 0)
            {
                prog_flag = 1;
                prog_direct_flag = 1;//�������
            }
            else
            {
                prog_flag = 0;
                prog_direct_flag = 0;//����ݼ�
            }
        }
        else
        {
            //������½��ȣ��˳�
            return;
        }
        if ((sliderbar_slider.attrib & SLIDER_DIRECTION_OPTION) == DIRECTION_OPTION_HORIZONTAL) //����ˢ��
        {
            fill_x += tmp_slider_prev_offset;
        }
        else//����ˢ��
        {
            fill_y = sliderbar_slider.slider_start_y -= tmp_slider_prev_offset;
            if ((sliderbar_slider.attrib & SLIDER_SYMMETRY_OPTION) == SYMMETRY_OPTION_SYMMETRY)
            {
                if (prog_direct_flag == 1)//���������y����-1
                {
                    fill_y--;
                }
                else//���������y����+1
                {
                    fill_y++;
                }
            }
        }

        if (prog_flag == 1) //��������
        {
            if ((sliderbar_slider.attrib & SLIDER_DIRECTION_OPTION) == DIRECTION_OPTION_HORIZONTAL) //����ˢ��
            {
                fill_region.x = fill_x;
                fill_region.y = sliderbar_slider.slider_start_y;
                fill_region.width = (uint16)temp_block;
                fill_region.height = 0;
                fill_mode = DUMP_LEFT2RIGHT;
            }
            else
            {
                fill_region.x = fill_x;
                if (prog_direct_flag == 1)
                {
                    fill_region.y = fill_y - (uint16)temp_block + 1;
                }
                else
                {
                    fill_region.y = fill_y;
                }
                fill_region.width = 0;
                fill_region.height = (uint16)temp_block;
                fill_mode = DUMP_DOWN2UP;
            }

            if (type == 0)
            {
                fill_id = sliderbar_slider.slider_fill_id;
            }
            else
            {
                fill_id = sliderbar_slider.slider_fill_act_id;
            }

            fill_rect_dump(&fill_region, fill_id, fill_mode);
        }
        else //���ȼ���
        {
            if ((sliderbar_slider.attrib & SLIDER_DIRECTION_OPTION) == DIRECTION_OPTION_HORIZONTAL) //����ˢ��
            {
                fill_region.x = fill_x - (uint16)temp_block;
                fill_region.y = sliderbar_slider.slider_start_y;
                fill_region.width = (uint16)temp_block;
                fill_region.height = 0;
                fill_mode = DUMP_LEFT2RIGHT;
            }
            else
            {
                fill_region.x = fill_x;
                if (prog_direct_flag == 1)
                {
                    fill_region.y = fill_y + 1;
                }
                else
                {
                    fill_region.y = fill_y - (uint16)temp_block;
                }
                fill_region.width = 0;
                fill_region.height = (uint16)temp_block;
                fill_mode = DUMP_DOWN2UP;
            }

            fill_id = sliderbar_slider.slider_unfill_id;
            fill_rect_dump(&fill_region, fill_id, fill_mode);
        }
    }
    
    if(((sliderbar_slider.attrib & SLIDER_SCALE_OPTION) == SCALE_OPTION_SCALE)//�п̶Ƚ�����
    && (prog_flag == 0)) //���ȼ���ʱ���ػ��̶���
    {
        i = tmp_slider_prev_offset - slider_cur_offset;
        if (i < 0)
        {
            i = -i;
        }
        while (i > 0)
        {
            if ((sliderbar_slider.attrib & SLIDER_DIRECTION_OPTION) == DIRECTION_OPTION_HORIZONTAL) //����ˢ��
            {
                u_show_pic(sliderbar_slider.slider_fill_id, fill_x, sliderbar_slider.slider_start_y);
            }
            else
            {
                if (prog_direct_flag == 1)
                {
                    u_show_pic(sliderbar_slider.slider_fill_id, fill_x, fill_y + scale);
                }
                else
                {
                    u_show_pic(sliderbar_slider.slider_fill_id, fill_x, fill_y);
                }
            }
            if (tmp_slider_prev_offset < 0)
            {
                fill_y -= scale;
            }
            else
            {
                fill_y += scale;
            }
            i = (int16) (i - scale);
        }
    }
    
    //ֻ�е�ǰ����ֵ�Ż���� prev ֵ
    if (type == 1)
    {
        slider_prev_offset = slider_cur_offset;
    }
}
/*! \endcond */
