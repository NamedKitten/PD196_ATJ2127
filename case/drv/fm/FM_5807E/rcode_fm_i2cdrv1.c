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

//��ʼ��ֵ for 5807hp
const uint8 InitBuffer_HP[110] =
{ 0xc4, 0x51, //02h:         bit10 ???? ????1.8v samsung flash
        //0xc0, 0x01,	  //
        0x00, 0x00, 0x04, 0x00, 0x86, 0xad, //05h://86
        0x40, 0x00, 0x56, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x0ah
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, //0x10h
        0x00, 0x19,// //0x00,0x09,//0830
        0x2a, 0x11, 0x00, 0x2e, 0x2a, 0x30, 0xb8, 0x3c, //0x15h
        0x90, 0x00, 0x2a, 0x91, 0x84, 0x12, 0x00, 0xa8, 0xc4, 0x00, //0x1ah
        0xe0, 0x00, 0x30, 0x1d,//0x24,0x9d,1ch
        0x81, 0x6a, 0x46, 0x08, 0x00, 0x86, //0x1fh
        0x06, 0x61,// 0x16,0x61, 20h
        0x00, 0x00, 0x10, 0x9e, 0x20, 0x40,//   0x24,0x47,//0830//23h
        0x04, 0x08,//0830
        0x06, 0x08, //0x06,0x08,//0830  //0x25h
        0xe1, 0x05, 0x3b, 0x6c, 0x2b, 0xec, 0x09, 0x0f, 0x34, 0x14, //0x2ah
        0x14, 0x50, 0x09, 0x6d, 0x2d, 0x96, 0x01, 0xda, 0x2a, 0x7b, 0x08, 0x21, //0x30h
        0x13, 0xd5, 0x48, 0x91, 0x00, 0xbc, 0x08, 0x96,//0830  0x34h
        0x15, 0x3c, 0x0b, 0x80, 0x25, 0xc7, 0x00, 0x00,
};

//��ʼ��ֵfor 5807P  32k
const uint8 InitBuffer_NP[64] =
{ 0xC0, 0x01, //0x51, //02H:
        0x00, 0x00, 0x04, 0x00, 0x8b, 0xAF, //05H:
        0x00, 0x00, 0x46, 0xC6, 0x50, 0x96, 0x00, 0x00, 0x40, 0x00, //0AH:
        0x00, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x05, //10H:
        0x90, 0x00, 0xF4, 0x84, 0x70, 0x01, 0x40, 0xF0, 0x21, 0x80, //15H:
        0x7A, 0xD0, 0x3E, 0x40, 0x55, 0xA9, 0xE8, 0x48, 0x50, 0x80, //1AH:
        0x00, 0x00, 0x37, 0xB6, 0x40, 0x0C, 0x07, 0x9B, 0x4C, 0x1D, 0x81, 0x11, //20H:
        0x45, 0x80,
};

//��ʼ��ֵfor 5807sp  24M --- 0xC4, 0x51;     32k --- 0xC0, 0x01
const uint8 InitBuffer_SP[104] =
{ //0xC4, 0x51, //02H:
    0xC0, 0x01, //02H:
            0x1b, 0x90, 0x04, 0x00, 0x86, 0xad, //05H://86
            0x80, 0x00, 0x5F, 0x1A, //07H  change the seek time;
            0x5e, 0xc6, 0x00, 0x00, 0x40, 0x6e, //0AH:
            0x2d, 0x80, 0x58, 0x03, 0x58, 0x04, 0x58, 0x04, 0x58, 0x04, 0x00, 0x47, //10H:
            0x90, 0x00, 0xF5, 0x87, 0x7F, 0x0B, //13H: reset ADC's parameters
            0x04, 0xF1, 0x5E, 0xc0, //15H: 0x42, 0xc0
            0x41, 0xe0, 0x50, 0x6f, 0x55, 0x92, 0x00, 0x7d, 0x10, 0xC0, // 1AH
            0x07, 0x80, 0x41, 0x1d, // 1CH,
            0x40, 0x06, 0x1f, 0x9B, 0x4c, 0x2b, // 1FH.
            0x81, 0x10, //20H:
            0x45, 0xa0, // 21H
            0x19, 0x3F, //22H: change AFC threshold
            0xaf, 0x40, 0x06, 0x81, 0x1b, 0x2a, //25H
            0x0D, 0x04, //26H, shutdown Rssi_autodown function
            0x80, 0x9F, //0x80, 0x2F,
            0x17, 0x8A, 0xD3, 0x49, 0x11, 0x42, 0xA0, 0xC4, // 2BH
            0x3E, 0xBB, 0x00, 0x00, 0x58, 0x04, 0x58, 0x04, // 2FH
            0x58, 0x04, 0x00, 0x74, 0x3D, 0x00, 0x03, 0x0C, 0x2F, 0x68, 0x38, 0x77, //35H
};

