/*******************************************************************************
 *                              us211A
 *                            Module: ui����
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liutingting     2011-9-9 16:23     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_driver.h
 * \brief    ui����ģ��������ݽṹ���궨�壬�ӿ�������
 * \author   liutinging
 * \version 1.0
 * \date  2011-9-9
 *******************************************************************************/

#ifndef _UI_DRIVER_H
#define _UI_DRIVER_H

#include "psp_includes.h"
#include "ui_driver_rom.h"
#include "display.h"

//�̻������޸�
#if (USE_MULTI_FONTLIB != 0)
extern uint8 fix_char_point_buffer[MAX_CHARPOINTE_SIZE];//��ģ������
extern sd_file_t fix_font_files[3];//�����ֿ��ļ�����������
extern uint8 fix_current_font;//0��ʾС�����ֿ⣬1��ʾ�������ֿ⣬2��ʾ�������ֿ�
#endif
extern uint8 *fix_p_char_point;
extern uint8 fix_font_height;
extern uint8 fix_font_height_actual;
extern uint8 fix_font_height_line;
extern uint8 fix_font_height_up_margin;
extern uint8 fix_font_width;
extern uint8 fix_font_size;

#ifndef PC
#define fix_font_cache_char_addr(n) ((uint8 *)font_cache_addr + font_cache_header + fix_font_size * (n))
#else
#define fix_font_cache_char_addr(n) (GET_REAL_ADDR(font_cache_addr) + font_cache_header + fix_font_size * (n))
#endif

//��ȡ����߶�
static inline uint8 get_fontlib_height(uint16 disp_mode)
{
#if (USE_MULTI_FONTLIB == 0)

    return 16;

#else

#if ((USE_MULTI_FONTLIB & 0x01) != 0)
    if((disp_mode & DISP_FONT_OPTION) == DISP_FONT_SMALL)
    {
        return FONTLIB_1_HEIGHT_ACTUAL;
    }
#endif

#if ((USE_MULTI_FONTLIB & 0x04) != 0)
    if((disp_mode & DISP_FONT_OPTION) == DISP_FONT_LARGE)
    {
        return FONTLIB_3_HEIGHT_ACTUAL;
    }
#endif

    return FONTLIB_2_HEIGHT_ACTUAL;

#endif
}

#define  UI_DEBUG //ui ��������ģʽ

/*!
 * \brief
 *  �ؼ�����Ŷ���
 */
#define  SCENE           0
#define  PICBOX          1
#define  TEXTBOX         2
#define  NUMBOX          3
#define  TIMEBOX         4
#define  PROGRESSBAR     5
#define  LISTBOX         6
#define  PARAMBOX        7
#define  DIALOGBOX       8
#define  SLIDERBAR       9
#define  ATTRIBUTEBOX    10

/*!
 * \brief
 *  transparent_pic_t ����͸��ͼ��/���ͼ������ݽṹ
 */
typedef struct
{
    /*! ͸��ͼ��ID */
    uint16 pic_id;
    /*! ͸��ͼ��X���� */
    uint16 pic_x;
    /*! ͸��ͼ��Y���� */
    uint16 pic_y;
    /*! ͸��ͼ�걳����ɫ */
    uint16 back_color;
    
    /*! ��ʵʱ������ͼID */
    uint16 back_id;
    /*! ��ʵʱ������ͼX���꣬����Ǻ���ͼ�꣬��ʾX��ʼֵ */
    uint16 back_x;
    /*! ��ʵʱ������ͼY���꣬���������ͼ�꣬��ʾY��ʼֵ����ײ�ֵ�� */
    uint16 back_y;
    
    /*! �Ƿ��������ۼ� */
    bool clear;
    /*! ����ͼ����0Ϊ����1Ϊ���� */
    uint8 direct;
} transparent_pic_t;

/*!
 * \brief
 *  picbox_t ����picbox�����ݽṹ
 */
typedef struct
{
    /*! ���id=-1����ʾ��֡pic,֡���б���frame�У�����ֵ��ʾres id */
    uint16 id;
    /*! picbox x����*/
    uint16 x;
    /*! picbox y����*/
    uint16 y;
    /*!
     * \li attrib<0>��ʾ�Ƿ���ʾpicbox��0Ϊ����ʾ��1Ϊ��ʾ;
     * \li attrib<1>��ʾframe �Ĵ洢���ͣ�0��ʾ��֡��12֡���ڣ�ͼƬ���洢��frame��
     *     1��ʾ�洢��frame[0],frame[1]ָ���ĵط�;
     * \li attrib<8-15>��ʾͼƬ��֡����
     */
    uint16 attrib;
    /*! ���attrib<1>=0����֡ʱ��֡��Ӧid�б�;
     *   ���attrib<1>=1��frame[0],frame[1]Ϊframe �Ŀ�ʼ��ַ
     */
    uint16 frame[12];
} picbox_t;

/*!
 * \brief
 *  textbox_t ����textbox�����ݽṹ
 */
typedef struct
{
    /*! �̶��ַ���res id �����Ϊ���̶� id =-1 */
    uint16 id;
    /*! ��ʾ�ַ�x���� */
    uint16 x;
    /*! ��ʾ�ַ�y���� */
    uint16 y;
    /*! ��ʾ�ַ��Ŀ�� */
    uint16 width;
    /*! ��ʾ�ַ��ĸ߶� */
    uint16 height;
    /*! ����ͼƬx���� */
    uint16 back_x;
    /*! ����ͼƬy���� */
    uint16 back_y;
    /*! ��attrib<1>=0ʱ����ʾ����ͼƬres id����ʱ����Ϊ-1����ʾû�б���ͼƬ;
     ��attrib<1>=1ʱ��backgroundID��ʾ������ɫ*/
    uint16 back_id;
    /*! ������ɫ */
    uint16 text_color;
    /*!
     * \li  attrib<0>��ʾ�ַ����Ƿ���ʾ��0��ʾ����ʾ,1��ʾ��ʾ;
     * \li  attrib<1>��ʾbackgroundID�Ǳ���ͼƬ,���Ǳ�����ɫ��0��ʾ������ɫ��1��ʾ����ͼƬ;
     * \li  attrib<2-3>��ʾ�����С��0��ʾС���壬1��ʾ�к����壬2��ʾ�����壬Ĭ��Ϊ�к�����;
     * \li  attrib<4>��ʾ�Ƿ�ȡ�����������Ҷ������ԣ�1��ʾȡ����0��ʾ��ȡ��;
     * \li  attrib<5-6>��ʾ���ַ�����ʾģʽ��0��ʾ������ʾ���Զ�������ʾ����1��ʾ������ʾ��
     *                 2��ʾ�ض���ʾ�������������ַ���3��ʾʡ�Ժ���ʾ;
     * \li  attrib<7-8>��ʾ�ַ����������ԣ�0��ʾ����룬1��ʾ���ж��룬2��ʾ���Ҷ��룬Ĭ�������;
     * \li  attrib<9-15>Ϊ�����ֶΡ�
     */
    uint16 attrib;
} textbox_t;

/*!
 * \brief
 *  numbox_t ����numbox�����ݽṹ
 */
typedef struct
{
    /*! ������ʾ����X���� */
    uint16 x;
    /*! ������ʾ����Y���� */
    uint16 y;
    /*! ������ʾ������ */
    uint16 width;
    /*! ������ʾ����߶� */
    uint16 height;
    /*! ����0��res id��0-9 ��res id ������� */
    uint16 num0_id;
    /*! �ָ���id��-1 ��ʾû�зָ��� */
    uint16 partition_id;
    /*! ����ͼƬ��� */
    uint8 pic_width;
    /*! �ָ���ͼƬ��� */
    uint8 partition_width;
    /*! ����ͼƬ������͸������ɫ */
    uint16 numpic_back_color;
    /*! ����ͼƬx���� */
    uint16 back_x;
    /*! ����ͼƬy���� */
    uint16 back_y;
    /*! ���ֺͷָ����ı���ͼƬID���߱�����ɫֵ */
    uint16 back_id;
    /*! ���ֺͷָ������ַ������Ļ�����ɫ */
    uint16 text_color;
    /*!
     * \li  position<0-7> ������ʾλ��[2]��ǰ4λ��ʾС�����ߵ�һ��
     *      ������4λ��ʾ�ڶ�����������attrib<4>=1��Ч����Ϊ0���
     *      ʾǰ��0���أ���Ϊ0���ʾ��ʾλ��;
     * \li  position<8-15> ��ʾС��λ����
     */
    uint16 position;
    /*!
     * \li  attrib<0>��ʾ�Ƿ���ʾnumberbox��1��ʾ��ʾ��0��ʾ����ʾ;
     * \li  attrib<1>��ʾ�Ƿ���ʾ+�ţ�1��ʾ��ʾ��0��ʾ����ʾ;
     * \li  attrib<2-3>��ʾ��ʾ���뷽ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ��룻3��Ч;
     * \li  attrib<4>��ʾ���ģʽ��0��ʾֻ��һ��������С����numbox��1��ʾ����������ɵ�numbox;
     * \li  attrib<5>��ʾ��ʾģʽ��0��ʾ�ַ�����ʾ���ָ�����С����Ĭ��Ϊ.����������Ĭ��Ϊ/����1��ʾ����ͼƬ��ʾ;
     * \li  attrib<6>��ʾ����ģʽ��0��ʾ������ɫ��1��ʾ����ͼƬ;
     * \li  attrib<7-8>��ʾ�����ֺţ�0��ʾС�����壬1��ʾ�к����壬2��ʾ������壬Ĭ���к�����;
     * \li  attrib<9-15> Ϊ�����ֶΡ�
     */
    uint16 attrib; //attrib<0>��ʾ�Ƿ���ʾnumberbox��1��ʾ��ʾ��0��ʾ����ʾ��
} numbox_t;

