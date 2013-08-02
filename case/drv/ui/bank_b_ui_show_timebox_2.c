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
 * \file    ui_show_timebox_2.c
 * \brief    �ַ�����ʾtimebox�ĺ���ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ���ַ�����ʽ��timebox����ʾ
 * \par      EXTERNALIZED FUNCTIONS:
 *               show_num_pic��show_pic
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//�ַ���ģʽ�ָ���
const char partition_id[4] = {':', '/', '-', ' '};

//timebox attribute bits location
#define TIMEBOX_DISP_OPTION_SHIFT       (1)
#define TIMEBOX_DISP_OPTION             (0x0001 << TIMEBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_CHAR                (0x0000 << TIMEBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_PIC                 (0x0001 << TIMEBOX_DISP_OPTION_SHIFT)
#define TIMEBOX_ALIGNMENT_OPTION_SHIFT  (2)
#define TIMEBOX_ALIGNMENT_OPTION        (0x0003 << TIMEBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_LEFT           (0x0000 << TIMEBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_CENTER         (0x0001 << TIMEBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_RIGHT          (0x0002 << TIMEBOX_ALIGNMENT_OPTION_SHIFT)
#define TIMEBOX_COUNT_OPTION_SHIFT      (4)
#define TIMEBOX_COUNT_OPTION            (0x0001 << TIMEBOX_COUNT_OPTION_SHIFT)
#define COUNT_OPTION_ONE                (0x0000 << TIMEBOX_COUNT_OPTION_SHIFT)
#define COUNT_OPTION_TWO                (0x0001 << TIMEBOX_COUNT_OPTION_SHIFT)

#define TIMEBOX_BG_OPTION_SHIFT         (13)
#define TIMEBOX_BG_OPTION               (0x0001 << TIMEBOX_BG_OPTION_SHIFT)
#define TIMEBOX_BG_OPTION_COLOR         (0x0000 << TIMEBOX_BG_OPTION_SHIFT)
#define TIMEBOX_BG_OPTION_PIC           (0x0001 << TIMEBOX_BG_OPTION_SHIFT)
#define TIMEBOX_FONT_OPTION_SHIFT       (14)
#define TIMEBOX_FONT_OPTION             (0x0003 << TIMEBOX_FONT_OPTION_SHIFT)
#define TIMEBOX_FONT_OPTION_SMALL       (0x0000 << TIMEBOX_FONT_OPTION_SHIFT)
#define TIMEBOX_FONT_OPTION_MIDDLE      (0x0001 << TIMEBOX_FONT_OPTION_SHIFT)
#define TIMEBOX_FONT_OPTION_LARGE       (0x0002<< TIMEBOX_FONT_OPTION_SHIFT)
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    �ַ�����ʾtimebox
 * \param[in]   time��Ӧ�ô��ݵ�timebox��˽�����ݽṹ��ָ��
 * \param[in]   timebox��timebox�ؼ����ݽṹ��ָ��
 * \param[in]   mode��timebox��ˢ�·�ʽ
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_timebox_string(timebox_private_t *time, timebox_t *timebox, timebox_draw_mode_e mode)
{
    region_t num_region;
    DC oldDC_sav;
    uint16 time_buffer[8];
    uint16 textout_mode = 0;
    uint8 disp_bits[8];
    uint8 data_buf[22];
    uint8 temp_buf[5];
    uint8 i, k;
    uint8 count;
    char partition[2];
    string_desc_t desc;

    libc_memset(data_buf, 0, sizeof(data_buf));
    libc_memset(disp_bits, 0, sizeof(disp_bits));
    count = TimeConvert(time, timebox, time_buffer, disp_bits);
    libc_memset(partition, 0, sizeof(partition));

    for (i = 0; i < count; i++)
    {
        libc_itoa(time_buffer[i], temp_buf, disp_bits[i]);
        libc_strncat(data_buf, temp_buf, libc_strlen(temp_buf));
        if (i != (count - 1))
        {
            if ((timebox->attrib & TIMEBOX_COUNT_OPTION) == COUNT_OPTION_ONE)
            {
                partition[0] = partition_id[timebox->part_type[0]];
            }
            else
            {
                if ((time->times[0].mode == DATE_DISP_MMDDYY) || (time->times[0].mode == DATE_DISP_YYMMDD)
                        || (time->times[0].mode == TIME_DISP_HHMMSS))
                {
                    k = 2; //������ʾ�������ж���Ҫ��ʾ�ĸ��ָ���
                }
                else
                {
                    k = 1;
                }
                if (i < k)
                {
                    partition[0] = partition_id[timebox->part_type[0]];
                }
                else if (i == k)
                {
                    partition[0] = partition_id[timebox->part_type[1]];
                }
                else
                {
                    partition[0] = partition_id[timebox->part_type[2]];
                }
            }
            libc_strncat(data_buf, partition, libc_strlen(partition));
        }
    }

    //�����ַ�����ʾ����
    num_region.x = timebox->x;
    num_region.y = timebox->y;
    num_region.width = timebox->width;
    num_region.height = timebox->height;
    romf_get_DC_status(&oldDC_sav, 0, 0);
    if ((timebox->attrib & TIMEBOX_BG_OPTION) == TIMEBOX_BG_OPTION_PIC)
    {
        textout_mode |= CHAR_FILL_NORMAL; //�б���ͼƬ������͸��ģʽ
        if (timebox->back_id != (uint16) - 1)
        {
            romf_show_pic(timebox->back_id, timebox->back_x, timebox->back_y);
        }
    }
    else
    {
        textout_mode |= CHAR_FILL_COLOR;//������ɫ�����Ƿ�͸��ģʽ,Ĭ��Ϊ��͸������ģʽ
        romf_set_backgd_color(timebox->back_id, 0, 0);
        //clear_screen(&num_region, 0, 0);
        romf_fill_rect(&num_region, (uint16) (dc.backgd_color & 0xffff));
    }
    //atrib<11-12>0��ʾС����,1��ʾ�к����壬2��ʾ�����壬Ĭ���к�����
    if ((timebox->attrib & TIMEBOX_FONT_OPTION) == TIMEBOX_FONT_OPTION_SMALL)
    {
        textout_mode |= DISP_FONT_SMALL;
    }
    else if ((timebox->attrib & TIMEBOX_FONT_OPTION) == TIMEBOX_FONT_OPTION_LARGE)
    {
        textout_mode |= DISP_FONT_LARGE;
    }
    else
    {
        textout_mode |= DISP_FONT_NORMAL; //Ĭ���к�����
    }

    //���û�����ɫ
    romf_set_pen_color(timebox->text_color, 0, 0);
    //������ʾ
    if ((timebox->attrib & TIMEBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_LEFT)
    {
        textout_mode |= DISP_ALIGN_LEFT;
    }
    //������ʾ
    else if ((timebox->attrib & TIMEBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_CENTER)
    {
        textout_mode |= DISP_ALIGN_MEDIACY;
    }
    //������ʾ
    else if ((timebox->attrib & TIMEBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_RIGHT)
    {
        textout_mode |= DISP_ALIGN_RIGHT;
    }
    else
    {
        //do nothing for QAC
    }
    textout_mode |= DISP_ARABIC_DISABLE;
    desc.data.str = data_buf;
    desc.length = (uint16) libc_strlen(data_buf);
    desc.language = 0;
    show_string(&desc, &num_region, textout_mode);
    //�ָ�������ɫ
    romf_set_pen_color(oldDC_sav.pen_color, 0, 0);
    romf_set_backgd_color(oldDC_sav.backgd_color, 0, 0);
}
/*! \endcond */
