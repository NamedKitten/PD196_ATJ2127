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
 * \file     eh_rdata_ex.h
 * \brief    ���ݽṹ�ͳ�������
 * \author   liminxian
 * \version 1.0
 * \date  2011/09/15
 *******************************************************************************/

#ifndef _eh_rdata_ex_h
#define _eh_rdata_ex_h

extern fsel_module_e fsel_module_type;
extern fsel_error_e fsel_error; //��������
extern uint8 dir_layer; //��ǰĿ¼���б�Ĳ�Σ�0��ʾ��Ŀ¼��1��ʾ��Ŀ¼����Ŀ¼���Դ�����
extern uint8 shuffle_sw; //SHUFFLE ����
extern uint16 file_no; //��ǰ�ļ���Ŀ¼���б��е����
extern uint16 file_total; //��ǰĿ¼���б�����ļ�����
extern uint32 cur_ext_name; //��ǰѡ���������չ��
extern uint16 root_dir; //��Ŀ¼����Ŀ¼ָ�룬1,2��������
extern uint8 obj_open_mode; //�ļ��򿪵�ģʽ
extern uint8 obj_open_type; //�ļ��򿪵��б�����
extern handle obj_handle; //�����Ķ��������б��ļ�
extern int32 vfs_mount; //FS ʶ���
extern fsel_param_t fsel_init_val; //ģ�����
extern uint16 save_read_offset; //����������ַ
extern bool write_data_flag; //��д���ݱ��

//userlist
extern uint16 uspl_file_total; //�ղؼ��ļ�������
extern uint16 uspl_first_offset; //��һ�ļ����ƫ��(0,1,...)
extern uint16 uspl_last_offset; //����ļ����ƫ��(0,1,...)
extern uint16 uspl_file_offset; //�ļ����ƫ��(0,1,...)

//playlist
extern plist_item_info_t plist_item_info; //item ��Ϣ��¼
extern uint16 plist_start_sort_index; //���Ŷ��е���ʼsort_index
extern uint16 plist_file_total; // �ļ�������
extern uint16 plist_info_offset; //�ļ���Ϣ��ƫ��
extern uint16 plist_list_layerno[4];
extern plist_type_e plist_type_selector;
extern uint16 plist_start_file_index;
extern uint16 plist_cur_file_index; //��ǰ���Ÿ�����FILE_INDEX
extern uint16 plist_layer_offset; //��ǰ�����listƫ��

extern pfile_offset_t lib_pfile_offset; //�б�ƫ��λ��

extern uint16 m3u_curlist_index;

#endif //#ifndef _eh_rdata_ex_h
