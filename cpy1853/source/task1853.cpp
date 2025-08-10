#include <math.h>
#include <stdlib.h>
#include <process.h>
#include "stdafx.h"
//#include "xrrno.h"
#include "xlog.h"
#include "xcommon.h"
#include "xlist.h"
#include "task.h"
#include "config.h"
#include "alarm.h"
//#include "task1853_errno.h"
#include "task.h"
#include "task1853.h"
#include "task1853_common.h"
#include "task1853_client_context.h"
#include "task1853_client_manager.h"

#define FONT1853_DIR "font\\1853\\"
#define BMP1853_DIR "bmp\\1853\\"


//static CRITICAL_SECTION g_Tz1853_criticalSection;
static HANDLE g_Tz1853_criticalSection;

#include <time.h>
static time_t g_lstChkTime;

void Tz1853_InitCriticalSection()
{
    //::InitializeCriticalSection(&g_Tz1853_criticalSection);
    g_Tz1853_criticalSection = ::CreateMutex(NULL, FALSE, NULL);
    if (NULL == g_Tz1853_criticalSection)
    {
        api_log_MsgError("create Tz1853_InitCriticalSection lock failed");
        exit(1);
    }
    return;
}

static void Tz1853EnterCriticalSection()
{
    //EnterCriticalSection(&g_Tz1853_criticalSection);
    ::WaitForSingleObject(g_Tz1853_criticalSection, INFINITE);
    return;
}

static void Tz1853LeaveCriticalSection()
{
    //::LeaveCriticalSection(&g_Tz1853_criticalSection);
    (void)::ReleaseMutex(g_Tz1853_criticalSection);
    return;
}

static void Tz1853DeleteCriticalSection()
{
    //::DeleteCriticalSection(&g_Tz1853_criticalSection);
    (void)::CloseHandle(g_Tz1853_criticalSection);
    return;
}

void Tz1853_ReleaseKey(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    
    if (1 == dmobj->GetKeyState(65))
    {
        dmobj->KeyUp(65);
        LUAMAN_WAIT_MS(50);
    }
    if (1 == dmobj->GetKeyState(68))
    {
        dmobj->KeyUp(68);
        LUAMAN_WAIT_MS(50);
    }
    if (1 == dmobj->GetKeyState(83))
    {
        dmobj->KeyUp(83);
        LUAMAN_WAIT_MS(50);
    }
    if (1 == dmobj->GetKeyState(87))
    {
        dmobj->KeyUp(87);
        LUAMAN_WAIT_MS(50);
    }

    dmobj->KeyDownChar("a");
    LUAMAN_WAIT_MS(50);
    dmobj->KeyUpChar("a");
    LUAMAN_WAIT_MS(50);

    dmobj->KeyDownChar("s");
    LUAMAN_WAIT_MS(50);
    dmobj->KeyUpChar("s");
    LUAMAN_WAIT_MS(50);

    dmobj->KeyDownChar("w");
    LUAMAN_WAIT_MS(50);
    dmobj->KeyUpChar("w");
    LUAMAN_WAIT_MS(50);

    dmobj->KeyDownChar("d");
    LUAMAN_WAIT_MS(50);
    dmobj->KeyUpChar("d");
    LUAMAN_WAIT_MS(50);

    Check_Terminate_Thread(luaman_task, dmobj);
    return;
}

int Tz1853_Fly2Copy(Idmsoft* dmobj, void* ctxt)
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

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        dmobj->KeyDownChar("m");
        Delay(200);
        dmobj->KeyUpChar("m");
        Delay(200);

        //�鿴m���Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT1853_DIR"font-fly-mcheck1.txt");
        //code = dmobj->FindStrFast(210, 45, 1138, 745, "�춴(����)", "ffffff-101010", 0.9, &pposX, &pposY);
        code = dmobj->FindStrFast(210, 45, 1138, 745, "���׵Ļ�¯", "cefbc9-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���׵Ļ�¯:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz1853_CheckLife(dmobj, ctxt);
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
        dmobj->SetDict(0, FONT1853_DIR"font-fly-mcheck2.txt");
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
        
        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("��������:����:�ƶ�:����:��ɱ��:����,�ɹ�");
            return code;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:���ƶ����̱�����ɱ�����ָ���ʱ���ܷ���SUCCESS 2021/06/14 11:12:33
        /*if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("��������:����:�ƶ�:����:����:�ɹ�:�ƶ�:����:�ɹ�");
            break;
        }*/

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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT1853_DIR"font-posxy.txt");
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
                //120:126 -> 442:388 2022/05/04 19:52:58
                if (abs(posX - 442) <= 3 && abs(posY - 388) <= 3)
                {
                    api_log_MsgDebug("��������:����:�ƶ��������ſ�:�ɹ�, posX:%ld, posY:%ld", posX, posY);
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz1853_CheckLife(dmobj, ctxt);
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

int Tz1853_CheckAlive(Idmsoft* dmobj, void* ctxt, int bCopyoutPre)
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

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->SetDict(0, FONT1853_DIR"font-blood.txt");
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
        (void)Tz1853_ReleaseKey(dmobj, ctxt);

        if (luaman_config->callbabydie)
        {
            //bug:��t�ٻ����� 2022/06/12 22:31:15
            dmobj->KeyDown(84);//t
            Delay(500);
            dmobj->KeyUp(84);
            Delay(1000);
        }
        
        if (1 == bCopyoutPre)
        {
            code = Tz1853_Fly2Copy(dmobj, ctxt);
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

int Tz1853_CheckLife_Outside(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int blueThreshold = luaman_config->blueThreshold;
    int maxBlueFiled = luaman_config->maxBlueFiled;
    int minBlue = luaman_config->minBlue;
    int bluebloodChecktime = luaman_config->bluebloodChecktime;
    int bluebloodLowLevel = luaman_config->bluebloodLowLevel;
    int bluebloodWidth = luaman_config->bluebloodWidth;
    int code = 0;
    time_t end;
    int offset = 0;
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
    time_t hiddenchk;
    int i = 0;
    
    (void)check_alarm(dmobj, ctxt);
    
    Check_Terminate_Thread(luaman_task, dmobj);

    /* ��CheckLife��ʱ���Ѿ��ж�,�˴���������ʱ����ڵ��´�����̱�����
    end = time(NULL);
    offset = end - (time_t)TlsGetValue(g_lstChkTime);
    if (offset < bluebloodChecktime)
    {
        return SNAIL_ERRNO_INGORE;
    }

    //����Ѫ�����ʱ��
    TlsSetValue(g_lstChkTime, (LPVOID)end);
    */

    ///////////////////////////////////////////////////////////////////////////////////
    //@bug:�����⸴��,���жϵ������֮ǰ�ǲ����ڸ������� 2019/4/24 21:14:44
    //Ĭ���ڸ�����������,����������Ҳ�޷��Ƕ�����ƶ�һ��,����Ӱ��ű��Ĺ���
    bCopyoutPre = 1;

    ///////////////////////////////////////////////////////////////////////////////////
    dmobj->SetDict(0, FONT1853_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("������:��ȡ:Ѫ��:�쳣");
        return SNAIL_ERRNO_INGORE;
    }

    cstr = strBBlood.GetBuffer(BUFFER_MAX);

    api_log_MsgDebug("������:��ȡ:Ѫ��:�ɹ�,cstr:%s", (cstr)? cstr : "null");
    
    strBBlood.ReleaseBuffer();

    sz = SplitString(strBBlood, '/', bBlood);
    if (2 == sz)
    {
        bBloodAva = atol(bBlood.GetAt(0));
        bBloodTot = atol(bBlood.GetAt(1));

        api_log_MsgDebug("������:Ѫ��:����:�ɹ�,bBloodAva:%ld, bBloodTot:%ld",
            bBloodAva, bBloodTot);
    }
    else
    {
        strBBloodLen = strBBlood.GetLength();
        bBloodAva = atol(strBBlood.Mid(1, strBBloodLen - bluebloodWidth));
        bBloodTot = atol(strBBlood.Mid(strBBloodLen - bluebloodWidth + 1, strBBloodLen));

        api_log_MsgDebug("������:Ѫ��:����:�ɹ�,"
            "bBloodAva:%ld, bBloodTot:%ld, sz:%d, strBBloodLen:%d,"
            "bluebloodWidth:%d",
            bBloodAva, bBloodTot, sz, strBBloodLen,
            bluebloodWidth);
    }

    if (bBloodAva > bluebloodLowLevel)
    {
        api_log_MsgDebug("������:Ѫ��:����,bBloodAva:%d, bluebloodLowLevel:%d", bBloodAva, bluebloodLowLevel);
        return ERRNO_SUCCESS;
    }

    Check_Terminate_Thread(luaman_task, dmobj);

    api_log_MsgDebug("������:Ѫ��:����,bBloodAva:%d, bluebloodLowLevel:%d", bBloodAva, bluebloodLowLevel);
    
    if (bBloodAva > 0)
    {
        dmobj->KeyDownChar("f3");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f3");
        LUAMAN_WAIT_MS(200);

        api_log_MsgDebug("Ѫ��:����:���,bBloodAva:%d", bBloodAva);
        return ERRNO_SUCCESS;
    }

    api_log_MsgDebug("Ӣ��:����:�ص�:������:��������");
    
    //���������ȼ�鷽����Ƿ���,���ɿ���Ӧ���µķ����
    Tz1853_ReleaseKey(dmobj, ctxt);
    
    api_log_MsgDebug("Ѫ��:���:����,�ȴ����ֽ�������");

LABEL_continue_after_realive_yx0_Outside:
    //�ȴ�������������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(200);

        //����еض����������,���ȵ�� 2019/7/19 20:47:59
        dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                Delay(500);
            }
        }
        
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:��ʱ����ְ�ɫ��'�������³�����'�㲻��,���º������̳��� 2019/8/26 20:46:37
        dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "ffffff-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                Delay(500);
            }
        }

        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(200);
        
        dmobj->SetDict(0, FONT1853_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:����:����:��Χ,�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }
        
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug: �����������˼��� 2020/2/13 17:02:16
        dmobj->SetDict(0, FONT1853_DIR"font-chcklife2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ʹ�ø������", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("����:����:ʹ�ø������:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            goto LABEL_continue_after_realive_x1_Outside;
        }

        dmobj->SetDict(0, FONT1853_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "Ҫ��ԭ�ظ�����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("����:����:Ҫ��ԭ�ظ�����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            goto LABEL_continue_after_realive_x2_Outside;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //���Ѫ���Ƿ�ָ�,����ָ��������������Ҽ��ܸ���,ֱ���˳���������
        code = Tz1853_CheckAlive(dmobj, ctxt, bCopyoutPre);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����:����:����������,������Դ��������ҵ�ʩ��");
            
            code = SNAIL_ERRNO_REALIVE;
            goto LABEL_CHECK_HIDDEN_OK_Outside;
        }

        api_log_MsgDebug("Ѫ��:���:����:����:����:��Χ,ʧ��");
        Delay(500);
    }

    Delay(500);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT1853_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (0 != code)
        {
            api_log_MsgDebug("Ѫ��:���:����:����:���,�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("Ѫ��:���:����:����:���,ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    Delay(500);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�ȴ�Ӣ�۸����վ����
    for (;;)
    {
        code = Tz1853_CheckAlive(dmobj, ctxt, bCopyoutPre);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѫ��:���:����:��������,�ɹ�");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("Ѫ��:���:����:��������:�ȴ�");
        Delay(500);
    }
    
LABEL_CHECK_HIDDEN_OK_Outside:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    hiddenchk = time(NULL);
    for (;;)
    {
        dmobj->KeyDownChar("9");
        Delay(200);
        dmobj->KeyUpChar("9");
        Delay(200);

        //ȷ���Ƿ�����ɹ�
        code = dmobj->CmpColor(376, 771, "4b4b4b-101010", 0.9);
        if (!code)
        {
            api_log_MsgDebug("����:����:����:�ɹ�");
            break;
        }

        end = time(NULL);
        offset = end - hiddenchk;
        if (offset >= 10)
        {
            api_log_MsgDebug("����:����:����:��ʱ,�����ֱ�ɱ��");
            break;
        }

        api_log_MsgDebug("����:����:����:ʧ��");
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    return SNAIL_ERRNO_REALIVE;
    
LABEL_continue_after_realive_x1_Outside:
    for (i = 0; i < 15; i++)
    {
        Delay_With_Terminate_Check(1, luaman_task, dmobj);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2 + 88, lposY2 + 72);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT1853_DIR"font-chcklife2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ʹ�ø������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if(code == -1)
        {
            api_log_MsgDebug("����:����:ʹ�ø������:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("����:����:ʹ�ø������:���:ʧ��");
        Delay(500);
    }

    goto LABEL_continue_after_realive_Outside;

LABEL_continue_after_realive_x2_Outside:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    //���Ҳ����"ȡ��",�Ҳ����ͼ�������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(200);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug: ����еض����������,���ȷ��;������ǵ�ȷ�϶�Ӧ��ȡ�� 2020/12/27 14:02:18
        //����еض����������,���ȵ�� 2019/7/19 20:47:59
        dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("ԭ�ظ���:�������³�����:���:�ɹ�");
                    break;
                }

                api_log_MsgDebug("ԭ�ظ���:�������³�����:���:ʧ��");
                
                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(500);
            }

            break;
        }
        
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:��ʱ����ְ�ɫ��'�������³�����'�㲻��,���º������̳��� 2019/8/26 20:46:37
        dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "ffffff-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("ԭ�ظ���:�������³�����:���:�ɹ�");
                    break;
                }

                api_log_MsgDebug("ԭ�ظ���:�������³�����:���:ʧ��");
                
                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(500);
            }

            break;
        }
        
        //����Ƿ����'ȷ��'�����ĶԻ���
        dmobj->SetDict(0, FONT1853_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ԭ�ظ���:ȷ��:���:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //���'ȡ��' ȷ��:578:498,ȡ��:678:498
        dmobj->MoveTo(lposX2 + 100, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("ԭ�ظ���:ȷ��:����:�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
    }

    api_log_MsgDebug("ԭ�ظ���:ȡ��:�ɹ�:����:����");
    
    goto LABEL_continue_after_realive_yx0_Outside;
    
LABEL_continue_after_realive_Outside:
    //�ȴ�Ӣ�۸����վ����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz1853_CheckAlive(dmobj, ctxt, bCopyoutPre);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѫ��:���:����:��������,�ɹ�");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("Ѫ��:���:����:��������:�ȴ�");
        Delay(500);
    }

    return code;
}

