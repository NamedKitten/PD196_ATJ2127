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
 * \file     manager_msg_loop.c
 * \brief    ��ȡ��Ϣ����
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "manager.h"

extern void manager_msg_callback(private_msg_t *pri_msg) __FAR__;
extern void _get_app_name(char *namebuf, uint8 ap_id) __FAR__;

/******************************************************************************/
/*!
 * \par  Description:
 *  ѭ����ȡ��Ϣ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
app_result_e manager_message_loop(void)
{
    private_msg_t pri_msg;
    uint8 ap_ret;
    int32 waitpid_ret;
    uint8 null_gui_app_count = 0;

    while (1)
    {
        if ((int) sys_mq_receive(MQ_ID_MNG, &pri_msg) == 0)
        {
            manager_msg_callback(&pri_msg);
        }
        
        if(g_app_info_vector[1].used == 0)//û��ǰ̨AP����
        {
            null_gui_app_count++;
            if(null_gui_app_count >= 60)//50ms * 60 = 3000ms = 3s
            {
                uint8 ap_name[12];

                while(1)                
                {                    
                    //����3����û��ǰ̨AP���ڣ����ܷ���������
                    //�������� mainmenu AP���в���  
                    waitpid_ret = libc_waitpid(&ap_ret, 0); 
                    if ((waitpid_ret >= 0) && (waitpid_ret != WAITPID_NO_PROCESS_EXIT))
                    {                        
                        break;                    
                    }                
                }                  
                
                //����3����û��ǰ̨AP���ڣ����ܷ����������������� mainmenu AP���в���
                _get_app_name(ap_name, APP_ID_MAINMENU);
                sys_free_ap(FALSE);
                sys_exece_ap(ap_name, 0, (int32)PARAM_NULL);
            }
        }
        else
        {
            null_gui_app_count = 0;
        }
        
        //����50ms�����������
        sys_os_time_dly(5);
    }
}
