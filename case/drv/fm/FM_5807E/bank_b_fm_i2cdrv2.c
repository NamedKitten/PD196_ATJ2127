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
extern const uint8 InitBuffer_SP[104];
extern const uint8 InitBuffer_HP[110];
extern const uint8 InitBuffer_NP[64];
extern uint16 signal_stg; //�����㣬��̨�ź�ǿ����Ϣ
extern uint8 ST_info; //������:   ��̨��������Ϣ  0~~������   1~~������
extern uint8 hardseek_flag;
extern uint8 WriteNum;
extern uint8 RDA_model; //ģ���ͺ�:    1  RDA 5807SP      2   RDA 5807HP    3  RDA 5807P


int FM_Standby_Drv(uint8 StandbyFlag);

extern int WriteRegister(uint8 count)
__FAR__;//д�Ĵ���
extern uint8 ReadRegister(uint8 count)
__FAR__; //���Ĵ���
extern uint16 FreqToChan(uint16 freq, uint8 mode)
__FAR__;

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
    uint16 ChipID;
    uint8 TH_Slevel;

    freq = FreqToChan(freq, Set_Band);

    ReadRegister(10);
    ChipID = ((uint16) ReadBuffer[8] << 8) + ReadBuffer[9];

    if (0x5804 == ChipID) //5807SP
    {
        libc_memcpy(&WriteBuffer[1], InitBuffer_SP, sizeof(InitBuffer_SP));
        WriteNum = sizeof(InitBuffer_NP);
        WriteBuffer[7] = 0x80; //8
        WriteBuffer[7] |= level; //��̨����
        RDA_model = 1;
    }
    else if (0x5801 == ChipID) //5807HP
    {
        libc_memcpy(&WriteBuffer[1], InitBuffer_HP, sizeof(InitBuffer_HP));
        WriteNum = sizeof(InitBuffer_NP);
        TH_Slevel = level >> 1;
        WriteBuffer[67] = TH_Slevel & 0x3f;
        TH_Slevel = level << 7;
        WriteBuffer[68] = TH_Slevel + level; //��̨����
        RDA_model = 2;
    }
    else //5807P
    {
        libc_memcpy(&WriteBuffer[1], InitBuffer_NP, sizeof(InitBuffer_NP));
        WriteNum = sizeof(InitBuffer_NP);
        WriteBuffer[7] = 0x80; //8
        WriteBuffer[7] |= level; //��̨����
        RDA_model = 3;
    }

    WriteBuffer[4] &= (uint8) 0xf0;
    WriteBuffer[4] |= Set_Band; //����Ƶ��

    WriteBuffer[1] &= (uint8) 0xfe; //��ֹSEEK ,ע��Ӳ��SEEK��ʽ,������Ƶ��\���� ʱ,����disable seek!!!!
    WriteBuffer[2] |= (0x01 << 0); //Enable FM
    WriteBuffer[3] = (uint8)(freq >> 8); //����Ҫд���Ƶ��
    WriteBuffer[4] &= 0x3f;
    WriteBuffer[4] |= (uint8)(freq % 256);
    WriteBuffer[4] |= (0x01 << 4); //enable tune

    return TRUE;
}
/*
 ***** ***************************************************************************
 * int  sFM_Init(uint8 band, uint8 level, void* null3)
 *
 * Description : FM��ʼ��,������������,����,������
 *
 * Arguments : ����ѡ��ֵ,����
 *                   band=0: �й�/������̨
 *                   band=1: �ձ���̨
 *                   band=2: ŷ�޵�̨
 * Returns : ����ʼ���ɹ�,�򷵻�1,����,����0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_Init(uint8 band, uint8 level, uint16 freq)
{
    int result;
    uint16 ChipID;
    uint8 TH_Slevel;

    if (band == 0)
    {
        //87~108, 100k
        Set_Band = 0;
    }
    else if (band == 1)
    {
        //76~91, 100k
        Set_Band = 0x04;
    }
    else
    {
        //87~108, 50k
        Set_Band = 0x02;
    }

    WriteBuffer[0] = 0x20; //д��ĵ�һ���ֽ�ΪFMģ���I2C������ַ
    WriteBuffer[1] = 0x00;
    WriteBuffer[2] = 0x02;

    if (freq != 0)
    {
        return sFM_Restore_Global_Data(band, level, freq);
    }
    WriteRegister(2); //FM reset
    sys_mdelay(10);

    ReadRegister(10);
    ChipID = ((uint16) ReadBuffer[8] << 8) + ReadBuffer[9];

    if (0x5804 == ChipID) //5807SP
    {
        libc_memcpy(&WriteBuffer[1], InitBuffer_SP, sizeof(InitBuffer_SP));
        WriteNum = sizeof(InitBuffer_SP);
        WriteBuffer[7] = 0x80; //8
        WriteBuffer[7] |= level; //��̨����
        RDA_model = 1;
    }
    else if (0x5801 == ChipID) //5807HP
    {
        libc_memcpy(&WriteBuffer[1], InitBuffer_HP, sizeof(InitBuffer_HP));
        WriteNum = sizeof(InitBuffer_HP);
        TH_Slevel = level >> 1;
        WriteBuffer[67] = TH_Slevel & 0x3f;
        TH_Slevel = level << 7;
        WriteBuffer[68] = TH_Slevel + level; //��̨����
        RDA_model = 2;
    }
    else //5807P
    {
        libc_memcpy(&WriteBuffer[1], InitBuffer_NP, sizeof(InitBuffer_NP));
        WriteNum = sizeof(InitBuffer_NP);
        WriteBuffer[7] = 0x80; //8
        WriteBuffer[7] |= level; //��̨����
        RDA_model = 3;
    }
    WriteBuffer[4] &= (uint8) 0xf0;
    WriteBuffer[4] |= Set_Band; //����Ƶ��

    result = FM_Standby_Drv(1);

    sys_mdelay(500); //��ʱ500MS���ȴ�FMģ���ȶ�
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
    result = FM_Standby_Drv(0);
    return result;
}

/*
 ************************************************* *******************************
 * int sFM_Mute(FM_MUTE_e mode, void* null2, void* null3)
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
    WriteBuffer[1] &= (uint8) 0xfe; //disable seek ,ע��Ӳ��SEE K��ʽ,������Ƶ��\����ʱ,����disable seek!!!!
    if (mode == releaseMUTE)
    {
        WriteBuffer[1] |= (0x01 << 6);
    }
    else
    {
        WriteBuffer[1] &= ~(0x01 << 6);
    }
    result = WriteRegister(2);
    return result;
}

/*
 ********************************************************************************
 *            int FM_Standby_Drv(uint8 StandbyFlag)
 *
 * Description : FM standby��wake up
 *
 * Arguments : standby��־,1��ʾwake up,0��ʾstandby
 *
 * Returns : �� standby��wake up�ɹ�������1;����,����0
 *
 * Notes :
 *
 ********************************************************************************
 */
