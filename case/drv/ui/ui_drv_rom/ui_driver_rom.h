/*******************************************************************************
 *                              us211A
 *                            Module: ui����
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cailizhen     2012-1-16 9:58     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_driver_rom.h
 * \brief    ui����ģ��������ݽṹ���궨�壬�ӿ������ȣ����ɸĶ���
 * \author   cailizhen
 * \version 1.0
 * \date  2012-1-16
 *******************************************************************************/

#ifndef _UI_DRIVER_ROM_H
#define _UI_DRIVER_ROM_H

#include "psp_includes.h"
#include "display.h"

#define RES_ID_BITS     0x7fff              //��ԴID�ź�12bit��ʾ��ЧID��ǰ4bit��������;
#define REAL_RES_ID(id) ((id) & RES_ID_BITS)    //��ȡ������ԴID
#define RES_TYPE_BIT    0x8000
#define RES_TYPE_COM    0x8000
#define RES_TYPE_AP     0x0000

//��ȡ��Դ�ļ������ROM����������O0�Ż�������չ����������������ֱ���ú궨��
#define res_ptr_select(res_id) ((((res_id) & RES_TYPE_BIT) == RES_TYPE_COM)? (&g_com_res): (&g_ap_res))

//�����С
#define FONT_LARGE      2
#define FONT_NORMAL     1
#define FONT_LITTER     0

//����lcd��ʾ������0��ʾBMPͼƬ��1��ʾ������䣬2��ʾ�ַ�����3��ʾ�ַ�������
#define LCD_PARAM_IMAGE     0
#define LCD_PARAM_FILL      1
#define LCD_PARAM_STRING    2
#define LCD_PARAM_SCROLL    3

//��ʾԭ���ַ�
typedef enum
{
    /*! �����ַ� */
    CHAR_EOF = 0,
    /*! Unicode�����־����0xff, 0xfe */
    CHAR_UNI_FLAG = 1,
    /*! ���з��� */
    CHAR_LR = 2,
    /*! ��ͨ�ַ� */
    CHAR_NORMAL = 3,
    /*! δ���ַ��������ݵ�ǰ�ֽ���������ȷ���Ǻ����ַ� */
    CHAR_NOPEND = 4,
} char_type_e;

//��ʾ�ַ�ģʽ
typedef enum
{
    /*! ������ʾ�ַ� */
    CHAR_DISP_NORMAL = 0,
    /*! ָ����ʼ�㣬��������ʾ����β */
    CHAR_DISP_LEFT2RIGHT = 1,
    /*! ָ����ʼ�㣬���ҵ�����ʾ����ͷ */
    CHAR_DISP_RIGHT2LEFT = 2,
} char_disp_e;

//�����ַ�����ʾ��ֹԭ�򣬷��� string_desc_t.result �з���
typedef enum
{
    /*! �ַ�����Ч�������� */
    STRING_TAIL_LEN_ZERO = 0,
    /*! �����ַ�����ֹ������δ������ */
    STRING_TAIL_EOF_NOPEND = 1,
    /*! �������з� */
    STRING_TAIL_CHAR_LR = 2,
    /*! ��ʾ����Խ�� */
    STRING_TAIL_REGION_ERR = 3,
    /*! �������� */
    STRING_TAIL_OTHER_ERR = 4,
} string_tail_e;

//�����ʾ���ݻ��������Ϊ16*160*2 = 5KB������һ���ֿռ�������ģ���󻺳���
#define DISP_MAX_WIDTH  160

//��ģ��������־��"font"�� uint32 ֵ
#define FONT_CACHE_MAGIC    0x746E6F66
#define FONT_ONE_SIZE       33//Unicode�ַ���ģ33�ֽڳ���
#define FONT_CYCLE_INDEX    4//���ѭ��������Ե�ַ
//��ģ��������ģn ��ַ
#ifndef PC
#define font_cache_char_addr(n) ((uint8 *)font_cache_addr + font_cache_header + FONT_ONE_SIZE * (n))
#else
#define font_cache_char_addr(n) (GET_REAL_ADDR(font_cache_addr) + font_cache_header + FONT_ONE_SIZE * (n))
#endif

/*!
 * \brief
 *  res_head_item_t������res_head�����ݽṹ
 */
typedef struct
{
    /*! ������ļ�ͷ��ƫ�� */
    uint32 offset;
    /*! ��Դ���� */
    uint16 count;
    /*! ��Դ���� */
    uint8 item_type;
    /*! �����ֶ� */
    uint8 reserve[9];
} res_head_item_t;

