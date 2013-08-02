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

const uint8 entry_favor_id1[] =
{
    OPTION_FAVOR1_NORMAL,
    OPTION_FAVOR1_MPLAYING,
    OPTION_FAVOR1_MPAUSE,
    OPTION_FAVOR1_RPLAYING,
    OPTION_FAVOR1_RPAUSE
};

const uint8 entry_favor_id2[] =
{
    OPTION_FAVOR2_NORMAL,
    OPTION_FAVOR2_MPLAYING,
    OPTION_FAVOR2_MPAUSE,
    OPTION_FAVOR2_RPLAYING,
    OPTION_FAVOR2_RPAUSE
};

const uint8 entry_favor_id3[] =
{
    OPTION_FAVOR3_NORMAL,
    OPTION_FAVOR3_MPLAYING,
    OPTION_FAVOR3_MPAUSE,
    OPTION_FAVOR3_RPLAYING,
    OPTION_FAVOR3_RPAUSE
};

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_menu_option(void)
 * \��һ���˵����option����(audible���б���ͬ)
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menuoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_menu_option(void)
{
    app_result_e result_value;
    engine_type_e engine_type;
    engine_state_e engine_state;

    menu_com_data_t menu;
    menu.app_id = APP_ID_MUSIC;

    //�����Ƿ����/��music or fm
    engine_type = get_engine_type();
    engine_state = get_engine_state();

    if (engine_type == ENGINE_MUSIC)
    {
        //���ڲ���
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //����music���ڲ�����ڲ˵�
            menu.menu_entry = OPTION_MENU_MPLAYING;
        }
        else
        {
            //����music��һ�β�����ڲ˵�
            menu.menu_entry = OPTION_MENU_MPAUSE;
        }
    }
    else if (engine_type == ENGINE_RADIO)
    {
        //�ж��Ƿ���
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //����radio���ڲ�����ڲ˵�
            menu.menu_entry = OPTION_MENU_RPLAYING;
        }
        else
        {
            //����radio��һ�β�����ڲ˵�
            menu.menu_entry = OPTION_MENU_RPAUSE;
        }
    }
    else
    {
        //�޺�̨����ʱ����ʾ��Ϣ string id(����Ӧ��ʹ�öԻ���)
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_OPTION_DESC);
        return RESULT_REDRAW;
    }
    //�Ӹ��˵�����
    menu.browse_mode = 0;
    menu.path_id = MUSIC_PATH_ID_MENU_PLAYLIST;
    menu.menu_func = NULL; //��ʲô��
    result_value = gui_menulist_simple(MENULIST_OPTION, &menu); //ִ��option�˵��ص�
    if (result_value == RESULT_REDRAW)
    {
        result_value = RESULT_MENU_REDRAW;
    }
    return result_value;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor_option(uint8 *entryid_buf)
 * \�ղؼе�option����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menuoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor_option(const uint8 *entryid_buf)
{
    app_result_e result_value;
    engine_type_e engine_type;
    engine_state_e engine_state;

    menu_com_data_t menu;
    menu.app_id = APP_ID_MUSIC;

    //menu.menu_func = NULL;
    //�����Ƿ����/��music or fm
    engine_type = get_engine_type();
    engine_state = get_engine_state();
    
    if (engine_type == ENGINE_MUSIC)
    {
        //���ڲ���
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //������ڲ˵�1 playnow & play
            menu.menu_entry = entryid_buf[1];

        }
        else
        {
            //������ڲ˵�2 lastplayed & play
            menu.menu_entry = entryid_buf[2];
        }
    }
    else if (engine_type == ENGINE_RADIO)
    {
        //bool sound_on;
        //�ж��Ƿ���
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //������ڲ˵�3 playnow & play
            menu.menu_entry = entryid_buf[3];
        }
        else
        {
            //������ڲ˵�4 lastplayed & play
            menu.menu_entry = entryid_buf[4];
        }
    }
    else
    {
        //������ڲ˵�5 play
        menu.menu_entry = entryid_buf[0];
    }
    //�Ӹ��˵�����
    menu.browse_mode = 0;
    menu.path_id = MUSIC_PATH_ID_MENU_FAVORLIST;
    menu.menu_func = NULL; //��ʲô��
    result_value = gui_menulist_simple(MENULIST_OPTION, &menu);
    //if (result_value == RESULT_REDRAW)
    //{
    //    result_value = RESULT_MENU_REDRAW;
    //}
    return result_value;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor1_option(void)
 * \�ղؼ��б�1 option�ص����� �Ƕ�list_scene_favor_option()�����ķ�װ
 * \param[in]
 * \param[out]
 * \return
 * \retval      app_result_e ���ػص��������
 * \ingroup     music_listmenu_menuoption
 * \note
 */
/*********************************************************************************/
app_result_e list_scene_favor1_option(void)
{
    return list_scene_favor_option(entry_favor_id1);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor2_option(void)
 * \�ղؼ��б�2 option�ص����� �Ƕ�list_scene_favor_option()�����ķ�װ
 * \param[in]
 * \param[out]
 * \return
 * \retval      app_result_e ���ػص��������
 * \ingroup     music_listmenu_menuoption
 * \note
 */
/********************************************************************************/
app_result_e list_scene_favor2_option(void)
{
    return list_scene_favor_option(entry_favor_id2);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor3_option(void)
 * \�ղؼ��б�3 option�ص����� �Ƕ�list_scene_favor_option()�����ķ�װ
 * \param[in]
 * \param[out]
 * \return
 * \retval      app_result_e ���ػص��������
 * \ingroup     music_listmenu_menuoption
 * \note
 */
/********************************************************************************/
app_result_e list_scene_favor3_option(void)
{
    return list_scene_favor_option(entry_favor_id3);
}