/*!
 * \brief
 *  show_number_param_t ������ͼƬ��ʽ��ʾ���ֲ�������
 */
typedef struct
{
    /*! ���ֺͷָ����ı���ͼƬID */
    uint16 back_id;
    /*! ������ʾ����X���� */
    uint16 x;
    /*! ������ʾ����Y���� */
    uint16 y;
    /*! ������ʾ������ */
    uint16 width;
    /*! ����0��res id��0-9 ��res id ������� */
    uint16 num0_id;
    /*! �ָ���id��-1 ��ʾû�зָ��� */
    uint16 partition_id;
    /*! ����ͼƬ������͸������ɫ */
    uint16 numpic_back_color;
    /*! ����ͼƬ��� */
    uint8 num_width;
    /*! �ָ���ͼƬ��� */
    uint8 partition_width;
    /*! ���뷽ʽ��0:left, 1:center, 2:right */
    uint8 align;
    /*! �ָ���λ�� */
    uint8 part_pos;
} show_number_param_t;

/*!
 * \brief
 *  timebox_t ����timebox�����ݽṹ
 */
typedef struct
{
    /*! ʱ���֡��롢�����Ӧ��X���� */
    uint16 x;
    /*! ʱ���֡��롢�����Ӧ��Y���� */
    uint16 y;
    /*! ʱ����ʾ������ */
    uint16 width;
    /*! ʱ����ʾ����߶� */
    uint16 height;
    /*! ʱ���֡��롢�����Ӧ������ 0 ��id�ţ�Ҫ��0-9��res id���� */
    uint16 num0_id;
    /*! ʱ���֡��롢����ķָ���resID��-1��ʾû�� */
    uint16 part_id[4];
    /*! �ָ����Ŀ�� */
    uint8 part_width;
    /*! ʱ���֡��롢������ԴͼƬ�Ŀ��*/
    uint8 pic_width;
    /*! �ֱ��ǵ�һ��time����һ��time��ڶ���time���ڶ���time�ķָ�������ID�ţ�ID��ЧֵΪ0~3 */
    uint8 part_type[3];
    /*! �����ֽ� */
    uint8 reserve;
    /*! ����ͼƬ������͸������ɫ */
    uint16 numpic_back_color;
    /*! ����ͼƬx���� */
    uint16 back_x;
    /*! ����ͼƬy���� */
    uint16 back_y;
    /*! ���ֺͷָ����ı���ͼƬID���߱�����ɫֵ */
    uint16 back_id;
    /*! ���ֺͷָ������ַ������Ļ�����ɫ */
    uint16 text_color;
    /*!
     * \li  attrib<0> ��ʾ�Ƿ���ʾTimeBox��0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<1> ��ʾ��ʾģʽ��0��ʾ�ַ�����ʾ��1��ʾ����ͼƬ��ʾ;
     * \li  attrib<2-3>��ʾ��ʾ�������ԣ�0��ʾ����룬1��ʾ���ж��룬2��ʾ���Ҷ���;
     * \li  attrib<4> ��ʾtime��Ŀ��0��ʾ1����1��ʾ����;
     * \li  attrib<5> ��һ��time���λ0�Զ����أ�0Ϊ���أ�1Ϊ��ʾ;
     * \li  attrib<6> �ڶ���time���λ0�Զ����أ�0Ϊ���أ�1Ϊ��ʾ;
     * \li  attrib<7-9>��һ��time��һ��������ʾ��λ��;
     * \li  attrib<10-12>��һ��time��һ��������ʾ��λ��;
     * \li  attrib<13> ��ʾ����ģʽ��0��ʾ������ɫ��1��ʾ����ͼƬ;
     * \li  attrib<14-15> ��ʾ�����ֺţ�0��ʾС�ţ�1��ʾ�кţ�2��ʾ��ţ�Ĭ��Ϊ�к�����;
     */
    uint16 attrib;
} timebox_t;

/*!
 * \brief
 *  progressbar_t ����progressbar�����ݽṹ
 */
typedef struct
{
    /*! ����������ͼƬ��res id */
    uint16 back_id;
    /*! ����������ͼƬ��X���� */
    uint16 back_x;
    /*! ����������ͼƬ��Y���� */
    uint16 back_y;
    /*! �ؼ�����ͼƬ��������������α� */
    uint16 widget_bg;
    /*! �������ͼƬ������Ϊ1�����ص��res id����������Ϊ2��4��8��16��32���ȵ������� */
    uint16 fill_id;
    /*! ���Ȳ���ͼƬ������Ϊ1�����ص��res id����������Ϊ2��4��8��16��32���ȵ������� */
    uint16 unfill_id;
    /*! �����������Ч���������X���� */
    uint16 fill_x;
    /*! �����������Ч���������Y���� */
    uint16 fill_y;
    /*! �α�ͼƬID */
    uint16 tag_id;
    /*! ����α���α걳��ͼƬID */
    uint16 tag_clear_id;
    /*! �����������Ч���α�ͼƬ��X���� */
    uint16 tag_x;
    /*! �����������Ч���α�ͼƬ��Y���� */
    uint16 tag_y;
    /*! �����������Ч������������ˢ����ʼ��X���� */
    uint16 prog_start_x;
    /*! �����������Ч������������ˢ����ʼ��Y���� */
    uint16 prog_start_y;
    /*! �������ĳ��� */
    uint16 length;
    /*! ��������ÿ���ȣ������� */
    uint16 scale;
    /*!
     * \li  attrib<0> ��ʾ�Ƿ���ʾProgressBar��0 ��ʾ����ʾ��1��ʾ��ʾ;
     * \li  attrib<1> ��ʾ����0��ʾ����1��ʾ����;
     * \li  attrib<2-3> ��ʾ�Ƿ���������0��ʾ��������䣻1��ʾ���α��ʾ��2��ʾ���߶�Ҫ;
     * \li  attrib<4-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
    /*! �����ֽ� */
    uint16 reserve;
} progressbar_t;

/*!
 * \brief
 *  listbox_t ����listbox�����ݽṹ
 */
typedef struct
{
    /*! ListBox����ṹ��ĵ�ַ�������sty�ű��ļ�ͷ��ƫ�� */
    uint32 title_addr;
    /*! ListBox�������ṹ���ַ�������sty�ű��ļ�ͷ��ƫ�� */
    uint32 slider_addr;
    /*! ListBox�б���˵���ṹ���ַ�������sty�ű��ļ�ͷ��ƫ�� */
    uint32 item_addr;
    /*! ListBox����/�����ṹ���ַ�������sty�ű��ļ�ͷ��ƫ�� */
    uint32 ratio_addr;
    /*! ListBox����ͼƬres_id */
    uint16 back_id;
    /*! ListBox����ͼƬ��ʾX���� */
    uint16 back_x;
    /*! ListBox����ͼƬ��ʾY���� */
    uint16 back_y;
    /*! item��ļ�� */
    uint8 inc_y;
    /*! item��һ������ʾ���� */
    uint8 num_one_page;
    /*!
     * \li  attrib<0>��ʾ�Ƿ���ʾ���⣬0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<1>��ʾ�Ƿ���ʾ���⣬0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<2>��ʾ�Ƿ���ʾ�Ҳ໬������0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<3>��ʾ�Ƿ���ʾ���/��������0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<4-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
    /*! �����ֶ� */
    uint16 reserve;
} listbox_t;

