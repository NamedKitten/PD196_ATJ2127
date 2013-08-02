/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2011-09-05        1.0              create this file
 *******************************************************************************/

#include "app_mengine.h"

#include "audio_device.h"

//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT		3

//globle variable
//�����м�����
void *mp_handle = NULL;

//����ļ�ϵͳid
int32 file_sys_id;

//����ģʽ
mengine_enter_mode_e mengine_enter_mode;

//������Ϣ
mengine_info_t mengine_info;

//���������ʼʱ��
uint32 g_intro_start;

//���music�ļ���׺����bitmap����
uint32 file_type_cfg;//

//ɾ����ǰ�ļ���ʶ
uint32 g_del_curfile;

//�ı��ļ�·����Ϣ��ʶ
uint32 g_change_path_flag = 0;

//��ʼ��ʱ���̷�
uint32 g_open_disk;

//��ʶaudbile�ļ�
uint16 g_is_audible = 0;

//audible �½��л���ʱ��
int8 g_chapter_timer = 0xff;

//audible ͼƬ��ⶨʱ��
int8 g_image_timer = 0xff;

//pos file buffer
uint8 g_audible_pos[16];

//�ļ���buffer
uint8 g_file_name[64];

#ifndef PC
//audbile info ptr
audible_para_t *g_audible_info_p = (audible_para_t *) 0x9fc2f000;
//sent to decoder audbile info ptr
audible_para_t *g_send_audible_p = (audible_para_t *) 0x9fc37000;
#else
//audbile info ptr
audible_para_t *g_audible_info_p;
//sent to decoder audbile info ptr
audible_para_t *g_send_audible_p;
#endif

//sd �ļ���
//uint8 g_sd_file_name[12];

//�ļ�·����Դ��Ϣ����,�ı�·����Ϣʱʹ��
fsel_type_e g_path_type;

//ϵͳ��������
//setting_comval_t g_setting_comval;
//comval_t g_comval;

//�м��״̬��ⶨʱ��
int8 g_status_timer;

//�м��״̬����־λ
uint8 g_check_status_flag;

//music engine��ʱ����
uint8 g_delay_time;

//audibleר��ͼƬ��Ŀ
uint16 g_image_num;

//�ؼ�����ֵ
app_result_e g_mengine_result;

//�������ñ����ṹ��ָ��
mengine_config_t *g_eg_cfg_p;

//����״̬�����ṹ��ָ��
mengine_status_t *g_eg_status_p;

//���沥����Ϣ�ṹ��ָ��
mengine_playinfo_t *g_eg_playinfo_p;

//˽����Ϣȫ�ֱ���
private_msg_t g_cur_msg;

//app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
app_timer_t mengine_app_timer_vector[APP_TIMER_COUNT];

#ifndef PC
OS_STK *ptos = (OS_STK *) AP_MENGINE_STK_POS;
#endif

INT8U prio = AP_MENGINE_PRIO;

//for audible test
#define AudibleExtNum 2

const uint8 audible_ext[AudibleExtNum][4] =
{ "AAX", "AA " };

