/*******************************************************************************
 *                              us212A
 *                            Module: ui����
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-9 16:23     1.0             build this file 
*******************************************************************************/
/*!
 * \file     display.h
 * \brief    ui����ģ��������ݽṹ���궨�壬�ӿ�������
 * \author   lzcai
 * \version 1.0
 * \date  2011-9-9
*******************************************************************************/

#ifndef _DISPLAY_H
#define _DISPLAY_H

/*! \cond UI_DRIVER_API */

#ifdef PC
#include "Simulator.h"
#endif
#include "psp_includes.h"
#include "lcd_driver.h"
#include "lang_id.h"

//�ֿ��ʽ�涨
//1����ģ������뱣����Height*Width = H*8|H*16|H*24|H*32�����У�H >= ��ģʵ�ʸ߶ȣ����� <= �����ȡ�
//2����H >= ��ģʵ�ʸ߶�ʱ����ģ����ֱ�����Ͼ��д�ţ����ʵ�ʸ߶�Ϊż������ô���±߸����һ�룻
//   ���Ϊ�������±߱��ϱ߶�1���㡣
//3����ģ����ˮƽ�����ϴ������Ҵ�ţ����ֽ����bit��ʼ��ţ�
//   ���ʵ�ʿ�ȴ��ھ�����W��һ�룬��ôÿ�У�W bits�����1����ģ����
//   ����ÿ�д��2����ģ���󣬼��� 2N �� 2N + 1�У���0��ʼ��ţ���ģ��������Ŵ�ţ�
//   ������ģ������������ھ���߶ȣ�����������С�
//   ע��8*8�����ֿⲻ֧��ÿ�д��2����ģ����Ҳ����˵����ʹʵ�ʿ�����ڵ���4��Ҳ��ÿ�д��1����ģ����
//4����ģ�������ݺ����ֽ�Ϊ��ģ�����ʵ�ʿ�ȣ����磬16*16�����ֿ⣬��32���ֽڣ���0��ʼ��ţ���Ϊʵ�ʿ�ȡ�

//�ֿⶨ�壬��ӦUI Editor�����е�С���塢������ʹ�����
//bit0��ʾС���壬�����Ʊ���Ϊ UNICODES.FON 
//bit1��ʾ�����壬�����Ʊ���Ϊ UNICODE.FON 
//bit2��ʾ�����壬�����Ʊ���Ϊ UNICODEL.FON 
//���Ϊ0����ʾʹ��Ĭ�ϱ�׼�ֿ⣬����ģʵ�ʿ��Ϊ16*16
//ע����� lcd ����֧�ֶ�������ʹ����Ҫʹ�ö��ֿ⣬Ҳ��Ҫ�� USE_MULTI_FONTLIB ����Ϊ 0x02
#define  USE_MULTI_FONTLIB           0x0 
//�ֿ�1����ӦUI Editor�����е�С���壻���û��С���壬UI Editor�������õ�С��������������ʾ
#define  FONTLIB_1_HEIGHT            8   //������ģ�������߶�
#define  FONTLIB_1_HEIGHT_ACTUAL     8   //������ģ����ʵ�����߶�
#define  FONTLIB_1_WIDTH             8   //������ģ��������ȣ�Ҫ��Ϊ8����
#define  FONTLIB_1_SIZE              (FONTLIB_1_HEIGHT * FONTLIB_1_WIDTH / 8 + 1)
//�ֿ�2����ӦUI Editor�����е������壬�������ֿ������
#define  FONTLIB_2_HEIGHT            16  //������ģ�������߶�
#define  FONTLIB_2_HEIGHT_ACTUAL     16  //������ģ����ʵ�����߶�
#define  FONTLIB_2_WIDTH             16  //������ģ��������ȣ�Ҫ��Ϊ8����
#define  FONTLIB_2_SIZE              (FONTLIB_2_HEIGHT * FONTLIB_2_WIDTH / 8 + 1)
//�ֿ�3����ӦUI Editor�����еĴ����壻���û�д����壬UI Editor�������õĴ���������������ʾ
#define  FONTLIB_3_HEIGHT            24  //������ģ�������߶�
#define  FONTLIB_3_HEIGHT_ACTUAL     24  //������ģ����ʵ�����߶�
#define  FONTLIB_3_WIDTH             24  //������ģ��������ȣ�Ҫ��Ϊ8����
#define  FONTLIB_3_SIZE              (FONTLIB_3_HEIGHT * FONTLIB_3_WIDTH / 8 + 1)
//�ֿ���ģ��������С��ͬ�������ֿ⣬���ڶ�����ģ�����������û�д������ֿ⣬��ѡ���������ֿ⣻
#define  MAX_CHARPOINTE_SIZE         FONTLIB_3_SIZE


/*!
 * \brief
 *  LIST_NUM_ONE_PAGE_MAX �б���Ŀ��Ŀ
 */
#define LIST_NUM_ONE_PAGE_MAX   6

/*!
 * \brief
 *  ROW_MAX_WIDTH �ı�����һ������ȣ�ÿ�������ʾ 128 �����ص���
 */
#define ROW_MAX_WIDTH           128
/*!
 * \brief
 *  BUFF_ONE_ROW ÿ��buffer�ֽ�����ÿ�а���� 22 ���ַ��㣬Unicode�ַ�����Ҫ 46���ֽ�
 */
#define BUFF_ONE_ROW            46

