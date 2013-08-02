/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-5-10 9:28     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _sys_timer.c
 * \brief    common ϵͳ��ʱ����ʼ�����˳��ӿ�ʵ��
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-5-10
*******************************************************************************/

#include "common_ui.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ����ϵͳ��ʱ��ʱ���� headbar ���ڸ�����ʱ������ǰ̨Ӧ�ý���Ӧ��ʱ���á�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  ���� config.app��ǰ̨Ӧ���ڽ���ʱ������øýӿڣ����򱳹⣬������ʡ��ػ���
 *      ��ʱ�ػ���״̬�����£��ȵȹ��ܽ��޷�ʹ�á�
*******************************************************************************/
void sys_timer_init(void)
{
    uint8 tag_backup;
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIMER_TAG_SYS);
    sys_counter_timer_id = set_app_timer(APP_TIMER_ATTRB_CONTROL, 500, sys_counter_handle);
    change_app_timer_tag(tag_backup);
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIMER_TAG_SYS);
    sys_status_timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 500, sys_status_handle);
    change_app_timer_tag(tag_backup);
    
    stop_app_timer(sys_status_timer_id);//Ĭ�ϲ���ʾ status��ͨ�� gui_set_headbar_mode ����
    g_sys_counter.battery_counter = 100;//ǿ�ƶ�ȡ���״̬�͵�ص���
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ɾ��ϵͳ��ʱ��ʱ���� headbar ���ڸ�����ʱ������ǰ̨Ӧ���˳�Ӧ��ʱ���á�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  �˳�Ӧ��ʱ����Ҫ���� sys_counter_exit����ɾ�� sys_counter_init ��������ʱ����
*******************************************************************************/
void sys_timer_exit(void)
{
    kill_app_timer(sys_counter_timer_id);
    kill_app_timer(sys_status_timer_id);
}

/*! \endcond */
