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
 * \file     ENHANCED.H
 * \brief    ���ݽṹ�ͳ�������
 * \author   liminxian
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#ifndef  _enhanced_h
#define  _enhanced_h

#include <typeext.h>
#include <api.h>
#include <driver_manager.h>
#include <kernel_interface.h>
#include <vfs_interface.h>
#include <libc_interface.h>
//#include <>
//#include <>
//#include <>


//c�궨��
/*!
 * \brief
 *      Device driver's system separating layer headfile
 * \li  v: pointer of type atomic_t
 */
//#define atomic_read(v)	((v)->counter)

#define  ID3_ITEM_SIZE    28    //ID3��Ϣ����
#define  DIRLAYER_MAX     9     //Ŀ¼���,root + 8����Ŀ¼
#define  EH_SECTOR_SIZE   512   //��д������С
//end

#define  USERPL_MAX        3    //�ղؼеĸ���
#define  USERPL_FILE_MAX   100  //�ղؼ�֧���ļ�����
#define  USERPL_FILE_SIZE  256  //�ղؼ�ÿ����Ϣ�Ĵ�С
#define  USERPL_PRE_SEC    (EH_SECTOR_SIZE/USERPL_FILE_SIZE) //ÿ��������Ϣ����
#define  USERPL_HEAD_SEC   1   //�ղؼ�ͷ����
#define  USERPL_TITLE_MAX  (ID3_ITEM_SIZE * 2)  //�ղؼ���ʾ����
//
#define  PLIST_HEAD_SEC    1    //�б�ͷ��С
#define  PLIST_FILE_SIZE   256  //�б��ļ���Ϣ��С
#define  FILENAME_LENGTH   32   //ebook file��s namebuf length; uint:UNICODE

// M3U ����,M3U.LIB���� LIST �� DAT ������
#define  M3U_LIST_MAX          50    //m3u�б��ļ�����, (VALUE < ID3_ITEM_SIZE*3;see m3ulist_f_info_t's m3u_index)
#define  M3U_LIST_SIZE         ((M3U_LIST_MAX/2 + M3U_LIST_MAX/4) * EH_SECTOR_SIZE)//list���ִ�С,unit:byte,0x4a00
#define  M3U_DAT_HEADOFFSET    0xa0  //unit:byte��ָ��PLIST_HEAD_SEC�е�ƫ��

//M3U DAT
#define  M3U_DAT_FILE_MAX         4000             //DAT �����ļ�������
#define  M3U_DAT_FILESIZE     	  PLIST_FILE_SIZE  //M3U�б����ļ���Ϣ��С
#define  M3U_DAT_INDEXSIZE    	  0x2000           //�ļ�FILE_INDEX�Ĵ�С��unit:byte
#define  M3U_DAT_POSINDEX_SIZE 	 (8 * 0x1000)
#define  M3U_DAT_SIZE         	 (M3U_DAT_INDEXSIZE * M3U_LIST_MAX + M3U_DAT_FILE_MAX * M3U_DAT_FILESIZE + M3U_DAT_POSINDEX_SIZE) //DAT ���ִ�С,//0x166000
  
#define  M3U_DAT_INDEXOFFSET  	 (M3U_LIST_SIZE ) //�ļ�FILE_INDEX��ƫ����ʼ,unit:byte,0x4a00
#define  M3U_DAT_FILEOFFSET   	 (M3U_DAT_INDEXOFFSET + (M3U_LIST_MAX * M3U_DAT_INDEXSIZE))   //�ļ���Ϣƫ��,unit:byte,0x68a00
#define  M3U_DAT_POS_INDEXOFFSET (M3U_DAT_FILEOFFSET + M3U_DAT_FILE_MAX * M3U_DAT_FILESIZE) //�ļ�λ������ƫ��,unit:byte,0x162a00

//c���Ͷ���
/*!
 *  \brief
 *      ssize_t - unsigned int
 */
//typedef unsigned int ssize_t;


//cö�ٱ���
/*!
 *  \brief
 *  enumValue test
 *  \li TEST_1 : enumValue_1
 *  \li TEST_2 : enumValue_2
 */

//�ļ�ѡ��������
typedef enum
{
    FSEL_MODULE_DIR = 0, 
    FSEL_MODULE_PLIST, 
    FSEL_MODULE_FAVOR,
//	FSEL_MODULE_OTHER
} fsel_module_e;

