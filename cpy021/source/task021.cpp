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
#include "task021.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "snail_client_context.h"
#include "snail_client_manager.h"

#define FONT021_DIR "font\\021\\"

int Tz021_Fly2Copy(Idmsoft* dmobj, void* ctxt)
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

    for (;;)
    {
        dmobj->KeyDownChar("m");
        Delay(200);
        dmobj->KeyUpChar("m");
        Delay(200);

        //查看m键是否按成功
        dmobj->SetDict(0, FONT021_DIR"font-fly-mcheck1.txt");
        code = dmobj->FindStrFast(210, 45, 1138, 745, "斯佳丽[遗忘的遇难船]", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("斯佳丽[遗忘的遇难船]:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz021_CheckLife(dmobj, ctxt);
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
        dmobj->SetDict(0, FONT021_DIR"font-fly-mcheck2.txt");
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
        
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("副本外面:复活:移动:找字:被杀死:复活,成功");
            return code;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:在移动过程被反复杀死了又复活时可能返回SUCCESS 2021/06/14 11:12:33
        if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("副本外面:复活:移动:死亡:复活:成功:移动:副本:成功");
            break;
        }

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
        dmobj->SetDict(0, FONT021_DIR"font-posxy.txt");
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
                if (abs(posX - 60) <= 1 && abs(posY - 104) <= 1)
                {
                    api_log_MsgDebug("副本外面:复活:移动到副本门口:成功");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz021_CheckLife(dmobj, ctxt);
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

int Tz021_CheckAlive(Idmsoft* dmobj, void* ctxt, int bCopyoutPre)
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
    
    dmobj->SetDict(0, FONT021_DIR"font-blood.txt");
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
        (void)CheckKeyState(dmobj, ctxt);

        if (1 == bCopyoutPre)
        {
            code = Tz021_Fly2Copy(dmobj, ctxt);
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

int Tz021_CheckLife_Inside_FightPoint_Special(Idmsoft* dmobj, void* ctxt)
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
    VARIANT pposX, pposY;
    long lposX = 0, lposY = 0;
    
    (void)check_alarm(dmobj, ctxt);

    dmobj->SetDict(0, FONT021_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("特殊打怪点:蓝血条:读取:异常, groupid:%d, hwnd:%ld",
            luaman_config->groupid, hwnd);
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

        api_log_MsgDebug("特殊打怪点:血条:解析:成功,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
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

    api_log_MsgDebug("特殊打怪点:死亡:等待:传送到:副本外");
    
    //等待被传到副本外面
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "德拉比斯野外", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            api_log_MsgDebug("特殊打怪点:死亡:已经传送到:副本外");
            break;
        }

        Delay(500);
        Check_Terminate_Thread(luaman_task, dmobj);
    }
    
    return ERRNO_REALIVE_X1;
}

//bug:副本外面的血条检查需要改变复活策略 2020/12/22 21:16:22 2021/01/02 13:34:15
int Tz021_CheckLife_Outside(Idmsoft* dmobj, void* ctxt)
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

    (void)check_alarm(dmobj, ctxt);
    
    ///////////////////////////////////////////////////////////////////////////////////
    //@bug:副本外复活,先判断点击接受之前是不是在副本外面 2019/4/24 21:14:44
    //默认在副本外面死的,即便是误判也无非是多余的移动一次,不会影响脚本的功能
    bCopyoutPre = 1;

    dmobj->SetDict(0, FONT021_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("蓝血条:读取:异常, groupid:%d, hwnd:%ld",
            luaman_config->groupid, hwnd);
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

    //出来按一下方向键,防止窜动
    (void)CheckKeyState(dmobj, ctxt);

    api_log_MsgDebug("英雄:死亡:地点:德拉比斯野外:副本外面");

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //等待出现死亡字样
    for (;;)
    {
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        //如果有地洞副本满框框,则先点掉 2019/7/19 20:47:59
        dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
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
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
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

        dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
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
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
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
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONT021_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("死亡:复活:接受:找字:大范围,成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("血条:检查:死亡:接受:找字:大范围,失败");
        Delay(500);
    }//end:等待出现死亡字样

    Delay(500);
    
    //点击接受
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT021_DIR"font-chcklife.txt");
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

    //等待英雄复活后站起来
    Delay(500);

    for (;;)
    {
        code = Tz021_CheckAlive(dmobj, ctxt, 1);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("血条:检查:死亡:自主复活,成功");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("血条:检查:死亡:自主复活:等待");
        Delay(500);
    }

    return SNAIL_ERRNO_REALIVE;

}

int Tz021_CheckLife(Idmsoft* dmobj, void* ctxt)
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
    
    (void)check_alarm(dmobj, ctxt);

    ///////////////////////////////////////////////////////////////////////////////////
    //@bug:副本外复活,先判断点击接受之前是不是在副本外面 2019/4/24 21:14:44
    //默认在副本外面死的,即便是误判也无非是多余的移动一次,不会影响脚本的功能
    bCopyoutPre = 1;
    
    dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
    code = dmobj->FindStrFast(1140, 29, 1273, 58, "德拉比斯野外", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 == code)
    {
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "遗忘的遇难船", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            bCopyoutPre = 0;
        }
        else
        {
            dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "奈特罗斯的安息处", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                bCopyoutPre = 2;
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////
    //bug:副本外面的血条检查需要改变复活策略 2020/12/22 21:16:22 2021/01/02 13:32:15
    if (1 == bCopyoutPre)
    {
        return Tz021_CheckLife_Outside(dmobj, ctxt);
    }

    if (2 == bCopyoutPre)
    {
        return Tz021_CheckLife_Inside_FightPoint_Special(dmobj, ctxt);;
    }

    dmobj->SetDict(0, FONT021_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("蓝血条:读取:异常, groupid:%d, hwnd:%ld",
            luaman_config->groupid, hwnd);
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

    //出来按一下方向键,防止窜动
    (void)CheckKeyState(dmobj, ctxt);

    api_log_MsgDebug("血条:检查:死亡,等待出现接受字样");
    
    //等待出现死亡字样
    for (;;)
    {
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        //如果有地洞副本满框框,则先点掉 2019/7/19 20:47:59
        dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
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
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
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

        dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
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
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
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
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONT021_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("死亡:复活:接受:找字:大范围,成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz021_CheckAlive(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("死亡:复活:非自主复活,可能来源于其他玩家的施救");
            return SNAIL_ERRNO_REALIVE;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("血条:检查:死亡:接受:找字:大范围,失败");
        Delay(500);
    }//end:等待出现死亡字样

    Delay(500);

    //点击接受
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT021_DIR"font-chcklife.txt");
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

    //等待英雄复活后站起来
    Delay(500);

    for (;;)
    {
        code = Tz021_CheckAlive(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_SUCCESS == code || SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("血条:检查:死亡:自主复活,成功");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("血条:检查:死亡:自主复活:等待");
        Delay(500);
    }

    return SNAIL_ERRNO_REALIVE;
}

int Tz021_ExitGame(Idmsoft* dmobj, void* ctxt)
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
    
    //按esc点击重新登录
    for (;;)
    {
        dmobj->KeyDownChar("esc");
        Delay(500);
        dmobj->KeyUpChar("esc");
        Delay(500);

        dmobj->SetDict(0, FONT021_DIR"font-exitgame2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "更换角色", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("esc:游戏结束:弹出,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("esc:游戏结束:弹出,失败");
        Delay(1000);
    }

    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONT021_DIR"font-exitgame2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "游戏信息保存", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("esc:游戏结束:点击,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("esc:游戏结束:点击,失败");
        Delay(1000);
    }

    return ERRNO_EXIT;
}

int Tz021_Walk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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
            if (walkTimeDelta > 40.0)
            //if (walkTimeDelta > 120.0)
            {
                (void)CheckKeyState(dmobj, ctxt);

                api_log_MsgDebug("寻路:坐标:卡住,停止寻路:等待副本消失,下一坐标, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //等待地洞副本消失
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //先检查下血条
            code = Tz021_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            //读取当前坐标
            dmobj->SetDict(0, FONT021_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)CheckKeyState(dmobj, ctxt);
                
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

                (void)CheckKeyState(dmobj, ctxt);
                
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
    /*
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-waitcopyoff.txt");
        code = dmobj->FindStrFast(1, 160, 190, 200, "副本消失等待", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("寻路:卡住:副本消失等待:找字,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        //检查血条
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:卡住:副本消失等待:血条:检查,死亡并复活");
            return code;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        //api_log_MsgDebug("寻路:卡住:副本消失等待:找字,失败");
    }
    */

    //检查地图是否退出到副本外面
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "德拉比斯野外", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("寻路:卡住:地图:德拉比斯野外:退出,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("寻路:卡住:地图:德拉比斯野外:退出,正在退出中");
    }

    return SNAIL_ERRNO_BLOCKED;
}

int Tz021_FightAutoStart(Idmsoft* dmobj, void* ctxt)
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

        dmobj->SetDict(0, FONT021_DIR"font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开始", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("战斗:辅助:打开:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("战斗:辅助:打开:失败");
        LUAMAN_WAIT_MS(500);

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("战斗:辅助:打开:死亡:复活");
            return code;
        }
    }

    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(500);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(500);

        dmobj->SetDict(0, FONT021_DIR"font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开始", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("战斗:辅助:开始:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("战斗:辅助:开始:失败,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        LUAMAN_WAIT_MS(500);

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("战斗:辅助:开始:死亡:复活");
            return code;
        }
    }

    return SNAIL_ERRNO_SUCCESS;
}

int Tz021_FightAutoStop(Idmsoft* dmobj, void* ctxt)
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
    
    return SNAIL_ERRNO_SUCCESS;
}


int Tz021_FightInFixPoint_atc(Idmsoft* dmobj, void* ctxt, struct fight021_param* params)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int code = -1;
    int uFailedCount = 0;
    int cindex = -1;
    int index = -1;
    time_t lstAdjustTime = time(NULL);
    time_t thisAdjustTime = time(NULL);
    double deltaAdjustTime = 0.0;
    time_t lstChkBluebloodTime = time(NULL);
    int uFlag = 0;
    int jndex = 0;
    time_t lstUtime = time(NULL);
    double deltaUtime = 0.0;
    char* fposXY = params->fposXY;
    char* ufontName = params->ufontName;
    char* uName = params->uName;
    char* uclrOffset = params->uclrOffset;
    unsigned int uFailedCountMax = params->uFailedCountMax;
    VARIANT pposX, pposY;
    //VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    //long lposX2 = 0, lposY2 = 0;
    time_t curTime = time(NULL);
    time_t startWalkTime = time(NULL);
    time_t lastUaodesayTime = 0;
    double walkTimeDelta = 0.0;
    double lastPointBattleTime = 60.0;
    int curPointIndex = 0;
    char* boxPosXYs[2][3] =
    {
        {"188:148", "178:163", ""},
        {"125:134", "", ""},
    };
    char* posXY = NULL;
    CString cstrposXY;
    int end_after_try_last = 0;
    
    if (!strcmp(fposXY, "254:254"))
    {
        uFlag = 1;
    }

Label_for_move_to_next_pos:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    code = -1;
    uFailedCount = 0;
    cindex = -1;
    index = -1;
    lstAdjustTime = time(NULL);
    thisAdjustTime = time(NULL);
    deltaAdjustTime = 0.0;
    lstChkBluebloodTime = time(NULL);
    uFlag = 0;
    jndex = 0;
    lstUtime = time(NULL);
    deltaUtime = 0.0;

    code = Tz021_FightAutoStart(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code)
    {
        api_log_MsgDebug("定点:打怪:开始:血条:死亡,复活");
        return code;
    }

Label_for_battle_last_point:
    dmobj->MoveTo(5, 5);
    Delay(100);

    //第三处直接判断怪有没有完
    if (!strcmp(fposXY, "254:254"))
    {
        uFlag = 1;
    }

    uFailedCount = 0;
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        if (strcmp(ufontName, "") && strcmp(uName, "") && (1 == uFlag))
        {
            dmobj->SetDict(0, ufontName);
            code = dmobj->FindStrFast(0, 27, 1279, 826, uName, uclrOffset, 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                uFailedCount = 0;
                uFailedCountMax = 3;

                api_log_MsgDebug("定点:u怪:%s:成功, lposX:%ld, lposY:%ld, fposXY:%s",
                    uName, lposX, lposY, fposXY);

                lastUaodesayTime = time(NULL);
                goto Label_for_check_procedure_end;
            }
            
            dmobj->SetDict(0, ufontName);
            code = dmobj->FindStrFast(0, 27, 1279, 826, uName, "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                uFailedCount = 0;
                uFailedCountMax = 3;

                api_log_MsgDebug("定点:u怪:绿色:%s:成功, lposX:%ld, lposY:%ld, fposXY:%s",
                    uName, lposX, lposY, fposXY);

                lastUaodesayTime = time(NULL);
                goto Label_for_check_procedure_end;
            }

            dmobj->SetDict(0, ufontName);
            code = dmobj->FindStrFast(0, 27, 1279, 826, uName, "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                uFailedCount = 0;
                uFailedCountMax = 3;

                api_log_MsgDebug("定点:u怪:白色:%s:成功, lposX:%ld, lposY:%ld, fposXY:%s",
                    uName, lposX, lposY, fposXY);

                lastUaodesayTime = time(NULL);
                goto Label_for_check_procedure_end;
            }

            //第一次u不到时记录时间
            if (0 == uFailedCount)
            {
                lastUaodesayTime = time(NULL);
                api_log_MsgDebug("定点:u怪:%s:失败, fposXY:%s", uName, fposXY);
            }
            
            uFailedCount = uFailedCount + 1;
            goto Label_for_check_procedure_end;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //第二处打怪
        if (!strcmp(fposXY, "127:156"))
        {
            dmobj->SetDict(0, FONT021_DIR"font-nightrose.txt");
            code = dmobj->FindStrFast(2, 462, 385, 616, "请移动至奈特罗斯的墓穴。", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                (void)Tz021_FightAutoStop(dmobj, ctxt);

                api_log_MsgDebug("请移动至奈特罗斯的墓穴:查找:成功:结束:战斗");
                return SNAIL_ERRNO_SUCCESS;
            }

            api_log_MsgDebug("定点:打怪:第二处:继续:%s", uName);

            //////////////////////////////////////////////////////////////////////////////////////////////////////
            //bug:2021/6/8 22:07:22 来回打
            curTime = time(NULL);
            walkTimeDelta = difftime(curTime, startWalkTime);
            //bug:由300秒修改为210秒 2022/01/08 15:40:15
            //if (walkTimeDelta > 300.0)
            if (walkTimeDelta > 210.0)
            {
                if (curPointIndex < 1)
                {
                    startWalkTime = time(NULL);
                    posXY = boxPosXYs[1][curPointIndex];
                    curPointIndex++;
                    
                    api_log_MsgDebug("定点:打怪:第二处:小怪:未打完:移到:坐标:%s", posXY);
                    
                    cstrposXY = CString(posXY);
                    //code = Tz021_Walk(dmobj, ctxt, cstrposXY, DIR_U);
                    code = Tz021_Walk(dmobj, ctxt, cstrposXY, luaman_config->direction);
                    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                    {
                        api_log_MsgDebug("定点:打怪:第二处:小怪:未打完:移到:坐标:%s:复活", posXY);
                        return code;
                    }
                    
                    api_log_MsgDebug("定点:打怪:第二处:小怪:未打完:移到:坐标:%s:成功", posXY);
                    goto Label_for_move_to_next_pos;
                }

                api_log_MsgDebug("缺陷:定点:打怪:第二处:小怪:超时:退出");
                return SNAIL_ERRNO_SUCCESS;
            }

            uFailedCount = 0;
            goto Label_for_check_procedure_end;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //第一处打怪
        if (!strcmp(fposXY, "167:140"))
        {
            if (0 == uFlag)
            {
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //bug:找左下角的奈特罗斯的鳞片获得就不打了,直接退出进行下一步 2021/06/14 11:26:44
                dmobj->SetDict(0, FONT021_DIR"font-catch-linpin.txt");
                code = dmobj->FindStrFast(2, 462, 385, 616, "奈特罗斯的鳞片获得。", "6495ed-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;
                    
                    (void)Tz021_FightAutoStop(dmobj, ctxt);
                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    api_log_MsgDebug("奈特罗斯的鳞片获得。:查找:成功:打怪:位置1:成功");
                    return SNAIL_ERRNO_SUCCESS;
                }

                //bug:增加奥德赛小怪结束条件 2022/01/15 10:17:32
                dmobj->SetDict(0, FONT021_DIR"font-npc101.txt");
                code = dmobj->FindStrFast(2, 462, 385, 616, "获得金币:", "6495ed-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    (void)Tz021_FightAutoStop(dmobj, ctxt);
                    Check_Terminate_Thread(luaman_task, dmobj);

                    api_log_MsgDebug("定点:打小怪:获得金币:查找:成功:打怪:完成,fposXY:%s", fposXY);
                    return SNAIL_ERRNO_SUCCESS;
                }
                
                dmobj->SetDict(0, FONT021_DIR"font-checkctn.txt");
                code = dmobj->FindStrFast(2, 462, 385, 616, "隐隐传来奥德赛上号的歌声。", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uFlag = 1;
                    uFailedCount = 0;
                    uFailedCountMax = 20;

                    api_log_MsgDebug("定点:打怪:第一处:%s:小怪打完:继续", uName);
                }
                else
                {
                    api_log_MsgDebug("定点:打怪:第一处:%s:小怪打完:未打完", uName);

                    curTime = time(NULL);
                    walkTimeDelta = difftime(curTime, startWalkTime);

                    //bug:最后一个点打4分钟,前两个点只打1分钟 2022/01/10 22:35:19
                    lastPointBattleTime = (end_after_try_last)? 60.0 * 4 : 60.0;
                    
                    if (walkTimeDelta > lastPointBattleTime)
                    {
                        if (curPointIndex < 2)
                        {
                            startWalkTime = time(NULL);
                            posXY = boxPosXYs[0][curPointIndex];
                            curPointIndex++;

                            api_log_MsgDebug("定点:打怪:第一处:小怪:未打完:移到:坐标:%s", posXY);
                    
                            cstrposXY = CString(posXY);
                            //code = Tz021_Walk(dmobj, ctxt, cstrposXY, DIR_U);
                            code = Tz021_Walk(dmobj, ctxt, cstrposXY, luaman_config->direction);
                            if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                            {
                                api_log_MsgDebug("第一处:寻路:前往:坐标:%s:复活", posXY);
                                return code;
                            }

                            api_log_MsgDebug("定点:打怪:第一处:小怪:未打完:移到:坐标:%s:成功", posXY);
                            goto Label_for_move_to_next_pos;
                        }

                        //////////////////////////////////////////////////////////////////
                        //bug: 最后一个点打了2分钟还没有结束就默认结束 2022/01/08 23:07:11
                        //bug: 如果第一次打完还没有找到就继续打一次 2022/01/14 21:26:33
                        if (2 == end_after_try_last)
                        {
                            (void)Tz021_FightAutoStop(dmobj, ctxt);

                            api_log_MsgDebug("打怪:最后一个点:%s:超时:结束:默认:成功", posXY);
                            return ERRNO_NEXTCOPY;
                        }
                        
                        end_after_try_last++;

                        api_log_MsgDebug("打怪:最后一个点:%s:再打4分钟:寻路:钩怪", posXY);
                        
                        //bug:打之前寻路把怪引过来 2022/01/10 22:36:55
                        posXY = "180:150|180:135";
                        cstrposXY = CString(posXY);
                        code = Tz021_Walk(dmobj, ctxt, cstrposXY, luaman_config->direction);
                        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                        {
                            api_log_MsgDebug("最后一个点:寻路:前往:坐标:%s:复活", posXY);
                            return code;
                        }

                        Delay_With_Terminate_Check(2, luaman_task, dmobj);

                        posXY = "180:135|180:150";
                        cstrposXY = CString(posXY);
                        code = Tz021_Walk(dmobj, ctxt, cstrposXY, luaman_config->direction);
                        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
                        {
                            api_log_MsgDebug("最后一个点:寻路:前往:坐标:%s:复活", posXY);
                            return code;
                        }
                        
                        startWalkTime = time(NULL);
                        
                        api_log_MsgDebug("打怪:最后一个点:%s:再打4分钟:%d", posXY, end_after_try_last);

                        code = Tz021_FightAutoStart(dmobj, ctxt);
                        if (SNAIL_ERRNO_REALIVE == code)
                        {
                            api_log_MsgDebug("定点:最后一个点:打怪:开始:血条:死亡,复活:%d", end_after_try_last);
                            return code;
                        }
                        goto Label_for_battle_last_point;
                        //////////////////////////////////////////////////////////////////
                    }
                    
                }//"隐隐传来奥德赛上号的歌声。"
                
            }//if (0 == uFlag)
        }
        
        uFailedCount = 0;

    Label_for_check_procedure_end:
        Check_Terminate_Thread(luaman_task, dmobj);
        
        if (uFailedCount > uFailedCountMax)
        {
            //bug:添加结束条件 2022/01/15 10:01:08
            if (!strcmp(fposXY, "167:140"))
            {
                dmobj->SetDict(0, FONT021_DIR"font-npc101.txt");
                code = dmobj->FindStrFast(2, 462, 385, 616, "获得金币:", "6495ed-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    (void)Tz021_FightAutoStop(dmobj, ctxt);
                    Check_Terminate_Thread(luaman_task, dmobj);

                    api_log_MsgDebug("定点:打怪:获得金币:查找:成功:打怪:完成,fposXY:%s", fposXY);
                    return SNAIL_ERRNO_SUCCESS;
                }

                //如果没有找到就看连续u失败的时间是否超过30秒,超过就退出
                curTime = time(NULL);
                walkTimeDelta = difftime(curTime, lastUaodesayTime);
                if (walkTimeDelta > 30.0)
                {
                    (void)Tz021_FightAutoStop(dmobj, ctxt);
                    Check_Terminate_Thread(luaman_task, dmobj);

                    api_log_MsgDebug("定点:打怪:连续u:30秒失败:%d:打怪:完成,fposXY:%s", uFailedCount, fposXY);
                    return SNAIL_ERRNO_SUCCESS;
                }

                api_log_MsgDebug("定点:打怪:连续u:%d:打怪:继续,fposXY:%s", uFailedCount, fposXY);
                goto Label_continue_u_aodesay;
            }
            
            (void)Tz021_FightAutoStop(dmobj, ctxt);

            api_log_MsgDebug("定点:u怪:重试:%d次:按怪物已打完处理:%s", uFailedCountMax, fposXY);

            for (jndex = 0; jndex <= 20; jndex++)
            {
                dmobj->KeyDown(32);
                Delay(50);
                dmobj->KeyUp(32);
                Delay(50);

                Check_Terminate_Thread(luaman_task, dmobj);
            }

            return SNAIL_ERRNO_SUCCESS;
        }

    Label_continue_u_aodesay:
        Delay(500);

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点:打怪:血条:死亡,复活,fposXY:%s", fposXY);
            return code;
        }

        if (SNAIL_ERRNO_SUCCESS == code)
        {
            if (!strcmp(fposXY, "127:156"))
            {
                dmobj->SetDict(0, FONT021_DIR"font-nightrose.txt");
                code = dmobj->FindStrFast(2, 462, 385, 616, "请移动至奈特罗斯的墓穴。", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    (void)Tz021_FightAutoStop(dmobj, ctxt);

                    Check_Terminate_Thread(luaman_task, dmobj);
                    
                    //按空格捡东西
                    for (jndex = 0; jndex <= 10; jndex++)
                    {
                        dmobj->KeyDown(32);
                        Delay(50);
                        dmobj->KeyUp(32);
                        Delay(50);
                    }

                    ////////////////////////////////////////////////////////////////////////
                    //@bug: 按0键冲掉"请移动至奈特罗斯的墓穴。"字样 2019/5/14 22:48:13
                    for (jndex = 0; jndex <= 6; jndex++)
                    {
                        dmobj->KeyDown(48);
                        Delay(50);
                        dmobj->KeyUp(48);
                        Delay(50);
                    }

                    api_log_MsgDebug("定点:打怪:请移动至奈特罗斯的墓穴。:成功,fposXY:%s", fposXY);
                    return SNAIL_ERRNO_SUCCESS;
                }
            }
        }
    }//for end

    (void)Tz021_FightAutoStop(dmobj, ctxt);

    api_log_MsgDebug("定点:打怪:异常,不可能达到该分支场景,fposXY:%s", fposXY);
    return SNAIL_ERRNO_SUCCESS;
}

int Tz021_CleanPackage(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    //VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    //long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int code = 0;
    int boxPosX0 = -1;
    int boxPosY0 = -1;
    int boxPosX1 = -1;
    int boxPosY1 = -1;
    int lineX = -1;
    int lineY = -1;
    struct xyposition labelPosXY[5];
    struct xyposition boxPosXYs[5][10];
    int deltaX = 30;
    int deltaY = 30;
    int index = 0;
    int jndex = 0;
    int ttndex = 0;
    int posX = 0;
    int posY = 0;
    
    dmobj->MoveTo(5, 5);
    Delay(100);

    //bug:不用清包了 2022/01/08 18:36:11
    return SNAIL_ERRNO_SUCCESS;

    for (;;)
    {
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //检查是否按成功
        dmobj->SetDict(0, FONT021_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("背包:找字:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("背包:找字:失败");
        Delay(500);
    }

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
    
    lineX = boxPosX0 + deltaX + 3;
    lineY = boxPosY0 + deltaY + 3;

    for (index = 0; index <= 4; index++)
    {
        posY = lineY + index * deltaY;
        for (jndex = 0; jndex <= 9; jndex++)
        {
            posX = lineX + jndex * deltaX;
            
            boxPosXYs[index][jndex].x = posX;
            boxPosXYs[index][jndex].y = posY;

            api_log_MsgDebug("boxPosXYs[%d][%d]:%d:%d", index, jndex, posX, posY);
        }
    }

    dmobj->MoveTo(labelPosXY[0].x, labelPosXY[0].y);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    for (jndex = 0; jndex <= 4; jndex++)
    {
        posX = boxPosXYs[0][jndex].x;
        posY = boxPosXYs[0][jndex].y;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(posX, posY);
        Delay(200);
        dmobj->KeyDownChar("ctrl");
        Delay(200);
        
        for(ttndex = 0; ttndex <= 10; ttndex++)
        {
            dmobj->LeftClick();
            Delay(100);
        }
    
        dmobj->KeyUpChar("ctrl");
        Delay(250);
    }

    posX = boxPosXYs[1][0].x;
    posY = boxPosXYs[1][0].y;
    
    dmobj->MoveTo(posX, posY);
    Delay(200);
    dmobj->KeyDownChar("ctrl");
    Delay(250);

    for(ttndex = 0; ttndex <= 10; ttndex++)
    {
        dmobj->LeftClick();
        Delay(100);
    }
    dmobj->KeyUpChar("ctrl");
    Delay(250);

    dmobj->MoveTo(5, 5);
    Delay(100);

    for (;;)
    {
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //检查是否按成功
        dmobj->SetDict(0, FONT021_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            api_log_MsgDebug("背包:关闭:成功");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("背包:关闭:失败");
        Delay(500);
    }

    return SNAIL_ERRNO_SUCCESS;
}

int Tz021_FinishTask(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    unsigned int playWndIndexVal = luaman_config->playWndIndexVal;
    int exitGameCopyFull = luaman_config->exitGameCopyFull;
    int wait2sleep = 0;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int copyFullCounter = 0;
    int retryPreFly = 0;
    int lastPos = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    struct fight021_param params;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int nfailed = 0;
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    //@bug:新增功能:副本刚刚进入之后启动脚本 @2019/4/15 21:28:44
    dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
    code = dmobj->FindStrFast(1140, 29, 1273, 58, "遗忘的遇难船", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("主号:地图:遗忘的遇难船:启动脚本,成功, lposX:%ld, lposY:%ld", lposX, lposY);
        goto LABEL_START_SCRIPT_IN_COPY;
    }

    api_log_MsgDebug("主号:地图:德拉比斯野外:启动脚本,成功, playWndIndexVal:%u", playWndIndexVal);

    if (playWndIndexVal > 0)
    {
        int ij = 0;
        for (ij = 0; ij < playWndIndexVal; ij++)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
        }
    }

COPY_LABEL_AGAIN:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    copyFullCounter = 0;
    retryPreFly = 0;

COPY_LABEL_TALK_WITH_NPC:
    api_log_MsgDebug("主号:npc1:对话:开始");

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //按n对话进入副本
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONT021_DIR"font-npc1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "遗忘的遇难船入场", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("遗忘的遇难船入场:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:对话:遗忘的遇难船入场:找字:死亡:复活");
            goto COPY_LABEL_TALK_WITH_NPC;
        }
        
        retryPreFly = retryPreFly + 1;
        if (retryPreFly > 5)
        {
            api_log_MsgDebug("移动到副本:retryPreFly:%d", retryPreFly);
            
            code = Tz021_Fly2Copy(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc1:对话:遗忘的遇难船入场:找字:超次数对话:移动:成功");
                goto COPY_LABEL_TALK_WITH_NPC;
            }
        }

        api_log_MsgDebug("npc1:对话:遗忘的遇难船入场:找字:大范围,失败");
        Delay(200);
    }

    //点击'遗忘的遇难船入场'
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        //检查是否出现'确认'字样的对话框
        dmobj->SetDict(0, FONT021_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("npc1:对话:确认:找字:大范围,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:对话:确认:找字:死亡:复活");
            goto COPY_LABEL_TALK_WITH_NPC;
        }

        api_log_MsgDebug("npc1:对话:确认:找字:大范围,失败");
        Delay(200);
    }
    
    //点击'确认'字样对话框
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:节省时间 2020/2/25 21:17:16
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:对话:确认:点击:死亡:复活");
            goto COPY_LABEL_TALK_WITH_NPC;
        }

        //检查是否出现'副本地下城已满'字样的对话框
        dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("npc1:对话:副本地下城已满:找字:大范围,成功,lposX:%ld, lposY:%ld", lposX, lposY);
            
            //点击"副本地下城已满"
            for (;;)
            {
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(500);
            }

            /////////////////////////////////////////////////////////////////////////////////////////////
            //@bug:副本地下城满连续超过5次则退出游戏 2019/5/19 23:08:15
            if (copyFullCounter > 5)
            {
                if (1 == exitGameCopyFull)
                {
                    return Tz021_ExitGame(dmobj, ctxt);
                }
            }
            else
            {
                copyFullCounter++;
            }

            api_log_MsgDebug("npc1:对话:副本地下城已满:重新对话,copyFullCounter:%d", copyFullCounter);
            goto COPY_LABEL_TALK_WITH_NPC;
        }//end:"副本地下城已满"

        //检查是否进入副本
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "遗忘的遇难船", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc1:对话:遗忘的遇难船:进入,成功,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        //检查血条
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:对话:遗忘的遇难船:进入,死亡并复活");
            goto COPY_LABEL_AGAIN;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(200);
        api_log_MsgDebug("npc1:对话:遗忘的遇难船,正在进入");
    }//end:点击'确认'字样对话框
    
