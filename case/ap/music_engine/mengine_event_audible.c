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

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_audible_getimage(void* msg_ptr)
 * \��ȡ��ǰaudbile�ļ���ͼƬ��Ϣ
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

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_event_audible.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_audible_getimage(void* msg_ptr)
{
    //��Ϣָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    //��ǰ����ʱ��
    uint32 cur_time = mengine_info.eg_playinfo.cur_time;
    //ͼƬʱ��
    uint32 image_time_cur, image_time_next;
    //���
    uint16 i = 0;

    if (g_audible_info_p->m_cImages_ > 200)
    {
        g_audible_info_p->m_cImages_ = 200;
    }

    for (i = 0; i < 200; i++)
    {
        if (i >= g_audible_info_p->m_cImages_)
        {
            break;
        }
        else
        {
            image_time_cur = g_audible_info_p->imageTime_[i];
            if (i == (g_audible_info_p->m_cImages_ - 1))
            {
                image_time_next = 0xffffffff;
            }
            else
            {
                image_time_next = g_audible_info_p->imageTime_[i + 1];
            }
        }

        if ((cur_time >= image_time_cur) && (cur_time < image_time_next))
        {
            g_image_num = i;
            mengine_info.eg_playinfo.cur_imag_flag = 1;
            mengine_info.eg_playinfo.cur_imag_pos = g_audible_info_p->imagePos_[i];
            break;
        }
    }

    //copy��ǰ����ʱ��ͱ����ʵ���Ϣָ��ָ���������
    libc_memcpy(data_ptr->reply->content, &mengine_info.eg_playinfo, sizeof(mengine_playinfo_t));

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_audible_get_section_info(void* msg_ptr)
 * \��ȡ��ǰ���ŵ�audible�ļ��½���Ϣ
 * \param[in]    msg_ptr  ������Ϣָ��
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_audible_get_section_info(void* msg_ptr)
{
    //��Ϣָ��
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //�½��л���־
    mengine_playinfo_t *play_info = &mengine_info.eg_playinfo;

    libc_memcpy(data_ptr->reply->content, &mengine_info.eg_config.file_cnt, sizeof(musfile_cnt_t));

    //����½��л���־
    play_info->cur_file_switch &= (uint8)(0xfd);

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _check_time_switch(uint32 time_sel)
 * \����ʱ����ת����
 * \param[in]    time_sel  para1
 * \param[in]    time_sel  Ŀ��ʱ��
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
void _check_time_switch(uint32 time_sel)
{
    //ֹͣ or ��ͣʱ ���Ͳ�������
    if ((mengine_info.eg_status.play_status == PauseSta) || (mengine_info.eg_status.play_status == StopSta))
    {
        uint32 checksum = 0;

        uint8 i;
        //����pos�ļ�����
        // ����λ��
        //*(uint32*)(g_audible_info_p->playbackPosFilebuffer[8]) = time_sel;
        _adjust_big_little_endian((uint8*) &time_sel, 4);
        libc_memcpy(&g_audible_info_p->playbackPosFilebuffer[8], &time_sel, 4);

        // ������־
        g_audible_info_p->playbackPosFilebuffer[15] = 0;

        for (i = 8; i < 16; i++)
        {
            checksum += g_audible_info_p->playbackPosFilebuffer[i];
        }
        //У���
        //*(uint32*)(g_audible_info_p->playbackPosFilebuffer[4]) = checksum;
        _adjust_big_little_endian((uint8*) &checksum, 4);
        libc_memcpy(&g_audible_info_p->playbackPosFilebuffer[4], &checksum, 4);
        //����pos�ļ�����
        _deal_audible_posfile(1);

    }
    else
    {
        //seek����
        mmm_mp_cmd(mp_handle, MMM_MP_SEEK, (unsigned int) time_sel);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _chapter_switch_force(bool next_prev)
 * \�ֶ��½��л�
 * \param[in]    next_prev  para1
 * \param[in]    next_prev  1--next;0--prev
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
void _chapter_switch_force(bool next_prev)
{
    //audible�½�ʱ��
    uint32 cha_time_cur;

    uint16 file_num = mengine_info.eg_config.file_cnt.num;

    //״̬��Ϣ
    bool stop_flag = FALSE;

    if (next_prev == 0)
    {
        //����ʱ��ǰ�л�
        if (file_num > 1)
        {
            file_num--;
        }
        else
        {
            stop_flag = TRUE;
        }
    }
    else
    {
        //����ʱ����л�
        if (file_num < g_audible_info_p->m_cChapters_)
        {
            file_num++;
        }
        else
        {
            stop_flag = TRUE;
        }
    }
    //�ֶ��л���Ҫֹͣ
    if (stop_flag == TRUE)
    {
        _stop(STOP_PAUSE);
    }
    else
    {
        if (file_num >= 200)
        {
            file_num = 200;
        }
        if (file_num < 1)
        {
            file_num = 1;
        }
        cha_time_cur = g_audible_info_p->SongChapters_[(file_num - 1)];
        _check_time_switch(cha_time_cur);
        //��ǰ����ʱ��
        mengine_info.eg_playinfo.cur_time = cha_time_cur;
    }
    //�ж�ͼƬ�Ƿ���Ҫˢ��
    _check_audible_image();

    //�½����
    mengine_info.eg_config.file_cnt.num = file_num;

    //�½�����
    mengine_info.eg_config.file_cnt.total = (uint16) g_audible_info_p->m_cChapters_;

    mengine_info.eg_playinfo.cur_file_switch |= 0x02;

    //�½ں�
    //eg_cfg->location.plist_location.file_num = eg_cfg->file_num;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_chapter_next(void* msg_ptr)
 * \�л�����һ��
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

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_chapter_next(void* msg_ptr)
{
    _chapter_switch_force(1);

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_chapter_prev(void* msg_ptr)
 * \�л�����һ��
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

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_chapter_prev(void* msg_ptr)
{
    _chapter_switch_force(0);

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_audible_bookmark(void* msg_ptr)
 * \����audible��ǩ����
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

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_event_audible.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_audible_bookmark(void* msg_ptr)
{
    //��Ϣָ��
    bool bret = TRUE;
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    mengine_config_t *eg_cfg = &mengine_info.eg_config;

    uint32 bk_time;

    //����ϵ�,������_stop֮��,��Ϊ_stop�л��ȡ�ϵ㣬����_stop֮ǰ�ϵ�ᱻ����
    libc_memcpy(&eg_cfg->bk_infor, data_ptr->msg.content.addr, sizeof(mmm_mp_bp_info_t));

    bk_time = (uint32) eg_cfg->bk_infor.bp_time_offset;

    //if(g_eg_status_p->play_status == PlaySta)
    //{
    //    _stop(STOP_NORMAL);
    //}

    _check_time_switch(bk_time);

    if ((g_eg_status_p->play_status == PauseSta) || (g_eg_status_p->play_status == StopSta))
    {
        //���¿�ʼ����, �����ļ�
        //if (_set_file() == TRUE)
        //{
        bret = _play(PLAY_RESUME);//����
        //
    }

    g_image_num = 0xffff;

    mengine_info.eg_playinfo.cur_file_switch |= 0x01;

    //���سɹ�
    mengine_reply_msg(msg_ptr, bret);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *     adjust big or little endian.
 * \param[in]    data_address   Address of Variable.
 * \param[in]    data_length    Length of Variable.
 * \param[out]   none
 * \return       none
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 *******************************************************************************/
void _adjust_big_little_endian(uint8 *data_address, uint8 data_length)
{
    uint8 tmp;
    uint8 *head_p, *end_p;
    head_p = data_address;
    end_p = data_address + data_length - 1;

    for (; head_p < end_p; ++head_p, --end_p)
    {
        tmp = *end_p;
        *end_p = *head_p;
        *head_p = tmp;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _set_chapter_timer(void)
 * \�ж��Ƿ�����audible timer
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
void _set_chapter_timer(void)
{
    if (mengine_info.eg_config.section_mod == SWITCH_SECTION)
    {
        if (g_chapter_timer == -1)
        {
            g_chapter_timer = set_app_timer(APP_TIMER_ATTRB_UI, 1000, _check_audible_chapter);
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _set_imag_timer(void)
 * \�ж��Ƿ�����audible timer
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
void _set_imag_timer(void)
{

    if (g_image_timer == -1)
    {
        g_image_timer = set_app_timer(APP_TIMER_ATTRB_UI, 1000, _check_audible_image);
    }

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _clr_chapter_timer(void)
 * \���audible timer
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
void _clr_chapter_timer(void)
{
    if (g_chapter_timer != -1)
    {
        kill_app_timer(g_chapter_timer);
        g_chapter_timer = -1;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _clr_imag_timer(void)
 * \���audible timer
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
void _clr_imag_timer(void)
{
    if (g_image_timer != -1)
    {
        kill_app_timer(g_image_timer);
        g_image_timer = -1;
    }
}