/*!
 * \brief
 *  listbox_title_t ����listbox��title���ݽṹ
 */
typedef struct
{
    /*! ������ʾ����X���� */
    uint16 str_x;
    /*! ������ʾ����Y���� */
    uint16 str_y;
    /*! ������ʾ������ */
    uint16 str_width;
    /*! ������ʾ����߶� */
    uint16 str_height;
    /*! �����ַ�����ʾ��ɫ */
    uint16 str_color;
    /*!
     * \li   attrib<0-1>�ַ����������ԣ�0��ʾ����룬1��ʾ���ж��룬2��ʾ���Ҷ��룬Ĭ�Ͼ��ж���;
     * \li   attrib<2-3> ��ʾ�����ֺţ�0��ʾС�ţ�1��ʾ�кţ�2��ʾ��ţ�Ĭ��Ϊ�к�����;
     * \li   attrib<4-15> Ϊ�����ֶ�
     */
    uint16 attrib;
} listbox_title_t;

/*!
 * \brief
 *  listbox_slider_t ����listbox��slider���ݽṹ
 */
typedef struct
{
    /*! ����������ͼƬres_id */
    uint16 slider_back_id;
    /*! ����������ͼƬX���� */
    uint16 slider_back_x;
    /*! ����������ͼƬY���� */
    uint16 slider_back_y;
    /*! ������ͼ��res_id */
    uint16 slider_point_id;
    /*! �����㱳��ͼ��res_id */
    uint16 slider_clear_id;
    /*! ������ͼ��X���� */
    uint16 slider_point_x;
    /*! ������ͼ��Y���� */
    uint16 slider_point_y;
    /*! ���������ȣ�Ҫ��sliderbackID��Ӧ��ͼƬ�ĸ߶ȵ������ֵ
     *   height(slider_back_id) - height(slider_point_id) - (slider_point_y - slider_back_y) * 2
     */
    uint16 slider_length;
} listbox_slider_t;

/*!
 * \brief
 *  listbox_item_t ����listbox��item���ݽṹ
 */
typedef struct
{
    /*! �б���������ͼ�꣬����Ϊ back_x,back_y */
    uint16 list_back_id;
    /*! �б���ǻ��ǰ��ͼ�꣬res id�������multyģʽ��Ҫ���ͼ��ID���� */
    uint16 head_id;
    /*! �б�����ǰ��ͼ�꣬res id�������multyģʽ��Ҫ���ͼ��ID���� */
    uint16 head_act_id;
    /*! �б����һ��ǰ��ͼ��X���� */
    uint16 head_x;
    /*! �б����һ��ǰ��ͼ��Y���� */
    uint16 head_y;
    /*! ����Ǵ�ɫ�����Ļ�����ô��ֵ��ʾ������ɫֵ������Ǳ���ͼƬģʽ����ô��ʾ�б���
     *   �ǻ���ͼƬres id�������multyģʽ��Ҫ����б����ͼ��ID����
     */
    uint16 back_id;
    /*! ����Ǵ�ɫ�����Ļ�����ô��ֵ��ʾ������ɫֵ������Ǳ���ͼƬģʽ����ô��ʾ�б���
     *   ����ͼƬres id�������multyģʽ��Ҫ����б����ͼ��ID����
     */
    uint16 back_act_id;
    /*! �б����һ���ͼƬX���� */
    uint16 back_x;
    /*! �б����һ���ͼƬY���� */
    uint16 back_y;
    /*! �б���ǻ�����ͼ�꣬res id�������multyģʽ��Ҫ���ͼ��ID���� */
    uint16 tail_id;
    /*! �б�����ѡ�У������ͼ�꣬res id�������multyģʽ��Ҫ���ͼ��ID���� */
    uint16 tail_act_id;
    /*! �б����һ�����ͼ��X���� */
    uint16 tail_x;
    /*! �б����һ�����ͼ��Y���� */
    uint16 tail_y;
    /*! �б����һ����ʾ����X���� */
    uint16 data_x;
    /*! �б����һ����ʾ����Y���� */
    uint16 data_y;
    /*! �б�����ʾ������ */
    uint16 data_width;
    /*! �б�����ʾ����߶�*/
    uint16 data_height;
    /*! �Ǽ�������ʾ������ɫ*/
    uint16 data_color;
    /*! ��������ʾ������ɫ*/
    uint16 data_act_color;
    /*!
     * \li  attrib<0-1>��ʾѡ����Ǽ���ʱ������ͼ����ʾ��ʽ��0��ʾ����ʾ��1��ʾ��ʾ��single����2��ʾ��ʾ��multy��;
     * \li  attrib<2-3>��ʾѡ�������ʱ������ͼ����ʾ��ʽ��0��ʾ����ʾ��1��ʾ��ʾ��single����2��ʾ��ʾ��multy��;
     * \li  attrib<4-5>��ʾǰ��ͼ�꣨�Ǽ���ʱ����ʾ��ʽ��0��ʾ����ʾ��1��ʾ��ʾ��single����2��ʾ��ʾ��multy����3��ʾ
     *      ��ʾ����ͼ�꣬headID[0]�д洢��������0 ��res id�����ַ�ʽֻ�������б�����������10�����;
     * \li  attrib<6-7>��ʾǰ��ͼ�꣨����ʱ����ʾ��ʽ��0��ʾ����ʾ��1��ʾ��ʾ��single����2��ʾ��ʾ��multy����3��ʾ
     *      ��ʾ����ͼ�꣬headID[0]�д洢��������0 ��res id�����ַ�ʽֻ�������б�����������10�����;
     * \li  attrib<8-9>��ʾ�Ǽ��������ʾģʽ��0��ʾ������ɫ��1��ʾ����ͼƬ��single����2��ʾ����ͼƬ��multy��;
     * \li  attrib<10-11>��ʾ���������ʾģʽ��0��ʾ������ɫ��1��ʾ����ͼƬ��single����2��ʾ����ͼƬ��multy��
     * \li  attrib<12-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
    /*!
     * \li  str_attrib<0-1>��ʾ����б����ַ����ֺţ�0��ʾС�ֺţ�1��ʾ���ֺţ�2��ʾ���ֺţ�Ĭ��Ϊ���ֺ�;
     * \li  str_attrib<2-3>��ʾ����ַ���������ʾ���뷽ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���;
     * \li  str_attrib<4-5>��ʾ����б����ַ����ĳ��ַ�����ʾ���ԣ�0��������ʾ��1��������ʾ��
     *                     2���ض�����ʾ��3��ʡ�Ժ���ʾ��Ĭ��Ϊ������ʾ;
     * \li  str_attrib<6-7>��ʾ�ǻ���б����ַ����ֺţ�0��ʾС�ֺţ�1��ʾ���ֺţ�2��ʾ���ֺţ�Ĭ��Ϊ���ֺ�;
     * \li  str_attrib<8-9>��ʾ�ǻ���ַ���������ʾ���뷽ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���;
     * \li  str_attrib<10-11>��ʾ�ǻ���б����ַ����ĳ��ַ�����ʾ���ԣ�0��������ʾ��1��������ʾ;
     *      2���ض�����ʾ��3��ʡ�Ժ���ʾ��Ĭ��Ϊʡ�Ժ���ʾ;
     * \li  str_attrib<12-15> Ϊ�����ֶΡ�
     */
    uint16 str_attrib;
    /*! �����ֶ� */
    uint16 reserve;
} listbox_item_t;

/*!
 * \brief
 *  listbox_ratio_t ����listbox��ratio���ݽṹ
 */
