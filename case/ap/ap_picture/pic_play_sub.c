/*******************************************************************************
 *                              US212A
 *                            Module: PIC_PLAY_SUB
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-12-9 15:04:19           1.0              build this file
 *******************************************************************************/
/*!
 * \file     pic_play_sub.c
 * \brief    ͼƬ������ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ��ģ�����ͼƬ���빦�ܣ�ʵ�ֽ����ʼ������ͼƬ�м���Ľ�����
 * \par      EXTERNALIZED FUNCTIONS:
 *               ��Ҫ����ͼƬ�м��ʵ�ֽ��빦��
 * \version 1.0
 * \date    2011-12-9
 *******************************************************************************/
#include "picture.h"
#include "picture_preview.h"

/******************************************************************************/
/*!
 * \par  Description:
 ȷ��ͼƬ��תʱ������
 * \param[in]    region1 Ӧ�ô��ݸ��м���Ĵ�����
 * \param[in]    region2 �м�����ظ�ap��ͼƬʵ�����꣬��Ҫ�Ը�������б任
 * \param[out]   none
 * \return       none
 * \ingroup      pic_play
 * \note
 * \li   ���ڵײ�δ��ͼƬ��תģʽ��������仯����Ҫap�����һ����
 * \li   ����bmpͼƬ,����ԭ�������½ǣ��������ֻ��xy����λ��
 * \li   ����jpg,bmpͼƬ������ԭ�������Ͻǣ���Ҫ�ڽ�����xy������ٵ���y����
 *******************************************************************************/
static void _format_region(wregion_t *region1, wregion_t *region2)
{
    uint16 temp_x = region2->x;
    uint16 region_width;
    uint16 region_height;

    region_width = region1->w;
    region_height = region1->h;

    if ((g_picture_userinfor.returnflag & RETURNFLAG_ROTATION) != 0)
    {
        //��ת����任
        region2->x = region2->y;
        if (g_picture_userinfor.file_format == IMAGE_BMP)
        {
            region2->y = temp_x;
        }
        else
        {
            region2->y = DISPLAY_LENGTH - temp_x - region1->w;
        }

        //���д���
        region2->x += (region_height - region2->w) / 2;
        region2->y += (region_width - region2->h) / 2;
    }
    else
    {
        //���д���
        region2->x += (region_width - region2->w) / 2;
        region2->y += (region_height - region2->h) / 2;
    }

    return;

}
/******************************************************************************/
/*!
 * \par  Description:
 *    ����ͼƬˢ��ģʽ���趨ͼƬ��������
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_play
 * \note
 * \li   ע���������������ˢ��ģʽ�����һ��Ҫ������������ˢ��ģʽ
 *******************************************************************************/
static void _pic_set_window(void)
{
    uint8 draw_mode;

    //���㴰����
    _format_region(&region_in, &region_out);

    switch (g_picture_userinfor.file_format)
    {

        case IMAGE_GIF:
        case IMAGE_JPG:
        if ((g_picture_userinfor.returnflag & RETURNFLAG_ROTATION) != 0)
        {
            //����
            draw_mode = DRAW_MODE_V_DEF;
        }
        else
        {
            //���� ���ϵ�����
            draw_mode = DRAW_MODE_H_DEF;
        }
        break;
        case IMAGE_BMP:
        if ((g_picture_userinfor.returnflag & RETURNFLAG_ROTATION) != 0)
        {
            //����
            draw_mode = DRAW_MODE_V_PIC_DEF;
        }
        else
        {
            //����
            draw_mode = DRAW_MODE_H_PIC_DEF;
        }
        break;

        default:
        draw_mode = DRAW_MODE_H_DEF;
        break;
    }

    lcd_set_draw_mode(draw_mode);

    //����ͼƬ��ʾ����
    lcd_set_window(&region_out);

}

/******************************************************************************/
/*!
 * \par  Description:
 *      ���ô�����ͼƬ�Ӻ���
 * \param[in]    none
 * \param[out]   none
 * \return       decode_status_e ���ؽ���״̬
 * \ingroup      pic_play
 * \note
 * \li  ����ͼƬ�������ļ�����Ҫ��ɼ���ͼƬ�м�������м�����������SET_FILE����ȹ���
 * \li  �������ʧ�ܣ��᷵����Ӧ�Ĵ���ţ�Ӧ�þݴ˽�����Ӧ����
 *******************************************************************************/
decode_status_e _pic_set_file(void)
{
    int32 dec_result;

    sys_bank_flush(AP_BANK_FRONT_ENHANCED_1);
    sys_bank_flush(AP_BANK_FRONT_ENHANCED_2);

    //װ��ͼƬ�м��
    if (-1 == sys_load_mmm("mmm_id.al", FALSE))
    {
        //����
        return DECODE_LOAD_ERR;
    }
    else
    {
        g_mmm_id_free = FALSE;
    }

    //���߳̾��
    dec_result = mmm_id_cmd(&picture_handle, MMM_ID_OPEN, 0);
    if ((picture_handle == NULL) || (dec_result == MMM_ID_ENGINE_ERROR))
    {
        return DECODE_OPEN_ERR;
    }

    //����SET_FILE����
    dec_result = mmm_id_cmd(picture_handle, MMM_ID_SET_FILE, (uint32) & g_picture_userinfor);
    if (dec_result != 0)
    {
        //�ļ���ʽ���ԣ������޷���ָ������С���ļ�
        //��Ҫ�л��ļ�
        g_picture_change_file |= up_picture_changefile;
        return DECODE_FILE_ERR;
    }
    return DECODE_NO_ERR;

}
/******************************************************************************/
/*!
 * \par  Description:
 *    ͼƬ���뺯��
 * \param[in]    none
 * \param[out]   none
 * \return       decode_status_e ����״̬
 * \ingroup      pic_play
 * \note
 * \li  ���ݴ������ļ������ͽ������ͬʱ����ļ���ʽ���м�顣�����ʱ��̨���ڲ���
        music����ǰ̨Ϊ���bmp/gif,�������ʾ�����ѡ��
 * \li  �ú������ܱ�ͼƬ���������ͼ���빲ͬ����
 *******************************************************************************/
