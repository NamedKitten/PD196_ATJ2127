/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mainmenu_msg_loop.c
 * \brief    ��Ϣѭ��������һЩ��صĺ���
 * \author   zhangxs
 * \version  1.0
 * \date  2011/10/08
 *******************************************************************************/
#include  "main_menu.h"
/*ȫ�ֱ�������*/
const key_map_t desktop_key_map_list[] =
{
    { { KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_HOLD}, EVENT_MAINMENU_NEXT_ITEM },
    { { KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_HOLD}, EVENT_MAINMENU_PREV_ITEM },
//    { { KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_MAINMENU_OPTIONMENU },
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_MAINMENU_CREATE_APP },
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ȡ�����ap��Ӧ���������е����
 * \param[in]    �����Ӧ��ID��0xffff��ʾ���ڲ��Ż�����һ�β���
 * \param[out]   none
 * \return       �����Ӧ�ö�Ӧ���������е����
 * \retval       �����Ӧ�ö�Ӧ���������е����
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
static uint16 _get_ap_index(uint16 active_id)
{
    uint16 index, total;

    total = g_mainmenu_var.cfg_var.total_item;
    if (total > MAX_APP_NUM)
    {
        total = MAX_APP_NUM;
    }
    for (index = 0; index < total; index++)
    {
        /*�ҵ�����ap��ID*/
        if (g_mainmenu_var.cfg_var.ap_id[index] == active_id)
        {
            break;
        }
    }
    /*δ�ҵ�����ap��ID����ʾ�����active_idΪ0xffff������һ�β��Ż������ڲ���*/
    if (index == g_mainmenu_var.cfg_var.total_item)
    {
        index = g_item_amount - 1;
    }
    return index;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  �����Ļص�����
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void item_scroll_handle(void)
{
    ui_show_textbox(NULL, NULL, TEXTBOX_DRAW_ITEM_SCROLL);
}

void _check_mengine_status(void)
{
    g_check_mengine_status = TRUE;
}




/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_status(mengine_status_t* play_status)
 * \��ȡ��ǰ����״̬
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_get_status(mengine_status_t* play_status)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;
    //��ȡ��ǰ����״̬��
    reply.content = play_status;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_GET_STATUS_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playinfo(mengine_playinfo_t* play_info)
 * \��ȡ��ǰ���ŵ�ʱ��ͱ�������Ϣ
 * \param[in]    void  para1
 * \param[out]   mengine_playinfo_t play_info
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_get_playinfo(mengine_playinfo_t* play_info)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;
    //��ȡ��ǰ����״̬��
    reply.content = play_info;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_GET_PLAYINFO_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _playing_check_status(file_path_info_t* path_locat)
 * \�������ȡ��ǰ״̬��Ϣ �ж��Ƿ���� �Ƿ��л�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
file_status_e _playing_check_status(file_path_info_t* path_locat)
{
    mengine_playinfo_t playinfo;
    
    //��ȡ������Ϣ
    music_get_playinfo(&playinfo);

    if ((playinfo.cur_file_switch & 0x01) != 0)
    {
        return FILE_CHANGE;
    }
    else
    {
        return FILE_NO_CHANGE;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  ���������ʾ����Ϣ�Ļ�ȡ
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
app_result_e mainmenu_desktop(void)
{
    bool need_draw = TRUE;
    bool msg_result;
    uint16 active_id = g_mainmenu_var.cfg_var.active_item;
    /*���صĽ����Ϣ*/
    mainmenu_ui_result_e ui_result = NORMAL;
    app_result_e ret_result = RESULT_NULL;
    input_gui_msg_t gui_msg;
    msg_apps_type_e gui_event;
    private_msg_t pri_msg;
    file_status_e file_sta;

    g_item_amount = g_mainmenu_var.cfg_var.total_item;
    if (get_engine_state() != ENGINE_STATE_NULL)
    {
        g_item_amount += 1;
    }
    /*��ȡ����˵���*/
    g_active_index = _get_ap_index(active_id); //_get_ap_index�õ���g_item_amount���������g_item_amount��ֵ�����

    if (g_active_index >= g_item_amount)
    {
        g_active_index = 0;
    }

    /*��ʼ��ap��ʾ��Ϣ*/
    _init_ap_display_infor();

    if (need_check_status == TRUE)
    {
        status_timer = set_app_timer(APP_TIMER_ATTRB_UI, 1000, _check_mengine_status);
    }

    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_NONE);
    headbar_update = HEADBAR_UPDATE_INIT;
    while (1)
    {
        /*��ʾ����*/
        if (need_draw == TRUE)
        {
            _paint_desktop(ui_result);
            headbar_update = HEADBAR_UPDATE_ALL;
            need_draw = FALSE;
        }

        if (g_check_mengine_status == TRUE)
        {
            g_check_mengine_status = FALSE;

            //�������ȡ��ǰ״̬��Ϣ
            file_sta = _playing_check_status(&g_file_path_info);
            if (file_sta == FILE_CHANGE)
            {
                if (g_active_index == (g_item_amount - 1))
                {
                    ui_result = BOTTOM;
                    need_draw = TRUE;
                }
            }
            //            else if (file_sta == FORMAT_ERR)
            //            {
            //                music_close_engine();
            //                if (g_active_index == (g_item_amount - 1))
            //                {
            //                    g_active_index = 0;
            //                }
            //                g_item_amount--;
            //                need_draw = TRUE;
            //            }
            //            else
            //            {
            //                ;//do nothing
            //            }
        }

        /*����ui��Ϣ*/
        msg_result = get_gui_msg(&gui_msg);

        if (msg_result == TRUE)
        {
            if (com_key_mapping(&gui_msg, &gui_event, desktop_key_map_list) == TRUE)
            {
                ret_result = mainmenu_gui_msg_handle(gui_event);

                switch (ret_result)
                {
                    case RESULT_MAIN_NEXT_ITEM:
                    ui_result = NEXTITEM;
                    need_draw = TRUE;
                    ret_result = RESULT_NULL;
                    break;

                    case RESULT_MAIN_PREV_ITEM:
                    ui_result = PREVITEM;
                    need_draw = TRUE;
                    ret_result = RESULT_NULL;
                    break;

                    case RESULT_REDRAW:
                    ui_result = NORMAL;
                    need_draw = TRUE;
                    ret_result = RESULT_NULL;
                    break;

                    default:
                    break;
                }
            }
        }
        else
        {
            /*����ap˽����Ϣ*/
            msg_result = get_app_msg(&pri_msg);
            if (msg_result == TRUE)
            {
                ret_result = mainmenu_app_msg_callback(&pri_msg);
                if (ret_result == RESULT_REDRAW)
                {
                    ui_result = NORMAL;
                    need_draw = TRUE;
                    ret_result = RESULT_NULL;
                }
            }
        }

        if (ret_result != RESULT_NULL)
        {
            if (scroll_timer != -1)//�Ѵ�����ɾ��
            {
                kill_app_timer(scroll_timer);
                scroll_timer = -1;
            }

            if (status_timer != -1)
            {
                kill_app_timer(status_timer);
                status_timer = -1;
            }
            return ret_result;
        }
        //����10ms�����������
        sys_os_time_dly(5);
    }/*end of while(1)*/
}
