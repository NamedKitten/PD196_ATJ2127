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
 *       mikeyang    2012-1-25  14:35     1.0             build this file
 ******************************************************************************
 */
#include "fm_drv.h"

/* ʵ�ʴ�������buffer */
uint8 TransBuffer[38];
/* дbuffer */
uint8 WriteBuffer[36];
/* ��buffer */
uint8 ReadBuffer[2];

//CLK 24M
/***********************************************************
 *Description: R0~R17
 * using external reference clock
 * 75-us de-emphasis,  disable smute & hmute
 * freq 87.5MHz,
 * seek up,  seek disable, space 100k, US/Europe band, seekth 16
 * volume control in R3&R14,
 * CLK  24MHz
 * seek_wrap:  no wrap
 ************************************************************/
const uint8 InitBuffer[36] =
{ 0xFF, 0x7B, 0x5B, 0x11, 0xD4, 0xB9, 0xA0, 0x10, 0x07, 0x80, 0x28, 0xAB, 0x64, 0x00, 0x1f, 0x87, 0x71, 0x41, 0x00,
        0x80, 0x81, 0xc6, 0x4F, 0x55, 0x97, 0x0c, 0xb8, 0x45, 0xfc, 0x2d, 0x80, 0x97, 0x04, 0xE1, 0xDF, 0x6A
};

//CLK 32k	,disable seek wrap
/***********************************************************
 *Description: R0~R17
 * using external reference clock
 * 75-us de-emphasis,  disable smute & hmute
 * freq 87.5MHz,
 * seek up,  seek disable, space 100k, US/Europe band, seekth 16
 * volume control in R3&R14,
 * CLK  32.768KHz
 * seek_wrap:  no wrap
 ************************************************************/
/*const uint8 InitBuffer[36]={0xFF,0x7B,0x5B,0x11,0xD4,0xB9,0xA0,0x10,0x07,0x80,0x28,0xAB,0x64,0x00,0x1e,0xe7,0x71,0x41,
 0x00,0x7d,0x82,0xc6,0x4F,0x55,0x97,0x0c,0xb8,0x45,0xfc,0x2d,0x80,0x97,0x04,0xA1,0xDF,0x6A};*/

/* CHAN<9:0>*/
uint16 Freq_RF;
/* ��̨�ź�ǿ��*/
uint16 signal_stg;
/* ��ǰ������Ϣ*/
uint8 Set_Band;
/* �Ƿ����50K ����*/
uint8 Space50k;
/* ��̨RSSI */
uint8 RSSI2;
/* ��̨RSSI */
uint8 RSSI1;
/* ������:   ��̨��������Ϣ  0~~������   1~~������*/
uint8 ST_info;
/* Ӳ��seek ״̬��ʶ*/
uint8 hardseek_flag;

uint8 WriteOneFrame(uint8 Addr, uint8 Count);
uint8 ReadOneFrame(uint8 Addr);
uint8 WaitSTC(void);
uint8 CheckStation(void);
uint8 FM_SetFrequency(uint16 Freq);
uint8 WriteRegister(uint8 Addr, uint8 Count);
uint8 ReadRegister(uint8 RegAddr);
uint8 TuneControl(uint8 mode);

extern uint16 FreqToChan(uint16 freq, uint8 space)
__FAR__;
extern uint16 ChanToFreq(uint16 chan, uint8 space)
__FAR__;

/*
 ********************************************************************************
 *  int sFM_SetFreq(uint16 freq, void* null2, void* null3)
 *
 * Description : ����Ƶ��,����tune����
 *
 * Arguments : �����õ�Ƶ��ֵ( ʵ��Ƶ��khz Ϊ��λ�ĵ�2 �ֽ�)
 *
 * Returns :   �ɹ�:  ����1�� ʧ��:  ����0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_SetFreq(uint16 freq, void* null2, void* null3)
{
    int result;
    uint16 TempFreq;

    TempFreq = FreqToChan(freq, Space50k); //����̨Ƶ��ת���ɼĴ�����д���ֵ
    result = FM_SetFrequency(TempFreq);
    return result;
}

/*
 ********************************************************************************
 * int sFM_GetStatus(void * pstruct_buf, uint8 mode, void* null3)
 *
 * Description : ��ȡ��ǰ��̨�������Ϣ��������ǰ��̨Ƶ��
 *                   �ź�ǿ��ֵ��������״̬����ǰ����
 *
 * Arguments : pstruct_buf   �����̨��Ϣ�Ľṹ��ָ��
 *                   mode==0,  ��������( ��seek ������)  ȡ��Ϣ
 *                   mode==1,  ��Ӳ��seek ������ȡ��Ϣ
 * Returns : //�Ƿ��ȡ״̬�ɹ�,�����ȡ�ɹ�,�򷵻�ֵΪ1
 * ����,����0
 * Notes :
 *
 ********************************************************************************
 */
