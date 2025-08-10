#include <math.h>
#include <stdlib.h>
#include "stdafx.h"
#include "xrrno.h"
#include "xlog.h"
#include "xcommon.h"
#include "xlist.h"
#include "task.h"
#include "config.h"
#include "alarm.h"
#include "task.h"
#include "task021.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "snail_client_context.h"
#include "snail_client_manager.h"

#define FONT021_DIR "font\\021\\"

int Tz021_Fly2Copy(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    
    //��m������С��ͼ,Ϊ��ֹ����,�Ƚ�����Ƶ����Ͻ�
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(300);

    for (;;)
    {
        dmobj->KeyDownChar("m");
        Delay(200);
        dmobj->KeyUpChar("m");
        Delay(200);

        //�鿴m���Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT021_DIR"font-fly-mcheck1.txt");
        code = dmobj->FindStrFast(210, 45, 1138, 745, "˹����[���������Ѵ�]", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("˹����[���������Ѵ�]:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("��������:����:m����:��ɱ��:����,�ɹ�");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("��������:����:m����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);

        //����'�ƶ�'����
        dmobj->SetDict(0, FONT021_DIR"font-fly-mcheck2.txt");
        code = dmobj->FindStrFast(210, 45, 1138, 745, "�ƶ�", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("��������:����:�ƶ�:����:�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("��������:����:�ƶ�:����:��ɱ��:����,�ɹ�");
            return code;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:���ƶ����̱�����ɱ�����ָ���ʱ���ܷ���SUCCESS 2021/06/14 11:12:33
        if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("��������:����:�ƶ�:����:����:�ɹ�:�ƶ�:����:�ɹ�");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("��������:����:�ƶ�:����:ʧ��");
        Delay(1000);
    }
    
    dmobj->MoveTo(lposX2, lposY2);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    //��鵱ǰ����,������겻��234:220������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                //bug:���̫����ܾ���Ŀ��㸽��,ʵ�ʲ�û�з�£ 2022/01/08 15:43:16
                if (abs(posX - 60) <= 1 && abs(posY - 104) <= 1)
                {
                    api_log_MsgDebug("��������:����:�ƶ��������ſ�:�ɹ�");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("��������:����:�ƶ�:�����:��ɱ��:����,�ɹ�");
            return code;
        }

        api_log_MsgDebug("��������:����:�ƶ�:δ����");
        Delay(1000);
    }
    
    return SNAIL_ERRNO_SUCCESS;
}

int Tz021_CheckAlive(Idmsoft* dmobj, void* ctxt, int bCopyoutPre)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int blueThreshold = luaman_config->blueThreshold;
    int maxBlueFiled = luaman_config->maxBlueFiled;
    int minBlue = luaman_config->minBlue;
    int code = 0;
    CString strBBlood;
    CStringArray bBlood;
    char* cstr = NULL;
    int sz = 0;
    long bBloodAva = 0;
    long bBloodTot = 0;
    int strBBloodLen = 0;
    
    dmobj->SetDict(0, FONT021_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("��Ѫ��:��ȡ:�쳣, groupid:%d, hwnd:%ld",
            luaman_config->groupid, hwnd);
        return SNAIL_ERRNO_EXCEPTION;
    }

    cstr = strBBlood.GetBuffer(BUFFER_MAX);
    strBBlood.ReleaseBuffer();

    sz = SplitString(strBBlood, '/', bBlood);
    if (2 == sz)
    {
        bBloodAva = atol(bBlood.GetAt(0));
        bBloodTot = atol(bBlood.GetAt(1));
    }
    else
    {
        strBBloodLen = strBBlood.GetLength();
        bBloodAva = atol(strBBlood.Mid(1, strBBloodLen - maxBlueFiled));
        bBloodTot = atol(strBBlood.Mid(strBBloodLen - maxBlueFiled + 1, strBBloodLen));

        api_log_MsgDebug("Ѫ��:����:�ɹ�,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
            hwnd, bBloodAva, bBloodTot);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (bBloodAva > 0)
    {
        /////////////////////////////////////////////////////////////////////////////////
        //@bug:վ����֮���Ҵ� 2019/4/12 23:25:07
        (void)CheckKeyState(dmobj, ctxt);

        if (1 == bCopyoutPre)
        {
            code = Tz021_Fly2Copy(dmobj, ctxt);
            if (SNAIL_ERRNO_SUCCESS != code && SNAIL_ERRNO_REALIVE != code)
            {
                api_log_MsgDebug("����:����:�ȴ�:�ƶ������д���,code:%d", code);
            }

            return code;
        }

        return SNAIL_ERRNO_SUCCESS;
    }

    return SNAIL_ERRNO_EXCEPTION;
}

int Tz021_CheckLife_Inside_FightPoint_Special(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int blueThreshold = luaman_config->blueThreshold;
    int maxBlueFiled = luaman_config->maxBlueFiled;
    int minBlue = luaman_config->minBlue;
    int code = 0;
    CString strBBlood;
    CStringArray bBlood;
    char* cstr = NULL;
    int sz = 0;
    long bBloodAva = 0;
    long bBloodTot = 0;
    int strBBloodLen = 0;
    VARIANT pposX, pposY;
    long lposX = 0, lposY = 0;
    
    (void)check_alarm(dmobj, ctxt);

    dmobj->SetDict(0, FONT021_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("�����ֵ�:��Ѫ��:��ȡ:�쳣, groupid:%d, hwnd:%ld",
            luaman_config->groupid, hwnd);
        return SNAIL_ERRNO_INGORE;
    }

    cstr = strBBlood.GetBuffer(BUFFER_MAX);
    strBBlood.ReleaseBuffer();

    sz = SplitString(strBBlood, '/', bBlood);
    if (2 == sz)
    {
        bBloodAva = atol(bBlood.GetAt(0));
        bBloodTot = atol(bBlood.GetAt(1));
    }
    else
    {
        strBBloodLen = strBBlood.GetLength();
        bBloodAva = atol(strBBlood.Mid(1, strBBloodLen - maxBlueFiled));
        bBloodTot = atol(strBBlood.Mid(strBBloodLen - maxBlueFiled + 1, strBBloodLen));

        api_log_MsgDebug("�����ֵ�:Ѫ��:����:�ɹ�,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
            hwnd, bBloodAva, bBloodTot);
    }

    if (bBloodAva > blueThreshold)
    {
        return SNAIL_ERRNO_SUCCESS;
    }

    if (bBloodAva > 0)
    {
        dmobj->KeyDownChar("f3");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f3");
        LUAMAN_WAIT_MS(200);

        return SNAIL_ERRNO_SUCCESS;
    }

    api_log_MsgDebug("�����ֵ�:����:�ȴ�:���͵�:������");
    
    //�ȴ���������������
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "������˹Ұ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            api_log_MsgDebug("�����ֵ�:����:�Ѿ����͵�:������");
            break;
        }

        Delay(500);
        Check_Terminate_Thread(luaman_task, dmobj);
    }
    
    return ERRNO_REALIVE_X1;
}

//bug:���������Ѫ�������Ҫ�ı临����� 2020/12/22 21:16:22 2021/01/02 13:34:15
int Tz021_CheckLife_Outside(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int blueThreshold = luaman_config->blueThreshold;
    int maxBlueFiled = luaman_config->maxBlueFiled;
    int minBlue = luaman_config->minBlue;
    int code = 0;
    int bCopyoutPre = 1;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    CString strBBlood;
    CStringArray bBlood;
    char* cstr = NULL;
    int sz = 0;
    long bBloodAva = 0;
    long bBloodTot = 0;
    int strBBloodLen = 0;

    (void)check_alarm(dmobj, ctxt);
    
    ///////////////////////////////////////////////////////////////////////////////////
    //@bug:�����⸴��,���жϵ������֮ǰ�ǲ����ڸ������� 2019/4/24 21:14:44
    //Ĭ���ڸ�����������,����������Ҳ�޷��Ƕ�����ƶ�һ��,����Ӱ��ű��Ĺ���
    bCopyoutPre = 1;

    dmobj->SetDict(0, FONT021_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("��Ѫ��:��ȡ:�쳣, groupid:%d, hwnd:%ld",
            luaman_config->groupid, hwnd);
        return SNAIL_ERRNO_INGORE;
    }

    cstr = strBBlood.GetBuffer(BUFFER_MAX);
    strBBlood.ReleaseBuffer();

    sz = SplitString(strBBlood, '/', bBlood);
    if (2 == sz)
    {
        bBloodAva = atol(bBlood.GetAt(0));
        bBloodTot = atol(bBlood.GetAt(1));
    }
    else
    {
        strBBloodLen = strBBlood.GetLength();
        bBloodAva = atol(strBBlood.Mid(1, strBBloodLen - maxBlueFiled));
        bBloodTot = atol(strBBlood.Mid(strBBloodLen - maxBlueFiled + 1, strBBloodLen));

        api_log_MsgDebug("Ѫ��:����:�ɹ�,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
            hwnd, bBloodAva, bBloodTot);
    }

    if (bBloodAva > blueThreshold)
    {
        return SNAIL_ERRNO_SUCCESS;
    }

    if (bBloodAva > 0)
    {
        dmobj->KeyDownChar("f3");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f3");
        LUAMAN_WAIT_MS(200);

        return SNAIL_ERRNO_SUCCESS;
    }

    //������һ�·����,��ֹ�ܶ�
    (void)CheckKeyState(dmobj, ctxt);

    api_log_MsgDebug("Ӣ��:����:�ص�:������˹Ұ��:��������");

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�ȴ�������������
    for (;;)
    {
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        //����еض����������,���ȵ�� 2019/7/19 20:47:59
        dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������³�����:fff6f5:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            
            //���"�������³�����"
            for (;;)
            {
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(500);
            }
        }

        dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "ffffff-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������³�����:ffffff:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            
            //���"�������³�����"
            for (;;)
            {
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "ffffff-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                
                Delay(500);
            }
        }
        
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONT021_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:����:����:��Χ,�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("Ѫ��:���:����:����:����:��Χ,ʧ��");
        Delay(500);
    }//end:�ȴ�������������

    Delay(500);
    
    //�������
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT021_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-000000", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("Ѫ��:���:����:����:���,�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("Ѫ��:���:����:����:���,ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�ȴ�Ӣ�۸����վ����
    Delay(500);

    for (;;)
    {
        code = Tz021_CheckAlive(dmobj, ctxt, 1);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѫ��:���:����:��������,�ɹ�");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("Ѫ��:���:����:��������:�ȴ�");
        Delay(500);
    }

    return SNAIL_ERRNO_REALIVE;

}

