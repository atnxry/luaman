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
#include "taskFire.h"

#define FONTFIRE_DIR "font\\fire\\"
#define BMPFIRE_DIR "bmp\\fire\\"

#define fire_fight_time(_w_, _f_) {(_w_), (_f_)}

struct deltavalxy
{
    int deltax;
    int deltay;
};

/*
    3 进入副本,右上角的地图名字，白色的 卢德妮(font-175MingZhi1.txt).
      根据设定时间，看看是否需要清包。
      然后判断坐标。坐标石242:220.执行A流程。坐标,312:220，执行B流程。如果都不是。就按后面流程.
    按等号键，等3秒，按M，点击贵宾服务相对位置（1），按0，等1秒。再按M。
    
   !!! 点击贵宾服务相对位置（1）
*/
struct deltavalxy deltaatpos_fire[] =
{
    {-401, 285}, //第1个点的x,y偏移
};

/*
    A1，按等号键，等3秒，按M，点击贵宾服务相对位置（2），按0，等1秒。再按M。再等3秒。按SHIFT+A。
    然后鼠标移动到位置（?）。交替按3和鼠标右键。按15秒。然后按SHIFT+B。
    !!! 鼠标位置
*/
struct deltavalxy deltaatpos_mouse_A[] =
{
    {631, 479}, //第1个点的x,y偏移
};

/*
    A1~A7步骤中各个点击贵宾服务相对位置
*/
struct deltavalxy deltaatpos_fire_A[] =
{
    {-316, 261}, //第1个
    {-506, 246},
    {-414, 221},
    {-357, 195},
    {-500, 133},
    {-313, 62},
    {-248, 72},
};

/*
    B1~B6步骤中各个点击贵宾服务相对位置
*/
struct deltavalxy deltaatpos_fire_B[] =
{
    {-399, 361},
    {-298, 389},
    {-365, 466},
    {-432, 499},
    {-305, 473},
    {-269, 471},
};

//与流程中的"开始打怪"对应
//参数1:等待时间
//参数2:打怪时间
#define taskfire_fight_atpoint_A2() {\
    pirate_fight_time(10, 10),\
}

#define taskfire_fight_atpoint_A3() {\
    pirate_fight_time(10, 7),\
}

#define taskfire_fight_atpoint_A4() {\
    pirate_fight_time(1, 35),\
}

#define taskfire_fight_atpoint_B1() {\
    pirate_fight_time(16, 5),\
}

#define taskfire_fight_atpoint_B2() {\
    pirate_fight_time(5, 20),\
}

#define taskfire_fight_atpoint_B3() {\
    pirate_fight_time(10, 5),\
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!!! 下面的不要动
static int checkkeystatefire(Idmsoft* dmobj, void* ctxt)
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


static int firewalkpath(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
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
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            //////////////////////////////////////////////////////////////////////////////
            //@bug:检查寻路时间是否超过阈值120秒,如果超过则认为是被卡住了,退出 2019/4/20 10:09:39
            //bug:修改为40秒 2022/01/10 21:57
            curTime = time(NULL);
            walkTimeDelta = difftime(curTime, startWalkTime);
            if (walkTimeDelta > 300.0)
            {
                (void)checkkeystatefire(dmobj, ctxt);

                api_log_MsgDebug("寻路:坐标:卡住,停止寻路:等待副本消失,下一坐标, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //等待地洞副本消失
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //读取当前坐标
            dmobj->SetDict(0, FONTFIRE_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)checkkeystatefire(dmobj, ctxt);
                
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

                (void)checkkeystatefire(dmobj, ctxt);
                
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

                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            }

            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            //WALK_LABEL_NEXT
        }//end:前往下一目标坐标点

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    }

    //寻路过程完毕,顺利到达目标终点坐标点
    return SNAIL_ERRNO_SUCCESS;

LABEL_WALK_BLOCK_EXIT_CHECK:
    //检查地图是否退出到副本外面
    for (;;)
    {
        dmobj->SetDict(0, FONTFIRE_DIR"font-chckmap.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "平静河谷", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("寻路:卡住:地图:平静河谷:退出,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("寻路:卡住:地图:平静河谷:退出,正在退出中");
    }

    return SNAIL_ERRNO_BLOCKED;
}


static HANDLE g_firecpy_criticalSection;
void firecpy_InitCriticalSection()
{
    g_firecpy_criticalSection = ::CreateMutex(NULL, FALSE, NULL);
    if (NULL == g_firecpy_criticalSection)
    {
        api_log_MsgError("create firecpy_InitCriticalSection lock failed");
        exit(1);
    }
    return;
}

static void firecpyEnterCriticalSection()
{
    ::WaitForSingleObject(g_firecpy_criticalSection, INFINITE);
    return;
}

static void firecpyLeaveCriticalSection()
{
    (void)::ReleaseMutex(g_firecpy_criticalSection);
    return;
}

