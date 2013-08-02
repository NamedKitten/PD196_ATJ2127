/*******************************************************************************
 *                                      US212A
 *                                  Module:Ebook
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       stevenluo     2011-11-11    11:11     1.0             build this file
 *       liaojinsong   2012-08-01                           ά�����ļ�
 *******************************************************************************/
/*!
 * \file     
 * \brief    ����������á��ṹ�嶨�弰�����뺯������
 * \author   stevenluo,liaojinsong
 * \version 1.0
 * \date  2012/08/01
 *******************************************************************************/
#ifndef   _EBOOK_H
#define   _EBOOK_H

#include  "psp_includes.h"
#include  "case_include.h"
#include  "ebook_res.h"
#include  "ebook_sty.h"
#include  "mmm_id.h"

/*!���������Ӧ�õ�����ջ�����߳����ȼ�*/
#define     AP_EBOOK_STK_POS    AP_FRONT_LOW_STK_POS    //ebookջ
#define     AP_EBOOK_PRIO       AP_FRONT_LOW_PRIO       //ebook���ȼ�

#define     APP_TIMER_COUNT             1   

//����ap��ʱ������ID
#define    APP_TIMER_TAG_EBOOK          0x01            

/*!����AP VM  ������ħ��ֵ*/
#define     MAGIC_EBOOK                 0x55aa     //д����ǩ�ļ���������ҳ��ͳ�����
#define     MAGIC_COMVAL                0x55aa
/*!����AP�ı��*/
/*!����option��ڲ˵��ı��*/
#define     M_NOPLAYNOREAD_NOCARD       0       //�޲��ţ���text�Ķ����޿��������
#define     M_NOPLAYNOREAD_CARDEXIST    1       //�޲��ţ���text�Ķ����д��ڽ������
#define     M_NOPLAYREAD_NOCARD         2       //�޲��ţ���text�Ķ����޿��������
#define     M_NOPLAYREAD_CARDEXIST      3       //�޲��ţ���text�Ķ����п��������
#define     M_NOWPLAYNOREAD_NOCARD      4       //���ڲ��ţ���text�Ķ����޿��������
#define     M_NOWPLAYNOREAD_CARDEXIST   5       //���ڲ��ţ���text�Ķ����п��������
#define     M_NOWPLAYREAD_NOCARD        6       //���ڲ��ţ���text�Ķ����޿��������
#define     M_NOWPLAYREAD_CARDEXIST     7       //���ڲ��ţ���text�Ķ����п��������
#define     M_LPLAYNOREAD_NOCARD        8       //�ϴβ��ţ���text�Ķ����޿��������
#define     M_LPLAYNOREAD_CARDEXIST     9       //�ϴβ��ţ���text�Ķ����п��������
#define     M_LPLAYREAD_NOCARD          10      //�ϴβ��ţ���text�Ķ����޿��������
#define     M_LPLAYREAD_CARDEXIST       11      //�ϴβ��ţ���text�Ķ����п��������
#define     AP_MSG_RTC                  91

#define     VM_SYSTEM                   0x0000

#define     AUTOTIME_MAX        30              //�Զ����ŵ������ʱ��(s)
#define     AUTOTIME_MIN        2               //�Զ����ŵ���С���ʱ��(s)
#define     AUTOTIME_STEP       1               //�Զ����ŵļ��ʱ��ÿ������������

/* ��ǩ����(title)�ĳ��� */
#define NODELEN                 0x10
#define MAX_TITLE_LEN           12              // 12 bytes
#define MAX_BKMARK_NUM          0x10            // Max is 0x20
#define FILENMAELEN             12
#define FILESIZE                0x200           // file length
#define BUFSIZE                 0x200           // sector
#define PAGE_MAGIC              0x55aaaa55      // ����д����ǩ�ļ����Ǽ���ҳ�����

//#define FS_OPEN_NORMAL          0x01            //����seek
//#define FS_OPEN_NOSEEK          0x00            //������seek
//#define FS_OPEN_MODIFY          0x02            //֧��׷��д
#define ONE_PAGE_ROW_NUM        9               //һ����ʾ�ı���ʵ������
#define MAX_FILTER_COUNT        50              //һ�������˵��ַ���
#define TEXT_ROW_MAX_COUNT      10              //һ����ʾ�ı����������

#define PLAY_ICON_WIDTH         20              //�Զ�����ͼ��Ĵ�С(û�õ�)
#define PLAY_ICON_START         0               //�Զ�����ͼ���״̬(û�õ�)

