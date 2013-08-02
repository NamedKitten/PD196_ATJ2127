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
 * \file     eh_lrc_load.c
 * \brief    �������
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_lrc.h"
#include  "storage_device.h"

uint8 lrc_temp_buf[SECTOR_SIZE] _BANK_DATA_ATTR_; //��lrc����
uint8 lrc_save_buf[SECTOR_SIZE] _BANK_DATA_ATTR_; //�������ݻ���

uint32 lrc_pos_buffer _BANK_DATA_ATTR_;
uint32 lrc_pos_file _BANK_DATA_ATTR_;
uint32 lrc_file_len _BANK_DATA_ATTR_;

uint8 positiveflag _BANK_DATA_ATTR_; //TRUE--'+',FALSE---'-'
uint8 offset_sec _BANK_DATA_ATTR_; // 1 s
uint8 offset_ms _BANK_DATA_ATTR_; //10ms

lrc_lable_t *lrc_lab_p _BANK_DATA_ATTR_; // ��ʱ���ǩָ��
uint8 *buf_point _BANK_DATA_ATTR_; //lrc��buf����ָ��
uint8 *save_lrc_buf_p _BANK_DATA_ATTR_; //��������ָ��
uint8 *lrc_vram_p _BANK_DATA_ATTR_;

uint8 get_time_buf[8] _BANK_DATA_ATTR_; //��ȡʱ���ǩ��ʱ����
uint8 *get_time_p _BANK_DATA_ATTR_; //���浥��ʱ���ǩָ��


uint16 same_str_count _BANK_DATA_ATTR_; //ͬ��ʵ�ʱ���ǩ����,1-ֻ��1����ǩ��2-��2����ǩ
uint16 lrc_str_len _BANK_DATA_ATTR_; //ÿ����ʵĳ���
uint16 lab_lrc_offset _BANK_DATA_ATTR_; //ÿ������������е�ƫ��λ��

//static const uint8 lrc_asc[]="LRC";
//static const uint16 lrc_unicode[4]={'L','R','C','\0'};

uint16 len_sting(uint8 *str, uint8 str_type);

bool read_page_data(void);
bool explain_lrc_file(void);
void set_info_to_vm(uint8 flag);

extern void sort_lrc_label(void);
extern bool read_offset(void);
extern uint8 read_time_begin(void);
extern uint8 read_time_minute(void);
extern uint8 read_time_second(void);
extern uint8 read_string_data(void);
extern void explain_lrc_init(void);
extern void explain_ok_deal(void);
extern bool open_lrc_file(char *lrc_name);
extern bool add_time_offset(void);

#define  READ_FUN_COUNT  4
typedef unsigned char (*read_func_f)(void);

static const read_func_f read_lrc_data[READ_FUN_COUNT] =
{ read_time_begin, read_time_minute, read_time_second, read_string_data };