static void firecpyDeleteCriticalSection()
{
    (void)::CloseHandle(g_firecpy_criticalSection);
    return;
}

static int firejoin2team(Idmsoft* dmobj, void* ctxt)
{
    int code = 0;
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int syncCountMax = 2/*luaman_config->syncCountMax*/;
    struct xyposition syncPosXYs[4] =
    {
        {1234, 266},
        {1272, 286},
        {1234, 298},
        {1272, 318},
    };
    time_t lastChckTime = time(NULL);
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    struct xyposition fellowPosXYs[2];
    time_t lastChckTimeAll;
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-inviteheil2team-1.txt");
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

    lastChckTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTFIRE_DIR"font-inviteheil2team-1.txt");
        code = dmobj->FindStrFast(350, 35, 1279, 826, "频道", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("组队:邀请:频道:找字,成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 20.0)
        {
            api_log_MsgDebug("组队:邀请:频道:找字,失败,超时,退出");
            
            return SNAIL_ERRNO_OFFLINE;
        }

        api_log_MsgDebug("组队:邀请:频道:找字,失败");
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

                dmobj->SetDict(0, FONTFIRE_DIR"font-inviteheil2team-1.txt");
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

            dmobj->SetDict(0, FONTFIRE_DIR"font-inviteheil2team-1.txt");
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

    api_log_MsgDebug("组队:成功:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int fireSaveXstrong2Library(Idmsoft* dmobj, void* ctxt)
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
    
    //查找背包标签
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        //检查是否按成功
        dmobj->SetDict(0, FONTFIRE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("仓库:背包:字样:找字,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //bug:调整界面相对位置 2021/6/18 10:11:19
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

        api_log_MsgDebug("仓库:背包:字样:找字,成功");
        Delay(500);
    }

    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;
    
    api_log_MsgDebug("仓库:背包:宝物柜, lineX:%ld, lineY:%ld", lineX, lineY);

    for (index = 0; index < 5; index++)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        posY = lineY + index * deltaY;
        
        for (jndex = 0; jndex < 10; jndex++)
        {
            posX = lineX + jndex * deltaX;
            
            boxPosXYs[index * 10 + jndex].x = posX;
            boxPosXYs[index * 10 + jndex].y = posY;

            api_log_MsgDebug("boxPosXYs[%d][%d] = [%ld, %ld]", index, jndex, posX, posY);
        }
    }

    api_log_MsgDebug("仓库:背包:宝物柜,准备处理");

    //处理每个背包的所有宝物柜
    int pkgnum = 2;
    for (lndex = 0; lndex < pkgnum; lndex++)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }

        //点开标签页
        dmobj->MoveTo(labelPosXY[lndex].x, labelPosXY[lndex].y);
        Delay(200);
        dmobj->LeftDown();
        Delay(200);
        dmobj->LeftUp();
        Delay(200);

        for (index = 0; index < 5; index++)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
            for (jndex = 0; jndex < 10; jndex++)
            {
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
                
                posX = boxPosXYs[index * 10 + jndex].x;
                posY = boxPosXYs[index * 10 + jndex].y;

                dmobj->MoveTo(posX, posY);
                Delay(200);

                api_log_MsgDebug("处理:格尼亚罗:背包:%d:柜子:[%d,%d]:坐标:[%ld, %ld]",
                    lndex, index, jndex, posX, posY);
                
                findxstrong = 0;

                //查找'小强|中强|高强'字样
                dmobj->SetDict(0, FONTFIRE_DIR"font-qingbao4.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "剑圣的翅膀", "ffcc00-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    findxstrong = 1;

                    lposX1 = pposX1.lVal;
                    lposY1 = pposY1.lVal;

                    Delay(200);
                    dmobj->RightClick();
                    Delay(200);

                    api_log_MsgDebug("宝物柜:剑圣的翅膀:存储,成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                    //break;
                }

                if (0 == findxstrong)
                {
                    dmobj->SetDict(0, FONTFIRE_DIR"font-qingbao2.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "转职技能书", "ffe08c-101010", 0.9, &pposX1, &pposY1);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        findxstrong = 2;

                        lposX1 = pposX1.lVal;
                        lposY1 = pposY1.lVal;

                        Delay(200);
                        dmobj->RightClick();
                        Delay(200);

                        api_log_MsgDebug("宝物柜:转职技能书:存储,成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                        //break;
                    }
                }
            }
        }
    }

    return SNAIL_ERRNO_SUCCESS;
}

static int firesellgabge(Idmsoft* dmobj, void* ctxt)
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
    
    api_log_MsgDebug("卖垃圾:开始");

    posX1 = 1069;
    posY1 = 48;

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        //点击之后出现"仓库"字样
        dmobj->SetDict(0, FONTFIRE_DIR"font-surebox.txt");
        code = dmobj->FindStrFast(590, 500, 687, 530, "仓库", "ffffff-101010", 0.9, &pposX1, &pposY1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX1 = pposX1.lVal;
            lposY1 = pposY1.lVal;

            api_log_MsgDebug("商店:仓库:字样:找字:成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
            break;
        }
        
        api_log_MsgDebug("商店:仓库:字样:找字:失败");
        Delay(500);
    }

    //点击商店 633:510->633:380
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX1, lposY1 - 130);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONTFIRE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("商店:背包:字样:找字:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //bug:调整界面相对位置 2021/6/18 10:11:19
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

        api_log_MsgDebug("商店:背包:字样:找字:失败");
        Delay(500);
    }

    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;

    api_log_MsgDebug("仓库:背包:宝物柜, lineX:%ld, lineY:%ld", lineX, lineY);

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
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }

        //点开标签页
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

                api_log_MsgDebug("处理:背包:%d:柜子:[%d,%d]:坐标:[%ld, %ld]",
                    lndex, index, jndex, posX, posY);
                
                dmobj->SetDict(0, FONTFIRE_DIR"font-qingbao1.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "镶嵌石", "ffe08c-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    api_log_MsgDebug("卖垃圾:镶嵌石:成功");
                    goto sell;
                }

                dmobj->SetDict(0, FONTFIRE_DIR"font-qingbao3.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "纹章", "ffe08c-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    api_log_MsgDebug("卖垃圾:纹章:成功");
                    goto sell;
                }

                continue;
                
            sell:
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
    
    api_log_MsgDebug("卖垃圾:完毕");
    return SNAIL_ERRNO_SUCCESS;
}


