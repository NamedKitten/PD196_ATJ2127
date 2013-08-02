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
 * Description :������˵�ѡ������
 *
 * Arguments  :
 *                  enter_mode:��ǰ��������(����ebook.h�е�scene_enter_mode_e)
 * Returns     :
 *                  ��������ֵ
 * Notes       :
 *
 ********************************************************************************/

app_result_e _ebook_option_menu(scene_enter_mode_e mode)
{
    app_result_e result = RESULT_NULL;
    uint8 card_flag, reading_flag = 0;                          //card_flag:1��ʾ���ڿ���0��ʾ��������
    menu_com_data_t op_menu;
    engine_state_e play_state = get_engine_state();

    /*��ʼ���˵��ؼ�*/
    card_flag = _detect_device(DRV_GROUP_STG_CARD);             //�������豸�Ƿ����
    // temp_type = enter_type;

    //�򿪵���������˵������
    if (mode == ENTER_FROM_READING)
    {
        reading_flag = 2;
    }
    op_menu.menu_entry = (uint8) (M_NOPLAYNOREAD_NOCARD + card_flag + play_state * 4 + reading_flag);
    op_menu.app_id = APP_ID_EBOOK;
    op_menu.path_id = 0;

    /*���岻ͬģʽ�µ���ڲ˵�*/
    op_menu.menu_func = NULL;

    /*����˵�˽����Ϣ*/
    result = gui_menulist(MENULIST_OPTION, &op_menu);           //������ʾ�˵�����ѡ��

    if ((result == RESULT_REDRAW) || (result == RESULT_CANCEL))
                                                                //�ػ�UI���أ�ͨ�����غ���Ҫ���»��Ƶ�ǰUI��
                                                                //������Ҫ�˳���ǰ��Ϣѭ��
                                                                //���� ȡ��ĳ���¼����������
    {
        if (mode == ENTER_FROM_READING)
        {
            result = RESULT_EBK_PLAY;
            enter_mode = ENTER_FROM_READINGMENU;
            is_reading_menu = FALSE;
        }
        else if (mode == ENTER_FROM_FILELIST)
        {
            result = RESULT_EBK_FILELIST;
            enter_mode = ENTER_FROM_DESKTOP;
        }
        else
        {
        }
    }
    else if ((result == RESULT_CONFIRM) && (enter_mode == ENTER_FROM_READINGMENU))
                                                                //ȷ��ĳ���¼����������
                                                                //�� ���Ķ����ò˵����볡��
    {
        result = RESULT_EBK_PLAY;
    }
    else
    {

    }

    return result;
}

/********************************************************************************
 * Description :���ڲ��Ų˵�ѡ���ִ�к���
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                  ��������ֵ
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_nowplaying(void *param)
{
    return RESULT_NOWPLAYING;
}

/********************************************************************************
 * Description :��һ�β��Ų˵�ѡ���ִ�к���
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                ��������ֵ
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_lastplay(void *param)
{
    return RESULT_LASTPLAY;
}

/********************************************************************************
 * Description :ɾ��������˵�ѡ���ִ�к���
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                ��������ֵ
 * Notes       :
 *
 ********************************************************************************/
