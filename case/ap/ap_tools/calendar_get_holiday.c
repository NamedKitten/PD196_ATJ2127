/*
 *******************************************************************************
 *                              USDK
 *                             Module: TOOLS AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_calendar_tools.C
 * History:
 *      <johnsen>    <2011-9-25 20:43>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "case_include.h"
#include "ap_tools.h"

const uint16 ChineseHoliday[] =
{ S_DAY_11, S_SWEET_D, S_WOMEN_D, S_PLANT_D, S_FOOLS_D, S_YOUTH_D, S_LABOR_D, S_MOTHER_D, S_NURSE_D,
  S_CHILD_D,S_FATHER_D, S_PARTY_D, S_ARMY_D, S_TEACH_D, S_EVENT918, S_NATION_D, S_CHRIS_D
};
const uint16 SpecHoliday[] =
{ S_DAY_11, S_DAY_55, S_DAY_77, S_DAY_815, S_DAY_99, S_HALL_EVE };

uint16 getChineseHoliday_S(uint16 year, uint8 month, uint8 day, uint8 week_day);
uint16 getForeignHoliday(uint16 year, uint8 month, uint8 day);

//���� �й�����

uint16 getChineseHoliday_S(uint16 year, uint8 month, uint8 day, uint8 week_day)
{
    int retVal;
    switch (month)
    {
        case 1:
        switch (day)
        {
            case 1:
            //    memcpy(str, "Ԫ��", strlen("Ԫ��")+1);
            retVal = ChineseHoliday[0];
            return retVal;
            default:
            return 0;
        }
        case 2:
        switch (day)
        {
            case 14:
            //    memcpy(str, "���˽�", strlen("���˽�")+1);
            retVal = ChineseHoliday[1];
            return retVal;
            default:
            return 0;
        }
        case 3:
        switch (day)
        {
            case 8:
            //    memcpy(str, "��Ů��", strlen("��Ů��")+1);
            retVal = ChineseHoliday[2];
            return retVal;

            case 12:
            //    memcpy(str, "ֲ����", strlen("ֲ����")+1);
            retVal = ChineseHoliday[3];
            return retVal;
            default:
            return 0;
        }
        case 4:
        switch (day)
        {
            case 1:
            //    memcpy(str, "���˽�", strlen("���˽�")+1);
            retVal = ChineseHoliday[4];
            return retVal;
            default:
            return 0;
        }
        case 5:
        if((week_day == 0) && (day > 7) && (day < 15))
        {
            retVal = ChineseHoliday[7];
            return retVal;
        }
        switch (day)
        {
            case 4:
            //    memcpy(str, "�����", strlen("�����")+1);
            retVal = ChineseHoliday[5];
            return retVal;
            case 1:
            //    memcpy(str, "�����Ͷ���", strlen("�����Ͷ���")+1);
            retVal = ChineseHoliday[6];
            return retVal;
            // case 14:
            //    memcpy(str, "ĸ�׽�", strlen("ĸ�׽�")+1);
            //       retVal=ChineseHoliday[7];
            //    return retVal;
            case 12:
            //    memcpy(str, "��ʿ��", strlen("��ʿ��")+1);
            retVal = ChineseHoliday[8];
            return retVal;
            default:
            return 0;
        }
        case 6:
        if((week_day == 0) && (day > 14) && (day < 22))
        {
            retVal = ChineseHoliday[10];
            return retVal;
        }
        switch (day)
        {
            case 1:
            //    memcpy(str, "��ͯ��", strlen("��ͯ��")+1);
            retVal = ChineseHoliday[9];
            return retVal;
            //   case 18:
            //    memcpy(str, "���׽�", strlen("���׽�")+1);
            //         retVal=ChineseHoliday[10];
            //    return retVal;
            default:
            return 0;
        }
        case 7:
        switch (day)
        {
            case 1:
            //    memcpy(str, "������", strlen("������")+1);
            retVal = ChineseHoliday[11];
            return retVal;
            default:
            return 0;
        }
        case 8:
        switch (day)
        {
            case 1:
            //    memcpy(str, "������", strlen("������")+1);
            retVal = ChineseHoliday[12];
            return retVal;
            default:
            return 0;
        }
        case 9:
        switch (day)
        {
            case 10:
            //    memcpy(str, "��ʦ��", strlen("��ʦ��")+1);
            retVal = ChineseHoliday[13];
            return retVal;
            case 18:
            //    memcpy(str, "��һ���±�", strlen("��һ���±�")+1);
            retVal = ChineseHoliday[14];
            return retVal;
            default:
            return 0;
        }
        case 10:
        switch (day)
        {
            case 1:
            //    memcpy(str, "�����", strlen("�����")+1);
            retVal = ChineseHoliday[15];
            return retVal;
            default:
            return 0;
        }
        case 12:
        switch (day)
        {
            case 25:
            //    memcpy(str, "ʥ����", strlen("ʥ����")+1);
            retVal = ChineseHoliday[16];
            return retVal;
            default:
            return 0;
        }

        default:
        return 0;
    }
}

//������������
uint16 getForeignHoliday(uint16 year, uint8 month, uint8 day)
{
    uint16 retVal = 0;
    switch (month)
    {
        case 1:
        switch (day)
        {
            case 1:
            //    memcpy(str, "New Years's Day", strlen("New Years's Day")+1);
            retVal = ChineseHoliday[0];
            return retVal;
            default:
            return 0;
        }
        case 2:
        switch (day)
        {
            case 14:
            //    memcpy(str, "Valentine's Day", strlen("valentine's day")+1);
            retVal = ChineseHoliday[1];
            return retVal;
            default:
            return 0;
        }
        case 3:
        switch (day)
        {
            case 12:
            //    memcpy(str, "Easter", strlen("Easter")+1);
            //	retVal=ChineseHoliday[2];
            //	return retVal;
            default:
            return 0;
        }
        case 4:
        switch (day)
        {
            case 1:
            //    memcpy(str, "Fool's Day", strlen("Fool's Day")+1);
            retVal = ChineseHoliday[4];
            return retVal;
            default:
            return 0;
        }
        case 10:
        switch (day)
        {
            case 31:
            //    memcpy(str, "Halloween Eve", strlen("Halloween Eve")+1);
            retVal = SpecHoliday[5];
            return retVal;
            default:
            return 0;
        }

        case 12:
        switch (day)
        {
            case 25:
            //    memcpy(str, "Christmas Day", strlen("Christmas Day")+1);
            retVal = ChineseHoliday[16];
            return retVal;
            default:
            return 0;
        }

        default:
        return 0;
    }
}

// û�ж�Ӧ���գ�����0�����򷵻�1
uint16 getLunarHoliday_S(const DateVarType *lunar)
{
    uint16 retVal = 0;
    switch (lunar->month)
    {
        case 1:
        switch (lunar->day)
        {
            case 1:
            //    memcpy(str, "����", strlen("����")+1);
            retVal = SpecHoliday[0];
            default:
            break;
        }
        break;
        case 5:
        switch (lunar->day)
        {
            case 5:
            //    memcpy(str, "�����", strlen("�����")+1);
            retVal = SpecHoliday[1];
            default:
            break;
        }
        break;
        case 7:
        switch (lunar->day)
        {
            case 7:
            //    memcpy(str, "��Ϧ", strlen("��Ϧ")+1);
            retVal = SpecHoliday[2];
            default:
            break;
        }
        break;
        case 8:
        switch (lunar->day)
        {
            case 15:
            //    memcpy(str, "�����", strlen("�����")+1);
            retVal = SpecHoliday[3];
            default:
            break;
        }
        break;
        case 9:
        switch (lunar->day)
        {
            case 9:
            //    memcpy(str, "������", strlen("������")+1);
            retVal = SpecHoliday[4];
            default:
            break;
        }
        break;
        default:
        retVal = 0;
        break;
    }
    return retVal;
}

uint16 getHolidayStr(const DateVarType *scolar, DateVarType *lunar, uint8 lanid, uint8 week_day)
{
    uint16 year = scolar->year;
    uint8 month = scolar->month;
    uint8 day = scolar->day;
    uint16 retVal = 0;
    switch (lanid)
    {
        case CHINESE_SIMPLIFIED: //���Ի����Ǽ�������
        case CHINESE_TRADITIONAL:
        if (0 == getLunarHoliday_S(lunar))
        {
            break;
        }
        retVal = getChineseHoliday_S(year, month, day, week_day);
        break;
        default: //Ĭ����Ӣ��
        retVal = getForeignHoliday(year, month, day);
        break;
    }

    return retVal;
}

