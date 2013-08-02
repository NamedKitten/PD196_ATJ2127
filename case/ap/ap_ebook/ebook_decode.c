/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_bookmark.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/

#include "ebook.h"
const uint8 str_partition[2] =
{ "/" };





/********************************************************************************
 * Description :�������Ķ������ѭ��������
 *
 * Arguments  :
 *               ��
 * Notes       :
 *
 ********************************************************************************/

app_result_e _ebook_text_read(void)
{
    //��*.txt�ļ�
    app_result_e retval;


    need_draw = TRUE;                                   //�����Ҫ���»���
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_NONE);
#if 0                                                   //�����Զ�����ͼ�걻ˢ��
    gui_headbar(HEADBAR_UPDATE_INIT);
    g_headbar_update = HEADBAR_UPDATE_CHANGE;
#endif
    change_app_state(APP_STATE_PLAYING_ALWAYS);         // ǰ̨AP���ڲ��� UI ״̬���ұ��ⳣ��


    draw_auto_flag = TRUE;                              //�����Ҫ�����Զ�����

    _show_filename();                                   //��ʾ�ļ���
 
    /****** ��������������ѭ�� *****/
    while (1)                       
    {
        _count_page_num();                              //����������ҳ��
        
        _display_text();                                //�������Ķ�������ʾ
        
        retval = _deal_key_msg(NULL, NORMAL_TYPE);      //��������ϵͳ��Ϣ
        if (retval != RESULT_NULL)
        {
            g_ebook_vars.page_num = curpagenum;
            if (retval != RESULT_EBK_READINGMENU)
            {
                _close_text_file(1);
            }
            change_app_state(APP_STATE_NO_PLAY);
            return retval;
        }
        //���������
        sys_os_time_dly(1);
    }

}

/********************************************************************************
 * Description :������ʾ�Ķ������µ��ļ��������ļ����
 *
 * Arguments  :
 *              ��
 * Returns     :
 *                  ��
 * Notes       :
 *
 ********************************************************************************/
void _update_text_head(void)
{

    style_infor_t numbox_style;
    numbox_private_t numbox_param;    
    
    /*** ��ʾҳ�� ***/
    numbox_style.style_id = TRACK_NUMBER_BOX;
    numbox_style.type = UI_AP;
    numbox_param.value = (int32)(curpagenum + 1);
    numbox_param.total = (int32)page_totalnum;

    if(numboxflag == TRUE)
    {
        numboxflag=FALSE;
        _clear_and_set_head_screen(0xffff, 0x0);    //����ͷ��ҳ��
        ui_show_numbox(&numbox_style, &numbox_param, NUMBOX_DRAW_ALL);
    }
    else
    {
        ui_show_numbox(&numbox_style, &numbox_param, NUMBOX_DRAW_NUMBER);
    }

#ifdef PC
    UpdateMainWnd();        //PC�ϣ����´���
#endif

}
/********************************************************************************
 * Description :��ʾ��������Ķ�����
 *
 * Arguments  :
 *               ��
 * Notes       :
 *
 ********************************************************************************/

void _display_text(void)
{
    uint32 temp_offset;
    
    if ((draw_page_num == TRUE) || (need_draw == TRUE))
    {       
        _update_text_head();
        draw_page_num = FALSE;
    }
    if (need_draw == TRUE)
    { 
        _clear_and_set_text_screen(0xffff, 0x0);    //����������ʾ���ֵı���

#ifdef PC
        UpdateMainWnd();                            //PC�ϣ����´���
#endif 

        view_file.file_offset = file_offset;
        _decode_one_page(&view_decode, &view_file, &temp_offset);   //�����ı�������ʾ����
        file_offset += temp_offset;
        need_draw = FALSE;
    }
    _draw_autoplya_icon();
}

/********************************************************************************
 * Description :��ʾ��������Զ���ҳ��ͼ��
 *
 * Arguments  :
 *               ��
 * Notes       :
 *
 ********************************************************************************/
