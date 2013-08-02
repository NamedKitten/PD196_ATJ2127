/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mainmenu_paint.c
 * \brief    ��ʾ������UI�Լ���ص�һЩ����
 * \author   zhangxs
 * \version  1.0
 * \date  2011/10/08
 *******************************************************************************/
#include  "main_menu.h"

/******************************************************************************/
/*!
 * \par  Description:
 *	  ������ת��Ϊ�ַ���
 * \param[in]    none
 * \param[out]   uint8 *ptr�����ڵ��ַ������
 * \return       none
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void _format_date(uint8 *ptr)
{
    date_t temp_sys_date;

    sys_get_date(&temp_sys_date);

    if (temp_sys_date.year >= 2099)
    {
        temp_sys_date.year = 2000;
        sys_set_date(&temp_sys_date);
    }

    switch (g_setting_comval.g_comval.date_format)
    {
        //DD_MM_YYYY
        case 0:
        libc_itoa(temp_sys_date.day, ptr, 2);
        *(ptr + 2) = '-';
        libc_itoa(temp_sys_date.month, ptr + 3, 2);
        *(ptr + 5) = '-';
        libc_itoa(temp_sys_date.year, ptr + 6, 4);
        break;

        //MM_DD_YYYY
        case 1:
        libc_itoa(temp_sys_date.month, ptr, 2);
        *(ptr + 2) = '-';
        libc_itoa(temp_sys_date.day, ptr + 3, 2);
        *(ptr + 5) = '-';
        libc_itoa(temp_sys_date.year, ptr + 6, 4);
        break;

        //YYYY_MM_DD
        default:
        sys_get_date(&temp_sys_date);
        libc_itoa(temp_sys_date.year, ptr, 4);
        *(ptr + 4) = '-';
        libc_itoa(temp_sys_date.month, ptr + 5, 2);
        *(ptr + 7) = '-';
        libc_itoa(temp_sys_date.day, ptr + 8, 2);
        break;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ���ļ�����ת��Ϊ�ַ���
 * \param[in]    uint16 value����������
 * \param[out]   uint8 *ptr���ַ������
 * \return       none
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void _format_num(uint16 value, uint8 *ptr)
{
    if (value < 10)
    {
        libc_itoa(value, ptr, 1);
    }
    if ((10 <= value) && (value < 100))
    {
        libc_itoa(value, ptr, 2);
    }
    if ((100 <= value) && (value < 1000))
    {
        libc_itoa(value, ptr, 3);
    }
    if ((1000 <= value) && (value < 10000))
    {
        libc_itoa(value, ptr, 4);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ����fmƵ��д��FMxx.x����FMxxx.x������g_title_buf������ʾ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void _get_fm_info(void)
{
    uint32 tmp;

    tmp = g_fm_feq / 1000;
    if (tmp < 100)
    {
        libc_itoa(tmp, g_title_buf, 2);
        g_title_buf[2] = '.';
        tmp = (g_fm_feq - tmp * 1000) / 100;
        libc_itoa(tmp, &g_title_buf[3], 1);
        g_title_buf[4] = 'F';
        g_title_buf[5] = 'M';
        g_title_buf[6] = 0;
    }
    else
    {
        libc_itoa(tmp, g_title_buf, 3);
        g_title_buf[3] = '.';
        tmp = (g_fm_feq - tmp * 1000) / 100;
        libc_itoa(tmp, &g_title_buf[4], 1);
        g_title_buf[5] = 'F';
        g_title_buf[6] = 'M';
        g_title_buf[7] = 0;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ����Ӧ����Ҫ��ʾ����Ϣ����÷ŵ�ap_string_infor��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void _format_ap_info(void)
{
#if 0	
    string_desc_t string_desc;
    string_desc_t string_name;
    uint8 tmpbuf[11];

    libc_memset(tmpbuf, 0, sizeof(tmpbuf));

    //�����ǰap������ʾ��Ϣ������record����ap_string_box��Ӧ����Ϊ0xffff
    if (ap_string_box[g_active_index] == 0xffff)
    {
        return;
    }

    //��Ҫ��ʾӦ�õ���Ϣ�����統ǰ����title
    if (ap_string_box[g_active_index] == TRACK_S)
    {
        if (get_engine_type() == ENGINE_MUSIC)
        {
            _get_music_info();
            libc_memcpy(ap_string_infor, g_title_buf, MAX_INFO_LEN);
        }
        else
        {
            _get_fm_info();
            libc_memcpy(ap_string_infor, g_title_buf, MAX_INFO_LEN);
        }
        return;
    }
    //��ʾ����
    if (ap_string_box[g_active_index] == DATE_S)
    {
        _format_date(tmpbuf);
        libc_memcpy(ap_string_infor, tmpbuf, 11);
        return;
    }

    //���������Ҫ��ʾ�ļ�����
    _format_num(ap_file_amount[g_active_index], tmpbuf);

    string_desc.data.str = ap_string_infor;
    string_desc.length = MAX_INFO_LEN;
    string_desc.argv = ap_string_info_box[g_active_index];
    string_desc.language = (int8) g_setting_comval.g_comval.language_id;
    string_name.data.str = tmpbuf;
    string_name.length = 12;
    //string_name.argv = 0;
    string_name.language = (int8) g_setting_comval.g_comval.language_id;
    //��ʽ��ap��Ϣ
    com_string_format(&string_desc, &string_name);
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ʾap����Ϣ�������ļ�����������title���ߵ�ǰ����
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void _show_ap_info(void)
{
    /*pic_box �ӿڱ���*/
    style_infor_t mainmenu_sty;
    /*text box�Ľӿڱ���*/
    textbox_private_t textbox_param;
    uint8 tag_backup;

    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIMER_TAG_MAIN);

    /*��ʾap����*/
