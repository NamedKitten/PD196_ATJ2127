/*******************************************************************************
 *                              US212A
 *                            Module: udiskap
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       yliang     2011-11-07      1.0             build this file
 *******************************************************************************/
/*!
 * \file     udisk_enter2exit.c
 * \brief    udisk init & exit deal
 * \author   yliang
 * \version  1.0
 * \date  2011/10/07
 *******************************************************************************/
#include  "ap_udisk.h"

/******************************************************************************/
/*!
 * \par  Description:
 *udisk ap init
 * \param[in]
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      main_udisk.c
 * \note
 *******************************************************************************/
void udisk_init_all(void)
{
    picbox_private_t picbox_data;
    textbox_private_t textbox_param;
    style_infor_t style_infor;
    setting_comval_t u_g_setting_vars;
    //bool  result=0;
    uint8 i = 0;
    ram_clk_to_mcu();
    /*��ʾ���ӽ���*/
    style_infor.type = UI_AP;
    style_infor.style_id = UDISK_LINK_BG;
    ui_show_picbox(&style_infor, NULL);

  /*  style_infor.style_id = UDISK_MP3;
    ui_show_picbox(&style_infor, NULL);

    style_infor.style_id = UDISK_LINK_TYPE;
    picbox_data.pic_id = -1;
    picbox_data.frame_id = 0;
    ui_show_picbox(&style_infor, &picbox_data);
*/
   /* for (i = 0; i < 5; i++)
    {
        style_infor.style_id = style_usb_point[i];
        picbox_data.pic_id = -1;
        picbox_data.frame_id = 0;
        //��ʾ����
        ui_show_picbox(&style_infor, &picbox_data);
    }
    if (timer_usb_connect_id == -1)
    {
        timer_usb_connect_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, timer_usb_display_connect);
    }
    style_infor.style_id = UDISK_STATE_STR;
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_id = S_UDISK_LINKING;
    textbox_param.str_value = NULL;
    ui_show_textbox(&style_infor, &textbox_param, TEXTBOX_DRAW_NORMAL);
*/
    ui_show_battery();
    //������Ҫ��̬ˢ�µ�ͼƬ���ڴ�
    //read_connect_pic_to_ram();
    read_battery_pic_to_ram();
    //�л���LCD_BUFFER
    ui_set_display_buf(LCD_RAM_INDEX);

    //ֱ�Ӱ�װbase����,���ø�VFS�򽻵�
    if (-1 == sys_drv_install(DRV_GROUP_STG_BASE, 0, "nand.drv"))
    {
        while (1)
        {
            ;//��ѭ��
        }
    }
    //Udisk buffer��flash����cache��ͻ,����flash����дcache��ַ,�����Ȱ�װflash�������ܵ���
    base_op_entry((void*) 1, (void*) 0x9FC3B000, 0, BASE_UPDATE);

    //==================================================================================
    //**********************���º������ô����ܸĶ�***************************************
    //==================================================================================
    //UDiskSetDiskNum(g_setting_comval.g_comval.online_device);       //��������ģʽ
    UDiskSetDiskNum(0);
    //AlwaysShowCardDisk(g_setting_comval.g_comval.support_card);      //�����Ƿ���ʾ���̷�  //reagan modify 2009-1-6
//    AlwaysShowCardDisk(1);
    //U�̳�ʼ��,�������:callback����,�Ƿ�֧��Udisk���к�(�����,ÿ��Udisk����һ��),�Ƿ���Udisk��֤ģʽ
    sys_vm_read(&u_g_setting_vars, VM_AP_SETTING, sizeof(setting_comval_t));
    AutoRunSet = u_g_setting_vars.g_comval.autorun_set;
    usbinit(NULL, 1);
    usb_setup(); //����USB����
    //return 1;
}