/*!
 * \brief
 *  res_head_t��res�ļ�ͷ���ݽṹ
 */
typedef struct
{
    /*! ��ʶ��'R', 'U', '2', 0x19 */
    char magic[4];
    /*! ��Դ�ĸ��� */
    uint16 counts;
    /*! �����ֶ�1 */
    uint8 reserve1[2];
    /*! sty��Ϣ��ƫ�Ƶ�ַ */
    uint32 style_addr;
    /*! �����ֶ�2 */
    uint8 reserve2[4];
    /*! ��Դͷitems */
    res_head_item_t res_items[31];
    /*! �����ֶ�3 */
    char reserve3[512];
} res_head_t; //1024 bytes


/*!
 * \brief
 *  res_entry_t����Դ���������ݽṹ
 */
typedef struct
{
    /*! ͼƬ���������ļ���ƫ��,4 bytes */
    uint32 offset;
    /*! ��Դ����, ��� 4G��4 bytes */
    uint32 length;
    /*! ��Դ����,0x01--language string ,0x02--PIC */
    uint8 type;
    /*! �����ֶ�1 */
    uint8 reserve1[1];
    /*! ����ͼƬ�������ͼƬ�������ַ����������ID���� */
    uint16 width;
    /*! ����ͼƬ�������ͼƬ���������ַ��������������Ե�ID */
    uint16 height;
    /*! �����ֶ�2 */
    uint8 reserve2[2];
} res_entry_t;//16 bytes


/*!
 * \brief
 *  res_open_infor_t����Դ�ļ��򿪺󱣴����ڴ��е���Ϣ
 */
typedef struct
{
    /*! res�ļ���� */
    sd_handle res_fp;
    /*! ͼƬ��Դ���ļ��ڵ�ƫ�� */
    uint32 pic_entry_offset;
    /*! �ַ�����Դ���ļ��ڵ�ƫ�� */
    uint32 string_entry_offset;
    /*! style�������ļ��ڵ�ƫ�� */
    uint32 cur_style_begin_offset;
    /*! picbox�������ļ��ڵ�ƫ�� */
    uint32 style_picbox_offset;
} res_open_infor_t;

/*!
 * \brief
 *  res_langid_entry_t�����������ԴID����������ݽṹ
 */
typedef struct
{
    /*! �ַ�ID�Ŷ�Ӧ�ַ����������ļ��ڵ�ƫ�� */
    uint32 offset;
    /*! �ַ������ȣ���unicode�����ַ������ֽ��� */
    uint16 length;
    /*! �����ֶ� */
    uint16 reserve;
} res_langid_entry_t; // 6B


extern res_open_infor_t g_ap_res; //��¼ap��Դ��Ϣ
extern res_open_infor_t g_com_res; //��¼common��Դ��Ϣ
extern sd_handle FontLib_fp; //�ֿ��ļ����
extern sd_handle MBtoUniTab_fp; //�������ת����ľ��

extern DC dc; //DC����
extern region_t region; //��ʾ����
extern region_t region_win;

#ifndef PC
extern uint32 display_buf_addr; //��ʾbuffer��ַ
extern uint32 font_cache_addr;//��ģ������buffer
#else
extern DLL_IMP uint32 display_buf_addr;
extern DLL_IMP uint32 font_cache_addr;
#endif
extern uint16 display_buf_len; //��ʾbuffer�ĳ���
extern uint16 font_cache_len; //��ģ��������С��Ĭ��֧��1KB
extern uint16 font_cache_header;//��ģ������ͷ����С������4�ֽڶ��룬Ĭ��Ϊ64B
extern bool font_cache_flag;//ʹ����ģ��������־

extern uint8 char_point_buffer[33]; //������ģ buffer

