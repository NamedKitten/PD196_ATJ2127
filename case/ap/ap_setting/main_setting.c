/********************************************************************************
 *                              USDK
 *                             Module: SETTING
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : main_setting.c
 * History:
 *      <johnsen>    <2011-8-25 13:55>           <V1.0 >             <desc>
 *
 ********************************************************************************/
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ap_setting.h"

/* define variable, �������� */
setting_comval_t g_setting_vars; //ϵͳ��������
//��ʱ���ж��������;����ҽ��ж���ں���
app_timer_t setting_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT]; 
style_infor_t style; //����ļ�����
textbox_private_t textbox_param; //�ı��ؼ�����
uint8 string_buffer[TIPS_BUFFER_SIZE];   //���ݴ�������
uint8 *res_filename;    //���SD�ļ���ָ��
menu_com_data_t menu_com;   //�˵��б�ؼ���ʼ���ṹ��
date_t date;    //����
time_t time;    //ʱ��
//music�������״����ʶ����ע��ֵΪENGINE_MUSIC/ENGINE_NULL
engine_type_e music_exist_flag; 
uint8 playlist_exist_flag;  //�Ա���ȷ��ʾ
uint8 current_disk; //���ڱ�ʾ��ǰ���ŵ��̺�
//�����������ڲ����϶���λ�ÿɱ䣬Ϊ�����������±�����¼�������������λ��
uint8 date_dd_location, date_mm_location, date_yy_location;
int32 g_vfs_mount_id;   //��¼��ǰ���ʴ���ID��-1��ʾδ��ʽ��
uint8 need_quit_ap_flag;    //���ڼ�¼�Ƿ���Ҫ�����˳�ap
uint8 set_language_id_argv;

#ifdef PC
extern menu_cb_func _get_menu_func(uint16 str_id);
extern menu_cb_leaf _get_callback_func(uint16 str_id);
extern menu_cb_option _get_menu_option(uint16 str_id);
#else
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS; //�߳�ջ��ָ��
#endif
INT8U prio = AP_FRONT_LOW_PRIO;     //�������ȼ�

const uint8 theme_setting_res[5][13] =
{ "setting.sty", "setgreen.sty", "set_gray.sty", "set_pink.sty", "set_red.sty" };

const uint8 themes_com[5][13] =
{
    "common.sty", //Ĭ�Ϸ��������ģ��������ɫ���
    "cm_green.sty", //���������������д+������������ɫ���
    "cm_gray.sty", //��ɫ���
    "cm_pink.sty", //�ۺ�ɫ���
    "cm_red.sty" //��ɫ���
};

const uint8 flash_drv_name[] =
{ "nand.drv" };

const uint8 card_drv_name[] =
{ "card.drv" };

#if 0
static const int8 user_srs_param_default[MAX_EQ_SEG] =
{ 8, 8, 5, 4, 6, 1, 0, 30, 1, 0, 0, 0 };//srs user default

static const int8 user_eq_param_default[MAX_EQ_SEG] =
{ -9, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 }; //user eq default
#endif

#ifdef USE_83NAME_CREAT
const char mplist_name[]={"MUSIC   LIB"};
#else
const char mplist_name[]={"MUSIC.LIB"};
#endif

