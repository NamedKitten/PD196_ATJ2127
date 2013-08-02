/*******************************************************************************
 *                              US212A
 *                            Module: PIC_COMFUNC
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-3-5 10:59:36           1.0              build this file
 *******************************************************************************/
/*!
 * \file     pic_comfunc.c
 * \brief    pictureģ�鹫�к������ᱻ��ͬ�������õĺ���
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2012-3-5
 *******************************************************************************/
#include "picture.h"

#define MAX_SUPPORT_STG_DEVICE 3

static const uint8 driver_type[MAX_SUPPORT_STG_DEVICE] =
{ DISK_C, DISK_H, DISK_U };

//picture֧�ֵ��ļ����͸���
#define PicExtNum 4

const uint8 support_ext[PicExtNum][4] =
{ "JPG", "JPEG", "BMP", "GIF" };

const image_type_t support_pic_type[] =
{ IMAGE_JPG, IMAGE_JPG, IMAGE_BMP, IMAGE_GIF, UNKOWN };
/******************************************************************************/
/*!
 * \par  Description:
 *    picture��ʼ���ļ�ѡ����
 * \param[in]    disk_flag Ҫ��ʼ�����ļ�ѡ�����̷�
 * \param[out]   none
 * \return       fsel_error_e ��enhanced��ȡ���ĳ�ʼ���������ֵ
 * \retval         FSEL_NO_ERR ��ʼ���ɹ�
 * \retval         other value ��ʼ��ʧ��
 * \ingroup      pic_comfunc
 * \note
 *******************************************************************************/
