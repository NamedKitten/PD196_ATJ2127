/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 21:49     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _app_sleep.c
 * \brief    common Ӧ��˯��
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               Ӧ��˯�߽ӿ�ʵ��
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2011-9-19
*******************************************************************************/

#include "common_ui.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    Ӧ�ò����ߣ����Խ��� gui ��Ϣ��
 * \param[in]    sleep_timer ˯��ʱ�䣬��0.5��Ϊ��λ
 * \param[in]    key_map_list ��ֹ˯�ߵİ���ӳ���б�ΪNULL��ʾ���ⰴ��������ֹ˯��
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_NULL ˯��ʱ�䵽ʱ����
 * \retval           gui_event ϵͳ��Ϣ���ش�������Ƕ��
 * \ingroup      misc_func
 * \par          exmaple code
 * \code
    ����1���ȴ�2���ӣ������ⰴ���˳�
    app_result_e result;
    
    result = com_app_sleep(2, NULL);
    if(result != RESULT_NULL)
    {
        �򰴼������˳�
    }
    
    ����2���ȴ�2���ӣ����� key_map_list �еİ����¼�
    app_result_e result;
    
    result = com_app_sleep(2, key_map_list);
    if(result != RESULT_NULL)
    {
        �򰴼��ж��˳���result ��ʾ�ð�����Ӧ���¼���Ӧ���зַ�����
    }
    
 * \endcode
 * \note 
 * \li  ��˯����һ�㲻�԰�����Ϣ��������һ�� key_map_list ΪNULL��
 * \li  ����û�ϣ������ͨ��ĳ��������Ϣ��ֹ˯�ߣ���ô�ͽ��ð�����Ϣ��ӵ� key_map_list 
 *      �����ݽ��뼴�ɡ�
 * \li  ����û�������յ� gui ����¼�ʱ�˳�˯�ߣ����԰Ѹÿ�ݰ�����ӵ� key_map_list �У�
 *      ��ӳ���������� MSG_NULL��
*******************************************************************************/
app_result_e com_app_sleep(uint32 sleep_timer, const key_map_t *key_map_list)
{
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    uint32 sleep_500ms_timer;
    
    //���� 10ms ��ʱ��
    sleep_500ms_timer = sleep_timer;
    if(sleep_500ms_timer == 0)
    {
        return RESULT_NULL;
    }
    while (1)
    {
        //��ȡgui��Ϣ
        while(get_gui_msg(&input_msg) == TRUE)//��gui��Ϣ
        {
            if(key_map_list != NULL)//�а���ӳ����Ͱ���ӳ������ӳ�䴦��
            {
                //���а���ӳ��
                if(com_key_mapping(&input_msg, &gui_event, key_map_list) == TRUE)
                {
                    return gui_event;
                }
            }
            else//�����ⰴ������
            {
                //�������˺�������
                com_filter_key_hold();
                return RESULT_NULL;
            }
        }
               
        //����10ms�����������
        sys_os_time_dly(50);
        
        //˯��ʱ���ʱ���ݼ�
        sleep_500ms_timer--;
        //˯��ʱ���ѵ����˳�
        if(sleep_500ms_timer == 0)
        {
            return RESULT_NULL;
        }
    }
}

/*! \endcond */
