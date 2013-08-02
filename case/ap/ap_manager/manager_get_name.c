/*******************************************************************************
 *                              US212A
 *                            Module: Manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_get_name.c
 * \brief    ����Ӧ��ID��ȡӦ������
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "manager.h"

const char app_name_ram[25][12] =
{
    "music.ap", "audible.ap", "video.ap", "picture.ap", "ebook.ap", "browser.ap", "voice.ap", "record.ap",
    "radio.ap", "tools.ap", "setting.ap", "user1.ap", "user2.ap", "user3.ap", "manager.ap", "mainmenu.ap",
    "playlist.ap", "config.ap", "udisk.ap", "mengine.ap", "fmengine.ap", "alarm.ap", "mtp.ap", "mtpsync.ap",
    "fwupdate.ap"
};

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ���� applib ģ���ϵĳ�ʼ��������Ӧ�õ� main ������ڴ�����
 * \param[in]    char *namebuf�����Ӧ�����Ƶ�buffer��ַ
 * \param[in]    uint8 ap_id��Ӧ��ID
 * \param[out]   none
 * \return       none
 * \retval
 * \note
 *******************************************************************************/
void _get_app_name(char *namebuf, uint8 ap_id)
{
    libc_memcpy(namebuf, app_name_ram[ap_id], 12);
}

