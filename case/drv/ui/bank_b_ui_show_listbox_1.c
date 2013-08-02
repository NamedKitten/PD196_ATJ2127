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
 * \file     ui_showlistbox_1.c
 * \brief    display  listbox ratio
 * \author   betty
 * \par      GENERAL DESCRIPTION:
 *               ʵ��lsitbox ratio�ؼ����ݵĽ�������ʾlistbox ratio
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���ϵͳSD�ļ�ϵͳ�Ľӿں�����������ʾlistbox ratio
 * \version 1.0
 * \date  2011/09/07
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//listbox ratio attribute bits location
#define RATIO_DISP_OPTION_SHIFT     (0)
#define RATIO_DISP_OPTION           (0x0001 << RATIO_DISP_OPTION_SHIFT)
#define DISP_OPTION_CHAR            (0x0000 << RATIO_DISP_OPTION_SHIFT)
#define DISP_OPTION_PIC             (0x0001 << RATIO_DISP_OPTION_SHIFT)
#define RATIO_ALIGNMENT_OPTION_SHIFT    (1)
#define RATIO_ALIGNMENT_OPTION      (0x0003 << RATIO_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_LEFT       (0x0000 << RATIO_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_CENTER     (0x0001 << RATIO_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_RIGHT      (0x0002 << RATIO_ALIGNMENT_OPTION_SHIFT)
#define RATIO_INDEX_ZERO_EN         (0x0001 << 3)
#define RATIO_TOTAL_ZERO_EN         (0x0001 << 4)
#define RATIO_BG_OPTION_SHIFT       (5)
#define RATIO_BG_OPTION             (0x0001 << RATIO_BG_OPTION_SHIFT)
#define RATIO_BG_OPTION_COLOR       (0x0000 << RATIO_BG_OPTION_SHIFT)
#define RATIO_BG_OPTION_PIC         (0x0001 << RATIO_BG_OPTION_SHIFT)
#define RATIO_FONT_OPTION_SHIFT     (6)
#define RATIO_FONT_OPTION           (0x0003 << RATIO_FONT_OPTION_SHIFT)
#define RATIO_FONT_OPTION_SMALL     (0x0000 << RATIO_FONT_OPTION_SHIFT)
#define RATIO_FONT_OPTION_MIDDLE    (0x0001 << RATIO_FONT_OPTION_SHIFT)
#define RATIO_FONT_OPTION_LARGE     (0x0002<< RATIO_FONT_OPTION_SHIFT)
const uint8 ratio_partition[2] = { "/" };

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾlistbox ��ratio
 * \param[in]   addr��listbox��slider style��Ϣ����Դ�ļ��е�ƫ�Ƶ�ַ
 * \param[in]   listbox_data��Ӧ�ô��ݵ�listbox��˽�����ݽṹ��ָ��
 * \param[in]   mode��listbox��ˢ��ģʽ
 * \param[in]   style_ptr����ǰ��Դ��Ϣ�ṹ��ָ��
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_list_ratio(uint32 addr, listbox_private_t *listbox_data, list_draw_mode_e mode, res_open_infor_t *style_ptr)
{
    listbox_ratio_t list_ratio;
    uint8 str_index[5];
    uint8 str_total[5];
    uint8 string[11];

    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, addr);
    sys_sd_fread(style_ptr->res_fp, &list_ratio, sizeof(listbox_ratio_t));

    if ((list_ratio.attrib & RATIO_INDEX_ZERO_EN) != RATIO_INDEX_ZERO_EN)
    {
        list_ratio.ratio_index_cnt = 0; //��Ų���Ҫ��0����ʾʵ��λ��
    }
    if ((list_ratio.attrib & RATIO_TOTAL_ZERO_EN) != RATIO_TOTAL_ZERO_EN)
    {
        list_ratio.ratio_total_cnt = 0; //��������Ҫ��0����ʾʵ��λ��
    }
    //������ת���ɶ�Ӧ�ַ�
    libc_memset(string, 0, sizeof(string));
    if (listbox_data->list_total == 0)
    {
        str_index[0] = '0';
        str_index[1] = 0;
        str_total[0] = '0';
        str_total[1] = 0;
    }
    else
    {
        libc_itoa(listbox_data->list_no + 1, str_index, list_ratio.ratio_index_cnt);
        libc_itoa(listbox_data->list_total, str_total, list_ratio.ratio_total_cnt);
    }
    if ((list_ratio.attrib & RATIO_DISP_OPTION) == DISP_OPTION_PIC)
    {
        show_number_param_t number_param;
        numbox_draw_mode_e draw_mode;

        //��ʼ������
        number_param.back_id = list_ratio.back_id;
        number_param.x = list_ratio.back_x;
        number_param.y = list_ratio.back_y;
        number_param.width = list_ratio.ratio_width;
        number_param.num0_id = list_ratio.num0_id;
        number_param.partition_id = list_ratio.partition_id;
        /*number_param.numpic_back_color = list_ratio.numpic_back_color;*/
        number_param.numpic_back_color = 0;//Ĭ��Ϊ��ɫ����
        number_param.num_width = list_ratio.num_width;
        number_param.partition_width = list_ratio.partition_width;
        
        libc_strncpy(string, str_index, libc_strlen(str_index));
        libc_strncat(string, str_total, libc_strlen(str_total));
        number_param.part_pos = (uint8) libc_strlen(str_index);
        
        if ((list_ratio.attrib & RATIO_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_CENTER)
        {
            number_param.align = 1;
        }
        //������ʾ��X����
        else if ((list_ratio.attrib & RATIO_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_RIGHT)
        {
            number_param.align = 2;
        }
        else
        {
            number_param.align = 0;
        }
        
        if (mode == LIST_DRAW_ALL)
        {
            draw_mode = NUMBOX_DRAW_ALL;
        }
        else
        {
            draw_mode = NUMBOX_DRAW_NUMBER;
        }
        
        show_number_pic(string, &number_param, draw_mode);
    }
    //�ַ�����ʾ��ʽ
    else
    {
        region_t ratio_region;
        DC oldDC_sav;
        uint16 textout_mode = 0;

        libc_strncpy(string, str_index, libc_strlen(str_index) + 1);
        libc_strncat(string, ratio_partition, libc_strlen(ratio_partition));
        libc_strncat(string, str_total, libc_strlen(str_total));
        romf_get_DC_status(&oldDC_sav, 0, 0);
        //�����ַ�����ʾ����
        ratio_region.x = list_ratio.ratio_x;
        ratio_region.y = list_ratio.ratio_y;
        ratio_region.width = list_ratio.ratio_width;
        ratio_region.height = list_ratio.ratio_height;
        if ((list_ratio.attrib & RATIO_BG_OPTION) == RATIO_BG_OPTION_PIC)
        {
            textout_mode |= CHAR_FILL_NORMAL; //�б���ͼƬ������͸��ģʽ
            if (list_ratio.back_id != (uint16) - 1)
            {
                u_show_pic(list_ratio.back_id, list_ratio.back_x, list_ratio.back_y);
            }
        }
        else
        {
            textout_mode |= CHAR_FILL_COLOR;//������ɫ�����Ƿ�͸��ģʽ,Ĭ��Ϊ��͸������ģʽ
            romf_set_backgd_color(list_ratio.back_id, 0, 0);
            //clear_screen(&ratio_region, 0, 0);
            romf_fill_rect(&ratio_region, (uint16) (dc.backgd_color & 0xffff));
        }
        //atrib<6-7>0��ʾС����,1��ʾ�к����壬2��ʾ�����壬Ĭ���к�����
        if ((list_ratio.attrib & RATIO_FONT_OPTION) == RATIO_FONT_OPTION_SMALL)
        {
            textout_mode |= DISP_FONT_SMALL;
        }
        else if ((list_ratio.attrib & RATIO_FONT_OPTION) == RATIO_FONT_OPTION_LARGE)
        {
            textout_mode |= DISP_FONT_LARGE;
        }
        else
        {
            textout_mode |= DISP_FONT_NORMAL; //Ĭ���к�����
        }

        //���û�����ɫ
        romf_set_pen_color(list_ratio.text_color, 0, 0);
        //������ʾ
        if ((list_ratio.attrib & RATIO_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_LEFT)
        {
            textout_mode |= DISP_ALIGN_LEFT;
        }
        //������ʾ
        else if ((list_ratio.attrib & RATIO_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_CENTER)
        {
            textout_mode |= DISP_ALIGN_MEDIACY;
        }
        //������ʾ
        else if ((list_ratio.attrib & RATIO_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_RIGHT)
        {
            textout_mode |= DISP_ALIGN_RIGHT;
        }
        else
        {
            //do nothing for QAC
        }
        textout_mode |= DISP_ARABIC_DISABLE;
        {
            string_desc_t desc;
            desc.data.str = string;
            desc.language = ENGLISH;
            desc.length = 11;
            show_string(&desc, &ratio_region, textout_mode);
        }

        //�ָ�������ɫ
        romf_set_pen_color(oldDC_sav.pen_color, 0, 0);
        romf_set_backgd_color(oldDC_sav.backgd_color, 0, 0);
    }
}
/*! \endcond */
