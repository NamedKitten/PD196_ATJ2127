/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 21:49     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _miscellaneous.c
 * \brief    common �����
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ���������ȡĬ�� setting_comval ֵ����������ӳ�䣬��Ļ�������ã���ȡ��ص������ȵ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2011-9-19
*******************************************************************************/

#include "common_ui.h"

//�͵��ѹ 3.3V
#define BATLOW_VEL      0x51

//��ص����ȼ���Ӧ ADC �ο�ֵ
const uint8 battery_grade_vel[BATTERY_GRADE_MAX+1] = 
{
    0,          //���
    BATLOW_VEL, //�͵�        ���� 0
    0x59,       //<= 3.5V     �ո��
    0x5e,       //3.5-3.6V    һ���
    0x60,       //3.6-3.65V   �����
    0x68,       //3.65-3.85V   ����磻3.85V����   �����
};

//�������ȵȼ�ӳ���
const uint8 lightness_map_data[VALUE_LIGHTNESS_MAX+1] = 
{
    1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 13, 15
};
/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ȡ��ص�����ת��Ϊ�����ȼ���
 * \param[in]    now �Ƿ�ʱ��ȡ����ʱ��ȡ����������7�Σ�Ȼ����м���
 * \param[out]   none 
 * \return       uint8 ���ص�ص����ȼ���1 ~ BATTERY_GRADE_MAX Ϊ������0��ʾ�͵硣
 * \ingroup      misc_func
 * \note 
 * \li  �͵�������Ϊ30�롣
*******************************************************************************/
uint8 com_get_battery_grade(bool now)
{
    uint16 battery_total = 0;
    uint8 i, battery_value;
    
    if(now == TRUE)
    {
        //��������7��
        for(i = 0; i < BAT_SAMPLE_TIME; i++)
        {
            g_bat_sample[i] = act_readb(BATADC_DATA);//0-6bit��Ч
        }
        
        g_sys_counter.battery_counter = 20;//���ַ�ʽ�õ����ȶ���ʱADC��Ҫ����������ȡ�ȶ�ƽ��ADC
    }
    
    for(i = 0; i < BAT_SAMPLE_TIME; i++)//������7��ȡƽ��ֵ
    {
        battery_total += g_bat_sample[i];
    }
    battery_value = (uint8)(battery_total/BAT_SAMPLE_TIME);
    if((battery_total%BAT_SAMPLE_TIME) >= BAT_SAMPLE_CARRY)
    {
        battery_value++;
    }
    
    g_bat_index = 0;
    
    for(i = BATTERY_GRADE_MAX; i > 0; i--)
    {
        if(battery_value >= battery_grade_vel[i])
        {
            return i;
        }
    }
    
    return 0;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ����Ӳ������Աȶȣ������ȡ�
 * \param[in]    lightness ϵͳ��������ֵ
 * \param[out]   none 
 * \return       none
 * \ingroup      misc_func
 * \note 
*******************************************************************************/
void com_set_contrast(uint8 lightness)
{
    uint8 map_value;
    
    if(lightness > VALUE_LIGHTNESS_MAX)
    {
        map_value = 0;
    }
    else
    {
        map_value = lightness_map_data[lightness];
    }
    
    lcd_set_contrast(map_value);
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���� GUI ��ʾ����С�������������ת������򣩡�
 * \param[in]    direct UI ��ʾ���򣬼� gui_direct_e ����
 * \param[out]   none 
 * \return       none
 * \ingroup      misc_func
 * \note 
*******************************************************************************/
void com_set_gui_direction(gui_direct_e direct)
{
    g_gui_direct = direct;
    if(g_gui_direct == GUI_DIRECT_NORMAL)
    {
        ui_set_screen_direction(0);
    }
    else
    {
        ui_set_screen_direction(1);
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ��ȡ GUI ��ʾ����С�������������ת������򣩡�
 * \param[in]    none
 * \param[out]   none 
 * \return       gui_direct_e
 * \retval           ���� GUI ��ʾ���򣬼� gui_direct_e ���塣
 * \ingroup      misc_func
 * \note 
*******************************************************************************/
gui_direct_e com_get_gui_direction(void)
{
    return g_gui_direct;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    ���� sys_comval ָ�룬�Ա� common ����ϵͳ�����
 * \param[in]    comval ȫ��ϵͳ��������ָ��
 * \param[out]   none 
 * \return       none
 * \ingroup      misc_func
 * \note 
 * \li  ǰ̨Ӧ���ڽ���ʱ������øýӿڣ����� common ģ�齫�޷��������С�
*******************************************************************************/
void com_set_sys_comval(comval_t *comval)
{
    sys_comval = comval;
}

/*! \endcond */