/******************************************************************************/
/*                    
 * \par  Description: mbyte to wchar
 *	  
 * \param[in]    dest��src

 * \param[out]      
 
 * \return  UNICODE ��������������ʶ���ͽ�����     
  
 * \note    ASCת��UNICODE���Զ������ʶ���ͽ�����
*******************************************************************************/
static uint16 mbyte_to_wchar(uint8 *dest,uint8 *src)
{
    uint16 num=0;
    
#ifdef USE_83NAME_CREAT

    while(*src!=0x00)
    {
        *dest=*src;
        dest++;
        src++;
        num++;
    }
    
    dest=0x00;

#else    

    *dest=0xff;
    *(dest+1)=0xfe;
    dest+=2;
    
    while(*src!=0x00)
    {
        *dest=*src;        
        *(dest+1)=0x00;     
        dest+=2;       
        src++;
        num++;
    }
    
    *dest=0x00;
    *(dest+1)=0x00;
    
#endif
    
    return num;
    
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool check_plist_exit(void)
 * \����б��Ƿ����
 * \param[in]    none  
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      main_setting.c
 * \note
 */
/*******************************************************************************/
bool check_plist_exit(void)
{
    uint32 plist_fp;
    bool check_ret;
    vfs_cd(g_vfs_mount_id,CD_ROOT,0);
     //���ļ�
    mbyte_to_wchar(string_buffer,(uint8 *)mplist_name);
    plist_fp = vfs_file_open(g_vfs_mount_id, string_buffer, R_FAST_SEEK);     
    if (0 == plist_fp)
    {
        check_ret = FALSE;
    }
    else
    {
        vfs_file_close(g_vfs_mount_id, plist_fp);
        check_ret = TRUE;
    }	
    return check_ret;   
}

/*
 *****************************************************************************
 * \par  Description:��ȡVM������Ϣ
 *      ��ͨ˵��
 *      �ص�˵��
 * \param[in]    int:
 * \param[in]    void *:
 * \param[out]   void *:
 * \retval       bool
 * \note: ע������˵��
 *
 ******************************************************************************
 */
void read_var(void)
{
    /*��ȡSetting VM����������Ϣ*/
    sys_vm_read(&g_setting_vars, VM_AP_SETTING, sizeof(g_setting_vars));
    /*��һ�ν��룬��ʼ��VM����*/
    if (g_setting_vars.g_comval.magic != MAGIC_SETTING)
    {
        g_setting_vars.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&g_setting_vars);
        //libc_memcpy(g_setting_vars.music_comval.eq_parameter,user_eq_param_default, MAX_EQ_SEG);
        //libc_memcpy(g_setting_vars.music_comval.srs_parameter,user_srs_param_default, MAX_EQ_SEG);
        sys_vm_write(&g_setting_vars, VM_AP_SETTING);
    }
    com_set_sys_comval(&g_setting_vars.g_comval);
    sys_vm_read(&g_setting_vars, VM_AP_SETTING, sizeof(g_setting_vars));
    sys_vm_write(&g_setting_vars, VM_AP_SETTING);
    sys_vm_read(&g_setting_vars, VM_AP_SETTING, sizeof(g_setting_vars));
    sys_vm_write(&g_setting_vars, VM_AP_SETTING);
    
    music_exist_flag = get_engine_type(); //��̨�����Ƿ����
}


/******************************************************************************/
/*!
 * \par  Description:
 * \void  setting_callback(void)
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       ap�˳����ߴ�������ap    
 * \ingroup      setting
 * \note
 */
