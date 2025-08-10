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
#include "task195dllext.h"
#include "task195manager.h"
#include "task195.h"

#define FONT195_DIR "font\\195\\"

static HANDLE g_Tz195_criticalSection;
void Tz195_InitCriticalSection(void)
{
    g_Tz195_criticalSection = ::CreateMutex(NULL, FALSE, NULL);
    if (NULL == g_Tz195_criticalSection)
    {
        api_log_MsgError("create Tz195_InitCriticalSection lock failed");
        exit(1);
    }
    return;
}

static void Tz195EnterCriticalSection()
{
    ::WaitForSingleObject(g_Tz195_criticalSection, INFINITE);
    return;
}

static void Tz195LeaveCriticalSection()
{
    (void)::ReleaseMutex(g_Tz195_criticalSection);
    return;
}

static void Tz195DeleteCriticalSection()
{
    (void)::CloseHandle(g_Tz195_criticalSection);
    return;
}

int Tz195_CheckBlueOut(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int maxBlueFiled = luaman_config->maxBlueFiled;
    int minBlue = luaman_config->minBlue;
    int blueThreshold = luaman_config->blueThreshold;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    CString strBBlood;
    CStringArray bBlood;
    char* cstr = NULL;
    int sz = 0;
    long bBloodAva = 0;
    long bBloodTot = 0;
    int strBBloodLen = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int code = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0;
    long posY = 0;
    char* pathofposxys = NULL;
    CString pathOfPosXYs;
    int realived = 0;
    int realived_check_num = 0;
    
Label_check_blood_again:
    realived_check_num = 0;
    
    Check_Terminate_Thread(luaman_task, dmobj);
    
    (void)check_alarm(dmobj, ctxt);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->SetDict(0, "font\\font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("������:Ѫ��:����:�ɹ�, groupid:%d, hwnd:%ld",
            luaman_config->groupid, hwnd);
        return ERRNO_EXCEPTION;
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

        api_log_MsgDebug("������:Ѫ��:����:�ɹ�,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
            hwnd, bBloodAva, bBloodTot);
    }

    if (bBloodAva > blueThreshold)
    {
        ////////////////////////////////////////////////////////////////////
        //bug:�ӽ��븱�����Ƶ����� 2022/05/15 21:13:11
        if (ROLE_M == role && realived)
        {
            dmobj->KeyDownChar("t");
            LUAMAN_WAIT_MS(500);
            dmobj->KeyUpChar("t");
            LUAMAN_WAIT_MS(2500);
        }
        ////////////////////////////////////////////////////////////////////
        
        return (realived)? ERRNO_REALIVE : ERRNO_SUCCESS;
    }

    if (bBloodAva > 0)
    {
        ////////////////////////////////////////////////////////////////////
        //bug:�ӽ��븱�����Ƶ����� 2022/05/15 21:13:11
        if (ROLE_M == role && realived)
        {
            dmobj->KeyDownChar("t");
            LUAMAN_WAIT_MS(500);
            dmobj->KeyUpChar("t");
            LUAMAN_WAIT_MS(2500);
        }
        ////////////////////////////////////////////////////////////////////
        
        dmobj->KeyDownChar("f3");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f3");
        LUAMAN_WAIT_MS(200);

        return (realived)? ERRNO_REALIVE : ERRNO_SUCCESS;
    }

    //Ӣ��������,���"����"����
    for (;;)
    {
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, "font\\font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("������:����:����:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        /////////////////////////////////////////////////////////////////////////////
        //bug:��������ʾ���� 2022/03/21 21:49:22
        dmobj->SetDict(0, "font\\font-chcklifex.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "Ҫ��ԭ�ظ�����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            dmobj->KeyDownChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);

            api_log_MsgDebug("Ҫ��ԭ�ظ�����:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("������:����:����:����:ʧ��");
        LUAMAN_WAIT_MS(500);

        ////////////////////////////////////////////////////////////////////////////
        //bug:2022/03/27 ��������������֮��,��Ҫ���¼��Ѫ��ֱ����ֹ��������
        realived_check_num++;
        if (realived_check_num > 10)
        {
            realived = 1;
            goto Label_check_blood_again;
        }
    }

    //���"����"����Ӣ��
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(200);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(200);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(200);

        dmobj->SetDict( 0, "font\\font-blood.txt");
        strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strBBlood == CString(""))
        {
            api_log_MsgDebug("������:����:����:���:ʧ��");
            LUAMAN_WAIT_MS(500);
            continue;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
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
        }

        if (bBloodAva > 0)
        {
            api_log_MsgDebug("������:����:����:���:�ɹ�");
            break;
        }
    }

    //��m������С��ͼ,Ϊ��ֹ����,�Ƚ�����Ƶ����Ͻ�
    dmobj->MoveTo(5, 5);
    Delay(200);

    Check_Terminate_Thread(luaman_task, dmobj);

    ////////////////////////////////////////////////////////////////////
    //bug:�ӽ��븱�����Ƶ����� 2022/05/15 21:13:11
    if (ROLE_M == role)
    {
        dmobj->KeyDownChar("t");
        LUAMAN_WAIT_MS(500);
        dmobj->KeyUpChar("t");
        LUAMAN_WAIT_MS(2500);
    }
    ////////////////////////////////////////////////////////////////////
    
    for (;;)
    {
        dmobj->KeyDownChar("m");
        Delay(200);
        dmobj->KeyUpChar("m");
        Delay(200);

        //�鿴m���Ƿ񰴳ɹ�
        dmobj->SetDict(0, "font\\font-checkblue-out.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������(��ļ���)", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("������:������(��ļ���):����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:������(��ļ���):����:ʧ��:����:����");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("������:������(��ļ���):����:ʧ��");
    }

    //���"������(��ļ���)"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);

        //����'�ƶ�'����
        dmobj->SetDict(0, "font\\font-fly-mcheck2.txt");
        code = dmobj->FindStrFast(210, 45, 1138, 745, "�ƶ�", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("������:�ƶ�:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:�ƶ�:����:ʧ��:����:����");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("������:�ƶ�:����:ʧ��");
        
    }


    dmobj->MoveTo(lposX2, lposY2);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    //��鵱ǰ����,������겻��245:126������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        dmobj->SetDict(0, "font\\font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 245) <= 1 && abs(posY - 126) <= 1)
                {
                    api_log_MsgDebug("������:�ƶ�:245:126:�ɹ�");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("������:�ƶ�:245:126:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    pathofposxys = "245:133";
    pathOfPosXYs = CString(pathofposxys);

    //TODO:���ø��������Ѱ·,�������ٸ�
    (void)Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("������:�ƶ�:245:133:�ɹ�");
    return ERRNO_REALIVE;
}

int Tz195_CheckBlue(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int maxBlueFiled = luaman_config->maxBlueFiled;
    int minBlue = luaman_config->minBlue;
    int blueThreshold = luaman_config->blueThreshold;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int code = 0;
    CString strBBlood;
    CStringArray bBlood;
    char* cstr = NULL;
    int sz = 0;
    long bBloodAva = 0;
    long bBloodTot = 0;
    int strBBloodLen = 0;
    time_t time_tm;
    struct tm* tm_tm = NULL;
    char thistime[64] = {0};
    int nextcopy = 0;
    int bKickCopyout = 0;
    CString strMapName;
    VARIANT pposX, pposY;
    long lposX = 0, lposY = 0;

    Check_Terminate_Thread(luaman_task, dmobj);
    
    (void)check_alarm(dmobj, ctxt);

    Check_Terminate_Thread(luaman_task, dmobj);

    /////////////////////////////////////////////////////////////////////////////////////
    //bug:�����������������⸴������ 2022/01/08 22:49:33
    dmobj->SetDict(0, "font\\font-mapstyle.txt");
    strMapName = dmobj->Ocr(1140, 29, 1273, 58, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strMapName == CString("�������"))
    {
        api_log_MsgDebug("������:Ѫ��:���");
        return Tz195_CheckBlueOut(dmobj, ctxt);
    }
    /////////////////////////////////////////////////////////////////////////////////////
    
    dmobj->SetDict(0, "font\\font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("Ѫ��:����:�ɹ�, groupid:%d, hwnd:%ld",
            luaman_config->groupid, hwnd);
        return ERRNO_EXCEPTION;
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
        return ERRNO_SUCCESS;
    }

    if (bBloodAva > 0)
    {
        dmobj->KeyDownChar("f3");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f3");
        LUAMAN_WAIT_MS(200);

        return ERRNO_SUCCESS;
    }

    time(&time_tm);
    tm_tm = localtime(&time_tm);

    //bug:�����������ʱ���Ѿ�����Ϊ2022��,c���Ա���ʱ����122,��Ҫ��1900 2022/01/03 11:58:59
    (void)_snprintf(thistime, 63, "%04d-%02d-%02d %02d:%02d:%02d",
        tm_tm->tm_year + 1900,
        tm_tm->tm_mon,
        tm_tm->tm_mday,
        tm_tm->tm_hour,
        tm_tm->tm_min,
        tm_tm->tm_sec);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //��������ʱ��
    MarkLastDietime(groupid, hwnd, thistime);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //����ͬ��״̬
    code = MarkWaitFlag(groupid, hwnd);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (ERRNO_SUCCESS == code)
    {
        api_log_MsgDebug("����:����:���:̫��:ͬ��:�ȴ�:��һ����");

        for(;;)
        {
            (void)check_alarm(dmobj, ctxt);

            Check_Terminate_Thread(luaman_task, dmobj);
            
            code = CheckWaitFlag(groupid);
            if (ERRNO_AGAIN != code)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                if (ERRNO_SUCCESS == code)
                {
                    nextcopy = 1;
                }

                if (1 == bKickCopyout)
                {
                    goto LABEL_Slave_copyout_alive;
                }

                LUAMAN_WAIT_MS(12 * 1000);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:�������˳���10���Ӿͻᱻ����ȥ,Ϊ���ⵯ��ȥ֮��ɱ��,�ڴ�ѭ�������м���ͼ�Ƿ�������,����������
            //    һֱ��Ѫ,ֱ��������ͬ����ɿ�ʼ��һ�ָ��� 2020/3/9 22:18:11
            if (ROLE_M != role)
            {
                dmobj->SetDict(0, "font\\font-mapstyle.txt");
                strMapName = dmobj->Ocr(1140, 29, 1273, 58, "ffffff-101010", 0.9);
                dmobj->ClearDict(0);
                if (strMapName == CString("�������"))
                {
                    dmobj->KeyDownChar("1");
                    LUAMAN_WAIT_MS(200);
                    dmobj->KeyUpChar("1");
                    LUAMAN_WAIT_MS(200);

                    if (!bKickCopyout)
                    {
                        bKickCopyout = 1;
                        api_log_MsgDebug("����:����:��ͣ:ͬ��:�ȴ�:����:����");
                    }
                }
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("����:����:��ͣ:ͬ��:�ȴ�");
            LUAMAN_WAIT_MS(1000);
        }
    }
    else
    {
        api_log_MsgError("����:����:��ͣ:ͬ��:�ȴ�");
        LUAMAN_WAIT_MS(1000);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //Ӣ��������,���"����"����
    for (;;)
    {
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, "font\\font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����:����:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        /////////////////////////////////////////////////////////////////////////////
        //bug:��������ʾ���� 2022/03/26 09:40:22
        dmobj->SetDict(0, "font\\font-chcklifex.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "Ҫ��ԭ�ظ�����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            dmobj->KeyDownChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);

            api_log_MsgDebug("Ҫ��ԭ�ظ�����:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
        }
        
        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("����:����:����:ʧ��");
        LUAMAN_WAIT_MS(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //���"����"����Ӣ��
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(200);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(200);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(200);

        dmobj->SetDict( 0, "font\\font-blood.txt");
        strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strBBlood == CString(""))
        {
            api_log_MsgDebug("����:����:���:ʧ��");
            LUAMAN_WAIT_MS(500);
            continue;
        }

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
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        if (bBloodAva > 0)
        {
            api_log_MsgDebug("����:����:���:�ɹ�");
            break;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
LABEL_Slave_copyout_alive:
    //������һ�·����,��ֹ�ܶ� 2020/1/12 20:43:16
    (void)CheckKeyState(dmobj, ctxt);

    Check_Terminate_Thread(luaman_task, dmobj);

    /////////////////////////////////////////////////////////////////////////
    //bug:�ӽ��븱����ִ���Ƶ���ǰλ�� 2022/05/15 21:10:23
    if (ROLE_M == role)
    {
        dmobj->KeyDownChar("t");
        LUAMAN_WAIT_MS(500);
        dmobj->KeyUpChar("t");
        LUAMAN_WAIT_MS(2500);
    }
    /////////////////////////////////////////////////////////////////////////
    
    if (1 != nextcopy)
    {
        return ERRNO_REALIVE;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (ROLE_M == role)
    {
        ResetGame(groupid, hwnd);
        ResetGroup(groupid);

        ///////////////////////////////////////////////////////////////////////////////////
        //bug: ����Ų���֮��,����Ҫ���½����� 2022/01/02 20:52:33
        //Tz195_InviteHeilfellow(dmobj, ctxt, 2);
        Delay_With_Terminate_Check(29, luaman_task, dmobj);
    }
    else
    {
        ResetGame(groupid, hwnd);
        LUAMAN_WAIT_MS(2000);

        dmobj->KeyDownChar("1");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("1");
        LUAMAN_WAIT_MS(200);

        Delay_With_Terminate_Check(33, luaman_task, dmobj);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    return ERRNO_BLOCK;
}

int Tz195_InviteHeilfellow(Idmsoft* dmobj, void* ctxt, int syncCountMax)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int code = 0;
    int syncCount = 0;
    int j = 0;
    struct xyposition syncPosXYs[4] =
    {
        {1234, 266},
        {1272, 286},
        {1234, 298},
        {1272, 318},
    };
    long posX1 = -1;
    long posY1 = -1;
    long posX2 = -1;
    long posY2 = -1;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long posX = 0, posY = 0;
    int invited = 0;
    struct xyposition fellowPosXYs[2];

    //bug:ע�͵���� 2022/01/08 18:33:15
    return ERRNO_SUCCESS;
    
    goto LABEL_check_heil_fellow;

    //��F���������ѶԻ���
LABEL_invite_heil_fellow:
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(200);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        dmobj->KeyDownChar("f");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f");
        LUAMAN_WAIT_MS(200);

        dmobj->SetDict(0, "font\\font-inviteheil.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ӻ���", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            api_log_MsgDebug("��Ӻ���:����:�ɹ�, groupid:%d, hwnd:%ld, lposX:%ld, lposY:%ld",
                luaman_config->groupid, hwnd, lposX, lposY);
            break;
        }

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("��Ӻ���:����:����:����, groupid:%d, hwnd:%ld",
                luaman_config->groupid, hwnd);
            goto LABEL_invite_heil_fellow;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        LUAMAN_WAIT_MS(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //����Ƶ��
    for (;;)
    {
        dmobj->SetDict(0, "font\\font-inviteheil2team-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ս������", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            api_log_MsgDebug("ս������:����:�ɹ�, groupid:%d, hwnd:%ld, lposX2:%ld, lposY2:%ld",
                luaman_config->groupid, hwnd, lposX2, lposY2);
            break;
        }

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("ս������:����:����:����, groupid:%d, hwnd:%ld",
                luaman_config->groupid, hwnd);
            goto LABEL_invite_heil_fellow;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        LUAMAN_WAIT_MS(500);
    }

    fellowPosXYs[0].x = lposX2;
    fellowPosXYs[0].y = lposY2;
    fellowPosXYs[1].x = lposX2 - 120;
    fellowPosXYs[1].y = lposY2 + 20;

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        for (j = 0; j < 2; j++)
        {
            posX = fellowPosXYs[j].x;
            posY = fellowPosXYs[j].y;

            for (;;)
            {
                dmobj->MoveTo(posX, posY);
                LUAMAN_WAIT_MS(200);
                dmobj->LeftClick();
                LUAMAN_WAIT_MS(200);

                dmobj->SetDict(0, "font\\font-inviteheil.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "7d7d7d-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;
                    api_log_MsgDebug("����:����:�ɹ�, groupid:%d, hwnd:%ld, lposX:%ld, lposY:%ld",
                        luaman_config->groupid, hwnd, lposX, lposY);
                    break;
                }

                code = Tz195_CheckBlue(dmobj, ctxt);
                if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("����:����:����:����, groupid:%d, hwnd:%ld",
                        luaman_config->groupid, hwnd);
                    goto LABEL_invite_heil_fellow;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
            }

            dmobj->MoveTo(lposX, lposY);
            LUAMAN_WAIT_MS(200);
            dmobj->LeftClick();
            LUAMAN_WAIT_MS(200);

            Check_Terminate_Thread(luaman_task, dmobj);
        }

        invited = 1;
        
    LABEL_check_heil_fellow:
        syncCount = 0;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        for (j = 0; j < 2; j++)
        {
            posX1 = syncPosXYs[2 * j].x;
            posY1 = syncPosXYs[2 * j].y;
            posX2 = syncPosXYs[2 * j + 1].x;
            posY2 = syncPosXYs[2 * j + 1].y;

            dmobj->SetDict(0, "font\\font-inviteheil.txt");
            code = dmobj->FindStrFast(posX1, posY1, posX2, posY2, "ͬ��", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                 syncCount = syncCount + 1;
                 
                 api_log_MsgDebug("���:����:��Ա:����, groupid:%d, hwnd:%ld",
                    luaman_config->groupid, hwnd);
            }

            Check_Terminate_Thread(luaman_task, dmobj);
        }

        if (syncCount == syncCountMax)
        {
            api_log_MsgDebug("���:���, groupid:%d, hwnd:%ld",
                luaman_config->groupid, hwnd);
            break;
        }

        if (0 == invited)
        {
            api_log_MsgDebug("���:���:δ��, groupid:%d, hwnd:%ld",
                luaman_config->groupid, hwnd);
            goto LABEL_invite_heil_fellow;
        }

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("���:�ȴ�:����:����, groupid:%d, hwnd:%ld",
                luaman_config->groupid, hwnd);
            goto LABEL_invite_heil_fellow;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    return ERRNO_SUCCESS;
}

void Tz195_ReleaseKey(Idmsoft* dmobj, void* ctxt, struct luamanwalkstate* walkstate)
{
    if (1 == walkstate->xPosKeyDown)
    {
        dmobj->KeyUpChar(walkstate->xPos);
        walkstate->xPosKeyDown = 0;
        LUAMAN_WAIT_MS(50);
    }

    if (1 == walkstate->xNegKeyDown)
    {
        dmobj->KeyUpChar(walkstate->xNeg);
        walkstate->xNegKeyDown = 0;
        LUAMAN_WAIT_MS(50);
    }

    if (1 == walkstate->yNegKeyDown)
    {
        dmobj->KeyUpChar(walkstate->yNeg);
        walkstate->yNegKeyDown = 0;
        LUAMAN_WAIT_MS(50);
    }

    if (1 == walkstate->yPosKeyDown)
    {
        dmobj->KeyUpChar(walkstate->yPos);
        walkstate->yPosKeyDown = 0;
        LUAMAN_WAIT_MS(50);
    }

    (void)ctxt;
    return;
}

int Tz195_CheckMapStyle(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int code = 0;
    CString strMapName;
    
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(300);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        dmobj->SetDict(0, "font\\font-mapstyle.txt");
        strMapName = dmobj->Ocr(1140, 29, 1273, 58, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strMapName == CString("�����Ŀ������"))
        {
            api_log_MsgDebug("��ͼ:ʶ��:�ɹ�:�����Ŀ������");
            return MAP_FORGET_TOWER;
        }

        if (strMapName == CString("�������"))
        {
            api_log_MsgDebug("��ͼ:ʶ��:�ɹ�:�������");
            return MAP_SEGRILA;
        }

        if (strMapName == CString("ƽ���ӹ�"))
        {
            api_log_MsgDebug("��ͼ:ʶ��:�ɹ�:ƽ���ӹ�");
            return MAP_SEGRILA;
        }

        api_log_MsgDebug("��ͼ:ʶ��:ʧ��");
        
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgWarn("��ͼ:ʶ��:����:����, code:%d", code);
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        LUAMAN_WAIT_MS(500);
    }
    
    return ERRNO_FAILED;
}

int Tz195_CheckLefttime2Terminate(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int groupid = luaman_config->groupid;
    int code = 0;
    CString strCopylefttime;
    int count = 0;
    CStringArray copylefttime;
    long copylefttimem = 0;
    char* ptr = NULL;
    int rdcltErrorCounter = 0;
    VARIANT pposX, pposY;
    long lposX = 0, lposY = 0;
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, "font\\font-copylefttime.txt");
        strCopylefttime = dmobj->Ocr(15, 172, 183, 194, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strCopylefttime != CString(""))
        {
            count = SplitStringWide(strCopylefttime, "��", copylefttime);
            if (count > 2)
            {
                copylefttimem = atol(copylefttime.GetAt(0));
                if (copylefttimem < 5)
                {
                    ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                    api_log_MsgDebug("����:ʣ��ʱ��:����:�ȴ�:����:��ʱ:%s", ptr);
                    strCopylefttime.ReleaseBuffer();
                    
                    goto LABEL_Check_left_time_terminate;
                }

                ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                api_log_MsgDebug("����:ʣ��ʱ��:����:%s", ptr);
                strCopylefttime.ReleaseBuffer();
                
                return ERRNO_SUCCESS;
            }
            
            rdcltErrorCounter = rdcltErrorCounter + 1;
            if (rdcltErrorCounter > 20)
            {
                goto LABEL_Check_left_time_terminate;
            }
        }
        else
        {
            /////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:����ҵ��ȴ�ʱ��Ҳ��ʣ�಻���߼����� 2020/2/18 20:55:16
            dmobj->SetDict(0, "font\\font-waittime.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ȴ�ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("�ȴ�ʱ��:��ȡ:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
                goto LABEL_Check_left_time_terminate;
            }

            rdcltErrorCounter = rdcltErrorCounter + 1;
            if (rdcltErrorCounter > 20)
            {
                return ERRNO_SUCCESS;
            }
        }

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("��ͼ:ʶ��:����:����, code:%d", code);
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
    }


LABEL_Check_left_time_terminate:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        code = Tz195_CheckMapStyle(dmobj, ctxt);
        if (MAP_SEGRILA == code)
        {
            api_log_MsgDebug("����:��ʱ:�ȴ�:�˳�����");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        LUAMAN_WAIT_MS(1000);
    }

    if (ROLE_M == role)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        ResetGame(groupid, hwnd);
        ResetGroup(groupid);
        (void)Tz195_InviteHeilfellow(dmobj, ctxt, 2);

        Delay_With_Terminate_Check(29, luaman_task, dmobj);
    }
    else
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        ResetGame(groupid, hwnd);
        Delay_With_Terminate_Check(2, luaman_task, dmobj);
        
        dmobj->KeyDownChar("1");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("1");
        LUAMAN_WAIT_MS(200);
        
        Delay_With_Terminate_Check(33, luaman_task, dmobj);
    }

    return ERRNO_TIMEOUT;
}

int Tz195_MovePosition(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int dir = luaman_config->direction;
    ROLE_E role = luaman_config->role;
    int groupid = luaman_config->groupid;
    int code = 0;
    CStringArray tgtPosXY;
    CStringArray tgtPosXYe;
    int tgtNum = 0;
    char* ptr = NULL;
    time_t lastKeyTimeBlue = 0;
    time_t checkTimeBlue = 0;
    double deltaTimeBlue = 0.0;
    int index = 0;
    long tposX = 0, tposY = 0;
    int xyc = 0;
    int rdcltErrorCounter = 0;
    CString strPosXY;
    struct luamanwalkstate walkstate;
    CStringArray posXY;
    int posX = 0;
    int posY = 0;
    int keyPauseTime = 20;
    CString strCopylefttime;
    CStringArray copylefttime;
    long copylefttimem = 0;
    int key_index = 0;

    Check_Terminate_Thread(luaman_task, dmobj);
    
    walkstate.xNeg = "w";
    walkstate.xPos = "s";
    walkstate.yNeg = "a";
    walkstate.yPos = "d";

    /*if (DIR_U == dir)
    {
        dmobj->MiddleClick();
        LUAMAN_WAIT_MS(1000);
        dmobj->MiddleClick();
        LUAMAN_WAIT_MS(1000);
    }*/
    
    if (DIR_LEFT == dir)
    {
        walkstate.xNeg = "d";
        walkstate.xPos = "a";
        walkstate.yNeg = "w";
        walkstate.yPos = "s";
    }

    tgtNum = SplitString(tgtPosXYs, '|', tgtPosXY);
    if (tgtNum < 0)
    {
        ptr = tgtPosXYs.GetBuffer(BUFFER_MAX);
        api_log_MsgError("·��:����:�Ƿ�:Ѱ·:ʧ��,tgtNum:%d, tgtPosXYs:%s", tgtNum, ptr);
        
        tgtPosXYs.ReleaseBuffer();
        return ERRNO_INVALID;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    lastKeyTimeBlue = time(NULL);
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
    
        //�ɹ����һ����ʱ����
        rdcltErrorCounter = 0;

        //ǰ��Ŀ�������
        for (;;)
        {
        LABEL_MovePosition_read_posxy:
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, "font\\font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                Tz195_ReleaseKey(dmobj, ctxt, &walkstate);
                
                walkstate.yNegKeyDown = 0;
                walkstate.yPosKeyDown = 0;
                walkstate.xPosKeyDown = 0;
                walkstate.xNegKeyDown = 0;

                api_log_MsgWarn("��ͼ:����:��ȡ:ʧ��");
                goto LABEL_MovePosition_read_posxy;
            }

            //��������
            xyc = SplitString(strPosXY, ':', posXY);
            if (xyc < 2)
            {
                ptr = strPosXY.GetBuffer(BUFFER_MAX);
                
                api_log_MsgWarn("��ͼ:����:����:ʧ��,strPosXY:%s, xyc:%d",
                    ptr, xyc);
            
                strPosXY.ReleaseBuffer();

                Tz195_ReleaseKey(dmobj, ctxt, &walkstate);
                
                walkstate.yNegKeyDown = 0;
                walkstate.yPosKeyDown = 0;
                walkstate.xPosKeyDown = 0;
                walkstate.xNegKeyDown = 0;
                
                goto LABEL_MovePosition_read_posxy;
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
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            //�����Ѫ��
            checkTimeBlue = time(NULL);
            deltaTimeBlue = difftime(checkTimeBlue, lastKeyTimeBlue);
            if (deltaTimeBlue > 10.0)
            {
                Tz195_ReleaseKey(dmobj, ctxt, &walkstate);

                if (1 == walkstate.xPosKeyDown)
                {
                    dmobj->KeyUpChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(2 * keyPauseTime);
                    dmobj->KeyDownChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(4 * keyPauseTime);
                    dmobj->KeyUpChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(2 * keyPauseTime);
                }

                if (1 == walkstate.xNegKeyDown)
                {
                    dmobj->KeyUpChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(2 * keyPauseTime);
                    dmobj->KeyDownChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(4 * keyPauseTime);
                    dmobj->KeyUpChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(2 * keyPauseTime);
                }

                if (1 == walkstate.yNegKeyDown)
                {
                    dmobj->KeyUpChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(2 * keyPauseTime);
                    dmobj->KeyDownChar(walkstate.yPos);
                    LUAMAN_WAIT_MS(4 * keyPauseTime);
                    dmobj->KeyUpChar(walkstate.yPos);
                    LUAMAN_WAIT_MS(2 * keyPauseTime);
                }

                if (1 == walkstate.yPosKeyDown)
                {
                    dmobj->KeyUpChar(walkstate.yPos);
                    LUAMAN_WAIT_MS(2 * keyPauseTime);
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(4 * keyPauseTime);
                    dmobj->KeyUpChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(2 * keyPauseTime);
                }
                
                walkstate.yNegKeyDown = 0;
                walkstate.yPosKeyDown = 0;
                walkstate.xPosKeyDown = 0;
                walkstate.xNegKeyDown = 0;

                //Ѱ·����������
                code = Tz195_CheckBlue(dmobj, ctxt);
                if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
                {
                    api_log_MsgWarn("Ѱ·:����:����,tposX:%ld, tposY:%ld, posX:%ld, posY:%ld",
                        tposX, tposY, posX, posY);
                    
                    return code;
                }

                dmobj->SetDict(0, "font\\font-copylefttime.txt");
                strCopylefttime = dmobj->Ocr(15, 172, 183, 194, "ffffff-101010", 0.9);
                dmobj->ClearDict(0);
                if (strCopylefttime != CString(""))
                {
                    //��������ʣ��ʱ�����������
                    xyc = SplitStringWide(strCopylefttime, "��", copylefttime);
                    if (xyc > 1)
                    {
                        copylefttimem = atol(copylefttime.GetAt(0));
                        if (copylefttimem < 5)
                        {
                            ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                            api_log_MsgWarn("Ѱ·:��ס:����:ʣ��ʱ��:%s", ptr);
                            strCopylefttime.ReleaseBuffer();
                            
                            goto LABEL_MovePosition_blocked_posxy;
                        }
                    }
                    else
                    {
                        rdcltErrorCounter = rdcltErrorCounter + 1;
                        if (rdcltErrorCounter > 20)
                        {
                            api_log_MsgWarn("Ѱ·:��ס:����:ʣ��ʱ��:ʶ��:����:20��:%s", ptr);
                            goto LABEL_MovePosition_blocked_posxy;
                        }
                    }
                }
                else
                {
                    rdcltErrorCounter = rdcltErrorCounter + 1;
                    if (rdcltErrorCounter > 20)
                    {
                        code = Tz195_CheckMapStyle(dmobj, ctxt);
                        if (MAP_FORGET_TOWER == code || ERRNO_BLOCK == code)
                        {
                            api_log_MsgWarn("������:Ѱ·:��ס:20��");

                            code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
                            if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
                            {
                                api_log_MsgDebug("������:Ѱ·:��ס:��ʱ");
                                goto LABEL_MovePosition_blocked_posxy;
                            }

                            if (ERRNO_REALIVE == code)
                            {
                                api_log_MsgDebug("������:Ѱ·:��ס:����:����");
                                goto LABEL_MovePosition_blocked_posxy;
                            }
                            
                            rdcltErrorCounter = 0;
                        }

                        Check_Terminate_Thread(luaman_task, dmobj);
                    }
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                lastKeyTimeBlue = time(NULL);
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            //Ѫ��������
            //LABEL_MovePosition_read_posxy
        }

        Tz195_ReleaseKey(dmobj, ctxt, &walkstate);
        walkstate.yNegKeyDown = 0;
        walkstate.yPosKeyDown = 0;
        walkstate.xPosKeyDown = 0;
        walkstate.xNegKeyDown = 0;

        LUAMAN_WAIT_MS(25);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    return ERRNO_SUCCESS;
    
LABEL_MovePosition_blocked_posxy:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        code = Tz195_CheckMapStyle(dmobj, ctxt);
        if (MAP_SEGRILA == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("����ʣ��ʱ��:��ֹ:Ŀ������,tposX:%ld, tposY:%ld",
                tposX, tposY);
            break;
        }

        dmobj->KeyDownChar("u");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("u");
        LUAMAN_WAIT_MS(200);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        for (key_index = 1; key_index < 8; ++key_index)
        {
            dmobj->KeyDown(48 + index);
            LUAMAN_WAIT_MS(50);
            dmobj->KeyUp(48 + index);
            LUAMAN_WAIT_MS(50);
        }

        //���Ѫ��
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:��ס:�ȴ�:����:����:����");
            break;
        }

        if (ERRNO_BLOCK == code)
        {
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (ROLE_M == role)
    {
        ResetGame(groupid, hwnd);
        ResetGroup(groupid);
        Tz195_InviteHeilfellow(dmobj, ctxt, 2);
        Delay_With_Terminate_Check(29, luaman_task, dmobj);
    }
    else
    {
        ResetGame(groupid, hwnd);
        Delay_With_Terminate_Check(2, luaman_task, dmobj);
        
        dmobj->KeyDownChar("1");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("1");
        LUAMAN_WAIT_MS(200);

        Delay_With_Terminate_Check(33, luaman_task, dmobj);
    }

    return ERRNO_BLOCK;
}

int Tz195_Login(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    
    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->MoveTo(109, 104);
    LUAMAN_WAIT_MS(200);
    dmobj->MoveTo(109, 104);
    LUAMAN_WAIT_MS(200);

    dmobj->LeftClick();
    LUAMAN_WAIT_MS(200);

    Check_Terminate_Thread(luaman_task, dmobj);
    LUAMAN_WAIT_MS(2000);
    Check_Terminate_Thread(luaman_task, dmobj);

    dmobj->MoveTo(636, 175);
    LUAMAN_WAIT_MS(200);
    dmobj->MoveTo(636, 175);
    LUAMAN_WAIT_MS(200);
    
    dmobj->LeftDown();
    LUAMAN_WAIT_MS(200);
    dmobj->LeftUp();
    LUAMAN_WAIT_MS(200);
    dmobj->LeftClick();
    LUAMAN_WAIT_MS(200);
    dmobj->LeftClick();
    LUAMAN_WAIT_MS(200);

    LUAMAN_WAIT_MS(800);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->MoveTo(636, 230);
    LUAMAN_WAIT_MS(200);
    dmobj->MoveTo(636, 230);
    LUAMAN_WAIT_MS(200);
    
    LUAMAN_WAIT_MS(1000);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->LeftDown();
    LUAMAN_WAIT_MS(200);
    dmobj->LeftUp();
    LUAMAN_WAIT_MS(200);
    dmobj->LeftClick();
    LUAMAN_WAIT_MS(200);
    dmobj->LeftClick();
    LUAMAN_WAIT_MS(200);

    dmobj->KeyDown(27);
    LUAMAN_WAIT_MS(200);
    dmobj->KeyUp(27);
    LUAMAN_WAIT_MS(200);

    Check_Terminate_Thread(luaman_task, dmobj);
    return ERRNO_SUCCESS;
}

int Tz195_CheckFightSkip(Idmsoft* dmobj, void* ctxt, int location)
{
    int waitTimeSec = 0;
    int leftTimeMin = 0;
    CString strCopylefttime;
    CStringArray copylefttime;
    int count = 0;
    long copylefttimem = 0;
    VARIANT pposX, pposY;
    long lposX = 0, lposY = 0;
    int code = 0;
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    
    if (11 == location)
    {
        waitTimeSec = 80;
        leftTimeMin = 22;
    }

    if (13 == location)
    {
        waitTimeSec = 80;
        leftTimeMin = 19;
    }

    if (15 == location)
    {
        waitTimeSec = 80;
        leftTimeMin = 16;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->SetDict(0, "font\\font-copylefttime.txt");
    strCopylefttime = dmobj->Ocr(15, 172, 183, 194, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strCopylefttime != CString(""))
    {
        count = SplitStringWide(strCopylefttime, "��", copylefttime);
        if (count >= 2)
        {
            copylefttimem = atol(copylefttime.GetAt(0));
            if (copylefttimem < leftTimeMin)
            {
                api_log_MsgDebug("����:����ʣ��ʱ��:����,copylefttimem:%ld, location:%d",
                    copylefttimem, location);
                return 0;
            }

            api_log_MsgDebug("����:����ʣ��ʱ��:����,copylefttimem:%ld, location:%d",
                copylefttimem, location);
        }
        else
        {
            api_log_MsgDebug("����:����ʣ��ʱ��:���:�쳣,location:%d", location);
        }
    }
    else
    {
        api_log_MsgDebug("����:����ʣ��ʱ��:���:ʧ��,location:%d", location);

        dmobj->SetDict(0, "font\\font-waittime.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ȴ�ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����:�ȴ�ʱ��:���:�ɹ�,location:%d, lposX:%ld, lposY:%ld",
                location, lposX, lposY);
            return 0;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    return waitTimeSec;
}

int Tz195_FightAutoStart(Idmsoft* dmobj, void* ctxt)
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

        dmobj->SetDict(0, "font\\font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ʼ", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("ս��:����:��:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("ս��:����:��:ʧ��");
        LUAMAN_WAIT_MS(500);

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("ս��:����:��:����:����");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(500);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(500);

        dmobj->SetDict(0, "font\\font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ʼ", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ս��:����:��ʼ:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
            
        api_log_MsgDebug("ս��:����:��ʼ:ʧ��,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        LUAMAN_WAIT_MS(500);

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("ս��:����:��ʼ:����:����");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    return ERRNO_SUCCESS;
}

int Tz195_FightAutoStop(Idmsoft* dmobj, void* ctxt)
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
    
    return ERRNO_SUCCESS;
}

int Tz195_FightatFixPointSimple(Idmsoft* dmobj, void* ctxt, int location, char* fposXY)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int groupid = luaman_config->groupid;
    int code = 0;
    int fightMaxTime = 0;
    int ufailedMax = 30;
    char* strFontFile = "";
    char* strFontCtxt = "";
    char* strFontColorOffset = "";
    int pGamePosition = 0;
    time_t fightStartTime = 0;
    int uFailedCount = 0;
    time_t curTime = 0;
    double fightTime = 0.0;
    VARIANT pposX, pposY;
    long lposX = 0, lposY = 0;
    int uok = 0;
    int pGroupPosition = 0;
    
    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (ROLE_S0 == role || ROLE_S1 == role)
    {
        if (11 == location || 13 == location || 15 == location)
        {
            fightMaxTime = 90;

            code = Tz195_CheckFightSkip(dmobj, ctxt, location);
            if (0 == code)
            {
                api_log_MsgDebug("ʣ��ʱ�䲻��:����:����,location:%d", location);
                return ERRNO_SUCCESS;
            }

            //bug:��ֵ����ʱ�� 2020/7/8 23:22:19
            fightMaxTime = code;

            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("����:λ��,location:%d, fightMaxTime:%d", location, fightMaxTime);
        }
        
        if (17 == location)
        {
            fightMaxTime = 10;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (!strcmp(fposXY, "369:282"))
    {
        ufailedMax = 35;
    }

    switch (location)
    {
    case 1:
        strFontFile = "font\\font-aa-9-1.txt";
        strFontCtxt = "ͨ�����Ӽ��ҵ�·������";
        strFontColorOffset = "00ff00-101010";
        break;

    case 2:
        strFontFile = "font\\font-aa-20-1.txt";
        strFontCtxt = "ͨ�������㳡��·������";
        strFontColorOffset = "00ff00-101010";
        break;

    case 3:
        strFontFile = "font\\font-aa-32-1.txt";
        strFontCtxt = "ͨ������Ļ�������Ⱦ��ͥԺ��·������";
        strFontColorOffset = "00ff00-101010";
        break;
        
    case 5:
        strFontFile = "font\\font-aa-40-1.txt";
        strFontCtxt = "��Ⱦ��ͥԺ�Ľ��ʯ������";
        strFontColorOffset = "00ff00-101010";
        break;
        
    case 7:
        strFontFile = "font\\font-aa-46-1.txt";
        strFontCtxt = "ͨ�����õ�·������";
        strFontColorOffset = "00ff00-101010";
        break;
        
    case 18:
        strFontFile = "font\\font-aa-61-1.txt";
        strFontCtxt = "����ŵ˹";
        strFontColorOffset = "ff3300-101010";
        break;
        
    case 19:
        strFontFile = "font\\font-aaa-9-1.txt";
        strFontCtxt = "���ص�����ŵ˹��������";
        strFontColorOffset = "00ff00-101010";
        break;
    
    default:
        break;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    code = GetGamePosition(groupid, hwnd, &pGamePosition);
    if (ERRNO_SUCCESS == code)
    {
        if (pGamePosition >= location)
        {
            api_log_MsgDebug("����:���:����:��ͨ��,location:%d, pGamePosition:%d",
                location, pGamePosition);
            return ERRNO_SUCCESS;
        }

        api_log_MsgDebug("����:���:����:δͨ��,location:%d, pGamePosition:%d",
            location, pGamePosition);
    }
    else
    {
        api_log_MsgDebug("����:���:����:��ȡ:ʧ��,location:%d", location);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug: �Զ����֮ǰ��9 2020/1/16 21:57:19 bug:ȡ��11,13,15���ĵȴ� 2021/11/28 09:09:33
    if (1 != location && 11 != location && 13 != location && 15 != location)
    {
        dmobj->KeyDownChar("9");
        LUAMAN_WAIT_MS(250);
        dmobj->KeyUpChar("9");
        LUAMAN_WAIT_MS(250);
        LUAMAN_WAIT_MS(1750);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:���������5�� 2020/2/18 20:25:16
        if (ROLE_M == role && 2 == location)
        {
            LUAMAN_WAIT_MS(3000);
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    code = Tz195_FightAutoStart(dmobj, ctxt);
    if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
    {
        api_log_MsgDebug("����:���:��ʼ:����:����,location:%d, fposXY:%s", location, fposXY);
        return code;
    }

    fightStartTime = time(NULL);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
    //LABEL_Tz_FightatFixPointSimple_start:
        uFailedCount = 0;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            //////////////////////////////////////////////////////////////////////////////////////
            //����:����2����û��,���˾��뿪 2020/2/19 09:15:17
            if (11 == location || 13 == location || 15 == location || 17 == location)
            {
                if (fightMaxTime > 0)
                {
                    curTime = time(NULL);
                    fightTime = difftime(curTime, fightStartTime);
                    if (fightTime >= (fightMaxTime * 1.0))
                    {
                        if (17 == location)
                        {
                            Tz195_FightAutoStop(dmobj, ctxt);
                        }

                        api_log_MsgDebug("���:��ʱ:��ʱ,fightTime:%f, fightMaxTime:%f, location:%d, fposXY:%s",
                            fightTime, fightMaxTime, location, fposXY);
                        return ERRNO_SUCCESS;
                    }
                }
            }

            dmobj->KeyDownChar("u");
            Delay(250);
            dmobj->KeyUpChar("u");
            Delay(250);

            Check_Terminate_Thread(luaman_task, dmobj);
            
            if (18 != location && 19 != location)
            {
                code = dmobj->FindPic(968, 194, 1110, 210, "bmp\\lastblood.bmp", "101010", 0.9, 0, &pposX, &pposY);
                if (-1 != code)
                {
                    uok = 1;

                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("����:U��:�ɹ�,fposXY:%s, lposX:%ld, lposY:%ld",
                        fposXY, lposX, lposY);
                    break;
                }
            }
            else
            {
                dmobj->SetDict(0, strFontFile);
                code = dmobj->FindStrFast(0, 27, 1279, 826, strFontCtxt, strFontColorOffset, 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    uok = 1;

                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("����:U��:�ɹ�,fposXY:%s, lposX:%ld, lposY:%ld",
                        fposXY, lposX, lposY);
                    break;
                }
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            //û��U��,����Ƿ��Ѿ�������
            switch (location)
            {
            case 1:
            case 2:
            case 3:
            case 5:
            case 7:
                dmobj->SetDict(0, strFontFile);
                code = dmobj->FindStrFast(0, 27, 1279, 826, strFontCtxt, strFontColorOffset, 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("����:���:ͨ��:�ɹ�,fposXY:%s, lposX:%ld, lposY:%ld, location:%d",
                        fposXY, lposX, lposY, location);

                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    (void)UpdateGamePosition(groupid, hwnd, location, 0);
                    (void)UpdateGroupPosition(groupid, location, 0);
                    (void)Tz195_FightAutoStop(dmobj, ctxt);

                    Check_Terminate_Thread(luaman_task, dmobj);
                    return ERRNO_SUCCESS;
                }
                break;
            
            default:
                break;
            }

            //��ȡ�����
            code = GetGroupPosition(groupid, &pGroupPosition);
            if (ERRNO_SUCCESS == code)
            {
                api_log_MsgDebug("����:���:�����:��ȡ:�ɹ�,location:%d, pGroupPosition:%d",
                    location, pGroupPosition);

                if (pGroupPosition >= location)
                {
                    (void)Tz195_FightAutoStop(dmobj, ctxt);
                    return ERRNO_SUCCESS;
                }
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            //////////////////////////////////////////////////////////////////////////////////////
            //����:����2����û��,���˾��뿪 2020/1/5 18:48:16
            if (fightMaxTime > 0)
            {
                curTime = time(NULL);
                fightTime = difftime(curTime, fightStartTime);
                if (fightTime >= (fightMaxTime * 1.0))
                {
                    if (17 == location)
                    {
                        (void)Tz195_FightAutoStop(dmobj, ctxt);
                    }

                    api_log_MsgDebug("���:��ʱ:2����:��ʱ,location:%d, fposXY:%s", location, fposXY);
                    return ERRNO_SUCCESS;
                }
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            uFailedCount = uFailedCount + 1;
            if (uFailedCount > ufailedMax)
            {
                api_log_MsgDebug("����:���:����ͨ��,location:%d, fposXY:%s", location, fposXY);

                (void)Tz195_FightAutoStop(dmobj, ctxt);

                Check_Terminate_Thread(luaman_task, dmobj);
                return ERRNO_SUCCESS;
            }

            Delay(250);
            api_log_MsgDebug("����:U��:ʧ��,location:%d, fposXY:%s", location, fposXY);
        }

        //���Ѫ��
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("����:���:����:����,location:%d, fposXY:%s", location, fposXY);

            Check_Terminate_Thread(luaman_task, dmobj);
            (void)Tz195_FightAutoStop(dmobj, ctxt);
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("����:���:�ɹ�,location:%d, fposXY:%s", location, fposXY);
    return ERRNO_SUCCESS;
}

int Tz195_FightatFixPoint(Idmsoft* dmobj, void* ctxt, int location, char* fposXY)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int groupid = luaman_config->groupid;
    int code = 0;
    int ufailedMax = 20;
    char* strFontFile = "";
    char* strFontCtxt = "";
    char* strFontColorOffset = "";
    int pGamePosition = 0;
    int uFailedCount = 0;
    time_t ulastTime = 0;
    VARIANT pposX, pposY;
    long lposX = 0, lposY = 0;
    int uok = 0;
    char* fixPosXY = "";
    int pGroupPosition = 0;
    double udeltaTime = 0;
    time_t curTime = 0;
    CString pathOfPosXYs;
    int uinstance = 0;
    int pauseflag = 0;
    int markflag = 0;
    int cindex = 0;
    int index = 0;
    double deltaAdjustTime = 0;
    time_t lstAdjustTime = time(NULL);
    int kfixCounter = 0;
    int kfixmod = 0;

    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (!strcmp(fposXY, "369:282"))
    {
        ufailedMax = 25;
    }

    switch (location)
    {
    case 1:
        strFontFile = "font\\font-aa-9-1.txt";
        strFontCtxt = "ͨ�����Ӽ��ҵ�·������";
        strFontColorOffset = "00ff00-101010";
        break;

    case 2:
        strFontFile = "font\\font-aa-20-1.txt";
        strFontCtxt = "ͨ�������㳡��·������";
        strFontColorOffset = "00ff00-101010";
        break;

    case 3:
        strFontFile = "font\\font-aa-32-1.txt";
        strFontCtxt = "ͨ������Ļ�������Ⱦ��ͥԺ��·������";
        strFontColorOffset = "00ff00-101010";
        break;
        
    case 5:
        strFontFile = "font\\font-aa-40-1.txt";
        strFontCtxt = "��Ⱦ��ͥԺ�Ľ��ʯ������";
        strFontColorOffset = "00ff00-101010";
        break;
        
    case 7:
        strFontFile = "font\\font-aa-46-1.txt";
        strFontCtxt = "ͨ�����õ�·������";
        strFontColorOffset = "00ff00-101010";
        break;
        
    case 18:
        strFontFile = "font\\font-aa-61-1.txt";
        strFontCtxt = "����ŵ˹";
        strFontColorOffset = "ff3300-101010";
        break;
        
    case 19:
        strFontFile = "font\\font-aaa-9-1.txt";
        strFontCtxt = "���ص�����ŵ˹��������";
        strFontColorOffset = "00ff00-101010";
        break;
    
    default:
        break;
    }

    code = GetGamePosition(groupid, hwnd, &pGamePosition);
    if (ERRNO_SUCCESS == code && pGamePosition >= location)
    {
        api_log_MsgDebug("����:���:����:��ͨ��,location:%d, pGamePosition:%d",
            location, pGamePosition);
        return ERRNO_SUCCESS;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //bug:������֮ǰ��e 2019/12/22 21:23:19
    dmobj->KeyDown(69);
    Delay(200);
    dmobj->KeyUp(69);
    Delay(200);
    ////////////////////////////////////////////////////////////////////////////////

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
    LABEL_Tz_FightatFixPoint_start:
        uFailedCount = 0;
        ulastTime = time(NULL);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            if (18 != location && 19 != location)
            {
                dmobj->KeyDownChar("u");
                Delay(500);
                dmobj->KeyUpChar("u");
                Delay(500);
                code = dmobj->FindPic(968, 194, 1110, 210, "bmp\\lastblood.bmp", "101010", 0.9, 0, &pposX, &pposY);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("����:U��:�ɹ�,fposXY:%s, lposX:%ld, lposY:%ld",
                        fposXY, lposX, lposY);
                    break;
                }
            }
            else
            {
                if (0 == uok)
                {
                    dmobj->KeyDownChar("u");
                    Delay(500);
                    dmobj->KeyUpChar("u");
                    Delay(500);
                    
                    /////////////////////////////////////////////////////////////////////////////////////////////
                    //bug: ��u�߰�3 2020/1/19 21:04:11
                    if (18 == location)
                    {
                        if (!strcmp(fixPosXY, "") || !strcmp(fixPosXY, "244:152") || !strcmp(fixPosXY, "253:143"))
                        {
                            dmobj->KeyDownChar("3");
                            Delay(500);
                            dmobj->KeyUpChar("3");
                            Delay(500);

                            //bug:������7 2022/01/08 15:55:49
                            dmobj->KeyDownChar("7");
                            Delay(500);
                            dmobj->KeyUpChar("7");
                            Delay(500);
                        }
                    }

                    Check_Terminate_Thread(luaman_task, dmobj);
                    /////////////////////////////////////////////////////////////////////////////////////////////
                }

                dmobj->SetDict(0, strFontFile);
                code = dmobj->FindStrFast(0, 27, 1279, 826, strFontCtxt, strFontColorOffset, 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;
                    
                    uok = 1;

                    api_log_MsgDebug("����:U��:�ɹ�,fposXY:%s, lposX:%ld, lposY:%ld",
                        fposXY, lposX, lposY);
                    break;
                }

                /////////////////////////////////////////////////////////////////////////////////////////
                //bug:'���ص�����ŵ˹��������'������ɫ�ļ�� 2020/9/26 10:14:15
                if (!strcmp(strFontCtxt, "���ص�����ŵ˹��������"))
                {
                    dmobj->SetDict(0, strFontFile);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, strFontCtxt, "ffffff-101010", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;
                        
                        uok = 1;

                        api_log_MsgDebug("����:U��:�ɹ�,fposXY:%s, lposX:%ld, lposY:%ld",
                            fposXY, lposX, lposY);
                        break;
                    }
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                
                /////////////////////////////////////////////////////////////////////////////////////////
                //bug:ֻҪ���u�����˾��˳� 2020/1/18 15:43:36
                if (1 == uok)
                {
                    if (18 == location)
                    {
                        api_log_MsgDebug("����:���:ͨ��:�ɹ�,location:%d,fposXY:%s", location, fposXY);
                        
                        (void)UpdateGamePosition(groupid, hwnd, location, 0);
                        (void)UpdateGroupPosition(groupid, location, 0);

                        Check_Terminate_Thread(luaman_task, dmobj);
                        return ERRNO_SUCCESS;
                    }
                }
            }

            if (1 == location || 2 == location || 3 == location || 5 == location || 7 == location || 18 == location)
            {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////
                //bug: �ҵ���ʾ�Ͳ���u��,ֱ���˳� 2020/1/17 21:58:15  
                //bug: ֻ�����½������� 2020/2/29 22:50:16
                if (18 == location)
                {
                    dmobj->SetDict(0, "font\\font-xlnsok.txt");
                    code = dmobj->FindStrFast(2, 462, 385, 616, "�ɹ���������ŵ˹", "00ff00-101010", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                }
                else
                {
                    dmobj->SetDict(0, strFontFile);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, strFontCtxt, strFontColorOffset, 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                }
                if (-1 != code)
                {
                    (void)UpdateGamePosition(groupid, hwnd, location, 0);
                    (void)UpdateGroupPosition(groupid, location, 0);

                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    api_log_MsgDebug("����:���:�����:����:�ɹ�,location:%d", location);
                    return ERRNO_SUCCESS;
                }
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            //��ȡ�����
            code = GetGroupPosition(groupid, &pGroupPosition);
            if (ERRNO_SUCCESS == code && pGroupPosition >= location)
            {
                api_log_MsgDebug("����:���:ͨ��,location:%d, pGroupPosition:%d",
                    location, pGroupPosition);
                return ERRNO_SUCCESS;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            //bug: u�������ȹ�λ 2020/1/17 23:13:22
            curTime = time(NULL);
            udeltaTime = difftime(curTime, ulastTime);
            if (udeltaTime > 2 * 1.0)
            {
                pathOfPosXYs = CString(fposXY);
                code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
                if (ERRNO_SUCCESS != code)
                {
                    if (ERRNO_BLOCK == code || ERRNO_REALIVE == code)
                    {
                        Check_Terminate_Thread(luaman_task, dmobj);
                        
                        api_log_MsgDebug("����:���:��λ:��ס:����,location:%d, code:%d",
                            location, code);
                        return code;
                    }
                }
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            uFailedCount = uFailedCount + 1;
            if (uFailedCount > ufailedMax)
            {
                /////////////////////////////////////////////////////////////////////////////
                //bug:location=1�����û����������,�ߵ�153:412������ 2019/12/10 20:32:19
                if (1 == location)
                {
                    uok = 0;
                    fposXY = "153:412";
                    
                    pathOfPosXYs = CString(fposXY);
                    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
                    if (ERRNO_SUCCESS != code)
                    {
                        if (ERRNO_BLOCK == code || ERRNO_REALIVE == code)
                        {
                            Check_Terminate_Thread(luaman_task, dmobj);
                            
                            api_log_MsgDebug("����:���:��λ:��ס:����,location:%d, code:%d",
                                location, code);
                            return code;
                        }
                    }

                    api_log_MsgDebug("����:���:��λ:����,location:%d", location);
                    goto LABEL_Tz_FightatFixPoint_start;
                }
                else if (18 == location)
                {
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //bug:����u����� 2020/1/16 08:21:44
                    uok = 0;

                    if (0 == uinstance || 1 == uinstance)
                    {
                        if (0 == uinstance)
                        {
                            fixPosXY = "244:152";
                            uinstance = 1;
                        }
                        else
                        {
                            fixPosXY = "253:143";
                            uinstance = 2;
                        }

                        pathOfPosXYs = CString(fixPosXY);
                        code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
                        if (ERRNO_SUCCESS != code)
                        {
                            if (ERRNO_BLOCK == code || ERRNO_REALIVE == code)
                            {
                                Check_Terminate_Thread(luaman_task, dmobj);
                                
                                api_log_MsgDebug("����:���:��λ:��ס:����,location:%d, code:%d, fixPosXY:%s",
                                    location, code, fixPosXY);
                                return code;
                            }
                        }

                        api_log_MsgDebug("����:���:��λ:����,location:%d, fixPosXY:%s",
                            location, fixPosXY);
                        goto LABEL_Tz_FightatFixPoint_start;
                    }
                }

                //��ʱ��u�����п��ܻ�û�д���,�ߵ���һ����֤֮���ٸ��½���
                api_log_MsgDebug("����:���:����ͨ��,location:%d", location);
                Check_Terminate_Thread(luaman_task, dmobj);
                
                return ERRNO_SUCCESS;
            }

            Delay(500);
            api_log_MsgDebug("����:U��:ʧ��,location:%d", location);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //u���˾Ϳ�ʼ�����,��3��ѭ��uһ��
        /////////////////////////////////////////////////////////////////////////////////////
        //bug:����18���븱��17��ͬ��,����ͬ��֮����ܼ����� 2020/6/21 23:03:15
        if (ROLE_M == role && 18 == location && 1 == pauseflag)
        {
            if (0 == markflag)
            {
                (void)FlagMark(groupid, role);
                markflag = 1;
            }

            code = FlagCheck(groupid);
            if (0 == code)
            {
                api_log_MsgDebug("����ŵ˹:ͬ��:����:���");
                pauseflag = 2;
            }

            Delay(1000);
            dmobj->KeyDown(51);
            Delay(50);
            dmobj->KeyUp(51);
            Delay(50);
        }
        else
        {
            for (cindex = 0; cindex < 2; ++cindex)
            {
                //bug:����ŵ˹��ְ�1~8 2022/01/02 21:55:13
                //for (index = 0; index < 7; ++index)
                for (index = 0; index < 8; ++index)
                {
                    dmobj->KeyDown(48 + index + 1);
                    Delay(50);
                    dmobj->KeyUp(48 + index + 1);
                    Delay(50);
                }
            }

            /////////////////////////////////////////////////////////////////////////////////////
            //bug:���Ѫ���Ƿ�ﵽ��ͣ״̬ 2020/6/21 22:58
            if (ROLE_M == role && 18 == location && 0 == pauseflag)
            {
                code = dmobj->FindPic(968, 194, 1110, 210, "bmp\\fightpause.bmp", "101010", 0.9, 0, &pposX, &pposY);
                if (-1 == code)
                {
                    //lposX = pposX.lVal;
                    //lposY = pposY.lVal;
                    
                    pauseflag = 1;

                    api_log_MsgDebug("����:���Ĺ�:��ͣ:����");
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //��λ���
        curTime = time(NULL);
        deltaAdjustTime = difftime(curTime, lstAdjustTime);
        if (deltaAdjustTime > 40 * 1.0)
        {
            pathOfPosXYs = CString(fposXY);
            code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
            if (ERRNO_SUCCESS != code)
            {
                if (ERRNO_BLOCK == code || ERRNO_REALIVE == code)
                {
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    api_log_MsgDebug("����:���:��λ:��ס:����,location:%d, code:%d, fposXY:%s",
                        location, code, fposXY);
                    return code;
                }
            }

            api_log_MsgDebug("����:���:��λ:����,location:%d, fposXY:%s",
                location, fposXY);

            //////////////////////////////////////////////////////////////////////////////////
            //bug: ÿ��λ���ξͰ���9 2020/1/17 23:20:35
            kfixCounter = kfixCounter + 1;
            kfixmod = kfixCounter % 5;
            if (0 == kfixmod)
            {
                dmobj->KeyPressChar("9");
                Delay(200);
                dmobj->KeyUpChar("9");
                Delay(200);
                Delay(800);
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            lstAdjustTime = time(NULL);
        }

        //���Ѫ��
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("����:���:����:����,fposXY:%s,location:%d", fposXY, location);
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    return ERRNO_SUCCESS;
}

static int Tz195_ExitGroup(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int groupid = luaman_config->groupid;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, "font\\font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�˳�����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + 5, lposY + 5);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, "font\\font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("ȷ��:����:�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, "font\\font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:���:ʧ��,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
    }
    
    return ERRNO_SUCCESS;
}

static int Tz195_MasterMainLine(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int code = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    int instance = 0;
    int signin = 0;
    char* fposXY = NULL;
    int location = 0;
    int npc1_retry_count = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int npc2_retry_count = 0;
    int npc3_retry_count = 0;
    int pGroupPosition = 0;
    int npc4_retry_count = 0;
    int npc5_retry_count = 0;
    int key_index = 0;
    int index = 0;
    int ticketin = 1;
        
    api_log_MsgDebug("handle task195 master, hwnd:%ld", hwnd);

LABEL_Tz_MasterMain_enter_copy_invite:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //��ȥ����� �������,��!�����쳣������½��벻��� 2020/5/26 22:40:16
    //////////////////////////////////////////////////////////////////////////////
    //bug:�ٷ������汾��,����3���˲��ܽ� 2020/5/28 21:37:19
    Tz195_InviteHeilfellow(dmobj, ctxt, 2);
    
LABEL_Tz_MasterMain_enter_copy:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    pathofposxys = "255:133|250:135";
    pathOfPosXYs = CString(pathofposxys);
    Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    
LABEL_Tz_MasterMain_enter_copy_without_walk:
    //��N�Ի����븱��
    Check_Terminate_Thread(luaman_task, dmobj);

    ////////////////////////////////////////////////////////////////////////////
    //bug:���븱��ǰ�����볡ȯ 2022/06/11 22:31:10
    if (ticketin)
    {
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->KeyPressChar("n");
            Delay(300);
            dmobj->KeyUpChar("n");
            Delay(300);
            dmobj->MoveTo(5, 5);
            Delay(300);

            dmobj->SetDict(0, FONT195_DIR"font-npc100001.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "[�����Ŀ������]�볡ȯ����", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("[�����Ŀ������]�볡ȯ����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            code = Tz195_CheckBlueOut(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("[�����Ŀ������]�볡ȯ����:����:����:����");
                goto LABEL_Tz_MasterMain_enter_copy_without_walk;
            }

            api_log_MsgDebug("[�����Ŀ������]�볡ȯ����:����:ʧ��");
            Delay(500);
        }

        //���
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX, lposY);
            Delay(300);
            dmobj->LeftClick();
            Delay(300);
            dmobj->MoveTo(5, 5);
            Delay(300);

            dmobj->SetDict(0, FONT195_DIR"font-npc100001.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "�����Ŀ�������볡ȯ����", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                
                api_log_MsgDebug("�����Ŀ�������볡ȯ����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }

            dmobj->SetDict(0, FONT195_DIR"font-npc100002.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                
                api_log_MsgDebug("����������:����:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

                //�ص���������
                for (;;)
                {
                    Check_Terminate_Thread(luaman_task, dmobj);
            
                    dmobj->MoveTo(lposX2, lposY2);
                    Delay(300);
                    dmobj->LeftClick();
                    Delay(300);
                    dmobj->MoveTo(5, 5);
                    Delay(300);

                    dmobj->SetDict(0, FONT195_DIR"font-npc100002.txt");
                    code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("����������:���:�ɹ�");

                        (void)Tz195_ExitGroup(dmobj, ctxt);
                        break;
                    }

                    lposX = pposX.lVal;
                    lposY = pposY.lVal;
                    
                    api_log_MsgDebug("����������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
                }
                
                return ERRNO_EXIT;
            }
            
            code = Tz195_CheckBlueOut(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("�����Ŀ�������볡ȯ����:����:����:����");
                goto LABEL_Tz_MasterMain_enter_copy_without_walk;
            }

            api_log_MsgDebug("�����Ŀ�������볡ȯ����:����:ʧ��");
            Delay(500);
        }

        //���
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX2, lposY2);
            Delay(300);
            dmobj->LeftClick();
            Delay(300);
            dmobj->MoveTo(5, 5);
            Delay(300);

            dmobj->SetDict(0, FONT195_DIR"font-npc100002.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("����������:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
                return ERRNO_EXIT;
            }
            
            dmobj->SetDict(0, FONT195_DIR"font-npc100001.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "�����Ŀ�������볡ȯ����", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�����Ŀ�������볡ȯ����:���:�ɹ�");
                break;
            }

            code = Tz195_CheckBlueOut(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("�����Ŀ�������볡ȯ����:���:����:����");
                goto LABEL_Tz_MasterMain_enter_copy_without_walk;
            }

            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�����Ŀ�������볡ȯ����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            Delay(500);
        }

        ticketin = 0;
    }
    ////////////////////////////////////////////////////////////////////////////
    
    for (;;)
    {
        dmobj->KeyPressChar("n");
        LUAMAN_WAIT_MS(300);
        dmobj->KeyUpChar("n");
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, "font\\font-aa-2-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����Ŀ������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�����Ŀ������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("�����Ŀ������:����:����:����");
            //goto LABEL_Tz_MasterMain_enter_copy;
            goto LABEL_Tz_MasterMain_enter_copy_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;

            api_log_MsgDebug("�����Ŀ������:����:����:����:��������:������һ����");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�����Ŀ������:����:ʧ��");
        LUAMAN_WAIT_MS(500);
    }

    //���"�����Ŀ������"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, "font\\font-enter-ack.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("�����Ŀ������:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("�����Ŀ������:���:����:����");
            //goto LABEL_Tz_MasterMain_enter_copy;
            goto LABEL_Tz_MasterMain_enter_copy_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;

            api_log_MsgDebug("�����Ŀ������:���:����:����:��������:������һ����");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�����Ŀ������:���:ʧ��");
        LUAMAN_WAIT_MS(500);
    }

    //���"ȷ��"���븱��
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        code = Tz195_CheckMapStyle(dmobj, ctxt);
        if (MAP_FORGET_TOWER == code)
        {
            api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:���븱��");
            break;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("�����Ŀ������:ȷ��:���:����:����");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;

            api_log_MsgDebug("�����Ŀ������:ȷ��:���:����:����");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //��鸱�����Ƿ��д�boss,�����������"����޷��볡"
        dmobj->SetDict(0, "font\\font-reenter.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����޷��볡��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:�������д�boss����ȥ, lposX:%ld, lposY:%ld", lposX, lposY);

            //����"�������д�boss����ȥ:ȷ��"����
            for (;;)
            {
                dmobj->SetDict(0, "font\\font-reenter.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX3 = pposX3.lVal;
                    lposY3 = pposY3.lVal;

                    api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:�ɹ�, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
                    break;
                }

                //����Ƿ��������
                code = Tz195_CheckBlueOut(dmobj, ctxt);
                if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:����:����");
                    //goto LABEL_Tz_MasterMain_enter_copy;
                    goto LABEL_Tz_MasterMain_enter_copy_without_walk;
                }

                if (ERRNO_BLOCK == code)
                {
                    instance = 0;

                    api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:����:����:��������:������һ����");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                LUAMAN_WAIT_MS(500);
                api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:ʧ��");
            }

            //���"�������д�boss����ȥ:ȷ��"����
            for (;;)
            {
                dmobj->MoveTo(lposX3, lposY3);
                LUAMAN_WAIT_MS(300);
                dmobj->LeftClick();
                LUAMAN_WAIT_MS(300);
                dmobj->MoveTo(5, 5);
                LUAMAN_WAIT_MS(300);

                dmobj->SetDict(0, "font\\font-reenter.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("�������д�boss����ȥ:ȷ��:���:�ɹ�");
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                LUAMAN_WAIT_MS(500);
                api_log_MsgDebug("�������д�boss����ȥ:ȷ��:���:ʧ��");
            }

            api_log_MsgDebug("�������д�boss����ȥ:ȷ��:���:�ɹ�:���¶Ի�");
            goto LABEL_Tz_MasterMain_enter_copy_without_walk;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //û�н��븱��,Ҳû�г���"�������д�boss����ȥ",�����Ƿ���"�������³���"
        dmobj->SetDict(0, "font\\font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:�������³�����,lposX:%ld, lposY:%ld",
                lposX, lposY);

            for (;;)
            {
                dmobj->MoveTo(lposX + 55, lposY + 100);
                LUAMAN_WAIT_MS(500);
                dmobj->LeftClick();
                LUAMAN_WAIT_MS(500);
                dmobj->MoveTo(5, 5);
                LUAMAN_WAIT_MS(300);

                dmobj->SetDict(0, "font\\font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    lposX3 = pposX3.lVal;
                    lposY3 = pposY3.lVal;
                    
                    api_log_MsgDebug("�������³�����:ȷ��:���:�ɹ�,lposX3:%ld, lposY3:%ld",
                        lposX3, lposY3);
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("�������³�����:ȷ��:���:ʧ��");
                LUAMAN_WAIT_MS(500);
            }

            api_log_MsgDebug("�������³�����:ȷ��:���:�ɹ�:���¶Ի�");
            goto LABEL_Tz_MasterMain_enter_copy_without_walk;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:���"��Ա����" 2020/12/24 22:10:55
        dmobj->SetDict(0, "font\\font-peopleleak.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ա����", "ffffff-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�������³�����:ȷ��:��Ա����:����:�ɹ�,lposX:%ld, lposY:%d",
                lposX, lposY);

            for (;;)
            {
                dmobj->MoveTo(lposX + 32, lposY + 104);
                LUAMAN_WAIT_MS(300);
                dmobj->LeftClick();
                LUAMAN_WAIT_MS(500);
                dmobj->MoveTo(5, 5);
                LUAMAN_WAIT_MS(500);

                dmobj->SetDict(0, "font\\font-peopleleak.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ա����", "ffffff-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("�������³�����:ȷ��:��Ա����:���:�ɹ�:�������");

                    Tz195_InviteHeilfellow(dmobj, ctxt, 2);
                    goto LABEL_Tz_MasterMain_enter_copy_without_walk;
                }

                //����Ƿ��������
                code = Tz195_CheckBlueOut(dmobj, ctxt);
                if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("�������³�����:ȷ��:��Ա����:���:����:����:����");
                    goto LABEL_Tz_MasterMain_enter_copy_invite;
                }

                if (ERRNO_BLOCK == code)
                {
                    instance = 0;

                    api_log_MsgDebug("�������³�����:ȷ��:��Ա����:���:����:����:����:��������:������һ����");
                    goto LABEL_Tz_MasterMain_enter_copy_invite;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("�������³�����:ȷ��:��Ա����:���:ʧ��");
                LUAMAN_WAIT_MS(500);
            }
        }//"��Ա����"

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:����:Ŭ��������");
        LUAMAN_WAIT_MS(500);

        //bug:����Ѫ����� 2022/02/21 21:36
        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����:����:ȷ��:����:����:����");
            //goto LABEL_Tz_MasterMain_enter_copy;
            goto LABEL_Tz_MasterMain_enter_copy_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;
            
            api_log_MsgDebug("����:����:ȷ��:����:����:����");
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    //�ɹ����븱��,��״̬
    dmobj->KeyDownChar("0");
    LUAMAN_WAIT_MS(500);
    dmobj->KeyUpChar("0");
    LUAMAN_WAIT_MS(2000);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyDownChar("-");
    LUAMAN_WAIT_MS(500);
    dmobj->KeyUpChar("-");
    LUAMAN_WAIT_MS(2500);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyDownChar("=");
    LUAMAN_WAIT_MS(500);
    dmobj->KeyUpChar("=");
    LUAMAN_WAIT_MS(2500);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (0 == instance)
    {
        instance = 1;
    }
    else
    {
        ////////////////////////////////////////////////////////////
        //bug:�Ƶ��������� 2022/05/15 21:07:22
        /*
        dmobj->KeyDownChar("t");
        LUAMAN_WAIT_MS(500);
        dmobj->KeyUpChar("t");
        LUAMAN_WAIT_MS(2500);
        */

        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
        if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("�����Ŀ������:��ʱ");
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("�����Ŀ������:��ʱ:�ж�:���:����:����");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    if (0 == signin)
    {
        (void)Tz195_Login(dmobj, ctxt);
        signin = 1;

        Delay_With_Terminate_Check(5, luaman_task, dmobj);
    }

    //ǰ������107:415
    pathofposxys = "107:415";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�ȴ�,������"��������ʱ��"ʱ��ʼ����
    for (;;)
    {
        dmobj->SetDict(0, "font\\font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������ʱ��", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������ʱ��:����:�ɹ�,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("��������ʱ��:����:ʧ��");
        LUAMAN_WAIT_MS(1000);
    }

    pathofposxys = "107:415|145:414|160:411";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

LABEL_Tz_MasterMain_battle_place_1:
    fposXY = "160:411";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    //���������,����ǰ����һ��
    pathofposxys = "154:412|155:428|171:428|189:428|197:421";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    code = MarkReachFlag(groupid, hwnd, 1);
    if (ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("ͬ��:1:ʧ��,code:%d", code);
    }

    //����Ƿ񼯽����
    for (;;)
    {
        code = CheckSyncFlag(groupid, 1);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("ͬ��:1:���,code:%d", code);
            break;
        }

        api_log_MsgDebug("ͬ��:1:�ȴ�");
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
    }

    goto LABEL_Tz_MasterMain_battle_place_1_x2;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:�������� 2020/1/21 10:56:55
LABEL_Tz_MasterMain_battle_place_1_x1:
    pathofposxys = "197:421|191:429|176:430|166:429|154:429|152:415|135:415";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    //������
    fposXY = "135:415";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    //������
    fposXY = "122:414";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    //������
    fposXY = "106:415";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    pathofposxys = "106:415|122:414|135:415|152:415|154:429|166:429|176:430|191:429|197:421";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //������һ��,ͬ���ٽ��жԻ�,����ͬ����־
    code = MarkReachFlag(groupid, hwnd, 1);
    if (ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("ͬ��:1:ʧ��,code:%d", code);
    }

    //����Ƿ񼯽����
    for (;;)
    {
        code = CheckSyncFlag(groupid, 1);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("ͬ��:1:���,code:%d", code);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("ͬ��:1:�ȴ�");
        Delay(1000);
    }

LABEL_Tz_MasterMain_battle_place_1_x2:
    //�������,��ʼ�Ի�
    dmobj->MoveTo(5, 5);
    Delay(300);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    npc1_retry_count = 0;

    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-15-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ������Ӽ���", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ������Ӽ���:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        npc1_retry_count = npc1_retry_count + 1;
        if (npc1_retry_count > 5)
        {
            api_log_MsgDebug("�ƶ������Ӽ���:����:��ʱ");

            //�����ǵ�һ�ػ�û�д��,���µ���ȥ��(���ͬ����־)
            code = ClearMarkReachFlag(groupid, hwnd, 1);
            if (ERRNO_SUCCESS != code)
            {
                api_log_MsgDebug("NPC1:�Ի�:ʧ��:���:ͬ��:�ص�1:ʧ��,code:%d", code);
            }

            api_log_MsgDebug("NPC1:�Ի�:ʧ��:����:����:���:�ص�1");
            goto LABEL_Tz_MasterMain_battle_place_1_x1;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�ƶ������Ӽ���:����:ʧ��");
        Delay(500);
    }

    //���"�ƶ������Ӽ���"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-15-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ������Ӽ���", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ������Ӽ���:���:�ɹ�");
            break;
        }

        //TODO: ���Ѫ��

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�ƶ������Ӽ���:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //��鵱ǰ����,������겻��254:415������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        dmobj->SetDict(0, "font\\font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 254) <= 1 && abs(posY - 415) <= 1)
                {
                    api_log_MsgDebug("�ƶ������Ӽ���:�Ի�:�ɹ�");
                    break;
                }
                else if (abs(posX - 197) <= 1 && abs(posY - 421) <= 1)
                {
                    //���ͬ����־
                    (void)ClearMarkReachFlag(groupid, hwnd, 1);

                    api_log_MsgDebug("�ƶ������Ӽ���:�Ի�:ʧ��:����:����:���:�ص�1");
                    goto LABEL_Tz_MasterMain_battle_place_1_x1;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�ƶ������Ӽ���:�Ի�:����:��ȡ:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�����Լ��Ľ���
    location = 1;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //��һ�ش����,����ڶ���
    pathofposxys = "272:411|286:406|292:402";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);
    
LABEL_Tz_MasterMain_battle_place_2:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //////////////////////////////////////////////////////////////////////////////////////
    //bug:�ߵ���������ȥ�� 2020/2/22 14:12:22
    dmobj->KeyPressChar("a");
    Delay(50);
    dmobj->KeyPressChar("w");
    Delay(200);
    dmobj->KeyUpChar("a");
    Delay(50);
    dmobj->KeyUpChar("w");
    Delay(200);

    //////////////////////////////////////////////////////////////////////////////////////
    //bug:��λ��ʱ�򾭹�288:405�ٹ�λ,���ⱻ���� 2019/12/10 20:35:49
    //������
    fposXY = "288:405|292:402";
    location = 2;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    //Ѱ·"285:407|286:425|301:427|330:427|348:427|357:429"
    pathofposxys = "285:407|286:425|301:427|330:427|348:427|357:429";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //��ڶ���npc�Ի�
    dmobj->MoveTo(5, 5);
    Delay(300);
        
    npc2_retry_count = 0;
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
        
        dmobj->SetDict(0, "font\\font-aa-27-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ��������㳡", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ��������㳡:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        npc2_retry_count = npc2_retry_count + 1;
        if (npc2_retry_count > 5)
        {
            api_log_MsgDebug("�ƶ��������㳡:����:��ʱ:NPC2:�Ի�:ʧ��:����:����:���:�ص�2");
            goto LABEL_Tz_MasterMain_battle_place_2;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�ƶ��������㳡:����:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //���"�ƶ��������㳡"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //bug:�ڲ����ҵ��������ҵ��˵���һֱ��,�ߵ�����������,������һ��ѭ����ѭ�� 2022/02/23 22:22:22
        dmobj->SetDict(0, "font\\font-aa-27-1.txt");
        //code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ��������㳡", "00ff00-101010", 0.9, &pposX2, &pposY2);
        code = dmobj->FindStrFast(380, 27, 1279, 826, "�ƶ��������㳡", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ��������㳡:���:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("�ƶ��������㳡:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    //��鵱ǰ����,������겻��374:428������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        dmobj->SetDict(0, "font\\font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 374) <= 1 && abs(posY - 428) <= 1)
                {
                    api_log_MsgDebug("�ƶ��������㳡:�Ի�:�ɹ�");
                    break;
                }
                else if (abs(posX - 357) <= 1 && abs(posY - 429) <= 1)
                {
                    api_log_MsgDebug("�ƶ��������㳡:�Ի�:ʧ��:����:����:���:�ص�2");
                    goto LABEL_Tz_MasterMain_battle_place_1_x1;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("�ƶ��������㳡:�Ի�:����:��ȡ:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�����Լ��Ľ���
    location = 2;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //�ڶ��ش����,���������
    //Ѱ·"380:419|387:411"
    pathofposxys = "380:419|387:411";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

LABEL_Tz_MasterMain_battle_place_3:
    //������
    fposXY = "387:411";
    location = 3;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    //Ѱ·"380:397|378:392"
    pathofposxys = "380:397|378:392";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    //�������npc�Ի�
    dmobj->MoveTo(5, 5);
    Delay(300);
        
    npc3_retry_count = 0;
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-34-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�����һ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ�����һ����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        npc3_retry_count = npc3_retry_count + 1;
        if (npc3_retry_count > 5)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            ///////////////////////////////////////////////////////////////////////////////////
            //bug:����9, �������´� 2020/1/19 20:58:19
            //goto LABEL_Tz_MasterMain_battle_place_3
            dmobj->KeyPressChar("9");
            Delay(300);
            dmobj->KeyUpChar("9");
            Delay(300);
            Delay(1700);
            npc3_retry_count = 0;

            api_log_MsgDebug("�ƶ�����һ����:����:��ʱ:NPC3:�Ի�:ʧ��:����:����:���:�ص�3");
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�ƶ�����һ����:����:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //���"�ƶ�����һ����"
    npc3_retry_count = 0;
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-35-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�����Ⱦ��ͥԺ", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        npc3_retry_count = npc3_retry_count + 1;
        if (npc3_retry_count > 5)
        {
            api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:����:��ʱ:NPC3:�Ի�:ʧ��:����:����:���:�ص�3");
            goto LABEL_Tz_MasterMain_battle_place_3;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:����:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //���"�ƶ��������㳡"
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);
        
        dmobj->SetDict(0, "font\\font-aa-27-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�����Ⱦ��ͥԺ", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:���:�ɹ�");
            break;
        }


        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //��鵱ǰ����,������겻��386:312������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        dmobj->SetDict(0, "font\\font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 386) <= 1 && abs(posY - 312) <= 1)
                {
                    api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:�Ի�:�ɹ�");
                    break;
                }
                else if (abs(posX - 375) <= 1 && abs(posY - 388) <= 1)
                {
                    api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:�Ի�:ʧ��:����:����:���:�ص�2");
                    goto LABEL_Tz_MasterMain_battle_place_3;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:�Ի�:����:��ȡ:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�����Լ��Ľ���
    location = 3;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //�����ش����,������Ĺ�
LABEL_Tz_MasterMain_battle_place_4:
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //���Ĺؿ����Ѿ������,������岽
    //Ѱ·"386:312|377:297|369:282"
    pathofposxys = "386:312|377:297|369:282";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

LABEL_Tz_MasterMain_battle_place_5:
    //������
    fposXY = "369:282";
    location = 5;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //��ȡ����,��ȷ���Ƿ�������
    location = 5;
    code = GetGroupPosition(groupid, &pGroupPosition);
    if (ERRNO_SUCCESS == code && pGroupPosition >= location)
    {
        pathofposxys = "373:261|381:246|384:226|383:213";
        api_log_MsgDebug("���岽:����:���:�����:��ȡ:�ɹ�:��6��:����,pGroupPosition:%d", pGroupPosition);
    }
    else
    {
        api_log_MsgDebug("��6��:����:���:׼��,code:%d, pGroupPosition:%d", code, pGroupPosition);

        //��6��
        //Ѱ·���˶�����
        pathofposxys = "373:261|381:246";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
        if (ERRNO_SUCCESS != code)
        {
            if (ERRNO_BLOCK == code)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                api_log_MsgDebug("������|Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
                return ERRNO_SUCCESS;
            }

            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("������|Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
                goto LABEL_Tz_MasterMain_enter_copy;
            }
        }

        api_log_MsgDebug("������|Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

        //������
        fposXY = "381:246";
        location = 6;
        code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
        if (ERRNO_SUCCESS != code)
        {
            if (ERRNO_BLOCK == code)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                api_log_MsgDebug("������|������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
                return ERRNO_SUCCESS;
            }

            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("������|������:����:����:����:����:����,fposXY:%s", fposXY);
                goto LABEL_Tz_MasterMain_enter_copy;
            }
        }

        pathofposxys = "384:226|383:213";
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //��4~6�ؿ����Ѿ������,������߹�
    //Ѱ·����383:213
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //������һ��,ͬ���ٽ��жԻ�,����ͬ����־
    (void)MarkReachFlag(groupid, hwnd, 7);

    //����Ƿ񼯽����
    for (;;)
    {
        code = CheckSyncFlag(groupid, 7);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("ͬ��:7:���,code:%d", code);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("ͬ��:7:�ȴ�");
        Delay(1000);
    }

    //�������,��״̬
    dmobj->KeyDownChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay_With_Terminate_Check(2, luaman_task, dmobj);
    
    dmobj->KeyDownChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dmobj->KeyDownChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay_With_Terminate_Check(3,luaman_task, dmobj);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:���Ű�1,���Ű�8 2020/2/18 20:14:11
    if (ROLE_M == role)
    {
        Delay(500);
        dmobj->KeyDownChar("8");
        Delay(500);
        dmobj->KeyUpChar("8");
        Delay(500);
    }
    else
    {
        Delay(500);
        dmobj->KeyDownChar("1");
        Delay(500);
        dmobj->KeyUpChar("1");
        Delay(500);
    }

    //��U�ҵ����ʯ:�������ʯ
LABEL_Tz_MasterMain_battle_place_7:
    //������
    fposXY = "383:213";
    location = 7;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�����Լ��Ľ���
    location = 7;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    //���������
    (void)UpdateGroupPosition(groupid, location, 0);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:��286:113����3����� 2020/1/21 09:44:38
    //Ѱ· "383:213|368:210|352:206|330:196|318:185|313:170|309:158|301:141||293:127|286:113|263:111|244:112"
    pathofposxys = "383:213|368:210|352:206|330:196|318:185|313:170|309:158|301:141|293:127|286:113";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    dmobj->KeyPressChar("3");
    Delay(300);
    dmobj->KeyUpChar("3");
    Delay(300);

    pathofposxys = "263:111|244:112";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

LABEL_Tz_MasterMain_npc_place_8:
    dmobj->MoveTo(5, 5);
    Delay(300);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    npc4_retry_count = 0;
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
    
        dmobj->SetDict(0, "font\\font-aa-59-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("��������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��������:����:ʧ��");

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("��������:�Ի�:����:����:����:����:����");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;

            api_log_MsgDebug("��������:�Ի�:����:����:��������:������һ����");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-59-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("��������:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("��������:���:ʧ��");

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("��������:�Ի�:����:����:����:����:����");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;

            api_log_MsgDebug("��������:�Ի�:����:����:����:����:����:��������");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    //��鵱ǰ����,������겻�� 244:151 ������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        dmobj->SetDict(0, "font\\font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 244) <= 1 && abs(posY - 151) <= 1)
                {
                    api_log_MsgDebug("��������:�Ի�:�ɹ�");
                    break;
                }
                else if (abs(posX - 244) <= 1 && abs(posY - 113) <= 1)
                {
                    api_log_MsgDebug("��������:�Ի�:ʧ��:����:����:���:�ص�2");
                    goto LABEL_Tz_MasterMain_npc_place_8;
                }
            }
        }

        api_log_MsgDebug("��������:�Ի�:����:��ȡ:ʧ��");

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("��������:�Ի�:����:����:����:����:����");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;

            api_log_MsgDebug("��������:�Ի�:����:����:����:����:����:��������");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    //��u��'����ŵ˹'
LABEL_Tz_MasterMain_battle_place_9:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:��λ��255:149�����ٴ� 2020/1/11 13:09:25
    pathofposxys = "255:149";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //������
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug: ��BOSS֮ǰ��e 2020/1/14 21:45:42
    dmobj->KeyPressChar("e");
    Delay(300);
    dmobj->KeyUpChar("e");
    Delay(300);

    location = 18;
    fposXY = "255:149";
    code = Tz195_FightatFixPoint(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    Delay_With_Terminate_Check(4, luaman_task, dmobj);
    
    dmobj->KeyPressChar("8");
    Delay(500);
    dmobj->KeyUpChar("8");
    Delay(500);

    Delay_With_Terminate_Check(16, luaman_task, dmobj);

    pathofposxys = "244:151|245:171";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    //��ʼ�Ի�
    dmobj->MoveTo(5, 5);
    Delay(300);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    npc5_retry_count = 0;
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, "font\\font-aaa-4-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�Ի�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ������������鷿:�Ի�:����:�ɹ�, lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        npc5_retry_count = npc5_retry_count + 1;
        if (npc5_retry_count > 5)
        {
            api_log_MsgDebug("�ƶ������������鷿:�Ի�:����:��ʱ:NPC5:�Ի�:ʧ��:����:����:���:�ص�1");

            Check_Terminate_Thread(luaman_task, dmobj);
            
            if (0 != instance)
            {
                code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
                if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
                {
                    instance = 0;

                    api_log_MsgDebug("�ƶ������������鷿:�Ի�:��ʱ");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }
                else if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("�ƶ������������鷿:�Ի�:��ʱ:����:����");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�ƶ������������鷿:�Ի�:����:ʧ��");
        Delay(500);
    }
    
    npc5_retry_count = 0;

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //���"�Ի�"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aaa-4-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ��������������鷿", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("�ƶ��������������鷿:����:�ɹ�,lposX2:%ld, lposY2:%ld",
                lposX2, lposY2);
            break;
        }

        npc5_retry_count = npc5_retry_count + 1;
        if (npc5_retry_count > 5)
        {
            /////////////////////////////////////////////////////////////////////////////////////////////
            //bug:�Ի����ڻ���,��Ҫ��esc�ص� 2020/2/20 19:23:53
            dmobj->KeyPressChar("esc");
            Delay(300);
            dmobj->KeyUpChar("esc");
            Delay(300);
            /////////////////////////////////////////////////////////////////////////////////////////////

            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("�ƶ������������鷿:����:��ʱ:NPC5:�ƶ������������鷿:����:ʧ��:����:����:���:�ص�1");

            //bug:��������ʣ��ʱ���ж� 2020/2/18 21:17:19
            if (0 != instance)
            {
                code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
                if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
                {
                    instance = 0;

                    api_log_MsgDebug("�ƶ������������鷿:����:��ʱ");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }
                else if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("�ƶ������������鷿:����:��ʱ:����:����");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�ƶ��������������鷿:����:ʧ��");
        Delay(500);
    }

    npc5_retry_count = 0;
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //���"�ƶ��������������鷿"
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aaa-4-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ��������������鷿", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ��������������鷿:���:�ɹ�");
            break;
        }

        npc5_retry_count = npc5_retry_count + 1;
        if (npc5_retry_count > 5)
        {
            if (0 != instance)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
                if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
                {
                    instance = 0;

                    api_log_MsgDebug("�ƶ������������鷿:���:��ʱ");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }
                else if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("�ƶ������������鷿:���:��ʱ:����:����");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        Delay(500);
        api_log_MsgDebug("�ƶ��������������鷿:���:ʧ��");
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //��鵱ǰ����,������겻��234:220������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        dmobj->SetDict(0, "font\\font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 234) <= 1 && abs(posY - 220) <= 1)
                {
                    api_log_MsgDebug("�ƶ��������������鷿:�Ի�:�ɹ�");
                    break;
                }
                else if (abs(posX - 245) <= 1 && abs(posY - 171) <= 1)
                {
                    api_log_MsgDebug("�ƶ��������������鷿:�Ի�:ʧ��");
                    goto LABEL_Tz_MasterMain_battle_place_9;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�ƶ������Ӽ���:�Ի�:����:��ȡ:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�����Լ��Ľ���
    location = 18;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);
    
    dmobj->MoveTo(5, 5);
    Delay(300);

    /*
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aaa-7-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�Ի�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�Ի�:����:�ɹ�, lposX:%ld, lposY:%d", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�Ի�:����:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //���"�Ի�"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aaa-7-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ�Ͻ���", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("ȷ�Ͻ���:����:�ɹ�, lposX2:%ld, lposY2:%d", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("ȷ�Ͻ���:����:ʧ��");
        Delay(500);
        
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aaa-7-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ�Ͻ���", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ�Ͻ���:���:�ɹ�");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("ȷ�Ͻ���:���:ʧ��");
        Delay(500);
    }
    */

LABEL_Tz_MasterMain_battle_place_10:
    /////////////////////////////////////////////////////////////////////////////////////////
    /*
    location = 19;
    fposXY = "234:220";
    code = Tz195_FightatFixPoint(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }
    */
    /////////////////////////////////////////////////////////////////////////////////////////
    //bug:2022/05/26 23:07:11
    for (;;)
    {
        dmobj->SetDict(0, "font\\195\\font-npc1000.txt");
        //code = dmobj->FindStrFast(0, 27, 1279, 826, "���ص�����ŵ˹��������", "ffff00-101010", 0.9, &pposX, &pposY);
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("���ص�����ŵ˹��������:����:�ɹ�, lposX:%ld, lposY:%d", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("���ص�����ŵ˹��������:����:ʧ��");
        Delay(500);
    }

    //������� 573:429 ->611:467~611:502~670:466~670:499
    //573+38/97, 429+38/70
    lposX = lposX + 38 + time(NULL) % 50;
    lposY = lposY + 38 + time(NULL) % 25;

    api_log_MsgDebug("����:���:���:λ��:lposX:%ld, lposY:%ld", lposX, lposY);
    
    dmobj->MoveTo(lposX, lposY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);

    Delay_With_Terminate_Check(10, luaman_task, dmobj);
    
    //��20�¿ո�
    for (index = 0; index < 25; ++index)
    {
        dmobj->KeyDown(32);
        Delay(50);
        dmobj->KeyUp(32);
        Delay(50);
    }
    /////////////////////////////////////////////////////////////////////////////////////////

    //�����Լ��Ľ���
    location = 19;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (key_index = 1; key_index < 31; key_index++)
    {
        dmobj->KeyDown(32);
        Delay(50);
        dmobj->KeyUp(32);
        Delay(50);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //ͬ��,�Ի�,�˳�����
    //������һ��,ͬ���ٽ��жԻ�,����ͬ����־
    location = 19;
    code = MarkReachFlag(groupid, hwnd, location);
    
    //����Ƿ񼯽����
    for (;;)
    {
        code = CheckSyncFlag(groupid, location);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("ͬ��:code:%d", code);
            break;
        }

        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("ͬ��:�ȴ�");
    }
    
    //�Ի�,������
    dmobj->MoveTo(5, 5);
    Delay(300);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aaa-12-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�Ի�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�Ի�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�Ի�:����:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aaa-12-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ���뿪����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("ȷ���뿪����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("ȷ���뿪����:����:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        ///////////////////////////////////////////////////////////////////////////////////////    
        //bug:�����һ��֮��,�ֵ�����һ�� 2019/12/22 21:37:22
        dmobj->SetDict(0, "font\\font-aaa-12-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ���뿪����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ���뿪����:���:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("ȷ���뿪����:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    api_log_MsgDebug("�����񸱱�����ִ�гɹ�");

    //////////////////////////////////////////////////////////////////////////////
    //bug:�ٷ������汾��,����3���˲��ܽ� 2020/5/28 21:37:19
    (void)Tz195_InviteHeilfellow(dmobj, ctxt, 2);

    //bug:��30���Ϊ25�� 2021/12/11 22:10:22 2022/01/08 18:34:33
    Delay_With_Terminate_Check(32, luaman_task, dmobj);
    return ERRNO_SUCCESS;
}

static int Tz195_SlaveMainLine(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int code = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    int instance = 0;
    int signin = 0;
    char* fposXY = NULL;
    int location = 1;
    int npc1_retry_count = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0;
    long posY = 0;
    int npc2_retry_count = 0;
    int npc3_retry_count = 0;
    int pGroupPosition = 0;
    int second_entry = 0;
    int ticketin = 1;
    
LABEL_Tz_SlaveMain_enter_copy:
    pathofposxys = "255:133|250:135";
    pathOfPosXYs = CString(pathofposxys);
    (void)Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);

    Check_Terminate_Thread(luaman_task, dmobj);
    
LABEL_Tz_SlaveMain_enter_copy_without_walk:
    Check_Terminate_Thread(luaman_task, dmobj);

    ////////////////////////////////////////////////////////////////////////////
    //bug:���븱��ǰ�����볡ȯ 2022/06/11 22:31:10
    if (ticketin)
    {
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->KeyPressChar("n");
            Delay(300);
            dmobj->KeyUpChar("n");
            Delay(300);
            dmobj->MoveTo(5, 5);
            Delay(300);

            dmobj->SetDict(0, FONT195_DIR"font-npc100001.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "[�����Ŀ������]�볡ȯ����", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("[�����Ŀ������]�볡ȯ����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            code = Tz195_CheckBlueOut(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("[�����Ŀ������]�볡ȯ����:����:����:����");
                goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
            }

            api_log_MsgDebug("[�����Ŀ������]�볡ȯ����:����:ʧ��");
            Delay(500);
        }

        //���
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX, lposY);
            Delay(300);
            dmobj->LeftClick();
            Delay(300);
            dmobj->MoveTo(5, 5);
            Delay(300);

            dmobj->SetDict(0, FONT195_DIR"font-npc100001.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "�����Ŀ�������볡ȯ����", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                
                api_log_MsgDebug("�����Ŀ�������볡ȯ����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }

            dmobj->SetDict(0, FONT195_DIR"font-npc100002.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                
                api_log_MsgDebug("����������:����:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

                //�ص���������
                for (;;)
                {
                    Check_Terminate_Thread(luaman_task, dmobj);
            
                    dmobj->MoveTo(lposX2, lposY2);
                    Delay(300);
                    dmobj->LeftClick();
                    Delay(300);
                    dmobj->MoveTo(5, 5);
                    Delay(300);

                    dmobj->SetDict(0, FONT195_DIR"font-npc100002.txt");
                    code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("����������:���:�ɹ�");

                        (void)Tz195_ExitGroup(dmobj, ctxt);
                        break;
                    }

                    lposX = pposX.lVal;
                    lposY = pposY.lVal;
                    
                    api_log_MsgDebug("����������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
                }
                
                return ERRNO_EXIT;
            }
            
            code = Tz195_CheckBlueOut(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("[�����Ŀ������]�볡ȯ����:����:����:����");
                goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
            }

            api_log_MsgDebug("[�����Ŀ������]�볡ȯ����:����:ʧ��");
            Delay(500);
        }

        //���
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX2, lposY2);
            Delay(300);
            dmobj->LeftClick();
            Delay(300);
            dmobj->MoveTo(5, 5);
            Delay(300);

            dmobj->SetDict(0, FONT195_DIR"font-npc100002.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("����������:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
                return ERRNO_EXIT;
            }
            
            dmobj->SetDict(0, FONT195_DIR"font-npc100001.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "�����Ŀ�������볡ȯ����", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�����Ŀ�������볡ȯ����:���:�ɹ�");
                break;
            }

            code = Tz195_CheckBlueOut(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("�����Ŀ�������볡ȯ����:���:����:����");
                goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
            }

            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�����Ŀ�������볡ȯ����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            Delay(500);
        }

        ticketin = 0;
    }
    ////////////////////////////////////////////////////////////////////////////

    
    //��N�Ի����븱��
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);
        
        dmobj->SetDict(0, "font\\font-aa-2-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����Ŀ������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�����Ŀ������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("�����Ŀ������:����:����:����");
            //goto LABEL_Tz_SlaveMain_enter_copy;
            goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;
            
            api_log_MsgDebug("�����Ŀ������:����:����:����:��������");
            goto LABEL_Tz_SlaveMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�����Ŀ������:����:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //���"�����Ŀ������"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-enter-ack.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�����Ŀ������:���:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("�����Ŀ������:���:����:����");
            //goto LABEL_Tz_SlaveMain_enter_copy;
            goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;
            
            api_log_MsgDebug("�����Ŀ������:���:����:����:��������");
            goto LABEL_Tz_SlaveMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�����Ŀ������:���:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //���"ȷ��"���븱��
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        code = Tz195_CheckMapStyle(dmobj, ctxt);
        if (MAP_FORGET_TOWER == code)
        {
            api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:���븱��");
            break;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("�����Ŀ������:ȷ��:���:����:����");
            goto LABEL_Tz_SlaveMain_enter_copy;
        }

        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("�����Ŀ������:ȷ��:���:����:����:��������");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:������Ա����ʱ�ȵ��ȷ���ٶԻ�
        dmobj->SetDict(0, "font\\font-peopleleak.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ա����", "ffffff-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            dmobj->MoveTo(lposX + 32, lposY + 104);
            Delay(300);
            dmobj->LeftClick();
            Delay(300);
            dmobj->MoveTo(5, 5);
            Delay(300);

            goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
        }

        //��鸱�����Ƿ��д�boss,�����������"����޷��볡"
        dmobj->SetDict(0, "font\\font-reenter.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����޷��볡��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:�������д�boss����ȥ,lposX:%ld, lposY:%ld", lposX, lposY);

            //����"�������д�boss����ȥ:ȷ��"����
            for (;;)
            {
                dmobj->SetDict( 0, "font\\font-reenter.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX3 = pposX3.lVal;
                    lposY3 = pposY3.lVal;

                    api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:�ɹ�,lposX3:%ld, lposY3:%ld", lposX3, lposY3);
                    break;
                }

                code = Tz195_CheckBlueOut(dmobj, ctxt);
                if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:����:����");
                    //goto LABEL_Tz_SlaveMain_enter_copy;
                    goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
                }

                if (ERRNO_BLOCK == code)
                {
                    instance = 0;
                    
                    api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:����:����");
                    goto LABEL_Tz_SlaveMain_enter_copy;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:ʧ��");
                Delay(500);
            }

            //���"�������д�boss����ȥ:ȷ��"����
            for (;;)
            {
                dmobj->MoveTo(lposX3, lposY3);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(500);
                
                dmobj->SetDict(0, "font\\font-reenter.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("�������д�boss����ȥ:ȷ��:���:�ɹ�");
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("�������д�boss����ȥ:ȷ��:���:ʧ��");
                Delay(500);
            }

            api_log_MsgDebug("�������д�boss����ȥ:ȷ��:���:�ɹ�:���¶Ի�");
            goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
        }//"����޷��볡"

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //û�н��븱��,Ҳû�г���"�������д�boss����ȥ",�����Ƿ���"�������³���"
        dmobj->SetDict(0, "font\\font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:�������³�����, lposX:%ld, lposY:%ld", lposX, lposY);

            //���"�������³���:ȷ��"
            for (;;)
            {
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(500);
                
                dmobj->SetDict(0, "font\\font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("�������³�����:ȷ��:���:�ɹ�");
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("�������³�����:ȷ��:���:ʧ��");
                Delay(500);
            }

            goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
        }//"�������³�����"

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:����:Ŭ��������");
        Delay(500);

        //bug:����Ѫ����� 2022/02/21 21:36
        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����:����:ȷ��:����:����:����");
            //goto LABEL_Tz_SlaveMain_enter_copy;
            goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;
            
            api_log_MsgDebug("����:����:ȷ��:����:����:����");
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }//���"ȷ��"���븱��

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:��������ʣ��ʱ���ж� 2020/1/15 22:17:35
    if (0 == instance)
    {
        instance = 1;
    }
    else
    {
        code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
        if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("�����Ŀ������:��ʱ");
            return code;
        }
        else if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("�����Ŀ������:��ʱ:�ж�:���:����:����");
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    //�ɹ����븱��,��״̬
    dmobj->KeyDownChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay_With_Terminate_Check(2, luaman_task, dmobj);
    
    dmobj->KeyDownChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyDownChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    ///////////////////////////////////////////////////////////////////////////////////////
    //����:�������� 2020/1/12 20:49:34
    dmobj->KeyDownChar("t");
    Delay(500);
    dmobj->KeyUpChar("t");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyDownChar("9");
    Delay(500);
    dmobj->KeyUpChar("9");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyDownChar("1");
    Delay(500);
    dmobj->KeyUpChar("1");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    //ǩ��
    if (0 == signin)
    {
        signin = 1;
        (void)Tz195_Login(dmobj, ctxt);
    }

    //ǰ������107:415
    pathofposxys = "107:415";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    //�ȴ�,������"��������ʱ��"ʱ��ʼ����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, "font\\font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������ʱ��", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������ʱ��:����:�ɹ�,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        api_log_MsgDebug("��������ʱ��:����:ʧ��");
        LUAMAN_WAIT_MS(1000);
    }

    pathofposxys = "107:415|145:414|160:411";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_battle_place_1:
    ///////////////////////////////////////////////////////////////////////////////
    //bug:160:412��Ϊ163:411->160:411 2019/12/10 20:18:31/22:24:19
    //������
    fposXY = "160:411";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    //���������,����ǰ����һ��
    pathofposxys = "154:412|155:428|171:428|189:428|197:421";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //������һ��,ͬ���ٽ��жԻ�,����ͬ����־
    (void)MarkReachFlag(groupid, hwnd, 1);

    //����Ƿ񼯽����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = CheckSyncFlag(groupid, 1);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("ͬ��:1:���,code:%d", code);
            break;
        }

        api_log_MsgDebug("ͬ��:1:�ȴ�");
        Delay(1000);
    }
    
    goto LABEL_Tz_SlaveMain_battle_place_1_x2;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:�������� 2020/1/26 22:03:15
LABEL_Tz_SlaveMain_battle_place_1_x1:
    pathofposxys = "197:421|191:429|176:430|166:429|154:429|152:415|135:415";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);
    
    //������
    fposXY = "135:415";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }
    
    //������
    fposXY = "122:414";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }
    
    //������
    fposXY = "106:415";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    //Ѱ·"106:415|122:414|135:415|152:415|154:429|166:429|176:430|191:429|197:421"
    pathofposxys = "106:415|122:414|135:415|152:415|154:429|166:429|176:430|191:429|197:421";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);
    
    //������һ��,ͬ���ٽ��жԻ�,����ͬ����־
    (void)MarkReachFlag(groupid, hwnd, 1);
    
    //����Ƿ񼯽����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = CheckSyncFlag(groupid, 1);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("ͬ��:1:���");
            break;
        }

        api_log_MsgDebug("ͬ��:1:�ȴ�");
        Delay(1000);
    }

LABEL_Tz_SlaveMain_battle_place_1_x2:
    //�������,��ʼ�Ի�
    dmobj->MoveTo(5, 5);
    Delay(300);

    npc1_retry_count = 0;
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-15-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ������Ӽ���", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ������Ӽ���:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        npc1_retry_count = npc1_retry_count + 1;
        if (npc1_retry_count > 5)
        {
            (void)ClearMarkReachFlag(groupid, hwnd, 1);

            api_log_MsgDebug("NPC1:�Ի�:ʧ��:����:����:���:�ص�1");
            goto LABEL_Tz_SlaveMain_battle_place_1_x1;
        }

        api_log_MsgDebug("�ƶ������Ӽ���:����:ʧ��");
        Delay(500);
    }
    
    //���"�ƶ������Ӽ���"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-15-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ������Ӽ���", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ������Ӽ���:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("�ƶ������Ӽ���:���:ʧ��");
        Delay(500);
    }

    //��鵱ǰ����,������겻��254:415������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, "font\\font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 254) <= 1 && abs(posY - 415) <= 1)
                {
                    api_log_MsgDebug("�ƶ������Ӽ���:�Ի�:�ɹ�");
                    break;
                }
                else if (abs(posX - 197) <= 1 && abs(posY - 421) <= 1)
                {
                    (void)ClearMarkReachFlag(groupid, hwnd, 1);
                    
                    api_log_MsgDebug("�ƶ������Ӽ���:�Ի�:ʧ��");
                    goto LABEL_Tz_SlaveMain_battle_place_1_x1;
                }
            }
        }

        api_log_MsgDebug("�ƶ������Ӽ���:�Ի�:����:��ȡ:ʧ��");
        Delay(500);
    }

    //�����Լ��Ľ���
    location = 1;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //��һ�ش����,����ڶ���
    pathofposxys = "272:411|286:406|292:402";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_battle_place_2:
    //bug:�ߵ���������ȥ�� 2020/2/22 14:12:22
    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyPressChar("a");
    Delay(50);
    dmobj->KeyPressChar("w");
    Delay(200);
    dmobj->KeyUpChar("a");
    Delay(50);
    dmobj->KeyUpChar("w");
    Delay(200);

    //////////////////////////////////////////////////////////////////////////////////////
    //bug:��λ��ʱ�򾭹�288:405�ٹ�λ,���ⱻ���� 2019/12/10 20:35:49
    //������
    fposXY = "288:405|292:402";
    location = 2;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    pathofposxys = "285:407|286:425|301:427|330:427|348:427|357:429";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    //��ڶ���npc�Ի�
    dmobj->MoveTo(5, 5);
    Delay(300);
        
    npc2_retry_count = 0;
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-27-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ��������㳡", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ��������㳡:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        npc2_retry_count = npc2_retry_count + 1;
        if (npc2_retry_count > 5)
        {
            api_log_MsgDebug("�ƶ��������㳡:����:��ʱ:NPC2:�Ի�:ʧ��:����:����:���:�ص�2");
            goto LABEL_Tz_SlaveMain_battle_place_2;
        }

        api_log_MsgDebug("�ƶ��������㳡:����:ʧ��");
        Delay(500);
    }

    //���"�ƶ��������㳡"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-27-1.txt");
        //code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ��������㳡", "00ff00-101010", 0.9, &pposX2, &pposY2);
        code = dmobj->FindStrFast(380, 27, 1279, 826, "�ƶ��������㳡", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ��������㳡:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("�ƶ��������㳡:���:ʧ��");
        Delay(500);
    }

    //��鵱ǰ����,������겻��374:428������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, "font\\font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 374) <= 1 && abs(posY - 428) <= 1)
                {
                    api_log_MsgDebug("�ƶ��������㳡:�Ի�:�ɹ�");
                    break;
                }
                else if (abs(posX - 357) <= 1 && abs(posY - 429) <= 1)
                {
                    api_log_MsgDebug("�ƶ��������㳡:�Ի�:ʧ��:����:����:���:�ص�2");
                    goto LABEL_Tz_SlaveMain_battle_place_2;
                }
            }
        }

        api_log_MsgDebug("�ƶ��������㳡:�Ի�:����:��ȡ:ʧ��");
        Delay(500);
    }

    //�����Լ��Ľ���
    location = 2;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //�ڶ��ش����,���������
    pathofposxys = "380:419|387:411";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);
    
LABEL_Tz_SlaveMain_battle_place_3:
    //������
    fposXY = "387:411";
    location = 3;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    //Ѱ·"380:397|375:388"
    pathofposxys = "380:397|378:392";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    //�������npc�Ի�
    dmobj->MoveTo(5, 5);
    Delay(300);
        
    npc3_retry_count = 0;
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-34-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�����һ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ�����һ����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        npc3_retry_count = npc3_retry_count + 1;
        if (npc3_retry_count > 5)
        {
            dmobj->KeyPressChar("9");
            Delay(300);
            dmobj->KeyUpChar("9");
            Delay(300);
            Delay(1700);
            npc3_retry_count = 0;

            api_log_MsgDebug("�ƶ�����һ����:����:��ʱ:NPC3:�Ի�:ʧ��:����:����:���:�ص�3");
        }

        api_log_MsgDebug("�ƶ�����һ����:����:ʧ��");
        Delay(500);
    }

    //���"�ƶ�����һ����"
    npc3_retry_count = 0;
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-35-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�����Ⱦ��ͥԺ", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        npc3_retry_count = npc3_retry_count + 1;
        if (npc3_retry_count > 5)
        {
            api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:����:��ʱ:NPC3:�Ի�:ʧ��:����:����:���:�ص�3");
            goto LABEL_Tz_SlaveMain_battle_place_3;
        }

        api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:����:ʧ��");
        Delay(500);
    }

    //���"�ƶ�����Ⱦ��ͥԺ"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-27-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�����Ⱦ��ͥԺ", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:���:ʧ��");
        Delay(500);
    }

    //��鵱ǰ����,������겻��386:312������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, "font\\font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 386) <= 1 && abs(posY - 312) <= 1)
                {
                    api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:�Ի�:�ɹ�");
                    break;
                }
                else if (abs(posX - 375) <= 1 && abs(posY - 388) <= 1)
                {
                    api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:�Ի�:ʧ��");
                    goto LABEL_Tz_SlaveMain_battle_place_3;
                }
            }
        }

        api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:�Ի�:����:��ȡ:ʧ��");
        Delay(500);
    }
    
    //�����Լ��Ľ���
    location = 3;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //�����ش����,������Ĺ�
LABEL_Tz_SlaveMain_battle_place_4:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //���Ĺؿ����Ѿ������,������岽
    //Ѱ·"386:312|377:297|369:282"
    pathofposxys = "386:312|377:297|369:282";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_battle_place_5:
    //������
    fposXY = "369:282";
    location = 5;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    //��ȡ����,��ȷ���Ƿ�������
    location = 5;
    code = GetGroupPosition(groupid, &pGroupPosition);
    if (ERRNO_SUCCESS == code && pGroupPosition >= location)
    {
        pathofposxys = "373:261|381:246|384:226|383:213";
        api_log_MsgDebug("���岽:����:���:�����:��ȡ:�ɹ�:��6��:����,pGroupPosition:%d", pGroupPosition);
    }
    else
    {
        api_log_MsgDebug("��6��:����:���:׼��,code:%d, pGroupPosition:%d", code, pGroupPosition);

        //��6��
        //Ѱ·���˶�����
        pathofposxys = "373:261|381:246";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
        if (ERRNO_SUCCESS != code)
        {
            if (ERRNO_BLOCK == code)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
                return code;
            }

            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
                goto LABEL_Tz_SlaveMain_enter_copy;
            }
        }

        api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

        //������
        fposXY = "381:246";
        location = 6;
        code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
        if (ERRNO_SUCCESS != code)
        {
            if (ERRNO_BLOCK == code)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
                return code;
            }

            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
                goto LABEL_Tz_SlaveMain_enter_copy;
            }
        }

        pathofposxys = "384:226|383:213";
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //��4~6�ؿ����Ѿ������,������߹�
    //Ѱ·����383:213
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    //������һ��,ͬ���ٽ��жԻ�,����ͬ����־
    (void)MarkReachFlag(groupid, hwnd, 7);
    
    //����Ƿ񼯽����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = CheckSyncFlag(groupid, 7);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("ͬ��:7:���,code:%d", code);
            break;
        }

        api_log_MsgDebug("ͬ��:7:�ȴ�");
        Delay(1000);
    }

    //�������,��״̬
    dmobj->KeyDownChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay_With_Terminate_Check(2, luaman_task, dmobj);
    
    dmobj->KeyDownChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyDownChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��U�ҵ����ʯ:�������ʯ
