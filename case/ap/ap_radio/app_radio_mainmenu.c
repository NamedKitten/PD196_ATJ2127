/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-23          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"
#include "app_radio_menu_config.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \RadioUI ���˵�����
 * \app_result_e radioUI_mainmenu(void)
 * \param[in]    no
 * \param[in]
 * \param[out]   none
 * \return       int the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e radioUI_mainmenu(void)
{
    app_result_e ret_vals;
    menu_com_data_t menu_param;

    //�˵�������
    menu_param.app_id = APP_ID_RADIO;
    menu_param.menu_entry = MAINMENU_ENTRY;
    menu_param.path_id = 0;
    menu_param.browse_mode = 0;
    menu_param.menu_func = NULL;

    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_RADIO);

    /* ���ÿؼ�����ʾ���˵���*/
    ret_vals = gui_menulist(MENULIST, &menu_param);
    return ret_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_option_callback(void)
 * \Radio ���˵�option �ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_option_callback(void)
{
    show_dialog(S_OPTION_DESC, DIALOG_INFOR_WAIT, 0);
    //�ػ�ȫ���˵�
    return RESULT_REDRAW;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_saved_preset(void* param)
 * \�˵����ܺ���-----  ȷ�Ͻ���Ԥ���̨�б�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_saved_preset(void *param)
{
    app_result_e result;
    uint8 count;
    uint16 *tmp;

    g_stalist_entry = ENTER_NORMAL;
    result = RESULT_RADIO_STATION_LIST;

    if (g_radio_config.band_mode == Band_MAX)
    {
        //�����ǰ����Ϊ�û�ģʽ,  g_radio_config.FMStatus.station_count ��¼�����û���̨��
        //�ȱ���ָ��
        tmp = pfreq;

        if (g_menu_band == Band_Japan)
        {
            pfreq = &g_radio_config.fmstation_jp[0];
        }
        else if (g_menu_band == Band_Europe)
        {
            pfreq = &g_radio_config.fmstation_eu[0];
        }
        else
        {
            pfreq = &g_radio_config.fmstation_us[0];
        }
        //��õ�ǰ�˵�������ָʾ���б��еĵ�̨��
        count = get_station_count();
        //�ָ�ָ�룬��ָ���û��б���
        pfreq = tmp;
    }
    else
    {
        //���û�ģʽ�£�g_radio_config.FMStatus.station_count ��¼��Ϊ��ǰ���ε�̨��
        count = g_radio_config.FMStatus.station_count;
    }

    if (count == 0)
    {
        //��ǰ��̨�����޵�̨��ʱ������ʾ��ʾ��
        //�Ƿ�Ҫ�Զ���гԤ��
        result = show_dialog(TUNE_PRE, DIALOG_BUTTON_INCLUDE, BUTTON_YESNO_YES);
        if (result == RESULT_DIALOG_YES)
        {
            //ѡ������Զ���г
            result = radio_AutoSearch_scene(g_radio_config.seek_mode, DIR_UP);
            if (result == RESULT_ERROR)
            {
                //Ӳ�������ػ浱ǰ��( radio UI mainmenu)
                return RESULT_REDRAW;
            }
            //�Զ���г�󣬽���Ԥ���б���ʾ
        }
        else if (result == RESULT_REDRAW)
        {
            //�������Զ���г����ʾ�յ�̨�б�
            result = RESULT_RADIO_STATION_LIST;
        }
        else
        {
        }
    }
    //����Ԥ���̨�б�
    //return RESULT_RADIO_STATION_LIST;
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_user_preset(void *param)
 * \�˵����ܺ���-----  ȷ�Ͻ����û���̨�б�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note �û���̨�б�֧��С������Ӻ�ɾ����ֻ������ʾ
 */
/*******************************************************************************/
app_result_e menu_scene_user_preset(void *param)
{
    //�����û���̨�б���ʱ�Ѿ���ȡ�û���̨Ƶ��ֵ
    return RESULT_RADIO_USER_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_manual_tune(void *param)
 * \�˵����ܺ���----  ȷ�������ֶ���г
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note  �ֶ���гѡ��Ƶ�����
 * \1. ��ǰ��FM ��̨���ţ��ص���ǰƵ��( �����û���̨ģʽ)
 * \2. ��ǰ��̨FM ����δװ�أ�����ѡ��ǰ���ε�̨�б���
 * \    ��һ����̨����
 * \3. �����ǰ��̨�б�Ϊ�գ�ѡ��ǰ���ε�Ĭ��Ƶ�㲥��
 * \����2��3 �㣬���֮ǰ����Ϊ�û���̨������ѡ��֮ǰ��
 * \�û���̨����
 */
/*******************************************************************************/
app_result_e menu_scene_manual_tune(void *param)
{
    //���Ӷ�����Ϊ����������
    show_dialog(USE_ANT, DIALOG_INFOR_WAIT, 0);
    return RESULT_RADIO_START_PLAY;
}

