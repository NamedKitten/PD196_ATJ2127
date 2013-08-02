/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_filelist.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/
/*!
 * \file     ebook
 * \brief    ebook��filelist����ģ�飬����common_ui�Ŀؼ�����ʾ�ļ��б���Ϣ
 * \author   stevenluo
 * \version  1.0
 * \date  2011/11/11
 *******************************************************************************/
#include  "ebook.h"

#ifdef USE_83NAME_CREAT
const uint8 BMK_LIST_NAME[12]="EBOOK   LIB";
#else
const uint8 BMK_LIST_NAME[] = "EBOOK.LIB";
#endif

/*ȫ�ֱ�������*/
/********************************************************************************
 * Description :�����ļ�����Ļص�����
 *
 * Arguments  :
 *              status:
 * Returns     :
 *                ��������ֵ
 * Notes       :
 *
 ********************************************************************************/

app_result_e _ebook_filelist_callback(file_path_info_t *path_info, uint16 active)
{
    app_result_e result;
    
    //����ѡ���ļ�
    fsel_browser_select(active);
    //��ȡ��ǰ�ļ�·��
    fsel_browser_get_location(&(path_info->file_path), path_info->file_source);
    
    libc_memcpy(&g_ebook_vars.path, path_info, sizeof(file_path_info_t));
    enter_mode = ENTER_FROM_MENUDIR;
    result = _ebook_option_menu(enter_mode);
    return result;
}

/********************************************************************************
 * Description :������ҵ������ļ���ʽ���ļ��б�
 *
 * Arguments  :
 *              status:
 * Returns     :
 *                ��������ֵ
 * Notes       :
 *
 ********************************************************************************/

app_result_e _ebook_filelist(scene_enter_mode_e mode)
{
    app_result_e result;
    dir_com_data_t ebook_dir_data;
    uint8 card_flag;
    uint8 temp_disk;
    ebook_dir_data.list_option_func = _ebook_filelist_callback;
    ebook_dir_data.root_layer = 0;
    ebook_dir_data.menulist = NULL;
    //��·���ĵ�һ��Ŀ¼��ʼ���
    ebook_dir_data.browse_mode = 0;
    //���̺�FLASH�����ļ�
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_BROWSER);        //��ʾĬ��״̬��
    cur_file_diroffset = g_ebook_vars.path.file_path.dirlocation.dir_entry; //��ǰ�ı��ļ���Ŀ¼��Ϣ�е�Ŀ¼ƫ��

    /*����Ŀ¼�ؼ���ʾ��������ļ��б�*/
    result = gui_directory(DIRLIST, &g_ebook_vars.path, &ebook_dir_data);
    
    if (result == RESULT_REDRAW)                                            //���簴return ȡ������
    {
        if (mode == ENTER_FROM_MENUDIR)                                     //����Ӳ˵�Ŀ¼���볡��
        {
            result = _deal_menu_result();
        }
        else
        {
            result = RESULT_MAIN_APP;                                       /* �ص� MainMenu Ӧ�� */
        }
    }
    else if (result == RESULT_EBOOK_PLAY)                                   //��play��ѡ�в���
    {
        result = RESULT_EBK_PLAY;                                           /* �л����浽 ebook �Ĳ��Ž���*/
        _close_text_file(1);
        if (mode == ENTER_FROM_DESKTOP)
        {
            enter_mode = ENTER_FROM_FILELIST;
        }
        else
        {
            enter_mode = ENTER_FROM_MENUDIR;
        }
    }
    else if (result == RESULT_DIR_ERROR_NO_FILE)                            //���󣺸�Ŀ¼��û���ļ����ļ���
    {
        if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
        {
            _show_single_dialog(S_DISK_NOFILE, DIALOG_INFOR_WAIT);
            device_status |= 0x01;          //flashû���ļ�
            //û�п���
            card_flag = _detect_device(DRV_GROUP_STG_CARD);
            if (card_flag == 0)
            {
                return RESULT_MAIN_APP;
            }
//            else
//            {
//                device_status |= 0x01;          //flashû���ļ�
//            }

        }
        else
        {
            _show_single_dialog(S_CARD_NOFILE, DIALOG_INFOR_WAIT);
            device_status |= 0x02;
        }
        if ((device_status & 0x03) == 0x03)     //flash��card�����ļ�
        {
            return RESULT_MAIN_APP;
        }

        //���card�Ƿ����ļ�
        
        if (engine_type != ENGINE_MUSIC)        
        {
            if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
            {
                libc_memset(&g_ebook_vars.path.file_path.dirlocation, 0x0, sizeof(file_location_t));
                g_ebook_vars.path.file_path.dirlocation.disk = DISK_H;  //���ԭ����flash�����л���card
            }
            else
            {
                libc_memset(&g_ebook_vars.path.file_path.dirlocation, 0x0, sizeof(file_location_t));
                g_ebook_vars.path.file_path.dirlocation.disk = DISK_C;
            }
            _close_engine();
            fsel_exit();
            temp_disk=g_ebook_vars.path.file_path.dirlocation.disk;

            if(_ebook_filesel_init() == RESULT_CREATE_LIST_APP)
            {
                return RESULT_CREATE_LIST_APP;
            }
            
            //�����ʼ��flash���̷����ˣ�˵����ʼ��ʧ��
            if(temp_disk != g_ebook_vars.path.file_path.dirlocation.disk)
            {
                device_status |= 0x02;  //cardû���ļ�
                return RESULT_MAIN_APP;
            }
            result=_deal_menu_result();
        }
        else
        {
            return RESULT_MAIN_APP;
        }
    }
    else if(result == RESULT_EBK_FILELIST)   
    {
        enter_mode = ENTER_FROM_DESKTOP;        //��main ap ��browser���볡��
    }
    else 
    {
    }
    return result;
}


