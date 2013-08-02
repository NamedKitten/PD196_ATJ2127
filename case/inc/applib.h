/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.  
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-9 17:27     1.0             build this file 
*******************************************************************************/
/*! 
 * \file     applib.h
 * \brief    Applib ģ��������ݽṹ���궨�壬�ӿ������ȡ�
 * \author   lzcai
 * \version  1.0
 * \date     2011-9-9
*******************************************************************************/

#ifndef _applib_H_
#define _applib_H_

#include "psp_includes.h"
#include "case_type.h"
#include "gui_msg.h"
#include "app_msg.h"
#include "common_func.h"

//#define SUPPORT_FILTER_GRAB

/*! �������֧�� 3 ��Ӧ��ͬʱ���� */
#define MAX_APP_COUNT   3
/*! Ӧ��ץȡ app msg �����Ŀ */
#define MAX_GRAB_MSG    4
/*! Ӧ�ù��� app msg �����Ŀ */
#define MAX_FILTER_MSG  4

/*! ���������ඨʱ��������ϵͳ���ܶ�ʱ */ 
#define CONTROL_TIMER_COUNT     1
/*! ���� UI �ඨʱ����
 *  1������ �ػ�UI������UI��������ʾ�ȣ�
 *  2������ headbar ˢ�£�
 *  3������ ui_directory, ui_menulist �ȿؼ��������ַ���������
 */
#define GUI_TIMER_COUNT         3
/*! ������ʱ����Ŀ */
#define COMMON_TIMER_COUNT      (CONTROL_TIMER_COUNT + GUI_TIMER_COUNT) 

/*! ���Ź����ʱ��Ĭ��Ϊ 5�� */
#define SOFT_WATCHDOG_TIMEOUT   5000

/*! ������ʹ�ܱ�־λ��bit[7] */ 
#define SOUND_BIT_KT_ENABLE     0x80
#define SOUND_BIT_KT_DISABLE    0x7f
/*! ��Ƶ���״̬λ��bit[0-3]���� soundout_state_e */
#define SOUND_BIT_SOUND_OUT     0x0f
#define SOUND_BIT_SOUND_OUT_CLR 0xf0

/*! ��ص�ѹ�������� */
#define BAT_SAMPLE_TIME     7
/*! ��ص�ѹ������λ�� */
#define BAT_SAMPLE_CARRY    4

/*! 
 * \brief
 *  app_type_e Ӧ�ã����̣�����ö�����ͣ���Ӧ g_app_info_vector ����
 */ 
typedef enum 
{
    /*! ���̹����� */
    APP_TYPE_PMNG       = 0,
    /*! ǰ̨Ӧ�ã�gui���̣� */
    APP_TYPE_GUI        = 1,
    /*! ��̨Ӧ�ã�console���̣� */
    APP_TYPE_CONSOLE    = 2,
} app_type_e;

/*! 
 * \brief
 *  app_info_t Ӧ����Ϣ�ṹ��
 */ 
typedef struct
{
    /*! �ṹ��ʹ�ñ�־��1��ʾ�ѱ�ʹ�ã�0��ʾδ��ʹ�� */ 
    uint8 used; 
    /*! ����ID�ţ��� case_type.h �ж�����б� */
    uint8 app_id;
    /*! Ӧ�����ͣ���app_type_e ���� */
    uint8 app_type; 
    /*! ����˽����Ϣ����ID����mq_id_e ���� */
    uint8 mq_id;
    /*! �Ƿ��ֹ���� */ 
    uint8 screensaver_forbidden;
    /*! �Ƿ��ֹʡ��ػ� */ 
    uint8 auto_standby_forbidden;
    /*! �Ƿ��ֹ˯�߹ػ� */ 
    uint8 timed_shutdown_forbidden; 
    /*! �Ƿ��ֹ���Ź� */
    uint8 soft_watchdog_forbidden;
    /*! ���Ź��´γ�ʱ��ʱ�� */
    uint32 soft_watchdog_timeout_expires;
    
#ifdef SUPPORT_FILTER_GRAB
    /*! �Ƿ�filter���е�msg*/
    uint8 filter_all_msg;
    /*! ��grab��msg���� */
    msg_apps_type_e msg_grabed[MAX_GRAB_MSG];
    /*! ��filter��msg���� */
    msg_apps_type_e msg_filtered[MAX_FILTER_MSG];
#endif
} app_info_t;

