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
#include "taskSakura.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"

#define FONTSAKURA_DIR "font\\Sakura\\"
#define BMPSAKURA_DIR "bmp\\Sakura\\"


int api_TaskSakuraInit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int bindWnd = luaman_config->curBindWnd;
    
    api_log_MsgDebug("副本:Sakura:初始化");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    api_PrintConfig(luaman_config);
    
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("窗口:绑定:失败, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("窗口:绑定:成功, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("窗口:鼠标:锁定:失败, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("窗口:鼠标:锁定:成功, hwnd:%ld", hwnd);
    
    return ERRNO_SUCCESS;
}

static int sakura_checkkeystate(Idmsoft* dmobj, void* ctxt)
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

static int sakura_walk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir);

static int sakura_checklife(Idmsoft* dmobj, void* ctxt, int recusive)
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
    
    dmobj->SetDict(0, FONTSAKURA_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("蓝血条:读取:异常, groupid:%d, hwnd:%ld", luaman_config->groupid, hwnd);
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

        api_log_MsgDebug("血条:解析:成功,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
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

    api_log_MsgDebug("血条:检查:死亡,等待出现接受字样");

    sakura_checkkeystate(dmobj, ctxt);

    if (recusive)
    {
        return SNAIL_ERRNO_AGAIN;
    }
    
    //等待出现死亡字样
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("死亡:复活:接受:找字:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("血条:检查:死亡:接受:找字:失败");
        Delay(500);
    }
    
    //点击接受
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-000000", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("血条:检查:死亡:接受:点击,成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("血条:检查:死亡:接受:点击,失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //等待复活站起来
    for (;;)
    {
        code = sakura_checklife(dmobj, ctxt, 1);
        if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("复活:站立:完成");
            break;
        }

        Delay_With_Terminate_Check(1, luaman_task, dmobj);
    }

    //寻路
    pathofposxys = "300:118";
    pathOfPosXYs = CString(pathofposxys);
    code = sakura_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("复活:寻路:前往:坐标:[300:118]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("复活:寻路:前往:坐标:[300:118]:成功");
    
    return SNAIL_ERRNO_REALIVE;
}

static int sakura_walk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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
            if (walkTimeDelta > 30.0)
            {
                (void)sakura_checkkeystate(dmobj, ctxt);

                api_log_MsgDebug("寻路:坐标:卡住,停止寻路:等待副本消失,下一坐标, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //等待地洞副本消失
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //先检查下血条
            code = sakura_checklife(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //读取当前坐标
            dmobj->SetDict(0, FONTSAKURA_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)sakura_checkkeystate(dmobj, ctxt);
                
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

                (void)sakura_checkkeystate(dmobj, ctxt);
                
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
        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:卡住:血条:检查:死亡:复活");
            return code;
        }
        
        dmobj->SetDict(0, FONTSAKURA_DIR"font-map.txt");
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

static int sakura_fight(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int groupid = luaman_config->groupid;
    int code = 0;
    VARIANT pposX, pposY;
    //VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    //long lposX2 = 0, lposY2 = 0;
    int cindex = 0;
    int index = 0;
    
    //边u边打,直到出现退出条件
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = dmobj->FindPic(968, 194, 1110, 210, BMPSAKURA_DIR"lastblood.bmp", "101010", 0.9, 0, &pposX, &pposY);
        if (-1 == code)
        {
            dmobj->KeyDownChar("u");
            Delay(200);
            dmobj->KeyUpChar("u");
            Delay(200);

            api_log_MsgDebug("定点:u怪");
        }
        
        for (cindex = 0; cindex < 2; ++cindex)
        {
            //bug:打怪按3~9 2022/01/02 21:55:13
            for (index = 3; index < 10; ++index)
            {
                dmobj->KeyDown(48 + index);
                Delay(50);
                dmobj->KeyUp(48 + index);
                Delay(50);
            }
        }

        //检查退出条件
        dmobj->SetDict(0, FONTSAKURA_DIR"font-exitcopy.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本退场时间:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("副本退场时间::查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        //检查血条
        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("打怪:血条:检查:死亡:复活");
            return code;
        }
    }

    //按30下空格
    for (index = 0; index < 30; ++index)
    {
        dmobj->KeyDown(32);
        Delay(50);
        dmobj->KeyUp(32);
        Delay(50);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    return SNAIL_ERRNO_SUCCESS;
}

static int sakuracopy(Idmsoft* dmobj, void* ctxt)
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
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    
    //按n对话
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-npc1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[活动]堕落樱花净化副本", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("[活动]堕落樱花净化副本:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("[活动]堕落樱花净化副本:查找:失败");
        Delay(500);
    }

    //点击
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
                
            api_log_MsgDebug("[活动]堕落樱花净化副本:点击:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("[活动]堕落樱花净化副本:点击:失败");
        Delay(500);
    }

    //点击确认
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "[活动]堕落樱花净化", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
                
            api_log_MsgDebug("副本:[活动]堕落樱花净化:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        //检查是否有副本满
        dmobj->SetDict(0, FONTSAKURA_DIR"font-reenter.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "因此无法入场。", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
                
            api_log_MsgDebug("副本:因此无法入场。:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);

            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict(0, FONTSAKURA_DIR"font-reenter.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;
                        
                    api_log_MsgDebug("副本:因此无法入场。:确认:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                api_log_MsgDebug("副本:因此无法入场。:确认:查找:失败");
                Delay(500);
            }
            
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX, lposY);
                LUAMAN_WAIT_MS(300);
                dmobj->LeftClick();
                LUAMAN_WAIT_MS(300);
                dmobj->MoveTo(5, 5);
                LUAMAN_WAIT_MS(300);

                dmobj->SetDict(0, FONTSAKURA_DIR"font-reenter.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "因此无法入场。", "ffffff-101010", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("副本:因此无法入场。:点击:成功");
                    break;
                }

                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                    
                api_log_MsgDebug("副本:因此无法入场。:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                Delay(500);
            }

            //结束副本
            return ERRNO_NOT_SUPPORT;
        }
        
        api_log_MsgDebug("副本:[活动]堕落樱花净化:进入:失败");
        Delay(500);
    }

    //等待1分30秒
    if (waitcopytime > 0)
    {
        api_log_MsgDebug("副本:[活动]堕落樱花净化:进入:等待,waitcopytime:%d", waitcopytime);
        Delay_With_Terminate_Check(waitcopytime, luaman_task, dmobj);
    }
    
    pathofposxys = "260:60";
    pathOfPosXYs = CString(pathofposxys);
    code = sakura_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[260:60]:死亡:复活");
        goto finish;
    }

    api_log_MsgDebug("寻路:前往:坐标:[260:60]:成功");

    dmobj->KeyPressChar("1");
    Delay(500);
    dmobj->KeyUpChar("1");
    Delay(300);

    Delay_With_Terminate_Check(2, luaman_task, dmobj);

    dmobj->KeyPressChar("2");
    Delay(500);
    dmobj->KeyUpChar("2");
    Delay(300);

    //按n对话
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[副本开始]副本开始操控杆启动", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("[副本开始]副本开始操控杆启动:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("[副本开始]副本开始操控杆启动:查找:血条:检查,死亡并复活");
            return code;
        }
        
        api_log_MsgDebug("[副本开始]副本开始操控杆启动:查找:失败");
        Delay(500);
    }

    //点击“[副本开始]副本开始操控杆启动”
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[副本开始]副本开始操控杆启动", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("[副本开始]副本开始操控杆启动:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("[副本开始]副本开始操控杆启动:点击:血条:检查,死亡并复活");
            return code;
        }
        
        api_log_MsgDebug("[副本开始]副本开始操控杆启动:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    //寻路
    pathofposxys = "256:63|256:74|256:91|256:107|256:125|256:140|256:154|256:166|261:172";
    pathOfPosXYs = CString(pathofposxys);
    code = sakura_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[261:172]:死亡:复活");
        goto finish;
    }

    api_log_MsgDebug("寻路:前往:坐标:[261:172]:成功");

    //按n对话
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-npc3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "启动魔力屏壁操控杆", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("启动魔力屏壁操控杆:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("启动魔力屏壁操控杆:查找:血条:检查,死亡并复活");
            return code;
        }
        
        api_log_MsgDebug("启动魔力屏壁操控杆:查找:失败");
        Delay(500);
    }

    //点击“启动魔力屏壁操控杆
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-npc3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "启动魔力屏壁操控杆", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("启动魔力屏壁操控杆:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("启动魔力屏壁操控杆:点击:血条:检查,死亡并复活");
            return code;
        }
        
        api_log_MsgDebug("启动魔力屏壁操控杆:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    //寻路
    pathofposxys = "256:177|256:189|256:204|256:220|256:231|256:252|256:271|256:293|260:301";
    pathOfPosXYs = CString(pathofposxys);
    code = sakura_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[260:301]:死亡:复活");
        goto finish;
    }

    api_log_MsgDebug("寻路:前往:坐标:[260:301]:成功");

    //按n对话
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "启动魔力屏壁操控杆", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("启动魔力屏壁操控杆:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("启动魔力屏壁操控杆:查找:血条:检查,死亡并复活");
            return code;
        }
        
        api_log_MsgDebug("启动魔力屏壁操控杆:查找:失败");
        Delay(500);
    }

    //点击“启动魔力屏壁操控杆
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "启动魔力屏壁操控杆", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("启动魔力屏壁操控杆:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("启动魔力屏壁操控杆:点击:血条:检查,死亡并复活");
            return code;
        }
        
        api_log_MsgDebug("启动魔力屏壁操控杆:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    //寻路
    pathofposxys = "256:305|256:322|256:340|256:355|256:373|256:391|256:411|256:429|256:442|260:450";
    pathOfPosXYs = CString(pathofposxys);
    code = sakura_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[260:450]:死亡:复活");
        goto finish;
    }

    api_log_MsgDebug("寻路:前往:坐标:[260:450]:成功");

    //按n对话
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-npc5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "启动魔力屏壁操控杆", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("启动魔力屏壁操控杆:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("启动魔力屏壁操控杆:查找:血条:检查,死亡并复活");
            return code;
        }
        
        api_log_MsgDebug("启动魔力屏壁操控杆:查找:失败");
        Delay(500);
    }

    //点击“启动魔力屏壁操控杆
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-npc5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "启动魔力屏壁操控杆", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("启动魔力屏壁操控杆:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("启动魔力屏壁操控杆:点击:血条:检查,死亡并复活");
            return code;
        }
        
        api_log_MsgDebug("启动魔力屏壁操控杆:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    dmobj->KeyPressChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    //按shift+e
    dmobj->KeyDown(16);
    Delay(100);
    dmobj->KeyDown(69);
    Delay(500);

    dmobj->KeyUp(69);
    Delay(100);
    dmobj->KeyUp(16);
    Delay(500);

    //寻路
    pathofposxys = "256:453|256:463|256:475";
    pathOfPosXYs = CString(pathofposxys);
    code = sakura_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[256:475]:死亡:复活");
        goto finish;
    }

    api_log_MsgDebug("寻路:前往:坐标:[256:475]:成功");

    //开始打怪
    code = sakura_fight(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code)
    {
        api_log_MsgDebug("打怪:血条:检查:死亡:复活:副本:结束");
        goto finish;
    }

    api_log_MsgDebug("打怪:成功");

    //寻路
    pathofposxys = "256:487|255:494";
    pathOfPosXYs = CString(pathofposxys);
    code = sakura_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[255:494]:死亡:复活");
        goto finish;
    }

    api_log_MsgDebug("寻路:前往:坐标:[255:494]:成功");

    //按n对话
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-npc6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "从此处离开。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("从此处离开。:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("从此处离开。:查找:血条:检查,死亡并复活");
            return code;
        }
        
        api_log_MsgDebug("从此处离开。:查找:失败");
        Delay(500);
    }

    //点击“启动魔力屏壁操控杆
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-npc6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "从此处离开。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("从此处离开。:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("从此处离开。:点击:血条:检查,死亡并复活");
            return code;
        }
        
        api_log_MsgDebug("从此处离开。:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //等待跳到平静河谷
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
            
        dmobj->SetDict(0, FONTSAKURA_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "平静河谷", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:平静河谷:退出:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = sakura_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("地图:平静河谷:退出::血条:检查,死亡并复活");
            return code;
        }
        
        api_log_MsgDebug("地图:平静河谷:退出:失败");
        Delay(500);
    }

    //寻路
    pathofposxys = "300:118";
    pathOfPosXYs = CString(pathofposxys);
    code = sakura_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[300:118]:死亡:复活");
        goto finish;
    }

    api_log_MsgDebug("寻路:前往:坐标:[300:118]:成功");
    
