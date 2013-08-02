/*******************************************************************************
 *                              us211A
 *                            Module: Applib
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-11-27 22:06     1.0             build this file 
*******************************************************************************/
/*!
 * \file     app_msg.h
 * \brief    Ӧ�ò���Ϣ����ģ��������ݽṹ���궨���
 * \author   lzcai
 * \version  1.0
 * \date     2011-11-27
*******************************************************************************/

#ifndef _app_msg_H_
#define _app_msg_H_

#include "psp_includes.h"
#include "case_type.h"

/*!
 * \brief
 *  msg_apps_type_e Ӧ����Ϣö������
 */
typedef enum
{
    /*! ����Ϣ */
    MSG_NULL = 0x0000,
    
    /****************************** shortcut�¼� ****************************/
    /*! ��ݼ�ռλ���������ռ� 0x0020 ~ 0x003f */
    SHORTCUT_RESERVE_BEGIN = 0x001f,
    /*! ���� RETURN ������ mainmenu Ӧ���¼� */
    EVENT_RETURN_MAINMENU,
    /*! ���� VOL+ �� VOL- �����������ڽ����¼� */
    EVENT_ENTER_VOLUMEBAR,
    /*! ���� LOCK �������أ����밴���������¼� */
    EVENT_ENTER_KEYLOCK,
    /*! ���� LOCK �������������밴���������¼� */
    EVENT_ENTER_KEYLOCK_UN,
    /*! ������ס״̬�£������ⰴ�����밴���������¼� */
    EVENT_ENTER_KEYLOCK_HOLD,
    /*! ���� PLAY ������ػ�����ȷ���¼� */
    EVENT_ENTER_SHUTOFF_ASK,
    SHORTCUT_RESERVE_END = 0x0040,

    /******************************system message****************************/
    /*! system manager��Ϣռλ�������� 0x0080 ~ 0x00ff ������ϵͳ*/
    SYSTEM_RESERVE_BEGIN = 0x007f,
    /* for qac �ض���Ϊenum���ͣ����� qac ���� */
    MSG_RTC2HZ         = MSG_SYS_RTC2HZ,
    MSG_BAT_V_CHG      = MSG_SYS_BAT_V_CHG,
    MSG_LOW_POWER      = MSG_SYS_LOW_POWER,
    MSG_RTCALARM       = MSG_SYS_RTCALARM,
    MSG_POWER_OFF      = MSG_SYS_POWER_OFF,
    MSG_RECLAIM        = MSG_SYS_RECLAIM,
                                
    MSG_USB_STICK      = MSG_SYS_USB_STICK,
    MSG_USB_UNSTICK    = MSG_SYS_USB_UNSTICK,
                                
    MSG_ADAPTOR_IN     = MSG_SYS_ADAPTOR_IN,
    MSG_ADAPTOR_OUT    = MSG_SYS_ADAPTOR_OUT,
                                
    MSG_SD_IN          = MSG_SYS_SD_IN,
    MSG_SD_OUT         = MSG_SYS_SD_OUT,
                                
    MSG_UH_IN          = MSG_SYS_UH_IN,
    MSG_UH_OUT         = MSG_SYS_UH_OUT,
    
    /*! ���������ߣ����� */
    MSG_EARPHONE_IN    = 0x00f0,
    /*! ���������ߣ��γ� */
    MSG_EARPHONE_OUT,
    /*! ���������Ϣ */
    MSG_FULL_CHARGE,
    SYSTEM_RESERVE_END = 0x0100,

    /*! ѯ��Ӧ���Ƿ� ready ��ͬ����Ϣ */
    MSG_APP_ACTIVE_SYNC,
    /*! ֪ͨӦ�ó����˳���Ϣ��Ӧ�ó�����ܵ�����Ϣ�󣬽����˳����̣�����Ϣֻ���� manager.app ���� */
    MSG_APP_QUIT,
    /*! ֪ͨӦ�ûص����ڲ��Ž��棬�� sys_counter �ڼ�⵽45��û������������ */
    MSG_APP_BACKTO_PLAYING,
    /*! ������Ļ���� */
    MSG_APP_ENTER_SCREEN_SAVE,
    /*! ������Ļ�������ص����� */
    MSG_APP_ENTER_SCREEN_SAVE2,
    /*! �˳���Ļ���� */
    MSG_APP_EXIT_SCREEN_SAVE,

    /******************************manager message****************************/
    /*! process manager��Ϣռλ��*/
    MANAGER_BASE = 0x0200,
    /*! �첽����Ӧ�ó�����Ϣ */
    MSG_CREAT_APP,
    /*! ͬ������Ӧ�ó�����Ϣ */
    MSG_CREAT_APP_SYNC,  
    /*! �첽ɱ��Ӧ�ó�����Ϣ */
    MSG_KILL_APP,
    /*! ͬ��ɱ��Ӧ�ó�����Ϣ */
    MSG_KILL_APP_SYNC, 
    /*! ���� Udisk ���ݴ��� */
    MSG_USB_TRANS,
    /*! ֹͣmanager���Ź��������ʱ������Ϣ */
    MSG_STOP_HARD_WATCHDG_SYNC,
    /*! ����manager���Ź��������ʱ������Ϣ */
    MSG_RESTART_HARD_WATCHDG_SYNC,

    /***************************Music engine message**************************/
    /*! Music engine��Ϣռλ�� */
    MENGINE_BASE        = 0x0300,
    /*! ��ȡ������Ϣ�����ֲ��Ž��涨ʱ��ȡ������Ϣ */
    MSG_MENGINE_GET_ENGINE_INFO_SYNC,
    /*! ��ȡ��ǰ�ļ�·�� */
    MSG_MENGINE_GET_FILEPATH_SYNC,
    /*! ����EQģʽ */
    MSG_MENGINE_SET_EQ_SYNC,
    /*! ���ò���ģʽ */
    MSG_MENGINE_SET_PLAYMODE_SYNC,
    /*! ������ʾģʽ */
    MSG_MENGINE_SET_DISPMODE_SYNC,
    /*! ���õ��뵭�� */
    MSG_MENGINE_SET_FADE_SYNC,
    /*! ���ò����ٶ� */
    MSG_MENGINE_SET_PLAYSPEED_SYNC,
    /*! ����AB����ģʽ: �Զ����ֶ� */
    MSG_MENGINE_SET_AB_AUTO_SYNC,
    /*! ����AB�������� */
    MSG_MENGINE_SET_AB_COUNT_SYNC,
    /*! ����AB������� */
    MSG_MENGINE_SET_AB_GAP_SYNC,    
    /*! ����AB����A�� */
    MSG_MENGINE_SET_APOINT_SYNC,
    /*! ����AB����B�� */
    MSG_MENGINE_SET_BPOINT_SYNC,
    /*! ��ʼAB���� */
    MSG_MENGINE_START_AB_SYNC,
    /*! ȡ��AB���� */
    MSG_MENGINE_CLEAR_AB_SYNC,
    /*! ����AB������һ��: A��--> B�� */
    MSG_MENGINE_SET_AB_NEXT_SYNC,
    /*! ����AB������һ��: A��<-- B�� */
    MSG_MENGINE_SET_AB_PREV_SYNC,
    /*! �������� */
    MSG_MENGINE_PLAY_SYNC,
    /*! ֹͣ���� */
    MSG_MENGINE_STOP_SYNC,
    /*! ��ͣ���� */
    MSG_MENGINE_PAUSE_SYNC,
    /*! �ָ����� */
    MSG_MENGINE_RESUME_SYNC,
    /*! ����ָ��ʱ�䲥������ */
    MSG_MENGINE_SEEK_SYNC,
    /*! �������ֲ����ļ�(�ļ�ѡ������ʽ) */
    MSG_MENGINE_SET_FILEPATH_SYNC,
    /*! �������ֲ����ļ�(playlist��ʽ)  */
    MSG_MENGINE_SET_FILEINDEX_SYNC,
    /*! �л�����һ�� */
    MSG_MENGINE_PLAY_NEXT_SYNC,
    /*! �л�����һ�� */
    MSG_MENGINE_PLAY_PREV_SYNC,
    /*! ɾ���ļ� */
    MSG_MENGINE_DELETE_FILE_SYNC,
    /*! ��� */
    MSG_MENGINE_FFWD_SYNC,
    /*! ���� */
    MSG_MENGINE_FBWD_SYNC,
    /*! ȡ���������� */
    MSG_MENGINE_CANCEL_FFB_SYNC,
    /*! ���ÿ������˲��� */
    MSG_MENGINE_SET_FFBSTEP_SYNC,
    /*! ���ÿ������˲���ʱ�� */
    MSG_MENGINE_SET_FFBTIME_SYNC,
    /*! ��ȡ��ǰʱ�����ǩ��Ϣ */
    MSG_MENGINE_MAKE_BOOKMARK_SYNC,
    /*! �����ϴβ����ļ� */
    MSG_MENGINE_SET_LAST_FILE_SYNC,
    /*! �������״̬: UIӦ�ô������� */
    MSG_MENGINE_CLEAR_ERR_SYNC,
    /*! ɾ���ղؼУ������ղؼ���ź����� */
    MSG_MENGINE_DEL_FAVORITE_SYNC,  
    /*! ����ղؼУ������ղؼ����� */
    MSG_MENGINE_ADD_FAVORITE_SYNC,
    //for audible 
    /*! �л�����һ�� */
    MSG_MENGINE_CAHPTER_NEXT_SYNC,
    /*! �л�����һ�� */
    MSG_MENGINE_CHAPTER_PREV_SYNC,   
    /*! ��ȡ��ǰ��ͼƬ��Ϣ */
    MSG_MENGINE_AUDIBLE_GETIMAGE_SYNC,  
    //===========music ui ������Ϣ ======================
    
    /*! ��ȡ��Ϣ */
    MSG_MENGINE_GET_ENGINE_CFG_SYNC,
    /*! ��ȡѭ��ģʽ+shuffle */
    MSG_MENGINE_GET_PLAYMODE_SYNC,
    /*! ��ȡ����ģʽ */
    MSG_MENGINE_GET_ENGINE_PLAYMODE_SYNC,
    /*! ��ȡ�ļ���ʽ */
    MSG_MENGINE_GET_FILEINFO_SYNC,
    /*! ��ȡ��ǰ����״̬ */
    MSG_MENGINE_GET_STATUS_SYNC,
    /*! ��ȡ��ǰ����ʱ��ͱ����� */
    MSG_MENGINE_GET_PLAYINFO_SYNC,  
    /*! ��ȡ��ǰ����ʱ��ϵ� */
    MSG_MENGINE_GET_BKINFO_SYNC, 
    /*! ��ȡ��ǰ�½����������� */
    MSG_MENGINE_GET_SECTION_INFO_SYNC,   
    /*! �������� */
    MSG_MENGINE_SET_VOLUME_SYNC,
    /*! ����audible�½�ģʽ */
    MSG_MENGINE_SET_SECTION_MODE_SYNC,
    /*! ������ǩ */
    MSG_MENGINE_PLAY_BOOKMARK_SYNC,
    /*! music���ų���ɾ���ļ� */
    MSG_MENGINE_DELFILE_MUSUI_SYNC,
    /*! music���ų���ɾ���ļ� */
    MSG_MENGINE_DELFILE_OTER_SYNC,
    /*! music�ı��ļ�����·�� */
    MSG_MENGINE_CHANGE_FILEPATH_SYNC,
    /*! music standby���� */    
    MSG_MENGINE_STANDBY_PLAY_SYNC,
    /****************************fm engine message****************************/
    /*! fm engine��Ϣռλ��*/
    FMENGINE_BASE = 0x0400,

    /* ��ʼ����ж�����*/
    /* ��װFM drv ,  ִ��FM ������̺󼴰�װ*/
    MSG_FMENGINE_OPEN,                   
    /* FM ����IC  ��ʼ������*/
    MSG_FMENGINE_INIT,   
    /* FM ����IC ����Standby ״̬*/
    MSG_FMENGINE_STANDBY,    
    /* ���յ��������ж��FM ���������˳�����*/
    MSG_FMENGINE_CLOSE,                       

    /* �������*/   
    /* ����Ƶ�㲥��( �첽) */
    MSG_FMENGINE_SETFREQ,          
    /* ����Ƶ�㲥��( ͬ��)  */
    MSG_FMENGINE_SETFREQ_SYNC,       
    /* ���þ�����ȡ������*/
    MSG_FMENGINE_SETMUTE,        
    /* ���õ�̨����*/
    MSG_FMENGINE_SETBAND,
    /* ������̨����ֵ*/
    MSG_FMENGINE_SETTHRED,  
    /* ���������̨*/
    MSG_FMENGINE_SET_SOFTSEEK,   
    /* ����Ӳ����̨*/
    MSG_FMENGINE_SET_HARDSEEK,     
    /* �˳�Ӳ����̨*/
    MSG_FMENGINE_SET_SEEKBREAK,     
    /* ����������С*/
    MSG_FMENGINE_SET_VOLUME,           

    /* ��ȡ��Ϣ���*/
    /* ��ȡӲ���Զ���̨�Ƿ���ɣ���ӦEngine_Status_t.STC_flag */
    MSG_FMENGINE_AUTO_SEEK_OVER,     
    /* ��ȡȫ��״̬��Ϣ����ӦEngine_Status_t  �ṹ*/
    MSG_FMENGINE_GET_STATUS, 
    /* ��ȡ��ǰ���Σ���ӦEngine_Status_t.FM_CurBand */
    MSG_FMENGINE_GET_BAND,          
    /* ��ȡ��ǰƵ�ʣ���ӦEngine_Status_t.FM_CurrentFreq */
    MSG_FMENGINE_GET_FREQ,     
    /* ��ȡ��̨�ź�ǿ�ȣ���ӦEngine_Status_t.FM_SignalStg */
    MSG_FMENGINE_GET_INTENSITY, 
    /* ��ȡ����״̬( �����Ƿ����) */
    MSG_FMENGINE_GET_ANTENNA,     
    /* ��ȡ������״̬��Ϣ����ӦEngine_Status_t.FM_Stereo_Status */
    MSG_FMENGINE_GET_STEREO,       
    /* ��ȡ���沥��״̬��Ϣ����ӦEngine_Status_t.FM_VolStatus */
    MSG_FMENGINE_GET_PLAYSTATUS,   
    /* ��ȡ������С*/
    MSG_FMENGINE_GET_VOLUME,       
    /* ��ȡ��ǰģ��״̬*/
    MSG_FMENGINE_GET_MODUAL_STATUS,
    /* �ؽ�FMģ��ȫ����������*/
    MSG_FMENGINE_RESINTALL,
    
    /****************************user1 message****************************/
    /*! user1��Ϣռλ��*/
    USER1_BASE = 0x0500,
    /*! ������Ϣ */
    MSG_USER1_PLAY,
    MSG_USER1_STOP,
    MSG_USER1_ASK_SYNC,
    
    /****************************** dialog�¼� ****************************/
    /*! �Ի����¼�ռλ */
    EVENT_DIALOG_BASE = 0x2000,
    /*! �Ի����л�����һ����ť�¼� */
    EVENT_DIALOG_PREV_BUTTON,
    /*! �Ի����л�����һ����ť�¼� */
    EVENT_DIALOG_NEXT_BUTTON,
    /*! �Ի���ȷ����ť�¼� */
    EVENT_DIALOG_CONFIRM,
    /*! �Ի���ȡ��ѡ���¼� */
    EVENT_DIALOG_CANCEL,
    
    /****************************** user1�¼� ****************************/
    /*! USER1�¼�ռλ */
    EVENT_USER1_BASE = 0x2100,
    /*! �����¼� */
    EVENT_USER1_MODE,
    EVENT_USER1_PLAY,
    
    /****************************** play scene �¼� ****************************/
    /*! ���ų����¼�ռλ */
    EVENT_PLAYING_BASE = 0x2200,
    /*! ����¼� */
    EVENT_PLAYING_FFOREWORD,
    /*! �����¼� */
    EVENT_PLAYING_FBACKWORD,
    /*! ��һ�� or AB �¼� */
    EVENT_PLAYING_NEXTANDAB,
    /*! ��һ���¼� */
    EVENT_PLAYING_PREVSONG,
    /*! option�¼� */
    EVENT_PLAYING_OPTION,       
    /*! ����/��ͣ�¼� */
    EVENT_PLAYING_PAUSE,    
    /*! �����¼� */
    EVENT_PLAYING_RETURN,           
    
    /****************************** music bookmark �¼� ****************************/
    /*! ������ǩ�¼�ռλ */
    EVENT_BOOKMARK_BASE = 0x2280,
    /*! ��һ����ǩ */
    EVENT_BOOKMARK_SELECT_PREV,
    /*! ��һ����ǩ */
    EVENT_BOOKMARK_SELECT_NEXT,
    /*! ѡ����ǩȷ�� */
    EVENT_BOOKMARK_SELECT_SURE,
    /*! option�¼� */
    EVENT_BOOKMARK_OPTION,      
    /*! �����¼� */
    EVENT_BOOKMARK_RETURN,      
    
    /****************************** video�¼� ****************************/
    /*! ��Ƶ���ų����¼�ռλ */
    EVENT_VIDEO_PLAYING_BASE = 0x2300,  
    /*! ��Ƶ����һ��������ȫ��״̬ */
    EVENT_VIDEO_PLAYING_NEXT,
    /*! ��Ƶ����һ��������ȫ��״̬ */
    EVENT_VIDEO_PLAYING_PREV,
    /*! ��Ƶ����/��ͣ������ȫ��״̬*/
    EVENT_VIDEO_PLAYING_PAUSE,
    /*! ��Ƶ�������/����*/
    EVENT_VIDEO_PLAYING_END_SORTING,
    /*! ��Ƶ���*/
    EVENT_VIDEO_PLAYING_FFORWORD,
    /*! ��Ƶ���� */
    EVENT_VIDEO_PLAYING_FBACKWORD,
    /*! ��Ƶ����*/
    EVENT_VIDEO_PLAYING_RETURN,
    /*! ��Ƶoption*/
    EVENT_VIDEO_PLAYING_OPTION, 
    /*! ��Ƶ����*/
    EVENT_VIDEO_PLAYING_VOLUME,
    /*! ��Ƶ����*/
    EVENT_VIDEO_PLAYING_HOLD,

    /****************************** picture�¼� ****************************/
    /*! ͼƬ���ų����¼�ռλ */
    EVENT_PICTURE_PLAYING_BASE = 0x2400,
    /*! ͼƬ����һ��*/
    EVENT_PICTURE_PLAYING_NEXT,
    /*! ͼƬ����һ��*/
    EVENT_PICTURE_PLAYING_PREV,
    /*! ͼƬ�Զ�����/�ֶ�����*/
    EVENT_PICTURE_PLAYING_PAUSE,
    /*! ͼƬ����ͼup��*/
    EVENT_PICTURE_PREVIEW_MOVEUP,
    /*! ͼƬ����ͼdown��*/
    EVENT_PICTURE_PREVIEW_MOVEDOWN,
    /*! ͼƬ����ͼprev��*/
    EVENT_PICTURE_PREVIEW_MOVEPREV,
    /*! ͼƬ����ͼnext��*/
    EVENT_PICTURE_PREVIEW_MOVENEXT,
    /*! ͼƬ����*/
    EVENT_PICTURE_RETURN,
    /*! ͼƬoption*/
    EVENT_PICTURE_OPTION,
    
    /****************************** volumebar�¼� ****************************/
    /*! volumebar�¼�ռλ */
    EVENT_VOLUME_BASE = 0x2500,
    /*! ���������¼� */
    EVENT_VOLUME_INC,
    /*! �����ݼ��¼� */
    EVENT_VOLUME_DEC,
    /*! ��������ȷ���˳��¼� */
    EVENT_VOLUME_CONFIRM,
    /*! ��������ȡ���˳��¼� */
    EVENT_VOLUME_CANCEL,
    
    /****************************** menulist�¼� ****************************/
    /*! menulist�¼�ռλ */
    EVENT_MENULIST_BASE = 0x2600,
    /*! �˵��б�ѡ���¸�ѡ���¼� */
    EVENT_MENULIST_SELECT_NEXT,
    /*! �˵��б�ѡ���ϸ�ѡ���¼� */
    EVENT_MENULIST_SELECT_PREV,
    /*! �˵��б������һ���¼� */
    EVENT_MENULIST_ENTER_SON,
    /*! �˵��б�����һ���¼� */
    EVENT_MENULIST_BACK_PARENT,
    /*! �˵��б���� option ��ݲ˵��¼� */
    EVENT_MENULIST_ENTER_OPTION,
    
    /****************************** directory�¼� ****************************/
    /*! directory�¼�ռλ */
    EVENT_DIRECTORY_BASE = 0x2700,
    /*! �ļ������ѡ���¸�ѡ���¼� */
    EVENT_DIRECTORY_SELECT_NEXT,
    /*! �ļ������ѡ���ϸ�ѡ���¼� */
    EVENT_DIRECTORY_SELECT_PREV,
    /*! �ļ������ѡ����ͣ�¼� */
    EVENT_DIRECTORY_SELECT_STOP,
    /*! �ļ������������һ���¼� */
    EVENT_DIRECTORY_ENTER_SON,
    /*! �ļ������������һ���¼� */
    EVENT_DIRECTORY_BACK_PARENT,
    /*! �ļ���������� option ��ݲ˵��¼� */
    EVENT_DIRECTORY_ENTER_OPTION,
    
    /****************************** delete�¼� ****************************/
    /*! delete�¼�ռλ */
    EVENT_DELETE_BASE = 0x2800,
    /*! ɾ������ֹͣɾ�������ļ��¼� */
    EVENT_DELETE_STOP,
    
    /****************************** parameter�¼� ****************************/
    /*! parameter�¼�ռλ */
    EVENT_PARAMETER_BASE = 0x2900,
    /*! �������ÿ�ѡ���¸������¼� */
    EVENT_PARAMETER_SELECT_NEXT,
    /*! �������ÿ�ѡ���ϸ������¼� */
    EVENT_PARAMETER_SELECT_PREV,
    /*! �������ÿ�ֵ�����¼� */
    EVENT_PARAMETER_VALUE_INC,
    /*! �������ÿ�ֵ�ݼ��¼� */
    EVENT_PARAMETER_VALUE_DEC,
    /*! �������ÿ�ȷ���˳��¼� */
    EVENT_PARAMETER_CONFIRM,
    /*! �������ÿ�ȡ���˳��¼� */
    EVENT_PARAMETER_CANCEL,
    
    /****************************** text_read �¼� ****************************/
    /*! text_read�¼�ռλ */
    EVENT_TEXTREAD_BASE = 0x2a00,
    /*! �ı��Ķ�ѡ����ҳ�¼� */
    EVENT_TEXTREAD_NEXT_PAGE,
    /*! �ı��Ķ�ѡ����ҳ�¼� */
    EVENT_TEXTREAD_PREV_PAGE,
    /*! �ı��Ķ�ѡ���¸�ѡ���¼� */
    EVENT_TEXTREAD_KEY_DOWN,
    /*! �ı��Ķ�ѡ���ϸ�ѡ���¼� */
    EVENT_TEXTREAD_KEY_UP,
    /*! �ı��Ķ�ѡ�񲥷��¼� */
    EVENT_TEXTREAD_PLAY,
    /*! �ı��Ķ�ѡ��ȷ���¼� */
    EVENT_TEXTREAD_CONFIRM,
    /*! �ı��Ķ�ѡ��ȡ���˳��¼� */
    EVENT_TEXTREAD_CANCEL,
    /*! �ı��Ķ�ѡ��˵�ѡ���¼� */
    EVENT_TEXTREAD_OPTION,
    /*! �ı��Ķ�ѡ�񷵻��¼� */
    EVENT_TEXTREAD_RETURN,
    
    /****************************** shutoff�¼� ****************************/
    /*! shutoff�¼�ռλ */
    EVENT_SHUTOFF_BASE = 0x2b00,
    /*! �ػ�UI�˳��¼� */
    EVENT_SHUTOFF_QUIT,
    
    /****************************** mainmenu�¼� ****************************/
    /*! mainmenu�¼�ռλ */
    EVENT_MAINMENU_BASE = 0x2c00,
    /*! �л�����һ��Ӧ���¼� */
    EVENT_MAINMENU_NEXT_ITEM,
    /*! �л�����һ��Ӧ���¼� */
    EVENT_MAINMENU_PREV_ITEM,
    /*! ѡ�񴴽�Ӧ���¼� */
    EVENT_MAINMENU_CREATE_APP,
    /*! ���� option �˵��¼� */
    EVENT_MAINMENU_OPTIONMENU,

    /****************************** udisk�¼� ****************************/
    /*! udisk�¼�ռλ */
    EVENT_UDISK_BASE = 0x2d00,
    /*! �˻ص���һ��Ӧ���¼� */
    EVENT_UDISK_EXIT_LAST,
    /*! �л���MTPӦ���¼� */
    EVENT_UDISK_EXIT_MTP,

    /****************************** radio �¼� ****************************/
    /*! radio �¼�ռλ */
    EVENT_RADIO_BASE = 0x2e00,
    /*! �л�����һ��Ƶ�ʵ��¼� */
    EVENT_RADIO_PREV_FREQ,
    /*! �л�����һ��Ƶ�ʵ��¼� */
    EVENT_RADIO_NEXT_FREQ,
    /*! �л�����һ��Ԥ���̨�¼� */
    EVENT_RADIO_PREV_PRESET,
    /*! �л�����һ��Ԥ���̨�¼� */
    EVENT_RADIO_NEXT_PRESET,
    /*! �л������Ż���ͣ�¼� */
    EVENT_RADIO_PLAY_MUTE,
    /*! ��ǰ�Զ���̨�¼�*/
    EVENT_RADIO_SEEK_PREV,
    /*! ����Զ���̨�¼� */
    EVENT_RADIO_SEEK_NEXT,
    /*! �����¼� */
    EVENT_RADIO_RETURN,
    /*! ����option �¼� */
    EVENT_RADIO_OPTION,
    /*! ��̨�б�ѡ����һ���¼�*/
    EVENT_RADIO_SELECT_PREV,
    /*! ��̨�б�ѡ����һ���¼� */
    EVENT_RADIO_SELECT_NEXT,
    /*! ��̨�б�ѡ��ǰ�ʼ�����¼�*/
    EVENT_RADIO_SELECT_CUR,
    
    /****************************** tools �¼� ****************************/
    /*! tools �¼�ռλ */
    EVENT_TOOLS_BASE = 0x2f00,
    /*! �����л��������¼� */
    EVENT_TOOLS_CAL_SEL_YMD,
    /*! ������ǰ�����������¼� */
    EVENT_TOOLS_CAL_YMD_NEXT,
    /*! �����������������¼� */
    EVENT_TOOLS_CAL_YMD_PREV,
    /*! �����˳��¼� */
    EVENT_TOOLS_RETURN,
     /****************************** mtp�¼� ****************************/
    /*! mtp�¼�ռλ */
    EVENT_MTP_BASE = 0x3000,
    /*! �˻ص���һ��Ӧ���¼� */
    EVENT_MTP_EXIT_LAST,
    
    /****************************** config�¼� ****************************/
    /*! mainmenu�¼�ռλ */
    EVENT_CONFIG_BASE = 0x3100,
    /*! ����ĳ�����¼� */
    EVENT_CONFIG_KEY_UP,
    /*! ����ĳ�����¼� */
    EVENT_CONFIG_KEY_HOLD,
} msg_apps_type_e;

/*!
 * \brief  
 *  sys_msg_t ϵͳ��Ϣ�ṹ��
 */
typedef struct
{
    /*! Ӧ����Ϣ���� */
    msg_apps_type_e type;
} sys_msg_t;


/*!
 * \brief  
 *  msg_apps_t Ӧ����Ϣ�ṹ��
 */
typedef struct
{
    /*! Ӧ����Ϣ���ͣ��μ� msg_apps_type_e ���� */
    uint32 type;
    /*! Ӧ����Ϣ���� */
    union
    {
        /*! ��Ϣ������ʵ���� */
        uint8 data[4];
        /*! ��Ϣ���ݻ�����ָ�룬ָ����Ϣ���ͷ��ĵ�ַ�ռ� */
        void *addr; 
    }content;
} msg_apps_t;

/*!
 * \brief
 *  msg_reply_type_e ͬ����Ϣ��ִö������
 */
typedef enum
{   
    /*! ͬ����Ϣ�޻ظ�������Ӧ���˳�ʱӦ����δ�����ͬ����Ϣ */
    MSG_REPLY_NO_REPLY,
    /*! �ظ�ͬ����Ϣ�����ߣ�ͬ����ϢҪ���ܳɹ���� */
    MSG_REPLY_SUCCESS,
    /*! �ظ�ͬ����Ϣ�����ߣ�ͬ����ϢҪ�������ʧ�� */
    MSG_REPLY_FAILED,
} msg_reply_type_e;
    
