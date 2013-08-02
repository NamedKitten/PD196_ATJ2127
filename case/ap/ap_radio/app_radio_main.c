/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       mikeyang         2011-09-13          1.0              create this file
 *******************************************************************************/
#include "app_radio.h"
#include "app_radio_menu_config.h"

extern app_result_e radioUI_scene_dispatch(app_param_e enter_mode)
__FAR__;
extern bool radio_modual_standby(void);
extern bool radio_close_engine(void);

#if 0    //����֧��
//�����Դ����
const uint8 themes_com[5][13] =
{
    "common.sty", "cm_green.sty", "cm_gray.sty", "cm_pink.sty", "cm_red.sty"
};

const uint8 themes_ap[5][13] =
{
    "radio.sty", "ra_green.sty", "ra_gray.sty", "ra_pink.sty", "ra_red.sty"
};
#endif

/* radio ap 1�����ڹ�����1 �������Զ���̨
 **/
#define APP_TIMER_COUNT		2
/*!
 *  \brief
 *  app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
 */
app_timer_t radio_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

/*! for alarm_fm */
/* radio����ģʽ */
uint8 g_radio_from_alarm = 0;
/* alarm_fm�Ľ���ʱ�� */
time_t alarmfm_end_time;
/* alarm_fmƵ�� */
//uint16 alarmfm_frequency;
/* alarm_fm���� */
uint8 alarmfm_volume;

/* Radio UI ����ջ*/
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_RADIO_STK_POS;
#endif

