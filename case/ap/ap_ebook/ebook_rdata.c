/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_rdata.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo		2011-11-11         v1.0              create this file
 ********************************************************************************/
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ebook.h"

/****************************************************************************/

/* ��פ���ݶ� �������� */

//ҳ����ʾʱ��ǰ�����ֱ�����1-û�б���;10-10��;100-100��
uint16 page_multiple_sel;
//�����Ķ��ļ�����ǩ����
uint16 bookmark_total;

//ϵͳ��������
comval_t g_comval;
//Ӧ�ô洢�����ṹ��
g_ebook_var_t g_ebook_vars;
//��ǰ�Ķ��ı�������ҳ��
uint32 curpagenum;
//�����Ķ��ļ�����ҳ��
uint32 page_totalnum;
//����Ӧ���ļ�ϵͳ��������
int32 g_ebook_mount_id;
//�򿪵���ǩ�ļ����
uint32 bmk_modify_handle;
//���ڻ��浱ǰ�����Ķ���ҳ���ڶ�Ӧ�ļ��е�ƫ��
uint32 file_offset;
//���ڼ�¼��ǰҳ���ڶ�Ӧ�ļ��е�ƫ��
uint32 decode_offset;
//���ڼ���ҳ��ʱ�������д����ǩ�ļ��е����һ��ҳ��
uint32 bmk_last_pagenum;
//��ʾ��ǰbuf�е���������������,-1��ʾ��ǰbuf�е����ݲ����á�
int32 cur_BufSector;
//��ǰ�����Ķ���ҳ���ڶ�Ӧ���ļ���������������
uint32 decode_sector;
//��ǰ�ı��ļ���Ŀ¼��Ϣ�е�Ŀ¼ƫ��
uint32 cur_file_diroffset;
//�ڼ���ҳ��ʱ���ڻ�д����ǩ�ļ��ļ���
uint16 temp_count;
//�ı��Ķ����ݴ洢�ͽ��뻺�����ṹ�壬��Ҫ�����Ķ�ʱʹ��
//view_decode_t view_decode;
vram_bmk_info_t vram_bmk_info;
decode_bmk_info_t decode_bmk_info;
view_file_t view_file;

bool numboxflag ;

//�ı��Ķ����ݴ洢�ͽ��뻺�����ṹ�壬��Ҫ����ҳ��ʹ��
view_file_t page_count_file;

app_timer_t ebook_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

//��ǩ��Ϣ����ռ�
uint8 bmk_bufFile[FILE_SECTOR];

//��ǩ�ļ�������ռ�
uint8 BOOKMARK_filename[BMK_NAME_LENGTH];
//�����Ƿ�ˢ�±�־��0-����Ҫˢ��,1-��Ҫˢ��
uint8 need_draw;
//ҳ����Ϣ�Ƿ�ˢ�±�־��0-����Ҫˢ��,1-��Ҫˢ��
uint8 draw_page_num;

#if SHOW_FIFLNAME  
    //��ʾ�ļ���                                     
    uint8 BOOK_filename[MAX_FILENAME_LENGTH];
#endif

//�Զ�����ʱ��ͳ��
uint8 auto_play_count;
//�Ƿ��������ҳ����ǡ�0-�������㣬1-�������
uint8 page_count_flag;
//ˢ�µ������Զ�����ͼ����
uint8 draw_auto_flag;
//��ǩ��ɾ�������ӱ��
uint8 bmk_modify_flag;
//�Ƿ�Ϊ�Ķ��˵����
uint8 is_reading_menu;
//��ǰ����״̬��1-flashû���ļ���2-card���ļ���3-flash��card�����ļ�
uint8 device_status;
//��̨���ֲ��ŵ��̷�
uint8 music_play_device;
//�Ƿ�֧�ֿ���
uint8 support_card_flag;
//�Ƿ���Ҫ���������б��־0-����Ҫ;1-��Ҫ
uint8 need_creatlist;

scene_enter_mode_e enter_mode;
engine_type_e engine_type;