app_result_e udisk_exit_all(int argc)
{
    app_result_e ret = 0;
    msg_apps_t msg;
    void (*p_adfu_launcher)(void) = 0xbfc004b9;
    udisk_exit();
    /*��ȡVM��Ϣ*/
    sys_vm_read(&update_flag, UDISK_VM, 4);
    //modify--not creat playlist
    update_flag &= 0xfffffffe; //���flash��д��־
    update_flag &= 0xfffeffff; //�������д��־
    msg.type = MSG_CREAT_APP; //���������б�app
    //�����˳�
    if (usbstatus == 0x00)
    {
        if (returnkey_press == 1)
        {
            goto exit_0;
        }

    }
    else if (usbstatus == 0x70)
    {
        goto exit_0;
    }
    else
    {
        //����,����
        goto exit_0;
    }

    exit_0:
    //����playlist
    if (((update_flag & 0x00000001) != 0) || ((update_flag & 0x00010000) != 0))
    {
        ret = RESULT_CREATE_LIST_APP;
        msg.content.data[0] = (uint8)APP_ID_PLAYLIST;//playlist ap
        msg.content.data[1] = (uint8)PARAM_FROM_UDISK;
        if ((update_flag & 0x00000001) != 0)
        {
            msg.content.data[1] |= (uint8)PLIST_DISK_C;//playlist ap param
            update_flag &= 0xfffffffe; //���flash��д��־
        }
        if ((update_flag & 0x00010000) != 0)
        {
            msg.content.data[1] |= (uint8)PLIST_DISK_H;//playlist ap param
            update_flag &= 0xfffeffff; //�������д��־
        }
        sys_vm_write(&update_flag, UDISK_VM);
    }
    //������һ�ε�AP
    else
    {
        ret = RESULT_LASTPLAY;
        if ((argc & 0x00ff) == APP_ID_RADIO)
        {
            msg.content.data[0] = APP_ID_RADIO;
            //ֱ�ӻ�radio����.  ����ΪPARAM_BACKTO_PLAYINGʱ��������Ƶ��
            if ((((uint32) argc >> 16) & 0x00ff) == ENGINE_STATE_PLAYING)
            {
                msg.content.data[1] = PARAM_FROM_CONFIG; //PARAM_BACKTO_PLAYING;
            }
            else
            {
                msg.content.data[1] = PARAM_FROM_CONFIG; //PARAM_BACKTO_PLAYED;
            }
        }
        else if ((argc & 0x00ff) == APP_ID_MUSIC)
        {
            msg.content.data[0] = APP_ID_MUSIC;
            if ((((uint32) argc >> 16) & 0x00ff) == ENGINE_STATE_PLAYING)
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYING;
            }
            else
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYED;
            }
        }
        else if ((argc & 0x00ff) == APP_ID_VIDEO)
        {
            msg.content.data[0] = APP_ID_VIDEO;
            if ((((uint32) argc >> 16) & 0x00ff) == ENGINE_STATE_PLAYING)
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYING;
            }
            else
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYED;
            }
        }       
        else
        {
            msg.content.data[0] = APP_ID_MAINMENU;
            msg.content.data[1] = PARAM_FROM_UDISK;
        }
    }
    send_async_msg(APP_ID_MANAGER, &msg); //�����첽��Ϣ,���������б�app�Ľ���
    //exit:
    //�ָ�flash����дcache��ַ,������flash����ж��ǰ���ܵ���
    base_op_entry((void*) 1, (void*) 0x9FC32000, 0, BASE_UPDATE);
    sys_drv_uninstall(DRV_GROUP_STG_BASE);

    if (0x51 == usbstatus)
    {
        sys_adjust_clk(FREQ_24M, 0);
        /* rise vcc 3.3v and vdd 1.3v, improve usb signal.   xiaomaky.   2012-9-11 11:55.  */
        act_writel(((act_readl(VOUT_CTL)&0xFFFFFF80)|0x6A), VOUT_CTL);
        sys_udelay(200);
        ui_set_pen_color(Color_WHITE_def);
        ui_set_backgd_color(Color_BLACK_def);
        ui_clear_screen(NULL);
        act_writel(0, INTC_CFG0);
        act_writel(0, INTC_CFG1);
        act_writel(0, INTC_CFG2); //assgin usb intrrupt to IP2
        act_writel(0x58, WD_CTL);
        p_adfu_launcher();
    }
    return ret;
}