/*! 
 * \brief
 *  engine_type_e ��������ö�����ͣ�����Ӧ���û������̨��������
 */ 
typedef enum
{
    /*! û������ */ 
    ENGINE_NULL     = 0,
    /*! music���� */
    ENGINE_MUSIC    = 1,
    /*! fm���� */
    ENGINE_RADIO    = 2,
} engine_type_e;

/*! 
 * \brief
 *  engine_state_e ����״̬ö������ 
 */ 
typedef enum
{
    /*! û������ */ 
    ENGINE_STATE_NULL       = 0,
    /*! �������ڲ��� */ 
    ENGINE_STATE_PLAYING    = 1,
    /*! ���洦����ͣ״̬ */ 
    ENGINE_STATE_PAUSE      = 2,
} engine_state_e;

/*! 
 * \brief
 *  app_state_e ǰ̨Ӧ�� UI ״̬ö������
 */ 
typedef enum
{
    /*! ǰ̨AP���ڷǲ��� UI ״̬ */ 
    APP_STATE_NO_PLAY           = 0,
    /*! ǰ̨AP���ڲ��� UI ״̬ */
    APP_STATE_PLAYING           = 1,
    /*! ǰ̨AP���ڲ��� UI ״̬����ϣ�����ⳣ�� */
    APP_STATE_PLAYING_ALWAYS    = 2,
} app_state_e;

/*! 
 * \brief
 *  card_state_e ��״̬ö������ 
 */ 
typedef enum
{
    /*! û�� */ 
    CARD_STATE_CARD_NULL        = 0,
    /*! �п���û���� */ 
    CARD_STATE_CARD_IN          = 1,
    /*! û�� */ 
    CARD_STATE_CARD_PLAY        = 2,
} card_state_e; 

/*! 
 * \brief
 *  cable_state_e ����״̬ö������
 */ 
typedef enum
{
    /*! û�� USB �� */
    CABLE_STATE_CABLE_NULL      = 0,
    /*! �в� USB �� */
    CABLE_STATE_CABLE_IN        = 1,
    /*! �в� ADAPTOR �� */
    CABLE_STATE_ADAPTOR_IN      = 2,
} cable_state_e;

/*! 
 * \brief
 *  backlight_state_e ����״̬ö������
 */ 
typedef enum
{
    /*! ��������״̬������ */
    BACKLIGHT_STATE_NORMAL      = 0,
    /*! ����䰵״̬������ */
    BACKLIGHT_STATE_DARK        = 1,
    /*! ����ر�״̬������ */
    BACKLIGHT_STATE_OFF         = 2,
} backlight_state_e;

/*! 
 * \brief
 *  app_info_state_t ϵͳ��ǰ״̬�ṹ�� 
 */ 
typedef struct
{
    /*! ��ǰ���е��������ͣ��� engine_type_e ���� */
    uint8 type; 
    /*! ���浱ǰ״̬�����ڲ��Ż�����ͣ���� engine_state_e ���� */
    uint8 state;
    /*! ǰ̨Ӧ�� UI ״̬�������ڲ���UI�ͷǲ���UI���� app_state_e ���� */
    uint8 app_state;
    /*! ���˵��������������������� KEY_TYPE_HOLD & KEY_TYPE_SHORT_UP */ 
    uint8 filter_key;
    /*! ������״̬��TRUE��ʾ��ס��FALSE��ʾû�� */
    bool keylock_state; 
    /*! �������״̬�֣�bit[7]��ʾ�Ƿ�ʹ�ܰ�������bit[0-3]��ʾ��Ƶ���״̬����soundout_state_e */
    uint8 sound_state;
    /*! ��ǰ������С��������ǰ��̨Ӧ�ù��� */
    uint8 volume;
    /*! ����״̬�����������͹أ��� backlight_state_e ���� */
    uint8 backlight_state;
    /*! ��Ļ����״̬��TRUE��ʾ����Ļ�����У�FALSE��ʾ������Ļ�����У���״̬����Ļ����������� */
    bool screensave_state;
    /*! ��״̬��û�����п����п��Ҳ��ţ��� card_state_e ���� */ 
    uint8 card_state;
    /*! ����״̬��û�� USB �ߣ��в� USB �ߣ��� cable_state_e ���� */
    uint8 cable_state;
    /*! ���������ߣ�״̬ */
    bool antenna_state;
    /*! ���״̬���� charge_state_e ���� */
    uint8 charge_state;
    /*! ��ص����������͵��5��磬6��״̬ */
    uint8 bat_value;
    /*! ���Ƿ��Ѿ����������б��� card_state û��ֱ�ӹ�ϵ */
    bool card_plist;
    /*! ���Ƿ��Ѿ����´��� */
    bool card_update;
    /*! Video Ӧ��ר�ã���¼ Video Ӧ���˳�ʱ�Ƿ񲥷ţ�1��ʾ���ţ�0��ʾ������ */
    uint8 video_state;
} app_info_state_t; 

