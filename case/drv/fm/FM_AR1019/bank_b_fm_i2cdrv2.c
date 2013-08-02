/*
 ******************************************************************************
 *                               US212A
 *                            Module: Radio
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: main module of radio
 * Module:  AR1019/AR1010
 * History:
 *      <author>    <time>           <version >             <desc>
 *       Mikeyang    2010-11-25 14:35     1.0             build this file
 ******************************************************************************
 */
#include "fm_drv.h"

extern const uint8 InitBuffer[36];
extern uint8 WriteBuffer[];
/* CHAN<9:0>*/
extern uint16 Freq_RF;
extern uint8 ReadBuffer[];
extern uint8 Set_Band;
extern uint8 Space50k;

extern uint8 WriteOneFrame(uint8 Addr, uint8 Count)
__FAR__;
extern uint8 ReadOneFrame(uint8 Addr)
__FAR__;
extern uint8 WaitSTC(void)
__FAR__;
extern uint8 WriteRegister(uint8 Addr, uint8 Count)
__FAR__;
extern uint8 ReadRegister(uint8 RegAddr)
__FAR__;

#if 1
extern uint8 CheckStation(void)
__FAR__;
#endif

uint16 FreqToChan(uint16 freq, uint8 space);
uint16 ChanToFreq(uint16 chan, uint8 space);

/*
 ***** ***************************************************************************
 * int sFM_Restore_Global_Data(uint8 band, uint8 level, uint16 freq)
 *
 * Description : ���»ָ�FM��������Ҫ��ȫ������WriteBuffer������
 *
 * Arguments : ����ѡ��ֵ,����
 *                   band=0: �й�/������̨������100KHz
 *                   band=1: �ձ���̨������100KHz
 *                   band=2: ŷ�޵�̨������50KHz
 *
 * Returns : ����ʼ���ɹ�,�򷵻�1,����,����0
 *
 * Notes :
 *         ����FM������¼��codec��ռ�õĿռ���һ�µģ����FM¼��ʱFM�������뱻���ǣ�
 *         ���˳�FM����ʱ����Ҫ���»ָ�WriteBuffer�����ݣ��Ա������ȷ���������ģ
 *         ��ļĴ���ֵ
 *
 ********************************************************************************
 */
static int sFM_Restore_Global_Data(uint8 band, uint8 level, uint16 freq)
{
    uint8 chan_bit0;
    uint16 freq_tmp;

    band = band;
    level = level;

    freq = FreqToChan(freq, Space50k);

    chan_bit0 = (uint8)(freq % 2);
    freq_tmp = (freq >> 1); // CHAN<9:1>

    WriteBuffer[1] |= (0x01 << 0); //FM power up, Enable

    WriteBuffer[4] &= (uint8) 0xfd; //clear tune
    WriteBuffer[6] &= (uint8) ~(0x01 << 6); //clear SEEK

    //set band/space/chan,enable tune
    WriteBuffer[4] &= (uint8) 0xf6;
    WriteBuffer[4] |= (chan_bit0 << 3);
    WriteBuffer[4] |= (uint8)(freq_tmp >> 8);
    WriteBuffer[5] = (uint8)(freq_tmp % 256);

    return TRUE;

}

/*
 ***** ***************************************************************************
 * int  sFM_Init(uint8 band, uint8 level, void* null3)
 *
 * Description : FM��ʼ��,������������,����,������
 *
 * Arguments : ����ѡ��ֵ,����
 *                   band=0: �й�/������̨������100KHz
 *                   band=1: �ձ���̨������100KHz
 *                   band=2: ŷ�޵�̨������50KHz
 *
 * Returns : ����ʼ���ɹ�,�򷵻�1,����,����0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_Init(uint8 band, uint8 level, uint16 freq)
{
    int result;
    libc_memcpy(WriteBuffer, InitBuffer, sizeof(InitBuffer));
    Space50k = 0;
    if (band == 1)
    {
        /* 76MHz~90MHz*/
        Set_Band = 0x10;
    }
    else
    {
        /*87.5MHz~108MHz*/
        Set_Band = 0;
        if (band == 2)
        {
            /* ŷ�޵�̨������50k */
            Space50k = 0x01;
        }
    }

    WriteBuffer[6] &= (uint8) ~(0x01 << 6); //��ʼ��,disable SEEK

    WriteBuffer[6] &= (uint8) 0xe7;
    WriteBuffer[6] |= Set_Band; //����Ƶ��
    WriteBuffer[30] &= (uint8) 0xfe;
    WriteBuffer[30] |= Space50k; //���ò���

    WriteBuffer[7] &= (uint8) 0x80;
    WriteBuffer[7] |= level; //��̨����<6:0>

    WriteBuffer[1] &= ~(0x01 << 0); //Enable bit = 0

    if (freq != 0)
    {
        return sFM_Restore_Global_Data(band, level, freq);
    }

    result = WriteRegister(0x00, 0x02); //FM standby  �� д2 ���ֽڼĴ������ݣ�ֻ������FM_Enable
    if (result != 0)
    {
        result = WriteRegister(0x01, 34); //init, from R1 to R17
        if (result != 0)
        {
            WriteBuffer[1] |= (0x01 << 0); //FM power up, Enable
            result = WriteRegister(0x00, 2);
        }
    }
    if (result != 0)
    {
        result = WaitSTC(); //wait STC
    }
    return result;
}