LABEL_Tz_SlaveMain_battle_place_7:
    //������
    fposXY = "383:213";
    location = 7;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }
    
    //�����Լ��Ľ���
    location = 7;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);
    (void)UpdateGroupPosition(groupid, location, 0);

    pathofposxys = "383:213|368:210|352:206|330:196|318:185|313:170|309:158|301:141|293:127|286:113|263:111|244:112";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_continue_copy;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    //��ɱ,�����ȥ
    for (;;)
    {
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            second_entry = 1;
            api_log_MsgDebug("��ɱ:�ɹ�:����");
            break;
        }

        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("��ɱ:�ȴ�:����ʱ��̫��:������һ��");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("��ɱ:�ȴ�");
        Delay(1000);
    }

LABEL_Tz_SlaveMain_continue_copy:
    return Tz195_SlaveMainLine_Continue(dmobj, ctxt);
}

int Tz195_SlaveMainLine_Continue(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int code = 0;
    struct xyposition syncPosXYs[4] =
    {
        {1234, 266},
        {1272, 286},
        {1234, 298},
        {1272, 318},
    };
    char* pathofposxys = NULL;
    CString pathOfPosXYs;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    int instance = 0;
    int forward2place8 = 0;
    int forward2place10 = 0;
    int forward2place11 = 0;
    int location = 0;
    char* fposXY = NULL;
    int forward2place12 = 0;
    int forward2place13 = 0;
    int forward2place14 = 0;
    int forward2place15 = 0;
    int forward2place16 = 0;
    int forward2place17 = 0;
    int skip_alive_flag = 0;
    CString strMapName;
    int syncCount = 0;
    int index = 0;
    long posX1 = 0, posY1 = 0;
    long posX2 = 0, posY2 = 0;
    
LABEL_Tz_SlaveMain_enter_copy_continue:
    pathofposxys = "255:133|250:135";
    pathOfPosXYs = CString(pathofposxys);
    (void)Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    
LABEL_Tz_SlaveMain_enter_copy_continue_without_walk:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-2-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����Ŀ������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�����Ŀ������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("�����Ŀ������:����:����:����");
            //goto LABEL_Tz_SlaveMain_enter_copy_continue;
            goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("�����Ŀ������:����:����:����:��������");
            return code;
        }
        
        api_log_MsgDebug("�����Ŀ������:����:ʧ��");
        Delay(500);
    }

    //���"�����Ŀ������"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-enter-ack.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�����Ŀ������:���:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("�����Ŀ������:���:����:����");
            //goto LABEL_Tz_SlaveMain_enter_copy_continue;
            goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("�����Ŀ������:���:����:����:��������");
            return code;
        }
        
        api_log_MsgDebug("�����Ŀ������:���:ʧ��");
        Delay(500);
    }

    //���"ȷ��"���븱��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        code = Tz195_CheckMapStyle(dmobj, ctxt);
        if (MAP_FORGET_TOWER == code)
        {
            api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:���븱��");
            break;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("�����Ŀ������:ȷ��:���:����:����");
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }

        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("�����Ŀ������:ȷ��:���:����:����:��������");
            return code;
        }

        //��鸱�����Ƿ��д�boss,�����������"����޷��볡"
        dmobj->SetDict(0, "font\\font-reenter.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����޷��볡��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:�������д�boss����ȥ,lposX:%ld, lposY:%ld", lposX, lposY);

            //����"�������д�boss����ȥ:ȷ��"����
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict( 0, "font\\font-reenter.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX3 = pposX3.lVal;
                    lposY3 = pposY3.lVal;

                    api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:�ɹ�,lposX3:%ld, lposY3:%ld", lposX3, lposY3);
                    break;
                }

                code = Tz195_CheckBlueOut(dmobj, ctxt);
                if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:����:����");
                    //goto LABEL_Tz_SlaveMain_enter_copy_continue;
                    goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
                }

                if (ERRNO_BLOCK == code)
                {
                    api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:����:����");
                    return code;
                }

                api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:ʧ��");
                Delay(500);
            }

            //���"�������д�boss����ȥ:ȷ��"����
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX3, lposY3);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(500);
                
                dmobj->SetDict(0, "font\\font-reenter.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("�������д�boss����ȥ:ȷ��:���:�ɹ�");
                    break;
                }

                api_log_MsgDebug("�������д�boss����ȥ:ȷ��:���:ʧ��");
                Delay(500);
            }

            api_log_MsgDebug("�������д�boss����ȥ:ȷ��:���:�ɹ�:���¶Ի�");
            goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
        }//"����޷��볡"

        //û�н��븱��,Ҳû�г���"�������д�boss����ȥ",�����Ƿ���"�������³���"
        dmobj->SetDict(0, "font\\font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:�������³�����, lposX:%ld, lposY:%ld", lposX, lposY);

            //���"�������³���:ȷ��"
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(500);
                
                dmobj->SetDict(0, "font\\font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("�������³�����:ȷ��:���:�ɹ�");
                    break;
                }

                api_log_MsgDebug("�������³�����:ȷ��:���:ʧ��");
                Delay(500);
            }

            goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
        }//"�������³�����"

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:������Ա����ʱ�ȵ��ȷ���ٶԻ�
        dmobj->SetDict(0, "font\\font-peopleleak.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ա����", "ffffff-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 32, lposY + 104);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, "font\\font-peopleleak.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ա����", "ffffff-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("�������³�����:ȷ��:��Ա����:���:�ɹ�:�������");
                    goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
                }

                code = Tz195_CheckBlueOut(dmobj, ctxt);
                if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("�������³�����:ȷ��:��Ա����:���:����:����:����");
                    goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
                }

                if (ERRNO_BLOCK == code)
                {
                    api_log_MsgDebug("�������³�����:ȷ��:��Ա����:���:����:����:����:��������:������һ����");
                    goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
                }

                api_log_MsgDebug("�������³�����:ȷ��:��Ա����:���:ʧ��");
                Delay(500);
            }
        }

        api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:����:Ŭ��������");
        Delay(500);

        //bug:����Ѫ����� 2022/02/21 21:36
        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����:����:ȷ��:����:����:����");
            goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("����:����:ȷ��:����:����:����");
            goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
        }
    }//���"ȷ��"���븱��

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:��������ʣ��ʱ���ж� 2020/1/15 22:17:35
    if (0 == instance)
    {
        instance = 1;
    }
    else
    {
        code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
        if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("�����Ŀ������:��ʱ");
            return code;
        }
        else if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("�����Ŀ������:��ʱ:�ж�:���:����:����");
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    //�ɹ����븱��,��״̬
    dmobj->KeyDownChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay_With_Terminate_Check(2, luaman_task, dmobj);

    dmobj->KeyDownChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dmobj->KeyDownChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    ///////////////////////////////////////////////////////////////////////////////////////
    //����:�������� 2020/1/12 20:49:34
    dmobj->KeyDownChar("t");
    Delay(500);
    dmobj->KeyUpChar("t");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    pathofposxys = "101:405|119:414|139:415|152:416|158:429|183:429|196:422";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    dmobj->MoveTo(5, 5);
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-71-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ������Ӽ���", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ������Ӽ���:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�ƶ������Ӽ���:����:ʧ��");
        Delay(500);
    }

    //���"�ƶ������Ӽ���"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-71-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ������Ӽ���", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ������Ӽ���:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("�ƶ������Ӽ���:���:ʧ��");
        Delay(500);
    }

    pathofposxys = "254:415|275:420|292:425|306:426|323:427|342:428|356:429";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    dmobj->MoveTo(5, 5);
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-79-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ��������㳡", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ��������㳡:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�ƶ��������㳡:����:ʧ��");
        Delay(500);
    }

    //���"�ƶ��������㳡"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-79-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ��������㳡", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ��������㳡:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("�ƶ��������㳡:���:ʧ��");
        Delay(500);
    }

    pathofposxys = "374:428|381:410|379:397|376:389";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    dmobj->MoveTo(5, 5);
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-84-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�����һ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ�����һ����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�ƶ�����һ����:����:ʧ��");
        Delay(500);
    }

    //���"�ƶ�����һ����"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-85-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�������Ļ���", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("�ƶ�������Ļ���:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�ƶ�������Ļ���:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-85-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�������Ļ���", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ�������Ļ���:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("�ƶ�������Ļ���:���:ʧ��");
        Delay(500);
    }

    pathofposxys = "357:400|347:395|336:381|329:366|315:353|300:337|292:335";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    dmobj->MoveTo(5, 5);
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-94-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������Ļ���", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������Ļ���:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��������Ļ���:����:ʧ��");
        Delay(500);
    }

    //���"��������Ļ���"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-94-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������Ļ���", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("��������Ļ���:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("��������Ļ���:���:ʧ��");
        Delay(500);
    }

