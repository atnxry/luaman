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
#include "task.h"
#include "taskPirate.h"

//打完副本出来后的寻路坐标,1  出来后，寻路（1）
static char* __outsize_walkpath = "155:327|147:326|139:325|129:323";

//M点击的相对位置: 2  （先拿锁）按-号，等3秒。按M，点击贵宾服务相对位置（可选）。按M。等1秒。按-号。
static posxy_t __m_offset[] =
{
    //主机1，主机2,...，主机N的M偏移
    {-218, 179},
    
};

//M点击的相对位置: 3  按-号，等3秒。按M，点击贵宾服务相对位置（1）。
static posxy_t __m_offset3 = {-297, 154};

//寻路坐标: 3  按-号，等3秒。按M，点击贵宾服务相对位置（1）。按M。等1秒。按-号。寻路（2）。
static char* __outsize_walkpath2 = "139:325|147:326|152:327";

///////////////////////////////////////////////////////////////////////////////////////////
//说明:各个坐标点等待、打怪时间配置结束
//配置方法:
// 1.如果增加一个等待、打怪时间循环
// 2.注意新增的行必须以'\'结尾,也就是说,'\'后不能有空格,tab或其他任意字符
#define pirate_fight_atpoint_405_314() {\
    pirate_fight_time(0, 15),\
}

#define pirate_fight_atpoint_422_269() {\
    pirate_fight_time(0, 25),\
}

#define pirate_fight_atpoint_459_405() {\
    pirate_fight_time(0, 30),\
}

#define pirate_fight_atpoint_458_436() {\
    pirate_fight_time(0, 20),\
}

#define pirate_fight_atpoint_287_439() {\
    pirate_fight_time(0, 15),\
}

#define pirate_fight_atpoint_292_381() {\
    pirate_fight_time(0, 45),\
}

#define pirate_fight_atpoint_464_110() {\
    pirate_fight_time(0, 45),\
}

#define pirate_fight_atpoint_382_92() {\
    pirate_fight_time(5, 10),\
}

#define pirate_fight_atpoint_177_28() {\
    pirate_fight_time(0, 60),\
}

#define pirate_fight_atpoint_217_209() {\
    pirate_fight_time(0, 60),\
}

#define pirate_fight_atpoint_284_253() {\
    pirate_fight_time(0, 60),\
}

#define pirate_fight_atpoint_194_145() {\
    pirate_fight_time(5, 15),\
}

#define pirate_fight_atpoint_171_244() {\
    pirate_fight_time(5, 15),\
}

#define pirate_fight_atpoint_181_320() {\
    pirate_fight_time(5, 15),\
}

#define pirate_fight_atpoint_185_377() {\
    pirate_fight_time(5, 15),\
}

#define pirate_fight_atpoint_137_434() {\
    pirate_fight_time(0, 50),\
}

#define pirate_fight_atpoint_63_299() {\
    pirate_fight_time(5, 10),\
}

#define pirate_fight_atpoint_64_183() {\
    pirate_fight_time(0, 10),\
}

//坐标点等待、打怪时间配置结束
///////////////////////////////////////////////////////////////////////////////////////////

#define FONTPIRATE_DIR "font\\pirate\\"
#define BMPPIRATE_DIR "bmp\\pirate\\"


static HANDLE __lock;
static void _EnterCriticalSection(void)
{
    ::WaitForSingleObject(__lock, INFINITE);
    api_log_MsgDebug("_EnterCriticalSection::加锁:成功");
    return;
}

static void _LeaveCriticalSection(void)
{
    (void)::ReleaseMutex(__lock);
    api_log_MsgDebug("_LeaveCriticalSection::解锁:成功");
    return;
}

static int pirateCheckkeystate(Idmsoft* dmobj, void* ctxt)
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

static int pirateChecklife(Idmsoft* dmobj, void* ctxt, int recusive)
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
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    
    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    return ERRNO_SUCCESS;
}