LABEL_START_SCRIPT_IN_COPY:
    if (2 == lastPos)
    {
        dmobj->KeyDownChar("t");
        Delay(500);
        dmobj->KeyUpChar("t");
        Delay(500);

        api_log_MsgDebug("副本:复活:进入:跳转:第二关");//2037

        pathofposxys = "199:152|174:152|155:137|142:136|133:135|133:143|132:151";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("副本:复活:进入:跳转:前往:坐标:[132:151],死亡并复活");
            
            Delay(1000);
            goto COPY_LABEL_AGAIN;
        }

        api_log_MsgDebug("副本:复活:进入:跳转:前往:坐标:[132:151],成功");
        api_log_MsgDebug("副本:复活:进入:跳转:前往:坐标:[132:151]:对话:准备:完成");

        Check_Terminate_Thread(luaman_task, dmobj);
        
        goto Label_continue_for_step_2;
    }

    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyDownChar("u");
    Delay(500);
    dmobj->KeyUpChar("u");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyDown(192);
    Delay(500);
    dmobj->KeyUp(192);
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //等待左上角的开始时间变成限制时间
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-readygo.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "限制时间:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("限制时间:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("限制时间:查找:死亡:复活");
            
            Delay(1000);

            Check_Terminate_Thread(luaman_task, dmobj);
            goto COPY_LABEL_AGAIN;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("限制时间:查找:失败");
    }

    /******************************************************************************/
    //移动到174:152坐标处
    pathofposxys = "176:155|167:140";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[167:140],死亡并复活");
        
        Delay(1000);
        goto COPY_LABEL_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("寻路:前往:坐标:[167:140],成功");

