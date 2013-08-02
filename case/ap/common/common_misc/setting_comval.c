/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-5-11 12:34     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _setting_comval.c
 * \brief    common ϵͳ���ù���
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ���������ȡĬ�� setting_comval ֵ����ʼ�� sys_comval ָ�룬�ȵ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-5-11
*******************************************************************************/

#include "common_ui.h"

static const int8 user_srs_param_default[12] = 
{ 8, 8, 5, 4, 6, 1, 0, 30, 1, 0, 0, 0 };//srs user default

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��config.bin�ж�ȡĬ������
 * \param[in]    setting_comval ����������Ϣ�ṹ��ָ��
 * \param[out]   none 
 * \return       none
 * \ingroup      misc_func
 * \note 
*******************************************************************************/
void com_get_default_setting(setting_comval_t *setting_comval)
{
    //�������ȵȼ�
    setting_comval->g_comval.lightness = (uint8)com_get_config_default(SETTING_AP_ID_LIGHTNESS);
    //���� 0����ɫ 1����ɫ 2����ɫ 3���ۺ�ɫ 4����ɫ
    setting_comval->g_comval.theme = (uint8) com_get_config_default(SETTING_AP_ID_THEME);
    //��Ļ���� 0: �� 1: ����ʱ�� 2�����ͼ�� 3���ر���Ļ 4����ʾģʽ
    setting_comval->g_comval.screen_saver_mode = (uint8) com_get_config_default(SETTING_AP_ID_SCREEN_SAVER_MODE);
    //����ʱ�� ��0.5��Ϊ��λ��0��ʾ����
    setting_comval->g_comval.light_timer = (uint8) com_get_config_default(SETTING_AP_ID_LIGHT_TIMER);
    //��Ļ����ʱ�� ��0.5��Ϊ��λ��0��ʾ����Ļ����
    setting_comval->g_comval.screen_saver_timer = (uint8) com_get_config_default(SETTING_AP_ID_SCREEN_SAVER_TIMER);
    //˯��ʱ��,�ѷ���Ϊ��λ��0��ʾ����ʱ�ػ�
    setting_comval->g_comval.sleep_timer = (uint8) com_get_config_default(SETTING_AP_ID_SLEEP_TIMER);
    //ʡ��ػ�ʱ�� ��0.5��Ϊ��λ��0��ʾ��ʡ��ػ�
    setting_comval->g_comval.poweroff_timer = (uint8) com_get_config_default(SETTING_AP_ID_POWEROFF_TIMER);
    //��ʱ�������ڲ��Ž��� ��0.5��Ϊ��λ��0��ʾ���뷵��
    setting_comval->g_comval.playing_timer = (uint8) com_get_config_default(SETTING_AP_ID_PLAYING_TIMER);
    //ʱ���ʽ 0��12Сʱ�� 1��24Сʱ��
    setting_comval->g_comval.time_format = (uint8) com_get_config_default(SETTING_AP_ID_TIME_FORMAT);
    //���ڸ�ʽ 0��DD_MM_YYYY 1��MM_DD_YYYY 2��YYYY_MM_DD
    setting_comval->g_comval.date_format = (uint8) com_get_config_default(SETTING_AP_ID_DATE_FORMAT);
    //��������
    setting_comval->g_comval.language_id = (uint8) com_get_config_default(SETTING_AP_ID_LANGUAGE_ID);
    //����ģʽ��0��MSC 1��MTP
    setting_comval->g_comval.online_device = (uint8) com_get_config_default(SETTING_AP_ID_ONLINE_DEVICE);
    //autorun֧��ѡ�� 0 ��autorun��1�� ֧��autorun
    setting_comval->g_comval.autorun_set = (uint8) com_get_config_default(SETTING_AP_ID_AUTORUN_SET);
    //֧�ֿ�ѡ��  0:��֧��  1:֧��
    setting_comval->g_comval.support_card = (uint8) com_get_config_default(SETTING_AP_ID_SUPPORT_CARD);
    setting_comval->g_comval.volume_limit = (uint8) com_get_config_default(SETTING_AP_ID_VOLUME_LIMIT);
    setting_comval->g_comval.volume_current = (uint8) com_get_config_default(SETTING_AP_ID_VOLUME_CURRENT);
    setting_comval->g_comval.slide_time = (uint8) com_get_config_default(SETTING_AP_ID_SLIDE_TIME);
    setting_comval->g_comval.slide_repeat_mode = (uint8) com_get_config_default(SETTING_AP_ID_SLIDE_REPEAT_MODE);
    setting_comval->g_comval.slide_shuffle = (uint8) com_get_config_default(SETTING_AP_ID_SLIDE_SHUFFLE);
    setting_comval->g_comval.dark_lightoff_timer = (uint8) com_get_config_default(SETTING_AP_ID_DARK_LIGHTOFF_TIMER);
    setting_comval->g_comval.screen_lightoff_timer=(uint8)com_get_config_default(SETTING_AP_ID_SCREEN_LIGHTOFF_TIMER);
    //��ȡ������������
    setting_comval->g_comval.key_tone = (uint8) com_get_config_default(SETTING_AP_ID_KEYTONE_ENABLE);
    
    //get music setting default
    setting_comval->music_comval.music_from = (uint8) com_get_config_default(SETTING_AP_ID_MUSIC_FROM);
    setting_comval->music_comval.music_repeat_mode = (uint8) com_get_config_default(SETTING_AP_ID_MUSIC_REPEAT_MODE);
    setting_comval->music_comval.music_shuffle = (uint8) com_get_config_default(SETTING_AP_ID_MUSIC_SHUFFLE);
    setting_comval->music_comval.fullsound = (uint8) com_get_config_default(SETTING_AP_ID_FULLSOUND);
    setting_comval->music_comval.srs = (uint8) com_get_config_default(SETTING_AP_ID_SRS);
    setting_comval->music_comval.variable_playback = (int8) com_get_config_default(SETTING_AP_ID_VARIABLE_PLAYBACK);    
    setting_comval->music_comval.eq_setting = (uint8) com_get_config_default(SETTING_AP_ID_EQ_SETTING);
    
    libc_memset(setting_comval->music_comval.eq_parameter, 0, 12);
    libc_memcpy(setting_comval->music_comval.srs_parameter, user_srs_param_default, 12);
}

/*! \endcond */
