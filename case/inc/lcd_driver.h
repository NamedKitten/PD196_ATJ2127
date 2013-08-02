/*******************************************************************************
 *                              us212A
 *                            Module: ui����
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-9 16:23     1.0             build this file 
*******************************************************************************/
/*!
 * \file     lcd_driver.h
 * \brief    lcd��������ģ��������ݽṹ���궨�壬�ӿ�������
 * \author   liutinging
 * \version 1.0
 * \date  2011-9-9
*******************************************************************************/

#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

//#define _DEMO_    /* for demo, enable this macro. complile welcome and lcd_driver.  */

/*! \cond LCD_DRIVER_API */

#include "psp_includes.h"

/*! �Ƿ�֧�ֶ������� */
#define SUPPORT_READ_CMD

/*!
 * \brief
 *      region_t��������Ϣ�ṹ�嶨��
 */
typedef struct
{
    /*! ��ʼλ��X���� */
    uint16 x; 
    /*! ��ʼλ��Y���� */
    uint16 y; 
    /*! ����Ŀ�� */
    uint16 width;
    /*! ����ĸ߶� */
    uint16 height;
} region_t;
/*!
 * \brief
 *  lcd_cmd_e��lcd������������ö������
 */
typedef enum
{
    /*!�贰*/
    LCD_SET_WINDOW= 0,
    /*!���öԱȶ�*/
    LCD_SET_CONTRAST,
    /*!������ʾģʽ*/
    LCD_SET_DRAWMODE,
    /*!��Ļstandby����*/
    LCD_STANDBY_SCREEN,
    /*!���ر������*/
    LCD_BACKLIGHT_ONOFF,
    /*!����ָ������Ļ����*/
    LCD_UPDATE_SCREEN,
    /*!��buffer�е���������*/
    LCD_TRANS_BUFFDATA,
    /*!��ȡLCD���ϵ�����*/
    LCD_GET_BUFFDATA,
    /*!����ʾBUFFER�е����ݷ�ɫ*/
    LCD_INVERT_BUFFDATA,
    /*!LCD���Ƴ�ʼ��*/
    LCD_CONTROLLER_INIT,
    /*!����DMA��������ݿ��*/
    LCD_DMA_SET_COUNTER,
    /*!����DMA��ʼ����*/
    LCD_DMA_START_TRANS,
    /*!����DMAԴ��ַ*/
    LCD_DMA_SET_SRC_ADDR,
    /*!����RAM8��CLK��MCU*/
    LCD_SET_JRAM_CLK,
    /*!�ָ�ԭ����CLK*/
    LCD_RESTORE_JRAM_CLK
} lcd_cmd_e;

/*!
 * \brief
 *  draw mode ��Ļ����ԭ�������½ǣ��߼�ԭ�������Ͻ�
 */
/*! ˮƽ�������ȣ��������ϵ����£�������������ʾ�ַ�������ԴͼƬ��������welcomeͼƬ��JPEG��GIF */
#define  DRAW_MODE_H_DEF         0 
/*! ��ֱ�������ȣ��������ϵ����£������ں�����ʾ�ַ�������ԴͼƬ��������JPEG��GIF��AVI */
#define  DRAW_MODE_V_DEF         1 
/*! ˮƽ�������ȣ��������µ����ϣ�������������ʾBMP */
#define  DRAW_MODE_H_PIC_DEF     2
/*! ��ֱ�������ȣ��������µ����ϣ������ں�����ʾBMP��AMV */
#define  DRAW_MODE_V_PIC_DEF     3 
/*! ��ֱ�������ȣ��������ϵ����£�������������ʾ�ַ������� */ 
#define  DRAW_MODE_H_SCROLL_DEF  4
/*! ˮƽ�������ȣ��������ϵ����£������ں�����ʾ�ַ������� */ 
#define  DRAW_MODE_V_SCROLL_DEF  5
/*! AVIģʽ������ֱ�������ȣ��������ϵ����£� */
#define  DRAW_MODE_AVI_DEF       DRAW_MODE_V_DEF  

#ifndef PC

#define JPEG_BUF_ADDR 0x32000
#define JPEG_BUF_LEN  0x1800
#define LCD_BUF_ADDR  0x18000//�����ģ��ʾ����������1KB��������ָ���㹻��Ŀռ��������������޸� set_display_buf ����
#define LCD_BUF_LEN    0x400

#else
#define JPEG_BUF_ADDR 0x3000
#define JPEG_BUF_LEN  0x1800
#define LCD_BUF_ADDR  0x3000//�޸�Ϊ0x3000��ʼ���ռ�Ϊ2KB��������֤�� ui simulator�Ͽ���֧�� 32*32 �ֿ�
#define LCD_BUF_LEN    0x800

#endif

#define LCD_RAM_INDEX      0
#define JPEG_RAM_INDEX     1
#define JPEG_CODEC_RAM_INDEX     2


/*!
 * \brief
 *      res_type_e����Դ�������ݽṹ
 */
typedef enum
{
    /*! RGBģʽ */
    RGB_MODE = 0, 
    /*! YUV444ģʽ */
    YUV444_MODE = 1, 
    /*! YUV420ģʽ */
    YUV420_MODE = 2 
} lcd_mode_e;

