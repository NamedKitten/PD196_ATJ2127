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
 * \file     _rcode_lcd_functions_2.c
 * \brief    ����DMA�������ݴ����ģ��
 * \author   liutingting
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "lcd_inner.h"
//modify--vzhan
/*! \cond LCD_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    ����DMA����
 * \param[in]    dma_ram��dma��Դ��ַbuffer���ͣ�LCD_RAM_INDEX��ʾUI 1K buffer��JEPG_RAM_INDEX��ʾJPEG 8K buffer��
 *                        JPEG_CODEC_RAM_INDEX��ʾjpeg codec buffer
 * \param[out]   none
 * \return       �����Ƿ�ɹ�
 * \retval       TRUE ����ɹ�
 * \retval       FALSE ����ʧ��
 * \ingroup     lcd_dma_trans
 * \par         exmaple code
 * \code
 *          uint8 lcd_buffer_type = LCD_RAM_INDEX;
 *          lcd_dma_start_trans(lcd_buffer_type);
 * \endcode
 * \note
 *******************************************************************************/
bool dma_start_trans(uint8 dma_ram, void *null2, void *null3)
{
	#if 0
    //DMA��ʼ����
    irq_status = sys_local_irq_save(); //���ж�
    set_mfp_to_lcd(); //��MFP��LCD
#ifndef _FPGA_
    store_ce_gio();
#endif
    reg_writel(reg_readl(LCD_CTL) | ((0x00000001 << LCD_CTL_LCDFI) | (0x00000001 << LCD_CTL_FOVF)), LCD_CTL)
    //����DMA��Դ��ַ
    if (trans_mode == RGB_MODE)
    {
        if (dma_ram == LCD_RAM_INDEX)
        {
            dma_set_src_addr(LCD_BUF_ADDR, 0, 0);
        }
        else if (dma_ram == JPEG_RAM_INDEX)
        {
            dma_set_src_addr(JPEG_BUF_ADDR, 0, 0);
        }
        else
        {
        }
    }
    //�л�RAM��clock��DMA3
    dma_set_RAM_clk(dma_ram);
    //start_dma(dma_chan);
    reg_writel(reg_readl(DMA1CTL) | 0x00000001, DMA1CTL)
    reg_writel(reg_readl(LCD_CTL) | (0x00000001 << LCD_CTL_EN), LCD_CTL)
    while ((reg_readl(DMA1CTL) & 0x00000001) != 0)
    {
        ; //�ȴ�DMA����
    }
    //wait DMA transfer flag
    while ((reg_readl(LCD_CTL) & (0x00000001 << LCD_CTL_LCDFI)) == 0)
    {
        ; //�ȴ�LCD�������
    }
    restore_ce_gio();
    sys_local_irq_restore(irq_status); //���ж�
    #endif
    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    ����DMA����Ĳ���
 * \param[in]    pix_cnt��ҪDMA�����ͼ��ߴ�(��*��)
 * \param[in]    data_width��ҪDMA�����ͼ��Ŀ��
 * \param[out]   none
 * \return       none
 * \ingroup     lcd_dma_trans
 * \par         exmaple code
 * \code
 *          lcd_dma_set_counter(scroll->width * scroll->height, scroll->width);
 * \endcode
 * \note
 *******************************************************************************/
void dma_set_counter(uint32 pix_cnt, uint16 data_width, void *null3)
{
	#if 0
    //set_dma_lcdwidth(dma_chan, data_width);
    //set_dma_framelen(dma_chan, pix_cnt);
    reg_writel(pix_cnt, DMA1FrameLen)
    reg_writel(data_width, DMA1LCDCFG)
    #endif
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ����DMA��Դ��ַ
 * \param[in]    addr0��Դ��ַ0
 * \param[in]    addr1��Դ��ַ1
 * \param[in]    addr2��Դ��ַ2
 * \param[out]   none
 * \return       none
 * \ingroup     lcd_dma_trans
 * \par         exmaple code
 * \code
 *          ����1: RGBģʽ
 *          lcd_dma_set_src_addr(addr0);
 *
 *          ����2: YUV420��YUV444ģʽ
 *          lcd_dma_set_src_addr(addr0,addr1,addr2);
 * \endcode
 * \note
 *******************************************************************************/
void dma_set_src_addr(uint32 addr0, uint32 addr1, uint32 addr2)
{
	#if 0
    if (trans_mode == RGB_MODE)
    {
        //set_dma_src0(dma_chan, addr0);
        reg_writel(addr0, DMA1SADDR0)
    }
    else
    {
        //set_dma_src0(dma_chan, addr0);
        //set_dma_src1(dma_chan, addr1);
        //set_dma_src2(dma_chan, addr2);
        reg_writel(addr0, DMA1SADDR0)
        reg_writel(addr1, DMA1SADDR1)
        reg_writel(addr2, DMA1SADDR2)
    }
    #endif
}
/*! \endcond */

/*
void restore_clk(uint32 memclk_ctl0, uint32 memclk_ctl1, uint32 cpuclk_ctl,uint32 clken_ctl)
{
//reg_writel(memclk_ctl0, MEMCLKCTL0)
//reg_writel(memclk_ctl1, MEMCLKCTL1)
//reg_writel(cpuclk_ctl, CPUCLKCTL)
//reg_writel(clken_ctl, CLKENCTL)
}

void store_clk(uint32* pmemclk_ctl0, uint32* pmemclk_ctl1, uint32* pcpuclk_ctl,uint32 *pclken_ctl)
{
// *pmemclk_ctl0 = reg_readl(MEMCLKCTL0);
// *pmemclk_ctl1 = reg_readl(MEMCLKCTL1);
// *pcpuclk_ctl = reg_readl(CPUCLKCTL);
// *pclken_ctl = reg_readl(CLKENCTL);
}
*/

