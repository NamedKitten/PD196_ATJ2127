/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ebook.h"

/****************************************************************************/

/* define variable, �������� */
const uint8 app_id[3] =
{ APP_ID_MAINMENU, APP_ID_MUSIC, APP_ID_RADIO };

 uint8 device_drv_name[MAX_DEVICE_NUM][10] = //Ϊ��ʡ�ռ䣬ȥ��const����
{ "nand.drv", "card.drv" };

const uint8 device_drv_index[MAX_DEVICE_NUM] =
{ DISK_C, DISK_H };


const uint8 key_drv_name[] =
{ "key.drv" };

const uint8 com_themes[5][13] =
{ "common.sty", "cm_green.sty", "cm_gray.sty", "cm_pink.sty", "cm_red.sty" };
const uint8 ebook_sty[] =
{ "ebook.sty" };
const uint8 ebook_mcg[] =
{ "ebook.mcg" };

#ifndef PC
OS_STK *ptos = (OS_STK *) AP_EBOOK_STK_POS;     //������Ӧ�õ�����ջ
#endif

INT8U prio = AP_EBOOK_PRIO;                     //������Ӧ�õ����߳����ȼ�
    
/********************************************************************************
 * Description :Ӧ�õ���ں���
 *
 * Arguments  :
 *              argc:����Ӧ�ó����Ĳ���
 *              argv:
 * Returns     :
 *                ��
 * Notes       :
 *
 ********************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e retval;  
    engine_type = get_engine_type();            //��ȡ��̨�������� 
    if (argc == PARAM_FROM_BROWSER)             //����� browser ap ����
    {
        enter_mode = ENTER_FROM_FILELIST;       //���Ǵ��ļ��б���볡��
    }

    /*!ebookӦ�ó�ʼ��*/
    retval = _ebook_init();

    if (retval == RESULT_NULL)                       //�����ʼ���ɹ�
    {
        /*!ebookӦ�ù��ܳ�������*/
        retval = _ebook_scene_dispatch(argc);   //����ͻ��л�����ebook�Ľ��棬 ebook �ĳ����л�

        if (argc == PARAM_FROM_BROWSER)         //��� �� browser ap ����
        {
            //browser�˻ز����б�
            if((retval == RESULT_EBK_FILELIST) || (retval == RESULT_MAIN_APP))   
            {
                retval = RESULT_BROWSER;            //�����������Ϣ
            }
        }
    }
    else if(retval == RESULT_ERROR)
    {   
        device_status |= 0x01;                  //���flashû���ļ�
        retval = RESULT_MAIN_APP;               //�ص� MainMenu Ӧ��
    }
    else
    {
        //ʣ�� retval=RESULT_CREATE_LIST_APP
    }
    

    /*!�˳�ebookӦ��*/
    _deal_exit_result(retval);  //�˳�ebookӦ��ʱ ������Ӧ�ķ��ؽ��
    _ebook_exit();

    return argc;
}


/********************************************************************************
 * Description :������Ӧ�õĳ�������
 *
 * Arguments  :
 *              param:����ģʽ
 * Returns     :
 *                Ӧ�ô����ķ��ؽ��
 * Notes       :
 *
 ********************************************************************************/
