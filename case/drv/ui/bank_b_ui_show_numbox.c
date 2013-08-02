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
 * \file     ui_show_numbox.c
 * \brief    ��ʾnumbox�ĺ���ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ��dislogbox�ؼ����ݵĽ�������ʾdialogbox
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���ϵͳSD�ļ�ϵͳ�Ľӿں�����������ʾdialogbox
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//numbox attribute bits location
#define NUMBOX_SHOW_EN                  (0x0001 << 0)
#define NUMBOX_PLUS_EN                  (0x0001 << 1)
#define NUMBOX_ALIGNMENT_OPTION_SHIFT   (2)
#define NUMBOX_ALIGNMENT_OPTION         (0x0003 << NUMBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_LEFT           (0x0000 << NUMBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_CENTER         (0x0001 << NUMBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_RIGHT          (0x0002 << NUMBOX_ALIGNMENT_OPTION_SHIFT)
#define NUMBOX_COMPOSE_OPTION_SHIFT     (4)
#define NUMBOX_COMPOSE_OPTION           (0x0001 << NUMBOX_COMPOSE_OPTION_SHIFT)
#define COMPOSE_OPTION_DECIMAL          (0x0000 << NUMBOX_COMPOSE_OPTION_SHIFT)
#define COMPOSE_OPTION_RATIO            (0x0001 << NUMBOX_COMPOSE_OPTION_SHIFT)
#define NUMBOX_DISP_OPTION_SHIFT        (5)
#define NUMBOX_DISP_OPTION              (0x0001 << NUMBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_CHAR                (0x0000 << NUMBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_PIC                 (0x0001 << NUMBOX_DISP_OPTION_SHIFT)

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡnumbox�Ŀؼ�������Ϣ��������show_num����numbox����ʾ
 * \param[in]   numbox_style ָ��numbox��style�ṹ�����
 * \param[in]   numbox_data Ӧ�ô��ݵ�numbox��˽�����ݽṹ��ָ��
 * \param[in]   mode numbox��ˢ��ģʽ���μ�numbox_draw_mode_e����
 * \param[out]  none
 * \return      ui_result_e ���͵Ĳ������μ�ui_result_e����
 * \ingroup     ui_controls
 * \par         exmaple code
 * \code
    ����1����ʾ XX/XXX ���֣������ numberbox ����Ϊ��Ӧģʽ
    style_infor_t numbox_style;
    numbox_private_t numbox_param;
    
    numbox_style.style_id = NUMBOX_ID1;
    numbox_style.type = UI_AP;
    numbox_param.value = g_file_num;
    numbox_param.total = g_file_total;
    
    ui_show_numbox(&numbox_style, &numbox_param, NUMBOX_DRAW_ALL);
    
    ����2����ʾ XXXX.X ���֣������ numberbox ����Ϊ��Ӧģʽ
    style_infor_t numbox_style;
    numbox_private_t numbox_param;
    
    numbox_style.style_id = NUMBOX_ID1;
    numbox_style.type = UI_AP;
    numbox_param.value = g_fm_freq;
    
    ui_show_numbox(&numbox_style, &numbox_param, NUMBOX_DRAW_ALL);
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e show_numbox(style_infor_t *numbox_style, numbox_private_t *numbox_data, numbox_draw_mode_e mode)
{
    numbox_t numbox;
    uint32 numbox_entry_addr;
    res_open_infor_t *style_ptr;
    if (numbox_data == NULL)
    {
        return UI_PARAM_ERR;
    }
    /*���ݴ���Դ�ļ������ͣ���ָ��ָ��ͬ��ȫ�ֱ���*/
    if (numbox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    //��ȡnumbox��style��ʼ��ַ
    get_ctrl_addr(NUMBOX, &numbox_entry_addr, style_ptr);
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, numbox_entry_addr + (uint32) (numbox_style->style_id & 0x0fff)
            * sizeof(numbox_t));
    sys_sd_fread(style_ptr->res_fp, &numbox, sizeof(numbox_t));
    //����ʾnumbox
    if ((numbox.attrib & NUMBOX_SHOW_EN) != NUMBOX_SHOW_EN)
    {
        return UI_NO_DISPLAY;
    }
    if ((numbox.attrib & NUMBOX_DISP_OPTION) == DISP_OPTION_PIC)
    {
        show_numbox_pic(numbox_data, &numbox, mode);
    }
    else
    {
        show_numbox_string(numbox_data, &numbox, mode);
    }
    return UI_NO_ERR;
}
/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾnumbox
 * \param[in]   numbox_data��Ӧ�ô��ݵ�numbox��˽�����ݽṹ��ָ��
 * \param[in]   numbox��numbox�Ľṹ�����
 * \param[in]   mode��numbox��ˢ��ģʽ
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_numbox_pic(numbox_private_t *numbox_data, numbox_t *numbox, numbox_draw_mode_e mode)
{
    show_number_param_t number_param;
    uint8 string[12];
    uint8 string1[6];
    uint8 string2[6];
    uint8 num_bits[2] = { 0, 0 };

    libc_memset(string, 0, sizeof(string));
    num_bits[0] = (uint8) (numbox->position & 0x000f);
    num_bits[1] = ((uint8) (numbox->position & 0x00f0)) >> 4;
    
    //��ʼ������
    number_param.back_id = numbox->back_id;
    number_param.x = numbox->x;
    number_param.y = numbox->y;
    number_param.width = numbox->width;
    number_param.num0_id = numbox->num0_id;
    number_param.partition_id = numbox->partition_id;
    number_param.numpic_back_color = numbox->numpic_back_color;
    number_param.num_width = numbox->pic_width;
    number_param.partition_width = numbox->partition_width;
    
    if ((numbox->attrib & NUMBOX_COMPOSE_OPTION) == COMPOSE_OPTION_DECIMAL) //ֻ��һ��������ʾ
    {
        number_param.part_pos = (uint8) ((numbox->position & 0xff00) >> 8);
        itoa_n(numbox_data->value, string, (uint8) (numbox->attrib & NUMBOX_PLUS_EN), (uint16) num_bits[0], 1);
        number_param.part_pos = (uint8) libc_strlen(string) - number_param.part_pos;
    }
    else //����������ɵ�numbox����ʾ
    {
        itoa_n(numbox_data->value, string1, (uint8) (numbox->attrib & NUMBOX_PLUS_EN), (uint16) num_bits[0], 1);
        itoa_n(numbox_data->total, string2, (uint8) (numbox->attrib & NUMBOX_PLUS_EN), (uint16) num_bits[1], 1);
        libc_strncpy(string, string1, libc_strlen(string1));
        libc_strncat(string, string2, libc_strlen(string2));
        number_param.part_pos = (uint8) libc_strlen(string1);
    }
    
    if ((numbox->attrib & NUMBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_CENTER)
    {
        number_param.align = 1;
    }
    //������ʾ��X����
    else if ((numbox->attrib & NUMBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_RIGHT)
    {
        number_param.align = 2;
    }
    else
    {
        number_param.align = 0;
    }
    
    show_number_pic(string, &number_param, mode);
}

/******************************************************************************/
/*!
 * \par  Description:
 *     ������ͼƬģʽ��ʾ����
 * \param[in]   num_string Ҫ��ʾ�����ֵ��ַ���
 * \param[in]   number_param ������ͼƬ��ʽ��ʾ���ֲ�������
 * \param[in]   mode ����ˢ��ģʽ
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_number_pic(uint8 *num_string, show_number_param_t *number_param, numbox_draw_mode_e mode)
{
    transparent_pic_t trs_pic;
    res_entry_t res_entry;
    res_open_infor_t *res_ptr;
    uint16 x = 0;
    uint16 back_x = 0;
    uint16 x_left = 0;
    uint16 x_right = 0;
    uint16 length;
    uint8 j;
    uint8 disp_bits;
    uint8 temp_num;
    int8 n = 0;

    if (mode == NUMBOX_DRAW_ALL)
    {
        libc_memset(numbox_disp_prev, 0x0, sizeof(numbox_disp_prev));
        num_total_bits = 0;
    }

    //��ȡ����ͼƬ��Ϣ
    res_ptr = res_ptr_select(number_param->num0_id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (number_param->num0_id & 0x7fff)
            * RESHEADITEM);
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
    
    disp_bits = (uint8) libc_strlen(num_string);
    if (num_total_bits != disp_bits)//λ�������仯����ˢ�����������numbox
    {
        if (number_param->back_id != (uint16)(-1))
        {
            u_show_pic(number_param->back_id, number_param->x, number_param->y);
        }
        libc_memset(numbox_disp_prev, 0x0, sizeof(numbox_disp_prev));
    }
    
    //��ʾ�ָ���
    if (number_param->partition_id != (uint16) - 1)
    {
        length = (uint16) (libc_strlen( num_string)) * number_param->num_width + number_param->partition_width;
    }
    else
    {
        length = (uint16) (libc_strlen( num_string)) * number_param->num_width;
    }
    //������ʾ��X����
    if (number_param->align == 0)
    {
        back_x = x = number_param->x;
    }
    //������ʾ��X����
    else if (number_param->align == 1)
    {
        back_x = x = number_param->x + (number_param->width - length) / 2;
    }
    //������ʾ��X����
    else
    {
        back_x = x = number_param->x + (number_param->width - length);
    }
    
    if(number_param->back_id != (uint16)(-1))
    {
        trs_pic.back_id = number_param->back_id;
        trs_pic.back_x = number_param->x;
        trs_pic.back_y = number_param->y;
        trs_pic.back_color = number_param->numpic_back_color;
        trs_pic.clear = 0;
        trs_pic.direct = 0;
        trs_pic.pic_y = number_param->y;
    }
    
    for (j = 0; j < disp_bits; j++)
    {
        //��ʾ�ָ���
        if (number_param->partition_id != (uint16) - 1)
        {
            if (j == number_param->part_pos)
            {
                if(num_string[j] != numbox_disp_prev[j])
                {
                    if(number_param->back_id != (uint16)(-1))
                    {
                        trs_pic.pic_id = number_param->partition_id;
                        trs_pic.pic_x = x;
                        show_pic_transparent(&trs_pic);
                    }
                    else
                    {
                        u_show_pic(number_param->partition_id, x, number_param->y);
                    }
                }
                x += number_param->partition_width;
            }
        }
        
        if(num_string[j] != numbox_disp_prev[j])
        {
            temp_num = num_string[j] - 0x30;
            if(number_param->back_id != (uint16)(-1))
            {
                trs_pic.pic_id = number_param->num0_id + temp_num;
                trs_pic.pic_x = x;
                show_pic_transparent(&trs_pic);
            }
            else
            {
                show_num_pic(temp_num, x, number_param->y, &res_entry, res_ptr);
            }
        }
        x += number_param->num_width;
    }
    
    if (number_param->back_id == (uint16)(-1))
    {
        n = (int8) (num_total_bits - (uint8) libc_strlen(num_string));
        if (n > 0)
        {
            //������ʾʱ������λ������ʱ�����߾����ñ���ͼ���ԭ��������
            if (number_param->align == 1)
            {
                if ((n % 2) == 0)
                {
                    n = n / 2;
                }
                else
                {
                    n = n / 2 + 1;
                }
                x_left = back_x - n * number_param->num_width;
                x_right = back_x + length;
            }
            //������ʾ��X����
            else if (number_param->align == 2)
            {
                x = back_x - n * number_param->num_width;
            }
            else
            {
                //do nothing for QAC
            }
        }
        while (n > 0)//û��ʵʱ�ӱ���ͼnumbox����Ҫ�ÿ�ͼ�����
        {
            if (number_param->align == 1)
            {
                show_num_pic(10, x_left, number_param->y, &res_entry, res_ptr);
                show_num_pic(10, x_right, number_param->y, &res_entry, res_ptr);
                x_left += number_param->num_width;
                x_right += number_param->num_width;
            }
            else
            {
                show_num_pic(10, x, number_param->y, &res_entry, res_ptr);
                x += number_param->num_width;
            }
            n--;
        }
    }
    
    libc_memcpy(numbox_disp_prev, num_string, 12);
    num_total_bits = disp_bits;
}

/*! \endcond */