#define REGION_HEIGHT           16              //��ʾ����ĸ߶�
#define REGION_LENGTH           128             //��ʾ����ĳ���
#define BMK_NAME_LENGTH         64              //һ�������˵��ַ���


#define MAX_DEVICE_NUM          2               //��������֧�ֵ��̷���Ŀ��Ŀǰֻ��Flash �� SD ����һ�� ����������Ϊ2��

#define UNICODE_FLAG            0xFEFF          //UNICODE ��ʽ����Ŀ�ͷ���
#define UTF8_FLAG               0xBFBBEF        //UTF8 ��ʽ����Ŀ�ͷ���

#define  SHOW_FIFLNAME          0               //��ʾ�ļ���:1Ϊ��ʾ��0Ϊ����ʾ��
                                                //������Ҫ�޸�:ONE_PAGE_ROW_NUM,
                                                //����Ҫ�޸�UI�ı���ͼƬ
#define  MAX_FILENAME_LENGTH    32              //�ļ�������󳤶�
#define  MAX_BMK_FILE           60              //д��vram������ǩ�ļ����֧�ָ���
#define  VRAM_END_ADDRESS       0x004FFFFF      //vram�������ڴ洢��ǩ��Ϣ������ַ
#define  MAX_FILE_LENGTH        0x01000000      //֧���ı��ļ����ĳ���16M

/*�������˵��ĳ���*/
typedef enum
{
    ENTER_FROM_DESKTOP = 0,     //��main ap ��browser���볡��
    ENTER_FROM_READING,         //���Ķ�������볡��
    ENTER_FROM_FILELIST,        //���ļ��б���볡��
    ENTER_FROM_SETTINGMENU,     //�����ò˵����볡��
    ENTER_FROM_READINGMENU,     //���Ķ����ò˵����볡��
    ENTER_FROM_MENUDIR,         //�Ӳ˵�Ŀ¼���볡��
    ENTER_FROM_MAX
} scene_enter_mode_e;

/*�����̨����ģʽ*/
/*!
 *  \brief
 *  Ӧ�����Ͷ���
 *  \li NOPLAY : �޲��ż�¼
 *  \li PLAYING : ���ڲ���
 *  \li LASTPLAY : ��һ�β���
 */
typedef enum
{
    NOPLAYING = 0, 
    LASTPLAY, 
    BMKMENU, 
    DESKTOP
} ebook_menu_type_e;

/*���岥�ų�����ui��Ϣ�¼�*/
/*!
 *  \brief
 *  Ӧ�����Ͷ���
 *  \li NEXTITEM : ��һ�ļ�
 *  \li PREVITEM : ��һ�ļ�
 *  \li OPTIONMENU : �˵�ѡ��
 *  \li PIC_RETURNLIST : �ļ��б�˵�
 *  \li VOLUME: ��������
 */
typedef enum
{
    EBOOK_NEXTITEM = 0,
    EBOOK_PREVITEM,
    EBOOK_OPTIONMENU,
    EBOOK_RETURNLIST,
    EBOOK_VOLUME,
    EBOOK_DEFAULT,
    EBOOK_RETURN_BROWSER
} play_result_e;

typedef enum
{
    NORMAL_TYPE,    //����ģʽ�������Ķ�����
    BMK_SEL,        //ѡ����ǩ
    BMK_DET,        //ɾ����ǩ
    PAGE_SEL        //ѡ��ҳ��
} inlet_type_e;

/*�������˵�Ӧ�õ�VMȫ�ֱ���*/
/*!
 * \brief
 *  EBOOK��VMȫ�ֱ���.
 */
typedef struct
{
    /*!VM ����ħ��*/
    uint16 magic;
    /*!����ģʽ��־��0:�ֶ�����;1:�Զ�����*/
    uint8 play_flag;
    /*!�Զ����ŵ�ʱ����:2-30��*/
    uint8 autoplaytime;
    /*!��ǰ��������ܳ���*/
    uint32 file_length;
    /*!�ļ��˳�Ӧ��ʱ��ҳ��*/
    uint32 page_num;
    /*�������ļ��Ĳ���·����Ϣ*/
    file_path_info_t path;
} g_ebook_var_t;

/* ��ǩ��Ϣ�����ݽṹ */
typedef struct
{
    UINT32 pagenum;                 // ��ǩ���ı��е�ҳ��
    uint8 title[MAX_TITLE_LEN];     // ����
} bkmark_node_t;

/* ��Ϣ����(û�õ�) */
typedef enum
{
    SINGLE, DOUBLE
} info_type_e;

