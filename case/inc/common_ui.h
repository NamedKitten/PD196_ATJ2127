/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-8 16:53     1.0             build this file 
*******************************************************************************/
/*!
 * \file     common_ui.h
 * \brief    ���ÿؼ�ģ��������ݽṹ���궨�壬�ӿ�������
 * \author   lzcai
 * \version  1.0
 * \date     2011-9-8
*******************************************************************************/

#ifndef _common_ui_H_
#define _common_ui_H_

#include "psp_includes.h"
#include "enhanced.h"
#include "applib.h"
#include "case_type.h"
#include "common_func.h"
#include "setting_common.h"
#include "alarm_common.h"
#include "key_interface.h"

/* �б�������б�������ַ�����������С��52�ֽ� */
#define LIST_ITEM_BUFFER_LEN    52

/* album art ͼƬ��ʾλ���� */
#define ALBUM_ART_MAX_COUNT     6

/*! ��������ģʽ */
#define SET_VOLUME_VALUE    0
#define SET_VOLUME_LIMIT    1
#define SET_VOLUME_ALARM    2
//ר�Ÿ���ƵӦ��ʹ��
#define SET_VOLUME_ENTER    0
#define SET_VOLUME_INC      1
#define SET_VOLUME_DEC      2

/*! �������ֵ 31���ܹ� 32 �� */
#define VOLUME_VALUE_MAX    31

/*! ��ص�ѹ�ȼ�������ʾ��ʾ�͵磬0~4 ��� 6 ��״̬ */
#define BATTERY_GRADE_MAX   5

/*! ������ʾ�ص��������� */
typedef void (*animation_callback)(void);

/*!
 * \brief
 *  com_str_buffer ui bank �鹲�� bank data������bank �亯�����ݲ���
 */
extern uint8 com_str_buffer[192] _BANK_DATA_ATTR_;
extern uint8 com_name_buffer[52] _BANK_DATA_ATTR_;
extern alarm_alarm_t g_alarm_temp _BANK_DATA_ATTR_;
extern alarm_fm_t g_aufm_temp _BANK_DATA_ATTR_;

/*!
 * \brief
 *  del_mode_e ɾ��ģʽö������
 */
typedef enum
{
    /*! ɾ�������ļ� */
    DEL_MODE_FILE       = 0,
    /*! ɾ�������ļ��л��б� */
    DEL_MODE_LIST       = 1,
} del_mode_e;

/*!
 * \brief
 *  list_update_mode_e �ļ��б����ģʽö������
 */
typedef enum
{
    /*! ������� */
    LIST_UPDATE_NULL        = 0,
    /*! ������ҳ���У� */
    LIST_UPDATE_NEXT        = 1,
    /*! ������ҳ���У� */
    LIST_UPDATE_PREV        = 2,
    /*! ���¸��µ�ǰҳ */
    LIST_UPDATE_REUPDATE    = 3,
    /*! ��ͷ���£����β�л���ͷ */
    LIST_UPDATE_HEAD        = 4,
    /*! ��β���£����ͷ�л���β */
    LIST_UPDATE_TAIL        = 5,
} list_update_mode_e;

/*!
 * \brief
 *  list_menu_t �ļ�������˵��������ݽṹ
 */
typedef struct
{
    /*! �㼶������-1��ʾ�����㼶���� */
    uint8  layer;
    /*! Ӧ��ID */
    uint8  app_id;
    /*! �б�˵����ID */
    uint16 list_menu;
} list_menu_t;

/*!
 * \brief
 *  dir_com_data_t �ļ�������ؼ���ʼ���ṹ��
 */
