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
#include "taskSignle.h"
#include "taskSignleCpy1.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "Markup.h"
#include "curl.h"

#define CHECK_CONN_LOST_TIME    (5)
#define FONTSIGNLE_DIR "font\\signle\\"
#define BMPSIGNLE_DIR "bmp\\signle\\"

#define CONFIG_SIGNLE_XML  "conf\\signle.xml"
struct signleconfig
{
    int bindwnd;
    int last_nr;
    char user_name[USRNAME_SIZE];
    char passwd[PASSWORD_SIZE];
    char passwdex[PASSWORD_SIZE];
};

struct gabage_info
{
    const char* name;
    const char* color_offset;
};

///////////////////////////////////////////////////////////////////////////////
//清包配置,每增加1种垃圾,就增加一行
//!!! '杂货商店'加入到字库文件font-clrpkg1.txt中
//!!!重要!!!:所有垃圾的名称加入到字库文件font-clrpkg2.txt中
//!!! 将字库文件font-clrpkg1.txt和font-clrpkg2.txt都放到product/font/signle/下
static gabage_info usrdef_gabage_info[] = 
{
    //{"垃圾1", "00ff00-010101"}, //一行表示一种垃圾
    //{"垃圾2", "00ff00-010101"},
    //TODO:参考垃圾1、垃圾2写,'//'表示注释
    {"要求职业", "ffffff-010101"},
    {"尊严腰带", "ffffff-010101"},
    {"尊严腰带", "c1c1ff-010101"},
    {"尊严腰带", "ffe08c-010101"},
    
    
};
///////////////////////////////////////////////////////////////////////////////

static void signleprintconfig(struct signleconfig* config)
{
    api_log_MsgDebug("bindwnd     : %d", config->bindwnd);
    api_log_MsgDebug("last_nr     : %d", config->last_nr);
    api_log_MsgDebug("user_name   : %s", config->user_name);
    api_log_MsgDebug("passwd      : %s", config->passwd);
    api_log_MsgDebug("passwdex    : %s", config->passwdex);
    
    return;
}

#define KEY_signle_Root         "Root"
#define KEY_signle_Account      "Account"
#define KEY_signle_Bindwnd      "Bindwnd"
#define KEY_signle_Lastnr       "Lastnr"
#define KEY_signle_Username     "Username"
#define KEY_signle_Password     "Password"
#define KEY_signle_Passwordex   "Passwordex"

static HANDLE signlelock;

int api_TaskSignlePrepare(void)
{
    signlelock = CreateMutex(NULL, FALSE, NULL);
    if (!signlelock)
    {
        api_log_MsgError("api_TaskSignlePrepare failed");
        exit(1);
    }

    api_log_MsgDebug("api_TaskSignlePrepare success");
    return 0;
}

static void signleEnterCriticalSection(void)
{
    ::WaitForSingleObject(signlelock, INFINITE);
    api_log_MsgDebug("signleEnterCriticalSection::加锁:成功");
    return;
}

static void signleLeaveCriticalSection(void)
{
    (void)::ReleaseMutex(signlelock);
    api_log_MsgDebug("signleEnterCriticalSection::解锁:成功");
    return;
}

static void signlelockonoff(int flag)
{
    if (flag)
    {
        signleEnterCriticalSection();
        return;
    }

    signleLeaveCriticalSection();
    return;
}

static int signlecreatXML(std::string& file, struct signleconfig* config)
{
    CMarkup xml;
    xml.SetDoc(CString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"));
    xml.AddElem(CString(KEY_signle_Root));

    xml.IntoElem();
    xml.AddElem(CString(KEY_signle_Account));
    xml.IntoElem();
    
    ITEM_NEW_D(xml, KEY_signle_Bindwnd, config->bindwnd);
    ITEM_NEW_D(xml, KEY_signle_Lastnr, config->last_nr);
    
    ITEM_NEW(xml, KEY_signle_Username, config->user_name);
    ITEM_NEW(xml, KEY_signle_Password, config->passwd);
    ITEM_NEW(xml, KEY_signle_Passwordex, config->passwdex);
    
    xml.OutOfElem();
    xml.OutOfElem();

    xml.Save(CString(file.c_str()));
    return ERRNO_SUCCESS;
}

static int __signlesaveconfig(std::string& file, struct signleconfig* config)
{
    CMarkup xml;
    bool bok = false;
    int bindwnd = -1;
    
    if (!xml.Load(CString(file.c_str())))
    {
        api_log_MsgError("load %s failed when saveXML", file.c_str());
        return signlecreatXML(file, config);
    }

    xml.ResetMainPos();
    if (!xml.FindElem(CString(KEY_signle_Root)))
    {
        return ERRNO_FAILED;
    }

    xml.IntoElem();
    while (xml.FindElem(CString(KEY_signle_Account)))
    {
        xml.IntoElem();
        while (xml.FindElem(NULL))
        {
            CString tag = xml.GetTagName();
            CString attr = xml.GetAttrib(CString(ATTR_VALUE));

            if (CString(KEY_signle_Bindwnd) == tag)
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
            
            if (CString(KEY_signle_Lastnr) == tag)
            {
                ITEM_UPDATE_D(xml, config->last_nr);
                continue;
            }

            if (CString(KEY_signle_Username) == tag)
            {
                ITEM_UPDATE(xml, config->user_name);
                continue;
            }

            if (CString(KEY_signle_Password) == tag)
            {
                ITEM_UPDATE(xml, config->passwd);
                continue;
            }

            if (CString(KEY_signle_Passwordex) == tag)
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
        xml.AddElem(CString(KEY_signle_Account));
        xml.IntoElem();
        
        ITEM_NEW_D(xml, KEY_signle_Bindwnd, config->bindwnd);
        ITEM_NEW_D(xml, KEY_signle_Lastnr, config->last_nr);
        ITEM_NEW(xml, KEY_signle_Username, config->user_name);
        ITEM_NEW(xml, KEY_signle_Password, config->passwd);
        ITEM_NEW(xml, KEY_signle_Passwordex, config->passwdex);
        
        xml.OutOfElem();
        xml.OutOfElem();

        xml.Save(CString(file.c_str()));
    }

    return ERRNO_SUCCESS;
}

static int signlesaveconfig(std::string& file, struct signleconfig* config)
{
    int bok = 0;
    
    LUAMAN_LOCK(signlelock);
    bok = __signlesaveconfig(file, config);
    LUAMAN_UNLOCK(signlelock);

    return bok;
}

static bool __signleloadconfig(int bindwnd, CMarkup& xml, struct signleconfig* config)
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
        
        if (CString(KEY_signle_Bindwnd) == tag)
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
        if (CString(KEY_signle_Lastnr) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->last_nr = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->last_nr:%d", config->last_nr);
            continue;
        }

        if (CString(KEY_signle_Username) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->user_name, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->user_name:%s", config->user_name);
            continue;
        }

        if (CString(KEY_signle_Password) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->passwd, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->passwd:%s", config->passwd);
            continue;
        }

        if (CString(KEY_signle_Passwordex) == tag)
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

static bool _signleloadconfig(int bindwnd, std::string& file, struct signleconfig* config)
{
    bool bok = false;
    CMarkup xml;
    
    if (!xml.Load(CString(file.c_str())))
    {
        api_log_MsgError("LoadXML %s failed", file.c_str());
        return false;
    }

    xml.ResetMainPos();
    if (!xml.FindElem(CString(KEY_signle_Root)))
    {
        api_log_MsgError("LoadXML %s failed, KEY_Root:%s", file.c_str(), KEY_signle_Root);
        return false;
    }

    xml.IntoElem();
    while (xml.FindElem(CString(KEY_signle_Account)))
    {
        xml.IntoElem();
        bok = __signleloadconfig(bindwnd, xml, config);
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

static bool signleloadconfig(int bindwnd, std::string& file, struct signleconfig* config)
{
    bool bok = false;

    memset(config, 0, sizeof(struct signleconfig));
    
    LUAMAN_LOCK(signlelock);
    bok = _signleloadconfig(bindwnd, file, config);
    LUAMAN_UNLOCK(signlelock);
    
    return bok;
}

int api_TaskSignleInit(Idmsoft* dmobj, void* ctxt)
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
    int curBindHostIdx = luaman_config->curBindHostIdx;
    
    api_log_MsgDebug("副本:Signle:初始化");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);
    api_log_MsgDebug("curBindHostIdx  : %d", curBindHostIdx);
    
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

    /* 初始化告警时间初始值 */
    init_alarm_ctxt();
    
    return ERRNO_SUCCESS;
}

static int signle_checkkeystate(Idmsoft* dmobj, void* ctxt)
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

static int signle_checklife(Idmsoft* dmobj, void* ctxt, int recusive)
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

    {
        return ERRNO_SUCCESS;
    }
    
    dmobj->SetDict(0, FONTSIGNLE_DIR"font-blood.txt");
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

    signle_checkkeystate(dmobj, ctxt);

    if (recusive)
    {
        return SNAIL_ERRNO_AGAIN;
    }
    
    //等待出现死亡字样
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-chcklife.txt");
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
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-chcklife.txt");
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
        code = signle_checklife(dmobj, ctxt, 1);
        if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("复活:站立:完成");
            break;
        }

        Delay_With_Terminate_Check(1, luaman_task, dmobj);
    }

    return SNAIL_ERRNO_REALIVE;
}

