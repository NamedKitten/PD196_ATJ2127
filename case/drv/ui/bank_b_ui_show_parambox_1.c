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
 * \file     ui_show_parambox_1.c
 * \brief    ��ʾparambox�Ĳ���ֵ��ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ��parambox num��ؼ����ݵĽ�������ʾ����ֵ
 * \par      EXTERNALIZED FUNCTIONS:
 *              ֱ�ӵ���ϵͳSD�ļ�ϵͳ�Ľӿں�����������ʾ����ֵ
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

#define PARAMBOX_LISTDIRECT_OPTION_SHIFT    (4)
#define PARAMBOX_LISTDIRECT_OPTION      (0x0001 << PARAMBOX_LISTDIRECT_OPTION_SHIFT)
#define PARAMBOX_LISTDIRECT_HORIZONTAL  (0x0000 << PARAMBOX_LISTDIRECT_OPTION_SHIFT)
#define PARAMBOX_LISTDIRECT_VERTICAL    (0x0001 << PARAMBOX_LISTDIRECT_OPTION_SHIFT)

//parambox number attribute bits location
#define NUMBER_SHOW_PLUS_EN             (0x0001 << 0)
#define NUMBER_SHOW_ZERO_EN             (0x0001 << 1)
#define NUMBER_DISP_OPTION_SHIFT        (2)
#define NUMBER_DISP_OPTION              (0x0001 << NUMBER_DISP_OPTION_SHIFT)
#define DISP_OPTION_CHAR                (0x0000 << NUMBER_DISP_OPTION_SHIFT)
#define DISP_OPTION_PIC                 (0x0001 << NUMBER_DISP_OPTION_SHIFT)
#define NUMBER_INACTIVE_BG_OPTION_SHIFT (3)
#define NUMBER_INACTIVE_BG_OPTION       (0x0003 << NUMBER_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_COLOR        (0x0000 << NUMBER_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_SPIC         (0x0001 << NUMBER_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_MPIC         (0x0002 << NUMBER_INACTIVE_BG_OPTION_SHIFT)
#define NUMBER_ACTIVE_BG_OPTION_SHIFT   (5)
#define NUMBER_ACTIVE_BG_OPTION         (0x0003 << NUMBER_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_COLOR          (0x0000 << NUMBER_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_SPIC           (0x0001 << NUMBER_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_MPIC           (0x0002 << NUMBER_ACTIVE_BG_OPTION_SHIFT)
#define NUMBER_ALIGNMENT_OPTION_SHIFT   (7)
#define NUMBER_ALIGNMENT_OPTION         (0x0003 << NUMBER_ALIGNMENT_OPTION_SHIFT)
#define NUMBER_ALIGNMENT_OPTION_LEFT    (0x0000 << NUMBER_ALIGNMENT_OPTION_SHIFT)
#define NUMBER_ALIGNMENT_OPTION_CENTER  (0x0001 << NUMBER_ALIGNMENT_OPTION_SHIFT)
#define NUMBER_ALIGNMENT_OPTION_RIGHT   (0x0002 << NUMBER_ALIGNMENT_OPTION_SHIFT)
#define NUMBER_FONT_OPTION_SHIFT        (9)
#define NUMBER_FONT_OPTION              (0x0003 << NUMBER_FONT_OPTION_SHIFT)
#define NUMBER_FONT_OPTION_SMALL        (0x0000 << NUMBER_FONT_OPTION_SHIFT)
#define NUMBER_FONT_OPTION_MIDDLE       (0x0001 << NUMBER_FONT_OPTION_SHIFT)
#define NUMBER_FONT_OPTION_LARGE        (0x0002 << NUMBER_FONT_OPTION_SHIFT)
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾparambox�Ĳ���ֵ
 * \param[in]   parambox��parambox�ؼ����ݽṹ�����
 * \param[in]   parambox_data��Ӧ�ô��ݵ�parambox��˽�����ݽṹ��ָ��
 * \param[in]   itemnum���ڼ���������
 * \param[in]   type����ǰ���Ƿ�Ϊ������
 * \param[in]   mode��parambox��ˢ��ģʽ
 * \param[in]   style_ptr����ǰ��Դ��Ϣ�ṹ��ָ��
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_parambox_num(parambox_t *parambox, parambox_private_t *parambox_data, uint16 itemnum, uint8 type,
        parambox_draw_mode_e mode, res_open_infor_t *style_ptr)
{
    parambox_num_t parambox_num;
    res_entry_t res_entry;
    region_t disp_region;
    res_open_infor_t *res_ptr;
    uint16 x, bak_x;
    uint16 x_left = 0;
    uint16 x_right = 0;
    uint16 back_x, back_y;
    uint16 length;
    uint16 textout_mode = 0;
    uint16 num_back_id = -1;
    uint8 string[10];
    uint8 num_bits = 0;
    int8 n = 0;
    uint8 plus_flag = 0;
    uint8 j;
    string_desc_t desc;

    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, parambox->num_addr);
    sys_sd_fread(style_ptr->res_fp, &parambox_num, sizeof(parambox_num_t));
    if ((mode == PARAMBOX_DRAW_ALL) || (mode == PARAMBOX_DRAW_PARAM)) //��ʼ����任����ʱ��Ҫ�ػ�����
    {
        param_bits = 0;
    }

    //�Ǽ����������
    if (type == 0)
    {
        if ((parambox_num.attrib & NUMBER_INACTIVE_BG_OPTION) == INACTIVE_BG_OPTION_COLOR)
        {
            romf_set_backgd_color(parambox_num.back_id, 0, 0);
            textout_mode |= CHAR_FILL_COLOR;
            //clear_screen(&disp_region,0,0);
            //romf_fill_rect(&disp_region, (uint16) (dc.backgd_color & 0xffff));
        }
        else
        {
            textout_mode |= CHAR_FILL_NORMAL;
            if ((parambox_num.attrib & NUMBER_INACTIVE_BG_OPTION) == INACTIVE_BG_OPTION_SPIC)
            {
                num_back_id = parambox_num.back_id;
            }
            else if ((parambox_num.attrib & NUMBER_INACTIVE_BG_OPTION) == INACTIVE_BG_OPTION_MPIC)
            {
                num_back_id = parambox_num.back_id + itemnum;
            }
            else
            {
                //do nothing for QAC
            }
            res_ptr = res_ptr_select(num_back_id);
            //��ȡͼƬ�����Ϣ
            sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (num_back_id & 0x7fff)
                    * RESHEADITEM);
            sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
            back_x = parambox->list_x[itemnum] + (parambox->list_width[itemnum] - res_entry.width) / 2;
            back_y = parambox->list_y[itemnum];
            u_show_pic(num_back_id, back_x, back_y);
        }
    }
    //�����������
    else
    {
        if ((parambox_num.attrib & NUMBER_ACTIVE_BG_OPTION) == ACTIVE_BG_OPTION_COLOR)
        {
            romf_set_backgd_color(parambox_num.back_act_id, 0, 0);
            textout_mode |= CHAR_FILL_COLOR;
            //clear_screen(&disp_region,0,0);
            //romf_fill_rect(&disp_region, (uint16) (dc.backgd_color & 0xffff));
        }
        else
        {
            textout_mode |= CHAR_FILL_NORMAL;
            if ((parambox_num.attrib & NUMBER_ACTIVE_BG_OPTION) == ACTIVE_BG_OPTION_SPIC)
            {
                num_back_id = parambox_num.back_act_id;
            }
            else if ((parambox_num.attrib & NUMBER_ACTIVE_BG_OPTION) == ACTIVE_BG_OPTION_MPIC)
            {
                num_back_id = parambox_num.back_act_id + itemnum;
            }
            else
            {
                //do nothing for QAC
            }

            res_ptr = res_ptr_select(num_back_id);
            //��ȡͼƬ�����Ϣ
            sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (num_back_id & 0x7fff)
                    * RESHEADITEM);
            sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
            if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_CENTER)
            {
                back_x = parambox->list_x[itemnum] + (parambox->list_width[itemnum] - res_entry.width) / 2;
            }
            else if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_RIGHT)
            {
                back_x = parambox->list_x[itemnum] + parambox->list_width[itemnum] - res_entry.width;
            }
            else
            {
                back_x = parambox->list_x[itemnum];
            }
            
            back_y = parambox->list_y[itemnum];
            u_show_pic(num_back_id, back_x, back_y);  //modify--vzhan
        }
    }
    
    //������ʾ����
    if(parambox->param_cnt > 1)//�����
    {
        if((parambox->attrib & PARAMBOX_LISTDIRECT_OPTION) == PARAMBOX_LISTDIRECT_HORIZONTAL)//��������
        {
            disp_region.x = parambox->list_x[itemnum];
            disp_region.y = parambox_num.num_y;
            disp_region.width = parambox->list_width[itemnum];
            disp_region.height = parambox_num.num_height;
        }
        else//��������
        {
            disp_region.x = parambox_num.num_x;
            disp_region.y = parambox->list_y[itemnum];
            disp_region.width = parambox_num.num_width;
            disp_region.height = parambox->list_height[itemnum];
        }
    }
    else//������
    {
        disp_region.x = parambox_num.num_x;
        disp_region.y = parambox_num.num_y;
        disp_region.width = parambox_num.num_width;
        disp_region.height = parambox_num.num_height;
    }

    if (parambox_data->items[itemnum].value_str == NULL)
    {
        //��ʾ����ֵ����
        if ((parambox_num.attrib & NUMBER_SHOW_ZERO_EN) != NUMBER_SHOW_ZERO_EN)
        {
            num_bits = 0; //���ظ�λ����0
        }
        else
        {
            num_bits = parambox_data->items[itemnum].max_number;
        }

        if((parambox_data->sign_flag == 1)
        && ((parambox_num.attrib & NUMBER_SHOW_PLUS_EN) == NUMBER_SHOW_PLUS_EN))
        {
            plus_flag = 1; //��Ҫ��ʾ����
        }
        else
        {
            plus_flag = 0;//����ʾ����
        }
        
        if ((parambox_num.attrib & NUMBER_DISP_OPTION) == DISP_OPTION_PIC)
        {
            itoa_n((int32) parambox_data->items[itemnum].value, string, plus_flag, num_bits, 1);
        }
        else
        {
            itoa_n((int32) parambox_data->items[itemnum].value, string, plus_flag, num_bits, 0);
        }
    }
    //����ͼƬ��ʾģʽ
    if ((parambox_num.attrib & NUMBER_DISP_OPTION) == DISP_OPTION_PIC)
    {
        if (parambox_data->items[itemnum].value_str == NULL)
        {
            res_ptr = res_ptr_select(parambox_num.pic_id);
            sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (parambox_num.pic_id & 0x7fff)
                    * RESHEADITEM);
            sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
            length = ((uint8) libc_strlen(string)) * parambox_num.pic_width;
            //������ʾ��X����
            if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_LEFT)
            {
                //������ʾ
                bak_x = x = disp_region.x;
            }
            //������ʾ��X����
            else if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_RIGHT)
            {
                bak_x = x = disp_region.x + disp_region.width - length;
            }
            else
            {
                //Ĭ�Ͼ�����ʾ
                bak_x = x = disp_region.x + (disp_region.width - length) / 2;
            }
            for (j = 0; j < (uint8) libc_strlen(string); j++)
            {
                show_num_pic(string[j] - 0x30, x, disp_region.y, &res_entry, res_ptr);
                x += parambox_num.pic_width;
            }
            if (mode == PARAMBOX_DRAW_VALUE)
            {
                n = (int8) (param_bits - (uint8) libc_strlen(string));
                if (n > 0)//��������ۼ�����
                {
                    //for QAC �� if else if ����switch������1�����Ƕ��
                    switch (parambox_num.attrib & NUMBER_ALIGNMENT_OPTION)
                    {
                    //������ʾʱ������λ������ʱ�����߾����ñ���ͼ���ԭ��������
                    case NUMBER_ALIGNMENT_OPTION_CENTER:
                        n = (n/2) + (n%2);//for QAC ��Ϊȡһ�룬������������Ӷ�1
                        x_left = bak_x - n * parambox_num.pic_width;
                        x_right = bak_x + length;
                        break;
                        
                    //������ʾ��X����
                    case NUMBER_ALIGNMENT_OPTION_RIGHT:
                        x = bak_x - n * parambox_num.pic_width;
                        break;
                    
                    default:
                        //do nothing for QAC
                        break;
                    }
                }
                while (n > 0)//����ۼ�
                {
                    if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_CENTER)
                    {
                        show_num_pic(12, x_left, disp_region.y, &res_entry, res_ptr);
                        show_num_pic(12, x_right, disp_region.y, &res_entry, res_ptr);
                        x_left += parambox_num.pic_width;
                        x_right += parambox_num.pic_width;
                    }
                    else
                    {
                        show_num_pic(12, x, disp_region.y, &res_entry, res_ptr);
                        x += parambox_num.pic_width;
                    }
                    n--;
                }
            }
            param_bits = (uint8) libc_strlen(string);
        }
    }
    else //�ַ�����ʾģʽ
    {
        //atrib<9-10>0��ʾС����,1��ʾ�к����壬2��ʾ�����壬Ĭ���к�����
        if ((parambox_num.attrib & NUMBER_FONT_OPTION) == NUMBER_FONT_OPTION_SMALL)
        {
            textout_mode |= DISP_FONT_SMALL;
        }
        else if ((parambox_num.attrib & NUMBER_FONT_OPTION) == NUMBER_FONT_OPTION_LARGE)
        {
            textout_mode |= DISP_FONT_LARGE;
        }
        else
        {
            textout_mode |= DISP_FONT_NORMAL; //Ĭ���к�����
        }

        //���û�����ɫ
        if (type == 0)
        {
            romf_set_pen_color(parambox_num.num_color, 0, 0);
        }
        else
        {
            romf_set_pen_color(parambox_num.num_act_color, 0, 0);
        }
        
        //clear_screen(&disp_region, 0, 0);
        //romf_fill_rect(&disp_region, (uint16) (dc.backgd_color & 0xffff));
        //������ʾ
        if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_LEFT)
        {
            textout_mode |= DISP_ALIGN_LEFT;
        }
        //������ʾ
        else if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_RIGHT)
        {
            textout_mode |= DISP_ALIGN_RIGHT;
        }
        //������ʾ
        else
        {
            textout_mode |= DISP_ALIGN_MEDIACY;
        }
        textout_mode |= DISP_ARABIC_DISABLE;
        if (parambox_data->items[itemnum].value_str == NULL)
        {
            desc.data.str = string;
            desc.length = (uint16) libc_strlen(string);
            desc.language = 0;
            show_string(&desc, &disp_region, textout_mode);
        }
        else
        {
            desc.data.str = parambox_data->items[itemnum].value_str;
            desc.length = (uint16) libc_strlen(parambox_data->items[itemnum].value_str);
            desc.language = 0;
            show_string(&desc, &disp_region, textout_mode);
        }
    }
}
/*! \endcond */
