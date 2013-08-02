/*
 *******************************************************************************
 *                              USDK
 *                             Module: TOOLS AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_fm_tools.c
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ap_tools.h"

//�л�Ƶ�ʷ���
#define  DIR_UP  0
#define  DIR_DOWN  1
//����������СƵ��
#define  MIN_FREQ  0x55cc  //87500 = 0x155cc
#define  MAX_FREQ  0xa5e0  //108000 = 0x1a5e0
/*����Ƶ��ͼƬ����*/
#define PIC_X_F1START       29    //����λ��Ϊ2  λʱ����ʼx  ����
#define PIC_X_F1START_1    24     //����λ��Ϊ3  λʱ����ʼx  ����
#define PIC_Y_F1START       43
/* ����Ƶ������ͼƬ֮�����������ֵ*/
#define PIC_X_INC1      11
#define PIC_X_INC2      11
#define PIC_X_INC3      12
#define PIC_X_INC4      5
#define PIC_X_INC5      11

uint8  FMFreNumOld;
uint16 freq;
uint8 Freqtab[7];
bool fm_need_draw = TRUE;

const uint8  pos_x_inc[5]={PIC_X_INC1, PIC_X_INC2, PIC_X_INC3, PIC_X_INC4, PIC_X_INC5};

/* radio ���ų��������¼�ӳ��*/
const key_map_t playing_key_map_list[] = 
{
    /*! �̰� NEXT ����һƵ���¼�*/
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_NEXT_FREQ },
    /*! �̰� PREV ����һƵ���¼� */
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_PREV_FREQ },
    /*!�̰�play����/��ͣ�¼� */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP  }, EVENT_RADIO_SELECT_CUR },
    /*! �̰�KEY_RETURN ����ת��Ϊ �����¼� */
    { { KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! ���һ��ӳ���¼� */
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL }, 
};

/******************************************************************************/
/*!
 * \par  Description:
 * \�л�����һ������һ��Ƶ��
 * \param[in]    dir �л�����
 * \param[in]
 * \param[out]   none
 * \return       void
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e change_freq(uint8 dir)
{
    app_result_e ret = RESULT_IGNORE;
    uint8 space;

    space = 100;

    if(dir == DIR_UP)     
    {
        if(freq < MAX_FREQ)
        {
            freq += space;
        }
        else
        {
            //���Ƶ���л�����С
            freq = MIN_FREQ;
        }
    }
    else
    {
        if(freq > MIN_FREQ)
        {
            freq -= space;
        }
        else
        {
            //��СƵ���л������
            freq = MAX_FREQ;
        }
    }
    return ret;          
}

app_result_e playwin_key_return(void)
{
    return RESULT_REDRAW;
}
app_result_e playwin_key_confirm(void)
{
    g_alarm_vars.g_aufm.frequency = freq;
    return RESULT_REDRAW;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \    gui��Ϣ����
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       void
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e deal_play_guimsg(msg_apps_type_e cur_event) 
{
    app_result_e result = RESULT_IGNORE;
    bool set_freq = FALSE;
    //bool need_play = FALSE;
    fm_need_draw = TRUE;
    switch (cur_event) 
    {
        //�л�����һ��Ƶ��
        case EVENT_RADIO_NEXT_FREQ:
        result = change_freq(DIR_UP);
        set_freq = TRUE;
        break;
        
        //�л�����һ��Ƶ��
        case EVENT_RADIO_PREV_FREQ:
        result = change_freq(DIR_DOWN);
        set_freq = TRUE;
        break;
        //ȷ�ϰ���
        case EVENT_RADIO_SELECT_CUR:
        result = playwin_key_confirm();
        break;
        //���ذ���
        case EVENT_RADIO_RETURN:
        result = playwin_key_return();
        break;
        
        default:
        fm_need_draw = FALSE;
        break;
    }

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \    �������Ƶ��ֵת��Ϊascill
 * \param[in]    freq
 * \param[out]   buf
 * \return
 * \retval
 * \note
 */
