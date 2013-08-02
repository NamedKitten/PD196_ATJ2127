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
 * \file     ui_scroll_string.c
 * \brief    ����ģ��
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ʵ���ַ����������
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

bool hori_scroll(uint16 mode, int8 language, region_t *scroll, uint16 str_len);

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    ����scroll_string_horizontalʵ�ֺ������
 * \param[in]   scroll_infor��������Ϣ�ṹ��ָ��
 * \param[in]   scroll_region����������
 * \param[in]   scroll_mode������ģʽ
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
bool scroll_string(string_desc_t *scroll_infor, region_t *scroll_region, scroll_mode_t scroll_mode) //����
{
    uint16 str_len = 0xffff;
    uint8 i = 0;
    if (scroll_mode.mode == TRUE)
    {
        // ���ַ�����������2���ո�
        if ((scroll_mode.disp_mode & DISP_SCROLL_REPEAT) != 0) //��Ҫѭ������ʱ����Ҫ�ӿո�
        {
            if (scroll_infor->language == UNICODELANGUAGE)
            {
                while (1)
                {
                    if (i >= 102)
                    {
                        break;
                    }
                    if ((*(scroll_infor->data.str + i) == 0x00) && (*(scroll_infor->data.str + i + 1) == 0x00))
                    {
                        *(scroll_infor->data.str + i) = 0x20;
                        *(scroll_infor->data.str + i + 1) = 0x00;
                        i += 2;
                        *(scroll_infor->data.str + i) = 0x20;
                        *(scroll_infor->data.str + i + 1) = 0x00;
                        i += 2;
                        *(scroll_infor->data.str + i) = 0x00;
                        *(scroll_infor->data.str + i + 1) = 0x00;
                        break;
                    }
                    else
                    {
                        i += 2;
                    }
                }
            }
            else
            {
                libc_strncat(scroll_infor->data.str, "  ", 2);
            }
        }
        scroll_desc.scroll_width = 0;
        // ���˵�Unicode��־0xfffe
        if ((*(scroll_infor->data.str) == 0xff) && (*((scroll_infor->data.str) + 1) == 0xfe))
        {
            scroll_infor->data.str += 2;
        }
        scroll_desc.scrollhead_pt = scroll_desc.strhead_pt = scroll_infor->data.str;
        libc_memcpy(&(scroll_desc.scroll_region), scroll_region, sizeof(region_t));
        scroll_desc.language = scroll_infor->language;
        // �������ַ���������ʾʱ��Ĭ��Ϊ�����
        if ((scroll_mode.disp_mode & DISP_ALIGN_OPTION) == DISP_ALIGN_MEDIACY)
        {
            scroll_mode.disp_mode &= (~DISP_ALIGN_OPTION);
        }
        scroll_desc.disp_mode = scroll_mode.disp_mode;
        return TRUE;
    }
    else
    {
        scroll_desc.scroll_width += 1;
        return hori_scroll(scroll_desc.disp_mode, scroll_desc.language, &(scroll_desc.scroll_region), str_len);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ʵ�ֺ������
 * \param[in]   mode���ַ�����ʾģʽ
 * \param[in]   language���ַ��������ͣ�unicode������
 * \param[in]   scroll����������
 * \param[in]   str_len���ַ������ȣ����ص㣩
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
bool hori_scroll(uint16 mode, int8 language, region_t *scroll, uint16 str_len)
{
    region_t kregion;//���²�������
    region_t cur_region;
    uint8 *nextchar;
    uint8 char_width;
    uint8 pt_step;
    string_desc_t string_infor;
    uint16 uni_code;
    uint8 uni_type;

    uni_type = (uint8) (language == UNICODELANGUAGE);
    libc_memcpy(&kregion, scroll, sizeof(region_t));
    
#if (USE_MULTI_FONTLIB != 0)
    //�л������ֿ�
    fix_switch_fontlib(mode);
#endif
    
    scroll_again:
    libc_memcpy(&cur_region, scroll, sizeof(region_t));
    if (uni_type != 0)
    {
        uni_code = *(scroll_desc.scrollhead_pt) + ((uint16) (*(scroll_desc.scrollhead_pt + 1)) << 8);
    }
    // ����Ѿ�������ͷ�򷵻�ͷ���¿�ʼ
    if ((uni_type && (uni_code == 0x0000)) || ((!uni_type) && (*(scroll_desc.scrollhead_pt) == '\0')))
    {
        scroll_desc.scrollhead_pt = scroll_desc.strhead_pt;
        scroll_desc.scroll_width = 0;
        //�������ѭ���������򷵻�FALSE�����Ҫ���Ŵ�ͷ��ʼ��������Ч��������ͣһ�ι���
        if((mode & DISP_SCROLL_OPTION_SHIFT) != DISP_SCROLL_REPEAT)
        {
            //�������������̣��������һ�ι����߽�������ص�
            if (need_arabic_deal(mode) == TRUE)
            {
                cur_region.x = scroll->width + scroll->x - 1;
                cur_region.y = text_bg_desc.back_y;
                cur_region.width = 1;
                clear_remain_pixel_sdimage(&cur_region);
            }
            else
            {
                cur_region.width = 1;
                cur_region.y = text_bg_desc.back_y;
                clear_remain_pixel_sdimage(&cur_region);
            }

            return FALSE;
        }
        else
        {
            goto scroll_again;
        }
    }

    //�����ͷ��һ���������ַ�
    pt_step = get_next_char_point_hook(scroll_desc.scrollhead_pt, language, fix_p_char_point);
    char_width = fix_p_char_point[fix_font_size - 1];
    nextchar = scroll_desc.scrollhead_pt + pt_step;

    //��ǰ�����ַ��������, ������һ���ַ�
    if (scroll_desc.scroll_width >= char_width)
    {
        scroll_desc.scroll_width = 0;
        scroll_desc.scrollhead_pt = nextchar;
        goto scroll_again;
    }
    
    //�������û�����ɫ�ͱ���ɫ
    romf_set_pen_color(scroll_desc.pen_color, 0, 0);
    if ((scroll_desc.disp_mode & DISP_CHAR_FILL_OPTION) == CHAR_FILL_COLOR)
    {
        romf_set_backgd_color(scroll_desc.back_id, 0, 0);
    }
    
    //�����һ���ַ�������ʾ1~width��
    kregion.width = char_width - scroll_desc.scroll_width;
    if (need_arabic_deal(mode) == TRUE)
    {
        kregion.x = scroll->width + scroll->x - kregion.width;//set start x
    }
    
    char_out_rect(scroll_desc.scrollhead_pt, language, &kregion, mode & DISP_CHAR_FILL_OPTION,
            scroll_desc.scroll_width, 1);

    if (need_arabic_deal(mode) == TRUE)
    {
        cur_region.x = scroll->x;
    }
    else
    {
        cur_region.x = scroll->x + kregion.width;
    }
    
    //������������ַ���
    start_again1: 
    cur_region.width = scroll->width - kregion.width;
    string_infor.data.str = nextchar;
    string_infor.length = -1;
    string_infor.language = language;
    nextchar = put_string(&string_infor, &cur_region, mode);
    if (string_infor.result != STRING_TAIL_REGION_ERR)
    {
        romf_get_text_pos(&cur_region.x, &cur_region.y);
        if ((mode & DISP_SCROLL_REPEAT) != 0)
        {
            //����ַ���ͷ���������������ĩβ�������ص�
            nextchar = scroll_desc.strhead_pt;
            goto start_again1;
        }
        else
        {
            //����հ����ص㣬����������߽�������ص�
            if (need_arabic_deal(mode) == TRUE)
            {
                if(region.width >= 1)
                {
                    cur_region.x = region.x + region.width - 1;
                    cur_region.width = 1;
                    clear_remain_pixel_sdimage(&cur_region);
                }
            }
            else
            {
                if(cur_region.x <= (scroll->x + scroll->width - 1))
                {
                    cur_region.width = 1;
                    clear_remain_pixel_sdimage(&cur_region);
                }
            }
        }
    }
    
    return TRUE;
}
/*! \endcond */
