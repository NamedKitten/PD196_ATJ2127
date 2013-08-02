/*******************************************************************************
 *                              US212A
 *                            Module: PIC_PLAY_SUB
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-12-9 15:04:19           1.0              build this file
 *******************************************************************************/
/*!
 * \file     pic_play_sub.c
 * \brief    ͼƬ������ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ��ģ�����ͼƬ���빦�ܣ�ʵ�ֽ����ʼ������ͼƬ�м���Ľ�����
 * \par      EXTERNALIZED FUNCTIONS:
 *               ��Ҫ����ͼƬ�м��ʵ�ֽ��빦��
 * \version 1.0
 * \date    2011-12-9
 *******************************************************************************/
#include "app_music.h"
#include "app_music_albumlist.h"

static ColorLcmInfor userinfor _BANK_DATA_ATTR_;
//static mmm_id_status_t pic_decoder_status _BANK_DATA_ATTR_;
//dir layer save
//static pdir_layer_t sys_layer_buf _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 ȷ��ͼƬ��תʱ������
 * \param[in]    region1 Ӧ�ô��ݸ��м���Ĵ�����
 * \param[in]    region2 �м�����ظ�ap��ͼƬʵ�����꣬��Ҫ�Ը�������б任
 * \param[out]   none
 * \return       none
 * \ingroup      pic_play
 * \note
 * \li   ���ڵײ�δ��ͼƬ��תģʽ��������仯����Ҫap�����һ����
 * \li   ����bmpͼƬ,����ԭ�������½ǣ��������ֻ��xy����λ��
 * \li   ����jpg,bmpͼƬ������ԭ�������Ͻǣ���Ҫ�ڽ�����xy������ٵ���y����
 *******************************************************************************/
static void _format_region(wregion_t *region1, wregion_t *region2)
{
    uint16 temp_x = region2->x;
    uint16 region_width = region1->w;
    uint16 region_height = region1->h;

    if ((userinfor.returnflag & 0x01) != 0)
    {
        //��ת����任
        region2->x = region2->y;

        region2->y = DISPLAY_LENGTH - temp_x - region1->w;

        //���д���
        region2->x += (region_height - region2->w) / 2;
        region2->y += (region_width - region2->h) / 2;
    }
    else
    {
        //���д���
        region2->x += (region_width - region2->w) / 2;
        region2->y += (region_height - region2->h) / 2;
    }

    return;

}
/******************************************************************************/
/*!
 * \par  Description:
 *    ����ͼƬˢ��ģʽ���趨ͼƬ��������
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_play
 * \note
 * \li   ע���������������ˢ��ģʽ�����һ��Ҫ������������ˢ��ģʽ
 *******************************************************************************/
