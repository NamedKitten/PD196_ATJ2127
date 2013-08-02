/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-26          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

//ת��ͼƬ����
#define NUM_ICON      8

extern bool radio_AutoSearch_init(FM_SeekDir_e dir)
__FAR__;
extern bool radio_AutoSearch_exit(void)
__FAR__;
extern void auto_display_init(void)
__FAR__;

/* radio ȫ�Զ���̨�����¼�ӳ��*/
const key_map_t autoseek_key_map_list[] =
{
    /*! �̰�KEY_VOL ����ת��Ϊ �����¼� */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! ���һ��ӳ���¼� */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/******************************************************************************/
/*!
 * \par  Description:
 * \void radio_auto_proc(void)
 * \ȫ�Զ���̨��������ˢ�¶�ʱ��
 * \param[in]    none
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note  ת��ͼƬ��ʱˢ��
 */
/*******************************************************************************/
void radio_auto_proc(void)
{
    style_infor_t auto_sty;
    picbox_private_t picbox_param;

    ui_icon_count = (ui_icon_count + 1) % (NUM_ICON);

    auto_sty.style_id = PICBOX_SEARCH;
    auto_sty.type = UI_AP;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = ui_icon_count;
    //picbox_param.direction = DIRECTION_NORMAL;
    ui_show_picbox(&auto_sty, &picbox_param);
}

/****************************************************************************************/
/*!
 * \par  Description:
 * \void update_progress(uint16 freq, uint8 mode)
 * \ȫ�Զ���̨�����У����ݵ�ǰƵ��ˢ�½�����
 * \param[in]    value:   ��ǰƵ��ֵ
 * \param[in]    mode:   �Ƿ�β����0:  ��  1: ��
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   ���ǰ���������̨ˢ�½�����, һ��ȫ�Զ���̨����SEEK UP
 */
/******************************************************************************************/
void update_progress(uint16 freq, uint8 mode)
{
    style_infor_t auto_sty;
    progressbar_private_t progressbar_param;
    uint32 tmp;

    auto_sty.type = UI_AP;
    auto_sty.style_id = PBAR_SEARCH;
    progressbar_param.total = 118;

    if (mode == 1)
    {
        //��̨��βˢ������������
        progressbar_param.value = progressbar_param.total;
    }
    else
    {
        /* 76MHz ~~ 90MHz */
        if (g_menu_band == Band_Japan)
        {
            tmp = (uint32)((freq - 0x28e0) * 118);
            progressbar_param.value = (uint16)(tmp / (0x36b0));
        }
        /* 87.5MHz ~~ 108MHz */
        else
        {
            tmp = (uint32)((freq - 0x55cc) * 118);
            progressbar_param.value = (uint16)(tmp / (0x5014));
        }
    }
    ui_show_progressbar(&auto_sty, &progressbar_param, PROGRESS_DRAW_PROGRESS);

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _scene_autoseek_sysmsg(void)
 * \ȫ�Զ���̨ϵͳ��Ϣ��˽����Ϣ����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e scene_autoseek_sysmsg(void)
{
    private_msg_t autoseek_msg;
    app_result_e msg_result = RESULT_NULL;
    if (get_app_msg(&autoseek_msg) == TRUE)
    {
        msg_result = radio_app_msg_callback(&autoseek_msg);
    }
    return msg_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_hard_autoseek(FM_SeekDir_e dir)
 * \Ӳ��ȫ�Զ���̨����
 * \param[in]    direction:   ��̨����
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e deal_hard_autoseek(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 tab_num, dir_seek;
    bool result, need_restart = TRUE;
    uint16 current_freq, end_freq;
    //��ȡgui��Ϣ
    input_gui_msg_t gui_msg;
    //gui ��Ϣ��Ӧgui �¼�
    msg_apps_type_e gui_event;

    //���õ�ǰ����, ��Ҫ��ȷ��space
    radio_set_band(g_menu_band);
    //��ʼƵ��
    current_freq = g_radio_config.FMStatus.freq;
    tab_num = 0;

    //��ȷ������,�Ƿ����
    //ȫ�Զ���̨��һ�����͵��������һ�飬������
    if (dir == DIR_UP)
    {
        //������̨,  ������
        dir_seek = 0x02;
    }
    else
    {
        //������̨, ������
        dir_seek = 0x03;
    }

    //���ý���Ƶ��
    if (g_menu_band == Band_Japan)
    {
        if (dir == DIR_UP)
        {
            end_freq = MAX_JP; // 90M
        }
        else
        {
            end_freq = MIN_JP; // 76M
        }
    }
    else
    {
        if (dir == DIR_UP)
        {
            end_freq = MAX_US; // 108M
        }
        else
        {
            end_freq = MIN_US; // 87.5M
        }
    }
    while (1)
    {
        if (need_restart == TRUE)
        {
            result = radio_set_hardseek(current_freq, dir_seek);
            need_restart = FALSE;
            if (!result)
            {
                //Ӳ����̨����ʧ��
                ret = RESULT_RADIO_STATION_LIST;
                break;
            }
        }

        //Ӳ����̨�����ɹ�,  ȡ�Ƿ���ɱ�־
        result = radio_get_seekflag();
        if (result == TRUE)
        {
            //������̨���,  ��ȡ��ǰƵ��
            radio_get_freq();
            current_freq = g_engine_status.FM_CurrentFreq;

            if (((current_freq >= end_freq) && (dir == DIR_UP)) || ((current_freq <= end_freq) && (dir == DIR_DOWN))
                    || ((g_engine_status.STC_flag == HARDSEEK_NOT_START)))
            {
                //�ѵ�����Ƶ���δ�ѵ���Ч̨, �˳���̨
                radio_break_hardseek();
                update_progress(current_freq, 1);
                ret = RESULT_RADIO_STATION_LIST;
                break;
            }

            if (g_engine_status.STC_flag == HARDSEEK_COMPLETE)
            {
                //����96MHZ �� 108MHZ Ƶ��
                if ((current_freq != 0x7700) && (current_freq != 0xa5e0))
                {
                    //������Ч��̨
                    Auto_tab[tab_num] = current_freq;
                    tab_num++;
                }
            }

            //δ������Ƶ��, ��������һ����̨
            need_restart = TRUE;
            //tab_num++;
        }
        else
        {
            //������̨��δ���,  ȡƵ����Ҫ�Ǹ��½�����
            radio_get_freq();
            current_freq = g_engine_status.FM_CurrentFreq;
        }

        //��̨�б�����
        if (tab_num >= MAX_STATION_COUNT)
        {
            radio_break_hardseek();
            update_progress(current_freq, 1);
            ret = RESULT_RADIO_STATION_LIST;
            break;
        }

        //���ݵ�ǰƵ�㣬���½�����
        update_progress(current_freq, 0);

        //����ϵͳ��Ϣ��gui ��Ϣ
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {

            if (com_key_mapping(&gui_msg, &gui_event, autoseek_key_map_list) == TRUE)
            {
                if (gui_event == EVENT_RADIO_RETURN)
                {
                    //return ֹͣ��̨,���ص�̨�б�
                    radio_break_hardseek();
                    ret = RESULT_RADIO_STATION_LIST;
                    break;
                }
            }
        }
        else
        {
            //һ��ѭ������һ�����͵���Ϣ
            ret = scene_autoseek_sysmsg();
        }

        if ((ret != RESULT_IGNORE) && (ret != RESULT_NULL))
        {
            break;
        }
        //����20ms �����񽻻�
        sys_os_time_dly(2);
    }

    //�����̨��
    g_radio_config.FMStatus.station_count = tab_num;
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_AutoSearch_scene(FM_SeekMode_e mode, FM_SeekDir_e dir)
 * \ȫ�Զ���̨����
 * \param[in]    mode:  ��̨ģʽ( Ӳ����̨or  �����̨)
 * \param[in]    dir: ��̨����( ���ϻ�����)
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note ȫ�Զ���̨�������ǰ�˵����ζ�Ӧ���б�
 * \       �����յ�ǰ�˵����ν�����̨
 */
/*******************************************************************************/
app_result_e radio_AutoSearch_scene(FM_SeekMode_e mode, FM_SeekDir_e dir)
{
    app_result_e result;
    bool bret;

#ifndef PC
    /* ��ʼ��*/
    bret = radio_AutoSearch_init(dir);
    if (!bret)
    {
        //��ʼ��ʧ�ܷ���
        return RESULT_ERROR;
    }

    //��ʼȫ�Զ���̨ǰ����mute
    radio_set_mute(SetMUTE);

    auto_display_init();

    //������ʱ��
    restart_app_timer(timer_auto_seek);

    if (mode == HARDSEEK)
    {
        //����Ӳ��ȫ�Զ���̨
        result = deal_hard_autoseek(dir);
    }
    else
    {
        //�������ȫ�Զ���̨
        result = deal_soft_autoseek(dir);
    }

    radio_AutoSearch_exit();
#else
    result = RESULT_RADIO_STATION_LIST; 
#endif
    return result;

}