app_result_e _menu_func_deleteebook(void *param)
{
    uint32 file_handle;
    uint16 filename_length;
    app_result_e result;
    del_com_data_t del_com;


    //�ȱ�����ǩ
    if (bmk_modify_handle != 0)
    {
        _writeback_bmk_info();
    }

    if (view_file.file_handle != 0)
    {
        if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
        {
            _vram_wb_bmk_headinfo();
        }
    }

    bookmark_total = 0xFF;      //����´����¶�ȡ��ǩ


    fsel_browser_set_file(&g_ebook_vars.path.file_path.dirlocation, FSEL_TYPE_LISTEBOOK);
                                                    //��λ���ļ��ڴ��̵�Ŀ¼��λ��
    filename_length = vfs_get_name(g_ebook_mount_id, BOOKMARK_filename, MAX_FILENAME_LENGTH / 2);

    del_com.filename = BOOKMARK_filename;           //��ǩ�ļ�������ռ�
    del_com.del_mode = DEL_MODE_FILE;               //ɾ��ģʽΪ:ɾ�������ļ�
    del_com.del_self = TRUE;
    del_com.del_func = _close_text_handle;          //delete �Ի�����ѡ���ǡ���ִ�еĺ�����ΪNULL��ʾ����ִ��
    result = gui_delete(&g_ebook_vars.path, &del_com);
                                                    //ɾ���ļ���Ŀ¼�ؼ�������ʵ�֡�
    if (result == RESULT_DELETE_FILE)
    {
        if (_get_bmk_name(filename_length) != 0)
        {
            file_handle = vfs_file_dir_exist(g_ebook_mount_id, BOOKMARK_filename, FS_FILE);
            if (file_handle != (uint32) NULL)
            {
                vfs_file_dir_remove(g_ebook_mount_id, BOOKMARK_filename, FS_FILE);
            }
        }
        //��Ҫ�ж����ڲ����б�ɾ�����ǲ��Ž���ɾ�� RESULT_EBK_PLAY
        if(enter_mode == ENTER_FROM_READING)
        {

            result =  RESULT_EBK_PLAY;         //�л�����һ�������鲥��
        }
        else
        {
            result = RESULT_EBK_FILELIST;
        }
    }
    else if( (result == RESULT_REDRAW)  || (result == RESULT_ERROR))
    {
        //result = RESULT_EBK_FILELIST;     //�����ļ������б����
        result =  RESULT_MENU_EXIT;         //����ԭ���Ľ���
    }
    else
    {
        //�п����������˳���Ϣ
        //QAC
    }
    return result;

}
/********************************************************************************
 * Description :��ǩѡ��˵�ѡ���ִ�к���
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                ��������ֵ
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_selbookmark(void *param)
{
    return _bmk_list_enter(NULL, BMK_SEL);
}
/********************************************************************************
 * Description :ɾ����ǩ�˵�ѡ���ִ�к���
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                ��������ֵ
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_detbookmark(void *param)
{
    return _bmk_list_enter(NULL, BMK_DET);
}
/********************************************************************************
 * Description :�����ǩ�˵�ѡ���ִ�к���
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                ��������ֵ
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_addbookmark(void *param)
{
    app_result_e retval;
    uint8 filename[13];
    uint8 filter_count;
    uint8 cpy_length;
    _get_bookmark_nums(BMK_DET);                            //��ȡ��ǩ����

    //���˻س����� Ӣ�Ŀո�
    filter_count = _filterwhitespace(BOOKMARK_filename, view_file.language);
    libc_memset(filename, 0x0, 13);
    if (filter_count > (MAX_FILTER_COUNT - MAX_TITLE_LEN))
    {
        cpy_length = MAX_FILTER_COUNT - filter_count;
    }
    else
    {
        cpy_length = MAX_TITLE_LEN;
    }
    if (filter_count < BMK_NAME_LENGTH)
    {
        libc_memcpy(filename, &BOOKMARK_filename[filter_count], cpy_length);
    }
    libc_memset(BOOKMARK_filename, 0x0, BMK_NAME_LENGTH);
    if (bookmark_total >= MAX_BKMARK_NUM)
    {
        retval = _show_double_dialog(S_FULLMSG, filename);
        libc_memcpy(BOOKMARK_filename, filename, 12);
        if (retval == RESULT_DIALOG_YES)
        {
            _del_bookmark(0);
            _add_bookmark();
            return RESULT_EBK_PLAY;
        }
    }
    else
    {
        retval = _show_double_dialog(S_ADDOPTION, filename);    //�����Ի�����ʾ�Ƿ������ǩ
        libc_memcpy(BOOKMARK_filename, filename, 12);
        if (retval == RESULT_DIALOG_YES)            //ѡ��YES��ť
        {
            _add_bookmark();
            return RESULT_EBK_PLAY;
        }
    }
    return RESULT_REDRAW;
}
/********************************************************************************
 * Description :ҳ��ѡ��˵�ѡ���ִ�к���
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                ��������ֵ
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_selbookpage(void *param)
{
    return _bmk_list_enter(NULL, PAGE_SEL);
}

/********************************************************************************
 * Description :����Ŀ¼�˵�ѡ���ִ�к���
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                ��������ֵ
 * Notes       :
 *
 ********************************************************************************/
app_result_e _menu_func_flashdir(void *param)
{
    app_result_e result;
    if ((device_status & 0x03) == 0x03)
    {
        return RESULT_MAIN_APP;
    }

    if ((music_play_device != DISK_C) && (music_play_device != 0))
    {
        _close_engine();
    }
    if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_H)
    {
        view_file.file_handle=0;        //�л��̣��ر��ļ����Ա����´�
        fsel_exit();
        libc_memset(&g_ebook_vars.path.file_path.dirlocation, 0x0, sizeof(file_location_t));
        g_ebook_vars.path.file_path.dirlocation.disk = DISK_C;
        result = _ebook_filesel_init();
        if (result == RESULT_CREATE_LIST_APP)
        {
            return RESULT_CREATE_LIST_APP;
        }
        else if(result == RESULT_REDRAW)
        {
            //�ָ��ֳ�����
            g_ebook_vars.path.file_path.dirlocation.disk = DISK_H;          //�л���C��
            _ebook_filesel_init();                                          //��ʼ����
            return result;
        }
        else
        {

        }

    }
    else if ((device_status & 0x01) == 0x01)
    {
        _show_single_dialog(S_DISK_NOFILE, DIALOG_INFOR_WAIT);
        return _deal_menu_result();
    }
    else
    {
    }
    enter_mode = ENTER_FROM_DESKTOP;
    return RESULT_EBK_FILELIST;
}



