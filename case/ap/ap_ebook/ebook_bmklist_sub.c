/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_bookmark_sub.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo		2011-11-11         v1.0              create this file
 ********************************************************************************/

#include "ebook.h"

//�ַ�"P_"
const uint8 page_str[2] =
{ "p_" };
//�ַ�"*"
const uint8 page_multiply[1] =
{ "*" };
/****************************************************************************/
/*									�����												       */
/****************************************************************************/

/********************************************************************************
 * Description :�л�����һ����ǩ�ļ�
 *
 * Arguments  :
 *			  	mlst_ctl:�˵��б���ƽṹ��((����common_ui.h�е�dir_control_t))
 *			 	 line_num:�л�������;0:�л�1��;1:�л�2�У������Դ�����
 * Returns     :
 *           	    ��
 * Notes       :
 *
 ********************************************************************************/
void _select_next_item(dir_control_t *mlst_ctl, uint8 line_num)
{
    //����б������������л����������򲻽��и���
    if (mlst_ctl->list_total <= line_num)
    {
        return;
    }
    if (line_num < 1)
    {
        line_num = 1;
    }
    //��ҳ���л�����δ��ҳ�����һ����¼�����
    if ((mlst_ctl->list_no + line_num) < mlst_ctl->bottom)
    {
        just_change_active_next: mlst_ctl->list_no += line_num;
        mlst_ctl->old = mlst_ctl->active;
        mlst_ctl->active += line_num;
        mlst_ctl->draw_mode = LIST_DRAW_ACTIVE;
    }
    else
    {
        //���б����һ������б�ͷ��ʾ
        if (mlst_ctl->list_no == (mlst_ctl->list_total - 1))
        {
            mlst_ctl->list_no = 0;
            //����һҳ������£�ֻ����¼�����
            if (mlst_ctl->list_total <= LIST_NUM_ONE_PAGE_MAX)
            {
                mlst_ctl->old = mlst_ctl->active;
                mlst_ctl->active = 0;
                mlst_ctl->draw_mode = LIST_DRAW_ACTIVE;
            }
            //��β�л���ͷ�����������б�
            else
            {
                mlst_ctl->top = 0;
                mlst_ctl->bottom = LIST_NUM_ONE_PAGE_MAX - 1;
                mlst_ctl->old = mlst_ctl->active = 0;
                mlst_ctl->update_mode = LIST_UPDATE_HEAD;
                mlst_ctl->draw_mode = LIST_DRAW_LIST;
            }
        }
        //���б�����line_num���������е����һ��
        else if (mlst_ctl->list_no >= (mlst_ctl->list_total - 1 - line_num))
        {
            //���������еĵ����ڶ����1����һ�ֻ����¼�����
            if (line_num == 1)
            {
                goto just_change_active_next;
            }
            //�ж��2���3������һ����������б�
            else
            {
                mlst_ctl->list_no = mlst_ctl->list_total - 1;
                mlst_ctl->top = mlst_ctl->list_total - LIST_NUM_ONE_PAGE_MAX;
                mlst_ctl->bottom = mlst_ctl->list_no;
                mlst_ctl->old = mlst_ctl->active = LIST_NUM_ONE_PAGE_MAX - 1;
                mlst_ctl->draw_mode = LIST_DRAW_LIST;
            }
        }
        //�ڵ����ڶ�����Һ������㹻�б��������ҳ�����������б�
        else
        {
            mlst_ctl->list_no += line_num;
            mlst_ctl->top += line_num;
            mlst_ctl->bottom += line_num;
            mlst_ctl->old = mlst_ctl->active;
            mlst_ctl->update_mode = LIST_UPDATE_NEXT;
            mlst_ctl->draw_mode = LIST_DRAW_LIST;
        }
    }
    need_draw = TRUE;
}
/********************************************************************************
 * Description :�л�����һ����ǩ�ļ�
 *
 * Arguments  :
 *			  	mlst_ctl:�˵��б���ƽṹ��((����common_ui.h�е�dir_control_t))
 *			 	 line_num:�л�������;0:�л�1��;1:�л�2�У������Դ�����
 * Returns     :
 *               	��
 * Notes       :
 *
 ********************************************************************************/
