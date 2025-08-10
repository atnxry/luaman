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
#include "taskExSignle.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "Markup.h"
#include "curl.h"
#include "taskExSignle_Pub.h"

#define KEY_exsignle_Root         "Root"
#define KEY_exsignle_Account      "Account"
#define KEY_exsignle_Bindwnd      "Bindwnd"
#define KEY_exsignle_Lastnr       "Lastnr"
#define KEY_exsignle_Lastroleno   "Lastroleno"
#define KEY_exsignle_Username     "Username"
#define KEY_exsignle_Password     "Password"
#define KEY_exsignle_Passwordex   "Passwordex"

#define CONFIG_EXSIGNLE_XML  "conf\\exsignle.xml"

struct exsignleconfig
{
    int bindwnd;
    int last_nr;
    int last_roleno;
    char user_name[USRNAME_SIZE];
    char passwd[PASSWORD_SIZE];
    char passwdex[PASSWORD_SIZE];
};

static HANDLE exsignlelock;
int api_TaskExSignlePrepare(void)
{
    exsignlelock = CreateMutex(NULL, FALSE, NULL);
    if (!exsignlelock)
    {
        api_log_MsgError("api_TaskExSignlePrepare failed");
        exit(1);
    }

    api_log_MsgDebug("api_TaskExSignlePrepare success");
    return 0;
}

static void exsignleEnterCriticalSection(void)
{
    ::WaitForSingleObject(exsignlelock, INFINITE);
    api_log_MsgDebug("exsignleEnterCriticalSection::加锁:成功");
    return;
}

static void exsignleLeaveCriticalSection(void)
{
    (void)::ReleaseMutex(exsignlelock);
    api_log_MsgDebug("exsignleLeaveCriticalSection::解锁:成功");
    return;
}

static void print_config(struct exsignleconfig* config)
{
    api_log_MsgDebug("bindwnd     : %d", config->bindwnd);
    api_log_MsgDebug("last_nr     : %d", config->last_nr);
    api_log_MsgDebug("last_roleno : %d", config->last_roleno);
    api_log_MsgDebug("user_name   : %s", config->user_name);
    api_log_MsgDebug("passwd      : %s", config->passwd);
    api_log_MsgDebug("passwdex    : %s", config->passwdex);
    
    return;
}