typedef struct
{
    /*! 
     * ���ģʽ��0��ʾ��·���ĵ�һ��Ŀ¼�����1��ʾֱ������·��ָ��Ŀ¼�����
     * bit7 Ϊ1��ʾ�����Ŀ¼û���ļ�����ֲ˵���������
     */
    uint8 browse_mode;
    /*! �ļ�����˵��б���б�����Ŀ��һ��Ϊ1 */
    uint8 menulist_cnt;
    /*! ��ǰ���Ŀ¼�����ڵ�㼶��Ĭ��Ϊ0������Ϊ1����ӵ�1����Ŀ¼����ʱҪ�˳������ */
    uint8 root_layer;
    /*! �Ƿ�ɾ���ɹ��󷵻أ�TRUE��ʾ�ǣ�FALSE��ʾ�� */
    bool del_back;
    
    /*! option�Ӳ˵��ص����� */
    list_cb_option list_option_func;
    /*! �ļ�������˵��б� */
    list_menu_t *menulist;
} dir_com_data_t;

/*!
 * \brief
 *  del_com_data_t �ļ����ļ��У��б�ɾ���ؼ���ʼ���ṹ��
 */
typedef struct
{
    /*! ɾ���������ƣ�Unicode���������0xfeffǰ׺ */
    uint8 *filename;
    /*! ɾ���ļ����ļ�����ţ�Ϊ�ļ�ͬ�ļ����ܺ��е���� */
    uint16 del_no;
    /*! ɾ��ģʽ��0��ʾɾ���ļ���1��ʾɾ���ļ��л��б��� del_mode_e ���� */
    uint8 del_mode;
    /*! �Ƿ�ɾ�����ڲ����ļ���TRUE��ʾ�ǣ�FALSE��ʾ�񣨽���ɾ���ļ���Ч�� */
    bool del_playing;
    /*! �Ƿ�ɾ���ļ��л��б���ֻ��ɾ���ļ��л��б���Ч */
    bool del_self;
    uint8 reserve[3];
    /*! delete �Ի�����ѡ���ǡ���ִ�еĺ�����ΪNULL��ʾ����ִ�� */
    void (*del_func)(void);
} del_com_data_t;

/*!
 * \brief
 *  dir_control_t �ļ���������ƽṹ��
 */
typedef struct
{
    /*! ������������̷���DISK_C��ʾ���̣�DISK_H��ʾ���� */
    uint8 dir_disk;
    /*! �ļ�ѡ����Դ�����ͣ��� fsel_type_e ���� */
    uint8 file_source;
    /*! Ŀ¼ģ��ÿҳ���� */
    uint8 one_page_count;
    uint8 reserve;
    
    /*! ��ǰ�ļ���������ƽṹ�� */
    dir_com_data_t *dir_com;

    /*! ��Ŀ�б���Ч����top����ţ���������б����ţ�ֵ��1��ʼ */
    uint16 top;
    /*! ��Ŀ�б���Ч����bottom����ţ���������б����ţ�ֵ��1��ʼ */
    uint16 bottom;
    /*! ��Ŀ�б������� */
    uint16 list_total;
    
    /*! ��ǰ�����ļ���Ϣ */
    /*! ��ǰ�ļ���ţ���������б����ţ�ֵ��1��ʼ */
    uint16 list_no;
    /*! ��ǰ�б�ļ����ֵ��1 ~ one_page_count*/
    uint16 active;
    /*! ��ǰ�б�ɵļ����ֵ��1 ~ one_page_count */
    uint16 old;
    
    /*! �ļ��б����ģʽ���μ� list_update_mode_e ���� */
    list_update_mode_e update_mode;
    /*! �б�ؼ�����ģʽ���μ� list_draw_mode_e ���� */
    list_draw_mode_e draw_mode;
    /*! �б���Ҫ����Ŀ¼ͷ���ݱ�־ */
    bool enter_dir;
} dir_control_t;


/*!
 * \brief
 *  menu_title_action_t ���˵���ȷ���������ص�ִ�нṹ��
 */