//�ļ�ѡ����ѡ������
typedef enum
{
    FSEL_TYPE_COMMONDIR = 0, //Ŀ¼����
    FSEL_TYPE_DISKSEQUNCE,   //ȫ����Ų���
    FSEL_TYPE_SDFILE,        //sd���ļ�    
    FSEL_TYPE_PLAYLIST,      //�����б�
    FSEL_TYPE_LISTAUDIBLE,   //AUDIBLE�����б�
    FSEL_TYPE_LISTVIDEO,     //VIDEO�б�
    FSEL_TYPE_LISTPICTURE,   //PICTURE�б�
    FSEL_TYPE_LISTEBOOK,     //EBOOK�б�
    FSEL_TYPE_M3ULIST,       //m3u�б�
    FSEL_TYPE_LISTFAVOR1,    //�ղؼ�1
    FSEL_TYPE_LISTFAVOR2,    //�ղؼ�2
    FSEL_TYPE_LISTFAVOR3     //�ղؼ�3
} fsel_type_e;

//����ģʽ
typedef enum
{
    FSEL_MODE_NORMAL = 0,   //��ͨ����
    FSEL_MODE_LOOPONE,      //ѭ���ŵ�ǰ��
    FSEL_MODE_LOOPALL,      //ѭ������
    FSEL_MODE_INTRO,        //�������
    FSEL_MODE_LOOPONEDIR,   //ѭ���ŵ�ǰĿ¼
    FSEL_MODE_DIRNOR,       //˳�򲥷�Ŀ¼
    FSEL_MODE_BROWSER       //�ļ������
} fsel_mode_e;

//browser ��ʾ
typedef enum
{
    FSEL_BROW_DIR = 0,  //ֻ��ʾĿ¼
    FSEL_BROW_FILE,     //ֻ��ʾ�ļ�
    FSEL_BROW_ALL       //��ʾ�ļ���Ŀ¼
} fsel_brow_e;

//typedef enum
//{
//    FSEL_DEL_FILE = 0, //ɾ���ļ�
//    FSEL_DEL_DIRFILE,  //ɾ��Ŀ¼
//} fsel_del_e;

//ID3��ȡ֧������
typedef enum
{
    ID3_TYPE_AA = 0,
    ID3_TYPE_AAC,
    ID3_TYPE_AAX,
    ID3_TYPE_APE,
    ID3_TYPE_FLAC,
    ID3_TYPE_MP3,
    ID3_TYPE_OGG,
    ID3_TYPE_WMA,
    ID3_TYPE_END
} id3_type_e;

//�����б��ӱ�����
typedef enum
{
    PLIST_TITLE = 0x00,  //
    PLIST_ALBUM = 0x01,  //
    PLIST_ARTIST = 0x02, //
    PLIST_GENRE = 0x03,  //
    PLIST_BOOK = 0x10,
    PLIST_AUTHOR = 0x12,
    PLIST_M3U   = 0x50
} plist_type_e;

/*
 typedef enum
 {
 TYPE_TITLE=0,
 TYPE_ABLUM,
 TYPE_ARTIST,
 TYPE_GENRE,
 TYPE_AUTHOR,
 TYPE_BOOK
 }id3info_type_e;
 */

//playlist ��ĸ���
typedef enum
{
    PL_LIB_MUSIC = 0, 
    PL_LIB_AUDIBLE, 
    PL_LIB_VEDIO, 
    PL_LIB_PICTURE, 
    PL_LIB_EBOOK, 
    PL_LIB_PIC2, 
    PL_LIB_MAX
} plist_lib_e;

//browser
typedef enum
{
    CUR_DIR = 0,        //���½��͵�ǰĿ¼ֻ��Ҫ��ȡdir_browse_t
    SON_DIR,            //������Ŀ¼
    PARENT_DIR,         //���ظ�Ŀ¼
    ROOT_DIR            //�����Ŀ¼
} brow_dir_e;

//ɾ������
typedef enum
{
    BROW_DEL_FILE = 0,      //ɾ���ļ�
    BROW_DEL_DIR            //ɾ����Ŀ¼
} brow_del_e;

