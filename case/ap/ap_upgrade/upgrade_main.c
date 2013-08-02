/*                              US212A
 *                            Module: UPDATE_MAIN
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-10-18 9:37:12           1.0              build this file
 *******************************************************************************/
/*!
 * \file     upgrade_main.c
 * \brief    �Զ�����ap��ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2012-10-18
 *******************************************************************************/
#include "ap_upgrade.h"
#include "audio_device.h"

#define APP_TIMER_COUNT 1

//ϵͳ����
setting_comval_t g_setting_comval;

//�ļ�ϵͳ�߱�
int32 g_file_sys_id;

//��дpage������
uint8 *g_data_buffer = (uint8 *) DECRYPT_RW_BUF_ADDR;

//FWƫ��
uint32 g_fw_offset;

//FW����
uint32 g_fw_length;

//FLASH_IDƫ��
uint32 g_flashid_offset;

//FLASH_ID����
uint32 g_flashid_length;

//BREC����
uint32 g_brec_length;

//ǰ̨Ƶ�ʱ���
uint32 g_save_freq;

//flash id
uint8 g_flash_id[64];

//��������ƫ��
uint32 g_firmware_offset;

//�ٷֱȼ���
uint8 g_precent_count;

//ˢ�¶�ʱ��ID
int8 g_flush_timer_id;

//cpu�ͷż�ʱ��
uint8 cpu_release_count;

//д����������
uint32 g_write_sector;

//�������ݽṹ��
PCFWDecrypt_struct g_decrypt;

app_timer_t conf_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

/*main��Ҫ���������ֵ����ϵͳ��Ա����*/
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS;
#endif
INT8U prio = AP_FRONT_LOW_PRIO;

static const uint8 nand_drv_name[] = "nand.drv";
static const uint8 card_drv_name[] = "card.drv";
static const uint8 uhost_drv_name[] = "uhost.drv";

