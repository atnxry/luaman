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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "添加好友", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("组队:邀请:添加好友:找字:大范围:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 20.0)
        {
            api_log_MsgDebug("组队:邀请:添加好友:找字:大范围,失败:超时");
            return SNAIL_ERRNO_FAILED;
        }

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        api_log_MsgDebug("组队:邀请:添加好友:找字:大范围,失败");
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(200);
    
    //查找'战场服务'字样
    lastChckTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-inviteheil2team-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "战场服务", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("组队:邀请:战场服务:找字,成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 20.0)
        {
            api_log_MsgDebug("组队:邀请:战场服务:找字,失败,超时,退出");
            return SNAIL_ERRNO_OFFLINE;
        }

        api_log_MsgDebug("组队:邀请:战场服务:找字,失败");
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

            //点击队员名称,弹出队伍对话框
            for (;;)
            {
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                
                dmobj->MoveTo(posX, posY);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);

                dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-inviteheil2team-1.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "队伍", "7d7d7d-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("组队:邀请:队员:队伍:找字,成功,lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                curTime = time(NULL);
                deltaTime = difftime(curTime, lastChckTime);
                if (deltaTime > 20.0)
                {
                    api_log_MsgDebug("组队:邀请:队员:队伍:找字,失败,超时,退出");
                    return SNAIL_ERRNO_OFFLINE;
                }

                api_log_MsgDebug("组队:邀请:队员:队伍:找字,失败");
                Delay(500);
            }

            //点击'队伍'邀请队友加入队伍
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);

            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        }

        syncCount = 0;

        //@bug:稍等片刻,避免二次组队浪费时间2019/4/8 23:06:19
        Delay(2000);

        api_log_MsgDebug("同行:个数:检查");
        
        //检查'同行'个数
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            posX1 = syncPosXYs[2 * i].x;
            posY1 = syncPosXYs[2 * i].y;
            posX2 = syncPosXYs[2 * i + 1].x;
            posY2 = syncPosXYs[2 * i + 1].y;

            api_log_MsgDebug("(%ld, %ld) --> (%ld, %ld)", posX1, posY1, posX2, posY2);

            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-inviteheil2team-1.txt");
            code = dmobj->FindStrFast(posX1, posY1, posX2, posY2, "同行", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                syncCount++;

                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;

                api_log_MsgDebug("同行:检查:成功:%ld:%ld", lposX2, lposY2);
            }
        }

        if (syncCount == syncCountMax)
        {
            api_log_MsgDebug("组队:邀请:队员:所有,组队完成");
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTimeAll);
        if (deltaTime > 30.0)
        {
            api_log_MsgDebug("组队:邀请:队员:队伍:找字,失败,超时,退出");
            
            if (0 == syncCount)
            {
                return SNAIL_ERRNO_FAILED;
            }

            return SNAIL_ERRNO_AGAIN;
        }

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        api_log_MsgDebug("组队:邀请:队员:所有,组队未完成,syncCount:%d", syncCount);
    }

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    api_log_MsgDebug("组队完成:成功:完成");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "更换角色", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("esc:游戏结束:弹出:成功:lposX:%ld,lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("esc:游戏结束:弹出:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "游戏信息保存", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            api_log_MsgDebug("esc:游戏结束:点击,成功");
            
            dmobj->UnBindWindow();
            _endthreadex(1);
        }

        api_log_MsgDebug("esc:游戏结束:点击,失败");
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
    
    api_log_MsgDebug("点1::打怪:开始");

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-tree.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[0];
            
            api_log_MsgDebug("树藤:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-tree.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[1];
            
            api_log_MsgDebug("树藤:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-tree.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[2];
            
            api_log_MsgDebug("树藤:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("树藤:查找:失败");
        Delay(500);
    }

    //按下波浪键
    dmobj->KeyDownChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);
    
    dmobj->KeyDownChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);
    
    //开始打怪
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
                code = dmobj->FindStrFast(0, 27, 1279, 105, "奥德赛", "ffffff-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    fontcolor = fontcolors[0];
                    flag = 1;
                    
                    api_log_MsgDebug("u怪:奥德赛:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc2.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 105, "奥德赛", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    fontcolor = fontcolors[1];
                    flag = 1;
                    
                    api_log_MsgDebug("u怪:奥德赛:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc2.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 105, "奥德赛", "ff3300-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    fontcolor = fontcolors[2];
                    flag = 1;
                    
                    api_log_MsgDebug("u怪:奥德赛:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }
                
                break;
            }

            if (!flag)
            {
                api_log_MsgDebug("u怪:奥德赛:继续");
                continue;
            }

            //dmobj->Capture(0, 27, 1279, 826, "cap\\aodysay.bmp");
        }

        //查看打死没有
        for (;;)
        {
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 105, "奥德赛", fontcolor, 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                flag = 2;
                
                api_log_MsgDebug("u怪:奥德赛:%s:打怪:成功", fontcolor);
                break;
            }

            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("u怪:奥德赛:%s:打怪:继续, lposX:%ld, lposY:%ld",
                fontcolor, lposX, lposY);
            
            break;
        }

        if (2 == flag)
        {
            api_log_MsgDebug("u怪:奥德赛:%s:打怪:退出", fontcolor);
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

    api_log_MsgDebug("点1::打怪:完成");
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

    api_log_MsgDebug("点2:打怪:开始");
    
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "奈特罗斯的墓穴", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("奈特罗斯的墓穴:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (240 * 1.0))
        {
            api_log_MsgDebug("奈特罗斯的墓穴:打怪:超时");
            break;
        }
        
        api_log_MsgDebug("奈特罗斯的墓穴:查找:失败");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("点2:打怪:完成");
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
    
    api_log_MsgDebug("点3::打怪:开始");

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 105, "奈特罗斯", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[0];
            
            api_log_MsgDebug("奈特罗斯:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 105, "奈特罗斯", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[1];
            
            api_log_MsgDebug("奈特罗斯:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-npc6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 105, "奈特罗斯", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[2];
            
            api_log_MsgDebug("奈特罗斯:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("奈特罗斯:查找:失败");
        Delay(500);
    }

    //开始打怪
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结束等待", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("u怪:奈特罗斯:%s:打怪:结束等待:查找:成功, lposX:%ld, lposY:%ld",
                fontcolor, lposX, lposY);
            break;
        }

        api_log_MsgDebug("u怪:奈特罗斯:%s:打怪:结束等待:查找:继续", fontcolor);
    }

    /*for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }*/

    api_log_MsgDebug("点3::打怪:完成");
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

        //组队开始
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            //组队
            api_log_MsgDebug("临界区:进入:等待");
            TzRefugeboatEnterCriticalSection();
            api_log_MsgDebug("临界区:进入:成功");

            for (;;)
            {
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                code = refugeboat_invite_join(dmobj, ctxt);
                if (SNAIL_ERRNO_AGAIN != code)
                {
                    api_log_MsgDebug("组队:完成");
                    break;
                }

            }

            api_log_MsgDebug("临界区:退出:等待");
            TzRefugeboatLeaveCriticalSection();
            api_log_MsgDebug("临界区:退出:成功");
            
            //bug:如果连续被杀,重新去检查组队会长时间进不去,直接去对话
            //    如果未组队会提示人员不足 2022/05/04 20:30:31
            if (SNAIL_ERRNO_REALIVE == code)
            {
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            //@bug:副号掉线,退出游戏窗口,停止线程,弹出错误对话框 2018/4/28 22:43:09
            if (SNAIL_ERRNO_OFFLINE == code)
            {
                //按esc点击重新登录
                for (;;)
                {
                    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                    
                    dmobj->KeyDownChar("esc");
                    Delay(500);
                    dmobj->KeyUpChar("esc");
                    Delay(500);

                    dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-exitgame1.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "结束游戏", "929292-101010", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("esc:游戏结束:弹出,成功,lposX:%ld, lposY:%ld", lposX, lposY);
                        break;
                    }

                    api_log_MsgDebug("esc:游戏结束:弹出,失败");
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
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "游戏信息保存", "ffffff-101010", 0.9, &pposX2, &pposY2);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX2 = pposX2.lVal;
                        lposY2 = pposY2.lVal;
                        
                        api_log_MsgDebug("esc:游戏结束:点击,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                        
                        dmobj->UnBindWindow();
                        _endthreadex(1);
                        break;
                    }
                }

                api_log_MsgDebug("esc:游戏结束:点击,失败");
                Delay(1000);
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            }

            ////////////////////////////////////////////////////////////////////////////
            if (SNAIL_ERRNO_SUCCESS == code)
            {
                api_log_MsgDebug("组队:邀请:成功");
                break;
            }

            //睡眠期间响应F8退出
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
                api_log_MsgDebug("组队:邀请:失败,睡眠一会后重试, sleepTimesj:%d, blockTimes:%ld", sleepTimesj, blockTimes);
            }
        }//组队结束

        copyFullCounter = 0;
        
    COPY_LABEL_TALK_WITH_NPC:
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        //开始执行副本任务
        //按n对话进入副本
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "遇难船入场", "00ff00-000000", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("npc1:对话:遗忘的遇难船入场:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("npc1:对话:遗忘的遇难船入场:查找:失败");
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

            //检查是否出现'确认'字样的对话框
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-okcancel.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
            
                api_log_MsgDebug("npc1:对话:确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }
            
            api_log_MsgDebug("npc1:对话:确认:查找:失败");
            Delay(500);
        }

        //点击'确认'字样对话框
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
            //bug: 如果有地洞副本满框框,则点确认;否则就是点确认对应的取消 2020/12/27 14:02:18
            //如果有地洞副本满框框,则先点掉 2019/7/19 20:47:59
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-copyfull.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("副本地下城已满:fff6f5:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
                
                //点击"副本地下城已满"
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
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("原地复活:副本地下城已满:点击:成功");
                        break;
                    }

                    api_log_MsgDebug("原地复活:副本地下城已满:点击:失败");
                    
                    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                    Delay(500);
                }

                //@bug:副本地下城满连续超过5次则退出游戏 2019/5/19 23:08:15
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
                
                api_log_MsgDebug("npc1:对话:副本地下城已满:重新对话,copyFullCounter:%d", copyFullCounter);
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            /////////////////////////////////////////////////////////////////////////////
            //有可能满足无法入场的提示 2022/03/06 22:03:48 580,398->636,500
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-cantenter.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "您不符合入场条件入场", "ffffff-202020", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("您不符合入场条件入场:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
                
                //点击"副本地下城已满"
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
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "您不符合入场条件入场", "ffffff-202020", 0.9, &pposX2, &pposY2);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("您不符合入场条件入场:点击:成功");
                        break;
                    }

                    api_log_MsgDebug("您不符合入场条件入场:点击:失败");
                    
                    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                    Delay(500);
                }

                api_log_MsgDebug("npc1:对话:您不符合入场条件入场:重新对话");
                goto COPY_LABEL_TALK_WITH_NPC;
            }
            /////////////////////////////////////////////////////////////////////////////

            //检查是否进入副本
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-maptips.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "遗忘的遇难船", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("npc1:对话:遗忘的遇难船:进入,成功:lposX:%ld,lposY:%ld", lposX, lposY);
                break;
            }

            //////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:检查"人员不足" 2022/03/15 22:02:55
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-peopleleak.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "人员不足", "ffffff-202020", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("人员不足:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);

                for (;;)
                {
                    //点击确认
                    dmobj->MoveTo(lposX + 32, lposY + 104);
                    Delay(300);
                    dmobj->LeftClick();
                    Delay(500);
                    dmobj->MoveTo(5, 5);
                    Delay(300);
                    
                    dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-peopleleak.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "人员不足", "ffffff-202020", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("人员不足:点击:成功:重新对话");
                        goto COPY_LABEL_AGAIN;
                    }

                    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
                    api_log_MsgDebug("npc1:对话:遗忘的遇难船,正在进入");
                    Delay(500);
                }
                
            }
            //////////////////////////////////////////////////////////////////////////////////////////////////////
            
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            api_log_MsgDebug("npc1:对话:遗忘的遇难船,正在进入");
            Delay(500);
        }

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        //打怪
        code = refugeboat_fight_phase1(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("打怪:点1:失败, code:%d", code);
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("打怪:点1:成功");

        dmobj->KeyDownChar("-");
        Delay(300);
        dmobj->KeyUpChar("-");
        Delay(300);
        
        Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

        //按M，找贵宾服务的相对位置1
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至船内", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("移动至船内:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("移动至船内:查找:失败");
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至船内", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("移动至船内:点击:成功");
                break;
            }
            
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("移动至船内:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
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
    
        //按M，找贵宾服务的相对位置1
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
            api_log_MsgDebug("打怪:点2:失败, code:%d", code);
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("打怪:点2:成功");
        
        dmobj->KeyDownChar("-");
        Delay(300);
        dmobj->KeyUpChar("-");
        Delay(300);

        Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);
    
        //按M，找贵宾服务的相对位置1
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "罗斯的安息处", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("罗斯的安息处:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("罗斯的安息处:查找:失败");
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "罗斯的安息处", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("罗斯的安息处:点击:成功");
                break;
            }
            
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("罗斯的安息处:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
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

        //按M，找贵宾服务的相对位置1
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
            api_log_MsgDebug("打怪:点3:失败, code:%d", code);
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("打怪:点3:成功");

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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至副本外", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("移动至副本外:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("移动至副本外:查找:失败");
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至副本外", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("移动至副本外:点击:成功");
                break;
            }
            
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("移动至副本外:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
            Delay(500);
        }
        
        //等待地图
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-maptips.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "德拉比斯野外", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("地图:德拉比斯野外:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("地图:德拉比斯野外:查找:失败");
            Delay(1000);
        }

        //按p先退出队伍
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
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(200);

            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-auxiend.txt");
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
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            dmobj->MoveTo(lposX2, lposY2);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(200);

            dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("确认:点击:成功");
                break;
            }

            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("确认:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
            Delay(500);
        }

        api_log_MsgDebug("副本:执行:完毕:继续:下一轮");
    }//COPY_LABEL_AGAIN

    api_log_MsgDebug("副本:主号:执行:结束");
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
        
        //判断是否出现"组队邀请"
        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "组队邀请", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("组队:借组:组队邀请:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        Delay(500);
        api_log_MsgDebug("组队:借组:组队邀请:找字:大范围,失败");
    }

    //点击'是'接受组队邀请
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "队伍口号", "f4be3d-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("组队:借组:队伍口号:找字:大范围,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Delay(500);
        api_log_MsgDebug("组队:借组:队伍口号:找字:大范围,失败");
    }

    lastChckTime = time(NULL);

    api_log_MsgDebug("检查主号是否进入副本:开始");
    
    //检查主号是否进入副本
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "遗忘的遇难船", "909090-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("组队:主号:进入副本:灰色:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "遗忘的遇难船", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("组队:主号:进入副本:白色:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "遗忘的遇难船", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("组队:主号:进入副本:黄色:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        if (1 == exitGameCopyFull)
        {
            curTime = time(NULL);
            deltaTime = difftime(curTime, lastChckTime);
            if (deltaTime > 600.0)
            {
                api_log_MsgDebug("检查主号是否进入副本:主号:进入副本:超时");
                break;
            }
        }

        api_log_MsgDebug("组队:主号:进入副本:失败");
        Delay(500);
    }
    
    //查找'退出队伍'
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("组队:借组:退出队伍:找字:灰色,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Delay(500);
        
        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("组队:借组:退出队伍:找字:白色,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("组队:借组:退出队伍:找字:失败");
        Delay(500);
    }

    //点击'退出队伍'
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确定要退出队伍吗", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("组队:主号:进入副本:确定要退出队伍吗:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("组队:主号:进入副本:确定要退出队伍吗:查找:失败");
        Delay(500);
    }

    //按回车
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyDownChar("enter");
        Delay(500);
        dmobj->KeyUpChar("enter");
        Delay(500);

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        dmobj->SetDict(0, FONT_REFUGEBOAT_DIR"font-join2team-4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确定要退出队伍吗", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("组队:主号:进入副本:确定要退出队伍吗:点击:成功");
            break;
        }

        api_log_MsgDebug("组队:主号:进入副本:确定要退出队伍吗:点击:失败");
        Delay(500);
    }
    
    api_log_MsgDebug("副号:线程执行完毕");
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

    api_log_MsgDebug("副本:refugeboat:初始化");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);

    api_PrintConfig(luaman_config);
    
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("副本:refugeboat:窗口:绑定:失败, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("副本:refugeboat:窗口:绑定:成功, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("副本:refugeboat:窗口:鼠标:锁定:失败, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }
    
    api_log_MsgDebug("副本:refugeboat:窗口:鼠标:锁定:成功, hwnd:%ld", hwnd);

    /* 初始化告警时间初始值 */
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
    
    api_log_MsgDebug("副本:refugeboat:主循环: hwnd:%ld, role:%d", hwnd, (int)role);

    switch (role)
    {
    case ROLE_M:
        //@bug:地洞副本满时主号结束游戏:exitGameCopyFull 2019/5/20 21:42:16
        api_log_MsgDebug("副本:refugeboat:主号:主循环:开始, hwnd:%ld", hwnd);
        refugeboat_loop_master(dmobj, ctxt);
        api_log_MsgDebug("副本:refugeboat:主号:主循环:完成, hwnd:%ld", hwnd);
        break;

    default:
        for (;;)
        {
            api_log_MsgDebug("副本:refugeboat:副号:主循环:开始, hwnd:%ld", hwnd);

            refugeboat_join_team(dmobj, ctxt);
            
            api_log_MsgDebug("副本:refugeboat:副号:主循环:完成, hwnd:%ld", hwnd);
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

    api_log_MsgDebug("副本:refugeboat:结束: hwnd:%ld, role:%d", hwnd, (int)role);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
    
    return ERRNO_SUCCESS;
}

