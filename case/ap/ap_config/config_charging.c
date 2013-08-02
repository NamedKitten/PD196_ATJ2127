/*******************************************************************************
 *                              US212A
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     config_charging
 * \brief    �ػ�����Ĵ����������״̬����ʾ������ļ���Լ����������ߵĴ���
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "ap_config.h"

const key_map_t charge_key_map_list[] =
{
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_CONFIG_KEY_UP },
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_CONFIG_KEY_UP },
    { { KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_CONFIG_KEY_UP },
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_CONFIG_KEY_UP },
    { { KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_CONFIG_KEY_UP },
    { { KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_CONFIG_KEY_HOLD },
    { { KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_CONFIG_KEY_HOLD },
    { { KEY_MODE, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_CONFIG_KEY_HOLD },
    { { KEY_PLAY, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_CONFIG_KEY_HOLD },
    { { KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_CONFIG_KEY_HOLD },
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};
/******************************************************************************/
/*!
 * \par  Description:
 *  standby��Ŀǰ�����ܸ�Ƶ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
int standby(void)
{
    uint32 temp_flag;
    uint32 bak_clk;
    uint32 bak_mrcr;
    int ret_value = 0;

    bak_clk = act_readl(CLKENCTL);
    bak_mrcr = act_readl(MRCR);

    //���ǹ��ĵĻ�������Ҫ�ر�һЩģ��
    act_writel(0x00000c00, CLKENCTL); //disable all module clk, except USB

    temp_flag = sys_local_irq_save();

    while (1)
    {
        //���߶ϵ�
        if ((act_readb(SYSTEM_VOL) & 0x80) == 0)
        {
            switch_off();
        }
        //play�����˳�ѭ��
        if ((act_readl(SYSTEM_CTL_RTCVDD) & 0x80000000) != 0)
        {
            //�ȴ�̧��
            while ((act_readl(SYSTEM_CTL_RTCVDD) & 0x80000000) != 0)
            {
                ;//do nothing
            }
            ret_value = GUI_OUTOF_STANDBY;
            break;
        }
        //�����������˳�ѭ��
        if ((act_readb(LRADC1_DATA) & 0x3f) != 0x3f)
        {
            //�ȴ�̧��
            while ((act_readb(LRADC1_DATA) & 0x3f) != 0x3f)
            {
                ;//do nothing
            }
            ret_value = GUI_OUTOF_STANDBY;
            break;
        }
        //alarm����
        if ((act_readl(RTC_CTL) & 0x00000001) == 1)
        {
            //��pending
            act_writel(act_readl(RTC_CTL), RTC_CTL);
            ret_value = ALARM_OUTOF_STANDBY;
            break;
        }
    }

    act_writel(bak_clk, CLKENCTL);
    //act_writel(bak_mrcr, MRCR);
    sys_local_irq_restore(temp_flag);
    return ret_value;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  �ж��Ƿ�����磬��ʾ���״̬
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void _display_charging(void)
{
    picbox_private_t temp_pic;
    style_infor_t temp_sty;

    if ((uint32) key_chargeget(0) == CHARGE_FULL)
    {
        //�����磬������30s
        if (charge_full_flag == FALSE)
        {
            charge_frame_id = BATTERY_GRADE_MAX - 1;
            lcd_standby_screen(TRUE);
            lcd_backlight_on_off(TRUE);
            backlight_flag = BACKLIGHT_ON;
            backlight_cnt = 0;
            charge_full_flag = TRUE;
        }
    }
    else
    {
        charge_frame_id++;
        if (charge_frame_id >= BATTERY_GRADE_MAX)
        {
            charge_frame_id = 0;
        }
    }

    //������ʱ��ʾ���ͼ��
    if (backlight_flag == BACKLIGHT_ON)
    {
        //��ʾ���ͼ��
        temp_sty.style_id = STY_CFG_CHARGING;
        temp_sty.type = UI_AP;
        temp_pic.pic_id = V_U16_INVALID;
        temp_pic.frame_id = charge_frame_id;
        ui_show_picbox(&temp_sty, &temp_pic);

        //�����ʱ��>30s�ر���
        backlight_cnt++;
        if (backlight_cnt >= 30)
        {
            backlight_flag = BACKLIGHT_OFF;
            lcd_backlight_on_off(FALSE);
            lcd_standby_screen(FALSE);
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *  ���Ĵ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void _config_charging(void)
{
    input_gui_msg_t gui_msg;
    style_infor_t temp_sty;
    private_msg_t private_msg;
    int standby_result;
    uint8 temp_mode = PARAM_ENTER_NORMAL;
    msg_apps_type_e gui_event;

    ui_res_open("config.sty", UI_AP);

    //������
    temp_sty.style_id = STY_CFG_BACKGROUND;
    temp_sty.type = UI_AP;
    ui_show_picbox(&temp_sty, NULL);

    //�趨1s�Ķ�ʱ�������ڸ��½�����ж��Ƿ�����
    timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, (timer_proc) _display_charging);

    while (1)
    {
        //���ⰴ������
        if (get_gui_msg(&gui_msg) == TRUE)
        {
            if (com_key_mapping(&gui_msg, &gui_event, charge_key_map_list) == TRUE)
            {
                if (gui_event == EVENT_CONFIG_KEY_HOLD)
                {
                    //�������̧����Ϣ
                    com_filter_key_hold();
                }
                //�������أ�ֻ������
                if (backlight_flag == BACKLIGHT_OFF)
                {
                    lcd_standby_screen(TRUE);
                    lcd_backlight_on_off(TRUE);
                    backlight_flag = BACKLIGHT_ON;
                    //�����ʱ�����0
                    backlight_cnt = 0;

                }
                else
                {
                    //�˳�ѭ��
                    break;
                }
            }
        }
        if ((charge_full_flag == TRUE) && (backlight_cnt >= 30))
        {
            //���粢�ұ����Ѿ�����30s������Ҫ�ٿ���ʱ��
            if (timer_id != -1)
            {
                kill_app_timer(timer_id);
                timer_id = -1;
            }
            //����standby��һֱѭ��
            standby_result = standby();
            //alarm��Ϣ����
            if (standby_result == ALARM_OUTOF_STANDBY)
            {
                temp_mode = PARAM_ENTER_ALARM;
                break;
            }
            else
            {
                //���ڰ�����standby����������������ʱ������ʾ�����ͼ��
                lcd_standby_screen(TRUE);
                lcd_backlight_on_off(TRUE);
                backlight_flag = BACKLIGHT_ON;
                backlight_cnt = 0;
                timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, (timer_proc) _display_charging);
            }
        }
        //��usb�ߣ��ػ��ϵ�
        if ((act_readb(SYSTEM_VOL) & 0x80) == 0)
        {
            if (timer_id != -1)
            {
                kill_app_timer(timer_id);
                timer_id = -1;
            }
            switch_off();
        }
        //alarm��Ϣ����
        if (get_app_msg(&private_msg) == TRUE)
        {
            if (private_msg.msg.type == MSG_RTCALARM)
            {
                temp_mode = PARAM_ENTER_ALARM;
                break;
            }
        }
    }

    if (timer_id != -1)
    {
        kill_app_timer(timer_id);
        timer_id = -1;
    }
    ui_res_close(UI_AP);
    if (backlight_flag == BACKLIGHT_OFF)
    {
        lcd_standby_screen(TRUE);
        lcd_backlight_on_off(TRUE);
    }
    _creat_application(temp_mode);
}