/*!
 * \brief
 *  msg_reply_t ͬ����Ϣ��ִ�ṹ��
 */
typedef struct
{
    /*! ͬ����Ϣ��ִö�����ͣ��μ� msg_reply_type_e ���� */
    uint8 type;
    uint8 reserve[3];
    /*! ��ִ���ݻ�����ָ�룬ָ����Ϣ���ͷ��ĵ�ַ�ռ� */
    void *content;
} msg_reply_t;

/*!
 * \brief  
 *  private_msg_t ��Ӧ�ü䣩˽����Ϣ�ṹ��
 */
typedef struct
{
    /*! ˽����Ϣ��Ϣ���� */
    msg_apps_t msg;
    /*! ͬ���ź��� */
    os_event_t *sem;
    /*! ͬ����Ϣ��ִָ�� */
    msg_reply_t *reply;
} private_msg_t;

/*!
 * \brief  
 *  msg_cb_func Ӧ����Ϣ����ص�����
 */
typedef app_result_e (*msg_cb_func)(private_msg_t *private_msg);

/*!
 * \brief  
 *  app_msg_dispatch Ӧ����Ϣ����ص����������ڴ��ݸ� common ģ��ʹ��
 */
typedef app_result_e (*app_msg_dispatch)(private_msg_t *msg_dispatch);

#endif