/*
 ********************************************************************************
 *             int sFM_Standby(void* null1, void* null2, void* null3)
 *
 * Description : FM standby
 *
 * Arguments : NULL
 *
 * Returns : ������standby�ɹ�,�򷵻�1,����,����0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_Standby(void* null1, void* null2, void* null3)
{
    int result;
    WriteBuffer[1] &= ~(0x01 << 0); //FM Enable bit = 0
    result = WriteRegister(0x00, 0x02);
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_Mute(FM_MUTE_e mode, void* null2, void* null3)
 *
 * Description : FM��������
 *
 * Arguments : �Ƿ���,0Ϊȡ������,1Ϊ����
 *
 * Returns : �����þ������Ƴɹ�,�򷵻�1,����,����0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_Mute(FM_MUTE_e mode, void* null2, void* null3)
{
    int result;
    if (mode == releaseMUTE)
    {
        WriteBuffer[3] &= ~(0x01 << 1); //release mute
    }
    else
    {
        WriteBuffer[3] |= (0x01 << 1); //enable mute
    }
    result = WriteRegister(0x01, 0x02);
    return result;
}

/*
 ****************************************************************************************
 *           uint16 FreqToChan(uint16 freq, uint8 space)
 *
 * Description : ��Ƶ��ת����д�Ĵ�����ֵ
 *
 * Arguments :  freq:  �����ת����Ƶ��( ʵ��Ƶ��khz Ϊ��λ�ĵ�2 �ֽ�)
 *			   space:  ����   0: 100K    1: 50K
 * Returns :	   CHAN<9:0>
 *
 * Notes :  ����Ƶ��ֵ( ��1khz Ϊ��λ)  �����bit  ��Ϊ1
 *
 * space 50K    CHAN<9:0> = FREQ ( ��50K Ϊ��λ)  -  69*20
 * else             CHAN<9:1> = FREQ ( ��100K Ϊ��λ) - 69*10
 *****************************************************************************************
 */
uint16 FreqToChan(uint16 freq, uint8 space)
{
    uint16 Chan, tmp_freq;
    uint32 temp;

    //ʵ��Ƶ��ֵ����khz Ϊ��λ
    temp = (uint32) freq + 0x10000;

    //����100K
    if (space == 0)
    {
        tmp_freq = (uint16)(temp / 100);
        //CHAN<9:1>
        Chan = tmp_freq - 69* 10 ;
        //CHAN<9:0>
        Chan = Chan*2;
    }
    //����50K

    else
    {
        tmp_freq = (uint16)(temp/50);
        //CHAN<9:0>
        Chan = tmp_freq - 69*20;
    }

    return Chan;
}

/*
 ********************************************************************************
 *           uint16 ChanToFreq(uint16 chan, uint8 space)
 *
 * Description : �������ļĴ���Ƶ��ֵת�����ϲ���Ҫ��Ƶ��
 *
 * Arguments :  chan:������Ƶ��ֵ CHAN<9:0>
 *                    space :  ����
 *
 * Returns :	   ת��������ʵ��Ƶ��ֵ(��1KHz Ϊ��λ�ĵ�2   �ֽ�)
 *
 * Notes :
 *
 ********************************************************************************
 */
uint16 ChanToFreq(uint16 chan, uint8 space)
{
    uint16 Freq;
    uint32 temp;

    //����100K  ��FREQ ( ��100K Ϊ��λ) = CHAN<9:1> +  69*10
    if (space == 0)
    {
        Freq = chan / 2 + 69* 10 ;
        //ת��Ϊ��1khz  Ϊ��λ
        temp = (uint32)Freq * 100;
    }
    //����50K �� FREQ ( ��50K Ϊ��λ) = CHAN<9:0> +  69*20

    else
    {
        Freq = chan + 69*20;
        //ת��Ϊ��1khz  Ϊ��λ
        temp = (uint32)Freq * 50;
    }

    //���λ��Ϊ1
    Freq = (uint16)(temp & 0x0ffff);
    return Freq;
}

