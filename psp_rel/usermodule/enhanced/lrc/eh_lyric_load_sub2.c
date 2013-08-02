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
 * \file     eh_lrc_load_sub2.c
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

extern uint32 lrc_pos_buffer;
extern uint32 lrc_pos_file;
extern uint32 lrc_file_len;

extern uint8 positiveflag; //TRUE--'+',FALSE---'-'
extern uint8 offset_sec; // 1 s
extern uint8 offset_ms; //10ms


extern lrc_lable_t *lrc_lab_p; // ��ʱ���ǩָ��
extern uint8 *buf_point; //lrc��buf����ָ��
extern uint8 *save_lrc_buf_p; //��������ָ��
extern uint8 *lrc_vram_p;

extern uint8 get_time_buf[8]; //��ȡʱ���ǩ��ʱ����
extern uint8 *get_time_p; //���浥��ʱ���ǩָ��


extern uint16 same_str_count; //ͬ��ʵ�ʱ���ǩ����,1-ֻ��1����ǩ��2-��2����ǩ
extern uint16 lrc_str_len; //ÿ����ʵĳ���
extern uint16 lab_lrc_offset; //ÿ������������е�ƫ��λ��


extern uint8 lrc_temp_buf[SECTOR_SIZE]; //��lrc����
extern uint8 lrc_save_buf[SECTOR_SIZE]; //�������ݻ���


static void deal_time_lab(void);
static void write_lrc_len(void);

extern bool read_page_data(void);
//extern char_type_e check_char_type(char data);

/******************************************************************************/
/*
 * \par  Description: ��һ��ʱ���ǩ��ʼ
 *
 * \param[in]

 * \param[out]

 * \return        �������

 * \note
 *******************************************************************************/
uint8 read_time_begin(void)
{
    uint8 temp_data;

    libc_memset(get_time_buf, 0, sizeof(get_time_buf));

    while (lrc_pos_file <= lrc_file_len)
    {
        if (lrc_pos_buffer < SECTOR_SIZE)
        {
            temp_data = *buf_point;
            buf_point++;
            lrc_pos_buffer++;
            if (temp_data == '[')
            {
                return READ_NEXT;
            }
            else if (temp_data == 0)
            {
                return READ_ZERO;
            }
            else
            {
                continue;
            }
        }
        else
        {
            if (FALSE == read_page_data())
            {
                return READ_FILE_END;
            }

            buf_point = lrc_temp_buf;

        }
    }

    return READ_FILE_END;
}

/******************************************************************************/
/*
 * \par  Description: ����ʱ��ķ�ֵ
 *
 * \param[in]

 * \param[out]

 * \return        �������

 * \note
 *******************************************************************************/
uint8 read_time_minute(void)
{
    uint8 temp_data;
    uint8 minute_num = 0;
    uint8 get_m_ok = FALSE;
    uint8 frist_is_zero = TRUE;
    uint8 frist_is_space = TRUE;

    while (lrc_pos_file <= lrc_file_len)
    {
        if (lrc_pos_buffer < SECTOR_SIZE)
        {
            temp_data = *buf_point;
            buf_point++;
            lrc_pos_buffer++;
            if (temp_data == ':')
            {
                return READ_NEXT;
            }
            else if (temp_data == 0)
            {
                return READ_ZERO;
            }
            else if ((temp_data == ' ') && (frist_is_space == TRUE))
            {
                frist_is_space = FALSE;
            }
            else if ((temp_data < '0') || (temp_data > '9'))
            {
                return READ_INVALID_LAB;
            }
            else
            {
                if ((temp_data == '0') && (frist_is_zero == TRUE))
                {
                    continue;
                }
                else
                {
                    frist_is_zero = FALSE;
                    frist_is_space = FALSE;
                    if (get_m_ok == FALSE)
                    {
                        if (minute_num < MINUTE_VALID) //������λ����λ
                        {
                            *(get_time_p) = *(get_time_p + 1);
                            *(get_time_p + 1) = temp_data - '0';
                        }
                        else
                        {
                            *(uint16*) get_time_p = 0x0909;//99min
                            get_m_ok = TRUE;
                        }
                        minute_num++;
                    }

                }
            }
        }
        else
        {
            if (FALSE == read_page_data())
            {
                return READ_FILE_END;
            }
            buf_point = lrc_temp_buf;
        }
    }

    return READ_FILE_END;
}

