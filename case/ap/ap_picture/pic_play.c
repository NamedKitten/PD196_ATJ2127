/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     pic_play.c
 * \brief    Picture��play����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include  "picture.h"

/*! \brief �������밴���¼��Ķ�Ӧ��ϵ*/
const key_map_t pic_play_key_map_list[] =
{
    /*! next�¼� */
    {{ KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_DOWN | KEY_TYPE_HOLD },  EVENT_PICTURE_PLAYING_NEXT },
    /*! prev�¼� */
    {{ KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_DOWN | KEY_TYPE_HOLD },  EVENT_PICTURE_PLAYING_PREV },
    /*! �̰�play�¼� */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP },  EVENT_PICTURE_PLAYING_PAUSE},
    /*! �̰�option�¼� */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_PICTURE_OPTION },
    /*! �̰�vol�����¼� */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_PICTURE_RETURN },
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};
/******************************************************************************/
/*!
 * \par  Description:
 *    �Զ����Ŷ�ʱ���ص�����
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      pic_play
 * \note
 * \li  ֻ���ñ�־λ������ѭ���л��ļ�
 *******************************************************************************/
void auto_play_flag(void)
{
    g_picture_change_file = up_picture_playnext;
    //��ֹͣtimer��ʱ�������ļ�����������ٿ���
    stop_app_timer(timer_pic_play);
}

#if 0
static void pic_show_headbar(void)
{
    style_infor_t picture_style;
    numbox_private_t picture_numbox_param;

    picture_style.type = UI_AP;
    picture_style.style_id = STY_PIC_NUM_NUMBERBOX;

    picture_numbox_param.total = g_picture_var.path.file_path.dirlocation.file_total;
    picture_numbox_param.value = g_picture_var.path.file_path.dirlocation.file_num;
    //    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, this_headbar_icon_id);
    ui_show_numbox(&picture_style, &picture_numbox_param, NUMBOX_DRAW_ALL);
}
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *    ͼƬ�ļ��л�ģ��
 * \param[in]    void
 * \param[out]   void
 * \return       bool  �л��Ƿ�ɹ�
 * \retval       TRUE  �ɹ�  FALSE ʧ��
 * \ingroup      pic_play
 * \note
 * \li   ����enhance��basal�ȸ��ã�������ļ��л�֮ǰ����Ҫ��ѯ������Ƿ���ڣ�
 *       ��������Ӧ�رս������ٵ���enhance�ӿ�
 *******************************************************************************/