int sFM_GetStatus(void * pstruct_buf, uint8 mode, void* null3)
{
    int result;

    Engine_Status_t * ptr_buf = (Engine_Status_t *)pstruct_buf;
    uint8 tmp;

    //Ӳ��seek �����У�ֱ��ȡ�Ĵ�����Ϣ
    if(mode == 1)
    {
        //read status reg
        result = ReadRegister(0x13);
    }
    //��Ӳ��seek �����У���ȴ�STC

    else
    {
        result = WaitSTC();
    }

    if(result == 1)
    {
        Freq_RF = (uint16)ReadBuffer[0] * 4;
        tmp = (uint8)((ReadBuffer[1] & 0x80)>>6);
        tmp +=(ReadBuffer[1] & 0x01);
        //CHAN<9:0>
        Freq_RF += tmp;
        ptr_buf->FM_CurrentFreq = ChanToFreq(Freq_RF, Space50k);

        tmp = ReadBuffer[1] & 0x08;
        if(tmp==0)
        {
            //mono
            ST_info = 1;
        }
        else
        {
            //stereo
            ST_info = 0;
        }
        ptr_buf->FM_Stereo_Status = (FM_Audio_e)ST_info;

        //STC Flag
        hardseek_flag = (ReadBuffer[1] & 0x20)>>5;
        if(mode!=1)
        {
            hardseek_flag = 1;
        }
        if(hardseek_flag==0)
        {
            ptr_buf->STC_flag = HARDSEEK_DOING;
        }
        else
        {
            ptr_buf->STC_flag = HARDSEEK_COMPLETE;
        }

        if(Set_Band == 0x10)
        {
            ptr_buf->FM_CurBand = Band_Japan;
        }
        else
        {
            if(Space50k == 0x01)
            {
                ptr_buf->FM_CurBand = Band_Europe;
            }
            else
            {
                ptr_buf->FM_CurBand = Band_China_US;
            }
        }

        result = ReadRegister(0x12);
        signal_stg = (uint16)((ReadBuffer[0] & 0xfe)>>1);
        ptr_buf->FM_SignalStg = signal_stg;
        result =1;
    }
    else
    {
        result = 0;
    }

    return result;
}

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
int sFM_Search(uint16 freq, uint8 direct, void* null3)
{
    int result;
    uint16 TempFreq;
    uint16 freq_tmp;
    uint8 chan_bit0;
    uint8 ret;

    //�л���single_seek ģʽ
    WriteBuffer[28] |= 0x02;
    ret=WriteRegister(0x0E,0x02);

    sFM_Mute(SetMUTE, 0, 0);

    if(direct == 0)
    {
        //seek down
        if(Space50k == 0x01)
        {
            freq_tmp = freq +50;
        }
        else
        {
            freq_tmp = freq +100;
        }
    }
    else
    {
        //seek up
        if(Space50k == 0x01)
        {
            freq_tmp = freq -50;
        }
        else
        {
            freq_tmp = freq -100;
        }
    }
    TempFreq = FreqToChan(freq_tmp, Space50k); //����̨Ƶ��ת���ɼĴ�����д���ֵ
    chan_bit0 = (uint8)(TempFreq%2);
    freq_tmp = (TempFreq>>1); // CHAN<9:1>

    WriteBuffer[4] &= (uint8)0xf4; //disable tune
    WriteBuffer[4] |= (chan_bit0<<3);
    WriteBuffer[4] |= (uint8)(freq_tmp>>8);
    WriteBuffer[5] = (uint8)(freq_tmp%256); //update chan
    ret=WriteRegister(0x02,0x02);

    WriteBuffer[6] &= (uint8)~(0x01<<6); //disable seek
    ret=WriteRegister(0x03,0x02);

    WriteBuffer[6] &= 0x7f; //����SEEK direct
    WriteBuffer[6] |= (uint8)(direct<<7); //����SEEK direct
    WriteBuffer[6] |= (uint8)(0x01<<6); //enable SEEK
    ret=WriteRegister(0x03,0x02);

    //�Ѿ�����single_seek
    if (ret != 0)
    {
        //�����̨���ж�������Ƶ���Ƿ���Ч��̨
        if (CheckStation() == 1) // check station

        {
            //0x17700~~96MHz, ����96MHz Ƶ��
            if( freq !=0x7700)
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
    WriteBuffer[28] &= (uint8)0xfd;
    ret=WriteRegister(0x0E,0x02);
    return result;
}


/*
 ********************************************************************************
 *            int sFM_HardSeek(uint16 freq, uint8 direct, void* null3)
 *
 * Description : ����FM Ӳ����̨����
 *
 * Arguments :
 param1: Ӳ����̨��ʽ����̨��ʼƵ��
 param2: ��������
 * Returns :  �ɹ�����Ӳ����̨������1�����򷵻�0
 *
 * Notes :  ��̨����˵��
 * bit0   ~~~  ���ϻ�������̨��0:   UP;    1: DOWN
 * bit1   ~~~  �Ƿ�߽���ơ�     0:   ����;    1:  ������
 ********************************************************************************
 */
int sFM_HardSeek(uint16 freq, uint8 direct, void* null3)
{
    int result;
    uint8 temp;
    uint16 TempFreq;
    uint8 chan_bit0;
    uint16 freq_tmp;

    //Ĭ��Ӳ��seek  ��δ����
    hardseek_flag = 1;

    //tune ����ʼƵ��
    TempFreq = FreqToChan(freq, Space50k); //����̨Ƶ��ת���ɼĴ�����д���ֵ
    chan_bit0 = (uint8)(TempFreq % 2);
    freq_tmp = (TempFreq >> (uint8) 1); // CHAN<9:1>

    WriteBuffer[4] &= (uint8) 0xf4; //disable tune
    WriteBuffer[4] |= (chan_bit0 << 3);
    WriteBuffer[4] |= (uint8)(freq_tmp >> 8);
    WriteBuffer[5] = (uint8)(freq_tmp % 256); //update chan
    result = WriteRegister(0x02, 0x02);

    WriteBuffer[6] &= (uint8) ~(0x01 << 6); //disable seek
    result = WriteRegister(0x03, 0x02);

    if (result != 0)
    {
        temp = direct & 0x01;
        //seek down
        if (temp != 0)
        {
            //R3--bit15
            WriteBuffer[6] &= (uint8) 0x7f;
        }
        //seek up
        else
        {
            WriteBuffer[6] |= (uint8) 0x80;
        }

        temp = direct & 0x02;
        //������
        if (temp != 0)
        {
            //R10--bit3
            WriteBuffer[21] &= (uint8) 0xf7;
        }
        //����
        else
        {
            WriteBuffer[21] |= (uint8) 0x08;
        }
        WriteBuffer[1] |= 0x01;
        WriteBuffer[6] |= 0x40; //start seek
        result = WriteRegister(0x01, 20); //R1~R10
        if (result != 0)
        {
            //Ӳ��seek ��������δ���
            hardseek_flag = 0;
        }
    }
    else
    {
        result = 0;
    }

    return result;
}

/*
 ********************************************************************************
 *             uint8 CheckStation(void)
 *
 * Description : ��̨�ж�
 *
 * Arguments   :
 *
 * Returns     :  ���ѵ��ĵ�̨Ϊ��̨,����1;����,����0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 CheckStation(void)
{
    uint8 result;
    uint8 temp;
    uint8 tmp;

    result = WaitSTC(); //wait STC flag
    if (result != 0)
    {
        temp = ReadBuffer[1] & 0x10; // �жϵ���SF  ��һλ
        if (temp == 0)
        {
            Freq_RF = (uint16) ReadBuffer[0] * 4;
            tmp = (ReadBuffer[1] & (uint8) 0x80) >> 6;
            tmp += (ReadBuffer[1] & 0x01);
            //CHAN<9:0>
            Freq_RF += tmp;
            return 1;
        }
    }
    return 0;
}

/*
 ********************************************************************************
 *             uint8 FM_SetFrequency(uint16 Freq)
 *
 * Description : Ƶ������
 *
 * Arguments   :  �����õ�Ƶ��ֵ CHAN <9:0>
 *
 * Returns     :   ��Ƶ�����óɹ�������1;����,����0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 FM_SetFrequency(uint16 Freq)
{
    uint8 result;
    uint8 chan_bit0;
    uint16 freq_tmp;

    chan_bit0 = (uint8)(Freq % 2);
    freq_tmp = (Freq >> 1); // CHAN<9:1>
    WriteBuffer[4] &= (uint8) 0xfd; //clear tune
    WriteBuffer[6] &= (uint8) ~(0x01 << 6); //clear SEEK
    result = WriteRegister(0x02, 0x04);

    WriteBuffer[22] &= 0x7f; //Read Low-side LO Injection
    WriteBuffer[23] &= (uint8) 0xfa;
    result = WriteRegister(0x0b, 0x02);

    //set band/space/chan,enable tune
    WriteBuffer[4] &= (uint8) 0xf6;
    WriteBuffer[4] |= (chan_bit0 << 3);
    WriteBuffer[4] |= (uint8)(freq_tmp >> 8);
    WriteBuffer[5] = (uint8)(freq_tmp % 256);
    result = TuneControl(1); //start tune

    result = WaitSTC(); //wait STC flag
    result = ReadRegister(0x12); //get RSSI (RSSI1)
    RSSI1 = ReadBuffer[0];

    //clear tune
    result = TuneControl(0);

    WriteBuffer[22] |= (uint8) 0x80; //Read High-side LO Injection
    WriteBuffer[23] |= 0x05;
    result = WriteRegister(0x0b, 0x02);

    //enable tune
    result = TuneControl(1);

    result = WaitSTC();
    result = ReadRegister(0x12); //get RSSI (RSSI2)
    RSSI2 = ReadBuffer[0];

    //clear tune
    result = TuneControl(0);

    if (RSSI1 >= RSSI2) //Set D15,  Clear D0/D2
    {
        WriteBuffer[22] |= (uint8) 0x80;
        WriteBuffer[23] &= (uint8) 0xfa;
    }
    else //Clear D15, Set D0/D2
    {
        WriteBuffer[22] &= 0x7f;
        WriteBuffer[23] |= 0x05;
    }
    result = WriteRegister(0x0b, 0x02);

    //enable tune
    result = TuneControl(1);
    if (result != 0)
    {
        result = WaitSTC();
    }

    return result;
}

/*
 ********************************************************************************
 *             uint8 WaitSTC(void)
 *
 * Description : wait STC flag,tune or seek complete
 *
 * Arguments   :
 *
 * Returns     :  ��STC��־��1���򷵻�1�����򣬷���0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 WaitSTC(void)
{
    uint8 result;
    uint8 temp;
    uint8 count = 0;
    sys_mdelay(50); //��ʱ50ms���ȴ�TUNE OR SEEK ����
    do
    {
        CLEAR_WATCHDOG();
        result = ReadRegister(0x13); // read status reg
        temp = ReadBuffer[1] | (uint8) 0xdf; //STC
        if (temp == 0xff)
        {
            return 1; //STC =1
        }
        count++;
        sys_mdelay(20);
    } while (/*(temp != (uint8)0xff) && (count < 255)*/count < 255);
    return 0;
}

/*
 ********************************************************************************
 *           uint8 WriteOneFrame(uint8 Addr,uint8 Count)
 *
 * Description : ��ָ����ַ�ļĴ�����ʼ��дָ�������ֽڵ�
 *                    ���ݵ��Ĵ���
 *
 * Arguments :   Addr:  ָ����ַ�ļĴ���   R0(Addr=0), R1(Addr=1)...
 *                     Count:  дָ������
 *
 * Returns :  �ɹ�:  ����1��ʧ��:  ����0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 WriteOneFrame(uint8 Addr, uint8 Count)
{
    uint8 i, result;

    if ((Count > 36) || ((Count + 2* Addr ) > 36))
    {
        result = 0;
        return result;
    }
    //�ȷ��豸��ַ���ٷ����ĸ��Ĵ�����ַ��ʼд
    TransBuffer[0] = 0x20;
    TransBuffer[1] = Addr;
    //����ͳ���д����д�ļĴ�������
    for (i = 0; i < Count; i++)
    {
        //��register table ����һ��Addr  Ϊ�����ֽ�
        TransBuffer[2 + i] = WriteBuffer[i + 2* Addr ];
    }
    result = I2C_Trans_Bytes(TransBuffer, Count + 2, I2C_gpio, &delay_i2c);
    return result;

}

/*
 ********************************************************************************
 *           uint8 ReadOneFrame(uint8 RegAddr)
 *
 * Description : ��ָ����ַ�ļĴ�������
 *
 * Arguments :   RegAddr:  ָ����ַ�ļĴ���   R0(Addr=0), R1(Addr=1)...
 *
 *
 * Returns :  �ɹ�:  ����1��ʧ��:  ����0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 ReadOneFrame(uint8 RegAddr)
{
    uint8 result = 0;

    IIC_Init(I2C_gpio, &delay_i2c);

    IIC_Start(I2C_gpio, &delay_i2c);
    //д�豸��ַ
    IIC_WriteByte(0x20, I2C_gpio, &delay_i2c);
    if (IIC_GetAck(I2C_gpio, &delay_i2c) != 0)
    {
        //���ȡ�ļĴ�����ַ
        IIC_WriteByte(RegAddr, I2C_gpio, &delay_i2c);
        if (IIC_GetAck(I2C_gpio, &delay_i2c) != 0)
        {
            IIC_Start(I2C_gpio, &delay_i2c);
            //�豸��ַ~  ��
            IIC_WriteByte(0x21, I2C_gpio, &delay_i2c);
            if (IIC_GetAck(I2C_gpio, &delay_i2c) != 0)
            {
                ReadBuffer[0] = IIC_ReadByte(I2C_gpio, &delay_i2c);
                IIC_SendAck(0, I2C_gpio, &delay_i2c);
                ReadBuffer[1] = IIC_ReadByte(I2C_gpio, &delay_i2c);
                IIC_SendAck(1, I2C_gpio, &delay_i2c);
                result = 1;
            }
        }
    }
    IIC_Stop(I2C_gpio, &delay_i2c);

    return result;

}

/*
 ********************************************************************************
 *           void SwitchGPIO(void)
 *
 * Description : ��GPIO disable
 *
 * Arguments : NULL
 *
 * Returns :  NULL
 *
 * Notes : ʹ����I2C ģ��IO �ں�disable  �����������
 *            ��������GPIO , ����Ӱ������Ӳ��ģ��
 *
 ********************************************************************************
 */
