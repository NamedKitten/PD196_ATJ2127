/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-4-13 16:47     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _alarm_msg_dispatch.c
 * \brief    
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               �ּ���alarm�ж���Ϣ
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-4-13
*******************************************************************************/

#include "common_ui.h"

app_result_e ap_fmautorecord_init(alarm_fm_t *aufm_temp);
app_result_e ap_alarm_init(alarm_alarm_t *alarm_temp);
uint8 CompareAndSet_timer_flag(alarm_fm_t *aufm_temp, alarm_alarm_t *g_alarm);

extern uint8 __get_weekofday(uint16 y, uint8 m, uint8 d);
extern uint8 compare_curtime(time_t *Cur_T, time_t *timer1, time_t *timer2, /*time_t *timer3,*/ uint8 num);

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    alarm��Ϣ��֮��Ĵ�������cycle������alarm on/off
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e
 * \retval           RESULT_AUTORECORD FM�Զ�¼�� alarm ��Ϣ
 * \retval           RESULT_ALARM ���� alarm ��Ϣ
 * \retval           RESULT_NULL ���봦�� alarm ��Ϣ
 * \ingroup      misc_func
 * \note
*******************************************************************************/
app_result_e com_alarm_msg_dispatch(void)
{
    alarm_vars_t  temp_alarm_val;
    uint8 flag;
    int result = 0;
    
    //sys_vm_read(&g_alarm_temp, VM_ALARM, sizeof(alarm_alarm_t));
    //sys_vm_read(&g_aufm_temp, VM_AP_DSC, sizeof(g_aufm_temp));
    sys_vm_read(&temp_alarm_val, VM_ALARM, sizeof(alarm_vars_t));
    /*���û���������Ӻ��Զ�FM¼�����ܣ�����*/
    if(temp_alarm_val.magic != MAGIC_ALARM)
    {
        return RESULT_NULL;
    }
    libc_memcpy(&g_alarm_temp, &temp_alarm_val.g_alarm, sizeof(alarm_alarm_t));
    libc_memcpy(&g_aufm_temp, &temp_alarm_val.g_aufm, sizeof(alarm_fm_t));
    
    if((g_aufm_temp.enable == 0) && (g_alarm_temp.enable == 0))
    {
        return RESULT_NULL;
    }

    if (temp_alarm_val.timer_flag == 0)
    {
        result = ap_fmautorecord_init(&g_aufm_temp);
        if (result == RESULT_ALARMFM)
        {
            if (temp_alarm_val.g_aufm.fmrec_enable == 1)
            {
                result = RESULT_FMRECORD;
            }
            //snooze���屻©�������
            if(g_alarm_temp.alarm_count != 0)//Snooze��־
            {
                time_t time_tmp;
                sys_get_time(&time_tmp);  //��ȡϵͳʱ��
                if((time_tmp.hour*60 + time_tmp.minute + 6) > 
                    (temp_alarm_val.snooze_time.hour*60 + temp_alarm_val.snooze_time.minute) )
                {
                    //��snoozeʱ������
                    temp_alarm_val.timer_flag = 1;
                    sys_set_alarm_time(&temp_alarm_val.snooze_time);
                }
            }
        }
    }
    else if (temp_alarm_val.timer_flag == 1)
    {
        result = ap_alarm_init(&g_alarm_temp);
    }
    else
    {
        return RESULT_NULL;
    }
    //����NEXT����
    flag = (uint8)CompareAndSet_timer_flag(&g_aufm_temp, &g_alarm_temp);
    if (flag == 1)
    {
        temp_alarm_val.timer_flag = 0;
        sys_set_alarm_time(&g_aufm_temp.start_time);
    }
    else if (flag == 2)
    {
        temp_alarm_val.timer_flag = 1;
        sys_set_alarm_time(&g_alarm_temp.start_time);
    }
    else
    {
        return RESULT_NULL;
    }
    //��VM����д��
    sys_vm_write(&temp_alarm_val, VM_ALARM);

    return result;
}

/*! \endcond */

/*! \cond */

/******************************************************************************/
/*!
 * \par  Description:
 *    ����cycle��־�ж��Ƿ񷢳�RESULT_ALARMFM��Ϣ
 * \param[in]    alarm_fm_t *aufm_temp
 * \param[out]   none
 * \return       app_result_e
 * \retval           RESULT_ALARMFM �Զ�FM��Ϣ
 * \retval           RESULT_NULL ����Ϣ
 * \ingroup      misc_func
 * \note
*******************************************************************************/
app_result_e ap_fmautorecord_init(alarm_fm_t *aufm_temp)
{
    uint8 startCol = 0;
    date_t t_data;
    //snooze���屻�󱨵����
    if(g_alarm_temp.alarm_count != 0)//Snooze��־
    {
        time_t time_tmp;
        sys_get_time(&time_tmp);  //��ȡ��ǰϵͳʱ��
        if(g_aufm_temp.start_time.minute != time_tmp.minute)
        {
            return RESULT_ALARM;
        }
    }
    
    sys_get_date(&t_data);
    if (aufm_temp->cycle_flag == 0) //ѭ��--����
    {
        if ((aufm_temp->cycle.one_time.year == t_data.year) &&
            (aufm_temp->cycle.one_time.month == t_data.month) &&
            (aufm_temp->cycle.one_time.day == t_data.day))
        {
            return RESULT_ALARMFM;
        }
    }
    else if (aufm_temp->cycle_flag == 1) //ѭ��--ÿ��
    {
        return RESULT_ALARMFM;
    }
    else if (aufm_temp->cycle_flag == 2) //ѭ��--������
    {
        startCol = __get_weekofday(t_data.year, t_data.month, t_data.day);
        if ((startCol > 0) && (startCol < 6)) //������
        {
            return RESULT_ALARMFM;
        }
    }
    else
    {
        //do nothing for QAC
    }
    
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ����cycle��־�ж��Ƿ񷢳�RESULT_ALARM��Ϣ
 * \param[in]    alarm_fm_t *aufm_temp
 * \param[out]   none
 * \return       app_result_e
 * \retval           RESULT_ALARM alarm��Ϣ
 * \retval           RESULT_NULL ����Ϣ
 * \ingroup      misc_func
 * \note
*******************************************************************************/
app_result_e ap_alarm_init(alarm_alarm_t *alarm_temp)
{
    uint8 startCol = 0;
    date_t t_data;
    sys_get_date(&t_data);
    if (alarm_temp->cycle_flag == 0) //ѭ��--����
    {
        if ((alarm_temp->cycle.one_time.year == t_data.year) &&
            (alarm_temp->cycle.one_time.month == t_data.month) &&
            (alarm_temp->cycle.one_time.day == t_data.day))
        {
            return RESULT_ALARM;
        }
    }
    else if (alarm_temp->cycle_flag == 1) //ѭ��--ÿ��
    {
        return RESULT_ALARM;
    }
    else if (alarm_temp->cycle_flag == 2) //ѭ��--������
    {
        startCol = __get_weekofday(t_data.year, t_data.month, t_data.day);
        if ((startCol > 0) && (startCol < 6)) //������
        {
            return RESULT_ALARM;
        }
    }
    else
    {
        //do nothing for QAC
    }
    
    return RESULT_NULL;
}


/******************************************************************************/
/*!
 * \par  Description:
 *    ����enable_flag��������alarm��ʱ���Ⱥ�,����
 * \param[in]    1 -- aufm
 * \param[in]    2 -- alarm
 * \param[out]   none
 * \return       app_result_e
 * \retval           1 -- aufm
 * \retval           2 -- alarm
 * \retval           0 -- error
 * \retval           4 -- ʱ����ͬ
 * \ingroup      misc_func
 * \note
*******************************************************************************/
uint8 CompareAndSet_timer_flag(alarm_fm_t *aufm_temp, alarm_alarm_t *g_alarm)
{
    uint8 result;
    time_t time_1;
    sys_get_time(&time_1);  //��ȡ��ǰϵͳʱ��
    time_1.second ++;  //��Ϊ�򻯱Ƚ�

    if((aufm_temp->enable == 1) && (g_alarm->enable == 1))
    {
        result = compare_curtime(&time_1, &aufm_temp->start_time, &g_alarm->start_time, /*NULL,*/ 3);
        return result;
    }
    if (aufm_temp->enable == 1)
    {
        return 1;
    }
    if (g_alarm->enable == 1)
    {
        return 2;
    }
    return 0;
}


/******************************************************************************/
/*!
 * \par  Description:
 *    ͨ���Ƚϼ��������ʱ���Ⱥ󡢼����ȼ�������alarmʱ�䡣
 * \param[in]    alarm_fm_t *aufm_temp
 * \param[in]    alarm_alarm_t *alarm_temp
 * \param[in]    uint8 *timer_flag
 * \param[out]   none
 * \return       uint8 *timer_flag
 * \retval           0 -- aufm
 * \retval           1 -- alarm
 * \retval           3 -- error
 * \ingroup      misc_func
 * \note         ����alarmʱ��ʱ����
*******************************************************************************/
void set_timer_msg(alarm_fm_t *aufm_temp, alarm_alarm_t *alarm_temp, uint8 *timer_flag)
{
    uint8 flag;
    flag = (uint8)CompareAndSet_timer_flag(aufm_temp, alarm_temp);
    switch(flag)
    {
    case 1:
        if(aufm_temp->enable == 1)
        {
            sys_set_alarm_time(&aufm_temp->start_time);
            *timer_flag = 0;
        }
        break;
        
    case 2:
        if(alarm_temp->enable == 1)
        {
            sys_set_alarm_time(&alarm_temp->start_time);
            *timer_flag = 1;
        }
        break;
        
    case 4:  //��ͬʱ�����ȼ�
        if((aufm_temp->enable == 1) && (alarm_temp->enable == 1))
        {
            sys_set_alarm_time(&aufm_temp->start_time);
            *timer_flag = 0;
        }
        break;
        
    case 0:
    default:
        *timer_flag = 3;
        break;
    }//end of switch
}

/*! \endcond */
