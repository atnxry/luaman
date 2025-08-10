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
//!!!修改配置参数区开始

//与流程中"开始打怪"的流程相对应
//参数1:要点击的x位置相对于"贵宾服务"的偏移,小于则为负数,大于则为正数
//参数2:要点击的y位置相对于"贵宾服务"的偏移,小于则为负数,大于则为正数
struct deltaval deltaatpos[] =
{
    {-163, 455}, //第1个点的x,y偏移
    {-245, 462}, //第2个点的x,y偏移
    {-381, 474}, //第3个点的x,y偏移
    {-384, 380}, //第4个点的x,y偏移
    {-410, 353}, //第5个点的x,y偏移
    {-509, 304}, //第6个点的x,y偏移
    {-354, 353}, //第7个点的x,y偏移
    {-275, 308}, //第8个点的x,y偏移
    {-368, 326}, //第9个点的x,y偏移
    {-358, 248}, //第10个点的x,y偏移
    {-442, 166}, //第11个点的x,y偏移
    {-316, 147}, //第12个点的x,y偏移
    {-358, 199}, //第13个点的x,y偏移
};

//与流程中u到怪才打的流程相对应,即:步骤9/12/16/18/20/21/22
//参数1:要点击的x位置相对于"贵宾服务"的偏移,小于则为负数,大于则为正数
//参数2:要点击的y位置相对于"贵宾服务"的偏移,小于则为负数,大于则为正数
struct deltaval deltaatpos_u[] =
{
    {-537, 160}, //第1个点的x,y偏移
    {-186, 229}, //第2个点的x,y偏移
    {-467, 164}, //第3个点的x,y偏移
    {24, 82}, //第4个点的x,y偏移
    {-321, 117}, //第5个点的x,y偏移
    {24, 82}, //第6个点的x,y偏移
    {-406, 89}, //第7个点的x,y偏移
    {-486, 429}, //第9个点的x,y偏移
    {-506, 80}, //第10个点的x,y偏移
};

//与流程中的"开始打怪"对应
//参数1:等待时间
//参数2:打怪时间
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

