#ifndef _FM_INTERFACE_H         
#define _FM_INTERFACE_H

#include "psp_includes.h"

typedef struct
{
    uint16 freq; //��ǰ��̨��Ƶ�ʡ���λΪ1k (���λ��Ϊ1)��ȡ���2 �ֽ�
    uint8 station; //��ǰ��̨��Ӧ�ĵ�̨����ţ�from 1 to station_count����ǰ��̨δ����ʱΪ0
    uint8 station_count; //��̨���еı����̨����
}fm_play_status_t;


/* FM ��������ö������*/
typedef enum
{
    /*FMģ���ʼ��*/
    FM_INIT = 0,
    /*FMģ�����standby*/
    FM_STANDBY,
    /*����Ƶ�㿪ʼ����*/
    FM_SETFREQ,         
    /*��ȡ��ǰ״̬��Ϣ*/
    FM_GETSTATUS,
    /*������������*/
    FM_MUTE_VOL,
    /*���������̨����*/
    FM_SEARCH,
    /*����Ӳ����̨����*/
    FM_HARDSEEK,
    /* ���õ�̨Ƶ��*/
    FM_SETBAND,
    /* ������̨����*/
    FM_SETTHROD,
    /* �ֶ��˳�Ӳ��seek ����*/
    FM_BREAKSEEK,
    /* ��ȡӲ��seek �Ƿ���ɱ�ʶ*/
    FM_GET_FLAG,
    /* ��ȡ��ǰƵ����Ϣ*/
    FM_GET_BAND,
    /* ��ȡ��ǰƵ����Ϣ*/
    FM_GET_FREQ,
    /* ��ȡ��ǰ��̨�ź�ǿ��*/
    FM_GET_INTENTY,
    /* ��ȡ����״̬*/
    FM_GET_ANTEN,
    /* ��ȡ��̨������״̬*/
    FM_GET_STEREO,
} fm_cmd_e;

/* FM �������ϲ��ṩ��ͳһ��ڶ���*/
typedef void* (*fm_op_func)(void*, void*, void*);

/* �����ڲ�ʵ�ֵľ���ӿں�������*/
typedef struct
{
    fm_op_func  Init;
    fm_op_func  Standby;
    fm_op_func  SetFreq;
    fm_op_func  GetStatus;
    fm_op_func  Mute;
    fm_op_func  Search;
    fm_op_func  HardSeek;
    fm_op_func  SetBand;    
    fm_op_func  SetThrod;   
    fm_op_func  SeekBreak;
    fm_op_func  GetSeekOver;
    fm_op_func  GetBand;
    fm_op_func  GetFreq;
    fm_op_func  GetIntsity;
    fm_op_func  GetAntenna;
    fm_op_func  GetStereo;  
} fm_driver_operations;

extern void* fm_op_entry(void *param1, void *param2, void *param3, fm_cmd_e cmd)__FAR__;

#define fm_open(a,b,c)            (int)fm_op_entry((void*)(uint32)(a), (void*)(uint32)(b), (void*)(uint32)(c), FM_INIT)
#define fm_close()              (int)fm_op_entry((void*)(0), (void*)(0), (void*)(0), FM_STANDBY)
#define fm_set_freq(a)          (int)fm_op_entry((void*)(uint32)(a), (void*)(0), (void*)(0), FM_SETFREQ)
#define fm_get_status(a,b)      (int)fm_op_entry((void*)(a), (void*)(uint32)(b), (void*)(0), FM_GETSTATUS)
#define fm_mute(a)              (int)fm_op_entry((void*)(uint32)(a), (void*)(0), (void*)(0), FM_MUTE_VOL)
#define fm_soft_seek(a,b)         (int)fm_op_entry((void*)(uint32)(a),  (void*)(uint32)(b), (void*)(0), FM_SEARCH)
#define fm_hard_seek(a,b)       (int)fm_op_entry((void*)(uint32)(a), (void*)(uint32)(b), (void*)(0), FM_HARDSEEK)
#define fm_set_band(a)          (int)fm_op_entry((void*)(uint32)(a), (void*)(0), (void*)(0), FM_SETBAND)
#define fm_set_throd(a)         (int)fm_op_entry((void*)(uint32)(a), (void*)(0), (void*)(0), FM_SETTHROD)
#define fm_set_breakseek()      (int)fm_op_entry((void*)(0), (void*)(0), (void*)(0), FM_BREAKSEEK)
#define fm_get_seekflag(a)      (int)fm_op_entry((void*)(a), (void*)(0), (void*)(0), FM_GET_FLAG)
#define fm_get_band(a)          (int)fm_op_entry((void*)(a), (void*)(0), (void*)(0), FM_GET_BAND)
#define fm_get_freq(a)          (int)fm_op_entry((void*)(a), (void*)(0), (void*)(0), FM_GET_FREQ)
#define fm_get_intensity(a)     (int)fm_op_entry((void*)(a), (void*)(0), (void*)(0), FM_GET_INTENTY)
#define fm_get_antena(a)        (int)fm_op_entry((void*)(a), (void*)(0), (void*)(0), FM_GET_ANTEN)
#define fm_get_stereo(a)        (int)fm_op_entry((void*)(a), (void*)(0), (void*)(0), FM_GET_STEREO)



#endif

