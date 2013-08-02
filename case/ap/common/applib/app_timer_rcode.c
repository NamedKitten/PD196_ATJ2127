/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-28 21:37     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _app_timer_rcode.c
 * \brief    Ӧ�ü���ʱ������ģ��ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ��һ�׻�����Ӧ�ü���ʱ������ӿڣ������������޸ģ�ɾ����handle����ȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel��libc���ȵȡ�
 * \version  1.0
 * \date     2011-9-28
*******************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    Ӧ�ü���ʱ�� handle ������Ӧ����Ϣѭ����ѭ�����ã�ѭ��ɨ�����ж�ʱ����ִ��
 *    ��ʱ����Ķ�ʱ���ķ������� proc�������ݶ�ʱ�����ͽ������á�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      app_timer
 * \note 
*******************************************************************************/
void handle_timers(void)
{
    app_timer_t *tm;
    uint32 cur_time_ms;
    int8 i;

    for(i = 0; i < g_app_timer_count; i++) 
    {
        tm = &g_app_timer_vector[i];
        if(((tm->attrb==APP_TIMER_ATTRB_CONTROL)||(g_app_info_state.backlight_state!=BACKLIGHT_STATE_OFF))//����У��
        && ((tm->tag == g_this_app_timer_tag) || (tm->tag == APP_TIMER_TAG_SYS)))//����У��
        {
            cur_time_ms = sys_get_ab_timer();
            if((cur_time_ms >= tm->timeout_expires)//��ʱʱ���ѵ�
            && (tm->state == TIMER_STATE_RUNNING)//��ʱ���������в���Ҫִ��handle
            && (tm->func_proc != NULL))//��ʱ��handle��Ч
            {
                //������ʱ����ʱʱ��
                if(tm->timeout > (cur_time_ms - tm->timeout_expires))
                {
                    tm->timeout_expires += tm->timeout;//ֱ����expires�ϼӶ�ʱ���ڣ���ȷ����ζ�ʱʱ��׼ȷ
                }
                else
                {
                    tm->timeout_expires = sys_get_ab_timer() + tm->timeout;
                }
                
                //������ʱ��һ��ִ����� kill ��
                if(tm->type == TIMER_TYPE_SINGLE_SHOT)
                {
                    tm->state = TIMER_STATE_NOUSED;
                }
                
                //������ʱ����������
                tm->func_proc();
            }
    
            if( cur_time_ms <= (tm->timeout_expires - tm->timeout ))
            {
                //���ϵͳʱ����ĳ��ԭ�����ڳ�ʱʱ�����棬��ô�����޸���ʱ���
                tm->timeout_expires = sys_get_ab_timer() + tm->timeout;
            }
        }
    }
}

/*! \endcond */
