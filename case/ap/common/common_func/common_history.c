/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-23 14:46     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_history.c
 * \brief    ·�����书�ܽӿ�ʵ�֡�
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ����һ��·�����书�ܽӿڣ�������ȡ���䣬������䣬�������ȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel vram_rw��psp���ȵȡ�
 * \version  1.0
 * \date     2011-9-23
*******************************************************************************/

#include "common_func.h"

#define HISTORY_LAYER_ONE   (4)     //ÿ��·������ 4 �ֽ�
#define DIR_HISTORY_ONE     (4 * 16)//֧�� 8 ����Ŀ¼����Ҫ 9 ��·�����䣬Ϊ�˷��� 512 ��������Ϊ 16 ��
#define DIR_PATH_ID_MAX     (4)     //Ӧ�����֧�� 4 ���ļ����·������
#define MENU_HISTORY_ONE    (4 * 8) //֧�� 8 ��·������
#define MENU_PATH_ID_MAX    (8)     //Ӧ�����֧�� 8 ���˵��б�·������
#define DIR_PATH_SEC_MAX    (32 * DIR_PATH_ID_MAX * DIR_HISTORY_ONE / 512)
#define MENU_PATH_SEC_MAX   (32 * MENU_PATH_ID_MAX * MENU_HISTORY_ONE / 512)
static uint8 get_dir_path_id(uint8 disk);

/*! \cond COMMON_API */

//�ļ������·�����䣺
//��VRAM�п��� 32 * DIR_PATH_ID_MAX * DIR_HISTORY_ONE �ṹ����󣬰� app_id �� path_id ��ɾ��������ŷ��ʡ�
//�˵��б�·�����䣺
//��VRAM�п��� 32 * MENU_PATH_ID_MAX * MENU_HISTORY_ONE �ṹ����󣬰� app_id �� path_id ��ɾ��������ŷ��ʡ�

//д·����¼������ʱ������
uint8 history_buf[512] _BANK_DATA_ATTR_;

#ifdef PC
uint8 tmp_dir_history_buf[DIR_HISTORY_ONE];
uint8 tmp_menu_history_buf[MENU_HISTORY_ONE];
#endif

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ȡ·�������
 * \param[in]    index ·��������������ϸ��history_index_t����
 * \param[out]   history ָ��·����������ڷ���·��������
 * \return       bool
 * \retval           TRUE ��ȡ·��������ɹ���
 * \retval           FALSE ��ȡ·��������ʧ�ܡ�
 * \ingroup      common_func
 * \note 
