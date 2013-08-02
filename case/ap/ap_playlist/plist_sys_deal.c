/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    ������д�ļ��ĸ���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

static const char stg_drv_name[][10] =
{ "nand.drv", "card.drv", "uhost.drv" };

/******************************************************************************/
/*!
 * \par  Description:
 *    ֪ͨ��̨�����˳�
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      video
 * \note
 *******************************************************************************/
static void close_engine(void)
{
    msg_apps_t msg;
    engine_type_e engine_type;
    engine_state_e engine_status;

    engine_status = get_engine_state();

    if (engine_status != ENGINE_STATE_NULL)
    {
        engine_type = get_engine_type();

        //��process manager����ϢҪ���̨�˳�
        msg.type = MSG_KILL_APP_SYNC;
        if (engine_type == ENGINE_RADIO)
        {
            msg.content.data[0] = APP_ID_FMENGINE;
        }
        else if (engine_type == ENGINE_MUSIC)
        {
            msg.content.data[0] = APP_ID_MENGINE;
        }
        else
        {
            ;//bug
        }
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    return;
}

//�˳�ap����
void _deal_playlist_result(app_param_e enter_mode)
{
    msg_apps_t msg;
    //    bool need_send_msg = TRUE;

    //��process manager���ʹ�������������Ϣ
    msg.type = MSG_CREAT_APP;
    msg.content.data[1] = (uint8) PARAM_FROM_PLAYLIST;

    //�˳�ǰ����music��audible�ϵ���Ϣ
    libc_memset(temp_plist_buf, 0, SECTOR_SIZE);
    sys_vm_write(temp_plist_buf, VM_AP_MUSIC_AUDBK);

    switch (enter_mode)
    {
        case PARAM_FROM_MUSIC:
        msg.content.data[0] = APP_ID_MUSIC;
        break;

        case PARAM_FROM_PICTURE:
        msg.content.data[0] = APP_ID_PICTURE;
        break;

        case PARAM_FROM_VIDEO:
        msg.content.data[0] = APP_ID_VIDEO;
        break;

        case PARAM_FROM_EBOOK:
        msg.content.data[0] = APP_ID_EBOOK;
        break;

        case PARAM_FROM_CONFIG:
        msg.content.data[0] = APP_ID_MAINMENU;
        break;

        case PARAM_FROM_UDISK:
        //��Ҫͬ��MTP����
        //        msg.content.data[0] = APP_ID_MTPSYNC;//APP_ID_MAINMENU
        msg.content.data[0] = APP_ID_MAINMENU;
        break;

        default:
        msg.content.data[0] = APP_ID_MAINMENU;
        break;
    }

    //    if (TRUE == need_send_msg)
    //    {
    //        //��process manager���ʹ���������Ϣ ͬ����Ϣ�����첽��Ϣ
    //        send_async_msg(APP_ID_MANAGER, &msg);
    //    }

    send_async_msg(APP_ID_MANAGER, &msg);

    return;
}

//system enter init
void plist_sys_init(void)
{

    //�ȹرպ�̨
    close_engine();

    //��װnand�洢����
    if (sys_drv_install(DRV_GROUP_STG_BASE, MODE_NORMAL, stg_drv_name[0]) != -1)
    {
        /****
         ��ΪҪ�õ�nandĬ�ϵ�cacheҳ���ַ0x9fc34000������
         ������Ҫ��nand��cache�е����ҳ��
         ****/
#ifndef PC
        base_op_entry((void*) 1, (void*) NAND_CACHE_ADDR, 0, BASE_UPDATE);
#endif
        sys_drv_uninstall(DRV_GROUP_STG_BASE);
    }

}

//system enter exit
void plist_sys_exit(app_param_e enter_mode)
{

    _deal_playlist_result(enter_mode);

    // AP �˳�ǰ
    //��װnand�������ָ�nand ��cacheĬ�ϵ�ַ0x9fc34000

    //��װnand�洢����
    if (sys_drv_install(DRV_GROUP_STG_BASE, MODE_NORMAL, stg_drv_name[0]) != -1)
    {
#ifndef PC
        //�ָ�nand��cacheҳ��
        base_op_entry((void*) 1, (void*) NAND_DEFAULT_ADDR, 0, BASE_UPDATE);
#endif
        sys_drv_uninstall(DRV_GROUP_STG_BASE);
    }

}