/********************************************************************************
 * Description :�Զ��������ò˵�ѡ���ִ�к���
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                ��������ֵ
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_autoplay(void *param)
{
    app_result_e retval;
    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item;

    s_param_item.unit_id = -1;
    s_param_item.min = AUTOTIME_MIN;
    s_param_item.max = AUTOTIME_MAX;
    s_param_item.step = AUTOTIME_STEP;
    s_param_item.max_number = 2;
    s_param_item.value = (int16) g_ebook_vars.autoplaytime;
    s_param_item.cycle = TRUE;
    s_param_item.adjust_func = NULL;
    s_param_item.callback = NULL;//_set_autotime_callback;
    s_param_item.value_str = NULL;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 1;
    s_param_private.items = &s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;

    retval = gui_set_parameter(PARAM_SIMPLE, &s_param_com);
    if (retval == RESULT_CONFIRM)
    {
        g_ebook_vars.autoplaytime = (uint8) s_param_com.private_data->items->value;
        retval=RESULT_REDRAW; //������һ��
    }
//    else if(retval == RESULT_REDRAW)//ȡ�����ò������أ��ػ�UI
//    {
//
//    }
//    else//�������ؽ������Ҫ�����⴦��
//    {
//
//    }

    return retval; //������һ��
}
/********************************************************************************
 * Description :�Զ�����ʱ�������ڻص�����
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                ��ǰ���ż��������ֵ
 * Notes       :
 *
 ********************************************************************************/

bool _set_autotime_callback(uint16 value)
{
    value = (uint8) value;
    return TRUE;
}

/********************************************************************************
 * Description :��ʾ��������������ʾ��
 *
 * Arguments  :
 *              option_id:��ʾ��Դ�ļ�ID
 *                    str_name:��ʾ�ַ���
 * Returns     :
 *                ��
 * Notes       :
 *
 ********************************************************************************/

app_result_e _show_double_dialog(uint16 option_id, uint8 *str_name)
{
    string_desc_t string_desc;
    string_desc_t string_name;
    dialog_com_data_t dialog_com;

    _clear_and_set_screen(0xffff, 0x0);
    if (str_name == NULL)
    {
        string_desc.data.id = option_id;
        string_desc.language = UNICODEID;
    }
    else
    {
        string_desc.data.str = (uint8 *)file_list;
        string_desc.length = BMK_NAME_LENGTH;
        string_desc.argv = option_id;
        string_desc.language = (int8) g_comval.language_id;

        string_name.data.str = str_name;
        string_name.length = 12;
        string_name.language = view_file.language;
        com_string_format(&string_desc, &string_name);
    }
    dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
    dialog_com.button_type = BUTTON_YESNOCANCEL;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    if(option_id == S_IS_CREATE_PLAYLIST)
    {
        dialog_com.active_default = 1;
    }
    else
    {
        dialog_com.active_default = 0;
    }
    return gui_dialog(DIALOG_ASK, &dialog_com);
}
/********************************************************************************
 * Description :�������豸�Ƿ����
 * Arguments  :
 *              device_type:�����豸��Ӧ���̷�
 * Returns     :
 *                TRUE or FALSE
 * Notes       :
 *
 ********************************************************************************/

bool _detect_device(uint8 device_type)
{
    if ((support_card_flag == 0) && (device_type == DRV_GROUP_STG_CARD))
    {
        return 0;
    }
    if (sys_detect_disk(device_type) == -1)
    {
        return 0;
    }
    return 1;
}

/********************************************************************************
 * Description :�����ַ����������ַ�ǰ��Ļس����У� Ӣ�Ŀո���ַ�
 *
 * Arguments  :
 *              str:��Ҫ���˵��ַ���
 *              language:��ǰ�ַ�����������(unicode������)
 * Returns     :
 *                ��
 * Notes       :
 *
 ********************************************************************************/
uint8 _filterwhitespace(uint8 *str, int8 language)
{
    uint8 str_count = 0;//�����ַ��ĸ�������
    uint8 add_offset = 1; //�ֽ����ӵ�ƫ�Ƹ���
    uint16 test_str;//��Ҫ�����ַ�������ֵ
    uint8 str_length = MAX_FILTER_COUNT;
    while (1)
    {
        //����ַ�Ϊunicodeʱ������Ҫȡ�����ֽ�
        if (language == UNICODEDATA)
        {
            test_str = (uint16) ((*(str + str_count)) * 0x100 + *((str + str_count) + 1));
            add_offset = 2;
        }
        else
        {
            test_str = *(str + str_count);
        }
        //����ַ�������Ϊ�ջ��߳��Ȳ�����0ʱ��������
        if ((test_str == 0x0) || (str_length <= 0))
        {
            break;
        }
        //����ַ���������Ϊ0x0d,0x0a,0x20�е�һ�֣�����Ϊ���ڻس��Ϳո�
        //����Ҫ����
        if ((test_str == 0x0d) || (test_str == 0x0a) || (test_str == 0x20))
        {

            str_count += add_offset;
            str_length -= add_offset;
        }
        else
        {
            break;
        }
    }
    return str_count;
}

