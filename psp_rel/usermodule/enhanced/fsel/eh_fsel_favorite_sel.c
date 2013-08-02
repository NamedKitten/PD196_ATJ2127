/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    �ղؼв���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_fsel.h"

static userpl_file_t *user_pl_p _BANK_DATA_ATTR_;
static flist_location_t *file_location_p _BANK_DATA_ATTR_;

bool userpl_set_location(flist_location_t*location, fsel_type_e type);
//bool userpl_get_byno(uint8 *strfile, uint16 num);

//static bool userpl_get_title(uint8 *namebuf);
static bool userpl_next(uint8 *strfile);
static bool userpl_prev(uint8 *strfile);
static userpl_file_t* userpl_read_info(uint16 t_file_offset);
static bool userpl_set_position(flist_location_t *location);
static bool userpl_get_file_byno(uint8 *strfile, uint16 num);
//static bool userpl_write_data(uint16 offset);

extern bool fsel_read_sector_data(uint16 sector_offset) __FAR__;

extern uint8 fsel_temp_buf[SECTOR_SIZE];

/******************************************************************************/
/*
 * \par  Description: ��ȡ��һ���ļ����
 *
 * \param[in]

 * \param[out]      strfile--��ext

 * \return          TRUE OR FALSE

 * \note
 *******************************************************************************/