/* �Ի�����Ϣ(û�õ�) */
typedef struct
{
    uint8* str_name;        // ����
    uint8 info_type;        // ��Ϣ����
    uint8 dialog_type;      // �Ի�������
    uint8 button_type;      // ��������
    uint16 option_id;       // ѡ��ID
    int8 language_id;       // ����ID
} dialog_info_t;

typedef struct
{
    /*! ��ǰ��ʾ��Ŀ�б���Ϣ */
    /*! ��Ŀ�б�ҳ�ţ�����ҳģʽ����Ч */
    uint32 group;
    /*! ��Ŀ�б���Ч����top����ţ���������б����ţ�ֵ��1��ʼ */
    uint32 top;
    /*! ��Ŀ�б���Ч����bottom����ţ���������б����ţ�ֵ��1��ʼ */
    uint32 bottom;
    /*!��Ŀ�б����Ŀ����*/
    uint32 total;

    /*! ��ǰ�����ļ���Ϣ */
    /*! ��ǰ�ļ���ţ���������б����ţ�ֵ��0��ʼ */
    uint32 list_no;
    /*! ��ǰ�б�ļ����ֵ��1 ~ one_page_count*/
    uint32 active;
    /*! ��ǰ�б�ɵļ����ֵ��1 ~ one_page_count */
    uint32 old;

    /*! �ļ��б����ģʽ���μ� list_update_mode_e ���� */
    list_update_mode_e update_mode;
    /*! �б�ؼ�����ģʽ���μ� list_draw_mode_e ���� */
    list_draw_mode_e draw_mode;
} pagenum_control_t;

/*!
 * \brief
 *      text_buf_t���ı��Ķ����뻺�����ṹ��
 */
typedef struct
{
    /*! һ���ַ��������� */
    /*! ����������Ч���ݳ��� */
    uint16 text_buf_length;
    uint8 text_prev[BUFF_ONE_ROW];
    /*! һ��������������512���ֽ� */
    uint8 text_buf[FILE_SECTOR];
    /*! ����������Ч����ָ�� */
    uint8 *text_buf_valid;

    /*! ��������ʣ���ֽ��� */
    uint8 remain;
    /*! buffer �����Ƿ���Ч */
    bool valid;
    /*! �ı���ʾ�����ṹ�� */
    text_show_param_t param;
    /*! text ��ʾ�лص����� */
    void (*text_show_line)(string_desc_t *string_desc, uint8 index);
} view_decode_t;

/*!
 * \brief
 *      text_file_t���ı��Ķ������ļ��ṹ�壬Ϊ�˼���SD�ļ����ļ�ϵͳ�ļ�
 */
typedef struct
{
    /*! �򿪵��ļ���� */
    uint32 file_handle;
    /*! �ļ��������� */
    uint32 file_sectors;
    /*! �ļ����ֽ��� */
    uint32 file_length;
    /*���ļ�ƫ�Ƶ�ַ�����ֽ�Ϊ��λ */
    uint32 file_offset;
    int8 language;
    /*! �ļ���λ�ӿں���ָ�룬������Ϊ��λ */
    bool (*fseek)(uint32 sector_offset, uint32 file_handle);
    /*! �ļ����ݶ��ӿں���ָ�룬������Ϊ��λ */
    bool (*fread)(uint8 *text_buf, uint32 file_handle);
} view_file_t;

typedef struct
{
    //�ļ��״غ�
    uint32 cluster_no;
   //�ļ��ڸô��ڵ�ƫ��
    uint32 dir_entry;
   //�洢��ǩ��Ϣ�Ŀ�ʼ��ַ
    uint32 startadd;
} vram_bmk_info_t;
typedef struct
{
    //��ǩ�ļ���
    uint8 file_name[32];
   //ҳ�������Ƿ���ɱ��
    uint32 decode_flag;
   //��ǰ�ļ���ҳ��
    uint32 totalpage;
   //��ǰ�ļ�����ҳ����ƫ��
    uint32 decode_offset;
    uint32 reserve;
} decode_bmk_info_t;