static int signle_walk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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
                (void)signle_checkkeystate(dmobj, ctxt);

                api_log_MsgDebug("寻路:坐标:卡住,停止寻路:等待副本消失,下一坐标, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //等待地洞副本消失
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //先检查下血条
            code = signle_checklife(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //读取当前坐标
            dmobj->SetDict(0, FONTSIGNLE_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)signle_checkkeystate(dmobj, ctxt);
                
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

                (void)signle_checkkeystate(dmobj, ctxt);
                
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
        code = signle_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:卡住:血条:检查:死亡:复活");
            return code;
        }
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
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

static int signlecopy1_pre_join2team(Idmsoft* dmobj, void* ctxt)
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
    
    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    lastChckTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("f");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f");
        LUAMAN_WAIT_MS(200);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-inviteheil2team-1.txt");
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
        check_alarm(dmobj, ctxt);
        
        //检查血条
        code = signle_checklife(dmobj, ctxt, 0);
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

    lastChckTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-inviteheil2team-1.txt");
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

        //检查血条
        code = signle_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("组队:邀请:频道:找字:死亡:复活");
            return code;
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
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            posX = fellowPosXYs[i].x;
            posY = fellowPosXYs[i].y;
            
            api_log_MsgDebug("i:%d, posX:%ld, posY:%ld", i, posX, posY);

            lastChckTime = time(NULL);

            //点击队员名称,弹出队伍对话框
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                dmobj->MoveTo(posX, posY);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);

                dmobj->SetDict(0, FONTSIGNLE_DIR"font-inviteheil2team-1.txt");
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
                code = signle_checklife(dmobj, ctxt, 0);
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
            check_alarm(dmobj, ctxt);
        }

        syncCount = 0;

        //@bug:稍等片刻,避免二次组队浪费时间2019/4/8 23:06:19
        Delay(2000);

        api_log_MsgDebug("同行:个数:检查");
        
        //检查'同行'个数
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            posX1 = syncPosXYs[2 * i].x;
            posY1 = syncPosXYs[2 * i].y;
            posX2 = syncPosXYs[2 * i + 1].x;
            posY2 = syncPosXYs[2 * i + 1].y;

            api_log_MsgDebug("(%ld, %ld) --> (%ld, %ld)", posX1, posY1, posX2, posY2);

            dmobj->SetDict(0, FONTSIGNLE_DIR"font-inviteheil2team-1.txt");
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
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("组队:邀请:队员:所有,组队未完成,syncCount:%d", syncCount);
    }

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    api_log_MsgDebug("组队完成:成功:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int signlecopy1_pre(Idmsoft* dmobj, void* ctxt)
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
    
    dmobj->KeyDownChar("f2");
    Delay(300);
    dmobj->KeyUpChar("f2");
    Delay(300);

    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "349:129|353:118|355:112|359:102|368:101|375:101|385:101|396:98|406:94|415:90|424:89|431:88|437:89|445:88|451:88|460:89|467:89";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[467:89]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[467:89]:成功");

    dmobj->KeyDownChar("s");
    Delay(1000);
    dmobj->KeyUpChar("s");
    Delay(300);

    //检查地图是不是变成大型竞技场广场
    i = 0;
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "大型竞技场广场", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:大型竞技场广场:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        i++;
        if (i == 10)
        {
            dmobj->KeyDownChar("d");
            Delay(300);
            dmobj->KeyUpChar("d");
            Delay(300);

            i = 0;
            api_log_MsgDebug("地图:大型竞技场广场:查找:d键:成功");
        }
        
        api_log_MsgDebug("地图:大型竞技场广场:查找:失败");
        Delay(1000);
    }

    i = 0;
    
    pathofposxys = "370:256|360:256|351:256|341:256|334:256|324:252";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[324:252]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[324:252]:成功");

    dmobj->KeyDownChar("=");
    Delay(1000);
    dmobj->KeyUpChar("=");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyDownChar("0");
    Delay(1000);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check(2, luaman_task, dmobj);

    pathofposxys = "314:245|308:237|307:227|307:221|307:210";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[307:210]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[307:210]:成功");

    return SNAIL_ERRNO_SUCCESS;
}

