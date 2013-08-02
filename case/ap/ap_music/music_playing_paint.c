/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-15          1.0              create this file
 *******************************************************************************/
#include "app_music.h"

#include "app_music_playing.h"

#include "mmm_id.h"

//����const data ���к�׺����
static const uint8 support_ext[MusicExtNum][4] =
{
    "MP1", //mp3
    "MP2", //mp3
    "MP3", //mp3
    "WMA", //wma
    "APE", //ape
    "FLA", //flac
    "OGG", //ogg
    "AAC", //aac
    "AAX", //aax
    "M4A", //m4a
    "AA " //aa
};

wregion_t wregion1 _BANK_DATA_ATTR_;
wregion_t wregion2 _BANK_DATA_ATTR_;

extern void music_paint_albumart(uint8 albumart_mode, uint8 full_mode, uint32 file_offset,
    wregion_t *region_in, wregion_t *region_out);

/******************************************************************************/
/*!
 * \par  Description:
 * \id3_type_e play_scene_checktype(char * extname)
 * \��ȡ��׺��
 * \param[in]    extname��׺��buffer  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
static uint8 _music_checktype(uint8 * extname)
{
    uint8 cnt;

    //ת����ǰ��׺��(ת�ɴ�д)
    extname[0] = ((extname[0] >= 'a') && (extname[0] <= 'z')) ? (extname[0] - 'a' + 'A') : extname[0];
    extname[1] = ((extname[1] >= 'a') && (extname[1] <= 'z')) ? (extname[1] - 'a' + 'A') : extname[1];
    extname[2] = ((extname[2] >= 'a') && (extname[2] <= 'z')) ? (extname[2] - 'a' + 'A') : extname[2];
    //�ȽϺ�׺
    for (cnt = 0; cnt < MusicExtNum; cnt++)
    {
        if (libc_strncmp(extname, support_ext[cnt], sizeof(support_ext[cnt])) == 0)
        {
            break;
        }
    }

    if(cnt < 6)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_albumart(void)
 * \����ר������album art
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_paint.c
 * \note
 */