int Tz021_CheckLife(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int blueThreshold = luaman_config->blueThreshold;
    int maxBlueFiled = luaman_config->maxBlueFiled;
    int minBlue = luaman_config->minBlue;
    int code = 0;
    int bCopyoutPre = 1;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    CString strBBlood;
    CStringArray bBlood;
    char* cstr = NULL;
    int sz = 0;
    long bBloodAva = 0;
    long bBloodTot = 0;
    int strBBloodLen = 0;
    
    (void)check_alarm(dmobj, ctxt);

    ///////////////////////////////////////////////////////////////////////////////////
    //@bug:�����⸴��,���жϵ������֮ǰ�ǲ����ڸ������� 2019/4/24 21:14:44
    //Ĭ���ڸ�����������,����������Ҳ�޷��Ƕ�����ƶ�һ��,����Ӱ��ű��Ĺ���
    bCopyoutPre = 1;
    
    dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
    code = dmobj->FindStrFast(1140, 29, 1273, 58, "������˹Ұ��", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 == code)
    {
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���������Ѵ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            bCopyoutPre = 0;
        }
        else
        {
            dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "������˹�İ�Ϣ��", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                bCopyoutPre = 2;
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////
    //bug:���������Ѫ�������Ҫ�ı临����� 2020/12/22 21:16:22 2021/01/02 13:32:15
    if (1 == bCopyoutPre)
    {
        return Tz021_CheckLife_Outside(dmobj, ctxt);
    }

    if (2 == bCopyoutPre)
    {
        return Tz021_CheckLife_Inside_FightPoint_Special(dmobj, ctxt);;
    }

    dmobj->SetDict(0, FONT021_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("��Ѫ��:��ȡ:�쳣, groupid:%d, hwnd:%ld",
            luaman_config->groupid, hwnd);
        return SNAIL_ERRNO_INGORE;
    }

    cstr = strBBlood.GetBuffer(BUFFER_MAX);
    strBBlood.ReleaseBuffer();

    sz = SplitString(strBBlood, '/', bBlood);
    if (2 == sz)
    {
        bBloodAva = atol(bBlood.GetAt(0));
        bBloodTot = atol(bBlood.GetAt(1));
    }
    else
    {
        strBBloodLen = strBBlood.GetLength();
        bBloodAva = atol(strBBlood.Mid(1, strBBloodLen - maxBlueFiled));
        bBloodTot = atol(strBBlood.Mid(strBBloodLen - maxBlueFiled + 1, strBBloodLen));

        api_log_MsgDebug("Ѫ��:����:�ɹ�,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
            hwnd, bBloodAva, bBloodTot);
    }

    if (bBloodAva > blueThreshold)
    {
        return SNAIL_ERRNO_SUCCESS;
    }

    if (bBloodAva > 0)
    {
        dmobj->KeyDownChar("f3");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f3");
        LUAMAN_WAIT_MS(200);

        return SNAIL_ERRNO_SUCCESS;
    }

    //������һ�·����,��ֹ�ܶ�
    (void)CheckKeyState(dmobj, ctxt);

    api_log_MsgDebug("Ѫ��:���:����,�ȴ����ֽ�������");
    
    //�ȴ�������������
    for (;;)
    {
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        //����еض����������,���ȵ�� 2019/7/19 20:47:59
        dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������³�����:fff6f5:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            
            //���"�������³�����"
            for (;;)
            {
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(500);
            }
        }

        dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "ffffff-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������³�����:ffffff:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            
            //���"�������³�����"
            for (;;)
            {
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "ffffff-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(500);
            }
        }
        
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONT021_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:����:����:��Χ,�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz021_CheckAlive(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����:����:����������,������Դ��������ҵ�ʩ��");
            return SNAIL_ERRNO_REALIVE;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("Ѫ��:���:����:����:����:��Χ,ʧ��");
        Delay(500);
    }//end:�ȴ�������������

    Delay(500);

    //�������
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT021_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-000000", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("Ѫ��:���:����:����:���,�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("Ѫ��:���:����:����:���,ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�ȴ�Ӣ�۸����վ����
    Delay(500);

    for (;;)
    {
        code = Tz021_CheckAlive(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѫ��:���:����:��������,�ɹ�");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("Ѫ��:���:����:��������:�ȴ�");
        Delay(500);
    }

    return SNAIL_ERRNO_REALIVE;
}

int Tz021_ExitGame(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    
    //��esc������µ�¼
    for (;;)
    {
        dmobj->KeyDownChar("esc");
        Delay(500);
        dmobj->KeyUpChar("esc");
        Delay(500);

        dmobj->SetDict(0, FONT021_DIR"font-exitgame2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ɫ", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("esc:��Ϸ����:����,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("esc:��Ϸ����:����,ʧ��");
        Delay(1000);
    }

    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONT021_DIR"font-exitgame2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ϸ��Ϣ����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("esc:��Ϸ����:���,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("esc:��Ϸ����:���,ʧ��");
        Delay(1000);
    }

    return ERRNO_EXIT;
}

int Tz021_Walk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int dir = luaman_config->direction;
    ROLE_E role = luaman_config->role;
    int groupid = luaman_config->groupid;
    int code = 0;
    struct luamanwalkstate walkstate;
    CStringArray tgtPosXY;
    CStringArray tgtPosXYe;
    int tgtNum = 0;
    char* ptr = NULL;
    int index = 0;
    long tposX = 0, tposY = 0;
    int xyc = 0;
    int rdcltErrorCounter = 0;
    CString strPosXY;
    time_t startWalkTime = time(NULL);
    time_t curTime = 0;
    double walkTimeDelta = 0.0;
    int posX = 0;
    int posY = 0;
    CStringArray posXY;
    int keyPauseTime = 20;
    VARIANT pposX, pposY;
    //VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    //long lposX2 = 0, lposY2 = 0;
    
    walkstate.xNeg = "w";
    walkstate.xPos = "s";
    walkstate.yNeg = "a";
    walkstate.yPos = "d";

    if (cdir != dir)
    {
        dir = cdir;
    }
    
    if (DIR_LEFT == dir)
    {
        walkstate.xNeg = "d";
        walkstate.xPos = "a";
        walkstate.yNeg = "w";
        walkstate.yPos = "s";
    }

    if (DIR_U == dir)
    {
        dmobj->MiddleClick();
        LUAMAN_WAIT_MS(1000);
        dmobj->MiddleClick();
        LUAMAN_WAIT_MS(1000);
    }

    tgtNum = SplitString(tgtPosXYs, '|', tgtPosXY);
    if (tgtNum < 0)
    {
        ptr = tgtPosXYs.GetBuffer(BUFFER_MAX);
        api_log_MsgError("·��:����:�Ƿ�:Ѱ·:ʧ��,tgtNum:%d, tgtPosXYs:%s", tgtNum, ptr);
        
        tgtPosXYs.ReleaseBuffer();
        return SNAIL_ERRNO_INVALID;
    }
    
    //ǰ����ÿ�������
    for (index = 0; index < tgtNum; ++index)
    {
        xyc = SplitString(tgtPosXY.GetAt(index), ':', tgtPosXYe);
        if (xyc < 2)
        {
            ptr = tgtPosXYs.GetBuffer(BUFFER_MAX);
            
            api_log_MsgError("·��:����:����:�Ƿ�:Ѱ·:ʧ��,index:%d, tgtPosXYs:%s, xyc:%d",
                index, ptr, xyc);
        
            tgtPosXYs.ReleaseBuffer();
            return ERRNO_INVALID;
        }

        tposX = atol(tgtPosXYe.GetAt(0));
        tposY = atol(tgtPosXYe.GetAt(1));

        //api_log_MsgDebug("����:����:�ɹ�,index:%d, tposX:%ld, tposY:%ld", index, tposX, tposY);

        walkstate.yNegKeyDown = 0;
        walkstate.yPosKeyDown = 0;
        walkstate.xPosKeyDown = 0;
        walkstate.xNegKeyDown = 0;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //ǰ����һĿ�������
        for (;;)
        {
        WALK_LABEL_NEXT:
            Check_Terminate_Thread(luaman_task, dmobj);
            
            //////////////////////////////////////////////////////////////////////////////
            //@bug:���Ѱ·ʱ���Ƿ񳬹���ֵ120��,�����������Ϊ�Ǳ���ס��,�˳� 2019/4/20 10:09:39
            //bug:�޸�Ϊ40�� 2022/01/10 21:57
            curTime = time(NULL);
            walkTimeDelta = difftime(curTime, startWalkTime);
            if (walkTimeDelta > 40.0)
            //if (walkTimeDelta > 120.0)
            {
                (void)CheckKeyState(dmobj, ctxt);

                api_log_MsgDebug("Ѱ·:����:��ס,ֹͣѰ·:�ȴ�������ʧ,��һ����, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //�ȴ��ض�������ʧ
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //�ȼ����Ѫ��
            code = Tz021_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("Ѱ·:Ѫ��:���,����������");
                return code;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            //��ȡ��ǰ����
            dmobj->SetDict(0, FONT021_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)CheckKeyState(dmobj, ctxt);
                
                walkstate.yNegKeyDown = 0;
                walkstate.yPosKeyDown = 0;
                walkstate.xPosKeyDown = 0;
                walkstate.xNegKeyDown = 0;

                api_log_MsgWarn("��ͼ:����:��ȡ:ʧ��");
                goto WALK_LABEL_NEXT;
            }

            //������ǰ����
            xyc = SplitString(strPosXY, ':', posXY);
            if (xyc < 2)
            {
                ptr = strPosXY.GetBuffer(BUFFER_MAX);
                
                api_log_MsgWarn("��ͼ:����:����:ʧ��,strPosXY:%s, xyc:%d",
                    ptr, xyc);
            
                strPosXY.ReleaseBuffer();

                (void)CheckKeyState(dmobj, ctxt);
                
                walkstate.yNegKeyDown = 0;
                walkstate.yPosKeyDown = 0;
                walkstate.xPosKeyDown = 0;
                walkstate.xNegKeyDown = 0;
                
                goto WALK_LABEL_NEXT;
            }

            posX = atol(posXY.GetAt(0));
            posY = atol(posXY.GetAt(1));

            if (posX - tposX > 1)
            {
                if (posY - tposY > 1)
                {
                    /* ������ */
                    dmobj->KeyDownChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 1;
                    
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yNegKeyDown = 1;
                }
                else if (posY - tposY < -1)
                {
                    /* ������ */
                    dmobj->KeyDownChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 1;

                    dmobj->KeyDownChar(walkstate.yPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yPosKeyDown = 1;
                }
                else
                {
                    if (1 == walkstate.yNegKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yNeg);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yNegKeyDown = 0;
                    }
                    
                    if (1 == walkstate.yPosKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yPos);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yPosKeyDown = 0;
                    }

                    /* Y�����겻��,X���ƶ� */
                    dmobj->KeyDownChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 1;
                }
            }
            else if (posX - tposX < -1)
            {
                if (posY - tposY > 1)
                {
                    /* ������ */
                    dmobj->KeyDownChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 1;
                    
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yNegKeyDown = 1;
                }
                else if (posY - tposY < -1)
                {
                    /* ������ */
                    dmobj->KeyDownChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 1;
                    
                    dmobj->KeyDownChar(walkstate.yPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yPosKeyDown = 1;
                }
                else
                {
                    if (1 == walkstate.yNegKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yNeg);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yNegKeyDown = 0;
                    }

                    if (1 == walkstate.yPosKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yPos);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yPosKeyDown = 0;
                    }

                    dmobj->KeyDownChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 1;
                }
            }
            else
            {
                if (1 == walkstate.xPosKeyDown)
                {
                    dmobj->KeyUpChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 0;
                }

                if (1 == walkstate.xNegKeyDown)
                {
                    dmobj->KeyUpChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 0;
                }

                /* X�����겻��,ֻ�ж�Y�� */
                if (posY - tposY > 1)
                {
                    /* ������ */
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yNegKeyDown = 1;
                }
                else if (posY - tposY < -1)
                {
                    /* ������ */
                    dmobj->KeyDownChar(walkstate.yPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yPosKeyDown = 1;
                }
                else
                {
                    if (1 == walkstate.yNegKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yNeg);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yNegKeyDown = 0;
                    }

                    if (1 == walkstate.yPosKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yPos);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yPosKeyDown = 0;
                    }

                    if (1 == walkstate.xPosKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.xPos);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.xPosKeyDown = 0;
                    }

                    if (1 == walkstate.xNegKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.xNeg);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.xNegKeyDown = 0;
                    }

                    LUAMAN_WAIT_MS(keyPauseTime);

                    //��ǰĿ������㴦�����
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            //WALK_LABEL_NEXT
        }//end:ǰ����һĿ�������

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    //Ѱ·�������,˳������Ŀ���յ������
    return SNAIL_ERRNO_SUCCESS;

LABEL_WALK_BLOCK_EXIT_CHECK:
    //�鿴'������ʧ�ȴ�'����,�ҵ���֮��������9����,���ȴ��˳���������
    //bug:ע�͵���� 2022/01/10 21:58:49
    /*
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-waitcopyoff.txt");
        code = dmobj->FindStrFast(1, 160, 190, 200, "������ʧ�ȴ�", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ѱ·:��ס:������ʧ�ȴ�:����,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        //���Ѫ��
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:��ס:������ʧ�ȴ�:Ѫ��:���,����������");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        //api_log_MsgDebug("Ѱ·:��ס:������ʧ�ȴ�:����,ʧ��");
    }
    */

    //����ͼ�Ƿ��˳�����������
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "������˹Ұ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ѱ·:��ס:��ͼ:������˹Ұ��:�˳�,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("Ѱ·:��ס:��ͼ:������˹Ұ��:�˳�,�����˳���");
    }

    return SNAIL_ERRNO_BLOCKED;
}

