/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        mikeyang       2011-09-26          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

#include "app_radio_menu_config.h"

/* Ƶ����ʾ��Դ����*/
const uint16 band_text_table[] =
{ BAND_CU, BAND_JP, BAND_EU };

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_return_list(void *param)
 * \Ԥ���̨�б���Ӳ˵���option �ص�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   �ص�Ԥ���̨�б�������ʾ
 */
/*******************************************************************************/
app_result_e menu_return_list(void)
{
    /* ���ص�Ԥ���̨�б�������ʾģʽ*/
    g_radio_listtype = STATIONLIST_SAVE;
    g_stalist_entry = ENTER_NORMAL;
    return RESULT_RADIO_STATION_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_return_play(void *param)
 * \���Ž�����Ӳ˵���option �ص�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   �ص����Ž��棬�ٰ�return�����radio mainmenu
 */
/*******************************************************************************/
app_result_e menu_return_play(void)
{
    g_playwin_mode = ENTER_PLAY_FROM_MAIN;
    return RESULT_RADIO_START_PLAY;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_auto_tune(void* param)
 * \�˵����ܺ���-----  ȷ�������Զ���г
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_auto_tune(void *param)
{
    app_result_e result;

    //��ʾ:  �Ƿ�Ҫ�Զ���гԤ��Ƶ��
    result = show_dialog(TUNE_PRE, DIALOG_BUTTON_INCLUDE, BUTTON_YESNO_YES);
    if (result == RESULT_DIALOG_YES)
    {
        //ѡ������Զ���г
        result = radio_AutoSearch_scene(g_radio_config.seek_mode, DIR_UP);
        if (result == RESULT_ERROR)
        {
            //Ӳ�������ػ浱ǰ����( ���������˵��򲥷ų����Ӳ˵�)
            return RESULT_REDRAW;
        }
    }
    else
    {
        //���ػ浱ǰ����( ���������˵��򲥷ų����Ӳ˵�)
        //return RESULT_REDRAW;
        return result;
    }

    if (result == RESULT_RADIO_STATION_LIST)
    {
        //�Զ���г�����Ԥ���̨�б�
        g_radio_listtype = STATIONLIST_SAVE;
        /* �����������*/
        g_stalist_entry = ENTER_NORMAL;
    }

    //return RESULT_RADIO_STATION_LIST;
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_delone(void* param)
 * \�������Ԥ���̨
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       bool the result
 * \retval
 * \retval
 * \note   ��Ԥ���̨�б��option �˵���ѡ�񣬷���ʱ��ص�
 * \         ��̨�б��option  �˵���
 */
/*******************************************************************************/
app_result_e menu_scene_delone(void *param)
{
    g_radio_listtype = STATIONLIST_SAVE;
    /* ѡ�����Ԥ�����*/
    g_stalist_entry = ENTER_DEL_STATION;
    from_where = 1;
    return RESULT_RADIO_STATION_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_delall(void)
 * \�������Ԥ���̨
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note  ����dialog �Ĳ�ͬѡ�񣬷��ز�ͬ����
 */
/*******************************************************************************/
app_result_e menu_scene_delall(void *param)
{
    app_result_e result;

    result = show_dialog(DELALL, DIALOG_BUTTON_INCLUDE, BUTTON_YESNO_NO);

    //ѡ����
    if (result == RESULT_DIALOG_YES)
    {
        clear_preset_list(g_menu_band);
        //��ʾ����Ԥ�������!
        show_dialog(PRESET_DELED, DIALOG_INFOR_WAIT, 0);

        //ֱ�ӷ��ز��ų���
        result = RESULT_RADIO_START_PLAY;
    }
    //ѡ���
    else if (result == RESULT_REDRAW)
    {
        g_radio_listtype = STATIONLIST_SAVE;
        g_stalist_entry = ENTER_NORMAL;
        result = RESULT_RADIO_STATION_LIST;
    }
    else
    {
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_start_rec(void *param)
 * \��ʼFM ¼��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note    ֱ�ӷ��أ�����¼������
 */
/*******************************************************************************/
app_result_e menu_scene_start_rec(void *param)
{
    return RESULT_RECORD_REC;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_saveone(void *param)
 * \��Ƶ�㱣�浽Ԥ���̨�б�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_saveone(void *param)
{
    g_radio_listtype = STATIONLIST_SAVE;
    /* ���浽Ԥ������б�*/
    g_stalist_entry = ENTER_ADD_STATION;
    return RESULT_RADIO_STATION_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_delone_2(void *param)
 * \�������Ԥ���̨
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       bool the result
 * \retval
 * \retval
 * \note   �Ӳ��Ž����option �˵���ѡ�񣬷���ʱ��ص�
 * \         ���Ž����option  �˵���
 */
/*******************************************************************************/
app_result_e menu_scene_delone_2(void *param)
{
    g_radio_listtype = STATIONLIST_SAVE;
    /* ���Ԥ�����Ԥ���б�*/
    g_stalist_entry = ENTER_DEL_STATION;
    from_where = 2;
    return RESULT_RADIO_STATION_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_voice(void *param)
 * \FM ¼�ƣ��������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_voice(void *param)
{
    if (g_option_type == SUBS_OPT_PLAY)
    {
        /* ��option �Ӳ˵��������*/
        g_radio_config.enter_voice = FROM_OPTION_MENU;
    }
    else
    {
        /* ��radioUI �����˵��������*/
        g_radio_config.enter_voice = FROM_RADIO_MAINMENU;
    }
    return RESULT_VOICE_FM_MENU;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_selband(void *title_action)
 * \��̨����active ��ѯ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_selband(void *title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title;

    menu_title = (menu_title_action_t*) title_action;
    active_id = (uint16) g_menu_band;

    if (active_id >= Band_MAX)
    {
        active_id = Band_China_US;
    }

    menu_title->menu_entry = 0xffff; //���������
    menu_title->str_id = band_text_table[active_id]; //ָ��active��
    menu_title->ret_str = NULL;
    menu_title->source_id = 0xffff;

    return RESULT_MENU_SON; //������һ��
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_uband(void *param)
 * \ѡ���̨����---- ��ͨƵ��(  �й�/ ������̨)
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note  ѡ�񲨶κ󣬻ص����ų���������������Ӧ���β���
 */
/*******************************************************************************/
app_result_e menu_scene_uband(void *param)
{
    uint8 real_num;
    g_radio_config.band_mode = Band_China_US;
    g_menu_band = g_radio_config.band_mode;
    /* �����б�ָ��*/
    init_point_freq();
    /* ��ȡ��ͨ���ε�Ԥ���̨����*/
    g_radio_config.FMStatus.station_count = get_station_count();

    /* �ò������ѱ����̨���л�����һ�������̨*/
    if (g_radio_config.FMStatus.station_count != 0)
    {
        //��õ�һ���ѱ����̨�ĵ�̨��
        real_num = get_real_station_num(1, DIR_UP);
        if ((real_num > MAX_STATION_COUNT) || (real_num == 0))
        {
            //ȡֵ������Χ
            g_radio_config.FMStatus.freq = MIN_US;
            g_radio_config.FMStatus.station = 0;
        }
        else
        {
            g_radio_config.FMStatus.freq = g_radio_config.fmstation_us[real_num - 1];
            g_radio_config.FMStatus.station = real_num;
        }
    }
    /* �ò����ޱ����̨���л�����СƵ��*/
    else
    {
        g_radio_config.FMStatus.freq = MIN_US;
        g_radio_config.FMStatus.station = 0;
    }

    return RESULT_RADIO_START_PLAY;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_jband(void *param)
 * \ѡ���̨����---- �ձ�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_jband(void *param)
{
    uint8 real_num;
    g_radio_config.band_mode = Band_Japan;
    g_menu_band = g_radio_config.band_mode;
    /* �����б�ָ��*/
    init_point_freq();
    /* ��ȡ�ձ����ε�Ԥ���̨����*/
    g_radio_config.FMStatus.station_count = get_station_count();

    /* �ò������ѱ����̨���л�����һ�������̨*/
    if (g_radio_config.FMStatus.station_count != 0)
    {
        //��õ�һ���ѱ����̨�ĵ�̨��
        real_num = get_real_station_num(1, DIR_UP);
        if ((real_num > MAX_STATION_COUNT) || (real_num == 0))
        {
            //ȡֵ������Χ
            g_radio_config.FMStatus.freq = MIN_JP;
            g_radio_config.FMStatus.station = 0;
        }
        else
        {
            g_radio_config.FMStatus.freq = g_radio_config.fmstation_jp[real_num - 1];
            g_radio_config.FMStatus.station = real_num;
        }
    }
    /* �ò����ޱ����̨���л�����СƵ��*/
    else
    {
        g_radio_config.FMStatus.freq = MIN_JP;
        g_radio_config.FMStatus.station = 0;
    }

    return RESULT_RADIO_START_PLAY;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_eband(void)
 * \ѡ���̨����--- ŷ�޲���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_eband(void *param)
{
    uint8 real_num;
    g_radio_config.band_mode = Band_Europe;
    g_menu_band = g_radio_config.band_mode;
    /* �����б�ָ��*/
    init_point_freq();
    /* ��ȡŷ�޲��ε�Ԥ���̨����*/
    g_radio_config.FMStatus.station_count = get_station_count();

    /* �ò������ѱ����̨���л�����һ�������̨*/
    if (g_radio_config.FMStatus.station_count != 0)
    {
        //��õ�һ���ѱ����̨�ĵ�̨��
        real_num = get_real_station_num(1, DIR_UP);
        if ((real_num > MAX_STATION_COUNT) || (real_num == 0))
        {
            //ȡֵ������Χ
            g_radio_config.FMStatus.freq = MIN_EU;
            g_radio_config.FMStatus.station = 0;
        }
        else
        {
            g_radio_config.FMStatus.freq = g_radio_config.fmstation_eu[real_num - 1];
            g_radio_config.FMStatus.station = real_num;
        }
    }
    /* �ò����ޱ����̨���л�����СƵ��*/
    else
    {
        g_radio_config.FMStatus.freq = MIN_EU;
        g_radio_config.FMStatus.station = 0;
    }

    return RESULT_RADIO_START_PLAY;
}

