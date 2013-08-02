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
 * \file    ui_show_timebox_1.c
 * \brief    ����ͼƬ��ʾtimebox�ĺ���ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ������ͼƬ��ʽ��timebox����ʾ
 * \par      EXTERNALIZED FUNCTIONS:
 *               show_num_pic��show_pic
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

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

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ����ͼƬ��ʾtimebox
 * \param[in]   time��Ӧ�ô��ݵ�timebox��˽�����ݽṹ��ָ��
 * \param[in]   timebox��timebox�ؼ����ݽṹ��ָ��
 * \param[in]   mode��timebox��ˢ�·�ʽ
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_timebox_pic(timebox_private_t *time, timebox_t *timebox, timebox_draw_mode_e mode)
{
    transparent_pic_t trs_pic;
    res_entry_t res_entry;
    res_open_infor_t *res_ptr;
    uint16 time_save[8]; //������һ����ʾ��ʱ��ֵ
    uint16 time_buffer[8];
    uint16 partition_id = -1;
    uint16 x = 0;
    uint16 back_x = 0;
    uint16 x_left = 0;
    uint16 x_right = 0;
    uint16 display_length;
    uint8 disp_bits[8];
    uint8 update_flag[8];
    uint8 temp_num;
    uint8 data_buf[17];
    uint8 temp_buf[5];
    uint8 i, j, k;
    uint8 count;
    int8 n = -1;

    libc_memset(data_buf, 0, sizeof(data_buf));
    libc_memset(disp_bits, 0, sizeof(disp_bits));
    libc_memset(time_save, 0, sizeof(time_save));
    count = TimeConvert(time, timebox, time_buffer, disp_bits);
    if (mode == TIMEBOX_DRAW_TIME)
    {
        for (i = 0; i < count; i++)
        {
            if (time_buffer[i] != time_save[i])
            {
                update_flag[i] = 1;
            }
            else
            {
                update_flag[i] = 0;
            }
        }
    }
    //modeΪȫ������ʱ����ȫ�ֱ�����0
    else if (mode == TIMEBOX_DRAW_ALL)
    {
        //time_bits= 0;
        for (i = 0; i < 8; i++)
        {
            update_flag[i] = 1;
            time_save[i] = 0;
        }
    }
    else
    {
        libc_memset(&update_flag, 0, 8);
    }
    //��ȡ����ͼƬ��Ϣ
    res_ptr = res_ptr_select(timebox->num0_id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (timebox->num0_id & 0x7fff)
            * RESHEADITEM);
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
    for (i = 0; i < count; i++)
    {
        libc_itoa(time_buffer[i], temp_buf, disp_bits[i]);
        libc_strncat(data_buf, temp_buf, libc_strlen(temp_buf));
    }
    display_length = (uint16) (libc_strlen( data_buf)) *timebox->pic_width + timebox->part_width * (count - 1);
    //������ʾ��X����
    if ((timebox->attrib & TIMEBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_LEFT)
    {
        back_x = x = timebox->x;
    }
    //������ʾ��X����
    else if ((timebox->attrib & TIMEBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_CENTER)
    {
        if (timebox->width > display_length)
        {
            back_x = x = timebox->x + (timebox->width - display_length) / 2;
        }
        else
        {
            back_x = x = timebox->x;
        }
    }
    //������ʾ��X����
    else if ((timebox->attrib & TIMEBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_RIGHT)
    {
        back_x = x = timebox->x + timebox->width - display_length;
    }
    else
    {
        //do nothing for QAC
    }
    
    if(timebox->back_id != (uint16)(-1))
    {
        trs_pic.back_id = timebox->back_id;
        trs_pic.back_x = timebox->x;
        trs_pic.back_y = timebox->y;
        trs_pic.back_color = timebox->numpic_back_color;
        trs_pic.clear = 0;
        trs_pic.direct = 0;
        trs_pic.pic_y = timebox->y;
    }
    
    for (i = 0; i < count; i++)
    {
        libc_itoa(time_buffer[i], temp_buf, disp_bits[i]);
        if ((update_flag[i] == 1) || (time_bits != (uint8) libc_strlen(data_buf))) //�����ֱ仯������λ���仯ʱ��Ҫ����
        {
            //��ʾ����
            for (j = 0; j < (uint8) libc_strlen(temp_buf); j++)
            {
                temp_num = temp_buf[j] - 0x30;
                if(timebox->back_id != (uint16)(-1))
                {
                    trs_pic.pic_id = timebox->num0_id + temp_num;
                    trs_pic.pic_x = x;
                    show_pic_transparent(&trs_pic);
                }
                else
                {
                    show_num_pic(temp_num, x, timebox->y, &res_entry, res_ptr);
                }
                x += timebox->pic_width;
            }
            if (i == (count - 1))
            {
                break;//���������Ѿ���ʾ��ϣ��˳�
            }
            
            //��ʾ�ָ���
            if ((timebox->attrib & TIMEBOX_COUNT_OPTION) == COUNT_OPTION_ONE)
            {
                partition_id = timebox->part_id[timebox->part_type[0]];
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
                    partition_id = timebox->part_id[timebox->part_type[0]];
                }
                else if (i == k)
                {
                    partition_id = timebox->part_id[timebox->part_type[1]];
                }
                else
                {
                    partition_id = timebox->part_id[timebox->part_type[2]];
                }
            }
            
            if(timebox->back_id != (uint16)(-1))
            {
                trs_pic.pic_id = partition_id;
                trs_pic.pic_x = x;
                show_pic_transparent(&trs_pic);
            }
            else
            {
                romf_show_pic(partition_id, x, timebox->y);
            }
            x += timebox->part_width;
        }
        else //����Ҫˢ�µ����
        {
            x += (uint16) (libc_strlen( temp_buf)) *timebox->pic_width + timebox->part_width;
        }
    }
    //����λ���仯ʱ����Ҫ�ñ������ͼƬ�����Ӧ����
    n = (int8) (time_bits - (uint8) libc_strlen(data_buf) - count + 1);
    if (n > 0)
    {
        //������ʾʱ������λ������ʱ�����߾����ñ���ͼ���ԭ��������
        if ((timebox->attrib & TIMEBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_CENTER)
        {
            n = (n/2) + (n%2);//for QAC ��Ϊȡһ�룬������������Ӷ�1
            x_left = back_x - n * timebox->pic_width;
            x_right = back_x + display_length;
        }
        //������ʾ��X����
        else if ((timebox->attrib & TIMEBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_RIGHT)
        {
            x = back_x - n * timebox->pic_width;
        }
        else
        {
            //do nothing for QAC
        }
    }
    while (n > 0)
    {
        if ((timebox->attrib & TIMEBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_CENTER)
        {
            if(timebox->back_id != (uint16)(-1))
            {
                trs_pic.pic_id = timebox->num0_id + 10;
                trs_pic.clear = 1;
                trs_pic.pic_x = x_left;
                show_pic_transparent(&trs_pic);
                trs_pic.pic_x = x_right;
                show_pic_transparent(&trs_pic);
            }
            else
            {
                show_num_pic(10, x_left, timebox->y, &res_entry, res_ptr);
                show_num_pic(10, x_right, timebox->y, &res_entry, res_ptr);
            }
            x_left += timebox->pic_width;
            x_right += timebox->pic_width;
        }
        else
        {
            if(timebox->back_id != (uint16)(-1))
            {
                trs_pic.pic_id = timebox->num0_id + 10;
                trs_pic.clear = 1;
                trs_pic.pic_x = x;
                show_pic_transparent(&trs_pic);
            }
            else
            {
                show_num_pic(10, x, timebox->y, &res_entry, res_ptr);
            }
            x += timebox->pic_width;
        }
        n--;
    }
    time_bits = (uint8) (libc_strlen(data_buf) + count - 1);
    libc_memcpy(time_save, time_buffer, 8);
}
/*! \endcond */