/******************************************************************************/
/*
 * \par  Description:��ʱ���ǩ���뼶�ͺ��뼶
 *
 * \param[in]

 * \param[out]

 * \return        �������

 * \note
 *******************************************************************************/
uint8 read_time_second(void)
{
    uint8 temp_data;
    uint8 char_num = 0;
    uint8 get_s_ok = FALSE;
    uint8 frist_is_zero = TRUE;
    uint8 msecond_flag = FALSE;
    uint8 get_ms_ok = FALSE;
    uint8 frist_is_space = TRUE;

    while (lrc_pos_file <= lrc_file_len)
    {
        if (lrc_pos_buffer < SECTOR_SIZE)
        {
            temp_data = *buf_point++;
            lrc_pos_buffer++;
            if (temp_data == ']')
            {
                deal_time_lab();
                if (time_lab_count < MAX_LAB_COUNT)
                {
                    return READ_NEXT;
                }
                else
                {
                    //��ǩ�ռ��ѷ���,�˳�
                    return READ_FILE_END;
                }

            }
            else if (temp_data == 0)
            {
                return READ_ZERO;
            }
            else if (temp_data == ' ' && frist_is_space)
            {
                frist_is_space = FALSE;
            }
            //            else if(temp_data<'0' || temp_data>'9')
            else if (temp_data != ':' && temp_data != '.' && (temp_data < '0' || temp_data > '9'))
            {
                return READ_INVALID_LAB;
            }
            else
            {
                if (get_ms_ok == TRUE)
                {
                    continue;
                }

                if (temp_data == '.' || temp_data == ':')
                {
                    if (FALSE == msecond_flag)
                    {
                        msecond_flag = TRUE;
                        get_s_ok = TRUE;
                        //MS ����һλΪ0,����
                        frist_is_zero = TRUE;
                        frist_is_space = TRUE;
                        char_num = 0;
                        continue;
                    }
                    else
                    {
                        return READ_INVALID_LAB;
                    }

                }

                if (temp_data == '0' && frist_is_zero)
                {
                    continue;
                }
                else
                {
                    frist_is_zero = FALSE;
                    frist_is_space = FALSE;
                    if (get_s_ok == FALSE)
                    {
                        if (char_num < SECOND_VALID) //������λ����λ
                        {
                            *(get_time_p + 2) = *(get_time_p + 3);
                            *(get_time_p + 3) = *(get_time_p + 4);
                            *(get_time_p + 4) = temp_data - '0';
                        }
                        else
                        {//999s
                            *(get_time_p + 2) = 0x09;
                            *(uint16*) (get_time_p + 3) = 0x99;
                            get_s_ok = TRUE;
                        }
                        char_num++;
                    }

                    if (msecond_flag == TRUE)
                    {

                        if (char_num < MS_VALID) //������λ����λ
                        {
                            *(get_time_p + 5) = *(get_time_p + 6);
                            *(get_time_p + 6) = *(get_time_p + 7);
                            *(get_time_p + 7) = temp_data - '0';
                        }
                        else
                        {
                            //999ms
                            *(get_time_p + 5) = 0x09; //
                            *(uint16*) (get_time_p + 6) = 0x099; //
                            get_ms_ok = TRUE;
                        }

                        char_num++;

                    }

                }
            }
        }
        else
        {
            if (FALSE == read_page_data())
            {
                return READ_FILE_END;
            }
            buf_point = lrc_temp_buf;
        }
    }

    return READ_FILE_END;
}

/******************************************************************************/
/*
 * \par  Description:���������
 *
 * \param[in]

 * \param[out]

 * \return        �������

 * \note
 *******************************************************************************/
