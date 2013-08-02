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

uint8 aud_sys_buffer[512] _BANK_DATA_ATTR_;
/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 _conver_numer(uint8 num)
 * \ת��С���豸ID���֣���hex�ַ�ת��Ϊ����������
 * \param[in]    none
 * \return       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/

static uint8 _conver_numer(uint8 num)
{
    uint8 temp = num;

    if ((temp >= '0') && (temp <= '9'))
    {
        temp -= 0x30;
    }
    else if ((temp >= 'A') && (temp <= 'Z'))
    {
        temp -= 0x37;
    }
    else
    {
        temp -= 0x57;
    }
    return temp;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void _get_firmware_info(void)
 * \��ȡ��ת��С�����豸ID
 * \param[in]    none
 * \return       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
static void _get_firmware_info(void)
{
    uint8 fw_id[16];
    uint8 i;
    uint8 j = 0;

    //device id get Ϊ����д�̶�ֵ,usb�ṩ��ȡ����Ϣ�Ľӿ�
    //libc_memcpy(g_send_audible_p->DeviceId_, DeviceId, sizeof(g_send_audible_p->DeviceId_));
    for (i = 0; i < 16; i++)
    {
        fw_id[i] = 0x00;
    }

    libc_memset(g_send_audible_p->DeviceId_, 0, 20);

    sys_get_fw_info(fw_id, FW_INFO_USB_ID_ADDR, 16);

    //ѹ���洢
    for (i = 0; i < 8; i++)
    {
        g_send_audible_p->DeviceId_[i] = (_conver_numer(fw_id[j]) << 4);
        j++;
        g_send_audible_p->DeviceId_[i] |= _conver_numer(fw_id[j]);
        j++;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void _get_audible_sysfile(void)
 * \��ȡaudible��sys�ļ���Ϣ,���ļ���audible������С������,���洢��VM��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
static void _get_audible_sysfile(void)
{
    sys_vm_read(aud_sys_buffer, VM_AUD_SYS, 512);
    libc_memcpy(g_send_audible_p->ActivationRecord_, aud_sys_buffer, 512);
    sys_vm_read(aud_sys_buffer, VM_AUD_SYS + 512, 48);
    libc_memcpy(&g_send_audible_p->ActivationRecord_[512], aud_sys_buffer, 48);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _deal_audible_sysfile(void)
 * \audible�ļ���sys�ļ�����Ϣ���Լ��豸�ŵ���Ϣ�Ĵ���
 * \param[in]    void  para1
 * \param[in]    none
 * \param[out]   none
 * \return       bool the result
 * \retval
 * \retval
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
void _deal_audible_sysfile(void)
{
    //��ȡͬ��������sys�ļ�
    _get_audible_sysfile();

    //��ȡС��������豸ID
    _get_firmware_info();
}