//listbox �˵�����ʾ����bitmap
//����ͼ��ѡ��
#define ITEMS_BACK_TAG_ARGV_SHIFT   (0)
#define ITEMS_BACK_TAG_ARGV         (1 << ITEMS_BACK_TAG_ARGV_SHIFT)
#define ITEMS_BACK_TAG_ISNOT_SELECT (0 << ITEMS_BACK_TAG_ARGV_SHIFT)
#define ITEMS_BACK_TAG_IS_SELECT    (1 << ITEMS_BACK_TAG_ARGV_SHIFT)
//ǰ��ͼ��ѡ����bit��0����ʾ��1��ʾ����MULTY ����ԣ�bit8~14 ��ʾindex��0x7f��ʾֻ��λ����أ�����ֵ��ʾ����ֵָ��
#define ITEMS_FRONT_TAG_ARGV_SHIFT  (15)
#define ITEMS_FRONT_TAG_INDEX_SHIFT (8)
#define ITEMS_FRONT_TAG_INDEX       (0x7f << ITEMS_FRONT_TAG_INDEX_SHIFT)
#define ITEMS_FRONT_TAG_ARGV        (0x1 << ITEMS_FRONT_TAG_ARGV_SHIFT)
#define ITEMS_FRONT_TAG_ISNOT_DISP  (0x0 << ITEMS_FRONT_TAG_ARGV_SHIFT)
#define ITEMS_FRONT_TAG_IS_DISP     (0x1 << ITEMS_FRONT_TAG_ARGV_SHIFT)

//�ַ�������ѡ�� bit10-12Ԥ�����������С
//���˿���ѡ��
#define FILTER_BLANK_OPTION     (0x01 << 0)
#define FILTER_BLANK_DISABLE    (0x00 << 0)
#define FILTER_BLANK_ENABLE     (0x01 << 0)
//�Զ��ִ�ѡ��
#define DIVIDE_WORD_OPTION      (0x01 << 1)
#define DIVIDE_WORD_DISABLE     (0x00 << 1)
#define DIVIDE_WORD_ENABLE      (0x01 << 1)
//����ģʽѡ��
#define FUNC_MODE_OPTION        (0x0f << 4)//bit4 ~ bit8
#define FUNC_GET_LINE_CHARS     (0x00 << 4)//��ȡ��ǰ���ֽ���
#define FUNC_GET_TOTAL_LINES    (0x01 << 4)//��ȡ�ַ���������
#define FUNC_GET_CHAR_POINTS    (0x02 << 4)//��ȡ��ǰ�е��󻺳�

/*!
 * \brief
 *      textline_param_t ��ȡ�ַ�������ṹ��
 */
typedef struct
{
    /*! �ַ�������buffer */
    uint8 *point_buf;
    /*! �ַ������������ */
    uint16 true_width;
    /*! �ַ����������߶� */
    uint16 true_height;
} textline_param_t;


/*!
 * \brief
 *  ui_error_e ui���������������
 */
typedef enum
{
    /*! ����ʾ�ؼ� */
    UI_NO_DISPLAY           = 0x00,
    /*! ������ʾ */
    UI_NO_ERR               = 0x01,
    /*! �ַ�����ʾ��Ҫ���� */
    UI_NEED_SCROLL          = 0x02,
    /*! �����Ƿ����� */
    UI_PARAM_ERR            = 0x03,
    /*! �ַ�����ʾX��Խ����� */
    UI_PUTCHAR_X_ERR        = 0x04,
    /*! �ַ�����ʾY��Խ����� */
    UI_PUTCHAR_Y_ERR        = 0x05,
    /*! �ַ�����ʾ�������� */
    UI_PUTCHAR_OTHER_ERR    = 0x06,
    /*! �ַ�����ʾ������ĩβ */
    UI_SCROLL_OVER          = 0x07,
} ui_result_e;

/*!
 * \brief
 *  ��ɫֵ��������
 */
typedef uint32 COLORREF;

/*!
 * \brief
 *      DC ��ͼ�����ṹ��
 */
typedef struct
{
    /*! ���ʺ�������ɫ*/
    COLORREF pen_color;
    /*! ������ɫ*/     
    COLORREF backgd_color;
} DC, *PDC;

/*!
 * \brief
 *      string_desc_t �ַ����������ṹ��
 */
typedef struct
{
    /*! �ַ������ݣ����������ַ���ID����code�������Unicode�� */
    union
    {
        /*! code������ָ�� */
        uint8 *str;
        /*! �ַ���ID */
        uint16 id;
    } data;
    /*! �ַ�����������
     *  UNICODELANGUAGE��0x7f�� ��ʾ Unicode ��Դ��ֻ�� ui �����ڲ�ʹ�ã�
     *  UNICODEID��0x7e�� ��ʾ Unicode ID��
     *  UNICODEDATA��0x7d�� ��ʾ Unicode code data��
     *  UTF_8DATA��0x7c�� ��ʾ utf-8 code data��
     *  ANSIDATAAUTO��0x7b�� ��ʾ Ansi code data���Զ�����Ϊui ������ǰ���ԣ���
     *  ��������ID���� ENGLISH�� ��ʾ Ansi code data��
     *  ˵����Ϊ�˱�����ң����ǹ涨 UNICODELANGUAGE ֻ�� ui �����ڲ�ʹ�ã�Ӧ���� UNICODEID �� UNICODEDATA ����֮��
     */
    int8 language;
    uint8 reserve;
    /*! OUT����Ч�ֽڣ�IN����������С */
    uint16 length;
    /*! ����������ɾ��峡�Ͼ��������壻���ڶ����п���ָ����������ص��ȣ� */
    uint16 argv;
    /*! ���ڴ�Ž�����ɾ��峡�Ͼ��������壻���ڶ����п��Ա�ʾ��ʾ���ֽ����� */
    uint16 result;
} string_desc_t;



/*!
 * \brief
 *      picbox��ʾ˳��������ʾ
 */
#define DIRECTION_NORMAL    0
/*!
 * \brief
 *      picbox��ʾ˳��������ʾ
 */
#define DIRECTION_REVERSE   1

/*!
 * \brief
 *      picbox_private_t picbox˽�����ݽṹ
 * \note         
 *      pic_id!= -1��������ʾpic_id��ָ���ͼƬ��
 *      pic_id = -1��frame_id��=-1������ʾStyleID��ָ���Picbox��frame_id��ͼƬ����֡ͼƬ�е�һ֡��
 */
