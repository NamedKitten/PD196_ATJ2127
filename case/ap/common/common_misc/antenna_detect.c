/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-7-24 11:14     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _antenna_detect.c
 * \brief    common ���������ߣ���⼰����ӿ�ʵ��
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ���������ߣ���⼰����ӿ�ʵ�֡�
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-7-24
*******************************************************************************/

#include "common_ui.h"

extern bool fm_set_mute(uint8 mode);

/*! \cond */

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fm_set_mute(FM_MUTE_e mode)
 * \���þ�����������
 * \param[in]    mode    0:   �������  1: ����
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool fm_set_mute(uint8 mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = mode;
    msg.content.data[1] = sys_comval->volume_current;

    msg.type = MSG_FMENGINE_SETMUTE;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���������ߣ�״̬���Ĵ���
 * \param[in]    in_out TRUE ��ʾ��⵽���룬FALSE ��ʾ��⵽�γ�
 * \param[out]   none
 * \return       none
 * \note 
*******************************************************************************/
void deal_antenna_detect(bool in_out)
{
    //���Ķ��������ߣ�״̬
    g_app_info_state.antenna_state = in_out;
    
    if(get_engine_type() == ENGINE_RADIO)
    {
        if(g_this_app_info->app_id == APP_ID_RECORD)//FM ¼���У�ֱ�Ӳ��� PA ����
        {
            if(in_out == FALSE)
            {
                //����
                set_pa_volume(0);
            }
            else
            {
                //�������������Ϊ��ǰ����
                set_pa_volume(sys_comval->volume_current);
            }
        }
        else
        {
            if(in_out == FALSE)
            {
                //�����γ�, ����
                fm_set_mute(1);  
            }
            else
            {
                //��������, �������
                fm_set_mute(0);
            }
        }
    }  
}

/*! \endcond */
