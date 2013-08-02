/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-15          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

app_result_e deal_soft_search(FM_SeekDir_e dir);
app_result_e deal_hard_search(FM_SeekDir_e dir);

/* radio ���Զ���̨���������¼�ӳ��*/
const key_map_t search_key_map_list[] =
{
    /*! �̰�option�¼� */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_OPTION },
    /*!�̰�play ֹͣ��̨*/
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! �̰�KEY_VOL ����ֹͣ��̨*/
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*!�̰�prev ֹͣ��̨*/
    {{ KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*!�̰�next ֹͣ��̨*/
    {{ KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! ���һ��ӳ���¼� */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/******************************************************************************/
/*!
 * \par  Description:
 * \bool  scene_search_init(void)
 * \radio ���Զ���̨������ʼ��
 * \param[in]    none
 * \param[out]   none
 * \return       success/fail
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
bool scene_search_init(void)
{
    bool ret = TRUE;

    radio_get_freq();
    startfreq = g_engine_status.FM_CurrentFreq;
    g_radio_config.FMStatus.freq = startfreq;

    //ˢ����������ϴε�̨������ʹ��������ʾ����ˢҲû��ϵ
    ui_show_pic(TEXTBOX_PRESET_INFOR_BG, 21, 100);
    //����������رչ�����ʱ��
    if (timer_flush_radioinfo != -1)
    {
        kill_app_timer(timer_flush_radioinfo);
        timer_flush_radioinfo = -1;
    }
    //��ʼ���Զ���̨ǰ����mute
    radio_set_mute(SetMUTE);
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool scene_search_exit(void)
 * \radio ���Զ���̨�����˳�
 * \param[in]    none
 * \param[out]   none
 * \return       success/fail
 * \retval       none
 * \retval       none
 * \note     ��ȷ���˳����Ƶ��͵�̨�ţ�����ˢ�½���
 */
/*******************************************************************************/
bool scene_search_exit(void)
{
    uint8 num;
    bool result;

    //��ȡ��ǰƵ��ֵ
    result = radio_get_freq();
    if (result == TRUE)
    {
        //������ȡ�����ŵ�ǰƵ��
        g_radio_config.FMStatus.freq = g_engine_status.FM_CurrentFreq;

        if ((g_radio_config.FMStatus.freq > max_freq[g_radio_config.band_mode]) || ((g_radio_config.FMStatus.freq
                < min_freq[g_radio_config.band_mode])))
        {
            //��ȡ��Ƶ�㲻��ȷ�����Ű��Զ���̨ǰƵ��
            g_radio_config.FMStatus.freq = startfreq;
        }
    }
    else
    {
        //��ȡʧ�ܣ����Ű��Զ���̨ǰƵ��
        g_radio_config.FMStatus.freq = startfreq;
    }

    //�жϵ�ǰƵ���Ƿ����ѱ���Ƶ��
    num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);

    //�л����Ƶ����δ����Ƶ��
    if (num == 0xff)
    {
        if (g_radio_config.band_mode == Band_MAX)
        {
            g_userlist_index = 0xff;
        }
        g_radio_config.FMStatus.station = 0;
    }
    //�л����Ƶ�����ѱ���Ƶ��
    else
    {
        if (g_radio_config.band_mode == Band_MAX)
        {
            //���ݵ�̨�Ż�ȡ����
            g_userlist_index = get_index_from_num(num);
            if (g_userlist_index == 0xff)
            {
                //û�ҵ���������Ϊδ����
                num = 0;
            }

            //�û���̨ˢ�µ�̨����
            paint_flag |= PAINT_STATION_INFOR;
        }
        g_radio_config.FMStatus.station = num;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e scene_play_search(FM_SeekDir_e dir)
 * \radio ���Ž�����Զ���̨����
 * \param[in]    dir   ���Զ���̨����
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note  ������Զ���̨����ʱ������Ӧ���Ѱ�װ��ģ��Ҳ��
 * \        ��ʼ����ͬʱ����Ҳ����ȫ��ˢ�£�ֻ������Ƶ��
 * \        �����ɣ�����Ƶ��ͼƬ����̨�ţ�tunebar��
 */
/*******************************************************************************/
app_result_e scene_play_search(FM_SeekDir_e dir)
{
    bool result;
    //��ȡgui��Ϣ
    //input_gui_msg_t gui_msg;
    app_result_e ret = RESULT_IGNORE;

#ifndef PC
    //���˺���������Ϣ
    com_filter_key_hold();
    result = scene_search_init();
    if (!result)
    {
        //�����������ų���
        return RESULT_IGNORE;
    }

    if (g_radio_config.seek_mode == HARDSEEK)
    {
        //����Ӳ�����Զ���̨
        ret = deal_hard_search(dir);
    }
    else
    {
        //����������Զ���̨
        ret = deal_soft_search(dir);
    }
    scene_search_exit();    
#endif
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 init_start_freq(FM_SeekDir_e dir)
 * \���Զ���̨��ʼ����ʼƵ��
 * \param[in]    direction:   ��̨����
 * \param[out]   none
 * \return       ����space
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
uint8 init_start_freq(FM_SeekDir_e dir)
{
    uint8 space;
    //��̨����
    space = 100;
    if ((g_radio_config.band_mode == Band_Europe) || (g_radio_config.band_mode == Band_MAX))
    {
        //ŷ�޲��λ������û�ģʽ����������Ϊ50K
        space = 50;
    }

    /* ���ó�ʼ�ж�Ƶ��*/
    if (dir == DIR_UP)
    {
        if (g_radio_config.FMStatus.freq < max_freq[g_radio_config.band_mode])
        {
            g_radio_config.FMStatus.freq += space;
        }
        else
        {
            //���Ƶ���л�����С
            g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        }
    }
    else
    {
        if (g_radio_config.FMStatus.freq > min_freq[g_radio_config.band_mode])
        {
            g_radio_config.FMStatus.freq -= space;
        }
        else
        {
            //��СƵ���л������
            g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
        }
    }
    return space;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_soft_search(FM_SeekDir_e dir)
 * \������Զ���̨����
 * \param[in]    direction:   ��̨����
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   һ������һ��Ƶ�����tune�����ж��Ƿ���̨
 * \         �ҵ�һ����̨��ֹͣ���߽���ƣ�����һ�֡�
 */
/*******************************************************************************/
app_result_e deal_soft_search(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    bool is_true = FALSE;
    bool result;
    uint8 space;
    uint8 num;
    uint8 dir_seek;

    //��ȡgui��Ϣ
    input_gui_msg_t gui_msg;
    //gui ��Ϣ��Ӧgui �¼�
    msg_apps_type_e gui_event;

    space = init_start_freq(dir);

    if (dir == DIR_DOWN)
    {
        dir_seek = 0;
    }
    else
    {
        dir_seek = 1;
    }

    while (1)
    {
        //�жϵ�ǰƵ���Ƿ����ѱ���Ƶ��
        //�����µ�̨��
        num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);
        if (num == 0xff)
        {
            g_radio_config.FMStatus.station = 0;
        }
        else
        {
            g_radio_config.FMStatus.station = num;
        }
        //����ˢ��
        show_freq_in_play();
        show_station_num();

        //�жϵ�ǰƵ���Ƿ���Ч̨
        is_true = radio_set_softseek(g_radio_config.FMStatus.freq, dir_seek);
        if (is_true == TRUE)
        {
            //����96MHZ �� 108MHZ Ƶ��
            if ((g_radio_config.FMStatus.freq != 0x7700) && (g_radio_config.FMStatus.freq != 0xa5e0))
            {
                //����Ч��̨������˳�
                break;
            }
        }

        //�˳�����
        if (dir == DIR_UP)
        {
            //�߽����
            if (g_radio_config.FMStatus.freq >= max_freq[g_radio_config.band_mode])
            {
                g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
            }
            //����һ�֣�δ�ҵ���Ч��̨���˳�
            if (g_radio_config.FMStatus.freq == startfreq)
            {
                break;
            }
            else
            {
                //����Ƶ��
                g_radio_config.FMStatus.freq += space;
            }
        }
        else //if (dir == DIR_DOWN)
        {
            //�߽����
            if (g_radio_config.FMStatus.freq <= min_freq[g_radio_config.band_mode])
            {
                g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
            }
            //����һ�֣�δ�ҵ���Ч��̨���˳�
            if (g_radio_config.FMStatus.freq == startfreq)
            {
                break;
            }
            else
            {
                //����Ƶ��
                g_radio_config.FMStatus.freq -= space;
            }
        }
        //else
        //{
        //    break;
        //}

        //����ϵͳ��Ϣ��gui ��Ϣ
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {
            if (com_key_mapping(&gui_msg, &gui_event, search_key_map_list) == TRUE)
            {
                if (gui_event == EVENT_RADIO_OPTION)
                {
                    //����option �˵�
                    ret = RESULT_RADIO_OPTION_PLAYWIN;
                    break;
                }
                else if (gui_event == EVENT_RADIO_RETURN)
                {
                    //�˳����Զ���̨���ص����ų���
                    ret = RESULT_NULL;
                    break;
                }
                else
                {
                    ret = RESULT_NULL;
                    break;
                }
            }
        }
        else
        {
            //һ��ѭ������һ�����͵���Ϣ
            ret = scene_play_sysmsg();
        }

        if ((ret != RESULT_IGNORE) && (ret != RESULT_NULL))
        {
            break;
        }
        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_hard_search(FM_SeekDir_e dir)
 * \Ӳ�����Զ���̨����
 * \param[in]    direction:   ��̨����
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e deal_hard_search(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 dir_seek;
    bool result, need_restart = TRUE;
    uint16 current_freq, end_freq;
    radio_band_e mode;
    uint8 num;

    //��ȡgui��Ϣ
    input_gui_msg_t gui_msg;
    //gui ��Ϣ��Ӧgui �¼�
    msg_apps_type_e gui_event;

    //�˵�����
    mode = g_menu_band;
    if (g_radio_config.band_mode == Band_MAX)
    {
        //��ǰ��Ϊ�û�ģʽ������space Ϊ50k
        mode = Band_Europe;
    }
    //���õ�ǰ����, ��Ҫ��ȷ��space
    radio_set_band(mode);

    init_start_freq(dir);

    //��ʼƵ��
    current_freq = g_radio_config.FMStatus.freq;

    //��ȷ������,�Ƿ����
    if (dir == DIR_UP)
    {
        //������̨,  ����
        dir_seek = 0;
    }
    else
    {
        //������̨, ����
        dir_seek = 0x01;
    }

    //���ý���Ƶ��
    end_freq = startfreq;

    while (1)
    {
        //�жϵ�ǰƵ���Ƿ����ѱ���Ƶ��
        //�����µ�̨��
        num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);
        if (num == 0xff)
        {
            g_radio_config.FMStatus.station = 0;
        }
        else
        {
            g_radio_config.FMStatus.station = num;
        }
        //����ˢ��
        show_freq_in_play();
        show_station_num();

        //�Ƿ���������һ��Ӳ��seek
        if (need_restart == TRUE)
        {
            result = radio_set_hardseek(current_freq, dir_seek);
            need_restart = FALSE;
            if (!result)
            {
                //Ӳ����̨����ʧ��
                //�˳����Զ���̨���ص����ų���
                break;
            }
        }

        //Ӳ����̨�����ɹ�,  ȡ�Ƿ���ɱ�־
        result = radio_get_seekflag();

        if ((result) || (g_engine_status.STC_flag == HARDSEEK_NOT_START))
        {
            //������̨��ɻ�seek fail, �˳�
            radio_break_hardseek();
            break;
        }
        else
        {
            //������̨��δ��ɣ�ȡƵ����ʾ
            radio_get_freq();
            current_freq = g_engine_status.FM_CurrentFreq;
            g_radio_config.FMStatus.freq = current_freq;
        }

        //����ϵͳ��Ϣ��gui ��Ϣ
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {
            if (com_key_mapping(&gui_msg, &gui_event, search_key_map_list) == TRUE)
            {
                //�а������˳����Զ���̨
                radio_break_hardseek();
                if (gui_event == EVENT_RADIO_OPTION)
                {
                    //����option �˵�
                    ret = RESULT_RADIO_OPTION_PLAYWIN;
                    break;
                }
                else if (gui_event == EVENT_RADIO_RETURN)
                {
                    //�˳����Զ���̨���ص����ų���
                    ret = RESULT_NULL;
                    break;
                }
                else
                {
                    ret = RESULT_NULL;
                    break;
                }
            }
        }
        else
        {
            //һ��ѭ������һ�����͵���Ϣ
            ret = scene_play_sysmsg();
        }

        if ((ret != RESULT_IGNORE) && (ret != RESULT_NULL))
        {
            break;
        }
        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }

    return ret;
}

