/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo		2011-11-11         v1.0              create this file
 ********************************************************************************/
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ebook.h"

/****************************************************************************/

/* bank�� �������� */
//�ı��Ķ����ݴ洢�ͽ��뻺�����ṹ�壬��Ҫ�����Ķ�ʱʹ��
view_decode_t view_decode _BANK_DATA_ATTR_;
view_decode_t page_count_decode _BANK_DATA_ATTR_;

//��ǩ��Ϣ�洢�ṹ��
bkmark_node_t bkmarks[MAX_BKMARK_NUM]_BANK_DATA_ATTR_;
//�б����Ϣ�ṹ��
listbox_private_t bmklist_data _BANK_DATA_ATTR_;

//�洢�б���ʾ��Ϣ������
uint8 file_list[LIST_NUM_ONE_PAGE_MAX][FILENMAELEN]_BANK_DATA_ATTR_;