//!!!修改配置参数区末尾
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
    
    api_log_MsgDebug("打怪:等待:%d秒:开始", waittime);
    
    if (waittime > 0)
    {
        Delay_With_Terminate_Check(waittime, luaman_task, dmobj);
    }

    api_log_MsgDebug("打怪:等待:%d秒:结束", waittime);

    if (fighttime <= 0)
    {
        api_log_MsgDebug("打怪:%d秒:跳过", fighttime);
        return SNAIL_ERRNO_SUCCESS;
    }
    
    api_log_MsgDebug("打怪:%d秒:开始", fighttime);
    
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
            api_log_MsgDebug("打怪:%d秒:完成", fighttime);
            break;
        }

        api_log_MsgDebug("打怪:%d秒:继续", fighttime);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }
    
    api_log_MsgDebug("打怪:%d秒:结束", fighttime);
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

    api_log_MsgDebug("点1:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点1:打怪:完成");
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

    api_log_MsgDebug("点2:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点2:打怪:完成");
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

    api_log_MsgDebug("点3:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点3:打怪:完成");
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

    api_log_MsgDebug("点4:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点4:打怪:完成");
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

    api_log_MsgDebug("点5:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点5:打怪:完成");
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

    api_log_MsgDebug("点6:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点6:打怪:完成");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "研究所长", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("研究所长:红色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "研究所长", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("研究所长:白色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "研究所长", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("研究所长:绿色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("研究所长:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "研究所长", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "研究所长", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "研究所长", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("研究所长:打怪:完成");
                    break;
                }
                
                Delay(500);
            }
        }

        api_log_MsgDebug("研究所长:打怪:继续");
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("阶段9:打怪:完成");
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

    api_log_MsgDebug("点7:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点7:打怪:完成");
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

    api_log_MsgDebug("点8:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点8:打怪:完成");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "咒术师艾", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("咒术师艾:红色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "咒术师艾", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("咒术师艾:白色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "咒术师艾", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("咒术师艾:绿色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("咒术师艾:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "咒术师艾", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "咒术师艾", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                dmobj->SetDict(0, FONT175_DIR"font-175MingZhi2.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "咒术师艾", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("咒术师艾:打怪:完成");
                    break;
                }
                
                Delay(500);
            }
        }

        api_log_MsgDebug("咒术师艾:打怪:继续");
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("阶段12:打怪:完成");
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

    api_log_MsgDebug("点9:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点9:打怪:完成");
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

    api_log_MsgDebug("点10:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点10:打怪:完成");
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

    api_log_MsgDebug("点11:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点11:打怪:完成");
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

    api_log_MsgDebug("点12:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点12:打怪:完成");
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

    api_log_MsgDebug("点13:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点13:打怪:完成");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贝尔纳帕", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贝尔纳帕:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贝尔纳帕", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贝尔纳帕:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贝尔纳帕", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贝尔纳帕:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("贝尔纳帕:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贝尔纳帕", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict(0, FONT175_DIR"font-175MingZhi4.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "贝尔纳帕", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                dmobj->SetDict(0, FONT175_DIR"font-175MingZhi4.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "贝尔纳帕", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("贝尔纳帕:打怪:完成");
                    break;
                }
                
                Delay(500);
            }
        }

        api_log_MsgDebug("贝尔纳帕:打怪:继续");
    }

    for (idx = 0; idx < 50; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("阶段20:打怪:完成");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "内心的丑恶", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("内心的丑恶:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "内心的丑恶", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("内心的丑恶:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "内心的丑恶", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("内心的丑恶:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("内心的丑恶:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "内心的丑恶", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict(0, FONT175_DIR"font-175MingZhi5.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "内心的丑恶", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                dmobj->SetDict(0, FONT175_DIR"font-175MingZhi5.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "内心的丑恶", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("内心的丑恶:打怪:完成");
                    break;
                }
                
                Delay(500);
            }
        }

        api_log_MsgDebug("内心的丑恶:打怪:继续");
    }

    for (idx = 0; idx < 60; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("阶段22:打怪:完成");
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

    api_log_MsgDebug("位置1:开始");
    
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置1
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:对话:树藤:查找:失败");
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
    
    //打怪
    code = fightatpoint1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("打怪1:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置1:完成");
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

    api_log_MsgDebug("位置2:开始");
    
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置2
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc2:对话:树藤:找字:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc2:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
            api_log_MsgDebug("npc2:对话:树藤:查找:失败");
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
    
    //打怪
    code = fightatpoint2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("位置2:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置2:完成");
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

    api_log_MsgDebug("位置3:开始");
    
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置3
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc3:对话:树藤:找字:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc3:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        

        api_log_MsgDebug("npc3:对话:树藤:找字:查找:失败");
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
    
    //打怪
    code = fightatpoint3(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("位置3:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置3:完成");
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

    api_log_MsgDebug("位置4:开始");
     
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置4
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc4:对话:树藤:找字:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc4:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        

        api_log_MsgDebug("npc4:对话:树藤:找字:查找:失败");
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
    
    //打怪
    code = fightatpoint4(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("位置4:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置4:完成");
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

    api_log_MsgDebug("位置5:开始");
    
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置5
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc5:对话:树藤:找字:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc5:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        

        api_log_MsgDebug("npc5:对话:树藤:找字:查找:失败");
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
    
    //打怪
    code = fightatpoint5(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("位置5:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置5:完成");
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

    api_log_MsgDebug("位置6:开始");
    
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置6
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc6:对话:树藤:找字:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc6:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        

        api_log_MsgDebug("npc6:对话:树藤:找字:查找:失败");
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
    
    //打怪
    code = fightatpoint6(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("位置6:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置6:完成");
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

    api_log_MsgDebug("位置7:开始");
    
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置1
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc7:对话:树藤:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc7:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("npc7:对话:树藤:找字:大范围,失败");
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
    
    //打怪
    code = fightatpoint7(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("位置7:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置7:完成");
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

    api_log_MsgDebug("位置8:开始");
    
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置8
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc8:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc8:对话:树藤:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc8:对话:树藤:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc8:对话:树藤:找字:大范围,失败");
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
    
    //打怪
    code = fightatpoint8(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("位置8:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置8:完成");
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

    api_log_MsgDebug("位置9:开始");
    
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置11
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc9:对话:树藤:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc9:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("npc9:对话:树藤:找字:大范围,失败");
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
    
    //打怪
    code = fightatpoint9(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("位置9:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置9:完成");
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

    api_log_MsgDebug("位置10:开始");
    
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置12
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }


        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc10:对话:树藤:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc10:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        

        api_log_MsgDebug("npc10:对话:树藤:找字:大范围,失败");
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
    
    //打怪
    code = fightatpoint10(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("位置10:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置10:完成");
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

    api_log_MsgDebug("位置11:开始");
    
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置13
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc11:对话:树藤:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc11:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("npc11:对话:树藤:找字:大范围,失败");
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
    
    //打怪
    code = fightatpoint11(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("位置11:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置11:完成");
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

    api_log_MsgDebug("位置12:开始");
    
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置16
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc12:对话:树藤:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc12:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("npc12:对话:树藤:找字:大范围,失败");
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
    
    //打怪
    code = fightatpoint12(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("位置12:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置12:完成");
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

    api_log_MsgDebug("位置13:开始");
    
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置19
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc13:对话:树藤:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc13:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("npc13:对话:树藤:找字:大范围,失败");
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
    
    //打怪
    code = fightatpoint13(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("位置13:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("位置13:完成");
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
        api_log_MsgError("路径:坐标:非法:寻路:失败,tgtNum:%d, tgtPosXYs:%s", tgtNum, ptr);
        
        tgtPosXYs.ReleaseBuffer();
        return SNAIL_ERRNO_INVALID;
    }
    
    //前进到每个坐标点
    for (index = 0; index < tgtNum; ++index)
    {
        xyc = SplitString(tgtPosXY.GetAt(index), ':', tgtPosXYe);
        if (xyc < 2)
        {
            ptr = tgtPosXYs.GetBuffer(BUFFER_MAX);
            
            api_log_MsgError("路径:坐标:参数:非法:寻路:失败,index:%d, tgtPosXYs:%s, xyc:%d",
                index, ptr, xyc);
        
            tgtPosXYs.ReleaseBuffer();
            return ERRNO_INVALID;
        }

        tposX = atol(tgtPosXYe.GetAt(0));
        tposY = atol(tgtPosXYe.GetAt(1));

        api_log_MsgDebug("坐标:解析:成功,index:%d, tposX:%ld, tposY:%ld", index, tposX, tposY);

        walkstate.yNegKeyDown = 0;
        walkstate.yPosKeyDown = 0;
        walkstate.xPosKeyDown = 0;
        walkstate.xNegKeyDown = 0;

        //Check_Terminate_Thread(luaman_task, dmobj);
        
        //前往下一目标坐标点
        for (;;)
        {
        WALK_LABEL_NEXT:
            Check_Terminate_Thread(luaman_task, dmobj);
            
            //////////////////////////////////////////////////////////////////////////////
            //@bug:检查寻路时间是否超过阈值120秒,如果超过则认为是被卡住了,退出 2019/4/20 10:09:39
            //bug:修改为40秒 2022/01/10 21:57
            curTime = time(NULL);
            walkTimeDelta = difftime(curTime, startWalkTime);
            if (walkTimeDelta > 300.0)
            {
                (void)checkkeystate175(dmobj, ctxt);

                api_log_MsgDebug("寻路:坐标:卡住,停止寻路:等待副本消失,下一坐标, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //等待地洞副本消失
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //先检查下血条
            code = checklife175(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //读取当前坐标
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

                api_log_MsgWarn("地图:坐标:读取:失败");
                goto WALK_LABEL_NEXT;
            }

            //解析当前坐标
            xyc = SplitString(strPosXY, ':', posXY);
            if (xyc < 2)
            {
                ptr = strPosXY.GetBuffer(BUFFER_MAX);
                
                api_log_MsgWarn("地图:坐标:解析:失败,strPosXY:%s, xyc:%d",
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
                    /* 继续走 */
                    dmobj->KeyDownChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 1;
                    
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yNegKeyDown = 1;
                }
                else if (posY - tposY < -1)
                {
                    /* 继续走 */
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

                    /* Y轴坐标不动,X轴移动 */
                    dmobj->KeyDownChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 1;
                }
            }
            else if (posX - tposX < -1)
            {
                if (posY - tposY > 1)
                {
                    /* 继续走 */
                    dmobj->KeyDownChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 1;
                    
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yNegKeyDown = 1;
                }
                else if (posY - tposY < -1)
                {
                    /* 继续走 */
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

                /* X轴坐标不动,只判断Y轴 */
                if (posY - tposY > 1)
                {
                    /* 继续走 */
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yNegKeyDown = 1;
                }
                else if (posY - tposY < -1)
                {
                    /* 继续走 */
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

                    //当前目标坐标点处理完毕
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            //WALK_LABEL_NEXT
        }//end:前往下一目标坐标点

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    //寻路过程完毕,顺利到达目标终点坐标点
    return SNAIL_ERRNO_SUCCESS;

LABEL_WALK_BLOCK_EXIT_CHECK:
    //检查地图是否退出到副本外面
    for (;;)
    {
        //先检查下血条
        code = checklife175(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:卡住:血条:检查:死亡:复活");
            return code;
        }
        
        dmobj->SetDict(0, FONT175_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "平静河谷", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("寻路:卡住:地图:平静河谷:退出,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("寻路:卡住:地图:平静河谷:退出,正在退出中");
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
        
    //检查副本中是否有大boss,如果是则会出现"因此无法入场"
    dmobj->SetDict(0, FONT175_DIR"font-reenter.txt");
    code = dmobj->FindStrFast(0, 27, 1279, 826, "因此无法入场。", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:副本中有大boss进不去, lposX:%ld, lposY:%ld", lposX, lposY);

        //查找"副本中有大boss进不去:确认"字样
        for (;;)
        {
            dmobj->SetDict(0, FONT175_DIR"font-enter-ack.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;

                api_log_MsgDebug("副本中有大boss进不去:确认:查找:成功, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            LUAMAN_WAIT_MS(500);
            api_log_MsgDebug("副本中有大boss进不去:确认:查找:失败");
        }

        //点击"副本中有大boss进不去:确认"字样
        for (;;)
        {
            dmobj->MoveTo(lposX3, lposY3);
            LUAMAN_WAIT_MS(300);
            dmobj->LeftClick();
            LUAMAN_WAIT_MS(300);
            dmobj->MoveTo(5, 5);
            LUAMAN_WAIT_MS(300);

            dmobj->SetDict(0, FONT175_DIR"font-enter-ack.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功");
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            LUAMAN_WAIT_MS(500);
            api_log_MsgDebug("副本中有大boss进不去:确认:点击:失败");
        }

        api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功:重新对话");
        return SNAIL_ERRNO_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //没有进入副本,也没有出现"副本中有大boss进不去",则检查是否有"副本地下城满"
    dmobj->SetDict(0, FONT175_DIR"font-copyfull.txt");
    code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:副本地下城已满,lposX:%ld, lposY:%ld",
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;
                
                api_log_MsgDebug("副本地下城已满:确认:点击:成功,lposX3:%ld, lposY3:%ld",
                    lposX3, lposY3);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("副本地下城已满:确认:点击:失败");
            LUAMAN_WAIT_MS(500);
        }

        api_log_MsgDebug("副本地下城已满:确认:点击:成功:重新对话");
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
        api_log_MsgDebug("寻路:前往:坐标:[15:11]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[15:11]:成功");
    
    ///////////////////////////////////////////////////////////////////////////////////////
    //TODO:添加任务执行代码
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "圣魔之爪", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:圣魔之爪:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:对话:圣魔之爪:查找:失败");
        Delay(500);
    }

    //点击圣魔之爪
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("npc:对话:确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("npc:对话:确认:查找:失败");
        Delay(500);
    }

    //点击确认
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("npc:对话:确认:点击:成功");
            break;
        }

        code = check175copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc:对话:确认:点击:副本地下城已满:重新对话");
            goto LABEL_retalk_with_NPC;
        }
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("npc:对话:确认:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //等待进入圣魔之爪
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
            
        dmobj->SetDict(0, FONT175_DIR"font-chckmap.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "圣魔之爪", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:圣魔之爪:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:圣魔之爪:查找:失败");
        Delay(1000);
    }

    //视角向上
    dmobj->MiddleClick();
    LUAMAN_WAIT_MS(1000);
    dmobj->MiddleClick();
    LUAMAN_WAIT_MS(1000);

    //---------------------------------------------------------------------
    //第一段
    code = taskatpos1(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点1:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("任务:点1:成功");
    
    //---------------------------------------------------------------------
    //第2段
    code = taskatpos2(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点2:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("任务:点2:成功");

    //---------------------------------------------------------------------
    //第3段
    code = taskatpos3(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点3:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("任务:点3:成功");

    //第4段
    code = taskatpos4(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点4:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("任务:点4:成功");

    //第5段
    code = taskatpos5(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点5:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("任务:点5:成功");

    //第6段
    code = taskatpos6(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点6:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("任务:点6:成功");
    
    //第7段
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置7
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    //TODO:更新偏移值
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

    //找绿色入场
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "正在入场", "00ff00-101010", 0.9, &pposX3, &pposY3);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX3 = pposX3.lVal;
            lposY3 = pposY3.lVal;

            api_log_MsgDebug("对话:正在入场:查找:成功, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
            break;
        }

        api_log_MsgDebug("对话:正在入场:查找:失败");
        Delay(500);
    }

    //点击绿色入场
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "正在入场", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("对话:正在入场:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("对话:正在入场:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //等待坐标变成146:67
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

                //bug:误差太大可能就在目标点附近,实际并没有飞拢 2022/01/08 15:43:16
                if (abs(posX - 146) <= 1 && abs(posY - 67) <= 1)
                {
                    api_log_MsgDebug("坐标:146:67:匹配");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("坐标:146:67:未匹配");
        Delay(1000);
    }

    totaltime = 10;
    code = fightatstep9(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("步骤9:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("步骤9:成功");
    
    //第8段 
    code = taskatpos7(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点7:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("任务:点7:成功");
    //第9段

    code = taskatpos8(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点8:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("任务:点8:成功");
    
    //第10段 
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置10
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    //TODO:更新偏移值
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

    //找绿色入场
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "正在入场", "00ff00-101010", 0.9, &pposX4, &pposY4);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX4 = pposX4.lVal;
            lposY4 = pposY4.lVal;

            api_log_MsgDebug("对话:正在入场:找字:大范围,成功, lposX4:%ld, lposY4:%ld", lposX4, lposY4);
            break;
        }

        api_log_MsgDebug("对话:正在入场:找字:大范围,失败");
        Delay(500);
    }

    //点击绿色入场
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "正在入场:", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("对话:正在入场:找字:大范围,成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("对话：正在入场:找字:大范围,失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //等待坐标变成206:415
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

                //bug:误差太大可能就在目标点附近,实际并没有飞拢 2022/01/08 15:43:16
                if (abs(posX - 206) <= 1 && abs(posY - 415) <= 1)
                {
                    api_log_MsgDebug("坐标:206:415:匹配");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("坐标:206:415:未匹配");
        Delay(1000);
    }

    totaltime = 10;
    code = fightatstep12(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("步骤12:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("步骤12:成功");
    
    //第11段 
    code = taskatpos9(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点9:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("任务:点9:成功");
    
    //第12段 
    code = taskatpos10(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点10:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("任务:点10:成功");
    
    //第13段 
    code = taskatpos11(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点11:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("任务:点11:成功");
    
    //第14段

Label_retalk_at_NPC_16:
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置14
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    //TODO:更新偏移值
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

    //找黄色的气体操纵杆
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "气体操纵杆", "ffff00-101010", 0.9, &pposX5, &pposY5);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX5 = pposX5.lVal;
            lposY5 = pposY5.lVal;

            api_log_MsgDebug("气体操纵杆:查找:成功, lposX:%ld, lposY:%ld", lposX5, lposY5);
            break;
        }

        api_log_MsgDebug("气体操纵杆:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos_u[3].deltax;
    deltaY = deltaatpos_u[3].deltay;
    
    //点击黄色的气体操纵杆
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "气体操纵杆", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("气体操纵杆:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("气体操纵杆:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        goto Label_retalk_at_NPC_16;
    }
    
    //第15段
    code = taskatpos12(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点12:失败:code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("任务:点12:成功");
    
    //第16段
Label_retalk_at_NPC_18:
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置17
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    //TODO:更新偏移值
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

    //找黄色的气体操纵杆
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->SetDict(0, FONT175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "气体操纵杆", "ffff00-101010", 0.9, &pposX5, &pposY5);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX5 = pposX5.lVal;
            lposY5 = pposY5.lVal;

            api_log_MsgDebug("气体操纵杆:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX5, lposY5);
            break;
        }

        api_log_MsgDebug("气体操纵杆:找字:大范围,失败");
        Delay(500);
    }
    deltaX = deltaatpos_u[5].deltax;
    deltaY = deltaatpos_u[5].deltay;
    

    //点击黄色的气体操纵杆
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "气体操纵杆", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("气体操纵杆:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("气体操纵杆:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        goto Label_retalk_at_NPC_18;
    }
    
    //第17段
    code = taskatpos13(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("任务:点13:失败:code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("任务:点13:成功");
    
    //第18段
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置20
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    //TODO:更新偏移值
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

    //找绿色进入管制室
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175DuiHua3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入管制室", "00ff00-101010", 0.9, &pposX7, &pposY7);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX7 = pposX7.lVal;
            lposY7 = pposY7.lVal;

            api_log_MsgDebug("对话:进入管制室:查找:成功, lposX:%ld, lposY:%ld", lposX7, lposY7);
            break;
        }

        api_log_MsgDebug("对话:进入管制室:查找:失败");
        Delay(500);
    }

    //点击绿色进入管制室
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入管制室", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("进入管制室:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("进入管制室:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //等待坐标变成68:202
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

                //bug:误差太大可能就在目标点附近,实际并没有飞拢 2022/01/08 15:43:16
                if (abs(posX - 68) <= 1 && abs(posY - 202) <= 1)
                {
                    api_log_MsgDebug("坐标:68:202:匹配");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("坐标:68:202:未匹配");
        Delay(1000);
    }

    
    totaltime = 10;
    code = fightatstep20(dmobj, ctxt, totaltime);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("步骤20:失败:code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("步骤20:成功");
    
    //第20段
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置22
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    //TODO:更新偏移值
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
        api_log_MsgError("步骤22:失败:code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("步骤22:成功");
    
    //第21段
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置22
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    //TODO:更新偏移值
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
    
    //找绿色脱离副本
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONT175_DIR"font-175DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "脱离副本", "00ff00-101010", 0.9, &pposX8, &pposY8);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX8 = pposX8.lVal;
            lposY8 = pposY8.lVal;

            api_log_MsgDebug("对话:脱离副本:查找:成功, lposX8:%ld, lposY8:%ld", lposX8, lposY8);
            break;
        }

        api_log_MsgDebug("对话:脱离副本:查找:失败");
        Delay(500);
    }

    //点击绿色脱离副本
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "脱离副本", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("对话:脱离副本:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("对话:脱离副本:找字:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    Delay(100);

    //等待右上角地图名字变成德拉比斯野外
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT175_DIR"font-chckmap.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "德拉比斯野外", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:德拉比斯野外:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Delay(1000);
        api_log_MsgDebug("地图:德拉比斯野外:查找:失败");
    }

    //按p先退出队伍
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("退出队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("退出队伍:查找:失败");
        Delay(500);
    }

    //点击
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }

    //点击确认
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //重新组队
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "创建队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("创建队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("创建队伍:查找:失败");
        Delay(500);
    }

    //点击创建队伍
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }

    //点击确认
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //按掉p
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONT175_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("退出队伍:关闭:成功");
                break;
            }
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("退出队伍:关闭:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    
    
    ///////////////////////////////////////////////////////////////////////////////////////
    api_log_MsgDebug("副本:175:任务:执行:成功");
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

    api_log_MsgDebug("副本:175:初始化");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);

    api_PrintConfig(luaman_config);
    
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("副本:175:窗口:绑定:失败, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("副本:175:窗口:绑定:成功, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("窗口:鼠标:锁定:失败, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("窗口:鼠标:锁定:成功, hwnd:%ld", hwnd);
    
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
    
    api_log_MsgDebug("副本:175:主循环:开始: hwnd:%ld, role:%d", hwnd, (int)role);

    exec175taskloop(dmobj, ctxt);
    
    api_log_MsgDebug("副本:175:主循环:完成, hwnd:%ld", hwnd);
    
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

    api_log_MsgDebug("副本:175:结束: hwnd:%ld, role:%d", hwnd, (int)role);

    return ERRNO_SUCCESS;
}
