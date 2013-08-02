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
 * \file     ui_get_textbox_attrb.c
 * \brief    ��ȡtextbox����ʾ����
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ����textbox����ʾ����Ļ�ȡ��
 * \par      EXTERNALIZED FUNCTIONS:
 *               ֱ�ӵ���sd�ļ�ϵͳ�ӿں�����ȡtextbox����ʾ������Ϣ��
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡtextbox��������Ϣ
 * \param[in]    textbox_style ָ��textbox��style�ṹ�����
 * \param[in]    type Ҫ��ȡ��������Ϣ������
 * \param[out]   attrb ����textbox��������Ϣ
 * \return       ui_result_e ���͵Ĳ������μ�ui_result_e����
 * \ingroup      ui_controls
 * \par          exmaple code
 * \code
    ����1����ȡ textbox �ؼ���ʾ����
    style_infor_t style_infor;
    region_t textbox_region;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    ui_get_textbox_attrb(&style_infor, &textbox_region, 0);
    
    ����2����ȡ textbox �ؼ����ԣ����������С��
    style_infor_t style_infor;
    uint16 attrib;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    ui_get_textbox_attrb(&style_infor, &attrib, 1);
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e get_textbox_attrb(style_infor_t *textbox_style, void *attrb, uint8 type)
{
    textbox_t textbox;
    res_open_infor_t *res_ptr;
    uint32 textbox_entry_addr;
    
    /*���ݴ���Դ�ļ������ͣ���ָ��ָ��ͬ��ȫ�ֱ���*/
    if (textbox_style->type == UI_AP)
    {
        res_ptr = &g_ap_res;
    }
    else
    {
        res_ptr = &g_com_res;
    }
    get_ctrl_addr(TEXTBOX, &textbox_entry_addr, res_ptr);
    //��ȡ��ǰtextbox��style��Ϣ
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, textbox_entry_addr + (uint32) (textbox_style->style_id & 0x0fff)
            * sizeof(textbox_t));
    sys_sd_fread(res_ptr->res_fp, &textbox, sizeof(textbox_t));
    
    if (type == 0)//��ȡ textbox ��ʾ����
    {
        region_t *tmp_region = (region_t *) attrb;
        
        tmp_region->x = textbox.x;
        tmp_region->y = textbox.y;
        tmp_region->width = textbox.width;
        tmp_region->height = textbox.height;
    }
    else if(type == 1)//��ȡtextbox�ؼ�����
    {
        uint16 *attrib = (uint16 *) attrb;
        
        *attrib = textbox.attrib;
    }
    else//��������
    {
        return UI_PARAM_ERR;
    }
    
    return UI_NO_ERR;
}
/*! \endcond */