//��ȡ��ǰ���ʱ���ǩ
typedef enum
{
    LYRIC_CUR_TIME = 0,     //��ǰʱ���ǩ
    LYRIC_PREV_TIME,        //��һ��ʱ���ǩ
    LYRIC_NEXT_TIME         //��һ��ʱ���ǩ
} lrc_time_e;

//�ӿڵ��Դ�������
typedef enum
{
    FSEL_NO_ERR = 0,        //û�б���
    FSEL_ERR_OUTOF,         //ѡ�����
    FSEL_ERR_EMPTY,         //Ŀ¼�ձ���
    FSEL_ERR_SETDIR,        //����Ŀ¼·������
    FSEL_ERR_SETFILE,       //�����ļ���Ϣ����
    FSEL_ERR_EXCEPTION,     //�쳣����
    FSEL_ERR_FAIL,          //�ӿڲ���ʧ��
    FSEL_ERR_DISK,          //�̷�����
    FSEL_ERR_EXIST,         //�ļ��Ѵ���
    FSEL_ERR_NO_EXIST,      //�ļ�������
    FSEL_ERR_USING,         //�ļ���ʹ����
    FSEL_ERR_STG,           //��װ����ʧ��
    FSEL_ERR_FS,            //�ļ�ϵͳ�ӿڱ���
    FSEL_ERR_LIST_NOEXIST,  //�ղؼл򲥷��б�����
    FSEL_ERR_SHUFFLE,       //shuffle���Ƴ���
    FSEL_ERR_INPUT          //�����������
} fsel_error_e;

//c�ṹ�嶨��
/*!
 * \brief
 *      Device driver's system separating layer headfile.
 */
//struct list_head
//{
//       /*! next*/
//       struct list_head *next;
//       /*! prev*/
//       struct list_head *prev;
//};

//ENHANCED ��ʼ������
typedef struct
{
    uint8 fsel_type;            //ѡ������      fsel_type_e
    uint8 fsel_mode;            //����ģʽ      fsel_mode_e
    uint8 brow_type;            //browser��ģʽ fsel_brow_e
    uint8 disk;                 //DISK_C;DISK_H;DISK_U
    uint32 file_type_bit;       //�ļ���׺��bitmap
} fsel_param_t;

//Ŀ¼�µ��ļ���Ϣ
typedef struct
{
    uint8 disk;                 //DISK_C;DISK_H;DISK_U
    uint8 dir_layer;            //��ǰĿ¼��Σ���0��ʼ��0��ʾ��Ŀ¼
    uint16 resever;             //��������
    uint8 filename[4];          //�ļ��ĺ�׺����ȫ��0�����ļ���ϢΪ��
    uint32 cluster_no;          //�ļ���Ŀ¼�����ڵĴغ�
    uint32 dir_entry;           //�ļ���Ŀ¼�������ڴغ��ڵ�ƫ��
    pdir_layer_t dir_layer_info;//Ŀ¼�����Ϣ
    uint8 res_3byte[3];         //��������
    uint16 file_total;          //��ǰĿ¼�ļ�����
    uint16 file_num;            //��ǰ�ļ���Ŀ¼�е����
} file_location_t;

//�����б��µ��ļ���Ϣ
typedef struct
{
    uint8 disk;                 //DISK_C;DISK_H;DISK_U
    uint8 list_layer;           //�б����
    uint16 list_type;           //�ӱ����� plist_type_e
    uint8 filename[4];          //�ļ��ĺ�׺����ȫ��0�����ļ���ϢΪ��
    uint32 cluster_no;          //�ļ���Ŀ¼�����ڵĴغ�
    uint32 dir_entry;           //�ļ���Ŀ¼�������ڴغ��ڵ�ƫ��
    pdir_layer_t dir_layer_info;//Ŀ¼�����Ϣ
    uint8 res_3byte[3];         //��������
    uint16 file_total;          //��ǰ�ļ����� (�ڵ�ǰ�б����ļ��������)
    uint16 file_num;            //��ǰ�ļ���� (�ڵ�ǰ�б����ļ�������)
    uint16 file_index;          //��ǰ�ļ������ţ�������������ţ�
    uint16 reserved;            //��������
    uint16 list_layer_no[4];    //��¼�ļ����ڵ�ÿ���б��е�λ�ã����ֱ��¼��ÿ��������λ�ã�
    uint16 layer_list_offset;   //listƫ��λ��
    uint16 reserve;             //��������

} plist_location_t;

