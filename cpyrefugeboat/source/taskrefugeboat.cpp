#include <math.h>
#include <stdlib.h>
#include <process.h>
#include <direct.h>
#include "stdafx.h"
//#include "xrrno.h"
#include "xlog.h"
#include "xcommon.h"
#include "xlist.h"
#include "task.h"
#include "config.h"
#include "alarm.h"
#include "task.h"
#include "taskrefugeboat.h"

#define FONT_REFUGEBOAT_DIR "font\\refugeboat\\"
#define BMP_REFUGEBOAT_DIR "bmp\\refugeboat\\"

static HANDLE g_Tz_refugeboat_criticalSection;

static struct luaman_posxy deltaatpos_refugeboat[] = 
{
    {-467, 152},
    {-485, 139},
    {-500, 142},
    {-406, 294},
    //{0, 0},
};

void TzRefugeboat_InitCriticalSection()
{
    g_Tz_refugeboat_criticalSection = ::CreateMutex(NULL, FALSE, NULL);
    if (NULL == g_Tz_refugeboat_criticalSection)
    {
        api_log_MsgError("create g_Tz_refugeboat_criticalSection lock failed");
        exit(1);
    }
    return;
}

static void TzRefugeboatEnterCriticalSection()
{
    ::WaitForSingleObject(g_Tz_refugeboat_criticalSection, INFINITE);
    return;
}

static void TzRefugeboatLeaveCriticalSection()
{
    (void)::ReleaseMutex(g_Tz_refugeboat_criticalSection);
    return;
}

static void TzRefugeboatDeleteCriticalSection()
{
    (void)::CloseHandle(g_Tz_refugeboat_criticalSection);
    return;
}

