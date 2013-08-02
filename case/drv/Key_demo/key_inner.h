/*
 ********************************************************************************
 *                       ACTOS
 *                  key board message define
 *
 *                (c) Copyright 2002-2003, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : key.msa
 * By     : Gongee
 * Version: 1> v1.00     first version     2003-11-20 9:14
 ********************************************************************************
 ********************************************************************************
 */
#ifndef _KEY_INNER_H			//��ֹ�ض���
#define _KEY_INNER_H

#include "psp_includes.h"
#include "key_interface.h"
#include "gui_msg.h"
#include "app_msg.h"

extern charge_state_e key_inner_chargeget(void* null0, void* null1, void* null2);
extern uint8 key_inner_chargeset(charge_control_e setting, charge_current_e current, battery_full_t param3);
extern uint8 key_inner_beep(uint8 value, void* null0, void* null1);
extern uint8 *key_inner_getkeytabaddress(void* null0, void* null1, void* null2);
extern uint8 key_inner_holdcheck(void* null0, void* null1, void* null2);
extern uint8 *key_inner_speakcheck(uint8 onoff, void* null1, void* null2);

extern void PutSysMsg(unsigned int key);
extern void key_scan(void);
extern void chargedeal(void);
extern void stop_charge_exit(void)__FAR__;
extern uint8 BatteryFullCheck(void);

//#define lock_key

/*define key message of key driver*/

#define Msg_KeyShortUp        0x30    //�̰���̧��
#define Msg_KeyLongUp         0x31    //������̧��
//#define Msg_KeyStandby          0x21
#define Msg_KeyHold           0x32        //�����Msg_KeyLoop|AP_KEY_UP��ͻ��Mars��2008-04-16
#define Msg_KeyUnHold         0x34

//Hold min and max adc
#define MAX_HOLD_ADC 0x39
#define MIN_HOLD_ADC 0x33
/* Hold state */
typedef enum
{
    HOLD_NONE, HOLD_LOCK
} hold_t;

//����������
extern uint8 ChargeCurrent; //������ѡ��
extern uint8 TrickleTimeCounter; //������������30�����
extern uint8 BatteryFullFlag; //������ʶ
extern uint8 LowestCurrentCnt;
extern uint8 CurrentSetFlag;
extern uint8 BatteryRefVol; //����ѹֵ
extern unsigned int g_rtc_counter;
extern unsigned char key_map[16];
extern uint8 TheCharge; //����ʶ��0,stop��1��start
extern uint8 BatteryFullFlag; //������ʶ
extern uint8 TrickleTimeCounter;
extern int time_irq;
extern unsigned char g_speakonoff;
#endif/*_KEY_H*/
