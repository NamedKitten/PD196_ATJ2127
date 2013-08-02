/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-11-27 14:06     1.0             build this file 
*******************************************************************************/
/*!
 * \file     case_type.h
 * \brief    case �������ݽṹ��include case ��ֱ���ϲ�ģ��ͷ�ļ�
 * \author   lzcai
 * \version  1.0
 * \date     2011-11-27
*******************************************************************************/
#include "psp_includes.h"
#include "config_id.h"
#include "common_sty.h"
#include "common_res.h"
#include "lang_id.h"
#include "lcd_driver.h"
#include "display.h"
#include "key_interface.h"
#include "vm_def.h"

#ifndef _case_type_H_
#define _case_type_H_

/*!
 * \brief  
 *  ����Ӧ��ID�����֧��32��Ӧ�ã�ID��Ӧ app_name_ram �е����֡�
 * \note
 * \li  ǰ6��Ӧ��˳���ܹ���������Ӧ�ڷ�����֧�ֵĺ�׺�б�
 */
#define APP_ID_MUSIC        0x00
#define APP_ID_AUDIBLE      0x01
#define APP_ID_VIDEO        0x02
#define APP_ID_PICTURE      0x03
#define APP_ID_EBOOK        0x04
#define APP_ID_BROWSER      0x05
#define APP_ID_VOICE        0x06
#define APP_ID_RECORD       0x07
#define APP_ID_RADIO        0x08
#define APP_ID_TOOLS        0x09
#define APP_ID_SETTING      0x0a
#define APP_ID_USER1        0x0b
#define APP_ID_USER2        0x0c
#define APP_ID_USER3        0x0d
#define APP_ID_MANAGER      0x0e
#define APP_ID_MAINMENU     0x0f
#define APP_ID_PLAYLIST     0x10
#define APP_ID_CONFIG       0x11
#define APP_ID_UDISK        0x12
#define APP_ID_MENGINE      0x13
#define APP_ID_FMENGINE     0x14
#define APP_ID_ALARM        0x15
#define APP_ID_MTP          0x16
#define APP_ID_MTPSYNC      0x17
#define APP_ID_UPGRADE      0x18
#define APP_ID_MAX          0x19
//���� app_name_ram �б����� manager_get_name.c �С�
//const char app_name_ram[23][12] = 
//{
//  "music.ap", "audible.ap", "video.ap", "picture.ap", "ebook.ap", "browser.ap", "voice.ap", "record.ap",
//  "radio.ap", "tools.ap", "setting.ap", "user1.ap", "user2.ap", "user3.ap", "manager.ap", "mainmenu.ap",
//  "playlist.ap", "config.ap", "udisk.ap", "mengine.ap", "fmengine.ap", "alarm.ap", "mtp.ap", "mtpsync.ap",
//  "fwupdate.ap"
//};

/*!
 * \brief  
 *  Ӧ�ú�׺�� bitmap��bit<31> = 0h��
 * \note
 * \li  ���׺�������Ӧ��ϵ��bit[30] ��Ӧ��׺������ rom_ext_name ��Ԫ�أ��� MP3��
 */
#define MUSIC_BITMAP        0x7e010e80
//#define MUSIC_BITMAP        0x76000000
#define AUDIBLE_BITMAP      0x00003000
#define VIDEO_BITMAP        0x01004000
#define PICTURE_BITMAP      0x00780000
#define PICTURE_BITMAP1     0x00600000//��̨���ֲ���ʱ��ͼƬ�ļ�������
#define TEXT_BITMAP         0x00800000
#define RECORD_BITMAP       0x50000080//MP3,WAV,ACT
#define ALLALL_BITMAP       0x00000004
#define ALLFILE_BITMAP      0x00000002
#define ALLDIR_BITMAP       0x00000001
//���� rom_ext_name �б����� fs_common_func.c �С�
//const char rom_ext_name[31][4] = 
//{      
//    "MP3",//��Ӧ bit[30]
//    "WMA",
//    "WAV",   
//    "AAC",
//    "FLAC",
//    "APE", 
//    "AVI",//��Ӧ bit[24]
//    "TXT", 
//    "JPG", 
//    "JPEG", 
//    "BMP", 
//    "GIF", 
//    "LRC",
//    "ASF",   
//    "OGG",//��Ӧ bit[16]
//    "WMV", 
//    "AMV", 
//    "AAX",
//    "AA",  
//    "MP1", 
//    "MP2",
//    "M4A",
//    "POS",//��Ӧ bit[8]
//    "ACT",
//    "** ",
//    "** ",
//    "** ",
//    "** ",
//    "**",
//    "*f",
//    "*d",//��Ӧ bit[0]
//};

