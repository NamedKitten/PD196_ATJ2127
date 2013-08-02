/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-23          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

/* �ִ���󳤶ȶ����*/
#define  MAX_STRING   50

/* �ִ�����buffer  */
uint8 string_buf_1[MAX_STRING] _BANK_DATA_ATTR_;

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  string_joint_del(uint8 station_num, uint8 mode)
 * \param[in]    station_num  ��̨��  mode   ѯ�ʻ��ǳɹ�����ʾ
 * \param[out]   none
 * \return      none
 * \retval
 * \retval
 * \note   ɾ����̨ʱ�ַ���ƴ�Ӵ���
 */
/*************************************************************************************************/
app_result_e string_joint_del(uint8 station_num, uint8 mode)
{
    string_desc_t string_desc;
    string_desc_t string_source;
    uint8 tmp_buf[2];
    //uint8 tmp;
    dialog_com_data_t dialog_com;
    app_result_e ret;
    uint16 style_id;

    libc_memset(tmp_buf, 0, sizeof(tmp_buf));
    libc_memset(string_buf_1, 0, sizeof(string_buf_1));

    //��̨��ת��ascill
    libc_itoa(station_num, &tmp_buf[0], 2);

#if 0    //itoa �Ѿ�����ת��
    if(station_num <10)
    {
        //ֻ��һλʱ��ת�����轻��˳��
        tmp = tmp_buf[0];
        tmp_buf[0] = tmp_buf[1];
        tmp_buf[1] = tmp;
    }
#endif

    //Դ�ִ�--- ��̨�����
    string_source.data.str = tmp_buf;
    string_source.length = 3;
    string_source.language = ANSIDATAAUTO;

    //Ŀ���ִ�
    string_desc.data.str = string_buf_1;
    string_desc.length = MAX_STRING;
    if (mode == 0)
    {
        //�Ƿ����
        string_desc.argv = CON_CLEAR;
    }
    else
    {
        //����ɹ�
        string_desc.argv = SUC_CLEAR;
    }
    string_desc.language = (int8) g_comval.language_id;

    com_string_format(&string_desc, &string_source);

    //ѯ�ʣ����а�����dialog
    if (mode == 0)
    {
        dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
        dialog_com.button_type = BUTTON_YESNO;
        dialog_com.active_default = BUTTON_YESNO_NO;
        style_id = DIALOG_ASK;
    }
    //�ɹ�����ʾ����������
    else
    {
        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        style_id = DIALOG_MSG;
    }
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    ret = gui_dialog(style_id, &dialog_com);
    return ret;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \bool search_first_freq(uint16 *p_freq)
 * \param[in]    none
 * \param[out]   find freq
 * \return    success/fail
 * \retval
 * \retval
 * \note   �ڵ�ǰ��̨�б����ҵ���һ���������ЧƵ��
 */
/*************************************************************************************************/
bool search_first_freq(uint16 *p_freq)
{
    uint16 *ptr;
    uint8 i;
    uint16 freq;

    ptr = pfreq;
    for (i = 0; i < MAX_STATION_COUNT; i++)
    {
        freq = *ptr;
        if (freq != 0)
        {
            *p_freq = freq;
            //���µ�ǰ���ŵĵ�̨��
            g_radio_config.FMStatus.station = i + 1;
            return TRUE;
        }
        ptr++;
    }
    return FALSE;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e do_del_process(uint8 num)
 * \param[in]    num  ��̨��
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   ��̨ɾ��������ָ����̨�Ӳ˵����ε��б���ɾ��
 * \         ɾ����ص���ǰƵ�㲥��
 */
/*************************************************************************************************/
app_result_e do_del_process(uint8 num)
{
    //ɾ����ص����ų���
    app_result_e result = RESULT_RADIO_START_PLAY;
    bool ret;

    if ((num > MAX_STATION_COUNT) || (num == 0))
    {
        return result;
    }
    if (g_menu_band == Band_Japan)
    {
        g_radio_config.fmstation_jp[num - 1] = 0;
    }
    else if (g_menu_band == Band_Europe)
    {
        g_radio_config.fmstation_eu[num - 1] = 0;
    }
    else
    {
        g_radio_config.fmstation_us[num - 1] = 0;
    }

    //������û�ģʽ���л���Ԥ��ģʽ
    g_radio_config.band_mode = g_menu_band;
    //�����б�ָ��
    init_point_freq();
    //����Ƶ�ʵ��б��һ���ѱ���Ƶ�㲥��
    ret = search_first_freq(&g_radio_config.FMStatus.freq);

    if (!ret)
    {
        //û���ѱ���Ƶ���ˣ�����СƵ�ʲ���
        g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        g_radio_config.FMStatus.station = 0;
    }

    //���±����̨����
    g_radio_config.FMStatus.station_count = get_station_count();

    //ɾ���ɹ���ʾ
    string_joint_del(num, 1);
    return result;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_station_del(radiolist_control_t *p_ml_control)
 * \param[in]    p_ml_control
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   �������Ԥ���̨����
 */
/*************************************************************************************************/
app_result_e deal_station_del(radiolist_control_t *p_ml_control)
{
    app_result_e result = RESULT_NULL;
    uint16 *ptr;
    uint8 station;
    uint16 list_freq;

    //ָ����������б��ף���Ϊ��̨���ź�������ܲ���ͬһ�б�
    ptr = pfreq_list;
    //��̨��
    station = p_ml_control->list_no + 1;
    //��̨�б���Ƶ��ֵ
    list_freq = *(ptr + station - 1);

    if (list_freq == 0)
    {
        //��ѡ��ĵ�̨��û�б����̨������Ӧ
        return result;
    }

    //ѯ��
    result = string_joint_del(station, 0);

    if (result == RESULT_DIALOG_YES)
    {
        //ȷ��ɾ��
        result = do_del_process(station);
    }
#if 0
    else
    {
        //ȡ��ɾ�����ػ��б�
        result = RESULT_REDRAW;
    }
#endif
    return result;

}

