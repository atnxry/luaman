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
        //code = dmobj->FindStrFast(210, 45, 1138, 745, "红洞(沼泽)", "ffffff-101010", 0.9, &pposX, &pposY);
        code = dmobj->FindStrFast(210, 45, 1138, 745, "卡伦的火炉", "cefbc9-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("卡伦的火炉:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本外面:复活:m按键:被杀死:复活,成功");
            return code;
        }

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
        
        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本外面:复活:移动:找字:被杀死:复活,成功");
            return code;
        }

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
        
        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本外面:复活:移动:点击后:被杀死:复活,成功");
            return code;
        }

        api_log_MsgDebug("副本外面:复活:移动:未点中");
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
        api_log_MsgDebug("蓝血条:读取:异常, groupid:%d, hwnd:%ld",
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

        api_log_MsgDebug("血条:解析:成功,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
            hwnd, bBloodAva, bBloodTot);
    }

    Check_Terminate_Thread(luaman_task, dmobj);

    if (bBloodAva > 0)
    {
        /////////////////////////////////////////////////////////////////////////////////
        //@bug:站起来之后乱窜 2019/4/12 23:25:07
        (void)Tz1853_ReleaseKey(dmobj, ctxt);

        if (luaman_config->callbabydie)
        {
            //bug:按t召唤宝宝 2022/06/12 22:31:15
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
                api_log_MsgDebug("死亡:接受:等待:移动过程中错误,code:%d", code);
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

    /* 在CheckLife的时候已经判断,此处可能尚在时间段内导致处理过程被忽略
    end = time(NULL);
    offset = end - (time_t)TlsGetValue(g_lstChkTime);
    if (offset < bluebloodChecktime)
    {
        return SNAIL_ERRNO_INGORE;
    }

    //更新血条检查时间
    TlsSetValue(g_lstChkTime, (LPVOID)end);
    */

    ///////////////////////////////////////////////////////////////////////////////////
    //@bug:副本外复活,先判断点击接受之前是不是在副本外面 2019/4/24 21:14:44
    //默认在副本外面死的,即便是误判也无非是多余的移动一次,不会影响脚本的功能
    bCopyoutPre = 1;

    ///////////////////////////////////////////////////////////////////////////////////
    dmobj->SetDict(0, FONT1853_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("副本外:读取:血条:异常");
        return SNAIL_ERRNO_INGORE;
    }

    cstr = strBBlood.GetBuffer(BUFFER_MAX);

    api_log_MsgDebug("副本外:读取:血条:成功,cstr:%s", (cstr)? cstr : "null");
    
    strBBlood.ReleaseBuffer();

    sz = SplitString(strBBlood, '/', bBlood);
    if (2 == sz)
    {
        bBloodAva = atol(bBlood.GetAt(0));
        bBloodTot = atol(bBlood.GetAt(1));

        api_log_MsgDebug("副本外:血条:解析:成功,bBloodAva:%ld, bBloodTot:%ld",
            bBloodAva, bBloodTot);
    }
    else
    {
        strBBloodLen = strBBlood.GetLength();
        bBloodAva = atol(strBBlood.Mid(1, strBBloodLen - bluebloodWidth));
        bBloodTot = atol(strBBlood.Mid(strBBloodLen - bluebloodWidth + 1, strBBloodLen));

        api_log_MsgDebug("副本外:血条:解析:成功,"
            "bBloodAva:%ld, bBloodTot:%ld, sz:%d, strBBloodLen:%d,"
            "bluebloodWidth:%d",
            bBloodAva, bBloodTot, sz, strBBloodLen,
            bluebloodWidth);
    }

    if (bBloodAva > bluebloodLowLevel)
    {
        api_log_MsgDebug("副本外:血条:充足,bBloodAva:%d, bluebloodLowLevel:%d", bBloodAva, bluebloodLowLevel);
        return ERRNO_SUCCESS;
    }

    Check_Terminate_Thread(luaman_task, dmobj);

    api_log_MsgDebug("副本外:血条:不足,bBloodAva:%d, bluebloodLowLevel:%d", bBloodAva, bluebloodLowLevel);
    
    if (bBloodAva > 0)
    {
        dmobj->KeyDownChar("f3");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f3");
        LUAMAN_WAIT_MS(200);

        api_log_MsgDebug("血条:补充:完成,bBloodAva:%d", bBloodAva);
        return ERRNO_SUCCESS;
    }

    api_log_MsgDebug("英雄:死亡:地点:米特拉:副本外面");
    
    //死亡了则先检查方向键是否按下,并松开对应按下的方向键
    Tz1853_ReleaseKey(dmobj, ctxt);
    
    api_log_MsgDebug("血条:检查:死亡,等待出现接受字样");

LABEL_continue_after_realive_yx0_Outside:
    //等待出现死亡字样
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(200);

        //如果有地洞副本满框框,则先点掉 2019/7/19 20:47:59
        dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                Delay(500);
            }
        }
        
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:有时候出现白色的'副本地下城已满'点不中,导致后续流程出错 2019/8/26 20:46:37
        dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "ffffff-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("副本地下城已满:ffffff:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            
            //点击"副本地下城已满"
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
                code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "ffffff-202020", 0.9, &pposX2, &pposY2);
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("死亡:复活:接受:找字:大范围,成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }
        
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug: 副本里面死了继续 2020/2/13 17:02:16
        dmobj->SetDict(0, FONT1853_DIR"font-chcklife2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "使用复活道具", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("死亡:复活:使用复活道具:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            goto LABEL_continue_after_realive_x1_Outside;
        }

        dmobj->SetDict(0, FONT1853_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "要在原地复活吗", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("死亡:复活:要在原地复活吗:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            goto LABEL_continue_after_realive_x2_Outside;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //检查血条是否恢复,如果恢复则表明被其他玩家技能复活,直接退出复活流程
        code = Tz1853_CheckAlive(dmobj, ctxt, bCopyoutPre);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("死亡:复活:非自主复活,可能来源于其他玩家的施救");
            
            code = SNAIL_ERRNO_REALIVE;
            goto LABEL_CHECK_HIDDEN_OK_Outside;
        }

        api_log_MsgDebug("血条:检查:死亡:接受:找字:大范围,失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (0 != code)
        {
            api_log_MsgDebug("血条:检查:死亡:接受:点击,成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("血条:检查:死亡:接受:点击,失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
    }

    Delay(500);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //等待英雄复活后站起来
    for (;;)
    {
        code = Tz1853_CheckAlive(dmobj, ctxt, bCopyoutPre);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("血条:检查:死亡:自主复活,成功");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("血条:检查:死亡:自主复活:等待");
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

        //确认是否隐身成功
        code = dmobj->CmpColor(376, 771, "4b4b4b-101010", 0.9);
        if (!code)
        {
            api_log_MsgDebug("死亡:复活:隐身:成功");
            break;
        }

        end = time(NULL);
        offset = end - hiddenchk;
        if (offset >= 10)
        {
            api_log_MsgDebug("死亡:复活:隐身:超时,可能又被杀了");
            break;
        }

        api_log_MsgDebug("死亡:复活:隐身:失败");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "使用复活道具", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if(code == -1)
        {
            api_log_MsgDebug("死亡:复活:使用复活道具:点击:成功");
            break;
        }

        api_log_MsgDebug("死亡:复活:使用复活道具:点击:失败");
        Delay(500);
    }

    goto LABEL_continue_after_realive_Outside;

LABEL_continue_after_realive_x2_Outside:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    //查找并点击"取消",找不到就继续往下
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(200);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug: 如果有地洞副本满框框,则点确认;否则就是点确认对应的取消 2020/12/27 14:02:18
        //如果有地洞副本满框框,则先点掉 2019/7/19 20:47:59
        dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("原地复活:副本地下城已满:点击:成功");
                    break;
                }

                api_log_MsgDebug("原地复活:副本地下城已满:点击:失败");
                
                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(500);
            }

            break;
        }
        
        //////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:有时候出现白色的'副本地下城已满'点不中,导致后续流程出错 2019/8/26 20:46:37
        dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "ffffff-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("副本地下城已满:ffffff:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            
            //点击"副本地下城已满"
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
                code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "ffffff-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("原地复活:副本地下城已满:点击:成功");
                    break;
                }

                api_log_MsgDebug("原地复活:副本地下城已满:点击:失败");
                
                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(500);
            }

            break;
        }
        
        //检查是否出现'确认'字样的对话框
        dmobj->SetDict(0, FONT1853_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("原地复活:确认:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //点击'取消' 确认:578:498,取消:678:498
        dmobj->MoveTo(lposX2 + 100, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("原地复活:确认:查找:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
    }

    api_log_MsgDebug("原地复活:取消:成功:复活:继续");
    
    goto LABEL_continue_after_realive_yx0_Outside;
    
LABEL_continue_after_realive_Outside:
    //等待英雄复活后站起来
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz1853_CheckAlive(dmobj, ctxt, bCopyoutPre);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("血条:检查:死亡:自主复活,成功");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("血条:检查:死亡:自主复活:等待");
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
        api_log_MsgDebug("血条:检查:时间:太短:忽略,end:%ld, offset:%d, bluebloodChecktime:%d",
            end, offset, bluebloodChecktime);
        return SNAIL_ERRNO_INGORE;
    }

    api_log_MsgDebug("血条:检查:时间:到期,end:%ld, offset:%d", end, offset);
    
    //更新血条检查时间
    TlsSetValue(g_lstChkTime, (LPVOID)end);

    ///////////////////////////////////////////////////////////////////////////////////
    //@bug:副本外复活,先判断点击接受之前是不是在副本外面 2019/4/24 21:14:44
    //默认在副本外面死的,即便是误判也无非是多余的移动一次,不会影响脚本的功能
    bCopyoutPre = 1;

    dmobj->SetDict(0, FONT1853_DIR"font-maptips.txt");
    code = dmobj->FindStrFast(1140, 29, 1273, 58, "米特拉", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 == code)
    {
        api_log_MsgDebug("血条:检查:地图:米特拉:找字:失败");
        
        dmobj->SetDict(0, FONT1853_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "遗忘的地下神殿", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            bCopyoutPre = 0;

            api_log_MsgDebug("血条:检查:地图:遗忘的地下神殿:找字:成功");
        }
    }
    else
    {
        api_log_MsgDebug("血条:检查:地图:米特拉:找字:成功");
    }

    ///////////////////////////////////////////////////////////////////////////////////
    dmobj->SetDict(0, FONT1853_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("血条:读取:失败, groupid:%d, hwnd:%ld",
            luaman_config->groupid, hwnd);
        return SNAIL_ERRNO_INGORE;
    }

    cstr = strBBlood.GetBuffer(BUFFER_MAX);

    api_log_MsgDebug("血条:读取:成功,cstr:%s", (cstr)? cstr : "null");
    
    strBBlood.ReleaseBuffer();

    sz = SplitString(strBBlood, '/', bBlood);
    if (2 == sz)
    {
        bBloodAva = atol(bBlood.GetAt(0));
        bBloodTot = atol(bBlood.GetAt(1));

        api_log_MsgDebug("血条:解析:成功,bBloodAva:%ld, bBloodTot:%ld",
            bBloodAva, bBloodTot);
    }
    else
    {
        strBBloodLen = strBBlood.GetLength();
        bBloodAva = atol(strBBlood.Mid(1, strBBloodLen - bluebloodWidth));
        bBloodTot = atol(strBBlood.Mid(strBBloodLen - bluebloodWidth + 1, strBBloodLen));

        api_log_MsgDebug("血条:解析:成功,"
            "bBloodAva:%ld, bBloodTot:%ld, sz:%d, strBBloodLen:%d,"
            "bluebloodWidth:%d",
            bBloodAva, bBloodTot, sz, strBBloodLen,
            bluebloodWidth);
    }

    if (bBloodAva > bluebloodLowLevel)
    {
        api_log_MsgDebug("血条:充足,bBloodAva:%d, bluebloodLowLevel:%d", bBloodAva, bluebloodLowLevel);
        return ERRNO_SUCCESS;
    }

    api_log_MsgDebug("血条:不足,bBloodAva:%d, bluebloodLowLevel:%d", bBloodAva, bluebloodLowLevel);
    
    if (bBloodAva > 0)
    {
        dmobj->KeyDownChar("f3");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f3");
        LUAMAN_WAIT_MS(200);

        api_log_MsgDebug("血条:补充:完成,bBloodAva:%d", bBloodAva);
        return ERRNO_SUCCESS;
    }

    api_log_MsgDebug("血条:归零:复活,bBloodAva:%d, bCopyoutPre:%d", bBloodAva, bCopyoutPre);
    
    ///////////////////////////////////////////////////////////////////////////////////
    //bug:副本外面的血条检查需要改变复活策略 2021/03/13 20:20:35 从Tz1852-v1.0.17.1拷贝过来的
    if (1 == bCopyoutPre)
    {
        return Tz1853_CheckLife_Outside(dmobj, ctxt);
    }

    api_log_MsgDebug("英雄:死亡:地点:遗忘的地下神殿:副本里面");

    Tz1853_ReleaseKey(dmobj, ctxt);

    api_log_MsgDebug("血条:检查:死亡,等待出现接受字样");

    //等待出现死亡字样
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(200);

        //如果有地洞副本满框框,则先点掉 2019/7/19 20:47:59
        dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX2, &pposY2);
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
        //bug:有时候出现白色的'副本地下城已满'点不中,导致后续流程出错 2019/8/26 20:46:37
        dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "ffffff-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("副本地下城已满:ffffff:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            
            //点击"副本地下城已满"
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
                code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "ffffff-202020", 0.9, &pposX2, &pposY2);
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("死亡:复活:接受:找字:大范围,成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz1853_CheckAlive(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("死亡:复活:非自主复活,可能来源于其他玩家的施救");
            code = SNAIL_ERRNO_REALIVE;

            //bug:死亡复活之后隐身成功了才退出 2019/7/20 20:35:19
            goto LABEL_CHECK_HIDDEN_OK;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("血条:检查:死亡:接受:找字:大范围,失败");
        Delay(500);
    }

    LUAMAN_WAIT_MS(500);
    

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

        dmobj->SetDict(0, FONT1853_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-000000", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("血条:检查:死亡:接受:点击,成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("血条:检查:死亡:接受:点击,失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    LUAMAN_WAIT_MS(500);
    
    //等待英雄复活后站起来
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz1853_CheckAlive(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("血条:检查:死亡:自主复活,成功");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("血条:检查:死亡:自主复活:等待");
        Delay(500);
    }
    
LABEL_CHECK_HIDDEN_OK:
    //bug:死亡复活之后隐身成功了才退出, 复活之后有短暂保护
    //    不考虑隐身未成功的情况下又遭杀死了,如果出现了
    //    就请ruby哥出来走两圈 2019/7/20 20:35:19 
    hiddenchk = time(NULL);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("9");
        Delay(200);
        dmobj->KeyUpChar("9");
        Delay(200);

        //确认是否隐身成功
        code = dmobj->CmpColor(376, 771, "4b4b4b-101010", 0.9);
        if (!code)
        {
            api_log_MsgDebug("死亡:复活:隐身:成功");
            break;
        }

        end = time(NULL);
        offset = end - hiddenchk;
        if (offset >= 10.0)
        {
            api_log_MsgDebug("死亡:复活:隐身:超时,可能又被杀了");
            break;
        }

        api_log_MsgDebug("死亡:复活:隐身:失败");
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
        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("组队:邀请:添加好友:死亡:复活");
            return code;
        }

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
        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("组队:邀请:战场服务:找字:死亡:复活");
            return code;
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
                code = Tz1853_CheckLife(dmobj, ctxt);
                if (SNAIL_ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("组队:邀请:队员:队伍:找字:死亡:复活");
                    return code;
                }

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
    code = Tz1853_CheckLife(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code)
    {
        api_log_MsgDebug("组队完成:死亡:复活:完成");
        return code;
    }

    api_log_MsgDebug("组队完成:成功:完成");
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
        api_log_MsgError("路径:坐标:非法:寻路:失败,tgtNum:%d, tgtPosXYs:%s", tgtNum, ptr);
        
        tgtPosXYs.ReleaseBuffer();
        return SNAIL_ERRNO_INVALID;
    }
    
    //前进到每个坐标点
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

        Check_Terminate_Thread(luaman_task, dmobj);
        
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
            if (walkTimeDelta > 120.0)
            {
                (void)Tz1853_ReleaseKey(dmobj, ctxt);

                api_log_MsgDebug("寻路:坐标:卡住,停止寻路:等待副本消失,下一坐标, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //等待地洞副本消失
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //先检查下血条
            code = Tz1853_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            //读取当前坐标
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
    //查看'副本消失等待'字样,找到了之后立即按9隐身,并等待退出到副本外
    //bug:注释掉这个 2022/01/10 21:58:49
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT1853_DIR"font-waitcopyoff.txt");
        code = dmobj->FindStrFast(1, 160, 190, 200, "副本消失等待", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("寻路:卡住:副本消失等待:找字,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONT1853_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "米特拉", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("寻路:卡住:地图:米特拉:退出,成功, lposX:%ld, lposY:%ld", lposX, lposY);
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

        //检查血条
        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:卡住:副本消失等待:血条:检查,死亡并复活");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("寻路:卡住:副本消失等待:找字,失败");
    }

    Delay_With_Terminate_Check(4, luaman_task, dmobj);

    //检查地图是否退出到副本外面
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("9");
        Delay(200);
        dmobj->KeyUpChar("9");
        Delay(200);
        
        dmobj->SetDict(0, FONT1853_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "米特拉", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("寻路:卡住:地图:米特拉:退出,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("寻路:卡住:地图:米特拉:退出,正在退出中");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONT1853_DIR"font-exitgame2.txt");
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
        
    //查找背包标签,并下每个标签的坐标
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //检查是否按成功
        dmobj->SetDict(0, FONT1853_DIR"font-package.txt");
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

        api_log_MsgDebug("仓库:背包:字样:找字,失败");
        Delay(500);
    }

    for (index = 0; index < 5; index++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //点开标签页
        dmobj->MoveTo(labelPosXY[index].x, labelPosXY[index].y);
        Delay(200);
        dmobj->LeftDown();
        Delay(200);
        dmobj->LeftUp();
        Delay(200);
        
        api_log_MsgDebug("仓库:背包:清理, index:%d", index);

        //查找宝贝名称
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

                    api_log_MsgDebug("仓库:宝物:坐标:计算,成功,jndex:%d, posX:%ld, posY:%ld", jndex, posX, posY);

                    //点击右键将宝物从背包移到仓库中
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
    
    //查找背包标签
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        //检查是否按成功
        dmobj->SetDict(0, FONT1853_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("X强:仓库:背包:字样:找字,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

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

        api_log_MsgDebug("X强:仓库:背包:字样:找字,成功");
        Delay(500);
    }

    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;
    
    api_log_MsgDebug("X强:仓库:背包:宝物柜, lineX:%ld, lineY:%ld", lineX, lineY);

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

    api_log_MsgDebug("X强:仓库:背包:宝物柜,准备处理");

    //处理每个背包的所有宝物柜
    for (lndex = 0; lndex < 5; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
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
            
            for (jndex = 0; jndex < 10; jndex++)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                posX = boxPosXYs[index * 10 + jndex].x;
                posY = boxPosXYs[index * 10 + jndex].y;

                dmobj->MoveTo(posX, posY);
                Delay(200);

                api_log_MsgDebug("处理:玛瑙:背包:%d:柜子:[%d,%d]:坐标:[%ld, %ld]",
                    lndex, index, jndex, posX, posY);
                
                findxstrong = 0;

                //查找'小强|中强|高强'字样
                dmobj->SetDict(0, FONT1853_DIR"font-xstrong.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "耀金玛瑙", "22741c-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    findxstrong = 1;

                    lposX1 = pposX1.lVal;
                    lposY1 = pposY1.lVal;

                    Delay(200);
                    dmobj->RightClick();
                    Delay(200);

                    api_log_MsgDebug("宝物柜:耀金玛瑙:高强:存储,成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                    //break;
                }

                if (0 == findxstrong)
                {
                    dmobj->SetDict(0, FONT1853_DIR"font-xstrong.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "白银玛瑙", "22741c-101010", 0.9, &pposX1, &pposY1);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        findxstrong = 2;

                        lposX1 = pposX1.lVal;
                        lposY1 = pposY1.lVal;

                        Delay(200);
                        dmobj->RightClick();
                        Delay(200);

                        api_log_MsgDebug("宝物柜:白银玛瑙:高强:存储,成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                        //break;
                    }
                }

                if (0 == findxstrong)
                {
                    dmobj->SetDict(0, FONT1853_DIR"font-xstrong.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "古铜玛瑙", "22741c-101010", 0.9, &pposX1, &pposY1);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        findxstrong = 3;

                        lposX1 = pposX1.lVal;
                        lposY1 = pposY1.lVal;

                        Delay(200);
                        dmobj->RightClick();
                        Delay(200);

                        api_log_MsgDebug("宝物柜:古铜玛瑙:高强:存储,成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
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
    
    api_log_MsgDebug("买东西:普通180:开始");

    posX1 = 1069;
    posY1 = 48;

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        //点击之后出现"仓库"字样
        dmobj->SetDict(0, FONT1853_DIR"font-surebox.txt");
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
        
        dmobj->MoveTo(lposX1, lposY1 - 130);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONT1853_DIR"font-package.txt");
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

    api_log_MsgDebug("X强:仓库:背包:宝物柜, lineX:%ld, lineY:%ld", lineX, lineY);

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

                api_log_MsgDebug("处理:180:碎片:背包:%d:柜子:[%d,%d]:坐标:[%ld, %ld]",
                lndex, index, jndex, posX, posY);
                
                dmobj->SetDict(0, FONT1853_DIR"font-beasellman.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "普通", "ffe08c-101010", 0.9, &pposX1, &pposY1);
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
                code = dmobj->FindStrFast(0, 27, 1279, 826, "米特拉的悲愤碎片", "ffffff-101010", 0.9, &pposX2, &pposY2);
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
    
    api_log_MsgDebug("买东西:普通180:完毕");
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

    //按i弹出背包
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(200);
        dmobj->KeyUpChar("i");
        Delay(200);

        //检查是否按成功
        dmobj->SetDict(0, FONT1853_DIR"font-package.txt");
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
    
    /*
    //找整理背包图片
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

                api_log_MsgDebug("整理背包:位置:计算,成功, tgtNum:%d, posX:%ld, tposY:%ld", tgtNum, posX + 10, posY + 10);
                
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

                api_log_MsgDebug("整理背包:完毕,按i退出");
                break;
            }
        }

        api_log_MsgDebug("整理背包:找图,失败");
        Delay(200);
    }
    */

    //重新按i,并记录背包I/II/III/IV/V的坐标位置
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //检查是否按成功
        dmobj->SetDict(0, FONT1853_DIR"font-package.txt");
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

    //依次处理每一个背包
    for (index = 0; index < 5; index++)
    {
        //点开标签页
        dmobj->MoveTo(labelPosXY[index].x, labelPosXY[index].y);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("箱子:开箱,jndex:%d", jndex);
        
        //查找"箱子"图标
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

                        api_log_MsgDebug("箱子:坐标:计算,成功,jndex:%d, posX:%ld, posY:%ld", jndex, posX, posY);

                        //点击右键找到使用道具字样
                        for (;;)
                        {
                            Check_Terminate_Thread(luaman_task, dmobj);
                            
                            //鼠标移到箱子上,右键
                            dmobj->MoveTo(posX, posY);
                            Delay(500);
                            dmobj->RightClick();
                            Delay(500);

                            //找图片
                            dmobj->MoveTo(5, 5);
                            Delay(500);

                            dmobj->SetDict(0, FONT1853_DIR"font-surebox.txt");
                            code = dmobj->FindStrFast(0, 27, 1279, 826, "确定要使用该道具吗？", "ffffff-101010", 0.9, &pposX2, &pposY2);
                            dmobj->ClearDict(0);
                            if (-1 != code)
                            {
                                lposX2 = pposX2.lVal;
                                lposY2 = pposY2.lVal;

                                api_log_MsgDebug("箱子:开箱:道具:查找,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                                break;
                            }

                            api_log_MsgDebug("箱子:开箱:道具:查找,失败");
                            Delay(500);
                        }

                        //按回车键
                        for (;;)
                        {
                            Check_Terminate_Thread(luaman_task, dmobj);
                            
                            dmobj->KeyDownChar("enter");
                            Delay(200);
                            dmobj->KeyUpChar("enter");
                            Delay(200);

                            dmobj->SetDict(0, FONT1853_DIR"font-surebox.txt");
                            code = dmobj->FindStrFast(0, 27, 1279, 826, "确定要使用该道具吗？", "ffffff-101010", 0.9, &pposX2, &pposY2);
                            dmobj->ClearDict(0);
                            if (-1 == code)
                            {
                                api_log_MsgDebug("箱子:开箱,成功,jndex:%d", jndex);
                                break;
                            }

                            api_log_MsgDebug("箱子:开箱,失败,jndex:%d", jndex);
                        }
                        
                    }
                }//处理完一页中的所有箱子
                
            }
        }//处理完一页中的所有箱子

        //查找"箱子"图标 结束
        api_log_MsgDebug("箱子:开箱,完成,jndex:%d", jndex);
    }

    api_log_MsgDebug("背包:所有:开箱,成功");

    //点击下整理键
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
        api_log_MsgDebug("整理背包:完毕,按i退出, buildx:%ld, buildy:%ld", buildx, buildy);
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

                api_log_MsgDebug("再次整理背包:位置:计算,成功, tgtNum:%d, posX:%ld, tposY:%ld", tgtNum, posX +10, posY +10);
                
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
                api_log_MsgDebug("整理背包:完毕,按i退出");
                break;
            }
        }

        api_log_MsgDebug("整理背包:找图,失败");
        Delay(200);
    }
    */

    api_log_MsgDebug("准备查找仓库图标");
    
    posX1 = 1069;
    posY1 = 48;

    //点击图标
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);

        //点击之后出现"仓库"字样
        dmobj->SetDict(0, FONT1853_DIR"font-surebox.txt");
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
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);

        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONT1853_DIR"font-package.txt");
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

    api_log_MsgDebug("准备清理背包");

    //处理背包中的东西
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

    //直接点109,77坐标点
    dmobj->MoveTo(109, 104);
    Delay(200);
    dmobj->MoveTo(109, 104);
    Delay(200);

    dmobj->LeftClick();
    Delay(200);

    Check_Terminate_Thread(luaman_task, dmobj);
    Delay(2000);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //点奖励
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

    //按ESC退出
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开始", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
                
            api_log_MsgDebug("战斗:辅助:打开:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点:打怪:血条:死亡,复活");
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
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开始", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("战斗:辅助:开始");
            break;
        }

        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点:打怪:血条:死亡,复活");
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "神殿守护者", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                uFlag = 1;
                api_log_MsgDebug("定点:u怪:成功:lposX:%ld,lposY:%ld, fposXY:%s", lposX, lposY, fposXY);
                break;
            }

            uFailedCount++;
            if (uFailedCountMax > 0 && uFailedCount > uFailedCountMax)
            {
                api_log_MsgDebug("定点:u怪:超时:退出, fposXY:%s, uFailedCount:%d", fposXY, uFailedCount);
                return SNAIL_ERRNO_SUCCESS;
            }

            code = Tz1853_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("定点:u怪:血条:死亡,复活, fposXY:%s, uFailedCount:%d", fposXY, uFailedCount);
                return code;
            }

            api_log_MsgDebug("定点:u怪:失败, fposXY:%s", fposXY);
        }
    }
    else
    {
        //bug:在打之前按5下0 2020/7/30 22:53:09
        Tz1853_FiveZero(dmobj, ctxt);
        code = Tz1853_FightAutoStart(dmobj, ctxt, fType);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点:打怪:开始:血条:死亡,复活, fposXY:%s", fposXY);
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "通往下个关口的门已开启。", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
        }
        else if (31 == fType || 32 == fType || 33 == fType)
        {
            dmobj->SetDict(0, FONT1853_DIR"font-aa54.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "守护遗迹的神殿守卫苏醒了。", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                dmobj->SetDict(0, FONT1853_DIR"font-aa44.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "神殿守护者", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
            }
        }
        else if (4 == fType)
        {
            dmobj->SetDict(0, FONT1853_DIR"font-aa57.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "隐藏之房显现了。", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
        }
        else if (8 == fType)
        {
            //查找奖励等待时间 2020/6/3 22:41:11
            dmobj->SetDict(0, FONT1853_DIR"font-aa127.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "奖励等待时间:", "fdb602-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
        }
        else if (9 == fType)
        {
            if (1 == uFlag)
            {
                dmobj->SetDict(0, FONT1853_DIR"font-aa126.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "宝箱", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (0 == code)
                {
                    code = -1;
                }
                else
                {
                    //bug:加一个白色的检查 2021/01/02 21:11:13
                    dmobj->SetDict(0, FONT1853_DIR"font-aa126.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "宝箱", "ffffff-101010", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (0 == code)
                    {
                        code = -1;
                    }
                    else
                    {
                        //bug:怪物血条与195保持一致 2022/06/12 22:36:35
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

            api_log_MsgDebug("定点:打怪:退出:条件:满足, fposXY:%s", fposXY);
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

                api_log_MsgDebug("打怪:限时:超时, fposXY:%s", fposXY);
                return SNAIL_ERRNO_AGAIN;
            }
        }

        code = Tz1853_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            Tz1853_FightAutoStop(dmobj, ctxt);
            
            api_log_MsgDebug("定点:打怪:血条:死亡,复活, fposXY:%s", fposXY);
            return code;
        }
    }

    Tz1853_FightAutoStop(dmobj, ctxt);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("定点:打怪:完成, fposXY:%s", fposXY);
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
    //@bug:新增功能:副本刚刚进入之后启动脚本 @2019/4/15 21:28:44
    dmobj->SetDict(0, FONT1853_DIR"font-maptips.txt");
    code = dmobj->FindStrFast(1140, 29, 1273, 58, "卡伦的火炉", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("地图:卡伦的火炉:启动脚本:成功:lposX:%ld,lposY:%ld", lposX, lposY);
        goto LABEL_START_SCRIPT_IN_COPY;
    }

    api_log_MsgDebug("地图:米特拉:启动脚本:成功");

    for (;;)
    {
    COPY_LABEL_AGAIN:
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        sleepTimesj = 0;

        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("临界区:进入:等待");
            Tz1853EnterCriticalSection();
            api_log_MsgDebug("临界区:进入:成功");

            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                code = Tz1853_InviteHeilfellowJoin2Team(dmobj, ctxt);
                if (SNAIL_ERRNO_AGAIN != code)
                {
                    api_log_MsgDebug("组队:完成");
                    break;
                }

                check_alarm(dmobj, ctxt);
            }

            api_log_MsgDebug("临界区:退出:等待");
            Tz1853LeaveCriticalSection();
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
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    dmobj->KeyDownChar("esc");
                    Delay(500);
                    dmobj->KeyUpChar("esc");
                    Delay(500);

                    dmobj->SetDict(0, FONT1853_DIR"font-exitgame1.txt");
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
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    dmobj->MoveTo(lposX, lposY);
                    Delay(500);
                    dmobj->LeftClick();
                    Delay(500);

                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    dmobj->MoveTo(5, 5);
                    Delay(500);
                
                    dmobj->SetDict(0, FONT1853_DIR"font-exitgame2.txt");
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
                Check_Terminate_Thread(luaman_task, dmobj);
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
                Check_Terminate_Thread(luaman_task, dmobj);
                if (sleepTimesj > blockTimes * 60)
                {
                    break;
                }

                Delay(1000);
                Check_Terminate_Thread(luaman_task, dmobj);
                
                sleepTimesj++;
                api_log_MsgDebug("组队:邀请:失败,睡眠一会后重试, sleepTimesj:%d, blockTimes:%ld", sleepTimesj, blockTimes);
            }
        }
        
        //////////////////////////////////////////////////////////////////////////////////////
        //先走到固定的位置再对话 2022/04/28 23:04:11
        //移动到128:121|120:120|119:127
        /*
        pathofposxys = "128:121|120:120|119:127";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[119:127],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[119:127]:成功");
        */

        copyFullCounter = 0;
        Check_Terminate_Thread(luaman_task, dmobj);
        
    COPY_LABEL_TALK_WITH_NPC:
        Check_Terminate_Thread(luaman_task, dmobj);
        
        retryPreFly = 0;
        
        api_log_MsgDebug("npc1:对话:开始");
        
        Check_Terminate_Thread(luaman_task, dmobj);
    
        //按n对话进入副本
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "[入场]卡伦的火炉", "00ff00-000000", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("npc1:对话:[入场]卡伦的火炉:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            code = Tz1853_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc1:对话:[入场]卡伦的火炉:找字:死亡:复活");
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            api_log_MsgDebug("npc1:对话:[入场]卡伦的火炉:找字:大范围,失败");
            Delay(500);
        }

        //点击'[入场]卡伦的火炉'
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

            //检查是否出现'确认'字样的对话框
            dmobj->SetDict(0, FONT1853_DIR"font-okcancel.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;
            
                api_log_MsgDebug("npc1:对话:确认:找字:大范围,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }
            
            code = Tz1853_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc1:对话:确认:找字:死亡:复活");
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            api_log_MsgDebug("npc1:对话:确认:找字:大范围,失败");
            Delay(500);
        }
        
        //点击'确认'字样对话框
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
                api_log_MsgDebug("npc1:对话:确认:点击:死亡:复活");
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug: 如果有地洞副本满框框,则点确认;否则就是点确认对应的取消 2020/12/27 14:02:18
            //如果有地洞副本满框框,则先点掉 2019/7/19 20:47:59
            dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
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
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    dmobj->MoveTo(lposX + 55, lposY + 100);
                    Delay(200);
                    dmobj->LeftClick();
                    Delay(200);
                    dmobj->MoveTo(5, 5);
                    Delay(200);

                    dmobj->SetDict(0, FONT1853_DIR"font-copyfull.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("原地复活:副本地下城已满:点击:成功");
                        break;
                    }

                    api_log_MsgDebug("原地复活:副本地下城已满:点击:失败");
                    
                    Check_Terminate_Thread(luaman_task, dmobj);
                    Delay(500);
                }

                //@bug:副本地下城满连续超过5次则退出游戏 2019/5/19 23:08:15
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
                
                api_log_MsgDebug("npc1:对话:副本地下城已满:重新对话,copyFullCounter:%d", copyFullCounter);
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            /////////////////////////////////////////////////////////////////////////////
            //有可能满足无法入场的提示 2022/03/06 22:03:48 580,398->636,500
            dmobj->SetDict(0, FONT1853_DIR"font-cantenter.txt");
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
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    dmobj->MoveTo(lposX + 56, lposY + 102);
                    Delay(200);
                    dmobj->LeftClick();
                    Delay(200);
                    dmobj->MoveTo(5, 5);
                    Delay(200);

                    dmobj->SetDict(0, FONT1853_DIR"font-cantenter.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "您不符合入场条件入场", "ffffff-202020", 0.9, &pposX2, &pposY2);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("您不符合入场条件入场:点击:成功");
                        break;
                    }

                    api_log_MsgDebug("您不符合入场条件入场:点击:失败");
                    
                    Check_Terminate_Thread(luaman_task, dmobj);
                    Delay(500);
                }

                api_log_MsgDebug("npc1:对话:您不符合入场条件入场:重新对话");
                goto COPY_LABEL_TALK_WITH_NPC;
            }
            /////////////////////////////////////////////////////////////////////////////

            //检查是否进入副本
            dmobj->SetDict(0, FONT1853_DIR"font-maptips.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "卡伦的火炉", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("npc1:对话:卡伦的火炉:进入,成功:lposX:%ld,lposY:%ld", lposX, lposY);
                break;
            }

            //////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:检查"人员不足" 2022/03/15 22:02:55
            dmobj->SetDict(0, FONT1853_DIR"font-peopleleak.txt");
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
                    
                    dmobj->SetDict(0, FONT1853_DIR"font-peopleleak.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "人员不足", "ffffff-202020", 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 == code)
                    {
                        api_log_MsgDebug("人员不足:点击:成功:重新对话");
                        goto COPY_LABEL_AGAIN;
                    }

                    Check_Terminate_Thread(luaman_task, dmobj);
            
                    code = Tz1853_CheckLife(dmobj, ctxt);
                    if (SNAIL_ERRNO_REALIVE == code)
                    {
                        api_log_MsgDebug("npc1:对话:卡伦的火炉:进入,死亡并复活");
                        goto COPY_LABEL_AGAIN;
                    }

                    api_log_MsgDebug("npc1:对话:卡伦的火炉,正在进入");
                    Delay(500);
                }
                
            }
            //////////////////////////////////////////////////////////////////////////////////////////////////////
            
            Check_Terminate_Thread(luaman_task, dmobj);
            
            code = Tz1853_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc1:对话:卡伦的火炉:进入,死亡并复活");
                goto COPY_LABEL_TALK_WITH_NPC;
            }

            api_log_MsgDebug("npc1:对话:卡伦的火炉,正在进入");
            Delay(500);
        }
        
        //@bug:签到 2019/4/12 23:27:49 不需要再签到
        check_alarm(dmobj, ctxt);

        //新增功能:增加清包处理逻辑
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
        
        //按'0'等待3秒|按'-'等待3秒|按'='等待3秒
        //bug:移到复活后面去
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
        
        //召唤宝宝跟随
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
            
            api_log_MsgDebug("寻路:前往:坐标:[455:385],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[455:385]:成功");

        //@bug:将签到改到这里来 2019/5/15 21:41:23
        Tz1853_Login(dmobj, ctxt);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        ////////////////////////////////////////////////////////////////////////////////////
        //@bug:检查副本进行时间,有就不用再对话4/8/2019 23:43:55
        dmobj->SetDict(0, FONT1853_DIR"font-copyrunning.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本进行时间:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                //按n对话
                dmobj->KeyPressChar("n");
                Delay(500);
                dmobj->KeyUpChar("n");
                Delay(500);

                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(5, 5);
                Delay(200);

                //查找'知道了。(开始)'字样
                dmobj->SetDict(0, FONT1853_DIR"font-npc2.txt");
                code = dmobj->FindStrFast(388, 115, 984, 667, "知道了。(开始)", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("npc2:对话:知道了。(开始):找字,成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }
                

                api_log_MsgDebug("npc2:对话:知道了。(开始):找字,失败");
                Delay(500);
            }
            

            findOkFailedCount = 0;
            
            //点击'知道了。(开始)'字样
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
                code = dmobj->FindStrFast(388, 115, 984, 667, "知道了。(开始)", "00ff00-101010", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("npc2:对话:知道了。(开始):点击,成功");
                    break;
                }

                findOkFailedCount++;
                if (findOkFailedCount > 10)
                {
                    api_log_MsgDebug("npc2:对话:知道了。(开始):点击:失败:超时,继续:后续流程");
                    goto LABEL_find_failed_after_npc_talk;
                }

                api_log_MsgDebug("npc2:对话:好的:找字,失败");
                Delay(500);
            }
        }

    LABEL_find_failed_after_npc_talk:
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //召唤宝宝跟随
        Tz1853_CallPetbehand(dmobj, ctxt);

        //移动到228:155坐标处
        //bug:修改坐标4/8/2019 08:11:31
        pathofposxys = "455:385|446:384|436:383|425:383|415:382|405:383|397:389";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:前往:坐标:[397:389],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[397:389],成功");
    
        roundCounter = 0;
        
    LABEL_fight_at_point_1:
        Check_Terminate_Thread(luaman_task, dmobj);
        
        fposXY = "397:389";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 1, 10, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("定点:打怪:坐标:[397:389],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[397:389]:成功,roundCounter:%d", roundCounter);

        //移动到393:373坐标处
        pathofposxys = "385:389|380:382|385:374|393:373";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:前往:坐标:[393:373],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[393:373],成功, roundCounter:%d", roundCounter);

        fposXY = "393:373";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 1, 20, 10, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("定点:打怪:坐标:[393:373],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[393:373]:成功,roundCounter:%d", roundCounter);

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
                    
                    api_log_MsgDebug("寻路:前往:坐标:[397:389],死亡并复活");
                    goto COPY_LABEL_AGAIN;
                }

                api_log_MsgDebug("寻路:前往:坐标:[397:389],成功, roundCounter:%d", roundCounter);
                goto LABEL_fight_at_point_1;
            }
        }
        
        api_log_MsgDebug("定点:打怪:坐标:[393:373]:成功, roundCounter:%d", roundCounter);

        //移动到293:394坐标处
        pathofposxys = "386:383|378:384|371:384|361:384|351:384|341:384|331:383|320:389|310:393|293:394";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:前往:坐标:[293:394],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[293:394],成功");

        roundCounter = 0;

   LABEL_fight_at_point_2:
        Check_Terminate_Thread(luaman_task, dmobj);
        
        fposXY = "293:394";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 2, 10, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("定点:打怪:坐标:[293:394],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[293:394]:成功,roundCounter:%d", roundCounter);

        //移动到305:367坐标处
        pathofposxys = "287:384|290:372|298:364|308:366|305:367";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("寻路:前往:坐标:[305:367],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[305:367],成功,roundCounter:%d", roundCounter);
        
        fposXY = "305:367";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 2, 20, 10, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[305:367],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[305:367]:成功,roundCounter:%d", roundCounter);

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
                    api_log_MsgDebug("寻路:前往:坐标:[293:394],死亡并复活");
                    goto COPY_LABEL_AGAIN;
                }

                api_log_MsgDebug("寻路:前往:坐标:[293:394],成功, roundCounter:%d", roundCounter);
                goto LABEL_fight_at_point_2;
            }
        }

        api_log_MsgDebug("定点:打怪:坐标:[293:394],成功");

        //移动到278:305坐标处
        pathofposxys = "307:366|304:355|303:344|297:336|292:328|291:320|289:310|278:305";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[278:305],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[278:305],成功");

        roundCounter = 0;
        
    LABEL_fight_at_point_3:
        fposXY = "278:305";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 31, 10, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[278:305],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[278:305]:成功,roundCounter:%d", roundCounter);

        //移动到287:273坐标处
        pathofposxys = "278:305|278:292|278:278|287:273";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[287:273],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[287:273],成功,roundCounter:%d", roundCounter);

        fposXY = "287:273";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 32, 10, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[287:273],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[287:273]:成功,roundCounter:%d", roundCounter);

        //移动到294:295坐标处
        pathofposxys = "298:274|309:280|309:293|307:303|294:295";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[294:295],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[294:295],成功,roundCounter:%d", roundCounter);
    
        fposXY = "294:295";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 33, 10, 10, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[294:295],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[294:295]:成功,roundCounter:%d", roundCounter);

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
                    api_log_MsgDebug("寻路:前往:坐标:[278:305],死亡并复活");
                    goto COPY_LABEL_AGAIN;
                }

                api_log_MsgDebug("寻路:前往:坐标:[278:305],成功, roundCounter:%d", roundCounter);
                goto LABEL_fight_at_point_3;
            }
        }

        api_log_MsgDebug("定点:打怪:坐标:[294:295]:成功");

        fposXY = "294:295";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 4, 0, 10, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[294:295],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[294:295]:成功");
        
        //需求: 注释掉本段代码,直接前进至下一房间 2020/6/3 22:28:46
        pathofposxys = "294:295|289:282|282:273|286:266|286:255|286:243|285:236|280:231";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[280:231],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[280:231],成功");
        
        roundCounter = 0;
        
    LABEL_fight_at_point_4:
        fposXY = "280:231";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 6, 40, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[280:231],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[280:231]:成功, roundCounter:%d", roundCounter);
        
        //移动到281:202坐标处
        pathofposxys = "282:233|282:224|282:213|281:200|281:202";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[281:202],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[281:202],成功, roundCounter:%d", roundCounter);

        fposXY = "281:202";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 6, 30, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[281:202],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[281:202]:成功, roundCounter:%d", roundCounter);
        
        //bug:还是改回来 2020/8/17 23:10:11
        pathofposxys = "282:201|272:196|261:196|249:197";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[249:197],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[249:197],成功, roundCounter:%d", roundCounter);
        
        fposXY = "249:197";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 6, 30, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[249:197],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[249:197]:成功, roundCounter:%d", roundCounter);
        
        //移动到251:230坐标处
        pathofposxys = "249:197|251:204|251:212|252:220|250:234|251:230";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[251:230],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[251:230],成功, roundCounter:%d", roundCounter);

        fposXY = "251:230";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 6, 30, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[251:230],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[251:230]:成功, roundCounter:%d", roundCounter);

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
                    api_log_MsgDebug("寻路:前往:坐标:[280:231],死亡并复活");
                    goto COPY_LABEL_AGAIN;
                }

                api_log_MsgDebug("寻路:前往:坐标:[280:231],成功, roundCounter:%d", roundCounter);
                goto LABEL_fight_at_point_4;
            }
        }
        
        api_log_MsgDebug("定点:打怪:坐标:[251:230]:成功");
        
        //移动到198:224坐标处
        pathofposxys = "251:234|245:240|238:240|229:240|221:241|211:245|212:233|208:227|198:224";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[198:224],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[198:224],成功, roundCounter:%d", roundCounter);
        
        roundCounter = 0;
        
    LABEL_fight_at_point_5:
        fposXY = "198:224";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 7, 30, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[198:224],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[198:224]:成功, roundCounter:%d", roundCounter);
        
        //移动到157:244坐标处
        pathofposxys = "200:225|190:230|183:239|177:243|165:244|157:244";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[157:244],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[157:244],成功, roundCounter:%d", roundCounter);

        fposXY = "157:244";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 7, 30, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[157:244],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[157:244]:成功, roundCounter:%d", roundCounter);

        //移动到156:185坐标处
        pathofposxys = "162:245|169:233|172:226|154:225|153:215|153:205|156:185";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[156:185],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[156:185],成功, roundCounter:%d", roundCounter);
        
        fposXY = "156:185";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 7, 30, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[156:185],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[156:185]:成功, roundCounter:%d", roundCounter);
        
        //移动到208:190坐标处
        pathofposxys = "155:187|167:185|178:184|189:184|206:185|209:200|208:190";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[208:190],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[208:190],成功, roundCounter:%d", roundCounter);

        fposXY = "208:190";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 7, 45, 5, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[208:190],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[208:190]:成功, roundCounter:%d", roundCounter);
        
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
                    api_log_MsgDebug("寻路:前往:坐标:[198:224],死亡并复活");
                    goto COPY_LABEL_AGAIN;
                }

                api_log_MsgDebug("寻路:前往:坐标:[198:224],成功, roundCounter:%d", roundCounter);
                goto LABEL_fight_at_point_5;
            }
        }

        api_log_MsgDebug("定点:打怪:坐标:[208:190]:成功");
        
        //移动到199:146坐标处
        pathofposxys = "206:193|201:162|199:174|199:157|199:146";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[199:146],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[199:146],成功");
        
        //按n对话
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "进入里面。", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("对话:进入里面。:找字:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("对话:进入里面。:找字:失败");
            Delay(500);
        }

        //点击'进入里面。'
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "是，我要进去。", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;

                api_log_MsgDebug("对话:是，我要进去。:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }

            api_log_MsgDebug("对话:是，我要进去。:查找:失败");
            Delay(500);
        }

        //点击'是，我要进去。'
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "是，我要进去。", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("对话:是，我要进去。:点击:成功");
                break;
            }

            api_log_MsgDebug("对话:是，我要进去。:点击:失败");
            Delay(500);
        }
        
        //移动到198:102坐标处
        pathofposxys = "199:132|198:120|198:108|198:102";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz1853_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("寻路:前往:坐标:[198:102],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[198:102],成功");

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //bug:先按下减号 2020/6/3 23:19:44
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
            api_log_MsgDebug("定点:打怪:坐标:[198:102],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[198:102]:成功");


        Delay_With_Terminate_Check(2, luaman_task, dmobj);

    Label_end_next:
        fposXY = "198:102";
        code = Tz1853_FightInFixPointSimple(dmobj, ctxt, 9, 10, 15, fposXY);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("定点:打怪:坐标:[198:102],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("定点:打怪:坐标:[198:102]:成功");
        
        //bug: 2021/01/03 09:09:09 09:42:23 等20秒后按15下空格 2021/01/03 20:51:11
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
            api_log_MsgDebug("寻路:前往:坐标:[200:89],死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("寻路:前往:坐标:[200:89],成功");

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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "离开副本。", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("对话:离开副本。:找字:成功:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            dmobj->SetDict(0, FONT1853_DIR"font-endnext.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "那么我确认看看。", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("对话:那么我确认看看。:找字:成功:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                
                //关闭"那么我确认看看。"
                dmobj->KeyDownChar("esc");
                Delay(500);
                dmobj->KeyUpChar("esc");
                Delay(500);

                Check_Terminate_Thread(luaman_task, dmobj);
                api_log_MsgDebug("末尾:打怪:重试");
                goto Label_end_next;
            }
            
            
            api_log_MsgDebug("对话:离开副本。:找字:成功:失败");
            Delay(500);
        }

        //点击'进入里面。'
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
            code = dmobj->FindStrFast(0, 27, 1279, 826, "是，我要进去。", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("对话:离开副本。:点击:成功");
                break;
            }

            api_log_MsgDebug("对话:离开副本。:点击:失败");
            Delay(500);
        }

        api_log_MsgDebug("副本:进行:完毕:进入:下一循环");

        //退出副本,等待副本消失时间后重新开始下一轮循环
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
        
        //判断是否出现"组队邀请"
        dmobj->SetDict(0, FONT1853_DIR"font-join2team-1.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX - 32, lposY + 126);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONT1853_DIR"font-join2team-2.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT1853_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "卡伦的火炉", "909090-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("组队:主号:进入副本:灰色:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONT1853_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "卡伦的火炉", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("组队:主号:进入副本:白色:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONT1853_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "卡伦的火炉", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("组队:主号:进入副本:黄色:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT1853_DIR"font-join2team-3.txt");
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
        
        dmobj->SetDict(0, FONT1853_DIR"font-join2team-3.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT1853_DIR"font-join2team-4.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("enter");
        Delay(500);
        dmobj->KeyUpChar("enter");
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->SetDict(0, FONT1853_DIR"font-join2team-4.txt");
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

int api_Task1853Init(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    time_t start = time(NULL);
    
    api_log_MsgDebug("副本:1853:初始化: hwnd:%ld, role:%d", hwnd, (int)role);
    api_PrintConfig(luaman_config);
    
    g_lstChkTime = TlsAlloc();
    TlsSetValue(g_lstChkTime, (LPVOID)start);

    code = Tz1853_AllocClientContext(hwnd);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        if (SNAIL_ERRNO_EXIST == code)
        {
            api_log_MsgError("task1853 is aready running, hwnd:%ld", hwnd);
            AfxMessageBox("已运行1853副本");
            return code;
        }

        if (SNAIL_ERRNO_NOSPACE == code)
        {
            api_log_MsgError("task1853 team member reach max, hwnd:%ld", hwnd);
            AfxMessageBox("1853副本队伍满员");
            return code;
        }

        api_log_MsgError("task1853 invalid argument, hwnd:%ld", hwnd);
        AfxMessageBox("1853副本参数错误");
        
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

    /* 绑定窗口 */
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

    api_log_MsgDebug("副本:1853:初始化:完成, hwnd:%ld", hwnd);

    /* 初始化告警时间初始值 */
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
    
    api_log_MsgDebug("副本:1853:主循环: hwnd:%ld, role:%d", hwnd, (int)role);

    switch (role)
    {
    case ROLE_M:
        //@bug:地洞副本满时主号结束游戏:exitGameCopyFull 2019/5/20 21:42:16
        api_log_MsgDebug("副本:1853:主号:主循环:开始, hwnd:%ld", hwnd);
        Tz185_PassCopyLoop(dmobj, ctxt);
        api_log_MsgDebug("副本:1853:主号:主循环:完成, hwnd:%ld", hwnd);
        break;

    default:
        api_log_MsgDebug("副本:1853:副号:主循环:开始, hwnd:%ld", hwnd);
        Tz185_Join2Team(dmobj, ctxt);
        api_log_MsgDebug("副本:1853:副号:主循环:完成, hwnd:%ld", hwnd);
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
    
    api_log_MsgDebug("副本:1853:结束: hwnd:%ld, role:%d", hwnd, (int)role);

    TlsFree(g_lstChkTime);
    //Tz1853DeleteCriticalSection();
    return ERRNO_SUCCESS;
}

