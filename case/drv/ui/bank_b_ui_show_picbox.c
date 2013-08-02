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
 * \file    ui_show_picbox.c
 * \brief   ��ʾpicbox��ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ��picbox�ؼ����ݵĽ�������ʾpicbox
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���ϵͳSD�ļ�ϵͳ�Ľӿں�����������ʾpicbox
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
#define PICBOX_SHOW_EN                  (0x0001 << 0)
#define PICBOX_FRAME_OPTION_SHIFT       (1)
#define PICBOX_FRAME_OPTION             (0x0001 << PICBOX_FRAME_OPTION_SHIFT)
#define PICBOX_FRAME_OPTION_ID          (0x0000 << PICBOX_FRAME_OPTION_SHIFT)
#define PICBOX_FRAME_OPTION_ADDR        (0x0001 << PICBOX_FRAME_OPTION_SHIFT)
#define PICBOX_FRAME_COUNT_SHIFT        (8)
#define PICBOX_FRAME_COUNT(attrib)      (((attrib) >> PICBOX_FRAME_COUNT_SHIFT) & 0xff)

#define UP_HALF_DEF    0 //�ϰ벿��
#define DOWN_HALF_DEF  1 //�°벿��

void pic_overlay_id3(uint8 *pixel_result, uint8 *pic_source, region_t *b_region, uint16 start_row, uint8 mode);
bool put_image_id3(region_t *img_region);
bool show_pic_id3(uint16 bg_res_id, uint16 res_id, uint16 res1_id, picbox_t *picbox, uint16 start_row);

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾpicbox
 * \param[in]    picbox_style ָ��picbox��style�ṹ�����
 * \param[in]    picbox_data Ӧ�ô��ݵ�picbox��˽�����ݽṹ��ָ��
 * \param[out]   none
 * \return       ui_result_e���͵Ĳ������μ�ui_result_e����
 * \ingroup      ui_controls
 * \par          exmaple code
 * \code
    ����1����ʾ��֡picbox��ѡ���ͼƬ
    style_infor_t picbox_style;
    
    picbox_style.style_id = PICBOX_ID1;
    picbox_style.type = UI_AP;
    
    ui_show_picbox(&picbox_style, NULL);
    
    ����2����ʾ��֡picbox��ĳһ֡ͼƬ
    style_infor_t picbox_style;
    picbox_private_t picbox_param;
    
    picbox_style.style_id = PICBOX_ID1;
    picbox_style.type = UI_AP;
    picbox_param.pic_id = -1;//����Ϊ-1
    picbox_param.frame_id = 1;//��0��ʼ��1��ʾ�ڶ�֡
    
    ui_show_picbox(&picbox_style, &picbox_param);
    
    ����3����ʾ��֡picbox��ͼƬ���û�ָ����Ҫ��ͼƬ���ͬpicbox��ѡ���ͼƬһ��
    style_infor_t picbox_style;
    picbox_private_t picbox_param;
    
    picbox_style.style_id = PICBOX_ID1;
    picbox_style.type = UI_AP;
    picbox_param.pic_id = P_USER_ID1;
    picbox_param.frame_id = 0;
    
    ui_show_picbox(&picbox_style, &picbox_param);
 * \endcode
 * \note
 * \li  picbox_data->pic_id != -1��������ʾpic_id��ָ���ͼƬ��
 * \li  picbox_data->pic_id == -1��picbox_data->frame_id != -1������ʾStyleID��ָ���Picbox��
 *      frame_id��ͼƬ����֡ͼƬ�е�һ֡����
 * \li  ��ֱ����ʾ��ʾpicbox.idָ���ͼƬ����picbox_data����ֱ�Ӹ�ֵΪNULL,��picbox_data=NULL��

 *******************************************************************************/