int _ebook_scene_dispatch(int param)
{
    //    scene_enter_mode_e enter_mode;
    app_result_e scene_next, retval = RESULT_NULL;      //val;
    if (param == PARAM_FROM_BROWSER)                    //����� browser ap ����
    {
        scene_next = RESULT_EBK_PLAY;                   //�����һ������Ϊ��ebook �Ĳ��Ž���
    }
    else
    {
        scene_next = RESULT_EBK_FILELIST;               //��������һ������Ϊ��ebook ���ļ��б����
    }
    enter_mode = ENTER_FROM_DESKTOP;                    //��main ap ��browser���볡��

    /*����������*/
    while (retval != RESULT_EBOOK_BASE)                 //�ȴ� ������Ӧ�÷���
    {
        /*������ʾͼƬ*/
        _clear_and_set_screen(0xffff, 0x0);             //����������֤����ɾ�
        numboxflag = TRUE;                              //�����Ҫȫˢnumbox

        switch (scene_next)
        {
            case RESULT_EBK_FILELIST:                   //�ļ��б���
            scene_next = _ebook_filelist(enter_mode);   //����������ļ��б�ѡ��˵�
            break;
            case RESULT_EBK_SETTINGMENU:                //�˵����ó���
            case RESULT_EBK_READINGMENU:
            scene_next = _ebook_option_menu(enter_mode);
            break;
            case RESULT_EBK_PLAY:                       //�Ķ����泡��
            scene_next = _ebook_reading(enter_mode);    //�������Ķ��������ں������л���ebook �Ĳ��Ž��棬���������
            if( (param == PARAM_FROM_BROWSER) && (scene_next == RESULT_EBK_FILELIST) )
            {
                //����˻ص��ļ������б�����BROWSER����AP�ģ����˳�AP�����ɽ���BROWSER���ļ��б�
                retval = RESULT_EBOOK_BASE;             //��ǣ�������Ӧ�÷���
            }
            break;
            default:
            retval = RESULT_EBOOK_BASE;                 //��ǣ�������Ӧ�÷���
            break;
        }
        

        
    }

    if(g_ebook_mount_id >= 0)
    {
        _close_text_file(1);                        //�ر��Ѵ򿪵��ļ��ļ����
    }
    return scene_next;
}


/********************************************************************************
 * Description :Ӧ��������Ϣ��ʼ��
 *
 * Arguments  :
 *
 * Returns     :
 *                0-�ļ�ѡ������ʼ��ʧ�ܡ�1-��ʼ���ɹ�
 * Notes       :
 *            ��Ҫ����vram������У�飬��ʼ������Դ�ļ����صȹ���
 ********************************************************************************/
app_result_e _ebook_init(void)
{
    /* ����VM���� ��VM�ռ����ڱ���Ӧ�û�������*/
    _ebook_read_var();                                      //ȫ�ֱ�����ȡ�ͳ�ʼ��
    _check_play_device();                                   //����̨���ŵ��̷�
    g_ebook_vars.path.file_source = FSEL_TYPE_LISTEBOOK;    //EBOOK�б�
    if (enter_mode == ENTER_FROM_FILELIST)                  //������ļ��б���볡��
    {
        cur_file_diroffset = g_ebook_vars.path.file_path.dirlocation.dir_entry; 
                                                            //��ǰ�ı��ļ���Ŀ¼��Ϣ�е�Ŀ¼ƫ��

        sys_vm_read(&g_ebook_vars.path, VM_AP_BROWSER, sizeof(g_ebook_vars.path));
                                                            //��ȡVM����ĵ������ļ��Ĳ���·����Ϣ

        g_ebook_vars.path.file_source = FSEL_TYPE_COMMONDIR;//Ŀ¼����

        if (    (music_play_device != g_ebook_vars.path.file_path.dirlocation.disk) 
                                                            //�����̨���ֲ��ŵ��̷�����VM�����
            &&  (music_play_device != 0)                    //�Һ�̨���ֲ��ŵ��̷���Ϊ0
            )
        {
            _close_engine();                                //֪ͨ��̨�����˳�
        }
    }
    else if ((music_play_device != g_ebook_vars.path.file_path.dirlocation.disk) && (music_play_device != 0))
    {
        g_ebook_vars.path.file_path.dirlocation.disk = music_play_device;
    }
    else
    {
    }
    //��ʼ��Ӧ���ڵ�ȫ�ֱ���
    _ebook_param_init();
    
    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_EBOOK, APP_TYPE_GUI);

    //��ʼ����ʱ��
    init_app_timers(ebook_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init(_ebook_scene_msg_dispatch);

    sys_timer_init();

    /*��ȡui��Դ�ļ�*/
    ui_res_open(ebook_sty, UI_AP);
    ui_res_open(com_themes[g_comval.theme], UI_COM);
    com_open_confmenu_file(ebook_mcg);

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
    com_clear_all_history(PATH_HISTORY_MENU);
#endif

    /*��ʼ���ļ�ѡ����*/
    if(_ebook_filesel_init() == RESULT_CREATE_LIST_APP)
    {
        return RESULT_CREATE_LIST_APP;
    }

    /*�ļ�ѡ������ʼ�������˳�*/
    if (fsel_get_error() != FSEL_NO_ERR)
    {
        return RESULT_ERROR;
    }
    
    return RESULT_NULL;
}