LABEL_Tz_SlaveMain_Continue_walk_place_8:
    /////////////////////////////////////////////////////////////////////////////////////////
    //bug:��1������
    dmobj->KeyDownChar("1");
    Delay(500);
    dmobj->KeyUpChar("1");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    //bug:2021/11/28 09:08:12
    dmobj->KeyDownChar("9");
    Delay(500);
    dmobj->KeyUpChar("9");
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    forward2place8 = 0;

    pathofposxys = "281:303|278:300|277:290|280:286|280:275|269:275|264:278|255:278|254:270";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place8 = 1;
            
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);
    
LABEL_Tz_SlaveMain_Continue_walk_place_10:
    forward2place10 = 0;
    
    pathofposxys = "254:270|253:278|230:278|210:278";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place10 = 1;
            
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_Continue_battle_place_11:
    if (0 != instance)
    {
        code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
        if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("210:278:����ʣ��ʱ��:����:������һ��");
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("210:278:����ʣ��ʱ��:���:����:����");
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    forward2place11 = 0;

    //������
    fposXY = "210:278";
    location = 11;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place11 = 1;
            
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

LABEL_Tz_SlaveMain_Continue_walk_place_12:
    forward2place12 = 0;

    pathofposxys = "210:278|194:278|185:277|179:277|179:287|185:290";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place12 = 1;
            
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_Continue_battle_place_13:
    if (0 != instance)
    {
        code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
        if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("185:290:����ʣ��ʱ��:����:������һ��");
            return code;
        }
        else if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("185:290:����ʣ��ʱ��:���:����:����");
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    forward2place13 = 0;

    //������
    fposXY = "185:290";
    location = 13;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place13 = 1;
            
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

LABEL_Tz_SlaveMain_Continue_walk_place_14:
    forward2place14 = 0;
    
    pathofposxys = "185:290|178:290|178:301|180:307|179:327|177:333|178:345|188:348|194:337|194:324|187:321|196:313|205:314|208:323|209:332|204:341|210:348|224:341|240:340";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place14 = 1;
            
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_Continue_battle_place_15:
    if (0 != instance)
    {
        code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
        if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("240:340:����ʣ��ʱ��:����:������һ��");
            return code;
        }
        else if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("240:340:����ʣ��ʱ��:���:����:����");
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    forward2place15 = 0;

    //������
    fposXY = "240:340";
    location = 15;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place15 = 1;
            
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

LABEL_Tz_SlaveMain_Continue_walk_place_16:
    forward2place16 = 0;

    pathofposxys = "240:340|255:345|265:348|273:349|270:360|253:362|239:362|224:362|211:362|205:357|196:356|192:362|172:362|155:355|151:344|151:326|164:310|167:306|167:293|160:281";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place16 = 1;
            
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_Continue_npc_place_17:
    //������
    forward2place17 = 0;
    fposXY = "160:281";
    location = 17;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("������:����:����:��ʧ:����:��һ��,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("������:����:����:����:����:����,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    (void)FlagMark(groupid, role);

    pathofposxys = "161:282|158:262|154:242|154:227";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("Ѱ·:����:����:��ʧ:����:��һ��,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:����:����:����:����:����,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("Ѱ·:�ɹ�,pathofposxys:%s", pathofposxys);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = FlagCheck(groupid);
        if (0 == code)
        {
            api_log_MsgDebug("����ŵ˹:ͬ��:���");
            break;
        }

        api_log_MsgDebug("����ŵ˹:ͬ��:�ȴ�");
        Delay(1000);
    }

    goto Label_Tz_SlaveMain_Continue_waitout;


Label_Tz_SlaveMain_Continue_waitout:
    //�����Լ��Ľ���
    location = 19;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    //ͬ���� �Ƶ���⵽���ܺ���ͬ�� 2022/04/25 22:52:11
    //location = 19;
    //(void)MarkReachFlag(groupid, hwnd, location);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:�ҵ����ܺ���ͬ��,�����ȥʱʱ�䲻һ�� 2021/11/25 23:37:11
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����:�˳�:ͬ��:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        /////////////////////////////////////////////////////////////////////////////
        //bug:��������ʾ���� 2022/03/21 09:44:22
        dmobj->SetDict(0, "font\\font-chcklifex.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "Ҫ��ԭ�ظ�����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            dmobj->KeyDownChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);

            api_log_MsgDebug("Ҫ��ԭ�ظ�����:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
        }
        
        api_log_MsgDebug("����:�˳�:ͬ��:����:����:ʧ��");
        Delay(500);
    }

    //ͬ���� �Ƶ���⵽���ܺ���ͬ�� 2022/04/25 22:52:11
    location = 19;
    (void)MarkReachFlag(groupid, hwnd, location);

    skip_alive_flag = 0;

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = CheckSyncFlag(groupid, location);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("ͬ��:���");
            break;
        }

        if (ROLE_M != role)
        {
            dmobj->SetDict(0, "font\\font-mapstyle.txt");
            strMapName = dmobj->Ocr(1140, 29, 1273, 58, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (strMapName == CString("�������"))
            {
                if (0 == skip_alive_flag)
                {
                    skip_alive_flag = 1;
                }

                api_log_MsgDebug("ͬ��:����:�Զ�����:��������");
            }
        }

        (void)check_alarm(dmobj, ctxt);

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("ͬ��:�ȴ�");
        Delay(1000);
    }

    if (1 == skip_alive_flag)
    {
        api_log_MsgDebug("ͬ��:�ȴ�:�Զ�����:����:��ɱ:����:��һ����");
        goto LABEL_jump_to_next_copy;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //���� bug:�����Ѿ�ͬ����ȥ�ֽ�����,�����Ż��ڵȴ�����.�����Ϳ�ס��(δ���ȱ��) 2020/03/02 20:36:11
    for (;;)
    {
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("����:����:��һ��");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("����:����:��һ��;�ȴ�");
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
    }

    Delay(200);
    
LABEL_jump_to_next_copy:
    dmobj->KeyDownChar("1");
    Delay(200);
    dmobj->KeyUpChar("1");
    Delay(200);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:�����2��"ͬ��"��ſ�ʼ��ʱ�ȴ�30��Ϊ25�� 2021/12/11 22:16:35
    //���'ͬ��'���� 2022/05/26 23:25:11 ע�͵�
    /*for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        syncCount = 0;
        for (index = 0; index < 2; index++)
        {
            posX1 = syncPosXYs[2 * index].x;
            posY1 = syncPosXYs[2 * index].y;
            posX2 = syncPosXYs[2 * index + 1].x;
            posY2 = syncPosXYs[2 * index + 1].y;

            dmobj->SetDict(0, "font\\font-inviteheil.txt");
            code = dmobj->FindStrFast(posX1, posY1, posX2, posY2, "ͬ��", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                syncCount = syncCount + 1;
                api_log_MsgDebug("����:ͬ��:����:��Ա:��1");
            }
            Check_Terminate_Thread(luaman_task, dmobj);
        }

        if (2 == syncCount)
        {
            api_log_MsgDebug("����:ͬ��:���");
            break;
        }
    }*/
    ///////////////////////////////////////////////////////////////////////////////////////

    //bug:25�ĳ�27�� 2022/01/08 18:34:33
    Delay_With_Terminate_Check(32, luaman_task, dmobj);

    api_log_MsgDebug("������:����:part2:����");
    
    return ERRNO_SUCCESS;
}

