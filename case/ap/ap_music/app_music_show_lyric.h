/******************************************************************************
 *                               US212A
 *                            Module: SHOW_LYRIC
 *                 Copyright(c) 2003-2011 Actions Semiconductor
 *                            All Rights Reserved.
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan      2011-9-25 19:46:36          1.0              build this file
 ******************************************************************************/
/*!
 * \file     show_lyric.h
 * \brief    ��ʾ��ǩ����ģ�麯��
 * \author   wuyufan
 * \version  1.0
 * \date     2011-9-25
 *******************************************************************************/
#include "psp_includes.h"
#include "case_include.h"

/*--------------------------------------------------------*/

//����Ϊ�����ʾ�ĺ����ã�ͨ�����ò�ͬ�ĺ꣬ʵ���Ƿ�ϴʣ�
//�Ƿ���ʾ�����Ŀ���

/****Ӧ����Ҫ���õ�����************************/

#define LYRIC_MAX_SCREEN    4       //��ǰ���������༸����ʾ��1Ϊ���������ضϴ���
#define LYRIC_LINE_COUNT_ONE_PAGE 4 //һҳ��������������ʾ
#define LYRIC_MAX_WIDTH_ONE_LINE  92//һ��������ʾ������ص� 

//���������ڶϴʺͷ�������
#define NEXT_LINE    0x0a     //����
#define NEXT_SCREEN  0x00     //����
#define LRC_BUF_LEN 200

/*---------------------------------------------------------*/

//���˿�������
#define LRC_FILTER_BLANK_OPTION     (0x01 << 0)
#define LRC_FILTER_BLANK_DISABLE    (0x00 << 0)
#define LRC_FILTER_BLANK_ENABLE     (0x01 << 0)
//�ϴ�����
#define LRC_DIVIDE_WORD_OPTION      (0x01 << 1)
#define LRC_DIVIDE_WORD_DISABLE     (0x00 << 1)
#define LRC_DIVIDE_WORD_ENABLE      (0x01 << 1)
//��������
#define LRC_DIVIDE_SCREEN_OPTION    (0x01 << 2)
#define LRC_DIVIDE_SCREEN_DISABLE   (0x00 << 2)
#define LRC_DIVIDE_SCREEN_ENABLE    (0x01 << 2)

#ifndef PC
#define _LCDBUF_DATA_ATTR_    __attribute__((section(".lcdbuffer")))
#else
#define _LCDBUF_DATA_ATTR_
#endif

typedef struct
{
    /*! ��ʾһҳ���� */
    uint8 line_count_one_page;
    /*! ��ʾһ�����ص��� */
    uint8 max_width_one_line;
    /*! ��ʾģʽ���������˿��У��ִ���ʾ�� */
    uint8 mode;
    /*! �������� */
    int8 language;
} lyric_show_param_t;

typedef struct
{
    /*! ����buffer��ַ */
    uint8 *input_buffer;
    /*! ����buffer�ѽ������� */
    uint16 input_length;
    /*! ����bufferʣ�೤�� */
    uint16 input_remain;
    /*! ���buffer��ַ */
    uint8 *output_buffer;
    /*! ���buffer�ѽ������� */
    uint16 output_length;
    /*! ���������ַ */
    uint16 output_start[LYRIC_MAX_SCREEN];
    /*! ���buffer����ʱ�� */
    uint32 output_time[LYRIC_MAX_SCREEN];
    /*! �ı���ʾ�����ṹ�� */
    lyric_show_param_t param;
} lyric_decode_t;

/* ��ȡһ��ʱ���ǩ������ݻ����� */
extern uint8 lrc_buf[LRC_BUF_LEN] _LCDBUF_DATA_ATTR_;
/* һ��ʱ���ǩ�����ʾ���ݻ����� */
extern uint8 display_buf[LRC_BUF_LEN] _LCDBUF_DATA_ATTR_;
/* �����ʾ������Ʊ��� */
extern lyric_decode_t g_lyric_decode _LCDBUF_DATA_ATTR_;



extern void lyric_param_init(void);

extern void parse_lyric(lyric_decode_t *lyric_decode);

extern void display_lrc_str(uint8 *cur_lrc_str);

/*! \endcond */