LABEL_BATTLE_WITH_TARGET:
    params.fposXY = "167:140";
    params.ufontName = FONT021_DIR"font-audesay.txt";
    params.uName = "奥德赛";
    params.uclrOffset = "ff3300-101010";
    params.uFailedCountMax = 120;
    
    code = Tz021_FightInFixPoint_atc(dmobj, ctxt, &params);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("定点:打怪:坐标:[167:140],死亡并复活");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("定点:打怪:[167:140],成功");

    if (ERRNO_NEXTCOPY == code)
    {
        api_log_MsgDebug("167:140:打怪:超时:直接跳到:最后一步:主号");
        goto Label_wait_to_exec_next_copy_master;
    }

    if (0 == lastPos)
    {
        lastPos = 1;
    }

    /******************************************************************************/
    //移动到132:151坐标处
    pathofposxys = "167:140|155:137|142:136|133:135|133:143|132:151";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[132:151],死亡并复活");
        
        Delay(1000);
        goto COPY_LABEL_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    api_log_MsgDebug("寻路:前往:坐标:[132:151],成功");

Label_continue_for_step_2:
    for (;;)
    {
        dmobj->KeyPressChar("n");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("n");
        LUAMAN_WAIT_MS(200);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(100);

        dmobj->SetDict(0, FONT021_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至船内。", "00ff00-000000", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("npc2:对话:移动至船内。:找字:大范围,成功,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("npc2:对话:移动至船内。:找字:死亡:复活");
            goto COPY_LABEL_TALK_WITH_NPC;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(200);
        api_log_MsgDebug("npc2:对话:移动至船内。:找字:大范围,失败");
    }

    //点击'移动至船内。'
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONT021_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至船内。", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("npc2:对话:移动至船内。:点击,成功");
            break;
        }

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            api_log_MsgDebug("npc2:对话:移动至船内。:点击:死亡:复活");
            goto COPY_LABEL_TALK_WITH_NPC;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(500);
        api_log_MsgDebug("npc2:对话:移动至船内。:点击,失败");
    }

    //检查当前坐标,如果坐标不是 119:157 则表明对话失败,第一关还没有打过
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 119) <= 1 && abs(posY - 157) <= 1)
                {
                    api_log_MsgDebug("移动至船内。:对话:成功");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("移动至船内。:移动:进行中...");
        Delay(500);
    }
    
    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    
    //移动到127:156坐标处
    pathofposxys = "127:156";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[127:156],死亡并复活");
        
        Delay(1000);

        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN;
    }

    api_log_MsgDebug("寻路:前往:坐标:[127:156],成功");

    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (1 == lastPos)
    {
        lastPos = 2;
    }

    dmobj->SetDict(0, FONT021_DIR"font-nightrose.txt");
    code = dmobj->FindStrFast(2, 462, 385, 616, "请移动至奈特罗斯的墓穴。", "00ff00-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("查找:奈特罗斯:成功:跳过:房间2, lposX:%ld, lposY:%ld", lposX, lposY);
        goto Label_skip_battle_at_room_2;
    }

    params.fposXY = "127:156";
    params.ufontName = "";
    params.uName = "";
    params.uclrOffset = "";
    params.uFailedCountMax = 150;
    
    code = Tz021_FightInFixPoint_atc(dmobj, ctxt, &params);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("定点:打怪:坐标:[127:156],死亡并复活");
        
        Delay(1000);

        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("定点:打怪:[127:156],成功");

