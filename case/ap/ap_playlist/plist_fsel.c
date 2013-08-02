/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    ������д�ļ��ĸ���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "plist.h"
#include "rdata_ex.h"

//#define DIRLAYER_MAX 8

static const uint8 rec_dir_name[] = "RECORD  ";

static uint8 plist_dir_layer _BANK_DATA_ATTR_;
static uint16 plist_file_no _BANK_DATA_ATTR_;
static uint32 plist_cur_ext_name _BANK_DATA_ATTR_;
static uint32 plist_sel_bitmap _BANK_DATA_ATTR_;

static bool filtrate_dir(void);
//static bool dir_next_layer(uint8 *strfile);
static bool dir_next(uint8* strfile);
bool plist_fsel_init(uint32 file_type_bit);

//#define PLIST_DEBUG_FSEL

#ifdef PLIST_DEBUG_FSEL
static uint8 dir_name_buffer[64];
static uint8 file_name_buffer[64];
#endif
/******************************************************************************/
/*
 * \par  Description:��ʼ���ļ�ѡ����
 *
 * \param[in]    file_type_bit--ָ���ļ�����չ��bitmap

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/

bool plist_fsel_init(uint32 file_type_bit)
{
    plist_dir_layer = 0;
    plist_file_no = 0;
    plist_cur_ext_name = 0;
    plist_sel_bitmap = file_type_bit;
    //Ĭ�Ͻ����Ŀ¼
    vfs_cd(ap_vfs_mount, CD_ROOT, 0);
    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:����ѡ����ļ�����
 *
 * \param[in]    file_type_bit--�ļ���չ��bitmap
 reset_dir_flag ����Ŀ¼��,��ture���³�ʼ������Ŀ¼

 * \param[out]   none

 * \return       TRUE or FALSE

 * \note
 *******************************************************************************/

