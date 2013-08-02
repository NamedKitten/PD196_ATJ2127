/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     eh_lrc_load_sub3.c
 * \brief    �������
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_lrc.h"
#include "Simulator.h"

extern uint32 lrc_pos_buffer;
extern uint32 lrc_pos_file;
extern uint32 lrc_file_len;

extern uint8 positiveflag; //TRUE--'+',FALSE---'-'
extern uint8 offset_sec; // 1 s
extern uint8 offset_ms; //10ms


extern lrc_lable_t *lrc_lab_p; // ��ʱ���ǩָ��
extern uint8 *buf_point; //lrc��buf����ָ��
extern uint8 *save_lrc_buf_p; //��������ָ��
extern uint8 *lrc_vram_p;

extern uint8 get_time_buf[8]; //��ȡʱ���ǩ��ʱ����
extern uint8 *get_time_p; //���浥��ʱ���ǩָ��


extern uint16 same_str_count; //ͬ��ʵ�ʱ���ǩ����,1-ֻ��1����ǩ��2-��2����ǩ
extern uint16 lrc_str_len; //ÿ����ʵĳ���
extern uint16 lab_lrc_offset; //ÿ������������е�ƫ��λ��


extern uint8 lrc_temp_buf[SECTOR_SIZE]; //��lrc����
extern uint8 lrc_save_buf[SECTOR_SIZE]; //�������ݻ���

/******************************************************************************/
/*
 * \par  Description: ����ʱ���ǩ�������ʲ���ʱ��
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool add_time_offset(void)
{
    uint8 i, j;
    uint8 count;

    lrc_lab_p = (lrc_lable_t *) LRC_LABLE_ADDR;

    if ((offset_ms == 0) && (offset_sec == 0))
    {
        //��Ҫ�����ʱ��Ϊ0����Ҫ��
        return TRUE;
    }

    if (FALSE != positiveflag)
    {
        for (count = 0; count < time_lab_count; count++)
        {
            //Ϊ����
            //ClearWatchDog();
            if (((lrc_lab_p->sec != 0) || (lrc_lab_p->min != 0)) || (lrc_lab_p->p_ms != 0))
            {
                //10ms(0.01s)��λ��
                j = offset_ms + lrc_lab_p->p_ms;
                i = 0;
                if (j >= 100)
                {
                    j = j - 100;
                    i = 1;
                }
                lrc_lab_p->p_ms = j;

                //����λ��
                j = offset_sec + i + lrc_lab_p->sec;
                i = 0;
                if (j >= 60)
                {
                    j = j - 60;
                    i = 1;
                }

                lrc_lab_p->sec = j;
                //��λ����
                lrc_lab_p->min += i;
            }
            lrc_lab_p++; //ָ���һ
        }
    }
    else
    {
        //offset_sec�п��ܴ���60�����ܳ���1���ӣ�ֻ��С��100
        //ʱ���ǩ�������Ѿ������ˣ��䳬��60������ӽ�λ������
        //��ƫ����80��ʱ��ԭ��ʱ��Ϊ1���� 10�� 100����ʱ��
        //�Ƚ���Ȼ����������ȴ�Ǵ����
        //tmpbuf1[0] = offset_ms;
        //tmpbuf1[1] = offset_sec;
        //tmpbuf1[2] = 0;
        //tmpbuf1[3] = 0;

        uint8 tmpbuf1[4];
        uint8 tmpbuf2[4];

        if (offset_sec >= 60)
        {
            tmpbuf1[3] = 1;
            tmpbuf1[2] = offset_sec - 60;
        }
        else
        {
            tmpbuf1[3] = 0;
            tmpbuf1[2] = offset_sec;
        }
        tmpbuf1[1] = offset_ms;
        tmpbuf1[0] = 0;

        for (count = 0; count < time_lab_count; count--)
        {
            //ClearWatchDog();
            if ((lrc_lab_p->sec != 0) || (lrc_lab_p->min != 0) || (lrc_lab_p->p_ms != 0))
            {
                tmpbuf2[3] = lrc_lab_p->min;
                tmpbuf2[2] = lrc_lab_p->sec;
                tmpbuf2[1] = lrc_lab_p->p_ms;
                tmpbuf2[0] = 0;

                if (*(uint32 *) tmpbuf1 >= *(uint32 *) tmpbuf2)
                {
                    //����ʱ������OFFSETֵС�����
                    lrc_lab_p->min = 0;
                    lrc_lab_p->sec = 0;
                    lrc_lab_p->p_ms = 0;
                    lrc_lab_p++; //ָ���һ
                    continue;
                }

                //100ms(0.1s)��λ��
                if (lrc_lab_p->p_ms >= offset_ms)
                {
                    j = lrc_lab_p->p_ms;
                    i = 0;
                }
                else
                {
                    j = lrc_lab_p->p_ms + 10;
                    i = 1;//����1
                }
                lrc_lab_p->p_ms = j - offset_ms;

                //����λ��
                if (lrc_lab_p->sec >= (tmpbuf1[2] + i))
                {
                    j = lrc_lab_p->sec - i;
                    i = 0;
                }
                else
                {
                    j = lrc_lab_p->sec + 60 - i;
                    i = 1;
                }
                lrc_lab_p->sec = j - tmpbuf1[2];

                //��λ����
                lrc_lab_p->min -= (i + tmpbuf1[3]);
            }
            lrc_lab_p++; //ָ���һ
        }

    }

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description: ����ʱ���ǩ
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void sort_lrc_label(void)
{
    uint16 i, j;
    lrc_lab_p = (lrc_lable_t *) LRC_LABLE_ADDR;

    //ð���㷨����
    for (i = 1; i <= time_lab_count; i++)
    {
        for (j = i; j > 0; j--)
        {
            //ClearWatchDog();
            if (lrc_lab_p[j].min > lrc_lab_p[j - 1].min)
            {
                break;//continue;
            }
            else if ((lrc_lab_p[j].min == lrc_lab_p[j - 1].min) && (lrc_lab_p[j].sec > lrc_lab_p[j - 1].sec))
            {
                break;//continue;
            }
            else if ((lrc_lab_p[j].min == lrc_lab_p[j - 1].min) && (lrc_lab_p[j].sec == lrc_lab_p[j - 1].sec)
                    && ((lrc_lab_p[j].p_ms) >= (lrc_lab_p[j - 1].p_ms)))
            {
                break;//continue;
            }
            else//һ��ҪС�ڲŽ�������֤���˳��
            {
                libc_memcpy(lrc_temp_buf, lrc_lab_p + j, sizeof(lrc_lable_t));
                libc_memcpy(lrc_lab_p + j, lrc_lab_p + j - 1, sizeof(lrc_lable_t));
                libc_memcpy(lrc_lab_p + j - 1, lrc_temp_buf, sizeof(lrc_lable_t));
            }
        }
    }
    return;
}

/******************************************************************************/
/*
 * \par  Description: �������ʺ�������
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void explain_ok_deal(void)
{
    uint16 temp;
    uint8 *t_lrc_lab_p;

    // ��ʣ�µĸ������д��VRAM
    if ((lab_lrc_offset % SECTOR_SIZE) != 0)
    {
        sys_vm_write(lrc_save_buf, lrc_vram_p);
    }

    // �����ǩ
    sort_lrc_label();

    //��ʱ���ǩд��VRAM
    temp = (sizeof(lrc_lable_t) * time_lab_count - 1) / SECTOR_SIZE + 1;
    t_lrc_lab_p = (uint8*) LRC_LABLE_ADDR;
    lrc_vram_p = (uint8*) LRC_LABLE_VM;

    while (temp > 0)
    {
        temp--;
        sys_vm_write(t_lrc_lab_p, lrc_vram_p);
        lrc_vram_p += SECTOR_SIZE;
        t_lrc_lab_p += SECTOR_SIZE;
    }

    return;
}

