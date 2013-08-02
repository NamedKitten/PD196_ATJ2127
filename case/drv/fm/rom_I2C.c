/*******************************************************************************
 *                              US212A
 *                            Module: IIC driver
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     mikeyang     2011-11-03 15:00     1.0             build this file
 *******************************************************************************/
#include "I2C.h"

void IIC_Init(gpio_init_cfg_t* gpio, uint8* delay);
void IIC_Exit(gpio_init_cfg_t* gpio);
void IIC_Delay(uint8* delay);
void IIC_SDA_high_hold(gpio_init_cfg_t* gpio, uint8* delay);
void IIC_SDA_low_hold(gpio_init_cfg_t* gpio, uint8* delay);
void IIC_SCL_high_hold(gpio_init_cfg_t* gpio, uint8* delay);
void IIC_SCL_low_hold(gpio_init_cfg_t* gpio, uint8* delay);

void IIC_SDA_Output_En(gpio_init_cfg_t* gpio);
void IIC_SDA_Output_Dis(gpio_init_cfg_t* gpio);
void IIC_SDA_Input_En(gpio_init_cfg_t* gpio);
void IIC_SDA_Input_Dis(gpio_init_cfg_t* gpio);
void IIC_SCL_Output_En(gpio_init_cfg_t* gpio);
void IIC_SCL_Output_Dis(gpio_init_cfg_t* gpio);
void IIC_SCL_Input_En(gpio_init_cfg_t* gpio);
void IIC_SCL_Input_Dis(gpio_init_cfg_t* gpio);
uint8 IIC_Data_Input(gpio_init_cfg_t* gpio);

void IIC_Start(gpio_init_cfg_t* gpio, uint8* delay);
void IIC_Stop(gpio_init_cfg_t* gpio, uint8* delay);

uint8 I2C_Trans_Bytes(uint8 *buf, uint8 length, gpio_init_cfg_t* gpio, uint8* delay);
uint8 I2C_Recev_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay);
void IIC_WriteByte(uint8 dat, gpio_init_cfg_t* gpio, uint8* delay);
uint8 IIC_ReadByte(gpio_init_cfg_t* gpio, uint8* delay);
void IIC_SendAck(uint8 ack, gpio_init_cfg_t* gpio, uint8* delay);
uint8 IIC_GetAck(gpio_init_cfg_t* gpio, uint8* delay);

/*
 *******************************************************************************
 *             void IIC_Init(gpio_init_cfg_t* gpio, uint8* delay)
 *
 * Description : I2C��ʼ��
 *
 * Arguments   : ģ��I2C  �������õ�IO ,  ������ʱ

 * Returns     : NULL

 * Notes       :
 *
 *******************************************************************************
 */
void IIC_Init(gpio_init_cfg_t* gpio, uint8* delay)
{
    IIC_SDA_high_hold(gpio, delay);
    IIC_SCL_high_hold(gpio, delay);
    IIC_SDA_Output_En(gpio);
    IIC_SCL_Output_En(gpio);
    IIC_SCL_Input_En(gpio);
    IIC_SDA_Input_En(gpio);

}

/*
 *******************************************************************************
 *             void IIC_Exit(gpio_init_cfg_t* gpio)
 *
 * Description : I2C �˳�
 *
 * Arguments   : ģ��I2C  �������õ�IO 

 * Returns     : NULL

 * Notes       :
 *
 *******************************************************************************
 */
void IIC_Exit(gpio_init_cfg_t* gpio)
{
    IIC_SDA_Output_Dis(gpio);
    IIC_SDA_Input_Dis(gpio);
    IIC_SCL_Output_Dis(gpio);
    IIC_SCL_Input_Dis(gpio);
}

