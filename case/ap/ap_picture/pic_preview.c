/*******************************************************************************
 *                              US212A
 *                            Module: PIC_PREVIEW
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-11-22 13:17:17       1.0              build this file
 *******************************************************************************/
/*!
 * \file     pic_preview.c
 * \brief    ͼƬ����ͼ���ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 * 		     ΪͼƬ�ṩԤ��Ч������ʱ�滮Ϊ�Ź�����ʾ��������ʾbanner
 * 		     ���ļ���ţ�ռ128*16�����ص�;�ײ���ʾ�ļ�����ռ��128*16����
 * 		     �ص㡣�м�ÿ��ͼƬ����Ϊ42*42,ÿ��ͼƬ����СΪ40*40��
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2011-11-22
 *******************************************************************************/
#include "picture.h"
#include "picture_preview.h"

/*! \brief ����Ź���ͼƬ��x,y��ʼ���� */
const uint8 pic_start_x[] = { 2, 44, 86 };
const uint8 pic_start_y[] = { 18, 60, 102 };

/*! \brief �������밴���¼��Ķ�Ӧ��ϵ*/
const key_map_t preview_key_map_list[] =
{
    /*! next�¼� */
    {{ KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PICTURE_PREVIEW_MOVENEXT },
    /*! prev�¼� */
    {{ KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PICTURE_PREVIEW_MOVEPREV },
    /*! �̰�mode option�¼� */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_PICTURE_OPTION },
    /*! ����mode�����¼� */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_PICTURE_RETURN },
    /*! �̰�play���¼�*/
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_DIRECTORY_ENTER_SON },
    /*! ������־ */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};
/******************************************************************************/
/*!
 * \par  Description:
 *	  ����Ԥ��ģʽ��λ��
 * \param[in]   pregion  ��ǰregion
 * \param[in]   pic_number ��ǰ�ļ����
 * \param[out]  none
 * \return      none
 * \ingroup     pic_preview
 * \note
 *******************************************************************************/
