/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       cailizhen     2012-1-12 9:42     1.0             build this file
 *******************************************************************************/
/*!
 * \file     _rom_ui_image.c
 * \brief    ROM����ģ��--ͼƬ��ʾ���֣�С������
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2012-1-12
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ����ͼƬres_id��ʾ��ͼƬ
 * \param[in]    id��ͼƬ��ԴID
 * \param[in]    x��ͼƬ��X����
 * \param[in]    y��ͼƬ��Y����
 * \param[out]   none
 * \return       ͼƬ�Ƿ���ʾ�ɹ���1��0
 * \ingroup      picture
 * \note
 *******************************************************************************/
bool romf_show_pic(uint16 id, uint16 x, uint16 y)
{
    res_open_infor_t *res_ptr;
    res_entry_t res_entry;
    region_t res_region;

    //��ȡͼƬ��Դ��Ϣ
    res_ptr = res_ptr_select(id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(id) * RESHEADITEM));
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));

    /*�贰��������*/
    res_region.x = x;
    res_region.y = y;
    res_region.width = res_entry.width;
    res_region.height = res_entry.height;

    /*��ȡͼƬ���ݲ�ˢ��*/
    return put_sd_image(res_ptr->res_fp, res_entry.offset, &res_region);
}
/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾsd�ļ���ָ��λ�õ�ͼƬ
 * \param[in]    res_fp��SD��Դ�ļ����
 * \param[in]    img_addr����ǰͼƬ��sd�ļ��е���ʼ��ַ
 * \param[in]    img_region��ͼƬ��ʾ����
 * \param[out]   none
 * \return       none
 * \ingroup      ui driver
 * \note
 *******************************************************************************/
bool romf_put_sd_image(sd_handle *res_fp, uint32 img_addr, region_t *img_region)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint32 cnt;
    uint32 start_addr;
    uint16 count;
    bool result = TRUE;

    //�����ʾ�����Ƿ񳬳�
    if (((img_region->x + img_region->width) > (region_win.x + region_win.width))
     || ((img_region->y + img_region->height) > (region_win.y + region_win.height)))
    {
        return FALSE;//X�����Y��Խ��
    }

    //����ͼƬ��ʾģʽ������
    set_lcd_param_hook(img_region, LCD_PARAM_IMAGE);

    //��ʼ�����ݴ�С���´ζ�ȡ��ַ���Լ���������ַ
    cnt = ((uint32) img_region->width) * ((uint32) img_region->height) * 2;
    start_addr = img_addr;

    //�ֶ�ζ�ȡ��ʾ��ÿ�ζ�ȡ����ʾ��ʾ�ݴ����Ĵ�С��ͼƬ����
    while (cnt > 0)
    {
        //��ȡ����
        if (cnt >= (uint32) display_buf_len)
        {
            count = display_buf_len;
        }
        else
        {
            count = (uint16) cnt;
        }
        sys_sd_fseek(res_fp, SEEK_SET, start_addr);
        sys_sd_fread(res_fp, display_buffer, (uint32) count);

        //DMA��GRAM��
        if (dma_buffer_data_hook(count / 2, img_region->width) == FALSE)
        {
            result = FALSE;
            break;
        }

        //����ʣ�����ݴ�С���´ζ�ȡ��ַ
        cnt = cnt - count;
        start_addr += count;
    }

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ָ����������ָ����ɫ��������������ʾSD BMPͼƬ
 * \param[in]    fill_region��ָ��Ҫ��������
 * \param[in]    color�������ɫ
 * \param[out]   none
 * \return       ��������Ƿ�ɹ�
 * \note         ��һЩͼ�λ�������������Ϸ���У�����Ƿǳ������Ľӿڣ�����Ҳ�̻���ȥ��
 *******************************************************************************/
bool romf_fill_rect(region_t *fill_region, uint16 color)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint32 cnt, buf_cnt;
    uint32 count;
    uint16 i;
    bool result = TRUE;

    //�����ʾ�����Ƿ񳬳�
    if (((fill_region->x + fill_region->width) > (region_win.x + region_win.width)) 
     || ((fill_region->y + fill_region->height) > (region_win.y + region_win.height)))
    {
        return FALSE;//X�����Y��Խ��
    }

    //������ʾģʽ����䴰��
    set_lcd_param_hook(fill_region, LCD_PARAM_FILL);

    //��ʼ�����ݴ�С����������ַ�����������
    cnt = ((uint32) fill_region->width) * ((uint32) fill_region->height) * 2;
    //���Ҫ������������С�������ʼ����ô�໺����
    buf_cnt = display_buf_len;
    if (cnt < buf_cnt)
    {
        buf_cnt = cnt;//�����ʼ���պø����������Ļ�����
    }
    for (i = 0; i < (buf_cnt / 2); i++)
    {
        display_buffer[i * 2] = (uint8)(color & 0xff);
        display_buffer[i * 2 + 1] = (uint8)((color >> 8) & 0xff);
    }

    //�ֶ�����
    while (cnt > 0)
    {
        //��ȡ����
        if (cnt >= buf_cnt)
        {
            count = buf_cnt;
        }
        else
        {
            count = cnt;
        }

        //DMA��GRAM��
        if (dma_buffer_data_hook(count / 2, fill_region->width) == FALSE)
        {
            result = FALSE;
            break;
        }

        //����ʣ�����ݴ�С���´ζ�ȡ��ַ
        cnt = cnt - count;
    }

    return result;
}
/*! \endcond */
