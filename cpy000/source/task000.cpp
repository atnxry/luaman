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
#include "task000.h"
#include "taskSignin.h"
#include "taskSakura.h"
#include "taskDotask.h"
#include "taskFight.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"

#define FONT000_DIR "font\\000\\"

int api_Task000Init(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int bindWnd = luaman_config->curBindWnd;
    int curBindHostIdx = luaman_config->curBindHostIdx;
    
    api_log_MsgDebug("副本:000:初始化");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);
    api_log_MsgDebug("curBindHostIdx  : %d", curBindHostIdx);
    
    return ERRNO_SUCCESS;
}

int api_Task000Main(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int bindWnd = luaman_config->curBindWnd;
    int code = 0;
    struct cluamanbindinfo* bpos = NULL, *bn = NULL, *bnode = NULL;
    struct cluamanaccountinfo* apos = NULL, *an = NULL, *anode = NULL;
    struct cluamanconfig* cpos = NULL, *cn = NULL, *cnode = NULL;
    struct cluamanconfig config;
    int status = 0;
    int curBindHostIdx = luaman_config->curBindHostIdx;
    
    api_log_MsgDebug("副本:000:主循环");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);
    api_log_MsgDebug("curBindHostIdx  : %d", curBindHostIdx);
    
    //api_PrintConfig(luaman_config);

    list_for_each_entry_safe(bpos, bn, &luaman_config->bindinfoList, node, struct cluamanbindinfo)
    {
        //if (bpos->bindWndnr != bindWnd + 1)
        if (bpos->bindWndnr != bindWnd)
        {
            continue;
        }

        bnode = bpos;
    }

    if (NULL == bnode)
    {
        AfxMessageBox("没有配置绑定窗口信息,请检查配置文件BindInfo段");
        api_log_MsgError("配置文件没有配置绑定窗口信息,bindWnd:%d", bindWnd);
        return ERRNO_NOT_EXIST;
    }

    list_for_each_entry_safe(cpos, cn, &luaman_config->mixConfigList, node, struct cluamanconfig)
    {
        memset(&config, 0, sizeof(struct cluamanconfig));
        INIT_LIST_HEAD(&config.node);
        INIT_LIST_HEAD(&config.mixConfigList);
        INIT_LIST_HEAD(&config.bindinfoList);

        status = api_CloneConfig(&config, cpos);
        if (ERRNO_SUCCESS != status)
        {
            api_log_MsgError("clone cpy000 %s config failed, status:%d", cpos->copyName, status);
            continue;
        }

        status = api_CloneConfigAppendBindinfo(&config, luaman_config);
        if (ERRNO_SUCCESS != status)
        {
            api_log_MsgError("clone_append cpy000 %s config failed, status:%d", cpos->copyName, status);
            continue;
        }

        luaman_taskctxt->context = (void*)&config;
        
        //status = api_TaskSigninInit(dmobj, ctxt);
        status = config.ops.init(dmobj, ctxt);
        if (ERRNO_SUCCESS != status)
        {
            api_log_MsgError("cpy call %d failed, status:%d", config.ops.type, status);
            goto next;
        }

        //status = api_TaskSigninMain(dmobj, ctxt);
        status = config.ops.main(dmobj, ctxt);
        if (ERRNO_SUCCESS != status)
        {
            api_log_MsgError("cpy call %d failed, status:%d", config.ops.type, status);
            goto next;
        }

        //status = api_TaskSigninExit(dmobj, ctxt);
        status = config.ops.exit(dmobj, ctxt);
        if (ERRNO_SUCCESS != status)
        {
            api_log_MsgError("cpy call %d failed, status:%d", config.ops.type, status);
            goto next;
        }
        
    next:
        api_ClearMixConfig(&config);
    }

    //恢复000的配置
    luaman_taskctxt->context = luaman_config;
    
    return ERRNO_SUCCESS;
}

int api_Task000Exit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int bindWnd = luaman_config->curBindWnd;
    
    api_log_MsgDebug("副本:000:退出");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    return ERRNO_SUCCESS;
}

