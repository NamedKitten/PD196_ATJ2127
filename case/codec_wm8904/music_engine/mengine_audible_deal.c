/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2012-03-02        1.0              create this file
 *******************************************************************************/

#include "app_mengine.h"
#include "sys_info.h"

//pos file buffer
uint8 pos_file_buffer[512] _BANK_DATA_ATTR_;

//dir layer save
pdir_layer_t sys_layer_buf _BANK_DATA_ATTR_;

//bank data for audible resume play
audible_resume_info_t audible_resume_data _BANK_DATA_ATTR_;

//pos file extra name
const uint8 pos_ext[] =
{ '.', 0x00, 'P', 0x00, 'O', 0x00, 'S', 0x00, 0x00, 0x00 };

const uint8 pos_ext_name[] =
{ "POS" };
/******************************************************************************/
/*!
 * \par  Description:
 * \void _deal_audible_posfile(uint8 mode)
 * \��ȡaudible��pos�ļ���Ϣ,�л�����ʱ audible�ļ���sys�ļ�����Ϣ���Լ��豸�ŵ���Ϣ�Ĵ���
 * \param[in]    mode  para1
 * \param[in]    mode  0--get file countent ;1--set file countent ;
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
void _deal_audible_posfile(uint8 mode)
{
    uint32 save_freq;

    //audbile location
    plist_location_t* loaction_ptr;

    //path save
    uint8 temp_offset_save[8];

    //file handle
    handle pos_handle;

    //file name len
    uint16 name_lenth;

    loaction_ptr = (plist_location_t*) &(mengine_info.eg_config.location.plist_location);

    //��Ƶ����
    save_freq = sys_adjust_clk(FREQ_108M, 1);

    //ȡ������ 128 = 256/2
    name_lenth = vfs_get_name(file_sys_id, pos_file_buffer, 128);

    //��MUSIC�ĺ�׺���ĳ�.pos
    if ((pos_file_buffer[0] == 0xff) && (pos_file_buffer[1] == 0xfe))
    {
        uint16 i;
        name_lenth <<= 1;
        name_lenth -= 2;

        if (name_lenth > 255)
        {
            //for QAM
            name_lenth = 255;
        }

        for (i = name_lenth; i > 0; i -= 2)
        {
            if ((uint16) pos_file_buffer[i] == 0x002e)
            {
                break;
            }
        }

        //δ�ҵ���Ч�Ľ�������׺
        if (i == 0)
        {
            //��ǰ�ļ����ȼ�ȥ��׺�������10�ֽ�
            i = name_lenth - sizeof(pos_ext);
        }

        libc_memcpy(&pos_file_buffer[i], pos_ext, sizeof(pos_ext));
    }
    else
    {
        libc_memcpy(&pos_file_buffer[8], pos_ext_name, sizeof(pos_ext_name));
    }
#if 0
    //����audible���ļ����������
    if (mode == 0)
    {
        audbile_tracks.loact_num = loaction_ptr->file_num;
        audbile_tracks.locat_total = loaction_ptr->file_total;
    }
#endif

    //save current path
    vfs_file_dir_offset(file_sys_id, NULL, temp_offset_save, 0);

    pos_handle = vfs_file_open(file_sys_id, pos_file_buffer, OPEN_MODIFY);

    if (pos_handle != 0)
    {
        //��ȡpos�ļ�����
        vfs_file_read(file_sys_id, pos_file_buffer, sizeof(pos_file_buffer), pos_handle);

        if (mode == 0)
        {
            //��pos�ļ����ݴ��ݵ�g_audible_pos������ʹ��
            libc_memcpy(g_audible_pos, pos_file_buffer, sizeof(g_audible_pos));
        }
        else
        {
            //���м���ϵ���Ϣ���浽pos�ļ���
            libc_memcpy(pos_file_buffer, g_audible_info_p->playbackPosFilebuffer, sizeof(g_audible_pos));
            libc_memcpy(g_audible_pos, g_audible_info_p->playbackPosFilebuffer, sizeof(g_audible_pos));
            //sys_vm_write(pos_file_buffer, VM_BMK_INFO + (mengine_info.eg_config.location.dirlocation.file_num * 512));
            vfs_file_seek(file_sys_id, 0, SEEK_SET, pos_handle);
            vfs_file_write(file_sys_id, pos_file_buffer, sizeof(pos_file_buffer), pos_handle);
        }
    }
    else
    {
        //pos�ļ������ڴ���֮
        pos_handle = vfs_file_create(file_sys_id, pos_file_buffer, 0);
        if (pos_handle != 0)
        {
            libc_memset(pos_file_buffer, 0, sizeof(pos_file_buffer));

            if (mode == 0)
            {
                libc_memset(g_audible_pos, 0, sizeof(g_audible_pos));
                g_audible_pos[3] = 2;
                pos_file_buffer[3] = 2;
            }
            else
            {
                libc_memcpy(pos_file_buffer, g_audible_info_p->playbackPosFilebuffer, sizeof(g_audible_pos));
            }

            vfs_file_write(file_sys_id, pos_file_buffer, sizeof(pos_file_buffer), pos_handle);
        }
    }
    if (pos_handle != 0)
    {
        vfs_file_close(file_sys_id, pos_handle);
    }

    //restore path
    vfs_file_dir_offset(file_sys_id, NULL, temp_offset_save, 1);

    //�ָ�Ƶ��
    sys_adjust_clk(save_freq, 1);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_save_audible_breakinfo(void)
 * \���浱ǰ���ŵ�audible�����ϵ�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
void mengine_save_audible_breakinfo(void)
{
    //���ڲ���
    if (g_eg_status_p->play_status == PlaySta)
    {
        //��ȡ�ϵ�
        mmm_mp_cmd(mp_handle, MMM_MP_GET_BREAK_POINT, (unsigned int) &(g_eg_cfg_p->bk_infor));
    }

    libc_memcpy(&(audible_resume_data.bk_infor), &(g_eg_cfg_p->bk_infor), sizeof(mmm_mp_bp_info_t));

    audible_resume_data.locat_info.file_source = g_eg_cfg_p->fsel_type;
    libc_memcpy(&(audible_resume_data.locat_info.file_path), &(g_eg_cfg_p->location), sizeof(musfile_location_u));

    audible_resume_data.magic = MAGIC_AUDBK;
    //дvram
    sys_vm_write(&audible_resume_data, VM_AP_MUSIC_AUDBK);
}

/******************************************************************************/
/*!
 * \par  Description:
 *     adjust big or little endian.
 * \param[in]    data_address   Address of Variable.
 * \param[in]    data_length    Length of Variable.
 * \param[out]   none
 * \return       none
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 *******************************************************************************/
static void _adjust_endian(uint8 *data_address, uint8 data_length)
{
    uint8 tmp;
    uint8 *head_p, *end_p;
    head_p = data_address;
    end_p = data_address + data_length - 1;

    for (; head_p < end_p; ++head_p, --end_p)
    {
        tmp = *end_p;
        *end_p = *head_p;
        *head_p = tmp;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _check_pos_time(void)
 * \����pos���㵱ǰʱ��
 * \param[in]    pos_time  para1
 * \param[in]    pos_time  pos�ļ�����
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
void _check_pos_time(void)
{
    uint32 temp_content = 0;

    _deal_audible_posfile(0);

    //pos�ļ�����
    temp_content = *(uint32*) (&g_audible_pos[8]);

    // ������
    _adjust_endian((uint8*) &temp_content, 4);

    //��ǰ����ʱ��
    mengine_info.eg_playinfo.cur_time = temp_content;

}

