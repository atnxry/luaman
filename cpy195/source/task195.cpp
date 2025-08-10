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
        api_log_MsgDebug("副本外:血条:查找:成功, groupid:%d, hwnd:%ld",
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

        api_log_MsgDebug("副本外:血条:解析:成功,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
            hwnd, bBloodAva, bBloodTot);
    }

    if (bBloodAva > blueThreshold)
    {
        ////////////////////////////////////////////////////////////////////
        //bug:从进入副本处移到这里 2022/05/15 21:13:11
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
        //bug:从进入副本处移到这里 2022/05/15 21:13:11
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

    //英雄死掉了,检查"接受"字样
    for (;;)
    {
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, "font\\font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("副本外:复活:接受:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        /////////////////////////////////////////////////////////////////////////////
        //bug:新增该提示查找 2022/03/21 21:49:22
        dmobj->SetDict(0, "font\\font-chcklifex.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "要在原地复活吗", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            dmobj->KeyDownChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);

            api_log_MsgDebug("要在原地复活吗:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("副本外:复活:接受:查找:失败");
        LUAMAN_WAIT_MS(500);

        ////////////////////////////////////////////////////////////////////////////
        //bug:2022/03/27 当被别人拉起来之后,需要重新检查血条直接终止复活流程
        realived_check_num++;
        if (realived_check_num > 10)
        {
            realived = 1;
            goto Label_check_blood_again;
        }
    }

    //点击"接受"复活英雄
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
            api_log_MsgDebug("副本外:复活:接受:点击:失败");
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
            api_log_MsgDebug("副本外:复活:接受:点击:成功");
            break;
        }
    }

    //按m键弹出小地图,为防止误判,先将鼠标移到左上角
    dmobj->MoveTo(5, 5);
    Delay(200);

    Check_Terminate_Thread(luaman_task, dmobj);

    ////////////////////////////////////////////////////////////////////
    //bug:从进入副本处移到这里 2022/05/15 21:13:11
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

        //查看m键是否按成功
        dmobj->SetDict(0, "font\\font-checkblue-out.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "玛利亚(神的祭祀)", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("副本外:玛利亚(神的祭祀):查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本外:玛利亚(神的祭祀):查找:失败:死亡:复活");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("副本外:玛利亚(神的祭祀):查找:失败");
    }

    //点击"玛利亚(神的祭祀)"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);

        //查找'移动'字样
        dmobj->SetDict(0, "font\\font-fly-mcheck2.txt");
        code = dmobj->FindStrFast(210, 45, 1138, 745, "移动", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("副本外:移动:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本外:移动:查找:失败:死亡:复活");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("副本外:移动:查找:失败");
        
    }


    dmobj->MoveTo(lposX2, lposY2);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    //检查当前坐标,如果坐标不是245:126则表明对话失败,第一关还没有打过
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
                    api_log_MsgDebug("副本外:移动:245:126:成功");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("副本外:移动:245:126:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    pathofposxys = "245:133";
    pathOfPosXYs = CString(pathofposxys);

    //TODO:先用副本里面的寻路,出问题再改
    (void)Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("副本外:移动:245:133:成功");
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
    //bug:副本外面死按副本外复活流程 2022/01/08 22:49:33
    dmobj->SetDict(0, "font\\font-mapstyle.txt");
    strMapName = dmobj->Ocr(1140, 29, 1273, 58, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strMapName == CString("香格里拉"))
    {
        api_log_MsgDebug("副本外:血条:检查");
        return Tz195_CheckBlueOut(dmobj, ctxt);
    }
    /////////////////////////////////////////////////////////////////////////////////////
    
    dmobj->SetDict(0, "font\\font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("血条:查找:成功, groupid:%d, hwnd:%ld",
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

        api_log_MsgDebug("血条:解析:成功,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
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

    //bug:按键精灵计算时间已经调整为2022了,c语言本地时间是122,需要加1900 2022/01/03 11:58:59
    (void)_snprintf(thistime, 63, "%04d-%02d-%02d %02d:%02d:%02d",
        tm_tm->tm_year + 1900,
        tm_tm->tm_mon,
        tm_tm->tm_mday,
        tm_tm->tm_hour,
        tm_tm->tm_min,
        tm_tm->tm_sec);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //更新死亡时间
    MarkLastDietime(groupid, hwnd, thistime);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //更新同步状态
    code = MarkWaitFlag(groupid, hwnd);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (ERRNO_SUCCESS == code)
    {
        api_log_MsgDebug("连续:死亡:间隔:太短:同步:等待:下一副本");

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
            //bug:副号死了超过10分钟就会被弹出去,为避免弹出去之后被杀死,在此循环过程中检查地图是否在外面,如果在外面就
            //    一直加血,直到主副号同步完成开始下一轮副本 2020/3/9 22:18:11
            if (ROLE_M != role)
            {
                dmobj->SetDict(0, "font\\font-mapstyle.txt");
                strMapName = dmobj->Ocr(1140, 29, 1273, 58, "ffffff-101010", 0.9);
                dmobj->ClearDict(0);
                if (strMapName == CString("香格里拉"))
                {
                    dmobj->KeyDownChar("1");
                    LUAMAN_WAIT_MS(200);
                    dmobj->KeyUpChar("1");
                    LUAMAN_WAIT_MS(200);

                    if (!bKickCopyout)
                    {
                        bKickCopyout = 1;
                        api_log_MsgDebug("死亡:复活:暂停:同步:等待:弹出:副本");
                    }
                }
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("死亡:复活:暂停:同步:等待");
            LUAMAN_WAIT_MS(1000);
        }
    }
    else
    {
        api_log_MsgError("死亡:复活:暂停:同步:等待");
        LUAMAN_WAIT_MS(1000);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //英雄死掉了,检查"接受"字样
    for (;;)
    {
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, "font\\font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("复活:接受:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        /////////////////////////////////////////////////////////////////////////////
        //bug:新增该提示查找 2022/03/26 09:40:22
        dmobj->SetDict(0, "font\\font-chcklifex.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "要在原地复活吗", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            dmobj->KeyDownChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);

            api_log_MsgDebug("要在原地复活吗:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
        }
        
        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("复活:接受:查找:失败");
        LUAMAN_WAIT_MS(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //点击"接受"复活英雄
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
            api_log_MsgDebug("复活:接受:点击:失败");
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
            api_log_MsgDebug("复活:接受:点击:成功");
            break;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
LABEL_Slave_copyout_alive:
    //出来按一下方向键,防止窜动 2020/1/12 20:43:16
    (void)CheckKeyState(dmobj, ctxt);

    Check_Terminate_Thread(luaman_task, dmobj);

    /////////////////////////////////////////////////////////////////////////
    //bug:从进入副本处执行移到当前位置 2022/05/15 21:10:23
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
        //bug: 借组号不退之后,不需要重新借助了 2022/01/02 20:52:33
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

    //bug:注释掉组队 2022/01/08 18:33:15
    return ERRNO_SUCCESS;
    
    goto LABEL_check_heil_fellow;

    //按F键弹出好友对话框
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "添加好友", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            api_log_MsgDebug("添加好友:查找:成功, groupid:%d, hwnd:%ld, lposX:%ld, lposY:%ld",
                luaman_config->groupid, hwnd, lposX, lposY);
            break;
        }

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("添加好友:查找:死亡:复活, groupid:%d, hwnd:%ld",
                luaman_config->groupid, hwnd);
            goto LABEL_invite_heil_fellow;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        LUAMAN_WAIT_MS(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //查找频道
    for (;;)
    {
        dmobj->SetDict(0, "font\\font-inviteheil2team-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "战场服务", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            api_log_MsgDebug("战场服务:查找:成功, groupid:%d, hwnd:%ld, lposX2:%ld, lposY2:%ld",
                luaman_config->groupid, hwnd, lposX2, lposY2);
            break;
        }

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("战场服务:查找:死亡:复活, groupid:%d, hwnd:%ld",
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
                code = dmobj->FindStrFast(0, 27, 1279, 826, "队伍", "7d7d7d-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;
                    api_log_MsgDebug("查找:队伍:成功, groupid:%d, hwnd:%ld, lposX:%ld, lposY:%ld",
                        luaman_config->groupid, hwnd, lposX, lposY);
                    break;
                }

                code = Tz195_CheckBlue(dmobj, ctxt);
                if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("队伍:查找:死亡:复活, groupid:%d, hwnd:%ld",
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
            code = dmobj->FindStrFast(posX1, posY1, posX2, posY2, "同行", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                 syncCount = syncCount + 1;
                 
                 api_log_MsgDebug("组队:邀请:成员:增加, groupid:%d, hwnd:%ld",
                    luaman_config->groupid, hwnd);
            }

            Check_Terminate_Thread(luaman_task, dmobj);
        }

        if (syncCount == syncCountMax)
        {
            api_log_MsgDebug("组队:完成, groupid:%d, hwnd:%ld",
                luaman_config->groupid, hwnd);
            break;
        }

        if (0 == invited)
        {
            api_log_MsgDebug("组队:检查:未组, groupid:%d, hwnd:%ld",
                luaman_config->groupid, hwnd);
            goto LABEL_invite_heil_fellow;
        }

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("组队:等待:死亡:复活, groupid:%d, hwnd:%ld",
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
        if (strMapName == CString("遗弃的葵恩神殿"))
        {
            api_log_MsgDebug("地图:识别:成功:遗弃的葵恩神殿");
            return MAP_FORGET_TOWER;
        }

        if (strMapName == CString("香格里拉"))
        {
            api_log_MsgDebug("地图:识别:成功:香格里拉");
            return MAP_SEGRILA;
        }

        if (strMapName == CString("平静河谷"))
        {
            api_log_MsgDebug("地图:识别:成功:平静河谷");
            return MAP_SEGRILA;
        }

        api_log_MsgDebug("地图:识别:失败");
        
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgWarn("地图:识别:死亡:复活, code:%d", code);
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
            count = SplitStringWide(strCopylefttime, "分", copylefttime);
            if (count > 2)
            {
                copylefttimem = atol(copylefttime.GetAt(0));
                if (copylefttimem < 5)
                {
                    ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                    api_log_MsgDebug("副本:剩余时间:不足:等待:副本:超时:%s", ptr);
                    strCopylefttime.ReleaseBuffer();
                    
                    goto LABEL_Check_left_time_terminate;
                }

                ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                api_log_MsgDebug("副本:剩余时间:充足:%s", ptr);
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
            //bug:如果找到等待时间也按剩余不足逻辑处理 2020/2/18 20:55:16
            dmobj->SetDict(0, "font\\font-waittime.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "等待时间:", "fdb602-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("等待时间:读取:成功,lposX:%ld, lposY:%ld", lposX, lposY);
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
            api_log_MsgDebug("地图:识别:死亡:复活, code:%d", code);
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
            api_log_MsgDebug("副本:超时:等待:退出副本");
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
        api_log_MsgError("路径:坐标:非法:寻路:失败,tgtNum:%d, tgtPosXYs:%s", tgtNum, ptr);
        
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
            
            api_log_MsgError("路径:坐标:参数:非法:寻路:失败,index:%d, tgtPosXYs:%s, xyc:%d",
                index, ptr, xyc);
        
            tgtPosXYs.ReleaseBuffer();
            return ERRNO_INVALID;
        }

        tposX = atol(tgtPosXYe.GetAt(0));
        tposY = atol(tgtPosXYe.GetAt(1));

        //api_log_MsgDebug("坐标:解析:成功,index:%d, tposX:%ld, tposY:%ld", index, tposX, tposY);

        walkstate.yNegKeyDown = 0;
        walkstate.yPosKeyDown = 0;
        walkstate.xPosKeyDown = 0;
        walkstate.xNegKeyDown = 0;
    
        //成功跨过一个点时清零
        rdcltErrorCounter = 0;

        //前往目标坐标点
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

                api_log_MsgWarn("地图:坐标:读取:失败");
                goto LABEL_MovePosition_read_posxy;
            }

            //解析坐标
            xyc = SplitString(strPosXY, ':', posXY);
            if (xyc < 2)
            {
                ptr = strPosXY.GetBuffer(BUFFER_MAX);
                
                api_log_MsgWarn("地图:坐标:解析:失败,strPosXY:%s, xyc:%d",
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
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            //检查下血条
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

                //寻路过程中死亡
                code = Tz195_CheckBlue(dmobj, ctxt);
                if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
                {
                    api_log_MsgWarn("寻路:死亡:复活,tposX:%ld, tposY:%ld, posX:%ld, posY:%ld",
                        tposX, tposY, posX, posY);
                    
                    return code;
                }

                dmobj->SetDict(0, "font\\font-copylefttime.txt");
                strCopylefttime = dmobj->Ocr(15, 172, 183, 194, "ffffff-101010", 0.9);
                dmobj->ClearDict(0);
                if (strCopylefttime != CString(""))
                {
                    //解析副本剩余时间分钟数即可
                    xyc = SplitStringWide(strCopylefttime, "分", copylefttime);
                    if (xyc > 1)
                    {
                        copylefttimem = atol(copylefttime.GetAt(0));
                        if (copylefttimem < 5)
                        {
                            ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                            api_log_MsgWarn("寻路:卡住:副本:剩余时间:%s", ptr);
                            strCopylefttime.ReleaseBuffer();
                            
                            goto LABEL_MovePosition_blocked_posxy;
                        }
                    }
                    else
                    {
                        rdcltErrorCounter = rdcltErrorCounter + 1;
                        if (rdcltErrorCounter > 20)
                        {
                            api_log_MsgWarn("寻路:卡住:副本:剩余时间:识别:错误:20次:%s", ptr);
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
                            api_log_MsgWarn("副本内:寻路:卡住:20次");

                            code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
                            if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
                            {
                                api_log_MsgDebug("副本内:寻路:卡住:超时");
                                goto LABEL_MovePosition_blocked_posxy;
                            }

                            if (ERRNO_REALIVE == code)
                            {
                                api_log_MsgDebug("副本内:寻路:卡住:死亡:复活");
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
            //血条检查完毕
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
            api_log_MsgDebug("副本剩余时间:截止:目标坐标,tposX:%ld, tposY:%ld",
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

        //检查血条
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:卡住:等待:传出:死亡:复活");
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
        count = SplitStringWide(strCopylefttime, "分", copylefttime);
        if (count >= 2)
        {
            copylefttimem = atol(copylefttime.GetAt(0));
            if (copylefttimem < leftTimeMin)
            {
                api_log_MsgDebug("攻击:副本剩余时间:不足,copylefttimem:%ld, location:%d",
                    copylefttimem, location);
                return 0;
            }

            api_log_MsgDebug("攻击:副本剩余时间:充足,copylefttimem:%ld, location:%d",
                copylefttimem, location);
        }
        else
        {
            api_log_MsgDebug("攻击:副本剩余时间:检查:异常,location:%d", location);
        }
    }
    else
    {
        api_log_MsgDebug("攻击:副本剩余时间:检查:失败,location:%d", location);

        dmobj->SetDict(0, "font\\font-waittime.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "等待时间:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("攻击:等待时间:检查:成功,location:%d, lposX:%ld, lposY:%ld",
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开始", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("战斗:辅助:打开:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("战斗:辅助:打开:失败");
        LUAMAN_WAIT_MS(500);

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("战斗:辅助:打开:死亡:复活");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开始", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("战斗:辅助:开始:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
            
        api_log_MsgDebug("战斗:辅助:开始:失败,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        LUAMAN_WAIT_MS(500);

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("战斗:辅助:开始:死亡:复活");
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
                api_log_MsgDebug("剩余时间不足:攻击:结束,location:%d", location);
                return ERRNO_SUCCESS;
            }

            //bug:赋值攻击时间 2020/7/8 23:22:19
            fightMaxTime = code;

            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("攻击:位置,location:%d, fightMaxTime:%d", location, fightMaxTime);
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
        strFontCtxt = "通往神殿接见室的路开启了";
        strFontColorOffset = "00ff00-101010";
        break;

    case 2:
        strFontFile = "font\\font-aa-20-1.txt";
        strFontCtxt = "通往葵恩广场的路开启了";
        strFontColorOffset = "00ff00-101010";
        break;

    case 3:
        strFontFile = "font\\font-aa-32-1.txt";
        strFontCtxt = "通往破碎的回廊与污染的庭院的路开启了";
        strFontColorOffset = "00ff00-101010";
        break;
        
    case 5:
        strFontFile = "font\\font-aa-40-1.txt";
        strFontCtxt = "污染的庭院的结界石生成了";
        strFontColorOffset = "00ff00-101010";
        break;
        
    case 7:
        strFontFile = "font\\font-aa-46-1.txt";
        strFontCtxt = "通往礼堂的路开启了";
        strFontColorOffset = "00ff00-101010";
        break;
        
    case 18:
        strFontFile = "font\\font-aa-61-1.txt";
        strFontCtxt = "西勒诺斯";
        strFontColorOffset = "ff3300-101010";
        break;
        
    case 19:
        strFontFile = "font\\font-aaa-9-1.txt";
        strFontCtxt = "隐藏的西勒诺斯宝物箱子";
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
            api_log_MsgDebug("定点:打怪:进度:已通关,location:%d, pGamePosition:%d",
                location, pGamePosition);
            return ERRNO_SUCCESS;
        }

        api_log_MsgDebug("定点:打怪:进度:未通关,location:%d, pGamePosition:%d",
            location, pGamePosition);
    }
    else
    {
        api_log_MsgDebug("定点:打怪:进度:获取:失败,location:%d", location);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug: 自动打怪之前按9 2020/1/16 21:57:19 bug:取消11,13,15处的等待 2021/11/28 09:09:33
    if (1 != location && 11 != location && 13 != location && 15 != location)
    {
        dmobj->KeyDownChar("9");
        LUAMAN_WAIT_MS(250);
        dmobj->KeyUpChar("9");
        LUAMAN_WAIT_MS(250);
        LUAMAN_WAIT_MS(1750);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:主号在这等5秒 2020/2/18 20:25:16
        if (ROLE_M == role && 2 == location)
        {
            LUAMAN_WAIT_MS(3000);
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    code = Tz195_FightAutoStart(dmobj, ctxt);
    if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
    {
        api_log_MsgDebug("定点:打怪:开始:死亡:复活,location:%d, fposXY:%s", location, fposXY);
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
            //需求:看到2分钟没有,到了就离开 2020/2/19 09:15:17
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

                        api_log_MsgDebug("打怪:限时:超时,fightTime:%f, fightMaxTime:%f, location:%d, fposXY:%s",
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

                    api_log_MsgDebug("定点:U怪:成功,fposXY:%s, lposX:%ld, lposY:%ld",
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

                    api_log_MsgDebug("定点:U怪:成功,fposXY:%s, lposX:%ld, lposY:%ld",
                        fposXY, lposX, lposY);
                    break;
                }
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            //没有U到,检查是否已经打完了
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

                    api_log_MsgDebug("定点:打怪:通关:成功,fposXY:%s, lposX:%ld, lposY:%ld, location:%d",
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

            //获取组进度
            code = GetGroupPosition(groupid, &pGroupPosition);
            if (ERRNO_SUCCESS == code)
            {
                api_log_MsgDebug("定点:打怪:组进度:获取:成功,location:%d, pGroupPosition:%d",
                    location, pGroupPosition);

                if (pGroupPosition >= location)
                {
                    (void)Tz195_FightAutoStop(dmobj, ctxt);
                    return ERRNO_SUCCESS;
                }
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            //////////////////////////////////////////////////////////////////////////////////////
            //需求:看到2分钟没有,到了就离开 2020/1/5 18:48:16
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

                    api_log_MsgDebug("打怪:限时:2分钟:超时,location:%d, fposXY:%s", location, fposXY);
                    return ERRNO_SUCCESS;
                }
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            uFailedCount = uFailedCount + 1;
            if (uFailedCount > ufailedMax)
            {
                api_log_MsgDebug("定点:打怪:可能通关,location:%d, fposXY:%s", location, fposXY);

                (void)Tz195_FightAutoStop(dmobj, ctxt);

                Check_Terminate_Thread(luaman_task, dmobj);
                return ERRNO_SUCCESS;
            }

            Delay(250);
            api_log_MsgDebug("定点:U怪:失败,location:%d, fposXY:%s", location, fposXY);
        }

        //检查血条
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("定点:打怪:死亡:复活,location:%d, fposXY:%s", location, fposXY);

            Check_Terminate_Thread(luaman_task, dmobj);
            (void)Tz195_FightAutoStop(dmobj, ctxt);
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("定点:打怪:成功,location:%d, fposXY:%s", location, fposXY);
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
        strFontCtxt = "通往神殿接见室的路开启了";
        strFontColorOffset = "00ff00-101010";
        break;

    case 2:
        strFontFile = "font\\font-aa-20-1.txt";
        strFontCtxt = "通往葵恩广场的路开启了";
        strFontColorOffset = "00ff00-101010";
        break;

    case 3:
        strFontFile = "font\\font-aa-32-1.txt";
        strFontCtxt = "通往破碎的回廊与污染的庭院的路开启了";
        strFontColorOffset = "00ff00-101010";
        break;
        
    case 5:
        strFontFile = "font\\font-aa-40-1.txt";
        strFontCtxt = "污染的庭院的结界石生成了";
        strFontColorOffset = "00ff00-101010";
        break;
        
    case 7:
        strFontFile = "font\\font-aa-46-1.txt";
        strFontCtxt = "通往礼堂的路开启了";
        strFontColorOffset = "00ff00-101010";
        break;
        
    case 18:
        strFontFile = "font\\font-aa-61-1.txt";
        strFontCtxt = "西勒诺斯";
        strFontColorOffset = "ff3300-101010";
        break;
        
    case 19:
        strFontFile = "font\\font-aaa-9-1.txt";
        strFontCtxt = "隐藏的西勒诺斯宝物箱子";
        strFontColorOffset = "00ff00-101010";
        break;
    
    default:
        break;
    }

    code = GetGamePosition(groupid, hwnd, &pGamePosition);
    if (ERRNO_SUCCESS == code && pGamePosition >= location)
    {
        api_log_MsgDebug("定点:打怪:进度:已通关,location:%d, pGamePosition:%d",
            location, pGamePosition);
        return ERRNO_SUCCESS;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //bug:定点打怪之前按e 2019/12/22 21:23:19
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

                    api_log_MsgDebug("定点:U怪:成功,fposXY:%s, lposX:%ld, lposY:%ld",
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
                    //bug: 边u边按3 2020/1/19 21:04:11
                    if (18 == location)
                    {
                        if (!strcmp(fixPosXY, "") || !strcmp(fixPosXY, "244:152") || !strcmp(fixPosXY, "253:143"))
                        {
                            dmobj->KeyDownChar("3");
                            Delay(500);
                            dmobj->KeyUpChar("3");
                            Delay(500);

                            //bug:新增按7 2022/01/08 15:55:49
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

                    api_log_MsgDebug("定点:U怪:成功,fposXY:%s, lposX:%ld, lposY:%ld",
                        fposXY, lposX, lposY);
                    break;
                }

                /////////////////////////////////////////////////////////////////////////////////////////
                //bug:'隐藏的西勒诺斯宝物箱子'新增白色的检查 2020/9/26 10:14:15
                if (!strcmp(strFontCtxt, "隐藏的西勒诺斯宝物箱子"))
                {
                    dmobj->SetDict(0, strFontFile);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, strFontCtxt, "ffffff-101010", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;
                        
                        uok = 1;

                        api_log_MsgDebug("定点:U怪:成功,fposXY:%s, lposX:%ld, lposY:%ld",
                            fposXY, lposX, lposY);
                        break;
                    }
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                
                /////////////////////////////////////////////////////////////////////////////////////////
                //bug:只要大怪u不到了就退出 2020/1/18 15:43:36
                if (1 == uok)
                {
                    if (18 == location)
                    {
                        api_log_MsgDebug("定点:打怪:通关:成功,location:%d,fposXY:%s", location, fposXY);
                        
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
                //bug: 找到提示就不再u了,直接退出 2020/1/17 21:58:15  
                //bug: 只在左下角区域找 2020/2/29 22:50:16
                if (18 == location)
                {
                    dmobj->SetDict(0, "font\\font-xlnsok.txt");
                    code = dmobj->FindStrFast(2, 462, 385, 616, "成功消灭西勒诺斯", "00ff00-101010", 0.9, &pposX, &pposY);
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
                    
                    api_log_MsgDebug("定点:打怪:组进度:更新:成功,location:%d", location);
                    return ERRNO_SUCCESS;
                }
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            //获取组进度
            code = GetGroupPosition(groupid, &pGroupPosition);
            if (ERRNO_SUCCESS == code && pGroupPosition >= location)
            {
                api_log_MsgDebug("定点:打怪:通关,location:%d, pGroupPosition:%d",
                    location, pGroupPosition);
                return ERRNO_SUCCESS;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            //bug: u不到就先归位 2020/1/17 23:13:22
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
                        
                        api_log_MsgDebug("定点:打怪:归位:卡住:死亡,location:%d, code:%d",
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
                //bug:location=1处如果没有真正打完,走到153:412处继续 2019/12/10 20:32:19
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
                            
                            api_log_MsgDebug("定点:打怪:归位:卡住:死亡,location:%d, code:%d",
                                location, code);
                            return code;
                        }
                    }

                    api_log_MsgDebug("定点:打怪:移位:继续,location:%d", location);
                    goto LABEL_Tz_FightatFixPoint_start;
                }
                else if (18 == location)
                {
                    //////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //bug:连续u多个点 2020/1/16 08:21:44
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
                                
                                api_log_MsgDebug("定点:打怪:归位:卡住:死亡,location:%d, code:%d, fixPosXY:%s",
                                    location, code, fixPosXY);
                                return code;
                            }
                        }

                        api_log_MsgDebug("定点:打怪:移位:继续,location:%d, fixPosXY:%s",
                            location, fixPosXY);
                        goto LABEL_Tz_FightatFixPoint_start;
                    }
                }

                //长时间u不到有可能还没有打完,走到下一步验证之后再更新进度
                api_log_MsgDebug("定点:打怪:可能通关,location:%d", location);
                Check_Terminate_Thread(luaman_task, dmobj);
                
                return ERRNO_SUCCESS;
            }

            Delay(500);
            api_log_MsgDebug("定点:U怪:失败,location:%d", location);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //u到了就开始打怪物,打3个循环u一次
        /////////////////////////////////////////////////////////////////////////////////////
        //bug:主号18步与副号17步同步,主号同步之后才能继续打 2020/6/21 23:03:15
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
                api_log_MsgDebug("西勒诺斯:同步:主号:完成");
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
                //bug:西雷诺斯打怪按1~8 2022/01/02 21:55:13
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
            //bug:检查血条是否达到暂停状态 2020/6/21 22:58
            if (ROLE_M == role && 18 == location && 0 == pauseflag)
            {
                code = dmobj->FindPic(968, 194, 1110, 210, "bmp\\fightpause.bmp", "101010", 0.9, 0, &pposX, &pposY);
                if (-1 == code)
                {
                    //lposX = pposX.lVal;
                    //lposY = pposY.lVal;
                    
                    pauseflag = 1;

                    api_log_MsgDebug("主号:最后的怪:暂停:攻击");
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //归位检查
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
                    
                    api_log_MsgDebug("定点:打怪:归位:卡住:死亡,location:%d, code:%d, fposXY:%s",
                        location, code, fposXY);
                    return code;
                }
            }

            api_log_MsgDebug("定点:打怪:移位:继续,location:%d, fposXY:%s",
                location, fposXY);

            //////////////////////////////////////////////////////////////////////////////////
            //bug: 每归位几次就按下9 2020/1/17 23:20:35
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

        //检查血条
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("定点:打怪:死亡:复活,fposXY:%s,location:%d", fposXY, location);
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("退出队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("确认:查找:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:点击:失败,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
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
    
    //进去先组队 管他妈的,组!避免异常情况重新进入不组队 2020/5/26 22:40:16
    //////////////////////////////////////////////////////////////////////////////
    //bug:官方升级版本后,必须3个人才能进 2020/5/28 21:37:19
    Tz195_InviteHeilfellow(dmobj, ctxt, 2);
    
LABEL_Tz_MasterMain_enter_copy:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    pathofposxys = "255:133|250:135";
    pathOfPosXYs = CString(pathofposxys);
    Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    
LABEL_Tz_MasterMain_enter_copy_without_walk:
    //按N对话进入副本
    Check_Terminate_Thread(luaman_task, dmobj);

    ////////////////////////////////////////////////////////////////////////////
    //bug:进入副本前先领入场券 2022/06/11 22:31:10
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
            code = dmobj->FindStrFast(390, 27, 1279, 826, "[遗弃的葵恩神殿]入场券发放", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("[遗弃的葵恩神殿]入场券发放:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            code = Tz195_CheckBlueOut(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("[遗弃的葵恩神殿]入场券发放:查找:死亡:复活");
                goto LABEL_Tz_MasterMain_enter_copy_without_walk;
            }

            api_log_MsgDebug("[遗弃的葵恩神殿]入场券发放:查找:失败");
            Delay(500);
        }

        //点击
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
            code = dmobj->FindStrFast(390, 27, 1279, 826, "遗弃的葵恩神殿入场券发放", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                
                api_log_MsgDebug("遗弃的葵恩神殿入场券发放:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }

            dmobj->SetDict(0, FONT195_DIR"font-npc100002.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                
                api_log_MsgDebug("明天再来。:查找:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

                //关掉明日再来
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
                    code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("明天再来。:点击:成功");

                        (void)Tz195_ExitGroup(dmobj, ctxt);
                        break;
                    }

                    lposX = pposX.lVal;
                    lposY = pposY.lVal;
                    
                    api_log_MsgDebug("明天再来。:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
                }
                
                return ERRNO_EXIT;
            }
            
            code = Tz195_CheckBlueOut(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("遗弃的葵恩神殿入场券发放:查找:死亡:复活");
                goto LABEL_Tz_MasterMain_enter_copy_without_walk;
            }

            api_log_MsgDebug("遗弃的葵恩神殿入场券发放:查找:失败");
            Delay(500);
        }

        //点击
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
            code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("明天再来。:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
                return ERRNO_EXIT;
            }
            
            dmobj->SetDict(0, FONT195_DIR"font-npc100001.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "遗弃的葵恩神殿入场券发放", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("遗弃的葵恩神殿入场券发放:点击:成功");
                break;
            }

            code = Tz195_CheckBlueOut(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("遗弃的葵恩神殿入场券发放:点击:死亡:复活");
                goto LABEL_Tz_MasterMain_enter_copy_without_walk;
            }

            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("遗弃的葵恩神殿入场券发放:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "遗弃的葵恩神殿", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("遗弃的葵恩神殿:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:查找:死亡:复活");
            //goto LABEL_Tz_MasterMain_enter_copy;
            goto LABEL_Tz_MasterMain_enter_copy_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;

            api_log_MsgDebug("遗弃的葵恩神殿:查找:死亡:复活:反复死亡:进入下一副本");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("遗弃的葵恩神殿:查找:失败");
        LUAMAN_WAIT_MS(500);
    }

    //点击"遗弃的葵恩神殿"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, "font\\font-enter-ack.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("遗弃的葵恩神殿:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:点击:死亡:复活");
            //goto LABEL_Tz_MasterMain_enter_copy;
            goto LABEL_Tz_MasterMain_enter_copy_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;

            api_log_MsgDebug("遗弃的葵恩神殿:点击:死亡:复活:反复死亡:进入下一副本");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("遗弃的葵恩神殿:点击:失败");
        LUAMAN_WAIT_MS(500);
    }

    //点击"确认"进入副本
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
            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:进入副本");
            break;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:死亡:复活");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;

            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:死亡:复活");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //检查副本中是否有大boss,如果是则会出现"因此无法入场"
        dmobj->SetDict(0, "font\\font-reenter.txt");
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
                dmobj->SetDict(0, "font\\font-reenter.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX3 = pposX3.lVal;
                    lposY3 = pposY3.lVal;

                    api_log_MsgDebug("副本中有大boss进不去:确认:查找:成功, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
                    break;
                }

                //检查是否遭干死了
                code = Tz195_CheckBlueOut(dmobj, ctxt);
                if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("副本中有大boss进不去:确认:查找:死亡:复活");
                    //goto LABEL_Tz_MasterMain_enter_copy;
                    goto LABEL_Tz_MasterMain_enter_copy_without_walk;
                }

                if (ERRNO_BLOCK == code)
                {
                    instance = 0;

                    api_log_MsgDebug("副本中有大boss进不去:确认:查找:死亡:复活:反复死亡:进入下一副本");
                    goto LABEL_Tz_MasterMain_enter_copy;
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

                dmobj->SetDict(0, "font\\font-reenter.txt");
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
            goto LABEL_Tz_MasterMain_enter_copy_without_walk;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //没有进入副本,也没有出现"副本中有大boss进不去",则检查是否有"副本地下城满"
        dmobj->SetDict(0, "font\\font-copyfull.txt");
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

                dmobj->SetDict(0, "font\\font-copyfull.txt");
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
            goto LABEL_Tz_MasterMain_enter_copy_without_walk;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:检查"人员不足" 2020/12/24 22:10:55
        dmobj->SetDict(0, "font\\font-peopleleak.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "人员不足", "ffffff-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("副本地下城已满:确认:人员不足:查找:成功,lposX:%ld, lposY:%d",
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
                code = dmobj->FindStrFast(0, 27, 1279, 826, "人员不足", "ffffff-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("副本地下城已满:确认:人员不足:点击:成功:重新组队");

                    Tz195_InviteHeilfellow(dmobj, ctxt, 2);
                    goto LABEL_Tz_MasterMain_enter_copy_without_walk;
                }

                //检查是否遭干死了
                code = Tz195_CheckBlueOut(dmobj, ctxt);
                if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("副本地下城已满:确认:人员不足:点击:查找:死亡:复活");
                    goto LABEL_Tz_MasterMain_enter_copy_invite;
                }

                if (ERRNO_BLOCK == code)
                {
                    instance = 0;

                    api_log_MsgDebug("副本地下城已满:确认:人员不足:点击:查找:死亡:复活:反复死亡:进入下一副本");
                    goto LABEL_Tz_MasterMain_enter_copy_invite;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("副本地下城已满:确认:人员不足:点击:失败");
                LUAMAN_WAIT_MS(500);
            }
        }//"人员不足"

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:副本:努力加载中");
        LUAMAN_WAIT_MS(500);

        //bug:增加血条检查 2022/02/21 21:36
        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本:进入:确认:查找:死亡:复活");
            //goto LABEL_Tz_MasterMain_enter_copy;
            goto LABEL_Tz_MasterMain_enter_copy_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;
            
            api_log_MsgDebug("副本:进入:确认:查找:死亡:复活");
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    //成功进入副本,加状态
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
        //bug:移到复活流程 2022/05/15 21:07:22
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
            api_log_MsgDebug("遗弃的葵恩神殿:超时");
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:超时:判断:点击:死亡:复活");
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

    //前往坐标107:415
    pathofposxys = "107:415";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //等待,当出现"副本进行时间"时开始行走
    for (;;)
    {
        dmobj->SetDict(0, "font\\font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本进行时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("副本进行时间:查找:成功,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("副本进行时间:查找:失败");
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
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

LABEL_Tz_MasterMain_battle_place_1:
    fposXY = "160:411";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    //定点打怪完成,继续前往下一关
    pathofposxys = "154:412|155:428|171:428|189:428|197:421";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    code = MarkReachFlag(groupid, hwnd, 1);
    if (ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("同步:1:失败,code:%d", code);
    }

    //检查是否集结完毕
    for (;;)
    {
        code = CheckSyncFlag(groupid, 1);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("同步:1:完成,code:%d", code);
            break;
        }

        api_log_MsgDebug("同步:1:等待");
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
    }

    goto LABEL_Tz_MasterMain_battle_place_1_x2;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:新增处理 2020/1/21 10:56:55
LABEL_Tz_MasterMain_battle_place_1_x1:
    pathofposxys = "197:421|191:429|176:430|166:429|154:429|152:415|135:415";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    //定点打怪
    fposXY = "135:415";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    //定点打怪
    fposXY = "122:414";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    //定点打怪
    fposXY = "106:415";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
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
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //到达下一关,同步再进行对话,先置同步标志
    code = MarkReachFlag(groupid, hwnd, 1);
    if (ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("同步:1:失败,code:%d", code);
    }

    //检查是否集结完毕
    for (;;)
    {
        code = CheckSyncFlag(groupid, 1);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("同步:1:完成,code:%d", code);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("同步:1:等待");
        Delay(1000);
    }

LABEL_Tz_MasterMain_battle_place_1_x2:
    //集结完毕,开始对话
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至神殿接见室", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至神殿接见室:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        npc1_retry_count = npc1_retry_count + 1;
        if (npc1_retry_count > 5)
        {
            api_log_MsgDebug("移动至神殿接见室:查找:超时");

            //可能是第一关还没有打过,重新倒回去打(清除同步标志)
            code = ClearMarkReachFlag(groupid, hwnd, 1);
            if (ERRNO_SUCCESS != code)
            {
                api_log_MsgDebug("NPC1:对话:失败:清除:同步:地点1:失败,code:%d", code);
            }

            api_log_MsgDebug("NPC1:对话:失败:回退:定点:打怪:地点1");
            goto LABEL_Tz_MasterMain_battle_place_1_x1;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("移动至神殿接见室:查找:失败");
        Delay(500);
    }

    //点击"移动至神殿接见室"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aa-15-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至神殿接见室", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至神殿接见室:点击:成功");
            break;
        }

        //TODO: 检查血条

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("移动至神殿接见室:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //检查当前坐标,如果坐标不是254:415则表明对话失败,第一关还没有打过
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
                    api_log_MsgDebug("移动至神殿接见室:对话:成功");
                    break;
                }
                else if (abs(posX - 197) <= 1 && abs(posY - 421) <= 1)
                {
                    //清除同步标志
                    (void)ClearMarkReachFlag(groupid, hwnd, 1);

                    api_log_MsgDebug("移动至神殿接见室:对话:失败:回退:定点:打怪:地点1");
                    goto LABEL_Tz_MasterMain_battle_place_1_x1;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("移动至神殿接见室:对话:坐标:获取:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //更新自己的进度
    location = 1;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //第一关打过了,进入第二关
    pathofposxys = "272:411|286:406|292:402";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);
    
LABEL_Tz_MasterMain_battle_place_2:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //////////////////////////////////////////////////////////////////////////////////////
    //bug:走到咔咔里面去打 2020/2/22 14:12:22
    dmobj->KeyPressChar("a");
    Delay(50);
    dmobj->KeyPressChar("w");
    Delay(200);
    dmobj->KeyUpChar("a");
    Delay(50);
    dmobj->KeyUpChar("w");
    Delay(200);

    //////////////////////////////////////////////////////////////////////////////////////
    //bug:归位的时候经过288:405再归位,避免被卡死 2019/12/10 20:35:49
    //定点打怪
    fposXY = "288:405|292:402";
    location = 2;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    //寻路"285:407|286:425|301:427|330:427|348:427|357:429"
    pathofposxys = "285:407|286:425|301:427|330:427|348:427|357:429";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //与第二个npc对话
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至葵恩广场", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至葵恩广场:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        npc2_retry_count = npc2_retry_count + 1;
        if (npc2_retry_count > 5)
        {
            api_log_MsgDebug("移动至葵恩广场:查找:超时:NPC2:对话:失败:回退:定点:打怪:地点2");
            goto LABEL_Tz_MasterMain_battle_place_2;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("移动至葵恩广场:查找:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //点击"移动至葵恩广场"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //bug:在不该找的区域找找到了导致一直点,走到非期望区域,导致下一个循环死循环 2022/02/23 22:22:22
        dmobj->SetDict(0, "font\\font-aa-27-1.txt");
        //code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至葵恩广场", "00ff00-101010", 0.9, &pposX2, &pposY2);
        code = dmobj->FindStrFast(380, 27, 1279, 826, "移动至葵恩广场", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至葵恩广场:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("移动至葵恩广场:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    //检查当前坐标,如果坐标不是374:428则表明对话失败,第一关还没有打过
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
                    api_log_MsgDebug("移动至葵恩广场:对话:成功");
                    break;
                }
                else if (abs(posX - 357) <= 1 && abs(posY - 429) <= 1)
                {
                    api_log_MsgDebug("移动至葵恩广场:对话:失败:回退:定点:打怪:地点2");
                    goto LABEL_Tz_MasterMain_battle_place_1_x1;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("移动至葵恩广场:对话:坐标:获取:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //更新自己的进度
    location = 2;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //第二关打过了,进入第三关
    //寻路"380:419|387:411"
    pathofposxys = "380:419|387:411";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

LABEL_Tz_MasterMain_battle_place_3:
    //定点打怪
    fposXY = "387:411";
    location = 3;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    //寻路"380:397|378:392"
    pathofposxys = "380:397|378:392";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    //与第三个npc对话
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至下一区域", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至下一区域:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        npc3_retry_count = npc3_retry_count + 1;
        if (npc3_retry_count > 5)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            ///////////////////////////////////////////////////////////////////////////////////
            //bug:按下9, 不用重新打 2020/1/19 20:58:19
            //goto LABEL_Tz_MasterMain_battle_place_3
            dmobj->KeyPressChar("9");
            Delay(300);
            dmobj->KeyUpChar("9");
            Delay(300);
            Delay(1700);
            npc3_retry_count = 0;

            api_log_MsgDebug("移动至下一区域:查找:超时:NPC3:对话:失败:回退:定点:打怪:地点3");
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("移动至下一区域:查找:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //点击"移动至下一区域"
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至污染的庭院", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("移动至污染的庭院:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        npc3_retry_count = npc3_retry_count + 1;
        if (npc3_retry_count > 5)
        {
            api_log_MsgDebug("移动至污染的庭院:查找:超时:NPC3:对话:失败:回退:定点:打怪:地点3");
            goto LABEL_Tz_MasterMain_battle_place_3;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("移动至污染的庭院:查找:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //点击"移动至葵恩广场"
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);
        
        dmobj->SetDict(0, "font\\font-aa-27-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至污染的庭院", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至污染的庭院:点击:成功");
            break;
        }


        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("移动至污染的庭院:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //检查当前坐标,如果坐标不是386:312则表明对话失败,第一关还没有打过
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
                    api_log_MsgDebug("移动至污染的庭院:对话:成功");
                    break;
                }
                else if (abs(posX - 375) <= 1 && abs(posY - 388) <= 1)
                {
                    api_log_MsgDebug("移动至污染的庭院:对话:失败:回退:定点:打怪:地点2");
                    goto LABEL_Tz_MasterMain_battle_place_3;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("移动至污染的庭院:对话:坐标:获取:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //更新自己的进度
    location = 3;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //第三关打过了,进入第四关
LABEL_Tz_MasterMain_battle_place_4:
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //第四关可能已经打过了,进入第五步
    //寻路"386:312|377:297|369:282"
    pathofposxys = "386:312|377:297|369:282";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

LABEL_Tz_MasterMain_battle_place_5:
    //定点打怪
    fposXY = "369:282";
    location = 5;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //获取进度,以确定是否打完怪了
    location = 5;
    code = GetGroupPosition(groupid, &pGroupPosition);
    if (ERRNO_SUCCESS == code && pGroupPosition >= location)
    {
        pathofposxys = "373:261|381:246|384:226|383:213";
        api_log_MsgDebug("第五步:定点:打怪:组进度:获取:成功:第6步:跳过,pGroupPosition:%d", pGroupPosition);
    }
    else
    {
        api_log_MsgDebug("第6步:定点:打怪:准备,code:%d, pGroupPosition:%d", code, pGroupPosition);

        //第6步
        //寻路到此定点打怪
        pathofposxys = "373:261|381:246";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
        if (ERRNO_SUCCESS != code)
        {
            if (ERRNO_BLOCK == code)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                api_log_MsgDebug("第六步|寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
                return ERRNO_SUCCESS;
            }

            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("第六步|寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
                goto LABEL_Tz_MasterMain_enter_copy;
            }
        }

        api_log_MsgDebug("第六步|寻路:成功,pathofposxys:%s", pathofposxys);

        //定点打怪
        fposXY = "381:246";
        location = 6;
        code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
        if (ERRNO_SUCCESS != code)
        {
            if (ERRNO_BLOCK == code)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                api_log_MsgDebug("第六步|定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
                return ERRNO_SUCCESS;
            }

            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("第六步|定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
                goto LABEL_Tz_MasterMain_enter_copy;
            }
        }

        pathofposxys = "384:226|383:213";
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //第4~6关可能已经打过了,进入第七关
    //寻路到此383:213
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //到达下一关,同步再进行对话,先置同步标志
    (void)MarkReachFlag(groupid, hwnd, 7);

    //检查是否集结完毕
    for (;;)
    {
        code = CheckSyncFlag(groupid, 7);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("同步:7:完成,code:%d", code);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("同步:7:等待");
        Delay(1000);
    }

    //集结完毕,加状态
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
    //bug:副号按1,主号按8 2020/2/18 20:14:11
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

    //按U找到结界石:定点打结界石
LABEL_Tz_MasterMain_battle_place_7:
    //定点打怪
    fposXY = "383:213";
    location = 7;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //更新自己的进度
    location = 7;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    //更新组进度
    (void)UpdateGroupPosition(groupid, location, 0);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:在286:113处按3后继续 2020/1/21 09:44:38
    //寻路 "383:213|368:210|352:206|330:196|318:185|313:170|309:158|301:141||293:127|286:113|263:111|244:112"
    pathofposxys = "383:213|368:210|352:206|330:196|318:185|313:170|309:158|301:141|293:127|286:113";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

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
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入礼堂", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("进入礼堂:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("进入礼堂:查找:失败");

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("进入礼堂:对话:死亡:复活:重新:进入:副本");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;

            api_log_MsgDebug("进入礼堂:对话:死亡:复活:反复死亡:进入下一副本");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入礼堂", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("进入礼堂:点击:成功");
            break;
        }

        api_log_MsgDebug("进入礼堂:点击:失败");

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("进入礼堂:对话:死亡:复活:重新:进入:副本");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;

            api_log_MsgDebug("进入礼堂:对话:死亡:复活:重新:进入:副本:反复死亡");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    //检查当前坐标,如果坐标不是 244:151 则表明对话失败,第一关还没有打过
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
                    api_log_MsgDebug("进入礼堂:对话:成功");
                    break;
                }
                else if (abs(posX - 244) <= 1 && abs(posY - 113) <= 1)
                {
                    api_log_MsgDebug("进入礼堂:对话:失败:回退:定点:打怪:地点2");
                    goto LABEL_Tz_MasterMain_npc_place_8;
                }
            }
        }

        api_log_MsgDebug("进入礼堂:对话:坐标:获取:失败");

        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("进入礼堂:对话:死亡:复活:重新:进入:副本");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;

            api_log_MsgDebug("进入礼堂:对话:死亡:复活:重新:进入:副本:反复死亡");
            goto LABEL_Tz_MasterMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    //按u找'西勒诺斯'
LABEL_Tz_MasterMain_battle_place_9:
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:移位到255:149坐标再打 2020/1/11 13:09:25
    pathofposxys = "255:149";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //定点打怪
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug: 打BOSS之前按e 2020/1/14 21:45:42
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
            
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
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
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return ERRNO_SUCCESS;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    //开始对话
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动奎恩的秘密书房:对话:查找:成功, lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        npc5_retry_count = npc5_retry_count + 1;
        if (npc5_retry_count > 5)
        {
            api_log_MsgDebug("移动奎恩的秘密书房:对话:查找:超时:NPC5:对话:失败:回退:定点:打怪:地点1");

            Check_Terminate_Thread(luaman_task, dmobj);
            
            if (0 != instance)
            {
                code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
                if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
                {
                    instance = 0;

                    api_log_MsgDebug("移动奎恩的秘密书房:对话:超时");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }
                else if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("移动奎恩的秘密书房:对话:超时:死亡:复活");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("移动奎恩的秘密书房:对话:查找:失败");
        Delay(500);
    }
    
    npc5_retry_count = 0;

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //点击"对话"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aaa-4-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至葵恩的秘密书房", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("移动至葵恩的秘密书房:查找:成功,lposX2:%ld, lposY2:%ld",
                lposX2, lposY2);
            break;
        }

        npc5_retry_count = npc5_retry_count + 1;
        if (npc5_retry_count > 5)
        {
            /////////////////////////////////////////////////////////////////////////////////////////////
            //bug:对话窗口还在,需要按esc关掉 2020/2/20 19:23:53
            dmobj->KeyPressChar("esc");
            Delay(300);
            dmobj->KeyUpChar("esc");
            Delay(300);
            /////////////////////////////////////////////////////////////////////////////////////////////

            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("移动奎恩的秘密书房:查找:超时:NPC5:移动奎恩的秘密书房:查找:失败:回退:定点:打怪:地点1");

            //bug:新增副本剩余时间判断 2020/2/18 21:17:19
            if (0 != instance)
            {
                code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
                if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
                {
                    instance = 0;

                    api_log_MsgDebug("移动奎恩的秘密书房:查找:超时");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }
                else if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("移动奎恩的秘密书房:查找:超时:死亡:复活");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("移动至葵恩的秘密书房:查找:失败");
        Delay(500);
    }

    npc5_retry_count = 0;
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //点击"移动至葵恩的秘密书房"
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aaa-4-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至葵恩的秘密书房", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至葵恩的秘密书房:点击:成功");
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

                    api_log_MsgDebug("移动奎恩的秘密书房:点击:超时");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }
                else if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("移动奎恩的秘密书房:点击:超时:死亡:复活");
                    goto LABEL_Tz_MasterMain_enter_copy;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        Delay(500);
        api_log_MsgDebug("移动至葵恩的秘密书房:点击:失败");
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //检查当前坐标,如果坐标不是234:220则表明对话失败,第一关还没有打过
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
                    api_log_MsgDebug("移动至葵恩的秘密书房:对话:成功");
                    break;
                }
                else if (abs(posX - 245) <= 1 && abs(posY - 171) <= 1)
                {
                    api_log_MsgDebug("移动至葵恩的秘密书房:对话:失败");
                    goto LABEL_Tz_MasterMain_battle_place_9;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("移动至神殿接见室:对话:坐标:获取:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //更新自己的进度
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("对话:查找:成功, lposX:%ld, lposY:%d", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("对话:查找:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //点击"对话"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-aaa-7-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认奖励", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("确认奖励:查找:成功, lposX2:%ld, lposY2:%d", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("确认奖励:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认奖励", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认奖励:点击:成功");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("确认奖励:点击:失败");
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
            
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_MasterMain_enter_copy;
        }
    }
    */
    /////////////////////////////////////////////////////////////////////////////////////////
    //bug:2022/05/26 23:07:11
    for (;;)
    {
        dmobj->SetDict(0, "font\\195\\font-npc1000.txt");
        //code = dmobj->FindStrFast(0, 27, 1279, 826, "隐藏的西勒诺斯宝物箱子", "ffff00-101010", 0.9, &pposX, &pposY);
        code = dmobj->FindStrFast(0, 27, 1279, 826, "隐藏", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("隐藏的西勒诺斯宝物箱子:查找:成功, lposX:%ld, lposY:%d", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("隐藏的西勒诺斯宝物箱子:查找:失败");
        Delay(500);
    }

    //点击宝箱 573:429 ->611:467~611:502~670:466~670:499
    //573+38/97, 429+38/70
    lposX = lposX + 38 + time(NULL) % 50;
    lposY = lposY + 38 + time(NULL) % 25;

    api_log_MsgDebug("宝箱:随机:点击:位置:lposX:%ld, lposY:%ld", lposX, lposY);
    
    dmobj->MoveTo(lposX, lposY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);

    Delay_With_Terminate_Check(10, luaman_task, dmobj);
    
    //按20下空格
    for (index = 0; index < 25; ++index)
    {
        dmobj->KeyDown(32);
        Delay(50);
        dmobj->KeyUp(32);
        Delay(50);
    }
    /////////////////////////////////////////////////////////////////////////////////////////

    //更新自己的进度
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
    
    //同步,对话,退出副本
    //到达下一关,同步再进行对话,先置同步标志
    location = 19;
    code = MarkReachFlag(groupid, hwnd, location);
    
    //检查是否集结完毕
    for (;;)
    {
        code = CheckSyncFlag(groupid, location);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("同步:code:%d", code);
            break;
        }

        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("同步:等待");
    }
    
    //对话,出副本
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("对话:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("对话:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确定离开副本", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确定离开副本:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("确定离开副本:查找:失败");
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
        //bug:点完第一次之后,又弹出来一次 2019/12/22 21:37:22
        dmobj->SetDict(0, "font\\font-aaa-12-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确定离开副本", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确定离开副本:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("确定离开副本:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    api_log_MsgDebug("主任务副本主线执行成功");

    //////////////////////////////////////////////////////////////////////////////
    //bug:官方升级版本后,必须3个人才能进 2020/5/28 21:37:19
    (void)Tz195_InviteHeilfellow(dmobj, ctxt, 2);

    //bug:从30秒改为25秒 2021/12/11 22:10:22 2022/01/08 18:34:33
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
    //bug:进入副本前先领入场券 2022/06/11 22:31:10
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
            code = dmobj->FindStrFast(390, 27, 1279, 826, "[遗弃的葵恩神殿]入场券发放", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("[遗弃的葵恩神殿]入场券发放:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            code = Tz195_CheckBlueOut(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("[遗弃的葵恩神殿]入场券发放:查找:死亡:复活");
                goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
            }

            api_log_MsgDebug("[遗弃的葵恩神殿]入场券发放:查找:失败");
            Delay(500);
        }

        //点击
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
            code = dmobj->FindStrFast(390, 27, 1279, 826, "遗弃的葵恩神殿入场券发放", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                
                api_log_MsgDebug("遗弃的葵恩神殿入场券发放:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }

            dmobj->SetDict(0, FONT195_DIR"font-npc100002.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                
                api_log_MsgDebug("明天再来。:查找:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

                //关掉明日再来
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
                    code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("明天再来。:点击:成功");

                        (void)Tz195_ExitGroup(dmobj, ctxt);
                        break;
                    }

                    lposX = pposX.lVal;
                    lposY = pposY.lVal;
                    
                    api_log_MsgDebug("明天再来。:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
                }
                
                return ERRNO_EXIT;
            }
            
            code = Tz195_CheckBlueOut(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("[遗弃的葵恩神殿]入场券发放:查找:死亡:复活");
                goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
            }

            api_log_MsgDebug("[遗弃的葵恩神殿]入场券发放:查找:失败");
            Delay(500);
        }

        //点击
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
            code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("明天再来。:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
                return ERRNO_EXIT;
            }
            
            dmobj->SetDict(0, FONT195_DIR"font-npc100001.txt");
            code = dmobj->FindStrFast(390, 27, 1279, 826, "遗弃的葵恩神殿入场券发放", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("遗弃的葵恩神殿入场券发放:点击:成功");
                break;
            }

            code = Tz195_CheckBlueOut(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("遗弃的葵恩神殿入场券发放:点击:死亡:复活");
                goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
            }

            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("遗弃的葵恩神殿入场券发放:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
            Delay(500);
        }

        ticketin = 0;
    }
    ////////////////////////////////////////////////////////////////////////////

    
    //按N对话进入副本
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);
        
        dmobj->SetDict(0, "font\\font-aa-2-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "遗弃的葵恩神殿", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("遗弃的葵恩神殿:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:查找:死亡:复活");
            //goto LABEL_Tz_SlaveMain_enter_copy;
            goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;
            
            api_log_MsgDebug("遗弃的葵恩神殿:查找:死亡:复活:反复死亡");
            goto LABEL_Tz_SlaveMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("遗弃的葵恩神殿:查找:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //点击"遗弃的葵恩神殿"
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-enter-ack.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("遗弃的葵恩神殿:点击:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:点击:死亡:复活");
            //goto LABEL_Tz_SlaveMain_enter_copy;
            goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;
            
            api_log_MsgDebug("遗弃的葵恩神殿:点击:死亡:复活:反复死亡");
            goto LABEL_Tz_SlaveMain_enter_copy;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("遗弃的葵恩神殿:点击:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //点击"确认"进入副本
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
            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:进入副本");
            break;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:死亡:复活");
            goto LABEL_Tz_SlaveMain_enter_copy;
        }

        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:死亡:复活:反复死亡");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:出现人员不足时先点击确定再对话
        dmobj->SetDict(0, "font\\font-peopleleak.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "人员不足", "ffffff-202020", 0.9, &pposX, &pposY);
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

        //检查副本中是否有大boss,如果是则会出现"因此无法入场"
        dmobj->SetDict(0, "font\\font-reenter.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "因此无法入场。", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:副本中有大boss进不去,lposX:%ld, lposY:%ld", lposX, lposY);

            //查找"副本中有大boss进不去:确认"字样
            for (;;)
            {
                dmobj->SetDict( 0, "font\\font-reenter.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX3 = pposX3.lVal;
                    lposY3 = pposY3.lVal;

                    api_log_MsgDebug("副本中有大boss进不去:确认:查找:成功,lposX3:%ld, lposY3:%ld", lposX3, lposY3);
                    break;
                }

                code = Tz195_CheckBlueOut(dmobj, ctxt);
                if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("副本中有大boss进不去:确认:查找:死亡:复活");
                    //goto LABEL_Tz_SlaveMain_enter_copy;
                    goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
                }

                if (ERRNO_BLOCK == code)
                {
                    instance = 0;
                    
                    api_log_MsgDebug("副本中有大boss进不去:确认:查找:死亡:复活");
                    goto LABEL_Tz_SlaveMain_enter_copy;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("副本中有大boss进不去:确认:查找:失败");
                Delay(500);
            }

            //点击"副本中有大boss进不去:确认"字样
            for (;;)
            {
                dmobj->MoveTo(lposX3, lposY3);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(500);
                
                dmobj->SetDict(0, "font\\font-reenter.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功");
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("副本中有大boss进不去:确认:点击:失败");
                Delay(500);
            }

            api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功:重新对话");
            goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
        }//"因此无法入场"

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //没有进入副本,也没有出现"副本中有大boss进不去",则检查是否有"副本地下城满"
        dmobj->SetDict(0, "font\\font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:副本地下城已满, lposX:%ld, lposY:%ld", lposX, lposY);

            //点击"副本地下城满:确认"
            for (;;)
            {
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(500);
                
                dmobj->SetDict(0, "font\\font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("副本地下城已满:确认:点击:成功");
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("副本地下城已满:确认:点击:失败");
                Delay(500);
            }

            goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
        }//"副本地下城已满"

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:副本:努力加载中");
        Delay(500);

        //bug:增加血条检查 2022/02/21 21:36
        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本:进入:确认:查找:死亡:复活");
            //goto LABEL_Tz_SlaveMain_enter_copy;
            goto LABEL_Tz_SlaveMain_enter_copy_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            instance = 0;
            
            api_log_MsgDebug("副本:进入:确认:查找:死亡:复活");
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }//点击"确认"进入副本

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:新增副本剩余时间判断 2020/1/15 22:17:35
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
            api_log_MsgDebug("遗弃的葵恩神殿:超时");
            return code;
        }
        else if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:超时:判断:点击:死亡:复活");
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    //成功进入副本,加状态
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
    //需求:新增需求 2020/1/12 20:49:34
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

    //签到
    if (0 == signin)
    {
        signin = 1;
        (void)Tz195_Login(dmobj, ctxt);
    }

    //前往坐标107:415
    pathofposxys = "107:415";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    //等待,当出现"副本进行时间"时开始行走
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, "font\\font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本进行时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("副本进行时间:查找:成功,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        api_log_MsgDebug("副本进行时间:查找:失败");
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
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_battle_place_1:
    ///////////////////////////////////////////////////////////////////////////////
    //bug:160:412改为163:411->160:411 2019/12/10 20:18:31/22:24:19
    //定点打怪
    fposXY = "160:411";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    //定点打怪完成,继续前往下一关
    pathofposxys = "154:412|155:428|171:428|189:428|197:421";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //到达下一关,同步再进行对话,先置同步标志
    (void)MarkReachFlag(groupid, hwnd, 1);

    //检查是否集结完毕
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = CheckSyncFlag(groupid, 1);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("同步:1:完成,code:%d", code);
            break;
        }

        api_log_MsgDebug("同步:1:等待");
        Delay(1000);
    }
    
    goto LABEL_Tz_SlaveMain_battle_place_1_x2;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:新增处理 2020/1/26 22:03:15
LABEL_Tz_SlaveMain_battle_place_1_x1:
    pathofposxys = "197:421|191:429|176:430|166:429|154:429|152:415|135:415";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);
    
    //定点打怪
    fposXY = "135:415";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }
    
    //定点打怪
    fposXY = "122:414";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }
    
    //定点打怪
    fposXY = "106:415";
    location = 1;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    //寻路"106:415|122:414|135:415|152:415|154:429|166:429|176:430|191:429|197:421"
    pathofposxys = "106:415|122:414|135:415|152:415|154:429|166:429|176:430|191:429|197:421";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);
    
    //到达下一关,同步再进行对话,先置同步标志
    (void)MarkReachFlag(groupid, hwnd, 1);
    
    //检查是否集结完毕
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = CheckSyncFlag(groupid, 1);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("同步:1:完成");
            break;
        }

        api_log_MsgDebug("同步:1:等待");
        Delay(1000);
    }

LABEL_Tz_SlaveMain_battle_place_1_x2:
    //集结完毕,开始对话
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至神殿接见室", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至神殿接见室:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        npc1_retry_count = npc1_retry_count + 1;
        if (npc1_retry_count > 5)
        {
            (void)ClearMarkReachFlag(groupid, hwnd, 1);

            api_log_MsgDebug("NPC1:对话:失败:回退:定点:打怪:地点1");
            goto LABEL_Tz_SlaveMain_battle_place_1_x1;
        }

        api_log_MsgDebug("移动至神殿接见室:查找:失败");
        Delay(500);
    }
    
    //点击"移动至神殿接见室"
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至神殿接见室", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至神殿接见室:点击:成功");
            break;
        }

        api_log_MsgDebug("移动至神殿接见室:点击:失败");
        Delay(500);
    }

    //检查当前坐标,如果坐标不是254:415则表明对话失败,第一关还没有打过
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
                    api_log_MsgDebug("移动至神殿接见室:对话:成功");
                    break;
                }
                else if (abs(posX - 197) <= 1 && abs(posY - 421) <= 1)
                {
                    (void)ClearMarkReachFlag(groupid, hwnd, 1);
                    
                    api_log_MsgDebug("移动至神殿接见室:对话:失败");
                    goto LABEL_Tz_SlaveMain_battle_place_1_x1;
                }
            }
        }

        api_log_MsgDebug("移动至神殿接见室:对话:坐标:获取:失败");
        Delay(500);
    }

    //更新自己的进度
    location = 1;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //第一关打过了,进入第二关
    pathofposxys = "272:411|286:406|292:402";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_battle_place_2:
    //bug:走到咔咔里面去打 2020/2/22 14:12:22
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
    //bug:归位的时候经过288:405再归位,避免被卡死 2019/12/10 20:35:49
    //定点打怪
    fposXY = "288:405|292:402";
    location = 2;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
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
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    //与第二个npc对话
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至葵恩广场", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至葵恩广场:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        npc2_retry_count = npc2_retry_count + 1;
        if (npc2_retry_count > 5)
        {
            api_log_MsgDebug("移动至葵恩广场:查找:超时:NPC2:对话:失败:回退:定点:打怪:地点2");
            goto LABEL_Tz_SlaveMain_battle_place_2;
        }

        api_log_MsgDebug("移动至葵恩广场:查找:失败");
        Delay(500);
    }

    //点击"移动至葵恩广场"
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
        //code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至葵恩广场", "00ff00-101010", 0.9, &pposX2, &pposY2);
        code = dmobj->FindStrFast(380, 27, 1279, 826, "移动至葵恩广场", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至葵恩广场:点击:成功");
            break;
        }

        api_log_MsgDebug("移动至葵恩广场:点击:失败");
        Delay(500);
    }

    //检查当前坐标,如果坐标不是374:428则表明对话失败,第一关还没有打过
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
                    api_log_MsgDebug("移动至葵恩广场:对话:成功");
                    break;
                }
                else if (abs(posX - 357) <= 1 && abs(posY - 429) <= 1)
                {
                    api_log_MsgDebug("移动至葵恩广场:对话:失败:回退:定点:打怪:地点2");
                    goto LABEL_Tz_SlaveMain_battle_place_2;
                }
            }
        }

        api_log_MsgDebug("移动至葵恩广场:对话:坐标:获取:失败");
        Delay(500);
    }

    //更新自己的进度
    location = 2;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //第二关打过了,进入第三关
    pathofposxys = "380:419|387:411";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);
    
LABEL_Tz_SlaveMain_battle_place_3:
    //定点打怪
    fposXY = "387:411";
    location = 3;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    //寻路"380:397|375:388"
    pathofposxys = "380:397|378:392";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    //与第三个npc对话
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至下一区域", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至下一区域:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
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

            api_log_MsgDebug("移动至下一区域:查找:超时:NPC3:对话:失败:回退:定点:打怪:地点3");
        }

        api_log_MsgDebug("移动至下一区域:查找:失败");
        Delay(500);
    }

    //点击"移动至下一区域"
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至污染的庭院", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("移动至污染的庭院:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        npc3_retry_count = npc3_retry_count + 1;
        if (npc3_retry_count > 5)
        {
            api_log_MsgDebug("移动至污染的庭院:查找:超时:NPC3:对话:失败:回退:定点:打怪:地点3");
            goto LABEL_Tz_SlaveMain_battle_place_3;
        }

        api_log_MsgDebug("移动至污染的庭院:查找:失败");
        Delay(500);
    }

    //点击"移动至污染的庭院"
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至污染的庭院", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至污染的庭院:点击:成功");
            break;
        }

        api_log_MsgDebug("移动至污染的庭院:点击:失败");
        Delay(500);
    }

    //检查当前坐标,如果坐标不是386:312则表明对话失败,第一关还没有打过
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
                    api_log_MsgDebug("移动至污染的庭院:对话:成功");
                    break;
                }
                else if (abs(posX - 375) <= 1 && abs(posY - 388) <= 1)
                {
                    api_log_MsgDebug("移动至污染的庭院:对话:失败");
                    goto LABEL_Tz_SlaveMain_battle_place_3;
                }
            }
        }

        api_log_MsgDebug("移动至污染的庭院:对话:坐标:获取:失败");
        Delay(500);
    }
    
    //更新自己的进度
    location = 3;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //第三关打过了,进入第四关
LABEL_Tz_SlaveMain_battle_place_4:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //第四关可能已经打过了,进入第五步
    //寻路"386:312|377:297|369:282"
    pathofposxys = "386:312|377:297|369:282";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_battle_place_5:
    //定点打怪
    fposXY = "369:282";
    location = 5;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    //获取进度,以确定是否打完怪了
    location = 5;
    code = GetGroupPosition(groupid, &pGroupPosition);
    if (ERRNO_SUCCESS == code && pGroupPosition >= location)
    {
        pathofposxys = "373:261|381:246|384:226|383:213";
        api_log_MsgDebug("第五步:定点:打怪:组进度:获取:成功:第6步:跳过,pGroupPosition:%d", pGroupPosition);
    }
    else
    {
        api_log_MsgDebug("第6步:定点:打怪:准备,code:%d, pGroupPosition:%d", code, pGroupPosition);

        //第6步
        //寻路到此定点打怪
        pathofposxys = "373:261|381:246";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
        if (ERRNO_SUCCESS != code)
        {
            if (ERRNO_BLOCK == code)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
                return code;
            }

            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
                goto LABEL_Tz_SlaveMain_enter_copy;
            }
        }

        api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

        //定点打怪
        fposXY = "381:246";
        location = 6;
        code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
        if (ERRNO_SUCCESS != code)
        {
            if (ERRNO_BLOCK == code)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
                return code;
            }

            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
                goto LABEL_Tz_SlaveMain_enter_copy;
            }
        }

        pathofposxys = "384:226|383:213";
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //第4~6关可能已经打过了,进入第七关
    //寻路到此383:213
    pathOfPosXYs = CString(pathofposxys);
    code = Tz195_MovePosition(dmobj, ctxt, pathOfPosXYs);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    //到达下一关,同步再进行对话,先置同步标志
    (void)MarkReachFlag(groupid, hwnd, 7);
    
    //检查是否集结完毕
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = CheckSyncFlag(groupid, 7);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("同步:7:完成,code:%d", code);
            break;
        }

        api_log_MsgDebug("同步:7:等待");
        Delay(1000);
    }

    //集结完毕,加状态
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
    
    //按U找到结界石:定点打结界石
