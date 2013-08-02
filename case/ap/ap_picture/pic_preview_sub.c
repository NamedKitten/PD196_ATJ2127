/*******************************************************************************
 *                              US212A
 *                            Module: PIC_PREVIEW_SUB
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-12-9 11:16:08           1.0              build this file
 *******************************************************************************/
/*!
 * \file     pic_preview_sub.c
 * \brief    ͼƬ����ͼ������Ӧ����
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2011-12-9
 *******************************************************************************/
#include "picture.h"
#include "picture_preview.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    ��������ͼdir_control������س�Աֵ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_preview
 * \note
 *******************************************************************************/
void init_list_para(void)
{
    uint16 cur_group;
    dir_control.old = dir_control.active;
    cur_group = (dir_control.active / PIC_ONE_SCREEN);
    dir_control.top = cur_group * PIC_ONE_SCREEN;
    dir_control.bottom = dir_control.top + PIC_ONE_SCREEN - 1;
    if (dir_control.bottom > (dir_control.list_total - 1))
    {
        dir_control.bottom = (dir_control.list_total - 1);
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ���ݰ�����������µļ�����
 * \param[in]    move_direction �������� MOVE_UP/MOVE_DOWN/MOVE_PREV/MOVE_NEXT
 * \param[out]   none
 * \return       app_result_e �¼�����ֵ
 * \retval       RESULT_PIC_NEXTSCREEN ����������Ļ��ʾ
 * \retval       RESULT_PIC_NEXTITEM   ���¼�����ͷǼ�������ʾ
 * \retval       RESULT_NULL           �����и�����ʾ
 * \ingroup      pic_preview
 * \note
 *******************************************************************************/
static app_result_e move_sel(uint8 move_direction)
{
    uint16 cur_group, old_group;
    uint16 adjust_total = dir_control.list_total;
    uint8 adjust_value = (uint8) dir_control.list_total % PIC_PER_COLUME;
    uint16 list_active = dir_control.active;
    uint16 list_total = dir_control.list_total;

    app_result_e ui_result = RESULT_PIC_NEXTITEM;

    dir_control.old = dir_control.active;
    old_group = (dir_control.active / PIC_ONE_SCREEN);
    if (adjust_value != 0)
    {
        //�����ǰ�ļ���������3�ı�������Ҫ��list_total��������
        adjust_total += PIC_PER_COLUME - adjust_value;
    }

    if ((list_total == 0) || (adjust_total == 0))
    {
        return ui_result;
    }

    switch (move_direction)
    {
        case MOVE_UP:
        list_active = (list_active + adjust_total - PIC_PER_COLUME) % adjust_total;
        if (list_active > (list_total - 1))
        {
            list_active = list_active - PIC_PER_COLUME;
        }
        break;

        case MOVE_DOWN:
        list_active = (list_active + adjust_total + PIC_PER_COLUME) % adjust_total;
        if (list_active > (list_total - 1))
        {
            list_active = list_active % PIC_PER_COLUME;
        }
        break;

        case MOVE_PREV:
        list_active = (list_active + list_total - 1) % list_total;
        break;

        case MOVE_NEXT:
        list_active = (list_active + list_total + 1) % list_total;
        break;

        default:
        break;
    }
    cur_group = list_active / PIC_ONE_SCREEN;

    if (cur_group != old_group)
    {
        //��Ҫ������ʾ
        ui_result = RESULT_PIC_NEXTSCREEN;
    }
    else
    {
        if (list_active != dir_control.old)
        {
            //��Ҫ���¸��¼�����ͷǼ�����
            ui_result = RESULT_PIC_NEXTITEM;
        }
        else
        {
            //��������ѡ��ļ��������Ǹ���������κθ���
            ui_result = RESULT_NULL;
        }
    }
    dir_control.active = list_active;
    dir_control.list_no = list_active + 1;
    return ui_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *   ���浱ǰ·������
 * \param[in]    p_dir_control �ļ�����ṹ��ָ��
 * \param[out]   none
 * \return       none
 * \ingroup      pic_preview
 * \note
 * \li   ·��������Ҫ����list_no,list_top�����������
 *******************************************************************************/
static void _pic_save_history(dir_control_t *p_dir_control)
{
    if (p_dir_control->list_no < p_dir_control->one_page_count)
    {
        p_dir_control->top = 1;
    }
    else
    {
        p_dir_control->top = p_dir_control->list_no - p_dir_control->one_page_count + 1;
    }
    write_history_item(g_dir_browser.layer_no, p_dir_control, FALSE);
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ����ͼģ��GUI��Ϣ�����������ݲ�ͬ�¼�ֵ���д���
 * \param[in]    cur_event ���ݰ�����Ϣת���õ����¼�ֵ
 * \param[out]   none
 * \return       app_result_e �¼�������ֵ
 * \ingroup      pic_preview
 * \note
 *******************************************************************************/
app_result_e pic_preview_proc_key_event(msg_apps_type_e cur_event, dir_control_t *p_dir_control)
{
    //dir_control_t dir_control;
    app_result_e ui_result = RESULT_NULL;
    switch (cur_event)
    {
        case EVENT_PICTURE_PREVIEW_MOVEDOWN:
        ui_result = move_sel(MOVE_DOWN);
        break;

        case EVENT_PICTURE_PREVIEW_MOVEUP:
        ui_result = move_sel(MOVE_UP);
        break;

        case EVENT_PICTURE_PREVIEW_MOVEPREV:
        ui_result = move_sel(MOVE_PREV);
        break;

        case EVENT_PICTURE_PREVIEW_MOVENEXT:
        ui_result = move_sel(MOVE_NEXT);
        break;

        case EVENT_PICTURE_RETURN:
        _pic_save_history(p_dir_control);
        if (g_dir_browser.layer_no == 0)
        {
            ui_result = RESULT_PIC_RETURN;
        }
        else
        {
            //������һ��Ŀ¼
            if (fsel_browser_enter_dir(PARENT_DIR, 0, &g_dir_browser) == TRUE)
            {
                p_dir_control->enter_dir = TRUE;
                ui_result = RESULT_REDRAW;
            }
            else
            {
                ui_result = RESULT_PIC_RETURN;
            }
        }
        break;

        case EVENT_PICTURE_OPTION:
        //���浱ǰ����ļ������¼
        _pic_save_history(p_dir_control);
        ui_result = RESULT_PIC_OPTIONMENU;
        break;

        case EVENT_DIRECTORY_ENTER_SON:
        //���浱ǰ����ļ������¼
        _pic_save_history(p_dir_control);
        pic_decoder_free();
        if (dir_control.active < g_dir_browser.dir_total)
        {
            if (fsel_browser_enter_dir(SON_DIR, dir_control.active + 1, &g_dir_browser) == TRUE)
            {
                p_dir_control->enter_dir = TRUE;
                ui_result = RESULT_REDRAW;
            }
            else
            {
                ui_result = RESULT_PIC_RETURN;
            }
        }
        else
        {
            //����ѡ���ļ�
            fsel_browser_select(dir_control.list_no);
            //��ȡ��ǰ�ļ�·��
            fsel_browser_get_location(&g_picture_var.path.file_path, g_picture_var.path.file_source);
            ui_result = RESULT_PICTURE_PLAY;
        }
        break;

        default:
        pic_decoder_free();
        ui_result = com_message_box(cur_event);
        if ((ui_result == RESULT_NULL) || (ui_result == RESULT_CONFIRM))
        {
            ui_result = RESULT_REDRAW;
        }
        break;
    }
    return ui_result;
}