bool plist_fsel_set_typebit(uint32 file_type_bit, bool reset_dir_flag)
{
    plist_sel_bitmap = file_type_bit;
    if (reset_dir_flag == TRUE)
    {
        plist_fsel_init(file_type_bit);
    }
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ��ȡ���̵���һ���ļ�
 *
 * \param[in]

 * \param[out]    strfile--����ļ�����չ��

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
//bool  plist_fsel_get_nextfile (char *strfile )
//{
//    return dir_next_layer(strfile);
//}

/******************************************************************************/
/*
 * \par  Description: ��ȡ��ǰĿ¼�µ��¸��ļ�
 *
 * \param[in]

 * \param[out]      strfile--����ļ�����չ��

 * \return          TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool dir_next(uint8* strfile)
{
    uint8 dir_type;

    if (plist_file_no == 0)
    {
        dir_type = DIR_HEAD;
    }
    else
    {
        dir_type = DIR_NEXT;
    }

    plist_cur_ext_name = vfs_dir(ap_vfs_mount, dir_type, NULL, plist_sel_bitmap);
    if (0 != plist_cur_ext_name)
    {
        plist_file_no++;
        *(uint32*) strfile = plist_cur_ext_name;

#ifdef PLIST_DEBUG_FSEL
        vfs_get_name(ap_vfs_mount, file_name_buffer,30);
#endif

        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: ��ȡ���̵���һ���ļ�
 *
 * \param[in]

 * \param[out]        strfile--����ļ�����չ��

 * \return            TRUE OR FALSE

 * \note
 *******************************************************************************/
//static bool dir_next_layer(uint8 *strfile)
bool plist_fsel_get_nextfile(uint8 *strfile)
{
    //�ڵ�ǰĿ¼�������ļ�

    if (FALSE == dir_next(strfile))
    {
        CurrentDiragain:

        if (plist_dir_layer >= (DIRLAYER_MAX - 1))//�����ǰ��8��Ŀ¼�Ͳ������ļ���
        {
            goto ParentDiragainNext;
        }

        //Ѱ�Ҹ�Ŀ¼�ĵ�һ�����ļ���
        if ((0 != vfs_dir(ap_vfs_mount, DIR_HEAD, NULL, EXT_NAME_ALL_DIR)) && (FALSE == filtrate_dir()))
        {

#ifdef PLIST_DEBUG_FSEL
            vfs_get_name(ap_vfs_mount, dir_name_buffer,30);
#endif

            if (FALSE != vfs_cd(ap_vfs_mount, CD_SUB, NULL))
            {
                //�������ļ��У���ø����ļ��е��ļ�����
                plist_dir_layer++;
                plist_file_no = 0;
                // ��鵱ǰĿ¼�Ƿ��к��ʵ��ļ�
                if (FALSE != dir_next(strfile))
                {
                    return TRUE;
                }
                else //�����Ŀ¼
                {
                    //if(plist_dir_layer>DIRLAYER_MAX-1)
                    //    goto ParentDiragainNext;
                    //else
                    goto CurrentDiragain;
                }
            }
            else //������
            {
                return FALSE;
            }
        }
        else //���û�����ļ��У���������һ�㣬Ѱ�ҵ�ǰĿ¼����һ��Ŀ¼��ƽ��Ѱ�ң�
        {
            ParentDiragainNext:

            if (0 == plist_dir_layer)
            {
                return FALSE;
            }

            if (FALSE != vfs_cd(ap_vfs_mount, CD_BACK, NULL))
            {
                plist_dir_layer--;
                if ((0 != vfs_dir(ap_vfs_mount, DIR_NEXT, NULL, EXT_NAME_ALL_DIR)) && (FALSE == filtrate_dir()))
                {
#ifdef PLIST_DEBUG_FSEL
                    vfs_get_name(ap_vfs_mount, dir_name_buffer,30);
#endif

                    vfs_cd(ap_vfs_mount, CD_SUB, NULL);
                    plist_dir_layer++;
                    plist_file_no = 0;

                    if (FALSE != dir_next(strfile))
                    {
                        return TRUE;
                    }
                    else
                    {
                        // �����Ŀ¼.
                        //if(plist_dir_layer>=DIRLAYER_MAX)
                        //    goto ParentDiragainNext;
                        //else
                        goto CurrentDiragain;
                    }
                }
                else
                {
                    goto ParentDiragainNext;
                }
            }

            else
            { //�쳣����
                return FALSE;
            }
        }
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:���˸�Ŀ¼�µ�RECORDĿ¼
 *
 * \param[in]

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool filtrate_dir(void)
{
    uint8 dir_name_buffer[32];
    uint16 name_len;
    uint8 add_len, i, j;
    uint8 *buf;

    if ((plist_dir_layer != 0) || ((fsel_scan_flag & SCAN_FILTRATE_BIT) != 0))
    {
        return FALSE;
    }

    name_len = vfs_get_name(ap_vfs_mount, dir_name_buffer, 16);

    if ((name_len == 11) && (dir_name_buffer[0] != 0xff))
    {
        //8+3
        buf = dir_name_buffer;
        name_len = 8;
        add_len = 1;
    }
    else
    {
        if ((name_len == 8) && (dir_name_buffer[0] == 0xff))
        {
            //unciode
            buf = dir_name_buffer + 2;
            name_len = 6;
            add_len = 2;
        }
        else
        {
            return FALSE;
        }
    }

    i = 0;
    for (j = 0; j < name_len; j++)
    {
        if (rec_dir_name[j] != buf[i])
        {
            return FALSE;
        }
        i = i + add_len;
    }

    fsel_scan_flag = fsel_scan_flag | SCAN_FILTRATE_BIT;
    
    //���˳ɹ�������һ���ļ��з���
    if(0!=vfs_dir(ap_vfs_mount, DIR_NEXT, NULL, EXT_NAME_ALL_DIR))
    {
    //�ҵ������ҵ��ļ�������	    
        return FALSE;
    }
    else
    {
    //δ�ҵ����ܵݹ�����	    
        return TRUE;
    }

}