LABEL_Tz_SlaveMain_battle_place_7:
    //定点打怪
    fposXY = "383:213";
    location = 7;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
            goto LABEL_Tz_SlaveMain_enter_copy;
        }
    }
    
    //更新自己的进度
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
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_continue_copy;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    //自杀,复活出去
    for (;;)
    {
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            second_entry = 1;
            api_log_MsgDebug("自杀:成功:复活");
            break;
        }

        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("自杀:等待:死亡时间太短:进入下一轮");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("自杀:等待");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "遗弃的葵恩神殿", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("遗弃的葵恩神殿:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:查找:死亡:复活");
            //goto LABEL_Tz_SlaveMain_enter_copy_continue;
            goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:查找:死亡:复活:反复死亡");
            return code;
        }
        
        api_log_MsgDebug("遗弃的葵恩神殿:查找:失败");
        Delay(500);
    }

    //点击"遗弃的葵恩神殿"
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("遗弃的葵恩神殿:点击:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:点击:死亡:复活");
            //goto LABEL_Tz_SlaveMain_enter_copy_continue;
            goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:点击:死亡:复活:反复死亡");
            return code;
        }
        
        api_log_MsgDebug("遗弃的葵恩神殿:点击:失败");
        Delay(500);
    }

    //点击"确认"进入副本
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
            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:进入副本");
            break;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:死亡:复活");
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }

        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:死亡:复活:反复死亡");
            return code;
        }

        //检查副本中是否有大boss,如果是则会出现"因此无法入场"
        dmobj->SetDict(0, "font\\font-reenter.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "因此无法入场。", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:副本中有大boss进不去,lposX:%ld, lposY:%ld", lposX, lposY);

            //查找"副本中有大boss进不去:确认"字样
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict( 0, "font\\font-reenter.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX3 = pposX3.lVal;
                    lposY3 = pposY3.lVal;

                    api_log_MsgDebug("副本中有大boss进不去:确认:查找:成功,lposX3:%ld, lposY3:%ld", lposX3, lposY3);
                    break;
                }

                code = Tz195_CheckBlueOut(dmobj, ctxt);
                if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("副本中有大boss进不去:确认:查找:死亡:复活");
                    //goto LABEL_Tz_SlaveMain_enter_copy_continue;
                    goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
                }

                if (ERRNO_BLOCK == code)
                {
                    api_log_MsgDebug("副本中有大boss进不去:确认:查找:死亡:复活");
                    return code;
                }

                api_log_MsgDebug("副本中有大boss进不去:确认:查找:失败");
                Delay(500);
            }

            //点击"副本中有大boss进不去:确认"字样
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
                code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功");
                    break;
                }

                api_log_MsgDebug("副本中有大boss进不去:确认:点击:失败");
                Delay(500);
            }

            api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功:重新对话");
            goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
        }//"因此无法入场"

        //没有进入副本,也没有出现"副本中有大boss进不去",则检查是否有"副本地下城满"
        dmobj->SetDict(0, "font\\font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:副本地下城已满, lposX:%ld, lposY:%ld", lposX, lposY);

            //点击"副本地下城满:确认"
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
                code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("副本地下城已满:确认:点击:成功");
                    break;
                }

                api_log_MsgDebug("副本地下城已满:确认:点击:失败");
                Delay(500);
            }

            goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
        }//"副本地下城已满"

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:出现人员不足时先点击确定再对话
        dmobj->SetDict(0, "font\\font-peopleleak.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "人员不足", "ffffff-202020", 0.9, &pposX, &pposY);
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
                code = dmobj->FindStrFast(0, 27, 1279, 826, "人员不足", "ffffff-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("副本地下城已满:确认:人员不足:点击:成功:重新组队");
                    goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
                }

                code = Tz195_CheckBlueOut(dmobj, ctxt);
                if (ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("副本地下城已满:确认:人员不足:点击:查找:死亡:复活");
                    goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
                }

                if (ERRNO_BLOCK == code)
                {
                    api_log_MsgDebug("副本地下城已满:确认:人员不足:点击:查找:死亡:复活:反复死亡:进入下一副本");
                    goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
                }

                api_log_MsgDebug("副本地下城已满:确认:人员不足:点击:失败");
                Delay(500);
            }
        }

        api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:副本:努力加载中");
        Delay(500);

        //bug:增加血条检查 2022/02/21 21:36
        code = Tz195_CheckBlueOut(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本:进入:确认:查找:死亡:复活");
            goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
        }

        if (ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("副本:进入:确认:查找:死亡:复活");
            goto LABEL_Tz_SlaveMain_enter_copy_continue_without_walk;
        }
    }//点击"确认"进入副本

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:新增副本剩余时间判断 2020/1/15 22:17:35
    if (0 == instance)
    {
        instance = 1;
    }
    else
    {
        code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
        if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:超时");
            return code;
        }
        else if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:超时:判断:点击:死亡:复活");
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    //成功进入副本,加状态
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
    //需求:新增需求 2020/1/12 20:49:34
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
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至神殿接见室", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至神殿接见室:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("移动至神殿接见室:查找:失败");
        Delay(500);
    }

    //点击"移动至神殿接见室"
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至神殿接见室", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至神殿接见室:点击:成功");
            break;
        }

        api_log_MsgDebug("移动至神殿接见室:点击:失败");
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
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至葵恩广场", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至葵恩广场:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("移动至葵恩广场:查找:失败");
        Delay(500);
    }

    //点击"移动至葵恩广场"
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至葵恩广场", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至葵恩广场:点击:成功");
            break;
        }

        api_log_MsgDebug("移动至葵恩广场:点击:失败");
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
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至下一区域", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至下一区域:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("移动至下一区域:查找:失败");
        Delay(500);
    }

    //点击"移动至下一区域"
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至破碎的回廊", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("移动至破碎的回廊:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("移动至破碎的回廊:查找:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至破碎的回廊", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至破碎的回廊:点击:成功");
            break;
        }

        api_log_MsgDebug("移动至破碎的回廊:点击:失败");
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
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入破碎的回廊", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("进入破碎的回廊:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("进入破碎的回廊:查找:失败");
        Delay(500);
    }

    //点击"进入破碎的回廊"
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入破碎的回廊", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("进入破碎的回廊:点击:成功");
            break;
        }

        api_log_MsgDebug("进入破碎的回廊:点击:失败");
        Delay(500);
    }

