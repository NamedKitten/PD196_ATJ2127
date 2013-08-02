/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mainmenu_main.c
 * \brief    MainMenu��main����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "main_menu.h"

/*ȫ�ֱ�������*/
//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT		2
/*!
 *  \brief
 *  app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
 */
app_timer_t g_mainmenu_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];
/*mainmenuӦ����Ҫ���浽vram������*/
g_mainmenu_var_t g_mainmenu_var;
/*ϵͳ��ȫ�ֱ���*/
setting_comval_t g_setting_comval;
/*�����ap ID*/
uint16 g_active_index = 0;
/*��������ʾ��Ӧ��ͼ�������������̨������ڣ�Ӧ��ͼ������ = Ӧ������+1*/
uint16 g_item_amount;
/*Ӧ��ͼƬ��STYLE ID����*/
uint16 ap_pic_box[MAX_APP_NUM + 1];

uint16 ap_pic_box_bg[MAX_APP_NUM + 1];

/*Ӧ�����Ƶ�STYLE ID���飬�������֣���Ƶ*/
uint16 ap_text_box[FRAME_NUM][MAX_APP_NUM + 1];
/*Ӧ����Ϣ��STYLE ID���飬�����ܹ���*/
uint16 ap_info_box[FRAME_NUM][MAX_APP_NUM + 1];
/*��ʽ����ʾap�����RES ID���ı���*/
uint16 ap_string_box[MAX_APP_NUM + 1];
/*��ʽ����ʾap��Ϣ��RES ID���ı���*/
uint16 ap_string_info_box[MAX_APP_NUM + 1];
/*Ӧ�õ��ļ�����*/
uint16 ap_file_amount[MAX_APP_NUM + 1];
/*ap�ļ�������ض�̬��ʾ��Ϣ���ַ�������*/
uint8 ap_string_infor[MAX_INFO_LEN];
//���ֱ��⻺����
uint8 g_title_buf[Id3BufSize];
//����ļ�ϵͳid
int32 g_music_mount_id;
//�ļ�·����Դ��Ϣ����
file_path_info_t g_file_path_info;
//���id3��Ϣ
id3_info_t g_id3_info;
//��ǰ����״̬��Ϣ
mengine_status_t g_music_status;
//�Ƿ���Ҫ��ʼ���ļ�ѡ����
bool need_fsel_init = TRUE;
//�Ƿ����и��־
bool need_check_status = FALSE;
//�ڼ�֡����
uint8 g_frame_num = 0;
//���ϻ��������л�ͼƬ�����������л���ʾ�Ķ�̬ͼƬ��һ����UI
mainmenu_ui_result_e g_direction;
//��ʱ��ID
int8 scroll_timer = -1;
//��ѯ״̬��ʱ��ID
int8 status_timer = -1;
//fmƵ��
uint32 g_fm_feq;
//��¼��Ӧ�õ��ļ��������±��Ǹ�Ӧ�õ�ID���
uint16 g_ap_file_num[MAX_APP_NUM];
//headbar���±�־
uint8 headbar_update;
//�Ƿ���music engine״̬��־λ
uint8 g_check_mengine_status;

/*main��Ҫ���������ֵ����ϵͳ��Ա����*/
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS;
#endif
INT8U prio = AP_FRONT_LOW_PRIO;

#ifdef PC
extern menu_cb_func get_menu_func(uint16 str_id);
extern menu_cb_leaf get_callback_func(uint16 str_id);
extern menu_cb_option get_menu_option(uint16 str_id);
#endif

//�����ж�����������
const uint8 themes_com[5][13] =
{ "common.sty", "cm_green.sty", "cm_gray.sty", "cm_pink.sty", "cm_red.sty" };

const uint8 themes_ap[5][13] =
{ "mainmenu.sty", "mainmenu.sty", "mainmenu.sty", "mainmenu.sty", "mainmenu.sty" };


/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ȡfmƵ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _radio_get_freq(void)
{
    msg_apps_t msg;
    msg_reply_t msg_reply;
    uint16 temp_freq;

    msg.type = MSG_FMENGINE_GET_FREQ;
    msg_reply.content = &temp_freq;

    //����ͬ����Ϣ
    send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        ;//do nothing
    }
    g_fm_feq = (uint32)temp_freq + 0x10000;//��̨�ظ���Ƶ�����������λ����Ҫ����
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_file_init(file_path_info_t* init_path_locat)
 * \��ʼ���ļ�ѡ����
 * \param[in]    init_path_locat ��ʼ���ļ�·����Ϣָ�� para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mainmenu_main.c
 * \note
 */