uint16 Freq_RF; //Freq infor read from reg, 0ah bit[9:0],freq = FREQ_RF*channel sapcing + 87M(76M)
uint16 signal_stg; //�����㣬��̨�ź�ǿ����Ϣ
uint8 Set_Band; //Band,03 h bit[3:2](00 = US,01 = Japan)
uint8 ST_info; //������:   ��̨��������Ϣ  0~~������   1~~������
uint8 hardseek_flag; //�Ƿ���Ӳ��seek �����еı�־��0:  ����Ӳ��seek ������   1:  ������Ӳ��seek ������
uint8 WriteNum; //��ͬģ���ʼ��ʱ��д���ֽ���
uint8 RDA_model; //ģ���ͺ�:    1  RDA 5807SP      2   RDA 5807HP    3  RDA 5807P

uint8 ReadBuffer[10]; //��buffer
uint8 WriteBuffer[111]; //дbuffer

int FM_SetFrequency(uint16 Freq);
uint8 CheckStation(void); //����̨��Ч��
uint8 WaitFlag(uint8 mode); //�ȴ�ָ����ʶ
uint8 ReadRegister(uint8 count); //���Ĵ���
int WriteRegister(uint8 count);//д�Ĵ���

void DisAssemble_WriteBuffer(void);
uint16 FreqToChan(uint16 freq, uint8 mode);
uint16 ChanToFreq(uint16 Chan, uint8 mode);

/*
 ********************************************************************************
 * int sFM_SetFreq(uint16 freq, void* null2, void* null3)
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

    TempFreq = FreqToChan(freq, Set_Band); //����̨Ƶ��ת���ɼĴ�����д���ֵ
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
    Engine_Status_t * ptr_buf = (Engine_Status_t *) pstruct_buf;

    //Ӳ��seek �����У�ֱ��ȡ�Ĵ�����Ϣ
    if (mode == 1)
    {
        result = ReadRegister(4);
    }
    //��Ӳ��seek �����У���ȴ�STC
    else
    {
        result = (int) WaitFlag(0);
    }
    if (result == 1)
    {
        DisAssemble_WriteBuffer();
        ptr_buf->FM_CurrentFreq = ChanToFreq(Freq_RF, Set_Band);
        ptr_buf->FM_SignalStg = signal_stg;
        if (Set_Band == 0x04)
        {
            ptr_buf->FM_CurBand = Band_Japan;
        }
        else if (Set_Band == 0x02)
        {
            ptr_buf->FM_CurBand = Band_Europe;
        }
        else
        {
            ptr_buf->FM_CurBand = Band_China_US;
        }
        ptr_buf->FM_Stereo_Status = (FM_Audio_e) ST_info;

        if (mode != 1)
        {
            //��Ӳ��seek ������
            hardseek_flag = 1;
        }

        if (hardseek_flag == 0)
        {
            ptr_buf->STC_flag = HARDSEEK_DOING;
        }
        else
        {
            ptr_buf->STC_flag = HARDSEEK_COMPLETE;
        }
        result = 1;
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
 * Arguments :  �����̨��ʽ,�����õ�Ƶ��ֵ
 *
 * Returns : ��̨Ϊ��̨,�򷵻�1;����,����0
 *
 * Notes : ��ģ����tune ��seek ������ɺ󣬲��ܶ�ȡ�����µ�
 *            Ƶ��ֵ��������Ӳ��seek �����в���ʵʱ��ʾƵ��
 *            ���������seek
 ********************************************************************************
 */
