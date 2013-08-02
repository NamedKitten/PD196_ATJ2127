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

extern void update_progress(uint16 freq, uint8 mode)
__FAR__;
extern app_result_e scene_autoseek_sysmsg(void)
__FAR__;

/* radio ȫ�Զ���̨�����¼�ӳ��*/
const key_map_t softseek_key_map_list[] =
{
    /*! �̰�KEY_VOL ����ת��Ϊ �����¼� */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! ���һ��ӳ���¼� */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_soft_autoseek(FM_SeekDir_e direction)
 * \���ȫ�Զ���̨����
 * \param[in]    direction:   ��̨����
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   һ������һ��Ƶ�����tune�����ж��Ƿ���̨
 */
/*******************************************************************************/
app_result_e deal_soft_autoseek(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 space, tab_num;
    bool is_true = FALSE;
    bool result;
    uint8 dir_seek;

    //��ȡgui��Ϣ
    input_gui_msg_t gui_msg;
    //gui ��Ϣ��Ӧgui �¼�
    msg_apps_type_e gui_event;

    //��̨����
    space = 100;
    if (g_menu_band == Band_Europe)
    {
        //ŷ�޲���, ��������Ϊ50K
        space = 50;
    }

    if (dir == DIR_DOWN)
    {
        dir_seek = 0;
    }
    else
    {
        dir_seek = 1;
    }

    tab_num = 0;
    while (1)
    {
        //�жϵ�ǰƵ���Ƿ���Ч̨
        is_true = radio_set_softseek(g_radio_config.FMStatus.freq, dir_seek);
        if (is_true == TRUE)
        {
            //����96MHZ �� 108MHZ Ƶ��
            if ((g_radio_config.FMStatus.freq != 0x7700) && (g_radio_config.FMStatus.freq != 0xa5e0))
            {
                //����Ч��̨������
                Auto_tab[tab_num] = g_radio_config.FMStatus.freq;
                is_true = FALSE;
                tab_num++;
            }
        }

        //��̨�б�����
        if (tab_num >= MAX_STATION_COUNT)
        {
            update_progress(g_radio_config.FMStatus.freq, 1);
            ret = RESULT_RADIO_STATION_LIST;
            break;
        }

        if (dir == DIR_UP)
        {
            if (((g_menu_band == Band_China_US) || (g_menu_band == Band_Europe)) && ((g_radio_config.FMStatus.freq
                    + space) > MAX_US)) // 108MHz
            {
                update_progress(g_radio_config.FMStatus.freq, 1);
                ret = RESULT_RADIO_STATION_LIST;
                break;
            }
            else if ((g_menu_band == Band_Japan) && ((g_radio_config.FMStatus.freq + space) > MAX_JP)) // 90MHz
            {
                update_progress(g_radio_config.FMStatus.freq, 1);
                ret = RESULT_RADIO_STATION_LIST;
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
            if (((g_menu_band == Band_China_US) || (g_menu_band == Band_Europe)) && ((g_radio_config.FMStatus.freq
                    - space) < MIN_US)) // 87.5MHz
            {
                update_progress(g_radio_config.FMStatus.freq, 1);
                ret = RESULT_RADIO_STATION_LIST;
                break;
            }
            else if ((g_menu_band == Band_Japan) && ((g_radio_config.FMStatus.freq - space) < MIN_JP)) // 76MHz
            {
                update_progress(g_radio_config.FMStatus.freq, 1);
                ret = RESULT_RADIO_STATION_LIST;
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
        //    ret = RESULT_RADIO_STATION_LIST;
        //    break;
        //}

        //���ݵ�ǰƵ�㣬���½�����
        update_progress(g_radio_config.FMStatus.freq, 0);

        //����ϵͳ��Ϣ��gui ��Ϣ
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {

            if (com_key_mapping(&gui_msg, &gui_event, softseek_key_map_list) == TRUE)
            {
                if (gui_event == EVENT_RADIO_RETURN)
                {
                    //return ֹͣ��̨,���ص�̨�б�
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