/*! �޷���32λ������ -1 ֵ�궨�� */
#define V_U32_INVALID   0xffffffff
/*! �޷���16λ������ -1 ֵ�궨�� */
#define V_U16_INVALID   0xffff
/*! �޷���8 λ������ -1 ֵ�궨�� */
#define V_U8_INVALID    0xff

/*! �ļ�ϵͳ������С */
#define FILE_SECTOR     512



/*!
 * \brief
 *  ����playlist�����б��������
    ʹ��bit7~bit5��ʾ�̷�����������
    ��bit4~bit0��Ȼ��app_param_e
 */

#define PLIST_DISK_C      0x80
#define PLIST_DISK_H      0x40 
#define PLIST_DISK_U      0x20
#define PLIST_DISK_ALL    0xc0//0xe0,�ݲ�֧��Uhost

//#define FROM_ALARM    0x60

/*!
 * \brief
 *  app_param_e ����Ӧ�ô��ݲ�������ö������
 */
typedef enum
{
    /*! �ղ��� */
    PARAM_NULL = 0x0,
    /*! �� mainmenu ap ���� */
    PARAM_FROM_MAINMENU,
    /*! �� music ap ���� */
    PARAM_FROM_MUSIC,
    /*! �� audible ap ���� */
    PARAM_FROM_AUDIBLE,
    /*! �� video ap ���� */
    PARAM_FROM_VIDEO,
    /*! �� picture ap ���� */
    PARAM_FROM_PICTURE,
    /*! �� ebook ap ���� */
    PARAM_FROM_EBOOK,
    /*! �� browser ap ���� */   
    PARAM_FROM_BROWSER,
    /*! �� voice ap ���� */ 
    PARAM_FROM_VOICE,
    /*! �� record ap ���� */    
    PARAM_FROM_RECORD,
    /*! �� radio ap ���� */ 
    PARAM_FROM_RADIO,
    /*! �� tools ap ���� */ 
    PARAM_FROM_TOOLS,
    /*! �� setting ap ���� */   
    PARAM_FROM_SETTING,
    /*! �� playlist ap ���� */  
    PARAM_FROM_PLAYLIST,
    /*! �� config ap ���� */    
    PARAM_FROM_CONFIG,
    /*! �� udisk ap ���� */ 
    PARAM_FROM_UDISK,   
    /*! �����ڲ��Ų˵����� */
    PARAM_BACKTO_PLAYING,
    /*! ���ϴβ��Ų˵�����*/
    PARAM_BACKTO_PLAYED,
    /*! ��alarm ap����*/
    PARAM_FROM_ALARM,
} app_param_e;

/*!
 * \brief
 *  app_result_e Ӧ�ò�ӿڷ��ؽ��ö������
 */
