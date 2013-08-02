/*******************************************************************************
 *                              US212A
 *                            Module: video
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-15 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     video
 * \brief    video �����˵��б�ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/15
 *******************************************************************************/
#include  "video.h"
#include "video_menu_cfg.h"
static const uint8 menu_entry[] =
{
    VIDEO_NOPLAY_MENU_ENTRY,
    VIDEO_PLAYING_MENU_ENTRY,
    VIDEO_LASTPLAY_MENU_ENTRY,
    VIDEO_CARD_MENU_ENTRY
};

/******************************************************************************/
/*!
 * \par  Description:
 *    ���ڲ��Ų˵���ִ�к���
 * \param[in]    param ��ʵ������
 * \param[out]   none
 * \return       the result  �˵��ؼ����صĽṹ��Ϣ
 * \retval       RESULT_NOWPLAYING  �÷���ֵ�᷵�ص��������ȣ���ת������Ϣ
 * \ingroup      video_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_playing(void *param)
{
    return RESULT_NOWPLAYING;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��һ�β��Ų˵���ִ�к���
 * \param[in]    param ��ʵ������
 * \param[out]   none
 * \return       the result  �˵��ؼ����صĽṹ��Ϣ
 * \retval       RESULT_LASTPLAY  �÷���ֵ�᷵�ص��������ȣ���ת������Ϣ
 * \ingroup      video_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_last_play(void *param)
{
    return RESULT_LASTPLAY;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���öԱȶ�
 * \param[in]    param �Աȶ�ֵ
 * \param[out]   none
 * \return       the result
 * \retval       TRUE
 * \ingroup      video_optionmenu
 * \note
 * \li  �ڲ���������Ϊ�������ڿؼ��Ļص�����
 *******************************************************************************/
