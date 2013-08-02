/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-27 22:18     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _message_rcode.c
 * \brief    ��Ϣͨ�Ź���ģ��ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ��һ�׻�����Ϣͨ�Ź���ӿڣ������첽���ͣ�ͬ�����ͣ��㲥��Ϣ����������Ϣ�ȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel��libc��applib ���̹���ӿڣ��ȵȡ�
 * \version  1.0
 * \date     2011-9-27
*******************************************************************************/

#include "applib.h"

extern void com_app_msg_hook(private_msg_t *private_msg) __FAR__;

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���� gui ��Ϣ������������Ϣ��
 * \param[in]    none
 * \param[out]   input_msg�����ؽ��յ��� gui ��Ϣ
 * \return       bool
 * \retval           TRUE ���� gui ��Ϣ�ɹ���
 * \retval           FALSE ���� gui ��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    ����1��gui ��Ϣ���¼���ѭ��
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    bool ret;
    
    ret = get_gui_msg(&input_msg);
    if(ret == TRUE)
    {
        if(com_key_mapping(&input_msg, &gui_event, key_map_list) == TRUE)
        {
            switch(gui_event)
            {
            case EVENT_DO_SOMETHING:
                break;
               
            default:
                result = com_message_box(gui_event);
                break;
            }
        }
    }
    else
    {
        gui ��Ϣ���¼���������ϣ����������Դ���Ӧ��˽����Ϣ��
    }
 * \endcode
 * \note 
*******************************************************************************/
bool get_gui_msg(input_gui_msg_t *input_msg)
{
    uint32 key_data;
    
    if(sys_mq_receive(MQ_ID_GUI, (void *)&key_data) == 0)
    {
        //��Ϣ��ѹ��
        input_msg->type = (input_msg_type_e)(uint8)key_data;//byte 0
        //if(input_msg->type == INPUT_MSG_KEY)
        //{
        input_msg->data.kmsg.val = (uint8)(key_data >> 8);//byte 1
        input_msg->data.kmsg.type = (key_type_e)(key_data >> 16);//byte 2-3
        //}
        //else if(input_msg->type == INPUT_MSG_TOUCH)
        //{
        //    input_msg->data.tmsg.x = (uint16)(key_data >> 8);//bit[8-17]
        //    input_msg->data.tmsg.y = (uint16)(key_data >> 18);//bit[18-27]
        //    input_msg->data.tmsg.type = (tp_type_e)(key_data >> 28);//bit[28-31]
        //}
        //else//INPUT_MSG_GSENSOR
        //{
        //    
        //}
        
        //�յ�gui��Ϣ����ʾ���û�����������һЩϵͳ�������磺
        //0.������������˵���������
        //1.����رռ�ʱ����Ļ������ʱ��ʡ��ػ���ʱ���������ڲ��ż�ʱ ��0
        //2.����������
        //3.����������
        return com_gui_msg_hook(input_msg);
    }
    
    return FALSE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ����ϵͳ��Ϣ�����㲥������Ӧ�á�
 * \param[in]    none
 * \param[out]   sys_msg�����ؽ��յ���ϵͳ��Ϣ
 * \return       bool
 * \retval           TRUE ����ϵͳ��Ϣ�ɹ���
 * \retval           FALSE ����ϵͳ��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \note 
 * \li  ��ȡϵͳ��Ϣ�󣬻�㲥������Ӧ�á�
 * \li  �ýӿ�ֻ��ǰ̨AP�� get_app_msg �е��ã�Ҳ����˵��ϵͳ��Ϣ�ӷ�����������
 *      ���ܻᱻ delay һЩʱ�䣬ֱ����ǰ̨AP������Ϣѭ����
*******************************************************************************/
bool get_sys_msg(sys_msg_t *sys_msg)
{
    msg_apps_t msg;
    
    //���type��0����Ϊϵͳ��Ϣ����Ϊ2���ֽڣ�sys_mq_receive �����Ḳ�ǵ�type��4�ֽڣ���2�ֽڣ��ᵼ��type����
    sys_msg->type = MSG_NULL;
    if(sys_mq_receive(MQ_ID_SYS, (void *)sys_msg) == 0)
    {
        msg.type = sys_msg->type;
        return broadcast_msg_sys(&msg);
    }
    
    //��Ϣ�����ѿգ�����false
    return FALSE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ����Ӧ��˽����Ϣ����Щ�����ڽ���ϵͳ��Ϣʱת���ģ���
 * \param[in]    none
 * \param[out]   private_msg�����ؽ��յ���Ӧ��˽����Ϣ
 * \return       bool
 * \retval           TRUE ����Ӧ��˽����Ϣ�ɹ���
 * \retval           FALSE ����Ӧ��˽����Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    ����1��Ӧ��˽����Ϣ����
    private_msg_t private_msg;
    app_result_e result;
    bool ret;
    
    ret = get_app_msg(&private_msg);
    if(ret == TRUE)
    {
        result = msg_callback(&private_msg);
        if(result == RESULT_REDRAW)
        {
            �����ػ�
        }
        else
        {
            ��������ֵ����
        }
    }
 * \endcode
 * \note 
*******************************************************************************/
bool get_app_msg(private_msg_t *private_msg)
{
    //��ʱ��ɨ��
    handle_timers();
    
#ifdef PC
    UpdateMainWnd();
#endif
    
    if(g_this_app_info->app_type == APP_TYPE_GUI)
    {
        //ǰ̨Ӧ��
        sys_msg_t sys_msg;
        
        //��ȡϵͳ��Ϣ���㲥
        get_sys_msg(&sys_msg);
    }
    //���� Ӧ��˽����Ϣ
    if(sys_mq_receive((uint32)(g_this_app_info->mq_id), (void *)private_msg) == 0)
    {
        com_app_msg_hook(private_msg);
        return TRUE;
    }
    
    //��Ϣ�����ѿգ�����false
    return FALSE;
}

/*! \endcond */