static int signlecopy1_exitgroup(Idmsoft* dmobj, void* ctxt)
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
    int retrynr = 0;
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("退出队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        retrynr++;
        if (retrynr > 10)
        {
            api_log_MsgDebug("退出队伍:查找:失败:超时");
            return ERRNO_SUCCESS;
        }
        
        api_log_MsgDebug("退出队伍:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX + 5, lposY + 5);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("确认:查找:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Delay(500);
        api_log_MsgDebug("确认:查找:失败");
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-auxiend.txt");
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
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ESC:退出队伍:查找:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ESC:退出队伍:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    return ERRNO_SUCCESS;
}

static int signle_clickhome(Idmsoft* dmobj, void* ctxt)
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
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(300);
        dmobj->KeyUpChar("i");
        Delay(300);
        
        //检查是否按成功
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-package.txt");
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

    api_log_MsgDebug("背包:宝物柜:准备:处理");

    //处理每个背包的所有宝物柜
    for (lndex = 0; lndex < 5; lndex++)
    //for (lndex = 0; lndex < 2; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
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
            check_alarm(dmobj, ctxt);
            for (jndex = 0; jndex < 10; jndex++)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                posX = boxPosXYs[index * 10 + jndex].x;
                posY = boxPosXYs[index * 10 + jndex].y;

                dmobj->MoveTo(posX, posY);
                Delay(200);

                api_log_MsgDebug("处理:背包:%d:柜子:[%d,%d]:坐标:[%ld, %ld]",
                    lndex, index, jndex, posX, posY);
                
                dmobj->SetDict(0, FONTSIGNLE_DIR"font-home.txt");
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

////////////////////////////////////////////////////////////////////////////////////////////////
//开关控制区:打开则去掉前面的注释,关闭则注释

//愚人节副本,打开了愚人节副本就不能打开1~6段;反之亦然
//#define SINGLE_FOOLISH_DAY

#ifndef SINGLE_FOOLISH_DAY
    //#define SIGNLE_SEGMENT12_ONLY
    //#define SIGNLE_SEGMENT3
    //#define SIGNLE_SEGMENT124
    //#define SIGNLE_SEGMENT12345
    //#define SIGNLE_SEGMENT45
    //#define SIGNLE_SEGMENT123456
    //#define SIGNLE_SEGMENT6
    //#define SIGNLE_SEGMENT7
    //#define SIGNLE_SEGMENT1247
    //#define SIGNLE_SEGMENT123457
    //#define SIGNLE_SEGMENT1234567
    //#define SIGNLE_SEGMENT345
    //#define SIGNLE_SEGMENT3457
    //#define SIGNLE_SEGMENT457
    //#define SIGNLE_SEGMENT347
    //#define SIGNLE_SEGMENT12347
    //#define SIGNLE_SEGMENT12
    #define SIGNLE_SEGMENT123
    //#define SIGNLE_SEGMENT5
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
//!!! 以下的内容不要动
#ifdef SIGNLE_SEGMENT12_ONLY
    #define SIGNLE_SEGMENT12
    #define LARGE_SPORT_SQUARE
#endif

#if defined(SIGNLE_SEGMENT123)
    #define SIGNLE_SEGMENT12
    #define SIGNLE_SEGMENT3
#endif

#if defined(SIGNLE_SEGMENT124) || defined(SIGNLE_SEGMENT1247)
    #define SIGNLE_SEGMENT12
    #define SIGNLE_SEGMENT4
    #define SIGNLE_SEGMENT12345_124
#endif

#ifdef SIGNLE_SEGMENT3457
    #define SIGNLE_SEGMENT345
#endif

#ifdef SIGNLE_SEGMENT345
    #define SIGNLE_SEGMENT3
    #define SIGNLE_SEGMENT45
#endif

#ifdef SIGNLE_SEGMENT12347
    #define SIGNLE_SEGMENT12
    #define SIGNLE_SEGMENT347
#endif

#ifdef SIGNLE_SEGMENT347
    #define SIGNLE_SEGMENT3
    #define SIGNLE_SEGMENT4
    #define SIGNLE_SEGMENT7
#endif

#ifdef SIGNLE_SEGMENT457
    #define SIGNLE_SEGMENT45
#endif

#ifdef SIGNLE_SEGMENT45
    #define SIGNLE_SEGMENT4
    #define SIGNLE_SEGMENT5
#endif

#if defined(SIGNLE_SEGMENT123456) || defined(SIGNLE_SEGMENT1234567)
    #define SIGNLE_SEGMENT12345
    #define SIGNLE_SEGMENT6
#endif

#if defined(SIGNLE_SEGMENT12345) || defined(SIGNLE_SEGMENT123457)
    #define SIGNLE_SEGMENT12
    #define SIGNLE_SEGMENT12345_124
    #define SIGNLE_SEGMENT3
    #define SIGNLE_SEGMENT4
    #define SIGNLE_SEGMENT5
#endif

#if defined(SIGNLE_SEGMENT457) || defined(SIGNLE_SEGMENT3457) || defined(SIGNLE_SEGMENT1247) || defined(SIGNLE_SEGMENT123457) || defined(SIGNLE_SEGMENT1234567)
    #define SIGNLE_SEGMENT7
#endif

static int signleBuyBlue(Idmsoft* dmobj, void* ctxt)
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

    dmobj->MoveTo(5, 5);
    Delay(500);
        
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //点击之后出现"商店"字样
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-shop.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX1 + 160, lposY1 + 74);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-okcancel.txt");
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
    code = dmobj->SendString(hwnd, "600");
    if (1 != code)
    {
        api_log_MsgError("商店:金额:600:输入:失败");
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("密码:金额:600:输入:成功");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-okcancel.txt");
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

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    api_log_MsgDebug("买蓝:完毕");
    return SNAIL_ERRNO_SUCCESS;
}


static int signlecopy_sellgabage(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = 0x6;//luaman_config->cleanPackageX;//默认清理1,2包
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
    int idx = 0;
    
    api_log_MsgDebug("清包:开始");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-package.txt");
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

        api_log_MsgDebug("背包:查找:失败");
        Delay(500);
    }

    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;

    api_log_MsgDebug("背包:宝物柜, lineX:%ld, lineY:%ld", lineX, lineY);

    for (index = 0; index < 5; index++)
    {
        posY = lineY + index * deltaY;
        
        for (jndex = 0; jndex < 10; jndex++)
        {
            posX = lineX + jndex * deltaX;
            
            boxPosXYs[index * 10 + jndex].x = posX;
            boxPosXYs[index * 10 + jndex].y = posY;

            //api_log_MsgDebug("boxPosXYs[%d][%d] = [%ld, %ld]", index, jndex, posX, posY);
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

                for (idx = 0; idx < sizeof(usrdef_gabage_info)/sizeof(usrdef_gabage_info[0]); idx++)
                {
                    dmobj->SetDict(0, FONTSIGNLE_DIR"font-clrpkg2.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, usrdef_gabage_info[idx].name, usrdef_gabage_info[idx].color_offset, 0.9, &pposX1, &pposY1);
                    dmobj->ClearDict(0);
                    if (0 != code)
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

                    api_log_MsgDebug("清包:背包:%d:柜子:[%d:%d]:垃圾:%d:名称:%s:颜色:%s",
                        lndex, index, jndex, idx,
                        usrdef_gabage_info[idx].name, usrdef_gabage_info[idx].color_offset);
                    break;
                }
            }
        }
    }

    //买蓝 2023/5/17 22:00:00
    (void)signleBuyBlue(dmobj, ctxt);
    
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
    
    api_log_MsgDebug("清包:完毕");
    return SNAIL_ERRNO_SUCCESS;
}