/************************************************************************
 * Description:  ģ��IIC  ����IIC ������ʱ
 * Input:   ������ʱ
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_Delay(uint8* delay)
{
    uint8 j, value;

    value = *delay;
    for (j = 0; j < value; j++)
    {
        //��ʱ����
        //;
    }
}

/************************************************************************
 * Description:  ģ��IIC  ��������������
 * Input:   ģ��I2C  �������õ�IO ,  ������ʱ
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SDA_high_hold(gpio_init_cfg_t* gpio, uint8* delay)
{
    act_writel((act_readl((gpio + 1)->reg_data) | ((gpio + 1)->reg_bit)), (gpio + 1)->reg_data);
    IIC_Delay(delay);
}

/************************************************************************
 * Description:  ģ��IIC  ��������������
 * Input:   ģ��I2C  �������õ�IO ,  ������ʱ
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SDA_low_hold(gpio_init_cfg_t* gpio, uint8* delay)
{
    act_writel((act_readl((gpio + 1)->reg_data) & (~((gpio + 1)->reg_bit))), (gpio + 1)->reg_data);
    IIC_Delay(delay);
}

/************************************************************************
 * Description:  ģ��IIC  ����ʱ��������
 * Input:   ģ��I2C  �������õ�IO ,  ������ʱ
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SCL_high_hold(gpio_init_cfg_t* gpio, uint8* delay)
{
    act_writel((act_readl(gpio->reg_data) | (gpio->reg_bit)), gpio->reg_data);
    IIC_Delay(delay);
}

/************************************************************************
 * Description:  ģ��IIC  ����ʱ��������
 * Input:   ģ��I2C  �������õ�IO ,  ������ʱ
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SCL_low_hold(gpio_init_cfg_t* gpio, uint8* delay)
{
    act_writel((act_readl(gpio->reg_data) & (~(gpio->reg_bit))), gpio->reg_data);
    IIC_Delay(delay);
}

/************************************************************************
 * Description:  ģ��IIC  �������������ʹ��
 * Input:   ģ��I2C  �������õ�IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SDA_Output_En(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl((gpio + 1)->reg_out) | ((gpio + 1)->reg_bit)), (gpio + 1)->reg_out);
}

/************************************************************************
 * Description:  ģ��IIC  �������������disable
 * Input:   ģ��I2C  �������õ�IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SDA_Output_Dis(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl((gpio + 1)->reg_out) & (~((gpio + 1)->reg_bit))), (gpio + 1)->reg_out);
}

/************************************************************************
 * Description:  ģ��IIC  ��������������ʹ��
 * Input:  ģ��I2C  �������õ�IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SDA_Input_En(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl((gpio + 1)->reg_in) | ((gpio + 1)->reg_bit)), (gpio + 1)->reg_in);
}

/************************************************************************
 * Description:  ģ��IIC  ��������������disable
 * Input:   ģ��I2C  �������õ�IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SDA_Input_Dis(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl((gpio + 1)->reg_in) & (~((gpio + 1)->reg_bit))), (gpio + 1)->reg_in);
}

/************************************************************************
 * Description:  ģ��IIC  ����ʱ�������ʹ��
 * Input:   ģ��I2C  �������õ�IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SCL_Output_En(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl(gpio->reg_out) | (gpio->reg_bit)), gpio->reg_out);
}

/************************************************************************
 * Description:  ģ��IIC  ����ʱ�������disable
 * Input:   ģ��I2C  �������õ�IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SCL_Output_Dis(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl(gpio->reg_out) & (~(gpio->reg_bit))), gpio->reg_out);
}

/************************************************************************
 * Description:  ģ��IIC  ����ʱ��������ʹ��
 * Input:   ģ��I2C  �������õ�IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SCL_Input_En(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl(gpio->reg_in) | (gpio->reg_bit)), gpio->reg_in);
}

/************************************************************************
 * Description:  ģ��IIC  ����ʱ��������disable
 * Input:   ģ��I2C  �������õ�IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SCL_Input_Dis(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl(gpio->reg_in) & (~(gpio->reg_bit))), gpio->reg_in);
}

/************************************************************************
 * Description:  �����32 bit ������һ��bit Ϊ1
 * Input:   uint32
 * Output: ���ҵ���Ϊ1 ��bit λ
 * Note:
 *************************************************************************
 **/
uint8 IIC_Data_Bit(uint32 value)
{
    uint8 loop;
    uint32 temp;
    for (loop = 0; loop < 32; loop++)
    {
        temp = (value >> loop) & 0x00000001;
        if (temp != 0)
        {
            break;
        }
    }
    return loop;
}

/************************************************************************
 * Description:  ģ��IIC  ���߲���������������
 * Input:   ģ��I2C  �������õ�IO 
 * Output: ���������ϲ�������bit ֵ��ret  bit0
 * Note:
 *************************************************************************
 **/
uint8 IIC_Data_Input(gpio_init_cfg_t* gpio)
{
    uint32 value;
    uint8 ret;
    value = act_readl((gpio + 1)->reg_data) & ((gpio + 1)->reg_bit);
    ret = IIC_Data_Bit((gpio + 1)->reg_bit);
    value = (value >> ret) & 0x00000001;
    ret = (uint8) value;
    return ret;
}