LABEL_Tz_SlaveMain_Continue_walk_place_8:
    /////////////////////////////////////////////////////////////////////////////////////////
    //bug:按1后再走
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
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place8 = 1;
            
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);
    
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
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place10 = 1;
            
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_Continue_battle_place_11:
    if (0 != instance)
    {
        code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
        if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("210:278:副本剩余时间:不足:进入下一轮");
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("210:278:副本剩余时间:检测:死亡:复活");
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    forward2place11 = 0;

    //定点打怪
    fposXY = "210:278";
    location = 11;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place11 = 1;
            
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
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
            
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place12 = 1;
            
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_Continue_battle_place_13:
    if (0 != instance)
    {
        code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
        if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("185:290:副本剩余时间:不足:进入下一轮");
            return code;
        }
        else if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("185:290:副本剩余时间:检测:死亡:复活");
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    forward2place13 = 0;

    //定点打怪
    fposXY = "185:290";
    location = 13;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place13 = 1;
            
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
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
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place14 = 1;
            
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_Continue_battle_place_15:
    if (0 != instance)
    {
        code = Tz195_CheckLefttime2Terminate(dmobj, ctxt);
        if (ERRNO_TIMEOUT == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("240:340:副本剩余时间:不足:进入下一轮");
            return code;
        }
        else if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("240:340:副本剩余时间:检测:死亡:复活");
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    forward2place15 = 0;

    //定点打怪
    fposXY = "240:340";
    location = 15;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place15 = 1;
            
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
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
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            forward2place16 = 1;
            
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

LABEL_Tz_SlaveMain_Continue_npc_place_17:
    //定点打怪
    forward2place17 = 0;
    fposXY = "160:281";
    location = 17;
    code = Tz195_FightatFixPointSimple(dmobj, ctxt, location, fposXY);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_BLOCK == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点打怪:卡死:副本:消失:进入:下一轮,fposXY:%s", fposXY);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点打怪:死亡:复活:重新:进入:副本,fposXY:%s", fposXY);
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
            api_log_MsgDebug("寻路:卡死:副本:消失:进入:下一轮,pathofposxys:%s", pathofposxys);
            return code;
        }

        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:死亡:复活:重新:进入:副本,pathofposxys:%s", pathofposxys);
            goto LABEL_Tz_SlaveMain_enter_copy_continue;
        }
    }

    api_log_MsgDebug("寻路:成功,pathofposxys:%s", pathofposxys);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = FlagCheck(groupid);
        if (0 == code)
        {
            api_log_MsgDebug("西勒诺斯:同步:完成");
            break;
        }

        api_log_MsgDebug("西勒诺斯:同步:等待");
        Delay(1000);
    }

    goto Label_Tz_SlaveMain_Continue_waitout;


