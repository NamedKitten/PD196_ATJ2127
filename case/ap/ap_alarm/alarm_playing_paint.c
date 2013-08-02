/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-15          1.0              create this file
 *******************************************************************************/
#include "ap_alarm.h"



/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_clear_error(void)
 * \��������
 * \param[in]    void para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_paint.c
 * \note
 */
/*******************************************************************************/
bool music_clear_error(void)
{
    bool bret = TRUE;
    msg_apps_t msg;

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_CLEAR_ERR_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_eror_deal(void)
 * \������ʾ���л�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      alarm_playing_paint.c
 * \note
 */
/*******************************************************************************/
void play_eror_deal(void)
{
    dialog_com_data_t dialog_error;
    string_desc_t error_str_desc;
    //�ŵ�����һ��bank��,�������
    //��������
    music_clear_error();

    error_str_desc.data.id = S_FORMAT_ERROR;
    error_str_desc.language = UNICODEID;
    //�Ի������� ��ʾ
    dialog_error.dialog_type = DIALOG_INFOR_WAIT;
    dialog_error.icon_id = -1;
    //�ַ���
    dialog_error.string_desc = &error_str_desc;

    dialog_error.button_type = BUTTON_NO_BUTTON;
    //��ʾ����
    gui_dialog(DIALOG_MSG, &dialog_error);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_paint(uint32 *display_flag)
 * \���Ʋ��Ž���
 * \param[in]    para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      alarm_playing_paint.c
 * \note
 */
/********************************************** *********************************/
void play_scene_paint(void)
{
    style_infor_t style;
    picbox_private_t picbox_param;

    style.type = UI_AP;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = 0;
    //ui_show_pic(P_ALARM_BG,0,0);
    ui_show_pic(P_ALARM1 + alarm_pid, 23, 32);
    if(g_alarm_pid == FALSE)
    {
        alarm_pid --;
        if(alarm_pid <= 0)
        {
            g_alarm_pid = TRUE;
        }
    }
    else
    {
        alarm_pid ++;
        if(alarm_pid >= 4)
        {
            g_alarm_pid = FALSE;
        }
    }
}
