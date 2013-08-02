/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-10-14 16:58     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_msgmap.c
 * \brief    ������������������g-sensor����Ϣӳ��ӿ�ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ�ְ�����������������g-sensor��ӳ��Ϳ�ݼ�ӳ�书�ܡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               ��
 * \version  1.0
 * \date     2011-10-14
*******************************************************************************/

#include "common_func.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ������Ϣӳ�䣬�� gui ��Ϣӳ��Ϊ�¼�����֧�ֲ�ͬ���������Ӧͳһҵ����
 * \param[in]    input_msg Ҫ����ӳ��� gui ��Ϣ
 * \param[in]    key_map_list ����ӳ��İ���ӳ���
 * \param[out]   gui_event �洢ӳ����¼�
 * \return       bool
 * \retval           TRUE ӳ��õ���Ч gui �¼���
 * \retval           FALSE û��ӳ��õ���Ч gui �¼���������Ϣ�����˵��ˡ�
 * \ingroup      common_func
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
 * \li  ����ӳ��ӿ��У���������ӳ������ӳ�䣬һ���ǽӿ���Ĭ�ϵĿ�ݼ�ӳ���
 *      һ�����û��Զ���İ���ӳ����������ȼ��Ƚϸߡ�
 * \li  ����û�������Ӧĳ����ݼ����������ĳ����ݼ��������ã���ô�������Զ���
 *      �İ���ӳ�������Ӹð�����ӳ���
*******************************************************************************/
bool com_key_mapping(input_gui_msg_t *input_msg, msg_apps_type_e *gui_event, const key_map_t *key_map_list)
{
    const key_map_t *this_key_map;
    
    //��������е�ʹ�����
    if(INPUT_MSG_KEY == input_msg->type)
    {
        if(key_map_list != NULL)
        {
            this_key_map = key_map_list;
            for(;;this_key_map++)
            {
                /* �ﵽ�û����һ��ӳ�䣬��������ӳ�� */
                if ( KEY_NULL == this_key_map->key_event.val )
                {
                    break;
                }
    
                /* �ҵ���Ҫӳ��İ���, ����ӳ�� */
                if( (input_msg->data.kmsg.val == this_key_map->key_event.val)
                  &&(input_msg->data.kmsg.type & this_key_map->key_event.type) )
                {
                    *gui_event = this_key_map->event;
                    if(*gui_event == MSG_NULL)
                    {
                        return FALSE;
                    }
                    else
                    {
                        return TRUE;
                    }
                }
            }
        }
        
        //��ݼ�ӳ��
        this_key_map = key_shortcut_list;
        for (;;this_key_map++)
        {
            /* �ﵽ�û����һ����ݼ�, ���ߴﵽ����ݼ�����, �˳�*/
            if ( 0 == this_key_map->key_event.val )
            {
                break;
            }

            /* �ҵ���ݼ�, ���Ϳ�ݼ���Ӧ��Ϣ */
            if ( (input_msg->data.kmsg.val == this_key_map->key_event.val) 
              && (input_msg->data.kmsg.type & this_key_map->key_event.type) )
            {
                *gui_event = this_key_map->event;
                return TRUE;
            }
        }
    }
    ////������
    //else if(INPUT_MSG_TOUCH == input_msg->type)
    //{}
    ////g-sensor
    //else
    //{}

    return FALSE;
}

/*! \endcond */