int Tz1853_CheckLife(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int blueThreshold = luaman_config->blueThreshold;
    int maxBlueFiled = luaman_config->maxBlueFiled;
    int minBlue = luaman_config->minBlue;
    int bluebloodChecktime = luaman_config->bluebloodChecktime;
    int bluebloodLowLevel = luaman_config->bluebloodLowLevel;
    int bluebloodWidth = luaman_config->bluebloodWidth;
    int code = 0;
    time_t end;
    int offset = 0;
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
    time_t hiddenchk;
    
    (void)check_alarm(dmobj, ctxt);
    
    Check_Terminate_Thread(luaman_task, dmobj);

    end = time(NULL);
    offset = end - (time_t)TlsGetValue(g_lstChkTime);
    if (offset < bluebloodChecktime)
    {
        api_log_MsgDebug("Ѫ��:���:ʱ��:̫��:����,end:%ld, offset:%d, bluebloodChecktime:%d",
            end, offset, bluebloodChecktime);
        return SNAIL_ERRNO_INGORE;
    }

    api_log_MsgDebug("Ѫ��:���:ʱ��:����,end:%ld, offset:%d", end, offset);
    
    //����Ѫ�����ʱ��
    TlsSetValue(g_lstChkTime, (LPVOID)end);

    ///////////////////////////////////////////////////////////////////////////////////
    //@bug:�����⸴��,���жϵ������֮ǰ�ǲ����ڸ������� 2019/4/24 21:14:44
    //Ĭ���ڸ�����������,����������Ҳ�޷��Ƕ�����ƶ�һ��,����Ӱ��ű��Ĺ���
    bCopyoutPre = 1;

    dmobj->SetDict(0, FONT1853_DIR"font-maptips.txt");
    code = dmobj->FindStrFast(1140, 29, 1273, 58, "������", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 == code)
    {
        api_log_MsgDebug("Ѫ��:���:��ͼ:������:����:ʧ��");
        
        dmobj->SetDict(0, FONT1853_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "�����ĵ������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            bCopyoutPre = 0;

            api_log_MsgDebug("Ѫ��:���:��ͼ:�����ĵ������:����:�ɹ�");
        }
    }
    else
    {
        api_log_MsgDebug("Ѫ��:���:��ͼ:������:����:�ɹ�");
    }

    ///////////////////////////////////////////////////////////////////////////////////
    dmobj->SetDict(0, FONT1853_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("Ѫ��:��ȡ:ʧ��, groupid:%d, hwnd:%ld",
            luaman_config->groupid, hwnd);
        return SNAIL_ERRNO_INGORE;
    }

    cstr = strBBlood.GetBuffer(BUFFER_MAX);

    api_log_MsgDebug("Ѫ��:��ȡ:�ɹ�,cstr:%s", (cstr)? cstr : "null");
    
    strBBlood.ReleaseBuffer();

    sz = SplitString(strBBlood, '/', bBlood);
    if (2 == sz)
    {
        bBloodAva = atol(bBlood.GetAt(0));
        bBloodTot = atol(bBlood.GetAt(1));

        api_log_MsgDebug("Ѫ��:����:�ɹ�,bBloodAva:%ld, bBloodTot:%ld",
            bBloodAva, bBloodTot);
    }
    else
    {
        strBBloodLen = strBBlood.GetLength();
        bBloodAva = atol(strBBlood.Mid(1, strBBloodLen - bluebloodWidth));
        bBloodTot = atol(strBBlood.Mid(strBBloodLen - bluebloodWidth + 1, strBBloodLen));

        api_log_MsgDebug("Ѫ��:����:�ɹ�,"
            "bBloodAva:%ld, bBloodTot:%ld, sz:%d, strBBloodLen:%d,"
            "bluebloodWidth:%d",
            bBloodAva, bBloodTot, sz, strBBloodLen,
            bluebloodWidth);
    }

    if (bBloodAva > bluebloodLowLevel)
    {
        api_log_MsgDebug("Ѫ��:����,bBloodAva:%d, bluebloodLowLevel:%d", bBloodAva, bluebloodLowLevel);
        return ERRNO_SUCCESS;
    }

    api_log_MsgDebug("Ѫ��:����,bBloodAva:%d, bluebloodLowLevel:%d", bBloodAva, bluebloodLowLevel);
    
    if (bBloodAva > 0)
    {
        dmobj->KeyDownChar("f3");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f3");
        LUAMAN_WAIT_MS(200);

        api_log_MsgDebug("Ѫ��:����:���,bBloodAva:%d", bBloodAva);
        return ERRNO_SUCCESS;
    }

    api_log_MsgDebug("Ѫ��:����:����,bBloodAva:%d, bCopyoutPre:%d", bBloodAva, bCopyoutPre);
    
    ///////////////////////////////////////////////////////////////////////////////////
    //bug:���������Ѫ�������Ҫ�ı临����� 2021/03/13 20:20:35 ��Tz1852-v1.0.17.1����������
    if (1 == bCopyoutPre)
    {
        return Tz1853_CheckLife_Outside(dmobj, ctxt);
    }

    api_log_MsgDebug("Ӣ��:����:�ص�:�����ĵ������:��������");

    Tz1853_ReleaseKey(dmobj, ctxt);

    api_log_MsgDebug("Ѫ��:���:����,�ȴ����ֽ�������");

    //�ȴ�������������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(200);

        //����еض����������,���ȵ�� 2019/7/19 20:47:59
        dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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

        //////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:��ʱ����ְ�ɫ��'�������³�����'�㲻��,���º������̳��� 2019/8/26 20:46:37
        dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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
        LUAMAN_WAIT_MS(200);

        dmobj->SetDict(0, FONT1853_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:����:����:��Χ,�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz1853_CheckAlive(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����:����:����������,������Դ��������ҵ�ʩ��");
            code = SNAIL_ERRNO_REALIVE;

            //bug:��������֮������ɹ��˲��˳� 2019/7/20 20:35:19
            goto LABEL_CHECK_HIDDEN_OK;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("Ѫ��:���:����:����:����:��Χ,ʧ��");
        Delay(500);
    }

    LUAMAN_WAIT_MS(500);
    

    //�������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT1853_DIR"font-chcklife.txt");
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

    LUAMAN_WAIT_MS(500);
    
    //�ȴ�Ӣ�۸����վ����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz1853_CheckAlive(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѫ��:���:����:��������,�ɹ�");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("Ѫ��:���:����:��������:�ȴ�");
        Delay(500);
    }
    
LABEL_CHECK_HIDDEN_OK:
    //bug:��������֮������ɹ��˲��˳�, ����֮���ж��ݱ���
    //    ����������δ�ɹ������������ɱ����,���������
    //    ����ruby���������Ȧ 2019/7/20 20:35:19 
    hiddenchk = time(NULL);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("9");
        Delay(200);
        dmobj->KeyUpChar("9");
        Delay(200);

        //ȷ���Ƿ�����ɹ�
        code = dmobj->CmpColor(376, 771, "4b4b4b-101010", 0.9);
        if (!code)
        {
            api_log_MsgDebug("����:����:����:�ɹ�");
            break;
        }

        end = time(NULL);
        offset = end - hiddenchk;
        if (offset >= 10.0)
        {
            api_log_MsgDebug("����:����:����:��ʱ,�����ֱ�ɱ��");
            break;
        }

        api_log_MsgDebug("����:����:����:ʧ��");
        Delay(500);
    }
    
    return SNAIL_ERRNO_REALIVE;
}

