/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    ������д�ļ��ĸ���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

static uint8 cmp_string1[CMPSTR_MAX_LEN1] _BANK_DATA_ATTR_;
static uint8 cmp_string2[CMPSTR_MAX_LEN1] _BANK_DATA_ATTR_;

//����ʹ�û���
static char *lostk[30] _BANK_DATA_ATTR_;
static char *histk[30] _BANK_DATA_ATTR_;

static uint8 cmp_str_len _BANK_DATA_ATTR_;

extern cmp_obj_f compare_obj;

signed char compare_string(uint16 *data1, uint16 *data2, uint8 len);
signed char compare_word(uint16 *data1, uint16 *data2, uint8 len);
signed char compare_album_track(uint16 *data1, uint16 *data2, uint8 len);
void list_sort(char *base, uint16 num, uint16 width, void *depond_buf, uint8 cmp_type, uint8 cmp_len);

/******************************************************************************/
/*
 * \par  Description:��ȡ�Ƚϵ��ַ���
 *
 * \param[in]    str_num -- ��ȡ��ŵ�λ��
 len--��ȡ�ĳ���

 * \param[out]   string--��Ż�ȡ�����ַ���

 * \return

 * \note
 *******************************************************************************/
void get_string_data(uint16 str_num, char *string, uint8 len)
{
    char *str_addr = (char*) sort_depend_buf + str_num * len;
    libc_memcpy(string, str_addr, (uint32) len);
}
/******************************************************************************/
/*
 * \par  Description: �Ƚ��ַ���
 *
 * \param[in]    data1--string1��ŵ�λ�����
 data2--string2��ŵ�λ�����
 len--�Ƚϵĳ���

 * \param[out]

 * \return       0  -- string1=string2
 1  -- string1>string2
 -1 -- string1<string2
 * \note
 *******************************************************************************/
signed char compare_string(uint16 *data1, uint16 *data2, uint8 len)
{
    uint8 i;
    signed char ret_val = 0;

    get_string_data(*data1, cmp_string1, len);
    get_string_data(*data2, cmp_string2, len);

    for (i = 0; i < len; i++)
    {

        if ((cmp_string1[i] >= 'a') && (cmp_string1[i] <= 'z'))
        {
            cmp_string1[i] = cmp_string1[i] - 0x20;
        }

        if ((cmp_string2[i] >= 'a') && (cmp_string2[i] <= 'z'))
        {
            cmp_string2[i] = cmp_string2[i] - 0x20;
        }

        if (cmp_string1[i] > cmp_string2[i]) //string1>string2
        {
            ret_val = 1;
            break;
        }
        else if (cmp_string1[i] < cmp_string2[i]) //string1<string2
        {
            ret_val = -1;
            break;
        }
        else
        {
            if ((cmp_string1[i] == 0) && (cmp_string2[i] == 0)) //��ǰ����
            {
                ret_val = 0;
                break;
            }
        }
    }
    return ret_val;
}
/******************************************************************************/
/*
 * \par  Description: �Ƚ�16λ��ֵ
 *
 * \param[in]    data1--word1��ŵ�λ�����
 data2--word2��ŵ�λ�����
 len--(no used)

 * \param[out]

 * \return       0  -- word1=word2
 1  -- word1>word2
 -1 -- word1<word2

 * \note
 *******************************************************************************/
