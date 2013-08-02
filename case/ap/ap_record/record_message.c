/*******************************************************************************
 *                              US212A
 *                            Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       leiming   2011-10-13           1.0             build this file
 *******************************************************************************/
/*!
 * \file     record_message.c
 * \brief    record ����Ϣ������ģ��
 * \author   leiming
 * \version  1.0
 * \date  2011/10/13
 *******************************************************************************/
//#include "ap_com_rec_voice.h"
#include  "ap_record.h"

const key_map_t recording_key_map_list[] =
{
    /*!�̰�play����/��ͣ�¼� */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PAUSE },
    /*! �̰�KEY_RETURN ����ת��Ϊ �����¼� */
    { { KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/******************************************************************************/
/*!
 * \par  Description:
 * \int record_message_deal(private_msg_t *msg)
 * \Record ��ȡ˽����Ϣ�Ĵ���
 * \param[in]    private_msg_t *msg
 * \param[out]   none
 * \return               app_result_e
 * \retval
 * \ingroup      record_message.c
 * \note
 */
/*******************************************************************************/
app_result_e record_message_deal(private_msg_t *msg)
{
    app_result_e app_ret;

    app_ret = RESULT_NULL;
    switch (msg->msg.type)
    {
        case MSG_APP_QUIT: //�˳�app
        app_ret = RESULT_APP_QUIT;
        break;

        case MSG_SD_OUT:
        if(g_scene_param.record_scene == SCENE_MENU_REC)
        {
            if((g_record_vars.path.file_path.dirlocation.disk == DISK_H) 
                && (need_fsel_init == FALSE))
            {
                g_record_vars.path.file_path.dirlocation.disk = DISK_C;
                fsel_exit();
                need_fsel_init = TRUE;               
                gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
                app_ret = RESULT_RECORD_MEMU;
            }
        }
        break;

        default: //Ĭ��ϵͳ��Ϣ��messageBox����
        app_ret = com_message_box(msg->msg.type);
        break;
    }

    return app_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \static app_result_e _recording_play_key(void)
 * \record����play����������
 * \param[in]    void
 * \param[out]   none
 * \return       app_result_e result
 * \retval       none
 * \retval       ......
 * \ingroup      record_message.c
 * \note
 */
/*******************************************************************************/
static app_result_e _recording_play_key(void)
{
    app_result_e app_ret = RESULT_NULL;
    int result;

    /*��ȡ�м��״̬*/
    result = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_GET_STATUS, (unsigned int) &g_mr_param.mr_status);
    if (result < 0)
    {
        return RESULT_ERROR;
    }

    if (MMM_MR_ENGINE_ERROR == g_mr_param.mr_status.status)
    {
        if (REC_PLAY_STATUS == g_scene_param.record_status)
        {
            _recording_stop();
            _recording_close();
            g_scene_param.record_result = RESULT_SCENE_EXIT;
            return RESULT_ERROR;
        }
    }

    //��ʼ����װ��
    if (g_mr_param.mr_status.status == MMM_MR_ENGINE_STOPPED)
    {
        app_ret = _recording_start();
        g_scene_param.display_bitmap &= up_recordPlayIcon;
    }
    else if (g_mr_param.mr_status.status == MMM_MR_ENGINE_PAUSED)
    {
        //if (g_scene_param.record_status == REC_PAUSE_STATUS)
        {
            g_scene_param.display_bitmap |= up_recordPlayIcon;
            //g_scene_param.display_bitmap |= clr_recordTime;//��ʱ����ʾ
            app_ret = _recording_resume(); //��ǰΪpause״̬����Ӧplay������resume״̬

        }
    }
    else if (g_mr_param.mr_status.status == MMM_MR_ENGINE_ENCODING)
    {
        //���ݵ�ǰ����״̬
        //if (g_scene_param.record_status == REC_PLAY_STATUS)
        {
            g_scene_param.display_bitmap &= clr_recordPlayIcon;
            g_scene_param.display_bitmap |= up_recordPauseIcon;
            app_ret = _recording_pause(); //��ǰΪplay״̬����Ӧplay������pause״̬
        }
    }
    else
    {
    }

    g_scene_param.paint_flag = TRUE;
    return app_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e ui_show_save_dlg(void)
 * \��ʾ¼���ļ�����Ի���
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       none
 * \ingroup      ap_record_paint.c
 * \note
 */
/*******************************************************************************/
static app_result_e ui_show_save_dlg(void)
{
    dialog_com_data_t dialog_data;
    string_desc_t string_desc;
    app_result_e app_ret;

    string_desc.data.id = STR_RECORD_SAVETXT; //��ʾ���ݵĶ�������ַ���
    string_desc.language = UNICODEID; //�Ի��򳡾���unicode ��Դ�ַ���

    //����record save dialog
    dialog_data.dialog_type = DIALOG_BUTTON_INCLUDE; //�������ĶԻ���
    dialog_data.active_default = 0; //Ĭ�ϲ�����
    dialog_data.button_type = BUTTON_YESNOCANCEL; //"yes/no"
    dialog_data.string_desc = &string_desc; //�Ի������Դ�ַ���
    dialog_data.icon_id = -1;

    app_ret = gui_dialog(DIALOG_ASK, &dialog_data);
    return app_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \static app_result_e _recording_return_key(void)
 * \¼��return��������
 * \param[in]    void
 * \param[out]   none
 * \return       app_result_e result
 * \retval       none
 * \ingroup      record_message.c
 * \note
 */
/*******************************************************************************/
static app_result_e _recording_return_key(void)
{
    app_result_e app_ret = RESULT_NULL;
    bool need_resume = TRUE;

    if (g_scene_param.record_status != REC_PAUSE_STATUS)
    {
        _recording_pause();
    }
    else
    {
        need_resume = FALSE;
    }

    //����record save dialog
    app_ret = ui_show_save_dlg();

    //����dialog���ؽ���������ļ��򷵻�¼��
    if (app_ret == RESULT_DIALOG_YES)
    {
        //ȷ�ϱ���¼���ļ����ȹر�¼��
        //_recording_resume();
        _recording_stop();
        app_ret = _recording_close();
        //¼���ļ����ļ�������1
        g_file_total++;
        if (app_ret == RESULT_MAIN_APP)
        {
            return app_ret;
        }

        if (g_entry_mode == PARAM_FROM_MAINMENU)
        {
            //�����desktop����ap����¼��һ�׺󣬷��ص�menu����
            app_ret = RESULT_RECORD_MEMU;
        }
        else if (g_entry_mode == PARAM_FROM_RADIO)
        {
            //�����fm����ap�����˳�Ӧ�÷��ص�fm
            app_ret = RESULT_RADIO_PLAY;
        }
        else
        {
            ;//QAC need
        }
    }
    else if (app_ret == RESULT_DIALOG_NO)
    {
        //������¼���ļ�ʱ���ȹر�¼��
        //_recording_resume();
        _recording_stop();
        //��ɾ���ļ���Ȼ���ٹر��ļ�
        vfs_file_dir_remove(g_fs_param.vfs_handle, g_fs_param.rec_filename, 1);
        app_ret = _recording_close();

        if (g_entry_mode == PARAM_FROM_MAINMENU)
        {
            //�����ļ���ʱg_rec_num�Ѿ�����1�����Բ�����ʱ��Ҫ��1
            g_rec_num--;
            //�����desktop����ap����¼��һ�׺󣬷��ص�menu����
            app_ret = RESULT_RECORD_MEMU;
        }
        else if (g_entry_mode == PARAM_FROM_RADIO)
        {
            //�����ļ���ʱg_fmrec_num�Ѿ�����1�����Բ�����ʱ��Ҫ��1
            g_fmrec_num--;
            //�����fm����ap�����˳�Ӧ�÷��ص�fm
            app_ret = RESULT_RADIO_PLAY;
        }
        else
        {
            ;//QAC need
        }
    }
    else if (app_ret == RESULT_REDRAW)
    {
        SetFullInterface()
        if (need_resume == TRUE)
        {
            app_ret = _recording_resume();
            if (app_ret == RESULT_NULL)
            {
                app_ret = RESULT_REDRAW;
            }
        }
    }
    else//other ֱ�ӷ���

    {
    }
    //��ʱfm¼����������
    if ((app_ret == RESULT_RADIO_PLAY) && (g_record_from_alarm != 0))
    {
        app_ret = RESULT_ALARM_QUIT;
    }
    return app_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_gui_record(msg_apps_type_e msg_type)
 * \record����GUI��Ϣ������
 * \param[in]    msg_apps_type_e msg_type
 * \param[out]   none
 * \return       app_result_e
 * \retval       none
 * \retval       ......
 * \ingroup      record_message.c
 * \note
 */
/*******************************************************************************/
app_result_e deal_gui_record(msg_apps_type_e msg_type)
{
    //key_event_t kmsg;
    app_result_e app_ret;

    app_ret = RESULT_NULL;
    switch (msg_type)
    {
        case EVENT_PLAYING_PAUSE:
        app_ret = _recording_play_key();
        break;

        case EVENT_PLAYING_RETURN:
        app_ret = _recording_return_key();
        break;

        default:
        app_ret = com_message_box(msg_type);
        break;
    }

    return app_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \static app_result_e _scene_record_sysmsg(void)
 * \����gui��Ϣ
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       none
 * \retval       none
 * \ingroup      record_scene_recording.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_record_get_msg(void)
{
    uint16 ret;

    input_gui_msg_t gui_msg;
    msg_apps_type_e gui_event; //gui ��Ϣ��Ӧgui �¼�

    //get message
    private_msg_t record_msg;
    app_result_e app_ret = RESULT_NULL;

    /*��ȡ�ʹ���GUI��Ϣ*/
    ret = (uint16) get_gui_msg(&gui_msg);
    if ((ret == TRUE))
    {
        if (com_key_mapping(&gui_msg, &gui_event, recording_key_map_list) == TRUE)
        {
            app_ret = deal_gui_record(gui_event); //����¼��gui��Ϣ
        }
        else
        {
            ;//do nothing
        }
    }
    else //һ��ѭ������һ�����͵���Ϣ
    {
        ret = get_app_msg(&record_msg);

        if (ret == TRUE)
        {
            app_ret = record_message_deal(&record_msg);
        }
    }

    return app_ret;
}



