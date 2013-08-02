/*******************************************************************************
 *                              US212A
 *                            Module: lcd driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-20 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     _rcode_lcd_functions.c
 * \brief    lcd����������Ĳ�������ģ��
 * \author   liutingting
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "lcd_inner.h"
/*! \cond */


void write_command(uint8 cmd)
{
    //RS����͵�ƽ
    reg_writel(reg_readl(EXTMEM_CTL) & (~(0x00000001 << EXTMEM_CTL_RS)), EXTMEM_CTL)
    reg_writel(cmd, EXTMEM_DATA)
    #ifdef __WELCOME__
    welcome_delay_us(3);
    #else
    sys_udelay(3);
    #endif
}
//modify--vzhan
void write_data(uint8 *data_buf, uint16 pix_cnt)
{
    //дdata
    uint16 mid_data;
    uint8 merge_two_short_piexl=0;
	uint16 *data_buf_16 = (uint16 *)data_buf;
	uint16 *p_data;
    unsigned int i,j;
    unsigned int m;
    unsigned int k;
	uint8 beginpage,pagenum;
write_command(0xc8);
 	beginpage = region_setwin.y/8 + 0xb0;
	pagenum = beginpage + region_setwin.height/8;

	for(i=beginpage;i<pagenum;i++)
	{
		k=(i-beginpage)*8;
		write_command(i); //дCMD
		write_command(((region_setwin.x & 0xf0)>>4) | 0x10); //дCMD x �ĸ�λ
		write_command(region_setwin.x & 0x0f); //дCMD x�ĵ�λ

		for(j=0;j<region_setwin.width;j++)
		{
		    act_writel(act_readl(EXTMEM_CTL) | (0x00000001 << EXTMEM_CTL_RS), EXTMEM_CTL);
			merge_two_short_piexl=0;

			for(m=0;m<8;m++)
			{	
				p_data = &data_buf_16[(k+m)*region_setwin.width + j];
				mid_data = *p_data;
				
				if(mid_data==0xffff)
					merge_two_short_piexl |= (0x01 << (m));
			}
			act_writel(merge_two_short_piexl, EXTMEM_DATA);	
		}
	}
}

#ifdef __WELCOME__
void welcome_delay_ms(uint8 dly_ms)
{
    uint8 i, j;
    while (dly_ms != 0)
    {
        for (i = 0; i < 20; i++)
        {
            for (j = 0; j < 255; j++)
            {
                ;//����䣬�鿴lstȷ��ѭ����ִ��������
            }
        }
        dly_ms--;
    }
}

void welcome_delay_us(uint8 dly_us)
{
    uint8 i;
    while (dly_us != 0)
    {
        for (i = 0; i < 5; i++)
        {
            ;//����䣬�鿴lstȷ��ѭ����ִ��������
        }
        dly_us--;
    }
}
#endif//__WELCOME__

/*! \endcond */

/*! \cond LCD_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *       ��buffer�е���������
 * \param[in]   pix_cnt: Ҫд�����ص����
 * \param[in]   buff: д���ݵ�buffer
 * \return      none
 * \ingroup     lcd_write_read
 * \par         exmaple code
 * \code
 *          uint8 *display_buffer = (uint8 *) display_buf_addr;
 *          uint16 pix_cnt = 1024;
 *          lcd_buff_data_trans(display_buffer,pix_cnt);
 * \endcode
 * \note
 *******************************************************************************/
#ifndef PC
void buff_data_trans(uint8 *buff, uint16 pix_cnt, void *null3)
{
    //д��������һ��lcd buffer
    store_ce();
    write_data(buff, pix_cnt * 2);
    restore_ce();
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 *       ����
 * \param[in]   pix_cnt: Ҫ��ȡ�����ص����
 * \param[out]  buff: �����ȡ����Ļ���ݵ�buffer
 * \return      none
 * \ingroup     lcd_write_read
 * \par         exmaple code
 * \code
 *          uint8 *display_buffer = (uint8 *) display_buf_addr;
 *          uint16 pix_cnt = 1024;
 *          lcd_get_buff_data(display_buffer,pix_cnt);
 * \endcode
 * \note
 *******************************************************************************/
#ifndef PC
//�ýӿ�ֻ�ܶ���ͨ��DMA�������ص�����
//modify--vzhan
void get_buff_data(uint8 *buff, uint16 pix_cnt, void *null3)
{/*
    uint32 cpuclk_ctl, cpuclk_ctl_data; //����memory ʱ��״̬
    uint16 i;

    store_ce();
    
    cpuclk_ctl = cpuclk_ctl_data = reg_readl(CPUCLKCTL);
    //CPU CLK 24M; AHB_CLK div 2
    cpuclk_ctl_data &= ((~CPUCLKCTL_CPUCLKSEL_MASK) & (~(0x00000001 << CPUCLKCTL_AHBCLKDIV)));
    cpuclk_ctl_data |= (0x02 << CPUCLKCTL_CPUCLKSEL_SHIFT);
    reg_writel(cpuclk_ctl_data, CPUCLKCTL)
    
    write_command(LCD_CMD_READ);
    reg_writel(reg_readl(EXTMEM_CTL) | (0x00000001 << EXTMEM_CTL_RS), EXTMEM_CTL)
    //�ȿն�һ��
    reg_readb(EXTMEM_DATA);
    for (i = 0; i < pix_cnt; i++)
    {
        //EXTMEM_DATA ������ red green blue data
        *(uint16 *)buff = ( ((uint16) (reg_readb(EXTMEM_DATA) & 0xf8) << 8) 
                          | ((uint16) (reg_readb(EXTMEM_DATA) & 0xfc) << 3) 
                          | ((uint16) (reg_readb(EXTMEM_DATA) & 0xf8) >> 3) );
        buff += 2;
    }
    reg_writel(cpuclk_ctl, CPUCLKCTL)
    restore_ce();
*/
}
#endif

/*! \endcond */