typedef struct
{
    /*! ���pic_id��Ϊ-1����ô����������pic_id��ʾ */
    uint16 pic_id;
    /*! ָʾ��ʾ��һ֡ͼƬ */
    uint8 frame_id;
    /*! ��ʾģʽ����Ϊ��ͨģʽ��͸��ͼ�꣬��ֱ���� */
    uint8 reserve;
} picbox_private_t;

/*!
 * \brief
 *      textbox_private_t textbox˽�����ݽṹ
 */
typedef struct
{
    /*! �ַ�����ԴID */
    uint16 str_id;
    /*! str_value�ַ������������� */
    int8 lang_id;
    /*! �����ֽ� */
    uint8 reserve;
    /*! �ַ���ָ�룬�����NULL����str_value��ʾ */
    uint8 *str_value;
    /*! �ַ�����ʾ��ɫ����16bitΪ0x5a5a��ʾ�û��Զ�����ɫ����16bitΪ16λ��ɫֵ */
    COLORREF color;
} textbox_private_t;

/*!
 * \brief
 *  time_disp_mode_e ʱ����ʾģʽö������
 */
typedef enum
{
    /*! ������ʾ��YYMMDD */
    DATE_DISP_YYMMDD    = 0,
    /*! ������ʾ��MMDDYY */
    DATE_DISP_MMDDYY    = 1,
    /*! ������ʾ��YYMM */
    DATE_DISP_YYMM      = 2,
    /*! ������ʾ��MMDD */
    DATE_DISP_MMDD      = 3,
    /*! ʱ����ʾ��HHMMSS */
    TIME_DISP_HHMMSS    = 4,
    /*! ʱ����ʾ��HHMM */
    TIME_DISP_HHMM      = 5,
    /*! ʱ����ʾ��MMSS */
    TIME_DISP_MMSS      = 6
} time_disp_mode_e;

/*!
 * \brief
 *      time_timebox_t hour Ϊuint16�汾time_t
 */
typedef struct
{
    /*! Сʱ */
    uint16 hour;
    /*! ���� */
    uint8  minute;
    /*! ���� */
    uint8  second;
} time_timebox_t;

/*!
 * \brief
 *      timebox_private_t timebox˽�����ݽṹ
 */
typedef struct
{
    /*! �Զ���time�ṹ��ʱ����������յ���ֵ */
    struct
    {
        /*! ʱ�������� */
        union
        {
            date_t date;
            time_timebox_t time;
        } union_time;
        /*! time���ͣ�'D'��ʾ���ڣ�'T'��ʾʱ�䣬����ǿ��Ϊʱ�� */
        char time_type;
        /*! ʱ����ʾģʽ */
        time_disp_mode_e mode;
        /*! �����ֽ� */
        uint8 reserve;
    } times[2];
}timebox_private_t;

/*!
 * \brief
 *      numbox_private_t numbox˽�����ݽṹ
 */
typedef struct
{
    /*! Ҫ��ʾ����ֵ����С����λ����������Ϣ���� */
    int32 value;
    /*! �������Բ鿴��ǰ��ֵ������λ�� */
    int32 total;
} numbox_private_t;

/*!
 * \brief
 *      progressbar_private_t progressbar˽�����ݽṹ
 */
typedef struct
{
    /*! ��������ǰ���ȣ���ֵ����progressbar�ڲ�ת��Ϊ������ */
    uint32 value;
    /*! �������ܽ��� */
    uint32 total;
} progressbar_private_t;

/*!
 * \brief
 *      listbox_private_t listbox˽�����ݽṹ
 */
typedef struct
{
    /*! �����ַ��� */
    string_desc_t title;
    /*! �б����ַ������� */
    string_desc_t items[LIST_NUM_ONE_PAGE_MAX];
    /*! �Ǽ�����ǰ��ͼ���б�����ID */
    uint16 head_id;
    /*! ������ǰ��ͼ���б�����ID */
    uint16 head_act_id;
    /*! �б���items����Чǰ���� */
    uint16 item_valid;
    /*! ��ǰ������ */
    uint16 active;
    /*! ֮ǰ������ */
    uint16 old;
    
    /*! Ӧ�������������������ڻ����˺�ratio */
    uint16 list_total;
    /*! ��ǰ��������Ӧ�����������λ�ã����ڻ����˺�ratio */
    uint16 list_no;
} listbox_private_t;

/*!
 * \brief
 *      dialog_private_t dialog˽�����ݽṹ
 */
typedef struct
{
    /*! ��ϸ˵���ַ��������� */
    string_desc_t *string_desc;
    /*! �Ի���Icon ID */
    uint16 icon_id;
    /*! ��ť���飬���֧��3����ť */
    uint16 buttons[3];
    /*! ��ť��Ŀ�����3�� */
    uint8 button_cnt;
    /*! ��ǰ���ť��� */
    uint8 active;
    /*! ֮ǰ���ť��� */
    uint8 old;
} dialog_private_t;

typedef enum
{
    /*! �ս����û�е��� */
    ADJUST_RESULT_NULL      = 0,
    /*! �����������ֵ */
    ADJUST_RESULT_VALUE     = 1,
    /*! �����������ֵ����Ҫ����ˢ���в���ֵ */
    ADJUST_RESULT_ALL       = 2,
} adjust_result_e;

/*!
 * \brief
 *      parambox_one_t parambox�����������ṹ��
 */
typedef struct parambox_one_struct
{
    /*! ������λ��Դ�ַ���ID����ʵҲ�����ڵ�λ�����������⸨��˵���ַ��� */
    uint16 unit_id;
    /*! ����ֵ��Сֵ */
    int16 min;
    /*! ����ֵ���ֵ */
    int16 max;
    /*! ����ֵ���� */
    int16 step;
    /*! ������ǰֵ */
    int16 value;
    /*! �Ƿ�����ѭ�����ã�����������Ϊ��С����С�ݼ���Ϊ��� */
    uint8 cycle;
    /*! ����ֵ���λ�� */
    uint8 max_number;
    /*! ������ǰֵ�ַ�����ʾ������ͨ��adjust_funcת�� */
    uint8 *value_str;
    /*! ����ֵ�������ص��������������������������õȣ����ﴫ�ݽ�������
     *  �������飬���Ǽ����ڶ���������У�����λ���ǹ̶��ģ������ǿ���ͨ
     *  ��ȫ�ֱ�����õģ�
     */
    adjust_result_e (*adjust_func)(struct parambox_one_struct *params, uint8 active, bool inc);
    /*! ���ü�ʱ�ص������������������õ� */
    bool (*callback)(int16 value);
} parambox_one_t;

