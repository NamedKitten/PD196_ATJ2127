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

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_option_option(void)
 * \option�˵���option����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_option_option(void)
{
    //������һ��
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_musicplay_sure(void *param)
 * \music���ڲ���ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_musicplay_sure(void *param)
{
    //���ص����ų���
    return RESULT_PLAYING_KEEP;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_musicpause_sure(void *param)
 * \music��һ�β���ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_musicpause_sure(void *param)
{
    //���ص����ų���
    return RESULT_PLAYING_KEEP;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_radioplay_sure(void *param)
 * \radio���ڲ���ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_radioplay_sure(void *param)
{
    return RESULT_NOWPLAYING;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_radiomute_sure(void *param)
 * \radio��һ�β���ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_radiomute_sure(void *param)
{
    //���ص�radio���ų���
    return RESULT_LASTPLAY;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \file_path_info_t* list_scene_favorsel(fsel_type_e favor_from)
 * \ѡ���ղؼеĵ�һ���ļ���ʼ����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       bool the result
 * \retval          1 success
 * \retval          0 fail
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
bool list_scene_favorsel(file_path_info_t* select_path, fsel_type_e favor_from)
{
    uint8 cur_sel_save;
    uint16 cur_list_save;
    bool init_fsel = FALSE;
    bool init_result = TRUE;
    
    if(select_path->file_source != favor_from)
    {
        cur_sel_save = select_path->file_source;
        cur_list_save = select_path->file_path.plist_location.list_type;
        
        //�˳��ļ�ѡ����
        fsel_exit();
        //�ļ�ѡ������ʼ���ӿڵĲ�������
        select_path->file_source = favor_from;    
        init_result = music_file_init(select_path);
        init_fsel = TRUE;
    }
    
    if(init_result == TRUE)
    {
        //ͳ�Ƶ�ǰĿ¼�ļ�����
        fsel_browser_enter_dir(CUR_DIR, 0, NULL);
        
        //��ȡ��ǰģʽ�µĵ�һ���ɲ����ļ�
        init_result = fsel_browser_select(1);
        
        //�ҵ��ɲ����ļ�
        if (init_result == TRUE)
        {
            //��ȡ��ǰ�ļ���location��Ϣ
            init_result = fsel_browser_get_location(&g_browser_filepath.file_path.flist_location, favor_from);
        }
        else
        {
            if(init_fsel == TRUE)
            {
                //�˳��ļ�ѡ����
                fsel_exit();
                
                select_path->file_source = cur_sel_save;
                select_path->file_path.plist_location.list_type = cur_list_save;

                music_file_init(select_path);
            }
            
            //��ʾδ�����ļ�
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);
            init_result = FALSE;
        }
    }
    return init_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor1play_sure(void *param)
 * \��һ���ղؼ�ѡ��playȷ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor1play_sure(void *param)
{
    bool bret;
    bret = list_scene_favorsel(&g_browser_filepath, FSEL_TYPE_LISTFAVOR1);
    if (bret == TRUE)
    {
        return RESULT_PLAYING_START;
    }
    else
    {
        return RESULT_REDRAW;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor2play_sure(void *param)
 * \�ڶ����ղؼ�ѡ��playȷ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor2play_sure(void *param)
{
    bool bret;
    bret = list_scene_favorsel(&g_browser_filepath, FSEL_TYPE_LISTFAVOR2);
    if (bret == TRUE)
    {
        return RESULT_PLAYING_START;
    }
    else
    {
        return RESULT_REDRAW;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor3play_sure(void *param)
 * \�������ղؼ�ѡ��playȷ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor3play_sure(void *param)
{
    bool bret;
    bret = list_scene_favorsel(&g_browser_filepath, FSEL_TYPE_LISTFAVOR3);
    if (bret == TRUE)
    {
        return RESULT_PLAYING_START;
    }
    else
    {
        return RESULT_REDRAW;
    }
}
