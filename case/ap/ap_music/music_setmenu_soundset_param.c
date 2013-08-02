/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-21          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

extern bool check_fullsound_close(app_result_e* ask_result);

//������ȫ������
parambox_one_t sound_param_box[5];

parambox_private_t sound_param_private;

param_com_data_t sound_param_comdata;

static const uint16 user_srs_uint_string_id[5] =
{ S_USER_B, S_USER_L, S_USER_M, S_USER_H, S_USER_T }; //5����Ϊ�Զ��������������ʾ��

/******************************************************************************/
/*!
 * \par  Description:
 * \eq�ص�����
 * \param[in]    sound_param  ����ֵ
 * \param[in]    eq_type      eq����
 * \param[in]    end_index    ��Ч��������
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 fail
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/************************** ************************************************ *****/
static bool eq_callback(parambox_one_t *sound_param, uint8 eq_type, uint8 end_index)
{
    uint8 i;
    //����eqģʽ
    mmm_mp_eq_info_t eq_info;

    if (eq_type == EQ_TYPE_USR_MODE)
    {
        for (i = 0; i < end_index; i++)
        {
            eq_info.eq_para[i] = (char) (sound_param[i].value + 5);
        }

        for (i = end_index; i < 12; i++)
        {
            eq_info.eq_para[i] = (char) (g_setting_comval.music_comval.srs_parameter[i]);
        }
    }
    else if (eq_type == EQ_EQ_USR_MODE)
    {
        for (i = 0; i < end_index; i++)
        {
            eq_info.eq_para[i] = (char) (sound_param[i].value);
        }

        for (i = end_index; i < 12; i++)
        {
            eq_info.eq_para[i] = (char) (g_setting_comval.music_comval.eq_parameter[i]);
        }
    }
    else
    {
        ;//nothing
    }

    eq_info.eq_type = eq_type;

    music_set_eq(&eq_info);

    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \SRS eq�ص�����
 * \param[in]    active
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 fail
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/************************** ************************************************ *****/
static bool set_scene_srs_user_callback(int16 active)
{
    return eq_callback(sound_param_box, EQ_TYPE_USR_MODE, 5);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \�û� eq�ص�����
 * \param[in]    active
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 fail
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/************************** ************************************************ *****/

static bool set_scene_eq_user_callback(int16 active)
{
    return eq_callback(sound_param_box, EQ_EQ_USR_MODE, 5);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool set_scene_speed_callback(uint16 speed_value)
 * \���ò����ٶȻص�����
 * \param[in]    speed_value�����ٶ�ֵ  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 fail
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/************************** ************************************************ *****/
bool set_scene_speed_callback(int16 speed_value)
{
    //���ò����ٶ�
    mmm_mp_eq_info_t eq_info;
    eq_info.eq_type = 0x77;
    eq_info.eq_para[0] = (char) speed_value;
    //g_setting_comval.music_comval.variable_playback = (int8) speed_value;
    music_set_eq(&eq_info);
    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_eqcustom_sure(void *param)
 * \eq����Ϊ�û�ģʽ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_IGNORE ����
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqcustom_sure(void *param)
{
    app_result_e set_vals;

    uint8 i;
    for (i = 0; i < 5; i++)
    {
        //�ؼ���������
        sound_param_box[i].min = -5;
        sound_param_box[i].max = 5;
        sound_param_box[i].step = 1;
        sound_param_box[i].value = g_setting_comval.music_comval.eq_parameter[i];
        sound_param_box[i].cycle = FALSE;
        sound_param_box[i].adjust_func = NULL;
        sound_param_box[i].callback = set_scene_eq_user_callback;
        sound_param_box[i].max_number = 1;//����ʾ����
        sound_param_box[i].unit_id = user_srs_uint_string_id[i];
    }

    //����˽������
    sound_param_private.param_cnt = 5;
    sound_param_private.active = 0;
    sound_param_private.items = sound_param_box;

    sound_param_private.back_id = -1;
    sound_param_private.icon_id = -1;
    sound_param_private.title_id = -1;
    sound_param_private.sign_flag = 0;

    //��������
    sound_param_comdata.private_data = &sound_param_private;

    //���ÿؼ�
    set_vals = gui_set_parameter(SLIDER_SETEQ, &sound_param_comdata);

    if (set_vals == RESULT_CONFIRM)
    {
        if (check_fullsound_close(&set_vals) == TRUE)
        {
            //�������
            for (i = 0; i < 5; i++)
            {
                g_setting_comval.music_comval.eq_parameter[i] = (int8) sound_param_box[i].value;
            }

            g_setting_comval.music_comval.eq_setting = VALUE_EQ_CUSTOM;
            //foullsound�ر�
            g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
            //srs�ر�
            g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
            //speed�ر�
            g_setting_comval.music_comval.variable_playback = 0;
            //����Ӳ��EQ
            eq_callback(sound_param_box, EQ_EQ_USR_MODE, 0);
            //���ص���һ���˵�
            return RESULT_MENU_PARENT;
        }
    }

    //�ָ�֮ǰ��EQ�趨
    _scene_play_seteq(g_setting_comval.music_comval.eq_setting);
    return set_vals;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_srs_user_sure(void *param)
 * \srs����Ϊ�û�ģʽ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_IGNORE ����
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_srs_user_sure(void *param)
{
    app_result_e set_vals;

    uint8 i;
    for (i = 0; i < 5; i++)
    {
        //�ؼ���������
        sound_param_box[i].min = -5;
        sound_param_box[i].max = 5;
        sound_param_box[i].step = 1;
        sound_param_box[i].value = g_setting_comval.music_comval.srs_parameter[i] - 5;
        sound_param_box[i].cycle = FALSE;
        sound_param_box[i].adjust_func = NULL;
        sound_param_box[i].callback = set_scene_srs_user_callback;
        sound_param_box[i].max_number = 1;//����ʾ����
        sound_param_box[i].unit_id = -1;

    }
    //����˽������
    sound_param_private.param_cnt = 5;
    sound_param_private.active = 0;
    sound_param_private.items = sound_param_box;

    sound_param_private.back_id = -1;
    sound_param_private.icon_id = -1;
    sound_param_private.title_id = -1;
    sound_param_private.sign_flag = 0;
    //��������
    sound_param_comdata.private_data = &sound_param_private;

    //Ĭ��ֵ
    g_setting_comval.music_comval.srs_parameter[5] = 1;
    g_setting_comval.music_comval.srs_parameter[6] = 0;
    g_setting_comval.music_comval.srs_parameter[7] = 30;
    g_setting_comval.music_comval.srs_parameter[8] = 1;
    g_setting_comval.music_comval.srs_parameter[9] = 0;
    g_setting_comval.music_comval.srs_parameter[10] = 0;
    g_setting_comval.music_comval.srs_parameter[11] = 0;

    //���ÿؼ�
    set_vals = gui_set_parameter(SLIDER_SETEQ, &sound_param_comdata);

    if (set_vals == RESULT_CONFIRM)
    {
        if (check_fullsound_close(&set_vals) == TRUE)
        {

            //�������
            for (i = 0; i < 5; i++)
            {
                g_setting_comval.music_comval.srs_parameter[i] = (int8)(sound_param_box[i].value + 5);
            }
            //���ò���
            g_setting_comval.music_comval.srs = VALUE_USER_SRS_ON;
            //eq�ر�
            g_setting_comval.music_comval.eq_setting = VALUE_EQ_OFF;
            //foullsound�ر�
            g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
            //����Ӳ��EQ
            eq_callback(sound_param_box, EQ_TYPE_USR_MODE, 0);
            //���ص���һ���˵�
            return RESULT_MENU_PARENT;
        }
    }

    //�ָ�֮ǰ��EQ�趨
    _scene_play_seteq(g_setting_comval.music_comval.eq_setting);

    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_speed_sure(void *param)
 * \���ò����ٶ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the resul t
 * retval RESULT_IGNORE ����
 * retval
 * ingroup music_setmenu_soundset.c
 * note
 */
/**************************************************************************** ***/
app_result_e set_scene_speed_sure(void *param)
{
    app_result_e ret_vals;

    //�ؼ���������
    sound_param_box[0].min = -8;
    sound_param_box[0].max = 8;
    sound_param_box[0].step = 1;
    sound_param_box[0].value = g_setting_comval.music_comval.variable_playback;
    sound_param_box[0].cycle = FALSE;
    sound_param_box[0].adjust_func = NULL;
    sound_param_box[0].callback = set_scene_speed_callback;
    sound_param_box[0].max_number = 1;

    sound_param_box[0].value_str = NULL;
    //��λ
    sound_param_box[0].unit_id = S_SPEED_UNIT;

    //����˽������
    sound_param_private.param_cnt = 1;
    sound_param_private.active = 0;
    sound_param_private.items = &sound_param_box[0];

    sound_param_private.icon_id = -1;
    sound_param_private.title_id = -1;
    sound_param_private.back_id = -1;
    sound_param_private.sign_flag = 1;//��ʾ����

    //��������
    sound_param_comdata.private_data = &sound_param_private;
    //���ÿؼ�
    ret_vals = gui_set_parameter(SLIDER_NUM_SYM, &sound_param_comdata);

    if (ret_vals == RESULT_CONFIRM)
    {
        if (check_fullsound_close(&ret_vals) == TRUE)
        {

            set_scene_speed_callback(sound_param_box[0].value);

            g_setting_comval.music_comval.variable_playback = (int8) sound_param_box[0].value;
            //srs �ر�
            g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
            //eq�ر�
            g_setting_comval.music_comval.eq_setting = VALUE_EQ_OFF;
            //foullsound�ر�
            g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
            //�ػ浱ǰ���˵�
            return RESULT_REDRAW;
        }
    }

    //�ָ�֮ǰ��EQ�趨
    _scene_play_seteq(g_setting_comval.music_comval.eq_setting);

    return ret_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_vollimit_sure(void *param)
 * \��������
 * \param[in]    void  para1
 * par am[out] none
 * return app_result_e the result
 * retval RESULT_IGNORE ����
 * retval
 * ingroup music_setmenu_soundset.c
 * note
 */
/********************************************************************** *********/
app_result_e set_scene_vollimit_sure(void *param)
{
    app_result_e result;

    result = gui_volumebar(&g_setting_comval.g_comval.volume_current, &g_setting_comval.g_comval.volume_limit,
            SET_VOLUME_LIMIT);
    if ((result == RESULT_REDRAW) || (result == RESULT_CONFIRM))
    {
        //�ػ浱ǰ���˵�
        return RESULT_REDRAW;
    }
    else
    {
        return result;
    }
}
