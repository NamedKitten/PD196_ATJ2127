/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-10-14 16:58     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_msgmap_data.c
 * \brief    ������Ϣӳ����弯�ϡ�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               �������� common ��Ϣѭ������ӳ����塣
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-2-16
*******************************************************************************/

#include "common_func.h"

/*! \cond COMMON_API */

/*!
 * \brief
 *  delete_key_map_list��ɾ���ļ��ؼ�����ӳ���
 */
const key_map_t delete_key_map_list[] = 
{
    /*! �̰� VOL ������ֹɾ�������ļ� */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_DELETE_STOP},
    
    /*! ����Ӧ���¿�ݼ� */
    {{KEY_LOCKHOLD, 0, KEY_TYPE_ALL}, MSG_NULL},
    {{KEY_VOL, 0, KEY_TYPE_LONG}, MSG_NULL},
    
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  dialog_key_map_list���Ի���ؼ�����ӳ���
 */
const key_map_t dialog_key_map_list[] = 
{
    /*! PREV ����ת��Ϊ PREV_BUTTON �¼� */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_DIALOG_PREV_BUTTON},
    /*! NEXT ����ת��Ϊ NEXT_BUTTON�¼� */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_DIALOG_NEXT_BUTTON},
    /*! �̰�KEY_PLAY ����ת��Ϊ CONFIRM�¼� */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_DIALOG_CONFIRM},
    /*! �̰�KEY_VOL ����ת��Ϊ CANCEL�¼� */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_DIALOG_CANCEL},
    /*! �̰�KEY_MODE ����ת��Ϊ CANCEL�¼� */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_DIALOG_CANCEL},
    
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  directory_key_map_list���ļ����������ӳ���
 */
const key_map_t directory_key_map_list[] = 
{
    /*! PREV ����ת��Ϊ SELECT_PREV �¼� */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_DIRECTORY_SELECT_PREV},
    /*! PREV ���� ̧�� ת��Ϊ SELECT_STOP �¼� */
    {{KEY_PREV, 0, KEY_TYPE_SHORT_UP}, EVENT_DIRECTORY_SELECT_STOP},
    /*! NEXT ����ת��Ϊ SELECT_NEXT �¼� */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_DIRECTORY_SELECT_NEXT},
    /*! NEXT ���� ̧�� ת��Ϊ SELECT_STOP �¼� */
    {{KEY_NEXT, 0, KEY_TYPE_SHORT_UP}, EVENT_DIRECTORY_SELECT_STOP},
    /*! �̰�KEY_PLAY ����ת��Ϊ ENTER_SON �¼� */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_DIRECTORY_ENTER_SON},
    /*! �̰�KEY_VOL ����ת��Ϊ BACK_PARENT �¼� */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_DIRECTORY_BACK_PARENT},
    /*! �̰�KEY_MENU ����ת��Ϊ ENTER_OPTION �¼� */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_DIRECTORY_ENTER_OPTION},
    
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  keylock_list��������UI����ӳ���
 */
const key_map_t keylock_list[] = 
{
    /*! ���������а�������������״̬����ֹ��������ʾ�������� */
    {{KEY_LOCK, 0, KEY_TYPE_DOWN}, EVENT_ENTER_KEYLOCK},
    /*! ���������а�������������״̬����ֹ��������ʾ�������� */
    {{KEY_UNLOCK, 0, KEY_TYPE_DOWN}, EVENT_ENTER_KEYLOCK},
    
    /*! lock ״̬�²�����������ݼ� */
    {{KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, MSG_NULL},
    
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  menulist_key_map_list���˵��б���ӳ���
 */
const key_map_t menulist_key_map_list[] = 
{
    /*! PREV ����ת��Ϊ SELECT_PREV �¼� */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_MENULIST_SELECT_PREV},
    /*! NEXT ����ת��Ϊ SELECT_NEXT �¼� */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_MENULIST_SELECT_NEXT},
    /*! �̰�KEY_PLAY ����ת��Ϊ ENTER_SON �¼� */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_MENULIST_ENTER_SON},
    /*! �̰�KEY_VOL ����ת��Ϊ BACK_PARENT �¼� */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_MENULIST_BACK_PARENT},
    /*! �̰�KEY_MENU ����ת��Ϊ ENTER_OPTION �¼� */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_MENULIST_ENTER_OPTION},
    
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  parameter_key_map_list���������ÿ򰴼�ӳ���
 */
const key_map_t parameter_key_map_list[] = 
{
    /*! MODE ����ת��Ϊ SELECT_NEXT �¼� */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_PARAMETER_SELECT_NEXT},
    /*! NEXT ����ת��Ϊ VALUE_INC �¼� */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_PARAMETER_VALUE_INC},
    /*! PREV ����ת��Ϊ VALUE_DEC �¼� */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_PARAMETER_VALUE_DEC},
    /*! �̰�KEY_PLAY ����ת��Ϊ CONFIRM �¼� */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_PARAMETER_CONFIRM},
    /*! �̰�KEY_VOL ����ת��Ϊ CANCEL �¼� */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_PARAMETER_CANCEL},
    
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  shutoff_key_map_list���ػ�ȷ��UI����ӳ���
 */
