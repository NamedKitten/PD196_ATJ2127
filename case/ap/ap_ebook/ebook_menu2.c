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

//menu ����̫�࣬�ֿ������ļ�



/********************************************************************************
 * Description :��Ŀ¼�˵�ѡ���ִ�к���
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                ��������ֵ
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_carddir(void *param)
{
    //û�п���
    //���̺�FLASH�����ļ�
    uint16 dialog_msg = 0;
    app_result_e    result;
    uint8           disk=DISK_H;
    bool            need_create = FALSE;
    
    if ((device_status & 0x03) == 0x03)                                 //flash��card�����ļ�
    {
        //_show_single_dialog(S_DISK_NOFILE, DIALOG_INFOR_WAIT);
        return RESULT_MAIN_APP;                                         // �ص� MainMenu Ӧ�� 
    }
    if (_detect_device(DRV_GROUP_STG_CARD) == 0)                        
    {
        dialog_msg = S_CARD_OUT;                                        //֪ͨSD��������
    }
    else
    {
        //���л���H��
        if ((music_play_device != DISK_H) && (music_play_device != 0))      //music����SD�̲���
        {
            _close_engine();                                                //֪ͨ��̨�����˳�
        }

        if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
        {
            view_file.file_handle=0;        //�л��̣��ر��ļ����Ա����´�
            fsel_exit();                                                    //�˳�Flash
            libc_memset(&g_ebook_vars.path.file_path.dirlocation, 0x0, sizeof(file_location_t));
            g_ebook_vars.path.file_path.dirlocation.disk = disk;            //�л���
            result =_ebook_filesel_init();                                  //��ʼ����
            if (result == RESULT_CREATE_LIST_APP)
            {
                return result;
            }
            else if(result == RESULT_REDRAW)
            {
                //�ָ��ֳ�����
                g_ebook_vars.path.file_path.dirlocation.disk = DISK_C;          //�л���C��
                _ebook_filesel_init();                                          //��ʼ����
                return result;
            }
            else
            {

            }
        }

        //��C�̵�H��:��ʼ����ⲻ�˲����б�ͻ��л���C�̣�C�̿϶��в����б���ֱ�ӷ���
        //           ��ʼ����⵽�в����б���ֱ�ӷ���
        //��H�̵�H��:�϶��в����б���ֱ�ӷ���
        //������������Ժ��治��Ҫ���ж��Ƿ���ڲ����б�
        
    }
    if (dialog_msg != 0)
    {
        _show_single_dialog(dialog_msg, DIALOG_INFOR_WAIT); 
        return _deal_menu_result();
    }
    enter_mode = ENTER_FROM_DESKTOP;
    return RESULT_EBK_FILELIST;
}