void init_prev_view_window(wregion_t *pregion, uint16 pic_number)
{
    uint8 row_counter;
    uint8 colume_counter;

    uint8 pic_no = (uint8) pic_number % PIC_ONE_SCREEN;
    row_counter = pic_no / PIC_PER_ROW;
    colume_counter = pic_no % PIC_PER_COLUME;

    pregion->x = pic_start_x[colume_counter];
    pregion->y = pic_start_y[row_counter];
    pregion->w = PIC_PREV_VIEW_WIDTH;
    pregion->h = PIC_PREV_VIEW_HEIGHT;
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��Ԥ��ͼƬ��FRAME��ʵ����ѡ�п��ѡ�п��Ч��
 * \param[in]   ��������region����ɫֵcolor
 * \param[out]  none
 * \return      none
 * \retval      none
 * \ingroup     pic_preview
 * \note
 *******************************************************************************/
static void show_frame(wregion_t region, uint32 color)
{
    wregion_t frame_region;

    //������frame
    ui_set_backgd_color(color);
    frame_region.x = region.x - PIC_FRAME_HEIGHT;
    frame_region.y = region.y - PIC_FRAME_HEIGHT;
    frame_region.w = PIC_FRAME_WIDTH;
    frame_region.h = PIC_FRAME_HEIGHT;
    ui_clear_screen(&frame_region);
    frame_region.y = region.y + region.h;
    ui_clear_screen(&frame_region);

    //������frame
    frame_region.x = region.x - PIC_FRAME_HEIGHT;
    frame_region.y = region.y - PIC_FRAME_HEIGHT;
    frame_region.w = PIC_FRAME_HEIGHT;
    frame_region.h = PIC_FRAME_WIDTH;
    ui_clear_screen(&frame_region);
    frame_region.x = region.x + region.w;
    ui_clear_screen(&frame_region);

    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  ��������ѡ�п������Ǽ�����ѡ�п�
 * \param[in]   old_active, active,
 * \param[out]
 * \return
 * \retval
 * \ingroup     pic_preview
 * \note
 *******************************************************************************/
void show_active_frame(uint16 old_active, uint16 active)
{
    DC dc_status;

    ui_get_DC_status(&dc_status);
    if (old_active != active)
    {
        init_prev_view_window(&region_in, old_active);
        show_frame(region_in, Color_BLACK_def);
    }
    //�Ǽ�����region�������󣬻�������ѡ�п�
    init_prev_view_window(&region_in, active);
    show_frame(region_in, Color_WHITE_def);
    ui_set_pen_color(dc_status.pen_color);
    ui_set_backgd_color(dc_status.backgd_color);
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ʾ�������ļ��������ΪĿ¼����ʾĿ¼��
 * \param[in]   none
 * \param[out]  none
 * \return      ui_result_e
 * \retval      not care
 * \ingroup     pic_preview
 * \note
 *******************************************************************************/
void pic_show_filename(void)
{
    ui_result_e ui_result;
    style_infor_t picture_style;
    textbox_private_t picture_textbox_param;

    int8 language_id = UNICODEDATA;
    if ((g_picture_filename[0] != 0xff) && (g_picture_filename[1] != 0xfe))
    {
        language_id = (int8) g_comval.language_id;
    }
    //��ʾ�ļ���
    picture_style.type = UI_AP;
    picture_style.style_id = STY_PIC_FILENAME_TEXTBOX;
    picture_textbox_param.lang_id = language_id;

    picture_textbox_param.str_id = -1;
    picture_textbox_param.str_value = g_picture_filename;
    ui_result = (ui_result_e) ui_show_textbox(&picture_style, &picture_textbox_param, TEXTBOX_DRAW_NORMAL);

    //�Ƿ���Ҫ������ʾ�ļ���
    if (ui_result == UI_NEED_SCROLL)
    {
        restart_app_timer(timer_pic_play);
    }
    else
    {
        stop_app_timer(timer_pic_play);
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  ѡ������ļ�������������ڼ��������ʾ�������ļ���
 * \param[in]   item_no �ļ����
 * \param[in]   active  �������ļ����
 * \param[out]  none
 * \return      none
 * \ingroup     pic_preview
 * \note
 *******************************************************************************/
void select_file(uint16 item_no, uint16 active)
{
    //�ļ�ѡ����ѡ����Ӧ��Ԥ��ͼƬ
    if (item_no < g_dir_browser.dir_total)
    {
        bs_dir_set_dirpos(EXT_NAME_ALL_DIR, item_no + 1);
    }
    else
    {
        fsel_get_byno(g_picture_var.path.file_path.dirlocation.filename, item_no + 1);
    }

    if (item_no == active)
    {
        vfs_get_name(picture_mount, g_picture_filename, LIST_ITEM_BUFFER_LEN / 2);
        if ((g_picture_filename[0] != 0xff) && (g_picture_filename[0] != 0xfe))
        {
            if (item_no >= g_dir_browser.dir_total)
            {
                com_dot_to_shortname(g_picture_filename);
            }
        }
        pic_show_filename();
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ʾ�ļ������ͼ��������
 * \param[in]   active  �������ļ����
 * \param[out]  none
 * \return      none
 * \ingroup     pic_preview
 * \note
 *******************************************************************************/
void show_active_num(uint16 active, uint8 mode)
{
    style_infor_t picture_style;
    numbox_private_t picture_numbox_param;

    picture_style.type = UI_AP;
    picture_style.style_id = STY_PIC_PREVIEW_NUMBERBOX;
    picture_numbox_param.total = dir_control.list_total;
    picture_numbox_param.value = active + 1;
    if (mode == TRUE)
    {
        mode = NUMBOX_DRAW_ALL;
    }
    else
    {
        mode = NUMBOX_DRAW_NUMBER;
    }
    //����numbox��ʾ ����͸����ʾ
    ui_show_numbox(&picture_style, &picture_numbox_param, mode);
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  �ļ���������ʾ��ʱ��������
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \ingroup     pic_preview
 * \note
 * \li  ��־λ��λ�󣬻����һ�ι�����ʾ
 *******************************************************************************/
void scroll_play_flag(void)
{
    ui_show_textbox(NULL, NULL, TEXTBOX_DRAW_SCROLL);
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  GUI��ϵͳ��Ϣ������
 * \param[in]   mode ģʽ 0 ����ͼ����ʱ��ȡ��Ϣ 1����ͼ������ɻ�ȡ��Ϣ 
 * \param[in]   p_dir_control �ļ�����ṹ��ָ��
 * \param[out]  none
 * \return      none
 * \ingroup     pic_preview
 * \note
 *******************************************************************************/
app_result_e pic_preview_deal_msg(uint8 mode, dir_control_t *p_dir_control)
{
    bool bret;
    input_gui_msg_t ui_msg;
    private_msg_t pri_msg;
    msg_apps_type_e gui_event;
    app_result_e ui_result = RESULT_NULL;

    bret = get_gui_msg(&ui_msg);
    if (bret == TRUE)
    {
        if (com_key_mapping(&ui_msg, &gui_event, preview_key_map_list) == TRUE)
        {
            ui_result = pic_preview_proc_key_event(gui_event, p_dir_control);
            if (mode == 0)
            {
                if (ui_result == RESULT_PIC_NEXTITEM)
                {
                    ui_result = RESULT_NULL;
                }
            }
        }
    }
    else
    {
        if (mode == 1)
        {
            bret = get_app_msg(&pri_msg);
            if (bret == TRUE)
            {
                pic_decoder_free();
                ui_result = pic_msg_callback(&pri_msg);
            }
        }
    }
    return ui_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ʾһ���Ź���ͼƬ����ͼ
 * \param[in]   p_dir_control �ļ�����ṹ��ָ��
 * \param[out]  none
 * \return      app_result_e �¼�������ֵ
 * \ingroup     pic_preview
 * \note
 * \li   ��������ͼʱ�Ƿ���Ҫ��Ӧ����?
 *******************************************************************************/
app_result_e prev_view_one_screen(dir_control_t *p_dir_control)
{
    uint16 i;
    region_t region;
    uint16 top = p_dir_control->top;
    uint16 bottom = p_dir_control->bottom;
    uint16 active = p_dir_control->active;
    uint16 old_active = p_dir_control->old;

    app_result_e ui_result = RESULT_NULL;

    show_active_num(active, FALSE);

    if (active == old_active)
    {
        region.x = 0;
        region.y = 16;
        region.width = 128;
        region.height = 128;
        //������ʾͼƬ
        ui_clear_screen(&region);
        for (i = top; i <= bottom; i++)
        {
            //���ö�Ӧ�ļ�Ԥ������
            init_prev_view_window(&region_in, i);
            select_file(i, active);
            if (i < g_dir_browser.dir_total)
            {
                ui_show_pic(PICTURE_DIR, region_in.x, region_in.y);
            }
            else
            {
                if (DECODE_NO_ERR != pic_decode())
                {
                    ui_show_pic(PICTURE_NO_THUMBNAIL, region_in.x, region_in.y);
                }
                while (pic_check_decode() == FALSE)
                {
                    ui_result = pic_preview_deal_msg(0, p_dir_control);
                    if (ui_result == RESULT_NULL)
                    {
                        //sys_os_time_dly(1);
                    }
                    else
                    {
                        pic_decoder_free();
                        return ui_result;
                    }
                }

                //�����ǰ�߳�δ�ͷ�,�ͷŸ��߳�
                pic_decoder_free();
            }
            //sys_os_time_dly(1);
        }
    }
    else
    {
        select_file(active, active);
    }
    show_active_frame(p_dir_control->old, p_dir_control->active);
    return ui_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ͼƬ����ͼ����ģ��
 * \param[in]   void
 * \param[out]  none
 * \return      app_result_e ����ֵ
 * \ingroup     pic_preview
 * \note
 *******************************************************************************/
app_result_e pic_preview(void)
{
    bool need_draw_all = TRUE;
    bool need_draw = TRUE;
    bool is_scene_exit = FALSE;
    app_result_e ui_result = RESULT_NULL;
    file_path_info_t *path_info = &(g_picture_var.path);

    dir_control.dir_disk = path_info->file_path.dirlocation.disk;
    //��ʼ���ó�Ա,��ֹ·���������
    dir_control.one_page_count = 6;
    dir_control.dir_com = NULL;
    dir_control.enter_dir = TRUE;

    g_dir_browser.name_buf = g_picture_filename;
    g_dir_browser.name_len = LIST_ITEM_BUFFER_LEN;

    //�����ʼ��
    if (FALSE == pic_decode_init(1))
    {
        ui_result = RESULT_PIC_RETURN;
        is_scene_exit = TRUE;
    }
    else
    {
        //���ò���ȡ��Ŀ¼��Ŀ¼�ṹ��
        if (fsel_browser_enter_dir(ROOT_DIR, 0, &g_dir_browser) == FALSE)
        {
            ui_result = RESULT_PIC_RETURN;
            is_scene_exit = TRUE;
        }
    }
    while (!is_scene_exit)
    {
        if (need_draw_all == TRUE)
        {
            need_draw_all = FALSE;
            if (dir_control.enter_dir == TRUE)
            {
                dir_control.list_total = g_dir_browser.dir_total + g_dir_browser.file_total;
                if (dir_control.list_total == 0)
                {
                    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_PIC_FILE);
                    if ((g_dir_browser.layer_no != 0)
                            && (fsel_browser_enter_dir(PARENT_DIR, 0, &g_dir_browser) == TRUE))
                    {
                        //���·�����һ��Ŀ¼
                        need_draw_all = TRUE;
                        dir_control.enter_dir = TRUE;

                    }
                    else
                    {
                        is_scene_exit = TRUE;
                        ui_result = RESULT_PIC_RETURN;
                    }
                    continue;
                }
                //��ȡ��ǰ����ļ����·����¼
                read_history_item(g_dir_browser.layer_no, &dir_control);
                dir_control.enter_dir = FALSE;

                if (path_info->file_path.plist_location.file_total < dir_control.list_no)
                {
                    //��¼��Ч�����¸���
                    dir_control.list_no = path_info->file_path.plist_location.file_num;
                }
                dir_control.old = dir_control.active = (dir_control.list_no - 1);
            }
            show_active_num(dir_control.active, TRUE);
            init_list_para();
            need_draw = TRUE;
        }

        if (need_draw == TRUE)
        {
            need_draw = FALSE;
            //���ػ�Ԥ������
            ui_result = prev_view_one_screen(&dir_control);
        }
        else
        {
            ui_result = pic_preview_deal_msg(1, &dir_control);
        }

        switch (ui_result)
        {
            case RESULT_NULL:
            case RESULT_IGNORE:
            break;

            case RESULT_PIC_NEXTITEM:
            need_draw = TRUE;
            break;

            case RESULT_REDRAW:
            case RESULT_PIC_NEXTSCREEN:
            need_draw_all = TRUE;
            break;

            default:
            is_scene_exit = TRUE;
            break;
        }
        sys_os_time_dly(1);
    }
    kill_app_timer(timer_pic_play);

    if (ui_result == RESULT_PIC_RETURN)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            ui_result = RESULT_MAIN_APP;
        }
        else
        {
            ui_result = RESULT_PIC_MENU;
        }
    }
    //��д���ͷſռ�
    write_history_item(0, &dir_control, TRUE);
    return ui_result;
}