typedef struct
{
    /*! ����0��res id ��0-9 ��res id ������� */
    uint16 num0_id;
    /*! �ָ���ID */
    uint16 partition_id;
    /*! ������ԴͼƬ�Ŀ�� */
    uint8 num_width;
    /*! �ָ���ͼƬ�Ŀ�� */
    uint8 partition_width;
    /*! ����ͼƬ͸������������ɫ */
    /*uint16 numpic_back_color;*/
    /*! ���λ�� */
    uint16 ratio_index_cnt;
    /*! ����λ�� */
    uint16 ratio_total_cnt;
    /*! ��ʾ��ʼλ��X���� */
    uint16 ratio_x;
    /*! ��ʾ��ʼλ��Y���� */
    uint16 ratio_y;
    /*! ��ʾ�����ܿ�� */
    uint16 ratio_width;
    /*! ��ʾ�����ܸ߶� */
    uint16 ratio_height;
    /*! ����ͼƬx���� */
    uint16 back_x;
    /*! ����ͼƬy���� */
    uint16 back_y;
    /*! ���ֺͷָ����ı���ͼƬID���߱�����ɫֵ */
    uint16 back_id;
    /*! ���ֺͷָ������ַ������Ļ�����ɫ */
    uint16 text_color;
    /*!
     * \li  attrib<0>��ʾ��ʾģʽ��0��ʾ�ַ�����ʾ���ָ���Ĭ��Ϊ/����1��ʾ����ͼƬ��ʾ;
     * \li  attrib<1-2>��ʾ��ʾ���뷽ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���;
     * \li  attrib<3>��ʾ�Ƿ���Ų���λ��ʱ�Ƿ�ǰ�油0��1��ʾ�ǣ�0��ʾ��;
     * \li  attrib<4>��ʾ�Ƿ�����������λ��ʱ�Ƿ�ǰ�油0��1��ʾ�ǣ�0��ʾ��;
     * \li  attrib<5>��ʾ����ģʽ��0��ʾ������ɫ��1��ʾ����ͼƬ;
     * \li  attrib<6-7>��ʾ�����ֺţ�0��ʾС�����壬1��ʾ�к����壬2��ʾ������壬Ĭ��Ϊ�к�����;
     * \li  attrib<8-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
    /*! �����ֶ� */
    /*uint16 reserve;*/
} listbox_ratio_t;

/*!
 * \brief
 *  parambox_t ����parambox�����ݽṹ
 */
