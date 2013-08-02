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

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_seekflag(private_msg_t* cur_msg)
 * \����Ӳ����̨�����У���ȡӲ����̨�Ƿ������ʶ
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note
 */
/*******************************************************************************/

app_result_e fmengine_get_seekflag(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    uint8 over_flag;

#ifndef PC
    result = fm_get_seekflag(&over_flag);
#else
    result = 1;
    over_flag = 0;
#endif

    //�ɹ���ȡ����ʶ
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;

        //Ӳ����̨����
        if ((over_flag & 0x01) == 1) //bit0  Ϊ1
        {
            if ((over_flag & 0x02) != 0) //bit1 Ϊ1
            {
                //��Ч̨
                *(hardseek_status_e*) data_ptr->reply->content = HARDSEEK_COMPLETE;
                g_fmengine_status.STC_flag = HARDSEEK_COMPLETE;
            }
            else
            {
                //δ�ҵ���Ч̨
                *(hardseek_status_e*) data_ptr->reply->content = HARDSEEK_NOT_START;
                g_fmengine_status.STC_flag = HARDSEEK_NOT_START;
            }
        }
        else
        {
            *(hardseek_status_e*) data_ptr->reply->content = HARDSEEK_DOING;
            g_fmengine_status.STC_flag = HARDSEEK_DOING;
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
 * \app_result_e fmengine_get_status(private_msg_t* cur_msg)
 * \��ȡ״̬��Ϣ
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_status(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    Engine_Status_t engine_status_tmp;
    uint8 mode;

    //��ȡ״̬�ڵ�:   0-- ��������ʱȡ   1-- Ӳ��seek ������ȡ
    mode = data_ptr->msg.content.data[0];
#ifndef PC
    result = fm_get_status(&engine_status_tmp, mode);
#else
    result = 0;
#endif
    //��ȡ״̬�ɹ�
    if (result != 0)
    {
        engine_status_tmp.FM_VolStatus = g_fmengine_status.FM_VolStatus;
        libc_memcpy(&g_fmengine_status, &engine_status_tmp, sizeof(Engine_Status_t));

        //��ϢӦ��
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        //������״̬�ظ���RadioUI
        libc_memcpy(data_ptr->reply->content, &g_fmengine_status, sizeof(Engine_Status_t));
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
 * \app_result_e fmengine_get_band(private_msg_t* cur_msg)
 * \��ȡ������Ϣ
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_band(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
#ifndef PC
    result = fm_get_band(&g_fmengine_status.FM_CurBand);
#else
    result = 1;
    g_fmengine_status.FM_CurBand = Band_China_US;
#endif
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        *(radio_band_e*) data_ptr->reply->content = g_fmengine_status.FM_CurBand;
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
 * \app_result_e fmengine_get_freq(private_msg_t* cur_msg)
 * \��ȡ��ǰƵ��
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_freq(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
#ifndef PC
    result = fm_get_freq(&g_fmengine_status.FM_CurrentFreq);
#else
    result = 0;
#endif
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        *(uint16*) data_ptr->reply->content = g_fmengine_status.FM_CurrentFreq;
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
 * \app_result_e fmengine_get_intensity(private_msg_t* cur_msg)
 * \��ȡ��ǰ�ź�ǿ��
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_intensity(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;

#ifndef PC
    result = fm_get_intensity(&g_fmengine_status.FM_SignalStg);
#else
    result = 0;
#endif
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        *(uint16*) data_ptr->reply->content = g_fmengine_status.FM_SignalStg;
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
 * \app_result_e fmengine_get_antenna(private_msg_t* cur_msg)
 * \��ȡ����״̬
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_antenna(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    uint8 antena_status;
#ifndef PC
    result = fm_get_antena(&antena_status);
#else
    result = 0;
#endif
    if (result != 0)
    {
        //��ȡ״̬�ɹ�
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        //��ִ�������Ƿ����
        *(uint8*) data_ptr->reply->content = antena_status;
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
 * \app_result_e fmengine_get_stereo(private_msg_t* cur_msg)
 * \��ȡ������״̬
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_stereo(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
#ifndef PC
    result = fm_get_stereo(&g_fmengine_status.FM_Stereo_Status);
#else
    result = 0;
#endif
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        *(FM_Audio_e*) data_ptr->reply->content = g_fmengine_status.FM_Stereo_Status;
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
 * \app_result_e fmengine_get_playflag(private_msg_t* cur_msg)
 * \��ȡ��ǰ����״̬(����or  ��ͣ)
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_playflag(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;

    *(FM_Playstatus_e*) data_ptr->reply->content = g_fmengine_status.FM_VolStatus;
    data_ptr->reply->type = MSG_REPLY_SUCCESS;
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_volume(private_msg_t* cur_msg)
 * \��ȡ��ǰ����ֵ
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_volume(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 cur_vol;

#ifndef test
    cur_vol = (uint8) get_pa_volume();
#else
    cur_vol = (uint8)((*((REG32) (DAC_ANALOG1)) & DAC_ANALOG1_Volume_MASK)>>DAC_ANALOG1_Volume_SHIFT);
#endif
    *(uint8*) data_ptr->reply->content = cur_vol;
    data_ptr->reply->type = MSG_REPLY_SUCCESS;
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_modual_status(private_msg_t* cur_msg)
 * \��ȡFM ģ�鵱ǰ״̬
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_modual_status(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;

    *(Module_Status_e*) data_ptr->reply->content = g_module_status;
    data_ptr->reply->type = MSG_REPLY_SUCCESS;
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

