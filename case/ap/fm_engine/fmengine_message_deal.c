/*******************************************************************************
 *                              US211A
 *                            Module: radio engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     mikeyang     2011-11-03 15:00     1.0             build this file
 *******************************************************************************/
#include "app_fmengine.h"

extern uint32 volume_deal(uint8 vol_in) __FAR__;

#ifdef test

//���Դ��룬�ݲ�����Analog �������ڴ˴�����Ӳ���Ĵ�����ʹFM  ������
void init_fm_pa_out(void)
{
    int i;

    /***********************init pa and fm in************************/

    //set dac clock
    *((REG32) (CLKENCTL)) |= (1 << CLKENCTL_DACCLKEN);

    //enable DAC Module, so dac reg(include pa reg can be set ?)
    *((REG32) (MRCR)) |= (1 << MRCR_DACIISReset);

    //set adc clock
    *((REG32) (CLKENCTL)) |= (1 << CLKENCTL_ADCCLKEN);

    //enable ADC Module, so adc reg
    *((REG32) (MRCR)) |= (1 << MRCR_ADCReset);

    //set analog source input: 1 for fm
    *((REG32)(ADC_CTL)) = ((*((REG32)(ADC_CTL)) & (~(ADC_CTL_ANAINS_MASK))) | (1 << ADC_CTL_ANAINS_SHIFT));

    //enable L & R channel
    *((REG32)(ADC_CTL)) |= (1 << ADC_CTL_AINLEN);
    *((REG32)(ADC_CTL)) |= (1 << ADC_CTL_AINREN);

    /*2 for 0db as default: gain level 0-7*/
    *((REG32)(ADC_CTL)) = (*((REG32)(ADC_CTL)) & (~(ADC_CTL_AING_MASK))) | (2 << ADC_CTL_AING_SHIFT);

    /*delay*/
    for (i = 0; i < 0x2000; i++)
    {
        //i += 1;
    }

    //MIXEN, analog mixer and PA enable
    *((REG32) (DAC_ANALOG1)) |= 1 << DAC_ANALOG1_PMP_PAEN;
    /*pavcc connect vcc*/
    *((REG32) (DAC_ANALOG1)) |= 1 << DAC_ANALOG1_PVCTV;
    //PMP mode AA MUTE disable
    *((REG32)(DAC_ANALOG1)) |= 1 << DAC_ANALOG1_PMP_AAMUTE;

    /*32 for PA volume level : 0 - 41*/
    *((REG32) (DAC_ANALOG1)) |= (32 << DAC_ANALOG1_Volume_SHIFT) & DAC_ANALOG1_Volume_MASK;

    /*pa output stage enable, can output now*/
    *((REG32) (DAC_ANALOG2)) |= 1 << DAC_ANALOG2_OSEN;

    /*delay*/
    for (i = 0; i < 0x4000; i++)
    {
        //i += 1;
    }

}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_init(private_msg_t* cur_msg)
 * \FM ģ���ʼ����Ϣ���������������ִ�г�ʼ������
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note   FM ģ���ʼ�����ã�������PA , FM �����
 */
/*******************************************************************************/
app_result_e fm_module_init(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 band_info, level;
    int result;
    app_result_e ret;
    uint32 fm_ain_type = FM_AIN_TYPE;
    uint32 fm_ain_gain = FM_AIN_GAIN;

    band_info = (data_ptr->msg.content.data[0]); //��ʼ������Ƶ����Ϣ
    level = g_fmengine_cfg.fm_threshold; //��ʼ��������̨����

#ifndef PC
    result = fm_open(band_info, level, 0);
#else
    result = 1;
#endif

    //��ʼ���ɹ�
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        ret = RESULT_IGNORE;
        /* Ӳ������*/
#ifndef test
        //if (g_fmengine_cfg.paddv_mode == PA_DDV_MODE)
        //{
        //    enable_pa_ddv(0, 0); //������ʹ��PA
        //}
        //else
        //{
        //    enable_pa(0, 0); //������ʹ��PA
        //}
        com_set_sound_out(TRUE, SOUND_OUT_START, NULL);
        enable_ain(fm_ain_type, fm_ain_gain);
#endif

        //���Դ��룬Ӳ������
#ifdef test
        init_fm_pa_out();
#endif

        //ģ���ʼ���ɹ�������work ״̬
        g_module_status = MODULE_WORK;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
        ret = RESULT_NULL; //ģ���ʼ��ʧ�ܣ��˳�����AP
        g_module_status = MODULE_NO_INIT;
    }

    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_reinstall(private_msg_t* cur_msg)
 * \ Restore FM Driver global data��so no need to initial fm driver
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note
 *    ���øýӿڻָ�FM������ʹ�õ�ȫ������WriteBuffer���ݣ���˿ɱ������³�ʼ��FMģ��
 */