Label_skip_battle_at_room_2:
    if (2 == lastPos)
    {
        lastPos = 3;
    }

    //按F5
    dmobj->KeyDown(116);
    Delay(500);
    dmobj->KeyUp(116);
    Delay(1000);

    //////////////////////////////////////////////////////////////////////////////////
    //移动到121:125坐标处
    //bug:停下再走 2022/01/26 22:41:33
    //pathofposxys = "127:155|124:145|124:135|117:133|118:129|121:125";
    pathofposxys = "127:155|124:145|124:135|117:133";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[117:133],死亡并复活");
        
        Delay(1000);

        Check_Terminate_Thread(luaman_task, dmobj);

        if (SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("卡住:直接跳到:最后一步:开始:下一副本");
            goto Label_wait_to_exec_next_copy_master;
        }
        goto COPY_LABEL_AGAIN;
    }

    Delay_With_Terminate_Check(2, luaman_task, dmobj);
    
    api_log_MsgDebug("寻路:前往:坐标:[117:133],成功");
    
    pathofposxys = "118:129|121:125";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[121:125],死亡并复活");
        
        Delay(1000);

        Check_Terminate_Thread(luaman_task, dmobj);

        if (SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("卡住:直接跳到:最后一步:开始:下一副本");
            goto Label_wait_to_exec_next_copy_master;
        }
        goto COPY_LABEL_AGAIN;
    }

    //////////////////////////////////////////////////////////////////////////////////
    
    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("寻路:前往:坐标:[121:125],成功");
    api_log_MsgDebug("坐标:[121:125]:等待:暂停任务");

    //等待一会
    wait2sleep = 0;

    for (;;)
    {
        if (wait2sleep > luaman_config->Wait2Sleep)
        {
            api_log_MsgDebug("坐标:[121:125]:等待:时间到:恢复任务");
            break;
        }

        api_log_MsgDebug("坐标:[121:125]:等待:时间到:%d:%d", wait2sleep, luaman_config->Wait2Sleep);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        wait2sleep++;
        Delay(1000);
    }

    //u传送石按~传送
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(500);

    nfailed = 0;
    
    //bug:原来是按u再按`,修改为按n后点击对话 2022/01/06 22:17:59
    for (;;)
    {
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT021_DIR"font-npc100.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至奈特罗斯的息处", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("传送石:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);

        nfailed++;
        
        api_log_MsgDebug("传送石:查找:失败,nfailed:%d", nfailed);
        Delay(500);
        
        if (nfailed > 30)
        {
            api_log_MsgDebug("传送石:查找:失败:等待被传送出去");
            goto Label_wait_to_exec_next_copy_master;
        }
    }

    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONT021_DIR"font-npc100.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至奈特罗斯的息处", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至奈特罗斯的息处:查找:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("移动至奈特罗斯的息处:查找:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    //传送至260:235
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 260) <= 1 && abs(posY - 235) <= 1)
                {
                    api_log_MsgDebug("传送至260:235:成功:成功");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("传送至260:235:进行中...");
        Delay(1000);
    }

    //bug:传过来之后按F1 2021/06/14 11:17:15
    dmobj->KeyDownChar("f1");
    Delay(200);
    dmobj->KeyUpChar("f1");
    Delay(200);
    dmobj->MoveTo( 5, 5);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //移动到254:254坐标处
    pathofposxys = "260:235|255:249|254:254";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[254:254],死亡并复活");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);

        if (SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("卡住:直接跳到:最后一步:开始:下一副本");
            goto Label_wait_to_exec_next_copy_master;
        }
        goto COPY_LABEL_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    api_log_MsgDebug("寻路:前往:坐标:[254:254],成功");

    if (3 == lastPos)
    {
        lastPos = 4;
    }

    params.fposXY = "254:254";
    params.ufontName = FONT021_DIR"font-unightrose.txt";
    params.uName = "奈特罗斯";
    params.uclrOffset = "ff3300-101010";
    params.uFailedCountMax = 60;
    
    code = Tz021_FightInFixPoint_atc(dmobj, ctxt, &params);
    if (ERRNO_REALIVE_X1 == code)
    {
        api_log_MsgDebug("定点:打怪:坐标:[254:254],死亡:复活:在副本外");
        goto Label_finish_this_copy_master;
    }
    
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("定点:打怪:坐标:[254:254],死亡并复活");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);

        if (SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("卡住:直接跳到:最后一步:开始:下一副本");
            goto Label_wait_to_exec_next_copy_master;
        }
        
        goto COPY_LABEL_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    api_log_MsgDebug("定点:打怪:[254:254],成功");

    if (4 == lastPos)
    {
        lastPos = 5;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //bug:注释掉 2022/01/06 22:02:18
    /*
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(500);

    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-success_end.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "遇难船的转盘", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("查找:开始:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("查找:开始:失败");
        Delay(500);
    }
    
    dmobj->MoveTo(lposX + 27, lposY + 151);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    */
    ///////////////////////////////////////////////////////////////////////////////////////////

    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(500);

Label_wait_to_exec_next_copy_master:
    
    api_log_MsgDebug("地图:德拉比斯野外:传送:等待");

    Check_Terminate_Thread(luaman_task, dmobj);
    
    (void)Tz021_CleanPackage(dmobj, ctxt);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //等待被传送出去
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "德拉比斯野外", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            api_log_MsgDebug("地图:德拉比斯野外:传送:成功");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(1000);
    }

    if (playWndIndexVal > 0)
    {
        int ij = 0;
        for (ij = 0; ij < playWndIndexVal; ij++)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
        }
    }

    lastPos = 0;
    //COPY_LABEL_AGAIN