//�ղؼ��µ��ļ���Ϣ
typedef struct
{
    uint8 disk;         //'DISK_C;DISK_H;DISK_U
    uint8 reserved;     //��������
    uint16 file_index;  //no used
    uint8 filename[4];  //�ļ��ĺ�׺����ȫ��0�����ļ���ϢΪ��
    uint32 cluster_no;  //�ļ���Ŀ¼�����ڵĴغ�
    uint32 dir_entry;   //�ļ���Ŀ¼�������ڴغ��ڵ�ƫ��
    pdir_layer_t dir_layer_info;//Ŀ¼�����Ϣ
    uint8 res_3byte[3]; //��������
    uint16 file_total;  //��ǰ�ղؼ��ļ�����
    uint16 file_num;    //��ǰ�ļ����ղؼ��е����
    uint8 title[USERPL_TITLE_MAX]; // �ղؼ���ʾ���ļ�����
} flist_location_t;

//��ȡID3����Ϣ�ṹ
typedef struct
{
    uint8 *tit2_buffer;  //�����ļ�����buffer
    uint8 *tpe1_buffer;  //��������buffer
    uint8 *talb_buffer;  //����ר��buffer
    uint8 *genre_buffer; //��������buffer
    uint8 *drm_buffer;   //����drm��buffer
    uint8 *track_buffer; //��������buffer
    uint8 tit2_length;   //�ļ����ⳤ��
    uint8 tpe1_length;   //���߳���
    uint8 talb_length;   //ר������
    uint8 genre_length;  //���ɳ���
    uint8 drm_length;    //drm����
    uint8 track_length;  //���쳤��
    uint8 apic_type;     // 0-��ͼƬ��1-jpeg
    uint8 apic_flag;     //��ȡͼƬƫ�Ʊ��
    uint8 ldb_flag;      //��ȡldb���ƫ�Ʊ��
    uint8 reserve;       //��������
    uint16 track_num;    //�����
    uint32 ldb_offset;   //ldb������ļ��е��ֽ�ƫ��
    uint32 apic_offset;  //apicͼƬ���ļ��е��ֽ�ƫ��
} id3_info_t;

//�ļ���Ϣ������
typedef union
{
    file_location_t  dirlocation;       //Ŀ¼ģʽ�µ��ļ���Ϣ
    plist_location_t plist_location;    //�����б�ģʽ�µ��ļ���Ϣ
    flist_location_t flist_location;    //�ղؼ���ģʽ�µ��ļ���Ϣ
} musfile_location_u;

//�ļ���Ϣ
typedef struct
{
    uint8 file_source;              //�ļ���Ϣ���ͣ�Ŀ¼�������б���ղؼ�
    uint8 reserved[3];              //��������
    musfile_location_u file_path;   //�ļ���Ϣ�ṹ
} file_path_info_t;

//browser
typedef struct
{
    uint16 dir_total;   //��ǰĿ¼�����ļ�������
    uint16 file_total;  //��ǰĿ¼���˺���ļ�����
    uint8 name_len;     //���ֳ��ȣ���λ��bytes��
    uint8 layer_no;     //��ǰĿ¼�������
    uint8 reserve[2];   //��������
    uint8 *name_buf;    //���ֻ�����
} dir_brow_t;

//browser
typedef struct
{
    uint8 option; //����ѡ��
    uint8 num;    //����
    uint16 top;   //����λ��
} brow_decode_t;

// �ļ���¼�ṹ
typedef struct
{
    uint8 ext[4];     //�ļ���׺��
    uint8 name_len;   //���ֵĳ��ȣ���λ��bytes��
    uint8 reserve[3]; //4�ֽڶ���
    uint8 *name_buf;  //���ֻ�����
} file_brow_t;

//ר��ͼƬ��¼�ṹ
typedef struct
{
    uint8 album_flag;  //�Ƿ���ר��ͼƬ��־��0 - �� 1 - ��
    uint8 name_len;    //���ֵĳ��ȣ���λ��bytes��
    uint8 reserve[2];  //4�ֽڶ���
    uint8 *name_buf;   //���ֻ�����
    uint32 cluster_no; // ͼƬ�ļ�Ŀ¼��غ�
    uint32 dir_entry;  // ͼƬ�ļ�Ŀ¼��ƫ�� 
} album_brow_t;


