/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-ext in rom
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-10-12 11:04     1.0             build this file 
*******************************************************************************/
/*!
 * \file     common_ui_data.c
 * \brief    common ui ȫ������
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2011-10-12
*******************************************************************************/

#include "common_ui.h"

//����ȫ�ֱ���
comval_t *sys_comval;       //ϵͳ����ָ�룬ͨ�� com_set_sys_comval �ӿڴ��ݸ� common ��������ȫ��ָ��
int8 sys_counter_timer_id;  //ϵͳ��ʱ���ܣ��������⣬������ʡ��ػ�����ʱ�ػ����ȣ���ʱ�� timer id
int8 sys_status_timer_id;   //ϵͳ״̬�����ʱ�� timer id
gui_direct_e g_gui_direct;  //��ǰ gui ��ʾ����

///headbar ȫ�ֱ���
uint8 g_battery_display;    //�ڳ��ʱ������ʾ�õĵ���
uint8 g_icon_id;            //Ӧ��ͼ��frame id
uint8 g_sys_minute;         //���ϵͳʱ�����ֵ
uint8 g_battery_value;      //��ص����������޳��ʱ��ʾ��ص���
uint8 g_headbar_update;     //headbar ˢ��ģʽ
headbar_style_t this_headbar_style; //headbar style��Ͻṹ��
headbar_icon_e this_headbar_icon_id;//headbar ��ǰ��ʾӦ��ͼ��ID
headbar_mode_e this_headbar_mode;   //headbar ��ǰ��ʾģʽ

///gui_menulist ȫ�ֱ���
menu_title_data_t g_menu_title; //��ǰ�˵�ͷ�ṹ��
menu_item_data_t g_menu_items[LIST_NUM_ONE_PAGE_MAX];   //��ǰ�˵��б�������
back_to_parent g_back_to_parent_func;   //�˵����ð����ؼ��Ƿ�Ҫ���ûص�����
uint8 *g_back_to_parant_arg;    //�˵����ð����ؼ��Ƿ�Ҫ���ûص�������־

///gui_directory ȫ�ֱ���
dir_brow_t g_dir_browser;       //Ŀ¼ͷ���б�ͷ�ṹ��
menu_title_data_t dir_menu_title;   //Ƕ��˵�ͷ���ṹ
dir_control_t dir_control;      //�ļ���������ƽṹ��
dir_control_t last_dir_control; //���ݵ�ǰĿ¼���ƽṹ�壬�����Ϳ�������һ��Ŀ¼�����½����Ŀ¼
file_brow_t g_file_list[LIST_NUM_ONE_PAGE_MAX];//�ļ��������ǰ�б�������
uint16 speed_control;   //���ڿ����б��л��ٶȵļ�������
uint16 line_cnt;        //��ǰ�б��л�Ҫ�л�����

///������ȫ�ֱ���
int16 volume_value_last;        //��һ�λ���������ʱ����ֵ
uint8 volume_enter_forbidden;   //�����ﵽ�������ƣ���ֹ������������
bool g_vol_time_up_flag;        //������ 4 ���ް����Զ��˳���־

///USB ѡ��Ի���ȫ�ֱ���
bool usb_selecting_state;   //�Ƿ����� USB ѡ��Ի�����
uint8 g_usb_auto_select;    //USB ѡ���Զ�ѡ���ʱ����
int8 g_usb_timer_id;        //USB ѡ���Զ�ѡ��timer id

///������ʾȫ�ֱ��������ڶ�ʱˢ����֡
animation_callback g_anm_callback;  //ÿ֡����ˢ��֮��ִ�еĻص�����
style_infor_t *g_anm_sty_infor;     //������Ӧ picbox style�ṹ��
uint8 g_anm_index;          //����֡��ż�������
uint8 g_anm_direction;      //������ʾ���򣬷ִ�ͷ��β�ʹ�β��ͷ

///�ػ�ȷ��UIȫ�ֱ���
uint8 g_so_time;        //�ػ�ȷ����ʱ����

///��Ļ����ȫ�ֱ���
uint8 g_ss_tag_backup;  //��������ǰ����tag���ݣ��Ա��˳�����ʱ�ָ�
uint8 g_ss_mode_backup; //��������ǰ headbar ģʽ���ݣ��Ա��˳�����ʱ�ָ�
uint8 g_ss_icon_backup; //��������ǰ headbar Ӧ��ͼ��ID���ݣ��Ա��˳�����ʱ�ָ�
int8  g_ss_timer_id;    //����UI��ʱ��timer id
uint8 g_ss_clk_backup;  //��������ǰ MCU ��Ƶ���ݣ��Ա��˳�����ʱ�ָ�
gui_direct_e g_ss_direct_bk;    //��������ǰ gui ���򱸷ݣ��Ա��˳�����ʱ�ָ�
screen_save_mode_e g_ss_mode;   //��ǰ����ģʽ
uint8 g_ss_demo_mode_phase;     //����ģʽΪ��ʾģʽʱ����ʾͼƬ�л���������
uint8 g_ss_demo_frame_cnt;      //����ģʽΪ��ʾģʽʱ����ʾͼƬ����
uint8 g_ss_album_art_phase;     //����ģʽΪר��ͼƬʱ��ר��ͼƬ�л���������
uint8 g_ss_last_date;   //����ģʽΪ����ʱ��ʱ��������ڱ��ݣ��Ա��ж��Ƿ������ڸ���
uint8 g_ss_last_time;   //����ģʽΪ����ʱ��ʱ�����ʱ�䱸�ݣ��Ա��ж��Ƿ���ʱ�����
bool g_ss_vip_event;    //�Ƿ�����Ҫ�˳���������Ҫ�¼�������USB���룬��
uint8 g_ss_delay_flag;  //�Ƿ��Ƴ��˳������������Ż����ӳ������˳�����Ч��
com_albumart_cb_t g_com_paint_cb; //����������ʾ���ͼ���ص�����

///������ȫ�ֱ���
bool g_kl_time_is_up;   //��������ʱʱ���ѵ���־

///ר�� common �û�������UI bank���У�
uint8 com_str_buffer[192] _BANK_DATA_ATTR_; //192�ֽ��ַ���������
uint8 com_name_buffer[52] _BANK_DATA_ATTR_; //52�ֽ����ִ�������
alarm_alarm_t g_alarm_temp _BANK_DATA_ATTR_;//alarm ר�û�����
alarm_fm_t g_aufm_temp _BANK_DATA_ATTR_;    //alarm ר�û�����
