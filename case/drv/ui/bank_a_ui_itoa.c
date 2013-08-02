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
 * \file     ui_itoa.c
 * \brief    ����ת�����ַ�����ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *             ������ת���ɶ�Ӧ�ַ���
 * \par      EXTERNALIZED FUNCTIONS:
 *              none
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond */
uint8 *itoa_n(int32 num, uint8 *str, uint8 radix, uint16 counts, uint16 disp_mode)
{
    uint8 * ptr = str;

    if (num < 0)
    {
        num = -num;
        if (disp_mode == 1)
        {
            radix = ':'; //�����ʾ��ֵΪ����������ϸ���
        }
        else
        {
            radix = '-';
        }
    }
    else if (radix != 0)
    {
        if (disp_mode == 1)
        {
            radix = ';'; //�����Ҫ��ʾ���ţ����������
        }
        else
        {
            radix = '+';
        }
    }
    else
    {

    }
    if (radix != 0)
    {
        ptr[0] = radix;
        ptr = ptr + 1;
    }

    libc_itoa((uint32)num, ptr, counts);
    return str;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ʾ����ͼƬ
 * \param[in]   num����ǰ����
 * \param[in]   x��ͼƬ��X����
 * \param[in]   y��ͼƬ��Y����
 * \param[in]   res_entry������0ͼƬ����Դ�ṹ��Ϣ
 * \param[in]   res_ptr����ǰ��Դ��Ϣ�ṹ��ָ��
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void show_num_pic(uint16 num, uint16 x, uint16 y, res_entry_t *res_entry, res_open_infor_t *res_ptr)
{
    region_t res_region;
    uint32 addr = res_entry->offset;
    uint32 pic_bytes = (res_entry->width * res_entry->height) * 2;
    //�贰��������
    res_region.x = x;
    res_region.y = y;
    res_region.width = (uint8) res_entry->width;
    res_region.height = (uint8) res_entry->height;
    //��ȡͼƬ���ݲ�ˢ��
    while (num != 0)
    {
        num--;
        addr += pic_bytes;
    }
    put_sd_image(res_ptr->res_fp, addr, &res_region);
}
/*! \endcond */
