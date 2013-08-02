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
 * \file     ui_res_open.c
 * \brief    ��Դ�ļ��Ĳ���ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ����Դ�ļ��Ĵ򿪡��رռ����ݲ��ҵȹ���
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���ϵͳSD�ļ�ϵͳ�Ľӿں���������Դ�ļ���
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

void get_styctrl_offset(uint16 style_num, uint32 style_addr, res_open_infor_t *style_ptr);
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ����Դ�ļ�
 * \param[in]    filename����Դ�ļ�������
 * \param[in]    type����Դ���ͣ�AP��COMMON��Դ
 * \param[out]   none
 * \return       bool���͵Ĳ���������Դ�ļ��Ƿ�ɹ�
 * \retval       TRUE ����Դ�ļ��ɹ���
 * \retval       FALSE ����Դ�ļ�ʧ�ܡ�
 * \ingroup      res_manager
 * \par          exmaple code
 * \code   
    ����1���� common.sty��Ȼ��Ϳ���ʹ�� common ���̿ؼ���ͼƬ���ַ�����Դ
    ui_res_open("common.sty", UI_COM);
    
    ����2���� music.sty��Ȼ��Ϳ���ʹ�� music ���̿ؼ���ͼƬ���ַ�����Դ
    ui_res_open("music.sty", UI_AP);
 * \endcode
 * \note
 *******************************************************************************/
bool res_open(char *filename, res_type_e type, void *null3)
{
    res_head_t res_head;
    res_open_infor_t *res_ptr;

    /*���ݴ���Դ�ļ������ͣ���ָ��ָ��ͬ��ȫ�ֱ���*/
    if (type == UI_AP)
    {
        res_ptr = &g_ap_res;
    }
    else
    {
        res_ptr = &g_com_res;
    }

    res_ptr->res_fp = sys_sd_fopen(filename);
    if (res_ptr->res_fp == NULL)
    {
        return FALSE;
    }
    sys_sd_fread(res_ptr->res_fp, &res_head, sizeof(res_head_t));
    /*�ж��ļ���ʽ�Ƿ�Ϸ�*/
    if ((res_head.magic[0] != 'R') || (res_head.magic[1] != 'U') 
     || (res_head.magic[2] != '3') || (res_head.magic[3] != 0x19))
    {
        sys_sd_fclose(res_ptr->res_fp);
        return FALSE;
    }
    /*��Դ�ļ�Ϊap������*/
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, RESHEADITEM);
    sys_sd_fread(res_ptr->res_fp, &res_ptr->pic_entry_offset, sizeof(res_ptr->pic_entry_offset));

    /*��ȡ�������������entry��ַ*/
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, RESHEADITEM * 2);
    sys_sd_fread(res_ptr->res_fp, &res_ptr->string_entry_offset, sizeof(res_ptr->string_entry_offset));

    //��ȡ��ǰ�������͵�����entry��ַ
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->string_entry_offset + (uint32) language_id * RESHEADITEM);
    sys_sd_fread(res_ptr->res_fp, &res_ptr->string_entry_offset, sizeof(res_ptr->string_entry_offset));
    //��ȡstyle�ļ�������ƫ�Ƶ�ַ
    get_styctrl_offset(0, res_head.style_addr, res_ptr);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �ر���Դ�ļ�
 * \param[in]   type����Դ���ͣ�AP��COMMON��Դ
 * \param[out]  none
 * \return      none
 * \ingroup     res_manager
 * \par         exmaple code
 * \code   
    ����1���رմ��˵� common.sty 
    ui_res_close(UI_COM);
    
    ����2���رմ��˵� ap ���̣����� music.sty 
    ui_res_close(UI_AP);
 * \endcode
 * \note
 *******************************************************************************/
void res_close(res_type_e type, void *null2, void *null3)
{
    if (type == UI_AP)
    {
        sys_sd_fclose(g_ap_res.res_fp);
    }
    else
    {
        sys_sd_fclose(g_com_res.res_fp);
    }
}
/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡpicbox��textbox�����ؼ����ļ��е���ʼ��ַ
 * \param[in]    style_num����ǰstyle��
 * \param[in]    style_ptr����ǰ��Դ��Ϣ�ṹ��ָ��
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void get_styctrl_offset(uint16 style_num, uint32 style_addr, res_open_infor_t *style_ptr)
{
    uint32 style_count;
    //��ȡstyle������
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, style_addr + 4);
    sys_sd_fread(style_ptr->res_fp, &style_count, sizeof(uint32));
    if (style_num < style_count)
    {
        //��ȡ��ǰ������ʼ��ַ
        sys_sd_fseek(style_ptr->res_fp, SEEK_SET, (uint32) (style_addr + 8 + style_num * sizeof(style_t)));
        sys_sd_fread(style_ptr->res_fp, &style_ptr->cur_style_begin_offset, sizeof(uint32));
    }
    //��ȡ��ǰ���picbox����ʼ��ַ
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, (uint32) (style_ptr->cur_style_begin_offset + PICBOX * 8));
    sys_sd_fread(style_ptr->res_fp, &style_ptr->style_picbox_offset, sizeof(uint32));
}
/*! \endcond */
