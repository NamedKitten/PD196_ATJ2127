#ifndef _KEY_H			//��ֹ�ض���
#define _KEY_H
#include <typeext.h>

/*3.�԰������������ӿ���ʽ*/

extern uint8 key_inner_chargeget(void* null0, void* null1, void* null2);
extern uint8 key_inner_chargeset(uint8 setting, uint8 current, uint8 param3);
extern uint8 key_inner_beep(uint8 value, void* null0, void* null1);
extern uint8 *key_inner_getkeytabaddress(void* null0, void* null1, void* null2);
extern uint8 key_inner_holdcheck(void* null0, void* null1, void* null2);

#endif
