/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-21          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_menu_config.h"

#include "app_music_bookmark.h"

//bank data for paramter box private data

/*parambox_one_t timer_param_box _BANK_DATA_ATTR_;

 parambox_private_t timer_param_private _BANK_DATA_ATTR_;

 param_com_data_t timer_param_comdata _BANK_DATA_ATTR_;*/

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_sleeptimer_sure(void *param)
 * \���ö�ʱ�ػ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_otheritem.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_sleeptimer_sure(void *param)
{
    //�����style id
    string_desc_t string_desc;//Ŀ���ַ����ṹ
    string_desc_t string_name;//Դ�ַ����ṹ
    uint8 temp_string[5] =
    { 0 };

    //ָ�򷵻ز�����ָ��
    menu_title_action_t* retptr = (menu_title_action_t*) param;

    if (g_setting_comval.g_comval.sleep_timer == 0)
    {
        //������һ���˵�
        //active�ַ���id
        retptr->str_id = S_OFF;
    }
    else
    {
        //����һ����ڲ˵�styid
        retptr->menu_entry = SLEEP_TIMER_SETMENU;

        //string_desc.data.str = g_insert_dialog;
        //string_desc.length = 80;
        string_desc.data.str = g_temp_buffer_p;
        string_desc.length = 128;
        string_desc.argv = S_TIME_REMAIN;
        string_desc.language = (int8) g_setting_comval.g_comval.language_id;
        //string_desc.language = UNICODEDATA;

        libc_itoa((uint32) (g_setting_comval.g_comval.sleep_timer - g_sys_counter.sleep_counter / 120),
                &temp_string[1], 2); //��ʱ��ת���ַ�
        temp_string[0] = '[';
        temp_string[3] = ']';
        temp_string[4] = 0;
        string_name.data.str = temp_string;
        string_name.length = 5;
        string_name.language = (int8) g_setting_comval.g_comval.language_id;

        com_string_format(&string_desc, &string_name);

        //ȷ���ַ���id
        retptr->str_id = S_SET_SLEEP_TIMER;
        //retptr->ret_str = g_insert_dialog;
        retptr->ret_str = g_temp_buffer_p;
        retptr->source_id = -1;
        retptr->style_id = MENULIST_TITLE;
    }
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_closesleep_sure(void)
 * \�رն�ʱ�ػ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_otheritem.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_closesleep_sure(void *param)
{
    //��ʱ�ػ�ʱ������
    g_setting_comval.g_comval.sleep_timer = 0;
    //������һ���˵�
    return RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڵ����ػ�ʱ�����Сֵ
 *      �ص�˵�������ڵ��ڲ���ֵ����ϴ󳬹������ص㣬Ϊ�����γɱ�����ϵ����������
 *      99/33=3 ����һ���̶ȱ�ʾ3����ֵ����ֵ�仯3����һ���̶�
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
adjust_result_e adjust_min_time(parambox_one_t *params, uint8 active, bool inc)
{
    if (params->value == 0)
    {
        params->value = 1;
    }
    return ADJUST_RESULT_VALUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_setsleep_sure(void)
 * \���ö�ʱ�ػ�ʱ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_otheritem.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_setsleep_sure(void *param)
{
    parambox_one_t timer_param_box;

    parambox_private_t timer_param_private;

    param_com_data_t timer_param_comdata;
    //���ö�ʱ�ػ�ʱ��
    //����ֵ
    app_result_e return_result_temp;

    uint8 sleep_timer = g_setting_comval.g_comval.sleep_timer;

    if (sleep_timer == 0)
    {
        sleep_timer = 1;
    }

    timer_param_box.min = VALUE_SHUT_DOWN_TIME_MIN - 1;
    timer_param_box.max = VALUE_SHUT_DOWN_TIME_MAX;
    timer_param_box.step = VALUE_SHUT_DOWN_TIME_STEP;
    timer_param_box.value = (int16) sleep_timer; //value�Է���Ϊ��λ
    timer_param_box.cycle = FALSE;
    timer_param_box.adjust_func = adjust_min_time;
    timer_param_box.value_str = NULL;
    timer_param_box.callback = NULL;
    timer_param_box.unit_id = S_UINT_MIN;

    timer_param_private.back_id = -1;
    timer_param_private.title_id = -1;
    timer_param_private.icon_id = -1;
    timer_param_private.active = 0;
    timer_param_private.param_cnt = 1;
    timer_param_private.items = &timer_param_box;
    timer_param_private.sign_flag = 0;
    timer_param_private.scale = 2;

    timer_param_comdata.private_data = &timer_param_private;

    //�ؼ�����
    return_result_temp = gui_set_parameter(SLIDER_NUM, &timer_param_comdata);

    if (return_result_temp == RESULT_CONFIRM)
    {
        menu_title_action_t* menu_title_shut_down_timer;

        g_setting_comval.g_comval.sleep_timer = (uint8) timer_param_box.value; //value�Է���Ϊ��λ

        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_SET_SLEEP_TIMER_HINT);

        menu_title_shut_down_timer = (menu_title_action_t*) param;
        menu_title_shut_down_timer->str_id = S_SET_SLEEP_TIMER;

        return_result_temp = RESULT_MENU_CUR; //���½��뵱ǰ�������������⴦��
    }
    return return_result_temp;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_delete_sure(void)
 * \ɾ����ǰ���ڲ����ļ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_otheritem.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_delete_sure(void *param)
{
    app_result_e result_value;
    del_com_data_t del_com;
    uint32 cur_direnry, cur_cluster;

    plist_location_t* locatptr = (plist_location_t*) &g_file_path_info.file_path.plist_location;

    cur_cluster = locatptr->cluster_no;
    cur_direnry = locatptr->dir_entry;

    music_get_filepath(&g_file_path_info);

    //�ж��Ƿ��и�
    if ((cur_cluster != g_file_path_info.file_path.dirlocation.cluster_no) || (cur_direnry
            != g_file_path_info.file_path.dirlocation.dir_entry))
    {
        //����и裬��Ҫ���»�ȡ������ID3 TITLE
        fsel_browser_set_file(&g_file_path_info.file_path.plist_location, g_file_path_info.file_source);
        get_music_id3_title(g_title_buf, sizeof(g_title_buf), locatptr->filename);
    }

    del_com.del_mode = DEL_MODE_FILE;
    del_com.filename = g_title_buf;
    //ɾ�����ڲ��ŵ��ļ�
    del_com.del_playing = TRUE;
    del_com.del_self = TRUE;
    del_com.del_func = NULL;
    result_value = gui_delete(&g_file_path_info, &del_com);
    if (result_value == RESULT_DELETE_FILE)
    {
        if (g_file_path_info.file_path.plist_location.file_total == 1)
        {
            //ֻ���������ȥɾ�������ļ��Ž�����ʾ��������ap���뷵�غ�
            //������ʾ��������ﲻ��Ҫ��ʾ
            if(g_enter_mode == PARAM_FROM_MAINMENU)
            {
                //δ�ҵ�����!
                gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);
            }
            
            //ֻʣ��һ�׸�����֪ͨ�����˳�
            music_close_engine();

            if (g_enter_mode == PARAM_FROM_MAINMENU)
            {
                result_value = RESULT_LISTMENU_MENU;
            }
            else
            {
                result_value = RESULT_RETURN_BROWSER;
            }
        }
        else
        {
            music_play_next();

            music_play();
        }
        del_bookmark_file(&g_file_path_info.file_path.dirlocation);
    }
    if ((result_value == RESULT_DELETE_FILE) || (result_value == RESULT_ERROR))
    {
        result_value = RESULT_PLAYING_SCENE;
    }
    return result_value;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_getlicense_sure(void)
 * \��ȡ��Ȩ��Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_otheritem.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_getlicense_sure(void *param)
{
    //music_get_fileinfo();
    //������һ��
    return RESULT_MENU_PARENT;
}
