/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-27          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_favorite.h"


/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_addfavor_sure(void *param)
 * \��ӵ��ղؼе�ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_addfavor_sure(void *param)
{
    //���ز���
    menu_title_action_t* paramptr = (menu_title_action_t*) param;
    
    paramptr->style_id = MENULIST_TITLE;

    paramptr->title_id = S_ADD_TO_WHICH;
    //������һ���˵�
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_addfavor1_sure(void *param)
 * \��ӵ��ղؼ�1��ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_addfavor1_sure(void *param)
{

    return add_favorite_setmenu(FSEL_TYPE_LISTFAVOR1);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_addfavor2_sure(void *param)
 * \��ӵ��ղؼ�2��ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_addfavor2_sure(void *param)
{

    return add_favorite_setmenu(FSEL_TYPE_LISTFAVOR2);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_addfavor3_sure(void *param)
 * \��ӵ��ղؼ�3��ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_addfavor3_sure(void *param)
{

    return add_favorite_setmenu(FSEL_TYPE_LISTFAVOR3);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_delfavor1_sure(void *param)
 * \���ղؼ�1ɾ����ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_delfavor1_sure(void *param)
{

    return del_favorite_setmenu(FSEL_TYPE_LISTFAVOR1);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_delfavor2_sure(void *param)
 * \���ղؼ�2ɾ����ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_delfavor2_sure(void *param)
{

    return del_favorite_setmenu(FSEL_TYPE_LISTFAVOR2);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_delfavor3_sure(void *param)
 * \���ղؼ�3ɾ����ȷ�Ϻ���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_delfavor3_sure(void *param)
{
    return del_favorite_setmenu(FSEL_TYPE_LISTFAVOR3);
}
