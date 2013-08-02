/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     eh_id3_aac.c
 * \brief    ������д�ļ��ĸ���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_id3.h"

extern uint8 id3_temp_buf[SECTOR_SIZE];
extern uint8 key_buf[KEY_BUF_LEN];
extern uint8 check_count;
extern uint8 check_flag[8]; //����ID3Ҫ�ر��
extern uint16 id3_pos_buffer; //��ǰTempBufδ�����ַ�������
extern uint32 id3_pos_file; //��ǰ�ļ��ľ�ȷָ��

extern id3_save_t *id3_save_p; //FrameID�洢λ��
extern id3_info_t *id3_info_p; //ap���ṩ�Ĵ洢�ṹ


/******************************************************************************/
/*
 * \par  Description:����aac���͵�id3��Ϣ
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool get_id3_aac(void)
{
    if (FALSE != get_id3_aax())
    {
        return TRUE;
    }

    vfs_file_seek(vfs_mount, 0, SEEK_SET, id3_handle);
    if (FALSE != get_id3_mp3())
    {
        return TRUE;
    }
    vfs_file_seek(vfs_mount, 0, SEEK_SET, id3_handle);
    return get_id3_ape();
}