static int signlecopy_cleanpackage(Idmsoft* dmobj, void* ctxt)
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
    
    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "349:129|353:118|355:112|359:102|368:101|375:101|385:101|396:98|406:94|415:90|424:89|431:88|437:89|445:88|451:88|460:89|467:89";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[467:89]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[467:89]:成功");

    dmobj->KeyDownChar("s");
    Delay(1000);
    dmobj->KeyUpChar("s");
    Delay(300);

    //检查地图是不是变成大型竞技场广场
    i = 0;
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "大型竞技场广场", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:大型竞技场广场:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        i++;
        if (i == 10)
        {
            dmobj->KeyDownChar("d");
            Delay(300);
            dmobj->KeyUpChar("d");
            Delay(300);

            i = 0;
            api_log_MsgDebug("地图:大型竞技场广场:查找:d键:成功");
        }
        
        api_log_MsgDebug("地图:大型竞技场广场:查找:失败");
        Delay(1000);
    }

    i = 0;
    
    pathofposxys = "370:256|360:256|351:256|341:256|334:256|324:252|317:255|306:255|296:255|288:255|280:255|271:255|262:256|252:255|245:255|237:256|229:256|221:256|221:264|221:267|220:273|219:278|216:283|217:287";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[217:287]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[217:287]:成功");
    
    //按n
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-clrpkg1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "杂货商店", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("杂货商店:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("杂货商店:查找:失败");
        Delay(500);
    }
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-clrpkg1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "杂货商店", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("杂货商店:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
        
        api_log_MsgDebug("杂货商店:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    return signlecopy_sellgabage(dmobj, ctxt);
}

static int signlecopy(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    int tasknrref = 0;
    char* randtime = NULL;

#ifdef SINGLE_FOOLISH_DAY
    /*
      愚人节副本
      1.应该单独弄一个副本的,为了快速完成并应用,直接通过宏的方式放进来
      2.编译的时候直接编一个与之前1~6段区别开来的二进制程序版本 2023/4/1 10:04:11
    */
    tasknrref = 0;
    for (;;)
    {
        status = signle_copy_foolish(dmobj, ctxt, tasknrref);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("副本:愚人节:任务:完成:tasknrref:%d", tasknrref);
            return SNAIL_ERRNO_SUCCESS;
        }

        api_log_MsgDebug("副本:愚人节:任务:继续:tasknrref:%d", tasknrref);
        Delay(500);
    }
#endif

    dmobj->KeyDownChar("f2");
    Delay(500);
    dmobj->KeyUpChar("f2");
    Delay(500);

#ifdef SIGNLE_SEGMENT12
    //#ifdef SIGNLE_SEGMENT12345_124
        status = signlecopy1_pre(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != status)
        {
            api_log_MsgError("副本1:前往:失败:status:%d", status);
            return status;
        }

        api_log_MsgDebug("副本1:前往:成功");
    //#endif
    
    for (;;)
    {
        //组队
        /*
        #ifdef SIGNLE_SEGMENT12345
            code = signlecopy1_pre_join2team(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
            {
                api_log_MsgDebug("副本1:组队:死亡:复活");
                return SNAIL_ERRNO_REALIVE;
            }

            api_log_MsgDebug("副本1:组队:成功");
        #endif
        */
        
        api_log_MsgDebug("副本1:任务:开始:tasknrref:%d", tasknrref);
        
        status = signle_copy1(dmobj, ctxt, tasknr);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("副本1:任务:完成:继续:下一个:副本:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("副本1:任务:继续:tasknrref:%d", tasknrref);
    }
    
    status = signlecopy1_exitgroup(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgError("副本1:退组:失败:status:%d", status);
        return status;
    }

    api_log_MsgDebug("副本1:退组:成功");

    tasknrref = 0;
    for (;;)
    {
        status = signle_copy2(dmobj, ctxt, tasknr);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("副本2:任务:完成:继续:下一个:副本:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("副本2:任务:继续:tasknrref:%d", tasknrref);
    }
    
    //#ifdef SIGNLE_SEGMENT12345_124
        status = signle_clickhome(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != status)
        {
            api_log_MsgError("副本2:回城:失败:status:%d", status);
            return status;
        }

        api_log_MsgDebug("副本2:回城:成功");

        //等待回到平静河谷
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            dmobj->MoveTo(5, 5);
            Delay(200);
        
            //检查是否进入“平静河谷”
            Delay(1000);
            
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "平静河谷", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("回城:地图:平静河谷:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("回城:地图:平静河谷:查找:失败");
        }
    //#endif
#endif

#ifdef SIGNLE_SEGMENT3
    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "349:129|353:118|355:112|359:102|368:101|375:101|385:101|396:98|406:94|415:90|424:89|431:88|437:89|445:88|451:88|451:97|450:107|450:115";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[450:115]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[450:115]:成功");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-talk2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "净化地区", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("净化地区:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("净化地区:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-talk2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "净化地区", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("净化地区:查找:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("净化地区:查找:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        
        Delay(500);
    }

    //等待地图
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "阿尔卡迪亚", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:阿尔卡迪亚:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:阿尔卡迪亚:查找:失败");
        Delay(1000);
    }

    //步骤11:按M键 
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

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-vip.txt");
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

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //994:114 -> 602:273
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 392, lposY + 159);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //检查是否到达目标坐标
        /*dmobj->SetDict(0, FONTSIGNLE_DIR"font-posxy.txt");
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

        Delay(500);*/
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyPressChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);

    Delay_With_Terminate_Check(2, luaman_task, dmobj);

    pathofposxys = "139:236|138:248|137:257|125:257|114:257|105:257|96:257|88:257";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[88:257]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[88:257]:成功");

    (void)common_exitgroup(dmobj, ctxt);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[DAS]移动到DK服务器", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("[DAS]移动到DK服务器:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("[DAS]移动到DK服务器:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);

        //地图变成香格里拉就对了
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "香格里拉", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("地图:香格里拉:进入:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }
        
        randtime = ((time(NULL)) % 2)? "DK服务器1" : "DK服务器2";
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, randtime, "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("%s:点击:成功", randtime);
            continue;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        lposX = lposX2;
        lposY = lposY2;
        
        api_log_MsgDebug("%s:点击:失败, lposX2:%ld, lposY2:%ld", randtime, lposX2, lposY2);
        
        Delay(500);
    }

    //等待进入香格里拉地图
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "香格里拉", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:香格里拉:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:香格里拉:查找:失败");
        Delay(1000);
    }

    //创建队伍
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
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
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-auxiend.txt");
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

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-auxiend.txt");
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
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
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

    //寻路
    pathofposxys = "87:43|98:44|109:45|120:46";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[120:46]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[120:46]:成功");

    dmobj->KeyPressChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);
    
    dmobj->KeyPressChar("f2");
    Delay(500);
    dmobj->KeyUpChar("f2");
    Delay(500);
    
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    //TODO:按M:步骤13
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

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-vip.txt");
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

    //858:158 -> 388:402
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 470, lposY + 244);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //检查是否到达目标坐标
        /*dmobj->SetDict(0, FONTSIGNLE_DIR"font-posxy.txt");
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

        Delay(500);*/
    }
    

    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    //寻路
    pathofposxys = "222:147|231:142|239:138|243:136";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[243:136]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[243:136]:成功");

    tasknrref = 0;
    for (;;)
    {
        status = signle_copy3(dmobj, ctxt, tasknr);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("副本3:任务:完成:继续:下一个:副本:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("副本3:任务:继续:tasknrref:%d", tasknrref);
    }

    dmobj->MoveTo(5, 5);
    Delay(300);
    
    //步骤27
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("esc");
        Delay(500);
        dmobj->KeyUpChar("esc");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
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
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("esc:游戏结束:弹出,失败");
        Delay(500);
    }
    
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
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
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("esc:游戏结束:点击,失败");
        Delay(1000);
    }

    dmobj->MoveTo(5, 5);
    Delay(500);
        
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
        code = dmobj->FindStrFast(1040, 110, 1270, 240, "频道", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("频道:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            
            break;
        }

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
        code = dmobj->FindStrFast(1040, 110, 1270, 240, "频道", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("频道:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            
            break;
        }
        
        api_log_MsgDebug("频道:查找:失败");
        Delay(500);
    }
    
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->MoveTo(640, 670);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(1000);
    
    //点击进入游戏 640:670
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "阿尔卡迪亚", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:阿尔卡迪亚:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:阿尔卡迪亚:查找:失败");
        Delay(500);
    }

    status = signle_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgDebug("副本3->4:回城:失败:status:%d", status);
        return status;
    }

    api_log_MsgDebug("副本3->4:回城:成功");
    
    //等待回到平静河谷
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        //检查是否进入“平静河谷”
        Delay(1000);
        
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
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
    }