finish:
    //结束副本,继续下一轮
    return SNAIL_ERRNO_SUCCESS;
}

static int sakurawork(struct cluamanaccountinfo* accountinfo, Idmsoft* dmobj, void* ctxt)
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
    int i = 0;
    struct passwd2{char ch;char* path;} bmps[]=
    {
        {'0', BMPSAKURA_DIR"0.bmp"},
        {'1', BMPSAKURA_DIR"1.bmp"},
        {'2', BMPSAKURA_DIR"2.bmp"},
        {'3', BMPSAKURA_DIR"3.bmp"},
        {'4', BMPSAKURA_DIR"4.bmp"},
        {'5', BMPSAKURA_DIR"5.bmp"},
        {'6', BMPSAKURA_DIR"6.bmp"},
        {'7', BMPSAKURA_DIR"7.bmp"},
        {'8', BMPSAKURA_DIR"8.bmp"},
        {'9', BMPSAKURA_DIR"9.bmp"},
    };
    int j = 0;
    char* path = NULL;
    int sakura_num = 0;
    
    api_log_MsgDebug("user       : %s", accountinfo->user_name);
    api_log_MsgDebug("passwd     : %s", accountinfo->passwd);
    api_log_MsgDebug("passwdex   : %s", accountinfo->passwdex);