//ROM hook functions
void ramf_char_fill_hook(region_t *point_region, uint8 *trans_font);
bool ramf_dma_buffer_data_hook(uint32 pixel_cnt, uint16 data_width);
void ramf_read_str_bgpic_hook(uint8 *display_buffer, region_t *bgpic_region, char_fill_mode_e mode);
uint8 ramf_get_next_char_point_hook(uint8 *str, int8 language, uint8 *pbuffer);
char_type_e ramf_next_char_ansi_hook(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
char_type_e ramf_next_char_unicode_hook(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
void ramf_set_lcd_param_hook(region_t *win_region, uint8 mode);

//ROM hook functions typedef
typedef void (*__char_fill_hook)(region_t *point_region, uint8 *trans_font);
typedef bool (*__dma_buffer_data_hook)(uint32 pixel_cnt, uint16 data_width);
typedef void (*__read_str_bgpic_hook)(uint8 *display_buffer, region_t *bgpic_region, char_fill_mode_e mode);
typedef uint8 (*__get_next_char_point_hook)(uint8 *str, int8 language, uint8 *pbuffer);
typedef char_type_e (*__next_char_ansi_hook)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
typedef char_type_e (*__next_char_unicode_hook)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
typedef void (*__set_lcd_param_hook)(region_t *win_region, uint8 mode);

//ROM hook functions reference
extern __char_fill_hook char_fill_hook;
extern __dma_buffer_data_hook dma_buffer_data_hook;
extern __read_str_bgpic_hook read_str_bgpic_hook;
extern __get_next_char_point_hook get_next_char_point_hook;
extern __next_char_ansi_hook next_char_ansi_hook;
extern __next_char_unicode_hook next_char_unicode_hook;
extern __set_lcd_param_hook set_lcd_param_hook;

//ROM functions
bool romf_show_pic(uint16 id, uint16 x, uint16 y)__FAR__;
bool romf_put_sd_image(sd_handle *res_fp, uint32 img_addr, region_t *img_region)__FAR__;
bool romf_fill_rect(region_t *fill_region, uint16 color)__FAR__;
void romf_get_unicode_data(uint16 id, string_desc_t *desc)__FAR__;
void romf_black_char_to_color(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, uint8 start_col)__FAR__;
void romf_char_fill(uint8 *psoure_buf, char_disp_e char_disp, uint8 start_col)__FAR__;
void romf_read_str_bgpic(uint8 *display_buffer, region_t *bgpic_region, char_fill_mode_e mode)__FAR__;
char_type_e romf_get_next_char_type_unicode(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)__FAR__;
char_type_e romf_get_next_char_type_ansi(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)__FAR__;
uint16 romf_multi_char_to_unicode(uint16 code_rename)__FAR__;
void romf_get_font_point(uint16 code_rename, uint8 *picbuf)__FAR__;
void romf_get_next_char_point(uint16 uni_code, uint8 *pbuffer)__FAR__;
uint8 *romf_put_string_line(string_desc_t *desc, region_t *region, char_fill_mode_e mode)__FAR__;
uint16 romf_get_string_length(string_desc_t *desc, uint16 max_length, str_getlen_result_t *result)__FAR__;
bool romf_search_cache(uint16 uni_code, uint8 *cache_index)__FAR__;
void romf_get_DC_status(PDC pdc, void*null2, void *null3)__FAR__;
void romf_set_pen_color(COLORREF color, void *null2, void *null3)__FAR__;
void romf_set_backgd_color(COLORREF color, void*null2, void *null3)__FAR__;
void romf_set_text_pos(uint16 x, uint16 y)__FAR__;
void romf_get_text_pos(uint16 *x, uint16 *y)__FAR__;
bool romf_put_char(uint8 *psoure_buf, char_fill_mode_e mode, char_disp_e char_disp, uint8 start_col)__FAR__;

//ROM sub functions typedef
typedef bool (*__put_sd_image)(sd_handle *res_fp, uint32 img_addr, region_t *img_region);
typedef void (*__char_fill)(uint8 *psoure_buf, char_disp_e char_disp, uint8 start_col);
typedef void (*__black_char_to_color)(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, uint8 start_col);
typedef void (*__read_str_bgpic)(uint8 *display_buffer, region_t *bgpic_region, char_fill_mode_e mode);
typedef char_type_e (*__get_next_char_type_unicode)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
typedef char_type_e (*__get_next_char_type_ansi)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
typedef bool (*__search_cache)(uint16 uni_code, uint8 *cache_index);
typedef bool (*__put_char)(uint8 *psoure_buf, char_fill_mode_e mode, char_disp_e char_disp, uint8 start_col);

//ROM fix sub functions reference
extern __put_sd_image put_sd_image __FAR__;
extern __black_char_to_color black_char_to_color __FAR__;
extern __char_fill char_fill __FAR__;
extern __read_str_bgpic read_str_bgpic __FAR__;
extern __get_next_char_type_unicode get_next_char_type_unicode __FAR__;
extern __get_next_char_type_ansi get_next_char_type_ansi __FAR__;
extern __search_cache search_cache __FAR__;
extern __put_char put_char __FAR__;

#endif //_UI_DRIVER_ROM_H