/********************************************************************************
 * Description :�˳�Ӧ��ʱ������Ϣ�������Դж��
 *
 * Arguments  :
 *              param:����ģʽ
 * Returns     :
 *                ��
 * Notes       :
 *
 ********************************************************************************/
void _ebook_exit(void)
{
#ifdef PC
    uint8 temp[512];                    //�ȿ���512�ֽڵ�ջ�ռ䣬�Ա�setting_comval��д512�ֽڵ�ʱ�����㹻�ռ�
                                        //������PC���϶�ȡ512�ֽڣ�������Χ��
#endif
    setting_comval_t setting_comval; 
    
    //��ȡ���õı��� 
    sys_vm_read(&setting_comval, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&setting_comval.g_comval, &g_comval, sizeof(comval_t));
    /*����VM����*/
    sys_vm_write(&g_ebook_vars, VM_AP_EBOOK);
    sys_vm_write(&setting_comval, VM_AP_SETTING);

    fsel_exit();
    sys_timer_exit();

    //�ر�Ӧ�ò˵����ýű�
    com_close_confmenu_file();

    //�ر�ui��Դ�ļ�
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);
    //    ui_set_display_buf(JPEG_RAM_INDEX);
    //ִ��applib���ע������
    applib_quit();
}
/********************************************************************************
 * Description :Ӧ���ڵ�ȫ�ֱ�����ʼ��
 *
 * Arguments  :
 *              ��
 * Returns     :
 *                ��
 * Notes       :
 *
 ********************************************************************************/
void _ebook_param_init(void)
{
    bookmark_total = 0xFF;
    support_card_flag = 1;
}
/********************************************************************************
 * Description :ȫ�ֱ�����ȡ�ͳ�ʼ��
 *
 * Arguments  :
 *              ��
 * Returns     :
 *                ��
 * Notes       :
 *
 ********************************************************************************/

void _ebook_read_var(void)
{
    int result;
    setting_comval_t setting_comval;
    //��ȡ���õı���
    result = sys_vm_read(&setting_comval, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&g_comval, &setting_comval.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);
    if (g_comval.magic != MAGIC_COMVAL)
    {
        g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&g_comval);
        sys_vm_write(&g_comval, VM_AP_SETTING);
    }
    //    g_comval.language_id = CHINESE_SIMPLIFIED;

    //��ȡebookʹ�õı���
    result = sys_vm_read(&g_ebook_vars, VM_AP_EBOOK, sizeof(g_ebook_vars));
    if ((g_ebook_vars.magic != MAGIC_EBOOK) || (g_ebook_vars.path.file_path.dirlocation.disk == 0x0))
    {
        //�趨Ĭ��ֵ
        libc_memset(&g_ebook_vars, 0, sizeof(g_ebook_vars));
        g_ebook_vars.path.file_source = FSEL_TYPE_LISTEBOOK;
        g_ebook_vars.path.file_path.dirlocation.disk = DISK_C;
        g_ebook_vars.autoplaytime = 10;
        g_ebook_vars.magic = MAGIC_EBOOK;
        sys_vm_write(&g_ebook_vars, VM_AP_EBOOK);
    }
}