void _select_prev_item(dir_control_t *mlst_ctl, uint8 line_num)
{
    //����б������������л����������򲻽��и���
    if (mlst_ctl->list_total <= line_num)
    {
        return;
    }
    if (line_num < 1)
    {
        line_num = 1;
    }
    if (mlst_ctl->active >= (1 + line_num))
    {
        //�ڵ�ǰҳ���л�����һ���ļ�
        just_change_active_prev: mlst_ctl->list_no -= line_num;
        mlst_ctl->old = mlst_ctl->active;
        mlst_ctl->active -= line_num;
        mlst_ctl->draw_mode = LIST_DRAW_ACTIVE; //���¼�����
    }
    else
    {
        if (mlst_ctl->list_no == 0)
        {
            mlst_ctl->list_no = (mlst_ctl->list_total - 1);
            if (mlst_ctl->list_total <= LIST_NUM_ONE_PAGE_MAX)
            {
                mlst_ctl->old = mlst_ctl->active;
                mlst_ctl->active = (mlst_ctl->list_total - 1);
                mlst_ctl->draw_mode = LIST_DRAW_ACTIVE; //���¼�����
            }
            else
            {
                //��ͷ�е�β
                mlst_ctl->top = mlst_ctl->list_total - LIST_NUM_ONE_PAGE_MAX;
                mlst_ctl->bottom = mlst_ctl->list_no;
                mlst_ctl->old = mlst_ctl->active = LIST_NUM_ONE_PAGE_MAX - 1;
                mlst_ctl->update_mode = LIST_UPDATE_TAIL; //��ͷ�е�β
                mlst_ctl->draw_mode = LIST_DRAW_LIST; //���������ļ��б�
            }
        }
        //���б�����line_num���������е����һ��
        else if (mlst_ctl->list_no <= line_num)
        {
            //���������еĵڶ����1���һ�ֻ����¼�����
            if (line_num == 1)
            {
                goto just_change_active_prev;
            }
            //�ж��2���3�����һ����������б�
            else
            {
                mlst_ctl->list_no = 0;
                mlst_ctl->top = 0;
                mlst_ctl->old = mlst_ctl->active;
                mlst_ctl->active = 0;
                if (mlst_ctl->list_total <= LIST_NUM_ONE_PAGE_MAX)
                {
                    mlst_ctl->bottom = mlst_ctl->list_total - 1;
                }
                else
                {
                    mlst_ctl->bottom = LIST_NUM_ONE_PAGE_MAX - 1;
                }
                mlst_ctl->draw_mode = LIST_DRAW_LIST;
            }
        }
        else
        {
            //������ҳ
            mlst_ctl->list_no -= line_num;
            mlst_ctl->top -= line_num;
            mlst_ctl->bottom -= line_num;
            mlst_ctl->old = mlst_ctl->active;
            mlst_ctl->update_mode = LIST_UPDATE_PREV; //������ҳ
            mlst_ctl->draw_mode = LIST_DRAW_LIST; //���������ļ��б�
        }
    }
    need_draw = TRUE;
}
/********************************************************************************
 * Description :�˵��б��ʼ��
 *
 * Arguments  :
 *				mlst_ctl:�˵��б���ƽṹ��(����common_ui.h�е�dir_control_t)
 *				total:��ʾ���ļ�����
 * Returns     :
 *                ��
 * Notes       :
 *
 ********************************************************************************/
void _bmklist_init(dir_control_t *mlst_ctl, uint16 total)
{
    mlst_ctl->list_no = 0;
    mlst_ctl->top = 0;
    mlst_ctl->list_total = total;
    mlst_ctl->bottom = mlst_ctl->top + LIST_NUM_ONE_PAGE_MAX - 1;
    mlst_ctl->draw_mode = LIST_DRAW_ALL;
    if (mlst_ctl->bottom > (mlst_ctl->list_total - 1))
    {
        mlst_ctl->bottom = mlst_ctl->list_total - 1;
    }
    mlst_ctl->old = mlst_ctl->active = mlst_ctl->list_no - mlst_ctl->top;
}
/********************************************************************************
 * Description :�л�����һ����ǩ�ļ�
 *
 * Arguments  :
 *			  	mlst_ctl:�˵��б���ƽṹ��((����common_ui.h�е�dir_control_t))
 *			 	 line_num:�л�������;0:�л�1��;1:�л�2�У������Դ�����
 * Returns     :
 *           	    ��
 * Notes       :
 *
 ********************************************************************************/
void _update_delete_item(dir_control_t *mlst_ctl)
{
    mlst_ctl->list_total--;
    if (mlst_ctl->list_total == 0)
    {
        return;
    }
    if (mlst_ctl->bottom > (mlst_ctl->list_total - 1))
    {
        mlst_ctl->bottom = mlst_ctl->list_total - 1;
        if (mlst_ctl->top > 0)
        {
            mlst_ctl->top--;
            mlst_ctl->list_no--;
        }
    }
    if (mlst_ctl->list_no > (mlst_ctl->list_total - 1))
    {
        mlst_ctl->list_no = mlst_ctl->list_total - 1;
        if (mlst_ctl->active > mlst_ctl->list_no)
        {
            mlst_ctl->active = mlst_ctl->list_no;
        }
    }
    else if ((mlst_ctl->list_no == mlst_ctl->list_total) && (mlst_ctl->list_no > 1))
    {
        mlst_ctl->list_no--;
    }
    else
    {
    }
    if (mlst_ctl->active > (mlst_ctl->list_total - 1))
    {
        mlst_ctl->active = mlst_ctl->list_total - 1;
    }
    mlst_ctl->draw_mode = LIST_DRAW_ALL;
}

