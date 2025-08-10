#include <math.h>
#include <stdlib.h>
#include <process.h>
#include "stdafx.h"
#include "xlog.h"
#include "xcommon.h"
#include "xlist.h"
#include "task.h"
#include "config.h"
#include "alarm.h"
#include "task.h"
#include "task175.h"

#define FONT175_DIR "font\\175\\"
#define BMP175_DIR "bmp\\175\\"

//////////////////////////////////////////////////////////////////////////////
//!!!�޸����ò�������ʼ

//��������"��ʼ���"���������Ӧ
//����1:Ҫ�����xλ�������"�������"��ƫ��,С����Ϊ����,������Ϊ����
//����2:Ҫ�����yλ�������"�������"��ƫ��,С����Ϊ����,������Ϊ����
struct deltaval deltaatpos[] =
{
    {-163, 455}, //��1�����x,yƫ��
    {-245, 462}, //��2�����x,yƫ��
    {-381, 474}, //��3�����x,yƫ��
    {-384, 380}, //��4�����x,yƫ��
    {-410, 353}, //��5�����x,yƫ��
    {-509, 304}, //��6�����x,yƫ��
    {-354, 353}, //��7�����x,yƫ��
    {-275, 308}, //��8�����x,yƫ��
    {-368, 326}, //��9�����x,yƫ��
    {-358, 248}, //��10�����x,yƫ��
    {-442, 166}, //��11�����x,yƫ��
    {-316, 147}, //��12�����x,yƫ��
    {-358, 199}, //��13�����x,yƫ��
};

//��������u���ֲŴ���������Ӧ,��:����9/12/16/18/20/21/22
//����1:Ҫ�����xλ�������"�������"��ƫ��,С����Ϊ����,������Ϊ����
//����2:Ҫ�����yλ�������"�������"��ƫ��,С����Ϊ����,������Ϊ����
struct deltaval deltaatpos_u[] =
{
    {-537, 160}, //��1�����x,yƫ��
    {-186, 229}, //��2�����x,yƫ��
    {-467, 164}, //��3�����x,yƫ��
    {24, 82}, //��4�����x,yƫ��
    {-321, 117}, //��5�����x,yƫ��
    {24, 82}, //��6�����x,yƫ��
    {-406, 89}, //��7�����x,yƫ��
    {-486, 429}, //��9�����x,yƫ��
    {-506, 80}, //��10�����x,yƫ��
};

//�������е�"��ʼ���"��Ӧ
//����1:�ȴ�ʱ��
//����2:���ʱ��
#define task175_fight_atpoint_1() {\
    pirate_fight_time(2, 6),\
}

#define task175_fight_atpoint_2() {\
    pirate_fight_time(2, 6),\
    pirate_fight_time(4, 12),\
}

#define task175_fight_atpoint_3() {\
    pirate_fight_time(2, 5),\
}

#define task175_fight_atpoint_4() {\
        pirate_fight_time(2, 5),\
    }

#define task175_fight_atpoint_5() {\
        pirate_fight_time(2, 5),\
    }

#define task175_fight_atpoint_6() {\
        pirate_fight_time(2, 5),\
    }

#define task175_fight_atpoint_7() {\
    pirate_fight_time(2, 5),\
}

#define task175_fight_atpoint_8() {\
    pirate_fight_time(2, 6),\
}
#define task175_fight_atpoint_9() {\
    pirate_fight_time(2, 6),\
}
#define task175_fight_atpoint_10() {\
    pirate_fight_time(2, 6),\
}
#define task175_fight_atpoint_11() {\
    pirate_fight_time(2, 6),\
}
#define task175_fight_atpoint_12() {\
    pirate_fight_time(2, 6),\
}
#define task175_fight_atpoint_13() {\
    pirate_fight_time(2, 6),\
}

//!!!�޸����ò�����ĩβ
//////////////////////////////////////////////////////////////////////////////