/********************************************************************************
 * Description :ebookӦ���й��ļ�ѡ�����ĳ�ʼ��
 *
 * Arguments  :
 *
 * Returns     :
 *                ��
 * Notes       :
 *
 ********************************************************************************/
app_result_e _ebook_filesel_init(void)
{
    fsel_param_t ebook_fsel_param;//�ļ�ѡ������ʼ������
    uint8 i = 0, disk_count = 0;
    uint8 count_num=2;
    uint8 fsel_err_msg;
    uint16 error_id;
    app_result_e result;
    
    /*��ʼ��ebookӦ�õ��ļ�ѡ����*/
    ebook_fsel_param.fsel_type = g_ebook_vars.path.file_source;             //FSEL_TYPE_LISTEBOOK;//FSEL_TYPE_COMMONDIR;
    ebook_fsel_param.fsel_mode = FSEL_MODE_NORMAL;                          //��ͨ����
    ebook_fsel_param.brow_type = FSEL_BROW_ALL;                             //��ʾȫ��(�ļ���Ŀ¼)
    ebook_fsel_param.disk = g_ebook_vars.path.file_path.dirlocation.disk;   //�����̷�
    ebook_fsel_param.file_type_bit = TEXT_BITMAP;                           //���Ӧ�ú�׺��,����ֻ��ʾtxt�ļ�

    /* �ȼ���Ƿ���Ҫ��ʼ��SD�� */
    if(ebook_fsel_param.disk == DISK_H)                                     //������Ҫ��ʼ��SD��
    {
        if(_detect_device(DRV_GROUP_STG_CARD) == FALSE)                     //����SD��������
        {
            ebook_fsel_param.disk   = g_ebook_vars.path.file_path.dirlocation.disk
                                    = device_drv_index[0];                  //�л���Flash��
            count_num=1;                                                    //���ڱ�ǵ��µ�whileѭ������Ҫ�ټ��SD��
        }
    }

    /* ������������̷� */
    while (i < 2)                                                           //����������Ƿ�����
    {
        if (ebook_fsel_param.disk == device_drv_index[i]) 
        {
            disk_count++;                                      
            g_ebook_mount_id = fsel_init(&ebook_fsel_param, 0);             //��ʼ����        
            fsel_err_msg=fsel_get_error();                                  //��ȡ��ʼ�����
            if(fsel_err_msg==FSEL_ERR_LIST_NOEXIST)                         //�����ղؼл򲥷��б�����
            {
                result = _show_double_dialog(S_IS_CREATE_PLAYLIST, NULL);      //�������������б�˵�ѡ��
                if (result == RESULT_DIALOG_YES)
                {
                    g_ebook_vars.path.file_path.dirlocation.disk = device_drv_index[i];      //����ŷ�
                    //_close_engine();                                            //֪ͨ��̨�����˳�
//                    if(g_ebook_mount_id < 0)
//                    {
//                        fsel_exit();                                                //�˳���
//                    }
                    return RESULT_CREATE_LIST_APP;                              //���봴�������б�ap
                }
                else if (result != RESULT_DIALOG_NO  )                          //�˳��Ի���
                {
                    //�п����� RESULT_REDRAW
                    return result;
                }
                else
                {
                    //QAC
                }

                
                //�����������������                
                if(i==0)
                {
                    error_id=S_DISK_NOFILE;                                 //��Ǵ������ļ�
                }
                else
                {
                    error_id=S_CARD_NOFILE;                                 //��ǿ����ļ�
                }                
            }
            else 
            {
                error_id=S_DISK_ERROR;                                      //��Ǵ��̴���                   
            }
            if(fsel_err_msg!=FSEL_NO_ERR)                                   //�����б���
            {
                _show_single_dialog(error_id, DIALOG_INFOR_WAIT);           //�ڽ�������ʾ����
            }
            if ((g_ebook_mount_id == -1) && (disk_count < count_num))       //Ӧ���ļ�ϵͳ��������Ϊ-1������û�����
            {                
                if(ebook_fsel_param.disk == device_drv_index[0])            //���������Flash
                {
                    if(_detect_device(DRV_GROUP_STG_CARD) == FALSE)         //����Flash������(û��ʽ��)
                    {
                        break;
                    }
                }
                fsel_exit();                                                //�˳���
                i = 1 - i;                                                  //�л�����һ���̷�  
                ebook_fsel_param.disk   = g_ebook_vars.path.file_path.dirlocation.disk
                                        = device_drv_index[i];              
                
            }
            else
            {
                break;
            }
        }
        else
        {
            i++;
        }
    }
    return RESULT_NULL;
}

