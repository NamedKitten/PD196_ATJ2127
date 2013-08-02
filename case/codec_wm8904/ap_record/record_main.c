/********************************************************************************
 *                               us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      leiming         2011-10-13         v1.0              create this file
 ********************************************************************************/
 /*!
 * \file     record_main.c
 * \brief    record����ں���ģ��ͳ�������
 * \author   leiming
 * \version  1.0
 * \date  2011/10/13
 *******************************************************************************/
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ap_record.h"

#ifndef PC
OS_STK *ptos = (OS_STK *) AP_RECORD_STK_POS;
#endif

#ifdef PC
extern menu_cb_func _get_menu_func(uint16 str_id);
extern menu_cb_leaf _get_callback_func(uint16 str_id);
extern menu_cb_option _get_menu_option(uint16 str_id);
#endif

INT8U prio = AP_RECORD_PRIO;

//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT         2

//Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
app_timer_t rec_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

/* define variable, �������� */
/*******************************record***************************************/
//ϵͳ��������
comval_t g_comval;
//record��ȫ�ֱ���
record_vars_t g_record_vars;
//ap ���ģʽ
app_param_e g_entry_mode;
//record codec params
record_mr_param_t g_mr_param;
//¼��������������
record_scene_param_t g_scene_param;
//¼���ļ���ز�������
record_fs_param_t g_fs_param;
//¼�������д�ļ�ͷ�ṹ��
record_writehead_t g_writehead;
//����ʣ������
uint32 rec_free_space;
//��¼ʱ��
uint32 rec_free_time;
//������д�Ķ�ʱ������������0
uint16 rewrite_times;
//¼��Դ����
mmm_ai_type_t g_ai_source_bak;
//�Ƿ���Ҫ��ʼ���ļ�ѡ�����ı�־
bool need_fsel_init = TRUE;

