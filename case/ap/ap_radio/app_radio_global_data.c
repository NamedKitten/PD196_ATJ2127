/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-15          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \radio UI ap ʹ�õĲ���ȫ������
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/

/*****************************************************
 * ֻ����ȫ�ֱ���
 ******************************************************
 **/

//���������Ƶ��
const uint16 max_freq[4] =
{ MAX_US, MAX_JP, MAX_EU, MAX_USER };

//��������СƵ��
const uint16 min_freq[4] =
{ MIN_US, MIN_JP, MIN_EU, MIN_USER };

//Ĭ���û���̨Ƶ��
const uint16 default_freq[3] =
{ 0x5AE0, 0x8B50, 0xA25C }; //88.8, 101.2, 107.1

//Ĭ���û���̨����
const char default_name[7] = "_CH01_";

//�û���̨�б��ļ���( ֻ��flash  ���̵Ĺ̶�λ�ò���)
//#ifdef USE_83NAME_CREAT
//const char userlist[12] = "FM_LIST LST";
//#else
//FM �û���̨�ļ��̶�ʹ�ó���
const char userlist[16] = "FM_USERLIST.LST";
//#endif

/*****************************************************
 * ��������ر���
 ******************************************************
 **/

//ϵͳȫ�ֱ���
comval_t g_comval;

//radio ui ����������Ϣ
radio_config_t g_radio_config;

/*****************************************************
 * ��ʱ����ر���
 ******************************************************
 **/

/*���ڹ�����ʾ�û���̨��Ϣ�Ķ�ʱ��id ,  80ms ��ʱ*/
int8 timer_flush_radioinfo = -1;

/* �����Զ���̨����ˢ�µĶ�ʱ��id */
int8 timer_auto_seek = -1;

/*****************************************************
 * �û���̨�б���ر���
 ******************************************************
 **/

//radio �û���̨�б�ṹ��Ϣ
radio_userlist_t g_user_station;

//�û���̨�б������ź͵�̨��ӳ���
//�����±�Ϊ�����ţ�����ֵΪ��̨��
//�û���̨���֧�ֱ༭��MAX_STATION_COUNT
uint8 g_userlist_table[MAX_STATION_COUNT];

//��ǰ�����ŵ��û���̨����
//�����ź͵�̨�ſ��Բ�ͬ
uint8 g_userlist_index = 0xff;

//��ǰ�û���̨�б�����Ч�ĵ�̨����
uint8 g_userlist_total;

//�û���̨�б��ļ��ı��뷽ʽ
userlist_encode_e encode_mode;

//�Ƿ�ʹ��Ĭ�ϵ��û���̨�б�
bool g_use_default;

/* vfs ���ؽڵ�*/
uint32 vfs_mount_radio;

/* �û��б��ļ����ȣ��ֽ���*/
uint32 file_total_byte;

/* �û���̨�б��ļ����*/
handle fp_radio;

//�û���̨�б��ļ����ڵĴ洢������
char drv_type;

/*****************************************************
 * ������ʾ��ر���
 ******************************************************
 **/

//�Զ���̨��ʾͼ��
uint8 ui_icon_count;

//��¼֮ǰFM Ƶ��ֵ��λ��
uint8 FMFreNumOld;

//ˢ�±�ʶ
uint8 paint_flag = 0xff;

//Ƶ��ֵ��ascill ��
uint8 Freqtab[7] =
{ 0 }; //107.55

//��̨�б�listbox ˽�����ݽṹ
listbox_private_t radiolist_private;

//��̨�б���ƽṹ
radiolist_control_t list_control;

/*****************************************************
 * ȫ��buffer
 ******************************************************
 **/

//�Զ���̨�����ڱ�����Ч��̨����ʱbuffer
uint16 Auto_tab[MAX_STATION_COUNT];

//��̨�б����洢
uint8 g_radiolist_item[LIST_NUM_ONE_PAGE_MAX][LIST_STR_LENGTH];

/*****************************************************
 * ���̿�����ؼ�����
 ******************************************************
 **/

/* ָ��ǰ�����ŵĵ�̨�б����*/
uint16 *pfreq;

/* ָ��ǰ������ĵ�̨�б����*/
uint16 *pfreq_list;

//��ʶ��ǰ����Ӧ�ó���
radio_scene_e g_radio_scene;

//��ʶ������ĵ�̨�б�����
radio_listtype_e g_radio_listtype;

//����radioUI �����ģʽ
//app_param_e  g_enter_mode;

//��ǰ������option menu����
radio_opt_subscene_e g_option_type;

//���ų����Ľ���ģʽ
radio_playwin_mode_e g_playwin_mode;

//Ԥ���̨�б�����ģʽ
radio_list_entry_e g_stalist_entry;

//FM ���浱ǰ״̬��Ϣ
Engine_Status_t g_engine_status;

//���β˵���ѡ��Ĳ���ģʽ
radio_band_e g_menu_band;

//���ݽ��а��Զ���̨ǰ��Ƶ��
uint16 startfreq;

//ģ���Ƿ�ճ�ʼ����ʶ
bool first_wait = TRUE;

/*���ĸ����Ԥ�����Ԥ���б�
 ��̨�б��option �˵����룬from_where =1
 ���Ž����option �˵����룬from_where =2
 ��ϵ�����غ�ص��ĸ�����*/
uint8 from_where = 0xff;

//��Ҫ���°�װfm ������־
//��Ϊfm ¼��ʱ����fm ������פ���룬��fm ¼���ص�fm ������װfm ����
bool need_reinstall = FALSE;

//�ص����ų���ʱ�Ƿ���Ҫ����Ƶ��, ���������������������
bool need_set_freq = TRUE;

//�Ƿ��һ�δ�radio����(��Ҫ����Ƶ��)
uint8 g_first_open_engine;



