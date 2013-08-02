/*******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_paint.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      leiming       2011-10-27          v1.0
 *******************************************************************************/
/*include file of this application, ��Ӧ�õ�ͷ�ļ�*/
#include "ap_record.h"

headbar_icon_e headbar_callback(void)
{
    return HEADBAR_ICON_RECORD;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _record_scene_paint(uint32 display_bitmap)
 * \record����UI��ʾ����
 * \param[in]    uint32* pdisplay_bitmap����ʾ������
 * \param[out]   none
 * \return       none
 * \retval       none
 * \retval       ......
 * \ingroup      record_paint.c
 * \note
 */
/*******************************************************************************/
void _record_scene_paint(uint32* pdisplay_bitmap)
{
    style_infor_t style;
    picbox_private_t picbox_data;
    textbox_private_t textbox_data;

    /*��ʾrecord������ͼƬ*/
    if ((*pdisplay_bitmap & up_recordBackGDPic) != 0)
    {
        style.style_id = STY_RECORD_BG;
        style.type = UI_AP;

        ui_show_picbox((void*) &style, NULL);

        //����ʾ��־
        *pdisplay_bitmap &= clr_recordBackGDPic;
    }

    /*��ʾrecord HeadBar*/
    if ((*pdisplay_bitmap & up_recordHeadBar) != 0)
    {
        //gui_headbar(UpdateAppIcon|UpdateBattery);
        gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_RECORD);

        //����ʾ��־
        *pdisplay_bitmap &= clr_recordHeadBar;
    }

    /*��ʾ¼��״̬ͼ��*/
    if (((*pdisplay_bitmap & up_recordPlayIcon) != 0) || ((*pdisplay_bitmap & up_recordPauseIcon) != 0))
    {
        style.style_id = STY_RECORD_STATE;
        style.type = UI_AP;

        picbox_data.pic_id = -1;

        if ((*pdisplay_bitmap & up_recordPlayIcon) != 0)
        {
            picbox_data.frame_id = 0;
        }
        else if ((*pdisplay_bitmap & up_recordPauseIcon) != 0)
        {
            picbox_data.frame_id = 1;
        }
        else
        {
        }

        ui_show_picbox((void*) &style, (void*) &picbox_data);

        /*����ʾ��־*/
        if ((*pdisplay_bitmap & up_recordPlayIcon) != 0)
        {
            *pdisplay_bitmap &= clr_recordPlayIcon;
        }
        else if ((*pdisplay_bitmap & up_recordPauseIcon) != 0)
        {
            *pdisplay_bitmap &= clr_recordPauseIcon;
        }
        else
        {
        }
    }

    /*��ʾ�ļ���*/
    if ((*pdisplay_bitmap & up_recordFileTxt) != 0)
    {
        style.style_id = STY_RECORD_FILENAME;
        style.type = UI_AP;

        textbox_data.lang_id = ENGLISH;
        textbox_data.str_value = g_fs_param.show_filename;
        textbox_data.str_id = -1;

        ui_show_textbox((void*) &style, (void*) &textbox_data, TEXTBOX_DRAW_NORMAL);

        /*����ʾ��־*/
        *pdisplay_bitmap &= clr_recordFileTxt;
    }

    /*FM¼����ˢ��FMƵ��*/
    if ((*pdisplay_bitmap & up_recordFMFreq) != 0)
    {
        style.style_id = STY_RECORD_FMFREQ;
        style.type = UI_AP;

        textbox_data.lang_id = ENGLISH;
        textbox_data.str_value = g_scene_param.fm_freq; //�˴���ҪȡFMƵ��
        textbox_data.str_id = -1;

        ui_show_textbox((void*) &style, (void*) &textbox_data, TEXTBOX_DRAW_NORMAL);

        /*����ʾ��־*/
        *pdisplay_bitmap &= clr_recordFMFreq;
    }

    /*ˢ¼����ǰʱ��*/
    if ((*pdisplay_bitmap & up_recordTime) != 0)
    {
        _record_show_time_progress(pdisplay_bitmap, TRUE, 0);
    }

    /*��ʾ¼��ʱ�������*/
    if ((*pdisplay_bitmap & up_recordProgress) != 0)
    {
        _record_show_time_progress(pdisplay_bitmap, TRUE, 1);
    }
}

