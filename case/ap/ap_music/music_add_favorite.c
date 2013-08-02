/*******************************************************************************
 *                              US212A
 *                            Module: MUSIC_ADD_FAVORITE
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-11-9 16:17:43           1.0              build this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_playing.h"

#include "app_music_favorite.h"

const uint8 allsong_str[] =
{ "All Songs" };

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_ab_count(uint8 count)
 * \֪ͨ��̨��ӵ�ǰ���ŵ��ղؼ��ļ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_favorite
 * \note
 */
/*******************************************************************************/
bool music_add_favorite_deal(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_ADD_FAVORITE_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool ask_for_override_favorlist(void)
 * \��ѯ���ղؼ���Ŀ���������Ŀʱ�Ƿ񸲸�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 ����
 * \retval           0 ������
 * \ingroup      music_favorite
 * \note
 */
/*******************************************************************************/
static app_result_e _ask_for_override_favorlist(void)
{
    app_result_e result;
    dialog_com_data_t ask_dialog_data;
    string_desc_t ask_create_str;

    //�Ի����Ƿ񴴽������б�
    ask_create_str.data.id = S_IS_OVERRIDE_FAVLIST;
    ask_create_str.language = UNICODEID;
    //�Ի�������
    ask_dialog_data.dialog_type = DIALOG_BUTTON_INCLUDE;
    ask_dialog_data.icon_id = -1;
    //�ַ���
    ask_dialog_data.string_desc = &ask_create_str;

    //��������
    ask_dialog_data.button_type = BUTTON_YESNOCANCEL;
    /*! Ĭ�ϼ������ */
    ask_dialog_data.active_default = 0x01;

    result = gui_dialog(DIALOG_ASK, &ask_dialog_data);

    if (result == RESULT_DIALOG_NO)
    {
        result = RESULT_IGNORE;
    }

    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \��ȡID3��Ϣ ��ȡ�ļ����ֶ�
 * \param[in]  buffer ���id3��buffer param1
 * \param[in]  ext_name ���ļ��ĺ�׺�� param2
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_favorite
 * \note
 *******************************************************************************/
void get_music_id3_title(uint8 *buffer, uint8 len, uint8 *ext_name)
{
    id3_info_t id3_infor;
    id3_type_e cur_type;
    libc_memset(&id3_infor, 0, sizeof(id3_info_t));
    id3_infor.tit2_buffer = buffer;
    id3_infor.tit2_length = len;

    cur_type = play_scene_checktype(ext_name);
    get_id3_info(&id3_infor, NULL, cur_type);
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool check_add_favorite_result(uint8 ret)
 * \����ղؼ��ļ�����ʱ����ʾ
 * \����ӵ��ļ�����ʾ����Ӹ���Ŀ
 * \param[in] ret ����ղؼк�ķ���ֵ param1
 * \param[out]
 * \return    bool ��ӳɹ�/ʧ��
 * \ingroup      music_favorite
 * \note
 *******************************************************************************/

bool check_add_favorite_result(uint8 ret)
{
    //�Ի���
    bool result = TRUE;

    if (ret == FALSE)
    {
        ret = fsel_get_error();
        if (ret == FSEL_ERR_EXIST)
        {
            //����Ӹ���Ŀ
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_ALREADY_ADD);
        }
        result = FALSE;
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e add_favorite_nowplaying(uint8 target_type)
 * \����ǰ��̨���ڲ��ŵĸ�������ղؼ�
 * \param[in]  target_type �����ĸ��ղؼ�
 * \return
 * \ingroup     music_favorite
 * \note
 * \li   ���ڲ��ż����ղؼ�֮�󷵻ص��øú����˵�����һ���˵�
 * \li   ��ӵ����ļ������Ƿ���ӳɹ�
 *******************************************************************************/
app_result_e add_favorite_nowplaying(uint8 target_type)
{
    uint8 ret;
    bool check_result;
    flist_location_t favor_location;
    uint16 fav_file_total;
    app_result_e ui_result;

    void *plocation = (void *) &g_file_path_info.file_path;

    fav_file_total = fsel_favorite_get_total(target_type);
    if (fav_file_total == USERPL_FILE_MAX)
    {
        ui_result = _ask_for_override_favorlist();
        if (ui_result != RESULT_DIALOG_YES)
        {
            //���������Ŀ����ֹ��ǰ��Ӳ���
            return ui_result;
        }
    }

    fsel_change_fav_location(plocation, &favor_location, g_file_path_info.file_source, &g_id3_info);
    ret = fsel_favorite_add(&favor_location, target_type);
    check_result = check_add_favorite_result(ret);
    if (check_result == FALSE)
    {
        ui_result = RESULT_IGNORE;
    }
    else
    {
        if ((fav_file_total != USERPL_FILE_MAX) && (g_file_path_info.file_source == target_type))
        {
            music_add_favorite_deal();
        }
        ui_result = RESULT_MENU_PARENT;
    }

    return ui_result;

}
/******************************************************************************/
/*!
 * \par  Description:
 * \��ӵ����ļ����ղؼ�
 * \param[in]  plocation �ļ�·��
 * \param[in]  target_type �ղؼ�����
 * \param[in]  fav_file_total ��ǰ�ղؼ��ļ�����
 * \param[in]  list_no �ļ����
 * \return
 * \ingroup     music_favorite
 * \note
 * \li   ���ڲ��ż����ղؼ�֮�󷵻ص��øú����˵�����һ���˵�
 * \li   ��ӵ����ļ������Ƿ���ӳɹ�
 *******************************************************************************/
static app_result_e add_favorlist_single(plist_location_t * plocation, uint8 target_type, uint16 fav_file_total,
        uint16 list_no)
{
    uint8 ret = TRUE;
    app_result_e ui_result;
    flist_location_t flocation;

    //�����ļ����
    if (fav_file_total == USERPL_FILE_MAX)
    {
        ui_result = _ask_for_override_favorlist();
        if (ui_result != RESULT_DIALOG_YES)
        {
            //���������Ŀ����ֹ��ǰ��Ӳ���
            return ui_result;
        }
    }
    fsel_browser_select(list_no);
    fsel_browser_get_location(plocation, FSEL_TYPE_PLAYLIST);
    fsel_change_fav_location(plocation, &flocation, FSEL_TYPE_PLAYLIST, &g_id3_info);
    ret = fsel_favorite_add(&flocation, target_type);

    if (ret == TRUE)
    {
        if (g_file_path_info.file_source == target_type)
        {
            music_add_favorite_deal();
        }

        ui_result = RESULT_REDRAW;
    }
    else
    {
        ui_result = RESULT_ERROR;
    }

    return ui_result;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \��ui_directory()��ĳһ���ļ���ĳһ���ļ�������ղؼ�
 * \param[in]  plocation ��ǰ�ļ���·����Ϣ
 * \param[in]  target_type �����ĸ��ղؼ�
 * \param[in]  list_no  �ļ����
 * \param[out]
 * \return
 * \retval
 * \ingroup    music_favorite
 * \note
 * \li   ��ӵ����ļ������Ƿ���ӳɹ�
 * \li   ������֮���˳��˵��ؼ�������ui_directory
 * \li   �������ļ�����Ǵ�1��ʼ����Ŀ¼����һ��Ϊ0.����ļ����Ϊ0����Ŀ¼������
 Ϊ0�����ʾ���������ģʽ������all song/artist��ĳ�����ͱ�������ղؼ�
 *******************************************************************************/
app_result_e add_favorite_list(plist_location_t * plocation, uint8 target_type, uint16 list_no)
{
    app_result_e ui_result;
    bool check_result;
    //uint8 ret = TRUE;
    //�Ƿ���������ղؼ�
    uint8 batch_mode;
    uint16 fav_file_total;
    dir_brow_t browser;

    //�����ǰ�б�Ľṹ���ʼ��
    browser.name_buf = NULL;
    browser.name_len = 0;

    fsel_browser_enter_dir(CUR_DIR, 0, &browser);

    if (browser.dir_total != 0)
    {
        //��ǰ����һ��
        fsel_browser_enter_dir(SON_DIR, list_no, &browser);
        batch_mode = TRUE;
    }
    else if (list_no == 0)
    {
        batch_mode = TRUE;
    }
    else
    {
        batch_mode = FALSE;
    }

    fav_file_total = fsel_favorite_get_total(target_type);

    //�������
    if (batch_mode == TRUE)
    {
        ui_result = add_favorlist_all(plocation, target_type, fav_file_total, &browser);
    }
    else
    {
        ui_result = add_favorlist_single(plocation, target_type, fav_file_total, list_no);
    }
    if (ui_result == RESULT_ERROR)
    {
        check_result = check_add_favorite_result(FALSE);

        if (check_result == FALSE)
        {
            ui_result = RESULT_IGNORE;
        }
        else
        {
            ui_result = RESULT_REDRAW;
        }
    }

    return ui_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ����ղؼ�dialog�ؼ�ʵ�֣���Ҫ����ַ���
 * \param[in]   target_type Ҫ��ӵ��ղؼ�����
 * \param[in]   file_name   ת������ļ���(ID3)
 * \param[in]   dialog_type ���ǰ��ʾ 0 ��Ӻ���ʾ 1
 * \return      app_result_e the result
 * \retval
 * \ingroup     music_favorite
 * \note
 * \li   ���ַ�����֯��"��[%s](file_name)�����[%s](target_type)"��"�ѽ�[%s]�����
 [%s]"��ʽ,��������%s����Դ�ַ�����Ҫʹ��com_string_format()������֯

 *******************************************************************************/
app_result_e add_favorite_dialog(fsel_type_e target_type, uint8 *file_name, uint8 dialog_type)
{
    app_result_e ret;

    dialog_com_data_t dialog;
    //Ŀ���ַ���
    string_desc_t dest_data;
    //Դ�ַ���
    string_desc_t source_data[2];

    //Դ�ַ���0��ʼ��
    if (*(uint16*) file_name == 0xfeff)
    {
        source_data[0].language = UNICODEDATA;
    }
    else if ((file_name[0] == 0xef) && (file_name[1] == 0xbb) && (file_name[2] == 0xbf))
    {
        source_data[0].language = UTF_8DATA;
    }
    else
    {
        source_data[0].language = (int8) g_setting_comval.g_comval.language_id;
    }

    source_data[0].data.str = file_name;

    source_data[0].length = 32;

    //Դ�ַ���1��ʼ��
    source_data[1].language = UNICODEID;

    if (target_type == FSEL_TYPE_LISTFAVOR1)
    {
        source_data[1].data.id = S_NOW_FAVORITE1;
    }
    else if (target_type == FSEL_TYPE_LISTFAVOR2)
    {
        source_data[1].data.id = S_NOW_FAVORITE2;
    }
    else
    {
        source_data[1].data.id = S_NOW_FAVORITE3;
    }

    //Ŀ���ַ�����ʼ��
    //dest_data.data.str = g_insert_dialog;
    dest_data.data.str = g_temp_buffer_p;

    if (dialog_type == 0)
    {
        //���뵽�ĸ��ղؼ�
        dest_data.argv = S_ADD_FAVORITE1;
    }
    else
    {
        //���뵽�ĸ��ղؼ�
        dest_data.argv = S_ALREADY_ADD_FAVORITE;

    }

    //dest_data.language = (int8) g_setting_comval.g_comval.language_id;
    dest_data.language = UNICODEDATA;

    //dest_data.length = sizeof(g_insert_dialog);
    dest_data.length = 128;

    //��֯�Ի����ַ���
    com_string_format(&dest_data, source_data);

    //dialog�ؼ�������ʼ��
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
 * \app_result_e add_favorite_listmenu(plist_location_t* location, uint8 target_type, uint16 active)
 * \���б�˵���ѡ����벥���б��ղؼУ�
 * \param[in]  location ��ǰ�ļ�/�б��·����Ϣ param1
 * \param[in]  target_type ���뵽�Ǹ��ղؼ� param2
 * \param[in]  active ��ǰ�Ļ�� param3
 * \param[out]
 * \return    ���ز������,�Ƿ����ɹ�
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e add_favorite_listmenu(plist_location_t* location, uint8 target_type, uint16 active)
{
    app_result_e dialog_result;
    app_result_e add_result;
    uint8 ext_buf[4];

    brow_decode_t decode_param;
    file_brow_t file_list;
    //�жϲ��
    dir_brow_t browser;

    decode_param.option = 1;
    decode_param.num = 1;

    browser.name_buf = NULL;
    browser.name_len = 0;
    fsel_browser_enter_dir(CUR_DIR, 0, &browser);

    //active = dir_control.list_no - menu_cnt ���Ϊ0��ʾ��ǰΪ
    //ui_directoryǶ�ײ˵���
    if ((active == 0) && (browser.dir_total != 0))
    {
        fsel_browser_enter_dir(PARENT_DIR, 0, &g_dir_browser);
        decode_param.top = last_dir_control.list_no;
    }
    else
    {
        decode_param.top = active;
    }

    file_list.name_buf = com_name_buffer;
    libc_memcpy(file_list.ext, ext_buf, sizeof(ext_buf));
    file_list.name_len = sizeof(com_name_buffer);
    //��ȡ�ļ�����title
    fsel_browser_get_items(&decode_param, &file_list);
    if ((active == 0) && (browser.dir_total == 0))
    {
        //g_song_filename ����Ϊallsong
        libc_memcpy(com_name_buffer, allsong_str, sizeof(allsong_str));
    }

    //��ֹ�ļ�������,���ļ���ת���ɲ����ļ���+ʡ�Ժ���ʽ
    com_ellipsis_to_longname(com_name_buffer, com_name_buffer, 52);

    dialog_result = add_favorite_dialog(target_type, com_name_buffer, 0);
    if (dialog_result == RESULT_DIALOG_YES)
    {
        /* ȷ�� */
        add_result = add_favorite_list(location, target_type, decode_param.top);
        if (add_result == RESULT_REDRAW)
        {
            //��ʾ
            add_favorite_dialog(target_type, com_name_buffer, 1);
        }
        else if (add_result == RESULT_IGNORE)
        {
            add_result = RESULT_REDRAW;
        }
        else
        {
            ;//nothing for QAC
        }
    }

    else
    {
        /* �����ȼ���Ϣ */
        add_result = dialog_result;
    }
    if ((active == 0) && (browser.dir_total != 0))
    {
        fsel_browser_enter_dir(SON_DIR, last_dir_control.list_no, &g_dir_browser);
    }

    return add_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e add_favorite_setmenu(uint8 target_type)
 * \ �����ò˵���ѡ����벥���б�
 * \param[in]  target_type ��ӵ��ĸ��ղؼ� param1
 * \param[out]
 * \return    ���ز������,�Ƿ����ɹ�
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e add_favorite_setmenu(uint8 target_type)
{
    //uint16 cur_num, cur_total;
    uint32 cur_direnry, cur_cluster;
    //uint8 ret = 0;
    app_result_e dialog_result;
    app_result_e add_result;

    plist_location_t* locatptr = (plist_location_t*) &g_file_path_info.file_path.plist_location;

    cur_cluster = locatptr->cluster_no;
    cur_direnry = locatptr->dir_entry;

    music_get_filepath(&g_file_path_info);

    //�ж��Ƿ��и�
    if ((cur_cluster != g_file_path_info.file_path.dirlocation.cluster_no) || (cur_direnry
            != g_file_path_info.file_path.dirlocation.dir_entry))
    {
        //����и裬��Ҫ���»�ȡ������ID3 TITLE
        fsel_browser_set_file(&g_file_path_info.file_path.plist_location, target_type);
        get_music_id3_title(g_title_buf, sizeof(g_title_buf), locatptr->filename);
    }

    //��ֹ�ļ�������,���ļ���ת���ɲ����ļ���+ʡ�Ժ���ʽ
    com_ellipsis_to_longname(g_title_buf, com_name_buffer, 52);

    dialog_result = add_favorite_dialog(target_type, com_name_buffer, 0);
    if (dialog_result == RESULT_DIALOG_YES)
    {
        /* ȷ�� */
        add_result = add_favorite_nowplaying(target_type);
        if (add_result != RESULT_IGNORE)
        {
            //��ʾ
            add_favorite_dialog(target_type, com_name_buffer, 1);
        }
        add_result = RESULT_MENU_PARENT;

    }
    else
    {
        /* �����ȼ���Ϣ */
        add_result = dialog_result;
    }
    return add_result;
}

