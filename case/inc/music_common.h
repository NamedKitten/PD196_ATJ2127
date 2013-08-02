/********************************************************************************
 *                            Module: music_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-08 15:00     1.0             build this file
********************************************************************************/
#ifndef __MUSIC_COMMON_H__
#define __MUSIC_COMMON_H__

#include "psp_includes.h"
#include "enhanced.h"
#include "mmm_mp.h"

#define MAX_MUSIC_FILE_NUMS  20    //���4000�׸���

#define BM_HEADER_SECTOR_NUM (1)   //������ռ�õ�������

#define BM_DATA_SECTOR_NUM       ((MAX_MUSIC_FILE_NUMS + 1) / 2) //������ռ�õ�������

/*!
 * \brief
 *  ��ǰ����״̬
 */
typedef enum
{
	/*! ֹͣ */   
    StopSta  = 0, 
	/*! ��ͣ */       
    PauseSta, 
	/*! ���� */     
    PlaySta, 
	/*! AB���� */     
    PlayAB,
	/*! ����� */     
    PlayFast
} play_status_e;

/*!
 * \brief
 *  AB����״̬
 */
typedef enum
{
	/*! ��AB����״̬ */ 
    PlayAB_Null,
	/*! ��A��״̬ */     
    PlayAB_A,
	/*! ��B��״̬ */       
    PlayAB_B,
	/*! AB�������״̬ */      
    PlayAB_Do   
} ab_status_e;

/*!
 * \brief
 *  �����״̬
 */
typedef enum
{   
	/*! �������� */  
    FBPlay_Null,
	/*! ���״̬ */      
    FFPlaySta, 
	/*! ����״̬ */      
    FBPlaySta 
} fast_status_e;

/*!
 * \brief
 *  ������Ϣ
 */
typedef enum
{
	/*! û�д��� */ 
    EG_ERR_NONE = 0,        
    /*! ���ļ�ʧ�� */ 
    EG_ERR_OPEN_FILE,    
    /*! �ļ���ʽ��֧�� */ 
    EG_ERR_NOT_SUPPORT,    
    /*! ������� */ 
    EG_ERR_DECODER_ERROR,   
    /*! ��license�����Ŵ����������ǲ���ʱ���Ѿ�����  */ 
    EG_ERR_NO_LICENSE,
    /*! DRMʱ�Ӵ���  */     
    EG_ERR_SECURE_CLOCK,
    /*! DRM��Ϣ����  */  
    EG_ERR_DRM_INFO    
} eg_err_e;

/*!
 * \brief
 *  ����״̬
 */
typedef struct
{
    /*! ����״̬  */ 
    play_status_e play_status;
    /*! AB����״̬  */ 
    ab_status_e ab_status;
    /*! �����״̬  */ 
    fast_status_e fast_status;
    /*! ����״̬,��ȡ�����  */ 
    eg_err_e err_status;
} mengine_status_t;

/*!
 * \brief
 *  ��ǰ�����ļ���Ϣ
 */
typedef struct
{
    /*! ��ǰʱ�� */
    uint32 cur_time;
    /*! ������ */
    uint32 cur_bitrate;
    /*! audibleͼƬλ��ƫ�� */
    uint32 cur_imag_pos;
    /*! audibleͼƬ���ڱ�־(�Ƿֱ�ͼ) */
    uint8 cur_imag_flag;
    /*! ��ʴ��ڱ�־ */
    uint8 cur_lyric;
    /*! �ļ��л���־ bit0: �����Ƿ��л� bit1:audible�½��Ƿ��л�  */
    uint8 cur_file_switch;
} mengine_playinfo_t;

/*!
 * \brief
 *  music�ļ���������ż���
 */
typedef struct
{
    /*! ��ǰ�ļ���� */
    uint16 num;
    /*! ��ǰ�ļ����� */
    uint16 total;
} musfile_cnt_t;