static int pirateWalk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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
    check_alarm(dmobj, ctxt);
    
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
            check_alarm(dmobj, ctxt);
            
            //////////////////////////////////////////////////////////////////////////////
            //@bug:检查寻路时间是否超过阈值120秒,如果超过则认为是被卡住了,退出 2019/4/20 10:09:39
            //bug:修改为40秒 2022/01/10 21:57
            curTime = time(NULL);
            walkTimeDelta = difftime(curTime, startWalkTime);
            if (walkTimeDelta > 300.0)
            {
                (void)pirateCheckkeystate(dmobj, ctxt);

                api_log_MsgDebug("寻路:坐标:卡住,停止寻路:等待副本消失,下一坐标, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //等待地洞副本消失
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //先检查下血条
            code = pirateChecklife(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //读取当前坐标
            dmobj->SetDict(0, FONTPIRATE_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)pirateCheckkeystate(dmobj, ctxt);
                
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

                (void)pirateCheckkeystate(dmobj, ctxt);
                
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
                check_alarm(dmobj, ctxt);
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            //WALK_LABEL_NEXT
        }//end:前往下一目标坐标点

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
    }

    //寻路过程完毕,顺利到达目标终点坐标点
    return SNAIL_ERRNO_SUCCESS;

LABEL_WALK_BLOCK_EXIT_CHECK:
    //检查地图是否退出到副本外面
    for (;;)
    {
        //先检查下血条
        code = pirateChecklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:卡住:血条:检查:死亡:复活");
            return code;
        }
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-map.txt");
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
        check_alarm(dmobj, ctxt);
        
        Delay(500);
        api_log_MsgDebug("寻路:卡住:地图:平静河谷:退出,正在退出中");
    }

    return SNAIL_ERRNO_BLOCKED;
}

