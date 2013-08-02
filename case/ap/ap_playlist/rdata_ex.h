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

#ifndef  _rdata_ex_h
#define  _rdata_ex_h

#include "psp_includes.h"
#include "case_include.h"

extern app_timer_t playlist_app_timer_vector[APP_TIMER_TOTAL];

extern uint8 language_id; //����id
extern uint16 ap_pos_file_offset; //�ļ������ľ���ƫ��
extern handle ap_obj_handle;
extern int32 ap_vfs_mount;
extern uint8 utf8_flag;
extern comval_t g_comval;

//*lib file's info

//head

extern uint16 ap_plist_file_total;
extern uint16 ap_plist_file_offset;

//items 108
extern uint16 ap_file_index_offset[6];
extern uint16 ap_sort_index_offset[6];
extern uint16 ap_index_parent_offset[6];
extern uint16 ap_son_tree_num[6][3];
extern uint16 ap_son_tree_offset[6][3];

// sort file
extern uint8 *track_buf; //����ʱ���TRACK_NUM��RAM��ַ
extern uint16 sort_file_total; //����ĸ���
extern uint8 *sort_depend_buf; //compare �㷨����Ƚ�ʱ��������������


//vramдָ��
extern uint8 *title_vram_pt;
extern uint8 *album_vram_pt;
extern uint8 *artist_vram_pt;
extern uint8 *genre_vram_pt;
extern uint8 *audible_vram_pt;
extern uint8 *track_vram_pt;

//ram дָ��
extern uint8 *title_data_pt;
extern uint8 *album_data_pt;
extern uint8 *artist_data_pt;
extern uint8 *genre_data_pt;
extern uint8 *audible_data_pt;
extern uint8 *fileinfo_data_pt;
extern uint8 *track_data_pt;

//ID3��Ϣ��ͬʱ�����һ��
//file_indexΪ����������ǹ�����
extern uint16 *tidy_buf_a;//��ALBUM
extern uint16 *tidy_buf_b;//��ARTIST

//���������file_index��sort_indexΪ�����������file_index��
extern uint16 *file_index_buf;//

//file_indexΪ������������λ��sort_index��
extern uint16 *sort_index_buf;

//��¼��ǰ�б������ͬ���Ӹ�������СֵΪ1���磨1��3��2��...��
extern uint16 *count_buf_a;//
extern uint16 *count_buf_b;//
extern uint16 *count_buf_c;//

//file_indexΪ��������parent_offset��
extern uint16 *index_parent_buf;
//������temp_ex_index����һ�����file��

//extern cmp_obj_f compare_obj;

extern id3_info_t id3_file_info;
extern plist_f_info_t *music_f_p;
extern plist_f_info_t *audible_f_p;
extern plist_f_info_t *other_f_p;

extern uint8 temp_plist_buf[SECTOR_SIZE];
extern uint8 file_buffer[256];
extern uint8 plist_lib_offset[PL_LIB_MAX + USERPL_MAX][8];

extern uint8 ui_precent_count; //�ٷֱȼ���
extern uint8 ui_icon_count; //��ʾͼ��
extern uint8 ui_cur_strID; //��ʾid��
//extern int8  app_timer_id;      //��ʱ��id
extern uint8 cpu_release_count; //cpu�ͷż�ʱ��

extern uint8 lib_num; //��ǰ��������
extern uint8 drv_sn; //��ǰ�洢����

extern uint8 fsel_scan_flag; //ɨ���ļ����

#define  SCAN_FILTRATE_BIT    0x01

/////////////m3u��ʽ�ļ������������ȫ�ֱ���
extern uint16 m3u_cursec_num; //��ǰ��д�ļ�������ƫ��
extern uint16 m3u_curpos_offset; //��ǰ��д�ļ����ֽ�ƫ��
extern uint32 m3u_handle; //m3u��ʽ�ļ��򿪾��

extern uint8 *m3u_get_name_buf; //dirʱ��ȡ�ļ�����ʱbuf
extern uint8 *m3u_obj_name_buf; //m3u�е��ļ�����unicode��ʽ
extern uint8 *m3u_ext_name; //m3u���ļ�������չ����unicode��ʽ
extern m3ulist_f_info_t *m3u_dat_file_pt; //�ļ���Ϣָ��
extern file_pos_t *filepos_p;            //�ļ�dir ��Ϣָ��

extern uint16 ebook_total;
extern uint16 temp_count;

#endif //#ifndef  _rdata_ex_h