label_login:
    dmobj->MoveTo(5, 5);
    Delay(200);
        
    //查找“帐号”
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSAKURA_DIR"font-login.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "帐号", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
                
            api_log_MsgDebug("帐号:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("帐号:查找:失败");
        Delay(500);
    }

    //帐号 490:648 -> 600:655
    //dmobj->MoveTo(603, 647);
    dmobj->MoveTo(lposX2 + 110, lposY2 + 7);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    code = dmobj->SendString(hwnd, accountinfo->user_name);
    if (1 != code)
    {
        api_log_MsgError("帐号:%s:输入:失败", accountinfo->user_name);
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("帐号:%s:输入:成功", accountinfo->user_name);

    //密码 490:648 -> 600:680
    //dmobj->MoveTo(603, 672);
    dmobj->MoveTo(lposX2 + 110, lposY2 + 32);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    code = dmobj->SendString(hwnd, accountinfo->passwd);
    if (1 != code)
    {
        api_log_MsgError("密码:%s:输入:失败", accountinfo->passwd);
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("密码:%s:输入:成功", accountinfo->passwd);

    //进入游戏 490:648 -> 530:720
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //dmobj->MoveTo(509, 730);
        dmobj->MoveTo(lposX2 + 50, lposY2 + 72);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSAKURA_DIR"font-login.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "帐号", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (0 != code)
        {
            api_log_MsgDebug("账户:%s:登录:成功", accountinfo->user_name);
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
            
        api_log_MsgDebug("账户:%s:登录:失败, lposX:%ld, pposY:%ld", accountinfo->user_name, lposX, pposY);
        Delay(1000);
    }

    //检查二级密码
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSAKURA_DIR"font-security.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "请谨慎输入二级密码", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            lposX0 = lposX;
            lposY0 = lposY;
            
            api_log_MsgDebug("二级:密码:查找:成功, lposX:%ld, pposY:%ld", lposX, lposY);
            break;
        }

        //检查是否出现错误
        dmobj->SetDict(0, FONTSAKURA_DIR"font-loginerr.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "发生错误，请重新登录。", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("发生错误，请重新登录。:查找:成功, lposX:%ld, pposY:%ld", lposX, lposY);

            //点击“发生错误，请重新登录。” 523:242 -> 640:533
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 117, lposY + 291);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONTSAKURA_DIR"font-loginerr.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "发生错误，请重新登录。", "ffffff-101010", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("发生错误，请重新登录。:点击:成功");
                    goto label_login;
                }

                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;

                api_log_MsgDebug("发生错误，请重新登录。:点击:失败, lposX2:%ld, pposY2:%ld", lposX2, lposY2);
            }
        }

        api_log_MsgDebug("二级:密码:查找:失败");
        Delay(1000);
    }

    for (i = 0; i < strlen(accountinfo->passwdex); i++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        path = NULL;
        for (j = 0; j < sizeof(bmps)/sizeof(bmps[0]); j++)
        {
            if (bmps[j].ch == accountinfo->passwdex[i])
            {
                path = bmps[j].path;
                break;
            }
        }

        if (NULL == path)
        {
            api_log_MsgError("二次密码:非数字:不支持");
            return ERRNO_NOT_SUPPORT;
        }
        
        api_log_MsgDebug("输入:二次:密码:%s", path);

        dmobj->MoveTo(5, 5);
        Delay(200);
        
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, FONTSAKURA_DIR"font-security.txt");
            code = dmobj->FindPic(496, 310, 781, 417, path, "101010", 0.9, 0, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("二次:密码:%s:查找:成功, lposX:%ld, lposY:%ld", path, lposX, lposY);
                break;
            }

            api_log_MsgDebug("二次:密码:%s:查找:失败", path);
            Delay(1000);
        }

        //点击二次密码
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        api_log_MsgDebug("点击:二次:密码:%s:成功", path);
    }

    //点击确认,请谨慎输入二级密码: 586:266 -> 686:430
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX0 + 100, lposY0 + 164);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSAKURA_DIR"font-security.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "请谨慎输入二级密码", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("二级:密码:确认:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
                
        api_log_MsgDebug("二级:密码:确认:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(2000);
    }
    
    api_log_MsgDebug("进入:游戏:成功");

    //点击“进入游戏” 640:670
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(640, 670);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        //检查是否进入“平静河谷”
        Delay(1000);
        
        for (i = 0; i < 5; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, FONTSAKURA_DIR"font-map.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "平静河谷", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("地图:平静河谷:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                goto next;
            }

            api_log_MsgDebug("地图:平静河谷:查找:失败");
            Delay(1000);
        }
    }

