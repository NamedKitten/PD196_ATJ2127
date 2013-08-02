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
 * \file     ui_showlistbox_2.c
 * \brief   ��ʾlistbox��items
 * \author   betty
 * \par      GENERAL DESCRIPTION:
 *              ����listbox�Ŀؼ����ݽṹ��������ˢ��ģʽ����ʾlistbox items
 * \par      EXTERNALIZED FUNCTIONS:
 *              none
 * \version 1.0
 * \date  2011/09/07
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//listbox items attribute bits location
#define ITEMS_BACK_TAG_INA_OPTION_SHIFT     (0)
#define ITEMS_BACK_TAG_INA_OPTION       (0x0003 << ITEMS_BACK_TAG_INA_OPTION_SHIFT)
#define BACK_TAG_INA_OPTION_NONE        (0x0000 << ITEMS_BACK_TAG_INA_OPTION_SHIFT)
#define BACK_TAG_INA_OPTION_SINGLE      (0x0001 << ITEMS_BACK_TAG_INA_OPTION_SHIFT)
#define BACK_TAG_INA_OPTION_MULTY       (0x0002 << ITEMS_BACK_TAG_INA_OPTION_SHIFT)
#define ITEMS_BACK_TAG_OPTION_SHIFT         (2)
#define ITEMS_BACK_TAG_OPTION           (0x0003 << ITEMS_BACK_TAG_OPTION_SHIFT)
#define BACK_TAG_OPTION_NONE            (0x0000 << ITEMS_BACK_TAG_OPTION_SHIFT)
#define BACK_TAG_OPTION_SINGLE          (0x0001 << ITEMS_BACK_TAG_OPTION_SHIFT)
#define BACK_TAG_OPTION_MULTY           (0x0002 << ITEMS_BACK_TAG_OPTION_SHIFT)
#define ITEMS_FRONT_TAG_INA_OPTION_SHIFT    (4)
#define ITEMS_FRONT_TAG_INA_OPTION      (0x0003 << ITEMS_FRONT_TAG_INA_OPTION_SHIFT)
#define FRONT_TAG_INA_OPTION_NONE       (0x0000 << ITEMS_FRONT_TAG_INA_OPTION_SHIFT)
#define FRONT_TAG_INA_OPTION_SINGLE     (0x0001 << ITEMS_FRONT_TAG_INA_OPTION_SHIFT)
#define FRONT_TAG_INA_OPTION_MULTY      (0x0002 << ITEMS_FRONT_TAG_INA_OPTION_SHIFT)
#define FRONT_TAG_INA_OPTION_NUMBER     (0x0003 << ITEMS_FRONT_TAG_INA_OPTION_SHIFT)
#define ITEMS_FRONT_TAG_OPTION_SHIFT        (6)
#define ITEMS_FRONT_TAG_OPTION          (0x0003 << ITEMS_FRONT_TAG_OPTION_SHIFT)
#define FRONT_TAG_OPTION_NONE           (0x0000 << ITEMS_FRONT_TAG_OPTION_SHIFT)
#define FRONT_TAG_OPTION_SINGLE         (0x0001 << ITEMS_FRONT_TAG_OPTION_SHIFT)
#define FRONT_TAG_OPTION_MULTY          (0x0002 << ITEMS_FRONT_TAG_OPTION_SHIFT)
#define FRONT_TAG_OPTION_NUMBER         (0x0003 << ITEMS_FRONT_TAG_OPTION_SHIFT)
#define ITEMS_INACTIVE_BG_OPTION_SHIFT      (8)
#define ITEMS_INACTIVE_BG_OPTION        (0x0003 << ITEMS_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_COLOR        (0x0000 << ITEMS_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_SPIC         (0x0001 << ITEMS_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_MPIC         (0x0002 << ITEMS_INACTIVE_BG_OPTION_SHIFT)
#define ITEMS_ACTIVE_BG_OPTION_SHIFT        (10)
#define ITEMS_ACTIVE_BG_OPTION          (0x0003 << ITEMS_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_COLOR          (0x0000 << ITEMS_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_SPIC           (0x0001 << ITEMS_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_MPIC           (0x0002 << ITEMS_ACTIVE_BG_OPTION_SHIFT)
#define ITEMS_ACTIVE_FONT_OPTION_SHIFT      (0)
#define ITEMS_ACTIVE_FONT_OPTION        (0x0003 << ITEMS_ACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_ACTIVE_FONT_OPTION_SMALL  (0x0000 << ITEMS_ACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_ACTIVE_FONT_OPTION_MIDDLE (0x0001 << ITEMS_ACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_ACTIVE_FONT_OPTION_LARGE  (0x0002 << ITEMS_ACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_ACTIVE_ALIGNMENT_OPTION_SHIFT (2)
#define ITEMS_ACTIVE_ALIGNMENT_OPTION       (0x0003 << ITEMS_ACTIVE_ALIGNMENT_OPTION_SHIFT)
#define ACTIVE_ALIGNMENT_OPTION_LEFT        (0x0000 << ITEMS_ACTIVE_ALIGNMENT_OPTION_SHIFT)
#define ACTIVE_ALIGNMENT_OPTION_CENTER      (0x0001 << ITEMS_ACTIVE_ALIGNMENT_OPTION_SHIFT)
#define ACTIVE_ALIGNMENT_OPTION_RIGHT       (0x0002 << ITEMS_ACTIVE_ALIGNMENT_OPTION_SHIFT)
#define ITEMS_ACTIVE_DISP_OPTION_SHIFT      (4)
#define ITEMS_ACTIVE_DISP_OPTION            (0x0003 << ITEMS_ACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_ACTIVE_DISP_OPTION_MLINE      (0x0000 << ITEMS_ACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_ACTIVE_DISP_OPTION_SCROLL     (0x0001 << ITEMS_ACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_ACTIVE_DISP_OPTION_NORECT     (0x0002 << ITEMS_ACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_ACTIVE_DISP_OPTION_ELLIPSIS   (0x0003 << ITEMS_ACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_INACTIVE_FONT_OPTION_SHIFT    (6)
#define ITEMS_INACTIVE_FONT_OPTION          (0x0003 << ITEMS_INACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_INACTIVE_FONT_OPTION_SMALL    (0x0000 << ITEMS_INACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_INACTIVE_FONT_OPTION_MIDDLE   (0x0001 << ITEMS_INACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_INACTIVE_FONT_OPTION_LARGE    (0x0002 << ITEMS_INACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_INACTIVE_ALIGNMENT_OPTION_SHIFT   (8)
#define ITEMS_INACTIVE_ALIGNMENT_OPTION     (0x0003 << ITEMS_INACTIVE_ALIGNMENT_OPTION_SHIFT)
#define INACTIVE_ALIGNMENT_OPTION_LEFT      (0x0000 << ITEMS_INACTIVE_ALIGNMENT_OPTION_SHIFT)
#define INACTIVE_ALIGNMENT_OPTION_CENTER    (0x0001 << ITEMS_INACTIVE_ALIGNMENT_OPTION_SHIFT)
#define INACTIVE_ALIGNMENT_OPTION_RIGHT     (0x0002 << ITEMS_INACTIVE_ALIGNMENT_OPTION_SHIFT)
#define ITEMS_INACTIVE_DISP_OPTION_SHIFT    (10)
#define ITEMS_INACTIVE_DISP_OPTION          (0x0003 << ITEMS_INACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_INACTIVE_DISP_OPTION_MLINE    (0x0000 << ITEMS_INACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_INACTIVE_DISP_OPTION_SCROLL   (0x0001 << ITEMS_INACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_INACTIVE_DISP_OPTION_NORECT   (0x0002 << ITEMS_INACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_INACTIVE_DISP_OPTION_ELLIPSIS (0x0003 << ITEMS_INACTIVE_DISP_OPTION_SHIFT)
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾlistbox items
 * \param[in]   listbox��listbox�ṹ�����ָ��
 * \param[in]   listbox_data��Ӧ�ô��ݵ�listbox��˽�����ݽṹ��ָ��
 * \param[in]   itemnum����ǰ���ǵڼ���
 * \param[in]   type����ǰ���Ƿ�Ϊ������
 * \param[in]   style_ptr����ǰ��Դ��Ϣ�ṹ��ָ��
 * \param[out]  none
 * \return      uint8���͵Ĳ������Ƿ���Ҫ����
 * \note
 *******************************************************************************/
