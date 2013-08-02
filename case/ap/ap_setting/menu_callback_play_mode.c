/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_play_mode.c
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ap_setting.h"
#include "menu_callback_play_mode.h"




//һ���˵�-����ģʽ
//����ģʽ|-������Դ��...|-...��Դ�������ļ�
//        |              |-...��Դ�ڴ��ݳ���
//        |              |-...��Դ�ڴ�ר��
//        |              |-...��Դ�ڴ�����
//        |-�ظ�����|-��
//        |         |-�����ظ�
//        |         |-ȫ���ظ�
//        |-�������|-��
//                  |-��
////////////////////

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������option���Ĵ���������ʾ��ʾ��Ϣ
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ����
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e option_callback(void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;

    string_desc.data.id = S_SETTING_OPTION;
    string_desc.language = UNICODEID;

    dialog_com.dialog_type = DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);

    return RESULT_REDRAW;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playmode(uint8* playmode)
 * \��ȡѭ��ģʽ&shuffleģʽ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      menu_callback_play_mode.c
 * \note
 */
/*******************************************************************************/
bool music_get_playmode(uint8* playmode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;
    //��ȡ��ǰѭ��ģʽ
    reply.content = playmode;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_GET_PLAYMODE_SYNC;
    //����ͬ����Ϣ
    if (music_exist_flag != 0)
    {
        bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    }
    else
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_playmode(uint8 playmode)
 * \����ѭ��ģʽ&shuffleģʽ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      menu_callback_play_mode.c
 * \note
 */
/*******************************************************************************/
bool music_set_playmode(uint8 playmode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //����ѭ��ģʽ&shuffleģʽ
    msg.content.addr = &playmode;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_SET_PLAYMODE_SYNC;
    //����ͬ����Ϣ
    if (music_exist_flag != 0)
    {
        bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    }
    else
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void switch_play_mode(repeat_mode_e mode)
 * \�޸ĵ�ǰ�����ļ���ѭ��ģʽ
 * \param[in]    mode  para1
 * \             typedef enum
 * \             {
 * \             	LoopNor, //��ͨѭ��
 * \             	LoopOne, //ѭ���ŵ�ǰ��
 * \             	LoopOneDir, //ѭ���ŵ�ǰĿ¼
 * \             	LoopDir, //˳�򲥷�Ŀ¼
 * \             	LoopAll, //ѭ������
 * \              Intro, //�������
 * \              MaxRepeatMod
 * \             }repeat_mode_e;
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval
 * \ingroup      menu_callback_play_mode.c
 * \note
 */
/*******************************************************************************/
void switch_play_mode(repeat_mode_e mode)
{
    uint8 cur_mode;

    //��ȡ�����ѭ��ģʽ��Ϣ
    music_get_playmode(&cur_mode);
    //���ò���ģʽ
    cur_mode &= (uint8) 0xf0;
    cur_mode |= (uint8) mode;
    music_set_playmode(cur_mode);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_filepath(file_path_info_t *filepath)
 * \��ȡ��ǰ�����ļ���·����Ϣ
 * \param[in]    filepath����ļ�·��buffer��ָ��  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      menu_callback_play_mode.c
 * \note
 */
/*******************************************************************************/
bool music_get_filepath(file_path_info_t *filepath)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;
    //��ȡ��ǰ�����ļ���·����Ϣ
    reply.content = filepath;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
    //����ͬ����Ϣ
    if (music_exist_flag != 0)
    {
        bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    }
    else
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_change_filepath(file_path_info_t *filepath)
 * \���Ĳ����ļ���·����Ϣ
 * \param[in]    filepath����ļ�·��buffer��ָ��  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      menu_callback_play_mode.c
 * \note
 */
/*******************************************************************************/
bool music_change_filepath(file_path_info_t *filepath)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //���Ĳ����ļ���·����Ϣ
    msg.content.addr = filepath;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_CHANGE_FILEPATH_SYNC;
    //����ͬ����Ϣ
    if (music_exist_flag != 0)
    {
        bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    }
    else
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void set_music_source_to_engine(plist_type_e dest_type)
 * \�л���ǰ���ŵ��ļ�·��
 * \param[in]    dest_type  para1
 * \             typedef enum
 * \             {
 * \             	 Plist_AllSong,//music title
 * \             	 Plist_Artist, //music artist
 * \             	 Plist_Album,  //music album
 * \             	 Plist_Genre,  //music genre
 * \               Plist_Books,//audible books
 * \               Plist_Author //audible author
 * \             }plist_type_e
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
void set_music_source_to_engine(plist_type_e dest_type)
{
    plist_location_t result_ptr;
    file_path_info_t g_file_path_info;
    void* cur_locat;
    //��ȡ�ļ�·��
    music_get_filepath(&g_file_path_info);
    cur_locat = &g_file_path_info.file_path.dirlocation;
    //������ǰ�ļ���·��
    fsel_change_filepath(cur_locat, &result_ptr, g_file_path_info.file_source, dest_type);
    //�޸��ļ�ѡ������
    g_file_path_info.file_source = FSEL_TYPE_PLAYLIST;
    //���õ�ǰ·��������
    libc_memcpy(&(g_file_path_info.file_path.plist_location), &result_ptr, sizeof(plist_location_t));
    //�����ļ�·��,�����ͣ,�����¿�ʼ����
    music_change_filepath(&g_file_path_info);
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�������������music��Դ��all songs
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_music_from_all_songs_callback(void *menu_title_void)
{
    if ((music_exist_flag == ENGINE_MUSIC)&&(playlist_exist_flag ==1))
    {    
        g_setting_vars.music_comval.music_from = VALUE_FROM_ALL_SONGS;
        set_music_source_to_engine((plist_type_e) g_setting_vars.music_comval.music_from);
        return (app_result_e)RESULT_NOWPLAYING;
    }
    return (app_result_e) RESULT_REDRAW; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������music��Դ�ڸ��ݳ���
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_music_from_artist_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    if ((music_exist_flag != ENGINE_MUSIC)||(playlist_exist_flag !=1))
    {
        string_desc.data.id = S_NO_ARTIST;
        string_desc.language = UNICODEID;

        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        dialog_com.icon_id = V_U16_INVALID;
        dialog_com.string_desc = &string_desc;
        gui_dialog(DIALOG_MSG, &dialog_com);
    }
    else
    {
        g_setting_vars.music_comval.music_from = VALUE_FROM_ARTIST;
        set_music_source_to_engine((plist_type_e) g_setting_vars.music_comval.music_from);
        return (app_result_e)RESULT_NOWPLAYING;
    }
    return (app_result_e) RESULT_REDRAW; //������һ��
}
/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�������������music��Դ�ڸ�ר��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_music_from_album_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    if ((music_exist_flag != ENGINE_MUSIC)||(playlist_exist_flag !=1))
    {
        string_desc.data.id = S_NO_ALBUM;
        string_desc.language = UNICODEID;

        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        dialog_com.icon_id = V_U16_INVALID;
        dialog_com.string_desc = &string_desc;
        gui_dialog(DIALOG_MSG, &dialog_com);
    }
    else
    {
        g_setting_vars.music_comval.music_from = VALUE_FROM_ALBUM;
        set_music_source_to_engine((plist_type_e) g_setting_vars.music_comval.music_from);
        return (app_result_e)RESULT_NOWPLAYING;
    }
    return (app_result_e) RESULT_REDRAW; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�������������music��Դ�ڸ÷��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_music_from_genre_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    if ((music_exist_flag != ENGINE_MUSIC)||(playlist_exist_flag !=1))
    {
        string_desc.data.id = S_NO_GENRE;
        string_desc.language = UNICODEID;

        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        dialog_com.icon_id = V_U16_INVALID;
        dialog_com.string_desc = &string_desc;
        gui_dialog(DIALOG_MSG, &dialog_com);
    }
    else
    {
        g_setting_vars.music_comval.music_from = VALUE_FROM_GENRE;
        set_music_source_to_engine((plist_type_e) g_setting_vars.music_comval.music_from);
        return (app_result_e)RESULT_NOWPLAYING;
    }
    return (app_result_e) RESULT_REDRAW; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ�������ظ�����ģʽstring ID��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : �����ַ���ID�ţ��Ա����¼��˵�ȡ�ü�����
 * \retval       ��������һ���˵�
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e get_music_repeat_mode_id_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_repeat_mode;
    menu_title_repeat_mode = (menu_title_action_t*) title_action;
    if (g_setting_vars.music_comval.music_repeat_mode == VALUE_MUSIC_REPEAT_ONE)
    {
        active_id = S_REPEAT_ONE;
    }
    else if (g_setting_vars.music_comval.music_repeat_mode == VALUE_MUSIC_REPEAT_ALL)
    {
        active_id = S_REPEAT_ALL;
    }
    else if (g_setting_vars.music_comval.music_repeat_mode == VALUE_MUSIC_REPEAT_OFF)
    {
        active_id = S_REPEAT_OFF;
    }
    else if(g_setting_vars.music_comval.music_repeat_mode ==VALUE_MUSIC_REPEAT_INTRO)
    {
        active_id = S_REPEAT_INTRO;
    }
    else
    {
        active_id = S_REPEAT_OFF;
    }
    menu_title_repeat_mode->menu_entry = -1; //���������
    menu_title_repeat_mode->str_id = active_id; //ָ��active��
    menu_title_repeat_mode->ret_str = NULL;
    menu_title_repeat_mode->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�������������music�ر�ѭ��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       �����ء�������һ������Ϣ
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_music_repeat_off_callback(void *menu_title_void)
{
    g_setting_vars.music_comval.music_repeat_mode = VALUE_MUSIC_REPEAT_OFF;
    //�л�ѭ����ʽ
    switch_play_mode((repeat_mode_e) g_setting_vars.music_comval.music_repeat_mode);
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�������������music����ѭ��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       �����ء������ϼ�����Ϣ
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_music_repeat_one_callback(void *menu_title_void)
{
    g_setting_vars.music_comval.music_repeat_mode = VALUE_MUSIC_REPEAT_ONE;
    //�л�ѭ����ʽ
    switch_play_mode((repeat_mode_e) g_setting_vars.music_comval.music_repeat_mode);
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������musicȫ��ѭ��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       �����ء������ϼ�����Ϣ
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_music_repeat_all_callback(void *menu_title_void)
{
    g_setting_vars.music_comval.music_repeat_mode = VALUE_MUSIC_REPEAT_ALL;
    //�л�ѭ����ʽ
    switch_play_mode((repeat_mode_e) g_setting_vars.music_comval.music_repeat_mode);
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������music�������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       �����ء������ϼ�����Ϣ
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_music_intro_callback(void *menu_title_void)
{
    g_setting_vars.music_comval.music_repeat_mode = VALUE_MUSIC_REPEAT_INTRO;
    //�л�ѭ����ʽ
    switch_play_mode((repeat_mode_e) g_setting_vars.music_comval.music_repeat_mode);
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ��music���ѭ����ʽ
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : �����ַ���ID�ţ��Ա����¼��˵�ȡ�ü�����
 * \retval       ��������һ���˵�
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e get_music_shuflle_option(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_shuffle_option;
    menu_title_shuffle_option = (menu_title_action_t*) title_action;
    if (g_setting_vars.music_comval.music_shuffle == VALUE_MUSIC_SHUFFLE_ON)
    {
        active_id = S_ON;
    }
    else
    {
        active_id = S_OFF;
    }
    menu_title_shuffle_option->menu_entry = -1; //���������
    menu_title_shuffle_option->str_id = active_id; //ָ��active��
    menu_title_shuffle_option->ret_str = NULL;
    menu_title_shuffle_option->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�������������music shuffle �ر�
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       �����ء������ϼ�����Ϣ
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_music_shuffle_off_callback(void *menu_title_void)
{

    uint8 cur_mode;
    //��ȡ����Ĳ���ģʽ��Ϣ
    music_get_playmode(&cur_mode);
    //����shuffleģʽ
    cur_mode &= 0x0f;
    music_set_playmode(cur_mode);

    g_setting_vars.music_comval.music_shuffle = VALUE_MUSIC_SHUFFLE_OFF;
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�������������music shuffleʹ��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       �����ء������ϼ�����Ϣ
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_music_shuffle_on_callback(void *menu_title_void)
{
    uint8 cur_mode;
    //��ȡ����Ĳ���ģʽ��Ϣ
    music_get_playmode(&cur_mode);
    //����shuffleģʽ
    cur_mode |= 0x10;
    music_set_playmode(cur_mode);

    g_setting_vars.music_comval.music_shuffle = VALUE_MUSIC_SHUFFLE_ON;
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:�������ָ�music Ĭ������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ����
 * \note: ע������˵��
 *
 ******************************************************************************
 */
void restore_factory_setting_for_music(void)
{
    uint8 cur_mode;
    if(music_exist_flag == ENGINE_MUSIC)
    {    
//����Ҫ�ָ�������Դ
//        set_music_source_to_engine((plist_type_e) g_setting_vars.music_comval.music_from);
//�ָ�ѭ����ʽ
        switch_play_mode((repeat_mode_e) g_setting_vars.music_comval.music_repeat_mode);
//�ָ�shuffle ģʽ
        music_get_playmode(&cur_mode);    //��ȡ����Ĳ���ģʽ��Ϣ
        if(g_setting_vars.music_comval.music_shuffle == VALUE_MUSIC_SHUFFLE_ON)
        {
            cur_mode |= 0x10; //����shuffle onģʽ    
        }   
        else
        { 
            cur_mode &= 0x0f; //����shuffle offģʽ
        }
        music_set_playmode(cur_mode);  
    }
}

