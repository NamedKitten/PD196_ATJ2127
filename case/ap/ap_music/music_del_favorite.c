/*******************************************************************************
 *                              US212A
 *                            Module: ADD_FAVORITE
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-9-23 11:11:27           1.0              build this file
 *******************************************************************************/

#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_ab_count(uint8 count)
 * \֪ͨ��̨ɾ����ǰ���ŵ��ղؼ��ļ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_favorite
 * \note
 */
/*******************************************************************************/
bool music_del_favorite_deal(uint16 del_num)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //����ɾ�����ļ����
    msg.content.addr = &del_num;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_DEL_FAVORITE_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool check_del_favorite_result(uint8 ret)
 * \ɾ���ղؼ��ļ�����ʱ����ʾ
 * \��ɾ�����ļ���ɾ������ʾ�޷��ٲ����б���
 * \�ҵ��˸���
 * \param[in]  ret ��ʾѡ�� param1
 * \param[out]
 * \return      bool
 * \retval
 * \ingroup     music_favorite
 * \note
 *******************************************************************************/

bool check_del_favorite_result(uint8 ret)
{
    bool result = TRUE;
    if (ret == FALSE)
    {
        /* �޷��ڲ����б����ҵ��˸���! */
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NOT_IN_FAVORITE);
        result = FALSE;
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e del_favorite_dialog(fsel_type_e target_type, uint8 *file_name,uint8 dialog_type)
 * \ɾ���ղؼ�dialog�ؼ�ʵ��
 * \param[in]  target_type �ղؼж�λ param1
 * \param[in]  file_name �ļ��� param2
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_favorite
 * \note
 *******************************************************************************/
app_result_e del_favorite_dialog(fsel_type_e target_type, uint8 *file_name, uint8 dialog_type)
{
    app_result_e ret;

    dialog_com_data_t dialog;

    //�ַ�����֯
    string_desc_t dest_data;
    string_desc_t source_data[2];

    source_data[0].language = UNICODEID;

    if (*(uint16*) file_name == 0xfeff)
    {
        source_data[1].language = UNICODEDATA;
    }
    else if ((file_name[0] == 0xef) && (file_name[1] == 0xbb) && (file_name[2] == 0xbf))
    {
        source_data[1].language = UTF_8DATA;
    }
    else
    {
        source_data[1].language = (int8) g_setting_comval.g_comval.language_id;
    }

    if (target_type == FSEL_TYPE_LISTFAVOR1)
    {
        source_data[0].data.id = S_NOW_FAVORITE1;
    }
    else if (target_type == FSEL_TYPE_LISTFAVOR2)
    {
        source_data[0].data.id = S_NOW_FAVORITE2;
    }
    else
    {
        source_data[0].data.id = S_NOW_FAVORITE3;
    }

    source_data[1].data.str = file_name;

    source_data[1].length = 32;

    //dest_data.data.str = g_insert_dialog;
    dest_data.data.str = g_temp_buffer_p;
    dest_data.length = 128;

    if (dialog_type == 0)
    {
        //���ĸ��ղؼ�ɾ��
        dest_data.argv = S_DEL_FROM_FAVORITE1;
    }
    else
    {
        //���ĸ��ղؼ�ɾ��
        dest_data.argv = S_ALREADY_DEL_FAVORITE;
    }

    //dest_data.language = (int8) g_setting_comval.g_comval.language_id;
    dest_data.language = UNICODEDATA;

    //��֯�Ի����ַ���
    com_string_format(&dest_data, source_data);
    dialog.icon_id = -1;
    dialog.string_desc = &dest_data;
    if (dialog_type == 0)
    {
        //ѯ��
        //�Ի����������
        dialog.dialog_type = DIALOG_BUTTON_INCLUDE;

        dialog.button_type = BUTTON_YESNO;
        dialog.active_default = 0;
        ret = gui_dialog(DIALOG_ASK, &dialog);
    }
    else
    {
        //��ʾ
        //�Ի����������
        dialog.dialog_type = DIALOG_INFOR_WAIT;
        dialog.active_default = 0;
        ret = gui_dialog(DIALOG_MSG, &dialog);
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e del_favorite_nowplaying(uint8 target_type)
 * \ɾ���ղؼм�¼�������ǰ���ŵ������ղؼ��ļ���ֻ�и��ļ�
 * \ɾ��֮��ر��������棬�����ղؼв˵�
 * \��Ҫ֪���Ƿ��Ǵ��ղؼ�ѡ���������?
 * \ɾ���ղؼм�¼��Դ������:�ղؼУ����Ž���
 * \�����ִ����龰:�Ӳ��Ž���ɾ�������б����ɾ��
 * \param[in]  target_type �ղؼж�λ param1
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_favorite
 * \note
 *******************************************************************************/
app_result_e del_favorite_nowplaying(uint8 target_type)
{
    app_result_e retval;
    uint8 ret;
    bool check_val;
    //uint16 item_total;
    bool need_check_engine = FALSE;

    dir_brow_t get_brower_info;
    uint8 ext_buf[4];
    get_brower_info.name_buf = ext_buf;
    get_brower_info.name_len = 4;

    libc_memcpy(&g_browser_filepath, &g_file_path_info, sizeof(file_path_info_t));
    //��ǰ�Ƿ����ղؼ�ģʽ����
    if (g_file_path_info.file_source == target_type)
    {
        need_check_engine = TRUE;
    }
    else
    {
        //g_browser_filepath.file_source = target_type;
        fsel_exit();
        music_file_init(&g_browser_filepath);
        //��λ���µ����ͱ���
        fsel_browser_set_location(NULL, target_type);
    }

    //ret = fsel_browser_delete(BROW_DEL_FILE, 0, (void *) &g_browser_filepath.file_path.dirlocation);
    ret = fsel_favorite_delete(target_type, 0, &g_browser_filepath.file_path.flist_location);

    check_val = check_del_favorite_result(ret);

    if(check_val == TRUE)
    {
        del_favorite_dialog(target_type, com_name_buffer, 1);
        
        if (need_check_engine == TRUE) 
        {
            //item_total = fsel_get_total();
            fsel_browser_enter_dir(ROOT_DIR, 0, &get_brower_info);
            if (get_brower_info.file_total == 0)
            {
                //ֻʣ��һ�׸�����֪ͨ�����˳�
                music_close_engine();

                //δ�ҵ�����!
                gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);                

                //���۴�������룬����Ҫ���»ص�
                retval = RESULT_LISTMENU_MENU;
            }
            else
            {
                music_del_favorite_deal(g_browser_filepath.file_path.flist_location.file_num);
                //֪ͨ��������һ��
                music_play_next();

                //���»ص���һ���˵�
                retval = RESULT_MENU_PARENT;
            }
        }
        else
        {
            retval = RESULT_MENU_PARENT;
        }
        
    }
    else
    {
        retval = RESULT_MENU_PARENT;
    }
    
    return retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e del_favorite_setmenu(uint8 target_type)
 * \����ǰ��̨���ڲ��Ÿ������ղؼ���ɾ��
 * \param[in]  target_type �ղؼж�λ param1
 * \param[out]
 * \return   ���ز������,�Ƿ����ɹ�
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e del_favorite_setmenu(uint8 target_type)
{
    //uint16 cur_num, cur_total;
    uint32 cur_direnry, cur_cluster;
    app_result_e ret;
    app_result_e set_del_result;

    plist_location_t* locatptr = (plist_location_t*) &g_file_path_info.file_path.plist_location;

    cur_cluster = locatptr->cluster_no;
    cur_direnry = locatptr->dir_entry;

    music_get_filepath(&g_file_path_info);

    if ((cur_cluster != g_file_path_info.file_path.dirlocation.cluster_no) || (cur_direnry
            != g_file_path_info.file_path.dirlocation.dir_entry))
    {
        //����и裬��Ҫ���»�ȡ������ID3 TITLE
        fsel_browser_set_file(locatptr, target_type);
        get_music_id3_title(g_title_buf, sizeof(g_title_buf), locatptr->filename);
    }

    //��ֹ�ļ�������,���ļ���ת���ɲ����ļ���+ʡ�Ժ���ʽ
    com_ellipsis_to_longname(g_title_buf, com_name_buffer, 52);

    ret = del_favorite_dialog(target_type, com_name_buffer, 0);
    if (ret == RESULT_DIALOG_YES)
    {
        /* ȷ�� */
        set_del_result = del_favorite_nowplaying(target_type);
        if (set_del_result != RESULT_IGNORE)
        {
            
        }
        else
        {
            set_del_result = RESULT_MENU_PARENT;
        }
    }
    else
    {
        /* �����ȼ���Ϣ */
        set_del_result = ret;
    }
    return set_del_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e del_favorite_list(uint8 target_type, uint16 index)
 * \��ui_directoryĳһ�������ղؼ���ɾ��
 * \param[in]  target_type �ղؼж�λ param1
 * \param[in]  index ��ǰ�ļ���� param2
 * \param[out]
 * \return    ���ز������,�Ƿ����ɹ�
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e del_favorite_list(uint8 target_type, uint16 index)
{
    app_result_e del_result;
    uint8 ret;
    //uint16 item_total;
    bool check_ret;
    bool need_check_engine = FALSE;

    dir_brow_t get_brower_info_list;
    uint8 ext_buf_list[4];

    //dialog_com_data_t dialog_hint;

    //string_desc_t hint_str;

    //�Ƿ��̨����music
    engine_type_e engine_type = get_engine_type();

    get_brower_info_list.name_buf = ext_buf_list;
    get_brower_info_list.name_len = 4;

    if (engine_type == ENGINE_MUSIC)
    {
        music_get_filepath(&g_file_path_info);

        if (g_file_path_info.file_source == target_type)
        {
            need_check_engine = TRUE;
        }
    }

    ret = fsel_favorite_delete(target_type, index, NULL);

    check_ret = check_del_favorite_result(ret);

    fsel_browser_enter_dir(ROOT_DIR, 0, &get_brower_info_list);
    //֪ͨ��̨����ɾ���˵�ǰ���ŵ��ղؼе��ļ�
    if ((g_file_path_info.file_source == target_type) && (check_ret == TRUE))
    {
        music_del_favorite_deal(index);
    }

    if ((need_check_engine == TRUE) && (check_ret == TRUE))
    {
        if (get_brower_info_list.file_total == 0)
        {
            //ֻʣ��һ�׸�����֪ͨ�����˳�
            //δ�ҵ�����!
            music_close_engine();
            del_result = RESULT_DELETE_FILE;
        }
        else
        {
            if (g_file_path_info.file_path.flist_location.file_num == index)
            {
                //֪ͨ��������һ��
                music_play_next();
            }
            //���»ص���һ���˵�
            del_result = RESULT_DELETE_FILE;
        }
    }
    else
    {
        if (check_ret == FALSE)
        {
            del_result = RESULT_IGNORE;
        }
        else
        {
            del_result = RESULT_DELETE_FILE;
        }
    }
    return del_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e del_favorite_listmenu(uint8 target_type, uint16 active)
 * \��ui_directoryĳһ�������ղؼ���ɾ��
 * \param[in]  title_action
 * \param[out] ���ز������,�Ƿ����ɹ�
 * \return
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e del_favorite_listmenu(uint8 target_type, uint16 active)
{
    app_result_e dialog_result;
    app_result_e del_result;
    uint8 ext_buf[4];
    //uint8 file_name[12];

    brow_decode_t decode_param;
    file_brow_t file_list;

    decode_param.option = 1;
    decode_param.num = 1;
    decode_param.top = active;

    file_list.name_buf = com_name_buffer;
    file_list.name_len = sizeof(com_name_buffer);
    libc_memcpy(file_list.ext, ext_buf, sizeof(ext_buf));

    //��ȡ�ļ�����title
    fsel_browser_get_items(&decode_param, &file_list);

    //��ֹ�ļ�������,���ļ���ת���ɲ����ļ���+ʡ�Ժ���ʽ
    com_ellipsis_to_longname(com_name_buffer, com_name_buffer, 52);

    dialog_result = del_favorite_dialog(target_type, com_name_buffer, 0);

    if (dialog_result == RESULT_DIALOG_YES)
    {
        /* ȷ�� */
        del_result = del_favorite_list(target_type, active);
        if (del_result != RESULT_IGNORE)
        {
            del_favorite_dialog(target_type, com_name_buffer, 1);
        }
        else
        {
            del_result = RESULT_REDRAW;
        }
    }
    else
    {
        /* �����ȼ���Ϣorȡ�� */
        del_result = dialog_result;
    }
    return del_result;
}

/*! \endcond */
