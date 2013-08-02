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

extern void scroll_station_info(void)
__FAR__;

/******************************************************************************/
/*!
 * \par  Description:
 * \bool scene_play_init(void)
 * \radio ���ų�����ʼ��
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval       success
 * \retval       fail
 * \note
 */
/*******************************************************************************/
bool scene_play_init(void)
{
    bool bret;

    bret = radio_engine_init();
    if (!bret)
    {
        return FALSE;
    }
#ifdef ALARMFM
    if (g_radio_from_alarm != 0)
    {
        radio_set_mute(releaseMUTE);
        //set_pa_volume((uint32) g_comval.volume_current);//alarmfm_volume);
    }
#endif
    //���벥�ų�������ʼ��Ƶ�ʱ�ָ��
    init_point_freq();

    //��ǰΪ�û�����ģʽ
    if (g_radio_config.band_mode == Band_MAX)
    {
        //���ݵ�̨�ţ���ȡ����
        if (g_radio_config.FMStatus.station != 0)
        {
            g_userlist_index = get_index_from_num(g_radio_config.FMStatus.station);
        }
        else
        {
            g_userlist_index = 0xff;
        }
    }
    //����Ϊ����״̬
    change_app_state(APP_STATE_PLAYING);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool scene_play_exit(void)
 * \radio ���ų����˳�
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval       success
 * \retval       fail
 * \note
 */
/*******************************************************************************/
bool scene_play_exit(void)
{
    //ɾ���û���̨���ƹ�����ʱ��
    if (timer_flush_radioinfo != -1)
    {
        kill_app_timer(timer_flush_radioinfo);
        timer_flush_radioinfo = -1;
    }
    //����Ϊ�ǲ���״̬
    change_app_state(APP_STATE_NO_PLAY);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool show_station_info(bool need_show)
 * \���ų������û���̨������ʾ
 * \param[in]     need_draw    �Ƿ��е�̨����Ҫ��ʾ
 * \param[out]   none
 * \return       success/fail
 * \retval
 * \retval
 * \note  need_draw = FALSE   û�е�̨������ʾ��ֻˢ����
 */
/*******************************************************************************/
bool show_station_info(bool need_show)
{
    style_infor_t info_style;
    textbox_private_t info_textbox_param;

    if (!need_show)
    {
        //ˢ����������ϴε�̨��
        ui_show_pic(TEXTBOX_PRESET_INFOR_BG, 21, 100);
        //����������رչ�����ʱ��
        if (timer_flush_radioinfo != -1)
        {
            kill_app_timer(timer_flush_radioinfo);
            timer_flush_radioinfo = -1;
        }
        return TRUE;
    }
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

    if (UI_NEED_SCROLL == (ui_result_e) ui_show_textbox(&info_style, &info_textbox_param, TEXTBOX_DRAW_NORMAL))
    {
        if (timer_flush_radioinfo == -1)
        {
            timer_flush_radioinfo = set_app_timer(APP_TIMER_ATTRB_UI, 80, scroll_station_info);
        }
    }
    else
    {
        //����������رչ�����ʱ��
        if (timer_flush_radioinfo != -1)
        {
            kill_app_timer(timer_flush_radioinfo);
            timer_flush_radioinfo = -1;
        }
    }
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool get_default_name(void)
 * \��ȡĬ�ϵ��û���̨����
 * \param[in]    none
 * \param[out]   none
 * \return       success/fail
 * \retval       none
 * \retval       none
 * \note   ʹ��Ĭ�ϵ��û���̨ʱ, ���ݵ�̨�Ż�ȡ����
 */
/*******************************************************************************/
bool get_default_name(void)
{
    uint8 buf[3];
    bool ret = FALSE;

    libc_memset(g_user_station.station_name, 0, 40);
    /* ��ǰ������Ƶ�����ѱ�����û���̨*/
    if (g_radio_config.FMStatus.station > 0)
    {
        //libc_memset(g_user_station.station_name, 0, 40);
        libc_memcpy(g_user_station.station_name, default_name, sizeof(default_name));
        libc_itoa(g_radio_config.FMStatus.station, &buf[0], 2); //01, 02, 03
        //ĿǰĬ���û���ֻ̨��3  ��
        g_user_station.station_name[4] = (char) buf[1]; //_CH01_   _CH02_   _CH03_
        ret = TRUE;
    }
    return ret;
}