#endif

#ifdef SIGNLE_SEGMENT4
    #if defined(SIGNLE_SEGMENT12345_124) || defined(SIGNLE_SEGMENT45)
        dmobj->KeyDownChar("=");
        Delay(300);
        dmobj->KeyUpChar("=");
        Delay(300);

        Delay_With_Terminate_Check(3, luaman_task, dmobj);
        
        dmobj->KeyDownChar("0");
        Delay(300);
        dmobj->KeyUpChar("0");
        Delay(300);

        Delay_With_Terminate_Check(3, luaman_task, dmobj);
        
        pathofposxys = "349:129|353:118|355:112|359:102|368:101|375:101|385:101|396:98|406:94|415:90|424:89|431:88|437:89|445:88|451:88|451:78|451:69|450:62|450:58|450:53";
        pathOfPosXYs = CString(pathofposxys);
        code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("寻路:前往:坐标:[450:53]:死亡:复活");
            return SNAIL_ERRNO_REALIVE;
        }

        api_log_MsgDebug("寻路:前往:坐标:[450:53]:成功");
    #endif

    //28步
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[DAS]移动到DK服务器", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("[DAS]移动到DK服务器:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("[DAS]移动到DK服务器:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[DAS]移动到DK服务器", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("[DAS]移动到DK服务器:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("[DAS]移动到DK服务器:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        
        Delay(500);
    }
    
    //到达地图米特拉
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "米特拉", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:米特拉:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Delay(1000);
        api_log_MsgDebug("地图:米特拉:查找:失败");
    }

    //重新组队
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
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
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-auxiend.txt");
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

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-auxiend.txt");
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
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
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

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);

    Delay_With_Terminate_Check(2, luaman_task, dmobj);
    
    pathofposxys = "419:409|428:409|433:401|437:395|441:388";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[441:388]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[441:388]:成功");
    
    tasknrref = 0;
    for (;;)
    {
        status = signle_copy4(dmobj, ctxt, tasknr);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("副本4:任务:完成:继续:下一个:副本:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("副本4:任务:继续:tasknrref:%d", tasknrref);
    }

    //只有124副本完了才执行
    #ifdef SIGNLE_SEGMENT124
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->KeyDownChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);

            dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
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
            check_alarm(dmobj, ctxt);
            
            api_log_MsgDebug("esc:游戏结束:弹出,失败");
            Delay(500);
        }
        
        for (;;)
        {
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(500);

            dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
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
            check_alarm(dmobj, ctxt);
            
            api_log_MsgDebug("esc:游戏结束:点击,失败");
            Delay(1000);
        }

        dmobj->MoveTo(5, 5);
        Delay(500);
            
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
            code = dmobj->FindStrFast(1040, 110, 1270, 240, "频道", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("频道:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                
                break;
            }

            dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
            code = dmobj->FindStrFast(1040, 110, 1270, 240, "频道", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("频道:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                
                break;
            }
            
            api_log_MsgDebug("频道:查找:失败");
            Delay(500);
        }
        
        Delay_With_Terminate_Check(3, luaman_task, dmobj);

        dmobj->MoveTo(640, 670);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(1000);
        
        //点击进入游戏 640:670
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
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
            Delay(500);
        }

        status = signle_clickhome(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != status)
        {
            api_log_MsgDebug("副本3->4:回城:失败:status:%d", status);
            return status;
        }

        api_log_MsgDebug("副本3->4:回城:成功");
        
        //等待回到平静河谷
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->MoveTo(5, 5);
            Delay(200);
        
            //检查是否进入“平静河谷”
            Delay(1000);
            
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
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
        }
    #endif
#endif