int FM_Standby_Drv(uint8 StandbyFlag)
{
    int result;
    WriteBuffer[2] &= (uint8) 0xfe; //FM disable
    if (StandbyFlag != 0)
    {
        WriteBuffer[2] |= (0x01 << 0); //FM enable
    }
    result = WriteRegister(WriteNum);
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_SetBand(uint8 band, void* null2, void* null3)
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
    if (band == Band_China_US)
    {
        Set_Band = 0;
    }
    else if (band == Band_Japan)
    {
        Set_Band = 0x04;
    }
    else
    {
        Set_Band = 0x02;
    }

    WriteBuffer[4] &= (uint8) 0xf0;
    WriteBuffer[4] |= Set_Band; //����Ƶ��, ͬʱ������space
    result = WriteRegister(4);
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
    uint8 TH_Slevel, num;

    if (RDA_model == 2)
    {
        //5807HP
        TH_Slevel = level >> 1;
        WriteBuffer[67] = TH_Slevel & 0x3f;
        TH_Slevel = level << 7;
        WriteBuffer[68] = TH_Slevel + level; //��̨����
        num = 68;
    }
    else
    {
        //5807SP or 5807P
        WriteBuffer[7] = 0x80;
        WriteBuffer[7] |= level; //��̨����
        num = 7;
    }
    result = WriteRegister(num);
    return result;
}