//�ļ�ID3��Ϣ�ṹ
typedef struct
{
    char title_string[ID3_ITEM_SIZE * 2]; //����&�ļ����� ������16bytes��
    char artist_string[ID3_ITEM_SIZE];    //����������    ������8bytes��
    char album_string[ID3_ITEM_SIZE];     //ר������      ������8bytes��
    char gener_string[ID3_ITEM_SIZE];     //��������      ������8bytes��
    char track_string[8];                 //track num string
    uint8 filename[4];                    //�ļ��ĺ�׺��
    uint8 id3_tag_flag;                   //�Ƿ����ID3��Ϣ�ı�־��1--�У�0--��
    uint8 flag;                           //���ļ��Ƿ���� 1-�У�0-�ޣ�2-��ɾ��
    uint16 track_num;                     //ר����Ŀ��
    uint32 cluster_no;                    //�ļ�Ŀ¼�����ڴغ�
    uint32 dir_entry;                     //�ļ���Ŀ¼���ڵ�ǰ���ڵ�ƫ��
    uint16 prev_offset;                   //��һ�����λ�õ�ƫ��
    uint16 next_offset;                   //��һ�����λ�õ�ƫ��
    pdir_layer_t dir_layer_info;          //Ŀ¼�����Ϣ
} plist_f_info_t; // 256 bytes

//�����б�tree�ṹ
typedef struct
{
    uint8 flag;             //list��־ :0-no used,1-exist,2-deleted
    uint8 type;             //��ǰ�б���ʾID3��Ϣ��
    uint16 son_num;         //�¼����б�ĸ���
    uint16 file_total;      //�б��������ļ��ĸ���
    uint16 file_index;      //��ʼ�ļ�����ţ��ɶ�����sort_indexֵ
    uint16 son_offset;      //��list�Ĵ��λ��ƫ�Ƹ���
    uint16 parent_offset;   //��list�Ĵ��λ��ƫ�Ƹ���
    uint16 prev_offset;     //��һ����ƫ��
    uint16 next_offset;     //��һ����ƫ��
} plist_tree_t;

//�����б�ͷ��Ϣ
typedef struct
{
    char plist_name[8];         //LIB ʶ����
    uint16 file_total;          //�ļ�������
    uint16 file_info_offset;    //�ļ���Ϣ��ŵ�ƫ�ƣ�������Ϊ��λ
    uint16 reserve[2];          //��������
} plist_head_t;                 //��16bytes��

typedef struct
{
    char item_name[8];          //item����
    uint16 son_tree_num[3];     //�Ӽ����tree �ĸ���
    uint16 son_tree_offset[3];  //3���ӱ��tree���λ�õ�ƫ�ƣ�������Ϊ��λ
    uint16 file_index_offset;   //������file_indexƫ��λ�ã����ֽ�Ϊ��λ
    uint16 sort_index_offset;   //map���ֽ�ƫ�ƣ�file_indexΪ������������λ��sort_index
    uint16 index_parent_offset; //parent����ƫ�ƣ�file_indexΪ��������parent_offset
    uint16 reserve;             //��������
} plist_item_t; //��28bytes��


typedef struct
{
    uint16 num;     //����ѡ��λ�� 1~n
    uint16 total;   //��������
}plist_layer_t;


//�ղؼ�ͷ��Ϣ
typedef struct
{
    char file_name[8];      //�ղؼе����� ���磺USERPL1
    uint16 file_num;        //�ղؼ��ڵ��ļ�����
    uint16 first_offset;    //��һ���ļ����(0,1)
    uint16 last_offset;     //���һ���ļ����(0,1)
    uint16 cover_offset;    //�����󣬽�Ҫ�����ǵ��ļ����(0,1,...)
} userpl_head_t;

//�ղؼд洢���ļ���Ϣ
typedef struct
{
    uint8 flag;                 //0-δʹ�ã�1- �����ļ� 2: �ļ�ɾ�����ռ�ռ����
    uint8 reserve[3];           //��������
    uint16 prev_offset;         //ǰһ���ļ���ŵ�ƫ����
    uint16 next_offset;         //��һ���ļ���ŵ�ƫ����
    flist_location_t location;  //�ļ���Ϣ
} userpl_file_t;

