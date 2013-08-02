/*******************************************************************************
 *                              us212A
 *                            Module: manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-28 14:42     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_applib_funs.c
 * \brief    applib ���̹���ģ�飨bank���֣�ʵ��
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ��һ�׻��� ���̹��� �ӿڡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               ���� libc �� psp �Ľӿ�ʵ�֡�
 * \version 1.0
 * \date  2011-9-28
 *******************************************************************************/

#include "applib.h"
extern bool open_config_file(void);
/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ���� applib ģ���ϵĳ�ʼ��������Ӧ�õ� main ������ڴ�����
 * \param[in]    app_id��Ӧ��ID
 * \param[in]    type��Ӧ�����ͣ��μ�app_type_e ����
 * \param[out]   none
 * \return       bool
 * \retval           �����Ƿ��ʼ���ɹ���true��ʾ�ɹ���false��ʾʧ��
 * \note
 *******************************************************************************/
bool applib_init(uint8 app_id, app_type_e type)
{
    app_info_t *cur_app_info;

    if (app_id >= APP_ID_MAX)
    {
        return FALSE;
    }

    //��ȡ app_info �ṹ��ָ��
    cur_app_info = &g_app_info_vector[type];

    //��������ֹ�������
    sys_os_sched_lock();

    //��ʼ��Ӧ����Ϣ
    libc_memset(cur_app_info, 0x0, sizeof(app_info_t));
    cur_app_info->used = 1;//��־�ѱ�����ʹ��
    cur_app_info->app_id = app_id;
    cur_app_info->app_type = (uint8) type;
    if (type == APP_TYPE_PMNG)
    {
        cur_app_info->mq_id = (uint8) MQ_ID_MNG;
    }

    //�����������������
    sys_os_sched_unlock();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ȫ�����ݳ�ʼ�����ڽ��̹�������ͷ���ã��ڵ��� applib_init ǰ��
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           �����Ƿ��ʼ���ɹ���true��ʾ�ɹ���false��ʾʧ��
 * \note
 *******************************************************************************/
bool globe_data_init(void)
{
    uint8 i;

    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        libc_memset(&(g_app_info_vector[i]), 0x00, sizeof(app_info_t));
        g_app_info_vector[i].app_id = APP_ID_MAX;
    }

    g_app_info_state.type = ENGINE_NULL;
    g_app_info_state.state = ENGINE_STATE_NULL;
    g_app_info_state.app_state = APP_STATE_NO_PLAY;
    g_app_info_state.filter_key = KEY_NULL;
    g_app_info_state.keylock_state = FALSE;
    g_app_info_state.sound_state = SOUND_OUT_STOP;
    g_app_info_state.volume = 0;
    g_app_info_state.backlight_state = BACKLIGHT_STATE_NORMAL;
    g_app_info_state.screensave_state = FALSE;
    g_app_info_state.card_state = CARD_STATE_CARD_NULL;
#ifndef PC
    g_app_info_state.cable_state = CABLE_STATE_CABLE_NULL;
#else
    g_app_info_state.cable_state = CABLE_STATE_CABLE_IN;
#endif
    g_app_info_state.antenna_state = FALSE;
    g_app_info_state.charge_state = CHARGE_NONE;
    g_app_info_state.bat_value = 5;
    g_app_info_state.card_plist = FALSE;
    g_app_info_state.video_state = 0;
    g_app_info_state.card_update = FALSE;

    g_app_last_state.last_app = APP_ID_MAX;
    g_app_last_state.last_type = ENGINE_NULL;
    g_app_last_state.last_state = ENGINE_STATE_NULL;
    g_app_last_state.last_app_state = APP_STATE_NO_PLAY;
    g_app_last_state.last_card_state = CARD_STATE_CARD_NULL;
#ifndef PC
    g_app_last_state.last_cable_state = CABLE_STATE_CABLE_NULL;
#else
    g_app_last_state.last_cable_state = CABLE_STATE_CABLE_IN;
#endif
    g_app_last_state.last_light_timer = 0;
    g_app_last_state.last_screen_save_mode = 0;
    g_app_last_state.last_sleep_timer = 0;

    libc_memset(&g_sys_counter, 0x00, sizeof(sys_counter_t));
    g_bat_index = 0;

    return open_config_file();
}