int sFM_Search(uint16 freq, uint8 direct, void* null3)
{
    int result;

    direct = direct;
    result = sFM_SetFreq(freq, (void*) 0, (void*) 0);
    if (result != 0)
    {
        if (CheckStation() == 1) // check station
        {
            //��ǰƵ��Ϊ��Ч��̨
            result = 1;
        }
        else
        {
            //��ǰΪ��Ч��̨
            result = 0;
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

    //Ĭ��Ӳ��seek  ��δ����
    hardseek_flag = 1;

    //tune ����ʼƵ��
    result = sFM_SetFreq(freq, (void*) 0, (void*) 0);
    if (result != 0)
    {
        temp = direct & 0x01;
        //seek down
        if (temp != 0)
        {
            WriteBuffer[1] &= (uint8) 0xfd;
        }
        //seek up
        else
        {
            WriteBuffer[1] |= 0x02;
        }

        temp = direct & 0x02;
        //������
        if (temp != 0)
        {
            WriteBuffer[2] |= (uint8) 0x80;
        }
        //����
        else
        {
            WriteBuffer[2] &= 0x7f;
        }
        WriteBuffer[2] |= 0x01;
        WriteBuffer[1] |= 0x01; //start seek
        result = WriteRegister(2); //д��ؼĴ���
        if (result != 0)
        {
            //Ӳ��seek ������
            hardseek_flag = 0;
        }
    }

    return result;
}

/*
 ********************************************************************************
 *             uint8 CheckStation(void)
 *
 * Description : ��̨�ж�
 *
 * Arguments :
 *
 * Returns : ���ѵ��ĵ�̨Ϊ��̨,����1;����,����0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 CheckStation(void)
{
    uint8 result;
    uint8 temp;
    //check fm ready flag
    result = WaitFlag(1);
    if (result != 0)
    {
        //�����̨��ʽ����̨�жϴ�������FM_TRUE �ж��Ƿ���̨
        temp = (uint8)(ReadBuffer[2] & 0x01);
        if (temp == 0x01)
        {
            DisAssemble_WriteBuffer(); //��ȡ��̨Ƶ��

            //��ͨ���Σ�96M Ϊ��̨
            if ((Set_Band == 0) && (Freq_RF >= 89) && (Freq_RF <= 91))
            {
                return 0;
            }
            //ŷ�޲��Σ�96M Ϊ��̨
            else if ((Set_Band == 2) && (Freq_RF >= 178) && (Freq_RF <= 182))
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
    return 0;
}

/*
 ********************************************************************************
 *             int FM_SetFrequency(uint16 Freq)
 *
 * Description : Ƶ������
 *
 * Arguments : �����õ�Ƶ��ֵ(  ʵ��д�� FM  IC  �Ĵ���)
 *
 * Returns : ��Ƶ�����óɹ�������1;����,����0
 *
 * Notes :
 *
 ********************************************************************************
 */
int FM_SetFrequency(uint16 Freq)
{
    int result = 0;
    WriteBuffer[1] &= (uint8) 0xfe; //��ֹSEEK ,ע��Ӳ��SEEK��ʽ,������Ƶ��\���� ʱ,����disable seek!!!!
    WriteBuffer[2] |= (0x01 << 0); //Enable FM
    WriteBuffer[3] = (uint8)(Freq >> 8); //����Ҫд���Ƶ��
    WriteBuffer[4] &= 0x3f;
    WriteBuffer[4] |= (uint8)(Freq % 256);
    WriteBuffer[4] |= (0x01 << 4); //enable tune
    result = WriteRegister(4); //����Ƶ��,д��ؼĴ���
    if (result != 0)
    {
        //����Ƶ�ʳɹ����ȴ�tune ���
        result = (int) WaitFlag(0);
    }
    return result;
}

/*
 ********************************************************************************
 *           void SwitchGPIO(void)
 *
 * Description : ��GPIO disable
 *
 * Arguments :  NULL
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
 *           uint8 ReadRegister(uint8 count)
 *
 * Description : ��FM ģ��Ĵ���
 *
 * Arguments :  ���ȡ���ֽ���
 *
 * Returns :  �ɹ�:  ����1��ʧ��:   ����0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 ReadRegister(uint8 count)
{
    uint8 result;
    uint8 i;
    uint8 ret;
    uint32 flags;
    //���ж�
    flags = sys_local_irq_save();

    for (i = 0; i < 3; i++)
    {
        result = I2C_Recev_Bytes(ReadBuffer, 0x21, count, I2C_gpio, &delay_i2c);
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
 *           uint8 WaitFlag(uint8 mode)
 *
 * Description : �ȴ���־
 *
 * Arguments : mode:     0---STC Flag;    1---FM Ready Flag
 *
 * Returns :   �ɹ��ȵ�ָ����ʶ,  ����1�����򷵻�0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 WaitFlag(uint8 mode)
{
    uint8 result;
    uint8 temp = 0;
    uint8 count = 0;
    sys_mdelay(10); //��ʱ10ms���ȴ�FM Tune��SEEK���
    do
    {
        result = ReadRegister(10);
        if (result != 0)
        {
            if (mode == 0)
            {
                temp = (uint8)(ReadBuffer[0] | 0xbf); //check stc flag
            }
            else
            {
                temp = (uint8)(ReadBuffer[3] | 0x7f); //check fm ready flag, used for soft seek
            }
            if (temp == 0xff)
            {
                return 1; //����ȵ���־�����˳�
            }
        }
        count++;
        sys_mdelay(10);
    } while (/*(temp != 0xff) && (count < 255)*/count < 255);
    return 0;
}