/*******************************************************************************/
void play_paint_albumart(album_picinfo_t *album_picinfo)
{
    uint8 full_mode;

    uint8 *file_name = g_file_path_info.file_path.dirlocation.filename;

    uint32 file_offset = 0;

    full_mode = _music_checktype(file_name);

    libc_memcpy(&wregion1, &(album_picinfo->region), sizeof(wregion_t));

    if (album_picinfo->apic_type == 0)
    {
        //��λ���ļ�
        fsel_browser_set_file(&(g_file_path_info.file_path.plist_location), g_file_path_info.file_source);
        file_offset = album_picinfo->offset;
    }
    else
    {
        //��λ���ļ�
        vfs_file_dir_offset(g_music_mount_id, NULL, &(album_picinfo->cluster_no), 1);
    }
    music_paint_albumart(TRUE, full_mode, file_offset, &wregion1, &wregion2);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_paint(uint32 *display_flag)
 * \���Ʋ��Ž���
 * \param[in]    para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_paint.c
 * \note
 */
/********************************************** *********************************/
void play_scene_paint(uint32 *display_flag)
{
    uint8 play_mode;
    //Ĭ�ϲ�ˢ�±���
    uint8 draw_bg = FALSE;

    style_infor_t style;
    picbox_private_t picbox_param;

    style.type = UI_AP;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = 0;

    //�����audible �򲻻�ѭ��ģʽ��shuffle��eq��
    if (g_audible_now == TRUE)
    {
        //��ѭ��ģʽ��־
        *display_flag &= clr_musicloopmode;
        //���eq��ʶ
        *display_flag &= clr_musiceqmode;
        //���shuffle��ʶ
        *display_flag &= clr_musicshuffle;
    }
    //����
    if ((*display_flag & up_musicBackGDPic) != 0)
    {
        //��ͼ
        style.style_id = PLAY_SCENE_BACKGD;

        ui_show_picbox(&style, NULL);

        //���־
        *display_flag &= clr_musicBackGDPic;

        draw_bg = TRUE;
    }
    //������
    if ((*display_flag & up_musicheadbar) != 0)
    {
        //��ͼ
        if (g_music_config.browser_sel == TRUE)
        {
            gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_BROWSER);
        }
        else
        {
            gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_MUSIC);
        }
        //���־
        *display_flag &= clr_musicheadbar;
    }
    //��Ŀ��Ϣ
    if ((*display_flag & up_musictracks) != 0)
    {
        //number box
        numbox_private_t music_track_num;

        style.style_id = TRACK_NUMBER_BOX;

        music_track_num.value = g_file.num;

        music_track_num.total = g_file.total;

        if (draw_bg == TRUE)
        {
            //����numbox��ʾ ����͸����ʾ
            ui_show_numbox(&style, &music_track_num, NUMBOX_DRAW_ALL);
        }
        else
        {
            //����numbox��ʾ ����͸����ʾ
            ui_show_numbox(&style, &music_track_num, NUMBOX_DRAW_NUMBER);
        }
        //���־
        *display_flag &= clr_musictracks;
    }
    //����״̬
    if ((*display_flag & up_musicplaystatus) != 0)
    {
        style.style_id = PLAY_STATUS_PICBOX;
        if (g_music_status.play_status == PlayAB)
        {
            //ab����
            style.style_id = AB_STATUS_PICBOX;
            picbox_param.frame_id = 2;
        }
        else if (g_music_status.play_status == PlayFast)
        {
            //�����
            if (g_music_status.fast_status == FFPlaySta)
            {
                picbox_param.frame_id = 2;
            }
            else
            {
                picbox_param.frame_id = 3;
            }
        }
        else
        {
            //��������

            if (g_music_status.play_status == PlaySta)
            {
                picbox_param.frame_id = 1;
            }
            else
            {
                picbox_param.frame_id = 0;
            }
        }
        ui_show_picbox(&style, &picbox_param);

        //���־
        *display_flag &= clr_musicplaystatus;
    }
    //����id3��Ϣ
    if ((*display_flag & up_musicinformation) != 0)
    {
        //g_scroll_status = ID3_DEAL_INIT;
        //g_scroll_deal = TRUE;
        play_scroll_init();
        //���־
        *display_flag &= clr_musicinformation;
    }
    //ʱ����Ϣ
    if ((*display_flag & up_musictimeinfo) != 0)
    {
        //��ͼ
        //_show_now_time(TRUE, &style);
        _show_time_progress(TRUE, &style, 0);
        //���־
        *display_flag &= clr_musictimeinfo;
    }
    //eqģʽ
    if ((*display_flag & up_musiceqmode) != 0)
    {
        //��ͼ
        if (g_setting_comval.music_comval.fullsound == VALUE_FULLSOUND_ON)
        {
            //fullsound ����
            style.style_id = FULLSOUND_MODE_PICBOX;
            picbox_param.frame_id = g_setting_comval.music_comval.fullsound;
        }
        else if ((g_setting_comval.music_comval.eq_setting != VALUE_EQ_OFF) || (g_setting_comval.music_comval.srs
                != VALUE_SRS_OFF))
        {
            //eq
            style.style_id = EQ_MODE_PICBOX;
            picbox_param.frame_id = 0x01;//g_setting_comval.music_comval.eq_setting;
        }
        else
        {
            style.style_id = EQ_MODE_PICBOX;
            picbox_param.frame_id = 0;
        }
        ui_show_picbox(&style, &picbox_param);
        //���־
        *display_flag &= clr_musiceqmode;
    }
    //ѭ��ģʽ
    if ((*display_flag & up_musicloopmode) != 0)
    {
        //��ͼ
        music_get_playmode(&play_mode);
        play_mode &= 0x0f;

        style.style_id = LOOP_MODE_PICBOX;

        picbox_param.frame_id = play_mode;

        ui_show_picbox(&style, &picbox_param);
        //���־
        *display_flag &= clr_musicloopmode;
    }
    if ((*display_flag & up_musicshuffle) != 0)
    {
        music_get_playmode(&play_mode);

        style.style_id = SHUFFLE_MODE_PICBOX;

        picbox_param.frame_id = (play_mode >> 0x04);

        ui_show_picbox(&style, &picbox_param);

        *display_flag &= clr_musicshuffle;
    }

    //������
    if ((*display_flag & up_musicprogress) != 0)
    {
        //��ͼ
        //_show_progress(TRUE, &style);
        _show_time_progress(TRUE, &style, 1);
        //���־
        *display_flag &= clr_musicprogress;
    }
    //ר��ͼƬ
    if ((*display_flag & up_musicalbumart) != 0)
    {
        if (g_album_art == TRUE)
        {
            play_paint_albumart(&album_art_info);//�����ǰ�и��,�򽫸����ʾ��ʶ��λ
        }
        else
        {
            //��ʾ����ͼ��
            style.style_id = ALBUM_ART_PICBOX;
            ui_show_picbox(&style, NULL);
        }
        //���־
        *display_flag &= clr_musicalbumart;
    }

    if (g_need_open_pa == TRUE)
    {
#if 0    
        g_need_open_pa = FALSE;
        com_set_sound_out(TRUE, SOUND_OUT_START, NULL);

        if (g_enter_mode == PARAM_BACKTO_PLAYING)
        {
            //�ָ����ֲ���
            music_send_msg(NULL, NULL, MSG_MENGINE_RESUME_SYNC);

            //��ȡ״̬��Ϣ
            music_get_status(&g_music_status);

            //����ˢ�±�־λ
            g_display_bitmap |= up_musicplaystatus;
            g_paint_flag = TRUE;
        }
        else
        {
            //�ر���Ƶ���
            com_set_sound_out(FALSE, SOUND_OUT_PAUSE, NULL);
        }
#else
        g_need_open_pa = FALSE;
        music_standby_play();
        //��ȡ״̬��Ϣ
        music_get_status(&g_music_status);

        //����ˢ�±�־λ
        g_display_bitmap |= up_musicplaystatus;
        g_paint_flag = TRUE;   
        
        //���˶��յ���play������Ϣ
        sys_mq_flush(MQ_ID_GUI);     
#endif
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scroll_init(void)
 * \������ʼ������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
void play_scroll_init(void)
{
    //��ʼ��
    g_scroll_vars.init = TRUE;
    g_scroll_vars.update = TRUE;
    g_scroll_vars.index = 0;
    g_scroll_vars.direct = ID3_SCROLL_HORIZONTAL;

    //��ʼ����ʱ�����ñ�־λ
    g_scroll_deal = FALSE;

    if (play_scroll_deal() == TRUE)
    {
        g_scroll_status = ID3_DEAL_SCROLL;
        g_scroll_vars.init = FALSE;
    }
    else
    {
        g_scroll_status = ID3_DEAL_SWITCH;
        g_scroll_vars.init = FALSE;
        g_scroll_vars.update = FALSE;
    }

    if (scroll_frush_id == -1)
    {
        scroll_frush_id = set_app_timer(APP_TIMER_ATTRB_UI, 5000, play_scroll_update);
    }
    else
    {
        modify_app_timer(scroll_frush_id, 5000);
    }
}