/*******************************************************************************/
bool music_file_init(file_path_info_t* init_path_locat)
{
    fsel_param_t init_param;//�ļ�ѡ������ʼ������
    bool init_result = FALSE;

    //�ļ�ѡ������ʼ���ӿڵĲ�������
    init_param.disk = init_path_locat->file_path.plist_location.disk;
    init_param.fsel_type = init_path_locat->file_source;
    init_param.fsel_mode = (g_setting_comval.music_comval.music_repeat_mode
            | g_setting_comval.music_comval.music_shuffle);

    init_param.file_type_bit = MUSIC_BITMAP;

    //�ļ�ѡ������ʼ��
    g_music_mount_id = fsel_init(&init_param, 0);

    if (g_music_mount_id != -1)
    {
        init_result = TRUE;
    }

    return init_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  MainMenu ��ȡӦ��VMȫ�ֱ���,��ʼ��ȫ�ֱ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _read_var(void)
{
    bool ret_val;
    uint8 engine_type;

    /*��ȡcommon VM����������Ϣ*/
    sys_vm_read(&g_setting_comval, VM_AP_SETTING, sizeof(g_setting_comval));
    /*��ʼ��ϵͳ��VM����*/
    if (g_setting_comval.g_comval.magic != MAGIC_SETTING)
    {
        g_setting_comval.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&g_setting_comval);
        sys_vm_write(&g_setting_comval, VM_AP_SETTING);
    }
    com_set_sys_comval(&g_setting_comval.g_comval);

    sys_vm_read(&g_mainmenu_var, VM_AP_MAINMENU, sizeof(g_mainmenu_var_t));
    /*��һ�ν��룬��ʼ��VM����*/
    if (g_mainmenu_var.magic != MAGIC_MAINMENU)
    {
        g_mainmenu_var.magic = MAGIC_MAINMENU;

        /*�������ļ���ȡAP ID�����飬������Ĭ�ϻap��*/
        com_get_config_struct(MAINMENU_AP_ID_ARRAY, (uint8 *) &g_mainmenu_var.cfg_var, sizeof(g_mainmenu_var.cfg_var));

        sys_vm_write(&g_mainmenu_var, VM_AP_MAINMENU);
    }
    /*������Ӧ���ļ�����*/
    sys_vm_read(&g_ap_file_num, VM_FILE_NUM, sizeof(g_ap_file_num));

    engine_type = get_engine_type();
    if (engine_type == ENGINE_RADIO)
    {
        _radio_get_freq();
    }
    else if (engine_type == ENGINE_MUSIC)
    {
        music_get_filepath(&g_file_path_info);

        if (need_fsel_init == TRUE)
        {
            //�ļ�ѡ������ʼ��
            ret_val = music_file_init(&g_file_path_info);
            need_fsel_init = FALSE;
        }
        else
        {
            ret_val = TRUE;
        }

        if (ret_val == FALSE)
        {
            //��̨���ڲ���״̬���ļ�ѡ������ʼ�����ɹ�
            //�رպ�̨
            music_close_engine();
        }
        else
        {
            //����״̬��Ҫ�������״̬
            if (get_engine_state() == ENGINE_STATE_PLAYING)
            {
                need_check_status = TRUE;
            }
        }
    }
    else
    {
        ;//nothing
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  applib��ʼ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _app_init(void)
{
    /*����VM����*/
    _read_var();

    /*��ʼ��applib�⣬ǰ̨AP*/
    applib_init(APP_ID_MAINMENU, APP_TYPE_GUI);

    /*��ʼ����ʱ��*/
    init_app_timers(g_mainmenu_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    /*��ʼ�� applib ��Ϣģ��*/
    applib_message_init(mainmenu_app_msg_callback);

    /*��ʼ��ϵͳ��ʱ��*/
    sys_timer_init();

    /*��ap��common��style�ļ�*/
    ui_res_open(themes_ap[g_setting_comval.g_comval.theme], UI_AP);
    ui_res_open(themes_com[g_setting_comval.g_comval.theme], UI_COM);

    /*�򿪲˵������ļ�*/
    com_open_confmenu_file("mainmenu.mcg");

#ifdef PC
    load_menulist_simfunc(get_menu_func, get_callback_func, get_menu_option);
#endif
}


/******************************************************************************/
/*!
 * \par  Description:
 *	  applib�˳�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _app_exit(void)
{
    if (need_fsel_init == FALSE)
    {
        //�ļ�ѡ�����˳�
        fsel_exit();
    }
    /*����ϵͳ��ʱ��*/
    sys_timer_exit();

    /*����VM����*/
    sys_vm_write(&g_mainmenu_var, VM_AP_MAINMENU);
    sys_vm_write(&g_setting_comval, VM_AP_SETTING);

    /*�ر�ui��Դ�ļ�*/
    ui_res_close(UI_AP);
    ui_res_close(UI_COM);

    /*�رղ˵������ļ�*/
    com_close_confmenu_file();

    /*ִ��applib���ע������*/
    applib_quit();
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  MainMenu Ӧ�õ���ں����ͳ���������ģ��
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e result;
    scene_mainmenu_e scene_next = SCENE_MAINMENU;

    /*ap��ʼ��*/
    _app_init();

    /*����������*/
    while (scene_next != SCENE_EXIT)
    {
        switch (scene_next)
        {
            case SCENE_MAINMENU:
            result = mainmenu_desktop();
            if (result == RESULT_OPTION_MAINMENU)
            {
                scene_next = SCENE_OPTIONMENU;
            }
            else
            {
                scene_next = SCENE_EXIT; //exit
            }
            break;

            case SCENE_OPTIONMENU:
            result = option_menulist();
            if (result == RESULT_REDRAW)
            {
                scene_next = SCENE_MAINMENU;
            }
            else
            {
                scene_next = SCENE_EXIT; //exit
            }
            break;

            default:
            scene_next = SCENE_EXIT; //exit
            break;
        }

    }

    _app_exit();

#ifdef PC
    printf("mainmenu exit \n");
#endif
    return result;
}