signed char compare_word(uint16 *data1, uint16 *data2, uint8 len)
{
    uint16 t1;
    uint16 t2;
    signed char ret_val;

    len = len;

    //��Ϊsort_depend_buf��uint8���ͣ����Գ�2
    t1 = (*data1) * 2;
    t2 = (*data2) * 2;

    if (*(uint16*) (sort_depend_buf + t1) > *(uint16*) (sort_depend_buf + t2))
    {
        ret_val = 1;
    }
    else if (*(uint16*) (sort_depend_buf + t1) == *(uint16*) (sort_depend_buf + t2))
    {
        ret_val = 0;
    }
    else
    {
        ret_val = -1;
    }

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description: �Ƚϴ�TRACK��album������
 *
 * \param[in]    data1--word1��ŵ�λ�����
 data2--word2��ŵ�λ�����
 len--(no used)

 * \param[out]

 * \return       0  -- word1=word2
 1  -- word1>word2
 -1 -- word1<word2
 * \note
 *******************************************************************************/
signed char compare_album_track(uint16 *data1, uint16 *data2, uint8 len)
{

    uint16 t1;
    uint16 t2;

    signed char ret_val = 0;

    len = len;

    t1 = (*data1) * TRACK_CMP_LENGTH;
    t2 = (*data2) * TRACK_CMP_LENGTH;

    //TRACK ����ֻ�Ƚ�1��byte
    if (*(int8*) (track_buf + t1) > *(int8*) (track_buf + t2))
    {
        ret_val = 1;
    }
    else if (*(int8*) (track_buf + t1) == *(int8*) (track_buf + t2))
    {
        ret_val = 0;
    }
    else
    {
        ret_val = -1;
    }

    t1 = t1 << 1;
    t2 = t2 << 1;

    if (ret_val == 0)
    {
        if (*(uint16*) (sort_depend_buf + t1) > *(uint16*) (sort_depend_buf + t2))
        {
            ret_val = 1;
        }
        else if (*(uint16*) (sort_depend_buf + t1) == *(uint16*) (sort_depend_buf + t2))
        {
            ret_val = 0;
        }
        else
        {
            ret_val = -1;
        }

    }
    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description: ���������λ��
 *
 * \param[in]    a--����1
 b--����2
 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void swap(uint16 *a, uint16 *b)
{
    uint16 tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

/******************************************************************************/
/*
 * \par  Description: ð���������
 *
 * \param[in]    lo--���е���ʼ
 hi--���еĽ���
 width--���еĿ��
 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void shortsort(char *lo, char *hi, uint16 width)
{
    char i; //add by lmx
    char *p, *max;

    if (0 == width)
    {
        return;
    }

    while (hi > lo)
    {
        max = lo;
        i = 0; //add by lmx

        for (p = lo + width; p <= hi; p += width)
        {
            if (compare_obj((uint16 *) p, (uint16 *) max, cmp_str_len) > 0)
            {
                max = p;
                i++; //add by lmx
            }
        }

        if (i == ((hi - lo) / width)) //add by lmx
        {
            break; //lo~hi ֮��������������
        }

        if (max != hi) //add by lmx
        {
            swap((uint16 *) max, (uint16 *) hi);
        }

        hi -= width;
    }
}

/******************************************************************************/
/*
 * \par  Description: �Դ�baseΪ��ʼ��ַ�����Ϊwidth������Ϊn���н�������
 *
 * \param[in]   base--���е���ʼ��ַ
 num--���е���Ŀ
 width--���еĿ��
 depond_buf--���������Ķ���
 cmp_type--�Ƚϵ��ýӿ�����
 cmp_len--�Ƚϵĳ���
 * \param[out]

 * \return

 * \note
 *******************************************************************************/

void list_sort(char *base, uint16 num, uint16 width, void *depond_buf, uint8 cmp_type, uint8 cmp_len)
{
    char *lo, *hi;
    char *mid;
    char *loguy, *higuy;
    uint16 size;
    //    char *lostk[30], *histk[30];
    int16 stkptr;

    if ((num < 2) || (0 == width))
    {
        return; // nothing to do
    }

    if (cmp_type == SORT_CMP_WORD)
    {
        compare_obj = compare_word;
    }
    else if (cmp_type == SORT_CMP_ALBUM)
    {
        compare_obj = compare_album_track;
        //        compare_obj = compare_word; //����ֱ�ۣ��ݲ�����track����
    }
    else
    {
        compare_obj = compare_string;
    }

    cmp_str_len = cmp_len;

    stkptr = 0; // initialize stack
    lo = base;
    hi = (char *) base + width * (num - 1); // initialize limits
    sort_depend_buf = depond_buf;

    recurse: size = (uint16) (((hi - lo) / width) + 1); // number of el's to sort
    if (size <= CUTOFF)
    { //   <8
        shortsort(lo, hi, width); //ð�ݷ�����
    }
    else
    {
        mid = lo + ((size / 2) * width); // find middle element
        swap((uint16 *) mid, (uint16 *) lo); //swap it to beginning of array
        loguy = lo;
        higuy = hi + width;
        for (;;)
        {
            do
            {
                loguy += width;
            } while ((loguy <= hi) && (compare_obj((uint16 *) loguy, (uint16 *) lo, cmp_str_len) <= 0));

            do
            {
                higuy -= width;
            } while ((higuy > lo) && (compare_obj((uint16 *) higuy, (uint16 *) lo, cmp_str_len) >= 0));

            if (higuy < loguy)
            {
                break;
            }
            swap((uint16 *) loguy, (uint16 *) higuy);
        }

        if (lo != higuy) //add by lmx
        {
            swap((uint16 *) lo, (uint16 *) higuy); // put partition element in place
        }

        if ((higuy - 1 - lo) >= (hi - loguy))
        {
            if ((lo + width) < higuy)
            {
                lostk[stkptr] = lo;
                histk[stkptr] = (char*) ((uint32) higuy - width);
                ++stkptr;
            } //save big recursion for later,(save small-value array)

            if (loguy < hi) //more than one
            {
                lo = loguy;
                goto recurse;
                // do small recursion
            }
        }
        else
        {
            if (loguy < hi)
            {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr; //save big recursion for later ,(save big-value array)
            }

            if ((lo + width) < higuy) //more than two
            {
                hi = (char*) ((uint32) higuy - width);
                goto recurse;
                // do small recursion
            }
        }
    }

    --stkptr;

    if (stkptr >= 0)
    {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;
        // pop subarray from stack
    }
    else
    {
        return; /* all subarrays done */
    }

}