void _draw_autoplya_icon(void)
{
    style_infor_t picbox_style;
    picbox_private_t picbox_param;

    if (draw_auto_flag == TRUE)
    {   
        picbox_style.style_id = AUTO_PLAY_PICBOX;
        picbox_style.type = UI_AP;
        picbox_param.pic_id = -1;//����Ϊ-1

        picbox_param.frame_id = g_ebook_vars.play_flag; //���ű����UIͼ��λ�øպ�һһ��Ӧ

        ui_show_picbox(&picbox_style, &picbox_param);
#ifdef PC
        UpdateMainWnd();        //PC�ϣ����´���
#endif 
        draw_auto_flag = FALSE;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �ı�����ȱҳ�������ļ��ж�ȡһ���������ı�����
 * \param[in]    text_decode���ı�����ṹ��
 * \param[in]    text_file���ı��ļ����
 * \param[out]   none
 * \return       uint16
 * \retval           ��ȡ�ɹ�����TRUE��û���ı����ݿɶ����߶�ȡʧ�ܷ���FALSE
 * \note
 *******************************************************************************/
bool _read_text(view_decode_t *text_decode, view_file_t *text_file)
{
    uint32 file_sector;
    uint32 pos_in_sector;
    bool file_miss = (bool) ((text_file->file_offset / 512 + 1) >= text_file->file_sectors);

    //ת��Ϊ������������ƫ�Ƶ�ַ
    file_sector = text_file->file_offset / 512;
    pos_in_sector = text_file->file_offset % 512;
    decode_sector = file_sector;
    //����β������
    libc_memcpy(text_decode->text_prev, text_decode->text_buf + 512 - BUFF_ONE_ROW, BUFF_ONE_ROW);
    libc_memset(text_decode->text_buf, 0x0, FILE_SECTOR);
    //��ȡ��������
    if (text_file->fseek(file_sector, text_file->file_handle) == FALSE)
    {
        return FALSE;
    }
    if (text_file->fread(text_decode->text_buf, text_file->file_handle) == FALSE)
    {
        return FALSE;
    }

    //������Ч������ʼλ��
    if ((text_decode->remain != 0) && ((BUFF_ONE_ROW - text_decode->remain) >= 0))
    {
        text_decode->text_buf_valid = text_decode->text_prev + (BUFF_ONE_ROW - text_decode->remain);
    }
    else
    {
        text_decode->text_buf_valid = text_decode->text_buf + pos_in_sector;
    }

    if (file_miss != 0)
    {
        text_decode->text_buf_length = 
                        (uint16) (text_file->file_length - text_file->file_offset 
                        + text_decode->remain );
    }
    else
    {
        text_decode->text_buf_length = (uint16) (512 - (text_file->file_offset % 512) + text_decode->remain);
    }


    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ����һҳ�ı����ݣ�����ʾ����
 * \param[in]    text_decode���ı�����ṹ��
 * \param[in]    text_file���ı��ļ����
 * \param[in]    display_flag���Ƿ�Ҫ��ʾ����
 * \return       uint16
 * \retval           ���ص�ǰ�ı�ҳ���볤�ȣ�����-1��ʾ����ʧ��
 * \note         ui_viewpage����ȥ�����ı��ļ������Ƿ��Ѿ����꣬�ϲ�Ӧ���Լ���֤����
 *               ���ı��ĵ�ĩβ������
 *******************************************************************************/
text_end_mode_e _decode_one_page(view_decode_t *text_decode, view_file_t *text_file, uint32 *page_bytes)
{
    uint16 text_buf_index=0;
    uint32 view_total_bytes=0;
    uint32 temp_offset;
    string_desc_t str_desc;                             //�ַ����������ṹ��
    text_show_param_t *param = &(text_decode->param);   //�ı���ʾ�����ṹ��
    text_end_mode_e end_mode;                           //��������н�������
    bool file_miss;                                     //�ļ��Ѷ�ȡ��ϱ��
    uint8 row;                                          //��
    uint8 filter_count = 0;

    str_desc.argv = param->max_width_one_line;
    str_desc.language = param->language;

    temp_offset = text_file->file_offset;
    if (text_decode->valid != 0)
    {
        text_decode->text_buf_valid = text_decode->text_buf + (text_file->file_offset % 512);
    }

    /************************************************************************/
    /*              ���forѭ������ÿ��ÿ�ж�ȡ������ʾ����                 */ 
    /************************************************************************/
    for (row = 0; row < param->line_count_one_page; row++)
    {
        read_text: if (text_decode->valid == FALSE)
        {
            //��������
            if (_read_text(text_decode, text_file) == FALSE)
            {
                view_total_bytes = V_U32_INVALID;
                end_mode = TEXT_END_READ_ERR;
                goto viewpage_exit;
            }

            text_decode->valid = TRUE;
            text_decode->remain = 0;
            text_buf_index = 0;
        }

        // �Զ�����
        str_desc.data.str = text_decode->text_buf_valid + text_buf_index;
        str_desc.length = (text_decode->text_buf_length >= 256) ? 255 : text_decode->text_buf_length;
        //���˻س�����
        filter_count = _filter_enter_line(&str_desc);
        if (filter_count != 0)
        {
            str_desc.data.str += filter_count;
            str_desc.length -= filter_count;
            text_decode->text_buf_length -= filter_count;
            text_buf_index += filter_count;
            if ((temp_offset + text_buf_index) >= text_file->file_length)
            {
                //���˵��ǵ�һ�У������Ѿ������ļ�β�ˣ�����ҳ����1��
                //������0xFFFFFFFF��Ϊ���
                view_total_bytes = V_U32_INVALID;
                goto viewpage_exit;
            }
        }
           
        file_miss = (bool) ((text_file->file_offset / 512 + 1) >= text_file->file_sectors);

        end_mode = (text_end_mode_e) (uint32) ui_get_text_line(&str_desc, (uint32) param->mode, NULL);
        // ���ȱҳ������δ���ļ�ĩβ
        if ((end_mode == TEXT_END_PAGE_MISS) && (file_miss == 0))
        {
            view_total_bytes += text_buf_index;
            text_decode->remain = (uint8) text_decode->text_buf_length;
            text_file->file_offset = (text_file->file_offset / 512 + 1) * FILE_SECTOR;
            text_decode->valid = FALSE;
            goto read_text;
            //���غ����������ݺ��������
        }
        else
        {
            //�ۼӵ�ǰ����ʾ�ֽ���
            text_decode->text_buf_length -= (uint8) (str_desc.result);
            text_buf_index += (uint8) (str_desc.result);

            if (text_decode->text_show_line != NULL)
            {
                str_desc.length = str_desc.result;
                text_decode->text_show_line(&str_desc, row);    //text ��ʾ�лص���������ʾ����һ����������
                sys_os_time_dly(1);
            }


            if (end_mode == TEXT_END_NUL)           //������������Ӧ����ֹ����
            {
                view_total_bytes = text_file->file_length - decode_offset;
                break;                              //ֱ������ָ�Ƚ�β
            }   

            //ȱҳ���ļ��Ѷ�ȡ�꣬Ӧ����ֹ����
            if ( (end_mode == TEXT_END_PAGE_MISS )&& (file_miss == TRUE) )
            {
                break;
            }
        }

        if ((row == 0) && (text_decode->text_show_line != NULL))
        {
            libc_memcpy(BOOKMARK_filename, str_desc.data.str, BMK_NAME_LENGTH);
        }
    }



    //�ۼ����һ�ζ��ص��ı�������ʾ�˵��ֽ���
    view_total_bytes += text_buf_index;
    if (text_decode->text_buf_length > 512)
    {
        text_decode->valid = FALSE;
    }
    viewpage_exit: 
    *page_bytes = view_total_bytes;
    return TEXT_END_PAGE_MISS;
}


/********************************************************************************
 * Description :���˻س����з�
 *
 * Arguments  :
 *               str_desc:����˵��ַ�����Ϣ
 * Returns     :
 *                  �Ѿ����˵����ַ�����
 * Notes       :
 *
 ********************************************************************************/
uint8 _filter_enter_line(string_desc_t *str_desc)
{
    uint8 str_count = 0;
    uint8 add_offset = 1;
    uint16 test_str;
    uint8 *tmp_str = str_desc->data.str;
    uint16 tmp_length = str_desc->length;
    while (1)
    {
        if (tmp_length == 0)
        {
            break;
        }
        if (str_desc->language == UNICODEDATA)
        {
            test_str = (uint16) ((*(tmp_str + 1)) * 0x100 + *tmp_str);
            add_offset = 2;
        }
        else
        {
            test_str = *tmp_str;
        }
        if ((test_str == 0x0d) || (test_str == 0x0a))
        {
            str_count += add_offset;
            tmp_str += add_offset;
            tmp_length -= add_offset;
        }
        else
        {
            break;
        }
    }
    return str_count;
}


/********************************************************************************
 * Description :��ʾ����һ����������
 *
 * Arguments  :
 *               ��
 * Notes       :
 *
 ********************************************************************************/

void _show_text_line(string_desc_t *string_infor, uint8 line)
{
    region_t row_region;
    row_region.x = 0;
    row_region.y = REGION_HEIGHT * line + REGION_HEIGHT;
    row_region.width = REGION_LENGTH;
    row_region.height = REGION_HEIGHT;
    ui_show_string(string_infor, &row_region, DISP_LONGSTR_NO_RECT);
#ifdef PC
    UpdateMainWnd();        //PC�ϣ����´���
#endif
}


/********************************************************************************
 * Description :RTC��Ϣ������
 *
 * Arguments  :
 *               ��
 * Notes       :
 *
 ********************************************************************************/

void _deal_rtc_msg(void)
{
    if (g_ebook_vars.play_flag != 0)
    {
        _sel_next_page(1);
    }
}

#if SHOW_FIFLNAME
/********************************************************************************
 * Description :��ʾ�ļ���
 *
 * Arguments  :
 *               ��
 * Notes       :
 *
 ********************************************************************************/
void _show_filename(void)
{
    style_infor_t picbox_style;                         //����ͼƬ�ṹ
    style_infor_t textbox_style;
    textbox_private_t textbox_param;

    /****** ˢ���ļ������� *****/
    picbox_style.style_id = FILE_NAME_BACKGD;       
    picbox_style.type = UI_AP;
    ui_show_picbox(&picbox_style, NULL);                //��ʾ����  

    /****** ��ʾ�ļ��� *****/
    textbox_style.style_id = FILE_NAME;
    textbox_style.type = UI_AP;
    textbox_param.str_id = -1;
    if((BOOK_filename[0] == 0xff) && (BOOK_filename[1] == 0xfe))   //unicode���ݿ�ͷ��0xff��oxfe
    {
        textbox_param.lang_id = UNICODEDATA ;
    }
    else
    {
        textbox_param.lang_id = (int8)g_comval.language_id;
    } 
    textbox_param.str_value = BOOK_filename;

    ui_show_textbox(&textbox_style, &textbox_param, TEXTBOX_DRAW_NORMAL);
#ifdef PC
    UpdateMainWnd();        //PC�ϣ����´���
#endif
}

/********************************************************************************
 * Description :��ȡ�ļ���
 *
 * Arguments  :
 *               ��
 * Notes       :��Ҫ��vfs_get_name��data������
 *
 ********************************************************************************/
void _get_filename(uint8 *data,uint32 datalength)
{
    uint8   clean_count=0;                              //�����������".txt"
    //��ȡ�ļ��������� .txt��
    if((BOOKMARK_filename[0] == 0xff) && (BOOKMARK_filename[1] == 0xfe))
    {
        datalength <<= 1;
    }
    
    if (datalength > MAX_FILENAME_LENGTH)
    {
        datalength = MAX_FILENAME_LENGTH;
    } 
    
    while( (data[clean_count++] != '.') && (clean_count < datalength) )  
    {       
        //Ѱ��'.'��������'\0'
        if( (data[clean_count]==0 ) && (data[clean_count-1]==0 ))   
        {
            break;                                              //����while
        }
        if((data[clean_count]==0x20) && (data[++clean_count]==0x20) )
        {
            
            break;                                              //����while
        }
    }
    libc_memcpy(BOOK_filename,data,clean_count);   //�����ļ���
    BOOK_filename[clean_count-1]=0;                             //ɾ��".txt"
    if(clean_count < datalength)
    {
        BOOK_filename[clean_count]=0;                           //ɾ��".txt"
    } 
}

#endif  //#if SHOW_FIFLNAME
