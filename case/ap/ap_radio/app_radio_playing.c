/*******************************************************************************
 *                              US212A
 *                            Module: radio_ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeayng    2011-09-14          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

extern app_result_e deal_play_guimsg(msg_apps_type_e cur_event, radio_playwin_mode_e mode)
__FAR__;
extern bool scene_play_init(void)
__FAR__;
extern bool scene_play_exit(void)
__FAR__;
extern bool get_default_name(void)
__FAR__;
extern bool show_station_info(bool need_show)
__FAR__;

/* Ƶ������ͼƬ֮�����������ֵ*/
const uint8 pos_x_inc[5] =
{ PIC_X_INC1, PIC_X_INC2, PIC_X_INC3, PIC_X_INC4, PIC_X_INC5 };

/* radio ���ų��������¼�ӳ��*/
const key_map_t playing_key_map_list[] =
{
    /*! �̰�NEXT ����һƵ���¼�*/
    {{ KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_FREQ },
    /*! �̰�PREV ����һƵ���¼� */
    {{ KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PREV_FREQ },
    /*! ����NEXT �������Զ���̨�¼�*/
    {{ KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SEEK_NEXT },
    /*! ����PREV ����ǰ���Զ���̨�¼� */
    {{ KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SEEK_PREV },
    /*! �̰�option�¼� */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_OPTION },
    /*!�̰�play ��һ��̨�¼� */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_PRESET },
    /*! �̰�KEY_VOL ����ת��Ϊ �����¼� */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! ���һ��ӳ���¼� */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  scene_play_sysmsg(void)
 * \���ų���ϵͳ��Ϣ��˽����Ϣ����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e scene_play_sysmsg(void)
{
    private_msg_t play_msg;
    app_result_e msg_result = RESULT_NULL;
    if (get_app_msg(&play_msg) == TRUE)
    {
        msg_result = radio_app_msg_callback(&play_msg);
    }
    return msg_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void scroll_station_info(void)
 * \�û���̨����ģʽ��, ��̨���ƹ�����ʾ
 * \param[in]    none
 * \param[out]   none
 * \return
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
void scroll_station_info(void)
{
    style_infor_t info_style;
    textbox_private_t info_textbox_param;

    //��ʾ��̨����
    info_style.style_id = TEXTBOX_PRESET_INFOR;
    info_style.type = UI_AP;
    //��������뷽ʽ
    if (encode_mode == ENCODE_UNICODE)
    {
        info_textbox_param.lang_id = UNICODEDATA;
    }
    else
    {
        info_textbox_param.lang_id = (int8) g_comval.language_id;
    }
    info_textbox_param.str_id = -1;
    info_textbox_param.str_value = g_user_station.station_name;
    ui_show_textbox(&info_style, &info_textbox_param, TEXTBOX_DRAW_SCROLL);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool freq_change(uint8* buf,  uint16 freq, uint8 mode)
 * \�������Ƶ��ֵת��Ϊascill
 * \param[in]    freq--����Ƶ��ֵ; mode--��ʾ��ԴͼƬ�����ַ���
 * \param[out]   buf
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
bool freq_change(uint8* buf, uint16 freq, uint8 mode)
{
    uint32 true_freq;
    uint16 i, j;
    uint8 k, m;
    uint8 temp_buf[3];

    //��ת����ʵ��Ƶ��, khz Ϊ��λ
    true_freq = (uint32)((1 << 16) + freq);
    i = (uint16)(true_freq / 100);
    k = (uint8)(true_freq % 100);
    //�ٷ�λ
    k = k / 10;
    //����λ
    j = i / 10;
    //С��λ
    m = (uint8)((i % 10) * 10 + k);

    if (j < 100)
    {
        libc_itoa(j, buf, 2);
    }
    else
    {
        libc_itoa(j, buf, 3);
    }

    if (mode == 0)
    {
        //��ԴͼƬ��ʾʱ
        libc_strncat(buf, ":", 1);
    }
    else
    {
        //�ַ�����ʾʱ
        libc_strncat(buf, ".", 1);
    }
    libc_itoa(m, temp_buf, 2);
    temp_buf[2] = '\0';
    libc_strncat(buf, temp_buf, libc_strlen(temp_buf));
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void ui_show_FMdata(char *buf)
 * \���ų����µ�Ƶ��ͼƬˢ��
 * \param[in]    buf -- ��ת��Ϊascill ���Ƶ���ִ�
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
//void ui_show_FMdata(char *buf)
//{
//    uint8 FMFreNum = (uint8) libc_strlen(buf); //������������
//    style_infor_t style_infor;
//    uint16 x, y;
//    uint8 *p_xinc;
//
//    style_infor.type = UI_AP;
//    if (FMFreNum != FMFreNumOld)
//    {
//        //λ�������仯�����屳��
//        FMFreNumOld = FMFreNum;
//        style_infor.style_id = PICBOX_FREQ_BG;
//        ui_show_picbox(&style_infor, (void*) NULL);
//    }
//    if (FMFreNum < 6)
//    {
//        //<100MHz
//        x = PIC_X_F1START;
//        p_xinc = (uint8 *) &pos_x_inc[1];
//    }
//    else
//    {
//        //>=100MHz
//        x = PIC_X_F1START_1;
//        p_xinc = (uint8 *) &pos_x_inc[0];
//    }
//    y = PIC_Y_F1START;
//    while (1)
//    {
//        if (*buf != 0)
//        {
//            ui_show_pic(FMNUM0 + *buf - 0x30, x, y);
//        }
//        else
//        {
//            break;
//        }
//        buf++;
//        x += (*p_xinc);
//        p_xinc++;
//    }
//
//    //��ʾFM ͼƬ
//    x += 14;
//    ui_show_pic(FM_FM, x, 49);
//    return;
//}

/******************************************************************************/
/*!
 * \par  Description:
 * \void ui_show_freqbar(uint16 freq)
 * \���ݵ�ǰƵ�ʼ����Σ���ʾ���ų����µ�tune bar
 * \param[in]    freq
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void ui_show_freqbar(uint16 freq, uint8 mode)
{
    uint32 true_freq;
    style_infor_t style_infor;
    progressbar_private_t freq_progress;

    //������Ƶ��ֵ
    true_freq = (uint32)((1 << 16) + freq);

    //���屳��
    style_infor.type = UI_AP;
    style_infor.style_id = PBAR_BAND;

    //76M~~90M
    if (g_radio_config.band_mode == Band_Japan)
    {
        if (true_freq < 76000)
        {
            true_freq = 0;
        }
        else
        {
            true_freq -= 76000;
            true_freq /= 100;
        }
        freq_progress.value = (uint16)true_freq;
        freq_progress.total = 140;
    }
    //�������η�Χ87.5M~~108M
    else
    {
        if (true_freq < 87500)
        {
            true_freq = 0;
        }
        else
        {
            true_freq -= 87500;
            true_freq /= 100;
        }
        freq_progress.value = (uint16)true_freq;
        freq_progress.total = 205;
    }

    ui_show_progressbar(&style_infor, &freq_progress, mode);
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void show_freq_in_play(void)
 * \���ų����µ�Ƶ��ͼƬˢ��
 * \param[in]     none
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void show_freq_in_play(void)
{
    style_infor_t numbox_style;
    numbox_private_t numbox_param;
    uint32 true_freq = (uint32)((1 << 16) + g_radio_config.FMStatus.freq);

    //��ǰƵ��ֵת��Ϊascill ����
    //libc_memset(Freqtab, 0, sizeof(Freqtab));
    //freq_change(Freqtab, g_radio_config.FMStatus.freq, 0);
    //ui_show_FMdata(Freqtab);

    numbox_style.style_id = NUMBOX_FREQ;
    numbox_style.type = UI_AP;
    numbox_param.value = (int32)true_freq / 10;//��ʾ��10KHzΪ��λ
    numbox_param.total = 0;

    ui_show_numbox(&numbox_style, &numbox_param, NUMBOX_DRAW_NUMBER);

    ui_show_freqbar(g_radio_config.FMStatus.freq, PROGRESS_DRAW_PROGRESS);
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void show_station_num(void)
 * \���ų����µĵ�̨��ˢ��
 * \param[in]     none
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note    ���ô˺���ʱ����ȷ��g_radio_config.FMStatus.station
 * \          g_userlist_index ��g_userlist_total ��ȫ�ֱ�����ȷ
 */
/*******************************************************************************/
void show_station_num(void)
{
    style_infor_t style_infor;
    textbox_private_t station_textbox_param;
    uint8 buf[3]; //��̨��ת��Ϊascill ���buffer
    bool is_saved; //�Ƿ����ѱ����̨

    style_infor.type = UI_AP;

    if (g_radio_config.FMStatus.station != 0)
    {
        if (g_radio_config.band_mode != Band_MAX)
        {
            //Ԥ���̨ģʽ
            is_saved = TRUE;
        }
        else if (g_userlist_index < g_userlist_total)
        {
            //�û�ģʽ, ��Ҫ������С�ڵ�̨����
            is_saved = TRUE;
        }
        else
        {
            is_saved = FALSE;
        }
    }
    else
    {
        is_saved = FALSE;
    }

    if (is_saved == TRUE)
    {
        //���ѱ����̨
        style_infor.style_id = TEXTBOX_PRESET_NUM;
        libc_itoa(g_radio_config.FMStatus.station, &buf[0], 2);
        buf[2] = '\0';
        station_textbox_param.str_value = &buf[0];
    }
    else
    {
        //δ�����̨
        style_infor.style_id = TEXTBOX_NO_PRESET;
        station_textbox_param.str_value = "--";
        //��ǰ���ŵ���δ�����̨
        g_radio_config.FMStatus.station = 0;
    }
    station_textbox_param.lang_id = ANSIDATAAUTO;
    station_textbox_param.str_id = -1;
    ui_show_textbox(&style_infor, &station_textbox_param, TEXTBOX_DRAW_NORMAL);
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool get_station_info(void)
 * \��ȡ�û���̨������Ϣ
 * \param[in]    none
 * \param[out]   none
 * \return       1: �����û���̨����   0: �������û���̨����
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
bool get_station_info(void)
{
    bool ret;
    if (g_use_default == TRUE)
    {
        //ʹ��Ĭ�ϵ��û���̨, ��ȡĬ������
        ret = get_default_name();
    }
    else
    {
        /* ������û�ģʽ�£�����Ƶ�ʲ����ѱ����û���̨
         ��g_userlist_index =0xff,   g_radio_config.FMStatus.station = 0  */
        if (g_userlist_index != 0xff)
        {
            //��������ȡ����̨����
            radio_parse_userlist(g_user_station.station_name, INDEX_FROM_START, g_userlist_index);
            ret = TRUE;
        }
        else
        {
            ret = FALSE;
        }
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radioUI_scene_playing(radio_playwin_mode_e mode)
 * \����radio  ���ų�������
 * \param[in]    mode
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e radioUI_scene_playing(radio_playwin_mode_e mode)
{
    bool result;
    app_result_e ret = RESULT_IGNORE;
    style_infor_t style_infor;
    bool need_fetch = TRUE; //��Ҫȡ�û���̨����
    bool have_info = FALSE; //�Ƿ����û���̨������ʾ

    //��ȡgui��Ϣ
    input_gui_msg_t gui_msg;
    //gui ��Ϣ��Ӧgui �¼�
    msg_apps_type_e gui_event;

    result = scene_play_init();
    if (!result)
    {
        if (timer_flush_radioinfo != -1)
        {
            kill_app_timer(timer_flush_radioinfo);
            timer_flush_radioinfo = -1;
        }
        //����Ϊ�ǲ���״̬
        change_app_state(APP_STATE_NO_PLAY);
        //return RESULT_SCENE_EXIT;
        return RESULT_RADIO_MAINMENU;
    }

    style_infor.type = UI_AP;
    paint_flag |= (uint8) PAINT_ALL;

    //���õ�ǰƵ��
    if ((g_playwin_mode != ENTER_PLAY_FROM_NOWPLAY) && (need_set_freq))
    {
        radio_get_freq();

        //��ǰ��ȡ��Ƶ���Ҫ���õ�Ƶ�㲻һ�»��ߵ�һ�ο���radio����ʱ������Ҫǿ������Ƶ��
        if ((g_radio_config.FMStatus.freq != g_engine_status.FM_CurrentFreq) || (g_first_open_engine == TRUE))
        {
            //ѡ�����ڲ��Ž���, ������Ƶ��, �������һ�¶�������
            radio_set_freq(g_radio_config.FMStatus.freq);
        }
    }

    need_set_freq = TRUE;
    //radio ���ų���ѭ��
    while (1)
    {

        if (get_screensave_state() == FALSE)//��������ģʽ�Ż���ʾ
        {
            //ˢ��ȫ��
            if ((paint_flag & PAINT_ALL) == PAINT_ALL)
            {
                style_infor_t numbox_style;
                numbox_private_t numbox_param;
                uint32 true_freq = (uint32)((1 << 16) + g_radio_config.FMStatus.freq);

                //����
                style_infor.style_id = PICBOX_PLAY_BG;
                ui_show_picbox(&style_infor, (void*) NULL);

                //headbar
                gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_RADIO);

                numbox_style.style_id = NUMBOX_FREQ;
                numbox_style.type = UI_AP;
                numbox_param.value = (int32)true_freq / 10;//��ʾ��10KHzΪ��λ
                numbox_param.total = 0;
                ui_show_numbox(&numbox_style, &numbox_param, NUMBOX_DRAW_ALL);

                //FM�ֶ�
                ui_show_pic(FM_FM, 86, 49);

                //��״Ƶ����
                ui_show_freqbar(g_radio_config.FMStatus.freq, PROGRESS_DRAW_ALL);
                paint_flag &= CLEAR_PAINT_ALL;
            }

            //ˢ��Ƶ�������
            if ((paint_flag & PAINT_FREQ) != 0)
            {
                show_freq_in_play();
                show_station_num();
                paint_flag &= (uint8) CLEAR_PAINT_FREQ;
            }

            //���δ�������Ȳ��ţ��Ժ���ˢ�û���̨����
            if ((first_wait == TRUE) && (radio_get_antenna() == TRUE))
            {
                radio_set_mute(releaseMUTE);
                first_wait = FALSE;
            }

            //ˢ���û���̨�б��������
            if ((g_radio_config.band_mode == Band_MAX) && ((paint_flag & PAINT_STATION_INFOR) != 0))
            {

                //�û�ģʽ�£��л�Ƶ�ʺ�������ȡ��̨����
                if (need_fetch == TRUE)
                {
                    have_info = get_station_info();
                    need_fetch = FALSE;
                }
                show_station_info(have_info);
                paint_flag &= (uint8) CLEAR_PAINT_INFOR;
            }
        }

        //��ȡ�ʹ���GUI��Ϣ��˽����Ϣ
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {

            if (com_key_mapping(&gui_msg, &gui_event, playing_key_map_list) == TRUE)
            {
                //if(gui_event != EVENT_RADIO_PLAY_MUTE)
                {
                    //��������ͣ�¼��⣬����������ȡ��̨����
                    need_fetch = TRUE;
                }
                ret = deal_play_guimsg(gui_event, mode);
            }
        }
        else
        {
            //һ��ѭ������һ�����͵���Ϣ
            ret = scene_play_sysmsg();
        }
        if ((ret != RESULT_IGNORE) && (ret != RESULT_NULL) && (ret != RESULT_REDRAW))
        {
            break;
        }
        else if (ret == RESULT_REDRAW)
        {
            //��ˢȫ��
            paint_flag |= (uint8) PAINT_ALL;
        }
        else
        {
        }

        //����20ms �����񽻻�
        sys_os_time_dly(2);
    }
    //���ų����˳�
    scene_play_exit();

    return ret;
}