int Tz195_Walk2WaitPoint(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    
    //����'���ش�����'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-ww01.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ش�����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���ش�����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���ش�����:����:ʧ��");
        Delay(500);
    }

    //����'�ƶ�'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //����
        dmobj->SetDict(0, "font\\font-addmv.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("���ش�����:�ƶ�:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("���ش�����:�ƶ�:����:ʧ��");
        Delay(500);
    }

    //���'�ƶ�'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-addmv.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���ش�����:�ƶ�:���:�ɹ�");
            break;
        }

        Delay(500);
        api_log_MsgDebug("���ش�����:�ƶ�:���:ʧ��");
    }
    
    //���ŵ���Ŀ������ 127:31
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, "font\\font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 127) <= 1 && abs(posY - 31) <= 1)
                {
                    api_log_MsgDebug("���ش�����:[424:255]:�ﵽ");
                    break;
                }
            }
        }

        //���Ѫ��
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("���ش�����:����:����");
            return code;
        }
        
        api_log_MsgDebug("���ش�����:[127:31]:δ�ﵽ");
        Delay(500);
    }
    
    return ERRNO_SUCCESS;
}

int Tz195_MainAuxi(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    VARIANT pposX9, pposY9;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    long lposX9 = 0, lposY9 = 0;
    CString strPosXY;
    int spcnt = 0;
    CStringArray posXY;
    long posX = 0, posY = 0;
    time_t lastChckTime = 0;
    time_t curTime = 0;
    double deltaTime = 0;
    
    for (;;)
    {
    LABEL_Tz195_main_auxi_start:
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, "font\\font-join2team.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("�������:����:�ɹ�, lposX:%d, lposY:%ld", lposX, lposY);
                break;
            }

            Delay(1000);
        }

        //���'��'�����������
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX - 32, lposY + 126);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, "font\\font-join2team.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "����ں�", "f4be3d-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                    
                api_log_MsgDebug("�������:���:�ɹ�, lposX2:%d, lposY2:%ld", lposX2, lposY2);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            Delay(1000);
            api_log_MsgDebug("�������:���:ʧ��");
        }

        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, "font\\font-join2team.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "ffff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("�������:����:�ɹ�, lposX:%d, lposY:%ld", lposX, lposY);
                break;
            }

            Delay(500);
            api_log_MsgDebug("�������:����:ʧ��");
        }

        //����ƶ� 672:353->774:356 788:353->852:355
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            //dmobj.MoveTo pposX + 102, pposY + 3
            dmobj->MoveTo(lposX + 64, lposY + 2);
            Delay(300);
            dmobj->LeftClick();
            Delay(300);
            dmobj->MoveTo( 5, 5);
            Delay(300);

            dmobj->SetDict(0, "font\\font-move.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "��ӳ�Ա����", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                
                api_log_MsgDebug("��ӳ�Ա����:����:�ɹ�, lposX2:%d, lposY2:%ld", lposX2, lposY2);
                break;
            }

            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:�����˳������ʱ������յ����� 2020/2/23 14:06:55
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict(0, "font\\font-join2team.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "ffffff-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("��ӳ�Ա����:����:ʧ��:�������:����:ʧ��");
                    break;
                }

                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;
                    
                //���'��'ȡ�� �������: 614:409 ��: 582:535 ��: 697:535
                dmobj->MoveTo(lposX3 + 83, lposY3 + 126);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                api_log_MsgDebug("��ӳ�Ա����:����:ʧ��:�������:����:�ɹ�, lposX3:%d, lposY3:%ld", lposX3, lposY3);
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            Delay(1000);
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // bug:������ͼ��Ϊ������� 2021/11/25 22:29:15
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, "font\\font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (strPosXY != CString(""))
            {
                spcnt = SplitStringWide(strPosXY, ":", posXY);
                if (spcnt >= 2)
                {
                    posX = atol(posXY.GetAt(0));
                    posY = atol(posXY.GetAt(1));

                    if (abs(posX - 255) <= 1 && abs(posY - 134) <= 1)
                    {
                        api_log_MsgDebug("����:����:�ƶ�:255:134:�ɹ�");
                        break;
                    }
                }
            }

            //���Ѫ��
            code = Tz195_CheckBlue(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("����:����:�ƶ�:255:134:����:����");

                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(1000);
                goto LABEL_Tz195_main_auxi_skip;
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("1");
        Delay(500);
        dmobj->KeyUpChar("1");
        Delay_With_Terminate_Check(2, luaman_task, dmobj);
        
        /////////////////////////////////////////////////////////////////////
        //bug:����Ƿ�����,���˾�Ҫ���� 2020/2/20 19:28:11
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("����:����:�ƶ�:255:134:����:����");

            Check_Terminate_Thread(luaman_task, dmobj);
            Delay(1000);
            goto LABEL_Tz195_main_auxi_start;
        }

        //�س� bug:ע�͵��س� 2021/11/25 22:29:15 -->���صȴ��ص�
        code = Tz195_Walk2WaitPoint(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("ǰ��:�ȴ�:�ص�:����:����");

            Check_Terminate_Thread(luaman_task, dmobj);
            Delay(1000);
            goto LABEL_Tz195_main_auxi_start;
        }
        
    LABEL_Tz195_main_auxi_skip:
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->KeyDownChar("p");
            Delay(200);
            dmobj->KeyUpChar("p");
            Delay(200);
            dmobj->MoveTo(5, 5);
            Delay(200);
            
            dmobj->SetDict( 0, "font\\font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("�˳�����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict(0, "font\\font-join2team.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "ffffff-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("�˳�����:����:ʧ��:�������:����:ʧ��");
                    break;
                }

                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;

                api_log_MsgDebug("�˳�����:����:ʧ��:�������:����:�ɹ�,lposX3:%ld, lposY3:%ld", lposX3, lposY3);

                Check_Terminate_Thread(luaman_task, dmobj);
                
                //���'��'ȡ�� �������: 614:409 ��: 582:535 ��: 697:535
                dmobj->MoveTo(lposX3 + 83, lposY3 + 126);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);
            }

            Delay(500);
        }

        lastChckTime = time(NULL);
        //�һ���ɫ��"�����Ŀ������"��ŵ������
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, "font\\font-ww02.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�����Ŀ������", "ffff00-101010", 0.9, &pposX9, &pposY9);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX9 = pposX9.lVal;
                lposY9 = pposY9.lVal;
                
                api_log_MsgDebug("�˳�����:����:�����Ŀ������:����:�ɹ�, lposX9:%ld, lposY9:%ld", lposX9, lposY9);
                break;
            }

            api_log_MsgDebug("�˳�����:����:�����Ŀ������:����:ʧ��");
            Delay(500);

            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict(0, "font\\font-join2team.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "ffffff-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;

                //���'��'ȡ�� �������: 614:409 ��: 582:535 ��: 697:535
                dmobj->MoveTo(lposX3 + 83, lposY3 + 126);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);
            }

            curTime = time(NULL);
            deltaTime = difftime(curTime, lastChckTime);
            if (deltaTime > (120 * 1.0))
            {
                api_log_MsgDebug("�˳�����:����:�����Ŀ������:����:��ʱ");
                break;
            }
        }
        
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX + 5, lposY + 5);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, "font\\font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                
                api_log_MsgDebug("ȷ��:����:�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }

            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:�����˳������ʱ������յ����� 2020/2/23 14:06:55
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict(0, "font\\font-join2team.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "ffffff-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("ȷ��:����:ʧ��:�������:����:ʧ��");
                    break;
                }

                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;

                api_log_MsgDebug("ȷ��:����:ʧ��:�������:����:�ɹ�,lposX3:%ld, lposY3:%ld", lposX3, lposY3);

                Check_Terminate_Thread(luaman_task, dmobj);
                
                //���'��'ȡ�� �������: 614:409 ��: 582:535 ��: 697:535
                dmobj->MoveTo(lposX3 + 83, lposY3 + 126);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);
            }
            
            Delay(500);
        }
        
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX2, lposY2);
            Delay(200);
            dmobj->LeftClick();
            Delay(200);
            dmobj->MoveTo(5, 5);
            Delay(200);
            
            dmobj->SetDict(0, "font\\font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                break;
            }

            lposX = pposX.lVal;
            lposY = pposY.lVal;

            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:�����˳������ʱ������յ����� 2020/2/23 14:06:55
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict(0, "font\\font-join2team.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "ffffff-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;
            
                //���'��'ȡ�� �������: 614:409 ��: 582:535 ��: 697:535
                dmobj->MoveTo(lposX3 + 83, lposY3 + 126);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);
            }

            Delay(500);
        }
        
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    return ERRNO_SUCCESS;
}