static void _pic_set_window(wregion_t *region1, wregion_t *region2)
{
    uint8 draw_mode;

    //���㴰����
    _format_region(region1, region2);

    draw_mode = DRAW_MODE_H_DEF;

    lcd_set_draw_mode(draw_mode);

    //����ͼƬ��ʾ����
    lcd_set_window(region2);

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_albumart(void)
 * \����ר������album art
 * \param[in]    albumart_mode 1 ��ʾalbumart 0 ��ʾר���б�ͼƬ
 * \param[in]    full_mode     1 С������ʾģʽ 0 ��������ʾģʽ
 * \param[in]    file_offset   ͼƬ���ļ���ƫ�ƣ����ڷ���ǶͼƬֵΪ0
 * \param[in]    region1     ���봰����
 * \param[out]   region2     ���������
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_paint.c
 * \note
 */
/*******************************************************************************/
void music_paint_albumart(uint8 albumart_mode, uint8 full_mode, uint32 file_offset, wregion_t *region1,
        wregion_t *region2)
{
    int result;
    style_infor_t style;
    //�������
    void* mmm_pic_handle = NULL;
    style.type = UI_AP;

    //��ʼ���������
    userinfor.mountindex = (uint32) g_music_mount_id;
    //�ļ�ƫ��
    userinfor.data_offset = file_offset;

    userinfor.playflag = 0;

    if (full_mode == FALSE)
    {
        //bypassģʽ
        userinfor.playflag |= 0x04;
    }

    userinfor.pregion1 = region1;
    userinfor.pregion2 = region2;

    //��enhance bankˢ��
    sys_bank_flush(AP_BANK_FRONT_ENHANCED_1);
    sys_bank_flush(AP_BANK_FRONT_ENHANCED_2);

    //���font cache
    ui_flush_font_buffer();

    //�����м������
    sys_load_mmm("mmm_id.al", FALSE);//������

    //�򿪽����
    mmm_id_cmd(&mmm_pic_handle, MMM_ID_OPEN, (uint32) NULL);

    if (mmm_pic_handle != NULL)
    {
        //���ý������
        result = mmm_id_cmd(mmm_pic_handle, MMM_ID_SET_ALBUM_INFO, (unsigned int) &userinfor);
        if (result == 0)
        {
            if (albumart_mode == TRUE)
            {
                if ((region1->w != region2->w) || (region1->h != region2->h))
                {
                    //��ʾ����
                    style.style_id = ALBUM_ART_BGPIC;
                    ui_show_picbox(&style, NULL);
                }
            }

            _pic_set_window(region1, region2);

            //��ʼ����
            result = mmm_id_cmd(mmm_pic_handle, MMM_ID_DECODE, (unsigned int) NULL);
            while (1)
            {
                //��ȡ״̬
                if (userinfor.dec_status == MMM_ID_FREE)
                {
                    break;
                }
                else
                {
                    sys_os_time_dly(2);
                }
            }
        }
        else
        {
            if (albumart_mode == TRUE)
            {
                //Ϊ��֧�ֵ�ר��ͼƬ���ͣ���ʾĬ��ͼƬ
                style.style_id = ALBUM_ART_PICBOX;
                ui_show_picbox(&style, NULL);
            }
            else
            {
                style.style_id = ALBUM_NO;
                ui_show_pic(ALBUM_NO, region1->x, region1->y);
            }
        }

    }
    //�رս����
    mmm_id_cmd(mmm_pic_handle, MMM_ID_CLOSE, (uint32) NULL);

    sys_free_mmm(FALSE);
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʾһ��ר���б�ͼƬ
 * \param[in]    active  ��ǰר���б������
 * \param[out]   none
 * \return       none
 * \ingroup      pic_play
 * \note
 * \li   ע���������������ˢ��ģʽ�����һ��Ҫ������������ˢ��ģʽ
 *******************************************************************************/
void pic_paint(uint8 active)
{
    //path save
    //uint8 cur_offset_save[8];
    uint8 full_mode = TRUE;

    //save current path
    //vfs_file_dir_offset(g_music_mount_id, &sys_layer_buf, cur_offset_save, 0);

    vfs_file_dir_offset(g_music_mount_id, NULL, &(g_album_list[active].cluster_no), 1);

    if ((g_app_info_state.type == ENGINE_MUSIC) && (g_app_info_state.state == ENGINE_STATE_PLAYING))
    {
        full_mode = FALSE;
    }

    music_paint_albumart(0, full_mode, 0, &region_in, &region_out);

    //restore path
    //vfs_file_dir_offset(g_music_mount_id, &sys_layer_buf, cur_offset_save, 1);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʼ��g_album_list����
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_play
 * \note
 * \li   ���ڸñ���Ϊbank_data,�����Ҫע�����ֵ�Ļָ�
 *******************************************************************************/
void pic_init_display_buffer(void)
{
    uint8 i;

    uint8 *temp_buffer = g_temp_buffer_p;

    for (i = 0; i < PIC_ONE_SCREEN; i++)
    {
        g_album_list[i].name_buf = &temp_buffer[LIST_ITEM_BUFFER_LEN * i];
        g_album_list[i].name_len = LIST_ITEM_BUFFER_LEN;
    }

    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ͼƬ�����ʼ��
 * \param[in]  p_dir_control �ļ�����ṹ��ָ��
 * \param[in]  path_info     �ļ�·����Ϣ
 * \param[out] none
 * \return     app_result_e
 * \ingroup    pic_play
 * \note
 *******************************************************************************/
app_result_e pic_decode_init(dir_control_t *p_dir_control, file_path_info_t *path_info)
{
    app_result_e ret_val = RESULT_NULL;

    ui_clear_screen(NULL);

    //����ͼģʽ���������Ȳ�����
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, this_headbar_icon_id);

    //��������80ms�������ŵĶ�ʱ��
    scroll_frush_id = set_app_timer(APP_TIMER_ATTRB_UI, 80, scroll_play_flag);
    stop_app_timer(scroll_frush_id);

    //��ʼ�б�������ռ�
    g_dir_browser.name_len = 0;

    //��·���ĵ�һ��Ŀ¼���
    if (p_dir_control->dir_com->browse_mode == 0)
    {
        //���벢��ȡ��Ŀ¼��Ŀ¼�ṹ��
        if (fsel_browser_enter_dir(ROOT_DIR, 0, &g_dir_browser) == FALSE)
        {
            return RESULT_DIR_ERROR_ENTER_DIR;
        }
    }
    else//ֱ������·��ָ��Ŀ¼���
    {
        if (fsel_browser_set_location(&(path_info->file_path), path_info->file_source) == TRUE)
        {
            //��ȡ��ǰ·����Ŀ¼�ṹ��
            if (fsel_browser_enter_dir(CUR_DIR, 0, &g_dir_browser) == FALSE)
            {
                return RESULT_DIR_ERROR_ENTER_DIR;
            }
        }
        else
        {
            if (p_dir_control->dir_com->root_layer == 0)
            {
                //���벢��ȡ��Ŀ¼��Ŀ¼�ṹ��
                if (fsel_browser_enter_dir(ROOT_DIR, 0, &g_dir_browser) == FALSE)
                {
                    return RESULT_DIR_ERROR_ENTER_DIR;
                }

                //��ȡ ROOT �ļ�·����ȷ���´ν���ʱ���óɹ�
                fsel_browser_get_location(&(path_info->file_path), path_info->file_source);

                //�Ҳ���·���������Ӧ���ļ��������
                com_clear_app_history(PATH_HISTORY_DIR, g_this_app_info->app_id, p_dir_control->dir_disk);
            }
            else
            {
                //���㲻�Ǹ�Ŀ¼�����󷵻أ����ϲ��Լ�����
                return RESULT_DIR_ERROR_SETLOC;
            }
        }
    }
    return ret_val;
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
#if 0
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
#endif
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
 * \param[in]    path_info ·����Ϣ
 * \param[in]    p_dir_control �ļ�����ṹ��ָ��
 * \param[in]    cur_event ���ݰ�����Ϣת���õ����¼�ֵ
 * \param[out]   none
 * \return       app_result_e �¼�������ֵ
 * \ingroup      pic_preview
 * \note
 *******************************************************************************/
app_result_e pic_preview_proc_key_event(file_path_info_t *path_info, dir_control_t *p_dir_control,
        msg_apps_type_e cur_event)
{
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
        ui_result = RESULT_PIC_RETURN;
        break;

        case EVENT_PICTURE_OPTION:
        if (p_dir_control->dir_com->list_option_func != NULL)
        {
            ui_result = p_dir_control->dir_com->list_option_func(path_info, p_dir_control->list_no);
            if ((ui_result != RESULT_NULL) && (ui_result != RESULT_REDRAW))
            {
                _pic_save_history(p_dir_control);
            }
        }
        break;

        case EVENT_DIRECTORY_ENTER_SON:
        //���浱ǰ����ļ������¼
        _pic_save_history(p_dir_control);
        if (fsel_browser_enter_dir(SON_DIR, p_dir_control->active + 1, &g_dir_browser) == TRUE)
        {
            fsel_browser_select(1);
            fsel_browser_get_location(&(path_info->file_path), path_info->file_source);
            ui_result = RESULT_ENTER_TITLE_LIST;
        }
        else
        {
            ui_result = RESULT_PIC_RETURN;
        }
        break;

        default:
        //pic_decoder_free();
        ui_result = com_message_box(cur_event);
        if ((ui_result == RESULT_NULL) || (ui_result == RESULT_CONFIRM))
        {
            ui_result = RESULT_REDRAW;
        }
        break;
    }
    return ui_result;
}