const key_map_t shutoff_key_map_list[] = 
{
    /*! ֻ���� PLAY ����̧��ʱ��ֹ */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_SHUTOFF_QUIT},
    
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  textread_key_map_list���ı���Ϣ�鿴������ӳ���
 */
const key_map_t textread_key_map_list[] = 
{
    /*! PREV ����ת��Ϊ PREV_PAGE �¼� */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_TEXTREAD_PREV_PAGE},
    /*! NEXT ����ת��Ϊ NEXT_PAGE �¼� */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_TEXTREAD_NEXT_PAGE},
    /*! �̰�KEY_VOL ����ת��Ϊ CANCEL �¼� */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_TEXTREAD_CANCEL},
    /*! �̰�KEY_PLAY ����ת��Ϊ CANCEL �¼� */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_TEXTREAD_CANCEL},
    
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  usbconnect_key_map_list���豸���ӶԻ��򰴼�ӳ���
 */
const key_map_t usbconnect_key_map_list[] = 
{
    /*! PREV ����ת��Ϊ PREV_BUTTON �¼� */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_DIALOG_PREV_BUTTON},
    /*! NEXT ����ת��Ϊ NEXT_BUTTON �¼� */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_DIALOG_NEXT_BUTTON},
    /*! �̰�KEY_PLAY ����ת��Ϊ DIALOG_CONFIRM �¼� */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_DIALOG_CONFIRM},
    /*! �̰�KEY_VOL ����ת��Ϊ DIALOG_CANCEL �¼� */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_DIALOG_CANCEL},
    /*! �̰�KEY_MODE ����ת��Ϊ DIALOG_CANCEL �¼� */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_DIALOG_CANCEL},
    
    /*! ����Ӧ���¿�ݼ� */
    {{KEY_PLAY, 0, KEY_TYPE_LONG}, MSG_NULL},
    {{KEY_LOCKHOLD, 0, KEY_TYPE_ALL}, MSG_NULL},
    {{KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, MSG_NULL},
    
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  volume_key_map_list������������ӳ���
 */
const key_map_t volume_key_map_list[] = 
{
    /*! NEXT ����ת��Ϊ VOLUME_INC �¼� */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_VOLUME_INC},
    /*! PREV ����ת��Ϊ VOLUME_DEC �¼� */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_VOLUME_DEC},
    /*! �̰�KEY_PLAY ����ת��Ϊ VOLUME_CANCEL �¼������������Ǽ�ʱ���ڵģ����践��CONFRIM */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_VOLUME_CANCEL},
    /*! �̰�KEY_VOL ����ת��Ϊ VOLUME_CANCEL �¼������������Ǽ�ʱ���ڵģ����践��CONFRIM */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_VOLUME_CANCEL},
    /*! �̰�KEY_MODE ����ת��Ϊ VOLUME_CANCEL �¼������������Ǽ�ʱ���ڵģ����践��CONFRIM */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_VOLUME_CANCEL},
    
    /*! �����������ڿ�ݼ� */
    {{KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, MSG_NULL},
    
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  limit_key_map_list�������������ð���ӳ���
 */
const key_map_t limit_key_map_list[] = 
{
    /*! NEXT ����ת��Ϊ VOLUME_INC �¼� */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_VOLUME_INC},
    /*! PREV ����ת��Ϊ VOLUME_DEC �¼� */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_VOLUME_DEC},
    /*! �̰�KEY_PLAY ����ת��Ϊ VOLUME_CONFIRM�¼� */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_VOLUME_CONFIRM},
    /*! �̰�KEY_VOL ����ת��Ϊ VOLUME_CANCEL�¼� */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_VOLUME_CANCEL},
    /*! �̰�KEY_MODE ����ת��Ϊ VOLUME_CANCEL�¼� */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_VOLUME_CANCEL},
    
    /*! �����������ڿ�ݼ� */
    {{KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, MSG_NULL},
    
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  key_shortcut_list����ݼ�ӳ���
 */
const key_map_t key_shortcut_list[] = 
{
    /*! ���� PLAY �����ػ� */
    {{KEY_PLAY, 0, KEY_TYPE_LONG}, EVENT_ENTER_SHUTOFF_ASK},
    /*! ������������״̬���������� */
    {{KEY_LOCK, 0, KEY_TYPE_DOWN}, EVENT_ENTER_KEYLOCK},
    /*! ������������״̬�����̽��� */
    {{KEY_UNLOCK, 0, KEY_TYPE_DOWN}, EVENT_ENTER_KEYLOCK_UN},
    /*! ���ⰴ������������סʱ�����κ�������������ת��Ϊ KEY_LOCKHOLD����ʾ������ס */
    {{KEY_LOCKHOLD, 0, KEY_TYPE_ALL}, EVENT_ENTER_KEYLOCK_HOLD},
    /*! ���� VOL �������������ڿ� */
    {{KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_ENTER_VOLUMEBAR},
    
    /*! ������־ */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*! \endcond */
