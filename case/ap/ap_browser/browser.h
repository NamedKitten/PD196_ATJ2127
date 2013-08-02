/******************************************************************************
 *                               US212A
 *                            Module: BROWSER
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan      2011-10-25 14:41:35          1.0              build this file
 ******************************************************************************/
/*!
 * \file     browser.h
 * \brief    browserģ��ͷ�ļ�
 * \author   wuyufan
 * \version  1.0
 * \date     2011-10-25
 *******************************************************************************/
#ifndef _BROWSER_H_
#define _BROWSER_H_

#include "psp_includes.h"
#include "case_include.h"
#include "browser_res.h"
#include "browser_sty.h"

/*!����browserӦ�õ�����ջ*/
#define    AP_BROWSER_STK_POS  AP_FRONT_LOW_STK_POS

/*!����browserӦ�õ����߳����ȼ�*/
#define    AP_BROWSER_PRIO     AP_FRONT_LOW_PRIO

/*!Ӧ���Լ���Ҫ����ʱ������*/
#define APP_TIMER_COUNT		1

#define APP_TIMER_TOTAL		(COMMON_TIMER_COUNT + APP_TIMER_COUNT)

#define DISK_BASE           DISK_C

//#define AUTO_CHANGE_DISK  //�Ƿ�֧���Զ��л��̷�

/*!
 * \brief
 *  scene_browser_e browser����ö������
 */
typedef enum
{
    /*! �˳����� */
    SCENE_EXIT = 0,
    /*! �б��� */
    SCENE_FILELIST = 1,
    /*! �˵����� */
    SCENE_MENU = 2
} scene_browser_e;

/* ȫ�ֱ������� */

/*! \brief browserӦ��VM����*/
extern browser_var_t g_browser_var;

/*! \brief ͨ��VM����*/
extern comval_t g_comval;

/*! \brief browser��һ������ */
extern scene_browser_e g_browser_scene_next;

/*! \brief browser �����ļ�ϵͳ��mount ID*/
extern int32 browser_mount;

/*! \brief Ӧ�ý���ģʽ*/
extern app_param_e g_browser_enter_mode;

/*! \brief �Ƿ��Ǵ�����ap����browser*/
extern bool g_return_browser;

/*! \brief browser�ļ��� */
extern uint8 browser_file_name[LIST_ITEM_BUFFER_LEN];


/* �������� */

extern app_result_e browser_msg_callback(private_msg_t *msg);

extern app_result_e browser_scene_filelist(void) __FAR__;

extern app_result_e browser_scene_menu(void);

extern app_result_e browser_menu_option(file_path_info_t *path_info, uint16 active);

extern fsel_error_e browser_fsel_initial(uint8 disk_flag) __FAR__;

extern app_result_e browser_disk_initial(void) __FAR__;

extern void music_close_engine(void)__FAR__;

#endif
