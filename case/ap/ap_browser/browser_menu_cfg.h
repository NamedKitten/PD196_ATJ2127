/******************************************************************************
 *                               USDK
 *                            Module: app
 *                 Copyright(c) 2002-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File: browser_cfg_menu.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyufan    2011-8-22 13:29:40      1.0             build this file
 ******************************************************************************/
 /*!
 * \file     browser_menu_cfg.h
 * \brief    browser�˵�����ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               �˵�����ͷ�ļ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               ��ģ�鲻�ᱻӦ�õ��ã�����ͨ���������ļ����ɵ�Ŀ���ļ��Ӷ�����
 *               �˵������ļ�
 * \version 1.0
 * \date    2011-8-22
 *******************************************************************************/
 #ifndef _BROWSER_MENU_CFG_H
 #define _BROWSER_MENU_CFG_H
 
 #include "psp_includes.h"
#include "case_include.h"	
 
 /*! ����browser��ڲ˵���� ��5����ڲ˵�*/
#define BROWSER_MENU_ENTRY_NOPLAY    0	//ɾ��
#define BROWSER_MENU_ENTRY_PLAYING   1	//���ڲ���/ɾ��
#define BROWSER_MENU_ENTRY_LASTPLAY  2  //��һ�β���/ɾ��
#define BROWSER_MENU_ENTRY_CARD      3  //����Ŀ¼/��Ŀ¼
#define BROWSER_MENU_ENTRY_DEL_ALL   4  //ȫ��ɾ�� Ϊrecord/radioӦ��ui_directoryǶ�ײ˵���

//Ҷ�Ӳ˵� �ṩ���˵������ļ�

extern app_result_e menu_func_main_disk(void *param);

extern app_result_e menu_func_card_disk(void *param);

extern app_result_e menu_func_del_all(void *param);

extern app_result_e menu_func_playing(void * param);

extern app_result_e menu_func_last_play(void * param);

extern app_result_e menu_func_del_file(void *param);

extern app_result_e menu_func_option_option(void);

#ifdef PC

extern menu_cb_func _get_menu_func(uint16 str_id);

extern menu_cb_leaf _get_callback_func(uint16 str_id);

extern menu_cb_option _get_menu_option(uint16 str_id);

#endif

#endif