/*
 ************************************************* *******************************
 * int  sFM_SetBand(radio_band_e band, void* null2, void* null3)
 *
 * Description : FM  ��������
 *
 * Arguments : band    0:  �й�/ �������� 1: �ձ�����  2: ŷ�޲���
 *
 * Returns : ���óɹ�������1;  ���򣬷���0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_SetBand(radio_band_e band, void* null2, void* null3)
{
    int result;
    uint8 space_bak;

    space_bak = Space50k;
    Space50k = 0;

    if (band == Band_Japan)
    {
        Set_Band = 0x10;
    }
    else
    {
        Set_Band = 0;
        if (band == Band_Europe)
        {
            Space50k = 1;
        }
    }

    WriteBuffer[6] &= (uint8) 0xe7;
    WriteBuffer[6] |= Set_Band; //����Ƶ��
    WriteBuffer[30] &= (uint8) 0xfe;
    WriteBuffer[30] |= Space50k; //���ò���

    result = WriteRegister(0x03, 2);
    result = WriteRegister(0x0f, 2);
    if (result == 0)
    {
        //����Ƶ��ʧ�ܣ��ָ�space
        Space50k = space_bak;
    }
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_SetThrod(uint8 level, void* null2, void* null3)
 *
 * Description : FM  ��̨��������
 *
 * Arguments : level---��̨����ֵ
 *
 * Returns : ���óɹ�������1;  ���򣬷���0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_SetThrod(uint8 level, void* null2, void* null3)
{
    int result;

    WriteBuffer[7] &= (uint8) 0x80;
    WriteBuffer[7] |= level; //��̨����
    result = WriteRegister(0x03, 2);
    return result;
}


#if 0
/*
 ********************************************************************************
 *            int  sFM_Search(uint16 freq,  uint8 direct, void* null3)
 *
 * Description : FM  �����̨��tune ��Ƶ�㣬�ж��Ƿ���Ч��̨
 *
 * Arguments :  �����̨��ʽ,�����õ�Ƶ��ֵ����������
 *
 * Returns : ��̨Ϊ��̨,�򷵻�1;����,����0
 *
 * Notes :
 *
 ********************************************************************************
 */
//�İ�ǰ�������̨�����ݷ�bank
int sFM_Search(uint16 freq, uint8 direct, void* null3)
{
    int result;
    uint16 TempFreq;
    uint16 freq_tmp;
    uint8 chan_bit0;
    uint8 ret;

    //�л���single_seek ģʽ
    WriteBuffer[28] |= (uint8) 0x02;
    ret = WriteRegister(0x0E, 0x02);

    sFM_Mute(SetMUTE, 0, 0);

    if (direct == 0)
    {
        //seek down
        if (Space50k == 0x01)
        {
            freq_tmp = freq + 50;
        }
        else
        {
            freq_tmp = freq + 100;
        }
    }
    else
    {
        //seek up
        if (Space50k == 0x01)
        {
            freq_tmp = freq - 50;
        }
        else
        {
            freq_tmp = freq - 100;
        }
    }
    TempFreq = FreqToChan(freq_tmp, Space50k); //����̨Ƶ��ת���ɼĴ�����д���ֵ
    chan_bit0 = (uint8)(TempFreq % 2);
    freq_tmp = (TempFreq >> 1); // CHAN<9:1>

    WriteBuffer[4] &= (uint8) 0xf4; //disable tune
    WriteBuffer[4] |= (chan_bit0 << 3);
    WriteBuffer[4] |= (uint8)(freq_tmp >> 8);
    WriteBuffer[5] = (uint8)(freq_tmp % 256); //update chan
    ret = WriteRegister(0x02, 0x02);

    WriteBuffer[6] &= (uint8) ~(0x01 << 6); //disable seek
    ret = WriteRegister(0x03, 0x02);

    WriteBuffer[6] &= (uint8) 0x7f; //����SEEK direct
    WriteBuffer[6] |= (uint8)(direct << 7); //����SEEK direct
    WriteBuffer[6] |= (uint8)(0x01 << 6); //enable SEEK
    ret = WriteRegister(0x03, 0x02);

    //�Ѿ�����single_seek
    if (ret != 0)
    {
        //�����̨���ж�������Ƶ���Ƿ���Ч��̨
        if (CheckStation() == 1) // check station
        {
            //0x17700~~96MHz, ����96MHz Ƶ��
            if (freq != 0x7700)
            {
                result = 1;
            }
            else
            {
                result = 0;
            }
        }
        else
        {
            result = 0;
        }

    }
    else
    {
        result = 0;
    }

    //�л�������ģʽ
    WriteBuffer[28] &= (uint8) 0xfd;
    ret = WriteRegister(0x0E, 0x02);
    return result;
}
#endif