typedef enum
{
    /*! û���κ���Ҫ֪ͨ�����ߵ�������أ������߲���Ҫ���κ����⴦�� */
    RESULT_NULL = 0x00,
    /*! ��0x8000��ʼ��Ԥ���� 0x0000 ~ 0x7fff �� msg_apps_type_e���԰�
     *  msg_apps_type_e ���� app_result_e ���Ӽ�
     */
    _RESERVE_FOR_APP_MSG_TYPE_ = 0x8000,
    
    /*! �����ļ�������Ϣ 0x8001*/
    RESULT_MUSIC_PLAY,
    /*! �����鼮������Ϣ*/
    RESULT_AUDIBLE_PLAY,
    /*! ��Ƶ�ļ�������Ϣ*/
    RESULT_MOVIE_PLAY,
    /*! ͼƬ�ļ�������Ϣ*/
    RESULT_PICTURE_PLAY,
    /*! �ı��ļ�������Ϣ*/
    RESULT_EBOOK_PLAY,
    /*! �ص� MainMenu Ӧ�� 0x8006*/
    RESULT_MAIN_APP,
    /*!���ֽ�����Ϣ  0x8007*/
    RESULT_MUSIC,
    /*!��Ƶ������Ϣ  0x8008*/
    RESULT_VIDEO,
    /*!¼��������Ϣ  0x8009*/
    RESULT_RECORD,
    /*!ͼƬ������Ϣ  0x800a*/
    RESULT_PICTURE,
    /*!RADIO������Ϣ  0x800b*/
    RESULT_RADIO,
    /*!�����������Ϣ 0x800c*/
    RESULT_BROWSER,
    /*!���ý�����Ϣ  0x800d*/
    RESULT_SETTING,
    /*!���߽�����Ϣ  0x800e*/
    RESULT_TOOLS,
    /*!��һ�β��Ž�����Ϣ  0x800f*/
    RESULT_LASTPLAY,
    /*!���ڲ��Ž�����Ϣ  0x8010*/
    RESULT_NOWPLAYING,
    /*! ȷ��ĳ���¼����������  0x8011*/
    RESULT_CONFIRM,
    /*! ȡ��ĳ���¼����������  0x8012*/
    RESULT_CANCEL,
    /*! ���Է��أ�������RESULT_NULL�������߲���Ҫ���κ����⴦�� 0x8013*/
    RESULT_IGNORE,
    /*! �ػ�UI���أ�ͨ�����غ���Ҫ���»��Ƶ�ǰUI��������Ҫ�˳���ǰ��Ϣѭ�� 0x8014*/
    RESULT_REDRAW,
    /*! Ӧ���˳���Ϣ 0x8015*/
    RESULT_APP_QUIT,
    /*! USBѡ��ѡ�����ݴ���ģʽ */
    RESULT_USB_TRANS,
    /*! USBѡ��ѡ���粥��ģʽ */
    RESULT_CHARGE_PLAY,
    /*! USB�˵�ѡ���аγ�USB���� */
    RESULT_USB_UNSTICK,
    /*! ɾ�������ļ���ɺ󷵻� */
    RESULT_DELETE_FILE,
    /*! ɾ���ļ��л��б���ɺ󷵻� */
    RESULT_DELETE_DIRLIST,
    /*! ɾ���ļ��л��б���ɾ��������ɺ󷵻� */
    RESULT_DELETE_DIRLIST_NOSELF,
    /*! �������󷵻� 0x801c*/
    RESULT_ERROR,
    
    /*! ���ڲ��� */
    RESULT_OPTION_PLAYING,
    /*! �ϴβ��� */
    RESULT_OPTION_LASTPLAY, 
    /*! ���ţ��Զ�ѡ���б��е�һ�׸貥�� */
    RESULT_OPTION_PLAY,
    /*! ��ӵ��ղؼ� */
    RESULT_OPTION_ADD_FAVOURITE,
    /*! ɾ�������ݼ�����ɾ���ļ���Ŀ¼ */
    RESULT_OPTION_DELETE,
    /*! ˵����Ϣ */
    RESULT_OPTION_NULL,
    /*! �����Զ����� */
    RESULT_AUTO_UPGRADE,
    

    /*! �ļ���������ؽ����ռλ�� */
    RESULT_DIR_BASE = 0x8100,
    /*! ���󣺽���Ŀ¼���󣬰��� root */
    RESULT_DIR_ERROR_ENTER_DIR,
    /*! ���󣺸�Ŀ¼��û���ļ����ļ��� */
    RESULT_DIR_ERROR_NO_FILE,
    /*! ���󣺶��㲻�Ǹ�Ŀ¼����£�set location ʧ�� */
    RESULT_DIR_ERROR_SETLOC,
    
    /*! �˵��б��ؽ����ռλ�� */
    RESULT_MENU_NULL = 0x8200,
    /*! ������һ���˵� */
    RESULT_MENU_PARENT,
    /*! ������һ���˵�����ʵûʲôʵ������ */
    RESULT_MENU_SON,
    /*! ���½��뵱ǰ������ִ�в˵�ͷ�ص����� menu_func */
    RESULT_MENU_CUR,
    /*! �ػ�UI�󷵻���һ���˵� */
    RESULT_MENU_REDRAW_PARENT,
    /*! �˳��˵������� */
    RESULT_MENU_EXIT,
    /*! ���¼��ز˵�������Ƕ�׵��� menulist ���� */
    RESULT_MENU_REDRAW,
    
    /*! �Ի��򷵻ؽ����ռλ�� */
    RESULT_DIALOG_BASE = 0x8300,
    /*! ѡ��OK��ť */
    RESULT_DIALOG_OK,   
    /*! ѡ��CANCEL��ť */   
    RESULT_DIALOG_CANCEL,
    /*! ѡ��ABORT��ť */
    RESULT_DIALOG_ABORT,
    /*! ѡ��RETRY��ť */
    RESULT_DIALOG_RETRY,
    /*! ѡ��IGNORE��ť */
    RESULT_DIALOG_IGNORE,
    /*! ѡ��YES��ť */
    RESULT_DIALOG_YES,
    /*! ѡ��NO��ť */
    RESULT_DIALOG_NO,
    
    /*! �������ÿ򷵻ؽ����ռλ�� */
    RESULT_PARAM_BASE = 0x8400,
    
    /*! ���˵�Ӧ�÷��ؽ����ռλ�� */
    RESULT_MAINMENU_BASE = 0x8500,
    /*! �л�����һ��Ӧ�� */
    RESULT_MAIN_NEXT_ITEM,
    /*! �л�����һ��Ӧ�� */
    RESULT_MAIN_PREV_ITEM,
    /*! ����ѡ��˵� */
    RESULT_OPTION_MAINMENU,   
    /*! �������˵� */
    RESULT_ROOTMENU,
    
    /*! ����Ӧ�÷��ؽ����ռλ�� */
    RESULT_MUSIC_BASE = 0x8600,
    
    /*! ������ʼ���� */
    RESULT_PLAYING_START,
    /*! ȫ��������� */
    RESULT_PLAYING_SHUFFLE,
  
    /*! ���벥�ų�������ԭ��״̬ */
    RESULT_PLAYING_KEEP,
    /*! �ص�fm���� */
    RESULT_RADIO_PLAY,
    
    /*! ����ղؼ�1 */
    RESULT_ADD_FAVOR1,
    /*! ����ղؼ�2 */
    RESULT_ADD_FAVOR2,
    /*! ����ղؼ�3 */
    RESULT_ADD_FAVOR3,
    
    /*! ɾ���ղؼ� */
    RESULT_DEL_FAVOR,
    /*! ����ղؼ� */
    RESULT_CLR_FAVOR,
    
    /*! ����allsong�б� */
    RESULT_ENTER_TITLE_LIST,
    /*! ����ר���б� */
    RESULT_ENTER_ALBUM_LIST,
    /*! ��������б� */
    RESULT_ENTER_ARTIST_LIST,    
    /*! �������б� */
    RESULT_ENTER_GENRE_LIST,
    /*! ����ר��ͼƬ�б� */    
    RESULT_ENTER_ALBUM_THUMBNAIL,

    /*! �����ղؼ��б� */
    RESULT_ENTER_FAVOR_LIST,
    /*! �����ղؼ�1�б� */
    RESULT_ENTER_FAVOR1_LIST,
    /*! �����ղؼ�2�б� */
    RESULT_ENTER_FAVOR2_LIST,
    /*! �����ղؼ�3�б� */
    RESULT_ENTER_FAVOR3_LIST,
    
    /*! audible���� */
    RESULT_AUDIBLE_RESUME,
    /*! ����audible�鼮�б� */
    RESULT_ENTER_BOOKS_LIST,
    /*! ����audible�����б� */
    RESULT_ENTER_AUTHORS_LIST,
    
    /*! �б����˵� */
    RESULT_LISTMENU_MENU,
    /*! �б����б� */
    RESULT_LISTMENU_LIST,
    /*! audible���ò˵� */
    RESULT_SETMENU_AUDIBLE,
    /*! music���ò˵� */
    RESULT_SETMENU_MUSIC,
    /*! ���ų��� */
    RESULT_PLAYING_SCENE,
    /*! ���������˳� */
    RESULT_SCENE_EXIT,
    /*! ���봴�������б�ap */
    RESULT_CREATE_LIST_APP,
    
    /*! �����µ���ڲ˵� */
    RESULT_ENTER_NEWENTRY,
    
    /*! card�γ����� */
    RESULT_CARD_OUT_ERROR,

    /*! ��������Ա�״̬ */
    RESULT_MUSIC_RECORDING,
    /*! ������Աȶ����Ž��� */
    RESULT_PLAY_END,
    
    /*! ����Ӧ�÷��ؽ����ռλ�� */
    RESULT_VIDEO_BASE = 0x8700,
    /*!video�Ĳ��Ž���*/
    RESULT_VIDEO_PLAY,
    /*!video���ļ��б����*/
    RESULT_VIDEO_FILELIST,
    /*!video��option�˵�����*/
    RESULT_VIDEO_OPTIONMENU,
    /*!video�Ĳ˵�����*/
    RESULT_VIDEO_MENU,    
    /*!video�ķ���*/
    RESULT_VIDEO_RETURN,
#if 0
    /*!video��ȫ������*/
    RESULT_VIDEO_FULL_SCREEN,
    /*!video����������*/
    RESULT_VIDEO_PLAYBAR,
    /*!video����������*/
    RESULT_VIDEO_VOLUMEBAR,
#endif

    /*! ¼��Ӧ�÷��ؽ����ռλ�� */
    RESULT_RECORD_BASE = 0x8800,
    /*!¼��Ӧ�õ�¼������*/
    RESULT_RECORD_REC,
    /*!¼��Ӧ�õ����ò˵�*/
    RESULT_RECORD_MEMU,
    RESULT_RECORD_OPTION_MEMU,

    
    /*! RADIOӦ�÷��ؽ����ռλ�� */
    RESULT_RADIO_BASE = 0x8900,
       /* ����Ԥ���̨�б���*/
    RESULT_RADIO_STATION_LIST,
    /* �����û���̨�б���*/
    RESULT_RADIO_USER_LIST,
    /* �����Զ���г���ٽ��벥�ų���*/
       //RESULT_RADIO_AUTO_PLAY,
    /* �����̨�б��option �˵�*/
    RESULT_RADIO_OPTION_STATION,
    /* ���벥�ų����µ�option �˵�*/
    RESULT_RADIO_OPTION_PLAYWIN,
    /* ��ʼFM  ���ţ�ֱ�ӽ��벥�ų���*/
    RESULT_RADIO_START_PLAY,
    /* ����FM ��һ�����˵�����*/
    RESULT_RADIO_MAINMENU,


    /*! ������Ӧ�÷��ؽ����ռλ�� */
    RESULT_EBOOK_BASE = 0x8a00,
    /*!ebook �Ĳ��Ž���*/
    RESULT_EBK_PLAY,
    /*!ebook ���ļ��б����*/
    RESULT_EBK_FILELIST, 
    /*!ebook �Ĳ˵�����*/
    RESULT_EBK_SETTINGMENU,
    /*!ebook ���Ķ��˵�����*/
    RESULT_EBK_READINGMENU,
    /*! ����Ӧ�÷��ؽ����ռλ�� */
    RESULT_SETTING_BASE = 0x8b00,
    
    /*! ����Ӧ�÷��ؽ����ռλ�� */
    RESULT_TOOLS_BASE = 0x8c00,
    /*TOOLS �Ĳ˵�����*/
    RESULT_TOOLS_MENULIST,
    /*TOOLS ���˳�����*/
    RESULT_TOOLS_EXIT,
    /*TOOLS ����������*/
    RESULT_CALENDAR,
    /*TOOLS ��������*/
    RESULT_STOPWATCH,
    /*Reserved*/
    RESULT_ALARMMENU,
    /*�˳������ݽ���*/
    RESULT_DATAEXCH,
    /*�˳���U��*/
    RESULT_UDISK,
    /*��ʱalarm��Ϣ 0x8c08*/
    RESULT_ALARM,
    /*��ʱFM��Ϣ 0x8c09*/
    RESULT_ALARMFM,
    /*��ʱFMREC��Ϣ 0x8c0a*/
    RESULT_FMRECORD,
    /*alarm�˳���Ϣ 0x8c0b*/
    RESULT_ALARM_QUIT,

    /*! �ļ�����Ӧ�÷��ؽ����ռλ�� */
    RESULT_BROWSER_BASE = 0x8d00,
    /*! ����browser�ļ������ */
    RESULT_BROWSER_FILELIST,
    /*! ����browser�˵� */
    RESULT_BROWSER_MENU,
    /*! ������ap����browser */
    RESULT_RETURN_BROWSER,
    /*! �˳�browserӦ�� */
    RESULT_BROWSER_EXIT,
    
    /*! ��������Ӧ�÷��ؽ����ռλ�� */
    RESULT_MENGINE_BASE = 0x8e00,
    
    /*! FM����Ӧ�÷��ؽ����ռλ�� */
    RESULT_FMENGINE_BASE = 0x8f00,
    
    /*!picture ģ�鷵�ص��¼���ռλ��*/ 
    RESULT_PIC_BASE = 0x9000,   
    /*!picture �Ĳ��Ž���*/
    RESULT_PIC_PLAY,
    /*!picture ���ļ��б����*/
    RESULT_PIC_FILELIST, 
    /*!picture �Ĵ���ѡ��˵�����*/
    RESULT_PIC_MENU,
    /*!picture ��option�˵�����*/
    RESULT_PIC_OPTIONMENU,    
    /*!picture ����*/
    RESULT_PIC_RETURN,
    /*!picture����ͼ��һ��*/
    RESULT_PIC_NEXTITEM,
    /*!picture����ͼ��һ��*/
    RESULT_PIC_NEXTSCREEN,
    
    /*!voiceģ�鷵�ص��¼���ռλ��*/ 
    RESULT_VOICE_BASE = 0x9100,
    /*!����voice play����*/ 
    RESULT_VOICE_PLAY,
    /*!����voice menu����*/ 
    RESULT_VOICE_MENU,
    /*!����voice fm menu����*/ 
    RESULT_VOICE_FM_MENU,
    /*!����record root menu����*/ 
    RESULT_RECORD_ROOT_MENU,
    /*!����fm root menu����*/ 
    RESULT_FM_ROOT_MENU,
    /*!����fm Ԥ���̨�б��µ�menu����*/ 
    RESULT_FM_FREQ_MENU,

    /*! playlistӦ�÷��ؽ����ռλ�� */
    RESULT_PLAYLIST_BASE = 0x9200,
} app_result_e;