/*! LCD��ʾ�ĳ��� */
#define DISPLAY_LENGTH      128
/*! LCD��ʾ�ĸ߶� */
#define DISPLAY_HEIGHT     160

//#define _FPGA_
#ifdef _FPGA_
#define LCMRST_GIO_EN_REG        GPIO_AOUTEN    //GPIO_A5 (Output)
#define LCMRST_GIO_DATA_REG      GPIO_ADAT
#define LCMRST_GIO_EN_BIT        (0x00000001<<5)
#define LCMRST_SET_BIT           (0x00000001<<5)
#define LCMRST_CLR_BIT           ~(0x00000001<<5)

#define LCMBL_GIO_EN_REG        GPIO_AOUTEN        //GPIO_A6 (Output)
#define LCMBL_GIO_DATA_REG      GPIO_ADAT
#define LCMBL_GIO_EN_BIT        (0x00000001<<6)
#define LCMBL_SET_BIT           (0x00000001<<6)
#define LCMBL_CLR_BIT           ~(0x00000001<<6)

#else

#define LCMRST_GIO_EN_REG        GPIO_AOUTEN    //GPIO_A15 (Output)
#define LCMRST_GIO_DATA_REG      GPIO_ADAT
#define CE_GIO_EN_REG           GPIO_AOUTEN        //GPIO_A18 CS pin 
#define CE_GIO_DATA_REG         GPIO_ADAT

#ifdef _DEMO_
#define LCMRST_GIO_EN_BIT        (0x00000001<<0)
#define LCMRST_SET_BIT           (0x00000001<<0)
#define LCMRST_CLR_BIT           ~(0x00000001<<0)
#define CE_EN_BIT               (0x00000001<<15)
#define CE_DN_BIT               ~(0x00000001<<15)
#define CE_SET_BIT              (0x00000001<<15)
#define CE_CLR_BIT              ~(0x00000001<<15) 

#else
#define LCMRST_GIO_EN_BIT        (0x00000001<<15)
#define LCMRST_SET_BIT           (0x00000001<<15)
#define LCMRST_CLR_BIT           ~(0x00000001<<15)
#define CE_EN_BIT               (0x00000001<<18)
#define CE_DN_BIT               ~(0x00000001<<18)
#define CE_SET_BIT              (0x00000001<<18)
#define CE_CLR_BIT              ~(0x00000001<<18) 
#endif //_DEMO_

#define LCMBL_GIO_EN_REG        GPIO_AOUTEN        //GPIO_A16 (Output)
#define LCMBL_GIO_DATA_REG      GPIO_ADAT
#define LCMBL_GIO_EN_BIT        (0x00000001<<16)
#define LCMBL_SET_BIT           (0x00000001<<16)
#define LCMBL_CLR_BIT           ~(0x00000001<<16)




#endif

#define EXTMEM_CE_SEL      0x00000005  //CE4


#define reg_writeb(val, reg)  *(volatile unsigned char *)(reg) = (val);
#define reg_writew(val, reg)  *(volatile unsigned short *)(reg) = (val);
#define reg_writel(val, reg)  *(volatile unsigned int *)(reg) = (val);
#define reg_readb(port)      (*(volatile unsigned char *)(port))
#define reg_readw(port)      (*(volatile unsigned short *)(port))
#define reg_readl(port)      (*(volatile unsigned int *)(port))




//display mode

#define RGBFLAG     0x08
#define MNONEFLAG   0x00
#define MXENDFLAG   0x01
#define MYENDFLAG   0x02
#define MVENDFLAG   0x03
//add for less mode 
#define MYREVERSE   0x04
//add for string scroll
#define SCROLLFLAG  0x05

extern uint8 lcd_draw_mode;  //LCD����ʾģʽ
extern int8 reent_flag;    //Ƭѡ��־
//extern uint32 ce_bak;    //����Ƭѡ��Ϣ
extern uint32 irq_status;
extern uint8 contrast_grade;  //���⼶��
extern uint32 memclk_ctl1;    //����memory ʱ��״̬
extern uint8 dma_chan;      //�����DMAͨ����
extern uint8 trans_mode;   //���ݴ����ʽ
extern uint8 pic_mode;     //ˢ��ģʽ
///���ϲ�ͳһ�ӿ�����
extern void *lcd_op_entry(void *param1,void *param2,void *param3, lcd_cmd_e cmd)__FAR__;