static int fighttimeout(Idmsoft* dmobj, void* ctxt, int waittime, int fighttime)
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
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    api_log_MsgDebug("���:�ȴ�:%d��:��ʼ", waittime);
    
    if (waittime > 0)
    {
        Delay_With_Terminate_Check(waittime, luaman_task, dmobj);
    }

    api_log_MsgDebug("���:�ȴ�:%d��:����", waittime);

    if (fighttime <= 0)
    {
        api_log_MsgDebug("���:%d��:����", fighttime);
        return SNAIL_ERRNO_SUCCESS;
    }
    
    api_log_MsgDebug("���:%d��:��ʼ", fighttime);
    
    lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
            
        dmobj->KeyDownChar("u");
        Delay(20);
        dmobj->KeyUpChar("u");
        Delay(20);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(15);
            dmobj->KeyUpChar("3");
            Delay(15);
        }

        Check_Terminate_Thread(luaman_task, dmobj);

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > (fighttime * 1.0))
        {
            api_log_MsgDebug("���:%d��:���", fighttime);
            break;
        }

        api_log_MsgDebug("���:%d��:����", fighttime);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }
    
    api_log_MsgDebug("���:%d��:����", fighttime);
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint1(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_1();

    api_log_MsgDebug("��1:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��1:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint2(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_2();

    api_log_MsgDebug("��2:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��2:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint3(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_3();

    api_log_MsgDebug("��3:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��3:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint4(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_4();

    api_log_MsgDebug("��4:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��4:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint5(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_5();

    api_log_MsgDebug("��5:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��5:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint6(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_6();

    api_log_MsgDebug("��6:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��6:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatstep9(Idmsoft* dmobj, void* ctxt, int totaltime)
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

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�о�����", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�о�����:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�о�����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�о�����:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�о�����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�о�����:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("�о�����:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�о�����", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�о�����", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�о�����", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("�о�����:���:���");
                    break;
                }
                
                Delay(500);
            }
        }

        api_log_MsgDebug("�о�����:���:����");
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("�׶�9:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint7(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_7();

    api_log_MsgDebug("��7:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��7:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint8(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_8();

    api_log_MsgDebug("��8:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��8:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatstep12(Idmsoft* dmobj, void* ctxt, int totaltime)
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

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ʦ��", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����ʦ��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ʦ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����ʦ��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ʦ��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����ʦ��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("����ʦ��:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ʦ��", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "����ʦ��", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "����ʦ��", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("����ʦ��:���:���");
                    break;
                }
                
                Delay(500);
            }
        }

        api_log_MsgDebug("����ʦ��:���:����");
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("�׶�12:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint9(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_9();

    api_log_MsgDebug("��9:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��9:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint10(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_10();

    api_log_MsgDebug("��10:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��10:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint11(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_11();

    api_log_MsgDebug("��11:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��11:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint12(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_12();

    api_log_MsgDebug("��12:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��12:���:���");
    return SNAIL_ERRNO_SUCCESS;
}


static int fightatpoint13(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = task175_fight_atpoint_13();

    api_log_MsgDebug("��13:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��13:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatstep20(Idmsoft* dmobj, void* ctxt, int totaltime)
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

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("��������:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict(0, FONT175_DIR"font-175MingZhi4.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                dmobj->SetDict(0, FONT175_DIR"font-175MingZhi4.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("��������:���:���");
                    break;
                }
                
                Delay(500);
            }
        }

        api_log_MsgDebug("��������:���:����");
    }

    for (idx = 0; idx < 50; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("�׶�20:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatstep22(Idmsoft* dmobj, void* ctxt, int totaltime)
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

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ĵĳ��", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���ĵĳ��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ĵĳ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���ĵĳ��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ĵĳ��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���ĵĳ��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("���ĵĳ��:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ĵĳ��", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict(0, FONT175_DIR"font-175MingZhi5.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "���ĵĳ��", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                dmobj->SetDict(0, FONT175_DIR"font-175MingZhi5.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "���ĵĳ��", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("���ĵĳ��:���:���");
                    break;
                }
                
                Delay(500);
            }
        }

        api_log_MsgDebug("���ĵĳ��:���:����");
    }

    for (idx = 0; idx < 60; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("�׶�22:���:���");
    return SNAIL_ERRNO_SUCCESS;
}
static int taskatpos1(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[0].deltax;
    int deltaY = deltaatpos[0].deltay;

    api_log_MsgDebug("λ��1:��ʼ");
    
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��1
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:�Ի�:����:����:ʧ��");
        Delay(500);
    }
    
    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("���1:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��1:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int taskatpos2(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[1].deltax;
    int deltaY = deltaatpos[1].deltay;

    api_log_MsgDebug("λ��2:��ʼ");
    
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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


    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc2:�Ի�:����:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc2:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
            api_log_MsgDebug("npc2:�Ի�:����:����:ʧ��");
            Delay(500);
        }

    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("λ��2:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��2:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int taskatpos3(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[2].deltax;
    int deltaY = deltaatpos[2].deltay;

    api_log_MsgDebug("λ��3:��ʼ");
    
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��3
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc3:�Ի�:����:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc3:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        

        api_log_MsgDebug("npc3:�Ի�:����:����:����:ʧ��");
        Delay(500);
    }
    
    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint3(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("λ��3:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��3:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int taskatpos4(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[3].deltax;
    int deltaY = deltaatpos[3].deltay;

    api_log_MsgDebug("λ��4:��ʼ");
     
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��4
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc4:�Ի�:����:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc4:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        

        api_log_MsgDebug("npc4:�Ի�:����:����:����:ʧ��");
        Delay(500);
    }
    
    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint4(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("λ��4:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��4:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int taskatpos5(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[4].deltax;
    int deltaY = deltaatpos[4].deltay;

    api_log_MsgDebug("λ��5:��ʼ");
    
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��5
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc5:�Ի�:����:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc5:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        

        api_log_MsgDebug("npc5:�Ի�:����:����:����:ʧ��");
        Delay(500);
    }
    
    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint5(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("λ��5:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��5:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int taskatpos6(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[5].deltax;
    int deltaY = deltaatpos[5].deltay;

    api_log_MsgDebug("λ��6:��ʼ");
    
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��6
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc6:�Ի�:����:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc6:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        

        api_log_MsgDebug("npc6:�Ի�:����:����:����:ʧ��");
        Delay(500);
    }
    
    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint6(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("λ��6:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��6:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int taskatpos7(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[6].deltax;
    int deltaY = deltaatpos[6].deltay;

    api_log_MsgDebug("λ��7:��ʼ");
    
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��1
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc7:�Ի�:����:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc7:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("npc7:�Ի�:����:����:��Χ,ʧ��");
        Delay(500);
    }
    
    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint7(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("λ��7:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��7:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int taskatpos8(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[7].deltax;
    int deltaY = deltaatpos[7].deltay;

    api_log_MsgDebug("λ��8:��ʼ");
    
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��8
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc8:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc8:�Ի�:����:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc8:�Ի�:����:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc8:�Ի�:����:����:��Χ,ʧ��");
        Delay(500);
    }
    
    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint8(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("λ��8:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��8:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int taskatpos9(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[8].deltax;
    int deltaY = deltaatpos[8].deltay;

    api_log_MsgDebug("λ��9:��ʼ");
    
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��11
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc9:�Ի�:����:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc9:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("npc9:�Ի�:����:����:��Χ,ʧ��");
        Delay(500);
    }
    
    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint9(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("λ��9:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��9:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int taskatpos10(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[9].deltax;
    int deltaY = deltaatpos[9].deltay;

    api_log_MsgDebug("λ��10:��ʼ");
    
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��12
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc10:�Ի�:����:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc10:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        

        api_log_MsgDebug("npc10:�Ի�:����:����:��Χ,ʧ��");
        Delay(500);
    }
    
    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint10(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("λ��10:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��10:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int taskatpos11(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[10].deltax;
    int deltaY = deltaatpos[10].deltay;

    api_log_MsgDebug("λ��11:��ʼ");
    
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��13
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc11:�Ի�:����:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc11:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("npc11:�Ի�:����:����:��Χ,ʧ��");
        Delay(500);
    }
    
    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint11(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("λ��11:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��11:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int taskatpos12(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[11].deltax;
    int deltaY = deltaatpos[11].deltay;

    api_log_MsgDebug("λ��12:��ʼ");
    
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��16
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc12:�Ի�:����:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc12:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("npc12:�Ի�:����:����:��Χ,ʧ��");
        Delay(500);
    }
    
    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint12(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("λ��12:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��12:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int taskatpos13(Idmsoft* dmobj, void* ctxt, int totaltime)
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
    int deltaX = deltaatpos[12].deltax;
    int deltaY = deltaatpos[12].deltay;

    api_log_MsgDebug("λ��13:��ʼ");
    
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��19
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc13:�Ի�:����:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc13:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("npc13:�Ի�:����:����:��Χ,ʧ��");
        Delay(500);
    }

    for (index = 0; index < 2; index++)
    {
        dmobj->KeyDown(51);
        Delay(50);
        dmobj->KeyUp(51);
        Delay(50);
    }
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    
    //���
    code = fightatpoint13(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("λ��13:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("λ��13:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int checkkeystate175(Idmsoft* dmobj, void* ctxt)
{
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

    return ERRNO_SUCCESS;
}

static int checklife175(Idmsoft* dmobj, void* ctxt, int recusive)
{
    return ERRNO_SUCCESS;
}

static int walkpath(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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

    Check_Terminate_Thread(luaman_task, dmobj);
    
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
        LUAMAN_WAIT_MS(200);
        dmobj->MiddleClick();
        LUAMAN_WAIT_MS(200);
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

        api_log_MsgDebug("����:����:�ɹ�,index:%d, tposX:%ld, tposY:%ld", index, tposX, tposY);

        walkstate.yNegKeyDown = 0;
        walkstate.yPosKeyDown = 0;
        walkstate.xPosKeyDown = 0;
        walkstate.xNegKeyDown = 0;

        //Check_Terminate_Thread(luaman_task, dmobj);
        
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
            if (walkTimeDelta > 300.0)
            {
                (void)checkkeystate175(dmobj, ctxt);

                api_log_MsgDebug("Ѱ·:����:��ס,ֹͣѰ·:�ȴ�������ʧ,��һ����, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //�ȴ��ض�������ʧ
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //�ȼ����Ѫ��
            code = checklife175(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("Ѱ·:Ѫ��:���,����������");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //��ȡ��ǰ����
            dmobj->SetDict(0, FONT175_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)checkkeystate175(dmobj, ctxt);
                
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

                (void)checkkeystate175(dmobj, ctxt);
                
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
    //����ͼ�Ƿ��˳�����������
    for (;;)
    {
        //�ȼ����Ѫ��
        code = checklife175(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:��ס:Ѫ��:���:����:����");
            return code;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "ƽ���ӹ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ѱ·:��ס:��ͼ:ƽ���ӹ�:�˳�,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("Ѱ·:��ס:��ͼ:ƽ���ӹ�:�˳�,�����˳���");
    }

    return SNAIL_ERRNO_BLOCKED;
}

static int check175copyfull(Idmsoft* dmobj, void* ctxt)
{
    int status = 0;
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;

    Check_Terminate_Thread(luaman_task, dmobj);
        
    //��鸱�����Ƿ��д�boss,�����������"����޷��볡"
    dmobj->SetDict(0, FONT175_DIR"font-reenter.txt");
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
            dmobj->SetDict(0, FONT175_DIR"font-enter-ack.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;

                api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:�ɹ�, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
                break;
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

            dmobj->SetDict(0, FONT175_DIR"font-enter-ack.txt");
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
        return SNAIL_ERRNO_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //û�н��븱��,Ҳû�г���"�������д�boss����ȥ",�����Ƿ���"�������³���"
    dmobj->SetDict(0, FONT175_DIR"font-copyfull.txt");
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

            dmobj->SetDict(0, FONT175_DIR"font-copyfull.txt");
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
        return SNAIL_ERRNO_AGAIN;;
    }
    
    return SNAIL_ERRNO_SUCCESS;
}

static int exec175task(Idmsoft* dmobj, void* ctxt)
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
    VARIANT pposX3, pposY3;
    VARIANT pposX4, pposY4;
    VARIANT pposX5, pposY5;
    VARIANT pposX6, pposY6;
    VARIANT pposX7, pposY7;
    VARIANT pposX8, pposY8;

    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    long lposX4 = 0, lposY4 = 0;
    long lposX5 = 0, lposY5 = 0;
    long lposX6 = 0, lposY6 = 0;
    long lposX7 = 0, lposY7 = 0;
    long lposX8 = 0, lposY8 = 0;
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
    int deltaX = 0;
    int deltaY = 0;

    int totaltime = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;

    pathofposxys = "15:11";
    pathOfPosXYs = CString(pathofposxys);
    code = walkpath(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[15:11]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[15:11]:�ɹ�");
    
    ///////////////////////////////////////////////////////////////////////////////////////
    //TODO:�������ִ�д���
LABEL_retalk_with_NPC:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->MoveTo(5, 5);
        Delay(500);
        
        dmobj->SetDict(0, FONT175_DIR"font-chckmap.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ʥħ֮צ", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:ʥħ֮צ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:�Ի�:ʥħ֮צ:����:ʧ��");
        Delay(500);
    }

    //���ʥħ֮צ
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT175_DIR"font-enter-ack.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("npc:�Ի�:ȷ��:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("npc:�Ի�:ȷ��:����:ʧ��");
        Delay(500);
    }

    //���ȷ��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT175_DIR"font-enter-ack.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("npc:�Ի�:ȷ��:���:�ɹ�");
            break;
        }

        code = check175copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc:�Ի�:ȷ��:���:�������³�����:���¶Ի�");
            goto LABEL_retalk_with_NPC;
        }
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("npc:�Ի�:ȷ��:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�ȴ�����ʥħ֮צ
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
            
        dmobj->SetDict(0, FONT175_DIR"font-chckmap.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "ʥħ֮צ", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:ʥħ֮צ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:ʥħ֮צ:����:ʧ��");
        Delay(1000);
    }

    //�ӽ�����
    dmobj->MiddleClick();
    LUAMAN_WAIT_MS(1000);
    dmobj->MiddleClick();
    LUAMAN_WAIT_MS(1000);

    //---------------------------------------------------------------------
    //��һ��
    code = taskatpos1(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��1:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����:��1:�ɹ�");
    
    //---------------------------------------------------------------------
    //��2��
    code = taskatpos2(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��2:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����:��2:�ɹ�");

    //---------------------------------------------------------------------
    //��3��
    code = taskatpos3(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��3:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����:��3:�ɹ�");

    //��4��
    code = taskatpos4(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��4:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����:��4:�ɹ�");

    //��5��
    code = taskatpos5(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��5:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����:��5:�ɹ�");

    //��6��
    code = taskatpos6(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��6:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����:��6:�ɹ�");
    
    //��7��
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��7
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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

    //TODO:����ƫ��ֵ
    deltaX = deltaatpos_u[0].deltax;
    deltaY = deltaatpos_u[0].deltay;
    
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //����ɫ�볡
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONT175_DIR"font-175DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����볡", "00ff00-101010", 0.9, &pposX3, &pposY3);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX3 = pposX3.lVal;
            lposY3 = pposY3.lVal;

            api_log_MsgDebug("�Ի�:�����볡:����:�ɹ�, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
            break;
        }

        api_log_MsgDebug("�Ի�:�����볡:����:ʧ��");
        Delay(500);
    }

    //�����ɫ�볡
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->MoveTo(lposX3, lposY3);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT175_DIR"font-175DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����볡", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�Ի�:�����볡:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("�Ի�:�����볡:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�ȴ�������146:67
    for (;;)
    {
        dmobj->SetDict(0, FONT175_DIR"font-posxy.txt");
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
                if (abs(posX - 146) <= 1 && abs(posY - 67) <= 1)
                {
                    api_log_MsgDebug("����:146:67:ƥ��");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("����:146:67:δƥ��");
        Delay(1000);
    }

    totaltime = 10;
    code = fightatstep9(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����9:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����9:�ɹ�");
    
    //��8�� 
    code = taskatpos7(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��7:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����:��7:�ɹ�");
    //��9��

    code = taskatpos8(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��8:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����:��8:�ɹ�");
    
    //��10�� 
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��10
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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

    //TODO:����ƫ��ֵ
    deltaX = deltaatpos_u[1].deltax;
    deltaY = deltaatpos_u[1].deltay;
    
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //����ɫ�볡
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����볡", "00ff00-101010", 0.9, &pposX4, &pposY4);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX4 = pposX4.lVal;
            lposY4 = pposY4.lVal;

            api_log_MsgDebug("�Ի�:�����볡:����:��Χ,�ɹ�, lposX4:%ld, lposY4:%ld", lposX4, lposY4);
            break;
        }

        api_log_MsgDebug("�Ի�:�����볡:����:��Χ,ʧ��");
        Delay(500);
    }

    //�����ɫ�볡
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->MoveTo(lposX4, lposY4);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT175_DIR"font-175DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����볡:", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�Ի�:�����볡:����:��Χ,�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("�Ի��������볡:����:��Χ,ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�ȴ�������206:415
    for (;;)
    {
        dmobj->SetDict(0, FONT175_DIR"font-posxy.txt");
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
                if (abs(posX - 206) <= 1 && abs(posY - 415) <= 1)
                {
                    api_log_MsgDebug("����:206:415:ƥ��");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("����:206:415:δƥ��");
        Delay(1000);
    }

    totaltime = 10;
    code = fightatstep12(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����12:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����12:�ɹ�");
    
    //��11�� 
    code = taskatpos9(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��9:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����:��9:�ɹ�");
    
    //��12�� 
    code = taskatpos10(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��10:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����:��10:�ɹ�");
    
    //��13�� 
    code = taskatpos11(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��11:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����:��11:�ɹ�");
    
    //��14��

Label_retalk_at_NPC_16:
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��14
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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

    //TODO:����ƫ��ֵ
    deltaX = deltaatpos_u[2].deltax;
    deltaY = deltaatpos_u[2].deltay;
    
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    dmobj->MiddleClick();
    LUAMAN_WAIT_MS(500);
    dmobj->MiddleClick();
    LUAMAN_WAIT_MS(500);

    //�һ�ɫ��������ݸ�
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ݸ�", "ffff00-101010", 0.9, &pposX5, &pposY5);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX5 = pposX5.lVal;
            lposY5 = pposY5.lVal;

            api_log_MsgDebug("������ݸ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX5, lposY5);
            break;
        }

        api_log_MsgDebug("������ݸ�:����:ʧ��");
        Delay(500);
    }

    deltaX = deltaatpos_u[3].deltax;
    deltaY = deltaatpos_u[3].deltay;
    
    //�����ɫ��������ݸ�
    int idx = 0;
    for (idx = 0; idx < 1; idx++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->MoveTo(lposX5 + deltaX, lposY5 + deltaY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ݸ�", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("������ݸ�:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("������ݸ�:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        goto Label_retalk_at_NPC_16;
    }
    
    //��15��
    code = taskatpos12(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��12:ʧ��:code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("����:��12:�ɹ�");
    
    //��16��
Label_retalk_at_NPC_18:
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��17
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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

    //TODO:����ƫ��ֵ
    deltaX = deltaatpos_u[4].deltax;
    deltaY = deltaatpos_u[4].deltay;
    
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    dmobj->MiddleClick();
    LUAMAN_WAIT_MS(500);
    dmobj->MiddleClick();
    LUAMAN_WAIT_MS(500);

    //�һ�ɫ��������ݸ�
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ݸ�", "ffff00-101010", 0.9, &pposX5, &pposY5);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX5 = pposX5.lVal;
            lposY5 = pposY5.lVal;

            api_log_MsgDebug("������ݸ�:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX5, lposY5);
            break;
        }

        api_log_MsgDebug("������ݸ�:����:��Χ,ʧ��");
        Delay(500);
    }
    deltaX = deltaatpos_u[5].deltax;
    deltaY = deltaatpos_u[5].deltay;
    

    //�����ɫ��������ݸ�
    for (idx = 0; idx < 1; idx++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->MoveTo(lposX5, lposY5);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ݸ�", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("������ݸ�:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("������ݸ�:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        goto Label_retalk_at_NPC_18;
    }
    
    //��17��
    code = taskatpos13(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:��13:ʧ��:code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("����:��13:�ɹ�");
    
    //��18��
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��20
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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

    //TODO:����ƫ��ֵ
    deltaX = deltaatpos_u[6].deltax;
    deltaY = deltaatpos_u[6].deltay;
    
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //����ɫ���������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175DuiHua3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���������", "00ff00-101010", 0.9, &pposX7, &pposY7);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX7 = pposX7.lVal;
            lposY7 = pposY7.lVal;

            api_log_MsgDebug("�Ի�:���������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX7, lposY7);
            break;
        }

        api_log_MsgDebug("�Ի�:���������:����:ʧ��");
        Delay(500);
    }

    //�����ɫ���������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->MoveTo(lposX7, lposY7);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT175_DIR"font-175DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���������", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���������:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("���������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�ȴ�������68:202
    for (;;)
    {
        dmobj->SetDict(0, FONT175_DIR"font-posxy.txt");
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
                if (abs(posX - 68) <= 1 && abs(posY - 202) <= 1)
                {
                    api_log_MsgDebug("����:68:202:ƥ��");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("����:68:202:δƥ��");
        Delay(1000);
    }

    
    totaltime = 10;
    code = fightatstep20(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����20:ʧ��:code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("����20:�ɹ�");
    
    //��20��
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��22
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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

    //TODO:����ƫ��ֵ
    deltaX = deltaatpos_u[7].deltax;
    deltaY = deltaatpos_u[7].deltay;
    
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
    
    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    totaltime = 10;
    code = fightatstep22(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����22:ʧ��:code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("����22:�ɹ�");
    
    //��21��
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��22
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONT175_DIR"font-vip.txt");
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

    //TODO:����ƫ��ֵ
    deltaX = deltaatpos_u[8].deltax;
    deltaY = deltaatpos_u[8].deltay;
    
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
    
    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);
    
    Delay_With_Terminate_Check(2, luaman_task, dmobj);
    
    //����ɫ���븱��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���븱��", "00ff00-101010", 0.9, &pposX8, &pposY8);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX8 = pposX8.lVal;
            lposY8 = pposY8.lVal;

            api_log_MsgDebug("�Ի�:���븱��:����:�ɹ�, lposX8:%ld, lposY8:%ld", lposX8, lposY8);
            break;
        }

        api_log_MsgDebug("�Ի�:���븱��:����:ʧ��");
        Delay(500);
    }

    //�����ɫ���븱��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->MoveTo(lposX8, lposY8);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT175_DIR"font-175DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���븱��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�Ի�:���븱��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�Ի�:���븱��:����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    Delay(100);

    //�ȴ����Ͻǵ�ͼ���ֱ�ɵ�����˹Ұ��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT175_DIR"font-chckmap.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "������˹Ұ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:������˹Ұ��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Delay(1000);
        api_log_MsgDebug("��ͼ:������˹Ұ��:����:ʧ��");
    }

    //��p���˳�����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("p");
        Delay(500);
        dmobj->KeyUpChar("p");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONT175_DIR"font-auxiend.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + 5, lposY + 5);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT175_DIR"font-auxiend.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT175_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONT175_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��������:����:ʧ��");
        Delay(500);
    }

    //�����������
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
    
        dmobj->SetDict(0, FONT175_DIR"font-auxiend.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONT175_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //����p
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONT175_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONT175_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�˳�����:�ر�:�ɹ�");
                break;
            }
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�˳�����:�ر�:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    
    
    ///////////////////////////////////////////////////////////////////////////////////////
    api_log_MsgDebug("����:175:����:ִ��:�ɹ�");
    return ERRNO_SUCCESS;
}

static int exec175taskloop(Idmsoft* dmobj, void* ctxt)
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

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        exec175task(dmobj, ctxt);
    }
    
    return ERRNO_SUCCESS;
}

int api_Task175Init(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;

    api_log_MsgDebug("����:175:��ʼ��");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);

    api_PrintConfig(luaman_config);
    
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("����:175:����:��:ʧ��, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("����:175:����:��:�ɹ�, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("����:���:����:ʧ��, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("����:���:����:�ɹ�, hwnd:%ld", hwnd);
    
    return ERRNO_SUCCESS;
}

int api_Task175Main(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    
    api_log_MsgDebug("����:175:��ѭ��:��ʼ: hwnd:%ld, role:%d", hwnd, (int)role);

    exec175taskloop(dmobj, ctxt);
    
    api_log_MsgDebug("����:175:��ѭ��:���, hwnd:%ld", hwnd);
    
    return ERRNO_SUCCESS;
}

int api_Task175Exit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;

    api_log_MsgDebug("����:175:����: hwnd:%ld, role:%d", hwnd, (int)role);

    return ERRNO_SUCCESS;
}
