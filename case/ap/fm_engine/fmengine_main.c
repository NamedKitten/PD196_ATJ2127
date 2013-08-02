/*******************************************************************************
 *                              US212A
 *                            Module: radio engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      mikeyang  2011-09-05        1.0              create this file
 *******************************************************************************/

#include "App_fmengine.h"

//globle variable
//FM ����״̬��Ϣ
Engine_Status_t g_fmengine_status;

//����������Ϣ
FMEngine_cfg_t g_fmengine_cfg;

//FM ģ������״̬
Module_Status_e g_module_status;

//FM ��������ջ
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FMENGINE_STK_POS;
#endif

//FM �������߳����ȼ�
INT8U prio = AP_FMENGINE_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 * \void fmengine_load_cfg(void)
 * \��ȡ������Ϣ
 * \param[in]    void  
 * \param[out]   none
 * \return       void 
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void fmengine_load_cfg(void)
{
    //��ȡFM ����������Ϣ
    sys_vm_read(&g_fmengine_cfg, VM_AP_FMENGINE, sizeof(FMEngine_cfg_t));
    if (g_fmengine_cfg.magic != MAGIC_USER1) //0x55AA
    {
        g_fmengine_cfg.magic = MAGIC_USER1;
        g_fmengine_cfg.fm_threshold = (uint8) com_get_config_default(FMENGINE_AP_ID_SEEKTH); //THRESHOLD_DEFAULT;
        g_fmengine_cfg.paddv_mode = (PA_DDV_Mode_e) com_get_config_default(FMENGINE_AP_ID_PAMODE); //PA_MODE;
        /*����VM����*/
        sys_vm_write(&g_fmengine_cfg, VM_AP_FMENGINE);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void fmengine_save_cfg(void)
 * \����������Ϣ
 * \param[in]    void  
 * \param[out]   none
 * \return       void 
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void fmengine_save_cfg(void)
{
    /*����VM����*/
    sys_vm_write(&g_fmengine_cfg, VM_AP_FMENGINE);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _app_init(void)
 * \��ȡ������Ϣ����װFM  ����
 * \param[in]    void  
 * \param[out]   none
 * \return       int the result
 * \retval           0 sucess
 * \retval          -1 failed
 * \note
 */
/*******************************************************************************/
int fmengine_init(void)
{
    int app_init_ret = 0;

    //��ȡ������Ϣ (����vm��Ϣ)
    fmengine_load_cfg();

    //��ʼ��applib�⣬��̨AP
    applib_init(APP_ID_FMENGINE, APP_TYPE_CONSOLE);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init(NULL);

#ifndef PC
    //��װfm  ����
    app_init_ret = sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
#else
    app_init_ret = 0;
#endif
    //������װ�ɹ�
    if (app_init_ret == 0)
    {
        //ģ����δ��ʼ�����账��MSG_FMENGINE_INIT  ��Ϣ����г�ʼ��
        g_module_status = MODULE_NO_INIT;
    }

    return app_init_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fmengine_deinit(void)
 * \�˳�app�Ĺ��ܺ���,����������Ϣ
 * \param[in]    void 
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool fmengine_deinit(void)
{
    //ִ��applib���ע������
    applib_quit();

    //save config дvram
    fmengine_save_cfg();

#ifndef PC
    //ж��fm ����
    sys_drv_uninstall(DRV_GROUP_FM);
#endif
    return TRUE;
}

/******************************************************************************/
/*!
 * \Description: FM Engine ap entry function
 * \int main(int argc, const char *argv[])
 * \app��ں���
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e result = RESULT_NULL;
    //��ʼ��
    if (fmengine_init() == 0)
    {
        //����հ�װʱ��Ĭ��Ϊ����
        change_engine_state(ENGINE_STATE_PLAYING);
        result = fmengine_control_block();
    }
    fmengine_deinit();//�˳�

    return result;
}

