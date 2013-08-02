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
#include "plist.h"

#ifndef WIN32
OS_STK *ptos = (OS_STK *) AP_PLAYLIST_STK_POS;
#endif

INT8U prio = AP_PLAYLIST_PRIO;

uint8 language_id; //����id
uint16 ap_pos_file_offset; //�ļ������ľ���ƫ��
handle ap_obj_handle;
int32 ap_vfs_mount;
comval_t g_comval;

uint8 utf8_flag;
//*lib file's info

//head

uint16 ap_plist_file_total;
uint16 ap_plist_file_offset;

//items 108
uint16 ap_file_index_offset[6];
uint16 ap_sort_index_offset[6];
uint16 ap_index_parent_offset[6];
uint16 ap_son_tree_num[6][3];
uint16 ap_son_tree_offset[6][3];

// sort file
uint8 *track_buf; //����ʱ���TRACK_NUM��RAM��ַ,һ��TRACK_NUMռ1bytes
uint16 sort_file_total; //����ĸ���
uint8 *sort_depend_buf; //compare �㷨����Ƚ�ʱ��������������


//vramдָ��
uint8 *title_vram_pt;
uint8 *album_vram_pt;
uint8 *artist_vram_pt;
uint8 *genre_vram_pt;
uint8 *audible_vram_pt;
uint8 *track_vram_pt;

//ram дָ��
uint8 *title_data_pt;
uint8 *album_data_pt;
uint8 *artist_data_pt;
uint8 *genre_data_pt;
uint8 *audible_data_pt;
uint8 *fileinfo_data_pt;
uint8 *track_data_pt;

//ID3��Ϣ��ͬʱ�����һ��
//file_indexΪ����������ǹ�����
uint16 *tidy_buf_a;//��ALBUM
uint16 *tidy_buf_b;//��ARTIST

//���������file_index��sort_indexΪ�����������file_index��
uint16 *file_index_buf;//

//file_indexΪ������������λ��sort_index��
uint16 *sort_index_buf;

//��¼��ǰ�б������ͬ���Ӹ�������СֵΪ1���磨1��3��2��...��
uint16 *count_buf_a;//
uint16 *count_buf_b;//
uint16 *count_buf_c;//

//file_indexΪ��������parent_offset��ʹ��ǰ��ֵ���ռ临��
uint16 *index_parent_buf;
//������temp_ex_index����һ�����file��

cmp_obj_f compare_obj;

id3_info_t id3_file_info;
plist_f_info_t *music_f_p;
plist_f_info_t *audible_f_p;
plist_f_info_t *other_f_p;

uint8 temp_plist_buf[SECTOR_SIZE];
uint8 file_buffer[256];

uint8 plist_lib_offset[PL_LIB_MAX + USERPL_MAX][8];//���offsetƫ��

#ifdef  PLIST_SYS_DEF_SPACE
uint8 sys_vram_space[0x20000];//256k
uint8 sys_sdram_space[0x20000];//128k
#endif

app_timer_t playlist_app_timer_vector[APP_TIMER_TOTAL];

uint8 ui_precent_count; //�ٷֱȼ���
uint8 ui_icon_count; //��ʾͼ��
uint8 ui_cur_strID; //��ʾid��

//int8  app_timer_id;      //��ʱ��id
uint8 cpu_release_count; //cpu�ͷż�ʱ��

uint8 lib_num; //��ǰ��������
uint8 drv_sn; //��ǰ�洢����

uint8 fsel_scan_flag; //ɨ���ļ����


/////////////m3u��ʽ�ļ������������ȫ�ֱ���

uint16 m3u_cursec_num;    //��ǰ��д�ļ�������ƫ��
uint16 m3u_curpos_offset; //��ǰ��д�ļ����ֽ�ƫ��
uint32 m3u_handle;        //m3u��ʽ�ļ��򿪾��

uint8 *m3u_get_name_buf;  //dirʱ��ȡ�ļ�����ʱbuf
uint8 *m3u_obj_name_buf; //m3u�е��ļ�����unicode��ʽ
uint8 *m3u_ext_name;     //m3u���ļ�������չ����unicode��ʽ
m3ulist_f_info_t *m3u_dat_file_pt; //�ļ���Ϣָ��
file_pos_t *filepos_p;            //�ļ�dir ��Ϣָ��


uint16 ebook_total;
uint16 temp_count;

