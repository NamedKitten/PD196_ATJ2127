/*
 *******************************************************************************
 *                              USDK
 *                             Module: TOOLS AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ap_cfg_menu_tools.c
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ap_tools.h"

#define ENTRY_TOTAL 2
#define ITEM_TOTAL 34

/* define variable, �������� */
/*������ڲ˵�*/
const conf_item_head_t entrymenu =
{ "ENTRY MENU  ", ENTRY_TOTAL };

const conf_menu_entry_t entry[ENTRY_TOTAL] =
{
    { MENU_ENTRY_ID_MAIN, S_TOOLS, 0 },
    { MENU_ENTRY_ID_MAIN_NOCARD, S_TOOLS, 0 }
};
/*����Ҷ�Ӳ˵���*/
const conf_item_head_t item_head =
{ "MENU ITEM   ", ITEM_TOTAL };

const conf_menu_item_t item[ITEM_TOTAL] =
{
    { S_CALENDER, 0, &calendar_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 1, 0 }, /*����*/
    { S_STOPWATCH, 0, &stopwatch_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 2, 0 }, /*���*/
    { S_ALARM, 0, &alarm_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 3, 0 }, /*����*/

    //{ S_ALARM_1, NULL, NULL, NULL, NULL, NORMAL_MENU_ITEM,4,0}, /*����1*/
    /*�����˵� - ����-����1 - ���ӿ���*/
    { S_ALARM1_ONOFF, 0, &set_alarm1_onoff_callback, NULL, NULL, NORMAL_MENU_ITEM, 4, 0 },
    /*�ļ��˵� - ����-����1 - ���ӹ�*/
    { S_ALARM1_OFF, 0, &set_alarm1_off_callback, NULL, NULL, RAIDO_MENU_ITEM, 5, 0 },
    /*�ļ��˵� - ����-����1 - ���ӿ�*/
    { S_ALARM1_ON, 0, &set_alarm1_on_callback, NULL, NULL, RAIDO_MENU_ITEM, 6, 0 },
    /*�����˵� - ����-����1 - ����ʱ��*/
    { S_ALARM1_TIME, 0, &set_alarm1_time_callback, NULL, NULL, NORMAL_MENU_ITEM, 7, 0 },
    /*�����˵� - ����-����1 - ��������*/
    { S_ALARM1_CYC, 0, &set_alarm1_cyc_callback, NULL, NULL, NORMAL_MENU_ITEM, 8, 0 },
    /*�ļ��˵� - ����-����1 - �������� - ����*/
    { S_ALARM1_CYC_ONCE, 0, &alarm1_once_callback, NULL, NULL, RAIDO_MENU_ITEM, 9, 0 },
    /*�ļ��˵� - ����-����1 - �������� - ÿ��*/
    { S_ALARM1_CYC_DAYS, 0, &alarm1_days_callback, NULL, NULL, RAIDO_MENU_ITEM, 10, 0 },
    /*�ļ��˵� - ����-����1 - �������� - ������*/
    { S_ALARM1_CYC_WORK, 0, &alarm1_work_callback, NULL, NULL, RAIDO_MENU_ITEM, 11, 0 },
    /*�����˵� - ����-����1 - ��������*/
    { S_ALARM1_MUSIC, 0, &set_alarm1_ring_callback, NULL, NULL, NORMAL_MENU_ITEM, 12, 0 },
    /*�ļ��˵� - ����-����1 - �������� - ��������*/
    { S_RING1_SD, 0, &alarm1_ring_sd_callback, NULL, NULL, RAIDO_MENU_ITEM, 13, 0 },
    /*�ļ��˵� - ����-����1 - �������� - ��������*/
    { S_RING1_DISK, 0, &alarm1_ring_flash_callback, NULL, NULL, NORMAL_MENU_ITEM, 14, 0 },
    /*�ļ��˵� - ����-����1 - �������� - ��������*/
    { S_RING1_CARD, 0, &alarm1_ring_card_callback, NULL, NULL, NORMAL_MENU_ITEM, 15, 0 },
    /*�����˵� - ����-����1 - ��������*/
    { S_ALARM1_VOL, 0, &alarm1_volume_callback, NULL, NULL, NORMAL_MENU_ITEM, 16, 0 },
    /*�����˵� - ����-����1 - �˳�*/
    /*{ S_ALARM1_EXIT, 0, &alarm1_exit_callback, NULL, NULL, NORMAL_MENU_ITEM, 17, 0 },*/

    ///*�����˵� - ����-��ʱ¼��*/
    //    { S_ALARM_REC, NULL, NULL, NULL, NULL, NORMAL_MENU_ITEM,18,0}, /*��ʱ¼��*/
    ///*�����˵� - ����-��ʱ¼�� - ����*/
    //    { S_AUREC_ONOFF, NULL, &set_rec_onoff_callback, NULL, NULL, NORMAL_MENU_ITEM,19,0},
    ///*�ļ��˵� - ����-��ʱ¼�� - ��*/
    //    { S_AUREC_OFF, NULL, &set_rec_off_callback, NULL, NULL, RAIDO_MENU_ITEM,20,0},
    ///*�ļ��˵� - ����-��ʱ¼�� - ��*/
    //    { S_AUREC_ON, NULL, &set_rec_on_callback, NULL, NULL, RAIDO_MENU_ITEM,21,0},
    ///*�����˵� - ����-��ʱ¼�� - ¼����ʼʱ��*/
    //    { S_AUREC_ON_TIME, NULL, &set_RECtime_ON_callback, NULL, NULL, NORMAL_MENU_ITEM,22,0},
    ///*�����˵� - ����-��ʱ¼�� - ¼������ʱ��*/
    //    { S_AUREC_OFF_TIME, NULL, &set_RECtime_OFF_callback, NULL, NULL, NORMAL_MENU_ITEM,23,0},
    ///*�����˵� - ����-��ʱ¼�� - ��������*/
    //    { S_AUREC_CYC, NULL, &set_rec_cyc_callback, NULL, NULL, NORMAL_MENU_ITEM,24,0},
    ///*�ļ��˵� - ����-��ʱ¼�� - �������� - ����*/
    //    { S_AUREC_CYC_ONCE, NULL, &set_rec_once_callback, NULL, NULL, RAIDO_MENU_ITEM,25,0},
    ///*�ļ��˵� - ����-��ʱ¼�� - �������� - ÿ��*/
    //    { S_AUREC_CYC_DAYS, NULL, &set_rec_days_callback, NULL, NULL, RAIDO_MENU_ITEM,26,0},
    ///*�ļ��˵� - ����-��ʱ¼�� - �������� - ������*/
    //    { S_AUREC_CYC_WORK, NULL, &set_rec_work_callback, NULL, NULL, RAIDO_MENU_ITEM,27,0},

    /*�����˵� - ����-��ʱFM*/
    { S_ALARM_FM, 0, &fmrec_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 17, 0 }, /*��ʱFM*/
    /*�����˵� - ����-��ʱFM - ����*/
    { S_AUFM_ONOFF, 0, &set_fm_onoff_callback, NULL, NULL, NORMAL_MENU_ITEM, 18, 0 },
    /*�ļ��˵� - ����-��ʱFM - ��*/
    { S_AUFM_OFF, 0, &set_fm_off_callback, NULL, NULL, RAIDO_MENU_ITEM, 19, 0 },
    /*�ļ��˵� - ����-��ʱFM - ��*/
    { S_AUFM_ON, 0, &set_fm_on_callback, NULL, NULL, RAIDO_MENU_ITEM, 20, 0 },
    /*�����˵� - ����-��ʱFM - ʱ��*/
    //    { S_AUFM_TIME, NULL, &set_FMtime_callback, NULL, NULL, NORMAL_MENU_ITEM,32,0},
    /*�����˵� - ����-��ʱ¼�� - ��ʼʱ��*/
    { S_START_TIME, 0, &set_fmtime_on_callback, NULL, NULL, NORMAL_MENU_ITEM, 21, 0 },
    /*�����˵� - ����-��ʱ¼�� - ����ʱ��*/
    { S_END_TIME, 0, &set_fmtime_off_callback, NULL, NULL, NORMAL_MENU_ITEM, 22, 0 },

    /*�����˵� - ����-��ʱFM - ����*/
    { S_AUFM_CYC, 0, &set_fm_cyc_callback, NULL, NULL, NORMAL_MENU_ITEM, 23, 0 },
    /*�ļ��˵� - ����-��ʱFM - ���� - ����*/
    { S_AUFM_CYC_ONCE, 0, &set_fm_once_callback, NULL, NULL, RAIDO_MENU_ITEM, 24, 0 },
    /*�ļ��˵� - ����-��ʱFM - ���� - ÿ��*/
    { S_AUFM_CYC_DAYS, 0, &set_fm_days_callback, NULL, NULL, RAIDO_MENU_ITEM, 25, 0 },
    /*�ļ��˵� - ����-��ʱFM - ���� - ������*/
    { S_AUFM_CYC_WORK, 0, &set_fm_work_callback, NULL, NULL, RAIDO_MENU_ITEM, 26, 0 },
    /*�����˵� - ����-��ʱFM - FMƵ��*/
    { S_AUFM_FQ, 0, &set_fm_fq_callback, NULL, NULL, NORMAL_MENU_ITEM, 27, 0 },
    /*�����˵� - ����-��ʱFM - ����*/
    { S_AUFM_VOL, 0, &fm_volume_callback, NULL, NULL, NORMAL_MENU_ITEM, 28, 0 },
    /*�����˵� - ����-��ʱFM - ¼������*/
    { S_RECORD_ONOFF, 0, &fmrec_onoff_callback, NULL, NULL, NORMAL_MENU_ITEM, 29, 0 },
    /*�ļ��˵� - ����-��ʱ¼�� - ��*/
    { S_RECORD_OFF, 0, &fmrec_off_callback, NULL, NULL, RAIDO_MENU_ITEM, 30, 0 },
    /*�ļ��˵� - ����-��ʱ¼�� - ��*/
    { S_RECORD_ON, 0, &fmrec_on_callback, NULL, NULL, RAIDO_MENU_ITEM, 31, 0 },
    /*�����˵� - ����-��ʱ¼�� - �˳�*/
    /*{ S_ALARM2_EXIT, 0, &fmrec_exit_callback, NULL, NULL, NORMAL_MENU_ITEM, 33, 0 },*/

    /*һ���˵���-���ݽ���*/
    { S_DATAEXCH, 0, &dataexch_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 32, 0 }, /*���ݽ���*/
    /*һ���˵���-�̼�����*/
    { S_FWUPDATA, 0, &fwupdata_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 33, 0 }, /*�̼�����*/
    /*һ���˵���-�Զ�����*/
    { S_AUTOUPGRADE, 0, autoupgrade_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 34, 0 } /*�Զ�����*/    
};



#ifdef PC
menu_cb_func _get_menu_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;
   
    item_cnt = item_head.total;
    
    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].menu_func;
        }
    }
    
    return NULL;
}
 
menu_cb_leaf _get_callback_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;
    
    item_cnt = item_head.total;
    
    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].callback;
        }
    }
    
    return NULL;
}

menu_cb_option _get_menu_option(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;
    
    item_cnt = item_head.total;
    
    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].menu_option;
        }
    }

    return NULL;
}
#endif