static bool _set_lightness_callback(int16 param)
{
    g_comval.lightness = (uint8) param;
    //���õ�Ӳ��
    com_set_contrast(g_comval.lightness);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���öԱȶȲ˵���ص�����
 * \param[in]    param  ��ʵ������
 * \param[out]   none
 * \return       the result  �������ڿؼ�����ֵ
 * \ingroup      video_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_bright(void *param)
{
    //��������
    uint16 style_id;
    app_result_e result;
    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item;

    s_param_item.min = 0;
    s_param_item.max = 11;
    s_param_item.step = 1;
    s_param_item.value = g_comval.lightness;
    s_param_item.cycle = FALSE;
    s_param_item.adjust_func = NULL;
    s_param_item.callback = _set_lightness_callback;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 1;
    s_param_private.items = &s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;

    if (g_video_scene_prev == SCENE_PLAY)
    {
        style_id = SLIDER_SIMPLE_V;
    }
    else
    {
        style_id = SLIDER_SIMPLE;
    }

    result = gui_set_parameter(style_id, &s_param_com);
    if (result == RESULT_CONFIRM)
    {
        g_comval.lightness = (uint8) s_param_com.private_data->items->value;
        result = RESULT_REDRAW;
    }
    return result; //������һ��
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ɾ��ͼƬ�˵���ִ�к���
 * \param[in]    param ��ʵ������
 * \param[out]   none
 * \return       the result  ui_delete�ؼ�����ֵ
 * \ingroup      video_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_delete_video(void *param)
{
    bool ret_val;
    app_result_e result;
    del_com_data_t del_com;

    if (g_video_scene_prev == SCENE_PLAY)
    {
        //��֪��location
        del_com.del_no = 0;
    }
    else
    {
        //�б�ģʽ,������ȡlocation
        del_com.del_no = dir_control.list_no;
        fsel_browser_select(dir_control.list_no);
        fsel_browser_get_location(&g_video_var.path.file_path, g_video_var.path.file_source);
        fsel_browser_set_file(&g_video_var.path.file_path, g_video_var.path.file_source);
    }

    vfs_get_name(video_mount, g_video_filename, LIST_ITEM_BUFFER_LEN / 2);
    if ((g_video_filename[0] != 0xff) && (g_video_filename[0] != 0xfe))
    {
        com_dot_to_shortname(g_video_filename);
    }

    del_com.del_mode = DEL_MODE_FILE;
    del_com.filename = g_video_filename;
    del_com.del_playing = FALSE;
    del_com.del_self = TRUE;
    del_com.del_func = NULL;
    result = gui_delete(&g_video_var.path, &del_com);
    if ((result == RESULT_DELETE_FILE) || (result == RESULT_DELETE_DIRLIST))
    {
        //�˳���ǰoption�˵�����
        if(g_video_var.path.file_path.dirlocation.file_total == 0)
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_VIDEO_FILE);
            result = RESULT_MAIN_APP;
        }
        else
        {
            g_del_file = TRUE;

            //��������location
            if (g_video_var.path.file_path.dirlocation.file_num > g_video_var.path.file_path.dirlocation.file_total)
            {
                g_video_var.path.file_path.dirlocation.file_num = 1;
            }
            //��ȡ��һ��
            ret_val = fsel_get_byno(g_video_var.path.file_path.dirlocation.filename,
                    g_video_var.path.file_path.dirlocation.file_num);

            if (ret_val == TRUE)
            {
                fsel_get_location(&g_video_var.path.file_path.dirlocation, g_video_var.path.file_source);
            }
            result = RESULT_MENU_EXIT;
        }

    }
    return result;

}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���������б�˵���ִ�к���
 * \param[in]    param ��ʵ������
 * \param[out]   none
 * \return       the result
 * \retval       RESULT_CREATE_PLAYLIST ��mainģ�齫����ֵת������Ϣ
 * \ingroup      video_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_generate_video_playlist(void *param)
{
    return RESULT_CREATE_LIST_APP;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ����ѭ��ģʽ���˵�ִ�к���
 * \param[in]    param ����Ϊ��ڲ��������ݵ�ǰ��ѭ��ģʽѡ��Ҷ�Ӳ˵�ѡ��ֵ
 * \param[out]   none
 * \return       the result
 * \ingroup      video
 * \note
 * \li  ����ѭ��ģʽֵƥ��Ҷ�Ӳ˵�activeֵ
 *******************************************************************************/
app_result_e menu_func_repeat_mode(void *param)
{
    menu_title_action_t* retptr = (menu_title_action_t*) param;
    retptr->menu_entry = -1;

    switch (g_video_var.repeat_mode)
    {
        //normal
        case VIDEO_REPEAT_NORMAL:
        retptr->str_id = S_REPEAT_NORMAL;
        break;

        //repeate one
        case VIDEO_REPEAT_ONE:
        retptr->str_id = S_REPEAT_ONE;
        break;

        //repeate all
        case VIDEO_REPEAT_ALL:
        retptr->str_id = S_REPEAT_ALL;
        break;

        default:
        retptr->str_id = -1;
        break;
    }
    retptr->ret_str = NULL;
    retptr->source_id = -1;
    return RESULT_MENU_SON;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ����videoѭ��ģʽ
 * \param[in]    param ��ʵ������
 * \param[out]   none
 * \return       the result
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e _video_set_repeat_mode(uint8 mode)
{
    g_video_var.repeat_mode = mode;

    //���ò���ģʽ
    fsel_set_mode(mode);

    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ͨģʽҶ�Ӳ˵���ִ�к���
 * \param[in]    param ��ʵ������
 * \param[out]   none
 * \return       the result
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e menu_func_repeat_normal(void *param)
{
    return _video_set_repeat_mode(VIDEO_REPEAT_NORMAL);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ����ģʽҶ�Ӳ˵���ִ�к���
 * \param[in]    param ��ʵ������
 * \param[out]   none
 * \return       the result
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e menu_func_repeat_one(void *param)
{
    return _video_set_repeat_mode(VIDEO_REPEAT_ONE);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ȫ��ѭ��ģʽҶ�Ӳ˵���ִ�к���
 * \param[in]    param ��ʵ������
 * \param[out]   none
 * \return       the result
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e menu_func_repeat_all(void *param)
{
    //���ò���ģʽ
    return _video_set_repeat_mode(VIDEO_REPEAT_ALL);
}

app_result_e menu_func_option_option(void)
{
    //������һ��
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Option�˵��б���ڲ˵�
 * \param[in]    scene_prev  ��¼����option�˵�����֮ǰ�Ĳ˵�����
 * \param[out]   none
 * \return       the result  �˵��ؼ����صĽṹ��Ϣ
 * \ingroup      video_optionmenu
 * \note         play��������option�˵���filelist��������option�˵�
 *               �˵�����ͬ�����ͬ��ͬʱҪ�����п����޿������ڲ���
 *               /��һ�β���/�޺�̨���ŵĶ�̬�˵���
 *******************************************************************************/
app_result_e video_option_menulist(void)
{
    uint8 i;
    uint16 video_menu_style_id;
    menu_com_data_t op_menu;
    engine_state_e engine_status;
    app_result_e result = RESULT_NULL;

    ui_clear_screen(NULL);

    engine_status = get_engine_state();

    if (engine_status == ENGINE_STATE_NULL)
    {
        i = 0;
    }
    else if (engine_status == ENGINE_STATE_PLAYING)
    {
        i = 1;
    }
    else
    {
        i = 2;
    }

    if (g_video_scene_prev == SCENE_PLAY)
    {
        //���Ž����²˵�Ϊ�������
        com_set_gui_direction(1);
        video_menu_style_id = MENULIST_OPTION_V;
    }
    else
    {
        video_menu_style_id = MENULIST_OPTION;
    }

    /*��ʼ���˵��ؼ�*/
    op_menu.app_id = APP_ID_VIDEO;

    /*���岻ͬģʽ�µ���ڲ˵�*/
    op_menu.menu_entry = menu_entry[i];
    op_menu.path_id = 0;
    /* �ײ�˵��ص����� */
    op_menu.menu_func = NULL;

    /*����˵�˽����Ϣ*/
    result = gui_menulist(video_menu_style_id, &op_menu);

    if (g_video_scene_prev == SCENE_PLAY)
    {
        com_set_gui_direction(0);
    }

    if (result == RESULT_REDRAW)
    {
        if (g_video_scene_prev == SCENE_PLAY)
        {
            result = RESULT_MOVIE_PLAY;
        }
        else
        {
            result = RESULT_VIDEO_FILELIST;
        }
    }
    return result;
}
