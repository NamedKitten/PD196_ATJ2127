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
 * \file    ui_show_timebox.c
 * \brief    ��ʾtimebox�ĺ���ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ��timebox�ؼ����ݵĽ�������ʾtimebox
 * \par      EXTERNALIZED FUNCTIONS:
 *               ֱ�ӵ���ϵͳSD�ļ�ϵͳ�Ľӿں�����������ʾtimebox
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//timebox attribute bits location
#define TIMEBOX_SHOW_EN                 (0x0001 << 0)
#define TIMEBOX_DISP_OPTION_SHIFT       (1)
#define TIMEBOX_DISP_OPTION             (0x0001 << TIMEBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_CHAR                (0x0000 << TIMEBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_PIC                 (0x0001 << TIMEBOX_DISP_OPTION_SHIFT)
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾtimebox
 * \param[in]   timebox_style ָ��timebox��style�ṹ�����
 * \param[in]   timebox_data Ӧ�ô��ݵ�timebox��˽�����ݽṹ��ָ��
 * \param[in]   mode timebox��ˢ�·�ʽ���μ�timebox_draw_mode_e����
 * \param[out]  none
 * \return      ui_result_e ���͵Ĳ������μ�ui_result_e����
 * \ingroup     ui_controls
 * \par         exmaple code
 * \code
    ����1����ʾ HH:MM:SS/HH:MM:SS��timebox ��������Ϊ��Ӧģʽ
    style_infor_t timebox_style;
    timebox_private_t timebox_param;
    time_timebox_t *tmp_time;
    
    timebox_style.style_id = TIMEBOX_ID1;
    timebox_style.type = UI_AP;
    
    timebox_param.times[0].time_type = 'T';
    tmp_time = &(timebox_param.times[0].union_time.time);
    tmp_time->hour = g_cur_time.hour;
    tmp_time->minute = g_cur_time.minute;
    tmp_time->second = g_cur_time.second;
    timebox_param.times[0].mode = TIME_DISP_HHMMSS;
    timebox_param.times[1].time_type = 'T';
    tmp_time = &(timebox_param.times[1].union_time.time);
    tmp_time->hour = g_total_time.hour;
    tmp_time->minute = g_total_time.minute;
    tmp_time->second = g_total_time.second;
    timebox_param.times[1].mode = TIME_DISP_HHMMSS;
    
    ui_show_timebox(&timebox_style, &timebox_param, TIMEBOX_DRAW_ALL);
    
    ����2����������ʾ��󣬺�������һ��ֻ������ĳ�������ַ����仯��ģʽӦ��Ϊ TIMEBOX_DRAW_TIME
    ��������һ�������ģʽΪ TIMEBOX_DRAW_TIME
    ui_show_timebox(&timebox_style, &timebox_param, TIMEBOX_DRAW_TIME);
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e show_timebox(style_infor_t *timebox_style, timebox_private_t *timebox_data, timebox_draw_mode_e mode)
{
    timebox_t timebox;
    uint32 timebox_entry_addr;
    res_open_infor_t *style_ptr;
    if (timebox_data == NULL)
    {
        return UI_PARAM_ERR;
    }
    /*���ݴ���Դ�ļ������ͣ���ָ��ָ��ͬ��ȫ�ֱ���*/
    if (timebox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    if (timebox_sty_id != timebox_style->style_id)
    {
        time_bits = 0;
    }
    timebox_sty_id = timebox_style->style_id;
    get_ctrl_addr(TIMEBOX, &timebox_entry_addr, style_ptr); //��ȡtimebox�ؼ���UI�����ļ��еĿ�ʼλ��
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, timebox_entry_addr + (uint32) (timebox_style->style_id & 0x0fff)
            * sizeof(timebox_t));
    sys_sd_fread(style_ptr->res_fp, &timebox, sizeof(timebox_t));
    //����ʾtimebox
    if ((timebox.attrib & TIMEBOX_SHOW_EN) != TIMEBOX_SHOW_EN)
    {
        return UI_NO_DISPLAY;
    }
    //������ͼƬ��ʽ��ʾtimebox
    if ((timebox.attrib & TIMEBOX_DISP_OPTION) == DISP_OPTION_PIC)
    {
        show_timebox_pic(timebox_data, &timebox, mode);
    }
    //�ַ�����ʾtimebox
    else
    {
        show_timebox_string(timebox_data, &timebox, mode);
    }
    return UI_NO_ERR;
}
/*! \endcond */
