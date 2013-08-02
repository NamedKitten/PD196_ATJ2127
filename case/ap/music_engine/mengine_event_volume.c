/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2012-05-30        1.0              create this file
 *******************************************************************************/

#include "app_mengine.h"

//first 32*2 is normal volume, last 32*2 is eq volume
const uint8 VolumeTbl[32 * 2][2] =
{
    //normal volume
    { 40, 64 },
    { 40, 60 },
    { 39, 64 },
    { 39, 60 },
    { 38, 64 },
    { 38, 60 },
    { 37, 60 },
    { 36, 60 },
    { 35, 60 },
    { 34, 60 },
    { 33, 60 },
    { 32, 60 },
    { 31, 60 },
    { 30, 58 },
    { 28, 60 },
    { 27, 56 },
    { 26, 56 },
    { 25, 56 },
    { 24, 56 },
    { 23, 56 },
    { 22, 54 },
    { 20, 62 },
    { 19, 59 },
    { 17, 62 },
    { 16, 53 },
    { 14, 51 },
    { 12, 51 },
    { 10, 46 },
    { 8, 41 },
    { 6, 33 },
    { 4, 24 },
    { 0, 24 },
    //eq volume
    //normal volume
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 39, 24 },
    { 38, 23 },
    { 37, 22 },
    { 35, 23 },
    { 33, 24 },
    { 32, 22 },
    { 30, 24 },
    { 28, 25 },
    { 27, 22 },
    { 25, 25 },
    { 24, 24 },
    { 22, 24 },
    { 20, 25 },
    { 18, 23 },
    { 16, 22 },
    { 13, 23 },
    { 10, 26 },
    { 8, 20 },
    { 4, 24 },
    { 0, 24 },
};

//֧�ֱ��ٲ����ļ�����
const uint8 music_speed_ext[5][4] =
{
    "MP1", //mp3
    "MP2", //mp3
    "MP3", //mp3
    "WMA", //wma
    "WAV"  //wav
};

static uint8 eq_volume_type[3] =
{
    EQ_EQ_USR_MODE,
    EQ_TYPE_USR_MODE,
    EQ_FULLSOUND_MODE
};

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_set_real_volume(uint8 volume, uint8 eq_type)
 * \����ת��
 * \param[in]    volume  para1 ��������ֵ
 * \param[in]    eq_type  param2 eq����
 * \param[out]   none
 * \return       uint32 the result
 * \retval
 * \retval
 * \ingroup      mengine_event_volume.c
 * \note
 */
