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
 * \file     ui_get_listbox_attrb.c
 * \brief    ��ȡpicbox��������Ϣ
 * \author   betty
 * \par      GENERAL DESCRIPTION:
 *               ʵ��lsitbox�ؼ����ݵĻ�ȡ
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���ϵͳSD�ļ�ϵͳ�Ľӿں�������listbox������
 * \version 1.0
 * \date  2011/09/07
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡlistbox��������Ϣ
 * \param[in]    listbox_style ָ��listbox��style�ṹ�����
 * \param[in]    type Ҫ��ȡ��������Ϣ������
 * \param[out]   attrb ����listbox��������Ϣ
 * \return       ui_result_e ���͵Ĳ������μ�ui_result_e����
 * \ingroup      ui_controls
 * \par          exmaple code
 * \code
    ����1����ȡ listbox �ؼ�ÿҳ����
    style_infor_t style_infor;
    uint8 one_page_count;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    ui_get_listbox_attrb(&style_infor, &one_page_count, 0);
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e get_listbox_attrb(style_infor_t *listbox_style, void *attrb, uint8 type)
{
    listbox_t listbox;
    res_open_infor_t *style_ptr;
    uint32 listbox_entry_addr;

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
    
    if(type == 0)//��ȡ listbox ÿҳ��ʾ����
    {
        *(uint8 *)attrb = listbox.num_one_page;
    }
    else//��������
    {
        return UI_PARAM_ERR;
    }

    return UI_NO_ERR;
}
/*! \endcond */