int Tz1853_InviteHeilfellowJoin2Team(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int syncCountMax = luaman_config->syncCountMax;
    int code = 0;
    struct xyposition syncPosXYs[4] =
    {
        {1234, 266},
        {1272, 286},
        {1234, 298},
        {1272, 318},
    };
    struct xyposition fellowPosXYs[2];
    
    time_t lastChckTime = time(NULL);
    time_t curTime = time(NULL);
    time_t lastChckTimeAll;
    double deltaTime = 0.0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int i = 0, j = 0;
    long posX = 0, posY = 0;
    long posX1 = 0, posY1 = 0;
    long posX2 = 0, posY2 = 0;
    int syncCount = 0;
    
    Check_Terminate_Thread(luaman_task, dmobj);

    lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("f");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f");
        LUAMAN_WAIT_MS(200);

        dmobj->SetDict(0, FONT1853_DIR"font-inviteheil2team-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ӻ���", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���:����:��Ӻ���:����:��Χ:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 20.0)
        {
            api_log_MsgDebug("���:����:��Ӻ���:����:��Χ,ʧ��:��ʱ");
            return SNAIL_ERRNO_FAILED;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //���Ѫ��
        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("���:����:��Ӻ���:����:����");
            return code;
        }

        api_log_MsgDebug("���:����:��Ӻ���:����:��Χ,ʧ��");
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(200);
    
    //����'ս������'����
    lastChckTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT1853_DIR"font-inviteheil2team-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ս������", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���:����:ս������:����,�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 20.0)
        {
            api_log_MsgDebug("���:����:ս������:����,ʧ��,��ʱ,�˳�");
            return SNAIL_ERRNO_OFFLINE;
        }

        //���Ѫ��
        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("���:����:ս������:����:����:����");
            return code;
        }

        api_log_MsgDebug("���:����:ս������:����,ʧ��");
        Delay(500);
    }

    fellowPosXYs[0].x = lposX2;
    fellowPosXYs[0].y = lposY2;
    fellowPosXYs[1].x = lposX2 - 120;
    fellowPosXYs[1].y = lposY2 + 20;

    lastChckTimeAll = time(NULL);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            posX = fellowPosXYs[i].x;
            posY = fellowPosXYs[i].y;
            
            api_log_MsgDebug("i:%d, posX:%ld, posY:%ld", i, posX, posY);

            lastChckTime = time(NULL);

            //�����Ա����,��������Ի���
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(posX, posY);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);

                dmobj->SetDict(0, FONT1853_DIR"font-inviteheil2team-1.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "7d7d7d-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("���:����:��Ա:����:����,�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                curTime = time(NULL);
                deltaTime = difftime(curTime, lastChckTime);
                if (deltaTime > 20.0)
                {
                    api_log_MsgDebug("���:����:��Ա:����:����,ʧ��,��ʱ,�˳�");
                    return SNAIL_ERRNO_OFFLINE;
                }

                //���Ѫ��
                code = Tz1853_CheckLife(dmobj, ctxt);
                if (SNAIL_ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("���:����:��Ա:����:����:����:����");
                    return code;
                }

                api_log_MsgDebug("���:����:��Ա:����:����,ʧ��");
                Delay(500);
            }

            //���'����'������Ѽ������
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
        }

        syncCount = 0;

        //@bug:�Ե�Ƭ��,�����������˷�ʱ��2019/4/8 23:06:19
        Delay(2000);

        api_log_MsgDebug("ͬ��:����:���");
        
        //���'ͬ��'����
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            posX1 = syncPosXYs[2 * i].x;
            posY1 = syncPosXYs[2 * i].y;
            posX2 = syncPosXYs[2 * i + 1].x;
            posY2 = syncPosXYs[2 * i + 1].y;

            api_log_MsgDebug("(%ld, %ld) --> (%ld, %ld)", posX1, posY1, posX2, posY2);

            dmobj->SetDict(0, FONT1853_DIR"font-inviteheil2team-1.txt");
            code = dmobj->FindStrFast(posX1, posY1, posX2, posY2, "ͬ��", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                syncCount++;

                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;

                api_log_MsgDebug("ͬ��:���:�ɹ�:%ld:%ld", lposX2, lposY2);
            }
        }

        if (syncCount == syncCountMax)
        {
            api_log_MsgDebug("���:����:��Ա:����,������");
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTimeAll);
        if (deltaTime > 30.0)
        {
            api_log_MsgDebug("���:����:��Ա:����:����,ʧ��,��ʱ,�˳�");
            
            if (0 == syncCount)
            {
                return SNAIL_ERRNO_FAILED;
            }

            return SNAIL_ERRNO_AGAIN;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("���:����:��Ա:����,���δ���,syncCount:%d", syncCount);
    }

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //���Ѫ��
    code = Tz1853_CheckLife(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code)
    {
        api_log_MsgDebug("������:����:����:���");
        return code;
    }

    api_log_MsgDebug("������:�ɹ�:���");
    return SNAIL_ERRNO_SUCCESS;
}

int Tz1853_Walk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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
    int cindex = 0;
    int cjndex = 0;
    
    walkstate.xNeg = "w";
    walkstate.xPos = "s";
    walkstate.yNeg = "a";
    walkstate.yPos = "d";

    Check_Terminate_Thread(luaman_task, dmobj);
    
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
        Check_Terminate_Thread(luaman_task, dmobj);
        
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
            if (walkTimeDelta > 120.0)
            {
                (void)Tz1853_ReleaseKey(dmobj, ctxt);

                api_log_MsgDebug("Ѱ·:����:��ס,ֹͣѰ·:�ȴ�������ʧ,��һ����, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //�ȴ��ض�������ʧ
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //�ȼ����Ѫ��
            code = Tz1853_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("Ѱ·:Ѫ��:���,����������");
                return code;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            //��ȡ��ǰ����
            dmobj->SetDict(0, FONT1853_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)Tz1853_ReleaseKey(dmobj, ctxt);
                
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

                (void)Tz1853_ReleaseKey(dmobj, ctxt);
                
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
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT1853_DIR"font-waitcopyoff.txt");
        code = dmobj->FindStrFast(1, 160, 190, 200, "������ʧ�ȴ�", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ѱ·:��ס:������ʧ�ȴ�:����,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT1853_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ѱ·:��ס:��ͼ:������:�˳�,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->KeyDownChar("u");
        Delay(200);
        dmobj->KeyUpChar("u");
        Delay(200);

        for (cindex = 0; cindex < 3; cindex++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            for (cjndex = 1; cjndex < 9; cjndex++)
            {
                dmobj->KeyDown(48 + cjndex);
                Delay(50);
                dmobj->KeyUp(48 + cjndex);
                Delay(50);
            }
        }

        //���Ѫ��
        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:��ס:������ʧ�ȴ�:Ѫ��:���,����������");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("Ѱ·:��ס:������ʧ�ȴ�:����,ʧ��");
    }

    Delay_With_Terminate_Check(4, luaman_task, dmobj);

    //����ͼ�Ƿ��˳�����������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("9");
        Delay(200);
        dmobj->KeyUpChar("9");
        Delay(200);
        
        dmobj->SetDict(0, FONT1853_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ѱ·:��ס:��ͼ:������:�˳�,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("Ѱ·:��ס:��ͼ:������:�˳�,�����˳���");
    }

    return SNAIL_ERRNO_BLOCKED;
}

int Tz1853_ExitGame(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("esc");
        Delay(500);
        dmobj->KeyUpChar("esc");
        Delay(500);

        dmobj->SetDict(0, FONT1853_DIR"font-exitgame2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ɫ", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("esc:��Ϸ����:����:�ɹ�:lposX:%ld,lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("esc:��Ϸ����:����:ʧ��");
        Delay(1000);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONT1853_DIR"font-exitgame2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ϸ��Ϣ����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            api_log_MsgDebug("esc:��Ϸ����:���,�ɹ�");
            
            dmobj->UnBindWindow();
            _endthreadex(1);
        }

        api_log_MsgDebug("esc:��Ϸ����:���,ʧ��");
        Delay(1000);
    }

    return SNAIL_ERRNO_SUCCESS;
}

static void Tz1853_SavePack2Library(Idmsoft* dmobj, void* ctxt, char* objTypeName)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = luaman_config->cleanPackageX;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX1 = 0, lposY1 = 0;
    long lposX2 = 0, lposY2 = 0;
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    int index;
    CString strPosXYtgt;
    CStringArray posXYtgt;
    int tgtNum = 0;
    int jndex = 0;
    CString strPosXY;
    CStringArray posXY;
    int tgtNum2 = 0;
    long posX, posY;
    
    dmobj->MoveTo(5, 5);
    Delay(100);
        
    //���ұ�����ǩ,����ÿ����ǩ������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT1853_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�ֿ�:����:����:����,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

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
            
            break;
        }

        api_log_MsgDebug("�ֿ�:����:����:����,ʧ��");
        Delay(500);
    }

    for (index = 0; index < 5; index++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //�㿪��ǩҳ
        dmobj->MoveTo(labelPosXY[index].x, labelPosXY[index].y);
        Delay(200);
        dmobj->LeftDown();
        Delay(200);
        dmobj->LeftUp();
        Delay(200);
        
        api_log_MsgDebug("�ֿ�:����:����, index:%d", index);

        //���ұ�������
        if (strcmp(BMP1853_DIR"golden-corn.bmp", objTypeName))
        {
            strPosXYtgt = dmobj->FindPicEx(boxPosX0, boxPosY0, boxPosX1, boxPosY1, objTypeName, "202020", 0.9, 3);
        }
        else
        {
            strPosXYtgt = dmobj->FindPicEx(boxPosX0, boxPosY0, boxPosX1, boxPosY1, objTypeName, "303030", 0.8, 3);
        }

        char* ptr = strPosXYtgt.GetBuffer(BUFFER_MAX);
        api_log_MsgDebug("strPosXYtgt:%s", ptr);
        strPosXYtgt.ReleaseBuffer();
        
        if (CString("") != strPosXYtgt)
        {
            tgtNum = SplitString(strPosXYtgt, '|', posXYtgt);

            api_log_MsgDebug("tgtNum:%d", tgtNum);
            
            for (jndex = 0; jndex < tgtNum; jndex++)
            {
                strPosXY = posXYtgt.GetAt(jndex);
                tgtNum2 = SplitString(strPosXY, ',', posXY);

                char* ptr2 = strPosXY.GetBuffer(BUFFER_MAX);
                api_log_MsgDebug("strPosXY:%s, tgtNum2:%d", ptr2, tgtNum2);
                strPosXY.ReleaseBuffer();
                
                if (tgtNum2 > 2)
                {
                    posX = atol(posXY.GetAt(1));
                    posY = atol(posXY.GetAt(2));

                    api_log_MsgDebug("�ֿ�:����:����:����,�ɹ�,jndex:%d, posX:%ld, posY:%ld", jndex, posX, posY);

                    //����Ҽ�������ӱ����Ƶ��ֿ���
                    dmobj->MoveTo(posX, posY);
                    Delay(200);
                    dmobj->RightClick();
                    Delay(200);
                }
            }
        }
    }

    return;
}

int Tz1853_SaveXstrong2Library(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = luaman_config->cleanPackageX;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX1 = 0, lposY1 = 0;
    long lposX2 = 0, lposY2 = 0;
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    long deltaX = 35;
    long  deltaY = 35;
    long  lineX = -1;
    long  lineY = -1;
    int index = 0;
    int jndex = 0;
    int lndex = 0;
    long posX = 0;
    long posY = 0;
    struct xyposition boxPosXYs[5 * 10];
    int findxstrong = 0;

    api_log_MsgDebug("cleanPackageX:0x%x", cleanPackageX);
    
    //���ұ�����ǩ
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT1853_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("Xǿ:�ֿ�:����:����:����,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

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

            break;
        }

        api_log_MsgDebug("Xǿ:�ֿ�:����:����:����,�ɹ�");
        Delay(500);
    }

    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;
    
    api_log_MsgDebug("Xǿ:�ֿ�:����:�����, lineX:%ld, lineY:%ld", lineX, lineY);

    for (index = 0; index < 5; index++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        posY = lineY + index * deltaY;
        
        for (jndex = 0; jndex < 10; jndex++)
        {
            posX = lineX + jndex * deltaX;
            
            boxPosXYs[index * 10 + jndex].x = posX;
            boxPosXYs[index * 10 + jndex].y = posY;

            api_log_MsgDebug("boxPosXYs[%d][%d] = [%ld, %ld]", index, jndex, posX, posY);
        }
    }

    api_log_MsgDebug("Xǿ:�ֿ�:����:�����,׼������");

    //����ÿ�����������б����
    for (lndex = 0; lndex < 5; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }

        //�㿪��ǩҳ
        dmobj->MoveTo(labelPosXY[lndex].x, labelPosXY[lndex].y);
        Delay(200);
        dmobj->LeftDown();
        Delay(200);
        dmobj->LeftUp();
        Delay(200);

        for (index = 0; index < 5; index++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            for (jndex = 0; jndex < 10; jndex++)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                posX = boxPosXYs[index * 10 + jndex].x;
                posY = boxPosXYs[index * 10 + jndex].y;

                dmobj->MoveTo(posX, posY);
                Delay(200);

                api_log_MsgDebug("����:���:����:%d:����:[%d,%d]:����:[%ld, %ld]",
                    lndex, index, jndex, posX, posY);
                
                findxstrong = 0;

                //����'Сǿ|��ǿ|��ǿ'����
                dmobj->SetDict(0, FONT1853_DIR"font-xstrong.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "ҫ�����", "22741c-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    findxstrong = 1;

                    lposX1 = pposX1.lVal;
                    lposY1 = pposY1.lVal;

                    Delay(200);
                    dmobj->RightClick();
                    Delay(200);

                    api_log_MsgDebug("�����:ҫ�����:��ǿ:�洢,�ɹ�, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                    //break;
                }

                if (0 == findxstrong)
                {
                    dmobj->SetDict(0, FONT1853_DIR"font-xstrong.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "22741c-101010", 0.9, &pposX1, &pposY1);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        findxstrong = 2;

                        lposX1 = pposX1.lVal;
                        lposY1 = pposY1.lVal;

                        Delay(200);
                        dmobj->RightClick();
                        Delay(200);

                        api_log_MsgDebug("�����:�������:��ǿ:�洢,�ɹ�, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                        //break;
                    }
                }

                if (0 == findxstrong)
                {
                    dmobj->SetDict(0, FONT1853_DIR"font-xstrong.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "��ͭ���", "22741c-101010", 0.9, &pposX1, &pposY1);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        findxstrong = 3;

                        lposX1 = pposX1.lVal;
                        lposY1 = pposY1.lVal;

                        Delay(200);
                        dmobj->RightClick();
                        Delay(200);

                        api_log_MsgDebug("�����:��ͭ���:��ǿ:�洢,�ɹ�, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                        //break;
                    }
                }
            }
        }
    }

    return SNAIL_ERRNO_SUCCESS;
}

