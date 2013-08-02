/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-15 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     pic_option_menu.c
 * \brief    Picture ��option�˵��б�ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/15
 *******************************************************************************/
#include  "picture.h"
#include  "pic_menu_cfg.h"

#define VALUE_SLIDE_TIME_MIN  2
#define VALUE_SLIDE_TIME_MAX  30
#define VALUE_SLIDE_TIME_STEP 1


const uint16 backlight_timer_text_table[] =
{ S_TIME_ALWAYS, S_TIME_10S, S_TIME_20S, S_TIME_30S };

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture���ڲ���Ҷ�Ӳ˵�ִ�к���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_playing(void *param)
{
    return RESULT_NOWPLAYING;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture��һ�β���Ҷ�Ӳ˵�ִ�к���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_last_play(void *param)
{
    return RESULT_LASTPLAY;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture���ñ������ȹ��ܻص�����
 * \param[in]    param  ���õı������
 * \param[out]   none
 * \return       bool   TRUE ���óɹ�
 * \ingroup      pic_optionmenu
 * \note
 * \li  ��Ϊmenu_func_bright������ؼ��ص�����������
 *******************************************************************************/
static bool _set_lightness_callback(int16 param)
{
    g_comval.lightness = (uint8) param;

    //���õ�Ӳ��
    com_set_contrast((uint8) param);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture���ñ�������Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 * \li  ��Ϊmenu_func_bright������ؼ��ص�����������
 * \li  ��������Կ���һ���������ӳ�����֧�ֵ������Ͷ����
 *******************************************************************************/
app_result_e menu_func_bright(void *param)
{
    //��������
    app_result_e result;
    //������ؼ����ݽṹ��
    param_com_data_t s_param_com;
    //parambox˽����������
    parambox_private_t s_param_private;
    //parambox������������
    parambox_one_t s_param_item;

    //������λ
    s_param_item.unit_id = -1;
    //������Сֵ
    s_param_item.min = VALUE_LIGHTNESS_MIN;
    //�������ֵ
    s_param_item.max = VALUE_LIGHTNESS_MAX;
    //������ʾʱ���λ��
    s_param_item.max_number = 2;
    //�������ڲ���
    s_param_item.step = VALUE_LIGHTNESS_STEP;
    //������ǰֵ
    s_param_item.value = g_comval.lightness;
    //������ؼ������Ƿ����ѭ��
    s_param_item.cycle = FALSE;
    //������������
    s_param_item.adjust_func = NULL;
    //���ڲ���ʱʵʱ��Ӧ����
    s_param_item.callback = _set_lightness_callback;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    //�����ʱָ����ǰ�ļ�����
    s_param_private.active = 0;
    //��������
    s_param_private.param_cnt = 1;
    s_param_private.items = &s_param_item;
    s_param_private.sign_flag = 0;
    s_param_com.private_data = &s_param_private;

    //������ģ�飬��Ӧ�Ĳ�������
    result = gui_set_parameter(SLIDER_SIMPLE, &s_param_com);
    //ֻ��ȷ�ϼ�ʱ����Ч
    if (result == RESULT_CONFIRM)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_BRIGHT_SETTED);
        g_comval.lightness = (uint8) s_param_com.private_data->items->value;
        result = RESULT_REDRAW;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture����ʱ����˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 * \li  ͨ�������param�������ַ�������ƥ�䣬ȷ����ǰ�ļ�����
 * \li
 *******************************************************************************/
app_result_e menu_func_backlight_timer(void *param)
{
    menu_title_action_t *menu_title;
    menu_title = (menu_title_action_t*) param;

    //light_timer��0.5��Ϊ��λ��0��ʾ���ⳣ��
    menu_title->str_id = backlight_timer_text_table[g_comval.light_timer / 20];
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 10s����Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_timer_10s(void *param)
{
    g_comval.light_timer = 10 * 2;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 20s����Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_timer_20s(void *param)
{
    g_comval.light_timer = 20 * 2;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 30s����Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_timer_30s(void *param)
{
    g_comval.light_timer = 30 * 2;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture ��������Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_timer_all_time(void *param)
{
    g_comval.light_timer = 0;
    return RESULT_MENU_PARENT;
}
#if 0
/******************************************************************************/
/*!
 * \par  Description:
 *	  picture �õ�Ƭ��ӳ���˵�ִ�к���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note ���ڸø��˵��¼��˵�����Ҫ�����ַ���ƥ�䣬��˲���Ҫ��ִ�к���
 *******************************************************************************/
app_result_e menu_func_slideshow_set(void *param)
{
    return RESULT_MENU_SON;
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture �õ�Ƭ����ʱ��Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_per_slide(void *param)
{
    app_result_e result;
    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item;

    //������λΪ��
    s_param_item.unit_id = S_SECOND;
    s_param_item.min = VALUE_SLIDE_TIME_MIN;
    s_param_item.max = VALUE_SLIDE_TIME_MAX;
    s_param_item.max_number = 2;
    s_param_item.step = VALUE_SLIDE_TIME_STEP;
    s_param_item.value = g_comval.slide_time;
    s_param_item.value_str = NULL;
    //�������ֵѭ������
    s_param_item.cycle = TRUE;
    s_param_item.adjust_func = NULL;
    s_param_item.callback = NULL;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 1;
    s_param_private.items = &s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;

    result = gui_set_parameter(PARAM_SIMPLE, &s_param_com);
    if (result == RESULT_CONFIRM)
    {
        g_comval.slide_time = (uint8) s_param_com.private_data->items->value;
        result = RESULT_REDRAW;
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  picture �õ�Ƭ�ظ����Ÿ��˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_repeat(void *param)
{
    uint16 active_id;
    menu_title_action_t * menu_title;
    menu_title = (menu_title_action_t*) param;
    if (g_comval.slide_repeat_mode == FSEL_MODE_NORMAL)
    {
        //loopnormal
        active_id = S_OFF;
    }
    else
    {
        //loopall
        active_id = S_ON;
    }
    menu_title->str_id = active_id;
    return RESULT_MENU_SON;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  picture ͼƬ�����ظ���Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_repeat_on(void *param)
{
    uint8 cur_mode = (g_comval.slide_shuffle << 4);
    g_comval.slide_repeat_mode = FSEL_MODE_LOOPALL;
    cur_mode |= g_comval.slide_repeat_mode;
    fsel_set_mode(cur_mode);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture ͼƬ�ظ����Ź�Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_repeat_off(void *param)
{
    uint8 cur_mode = (g_comval.slide_shuffle << 4);
    g_comval.slide_repeat_mode = FSEL_MODE_NORMAL;
    cur_mode |= g_comval.slide_repeat_mode;
    fsel_set_mode(cur_mode);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture ͼƬ������Ÿ��˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_shuffle(void *param)
{
    uint16 active_id;
    menu_title_action_t * menu_title;
    menu_title = (menu_title_action_t*) param;
    if (g_comval.slide_shuffle == 0)
    {
        active_id = S_OFF;
    }
    else
    {
        active_id = S_ON;
    }
    //ָ��active��
    menu_title->str_id = active_id;
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture ͼƬ������ſ�Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_shuffle_on(void *param)
{
    uint8 cur_mode = g_comval.slide_repeat_mode;
    g_comval.slide_shuffle = 1;
    cur_mode |= 0x10;
    fsel_set_mode(cur_mode);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture ͼƬ������Ź�Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_shuffle_off(void *param)
{
    uint8 cur_mode = g_comval.slide_repeat_mode;
    g_comval.slide_shuffle = 0;
    fsel_set_mode(cur_mode);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture ͼƬ����ͼ���ø��˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_preview(void *param)
{
    uint16 active_id;
    menu_title_action_t * menu_title;
    menu_title = (menu_title_action_t*) param;
    if ((g_picture_var.thumbnail_flag & VALUE_PREVIEW_ON) == 0)
    {
        active_id = S_OFF;
    }
    else
    {
        active_id = S_ON;
    }
    menu_title->str_id = active_id;
    return RESULT_MENU_SON;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  picture ͼƬ����ͼ��ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_preview_on(void *param)
{
    g_picture_var.thumbnail_flag = (uint8)(g_picture_var.thumbnail_flag | VALUE_PREVIEW_ON);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture ͼƬ����ͼ��ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_preview_off(void *param)
{
    g_picture_var.thumbnail_flag = (uint8)(g_picture_var.thumbnail_flag & VALUE_PREVIEW_OFF);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture ͼƬɾ���ļ�Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 * \li  �ú��������common��ui_delete�ؼ�ʵ���ļ�ɾ������
 * \li  �����̨��music���ţ�����ֹͣmusic���ţ���ɾ����������¿���
 *******************************************************************************/
app_result_e menu_func_delete_pic(void *param)
{
    bool ret_val;
    app_result_e result;
    del_com_data_t del_com;

    if (g_picture_scene_prev == SCENE_PLAY)
    {
        //��֪��location
        del_com.del_no = 0;
    }
    else
    {
        //�б�ģʽ,������ȡlocation
        del_com.del_no = dir_control.list_no;
        fsel_browser_select(dir_control.list_no);
        fsel_browser_get_location(&g_picture_var.path.file_path, g_picture_var.path.file_source);
        fsel_browser_set_file(&g_picture_var.path.file_path, g_picture_var.path.file_source);
    }

    vfs_get_name(picture_mount, g_picture_filename, LIST_ITEM_BUFFER_LEN / 2);
    if ((g_picture_filename[0] != 0xff) && (g_picture_filename[0] != 0xfe))
    {
        com_dot_to_shortname(g_picture_filename);
    }

    del_com.del_mode = DEL_MODE_FILE;
    del_com.filename = g_picture_filename;
    del_com.del_playing = FALSE;
    del_com.del_func = NULL;
    del_com.del_self = TRUE;
    result = gui_delete(&g_picture_var.path, &del_com);
    if ((result == RESULT_DELETE_FILE) || (result == RESULT_DELETE_DIRLIST))
    {
        //�˳���ǰoption�˵�����
        if(g_picture_var.path.file_path.dirlocation.file_total == 0)
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_PIC_FILE);
            result = RESULT_MAIN_APP;
        }
        else
        {
            g_del_file = TRUE;

            //����location��Ϣ
            if (g_picture_var.path.file_path.dirlocation.file_num > g_picture_var.path.file_path.dirlocation.file_total)
            {
                g_picture_var.path.file_path.dirlocation.file_num = 1;
            }
            //��ȡ��һ��
            ret_val = fsel_get_byno(g_picture_var.path.file_path.dirlocation.filename,
                    g_picture_var.path.file_path.dirlocation.file_num);

            if (ret_val == TRUE)
            {
                fsel_get_location(&g_picture_var.path.file_path.dirlocation, g_picture_var.path.file_source);
            }
            result = RESULT_MENU_EXIT;
        }
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  picture ͼƬ���������б�Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_generate_pic_playlist(void *param)
{
    return RESULT_CREATE_LIST_APP;
}


/******************************************************************************/
/*!
 * \par  Description:
 *	  picture �˵��option���Ļص�����
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_option_option(void)
{
    //������һ��
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Option�˵���������
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \retval       app_result_e, �˵��ؼ����صĽṹ��Ϣ
 * \ingroup      pic_optionmenu
 * \note
 * \li  ���ݵ�ǰ�Ƿ��к�̨�����Լ�����ͼ�����Ƿ�֧��ȷ����ڲ˵����
 *******************************************************************************/
app_result_e pic_option_menulist(void)
{
    uint8 i, j;
    engine_state_e play_sta;
    app_result_e result = RESULT_NULL;

    //�˵��ؼ�����
    menu_com_data_t op_menu;
    op_menu.app_id = APP_ID_PICTURE;

    play_sta = get_engine_state();

    if (play_sta == ENGINE_STATE_NULL)
    {
        i = 0;
    }
    else if (play_sta == ENGINE_STATE_PLAYING)
    {
        i = 1;
    }
    else
    {
        i = 2;
    }

    if ((g_picture_var.thumbnail_flag & VALUE_PREVIEW_SUPPORT) == 0)
    {
        j = 0;
    }
    else
    {
        j = 1;
    }

    //���岻ͬģʽ�µ���ڲ˵�
    op_menu.menu_entry = i + j * 3;
    //��ǰ�˵��ؼ�·��������
    op_menu.path_id = 0;
    //�ײ�˵��ص�����
    op_menu.menu_func = NULL;
    //���ģʽ
    op_menu.browse_mode = 0;

    result = gui_menulist(MENULIST_OPTION, &op_menu);
    if (result == RESULT_REDRAW)
    {
        if (g_picture_scene_prev == SCENE_PLAY)
        {
            result = RESULT_PICTURE_PLAY;
        }
        else
        {
            result = RESULT_PIC_FILELIST;
        }
    }
    return result;
}