typedef struct
{
    /*! ���ر����˵�style_id�����Ը���ui_menulist��������Ĭ��style_id����
     *  ��ʾ��-1��ʾ����Ĭ��style_id
     */
    uint16 style_id;
    /*! ���ض�̬�˵����ID��-1��ʾ���ò˵�ͷĬ�ϵĸ��˵�ID */
    uint16 menu_entry;
    /*! �˵����⣨��׼ȷ˵�ǲ˵���������ID��-1��ʾ���ò˵�ͷ�ַ���ID */
    uint16 title_id;
    /*! ���ؼ������������ַ���ID��-1��ʾ����Ĭ�ϼ����� */
    uint16 str_id;
    /*! ���ض�̬�˵����ַ���ָ�룬NULL��ʾ��Ч�����Ҳ������source_id��ֵ */
    uint8 *ret_str;
    /*! ���ز˵����ù����������ַ���ID��ָ��̬�˵��-1��ʾ�˵����� */
    uint16 source_id;
    /*! �Ǽ�����ǰ��ͼ���б�����ID��ֻ�� MENULIST_HEAD ģ����Ч */
    uint16 head_id;
    /*! ������ǰ��ͼ���б�����ID��ֻ�� MENULIST_HEAD ģ����Ч */
    uint16 head_act_id;
} menu_title_action_t;

/*!
 * \brief
 *  menu_com_data_t �˵��б�ؼ���ʼ���ṹ��
 */
typedef struct
{
    /*! Ӧ��ID */
    uint8 app_id;
    /*! ��ڲ˵�ID */
    uint8 menu_entry;
    /*! ·������ID */
    uint8 path_id;
    /*! ���ģʽ��0��ʾ����ڲ˵�0�㣬1��ʾֱ������֮ǰ�˳�ʱ������ */
    uint8 browse_mode;
    /*! �ײ�˵��ص����� */
    menu_cb_func menu_func;
} menu_com_data_t;

/*!
 * \brief
 *  menulist_stack_t �˵��б�Ƕ�׶�ջ�ṹ��
 */
typedef struct
{
    /*! ��ǰ��ڲ˵�ID */
    uint8 cur_entry;
    /*! ��ǰ��ڲ˵���Ӧ�߼��˵����ڵ�ID */
    uint8 ori_entry;
    /*! ��ǰ���˵�ID */
    uint8 menu_id;
    /*! ��ǰ�˵���� */
    uint8 layer;
} menulist_stack_t;

/*!
 * \brief
 *  menulist_control_t �˵��б���ƽṹ��
 */
typedef struct
{
    /*! Ĭ�ϲ˵�style���ͣ������� gui_menulist ʱ���ݽ�����style_id */
    uint16 style_id;
    /*! ��ǰ��ڲ˵�ջָ�� */
    uint8 stack_pointer;
    /*! ��ǰ���˵�ÿҳ���� */
    uint8 one_page_count;
    
    /*! ��ǰ�˵��б���ƽṹ�� */
    menu_com_data_t *menu_com;
    
    /*! ·������ID */
    uint8 path_id;
    /*! ·������㼶ID */
    uint8 layer_no;
    
    /*! ��ǰ���˵��б������� */
    uint16 total;
    /*! ��ǰ��ʾ�Ĳ˵��б�������� */
    uint16 top;
    /*! ��ǰ��ʾ�Ĳ˵��б�β����� */
    uint16 bottom;
    /*! ��ǰ������λ�� */
    uint16 list_no;
    /*! ��ǰѡ������� RADIO �˵���Ч��selectΪ-1��ʾ�� RADIO �˵� */
    uint16 select;
    /*! ��ǰ���������� */
    uint16 active;
    /*! ֮ǰ���������� */
    uint16 old;
    /*! �˵������ģʽ���μ� list_update_mode_e ���� */
    list_update_mode_e update_mode;
    /*! �б�UI����ģʽ���μ� list_draw_mode_e ���� */
    list_draw_mode_e draw_mode;
    /*! �б���Ҫ���²˵�ͷ���ݱ�־ */
    bool enter_menu;
} menulist_control_t;