//#define DEV_MODE
#ifdef DEV_MODE
int api_Task195Init(Idmsoft* dmobj, void* ctxt)
{
    return ERRNO_SUCCESS;
}

int api_Task195Main(Idmsoft* dmobj, void* ctxt)
{
    for (;;)
    {
        api_log_MsgDebug("execute api_Task195Main loop finish");
        Delay(2000);
    }
    
    return ERRNO_SUCCESS;
}

int api_Task195Exit(Idmsoft* dmobj, void* ctxt)
{
    return ERRNO_SUCCESS;
}
#else
//ctxt:struct clumantask
int api_Task195Init(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int code = 0;
    
    api_log_MsgDebug("����:195:��ʼ��:��ʼ, hwnd:%ld", hwnd);

    code = GetContext(groupid, hwnd, luaman_config->role);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_EXIST == code)
        {
            api_log_MsgError("task195 is aready running, hwnd:%ld", hwnd);
            AfxMessageBox("������195����");
            return code;
        }

        if (ERRNO_CONFLICT == code)
        {
            api_log_MsgError("role conflict, hwnd:%ld", hwnd);
            AfxMessageBox("��ɫ��ͻ,����195����");
            return code;
        }

        api_log_MsgError("run task195 failed, hwnd:%ld", hwnd);
        AfxMessageBox("����195����ʧ��");
        return code;
    }
    
    code = UpdateGameTaskId(groupid, hwnd, luaman_task->taskid);
    if (ERRNO_SUCCESS != code)
    {
        api_log_MsgError("UpdateGameTaskId failed, hwnd:%ld, code:%d, taskid:%ld",
            hwnd, code, luaman_task->taskid);

        (void)PutContext(groupid, hwnd);
        return code;
    }
    
    /* �󶨴��� */
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("bind hwnd failed, hwnd:%ld", hwnd);

        (void)PutContext(groupid, hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("bind hwnd success, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("lock hwnd failed, hwnd:%ld", hwnd);

        (void)PutContext(groupid, hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("����:195:��ʼ��:���, hwnd:%ld", hwnd);

    /* ��ʼ���澯ʱ���ʼֵ */
    init_alarm_ctxt();

    return ERRNO_SUCCESS;
}

/* ���ش����뽫��ֹ�߳� */
int api_Task195Main(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int groupid = luaman_config->groupid;
    int status = 0;
    
    api_log_MsgDebug("����:195:��ѭ��: hwnd:%ld, role:%d", hwnd, (int)role);

    switch (role)
    {
    case ROLE_M:
        api_log_MsgDebug("����:195:����:��ѭ��:��ʼ, hwnd:%ld", hwnd);
        
        Check_Terminate_Thread(luaman_task, dmobj);
        
        FlagReset(groupid);
        status = Tz195_MasterMainLine(dmobj, ctxt);
        if (ERRNO_EXIT == status)
        {
            api_log_MsgDebug("����:195:����:��ѭ��:����, hwnd:%ld", hwnd);
            return status;
        }
        
        Check_Terminate_Thread(luaman_task, dmobj);
        
        ResetGame(groupid, hwnd);
        ResetGroup(groupid);

        api_log_MsgDebug("����:195:����:��ѭ��:���, hwnd:%ld", hwnd);
        break;

    case ROLE_S0:
    case ROLE_S1:
        api_log_MsgDebug("����:195:����:��ѭ��:��ʼ, hwnd:%ld", hwnd);
    
        Check_Terminate_Thread(luaman_task, dmobj);

        status = Tz195_SlaveMainLine(dmobj, ctxt);
        if (ERRNO_EXIT == status)
        {
            api_log_MsgDebug("����:195:����:��ѭ��:����, hwnd:%ld", hwnd);
            return status;
        }
        
        Check_Terminate_Thread(luaman_task, dmobj);
        
        ResetGame(groupid, hwnd);

        api_log_MsgDebug("����:195:����:��ѭ��:���, hwnd:%ld", hwnd);
        break;

    case ROLE_AUX:
        api_log_MsgDebug("����:195:�����:��ѭ��:��ʼ, hwnd:%ld", hwnd);
        
        Check_Terminate_Thread(luaman_task, dmobj);
        Tz195_MainAuxi(dmobj, ctxt);
        Check_Terminate_Thread(luaman_task, dmobj);

        api_log_MsgDebug("����:195:�����:��ѭ��:���, hwnd:%ld", hwnd);
        break;
        
    default:
        break;
    }
    
    return ERRNO_SUCCESS;
}

#ifndef FONT1853_DIR
#define FONT1853_DIR "font\\1853\\"
#endif

static int Tz195_InviteHeilfellowJoin2Team(Idmsoft* dmobj, void* ctxt)
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
        /*code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("���:����:��Ӻ���:����:����");
            return code;
        }*/

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
        /*code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("���:����:ս������:����:����:����");
            return code;
        }*/

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
                /*code = Tz1853_CheckLife(dmobj, ctxt);
                if (SNAIL_ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("���:����:��Ա:����:����:����:����");
                    return code;
                }*/

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
    /*code = Tz1853_CheckLife(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code)
    {
        api_log_MsgDebug("������:����:����:���");
        return code;
    }*/

    api_log_MsgDebug("������:�ɹ�:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int _Fly2Copy185(Idmsoft* dmobj, void* ctxt)
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
        code = dmobj->FindStrFast(210, 45, 1138, 745, "���׵Ļ�¯", "cefbc9-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���׵Ļ�¯:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        /*code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("��������:����:m����:��ɱ��:����,�ɹ�");
            return code;
        }*/

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
        
        /*code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("��������:����:�ƶ�:����:��ɱ��:����,�ɹ�");
            return code;
        }*/

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
        
        /*code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("��������:����:�ƶ�:�����:��ɱ��:����,�ɹ�");
            return code;
        }*/

        api_log_MsgDebug("��������:����:�ƶ�:δ����");
        Delay(1000);
    }
    
    return SNAIL_ERRNO_SUCCESS;
}
static int _Fly195to185(Idmsoft* dmobj, void* ctxt)
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
    CString strMapName;
    
    //��m������С��ͼ,Ϊ��ֹ����,�Ƚ�����Ƶ����Ͻ�
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(300);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);

        //����'�ƶ�'����
        dmobj->SetDict(0, FONT1853_DIR"font-fly-mcheck2.txt");
        code = dmobj->FindStrFast(210, 45, 1138, 745, "�ƶ�", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("195:185:�ƶ�:����:�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        /*code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("��������:195:185:�ƶ�:����:����,�ɹ�");
            return code;
        }*/

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("��������:195:185:�ƶ�:����:ʧ��");
        Delay(500);
    }

    //����Ƶ�
    dmobj->MoveTo(lposX2, lposY2);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, "font\\font-mapstyle.txt");
        strMapName = dmobj->Ocr(1140, 29, 1273, 58, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);

        if (strMapName == CString("������"))
        {
            api_log_MsgDebug("��������:195:185:������:����");
            break;
        }
        
        api_log_MsgDebug("��������:195:185:������:δ����");
        Delay(500);
    }

    return _Fly2Copy185(dmobj, ctxt);
}

