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

uint16 Read_Data_Uni(uint32 offset);

/* �ļ���ȡbuffer */
uint8 radio_buf_uni[SECTOR_SIZE] _BANK_DATA_ATTR_;

/* ��¼��ǰ��ȡλ�ã�������*/
uint16 cursec_num_uni _BANK_DATA_ATTR_;
/* ��¼��ǰ��ȡλ�ã��ֽ�ƫ��*/
uint32 cur_offset_uni _BANK_DATA_ATTR_;
/* Ƶ���ִ�����*/
uint8 freq_string_uni[FREQ_LENTH*2] _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \static uint32 atoi_uni(const char *src, int i)
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
static uint32 atoi_uni(const char *src, int i)
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
 * \void deal_station_num_uni(uint8 *ptr_num, uint8 index)
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
void deal_station_num_uni(uint8 *ptr_num, uint8 index)
{
    uint8 station_num;
    station_num = (uint8) atoi_uni(ptr_num, 2);
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
 * \void deal_user_freq_uni(char* ptr_freq, uint8 index)
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
void deal_user_freq_uni(char* ptr_freq, uint8 index)
{
    uint32 freq;
    uint16 save_freq;
    uint8 station_num;

    freq = atoi_uni(ptr_freq, FREQ_LENTH);
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
 * \void  seek_to_pos_uni(uint8 mode)
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
void seek_to_pos_uni(uint8 mode)
{
    uint32 byte_start, byte_cnt;
    uint32 temp;
    byte_start = cur_offset_uni;
    temp = byte_start;

    /* �ų����[END] ��10 �ֽ�*/
    for (byte_cnt = byte_start; byte_cnt < (file_total_byte - 10); byte_cnt = byte_cnt + 2)
    {
        if (mode == 0)
        {
            //����'=' ��λƵ���ִ�
            if (Read_Data_Uni(byte_cnt) == 0x003D)
            {
                //����'=' ����λ��Ƶ��λ��
                cur_offset_uni += 2;
                break;
            }
        }
        else
        {
            //����',' ��λ��̨�����ִ�
            if (Read_Data_Uni(byte_cnt) == 0x002C)
            {
                //����','����λ��Ƶ��λ��
                cur_offset_uni += 2;
                break;
            }
        }
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 Read_Data_Uni(uint32 offset)
 * \���б��ļ��ж�ȡָ��ƫ��λ�ô��İ���(  unicode ����)
 * \param[in]    offset  ��ȡλ��~  �ֽ�ƫ��
 * \param[out]   none
 * \return       val_ret
 * \retval
 * \retval
 * \note  �˺������޸Ķ�дλ��cur_offset
 */
/*******************************************************************************/
uint16 Read_Data_Uni(uint32 offset)
{
    uint16 val_ret;

    /*��ȡ�����ݲ��ڵ�ǰ��buff*/
    if ((uint16)(offset / 512) != cursec_num_uni)
    {
        cursec_num_uni = (uint16)(offset / 512);
        vfs_file_seek(vfs_mount_radio, cursec_num_uni * 512, SEEK_SET, fp_radio);
        /* ��ȡ���������ݵ�buf */
        vfs_file_read(vfs_mount_radio, radio_buf_uni, SECTOR_SIZE, fp_radio);
    }
    cur_offset_uni = offset;

    val_ret = *(uint16*) &radio_buf_uni[offset % 512];
    return val_ret;
}

/*********************************************************************************************/
/*!
 * \par  Description:
 * \uint8 get_need_filenum_uni(userlist_parse_e mode, uint8 num)
 * \param[in]    mode
 * \ mode = ALL_FREQ, ��ͷ��ʼ������Ч��̨����
 * \ mode = INDEX_FROM_START,��ͷ��ʼ���ҵ�����num��Ӧ��λ��(num from 0 to total-1)
 * \ mode = NEXT_FROM_CUR,�ӵ�ǰλ�ò�����һ����Ч��λ��
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/**********************************************************************************************/
uint8 get_need_filenum_uni(userlist_parse_e mode, uint8 num)
{
    uint8 file_total = 0;
    uint32 byte_cnt, byte_start;

    if (mode == NEXT_FROM_CUR)
    {
        byte_start = cur_offset_uni;
    }
    else
    {
        /* �����ļ�ͷ��unicode ��ʶ*/
        byte_start = 2;
    }
    for (byte_cnt = byte_start; byte_cnt < (file_total_byte - 2); byte_cnt = byte_cnt + 2)
    {
        //�س����к�����ŵ�̨��Ϣ
        if ((Read_Data_Uni(byte_cnt) == 0x000D) && (Read_Data_Uni(byte_cnt + 2) == 0x000A))
        {
            /* '['  END ��ʼ���ҵ������*/
            if (Read_Data_Uni(byte_cnt + 4) == 0x005B)
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
 * \void get_cur_data_uni(uint8 byte_cnt)
 * \�ӵ�ǰoffset ��ʼ����ȡָ�����ȵ�����
 * \param[in]    byte_cnt   ���ȡ���ֽڳ���
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void get_cur_data_uni(uint8 byte_cnt)
{
    uint16 value;
    uint8 i;
    uint8 temp;
    uint32 byte_start;
    byte_start = cur_offset_uni;

    libc_memset(&freq_string_uni[0], 0, FREQ_LENTH * 2);
    for (i = 0; i < byte_cnt; i = i + 2)
    {
        if (i >= (FREQ_LENTH * 2))
        {
            /* ����buffer ��Χ*/
            break;
        }
        value = Read_Data_Uni(byte_start + i);
        *(uint16*) (&freq_string_uni[i]) = value;
    }

    //����ȡ����unicode �ַ��������ֽڣ����ASCILL
    //��Ϊ�˺���ֻ����ȡ��̨�ź�Ƶ������
    for (i = 0; i < (byte_cnt / 2); i++)
    {
        temp = freq_string_uni[i * 2];
        freq_string_uni[i] = temp;
    }
    for (; i < byte_cnt; i++)
    {
        freq_string_uni[i] = 0;
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void get_all_freqinfo_uni(void)
 * \��ȡ�û���̨�б������е�Ƶ����Ϣ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void get_all_freqinfo_uni(void)
{
    uint8 cnt;

    //�û���̨�б�Ƶ����Ϣ���
    libc_memset(g_user_station.fmstation_user, 0, MAX_STATION_COUNT * 2);

    /* �û���̨�б��е�̨����*/
    g_userlist_total = get_need_filenum_uni(ALL_FREQ, 0);
    if (g_userlist_total > MAX_STATION_COUNT)
    {
        //֧��������30 ���û���̨
        g_userlist_total = MAX_STATION_COUNT;
    }

    /*��λ����һ����̨��Ϣ��ʼλ�ã�index 0*/
    get_need_filenum_uni(INDEX_FROM_START, 0);

    //ѭ����ȡ�����û���̨��Ƶ����Ϣ����g_user_station.fmstation_user[]
    //û�еĵ�̨��Ƶ��ֵ���Ϊ0
    for (cnt = 0; cnt < g_userlist_total; cnt++)
    {
        /*��ȡ8  �ֽڵ�̨��*/
        get_cur_data_uni(STANUM_LENTH * 2);
        deal_station_num_uni(&freq_string_uni[2], cnt);
        /* ��λ��Ƶ��ֵ��ʼ��*/
        seek_to_pos_uni(0);
        get_cur_data_uni(FREQ_LENTH * 2);
        /*�����û���̨Ƶ��*/
        deal_user_freq_uni(&freq_string_uni[0], cnt);
        get_need_filenum_uni(NEXT_FROM_CUR, 0); /*��λ����һ����̨��ʼλ��*/
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * void deal_station_info_uni(void)
 * \ ��ȡ�û���̨�ĵ�̨��Ϣ�����ڲ��Ž�����ʾ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note  ��ȡ��ǰ�û���̨���Ƶ�g_user_station.station_name[40]
 */
/*******************************************************************************/
void deal_station_info_uni(void)
{
    uint16 value;
    uint8 i;
    uint32 byte_start;
    byte_start = cur_offset_uni;

    /* ����û���̨����buffer */
    libc_memset(g_user_station.station_name, 0, 40);
    for (i = 0; i < 38; i = i + 2)
    {
        value = Read_Data_Uni(byte_start + i);
        if (value != 0x000D)
        {
            *(uint16*) (&g_user_station.station_name[i]) = value;
        }
        else
        {
            break;
        }
    }
    *(uint16*) (&g_user_station.station_name[i]) = 0; //������
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool parse_userlist_uni(char* station_name, userlist_parse_e mode, uint8 num)
 * \��unicode ������û���̨�б���н���
 * \param[in]    char* station_name��mode��num
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool parse_userlist_uni(char* station_name, userlist_parse_e mode, uint8 num)
{
    bool ret = TRUE;

    //��ʼ������Ϊ-1	��buffer ���ݲ���ֱ��ʹ��
    cursec_num_uni = 0xffff;
    cur_offset_uni = 0;

    //����Ƶ��Ƶ��ֵ�����������û���̨�б���ʾ
    if (mode == ALL_FREQ)
    {
        get_all_freqinfo_uni();
    }
    //���������ţ���ȡ��̨������ʾ
    //��ʱ�������ź͵�̨�ŵ�ӳ����Ѿ���������ֱ�ӻ�õ�̨��
    //���ݵ�̨�ţ��ɻ��Ƶ��ֵ
    //���ԣ��û���̨�б������л�ʱ�����ȵõ�Ƶ�㿪ʼ����
    //�ٻ�ȡ������ʾ
    else if (mode == INDEX_FROM_START)
    {
        /* ��λ������������ʼλ��CH*/
        get_need_filenum_uni(INDEX_FROM_START, num);
        /* ����ҵ�','����λ�����ƿ�ʼλ��*/
        seek_to_pos_uni(1);
        deal_station_info_uni();
    }
    //NEXT_FROM_CUR ģʽ���ڲ�ʹ��
    else
    {
        ret = FALSE;
    }
    return ret;
}