/*
 ********************************************************************************
 *           int WriteRegister(uint8 count)
 *
 * Description : дFM ģ��Ĵ���
 *
 * Arguments :   count:д�Ĵ����ĸ���
 *
 * Returns :  �ɹ�:  ����1��ʧ��:  ����0
 *
 * Notes :
 *
 ********************************************************************************
 */
int WriteRegister(uint8 count)
{
    uint8 i;
    uint8 result;
    uint32 flags;
    int ret;

    //���ж�
    flags = sys_local_irq_save();

    //һ�β������ɹ������Բ����ڶ���
    for (i = 0; i < 3; i++)
    {
        /*Ҫд����ֽ���Ϊд��Ĵ������ֽ�������I2C������ַ*/
        result = I2C_Trans_Bytes(WriteBuffer, count + 1, I2C_gpio, &delay_i2c);
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
 *           void DisAssemble_WriteBuffer(void)
 *
 * Description : ��ReadBuffer  ������ת��Ϊ�������Ϣ
 *
 * Arguments :  none
 *
 * Returns :   none
 *
 * Notes :
 *
 ********************************************************************************
 */
void DisAssemble_WriteBuffer(void)
{
    //��ȡ�������ĵ�̨Ƶ��
    Freq_RF = (uint16)(ReadBuffer[0] & 0x03);
    Freq_RF <<= 8;
    Freq_RF += ReadBuffer[1];

    //�ź�ǿ��RSSI
    signal_stg = (uint16)((ReadBuffer[2] & 0xfe) >> 1);

    //��������Ϣ
    ST_info = (ReadBuffer[0] & 0x04) >> 2;
    ST_info = 1 - ST_info; //����IC  ���ϲ㶨���෴

    //read STC��Ӳ��seek �����Ƿ����
    hardseek_flag = (ReadBuffer[0] & 0x40) >> 6;
    return;
}

/*
 ****************************************************************************************
 *           uint16 FreqToChan(uint16 freq, uint8 mode)
 *
 * Description : ��Ƶ��ת����д�Ĵ�����ֵ
 *
 * Arguments :  freq:  �����ת����Ƶ��( ʵ��Ƶ��khz Ϊ��λ�ĵ�2 �ֽ�)
 *			   mode��Ƶ��
 * Returns :	   ת��������Ҫд��Ĵ����ĵ�ֵ̨
 *
 * Notes :  ����Ƶ��ֵ( ��1khz Ϊ��λ)  �����bit  ��Ϊ1
 *
 *****************************************************************************************
 */
uint16 FreqToChan(uint16 freq, uint8 mode)
{
    uint16 Chan, tmp_freq;
    uint32 temp;

    /* �Ƚ�����ת��Ϊ��50khz Ϊ��λ��Ƶ��*/
    temp = (uint32) freq + 0x10000; //ʵ��Ƶ��ֵ����khz Ϊ��λ
    tmp_freq = (uint16)(temp / 50);

    if (mode == 0) //�й�/ ����
    {
        Chan = ((tmp_freq / 2 - 870) << 6) + 0x0010;
    }
    else if (mode == 1) //�ձ�
    {
        Chan = ((tmp_freq / 2 - 760) << 6) + 0x0010;
    }
    else //ŷ��
    {
        Chan = ((tmp_freq - 870* 2 ) << 6) + 0x0010; 
    }
    return Chan;
}

/*
 ********************************************************************************
 *           uint16 ChanToFreq(uint16 chan, uint8 mode)
 *
 * Description : �������ļĴ���Ƶ��ֵת�����ϲ���Ҫ��Ƶ��
 *
 * Arguments :  chan:������Ƶ��ֵ
 *			   mode��Ƶ��
 *
 * Returns :	   ת��������ʵ��Ƶ��ֵ(��1KHz Ϊ��λ�ĵ�2   �ֽ�)
 *
 * Notes :
 *
 ********************************************************************************
 */
uint16 ChanToFreq(uint16 chan, uint8 mode)
{
    uint16 Freq;
    uint32 temp;

    //��ת��Ϊ��50khz Ϊ��λƵ��
    if (mode == 0)
    {
        Freq = (chan + 870) * 2;
    }
    else if (mode == 1)
    {
        Freq = (chan + 760) * 2;
    }
    else
    {
        Freq = chan + 870* 2 ;
    }

    //ת��Ϊ��1khz  Ϊ��λ
    temp = Freq*50;
    Freq = (uint16)(temp & 0x0ffff);
    return Freq;
}

/*
 ********************************************************************************
 *             int  sFM_GetHardSeekflag(void* flag, void* null2, void* null3)
 *
 * Description : ��ȡӲ��seek �����Ƿ������־
 *
 * Arguments : ���������־��ָ��
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
    result = ReadRegister(4);
    if (result == 1)
    {
        DisAssemble_WriteBuffer();
        //seek ���̽���
        if (hardseek_flag == 0x01)
        {
            *stc_flag = 0x01;
            if ((ReadBuffer[0] & 0x20) == 0) //SF = 0
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
 * Arguments :  NULL
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

    result = ReadRegister(4);
    if (result == 1)
    {
        DisAssemble_WriteBuffer();
        //�Դ���Ӳ��seek ������
        if (hardseek_flag == 0)
        {
            WriteBuffer[1] &= (uint8) 0xfe; //stop seek
            result = WriteRegister(2); //д��ؼĴ���
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