/*
 *******************************************************************************
 *             IIC_Start
 *
 * Description :  ģ��IIC   Start  ����
 *
 * Arguments   : ģ��I2C  �������õ�IO ,  ������ʱ
 *
 * Returns     : NULL
 *
 * Notes       :
 *
 *******************************************************************************
 */
/*
 ____
 SDA:    |_______

 ______
 SCL:      |_____
 */

void IIC_Start(gpio_init_cfg_t* gpio, uint8* delay) /*IIC start command*/
{
    IIC_SDA_high_hold(gpio, delay); /*������ʼ�����������ź�*/
    IIC_SCL_high_hold(gpio, delay); /*��ʼ��������ʱ�����4.7us,��ʱ*/
    IIC_SDA_low_hold(gpio, delay); /*������ʼ�ź�*//* ��ʼ��������ʱ�����4��s*/
    IIC_SCL_low_hold(gpio, delay); /*ǯסIIC���ߣ�SCLΪ�͵�ƽ��׼�����ͻ�������� */
}

/*
 *******************************************************************************
 *             IIC_Stop
 *
 * Description :  ģ��IIC  Stop  ����
 *
 * Arguments   : ģ��I2C  �������õ�IO ,  ������ʱ
 *
 * Returns     : NULL
 *
 * Notes       :
 *
 *******************************************************************************
 */
/*
 _______
 SDA: _______|
 ________
 SCL: ______|
 */

void IIC_Stop(gpio_init_cfg_t* gpio, uint8* delay) /*IIC stop command*/
{
    IIC_SDA_low_hold(gpio, delay); /*���ͽ��������������ź�*/
    IIC_SCL_high_hold(gpio, delay); /*������������ʱ�����4��s*/
    IIC_SDA_high_hold(gpio, delay); /*����IIC���߽����ź�*/
}

//=============================================================================================
/*
 ******************************************************************************
 *             uint8 I2C_Trans_Bytes(uint8 *buf, uint8 length)
 *
 * Description : ����ŵ�*buf��ַ�ﳤ��Ϊlength�����ݷ��ͳ�ȥ
 *
 * Arguments   : unsigned char *buf: ���ݴ�ŵ�ַ
 unsigned char length: ���ݳ���, ��byteΪ��λ
 *
 * Returns     : FALSE: �������ݽ��յ�nack
 TRUE: �������ݽ��յ�ack
 *
 * Notes       : ��������I2C �豸�������ݣ����������豸����Ӧ
 *                  �����ڲ��������������ṩ�ӿ�
 ********************************************************************************
 */
uint8 I2C_Trans_Bytes(uint8 *buf, uint8 length, gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 ret;

    IIC_Init(gpio, delay);

    IIC_Start(gpio, delay);
    //��������
    do
    {
        IIC_WriteByte(*buf, gpio, delay);
        buf++;
        if (!IIC_GetAck(gpio, delay))
        {
            break;
        }
        --length;
    } while (length != 0);

    if (length == 0)
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }
    IIC_Stop(gpio, delay);
    return ret;
}

//=============================================================================================
/*
 ******************************************************************************
 *             uint8 I2C_Recev_Bytes(uint8 *buf, uint8 address, uint8 length)
 *
 * Description : ��ȡ����length
 *
 * Arguments   : unsigned char *buf: ���ݴ�ŵ�ַ
 unsigned char address: slave��ַ
 *               unsigned char length: ���ݳ���,��byteΪ��λ
 *
 * Returns     : FALSE: slave����Ӧ
 TRUE: �յ�����
 *
 * Notes       :  �����ڲ��ӿڣ��������ṩ
 *
 ********************************************************************************
 */
uint8 I2C_Recev_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 ret = 0;
    uint8 *p;

    p = buf;

    IIC_Init(gpio, delay);

    IIC_Start(gpio, delay);

    /*����slave ��ַ*/
    IIC_WriteByte(address, gpio, delay);
    if (!IIC_GetAck(gpio, delay))
    {
        ret = 0;
        goto recev_ret;
    }

    while (length != 0)
    {
        if (length == 1) //the last byte, sent nack
        {
            *p = IIC_ReadByte(gpio, delay);
            IIC_SendAck(1, gpio, delay);
        }
        else
        {
            *p = IIC_ReadByte(gpio, delay);
            IIC_SendAck(0, gpio, delay);
        }
        p++;
        length--;
    }
    ret = 1;
    recev_ret: IIC_Stop(gpio, delay);
    return ret;
}