#ifdef SIGNLE_SEGMENT5
    //33步
    pathofposxys = "436:399|434:408|433:416|433:425|438:434";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[438:434]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[438:434]:成功");

    dmobj->KeyDownChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);
    Delay(500);

    pathofposxys = "443:442|448:448|453:454|457:458|460:460";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[460:460]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[460:460]:成功");
    
    tasknrref = 0;
    for (;;)
    {
        status = signle_copy5(dmobj, ctxt, tasknr);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("副本5:任务:完成:继续:下一个:副本:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("副本5:任务:继续:tasknrref:%d", tasknrref);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    //注释掉兑换
    #ifdef HAVE_MAGIC_STONE
        int dianum = 20; //神秘的宝石按n对话的次数 罗爱平 2023/1/26 10:34:22
        for (i = 0; i < dianum; i++)
        {
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->KeyDownChar("n");
                Delay(500);
                dmobj->KeyUpChar("n");
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(200);
                
                dmobj->SetDict( 0, FONTSIGNLE_DIR"font-DuiHua10.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "神秘的宝石", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("神秘的宝石:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                
                api_log_MsgDebug("神秘的宝石:查找:失败");
                Delay(500);
            }

            int jix = 0;
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX, lposY);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict( 0, FONTSIGNLE_DIR"font-DuiHua10.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "谢谢", "00ff00-101010", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX2 = pposX2.lVal;
                    lposY2 = pposY2.lVal;

                    api_log_MsgDebug("谢谢:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                    break;
                }

                dmobj->SetDict( 0, FONTSIGNLE_DIR"font-DuiHua10.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "神秘的宝石", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    if (jix == 4)
                    {
                        api_log_MsgDebug("神秘的宝石:点击:完成");
                        goto Label_continue_talk;
                    }

                    api_log_MsgDebug("神秘的宝石:点击:继续:%d", jix);

                    jix++;
                    continue;
                 
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;

                jix++;
                api_log_MsgDebug("神秘的宝石:点击:继续, lposX:%ld, lposY:%ld, jix:%d", lposX, lposY, jix);
                Delay(500);
            }

            //点谢谢
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX2, lposY2);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict( 0, FONTSIGNLE_DIR"font-DuiHua10.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "谢谢", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("谢谢:查找:失败");
                    break;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("谢谢:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            
        Label_continue_talk:
            api_log_MsgDebug("对话:次数:%d", i);
        }
    #endif
    /////////////////////////////////////////////////////////////////////////////////////////////

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("esc");
        Delay(500);
        dmobj->KeyUpChar("esc");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
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
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("esc:游戏结束:弹出,失败");
        Delay(500);
    }
    
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
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
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("esc:游戏结束:点击,失败");
        Delay(1000);
    }

    dmobj->MoveTo(5, 5);
    Delay(500);
        
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
        code = dmobj->FindStrFast(1040, 110, 1270, 240, "频道", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("频道:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            
            break;
        }

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
        code = dmobj->FindStrFast(1040, 110, 1270, 240, "频道", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("频道:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            
            break;
        }
        
        api_log_MsgDebug("频道:查找:失败");
        Delay(500);
    }
    
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->MoveTo(640, 670);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(1000);
        
    //点击进入游戏 640:670
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
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
        Delay(500);
    }

    status = signle_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgDebug("副本3->4:回城:失败:status:%d", status);
        return status;
    }

    api_log_MsgDebug("副本3->4:回城:成功");
    
    //等待回到平静河谷
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        //检查是否进入“平静河谷”
        Delay(1000);
        
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
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
    }
#endif

#ifdef SIGNLE_SEGMENT6
    tasknrref = 0;
    for (;;)
    {
        status = signle_copy6(dmobj, ctxt, tasknrref);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("副本6:任务:完成:继续:下一个:副本:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("副本6:任务:继续:tasknrref:%d", tasknrref);
    }

    status = signle_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgError("副本6:回城:失败:status:%d", status);
        return status;
    }

    api_log_MsgDebug("副本6:回城:成功");

    //等待回到平静河谷
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        //检查是否进入“平静河谷”
        Delay(1000);
        
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "平静河谷", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("回城:地图:平静河谷:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("回城:地图:平静河谷:查找:失败");
    }
#endif

#ifdef SIGNLE_SEGMENT7
    tasknrref = 0;
    for (;;)
    {
        status = signle_copy7(dmobj, ctxt, tasknrref);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("副本7:任务:完成:继续:下一个:副本:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("副本7:任务:继续:tasknrref:%d", tasknrref);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("esc");
        Delay(500);
        dmobj->KeyUpChar("esc");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
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
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("esc:游戏结束:弹出,失败");
        Delay(500);
    }
    
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
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
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("esc:游戏结束:点击,失败");
        Delay(1000);
    }

    dmobj->MoveTo(5, 5);
    Delay(500);
        
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
        code = dmobj->FindStrFast(1040, 110, 1270, 240, "频道", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("频道:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            
            break;
        }

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
        code = dmobj->FindStrFast(1040, 110, 1270, 240, "频道", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("频道:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            
            break;
        }
        
        api_log_MsgDebug("频道:查找:失败");
        Delay(500);
    }
    
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->MoveTo(640, 670);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
        
    //点击进入游戏 640:670
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-DiTUMingZhi1.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "冰湖山地", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:冰湖山地:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:冰湖山地:查找:失败");
        Delay(500);
    }
    
    status = signle_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgError("副本7:回城:失败:status:%d", status);
        return status;
    }

    api_log_MsgDebug("副本7:回城:成功");

    //等待回到平静河谷
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        //检查是否进入“平静河谷”
        Delay(1000);
        
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
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
    }
    
#endif
    
    return SNAIL_ERRNO_SUCCESS;
}

static int signlework(struct cluamanaccountinfo* accountinfo, Idmsoft* dmobj, void* ctxt)
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
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    int i = 0;
    struct passwd2{char ch;char* path;} bmps[]=
    {
        {'0', BMPSIGNLE_DIR"0.bmp"},
        {'1', BMPSIGNLE_DIR"1.bmp"},
        {'2', BMPSIGNLE_DIR"2.bmp"},
        {'3', BMPSIGNLE_DIR"3.bmp"},
        {'4', BMPSIGNLE_DIR"4.bmp"},
        {'5', BMPSIGNLE_DIR"5.bmp"},
        {'6', BMPSIGNLE_DIR"6.bmp"},
        {'7', BMPSIGNLE_DIR"7.bmp"},
        {'8', BMPSIGNLE_DIR"8.bmp"},
        {'9', BMPSIGNLE_DIR"9.bmp"},
    };
    int j = 0;
    char* path = NULL;
    int dotask_num = 0;
    time_t check_conn_start;
    time_t check_conn_stop;
    double check_conn_deltaTime = 0.0;
    
    api_log_MsgDebug("user       : %s", accountinfo->user_name);
    api_log_MsgDebug("passwd     : %s", accountinfo->passwd);
    api_log_MsgDebug("passwdex   : %s", accountinfo->passwdex);

