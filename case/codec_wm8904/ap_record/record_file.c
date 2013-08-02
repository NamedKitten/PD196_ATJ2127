/*
 *******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_file.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *              leiming    2011-10-27          v1.0                 create this file
 *******************************************************************************
 */
/*include file of this application, ��Ӧ�õ�ͷ�ļ�*/
#include "ap_record.h"

/*������д���ٵ�ʱ���������ͷ�ļ�*/
//#include  <card.h>
//#include  <sysdef.h>

//¼���ļ���
static const uint8 rec_filehead[4] = "REC";

//fm¼���ļ���
static const uint8 fmrec_filehead[6] = "FMREC";

//¼����׺��
static const uint8 rec_ext[][4] =
{ "WAV", "MP3", "ACT" }; //wav��ʽ��MP3��ʽ

//record bitrate,ת��Ϊbyte/ms
static const uint32 rec_bit_rate[10] =
{ 1, 4, 8, 16, 24, 32, 64, 96, 128, 192 };


static const uint8 record_short_name[] = "RECORD     ";

/*! \brief recordĿ¼���ļ������� */
static const uint8 record_long_name[] =
{
    0xff, 0xfe,  //flag
    'R',  0x00,
    'E',  0x00,
    'C',  0x00,
    'O',  0x00,
    'R',  0x00,
    'D',  0x00,
    0x00, 0x00  //end flag
};

//dir layer save
//static pdir_layer_t record_layer_buf _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _record_set_path(void)
 * \����Ƿ���¼��RECORDĿ¼��û���򴴽���Ŀ¼
 * \param[in]    void
 * \param[out]   none
 * \return       bool result
 * \retval       TRUE/FALSE
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
static bool _record_set_path(void)
{
    bool dir_exsit = FALSE;

    //��λ����Ŀ¼
    vfs_cd(g_fs_param.vfs_handle, CD_ROOT, 0);

    if (vfs_cd(g_fs_param.vfs_handle, CD_SUB, record_short_name) != 0)
    {
        dir_exsit = TRUE;
    }
    else
    {
        if (vfs_cd(g_fs_param.vfs_handle, CD_SUB, record_long_name) != 0)
        {
            dir_exsit = TRUE;
        }

    }

    if (dir_exsit == FALSE)
    {
        //recordĿ¼�������ڣ�����RECORDĿ¼
        if (!vfs_make_dir(g_fs_param.vfs_handle, (void*) record_long_name))
        {
            return FALSE;
        }

        if (vfs_cd(g_fs_param.vfs_handle, CD_SUB, record_long_name) == 0)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _parse_total_num(void)
 * \���㵱ǰĿ¼�µ���¼���ļ�����
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       none
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
static int8 _record_parse_filenum(void)
{
    if (g_first_enter_flag == TRUE)
    {
        g_file_total = fsel_get_total();
        g_first_enter_flag = FALSE;
    }

    if (g_file_total >= FILE_NUM_MAX)
    {
        return -1;
    }
    else if (g_file_total == 0)
    {
        g_rec_num = 0;
        g_fmrec_num = 0;

        /*����VM����*/
        if (g_record_vars.path.file_path.dirlocation.disk == DISK_C)
        {
            g_record_vars.rec_num = g_rec_num;
            g_record_vars.fmrec_num = g_fmrec_num;
        }
        else
        {
            g_record_vars.rec_num_card = g_rec_num;
            g_record_vars.fmrec_num_card = g_fmrec_num;
        }
        sys_vm_write(&g_record_vars, VM_AP_RECORD);
    }
    else
    {
        ;//nothing for QAC
    }

    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _record_parse_freetime(void)
 * \ͨ�����������ͱ����ʣ��������ʣ��¼��ʱ��
 * \param[in]    void
 * \param[out]   none
 * \return       bool
 * \retval       false:space low; true:space ok
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
static bool _record_parse_freetime(void)
{
    //get total space
    if (!vfs_get_space((void*) g_fs_param.vfs_handle, (void*) &g_fs_param.free_space, GET_SPARE_SPACE))
    {
        return FALSE;
    }

    if (g_fs_param.free_space > REC_SPACE_LOW)
    {
        g_fs_param.free_space -= REC_SPACE_LOW;
    }
    else
    {
        g_fs_param.free_space = 0;
        return FALSE;
    }

    //¼��ʱ��ֻ�ͱ������й�ϵ��ֱ�Ӽ���ó�,��λs
    //���ǵ�free_timeΪ32λ�������ȳ�rec_bit_rate���512
    g_scene_param.free_time = (g_fs_param.free_space - 1) / rec_bit_rate[g_record_vars.bitrate] * 512 / 1000;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _record_format_filename(void)
 * \��ʽ��¼���ļ���
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       NULL
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
static void _record_format_filename(void)
{
    libc_memset(&g_fs_param.rec_filename, 0, 12);

    if (PARAM_FROM_MAINMENU == g_entry_mode)
    {
        //��fm¼���ļ�����Ϊ"RECxxx.xxx"
        libc_strncpy(g_fs_param.rec_filename, rec_filehead, sizeof(rec_filehead)); //record filename
        libc_itoa(g_rec_num, &g_fs_param.rec_filename[3], 3);
        g_fs_param.rec_filename[6] = '.';
        libc_memcpy(&g_fs_param.rec_filename[7], rec_ext[g_record_vars.file_type], 3);
    }
    else if (PARAM_FROM_RADIO == g_entry_mode)
    {
        //fm¼���ļ�����Ϊ"FMRECxxx.xxx"
        libc_strncpy(g_fs_param.rec_filename, fmrec_filehead, sizeof(fmrec_filehead)); //fm record filename
        libc_itoa(g_fmrec_num, &g_fs_param.rec_filename[5], 3);
        g_fs_param.rec_filename[8] = '.';
        libc_memcpy(&g_fs_param.rec_filename[9], rec_ext[g_record_vars.file_type], 3);
    }
    else
    {
        ;//qac need
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 _format_83name(uint8 *old_name, uint8 *new_name)
 * \����ת����83��ʽ�ļ����������ж϶��ļ����ļ��Ƿ����
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       NULL
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
void _format_83name(uint8 *old_name, uint8 *new_name)
{
    uint8 i, j;

    for (i = 0; i < 12; i++)
    {
        new_name[i] = ' ';
    }

    for (i = 0; i < 12; i++)
    {
        if (old_name[i] != '.')
        {
            new_name[i] = old_name[i];
        }
        else
        {
            break;
        }
    }

    //skip '.'
    i++;
    j = 8;

    for (; i < 12; i++, j++)
    {
        if (old_name[i] != '\0')
        {
            new_name[j] = old_name[i];
        }
    }

    new_name[11] = '\0';

    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \BOOL _record_generate_filename(void)
 * \����¼������ţ���001��ʼ����
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       NULL
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
static bool _record_generate_filename(void)
{
    uint32 cluster_no;
    uint16 loops;
    uint16 num;
    uint8 new_name[12];
    uint8 num_buf[4];
    uint8 longname[28];

    loops = 0;

    //���Ƶ�new_name
    libc_memcpy(new_name, g_fs_param.rec_filename, 12);

    //����ļ����Ƿ���xxx����ļ�,��û��RETURN 1
    if (PARAM_FROM_MAINMENU == g_entry_mode)
    {
        //new_name[3] = '0';
        //new_name[4] = '0';
        //new_name[5] = '1';
        num = (uint16)(g_rec_num + 1);
        //add:��ǰ¼���ļ�������200����ӱ��1��ʼ�����ļ�
        if (num > FILE_NUM_MAX)
        {
            num = 1;
        }
        libc_itoa(num, num_buf, 3);
        libc_memcpy(&new_name[3], num_buf, 3);
    }
    else
    {
        //new_name[5] = '0';
        //new_name[6] = '0';
        //new_name[7] = '1';
        num = (uint16)(g_fmrec_num + 1);
        //add:��ǰ¼���ļ�������200����ӱ��1��ʼ�����ļ�
        if (num > FILE_NUM_MAX)
        {
            num = 1;
        }
        libc_itoa(num, num_buf, 3);
        libc_memcpy(&new_name[5], num_buf, 3);
    }

    //num = 1;
    //����ļ�����,���ּ�1
    do
    {
        libc_memset(longname, 0, 28);
        libc_memcpy(longname, new_name, 12);
        com_ansi_to_unicode(longname);
        //���жϳ��ļ����ļ��Ƿ����
        cluster_no = vfs_file_dir_exist(g_fs_param.vfs_handle, longname, 1);
        if (cluster_no == 0)
        {
            _format_83name(new_name, longname);
            //���ж϶��ļ���
            cluster_no = vfs_file_dir_exist(g_fs_param.vfs_handle, longname, 1);
        }

        if (cluster_no == 0)
        {
            //��ǰ�ļ������ڣ��Ե�ǰ�ļ�����record�ļ�������
            libc_memcpy(g_fs_param.rec_filename, new_name, 11);

            if (PARAM_FROM_MAINMENU == g_entry_mode)
            {
                g_rec_num = num;
            }
            else if (PARAM_FROM_RADIO == g_entry_mode)
            {
                g_fmrec_num = num;
            }
            else
            {
                ; //QAC need
            }

            return TRUE;
        }
        else
        {
            num++;
            if (num > FILE_NUM_MAX)
            {
                num = 1;
            }

            libc_itoa(num, num_buf, 3);
            if (PARAM_FROM_MAINMENU == g_entry_mode)
            {
                libc_memcpy(&new_name[3], num_buf, 3);
            }
            else if (PARAM_FROM_RADIO == g_entry_mode)
            {
                libc_memcpy(&new_name[5], num_buf, 3);
            }
            else
            {
            }
        }

        loops++;
    } while (loops < FILE_NUM_MAX);

    return FALSE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _record_create_filename(void)
 * \����¼���ļ���
 * \param[in]    void
 * \param[out]   none
 * \return       BOOL
 * \retval       0:file num full; 1-gen file success
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
static bool _record_create_filename(void)
{
    libc_memset(g_fs_param.rec_filename, 0, sizeof(g_fs_param.rec_filename)); //����ļ���buf

    //��ʽ���ļ���
    _record_format_filename();

    //�����ļ���
    if (!_record_generate_filename())
    {
        return FALSE;
    }

    libc_memset(g_fs_param.show_filename, 0, sizeof(g_fs_param.show_filename));
    libc_memcpy(g_fs_param.show_filename, g_fs_param.rec_filename, 12);
    //ǰ���ʽ���ļ�����ʹ�ö���������
    com_ansi_to_unicode(g_fs_param.rec_filename);
    //g_record_vars.file_num = g_record_vars.rec_num + g_record_vars.fmrec_num;

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _record_file_setup(void)
 * \¼��֮ǰ��Ŀ¼���飬���̿ռ���飬¼���ļ�����
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval       false: ʧ�� true:�ɹ�
 * \ingroup      record_file.c
 * \note
 */
/*******************************************************************************/
bool _record_file_setup(void)
{
    /*����Ƿ���¼��RECORDĿ¼��û���򴴽���Ŀ¼*/
    if (!_record_set_path())
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        return FALSE;
    }

    /*���㵱ǰ¼��Ŀ¼�µ��ļ�����������200�����˳�¼��*/
    if (_record_parse_filenum() < 0)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_RECORD_FILE_FULL);
        return FALSE;
    }

    /*���㵱ǰ��ʽ�µ�¼��ʣ��ʱ��*/
    if (_record_parse_freetime() == FALSE)
    {
        return FALSE;
    }

    /*���ɵ�ǰ¼���ļ���*/
    if (!_record_create_filename())
    {
        //print some error message
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_RECORD_FILE_FULL);
        return FALSE;
    }

    /*����¼���ļ�*/
    g_fs_param.fhandle = vfs_file_create(g_fs_param.vfs_handle, g_fs_param.rec_filename, 0);
    if (g_fs_param.fhandle == 0)
    {
        //failed to create file
        return FALSE;
    }

    /*������д���ٲο�*/
    /*
     if(DISK_H == g_record_vars.path.file_path.dirlocation.disk)
     {
     card_update();
     g_card_para = 0x0f;
     }
     */
    if (g_record_vars.path.file_path.dirlocation.disk == DISK_C)
    {
        //��ǰ֪ͨflash����merge
        vfs_file_write(g_fs_param.vfs_handle, NULL, 0, g_fs_param.fhandle);
        //���Դ��룬ʹ��GPIO
        //act_writel(act_readl(GPIO_AOUTEN) | 0x00400000, GPIO_AOUTEN);
        //act_writel(act_readl(GPIO_AOUTEN) | 0x00000020, GPIO_AOUTEN);
        //act_writel(act_readl(GPIO_ADAT) & 0xffbfffff, GPIO_ADAT);
    }

    /*��ȡpath��file��Ϣ��д��vram���Ա�������дfile head*/
    libc_memset(&g_writehead, 0, sizeof(record_writehead_t));
    //��ȡpath��file��Ϣ
    if (!vfs_file_dir_offset(g_fs_param.vfs_handle, &g_writehead.ptr_layer, &g_writehead.ptr_file_offset, 0))
    {
        return FALSE;
    }

    libc_memcpy(g_writehead.rec_filename, g_fs_param.rec_filename, sizeof(g_fs_param.rec_filename));
    g_writehead.disk_no = g_record_vars.path.file_path.dirlocation.disk;
    g_writehead.rewrite_need = TRUE;
    g_writehead.rec_format = (uint8) g_record_vars.file_type;
    g_writehead.bitrate = rec_bit_rate[g_record_vars.bitrate] * 8 * 1000;
    vram_wr_cnt = 1;
    /*�����籣����ؽṹ��дvm����*/
    sys_vm_write(&g_writehead, VM_AP_RECORD_WAV);

    return TRUE;
}