int Tz1853_BeaSellman(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = luaman_config->cleanPackageX;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX1 = 0, lposY1 = 0;
    long lposX2 = 0, lposY2 = 0;
    int posX1 = 0;
    int posY1 = 0;
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    long deltaX = 35;
    long  deltaY = 35;
    long  lineX = -1;
    long  lineY = -1;
    int index = 0;
    int jndex = 0;
    int lndex = 0;
    long posX = 0;
    long posY = 0;
    struct xyposition boxPosXYs[5 * 10];
    
    api_log_MsgDebug("����:��ͨ180:��ʼ");

    posX1 = 1069;
    posY1 = 48;

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        //���֮�����"�ֿ�"����
        dmobj->SetDict(0, FONT1853_DIR"font-surebox.txt");
        code = dmobj->FindStrFast(590, 500, 687, 530, "�ֿ�", "ffffff-101010", 0.9, &pposX1, &pposY1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX1 = pposX1.lVal;
            lposY1 = pposY1.lVal;

            api_log_MsgDebug("�̵�:�ֿ�:����:����:�ɹ�, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
            break;
        }
        
        api_log_MsgDebug("�̵�:�ֿ�:����:����:ʧ��");
        Delay(500);
    }

    //����̵� 633:510->633:380
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX1, lposY1 - 130);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        //���ҳ��ֱ������������ɹ�
        dmobj->SetDict(0, FONT1853_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�̵�:����:����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

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
            
            break;
        }

        api_log_MsgDebug("�̵�:����:����:����:ʧ��");
        Delay(500);
    }

    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;

    api_log_MsgDebug("Xǿ:�ֿ�:����:�����, lineX:%ld, lineY:%ld", lineX, lineY);

    for (index = 0; index < 5; index++)
    {
        posY = lineY + index * deltaY;
        
        for (jndex = 0; jndex < 10; jndex++)
        {
            posX = lineX + jndex * deltaX;
            
            boxPosXYs[index * 10 + jndex].x = posX;
            boxPosXYs[index * 10 + jndex].y = posY;

            api_log_MsgDebug("boxPosXYs[%d][%d] = [%ld, %ld]", index, jndex, posX, posY);
        }
    }

    for (lndex = 0; lndex < 5; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }

        //�㿪��ǩҳ
        dmobj->MoveTo(labelPosXY[lndex].x, labelPosXY[lndex].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        for (index = 0; index < 5; index++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            for (jndex = 0; jndex < 10; jndex++)
            {
                posX = boxPosXYs[index * 10 + jndex].x;
                posY = boxPosXYs[index * 10 + jndex].y;

                dmobj->MoveTo(posX, posY);
                Delay(250);

                api_log_MsgDebug("����:180:��Ƭ:����:%d:����:[%d,%d]:����:[%ld, %ld]",
                lndex, index, jndex, posX, posY);
                
                dmobj->SetDict(0, FONT1853_DIR"font-beasellman.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "��ͨ", "ffe08c-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (0 != code)
                {
                    continue;
                }

                dmobj->SetDict(0, FONT1853_DIR"font-beasellman.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "180", "ffe08c-101010", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (0 != code)
                {
                    continue;
                }

                dmobj->SetDict(0, FONT1853_DIR"font-clr180.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������ı�����Ƭ", "ffffff-101010", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (0 == code)
                {
                    continue;
                }

                Delay(200);
                dmobj->RightClick();
                Delay(300);
                dmobj->KeyDownChar("enter");
                Delay(300);
                dmobj->KeyUpChar("enter");
                Delay(300);
            }
        }
    }

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("����:��ͨ180:���");
    return SNAIL_ERRNO_SUCCESS;
}

int Tz1853_CleanPackage(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = luaman_config->cleanPackageX;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX1 = 0, lposY1 = 0;
    long lposX2 = 0, lposY2 = 0;
    CString strArrangePackPosXYs;
    CStringArray arrangePackPosXYs;
    int tgtNum = 0;
    int tgtNum2 = 0;
    long posX = 0;
    long posY = 0;
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    int index = 0;
    int jndex = 0;
    CString strPosXYtgt;
    CStringArray posXYtgt;
    CString strPosXY;
    CStringArray posXY;
    long posX1 = 0, posY1 = 0;
    long buildx = 0, buildy = 0;

    //��i��������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(200);
        dmobj->KeyUpChar("i");
        Delay(200);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT1853_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("������:����:����,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("������:����:����,ʧ��");
        Delay(500);
    }

    api_log_MsgDebug("���:׼��:���");
    
    //916,126->786,522
    {
        buildx = lposX - 130;
        buildy = lposY + 396;
        
        dmobj->MoveTo(buildx, buildy);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        api_log_MsgDebug("������:���,��i�˳�, buildx:%ld, buildy:%ld", buildx, buildy);
    }
    
    /*
    //��������ͼƬ
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        strArrangePackPosXYs = dmobj->FindPicE(0, 27, 1279, 826, BMP1853_DIR"package.bmp", "202020", 0.9, 3);
        //strArrangePackPosXYs = dmobj->FindPicE(0, 27, 1279, 826, "bmp\\1853\\package.bmp", "202020", 0.9, 3);
        if (strArrangePackPosXYs != CString(""))
        {
            //char* ptr = strArrangePackPosXYs.GetBuffer(BUFFER_MAX);
            //api_log_MsgDebug("FindPicE package.bmp, ptr:%s", ptr);
            //strArrangePackPosXYs.ReleaseBuffer();
            
            tgtNum = SplitString(strArrangePackPosXYs, '|', arrangePackPosXYs);

            //api_log_MsgDebug("tgtNum:%d", tgtNum);
            
            if (3 == tgtNum || 6 == tgtNum)
            {
                if (3 == tgtNum)
                {
                    posX = atol(arrangePackPosXYs.GetAt(1));
                    posY = atol(arrangePackPosXYs.GetAt(2));
                }
                else
                {
                    posX = atol(arrangePackPosXYs.GetAt(4));
                    posY = atol(arrangePackPosXYs.GetAt(5));
                }

                api_log_MsgDebug("������:λ��:����,�ɹ�, tgtNum:%d, posX:%ld, tposY:%ld", tgtNum, posX + 10, posY + 10);
                
                dmobj->MoveTo(posX + 10, posX + 10);
                Delay(500);
                dmobj->LeftDown();
                Delay(500);
                dmobj->LeftUp();
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                
                dmobj->KeyDownChar("i");
                Delay(500);
                dmobj->KeyUpChar("i");
                Delay(500);

                api_log_MsgDebug("������:���,��i�˳�");
                break;
            }
        }

        api_log_MsgDebug("������:��ͼ,ʧ��");
        Delay(200);
    }
    */

    //���°�i,����¼����I/II/III/IV/V������λ��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT1853_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:����,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

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

            break;
        }

        api_log_MsgDebug("����:����:����,ʧ��");
        Delay(500);
    }

    //���δ���ÿһ������
    for (index = 0; index < 5; index++)
    {
        //�㿪��ǩҳ
        dmobj->MoveTo(labelPosXY[index].x, labelPosXY[index].y);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("����:����,jndex:%d", jndex);
        
        //����"����"ͼ��
        //strPosXYtgt = dmobj->FindPicEx(boxPosX0, boxPosY0, boxPosX1, boxPosY1, BMP1853_DIR"box.bmp", "000000", 0.9, 3);
        strPosXYtgt = dmobj->FindPicEx(boxPosX0, boxPosY0, boxPosX1, boxPosY1, "bmp\\1853\\box.bmp", "000000", 0.9, 3);
        if (strPosXYtgt != CString(""))
        {
            //char* ptr = strPosXYtgt.GetBuffer(BUFFER_MAX);
            //api_log_MsgDebug("FindPicEx box.bmp, ptr:%s", ptr);
            //strPosXYtgt.ReleaseBuffer();
            
            tgtNum = SplitString(strPosXYtgt, '|', posXYtgt);

            //api_log_MsgDebug("tgtNum:%d", tgtNum);
            
            if (tgtNum > 0)
            {
                for (jndex = 0; jndex < tgtNum; jndex++)
                {
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    strPosXY = posXYtgt.GetAt(jndex);

                    //char* ptr2 = strPosXY.GetBuffer(BUFFER_MAX);
                    //api_log_MsgDebug("box.bmp, ptr2:%s", ptr2);
                    //strPosXY.ReleaseBuffer();
                    
                    tgtNum2 = SplitString(strPosXY, ',', posXY);

                    //api_log_MsgDebug("tgtNum2:%d", tgtNum2);
                    
                    if (tgtNum2 > 2)
                    {
                        posX = atol(posXY.GetAt(1));
                        posY = atol(posXY.GetAt(2));

                        api_log_MsgDebug("����:����:����,�ɹ�,jndex:%d, posX:%ld, posY:%ld", jndex, posX, posY);

                        //����Ҽ��ҵ�ʹ�õ�������
                        for (;;)
                        {
                            Check_Terminate_Thread(luaman_task, dmobj);
                            
                            //����Ƶ�������,�Ҽ�
                            dmobj->MoveTo(posX, posY);
                            Delay(500);
                            dmobj->RightClick();
                            Delay(500);

                            //��ͼƬ
                            dmobj->MoveTo(5, 5);
                            Delay(500);

                            dmobj->SetDict(0, FONT1853_DIR"font-surebox.txt");
                            code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��Ҫʹ�øõ�����", "ffffff-101010", 0.9, &pposX2, &pposY2);
                            dmobj->ClearDict(0);
                            if (-1 != code)
                            {
                                lposX2 = pposX2.lVal;
                                lposY2 = pposY2.lVal;

                                api_log_MsgDebug("����:����:����:����,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                                break;
                            }

                            api_log_MsgDebug("����:����:����:����,ʧ��");
                            Delay(500);
                        }

                        //���س���
                        for (;;)
                        {
                            Check_Terminate_Thread(luaman_task, dmobj);
                            
                            dmobj->KeyDownChar("enter");
                            Delay(200);
                            dmobj->KeyUpChar("enter");
                            Delay(200);

                            dmobj->SetDict(0, FONT1853_DIR"font-surebox.txt");
                            code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��Ҫʹ�øõ�����", "ffffff-101010", 0.9, &pposX2, &pposY2);
                            dmobj->ClearDict(0);
                            if (-1 == code)
                            {
                                api_log_MsgDebug("����:����,�ɹ�,jndex:%d", jndex);
                                break;
                            }

                            api_log_MsgDebug("����:����,ʧ��,jndex:%d", jndex);
                        }
                        
                    }
                }//������һҳ�е���������
                
            }
        }//������һҳ�е���������

        //����"����"ͼ�� ����
        api_log_MsgDebug("����:����,���,jndex:%d", jndex);
    }

    api_log_MsgDebug("����:����:����,�ɹ�");

    //����������
    {
        dmobj->MoveTo(buildx, buildy);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("������:���,��i�˳�, buildx:%ld, buildy:%ld", buildx, buildy);
    }
    /*
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //strArrangePackPosXYs = dmobj->FindPicE(0, 27, 1279, 826, BMP1853_DIR"package.bmp", "202020", 0.9, 3);
        strArrangePackPosXYs = dmobj->FindPicE(0, 27, 1279, 826, "bmp\\1853\\package.bmp", "202020", 0.9, 3);
        if (strArrangePackPosXYs != CString(""))
        {
            tgtNum = SplitString(strArrangePackPosXYs, '|', arrangePackPosXYs);
            if (3 == tgtNum || 6 == tgtNum)
            {
                if (3 == tgtNum)
                {
                    posX = atol(arrangePackPosXYs.GetAt(1));
                    posY = atol(arrangePackPosXYs.GetAt(2));
                }
                else
                {
                    posX = atol(arrangePackPosXYs.GetAt(4));
                    posY = atol(arrangePackPosXYs.GetAt(5));
                }

                api_log_MsgDebug("�ٴ�������:λ��:����,�ɹ�, tgtNum:%d, posX:%ld, tposY:%ld", tgtNum, posX +10, posY +10);
                
                dmobj->MoveTo(posX + 10, posX + 10);
                Delay(500);
                dmobj->LeftDown();
                Delay(500);
                dmobj->LeftUp();
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->KeyDownChar("i");
                Delay(500);
                dmobj->KeyUpChar("i");
                Delay(500);

                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("������:���,��i�˳�");
                break;
            }
        }

        api_log_MsgDebug("������:��ͼ,ʧ��");
        Delay(200);
    }
    */

    api_log_MsgDebug("׼�����Ҳֿ�ͼ��");
    
    posX1 = 1069;
    posY1 = 48;

    //���ͼ��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);

        //���֮�����"�ֿ�"����
        dmobj->SetDict(0, FONT1853_DIR"font-surebox.txt");
        code = dmobj->FindStrFast(590, 500, 687, 530, "�ֿ�", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�ֿ�:����:����,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�ֿ�:����:����,ʧ��");
        Delay(500);
    }

    api_log_MsgDebug("׼������ֿⰴť, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
    
    //���'�ֿ�'����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);

        //���ҳ��ֱ������������ɹ�
        dmobj->SetDict(0, FONT1853_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX1, &pposY1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX1 = pposX1.lVal;
            lposY1 = pposY1.lVal;

            api_log_MsgDebug("�ֿ�:����:����:����,�ɹ�, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
            break;
        }

        api_log_MsgDebug("�ֿ�:����:����:����,ʧ��");
        Delay(500);
    }

    api_log_MsgDebug("׼��������");

    //�������еĶ���
    Tz1853_SavePack2Library(dmobj, ctxt, BMP1853_DIR"golden-corn.bmp");
    Tz1853_SaveXstrong2Library(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    return SNAIL_ERRNO_SUCCESS;
}

int Tz1853_Login(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;

    //ֱ�ӵ�109,77�����
    dmobj->MoveTo(109, 104);
    Delay(200);
    dmobj->MoveTo(109, 104);
    Delay(200);

    dmobj->LeftClick();
    Delay(200);

    Check_Terminate_Thread(luaman_task, dmobj);
    Delay(2000);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�㽱��
    dmobj->MoveTo(636, 175);
    Delay(200);
    dmobj->MoveTo(636, 175);
    Delay(200);

    dmobj->LeftDown();
    Delay(200);
    dmobj->LeftUp();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    Delay(800);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->MoveTo(636, 230);
    Delay(200);
    dmobj->MoveTo(636, 230);
    Delay(200);

    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->LeftDown();
    Delay(200);
    dmobj->LeftUp();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    //��ESC�˳�
    dmobj->KeyDown(27);
    Delay(200);
    dmobj->KeyUp(27);
    Delay(200);

    Check_Terminate_Thread(luaman_task, dmobj);
    return SNAIL_ERRNO_SUCCESS;
}

int Tz1853_CallPetbehand(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    
    dmobj->KeyDown(16);
    Delay(200);
    dmobj->KeyDown(69);
    Delay(200);

    dmobj->KeyUp(69);
    Delay(200);
    dmobj->KeyUp(16);
    Delay(200);

    Check_Terminate_Thread(luaman_task, dmobj);
    return SNAIL_ERRNO_SUCCESS;
}

int Tz1853_FiveZero(Idmsoft* dmobj, void* ctxt)
{
    int i = 0;

    for (i = 0; i < 5; i++)
    {
        dmobj->KeyDownChar("0");
        Delay(10);
        dmobj->KeyUpChar("0");
        Delay(10);
    }
    
    return SNAIL_ERRNO_SUCCESS;
}

int Tz1853_FightAutoStop(Idmsoft* dmobj, void* ctxt)
{
    dmobj->KeyDownChar("w");
    Delay(25);
    dmobj->KeyUpChar("w");
    Delay(25);
    
    dmobj->KeyDownChar("s");
    Delay(25);
    dmobj->KeyUpChar("s");
    Delay(25);

    return SNAIL_ERRNO_SUCCESS;
}

int Tz1853_FightAutoStart(Idmsoft* dmobj, void* ctxt, int fType)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(105, 143);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONT1853_DIR"font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ʼ", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
                
            api_log_MsgDebug("ս��:����:��:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����:���:Ѫ��:����,����");
            return code;
        }
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONT1853_DIR"font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ʼ", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ս��:����:��ʼ");
            break;
        }

        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����:���:Ѫ��:����,����");
            return code;
        }
    }

    return SNAIL_ERRNO_SUCCESS;
}