/*!
 * \brief
 *      parambox_private_t parambox˽�����ݽṹ
 */
typedef struct
{
    /*! ����ͼƬID�������û�����ģ�屳��ͼƬ */
    uint16 back_id;
    /*! ��־ͼ��ID */
    uint16 icon_id;
    /*! ���ñ�����Դ�ַ���ID */
    uint16 title_id;
    /*! ���ò������� */
    uint8 param_cnt;
    /*! ��ǰ�����Ҳ��Ϊ���������ʱĬ�ϼ����� */
    uint8 active;
    /*! ֮ǰ������ */
    uint8 old;
    /*! �Ƿ���ʾ����ֵ���ţ�0��ʾ����ʾ��1��ʾ��ʾ */
    uint8 sign_flag;
    /*! �̶����ص�ֵ������slider��Ч */
    uint8 scale;
    uint8 reserve;
    /*! ���ò����б� */
    parambox_one_t *items;
} parambox_private_t;

/*!
 * \brief
 *      res_type_e ��Դ�������ݽṹ
 */
typedef enum
{
    /*! ��Դ����ΪAP��Դ */
    UI_AP = 0, 
    /*! ��Դ����ΪCOMMON��Դ */ 
    UI_COM = 1  
} res_type_e;

/*!
 * \brief
 *      style_infor_t �ؼ���style��Ϣ
 */
typedef struct
{
    /*! Ҫ��ʾ�Ŀؼ�����Դ�ļ��е�style ID */
    uint16 style_id;   
    /*! ��Դ���� */
    res_type_e type;
} style_infor_t;

/*!
 * \brief
 *      scroll_mode_t ����ģʽ
 */
typedef struct
{
    /*! ģʽ��TRUE or FALSE */
    uint16 mode;
    /*! ��ʾģʽ�����ַ���UI��ʾЧ������*/
    uint16 disp_mode; 
} scroll_mode_t;

/*!
 * \brief
 *      scroll_content_t Ҫ�������ַ�����ͼ����Ϣ��musicӦ��ID3����ר�ã�
 */
typedef struct
{
    /*! Ҫ�������ַ���ָ��*/
    uint8 *strbuf;
    /*! ��ǰ�ַ���������*/
    int8  language;
    /*! �����ֽ� */
    uint8 reserve[3];
} scroll_content_t;

/*!
 * \brief
 *      scroll_param_t ����������musicӦ��ID3����ר�ã�
 */
typedef struct
{  
    /*! �Ƿ�Ϊ��ʼ������*/
    uint8 init;
    /*! �Ƿ�ˢ����ʾ�����ڹ�����ʼ��ʱ��Ч */
    uint8 update;
    /*! ��ǰ���*/     
    uint8 active; 
    /*! ��������*/
    uint8 direct; 
    /*! ID3 textbox */
    style_infor_t *id3_text;
    /*! ID3 picbox multi frame */
    style_infor_t *id3_icon;
} scroll_param_t;


/*!
 * \brief
 *  str_getlen_result_t ��ȡ�ַ������ȸ�������ṹ�壬�԰�������ϣ������ʾ�����ش�
 */
typedef struct
{
    /*! �պ�δ���� max_length �ַ������ȣ���ͷ�����ⲿ���ַ������������ַ�����ʾ */
    uint16 true_length;
    /*! ���ϲ����ַ����ֽ�������ͷ�����ⲿ���ַ������������ַ�����ʾ */
    uint16 true_bytes;
} str_getlen_result_t;


/*����USB�����ݽṹ*/
/*!
 *  \brief
 *  ��Դ�ĳ�����Ϣ
 */
typedef struct
{
    uint16 width;
    uint16 height;
}res_size_t;
/*!
 * \brief
 *  ui_cmd_e ui������������ö������
 */
typedef enum
{
    /*! ����Դ�ļ�*/
    UI_RES_OPEN = 0,
    /*! �ر���Դ�ļ�*/
    UI_RES_CLOSE,
    /*! ��ʾPicBox*/
    UI_SHOW_PICBOX,             
    /*! ��ʾTextBox*/
    UI_SHOW_TEXTBOX,            
    /*! ��ʾTimeBox*/
    UI_SHOW_TIMEBOX,
    /*! ��ʾNumBox*/
    UI_SHOW_NUMBOX,
    /*! ��ʾProgressBar*/
    UI_SHOW_PROGRESSBAR,
    /*! load AttributeBox������*/
    UI_LOAD_ATTRIBUTEBOX,
    /*! ��ʾListBox*/       
    UI_SHOW_LISTBOX,
    /*! ��ʾDialogBox*/ 
    UI_SHOW_DIALOG,
    /*! ��ʾParamBox*/ 
    UI_SHOW_PARAMBOX,
    /*! ��ʾsliderbar*/
    UI_SHOW_SLIDERBAR,
    /*! ��ȡtextbox�������*/
    UI_GET_TEXTBOX_ATTRB,
    /*! ��ȡpicbox����*/
    UI_GET_PICBOX_ATTRB,
    /*! ��ȡlistbox���� */
    UI_GET_LISTBOX_ATTRB,
    /*! ID3����ר�ú���*/ 
    UI_SCROLLSTRING_EXT,
    /*! ������������*/            
    UI_SET_LANGUAGE,
    /*! ��ȡDC״̬*/
    UI_GET_DC_STATUS,
    /*! ����*/
    UI_CLEARSCREEN,
    /*! ���û�����ɫ*/
    UI_SET_PEN_COLOR,
    /*! ���ñ�����ɫ*/
    UI_SET_BACKGD_COLOR,
    /*! ��ʾ�ַ���*/
    UI_SHOW_STRING,
    /*! �ַ�������Ϊ������...����ʽ*/
    UI_ELLIPSIS_TO_LONGSTR,
    /*! ��ȡ�ַ����ĳ���*/
    UI_GET_STRING_LENGTH, 
    /*! �ı�������У��������˵��հ��ַ��ͷִ���ʾ�� */
    UI_GET_TEXT_LINE,
    /*! ������ʾBUFFER�ĵ�ַ����С*/
    UI_SET_DISPLAYBUF,
    /*! Unicodeת��������*/
    UI_UNICODE_TO_CHAR,
    /*! ����ת����Unicode*/
    UI_CHAR_TO_UNICODE,
    /*! ��ȡ��������ַ�����UNICODE����*/
    UI_GET_MULTI_STRING_UNICODE,
    /*! UTF8����ת����UNICODE*/
    UI_UTF8_TO_UNICODE,
    /*! ���ú���������ģʽ*/
    UI_SET_SCREEN_DIRECTION,
    /*! ��ʾ��ԴͼƬ*/
    UI_SHOW_PIC,
    /*! ����ԴͼƬ����buffer��*/
    UI_READ_PIC_TO_BUFFER,
    /*! ���font������ */
    UI_FLUSH_FONT_BUFFER,
    /*! ��������ѡ�� */
    UI_SET_LANG_OPTION,
} ui_cmd_e;