//EBOOK��Ϣ�ṹ
typedef struct
{
    char file_name[FILENAME_LENGTH * 2];    //����&�ļ����� ������16bytes��
    char file_length[4];                    //����������    ������8bytes��
    char filename_lenth;                    //ר������      ������8bytes��
} plist_ebook_info_t;// 256 bytes


//m3u �б��ļ���Ϣ�ṹ
typedef struct
{
    char  title_string[ID3_ITEM_SIZE * 2];//����&�ļ����� ������16bytes��
    uint8 m3u_index[ID3_ITEM_SIZE*3];	  //ʹ�ø��ļ���M3U ��index��0	xFFΪ��;ɾ��ʱ���������
    char  track_string[8];                //track num string
    uint8 filename[4];                    //�ļ��ĺ�׺��
    uint8 id3_tag_flag;                   //�Ƿ����ID3��Ϣ�ı�־��1--�У�0--��
    uint8 flag;                           //���ļ��Ƿ���� 1-�У�0-�ޣ�2-��ɾ��
    uint16 track_num;                     //ר����Ŀ��
    uint32 cluster_no;                    //�ļ�Ŀ¼�����ڴغ�
    uint32 dir_entry;                     //�ļ���Ŀ¼���ڵ�ǰ���ڵ�ƫ��
    uint16 prev_offset;                   //��һ�����λ�õ�ƫ��
    uint16 next_offset;                   //��һ�����λ�õ�ƫ��
    pdir_layer_t dir_layer_info;          //Ŀ¼�����Ϣ
} m3ulist_f_info_t; // 256 bytes,��plist_f_info_t ��Сһ��

//m3u �б�ͷ��Ϣ
typedef struct
{
    uint16 dat_file_total;        //DAT���ļ�������    
    uint16 list_file_num[M3U_LIST_MAX];    //��Ӧÿ��M3U�����ļ��ĸ���
    uint16 audible_file_num[M3U_LIST_MAX]; //��Ӧÿ��M3U����audible�ļ��ĸ���
} m3udat_head_t;  //ֻ�����ӳ�Ա�������޸ĳ�Աλ��


/*******
 �ļ��б���ö�ٺͽṹ����
 ********/

//�ļ��б���ö������
typedef enum
{
    /*! ɾ���ļ����� */
    SCANFILE_TYPE_DELETE = 0,
    /*! ���ݽ��������ƣ����� */
    SCANFILE_TYPE_DXCH = 1,
} scanfile_type_e;

///����ļ��з��ؽ��ö������
typedef enum
{
    /*! ���ظ��ļ��� */
    SCANFILE_RESULT_SELF = 0,
    /*! ����ƥ���ļ� */
    SCANFILE_RESULT_FILE = 1,
    /*! �������ļ��� */
    SCANFILE_RESULT_SONDIR = 2,
    /*! ���ظ��ļ��У����������ļ��� */
    SCANFILE_RESULT_PARENTDIR = 3,
    /*! ������ӿڳ��� */
    SCANFILE_RESULT_ERR = 4,
} scanfile_result_e;

//bs ģ�����·������ṹ��
typedef struct
{
    /*! 8��·����¼��ֻ���Ҫ������ļ��в㼶��ʼ���� */
    uint8 bs_dir_no[DIRLAYER_MAX];
    /*! ��ǰ������ */
    uint8 bs_layer;
    /*! ��������ļ������ڲ�Σ���ֵ����������в��� */
    uint8 root_layer;
    /*! ��ǰ����ļ���� */
    uint16 list_no;
} bs_dir_location_t;

/********************************************************************************
 *               ENHANCED ģ������ṩ�Ľӿ��б�
 *********************************************************************************/

/**********************************
 (1)ģ��ϵͳ�ӿ�
 Ŀ¼��\COMMON
 **********************************/
extern fsel_error_e fsel_get_error(void) __FAR__;
extern int32 fsel_init(fsel_param_t *init_param,uint8 drv_mode) __FAR__;
extern bool fsel_exit(void) __FAR__;

/**********************************
 (2)�����Խӿ�
 Ŀ¼��\FUNC
 **********************************/
