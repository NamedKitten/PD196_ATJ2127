/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/

#include  "ebook.h"
#include  "ebook_res.h"

const conf_entry_head_t ebook_entrymenu =
{ "ENTRY MENU", 12 };

/*�������ڲ˵���Ŀ*/
const conf_menu_entry_t ebook_entry[] =
{
    { M_NOPLAYNOREAD_NOCARD, S_NOPLAYNOREAD, 0 },               //�޲��ź����Ķ��˵�
    { M_NOPLAYNOREAD_CARDEXIST, S_NOPLAYNOREAD_C, 0 },          //�޲��ź����Ķ��˵�(card)
    { M_NOPLAYREAD_NOCARD, S_NOPLAYREAD, 0 },                   //�޲��ź����Ķ��˵�
    { M_NOPLAYREAD_CARDEXIST, S_NOPLAYREAD_C, 0 },              //�޲��ź����Ķ��˵�(card)
    { M_NOWPLAYNOREAD_NOCARD, S_NOWPLAYNOREAD, 0 },             //���ڲ��ź����Ķ��˵�
    { M_NOWPLAYNOREAD_CARDEXIST, S_NOWPLAYNOREAD_C, 0 },        //���ڲ��ź����Ķ��˵�(card)
    { M_NOWPLAYREAD_NOCARD, S_NOWPLAYREAD, 0 },                 //���ڲ��ź����Ķ��˵�
    { M_NOWPLAYREAD_CARDEXIST, S_NOWPLAYREAD_C, 0 },            //���ڲ��ź����Ķ��˵�(card)
    { M_LPLAYNOREAD_NOCARD, S_LPLAYNOREAD, 0 },                 //�ϴβ��ź����Ķ��˵�
    { M_LPLAYNOREAD_CARDEXIST, S_LPLAYNOREAD_C, 0 },            //�ϴβ��ź����Ķ��˵�(card)
    { M_LPLAYREAD_NOCARD, S_LPLAYREAD, 0 },                     //�ϴβ��ź����Ķ��˵�
    { M_LPLAYREAD_CARDEXIST, S_LPLAYREAD_C, 0 }                 //�ϴβ��ź����Ķ��˵�(card)
};

/*����Ҷ�Ӳ˵���*/
const conf_item_head_t ebook_item_head =
{ "MENU ITEM", 10 };

const conf_menu_item_t menu_item[] =
{
    //���ڲ���      �˵�ѡ���ִ�к���
    { S_NOW_PLAYING, 0, _menu_func_nowplaying, NULL, NULL, NORMAL_MENU_ITEM, 1, 0 }, 
    
    //��һ�β���    �˵�ѡ���ִ�к���
    { S_LAST_PLAY, 0, _menu_func_lastplay, NULL, NULL, NORMAL_MENU_ITEM, 2, 0 },   
    
    //�Զ���������  �˵�ѡ���ִ�к���
    { S_PLAYSETTING, 0, _menu_func_autoplay, NULL, NULL, NORMAL_MENU_ITEM, 3, 0 },  
    
    //ɾ��������    �˵�ѡ���ִ�к���
    { S_DELETEEBK, 0, _menu_func_deleteebook, NULL, NULL, NORMAL_MENU_ITEM, 4, 0 },  
    
    //��ǩѡ��      �˵�ѡ���ִ�к���
    { S_BMKSELECT, 0, _menu_func_selbookmark, NULL, NULL, NORMAL_MENU_ITEM, 5, 0 },
    
    //ɾ����ǩ      �˵�ѡ���ִ�к���  
    { S_DELETE_BMK, 0, _menu_func_detbookmark, NULL, NULL, NORMAL_MENU_ITEM, 6, 0 },  
    
    //�����ǩ      �˵�ѡ���ִ�к���      
    { S_ADD_BMK, 0, _menu_func_addbookmark, NULL, NULL, NORMAL_MENU_ITEM, 7, 0 },  
    
    //ҳ��ѡ��      �˵�ѡ���ִ�к���
    { S_PAGESELECT, 0, _menu_func_selbookpage, NULL, NULL, NORMAL_MENU_ITEM, 8, 0 },
    
    //����Ŀ¼      �˵�ѡ���ִ�к���
    { S_LOCAL_FOLDER, 0, _menu_func_flashdir, NULL, NULL, NORMAL_MENU_ITEM, 9, 0 }, 
    
    //��Ŀ¼        �˵�ѡ���ִ�к���
    { S_CARD_FOLDER, 0, _menu_func_carddir, NULL, NULL, NORMAL_MENU_ITEM, 10, 0 }          
    
};
#ifdef PC
menu_cb_func _get_menu_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = ebook_item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((menu_item[i].major_id == str_id) && (menu_item[i].minor_id == str_id_sub))
        {
            return menu_item[i].menu_func;
        }
    }
    return NULL;
}

menu_cb_leaf _get_callback_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = ebook_item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((menu_item[i].major_id == str_id) && (menu_item[i].minor_id == str_id_sub))
        {
            return menu_item[i].callback;
        }
    }

    return NULL;
}

menu_cb_option _get_menu_option(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = ebook_item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((menu_item[i].major_id == str_id) && (menu_item[i].minor_id == str_id_sub))
        {
            return menu_item[i].menu_func;
        }
    }
    return NULL;
}
#endif

