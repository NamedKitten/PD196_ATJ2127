/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-8 16:53     1.0             build this file 
*******************************************************************************/
/*!
 * \file     common_func.h
 * \brief    ���ÿؼ�ģ��������ݽṹ���궨�壬�ӿ�������
 * \author   lzcai
 * \version  1.0
 * \date     2011-9-8
*******************************************************************************/

#ifndef _common_func_H_
#define _common_func_H_

#include "psp_includes.h"
#include "enhanced.h"
#include "case_type.h"
#include "gui_msg.h"
#include "app_msg.h"
#include "setting_common.h"

/*! �˵�·���������� */
#define PATH_HISTORY_MENU   0
/*! �ļ����·���������� */
#define PATH_HISTORY_DIR    1

/*! Ҷ�Ӳ˵�������ֵtype[0-14] = 0x7fff */
#define MENU_TYPE_LEAF      0x7fff
/*! ��ڲ˵�ͷ��father_indexֵ -1��ʾ��ڲ˵��� */
#define MENU_TYPE_ENTRY     0xffff

/*! ������...���ַ�����󳤶ȣ���Ϊ 84 ���ص� */
#define ELLIPSIS_MAX_LEN    84

/*!
 * \brief  
 *  menu_cb_func �˵���ȷ��ִ�лص�����
 * \li  param: ָ����˵���ȷ���������ص�ִ�нṹ�� menu_title_action_t
 */
typedef app_result_e (*menu_cb_func)(void *param);

/*!
 * \brief  
 *  menu_cb_leaf Ҷ�Ӳ˵���ʱ�ص�����
 */
typedef void (*menu_cb_leaf)(void);

/*!
 * \brief  
 *  menu_cb_option �˵���option�Ӳ˵��ص�����
 */
typedef app_result_e (*menu_cb_option)(void);

/*!
 * \brief  
 *  back_to_parent ���ظ��˵��ص�����
 */
typedef void (*back_to_parent)(void);

/*!
 * \brief  
 *  list_cb_option �ļ������option�Ӳ˵��ص�����
 * \li  path_info �ļ��������ȡ���ĵ�ǰ�ļ�/�ļ��� location
 * \li  list_no �ļ��������ǰ������
 */
typedef app_result_e (*list_cb_option)(file_path_info_t *path_info, uint16 list_no);

/*!
 * \brief  
 *  param_adjust_func ����ֵ�������ص��������������������������õ�
 * \li  params ���������ָ��
 * \li  active ��ǰ�������ò��������
 * \li  inc ��ǰ�����Ƿ�Ϊ����ֵ����
 */
typedef adjust_result_e (*param_adjust_func)(parambox_one_t *params, uint8 active, bool inc);

/*!
 * \brief  
 *  param_cb_func ���ü�ʱ�ص������������������õ�
 * \li  value ��ǰ����ֵ
 */
typedef bool (*param_cb_func)(int16 value);

/*!
 * \brief  
 *  sound_start_func �����豸��������ص�����
 * \li  volume ��ǰ����ֵ
 */
typedef bool (*sound_start_func)(uint8 volume);

/*!
 * \brief  
 *  sound_stop_func ֹͣ�豸��������ص�����
 * \li  volume ��ǰ����ֵ
 */
typedef bool (*sound_stop_func)(void);

/*!
 * \brief
 *  history_item_t ·��������ṹ��
 */
typedef struct
{
    /*! ��ǰ��ʾ�б������ţ�0xffff��ʾ��Ч */
    uint16 top;
    /*! �������ڵ�ǰ��ʾ�б��е�λ�ã�0xffff��ʾ��Ч */
    uint16 list_no;
} history_item_t;

/*!
 * \brief
 *  history_index_t ·�����������ṹ��
 */
typedef struct
{
    /*! ·�����ͣ�0��ʾ�˵��б�1��ʾ�ļ���� */
    uint8 type;
    /*! Ӧ��ID��ȡֵ��Χ��0 ~ 31 */
    uint8 app_id;
    /*��·������ID��ȡֵ��Χ��0 ~ 7(3) */
    uint8 path_id;
    /*! ��ǰ�㼶��ȡֵ��Χ��0 ~ 7(15) */
    uint8 layer;
} history_index_t;

/*!
 * \brief  
 *  menu_item_type_e���˵�������ö������
 */
typedef enum
{
    /*! ��ͨ�˵��� */
    NORMAL_MENU_ITEM        = 0,
    /*! ��ѡ��ť�˵��� */
    RAIDO_MENU_ITEM         = 1,
} menu_item_type_e;

//����Ӧ�ÿ�����Ա��д�˵�������ʹ�ã�pc�˵����ù��߿�ʶ��
/*!
 * \brief  
 *  conf_entry_head_t �˵����ýű��ļ�����ڲ˵�ͷ���ṹ��
 */
typedef struct
{
    /*! ��ڲ˵���ʶ�ַ�������ʾ��ڲ˵�������Ŀ�ʼ��Ĭ��Ϊ ENTRY MENU */
    uint8  key_string[14];
    /*! ��ڲ˵����������� */
    uint16 total;
} conf_entry_head_t;

/*!
 * \brief  
 *  conf_menu_entry_t ��ڲ˵�������ṹ��
 */
typedef struct
{
    /*! ��ڲ˵�������������� */
    uint16 menu_id;
    /*! ��ڲ˵���������ʾ����ID--��ID */
    uint16 major_id;
    /*! ��ڲ˵���������ʾ����ID--��ID�����minor_id��0��pc������ʹ��minor_id��ʾ������ʹ��major_id������ʾ */
    uint16 minor_id;
} conf_menu_entry_t;

/*!
 * \brief  
 *  conf_item_head_t �˵����ýű��ļ��Ĳ˵���ͷ���ṹ��
 */
typedef struct
{
    /*! �˵���ʶ���־�ַ�������ʾ�˵����Ŀ�ʼ */
    uint8  key_string[14];
    /*! �˵������� */
    uint16 total;
} conf_item_head_t;

/*!
 * \brief  
 *  conf_menu_item_t �˵���ṹ��
 */
typedef struct
{
    /*! �˵�����ʾ�ַ�ID--��ID */
    uint16 major_id;
    /*! �˵�����ʾ�ַ�ID--��ID�����minor_id��0��pc������ʹ��minor_id��ʾ������ʹ��major_id������ʾ */
    uint16 minor_id;
    /*! �˵����ܺ��� */    
    menu_cb_func menu_func;  
    /*! ʵʱ�˵��ص�����,ΪNULL ��ʾ��ʵʱ�˵� */      
    menu_cb_leaf callback; 
    /*! �˵�option�Ļص����� */
    menu_cb_option menu_option;
    /*! �˵������ͣ���ϸ�μ� menu_item_type_e ���� */
    uint8 type;
    /*! �˵���������ţ����˵��༭����ʱʹ��*/
    uint8 item_id;
    /*! ͼƬ�б�index������ǰ��ͼ�������˵���󶨵����*/
    uint8 pic_index;
    /*! �����ֽ� */
    uint8 reserve;
} conf_menu_item_t;

//С���ڲ�ʹ�õ����ݽṹ����pc�˵����ù�������
/*!
 * \brief
 *  menu_title_data_t �˵�ͷ���ݽṹ
 */
typedef struct
{
    /*! �˵�ͷ��Դ�ַ���ID */
    uint16 str_id;
    /*! ��ǰ�˵��Ĳ˵������ */
    uint16 count;
    /*! �˵�item data ��ʼ��ַ */
    uint16 offset;
    /*! Ĭ�ϵĲ˵���ͨ���������� */
    uint16 active_default;
    /*! ��ǰ�˵�����һ���˵��������������ǰ�˵�Ϊ��ڸ��˵�������Ϊ0x7fff */
    uint16 father_index;
    /*! ��ǰ�˵��б��Ӧ�ĸ��˵����ڸ��˵��еı�� */
    uint16 father_active;
} menu_title_data_t;

/*!
 * \brief
 *  menu_title_data_t �˵������ݽṹ
 */
typedef struct
{
    /*! bit<15>��ʾ�˵��Ƿ�Ϊradio��ť��0��ʾ��1��ʾ��
     *  bit<14-0>��ʾ�˵������ͣ�=0x7fff����ʾҶ�Ӳ˵�; ����ֵ��ʾ���˵���ֵ��ʾָ��Ĳ˵�ͷ����ֵ
     */
    uint16 child_index;     
    /*! �˵�����Դ�ַ���ID */
    uint16 str_id;
    /*! �˵���ʾ�ַ���ID������ID��ͬ������£�ģ��������Ҫʹ�ô�ID�ҵ������ص������ĵ�ַ */
    uint16 str_id_sub; 
    /*! ͼƬ�б�index������ǰ��ͼ�������˵���󶨵���� */
    uint8 pic_index;
    /*! �����ֽ� */
    uint8 reserve; 
    /*! �˵���ȷ��ִ�лص�����
     *  ���ڲ˵�ͷ�����ֵ���������� RESULT_NULL ���ɣ���Ҷ�Ӳ˵�������Ҫ����ѡ�񷵻�ֵ
     */
    menu_cb_func menu_func;
    /*! ��ʱҶ�Ӳ˵��ص���������NULL��ʾ��Ҷ�Ӳ˵�Ϊ��ʱҶ�Ӳ˵���NULL��ʾ���Ǽ�ʱҶ�Ӳ˵� */
    menu_cb_leaf callback;
    
    /*! PHILIPS����֧�֣��˵���option�ص����� */
    menu_cb_option menu_option;  
} menu_item_data_t;


/*!
 * \brief
 *  key_map_t ������gui����Ϣӳ��ṹ��
 */
typedef struct
{
    /*! Դ�����¼� */
    key_event_t key_event;
    /*! ӳ�� gui �¼� */
    msg_apps_type_e event;
} key_map_t;

/*!
 * \brief
 *  conf_type_e ���ýű��ļ����
 */
typedef enum
{
    /*! �ַ������� */
    CONF_TYPE_STRING        = 0,
    /*! ������ֵ���� */
    CONF_TYPE_NUM_LINEAR    = 1,
    /*! ��������ֵ���� */
    CONF_TYPE_NUM_NONLINEAR = 2,
} conf_type_e;

/*!
 * \brief
 *  config_index_t �������������ݽṹ
 */
typedef struct
{
    /*! ��������������ļ�ͷ��ƫ�� */
    uint32 offset;
    /*! ���������ݳ��� */
    uint32 length;
} config_index_t;

/*!
 * \brief
 *  config_string_t �ַ������������ݽṹ
 */
typedef struct
{
    /*! �ַ������ݣ��ɱ䳤���飬������txt����һ�£���'\0'���� */
    uint8 value[1];
} config_string_t;

/*!
 * \brief
 *  config_linear_t ������ֵ���������ݽṹ
 */
typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ȡֵ�������Сֵ */
    uint16 min;
    /*! ȡֵ��������ֵ */
    uint16 max;
    /*! ���� */
    uint16 step;
} config_linear_t;

/*!
 * \brief
 *  config_nonlinear_t ��������ֵ���������ݽṹ
 */
typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! ��Чֵ���飬�ɱ䳤���� */
    uint16 value[1];
} config_nonlinear_t;

/*!
 * \brief
 *  soundout_state_e �������״̬ö������
 */