/******************************************************************************/
/*
 * \par  Description: mbyte to wchar
 *
 * \param[in]    dest��src

 * \param[out]

 * \return  UNICODE ��������������ʶ���ͽ�����

 * \note    ASCת��UNICODE���Զ������ʶ���ͽ�����
 *******************************************************************************/
uint16 _char_to_unicode(uint8 *dest, uint8 *src)
{
    uint16 num = 0;
    
    #ifdef USE_83NAME_CREAT         //����궨��USE_83NAME_CREAT���� �����ļ�ʹ�ö�������
    
    while(*src!=0x00)
    {
        *dest=*src;
        dest++;
        src++;
        num++;
    }
    
    dest=0x00;
    
    #else                           //�����ļ�ʹ�ó�������                  
            
    *dest = 0xff;                     //�����ʶ��  
    *(dest + 1) = 0xfe;
    dest += 2;          
  
    while (*src != 0x00)
    {
        *dest = *src;
        *(dest + 1) = 0x00;
        dest += 2;
        src++;
        num++;
    }
    
    *dest       = 0x00;                 //���0x00��ʾ����
    *(dest + 1) = 0x00;            
    
    #endif

    return num;

}




/******************************************************************************/
/*
 * \par  Description: �����ǩ�ļ�EBOOK.LIB �Ƿ����
 *
 * \return  1:��ǩ�ļ�EBOOK.LIB ������ ��0��ʾ����

 * \note    
 *******************************************************************************/
uint16 _check_bmk_list(void)
{
    uint16 total_num;
    uint32 ebook_lib;
    
    vfs_cd(g_ebook_mount_id, CD_ROOT, 0);
    _char_to_unicode(bmk_bufFile, (uint8 *) BMK_LIST_NAME);                 //ASCת��UNICODE���Զ������ʶ���ͽ�����
    ebook_lib = vfs_file_open(g_ebook_mount_id, bmk_bufFile, R_NORMAL_SEEK);
    if (ebook_lib != 0)                                                     //������ļ��ɹ�
    {
        vfs_file_read(g_ebook_mount_id, bmk_bufFile, 512, ebook_lib);       //��ȡ��ǩ��Ϣ512�ֽڵ�bmk_bufFile
        libc_memcpy(&total_num, &bmk_bufFile[8], 2);                        //��ȡ��ҳ��total_num
        vfs_file_close(g_ebook_mount_id, ebook_lib);
        if (total_num != 0)
        {
            device_status &= 0x1;                                           //���SD�����ļ�
            return 0;
        }
    }
    device_status |= 0x02;                                                  //���SD�����ļ�
    return 1;
}

void _close_text_handle(void)
{
    if (bmk_modify_handle != 0)
    {
        _writeback_bmk_info();
        vfs_file_close(g_ebook_mount_id, bmk_modify_handle);
        bmk_modify_handle = 0x0;
    }
    if (view_file.file_handle != 0)
    {
        vfs_file_close(g_ebook_mount_id, view_file.file_handle);
        view_file.file_handle = 0x0;
    }
}