int Tz021_FightAutoStart(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    int code = 0;
    VARIANT pposX, pposY;
    long lposX = 0, lposY = 0;
    VARIANT pposX2, pposY2;
    long lposX2 = 0, lposY2 = 0;
    
    for (;;)
    {
        dmobj->MoveTo(105, 143);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONT021_DIR"font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ʼ", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("ս��:����:��:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("ս��:����:��:ʧ��");
        LUAMAN_WAIT_MS(500);

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("ս��:����:��:����:����");
            return code;
        }
    }

    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(500);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(500);

        dmobj->SetDict(0, FONT021_DIR"font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ʼ", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ս��:����:��ʼ:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("ս��:����:��ʼ:ʧ��,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        LUAMAN_WAIT_MS(500);

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("ս��:����:��ʼ:����:����");
            return code;
        }
    }

    return SNAIL_ERRNO_SUCCESS;
}

int Tz021_FightAutoStop(Idmsoft* dmobj, void* ctxt)
{
    char* xNeg = "w";
    char* xPos = "s";

    dmobj->KeyDownChar(xNeg);
    LUAMAN_WAIT_MS(25);
    dmobj->KeyUpChar(xNeg);
    LUAMAN_WAIT_MS(25);
    
    dmobj->KeyDownChar(xPos);
    LUAMAN_WAIT_MS(25);
    dmobj->KeyUpChar(xPos);
    LUAMAN_WAIT_MS(25);

    (void)dmobj;
    (void)ctxt;
    
    return SNAIL_ERRNO_SUCCESS;
}


int Tz021_FightInFixPoint_atc(Idmsoft* dmobj, void* ctxt, struct fight021_param* params)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int code = -1;
    int uFailedCount = 0;
    int cindex = -1;
    int index = -1;
    time_t lstAdjustTime = time(NULL);
    time_t thisAdjustTime = time(NULL);
    double deltaAdjustTime = 0.0;
    time_t lstChkBluebloodTime = time(NULL);
    int uFlag = 0;
    int jndex = 0;
    time_t lstUtime = time(NULL);
    double deltaUtime = 0.0;
    char* fposXY = params->fposXY;
    char* ufontName = params->ufontName;
    char* uName = params->uName;
    char* uclrOffset = params->uclrOffset;
    unsigned int uFailedCountMax = params->uFailedCountMax;
    VARIANT pposX, pposY;
    //VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    //long lposX2 = 0, lposY2 = 0;
    time_t curTime = time(NULL);
    time_t startWalkTime = time(NULL);
    time_t lastUaodesayTime = 0;
    double walkTimeDelta = 0.0;
    double lastPointBattleTime = 60.0;
    int curPointIndex = 0;
    char* boxPosXYs[2][3] =
    {
        {"188:148", "178:163", ""},
        {"125:134", "", ""},
    };
    char* posXY = NULL;
    CString cstrposXY;
    int end_after_try_last = 0;
    
    if (!strcmp(fposXY, "254:254"))
    {
        uFlag = 1;
    }

Label_for_move_to_next_pos:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    code = -1;
    uFailedCount = 0;
    cindex = -1;
    index = -1;
    lstAdjustTime = time(NULL);
    thisAdjustTime = time(NULL);
    deltaAdjustTime = 0.0;
    lstChkBluebloodTime = time(NULL);
    uFlag = 0;
    jndex = 0;
    lstUtime = time(NULL);
    deltaUtime = 0.0;

    code = Tz021_FightAutoStart(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code)
    {
        api_log_MsgDebug("����:���:��ʼ:Ѫ��:����,����");
        return code;
    }

