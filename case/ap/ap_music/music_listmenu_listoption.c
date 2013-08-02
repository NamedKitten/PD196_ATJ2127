/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-26          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_favorite.h"

#include "app_music_menu_config.h"

//��ڲ˵���
const uint8 entry_id1[] =
{
    OPTION_FAVORLIST1_NORMAL,
    OPTION_FAVORLIST1_MPLAYING,
    OPTION_FAVORLIST1_MPAUSE,
    OPTION_FAVORLIST1_RPLAYING,
    OPTION_FAVORLIST1_RPAUSE
};

const uint8 entry_id2[] =
{
    OPTION_FAVORLIST2_NORMAL,
    OPTION_FAVORLIST2_MPLAYING,
    OPTION_FAVORLIST2_MPAUSE,
    OPTION_FAVORLIST2_RPLAYING,
    OPTION_FAVORLIST2_RPAUSE
};

const uint8 entry_id3[] =
{
    OPTION_FAVORLIST3_NORMAL,
    OPTION_FAVORLIST3_MPLAYING,
    OPTION_FAVORLIST3_MPAUSE,
    OPTION_FAVORLIST3_RPLAYING,
    OPTION_FAVORLIST3_RPAUSE
};

/******************************************************************************/
/*!
 * \par  Description:
 *	  music ѡ����Ӧ���б���������
 * \param[in]   location �ļ�·��
 * \param[in]   list_no  ��������� 
 * \param[out]  none
 * \return      app_result_e
 * \retval
 * \ingroup     music_ui
 * \note
 * \li   ������С�ڵ���0����ʾΪui_direcotry()�ؼ�Ƕ�ײ˵������allsong�����и���
 *******************************************************************************/
static app_result_e _music_listmenu_select_file(file_path_info_t* location, uint16 list_no)
{
    bool ret_val; 

    app_result_e ui_result;  
    
    plist_location_t* plist_locatp;

    plist_locatp = &(location->file_path.plist_location);

    //ѡ����Ӧ�б���ļ�
    ret_val = fsel_browser_select(list_no);

    if(ret_val == TRUE)
    {
        ret_val = fsel_browser_get_location(plist_locatp, location->file_source);
        if(ret_val == TRUE)
        {
            ui_result = RESULT_PLAYING_START;
        }
        else
        {
            //��ʾδ�����ļ�
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);        
            ui_result = RESULT_REDRAW;
        }
    }
    else
    {
        //��ʾδ�����ļ�
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);
        ui_result = RESULT_REDRAW;        
    }

    return ui_result;
    
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  music ѡ����Ӧ���б�����
 * \param[in]   location �ļ�·��
 * \param[in]   active   ��������� 
 * \param[out]  none
 * \return      app_result_e
 * \retval
 * \ingroup     music_ui
 * \note
 * \li   ������С�ڵ���0����ʾΪui_direcotry()�ؼ�Ƕ�ײ˵������allsong�����и���
 *******************************************************************************/