decode_status_e pic_decode(void)
{
    decode_status_e dec_status;
    engine_type_e engine_type = get_engine_type();
    engine_state_e engine_state = get_engine_state();

    if ((engine_type == ENGINE_MUSIC) && (engine_state == ENGINE_STATE_PLAYING))
    {
        //��̨��music���ţ���ֻ֧������������
        g_picture_userinfor.playflag |= PLAYFLAG_BYPASS_MODE;
    }
    else
    {
        g_picture_userinfor.playflag &= PLAYFLAG_FULL_MODE;
    }

    //��Чfont cache
    ui_flush_font_buffer();
    dec_status = _pic_set_file();
    if (dec_status != DECODE_NO_ERR)
    {
        return dec_status;
    }

    if ((engine_type == ENGINE_MUSIC) && (engine_state == ENGINE_STATE_PLAYING))
    {
        if(g_picture_userinfor.file_format != IMAGE_JPG)
        {
            //�ر�ͼƬ����
            pic_decoder_free();

            //����ļ���ʽ�ڵ�ǰ�����Ƿ�֧��
            if (pic_check_engine(g_picture_userinfor.file_format) == FALSE)
            {
                //ȷ�ϲ�����picture����
                return DECODE_UNSUPPORT_ERR;
            }
            else
            {
                //���¿�ʼ����
                g_picture_userinfor.playflag &= 0xfb;

                //���� ��ʼ����
                ui_clear_screen(NULL);

                //����ͼƬ����
                _pic_set_file();
            }
        }
    }

    //�贰
    _pic_set_window();

    //��ʼͼƬ����
    mmm_id_cmd(picture_handle, MMM_ID_DECODE, 0);
    return DECODE_NO_ERR;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ͼƬ�����ʼ��
 * \param[in]  mode ��ʼ��ģʽ 0������ģʽ 1������ͼģʽ
 * \param[out] none
 * \return     the result
 * \retval     TRUE  ��ʼ���ɹ�
 * \retval     FALSE ��ʼ��ʧ��
 * \ingroup    pic_play
 * \note
 * \li  ��ʼ�������򿪽����߳̾������ʼ���ļ��趨
 * \li  �ú������ܱ�ͼƬ���������ͼ���빲ͬ����
 *******************************************************************************/
bool pic_decode_init(uint8 mode)
{
    bool ret_val = TRUE;
    //engine_type_e engine_type = get_engine_type();

    g_mmm_id_free = TRUE;
    ui_clear_screen(NULL);

    //ÿ�ν���֮ǰ���ѡ���ǹر�music�������һ��ѯ��
    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    if (mode == 0)
    {
        //���Ž�������ͼƬ���봰��Ϊȫ��ģʽ
        region_in.x = 0;
        region_in.y = 0;
        region_in.w = DISPLAY_LENGTH;
        region_in.h = DISPLAY_HEIGHT;

        g_picture_change_file = 0;

        //����Ϊ���Զ�����ģʽ
        g_picture_auto_play = FALSE;

        g_picture_userinfor.playflag &= PLAYFLAG_NORMAL_DECODE;

        //��Ҫ��ʱ
        g_picture_userinfor.playflag |= PLAYFLAG_DELAY_MODE;

        //���������Զ����ŵĶ�ʱ��
        timer_pic_play = set_app_timer(APP_TIMER_ATTRB_UI, (uint16)(g_comval.slide_time * 1000), auto_play_flag);
        stop_app_timer(timer_pic_play);
    }
    else
    {
        //����ͼģʽ���������Ȳ�����
        gui_set_headbar_mode(HEADBAR_MODE_NORMAL, this_headbar_icon_id);
        g_picture_userinfor.playflag |= PLAYFLAG_PREVIEW_DECODE;

        //����Ҫ��ʱ
        g_picture_userinfor.playflag &= PLAYFLAG_NODELAY_MODE;

        //��������80ms�������ŵĶ�ʱ��
        timer_pic_play = set_app_timer(APP_TIMER_ATTRB_UI, 80, scroll_play_flag);
        stop_app_timer(timer_pic_play);
    }

    //����Ҫ�����ļ�·��
    ret_val = fsel_set_location(&g_picture_var.path.file_path, g_picture_var.path.file_source);

    if (ret_val == FALSE)
    {
        ret_val = fsel_get_nextfile(g_picture_var.path.file_path.dirlocation.filename);
        if (ret_val == TRUE)
        {
            ret_val = fsel_get_location(&g_picture_var.path.file_path, g_picture_var.path.file_source);
        }

    }
    return ret_val;
}


