/*******************************************************************************
 *                              US212A
 *                            Module: Manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_msg_handle2.c
 * \brief    ��Ϣ����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "manager.h"
/******************************************************************************/
/*!
 * \par  Description:
 *    ��Ϣ�������ڿռ����ƽ�����2������
 * \param[in]    ˽����Ϣָ��
 * \param[out]   none
 * \return       none
 * \retval       
 * \retval       
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void manager_msg_callback_sub(private_msg_t *pri_msg)
{
    msg_apps_t async_msg;
    int32 waitpid_ret;
    uint8 ap_ret;
    engine_type_e engine_type;
    engine_state_e last_engine_state;
    uint8 last_ap_id = 0xff;
    uint8 temp_ap_id;
    comval_t temp_setting_comval;

    switch (pri_msg->msg.type)
    {
        case MSG_POWER_OFF:
        case MSG_USB_TRANS:
        case MSG_RECLAIM://nandflash reclaim
        //��ɱ��ǰ̨
        async_msg.type = MSG_APP_QUIT;
        //ֻ�㲥��ǰ̨Ӧ��
        if(broadcast_msg(&async_msg) == TRUE)
        {
            while (1)
            {
                waitpid_ret = libc_waitpid(&ap_ret, 0);
                if ((waitpid_ret >= 0) && (waitpid_ret != WAITPID_NO_PROCESS_EXIT))
                {
                    break;
                }
            }
            //�ͷ�ǰ̨ap
            sys_free_ap(FALSE);
        }
        //��ɱ����̨����Ҫ����̨�������ͺ�״̬����configӦ��
        last_engine_state = g_app_info_state.state;//get_engine_state();
        if (last_engine_state != ENGINE_STATE_NULL)
        {
            engine_type = g_app_info_state.type;//get_engine_type();
            if (engine_type == ENGINE_RADIO)
            {
                last_ap_id = APP_ID_RADIO;
                temp_ap_id = APP_ID_FMENGINE;
//#ifndef PC
//                //fm¼��ʱ�ػ���fm�����ռ䱻¼��codec���ǣ�����ͳһ���°�װһ��fm����
//                sys_drv_uninstall(DRV_GROUP_FM);
//                sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
//#endif
            }
            else
            {
                last_ap_id = APP_ID_MUSIC;
                temp_ap_id = APP_ID_MENGINE;
            }
            //async_msg.type = MSG_APP_QUIT;
            send_async_msg(temp_ap_id, &async_msg);
            while (1)
            {
                waitpid_ret = libc_waitpid(&ap_ret, 0);
                if ((waitpid_ret >= 0) && (waitpid_ret != WAITPID_NO_PROCESS_EXIT))
                {
                    break;
                }
            }
        }
        else
        {           
            if((g_app_info_state.video_state == 1)
                && (g_app_last_state.last_app == APP_ID_VIDEO))//get_last_app()
            {
                last_ap_id = APP_ID_VIDEO; 
                last_engine_state = ENGINE_STATE_PLAYING;
            }
        }
        //�����usb������Ϣ,����usb ap
        if (pri_msg->msg.type == MSG_USB_TRANS)
        {
            sys_vm_read(&temp_setting_comval, VM_AP_SETTING, sizeof(temp_setting_comval));
            if (temp_setting_comval.online_device == 0)
            {
                sys_exece_ap("udisk.ap", 0, last_ap_id | (last_engine_state << 16));
            }
            else
            {
                sys_exece_ap("mtp.ap", 0, last_ap_id | (last_engine_state << 16));
            }
        }
        else if(pri_msg->msg.type == MSG_RECLAIM)//nandflash reclaim
        {
            sys_exece_ap("config.ap", 0, 2 | ((uint32)last_ap_id << 8) | ((uint32)last_engine_state << 16));
        }
        else//power off
        {
            sys_exece_ap("config.ap", 0, 1 | ((uint32)last_ap_id << 8) | ((uint32)last_engine_state << 16));
        }
        break;

        default:
        break;
    }

}
