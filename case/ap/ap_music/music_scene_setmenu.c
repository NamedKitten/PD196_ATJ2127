/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-19          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_menu_config.h"

//dir layer save
static pdir_layer_t sys_layer_buf _BANK_DATA_ATTR_;
static uint8 dentry_name[16] ;

static const uint8 record_name[] =
{
    0xff, 0xfe,  //flag
    'R',  0x00,
    'E',  0x00,
    'C',  0x00,
    'O',  0x00,
    'R',  0x00,
    'D',  0x00,
    0x00, 0x00  //end flag
};

#if 0
/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_setmenu_init()
 * \���ų�����ʼ��
 * \param[in]    paly_set���벥�ų�������� para1
 *               typedef struct
 *							 {
 *							 	fsel_type_e file_sorce;
 *							 	uint16 reserved;
 *							 	musfile_location_u file_path;
 *							 }file_path_info_t
 * \param[in]    path_locat�ļ�·����Ϣָ�� para2
 *							 typedef enum
 *							 {
 *							 	PLAY_START //�����ļ���ʼ����
 *							 	PLAY_RESUME //����ͣ�ָ�����
 *							 	PLAY_KEEP //���ֲ���/��ͣ״̬
 *							 }music_play_set_e;
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       ......
 * \ingroup      music_scene_setmenu.c
 * \note
 */
/*******************************************************************************/
bool _scene_setmenu_init(void)
{
    return TRUE;

}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_exit(void)
 * \�˳����ų���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool _scene_setmenu_exit(void)
{
    return TRUE;
}
#endif
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_leveltop_option(void)
 * \��߲�˵���option��Ӧ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_PLAYING_SCENE �ص����ų���
 * \retval       RESULT_IGNORE ����
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_leveltop_option(void)
{
    //���ص����ų���
    return RESULT_PLAYING_SCENE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_soudset_option(void)
 * \������˵���option��Ӧ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_IGNORE �ص���һ���˵�
 * \retval
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_levelother_option(void)
{
    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool set_scene_record_file(void)
 * \�жϵ�ǰ���������Ƿ�ΪrecordĿ¼����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \ingroup      music_setmenu
 * \note
 */
/*******************************************************************************/
bool set_scene_record_file(void)
{
    //path save
    uint8 cur_offset_save[8];
    dir_brow_t brow;
    uint8 dir_layer;
    uint8 dir_type;

    bool record_file = FALSE;

    file_path_info_t *path_locat = &g_file_path_info;
    dir_layer = path_locat->file_path.dirlocation.dir_layer;

    if (g_enter_mode == PARAM_FROM_RECORD)
    {
        record_file = TRUE;
    }
    else
    {
        //�����ڲ���ͼ���ģʽ����
        if (path_locat->file_source == FSEL_TYPE_COMMONDIR)
        {
            brow.name_len = 16;
            brow.name_buf = dentry_name;

            //save current path
            vfs_file_dir_offset(g_music_mount_id, &sys_layer_buf, cur_offset_save, 0);

            while (dir_layer > 0)
            {
                if (dir_layer == 1)
                {
                    dir_type = CUR_DIR;
                }
                else
                {
                    dir_type = PARENT_DIR;
                }

                fsel_browser_enter_dir(dir_type, 0, &brow);
                dir_layer--;

                if (dir_type == CUR_DIR)
                {
                    //����Ŀ¼��ƥ��
                    if (libc_memcmp(dentry_name, record_name, 16) == 0)
                    {
                        record_file = TRUE;
                    }
                }
            }

            //set current path
            vfs_file_dir_offset(g_music_mount_id, &sys_layer_buf, cur_offset_save, 0);
        }
    }

    return record_file;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e music_scene_setmenu(bool music_audible)
 * \���ò˵���������
 * \param[in]    music_audible  para1
 *							 bool ���ھ����ǽ���music�˵� ����audible�˵�
 * \param[in]    paly_set���벥�ų�������� para2
 *               typedef struct
 *							 {
 *							 	fsel_type_e file_sorce;
 *							 	uint16 reserved;
 *							 	musfile_location_u file_path;
 *							 }file_path_info_t
 * \param[in]    path_locat�ļ�·����Ϣָ�� para3
 *							 typedef enum
 *							 {
 *							 	PLAY_START //�����ļ���ʼ����
 *							 	PLAY_RESUME //����ͣ�ָ�����
 *							 	PLAY_KEEP //���ֲ���/��ͣ״̬
 *							 }music_play_set_e;
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       ......
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/

app_result_e music_scene_setmenu(bool music_audible)
{
    //bool result;
    uint16 ret_vals;
    engine_state_e engine_state;
    //�˵�����
    menu_com_data_t menu_param;
    menu_param.app_id = APP_ID_MUSIC;
    //�ײ�˵��ص�����
    menu_param.menu_func = NULL;

    if (set_scene_record_file() == TRUE)
    {
        engine_state = get_engine_state();
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            menu_param.menu_entry = VOICE_MENU_PLAYING;
        }
        else
        {
            menu_param.menu_entry = VOICE_MENU_LASTPLAY;
        }

    }
    else
    {
        //��music�����ò˵�
        if (music_audible == TRUE)
        {
            menu_param.menu_entry = MUSICSET_MENU;
            menu_param.path_id = MUSIC_PATH_ID_SETMENU_MUSIC;
        }
        else
        {
            //��audible�����ò˵�
            menu_param.menu_entry = AUDIBLESET_MENU;
            menu_param.path_id = MUSIC_PATH_ID_SETMENU_AUDIBLE;
        }
    }
    //������ʼ��
    //result = _scene_setmenu_init();

    //if (result == FALSE)
    //{
    //    return RESULT_SCENE_EXIT;
    //}

    //���ò˵��������ؼ�
    ret_vals = gui_menulist(MENULIST_OPTION, &menu_param);
    if ((ret_vals == RESULT_NULL) || (ret_vals == RESULT_REDRAW))
    {
        ret_vals = RESULT_PLAYING_SCENE;
    }

    //}//end of while
    //���ó����˳�����
    //_scene_setmenu_exit();
    return ret_vals;
}
