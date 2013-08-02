/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-20 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_op_entry.c
 * \brief    ui�����ܽӿڶ���
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *              ����ui�����ӿ�����
 * \par      EXTERNALIZED FUNCTIONS:
 none
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//�̻������޸�
#if (USE_MULTI_FONTLIB != 0)
uint8 fix_char_point_buffer[MAX_CHARPOINTE_SIZE];//��ģ������
sd_file_t fix_font_files[3];//�����ֿ��ļ�����������
uint8 fix_current_font;//0��ʾС�����ֿ⣬1��ʾ�������ֿ⣬2��ʾ�������ֿ�
#endif
uint8 *fix_p_char_point;
uint8 fix_font_height;
uint8 fix_font_height_actual;
uint8 fix_font_height_line;//�и߶ȣ��м��Ϊ1/4��
uint8 fix_font_height_up_margin;
uint8 fix_font_width;
uint8 fix_font_size;

//ui�����ڲ��ַ���������
uint8 string_buf[STRING_BUFFER_LEN];

//������ر���
uint8 scroll_buf[STRING_BUFFER_LEN];
scroll_desc_t scroll_desc;
string_desc_t scroll_str;
text_background_t text_bg_desc;
bool  scroll_flag; //�ַ����Ƿ񳬳�
uint8 scrollhead_height; //�����ĸ߶�

//��������ϣ��������ر���
uint8 prev_valid_next_joining; //����arabic�ַ���д����
arabic_status_e arabic_stat;//��������ϣ�����������״̬
#ifndef PC
uint8 *arabic_string_buf;//�ÿռ䶯̬����
#else
uint8 arabic_string_buf[STRING_BUFFER_LEN];
#endif

listbox_item_t list_item; //����listbox item�Ŀؼ����ݽṹ
uint16 timebox_sty_id = 0; //������һ�ε�timebox��style id
uint16 prev_num[2] = {0, 0}; //��¼progress��һ�ν���
int16 slider_prev_offset = 0;//����parambox��slider ��һ�ο̶�ֵ
uint16 prev_pixel = 0;   //����listbox slider��һ�εĿ̶�ֵ
uint8 ratio_bits = 0;   //����ratio������λ��
uint8 num_total_bits = 0; //����numbox������λ������
uint8 numbox_disp_prev[12];//����numbox����һ����ʾ����
uint8 param_bits = 0;    //����parambox������λ������
uint8 time_bits = 0; //������һ��timebox��ʾ����������

uint8 lcd_buffer_type; //��ʾbuffer����
uint8 language_id; //��ǰ��������
bool  language_2byte;//�Ƿ�˫�ֽڣ����룩���ԣ��������պ���
bool  rotate_flag;//��Ļ��תΪ����
uint16 lang_option;//��������ѡ��

typedef void* (*ui_op_func)(void*, void*, void*);
typedef struct
{
    ui_op_func res_open;
    ui_op_func res_close;
    ui_op_func show_picbox;
    ui_op_func show_textbox;
    ui_op_func show_timebox;
    ui_op_func show_numbox;
    ui_op_func show_progressbar;
    ui_op_func load_attributebox;
    ui_op_func show_listbox;
    ui_op_func show_dialogbox;
    ui_op_func show_parambox;
    ui_op_func show_sliderbar;
    ui_op_func get_textbox_attrb;
    ui_op_func get_picbox_attrb;
    ui_op_func get_listbox_attrb;
    ui_op_func scroll_string_ext;
    ui_op_func set_language;
    ui_op_func get_DC_status;
    ui_op_func clear_screen;
    ui_op_func set_pen_color;
    ui_op_func set_backgd_color;
    ui_op_func show_string;
    ui_op_func ellipsis_to_longstr;
    ui_op_func get_string_length;
    ui_op_func get_text_line;
    ui_op_func set_display_buf;
    ui_op_func unicode_to_char;
    ui_op_func char_to_unicode;
    ui_op_func get_unicode_data;
    ui_op_func utf8_to_unicode;
    ui_op_func set_screen_direction;
    ui_op_func show_pic;
    ui_op_func read_pic_to_buffer;
    ui_op_func flush_font_buffer;
    ui_op_func set_lang_option;
} ui_driver_operations;

ui_driver_operations ui_driver_op =
{ 
    (ui_op_func) res_open, (ui_op_func) res_close, (ui_op_func) show_picbox, 
    (ui_op_func) show_textbox, (ui_op_func) show_timebox, (ui_op_func) show_numbox, 
    (ui_op_func) show_progressbar, (ui_op_func) load_attributebox, (ui_op_func) show_listbox, 
    (ui_op_func) show_dialogbox, (ui_op_func) show_parambox, (ui_op_func) show_sliderbar, 
    (ui_op_func) get_textbox_attrb, (ui_op_func) get_picbox_attrb, (ui_op_func) get_listbox_attrb, 
    (ui_op_func) scroll_string_ext, (ui_op_func) set_language, (ui_op_func) romf_get_DC_status, 
    (ui_op_func) clear_screen, (ui_op_func) romf_set_pen_color, (ui_op_func) romf_set_backgd_color, 
    (ui_op_func) show_string, (ui_op_func) ellipsis_to_longstr, (ui_op_func) romf_get_string_length, 
    (ui_op_func) get_text_line, (ui_op_func) set_display_buf, (ui_op_func) unicode_to_char, 
    (ui_op_func) char_to_unicode, (ui_op_func) romf_get_unicode_data, (ui_op_func) utf8_to_unicode, 
    (ui_op_func) set_screen_direction, (ui_op_func) romf_show_pic, (ui_op_func)read_pic_to_buffer, 
    (ui_op_func) flush_font_buffer,(ui_op_func) set_lang_option,
};