extern bool numboxflag;                         //�Ƿ�ȫˢnumbox���
extern g_ebook_var_t g_ebook_vars;              //Ӧ�ô洢�����ṹ��
extern comval_t g_comval;                       //ȫ��ϵͳ����
extern uint8 need_draw;                         //�����Ƿ�ˢ�±�־��0-����Ҫˢ��,1-��Ҫˢ��
extern uint8 auto_play_count;                   //�Զ�����ʱ��ͳ��
extern uint8 page_count_flag;                   //�Ƿ��������ҳ����ǡ�0-�������㣬1-�������
extern uint32 curpagenum;                       //��ǰ�Ķ��ı�������ҳ��
extern uint32 decode_sector;                    //��ǰ�����Ķ���ҳ���ڶ�Ӧ���ļ���������������
extern uint32 cur_file_diroffset;               //��ǰ�ı��ļ���Ŀ¼��Ϣ�е�Ŀ¼ƫ��
extern view_decode_t view_decode _BANK_DATA_ATTR_;//�ı��Ķ����뻺�����ṹ�壬��Ҫ�����Ķ�ʱʹ��
extern vram_bmk_info_t vram_bmk_info;
extern decode_bmk_info_t decode_bmk_info;
extern view_file_t view_file;

extern view_decode_t page_count_decode _BANK_DATA_ATTR_;

extern view_file_t page_count_file;
extern listbox_private_t bmklist_data _BANK_DATA_ATTR_;

//�洢�б���ʾ��Ϣ������
extern uint8 file_list[LIST_NUM_ONE_PAGE_MAX][FILENMAELEN]_BANK_DATA_ATTR_;

extern bkmark_node_t bkmarks[MAX_BKMARK_NUM] _BANK_DATA_ATTR_;
extern uint8 bmk_bufFile[FILE_SECTOR];
extern uint8 BOOKMARK_filename[BMK_NAME_LENGTH];

#if SHOW_FIFLNAME                                       //��ʾ�ļ���
    extern uint8 BOOK_filename[MAX_FILENAME_LENGTH];
#endif

extern app_timer_t ebook_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];
extern uint8 page_count_flag;                   //�Ƿ�����������ҳ����ǡ�0-�������㣬1-������
extern uint32 page_totalnum;                    //��ҳ��
extern int32 g_ebook_mount_id;                  //����Ӧ���ļ�ϵͳ��������
extern uint16 bookmark_total;
extern uint32 file_offset;
extern uint32 decode_offset;
extern int32 cur_BufSector;
extern uint16 page_multiple_sel;
extern uint16 temp_count;
extern uint8 draw_page_num;
extern uint32 bmk_modify_handle;
extern uint8 draw_auto_flag;
extern uint8 bmk_modify_flag;
extern scene_enter_mode_e enter_mode;
extern engine_type_e engine_type;
extern uint8 is_reading_menu;
extern uint8 device_status;
extern uint8 music_play_device;
extern uint32 bmk_last_pagenum;
extern uint8 support_card_flag;
extern uint8 need_creatlist;
//ebook_main.c
app_result_e _ebook_scene_msg_dispatch(private_msg_t *private_msg)__FAR__;
int _ebook_scene_dispatch(int param);
app_result_e _ebook_init(void);
void _ebook_exit(void);
void _ebook_read_var(void);
app_result_e _ebook_filesel_init(void) __FAR__;
void _ebook_param_init(void);
void _deal_exit_result(app_result_e result);
bool _deal_return_val(app_result_e retval);
void _check_play_device(void);

//ebook_menu.c
app_result_e _ebook_option_menu(scene_enter_mode_e mode);
app_result_e _menu_func_nowplaying(void *param);
app_result_e _menu_func_lastplay(void *param);
app_result_e _menu_func_deleteebook(void *param);
app_result_e _menu_func_selbookmark(void *param);
app_result_e _menu_func_detbookmark(void *param);
app_result_e _menu_func_selbookmark(void *param);
app_result_e _menu_func_addbookmark(void * param);
app_result_e _menu_func_selbookpage(void *param);
app_result_e _menu_func_carddir(void *param);
app_result_e _menu_func_flashdir(void *param);
uint16 _check_bmk_list(void);
void _close_text_handle(void);

app_result_e _menu_func_autoplay(void *param);
//app_result_e _option_callback(dialog_info_t *dialog_info) __FAR__;
bool _set_autotime_callback(uint16 value);
uint8 _filterwhitespace(uint8 *str, int8 language);
app_result_e _show_double_dialog(uint16 option_id,uint8 *str_name);

//ebook_filelist.c
app_result_e _ebook_filelist_callback(file_path_info_t *path_info, uint16 active);
app_result_e _ebook_filelist(scene_enter_mode_e mode);


//ebook_reading.c
app_result_e _ebook_reading(scene_enter_mode_e mode);
app_result_e _ebook_text_read(void);
void _display_text(void);
void _sel_next_page(uint8 page_sel);
void _sel_prev_page(uint8 page_sel);
void _deal_rtc_msg(void);
void _update_text_head(void);
void _init_decode_param(void);
void _draw_autoplya_icon(void);
#if     SHOW_FIFLNAME
void _show_filename(void);
void _get_filename(uint8 *data,uint32 datalength);
#else
#define _show_filename()
#define _get_filename(data,datalength)
#endif

