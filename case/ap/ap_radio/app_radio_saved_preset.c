/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-23          1.0              create this file
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
extern app_result_e deal_station_del(radiolist_control_t *p_ml_control)
__FAR__;

/* �ִ�������󳤶ȶ����*/
#define  MAX_STRING   50

/* Ԥ���б�listbox  ˽�нṹ*/
//static listbox_private_t savelist_private _BANK_DATA_ATTR_;

/* �ִ�����buffer */
uint8 string_buf[MAX_STRING] _BANK_DATA_ATTR_;

/*!
 * \brief
 *  savelist_key_map_list��Ԥ���̨�б���ӳ���
 */
const key_map_t savelist_key_map_list[] =
{
    /*! PREV ����ת��Ϊ SELECT_PREV �¼� */
    {{ KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SELECT_PREV },
    /*! NEXT ����ת��Ϊ SELECT_NEXT �¼� */
    {{ KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SELECT_NEXT },
    /*! �̰�KEY_PLAY ����ת��Ϊ SELECT_CUR �¼� */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_SELECT_CUR },
    /*! �̰�KEY_VOL ����ת��Ϊ RETURN �¼� */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! �̰�KEY_MODE ����ת��Ϊ OPTION �¼� */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_OPTION },
    /*! ������־ */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  string_joint_save(uint8 station_num, uint8 mode)
 * \param[in]    station_num  ��̨��  mode   ѯ�ʻ��ǳɹ�����ʾ
 * \param[out]   none
 * \return      none
 * \retval
 * \retval
 * \note   �����̨ʱ�ַ���ƴ�Ӵ���
 */
/*************************************************************************************************/
app_result_e string_joint_save(uint8 station_num, uint8 mode)
{
    string_desc_t string_desc;
    string_desc_t string_source[2];
    uint8 tmp_buf[7];
    //uint8 tmp;
    dialog_com_data_t dialog_com;
    app_result_e ret;
    uint16 style_id;

    libc_memset(tmp_buf, 0, sizeof(tmp_buf));
    libc_memset(string_buf, 0, sizeof(string_buf));

    //��̨��ת��ascill
    libc_itoa(station_num, &tmp_buf[0], 2);

#if 0    //itoa �Ѿ�����ת��
    if(station_num <10)
    {
        //ֻ��һλʱ��ת�����轻��˳��
        tmp = tmp_buf[0];
        tmp_buf[0] = tmp_buf[1];
        tmp_buf[1] = tmp;
    }
#endif

    //Դ�ִ�--- ��̨�����
    string_source[1].data.str = tmp_buf;
    string_source[1].length = 3;
    string_source[1].language = ANSIDATAAUTO;

    //��ǰ�豣��Ƶ��--- ����ǰ����Ƶ��תΪascill
    libc_memset(Freqtab, 0, sizeof(Freqtab));
    freq_change(Freqtab, g_radio_config.FMStatus.freq, 1);
    //Դ�ִ�--- Ƶ��ֵ���
    string_source[0].data.str = Freqtab;
    string_source[0].length = 7;
    string_source[0].language = ANSIDATAAUTO;

    //Ŀ���ִ�
    string_desc.data.str = string_buf;
    string_desc.length = MAX_STRING;
    if (mode == 0)
    {
        //�Ƿ񱣴�
        string_desc.argv = CON_SAVE;
    }
    else
    {
        //�ɹ�����
        string_desc.argv = SUC_SAVE;
    }
    string_desc.language = (int8) g_comval.language_id;

    com_string_format(&string_desc, &string_source[0]);

    //ѯ�ʣ����а�����dialog
    if (mode == 0)
    {
        dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
        dialog_com.button_type = BUTTON_YESNO;
        dialog_com.active_default = BUTTON_YESNO_YES;
        style_id = DIALOG_ASK;
    }
    //�ɹ�����ʾ����������
    else
    {
        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        style_id = DIALOG_MSG;
    }
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    ret = gui_dialog(style_id, &dialog_com);
    return ret;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e do_save_process(uint8 num)
 * \param[in]    num  ��̨��
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   ��̨���洦������ǰ����Ƶ�㱣�浽�˵�����ָ���б�
 */
/*************************************************************************************************/
app_result_e do_save_process(uint8 num)
{
    //�����ص����ų���
    app_result_e result = RESULT_RADIO_START_PLAY;

    if ((num > MAX_STATION_COUNT) || (num == 0))
    {
        //��̨�ų���Χ
        return result;
    }

    if (g_menu_band == Band_Japan)
    {
        g_radio_config.fmstation_jp[num - 1] = g_radio_config.FMStatus.freq;
    }
    else if (g_menu_band == Band_Europe)
    {
        g_radio_config.fmstation_eu[num - 1] = g_radio_config.FMStatus.freq;
    }
    else
    {
        g_radio_config.fmstation_us[num - 1] = g_radio_config.FMStatus.freq;
    }

    //������û�ģʽ���л���Ԥ��ģʽ
    g_radio_config.band_mode = g_menu_band;
    //�����б�ָ��
    init_point_freq();
    //���µ�̨��
    g_radio_config.FMStatus.station = num;
    //���±����ĵ�̨����
    g_radio_config.FMStatus.station_count = get_station_count();

    //����ɹ���ʾ
    string_joint_save(num, 1);
    return result;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_station_save(radiolist_control_t *p_ml_control)
 * \param[in]    p_ml_control
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   ���浽Ԥ�账��
 */
/*************************************************************************************************/
app_result_e deal_station_save(radiolist_control_t *p_ml_control)
{
    uint8 station_num;
    app_result_e ret = RESULT_NULL;

    //����λ�ã���̨��
    station_num = p_ml_control->list_no + 1;

    ret = string_joint_save(station_num, 0);

    if (ret == RESULT_DIALOG_YES)
    {
        //ȷ�ϱ���
        ret = do_save_process(station_num);
    }
#if 0	
    else
    {
        //ȡ�����棬�ػ��б�
        ret = RESULT_REDRAW;
    }
#endif	
    return ret;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e gui_event_process(msg_apps_type_e event, radio_list_entry_e entry, uint8 index)
 * \param[in]    entry   ��̨�б����ģʽ��Ӱ�찴play ����Ӧ
 * \param[in]    index   ���ĸ����Ԥ��Ĳ˵����룬Ӱ�찴return ����Ӧ
 * \param[in]    event   �账����¼�
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   avoid qac warning
 */
/*************************************************************************************************/
app_result_e gui_event_process(msg_apps_type_e event, radio_list_entry_e entry, uint8 index)
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
        //������ʾ���룬ѡ���̨����
        if (entry == ENTER_NORMAL)
        {
            //ѡ����Ԥ���̨���ţ����²���ģʽ
            g_radio_config.band_mode = g_menu_band;
            select_current_play(&list_control);
            result = RESULT_RADIO_START_PLAY;
        }
        //���浽Ԥ����룬�������
        else if (entry == ENTER_ADD_STATION)
        {
            result = deal_station_save(&list_control);
            if (result == RESULT_REDRAW)
            {
                list_control.draw_mode = LIST_DRAW_ALL;
            }
        }
        //���Ԥ����룬�������
        else //if (entry_mode == ENTER_DEL_STATION)
        {
            result = deal_station_del(&list_control);
            if (result == RESULT_REDRAW)
            {
                list_control.draw_mode = LIST_DRAW_ALL;
            }
        }
        //else
        //{
        //    ;
        //}
        break;

        case EVENT_RADIO_RETURN:
        if (entry == ENTER_NORMAL)
        {
            //����radio ���˵�
            result = RESULT_RADIO_MAINMENU;
        }
        else if (entry == ENTER_ADD_STATION)
        {
            //���ز��ų�����option
            result = RESULT_RADIO_OPTION_PLAYWIN;
        }
        else //if (entry_mode == ENTER_DEL_STATION)
        {
            //��Ԥ���б��option ��ѡ���������
            if (index == 1)
            {
                result = RESULT_RADIO_OPTION_STATION;
            }
            //�Ӳ��ų�����option ��ѡ���������
            else if (index == 2)
            {
                result = RESULT_RADIO_OPTION_PLAYWIN;
            }
            //��������radio ���˵�
            else
            {
                result = RESULT_RADIO_MAINMENU;
            }
        }
        //else
        //{
        //    ;
        //}
        break;

        case EVENT_RADIO_OPTION:
        if (entry == ENTER_NORMAL)
        {
            //�����̨�б�option ����
            result = RESULT_RADIO_OPTION_STATION;
        }
        //ɾ��Ԥ��򱣴�Ԥ�����������Ӧoption ��
        break;

        default:
        result = com_message_box(event);
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
 * \app_result_e show_saved_station(radio_list_entry_e  entry_mode, uint8 from)
 * \param[in]    entry_mode   ��̨�б����ģʽ��Ӱ�찴play ����Ӧ
 * \param[in]    from    ���ĸ����Ԥ��Ĳ˵����룬Ӱ�찴return ����Ӧ
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   Ԥ���̨�б��ˢ����ʾ
 */
/*************************************************************************************************/
app_result_e show_saved_station(radio_list_entry_e entry_mode, uint8 from)
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

    //��ʼ����������б�ָ��
    if (g_menu_band == Band_China_US)
    {
        pfreq_list = &g_radio_config.fmstation_us[0];
    }
    else if (g_menu_band == Band_Japan)
    {
        pfreq_list = &g_radio_config.fmstation_jp[0];
    }
    else
    {
        pfreq_list = &g_radio_config.fmstation_eu[0];
    }

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
            if (com_key_mapping(&input_msg, &gui_event, savelist_key_map_list) == TRUE)
            {
                result = gui_event_process(gui_event, entry_mode, from);
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

