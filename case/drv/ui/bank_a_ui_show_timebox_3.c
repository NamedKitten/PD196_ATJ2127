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
 * \file    ui_show_timebox_3.c
 * \brief    ʱ����ֵ����ת��ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ��ʱ����ֵ��ת������
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

#define TIMEBOX_COUNT_OPTION_SHIFT      (4)
#define TIMEBOX_COUNT_OPTION            (0x0001 << TIMEBOX_COUNT_OPTION_SHIFT)
#define COUNT_OPTION_ONE                (0x0000 << TIMEBOX_COUNT_OPTION_SHIFT)
#define COUNT_OPTION_TWO                (0x0001 << TIMEBOX_COUNT_OPTION_SHIFT)
#define TIMEBOX_ZERO_EN(a)              (0x0001 << (5+(a)))
#define TIMEBOX_FIRST_DIGIT_SHIFT(a)    (7 + 3*(a))
#define TIMEBOX_FIRST_DIGIT(attrib, a)  \
(((attrib) & (0x0007 << TIMEBOX_FIRST_DIGIT_SHIFT(a))) >> TIMEBOX_FIRST_DIGIT_SHIFT(a))

static uint8 count_digit(uint16 num)
{
    uint8 dight = 1;
    
    while(num >= 10)
    {
        dight++;
        num = num / 10;
    }
    
    return dight;
}

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    �ַ�����ʾtimebox
 * \param[in]   time��Ӧ�ô��ݵ�timebox��˽�����ݽṹ��ָ��
 * \param[in]   timebox��timebox�ؼ����ݽṹ��ָ��
 * \param[out]  time_buffer������ʱ����ֵ��bufferָ��
 * \param[out]  disp_bits������ʱ���֡�����ʾλ����bufferָ��
 * \return      �ܹ�Ҫ��ʾ��������
 * \note
 *******************************************************************************/
uint8 TimeConvert(timebox_private_t *time, timebox_t *timebox, uint16 *time_buffer, uint8 *disp_bits)
{
    uint16 buffer[8];
    uint8 num_bits[8];
    uint8 i, j;

    libc_memset(buffer, -1, sizeof(buffer));
    libc_memset(num_bits, 0, sizeof(num_bits));
    if ((timebox->attrib & TIMEBOX_COUNT_OPTION) == COUNT_OPTION_ONE)
    {
        j = 1;
    }
    else
    {
        j = 2;
    }
    for (i = 0; i < j; i++)
    {
        switch(time->times[i].mode)
        {
        //���ꡢ�¡��յ�ֵת������Ҫ�ĸ�ʽ
        case DATE_DISP_YYMMDD:
        case DATE_DISP_YYMM:
            buffer[4 * i] = time->times[i].union_time.date.year;
            buffer[4 * i + 1] = time->times[i].union_time.date.month;
            if (time->times[i].mode == DATE_DISP_YYMMDD)
            {
                buffer[4 * i + 2] = time->times[i].union_time.date.day;
            }
            
            //���λ���ɵ�һ�����ֵ�λ��ָ��
            num_bits[4 * i] = (uint8)TIMEBOX_FIRST_DIGIT(timebox->attrib, i);
            num_bits[4 * i + 1] = 2;
            if (time->times[i].mode == DATE_DISP_YYMMDD)
            {
                num_bits[4 * i + 2] = 2;
            }
            break;

        case DATE_DISP_MMDDYY:
        case DATE_DISP_MMDD:
            buffer[4 * i] = time->times[i].union_time.date.month;
            buffer[4 * i + 1] = time->times[i].union_time.date.day;
            if (time->times[i].mode == DATE_DISP_MMDDYY)
            {
                buffer[4 * i + 2] = time->times[i].union_time.date.year;
            }
            
            num_bits[4 * i] = 2;
            num_bits[4 * i + 1] = 2;
            if (time->times[i].mode == DATE_DISP_MMDDYY)
            {
                //���λ���ɵ�һ�����ֵ�λ��ָ��
                num_bits[4 * i + 2] = (uint8)TIMEBOX_FIRST_DIGIT(timebox->attrib, i);
                if(num_bits[4 * i + 2] == 4)
                {
                    buffer[4 * i + 2] += 2000;
                }
            }
            break;
        
        //��ʱ�����ֵת����Ҫ�ĸ�ʽ
        case TIME_DISP_HHMMSS:
        case TIME_DISP_HHMM:
            buffer[4 * i] = time->times[i].union_time.time.hour;
            buffer[4 * i + 1] = time->times[i].union_time.time.minute;
            if (time->times[i].mode == TIME_DISP_HHMMSS)
            {
                buffer[4 * i + 2] = time->times[i].union_time.time.second;
            }
            
            //Сʱλ���ɵ�һ�����ֵ�λ��ָ��
            num_bits[4 * i] = (uint8) TIMEBOX_FIRST_DIGIT(timebox->attrib, i);
            if ((timebox->attrib & TIMEBOX_ZERO_EN(i)) != TIMEBOX_ZERO_EN(i))
            {
                //����ʾǰ��0�������ʵλ������ָ��λ��������ʵλ����ʾ
                if(count_digit(buffer[4 * i]) < num_bits[4 * i])
                {
                    num_bits[4 * i] = count_digit(buffer[4 * i]);
                }
            }
            num_bits[4 * i + 1] = 2;
            if (time->times[i].mode == TIME_DISP_HHMMSS)
            {
                num_bits[4 * i + 2] = 2;
            }
            break;
            
        case TIME_DISP_MMSS:
            buffer[4 * i + 1] = time->times[i].union_time.time.minute;
            buffer[4 * i + 2] = time->times[i].union_time.time.second;
            
            //����λ���ɵ�һ�����ֵ�λ��ָ��
            num_bits[4 * i + 1] = (uint8) TIMEBOX_FIRST_DIGIT(timebox->attrib, i);
            if (num_bits[4 * i + 1] > 2)//�����ʾ2λ
            {
                num_bits[4 * i + 1] = 2;
            }
            if ((timebox->attrib & TIMEBOX_ZERO_EN(i)) != TIMEBOX_ZERO_EN(i))
            {
                //����ʾǰ��0�������ʵλ������ָ��λ��������ʵλ����ʾ
                if(count_digit(buffer[4 * i + 1]) < num_bits[4 * i + 1])
                {
                    num_bits[4 * i + 1] = count_digit(buffer[4 * i + 1]);
                }
            }
            num_bits[4 * i + 2] = 2; //��������ʾ2λ
            break;
            
        default:
            //do nothing for QAC
            break;
        }
    }
    j = 0;
    for (i = 0; i < 8; i++)
    {
        if (buffer[i] != (uint16) - 1)
        {
            time_buffer[j] = buffer[i];
            disp_bits[j] = num_bits[i];
            j++;
        }
    }
    return j;
}
/*! \endcond */