/*!
 * \brief
 *  dialog_type_e �Ի�������ö������
 */
typedef enum
{
    /*! ���а���ѡ��ĶԻ��� */
    DIALOG_BUTTON_INCLUDE       = 0x00,
    /*! ���������ļ���ʾ�Ի��򣬵��������ⰴ���˳� */
    DIALOG_BUTTON_ANYKEY        = 0x01,
    /*! ���������ļ���ʾ�Ի��򣬵�����ȴ�2���Ӻ��˳� */
    DIALOG_INFOR_WAIT           = 0x02,
    /*! ���������ļ���ʾ�Ի��򣬵����������˳� */
    DIALOG_INFOR_IMMEDIATE      = 0x03,
} dialog_type_e;

/*!
 * \brief
 *  button_type_e �Ի��򰴼��������ö������
 */
typedef enum
{
    /*! ȷ����ȡ�� */
    BUTTON_OKCANCEl             = 0x00,
    /*! ��ֹ�����Ժͺ��� */
    BUTTON_ABORTRETRYIGNORE     = 0x01,
    /*! �ǡ����ȡ�� */
    BUTTON_YESNOCANCEL          = 0x02,//������ʷԭ����ʱŲ��ΪBUTTON_YESNO����NO��ť��ϢΪRESULT_DIALOG_NO
    /*! �Ǻͷ� */
    BUTTON_YESNO                = 0x03,
    /*! ���Ժ�ȡ�� */
    BUTTON_RETRYCANCEL          = 0x04,
    /*! û�а�ť */
    BUTTON_NO_BUTTON            = 0x05,
} button_type_e;

/*!
 * \brief
 *  button_list_item_t �����б���ṹ��
 */
typedef struct
{
    /*! �������� */
    uint8 button_cnt;
    /*! ����-��Ϣӳ�����飬���3������ */
    struct
    {
        /*! ������Դ�ַ���ID */
        uint16 str_id;
        /*! ������Դ�ַ���ID */
        uint16 result;
    } buttons[3];
} button_list_item_t;

/*!
 * \brief
 *  dialog_com_data_t �Ի���ؼ���ʼ���ṹ��
 */
typedef struct
{
    /*! �Ի������ͣ��μ� dialog_type_e ���� */
    uint8 dialog_type;
    /*! �Ի��򰴼�������ͣ��μ� button_type_e ���� */
    uint8 button_type;
    /*! �Ի����ʶͼ��ID��ֵΪ-1��ʾ����ʾ */
    uint16 icon_id;
    /*! ��ϸ˵���ַ��������� */
    string_desc_t *string_desc;
    /*! Ĭ�ϼ������ */
    uint8 active_default;
    /*! �Ի���ˢ��ģʽ������Ӧ�ò�ʹ�� */
    dialog_draw_mode_e draw_mode;
    /*! �����ֽ� */
    uint8 reserve[2];
} dialog_com_data_t;


/*!
 * \brief
 *  param_com_data_t �������ÿؼ���ʼ���ṹ��
 */
typedef struct
{
    /*! ParamBox˽�����ݽṹָ�� */
    parambox_private_t *private_data;
    /*! ParamBoxˢ��ģʽ������Ӧ�ò�ʹ�� */
    parambox_draw_mode_e draw_mode;
} param_com_data_t;


/*!
 * \brief
 *  headbar_icon_e ״̬��ͼ��IDö������
 */
