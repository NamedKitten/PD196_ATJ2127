/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang    2011-09-19          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    ѡ���̨�б��е���һ��
 * \param[in]    p_ml_control��ָ���б���ƽṹ��
 * \param[out]   p_ml_control�����ظ����˵��б����
 * \return       none
 * \note
 *******************************************************************************/
void radiolist_select_next(radiolist_control_t *p_ml_control)
{
    //��ҳ���л����к���δ��ҳ�����һ���ൽ�����ڶ�������¼�����
    if ((p_ml_control->list_no + 1) < p_ml_control->bottom)
    {
        just_change_active_next: p_ml_control->list_no++;
        p_ml_control->old = p_ml_control->active;
        p_ml_control->active++;
        p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
    }
    else
    {
        //���б����һ������б�ͷ��ʾ
        if (p_ml_control->list_no == (p_ml_control->total - 1))
        {
            p_ml_control->list_no = 0;
            //����һҳ������£�ֻ����¼�����
            if (p_ml_control->total <= p_ml_control->one_page_count)
            {
                //����������1�����Ҫ�л�
                if (p_ml_control->total > 1)
                {
                    p_ml_control->old = p_ml_control->active;
                    p_ml_control->active = 0;
                    p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
                }
            }
            //��β�л���ͷ�����������б�
            else
            {
                p_ml_control->top = 0;
                p_ml_control->bottom = p_ml_control->one_page_count - 1;
                p_ml_control->old = p_ml_control->active = 0;
                p_ml_control->update_mode = LIST_UPDATE_HEAD;
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
        }
        else
        {
            //���������еĵ����ڶ���е����һ�ֻ����¼�����
            if (p_ml_control->list_no == (p_ml_control->total - 2))
            {
                goto just_change_active_next;
            }
            //�ڵ�ǰҳ�еĵ����ڶ�����Һ������㹻�б��������ҳ�����������б�
            else
            {
                p_ml_control->list_no++;
                p_ml_control->top++;
                p_ml_control->bottom++;
                p_ml_control->old = p_ml_control->active;
                p_ml_control->update_mode = LIST_UPDATE_NEXT;
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ѡ���̨�б��е���һ��
 * \param[in]    p_ml_control��ָ���б���ƽṹ��
 * \param[out]   p_ml_control�����ظ����˵��б����
 * \return       none
 * \note
 *******************************************************************************/
void radiolist_select_prev(radiolist_control_t *p_ml_control)
{
    //��ҳ���л����к���δ��ҳ�������ൽ�ڶ�������¼�����
    if (p_ml_control->list_no > (p_ml_control->top + 1))
    {
        just_change_active_prev: p_ml_control->list_no--;
        p_ml_control->old = p_ml_control->active;
        p_ml_control->active--;
        p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
    }
    else
    {
        //���б��һ������б�β��ʾ
        if (p_ml_control->list_no == 0)
        {
            p_ml_control->list_no = p_ml_control->total - 1;
            //����һҳ������£�ֻ����¼�����
            if (p_ml_control->total <= p_ml_control->one_page_count)
            {
                //����������1�����Ҫ�л�
                if (p_ml_control->total > 1)
                {
                    p_ml_control->old = p_ml_control->active;
                    p_ml_control->active = p_ml_control->total - 1;
                    p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
                }
            }
            //��ͷ�е�β�����������б�
            else
            {
                p_ml_control->top = p_ml_control->total - p_ml_control->one_page_count;
                p_ml_control->bottom = p_ml_control->list_no;
                p_ml_control->old = p_ml_control->active = p_ml_control->one_page_count - 1;
                p_ml_control->update_mode = LIST_UPDATE_TAIL;
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
        }
        else
        {
            //���������еĵڶ���е���һ�ֻ����¼�����
            if (p_ml_control->list_no == 1)
            {
                goto just_change_active_prev;
            }
            //�ڵ�ǰҳ�еĵڶ������ǰ�����㹻�б��������ҳ�����������б�
            else
            {
                p_ml_control->list_no--;
                p_ml_control->top--;
                p_ml_control->bottom--;
                p_ml_control->old = p_ml_control->active;
                p_ml_control->update_mode = LIST_UPDATE_PREV;
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ѡ���̨�б��е�һ����в���
 * \param[in]    p_ml_control��ָ���б���ƽṹ��
 * \param[out]
 * \return       none
 * \note
 *******************************************************************************/
void select_current_play(radiolist_control_t *p_ml_control)
{
    uint16 *ptr = pfreq_list;
    uint16 cur_freq;
    bool is_saved = TRUE;

    cur_freq = *(ptr + p_ml_control->list_no);
    if (cur_freq == 0)
    {
        //��ѡ������Ƶ�㱣��
        is_saved = FALSE;
    }

    //���µ�ǰ������Ϣ
    if (is_saved == TRUE)
    {
        g_radio_config.FMStatus.freq = cur_freq; //����Ƶ��ֵ
        g_radio_config.FMStatus.station = p_ml_control->list_no + 1; //��ǰ��̨��
    }
    else
    {
        g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        g_radio_config.FMStatus.station = 0;
    }
}