static int fireCleanPackage(Idmsoft* dmobj, void* ctxt)
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

    //按i弹出背包
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(200);
        dmobj->KeyUpChar("i");
        Delay(200);

        //检查是否按成功
        dmobj->SetDict(0, FONTFIRE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("清理背包:背包:找字,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("清理背包:背包:找字,失败");
        Delay(500);
    }

    api_log_MsgDebug("清包:准备:点击");
    
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

        api_log_MsgDebug("整理背包:完毕,按i退出, buildx:%ld, buildy:%ld", buildx, buildy);
    }
    
    //重新按i,并记录背包I/II/III/IV/V的坐标位置
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //检查是否按成功
        dmobj->SetDict(0, FONTFIRE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("背包:字样:找字,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //bug:调整界面相对位置 2021/6/18 10:11:19
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

        api_log_MsgDebug("背包:字样:找字,失败");
        Delay(500);
    }

    api_log_MsgDebug("准备查找仓库图标");
    
    posX1 = 1069;
    posY1 = 48;

    //点击图标
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);

        //点击之后出现"仓库"字样
        dmobj->SetDict(0, FONTFIRE_DIR"font-surebox.txt");
        code = dmobj->FindStrFast(590, 500, 687, 530, "仓库", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("仓库:字样:找字,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("仓库:字样:找字,失败");
        Delay(500);
    }

    api_log_MsgDebug("准备点击仓库按钮, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
    
    //点击'仓库'字样
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);

        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONTFIRE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX1, &pposY1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX1 = pposX1.lVal;
            lposY1 = pposY1.lVal;

            api_log_MsgDebug("仓库:背包:字样:找字,成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
            break;
        }

        api_log_MsgDebug("仓库:背包:字样:找字,失败");
        Delay(500);
    }

    api_log_MsgDebug("准备:清理:背包");

    //处理背包中的东西
    fireSaveXstrong2Library(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    return SNAIL_ERRNO_SUCCESS;
}