//ebook_message.c
extern app_result_e _ebook_msg_callback(private_msg_t *pri_msg) __FAR__;

//ebook_bmklist.c
app_result_e _bmk_list_enter(msg_cb_func msg_callback, inlet_type_e inlet_type);
app_result_e _deal_msg_result(input_gui_msg_t *ui_msg,dir_control_t *mlst_ctl,inlet_type_e inlet_type)__FAR__;

//ebook_bookmark.c
uint16 _get_bookmark_nums(inlet_type_e inlet_type);
void _del_bookmark(uint16 id);
bool _add_bookmark(void);
uint32 _get_bmk_page(uint16 index);
uint8 _count_num_bytes(uint32 num);

//ebook_bmklist_sub.c
app_result_e _deal_confirm_item(dir_control_t *mlst_ctl, inlet_type_e inlet_type);
void _select_next_item(dir_control_t *mlst_ctl, uint8 line_cnt);
void _select_prev_item(dir_control_t *mlst_ctl, uint8 line_cnt);
void _update_delete_item(dir_control_t *mlst_ctl);
void _bmklist_init(dir_control_t *mlst_ctl, uint16 total);
void _update_list_info(dir_control_t *dir_control, listbox_private_t *dir_private, inlet_type_e inlet_type);
uint8 *_get_bmk_title(uint8 index);

//ebook_fileoperate.c
bool _seek_file_sector(uint32 sector_offset, uint32 file_handle);
bool _read_file_sector(uint8 *viewfilebuf, uint32 file_handle);
uint32 _get_file_length(uint32 file_handle);
void _close_text_file(uint8 update_flag) __FAR__;
uint16 _init_bmk_info(void);
uint16 _init_file_info(void);
bool _get_bmk_firstsec_info(void);
void _writeback_bmk_info(void);

//ebook_fileoperate2.c
bool Is_utf8_nobom(uint8 * str,uint16 len);

//ebook_bmklist_sub.c
uint8 *_get_page_title(uint16 top, uint8 index);

//ebook_decode.c
text_end_mode_e _decode_one_page(view_decode_t *text_decode, view_file_t *text_file, uint32 *page_bytes) __FAR__;
bool _read_text(view_decode_t *text_decode, view_file_t *text_file);
void _show_text_line(string_desc_t *string_infor, uint8 line);
uint8 _filter_enter_line(string_desc_t *str_desc);

//����Ĵ���δ���ڳ�פ�����

//ebook_comfunc.c
void _clear_and_set_screen(COLORREF pencolor, COLORREF backcolor)__FAR__;   //ȫ������
void _clear_and_set_text_screen(COLORREF pencolor, COLORREF backcolor)__FAR__;     //����������ʾ����
void _clear_and_set_head_screen(COLORREF pencolor, COLORREF backcolor)__FAR__;
void _show_single_dialog(uint16 option_id,uint8 diglog_type) __FAR__;
uint8 _get_bmk_name(uint16 filename_length) __FAR__;
app_result_e _deal_menu_result(void) __FAR__;


//ebook_residentcode.c
void _get_page_offset(uint32 pagenum)__FAR__;
void _count_page_num(void)__FAR__;
app_result_e _deal_key_msg(dir_control_t *mlst_ctl, inlet_type_e inlet_type) __FAR__;
bool _seek_and_read(uint32 seek_offset, uint32 read_size,uint8 *databuf) __FAR__;
bool _seek_and_write(uint32 seek_offset, uint32 write_size,uint8 *databuf) __FAR__;
bool _detect_device(uint8 device_type) __FAR__;
void _close_engine(void) __FAR__;

uint16 _vram_init_bmk_info(void);
void _vram_init_bmk_headinfo(uint8 file_index);
void _vram_wb_bmk_headinfo(void);
void _vram_write_bmkinfo(void)__FAR__;
void _vram_read_bmkinfo(void);
void _vram_write_data(uint32 address_offset);
void _vram_limit_file_length(void);


#ifdef PC
menu_cb_option _get_menu_option(uint16 str_id, uint16 str_id_sub);
menu_cb_func _get_menu_func(uint16 str_id, uint16 str_id_sub);
menu_cb_leaf _get_callback_func(uint16 str_id, uint16 str_id_sub);

#endif

#endif
