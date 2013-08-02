/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       mikeyang         2011-09-13          1.0              create this file
 *******************************************************************************/
#include "app_radio.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radioUI_scene_dispatch(app_param_e enter_mode)
 * \����������
 * \param[in]    enter_mode ����ģʽ
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       ���ص��δ�
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e radioUI_scene_dispatch(app_param_e enter_mode)
{
    app_result_e scene_result;
    engine_type_e engine_type;

    //��ȡ��ǰ��̨����״̬
    engine_type = get_engine_type();

    switch (enter_mode)
    {
        //��main menuѡ����radio ���룬��������˵�����
        case PARAM_FROM_MAINMENU:
        g_radio_scene = FM_SCENE_MAINMENU;
        break;

        case PARAM_FROM_ALARM:
        g_radio_scene = FM_SCENE_PLAYING;
        break;

        //��main menuѡ�������ڲ���/ �ϴβ��Ž���
        //������ap��option�˵�ѡ�������ڲ���/�ϴβ���
        case PARAM_BACKTO_PLAYING:
        case PARAM_BACKTO_PLAYED:
        g_radio_scene = FM_SCENE_PLAYING;
        g_playwin_mode = ENTER_PLAY_FROM_NOWPLAY;
        break;

        //��FM ¼�����̷��غ����
        case PARAM_FROM_RECORD:
        g_radio_scene = FM_SCENE_PLAYING;
        g_playwin_mode = ENTER_PLAY_FROM_MAIN;
        need_reinstall = TRUE;
        break;

        //����FM ʱ�ػ�, ����������ֱ�ӽ�RADIO  ����,  ��Ҫ����Ƶ��
        case PARAM_FROM_CONFIG:
        g_radio_scene = FM_SCENE_PLAYING;
        g_playwin_mode = ENTER_PLAY_FROM_MAIN;
        break;

        //�ӷ������غ����
        case PARAM_FROM_BROWSER:

        //����Ǵ�FM  ���ų�����option ����VOICE
        if (g_radio_config.enter_voice == FROM_OPTION_MENU)
        {
            //��δ����¼���ļ��Ļطţ����option menu
            if (engine_type == ENGINE_RADIO)
            {
                g_radio_scene = FM_SCENE_OPTMENU;
                g_option_type = SUBS_OPT_PLAY;
            }

            //�Ѿ�����¼���ļ��Ļط�, ��̨�����Ѿ�����RADIO
            else
            {
                //���ز��ų�����ͬʱ�迪��radio ����
                g_radio_scene = FM_SCENE_PLAYING;
                g_playwin_mode = ENTER_PLAY_FROM_MAIN;
            }
        }

        //����Ǵ�FM һ�����˵�����VOICE �����ص�һ�����˵�
        //�������( �����쳣) �� ���ص�һ�����˵�
        else //if(g_radio_config.enter_voice == FROM_RADIO_MAINMENU)
        {
            g_radio_scene = FM_SCENE_MAINMENU;
        }
        break;

        //Ĭ��������������˵�����
        default:
        g_radio_scene = FM_SCENE_MAINMENU;
        break;
    }

    /* radioUI ����ѭ��*/
    while (g_radio_scene != FM_SCENE_EXIT)
    {
        switch (g_radio_scene)
        {
            /* ����RadioUI ���˵�����*/
            case FM_SCENE_MAINMENU:

            g_option_type = NO_ENTER_SUB;
            scene_result = radioUI_mainmenu();

            switch (scene_result)
            {
                //ֱ�ӽ��벥�ų������ֶ���г��
                case RESULT_RADIO_START_PLAY:
                g_radio_scene = FM_SCENE_PLAYING;
                g_playwin_mode = ENTER_PLAY_FROM_MAIN;
                break;

                //Ԥ���̨���û���̨���Զ���г��
                case RESULT_RADIO_STATION_LIST:
                case RESULT_RADIO_USER_LIST:
                g_radio_scene = FM_SCENE_STALIST;
                if (scene_result == RESULT_RADIO_USER_LIST)
                {
                    /* �����û���̨�б�*/
                    g_radio_listtype = STATIONLIST_USER;
                }
                else
                {
                    /* ����Ԥ���̨�б�*/
                    g_radio_listtype = STATIONLIST_SAVE;
                }
                break;

                //��������˳���������( ��FM ¼�ƽ���VOICE)
                default:
                g_radio_scene = FM_SCENE_EXIT;
                break;
            }
            break;

            /* ����RADIO ���ų���*/
            case FM_SCENE_PLAYING:

            scene_result = radioUI_scene_playing(g_playwin_mode);

            switch (scene_result)
            {
                //�ص����˵�����
                case RESULT_RADIO_MAINMENU:

                g_radio_scene = FM_SCENE_MAINMENU;
                break;

                //����Ԥ���̨�б����
                case RESULT_RADIO_STATION_LIST:
                g_radio_scene = FM_SCENE_STALIST;
                g_radio_listtype = STATIONLIST_SAVE;
                break;

                //�����û���̨�б����
                case RESULT_RADIO_USER_LIST:
                g_radio_scene = FM_SCENE_STALIST;
                g_radio_listtype = STATIONLIST_USER;
                break;

                //���벥�ų�����option �˵�
                case RESULT_RADIO_OPTION_PLAYWIN:
                g_radio_scene = FM_SCENE_OPTMENU;
                g_option_type = SUBS_OPT_PLAY;
                break;

                //��������˳���������
                case RESULT_ALARM_QUIT:
                default:
                g_radio_scene = FM_SCENE_EXIT;
                break;
            }
            break;

            /*  �����̨�б���( ����Ԥ���б���û��б�) */
            case FM_SCENE_STALIST:

            scene_result = radioUI_scene_stationlist(g_radio_listtype, g_stalist_entry, from_where);

            switch (scene_result)
            {
                //ѡ���̨�󣬽��벥�ų�����ʼ����
                //ɾ����̨�󣬻ص����ų�������
                //�����̨�󣬻ص����ų�������
                case RESULT_RADIO_START_PLAY:
                g_radio_scene = FM_SCENE_PLAYING;
                g_playwin_mode = ENTER_PLAY_FROM_LIST;
                break;

                //���б���Radio ���˵�
                case RESULT_RADIO_MAINMENU:
                g_radio_scene = FM_SCENE_MAINMENU;
                break;

                //�����̨�б�option
                case RESULT_RADIO_OPTION_STATION:
                g_radio_scene = FM_SCENE_OPTMENU;
                g_option_type = SUBS_OPT_STATION;
                break;

                //���벥�ų���option����ȡ�����Ԥ�裬���浽Ԥ��
                case RESULT_RADIO_OPTION_PLAYWIN:
                g_radio_scene = FM_SCENE_OPTMENU;
                g_option_type = SUBS_OPT_PLAY;
                break;

                //�����˳���������
                default:
                g_radio_scene = FM_SCENE_EXIT;
                break;
            }
            break;

            /*  ����option menu ����( �������Ž����º͵�̨�б���) */
            case FM_SCENE_OPTMENU:
            scene_result = radioUI_scene_optionlist(g_option_type);

            switch (scene_result)
            {
                //����Ԥ���̨�б�
                //���������Ԥ��򱣴浽Ԥ�����ͨ�������
                case RESULT_RADIO_STATION_LIST:
                g_radio_scene = FM_SCENE_STALIST;
                g_radio_listtype = STATIONLIST_SAVE;
                break;

                case RESULT_RADIO_START_PLAY:
                g_radio_scene = FM_SCENE_PLAYING;
                g_playwin_mode = ENTER_PLAY_FROM_MAIN;
                break;

                //��������˳���������
                default:
                g_radio_scene = FM_SCENE_EXIT;
                break;
            }
            break;

            default:
            //�˳���������
            g_radio_scene = FM_SCENE_EXIT;
            break;
        }
    }
    return scene_result;

}

