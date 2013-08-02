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
 * \file     ui_load_attributebox.c
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ����atrtributebox�ؼ�������������Ϣ�Ļ�ȡ��
 * \par      EXTERNALIZED FUNCTIONS:
 *               ֱ�ӵ���ϵͳsd�ļ�ϵͳ�ӿں�����ȡattributebox����Ϣ��
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡattributebox�����ݽṹ��Ϣ
 * \param[in]    attributebox_style ָ��attributebox��style�ṹ�����
 * \param[in]    count Ҫ��ȡ���ֽ���
 * \param[out]   attributebox_data ����attributebox��������Ϣ
 * \return       ui_result_e ���͵Ĳ������Ƿ�ɹ�
 * \ingroup      ui_controls
 * \par          exmaple code
 * \code
    style_infor_t style_infor;
    my_attrbdata_t my_attrbdata;
    
    style_infor.style_id = ATTRIBBOX_ID1;
    style_infor.type = UI_AP;
    
    ui_load_attributebox(&style_infor, &my_attrbdata, sizeof(my_attrbdata_t));
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e load_attributebox(style_infor_t *attributebox_style, void*attributebox_data, uint16 count)
{
    res_open_infor_t *style_ptr;
    if (attributebox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, \
        style_ptr->cur_style_begin_offset + (uint32) attributebox_style->style_id);
    sys_sd_fread(style_ptr->res_fp, attributebox_data, (uint32) count);
    return UI_NO_ERR;
}
/*! \endcond */