next:
    //执行“樱花副本”
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = sakuracopy(dmobj, ctxt);
        if (ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("副本:樱花:结束:继续:下一个:账户,sakura_num:%d, code:%d",
                sakura_num, code);
            break;
        }

        sakura_num++;
        api_log_MsgDebug("副本:樱花:继续,sakura_num:%d", sakura_num);
    }

    api_log_MsgDebug("副本:樱花:完成,sakura_num:%d", sakura_num);

    //退出副本
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //点击 1262:747
        dmobj->MoveTo(1262, 747);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-logout.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "重新登录", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("重新登录:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("重新登录:查找:失败");
        Delay(500);
    }

    //点击"重新登录"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSAKURA_DIR"font-saveconf.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "游戏信息保存", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("游戏信息保存:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("游戏信息保存:查找:失败");
        Delay(500);
    }

    //等待“游戏信息保存”消失
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSAKURA_DIR"font-saveconf.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "游戏信息保存", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("游戏信息保存:完成");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
            
        api_log_MsgDebug("游戏信息保存:进行, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    return ERRNO_SUCCESS;
}

static int TaskSakuraMainLoop(struct list_head* accountList, Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int status = 0;
    int bindWnd = luaman_config->curBindWnd;
    struct cluamanaccountinfo* apos = NULL, *an = NULL, *anode = NULL;

    list_for_each_entry_safe(apos, an, accountList, node, struct cluamanaccountinfo)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        status = sakurawork(apos, dmobj, ctxt);
        if (ERRNO_SUCCESS != status)
        {
            api_log_MsgError("副本:樱花:%s:失败, status:%d", apos->user_name, status);
        }
    }
    
    return ERRNO_SUCCESS;
}

int api_TaskSakuraMain(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int bindWnd = luaman_config->curBindWnd;
    struct cluamanbindinfo* bpos = NULL, *bn = NULL, *bnode = NULL;
    
    api_log_MsgDebug("副本:Sakura:主循环");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    list_for_each_entry_safe(bpos, bn, &luaman_config->bindinfoList, node, struct cluamanbindinfo)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        if (bindWnd != bpos->bindWndnr)
        {
            continue;
        }

        return TaskSakuraMainLoop(&bpos->accountList, dmobj, ctxt);
    }

    return ERRNO_SUCCESS;
}

int api_TaskSakuraExit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int bindWnd = luaman_config->curBindWnd;
    
    api_log_MsgDebug("副本:Sakura:退出");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
        
    return ERRNO_SUCCESS;
}