/* Radio UI ���̵����߳����ȼ�*/
INT8U prio = AP_RADIO_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 * \void radio_load_cfg(void)
 * \����Ӧ�ó�ʼ����Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void radio_load_cfg(void)
{
    setting_comval_t temp_comval;
    alarm_vars_t temp_alarm_val;
    uint8 default_band;

    //ϵͳ������ȡ
    sys_vm_read(&temp_comval, VM_AP_SETTING, sizeof(setting_comval_t));

    /*��ʼ��ϵͳ��VM����*/
    if (temp_comval.g_comval.magic != MAGIC_SETTING)
    {
        temp_comval.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&temp_comval);
        sys_vm_write(&temp_comval, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_comval.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    //��ȡradio AP ������Ϣ
    sys_vm_read(&g_radio_config, VM_AP_RADIO, sizeof(radio_config_t));

    //ͳһΪ0x55AA
    if (g_radio_config.magic != MAGIC_USER1)
    {
        g_radio_config.magic = MAGIC_USER1;

        //��������Ԥ���̨�б�����
        libc_memset(g_radio_config.fmstation_us, 0, MAX_STATION_COUNT * 2* 3 );

        default_band = (uint8)com_get_config_default(RADIO_AP_ID_BAND);
        g_radio_config.FMStatus.freq = min_freq[default_band];
        g_radio_config.FMStatus.station = 0;
        g_radio_config.FMStatus.station_count = 0;
        g_radio_config.band_mode = (radio_band_e)default_band; // Band_China_US

        //Ĭ��δ����voice ����
        g_radio_config.enter_voice= NO_ERTER_VOICE;
        //Ĭ����̨��ʽ
        g_radio_config.seek_mode = (FM_SeekMode_e)com_get_config_default(RADIO_AP_ID_SEEKMODE);

        sys_vm_write(&g_radio_config, VM_AP_RADIO);
    }
    //alarm_fmģʽ ��ȡalarm VM��Ϣ
#ifdef ALARMFM
    if (g_radio_from_alarm != 0)
    {
        sys_vm_read(&temp_alarm_val, VM_ALARM, sizeof(alarm_vars_t));
        if (temp_alarm_val.magic != MAGIC_ALARM)
        {
            ;//return FALSE;
        }
        //libc_memcpy(&g_aufm, &temp_alarm_val.g_aufm, sizeof(alarm_fm_t));
        //��ʡ�ռ䣬ֻȡ��FM�й���Ϣ
        libc_memcpy(&alarmfm_end_time, &temp_alarm_val.g_aufm.end_time, sizeof(time_t)); //��ʱFM�Ľ���ʱ��
        g_radio_config.FMStatus.freq = temp_alarm_val.g_aufm.frequency; //FMƵ��
        alarmfm_volume = temp_alarm_val.g_aufm.volume; //FM����
        g_comval.volume_current = temp_alarm_val.g_aufm.volume; //FM����
    }
#endif
    /* ��һ���˳�RadioUI ʱ�����ŵĲ�Ϊ�û���̨*/
    if(g_radio_config.band_mode != Band_MAX)
    {
        /* �˵����μ�����*/
        g_menu_band = g_radio_config.band_mode;
    }
    else
    {
        /* �ϴ��˳����ŵ����û���̨����˵���Ĭ�ϼ���Ϊŷ�޲���*/
        /* �û���̨�ɱ༭��С������ŷ�޲���һ����Ϊ50k */
	 /* ����״���̨�󲥷��û���̨�˳����ٽ�������ʾ����̨���� */	
        g_menu_band = (uint8)com_get_config_default(RADIO_AP_ID_BAND);
    }

    //�û���̨�б����㣬���½���
    libc_memset(g_user_station.fmstation_user, 0, MAX_STATION_COUNT*2);
    //�û���̨��ʾ�����ִ�����
    libc_memset(g_user_station.station_name, 0, 40);
    //�û���̨�б�����������
    libc_memset(g_userlist_table, 0, MAX_STATION_COUNT);

    //��ʹ��Ĭ���û���̨�б�
    g_use_default = FALSE;

    /* ָ��ָ��ǰ���ŵ�Ƶ�ʱ���*/
    init_point_freq();

    //��ʼ����������б�ָ��
    if(g_menu_band == Band_China_US)
    {
        pfreq_list = &g_radio_config.fmstation_us[0];
    }
    else if(g_menu_band == Band_Japan)
    {
        pfreq_list = &g_radio_config.fmstation_jp[0];
    }
    else
    {
        pfreq_list = &g_radio_config.fmstation_eu[0];
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void radio_save_cfg(void)
 * \����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void radio_save_cfg(void)
{
    //��Ӧ���ļ���
    uint16 ap_file_num[MAX_APP_NUM];
    setting_comval_t temp_comval;
#ifdef ALARMFM
    if (g_radio_from_alarm != 0)
    {
        return;
    }
#endif
    sys_vm_read(&temp_comval, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&temp_comval.g_comval, &g_comval, sizeof(comval_t));
    /*����VM����*/
    sys_vm_write(&temp_comval, VM_AP_SETTING);
    sys_vm_write(&g_radio_config, VM_AP_RADIO);

    /*������Ӧ���ļ�����*/
    sys_vm_read(&ap_file_num, VM_FILE_NUM, sizeof(ap_file_num));
    ap_file_num[APP_ID_RADIO] = g_radio_config.FMStatus.station_count;
    sys_vm_write(&ap_file_num, VM_FILE_NUM);

    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radioUI_app_init(void)
 * \RadioUI Ӧ����Ϣ��ʼ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radioUI_app_init(void)
{
    bool app_init_ret = TRUE;
    uint8 i;

    //��ȡ������Ϣ (����vm��Ϣ)
    radio_load_cfg();

    /*����UI  ��Դ�ļ�*/
#if 0
    app_init_ret = ui_res_open(themes_com[g_comval.theme], UI_COM);
    app_init_ret = ui_res_open(themes_ap[g_comval.theme], UI_AP);
#endif
    app_init_ret = ui_res_open("common.sty", UI_COM);
    app_init_ret = ui_res_open("radio.sty", UI_AP);
    if (!app_init_ret)
    {
        return FALSE;
    }

    /* �򿪲˵����ýű��ļ�*/
    app_init_ret = com_open_confmenu_file("radio.mcg");
    if (!app_init_ret)
    {
        return FALSE;
    }

#ifdef PC
    load_menulist_simfunc(get_menu_func, get_callback_func, get_menu_option);
#endif

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_RADIO, APP_TYPE_GUI);

    //��ʼ����ʱ��
    init_app_timers(radio_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    applib_message_init(radio_app_msg_callback);

    //��ʼ��ϵͳ��ʱ��
    sys_timer_init();

    //�����û���̨�б��������������ȡ����Ƶ����Ϣ
    app_init_ret = radio_parse_userlist(NULL, ALL_FREQ, 0);
    if (!app_init_ret)
    {
        //��������ʹ��Ĭ�ϵ��û���̨�б�
        g_use_default = TRUE;
        encode_mode = ENCODE_MUL;

        libc_memset(g_user_station.fmstation_user, 0, MAX_STATION_COUNT * 2);
        libc_memset(g_user_station.station_name, 0, 40);
        //Ĭ���û���̨�б����
        for (i = 0; i < 3; i++)
        {
            g_user_station.fmstation_user[i] = default_freq[i];
            g_userlist_table[i] = i + 1; //CH1, CH2, CH3
        }
        //Ĭ���û���̨�ݶ�3  ��
        g_userlist_total = 3;
        libc_memcpy(g_user_station.station_name, default_name, sizeof(default_name));
    }
    else
    {
        /* �ɹ���������ʹ��Ĭ���б�*/
        g_use_default = FALSE;
    }
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radioUI_app_exit(void)
 * \�˳�app�Ĺ��ܺ��� ����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radioUI_app_exit(void)
{
    //ɾ��ϵͳ��ʱ��
    sys_timer_exit();

    //������Ϣ��дVRAM
    radio_save_cfg();

    //�ر���Դ�ļ�
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    //�ر�Ӧ�ò˵����ýű�
    com_close_confmenu_file();

    /*ִ��applib���ע������*/
    applib_quit();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void radioUI_scene_result_deal(app_result_e result_val)
 * \��������ֵ����
 * \param[in]    result_val �������ȷ���ֵ para1
 * \param[out]   none
 * \return       none the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void radioUI_scene_result_deal(app_result_e result_val)
{
    msg_apps_t msg;
    bool need_create_app = TRUE;

    //��������Ӧ�ý���
    msg.type = MSG_CREAT_APP;
    switch (result_val)
    {
        //�˳�FM UI���̣�����Browser ����( Browser ���ݴ��ι���¼�����ļ�)
        case RESULT_VOICE_FM_MENU:
        msg.content.data[0] = APP_ID_BROWSER; //browser ap
        break;

        //����main menu����
        case RESULT_MAIN_APP:
        msg.content.data[0] = APP_ID_MAINMENU;//main ap
        break;

        //����FM record ����
        case RESULT_RECORD_REC:
        msg.content.data[0] = APP_ID_RECORD; //fm record ap
        break;

        //�ص�music���ڲ���ǰ̨����
        case RESULT_MUSIC:
        msg.content.data[0] = APP_ID_MUSIC; //music ui ap
        break;

        case RESULT_APP_QUIT:
        need_create_app = FALSE;
        break;

        //Ĭ�ϻص�main
        default:
        msg.content.data[0] = APP_ID_MAINMENU;//main ap
        break;
    }

    if (need_create_app == TRUE)
    {
        msg.content.data[1] = (uint8) PARAM_FROM_RADIO;
        if (result_val == RESULT_MUSIC)
        {
            msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYING;
        }
        //�����첽��Ϣ����������ǰ̨����
        send_async_msg(APP_ID_MANAGER, &msg);
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc, const char *argv[])
 * \radioUI app ��ں���
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e retval;
    bool result;
    app_param_e enter_mode = (app_param_e)(argc);
    if (enter_mode == PARAM_FROM_ALARM)
    {
        g_radio_from_alarm = 1;
    }
    //    app_param_e enter_mode = (app_param_e) (argc & 0x1f);
    //    //��λ�֪����ģʽ?
    //    g_radio_from_alarm = (uint8) (argc & 0xe0);
    result = radioUI_app_init();//��ʼ��
    if (!result)
    {
        //��ʼ��ʧ�ܣ�������Ӧ��
        retval = RESULT_MAIN_APP;
    }
    else
    {
        retval = radioUI_scene_dispatch(enter_mode);//����������
    }
#ifdef ALARMFM
    if (g_radio_from_alarm != 0)
    {
        //���ڷ���ֵΪRESULT_APP_QUIT�ķ���ֵ��һ��Ϊ����USB����manager����ǰ��̨
        //���˳�����ʱ����ҪӦ�ÿ���ap���˳�������Ĺر�
        if(retval != RESULT_APP_QUIT)
        {
            if(radio_get_vol_status() == TRUE)
            {
                radio_close_engine();
            }
        }
    }
#endif
    //�������ȷ��ؽ������
    radioUI_scene_result_deal(retval);
    radioUI_app_exit();//�˳�
    return retval;
}