ui_result_e show_picbox(style_infor_t *picbox_style, picbox_private_t *picbox_data, void *null3)
{
    picbox_t picbox;
    uint32 frame_entry_offset;
    res_open_infor_t *style_ptr;
    uint16 res_id = (uint16) - 1;
    uint16 index = (uint16) - 1;
    uint16 frame_cnt;

    /*���ݴ���Դ�ļ������ͣ���ָ��ָ��ͬ��ȫ�ֱ���*/
    if (picbox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, style_ptr->style_picbox_offset + (uint32) (picbox_style->style_id
            & 0x0fff) * sizeof(picbox_t));
    sys_sd_fread(style_ptr->res_fp, &picbox, sizeof(picbox_t));
    //����ʾpicbox
    if ((picbox.attrib & PICBOX_SHOW_EN) != PICBOX_SHOW_EN)
    {
        return UI_NO_DISPLAY;
    }
    //��ȡpicbox ֡��
    frame_cnt = PICBOX_FRAME_COUNT(picbox.attrib);
    if (picbox_data != NULL)
    {
        //������ʾpic_id�е�ͼƬ
        if (picbox_data->pic_id != (uint16) - 1)
        {
            res_id = picbox_data->pic_id;
        }
        else
        {
            if (picbox_data->frame_id != (uint8) - 1)
            {
                index = picbox_data->frame_id;
                if (index >= frame_cnt)//֡�Ŵ���
                {
                    return UI_PARAM_ERR;
                }
            }
        }
    }
    else
    {
        index = (uint16) - 1;
    }
    //������ʾpicbox_data->pic_id��ָ���ͼƬ
    if (((picbox_data != NULL) && (picbox_data->pic_id == (uint16) - 1)) || (picbox_data == NULL))
    {
        if (index != (uint16) - 1)
        {
            //���attrib<0>==0��֡��Ӧid�б�
            if ((picbox.attrib & PICBOX_FRAME_OPTION) == PICBOX_FRAME_OPTION_ID)
            {
                if (index < 12)
                {
                    res_id = picbox.frame[index];
                }
            }
            else
            {
                frame_entry_offset = (uint32) picbox.frame[1] << 16;
                frame_entry_offset += (uint32) picbox.frame[0];
                sys_sd_fseek(style_ptr->res_fp, SEEK_SET, frame_entry_offset + (uint32) index * sizeof(uint16));
                sys_sd_fread(style_ptr->res_fp, &res_id, sizeof(res_id));
            }
        }
        else if (picbox.id != (uint16) - 1)
        {
            res_id = picbox.id;
        }
        else
        {
            return UI_PARAM_ERR;
        }
    }
    if (res_id != (uint16) - 1)
    {
        u_show_pic(res_id, picbox.x, picbox.y);
    }

    return UI_NO_ERR;
}
/*! \endcond */

/*! \cond */

ui_result_e show_picbox_id3(style_infor_t *picbox_style, picbox_private_t *picbox_data, uint16 start_row)
{
    picbox_t picbox;
    uint32 frame_entry_offset;
    res_open_infor_t *style_ptr;
    uint16 bg_res_id, res_id, res1_id;
    uint16 frame_cnt;
    uint8 index, index1;

    /*���ݴ���Դ�ļ������ͣ���ָ��ָ��ͬ��ȫ�ֱ���*/
    if (picbox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, style_ptr->style_picbox_offset + (uint32) (picbox_style->style_id
            & 0x0fff) * sizeof(picbox_t));
    sys_sd_fread(style_ptr->res_fp, &picbox, sizeof(picbox_t));
    //����ʾpicbox
    if ((picbox.attrib & PICBOX_SHOW_EN) != PICBOX_SHOW_EN)
    {
        return UI_NO_DISPLAY;
    }
    //��ȡpicbox ֡��
    frame_cnt = PICBOX_FRAME_COUNT(picbox.attrib);
    index = picbox_data->frame_id;
    if (index >= frame_cnt)//֡�Ŵ���
    {
        return UI_PARAM_ERR;
    }
    
    if (index < (frame_cnt - 1))
    {
        index1 = index + 1;
    }
    else
    {
        index1 = 1;
    }

    //��ȡ֡��ӦͼƬid
    if ((picbox.attrib & PICBOX_FRAME_OPTION) == PICBOX_FRAME_OPTION_ID)
    {
        res_id = picbox.frame[index];
        res1_id = picbox.frame[index1];
        bg_res_id = picbox.frame[0];
    }
    else
    {
        frame_entry_offset = (uint32) picbox.frame[1] << 16;
        frame_entry_offset += (uint32) picbox.frame[0];
        sys_sd_fseek(style_ptr->res_fp, SEEK_SET, frame_entry_offset + (uint32) index * sizeof(uint16));
        sys_sd_fread(style_ptr->res_fp, &res_id, sizeof(res_id));
        
        if(start_row > 0)
        {
            sys_sd_fseek(style_ptr->res_fp, SEEK_SET, frame_entry_offset + (uint32) index1 * sizeof(uint16));
            sys_sd_fread(style_ptr->res_fp, &res1_id, sizeof(res_id));
        }
        else
        {
            res1_id = 0;//for QAC/QAM
        }
        
        sys_sd_fseek(style_ptr->res_fp, SEEK_SET, frame_entry_offset);
        sys_sd_fread(style_ptr->res_fp, &bg_res_id, sizeof(res_id));
    }
    
    //��ʾID3ͼ�꣬����Ч��
    if(show_pic_id3(bg_res_id, res_id, res1_id, &picbox, start_row) == TRUE)
    {
        return UI_NEED_SCROLL;
    }
    else
    {
        return UI_NO_ERR;
    }
}