Label_finish_this_copy_master:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("副本:执行:完成:继续:下一轮:副本");
    return ERRNO_SUCCESS;
}

int Tz021_FinishTask_slave(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    unsigned int playWndIndexVal = luaman_config->playWndIndexVal;
    int exitGameCopyFull = luaman_config->exitGameCopyFull;
    int code = 0;
    time_t cleanPackageTimeLast = time(NULL);
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int copyFullCounter = 0;
    int retryPreFly = 0;
    int lastPos = 0;
    int refAlive = 0;
    int REALIVE_REF_POS_2 = 1000;
    CString strCopylefttime;
    int count = 0;
    CStringArray copylefttime;
    long copylefttimem = 0;
    char* ptr = NULL;
    char* pathofposxys = NULL;
    CString pathOfPosXYs;
    struct fight021_param params;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int wait2sleep = 0;
    int nfailed = 0;
    
    if (1 == luaman_config->cleanPackageWhenStart)
    {
        cleanPackageTimeLast = -1;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    //@bug:新增功能:副本刚刚进入之后启动脚本 @2019/4/15 21:28:44
    dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
    code = dmobj->FindStrFast(1140, 29, 1273, 58, "遗忘的遇难船", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("副号:地图:遗忘的遇难船:启动脚本,成功, lposX:%ld, lposY:%ld", lposX, lposY);
        goto LABEL_START_SCRIPT_IN_COPY_slave;
    }

    api_log_MsgDebug("副号:地图:德拉比斯野外:启动脚本,成功, playWndIndexVal:%u", playWndIndexVal);

    if (playWndIndexVal > 0)
    {
        int ij = 0;
        for (ij = 0; ij < playWndIndexVal; ij++)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
        }
    }

COPY_LABEL_AGAIN_slave:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    copyFullCounter = 0;
    retryPreFly = 0;

COPY_LABEL_TALK_WITH_NPC_slave:
    api_log_MsgDebug("副号:npc1:对话:开始");

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //按n对话进入副本
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONT021_DIR"font-npc1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "遗忘的遇难船入场", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("遗忘的遇难船入场:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:对话:遗忘的遇难船入场:找字:死亡:复活");
            goto COPY_LABEL_TALK_WITH_NPC_slave;
        }
        
        retryPreFly = retryPreFly + 1;
        if (retryPreFly > 5)
        {
            api_log_MsgDebug("移动到副本:retryPreFly:%d", retryPreFly);
            
            code = Tz021_Fly2Copy(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc1:对话:遗忘的遇难船入场:找字:超次数对话:移动:成功");

                Check_Terminate_Thread(luaman_task, dmobj);
                goto COPY_LABEL_TALK_WITH_NPC_slave;
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("npc1:对话:遗忘的遇难船入场:找字:大范围,失败");
        Delay(200);
    }

    //点击'遗忘的遇难船入场'
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        //检查是否出现'确认'字样的对话框
        dmobj->SetDict(0, FONT021_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("npc1:对话:确认:找字:大范围,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:对话:确认:找字:死亡:复活");
            goto COPY_LABEL_TALK_WITH_NPC_slave;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("npc1:对话:确认:找字:大范围,失败");
        Delay(200);
    }

    //点击'确认'字样对话框
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:节省时间 2020/2/25 21:17:16
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:对话:确认:点击:死亡:复活");
            goto COPY_LABEL_TALK_WITH_NPC_slave;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        //检查是否出现'副本地下城已满'字样的对话框
        dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("npc1:对话:副本地下城已满:找字:大范围,成功,lposX:%ld, lposY:%ld", lposX, lposY);
            
            //点击"副本地下城已满"
            for (;;)
            {
                dmobj->MoveTo(lposX + 55, lposY + 100);
                Delay(200);
                dmobj->LeftClick();
                Delay(200);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT021_DIR"font-copyfull.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(500);
            }

            /////////////////////////////////////////////////////////////////////////////////////////////
            //@bug:副本地下城满连续超过5次则退出游戏 2019/5/19 23:08:15
            if (copyFullCounter > 5)
            {
                if (1 == exitGameCopyFull)
                {
                    return Tz021_ExitGame(dmobj, ctxt);
                }
            }
            else
            {
                copyFullCounter++;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("npc1:对话:副本地下城已满:重新对话,copyFullCounter:%d", copyFullCounter);
            goto COPY_LABEL_TALK_WITH_NPC_slave;
        }//end:"副本地下城已满"

        //检查是否进入副本
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "遗忘的遇难船", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("npc1:对话:遗忘的遇难船:进入,成功,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        //检查血条
        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("npc1:对话:遗忘的遇难船:进入,死亡并复活");
            goto COPY_LABEL_AGAIN_slave;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        Delay(200);
        api_log_MsgDebug("npc1:对话:遗忘的遇难船,正在进入");
    }//end:点击'确认'字样对话框

LABEL_START_SCRIPT_IN_COPY_slave:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    if (2 == lastPos)
    {
        dmobj->KeyDownChar("t");
        Delay(500);
        dmobj->KeyUpChar("t");
        Delay(500);

        api_log_MsgDebug("副本:复活:进入:跳转:第二关");//2037

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //bug:新增功能 2021/06/11 21:45:16 在第二步死了进来判断时间不足就直接等被拉到第三个房间
        //bug:修改查找范围 2021/06/13 07:56:33
        ////////////////////////////////////////////////////////////////////////////
        //bug:现在主号拉不过去了,需要自己走过去 2022/01/08 15:34:23
        //bug:直接跳到boss打完,等待下一盘 2022/01/08 18:36:55
        if (REALIVE_REF_POS_2 == refAlive)
        {
            dmobj->SetDict(0, FONT021_DIR"font-copylefttime.txt");
            strCopylefttime = dmobj->Ocr(10, 200, 185, 230, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (strCopylefttime != CString(""))
            {
                count = SplitStringWide(strCopylefttime, "分", copylefttime);
                if (count > 2)
                {
                    copylefttimem = atol(copylefttime.GetAt(0));
                    if (copylefttimem < 26)
                    {
                        refAlive = 0;
                        
                        ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                        api_log_MsgDebug("副本:复活:时间:不足:等待:拉到:房间3:%s", ptr);
                        strCopylefttime.ReleaseBuffer();

                        Check_Terminate_Thread(luaman_task, dmobj);

                        goto Label_wait_to_the_next_loop;
                        //goto Label_for_wait_to_polled_lytayrose_slave;
                    }
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////

        Check_Terminate_Thread(luaman_task, dmobj);
        
        pathofposxys = "199:152|174:152|155:137|142:136|133:135|133:143|132:151";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("副本:复活:进入:跳转:前往:坐标:[132:151],死亡并复活");
            
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            goto COPY_LABEL_AGAIN_slave;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("副本:复活:进入:跳转:前往:坐标:[132:151],成功");
        api_log_MsgDebug("副本:复活:进入:跳转:前往:坐标:[132:151]:对话:准备:完成");

        goto Label_continue_for_step_2_slave;
    }

    //等待左上角的开始时间变成限制时间
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-readygo.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "限制时间:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("限制时间:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = Tz021_CheckLife(dmobj, ctxt);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("限制时间:查找:死亡:复活");
            
            Delay(1000);
            goto COPY_LABEL_AGAIN_slave;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        Delay(500);
        api_log_MsgDebug("限制时间:查找:失败");
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    /******************************************************************************/
    //移动到174:152坐标处
    pathofposxys = "176:155|167:140";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[167:140],死亡并复活");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN_slave;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    api_log_MsgDebug("寻路:前往:坐标:[167:140],成功");
    
LABEL_BATTLE_WITH_TARGET_slave:
    params.fposXY = "167:140";
    params.ufontName = FONT021_DIR"font-audesay.txt";
    params.uName = "奥德赛";
    params.uclrOffset = "ff3300-101010";
    params.uFailedCountMax = 120;
    
    code = Tz021_FightInFixPoint_atc(dmobj, ctxt, &params);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("定点:打怪:坐标:[167:140],死亡并复活");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN_slave;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("定点:打怪:[167:140],成功");

    if (ERRNO_NEXTCOPY == code)
    {
        api_log_MsgDebug("167:140:打怪:超时:直接跳到:最后一步:副号");
        goto Label_wait_to_exec_next_copy_slave;
    }
    
    if (0 == lastPos)
    {
        lastPos = 1;
    }

    if (lastPos > 1)
    {
        /******************************************************************************/
        //移动到132:151坐标处
        pathofposxys = "167:140|155:137|142:136|133:135|133:143|132:151";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("寻路:前往:坐标:[132:151],死亡并复活");
            
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
            goto COPY_LABEL_AGAIN_slave;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("寻路:前往:坐标:[132:151],成功");
    }
    
Label_continue_for_step_2_slave:
    if (lastPos > 1)
    {
        for (;;)
        {
            dmobj->KeyPressChar("n");
            LUAMAN_WAIT_MS(200);
            dmobj->KeyUpChar("n");
            LUAMAN_WAIT_MS(200);
            dmobj->MoveTo(5, 5);
            LUAMAN_WAIT_MS(100);

            dmobj->SetDict(0, FONT021_DIR"font-npc2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至船内。", "00ff00-000000", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("npc2:对话:移动至船内。:找字:大范围,成功,lposX:%ld, lposY:%ld",
                    lposX, lposY);
                break;
            }

            code = Tz021_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc2:对话:移动至船内。:找字:死亡:复活");
                goto COPY_LABEL_TALK_WITH_NPC_slave;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            Delay(200);
            api_log_MsgDebug("npc2:对话:移动至船内。:找字:大范围,失败");
        }

        //点击'移动至船内。'
        for (;;)
        {
            dmobj->MoveTo(lposX, lposY);
            LUAMAN_WAIT_MS(300);
            dmobj->LeftClick();
            LUAMAN_WAIT_MS(300);
            dmobj->MoveTo(5, 5);
            LUAMAN_WAIT_MS(300);

            dmobj->SetDict(0, FONT021_DIR"font-npc2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至船内。", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("npc2:对话:移动至船内。:点击,成功");
                break;
            }

            code = Tz021_CheckLife(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("npc2:对话:移动至船内。:点击:死亡:复活");
                goto COPY_LABEL_TALK_WITH_NPC_slave;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            Delay(500);
            api_log_MsgDebug("npc2:对话:移动至船内。:点击,失败");
        }
    }

    //检查当前坐标,如果坐标不是 119:157 则表明对话失败,第一关还没有打过
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 119) <= 1 && abs(posY - 157) <= 1)
                {
                    api_log_MsgDebug("移动至船内。:对话:成功");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("移动至船内。:移动:进行中...");
        Delay(1000);
    }

    /////////////////////////////////////////////////////////////////////////
    //bug:现在不需要检查这个了 2022/01/08 15:36:56 ==>恢复,跳过打boss 2022/01/08 18:38:15
    dmobj->SetDict(0, FONT021_DIR"font-nightrose.txt");
    code = dmobj->FindStrFast(2, 462, 385, 616, "请移动至奈特罗斯的墓穴。", "00ff00-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("查找:奈特罗斯:成功:跳过:房间2, lposX:%ld, lposY:%ld", lposX, lposY);
        //goto Label_for_wait_to_polled_lytayrose_slave;
        goto Label_fight_127_156_finish;
    }
    
    /////////////////////////////////////////////////////////////////////////
    
    Check_Terminate_Thread(luaman_task, dmobj);
    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    Delay(1000);
    Check_Terminate_Thread(luaman_task, dmobj);
    Delay(1000);

    
    //移动到127:156坐标处
    pathofposxys = "127:156";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[127:156],死亡并复活");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN_slave;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("寻路:前往:坐标:[127:156],成功");

    if (1 == lastPos)
    {
        lastPos = 2;
    }

    params.fposXY = "127:156";
    params.ufontName = "";
    params.uName = "";
    params.uclrOffset = "";
    params.uFailedCountMax = 150;
    
    code = Tz021_FightInFixPoint_atc(dmobj, ctxt, &params);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("定点:打怪:坐标:[127:156],死亡并复活");

        refAlive = REALIVE_REF_POS_2;
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);
        goto COPY_LABEL_AGAIN_slave;
    }

Label_fight_127_156_finish:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    refAlive = 0;
    
    api_log_MsgDebug("定点:打怪:[127:156],成功");

    if (2 == lastPos)
    {
        lastPos = 3;
    }

    //按F5
    dmobj->KeyDown(116);
    Delay(500);
    dmobj->KeyUp(116);
    Delay(1000);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //bug:跟主号保持一致 2022/01/05 22:28:29
    //if (lastPos > 3) 
    {
        //移动到121:125坐标处 bug:停2秒再继续走 2022/01/26 22:45:16
        //pathofposxys = "127:155|124:145|124:135|117:133|118:129|121:125";
        pathofposxys = "127:155|124:145|124:135|117:133";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("寻路:前往:坐标:[117:133],死亡并复活");
            
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);

            if (SNAIL_ERRNO_BLOCKED == code)
            {
                api_log_MsgDebug("卡住:直接跳到:最后一步:开始:下一副本");
                goto Label_wait_to_the_next_loop;
            }
            
            goto COPY_LABEL_AGAIN_slave;
        }

        api_log_MsgDebug("寻路:前往:坐标:[117:133],成功");

        Delay_With_Terminate_Check(2, luaman_task, dmobj);
        
        pathofposxys = "118:129|121:125";
        pathOfPosXYs = CString(pathofposxys);
        code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("寻路:前往:坐标:[121:125],死亡并复活");
            
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);

            if (SNAIL_ERRNO_BLOCKED == code)
            {
                api_log_MsgDebug("卡住:直接跳到:最后一步:开始:下一副本");
                goto Label_wait_to_the_next_loop;
            }
            
            goto COPY_LABEL_AGAIN_slave;
        }
        
        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("寻路:前往:坐标:[121:125],成功");
        api_log_MsgDebug("坐标:[121:125]:等待:暂停任务");

        //等待一会
        wait2sleep = 0;

        for (;;)
        {
            if (wait2sleep > luaman_config->Wait2Sleep)
            {
                api_log_MsgDebug("坐标:[121:125]:等待:时间到:恢复任务");
                break;
            }

            api_log_MsgDebug("坐标:[121:125]:等待:时间到:%d:%d", wait2sleep, luaman_config->Wait2Sleep);
            
            wait2sleep++;
            Delay(1000);
            
            Check_Terminate_Thread(luaman_task, dmobj);
        }

        //u传送石按~传送
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(500);

        nfailed = 0;
        
        //bug:原来是按u再按`,修改为按n后点击对话 2022/01/06 22:17:59
        for (;;)
        {
            dmobj->KeyDownChar("n");
            Delay(500);
            dmobj->KeyUpChar("n");
            Delay(500);

            dmobj->SetDict(0, FONT021_DIR"font-npc100.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至奈特罗斯的息处", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("移动至奈特罗斯的息处:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);

            nfailed++;
            
            api_log_MsgDebug("移动至奈特罗斯的息处:查找:失败, nfailed:%d", nfailed);
            Delay(500);

            //bug:增加最大次数限制 2022/01/26 22:49:11
            if (nfailed > 30)
            {
                api_log_MsgDebug("移动至奈特罗斯的息处:查找:失败:等待被传送出去");
                goto Label_wait_to_the_next_loop;
            }
        }

        for (;;)
        {
            dmobj->MoveTo(lposX, lposY);
            LUAMAN_WAIT_MS(300);
            dmobj->LeftClick();
            LUAMAN_WAIT_MS(300);
            dmobj->MoveTo(5, 5);
            LUAMAN_WAIT_MS(300);

            dmobj->SetDict(0, FONT021_DIR"font-npc100.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至奈特罗斯的息处", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("移动至奈特罗斯的息处:点击:成功");
                break;
            }

            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            Check_Terminate_Thread(luaman_task, dmobj);
            
            api_log_MsgDebug("移动至奈特罗斯的息处:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            Delay(500);
        }
    }

Label_for_wait_to_polled_lytayrose_slave:
    //传送至260:235
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 260) <= 1 && abs(posY - 235) <= 1)
                {
                    api_log_MsgDebug("传送至260:235:成功:成功");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        
        api_log_MsgDebug("传送至260:235:进行中...");
        Delay(1000);
    }

    if (3 == lastPos)
    {
        lastPos = 4;
    }

    //bug:传过来之后按F1 2021/06/14 11:17:15
    dmobj->KeyDownChar("f1");
    Delay(200);
    dmobj->KeyUpChar("f1");
    Delay(200);
    dmobj->MoveTo( 5, 5);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //移动到254:254坐标处
    pathofposxys = "260:235|255:249|254:254";
    pathOfPosXYs = CString(pathofposxys);
    code = Tz021_Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[254:254],死亡并复活");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);

        if (SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("卡住:直接跳到:最后一步:开始:下一副本");
            goto Label_wait_to_the_next_loop;
        }
        goto COPY_LABEL_AGAIN_slave;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("寻路:前往:坐标:[254:254],成功");

    params.fposXY = "254:254";
    params.ufontName = FONT021_DIR"font-unightrose.txt";
    params.uName = "奈特罗斯";
    params.uclrOffset = "ff3300-101010";
    params.uFailedCountMax = 60;
    
    code = Tz021_FightInFixPoint_atc(dmobj, ctxt, &params);
    if (ERRNO_REALIVE_X1 == code)
    {
        api_log_MsgDebug("定点:打怪:坐标:[254:254],死亡:复活:在副本外");
        goto Label_finish_this_copy_slave;
    }
    
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("定点:打怪:坐标:[254:254],死亡并复活");
        
        Delay(1000);
        Check_Terminate_Thread(luaman_task, dmobj);

        if (SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("卡住:直接跳到:最后一步:开始:下一副本");
            goto Label_wait_to_the_next_loop;
        }
        
        goto COPY_LABEL_AGAIN_slave;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("定点:打怪:[254:254],成功");

    if (4 == lastPos)
    {
        lastPos = 5;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    //bug:注释掉 2022/01/06 22:01:16
    /*
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(500);

    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-success_end.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "遇难船的转盘", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("查找:开始:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("查找:开始:失败");
        Delay(500);
    }

    dmobj->MoveTo(lposX + 27, lposY + 151);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    */
    /////////////////////////////////////////////////////////////////////////////////////////
    
    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(500);

Label_wait_to_exec_next_copy_slave:
Label_wait_to_the_next_loop:
    api_log_MsgDebug("地图:德拉比斯野外:传送:等待");

    Check_Terminate_Thread(luaman_task, dmobj);
    
    (void)Tz021_CleanPackage(dmobj, ctxt);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //等待被传送出去
    for (;;)
    {
        dmobj->SetDict(0, FONT021_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "德拉比斯野外", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            api_log_MsgDebug("地图:德拉比斯野外:传送:成功");
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        Delay(1000);
    }

    if (playWndIndexVal > 0)
    {
        int ij = 0;
        for (ij = 0; ij < 2 * playWndIndexVal; ij ++)
        {
            Delay(1000);
            Check_Terminate_Thread(luaman_task, dmobj);
        }
    }

    lastPos = 0;

Label_finish_this_copy_slave:
    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("副本:执行:完成:继续:下一轮:副本");
    return ERRNO_SUCCESS;
}

int api_Task021Init(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int code = 0;
    
    api_log_MsgDebug("副本:021:初始化:开始, hwnd:%ld", hwnd);

    code = snail_AllocClientContext(hwnd);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        if (SNAIL_ERRNO_EXIST == code)
        {
            api_log_MsgError("task021 is aready running, hwnd:%ld", hwnd);
            AfxMessageBox("已运行021副本");
            return code;
        }

        if (SNAIL_ERRNO_NOSPACE == code)
        {
            api_log_MsgError("task021 team member reach max, hwnd:%ld", hwnd);
            AfxMessageBox("021副本队伍满员");
            return code;
        }

        api_log_MsgError("task021 invalid argument, hwnd:%ld", hwnd);
        AfxMessageBox("021副本参数错误");
        
        return code;
    }

    code = snail_UpdateClientTaskId(hwnd, luaman_task->taskid);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        snail_FreeClientContext(hwnd);

        api_log_MsgError("task021 snail_UpdateClientTaskId failed, hwnd:%ld, code:%d",
            hwnd, code);
        return code;
    }

    /* 绑定窗口 */
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("bind 021 hwnd failed, hwnd:%ld", hwnd);

        snail_FreeClientContext(hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("bind 021 hwnd success, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("lock 021 hwnd failed, hwnd:%ld", hwnd);

        snail_FreeClientContext(hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("副本:021:初始化:完成, hwnd:%ld", hwnd);

    /* 初始化告警时间初始值 */
    init_alarm_ctxt();
    
    return ERRNO_SUCCESS;
}

int api_Task021Main(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int code = 0;

    api_log_MsgDebug("副本:021:主循环: hwnd:%ld, role:%d", hwnd, (int)role);

    switch (role)
    {
    case ROLE_M:
        api_log_MsgDebug("副本:021:主号:主循环:开始, hwnd:%ld", hwnd);
        (void)Tz021_FinishTask(dmobj, ctxt);
        api_log_MsgDebug("副本:021:主号:主循环:完成, hwnd:%ld", hwnd);
        break;

    case ROLE_S0:
        api_log_MsgDebug("副本:021:副号:主循环:开始, hwnd:%ld", hwnd);
        (void)Tz021_FinishTask_slave(dmobj, ctxt);
        api_log_MsgDebug("副本:021:副号:主循环:完成, hwnd:%ld", hwnd);
        break;
    
    case ROLE_S1:
    case ROLE_AUX:
    default:
        {
            AfxMessageBox("请选择正确的游戏角色(仅支持主号|副号),当前窗口任务终止");
            AfxMessageBox("5秒钟后重启窗口任务");
            return ERRNO_INVALID;
        }
        break;
    }
    
    return ERRNO_SUCCESS;
}

int api_Task021Exit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    
    snail_FreeClientContext(hwnd);

    api_log_MsgDebug("副本:021:退出, hwnd:%ld, groupid:%d, role:%s",
        hwnd, groupid, roletips[role]);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
    
    return ERRNO_SUCCESS;
}