extern bool fsel_favorite_add(flist_location_t *location, fsel_type_e target_fsel_type) __FAR__;
extern bool fsel_favorite_clear(fsel_type_e target_fsel_type) __FAR__;
extern bool fsel_favorite_delete(fsel_type_e target_fsel_type,uint16 list_no,flist_location_t *location) __FAR__;//�ղ�ɾ���ļ�
extern uint16 fsel_favorite_get_total(fsel_type_e target_fsel_type) __FAR__;//��ȡָ���ղؼ����ļ�������
extern bool fsel_change_filepath(void* source_location,plist_location_t *dest_location,fsel_type_e source_type,plist_type_e dest_type) __FAR__;
extern bool fsel_change_fav_location(void *source_location,flist_location_t *dest_location,fsel_type_e source_type,id3_info_t *id3_info_buf) __FAR__;
extern bool fsel_make_plistlayer(plist_layer_t *layer_buffer,plist_location_t * location) __FAR__;
/**********************************
 (3)����/�����б�/�ղؼ� �ļ���λ���Žӿ�
 Ŀ¼��\fsel
 **********************************/
extern bool fsel_set_mode(fsel_mode_e mode) __FAR__;
extern bool fsel_set_typebit(uint32 file_type_bit,bool reset_dir_flag) __FAR__;
extern bool fsel_get_location(void *location,fsel_type_e type) __FAR__;
extern bool fsel_set_location(void *location,fsel_type_e type) __FAR__;
extern bool fsel_get_nextfile (char *strfile) __FAR__;
extern bool fsel_get_prevfile(char *strfile) __FAR__;
extern bool fsel_get_byno(char *strfile, uint16 num) __FAR__;
extern uint16 fsel_get_total(void) __FAR__;
extern uint16 fsel_get_fileno(void) __FAR__;

//����ΪĿ¼����ʱ����λĿ¼���Žӿ�(Ԥ��)
extern bool fsel_get_nextdir (char *strfile) __FAR__;
extern bool fsel_get_prevdir(char *strfile) __FAR__;
extern bool fsel_enter_dir(uint8 mode,char *dir_name) __FAR__;

/**********************************
 (4)����/�����б�/�ղؼ� �ļ���Ϣ����ӿ�
 Ŀ¼��\bs
 **********************************/
extern bool fsel_browser_get_location(void *location,fsel_type_e type) __FAR__;
extern bool fsel_browser_set_location(void *location,fsel_type_e type) __FAR__;
extern bool fsel_browser_get_items(brow_decode_t *decode_param, file_brow_t *file_list) __FAR__;
extern bool fsel_browser_enter_dir(brow_dir_e type,uint16 list_no, dir_brow_t *dir_brow) __FAR__;
extern bool fsel_browser_select(uint16 list_no) __FAR__;
extern bool fsel_browser_delete(brow_del_e type,uint16 list_no ,void *location) __FAR__;
extern bool fsel_browser_modify_location(file_location_t *location,uint32 bitmap) __FAR__;
extern bool fsel_browser_set_file(void *location,fsel_type_e type) __FAR__;
extern bool fsel_browser_get_albums(brow_decode_t *decode_param, album_brow_t *album_list) __FAR__;
//�ļ����±����ļ�
extern bool fsel_browser_scanfile_init (dir_brow_t *dir_browser) __FAR__;
extern scanfile_result_e fsel_browser_scanfile(file_path_info_t *path_info, dir_brow_t *dir_browser, scanfile_type_e type) __FAR__;

/**********************************
 (5)��ʴ򿪺���ʾ�ӿ�
 Ŀ¼��\lrc
 **********************************/
extern bool lyric_open(char * music_filename) __FAR__;
extern bool lyric_open_dlb(char * music_filename,uint32 dlb_offset) __FAR__;
//extern bool lyric_close(void) __FAR__;

extern bool lyric_get_init(void) __FAR__;
extern bool lyric_get_next(char * lyric_str, uint8 len) __FAR__;
extern bool lyric_get_prev(char * lyric_str, uint8 len) __FAR__;
extern bool lyric_get_time(lrc_time_e time_type,uint32 *time) __FAR__;
extern bool lyric_check_query_time(uint32 time) __FAR__;
extern bool lyric_seek_for(char *lyric_str, uint8 len, uint32 time) __FAR__;

/**********************************
 (6)id3��Ϣ��ȡ
 Ŀ¼��\id3
 **********************************/
extern bool get_id3_info(id3_info_t *id3_info,const char *filename,id3_type_e file_type) __FAR__;

#endif//#ifndef  _enhanced_h
