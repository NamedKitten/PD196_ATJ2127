/********************************************************************************
*                        USDK130
*                     watchdog driver
*
*                (c) Copyright 2002, Actions Co,Ld.
*                        All Right Reserved
*
* File: watchdog.h
* By  : huanghe
* Version: 1> v1.00     first version     2007-10-30 15:51
*******************************************************************************
*/

/*!
* \file  watchdog.h
* \brief ��������״̬������������Ķ���ͷ�ļ�
* \author wurui
* \par GENERAL DESCRIPTION:
*           ÿһ�������ڴ�������Ҫ���ϵ���������򱨸�����״̬, ����������ٱ����������Ƿ��������ڲ�������״̬,
*       ��������������ϵͳ.
*  \version 1.0
*  \date  2010/09/01
*/

#ifndef __WDOG_H__
#define __WDOG_H__


/*!
*  ��ѯ���,��λ: ms \n
*/ 
#define WDTASK_INTERVAL 500

/*!
*  WDD �豸��� \n
*  \li\b time_out   : WDD�ĳ�ʱ�趨
*  \li\b count      : WDD�ļ�ʱ��
*/ 
typedef struct
{
    u32 time_out;
    u32 count;
    s32 index;
}WDD_handle_t;

/*!
*  WDD ���� \n
*  \li\b WDD_CMD_SET_TIME   : ���ü�ʱ��ʱʱ��, ������λ:ms
*/ 
typedef enum
{
    WDD_CMD_SET_TIME,
}WDD_cmd_t;

#endif  /*#ifndef __WDOG_H__*/