/*! 
 * \brief
 *  app_last_state_t ϵͳ���״̬�ṹ�壬�����Զ����״̬�仯������ 
 */ 
typedef struct
{
    /*! ����˳���ǰ̨AP���Ա㷵�� */
    uint8 last_app; 
    /*! ����������ͣ��� engine_type_e ���� */
    uint8 last_type;
    /*! �������״̬�����ڲ��Ż�����ͣ���� engine_state_e ���� */
    uint8 last_state;
    /*! ���ǰ̨AP UI ״̬���� app_state_e ���壨��ʱֻ������Ƶ��¼�˳�ǰ��״̬�� */
    uint8 last_app_state;
    /*! �����״̬��û�����п����п��Ҳ��ţ��� card_state_e ���� */ 
    uint8 last_card_state;
    /*! �������״̬��û�� USB �ߣ��в� USB �ߣ��� cable_state_e ���� */
    uint8 last_cable_state; 
    /*! ������õı���ʱ�䣬��0.5s Ϊ��λ */
    uint8 last_light_timer; 
    /*! ���������Ļ����ģʽ */ 
    uint8 last_screen_save_mode;
    /*! �������˯�߹ػ�ʱ�� */ 
    uint8 last_sleep_timer; 
} app_last_state_t; 


/*! 
 * \brief
 *  Ӧ�ü���ʱ���������� tag id�����ڰѶ�ʱ��������ĳ�����������õع���Ӧ�������ж�ʱ����
 */ 
/*! AP ������ tag */
#define APP_TIMER_TAG_MAIN          0x00
/*! ϵͳ���� tag�����ⳡ�����κγ����¶���ִ�� */
#define APP_TIMER_TAG_SYS           0xff
/*! common ģ�����ⳡ����־ID��0x80��ʼ��0x01 - 0x7fԤ���� ap */
#define APP_TIMER_TAG_SLEEP         0x80
#define APP_TIEMR_TAG_ANIM          0x81
#define APP_TIEMR_TAG_DELETE        0x82
#define APP_TIEMR_TAG_DIALOG        0x83
#define APP_TIEMR_TAG_DIR           0x84
#define APP_TIEMR_TAG_KEYLK         0x85
#define APP_TIEMR_TAG_MENU          0x86//��·�����书�ܲ˵��б�
#define APP_TIEMR_TAG_MENU_SIMPLE   0x87//����·�����书�ܲ˵��б�
#define APP_TIEMR_TAG_PARAM         0x88
#define APP_TIEMR_TAG_SCREEN        0x89
#define APP_TIEMR_TAG_SHUT          0x8a
#define APP_TIEMR_TAG_TEXTRD        0x8b
#define APP_TIEMR_TAG_USBCN         0x8c
#define APP_TIEMR_TAG_VOLUME        0x8d

/*! 
 * \brief
 *  timer_type_e ��ʱ������ö�ٽṹ
 */ 
typedef enum
{
    /*! ��ͨģʽ�Ķ�ʱ�����Զ����ڷ��� */
    TIMER_TYPE_NORMAL,
    /*! ������ʱ����ֻ��ʱһ�κ����TIMER_STATE_STOPED ״̬ */
    TIMER_TYPE_SINGLE_SHOT, 
} timer_type_e; 

/*! 
 * \brief
 *  timer_state_e ��ʱ��״̬ö�ٽṹ
 */ 
typedef enum
{
    /*! ������ʱ״̬ */ 
    TIMER_STATE_RUNNING,
    /*! ֹͣ��ʱ״̬��ֻ��ͨ��restart���¿�ʼ��ʱ */
    TIMER_STATE_STOPED, 
    /*! ��־����״̬��������������ʱ�� */
    TIMER_STATE_NOUSED, 
} timer_state_e;

