/*******************************************************************************
 *                              US212A
 *                            Module: radio_ui
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      mikeyang    2011-9-23 11:11:27           1.0              build this file
 *******************************************************************************/

#include "app_radio.h"

extern bool parse_userlist_uni(char* station_name, userlist_parse_e mode, uint8 num);
uint8 Read_Data(uint32 offset);

/* �ļ���ȡbuffer */
uint8 radio_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;

/* ��¼��ǰ��ȡλ�ã�������*/
uint16 cursec_num _BANK_DATA_ATTR_;
/* ��¼��ǰ��ȡλ�ã��ֽ�ƫ��*/
uint32 cur_offset _BANK_DATA_ATTR_;
/* Ƶ���ִ�����*/
uint8 freq_string[FREQ_LENTH] _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \static uint32 atoi2(const char *src, int i)
 * \�ַ���ת��Ϊ����
 * \param[in]    src   �ִ�ָ��
 * \                 i       �ֽ���
 * \param[out]
 * \return    ת�����ֵ
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
static uint32 atoi(const char *src, int i)
{
    int total = 0;

    while (i != 0)
    {
        total = total * 10 + (int) (*src - '0');
        src++;
        i--;
    }

    return total;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void deal_station_num(uint8 *ptr_num, uint8 index)
 * \�����û���̨�б�ĵ�̨��
 * \param[in]    ptr_num   �û���̨���ִ�ָ��
 * \                 index       ��ǰ�����̨�����û���̨�б��ļ�������
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void deal_station_num(uint8 *ptr_num, uint8 index)
{
    uint8 station_num;
    station_num = (uint8) atoi(ptr_num, 2);
    if (index < MAX_STATION_COUNT)
    {
        //�����û���̨�б��������
        g_userlist_table[index] = station_num;
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void deal_user_freq(char* ptr_freq, uint8 index)
 * \ �����û���̨�б�Ƶ�㣬���浽���ʵ�λ��
 * \param[in]    ptr_freq  Ƶ���ִ�ָ��
 * \                 index  ��ǰ����Ƶ�����û���̨�б��ļ�������
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void deal_user_freq(char* ptr_freq, uint8 index)
{
    uint32 freq;
    uint16 save_freq;
    uint8 station_num;

    freq = atoi(ptr_freq, FREQ_LENTH);
    //ֻ����Ƶ��ֵ�ĵ�2 �ֽڣ����bit ��Ϊ1
    save_freq = (uint16)(freq & 0x0ffff);

    station_num = g_userlist_table[index]; //��̨��
    if (station_num >= 1)
    {
        //��������£���̨�Ŷ���������CH01~CH30
        g_user_station.fmstation_user[station_num - 1] = save_freq; //�����û���̨Ƶ��ֵ
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void  seek_to_pos(uint8 mode)
 * \��ָ�붨λ����Ҫ������λ��
 * \param[in]    mode==0,  ��λ����һ��Ƶ��ֵ��ʼ��
 * \                 mode==1, ��λ����һ�����ƿ�ʼ��
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void seek_to_pos(uint8 mode)
{
    uint32 byte_start, byte_cnt;
    uint32 temp;
    byte_start = cur_offset;
    temp = byte_start;

    /* �ų����[END] ��5  �ֽ�*/
    for (byte_cnt = byte_start; byte_cnt < (file_total_byte - 5); byte_cnt++)
    {
        if (mode == 0)
        {
            /* ����'=' ��λƵ���ִ�*/
            if (Read_Data(byte_cnt) == 0x3D)
            {
                /* ����'='����λ��Ƶ��λ��*/
                cur_offset++;
                break;
            }
        }
        else
        {
            /* ����',' ��λ��̨�����ִ�*/
            if (Read_Data(byte_cnt) == 0x2C)
            {
                /*  ����','����λ�����ƿ�ʼ��*/
                cur_offset++;
                break;
            }
        }
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 Read_Data(uint32 offset)
 * \���б��ļ��ж�ȡָ��ƫ��λ�ô����ֽ�( �������)
 * \param[in]    offset  ��ȡλ��~  �ֽ�ƫ��
 * \param[out]   none
 * \return       val_ret
 * \retval
 * \retval
 * \note  �˺������޸Ķ�дλ��cur_offset
 */
/*******************************************************************************/
uint8 Read_Data(uint32 offset)
{
    uint8 val_ret;

    /*��ȡ�����ݲ��ڵ�ǰ��buff*/
    if ((uint16)(offset / 512) != cursec_num)
    {
        cursec_num = (uint16)(offset / 512);
        vfs_file_seek(vfs_mount_radio, cursec_num * 512, SEEK_SET, fp_radio);
        vfs_file_read(vfs_mount_radio, radio_buf, SECTOR_SIZE, fp_radio);
    }
    cur_offset = offset;

    val_ret = radio_buf[offset % 512];
    return val_ret;
}

/*********************************************************************************************/
/*!
 * \par  Description:
 * \uint8 get_need_filenum(userlist_parse_e mode, uint8 num)
 * \param[in]    mode
 * \ mode = ALL_FREQ, ��ͷ��ʼ������Ч��̨����
 * \ mode = INDEX_FROM_START,��ͷ��ʼ���ҵ�num��Ӧ��λ��(num from 0 to total-1)
 * \ mode = NEXT_FROM_CUR,�ӵ�ǰλ�ò�����һ����Ч��λ��
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/**********************************************************************************************/
uint8 get_need_filenum(userlist_parse_e mode, uint8 num)
{
    uint8 file_total = 0;
    uint32 byte_cnt, byte_start;

    if (mode == NEXT_FROM_CUR)
    {
        byte_start = cur_offset;
    }
    else
    {
        byte_start = 0;
    }
    for (byte_cnt = byte_start; byte_cnt < (file_total_byte - 2); byte_cnt++)
    {
        //�س����к�����ŵ�̨��Ϣ
        if ((Read_Data(byte_cnt) == 0x0D) && (Read_Data(byte_cnt + 1) == 0x0A))
        {
            /* '['  END ��ʼ���ҵ������*/
            if (Read_Data(byte_cnt + 2) == 0x5B)
            {
                if (mode == ALL_FREQ)
                {
                    /* �Ѿ��ҵ��ļ�β�������Ч��̨����*/
                    return file_total;
                }
                else
                {
                    /* �ѵ��ļ�β��׼���ص�һ��*/
                    return 0xff;
                }
            }

            /*�ҵ���һ���ļ�*/
            if (mode == NEXT_FROM_CUR)
            {
                break;
            }
            else if (mode == INDEX_FROM_START)
            {
                if (file_total == num)
                {
                    break;
                }
            }
            else     
            {
            }
            file_total++;
        }
    }
    return file_total;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void get_cur_data(uint8 byte_cnt)
 * \�ӵ�ǰoffset ��ʼ����ȡָ�����ȵ�����
 * \param[in]    byte_cnt   ���ȡ���ֽڳ���
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void get_cur_data(uint8 byte_cnt)
{
    uint8 value;
    uint8 i;
    uint32 byte_start;
    byte_start = cur_offset;

    /* �����buffer */
    libc_memset(&freq_string[0], 0, FREQ_LENTH);

    for (i = 0; i < byte_cnt; i++)
    {
        if (i >= FREQ_LENTH)
        {
            /* ����buffer ��Χ*/
            break;
        }
        value = Read_Data(byte_start + i);
        freq_string[i] = value;
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void get_all_freqinfo(void)
 * \��ȡ�û���̨�б������е�Ƶ����Ϣ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void get_all_freqinfo(void)
{
    uint8 cnt;

    //�û���̨�б�Ƶ����Ϣ���
    libc_memset(g_user_station.fmstation_user, 0, MAX_STATION_COUNT * 2);

    /* �û���̨�б��е�̨����*/
    g_userlist_total = get_need_filenum(ALL_FREQ, 0);

    if (g_userlist_total > MAX_STATION_COUNT)
    {
        //֧��������30 ���û���̨
        g_userlist_total = MAX_STATION_COUNT;
    }

    /*��λ����һ����̨��Ϣ��ʼλ��*/
    get_need_filenum(INDEX_FROM_START, 0);

    //ѭ����ȡ�����û���̨��Ƶ����Ϣ����g_user_station.fmstation_user[]
    //û�еĵ�̨��Ƶ��ֵ���Ϊ0
    for (cnt = 0; cnt < g_userlist_total; cnt++)
    {
        /*��ȡ4  �ֽڵ�̨��*/
        get_cur_data(STANUM_LENTH);
        deal_station_num(&freq_string[2], cnt);
        /* ��λ��Ƶ��ֵ��ʼ��*/
        seek_to_pos(0);
        get_cur_data(FREQ_LENTH);
        /*�����û���̨Ƶ��*/
        deal_user_freq(&freq_string[0], cnt);
        get_need_filenum(NEXT_FROM_CUR, 0); /*��λ����һ����̨��ʼλ��*/
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * void deal_station_info(void)
 * \ ��ȡ�û���̨�ĵ�̨��Ϣ�����ڲ��Ž�����ʾ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void deal_station_info(void)
{
    uint8 value;
    uint8 i;
    uint32 byte_start;
    byte_start = cur_offset;

    /* ����յ�̨����buffer */
    libc_memset(g_user_station.station_name, 0, 40);

    for (i = 0; i < 39; i++)
    {
        value = Read_Data(byte_start + i);
        if (value != 0x0D)
        {
            g_user_station.station_name[i] = (char)value;
        }
        else
        {
            break;
        }
    }
    g_user_station.station_name[i] = 0; //������
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool parse_userlist_mul(char* station_name, userlist_parse_e mode, uint8 num)
 * \�Զ���������������û���̨�б���н���
 * \param[in]    char* station_name��mode��num
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool parse_userlist_mul(char* station_name, userlist_parse_e mode, uint8 num)
{
    bool ret = TRUE;

    //��ʼ������Ϊ-1	��buffer ���ݲ���ֱ��ʹ��
    cursec_num = 0xffff;
    cur_offset = 0;

    //����Ƶ��Ƶ��ֵ�����������û���̨�б���ʾ
    //����radioUI  ����һ��
    if (mode == ALL_FREQ)
    {
        get_all_freqinfo();
    }
    //���������ţ���ȡ��̨������ʾ
    //��ʱ�������ź͵�̨�ŵ�ӳ����Ѿ���������ֱ�ӻ�õ�̨��
    //���ݵ�̨�ţ��ɻ��Ƶ��ֵ
    //���ԣ��û���̨�б������л�ʱ�����ȵõ�Ƶ�㿪ʼ����
    //�ٻ�ȡ������ʾ
    else if (mode == INDEX_FROM_START)
    {
        /* ��λ������������ʼλ��CH*/
        get_need_filenum(INDEX_FROM_START, num);
        /* ����ҵ�','����λ�����ƿ�ʼλ��*/
        seek_to_pos(1);
        deal_station_info();
    }
    else
    {
        ret = FALSE;
    }
    return ret;
}

