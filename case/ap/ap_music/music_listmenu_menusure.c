/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-26          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_menu_config.h"

const uint8 audible_ext_name[][4] =
{ "AAX", "AA " };

const id3_type_e audible_id3_type[] =
{
    //AAX
    ID3_TYPE_AAX,
    //AA
    ID3_TYPE_AA
};
//bank data for audible resume play
audible_resume_info_t audible_resume_data _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_allsong_sure(void* param)
 * \���и���ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_allsong_sure(void* param)
{
    //���ص�file list
    return RESULT_ENTER_TITLE_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_artist_sure(void* param)
 * \����ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_artist_sure(void* param)
{
    //���ص�file list
    return RESULT_ENTER_ARTIST_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_album_sure(void* param)
 * \ר��ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_album_sure(void* param)
{
    //���ص�file list
    return RESULT_ENTER_ALBUM_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_genre_sure(void)
 * \����ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_genre_sure(void* param)
{
    //���ص�file list
    return RESULT_ENTER_GENRE_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 ����ר��ͼƬ�б�ȷ��ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_ENTER_ALBUM_THUMBNAIL
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/

app_result_e list_scene_albumlist_sure(void *param)
{
    return RESULT_ENTER_ALBUM_THUMBNAIL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool list_scene_favor_total(file_path_info_t* select_path fsel_type_e favor_from)
 * \����ղؼ��Ƿ�����ļ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       bool the result
 * \retval          1 success
 * \retval          0 fail
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
bool list_scene_favor_total(file_path_info_t* select_path, fsel_type_e favor_from)
{
    bool retval;

    uint8 cur_sel_save;
    uint16 cur_list_save;

    dir_brow_t browser_plist;

    //����ȡ�ļ���
    browser_plist.name_len = 0;

    cur_sel_save = select_path->file_source;

    cur_list_save = select_path->file_path.plist_location.list_type;

    //�˳��ļ�ѡ����
    fsel_exit();

    //�ļ�ѡ������ʼ��
    select_path->file_source = favor_from;

    retval = music_file_init(select_path);

    if (retval == TRUE)
    {
        fsel_browser_set_location(&select_path->file_path.flist_location, favor_from);

        //��ȡ��ǰ�ղؼ��ļ�����
        fsel_browser_enter_dir(CUR_DIR, 0, &browser_plist);

        if (browser_plist.file_total == 0)
        {
            fsel_exit();

            select_path->file_source = cur_sel_save;
            select_path->file_path.plist_location.list_type = cur_list_save;

            music_file_init(select_path);

            //��ʾδ�����ļ�
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);
            retval = FALSE;
        }
    }
    return retval;
}
/******************************************************************************/
/*!
 * \par  Description:
 �����ղؼ�Ҷ�Ӳ˵��ص�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor_sure(void *param)
{
    //����filelist
    return RESULT_ENTER_FAVOR_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor1_sure(void)
 * \�ղؼ�1����ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor1_sure(void* param)
{
    bool bret;
    //check�ղؼ��Ƿ�����ļ�
    bret = list_scene_favor_total(&g_browser_filepath, FSEL_TYPE_LISTFAVOR1);
    if (bret == TRUE)
    {
        //���ص�file list
        return RESULT_ENTER_FAVOR1_LIST;
    }
    else
    {
        //��ǰ�˵�
        return RESULT_REDRAW;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor2_sure(void)
 * \�ղؼ�2����ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor2_sure(void* param)
{
    bool bret;
    //check�ղؼ��Ƿ�����ļ�
    bret = list_scene_favor_total(&g_browser_filepath, FSEL_TYPE_LISTFAVOR2);
    if (bret == TRUE)
    {
        //���ص�file list
        return RESULT_ENTER_FAVOR2_LIST;
    }
    else
    {
        //��ǰ�˵�
        return RESULT_REDRAW;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor3_sure(void)
 * \�ղؼ�3����ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor3_sure(void* param)
{
    bool bret;
    //check�ղؼ��Ƿ�����ļ�
    bret = list_scene_favor_total(&g_browser_filepath, FSEL_TYPE_LISTFAVOR3);
    if (bret == TRUE)
    {
        //���ص�file list
        return RESULT_ENTER_FAVOR3_LIST;
    }
    else
    {
        //��ǰ�˵�
        return RESULT_REDRAW;
    }
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
 * \ingroup     music_add_favorite.c
 * \note
 *******************************************************************************/
void get_audible_id3_title(uint8 *buffer, uint8 *ext_name)
{
    id3_info_t id3_infor;
    id3_type_e cur_type;
    uint8 i;
    libc_memset(&id3_infor, 0, sizeof(id3_info_t));
    id3_infor.tit2_buffer = buffer;
    id3_infor.tit2_length = 32;

    //cur_type = check_music_type(ext_name);
    for (i = 0; i < 2; i++)
    {
        if (libc_strncmp(&audible_ext_name[i], ext_name, 4) == 0)
        {
            break;
        }
    }
    if (i < 2)
    {
        cur_type = audible_id3_type[i];
        get_id3_info(&id3_infor, NULL, cur_type);
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 ��ȡ��ǰ��audible���Ŷϵ�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      music_listmenu_menusure.c
 * \note
 * \li   �ú���ֻ�ǻ�ȡ��ǰaudible�Ĳ��Ŷϵ㣬�����洢�ϵ㡣�ϵ�Ĵ洢��music engine���
         ����audible_resume_dataΪbank data������ڸ��˵���Ҷ�Ӳ˵�����Ҫ��ȡ
 * \li   ����Զϵ�ı��������������:��audible�л���music������music engine�˳�����������
         ����VRAM�Ķ�д����
 */
/*******************************************************************************/
void load_breakinfo(void)
{
    if ((g_audible_now == FALSE) && (g_music_config.audible_bk_flag == TRUE))
    {
        //��ȡaudible�Ķϵ���Ϣ
        sys_vm_read(&audible_resume_data, VM_AP_MUSIC_AUDBK, sizeof(audible_resume_info_t));
        if (audible_resume_data.magic != MAGIC_AUDBK)
        {
            //�ϵ���Ч
            g_music_config.audible_bk_flag = FALSE;
        }
    }
    else
    {
        if (g_audible_now == TRUE)
        {
            //��ȡ�ϵ���Ϣ
            music_get_bkinfo(&g_audible_bkinfo);

            //��ȡ��ǰ�����ļ���·����Ϣ
            music_get_filepath(&g_file_path_info);

            libc_memcpy(&(audible_resume_data.bk_infor), &g_audible_bkinfo, sizeof(g_audible_bkinfo));
            libc_memcpy(&(audible_resume_data.locat_info), &g_file_path_info, sizeof(file_path_info_t));
        }
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_audible_sure(void *param)
 * \�����鼮ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_audible_sure(void *param)
{
    file_path_info_t temp_file_path;

    plist_location_t* locat_ptr;

    //��ȡ��ǰ�����ļ���·����Ϣ
    locat_ptr = &temp_file_path.file_path.plist_location;

    load_breakinfo();

    //�Ƿ��������Ź�audible����
    if (g_music_config.audible_bk_flag == TRUE)
    {
        //���ö�̬��ڲ˵� ����/�鼮/����
        string_desc_t aud_resume_desc;//Ŀ���ַ����ṹ
        string_desc_t aud_resume_name;//Դ�ַ����ṹ
        //���ز���
        menu_title_action_t* retptr = (menu_title_action_t*) param;

        libc_memcpy(&temp_file_path, &audible_resume_data.locat_info, sizeof(file_path_info_t));

        if (temp_file_path.file_path.plist_location.disk != locat_ptr->disk)
        {
            //�����̷���һ�£���ֱ�ӽ����鼮�������б�
            return RESULT_MENU_SON;
        }

        //��λ���ļ�
        if (fsel_browser_set_file(locat_ptr, temp_file_path.file_source) == TRUE)
        {
            get_audible_id3_title(g_song_filename, ((plist_location_t*) locat_ptr)->filename);
        }
        else
        {
            //�ļ��޷��ҵ���֤���öϵ���Ч ��ϵ��־
            g_music_config.audible_bk_flag = FALSE;
            return RESULT_MENU_SON;
        }

        //����һ����ڲ˵�styid
        retptr->menu_entry = SLEEP_TIMER_SETMENU;

        //aud_resume_desc.data.str = g_insert_dialog;
        //aud_resume_desc.length = 80;
        aud_resume_desc.data.str = g_temp_buffer_p;
        aud_resume_desc.length = 128;       
        aud_resume_desc.argv = S_PLAY_RESUME;
        //�˵�����֯���ַ�����ǰֻ֧��������ʾ
        aud_resume_desc.language = (int8) g_setting_comval.g_comval.language_id;
        //aud_resume_desc.language = UNICODEDATA;
        aud_resume_name.data.str = g_song_filename;
        aud_resume_name.length = 32;
        if (*(uint16*) g_song_filename == 0xfeff)
        {
            aud_resume_name.language = UNICODEDATA;
        }
        else if ((g_song_filename[0] == 0xef) && (g_song_filename[1] == 0xbb) && (g_song_filename[2] == 0xbf))
        {
            aud_resume_name.language = UTF_8DATA;
        }
        else
        {
            aud_resume_name.language = (int8) g_setting_comval.g_comval.language_id;
        }

        com_string_format(&aud_resume_desc, &aud_resume_name);

        retptr->menu_entry = LISTMENU_AUDIBLE_RESUME;//��̬�˵����ID
        retptr->str_id = -1;//�������������ַ���ID��-1��ʾ����Ĭ�ϼ�����
        //retptr->ret_str = g_insert_dialog;//��̬�˵����ַ���ָ��
        retptr->ret_str = g_temp_buffer_p;//��̬�˵����ַ���ָ��
        retptr->style_id = -1;//�����˵�style_id�����Ը���ui_menulist��������Ĭ��style_id��-1��ʾ����Ĭ��style_id
        retptr->source_id = S_PLAY_RESUME;
    }
    //������һ��
    return RESULT_MENU_SON;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_audresume_sure(void *param)
 * \audible����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_audresume_sure(void *param)
{
    bool ret_val = TRUE;
    fsel_type_e cur_sel_save = g_file_path_info.file_source;

    //��ȡaudible�Ķϵ���Ϣ
    //sys_vm_read(&audible_resume_data, VM_AP_MUSIC_AUDBK, sizeof(audible_resume_info_t));
    load_breakinfo();

    libc_memcpy(&g_file_path_info, &audible_resume_data.locat_info, sizeof(file_path_info_t));

    //�ϵ���Ϣ
    libc_memcpy(&g_audible_bkinfo, &(audible_resume_data.bk_infor), sizeof(g_audible_bkinfo));

    if (g_file_path_info.file_source != cur_sel_save)
    {
        fsel_exit();
        ret_val = music_file_init(&g_file_path_info);
    }

    if (ret_val == TRUE)
    {
        //���ص�audible����
        return RESULT_AUDIBLE_RESUME;
    }
    else
    {
        //��ʼ���ļ�ѡ����ʧ��
        return RESULT_LISTMENU_MENU;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_books_sure(void *param)
 * \audible���鼮�б�ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_books_sure(void *param)
{
    //���ص�file list
    return RESULT_ENTER_BOOKS_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_authors_sure(void)
 * \audible�������б�ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_authors_sure(void *param)
{
    //���ص�file list
    return RESULT_ENTER_AUTHORS_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_create_sure(void)
 * \���������б�ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_create_sure(void *param)
{
    //���봴�������б�app
    return RESULT_CREATE_LIST_APP;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool list_scene_check_change(uint8 disk_flag)
 * \��������Ŀ¼ȷ�Ϻ���
 * \param[in]    disk_flag ��ǰ�̷�  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       TRUE ��Ҫ�������Ҹ����ɹ�
 * \retval       FALSE ����Ҫ������
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_check_change(uint8 disk_flag)
{
    uint8 cur_disk;
    //fsel_error_e get_erro;
    bool retval = TRUE;

    music_get_filepath(&g_file_path_info);
    cur_disk = g_file_path_info.file_path.dirlocation.disk;
    if (cur_disk != disk_flag)
    {
        //�Ƿ��̨����music
        engine_type_e engine_type = get_engine_type();

        if (engine_type == ENGINE_MUSIC)
        {
            sys_vm_write(&g_setting_comval, VM_AP_SETTING);
            //����ر�
            music_close_engine();
        }

        //�˳��ļ�ѡ����
        fsel_exit();

        g_file_path_info.file_path.dirlocation.disk = disk_flag;
        g_browser_filepath.file_path.plist_location.disk = disk_flag;
        retval = music_file_init(&g_file_path_info);
        //�л�ʧ��,�ص�ԭ��
        if (retval == FALSE)
        {
            //�˳��ļ�ѡ����
            fsel_exit();
            //�л�������
            g_file_path_info.file_path.dirlocation.disk = cur_disk;
            g_browser_filepath.file_path.plist_location.disk = cur_disk;
            music_file_init(&g_file_path_info);
        }
    }

    if (retval == TRUE)
    {
        return RESULT_ENTER_TITLE_LIST;
    }
    else
    {
#if 0
        get_erro = fsel_get_error();
        //��ʾ
        if ((get_erro == FSEL_ERR_STG) || (get_erro == FSEL_ERR_FS))
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        }
        else if(get_erro == FSEL_ERR_LIST_NOEXIST)
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_PLAYLIST);
        }
#endif
        return RESULT_REDRAW;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_maindisk_sure(void)
 * \��������Ŀ¼ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_maindisk_sure(void *param)
{
    app_result_e change_ret;
    change_ret = list_scene_check_change(DISK_C);
    return change_ret;

}

static app_result_e _ask_for_change_disk(void)
{
    app_result_e result;
    dialog_com_data_t ask_dialog_data;
    string_desc_t ask_create_str;

    uint8 old_disk = g_browser_filepath.file_path.plist_location.disk;

    //Ϊ����Ӧ���γ���Ϣ������ʱ�����̷�
    g_browser_filepath.file_path.plist_location.disk = DISK_H;

    //�Ի����Ƿ񴴽������б�
    ask_create_str.data.id = S_IS_CREATE_PLAYLIST;
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

    //�ָ�֮ǰ���̷�
    g_browser_filepath.file_path.plist_location.disk = old_disk;

    return result;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_carddisk_sure(void)
 * \���뿨��Ŀ¼ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_carddisk_sure(void *param)
{
    //bool ret;
    app_result_e result;

    //���ȼ�鿨�Ƿ����
    if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
    {
        //card��û�и��²����б�
        if (get_card_plist() == FALSE)
        {
            //ѯ���Ƿ񴴽������б�
            result = _ask_for_change_disk();
            if (result == RESULT_DIALOG_YES)
            {
                //ȷ�ϵ�������Ժ��ٴ��������б�
                //ֻ�����̷��������³�ʼ���ļ�ѡ����
                //�Դ��ݸ�plist����
                g_browser_filepath.file_path.plist_location.disk = DISK_H;
                g_file_path_info.file_path.plist_location.disk = DISK_H;
                result = RESULT_CREATE_LIST_APP;
            }
            else if (result == RESULT_DIALOG_NO)
            {
                result = list_scene_check_change(DISK_H);
            }
            else
            {
                ;//nothing �����ȼ���Ϣ
            }
        }
        else
        {
            result = list_scene_check_change(DISK_H);
        }
    }
    else
    {
        //��ʾ��
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
        //���½���menu�˵�
        result = RESULT_CARD_OUT_ERROR;
    }
    return result;
}