static int refugeboat_invite_join(Idmsoft* dmobj, void* ctxt)
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
    
    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

    lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyDownChar("f");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f");
        LUAMAN_WAIT_MS(200);

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-inviteheil2team-1.txt");
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

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        api_log_MsgDebug("���:����:��Ӻ���:����:��Χ,ʧ��");
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(200);
    
    //����'ս������'����
    lastChckTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-inviteheil2team-1.txt");
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
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            posX = fellowPosXYs[i].x;
            posY = fellowPosXYs[i].y;
            
            api_log_MsgDebug("i:%d, posX:%ld, posY:%ld", i, posX, posY);

            lastChckTime = time(NULL);

            //�����Ա����,��������Ի���
            for (;;)
            {
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                
                dmobj->MoveTo(posX, posY);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);

                dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-inviteheil2team-1.txt");
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

                api_log_MsgDebug("���:����:��Ա:����:����,ʧ��");
                Delay(500);
            }

            //���'����'������Ѽ������
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);

            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        }

        syncCount = 0;

        //@bug:�Ե�Ƭ��,�����������˷�ʱ��2019/4/8 23:06:19
        Delay(2000);

        api_log_MsgDebug("ͬ��:����:���");
        
        //���'ͬ��'����
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            posX1 = syncPosXYs[2 * i].x;
            posY1 = syncPosXYs[2 * i].y;
            posX2 = syncPosXYs[2 * i + 1].x;
            posY2 = syncPosXYs[2 * i + 1].y;

            api_log_MsgDebug("(%ld, %ld) --> (%ld, %ld)", posX1, posY1, posX2, posY2);

            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-inviteheil2team-1.txt");
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

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        api_log_MsgDebug("���:����:��Ա:����,���δ���,syncCount:%d", syncCount);
    }

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    api_log_MsgDebug("������:�ɹ�:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int refugeboat_exit_game(Idmsoft* dmobj, void* ctxt)
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
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyDownChar("esc");
        Delay(500);
        dmobj->KeyUpChar("esc");
        Delay(500);

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-exitgame2.txt");
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
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-exitgame2.txt");
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

static int refugeboat_fight_phase1(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    const char* fontcolors[] = {"ffffff-101010", "00ff00-101010", "ff3300-101010"};
    const char* fontcolor = fontcolors[0];
    int flag = 0;
    
    api_log_MsgDebug("��1::���:��ʼ");

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-tree.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[0];
            
            api_log_MsgDebug("����:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-tree.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[1];
            
            api_log_MsgDebug("����:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-tree.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[2];
            
            api_log_MsgDebug("����:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("����:����:ʧ��");
        Delay(500);
    }

    //���²��˼�
    dmobj->KeyDownChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);
    
    dmobj->KeyDownChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);
    
    //��ʼ���
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(15);
            dmobj->KeyUpChar("3");
            Delay(15);
        }

        if (!flag)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->KeyPressChar("u");
            Delay(20);
            dmobj->KeyUpChar("u");
            Delay(20);

            for (;;)
            {
                dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc2.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 105, "�µ���", "ffffff-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    fontcolor = fontcolors[0];
                    flag = 1;
                    
                    api_log_MsgDebug("u��:�µ���:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc2.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 105, "�µ���", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    fontcolor = fontcolors[1];
                    flag = 1;
                    
                    api_log_MsgDebug("u��:�µ���:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc2.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 105, "�µ���", "ff3300-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    fontcolor = fontcolors[2];
                    flag = 1;
                    
                    api_log_MsgDebug("u��:�µ���:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }
                
                break;
            }

            if (!flag)
            {
                api_log_MsgDebug("u��:�µ���:����");
                continue;
            }

            //dmobj->Capture(0, 27, 1279, 826, "cap\\aodysay.bmp");
        }

        //�鿴����û��
        for (;;)
        {
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 105, "�µ���", fontcolor, 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                flag = 2;
                
                api_log_MsgDebug("u��:�µ���:%s:���:�ɹ�", fontcolor);
                break;
            }

            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("u��:�µ���:%s:���:����, lposX:%ld, lposY:%ld",
                fontcolor, lposX, lposY);
            
            break;
        }

        if (2 == flag)
        {
            api_log_MsgDebug("u��:�µ���:%s:���:�˳�", fontcolor);
            break;
        }
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("��1::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int refugeboat_fight_phase2(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    api_log_MsgDebug("��2:���:��ʼ");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������˹��ĹѨ", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("������˹��ĹѨ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (240 * 1.0))
        {
            api_log_MsgDebug("������˹��ĹѨ:���:��ʱ");
            break;
        }
        
        api_log_MsgDebug("������˹��ĹѨ:����:ʧ��");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("��2:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int refugeboat_fight_phase3(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    const char* fontcolors[] = {"ffffff-101010", "00ff00-101010", "ff3300-101010"};
    const char* fontcolor = fontcolors[0];
    int flag = 0;
    
    api_log_MsgDebug("��3::���:��ʼ");

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 105, "������˹", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[0];
            
            api_log_MsgDebug("������˹:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 105, "������˹", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[1];
            
            api_log_MsgDebug("������˹:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 105, "������˹", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[2];
            
            api_log_MsgDebug("������˹:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("������˹:����:ʧ��");
        Delay(500);
    }

    //��ʼ���
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(15);
            dmobj->KeyUpChar("3");
            Delay(15);
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc7.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����ȴ�", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("u��:������˹:%s:���:�����ȴ�:����:�ɹ�, lposX:%ld, lposY:%ld",
                fontcolor, lposX, lposY);
            break;
        }

        api_log_MsgDebug("u��:������˹:%s:���:�����ȴ�:����:����", fontcolor);
    }

    /*for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }*/

    api_log_MsgDebug("��3::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int refugeboat_loop_master(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_refugeboat[0].x;
    int deltaY = deltaatpos_refugeboat[0].y;
    
    for (;;)
    {
    COPY_LABEL_AGAIN:
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        sleepTimesj = 0;

        //��ӿ�ʼ
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            //���
            api_log_MsgDebug("�ٽ���:����:�ȴ�");
            TzRefugeboatEnterCriticalSection();
            api_log_MsgDebug("�ٽ���:����:�ɹ�");

            for (;;)
            {
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                code = refugeboat_invite_join(dmobj, ctxt);
                if (SNAIL_ERRNO_AGAIN != code)
                {
                    api_log_MsgDebug("���:���");
                    break;
                }

            }

            api_log_MsgDebug("�ٽ���:�˳�:�ȴ�");
            TzRefugeboatLeaveCriticalSection();
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
                    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                    
                    dmobj->KeyDownChar("esc");
                    Delay(500);
                    dmobj->KeyUpChar("esc");
                    Delay(500);

                    dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-exitgame1.txt");
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
                    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                    
                    dmobj->MoveTo(lposX, lposY);
                    Delay(500);
                    dmobj->LeftClick();
                    Delay(500);

                    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                    
                    dmobj->MoveTo(5, 5);
                    Delay(500);
                
                    dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-exitgame2.txt");
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
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
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
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                if (sleepTimesj > blockTimes * 60)
                {
                    break;
                }

                Delay(1000);
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                
                sleepTimesj++;
                api_log_MsgDebug("���:����:ʧ��,˯��һ�������, sleepTimesj:%d, blockTimes:%ld", sleepTimesj, blockTimes);
            }
        }//��ӽ���

        copyFullCounter = 0;
        
    COPY_LABEL_TALK_WITH_NPC:
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        //��ʼִ�и�������
        //��n�Ի����븱��
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->KeyPressChar("n");
            Delay(500);
            dmobj->KeyUpChar("n");
            Delay(500);

            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            dmobj->MoveTo(5, 5);
            Delay(200);
            
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc1.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "���Ѵ��볡", "00ff00-000000", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("npc1:�Ի�:���������Ѵ��볡:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("npc1:�Ի�:���������Ѵ��볡:����:ʧ��");
            Delay(500);
        }

        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);

            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            dmobj->MoveTo(5, 5);
            Delay(200);

            //����Ƿ����'ȷ��'�����ĶԻ���
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-okcancel.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
            
                api_log_MsgDebug("npc1:�Ի�:ȷ��:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }
            
            api_log_MsgDebug("npc1:�Ի�:ȷ��:����:ʧ��");
            Delay(500);
        }

        //���'ȷ��'�����Ի���
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX2, lposY2);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);

            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            dmobj->MoveTo(5, 5);
            Delay(200);

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug: ����еض����������,���ȷ��;������ǵ�ȷ�϶�Ӧ��ȡ�� 2020/12/27 14:02:18
            //����еض����������,���ȵ�� 2019/7/19 20:47:59
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-copyfull.txt");
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
                    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                    
                    dmobj->MoveTo(lposX + 55, lposY + 100);
                    Delay(200);
                    dmobj->LeftClick();
                    Delay(200);
                    dmobj->MoveTo(5, 5);
                    Delay(200);

                    dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-copyfull.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("ԭ�ظ���:�������³�����:���:�ɹ�");
                        break;
                    }

                    api_log_MsgDebug("ԭ�ظ���:�������³�����:���:ʧ��");
                    
                    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                    Delay(500);
                }

                //@bug:�������³�����������5�����˳���Ϸ 2019/5/19 23:08:15
                if (copyFullCounter > 5)
                {
                    if (1 == exitGameCopyFull)
                    {
                        refugeboat_exit_game(dmobj, ctxt);
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
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-cantenter.txt");
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
                    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                    
                    dmobj->MoveTo(lposX + 56, lposY + 102);
                    Delay(200);
                    dmobj->LeftClick();
                    Delay(200);
                    dmobj->MoveTo(5, 5);
                    Delay(200);

                    dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-cantenter.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "���������볡�����볡", "ffffff-202020", 0.9, &pposX2, &pposY2);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("���������볡�����볡:���:�ɹ�");
                        break;
                    }

                    api_log_MsgDebug("���������볡�����볡:���:ʧ��");
                    
                    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                    Delay(500);
                }

                api_log_MsgDebug("npc1:�Ի�:���������볡�����볡:���¶Ի�");
                goto COPY_LABEL_TALK_WITH_NPC;
            }
            /////////////////////////////////////////////////////////////////////////////

            //����Ƿ���븱��
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-maptips.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "���������Ѵ�", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("npc1:�Ի�:���������Ѵ�:����,�ɹ�:lposX:%ld,lposY:%ld", lposX, lposY);
                break;
            }

            //////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:���"��Ա����" 2022/03/15 22:02:55
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-peopleleak.txt");
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
                    
                    dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-peopleleak.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ա����", "ffffff-202020", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("��Ա����:���:�ɹ�:���¶Ի�");
                        goto COPY_LABEL_AGAIN;
                    }

                    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
                    api_log_MsgDebug("npc1:�Ի�:���������Ѵ�,���ڽ���");
                    Delay(500);
                }
                
            }
            //////////////////////////////////////////////////////////////////////////////////////////////////////
            
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            api_log_MsgDebug("npc1:�Ի�:���������Ѵ�,���ڽ���");
            Delay(500);
        }

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        //���
        code = refugeboat_fight_phase1(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("���:��1:ʧ��, code:%d", code);
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("���:��1:�ɹ�");

        dmobj->KeyDownChar("-");
        Delay(300);
        dmobj->KeyUpChar("-");
        Delay(300);
        
        Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

        //��M���ҹ����������λ��1
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->MoveTo(5, 5);
            Delay(100);
            dmobj->KeyDownChar("m");
            Delay(300);
            dmobj->KeyUpChar("m");
            Delay(300);

            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-vip.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("�������:����:ʧ��");
            Delay(500);
        }

        deltaX = deltaatpos_refugeboat[0].x;
        deltaY = deltaatpos_refugeboat[0].y;
        dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->KeyDownChar("-");
        Delay(300);
        dmobj->KeyUpChar("-");
        Delay(300);

        Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->KeyPressChar("n");
            Delay(500);
            dmobj->KeyUpChar("n");
            Delay(500);

            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc3.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�������", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("�ƶ�������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("�ƶ�������:����:ʧ��");
            Delay(500);
        }

        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(200);
            
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc3.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�������", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�ƶ�������:���:�ɹ�");
                break;
            }
            
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�ƶ�������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            Delay(500);
        }

        dmobj->KeyDownChar("-");
        Delay(300);
        dmobj->KeyUpChar("-");
        Delay(300);

        Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);
    
        //��M���ҹ����������λ��1
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->MoveTo(5, 5);
            Delay(100);
            dmobj->KeyDownChar("m");
            Delay(300);
            dmobj->KeyUpChar("m");
            Delay(300);

            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-vip.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("�������:����:ʧ��");
            Delay(500);
        }

        deltaX = deltaatpos_refugeboat[1].x;
        deltaY = deltaatpos_refugeboat[1].y;
        dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->KeyDownChar("-");
        Delay(300);
        dmobj->KeyUpChar("-");
        Delay(300);

        Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        Delay_With_Terminate_Check_Alram(120, luaman_task, dmobj);

        code = refugeboat_fight_phase2(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("���:��2:ʧ��, code:%d", code);
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("���:��2:�ɹ�");
        
        dmobj->KeyDownChar("-");
        Delay(300);
        dmobj->KeyUpChar("-");
        Delay(300);

        Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);
    
        //��M���ҹ����������λ��1
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->MoveTo(5, 5);
            Delay(100);
            dmobj->KeyDownChar("m");
            Delay(300);
            dmobj->KeyUpChar("m");
            Delay(300);

            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-vip.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("�������:����:ʧ��");
            Delay(500);
        }

        deltaX = deltaatpos_refugeboat[2].x;
        deltaY = deltaatpos_refugeboat[2].y;
        dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->KeyDownChar("-");
        Delay(300);
        dmobj->KeyUpChar("-");
        Delay(300);

        Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->KeyPressChar("n");
            Delay(500);
            dmobj->KeyUpChar("n");
            Delay(500);

            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc5.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "��˹�İ�Ϣ��", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("��˹�İ�Ϣ��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("��˹�İ�Ϣ��:����:ʧ��");
            Delay(500);
        }

        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(200);
            
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc5.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "��˹�İ�Ϣ��", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("��˹�İ�Ϣ��:���:�ɹ�");
                break;
            }
            
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("��˹�İ�Ϣ��:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            Delay(500);
        }

        dmobj->KeyDownChar("-");
        Delay(300);
        dmobj->KeyUpChar("-");
        Delay(300);

        Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        //��M���ҹ����������λ��1
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->MoveTo(5, 5);
            Delay(100);
            dmobj->KeyDownChar("m");
            Delay(300);
            dmobj->KeyUpChar("m");
            Delay(300);

            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-vip.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("�������:����:ʧ��");
            Delay(500);
        }

        deltaX = deltaatpos_refugeboat[3].x;
        deltaY = deltaatpos_refugeboat[3].y;
        dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->KeyDownChar("-");
        Delay(300);
        dmobj->KeyUpChar("-");
        Delay(300);

        Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        code = refugeboat_fight_phase3(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("���:��3:ʧ��, code:%d", code);
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("���:��3:�ɹ�");

        Delay_With_Terminate_Check_Alram(2, luaman_task, dmobj);

        dmobj->KeyDownChar("0");
        Delay(300);
        dmobj->KeyUpChar("0");
        Delay(300);

        Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->KeyPressChar("n");
            Delay(500);
            dmobj->KeyUpChar("n");
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(200);
            
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc8.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ���������", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("�ƶ���������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("�ƶ���������:����:ʧ��");
            Delay(500);
        }

        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(200);
            
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc8.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ���������", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�ƶ���������:���:�ɹ�");
                break;
            }
            
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�ƶ���������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            Delay(500);
        }
        
        //�ȴ���ͼ
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-maptips.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "������˹Ұ��", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("��ͼ:������˹Ұ��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("��ͼ:������˹Ұ��:����:ʧ��");
            Delay(1000);
        }

        //��p���˳�����
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->KeyDownChar("p");
            Delay(500);
            dmobj->KeyUpChar("p");
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(200);
            
            dmobj->SetDict( 0, FONT_REFUGEBOAT_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("�˳�����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("�˳�����:����:ʧ��");
            Delay(500);
        }

        //���
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(200);

            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;

                api_log_MsgDebug("ȷ��:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }

            api_log_MsgDebug("ȷ��:����:ʧ��");
            Delay(500);
        }

        //���ȷ��
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX2, lposY2);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(200);

            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("ȷ��:���:�ɹ�");
                break;
            }

            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("ȷ��:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            Delay(500);
        }

        api_log_MsgDebug("����:ִ��:���:����:��һ��");
    }//COPY_LABEL_AGAIN

    api_log_MsgDebug("����:����:ִ��:����");
    return ERRNO_SUCCESS;
}