/*! 
 * \brief
 *  timer_attrb_e ��ʱ������ö�ٽṹ����������ʱ����Դ���г�ʼ����
 */ 
typedef enum
{
    /*! UI ����ʱ�����ڱ���Ϩ�����ִ�� ISR */
    APP_TIMER_ATTRB_UI, 
    /*! control ����ʱ�����κ�ʱ�򶼻�ִ�� ISR */ 
    APP_TIMER_ATTRB_CONTROL,
} timer_attrb_e;

/*! 
 * \brief
 *  timer_proc ��ʱ���������� 
 */ 
typedef void (*timer_proc)(void);

/*! 
 * \brief
 *  app_timer_t ��ʱ������ṹ��
 */ 
typedef struct  
{
    /*! ��ʱ��״̬����timer_state_e ���壬TIMER_STATE_NOUSED��ʾ���� */
    uint8 state;
    /*! ��ʱ�����ԣ���timer_attrb_e ���� */
    uint8 attrb;
    /*! ��ʱ�� ��ʱ���ڣ���λΪ 1ms */            
    uint16 timeout; 
    /*! ��ʱ�����ͣ���timer_type_e ���� */
    uint8 type; 
    /*! ��ʱ��������־ tag */ 
    uint8 tag;
    uint8 reserve[2];
    /*! ��ʱ�� ��һ�γ�ʱ����ʱ��㣬��λΪ 1ms */
    uint32 timeout_expires; 
    /*! ��ʱ�� �������� */
    timer_proc func_proc;
} app_timer_t;



/*! 
 *  \brief
 *  g_app_info_vector ȫ��Ӧ����Ϣ�ṹ�����飬�����ϵͳȫ�ֿռ�
 */ 
#ifndef PC
extern app_info_t g_app_info_vector[MAX_APP_COUNT]; 
#else
extern DLL_IMP app_info_t g_app_info_vector[];
#endif

/*! 
 *  \brief
 *  g_app_info_state ȫ��ϵͳ��ǰ״̬�ṹ�壬�����ϵͳȫ�ֿռ� 
 */ 
#ifndef PC
extern app_info_state_t g_app_info_state;
#else
extern DLL_IMP app_info_state_t g_app_info_state;
#endif

/*! 
 *  \brief
 *  g_app_last_state ȫ��ϵͳ���״̬�ṹ��
 */ 
#ifndef PC
extern app_last_state_t g_app_last_state;
#else
extern DLL_IMP app_last_state_t g_app_last_state;
#endif

/*! 
 *  \brief
 *  g_bat_sample ��ص�������ֵ������7��ȡƽ��ֵ
 */ 
#ifndef PC
extern uint8 g_bat_sample[BAT_SAMPLE_TIME];
#else
extern DLL_IMP uint8 g_bat_sample[BAT_SAMPLE_TIME];
#endif

/*! 
 *  \brief
 *  g_bat_index ��ص����������
 */ 
#ifndef PC
extern uint8 g_bat_index;
#else
extern DLL_IMP uint8 g_bat_index;
#endif

/*! 
 *  \brief
 *  g_this_app_info ��ǰӦ����Ϣ�ṹ��ָ��
 */ 
extern const app_info_t *g_this_app_info;

/*! 
 *  \brief
 *  thread_mutex �̻߳����ź���ָ�� 
 */ 
extern os_event_t *thread_mutex;

/*! 
 *  \brief
 *  g_this_app_msg_dispatch Ӧ��˽����Ϣ�ַ�����
 */ 
extern app_msg_dispatch g_this_app_msg_dispatch;

/*! 
 *  \brief
 *  g_app_timer_vector Ӧ�ü���ʱ������ָ�룬ָ��Ӧ�ÿռ�Ķ�ʱ������
 */ 
extern app_timer_t *g_app_timer_vector; 

/*! 
 *  \brief
 *  g_app_timer_count Ӧ�ü���ʱ����Ŀ
 */ 
extern uint8 g_app_timer_count; 

/*! 
 *  \brief
 *  g_this_app_timer_tag ��ʱ��������־��AP����Ϊ0
 */ 
extern uint8 g_this_app_timer_tag;


