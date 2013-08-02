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
 * \file     *.c
 * \brief    �ļ�ѡ������פ����
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"

fsel_module_e fsel_module_type; //ģ������

fsel_error_e fsel_error; //��������
uint8 dir_layer; //��ǰĿ¼���б�Ĳ�Σ�0��ʾ��Ŀ¼��1��ʾ��Ŀ¼����Ŀ¼���Դ�����
uint8 shuffle_sw; //SHUFFLE ����
uint16 file_no; //��ǰ�ļ���Ŀ¼���б��е����
uint16 file_total; //��ǰĿ¼���б�����ļ�����

uint32 cur_ext_name; //��ǰѡ���������չ��
uint16 root_dir; //��Ŀ¼����Ŀ¼ָ�룬1,2��������
uint8 obj_open_mode; //�ļ��򿪵�ģʽ,��Ϊ0xee��������VM�ľ������¼ƫ�ƣ�
uint8 obj_open_type; //�ļ��򿪵��б�����
handle obj_handle; //�����Ķ��������б��ļ�
uint32 vfs_mount; //vFS ʶ���
fsel_param_t fsel_init_val; //ģ�����

uint16 save_read_offset; //����������ַ
bool write_data_flag; //��д���ݱ��

//userlist
uint16 uspl_file_total; //�ղؼ��ļ�������
uint16 uspl_first_offset; //��һ�ļ����ƫ��(0,1,...)
uint16 uspl_last_offset; //����ļ����ƫ��(0,1,...)
uint16 uspl_file_offset; //�ļ����ƫ��(0,1,...)

//playlist
plist_item_info_t plist_item_info; //item ��Ϣ��¼
plist_type_e plist_type_selector; //��ǰ������б�����
uint16 plist_file_total; // �ļ�������
uint16 plist_info_offset; //�ļ���Ϣ��ƫ��
uint16 plist_list_layerno[4];
uint16 plist_start_sort_index; //���Ŷ��е���ʼsort_index
uint16 plist_start_file_index; //��ʼ��file_index
uint16 plist_cur_file_index; //��ǰ���Ÿ�����FILE_INDEX
uint16 plist_layer_offset; //��ǰ�����listƫ��

pfile_offset_t lib_pfile_offset; //�б�ƫ��λ��

// m3u
uint16 m3u_curlist_index;  //��ǰѡ���M3U LIST ���


