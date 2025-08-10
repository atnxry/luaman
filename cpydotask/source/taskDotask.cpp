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
#include "taskDotask.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "Markup.h"

#define FONTDOTASK_DIR "font\\Dotask\\"
#define BMPDOTASK_DIR "bmp\\Dotask\\"

#define CONFIG_DOTASK_XML  "conf\\dotask.xml"
struct dotaskconfig
{
    int bindwnd;
    int last_nr;
    char user_name[USRNAME_SIZE];
    char passwd[PASSWORD_SIZE];
    char passwdex[PASSWORD_SIZE];
};

static void dotaskprintconfig(struct dotaskconfig* config)
{
    api_log_MsgDebug("bindwnd     : %d", config->bindwnd);
    api_log_MsgDebug("last_nr     : %d", config->last_nr);
    api_log_MsgDebug("user_name   : %s", config->user_name);
    api_log_MsgDebug("passwd      : %s", config->passwd);
    api_log_MsgDebug("passwdex    : %s", config->passwdex);
    
    return;
}

#define KEY_dotask_Root         "Root"
#define KEY_dotask_Account      "Account"
#define KEY_dotask_Bindwnd      "Bindwnd"
#define KEY_dotask_Lastnr       "Lastnr"
#define KEY_dotask_Username     "Username"
#define KEY_dotask_Password     "Password"
#define KEY_dotask_Passwordex   "Passwordex"

static HANDLE dotasklock;

int api_TaskDotaskPrepare(void)
{
    dotasklock = CreateMutex(NULL, FALSE, NULL);
    if (!dotasklock)
    {
        api_log_MsgError("api_TaskDotaskPrepare failed");
        exit(1);
    }

    return 0;
}

static int dotaskcreatXML(std::string& file, struct dotaskconfig* config)
{
    CMarkup xml;
    xml.SetDoc(CString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"));
    xml.AddElem(CString(KEY_dotask_Root));

    xml.IntoElem();
    xml.AddElem(CString(KEY_dotask_Account));
    xml.IntoElem();
    
    ITEM_NEW_D(xml, KEY_dotask_Bindwnd, config->bindwnd);
    ITEM_NEW_D(xml, KEY_dotask_Lastnr, config->last_nr);
    
    ITEM_NEW(xml, KEY_dotask_Username, config->user_name);
    ITEM_NEW(xml, KEY_dotask_Password, config->passwd);
    ITEM_NEW(xml, KEY_dotask_Passwordex, config->passwdex);
    
    xml.OutOfElem();
    xml.OutOfElem();

    xml.Save(CString(file.c_str()));
    return ERRNO_SUCCESS;
}

static int __dotasksaveconfig(std::string& file, struct dotaskconfig* config)
{
    CMarkup xml;
    bool bok = false;
    int bindwnd = -1;
    
    if (!xml.Load(CString(file.c_str())))
    {
        api_log_MsgError("load %s failed when saveXML", file.c_str());
        return dotaskcreatXML(file, config);
    }

    xml.ResetMainPos();
    if (!xml.FindElem(CString(KEY_dotask_Root)))
    {
        return ERRNO_FAILED;
    }

    xml.IntoElem();
    while (xml.FindElem(CString(KEY_dotask_Account)))
    {
        xml.IntoElem();
        while (xml.FindElem(NULL))
        {
            CString tag = xml.GetTagName();
            CString attr = xml.GetAttrib(CString(ATTR_VALUE));

            if (CString(KEY_dotask_Bindwnd) == tag)
            {
                char* cbindwnd = attr.GetBuffer(BUFFER_MAX);
                bindwnd = atoi(cbindwnd);
                attr.ReleaseBuffer();

                if (bindwnd != config->bindwnd)
                {
                    break;
                }
                
                continue;
            }

            bok = true;
            
            if (CString(KEY_dotask_Lastnr) == tag)
            {
                ITEM_UPDATE_D(xml, config->last_nr);
                continue;
            }

            if (CString(KEY_dotask_Username) == tag)
            {
                ITEM_UPDATE(xml, config->user_name);
                continue;
            }

            if (CString(KEY_dotask_Password) == tag)
            {
                ITEM_UPDATE(xml, config->passwd);
                continue;
            }

            if (CString(KEY_dotask_Passwordex) == tag)
            {
                ITEM_UPDATE(xml, config->passwdex);
                continue;
            }
        }

        xml.OutOfElem();
        if (bok)
        {
            break;
        }
    }

    xml.OutOfElem();
    if (bok)
    {
        xml.Save(CString(file.c_str()));
        return ERRNO_SUCCESS;
    }

    {
        xml.IntoElem();
        xml.AddElem(CString(KEY_dotask_Account));
        xml.IntoElem();
        
        ITEM_NEW_D(xml, KEY_dotask_Bindwnd, config->bindwnd);
        ITEM_NEW_D(xml, KEY_dotask_Lastnr, config->last_nr);
        ITEM_NEW(xml, KEY_dotask_Username, config->user_name);
        ITEM_NEW(xml, KEY_dotask_Password, config->passwd);
        ITEM_NEW(xml, KEY_dotask_Passwordex, config->passwdex);
        
        xml.OutOfElem();
        xml.OutOfElem();

        xml.Save(CString(file.c_str()));
    }

    return ERRNO_SUCCESS;
}

static int dotasksaveconfig(std::string& file, struct dotaskconfig* config)
{
    int bok = 0;
    
    LUAMAN_LOCK(dotasklock);
    bok = __dotasksaveconfig(file, config);
    LUAMAN_UNLOCK(dotasklock);

    return bok;
}

static bool __dotaskloadconfig(int bindwnd, CMarkup& xml, struct dotaskconfig* config)
{
    bool bok = false;
    
    while (xml.FindElem(NULL))
    {
        CString tag = xml.GetTagName();
        CString attr = xml.GetAttrib(CString(ATTR_VALUE));

        /*char* ctag = tag.GetBuffer(BUFFER_MAX);
        api_log_MsgDebug("ctag   : %s", ctag);
        tag.ReleaseBuffer();
        
        char* ccattr = attr.GetBuffer(BUFFER_MAX);
        api_log_MsgDebug("ccattr : %s", ccattr);
        attr.ReleaseBuffer();*/
        
        if (CString(KEY_dotask_Bindwnd) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            if (bindwnd != atoi(cattr))
            {
                attr.ReleaseBuffer();
                break;
            }

            config->bindwnd = bindwnd;
            attr.ReleaseBuffer();
            continue;
        }

        bok = true;
        if (CString(KEY_dotask_Lastnr) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->last_nr = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->last_nr:%d", config->last_nr);
            continue;
        }

        if (CString(KEY_dotask_Username) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->user_name, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->user_name:%s", config->user_name);
            continue;
        }

        if (CString(KEY_dotask_Password) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->passwd, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->passwd:%s", config->passwd);
            continue;
        }

        if (CString(KEY_dotask_Passwordex) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->passwdex, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->passwdex:%s", config->passwdex);
            continue;
        }
    }

    return bok;
}

static bool _dotaskloadconfig(int bindwnd, std::string& file, struct dotaskconfig* config)
{
    bool bok = false;
    CMarkup xml;
    
    if (!xml.Load(CString(file.c_str())))
    {
        api_log_MsgError("LoadXML %s failed", file.c_str());
        return false;
    }

    xml.ResetMainPos();
    if (!xml.FindElem(CString(KEY_dotask_Root)))
    {
        api_log_MsgError("LoadXML %s failed, KEY_Root:%s", file.c_str(), KEY_dotask_Root);
        return false;
    }

    xml.IntoElem();
    while (xml.FindElem(CString(KEY_dotask_Account)))
    {
        xml.IntoElem();
        bok = __dotaskloadconfig(bindwnd, xml, config);
        xml.OutOfElem();
        
        if (bok)
        {
            break;
        }
    }
    xml.OutOfElem();

    if (true != bok)
    {
        api_log_MsgDebug("LoadXML %s failed", file.c_str());
        return false;
    }

    api_log_MsgDebug("LoadXML %s success", file.c_str());
    return true;
}

static bool dotaskloadconfig(int bindwnd, std::string& file, struct dotaskconfig* config)
{
    bool bok = false;

    memset(config, 0, sizeof(struct dotaskconfig));
    
    LUAMAN_LOCK(dotasklock);
    bok = _dotaskloadconfig(bindwnd, file, config);
    LUAMAN_UNLOCK(dotasklock);
    
    return bok;
}

int api_TaskDotaskInit(Idmsoft* dmobj, void* ctxt)
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
    
    api_log_MsgDebug("副本:Dotask:初始化");
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

static int dotask_checkkeystate(Idmsoft* dmobj, void* ctxt)
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

static int dotask_checklife(Idmsoft* dmobj, void* ctxt, int recusive)
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
    
    dmobj->SetDict(0, FONTDOTASK_DIR"font-blood.txt");
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

    dotask_checkkeystate(dmobj, ctxt);

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

        dmobj->SetDict(0, FONTDOTASK_DIR"font-chcklife.txt");
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

        dmobj->SetDict(0, FONTDOTASK_DIR"font-chcklife.txt");
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
        code = dotask_checklife(dmobj, ctxt, 1);
        if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("复活:站立:完成");
            break;
        }

        Delay_With_Terminate_Check(1, luaman_task, dmobj);
    }

    return SNAIL_ERRNO_REALIVE;
}

static int dotask_walk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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
                (void)dotask_checkkeystate(dmobj, ctxt);

                api_log_MsgDebug("寻路:坐标:卡住,停止寻路:等待副本消失,下一坐标, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //等待地洞副本消失
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //先检查下血条
            code = dotask_checklife(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //读取当前坐标
            dmobj->SetDict(0, FONTDOTASK_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)dotask_checkkeystate(dmobj, ctxt);
                
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

                (void)dotask_checkkeystate(dmobj, ctxt);
                
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
        code = dotask_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:卡住:血条:检查:死亡:复活");
            return code;
        }
        
        dmobj->SetDict(0, FONTDOTASK_DIR"font-map.txt");
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

static int dotask_clickhome(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = luaman_config->cleanPackageX;
    int code = 0;
    //VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    //long lposX = 0, lposY = 0;
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

    //api_log_MsgDebug("cleanPackageX:0x%x", cleanPackageX);
    
    //查找背包标签
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(300);
        dmobj->KeyUpChar("i");
        Delay(300);
        
        //检查是否按成功
        dmobj->SetDict(0, FONTDOTASK_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("背包:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

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

        api_log_MsgDebug("仓库:查找:成功");
        Delay(500);
    }

    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;
    
    api_log_MsgDebug("背包:宝物柜, lineX:%ld, lineY:%ld", lineX, lineY);

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

    api_log_MsgDebug("背包:宝物柜:准备:处理");

    //处理每个背包的所有宝物柜
    for (lndex = 0; lndex < 5; lndex++)
    //for (lndex = 0; lndex < 2; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        /*if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }*/

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

                api_log_MsgDebug("处理:背包:%d:柜子:[%d,%d]:坐标:[%ld, %ld]",
                    lndex, index, jndex, posX, posY);
                
                dmobj->SetDict(0, FONTDOTASK_DIR"font-home.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "返回卷轴", "ffffff-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX1 = pposX1.lVal;
                    lposY1 = pposY1.lVal;

                    Delay(200);
                    dmobj->RightClick();
                    Delay(200);

                    api_log_MsgDebug("返回卷轴:处理:成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);

                    return SNAIL_ERRNO_SUCCESS;
                }
            }
        }
    }

    return SNAIL_ERRNO_NOT_EXIST;
}


struct _targetpos
{
    long deltaX;
    long deltaY;
    long posX;
    long posY;
};