/*!
 * \brief
 *      textbox_draw_mode_e �ı������ģʽö������
 */
typedef enum
{
    /*! ������� */
    TEXTBOX_DRAW_NULL         = 0,
    /*! ����ͨ��ʽ��ʾTEXTBOX */
    TEXTBOX_DRAW_NORMAL       = 1,
    /*! ����ͨ��ʽ��ʾTEXTBOX���������й�����ʼ�� */
    TEXTBOX_DRAW_NOSCROLL     = 2,
    /*! ����+���� */
    TEXTBOX_DRAW_ITEM         = 3,
    /*! ������ʾ����ͨģʽ */
    TEXTBOX_DRAW_SCROLL       = 4,
    /*! ������ʾ������+���ݣ�ֻ������ */
    TEXTBOX_DRAW_ITEM_SCROLL  = 5,
    /*! ID3��ʼ��ˮƽ����ר�� */
    TEXTBOX_DRAW_ID3_HORIZONTAL    = 6,
    /*! ID3��ʼ����ֱ����ר�� */
    TEXTBOX_DRAW_ID3_VERTICAL      = 7,
} textbox_draw_mode_e;

/*!
 * \brief
 *      list_draw_mode_e �б�ؼ�����ģʽö������
 */
typedef enum
{
    /*! ������� */
    LIST_DRAW_NULL      = 0,
    /*! �ػ漤���� */
    LIST_DRAW_ACTIVE    = 1,
    /*! �ػ������б� */
    LIST_DRAW_LIST      = 2,
    /*! �ػ�ȫ�� */
    LIST_DRAW_ALL       = 3,
    /*! �����б�ͷ */
    LIST_DRAW_TITLE_SCROLL = 4,
    /*! ���������� */
    LIST_DRAW_ACTIVE_SCROLL = 5,
} list_draw_mode_e;
/*!
 * \brief
 *      dialog_draw_mode_e �Ի���ؼ�����ģʽö������
 */
typedef enum
{
    /*! ������� */
    DIALOG_DRAW_NULL        = 0,
    /*! �ػ水ť */
    DIALOG_DRAW_BUTTON      = 1,
    /*! �ػ�Ի��� */
    DIALOG_DRAW_ALL         = 2,
} dialog_draw_mode_e;

/*!
 * \brief
 *      parambox_draw_mode_e �������ÿؼ�����ģʽö������
 */
typedef enum
{
    /*! ������� */
    PARAMBOX_DRAW_NULL      = 0,
    /*! �ػ����ֵ */
    PARAMBOX_DRAW_VALUE     = 1,
    /*! �ػ���������������� */
    PARAMBOX_DRAW_PARAM     = 2,
    /*! �ػ�������ÿ� */
    PARAMBOX_DRAW_ALL       = 3,
} parambox_draw_mode_e;

/*!
 * \brief
 *      progress_draw_mode_e �������ؼ�����ģʽö������
 */
typedef enum
{
    /*! ������� */
    PROGRESS_DRAW_NULL           = 0,
    /*! �ػ���� */
    PROGRESS_DRAW_PROGRESS       = 1,
    /*! ������ר���ػ���ȣ����ͷ��ʼ�� */
    PROGRESS_DRAW_LEFT_INIT      = 2,
    /*! ������ר���ػ���ȣ����ͷ���� */
    PROGRESS_DRAW_PROGRESS_LEFT  = 3,
    /*! ������ר���ػ���ȣ��Ҽ�ͷ��ʼ�� */
    PROGRESS_DRAW_RIGHT_INIT     = 4,
    /*! ������ר���ػ���ȣ��Ҽ�ͷ */
    PROGRESS_DRAW_PROGRESS_RIGHT = 5,
    /*! �ػ�ȫ�� */
    PROGRESS_DRAW_ALL            = 6,
} progress_draw_mode_e;

/*!
 * \brief
 *      numbox_draw_mode_e ���ֿؼ�����ģʽö������
 */
typedef enum
{
    /*! ������� */
    NUMBOX_DRAW_NULL        = 0,
    /*! �ػ����� */
    NUMBOX_DRAW_NUMBER      = 1,
    /*! �ػ�ȫ�� */
    NUMBOX_DRAW_ALL         = 2,
} numbox_draw_mode_e;

/*!
 * \brief
 *      timebox_draw_mode_e ʱ��ؼ�����ģʽö������
 */
