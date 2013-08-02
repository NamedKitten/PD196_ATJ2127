/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-23          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"
#include "app_radio_menu_config.h"

extern bool parse_userlist_uni(char* station_name, userlist_parse_e mode, uint8 num);
extern bool parse_userlist_mul(char* station_name, userlist_parse_e mode, uint8 num);

//�û���̨�б��ļ���( ֻ��flash  ���̵Ĺ̶�λ�ò���)
extern const char userlist[12];

/* �洢����������*/
static const char card_driver_name[] = "card.drv";
static const char flash_driver_name[] = "nand.drv";

/* �ļ���ȡ��ʱbuffer */
uint8 tmp_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radioUI_scene_optionlist(radio_opt_subscene_e mode)
 * \����option  �Ӳ˵�����
 * \param[in]    mode   �Ӳ˵����ģʽ
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note  �������ų�����option  ��Ԥ���б��option
 */
/*******************************************************************************/
app_result_e radioUI_scene_optionlist(radio_opt_subscene_e mode)
{
    app_result_e result = 0;
    /*��ʼ���˵��ؼ�*/
    menu_com_data_t op_menu;

    op_menu.app_id = APP_ID_RADIO;
    op_menu.browse_mode = 0;
    op_menu.menu_func = NULL;

    /*���岻ͬģʽ�µ���ڲ˵�*/
    if (mode == SUBS_OPT_STATION)
    {
        op_menu.menu_entry = OPTION_MENU_STATION;
        op_menu.path_id = 1;
    }
    else
    {
        op_menu.menu_entry = OPTION_PLAYWIN;
        op_menu.path_id = 2;
    }

    /* �ؼ���ʾ�˵�*/
    result = gui_menulist(MENULIST_OPTION, &op_menu);

    //��return ������
    if (result == RESULT_REDRAW)
    {
        //�ص�Ԥ���б�
        if (mode == SUBS_OPT_STATION)
        {
            result = RESULT_RADIO_STATION_LIST;
            g_radio_listtype = STATIONLIST_SAVE;
            /* �����������*/
            g_stalist_entry = ENTER_NORMAL;
        }
        //�ص���̨���Ž���
        else
        {
            //�Ӳ��ŵ�option �˵��ز��Ž���, ������Ƶ��
            need_set_freq = FALSE;
            result = RESULT_RADIO_START_PLAY;
        }
    }
    return result;
}

//�������û���̨���ò��ִ���

/******************************************************************************/
/*
 * \par  Description: mbyte to wchar
 *
 * \param[in]    dest��src

 * \param[out]

 * \return  UNICODE ��������������ʶ���ͽ�����

 * \note    ASCת��UNICODE���Զ������ʶ���ͽ�����
 *******************************************************************************/
