/*
 ******************************************************************************
 *                               USDK
 *                            Module: app
 *                 Copyright(c) 2002-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File: ui_show_bookmark.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyufan    2011-9-1 9:27:12      1.0             build this file
 ******************************************************************************
 */
#include "app_music.h"
#include "app_music_bookmark.h"

//.rodata
const uint8 empty_items_str[] = "hh:mm:ss";
//const uint8 ROOT_DIR_NAME[] = ":";

const key_map_t bookmark_key_map_list[] =
{
    /*! UP ����ת��Ϊ SELECT_PREV �¼� */
    {{ KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_BOOKMARK_SELECT_PREV },
    /*! DOWN ����ת��Ϊ SELECT_NEXT �¼� */
    {{ KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_BOOKMARK_SELECT_NEXT },
    /*! �̰�KEY_PLAY ����ת��Ϊ SELECT_SURE �¼� */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_BOOKMARK_SELECT_SURE },
    /*! �̰�KEY_MENU ����ת��Ϊ ENTER_OPTION �¼� */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_BOOKMARK_OPTION },
    /*! �̰�KEY_RETURN ����ת��Ϊ RETURN �¼� */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_BOOKMARK_RETURN },
    /*! ������־ */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

uint16 bmk_cache_sector;
//uint8  bmk_cache_attr;
music_bm_head bm_head;


//bank data
uint8 disp_buf[MAX_BOOKMARKS][BM_ITEM_LEN] _BANK_DATA_ATTR_;

listbox_private_t mbk_show_data _BANK_DATA_ATTR_;

extern void menulist_select_next(menulist_control_t *p_ml_control);
extern void menulist_select_prev(menulist_control_t *p_ml_control);

/******************************************************************************/
/*!
 * \par  Description:
 *	  ����ȡ���ı�ǩʱ��ת��Ϊʱ������ʾ���
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_ui
 * \note
 *******************************************************************************/