typedef enum
{
    /*! ������� */
    TIMEBOX_DRAW_NULL       = 0,
    /*! �ػ�ʱ������� */
    TIMEBOX_DRAW_TIME       = 1,
    /*! �ػ�ȫ�� */
    TIMEBOX_DRAW_ALL        = 2,
} timebox_draw_mode_e;


/*!
 * \brief
 *  16bit �ַ�����ʾģʽ��ʽ��
 *  0-1      | 2-3      | 4-7          | 8-9      | 10-11 | 12       | 13       | 14           | 15
 *  ���ģʽ | ���뷽ʽ | ������ʾģʽ | �������� | �ֺ�  | �Զ��ִ� | ȡ����ʾ | �����б�˵� | ȡ������������
 */
#define DISP_ALIGN_OPTION_SHIFT     (2)
#define DISP_ALIGN_OPTION           (0x3 << DISP_ALIGN_OPTION_SHIFT)
#define DISP_ALIGN_LEFT             (0 << DISP_ALIGN_OPTION_SHIFT)//����Ĭ������������
#define DISP_ALIGN_MEDIACY          (1 << DISP_ALIGN_OPTION_SHIFT)//����
#define DISP_ALIGN_RIGHT            (2 << DISP_ALIGN_OPTION_SHIFT)//����

#define DISP_LONGSTR_OPTION_SHIFT   (4)
#define DISP_LONGSTR_OPTION         (0xf << DISP_LONGSTR_OPTION_SHIFT)
#define DISP_LONGSTR_RECT           (0 << DISP_LONGSTR_OPTION_SHIFT)//Ĭ��ģʽ����ʾ�������ַ�
#define DISP_LONGSTR_NO_RECT        (1 << DISP_LONGSTR_OPTION_SHIFT)//�����������ַ�
#define DISP_LONGSTR_MULTI_ROW      (2 << DISP_LONGSTR_OPTION_SHIFT)//������ʾ
#define DISP_LONGSTR_ELLIPSIS       (3 << DISP_LONGSTR_OPTION_SHIFT)//��ʡ�Ժ���ʾ
#define DISP_LONGSTR_SCROLL         (4 << DISP_LONGSTR_OPTION_SHIFT)//������ʾ

#define DISP_SCROLL_OPTION_SHIFT    (8)
#define DISP_SCROLL_OPTION          (0x3 << DISP_SCROLL_OPTION_SHIFT)
#define DISP_SCROLL_HORIZONTAL      (0 << DISP_SCROLL_OPTION_SHIFT)//Ĭ�����ԣ�ˮƽ����
#define DISP_SCROLL_VERTICAL        (1 << DISP_SCROLL_OPTION_SHIFT)//���Ϲ���
#define DISP_SCROLL_REPEAT          (2 << DISP_SCROLL_OPTION_SHIFT)//ѭ��������Ҳ��ˮƽ��������β��ӣ��м����2�ո��

//ID3��������
#define ID3_SCROLL_HORIZONTAL       (0)//ID3ˮƽ����
#define ID3_SCROLL_VERTICAL         (1)//ID3��ֱ����

#define DISP_FONT_OPTION_SHIFT      (10)
#define DISP_FONT_OPTION            (0x3 << DISP_FONT_OPTION_SHIFT)
#define DISP_FONT_NORMAL            (0 << DISP_FONT_OPTION_SHIFT)//Ĭ�����壬�к�����
#define DISP_FONT_LARGE             (1 << DISP_FONT_OPTION_SHIFT)//�������
#define DISP_FONT_SMALL             (2 << DISP_FONT_OPTION_SHIFT)//С������

#define DISP_DIVIDE_WORD_SHIFT      (12)
#define DISP_DIVIDE_WORD_ENABLE     (1 << DISP_DIVIDE_WORD_SHIFT)//�Զ��ִ�

#define DISP_DISPLAY_DISABLE_SHIFT  (13)
#define DISP_DISPLAY_DISABLE        (1 << DISP_DISPLAY_DISABLE_SHIFT)//ȡ����ʾ��������

#define DISP_LANG_LIST_MENU_SHIFT   (14)
#define DISP_LANG_LIST_MENU         (1 << DISP_LANG_LIST_MENU_SHIFT)//�����б�˵�ѡ���Ҫ���⴦��

#define DISP_ARABIC_DISABLE_SHIFT   (15)
#define DISP_ARABIC_DISABLE         (1 << DISP_ARABIC_DISABLE_SHIFT)//ȡ������������

#define DISP_CHAR_FILL_OPTION       (0x3 << 0)

/*!
 * \brief
 *  �ַ����ģʽ��͸��ģʽ���ȼ�Ϊ CHAR_FILL_NORMAL > CHAR_FILL_POINT������ģʽ��תΪ CHAR_FILL_SDIMAGE��
 *  ���lcd��֧��GRAM��������ui ������ģʽ��ֵ CHAR_FILL_NORMAL ȫ������ CHAR_FILL_POINT
 */
typedef enum
{
    /*! ��ͨģʽ����GRAM�����ַ����������ٴ�����ȥ */
    CHAR_FILL_NORMAL   = 0,
    /*! ��͸��ģʽ����ʾ�����ڣ��ֵ���ǰ��ɫ���հ׵����ɫ */
    CHAR_FILL_COLOR    = 1,
    /*! ֱ�Ӷ�ͼģʽ��Ҫ���ַ��������������ȴ洢�����Դ�SD����
     *  �����ַ����򱳾�ͼƬ�����غ�ͬ NORMAL һ������
     */
    CHAR_FILL_SDIMAGE  = 2,
    /*! ֱ�Ӵ��ģʽ��͸�����������ر���ͼƬ���ֵ�ֱ�Ӵ�ǰ��ɫ���հ׵����� */
    CHAR_FILL_POINT    = 3,
} char_fill_mode_e;

/*!
 * \brief
 *  ��������н�������
 */