Label_for_battle_last_point:
    dmobj->MoveTo(5, 5);
    Delay(100);

    //������ֱ���жϹ���û����
    if (!strcmp(fposXY, "254:254"))
    {
        uFlag = 1;
    }

    uFailedCount = 0;
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        if (strcmp(ufontName, "") && strcmp(uName, "") && (1 == uFlag))
        {
            dmobj->SetDict(0, ufontName);
            code = dmobj->FindStrFast(0, 27, 1279, 826, uName, uclrOffset, 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                uFailedCount = 0;
                uFailedCountMax = 3;

                api_log_MsgDebug("����:u��:%s:�ɹ�, lposX:%ld, lposY:%ld, fposXY:%s",
                    uName, lposX, lposY, fposXY);

                lastUaodesayTime = time(NULL);
                goto Label_for_check_procedure_end;
            }
            
            dmobj->SetDict(0, ufontName);
            code = dmobj->FindStrFast(0, 27, 1279, 826, uName, "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                uFailedCount = 0;
                uFailedCountMax = 3;

                api_log_MsgDebug("����:u��:��ɫ:%s:�ɹ�, lposX:%ld, lposY:%ld, fposXY:%s",
                    uName, lposX, lposY, fposXY);

                lastUaodesayTime = time(NULL);
                goto Label_for_check_procedure_end;
            }

            dmobj->SetDict(0, ufontName);
            code = dmobj->FindStrFast(0, 27, 1279, 826, uName, "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                uFailedCount = 0;
                uFailedCountMax = 3;

                api_log_MsgDebug("����:u��:��ɫ:%s:�ɹ�, lposX:%ld, lposY:%ld, fposXY:%s",
                    uName, lposX, lposY, fposXY);

                lastUaodesayTime = time(NULL);
                goto Label_for_check_procedure_end;
            }

            //��һ��u����ʱ��¼ʱ��
            if (0 == uFailedCount)
            {
                lastUaodesayTime = time(NULL);
                api_log_MsgDebug("����:u��:%s:ʧ��, fposXY:%s", uName, fposXY);
            }
            
            uFailedCount = uFailedCount + 1;
            goto Label_for_check_procedure_end;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //�ڶ������
        if (!strcmp(fposXY, "127:156"))
        {
            dmobj->SetDict(0, FONT021_DIR"font-nightrose.txt");
            code = dmobj->FindStrFast(2, 462, 385, 616, "���ƶ���������˹��ĹѨ��", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                (void)Tz021_FightAutoStop(dmobj, ctxt);

                api_log_MsgDebug("���ƶ���������˹��ĹѨ:����:�ɹ�:����:ս��");
                return SNAIL_ERRNO_SUCCESS;
            }

            api_log_MsgDebug("����:���:�ڶ���:����:%s", uName);

            //////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:2021/6/8 22:07:22 ���ش�
            curTime = time(NULL);
            walkTimeDelta = difftime(curTime, startWalkTime);
            //bug:��300���޸�Ϊ210�� 2022/01/08 15:40:15
            //if (walkTimeDelta > 300.0)
            if (walkTimeDelta > 210.0)
            {
                if (curPointIndex < 1)
                {
                    startWalkTime = time(NULL);
                    posXY = boxPosXYs[1][curPointIndex];
                    curPointIndex++;
                    
                    api_log_MsgDebug("����:���:�ڶ���:С��:δ����:�Ƶ�:����:%s", posXY);
                    
                    cstrposXY = CString(posXY);
                    //code = Tz021_Walk(dmobj, ctxt, cstrposXY, DIR_U);
                    code = Tz021_Walk(dmobj, ctxt, cstrposXY, luaman_config->direction);
                    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                    {
                        api_log_MsgDebug("����:���:�ڶ���:С��:δ����:�Ƶ�:����:%s:����", posXY);
                        return code;
                    }
                    
                    api_log_MsgDebug("����:���:�ڶ���:С��:δ����:�Ƶ�:����:%s:�ɹ�", posXY);
                    goto Label_for_move_to_next_pos;
                }

                api_log_MsgDebug("ȱ��:����:���:�ڶ���:С��:��ʱ:�˳�");
                return SNAIL_ERRNO_SUCCESS;
            }

            uFailedCount = 0;
            goto Label_for_check_procedure_end;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //��һ�����
        if (!strcmp(fposXY, "167:140"))
        {
            if (0 == uFlag)
            {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //bug:�����½ǵ�������˹����Ƭ��þͲ�����,ֱ���˳�������һ�� 2021/06/14 11:26:44
                dmobj->SetDict(0, FONT021_DIR"font-catch-linpin.txt");
                code = dmobj->FindStrFast(2, 462, 385, 616, "������˹����Ƭ��á�", "6495ed-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;
                    
                    (void)Tz021_FightAutoStop(dmobj, ctxt);
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    api_log_MsgDebug("������˹����Ƭ��á�:����:�ɹ�:���:λ��1:�ɹ�");
                    return SNAIL_ERRNO_SUCCESS;
                }

                //bug:���Ӱµ���С�ֽ������� 2022/01/15 10:17:32
                dmobj->SetDict(0, FONT021_DIR"font-npc101.txt");
                code = dmobj->FindStrFast(2, 462, 385, 616, "��ý��:", "6495ed-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    (void)Tz021_FightAutoStop(dmobj, ctxt);
                    Check_Terminate_Thread(luaman_task, dmobj);

                    api_log_MsgDebug("����:��С��:��ý��:����:�ɹ�:���:���,fposXY:%s", fposXY);
                    return SNAIL_ERRNO_SUCCESS;
                }
                
                dmobj->SetDict(0, FONT021_DIR"font-checkctn.txt");
                code = dmobj->FindStrFast(2, 462, 385, 616, "���������µ����Ϻŵĸ�����", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uFlag = 1;
                    uFailedCount = 0;
                    uFailedCountMax = 20;

                    api_log_MsgDebug("����:���:��һ��:%s:С�ִ���:����", uName);
                }
                else
                {
                    api_log_MsgDebug("����:���:��һ��:%s:С�ִ���:δ����", uName);

                    curTime = time(NULL);
                    walkTimeDelta = difftime(curTime, startWalkTime);

                    //bug:���һ�����4����,ǰ������ֻ��1���� 2022/01/10 22:35:19
                    lastPointBattleTime = (end_after_try_last)? 60.0 * 4 : 60.0;
                    
                    if (walkTimeDelta > lastPointBattleTime)
                    {
                        if (curPointIndex < 2)
                        {
                            startWalkTime = time(NULL);
                            posXY = boxPosXYs[0][curPointIndex];
                            curPointIndex++;

                            api_log_MsgDebug("����:���:��һ��:С��:δ����:�Ƶ�:����:%s", posXY);
                    
                            cstrposXY = CString(posXY);
                            //code = Tz021_Walk(dmobj, ctxt, cstrposXY, DIR_U);
                            code = Tz021_Walk(dmobj, ctxt, cstrposXY, luaman_config->direction);
                            if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                            {
                                api_log_MsgDebug("��һ��:Ѱ·:ǰ��:����:%s:����", posXY);
                                return code;
                            }

                            api_log_MsgDebug("����:���:��һ��:С��:δ����:�Ƶ�:����:%s:�ɹ�", posXY);
                            goto Label_for_move_to_next_pos;
                        }

                        //////////////////////////////////////////////////////////////////
                        //bug: ���һ�������2���ӻ�û�н�����Ĭ�Ͻ��� 2022/01/08 23:07:11
                        //bug: �����һ�δ��껹û���ҵ��ͼ�����һ�� 2022/01/14 21:26:33
                        if (2 == end_after_try_last)
                        {
                            (void)Tz021_FightAutoStop(dmobj, ctxt);

                            api_log_MsgDebug("���:���һ����:%s:��ʱ:����:Ĭ��:�ɹ�", posXY);
                            return ERRNO_NEXTCOPY;
                        }
                        
                        end_after_try_last++;

                        api_log_MsgDebug("���:���һ����:%s:�ٴ�4����:Ѱ·:����", posXY);
                        
                        //bug:��֮ǰѰ·�ѹ������� 2022/01/10 22:36:55
                        posXY = "180:150|180:135";
                        cstrposXY = CString(posXY);
                        code = Tz021_Walk(dmobj, ctxt, cstrposXY, luaman_config->direction);
                        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                        {
                            api_log_MsgDebug("���һ����:Ѱ·:ǰ��:����:%s:����", posXY);
                            return code;
                        }

                        Delay_With_Terminate_Check(2, luaman_task, dmobj);

                        posXY = "180:135|180:150";
                        cstrposXY = CString(posXY);
                        code = Tz021_Walk(dmobj, ctxt, cstrposXY, luaman_config->direction);
                        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                        {
                            api_log_MsgDebug("���һ����:Ѱ·:ǰ��:����:%s:����", posXY);
                            return code;
                        }
                        
                        startWalkTime = time(NULL);
                        
                        api_log_MsgDebug("���:���һ����:%s:�ٴ�4����:%d", posXY, end_after_try_last);

                        code = Tz021_FightAutoStart(dmobj, ctxt);
                        if (SNAIL_ERRNO_REALIVE == code)
                        {
                            api_log_MsgDebug("����:���һ����:���:��ʼ:Ѫ��:����,����:%d", end_after_try_last);
                            return code;
                        }
                        goto Label_for_battle_last_point;
                        //////////////////////////////////////////////////////////////////
                    }
                    
                }//"���������µ����Ϻŵĸ�����"
                
            }//if (0 == uFlag)
        }
        
        uFailedCount = 0;

    Label_for_check_procedure_end:
        Check_Terminate_Thread(luaman_task, dmobj);
        
        if (uFailedCount > uFailedCountMax)
        {
            //bug:��ӽ������� 2022/01/15 10:01:08
            if (!strcmp(fposXY, "167:140"))
            {
                dmobj->SetDict(0, FONT021_DIR"font-npc101.txt");
                code = dmobj->FindStrFast(2, 462, 385, 616, "��ý��:", "6495ed-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    (void)Tz021_FightAutoStop(dmobj, ctxt);
                    Check_Terminate_Thread(luaman_task, dmobj);

                    api_log_MsgDebug("����:���:��ý��:����:�ɹ�:���:���,fposXY:%s", fposXY);
                    return SNAIL_ERRNO_SUCCESS;
                }

                //���û���ҵ��Ϳ�����uʧ�ܵ�ʱ���Ƿ񳬹�30��,�������˳�
                curTime = time(NULL);
                walkTimeDelta = difftime(curTime, lastUaodesayTime);
                if (walkTimeDelta > 30.0)
                {
                    (void)Tz021_FightAutoStop(dmobj, ctxt);
                    Check_Terminate_Thread(luaman_task, dmobj);

                    api_log_MsgDebug("����:���:����u:30��ʧ��:%d:���:���,fposXY:%s", uFailedCount, fposXY);
                    return SNAIL_ERRNO_SUCCESS;
                }

                api_log_MsgDebug("����:���:����u:%d:���:����,fposXY:%s", uFailedCount, fposXY);
                goto Label_continue_u_aodesay;
            }
            
            (void)Tz021_FightAutoStop(dmobj, ctxt);

            api_log_MsgDebug("����:u��:����:%d��:�������Ѵ��괦��:%s", uFailedCountMax, fposXY);

            for (jndex = 0; jndex <= 20; jndex++)
            {
                dmobj->KeyDown(32);
                Delay(50);
                dmobj->KeyUp(32);
                Delay(50);

                Check_Terminate_Thread(luaman_task, dmobj);
            }

            return SNAIL_ERRNO_SUCCESS;
        }

    Label_continue_u_aodesay:
        Delay(500);

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����:���:Ѫ��:����,����,fposXY:%s", fposXY);
            return code;
        }

        if (SNAIL_ERRNO_SUCCESS == code)
        {
            if (!strcmp(fposXY, "127:156"))
            {
                dmobj->SetDict(0, FONT021_DIR"font-nightrose.txt");
                code = dmobj->FindStrFast(2, 462, 385, 616, "���ƶ���������˹��ĹѨ��", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    (void)Tz021_FightAutoStop(dmobj, ctxt);

                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    //���ո����
                    for (jndex = 0; jndex <= 10; jndex++)
                    {
                        dmobj->KeyDown(32);
                        Delay(50);
                        dmobj->KeyUp(32);
                        Delay(50);
                    }

                    ////////////////////////////////////////////////////////////////////////
                    //@bug: ��0�����"���ƶ���������˹��ĹѨ��"���� 2019/5/14 22:48:13
                    for (jndex = 0; jndex <= 6; jndex++)
                    {
                        dmobj->KeyDown(48);
                        Delay(50);
                        dmobj->KeyUp(48);
                        Delay(50);
                    }

                    api_log_MsgDebug("����:���:���ƶ���������˹��ĹѨ��:�ɹ�,fposXY:%s", fposXY);
                    return SNAIL_ERRNO_SUCCESS;
                }
            }
        }
    }//for end

    (void)Tz021_FightAutoStop(dmobj, ctxt);

    api_log_MsgDebug("����:���:�쳣,�����ܴﵽ�÷�֧����,fposXY:%s", fposXY);
    return SNAIL_ERRNO_SUCCESS;
}