/********************************************************************************
 * Description :�˳�ebookӦ��ʱ������Ӧ�ķ��ؽ��
 *
 * Arguments  :
 *              result:Ӧ�÷�����Ҫ����Ľ��
 *
 * Returns     :
 *                ��
 * Notes       :
 *
 ********************************************************************************/

void _deal_exit_result(app_result_e result)
{
    msg_apps_t msg;
    uint8 need_send_msg = TRUE;
    //��process manager���ʹ�������������Ϣ
    msg.type = MSG_CREAT_APP;                               //�첽����Ӧ�ó�����Ϣ
    msg.content.data[1] = (uint8) PARAM_FROM_EBOOK;         //�� ebook ap ���� 
    switch (result)
    {
        case RESULT_MAIN_APP:                               //�ص� MainMenu Ӧ��
        msg.content.data[0] = APP_ID_MAINMENU;
        break;

        case RESULT_BROWSER:                                //�����������Ϣ
        msg.content.data[0] = APP_ID_BROWSER;
        break;
        case RESULT_CREATE_LIST_APP:                        //���봴�������б�ap
        msg.content.data[0] = APP_ID_PLAYLIST;
        if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_H)
        {
            //ֻ��������
            msg.content.data[1] = (PARAM_FROM_EBOOK | PLIST_DISK_H);//playlist ap param
        }
        else
        {
            //����������
            msg.content.data[1] = (PARAM_FROM_EBOOK | PLIST_DISK_ALL);//playlist ap param
        }
        break;
        case RESULT_NOWPLAYING:                             //���ڲ��Ž�����Ϣ
        msg.content.data[0] = app_id[engine_type];
        if (engine_type == ENGINE_NULL)
        {
            msg.content.data[1] = (uint8) PARAM_FROM_EBOOK;
        }
        else
        {
            msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYING;
        }
        break;
        case RESULT_LASTPLAY:
        msg.content.data[0] = app_id[engine_type];
        if (engine_type == ENGINE_NULL)
        {
            msg.content.data[1] = (uint8) PARAM_FROM_EBOOK;
        }
        else
        {
            msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYED;
        }
        break;
        case RESULT_USB_TRANS:
        msg.content.data[0] = APP_ID_UDISK;
        msg.content.data[1] = PARAM_NULL;
        break;

        default:
        need_send_msg = FALSE;
        break;
    }
    //��process manager���ʹ��������첽��Ϣ
    if (need_send_msg == TRUE)
    {
        send_async_msg(APP_ID_MANAGER, &msg);
    }
}

/********************************************************************************
 * Description :����̨���ŵ��̷�
 *
 * Arguments  :   ��
 *
 *
 * Returns     :
 *                ��
 * Notes       :
 *
 ********************************************************************************/
void _check_play_device(void)
{
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;
    file_path_info_t music_file_path;
    // engine_type_e engine_type = get_engine_type();
    //��ȡ��ǰ�����ļ���·����Ϣ
    reply.content = &music_file_path;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
    //����ͬ����Ϣ
    send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    if ((engine_type == ENGINE_MUSIC))
    {
        music_play_device = music_file_path.file_path.dirlocation.disk;
    }
}

