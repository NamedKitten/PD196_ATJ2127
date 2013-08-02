/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_bookmark.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ebook.h"

//������ת����
const uint16 multiple_num[8] =
{ 1, 1, 5, 10, 50, 100, 500, 1000 };

/****************************************************************************/
/*                                  �����                                                     */
/****************************************************************************/

/********************************************************************************
 * Description :��ȡ��ǩ����
 *
 * Arguments  :
 *                  inlet_type:�˵��б���������(����ebook.h�е�ebookmark_mode_e)
 *                  (inlet_type)0-��ǩѡ��˵���1-��ǩɾ���˵�,2-ҳ��ѡ��˵���
 * Returns     :
 *                  ���inlet_type=PAGE_SEL,������ҳ�������inlet_type=PAGE_SEL,������ǩ����
 * Notes       :
 *
 ********************************************************************************/
uint16 _get_bookmark_nums(inlet_type_e inlet_type)
{
    uint8 count_num = 0;
    if (inlet_type == PAGE_SEL)     //�����ҳ��ѡ��
    {
        //���ڳ���10M ���ı��ļ����ܵ�ҳ��������65535ҳ��������ʾ
        //ʱֻ���Ա����ķ�ʽ����ʾ��ѡ��
        count_num = _count_num_bytes(page_totalnum);
        if (count_num < 9)
        {
            page_multiple_sel = multiple_num[count_num - 1];
        }
        if (page_multiple_sel > 1)
        {
            return (uint16) ((page_totalnum / page_multiple_sel) + 1);
        }
        else
        {
            return (uint16) page_totalnum;
        }
    }
    else
    {
        //���bookmark_totalΪ0xff��˵��û�м������ǩ�����������ǩ�ļ���
        //��ȡ��ǩ��Ϣ��Ȼ�������ǩ����
        //����Ļ�ֱ�ӷ�����ǩ���������ˡ�
        if (bookmark_total == 0xFF)
        {
            _seek_and_read(0, 0x100, (uint8*) bkmarks);
        }
        else
        {
            return bookmark_total;
        }
        bookmark_total = 0;
        //������ǩ���������pagenumΪ0xFFFFFFFF��˵����������ݶ�������ǩ��Ϣ
        //��PC����pagenumΪ0��˵����������ݶ�������ǩ��Ϣ
        while (1)
        {
            if (    (bookmark_total >= MAX_BKMARK_NUM) 
                ||  (bkmarks[bookmark_total].pagenum == V_U32_INVALID)
                ||  (bkmarks[bookmark_total].title[0] == 0)
                )
            {
                break;
            }
            bookmark_total++;
        }
        return bookmark_total;
    }
}

/********************************************************************************
 * Description :ɾ����ǩ
 *
 * Arguments  :
 *              id:��ǰѡ�����ǩ������
 * Returns     :
 *                  0:��ǩɾ�����.1:��ǩ������Ϊ0
 * Notes       :
 *            ����id�Ŵ�bkmarks��ǩ�б���ɾ��ѡ�е���ǩ���ݣ�Ȼ���ٰѺ�����ǩ��Ӧ��ǰ��
 ********************************************************************************/
void _del_bookmark(uint16 id)
{
    uint16 i = id;
    //��ǰid���Ժ����ǩ��ǰ�ƣ�Ȼ����λ������һ�����Ϊ0xFFFFFFFF
    for (; i < bookmark_total; i++)
    {
        if (i < 15)
        {
            bkmarks[i].pagenum = bkmarks[i + 1].pagenum;
            libc_memcpy(bkmarks[i].title, bkmarks[i + 1].title, (MAX_TITLE_LEN));
        }
    }
    bookmark_total--;
    bkmarks[bookmark_total].pagenum = V_U32_INVALID;
    bmk_modify_flag = TRUE;
}

/********************************************************************************
 * Description :��Ӽ���ǩ
 *
 * Arguments  :
 *              ��
 * Returns     :
 *                  0:���ʧ�ܡ�1:��ӳɹ�
 * Notes       :
 *
 ********************************************************************************/
bool _add_bookmark(void)
{
    int i, j;
    //�����ǩ�����Ѿ���������������������ʾ�Ƿ�ɾ����һ���ļ���
    //�滻��ǰ����ǩ��Ҳ����������ǩ��ǰ��һλ��
    if (bookmark_total >= MAX_BKMARK_NUM)
    {
        return 0;
    }
    //�����ǩ����Ϊ0�Ļ���ֱ�ӽ���ǩ��Ž�ȥ��
    if (bookmark_total == 0)
    {
        i = 0;
    }
    else
    {
        for (i = (bookmark_total - 1); i >= 0; i--)
        {
            //�����ǩ���Ѿ����ڸ�ҳ������ֱ�ӷ���
            if (curpagenum == bkmarks[i].pagenum)
            {
                return 1;
            }
            //�����ǰҳ�����ڻ�ȡ����ҳ������˵����Ҫ��i��(i+1)���λ�ò���ҳ��
            else if (curpagenum > bkmarks[i].pagenum)
            {
                break;
            }
            //�����ǰҳ��С�ڻ�ȡ����ҳ������������ϲ���
            else
            {

            }
        }
        i++;
        /* �������м���� ҳ��*/
        /* [1],[2],...[*],[*+1],[*+2],.... */
        /*             |    |              */
        /*           prev  cur             */
        /*            [*],[��],[*+1]        */
        /*                 |               */
        /*              pre,cur          */
        for (j = bookmark_total; j > i; j--)
        {
            bkmarks[(uint8) j].pagenum = bkmarks[j - 1].pagenum;
            libc_memcpy(bkmarks[j].title, bkmarks[j - 1].title, (MAX_TITLE_LEN));
            libc_memset(&bkmarks[j].title[MAX_TITLE_LEN - 2], 0x0, 2);
        }
    }
    bkmarks[(uint8) i].pagenum = curpagenum;
    libc_memcpy(bkmarks[i].title, BOOKMARK_filename, (MAX_TITLE_LEN));
    libc_memset(&bkmarks[i].title[MAX_TITLE_LEN - 2], 0x0, 2);
    bookmark_total++;
    bmk_modify_flag = TRUE;
    //   _seek_and_write(0, bookmark_total*NODELEN,bkmarks);
    return 1;
}
/********************************************************************************
 * Description :��ȡ��ǰ��ǩ����
 *
 * Arguments  :
 *              index:��ǩ�б��������
 * Returns     :
 *                  �����ַ���ָ��
 * Notes       :
 *
 ********************************************************************************/

uint8 *_get_bmk_title(uint8 index)
{
    return bkmarks[index].title;
}
/********************************************************************************
 * Description :��ȡ��ǰ��ǩ������ҳ��
 *
 * Arguments  :
 *              index:��ǩ�б��������
 * Returns     :
 *                  ҳ��
 * Notes       :
 *
 ********************************************************************************/
uint32 _get_bmk_page(uint16 index)
{
    return bkmarks[index].pagenum;
}
/********************************************************************************
 * Description :���㵱ǰ���ֵ�λ��
 *
 * Arguments  :
 *              num:��Ҫ���������
 * Returns     :
 *                    ���������λ��
 * Notes       :
 *
 ********************************************************************************/
uint8 _count_num_bytes(uint32 num)
{
    uint8 count = 0;
    while (1)
    {
        count++;
        num /= 10;
        if (num == 0)
        {
            break;
        }
    }
    return count;
}