/*!
 * \brief
 *  sys_counter_t ϵͳ���ܼ�ʱ�����ݽṹ������ʱ������0.5 sΪ��λ
 */
typedef struct
{
    /*! ����䰵��رռ�ʱ�� */
    uint16 light_counter;
    /*! ��Ļ������ʱ�� */
    uint16 screen_saver_counter;
    /*! �������ڲ��ż�ʱ�� */
    uint16 playing_counter;
    /*! ��ȡ��ص�����ʱ�� */
    uint16 battery_counter;
    /*! ʡ��ػ���ʱ�� */
    uint16 poweroff_counter;
    /*! ˯�߹ػ���ʱ�� */
    uint16 sleep_counter;
} sys_counter_t;

/*!
 *  \brief
 *  g_sys_counter ��ȫ�֣�ϵͳ���ܼ�ʱ���������ϵͳȫ�ֿռ�
 */
#ifndef PC
extern sys_counter_t g_sys_counter;
#else
extern DLL_IMP sys_counter_t g_sys_counter;
#endif

/*!
 *  \brief
 *  config_fp ��ȫ�֣�Ӧ�����ýű��ļ�����������ϵͳȫ�ֿռ�
 */
#ifndef PC
extern sd_handle config_fp;
#else
extern DLL_IMP sd_handle config_fp;
#endif

#endif