/*******************************************************************************/
bool freq_change(uint8* buf, uint16 temp_freq) 
{
    uint32 true_freq;
    uint16 i, j;
    uint8 k, m;
    uint8 temp_buf[3];

    //��ת����ʵ��Ƶ��, khz Ϊ��λ
    true_freq = (uint32) ((1 << 16) + temp_freq); //107900
    i = true_freq / 100; //875 1079
    k = true_freq % 100; //0
    //�ٷ�λ
    k = k / 10;
    //����λ
    j = i / 10; //87 107
    //С��λ
    m = (i % 10) * 10 + k; //50 90

    if (j < 100) 
    {
        libc_itoa(j, buf, 2);
    }
    else
    {
        libc_itoa(j, buf, 3);
    }
    libc_strncat(buf, ":", 1);
    libc_itoa(m, temp_buf, 2);
    temp_buf[2] = '\0';
    libc_strncat(buf, temp_buf, libc_strlen(temp_buf));
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \    ��ʾFMƵ��
 * \param[in]    buf -- ��ת��Ϊascill ���Ƶ���ִ�
 * \param[out]   none
 * \return
 * \retval
 * \note
 */
/*******************************************************************************/
void ui_show_FMdata(char *buf)
{
    uint8 FMFreNum = (uint8) libc_strlen(buf); //������������
    style_infor_t style_infor;
    uint16 x, y;
    uint8 *p_xinc;

    style_infor.type = UI_AP;
    if (FMFreNum != FMFreNumOld)
    {
        //λ�������仯�����屳��
        FMFreNumOld = FMFreNum;
        style_infor.style_id = PICBOX_FREQ_BG;
        ui_show_picbox(&style_infor, NULL);
    }
    if (FMFreNum < 6)
    { //<100MHz
        x = PIC_X_F1START;
        p_xinc = (uint8 *) &pos_x_inc[1];
    }
    else 
    { //>=100MHz
        x = PIC_X_F1START_1;
        p_xinc = (uint8 *) &pos_x_inc[0];
    }
    y = PIC_Y_F1START;
    while (1) 
    {
        if (*buf != 0) 
        {
            ui_show_pic(P_FMNUM0 + *buf - 0x30, x, y);
        }
        else
        {
            break;
        }
        buf++;
        x += (*p_xinc);
        p_xinc++;
    }
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \    fmƵ��ͼƬˢ��
 * \param[in]     none
 * \param[out]   none
 * \return       
 * \retval
 * \note 
 */
/*******************************************************************************/
void show_freq_in_play(void)
{
    //��ǰƵ��ֵת��Ϊascill ����
    libc_memset(Freqtab, 0, sizeof(Freqtab));
    freq_change(Freqtab, freq);
    ui_show_FMdata(Freqtab);    
    //ui_show_freqbar(g_radio_config.FMStatus.freq);      
    //return;      
}
/******************************************************************************/
/*!
 * \par  Description:
 * \    ����fmƵ��
 * \param[in]    mode
 * \param[out]   none
 * \return       void
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e set_fmfreq_main(void)
{
    bool fm_need_draw_bg = TRUE;
    app_result_e result = RESULT_IGNORE;
    style_infor_t style_infor;
    //gui��Ϣ
    input_gui_msg_t gui_msg;
    //gui �¼�
    msg_apps_type_e gui_event;
    //ϵͳ��Ϣ
    private_msg_t private_msg;
    
    freq = g_alarm_vars.g_aufm.frequency;
    //paint_flag |= PAINT_ALL;

    while (1) 
    {
        //ˢ��ȫ��
        if (fm_need_draw_bg == TRUE)
        {
            //����
            style_infor.type = UI_AP;
            style_infor.style_id = TOOLS_BG;
            ui_show_picbox(&style_infor, NULL);
            //FM ͼ��
            style_infor.style_id = PICBOX_FM_ICON;
            ui_show_picbox(&style_infor, NULL);
            fm_need_draw_bg = FALSE;
        }
        
        //ˢ��Ƶ�������
        if (fm_need_draw == TRUE)
        {
            fm_need_draw = FALSE;
            show_freq_in_play();
        }

        //��ȡ�ʹ���GUI��Ϣ��˽����Ϣ
        if ((get_gui_msg(&gui_msg) == TRUE))
        {
            if (com_key_mapping(&gui_msg, &gui_event, playing_key_map_list) == TRUE)
            {
                result = deal_play_guimsg(gui_event); //RESULT_REDRAW��RESULT_IGNORE
                if (result == RESULT_REDRAW)
                {
                    break;
                }
            }
        }
        
        //����ϵͳ��Ϣ
        if (get_app_msg(&private_msg) == TRUE)
        {
            result = com_message_box(private_msg.msg.type);
            if(result == RESULT_REDRAW)
            {
                fm_need_draw_bg = TRUE;
                fm_need_draw = TRUE;
            }
            else if(result != RESULT_NULL)
            {
                break;
            }
            else
            {
                //QAC
            }
        }
        //����20ms �����񽻻�
        sys_os_time_dly(2);
    }

    return result;
}