/******************************************************************************/
/*
 * \par  Description: open��Ƕ�ĸ��
 *
 * \param[in]    music_filename��������
 dlb_offset�����ƫ��

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool lyric_open_dlb(char * music_filename, uint32 dlb_offset)
{
    if (lrc_handle != 0)
    {
        vfs_file_close(vfs_mount, lrc_handle);
        lrc_handle = 0;
    }

    lrc_handle = vfs_file_open(vfs_mount, lrc_temp_buf, 2);
    if (lrc_handle == 0)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: װ��lrc���
 *
 * \param[in]    music_filename��������

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool lyric_open(char * music_filename)
{
    uint8 result;

    if (lrc_handle != 0)
    {
        vfs_file_close(vfs_mount, lrc_handle);
        lrc_handle = 0;
    }

    if (FALSE != open_lrc_file(music_filename))
    {
#ifndef PC        
        if(fsel_init_val.disk == DISK_C)
        {
        	base_enter_compact_mode();
    	}
#endif    	
        result = read_offset(); //��OFFSETֵ
        if (FALSE == result)
        {
            offset_ms = 0;
            offset_sec = 0;
        }

        result = explain_lrc_file();//right 0; error; 1

        vfs_file_close(vfs_mount, lrc_handle);
        lrc_handle = 0;
        
#ifndef PC         
        if(fsel_init_val.disk == DISK_C)
        {
        	base_exit_compact_mode();
        }
#endif        
        
    }
    else
    {
        result = FALSE;
    }

    set_info_to_vm(result);

    return result;

}

/******************************************************************************/
/*
 * \par  Description:�������������
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool explain_lrc_file(void)
{
    ret_read_e result_read;
    uint8 func_num;
    //    uint16  temp;
    //    read_func_f read_lrc_func;

    explain_lrc_init();

    func_num = 0;

    while (lrc_pos_file <= lrc_file_len)
    {
        //        read_lrc_func=read_lrc_data[func_num];
        //        result_read=read_lrc_func();

        result_read = read_lrc_data[func_num]();

        switch (result_read)
        {
            case READ_NEXT:
            func_num++;
            break;

            case READ_DATA_END:
            case READ_INVALID_LAB:
            func_num = 0;
            break;

            case READ_LAB_START:
            func_num = 1;
            break;

            case READ_ZERO:
            case READ_FILE_END:
            goto explain_ok;
            break;

            case READ_FILE_ERROR:
            default:
            goto explain_err;
            break;

        }

        if (func_num >= READ_FUN_COUNT)
        {
            goto explain_err;
        }

    }

    explain_ok:

    if (0 == time_lab_count)
    {
        return FALSE;
    }

    //��ʽ�����ʶ�������ڱ�ǩ����time_lab_count
    lrc_lab_p->min = 0xff;
    lrc_lab_p->sec = 0x55;
    lrc_lab_p->p_ms = 0xaa;
    lrc_lab_p->lrc_len = 0;
    lrc_lab_p->offset_byte = 0;

    add_time_offset();
    explain_ok_deal();

    return TRUE;

    explain_err: time_lab_count = 0;
    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: ���ļ�һ������
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool read_page_data(void)
{
    //    uint8 result;
    if (lrc_pos_buffer >= SECTOR_SIZE)
    {
        //�����ݲ����Ͷ�һ��PAGE����
        //��lrc_pos_buffer������
        lrc_pos_buffer = vfs_file_read(vfs_mount, lrc_temp_buf, SECTOR_SIZE, lrc_handle);
        if (lrc_pos_buffer != 0)
        {
            if (lrc_pos_buffer != SECTOR_SIZE)
            {
                //��������,�����ļ����ձ�־
                lrc_temp_buf[lrc_pos_buffer] = 0x00;
            }
            lrc_pos_file = lrc_pos_file + lrc_pos_buffer;
            lrc_pos_buffer = 0;
        }
        else
        {
            lrc_pos_buffer = lrc_file_len + 1;
            return FALSE;
        }
    }
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ʶ���ַ�����
 *
 * \param[in]    data���ַ�

 * \param[out]

 * \return        �ַ�������

 * \note
 *******************************************************************************/
char_type_e check_char_type(char data)
{
    if ((data >= '0') && (data <= '9'))
    {
        return CHAR_TYPE_NUM;
    }

    if ((data >= 'A') && (data <= 'Z'))
    {
        return CHAR_TYPE_UPER;
    }

    if ((data >= 'a') && (data <= 'z'))
    {
        return CHAR_TYPE_DNER;
    }

    if (data == ' ')
    {
        return CHAR_TYPE_SPACE;
    }
    return CHAR_TYPE_OTHER;
}

/******************************************************************************/
/*
 * \par  Description: ���ø��VRAM��ʶ��Ϣ
 *
 * \param[in]    ��ʶ,1--дinfo��0--��info

 * \param[out]

 * \return        �ַ�������

 * \note
 *******************************************************************************/
void set_info_to_vm(uint8 flag)
{
    if (0 == flag)
    {
        sys_vm_read(&time_lab_count, LRC_INFO_VM, sizeof(time_lab_count));
        if (0 != time_lab_count)
        {
            time_lab_count = 0;
        }
        else
        {
            return;
        }
    }

    sys_vm_write(&time_lab_count, LRC_INFO_VM);

    return;
}

/******************************************************************************/
/*
 * \par  Description:�رո��
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
//bool lyric_close(void)
//{
//
//    return TRUE;
//}