int Tz1853_FightInFixPointSimple(Idmsoft* dmobj, void* ctxt,
    int fType, int fightTimeMax, int uFailedCountMax, char* fposXY)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int uFlag = 0;
    int uFailedCount = 0;
    time_t fightStartTime;
    time_t curTime;
    double fightTime;

    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (4 == fType)
    {
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->KeyDownChar("u");
            Delay(200);
            dmobj->KeyUpChar("u");
            Delay(200);

            dmobj->SetDict(0, FONT1853_DIR"font-aa44.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "����ػ���", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                uFlag = 1;
                api_log_MsgDebug("����:u��:�ɹ�:lposX:%ld,lposY:%ld, fposXY:%s", lposX, lposY, fposXY);
                break;
            }

            uFailedCount++;
            if (uFailedCountMax > 0 && uFailedCount > uFailedCountMax)
            {
                api_log_MsgDebug("����:u��:��ʱ:�˳�, fposXY:%s, uFailedCount:%d", fposXY, uFailedCount);
                return SNAIL_ERRNO_SUCCESS;
            }

            code = Tz1853_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("����:u��:Ѫ��:����,����, fposXY:%s, uFailedCount:%d", fposXY, uFailedCount);
                return code;
            }

            api_log_MsgDebug("����:u��:ʧ��, fposXY:%s", fposXY);
        }
    }
    else
    {
        //bug:�ڴ�֮ǰ��5��0 2020/7/30 22:53:09
        Tz1853_FiveZero(dmobj, ctxt);
        code = Tz1853_FightAutoStart(dmobj, ctxt, fType);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����:���:��ʼ:Ѫ��:����,����, fposXY:%s", fposXY);
            return code;
        }
    }

    fightStartTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        if (fType == 1 || 2 == fType || 6 == fType || 7 == fType)
        {
            dmobj->SetDict(0, FONT1853_DIR"font-aa20.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "ͨ���¸��ؿڵ����ѿ�����", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
        }
        else if (31 == fType || 32 == fType || 33 == fType)
        {
            dmobj->SetDict(0, FONT1853_DIR"font-aa54.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ػ��ż���������������ˡ�", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                dmobj->SetDict(0, FONT1853_DIR"font-aa44.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "����ػ���", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
            }
        }
        else if (4 == fType)
        {
            dmobj->SetDict(0, FONT1853_DIR"font-aa57.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "����֮�������ˡ�", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
        }
        else if (8 == fType)
        {
            //���ҽ����ȴ�ʱ�� 2020/6/3 22:41:11
            dmobj->SetDict(0, FONT1853_DIR"font-aa127.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�����ȴ�ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
        }
        else if (9 == fType)
        {
            if (1 == uFlag)
            {
                dmobj->SetDict(0, FONT1853_DIR"font-aa126.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (0 == code)
                {
                    code = -1;
                }
                else
                {
                    //bug:��һ����ɫ�ļ�� 2021/01/02 21:11:13
                    dmobj->SetDict(0, FONT1853_DIR"font-aa126.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (0 == code)
                    {
                        code = -1;
                    }
                    else
                    {
                        //bug:����Ѫ����195����һ�� 2022/06/12 22:36:35
                        //code = dmobj->FindPic(535, 49, 768, 160, BMP1853_DIR"bmp-lastblood.bmp", "000000", 0.9, 0, &pposX, &pposY);
                        //code = dmobj->FindPic(535, 49, 768, 160, "bmp\\1853\\bmp-lastblood.bmp", "000000", 0.9, 0, &pposX, &pposY);
                        code = dmobj->FindPic(968, 194, 1110, 210, "bmp\\1853\\bmp-lastblood.bmp", "000000", 0.9, 0, &pposX, &pposY);
                        if (0 == code)
                        {
                            code = -1;
                        }
                        else
                        {
                            code = 0;
                        }
                    }
                }
            }
            else
            {
                code = -1;
            }
        }
        else
        {
            code = -1;
        }
        
        if (-1 != code)
        {
            if (33 != fType)
            {
                Tz1853_FightAutoStop(dmobj, ctxt);
            }

            api_log_MsgDebug("����:���:�˳�:����:����, fposXY:%s", fposXY);
            return SNAIL_ERRNO_SUCCESS;
        }
        
        if (fightTimeMax > 0)
        {
            curTime = time(NULL);
            fightTime = difftime(curTime, fightStartTime);
            if (fightTime > (fightTimeMax * 1.0))
            {
                if (33 != fType)
                {
                    Tz1853_FightAutoStop(dmobj, ctxt);
                }

                api_log_MsgDebug("���:��ʱ:��ʱ, fposXY:%s", fposXY);
                return SNAIL_ERRNO_AGAIN;
            }
        }

        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            Tz1853_FightAutoStop(dmobj, ctxt);
            
            api_log_MsgDebug("����:���:Ѫ��:����,����, fposXY:%s", fposXY);
            return code;
        }
    }

    Tz1853_FightAutoStop(dmobj, ctxt);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("����:���:���, fposXY:%s", fposXY);
    return SNAIL_ERRNO_SUCCESS;
}

int Tz185_PassCopyLoop(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int waitCopytime = luaman_config->waitCopytime;
    int code = 0;
    int cleanPackageWhenStart = luaman_config->cleanPackageWhenStart;
    int blockTimes = luaman_config->blockTimes;
    int exitGameCopyFull = luaman_config->exitGameCopyFull;
    int cleanPackageTime = luaman_config->cleanPackageTime;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    time_t cleanPackageTimeLast = time(NULL);
    int sleepTimesj = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int copyFullCounter = 0;
    int retryPreFly = 0;
    double cleanPackageTimeDelta = 0.0;
    time_t curTime = time(NULL);
    int findOkFailedCount = 0;
    int roundCounter = 0;
    char* fposXY = NULL;
    int index = 0;
    
    if (1 == cleanPackageWhenStart)
    {
        cleanPackageTimeLast = 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    //@bug:��������:�����ոս���֮�������ű� @2019/4/15 21:28:44
    dmobj->SetDict(0, FONT1853_DIR"font-maptips.txt");
    code = dmobj->FindStrFast(1140, 29, 1273, 58, "���׵Ļ�¯", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("��ͼ:���׵Ļ�¯:�����ű�:�ɹ�:lposX:%ld,lposY:%ld", lposX, lposY);
        goto LABEL_START_SCRIPT_IN_COPY;
    }

    api_log_MsgDebug("��ͼ:������:�����ű�:�ɹ�");

    for (;;)
    {
    COPY_LABEL_AGAIN:
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        sleepTimesj = 0;

        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("�ٽ���:����:�ȴ�");
            Tz1853EnterCriticalSection();
            api_log_MsgDebug("�ٽ���:����:�ɹ�");

            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                code = Tz1853_InviteHeilfellowJoin2Team(dmobj, ctxt);
                if (SNAIL_ERRNO_AGAIN != code)
                {
                    api_log_MsgDebug("���:���");
                    break;
                }

                check_alarm(dmobj, ctxt);
            }

            api_log_MsgDebug("�ٽ���:�˳�:�ȴ�");
            Tz1853LeaveCriticalSection();
            api_log_MsgDebug("�ٽ���:�˳�:�ɹ�");

            //bug:���������ɱ,����ȥ�����ӻ᳤ʱ�����ȥ,ֱ��ȥ�Ի�
            //    ���δ��ӻ���ʾ��Ա���� 2022/05/04 20:30:31
            if (SNAIL_ERRNO_REALIVE == code)
            {
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            //@bug:���ŵ���,�˳���Ϸ����,ֹͣ�߳�,��������Ի��� 2018/4/28 22:43:09
            if (SNAIL_ERRNO_OFFLINE == code)
            {
                //��esc������µ�¼
                for (;;)
                {
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    dmobj->KeyDownChar("esc");
                    Delay(500);
                    dmobj->KeyUpChar("esc");
                    Delay(500);

                    dmobj->SetDict(0, FONT1853_DIR"font-exitgame1.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "������Ϸ", "929292-101010", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("esc:��Ϸ����:����,�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
                        break;
                    }

                    api_log_MsgDebug("esc:��Ϸ����:����,ʧ��");
                    Delay(500);
                }

                for (;;)
                {
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    dmobj->MoveTo(lposX, lposY);
                    Delay(500);
                    dmobj->LeftClick();
                    Delay(500);

                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    dmobj->MoveTo(5, 5);
                    Delay(500);
                
                    dmobj->SetDict(0, FONT1853_DIR"font-exitgame2.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ϸ��Ϣ����", "ffffff-101010", 0.9, &pposX2, &pposY2);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX2 = pposX2.lVal;
                        lposY2 = pposY2.lVal;
                        
                        api_log_MsgDebug("esc:��Ϸ����:���,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                        
                        dmobj->UnBindWindow();
                        _endthreadex(1);
                        break;
                    }
                }

                api_log_MsgDebug("esc:��Ϸ����:���,ʧ��");
                Delay(1000);
                Check_Terminate_Thread(luaman_task, dmobj);
            }
            
            ////////////////////////////////////////////////////////////////////////////
            if (SNAIL_ERRNO_SUCCESS == code)
            {
                api_log_MsgDebug("���:����:�ɹ�");
                break;
            }

            //˯���ڼ���ӦF8�˳�
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                if (sleepTimesj > blockTimes * 60)
                {
                    break;
                }

                Delay(1000);
                Check_Terminate_Thread(luaman_task, dmobj);
                
                sleepTimesj++;
                api_log_MsgDebug("���:����:ʧ��,˯��һ�������, sleepTimesj:%d, blockTimes:%ld", sleepTimesj, blockTimes);
            }
        }
        
        //////////////////////////////////////////////////////////////////////////////////////
        //���ߵ��̶���λ���ٶԻ� 2022/04/28 23:04:11
        //�ƶ���128:121|120:120|119:127
        /*
        pathofposxys = "128:121|120:120|119:127";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[119:127],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[119:127]:�ɹ�");
        */

        copyFullCounter = 0;
        Check_Terminate_Thread(luaman_task, dmobj);
        
    COPY_LABEL_TALK_WITH_NPC:
        Check_Terminate_Thread(luaman_task, dmobj);
        
        retryPreFly = 0;
        
        api_log_MsgDebug("npc1:�Ի�:��ʼ");
        
        Check_Terminate_Thread(luaman_task, dmobj);
    
        //��n�Ի����븱��
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->KeyPressChar("n");
            Delay(500);
            dmobj->KeyUpChar("n");
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
            dmobj->MoveTo(5, 5);
            Delay(200);
            
            dmobj->SetDict(0, FONT1853_DIR"font-npc1.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "[�볡]���׵Ļ�¯", "00ff00-000000", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("npc1:�Ի�:[�볡]���׵Ļ�¯:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            code = Tz1853_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc1:�Ի�:[�볡]���׵Ļ�¯:����:����:����");
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            api_log_MsgDebug("npc1:�Ի�:[�볡]���׵Ļ�¯:����:��Χ,ʧ��");
            Delay(500);
        }

        //���'[�볡]���׵Ļ�¯'
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
            dmobj->MoveTo(5, 5);
            Delay(200);

            //����Ƿ����'ȷ��'�����ĶԻ���
            dmobj->SetDict(0, FONT1853_DIR"font-okcancel.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
            
                api_log_MsgDebug("npc1:�Ի�:ȷ��:����:��Χ,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }
            
            code = Tz1853_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc1:�Ի�:ȷ��:����:����:����");
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            api_log_MsgDebug("npc1:�Ի�:ȷ��:����:��Χ,ʧ��");
            Delay(500);
        }
        
        //���'ȷ��'�����Ի���
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX2, lposY2);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
            dmobj->MoveTo(5, 5);
            Delay(200);

            code = Tz1853_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc1:�Ի�:ȷ��:���:����:����");
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug: ����еض����������,���ȷ��;������ǵ�ȷ�϶�Ӧ��ȡ�� 2020/12/27 14:02:18
            //����еض����������,���ȵ�� 2019/7/19 20:47:59
            dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    dmobj->MoveTo(lposX + 55, lposY + 100);
                    Delay(200);
                    dmobj->LeftClick();
                    Delay(200);
                    dmobj->MoveTo(5, 5);
                    Delay(200);

                    dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("ԭ�ظ���:�������³�����:���:�ɹ�");
                        break;
                    }

                    api_log_MsgDebug("ԭ�ظ���:�������³�����:���:ʧ��");
                    
                    Check_Terminate_Thread(luaman_task, dmobj);
                    Delay(500);
                }

                //@bug:�������³�����������5�����˳���Ϸ 2019/5/19 23:08:15
                if (copyFullCounter > 5)
                {
                    if (1 == exitGameCopyFull)
                    {
                        Tz1853_ExitGame(dmobj, ctxt);
                    }
                    else
                    {
                        copyFullCounter++;
                    }
                }
                
                api_log_MsgDebug("npc1:�Ի�:�������³�����:���¶Ի�,copyFullCounter:%d", copyFullCounter);
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            /////////////////////////////////////////////////////////////////////////////
            //�п��������޷��볡����ʾ 2022/03/06 22:03:48 580,398->636,500
            dmobj->SetDict(0, FONT1853_DIR"font-cantenter.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "���������볡�����볡", "ffffff-202020", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("���������볡�����볡:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
                
                //���"�������³�����"
                for (;;)
                {
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    dmobj->MoveTo(lposX + 56, lposY + 102);
                    Delay(200);
                    dmobj->LeftClick();
                    Delay(200);
                    dmobj->MoveTo(5, 5);
                    Delay(200);

                    dmobj->SetDict(0, FONT1853_DIR"font-cantenter.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "���������볡�����볡", "ffffff-202020", 0.9, &pposX2, &pposY2);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("���������볡�����볡:���:�ɹ�");
                        break;
                    }

                    api_log_MsgDebug("���������볡�����볡:���:ʧ��");
                    
                    Check_Terminate_Thread(luaman_task, dmobj);
                    Delay(500);
                }

                api_log_MsgDebug("npc1:�Ի�:���������볡�����볡:���¶Ի�");
                goto COPY_LABEL_TALK_WITH_NPC;
            }
            /////////////////////////////////////////////////////////////////////////////

            //����Ƿ���븱��
            dmobj->SetDict(0, FONT1853_DIR"font-maptips.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "���׵Ļ�¯", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("npc1:�Ի�:���׵Ļ�¯:����,�ɹ�:lposX:%ld,lposY:%ld", lposX, lposY);
                break;
            }

            //////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:���"��Ա����" 2022/03/15 22:02:55
            dmobj->SetDict(0, FONT1853_DIR"font-peopleleak.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ա����", "ffffff-202020", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("��Ա����:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);

                for (;;)
                {
                    //���ȷ��
                    dmobj->MoveTo(lposX + 32, lposY + 104);
                    Delay(300);
                    dmobj->LeftClick();
                    Delay(500);
                    dmobj->MoveTo(5, 5);
                    Delay(300);
                    
                    dmobj->SetDict(0, FONT1853_DIR"font-peopleleak.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ա����", "ffffff-202020", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("��Ա����:���:�ɹ�:���¶Ի�");
                        goto COPY_LABEL_AGAIN;
                    }

                    Check_Terminate_Thread(luaman_task, dmobj);
            
                    code = Tz1853_CheckLife(dmobj, ctxt);
                    if (SNAIL_ERRNO_REALIVE == code)
                    {
                        api_log_MsgDebug("npc1:�Ի�:���׵Ļ�¯:����,����������");
                        goto COPY_LABEL_AGAIN;
                    }

                    api_log_MsgDebug("npc1:�Ի�:���׵Ļ�¯,���ڽ���");
                    Delay(500);
                }
                
            }
            //////////////////////////////////////////////////////////////////////////////////////////////////////
            
            Check_Terminate_Thread(luaman_task, dmobj);
            
            code = Tz1853_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc1:�Ի�:���׵Ļ�¯:����,����������");
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            api_log_MsgDebug("npc1:�Ի�:���׵Ļ�¯,���ڽ���");
            Delay(500);
        }
        
        //@bug:ǩ�� 2019/4/12 23:27:49 ����Ҫ��ǩ��
        check_alarm(dmobj, ctxt);

        //��������:������������߼�
        if (0 == cleanPackageTimeLast)
        {
            Tz1853_CleanPackage(dmobj, ctxt);
            cleanPackageTimeLast = time(NULL);
            Tz1853_BeaSellman(dmobj, ctxt);
        }
        else
        {
            curTime = time(NULL);
            cleanPackageTimeDelta = difftime(curTime, cleanPackageTimeLast);
            if (cleanPackageTimeDelta >= (cleanPackageTime * 3600 * 1.0))
            {
                Tz1853_CleanPackage(dmobj, ctxt);
                cleanPackageTimeLast = time(NULL);
                Tz1853_BeaSellman(dmobj, ctxt);
            }
        }

        /////////////////////////////////////////////////////////////////////////////////
LABEL_START_SCRIPT_IN_COPY:
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //��'0'�ȴ�3��|��'-'�ȴ�3��|��'='�ȴ�3��
        //bug:�Ƶ��������ȥ
        if (0 == luaman_config->callbabydie)
        {
            dmobj->KeyDown(84);//t
            Delay(500);
            dmobj->KeyUp(84);
            Delay(1000);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDown(48);//0
        Delay(500);
        dmobj->KeyUp(48);
        Delay(500);
        Delay(2500);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDown(187);//=
        Delay(500);
        dmobj->KeyUp(187);
        Delay(500);
        Delay(2500);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //�ٻ���������
        dmobj->KeyDown(16);
        Delay(200);
        dmobj->KeyDown(69);
        Delay(200);
        
        dmobj->KeyUp(69);
        Delay(200);
        dmobj->KeyUp(16);
        Delay(200);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        pathofposxys = "470:372|462:379|455:385";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:ǰ��:����:[455:385],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[455:385]:�ɹ�");

        //@bug:��ǩ���ĵ������� 2019/5/15 21:41:23
        Tz1853_Login(dmobj, ctxt);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        ////////////////////////////////////////////////////////////////////////////////////
        //@bug:��鸱������ʱ��,�оͲ����ٶԻ�4/8/2019 23:43:55
        dmobj->SetDict(0, FONT1853_DIR"font-copyrunning.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                //��n�Ի�
                dmobj->KeyPressChar("n");
                Delay(500);
                dmobj->KeyUpChar("n");
                Delay(500);

                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(5, 5);
                Delay(200);

                //����'֪���ˡ�(��ʼ)'����
                dmobj->SetDict(0, FONT1853_DIR"font-npc2.txt");
                code = dmobj->FindStrFast(388, 115, 984, 667, "֪���ˡ�(��ʼ)", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("npc2:�Ի�:֪���ˡ�(��ʼ):����,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }
                

                api_log_MsgDebug("npc2:�Ի�:֪���ˡ�(��ʼ):����,ʧ��");
                Delay(500);
            }
            

            findOkFailedCount = 0;
            
            //���'֪���ˡ�(��ʼ)'����
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX, lposY);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);

                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT1853_DIR"font-npc2.txt");
                code = dmobj->FindStrFast(388, 115, 984, 667, "֪���ˡ�(��ʼ)", "00ff00-101010", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("npc2:�Ի�:֪���ˡ�(��ʼ):���,�ɹ�");
                    break;
                }

                findOkFailedCount++;
                if (findOkFailedCount > 10)
                {
                    api_log_MsgDebug("npc2:�Ի�:֪���ˡ�(��ʼ):���:ʧ��:��ʱ,����:��������");
                    goto LABEL_find_failed_after_npc_talk;
                }

                api_log_MsgDebug("npc2:�Ի�:�õ�:����,ʧ��");
                Delay(500);
            }
        }

    LABEL_find_failed_after_npc_talk:
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //�ٻ���������
        Tz1853_CallPetbehand(dmobj, ctxt);

        //�ƶ���228:155���괦
        //bug:�޸�����4/8/2019 08:11:31
        pathofposxys = "455:385|446:384|436:383|425:383|415:382|405:383|397:389";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:ǰ��:����:[397:389],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[397:389],�ɹ�");
    
        roundCounter = 0;
        
    LABEL_fight_at_point_1:
        Check_Terminate_Thread(luaman_task, dmobj);
        
        fposXY = "397:389";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 1, 10, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("����:���:����:[397:389],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[397:389]:�ɹ�,roundCounter:%d", roundCounter);

        //�ƶ���393:373���괦
        pathofposxys = "385:389|380:382|385:374|393:373";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:ǰ��:����:[393:373],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[393:373],�ɹ�, roundCounter:%d", roundCounter);

        fposXY = "393:373";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 1, 20, 10, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("����:���:����:[393:373],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[393:373]:�ɹ�,roundCounter:%d", roundCounter);

        if (SNAIL_ERRNO_AGAIN == code)
        {
            roundCounter++;
            if (roundCounter < 2)
            {
                pathofposxys = "393:373|385:374|380:382|385:389|397:389";
                pathOfPosXYs = CString(pathofposxys);
                code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
                if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                {
                    Delay(1000);
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    api_log_MsgDebug("Ѱ·:ǰ��:����:[397:389],����������");
                    goto COPY_LABEL_AGAIN;
                }

                api_log_MsgDebug("Ѱ·:ǰ��:����:[397:389],�ɹ�, roundCounter:%d", roundCounter);
                goto LABEL_fight_at_point_1;
            }
        }
        
        api_log_MsgDebug("����:���:����:[393:373]:�ɹ�, roundCounter:%d", roundCounter);

        //�ƶ���293:394���괦
        pathofposxys = "386:383|378:384|371:384|361:384|351:384|341:384|331:383|320:389|310:393|293:394";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:ǰ��:����:[293:394],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[293:394],�ɹ�");

        roundCounter = 0;

   LABEL_fight_at_point_2:
        Check_Terminate_Thread(luaman_task, dmobj);
        
        fposXY = "293:394";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 2, 10, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("����:���:����:[293:394],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[293:394]:�ɹ�,roundCounter:%d", roundCounter);

        //�ƶ���305:367���괦
        pathofposxys = "287:384|290:372|298:364|308:366|305:367";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:ǰ��:����:[305:367],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[305:367],�ɹ�,roundCounter:%d", roundCounter);
        
        fposXY = "305:367";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 2, 20, 10, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[305:367],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[305:367]:�ɹ�,roundCounter:%d", roundCounter);

        if (SNAIL_ERRNO_AGAIN == code)
        {
            roundCounter++;
            if (roundCounter < 2)
            {
                pathofposxys = "305:367|308:366|298:364|290:372|287:384|293:394";
                pathOfPosXYs = CString(pathofposxys);
                code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
                if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                {
                    Delay(1000);
                    Check_Terminate_Thread(luaman_task, dmobj);
                    api_log_MsgDebug("Ѱ·:ǰ��:����:[293:394],����������");
                    goto COPY_LABEL_AGAIN;
                }

                api_log_MsgDebug("Ѱ·:ǰ��:����:[293:394],�ɹ�, roundCounter:%d", roundCounter);
                goto LABEL_fight_at_point_2;
            }
        }

        api_log_MsgDebug("����:���:����:[293:394],�ɹ�");

        //�ƶ���278:305���괦
        pathofposxys = "307:366|304:355|303:344|297:336|292:328|291:320|289:310|278:305";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[278:305],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[278:305],�ɹ�");

        roundCounter = 0;
        
    LABEL_fight_at_point_3:
        fposXY = "278:305";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 31, 10, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[278:305],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[278:305]:�ɹ�,roundCounter:%d", roundCounter);

        //�ƶ���287:273���괦
        pathofposxys = "278:305|278:292|278:278|287:273";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[287:273],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[287:273],�ɹ�,roundCounter:%d", roundCounter);

        fposXY = "287:273";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 32, 10, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[287:273],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[287:273]:�ɹ�,roundCounter:%d", roundCounter);

        //�ƶ���294:295���괦
        pathofposxys = "298:274|309:280|309:293|307:303|294:295";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[294:295],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[294:295],�ɹ�,roundCounter:%d", roundCounter);
    
        fposXY = "294:295";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 33, 10, 10, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[294:295],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[294:295]:�ɹ�,roundCounter:%d", roundCounter);

        if (SNAIL_ERRNO_AGAIN == code)
        {
            roundCounter++;
            if (roundCounter < 2)
            {
                pathofposxys = "294:295|307:303|309:293|309:280|298:274|287:273|278:278|278:292|278:305";
                pathOfPosXYs = CString(pathofposxys);
                code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
                if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                {
                    Delay(1000);
                    Check_Terminate_Thread(luaman_task, dmobj);
                    api_log_MsgDebug("Ѱ·:ǰ��:����:[278:305],����������");
                    goto COPY_LABEL_AGAIN;
                }

                api_log_MsgDebug("Ѱ·:ǰ��:����:[278:305],�ɹ�, roundCounter:%d", roundCounter);
                goto LABEL_fight_at_point_3;
            }
        }

        api_log_MsgDebug("����:���:����:[294:295]:�ɹ�");

        fposXY = "294:295";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 4, 0, 10, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[294:295],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[294:295]:�ɹ�");
        
        //����: ע�͵����δ���,ֱ��ǰ������һ���� 2020/6/3 22:28:46
        pathofposxys = "294:295|289:282|282:273|286:266|286:255|286:243|285:236|280:231";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[280:231],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[280:231],�ɹ�");
        
        roundCounter = 0;
        
    LABEL_fight_at_point_4:
        fposXY = "280:231";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 6, 40, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[280:231],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[280:231]:�ɹ�, roundCounter:%d", roundCounter);
        
        //�ƶ���281:202���괦
        pathofposxys = "282:233|282:224|282:213|281:200|281:202";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[281:202],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[281:202],�ɹ�, roundCounter:%d", roundCounter);

        fposXY = "281:202";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 6, 30, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[281:202],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[281:202]:�ɹ�, roundCounter:%d", roundCounter);
        
        //bug:���ǸĻ��� 2020/8/17 23:10:11
        pathofposxys = "282:201|272:196|261:196|249:197";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[249:197],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[249:197],�ɹ�, roundCounter:%d", roundCounter);
        
        fposXY = "249:197";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 6, 30, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[249:197],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[249:197]:�ɹ�, roundCounter:%d", roundCounter);
        
        //�ƶ���251:230���괦
        pathofposxys = "249:197|251:204|251:212|252:220|250:234|251:230";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[251:230],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[251:230],�ɹ�, roundCounter:%d", roundCounter);

        fposXY = "251:230";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 6, 30, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[251:230],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[251:230]:�ɹ�, roundCounter:%d", roundCounter);

        if (SNAIL_ERRNO_AGAIN == code)
        {
            roundCounter++;
            if (roundCounter < 2)
            {
                pathofposxys = "251:230|250:234|252:220|251:212|251:204|249:197|261:196|272:196|282:201|281:202|281:200|282:213|282:224|282:233|280:231";
                pathOfPosXYs = CString(pathofposxys);
                code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
                if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                {
                    Delay(1000);
                    Check_Terminate_Thread(luaman_task, dmobj);
                    api_log_MsgDebug("Ѱ·:ǰ��:����:[280:231],����������");
                    goto COPY_LABEL_AGAIN;
                }

                api_log_MsgDebug("Ѱ·:ǰ��:����:[280:231],�ɹ�, roundCounter:%d", roundCounter);
                goto LABEL_fight_at_point_4;
            }
        }
        
        api_log_MsgDebug("����:���:����:[251:230]:�ɹ�");
        
        //�ƶ���198:224���괦
        pathofposxys = "251:234|245:240|238:240|229:240|221:241|211:245|212:233|208:227|198:224";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[198:224],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[198:224],�ɹ�, roundCounter:%d", roundCounter);
        
        roundCounter = 0;
        
    LABEL_fight_at_point_5:
        fposXY = "198:224";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 7, 30, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[198:224],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[198:224]:�ɹ�, roundCounter:%d", roundCounter);
        
        //�ƶ���157:244���괦
        pathofposxys = "200:225|190:230|183:239|177:243|165:244|157:244";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[157:244],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[157:244],�ɹ�, roundCounter:%d", roundCounter);

        fposXY = "157:244";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 7, 30, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[157:244],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[157:244]:�ɹ�, roundCounter:%d", roundCounter);

        //�ƶ���156:185���괦
        pathofposxys = "162:245|169:233|172:226|154:225|153:215|153:205|156:185";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[156:185],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[156:185],�ɹ�, roundCounter:%d", roundCounter);
        
        fposXY = "156:185";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 7, 30, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[156:185],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[156:185]:�ɹ�, roundCounter:%d", roundCounter);
        
        //�ƶ���208:190���괦
        pathofposxys = "155:187|167:185|178:184|189:184|206:185|209:200|208:190";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[208:190],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[208:190],�ɹ�, roundCounter:%d", roundCounter);

        fposXY = "208:190";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 7, 45, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[208:190],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[208:190]:�ɹ�, roundCounter:%d", roundCounter);
        
        if (SNAIL_ERRNO_AGAIN == code)
        {
            roundCounter++;
            if (roundCounter < 2)
            {
                pathofposxys = "208:190|209:200|206:185|189:184|178:184|167:185|155:187|156:185|153:205|153:215|154:225|172:226|169:233|162:245|157:244|165:244|177:243|183:239|190:230|200:225|198:224";
                pathOfPosXYs = CString(pathofposxys);
                code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
                if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                {
                    Delay(1000);
                    Check_Terminate_Thread(luaman_task, dmobj);
                    api_log_MsgDebug("Ѱ·:ǰ��:����:[198:224],����������");
                    goto COPY_LABEL_AGAIN;
                }

                api_log_MsgDebug("Ѱ·:ǰ��:����:[198:224],�ɹ�, roundCounter:%d", roundCounter);
                goto LABEL_fight_at_point_5;
            }
        }

        api_log_MsgDebug("����:���:����:[208:190]:�ɹ�");
        
        //�ƶ���199:146���괦
        pathofposxys = "206:193|201:162|199:174|199:157|199:146";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[199:146],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[199:146],�ɹ�");
        
        //��n�Ի�
        dmobj->MoveTo(5, 5);
        Delay(200);

        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->KeyPressChar("n");
            Delay(500);
            dmobj->KeyUpChar("n");
            Delay(500);

            dmobj->SetDict(0, FONT1853_DIR"font-aa119.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�������档", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("�Ի�:�������档:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("�Ի�:�������档:����:ʧ��");
            Delay(500);
        }

        //���'�������档'
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX, lposY);
            Delay(200);
            dmobj->LeftClick();
            Delay(200);
            dmobj->MoveTo(5, 5);
            Delay(100);

            dmobj->SetDict(0, FONT1853_DIR"font-aa120.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ǣ���Ҫ��ȥ��", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;

                api_log_MsgDebug("�Ի�:�ǣ���Ҫ��ȥ��:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }

            api_log_MsgDebug("�Ի�:�ǣ���Ҫ��ȥ��:����:ʧ��");
            Delay(500);
        }

        //���'�ǣ���Ҫ��ȥ��'
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX2, lposY2);
            Delay(200);
            dmobj->LeftClick();
            Delay(200);
            dmobj->MoveTo(5, 5);
            Delay(100);

            dmobj->SetDict(0, FONT1853_DIR"font-aa120.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ǣ���Ҫ��ȥ��", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�Ի�:�ǣ���Ҫ��ȥ��:���:�ɹ�");
                break;
            }

            api_log_MsgDebug("�Ի�:�ǣ���Ҫ��ȥ��:���:ʧ��");
            Delay(500);
        }
        
        //�ƶ���198:102���괦
        pathofposxys = "199:132|198:120|198:108|198:102";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[198:102],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[198:102],�ɹ�");

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //bug:�Ȱ��¼��� 2020/6/3 23:19:44
        dmobj->KeyDownChar("-");
        Delay(200);
        dmobj->KeyUpChar("-");
        Delay(200);

        dmobj->KeyDownChar("shift");
        Delay(200);
        dmobj->KeyDownChar("e");
        Delay(200);
        dmobj->KeyUpChar("shift");
        Delay(200);
        dmobj->KeyUpChar("e");
        Delay(200);

        fposXY = "198:102";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 8, 0, 15, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[198:102],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[198:102]:�ɹ�");


        Delay_With_Terminate_Check(2, luaman_task, dmobj);

    Label_end_next:
        fposXY = "198:102";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 9, 10, 15, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:���:����:[198:102],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("����:���:����:[198:102]:�ɹ�");
        
        //bug: 2021/01/03 09:09:09 09:42:23 ��20���15�¿ո� 2021/01/03 20:51:11
        Delay_With_Terminate_Check(20, luaman_task, dmobj);

        for (index = 0; index < 16; index++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->KeyDownChar("space");
            Delay(500);
            dmobj->KeyUpChar("space");
            Delay(500);
        }

        pathofposxys = "197:103|200:89";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:ǰ��:����:[200:89],����������");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[200:89],�ɹ�");

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("9");
        Delay(500);
        dmobj->KeyUpChar("9");
        Delay(500);

        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->KeyPressChar("n");
            Delay(500);
            dmobj->KeyUpChar("n");
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, FONT1853_DIR"font-aa130.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�뿪������", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("�Ի�:�뿪������:����:�ɹ�:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            dmobj->SetDict(0, FONT1853_DIR"font-endnext.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "��ô��ȷ�Ͽ�����", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("�Ի�:��ô��ȷ�Ͽ�����:����:�ɹ�:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                
                //�ر�"��ô��ȷ�Ͽ�����"
                dmobj->KeyDownChar("esc");
                Delay(500);
                dmobj->KeyUpChar("esc");
                Delay(500);

                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("ĩβ:���:����");
                goto Label_end_next;
            }
            
            
            api_log_MsgDebug("�Ի�:�뿪������:����:�ɹ�:ʧ��");
            Delay(500);
        }

        //���'�������档'
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX, lposY);
            Delay(200);
            dmobj->LeftClick();
            Delay(200);
            dmobj->MoveTo(5, 5);
            Delay(100);

            dmobj->SetDict(0, FONT1853_DIR"font-aa130.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ǣ���Ҫ��ȥ��", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�Ի�:�뿪������:���:�ɹ�");
                break;
            }

            api_log_MsgDebug("�Ի�:�뿪������:���:ʧ��");
            Delay(500);
        }

        api_log_MsgDebug("����:����:���:����:��һѭ��");

        //�˳�����,�ȴ�������ʧʱ������¿�ʼ��һ��ѭ��
        if (waitCopytime > 0)
        {
            Delay_With_Terminate_Check(waitCopytime, luaman_task, dmobj);
        }
    }//COPY_LABEL_AGAIN

    return SNAIL_ERRNO_SUCCESS;
}