/*******************************************************************************/
app_result_e fm_module_reinstall(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 band_info, level;
    int result;
    app_result_e ret;
    uint16 freq;

    band_info = (data_ptr->msg.content.data[0]); //��ʼ������Ƶ����Ϣ
    freq = data_ptr->msg.content.data[2];
    freq <<= 8;
    freq |= (data_ptr->msg.content.data[1]);
    level = g_fmengine_cfg.fm_threshold; //��ʼ��������̨����

#ifndef PC
    result = fm_open(band_info, level, freq);
#else
    result = 1;
#endif

    //��ʼ���ɹ�
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        ret = RESULT_IGNORE;

        //ģ���ʼ���ɹ�������work ״̬
        g_module_status = MODULE_WORK;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
        ret = RESULT_NULL; //ģ���ʼ��ʧ�ܣ��˳�����AP
        g_module_status = MODULE_NO_INIT;
    }

    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_standby(void)
 * \FM ģ���Standby  ��Ϣ���������������ִ��
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note
 */
/*******************************************************************************/
app_result_e fm_module_standby(void)
{
    int result;
    uint32 fm_ain_type = FM_AIN_TYPE;

    /* ��Щģ����ܴ���©�磬��δ��ʼ��ʱ����Ҫ�������standby */
    if (g_module_status != MODULE_STANDBY)
    {
#ifndef PC
        result = fm_close();
#else
        result = 1;
#endif
        //����Standby �ɹ�
        if (result != 0)
        {
#ifndef test
            if (g_module_status == MODULE_WORK)
            {
                /*Ӳ������*/
                set_pa_volume(0);
                disable_ain(fm_ain_type);
                //if (g_fmengine_cfg.paddv_mode == PA_DDV_MODE)
                //{
                //    disable_pa_ddv(); //ֱ��
                //}
                //else
                //{
                //    disable_pa(); //��ֱ��
                //}
                com_set_sound_out(FALSE, SOUND_OUT_STOP, NULL);
            }
#endif
            //ģ�����Standby ״̬
            g_module_status = MODULE_STANDBY;
        }
    }
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_freq_sync(private_msg_t* cur_msg)
 * \ͬ�����õ���Ƶ�㣬��ʼFM ����
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note:   Ƶ��ֵ�����ã�����FM ui ������Ӧ�ö�����ͳһ��
 * \ ������Ը���ģ���ת�������������
 **/
/*******************************************************************************/
app_result_e fmengine_set_freq_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint16 freq;
    int result;

    freq = *(uint16*) &(data_ptr->msg.content.data[0]);
#ifndef PC
    result = fm_set_freq(freq);
#else
    result = 1;
#endif

    /* ����Ƶ��ɹ�*/
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_mute_sync(private_msg_t* cur_msg)
 * \���þ������߽������
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_mute_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 vol_bak;
    FM_MUTE_e mode;
    int result = 0;
    uint32 set_vol;

    mode = (FM_MUTE_e) data_ptr->msg.content.data[0];
    vol_bak = data_ptr->msg.content.data[1];

#ifndef PC
    result = fm_mute(mode);
#else
    result = 1;
#endif

#ifndef test
    /* �������*/
    if (mode == releaseMUTE)
    {
        //set_pa_volume((uint32) vol_bak);
        /* ��������ת��������� */
        set_vol = volume_deal(vol_bak);
        set_pa_volume(set_vol);
    }
    /* ����*/
    else if (mode == SetMUTE)
    {
        set_pa_volume(0);
    }
    else
    {
        ; /* һ�㲻�������� */
    }
#endif

    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        if (mode == releaseMUTE)
        {
            g_fmengine_status.FM_VolStatus = STATUS_PLAY;

            change_engine_state(ENGINE_STATE_PLAYING);
        }
        else if (mode == SetMUTE)
        {
            g_fmengine_status.FM_VolStatus = STATUS_PAUSE;

            change_engine_state(ENGINE_STATE_PAUSE);
        }
        else
        {
            //;
        }

    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_band_sync(private_msg_t* cur_msg)
 * \���õ�̨����
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_set_band_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    radio_band_e band_info;

    //�����õĵ�̨���Σ���radio_band_e
    band_info = (radio_band_e) data_ptr->msg.content.data[0];
#ifndef PC
    result = fm_set_band(band_info);
#else
    result = 1;
#endif

    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_throd(private_msg_t* cur_msg)
 * \������̨����
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_set_throd(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 level;
    int result;

    level = data_ptr->msg.content.data[0];
#ifndef PC
    result = fm_set_throd(level);
#else
    result = 1;
#endif
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        g_fmengine_cfg.fm_threshold = level;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_softseek(private_msg_t* cur_msg)
 * \���������̨
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note:  ����Ƶ�����ã����ж��Ƿ���̨����̨�ظ��ɹ�
 **/
/*******************************************************************************/
app_result_e fmengine_set_softseek(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint16 now_freq;
    int result;
    uint8 dir;

    //�����̨�ӿں�������
    now_freq = *(uint16*) &(data_ptr->msg.content.data[0]); //�����̨��ǰ�ж�Ƶ��
    dir = data_ptr->msg.content.data[2]; //��̨����

#ifndef PC
    result = fm_soft_seek(now_freq, dir);
#else
    result = 0;
#endif
    g_fmengine_status.FM_CurrentFreq = now_freq; //��������㵱ǰƵ��ֵ

    if (result != 0)
    {
        //��Ч��̨
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        //��Ч��̨
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_hardseek(private_msg_t* cur_msg)
 * \����Ӳ����̨
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       Ӳ����̨�����ɹ����ظ��ɹ�
 * \note:   ��̨����˵��
 * bit0   ~~~  ���ϻ�������̨��0:   UP;    1: DOWN
 * bit1   ~~~  �Ƿ�߽���ơ�     0:   ����;    1:  ������
 **/
/*******************************************************************************/
app_result_e fmengine_set_hardseek(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint16 now_freq;
    uint8 dir;
    int result;

    //Ӳ����̨�ӿں�������
    now_freq = *(uint16*) &(data_ptr->msg.content.data[0]); //Ӳ����̨��ʼƵ��
    dir = data_ptr->msg.content.data[2]; //��̨����

    //����Ӳ����̨
#ifndef PC
    result = fm_hard_seek(now_freq, dir);
#else
    result = 1;
#endif

    if (result != 0)
    {
        //�����ɹ�
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        g_fmengine_status.STC_flag = HARDSEEK_DOING;
    }
    else
    {
        //����ʧ��
        data_ptr->reply->type = MSG_REPLY_FAILED;
        g_fmengine_status.STC_flag = HARDSEEK_NOT_START;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_seek_break(private_msg_t* cur_msg)
 * \����Ӳ����̨�����У��ֶ�ֹͣ�Զ���̨����
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       �ɹ�ֹͣseek  ���̣��ظ��ɹ�������ظ�ʧ��
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_seek_break(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;

#ifndef PC
    result = fm_set_breakseek();
#else
    result = 1;
#endif

    //�ɹ�ֹͣӲ����̨
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
#ifndef PC
        fm_get_freq(&g_fmengine_status.FM_CurrentFreq); //���µ�ǰƵ��
#endif
        g_fmengine_status.STC_flag = HARDSEEK_COMPLETE;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

