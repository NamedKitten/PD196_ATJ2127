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

/*!
 * \brief
 *  delete_key_map_list����������ļ����ղؼпؼ�����ӳ���
 */
const key_map_t add_favor_key_map_list[] =
{
    /*! �̰� VOL ������ֹ��Ӻ����ļ� */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_PLAYING_RETURN},

    /*! ����Ӧ���¿�ݼ� */
    {{KEY_LOCKHOLD, 0, KEY_TYPE_ALL}, MSG_NULL},
    {{KEY_VOL, 0, KEY_TYPE_LONG}, MSG_NULL},

    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

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
static bool music_add_favorite_deal(void)
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
 *   ��������ļ����ղؼ��ӳ�������
 * \param[in]  plocation ��ǰ�ļ���·����Ϣ
 * \param[in]  target_type �����ĸ��ղؼ�
 * \param[in]  fav_file_total ��ǰ�ղؼ��ļ�����
 * \param[in]  browser
 * \param[out]   none
 * \return       app_result_e the result
 * \retval           RESULT_REDRAW
 * \retval           0 ������
 * \ingroup      music_favorite
 * \note
 */
/*******************************************************************************/
app_result_e add_favorlist_all(plist_location_t * plocation, uint8 target_type, uint16 fav_file_total,
        dir_brow_t *browser)
{
    uint8 tag_backup;
    flist_location_t flocation;
    scanfile_result_e browser_result;
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    private_msg_t private_msg;
    fsel_error_e fsel_error;
    bool ret = TRUE;
    bool is_override_favlist = FALSE;
    app_result_e result = RESULT_NULL;

    //��������ղؼпɿ����������ӳ������ó������ܱ�common��ui_direcotry��ap��ablumlist���ã�����Щ������
    //��ʱ���ڸó����²�Ӧ�������ã�ͨ��change_app_timer_tag,������ͣ��Щ��ʱ�������˳���������ղؼ��ӳ���
    //�������ָ�����Ҫע���������õĶ�ʱ��TAG���ܺ�applib.h������ظ�
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIMER_TAG_FAVOR);

    //��ʾ"���Ժ�"
    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_WAITING);

    //�ļ��б��������ʼ��
    fsel_browser_scanfile_init(browser);

    //�����ļ��в�ɾ���ļ������ļ���
    while (1)
    {
        browser_result = fsel_browser_scanfile(&(g_browser_filepath), browser, SCANFILE_TYPE_DXCH);
        if (browser_result == SCANFILE_RESULT_FILE)//�����ļ�
        {
            if ((fav_file_total == USERPL_FILE_MAX) && (is_override_favlist == FALSE))
            {
                result = _ask_for_override_favorlist();
                if (result != RESULT_DIALOG_YES)
                {
                    //���������Ŀ����ֹ��ǰ��Ӳ���
                    break;
                }
                else
                {
                    //��ʼ���ǵ�ǰ���ղؼ��ļ�
                    is_override_favlist = TRUE;
                }
            }

            fsel_change_fav_location(&(g_browser_filepath.file_path.plist_location), &flocation, FSEL_TYPE_PLAYLIST,
                    &g_id3_info);

            ret = fsel_favorite_add(&flocation, target_type);

            //δ��������ʱ�������������º�̨�ļ�����
            if (ret == TRUE)
            {
                if (fav_file_total != USERPL_FILE_MAX)
                {
                    fav_file_total++;
                    if (g_file_path_info.file_source == target_type)
                    {
                        music_add_favorite_deal();
                    }
                }
            }
            else
            {
                fsel_error = fsel_get_error();

                //�������ģʽ��������ļ�����ӹ��1�7
                //����Ըô���
                if (fsel_error != FSEL_ERR_EXIST)
                {
                    //������ֹ��ǰ��Ӳ���
                    result = RESULT_ERROR;
                    break;
                }
            }

        }
        else//�����ļ���
        {
            //����ļ���Ϊ�գ�����
            if ((browser->file_total == 0) && (browser->dir_total == 0))
            {
                ;//nothing
            }

            //�ļ��������ǰ�ѻص�playlist����㣬������Ӳ���
            if ((browser_result == SCANFILE_RESULT_SELF) || (browser->layer_no == 0))
            {
                fsel_browser_enter_dir(PARENT_DIR, 0, browser);
                result = RESULT_REDRAW;
                break;
            }
        }

        //��ȡgui��Ϣ
        ret = get_gui_msg(&input_msg);
        if (ret == TRUE)//��gui��Ϣ
        {
            //���а���ӳ��
            if (com_key_mapping(&input_msg, &gui_event, add_favor_key_map_list) == TRUE)
            {
                //GUI������Ϣ����
                switch (gui_event)
                {
                    case EVENT_PLAYING_RETURN:
                    //ȡ������
                    return RESULT_IGNORE;

                    default:
                    break;
                }
            }
        }
        else
        {
            //���ˣ�gui��Ϣ�����Ѿ��������
            //��ȡap˽����Ϣ��ϵͳ��Ϣ������ת��Ϊ˽����Ϣ�ٷ��أ���������
            ret = get_app_msg(&private_msg);
            if (ret == TRUE)
            {
                result = music_message_deal(&private_msg);
                if ((result == RESULT_REDRAW) || (result == RESULT_NULL))
                {
                    //��ʾ"���Ժ�"
                    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_WAITING);
                }
                else
                {
                    break;
                }
            }
        }

        //����10ms�����������
        sys_os_time_dly(1);
    }
    change_app_timer_tag(tag_backup);
    return result;
}