int Tz021_CleanPackage(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    //VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    //long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int code = 0;
    int boxPosX0 = -1;
    int boxPosY0 = -1;
    int boxPosX1 = -1;
    int boxPosY1 = -1;
    int lineX = -1;
    int lineY = -1;
    struct xyposition labelPosXY[5];
    struct xyposition boxPosXYs[5][10];
    int deltaX = 30;
    int deltaY = 30;
    int index = 0;
    int jndex = 0;
    int ttndex = 0;
    int posX = 0;
    int posY = 0;
    
    dmobj->MoveTo(5, 5);
    Delay(100);

    //bug:��������� 2022/01/08 18:36:11
    return SNAIL_ERRNO_SUCCESS;

    for (;;)
    {
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT021_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("����:����:ʧ��");
        Delay(500);
    }

    //bug:�����������λ�� 2021/6/18 10:11:19
    //841:93->667:505 1037:687
    boxPosX0 = lposX2 - 174;
    boxPosY0 = lposY2 + 412;
    boxPosX1 = lposX2 + 196;
    boxPosY1 = lposY2 + 594;

    labelPosXY[0].x = lposX2 - 70;
    labelPosXY[0].y = lposY2 + 400;

    labelPosXY[1].x = lposX2 - 10;
    labelPosXY[1].y = lposY2 + 400;

    labelPosXY[2].x = lposX2 + 50;
    labelPosXY[2].y = lposY2 + 400;

    labelPosXY[3].x = lposX2 + 110;
    labelPosXY[3].y = lposY2 + 400;

    labelPosXY[4].x = lposX2 + 170;
    labelPosXY[4].y = lposY2 + 400;
    
    lineX = boxPosX0 + deltaX + 3;
    lineY = boxPosY0 + deltaY + 3;

    for (index = 0; index <= 4; index++)
    {
        posY = lineY + index * deltaY;
        for (jndex = 0; jndex <= 9; jndex++)
        {
            posX = lineX + jndex * deltaX;
            
            boxPosXYs[index][jndex].x = posX;
            boxPosXYs[index][jndex].y = posY;

            api_log_MsgDebug("boxPosXYs[%d][%d]:%d:%d", index, jndex, posX, posY);
        }
    }

    dmobj->MoveTo(labelPosXY[0].x, labelPosXY[0].y);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    for (jndex = 0; jndex <= 4; jndex++)
    {
        posX = boxPosXYs[0][jndex].x;
        posY = boxPosXYs[0][jndex].y;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(posX, posY);
        Delay(200);
        dmobj->KeyDownChar("ctrl");
        Delay(200);
        
        for(ttndex = 0; ttndex <= 10; ttndex++)
        {
            dmobj->LeftClick();
            Delay(100);
        }
    
        dmobj->KeyUpChar("ctrl");
        Delay(250);
    }

    posX = boxPosXYs[1][0].x;
    posY = boxPosXYs[1][0].y;
    
    dmobj->MoveTo(posX, posY);
    Delay(200);
    dmobj->KeyDownChar("ctrl");
    Delay(250);

    for(ttndex = 0; ttndex <= 10; ttndex++)
    {
        dmobj->LeftClick();
        Delay(100);
    }
    dmobj->KeyUpChar("ctrl");
    Delay(250);

    dmobj->MoveTo(5, 5);
    Delay(100);

    for (;;)
    {
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT021_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            api_log_MsgDebug("����:�ر�:�ɹ�");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("����:�ر�:ʧ��");
        Delay(500);
    }

    return SNAIL_ERRNO_SUCCESS;
}

int Tz021_FinishTask(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    unsigned int playWndIndexVal = luaman_config->playWndIndexVal;
    int exitGameCopyFull = luaman_config->exitGameCopyFull;
    int wait2sleep = 0;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int copyFullCounter = 0;
    int retryPreFly = 0;
    int lastPos = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    struct fight021_param params;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int nfailed = 0;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    //@bug:��������:�����ոս���֮�������ű� @2019/4/15 21:28:44
    dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
    code = dmobj->FindStrFast(1140, 29, 1273, 58, "���������Ѵ�", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("����:��ͼ:���������Ѵ�:�����ű�,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
        goto LABEL_START_SCRIPT_IN_COPY;
    }

    api_log_MsgDebug("����:��ͼ:������˹Ұ��:�����ű�,�ɹ�, playWndIndexVal:%u", playWndIndexVal);

    if (playWndIndexVal > 0)
    {
        int ij = 0;
        for (ij = 0; ij < playWndIndexVal; ij++)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
        }
    }

COPY_LABEL_AGAIN:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    copyFullCounter = 0;
    retryPreFly = 0;

COPY_LABEL_TALK_WITH_NPC:
    api_log_MsgDebug("����:npc1:�Ի�:��ʼ");

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //��n�Ի����븱��
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONT021_DIR"font-npc1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���������Ѵ��볡", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���������Ѵ��볡:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:�Ի�:���������Ѵ��볡:����:����:����");
            goto COPY_LABEL_TALK_WITH_NPC;
        }
        
        retryPreFly = retryPreFly + 1;
        if (retryPreFly > 5)
        {
            api_log_MsgDebug("�ƶ�������:retryPreFly:%d", retryPreFly);
            
            code = Tz021_Fly2Copy(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc1:�Ի�:���������Ѵ��볡:����:�������Ի�:�ƶ�:�ɹ�");
                goto COPY_LABEL_TALK_WITH_NPC;
            }
        }

        api_log_MsgDebug("npc1:�Ի�:���������Ѵ��볡:����:��Χ,ʧ��");
        Delay(200);
    }

    //���'���������Ѵ��볡'
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        //����Ƿ����'ȷ��'�����ĶԻ���
        dmobj->SetDict(0, FONT021_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("npc1:�Ի�:ȷ��:����:��Χ,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:�Ի�:ȷ��:����:����:����");
            goto COPY_LABEL_TALK_WITH_NPC;
        }

        api_log_MsgDebug("npc1:�Ի�:ȷ��:����:��Χ,ʧ��");
        Delay(200);
    }
    
    //���'ȷ��'�����Ի���
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:��ʡʱ�� 2020/2/25 21:17:16
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:�Ի�:ȷ��:���:����:����");
            goto COPY_LABEL_TALK_WITH_NPC;
        }

        //����Ƿ����'�������³�����'�����ĶԻ���
        dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("npc1:�Ի�:�������³�����:����:��Χ,�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            
            //���"�������³�����"
            for (;;)
            {
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(500);
            }

            /////////////////////////////////////////////////////////////////////////////////////////////
            //@bug:�������³�����������5�����˳���Ϸ 2019/5/19 23:08:15
            if (copyFullCounter > 5)
            {
                if (1 == exitGameCopyFull)
                {
                    return Tz021_ExitGame(dmobj, ctxt);
                }
            }
            else
            {
                copyFullCounter++;
            }

            api_log_MsgDebug("npc1:�Ի�:�������³�����:���¶Ի�,copyFullCounter:%d", copyFullCounter);
            goto COPY_LABEL_TALK_WITH_NPC;
        }//end:"�������³�����"

        //����Ƿ���븱��
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���������Ѵ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc1:�Ի�:���������Ѵ�:����,�ɹ�,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        //���Ѫ��
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:�Ի�:���������Ѵ�:����,����������");
            goto COPY_LABEL_AGAIN;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(200);
        api_log_MsgDebug("npc1:�Ի�:���������Ѵ�,���ڽ���");
    }//end:���'ȷ��'�����Ի���
    
LABEL_START_SCRIPT_IN_COPY:
    if (2 == lastPos)
    {
        dmobj->KeyDownChar("t");
        Delay(500);
        dmobj->KeyUpChar("t");
        Delay(500);

        api_log_MsgDebug("����:����:����:��ת:�ڶ���");//2037

        pathofposxys = "199:152|174:152|155:137|142:136|133:135|133:143|132:151";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("����:����:����:��ת:ǰ��:����:[132:151],����������");
            
            Delay(1000);
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:����:����:��ת:ǰ��:����:[132:151],�ɹ�");
        api_log_MsgDebug("����:����:����:��ת:ǰ��:����:[132:151]:�Ի�:׼��:���");

        Check_Terminate_Thread(luaman_task, dmobj);
        
        goto Label_continue_for_step_2;
    }

    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyDownChar("u");
    Delay(500);
    dmobj->KeyUpChar("u");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyDown(192);
    Delay(500);
    dmobj->KeyUp(192);
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�ȴ����ϽǵĿ�ʼʱ��������ʱ��
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-readygo.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����ʱ��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����ʱ��:����:����:����");
            
            Delay(1000);

            Check_Terminate_Thread(luaman_task, dmobj);
            goto COPY_LABEL_AGAIN;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("����ʱ��:����:ʧ��");
    }

    /******************************************************************************/
    //�ƶ���174:152���괦
    pathofposxys = "176:155|167:140";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[167:140],����������");
        
        Delay(1000);
        goto COPY_LABEL_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("Ѱ·:ǰ��:����:[167:140],�ɹ�");

LABEL_BATTLE_WITH_TARGET:
    params.fposXY = "167:140";
    params.ufontName = FONT021_DIR"font-audesay.txt";
    params.uName = "�µ���";
    params.uclrOffset = "ff3300-101010";
    params.uFailedCountMax = 120;
    
    code = Tz021_FightInFixPoint_atc(dmobj, ctxt, &params);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����:���:����:[167:140],����������");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("����:���:[167:140],�ɹ�");

    if (ERRNO_NEXTCOPY == code)
    {
        api_log_MsgDebug("167:140:���:��ʱ:ֱ������:���һ��:����");
        goto Label_wait_to_exec_next_copy_master;
    }

    if (0 == lastPos)
    {
        lastPos = 1;
    }

    /******************************************************************************/
    //�ƶ���132:151���괦
    pathofposxys = "167:140|155:137|142:136|133:135|133:143|132:151";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[132:151],����������");
        
        Delay(1000);
        goto COPY_LABEL_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    api_log_MsgDebug("Ѱ·:ǰ��:����:[132:151],�ɹ�");