static int creat_XML(std::string& file, struct exsignleconfig* config)
{
    CMarkup xml;
    xml.SetDoc(CString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"));
    xml.AddElem(CString(KEY_exsignle_Root));

    xml.IntoElem();
    xml.AddElem(CString(KEY_exsignle_Account));
    xml.IntoElem();
    
    ITEM_NEW_D(xml, KEY_exsignle_Bindwnd, config->bindwnd);
    ITEM_NEW_D(xml, KEY_exsignle_Lastnr, config->last_nr);
    ITEM_NEW_D(xml, KEY_exsignle_Lastroleno, config->last_roleno);
    
    ITEM_NEW(xml, KEY_exsignle_Username, config->user_name);
    ITEM_NEW(xml, KEY_exsignle_Password, config->passwd);
    ITEM_NEW(xml, KEY_exsignle_Passwordex, config->passwdex);
    
    xml.OutOfElem();
    xml.OutOfElem();

    xml.Save(CString(file.c_str()));
    return ERRNO_SUCCESS;
}

static int __save_config(std::string& file, struct exsignleconfig* config)
{
    CMarkup xml;
    bool bok = false;
    int bindwnd = -1;
    
    if (!xml.Load(CString(file.c_str())))
    {
        api_log_MsgError("load %s failed when saveXML", file.c_str());
        return creat_XML(file, config);
    }

    xml.ResetMainPos();
    if (!xml.FindElem(CString(KEY_exsignle_Root)))
    {
        return ERRNO_FAILED;
    }

    xml.IntoElem();
    while (xml.FindElem(CString(KEY_exsignle_Account)))
    {
        xml.IntoElem();
        while (xml.FindElem(NULL))
        {
            CString tag = xml.GetTagName();
            CString attr = xml.GetAttrib(CString(ATTR_VALUE));

            if (CString(KEY_exsignle_Bindwnd) == tag)
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
            
            if (CString(KEY_exsignle_Lastnr) == tag)
            {
                ITEM_UPDATE_D(xml, config->last_nr);
                continue;
            }

            if (CString(KEY_exsignle_Lastroleno) == tag)
            {
                ITEM_UPDATE_D(xml, config->last_roleno);
                continue;
            }

            if (CString(KEY_exsignle_Username) == tag)
            {
                ITEM_UPDATE(xml, config->user_name);
                continue;
            }

            if (CString(KEY_exsignle_Password) == tag)
            {
                ITEM_UPDATE(xml, config->passwd);
                continue;
            }

            if (CString(KEY_exsignle_Passwordex) == tag)
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
        xml.AddElem(CString(KEY_exsignle_Account));
        xml.IntoElem();
        
        ITEM_NEW_D(xml, KEY_exsignle_Bindwnd, config->bindwnd);
        ITEM_NEW_D(xml, KEY_exsignle_Lastnr, config->last_nr);
        ITEM_NEW_D(xml, KEY_exsignle_Lastroleno, config->last_roleno);
        ITEM_NEW(xml, KEY_exsignle_Username, config->user_name);
        ITEM_NEW(xml, KEY_exsignle_Password, config->passwd);
        ITEM_NEW(xml, KEY_exsignle_Passwordex, config->passwdex);
        
        xml.OutOfElem();
        xml.OutOfElem();

        xml.Save(CString(file.c_str()));
    }

    return ERRNO_SUCCESS;
}

static int save_config(std::string& file, struct exsignleconfig* config)
{
    int bok = 0;
    
    LUAMAN_LOCK(exsignlelock);
    bok = __save_config(file, config);
    LUAMAN_UNLOCK(exsignlelock);

    return bok;
}

static bool __load_config(int bindwnd, CMarkup& xml, struct exsignleconfig* config)
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
        
        if (CString(KEY_exsignle_Bindwnd) == tag)
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
        if (CString(KEY_exsignle_Lastnr) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->last_nr = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->last_nr:%d", config->last_nr);
            continue;
        }

        if (CString(KEY_exsignle_Lastroleno) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->last_roleno = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->last_roleno:%d", config->last_roleno);
            continue;
        }

        if (CString(KEY_exsignle_Username) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->user_name, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->user_name:%s", config->user_name);
            continue;
        }

        if (CString(KEY_exsignle_Password) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->passwd, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->passwd:%s", config->passwd);
            continue;
        }

        if (CString(KEY_exsignle_Passwordex) == tag)
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

static bool _load_config(int bindwnd, std::string& file, struct exsignleconfig* config)
{
    bool bok = false;
    CMarkup xml;
    
    if (!xml.Load(CString(file.c_str())))
    {
        api_log_MsgError("LoadXML %s failed", file.c_str());
        return false;
    }

    xml.ResetMainPos();
    if (!xml.FindElem(CString(KEY_exsignle_Root)))
    {
        api_log_MsgError("LoadXML %s failed, KEY_Root:%s", file.c_str(), KEY_exsignle_Root);
        return false;
    }

    xml.IntoElem();
    while (xml.FindElem(CString(KEY_exsignle_Account)))
    {
        xml.IntoElem();
        bok = __load_config(bindwnd, xml, config);
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

static bool load_config(int bindwnd, std::string& file, struct exsignleconfig* config)
{
    bool bok = false;

    memset(config, 0, sizeof(struct exsignleconfig));
    
    LUAMAN_LOCK(exsignlelock);
    bok = _load_config(bindwnd, file, config);
    LUAMAN_UNLOCK(exsignlelock);
    
    return bok;
}

static int main_loop(struct list_head* accountList, Idmsoft* dmobj, void* ctxt)
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
    
    struct exsignleconfig dotask_config;
    int lastnr = 0;
    int maxroleno = luaman_config->copyrolenr + 1;
    int roleno = 0;
    int bok = 0;
    int exitflag = 0;

    api_log_MsgDebug("main_loop:开始, maxroleno:%d", maxroleno);
    
    bok = load_config(bindWnd, std::string(CONFIG_EXSIGNLE_XML), &dotask_config);
    
    //拷贝进来,便于副本里面循环检查时输出用户名信息
    luaman_task->bindWnd = bindWnd;
    
    print_config(&dotask_config);

    GetLocalTime(&lastTimeStamp);
    lastTimeday = lastTimeStamp.wYear * 10000 +
        lastTimeStamp.wMonth * 100 + lastTimeStamp.wDay;

    //不同账号执行副本任务
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

        //dotask_config.bindwnd = bindWnd;
        memcpy(dotask_config.user_name, apos->user_name, strlen(apos->user_name) + 1);
        memcpy(dotask_config.passwd, apos->passwd, strlen(apos->passwd) + 1);
        memcpy(dotask_config.passwdex, apos->passwdex, strlen(apos->passwdex) + 1);
        
        api_log_MsgDebug("任务:%s:窗口%d:主机%d:任务:开始:账号:%d",
            apos->user_name, bindWnd + 1, curBindHostIdx + 1, lastnr);

        status = ERRNO_SUCCESS;
        exitflag = 0;
        
        //不同角色切换执行副本任务
        for (roleno = 0; roleno < maxroleno; roleno++)
        {
            if (roleno < dotask_config.last_roleno)
            {
                continue;
            }
            
            api_log_MsgDebug("任务:%s:窗口%d:主机%d:任务:开始:账号:%d:角色:%d:%d",
                apos->user_name, bindWnd + 1, curBindHostIdx + 1, lastnr, roleno, dotask_config.last_roleno);
            
            status = start_work(apos, roleno, dmobj, ctxt);
            if (ERRNO_SUCCESS != status)
            {
                dotask_config.last_roleno = roleno % maxroleno;
                exitflag = 1;
                
                api_log_MsgError("任务:%s:角色:%d:失败, status:%d", apos->user_name, roleno, status);
                break;
            }

            dotask_config.last_roleno = (roleno + 1) % maxroleno;

            /* 每个角色完了也要记录 */
            (void)save_config(std::string(CONFIG_EXSIGNLE_XML), &dotask_config);
            if (ERRNO_SUCCESS != status)
            {
                api_log_MsgDebug("任务:%s:窗口%d:主机%d:任务:账号:%d:角色:%d:记录:失败:%d",
                    apos->user_name, bindWnd + 1, curBindHostIdx + 1, lastnr, dotask_config.last_roleno,
                    status);
                
                return status;
            }

            api_log_MsgDebug("任务:%s:窗口%d:主机%d:任务:账号:%d:角色:%d:记录:成功",
                apos->user_name, bindWnd + 1, curBindHostIdx + 1, lastnr, dotask_config.last_roleno);
        }

        if (ERRNO_SUCCESS == status) //全部执行成功了才递增
        {
            lastnr++;
        }
        
        dotask_config.last_nr = lastnr;
        (void)save_config(std::string(CONFIG_EXSIGNLE_XML), &dotask_config);
        if (ERRNO_SUCCESS != status)
        {
            api_log_MsgDebug("任务:%s:窗口%d:主机%d:任务:账号:%d:角色:%d:中止",
                apos->user_name, bindWnd + 1, curBindHostIdx + 1, lastnr, dotask_config.last_roleno);
            
            return status;
        }

        if (exitflag)
        {
            api_log_MsgDebug("任务:%s:窗口%d:主机%d:任务:账号:%d:角色:%d:终止",
                apos->user_name, bindWnd + 1, curBindHostIdx + 1, lastnr, dotask_config.last_roleno);
            
            return ERRNO_FAILED;
        }

        api_log_MsgDebug("任务:%s:窗口%d:主机%d:任务:账号:%d:角色:%d:继续",
            apos->user_name, bindWnd + 1, curBindHostIdx + 1, lastnr, dotask_config.last_roleno);
    }

    //循环复原,所有账号所有角色都执行完了归零
    apos = list_first_entry(accountList, struct cluamanaccountinfo, node);
    dotask_config.last_nr = 0;
    dotask_config.last_roleno = 0;
    memcpy(dotask_config.user_name, apos->user_name, strlen(apos->user_name) + 1);
    memcpy(dotask_config.passwd, apos->passwd, strlen(apos->passwd) + 1);
    memcpy(dotask_config.passwdex, apos->passwdex, strlen(apos->passwdex) + 1);
    save_config(std::string(CONFIG_EXSIGNLE_XML), &dotask_config);

    api_log_MsgDebug("任务:%s:窗口%d:主机%d:任务:账号:%d:角色:%d:完成",
        apos->user_name, bindWnd + 1, curBindHostIdx + 1, dotask_config.last_nr, dotask_config.last_roleno);
    
    return ERRNO_SUCCESS;
}

int api_TaskExSignleInit(Idmsoft* dmobj, void* ctxt)
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
    
    api_log_MsgDebug("副本:ExSignle:初始化");
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

int api_TaskExSignleMain(Idmsoft* dmobj, void* ctxt)
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
    
    api_log_MsgDebug("副本:ExSignle:主循环");
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
            api_log_MsgDebug("ExSignleMain bindwnd unexpected, bindWnd:%ld. bindWndnr:%ld",
                bindWnd, bpos->bindWndnr);
            continue;
        }

        return main_loop(&bpos->accountList, dmobj, ctxt);
    }

    return ERRNO_SUCCESS;
}

int api_TaskExSignleExit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int bindWnd = luaman_config->curBindWnd;
    
    api_log_MsgDebug("副本:ExSignle:退出");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
    
    return ERRNO_SUCCESS;
}