/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \��ȡ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{

    //ϵͳ������ȡ
    //sys_vm_read(&g_setting_comval, VM_AP_SETTING, sizeof(setting_comval_t));
    //sys_vm_read(&g_comval, VM_AP_SETTING, sizeof(comval_t));

    //music engine ������ȡ
    sys_vm_read(&mengine_info.eg_config, VM_AP_MENGINE, sizeof(mengine_config_t));
    if (mengine_info.eg_config.magic != MAGIC_MENGINE)
    {
        mengine_info.eg_config.magic = MAGIC_MENGINE;
        //�ļ�ѭ��ģʽ
        mengine_info.eg_config.repeat_mode = FSEL_MODE_LOOPALL;
        //shuffle flag
        mengine_info.eg_config.shuffle_flag = 0;
        //����ͨ��ģʽ��0��������1��ֻ�����������2��ֻ�����������3��������������
        mengine_info.eg_config.chanel_select_mode = 0;
        //�ļ�ѡ������ �б�/�ղؼ�/audible/Ŀ¼
        //mengine_info.eg_config.fsel_type = FSEL_TYPE_PLAYLIST;
        mengine_info.eg_config.fsel_type = FSEL_TYPE_COMMONDIR;
        //����ʱ�䵥λ����
        mengine_info.eg_config.fade_out_time = 0;
        //����ʱ�䵥λ����
        mengine_info.eg_config.fade_in_time = 0;
        //�������
        mengine_info.eg_config.fff_step = 2;
        //audible�½�ģʽ
        mengine_info.eg_config.section_mod = 0;
        //��ǰ�ļ����
        mengine_info.eg_config.file_cnt.num = 0;
        //��ǰ�ļ��½�����
        mengine_info.eg_config.file_cnt.total = 0;
        //(��¼��ǰ�ļ���·����Ϣ/�б��ļ��Ĳ㼶��ϵ)
        libc_memset(&mengine_info.eg_config.location.dirlocation, 0, sizeof(musfile_location_u));

        //�ϵ���Ϣ
        libc_memset(&mengine_info.eg_config.bk_infor, 0, sizeof(mmm_mp_bp_info_t));
    }

    if (mengine_enter_mode == ENTER_START)
    {
        mengine_info.eg_config.fsel_type = FSEL_TYPE_COMMONDIR;
        //(��¼��ǰ�ļ���·����Ϣ/�б��ļ��Ĳ㼶��ϵ)
        libc_memset(&mengine_info.eg_config.location.dirlocation, 0, sizeof(musfile_location_u));
        mengine_info.eg_config.location.dirlocation.disk = (uint8) g_open_disk;
    }
    else
    {
        //����ļ���Ч��
        if (mengine_info.eg_config.location.dirlocation.disk == DISK_H)
        {
            if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
            {
                mengine_info.eg_config.fsel_type = FSEL_TYPE_COMMONDIR;
                libc_memset(&mengine_info.eg_config.location.dirlocation, 0, sizeof(musfile_location_u));
                mengine_info.eg_config.location.dirlocation.disk = DISK_C;
            }
        }

        //��Ҫ������ȡ�ļ���Ϣ
        mengine_info.eg_playinfo.cur_file_switch |= 0x01;
    }
    //mengine_info.eg_config.volume = g_comval.volume_current;
    g_image_num = 0xffff;

#ifdef PC
    g_audible_info_p = (audible_para_t *) (GET_REAL_ADDR(0x2f000));
    g_send_audible_p = (audible_para_t *) (GET_REAL_ADDR(0x37000));
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{
    /*����VM����*/
    //setting_comval_t temp_val;
    if (mengine_enter_mode != ENTER_ALARM)
    {
        //����Ӧ�ò������κα���
        sys_vm_write(&mengine_info.eg_config, VM_AP_MENGINE);
        //sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));
        //ע��ֻ����������Ҫ������ֵ���������ݾ����ı�
        //�����п���ǰ̨���µ����ݱ����渲��
        //temp_val.g_comval.volume_current = g_comval.volume_current;
        //sys_vm_write(&temp_val, VM_AP_SETTING);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool mengine_file_init(void)
 * \��ʼ���ļ�ѡ����,��λ���趨�ļ�or��ǰģʽ��һ���ɲ����ļ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
bool mengine_file_init(void)
{
    fsel_param_t init_param;//�ļ�ѡ������ʼ������
    uint8 exname[4];

    plist_location_t* loaction_ptr;
    fsel_type_e file_source;
    bool init_result = FALSE;
    uint8 loop_cnt = 0;
    mengine_config_t *cfg_ptr = &mengine_info.eg_config;

    //��ȡ��׺������
    //get_config(CFG_MUSIC_FILE_EXT,init_param.file_type_bit);

    loaction_ptr = &(cfg_ptr->location.plist_location);

    //�ļ�ѡ������ʼ���ӿڵĲ�������

    //�̷���Ϣ
    init_param.disk = loaction_ptr->disk;

    file_source = cfg_ptr->fsel_type;

    init_param.fsel_type = file_source;

    init_param.fsel_mode = (uint8)(cfg_ptr->repeat_mode | cfg_ptr->shuffle_flag);

    //�ж��Ƿ���audible
    for (loop_cnt = 0; loop_cnt < AudibleExtNum; loop_cnt++)
    {
        if (libc_strncmp(loaction_ptr->filename, audible_ext[loop_cnt], sizeof(loaction_ptr->filename)) == 0)
        {
            g_is_audible = TRUE;
            break;
        }
    }
    if (loop_cnt == AudibleExtNum)
    {
        g_is_audible = FALSE;
    }

#ifdef PC
    //ģ������֧��������audible�ļ�����
    //������е����������ͨ�ļ���֧
    g_is_audible = FALSE;
#endif

    //bitmap ����
    if (g_is_audible == TRUE)
    {
        init_param.file_type_bit = AUDIBLE_BITMAP;
        init_param.fsel_mode = FSEL_MODE_NORMAL;
        cfg_ptr->repeat_mode = FSEL_MODE_NORMAL;
        cfg_ptr->shuffle_flag = 0;
    }
    else
    {
        init_param.file_type_bit = MUSIC_BITMAP;
    }

    file_type_cfg = init_param.file_type_bit;

    //�ļ�ѡ������ʼ��
    file_sys_id = fsel_init(&init_param, 0);
    if (file_sys_id != -1)
    {
        //�����ϴ��˳����ļ�
        init_result = fsel_set_location(loaction_ptr, file_source);

        if (init_result == TRUE)
        {
            //�ȽϺ�׺��
            vfs_get_name(file_sys_id, exname, 0);

            if (libc_strncmp(exname, loaction_ptr->filename, sizeof(exname)) != 0)
            {
                init_result = FALSE;
            }
        }

        //����ʧ��
        if (init_result == FALSE)
        {
            //����ϵ���Ϣ
            libc_memset(&cfg_ptr->bk_infor, 0, sizeof(mmm_mp_bp_info_t));

            //��ȡ��ǰģʽ�µĵ�һ���ɲ����ļ�
            init_result = fsel_get_nextfile(loaction_ptr->filename);
            //�ҵ��ɲ����ļ�
            if (init_result == TRUE)
            {
                //��ȡ��ǰ�ļ���location��Ϣ
                init_result = fsel_get_location(loaction_ptr, file_source);
            }
            else
            {
                //û�пɲ����ļ�
                loaction_ptr->file_total = 0;
            }
        }
    }

    return init_result;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _app_init(void)
 * \��ʼ�����桢��ȡ������Ϣ����ʼ���ļ�ѡ�������м��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval          -1 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
int _app_init(void)
{
    int app_init_ret = 0;
    bool init_fsel_ret;
    char* mmm_name =
    { "mmm_mp.al" };

    //��ȡ������Ϣ (����vm��Ϣ)
    _load_cfg();

    //��ʼ��applib�⣬��̨AP
    applib_init(APP_ID_MENGINE, APP_TYPE_CONSOLE);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init(NULL);

    //��ʼ����ʱ��
    init_app_timers(mengine_app_timer_vector, APP_TIMER_COUNT);

    //�����м������
    sys_load_mmm(mmm_name, TRUE);//������
    //��ʼ���ļ���ȡ�ɲ����ļ�/�ϵ�
    init_fsel_ret = mengine_file_init();

    //��ʼ���м����
    app_init_ret = mmm_mp_cmd(&mp_handle, MMM_MP_OPEN, (unsigned int) NULL);
    if (app_init_ret == 0)
    {
        if (init_fsel_ret == TRUE)
        {
            _file_switch_info();
        }

        if (mengine_enter_mode == ENTER_START)
        {
            //ENTER_STARTģʽ��������PA,����ģʽ�ȵ����Ž���ˢ��
            //֮���ٿ���PA
            com_set_sound_out(TRUE, SOUND_OUT_START, NULL);
            g_eg_playinfo_p->cur_time = 0;
        }
        else
        {
            g_eg_playinfo_p->cur_time = (uint32)g_eg_cfg_p->bk_infor.bp_time_offset;
        }
    }

    g_delay_time = 2;
    //��һ�ν���������ѯ״̬���Ը���mengine����
    g_check_status_flag = TRUE;

    return app_init_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \�˳�app�Ĺ��ܺ���,����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    //�������˳�ǰ�޸Ŀ�״̬
    if (mengine_info.eg_config.location.plist_location.disk == DISK_H)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
        {
            //���ڿ���
            //��ǰ�̷��ǿ���,����״̬Ϊ��δ����״̬
            change_card_state(CARD_STATE_CARD_IN);
        }
        else
        {
            change_card_state(CARD_STATE_CARD_NULL);
        }

    }

    //�ر��ļ�ѡ����
    fsel_exit();

    //�ر������豸
    //mmm_mp_cmd(mp_handle, MMM_MP_AOUT_CLOSE, (unsigned int) NULL);
    //�ر��м���豸
    mmm_mp_cmd(mp_handle, MMM_MP_CLOSE, (unsigned int) NULL);

    //disable_dac();
    //disable_pa();
    com_set_sound_out(FALSE, SOUND_OUT_STOP, NULL);

    //ִ��applib���ע������
    applib_quit();

    //save config дvram
    _save_cfg();
    //ж�ؽ�������
    sys_free_mmm(TRUE);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc, const char *argv[])
 * \app��ں���
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{
    mengine_enter_mode = (mengine_enter_mode_e)(argc & 0xc0);

    //��alarm���룬�������޸�ΪENTER_START
    //alarm�������Ų���Ҫ֧�ֶϵ㲥�ţ�����Ҫ����location
    if (mengine_enter_mode == ENTER_ALARM)
    {
        mengine_enter_mode = ENTER_START;
    }

    g_open_disk = (uint8)(argc & 0x3f);

    g_eg_cfg_p = &mengine_info.eg_config;

    g_eg_status_p = &mengine_info.eg_status;

    g_eg_playinfo_p = &mengine_info.eg_playinfo;

    //��ʼ��
    //if (_app_init() == 1)
    _app_init();
    {
        g_mengine_result = mengine_control_block();//app���ܴ���z
    }
    //���»ָ�enter_mode
    mengine_enter_mode = (mengine_enter_mode_e)(argc & 0xc0);
    _app_deinit();//�˳�

    return g_mengine_result;
}
