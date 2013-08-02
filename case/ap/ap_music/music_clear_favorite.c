/*******************************************************************************
 *                              US212A
 *                            Module: MUSIC_CLEAR_FAVORITE
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-11-9 16:12:18           1.0              build this file
 *******************************************************************************/

#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 *	   ���ĳһ�ղؼ��б������ǰ���Ÿ��������Ǹ��ղؼе���Ŀ
 *     ��ֹͣ���ֲ��ţ����ز˵�����
 * \param[in]  target_type Ŀ���ղؼ�����
 * \param[out] ���ز������,�Ƿ����ɹ�
 * \return
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e clr_favorite_list(uint8 target_type)
{
    uint8 ret = 0;
    bool need_check_engine = FALSE;

    dialog_com_data_t dialog_clr;
    //Ŀ���ַ���
    string_desc_t ret_clr_str;
    //Դ�ַ���
    string_desc_t clr_src_str;

    app_result_e clr_result;

    //�Ƿ��̨����music
    engine_type_e engine_type = get_engine_type();

    if (engine_type == ENGINE_MUSIC)
    {
        music_get_filepath(&g_file_path_info);
        if (g_file_path_info.file_source == target_type)
        {
            need_check_engine = TRUE;
        }
    }

    dialog_clr.dialog_type = DIALOG_INFOR_WAIT;
    dialog_clr.icon_id = -1;

    //�ַ���
    dialog_clr.string_desc = &ret_clr_str;
    
    ret = fsel_favorite_clear(target_type);
    //�ر�����
    if ((need_check_engine == TRUE)&&(ret == TRUE))
    {
        music_close_engine();
    }
    if (ret == TRUE)
    {
        //���ڲ����б�xxx�����
        clr_src_str.language = UNICODEID;

        //����ĸ��ղؼ�
        if (target_type == FSEL_TYPE_LISTFAVOR1)
        {
            clr_src_str.data.id = S_NOW_FAVORITE1;
        }
        else if (target_type == FSEL_TYPE_LISTFAVOR2)
        {
            clr_src_str.data.id = S_NOW_FAVORITE2;
        }
        else
        {
            clr_src_str.data.id = S_NOW_FAVORITE3;
        }
        
        ret_clr_str.argv = S_ALREADY_CLR_FAVORITE;
        
        //ret_clr_str.data.str = g_insert_dialog;
        ret_clr_str.data.str = g_temp_buffer_p;
        ret_clr_str.length = 128;
        
        //ret_clr_str.language = (int8) g_setting_comval.g_comval.language_id;
        ret_clr_str.language = UNICODEDATA;
         //��֯�Ի����ַ���
        com_string_format(&ret_clr_str, &clr_src_str);
    
        gui_dialog(DIALOG_MSG, &dialog_clr);
    }

    if (need_check_engine == TRUE)
    {
        //δ�ҵ�����
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT,S_NO_SONG);

    }
    //���ػص��ú����Ĳ˵���
    clr_result = RESULT_LISTMENU_MENU;
    return clr_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	   ��������б�˵��ص�����
 * \param[in]  target_type �ղؼ�����
 * \param[out] ���ز������,�Ƿ����ɹ�
 * \return
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e clr_favorite_menu(uint8 target_type)
{

    dialog_com_data_t ask_clr_data;
    //Ŀ���ַ���
    string_desc_t ask_clr_str;   
    //�ַ�����֯
    string_desc_t format_data;

    app_result_e ret;

    app_result_e clr_result;
    
    format_data.language = UNICODEID;

    //����ĸ��ղؼ�
    if (target_type == FSEL_TYPE_LISTFAVOR1)
    {
        format_data.data.id = S_NOW_FAVORITE1;
    }
    else if (target_type == FSEL_TYPE_LISTFAVOR2)
    {
        format_data.data.id = S_NOW_FAVORITE2;
    }
    else
    {
        format_data.data.id = S_NOW_FAVORITE3;
    }
    
    ask_clr_str.argv = S_CLEAR_FAVORITE1;
    
    //ask_clr_str.data.str = g_insert_dialog;
    ask_clr_str.data.str = g_temp_buffer_p;
    ask_clr_str.length = 128;
    
    //ask_clr_str.language = (int8) g_setting_comval.g_comval.language_id;
    ask_clr_str.language = UNICODEDATA;
     //��֯�Ի����ַ���
    com_string_format(&ask_clr_str, &format_data);
    
    //�Ի�������
    ask_clr_data.dialog_type = DIALOG_BUTTON_INCLUDE;
    ask_clr_data.icon_id = -1;
    //�ַ���
    ask_clr_data.string_desc = &ask_clr_str;

    //��������
    ask_clr_data.button_type = BUTTON_YESNO;
    /*! Ĭ�ϼ������ */
    ask_clr_data.active_default = 0x00;

    ret = gui_dialog(DIALOG_ASK, &ask_clr_data);

    if (ret == RESULT_DIALOG_YES)
    {
        clr_result = clr_favorite_list(target_type);
    }
    else if (ret == RESULT_REDRAW)
    {
        clr_result = RESULT_LISTMENU_MENU;
    }
    else
    {
        clr_result = ret;
    }
    return clr_result;
}

/*! \endcond */