bool show_pic_id3(uint16 bg_res_id, uint16 res_id, uint16 res1_id, picbox_t *picbox, uint16 start_row)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint8 *display_buffer_tmp = display_buffer + 512;
    res_open_infor_t *back_ptr, *res_ptr;
    res_entry_t back_entry, res_entry;
    region_t back_region;

    //���ر���ͼƬ
    back_ptr = res_ptr_select(bg_res_id);
    sys_sd_fseek(back_ptr->res_fp, SEEK_SET, \
            back_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(bg_res_id) * RESHEADITEM));
    sys_sd_fread(back_ptr->res_fp, &back_entry, sizeof(res_entry_t));
    
    //��ȡ����ͼƬ����
    sys_sd_fseek(back_ptr->res_fp, SEEK_SET, back_entry.offset);
    sys_sd_fread(back_ptr->res_fp, display_buffer, back_entry.width * back_entry.height * 2);
    
    //������ʾ����
    back_region.x = picbox->x;
    back_region.y = picbox->y;
    back_region.width = back_entry.width;
    back_region.height = back_entry.height;
    
    if(start_row <= back_entry.height)//����δ���
    {
        if(start_row < back_entry.height)//��ʾres_id�����start_row Ϊ back_entry.height������ʾres1_id
        {
            //����res_id
            res_ptr = res_ptr_select(res_id);
            sys_sd_fseek(res_ptr->res_fp, SEEK_SET, \
                    res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(res_id) * RESHEADITEM));
            sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
            
            //��ȡ��Ҫ��ʾ����ͼƬ����
            sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_entry.offset + res_entry.width * start_row * 2);
            sys_sd_fread(res_ptr->res_fp, display_buffer_tmp, res_entry.width * (res_entry.height - start_row) * 2);
            
            //����res_id��������
            pic_overlay_id3(display_buffer, display_buffer_tmp, &back_region, start_row, UP_HALF_DEF);
        }
    
        if(start_row == 0)//����ʾres_id
        {
            goto disp_id3;
        }
        
        //����res1_id
        res_ptr = res_ptr_select(res1_id);
        sys_sd_fseek(res_ptr->res_fp, SEEK_SET, \
                res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(res1_id) * RESHEADITEM));
        sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
        
        //��ȡ��Ҫ��ʾ����ͼƬ����
        sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_entry.offset);
        sys_sd_fread(res_ptr->res_fp, display_buffer_tmp, res_entry.width * start_row * 2);
        
        //����res_id��������
        pic_overlay_id3(display_buffer, display_buffer_tmp, &back_region, start_row, DOWN_HALF_DEF);
        
        //��ʾ����ok id3ͼƬ
        disp_id3:
        put_image_id3(&back_region);
        return TRUE;
    }
    else//�������
    {
        return FALSE;
    }
}

//��͸��ͼ����ӵ����������� mode UP_HALF_DEF��ʾ�ϰ벿�ֵ��ӣ�DOWN_HALF_DEF��ʾ�°벿�ֵ���
void pic_overlay_id3(uint8 *pixel_result, uint8 *pic_source, region_t *b_region, uint16 start_row, uint8 mode)
{
    uint16 x, y, k;
    uint16 *pixel_result_16 = (uint16 *)pixel_result;
    uint16 *pic_source_16 = (uint16 *)pic_source;
    uint16 begin_row, end_row;
    
    if(mode == UP_HALF_DEF)
    {
        begin_row = 0;
        end_row = b_region->height - start_row;
    }
    else
    {
        begin_row = b_region->height - start_row;
        end_row = b_region->height;
    }
    for(y = begin_row, k = 0; y < end_row; y++, k++)
    {
        for(x = 0; x < b_region->width; x++)
        {
            if(pic_source_16[k * b_region->width + x] != 0)
            {
                pixel_result_16[y *b_region->width + x] = pic_source_16[k * b_region->width + x];
            }
        }
    }
}

//���ͼƬ���ݵ� lcd ��
bool put_image_id3(region_t *img_region)
{
    uint32 count;
    bool result = TRUE;

    //����ͼƬ��ʾģʽ������
    set_lcd_param_hook(img_region, LCD_PARAM_IMAGE);

    count = ((uint32) img_region->width) * ((uint32) img_region->height) * 2;
    //DMA��GRAM��
    if (dma_buffer_data_hook(count / 2, img_region->width) == FALSE)
    {
        result = FALSE;
    }

    return result;
}

/*! \endcond */