Label_Tz_SlaveMain_Continue_waitout:
    //更新自己的进度
    location = 19;
    (void)UpdateGamePosition(groupid, hwnd, location, 0);

    //同步下 移到检测到接受后再同步 2022/04/25 22:52:11
    //location = 19;
    //(void)MarkReachFlag(groupid, hwnd, location);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //bug:找到接受后再同步,避免出去时时间不一致 2021/11/25 23:37:11
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, "font\\font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("副号:退出:同步:接受:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        /////////////////////////////////////////////////////////////////////////////
        //bug:新增该提示查找 2022/03/21 09:44:22
        dmobj->SetDict(0, "font\\font-chcklifex.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "要在原地复活吗", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            dmobj->KeyDownChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);

            api_log_MsgDebug("要在原地复活吗:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
        }
        
        api_log_MsgDebug("副号:退出:同步:接受:查找:失败");
        Delay(500);
    }

    //同步下 移到检测到接受后再同步 2022/04/25 22:52:11
    location = 19;
    (void)MarkReachFlag(groupid, hwnd, location);

    skip_alive_flag = 0;

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = CheckSyncFlag(groupid, location);
        if (ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("同步:完成");
            break;
        }

        if (ROLE_M != role)
        {
            dmobj->SetDict(0, "font\\font-mapstyle.txt");
            strMapName = dmobj->Ocr(1140, 29, 1273, 58, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (strMapName == CString("香格里拉"))
            {
                if (0 == skip_alive_flag)
                {
                    skip_alive_flag = 1;
                }

                api_log_MsgDebug("同步:死亡:自动复活:弹出副本");
            }
        }

        (void)check_alarm(dmobj, ctxt);

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("同步:等待");
        Delay(1000);
    }

    if (1 == skip_alive_flag)
    {
        api_log_MsgDebug("同步:等待:自动复活:跳过:自杀:进入:下一副本");
        goto LABEL_jump_to_next_copy;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //复活 bug:主号已经同步出去又进来了,而副号还在等待死亡.这样就卡住了(未解决缺陷) 2020/03/02 20:36:11
    for (;;)
    {
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code || ERRNO_BLOCK == code)
        {
            api_log_MsgDebug("副号:复活:下一轮");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("副号:复活:下一轮;等待");
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
    //bug:检查有2个"同行"后才开始计时等待30变为25秒 2021/12/11 22:16:35
    //检查'同行'个数 2022/05/26 23:25:11 注释掉
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
            code = dmobj->FindStrFast(posX1, posY1, posX2, posY2, "同行", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                syncCount = syncCount + 1;
                api_log_MsgDebug("副号:同行:查找:成员:加1");
            }
            Check_Terminate_Thread(luaman_task, dmobj);
        }

        if (2 == syncCount)
        {
            api_log_MsgDebug("副号:同行:完成");
            break;
        }
    }*/
    ///////////////////////////////////////////////////////////////////////////////////////

    //bug:25改成27秒 2022/01/08 18:34:33
    Delay_With_Terminate_Check(32, luaman_task, dmobj);

    api_log_MsgDebug("从任务:主线:part2:结束");
    
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
    
    //查找'返回传送门'
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "返回传送门", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("返回传送门:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("返回传送门:查找:失败");
        Delay(500);
    }

    //查找'移动'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //查找
        dmobj->SetDict(0, "font\\font-addmv.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("返回传送门:移动:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("返回传送门:移动:查找:失败");
        Delay(500);
    }

    //点击'移动'
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "添加", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("返回传送门:移动:点击:成功");
            break;
        }

        Delay(500);
        api_log_MsgDebug("返回传送门:移动:点击:失败");
    }
    
    //等着到达目的坐标 127:31
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
                    api_log_MsgDebug("返回传送门:[424:255]:达到");
                    break;
                }
            }
        }

        //检查血条
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("返回传送门:死亡:复活");
            return code;
        }
        
        api_log_MsgDebug("返回传送门:[127:31]:未达到");
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "组队邀请", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("组队邀请:查找:成功, lposX:%d, lposY:%ld", lposX, lposY);
                break;
            }

            Delay(1000);
        }

        //点击'是'接受组队邀请
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "队伍口号", "f4be3d-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                    
                api_log_MsgDebug("组队邀请:点击:成功, lposX2:%d, lposY2:%ld", lposX2, lposY2);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            Delay(1000);
            api_log_MsgDebug("组队邀请:点击:失败");
        }

        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, "font\\font-join2team.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "香格里拉", "ffff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("香格里拉:查找:成功, lposX:%d, lposY:%ld", lposX, lposY);
                break;
            }

            Delay(500);
            api_log_MsgDebug("香格里拉:查找:失败");
        }

        //点击移动 672:353->774:356 788:353->852:355
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "组队成员传送", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                
                api_log_MsgDebug("组队成员传送:查找:成功, lposX2:%d, lposY2:%ld", lposX2, lposY2);
                break;
            }

            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:正在退出队伍的时候可能收到邀请 2020/2/23 14:06:55
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict(0, "font\\font-join2team.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "组队邀请", "ffffff-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("组队成员传送:查找:失败:组队邀请:查找:失败");
                    break;
                }

                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;
                    
                //点击'否'取消 组队邀请: 614:409 是: 582:535 否: 697:535
                dmobj->MoveTo(lposX3 + 83, lposY3 + 126);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                api_log_MsgDebug("组队成员传送:查找:失败:组队邀请:查找:成功, lposX3:%d, lposY3:%ld", lposX3, lposY3);
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            Delay(1000);
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // bug:将检查地图改为检查坐标 2021/11/25 22:29:15
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
                        api_log_MsgDebug("借组:坐标:移动:255:134:成功");
                        break;
                    }
                }
            }

            //检查血条
            code = Tz195_CheckBlue(dmobj, ctxt);
            if (ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("借组:坐标:移动:255:134:死亡:复活");

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
        //bug:检查是否死亡,死了就要复活 2020/2/20 19:28:11
        code = Tz195_CheckBlue(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("借组:坐标:移动:255:134:死亡:复活");

            Check_Terminate_Thread(luaman_task, dmobj);
            Delay(1000);
            goto LABEL_Tz195_main_auxi_start;
        }

        //回城 bug:注释掉回城 2021/11/25 22:29:15 -->返回等待地点
        code = Tz195_Walk2WaitPoint(dmobj, ctxt);
        if (ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("前往:等待:地点:死亡:复活");

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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("退出队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict(0, "font\\font-join2team.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "组队邀请", "ffffff-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("退出队伍:查找:失败:组队邀请:查找:失败");
                    break;
                }

                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;

                api_log_MsgDebug("退出队伍:查找:失败:组队邀请:查找:成功,lposX3:%ld, lposY3:%ld", lposX3, lposY3);

                Check_Terminate_Thread(luaman_task, dmobj);
                
                //点击'否'取消 组队邀请: 614:409 是: 582:535 否: 697:535
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
        //找黄颜色的"遗弃的葵恩神殿"后才点击退组
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, "font\\font-ww02.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "遗弃的葵恩神殿", "ffff00-101010", 0.9, &pposX9, &pposY9);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX9 = pposX9.lVal;
                lposY9 = pposY9.lVal;
                
                api_log_MsgDebug("退出队伍:查找:遗弃的葵恩神殿:查找:成功, lposX9:%ld, lposY9:%ld", lposX9, lposY9);
                break;
            }

            api_log_MsgDebug("退出队伍:查找:遗弃的葵恩神殿:查找:失败");
            Delay(500);

            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict(0, "font\\font-join2team.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "组队邀请", "ffffff-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;

                //点击'否'取消 组队邀请: 614:409 是: 582:535 否: 697:535
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
                api_log_MsgDebug("退出队伍:查找:遗弃的葵恩神殿:查找:超时");
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
                
                api_log_MsgDebug("确认:查找:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }

            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:正在退出队伍的时候可能收到邀请 2020/2/23 14:06:55
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict(0, "font\\font-join2team.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "组队邀请", "ffffff-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("确认:查找:失败:组队邀请:查找:失败");
                    break;
                }

                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;

                api_log_MsgDebug("确认:查找:失败:组队邀请:查找:成功,lposX3:%ld, lposY3:%ld", lposX3, lposY3);

                Check_Terminate_Thread(luaman_task, dmobj);
                
                //点击'否'取消 组队邀请: 614:409 是: 582:535 否: 697:535
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                break;
            }

            lposX = pposX.lVal;
            lposY = pposY.lVal;

            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:正在退出队伍的时候可能收到邀请 2020/2/23 14:06:55
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->SetDict(0, "font\\font-join2team.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "组队邀请", "ffffff-101010", 0.9, &pposX3, &pposY3);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;
            
                //点击'否'取消 组队邀请: 614:409 是: 582:535 否: 697:535
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
    
    api_log_MsgDebug("副本:195:初始化:开始, hwnd:%ld", hwnd);

    code = GetContext(groupid, hwnd, luaman_config->role);
    if (ERRNO_SUCCESS != code)
    {
        if (ERRNO_EXIST == code)
        {
            api_log_MsgError("task195 is aready running, hwnd:%ld", hwnd);
            AfxMessageBox("已运行195副本");
            return code;
        }

        if (ERRNO_CONFLICT == code)
        {
            api_log_MsgError("role conflict, hwnd:%ld", hwnd);
            AfxMessageBox("角色冲突,跳过195副本");
            return code;
        }

        api_log_MsgError("run task195 failed, hwnd:%ld", hwnd);
        AfxMessageBox("运行195副本失败");
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
    
    /* 绑定窗口 */
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

    api_log_MsgDebug("副本:195:初始化:完成, hwnd:%ld", hwnd);

    /* 初始化告警时间初始值 */
    init_alarm_ctxt();

    return ERRNO_SUCCESS;
}

/* 返回错误码将终止线程 */
int api_Task195Main(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int groupid = luaman_config->groupid;
    int status = 0;
    
    api_log_MsgDebug("副本:195:主循环: hwnd:%ld, role:%d", hwnd, (int)role);

    switch (role)
    {
    case ROLE_M:
        api_log_MsgDebug("副本:195:主号:主循环:开始, hwnd:%ld", hwnd);
        
        Check_Terminate_Thread(luaman_task, dmobj);
        
        FlagReset(groupid);
        status = Tz195_MasterMainLine(dmobj, ctxt);
        if (ERRNO_EXIT == status)
        {
            api_log_MsgDebug("副本:195:主号:主循环:结束, hwnd:%ld", hwnd);
            return status;
        }
        
        Check_Terminate_Thread(luaman_task, dmobj);
        
        ResetGame(groupid, hwnd);
        ResetGroup(groupid);

        api_log_MsgDebug("副本:195:主号:主循环:完成, hwnd:%ld", hwnd);
        break;

    case ROLE_S0:
    case ROLE_S1:
        api_log_MsgDebug("副本:195:副号:主循环:开始, hwnd:%ld", hwnd);
    
        Check_Terminate_Thread(luaman_task, dmobj);

        status = Tz195_SlaveMainLine(dmobj, ctxt);
        if (ERRNO_EXIT == status)
        {
            api_log_MsgDebug("副本:195:副号:主循环:结束, hwnd:%ld", hwnd);
            return status;
        }
        
        Check_Terminate_Thread(luaman_task, dmobj);
        
        ResetGame(groupid, hwnd);

        api_log_MsgDebug("副本:195:副号:主循环:完成, hwnd:%ld", hwnd);
        break;

    case ROLE_AUX:
        api_log_MsgDebug("副本:195:借组号:主循环:开始, hwnd:%ld", hwnd);
        
        Check_Terminate_Thread(luaman_task, dmobj);
        Tz195_MainAuxi(dmobj, ctxt);
        Check_Terminate_Thread(luaman_task, dmobj);

        api_log_MsgDebug("副本:195:借组号:主循环:完成, hwnd:%ld", hwnd);
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

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //检查血条
        /*code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("组队:邀请:添加好友:死亡:复活");
            return code;
        }*/

        api_log_MsgDebug("组队:邀请:添加好友:找字:大范围,失败");
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(200);
    
    //查找'战场服务'字样
    lastChckTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT1853_DIR"font-inviteheil2team-1.txt");
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

        //检查血条
        /*code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("组队:邀请:战场服务:找字:死亡:复活");
            return code;
        }*/

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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            posX = fellowPosXYs[i].x;
            posY = fellowPosXYs[i].y;
            
            api_log_MsgDebug("i:%d, posX:%ld, posY:%ld", i, posX, posY);

            lastChckTime = time(NULL);

            //点击队员名称,弹出队伍对话框
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(posX, posY);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);

                dmobj->SetDict(0, FONT1853_DIR"font-inviteheil2team-1.txt");
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

                //检查血条
                /*code = Tz1853_CheckLife(dmobj, ctxt);
                if (SNAIL_ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("组队:邀请:队员:队伍:找字:死亡:复活");
                    return code;
                }*/

                api_log_MsgDebug("组队:邀请:队员:队伍:找字,失败");
                Delay(500);
            }

            //点击'队伍'邀请队友加入队伍
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
        }

        syncCount = 0;

        //@bug:稍等片刻,避免二次组队浪费时间2019/4/8 23:06:19
        Delay(2000);

        api_log_MsgDebug("同行:个数:检查");
        
        //检查'同行'个数
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            posX1 = syncPosXYs[2 * i].x;
            posY1 = syncPosXYs[2 * i].y;
            posX2 = syncPosXYs[2 * i + 1].x;
            posY2 = syncPosXYs[2 * i + 1].y;

            api_log_MsgDebug("(%ld, %ld) --> (%ld, %ld)", posX1, posY1, posX2, posY2);

            dmobj->SetDict(0, FONT1853_DIR"font-inviteheil2team-1.txt");
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

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("组队:邀请:队员:所有,组队未完成,syncCount:%d", syncCount);
    }

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //检查血条
    /*code = Tz1853_CheckLife(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code)
    {
        api_log_MsgDebug("组队完成:死亡:复活:完成");
        return code;
    }*/

    api_log_MsgDebug("组队完成:成功:完成");
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

    //按m键弹出小地图,为防止误判,先将鼠标移到左上角
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(300);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        dmobj->KeyDownChar("m");
        Delay(200);
        dmobj->KeyUpChar("m");
        Delay(200);

        //查看m键是否按成功
        dmobj->SetDict(0, FONT1853_DIR"font-fly-mcheck1.txt");
        code = dmobj->FindStrFast(210, 45, 1138, 745, "卡伦的火炉", "cefbc9-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("卡伦的火炉:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        /*code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本外面:复活:m按键:被杀死:复活,成功");
            return code;
        }*/

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("副本外面:复活:m按键:失败");
        Delay(500);
    }

    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);

        //查找'移动'字样
        dmobj->SetDict(0, FONT1853_DIR"font-fly-mcheck2.txt");
        code = dmobj->FindStrFast(210, 45, 1138, 745, "移动", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("副本外面:复活:移动:找字:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        /*code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本外面:复活:移动:找字:被杀死:复活,成功");
            return code;
        }*/

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:在移动过程被反复杀死了又复活时可能返回SUCCESS 2021/06/14 11:12:33
        /*if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("副本外面:复活:移动:死亡:复活:成功:移动:副本:成功");
            break;
        }*/

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("副本外面:复活:移动:找字:失败");
        Delay(1000);
    }

    dmobj->MoveTo(lposX2, lposY2);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    //检查当前坐标,如果坐标不是234:220则表明对话失败,第一关还没有打过
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

                //bug:误差太大可能就在目标点附近,实际并没有飞拢 2022/01/08 15:43:16
                //120:126 -> 442:388 2022/05/04 19:52:58
                if (abs(posX - 442) <= 3 && abs(posY - 388) <= 3)
                {
                    api_log_MsgDebug("副本外面:复活:移动到副本门口:成功, posX:%ld, posY:%ld", posX, posY);
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        /*code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本外面:复活:移动:点击后:被杀死:复活,成功");
            return code;
        }*/

        api_log_MsgDebug("副本外面:复活:移动:未点中");
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
    
    //按m键弹出小地图,为防止误判,先将鼠标移到左上角
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(300);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    for (;;)
    {
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);

        //查找'移动'字样
        dmobj->SetDict(0, FONT1853_DIR"font-fly-mcheck2.txt");
        code = dmobj->FindStrFast(210, 45, 1138, 745, "移动", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("195:185:移动:查找:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        /*code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本外面:195:185:移动:查找:复活,成功");
            return code;
        }*/

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("副本外面:195:185:移动:查找:失败");
        Delay(500);
    }

    //点击移到
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

        if (strMapName == CString("米特拉"))
        {
            api_log_MsgDebug("副本外面:195:185:米特拉:到达");
            break;
        }
        
        api_log_MsgDebug("副本外面:195:185:米特拉:未到达");
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
    
    api_log_MsgDebug("临界区:进入:等待");
    Tz195EnterCriticalSection();
    api_log_MsgDebug("临界区:进入:成功");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        code = Tz195_InviteHeilfellowJoin2Team(dmobj, ctxt);
        if (SNAIL_ERRNO_AGAIN != code)
        {
            api_log_MsgDebug("组队:完成");
            break;
        }

        check_alarm(dmobj, ctxt);
    }

    api_log_MsgDebug("临界区:退出:等待");
    Tz195LeaveCriticalSection();
    api_log_MsgDebug("临界区:退出:成功");

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
    //需求:195打完之后飞到185副本门口 2022/06/12 10:00:00
    (void)Fly195to185(dmobj, ctxt);

    ///////////////////////////////////////////////////////////////////////////////////
    
    (void)PutContext(groupid, hwnd);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
    
    api_log_MsgDebug("副本:195:退出, hwnd:%ld, groupid:%d, role:%s",
        hwnd, groupid, roletips[role]);
    return ERRNO_SUCCESS;
}
#endif