typedef enum
{
    /*! ����ͼ�� */
    HEADBAR_ICON_MUSIC      = 0,
    /*! ��Ƶͼ�� */
    HEADBAR_ICON_VIDEO      = 1,
    /*! ͼƬͼ�� */
    HEADBAR_ICON_PICTURE    = 2,
    /*! ����ͼ�� */
    HEADBAR_ICON_RADIO      = 3,
    /*! �����ͼ�� */
    HEADBAR_ICON_BROWSER    = 4,
    /*! ����ͼ�� */
    HEADBAR_ICON_SETTING    = 5,
    /*! ¼��ͼ�� */
    HEADBAR_ICON_RECORD     = 6,
    /*! ������ͼ�� */
    HEADBAR_ICON_KEYLOCK    = 7,
    /*! ��Ƶͼ�� for video */
    HEADBAR_ICON_VIDEO_V    = 8,
    /*! �����ͼ�� for video */
    HEADBAR_ICON_BROWSER_V  = 9,
    /*! ������ͼ�� for video */
    HEADBAR_ICON_KEYLOCK_V  = 10,
    /*! ����ʾ������MainMenu */
    HEADBAR_ICON_NONE       = 11,
} headbar_icon_e;

/*!
 * \brief
 *  headbar_style_t ״̬����ʾԪ�ؽṹ��
 */
typedef struct
{
    /*! ����ͼ�� style id��-1��ʾ����ʾ����ͼ�� */
    uint16 bg_sty;
    /*! Ӧ��ͼ�� style id��-1��ʾ����ʾӦ��ͼ�� */
    uint16 icon_sty;
    /*! ϵͳʱ�� style id��-1��ʾ����ʾϵͳʱ�� */
    uint16 time_sty;
    /*! ���ͼ�� style id��-1��ʾ����ʾ���ͼ�� */
    uint16 battery_sty;
    /*! ����״̬ͼ�� style id��-1��ʾ����ʾ����״̬ͼ�� */
    uint16 cable_sty;
    /*! ��״̬ͼ�� style id��-1��ʾ����ʾ��״̬ͼ�� */
    uint16 card_sty;
} headbar_style_t;

/*!
 * \brief
 *  headbar_mode_e ״̬����ʾģʽö������
 */
typedef enum
{
    /*! ����ʾ״̬�� */
    HEADBAR_MODE_NULL       = 0,
    /*! ��ʾĬ��״̬�� */
    HEADBAR_MODE_NORMAL     = 1,
} headbar_mode_e;

/*!
 * \brief
 *  headbar_update_e ״̬������ģʽö������
 */
typedef enum
{
    /*! ��ʼ����״̬�� */
    HEADBAR_UPDATE_INIT     = 0,
    /*! ȫ��ȫ��״̬�� */
    HEADBAR_UPDATE_ALL      = 1,
    /*! ���±仯�˵�״̬�� */
    HEADBAR_UPDATE_CHANGE   = 2,
} headbar_update_e;


/*!
 * \brief
 *  screen_save_mode_e ��Ļ��������ö������
 */
typedef enum
{
    /*! �ر���Ļ���� */
    SCREEN_NULL             = 0x00,
    /*! ����ʱ�ӣ���ʾ���ں�ʱ�� */
    SCREEN_DIGIT_CLOCK      = 0x01,
    /*! ����ר��ͼƬ���������ǰ̨Ӧ�ò������֣��л�������ʱ�� */
    SCREEN_ALBUM_ART        = 0x02,
    /*! �ص�lcd����� */
    SCRREN_SCREEN_OFF       = 0x03,
    /*! ��ʾģʽ����ʾ����Ӧ��Demo���� */
    SCREEN_DEMO_MODE        = 0x04,
} screen_save_mode_e;

/*!
 * \brief
 *  key_lock_mode_e ������UI����ö������
 */
typedef enum
{
    /*! û�а����� */
    KEY_LOCK_MODE_NULL      = 0x00,
    /*! �������������� */
    KEY_LOCK_MODE_LOCKING   = 0x01,
    /*! ��������ס��ʾ */
    KEY_LOCK_MODE_HOLD      = 0x02,
    /*! �������������� */
    KEY_LOCK_MODE_UNLOCKING = 0x03, 
} key_lock_mode_e;


/*!
 * \brief
 *  animation_com_data_t ������ʾ��ʼ���ṹ��
 */