/******************************************************************************/
/*!
 * \par  Description:
 *  ��ȡӦ��VMȫ�ֱ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
static void _upgrade_read_var(void)
{
    //��ȡcommon VM����������Ϣ
    sys_vm_read(&g_setting_comval, VM_AP_SETTING, sizeof(g_setting_comval));

    //��ʼ��ϵͳ��VM����
    if (g_setting_comval.g_comval.magic != MAGIC_SETTING)
    {
        g_setting_comval.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&g_setting_comval);
        sys_vm_write(&g_setting_comval, VM_AP_SETTING);
    }

    com_set_sys_comval(&g_setting_comval.g_comval);
}
/******************************************************************************/
/*!
 * \par  Description:
 *  װ��������ʺ��ļ�ϵͳ
 * \param[in]    disk_type ��������
 * \param[out]   none
 * \return       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
int8 _upgrade_fs_init(uint8 disk_type)
{
    int32 ret_val;
    uint8 drv_type;
    uint8 *drv_name;

    if (disk_type == DISK_C)
    {
        drv_type = DRV_GROUP_STG_BASE;
        drv_name = (uint8 *) nand_drv_name;
    }
    else if (disk_type == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
        drv_name = (uint8 *) card_drv_name;
    }
    else if (disk_type == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
        drv_name = (uint8 *) uhost_drv_name;
    }
    else
    {
        return -1;
    }

    //װ���������
    if (sys_drv_install(drv_type, 0, drv_name) != 0)
    {
        return -1;
    }

    //װ���ļ�ϵͳ
    g_file_sys_id = sys_mount_fs(drv_type, disk_type, 0);

    if (g_file_sys_id == -1)
    {
        sys_drv_uninstall(drv_type);
        return -1;
    }

#ifndef PC
    //�л�nand cache��ַ
    base_op_entry((void*) 1, (void*) NAND_CACHE_ADDR, 0, BASE_UPDATE);
#endif

    ret_val = sys_drv_uninstall(DRV_GROUP_AUDIO_DEVICE);

    if (ret_val == -1)
    {
        return ret_val;
    }

    ret_val = sys_drv_install(DRV_GROUP_AUDIO_DEVICE, 0, "FWDec.al");

    //��Ƶ����
    g_save_freq = sys_adjust_clk(FREQ_108M, 0);

    return g_file_sys_id;
}
/******************************************************************************/
/*!
 * \par  Description:
 *  ж��������ʺ��ļ�ϵͳ
 * \param[in]    disk_type ��������
 * \param[out]   none
 * \return       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
int8 _upgrade_fs_deinit(uint8 disk_type)
{
    int32 ret_val;
    uint8 drv_type;

#ifndef PC
    //�л�nand cache��ַ
    base_op_entry((void*) 1, (void*) NAND_DEFAULT_ADDR, 0, BASE_UPDATE);
#endif

    if (g_file_sys_id != -1)
    {
        ret_val = sys_unmount_fs(g_file_sys_id);
        g_file_sys_id = -1;
    }
    else
    {
        return TRUE;
    }

    if (0 > ret_val)
    {
        return FALSE;
    }

    if (disk_type == DISK_C)
    {
        drv_type = DRV_GROUP_STG_BASE;
    }
    else if (disk_type == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
    }
    else if (disk_type == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
    }
    else
    {
        return -1;
    }

    ret_val = sys_drv_uninstall(drv_type);

    if (0 > ret_val)
    {
        return FALSE;
    }

    sys_drv_uninstall(DRV_GROUP_AUDIO_DEVICE);

    sys_drv_install(DRV_GROUP_AUDIO_DEVICE, 0, "aud_dev.drv");

    sys_adjust_clk(g_save_freq, 0);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    upgrade applib�˳�
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \retval       1    success
 * \retval       0    fail
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
bool _upgrade_app_deinit(void)
{
    kill_app_timer(g_flush_timer_id);

    //ж���ļ�ϵͳ
    _upgrade_fs_deinit(DISK_C);

    sys_timer_exit();

    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    ui_set_display_buf(JPEG_RAM_INDEX);

    applib_quit();

    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    upgrade��ʾ��ʼ��
 * \param[in]    void
 * \param[out]   void
 * \return       none
 * \retval       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/

void _upgrade_display_init(void)
{
    /*pic_box �ӿڱ���*/
    style_infor_t upgrade_sty;
    progressbar_private_t progressbar_param;
    textbox_private_t textbox_param;

    g_precent_count = 0;

    upgrade_sty.type = UI_AP;

    /*��ʾ�ײ�ͼƬ*/
    upgrade_sty.style_id = UPGRADE_BACKGROUND;
    ui_show_picbox(&upgrade_sty, NULL);

    // ��ʾ������
    upgrade_sty.style_id = UPGRADE_PROGRESS;
    progressbar_param.total = 120;
    progressbar_param.value = 0;
    ui_show_progressbar(&upgrade_sty, &progressbar_param, PROGRESS_DRAW_ALL);

    // ��ʾ���������ַ���
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_id = -1;
    textbox_param.str_value = NULL;
    upgrade_sty.style_id = UPGRADE_STRING;
    ui_show_textbox(&upgrade_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

    // ��ʾ��ʾ�ַ���
    upgrade_sty.style_id = UPGRADE_STRING2;
    ui_show_textbox(&upgrade_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

    // ��ʾ�ٷֱ�
    upgrade_sty.style_id = UPGRADE_PERCENT;
    textbox_param.lang_id = ANSIDATAAUTO;
    textbox_param.str_value = "%0";
    ui_show_textbox(&upgrade_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

    //��ʱ��2s
    g_flush_timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, _upgrade_proc_timer);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ֪ͨ��̨�����˳�
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
static void _close_engine(void)
{
    msg_apps_t msg;
    engine_type_e engine_type;

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

    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  applib��ʼ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
bool _upgrade_app_init(void)
{
    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_SETTING, APP_TYPE_GUI);

    //��ʼ����ʱ��
    init_app_timers(conf_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init(NULL);

    _close_engine();

    //��ʼ��ap��Դ
    ui_res_open("upgrade.sty", UI_AP);

    //��ʼ��common��Դ
    ui_res_open("common.sty", UI_COM);

    //װ���ļ�ϵͳ
    _upgrade_fs_init(DISK_C);

    //����UI��ʹ��picture buffer
    ui_set_display_buf(LCD_RAM_INDEX);

    _upgrade_display_init();

    _firmware_load_bank();

    g_decrypt.initusebuffer = (uint8 *) DECRYPT_INIT_BUF_ADDR;
    g_decrypt.initusebufferlen = DECRYPT_INIT_BUF_LEN;
    g_decrypt.pInOutBuffer = (uint8 *) DECRYPT_USE_BUF_ADDR;
    g_decrypt.InOutLen = DECRYPT_USR_BUF_LEN;
    g_decrypt.pGLBuffer = (uint8 *) DECRYPT_GL_BUF_ADDR;

    g_write_sector = 0;

    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *  �����ɹ���д���Ź��Ĵ����ȴ���λ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/

void _upgrade_reboot_system(void)
{
    uint32 wd_ctl_tmp;

    //���ÿ��Ź���176ms��λ����
    wd_ctl_tmp = act_readl(WD_CTL);
    wd_ctl_tmp &= 0xffffffd1;//clksel = 176ms
    wd_ctl_tmp |= 0x11;//wden & clear wd timer
    act_writel(wd_ctl_tmp, WD_CTL);
    while (1)
    {
        ;//nothing
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    upgradeӦ�õ���ں���
 * \param[in]    0-����������ֵ���ػ�����16bit����ػ�ʱ��ǰ̨Ӧ��ID����16bit��ʾ��̨�����״̬
 * \param[out]   none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    msg_apps_t msg;
    upgrade_status_e result;

    //Ӧ�ñ�����ʼ��
    _upgrade_read_var();

    //��ʼ��applib
    _upgrade_app_init();

    result = _upgrade_firmware();

    if (result == FW_UPGRADE_NO_ERR)
    {
        //�̼������ɹ�
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_UPGRADE_SUCCEED);

        //�����̼�
        ud_sector_write(g_data_buffer, 0, 0);

        //׼������
        _upgrade_reboot_system();

    }
    else
    {
        if (result == FW_UPGRADE_FILE_ERR)
        {
            //�̼��ļ�����
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_OPEN_ERROR);

        }
        else
        {
            //�̼�����ʧ��
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_UPGRADE_FAILED);
        }

        //����ʧ�ܣ�����main ap
        msg.type = MSG_CREAT_APP;
        msg.content.data[0] = (uint8) APP_ID_TOOLS;
        send_async_msg(APP_ID_MANAGER, &msg);
    }

    _upgrade_app_deinit();

    return result;
}