int Tz185_Join2Team(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int exitGameCopyFull = luaman_config->exitGameCopyFull;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    time_t lastChckTime;
    time_t curTime;
    double deltaTime = 0.0;

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //�ж��Ƿ����"�������"
        dmobj->SetDict(0, FONT1853_DIR"font-join2team-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���:����:�������:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        Delay(500);
        api_log_MsgDebug("���:����:�������:����:��Χ,ʧ��");
    }

    //���'��'�����������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX - 32, lposY + 126);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONT1853_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ں�", "f4be3d-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���:����:����ں�:����:��Χ,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Delay(500);
        api_log_MsgDebug("���:����:����ں�:����:��Χ,ʧ��");
    }

    lastChckTime = time(NULL);

    api_log_MsgDebug("��������Ƿ���븱��:��ʼ");
    
    //��������Ƿ���븱��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT1853_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���׵Ļ�¯", "909090-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���:����:���븱��:��ɫ:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONT1853_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���׵Ļ�¯", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���:����:���븱��:��ɫ:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONT1853_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���׵Ļ�¯", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���:����:���븱��:��ɫ:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        if (1 == exitGameCopyFull)
        {
            curTime = time(NULL);
            deltaTime = difftime(curTime, lastChckTime);
            if (deltaTime > 600.0)
            {
                api_log_MsgDebug("��������Ƿ���븱��:����:���븱��:��ʱ");
                break;
            }
        }

        api_log_MsgDebug("���:����:���븱��:ʧ��");
        Delay(500);
    }
    
    //����'�˳�����'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT1853_DIR"font-join2team-3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���:����:�˳�����:����:��ɫ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Delay(500);
        
        dmobj->SetDict(0, FONT1853_DIR"font-join2team-3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���:����:�˳�����:����:��ɫ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���:����:�˳�����:����:ʧ��");
        Delay(500);
    }

    //���'�˳�����'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT1853_DIR"font-join2team-4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��Ҫ�˳�������", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���:����:���븱��:ȷ��Ҫ�˳�������:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("���:����:���븱��:ȷ��Ҫ�˳�������:����:ʧ��");
        Delay(500);
    }

    //���س�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("enter");
        Delay(500);
        dmobj->KeyUpChar("enter");
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->SetDict(0, FONT1853_DIR"font-join2team-4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��Ҫ�˳�������", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���:����:���븱��:ȷ��Ҫ�˳�������:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("���:����:���븱��:ȷ��Ҫ�˳�������:���:ʧ��");
        Delay(500);
    }
    
    api_log_MsgDebug("����:�߳�ִ�����");
    return SNAIL_ERRNO_SUCCESS;
}

