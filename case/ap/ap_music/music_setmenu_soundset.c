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

//EQ���������Զ���
typedef enum
{
    EQ_PARAM_NONE = 0x0,
    EQ_PARAM_ROCK,
    EQ_PARAM_FUNK,
    EQ_PARAM_HIPHOP,
    EQ_PARAM_JAZZ,
    EQ_PARAM_CALSSIC,
    EQ_PARAM_TECHNO,
    EQ_PARAM_CUSTOM,
    EQ_PARAM_FULLSOUND,
    EQ_PARAM_USEREQ,
    EQ_PARAM_USERSRS,
    EQ_PARAM_WOWSRS
}eq_param_e;

//const data
const int8 eq_param[9][12] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //none
    { 5, 3, 0, -1, 0, 4, 5, 6, 0, 0, 0, 0 }, //rock
    { 3, 0, 0, 0, 0, -1, -2, 6, 0, 0, 0, 0 }, //funk--pop
    { 0, 0, 1, 2, 0, 1, 1, 6, 0, 0, 0, 0 }, //hiphop--soft
    { 3, 0, 0, -1, 0, 2, 4, 6, 0, 0, 0, 0 }, //Jazz
    { 0, 0, 0, -1, -1, -2, -4, 6, 0, 0, 0, 0 }, //Classic
    { 5, 2, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 }, //techno--dbb
    { -9, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 }, //custom--speaker
    { 10, 4, 50, 0, 0, 0, 6, 0, 0, 0, 0, 0 } //fullsound on--default value

};

const int8 srs_wowhd_param[] =
{
    8, 8, 5, 4, 6, 1, 0, 30, 1, 0 //srs
};

/******************************************************************************/
/*!
 * \par  Description:
 ��Ҷ�Ӳ˵�����ʱ���ݵ�ǰ���õ�EQ��������EQ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool
 * \ingroup      music_setmenu_soundset.c
 * \note
 * \li  ����Ҷ�Ӳ˵�֧��ʵʱ�����ص�����˿��ܸı�ʵ�ʵ�EQ�����ظ��˵�ʱ���øú���
 �ָ������赱ǰEQ
 */
/*******************************************************************************/
static void restore_music_eq(void)
{
    _scene_play_seteq(g_setting_comval.music_comval.eq_setting);
}