fsel_error_e pic_fsel_initial(uint8 disk_flag)
{
    uint8 i;
    fsel_param_t pic_fsel_param;
    fsel_error_e fsel_error = FSEL_NO_ERR;

    //��ʼ��pictureӦ�õ��ļ�ѡ����
    pic_fsel_param.fsel_type = g_picture_var.path.file_source;
    pic_fsel_param.fsel_mode = g_comval.slide_repeat_mode | (g_comval.slide_shuffle << 4);
    pic_fsel_param.brow_type = FSEL_BROW_ALL;
    pic_fsel_param.disk = disk_flag;
    pic_fsel_param.file_type_bit = g_picture_var.file_ext_bitmap;

    for (i = 0; i < MAX_SUPPORT_STG_DEVICE; i++)
    {
        if (disk_flag == driver_type[i])
        {
            break;
        }
    }

    if (i == MAX_SUPPORT_STG_DEVICE)
    {
        fsel_error = FSEL_ERR_INPUT;
    }
    else
    {
        picture_mount = fsel_init(&pic_fsel_param, 0);

        fsel_error = fsel_get_error();
    }
    return fsel_error;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �Ժ���"�Ƿ�"�ַ�����dialog�Ի���������з�װ
 * \param[in]    str_id ��Ҫ˵�����ַ���ID
 * \param[out]   none
 * \return       app_resule_e
 * \ingroup      pic_comfunc
 * \note
 *******************************************************************************/
static app_result_e _pic_ask_dialog(uint16 str_id)
{
    app_result_e result;
    dialog_com_data_t ask_dialog_data;
    string_desc_t ask_create_str;

    //�Ի����Ƿ񴴽������б�
    ask_create_str.data.id = str_id;
    ask_create_str.language = UNICODEID;
    //�Ի�������
    ask_dialog_data.dialog_type = DIALOG_BUTTON_INCLUDE;
    ask_dialog_data.icon_id = -1;
    //�ַ���
    ask_dialog_data.string_desc = &ask_create_str;

    //��������
    ask_dialog_data.button_type = BUTTON_YESNO;
    /*! Ĭ�ϼ������ */
    ask_dialog_data.active_default = 0x01;

    result = gui_dialog(DIALOG_ASK, &ask_dialog_data);

    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ѯ���Ƿ񴴽������б�
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e �Ի��򷵻�ֵ
 * \retval       RESULT_DIALOG_YES ȷ��
 * \retval       RESULT_REDRAW     ȡ��ѡ��
 * \ingroup      pic_comfuc
 * \note
 *******************************************************************************/
static app_result_e ask_for_creat_playlist(void)
{
    app_result_e result;

    uint8 old_disk = g_picture_var.path.file_path.dirlocation.disk;

    //Ϊ��Ӧ���γ���Ϣ������ʱ�����̷�
    g_picture_var.path.file_path.dirlocation.disk = DISK_H;

    result = _pic_ask_dialog(S_IS_CREATE_PLAYLIST);
    g_picture_var.path.file_path.dirlocation.disk = old_disk;
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    picture��ʼ���ļ�ѡ��������pic_fsel_init()��װ
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e
 * \retval       RESULT_NULL   ��ʼ���ɹ�
 * \retval       ����ֵ        ��ʼ��ʧ��
 * \ingroup      pic_comfuc
 * \note
 *******************************************************************************/
app_result_e pic_disk_initial(void)
{
    fsel_error_e fsel_error;
    app_result_e result;
    uint8 disk_flag = g_picture_var.path.file_path.dirlocation.disk;

    //��ʼ��browserӦ�õ��ļ�ѡ����
    fsel_error = pic_fsel_initial(disk_flag);

    switch(fsel_error)
    {
        case FSEL_NO_ERR:
        //��ʼ���ɹ�
        is_fsel_init = TRUE;
        result = RESULT_NULL;
        break;

        case FSEL_ERR_STG:
        case FSEL_ERR_FS:
        case FSEL_ERR_LIST_NOEXIST:
        if(fsel_error == FSEL_ERR_LIST_NOEXIST)
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_PLAYLIST);    
        }
        else
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);    
        }
        
        if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
        {
            //���ڿ���,�Ƚ��̷�����Ϊ�Ƿ�ֵ�Ա����³�ʼ���ļ�ѡ����
            g_picture_var.path.file_path.dirlocation.disk = 0xff;
            result = RESULT_PIC_MENU;
        }
        else
        {
            //����main ap
            result = RESULT_MAIN_APP;
        } 
        break;

        default:
        result = RESULT_MAIN_APP;
        break;       
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �ָ���̨����
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      pic_comfunc
 * \note
 * \li  MSG_CREAT_APP_SYNC ͬ��������̨��Ϣ ����music��̨����Ҫ�����������ͺ��̷�
 *******************************************************************************/
bool music_resume_engine(void)
{
    bool bret = FALSE;
    msg_apps_t msg;
    engine_type_e engine_type;

    engine_type = get_engine_type();

    if (engine_type == ENGINE_MUSIC)
    {
        //��Ϣ����(����Ϣ����)
        msg.type = MSG_MENGINE_RESUME_SYNC;

        //����ͬ����Ϣ
        bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    }
    return bret;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ��ͣ��̨����
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      pic_comfunc
 * \note
 *******************************************************************************/
bool music_pause_engine(void)
{
    bool bret = FALSE;
    msg_apps_t msg;
    engine_type_e engine_type;

    engine_type = get_engine_type();

    if (engine_type == ENGINE_MUSIC)
    {
        msg.type = MSG_MENGINE_PAUSE_SYNC;

        bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    }
    return bret;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ֪ͨ��̨�����˳�
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      pic_comfunc
 * \note
 *******************************************************************************/
void music_close_engine(void)
{
    msg_apps_t msg;
    engine_type_e engine_type;

    engine_type = get_engine_type();

    if (engine_type == ENGINE_MUSIC)
    {
        //��process manager����ϢҪ���̨�˳�
        msg.type = MSG_KILL_APP_SYNC;

        msg.content.data[0] = APP_ID_MENGINE;

        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ǰ̨���bmp/gifʱ������̨�Ƿ�Ϊmusic engine
 * \param[in]    engine_type ��������
 * \param[in]    image_type  ͼƬ����
 * \param[out]   none
 * \return       bool
 * \retval       TRUE  ��������ͼƬ
 * \retval       FALSE ��ֹͼƬ����
 * \ingroup      pic_comfunc
 * \note
 * \li  ���ں�̨��musicʱ��ǰֻ̨�ܽ�jpg�������Ҫ�����������ͺ�ͼƬ���ͽ����ж�
 *******************************************************************************/
bool pic_check_engine(image_type_t image_type)
{
    bool result = TRUE;
    app_result_e dialog_result = RESULT_NULL;

    //ѡ���JPEGͼƬ������Ҫ��ǰ��̨�����ж�
    if ((image_type == IMAGE_BMP) || (image_type == IMAGE_GIF))
    {
        if ((g_picture_userinfor.playflag & PLAYFLAG_PREVIEW_DECODE) == 0)
        {
            if (g_picture_auto_play == TRUE)
            {
                return FALSE;
            }

            if (g_stop_music == 1)
            {
                //ȷ�� ��ͣ��������
                music_pause_engine();
                g_need_resume_engine = 1;
            }
            else
            {
                dialog_result = _pic_ask_dialog(S_IS_STOP_MUSIC);
                if (dialog_result == RESULT_DIALOG_YES)
                {
                    //ȷ�� ��ͣ��������
                    music_pause_engine();
                    g_need_resume_engine = 1;
                    g_stop_music = TRUE;
                }
                else
                {
                    //��ȷ�ϣ�����
                    g_stop_music = 2;
                    result = FALSE;
                }
            }
        }
        else
        {
            //����ͼ�����²���ʾ������bmp/gif
            result = FALSE;
        }
    }
    else if (image_type == UNKOWN)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FORMAT_ERROR);
        return FALSE;
    }
    else
    {
        ;//nothing
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ���ݺ�׺����ȡͼƬ�ļ�����
 * \param[in]    extname  �ļ���׺��
 * \param[out]   none
 * \return       image_type_t ͼƬ����
 * \ingroup      pic_comfunc
 * \note
 *******************************************************************************/
static image_type_t _check_image_type(uint8 *extname)
{
    uint8 cnt;
    image_type_t ret;
    uint8 ext_name[4];
    //ת����׺��
    ext_name[0] = ((extname[0] >= 'a') && (extname[0] <= 'z')) ? (extname[0] - 'a' + 'A') : extname[0];
    ext_name[1] = ((extname[1] >= 'a') && (extname[1] <= 'z')) ? (extname[1] - 'a' + 'A') : extname[1];
    ext_name[2] = ((extname[2] >= 'a') && (extname[2] <= 'z')) ? (extname[2] - 'a' + 'A') : extname[2];
    //�ȽϺ�׺
    for (cnt = 0; cnt < PicExtNum; cnt++)
    {
        if (libc_strncmp(ext_name, support_ext[cnt], sizeof(support_ext[cnt])) == 0)
        {
            ret = support_pic_type[cnt];
            break;
        }
    }
    if (cnt == PicExtNum)
    {
        ret = UNKOWN;
    }
    return ret;
}
/******************************************************************************/
/*!
 * \par  Description:
 *   pictureӦ�ý���ʱ�����ݽ���ģʽ�ͺ�̨���ͳ�ʼ��picture��ر���
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e
 * \ingroup      pic_comfunc
 * \note
 *******************************************************************************/
app_result_e pic_data_init(void)
{
    uint8 other_enter_mode = 0;
    app_result_e result = RESULT_NULL;
    image_type_t image_type;
    engine_type_e engine_type = get_engine_type();
    engine_state_e engine_state = get_engine_state();

    //�ƺ�����Ҫ�������
    g_picture_userinfor.filename = NULL;
    g_picture_userinfor.data_offset = 0;
    g_picture_userinfor.pregion1 = &region_in;
    g_picture_userinfor.pregion2 = &region_out;
    g_picture_userinfor.mountindex = (uint32) picture_mount;

    if (g_picture_var.revolve_flag == 1)
    {
        //ͼƬ��Ҫ��ת����
        g_picture_userinfor.playflag |= PLAYFLAG_AUTO_ROTATION;
    }
    else
    {
        g_picture_userinfor.playflag &= PLAYFLAG_NO_ROTATION;
    }

    //Ĭ��ʹ��JPG/BMP/GIF�ļ���׺����
    g_picture_var.file_ext_bitmap = PICTURE_BITMAP;

    if ((g_picture_enter_mode != PARAM_FROM_MAINMENU) && (g_picture_enter_mode != PARAM_FROM_BROWSER))
    {
        other_enter_mode = TRUE;
        g_picture_enter_mode = g_picture_var.prev_enter_mode;
    }

    if (g_picture_enter_mode == PARAM_FROM_MAINMENU)
    {
        if ((sys_detect_disk(DRV_GROUP_STG_CARD) == -1) || (other_enter_mode == TRUE))
        {
            if ((g_picture_var.thumbnail_flag & VALUE_PREVIEW_MODULE_ON) == VALUE_PREVIEW_MODULE_ON)
            {
                g_picture_scene_next = SCENE_PREVIEW;
            }
            else
            {
                g_picture_scene_next = SCENE_FILELIST;
            }
        }
        else
        {
            g_picture_scene_next = SCENE_MENU;
        }

        //pictureֻ��title���͵�playlist
        g_picture_var.path.file_source = FSEL_TYPE_LISTPICTURE;
        g_picture_var.path.file_path.plist_location.list_type = PLIST_TITLE;
        this_headbar_icon_id = HEADBAR_ICON_PICTURE;
    }
    else
    {
        image_type = _check_image_type(g_picture_var.path.file_path.dirlocation.filename);

        g_picture_var.path.file_source = FSEL_TYPE_COMMONDIR;
        g_picture_scene_next = SCENE_PLAY;
        this_headbar_icon_id = HEADBAR_ICON_BROWSER;

        if ((engine_type == ENGINE_MUSIC) && (engine_state == ENGINE_STATE_PLAYING))
        {
            if (pic_check_engine(image_type) == FALSE)
            {
                //ȷ�ϲ�����picture����
                result = RESULT_RETURN_BROWSER;
            }
        }
    }

    g_stop_music = 0;
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  Picture ��ȡ˽����Ϣ�Ĵ���
 * \param[in]    msg Picture �Ĵ���ĸ���˽����Ϣ�Ļص�����
 * \param[out]   none
 * \return       app_result_e
 * \ingroup      pic_comfunc
 * \note
 *******************************************************************************/
app_result_e pic_msg_callback(private_msg_t *pri_msg)
{
    app_result_e result = RESULT_NULL;
    engine_type_e engine_type;
    uint8 need_restart_thread = FALSE;

    if(g_mmm_id_free == FALSE)
    {
        pic_decoder_free();
        need_restart_thread = TRUE;
    }
    
    //msg_reply_t m_reply;
    /*ͬ����Ϣ����*/
    switch (pri_msg->msg.type)
    {
        case MSG_SD_OUT:
        if (g_picture_var.path.file_path.dirlocation.disk == DISK_H)
        {
            //���ڷ��ʿ����ļ����˻ص���Ӧ��
            engine_type = get_engine_type();

            if (engine_type == ENGINE_MUSIC)
            {
                music_close_engine();
            }
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
            result = RESULT_CARD_OUT_ERROR;
        }
        break;

        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;

        default:
        result = com_message_box(pri_msg->msg.type);
        break;
    }

    if(result == RESULT_NULL)
    {
        if(need_restart_thread == TRUE)
        {
            result = RESULT_REDRAW;
        }
    }    
    return result;
}