int api_Task1853Init(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    time_t start = time(NULL);
    
    api_log_MsgDebug("����:1853:��ʼ��: hwnd:%ld, role:%d", hwnd, (int)role);
    api_PrintConfig(luaman_config);
    
    g_lstChkTime = TlsAlloc();
    TlsSetValue(g_lstChkTime, (LPVOID)start);

    code = Tz1853_AllocClientContext(hwnd);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        if (SNAIL_ERRNO_EXIST == code)
        {
            api_log_MsgError("task1853 is aready running, hwnd:%ld", hwnd);
            AfxMessageBox("������1853����");
            return code;
        }

        if (SNAIL_ERRNO_NOSPACE == code)
        {
            api_log_MsgError("task1853 team member reach max, hwnd:%ld", hwnd);
            AfxMessageBox("1853����������Ա");
            return code;
        }

        api_log_MsgError("task1853 invalid argument, hwnd:%ld", hwnd);
        AfxMessageBox("1853������������");
        
        return code;
    }

    code = Tz1853_UpdateClientTaskId(hwnd, luaman_task->taskid);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        Tz1853_FreeClientContext(hwnd);

        api_log_MsgError("task1853 snail_UpdateClientTaskId failed, hwnd:%ld, code:%d",
            hwnd, code);
        return code;
    }

    /* �󶨴��� */
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("bind 1853 hwnd failed, hwnd:%ld", hwnd);

        Tz1853_FreeClientContext(hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("bind 1853 hwnd success, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("lock 1853 hwnd failed, hwnd:%ld", hwnd);

        Tz1853_FreeClientContext(hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("����:1853:��ʼ��:���, hwnd:%ld", hwnd);

    /* ��ʼ���澯ʱ���ʼֵ */
    init_alarm_ctxt();
    
    return ERRNO_SUCCESS;
}

int api_Task1853Main(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    
    api_log_MsgDebug("����:1853:��ѭ��: hwnd:%ld, role:%d", hwnd, (int)role);

    switch (role)
    {
    case ROLE_M:
        //@bug:�ض�������ʱ���Ž�����Ϸ:exitGameCopyFull 2019/5/20 21:42:16
        api_log_MsgDebug("����:1853:����:��ѭ��:��ʼ, hwnd:%ld", hwnd);
        Tz185_PassCopyLoop(dmobj, ctxt);
        api_log_MsgDebug("����:1853:����:��ѭ��:���, hwnd:%ld", hwnd);
        break;

    default:
        api_log_MsgDebug("����:1853:����:��ѭ��:��ʼ, hwnd:%ld", hwnd);
        Tz185_Join2Team(dmobj, ctxt);
        api_log_MsgDebug("����:1853:����:��ѭ��:���, hwnd:%ld", hwnd);
        break;
    }
    
    return ERRNO_SUCCESS;
}

int api_Task1853Exit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;

    Tz1853_FreeClientContext(hwnd);
    
    api_log_MsgDebug("����:1853:����: hwnd:%ld, role:%d", hwnd, (int)role);

    TlsFree(g_lstChkTime);
    //Tz1853DeleteCriticalSection();
    return ERRNO_SUCCESS;
}