typedef enum
{
    /*! ��ʼ���н������� */
    TEXT_END_INIT         = 0,
    /*! ȱҳ���� */
    TEXT_END_PAGE_MISS    = 1,
    /*! ���� '\0' �ַ����� */
    TEXT_END_NUL          = 2,
    /*! �������з����� */
    TEXT_END_LR           = 3,
    /*! �����Ѿ��������� */
    TEXT_END_OVER         = 4,
    /*! ��ȡ�ı�����ʧ�� */
    TEXT_END_READ_ERR     = 5,
} text_end_mode_e;

/*!
 * \brief
 *  ��������ѡ��
 */
/*! �����б�˵�ѡ���Ҫ�����������ͽ������⴦���Ա�֤�κ����Զ��ܿ���һ��Ч�� */
#define  LANG_LIST_MENU_OPTION  (0x01 << 0)
#define  LANG_LIST_MENU_ENTER   (0x01 << 0)
#define  LANG_LIST_MENU_EXIT    (0x00 << 0)

/*!
 * \brief
 *  �ַ��������Ͷ���
 */
/*! �ַ���ԴΪunicode��Դ����ָunicode data �� unicode ID */ 
#define  UNICODELANGUAGE  0x7f    
/*! �ַ���ԴΪunicode��ԴID */ 
#define  UNICODEID        0x7e   
/*! �ַ���ԴΪunicode data */ 
#define  UNICODEDATA      0x7d 
/*! �ַ���ԴΪutf-8 data */ 
#define  UTF_8DATA        0x7c  
/*! �ַ���ԴΪansi data�����������Զ�����Ϊui ������ǰ���� */ 
#define  ANSIDATAAUTO     0x7b    
                               

/*!
 * \brief
 *  ������ɫ��
 */
#define  Color_BLACK_def        0x00000000//��      --��Ϊͨ�õı�����ɫ
#define  Color_WHITE_def        0x0000ffff//��      --��Ϊͨ�õĻ�����ɫ
#define  Color_GRAY_def         0x0000bdf7//��191
#define  Color_RED_def          0x0000f800//��
#define  Color_YELLOW_def       0x0000e748//��
#define  Color_GREEN_def        0x000007e0//��
#define  Color_BULE_def         0x0000001f//��
#define  Color_BULE_Dark_def    0x0000753E//����
#define  Color_BULE_Light_def   0x0000335f//ǳ��
#define  Color_ORANGE_def       0x0000fcc0//��
#define  Color_CYAN_def         0x00008cc1//��
#define  Color_Gold_def         0x0000f488//���ɫ
#define  Color_Red_Dark_def     0x00003800//����ɫ
/*!
 * \brief
 *   �ɲ��� windows ���ʵ���ɫ��
 */
#define  Color_tab0101_def   Color_BLACK_def
#define  Color_tab0102_def   0x00008410
#define  Color_tab0103_def   0x00008000
#define  Color_tab0104_def   0x00008400
#define  Color_tab0105_def   0x00000400
#define  Color_tab0106_def   0x00000410
#define  Color_tab0107_def   0x00000010
#define  Color_tab0108_def   0x00008010
#define  Color_tab0109_def   0x00008408
#define  Color_tab0110_def   0x00000208
#define  Color_tab0111_def   0x0000041f
#define  Color_tab0112_def   0x00000210
#define  Color_tab0113_def   0x0000401f
#define  Color_tab0114_def   0x00008200
#define  Color_tab0201_def   Color_WHITE_def
#define  Color_tab0202_def   0x0000c618
#define  Color_tab0203_def   Color_RED_def
#define  Color_tab0204_def   0x0000ffe0
#define  Color_tab0205_def   Color_GREEN_def
#define  Color_tab0206_def   0x000007ff
#define  Color_tab0207_def   Color_BULE_def
#define  Color_tab0208_def   0x0000f81f
#define  Color_tab0209_def   0x0000fff0
#define  Color_tab0210_def   0x000007f0
#define  Color_tab0211_def   0x000087ff
#define  Color_tab0212_def   0x0000841f
#define  Color_tab0213_def   0x0000f810
#define  Color_tab0214_def   0x0000fc08


//ui��ʾ�ӿں�������
extern void *ui_op_entry(void *param1, void *param2, void *param3,ui_cmd_e cmd) __FAR__;

/*!
 * \brief
 *   ����Դ�ļ�
 */
#define ui_res_open(filename,type)                     \
ui_op_entry((void*)(filename), (void*)(type), (void*)(0), UI_RES_OPEN)

/*!
 * \brief
 *   �ر���Դ�ļ�
 */
#define ui_res_close(type)                             \
ui_op_entry((void*)(type), (void*)(0), (void*)(0), UI_RES_CLOSE)

/*!
 * \brief
 *   ID3����
 */
#define ui_scroll_string_ext(infor,param)  \
ui_op_entry((void*)(infor), (void*)(param), (void*)(0), UI_SCROLLSTRING_EXT)

/*!
 * \brief
 *   ��ʾpicbox
 */
#define ui_show_picbox(style,data)                     \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(0), UI_SHOW_PICBOX)

/*!
 * \brief
 *   ��ʾtextbox
 */
#define ui_show_textbox(style,data,mode)               \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_TEXTBOX)

/*!
 * \brief
 *   ��ʾtimebox
 */
#define ui_show_timebox(style,data,mode)               \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_TIMEBOX)

/*!
 * \brief
 *   ��ʾnumbox
 */
#define ui_show_numbox(style,data,mode)                \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_NUMBOX)

/*!
 * \brief
 *   ��ʾprogressbar
 */
#define ui_show_progressbar(style,data, mode)          \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_PROGRESSBAR)

/*!
 * \brief
 *   ��ȡattributebox��������Ϣ
 */
#define ui_load_attributebox(style,data, count)        \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(count), UI_LOAD_ATTRIBUTEBOX)

/*!
 * \brief
 *   ��ʾlistbox
 */
#define ui_show_listbox(style,data,mode)               \
(ui_result_e)(int)(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_LISTBOX)

/*!
 * \brief
 *   ��ʾdialogbox
 */
