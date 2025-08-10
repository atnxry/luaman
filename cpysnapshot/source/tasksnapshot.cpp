#include <math.h>
#include <stdlib.h>
#include <process.h>
#include <direct.h>
#include "stdafx.h"
//#include "xrrno.h"
#include "xlog.h"
#include "xcommon.h"
#include "xlist.h"
#include "task.h"
#include "config.h"
#include "alarm.h"
#include "task.h"
#include "tasksnapshot.h"

static int spec_mkdir(char* path)
{
    char* ptr = path;

    while (*ptr)
    {
        if ('\\' == *ptr)
        {
            char szdir[128] = {0};

            memset(szdir, 0, sizeof(szdir));
            memcpy(szdir, path, ptr - path);
            //::CreateDirectory(szdir, NULL);
            mkdir(szdir);

            //api_log_MsgDebug("创建:szdir:%s", szdir);
        }

        ++ptr;
    }

    //::CreateDirectory(path, NULL);
    mkdir(path);

    //api_log_MsgDebug("创建:path:%s", path);
    return 0;
}

static int dosnapshot(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    VARIANT posX1, posY1;
    VARIANT posX2, posY2;
    long lposX1 = 0, lposY1 = 0;
    long lposX2 = 0, lposY2 = 0;
    VARIANT width, height;
    long lwidth = 0, lheight = 0;
    SYSTEMTIME curTimeStamp;
    int offset = 0;
    char buff[256] = {0};
    
    code = dmobj->GetWindowRect(hwnd, &posX1, &posY1, &posX2, &posY2);
    if (!code)
    {
        api_log_MsgError("GetWindowRect:失败");
        return ERRNO_EXCEPTION;
    }

    lposX1 = posX1.lVal;
    lposY1 = posY1.lVal;
    lposX2 = posX2.lVal;
    lposY2 = posY2.lVal;

    api_log_MsgInfo("GetWindowRect:成功");
    api_log_MsgInfo("lposX1:%ld, lposY1:%ld, lposX2:%ld, lposY2:%ld",
        lposX1, lposY1, lposX2, lposY2);
    
    code = dmobj->GetClientRect(hwnd, &posX1, &posY1, &posX2, &posY2);
    if (!code)
    {
        api_log_MsgError("GetClientRect:失败");
        return ERRNO_EXCEPTION;
    }

    lposX1 = posX1.lVal;
    lposY1 = posY1.lVal;
    lposX2 = posX2.lVal;
    lposY2 = posY2.lVal;

    api_log_MsgInfo("GetClientRect:成功");
    api_log_MsgInfo("lposX1:%ld, lposY1:%ld, lposX2:%ld, lposY2:%ld",
        lposX1, lposY1, lposX2, lposY2);
    
    code = dmobj->GetClientSize(hwnd, &width, &height);
    if (!code)
    {
        api_log_MsgError("GetClientSize:失败");
        return ERRNO_EXCEPTION;
    }

    lwidth = width.lVal;
    lheight = height.lVal;
    
    api_log_MsgInfo("GetClientSize:成功, lwidth:%ld, lheight:%ld", lwidth, lheight);

    GetLocalTime(&curTimeStamp);
    offset = _snprintf(buff, sizeof(buff), "snapshot\\%04d%02d%02d\\",
        curTimeStamp.wYear, curTimeStamp.wMonth, curTimeStamp.wDay);
    if (offset <= 0 || offset == sizeof(buff))
    {
        api_log_MsgError("_snprintf:目录:失败");
        return ERRNO_IO;
    }

    (void)spec_mkdir(buff);
    
    offset = _snprintf(buff, sizeof(buff), "snapshot\\%04d%02d%02d\\%02d%02d%02d_%d.bmp",
        curTimeStamp.wYear, curTimeStamp.wMonth, curTimeStamp.wDay,
        curTimeStamp.wHour, curTimeStamp.wMinute, curTimeStamp.wSecond,
        curTimeStamp.wMilliseconds);
    if (offset <= 0 || offset == sizeof(buff))
    {
        api_log_MsgError("_snprintf:图片:失败");
        return ERRNO_IO;
    }
    
    code = dmobj->Capture(0, 0, lwidth, lheight, buff);
    if (1 != code)
    {
        api_log_MsgError("截图:失败:%s", buff);
        AfxMessageBox("截图失败");
        return ERRNO_IO;
    }

    AfxMessageBox("截图成功");
    api_log_MsgDebug("截图:成功:%s", buff);
    return ERRNO_SUCCESS;
}

int api_TaskSnapshotInit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;

    api_log_MsgDebug("副本:snapshot:初始化");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);

    api_PrintConfig(luaman_config);
    
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    //code = dmobj->BindWindow(hwnd, "dx", "dx2", "dx", 1);
    if (1 != code)
    {
        api_log_MsgError("副本:snapshot:窗口:绑定:失败, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("副本:snapshot:窗口:绑定:成功, hwnd:%ld", hwnd);

    code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("副本:snapshot:窗口:鼠标:锁定:失败, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("副本:snapshot:窗口:鼠标:锁定:成功, hwnd:%ld", hwnd);

    /* 初始化告警时间初始值 */
    init_alarm_ctxt();
    
    return ERRNO_SUCCESS;
}

int api_TaskSnapshotMain(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    
    api_log_MsgDebug("副本:snapshot:主循环:开始: hwnd:%ld, role:%d", hwnd, (int)role);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        code = dmobj->WaitKey(KEY_F7, 1000);
        if (0 == code)
        {
            continue;
        }
        
        (void)dosnapshot(dmobj, ctxt);
    }
    
    api_log_MsgDebug("副本:snapshot:主循环:完成, hwnd:%ld", hwnd);
    
    return ERRNO_SUCCESS;
}

int api_TaskSnapshotExit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;

    api_log_MsgDebug("副本:snapshot:结束: hwnd:%ld, role:%d", hwnd, (int)role);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
    
    return ERRNO_SUCCESS;
}