void SwitchGPIO(void)
{
    IIC_SDA_Output_Dis(I2C_gpio);
    IIC_SDA_Input_Dis(I2C_gpio);
    IIC_SCL_Output_Dis(I2C_gpio);
    IIC_SCL_Input_Dis(I2C_gpio);
}

/*
 ********************************************************************************
 *           uint8 WriteRegister(uint8 Addr,uint8 Count)
 *
 * Description : дFM ģ��Ĵ���
 *
 * Arguments :   Addr:  �Ĵ�����ַ count:д�Ĵ����ĸ���
 *
 * Returns :  �ɹ�:  ����1��ʧ��:  ����0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 WriteRegister(uint8 Addr, uint8 Count)
{
    uint8 i;
    uint8 result;
    uint32 flags;
    uint8 ret;

    //���ж�
    flags = sys_local_irq_save();
    //�ɹ����أ����ɹ�����3   ��
    for (i = 0; i < 3; i++)
    {
        result = WriteOneFrame(Addr, Count);
        if (result != 0)
        {
            ret = 1;
            break;
        }
    }
    if (i == 3)
    {
        ret = 0;
    }
    SwitchGPIO();
    //���ж�
    sys_local_irq_restore(flags);
    return ret;
}

/*
 ********************************************************************************
 *           uint8 ReadRegister(uint8 RegAddr)
 *
 * Description : ��FM ģ���һ���Ĵ���
 *
 * Arguments :   RegAddr:  �Ĵ�����ַ
 *
 * Returns :  �ɹ�:  ����1��ʧ��:  ����0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 ReadRegister(uint8 RegAddr)
{
    uint8 i;
    uint8 result;
    uint8 ret;
    uint32 flags;
    //���ж�
    flags = sys_local_irq_save();
    //�ɹ����أ����ɹ�����3   ��
    for (i = 0; i < 3; i++)
    {
        result = ReadOneFrame(RegAddr);
        if (result != 0)
        {
            ret = 1;
            break;
        }
    }
    if (i == 3)
    {
        ret = 0;
    }
    SwitchGPIO();
    //���ж�
    sys_local_irq_restore(flags);
    return ret;
}

/*
 ********************************************************************************
 *           uint8 TuneControl(uint8 mode)
 *
 * Description : tune ����
 *
 * Arguments :   mode   0: clear tune;   1: start tune
 *
 * Returns :  �ɹ�:  ����1��ʧ��:  ����0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 TuneControl(uint8 mode)
{
    if (mode == 0)
    {
        WriteBuffer[4] &= ~(0x01 << 1); //clear tune
    }
    else
    {
        WriteBuffer[4] |= (0x01 << 1); //start tune
    }
    return WriteRegister(0x02, 0x02);

}

/*
 ********************************************************************************
 *             int  sFM_GetHardSeekflag(void* flag, void* null2, void* null3)
 *
 * Description : ��ȡӲ��seek �����Ƿ������־
 *
 * Arguments : ���������־��ָ�룬1  Ӳ��seek ����
 *
 * Returns : ����ȡ�ɹ�������1;����,����0
 *
 * Notes :   flag :  bit0  ����Ӳ��seek �Ƿ����    1:  �ѽ���   0:  δ����
 *                       bit1  ����Ӳ��seek �ҵ����Ƿ���Ч̨   1:  ��Ч̨
 ********************************************************************************
 */
int sFM_GetHardSeekflag(void* flag, void* null2, void* null3)
{
    int result;
    uint8* stc_flag;

    stc_flag = (uint8*) flag;
    result = ReadRegister(0x13);
    if (result == 1)
    {
        //STC Flag
        hardseek_flag = (ReadBuffer[1] & 0x20) >> 5;
        //seek ���̽���
        if (hardseek_flag == 0x01)
        {
            *stc_flag = 0x01;
            if ((ReadBuffer[1] & 0x10) == 0) //SF = 0
            {
                //seek ����Ч̨
                *stc_flag |= (0x01 << 1);
            }
        }
        //seek ����δ����
        else
        {
            *stc_flag = 0;
        }
        result = 1;
    }
    else
    {
        *stc_flag = 0;
        result = 0;
    }
    return result;

}

/*
 ********************************************************************************
 *             int  sFM_BreakSeek(void* null1, void* null2, void* null3);
 *
 * Description : Ӳ��seek �����У��ֶ�ֹͣseek ����
 *
 * Arguments :
 *
 * Returns : �ɹ�ֹͣ������1;����,����0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_BreakSeek(void* null1, void* null2, void* null3)
{
    int result;

    result = ReadRegister(0x13);
    if (result == 1)
    {
        //STC Flag
        hardseek_flag = (ReadBuffer[1] & 0x20) >> 5;
        //�Դ���Ӳ��seek ������
        if (hardseek_flag == 0)
        {
            WriteBuffer[6] &= (uint8) 0xbf; //stop seek
            result = WriteRegister(0x03, 0x02); //д��ؼĴ���
            if (result != 0)
            {
                hardseek_flag = 1;
                result = 1;
            }
            else
            {
                result = 0;
            }
        }
        //�Ѿ�������Ӳ��seek ���̣�ֱ�ӷ���ֹͣ�ɹ�
        else
        {
            result = 1;
        }
    }
    else
    {
        result = 0;
    }
    return result;
}