/*
 *******************************************************************************
 *             void IIC_WriteByte(uint8 dat)
 *
 * Description : д1���ֽڵ����ݵ�slave
 *
 * Arguments   : 1���ֽ�����

 *
 * Returns     : 1 ack; 0 nack
 *
 * Notes       :
 *
 *******************************************************************************
 */
void IIC_WriteByte(uint8 dat, gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 i;
    for (i = 0; i < 8; i++)
    {
        if (((dat << i) & 0x80) != 0) //����һ���ֽڣ��Ӹ�bit ����bit ���η���
        {
            IIC_SDA_high_hold(gpio, delay);
        }
        else
        {
            IIC_SDA_low_hold(gpio, delay);
        }
        IIC_SCL_high_hold(gpio, delay);
        IIC_SCL_low_hold(gpio, delay);
    }
}

/*
 *******************************************************************************
 *             uint8 IIC_ReadByte(void)
 *
 * Description : ��ȡ1���ֽڵ�����,����ack����nack��slave
 *
 * Arguments   : unsigned char ack: 1 ack; 0 nack

 *
 * Returns     : ����������
 *
 * Notes       :
 *
 *******************************************************************************
 */
uint8 IIC_ReadByte(gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 t = 8, dat = 0;
    uint8 IIC_DATA;

    //IIC_SDA_high_hold(gpio, delay);

    //IIC_Delay(delay);
    IIC_SDA_Output_Dis(gpio);
    //IIC_SDA_Input_En(gpio);
    IIC_Delay(delay);

    do
    {
        IIC_SCL_high_hold(gpio, delay);

        dat <<= 1;
        IIC_DATA = IIC_Data_Input(gpio);
        if (IIC_DATA != 0)
        {
            dat |= 0x01;
        } //��I2C �������ϲ���һ���ֽڵ����ݣ��ǴӸ�bit ����bit ����

        IIC_SCL_low_hold(gpio, delay);
        --t;
    } while (t != 0);

    //IIC_Delay(delay);
    //IIC_SDA_Input_Dis(gpio);
    IIC_SDA_Output_En(gpio);
    IIC_Delay(delay);
    return dat;
}

/*
 *******************************************************************************
 *             void IIC_SendAck(uint8 ack)
 *
 * Description : ����ack����nack
 *
 * Arguments   : unsigned char ack: 1 noack; 0 ack

 *
 * Returns     :
 *
 * Notes       :
 *
 *******************************************************************************
 */
void IIC_SendAck(uint8 ack, gpio_init_cfg_t* gpio, uint8* delay)
{
    if (ack != 0)
    {
        IIC_SDA_high_hold(gpio, delay); //nack
    }
    else
    {
        IIC_SDA_low_hold(gpio, delay); //ack
    }
    IIC_SCL_high_hold(gpio, delay);
    IIC_SCL_low_hold(gpio, delay);
}
/*
 *******************************************************************************
 *             uint8 IIC_GetAck(void)
 *
 * Description : Return Ack From the slave device, one is Ack, but zero NoAck.
 *
 * Arguments   : unsigned char ack: 1 noack; 0 ack

 *
 * Returns     :
 *
 * Notes       :   ��ȡ���豸�˷��͹�������Ӧ�ź�Ack  or  NAck
 *
 *******************************************************************************
 */
uint8 IIC_GetAck(gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 ack;
    uint8 IIC_DATA;

    // IIC_SDA_high_hold(gpio, delay);


    //IIC_Delay(delay);
    IIC_SDA_Output_Dis(gpio);
    //IIC_SDA_Input_En(gpio);
    IIC_Delay(delay);
    IIC_SCL_high_hold(gpio, delay);
    IIC_DATA = IIC_Data_Input(gpio); //ֻ����һ��bit
    //IIC_SDA_Input_Dis(gpio);
    if (IIC_DATA != 0)
    {
        ack = 0; //nack
        //IIC_SDA_high_hold(gpio, delay);
    }
    else
    {
        ack = 1; //ack
        //IIC_SDA_low_hold(gpio, delay);
    }
    IIC_SDA_Output_En(gpio);
    IIC_Delay(delay);
    IIC_SCL_low_hold(gpio, delay);
    IIC_SDA_low_hold(gpio, delay);
    //IIC_SDA_Input_Dis(gpio);

    return ack;
}

