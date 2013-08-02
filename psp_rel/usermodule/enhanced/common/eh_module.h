/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     eh_module.h
 * \brief    ���ݽṹ�ͳ�������
 * \author   liminxian
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#ifndef   _eh_module_h
#define   _eh_module_h

#include "enhanced.h"

//����caseĿ¼��ͷ�ļ�
#include "../../../../case/inc/vm_def.h"
#include "../../../../case/inc/case_type.h"

//ϵͳĬ�ϴ����ļ�ʹ�ó���

//#define USE_83NAME_CREAT  //�����ļ�ʹ�ö�������

//��¼LIB��offsetλ��
#define NOTE_LIB_OFFSET

#define FILE_SIZE_MIN    512
#define SECTOR_SIZE      EH_SECTOR_SIZE//512
#define KEY_BUF_LEN        64

#define PLIST_DIR_MAX    4  //����㼶����ȣ�0~3��
#define PLIST_INVALID_INDEX  0xffff
//#define USERPL_MAX        3

//note:��playlistģ���plist.h���屣��һ��
#define  VM_USERPL_INFO    VM_FAVORITE_INFO//0x0E0000
#define  USERPL_VM_SIZE    0x8000   //32K

//mainmenu��ʾ����VM��ַ
#define  VM_MAINMENU_CNT_INFO      VM_FILE_NUM

//const char ext_vec_rom[32][4] =
//{
//	"MP3", "WMA", "WAV", "MP1", "MP2", "OGG", "APE", "FLAC", "AAC", "ASF", "AA", "AAX",
//	"AA", "AAX",
//    "AMV", "AVI",
//    "JPG", "JPEG", "BMP", "GIF",
//    "TXT", "LRC",
//    "*  ",
//};


//#define FS_EXT_DIR     0
#define EH_GET_EXT_FILE EXT_NAME_ALL_FILE    //ǰ���ֽ�Ϊ��*f������ʾdir�����ļ�
#define EH_GET_EXT_DIR  EXT_NAME_ALL_DIR     //ǰ���ֽ�Ϊ��*d������ʾdir����Ŀ¼
#define EH_GET_EXT_ALL  EXT_NAME_ALL         //ǰ���ֽ�Ϊ��**������ʾdir����Ŀ¼���ļ�
#define EH_DIR_FLAG   ('d'*0x100+'*')
#define EH_FILE_FLAG  ('f'*0x100+'*')

typedef enum
{
    CHAR_TYPE_NUM = 0, //���֣�0-9��
    CHAR_TYPE_UPER, //��д��ĸ��A-Z��
    CHAR_TYPE_DNER, //Сд��ĸ��a-z��
    CHAR_TYPE_SPACE, //�ո� (" ")
    CHAR_TYPE_OTHER
//����ֵ
} char_type_e; //

typedef enum
{
    TYPE_TITLE = 0, TYPE_ABLUM, TYPE_ARTIST, TYPE_GENRE, TYPE_AUTHOR, TYPE_BOOK
} id3info_type_e;

//typedef struct
//{
//    char file_name[8];     //�ղؼе����� ���磺USERPL1
//    uint16 file_num;       //�ղؼ��ڵ��ļ�����
//    uint16 first_offset;   //��һ���ļ����(0,1)
//    uint16 last_offset;    //���һ���ļ����(0,1)
//    uint16 reserve;        //
//}userpl_head_t;
//
//
//typedef struct
//{
//    uint8   flag;                //  0-δʹ�ã�1- �����ļ� 2: �ļ�ɾ�����ռ�ռ����
//    uint8   reserve[3];           //  ����
//    uint16  prev_offset;          // ǰһ���ļ���ŵ�ƫ����
//    uint16  next_offset;          // ��һ���ļ���ŵ�ƫ����
//    flist_location_t  location;  //
//}userpl_file_t;


//ID3�������ݽṹ

typedef struct
{
    uint8 find_flag[6]; //����ID3�ı��
    uint16 buffer_offset; //BUF��offsetλ��
    uint32 item_offset; //�ڵ�ǰitem��offset
    handle file_hdl; //�ļ����
    uint8 *key_buf; //���ݽ���buf
    uint8 *data_buf; //��ȡ�ļ����ݵ�buf
} id3_parse_t;

//lyric
//typedef struct
//{
//    uint8  p1s;           //  0.1s
//    uint8  sec;           //  ��
//    uint8  min;           // ��
//    uint8  reserve;    //
//    uint16 offset_byte;   //����ڸ�ʿ��е��ֽ�ƫ��
//    uint16 lrc_len;          //��ʵĳ���
//} lrc_lable_t;


//playlist
typedef struct
{
    uint16 son_tree_num[3]; //���ӱ��tree �ĸ���
    uint16 son_tree_offset[3]; // 3���ӱ��tree���λ�õ�ƫ�ƣ�������Ϊ��λ
    uint16 file_index_offset; //������file_indexƫ��λ�ã����ֽ�Ϊ��λ
    uint16 sort_index_offset; // map���ֽ�ƫ�ƣ�file_indexΪ������������λ��sort_index
    uint16 index_parent_offset; // parent����ƫ�ƣ�file_indexΪ��������parent_offset
    uint16 reserve;
    //uint8 layer;
    //uint8 reserve;
} plist_item_info_t;

typedef struct
{
    uint8 savelen; // FrameID����������
    uint8* saveaddr; // FrameID�����ַ
} id3_save_t;

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
    //uint16 total_music;//�����ļ�����
    //uint16 total_video;//��Ƶ�ļ�����
    //uint16 total_picture;//ͼƬ�ļ�����
    //uint16 total_ebook;//�������ļ�����
    //uint16 total_station;//FMԤ���̨����
    uint16 file_num[MAX_APP_NUM];//ÿ��Ӧ�õ��ļ�����
} g_mainmenu_var_t;

#endif

