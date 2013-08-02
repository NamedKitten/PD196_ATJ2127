/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang    2011-09-19          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

extern void init_list_control(radiolist_control_t *p_list_control)
__FAR__;
extern void load_onepage_item_data(radiolist_control_t *list_control)
__FAR__;
extern void update_list_private(radiolist_control_t *p_ml_control, listbox_private_t *list_private)
__FAR__;
extern void radiolist_scroll_handle(void)
__FAR__;
extern void radiolist_select_next(radiolist_control_t *p_ml_control)
__FAR__;
extern void radiolist_select_prev(radiolist_control_t *p_ml_control)
__FAR__;
extern void select_current_play(radiolist_control_t *p_ml_control)
__FAR__;
//extern uint8 get_index_from_num(uint8 num)
//__FAR__;

/* �û��б�listbox  ˽�нṹ*/
//static listbox_private_t userlist_private _BANK_DATA_ATTR_;

/*!
 * \brief
 *  userlist_key_map_list���û���̨�б���ӳ���
 */
const key_map_t userlist_key_map_list[] =
{
    /*! PREV ����ת��Ϊ SELECT_PREV �¼� */
    {{ KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SELECT_PREV },
    /*! NEXT ����ת��Ϊ SELECT_NEXT �¼� */
    {{ KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SELECT_NEXT },
    /*! �̰�KEY_PLAY ����ת��Ϊ SELECT_CUR �¼� */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_SELECT_CUR },
    /*! �̰�KEY_VOL ����ת��Ϊ RETURN �¼� */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! ������־ */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/************************************************************************************************/
/*!
 * \par  Description:
 * \bool scene_list_init(void)
 * \param[in]    none
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note   ��̨�б�����ʼ��
 */
/*************************************************************************************************/
bool scene_list_init(void)
{
    //�б���ʾbuffer ����
    libc_memset(g_radiolist_item[0], 0, LIST_NUM_ONE_PAGE_MAX * LIST_STR_LENGTH);
    return TRUE;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \bool scene_list_exit(void)
 * \param[in]    none
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note   ��̨�б����˳�
 */
/*************************************************************************************************/
bool scene_list_exit(void)
{
    return TRUE;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_event(msg_apps_type_e event)
 * \param[in]    �账���¼�
 * \param[out]   none
 * \return      app_result_e �����Ϣ
 * \retval
 * \retval
 * \note   avoid qac warning
 */
/*************************************************************************************************/
app_result_e deal_event(msg_apps_type_e event)
{
    app_result_e result = RESULT_NULL;
    //GUI������Ϣ����
    switch (event)
    {
        case EVENT_RADIO_SELECT_NEXT:
        //�л�����һ����̨�б���
        radiolist_select_next(&list_control);
        break;

        case EVENT_RADIO_SELECT_PREV:
        //�л�����һ����̨�б���
        radiolist_select_prev(&list_control);
        break;

        case EVENT_RADIO_SELECT_CUR:
        //ѡ�����û���̨���ţ����±�Ҫ��Ϣ
        g_radio_config.band_mode = Band_MAX;
        //ѡ��ǰ��̨����
        select_current_play(&list_control);
        //���ݵ�̨�ţ���ȡ����
        if (g_radio_config.FMStatus.station != 0)
        {
            g_userlist_index = get_index_from_num(g_radio_config.FMStatus.station);
        }
        else
        {
            g_userlist_index = 0xff;
        }
        g_radio_config.FMStatus.station_count = g_userlist_total;
        result = RESULT_RADIO_START_PLAY;
        break;

        case EVENT_RADIO_RETURN:
        //����radio ���˵�
        result = RESULT_RADIO_MAINMENU;
        break;

        default:
        result = com_message_box(event);
        if (result == RESULT_ALARMFM) //���Ź�������fm alarm��Ϣ
        {
            g_radio_from_alarm = 1;
            alarmfm_in_playing();
            result = RESULT_REDRAW;
        }
        if (result == RESULT_REDRAW)
        {
            list_control.draw_mode = LIST_DRAW_ALL;
        }
        break;
    }
    return result;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e show_user_station(void)
 * \param[in]
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   �û���̨�б��ˢ����ʾ
 */
/*************************************************************************************************/
app_result_e show_user_station(void)
{
    style_infor_t style_infor;
    ui_result_e ui_result;
    int8 scroll_timer = -1;
    input_gui_msg_t input_msg; //��������Ϣ����gui��Ϣ
    msg_apps_type_e gui_event; //gui ��Ϣӳ���¼�
    private_msg_t private_msg; //˽����Ϣ
    app_result_e result = RESULT_NULL;

    //��̨�б�Ŀؼ����͹̶�
    style_infor.type = UI_COM;
    style_infor.style_id = MENULIST;

    list_control.draw_mode = LIST_DRAW_ALL;
    //���� listbox �ؼ����Բ�������ȡ�ؼ�һ����ʾ����
    ui_get_listbox_attrb(&style_infor, &list_control.one_page_count, 0);

    //��ʾheadbar
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_RADIO);

    //���Ϊ�û���̨�б�
    pfreq_list = &g_user_station.fmstation_user[0];

    //��ʼ�����ƽṹ
    init_list_control(&list_control);

    //�б�ѭ��
    while (1)
    {
        if (list_control.draw_mode != LIST_DRAW_NULL)
        {
            //����һҳ��ʾ��
            load_onepage_item_data(&list_control);
            //����listbox ˽�нṹ
            //update_list_private(&list_control, &userlist_private);
            update_list_private(&list_control, &radiolist_private);
            //��ʾ
            ui_result = (ui_result_e) (uint32) ui_show_listbox(&style_infor, &radiolist_private,
                    (uint32) list_control.draw_mode);
            if (ui_result == UI_NEED_SCROLL)
            {
                if (scroll_timer == -1)//δ����������
                {
                    scroll_timer = set_app_timer(APP_TIMER_ATTRB_UI, 80, radiolist_scroll_handle);
                }
            }
            else
            {
                if (scroll_timer != -1)//�Ѵ�����ɾ��
                {
                    kill_app_timer(scroll_timer);
                    scroll_timer = -1;
                }
            }
            list_control.draw_mode = LIST_DRAW_NULL;
        }

        //��ȡgui��Ϣ
        if (get_gui_msg(&input_msg) == TRUE)//��gui��Ϣ
        {
            //���а���ӳ��
            if (com_key_mapping(&input_msg, &gui_event, userlist_key_map_list) == TRUE)
            {
                result = deal_event(gui_event);
            }
        }
        else
        {
            //���ˣ�gui��Ϣ�����Ѿ��������
            //����ap˽����Ϣ��ϵͳ��Ϣ
            if (get_app_msg(&private_msg) == TRUE)
            {
                result = radio_app_msg_callback(&private_msg);
                if (result == RESULT_REDRAW)//������ʾ��ʾ�������ػ�UI
                {
                    list_control.draw_mode = LIST_DRAW_ALL;
                }
                else if (result != RESULT_NULL)
                {
                    goto menu_exit;
                }
                else
                {
                }
            }
        }

        //�˳���̨�б�
        if ((result != RESULT_NULL) && (result != RESULT_REDRAW))
        {
            goto menu_exit;
        }
        //����10ms�����������
        sys_os_time_dly(1);
    }

    menu_exit: if (scroll_timer != -1)//�Ѵ�����ɾ��
    {
        kill_app_timer(scroll_timer);
    }
    return result;
}