static int fireBuyBlue(Idmsoft* dmobj, void* ctxt)
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
    
    api_log_MsgDebug("买蓝:开始");

    posX1 = 1069;
    posY1 = 48;

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        //点击之后出现"仓库"字样
        dmobj->SetDict(0, FONTFIRE_DIR"font-surebox.txt");
        code = dmobj->FindStrFast(590, 500, 687, 530, "仓库", "ffffff-101010", 0.9, &pposX1, &pposY1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX1 = pposX1.lVal;
            lposY1 = pposY1.lVal;

            api_log_MsgDebug("商店:仓库:字样:找字:成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
            break;
        }
        
        api_log_MsgDebug("商店:仓库:字样:找字:失败");
        Delay(500);
    }

    //点击商店 633:510->633:380
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX1, lposY1 - 130);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONTFIRE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("商店:背包:字样:找字:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("商店:背包:字样:找字:失败");
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    Delay(500);
        
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        //点击之后出现"商店"字样
        dmobj->SetDict(0, FONTFIRE_DIR"font-shop.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "商店", "ffffff-101010", 0.9, &pposX1, &pposY1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX1 = pposX1.lVal;
            lposY1 = pposY1.lVal;

            api_log_MsgDebug("商店:商店:字样:找字:成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
            break;
        }
        
        api_log_MsgDebug("商店:商店:字样:找字:失败");
        Delay(500);
    }

    //498:204 -> 658:278
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX1 + 160, lposY1 + 74);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTFIRE_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("商店:确认:字样:找字:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("商店:确认:字样:找字:失败");
        Delay(500);
    }

    //输入金额后点确认
    code = dmobj->SendString(hwnd, "22");
    if (1 != code)
    {
        api_log_MsgError("商店:金额:60:输入:失败");
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("密码:金额:60:输入:成功");

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTFIRE_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("商店:确认:字样:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("商店:确认:字样:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    api_log_MsgDebug("买蓝:完毕");
    return SNAIL_ERRNO_SUCCESS;
}

static int firecopyfull(Idmsoft* dmobj, void* ctxt)
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

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
    //检查副本中是否有大boss,如果是则会出现"因此无法入场"
    dmobj->SetDict(0, FONTFIRE_DIR"font-reenter.txt");
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
            dmobj->SetDict(0, FONTFIRE_DIR"font-enter-ack.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;

                api_log_MsgDebug("副本中有大boss进不去:确认:查找:成功, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
                break;
            }

            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
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

            dmobj->SetDict(0, FONTFIRE_DIR"font-enter-ack.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功");
                break;
            }

            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            LUAMAN_WAIT_MS(500);
            api_log_MsgDebug("副本中有大boss进不去:确认:点击:失败");
        }

        api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功:重新对话");
        return SNAIL_ERRNO_AGAIN;
    }

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    //没有进入副本,也没有出现"副本中有大boss进不去",则检查是否有"副本地下城满"
    dmobj->SetDict(0, FONTFIRE_DIR"font-copyfull.txt");
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

            dmobj->SetDict(0, FONTFIRE_DIR"font-copyfull.txt");
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

            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            api_log_MsgDebug("副本地下城已满:确认:点击:失败");
            LUAMAN_WAIT_MS(500);
        }

        api_log_MsgDebug("副本地下城已满:确认:点击:成功:重新对话");
        return SNAIL_ERRNO_AGAIN;;
    }
    
    return SNAIL_ERRNO_SUCCESS;
}

static int firefighttimeout(Idmsoft* dmobj, void* ctxt, int waittime, int fighttime)
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
        Delay_With_Terminate_Check_Alram(waittime, luaman_task, dmobj);
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
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
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

        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

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


