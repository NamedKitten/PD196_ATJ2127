/*! \cond MMM_VP_API*/
/********************************************************************************
 *                            GL5110
 *                            Module: videoplay.c
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>           <version >             <desc>
 *   yonglu,bruceding    2011-8-25            1.0             build this file
 ********************************************************************************/
/*!
 * \file     mmm_vp.h
 * \brief    �������ֲ�����Ӧ�ò�Ľӿڼ�����
 * \author   yonglu,bruceding
 * \version 1.0
 * \date  2011-8-25
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_VP_H__
#define __MMM_VP_H__

#ifdef __cplusplus
extern "C"
{
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

/*! \cond MMM_VP_API*/
/*!
 * \brief
 *      �ṩ��Ӧ�ò�Ĺ�����������
 */
typedef enum {
	/*! ����Ƶ�м�� */
	MMM_VP_OPEN = 0,
	/*! �ر���Ƶ�м�� */
	MMM_VP_CLOSE,
	/*! ���ò��ŵ��ļ��� */
	MMM_VP_SET_FILE,
	/*! ��ȡ��ý���ļ�����Ϣ */
	MMM_VP_GET_MEDIA_INFO,
	/*! ��ʾ������ */
	MMM_VP_PROGRESSBAR,
	/*! ��ʾ������ */
	MMM_VP_VOLUMEBAR,
	/*! ȡ�������� */
	MMM_VP_NOPROGRESS,
	/*! ȡ��������*/
	MMM_VP_NOVOLUME,
	/*! ���� */
	MMM_VP_PLAY,
	/*! ֹͣ */
	MMM_VP_STOP,
	/*! ��ͣ */
	MMM_VP_PAUSE,
	/*! ��� */
	MMM_VP_FAST_FORWARD,
	/*! ���� */
	MMM_VP_FAST_BACKWARD,
	/*! ��ȡ������Ϣ */
	MMM_VP_GET_ERRNO,
} mmm_vp_cmd_t;

/*!
 *  \brief
 *      cmd ����ֵ
 */

typedef enum {
	/*! �޴� */
	MMM_CMD_OK,
	/*! ���ļ�β */
	MMM_CMD_FILEEND,
	/*! ���ļ�ͷ */
	MMM_CMD_FILEHEAD,
	/*! ������� */
	MMM_CMD_ERR,
} mmm_vp_cmd_return_t;

/*!
 *  \brief
 *      ��Ƶ�м�������
 */

typedef enum {
	/*! �޴� */
	MMM_VP_NO_ERR,
	/*! ���ļ�ʧ�� */
	MMM_VP_ERR_OPEN_FILE,
	/*! �ļ���ʽ��֧�� */
	MMM_VP_ERR_FILE_NOT_SUPPORT,
	/*! ������� */
	MMM_VP_ERR_DECODER,

} mmm_vp_errno_t;

/*!
 *  \brief
 *  ö����Ƶ����
 */
typedef enum {
	VIDEO_AMV = 0, VIDEO_AVI
} video_type_t;

/*!
 *  \brief
 *     ��Ƶ�ļ�����Ϣ
 */
typedef struct {
	/*! ��Ƶ���� */
	video_type_t type;
	/*! �ļ�����ʱ�䣨ms�ƣ� */
	unsigned int total_time;
	/*! ��Ƶ�ļ���֡�ʣ�fps�� */
	unsigned int frame_rate;
	/*! ��Ƶ�Ŀ�pixels�� */
	unsigned int width;
	/*! ��Ƶ�ĸߣ�pixels�� */
	unsigned int height;
	/*! �ļ����� */
	unsigned int file_len;
} mmm_vp_file_info_t;

/*!
 *  \brief
 *     ��ʾ����
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
 *  \brief
 *     ���Լ�������Ϣ
 */
typedef struct {

	/*! ���� */
	unsigned int fadeout;
	/*! ������ ��17bit 0��ʾ�����ͣ�1��ʾ��С�ͣ���16bit ��ʾλ�ã�0���£�1���ϣ���16bits��ʾ�߶ȣ�*/
	unsigned int progressbar;
	/*! ������ ��17bit 0��ʾ�����ͣ�1��ʾ��С�ͣ���16bit ��ʾλ�ã�0���£�1���ϣ���16bits��ʾ�߶ȣ�*/
	unsigned int volumebar;
	/*! ������ */
	unsigned int softvolume;
	/*! ���� */
	unsigned int reserved;
} ctlinfor_t;

/*!
 *  \brief
 *     ��Ƶ�ϵ����Ϣ
 */
typedef struct {
	/*! ���ļ��е�λ�� */
	unsigned int bpbyte; // breakpointer  flash counter
	/*! ֡�� */
	unsigned int framecnt;
	/*! ʱ�䣨��λ�룩 */
	unsigned int time;
} videobpinfor_t;//

/*!
 *  \brief
 *     Ӧ�ò�Ŀ�����Ϣ
 */
typedef struct {
    /*! ���� */
    unsigned int mountindex;	
	/*! �ļ��� */
	unsigned char *filename;
	/*! ������Ϣ */
	ctlinfor_t *plcminfor;
	/*! ��Ƶ�ϵ����Ϣ */
	videobpinfor_t *pbreakinfor;
	/*! ����*/
	wregion_t *pregion1;
	/*! ������*/
	wregion_t *pregion2;
	/*�н�����������������ʾ����*/
	wregion_t *pregion3;

} mmm_vp_userparam_t;

/*!
 *  \brief
 *     �ӿں�������
 *     �ӿں���������kernel_api.S�����������Ժ�ʹ�ã�����API����api.h��������ͳһ����
 */
extern int mmm_vp_cmd(void *vp_handle, unsigned int cmd, unsigned int param)__FAR__;

#ifdef __cplusplus
}
#endif

#endif

/*! \endcond*/