typedef enum
{
    /*! ��ʼ�������������pa */
    SOUND_OUT_START         = 0,
    /*! ֹͣ����������ص�pa */
    SOUND_OUT_STOP          = 1,
    /*! ��ͣ�������������pa */
    SOUND_OUT_PAUSE         = 2,
    /*! �ָ��������������pa */
    SOUND_OUT_RESUME        = 3,
} soundout_state_e;


//�����ò˵��ӿ�����
extern sd_handle confmenu_fp;
extern uint32 confmenu_offset;
extern bool com_open_confmenu_file(const char* file_name) __FAR__;
extern bool com_close_confmenu_file(void) __FAR__;
extern bool com_get_confmenu_title(uint8 menu_id, menu_title_data_t* confmenu_title) __FAR__;
extern bool com_get_confmenu_item(menu_title_data_t* confmenu_title, uint16 item_index, \
menu_item_data_t *confmenu_item) __FAR__;
extern uint16 com_get_confmenu_active(menu_title_data_t* confmenu_title, uint16 key_str) __FAR__;

//·��������ӿ�����
extern bool dir_history_valid;
extern bool menu_history_valid;
extern uint8 *dir_history_addr;
extern uint8 *menu_history_addr;
extern bool com_get_history_item(history_index_t *index, history_item_t *history) __FAR__;
extern bool com_set_history_item(history_index_t *index, history_item_t *history) __FAR__;
extern bool com_update_path_history(history_index_t *index) __FAR__;
extern bool com_clear_all_history(uint8 type) __FAR__;
extern bool com_clear_app_history(uint8 type, uint8 app_id, uint8 disk) __FAR__;

//����ӳ��ӿ�����
extern bool com_key_mapping(input_gui_msg_t *input_msg, msg_apps_type_e *gui_event, \
const key_map_t *key_map_list) __FAR__;

//������ӿ�����
extern bool com_open_config_file(const char* config_file) __FAR__;
extern bool com_close_config_file(void) __FAR__;
extern bool com_get_config_struct(uint16 config_id, uint8 *buf, uint16 buf_len) __FAR__;
extern uint16 com_get_config_default(uint16 config_id) __FAR__;//��������ֵ�Ͳ���

//�ַ�������ӿ�����
extern bool com_string_format(string_desc_t *pattern, string_desc_t *strings) __FAR__;
extern void com_dot_to_shortname(uint8 *short_name) __FAR__;
extern void com_ansi_to_unicode(uint8 *file_name) __FAR__;
extern uint16 com_ellipsis_to_longname(uint8 *long_name, uint8 *ellipsis_name, uint16 ellipsis_len) __FAR__;
extern void com_dump_debug(uint16 x, uint16 y, uint8 *format, uint32 value) __FAR__;

//common key����ӿ�����
extern uint8 this_filter_key_value;
extern bool com_gui_msg_hook(input_gui_msg_t *input_msg) __FAR__;
extern void com_filter_key_hold(void) __FAR__;

//�����������ӿ�����
extern bool g_keytone_backup;
extern bool com_set_sound_keytone(uint8 kt_enable) __FAR__;
extern bool com_set_sound_out(bool sound_out, soundout_state_e state, void *sound_func) __FAR__;
extern bool com_set_sound_volume(uint8 volume) __FAR__;
extern bool com_set_sound_record(bool on_off, uint8 type) __FAR__;

//����ӳ���
extern const key_map_t delete_key_map_list[];
extern const key_map_t dialog_key_map_list[];
extern const key_map_t directory_key_map_list[];
extern const key_map_t keylock_list[];
extern const key_map_t menulist_key_map_list[];
extern const key_map_t parameter_key_map_list[];
extern const key_map_t shutoff_key_map_list[];
extern const key_map_t textread_key_map_list[];
extern const key_map_t usbconnect_key_map_list[];
extern const key_map_t volume_key_map_list[];
extern const key_map_t limit_key_map_list[];
extern const key_map_t key_shortcut_list[];

#endif
