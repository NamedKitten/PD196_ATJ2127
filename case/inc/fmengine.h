/********************************************************************************
 *                            Module: radio_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-08 15:00     1.0             build this file
********************************************************************************/
#ifndef __FMENGINE_H__
#define __FMENGINE_H__

#include "psp_includes.h"
#include "fm_interface.h"


typedef enum
{
    STATUS_PLAY = 0,                   // ��������״̬
    STATUS_PAUSE,                       //������ͣ״̬(����״̬)
    STATUS_ERROR,                      //Ӳ������
} FM_Playstatus_e;

typedef enum
{
    STERE = 0,               //[0]������
    MONO,                    //[1]������        
} FM_Audio_e;

typedef enum
{
    releaseMUTE = 0,        //�������
    SetMUTE,                   //����        
} FM_MUTE_e;

typedef enum
{
    HARDSEEK = 0,            //����Ӳ��seek ģʽ
    SOFTSEEK,                  //�������seek ģʽ
} FM_SeekMode_e;

typedef enum
{
    DIR_UP = 0x10,             //������̨
    DIR_DOWN = 0x20,       //������̨
}FM_SeekDir_e;


/* ��̨����ģʽ��Ϊ����ģʽ�Ͳ˵�ѡ��ģʽ
** ����ڷ��û���̨����ģʽ�£����ŵĲ��κ�
** �˵���ѡ��Ĳ���һ�¡�
** �û���̨����ʱ������ģʽ�Ĳ���ΪBand_MAX 
** �˵��еĲ���ά�ֲ���
** ��ʱ���ѡ�����Ԥ��򱣴浽Ԥ�裬���ǽ�
** �û���̨��Ƶ�㱣�浽��ǰ�˵�����ָ����
** Ԥ���б���ߴӲ˵�����ָ�����б�ѡ��Ƶ��ɾ��
** �����û���̨ʱ������ڲ˵��иı�˵�����
** ��ѡ��ֵ������в����л���������ΪԤ�貥��ģʽ*/
typedef enum                    
{
    Band_China_US = 0,              //china/USA band   87500---108000,  step 100kHz
    Band_Japan,                         //Japan band   76000---90000, step 100kHz
    Band_Europe,                       //Europe band  87500---108000, step 50kHz
    Band_MAX                           //�����û���̨ʱ������ģʽ����ΪBand_MAX
} radio_band_e;


/*Ӳ��seek ״̬ö��*/
typedef enum
{
    HARDSEEK_NOT_START = 0,             //δ����Ӳ����̨���ߴ���Ӳ����̨δ�ѵ���Ч̨
    HARDSEEK_DOING,                           //Ӳ����̨������
    HARDSEEK_COMPLETE,                     //����Ӳ����̨���̽������ѵ���Ч̨
} hardseek_status_e;

typedef struct
{
    UINT16            FM_CurrentFreq;           //��ǰƵ��
    UINT16            FM_SignalStg;             //��ǰƵ���ź�ǿ��
    FM_Audio_e        FM_Stereo_Status;        //0������������0��������
    radio_band_e      FM_CurBand;                //��ǰ���� 
    FM_Playstatus_e   FM_VolStatus;               //��ǰ����״̬
    hardseek_status_e STC_flag;                      //Ӳ��seek �Ƿ���ɱ�־����Ҫ����Ӳ��seek ��Ҫ
} Engine_Status_t;


typedef enum
{
    PA_MODE = 0,                                      //��ֱ��
    PA_DDV_MODE,                               //ֱ��
} PA_DDV_Mode_e;

/* FM ����������Ϣ�ṹ��*/
typedef struct
{
    uint16  magic;
    PA_DDV_Mode_e  paddv_mode;               //PA ֱ��ģʽ���ֱ��ģʽ          
    uint8  fm_threshold;                               //ģ��Ĭ����̨����ֵ                          
} FMEngine_cfg_t;

/* FM ģ��״̬ö��*/
typedef enum
{
    MODULE_NO_INIT = 0,              //ģ�鴦��δ��ʼ��״̬
    MODULE_WORK,                       //ģ���ʼ����ɣ�������������״̬
    MODULE_STANDBY,                  //ģ�鴦��Standby ״̬
} Module_Status_e;

#endif //__FMENGINE_H__