static int dotaskcopy(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    char* buddyName = luaman_config->buddyName;
    char tmpbuf[128] = {0};
    struct _targetpos pjhg_tgtpos[] =
    {
        //862:157 -> 388:525
        //{-474, 368, 346, 149},
        {-472, 368, 346, 149},
    };
    int pjhg_tgtpos_index = 0;//配置可选
    struct _targetpos hlgd_tgtpos[] =
    {
        //862:157 -> 251:432->254:429
        //{-611, 266, 252, 14},
        {-606, 272, 252, 14},
    };
    int hlgd_tgtpos_index = 0;//配置可选
    struct _targetpos swjd_tgtpos[] =
    {
        //{-408, 338, 317, 216}, //862:157 ->454:495   317:216 
        //{-441, 347, 322, 182}, //862:157 ->421:504   322:182 
        //{-453, 309, 283, 169}, //862:157 ->409:466   283:169 
        //{-165, 337, 313, 460}, //862:157 ->697:494   313:460 
        //{-192, 300, 275, 433}, //862:157 ->670:457   275:433 
        {-408, 338, 317, 211}, //862:157 ->454:495   317:216 
        {-441, 342, 321, 177}, //862:157 ->421:504   322:182 
        {-446, 309, 288, 173}, //862:157 ->409:466   283:169 
        {-163, 337, 316, 455}, //862:157 ->697:494   313:460 
        {-190, 297, 276, 429}, //862:157 ->670:457   275:433 
        {-192, 339, 316, 432}, //862:157 ->670:496   316:432 
    };
    int swjd_tgtpos_index = luaman_config->swjd_index;//配置可选
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    time_t startTalkTime = time(NULL);
    time_t curTime = 0;
    double talkTimeDelta = 0.0;
    time_t startWalkTime = time(NULL);
    double walkTimeDelta = 0.0;
    time_t startFindTime = time(NULL);
    double findTimeDelta = 0.0;
    int swjd_dir = 0;
    
    if (tasknr)
    {
        goto start_task;
    }
    
    //归位下
    pathofposxys = "299:112";
    pathOfPosXYs = CString(pathofposxys);
    code = dotask_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[299:112]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[299:112]:成功");

start_find:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("p");
        Delay(300);
        dmobj->KeyUpChar("p");
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-npc1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "寻找队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("寻找队伍:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("寻找队伍:查找:失败");
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
        Delay(200);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-search.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "搜索", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("搜索:查找:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("搜索:查找:失败");
        Delay(500);
    }

    //点击搜索
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-logo.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "队伍口号", "ffca40-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("队伍口号:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("队伍口号:查找:失败");
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    //查找"队伍" 660:332 -> 589:330~780:550
    (void)_snprintf(tmpbuf, sizeof(tmpbuf) - 1, "%s的队伍", buddyName);

    startFindTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        curTime = time(NULL);
        findTimeDelta = difftime(curTime, startFindTime);
        if (findTimeDelta > 10.0)
        {
            dmobj->KeyDownChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);

            dmobj->KeyDownChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);

            api_log_MsgDebug("%s:查找:失败:重新:搜索", tmpbuf);
            goto start_find;
        }
        
        dmobj->SetDict(0, FONTDOTASK_DIR"font-buddyname.txt");
        //code = dmobj->FindStrFast(lposX - 71, lposY - 2, lposX + 120, lposY + 218, buddyName, "ffffff-101010", 0.9, &pposX2, &pposY2);
        code = dmobj->FindStrFast(lposX - 71, lposY - 2, lposX + 120, lposY + 218, tmpbuf, "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX2.lVal;
            lposY = pposY2.lVal;

            api_log_MsgDebug("%s:查找:成功,lposX:%ld, lposY:%ld", tmpbuf, lposX, lposY);
            break;
        }

        api_log_MsgDebug("%s:查找:失败", tmpbuf);
        Delay(500);
    }

    //点击"申请" 593:371 -> 802:376
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->MoveTo(lposX + 209, lposY + 5);
        //dmobj->MoveTo(lposX + 209, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-setpassword.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "输入密码", "f4be3d-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("输入密码:查找:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("输入密码:查找:失败");
        Delay(500);
    }

    //输入密码 593:414 -> 575:461
    /*dmobj->MoveTo(lposX2 - 18, lposY2 + 47);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);*/

    code = dmobj->SendString(hwnd, "9999");
    if (1 != code)
    {
        api_log_MsgError("密码:9999:输入:失败");
        return ERRNO_IGNORE;
    }

    //点击确认 593:414 -> 580:500
    for (;;)
    {
        dmobj->MoveTo(lposX2 - 13, lposY2 + 86);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-setpassword.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "输入密码", "f4be3d-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("输入密码:确认:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("输入密码:确认:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //按p关掉窗口
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("p");
        Delay(300);
        dmobj->KeyUpChar("p");
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-exitgroup.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("关闭:窗口:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("关闭:窗口:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //按j跳到坐骑上
    dmobj->KeyDownChar("j");
    Delay(2000);
    dmobj->KeyUpChar("j");
    Delay(1000);
    Delay(4000);
    
start_task:
    //按n对话
    startTalkTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "战神的崛起之路", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("战神的崛起之路:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        curTime = time(NULL);
        talkTimeDelta = difftime(curTime, startTalkTime);
        if (talkTimeDelta > 10.0)
        {
            //按esc退出继续下一个号
            dmobj->KeyPressChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);

            api_log_MsgDebug("战神的崛起之路:查找:超时:退出:继续:下一个:账号");
            return SNAIL_ERRNO_EXIT;
        }
        
        api_log_MsgDebug("战神的崛起之路:查找:失败");
        Delay(500);
    }

    //点击"战神的崛起之路"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "交给我吧。", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("交给我吧。:查找:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("交给我吧。:查找:失败");
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
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "交给我吧。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("交给我吧。:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("交给我吧。:点击:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //按esc退出
    dmobj->KeyPressChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);
    Delay(1000);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-vip.txt");
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

    //贵宾服务 862:157 -> 388:525
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + pjhg_tgtpos[pjhg_tgtpos_index].deltaX, lposY + pjhg_tgtpos[pjhg_tgtpos_index].deltaY);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //检查是否到达目标坐标
        dmobj->SetDict(0, FONTDOTASK_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pjhg_tgtpos[pjhg_tgtpos_index].posX) <= 3 && abs(posY - pjhg_tgtpos[pjhg_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("目标:坐标:%ld:%ld:到达:成功",
                        pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("前往:目标:坐标:%ld:%ld:当前:坐标:%ld:%ld",
                    pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);
    }

    //按esc退出m
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-vip.txt");
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

    //归位
    pathofposxys = "347:144";
    pathOfPosXYs = CString(pathofposxys);
    code = dotask_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[347:144]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[347:144]:成功");

    //按n对话
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-npc3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[传送]想传送。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("[传送]想传送。:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Delay(500);
        api_log_MsgDebug("[传送]想传送。:查找:失败");
    }
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-npc3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "荒凉谷地:", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("荒凉谷地::查找:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Delay(500);
        api_log_MsgDebug("荒凉谷地::查找:失败");
    }

    //点击荒凉谷地:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-npc3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "荒凉谷地:", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("荒凉谷地::查找:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("荒凉谷地::查找:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //等待进入荒凉谷地
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
            
        dmobj->SetDict(0, FONTDOTASK_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "荒凉谷地", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:荒凉谷地:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:荒凉谷地:查找:失败");
        Delay(1000);
    }

    //按m
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-vip.txt");
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
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + hlgd_tgtpos[hlgd_tgtpos_index].deltaX, lposY + hlgd_tgtpos[hlgd_tgtpos_index].deltaY);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //检查是否到达目标坐标
        dmobj->SetDict(0, FONTDOTASK_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - hlgd_tgtpos[hlgd_tgtpos_index].posX) <= 3 && abs(posY - hlgd_tgtpos[hlgd_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("目标:坐标:%ld:%ld:到达:成功",
                        hlgd_tgtpos[hlgd_tgtpos_index].posX, hlgd_tgtpos[hlgd_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("前往:目标:坐标:%ld:%ld:当前:坐标:%ld:%ld",
                    hlgd_tgtpos[hlgd_tgtpos_index].posX, hlgd_tgtpos[hlgd_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);
    }

    pathofposxys = "260:17|273:14";
    pathOfPosXYs = CString(pathofposxys);
    code = dotask_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[273:14]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[273:14]:成功");

    //关掉m
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-vip.txt");
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

    dmobj->MiddleClick();
    Delay(1000);
    dmobj->MiddleClick();
    Delay(1000);
    dmobj->MoveTo(5, 5);
    Delay(200);

    //按s进入死亡绝地
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("s");
        Delay(300);
        dmobj->KeyUpChar("s");
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "荒凉谷地", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("地图:荒凉谷地:退出:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("地图:荒凉谷地:退出:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(1000);
    }

    //等待进入死亡绝地
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
            
        dmobj->SetDict(0, FONTDOTASK_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "死亡绝地", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:死亡绝地:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:死亡绝地:查找:失败");
        Delay(1000);
    }

    //按m
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-vip.txt");
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

    startWalkTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + swjd_tgtpos[swjd_tgtpos_index].deltaX, lposY + swjd_tgtpos[swjd_tgtpos_index].deltaY);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        curTime = time(NULL);
        walkTimeDelta = difftime(curTime, startWalkTime);
        if (walkTimeDelta > 30.0)
        {
            switch (swjd_dir % 4)
            {
            case 0:
                dmobj->KeyDownChar("a");
                LUAMAN_WAIT_MS(300);
                dmobj->KeyUpChar("a");
                LUAMAN_WAIT_MS(300);
                break;

            case 1:
                dmobj->KeyDownChar("s");
                LUAMAN_WAIT_MS(300);
                dmobj->KeyUpChar("s");
                LUAMAN_WAIT_MS(300);
                break;

            case 2:
                dmobj->KeyDownChar("w");
                LUAMAN_WAIT_MS(300);
                dmobj->KeyUpChar("w");
                LUAMAN_WAIT_MS(300);
                break;

            case 3:
                dmobj->KeyDownChar("d");
                LUAMAN_WAIT_MS(300);
                dmobj->KeyUpChar("d");
                LUAMAN_WAIT_MS(300);
                break;
            default:
                break;
            }

            swjd_dir++;
            swjd_dir %= 4;
            
            api_log_MsgDebug("前往:目标:坐标:调整:方向");

            startWalkTime = time(NULL);
        }

        //检查是否到达目标坐标
        dmobj->SetDict(0, FONTDOTASK_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - swjd_tgtpos[swjd_tgtpos_index].posX) <= 3 && abs(posY - swjd_tgtpos[swjd_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("目标:坐标:%ld:%ld:到达:成功",
                        swjd_tgtpos[swjd_tgtpos_index].posX, swjd_tgtpos[swjd_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("前往:目标:坐标:%ld:%ld:当前:坐标:%ld:%ld",
                    swjd_tgtpos[hlgd_tgtpos_index].posX, swjd_tgtpos[hlgd_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);
    }

    //关掉m
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-vip.txt");
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

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-buddynamewalk.txt");
        code = dmobj->FindStrFast(1038, 243, 1276, 448, buddyName, "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("同行:%s:查找:成功,lposX:%ld, lposY:%ld", buddyName, lposX, lposY);
            break;
        }

        api_log_MsgDebug("同行:%s:查找:失败", buddyName);
        Delay(500);
    }

    //点击"同行" 1110:271 -> 1254:276
    dmobj->MoveTo(lposX + 144, lposY + 5);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    
    
    //等待红色"完成"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-finish.txt");
        code = dmobj->FindStrFast(6, 168, 200, 260, "完成", "ff8000-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("完成:查找:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("完成:查找:失败");
        Delay(500);
    }

    code = dotask_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("返回:卷轴:处理:失败,code:%d", code);
        return code;
    }

    //等待回到平静河谷
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
            
        dmobj->SetDict(0, FONTDOTASK_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "平静河谷", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:平静河谷:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:平静河谷:查找:失败");
        Delay(1000);
    }

    pathofposxys = "341:137|328:136|314:126|304:119|299:112";
    pathOfPosXYs = CString(pathofposxys);
    code = dotask_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[299:112]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[299:112]:成功");

    //按n对话
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "战神的崛起之路", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("战神的崛起之路:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("战神的崛起之路:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "谢谢。", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("谢谢。:查找:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("谢谢。:查找:失败");
        Delay(500);
    }

    //点击"谢谢"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTDOTASK_DIR"font-npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "谢谢。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("谢谢。:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("谢谢。:点击:失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    return SNAIL_ERRNO_SUCCESS;
}

static int dotaskwork(struct cluamanaccountinfo* accountinfo, Idmsoft* dmobj, void* ctxt)
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
        {'0', BMPDOTASK_DIR"0.bmp"},
        {'1', BMPDOTASK_DIR"1.bmp"},
        {'2', BMPDOTASK_DIR"2.bmp"},
        {'3', BMPDOTASK_DIR"3.bmp"},
        {'4', BMPDOTASK_DIR"4.bmp"},
        {'5', BMPDOTASK_DIR"5.bmp"},
        {'6', BMPDOTASK_DIR"6.bmp"},
        {'7', BMPDOTASK_DIR"7.bmp"},
        {'8', BMPDOTASK_DIR"8.bmp"},
        {'9', BMPDOTASK_DIR"9.bmp"},
    };
    int j = 0;
    char* path = NULL;
    int dotask_num = 0;
    
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
        
        dmobj->SetDict(0, FONTDOTASK_DIR"font-login.txt");
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
    
        dmobj->SetDict(0, FONTDOTASK_DIR"font-login.txt");
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
        
        dmobj->SetDict(0, FONTDOTASK_DIR"font-security.txt");
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
        dmobj->SetDict(0, FONTDOTASK_DIR"font-loginerr.txt");
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

                dmobj->SetDict(0, FONTDOTASK_DIR"font-loginerr.txt");
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
            
            dmobj->SetDict(0, FONTDOTASK_DIR"font-security.txt");
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
    
        dmobj->SetDict(0, FONTDOTASK_DIR"font-security.txt");
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
            
            dmobj->SetDict(0, FONTDOTASK_DIR"font-map.txt");
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
    dotask_num = 0;
    
    //执行“任务副本”
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = dotaskcopy(dmobj, ctxt, dotask_num);
        if (SNAIL_ERRNO_EXIT == code)
        {
            api_log_MsgDebug("副本:任务:执行:超时,dotask_num:%d, code:%d",
                dotask_num + 1, code);
            break;
        }

        dotask_num++;
        api_log_MsgDebug("副本:任务:继续,dotask_num:%d", dotask_num);

        if (3 == dotask_num)
        {
            api_log_MsgDebug("副本:任务:完成:继续:下一个:账户");
            break;
        }
    }

    api_log_MsgDebug("副本:任务:完成,dotask_num:%d", dotask_num);

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

        dmobj->SetDict(0, FONTDOTASK_DIR"font-logout.txt");
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

        dmobj->SetDict(0, FONTDOTASK_DIR"font-saveconf.txt");
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
        
        dmobj->SetDict(0, FONTDOTASK_DIR"font-saveconf.txt");
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

static int TaskDotaskMainLoop(struct list_head* accountList, Idmsoft* dmobj, void* ctxt)
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

    struct dotaskconfig dotask_config;
    int lastnr = 0;
    int bok = dotaskloadconfig(bindWnd, std::string(CONFIG_DOTASK_XML), &dotask_config);

    dotaskprintconfig(&dotask_config);

    list_for_each_entry_safe(apos, an, accountList, node, struct cluamanaccountinfo)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        if (dotask_config.last_nr > lastnr)
        {
            lastnr++;
            continue;
        }
        
        status = dotaskwork(apos, dmobj, ctxt);
        if (ERRNO_SUCCESS != status)
        {
            api_log_MsgError("副本:任务:%s:失败, status:%d", apos->user_name, status);
        }

        lastnr++;

        dotask_config.last_nr = lastnr;
        memcpy(dotask_config.user_name, apos->user_name, strlen(apos->user_name) + 1);
        memcpy(dotask_config.passwd, apos->passwd, strlen(apos->passwd) + 1);
        memcpy(dotask_config.passwdex, apos->passwdex, strlen(apos->passwdex) + 1);
        
        (void)dotasksaveconfig(std::string(CONFIG_DOTASK_XML), &dotask_config);
    }

    //循环复原
    apos = list_first_entry(accountList, struct cluamanaccountinfo, node);
    dotask_config.last_nr = 0;
    memcpy(dotask_config.user_name, apos->user_name, strlen(apos->user_name) + 1);
    memcpy(dotask_config.passwd, apos->passwd, strlen(apos->passwd) + 1);
    memcpy(dotask_config.passwdex, apos->passwdex, strlen(apos->passwdex) + 1);
    dotasksaveconfig(std::string(CONFIG_DOTASK_XML), &dotask_config);
    
    return ERRNO_SUCCESS;
}

int api_TaskDotaskMain(Idmsoft* dmobj, void* ctxt)
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
    
    api_log_MsgDebug("副本:Dotask:主循环");
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

        return TaskDotaskMainLoop(&bpos->accountList, dmobj, ctxt);
    }

    return ERRNO_SUCCESS;
}

int api_TaskDotaskExit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int bindWnd = luaman_config->curBindWnd;
    
    api_log_MsgDebug("副本:Dotask:退出");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
        
    return ERRNO_SUCCESS;
}