static int fightatpoint_A2(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = taskfire_fight_atpoint_A2();

    api_log_MsgDebug("点A2:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)firefighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点A2:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint_A3(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = taskfire_fight_atpoint_A3();

    api_log_MsgDebug("点A3:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)firefighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点A3:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint_A4(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = taskfire_fight_atpoint_A4();

    api_log_MsgDebug("点A4:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)firefighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点A4:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint_A5(Idmsoft* dmobj, void* ctxt)
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

    api_log_MsgDebug("A5:打怪:开始");
    
    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(500, 30, 850, 130, "火之守护", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("火之守护:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(500, 30, 850, 130, "火之守护", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("火之守护:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(500, 30, 850, 130, "火之守护", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("火之守护:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("火之守护:查找:失败");
        Delay(500);

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > 60.0)
        {
            api_log_MsgDebug("A7:u怪:60秒:超时");
            return ERRNO_TIMEOUT;
        }
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "火之守护", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "火之守护", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi2.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "火之守护", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("火之守护:打怪:完成");
                    break;
                }
                
                Delay(500);
            }
        }

        api_log_MsgDebug("火之守护:打怪:继续");
    }

    for (idx = 0; idx < 50; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("A5:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint_A6(Idmsoft* dmobj, void* ctxt)
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

    api_log_MsgDebug("A6:打怪:开始");

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(500, 30, 850, 130, "愤怒的", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("愤怒的:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(500, 30, 850, 130, "愤怒的", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("愤怒的:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(500, 30, 850, 130, "愤怒的", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("愤怒的:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("愤怒的:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        dmobj->SetDict(0, FONTFIRE_DIR"font-TiShi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结束时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            api_log_MsgDebug("结束时间:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("结束时间:打怪:继续");
    }

    for (idx = 0; idx < 100; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("A6:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int firetask_A1(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_A[0].deltax;
    int deltaY = deltaatpos_fire_A[0].deltay;
    
    api_log_MsgDebug("任务A1:开始");

    /////////////////////////////////////////////////////////////////////////
    //阶段: A1
    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_A[0].deltax;
    deltaY = deltaatpos_fire_A[0].deltay;
    
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
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    dmobj->KeyDownChar("shift");
    Delay(200);
    dmobj->KeyDownChar("z");
    Delay(200);
    dmobj->KeyUpChar("shift");
    Delay(200);
    dmobj->KeyUpChar("z");
    Delay(200);

    deltaX = deltaatpos_mouse_A[0].deltax;
    deltaY = deltaatpos_mouse_A[0].deltay;

    //移动到鼠标位置
    dmobj->MoveTo(deltaX, deltaY);
    Delay(15000);

    //交替按3和右键
    for (index = 0; index < 30; index++)
    {
        dmobj->KeyDownChar("3");
        Delay(30);
        dmobj->KeyUpChar("3");
        Delay(30);
        
        dmobj->RightDown();
        Delay(30);
        dmobj->RightUp();
        Delay(30);
    }

    dmobj->KeyDownChar("shift");
    Delay(200);
    dmobj->KeyDownChar("x");
    Delay(200);
    dmobj->KeyUpChar("shift");
    Delay(200);
    dmobj->KeyUpChar("x");
    Delay(200);

    api_log_MsgDebug("任务A1:结束");
    return SNAIL_ERRNO_SUCCESS;
}

static int firetask_A2(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_A[1].deltax;
    int deltaY = deltaatpos_fire_A[1].deltay;
    
    api_log_MsgDebug("任务A2:开始");

    /////////////////////////////////////////////////////////////////////////
    //阶段: A1
    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_A[1].deltax;
    deltaY = deltaatpos_fire_A[1].deltay;
    
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
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //打怪1
    code = fightatpoint_A2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("任务A2:打怪:失败,code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("任务A2:打怪:成功");
    return SNAIL_ERRNO_SUCCESS;
}

static int firetask_A3(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_A[2].deltax;
    int deltaY = deltaatpos_fire_A[2].deltay;
    
    api_log_MsgDebug("任务A2:开始");

    /////////////////////////////////////////////////////////////////////////
    //阶段: A1
    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_A[2].deltax;
    deltaY = deltaatpos_fire_A[2].deltay;
    
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
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //打怪1
    code = fightatpoint_A3(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("任务A3:打怪:失败,code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("任务A3:打怪:成功");
    return SNAIL_ERRNO_SUCCESS;
}

static int firetask_A4(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_A[3].deltax;
    int deltaY = deltaatpos_fire_A[3].deltay;
    
    api_log_MsgDebug("任务A4:开始");

    /////////////////////////////////////////////////////////////////////////
    //阶段: A1
    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_A[3].deltax;
    deltaY = deltaatpos_fire_A[3].deltay;
    
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
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //打怪1
    code = fightatpoint_A4(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("任务A4:打怪:失败,code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("任务A4:打怪:成功");
    return SNAIL_ERRNO_SUCCESS;
}

static int firetask_A5(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_A[4].deltax;
    int deltaY = deltaatpos_fire_A[4].deltay;
    
    api_log_MsgDebug("任务A5:开始");

    /////////////////////////////////////////////////////////////////////////
    //阶段: A1
    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_A[4].deltax;
    deltaY = deltaatpos_fire_A[4].deltay;
    
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
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //打怪1
    code = fightatpoint_A5(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("任务A5:打怪:失败,code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("任务A5:打怪:成功");
    return SNAIL_ERRNO_SUCCESS;
}

static int firetask_A6(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_A[5].deltax;
    int deltaY = deltaatpos_fire_A[5].deltay;
    
    api_log_MsgDebug("任务A6:开始");

    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_A[5].deltax;
    deltaY = deltaatpos_fire_A[5].deltay;
    
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
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //打怪1
    code = fightatpoint_A6(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("任务A6:打怪:失败,code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("任务A6:打怪:成功");
    return SNAIL_ERRNO_SUCCESS;
}

static int firetask_A7(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_A[6].deltax;
    int deltaY = deltaatpos_fire_A[6].deltay;
    
    api_log_MsgDebug("任务A7:开始");

    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_A[6].deltax;
    deltaY = deltaatpos_fire_A[6].deltay;
    
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
    
    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

    //按N对话
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTFIRE_DIR"font-175DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("对话:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("对话:查找:失败");
        Delay(500);
    }

    //点击对话
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        //检查
        dmobj->SetDict(0, FONTFIRE_DIR"font-175DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "我知道", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("对话:我知道:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("对话:我知道:查找:失败");
        Delay(500);
    }
    
    //点击卢德妮
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTFIRE_DIR"font-175DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "我知道", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("我知道:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("我知道:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    api_log_MsgDebug("任务A7:打怪:成功");
    return SNAIL_ERRNO_SUCCESS;
}


static int firetask_A(Idmsoft* dmobj, void* ctxt)
{
    int code = 0;

    api_log_MsgDebug("firetask_A:开始");
    
    code = firetask_A1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        return code;
    }

    code = firetask_A2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        return code;
    }
    
    code = firetask_A3(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        return code;
    }

    code = firetask_A4(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        return code;
    }

    code = firetask_A5(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        if (ERRNO_TIMEOUT == code)
        {
            goto label_A7;
        }
        
        return code;
    }

    code = firetask_A6(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        return code;
    }

label_A7:
    code = firetask_A7(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("firetask_A:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("firetask_A:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint_B1(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = taskfire_fight_atpoint_B1();

    api_log_MsgDebug("点B1:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)firefighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点B1:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint_B2(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = taskfire_fight_atpoint_B2();

    api_log_MsgDebug("点B2:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)firefighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点B2:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint_B3(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = taskfire_fight_atpoint_B3();

    api_log_MsgDebug("点B3:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)firefighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点B3:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint_B4(Idmsoft* dmobj, void* ctxt)
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

    api_log_MsgDebug("B4:打怪:开始");
    
    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi4.txt");
        code = dmobj->FindStrFast(500, 30, 850, 130, "先知圣", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("先知圣:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi4.txt");
        code = dmobj->FindStrFast(500, 30, 850, 130, "先知圣", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("先知圣:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi4.txt");
        code = dmobj->FindStrFast(500, 30, 850, 130, "先知圣", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("先知圣:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("先知圣:查找:失败");
        Delay(500);

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > 60.0)
        {
            api_log_MsgDebug("B4:u怪:60秒:超时");
            return ERRNO_TIMEOUT;
        }
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "先知圣", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi4.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "先知圣", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi4.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "火之守护", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("先知圣:打怪:完成");
                    break;
                }
                
                Delay(500);
            }
        }

        api_log_MsgDebug("先知圣:打怪:继续");
    }

    for (idx = 0; idx < 50; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("B4:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int fightatpoint_B5(Idmsoft* dmobj, void* ctxt)
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

    api_log_MsgDebug("B5:打怪:开始");

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi5.txt");
        code = dmobj->FindStrFast(500, 30, 850, 130, "酷寒的", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("酷寒的:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi5.txt");
        code = dmobj->FindStrFast(500, 30, 850, 130, "酷寒的", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("酷寒的:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi5.txt");
        code = dmobj->FindStrFast(500, 30, 850, 130, "酷寒的", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("酷寒的:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("酷寒的:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        dmobj->SetDict(0, FONTFIRE_DIR"font-TiShi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结束时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            api_log_MsgDebug("结束时间:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("结束时间:打怪:继续");
    }

    for (idx = 0; idx < 100; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("B5:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}


static int firetask_B1(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_B[0].deltax;
    int deltaY = deltaatpos_fire_B[0].deltay;
    
    api_log_MsgDebug("任务B1:开始");

    /////////////////////////////////////////////////////////////////////////
    //阶段: 
    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_B[0].deltax;
    deltaY = deltaatpos_fire_B[0].deltay;
    
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
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    code = fightatpoint_B1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("任务B1:打怪:失败,code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("任务B1:结束");
    return SNAIL_ERRNO_SUCCESS;
}

static int firetask_B2(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_B[1].deltax;
    int deltaY = deltaatpos_fire_B[1].deltay;
    
    api_log_MsgDebug("任务B2:开始");

    /////////////////////////////////////////////////////////////////////////
    //阶段: 
    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_B[1].deltax;
    deltaY = deltaatpos_fire_B[1].deltay;
    
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
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    code = fightatpoint_B2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("任务B2:打怪:失败,code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("任务B2:结束");
    return SNAIL_ERRNO_SUCCESS;
}

static int firetask_B3(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_B[2].deltax;
    int deltaY = deltaatpos_fire_B[2].deltay;
    
    api_log_MsgDebug("任务B3:开始");

    /////////////////////////////////////////////////////////////////////////
    //阶段: 
    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_B[2].deltax;
    deltaY = deltaatpos_fire_B[2].deltay;
    
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
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    code = fightatpoint_B3(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("任务B3:打怪:失败,code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("任务B3结束");
    return SNAIL_ERRNO_SUCCESS;
}

static int firetask_B4(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_B[3].deltax;
    int deltaY = deltaatpos_fire_B[3].deltay;
    
    api_log_MsgDebug("任务B4:开始");

    /////////////////////////////////////////////////////////////////////////
    //阶段: A1
    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_B[3].deltax;
    deltaY = deltaatpos_fire_B[3].deltay;
    
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
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //打怪1
    code = fightatpoint_B4(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("任务B4:打怪:失败,code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("任务B4:打怪:成功");
    return SNAIL_ERRNO_SUCCESS;
}

static int firetask_B5(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_B[4].deltax;
    int deltaY = deltaatpos_fire_B[4].deltay;
    
    api_log_MsgDebug("任务B5:开始");

    /////////////////////////////////////////////////////////////////////////
    //阶段: A1
    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_B[4].deltax;
    deltaY = deltaatpos_fire_B[4].deltay;
    
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
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //打怪1
    code = fightatpoint_B5(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("任务B5:打怪:失败,code:%d", code);
        return code;
    }
    
    api_log_MsgDebug("任务B5:打怪:成功");
    return SNAIL_ERRNO_SUCCESS;
}

static int firetask_B6(Idmsoft* dmobj, void* ctxt)
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
    int deltaX = deltaatpos_fire_B[5].deltax;
    int deltaY = deltaatpos_fire_B[5].deltay;
    
    api_log_MsgDebug("任务B6:开始");

    
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire_B[5].deltax;
    deltaY = deltaatpos_fire_B[5].deltay;
    
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
    
    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

    //按N对话
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTFIRE_DIR"font-175DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("对话:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("对话:查找:失败");
        Delay(500);
    }

    //点击对话
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        //检查
        dmobj->SetDict(0, FONTFIRE_DIR"font-175DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "我知道", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("对话:我知道:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("对话:我知道:查找:失败");
        Delay(500);
    }
    
    //点击卢德妮
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTFIRE_DIR"font-175DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "我知道", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("我知道:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("我知道:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    return SNAIL_ERRNO_SUCCESS;
}


static int firetask_B(Idmsoft* dmobj, void* ctxt)
{
    int code = 0;

    code = firetask_B1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        return code;
    }

    code = firetask_B2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        return code;
    }

    code = firetask_B3(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        return code;
    }

    code = firetask_B4(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        if (ERRNO_TIMEOUT == code)
        {
            goto label_B6;
        }
        
        return code;
    }
    
    code = firetask_B5(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        return code;
    }

label_B6:
    code = firetask_B6(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        return code;
    }
    
    return SNAIL_ERRNO_SUCCESS;
}

static int execfiretask(Idmsoft* dmobj, void* ctxt, time_t* cleanPackageTimeLast, unsigned int flag1st)
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
    int deltaX = deltaatpos_fire[0].deltax;
    int deltaY = deltaatpos_fire[0].deltay;

    int totaltime = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;

    api_log_MsgDebug("副本:fire:任务:执行:开始");

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
        
        dmobj->SetDict( 0, FONTFIRE_DIR"font-auxiend.txt");
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-auxiend.txt");
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-auxiend.txt");
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

    api_log_MsgDebug("临界区:进入:等待");
    firecpyEnterCriticalSection();
    api_log_MsgDebug("临界区:进入:成功");
    
    code = firejoin2team(dmobj, ctxt);

    api_log_MsgDebug("临界区:退出:等待");
    firecpyLeaveCriticalSection();
    api_log_MsgDebug("临界区:退出:成功");
    
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("副本:fire:组队:失败,code:%d", code);
        return code;
    }

    api_log_MsgDebug("副本:fire:组队:成功");

LABEL_retalk_with_NPC:
    //按N对话
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        code = dmobj->FindPic(0, 27, 1279, 826, BMPFIRE_DIR"边框下箭头.bmp", "101010", 0.9, 0, &pposX, &pposY);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("对话:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("对话:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        //移动到箭头处
        dmobj->MoveTo(lposX, lposY);
        Delay(200);

        for (index = 0; index < 9; index++)
        {
            dmobj->LeftClick();
        }

        //检查
        dmobj->SetDict(0, FONTFIRE_DIR"font-175DuiHua1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "卢德妮", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("npc1:对话:卢德妮:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("npc1:对话:卢德妮:查找:失败");
        Delay(500);
    }
    
    //点击卢德妮
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTFIRE_DIR"font-outdoor.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc:对话:确认:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = firecopyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc:对话:确认:查找:副本地下城已满:重新对话");
            goto LABEL_retalk_with_NPC;
        }
        
        api_log_MsgDebug("npc:对话:确认:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTFIRE_DIR"font-outdoor.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("npc:对话:确认:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("npc:对话:确认:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        
        code = firecopyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc:对话:确认:点击:副本地下城已满:重新对话");
            goto LABEL_retalk_with_NPC;
        }
        
        api_log_MsgDebug("npc:对话:确认:点击:失败");
        Delay(500);
    }

    //等待进入圣魔之爪
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "卢德妮", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:卢德妮:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:卢德妮:查找:失败");
        Delay(1000);
    }

    //判断是否清包
    cleanPackageTime = luaman_config->cleanPackageTime;

    //海盗船副本 启动时清包，1:启动时清包, 0:启动时不清包
    //罗爱平 2023/1/22 22:03:11
    cleanPackageWhenStart = luaman_config->cleanPackageWhenStart;

    api_log_MsgDebug("清包:检查");
    api_log_MsgDebug("cleanPackageTimeLast  : %d", *cleanPackageTimeLast);
    api_log_MsgDebug("cleanPackageWhenStart : %d", cleanPackageWhenStart);
    
    if (1 == cleanPackageWhenStart && 0 == flag1st)
    {
        *cleanPackageTimeLast = 0;
    }
    
    api_log_MsgDebug("cleanPackageTimeLast  : %d", *cleanPackageTimeLast);

    //新增功能:增加清包处理逻辑
    if (0 == *cleanPackageTimeLast)
    {
        api_log_MsgDebug("启动时:清包:执行");
        
        fireCleanPackage(dmobj, ctxt);
        *cleanPackageTimeLast = time(NULL);
        firesellgabge(dmobj, ctxt);
        fireBuyBlue(dmobj, ctxt);
    }
    else
    {
        api_log_MsgDebug("运行时:清包:检查");
        
        curTime = time(NULL);
        cleanPackageTimeDelta = difftime(curTime, *cleanPackageTimeLast);

        api_log_MsgDebug("curTime               : %d", curTime);
        api_log_MsgDebug("cleanPackageTimeDelta : %f", cleanPackageTimeDelta);
        api_log_MsgDebug("cleanPackageTime      : %d", cleanPackageTime);
        
        if (cleanPackageTimeDelta >= (cleanPackageTime * 3600 * 1.0))
        {
            api_log_MsgDebug("运行时:清包:执行");
            
            fireCleanPackage(dmobj, ctxt);
            *cleanPackageTimeLast = time(NULL);
            firesellgabge(dmobj, ctxt);
            fireBuyBlue(dmobj, ctxt);
        }
    }

check_next_label:
    for (index = 0; index < 3; index++)
    {
        //判断坐标
        dmobj->SetDict(0, FONTFIRE_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 242) <= 3 && abs(posY - 220) <= 3)
                {
                    api_log_MsgDebug("目标:坐标:242:220:到达:成功");
                    goto label_A;
                }

                if (abs(posX - 312) <= 3 && abs(posY - 220) <= 3)
                {
                    api_log_MsgDebug("目标:坐标:312:220:到达:成功");
                    goto label_B;
                }
            }
        }

        api_log_MsgDebug("目标:坐标:未知:%d", index);
    }

    api_log_MsgDebug("对话:开始");
    
    //对话选择下一步
    //按一下减号键，等三秒
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-vip.txt");
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

    deltaX = deltaatpos_fire[0].deltax;
    deltaY = deltaatpos_fire[0].deltay;
    
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
    
    dmobj->MoveTo(5, 5);
    Delay(100);
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
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTFIRE_DIR"font-175DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("对话:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("对话:查找:失败");
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

        dmobj->SetDict(0, FONTFIRE_DIR"font-175DuiHua3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开启副本", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("开启副本:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }
        
        api_log_MsgDebug("开启副本:查找:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTFIRE_DIR"font-175DuiHua3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开启副本", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("开启副本:点击:成功");
            break;
        }
        
        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("开启副本:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    api_log_MsgDebug("对话:结束");
    goto check_next_label;

label_A:
    code = firetask_A(dmobj, ctxt);
    api_log_MsgDebug("firetask_A:完成:%d", code);
    goto finish;

label_B:
    firetask_B(dmobj, ctxt);

finish:
    //等待传出去
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
        dmobj->SetDict(0, FONTFIRE_DIR"font-175MingZhi6.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "德拉比斯", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:德拉比斯:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:德拉比斯:查找:失败");
        Delay(1000);
    }

    //寻路
    pathofposxys = "62:474|68:476|72:476";
    pathOfPosXYs = CString(pathofposxys);
    code = firewalkpath(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:74:477:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:74:477:成功");
    return SNAIL_ERRNO_SUCCESS;
}

static int execfiretaskloop(Idmsoft* dmobj, void* ctxt)
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
    unsigned int index = 0;

    api_log_MsgDebug("开始:cleanPackageTimeLast:%lld", cleanPackageTimeLast);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        execfiretask(dmobj, ctxt, &cleanPackageTimeLast, index);
        if (0 == index)
        {
            index++;
        }
    }
    
    return ERRNO_SUCCESS;
}

int api_TaskFireInit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;

    api_log_MsgDebug("副本:火本:初始化");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);

    api_PrintConfig(luaman_config);
    
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("副本:火本:窗口:绑定:失败, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("副本:火本:窗口:绑定:成功, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("窗口:鼠标:锁定:失败, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("窗口:鼠标:锁定:成功, hwnd:%ld", hwnd);

    /* 初始化告警时间初始值 */
    init_alarm_ctxt();
    
    return ERRNO_SUCCESS;
}

int api_TaskFireMain(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    
    api_log_MsgDebug("副本:火本:主循环:开始: hwnd:%ld, role:%d", hwnd, (int)role);

    execfiretaskloop(dmobj, ctxt);
    
    api_log_MsgDebug("副本:火本:主循环:完成, hwnd:%ld", hwnd);
    
    return ERRNO_SUCCESS;
}

int api_TaskFireExit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;

    api_log_MsgDebug("副本:火本:结束: hwnd:%ld, role:%d", hwnd, (int)role);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
    
    return ERRNO_SUCCESS;
}