label_login:
    dmobj->MoveTo(5, 5);
    Delay(200);

    check_conn_start = time(NULL);
    
    //查找“帐号”
    //for (;;)
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-login.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "帐号", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
                
            api_log_MsgDebug("帐号:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        api_log_MsgDebug("帐号:查找:失败");
        Delay(500);

        check_conn_stop = time(NULL);
        check_conn_deltaTime = difftime(check_conn_stop, check_conn_start);
        if (check_conn_deltaTime > CHECK_CONN_LOST_TIME)
        {
            api_log_MsgWarn("帐号:查找:失败:掉线:检查");
            code = check_connection_status(dmobj, ctxt, api_TaskSignleInit, signlelockonoff);
            if (ERRNO_OFFLINE == code)
            {
                api_log_MsgWarn("账号:查找:掉线:重新:登录");
                goto label_login;
            }
        }
    }
    for_timeout_check_end()
    
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
        check_alarm(dmobj, ctxt);
        
        //dmobj->MoveTo(509, 730);
        dmobj->MoveTo(lposX2 + 50, lposY2 + 72);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-login.txt");
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

    check_conn_start = time(NULL);
    
    //检查二级密码
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-security.txt");
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
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-loginerr.txt");
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
                check_alarm(dmobj, ctxt);
                
                dmobj->MoveTo(lposX + 117, lposY + 291);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONTSIGNLE_DIR"font-loginerr.txt");
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

        check_conn_stop = time(NULL);
        check_conn_deltaTime = difftime(check_conn_stop, check_conn_start);
        if (check_conn_deltaTime > CHECK_CONN_LOST_TIME)
        {
            api_log_MsgDebug("二级:密码:查找:掉线:检查");
            
            code = check_connection_status(dmobj, ctxt, api_TaskSignleInit, signlelockonoff);
            if (ERRNO_OFFLINE == code)
            {
                api_log_MsgWarn("二级:密码:查找:掉线:重新:登录");
                goto label_login;
            }
        }
        
        api_log_MsgDebug("二级:密码:查找:失败");
        Delay(1000);
    }

    for (i = 0; i < strlen(accountinfo->passwdex); i++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
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
            check_alarm(dmobj, ctxt);
            
            dmobj->SetDict(0, FONTSIGNLE_DIR"font-security.txt");
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

    check_conn_start = time(NULL);
    
    //点击确认,请谨慎输入二级密码: 586:266 -> 686:430
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX0 + 100, lposY0 + 164);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-security.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "请谨慎输入二级密码", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("二级:密码:确认:成功");
            break;
        }

        check_conn_stop = time(NULL);
        check_conn_deltaTime = difftime(check_conn_stop, check_conn_start);
        if (check_conn_deltaTime > CHECK_CONN_LOST_TIME)
        {
            api_log_MsgDebug("二级:密码:确认:掉线:检查");
            
            code = check_connection_status(dmobj, ctxt, api_TaskSignleInit, signlelockonoff);
            if (ERRNO_OFFLINE == code)
            {
                api_log_MsgWarn("二级:密码:确认:掉线:重新:登录");
                goto label_login;
            }
        }
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;
                
        api_log_MsgDebug("二级:密码:确认:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(2000);
    }
    
    api_log_MsgDebug("进入:游戏:成功");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);

    dmobj->MoveTo(640, 670);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(200);

    i = 0;
    
    //点击“进入游戏” 640:670
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        //检查是否进入“平静河谷”
        Delay(1000);
        //Delay_With_Terminate_Check(3, luaman_task, dmobj);
        
    #ifdef LARGE_SPORT_SQUARE
        for (i = 0; i < 5; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "大型竞技场广场", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("地图:大型竞技场广场:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                goto next;
            }

            api_log_MsgDebug("地图:大型竞技场广场:查找:失败");
            Delay(1000);
        }
    #else
        #define LOOKUP_POSXY_AT_START_POSITION
        #ifdef LOOKUP_POSXY_AT_START_POSITION
            int ji = 0;
            for (ji = 0; ji < 5; ji++)
            {
                dmobj->SetDict(0, FONTSIGNLE_DIR"font-posxy.txt");
                strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
                dmobj->ClearDict(0);
                if (strPosXY != CString(""))
                {
                    spcnt = SplitStringWide(strPosXY, ":", posXY);
                    if (spcnt >= 2)
                    {
                        lposX = atol(posXY.GetAt(0));
                        lposY = atol(posXY.GetAt(1));

                        if (abs(lposX - 349) <= 1 && abs(lposY - 129) <= 1)
                        {
                            api_log_MsgDebug("进入游戏:坐标:349:129:查找:成功");
                            goto next;
                        }
                    }
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                api_log_MsgDebug("进入游戏:坐标:349:129:查找:失败:%d", i);
                Delay(1000);
                
                i++;
                if (i >= 10)
                {
                    //检查有没有掉线
                    code = check_connection_status(dmobj, ctxt, api_TaskSignleInit, signlelockonoff);
                    if (ERRNO_OFFLINE == code)
                    {
                        api_log_MsgWarn("进入游戏:坐标:349:129:失败:掉线:重新:登录");
                        goto label_login;
                    }
                    
                    //回城
                    code = signle_clickhome(dmobj, ctxt);
                    if (SNAIL_ERRNO_SUCCESS != code)
                    {
                        api_log_MsgError("进入游戏:坐标:349:129:回城:失败:code:%d", code);
                        return code;
                    }

                    api_log_MsgDebug("进入游戏:坐标:349:129:回城:成功");

                    //等待回到平静河谷
                    for (;;)
                    {
                        Check_Terminate_Thread(luaman_task, dmobj);
                        check_alarm(dmobj, ctxt);
                        
                        dmobj->MoveTo(5, 5);
                        Delay(200);
                    
                        //检查是否进入“平静河谷”
                        Delay(1000);
                        
                        Check_Terminate_Thread(luaman_task, dmobj);
                        check_alarm(dmobj, ctxt);
                        
                        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
                        code = dmobj->FindStrFast(1140, 29, 1273, 58, "平静河谷", "ffffff-101010", 0.9, &pposX, &pposY);
                        dmobj->ClearDict(0);
                        if (-1 != code)
                        {
                            lposX = pposX.lVal;
                            lposY = pposY.lVal;

                            api_log_MsgDebug("进入游戏:坐标:349:129:回城:地图:平静河谷:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                            break;
                        }

                        api_log_MsgDebug("进入游戏:坐标:349:129:回城:地图:平静河谷:查找:失败");
                    }

                    break;
                }
            }
        #else
            for (i = 0; i < 5; i++)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
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
        #endif
    #endif
    }

next:
    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dotask_num = 0;
    
    //执行“任务副本”
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        code = signlecopy(dmobj, ctxt, dotask_num);
        if (SNAIL_ERRNO_EXIT == code)
        {
            api_log_MsgDebug("副本:任务:执行:超时,dotask_num:%d, code:%d",
                dotask_num + 1, code);
            break;
        }

        dotask_num++;
        api_log_MsgDebug("副本:任务:继续,dotask_num:%d", dotask_num);

        if (1 == dotask_num)
        {
            api_log_MsgDebug("副本:任务:完成:继续:下一个:账户");
            break;
        }
    }

    api_log_MsgDebug("副本:任务:完成,dotask_num:%d", dotask_num);

    //清包
    (void)signlecopy_cleanpackage(dmobj, ctxt);

    //退出副本
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        //点击 1262:747
        dmobj->MoveTo(1262, 747);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-logout.txt");
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
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-saveconf.txt");
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
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-saveconf.txt");
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

