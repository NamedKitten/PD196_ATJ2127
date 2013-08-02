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
 * \file     ui_show_sliderbar_3.c
 * \brief    ��ʾsliderbar��title�� unit��ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ��title�� unit��ؼ����ݵĽ�������ʾtitle�� unit
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���ϵͳSD�ļ�ϵͳ�Ľӿں�����������ʾtitle�� unit
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

#define SLIDERBAR_LISTDIRECT_OPTION_SHIFT    (5)
#define SLIDERBAR_LISTDIRECT_OPTION      (0x0001 << SLIDERBAR_LISTDIRECT_OPTION_SHIFT)
#define SLIDERBAR_LISTDIRECT_HORIZONTAL  (0x0000 << SLIDERBAR_LISTDIRECT_OPTION_SHIFT)
#define SLIDERBAR_LISTDIRECT_VERTICAL    (0x0001 << SLIDERBAR_LISTDIRECT_OPTION_SHIFT)

#define TITLE_ALIGNMENT_OPTION_SHIFT    (0)
#define TITLE_ALIGNMENT_OPTION          (0x0003 << TITLE_ALIGNMENT_OPTION_SHIFT)
#define TITLE_ALIGNMENT_OPTION_LEFT     (0x0000 << TITLE_ALIGNMENT_OPTION_SHIFT)
#define TITLE_ALIGNMENT_OPTION_CENTER   (0x0001 << TITLE_ALIGNMENT_OPTION_SHIFT)
#define TITLE_ALIGNMENT_OPTION_RIGHT    (0x0002 << TITLE_ALIGNMENT_OPTION_SHIFT)
#define TITLE_FONT_OPTION_SHIFT         (2)
#define TITLE_FONT_OPTION               (0x0003 << TITLE_FONT_OPTION_SHIFT)
#define TITLE_FONT_OPTION_SMALL         (0x0000 << TITLE_FONT_OPTION_SHIFT)
#define TITLE_FONT_OPTION_MIDDLE        (0x0001 << TITLE_FONT_OPTION_SHIFT)
#define TITLE_FONT_OPTION_LARGE         (0x0002<< TITLE_FONT_OPTION_SHIFT)