static int Fly195to185(Idmsoft* dmobj, void* ctxt)
{
    int code = 0;
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    int groupid = luaman_config->groupid;
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    
    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("�ٽ���:����:�ȴ�");
    Tz195EnterCriticalSection();
    api_log_MsgDebug("�ٽ���:����:�ɹ�");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        code = Tz195_InviteHeilfellowJoin2Team(dmobj, ctxt);
        if (SNAIL_ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("���:���");
            break;
        }

        check_alarm(dmobj, ctxt);
    }

    api_log_MsgDebug("�ٽ���:�˳�:�ȴ�");
    Tz195LeaveCriticalSection();
    api_log_MsgDebug("�ٽ���:�˳�:�ɹ�");

    return _Fly195to185(dmobj, ctxt);
}

int api_Task195Exit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    int groupid = luaman_config->groupid;
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();

    ///////////////////////////////////////////////////////////////////////////////////
    //����:195����֮��ɵ�185�����ſ� 2022/06/12 10:00:00
    (void)Fly195to185(dmobj, ctxt);

    ///////////////////////////////////////////////////////////////////////////////////
    
    (void)PutContext(groupid, hwnd);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
    
    api_log_MsgDebug("����:195:�˳�, hwnd:%ld, groupid:%d, role:%s",
        hwnd, groupid, roletips[role]);
    return ERRNO_SUCCESS;
}
#endif