typedef struct
{
    /*! ParamBox����ṹ��Ĵ洢��ַ */
    uint32 title_addr;
    /*! ParamBox������ʾ���Ĵ洢��ַ */
    uint32 num_addr;
    /*! ParamBox��λ�洢��ַ */
    uint32 unit_addr;
    /*! ParamBox����ͼƬ��res id */
    uint16 back_id;
    /*! ParamBox����ͼƬ��X���� */
    uint16 back_x;
    /*! ParamBox����ͼƬ��Y���� */
    uint16 back_y;
    /*! ParamBox��־ͼ��res id */
    uint16 icon_id;
    /*! ParamBox��־ͼ��X���� */
    uint16 icon_x;
    /*! ParamBox��־ͼ��Y���� */
    uint16 icon_y;
    /*! ParamBox������Ŀ������UI Editor��ʾ�Լ���֤������Ŀ�Ƿ�ƥ�� */
    uint16 param_cnt;
    /*! ��������������X���� */
    uint16 list_x[3];
    /*! ��������������Y���� */
    uint16 list_y[3];
    /*! �������������򣩿�� */
    uint16 list_width[3];
    /*! �������������򣩸߶� */
    uint16 list_height[3];
    /*! �����ָ���ID�ţ�-1��ʾ����ʾ */
    uint16 param_part_id[2];
    /*! �����ָ���X���� */
    uint16 param_part_x[2];
    /*! �����ָ���Y���� */
    uint16 param_part_y[2];
    /*!
     * \li  attrib<0> �Ƿ���ʾParamBox��0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<1> ����title�� 0Ϊ�ޣ�1Ϊ��;
     * \li  attrib<2> �Ƿ���ʾ���֣�0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<3> �Ƿ���ʾ��λ��0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<4> �����б����з���0��ʾ����1��ʾ����;
     * \li  attrib<5> �Ƿ���ʾ����ͼ�꣬0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<6-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
} parambox_t;

/*!
 * \brief
 *  parambox_title_t ����parambox��title���ݽṹ
 */
typedef struct
{
    /*! ����������ԴID��Ĭ����ʾΪ"��������" */
    uint16 title_id;
    /*! ����������ʾλ��x���� */
    uint16 title_x;
    /*! ����������ʾλ��y���� */
    uint16 title_y;
    /*! ����������ʾ������ */
    uint16 title_width;
    /*! ����������ʾ����߶� */
    uint16 title_height;
    /*! ������ʾ��ɫ */
    uint16 color;
    /*!
     * \li  attrib<0-1>��ʾ��ʾ���뷽ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���;
     * \li  attrib<2-3> ��ʾ�����ֺţ�0��ʾС�ţ�1��ʾ�кţ�2��ʾ��ţ�Ĭ��Ϊ�к�����;
     * \li  attrib<4-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
    /*! �����ֶ� */
    uint16 reserve;
} parambox_title_t;

/*!
 * \brief
 *  parambox_num_t ����parambox��num���ݽṹ
 */
typedef struct
{
    /*! �Ǽ������ֵ����ͼƬ���߱�����ɫ�������ͼƬ��multy��ģʽ��Ҫ�������ͼID���� */
    uint16 back_id;
    /*! �������ֵ����ͼƬ���߱�����ɫ�������ͼƬ��multy��ģʽ��Ҫ�������ͼID���� */
    uint16 back_act_id;
    /*! �������������ʱ��������������Y���꣬Y����ͬ�������򣻵��������� */
    uint16 num_x;
    /*! �������������ʱ��������������Y���꣬X����ͬ�������򣻵��������� */
    uint16 num_y;
    /*! �������������ʱ��������������߶ȣ����ͬ�������򣻵��������� */
    uint16 num_height;
    /*! �������������ʱ���������������ȣ��߶�ͬ�������򣻵��������� */
    uint16 num_width;
    /*! ����0ͼƬ��ԴID��Ҫ��0-9��+��-���� ��ͼƬ��ԴID������*/
    uint16 pic_id;
    /*! ����ͼƬ��� */
    uint16 pic_width;
    /*! �Ǽ��������ֵ�ַ�����ɫ */
    uint16 num_color;
    /*! ���������ֵ�ַ�����ɫ */
    uint16 num_act_color;
    /*!
     * \li  attrib<0> �Ƿ���ʾ����ǰ���+�ţ�0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<1> �Ƿ���ʾǰ��0������λ������ʱ�Ƿ���0���䣬1Ϊ�ǣ�0Ϊ��;
     * \li  attrib<2> �Ƿ����ֵ��ʾģʽ��0��ʾ�ַ�����ʾ��1��ʾ����ͼƬ��ʾ;
     * \li  attrib<3-4>��ʾ�Ǽ������ģʽ��0��ʾ������ɫ��1��ʾ����ͼƬ��single����2��ʾ����ͼƬ��multy��;
     * \li  attrib<5-6>��ʾ�������ģʽ��0��ʾ������ɫ��1��ʾ����ͼƬ��single����2��ʾ����ͼƬ��multy��;
     * \li  attrib<7-8>��ʾ��ʾ���뷽ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ��룬Ĭ��Ϊ����;
     * \li  attrib<9-10> ��ʾ���������ֺţ�0��ʾС�ţ�1��ʾ�кţ�2��ʾ��ţ�Ĭ��Ϊ�к�����;
     * \li  attrib<11-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
    /*! �����ֽ� */
    uint16 reserve;
} parambox_num_t;

/*!
 * \brief
 *  parambox_unit_t ����parambox��unit���ݽṹ
 */
typedef struct
{
    /*! �������������ʱ��������λ�ַ���Y���ꣻ���������� */
    uint16 unit_x;
    /*! �������������ʱ��������λ�ַ���Y���ꣻ���������� */
    uint16 unit_y;
    /*! �������������ʱ��������λ�ַ����߶ȣ���ȵ������������ȣ����������� */
    uint16 unit_height;
    /*! �������������ʱ��������λ�ַ�����ȣ��߶ȵ�����������߶ȣ����������� */
    uint16 unit_width;
    /*! ��λ�ַ�����ʾ��ɫ */
    uint16 uint_color;
    /*!
     * \li  attrib<0-1>��ʾ��ʾ���뷽ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ��룬Ĭ��Ϊ����;
     * \li  attrib<2-3> ��ʾ��λ�����ֺţ�0��ʾС�ţ�1��ʾ�кţ�2��ʾ��ţ�Ĭ��Ϊ�к�����;
     * \li  attrib<4-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
} parambox_unit_t;

/*!
 * \brief
 *  dialogbox_t ����dialogbox�����ݽṹ
 */
typedef struct
{
    /*! DialogBoxѡ���б�Ĵ洢��ַ */
    uint32 item_addr;
    /*! DialogBox����ͼƬ��res id */
    uint16 back_id;
    /*! DialogBox����ͼƬ��X���� */
    uint16 back_x;
    /*! DialogBox����ͼƬ��Y���� */
    uint16 back_y;
    /*! DialogBox����ͼƬ��res id */
    uint16 icon_id;
    /*! DialogBox����ͼƬ��X���� */
    uint16 icon_x;
    /*! DialogBox����ͼƬ��Y���� */
    uint16 icon_y;
    /*! DialogBox˵���ַ�����X���� */
    uint16 desc_x;
    /*! DialogBox˵���ַ�����Y���� */
    uint16 desc_y;
    /*! DialogBox˵���ַ����Ŀ�� */
    uint16 desc_width;
    /*! DialogBox˵���ַ����ĸ߶� */
    uint16 desc_height;
    /*! DialogBox˵���ַ�����������ɫ*/
    uint16 desc_color;
    /*! DialogBox��ť��Ŀ������UI Editor��ʾ���Լ���֤��ť��Ŀ�Ƿ�ƥ�� */
    uint16 item_cnt;
    /*!
     * \li  attrib<0> �Ƿ���ʾDialogBox��0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<1> ����;
     * \li  attrib<2-3>��ʾ˵���ַ������뷽ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ��룬Ĭ��Ϊ���ж���;
     * \li  attrib<4-5> ��ʾ�����ֺţ�0��ʾС�ţ�1��ʾ�кţ�2��ʾ��ţ�Ĭ��Ϊ�к�����;
     * \li  attrib<6> �Ƿ���ʾѡ���б�0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<7> �Ƿ���ʾ icon ͼ��
     * \li  attrib<8-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
    /*! �����ֶ� */
    uint16 reserve;
} dialogbox_t;

/*!
 * \brief
 *  dialogbox_item_t ����dialogbox_item�����ݽṹ
 */
typedef struct
{
    /*! ѡ���б�����X���� */
    uint16 item_x[3];
    /*! ѡ���б�����Y���� */
    uint16 item_y[3];
    /*! ѡ���б������� */
    uint16 item_width[3];
    /*! ѡ���б�����߶� */
    uint16 item_height[3];
    /*! ��ѡ����ı���ͼƬ����multyģʽ��Ҫ�󱳾�ͼƬID�����������߱�����ɫ������ѡ���������찴ťЧ����*/
    uint16 backgd_id;
    /*! ��ѡ�����������ɫ */
    uint16 text_color;
    /*! ѡ����ı���ͼƬ����multyģʽ��Ҫ�󱳾�ͼƬID�����������߱�����ɫ������ѡ���������찴ťЧ����*/
    uint16 act_backgd_id;
    /*! ѡ�����������ɫ */
    uint16 act_text_color;
    /*!
     * \li  attrib<0-1>��ʾ��ѡ�������ʾģʽ��0��ʾ������ɫ��1��ʾ����ͼƬ��single����2��ʾ����ͼƬ��multy��;
     * \li  attrib<2-3>��ʾѡ�������ʾģʽ��0��ʾ������ɫ��1��ʾ����ͼƬ��single����2��ʾ����ͼƬ��multy��;
     * \li  attrib<4-5>��ʾitem�ַ������뷽ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ��룬Ĭ��Ϊ���ж���;
     * \li  attrib<6-7> ��ʾ�����ֺţ�0��ʾС�ţ�1��ʾ�кţ�2��ʾ��ţ�Ĭ��Ϊ�к�����;
     * \li  attrib<8-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
    /*! �����ֶ� */
    uint16 reserve;
} dialogbox_item_t;

/*!
 * \brief
 *  sliderbar_t ����sliderbar�����ݽṹ
 */
typedef struct
{
    /*! sliderbar����ṹ��Ĵ洢��ַ */
    uint32 title_addr;
    /*! sliderbar������ʾ���Ĵ洢��ַ */
    uint32 num_addr;
    /*! sliderbar��λ�洢��ַ */
    uint32 unit_addr;
    /*! sliderbar���������Ĵ洢��ַ */
    uint32 slider_addr;
    /*! sliderbar����ͼƬ��res id */
    uint16 back_id;
    /*! sliderbar����ͼƬ��X���� */
    uint16 back_x;
    /*! sliderbar����ͼƬ��Y���� */
    uint16 back_y;
    /*! sliderbar��־ͼ��res id */
    uint16 icon_id;
    /*! sliderbar��־ͼ��X���� */
    uint16 icon_x;
    /*! sliderbar��־ͼ��Y���� */
    uint16 icon_y;
    /*! sliderbar������Ŀ������UI Editor��ʾ�Լ���֤������Ŀ�Ƿ�ƥ�� */
    uint16 param_cnt;
    /*! ��������������X���� */
    uint16 list_x;
    /*! ��������������Y���� */
    uint16 list_y;
    /*! �������������򣩿�� */
    uint16 list_width;
    /*! �������������򣩸߶� */
    uint16 list_height;
    /*! ����ͼƬ��������ʾ�Գƽ��������м�ָ��ߵ�ͼ�� */
    uint16 addition_pic;
    /*! ����ͼƬ X ���� */
    uint16 addition_x;
    /*! ����ͼƬ Y ���� */
    uint16 addition_y;
    /*!
     * \li  attrib<0> �Ƿ���ʾsliderbar��0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<1> ����title�� 0Ϊ�ޣ�1Ϊ��;
     * \li  attrib<2> �Ƿ���ʾ���֣�0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<3> �Ƿ���ʾ��λ��0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<4> �Ƿ���ʾ��������0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<5> �����б����з���0��ʾ����1��ʾ����;
     * \li  attrib<6> �Ƿ���ʾ����ͼ�꣬0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<7-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
    /*! �����ֽ� */
    uint16 reserve;
} sliderbar_t;

/*!
 * \brief
 *  sliderbar_title_t ����sliderbar��title���ݽṹ
 */
typedef struct
{
    /*! ����������ԴID��Ĭ����ʾΪ"��������" */
    uint16 title_id;
    /*! ����������ʾλ��x���� */
    uint16 title_x;
    /*! ����������ʾλ��y���� */
    uint16 title_y;
    /*! ����������ʾ������ */
    uint16 title_width;
    /*! ����������ʾ����߶� */
    uint16 title_height;
    /*! ������ʾ��ɫ */
    uint16 color;
    /*!
     * \li  attrib<0-1>��ʾ��ʾ���뷽ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���;
     * \li  attrib<2-3> ��ʾ�����ֺţ�0��ʾС�ţ�1��ʾ�кţ�2��ʾ��ţ�Ĭ��Ϊ�к�����;
     * \li  attrib<4-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
    /*! �����ֶ� */
    uint16 reserve;
} sliderbar_title_t;

/*!
 * \brief
 *  sliderbar_num_t ����sliderbar��num���ݽṹ
 */
typedef struct
{
    /*! �����ı���ͼƬY���꣬X�������list_w��ͼƬ��Ⱦ�����ʾ��ʽ������ */
    uint16 back_y;
    /*! �Ǽ������ֵ����ͼƬ���߱�����ɫ�������ͼƬ��multy��ģʽ��Ҫ�������ͼID���� */
    uint16 back_id;
    /*! �������ֵ����ͼƬ���߱�����ɫ�������ͼƬ��multy��ģʽ��Ҫ�������ͼID���� */
    uint16 back_act_id;
    /*! �������������ʱ��������������Y���꣬Y����ͬ�������򣻵��������� */
    uint16 num_x;
    /*! �������������ʱ��������������Y���꣬X����ͬ�������򣻵��������� */
    uint16 num_y;
    /*! �������������ʱ��������������߶ȣ����ͬ�������򣻵��������� */
    uint16 num_height;
    /*! �������������ʱ���������������ȣ��߶�ͬ�������򣻵��������� */
    uint16 num_width;
    /*! ����0ͼƬ��ԴID��Ҫ��0-9��+��-���� ��ͼƬ��ԴID������*/
    uint16 pic_id;
    /*! ����ͼƬ��� */
    uint16 pic_width;
    /*! �Ǽ��������ֵ�ַ�����ɫ */
    uint16 num_color;
    /*! ���������ֵ�ַ�����ɫ */
    uint16 num_act_color;
    /*!
     * \li  attrib<0> �Ƿ���ʾ����ǰ���+�ţ�0Ϊ����ʾ��1Ϊ��ʾ;
     * \li  attrib<1> �Ƿ���ʾǰ��0������λ������ʱ�Ƿ���0���䣬1Ϊ�ǣ�0Ϊ��;
     * \li  attrib<2> �Ƿ����ֵ��ʾģʽ��0��ʾ�ַ�����ʾ��1��ʾ����ͼƬ��ʾ;
     * \li  attrib<3-4>��ʾ�Ǽ������ģʽ��0��ʾ������ɫ��1��ʾ����ͼƬ��single����2��ʾ����ͼƬ��multy��;
     * \li  attrib<5-6>��ʾ�������ģʽ��0��ʾ������ɫ��1��ʾ����ͼƬ��single����2��ʾ����ͼƬ��multy��;
     * \li  attrib<7-8>��ʾ��ʾ���뷽ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ��룬Ĭ��Ϊ����;
     * \li  attrib<9-10> ��ʾ���������ֺţ�0��ʾС�ţ�1��ʾ�кţ�2��ʾ��ţ�Ĭ��Ϊ�к�����;
     * \li  attrib<11-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
} sliderbar_num_t;

/*!
 * \brief
 *  sliderbar_unit_t ����sliderbar��unit���ݽṹ
 */
typedef struct
{
    /*! �������������ʱ��������λ�ַ���Y���ꣻ���������� */
    uint16 unit_x;
    /*! �������������ʱ��������λ�ַ���Y���ꣻ���������� */
    uint16 unit_y;
    /*! �������������ʱ��������λ�ַ����߶ȣ���ȵ������������ȣ����������� */
    uint16 unit_height;
    /*! �������������ʱ��������λ�ַ�����ȣ��߶ȵ�����������߶ȣ����������� */
    uint16 unit_width;
    /*! ��λ�ַ�����ʾ��ɫ */
    uint16 uint_color;
    /*!
     * \li  attrib<0-1>��ʾ��ʾ���뷽ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ��룬Ĭ��Ϊ����;
     * \li  attrib<2-3> ��ʾ��λ�����ֺţ�0��ʾС�ţ�1��ʾ�кţ�2��ʾ��ţ�Ĭ��Ϊ�к�����;
     * \li  attrib<4-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
} sliderbar_unit_t;

/*!
 * \brief
 *  sliderbar_slider_t ����sliderbar��slider���ݽṹ
 */
typedef struct
{
    /*! ����������ͼƬID�����Ϊmulty��Ҫ�󱳾�ͼƬID���� */
    uint16 slider_back_id;
    /*! ����������ͼƬID�����Ϊmulty��Ҫ�󱳾�ͼƬID���������ֻ��һ��������������Ϊ-1 */
    uint16 slider_back_act_id;
    /*! ����������ʱ������������ͼƬX���ꣻ����������ʱ���ɽ��������м������� */
    uint16 slider_back_x;
    /*! ����������ʱ������������ͼƬY���ꣻ����������ʱ���ɽ��������м������� */
    uint16 slider_back_y;
    /*! ���������ͼ�꣬����Ϊ1�����ص��res id��
     *  ���ֻ��һ������������û�п̶ȣ�����Ϊ-1�����ֻ��һ�����������п̶ȣ���Ҫ��һ�����ص��ȵ�ͼ������Ϊ�̶�
     */
    uint16 slider_fill_id;
    /*! ����������ͼ�꣬����Ϊ1�����ص��res id */
    uint16 slider_fill_act_id;
    /*! �������������ͼ�꣬����Ϊ1�����ص��res id */
    uint16 slider_unfill_id;
    /*! ����������ʱ��Ч���������ͼ�� ��ʼX���� */
    uint16 slider_fill_x;
    /*! ����������ʱ��Ч���������ͼ�� ��ʼY���� */
    uint16  slider_fill_y;
    /*! �������α�ͼƬ��res id */
    uint16 prog_id;
    /*! �뻬��������һ���Ļ������α�ͼƬ��res id */
    uint16 unprog_id;
    /*! �������α�ͼƬ��ʼX���� */
    /*! ����������ʱ��Ч���������α�ͼƬ ��ʼX���� */
    uint16  prog_x;
    /*! ����������ʱ��Ч���������α�ͼƬ ��ʼY���꣨λ�ڽ���������ײ��� */
    uint16  prog_y;
    /*! ����������ʱ��Ч��������������ʼY���� */
    uint16  slider_start_y;
    /*! ����������ʱ��Ч��������������ʼ X���� */
    uint16  slider_start_x;
    /*!  �������ɻ�����Χ����λΪ���ص㣬���㹫ʽΪ��
     * \li  ���α������������� = slider_back_length - slider_prog_length - (slider_back_y_end - slider_start_y) * 2;
     * \li  ���α������������� = slider_back_length - (slider_back_y_end - slider_start_y) * 2;
     * \li  ���α������������ = slider_back_length - slider_prog_length - (slider_start_x - slider_back_x) * 2;
     * \li  ���α������������ = slider_back_length - (slider_start_x - slider_back_x) * 2;
     */
    uint16 length;
    /*! �̶ȣ���ʾÿ���̶ȵ��ڶ������ص��ȣ�Ĭ��Ϊ1����ʱ��֧������ֵ�� */
    uint16 scale;
    /*!
     * \li  attrib<0> ��ʾ����0Ϊ����1Ϊ����;
     * \li  attrib<1-2> ��ʾ���0��ʾ��������䣻1��ʾ���α��ʾ��2��ʾ���߶�Ҫ;
     * \li  attrib<3> ��ʾ�Ƿ�̶Ƚ�������0��ʾ��1��ʾ�ǣ���������£����ʱҪע�Ᵽ���̶�;
     * \li  attrib<4> ��ʾ�Ƿ�Գ�����ֵģʽ�����������м�̶�Ϊ0������Ϊ��������Ϊ�������罫�����������ϲ�Ϊһ����
     *      0��ʾһ��ģʽ��1��ʾ����ֵģʽ����ʱlengthӦȡһ�룩;
     * \li  attrib<5>��ʾ�Ǽ����������ͼƬģʽ��0��ʾsingle��1��ʾmulty;
     * \li  attrib<6>��ʾ�����������ͼƬģʽ��0��ʾsingle��1��ʾmulty;
     * \li  attrib<7-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
} sliderbar_slider_t;

/*!
 * \brief
 *  style_t ����style�����ݽṹ
 */
typedef struct
{
    /*! ��ǰ�����sty�ļ��еĴ洢��ʼ��ַ */
    uint32 style_begin_addr;
    /*! LCD��Ļ�ĸ߶� */
    uint16 lcd_height;
    /*! LCD��Ļ�Ŀ�� */
    uint16 lcd_width;
} style_t;

/*!
 * \brief
 *  scene_t ����scene�����ݽṹ
 */
typedef struct
{
    /*! ����ͼƬ res id�����߱�����ɫֵ */
    uint16 back_id;
    /*! ��������X���� */
    uint16 x;
    /*! ��������Y���� */
    uint16 y;
    /*! ���������� */
    uint16 width;
    /*! ��������߶� */
    uint16 height;
    /*! �����пؼ�����Ŀ */
    uint16 ctrl_count;
    /*! �ؼ������б�Ŀ�ʼλ�� */
    uint32 ctrl_list_addr;
    /*!
     * \li  atrrib<0>��ʾ����ģʽ��0Ϊ����ͼ��1Ϊ������ɫ;
     * \li  attrib<1-15> Ϊ�����ֶΡ�
     */
    uint16 attrib;
    /*! �����ֶ� */
    uint16 reserve;
} scene_t;


/*!
 * \brief
 *  scroll_desc_t ����dialogbox_item�����ݽṹ
 */
typedef struct
{
    /*! �����ַ����׵�ַ */
    uint8 *scrollhead_pt;
    /*! �����ַ����׵�ַ */
    uint8 *strhead_pt;
    /*! �������� */
    region_t scroll_region; //��������
    /*! �����Ŀ��*/
    uint8 scroll_width; //�����Ŀ��
    /*! �������� */
    int8 language;
    /*! �����ַ�����ʾģʽ */
    uint16 disp_mode;
    /*! �����ַ�������ͼƬid */
    uint16 back_id;
    /*! �����ַ�������ͼƬx���� */
    uint16 back_x;
    /*! �����ַ�������ͼƬy���� */
    uint16 back_y;
    /*! �����ַ���������ɫ */
    uint16 pen_color;
} scroll_desc_t;

/*!
 * \brief
 *      arabic_status_e ���������ϣ�����������״̬ö������
 */
typedef enum
{
    /*! ��ʼ״̬���� string_desc_t ���� */
    ARABIC_INIT       = 0,
    /*! UNICODE ��д״̬ */
    ARABIC_UNI_JOIN   = 1,
    /*! ���ķ�ת״̬ */
    ARABIC_ASCII_TURN = 2,
    /*! �м��������з�ת״̬ */
    ARABIC_LINE_TURN  = 3
} arabic_status_e;

/*!
 * \brief
 *      text_background_t �ַ������������ṹ��
 */
typedef struct
{
    /*!�ַ�������ͼƬid */
    uint16 back_id;
    /*!�ַ�������ͼƬx���� */
    uint16 back_x;
    /*!�ַ�������ͼƬy���� */
    uint16 back_y;
    /*!�ַ�������ͼƬ���*/
    uint16 width;
    /*!�ַ�������ͼƬ�߶� */
    uint16 height;
    /*!�������ȴ洢ͼƬ������ʼλ��*/
    uint32 offset_col;
} text_background_t;

/*!
 * \brief
 *      fill_dump_direction_e ��䷽��ö������
 */
typedef enum
{
    /*!��������� */
    DUMP_LEFT2RIGHT = 0,
    /*!���ҵ������ */
    DUMP_RIGHT2LEFT = 1,
    /*!���ϵ������ */
    DUMP_UP2DOWN = 2,
    /*!���µ������ */
    DUMP_DOWN2UP = 3,
} fill_dump_direction_e;

/*! ��Դͷ�ֽ��� */
#define  RESHEADITEM     16

/*! Unicode�ַ���Ĭ�ϸ߶� */
#define UNICODE_HEIGHT_DEF     16
/*! Unicode�ַ���Ĭ�Ͽ�� */
#define UNICODE_WIDTH_DEF      16

/*! �����ַ���ASCII��ķֽ�ֵ */
#define  ASCIIDISCODE          0x80

/*!
 * \brief
 *    3DЧ��
 */
#define  D3_NORMAL_DEF         0 //ֻ�ӱ߿�
#define  D3_KEYUP_DEF          1 //ͻ��Ч��
#define  D3_KEYDOWN_DEF        2 //����Ч��
/*!
 * \brief
 *      3Dͼ��Ч����ɫ
 */
#define  FaceLightColor_def  Color_tab0202_def   //���ߵ���ɫ
#define  BackLightColor_def  Color_tab0102_def   //����ߵ���ɫ
#define  SelectedColor_def   Color_BLACK_def     //��ѡ��ͼƬ�ı߿���ɫ

#define  ROW_PRIORITY_DEF    0 //������
#define  COL_PRIORITY_DEF    1 //������

//�ַ������������ȣ�ͳһΪ124�ֽڣ����60���ַ���UNICODE����£�����ǰ�ñ�־�ͽ�������
#define  STRING_BUFFER_LEN   124

//ui�����ڲ��ַ���������
extern uint8 string_buf[STRING_BUFFER_LEN];

//������ر���
extern uint8 scroll_buf[STRING_BUFFER_LEN];
extern scroll_desc_t scroll_desc;
extern string_desc_t scroll_str;
extern text_background_t text_bg_desc;
extern bool  scroll_flag;//�ַ����Ƿ񳬳�
extern uint8 scrollhead_height; //�����߶�

//��������ϣ��������ر���
extern uint8 prev_valid_next_joining; //����arabic�ַ���д����
extern arabic_status_e arabic_stat;//��������ϣ�����������״̬
#ifndef PC
extern uint8 *arabic_string_buf;
#else
extern uint8 arabic_string_buf[STRING_BUFFER_LEN];
#endif

extern listbox_item_t list_item; //����listbox item�Ŀؼ����ݽṹ
extern uint16 timebox_sty_id;
extern uint16 prev_num[2]; //��¼progress��һ�ν���
extern int16 slider_prev_offset;//����parambox��slider ��һ�ο̶�ֵ
extern uint16 prev_pixel;   //����listbox slider��һ�εĿ̶�ֵ
extern uint8 ratio_bits;   //����ratio������λ��
extern uint8 num_total_bits; //����numbox������λ������
extern uint8 numbox_disp_prev[12];//����numbox����һ����ʾ����
extern uint8 param_bits;    //����parambox������λ������
extern uint8 time_bits; //������һ��timebox��ʾ����������

extern uint8 lcd_buffer_type; //buffer����
extern uint8 language_id; //��ǰ��������
extern bool  language_2byte;//�Ƿ�˫�ֽڣ����룩���ԣ��������պ���
extern bool  rotate_flag;//��Ļ��תΪ����
extern uint16 lang_option;//��������ѡ��

//�Ƿ���Ҫ���а�������ϣ�������⴦��
static inline bool need_arabic_deal(uint16 disp_mode)
{
    if (((language_id == ARABIC) || (language_id == HEBREW)) && ((disp_mode & DISP_ARABIC_DISABLE) == 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//�Ƿ����������ַ�
static inline bool is_arabic_char(uint16 ch)
{
    if (((ch >= 0x600) && (ch <= 0x6ff)) || ((ch >= 0x750) && (ch <= 0x77f)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//�Ƿ�ϣ���������ַ�
static inline bool is_hebrew_char(uint16 ch)
{
    if ((ch >= 0x590) && (ch <= 0x5ff))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//��ȡ�ؼ����ļ��е���ʼ��ַ
static inline void get_ctrl_addr(uint8 ctrl_type, uint32 *ctrl_begin_addr, res_open_infor_t *style_ptr)
{
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, (uint32) (style_ptr->cur_style_begin_offset + ctrl_type * 8));
    sys_sd_fread(style_ptr->res_fp, ctrl_begin_addr, sizeof(uint32));
}
//����Դ�ļ�
bool res_open(char *filename, res_type_e type, void *null3);
//�ر���Դ�ļ�
void res_close(res_type_e type, void *null2, void *null3);
//music ID3����ר�ú���
bool scroll_string_ext(scroll_content_t *scroll_string_infor, scroll_param_t *scroll_param, void *null3);
//��ʾpicbox
ui_result_e show_picbox(style_infor_t *picbox_style,picbox_private_t *picbox_data, void *null3)__FAR__;
//��ʾtextbox
ui_result_e show_textbox(style_infor_t *textbox_style,textbox_private_t *textbox_data,textbox_draw_mode_e mode)__FAR__;
//��ʾtimebox
ui_result_e show_timebox(style_infor_t *timebox_style, timebox_private_t *timebox_data, timebox_draw_mode_e mode);
//��ʾnumbox
ui_result_e show_numbox(style_infor_t *numbox_style, numbox_private_t *numbox_data, numbox_draw_mode_e mode);
//��ʾprogressbar
ui_result_e show_progressbar(style_infor_t *progressbar_style, progressbar_private_t *progressbar_data,
        progress_draw_mode_e mode);
//load attributebox
ui_result_e load_attributebox(style_infor_t *attributebox_style, void*attributebox_data, uint16 count);
//��ʾlistbox
ui_result_e show_listbox(style_infor_t *listbox_style, listbox_private_t *listbox_data, list_draw_mode_e mode);
//��ʾdialogbox
ui_result_e show_dialogbox(style_infor_t *dialogbox_style, dialog_private_t *dialogbox_data, dialog_draw_mode_e mode);
//��ʾparambox
ui_result_e show_parambox(style_infor_t *parambox_style, parambox_private_t *parambox_data, parambox_draw_mode_e mode);
//��ʾsliderbar
ui_result_e show_sliderbar(style_infor_t *parambox_style, parambox_private_t *parambox_data, parambox_draw_mode_e mode);
//��ȡtextbox���������
ui_result_e get_textbox_attrb(style_infor_t *textbox_style, void *attrb, uint8 type);
ui_result_e get_picbox_attrb(style_infor_t *picbox_style, void *attrb, uint8 type);
ui_result_e get_listbox_attrb(style_infor_t *listbox_style, void *attrb, uint8 type);
void read_pic_to_buffer(uint16 id, uint8 *buffer, res_size_t *res_size);
//��������
bool set_language(uint8 lang_id,void *null2, void *null3)__FAR__;
//����
void clear_screen(region_t *clrregion,void *null2,void *null3)__FAR__;
void flush_font_buffer(void *null1, void *null2, void *null3)__FAR__;
//��ʾ�ַ���
uint8 show_string(string_desc_t *string_infor, region_t *str_region,uint16 mode)__FAR__;
//�ַ�������Ϊ������...����ʽ
uint16 ellipsis_to_longstr(string_desc_t *desc, uint16 max_len, str_getlen_result_t *getlen_result)__FAR__;
//�ַ������У�Ҳ������ͳ������
text_end_mode_e get_text_line(string_desc_t *str_desc, uint16 mode, void *null3)__FAR__;
//ѡ��buffer�ĵ�ַ����С
void set_display_buf(uint8 mode,void *null2,void *null3)__FAR__;
//������������ѡ��
void set_lang_option(uint16 mode, void *null2, void *null3)__FAR__;
//unicodeת����
bool unicode_to_char(uint8 *str,uint16 len,uint8 lang_id)__FAR__;
//����תunicode
bool char_to_unicode(uint8 *dsc,uint8 *src,uint16 len)__FAR__;
//UTF8תunicode
void utf8_to_unicode(uint8* src,uint16*dest,int16* size)__FAR__;
//������Ļģʽ������������
void set_screen_direction(uint8 screen_mode, void *null2, void *null3)__FAR__;
void display_buffer_deal(void)__FAR__;
uint8 *put_string(string_desc_t *string_infor, region_t *str_region, uint16 mode);
bool scroll_string(string_desc_t *scroll_infor, region_t *scroll_region, scroll_mode_t scroll_mode)__FAR__;
void show_num_pic(uint16 num, uint16 x, uint16 y, res_entry_t *res_entry, res_open_infor_t *res_ptr)__FAR__;
uint8 *itoa_n(int32 num, uint8 *str, uint8 radix, uint16 counts, uint16 disp_mode)__FAR__;
void show_timebox_pic(timebox_private_t *time, timebox_t *timebox, timebox_draw_mode_e mode)__FAR__;
void show_timebox_string(timebox_private_t *time, timebox_t *timebox, timebox_draw_mode_e mode)__FAR__;
void show_numbox_pic(numbox_private_t *numbox_data, numbox_t *numbox, numbox_draw_mode_e mode)__FAR__;
void show_number_pic(uint8 *num_string, show_number_param_t *number_param, numbox_draw_mode_e mode)__FAR__;
void show_numbox_string(numbox_private_t *numbox_data, numbox_t *numbox, numbox_draw_mode_e mode)__FAR__;
uint8 TimeConvert(timebox_private_t *time, timebox_t *timebox, uint16 *time_buffer, uint8 *disp_bits)__FAR__;
void show_list_ratio(uint32 addr, listbox_private_t *listbox_data, uint8 mode, res_open_infor_t *style_ptr)__FAR__;
uint8 show_list_item(listbox_t *listbox, listbox_private_t *listbox_data, uint16 itemnum, uint8 type,\
res_open_infor_t *style_ptr)__FAR__;
void show_list_title(uint32 addr, string_desc_t *title_str, list_draw_mode_e mode, res_open_infor_t *style_ptr)__FAR__;
void show_list_slider(uint32 addr, listbox_private_t *listbox_data, list_draw_mode_e mode, \
res_open_infor_t *style_ptr)__FAR__;
void show_parambox_num(parambox_t *parambox, parambox_private_t *parambox_data, uint16 itemnum, uint8 type,\
parambox_draw_mode_e mode,res_open_infor_t *style_ptr)__FAR__;
void show_parambox_title(uint32 addr, uint16 title_id, res_open_infor_t *style_ptr)__FAR__;
void show_parambox_unit(parambox_t *parambox, parambox_private_t *parambox_data, uint16 itemnum,\
res_open_infor_t *style_ptr)__FAR__;
void show_sliderbar_num(sliderbar_t *sliderbar, parambox_private_t *parambox_data, uint16 itemnum, uint8 type,\
parambox_draw_mode_e mode,res_open_infor_t *style_ptr)__FAR__;
void show_sliderbar_title(uint32 addr, uint16 title_id, res_open_infor_t *style_ptr)__FAR__;
void show_sliderbar_unit(sliderbar_t *sliderbar,parambox_private_t *parambox_data, uint16 itemnum,\
res_open_infor_t *style_ptr)__FAR__;
void show_sliderbar_slider(sliderbar_t *sliderbar, parambox_private_t *parambox_data, uint16 itemnum, uint8 type,\
parambox_draw_mode_e mode,res_open_infor_t *style_ptr)__FAR__;

void char_out_rect(uint8 *str, int8 language, region_t *char_region, uint16 disp_mode, uint8 start_col, \
uint8 mode)__FAR__;
bool fill_rect_dump(region_t *fill_region, uint16 pic_id, fill_dump_direction_e direction)__FAR__;
uint8 *put_string_line_sdimage(string_desc_t *desc, region_t *str_region, uint8 mode)__FAR__;
void clear_remain_pixel_sdimage(region_t *remain_region)__FAR__;
#if 0//2012-10-30 17:27 �޸��汾
uint8 *fixf_put_string_line(string_desc_t *desc, region_t *str_region, char_fill_mode_e mode)__FAR__;
#endif

uint8 thai_point(uint8 *str, int8 language, uint16 source_code, uint8* char_buf, uint8 *pt_step)__FAR__;
uint16 arabic_char_to_unicode(string_desc_t *desc)__FAR__;
uint8 is_arabic_hebrew_string(string_desc_t *desc)__FAR__;
bool arabic_is_nsm(uint16 c)__FAR__;
uint8 arabic_do_join(string_desc_t *desc)__FAR__;
void arabic_uni_join(string_desc_t *desc)__FAR__;
void arabic_do_turn(uint8 *str, uint16 start, uint16 end) __FAR__;
void arabic_ascii_turn(string_desc_t *desc)__FAR__;
void bankaf_put_string_line_arabic_hook(string_desc_t *string_infor, uint16 disp_mode, uint16 param)__FAR__;

void ramf_put_string_line_hook(string_desc_t *string_infor, uint16 disp_mode, uint16 param)__FAR__;
uint8 verti_scroll_string(region_t *scroll, scroll_content_t *scroll_string_infor, uint8 index)__FAR__;
uint8 *put_string(string_desc_t *string_infor, region_t *str_region, uint16 mode)__FAR__;
uint16 get_string_length_id(uint16 str_id, uint16 disp_mode)__FAR__;

//͸��ͼƬ��ʾ������͸������ͼƬ
void show_pic_transparent(transparent_pic_t *p_trs_pic)__FAR__;

//CHAR_FILL_SDIMAGE ֧�֣�ֱ�Ӵ�SD��ȡ�����ٵ����ַ���
void put_char_sd(uint8 *psoure_buf, region_t *char_region, char_disp_e char_disp, uint8 start_col, uint8 mode)__FAR__;
void black_char_to_color_sd(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, \
uint8 start_col, uint8 start_row, uint8 end_row, uint8 mode)__FAR__;
void load_bgpic_sd(uint16 id, uint16 x, uint16 y, uint8 mode)__FAR__;
bool u_put_sd_image(sd_handle *res_fp, uint32 img_addr, region_t *img_region)__FAR__;
ui_result_e show_picbox_id3(style_infor_t *picbox_style, picbox_private_t *picbox_data, uint16 start_row)__FAR__;

//ͼ�δ���ӿ�
void draw_rect_frame(region_t* frame_region)__FAR__;
void draw_3D_rect(region_t* D3region, uint8 type)__FAR__;

#if (USE_MULTI_FONTLIB != 0)
//�̻�����fix
void fixf_get_next_char_point(uint16 uni_code, uint8 *pbuffer)__FAR__;
void fixf_get_font_point(uint16 code_rename, uint8 *picbuf)__FAR__;

void fix_switch_fontlib(uint16 disp_mode)__FAR__;
uint16 fixf_get_string_length(string_desc_t *desc, uint16 max_length, str_getlen_result_t *result)__FAR__;
void fixf_black_char_to_color(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, uint8 start_col)__FAR__;
void fixf_char_fill(uint8 *psoure_buf, char_disp_e char_disp, uint8 start_col)__FAR__;
bool fixf_put_char(uint8 *psoure_buf, char_fill_mode_e mode, char_disp_e char_disp, uint8 start_col)__FAR__;
uint8 *fixf_put_string_line(string_desc_t *desc, region_t *str_region, char_fill_mode_e mode)__FAR__;
#endif
bool u_show_pic(uint16 id, uint16 x, uint16 y)__FAR__;
bool u_put_sd_image(sd_handle *res_fp, uint32 img_addr, region_t *img_region)__FAR__;
uint8 *fixft_put_string_line(string_desc_t *desc, region_t *str_region, char_fill_mode_e mode)__FAR__;
bool fix_fill_rect(region_t *fill_region, uint16 color);

#endif //_UI_DRIVER_H