/*******************************************************************************/
void mengine_set_real_volume(uint8 volume, uint8 eq_type)
{
    uint8 i;
    uint8 eq_volume = FALSE;

    //������ȥ������ֵ
    uint32 volum_val;

    uint32 volum_val_high;

    uint8 tab_index;

    //�ж�EQ����
    for (i = 0; i < 3; i++)
    {
        if (eq_type == eq_volume_type[i])
        {
            eq_volume = TRUE;
        }
    }

    if (eq_volume == FALSE)
    {
        tab_index = (31 - volume);
    }
    else
    {
        tab_index = (31 - volume) + 32;
    }

    volum_val = (VolumeTbl[tab_index][0] & 0x3f);

    volum_val_high = VolumeTbl[tab_index][1];

    volum_val_high <<= 16;

    volum_val |= volum_val_high;

    //��������
    mmm_mp_cmd(mp_handle, MMM_MP_SET_VOLUME, volum_val);

    return;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_volume(void* msg_ptr)
 * \��������
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//ͬ����־��1B
 *                 sem_id_t sem_id; 		  //�ź���ID��1B
 *                 msg_apps_t msg;		    //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//��Ϣ���ͣ�2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   msg_ptr->msg.content.addr //����eq��Ϣ��ָ��
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_event_volume.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_volume(void* msg_ptr)
{
    //�����������ݵ�ָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //����Ϣָ��ָ�������copy���������
    g_eg_cfg_p->volume = *(uint8*) (data_ptr->msg.content.addr);

    mengine_set_real_volume(g_eg_cfg_p->volume, (uint8) g_eg_cfg_p->eq_info.eq_type);

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;

}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool check_music_file_speed_type(uint8 * extname)
 * \��⵱ǰ�ļ��Ƿ�֧�ֱ��ٲ���
 * \param[in]    extname  �������ļ���׺��
 * \param[out]   none
 * \return       bool
 * \retval       TRUE ֧�ֱ��ٲ���  FALSE ��֧�ֱ��ٲ���
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
bool _check_music_file_speed_type(uint8 * extname)
{
    uint8 cnt;
    bool ret_val = FALSE;
    //ת����ǰ��׺��(ת�ɴ�д)
    extname[0] = ((extname[0] >= 'a') && (extname[0] <= 'z')) ? (extname[0] - 'a' + 'A') : extname[0];
    extname[1] = ((extname[1] >= 'a') && (extname[1] <= 'z')) ? (extname[1] - 'a' + 'A') : extname[1];
    extname[2] = ((extname[2] >= 'a') && (extname[2] <= 'z')) ? (extname[2] - 'a' + 'A') : extname[2];
    //�ȽϺ�׺
    for (cnt = 0; cnt < 5; cnt++)
    {
        if (libc_strncmp(extname, music_speed_ext[cnt], sizeof(music_speed_ext[cnt])) == 0)
        {
            ret_val = TRUE;
        }
    }
    return ret_val;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_set_real_eq(uint8 mode)
 * \������м����������EQ����
 * \param[in]    mode TRUE �����и�����EQ FALSE ͨ���˵�����EQ
 * \param[out]   none
 * \return       none
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void mengine_set_real_eq(uint8 mode)
{
    int32 eq_type;
    uint8 *ext_name;
    bool is_real_eq = TRUE;

    eq_type = mengine_info.eg_config.eq_info.eq_type;

    //��ǰ�Ƿ�Ϊ���ٲ���
    if (eq_type == EQ_VPS_USR_MODE)
    {
        ext_name = mengine_info.eg_config.location.dirlocation.filename;

        if (_check_music_file_speed_type(ext_name) == FALSE)
        {
            //��֧�ֱ��ٲ����ļ����ͣ����ı䵱ǰʵ��EQ
            is_real_eq = FALSE;
            if (mode == FALSE)
            {
                //��֧�ֱ��ٲ����ļ����ڱ��ٲ��Ų���Ҫ����Ӳ��EQ
                return;
            }
        }
    }

    if (is_real_eq == TRUE)
    {
        //��ǰΪ�Ǳ��ٲ���ģʽ��֧�ֱ��ٲ���ģʽ���ļ������õ�ǰeq��������
        mmm_mp_cmd(mp_handle, MMM_MP_SET_EQ, (unsigned int) &mengine_info.eg_config.eq_info);
    }
    else
    {
        //��ǰΪ���ٲ���ģʽ���ļ���֧�ֱ��ٲ���, ������һ�α����EQ��������
        mmm_mp_cmd(mp_handle, MMM_MP_SET_EQ, (unsigned int) &mengine_info.eg_config.eq_info_other);
    }

    if (mode == TRUE)
    {
        //��������
        mengine_set_real_volume(g_eg_cfg_p->volume, (uint8)eq_type);
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_eq(void* msg_ptr)
 * \����eq
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//ͬ����־��1B
 *                 sem_id_t sem_id; 		  //�ź���ID��1B
 *                 msg_apps_t msg;		    //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//��Ϣ���ͣ�2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   msg_ptr->msg.content.addr //����eq��Ϣ��ָ��
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_eq(void* msg_ptr)
{
    uint8 i;
    uint8 eq_type;
    uint8 normal_volume = TRUE;
    bool need_set_volume = FALSE;

    //����eq�������ݵ�ָ��
    private_msg_t * data_ptr = (private_msg_t*) msg_ptr;

    mmm_mp_eq_info_t *eq_info_p = data_ptr->msg.content.addr;

    eq_type = (uint8)(eq_info_p->eq_type);

    for (i = 0; i < 3; i++)
    {
        if (eq_type == eq_volume_type[i])
        {
            //��Ҫ����EQ����
            normal_volume = FALSE;
        }
    }

    //Ҫ���õ���Ч���ͺ������¼����Ч���Ͳ�һ��
    if (eq_type != g_eg_cfg_p->eq_info.eq_type)
    {
        //��������Ч�л�����EQ��Ч����EQ��Ч��FullSound��
        if (normal_volume == TRUE)
        {
            //ֻ����Ч���Ͳ�һ��ʱ����Ҫ����,���ٲ�����Ҫ����֮ǰ���õ�EQֵ����Ϊ���ڲ�֧�ֱ��ٲ��ŵ��ļ�
            //����Ҫ����֮ǰ����Ч
            if (eq_type == EQ_VPS_USR_MODE)
            {
                //���ڱ��ٲ��ţ�����Ҫ����֮ǰ���õ�EQ
                libc_memcpy(&(mengine_info.eg_config.eq_info_other), &(g_eg_cfg_p->eq_info), sizeof(mmm_mp_eq_info_t));
            }

            //��fullsound��Ч�л���������Ч��������ΪNORMAL��Ч
            g_eg_cfg_p->eq_info.eq_type = EQ_NORMAL;
            mengine_set_real_eq(0);
        }

        need_set_volume = TRUE;
    }

    //����Ϣָ��ָ�������copy���������
    libc_memcpy(&(g_eg_cfg_p->eq_info), (data_ptr->msg.content.addr), sizeof(mmm_mp_eq_info_t));

    //���õ�ǰ��EQ
    mengine_set_real_eq(0);

    if (need_set_volume == TRUE)
    {
        mengine_set_real_volume(g_eg_cfg_p->volume, eq_type);
    }

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}
