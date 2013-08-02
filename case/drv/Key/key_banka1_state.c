/*******************************************************************************
 *                              US212A
 *                            Module: Key driver
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       reagan     2011-9-2 14:45    1.0             build this file
 *******************************************************************************/
/*!
 * \file     key_state.c
 * \brief    Get hold state and key map address
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

#include "key_inner.h"



/******************************************************************************/
/*!
 * \par  Description:
 *    �������ȿ������
 * \param[in]    onoff 1��ʾ���ã��������γ�ʱ�����ȷ�����0��ʾ������
 * \param[out]   uint8 ���óɹ����0��ʾ�ɹ���1��ʾʧ�ܡ�
 * \ingroup      key_settings
 * \par          exmaple code
 * \code
 * //�������ȿ���
 * key_speakcheck(1);
 *
 * //�������Ȳ�����
 * key_speakcheck(0);
 * \endcode
 * \note
 *******************************************************************************/
uint8 *key_inner_speakcheck(uint8 onoff, void* null1, void* null2)
{
    g_speakonoff = onoff ;
    if(g_speakonoff == 0)
    {
        act_writel(act_readl(GPIO_ADAT) &0xfffffffd, GPIO_ADAT);//enable speak control mute
    }
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ����ӳ����ַ��
 * \param[in]    none
 * \param[out]   uint8* ����ӳ����ַ���ñ��ڰ���������פ���ݿռ��С�
 * \ingroup      key_infor
 * \par          exmaple code
 * \code
 *  //��ȡ����ӳ����ַ��Ȼ������û����Ի�ӳ���
 *  key_table_addr = key_getkeytabaddress();
 *  libc_memcpy(key_table_addr, g_config_var.keyID, sizeof(g_config_var.keyID));
 * \endcode
 * \note
 *******************************************************************************/
uint8 *key_inner_getkeytabaddress(void* null0, void* null1, void* null2)
{
    return key_map;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ����hold����״̬��
 * \param[in]    none
 * \param[out]   uint8
 * \return       the result
 * \retval           0 holdû����ס
 * \retval           1 hold��ס
 * \ingroup      key_infor
 * \par          exmaple code
 * \code
 *  //��ȡhold����״̬��У��Ӧ��hold״̬
 *  hold_state = key_holdcheck();
 * \endcode
 * \note
 *******************************************************************************/
uint8 key_inner_holdcheck(void* null0, void* null1, void* null2)
{
    unsigned char key_val;
    unsigned char ret;

    key_val = act_readb(LRADC1_DATA) & 0x3f;
    if ((key_val < MIN_HOLD_ADC) || (key_val > MAX_HOLD_ADC))
    {
        ret = 0; //unhold
    }
    else
    {
        ret = 1; //hold
    }

    return ret;
}