#define ui_show_dialogbox(style,data,mode)             \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_DIALOG)

/*!
 * \brief
 *   ��ʾparambox
 */
#define ui_show_parambox(style,data,mode)              \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_PARAMBOX)

/*!
 * \brief
 *   ��ʾsliderbar
 */
#define ui_show_sliderbar(style,data,mode)              \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_SLIDERBAR)

/*!
 * \brief
 *   ��ȡtextbox������
 */
#define ui_get_textbox_attrb(style,attrb,type)         \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(attrb), (void*)(type), UI_GET_TEXTBOX_ATTRB)

/*!
 * \brief
 *   ��ȡpicbox������
 */
#define ui_get_picbox_attrb(style,attrb,type)          \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(attrb), (void*)(type), UI_GET_PICBOX_ATTRB)

/*!
 * \brief
 *   ��ȡlistbox������
 */
#define ui_get_listbox_attrb(style,attrb,type)         \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(attrb), (void*)(type), UI_GET_LISTBOX_ATTRB)

/*!
 * \brief
 *   ��������
 */
#define ui_set_language(lang_id)                       \
ui_op_entry((void*)(uint32)(lang_id), (void*)(0), (void*)(0), UI_SET_LANGUAGE)

/*!
 * \brief
 *   ��ȡDC״̬
 */
#define ui_get_DC_status(pdc)                          \
ui_op_entry((void*)(pdc), (void*)(0), (void*)(0), UI_GET_DC_STATUS)

/*!
 * \brief
 *   ����
 */
#define ui_clear_screen(clrregion)                     \
ui_op_entry((void*)(clrregion), (void*)(0), (void*)(0), UI_CLEARSCREEN)

/*!
 * \brief
 *   ���û�����ɫ
 */
#define ui_set_pen_color(color)                        \
ui_op_entry((void*)(color), (void*)(0), (void*)(0), UI_SET_PEN_COLOR)

/*!
 * \brief
 *   ���ñ�����ɫ
 */
#define ui_set_backgd_color(color)                     \
ui_op_entry((void*)(color), (void*)(0), (void*)(0), UI_SET_BACKGD_COLOR)

/*!
 * \brief
 *   ��ָ��������ʾ�ַ���
 */
#define ui_show_string(infor, region, mode)             \
ui_op_entry((void*)(infor), (void*)(region), (void*)(uint32)(mode), UI_SHOW_STRING)

/*!
 * \brief
 *   �ַ�������Ϊ������...����ʽ
 */
#define ui_ellipsis_to_longstr(desc, max, result)      \
ui_op_entry((void*)(desc), (void*)(uint32)(max), (void*)(uint32)(result), UI_ELLIPSIS_TO_LONGSTR)

/*!
 * \brief
 *   ��ȡ�ַ����ĳ��ȣ�Unicode�����ַ�������ʹ�� UNICODELANGUAGE ��Ϊ��������
 */
#define ui_get_string_length(desc, max, result)        \
ui_op_entry((void*)(desc), (void*)(max), (void*)(result), UI_GET_STRING_LENGTH)

/*!
 * \brief
 *  �����ַ�����ר�����ı��Ķ�
 */
#define ui_get_text_line(desc, mode, param)            \
ui_op_entry((void*)(desc), (void*)(uint32)(mode), (void*)(param), UI_GET_TEXT_LINE)

/*!
 * \brief
 *  ������ʾbuffer
 */
#define ui_set_display_buf(mode)                       \
ui_op_entry((void*)(uint32)(mode), (void*)(0), (void*)(0), UI_SET_DISPLAYBUF)

/*!
 * \brief
 *  unicodeת����
 */
#define ui_unicode_to_char(str,len,lang_id)            \
ui_op_entry((void*)(str), (void*)(uint32)(len), (void*)(int32)(lang_id), UI_UNICODE_TO_CHAR)

/*!
 * \brief
 *  ����תunicode
 */
#define ui_char_to_unicode(dest,src,len)               \
ui_op_entry((void*)(dest), (void*)(src), (void*)(uint32)(len), UI_CHAR_TO_UNICODE)

/*!
 * \brief
 * ��ȡ��������ַ�����Դ��unicode����
 */
#define ui_get_multi_string_unicode(id,infor)          \
ui_op_entry((void*)(uint32)(id), (void*)(infor), (void*)(0), UI_GET_MULTI_STRING_UNICODE)

/*!
 * \brief
 * utf8תunicode
 */
#define ui_utf8_to_unicode(src,dest,size)              \
ui_op_entry((void*)(src), (void*)(dest), (void*)(size), UI_UTF8_TO_UNICODE)

/*!
 * \brief
 *  ������Ļģʽ
 */
#define ui_set_screen_direction(screen_mode)           \
ui_op_entry((void*)(uint32)(screen_mode), (void*)(0), (void*)(0), UI_SET_SCREEN_DIRECTION)

/*!
 * \brief
 * ��ʾͼƬ
 */
#define ui_show_pic(id,x,y)                            \
ui_op_entry((void*)(uint32)(id), (void*)(uint32)(x), (void*)(uint32)(y), UI_SHOW_PIC)

/*!
 * \brief
 * ��ȡͼƬ��Ϣ���浽buffer��
 */
#define ui_read_pic_to_buffer(id,buffer,res_size)      \
ui_op_entry((void*)(uint32)(id), (void*)(buffer), (void*)(res_size), UI_READ_PIC_TO_BUFFER)

/*!
 * \brief
 * �����ַ�������ģ������
 */
#define ui_flush_font_buffer()                         \
ui_op_entry((void*)(0), (void*)(0), (void*)(0), UI_FLUSH_FONT_BUFFER)

/*!
 * \brief
 * ������������ѡ��
 */
#define ui_set_lang_option(mode)                       \
ui_op_entry((void*)(uint32)(mode), (void*)(0), (void*)(0),UI_SET_LANG_OPTION)

/*! \endcond */

#endif //_DISPLAY_H