Label_continue_for_step_2:
    for (;;)
    {
        dmobj->KeyPressChar("n");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("n");
        LUAMAN_WAIT_MS(200);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(100);

        dmobj->SetDict(0, FONT021_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ������ڡ�", "00ff00-000000", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("npc2:�Ի�:�ƶ������ڡ�:����:��Χ,�ɹ�,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("npc2:�Ի�:�ƶ������ڡ�:����:����:����");
            goto COPY_LABEL_TALK_WITH_NPC;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(200);
        api_log_MsgDebug("npc2:�Ի�:�ƶ������ڡ�:����:��Χ,ʧ��");
    }

    //���'�ƶ������ڡ�'
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONT021_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ������ڡ�", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("npc2:�Ի�:�ƶ������ڡ�:���,�ɹ�");
            break;
        }

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("npc2:�Ի�:�ƶ������ڡ�:���:����:����");
            goto COPY_LABEL_TALK_WITH_NPC;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("npc2:�Ի�:�ƶ������ڡ�:���,ʧ��");
    }

    //��鵱ǰ����,������겻�� 119:157 ������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 119) <= 1 && abs(posY - 157) <= 1)
                {
                    api_log_MsgDebug("�ƶ������ڡ�:�Ի�:�ɹ�");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("�ƶ������ڡ�:�ƶ�:������...");
        Delay(500);
    }
    
    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�ƶ���127:156���괦
    pathofposxys = "127:156";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[127:156],����������");
        
        Delay(1000);

        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[127:156],�ɹ�");

    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (1 == lastPos)
    {
        lastPos = 2;
    }

    dmobj->SetDict(0, FONT021_DIR"font-nightrose.txt");
    code = dmobj->FindStrFast(2, 462, 385, 616, "���ƶ���������˹��ĹѨ��", "00ff00-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("����:������˹:�ɹ�:����:����2, lposX:%ld, lposY:%ld", lposX, lposY);
        goto Label_skip_battle_at_room_2;
    }

    params.fposXY = "127:156";
    params.ufontName = "";
    params.uName = "";
    params.uclrOffset = "";
    params.uFailedCountMax = 150;
    
    code = Tz021_FightInFixPoint_atc(dmobj, ctxt, &params);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����:���:����:[127:156],����������");
        
        Delay(1000);

        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("����:���:[127:156],�ɹ�");

Label_skip_battle_at_room_2:
    if (2 == lastPos)
    {
        lastPos = 3;
    }

    //��F5
    dmobj->KeyDown(116);
    Delay(500);
    dmobj->KeyUp(116);
    Delay(1000);

    //////////////////////////////////////////////////////////////////////////////////
    //�ƶ���121:125���괦
    //bug:ͣ������ 2022/01/26 22:41:33
    //pathofposxys = "127:155|124:145|124:135|117:133|118:129|121:125";
    pathofposxys = "127:155|124:145|124:135|117:133";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[117:133],����������");
        
        Delay(1000);

        Check_Terminate_Thread(luaman_task, dmobj);

        if (SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("��ס:ֱ������:���һ��:��ʼ:��һ����");
            goto Label_wait_to_exec_next_copy_master;
        }
        goto COPY_LABEL_AGAIN;
    }

    Delay_With_Terminate_Check(2, luaman_task, dmobj);
    
    api_log_MsgDebug("Ѱ·:ǰ��:����:[117:133],�ɹ�");
    
    pathofposxys = "118:129|121:125";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[121:125],����������");
        
        Delay(1000);

        Check_Terminate_Thread(luaman_task, dmobj);

        if (SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("��ס:ֱ������:���һ��:��ʼ:��һ����");
            goto Label_wait_to_exec_next_copy_master;
        }
        goto COPY_LABEL_AGAIN;
    }

    //////////////////////////////////////////////////////////////////////////////////
    
    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("Ѱ·:ǰ��:����:[121:125],�ɹ�");
    api_log_MsgDebug("����:[121:125]:�ȴ�:��ͣ����");

    //�ȴ�һ��
    wait2sleep = 0;

    for (;;)
    {
        if (wait2sleep > luaman_config->Wait2Sleep)
        {
            api_log_MsgDebug("����:[121:125]:�ȴ�:ʱ�䵽:�ָ�����");
            break;
        }

        api_log_MsgDebug("����:[121:125]:�ȴ�:ʱ�䵽:%d:%d", wait2sleep, luaman_config->Wait2Sleep);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        wait2sleep++;
        Delay(1000);
    }

    //u����ʯ��~����
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(500);

    nfailed = 0;
    
    //bug:ԭ���ǰ�u�ٰ�`,�޸�Ϊ��n�����Ի� 2022/01/06 22:17:59
    for (;;)
    {
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT021_DIR"font-npc100.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ���������˹��Ϣ��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����ʯ:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);

        nfailed++;
        
        api_log_MsgDebug("����ʯ:����:ʧ��,nfailed:%d", nfailed);
        Delay(500);
        
        if (nfailed > 30)
        {
            api_log_MsgDebug("����ʯ:����:ʧ��:�ȴ������ͳ�ȥ");
            goto Label_wait_to_exec_next_copy_master;
        }
    }

    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT021_DIR"font-npc100.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ���������˹��Ϣ��", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ���������˹��Ϣ��:����:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("�ƶ���������˹��Ϣ��:����:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    //������260:235
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 260) <= 1 && abs(posY - 235) <= 1)
                {
                    api_log_MsgDebug("������260:235:�ɹ�:�ɹ�");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("������260:235:������...");
        Delay(1000);
    }

    //bug:������֮��F1 2021/06/14 11:17:15
    dmobj->KeyDownChar("f1");
    Delay(200);
    dmobj->KeyUpChar("f1");
    Delay(200);
    dmobj->MoveTo( 5, 5);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�ƶ���254:254���괦
    pathofposxys = "260:235|255:249|254:254";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[254:254],����������");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);

        if (SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("��ס:ֱ������:���һ��:��ʼ:��һ����");
            goto Label_wait_to_exec_next_copy_master;
        }
        goto COPY_LABEL_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    api_log_MsgDebug("Ѱ·:ǰ��:����:[254:254],�ɹ�");

    if (3 == lastPos)
    {
        lastPos = 4;
    }

    params.fposXY = "254:254";
    params.ufontName = FONT021_DIR"font-unightrose.txt";
    params.uName = "������˹";
    params.uclrOffset = "ff3300-101010";
    params.uFailedCountMax = 60;
    
    code = Tz021_FightInFixPoint_atc(dmobj, ctxt, &params);
    if (ERRNO_REALIVE_X1 == code)
    {
        api_log_MsgDebug("����:���:����:[254:254],����:����:�ڸ�����");
        goto Label_finish_this_copy_master;
    }
    
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����:���:����:[254:254],����������");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);

        if (SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("��ס:ֱ������:���һ��:��ʼ:��һ����");
            goto Label_wait_to_exec_next_copy_master;
        }
        
        goto COPY_LABEL_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    api_log_MsgDebug("����:���:[254:254],�ɹ�");

    if (4 == lastPos)
    {
        lastPos = 5;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //bug:ע�͵� 2022/01/06 22:02:18
    /*
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(500);

    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-success_end.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���Ѵ���ת��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����:��ʼ:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����:��ʼ:ʧ��");
        Delay(500);
    }
    
    dmobj->MoveTo(lposX + 27, lposY + 151);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    */
    ///////////////////////////////////////////////////////////////////////////////////////////

    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(500);

Label_wait_to_exec_next_copy_master:
    
    api_log_MsgDebug("��ͼ:������˹Ұ��:����:�ȴ�");

    Check_Terminate_Thread(luaman_task, dmobj);
    
    (void)Tz021_CleanPackage(dmobj, ctxt);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�ȴ������ͳ�ȥ
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "������˹Ұ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            api_log_MsgDebug("��ͼ:������˹Ұ��:����:�ɹ�");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(1000);
    }

    if (playWndIndexVal > 0)
    {
        int ij = 0;
        for (ij = 0; ij < playWndIndexVal; ij++)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
        }
    }

    lastPos = 0;
    //COPY_LABEL_AGAIN

Label_finish_this_copy_master:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("����:ִ��:���:����:��һ��:����");
    return ERRNO_SUCCESS;
}