#ifdef PC
#define inline
#endif

///��ȡ��̨�������� 
static inline engine_type_e get_engine_type(void)
{
    return g_app_info_state.type;
}

///�޸ĺ�̨��������
static inline void change_engine_type(engine_type_e type)
{
    g_app_info_state.type = type;
}

///��ȡ��̨����״̬ 
static inline engine_state_e get_engine_state(void) 
{
    return g_app_info_state.state;
}

///�޸ĺ�̨����״̬ 
static inline void change_engine_state(engine_state_e state)
{
    g_app_info_state.state = state; 
}

///��ȡ��̨����״̬ 
static inline uint8 get_filter_key(void)
{
    return g_app_info_state.filter_key; 
}

///�޸ĺ�̨����״̬ 
static inline void change_filter_key(uint8 key) 
{
    g_app_info_state.filter_key = key;
}

///��ȡ������״̬��TRUE��ʾ��ס��FALSE��ʾû��
static inline bool get_keylock_state(void)
{
    return g_app_info_state.keylock_state;
}

///�޸İ�����״̬��TRUE��ʾ��ס��FALSE��ʾû��
static inline void change_keylock_state(bool keylock)
{
    g_app_info_state.keylock_state = keylock;
}

///��ȡ������ʹ�����TRUE��ʾʹ�ܣ�FALSE��ʾ��ֹ
static inline bool get_keytone_enable(void) 
{
    return ((g_app_info_state.sound_state & SOUND_BIT_KT_ENABLE) != 0);
}

///��ȡ��Ƶ���״̬����soundout_state_e
static inline uint8 get_soundout_state(void)
{
    return (g_app_info_state.sound_state & SOUND_BIT_SOUND_OUT);
}

///��ȡ��ǰ����ֵ
static inline uint8 get_current_volume(void)
{
    return g_app_info_state.volume;
}

///���õ�ǰ����ֵ
static inline void set_current_volume(uint8 volume) 
{
    g_app_info_state.volume = volume;
}

///��ȡ����״̬��TRUE��ʾ��������FALSE��ʾ������
static inline backlight_state_e get_backlight_state(void)
{
    return g_app_info_state.backlight_state;
}

///�޸ı���״̬ 
static inline void change_backlight_state(backlight_state_e state)
{
    g_app_info_state.backlight_state = state;
}

///��ȡ��Ļ����״̬��TRUE��ʾ����Ļ�����У�FALSE��ʾ������Ļ������
static inline bool get_screensave_state(void)
{
    return g_app_info_state.screensave_state;
}

///�޸���Ļ����״̬ 
static inline void change_screensave_state(bool state)
{
    g_app_info_state.screensave_state = state;
}

///��ȡǰ̨Ӧ�� UI ״̬�������ڲ���UI�ͷǲ���UI 
static inline app_state_e get_app_state(void)
{
    return g_app_info_state.app_state;
}

///�޸�ǰ̨Ӧ�� UI ״̬�������ڲ���UI�ͷǲ���UI 
static inline void change_app_state(app_state_e state)
{
    g_app_info_state.app_state = state; 
}

///��ȡ��ʱ��������־ 
static inline uint8 get_app_timer_tag(void) 
{
    return g_this_app_timer_tag;
}

///�޸���ʱ��������־ 
static inline void change_app_timer_tag(uint8 tag)
{
    g_this_app_timer_tag = tag; 
}

///��ȡϵͳ��״̬
static inline card_state_e get_card_state(void) 
{
    return g_app_info_state.card_state; 
}

///�޸�ϵͳ��״̬
static inline void change_card_state(card_state_e card_state)
{
    g_app_info_state.card_state = card_state;
}

///��ȡϵͳ USB ����״̬
static inline cable_state_e get_cable_state(void)
{
    return g_app_info_state.cable_state;
}

///�޸�ϵͳ USB ����״̬
static inline void change_cable_state(cable_state_e cable_state)
{
    g_app_info_state.cable_state = cable_state; 
}

///��ȡ���������ߣ�״̬
static inline bool get_antenna_state(void)
{
    return g_app_info_state.antenna_state;
}

///�޸Ķ��������ߣ�״̬
static inline void change_antenna_state(bool in_out)
{
    g_app_info_state.antenna_state = in_out; 
}

///��ȡ���״̬
static inline uint8 get_charge_state(void)
{
    return g_app_info_state.charge_state;
}