/*******************************************************************************/
app_result_e setting_callback(private_msg_t *private_msg)
{
    app_result_e result = RESULT_NULL;
    engine_type_e engine_type;
    msg_apps_t msg;
    
    switch (private_msg->msg.type)
    {
        case MSG_SD_OUT:
        if (current_disk == DISK_H)
        {
            //���ڷ��ʿ����ļ����˻ص���Ӧ��
            engine_type = get_engine_type();

            if (engine_type == ENGINE_MUSIC)
            {
                //��Ϣ����(����Ϣ����)ɱ����̨����
                msg.type = MSG_KILL_APP_SYNC;
                msg.content.data[0] = APP_ID_MENGINE;
                //����ͬ����Ϣ
                send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
                music_exist_flag = ENGINE_NULL;
            }
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
            result = RESULT_MAIN_APP;
        }
        break;
        
        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;

        default:
        result = com_message_box(private_msg->msg.type);
        break;
    }  
    return result;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e setting_menu_func(void *menu_title_void)
 * \param[in]    void *menu_title_void ���˵���ȷ���������ص�ִ�нṹ��ָ��
 * \param[out]   ����ĸ��²˵��ṹ����
 * \return       �˵���Ϊ
 * \ingroup      setting
 * \note
 */
/*******************************************************************************/
app_result_e setting_menu_func(void *menu_title_void)
{
    menu_title_action_t* menu_title_restore;

    menu_title_restore = (menu_title_action_t*) menu_title_void;
    if(g_setting_vars.g_comval.online_device == VALUE_STORAGE_MTP)
    {
        menu_title_restore->menu_entry = MENU_ENTRY_ID_MAIN_MTP; //�������    
    }
    else
    {
        menu_title_restore->menu_entry = MENU_ENTRY_ID_MAIN; //�������
    }    
    menu_title_restore->str_id = -1;
    menu_title_restore->ret_str = NULL;
    menu_title_restore->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_file_init(file_path_info_t* init_path_locat)
 * \��ʼ���ļ�ѡ����
 * \param[in]    init_path_locat ��ʼ���ļ�·����Ϣָ�� para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      setting
 * \note
 */
/*******************************************************************************/
bool music_file_init(file_path_info_t* init_path_locat)
{
    fsel_param_t init_param;//�ļ�ѡ������ʼ������
    uint8* drv_name;

    bool init_result = FALSE;

    //�ļ�ѡ������ʼ���ӿڵĲ�������
    init_param.disk = init_path_locat->file_path.plist_location.disk;

    init_param.fsel_type = init_path_locat->file_source;

    init_param.fsel_mode = (g_setting_vars.music_comval.music_repeat_mode | g_setting_vars.music_comval.music_shuffle);

    init_param.file_type_bit = MUSIC_BITMAP;

    if (init_param.disk == DISK_C)
    {
        drv_name = (uint8*) flash_drv_name;
    }
    else
    {
        drv_name = (uint8*) card_drv_name;
    }
    //�ļ�ѡ������ʼ��
    g_vfs_mount_id =  fsel_init(&init_param, 0);

    if (g_vfs_mount_id != -1)
    {
        init_result = TRUE;
    }

    return init_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_filepath0(file_path_info_t *filepath)
 * \��ȡ��ǰ�����ļ���·����Ϣ
 * \param[in]    filepath����ļ�·��buffer��ָ��  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      setting
 * \note
 */
/*******************************************************************************/
bool music_get_filepath0(file_path_info_t *filepath)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;
    //��ȡ��ǰ�����ļ���·����Ϣ
    reply.content = filepath;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    setting Ӧ�õĳ�ʼ��
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \retval       1    success
 * \retval       0    fail
 * \ingroup      setting
 * \note
 *******************************************************************************/
bool setting_app_init(void)
{
    //�ļ�·����Դ��Ϣ����
    file_path_info_t g_file_path_info; //����ʱ����

    /*��ʼ��applib�⣬ǰ̨AP;�������*/
    if (FALSE == applib_init(APP_ID_SETTING, APP_TYPE_GUI))
    {
        while (1)
        {
            ; //do nothing
        }
    }

    /*��ʼ����ʱ��;�������*/
    init_app_timers(setting_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    /*��ʼ�� applib ��Ϣģ�飬������� */
    applib_message_init(setting_callback);

    /*��ʼ��ϵͳ��ʱ��;����������ڶ�ȡVM֮��*/
    sys_timer_init();

    //��ʼui��Դ�ļ�
#if 0
    if (FALSE == ui_res_open("common.sty", UI_COM))
    {
        while (1)
        {
            ; //do nothing
        }
    }

    if (FALSE == ui_res_open("setting.sty", UI_AP))
    {
        while (1)
        {
            ; //do nothing
        }
    }
#else
    ui_res_open(theme_setting_res[g_setting_vars.g_comval.theme], UI_AP);
    ui_res_open(themes_com[g_setting_vars.g_comval.theme], UI_COM);
#endif

    //��ǰ̨Ӧ�ò˵����ýű�
    if (FALSE == com_open_confmenu_file("setting.mcg"))
    {
        while (1)
        {
            ; //do nothing
        }
    }

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
#endif
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_SETTING);

    
    //��ʼ���ļ�ѡ����
    g_file_path_info.file_path.plist_location.disk = DISK_C; //DISK_H ���� DISK_U uhost ��
    current_disk = DISK_C;
    //��ʼ���ļ�ѡ����ΪĿ¼ģʽ��һ���ɹ�
    g_file_path_info.file_source = FSEL_TYPE_COMMONDIR;

    //����������ȡ��ǰ�����ļ���·����Ϣ
    if (music_exist_flag == ENGINE_MUSIC)
    {
        music_get_filepath0(&g_file_path_info);//��ȡ�ļ�·��
        current_disk = g_file_path_info.file_path.plist_location.disk;
    }
    //�ļ�ѡ������ʼ��
    music_file_init(&g_file_path_info);
    if (g_vfs_mount_id != -1)
    {
        if(TRUE==check_plist_exit())
        {
            playlist_exist_flag = 1;     
        }
        else
        {
            playlist_exist_flag = 0;
        }    
            
    }
    else
    {
        playlist_exist_flag = 0;
    }    
    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    setting Ӧ�õ��˳�
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \retval       1    success
 * \retval       0    fail
 * \ingroup      setting
 * \note
 *******************************************************************************/
bool setting_app_deinit(void)
{

    //�ļ�ѡ�����˳�
    fsel_exit();

    //�������ֵ
    sys_timer_exit();

    //�ر�Ӧ�ò˵����ýű�
    com_close_confmenu_file();

    //�ر�ui��Դ�ļ�
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    //ִ��applib���ע������
    applib_quit();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  setting�������ģ��
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      setting
 * \note
 *******************************************************************************/

static app_result_e setting_proc_result(app_result_e result)
{
    msg_apps_t msg;
    uint8 temp_flag = 0 ;
    msg.type = MSG_CREAT_APP; //��process manager���ʹ�������������Ϣ

    switch (result)
    {
        case RESULT_MAIN_APP:
        msg.content.data[0] = APP_ID_MAINMENU;
        msg.content.data[1] = (uint8) PARAM_FROM_SETTING;
        break;

        case RESULT_NOWPLAYING:
        msg.content.data[0] = APP_ID_MUSIC;
        msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYING;
        break;        
        

        case RESULT_MUSIC_PLAY:
        msg.content.data[0] = APP_ID_MUSIC;
        msg.content.data[1] = (uint8) PARAM_FROM_SETTING;
        break;

        case RESULT_USB_TRANS:
        msg.content.data[0] = APP_ID_UDISK;
        msg.content.data[1] = (uint8) PARAM_FROM_SETTING;
        break;
        case RESULT_APP_QUIT:
        temp_flag = 1;
        break;
        default:
        msg.content.data[0] = APP_ID_MAINMENU;
        msg.content.data[1] = (uint8) PARAM_FROM_SETTING;
        break;
    }
    //��process manager���ʹ���������Ϣ
    if(temp_flag!=1)
    {
        send_async_msg(APP_ID_MANAGER, &msg);
    }
    return result;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��
 *      �ص�˵��
 * \param[in]    int:
 * \param[in]    void *:
 * \param[out]   void *:
 * \retval       bool
 * \note: ע������˵��
 *
 ******************************************************************************
 */
int main(int argc, const char *argv[])
{
    app_result_e result;
    scene_e setting_next_scene;

    /* ����VM����;������� */
    read_var();

    /*��ʼ��*/
    if (setting_app_init() == FALSE)
    {
        result = RESULT_MAIN_APP;
        setting_next_scene = SCENE_EXIT;
    }
    else
    {
        setting_next_scene = SETTING_MENU_LAYER;
    }

    /*����������*/
    while (setting_next_scene != SCENE_EXIT)
    {
        menu_com.app_id = APP_ID_SETTING;
        if (g_setting_vars.g_comval.online_device == VALUE_STORAGE_MTP)
        {
            menu_com.menu_entry = MENU_ENTRY_ID_MAIN_MTP;
        }
        else
        {
            menu_com.menu_entry = MENU_ENTRY_ID_MAIN;
        }

        menu_com.menu_func = setting_menu_func;
        menu_com.path_id = 0;
        result = gui_menulist(MENULIST, &menu_com);
        {
            setting_next_scene = SCENE_EXIT;
        }
    }

    setting_proc_result(result);

    /*����VM����*/
    sys_vm_write(&g_setting_vars, VM_AP_SETTING);
    setting_app_deinit();
    return result;
}