bool userpl_get_next(uint8 *strfile)
{
    //    uint16 temp;
    bool ret_val = FALSE;

    if (0 == fsel_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    if (0 == file_total)
    {
        fsel_close_object();
        return ret_val;
    }

    switch (fsel_init_val.fsel_mode)
    {
        //��ǰһ���ļ�
        case FSEL_MODE_LOOPONE:
        {
            if (0 == file_no)
            {
                file_no = 1;
                ret_val = userpl_get_file_byno(strfile, file_no);
            }
            else
            {
                if (strfile != NULL)
                {
                    *(uint32*) strfile = cur_ext_name;
                }
                ret_val = TRUE;
            }

            break;
        }

        case FSEL_MODE_DIRNOR:
        case FSEL_MODE_INTRO:
        case FSEL_MODE_NORMAL:
        {
            ret_val = userpl_next(strfile);
            break;
        }

        case FSEL_MODE_LOOPALL:
        case FSEL_MODE_LOOPONEDIR:
        {
            ret_val = userpl_next(strfile);
            if (FALSE == ret_val)
            {
                file_no = 0;
                if (TRUE == shuffle_sw)
                {
                    init_shuffle(file_total);
                }
                ret_val = userpl_next(strfile);
            }

            break;
        }

        default:
        break;

    }

    fsel_close_object();
    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description: ��ȡ��һ���ļ����
 *
 * \param[in]

 * \param[out]      strfile--��ext

 * \return          TRUE OR FALSE

 * \note
 *******************************************************************************/
bool userpl_get_prev(uint8 *strfile)
{
    //    uint16 num;
    bool ret_val = FALSE;

    if (0 == fsel_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    if (0 == file_total)
    {
        fsel_close_object();
        return ret_val;
    }

    switch (fsel_init_val.fsel_mode)
    {
        case FSEL_MODE_LOOPONE:
        {
            if (0 == file_no)
            {
                file_no = 1;
                ret_val = userpl_get_file_byno(strfile, file_no);
            }
            else
            {
                if (strfile != NULL)
                {
                    *(uint32*) strfile = cur_ext_name;
                }
                ret_val = TRUE;
            }
            break;
        }

        case FSEL_MODE_DIRNOR:
        case FSEL_MODE_INTRO:
        case FSEL_MODE_NORMAL:
        {
            ret_val = userpl_prev(strfile);
            break;
        }

        case FSEL_MODE_LOOPALL:
        case FSEL_MODE_LOOPONEDIR:
        {
            ret_val = userpl_prev(strfile);
            if (FALSE == ret_val)
            {
                file_no = 0;
                if (TRUE == shuffle_sw)
                {
                    init_shuffle(file_total);
                }
                ret_val = userpl_prev(strfile);
            }

            break;
        }

        default:
        break;
    }

    fsel_close_object();
    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description: ��λָ��������ŵ��ļ�
 *
 * \param[in]    strfile--���ext_name
 num--�������1~n

 * \param[out]

 * \return

 * \note        ���� file_no �� uspl_file_offset
 *******************************************************************************/
bool userpl_get_byno(uint8 *strfile, uint16 num)
{
    bool ret_val;

    if (0 == fsel_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    ret_val = userpl_get_file_byno(strfile, num);

    fsel_close_object();
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ��ȡ�ļ���title��Ϣ
 *
 * \param[in]    namebuf--���buff

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
/*
 static bool userpl_get_title(uint8 *namebuf)
 {
 user_pl_p=userpl_read_info(uspl_file_offset);
 file_location_p=(flist_location_t *)&user_pl_p->location;
 libc_memcpy(namebuf,file_location_p->title,USERPL_TITLE_MAX);
 return TRUE;
 }
 */
/******************************************************************************/
/*
 * \par  Description: ���ò��Ŷϵ�
 *
 * \param[in]    location--�ļ��ϵ���Ϣ
 type--location���ͣ�no used��

 * \param[out]

 * \return      TRUE OR FALSE

 * \note
 *******************************************************************************/
bool userpl_set_location(flist_location_t *location, fsel_type_e type)
{
    bool ret_val;

    type = type;

    if (0 == fsel_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    if (0 == file_total)
    {
        file_no = 0;
        fsel_close_object();
        return FALSE;
    }
    
    save_read_offset = 0xffff;

    if ((NULL == location) || (*(uint32*) &location->filename == 0x00))
    {
//        file_no = 0;
//        fsel_close_object();
        ret_val = FALSE;
        goto exit_func;
    }

    if (location->file_num > file_total)
    {
        ret_val = FALSE;
    }

    else
    {
        ret_val = userpl_get_file_byno(NULL, location->file_num);

        if (FALSE != ret_val)
        {
            user_pl_p = userpl_read_info(uspl_file_offset);
            file_location_p = (flist_location_t *) &user_pl_p->location;

            if (0 != libc_memcmp(&location->cluster_no, &file_location_p->cluster_no, 8))
            {
                ret_val = FALSE;
            }
            else
            {
                //�ӿ��ڸ�ֵ����cur_ext_name
                ret_val = userpl_set_position(location);
            }
        }
    }


exit_func:

    if (FALSE == ret_val)
    {
        file_no = 0;
    }

    fsel_close_object();

    //�����ϵ㲥�ţ����³�ʼ�������    
    if (TRUE == shuffle_sw)
    {
        init_shuffle(file_total);
        filtrate_shuffle_no = file_no;
    }

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description:����ѡ���ļ��Ĵ���λ��
 *
 * \param[in]    location--�ļ���Ϣ

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool userpl_set_position(flist_location_t *location)
{
    cur_ext_name = *(uint32*) &location->filename;
    return vfs_file_dir_offset(vfs_mount, &location->dir_layer_info, &location->cluster_no, 1);
}

/******************************************************************************/
/*
 * \par  Description: ��ȡ���Ŷϵ���Ϣ
 *
 * \param[in]       type--����(no used)

 * \param[out]      location--�ϵ���Ϣ

 * \return          TURE OR FALSE

 * \note
 *******************************************************************************/
bool userpl_get_location(flist_location_t *location, fsel_type_e type)
{
    type = type;

    if (0 == fsel_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    if (0 == file_total)
    {
        fsel_close_object();
        return FALSE;
    }
    save_read_offset = 0xffff;
    user_pl_p = userpl_read_info(uspl_file_offset);
    libc_memcpy(location, &user_pl_p->location, sizeof(flist_location_t));
    location->file_total = file_total;
    location->file_num = file_no;

    fsel_close_object();
    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:��ȡ��ǰ������һ���ļ�
 *
 * \param[in]

 * \param[out]   strfile--��ext_name

 * \return       TRUE OR FALSE

 * \note        ���� file_no �� uspl_file_offset
 *******************************************************************************/
static bool userpl_next(uint8 *strfile)
{
    uint16 temp;

    if (TRUE == shuffle_sw)
    {
        temp = shuffle_next_file();
        if (temp == 0)
        {
            return FALSE;
        }
        return userpl_get_file_byno(strfile, temp);
    }

    if (0 == file_no)
    {
        return userpl_get_file_byno(strfile, 1);
    }

    if (file_no == file_total)
    {
        return FALSE;
    }

    file_no++;

    save_read_offset = 0xffff;
    user_pl_p = userpl_read_info(uspl_file_offset);
    uspl_file_offset = user_pl_p->next_offset;
    user_pl_p = userpl_read_info(uspl_file_offset);

    file_location_p = (flist_location_t *) &user_pl_p->location;

    file_location_p->file_total = file_total;
    file_location_p->file_num = file_no;

    if (FALSE != userpl_set_position(file_location_p))
    {
        if (strfile != NULL)
        {
            *(uint32*) strfile = *(uint32*) &file_location_p->filename;
        }
#ifdef _VC_DEBUG
        printf("%s",file_location_p->title);
#endif
        return TRUE;
    }

    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: ��ȡ��ǰ������һ���ļ�
 *
 * \param[in]

 * \param[out]      strfile--��ext_name

 * \return          TRUE OR FALSE

 * \note           ���� file_no �� uspl_file_offset
 *******************************************************************************/
static bool userpl_prev(uint8 *strfile)
{
    uint16 temp;

    if (TRUE == shuffle_sw)
    {
        temp = shuffle_prev_file();
        if (temp == 0)
        {
            return FALSE;
        }
        return userpl_get_file_byno(strfile, temp);
    }

    if (file_no > 1)
    {
        file_no--;
    }
    else if (0 == file_no) //��һ�ν���
    {
        return userpl_get_file_byno(strfile, file_total);
    }
    else
    {
        //�������һ���ļ���
        return FALSE;
    }

    save_read_offset = 0xffff;
    user_pl_p = userpl_read_info(uspl_file_offset);
    uspl_file_offset = user_pl_p->prev_offset;
    user_pl_p = userpl_read_info(uspl_file_offset);

    file_location_p = (flist_location_t *) &user_pl_p->location;

    file_location_p->file_total = file_total;
    file_location_p->file_num = file_no;

    if (FALSE != userpl_set_position(file_location_p))
    {
        if (strfile != NULL)
        {
            *(uint32*) strfile = *(uint32*) &file_location_p->filename[0];
        }
#ifdef _VC_DEBUG
        printf("%s",file_location_p->title);
#endif
        return TRUE;
    }

    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: ��λָ��������ŵ��ļ�
 *
 * \param[in]    strfile--���ext_name
 num--�������1~n

 * \param[out]

 * \return

 * \note        ���� file_no �� uspl_file_offset
 *******************************************************************************/
static bool userpl_get_file_byno(uint8 *strfile, uint16 num)
{
    uint16 i;

    if (0 == file_total)
    {
        return FALSE;
    }

    if (num > file_total)
    {
        return FALSE;
    }

    save_read_offset = 0xffff;
    uspl_file_offset = uspl_first_offset;
    user_pl_p = userpl_read_info(uspl_file_offset);

    for (i = 1; i != num; i++)
    {
        if (user_pl_p->flag != 1)
        {
            return FALSE;
        }
        uspl_file_offset = user_pl_p->next_offset;
        user_pl_p = userpl_read_info(uspl_file_offset);
    }

    file_location_p = (flist_location_t *) &user_pl_p->location;

    if (FALSE == userpl_set_position(file_location_p))
    {
        return FALSE;
    }
    else
    {
        cur_ext_name = *(uint32*) &file_location_p->filename[0];
        if (NULL != strfile)
        {
            *(uint32*) strfile = *(uint32*) &file_location_p->filename;
        }
#ifdef _VC_DEBUG
        printf("%s",file_location_p->title);
#endif
    }

    file_no = num;

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:��ָ��ƫ�ƺŵ��ļ���Ϣ
 *
 * \param[in]    t_file_offset--�ļ���λ��ƫ�����0~n

 * \param[out]

 * \return       �ļ���Ϣ��bufferָ��

 * \note
 *******************************************************************************/
static userpl_file_t* userpl_read_info(uint16 t_file_offset)
{
    uint16 sector_offset;
    uint16 buffer_offset;

    sector_offset = t_file_offset / USERPL_PRE_SEC + USERPL_HEAD_SEC;//add head sector
    buffer_offset = (t_file_offset % USERPL_PRE_SEC) * USERPL_FILE_SIZE;
    fsel_read_sector_data(sector_offset);

    return (userpl_file_t*) (fsel_temp_buf + buffer_offset);
}

