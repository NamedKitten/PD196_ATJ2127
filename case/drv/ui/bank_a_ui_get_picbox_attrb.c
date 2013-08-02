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
 * \file     ui_get_picbox_attrb.c
 * \brief    ��ȡpicbox��������Ϣ
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ����picbox��������Ϣ��ȡ��
 * \par      EXTERNALIZED FUNCTIONS:
 *               ֱ�ӵ���sd�ļ�ϵͳ�ӿں�����ȡpicbox��������Ϣ��
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡpicbox��������Ϣ
 * \param[in]    picbox_style ָ��picbox��style�ṹ�����
 * \param[in]    type Ҫ��ȡ��������Ϣ������
 * \param[out]   attrb ����picbox��������Ϣ
 * \return       ui_result_e ���͵Ĳ������μ�ui_result_e����
 * \ingroup      ui_controls
 * \par          exmaple code
 * \code
    ����1����ȡ picbox �ؼ���ʾ����
    style_infor_t style_infor;
    region_t pic_region;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    ui_get_picbox_attrb(&style_infor, &pic_region, 0);//type��0����ʾ��ʾ����
    
    ����2����ȡ picbox �ؼ�֡��
    style_infor_t style_infor;
    uint8 frame_cnt;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    ui_get_picbox_attrb(&style_infor, &frame_cnt, 1);//type��1����ʾ֡��
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e get_picbox_attrb(style_infor_t *picbox_style, void *attrb, uint8 type)
{
    picbox_t picbox;
    res_open_infor_t *style_ptr;

    /*���ݴ���Դ�ļ������ͣ���ָ��ָ��ͬ��ȫ�ֱ���*/
    if (picbox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, style_ptr->style_picbox_offset + (uint32) (picbox_style->style_id
            & 0x0fff) * sizeof(picbox_t));
    sys_sd_fread(style_ptr->res_fp, &picbox, sizeof(picbox_t));

    switch (type)
    {
        case 0://��ȡ��ʾ����
        {
            region_t *picbox_region = (region_t *) attrb;

            picbox_region->x = picbox.x;
            picbox_region->y = picbox.y;
            break;
        }

        case 1://��ȡpicbox֡��
        {
            uint8 *frame_cnt = (uint8 *) attrb;

            *frame_cnt = (uint8)(((picbox.attrib) >> 8) & 0xff);
            break;
        }

        default://��������
        {
            return UI_PARAM_ERR;
        }
    }

    return UI_NO_ERR;
}
/*! \endcond */