static int refugeboat_join_team(Idmsoft* dmobj, void* ctxt)
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

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        //�ж��Ƿ����"�������"
        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-1.txt");
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
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX - 32, lposY + 126);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-2.txt");
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
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���������Ѵ�", "909090-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���:����:���븱��:��ɫ:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���������Ѵ�", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���:����:���븱��:��ɫ:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���������Ѵ�", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���:����:���븱��:��ɫ:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
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
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-3.txt");
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
        
        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-3.txt");
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
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-4.txt");
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
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyDownChar("enter");
        Delay(500);
        dmobj->KeyUpChar("enter");
        Delay(500);

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-4.txt");
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


int api_TaskRefugeboatInit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;

    api_log_MsgDebug("����:refugeboat:��ʼ��");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);

    api_PrintConfig(luaman_config);
    
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("����:refugeboat:����:��:ʧ��, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("����:refugeboat:����:��:�ɹ�, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("����:refugeboat:����:���:����:ʧ��, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }
    
    api_log_MsgDebug("����:refugeboat:����:���:����:�ɹ�, hwnd:%ld", hwnd);

    /* ��ʼ���澯ʱ���ʼֵ */
    init_alarm_ctxt();
    
    return ERRNO_SUCCESS;
}

int api_TaskRefugeboatMain(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    
    api_log_MsgDebug("����:refugeboat:��ѭ��: hwnd:%ld, role:%d", hwnd, (int)role);

    switch (role)
    {
    case ROLE_M:
        //@bug:�ض�������ʱ���Ž�����Ϸ:exitGameCopyFull 2019/5/20 21:42:16
        api_log_MsgDebug("����:refugeboat:����:��ѭ��:��ʼ, hwnd:%ld", hwnd);
        refugeboat_loop_master(dmobj, ctxt);
        api_log_MsgDebug("����:refugeboat:����:��ѭ��:���, hwnd:%ld", hwnd);
        break;

    default:
        for (;;)
        {
            api_log_MsgDebug("����:refugeboat:����:��ѭ��:��ʼ, hwnd:%ld", hwnd);

            refugeboat_join_team(dmobj, ctxt);
            
            api_log_MsgDebug("����:refugeboat:����:��ѭ��:���, hwnd:%ld", hwnd);
        }
        
        break;
    }
    
    return ERRNO_SUCCESS;
}

int api_TaskRefugeboatExit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;

    api_log_MsgDebug("����:refugeboat:����: hwnd:%ld, role:%d", hwnd, (int)role);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
    
    return ERRNO_SUCCESS;
}