///�޸ĳ��״̬
static inline void change_charge_state(uint8 state)
{
    g_app_info_state.charge_state = state; 
}

///��ȡ��ص���
static inline uint8 get_bat_value(void)
{
    return g_app_info_state.bat_value;
}

///�޸ĵ�ص���
static inline void change_bat_value(uint8 value)
{
    g_app_info_state.bat_value = value; 
}

///��ȡ���Ƿ��Ѿ����������б�
static inline bool get_card_plist(void)
{
    return g_app_info_state.card_plist;
}

///�޸Ŀ��Ƿ��Ѿ����������б�
static inline void change_card_plist(bool plist)
{
    g_app_info_state.card_plist = plist; 
}

///��ȡ���Ƿ��Ѿ��������
static inline bool get_card_update(void)
{
    return g_app_info_state.card_update;
}

///�޸Ŀ��Ƿ��Ѿ��������
static inline void change_card_update(bool update)
{
    g_app_info_state.card_update = update; 
}


///��ȡ���ǰ̨AP
static inline uint8 get_last_app(void)
{
    return g_app_last_state.last_app;
}

///��ȡ���ǰ̨AP UI ״̬
static inline app_state_e get_last_app_state(void)
{
    return g_app_last_state.last_app_state;
}

///�������ǰ̨AP UI ״̬
static inline void set_last_app_state(app_state_e state)
{
    g_app_last_state.last_app_state = state;
}



//Ӧ�ã����̣�����ģ��ӿ���������ϸ˵�����ӿڶ��壩
extern bool applib_init(uint8 app_id, app_type_e type) __FAR__; 
extern bool applib_quit(void) __FAR__;


//Ӧ�ü���ʱ������ģ��ӿ���������ϸ˵�����ӿڶ��壩
extern int8 set_app_timer(timer_attrb_e attrb, uint16 timeout, timer_proc func_proc) __FAR__;
extern int8 set_single_shot_app_timer(timer_attrb_e attrb, uint16 timeout, timer_proc func_proc) __FAR__;
extern bool modify_app_timer(int8 timer_id, uint16 timeout) __FAR__;
extern bool stop_app_timer(int8 timer_id) __FAR__;
extern bool restart_app_timer(int8 timer_id) __FAR__;
extern bool kill_app_timer(int8 timer_id) __FAR__;
extern bool init_app_timers(app_timer_t *timers, uint8 count) __FAR__;
extern void handle_timers(void) __FAR__;


//��Ϣͨ�Ź���ģ��ӿ���������ϸ˵�����ӿڶ��壩
//������Ϣ�ӿ�
bool get_gui_msg(input_gui_msg_t *input_msg) __FAR__;
//bool get_sys_msg(sys_msg_t *sys_msg);���ڲ��ӿڣ���get_app_msg���ã�
bool get_app_msg(private_msg_t *private_msg) __FAR__;
bool get_app_msg_for_engine(private_msg_t *private_msg) __FAR__;


//������Ϣ�ӿ�
//bool send_sys_msg(sys_msg_t *sys_msg);����pspʵ�֣�
//bool post_key_msg(input_gui_msg_t *input_msg);����key ����ʵ�֣�
bool send_sync_msg(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout) __FAR__;
bool send_async_msg(uint8 app_id, msg_apps_t *msg) __FAR__; 
bool broadcast_msg(msg_apps_t *msg) __FAR__;
bool broadcast_msg_sys(msg_apps_t *msg) __FAR__;
//�㲥��Ϣ�����ӿ�
#ifdef SUPPORT_FILTER_GRAB
bool filter_msg(msg_apps_type_e msg_type) __FAR__;
bool unfilter_msg(msg_apps_type_e msg_type) __FAR__;
bool filter_all_msg(void) __FAR__;
bool unfilter_all_msg(void) __FAR__;
bool grab_msg(msg_apps_type_e msg_type) __FAR__;
bool release_msg(msg_apps_type_e msg_type) __FAR__; 
#endif

//ͬ����Ϣ��ִ�ӿ�
bool reply_sync_msg(private_msg_t *private_msg) __FAR__;

//��Ϣͨ�Ź���ģ���ʼ���ӿ�
bool applib_message_init(app_msg_dispatch msg_dispatch) __FAR__;

#endif