static uint16 mbyte_to_wchar(uint8 *dest, uint8 *src)
{
    uint16 num = 0;

#ifdef USE_83NAME_CREAT

    while(*src!=0x00)
    {
        *dest=*src;
        dest++;
        src++;
        num++;
    }

    dest=0x00;

#else

    *dest = 0xff;
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

    *dest = 0x00;
    *(dest + 1) = 0x00;

#endif

    return num;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool check_UserStation_path(void)
 * \��ָ��Ŀ¼�����û���̨�б��ļ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool check_UserStation_path(void)
{
    char* drv_name;
    bool result = TRUE;
    uint8 disk;

    //���û�а�װflash �������ж��Ƿ��п�����
    if ((sys_get_drv_install_info(DRV_GROUP_STG_BASE) & 0xf) == 0)
    {
        drv_type = DRV_GROUP_STG_CARD;
        if ((sys_get_drv_install_info(drv_type) & 0xf) == 0)
        {
            //���flash �Ϳ�������δ��װ��Ĭ��ȡflash �ϵ��ļ�����
            drv_type = DRV_GROUP_STG_BASE;
        }
    }
    else
    {
        //����а�װnand �����������ڲ���nand �ϵĸ�������������û���̨
        drv_type = DRV_GROUP_STG_BASE;
    }

    if (drv_type == DRV_GROUP_STG_BASE)
    {
        drv_name = (char *) flash_driver_name;
        disk = DISK_C;
    }
    else
    {
        drv_name = (char *) card_driver_name;
        disk = DISK_H;
    }

    //��װ�洢����
    if (sys_drv_install(drv_type, 0, drv_name) != 0)
    {
        return FALSE;
    }

    //����FS
    vfs_mount_radio = (uint32) sys_mount_fs(drv_type, disk, 0);
    //if(0>vfs_mount_radio ||2<vfs_mount_radio)
    if (vfs_mount_radio == -1)
    {
        sys_drv_uninstall(drv_type);
        return FALSE;
    }

    //Ĭ�Ͻ����Ŀ¼�����ҽ����ļ�
    vfs_cd(vfs_mount_radio, CD_ROOT, 0);
    mbyte_to_wchar(tmp_buf, (uint8 *) userlist);
    //fp_radio = vfs_file_open(vfs_mount_radio, tmp_buf, FS_OPEN_NORMAL);
    fp_radio = vfs_file_open(vfs_mount_radio, tmp_buf, R_NORMAL_SEEK);

    //���ļ��ɹ�
    if (0 != fp_radio)
    {
        vfs_file_read(vfs_mount_radio, tmp_buf, SECTOR_SIZE, fp_radio);
        /* ȷ���û���̨�б��ļ����뷽ʽ*/
        if ((tmp_buf[0] == 0xff) && (tmp_buf[1] == 0xfe))
        {
            encode_mode = ENCODE_UNICODE;
        }
        else
        {
            encode_mode = ENCODE_MUL;
        }

        //��ʼ������Ϊ-1	��buffer ���ݲ���ֱ��ʹ��
        //cursec_num = 0xffff;
        //cur_offset=0;

        //��ȡ�ļ�����
        result = vfs_file_get_size(vfs_mount_radio, &file_total_byte, fp_radio, 0);
        //��ȡ����ʧ�ܣ��ر��ļ���ж������
        if (!result)
        {
            vfs_file_close(vfs_mount_radio, fp_radio);

            if (vfs_mount_radio != -1)
            {
                //ж���ļ�ϵͳ����
                sys_unmount_fs(vfs_mount_radio);
                vfs_mount_radio = -1;
            }

            //ж�ش洢������ж�غ�װ�ر���ƥ��
            sys_drv_uninstall(drv_type);
        }
        return result;
    }

    //���ļ�ʧ�ܣ�ж������
    if (vfs_mount_radio != -1)
    {
        //ж���ļ�ϵͳ����
        sys_unmount_fs(vfs_mount_radio);
        vfs_mount_radio = -1;
    }

    //ж�ش洢������ж�غ�װ�ر���ƥ��
    sys_drv_uninstall(drv_type);
    return FALSE;
}

/**********************************************************************
 * Description: ���ڶ��û��Զ����̨�б��ļ����н���
 * Input: mode-- ����ģʽ
 *          num -- ������ĵ�̨������
 * Output:  station_name --- ���ڱ�������ȡ�ĵ�̨����
 * Return:  bool   �ɹ�/ ʧ��
 * Note:   �û���̨�б�ǰ��֧����������unicode ����
 ***********************************************************************
 **/
bool radio_parse_userlist(char* station_name, userlist_parse_e mode, uint8 num)
{
    bool ret;
    ret = check_UserStation_path();

    if (!ret)
    {
        return FALSE;
    }

    if (encode_mode == ENCODE_UNICODE)
    {
        ret = parse_userlist_uni(station_name, mode, num);
    }
    else
    {
        ret = parse_userlist_mul(station_name, mode, num);
    }

    //��̨������ɣ��ر��ļ�
    vfs_file_close(vfs_mount_radio, fp_radio);

    if (vfs_mount_radio != -1)
    {
        //ж���ļ�ϵͳ����
        sys_unmount_fs(vfs_mount_radio);
        vfs_mount_radio = -1;
    }

    //�û���̨������ɣ�ж�ش洢������ж�غ�װ�ر���ƥ��
    sys_drv_uninstall(drv_type);
    return ret;
}

