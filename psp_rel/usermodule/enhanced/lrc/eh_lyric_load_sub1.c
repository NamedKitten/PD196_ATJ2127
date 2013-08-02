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
 * \file     eh_lrc_load_sub1.c
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

#define LRC_EXT_BITMAP EXT_NAME_LRC

static const char offset_keyword[] =
{ 8, '[', 'O', 'F', 'F', 'S', 'E', 'T', ':' };//8��buffer�ĳ���

static const uint8 lrc_asc[] = "LRC";
static pfile_offset_t temp_pfile_offset _BANK_DATA_ATTR_;
static pdir_layer_t temp_pdir_layer _BANK_DATA_ATTR_;

//static const uint16 lrc_unicode[4]={'L','R','C','\0'};

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


//void deal_time_lab(void);
//bool add_time_offset(void);

//extern void write_lrc_len(void);
extern bool read_page_data(void);
extern char_type_e check_char_type(char data);

bool get_offset_data(void);
bool mem_search_key(uint8 *sBuff, const char *key_buf);
uint16 len_sting(uint8 *str, uint8 str_type);

/******************************************************************************/
/*
 * \par  Description: ��ָ����buffer����ָ���Ĺؼ�key
 *
 * \param[in]    sBuff��ָ��buffer
 key_buf,�ؼ�key

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool mem_search_key(uint8 *sBuff, const char *key_buf)
{

    uint8 k;
    uint16 j;
    bool result;
    j = 0;

    while (lrc_pos_file <= lrc_file_len)
    {
        result = read_page_data();//�����ݲ����Ͷ�һ��PAGE����
        if (!result)
        {
            return result;//��ʧ�ܾ��˳�
        }

        k = sBuff[lrc_pos_buffer];

        if (check_char_type((char) k) == CHAR_TYPE_DNER)
        {
            k = k - 0x20;//��Ϊ��д������
        }

        if (k != key_buf[j + 1])//��Ϊ��һ��BUFF������ĳ���
        {
            j = 0;
        }
        else
        {
            j++;
        }
        lrc_pos_buffer++;

        if (j >= key_buf[0])
        {
            //�Ƿ�ȫ��������
            return TRUE;
        }
    }
    return 0;//cancel the warmning

}

/******************************************************************************/
/*
 * \par  Description: ����ʱ�������
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool read_offset(void)
{
    bool result;

    offset_sec = 0;// 1 s
    offset_ms = 0; //10ms

    lrc_pos_buffer = SECTOR_SIZE;//Ҫ�������(������512��Ҫ������)
    lrc_pos_file = 0;

    while (lrc_pos_file <= lrc_file_len)
    {
        //ȫ����һ���ַ�һ���ַ��ؼ�⣬����Ҫ����
        result = read_page_data();//��һ��PAGE����
        if (!result)
        {
            //            vfs_file_seek(vfs_mount,0,SEEK_SET,lrc_handle);//�ص��ļ��Ŀ�ʼλ��
            return FALSE;
        }

        //�ҹؼ��� [OFFSET:
        result = mem_search_key(lrc_temp_buf, offset_keyword);//���ִ�
        if (!result)
        {
            //            vfs_file_seek(vfs_mount,0,SEEK_SET,lrc_handle);//�ص��ļ��Ŀ�ʼλ��
            return FALSE;
        }

        //�ҵ��ؼ��֣���ȡƫ��ʱ��
        result = get_offset_data();
        if (FALSE != result)
        {
            //�ɹ��ҵ�Offset��ֵ���˳�
            //            vfs_file_seek(vfs_mount,0,SEEK_SET,lrc_handle);//�ص��ļ��Ŀ�ʼλ��
            return TRUE;
        }
        //ѭ���������ǣ���ƫ��ʱ����������[OFFEST:XXX]�������������˳�
        //�����ʱ���ļ���δ���������Լ�������
    }
    return 0;//cancel the warmning
}

/******************************************************************************/
/*
 * \par  Description: �����ʱ�����ֵ
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool get_offset_data(void)
{
    uint8 result;
    uint8 i = 0;
    uint8 tmpbuff[5] =
    { 0, 0, 0, 0, 0 };

    positiveflag = TRUE;//Ĭ��Ϊ����

    if (lrc_temp_buf[lrc_pos_buffer] == '-')
    {
        positiveflag = FALSE;
        lrc_pos_buffer++;
        result = read_page_data();
        if (!result)
        {
            return FALSE;
        }
    }
    else if (lrc_temp_buf[lrc_pos_buffer] == '+')
    {
        lrc_pos_buffer++;
        result = read_page_data();
        if (!result)
        {
            return FALSE;
        }
    }
    else
    {
    }

    while (lrc_pos_file <= lrc_file_len)
    {
        if (lrc_temp_buf[lrc_pos_buffer] == ']')
        {
            //�Ѷ������ݣ���ת������������ �����ǵ�λ
            //����[OFFEST:234],ƫ��234���룬��Ϊ[2][3][4][][] ��Ӧ�Ƴ�[0][0][2][3][4]
            //������LRC�ļ�����Ҳ��ASCII�ַ�
            for (; i < 5; i++)
            {
                //����һλ,��߲�0
                tmpbuff[4] = tmpbuff[3];
                tmpbuff[3] = tmpbuff[2];
                tmpbuff[2] = tmpbuff[1];
                tmpbuff[1] = tmpbuff[0];
                tmpbuff[0] = 0;
            }

            offset_sec = tmpbuff[1] + (uint8) (tmpbuff[0] * 10);//�������
            offset_ms = tmpbuff[3] + tmpbuff[2] * 10; //��ȡ10ms��
            return TRUE;
        }

        //��ֹ  [OFFEST:2XXX]�������
        if (check_char_type((char) lrc_temp_buf[lrc_pos_buffer]) != CHAR_TYPE_NUM)
        {
            //�����������ֵľͳ�
            return FALSE;
        }

        if (i < 5)
        {
            //BUFF ֻ�����byte,�����˾Ͳ��ٲ���������ܻ�������
            tmpbuff[i] = lrc_temp_buf[lrc_pos_buffer] - '0';
            i++;
        }

        lrc_pos_buffer++;
        result = read_page_data();
        if (!result)
        {
            return FALSE; //
        }
    }
    return 0;//cancel the warmning
}

/******************************************************************************/
/*
 * \par  Description: ������ʳ�ʼ����
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void explain_lrc_init(void)
{
    vfs_file_seek(vfs_mount, 0, SEEK_SET, lrc_handle);
    lrc_pos_buffer = 0xffff;
    lrc_pos_file = 0;
    read_page_data();

    lrc_lab_p = (lrc_lable_t *) LRC_LABLE_ADDR;
    buf_point = lrc_temp_buf;
    get_time_p = get_time_buf;
    save_lrc_buf_p = lrc_save_buf;

    time_lab_count = 0;
    same_str_count = 0;
    lrc_str_len = 0;
    lab_lrc_offset = 0;
    lrc_vram_p = (uint8 *) LRC_CONTENT_VM;

    return;
}

/******************************************************************************/
/*
 * \par  Description: �����ַ����ĳ���
 *
 * \param[in]    str���ַ�������
 str_type,�ַ������ͣ�1-unicode��0-����

 * \param[out]

 * \return       �ַ��ĳ���

 * \note
 *******************************************************************************/
uint16 len_sting(uint8 *str, uint8 str_type)
{
    uint16 len = 0;

    if (str_type == 1)
    {
        while (*(uint16*) str != 0x00)
        {
            len++;
            str = str + 2;
        }
    }
    else
    {
        while (*str != 0x00)
        {
            len++;
            str++;
        }
    }

    return len;

}

/******************************************************************************/
/*
 * \par  Description:�򿪸���ļ�
 *
 * \param[in]    lrc_name������ļ���

 * \param[out]

 * \return      TRUE OR FALSE

 * \note
 *******************************************************************************/
bool open_lrc_file(uint8 *lrc_name)
{
    //    uint8 *lrc_name;
    uint8 i;
    uint16 temp;

    if (lrc_name == NULL)
    {
        return FALSE;
    }

    if (*(uint16*) lrc_name == 0xfeff)
    {//unicode
        i = 1;
    }
    else
    {
        i = 0;
    }

    temp = len_sting(lrc_name, i);
    if ((0 == temp) || ((SECTOR_SIZE / 2) < temp))
    {
        return FALSE;
    }

    //save current path
    vfs_file_dir_offset(vfs_mount, &temp_pdir_layer, &temp_pfile_offset, 0);

    if (i > 0)
    {
        //unicode
        *(uint16*) (lrc_name + (temp - 3) * 2) = 0x0000;//��MUSIC�ĺ�׺��ȥ��
        if (0 != vfs_dir(vfs_mount, DIR_HEAD, lrc_name, LRC_EXT_BITMAP))
        {
            lrc_handle = vfs_file_open(vfs_mount, NULL, R_NORMAL_SEEK);
        }
        else
        {
            lrc_handle = 0;
        }
    }
    else
    {
        temp = temp - 3;
        libc_memcpy(lrc_temp_buf, lrc_name, (uint32) temp);
        libc_memcpy(lrc_temp_buf + temp, lrc_asc, 4);
        lrc_handle = vfs_file_open(vfs_mount, lrc_temp_buf, R_NORMAL_SEEK);
    }

    //back up path
    vfs_file_dir_offset(vfs_mount, &temp_pdir_layer, &temp_pfile_offset, 1);

    if (lrc_handle == 0)
    {
        return FALSE;
    }

    lrc_file_len = 0;
    vfs_file_get_size(vfs_mount, &lrc_file_len, lrc_handle, 0);

    if (lrc_file_len < 16)
    {
        vfs_file_close(vfs_mount, lrc_handle);
        lrc_handle = 0;
        return FALSE;
    }

    return TRUE;
}

