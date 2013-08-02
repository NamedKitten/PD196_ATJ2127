/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_language.C
 * History:
 *      <johnsen>    <2011-9-25 19:25>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ap_setting.h"
#include "menu_callback_language.h"


const uint16 language_text_table[LANGUAGE_TOTAL] =
{ 
    S_LANGUAGE_CZECH, S_LANGUAGE_GREEK, S_LANGUAGE_ENGLISH, S_LANGUAGE_DENISH, S_LANGUAGE_GERMANY,
    S_LANGUAGE_SPANISH, S_LANGUAGE_FRENCH, S_LANGUAGE_ITALIAN, S_LANGUAGE_HUNGARIAN, S_LANGUAGE_DUTCH,
    S_LANGUAGE_NORWEGIAN, S_LANGUAGE_POLAND, S_LANGUAGE_PORTUGUESE_EUROPEAN, S_LANGUAGE_PORTUGUESE_BRAZILIAN,
    S_LANGUAGE_RUSSIAN, S_LANGUAGE_SLOVAK, S_LANGUAGE_FINNISH, S_LANGUAGE_SWEDISH, S_LANGUAGE_TURKEY, 
    S_LANGUAGE_SIMPLIFIED_CHINESE,S_LANGUAGE_TRAD_CHINESE, S_LANGUAGE_JAPANESE, S_LANGUAGE_KOREAN,
    S_LANGUAGE_HEBREW,S_LANGUAGE_ARABIC,S_LANGUAGE_THAI,S_LANGUAGE_ROMANIAN,S_LANGUAGE_INDONESIAN
}; /*�˱��������ʾ����������ȫһ�£�����ʾ�������������ұ���������������*/


//һ���˵�-language/����
//language/����
//             |-�ݿ���
//             |-ϣ����
//             |-Ӣ��
//             |-������
//             |-����
//             |-��������
//             |-����
//             |-�������
//             |-��������
//             |-������
//             |-Ų����
//             |-������
//             |-��������-ŷ��
//             |-��������-����
//             |-����
//             |-˹�工����
//             |-������
//             |-�����
//             |-��������
//             |-��������
//             |-��������
//             |-����
//             |-����
//             |-ϣ������
//             |-��������
//             |-̩��
//             |-������
//             |-ӡ����
//==============���漸���ݲ�֧��=====================
//             |-���޵�����
//             |-˹����������



////////////////////

//�������б�˵��˳�ʱ�ص�����
void set_language_id_back2parent(void)
{
    ui_set_lang_option(LANG_LIST_MENU_EXIT);
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ�õ�ǰ����ѡ��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��������ѡ��string ID
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e get_language_id_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_language;
    menu_title_language = (menu_title_action_t*) title_action;
    active_id = language_text_table[g_setting_vars.g_comval.language_id];
    menu_title_language->menu_entry = -1; //���������
    menu_title_language->str_id = active_id; //ָ��active��
    menu_title_language->ret_str = NULL;
    menu_title_language->source_id = -1;
    
    //���������б�˵����˵����ַ������⴦��
    ui_set_lang_option(LANG_LIST_MENU_ENTER);
    //�������б�˵��˳�ʱ���� set_language_id_back2parent
    set_language_id_argv = 1;
    gui_menulist_set_back2parent(set_language_id_back2parent, &set_language_id_argv);
    
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ��������
 *      �ص�˵��m
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_simplified_chinese_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_SIMPLIFIED_CHINESE;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������ΪӢ��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_english_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_ENGLISH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ��������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_trad_chinese_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_TRAD_CHINESE;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_japanese_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_JAPANESE;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_korean_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_KOREAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_french_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_FRENCH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_germany_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_GERMANY;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ�������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_italian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_ITALIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_dutch_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_DUTCH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}


/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ��������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_spanish_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_SPANISH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ�����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_swedish_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_SWEDISH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ�ݿ���
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_czech_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_CZECH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_denish_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_DENISH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص������������ڽ���������Ϊ������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_poland_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_POLAND;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_russian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_RUSSIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ��������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_turkey_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_TURKEY;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊϣ������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_hebrew_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_HEBREW;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ̩��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_thai_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_THAI;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ��������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_hungarian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_HUNGARIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_romanian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_ROMANIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ��������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_arabic_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_ARABIC;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ���޵�����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_croatian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_CROATIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ˹����������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_slovenian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_SLOVENIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}


/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊϣ����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_greek_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_GREEK;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������ΪŲ����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_norwegian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_NORWEGIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ��������-ŷ��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_portuguese_european_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_PORTUGUESE_EUROPEAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ��������-����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_portuguese_brazilian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_PORTUGUESE_BRAZILIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ˹�工����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_slovak_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_SLOVAK;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊ������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_finnish_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_FINNISH;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}


/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���������Ϊӡ����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_indonesian_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.language_id = LANGUAGE_ID_INDONESIAN;
    ui_set_language((uint32) g_setting_vars.g_comval.language_id);
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