typedef struct
{
    /*! ÿ֡ͼƬ��ʾʱ��������λ��1ms */
    uint16 interval;
    /*! �������򣬿���ѡ������0���ͷ���1�� */
    uint8 direction;
    /*! �Ƿ�������ֹ��TRUE��ʾ��������ֹ��FALSE��ʾ������ֹ */
    uint8 forbid;
    /*! ��ֹ������ʾ�İ���ӳ���б�ΪNULL��ʾ�������п�ݼ���ֹ���� */
    const key_map_t *key_map_list;
    /*! ÿ֡������ִ�лص����� */
    animation_callback callback;
} animation_com_data_t;


/*!
 * \brief
 *  ext_show_param_t �ı���ʾ�����ṹ��
 */
typedef struct
{
    /*! ��ʾһҳ���� */
    uint8 line_count_one_page;
    /*! ��ʾһ�����ص��� */
    uint8 max_width_one_line;
    /*! ��ʾģʽ���������˿��У��ִ���ʾ�� */
    uint8 mode;
    /*! �������� */
    int8  language;
} text_show_param_t;

/*!
 * \brief
 *  text_buf_t �ı��Ķ����뻺�����ṹ��
 */
typedef struct
{
    /*! һ���ַ��������� */
    uint8 text_prev[BUFF_ONE_ROW];
    /*! һ��������������512���ֽ� */
    uint8 text_buf[FILE_SECTOR];
    /*! ����������Ч����ָ�� */
    uint8 *text_buf_valid;
    /*! ����������Ч���ݳ��� */
    uint16 text_buf_length;
    /*! ��������ʣ���ֽ��� */
    uint8 remain;
    /*! buffer �����Ƿ���Ч */
    bool valid;
    /*! �ı���ʾ�����ṹ�� */
    text_show_param_t param;
    /*! text ��ʾ�лص����� */
    void (*text_show_line)(string_desc_t *string_desc, uint8 index);
} text_decode_t;

/*!
 * \brief
 *  text_file_t �ı��Ķ������ļ��ṹ�壬Ϊ�˼���SD�ļ����ļ�ϵͳ�ļ�
 */
typedef struct
{
    /*! �򿪵��ļ���� */
    void* file_handle;
    /*! �ļ��������� */
    uint32 file_sectors;
    /*! �ļ����ֽ��� */
    uint32 file_length;
    /*���ļ�ƫ�Ƶ�ַ�����ֽ�Ϊ��λ */
    uint32 file_offset;
    /*! �ļ���λ�ӿں���ָ�룬������Ϊ��λ */
    bool (*fseek) (void* file_handle, uint8 where, int32 sector_offset);
    /*! �ļ����ݶ��ӿں���ָ�룬������Ϊ��λ */
    bool (*fread) (void* file_handle, uint8 *text_buf, uint32 sector_count);
} text_file_t;

/*!
 * \brief
 *  gui_direct_e gui ��ʾ����ö������
 */
typedef enum
{
    /*! С���������ڴ˷���Ϊ���򣨿�128*��160�� */
    GUI_DIRECT_NORMAL       = 0,
    /*! С��˳ʱ����ת90�ȷ��򣬺��򣨿�160*��128�� */
    GUI_DIRECT_HORIZONTAL   = 1,
} gui_direct_e;


/*!
 * \brief
 *  sys_comval ָ��ȫ�� comval ������������ Common ģ����� comval
 */
extern comval_t *sys_comval;
/*!
 * \brief
 *  sys_counter_timer_id ϵͳ���ܶ�ʱ��id
 */
extern int8 sys_counter_timer_id;
/*!
 * \brief
 *  sys_status_timer_id ϵͳ״̬���¶�ʱ��id
 */
extern int8 sys_status_timer_id;
/*!
 * \brief
 *  g_gui_direct ϵͳ gui ���򣬺�����������
 */
extern gui_direct_e g_gui_direct;

/*!
 * \brief
 *  g_light_timer_bk ���ݱ���ʱ��
 */