uint8 show_list_item(listbox_t *listbox, listbox_private_t *listbox_data, uint16 itemnum, uint8 type,
        res_open_infor_t *style_ptr)
{
    string_desc_t *cur_item = &(listbox_data->items[itemnum]);
    region_t back_region, item_region;
    scroll_mode_t scroll_mode;
    uint16 tmp = 0;
    uint16 disp_mode = 0, textout_mode;
    uint16 head_y, back_y, data_y, tail_y;
    uint16 back_act_id = -1;
    uint8 result;
    uint8 actual_h;
    
    tmp = itemnum % listbox->num_one_page;

    //���㵱ǰ�˵����Y����
    head_y = list_item.head_y + tmp * listbox->inc_y;
    back_y = list_item.back_y + tmp * listbox->inc_y;
    if (type == 1)
    {
        //str_atrib<0-1>0��ʾС����,1��ʾ�к����壬2��ʾ�����壬Ĭ���к�����
        if ((list_item.str_attrib & ITEMS_ACTIVE_FONT_OPTION) == ITEMS_ACTIVE_FONT_OPTION_SMALL)
        {
            disp_mode |= DISP_FONT_SMALL;
        }
        else if ((list_item.str_attrib & ITEMS_ACTIVE_FONT_OPTION) == ITEMS_ACTIVE_FONT_OPTION_LARGE)
        {
            disp_mode |= DISP_FONT_LARGE;
        }
        else
        {
            disp_mode |= DISP_FONT_NORMAL; //Ĭ���к�����
        }
    }
    else
    {
        //str_atrib<6-7>0��ʾС����,1��ʾ�к����壬2��ʾ�����壬Ĭ���к�����
        if ((list_item.str_attrib & ITEMS_INACTIVE_FONT_OPTION) == ITEMS_INACTIVE_FONT_OPTION_SMALL)
        {
            disp_mode |= DISP_FONT_SMALL;
        }
        else if ((list_item.str_attrib & ITEMS_INACTIVE_FONT_OPTION) == ITEMS_INACTIVE_FONT_OPTION_LARGE)
        {
            disp_mode |= DISP_FONT_LARGE;
        }
        else
        {
            disp_mode |= DISP_FONT_NORMAL; //Ĭ���к�����
        }
    }
    actual_h = get_fontlib_height(disp_mode);
    data_y = list_item.back_y + ((listbox->inc_y - actual_h)/2) + tmp * listbox->inc_y;
    tail_y = list_item.tail_y + tmp * listbox->inc_y;

    //��ʾ��������
    back_region.x = list_item.data_x;
    back_region.y = list_item.back_y + tmp * listbox->inc_y;
    back_region.width = list_item.data_width;
    back_region.height = listbox->inc_y;
    
    item_region.x = list_item.data_x;
    item_region.y = data_y;
    item_region.width = list_item.data_width;
    item_region.height = actual_h;
    //��ʾ���
    if (type == 1)
    {
        if ((list_item.attrib & ITEMS_ACTIVE_BG_OPTION) == ACTIVE_BG_OPTION_SPIC)
        {
            textout_mode = CHAR_FILL_NORMAL; //�б���ͼƬ������͸��ģʽ
            if (list_item.back_act_id != (uint16) - 1)
            {
                back_act_id = list_item.back_act_id;
                u_show_pic(list_item.back_act_id, list_item.back_x, back_y);
            }
        }
        else if ((list_item.attrib & ITEMS_ACTIVE_BG_OPTION) == ACTIVE_BG_OPTION_MPIC)
        {
            textout_mode = CHAR_FILL_NORMAL; //�б���ͼƬ������͸��ģʽ
            if (list_item.back_act_id != (uint16) - 1)
            {
                back_act_id = list_item.back_act_id + tmp;
                u_show_pic(list_item.back_act_id + tmp, list_item.back_x, back_y);
            }
        }
        else
        {
            textout_mode = CHAR_FILL_COLOR; //������ɫ�����Ƿ�͸��ģʽ,Ĭ��Ϊ��͸������ģʽ
            back_act_id = list_item.back_act_id;
            romf_set_backgd_color(list_item.back_act_id, 0, 0);
            //clear_screen(&back_region, 0, 0);
            romf_fill_rect(&back_region, (uint16) (dc.backgd_color & 0xffff));
        }
        disp_mode |= textout_mode;

        if (((list_item.attrib & ITEMS_FRONT_TAG_OPTION) != FRONT_TAG_OPTION_NONE) 
         && ((cur_item->argv & ITEMS_FRONT_TAG_ARGV) == ITEMS_FRONT_TAG_IS_DISP))
        {
            if ((list_item.attrib & ITEMS_FRONT_TAG_OPTION) == FRONT_TAG_OPTION_SINGLE)
            {
                u_show_pic(list_item.head_act_id, list_item.head_x, head_y);
            }
            else
            {
                if ((cur_item->argv & ITEMS_FRONT_TAG_INDEX) != ITEMS_FRONT_TAG_INDEX)
                {
                    u_show_pic(listbox_data->head_act_id + \
                    ((cur_item->argv & ITEMS_FRONT_TAG_INDEX) >> ITEMS_FRONT_TAG_INDEX_SHIFT), list_item.head_x, head_y);
                }
                else
                {
                u_show_pic(list_item.head_act_id + itemnum, list_item.head_x, head_y);
                }
            }
        }

        if ((cur_item->argv & ITEMS_BACK_TAG_ARGV) == ITEMS_BACK_TAG_IS_SELECT)
        {
            if ((list_item.attrib & ITEMS_BACK_TAG_OPTION) == BACK_TAG_OPTION_SINGLE)
            {
                u_show_pic(list_item.tail_act_id, list_item.tail_x, tail_y);
            }
            else if ((list_item.attrib & ITEMS_BACK_TAG_OPTION) == BACK_TAG_OPTION_MULTY)
            {
                u_show_pic(list_item.tail_act_id + itemnum, list_item.tail_x, tail_y);
            }
            else
            {
                //do nothing for QAC
            }
        }
        
        romf_set_pen_color(list_item.data_act_color, 0, 0);
        
        //str_attrib<2-3>��ʾ����ַ����������ԣ�0������룬1�����ж��룬2���Ҷ��룬Ĭ��Ϊ�������ʾ��
        if ((list_item.str_attrib & ITEMS_ACTIVE_ALIGNMENT_OPTION) == ACTIVE_ALIGNMENT_OPTION_CENTER)
        {
            disp_mode |= DISP_ALIGN_MEDIACY;
        }
        //else if ((list_item.str_attrib & ITEMS_ACTIVE_ALIGNMENT_OPTION) == ACTIVE_ALIGNMENT_OPTION_RIGHT)
        //{
        //    disp_mode |= DISP_ALIGN_RIGHT;
        //}
        else
        {
            disp_mode |= DISP_ALIGN_LEFT; //Ĭ���������ʾ
        }
        //str_attrib<4-5>��ʾ���ַ�����ʾ���ԣ�0��������ʾ��1��������ʾ��2���ض�����ʾ
        //3��ʡ�Ժ���ʾ��Ĭ��Ϊ������ʾ��
        if ((list_item.str_attrib & ITEMS_ACTIVE_DISP_OPTION) == ITEMS_ACTIVE_DISP_OPTION_MLINE)
        {
            disp_mode |= DISP_LONGSTR_MULTI_ROW;
        }
        else if ((list_item.str_attrib & ITEMS_ACTIVE_DISP_OPTION) == ITEMS_ACTIVE_DISP_OPTION_NORECT)
        {
            disp_mode |= DISP_LONGSTR_NO_RECT;
        }
        else if ((list_item.str_attrib & ITEMS_ACTIVE_DISP_OPTION) == ITEMS_ACTIVE_DISP_OPTION_ELLIPSIS)
        {
            disp_mode |= DISP_LONGSTR_ELLIPSIS;
        }
        else if ((list_item.str_attrib & ITEMS_ACTIVE_DISP_OPTION) == ITEMS_ACTIVE_DISP_OPTION_SCROLL)
        {
            disp_mode |= DISP_LONGSTR_SCROLL;//Ĭ�Ϲ�����ʾ
        }
        else
        {
            disp_mode |= DISP_LONGSTR_RECT;
        }
    }
    //�ǻ��
    else
    {
        if ((list_item.attrib & ITEMS_INACTIVE_BG_OPTION) == INACTIVE_BG_OPTION_SPIC)
        {
            textout_mode = CHAR_FILL_NORMAL; //�б���ͼƬ������͸��ģʽ
            if (list_item.back_id != (uint16) - 1)
            {
                u_show_pic(list_item.back_id, list_item.back_x, back_y);
            }
        }
        else if ((list_item.attrib & ITEMS_INACTIVE_BG_OPTION) == INACTIVE_BG_OPTION_MPIC)
        {
            textout_mode = CHAR_FILL_NORMAL; //�б���ͼƬ������͸��ģʽ
            if (list_item.back_id != (uint16) - 1)
            {
                u_show_pic(list_item.back_id + tmp, list_item.back_x, back_y);
            }
        }
        else
        {
            textout_mode = CHAR_FILL_COLOR; //������ɫ�����Ƿ�͸��ģʽ,Ĭ��Ϊ��͸������ģʽ
            romf_set_backgd_color(list_item.back_id, 0, 0);
            //clear_screen(&back_region, 0, 0);
            romf_fill_rect(&back_region, (uint16) (dc.backgd_color & 0xffff));
        }
        disp_mode |= textout_mode;

        if (type == 0xff)//���������
        {
            return UI_NO_ERR;
        }
        
        if (((list_item.attrib & ITEMS_FRONT_TAG_INA_OPTION) != FRONT_TAG_INA_OPTION_NONE) 
         && ((cur_item->argv & ITEMS_FRONT_TAG_ARGV) == ITEMS_FRONT_TAG_IS_DISP))
        {
            if ((list_item.attrib & ITEMS_FRONT_TAG_INA_OPTION) == FRONT_TAG_INA_OPTION_SINGLE)
            {
                u_show_pic(list_item.head_id, list_item.head_x, head_y);
            }
            else
            {
                if ((cur_item->argv & ITEMS_FRONT_TAG_INDEX) != ITEMS_FRONT_TAG_INDEX)
                {
                    u_show_pic(listbox_data->head_id + \
                    ((cur_item->argv & ITEMS_FRONT_TAG_INDEX) >> ITEMS_FRONT_TAG_INDEX_SHIFT), list_item.head_x, head_y);
                }
                else
                {
                    u_show_pic(list_item.head_id + itemnum, list_item.head_x, head_y);
                }
            }
        }

        if ((cur_item->argv & ITEMS_BACK_TAG_ARGV) == ITEMS_BACK_TAG_IS_SELECT)
        {
            if ((list_item.attrib & ITEMS_BACK_TAG_INA_OPTION) == BACK_TAG_INA_OPTION_SINGLE)
            {
                u_show_pic(list_item.tail_id, list_item.tail_x, tail_y);
            }
            else if ((list_item.attrib & ITEMS_BACK_TAG_INA_OPTION) == BACK_TAG_INA_OPTION_MULTY)
            {
                u_show_pic(list_item.tail_id + itemnum, list_item.tail_x, tail_y);
            }
            else
            {
                //do nothing for QAC
            }
        }
        romf_set_pen_color(list_item.data_color, 0, 0);
        
        //str_attrib<8-9>��ʾ�ǻ���ַ����������ԣ�0������룬1�����ж��룬2���Ҷ��룬Ĭ��Ϊ�������ʾ��
        if ((list_item.str_attrib & ITEMS_INACTIVE_ALIGNMENT_OPTION) == INACTIVE_ALIGNMENT_OPTION_CENTER)
        {
            disp_mode |= DISP_ALIGN_MEDIACY;
        }
        //else if ((list_item.str_attrib & ITEMS_INACTIVE_ALIGNMENT_OPTION) == INACTIVE_ALIGNMENT_OPTION_RIGHT)
        //{
        //    disp_mode |= DISP_ALIGN_RIGHT;
        //}
        else
        {
            disp_mode |= DISP_ALIGN_LEFT; //Ĭ���������ʾ
        }

        //str_attrib<10-11>��ʾ�ǻ��ַ�����ʾ���ԣ�0��������ʾ��1��������ʾ��2���ض�����ʾ
        //3��ʡ�Ժ���ʾ��Ĭ��Ϊ������ʾ��
        if ((list_item.str_attrib & ITEMS_INACTIVE_DISP_OPTION) == ITEMS_INACTIVE_DISP_OPTION_MLINE)
        {
            disp_mode |= DISP_LONGSTR_MULTI_ROW;
        }
        else if ((list_item.str_attrib & ITEMS_INACTIVE_DISP_OPTION) == ITEMS_INACTIVE_DISP_OPTION_NORECT)
        {
            disp_mode |= DISP_LONGSTR_NO_RECT;
        }
        else if ((list_item.str_attrib & ITEMS_INACTIVE_DISP_OPTION) == ITEMS_INACTIVE_DISP_OPTION_SCROLL)
        {
            disp_mode |= DISP_LONGSTR_SCROLL;
        }
        else if ((list_item.str_attrib & ITEMS_INACTIVE_DISP_OPTION) == ITEMS_INACTIVE_DISP_OPTION_ELLIPSIS)
        {
            disp_mode |= DISP_LONGSTR_ELLIPSIS; //Ĭ��ʡ�Ժ���ʾ
        }
        else
        {
            disp_mode |= DISP_LONGSTR_RECT;
        }
    }
    
    //����ѡ��˵��У��˵����ַ������⴦��
    if((lang_option & LANG_LIST_MENU_OPTION) == LANG_LIST_MENU_ENTER)
    {
        disp_mode |= DISP_LANG_LIST_MENU;
    }
    
    result = show_string(cur_item, &item_region, disp_mode);
    if ((result == UI_NEED_SCROLL) && (type == 1))
    {
        if(textout_mode != CHAR_FILL_COLOR)//͸��ģʽ���б���ͼƬ
        {
            //���ز���ʼ������ͼƬ
            load_bgpic_sd(back_act_id, list_item.back_x, back_y, 1);
            disp_mode &= ~DISP_CHAR_FILL_OPTION;
            disp_mode |= CHAR_FILL_SDIMAGE;
        }
        
        //��ʼ���ַ��������������������ַ���ĩβ���2�ո�
        scroll_mode.mode = TRUE;
        scroll_mode.disp_mode = disp_mode;
        scroll_desc.back_id = back_act_id;//�����ڼ�¼������ɫ
        //scroll_desc.back_x = list_item.back_x;//��ʱû��
        //scroll_desc.back_y = back_y;//��ʱû��
        scroll_desc.pen_color = list_item.data_act_color;
        scroll_string(&scroll_str, &item_region, scroll_mode);
    }
    return result;
}
/*! \endcond */