/********************************************************************************
 * Description :�����б���ʾ��Ϣ������
 *
 * Arguments  :
 *			 	 dir_private:listbox ˽�нṹ��(����display.h�е�listbox_private_t)
 *			 	 dir_control:�˵��б���ƽṹ��(����common_ui.h�е�dir_control_t)
 *			 	 iinlet_type:���ܺ������������(����ebook.h�е�inlet_type_e)
 *                    (inlet_type)0-�Ķ�����;1-��ǩѡ��˵���1-��ǩɾ���˵�,3-ҳ��ѡ��˵�
 * Returns     :
 *                ��
 * Notes       :
 *
 ********************************************************************************/
void _update_list_info(dir_control_t *menlist, listbox_private_t *dir_private, inlet_type_e inlet_type)
{
    uint16 index;
    dir_private->title.data.str = BOOKMARK_filename;
    dir_private->title.length = LIST_ITEM_BUFFER_LEN;
    if (*(uint16*) (dir_private->title.data.str) == 0xfeff)
    {
        dir_private->title.language = UNICODEDATA;
    }
    else
    {
        dir_private->title.language = (int8) g_comval.language_id;
    }
    dir_private->item_valid = (uint8) (menlist->bottom - menlist->top + 1);
    dir_private->active = menlist->active;
    dir_private->old = menlist->old;
    dir_private->list_no = menlist->list_no;
    dir_private->list_total = menlist->list_total;

    for (index = 0; index < dir_private->item_valid; index++)
    {
        if (inlet_type == PAGE_SEL)
        {
            dir_private->items[index].data.str = _get_page_title(menlist->top, (uint8) index);
            dir_private->items[index].language = (int8) g_comval.language_id;
        }
        else
        {
            dir_private->items[index].data.str = _get_bmk_title((uint8) (menlist->top + index));
            dir_private->items[index].language = (int8) view_file.language;
        }
        dir_private->items[index].length = LIST_ITEM_BUFFER_LEN;
        dir_private->items[index].argv = 0;
    }
}
/********************************************************************************
 * Description :������ǩ�б��µ�ȷ��������
 *
 * Arguments  :
 *			 	 mlst_ctl:�˵��б���ƽṹ��(����common_ui.h�е�dir_control_t)
 *			 	inlet_type:���ܺ������������(����ebook.h�е�inlet_type_e)
 *                    (inlet_type)0-�Ķ�����;1-��ǩѡ��˵���1-��ǩɾ���˵�,3-ҳ��ѡ��˵�
 * Returns     :
 *               	 ������Ӧ�Ĵ�����
 * Notes       :
 *
 ********************************************************************************/
app_result_e _deal_confirm_item(dir_control_t *mlst_ctl, inlet_type_e inlet_type)
{
    app_result_e retval = RESULT_NULL;
    switch (inlet_type)
    {
        case BMK_SEL:
        curpagenum = _get_bmk_page(mlst_ctl->list_no);
        cur_BufSector = -1;
        retval = RESULT_EBK_PLAY;
        break;
        case BMK_DET:
        retval = _show_double_dialog(S_DELETEOPTION, _get_bmk_title((uint8) mlst_ctl->list_no));
        if (retval == RESULT_DIALOG_YES)                //��������ǣ���ɾ��������
        {
            _del_bookmark(mlst_ctl->list_no);
            _update_delete_item(mlst_ctl);
            if (mlst_ctl->list_total == 0)
            {
                _show_single_dialog(S_NOBMK, DIALOG_INFOR_WAIT);
                return RESULT_REDRAW;
            }
            retval = RESULT_NULL;
            need_draw = TRUE;
        }
        else
        {
            need_draw = TRUE;
            mlst_ctl->draw_mode = LIST_DRAW_ALL;
            retval = RESULT_NULL;
        }

        break;
        case PAGE_SEL:
        curpagenum = mlst_ctl->list_no * page_multiple_sel;
        if ((page_multiple_sel > 1) && (mlst_ctl->list_no > 0))
        {
            curpagenum--;
        }
        retval = RESULT_EBK_PLAY;
        break;
        default:
        break;
    }
    return retval;
}
/********************************************************************************
 * Description :��ȡҳ��������Ϣ
 *
 * Arguments  :
 *			 	 top:�˵��б����еĶ�����
 *			 	 index:�˵��б����е�������
 *
 *               	 �����б���ָ�룬����洢ҳ���ַ�����Ϣ
 * Notes       :
 *
 ********************************************************************************/
uint8 *_get_page_title(uint16 top, uint8 index)
{
    uint8 num_count;
    uint8 file_str[12];
    libc_memset(file_str, 0x0, 12);
    if ((page_multiple_sel > 1) && ((top + index) != 0))
    {
        num_count = libc_itoa(top + index, &file_str[2], 1);
        if ((2 + num_count) <= 11)
        {
            libc_strncpy(&file_str[2 + num_count], page_multiply, 1);
            libc_itoa(page_multiple_sel, &file_str[2 + num_count + 1], 0);
        }
    }
    else
    {
        libc_itoa(top + index + 1, &file_str[2], 1);
    }
    libc_strncpy(&file_str[0], page_str, 2);
    libc_memcpy(&file_list[index], file_str, 12);
    return (uint8*) &file_list[index];
}

