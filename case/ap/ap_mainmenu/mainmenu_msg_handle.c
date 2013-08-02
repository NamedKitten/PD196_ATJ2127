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
 * \file     mainmenu_msg_handle.c
 * \brief    gui��˽����Ϣ�Ĵ�����
 * \author   zhangxs
 * \version  1.0
 * \date  2011/10/08
 *******************************************************************************/
#include  "main_menu.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_close_engine(void)
 * \ж������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_close_engine(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //msg_reply_t temp_reply;
    //ɱ��music ����
    msg.type = MSG_KILL_APP_SYNC;
    msg.content.data[0] = APP_ID_MENGINE;
    //msg.content.data[1] = 0x00;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    return bret;
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
 * \ingroup      music_control.c
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
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  gui��Ϣ�Ĵ���
 * \param[in]    msg_apps_type_e gui_event��gui�¼�
 * \param[out]   none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
app_result_e mainmenu_gui_msg_handle(msg_apps_type_e gui_event)
{
    app_result_e ret_result;
    msg_apps_t mainmenu_msg;

    switch (gui_event)
    {
        case EVENT_MAINMENU_NEXT_ITEM:
        g_active_index++;
        if (g_active_index >= g_item_amount)
        {
            g_active_index = 0;
        }
        ret_result = RESULT_MAIN_NEXT_ITEM;
        break;

        case EVENT_MAINMENU_PREV_ITEM:
        if (g_active_index == 0)
        {
            g_active_index = g_item_amount - 1;
        }
        else
        {
            g_active_index--;
        }
        ret_result = RESULT_MAIN_PREV_ITEM;
        break;

        case EVENT_MAINMENU_CREATE_APP:
        /*����Ӧ��*/
        g_mainmenu_var.cfg_var.active_item = g_mainmenu_var.cfg_var.ap_id[g_active_index];
        mainmenu_msg.type = MSG_CREAT_APP;
        if ((get_engine_state() != ENGINE_STATE_NULL) && (g_active_index == (g_item_amount - 1)))
        {
            g_mainmenu_var.cfg_var.active_item = 0xff; //���ڲ���
            if (get_engine_type() == ENGINE_MUSIC)
            {
                mainmenu_msg.content.data[0] = APP_ID_MUSIC;
            }
            else
            {
                mainmenu_msg.content.data[0] = APP_ID_RADIO;
            }
            if (get_engine_state() == ENGINE_STATE_PLAYING)
            {
                mainmenu_msg.content.data[1] = PARAM_BACKTO_PLAYING;
            }
            else
            {
                mainmenu_msg.content.data[1] = PARAM_BACKTO_PLAYED;
            }
        }
        else
        {
            mainmenu_msg.content.data[0] = (uint8) g_mainmenu_var.cfg_var.ap_id[g_active_index];
            mainmenu_msg.content.data[1] = PARAM_FROM_MAINMENU;
        }
        send_async_msg(APP_ID_MANAGER, &mainmenu_msg);
        ret_result = RESULT_APP_QUIT;
        break;

        case EVENT_MAINMENU_OPTIONMENU:
        ret_result = RESULT_OPTION_MAINMENU;
        break;

        default:
        ret_result = com_message_box(gui_event);
        break;
    }

    return ret_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  MainMenu ��ȡ˽����Ϣ�Ĵ���
 * \param[in]    msg MainMenu �Ĵ���ĸ���˽����Ϣ�Ļص�����
 * \param[out]   none
 * \return       app_result_e
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
app_result_e mainmenu_app_msg_callback(private_msg_t *pri_msg)
{
    app_result_e ret_result = RESULT_NULL;
    //msg_reply_t m_reply;
    /*ͬ����Ϣ����*/
    switch (pri_msg->msg.type)
    {
        case MSG_APP_QUIT:
        ret_result = RESULT_APP_QUIT;
        break;

        case MSG_SD_OUT:
        if (get_engine_type() == ENGINE_MUSIC)
        {
            if (g_file_path_info.file_path.dirlocation.disk == DISK_H)
            {
                music_close_engine();
                gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
                if (g_active_index == (g_item_amount - 1))
                {
                    g_active_index = 0;
                }
                g_item_amount--;
                if (scroll_timer != -1)//�Ѵ�����ɾ��
                {
                    kill_app_timer(scroll_timer);
                }
                ret_result = RESULT_REDRAW;
            }
        }
        break;

        default:
        ret_result = com_message_box(pri_msg->msg.type);
        break;
    }

    return ret_result;
}