uint8 read_string_data(void)
{
    uint8 temp_data;

    lrc_str_len = 0;

    while (lrc_pos_file <= lrc_file_len)
    {
        if (lrc_pos_buffer < SECTOR_SIZE)
        {
            temp_data = *buf_point;
            buf_point++;
            lrc_pos_buffer++;
            if (temp_data == 0)
            {
                write_lrc_len();
                return READ_ZERO;
            }
            else if ((temp_data == 0x0d) || (temp_data == 0x0a))
            {
                //�����Ƿ�֧�ָ�����ݱ��س����з��ض�
#if 0
                continue;//֧�ֱ��ض�
#else
                //��֧�ֽض�
                write_lrc_len();
                return READ_DATA_END;
#endif

            }
            else
            {
                //�Ƿ����µı�ǩ
                if (temp_data == '[')
                {
                    //if(TRUE==check_lab_avild())
                    if (lrc_pos_buffer == SECTOR_SIZE)
                    {
                        read_page_data();
                        buf_point = lrc_temp_buf;
                    }

                    if ((*buf_point >= '0') && (*buf_point <= '9'))
                    {
                        if (lrc_str_len != 0)
                        {
                            //��һ����ʵı�ǩ
                            write_lrc_len();
                        }
                        else
                        {//��ͬ��ʣ������ǩ����
                            ;//������
                        }
                        libc_memset(get_time_buf, 0, sizeof(get_time_buf));
                        return READ_LAB_START;
                    }
                }

                //����������
                *save_lrc_buf_p = temp_data;
                save_lrc_buf_p++;
                {
                    lab_lrc_offset++;
                    lrc_str_len++;
                    if ((lab_lrc_offset % SECTOR_SIZE) == 0)
                    {
                        sys_vm_write(lrc_save_buf, lrc_vram_p);
                        save_lrc_buf_p = lrc_save_buf;
                        lrc_vram_p += SECTOR_SIZE;

                        if ((uint32) lrc_vram_p >= LRC_CONTENT_MAX)
                        {
                            //VM ��Χ���
                            write_lrc_len();
                            return READ_FILE_END;
                        }
                    }
                }
            }
        }
        else
        {
            if (FALSE == read_page_data())
            {
                write_lrc_len();
                return READ_FILE_END;
            }
            buf_point = lrc_temp_buf;
        }
    }

    write_lrc_len();
    return READ_FILE_END;

}

/******************************************************************************/
/*
 * \par  Description: ����һ���Ϸ���ʱ���ǩֵ
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void deal_time_lab(void)
{
    uint8 temp;
    uint8 minute = 0;
    uint16 second = 0;
    uint16 m_second = 0;

    for (temp = 0; temp < MINUTE_VALID; temp++)
    {
        minute *= 10;
        minute += *(get_time_p + temp);
    }
    for (temp = 0; temp < SECOND_VALID; temp++)
    {
        second *= 10;
        second += *(get_time_p + 2 + temp);
    }

    for (temp = 0; temp < MS_VALID; temp++)
    {
        m_second *= 10;
        m_second += *(get_time_p + 5 + temp);
    }

    temp = (uint8) (second / 60);

    if ((temp + minute) > MAX_MINUTE)
    {
        lrc_lab_p->min = MAX_MINUTE;
        lrc_lab_p->sec = 59;
        lrc_lab_p->p_ms = 99;
    }
    else
    {
        minute += temp;
        second %= 60;
        lrc_lab_p->min = minute;
        lrc_lab_p->sec = (uint8) second;
        lrc_lab_p->p_ms = (uint8) (m_second / 10);
    }
    lrc_lab_p->offset_byte = lab_lrc_offset;
    lrc_lab_p->lrc_len = 0;
    lrc_lab_p++;
    time_lab_count++;
    same_str_count++;
}

/******************************************************************************/
/*
 * \par  Description: д���ʵĳ���
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void write_lrc_len(void)
{
    //    uint8 i;
    lrc_lable_t *temp_lab_p;

    temp_lab_p = lrc_lab_p;

    while (same_str_count > 0)
    {
        temp_lab_p--;
        temp_lab_p->lrc_len = lrc_str_len;
        same_str_count--;
    }
    return;
}

/*
 bool check_label_avail(void)
 {

 }
 */