bool pic_change_file(void)
{
    bool result = FALSE;
    uint8 *file_name = g_picture_var.path.file_path.dirlocation.filename;

    pic_decoder_free();

    if ((g_picture_change_file & up_picture_playprev) == up_picture_playprev)
    {
        result = fsel_get_prevfile(file_name);
    }
    else
    {
        result = fsel_get_nextfile(file_name);
    }

    if (result == TRUE)
    {
        fsel_get_location((void *) &g_picture_var.path.file_path, g_picture_var.path.file_source);
    }
    g_picture_change_file = (uint8)(g_picture_change_file & clr_picture_changefile);
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ͼƬ���Ž�����ӦGUI�¼�����
 * \param[in]   cur_event��GUI�¼�
 * \param[in]   auto_play_flag:��ǰ�Ƿ�Ϊ�Զ�����״̬
 * \param[out]  none
 * \return      the result �����¼�����ֵ
 * \ingroup     pic_play
 * \note
 *******************************************************************************/
app_result_e pic_play_proc_key_event(msg_apps_type_e gui_event)
{
    app_result_e ui_result = RESULT_NULL;

    switch (gui_event)
    {
        case EVENT_PICTURE_PLAYING_NEXT:
        g_picture_change_file = up_picture_playnext;
        if (g_picture_auto_play == TRUE)
        {
            //��ֹͣtimer��ʱ�������ļ��������������Ƿ���
            stop_app_timer(timer_pic_play);
        }
        break;

        case EVENT_PICTURE_PLAYING_PREV:
        g_picture_change_file = up_picture_playprev;
        if (g_picture_auto_play == TRUE)
        {
            //��ֹͣtimer��ʱ�������ļ��������������Ƿ���
            stop_app_timer(timer_pic_play);
        }
        break;

        case EVENT_PICTURE_RETURN:
        ui_result = RESULT_PIC_RETURN;
        break;

        case EVENT_PICTURE_OPTION:
        if (g_picture_auto_play == FALSE)
        {
            ui_result = RESULT_PIC_OPTIONMENU;
        }
        else
        {
            /*�Զ����Ų���Ӧmode����*/
            ui_result = RESULT_NULL;
        }
        break;

        case EVENT_PICTURE_PLAYING_PAUSE:
        g_picture_auto_play ^= 0x01;
        if (g_picture_auto_play == TRUE)
        {
            //�����Զ��л�ͼƬ�Ķ�ʱ��
            restart_app_timer(timer_pic_play);
            change_app_state(APP_STATE_PLAYING_ALWAYS);
        }
        else
        {
            stop_app_timer(timer_pic_play);
            change_app_state(APP_STATE_NO_PLAY);
        }
        break;

        default:
        //�����ǰ�߳�δ�رգ���Ӧ�ȼ���Ϣǰ�ȹر��̣߳�������
        //����ֵȷ���Ƿ���Ҫ����ˢͼ
        if(g_mmm_id_free == FALSE)
        {
            pic_decoder_free(); 
            ui_result = com_message_box(gui_event);
            if(ui_result == RESULT_NULL)
            {
                //���½���ͼƬ����
                ui_result = RESULT_REDRAW;
            }           
        }
        else
        {
            ui_result = com_message_box(gui_event);    
        }        
        break;
    }
    return ui_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ������״̬
 * \param[in]    dec_status ����״̬
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess   ��Ҫ�˳�����
 * \retval       1 failed   ��������
 * \ingroup      pic_play
 * \note
 * \li  ʵ��ͼƬ�ֶ�/�Զ����ţ��ļ��л��ȹ���
 *******************************************************************************/
static bool _deal_decode_status(decode_status_e dec_status)
{
    bool ret_val = TRUE;

    switch (dec_status)
    {
        case DECODE_NO_ERR:
        g_file_err = 0;
        break;

        case DECODE_FILE_ERR:
        //���Զ�������ֱ�ӷ����б����
        //�Զ����Ŵ����ļ�����һ������Ҳ�᷵���б����
        if ((g_picture_auto_play == FALSE) || (g_file_err >= g_file_total))
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_NO_SUPPORT);
            ret_val = FALSE;
        }
        else
        {
            //�л�����
            g_file_err++;
        }
        break;

        case DECODE_UNSUPPORT_ERR:
        //�˳�����֧�ָ��ֲ���ģʽ
        //���Ϊ���Զ�����ģʽ����ֱ�ӷ���
        if (g_picture_auto_play == FALSE)
        {
            ret_val = FALSE;
        }
        else
        {
            //����һ��
            g_picture_change_file = up_picture_playnext;
        }
        break;

        default:
        ret_val = FALSE;
        break;
    }
    return ret_val;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  picture���ų�������
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \note
 * \li  ʵ��ͼƬ�ֶ�/�Զ����ţ��ļ��л��ȹ���
 *******************************************************************************/
app_result_e pic_play(void)
{
    bool result;
    decode_status_e dec_status;
    input_gui_msg_t ui_msg;
    private_msg_t private_msg;
    msg_apps_type_e gui_event;

    bool need_play = TRUE;
    bool is_scene_exit = FALSE;
    app_result_e ui_result = RESULT_NULL;

    //�����ʼ��
    if (pic_decode_init(0) == FALSE)
    {
        ui_result = RESULT_PIC_RETURN;
        is_scene_exit = TRUE;
    }

    while (!is_scene_exit)
    {
        if ((g_picture_change_file & up_picture_changefile) != 0)
        {
            if (pic_change_file() == TRUE)
            {
                //ע�����л��ļ�֮ǰ��Ҫ������ʾ��������
                ui_clear_screen(NULL);
                //gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
                need_play = TRUE;
            }
            else
            {
                //�л�����ʧ��
                ui_result = RESULT_PIC_RETURN;
                is_scene_exit = TRUE;
                continue;
            }
        }

        if (need_play == TRUE)
        {
            need_play = FALSE;

            g_file_total = g_picture_var.path.file_path.dirlocation.file_total;

            dec_status = pic_decode();

            if (_deal_decode_status(dec_status) == FALSE)
            {
                if ((g_picture_var.path.file_path.dirlocation.disk == DISK_H) && (sys_detect_disk(DRV_GROUP_STG_CARD)
                        == -1))
                {
                    //��ⲻ������
                    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
                    ui_result = RESULT_CARD_OUT_ERROR;
                }
                else
                {
                    ui_result = RESULT_PIC_RETURN;
                }
                is_scene_exit = TRUE;
                continue;
            }
        }

        //����⻹δ�ͷŵ��߳̽����ѽ���
        if ((g_mmm_id_free == FALSE) && (pic_check_decode() == TRUE))
        {
            //�ͷŸý����м����Դ
            pic_decoder_free();

            if (g_picture_auto_play == FALSE)
            {
                //ͼƬΪ��֡��û�б���
                if (((g_picture_userinfor.returnflag & RETURNFLAG_MULTIFRAME) != 0)
                        && (get_screensave_state() == FALSE))
                {
                    //�ظ���ʾ�ö�֡ͼƬ
                    need_play = TRUE;
                }
                else
                {
                    //���Զ�����״̬��ʾheadbar
                    //pic_show_headbar();
                }
            }
            else
            {
                restart_app_timer(timer_pic_play);
            }
        }

        result = get_gui_msg(&ui_msg);
        if (result == TRUE)//��ui��Ϣ
        {
            //gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
            if (com_key_mapping(&ui_msg, &gui_event, pic_play_key_map_list) == TRUE)
            {
                ui_result = pic_play_proc_key_event(gui_event);
            }
        }
        else
        {
            //����ap˽����Ϣ
            result = get_app_msg(&private_msg);
            if (result == TRUE)
            {
                //gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
                ui_result = pic_msg_callback(&private_msg);                
            }

        }

        switch (ui_result)
        {
            case RESULT_NULL:
            break;

            case RESULT_REDRAW:
            //�ػ浱ǰͼƬ
            need_play = TRUE;
            ui_result = RESULT_NULL;
            ui_clear_screen(NULL);
            break;

            default:
            if (g_picture_auto_play == TRUE)
            {
                change_app_state(APP_STATE_NO_PLAY);
            }
            is_scene_exit = TRUE;
            break;

        }

        //����20ms�����������
        sys_os_time_dly(2);
    }

    pic_decoder_free();

    kill_app_timer(timer_pic_play);
    change_app_state(APP_STATE_NO_PLAY);

    if (ui_result == RESULT_PIC_RETURN)
    {
        if (g_picture_enter_mode == PARAM_FROM_MAINMENU)
        {
            ui_result = RESULT_PIC_FILELIST;
        }
        else
        {
            ui_result = RESULT_RETURN_BROWSER;
        }
    }

    if ((g_need_resume_engine == TRUE) && (ui_result != RESULT_APP_QUIT))
    {
        g_need_resume_engine = FALSE;
        music_resume_engine();
    }

    return ui_result;
}