/*    if (g_active_index == g_mainmenu_var.cfg_var.total_item)
    {
        if (get_engine_state() == ENGINE_STATE_PLAYING)
        {
            mainmenu_sty.style_id = STY_NOWPLAY_TEXT;
        }
        else
        {
            mainmenu_sty.style_id = STY_LASTPLAY_TEXT;
        }
    }
    else*/
    {
        mainmenu_sty.style_id = ap_text_box[g_frame_num][g_active_index];
    }
    mainmenu_sty.type = UI_AP;
    ui_show_textbox(&mainmenu_sty, NULL, TEXTBOX_DRAW_NORMAL);

#if 0
    /*��ʾӦ����Ϣ*/
    mainmenu_sty.style_id = ap_info_box[g_frame_num][g_active_index];
    mainmenu_sty.type = UI_AP;
    //textbox_param.lang_id = (int8) g_setting_comval.g_comval.language_id;
    //textbox_param.str_id = -1;
    textbox_param.str_id = ap_string_box[g_active_index];
    if (ap_string_box[g_active_index] != 0xffff)
    {
        textbox_param.str_value = ap_string_infor;
        // �ж��Ƿ���UNICODE����
        if ((*(textbox_param.str_value) == 0xff) && (*((textbox_param.str_value) + 1) == 0xfe))
        {
            textbox_param.lang_id = UNICODEDATA;
        }
        else if ((*(textbox_param.str_value) == 0xef) && (*((textbox_param.str_value) + 1) == 0xbb)
                && (*((textbox_param.str_value) + 2) == 0xbf))
        {
            textbox_param.lang_id = UTF_8DATA;
        }
        else
        {
            textbox_param.lang_id = (int8) g_setting_comval.g_comval.language_id;
        }
    }
    else
    {
        textbox_param.str_value = NULL;
    }

    if ((ui_result_e) ui_show_textbox(&mainmenu_sty, &textbox_param, TEXTBOX_DRAW_ITEM) 
        == UI_NEED_SCROLL)//�Թ�����ʽ��ʾ����+����
    {
        //����������������ʱ��
        if (scroll_timer == -1)//δ����������
        {
            scroll_timer = set_app_timer(APP_TIMER_ATTRB_UI, 80, item_scroll_handle);
        }
    }
    else
    {
        //����������رչ�����ʱ��
        if (scroll_timer != -1)//�Ѵ�����ɾ��
        {
            kill_app_timer(scroll_timer);
            scroll_timer = -1;
        }
    }
#endif    
    change_app_timer_tag(tag_backup);
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ʾ������Ϣ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void mainmenu_show_others(void)
{
    gui_headbar(HEADBAR_UPDATE_ALL);

    if (g_frame_num == 0)
    {
        _format_ap_info();
    }
    _show_ap_info();

    /*QAC���warning����Ϊ����FRAME_NUM=1�����������������Զ���������Խ������Σ����FRAME_NUM != 1 ����Ҫ�ָ�
     g_frame_num++;
     if (g_frame_num >= FRAME_NUM)
     {
     g_frame_num = 0;
     }
     */
}

//����������
void _paint_desktop(mainmenu_ui_result_e ui_result)
{
    /*pic_box �ӿڱ���*/
    style_infor_t mainmenu_sty;
    picbox_private_t picbox_param;
    //animation_com_data_t mainmenu_animation;

    g_direction = ui_result;

    //ui_result == BOTTOM��ֻ���µײ�Ӧ�õ���Ϣ
    if (ui_result == BOTTOM)
    {
        g_frame_num = FRAME_NUM - 1;
        _format_ap_info();
        _show_ap_info();
        return;
    }

    g_frame_num = FRAME_NUM - 1;
    mainmenu_sty.style_id = ap_pic_box[g_active_index];
    mainmenu_sty.type = UI_AP;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = g_frame_num;
    //ui_show_picbox(&mainmenu_sty, &picbox_param);
    ui_show_picbox(&mainmenu_sty, NULL);
    /*��ʾheadbar*/
    gui_headbar(headbar_update);
    _format_ap_info();
    _show_ap_info();
}
