/*! \cond MMM_ID_API*/
/********************************************************************************
 *                              5110
 *                            Module: image_decode
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>       <time>           <version >             <desc>
 * yonglu,bruceding    2011-08-27           1.0             build this file
 ********************************************************************************/
/*!
 * \file     mmm_id.h
 * \brief    ����ͼƬ�����м��ṩ�Ľӿڼ�����
 * \author   bruceding
 * \version  1.0
 * \date  2011/09/19
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_ID_H__
#define __MMM_ID_H__

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief
 *      ����ϵͳͷ�ļ�
 */
#ifdef WIN32
/* VC */
#define __FAR__
#else
/* MIPS */
#include <psp_includes.h>
#endif

/*! \cond MMM_ID_API*/
/*!
 *  \brief
 *     �ṩ��Ӧ�ò��ͼƬ��������
 */
typedef enum {
	/*! ��ͼƬ�м�� */
	MMM_ID_OPEN = 0,
	/*! �ر�ͼƬ�м�� */
	MMM_ID_CLOSE,
	/*! ����ר��ͼƬ��Ϣ*/
	MMM_ID_SET_ALBUM_INFO,
	/*! ���ò��ŵ��ļ��� */
	MMM_ID_SET_FILE,
	/*! ��ȡ��ý���ļ�����Ϣ */
	MMM_ID_GET_MEDIA_INFO,
	/*! ��ȡ������Ϣ*/
	MMM_ID_GET_DECODER_STATUS,
	/*! ͼƬ����ͼ����*/
	MMM_ID_BROWSE,
	/*! ͼƬ����*/
	MMM_ID_DECODE,
	/*! ȡ��ͼƬ����*/
	MMM_ID_ABORT_DECODE,
} mmm_id_cmd_t;

/*!
 *  \brief
 *      ͼƬ����״̬
 */
typedef enum {
	/*! ����״̬ */
	MMM_ID_ENGINE_ERROR = -1,
	/*! ����״̬ */
	MMM_ID_ENGINE_NORMAL
} mmm_id_engine_t;

/*!
 *  \brief
 *     �м��ͼƬ����״̬
 */
typedef enum {
	/*! ����״̬*/
   MMM_ID_BUSY = 10,
	/*! ���ڿ���״̬*/
	MMM_ID_FREE,
	/*! ���ڴ���״̬*/
	MMM_ID_ERROR
} mmm_id_status_flag_t;

/*!
 * \brief
 *      ͼƬ�������
 */
typedef struct {
	/*! ����״̬*/
	mmm_id_status_flag_t status;
	/*! �����*/
	unsigned int err_no;
	/*! �ļ��Ƿ�ﵽβ���ı�־*/
	unsigned int end_of_file;
} mmm_id_status_t;

/*!
 * \brief
 *      ͼƬ������Ϣ
 */
typedef struct {
	/*! ͼƬ��������*/
	char datetime[20];
} mmm_id_exif_t;

/*!
 *  \brief
 *  ö��ͼƬ����
 */
typedef enum {
	/*! JPEG ͼƬ*/
	IMAGE_JPG = 0,
	/*! BMP ͼƬ*/
	IMAGE_BMP,
	/*! GIF ͼƬ*/
	IMAGE_GIF,
	/*! ��֧��ͼƬ*/
	UNKOWN,
} image_type_t;

/*!
 * \brief
 *      ͼƬ��Ϣ
 */
typedef struct {
	/*! ͼƬ��ʽ*/
	image_type_t file_format;
	/*! ͼƬ���*/
	unsigned int width;
	/*! ͼƬ�߶�*/
	unsigned int height;
	/*! ͼƬ������Ϣ*/
	mmm_id_exif_t exif;
} mmm_id_file_info_t;

/*!
 * \brief
 *      ������Ϣ�ṹ��
 */
typedef struct {
	/*! x���� */
	unsigned short x;
	/*! y���� */
	unsigned short y;
	/*! �� */
	unsigned short w;
	/*! �� */
	unsigned short h;
} wregion_t;

/*!
 * \brief
 *      ������Ϣ�ṹ��
 */
typedef struct {
	/*! ���� */
	unsigned int mountindex;
	/*! �ļ��� */
	unsigned char *filename;
	/* ͼƬ���ļ��е�ƫ���� */
	unsigned int data_offset;
	/* ��Ļ�ϵ� ��ͼ��*/
	wregion_t *pregion1;
	/* ʵ���ϵ� ��ͼ�� */
	wregion_t *pregion2;
	/*! ͼƬ��ʽ*/
	image_type_t file_format;
	/* ���ŵı�־λ bit0 auto rotation (0 no,1 yes) ��bit1�� 0��appΪ�������������1��appΪ����ͼ���*/
	unsigned int playflag;
	/* ���صı�־λ  0bit winh2v   (0 no,1 yes)*/
	unsigned int returnflag;
	/* ������״̬λ*/
	mmm_id_status_flag_t dec_status;	      
} ColorLcmInfor;

/*!
 *  \brief
 *     �ӿں�������
 *     �ӿں���������kernel_api.S�����������Ժ�ʹ�ã�����API����api.h��������ͳһ����
 */
extern int mmm_id_cmd(void *id_handle,mmm_id_cmd_t cmd,unsigned int param)__FAR__;

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_ID_H_