static app_result_e _music_listmenu_play_file(file_path_info_t* location, uint16 active)
{   
    //�жϲ��
    dir_brow_t browser;


    browser.name_buf = NULL;
    browser.name_len = 0;
        
    fsel_browser_enter_dir(CUR_DIR, 0, &browser); 

    if((active == 0) && (browser.dir_total != 0))
    {
        //ѡ��������ͬ���͵ı����
        active = 0xffff;
    }
    else
    {
        if(browser.dir_total != 0)
        {
             //��ǰ����һ��    
        }
        else
        {
            if (active == 0)
            {
                active = 1;
            }          
        }
    }
    
    return _music_listmenu_select_file(location, active);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e playlist_option_result_proc(void * location, app_result_e check_type, uint8 active)
 *	  music playlist�Ľ��������
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_ui
 * \note
 * \philips�����б�㼶Ϊ:
 * \All song һ��
 * \Artist:  artist + album + all song
 * \album��  album + all song
 * \genre:   genre + album + all song
 *******************************************************************************/

app_result_e playlist_option_result_proc(file_path_info_t* location, app_result_e check_type, uint16 active)
{
    app_result_e result;

    plist_location_t* plist_locatp;

    plist_locatp = &(location->file_path.plist_location);
    switch (check_type)
    {
        //����
        case RESULT_PLAYING_START:
        result = _music_listmenu_play_file(location, active);
        break;

        case RESULT_ADD_FAVOR1:
        //����ղؼ�1
        result = add_favorite_listmenu(plist_locatp, FSEL_TYPE_LISTFAVOR1, active);
        break;
        case RESULT_ADD_FAVOR2:
        //����ղؼ�2
        result = add_favorite_listmenu(plist_locatp, FSEL_TYPE_LISTFAVOR2, active);
        break;
        case RESULT_ADD_FAVOR3:
        //����ղؼ�3
        result = add_favorite_listmenu(plist_locatp, FSEL_TYPE_LISTFAVOR3, active);
        break;
        default:
        result = check_type;
        break;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  �ղؼ��б�1,2,3 gui_directory()�ص������ķ���ֵ������
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_ui
 * \note
 *******************************************************************************/
app_result_e favorlist_option_result_proc(uint8 file_source, app_result_e check_type, uint16 index)
{
    app_result_e result = RESULT_IGNORE;
    switch (check_type)
    {
        case RESULT_PLAYING_START:
        //����ĳ���ղؼ��б�ĳ���ļ�
        //fsel_get_location((void *) &g_browser_filepath.file_path.flist_location, file_source);
        //g_browser_filepath.file_source = file_source;
        //cpy������ʹ�õ�ȫ�ֱ�����
        //libc_memcpy(&g_file_path_info, &g_browser_filepath, sizeof(file_path_info_t));
        fsel_browser_select(index);
        fsel_browser_get_location(&g_browser_filepath.file_path.flist_location, file_source);
        result = check_type;
        break;
        case RESULT_DEL_FAVOR:
        //ɾ���ղؼ��ļ�
        result = del_favorite_listmenu(file_source, index);
        break;
        case RESULT_CLR_FAVOR:
        //����ղؼ��ļ�
        result = clr_favorite_menu(file_source);
        break;
        default:
        result = check_type;
        break;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_list_option(void)
 * \�б��option����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_list_option(void)
{
    menu_com_data_t menu;
    app_result_e result_value;
    engine_type_e engine_type;
    engine_state_e engine_state;
    //for temp complie
    //bool sound_on = TRUE;

    menu.app_id = APP_ID_MUSIC;
    menu.menu_func = NULL;
    //�����Ƿ����/��music or fm
    engine_type = get_engine_type();
    engine_state = get_engine_state();
    if (engine_type == ENGINE_MUSIC)
    {
        //���ڲ���
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //������ڲ˵�m playnow & play & add to favorite
            menu.menu_entry = OPTION_PLIST_MPLAING;
        }
        else
        {
            //������ڲ˵�n lastplayed & paly & add to favorite
            menu.menu_entry = OPTION_PLIST_MPAUSE;
        }
    }
    else if (engine_type == ENGINE_RADIO)
    {
        //�ж��Ƿ���
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //������ڲ˵�j playnow & play & add to favorite
            menu.menu_entry = OPTION_PLIST_RPLAYING;
        }
        else
        {
            //������ڲ˵�k lastplayed & paly & add to favorite
            menu.menu_entry = OPTION_PLIST_RPAUSE;
        }
    }
    else
    {
        //������ڲ˵� i paly & add to favorite
        menu.menu_entry = OPTION_PLIST_NORMAL;
    }
    //�Ӹ��˵�����
    menu.browse_mode = 0;
    menu.path_id = MUSIC_PATH_ID_LIST_PLAYLIST;
    menu.menu_func = NULL; //��ʲô��
    result_value = gui_menulist(MENULIST_OPTION, &menu);
    return result_value;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favorlist_option(void)
 * \�б��option����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favorlist_check(const uint8* entryid_buf)
{
    menu_com_data_t menu;
    app_result_e result_value;
    engine_type_e engine_type;
    engine_state_e engine_state;

    //for temp complie

    menu.app_id = APP_ID_MUSIC;
    menu.menu_func = NULL;

    //�����Ƿ����/��music or fm
    engine_type = get_engine_type();
    engine_state = get_engine_state();

    if (engine_type == ENGINE_MUSIC)
    {
        //���ڲ���
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //������ڲ˵�entryid[1] playnow & play & del form favorite & clear favorite
            menu.menu_entry = entryid_buf[1];
        }
        else
        {
            //������ڲ˵�entryid[2] lastplayed & paly & del form favorite & clear favorite
            menu.menu_entry = entryid_buf[2];
        }
    }
    else if (engine_type == ENGINE_RADIO)
    {
        //�ж��Ƿ���
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //������ڲ˵�entryid[3] playnow & play & del form favorite & clear favorite
            menu.menu_entry = entryid_buf[3];
        }
        else
        {
            //������ڲ˵�entryid[4] lastplayed & paly & del form favorite & clear favorite
            menu.menu_entry = entryid_buf[4];
        }
    }
    else
    {
        //������ڲ˵� entryid[0] paly & & del form favorite & clear favorite
        menu.menu_entry = entryid_buf[0];
    }
    //�Ӹ��˵�����
    menu.browse_mode = 0;
    menu.path_id = MUSIC_PATH_ID_LIST_FAVORLIST;
    menu.menu_func = NULL; //��ʲô��
    result_value = gui_menulist(MENULIST_OPTION, &menu);
    return result_value;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favorlist1_option(void)
 * \�ղؼ�1�б��option����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favorlist1_option(void)
{
    return list_scene_favorlist_check(entry_id1);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favorlist2_option(void)
 * \�ղؼ�2�б��option����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favorlist2_option(void)
{
    return list_scene_favorlist_check(entry_id2);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favorlist3_option(void)
 * \�ղؼ�3�б��option����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favorlist3_option(void)
{
    return list_scene_favorlist_check(entry_id3);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_m3ulist_option(void)
 * \m3u�б��option����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_m3ulist_option(void)
{
    menu_com_data_t menu;
    app_result_e result_value;
    engine_type_e engine_type;
    engine_state_e engine_state;

    menu.app_id = APP_ID_MUSIC;
    menu.menu_func = NULL;
    //�����Ƿ����/��music or fm
    engine_type = get_engine_type();
    engine_state = get_engine_state();
    if (engine_type == ENGINE_MUSIC)
    {
        //���ڲ���
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //������ڲ˵�m playnow & play 
            menu.menu_entry = OPTION_M3ULIST_MPLAYING;
        }
        else
        {
            //������ڲ˵�n lastplayed & paly 
            menu.menu_entry = OPTION_M3ULIST_MPAUSE;
        }
    }
    else if (engine_type == ENGINE_RADIO)
    {
        //�ж��Ƿ���
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //������ڲ˵�j playnow & play 
            menu.menu_entry = OPTION_M3ULIST_RPLAYING;
        }
        else
        {
            //������ڲ˵�k lastplayed & paly
            menu.menu_entry = OPTION_M3ULIST_RPAUSE;
        }
    }
    else
    {
        //������ڲ˵� i paly 
        menu.menu_entry = OPTION_M3ULIST_NORMAL;
    }
    //�Ӹ��˵�����
    menu.browse_mode = 0;
    menu.path_id = MUSIC_PATH_ID_LIST_M3ULIST;
    menu.menu_func = NULL; //��ʲô��
    result_value = gui_menulist(MENULIST_OPTION, &menu);
    return result_value;            
}
/******************************************************************************/
/*!
 * \par  Description:
 * \m3u�б��option�ص������������
 * \param[in]    location �ļ�·��
 * \param[in]    check_type Ҷ�Ӳ˵�����ֵ 
 * \param[in]    active     ���������
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e m3ulist_option_result_proc(file_path_info_t* location, app_result_e check_type, uint16 active)
{
    app_result_e result;     
    int16 real_active = (int16)active;

    plist_location_t* plist_locatp;

    plist_locatp = &(location->file_path.plist_location);

    if(check_type == RESULT_PLAYING_START)
    {   
        if(real_active > 0)
        {
            //ͳ�Ƶ�ǰ�ļ�����
            fsel_browser_enter_dir(CUR_DIR, 0, NULL);
            
            result =  _music_listmenu_select_file(location, active);
        }
        else if(real_active == 0)
        {
            result = list_scene_favor3play_sure(NULL);
        }
        else if(real_active == -1)
        {
            result = list_scene_favor2play_sure(NULL);
        }
        else
        {
            result = list_scene_favor1play_sure(NULL);
        }
    }
    else
    {
        result = check_type;
    }

    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_list_option_calback(fsel_type_e fsorce)
 * \�б��option�ص�����
 * \param[in]    fsorce�ļ���Դ  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_list_option_callback(file_path_info_t *path_info, uint16 active)
{
    app_result_e result = RESULT_IGNORE;

    //ȡ�б�����
    fsel_type_e file_source = path_info->file_source;

    switch(file_source)
    {
        case FSEL_TYPE_PLAYLIST:   
        //�����б�
        result = list_scene_list_option();
        result = playlist_option_result_proc(path_info, result, active);
        break;

        case FSEL_TYPE_LISTAUDIBLE:
        //audible�б� ֻ������һ�β���/���ڲ���/��ʾ�� ���ֱ�ӷ��ش�����
        result = list_scene_menu_option();
        if (result == RESULT_MENU_REDRAW)
        {
            result = RESULT_REDRAW;
        }
        break;

        case FSEL_TYPE_LISTFAVOR1:
        result = list_scene_favorlist1_option();
        //�Ը����ղؼ��б���ֵ�Ĵ�����
        result = favorlist_option_result_proc(file_source, result, active);
        break;
        
        case FSEL_TYPE_LISTFAVOR2:
        result = list_scene_favorlist2_option();
        //�Ը����ղؼ��б���ֵ�Ĵ�����
        result = favorlist_option_result_proc(file_source, result, active);
        break;

        case FSEL_TYPE_LISTFAVOR3:
        result = list_scene_favorlist3_option();
        //�Ը����ղؼ��б���ֵ�Ĵ�����
        result = favorlist_option_result_proc(file_source, result, active);
        break;

        case FSEL_TYPE_M3ULIST:
        result = list_scene_m3ulist_option();
        result = m3ulist_option_result_proc(path_info, result, active);
        break;

        default:
        break;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_listplay_sure(void *param)
 * \�б���ѡ���ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_listplay_sure(void *param)
{
    //����ѡ���ļ�����
    return RESULT_PLAYING_START;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_addfavor_sure(void *param)
 * \��ӵ��ղؼе�ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_addfavor_sure(void *param)
{
    //���ز���
    menu_title_action_t* paramptr = (menu_title_action_t*) param;

    paramptr->style_id = MENULIST_TITLE;

    paramptr->title_id = S_ADD_TO_WHICH;
    //������һ���˵�
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_addfavor1_sure(void *param)
 * \��ӵ��ղؼ�1��ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_addfavor1_sure(void *param)
{
    return RESULT_ADD_FAVOR1;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_addfavor2_sure(void)
 * \��ӵ��ղؼ�2��ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_addfavor2_sure(void *param)
{
    return RESULT_ADD_FAVOR2;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_addfavor3_sure(void *param)
 * \��ӵ��ղؼ�3��ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_addfavor3_sure(void *param)
{
    return RESULT_ADD_FAVOR3;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_delfavor_sure(void *param)
 * \���ղؼ�ɾ����ȷ�Ϻ���(�Ѿ���λ��ĳ���ղؼ�)
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_delfavor_sure(void *param)
{
    return RESULT_DEL_FAVOR;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_clrfavor_sure(void *param)
 * \����ղؼе�ȷ�Ϻ���(�Ѿ���λ��ĳ���ղؼ�)
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_clrfavor_sure(void *param)
{
    return RESULT_CLR_FAVOR;
}