static int switch_node_ipaddr(int hwnd_index, int curBindHostIdx)
{
    CURL* curl = NULL;
    CURLcode status = CURLE_OK;
    char buffer[256] = {0};
    int offset = 0;
    int port = 54321;

    offset = _snprintf(buffer, sizeof(buffer) - 1,
        "http://127.0.0.1:%d/api/v1/nodeRequestSwitch?nodeOrder=%d",
        port , hwnd_index + 1 + curBindHostIdx * 5);
    if (offset <= 0 || offset == sizeof(buffer) - 1)
    {
        api_log_MsgError("build nodeRequestSwitch failed");
        return ERRNO_FAILED;
    }

    api_log_MsgDebug("nodeRequestSwitch:%s", buffer);
    
    curl = curl_easy_init();
    if(NULL == curl)
    {
        api_log_MsgError("curl_easy_init failed");
        return ERRNO_FAILED;
    }

    curl_easy_setopt(curl, CURLOPT_URL, buffer);
    status = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if(CURLE_OK != status)
    {
        api_log_MsgError("curl_easy_perform failed, status:%d", status);
        return ERRNO_EXCEPTION;
    }
    
    api_log_MsgDebug("switch_node_ipaddr success, index:%d", hwnd_index);
    return ERRNO_SUCCESS;
}

static int TaskSignleMainLoop(struct list_head* accountList, Idmsoft* dmobj, void* ctxt)
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
    SYSTEMTIME lastTimeStamp;
    SYSTEMTIME curTimeStamp;
    int lastTimeday = 0;
    int curTimeday = 0;
    int curBindHostIdx = luaman_config->curBindHostIdx;
    
    struct signleconfig dotask_config;
    int lastnr = 0;
    int bok = signleloadconfig(bindWnd, std::string(CONFIG_SIGNLE_XML), &dotask_config);

    //拷贝进来,便于副本里面循环检查时输出用户名信息
    luaman_task->bindWnd = bindWnd;
    
    signleprintconfig(&dotask_config);

    GetLocalTime(&lastTimeStamp);
    lastTimeday = lastTimeStamp.wYear * 10000 +
        lastTimeStamp.wMonth * 100 + lastTimeStamp.wDay;

next_day:
    list_for_each_entry_safe(apos, an, accountList, node, struct cluamanaccountinfo)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        //拷贝进来,便于副本里面循环检查时输出用户名信息
        LUAMAN_MEMCPY(luaman_task->user_name, apos->user_name, strlen(apos->user_name) + 1);

        if (dotask_config.last_nr > lastnr)
        {
            lastnr++;
            continue;
        }

        api_log_MsgDebug("副本:任务:%s:窗口%d:主机%d:任务:开始",
            apos->user_name, bindWnd + 1, curBindHostIdx + 1);
        
        status = signlework(apos, dmobj, ctxt);
        if (ERRNO_SUCCESS != status)
        {
            api_log_MsgError("副本:任务:%s:失败, status:%d", apos->user_name, status);
        }

        lastnr++;

        dotask_config.last_nr = lastnr;
        memcpy(dotask_config.user_name, apos->user_name, strlen(apos->user_name) + 1);
        memcpy(dotask_config.passwd, apos->passwd, strlen(apos->passwd) + 1);
        memcpy(dotask_config.passwdex, apos->passwdex, strlen(apos->passwdex) + 1);
        
        (void)signlesaveconfig(std::string(CONFIG_SIGNLE_XML), &dotask_config);


       Delay_With_Terminate_Check(10, luaman_task, dmobj);

       //切换IP
       status = switch_node_ipaddr(bindWnd, curBindHostIdx);
       if (ERRNO_SUCCESS != status)
       {
           api_log_MsgWarn("副本:任务:%s:窗口%d:主机%d:切换:IP地址:失败, status:%d",
               apos->user_name, bindWnd + 1, curBindHostIdx + 1, status);
       }
       else
       {
           api_log_MsgDebug("副本:任务:%s:窗口%d:主机%d:切换:IP地址:成功",
               apos->user_name, bindWnd + 1, curBindHostIdx + 1);
       }

       Delay_With_Terminate_Check(20, luaman_task, dmobj);

        GetLocalTime(&curTimeStamp);
        curTimeday = curTimeStamp.wYear * 10000 +
            curTimeStamp.wMonth * 100 + curTimeStamp.wDay;
        if (curTimeday > lastTimeday)
        {
            lastTimeday = curTimeday;

            api_log_MsgDebug("副本:号码:%d:完成:跨天:重头:再来", lastnr);

            lastnr = 0;
            apos = list_first_entry(accountList, struct cluamanaccountinfo, node);
            dotask_config.last_nr = 0;
            memcpy(dotask_config.user_name, apos->user_name, strlen(apos->user_name) + 1);
            memcpy(dotask_config.passwd, apos->passwd, strlen(apos->passwd) + 1);
            memcpy(dotask_config.passwdex, apos->passwdex, strlen(apos->passwdex) + 1);
            (void)signlesaveconfig(std::string(CONFIG_SIGNLE_XML), &dotask_config);
            
            apos = NULL;
            an = NULL;
            goto next_day;
        }
    }

    //循环复原
    apos = list_first_entry(accountList, struct cluamanaccountinfo, node);
    dotask_config.last_nr = 0;
    memcpy(dotask_config.user_name, apos->user_name, strlen(apos->user_name) + 1);
    memcpy(dotask_config.passwd, apos->passwd, strlen(apos->passwd) + 1);
    memcpy(dotask_config.passwdex, apos->passwdex, strlen(apos->passwdex) + 1);
    signlesaveconfig(std::string(CONFIG_SIGNLE_XML), &dotask_config);
    
    return ERRNO_SUCCESS;
}

int api_TaskSignleMain(Idmsoft* dmobj, void* ctxt)
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
    
    api_log_MsgDebug("副本:Signle:主循环");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    list_for_each_entry_safe(bpos, bn, &luaman_config->bindinfoList, node, struct cluamanbindinfo)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        if (bindWnd != bpos->bindWndnr)
        {
            api_log_MsgDebug("SignleMain bindwnd unexpected, bindWnd:%ld. bindWndnr:%ld",
                bindWnd, bpos->bindWndnr);
            continue;
        }

        return TaskSignleMainLoop(&bpos->accountList, dmobj, ctxt);
    }

    return ERRNO_SUCCESS;
}

int api_TaskSignleExit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int bindWnd = luaman_config->curBindWnd;
    
    api_log_MsgDebug("副本:Signle:退出");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
        
    return ERRNO_SUCCESS;
}