extern uint8 g_light_timer_bk;

 
//�ļ�������ļ�ɾ���ӿ���������ϸ˵�����ӿڶ��壩
extern dir_brow_t g_dir_browser;
extern file_brow_t g_file_list[LIST_NUM_ONE_PAGE_MAX];
extern menu_title_data_t dir_menu_title;
extern dir_control_t dir_control;
extern dir_control_t last_dir_control;
extern uint16 speed_control;
extern uint16 line_cnt;
extern app_result_e gui_directory (uint16 style_id, file_path_info_t *path_info, dir_com_data_t *dir_com) __FAR__;
extern app_result_e gui_delete (file_path_info_t *path_info, del_com_data_t *del_com) __FAR__;

//�˵��б�ӿ���������ϸ˵�����ӿڶ��壩
extern menu_title_data_t g_menu_title;
extern menu_item_data_t g_menu_items[LIST_NUM_ONE_PAGE_MAX];
extern back_to_parent g_back_to_parent_func;
extern uint8 *g_back_to_parant_arg;
extern app_result_e gui_menulist (uint16 style_id, menu_com_data_t *menu_com) __FAR__;
extern app_result_e gui_menulist_simple (uint16 style_id, menu_com_data_t *menu_com) __FAR__;

#ifdef PC
#define inline
#endif
//���÷��ظ��˵��ص�����
static inline void gui_menulist_set_back2parent(back_to_parent func, uint8 *arg_addr)
{
    g_back_to_parent_func = func;
    g_back_to_parant_arg = arg_addr;
}

/*!
 * \brief
 *  ר��ͼƬ�ṹ�嶨��
 */
typedef struct
{
    /*! ����ǶͼƬĿ¼��غ� */
    uint32 cluster_no;
    /*! ����ǶͼƬ���Ŀ¼��ƫ�ƣ���ǶͼƬ����ļ�ƫ�� */
    uint32 offset;
    /*! ��ʾ���ߴ����� */
    region_t region;
    /*! ͼƬ���� */
    uint32 apic_type;
} album_picinfo_t;

/*!
 * \brief  
 *  ˢר��ͼƬ�ص�����
 */
typedef struct
{
    void (*func)(album_picinfo_t *album_picinfo);
    album_picinfo_t *arg;
}com_albumart_cb_t;

#ifdef PC
typedef menu_cb_func (*__get_menu_func)(uint16 major_id, uint16 minor_id);
typedef menu_cb_leaf (*__get_callback_func)(uint16 major_id, uint16 minor_id);
typedef menu_cb_option (*__get_menu_option)(uint16 major_id, uint16 minor_id);

extern __get_menu_func this_get_menu_func;
extern __get_callback_func this_get_callback_func;
extern __get_menu_option this_get_menu_option;

extern void load_menulist_simfunc(__get_menu_func func1, __get_callback_func func2, __get_menu_option func3);
#endif

//�Ի���ӿ���������ϸ˵�����ӿڶ��壩
extern bool usb_selecting_state;
extern uint8 g_usb_auto_select;
extern int8 g_usb_timer_id;
extern app_result_e gui_dialog (uint16 style_id, dialog_com_data_t *dialog_com) __FAR__;
extern app_result_e gui_dialog_msg (uint16 style_id, dialog_type_e type, uint16 str_id) __FAR__;
extern app_result_e gui_usbconnect (void) __FAR__;

//�������ÿ�ӿ���������ϸ˵�����ӿڶ��壩
extern int16 volume_value_last;
extern uint8 volume_enter_forbidden;
extern bool g_vol_time_up_flag;
extern app_result_e gui_set_parameter (uint16 style_id, param_com_data_t *param_com) __FAR__;
extern app_result_e gui_volumebar (uint8 *volume, uint8 *limit, uint8 mode) __FAR__;

