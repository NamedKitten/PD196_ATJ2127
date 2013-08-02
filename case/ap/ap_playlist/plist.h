/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     en_lrc.h
 * \brief    ���ݽṹ�ͳ�������
 * \author   liminxian
 * \version 1.0
 * \date  2011/09/15
 *******************************************************************************/

#ifndef _plist_h

#define _plist_h

#include <typeext.h>
#include <enhanced.h>

#include "psp_includes.h"
#include "case_include.h"
#include "playlist_sty.h"
#include "playlist_res.h"

//ϵͳĬ�ϴ����ļ�ʹ�ó���

//#define USE_83NAME_CREAT  //�����ļ�ʹ�ö�������

//ϵͳĬ�������б��ļ�LIB,debugʱ�ȹر�
#define  HIDE_FILE_OPT

//APP����
#define APP_TIMER_COUNT		1
#define APP_TIMER_TOTAL (COMMON_TIMER_COUNT + APP_TIMER_COUNT)

//�б�����

#define SECTOR_SIZE         512

//�б�֧�����ĸ���
#define PLIST_MUSIC_MAX     4000
#define PLIST_AUDIBLE_MAX   1000
#define PLIST_VIDEO_MAX     1000
#define PLIST_PICTURE_MAX   1000
#define PLIST_EBOOK_MAX     1000


//�б�Ŀռ�(��λ��byte)
#define MUSIC_LIB_SIZE     ((PLIST_MUSIC_MAX/2 + PLIST_MUSIC_MAX/4) * SECTOR_SIZE)
#define AUDIBLE_LIB_SIZE   ((PLIST_AUDIBLE_MAX/2 + PLIST_AUDIBLE_MAX/4) * SECTOR_SIZE)
#define VIDEO_LIB_SIZE     ((PLIST_VIDEO_MAX/2 + PLIST_VIDEO_MAX/4) * SECTOR_SIZE)
#define PICTURE_LIB_SIZE   ((PLIST_PICTURE_MAX/2 + PLIST_PICTURE_MAX/4) * SECTOR_SIZE)
#define EBOOK_LIB_SIZE     ((PLIST_EBOOK_MAX/2 + PLIST_EBOOK_MAX/4) * SECTOR_SIZE)
//��LIST �� DAT�ϳ�һ���ļ�
#define M3U_LIB_SIZE       (M3U_LIST_SIZE + M3U_DAT_SIZE)

#define PLIST_SAVE_MAX      2048
#define PLIST_GENRE_MAX     1024

#define TITLE_CMP_LENGTH    16
#define ALBUM_CMP_LENGTH    8
#define ARTIST_CMP_LENGTH   8
#define GENRE_CMP_LENGTH    8
#define TRACK_CMP_LENGTH    1

//#define PLIST_FILE_SIZE        256

#define INDEX_BUFFER_SIZE      0x2000//8K
#define FILE_INFO_BUF_SIZE     0x1000//4k ,���뱣֤����4K���ſ���2048����RAM����
#define TITLE_BUF_SIZE         0x8000//32K
#define ALBUM_BUF_SIZE         0x4000//16K
#define ARTIST_BUF_SIZE        0x4000//16K
#define GENRE_BUF_SIZE         0x2000//8K ,Ϊ֧��16Kpage����С��8K����1024���ͻ���
#define TRACK_BUF_SIZE         0x0800//2K
#define TITLE_VRAM_SIZE         (2 * TITLE_BUF_SIZE) //64K
#define ALBUM_VRAM_SIZE         (2 * ALBUM_BUF_SIZE) //32K
#define ARTIST_VRAM_SIZE        (2 * ARTIST_BUF_SIZE)//32K
#define GENRE_VRAM_SIZE         (2 * GENRE_BUF_SIZE * 2) //32K
#define TRACK_VRAM_SIZE         (2 * TRACK_BUF_SIZE) //4K
//0---GERN->ARTIST->ALBUM->TITLE
//1---GERN->ALBUM->TITLE
#define GENRE_LIST_MODE        0

//find file mode
//#define ENHANCED_FIND_MODE

/*!����playlistӦ�õ�����ջ�����߳����ȼ�*/
#define    AP_PLAYLIST_STK_POS  AP_FRONT_LOW_STK_POS
#define    AP_PLAYLIST_PRIO     AP_FRONT_LOW_PRIO

#ifdef PC
#define PLIST_SYS_DEF_SPACE
#endif //#ifdef PC

#ifdef  PLIST_SYS_DEF_SPACE

extern uint8 sys_vram_space[0x20000];//256k
extern uint8 sys_sdram_space[0x20000];//128k