/******************************************************************************/
/*!
 * \par  Description:
 ���fullsound����
 * \param[in]    ask_result  ����ֵ����
 * \param[out]   ask_result  ����ֵ����
 * \return       bool
 * \retval       TRUE δ��or�ر�
 * \retval       FALSE ���Ҳ��ر�
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
bool check_fullsound_close(app_result_e* ask_result)
{
    bool check_ret = TRUE;
    app_result_e dialog_retval;
    dialog_com_data_t fullsound_ask_dialog;
    string_desc_t fullsound_str;
    if (g_setting_comval.music_comval.fullsound == VALUE_FULLSOUND_ON)
    {
        //�Ի�����ʾ�Ƿ�ر�
        fullsound_str.data.id = S_FULLSOUND_EQ_MUX;
        fullsound_str.language = UNICODEID;
        //�Ի�������
        fullsound_ask_dialog.dialog_type = DIALOG_BUTTON_INCLUDE;
        fullsound_ask_dialog.icon_id = -1;
        //�ַ���
        fullsound_ask_dialog.string_desc = &fullsound_str;

        //��������
        fullsound_ask_dialog.button_type = BUTTON_YESNO;
        /*! Ĭ�ϼ������ */
        fullsound_ask_dialog.active_default = 0x01;
        dialog_retval = gui_dialog(DIALOG_ASK, &fullsound_ask_dialog);
        if (dialog_retval != RESULT_DIALOG_YES)
        {
            check_ret = FALSE;
            *ask_result = dialog_retval;
        }
    }
    return check_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 fullsound���˵�ִ�к��������ݵ�ǰ��EQֵ���м�����ƥ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_fullsound_sure(void* param)
{
    //ָ�򷵻ز�����ָ��
    menu_title_action_t* retptr = (menu_title_action_t*) param;

    //ȷ���ַ���id
    switch (g_setting_comval.music_comval.fullsound)
    {
        //fullsound off
        case VALUE_FULLSOUND_OFF:
        retptr->str_id = S_OFF;
        break;
        //fullsound on
        case VALUE_FULLSOUND_ON:
        retptr->str_id = S_ON;
        break;
        default:
        retptr->str_id = -1;
        break;
    }
    //���÷��ص�ǰ���˵��Ļص�����
    g_eq_restore = 1;
    gui_menulist_set_back2parent(restore_music_eq, &g_eq_restore);
    //������һ���˵�
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 EQ���ø��˵�ȷ��ִ�к��������ݵ�ǰ��EQֵ���м�����ƥ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eq_sure(void* param)
{
    //ָ�򷵻ز�����ָ��
    menu_title_action_t* retptr = (menu_title_action_t*) param;

    //ȷ���ַ���id
    switch (g_setting_comval.music_comval.eq_setting)
    {
        //eq off
        case VALUE_EQ_OFF:
        retptr->str_id = S_OFF;
        break;
        //eq rock
        case VALUE_EQ_ROCK:
        retptr->str_id = S_ROCK;
        break;
        //ea funk
        case VALUE_EQ_FUNK:
        retptr->str_id = S_FUNK;
        break;
        //eq hip hop
        case VALUE_EQ_HIP_HOP:
        retptr->str_id = S_HIPHOP;
        break;
        //eq jazz
        case VALUE_EQ_JAZZ:
        retptr->str_id = S_JAZZ;
        break;
        //eq classical
        case VALUE_EQ_CLASSICAL:
        retptr->str_id = S_CLASSIC;
        break;
        //eq teckno
        case VALUE_EQ_TECHNO:
        retptr->str_id = S_TECHNO;
        break;
        //eq custom
        case VALUE_EQ_CUSTOM:
        retptr->str_id = S_CUSTOM;
        break;

        default:
        retptr->str_id = -1;
        break;
    }

    //���÷��ص�ǰ���˵��Ļص�����
    g_eq_restore = 1;
    gui_menulist_set_back2parent(restore_music_eq, &g_eq_restore);
    //������һ���˵�
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 SRS���ø��˵�ȷ��ִ�к��������ݵ�ǰ��EQֵ���м�����ƥ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_srs_sure(void* param)
{
    //ָ�򷵻ز�����ָ��
    menu_title_action_t* retptr = (menu_title_action_t*) param;

    //ȷ���ַ���id
    switch (g_setting_comval.music_comval.srs)
    {
        //srs off
        case VALUE_SRS_OFF:
        retptr->str_id = S_OFF;
        break;
        //srs wowhd
        case VALUE_WOWHD_ON:
        retptr->str_id = S_SRS_WOWHD;
        break;
        //srs user
        case VALUE_USER_SRS_ON:
        retptr->str_id = S_SRS_USER;
        break;
        default:
        retptr->str_id = -1;
        break;
    }
    //���÷��ص�ǰ���˵��Ļص�����
    g_eq_restore = 1;
    gui_menulist_set_back2parent(restore_music_eq, &g_eq_restore);
    //������һ���˵�
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 fullsound�ر�Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_fullsoundoff_sure(void *param)
{
    //����eqģʽ
    //mmm_mp_eq_info_t eq_info;
    //eq_info.eq_type = 0x00;
    //eq_info.eq_para = eq_param[0];
    //libc_memcpy(eq_info.eq_para, eq_param[0], sizeof(eq_param[0]));
    //music_set_eq(&eq_info);
    //foullsound�ر�
    g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 fullsound����Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_fullsoundon_sure(void *param)
{
    //����eqģʽ
    //mmm_mp_eq_info_t eq_info;
    //eq_info.eq_type = 0x07;
    //eq_info.eq_para = eq_param[8];
    //libc_memcpy(eq_info.eq_para, eq_param[8], sizeof(eq_param[8]));
    //music_set_eq(&eq_info);
    //foullsound��
    g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_ON;
    //eq�ر�
    g_setting_comval.music_comval.eq_setting = VALUE_EQ_OFF;
    //srs�ر�
    g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
    //speed�ر�
    g_setting_comval.music_comval.variable_playback = 0;
    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪnormalģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqnormal_sure(void *param)
{
    //����eqģʽ
    //mmm_mp_eq_info_t eq_info;
    //eq_info.eq_type = 0x00;
    //eq_info.eq_para = eq_param[0];
    //libc_memcpy(eq_info.eq_para, eq_param[0], sizeof(eq_param[0]));
    //music_set_eq(&eq_info);
    //eq�ر�
    g_setting_comval.music_comval.eq_setting = VALUE_EQ_OFF;
    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪrockģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqrock_sure(void *param)
{
    //����eqģʽ
    //mmm_mp_eq_info_t eq_info;
    app_result_e set_vals;
    //eq_info.eq_type = 0x07;
    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, eq_param[1], sizeof(eq_param[1]));
        //music_set_eq(&eq_info);
        //eq rock
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_ROCK;
        //foullsound�ر�
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //srs�ر�
        g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
        //speed�ر�
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //���ص���һ���˵�
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪfunkģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqfunk_sure(void *param)
{
    //����eqģʽ
    //mmm_mp_eq_info_t eq_info;
    app_result_e set_vals;
    //eq_info.eq_type = 0x07;
    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, eq_param[2], sizeof(eq_param[2]));
        //music_set_eq(&eq_info);
        //eq funk
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_FUNK;
        //foullsound�ر�
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //srs�ر�
        g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
        //speed�ر�
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //���ص���һ���˵�
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪhiphopģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqhiphop_sure(void *param)
{
    //����eqģʽ
    //mmm_mp_eq_info_t eq_info;
    app_result_e set_vals;
    //eq_info.eq_type = 0x07;
    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, eq_param[3], sizeof(eq_param[3]));
        //music_set_eq(&eq_info);
        //eq funk
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_HIP_HOP;
        //foullsound�ر�
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //srs�ر�
        g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
        //speed�ر�
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //���ص���һ���˵�
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪjazzģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqjazz_sure(void *param)
{
    //����eqģʽ
    //mmm_mp_eq_info_t eq_info;
    app_result_e set_vals;
    //eq_info.eq_type = 0x07;
    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, eq_param[4], sizeof(eq_param[4]));
        //music_set_eq(&eq_info);
        //eq funk
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_JAZZ;
        //foullsound�ر�
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //srs�ر�
        g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
        //speed�ر�
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //���ص���һ���˵�
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪclassicalģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqclassical_sure(void *param)
{
    //����eqģʽ
    //mmm_mp_eq_info_t eq_info;
    app_result_e set_vals;
    //eq_info.eq_type = 0x07;
    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, eq_param[5], sizeof(eq_param[5]));
        //music_set_eq(&eq_info);
        //eq funk
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_CLASSICAL;
        //foullsound�ر�
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //srs�ر�
        g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
        //speed�ر�
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //���ص���һ���˵�
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪtechnoģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqtechno_sure(void *param)
{
    //����eqģʽ
    //mmm_mp_eq_info_t eq_info;
    app_result_e set_vals;
    //eq_info.eq_type = 0x07;
    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, eq_param[6], sizeof(eq_param[6]));
        //music_set_eq(&eq_info);
        //eq funk
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_TECHNO;
        //foullsound�ر�
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //srs�ر�
        g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
        //speed�ر�
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //���ص���һ���˵�
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪSRS�ر�Ҷ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_srsoff_sure(void *param)
{
    //����eqģʽ
    //mmm_mp_eq_info_t eq_info;
    //eq_info.eq_type = 0x00;

    //libc_memcpy(eq_info.eq_para, eq_param[0], sizeof(eq_param[0]));
    //music_set_eq(&eq_info);
    //srs�ر�
    g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪSRS whdģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_srs_wowhd_sure(void *param)
{
    //����eqģʽ
    //mmm_mp_eq_info_t eq_info;

    app_result_e set_vals;

    //eq_info.eq_type = 0x4f;

    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, srs_param, sizeof(srs_param));
        //music_set_eq(&eq_info);
        //srs
        g_setting_comval.music_comval.srs = VALUE_WOWHD_ON;
        //eq�ر�
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_OFF;
        //foullsound�ر�
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //speed�ر�
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //���ص���һ���˵�
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 ����ʵ�ʵ�EQ�������ص�����
 * \param[in]    eq_type  EQ���ͣ��μ�mmm_mp_eqmode_t
 * \param[in]    eq_param_index EQ�������μ�eq_param_e
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
static app_result_e set_scene_set_real_eq(uint8 eq_type, uint8 eq_param_index)
{
    //����eqģʽ
    mmm_mp_eq_info_t eq_info;

    eq_info.eq_type = eq_type;

    if (eq_type == EQ_TYPE_USR_MODE)
    {
        if (eq_param_index == EQ_PARAM_USERSRS)
        {
            libc_memcpy(eq_info.eq_para, g_setting_comval.music_comval.srs_parameter, 12);
        }
        else
        {
            libc_memcpy(eq_info.eq_para, srs_wowhd_param, sizeof(srs_wowhd_param));
        }
    }
    else if (eq_param_index == EQ_PARAM_USEREQ)
    {
        libc_memcpy(eq_info.eq_para, g_setting_comval.music_comval.eq_parameter, 12);
    }
    else
    {
        libc_memcpy(eq_info.eq_para, eq_param[eq_param_index], sizeof(eq_param[eq_param_index]));
    }
    music_set_eq(&eq_info);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪNORMAL��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqnormal_callback(void)
{
    set_scene_set_real_eq(EQ_NORMAL, EQ_PARAM_NONE);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪfullsound��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_fullsoundon_callback(void)
{
    set_scene_set_real_eq(EQ_FULLSOUND_MODE, EQ_PARAM_FULLSOUND);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪrock��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqrock_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_ROCK);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪfunk��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqfunk_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_FUNK);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪhiphop��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqhiphop_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_HIPHOP);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪjazz��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqjazz_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_JAZZ);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪclassical��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqclassic_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_CALSSIC);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪtechno��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqtechno_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_TECHNO);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪequser��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_equser_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_USEREQ);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪsrswowhd��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_srswowhd_callback(void)
{
    set_scene_set_real_eq(EQ_TYPE_USR_MODE, EQ_PARAM_WOWSRS);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪsrsuser��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_srsuser_callback(void)
{
    set_scene_set_real_eq(EQ_TYPE_USR_MODE, EQ_PARAM_USERSRS);
}