//�Ƿ�ʱ���룬0-��1-��
uint8 g_record_from_alarm = 0;
//for alarm_fm
//��ʱ¼����ʱ�䳤��
uint32 alarm_rec_time;
//FMƵ��
uint16 fmrec_frequency;
//FM����
uint8 fmrec_volume;
//�Ƿ񳬹���ʱ¼��ʱ��
bool g_record_timeout = FALSE;
//дvram�Ĵ�������
uint8 vram_wr_cnt;
//����¼���ļ���ŵļ���
uint16 g_rec_num;
//����fm¼���ļ���ŵļ���
uint16 g_fmrec_num;
//��һ�ν�¼��ADC������־
uint8 g_adc_flag;
//�Ƿ��һ�ν���¼��Ӧ��
uint8 g_first_enter_flag;
//record�ļ����ļ�����
uint16 g_file_total;
//��ʱFM��ʱ��
int8 g_alarm_timer;
/******************************************************************************/
/*!
 * \par  Description:
 * \static void _read_fmrecord_var(void)
 * \��ȡ��ʱfm record��VM������Ϣ
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
static void _read_fmrecord_var(void)
{
    alarm_vars_t temp_alarm_val;
    if (g_record_from_alarm != 0)
    {
        sys_vm_read(&temp_alarm_val, VM_ALARM, sizeof(alarm_vars_t));
        if (temp_alarm_val.magic != MAGIC_ALARM)
        {
            ;//return FALSE;
        }
        //libc_memcpy(&g_aufm, &temp_alarm_val.g_aufm, sizeof(alarm_fm_t));
        //��ʡ�ռ䣬ֻȡ��FM�й���Ϣ
        //libc_memcpy(&record_end_time, &temp_alarm_val.g_aufm.end_time, sizeof(time_t)); //��ʱFM�Ľ���ʱ��
        alarm_rec_time = (uint32)((temp_alarm_val.g_aufm.end_time.hour * 3600 + temp_alarm_val.g_aufm.end_time.minute
                * 60) - (temp_alarm_val.g_aufm.start_time.hour * 3600 + temp_alarm_val.g_aufm.start_time.minute * 60));
        fmrec_frequency = temp_alarm_val.g_aufm.frequency; //FMƵ��
        fmrec_volume = temp_alarm_val.g_aufm.volume; //FM����
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \static void _read_record_var(void)
 * \��ȡrecord��VM������Ϣ
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
static void _read_record_var(void)
{
    int result;
    setting_comval_t temp_val;
    /*��ȡcommon VM����������Ϣ*/
    result = sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));

    /*��ʼ��ϵͳ��VM����*/
    if (temp_val.g_comval.magic != MAGIC_SETTING)
    {
        com_get_default_setting(&temp_val);
        sys_vm_write(&temp_val, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_val.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    // ��ȡrecord VM��������
    result = sys_vm_read(&g_record_vars, VM_AP_RECORD, sizeof(record_vars_t));

    if (g_record_vars.maigc != MAGIC_USER1)
    {
        //�趨Ĭ��ֵ
        g_record_vars.maigc = MAGIC_USER1;
        g_record_vars.path.file_path.dirlocation.disk = DISK_C;
        g_record_vars.file_type = (uint8) com_get_config_default(RECORD_FORMAT_TYPE);//REC_FORMAT_WAV;
        g_record_vars.mp3_bitrate = (uint8) com_get_config_default(RECORD_MP3_BITRATE);//BIT_32KBPS;
        g_record_vars.wav_bitrate = (uint8) com_get_config_default(RECORD_WAV_BITRATE);//BIT_32KBPS;
        g_record_vars.act_bitrate = BIT_8KBPS;

        if (g_record_vars.file_type == REC_FORMAT_WAV)
        {
            g_record_vars.bitrate = g_record_vars.wav_bitrate;
        }
        else if(g_record_vars.file_type == REC_FORMAT_MP3)
        {
            g_record_vars.bitrate = g_record_vars.mp3_bitrate;
        }
        else
        {
            g_record_vars.bitrate = g_record_vars.act_bitrate;    
        }
        g_record_vars.ai_source = (uint8) com_get_config_default(RECORD_INPUT_SOURCE);//AI_SOURCE_LINEIN;
        g_record_vars.encode_type = (uint8) com_get_config_default(RECORD_WAV_ENCODE_TYPE);//ADPCM;
        g_record_vars.file_num = 0;
        g_record_vars.rec_num = 0;
        g_record_vars.fmrec_num = 0;
        g_record_vars.rec_num_card = 0;
        g_record_vars.fmrec_num_card = 0;
        g_record_vars.noise_cancel_level = 0;
        g_record_vars.gain_input_linein = (uint8) com_get_config_default(RECORD_GAIN_INPUT_LINEIN); //2;
        g_record_vars.gain_adc_linein = (uint8) com_get_config_default(RECORD_GAIN_ADC_LINEIN);//0x2f;
        g_record_vars.gain_input_mic = (uint8) com_get_config_default(RECORD_GAIN_INPUT_MIC);//7;
        g_record_vars.gain_adc_mic = (uint8) com_get_config_default(RECORD_GAIN_ADC_MIC);//0x33;
        g_record_vars.track_mode = (uint8) com_get_config_default(RECORD_TRACK_MODE);//TRACK_OFF;
        g_record_vars.avr_mode = (uint8) com_get_config_default(RECORD_AVR_MODE);//AVR_OFF;
        g_record_vars.track_level_breaksong = (uint8) com_get_config_default(RECORD_TRACK_LEVEL_BREAKSONG);//80;
        g_record_vars.track_level_avr = (uint8) com_get_config_default(RECORD_TRACK_LEVEL_AVR);//65; //������ƽ
        g_record_vars.track_delay = (uint8) com_get_config_default(RECORD_TRACK_DELAY);//3;
    }
    //����alarm��Ϣ
    _read_fmrecord_var();
    //���̳�ʼ��
    if (g_record_vars.path.file_path.dirlocation.disk == 0)
    {
        libc_memset(&g_record_vars.path, 0, sizeof(file_path_info_t));
        g_record_vars.path.file_path.dirlocation.disk = DISK_C;
    }
    if (g_record_vars.path.file_path.dirlocation.disk == DISK_C)
    {
        g_rec_num = g_record_vars.rec_num;
        g_fmrec_num = g_record_vars.fmrec_num;
    }
    else
    {
        g_rec_num = g_record_vars.rec_num_card;
        g_fmrec_num = g_record_vars.fmrec_num_card;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \static void _save_record_var(void)
 * \����record��VM������Ϣ
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
static void _save_record_var(void)
{
    setting_comval_t temp_val;
#ifndef PC
    sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&temp_val.g_comval, &g_comval, sizeof(comval_t));
    sys_vm_write(&temp_val, VM_AP_SETTING);
#else
#endif
    /*����VM����*/
    if (g_record_vars.path.file_path.dirlocation.disk == DISK_C)
    {
        g_record_vars.rec_num = g_rec_num;
        g_record_vars.fmrec_num = g_fmrec_num;
    }
    else
    {
        g_record_vars.rec_num_card = g_rec_num;
        g_record_vars.fmrec_num_card = g_fmrec_num;
    }
    sys_vm_write(&g_record_vars, VM_AP_RECORD);

}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_init(void)
 * \applib�ĳ�ʼ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
static bool _app_init(void)
{
    /*��ȡui��Դ�ļ�*/
    ui_res_open("record.sty", UI_AP);
    ui_res_open("common.sty", UI_COM);

    //open mcg file
    com_open_confmenu_file("record.mcg");

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
#endif

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_RECORD, APP_TYPE_GUI);

    //��ʼ����ʱ��
    init_app_timers(rec_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init(&record_message_deal);

    //��ʼ��ϵͳ��ʱ��
    sys_timer_init();

    g_first_enter_flag = TRUE;

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_exit(void)
 * \ap�˳���һЩ����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
static bool _app_exit(void)
{
    //�ļ�ѡ�����˳�
    if (need_fsel_init == FALSE)
    {
        fsel_exit();
    }

    //����ϵͳ��ʱ��
    sys_timer_exit();

    //ִ��applib���ע������
    applib_quit();

    //�ر�Ӧ�ò˵����ýű�
    com_close_confmenu_file();

    /*�ر�ui��Դ�ļ�*/
    ui_res_close(UI_AP);
    ui_res_close(UI_COM);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _scene_dispatch(app_param_e enter_mode)
 * \����������
 * \param[in]    enter_mode ����ģʽ para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       ���ص��δ�
 * \retval
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_dispatch(app_param_e enter_mode)
{
    app_result_e app_ret = RESULT_SCENE_EXIT;
    /*ap����ĳ�ʼ����������ap����Ĳ��������þ���*/
    g_entry_mode = enter_mode;

    switch (g_entry_mode)
    {
        case PARAM_FROM_MAINMENU:
        g_scene_param.record_scene = SCENE_MENU_REC;
        break;

        //case PARAM_FROM_ALARM:
        case PARAM_FROM_RADIO:
        if (g_record_from_alarm != 0)
        {
            if (radio_alarm_init() != TRUE)
            {
                return RESULT_MAIN_APP;
            }
        }
        g_scene_param.record_scene = SCENE_UI_REC;
        g_ai_source_bak = g_record_vars.ai_source;
        g_record_vars.ai_source = AI_SOURCE_FM;
        _radio_get_freq();
        break;

        default:
        if (g_scene_param.record_scene != SCENE_EXIT)
        {
            g_scene_param.record_scene = SCENE_MENU_REC;
        }
        break;
    }
    //g_scene_param.record_scene = SCENE_MENU_REC;
    /*����������*/
    while (g_scene_param.record_scene != SCENE_EXIT)
    {
        switch (g_scene_param.record_scene)
        {
            //rec ui scene
            case SCENE_UI_REC:
            app_ret = record_scene_recording();
            if (app_ret == RESULT_RECORD_MEMU)
            {
                g_scene_param.record_scene = SCENE_MENU_REC;
            }
            else if (app_ret == RESULT_FMRECORD)//alarm_rec��Ϣ����¼��
            {
                g_scene_param.record_scene = SCENE_UI_REC;
                g_record_from_alarm = 1;
                g_entry_mode = PARAM_FROM_RADIO;
                //��ȡalarm_rec��vram
                _read_fmrecord_var();
#ifndef PC
                //��FM  ¼�����أ�����¼��codec ������fm �������룬��װ����
                if (get_engine_type() == ENGINE_RADIO)
                {
                    //ж��fm ����
                    sys_drv_uninstall(DRV_GROUP_FM);
                    //��װfm  ����
                    sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
                }
#endif
                if (radio_alarm_init() != TRUE)//��ʼ��fm��Ϣ
                {
                    g_scene_param.record_scene = SCENE_EXIT;
                }
                g_ai_source_bak = g_record_vars.ai_source;
                g_record_vars.ai_source = AI_SOURCE_FM;
                _radio_get_freq();
            }
            else
            {
                g_scene_param.record_scene = SCENE_EXIT;
            }
            break;

            //record menu scene
            case SCENE_MENU_REC:
            app_ret = record_scene_menu();
            if (app_ret == RESULT_RECORD_REC)
            {
                g_scene_param.record_scene = SCENE_UI_REC;
            }
            else if (app_ret == RESULT_REDRAW)
            {
                g_scene_param.record_scene = SCENE_EXIT; //return�˳���main
                app_ret = RESULT_MAIN_APP;
            }
            else if(app_ret == RESULT_RECORD_MEMU)
            {
                g_scene_param.record_scene = SCENE_MENU_REC;    
            }
            else
            {
                g_scene_param.record_scene = SCENE_EXIT;
            }
            break;

            default:
            g_scene_param.record_scene = SCENE_EXIT; //exit
            break;
        }
    }

    return app_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _scene_result_deal(app_result_e result_val)
 * \��������ֵ����
 * \param[in]    result_val �������ȷ���ֵ para1
 * \param[out]   none
 * \return       none the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _scene_result_deal(app_result_e result_val)
{
    msg_apps_t msg;
    switch (result_val)
    {
        case RESULT_RADIO_PLAY:
        //���ص�radio���ų���
        msg.type = MSG_CREAT_APP; //�ָ�����radio
        msg.content.data[0] = APP_ID_RADIO;//radio ap
        msg.content.data[1] = PARAM_FROM_RECORD;//radio ap param
        send_async_msg(APP_ID_MANAGER, &msg); //�����첽��Ϣ,����radio ui�Ľ���
        //�ָ��趨��¼��Դ
        g_record_vars.ai_source = g_ai_source_bak;
        break;

        case RESULT_ALARM_QUIT:
        //if (need_reinstall == TRUE)
        {
            //��FM  ¼�����أ�����¼��codec ������fm �������룬��װ����
#ifndef PC
            //ж��fm ����
            sys_drv_uninstall(DRV_GROUP_FM);
            //��װfm  ����
            sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
#endif
        }
        radio_close_engine();
        //�ָ��趨��¼��Դ
        g_record_vars.ai_source = g_ai_source_bak;
        case RESULT_MAIN_APP:
        case RESULT_SCENE_EXIT:
        msg.type = MSG_CREAT_APP; //����main manu
        msg.content.data[0] = APP_ID_MAINMENU;//main ap
        msg.content.data[1] = PARAM_FROM_RECORD;//main ap param
        send_async_msg(APP_ID_MANAGER, &msg); //�����첽��Ϣ,����main manu�Ľ���
        break;

        case RESULT_BROWSER:
        msg.type = MSG_CREAT_APP; //����main manu
        msg.content.data[0] = APP_ID_BROWSER;//main ap
        msg.content.data[1] = PARAM_FROM_RECORD;//main ap param
        send_async_msg(APP_ID_MANAGER, &msg); //�����첽��Ϣ,����main manu�Ľ���
        break;

        case RESULT_APP_QUIT:
        if (get_engine_type() == ENGINE_RADIO)
        {
            //��FM  ¼�����أ�����¼��codec ������fm �������룬��װ����
#ifndef PC
            //ж��fm ����
            sys_drv_uninstall(DRV_GROUP_FM);
            //��װfm  ����
            sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
#endif
            //��װ���������ָ�������ȫ�����ݿռ�
            radio_modual_reinstall();
        }
        //�ָ��趨��¼��Դ
        g_record_vars.ai_source = g_ai_source_bak;
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int param)
 * \record ap main ������
 * \param[in]    int param
 * \param[out]   none
 * \return       int
 * \retval       none
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e app_ret;
    app_param_e enter_mode = (app_param_e) argc;

    if (enter_mode == PARAM_FROM_ALARM)
    {
        g_record_from_alarm = 1;
        enter_mode = PARAM_FROM_RADIO;
    }
    else if (enter_mode == PARAM_FROM_BROWSER)
    {
        enter_mode = PARAM_FROM_MAINMENU;
    }
    else
    {
        ;//QAC need
    }

    /* ����VM���� */
    _read_record_var();

    /*applib init and open ui/mcg files*/
    if (!_app_init())
    {
        g_scene_param.record_scene = SCENE_EXIT;
    }

    g_scene_param.record_status = REC_STOP_STATUS;

    //�л���LCD_BUFFER
    ui_set_display_buf(LCD_RAM_INDEX);

    //headbar init
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_RECORD);

//    Codec_init(0);
    app_ret = _scene_dispatch(enter_mode);
    Codec_exit();

    _scene_result_deal(app_ret);

    /*����VM����*/
    _save_record_var();

    //�ָ�ΪJPEG_RAM
    ui_set_display_buf(JPEG_RAM_INDEX);

    /*ע��applib��close ui/mcg files*/
    if (!_app_exit())
    {
        return 0;
    }

    return 1;
}

