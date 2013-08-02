/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona        2011-09-14          1.0              create this file
 *******************************************************************************/

#include "ap_alarm.h"
#include "fmengine.h"
//sd �����ļ���
const char g_sd_filename[12] = "alarm1.mp3";

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_modual_init(void)
 * \��ʼ��FM  ģ��
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed--- ͬ����Ϣ����ʧ�ܣ���ģ���ʼ��ʧ��
 * \note
 */
/*******************************************************************************/
bool radio_modual_init(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = Band_China_US;

    //��ʼ��FM ģ��
    msg.type = MSG_FMENGINE_INIT;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  ��������ֵ
 * \param[in]    vol
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess   ���������ɹ�
 * \retval           0 failed
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
bool alarm_set_volume(uint8 vol)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_MENGINE_SET_VOLUME_SYNC;
    msg.content.addr = &vol;

    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  װ������
 * \param[in]    mengine_enter_mode_e
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
bool music_open_engine(mengine_enter_mode_e start_mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t temp_reply;
    //����Ϣ����
    msg.content.data[0] = APP_ID_MENGINE;
    msg.content.data[1] = (uint8)(start_mode | g_alarm.path.file_path.dirlocation.disk);
    //��Ϣ����(ͬ������Ӧ�ó�����Ϣ)
    msg.type = MSG_CREAT_APP_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MANAGER, &msg, &temp_reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  ���ò����ļ���·����Ϣ
 * \param[in]    filepath����ļ�·��buffer��ָ��  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
bool music_set_filepath(file_path_info_t *filepath)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //���ò����ļ���·����Ϣ
    msg.content.addr = filepath;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_SET_FILEPATH_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  ����ѭ��ģʽ&shuffleģʽ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
bool music_set_playmode(uint8 playmode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //����ѭ��ģʽ&shuffleģʽ
    msg.content.addr = &playmode;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_SET_PLAYMODE_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  ���Ͳ���������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
bool alarm_play(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //��Ϣ����(����Ϣ����)��ʼ��������
    msg.type = MSG_MENGINE_PLAY_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \  ���ų�����ʼ��
 * \param[in]    uint8 from_flag
 * \param[out]   none
 * \return       int the result
 * \retval       ......
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
bool _scene_play_init(uint8 from_flag)
{
    engine_type_e engine_type;
    msg_apps_t msg;
    uint8 cur_mode;
    int disk_exist = 0;
    //�����ļ�ϵͳ��mount ID
    int32 browser_mount;
    fsel_param_t fsel_param;
    bool result;
    //file_location_t* locatptr;//ָ��location��ָ��
    //id3_type_e cur_type;//��ǰ���ֵ�����
    //fsel_type_e comp_type = g_alarm.path.file_source;

    engine_type = get_engine_type();
    //ע��radio��̨
    if(engine_type != ENGINE_NULL)
    {
        if (engine_type == ENGINE_RADIO)
        {
            //��FM ¼�����أ�����¼��codec ������fm �������룬��װ����
            sys_drv_uninstall(DRV_GROUP_FM);
            sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
            //װ�������������³�ʼ��ģ��
            radio_modual_init();
            
            msg.type = MSG_KILL_APP_SYNC;//MSG_KILL_APP;
            msg.content.data[0] = APP_ID_FMENGINE;
            send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);    //����ͬ����Ϣ
        }
        else
        {
            music_close_engine();
        }
    }
    if(from_flag == 1) // DISK_C
    {
        disk_exist = sys_detect_disk(DRV_GROUP_STG_BASE);
    }
    else if(from_flag == 2) // DISK_H
    {
        disk_exist = sys_detect_disk(DRV_GROUP_STG_CARD);
    }
    else //SD file
    {
        g_alarm.path.file_path.dirlocation.disk = DISK_C;
    }

    if (disk_exist == -1)
    {
        //��ⲻ�����̣����ʼ����Ĭ���̷�
        from_flag = 0;
        g_alarm.path.file_path.dirlocation.disk = DISK_C;
    }
    
    //if (engine_type != ENGINE_MUSIC)
    {
        music_open_engine(ENTER_ALARM);
    }
        
    if(from_flag != 0)   //ѡ���������
    {
        //��ʼ���ļ�ѡ��������
        fsel_param.disk = g_alarm.path.file_path.dirlocation.disk;
        fsel_param.fsel_type = FSEL_TYPE_COMMONDIR;
        fsel_param.fsel_mode = FSEL_MODE_BROWSER;
        fsel_param.brow_type = FSEL_BROW_ALL;
        fsel_param.file_type_bit = MUSIC_BITMAP;
        //��ʼ���ļ�ѡ����
        browser_mount = fsel_init(&fsel_param, 0);
        // mount�����˳�
        if(browser_mount == -1)
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
            return RESULT_ALARM_QUIT;
        }

        g_alarm.path.file_source = FSEL_TYPE_COMMONDIR;
        result = fsel_set_location(&g_alarm.path.file_path, FSEL_TYPE_COMMONDIR);
        //�ļ�ѡ�����˳�
        fsel_exit();
        
        //if(music_set_filepath(&g_alarm.path) != TRUE)  //������
        if(result != TRUE)  //������
        {
            goto sd_play;
        }
        music_set_filepath(&g_alarm.path);
    }
    else  //SD_PLAY
    {
        sd_play:
        g_alarm.path.file_source = FSEL_TYPE_SDFILE;
        libc_memcpy(g_alarm.path.file_path.dirlocation.filename,g_sd_filename,12);
        if(music_set_filepath(&g_alarm.path) == FALSE)  //�����ļ�·��
        //if(fsel_set_location(&g_alarm.path,FSEL_TYPE_SDFILE) == FALSE)  //�����ļ�·��
        {
            return FALSE;
        }
    }
    set_current_volume(g_alarm.volume);
    //����Ĭ������
    alarm_set_volume(g_alarm.volume); 
    //����ѭ��ģʽ
    //cur_mode &= (uint8) 0xf0;
    cur_mode = FSEL_MODE_LOOPONE;
    music_set_playmode(cur_mode);
    
    alarm_play();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  ���ų�������
 * \param[in]    void param
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
app_result_e alarm_scene_playing(void)
{
    bool result;
    app_result_e ret_vals;
    //style_infor_t style_infor;

    result = _scene_play_init(g_alarm.ring_flag);
    if (result == FALSE)
    {
        return RESULT_SCENE_EXIT;
    }

    //����ѭ��
    ret_vals = playing_loop_deal();
    
    return ret_vals;
}