*******************************************************************************/
bool com_get_history_item(history_index_t *index, history_item_t *history)
{
    uint32 base_addr;
    uint32 path_size;
    uint8 *history_addr;
    
    //����·������ṹ���ַ
    if(index->type == PATH_HISTORY_MENU)
    {
        //�˵��б�
        path_size = MENU_HISTORY_ONE;
        base_addr = VM_MENU_HISTORY + index->app_id * (path_size*MENU_PATH_ID_MAX) + index->path_id*path_size;
        
        if(menu_history_valid == FALSE)
        {
            menu_history_addr = NULL;
            #ifndef PC
            sys_malloc(&menu_history_addr, path_size);
            #else
            menu_history_addr = tmp_menu_history_buf;
            #endif
            if(menu_history_addr == NULL)
            {
                return FALSE;
            }
            
            //��������·��������
            if(sys_vm_read(menu_history_addr, base_addr, path_size) == -1)
            {
                return FALSE;
            }
            menu_history_valid = TRUE;
        }
        history_addr = menu_history_addr;
    }
    else
    {
        //�ļ������
        path_size = DIR_HISTORY_ONE;
        base_addr = VM_DIR_HISTORY + index->app_id * (path_size*DIR_PATH_ID_MAX);
        base_addr += get_dir_path_id(index->path_id)*path_size;
        
        if(dir_history_valid == FALSE)
        {
            dir_history_addr = NULL;
            #ifndef PC
            sys_malloc(&dir_history_addr, path_size);
            #else
            dir_history_addr = tmp_dir_history_buf;
            #endif
            if(dir_history_addr == NULL)
            {
                return FALSE;
            }
            
            //��������·��������
            if(sys_vm_read(dir_history_addr, base_addr, path_size) == -1)
            {
                return FALSE;
            }
            dir_history_valid = TRUE;
        }
        history_addr = dir_history_addr;
    }
    
    //������ǰ��·������
    libc_memcpy((uint8 *)history, history_addr + index->layer * HISTORY_LAYER_ONE, HISTORY_LAYER_ONE);
    
    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ����·�������
 * \param[in]    index ·��������������ϸ��history_index_t����
 * \param[in]    history ָ��·��������
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ����·��������ɹ���
 * \retval           FALSE ����·��������ʧ�ܡ�
 * \ingroup      common_func
 * \note 
*******************************************************************************/
bool com_set_history_item(history_index_t *index, history_item_t *history)
{
    uint32 offset;
    uint32 path_size;
    uint8 *history_addr;
    
    //����·������ṹ���ַ
    if(index->type == PATH_HISTORY_MENU)
    {
        //�˵��б�
        path_size = MENU_HISTORY_ONE;
        history_addr = menu_history_addr;
    }
    else
    {
        //�ļ������
        path_size = DIR_HISTORY_ONE;
        history_addr = dir_history_addr;
    }
    
    offset = index->layer * HISTORY_LAYER_ONE;
    
    //�Ƚϵ�ǰҪ���õļ�¼���Ƿ����ı䣨ֻ�Ƚ�list_no��������ı䣬������²����Ϊ 0xffff
    if( (*((uint16 *)(history_addr + offset) + 1)) != (*((uint16 *)history + 1)) )
    {
        libc_memset(history_addr + offset, 0xff, path_size - offset);
    }
    
    //����·��������
    libc_memcpy(history_addr + offset, (uint8 *)history, HISTORY_LAYER_ONE);
    
    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ����·�������
 * \param[in]    index ·��������������ϸ��history_index_t����
 * \param[in]    history ָ��·��������
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ����·��������ɹ���
 * \retval           FALSE ����·��������ʧ�ܡ�
 * \ingroup      common_func
 * \note 
*******************************************************************************/
bool com_update_path_history(history_index_t *index)
{
    uint32 base_addr;
    uint32 sec_addr;
    uint32 offset;
    uint32 path_size;
    uint8 *history_addr;
    
    //����·������ṹ���ַ
    if(index->type == PATH_HISTORY_MENU)
    {
        path_size = MENU_HISTORY_ONE;
        //���û�ж�����Чpath���䣬����ҪдҲ����д��ֱ�ӷ��ؼ���
        if(menu_history_valid == FALSE)
        {
            return TRUE;
        }
        //�˵��б�
        base_addr = VM_MENU_HISTORY + index->app_id * (path_size*MENU_PATH_ID_MAX) + index->path_id*path_size;
        
        history_addr = menu_history_addr;
    }
    else
    {
        path_size = DIR_HISTORY_ONE;
        //���û�ж�����Чpath���䣬����ҪдҲ����д��ֱ�ӷ��ؼ���
        if(dir_history_valid == FALSE)
        {
            return TRUE;
        }
        //�ļ������
        base_addr = VM_DIR_HISTORY + index->app_id * (path_size * DIR_PATH_ID_MAX);
        base_addr += get_dir_path_id(index->path_id) * path_size;
        
        history_addr = dir_history_addr;
    }
    
    //����Ҫ����·���������ڵ�����
    sec_addr = base_addr;
    offset = sec_addr & 0x1ff;
    sec_addr = sec_addr - offset;
    if(sys_vm_read(history_buf, sec_addr, 512) == -1)
    {
        return FALSE;
    }
    
    //����·������
    libc_memcpy(history_buf + offset, history_addr, path_size);
    if(sys_vm_write(history_buf, sec_addr) == -1)
    {
        return FALSE;
    }
    
    //���ٶ�̬�ڴ�ռ䣬�������־
    if(index->type == PATH_HISTORY_MENU)
    {
        #ifndef PC
        sys_free(&menu_history_addr);
        #endif
        menu_history_valid = FALSE;
    }
    else
    {
        #ifndef PC
        sys_free(&dir_history_addr);
        #endif
        dir_history_valid = FALSE;
    }
    
    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ������·������VRAM ��������Ϊ��Ч��0xff����
 * \param[in]    type ·�����ͣ� PATH_HISTORY_MENU ��ʾ�˵��� PATH_HISTORY_DIR ��ʾ�ļ����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ���·������ɹ���
 * \retval           FALSE ���·������ɹ���
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    ����1���������Ӧ�ò˵��б�·����¼����ϵͳ����ʱ����
    com_clear_all_history(PATH_HISTORY_MENU);
    
    ����2���������Ӧ���ļ����·����¼���ڴ��������б�ʱ����
    com_clear_all_history(PATH_HISTORY_DIR);
 * \endcode
 * \note 
*******************************************************************************/
bool com_clear_all_history(uint8 type)
{
    uint32 sec_addr;
    uint16 sec_count, i;
    
    if(type == PATH_HISTORY_MENU)
    {
        //�˵��б�
        sec_addr = VM_MENU_HISTORY;
        sec_count = MENU_PATH_SEC_MAX;
    }
    else
    {
        //�ļ������
        sec_addr = VM_DIR_HISTORY;
        sec_count = DIR_PATH_SEC_MAX;
    }
    
    libc_memset(history_buf, 0xff, 512);
    for(i = 0; i < sec_count; i++)
    {
        if(sys_vm_write(history_buf, sec_addr) == -1)
        {
            return FALSE;
        }
        sec_addr += 512;
    }
    
    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ָ��AP·������VRAM ��������Ϊ��Ч��0xff����
 * \param[in]    type ·�����ͣ�PATH_HISTORY_MENU��ʾ�˵���PATH_HISTORY_DIR��ʾ�ļ����
 * \param[in]    app_id ָ��Ҫ���·����¼AP
 * \param[in]    disk ָ��Ҫ������̷�����Ϊ DISK_C���� DISK_H���� DISK_U
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ���·������ɹ���
 * \retval           FALSE ���·������ɹ���
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    ����1�����ĳӦ���ļ����·����¼�������ļ�ѡ�������ִ���ʱ����
    com_clear_app_history(PATH_HISTORY_DIR, APP_ID_PICTURE, DISK_C);
 * \endcode
 * \note 
 * \li  �ýӿ���ʱֻ�����ָ��AP�ļ������·�����䣬��֧�ֲ˵���
*******************************************************************************/
bool com_clear_app_history(uint8 type, uint8 app_id, uint8 disk)
{
    uint32 base_addr;
    uint32 sec_addr;
    uint32 offset;
    uint32 path_size;
    
    //ֻ���ļ��������Ч
    if(type != PATH_HISTORY_DIR)
    {
        return FALSE;
    }
    
    //����Ҫ������������ڵ�����
    path_size = DIR_HISTORY_ONE;
    base_addr = VM_DIR_HISTORY + app_id * (path_size * DIR_PATH_ID_MAX) + get_dir_path_id(disk) * path_size;
    offset = base_addr & 0x1ff;
    sec_addr = base_addr - offset;
    if(sys_vm_read(history_buf, sec_addr, 512) == -1)
    {
        return FALSE;
    }
    
    //���·������
    libc_memset(history_buf + offset, 0xff, path_size);
    //����·��������
    if(sys_vm_write(history_buf, sec_addr) == -1)
    {
        return FALSE;
    }
    
    return TRUE;
}

/*! \endcond */

/*! \cond COMMON_API */

///��ȡ�ļ������·����¼ path_id��DISK_CΪ0��DISK_HΪ1��DISK_UΪ2
static uint8 get_dir_path_id(uint8 disk)
{
    if(disk == DISK_C)
    {
        return 0;
    }
    else if(disk == DISK_H)
    {
        return 1;
    }
    else//disk == DISK_U
    {
        return 2;
    }
}

/*! \endcond */