/*!
 * \brief
 *  ����music�������ģʽ
 */
typedef enum
{
    /*! ѡ��������� */
    ENTER_START = 0x00,
    /*! ֹͣ */    
    ENTER_RESUME_PAUSE = 0x40, 
    /*! �ϵ����� */  
    ENTER_RESUME = 0x80, 
    /*! alarm��������  */      
    ENTER_ALARM = 0xc0 
} mengine_enter_mode_e;

//ѭ��ģʽ
typedef enum
{
    LoopNor, //��ͨѭ��
    LoopOne, //ѭ���ŵ�ǰ��
    LoopAll, //ѭ������
    Intro, //�������
    LoopOneDir, //ѭ���ŵ�ǰĿ¼
    LoopDir, //˳�򲥷�Ŀ¼
    MaxRepeatMod
} repeat_mode_e;

/*!
 * \brief
 *  audible�������ܽṹ�嶨��
 */
typedef struct
{
    uint16 magic;
    //uint8 title[30];
    /*! �ϵ���Ϣ */
    mmm_mp_bp_info_t bk_infor;
    /*! ·����Ϣ */
    file_path_info_t locat_info;
} audible_resume_info_t;


/*!
 * \brief
 *  audible�л�ģʽ
 */
typedef enum
{
    /*! �����л�ģʽ  */ 	
    SWITCH_TITLE = 0,
    /*! �½��л�ģʽ  */ 	    
    SWITCH_SECTION  
} audible_switch_mode_e;

/*!
 * \brief
 *  ��ǩͷ����Ϣ
 */
typedef struct
{
    uint16 magic; //ħ��
    uint16 total_index; //��ǩ����
    uint16 first_index;
    uint16 last_index;
    uint16 del_total; //��ɾ����������ǩ����
    //uint32 last_access_cluster_no; //���һ�η�����ǩ��cluster_no
    //uint32 last_access_dir_entry; //���һ�η�����ǩ��dir_entry
} music_bm_head;


/*!
 * \brief
 *  �������ñ����ṹ�嶨��
 */ 
typedef struct
{
    /*! ħ�� */ 	
    uint16 magic; 
    /*! ����ֵ*/     
    uint8 volume; 
    /*! shuffle ���� ȡ��4bit*/      
    uint8 shuffle_flag; 
    /*! repeat mode*/      
    fsel_mode_e repeat_mode; 
    /*! �����ٶ� */     
    uint8 play_rate; 
#if 0
    /*! ��������  */      
    uint8 replaytimes; 
    /*! �������  */     
    uint8 replaygap; 
#endif
    /*! �����ֽ� */  
    uint8 reserved[2];
    /*! ����ͨ��ģʽ��0��������1��ֻ�����������2��ֻ�����������3�������������� */     
    uint8 chanel_select_mode;
    /*! �ļ�ѡ������ �б�/�ղؼ�/audible/Ŀ¼ */      
    fsel_type_e fsel_type;
    /*! audible�½�ģʽ */       
    audible_switch_mode_e section_mod;
    /*! ������� */     
    uint8 fff_step;
    /*! ����ʱ�䵥λ���� */       
    uint32 fade_out_time;
    /*! ����ʱ�䵥λ���� */      
    uint32 fade_in_time;
    /*! �ļ���ż����� */     
    musfile_cnt_t file_cnt; 
    /*! wav/mp3/wma��Ч���� */     
    mmm_mp_eq_info_t eq_info; 
    /*! ������ʽ��Ч����*/    
    mmm_mp_eq_info_t eq_info_other; 
    /*! ��¼��ǰ�ļ���·����Ϣ/�б��ļ��Ĳ㼶��ϵ*/      
    musfile_location_u location; 
    /*! �ϵ���Ϣ*/        
    mmm_mp_bp_info_t bk_infor; 
} mengine_config_t;



#endif //__MUSIC_COMMON_H__
