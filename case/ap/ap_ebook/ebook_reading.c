/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ebook.h"

/********************************************************************************
 * Description :�������Ķ��������ں���
 *
 * Arguments  :
 * Returns     :
 *                  ��������ֵ
 * Notes       :
 *
 ********************************************************************************/

app_result_e _ebook_reading(scene_enter_mode_e mode)
{
    app_result_e init_result;
    int8 rtc_time__id;
    uint16 error_id;
    cur_BufSector = -1;

    //������Ķ���������ò˵��˻ص��Ķ�����ʱ���������³�ʼ���ļ���Ϣ
    if (view_file.file_handle == 0x0)
    {
        _close_text_file(0);                        //�ر��Ѵ򿪵��ļ��ļ����
        error_id = _init_file_info();               //�ļ���Ϣ��ʼ������Ҫ�����ļ�location��Ϣ��ȡ
                                                    //���ļ��򿪻�ȡ�ļ���С������������Ϣ
        if (error_id != 0)
        {
            _show_single_dialog(error_id, DIALOG_INFOR_WAIT);
            device_status |= 0x01;          //flashû���ļ�
            if (mode == ENTER_FROM_DESKTOP)
            {
                return RESULT_MAIN_APP;
            }
            else
            {
                return RESULT_EBK_FILELIST;
            }
        }
        
        g_ebook_vars.play_flag=0;                   //����ebook��Ӧ���ֶ����� ��            
        draw_auto_flag = 0;
    }
    else
    {
        draw_auto_flag = g_ebook_vars.play_flag;    //�Ӳ˵�����ebook��Ӧ�ñ���ԭ������״̬
    }

    
    if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_H)
    {
        change_card_state(CARD_STATE_CARD_PLAY);//��Ϊ����SD��
    }

    _init_decode_param();                           //�ı��ļ����������ʼ��
    if (curpagenum >= page_totalnum)                //��ǰ�Ķ��ı�������ҳ��   >=  �����Ķ��ļ�����ҳ��
    {
        curpagenum = 0;
    }
    if (curpagenum != 0)                            //��ǰ�Ķ��ı�������ҳ��   !=  0
    {
        _get_page_offset(curpagenum);
    }
    else
    {
        file_offset = 0;
    }

    sys_bank_flush(AP_BANK_FRONT_ENHANCED_2);       //����BANK2���ݣ���Ҫˢ��
    view_decode.param.language = view_file.language;
    page_count_decode.param.language = view_file.language;
    rtc_time__id = set_app_timer(APP_TIMER_ATTRB_UI, (uint16) (1000 * g_ebook_vars.autoplaytime), _deal_rtc_msg);
    decode_sector = 0;
    init_result = _ebook_text_read();               //��ʼ���ú󣬾Ϳ�ʼ����ebook���˳�ʱ������
                                                    //RESULT_CONFIRM��ȷ��ĳ���¼����������
    kill_app_timer(rtc_time__id);                   //ʱ��ˢ�µ�id

    /*���浱ǰ�ļ�·����Ϣ*/
    if (init_result == RESULT_CANCEL)               //��������
    {
        if (mode == ENTER_FROM_DESKTOP)             //��� ��main ap ��browser���볡��
        {
            init_result = RESULT_MAIN_APP;          //�ص� MainMenu Ӧ��
        }
        else
        {
            init_result = RESULT_EBK_FILELIST;      //�ص�ebook ���ļ��б����
        }
    }
    if (init_result != RESULT_EBK_SETTINGMENU)      //������Ƿ��ص� ebook �Ĳ˵�����
    {
        enter_mode = ENTER_FROM_READING;            //��ǣ����Ķ�������볡��
    }


    if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_H)
    {
        if(get_engine_state() != ENGINE_STATE_PLAYING)  //����û���ڲ���
        {
            change_card_state(CARD_STATE_CARD_IN);
        }
        
    }
    
    return init_result;
}
/********************************************************************************
 * Description :�ı��ļ����������ʼ��
 *
 * Arguments  :
 *              ��
 * Returns     :
 *                  ��
 * Notes       :
 *
 ********************************************************************************/
void _init_decode_param(void)
{
    view_decode.valid = FALSE;
    view_decode.remain = 0;
    view_decode.param.line_count_one_page = ONE_PAGE_ROW_NUM;       //һ����ʾ�ı���ʵ������
    view_decode.param.max_width_one_line = ROW_MAX_WIDTH;           //ÿ�������ʾ ROW_MAX_WIDTH �����ص���
    view_decode.param.mode = 0;                                     //��ʾģʽ���������˿��У��ִ���ʾ��
    view_decode.text_show_line = _show_text_line;                   //��ʾ����һ����������
    view_decode.param.language = view_file.language;                //��������
    libc_memcpy(&page_count_decode, &view_decode, sizeof(view_decode_t));
    page_count_decode.text_show_line = NULL;
}

/********************************************************************************
 * Description :�л�����һҳ
 *
 * Arguments  :

 *                ��
 * Notes       :
 *
 ********************************************************************************/

void _sel_next_page(uint8 page_sel)
{
    //���·�ҳ
    if (curpagenum < (page_totalnum - 1))
    {
        
        if (curpagenum >= (page_totalnum - page_sel))
        {
            curpagenum = page_totalnum - 1;
        }
        else
        {
            curpagenum += page_sel;
        }
        _get_page_offset(curpagenum);                   //��ȡҳ����ļ�ƫ��
        if (decode_sector != (file_offset / 512))       //�����ļ�ƫ�Ʋ��������Ķ���512�ֽ�������
        {
            view_decode.valid = FALSE; 
            decode_sector = 0;
        }
        else if (page_sel > 1) 
        {
            view_decode.valid = FALSE;
        }
        else
        {
        }
        need_draw = TRUE;
    }
    else if (page_count_flag == FALSE)                  //�����Ҫ��������ҳ��
    {
        curpagenum = page_sel - 1;
        cur_BufSector = -1;
        _get_page_offset(curpagenum);
        view_decode.valid = FALSE;
        need_draw = TRUE;
    }
    else
    {
    }
}

/********************************************************************************
 * Description :�л�����һҳ
 *
 * Arguments  :
 *               ��
 * Notes       :
 *
 ********************************************************************************/
void _sel_prev_page(uint8 page_sel)
{
    //���Ϸ�ҳ
    if (curpagenum > 0)
    {
        if (curpagenum <= page_sel)
        {
            curpagenum = 0;
        }
        else
        {
            curpagenum -= page_sel;
        }
        _get_page_offset(curpagenum);
        if (decode_sector != (file_offset / 512))
        {
            view_decode.valid = FALSE;
            decode_sector = 0;
        }
        else
        {
            view_decode.text_buf_length = (uint16) (512 - (file_offset % 512));
        }
        need_draw = TRUE;
    }
    else if (page_count_flag == FALSE)
    {
        curpagenum = page_totalnum - page_sel;
        cur_BufSector = -1;
        _get_page_offset(curpagenum);
        view_decode.valid = FALSE;
        need_draw = TRUE;
    }
    else
    {
    }
}