//�ı��Ķ��ؼ��ӿ���������ϸ˵�����ӿڶ��壩
extern text_end_mode_e text_decode_one_page(text_decode_t *text_decode, text_file_t *text_file, \
uint16 *page_bytes) __FAR__;
extern app_result_e gui_text_read(uint16 style_id, const char* file_name) __FAR__;

//״̬���ӿ���������ϸ˵�����ӿڶ��壩
extern uint8 g_battery_display;
extern uint8 g_icon_id;
extern uint8 g_sys_minute;
extern uint8 g_battery_value;
extern uint8 g_headbar_update;
extern headbar_style_t this_headbar_style;
extern headbar_icon_e this_headbar_icon_id;
extern headbar_mode_e this_headbar_mode;
extern void gui_set_headbar_mode(headbar_mode_e mode, headbar_icon_e icon_id) __FAR__;
extern void gui_get_headbar_mode(headbar_mode_e *mode, headbar_icon_e *icon_id) __FAR__;
extern app_result_e gui_headbar(headbar_update_e update) __FAR__;
extern void gui_headbar_handle(void) __FAR__;

//��Ļ���������������ػ�UI�ӿ���������ϸ˵�����ӿڶ��壩
extern uint8 g_so_time;
extern uint8 g_ss_tag_backup;
extern uint8 g_ss_mode_backup;
extern uint8 g_ss_icon_backup;
extern gui_direct_e g_ss_direct_bk;
extern int8  g_ss_timer_id;
extern uint8 g_ss_clk_backup;
extern screen_save_mode_e g_ss_mode;
extern uint8 g_ss_demo_mode_phase;
extern uint8 g_ss_demo_frame_cnt;
extern uint8 g_ss_album_art_phase;
extern uint8 g_ss_last_date;
extern uint8 g_ss_last_time;
extern bool g_ss_vip_event;
extern uint8 g_ss_delay_flag;
extern bool g_kl_time_is_up;
extern com_albumart_cb_t g_com_paint_cb;
extern app_result_e gui_screen_save_enter (screen_save_mode_e ss_mode) __FAR__;
extern app_result_e gui_screen_save_exit (void) __FAR__;
extern app_result_e gui_keylock (bool lockkey) __FAR__; 
extern app_result_e gui_shut_off (void) __FAR__;

//������ʾ�ӿ���������ϸ˵�����ӿڶ��壩
extern animation_callback g_anm_callback;
extern style_infor_t *g_anm_sty_infor;
extern uint8 g_anm_index;
extern uint8 g_anm_direction;
extern app_result_e gui_animation(style_infor_t *style_infor, animation_com_data_t *anm_com) __FAR__;
extern app_result_e gui_logo (style_infor_t *style_infor, animation_com_data_t *anm_com) __FAR__;

//����ܽӿ���������ϸ˵�����ӿڶ��壩
extern app_result_e com_app_sleep(uint32 sleep_timer, const key_map_t *key_map_list) __FAR__;
extern void sys_timer_init(void) __FAR__;
extern void sys_timer_exit(void) __FAR__;
extern void sys_counter_handle(void) __FAR__;
extern void sys_status_handle(void) __FAR__;
extern app_result_e com_message_box(msg_apps_type_e msg_type) __FAR__;
extern void com_get_default_setting(setting_comval_t *setting_comval) __FAR__;
extern void com_set_sys_comval(comval_t *comval) __FAR__;
extern void com_set_gui_direction(gui_direct_e direct) __FAR__;
extern gui_direct_e com_get_gui_direction(void) __FAR__;
extern app_result_e com_alarm_msg_dispatch(void) __FAR__;
extern void set_timer_msg(alarm_fm_t *aufm_temp, alarm_alarm_t *alarm_temp, uint8 *timer_flag) __FAR__;
extern uint8 com_get_battery_grade(bool now) __FAR__;
extern void com_set_contrast(uint8 lightness) __FAR__;
extern void com_app_msg_hook(private_msg_t *private_msg) __FAR__;

#endif