#define SYS_VRAM_ADDRESS   VM_PLAYLIST_INFO//sys_vram_space
#define SYS_SRAM_ADDRESS   sys_sdram_space

#else //#ifdef PLIST_SYS_DEF_SPACE
#define SYS_VRAM_ADDRESS   VM_PLAYLIST_INFO
#define SYS_SRAM_ADDRESS   0x9fc28000

#endif//#ifdef PLIST_SYS_DEF_SPACE
//������ҪVRAM�ռ�  168K

//VRAM �Ĵ洢��ַ�ռ�ֲ�
#define PLIST_VRAM_BASE_ADDR         SYS_VRAM_ADDRESS

#define TITLE_VRAM_ADDR       (PLIST_VRAM_BASE_ADDR + 0x0000)       //64K
#define ALBUM_VRAM_ADDR       (TITLE_VRAM_ADDR  + TITLE_VRAM_SIZE)  //32K
#define ARTIST_VRAM_ADDR      (ALBUM_VRAM_ADDR  + ALBUM_VRAM_SIZE)  //32K
#define GENRE_VRAM_ADDR       (ARTIST_VRAM_ADDR + ARTIST_VRAM_SIZE) //32K
#define TRACK_VRAM_ADDR       (GENRE_VRAM_ADDR  + GENRE_VRAM_SIZE)  //4K
//���¸���TITLE_VRAM_ADDR�Ŀռ�
#define TITLE_SORT_INDEX_VM_ADDR    (TITLE_VRAM_ADDR + 0*INDEX_BUFFER_SIZE) //8K
#define ALBUM_SORT_INDEX_VM_ADDR    (TITLE_VRAM_ADDR + 1*INDEX_BUFFER_SIZE) //8K
#define ARTIST_SORT_INDEX_VM_ADDR   (TITLE_VRAM_ADDR + 2*INDEX_BUFFER_SIZE) //8K
#define TIDY_BUF_A_VM_ADDR          (TITLE_VRAM_ADDR + 3*INDEX_BUFFER_SIZE) //8K
#define TIDY_BUF_B_VM_ADDR          (TITLE_VRAM_ADDR + 4*INDEX_BUFFER_SIZE) //8K
//ɨ���ļ�ʱ��SRAM �Ĵ洢��ַ�ռ�ֲ�  SRAM�ռ�95K
#define PLIST_DATA_BASE_ADDR         SYS_SRAM_ADDRESS

#define FILE_INFO_ADDR        (PLIST_DATA_BASE_ADDR + 0x0000)          //4K
#define TITLE_DATA_ADDR       (FILE_INFO_ADDR   + FILE_INFO_BUF_SIZE)  //32K
#define ALBUM_DATA_ADDR       (TITLE_DATA_ADDR  + TITLE_BUF_SIZE)      //16K
#define ARTIST_DATA_ADDR      (ALBUM_DATA_ADDR  + ALBUM_BUF_SIZE)      //16K
#define GENRE_DATA_ADDR       (ARTIST_DATA_ADDR + ARTIST_BUF_SIZE)     //16K
#define TRACK_DATA_ADDR       (GENRE_DATA_ADDR  + GENRE_BUF_SIZE)     //2K
//NOTE,TRACK_NUM��Ϊ�ռ����⣬ֻ���� 1 BYTE


//��ΪPLAYLISTʹ����nandĬ�ϵ�cache�����������ṩ8K��nandʹ��
#define NAND_CACHE_ADDR         0x9fc3bc00 // playlist����ʱnandʹ�õ�cache��ַ,֧��16K page
#define NAND_DEFAULT_ADDR       0x9fc32000 // nand Ĭ�ϵ�cache��ַ
//����ʱ��SRAM��buffer��ַ�ռ�ֲ�
#define FILE_INDEX_BUF_ADDR        (PLIST_DATA_BASE_ADDR + 0*INDEX_BUFFER_SIZE)//8K
#define SORT_INDEX_BUF_ADDR        (PLIST_DATA_BASE_ADDR + 1*INDEX_BUFFER_SIZE)//8K
//#define INDEX_PARENT_BUF_ADDR      (PLIST_DATA_BASE_ADDR + 2*INDEX_BUFFER_SIZE)//8K ��Ϊ�ռ临�ã����̶�
#define TIDY_BUF_A_ADDR            (PLIST_DATA_BASE_ADDR + 2*INDEX_BUFFER_SIZE)//8K
#define COUNT_BUF_A_ADDR           (PLIST_DATA_BASE_ADDR + 3*INDEX_BUFFER_SIZE)//8K
#define TIDY_BUF_B_ADDR            (PLIST_DATA_BASE_ADDR + 4*INDEX_BUFFER_SIZE)//8K
#define COUNT_BUF_B_ADDR           (PLIST_DATA_BASE_ADDR + 5*INDEX_BUFFER_SIZE)//8K
#define COUNT_BUF_C_ADDR           (PLIST_DATA_BASE_ADDR + 6*INDEX_BUFFER_SIZE)//8K
//compare
#define CMPSTR_MAX_LEN1       16
#define CMPSTR_MAX_LEN2       8