int Tz021_FinishTask_slave(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    unsigned int playWndIndexVal = luaman_config->playWndIndexVal;
    int exitGameCopyFull = luaman_config->exitGameCopyFull;
    int code = 0;
    time_t cleanPackageTimeLast = time(NULL);
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int copyFullCounter = 0;
    int retryPreFly = 0;
    int lastPos = 0;
    int refAlive = 0;
    int REALIVE_REF_POS_2 = 1000;
    CString strCopylefttime;
    int count = 0;
    CStringArray copylefttime;
    long copylefttimem = 0;
    char* ptr = NULL;
    char* pathofposxys = NULL;
    CString pathOfPosXYs;
    struct fight021_param params;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int wait2sleep = 0;
    int nfailed = 0;
    
    if (1 == luaman_config->cleanPackageWhenStart)
    {
        cleanPackageTimeLast = -1;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    //@bug:��������:�����ոս���֮�������ű� @2019/4/15 21:28:44
    dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
    code = dmobj->FindStrFast(1140, 29, 1273, 58, "���������Ѵ�", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("����:��ͼ:���������Ѵ�:�����ű�,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
        goto LABEL_START_SCRIPT_IN_COPY_slave;
    }

    api_log_MsgDebug("����:��ͼ:������˹Ұ��:�����ű�,�ɹ�, playWndIndexVal:%u", playWndIndexVal);

    if (playWndIndexVal > 0)
    {
        int ij = 0;
        for (ij = 0; ij < playWndIndexVal; ij++)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
        }
    }

COPY_LABEL_AGAIN_slave:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    copyFullCounter = 0;
    retryPreFly = 0;

COPY_LABEL_TALK_WITH_NPC_slave:
    api_log_MsgDebug("����:npc1:�Ի�:��ʼ");

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //��n�Ի����븱��
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONT021_DIR"font-npc1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���������Ѵ��볡", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���������Ѵ��볡:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:�Ի�:���������Ѵ��볡:����:����:����");
            goto COPY_LABEL_TALK_WITH_NPC_slave;
        }
        
        retryPreFly = retryPreFly + 1;
        if (retryPreFly > 5)
        {
            api_log_MsgDebug("�ƶ�������:retryPreFly:%d", retryPreFly);
            
            code = Tz021_Fly2Copy(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc1:�Ի�:���������Ѵ��볡:����:�������Ի�:�ƶ�:�ɹ�");

                Check_Terminate_Thread(luaman_task, dmobj);
                goto COPY_LABEL_TALK_WITH_NPC_slave;
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("npc1:�Ի�:���������Ѵ��볡:����:��Χ,ʧ��");
        Delay(200);
    }

    //���'���������Ѵ��볡'
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        //����Ƿ����'ȷ��'�����ĶԻ���
        dmobj->SetDict(0, FONT021_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("npc1:�Ի�:ȷ��:����:��Χ,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:�Ի�:ȷ��:����:����:����");
            goto COPY_LABEL_TALK_WITH_NPC_slave;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("npc1:�Ի�:ȷ��:����:��Χ,ʧ��");
        Delay(200);
    }

    //���'ȷ��'�����Ի���
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:��ʡʱ�� 2020/2/25 21:17:16
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:�Ի�:ȷ��:���:����:����");
            goto COPY_LABEL_TALK_WITH_NPC_slave;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //����Ƿ����'�������³�����'�����ĶԻ���
        dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("npc1:�Ի�:�������³�����:����:��Χ,�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            
            //���"�������³�����"
            for (;;)
            {
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(500);
            }

            /////////////////////////////////////////////////////////////////////////////////////////////
            //@bug:�������³�����������5�����˳���Ϸ 2019/5/19 23:08:15
            if (copyFullCounter > 5)
            {
                if (1 == exitGameCopyFull)
                {
                    return Tz021_ExitGame(dmobj, ctxt);
                }
            }
            else
            {
                copyFullCounter++;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("npc1:�Ի�:�������³�����:���¶Ի�,copyFullCounter:%d", copyFullCounter);
            goto COPY_LABEL_TALK_WITH_NPC_slave;
        }//end:"�������³�����"

        //����Ƿ���븱��
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���������Ѵ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc1:�Ի�:���������Ѵ�:����,�ɹ�,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        //���Ѫ��
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:�Ի�:���������Ѵ�:����,����������");
            goto COPY_LABEL_AGAIN_slave;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(200);
        api_log_MsgDebug("npc1:�Ի�:���������Ѵ�,���ڽ���");
    }//end:���'ȷ��'�����Ի���

LABEL_START_SCRIPT_IN_COPY_slave:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (2 == lastPos)
    {
        dmobj->KeyDownChar("t");
        Delay(500);
        dmobj->KeyUpChar("t");
        Delay(500);

        api_log_MsgDebug("����:����:����:��ת:�ڶ���");//2037

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:�������� 2021/06/11 21:45:16 �ڵڶ������˽����ж�ʱ�䲻���ֱ�ӵȱ���������������
        //bug:�޸Ĳ��ҷ�Χ 2021/06/13 07:56:33
        ////////////////////////////////////////////////////////////////////////////
        //bug:��������������ȥ��,��Ҫ�Լ��߹�ȥ 2022/01/08 15:34:23
        //bug:ֱ������boss����,�ȴ���һ�� 2022/01/08 18:36:55
        if (REALIVE_REF_POS_2 == refAlive)
        {
            dmobj->SetDict(0, FONT021_DIR"font-copylefttime.txt");
            strCopylefttime = dmobj->Ocr(10, 200, 185, 230, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (strCopylefttime != CString(""))
            {
                count = SplitStringWide(strCopylefttime, "��", copylefttime);
                if (count > 2)
                {
                    copylefttimem = atol(copylefttime.GetAt(0));
                    if (copylefttimem < 26)
                    {
                        refAlive = 0;
                        
                        ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                        api_log_MsgDebug("����:����:ʱ��:����:�ȴ�:����:����3:%s", ptr);
                        strCopylefttime.ReleaseBuffer();

                        Check_Terminate_Thread(luaman_task, dmobj);

                        goto Label_wait_to_the_next_loop;
                        //goto Label_for_wait_to_polled_lytayrose_slave;
                    }
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////

        Check_Terminate_Thread(luaman_task, dmobj);
        
        pathofposxys = "199:152|174:152|155:137|142:136|133:135|133:143|132:151";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("����:����:����:��ת:ǰ��:����:[132:151],����������");
            
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            goto COPY_LABEL_AGAIN_slave;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("����:����:����:��ת:ǰ��:����:[132:151],�ɹ�");
        api_log_MsgDebug("����:����:����:��ת:ǰ��:����:[132:151]:�Ի�:׼��:���");

        goto Label_continue_for_step_2_slave;
    }

    //�ȴ����ϽǵĿ�ʼʱ��������ʱ��
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-readygo.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����ʱ��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����ʱ��:����:����:����");
            
            Delay(1000);
            goto COPY_LABEL_AGAIN_slave;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        Delay(500);
        api_log_MsgDebug("����ʱ��:����:ʧ��");
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    /******************************************************************************/
    //�ƶ���174:152���괦
    pathofposxys = "176:155|167:140";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[167:140],����������");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN_slave;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    api_log_MsgDebug("Ѱ·:ǰ��:����:[167:140],�ɹ�");
    
LABEL_BATTLE_WITH_TARGET_slave:
    params.fposXY = "167:140";
    params.ufontName = FONT021_DIR"font-audesay.txt";
    params.uName = "�µ���";
    params.uclrOffset = "ff3300-101010";
    params.uFailedCountMax = 120;
    
    code = Tz021_FightInFixPoint_atc(dmobj, ctxt, &params);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����:���:����:[167:140],����������");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN_slave;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("����:���:[167:140],�ɹ�");

    if (ERRNO_NEXTCOPY == code)
    {
        api_log_MsgDebug("167:140:���:��ʱ:ֱ������:���һ��:����");
        goto Label_wait_to_exec_next_copy_slave;
    }
    
    if (0 == lastPos)
    {
        lastPos = 1;
    }

    if (lastPos > 1)
    {
        /******************************************************************************/
        //�ƶ���132:151���괦
        pathofposxys = "167:140|155:137|142:136|133:135|133:143|132:151";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("Ѱ·:ǰ��:����:[132:151],����������");
            
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            goto COPY_LABEL_AGAIN_slave;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("Ѱ·:ǰ��:����:[132:151],�ɹ�");
    }
    
Label_continue_for_step_2_slave:
    if (lastPos > 1)
    {
        for (;;)
        {
            dmobj->KeyPressChar("n");
            LUAMAN_WAIT_MS(200);
            dmobj->KeyUpChar("n");
            LUAMAN_WAIT_MS(200);
            dmobj->MoveTo(5, 5);
            LUAMAN_WAIT_MS(100);

            dmobj->SetDict(0, FONT021_DIR"font-npc2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ������ڡ�", "00ff00-000000", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("npc2:�Ի�:�ƶ������ڡ�:����:��Χ,�ɹ�,lposX:%ld, lposY:%ld",
                    lposX, lposY);
                break;
            }

            code = Tz021_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc2:�Ի�:�ƶ������ڡ�:����:����:����");
                goto COPY_LABEL_TALK_WITH_NPC_slave;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            Delay(200);
            api_log_MsgDebug("npc2:�Ի�:�ƶ������ڡ�:����:��Χ,ʧ��");
        }

        //���'�ƶ������ڡ�'
        for (;;)
        {
            dmobj->MoveTo(lposX, lposY);
            LUAMAN_WAIT_MS(300);
            dmobj->LeftClick();
            LUAMAN_WAIT_MS(300);
            dmobj->MoveTo(5, 5);
            LUAMAN_WAIT_MS(300);

            dmobj->SetDict(0, FONT021_DIR"font-npc2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ������ڡ�", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("npc2:�Ի�:�ƶ������ڡ�:���,�ɹ�");
                break;
            }

            code = Tz021_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc2:�Ի�:�ƶ������ڡ�:���:����:����");
                goto COPY_LABEL_TALK_WITH_NPC_slave;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            Delay(500);
            api_log_MsgDebug("npc2:�Ի�:�ƶ������ڡ�:���,ʧ��");
        }
    }

    //��鵱ǰ����,������겻�� 119:157 ������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 119) <= 1 && abs(posY - 157) <= 1)
                {
                    api_log_MsgDebug("�ƶ������ڡ�:�Ի�:�ɹ�");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�ƶ������ڡ�:�ƶ�:������...");
        Delay(1000);
    }

    /////////////////////////////////////////////////////////////////////////
    //bug:���ڲ���Ҫ�������� 2022/01/08 15:36:56 ==>�ָ�,������boss 2022/01/08 18:38:15
    dmobj->SetDict(0, FONT021_DIR"font-nightrose.txt");
    code = dmobj->FindStrFast(2, 462, 385, 616, "���ƶ���������˹��ĹѨ��", "00ff00-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("����:������˹:�ɹ�:����:����2, lposX:%ld, lposY:%ld", lposX, lposY);
        //goto Label_for_wait_to_polled_lytayrose_slave;
        goto Label_fight_127_156_finish;
    }
    
    /////////////////////////////////////////////////////////////////////////
    
    Check_Terminate_Thread(luaman_task, dmobj);
    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    Delay(1000);

    
    //�ƶ���127:156���괦
    pathofposxys = "127:156";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[127:156],����������");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN_slave;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("Ѱ·:ǰ��:����:[127:156],�ɹ�");

    if (1 == lastPos)
    {
        lastPos = 2;
    }

    params.fposXY = "127:156";
    params.ufontName = "";
    params.uName = "";
    params.uclrOffset = "";
    params.uFailedCountMax = 150;
    
    code = Tz021_FightInFixPoint_atc(dmobj, ctxt, &params);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����:���:����:[127:156],����������");

        refAlive = REALIVE_REF_POS_2;
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN_slave;
    }

Label_fight_127_156_finish:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    refAlive = 0;
    
    api_log_MsgDebug("����:���:[127:156],�ɹ�");

    if (2 == lastPos)
    {
        lastPos = 3;
    }

    //��F5
    dmobj->KeyDown(116);
    Delay(500);
    dmobj->KeyUp(116);
    Delay(1000);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //bug:�����ű���һ�� 2022/01/05 22:28:29
    //if (lastPos > 3) 
    {
        //�ƶ���121:125���괦 bug:ͣ2���ټ����� 2022/01/26 22:45:16
        //pathofposxys = "127:155|124:145|124:135|117:133|118:129|121:125";
        pathofposxys = "127:155|124:145|124:135|117:133";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("Ѱ·:ǰ��:����:[117:133],����������");
            
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);

            if (SNAIL_ERRNO_BLOCKED == code)
            {
                api_log_MsgDebug("��ס:ֱ������:���һ��:��ʼ:��һ����");
                goto Label_wait_to_the_next_loop;
            }
            
            goto COPY_LABEL_AGAIN_slave;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[117:133],�ɹ�");

        Delay_With_Terminate_Check(2, luaman_task, dmobj);
        
        pathofposxys = "118:129|121:125";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("Ѱ·:ǰ��:����:[121:125],����������");
            
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);

            if (SNAIL_ERRNO_BLOCKED == code)
            {
                api_log_MsgDebug("��ס:ֱ������:���һ��:��ʼ:��һ����");
                goto Label_wait_to_the_next_loop;
            }
            
            goto COPY_LABEL_AGAIN_slave;
        }
        
        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("Ѱ·:ǰ��:����:[121:125],�ɹ�");
        api_log_MsgDebug("����:[121:125]:�ȴ�:��ͣ����");

        //�ȴ�һ��
        wait2sleep = 0;

        for (;;)
        {
            if (wait2sleep > luaman_config->Wait2Sleep)
            {
                api_log_MsgDebug("����:[121:125]:�ȴ�:ʱ�䵽:�ָ�����");
                break;
            }

            api_log_MsgDebug("����:[121:125]:�ȴ�:ʱ�䵽:%d:%d", wait2sleep, luaman_config->Wait2Sleep);
            
            wait2sleep++;
            Delay(1000);
            
            Check_Terminate_Thread(luaman_task, dmobj);
        }

        //u����ʯ��~����
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(500);

        nfailed = 0;
        
        //bug:ԭ���ǰ�u�ٰ�`,�޸�Ϊ��n�����Ի� 2022/01/06 22:17:59
        for (;;)
        {
            dmobj->KeyDownChar("n");
            Delay(500);
            dmobj->KeyUpChar("n");
            Delay(500);

            dmobj->SetDict(0, FONT021_DIR"font-npc100.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ���������˹��Ϣ��", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("�ƶ���������˹��Ϣ��:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);

            nfailed++;
            
            api_log_MsgDebug("�ƶ���������˹��Ϣ��:����:ʧ��, nfailed:%d", nfailed);
            Delay(500);

            //bug:�������������� 2022/01/26 22:49:11
            if (nfailed > 30)
            {
                api_log_MsgDebug("�ƶ���������˹��Ϣ��:����:ʧ��:�ȴ������ͳ�ȥ");
                goto Label_wait_to_the_next_loop;
            }
        }

        for (;;)
        {
            dmobj->MoveTo(lposX, lposY);
            LUAMAN_WAIT_MS(300);
            dmobj->LeftClick();
            LUAMAN_WAIT_MS(300);
            dmobj->MoveTo(5, 5);
            LUAMAN_WAIT_MS(300);

            dmobj->SetDict(0, FONT021_DIR"font-npc100.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ���������˹��Ϣ��", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�ƶ���������˹��Ϣ��:���:�ɹ�");
                break;
            }

            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("�ƶ���������˹��Ϣ��:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            Delay(500);
        }
    }

Label_for_wait_to_polled_lytayrose_slave:
    //������260:235
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 260) <= 1 && abs(posY - 235) <= 1)
                {
                    api_log_MsgDebug("������260:235:�ɹ�:�ɹ�");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("������260:235:������...");
        Delay(1000);
    }

    if (3 == lastPos)
    {
        lastPos = 4;
    }

    //bug:������֮��F1 2021/06/14 11:17:15
    dmobj->KeyDownChar("f1");
    Delay(200);
    dmobj->KeyUpChar("f1");
    Delay(200);
    dmobj->MoveTo( 5, 5);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�ƶ���254:254���괦
    pathofposxys = "260:235|255:249|254:254";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[254:254],����������");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);

        if (SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("��ס:ֱ������:���һ��:��ʼ:��һ����");
            goto Label_wait_to_the_next_loop;
        }
        goto COPY_LABEL_AGAIN_slave;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("Ѱ·:ǰ��:����:[254:254],�ɹ�");

    params.fposXY = "254:254";
    params.ufontName = FONT021_DIR"font-unightrose.txt";
    params.uName = "������˹";
    params.uclrOffset = "ff3300-101010";
    params.uFailedCountMax = 60;
    
    code = Tz021_FightInFixPoint_atc(dmobj, ctxt, &params);
    if (ERRNO_REALIVE_X1 == code)
    {
        api_log_MsgDebug("����:���:����:[254:254],����:����:�ڸ�����");
        goto Label_finish_this_copy_slave;
    }
    
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����:���:����:[254:254],����������");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);

        if (SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("��ס:ֱ������:���һ��:��ʼ:��һ����");
            goto Label_wait_to_the_next_loop;
        }
        
        goto COPY_LABEL_AGAIN_slave;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("����:���:[254:254],�ɹ�");

    if (4 == lastPos)
    {
        lastPos = 5;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    //bug:ע�͵� 2022/01/06 22:01:16
    /*
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(500);

    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-success_end.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���Ѵ���ת��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����:��ʼ:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����:��ʼ:ʧ��");
        Delay(500);
    }

    dmobj->MoveTo(lposX + 27, lposY + 151);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    */
    /////////////////////////////////////////////////////////////////////////////////////////
    
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(500);

Label_wait_to_exec_next_copy_slave:
Label_wait_to_the_next_loop:
    api_log_MsgDebug("��ͼ:������˹Ұ��:����:�ȴ�");

    Check_Terminate_Thread(luaman_task, dmobj);
    
    (void)Tz021_CleanPackage(dmobj, ctxt);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�ȴ������ͳ�ȥ
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "������˹Ұ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            api_log_MsgDebug("��ͼ:������˹Ұ��:����:�ɹ�");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        Delay(1000);
    }

    if (playWndIndexVal > 0)
    {
        int ij = 0;
        for (ij = 0; ij < 2 * playWndIndexVal; ij ++)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
        }
    }

    lastPos = 0;

Label_finish_this_copy_slave:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("����:ִ��:���:����:��һ��:����");
    return ERRNO_SUCCESS;
}

int api_Task021Init(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int code = 0;
    
    api_log_MsgDebug("����:021:��ʼ��:��ʼ, hwnd:%ld", hwnd);

    code = snail_AllocClientContext(hwnd);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        if (SNAIL_ERRNO_EXIST == code)
        {
            api_log_MsgError("task021 is aready running, hwnd:%ld", hwnd);
            AfxMessageBox("������021����");
            return code;
        }

        if (SNAIL_ERRNO_NOSPACE == code)
        {
            api_log_MsgError("task021 team member reach max, hwnd:%ld", hwnd);
            AfxMessageBox("021����������Ա");
            return code;
        }

        api_log_MsgError("task021 invalid argument, hwnd:%ld", hwnd);
        AfxMessageBox("021������������");
        
        return code;
    }

    code = snail_UpdateClientTaskId(hwnd, luaman_task->taskid);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        snail_FreeClientContext(hwnd);

        api_log_MsgError("task021 snail_UpdateClientTaskId failed, hwnd:%ld, code:%d",
            hwnd, code);
        return code;
    }

    /* �󶨴��� */
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("bind 021 hwnd failed, hwnd:%ld", hwnd);

        snail_FreeClientContext(hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("bind 021 hwnd success, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("lock 021 hwnd failed, hwnd:%ld", hwnd);

        snail_FreeClientContext(hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("����:021:��ʼ��:���, hwnd:%ld", hwnd);

    /* ��ʼ���澯ʱ���ʼֵ */
    init_alarm_ctxt();
    
    return ERRNO_SUCCESS;
}

int api_Task021Main(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int code = 0;

    api_log_MsgDebug("����:021:��ѭ��: hwnd:%ld, role:%d", hwnd, (int)role);

    switch (role)
    {
    case ROLE_M:
        api_log_MsgDebug("����:021:����:��ѭ��:��ʼ, hwnd:%ld", hwnd);
        (void)Tz021_FinishTask(dmobj, ctxt);
        api_log_MsgDebug("����:021:����:��ѭ��:���, hwnd:%ld", hwnd);
        break;

    case ROLE_S0:
        api_log_MsgDebug("����:021:����:��ѭ��:��ʼ, hwnd:%ld", hwnd);
        (void)Tz021_FinishTask_slave(dmobj, ctxt);
        api_log_MsgDebug("����:021:����:��ѭ��:���, hwnd:%ld", hwnd);
        break;
    
    case ROLE_S1:
    case ROLE_AUX:
    default:
        {
            AfxMessageBox("��ѡ����ȷ����Ϸ��ɫ(��֧������|����),��ǰ����������ֹ");
            AfxMessageBox("5���Ӻ�������������");
            return ERRNO_INVALID;
        }
        break;
    }
    
    return ERRNO_SUCCESS;
}

int api_Task021Exit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    
    snail_FreeClientContext(hwnd);

    api_log_MsgDebug("����:021:�˳�, hwnd:%ld, groupid:%d, role:%s",
        hwnd, groupid, roletips[role]);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
    
    return ERRNO_SUCCESS;
}