static int pirateFighttimeout(Idmsoft* dmobj, void* ctxt, int waittime, int fighttime)
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
        check_alarm(dmobj, ctxt);
    }

    api_log_MsgDebug("打怪:等待:%d秒:结束", waittime);

    if (fighttime <= 0)
    {
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("打怪:%d秒:跳过", fighttime);
        return SNAIL_ERRNO_SUCCESS;
    }
    
    api_log_MsgDebug("打怪:%d秒:开始", fighttime);
    
    lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
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
        check_alarm(dmobj, ctxt);
        
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

    check_alarm(dmobj, ctxt);
    
    api_log_MsgDebug("打怪:%d秒:结束", fighttime);
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint1_01(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_405_314();

    api_log_MsgDebug("405:314:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("405:314:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}
static int pirateFightatPoint1(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_422_269();

    api_log_MsgDebug("422:269:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("422:269:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}


static int pirateFightatPoint2_01(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_459_405();

    api_log_MsgDebug("459:405:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("459:405:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}
static int pirateFightatPoint2(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_458_436();

    api_log_MsgDebug("458:436:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("458:436:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint3(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_287_439();

    api_log_MsgDebug("287:439:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("287:439:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint4(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_292_381();

    api_log_MsgDebug("292:381:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("292:381:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint5(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_464_110();

    api_log_MsgDebug("464:110:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("464:110:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint6(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_382_92();

    api_log_MsgDebug("382:92:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("382:92:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint7(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_177_28();

    api_log_MsgDebug("177:28:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("177:28:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint8(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_217_209();

    api_log_MsgDebug("217:209:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("217:209:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint9(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_284_253();

    api_log_MsgDebug("284:253:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("284:253:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint10(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_194_145();

    api_log_MsgDebug("194:145:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("194:145:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint11(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_171_244();

    api_log_MsgDebug("171:244:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("171:244:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}


static int pirateFightatPoint12_01(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_181_320();

    api_log_MsgDebug("181:320:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("181:320:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint12(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_185_377();

    api_log_MsgDebug("185:377:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("185:377:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint13(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_137_434();

    api_log_MsgDebug("137:434:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("137:434:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint14(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_63_299();

    api_log_MsgDebug("63:299:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("63:299:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint15(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = pirate_fight_atpoint_64_183();

    api_log_MsgDebug("64:183:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("64:183:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint16(Idmsoft* dmobj, void* ctxt)
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
    int uflag = 0;
    int failrnr = 0;
    int i = 0;

    api_log_MsgDebug("海德罗:打怪:开始");
    
    for (;;)
    {
    Label_FightatPoint16_start:

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        if (!uflag || 2 == uflag)
        {
            dmobj->MoveTo(5, 5);
            Delay(100);

            failrnr = 0;
            
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                dmobj->KeyDownChar("u");
                Delay(20);
                dmobj->KeyUpChar("u");
                Delay(200);

                dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi3.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "地狱海罗德", "ffffff-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uflag = 1;
                    
                    api_log_MsgDebug("地狱海罗德:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

				dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi3.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "地狱海罗德", "ff3300-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uflag = 1;
                    
                    api_log_MsgDebug("地狱海罗德:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                failrnr++;
                if (2 == uflag && failrnr > 5)
                {
                    api_log_MsgDebug("海德罗:打怪:超时:退出");
                    return SNAIL_ERRNO_SUCCESS;
                }
                
                api_log_MsgDebug("地狱海罗德:查找:失败");
                Delay(200);
            }
        }

        api_log_MsgDebug("地狱海罗德:攻击:发起");
        
        lastChckTime = time(NULL);
        
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            for (idx = 0; idx < 20; idx++)
            {
                dmobj->KeyDownChar("3");
                Delay(15);
                dmobj->KeyUpChar("3");
                Delay(15);
            }

            curTime = time(NULL);
            deltaTime = difftime(curTime, lastChckTime);
            if (deltaTime > 3.0)
            {
                api_log_MsgDebug("地狱海罗德:攻击:暂停");
                break;
            }
       }

        dmobj->MoveTo(5, 5);
        Delay(100);
        
       //检查有没有打死
       for (;;)
       {
			Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
			for(i = 0; i < 2; i++)
			{
				Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
				dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi3.txt");
				code = dmobj->FindStrFast(0, 27, 1279, 826, "地狱海罗德", "ffffff-101010", 0.9, &pposX, &pposY);
				dmobj->ClearDict(0);
				if (-1 != code)
				{
					lposX = pposX.lVal;
					lposY = pposY.lVal;
                
					api_log_MsgDebug("海德罗:白色:攻击:继续, lposX:%ld, lposY:%ld", lposX, lposY);
					goto Label_FightatPoint16_start;
				}

				dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi3.txt");
				code = dmobj->FindStrFast(0, 27, 1279, 826, "地狱海罗德", "ff3300-101010", 0.9, &pposX, &pposY);
				dmobj->ClearDict(0);
				if (-1 != code)
				{
					lposX = pposX.lVal;
					lposY = pposY.lVal;
                
					api_log_MsgDebug("海德罗:红色:攻击:继续, lposX:%ld, lposY:%ld", lposX, lposY);
					goto Label_FightatPoint16_start;
				 }
			}

            //找一下诅咒时间
            dmobj->SetDict(0, FONTPIRATE_DIR"font-TiShi1.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "化身发动", "fdb602-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                uflag = 2;
                failrnr = 0;
                
                api_log_MsgDebug("海德罗:攻击:化身发动:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("海德罗:打怪:完成");
            return SNAIL_ERRNO_SUCCESS;
       }
    }
    
    api_log_MsgDebug("海德罗:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint17(Idmsoft* dmobj, void* ctxt)
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
    int uflag = 0;
    
    api_log_MsgDebug("特罗斯:打怪:开始");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        if (!uflag)
        {
            dmobj->MoveTo(5, 5);
            Delay(100);
        
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                dmobj->KeyDownChar("u");
                Delay(20);
                dmobj->KeyUpChar("u");
                Delay(200);

                dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi4.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "愤怒至极的特罗斯", "ffffff-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uflag = 1;
                    
                    api_log_MsgDebug("愤怒至极的特罗斯:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi4.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "愤怒至极的特罗斯", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uflag = 1;
                    
                    api_log_MsgDebug("愤怒至极的特罗斯:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi4.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "愤怒至极的特罗斯", "ff3300-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uflag = 1;
                    
                    api_log_MsgDebug("愤怒至极的特罗斯:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }
                
                api_log_MsgDebug("愤怒至极的特罗斯:查找:失败");
                Delay(200);
            }
        }

        api_log_MsgDebug("愤怒至极的特罗斯:攻击:发起");
        
        lastChckTime = time(NULL);
        
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            for (idx = 0; idx < 20; idx++)
            {
                dmobj->KeyDownChar("3");
                Delay(15);
                dmobj->KeyUpChar("3");
                Delay(15);
            }

            curTime = time(NULL);
            deltaTime = difftime(curTime, lastChckTime);
            if (deltaTime > 3.0)
            {
                api_log_MsgDebug("特罗斯:攻击:暂停");
                break;
            }
       }

        dmobj->MoveTo(5, 5);
        Delay(100);
        
       //检查有没有打死
       for (;;)
       {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->SetDict(0, FONTPIRATE_DIR"font-TiShi2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "离开时间", "fdb602-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("离开时间:查找:成功:攻击:结束, lposX:%ld, lposY:%ld", lposX, lposY);
                goto Label_end_fight17;
            }

            api_log_MsgDebug("离开时间:查找:失败:攻击:继续");
            break;
       }
    }

Label_end_fight17:
    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }
    
    api_log_MsgDebug("愤怒至极的特罗斯:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateSaveXstrong2Library(Idmsoft* dmobj, void* ctxt)
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
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        //检查是否按成功
        dmobj->SetDict(0, FONTPIRATE_DIR"font-package.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
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
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
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
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            for (jndex = 0; jndex < 10; jndex++)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                posX = boxPosXYs[index * 10 + jndex].x;
                posY = boxPosXYs[index * 10 + jndex].y;

                dmobj->MoveTo(posX, posY);
                Delay(200);

                api_log_MsgDebug("处理:格尼亚罗:背包:%d:柜子:[%d,%d]:坐标:[%ld, %ld]",
                    lndex, index, jndex, posX, posY);
                
                findxstrong = 0;

                //查找'小强|中强|高强'字样
                dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi7.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "格尼亚罗", "ffffff-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    findxstrong = 1;

                    lposX1 = pposX1.lVal;
                    lposY1 = pposY1.lVal;

                    Delay(200);
                    dmobj->RightClick();
                    Delay(200);

                    api_log_MsgDebug("宝物柜:格尼亚罗:存储,成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                    //break;
                }

                if (0 == findxstrong)
                {
                    dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi7.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "海伯伦戒指", "a335ee-101010", 0.9, &pposX1, &pposY1);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        findxstrong = 2;

                        lposX1 = pposX1.lVal;
                        lposY1 = pposY1.lVal;

                        Delay(200);
                        dmobj->RightClick();
                        Delay(200);

                        api_log_MsgDebug("宝物柜:海伯伦戒指:存储,成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                        //break;
                    }
                }
            }
        }
    }

    return SNAIL_ERRNO_SUCCESS;
}
static int pirateCleanPackage(Idmsoft* dmobj, void* ctxt)
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
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(200);
        dmobj->KeyUpChar("i");
        Delay(200);

        //检查是否按成功
        dmobj->SetDict(0, FONTPIRATE_DIR"font-package.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //检查是否按成功
        dmobj->SetDict(0, FONTPIRATE_DIR"font-package.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);

        //点击之后出现"仓库"字样
        dmobj->SetDict(0, FONTPIRATE_DIR"font-surebox.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);

        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONTPIRATE_DIR"font-package.txt");
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
    pirateSaveXstrong2Library(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateBuyBlue(Idmsoft* dmobj, void* ctxt)
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
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        //点击之后出现"仓库"字样
        dmobj->SetDict(0, FONTPIRATE_DIR"font-surebox.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX1, lposY1 - 130);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONTPIRATE_DIR"font-package.txt");
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
        //点击之后出现"商店"字样
        dmobj->SetDict(0, FONTPIRATE_DIR"font-shop.txt");
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
        dmobj->MoveTo(lposX1 + 160, lposY1 + 74);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-okcancel.txt");
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
    code = dmobj->SendString(hwnd, "200");
    if (1 != code)
    {
        api_log_MsgError("商店:金额:200:输入:失败");
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("密码:金额:200:输入:成功");

    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-okcancel.txt");
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

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    api_log_MsgDebug("买蓝:完毕");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateCheckcopyfull(Idmsoft* dmobj, void* ctxt)
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
    check_alarm(dmobj, ctxt);
    
    //检查副本中是否有大boss,如果是则会出现"因此无法入场"
    dmobj->SetDict(0, FONTPIRATE_DIR"font-reenter.txt");
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
            dmobj->SetDict(0, FONTPIRATE_DIR"font-reenter.txt");
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
            check_alarm(dmobj, ctxt);
            
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

            dmobj->SetDict(0, FONTPIRATE_DIR"font-reenter.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功");
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            LUAMAN_WAIT_MS(500);
            api_log_MsgDebug("副本中有大boss进不去:确认:点击:失败");
        }

        api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功:重新对话");
        return SNAIL_ERRNO_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //没有进入副本,也没有出现"副本中有大boss进不去",则检查是否有"副本地下城满"
    dmobj->SetDict(0, FONTPIRATE_DIR"font-copyfull.txt");
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

            dmobj->SetDict(0, FONTPIRATE_DIR"font-copyfull.txt");
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
            check_alarm(dmobj, ctxt);
            
            api_log_MsgDebug("副本地下城已满:确认:点击:失败");
            LUAMAN_WAIT_MS(500);
        }

        api_log_MsgDebug("副本地下城已满:确认:点击:成功:重新对话");
        return SNAIL_ERRNO_AGAIN;;
    }
    
    return SNAIL_ERRNO_SUCCESS;
}
static int pirateTask(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX0 = 0, lposY0 = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int idx = 0;
    int retry_npc_num = 0;
    int alarmHostIndex = luaman_config->alarmHostIndex;

LABEL_start_this_copy:
    pathofposxys = __outsize_walkpath;
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", __outsize_walkpath);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", __outsize_walkpath);

    _EnterCriticalSection();//加锁

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    if (alarmHostIndex < 0 || alarmHostIndex >= LUAMAN_DIM(__m_offset))
    {
        _LeaveCriticalSection();
        
        for (;;)
        {
            api_log_MsgWarn("贵宾服务:M点击:偏移位置:获取:异常", alarmHostIndex);
            Delay(1000);
        }
    }

    api_log_MsgDebug("贵宾服务:M点击:偏移位置:主机:%d, x:%lld, y:%lld",
        alarmHostIndex,
        __m_offset[alarmHostIndex].x,
        __m_offset[alarmHostIndex].y);
    
    // 858:158 -> 590:642
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX + __m_offset[alarmHostIndex].x, lposY + __m_offset[alarmHostIndex].y);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->MoveTo(5, 5);
        Delay(100);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-fartm.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "加入远征队", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("加入远征队:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("加入远征队:查找:失败");
        Delay(500);
    }

    dmobj->KeyDownChar("enter");
    Delay(500);
    dmobj->KeyUpChar("enter");
    Delay(500);
    
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 590:642
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX + __m_offset3.x, lposY + __m_offset3.y);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);
    
    pathofposxys = __outsize_walkpath2;
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        _LeaveCriticalSection();
        
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", __outsize_walkpath2);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", __outsize_walkpath2);
    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤1
    /////////////////////////////////////////////////////////////////////////////////
LABEL_retalk_with_NPC:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        (void)check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "ffffff-101010", 0.9, &pposX, &pposY);
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

    //对话:    638:108 -> 638:494
    //delta: 0:386
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY + 386);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        code = pirateCheckcopyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("确认:查找:副本满:检查:重试");
            goto LABEL_retalk_with_NPC;
        }

        code = common_check_copy_access(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            _LeaveCriticalSection();
            
            api_log_MsgDebug("人员不足:检查:成功:重试");
            goto LABEL_start_this_copy;
        }
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-okcancel.txt");
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
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        code = pirateCheckcopyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("确认:点击:副本满:检查:重试");
            goto LABEL_retalk_with_NPC;
        }

        code = common_check_copy_access(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            _LeaveCriticalSection();
            
            api_log_MsgDebug("人员不足:检查:成功:重试");
            goto LABEL_start_this_copy;
        }
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 ==code)
        {
            api_log_MsgDebug("确认:点击:成功");
            
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("确认:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    _LeaveCriticalSection();//解锁
    
    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    double cleanPackageTimeDelta = 0.0;
    time_t curTime = time(NULL);
    int cleanPackageTime =2;// luaman_config->cleanPackageTime;
    time_t cleanPackageTimeLast = time(NULL);

    //海盗船副本 启动时清包，1:启动时清包, 0:启动时不清包
    //罗爱平 2023/1/22 22:03:11
    int cleanPackageWhenStart = 1; //luaman_config->cleanPackageWhenStart;

    if (1 == cleanPackageWhenStart)
    {
        cleanPackageTimeLast = 1;
    }
    
    api_log_MsgDebug("副本:开始");

    //新增功能:增加清包处理逻辑
    if (0 == cleanPackageTimeLast)
    {
        pirateCleanPackage(dmobj, ctxt);
        cleanPackageTimeLast = time(NULL);
        pirateBuyBlue(dmobj, ctxt);
    }
    else
    {
        curTime = time(NULL);
        cleanPackageTimeDelta = difftime(curTime, cleanPackageTimeLast);
        if (cleanPackageTimeDelta >= (cleanPackageTime * 3600 * 1.0))
        {
            pirateCleanPackage(dmobj, ctxt);
            cleanPackageTimeLast = time(NULL);
            pirateBuyBlue(dmobj, ctxt);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤2
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "381:407|381:394";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[381:394]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[381:394]:成功");
    
    //等待,当出现"副本进行时间"时开始行走
    for (;;)
    {
        dmobj->SetDict(0, FONTPIRATE_DIR"font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本进行时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("副本进行时间:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("副本进行时间:查找:失败");
        LUAMAN_WAIT_MS(1000);
    }

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤3
    /////////////////////////////////////////////////////////////////////////////////
	pathofposxys = "382:383|385:373|390:359|394:347|399:336|401:325|405:314";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[405:314]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[405:314]:成功");
    
    code = pirateFightatPoint1_01(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[405:314]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[405:314]:打怪:成功");

    pathofposxys = "409:303|413:292|416:280|422:269";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[422:269]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[422:269]:成功");
    
    code = pirateFightatPoint1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[422:269]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[422:269]:打怪:成功");
    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤4
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 590:642
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 268, lposY + 484);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

	pathofposxys = "461:359|461:371|460:381|459:393|459:405";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[459:405]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[459:405]:成功");

    code = pirateFightatPoint2_01(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[458:436]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[459:405]:打怪:成功");

    pathofposxys = "459:415|459:424|458:436";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[458:436]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[458:436]:成功");

    code = pirateFightatPoint2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[458:436]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[458:436]:打怪:成功");
    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤5
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 707:487
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 151, lposY + 329);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = "310:471|303:464|294:452|287:439";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[287:439]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[287:439]:成功");

    code = pirateFightatPoint3(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[287:439]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[287:439]:打怪:成功");

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤6
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "280:427|275:417|284:411|287:400|290:390|292:381";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[292:381]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[292:381]:成功");

    code = pirateFightatPoint4(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[292:381]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[292:381]:打怪:成功");

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤7
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 354:567
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 504, lposY + 409);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = "399:116";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[399:116]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[399:116]:成功");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "查看", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("查看:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("查看:查找:失败");
        Delay(500);
    }

    //点击查看
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至召唤", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("移动至召唤:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("移动至召唤:查找:失败");
        Delay(500);
    }

    //点击移动至召唤
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至召唤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至召唤:点击:成功");
            break;
        }
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("移动至召唤:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    pathofposxys = "458:114|464:110";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[464:110]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[464:110]:成功");
    
    code = pirateFightatPoint5(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[464:110]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[464:110]:打怪:成功");
    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤8
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 354:567
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 504, lposY + 409);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = "391:117|390:107|387:99|382:92";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[382:92]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[382:92]:成功");

    code = pirateFightatPoint6(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[382:92]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[382:92]:打怪:成功");

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤9
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "376:86|370:88|365:90|359:92|353:95|344:95|339:95|336:95";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[336:95]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[336:95]:成功");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "查看", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("查看:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("查看:查找:失败");
        Delay(500);
    }

    //点击查看
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "查看", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("查看:点击:成功");
            break;
        }
        
        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("查看:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤10
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 266:355
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 592, lposY + 197);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    code = pirateFightatPoint7(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[177:28]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[177:28]:打怪:成功");
    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤11
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 448:400
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 410, lposY + 242);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    code = pirateFightatPoint8(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[217:209]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[217:209]:打怪:成功");
    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤12
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 492:463
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 366, lposY + 305);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    code = pirateFightatPoint9(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[284:253]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[284:253]:打怪:成功");

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤13
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 353:383
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 526, lposY + 335);// 罗爱平 修改:按相对位置
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    dmobj->KeyPressChar("u");
    Delay(500);
    dmobj->KeyUpChar("u");
    Delay(500);

    dmobj->KeyPressChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////


    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 353:383
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 505, lposY + 225);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = "207:110|200:119|199:129|198:136|194:145";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[194:145]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[194:145]:成功");

    code = pirateFightatPoint10(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[194:145]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[194:145]:打怪:成功");

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤14
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "188:153|183:159|178:169|172:180|169:191|167:204|167:213|167:222|169:233|171:244";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[171:244]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[171:244]:成功");

    code = pirateFightatPoint11(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[171:244]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[171:244]:打怪:成功");
    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤15
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "174:255|176:266|177:280|179:290|181:305|181:320";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[181:320]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[181:320]:成功");

    code = pirateFightatPoint12_01(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[181:320]:打怪:失败:code:%d", code);
        return code;
    }

	pathofposxys = "179:335|180:348|181:357|184:364|185:377";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[185:377]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[185:377]:成功");

    code = pirateFightatPoint12(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[185:377]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[185:377]:打怪:成功");

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤16
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "180:395|175:402|168:409|162:416|153:423|145:429|137:434";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[137:434]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[137:434]:成功");

    code = pirateFightatPoint13(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[137:434]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[137:434]:打怪:成功");
    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤17
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "126:436|119:434";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[119:434]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[119:434]:成功");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua9.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("对话:查找:绿色:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        
        api_log_MsgDebug("对话:查找:失败");
        Delay(500);
    }
    
    //点击对话
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "我要移动", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("我要移动:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("我要移动:查找:失败");
        Delay(500);
    }
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "我要移动", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("我要移动:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("我要移动:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤18
    /////////////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "操纵杆", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("操纵杆:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("操纵杆:查找:失败");
        Delay(500);
    }

    //866:31 -> 869:85
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX + 3, lposY + 54);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    }

    Delay_With_Terminate_Check(2, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    pathofposxys = "61:308|63:304|63:299";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[63:299]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[63:299]:成功");
    
    code = pirateFightatPoint14(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[63:299]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[63:299]:打怪:成功");
    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤19
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 476:233
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 382, lposY + 75);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    pathofposxys = "59:230|63:227|64:224|64:219|69:219|72:214|70:207:72:199|73:190|69:186|64:183";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[64:183]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[64:183]:成功");
    
    code = pirateFightatPoint15(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[64:183]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[64:183]:打怪:成功");
    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤20
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 311:239
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 547, lposY + 81);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    code = pirateFightatPoint16(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("海罗德:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("海罗德:打怪:成功");

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤21
    /////////////////////////////////////////////////////////////////////////////////
    api_log_MsgDebug("海罗德:按空格:30下:开始");
    
    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("海罗德:按空格:30下:结束");

    int retry_Label_leave_time = 0;
    
    //找副本离开时间或者消灭时间
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-TiShi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "离开时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("离开时间:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            goto Label_leave_time;
        }

        dmobj->SetDict(0, FONTPIRATE_DIR"font-TiShi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "消灭时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("消灭时间:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            goto Label_die_time;
        }
        

        api_log_MsgDebug("离开时间:消灭时间:查找:失败");
        Delay(500);
    }

    //不可能走到这里
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("死循环:不可能:到这里");
        Delay(1000);
    }

Label_leave_time:
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 273:242
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 585, lposY + 84);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    retry_npc_num = 0;
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua9.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("对话:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        if (retry_Label_leave_time > 1)
        {
            api_log_MsgDebug("对话:查找:失败:超时:跳转:等待:传出:副本:%d", retry_Label_leave_time);
            goto Label_term_copy;
        }

        retry_npc_num++;
        if (retry_npc_num > 5)
        {
            api_log_MsgDebug("对话:查找:失败:超时:重试:%d", retry_Label_leave_time);
            
            retry_Label_leave_time++;
            goto Label_leave_time;
        }
        
        api_log_MsgDebug("对话:查找:失败");
        Delay(500);
    }

    //点击对话
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "想要召唤隐藏", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("想要召唤隐藏:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("想要召唤隐藏:查找:失败");
        Delay(500);
    }

    //点击
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "想要和隐藏", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("想要和隐藏:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("想要和隐藏:查找:失败");
        Delay(500);
    }

	//点击
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "想要和隐藏", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("想要和隐藏:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("想要和隐藏:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤22
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "64:34";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[64:34]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[64:34]:成功");

    code = pirateFightatPoint17(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[64:34]:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[64:34]:打怪:成功");
    goto Label_continue_Next_Cpy;
    
Label_die_time:
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤23
    /////////////////////////////////////////////////////////////////////////////////
    code = pirateFightatPoint17(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("消灭时间:打怪:失败:code:%d", code);
        return code;
    }

    api_log_MsgDebug("消灭时间:打怪:成功");

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
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

    // 858:158 -> 273:242
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 585, lposY + 84);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //关掉M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("贵宾服务:关闭:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("贵宾服务:关闭:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
Label_continue_Next_Cpy:
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤24
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "64:31";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[64:31]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[64:31]:成功");

    dmobj->KeyPressChar("9");
    Delay(500);
    dmobj->KeyUpChar("9");
    Delay(500);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua9.txt");
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

    //点击查看
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua7.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "只是做了该做的", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("只是做了该做的:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("只是做了该做的:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "谢谢", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("谢谢:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("谢谢:查找:失败");
        Delay(500);
    }

    //点击谢谢
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "谢谢", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("谢谢:点击:成功");
            break;
        }
        
        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("谢谢:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

Label_term_copy:
    dmobj->MoveTo(5, 5);
    Delay(200);
        
    //出来到外头了
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi1.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "古林群岛", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("古林群岛:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("古林群岛:查找:失败");
        Delay(500);
    }

    api_log_MsgDebug("副本:结束");
    api_log_MsgDebug("副本:结束:等待:27秒:开始");
    
    //Delay_With_Terminate_Check(27, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    api_log_MsgDebug("副本:结束:等待:27秒:结束");
    
    
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////
int api_TaskPirateInit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    time_t start = time(NULL);
    
    api_log_MsgDebug("副本:Pirate:初始化: hwnd:%ld, role:%d", hwnd, (int)role);
    api_PrintConfig(luaman_config);

    /* 绑定窗口 */
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("bind Pirate hwnd failed, hwnd:%ld", hwnd);

        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("bind Pirate hwnd success, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("lock Pirate hwnd failed, hwnd:%ld", hwnd);

        return ERRNO_BINDWND;
    }

    /* 初始化告警时间初始值 */
    init_alarm_ctxt();
    
    api_log_MsgDebug("副本:Pirate:初始化:完成, hwnd:%ld", hwnd);   
    return ERRNO_SUCCESS;
}

int api_TaskPirateMain(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    int j = 0;
    
    api_log_MsgDebug("副本:Pirate:主循环: hwnd:%ld, role:%d", hwnd, (int)role);

    for (j = 0; j < luaman_config->loop_num; j++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        api_log_MsgDebug("执行副本,j:%ld, loop_num:%d", j, luaman_config->loop_num);
        
        code = pirateTask(dmobj, ctxt);
        if (code)
        {
            api_log_MsgDebug("执行副本完毕,j:%ld, loop_num:%d", j, luaman_config->loop_num);
            break;
        }
    }

    api_log_MsgDebug("副本:Pirate:主循环:结束, hwnd:%ld, role:%d", hwnd, (int)role);
    return ERRNO_SUCCESS;
}

int api_TaskPirateExit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;

    api_log_MsgDebug("副本:Pirate:结束: hwnd:%ld, role:%d", hwnd, (int)role);
    return ERRNO_SUCCESS;
}

int api_TaskPiratePrepare(void)
{
    __lock = CreateMutex(NULL, FALSE, NULL);
    if (!__lock)
    {
        api_log_MsgError("api_TaskPiratePrepare failed");
        exit(1);
    }

    api_log_MsgDebug("api_TaskTeamgroupPrepare success");
    return 0;
}