#define MP1_VAL   ('M' + 'P'*0x100 + '1'*0x10000)
#define MP3_VAL   ('M' + 'P'*0x100 + '3'*0x10000)
#define MP2_VAL   ('M' + 'P'*0x100 + '2'*0x10000)
#define WMA_VAL   ('W' + 'M'*0x100 + 'A'*0x10000)
#define ASF_VAL   ('A' + 'S'*0x100 + 'F'*0x10000)
#define WMV_VAL   ('W' + 'M'*0x100 + 'V'*0x10000)
#define AAX_VAL   ('A' + 'A'*0x100 + 'X'*0x10000)
#define M4A_VAL   ('M' + '4'*0x100 + 'A'*0x10000)
#define AA_VAL    ('A' + 'A'*0x100 + ' '*0x10000)//aa + �ո�
#define OGG_VAL   ('O' + 'G'*0x100 + 'G'*0x10000)
#define AAC_VAL   ('A' + 'A'*0x100 + 'C'*0x10000)
#define FLAC_VAL  ('F' + 'L'*0x100 + 'A'*0x10000)
#define APE_VAL   ('A' + 'P'*0x100 + 'E'*0x10000)

#define  TRACK_NULL      0xffff
#define  UNICODE_FLAG    0xfeff

#define  UTF8_FLAG_0    0xef
#define  UTF8_FLAG_1    0xbb
#define  UTF8_FLAG_2    0xbf

//ð���������
#define CUTOFF  8

//compare functions
typedef enum
{
    SORT_CMP_WORD = 0, SORT_CMP_ALBUM, SORT_CMP_STRING
} sort_cmp_type_e;

//ͬ��main_menu.h����
#define    MAX_APP_NUM    11
typedef struct
{
    uint16 active_item;//�AP�˵�����
    uint16 total_item; //AP�˵�����
    uint16 ap_id[MAX_APP_NUM]; //֧��Ӧ�ñ�ŵ�����
} cfg_mainmenu_var_t;

typedef struct
{
    uint16 magic; //VM ����ħ��
    cfg_mainmenu_var_t cfg_var;
    uint16 file_num[MAX_APP_NUM];//ÿ��Ӧ�õ��ļ�����
} g_mainmenu_var_t;

////plist_main ����arg[1] ��bitmap ;del,���뵽case_type.h����
//#define PLIST_DISK_C      0x80
//#define PLIST_DISK_H      0x40
//#define PLIST_DISK_U      0x20
//#define PLIST_DISK_ALL    0xe0

#define PL_BUILD_MUSIC       0x01
#define PL_BUILD_AUDIBLE     0x02
#define PL_BUILD_VIDEO       0x04
#define PL_BUILD_PICTURE     0x08
#define PL_BUILD_EBOOK       0x10
#define PL_BUILD_M3U         0x20

#define PL_BUILD_ALL         0x3f

//VRAM �ռ䣬��Ų����б�LIB���ղؼ��ļ�PL��8�ֽ�ƫ��
#define  VM_LIB_PL_OFFSET    (VM_PLAYLIST_INFO + 0x00040000 - 0x200)   //��VM_PLAYLIST_INFO���0.5K����

//note:��enhancedģ���eh_module.h���屣��һ��
#define  VM_USERPL_INFO    VM_FAVORITE_INFO
#define  USERPL_VM_SIZE    0x8000   //32K

typedef signed char(*cmp_obj_f)(uint16 *data1, uint16 *data2, uint8 len);

//�ļ�λ�ýṹ
typedef struct
{    
    uint32 cluster_no;                    //�ļ�Ŀ¼�����ڴغ�
    uint32 dir_entry;                     //�ļ���Ŀ¼���ڵ�ǰ���ڵ�ƫ��    
} file_pos_t; // 

extern bool save_to_file(uint8 *data_buf,uint32 len) __FAR__;
extern bool read_from_vram(uint8 *dest,uint8 *source,uint32 len) __FAR__;
extern bool write_to_vram(uint8 *dest,uint8 *source,uint32 len) __FAR__;
extern void debug_count_flag(uint32 count,uint32 debug_value) __FAR__;
extern app_result_e plist_get_app_msg(void) __FAR__;

#endif

