/********************************************************************************
 *                            Module: radio_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      mikeyang         2011-09-13 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_RADIO_MENU_CONFIG_H__
#define __APP_RADIO_MENU_CONFIG_H__

#include "psp_includes.h"
#include "case_include.h"

//��һ����ڲ˵�
//Ԥ���̨+  �û���̨+  �ֶ���г+  �Զ���г+  FM ¼��
#define  MAINMENU_ENTRY       0

//��̨�б�option ��ڲ˵�
//���Ԥ��+  �������Ԥ��
#define  OPTION_MENU_STATION   1

//���Ž���option ��ڲ˵�
//��ʼFM ������¼��+  ���浽Ԥ�� +  ���Ԥ��+  �Զ���г+   ��̨Ƶ��+  FM ¼��
#define  OPTION_PLAYWIN       2

//�˵���������
extern app_result_e menu_scene_saved_preset(void *param)
__FAR__;
extern app_result_e menu_scene_user_preset(void *param)
__FAR__;
extern app_result_e menu_scene_manual_tune(void *param)
__FAR__;
extern app_result_e menu_scene_auto_tune(void *param)
__FAR__;
extern app_result_e menu_scene_voice(void *param)
__FAR__;

extern app_result_e menu_scene_delone(void *param)
__FAR__;
extern app_result_e menu_scene_delall(void *param)
__FAR__;

extern app_result_e menu_scene_start_rec(void *param)
__FAR__;
extern app_result_e menu_scene_saveone(void *param)
__FAR__;
extern app_result_e menu_scene_delone_2(void *param)
__FAR__;
extern app_result_e menu_scene_selband(void *title_action)
__FAR__;

extern app_result_e menu_scene_uband(void *param)
__FAR__;
extern app_result_e menu_scene_jband(void *param)
__FAR__;
extern app_result_e menu_scene_eband(void *param)
__FAR__;

extern app_result_e menu_option_callback(void)
__FAR__;
extern app_result_e menu_return_list(void)
__FAR__;
extern app_result_e menu_return_play(void)
__FAR__;

#ifdef PC

extern menu_cb_option get_menu_option(uint16 str_id);

extern menu_cb_leaf get_callback_func(uint16 str_id);

extern menu_cb_func get_menu_func(uint16 str_id);

#endif

#endif //__APP_RADIO_MENU_CONFIG_H__