extern void set_window(region_t *rgn,void *null2,void *null3)__FAR__;  //�贰
extern void set_contrast(uint8 contrast_value,void *null2,void *null3);  //���öԱȶ�
extern void set_draw_mode(uint8 mode,void *null2,void *null3)__FAR__;    //����ģʽ
extern void standby_screen(bool standby_flag,void *null2,void *null3);   //����LCD��standby
extern void backlight_on_off(bool on_off,void *null2,void *null3)__FAR__;     //���ر������
extern void update_screen(region_t *rgn,void*null2,void *null3);   //������Ļ��ʾ
extern void buff_data_trans(uint8 *buff,uint16 pix_cnt,void *null3)__FAR__;  //����ʾbuffer�е���������
extern void get_buff_data(uint8 *buff,uint16 pix_cnt,void *null3);   //��ȡ���ϵ����ݣ���ŵ�buffer��
extern void invert_buff_data_trans(uint8 *buff,uint16 pix_cnt,void *null3);  //��buffer�е����ݷ�ɫ
extern void controller_init(lcd_mode_e lcd_mode,void *null2,void *null3)__FAR__;     //LCD��������ʼ��
extern void dma_set_counter(uint32 pix_cnt,uint16 data_width,void *null3);   //����DMA��������ݳ���
extern bool dma_start_trans(uint8 dma_ram, void *null2,void *null3);    //����DMA����
extern void dma_set_src_addr(uint32 addr0,uint32 addr1,uint32 addr2)__FAR__;
extern void mcu_set_JRAM_clk(void *null1, void *null2,void *null3);   //��RAM8��clk��ΪMCU clk
extern void restore_JRAM_clk(void *null1, void *null2,void *null3);  //�ָ�RAM8��clk

/*!
 * \brief
 *   �贰
 */
#define lcd_set_window(rgn)                          \
lcd_op_entry((void *)(rgn), (void *)(0), (void *)(0), LCD_SET_WINDOW)
/*!
 * \brief
 *   ���öԱȶ�
 */
#define lcd_set_contrast(value)                      \
lcd_op_entry((void *)(uint32)(value), (void *)(0), (void *)(0), LCD_SET_CONTRAST)
/*!
 * \brief
 *   ������ʾģʽ
 */
#define lcd_set_draw_mode(mode)                      \
lcd_op_entry((void *)(uint32)(mode), (void *)(0), (void *)(0), LCD_SET_DRAWMODE)
/*!
 * \brief
 *   ��Ļstandby����
 */
#define lcd_standby_screen(flag)                     \
lcd_op_entry((void *)(uint32)(flag), (void *)(0), (void *)(0), LCD_STANDBY_SCREEN)
/*!
 * \brief
 *   ��Ļ�������
 */
#define lcd_backlight_on_off(on_off)                 \
lcd_op_entry((void *)(uint32)(on_off), (void *)(0), (void *)(0), LCD_BACKLIGHT_ONOFF)
/*!
 * \brief
 *   ������Ļ����
 */
#define lcd_update_screen(rgn)                       \
lcd_op_entry((void *)(rgn), (void *)(0), (void *)(0), LCD_UPDATE_SCREEN)
/*!
 * \brief
 *   ��buffer�е���������
 */
#define lcd_buff_data_trans(buff,pix_cnt)            \
lcd_op_entry((void *)(buff), (void *)(uint32)(pix_cnt), (void *)(0), LCD_TRANS_BUFFDATA)
/*!
 * \brief
 *   ��ȡLCD���ϵ�����
 */
#define lcd_get_buff_data(buff,pix_cnt)              \
lcd_op_entry((void *)(buff), (void *)(uint32)(pix_cnt), (void *)(0), LCD_GET_BUFFDATA)
/*!
 * \brief
 *   ����ʾBUFFER�е����ݷ�ɫ
 */
#define lcd_invert_buff_data_trans(buff,pix_cnt)     \
lcd_op_entry((void *)(buff), (void *)(uint32)(pix_cnt), (void *)(0), LCD_INVERT_BUFFDATA)
/*!
 * \brief
 *   LCD���Ƴ�ʼ��
 */
#define lcd_controller_init(mode)                    \
lcd_op_entry((void *)(uint32)(mode), (void *)(0), (void *)(0), LCD_CONTROLLER_INIT)
/*!
 * \brief
 *   ����DMA��������ݿ��
 */
#define lcd_dma_set_counter(pix_cnt,data_width)      \
lcd_op_entry((void *)(uint32)(pix_cnt), (void *)(uint32)(data_width), (void *)(0), LCD_DMA_SET_COUNTER)
/*!
 * \brief
 *   ����DMA��ʼ����
 */
#define lcd_dma_start_trans(dma_ram)                 \
lcd_op_entry((void *)(uint32)(dma_ram), (void *)(0), (void *)(0), LCD_DMA_START_TRANS)
/*!
 * \brief
 *   ����DMAԴ��ַ
 */
#define lcd_dma_set_src_addr(addr0,addr1,addr2)      \
lcd_op_entry((void *)(uint32)(addr0), (void *)(uint32)(addr1), (void *)(uint32)(addr2), LCD_DMA_SET_SRC_ADDR)
/*!
 * \brief
 *   ����RAM8��CLK��MCU
 */
#define lcd_mcu_set_JRAM_clk()                       \
lcd_op_entry((void *)(0), (void *)(0), (void *)(0), LCD_SET_JRAM_CLK)
/*!
 * \brief
 *   �ָ�ԭ����CLK
 */
#define lcd_restore_JRAM_clk()                       \
lcd_op_entry((void *)(0), (void *)(0), (void *)(0), LCD_RESTORE_JRAM_CLK)


/*! \endcond */

#endif //_LCD_DRIVER_H