void get_bookmark_item(uint8 *dispbuf, int index, time_t *time_buf)
{
    uint8 i;
    uint8 *tmp = (uint8 *) &time_buf[index];

    libc_itoa((index + 1), dispbuf, 2);
    dispbuf[2] = ' ';

    if ((time_buf[index].hour == 0xff) && (time_buf[index].minute == 0xff) && (time_buf[index].second == 0xff))
    {
        return;
    }
    else
    {
        for (i = 1; i < 4; i++)
        {
            libc_itoa(*tmp, &dispbuf[3 * i], 2);
            dispbuf[3 * i + 2] = ':';
            tmp++;
        }
    }
    dispbuf[11] = 0x00;
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  �����ǩ��ؼ�
 * \param[in]  mode ���ģʽ ���/ɾ��/ѡ����ǩ�� msg_call_back ��Ӧ�Ľ���˽����Ϣ
 * 			   ��ϵͳ��Ϣ������
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_ui
 * \note
 *******************************************************************************/
void ui_show_bookmark_item(menulist_control_t* mbk_list_pt)
{
    style_infor_t mbk_infor;
    uint8 i;
    //uint8 draw_mode;

    mbk_infor.style_id = FILELIST;
    mbk_infor.type = UI_COM;

    //�б���items����Чǰ����
    mbk_show_data.item_valid = mbk_list_pt->bottom - mbk_list_pt->top + 1;

    for (i = 0; i < mbk_show_data.item_valid; i++)
    {

        mbk_show_data.items[i].data.str = disp_buf[i + mbk_list_pt->top];
        mbk_show_data.items[i].language = ANSIDATAAUTO;
        mbk_show_data.items[i].length = BM_ITEM_LEN;
    }

    //��ǰ������
    mbk_show_data.active = mbk_list_pt->active;
    //֮ǰ������
    mbk_show_data.old = mbk_list_pt->old;
    //���������������ڻ����˺�ratio
    mbk_show_data.list_total = mbk_list_pt->total;
    //��ǰ���������������λ�ã����ڻ����˺�ratio
    mbk_show_data.list_no = mbk_list_pt->list_no;

    ui_show_listbox(&mbk_infor, &mbk_show_data, mbk_list_pt->draw_mode);
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ȡһ����ʾ��ʱ��
 * \param[in]    list : ������
 * \param[in]    bottom : ������
 * \param[in]    cur_index : ��ǰ������
 * \param[out]   ml_control�����ظ����˵Ĳ˵��б����
 * \return       none
 * \note
 *******************************************************************************/
void get_one_screen_time(uint8 top, uint8 bottom, uint16 cur_index)
{
    uint8 i;
    //uint32 mbmk_fp;
    time_t bm_items[MAX_BOOKMARKS];
    //��ΪҪ��ȡ�ļ�,�õ�����һ��bank��bank������
    g_bookmark_fp = open_bookmark();
    get_bookmark_time(cur_index, bm_items, g_bookmark_fp);

    //��ȡ��ǩ��Ϣ
    for (i = top; i <= bottom; i++)
    {
        libc_memcpy(&disp_buf[i][3], empty_items_str, 9);
        get_bookmark_item(disp_buf[i], i, bm_items);
    }

    close_bookmark(g_bookmark_fp);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 ui_get_bookmark_index(void)
 * \��ȡ��ǰ��ǩ���ڵ�index
 * \param[in]
 * \param[out]
 * \return     uint16
 * \retval     ��ǰ��ǩ�����
 * \ingroup     music_setmenu_bookmark_deal.c
 * \note
 *******************************************************************************/
uint16 ui_get_bookmark_index(void)
{
    uint16 index = 0xffff;

    file_location_t *plocation = (file_location_t *) &g_file_path_info.file_path;

    music_get_filepath(&g_file_path_info);

    //��λ���ļ�
    //fsel_set_location(&(g_file_path_info.file_path.plist_location), g_file_path_info.file_source);
    fsel_browser_set_file(&(g_file_path_info.file_path.plist_location), g_file_path_info.file_source);
    //ȡ������
    vfs_get_name(g_music_mount_id, g_song_filename, 32);

    // �����ļ��б�
    vfs_cd(g_music_mount_id, CD_ROOT, 0);

    g_bookmark_fp = open_bookmark();

    if (g_bookmark_fp == 0)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_BOOKMARK);
    }
    else
    {
        //ͨ��������cluster_no��dir_entry�ҵ���Ӧ��ǩ��index
        index = get_bookmark_index(plocation->cluster_no, plocation->dir_entry, g_song_filename, g_bookmark_fp);
        close_bookmark(g_bookmark_fp);
    }
    return index;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 ui_show_bookmark(uint8 mode, msg_cb_func msg_call_back)
 * \��ʾ��ǩ��
 * \param[in]    mode : ����ģʽ
 * \param[in]    msg_call_back: ��Ϣ����ص�����
 * \param[in]
 * \param[out]
 * \return       uint16
 * \note
 *******************************************************************************/
uint16 ui_show_bookmark(uint8 mode, msg_cb_func msg_call_back)
{
    bool ret;
    uint8 tag_backup;

    bool need_read_bmk = TRUE;

    uint16 cur_index, result;

    menulist_control_t ml_control_deal;

    input_gui_msg_t input_msg; //��������Ϣ����gui��Ϣ

    //gui ��Ϣ��Ӧgui �¼�
    msg_apps_type_e gui_event;

    private_msg_t private_msg; //˽����Ϣ

    libc_memset(&ml_control_deal, 0, sizeof(ml_control_deal));
    ml_control_deal.total = MAX_BOOKMARKS;
    ml_control_deal.bottom = LIST_NUM_ONE_PAGE - 1;
    ml_control_deal.draw_mode = LIST_DRAW_ALL;
    ml_control_deal.one_page_count = LIST_NUM_ONE_PAGE;

    // �����ļ��б�
    //vfs_cd(g_music_mount_id, 0, ROOT_DIR_NAME);

    cur_index = ui_get_bookmark_index();
    if (cur_index == 0xffff)
    {
        return RESULT_PLAYING_SCENE;
    }
    //��ǩ�ɿ����������ӳ������ó������ܱ�common��ui_menulist���ã�����Щ������
    //��ʱ���ڸó����²�Ӧ�������ã�ͨ��change_app_timer_tag,������ͣ��Щ��ʱ�������˳���ǩ�ӳ���
    //�������ָ�����Ҫע���������õĶ�ʱ��TAG���ܺ�applib.h������ظ�
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIMER_TAG_BOOKMARK);    

    while (1)
    {
        if (need_read_bmk == TRUE)
        {
            need_read_bmk = FALSE;
            get_one_screen_time(0, 9, cur_index);
        }
        if (ml_control_deal.draw_mode != LIST_DRAW_NULL)
        {
            ui_show_bookmark_item(&ml_control_deal);
            ml_control_deal.draw_mode = LIST_DRAW_NULL;
        }

        //��ȡgui��Ϣ
        ret = get_gui_msg(&input_msg);
        if (ret == TRUE)//��gui��Ϣ
        {
            ////���а���ӳ��
            if (com_key_mapping(&input_msg, &gui_event, bookmark_key_map_list) == TRUE)
            {
                result = RESULT_NULL;
                //GUI������Ϣ����
                switch (gui_event)
                {
                    case EVENT_BOOKMARK_SELECT_NEXT:
                    //bmlist_select_next(&ml_control_deal);
                    menulist_select_next(&ml_control_deal);
                    //ml_control_deal.draw_mode = 0;
                    break;

                    case EVENT_BOOKMARK_SELECT_PREV:
                    //bmlist_select_prev(&ml_control_deal);
                    menulist_select_prev(&ml_control_deal);
                    //ml_control_deal.draw_mode = 0;
                    break;

                    case EVENT_BOOKMARK_OPTION:
                    result = RESULT_REDRAW;
                    break;

                    case EVENT_BOOKMARK_SELECT_SURE:
                    //��ȷ�ϼ�����Ҫ��ӣ�ɾ����ת����ǩ
                    handle_bookmark((uint8) ml_control_deal.list_no, mode, cur_index);
                    result = RESULT_PLAYING_SCENE;
                    break;
                    //���ذ���
                    case EVENT_BOOKMARK_RETURN:
                    result = RESULT_REDRAW;
                    break;
                    default:
                    result = com_message_box(gui_event);
                    break;
                }
                if ((result != RESULT_NULL) && (result != RESULT_IGNORE))
                {
                    break;
                }
            }
        }
        else
        {
            //���ˣ�gui��Ϣ�����Ѿ��������
            //����ap˽����Ϣ��ϵͳ��Ϣ
            ret = get_app_msg(&private_msg);
            if (ret == TRUE)
            {
                result = msg_call_back(&private_msg);
                if ((result == RESULT_REDRAW) || (result == RESULT_NULL))
                {
                    need_read_bmk = TRUE;
                    ml_control_deal.draw_mode = LIST_DRAW_ALL;
                }
                else
                {
                    //goto menu_exit;
                    break;
                }
            }
        }
        sys_os_time_dly(2);

    }//end of while(1)
    change_app_timer_tag(tag_backup);    
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void del_bookmark_file(file_location_t *plocation)
 * \ɾ����ǩ�ļ�
 * \param[in]    plocation �ļ�·��
 * \param[out]
 * \return       none
 * \note
 *******************************************************************************/
void del_bookmark_file(file_location_t *plocation)
{
    //����Ŀ¼����ǩ�ļ�
    vfs_cd(g_music_mount_id, CD_ROOT, 0);

    g_bookmark_fp = open_bookmark();
    if (g_bookmark_fp != 0)
    {
        music_del_bookmark(plocation, 0, g_bookmark_fp);
        //vfs_file_close(g_music_mount_id, bm_fp);
        close_bookmark(g_bookmark_fp);
    }
    return;
}

