/*
 ******************************************************************************
 *                               US212A
 *                            Module: Radio
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: main module of radio
 * Module:  RDA5807E
 * History:
 *      <author>    <time>           <version >             <desc>
 *       Mikeyang    2010-11-25 14:35     1.0             build this file
 ******************************************************************************
 */
#include "fm_drv.h"

extern uint8 WriteBuffer[];
extern uint8 ReadBuffer[];
extern uint8 Set_Band;
extern uint16 Freq_RF;
extern uint16 signal_stg; //�����㣬��̨�ź�ǿ����Ϣ
extern uint8 ST_info; //������:   ��̨��������Ϣ  0~~������   1~~������
extern uint8 hardseek_flag;

extern uint16 FreqToChan(uint16 freq, uint8 mode)
__FAR__;
extern uint16 ChanToFreq(uint16 Chan, uint8 mode)
__FAR__;
extern void DisAssemble_WriteBuffer(void)
__FAR__;
extern int WriteRegister(uint8 count)
__FAR__;//д�Ĵ���
extern uint8 ReadRegister(uint8 count)
__FAR__; //���Ĵ���

/*
 ************************************************* *******************************
 * int  sFM_GetBand(void* band, void* null2, void* null3)
 *
 * Description : ��ȡ��ǰ������Ϣ
 *
 * Arguments : band      ��ŷ�����Ϣ�ĵ�ַ
 *
 * Returns : ��ȡ�ɹ�������1;  ���򣬷���0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_GetBand(void* band, void* null2, void* null3)
{
    radio_band_e* bandinfo;
    int result = 1;

    bandinfo = (radio_band_e*) band;

    //����ȥȡ��ֱ�Ӵ�������ȫ�ֱ�����ȡ��
    //�й�/ ������̨
    if (Set_Band == 0)
    {
        *bandinfo = Band_China_US;
    }
    //�ձ���̨
    else if (Set_Band == 0x04)
    {
        *bandinfo = Band_Japan;
    }
    //ŷ�޵�̨
    else if (Set_Band == 0x02)
    {
        *bandinfo = Band_Europe;
    }
    else
    {
        result = 0;
    }
    return result;

}

/*
 ************************************************* *******************************
 * int  sFM_GetFreq(void* freq, void* null2, void* null3)
 *
 * Description : ��ȡ��ǰƵ����Ϣ
 *
 * Arguments : freq      ��ŷ�����Ϣ�ĵ�ַ
 *
 * Returns : ��ȡ�ɹ�������1;  ���򣬷���0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_GetFreq(void* freq, void* null2, void* null3)
{
    int result;
    uint16* pfreq;

    pfreq = (uint16*) freq;
    result = ReadRegister(10);
    if (result == 1)
    {
        //��ȡ�ɹ�������ת��
        DisAssemble_WriteBuffer();
        *pfreq = ChanToFreq(Freq_RF, Set_Band);
    }
    else
    {
        //��ȡʧ��
        result = 0;
    }
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_GetIntsity(void* intensity, void* null2, void* null3)
 *
 * Description : ��ȡ��ǰ��̨�ź�ǿ��
 *
 * Arguments : intensity      ��ŷ�����Ϣ�ĵ�ַ
 *
 * Returns : ��ȡ�ɹ�������1;  ���򣬷���0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_GetIntsity(void* intensity, void* null2, void* null3)
{
    int result;
    uint16* p_intensity;

    p_intensity = (uint16*) intensity;
    result = ReadRegister(4);
    if (result == 1)
    {
        DisAssemble_WriteBuffer();
        *p_intensity = signal_stg;
    }
    else
    {
        result = 0;
    }
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_GetAnten(void* antenna, void* null2, void* null3)
 *
 * Description : ��ȡ��ǰ����״̬
 *
 * Arguments : antenna      ��ŷ�����Ϣ�ĵ�ַ
 *
 * Returns : ��ȡ�ɹ�������1;  ���򣬷���0
 *
 * Notes :  ����״̬�����Ӳ����أ��������
 *
 ********************************************************************************
 */
int sFM_GetAnten(void* antenna, void* null2, void* null3)
{
    int result;
    uint8* p_antenna;
    uint32 value;

    p_antenna = (uint8*) antenna;
    //���������ϣ�EVB ��GPIO_A1 ����������
    act_writel((act_readl(GPIO_AOUTEN) & (0xfffffffd)), GPIO_AOUTEN);
    act_writel((act_readl(GPIO_AINEN) | (0x00000002)), GPIO_AINEN);
    sys_mdelay(20);
    value = act_readl(GPIO_ADAT) & (0x00000002);
    if (value == 0)
    {
        *p_antenna = 1;
    }
    else
    {
        *p_antenna = 0;
    }
    //��ȡ�ɹ�
    result = 1;
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_GetStereo(void* stereo, void* null2, void* null3)
 *
 * Description : ��ȡ��ǰ��̨��������Ϣ
 *
 * Arguments : stereo      ��ŷ�����Ϣ�ĵ�ַ
 *
 * Returns : ��ȡ�ɹ�������1;  ���򣬷���0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_GetStereo(void* stereo, void* null2, void* null3)
{
    int result;
    FM_Audio_e* p_stereo;

    p_stereo = (FM_Audio_e*) stereo;
    result = ReadRegister(4);
    if (result == 1)
    {
        DisAssemble_WriteBuffer();
        *p_stereo = (FM_Audio_e) ST_info;
    }
    else
    {
        result = 0;
    }
    return result;
}