#define UNIT_ALIGNMENT_OPTION_SHIFT     (0)
#define UNIT_ALIGNMENT_OPTION           (0x0003 << UNIT_ALIGNMENT_OPTION_SHIFT)
#define UNIT_ALIGNMENT_OPTION_LEFT      (0x0000 << UNIT_ALIGNMENT_OPTION_SHIFT)
#define UNIT_ALIGNMENT_OPTION_CENTER    (0x0001 << UNIT_ALIGNMENT_OPTION_SHIFT)
#define UNIT_ALIGNMENT_OPTION_RIGHT     (0x0002 << UNIT_ALIGNMENT_OPTION_SHIFT)
#define UNIT_FONT_OPTION_SHIFT          (2)
#define UNIT_FONT_OPTION                (0x0003 << UNIT_FONT_OPTION_SHIFT)
#define UNIT_FONT_OPTION_SMALL          (0x0000 << UNIT_FONT_OPTION_SHIFT)
#define UNIT_FONT_OPTION_MIDDLE         (0x0001 << UNIT_FONT_OPTION_SHIFT)
#define UNIT_FONT_OPTION_LARGE          (0x0002 << UNIT_FONT_OPTION_SHIFT)
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾsliderbar��title
 * \param[in]   addr��sliderbar��title����Դ�ļ��е�ƫ�Ƶ�ַ
 * \param[in]   title_id��Ӧ�ô��ݵ�title��ԴID
 * \param[in]   style_ptr����ǰ��Դ��Ϣ�ṹ��ָ��
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_sliderbar_title(uint32 addr, uint16 title_id, res_open_infor_t *style_ptr)
{
    sliderbar_title_t sliderbar_title;
    region_t title_region;
    string_desc_t desc;
    uint16 id;
    uint16 align_mode = 0;

    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, addr);
    sys_sd_fread(style_ptr->res_fp, &sliderbar_title, sizeof(sliderbar_title_t));
    //atrib<2-3>0��ʾС����,1��ʾ�к����壬2��ʾ�����壬Ĭ���к�����
    if ((sliderbar_title.attrib & TITLE_FONT_OPTION) == TITLE_FONT_OPTION_SMALL)
    {
        align_mode |= DISP_FONT_SMALL;
    }
    else if ((sliderbar_title.attrib & TITLE_FONT_OPTION) == TITLE_FONT_OPTION_LARGE)
    {
        align_mode |= DISP_FONT_LARGE;
    }
    else
    {
        align_mode |= DISP_FONT_NORMAL; //Ĭ���к�����
    }

    romf_set_pen_color(sliderbar_title.color, 0, 0);
    //��ʾ��������
    title_region.x = sliderbar_title.title_x;
    title_region.y = sliderbar_title.title_y;
    title_region.width = sliderbar_title.title_width;
    title_region.height = sliderbar_title.title_height;

    if (title_id != (uint16) - 1)
    {
        id = title_id;
    }
    else if (sliderbar_title.title_id != (uint16) - 1)
    {
        id = sliderbar_title.title_id;
    }
    else
    {
        id = (uint16) - 1;
    }
    //������ʾ
    if ((sliderbar_title.attrib & TITLE_ALIGNMENT_OPTION) == TITLE_ALIGNMENT_OPTION_LEFT)
    {
        align_mode |= DISP_ALIGN_LEFT;
    }
    //������ʾ
    else if ((sliderbar_title.attrib & TITLE_ALIGNMENT_OPTION) == TITLE_ALIGNMENT_OPTION_RIGHT)
    {
        align_mode |= DISP_ALIGN_RIGHT;
    }
    //Ĭ�Ͼ�����ʾ
    else
    {
        align_mode |= DISP_ALIGN_MEDIACY;
    }

    desc.data.id = id;
    desc.language = UNICODEID;
    show_string(&desc, &title_region, align_mode); //Ĭ�Ͼ��ж���
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾsliderbar��unit
 * \param[in]   sliderbar��sliderbar�ؼ����ݽṹ�����
 * \param[in]   parambox_data��Ӧ�ô��ݵ�sliderbar��˽�����ݽṹ��ָ��
 * \param[in]   itemnum���ڼ���������
 * \param[in]   style_ptr����ǰ��Դ��Ϣ�ṹ��ָ��
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_sliderbar_unit(sliderbar_t *sliderbar, parambox_private_t *parambox_data, uint16 itemnum,
        res_open_infor_t *style_ptr)
{
    sliderbar_unit_t sliderbar_unit;
    region_t unit_region;
    uint16 align_mode = 0;
    uint16 one_width = sliderbar->list_width / sliderbar->param_cnt;
    uint16 one_height = sliderbar->list_height / sliderbar->param_cnt;

    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, sliderbar->unit_addr);
    sys_sd_fread(style_ptr->res_fp, &sliderbar_unit, sizeof(sliderbar_unit_t));
    //set_textout_mode(0,0,0); //Ĭ��Ϊ͸��ģʽ
    romf_set_pen_color(sliderbar_unit.uint_color, 0, 0);
    //��ʾ��������
    if(sliderbar->param_cnt > 1)//�����
    {
        if((sliderbar->attrib & SLIDERBAR_LISTDIRECT_OPTION) == SLIDERBAR_LISTDIRECT_HORIZONTAL)//��������
        {
            unit_region.x = sliderbar->list_x + (one_width * itemnum);
            unit_region.y = sliderbar_unit.unit_y;
            unit_region.width = one_width;
            unit_region.height = sliderbar_unit.unit_height;
        }
        else//��������
        {
            unit_region.x = sliderbar_unit.unit_x;
            unit_region.y = sliderbar->list_y + (one_height * itemnum);
            unit_region.width = sliderbar_unit.unit_width;
            unit_region.height = one_height;
        }
    }
    else//������
    {
        unit_region.x = sliderbar_unit.unit_x;
        unit_region.y = sliderbar_unit.unit_y;
        unit_region.width = sliderbar_unit.unit_width;
        unit_region.height = sliderbar_unit.unit_height;
    }
    //atrib<11-12>0��ʾС����,1��ʾ�к����壬2��ʾ�����壬Ĭ���к�����
    if ((sliderbar_unit.attrib & UNIT_FONT_OPTION) == UNIT_FONT_OPTION_SMALL)
    {
        align_mode |= DISP_FONT_SMALL;
    }
    else if ((sliderbar_unit.attrib & UNIT_FONT_OPTION) == UNIT_FONT_OPTION_LARGE)
    {
        align_mode |= DISP_FONT_LARGE;
    }
    else
    {
        align_mode |= DISP_FONT_NORMAL; //Ĭ���к�����
    }
    //set_font_size(font_size);
    //������ʾ
    if ((sliderbar_unit.attrib & UNIT_ALIGNMENT_OPTION) == UNIT_ALIGNMENT_OPTION_LEFT)
    {
        align_mode |= DISP_ALIGN_LEFT;
    }
    //������ʾ
    else if ((sliderbar_unit.attrib & UNIT_ALIGNMENT_OPTION) == UNIT_ALIGNMENT_OPTION_RIGHT)
    {
        align_mode |= DISP_ALIGN_RIGHT;
    }
    //Ĭ�Ͼ�����ʾ
    else
    {
        align_mode |= DISP_ALIGN_MEDIACY;
    }
    align_mode |= DISP_ARABIC_DISABLE;
    if (parambox_data->items[itemnum].unit_id != (uint16) - 1)
    {
        string_desc_t desc;

        desc.data.id = parambox_data->items[itemnum].unit_id;
        desc.language = UNICODEID;
        show_string(&desc, &unit_region, align_mode); //Ĭ�Ͼ��ж���
    }
}
/*! \endcond */
