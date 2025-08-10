#include "stdafx.h"
#include "xrrno.h"
#include "xlog.h"
#include "xcommon.h"
#include "alarm.h"
#include "task195dllext.h"
#include "task195.h"
#include "task195manager.h"
#include "keyboard_monitor.h"
#include "task.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "snail_client_context.h"
#include "snail_client_manager.h"
#include "task1853_common.h"
#include "task1853_client_context.h"
#include "task1853_client_manager.h"
#include "task1853.h"
#include "task195.h"
#include "taskSignleCpy1.h"
#include "taskDotask.h"
#include "taskFight.h"
#include "taskFire.h"
#include "taskrefugeboat.h"
#include "taskSignle.h"
#include "taskExSignle.h"
#include "taskSignleCpy1Pri.h"
#include "taskEx175.h"
#include "taskTeamGroup.h"
#include "taskPirate.h"

CKeyboardMonitor::CKeyboardMonitor()
{
    LUAMAN_MEMSET(&keyboard_monitor, 0, sizeof(keyboard_monitor));
    
    keyboard_monitor.lock = CreateMutex(NULL, FALSE, NULL);
    if (NULL == keyboard_monitor.lock)
    {
        api_log_MsgError("create keyboard_monitor lock failed");
        return;
    }

    api_log_MsgDebug("create keyboard_monitor lock success");
    return;
}

CKeyboardMonitor::~CKeyboardMonitor()
{
    return;
}

static void keyboardMonitorEntryExit(void* keyboardmonitor, unsigned char force)
{
    KEYBOARD_MONITOR_S* keyboard_monitor = (KEYBOARD_MONITOR_S*)keyboardmonitor;
    DWORD code = WAIT_FAILED;
    DWORD processId = GetCurrentProcessId();
    DWORD taskId = GetCurrentThreadId();
    unsigned char status = 0;
    unsigned int ref = 0;

    code = WaitForSingleObject(keyboard_monitor->lock, INFINITE);
    if (WAIT_OBJECT_0 != code)
    {
        return;
    }

    keyboard_monitor->stop = (force)? KILL_THREAD_FORCE : KILL_THREAD_GRACEFULLY;
    keyboard_monitor->ref = (keyboard_monitor->ref > 0)? keyboard_monitor->ref - 1 : 0;
    ref = keyboard_monitor->ref;

    (void)ReleaseMutex(keyboard_monitor->lock);

    if (!ref)
    {
        if (force)
        {
            (void)CloseHandle(keyboard_monitor->thread->lock);
            free(keyboard_monitor->thread);
            keyboard_monitor->thread = NULL;
        }
        
        (void)CloseHandle(keyboard_monitor->lock);
        keyboard_monitor->lock = NULL;
    }

    api_log_MsgInfo("keyboard_monitor entry exit, ref:%u", ref);
    return;
}

static unsigned __cdecl keyboardMonitorEntry(void* keyboardmonitor)
{
    KEYBOARD_MONITOR_S* keyboard_monitor = (KEYBOARD_MONITOR_S*)keyboardmonitor;
    DWORD code = WAIT_FAILED;
    DWORD processId = GetCurrentProcessId();
    DWORD taskId = GetCurrentThreadId();
    unsigned char stop = 0;
    unsigned int ref = 0;
    unsigned int size = 0;
    char* ptr = NULL;
    Idmsoft dmobj[DMOBJ_MAX_NUM];
    HRESULT hr = ::CoInitialize(NULL);//初始化线程com库
    dmobj[0].CreateDispatch("dm.dmsoft");
    int status = 0;
    
    api_log_MsgInfo("keyboard_monitor thread (%ld) is running, hr:%ld", taskId, hr);

    status = init_exCommonLock();
    if (status)
    {
        api_log_MsgError("init_exCommonLock failed, status:%d", status);
        exit(1);
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    /* 主线程初始化告警任务 *///初始化与195副本相关资源
    status = InitMutex();
    if (status)
    {
        api_log_MsgError("InitMutex failed, status:%d", status);
    }

    status = alloc_alarm_ctxt();
    if (status)
    {
        api_log_MsgError("alloc_alarm_ctxt failed, status:%d", status);
    }

    status = InitManager(32, 2, 20);
    if (status)
    {
        api_log_MsgError("InitManager failed, status:%d", status);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    //初始化tz021副本相关资源
    status = snail_InitClientManager(SNAIL_CONTEXT_NUM_MAX);
    if (status)
    {
        api_log_MsgError("snail_InitClientManager failed, status:%d", status);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    //初始化tz1853/195副本资源
    api_TaskPiratePrepare();
    Tz1853_InitCriticalSection();
    Tz195_InitCriticalSection();
    api_TaskSignlePrepare();
    api_TaskExSignlePrepare();
    signlecpy_InitCriticalSection();
    signlecpy_pri_InitCriticalSection();
    firecpy_InitCriticalSection();
    TzRefugeboat_InitCriticalSection();
    
    status = Tz1853_InitClientManager(SNAIL_CONTEXT_NUM_MAX - 1);
    if (status)
    {
        api_log_MsgError("Tz1853_InitClientManager failed, status:%d", status);
    }

    //dotask任务副本资源
    api_TaskDotaskPrepare();
    
    for (;;)
    {
        code = WaitForSingleObject(keyboard_monitor->lock, INFINITE);
        if (WAIT_OBJECT_0 != code)
        {
            break;
        }

        stop = keyboard_monitor->stop;
        (void)ReleaseMutex(keyboard_monitor->lock);

        if (stop)
        {
            api_log_MsgInfo("keyboard_monitor (%ld) ordered terminate", taskId);
            break;
        }

        if (keyboard_monitor->callback)
        {
            if (keyboard_monitor->callback(&dmobj[0], keyboard_monitor->context))
            {
                api_log_MsgInfo("keyboard_monitor (%ld) callback finish", taskId);
                break;
            }
        }

        //Sleep(1000);
    }

    code = WaitForSingleObject(keyboard_monitor->lock, INFINITE);
    if (WAIT_OBJECT_0 != code)
    {
        api_log_MsgInfo("terminate keyboard_monitor (%ld) NOT gracefully", taskId);
        return 0;
    }

    keyboard_monitor->ref = (keyboard_monitor->ref > 0)? keyboard_monitor->ref - 1 : 0;
    ref = keyboard_monitor->ref;

    api_log_MsgInfo("keyboard_monitor entry (%ld) reference (%u)", taskId, ref);
    (void)ReleaseMutex(keyboard_monitor->lock);

    if (!ref)
    {
        if (KILL_THREAD_FORCE == stop)
        {
            (void)CloseHandle(keyboard_monitor->thread->lock);
            free(keyboard_monitor->thread);
            keyboard_monitor->thread = NULL;
        }
        
        (void)CloseHandle(keyboard_monitor->lock);
        keyboard_monitor->lock = NULL;
        api_log_MsgInfo("release keyboard_monitor entry (%ld) resource", taskId);
    }

    api_log_MsgInfo("terminate keyboard_monitor entry (%ld) gracefully", taskId);
    return 0;
}

void CKeyboardMonitor::set(int (*callback)(Idmsoft* dmobj, void* ctxt), void* context)
{
    keyboard_monitor.callback = callback;
    keyboard_monitor.context = context;
    
    return;
}

int CKeyboardMonitor::run()
{
    int status = 0;

    status = start_Thread(NULL, 0, keyboardMonitorEntry, &keyboard_monitor, keyboardMonitorEntryExit, &keyboard_monitor.thread);
    if (status)
    {
        api_log_MsgError("start keyboardMonitor failed, status:%d", status);
        return ERRNO_FAILED;
    }

    api_log_MsgDebug("start keyboardMonitor success, taskId:%ld", keyboard_monitor.thread->taskId);
    return ERRNO_SUCCESS;
}

void CKeyboardMonitor::kill()
{
    stop_Thread(&keyboard_monitor.thread, 0);
    keyboard_monitor.context = NULL;
    keyboard_monitor.callback = NULL;
    keyboard_monitor.stop = 0;
    keyboard_monitor.ref = 0;
    
    api_log_MsgInfo("kill keyboard_monitor finish");
    return;
}

int keyboard_monitor_callback(Idmsoft* dmobj, void* ctxt)
{
    int code = 0;
    
    code = dmobj->WaitKey(KEY_F9, 1000);
    if (0 == code)
    {
        goto wait_f8;
    }
    
    //启动窗口
    (void)g_luamantaskmgr.startTask(dmobj);
    return ERRNO_SUCCESS;

wait_f8:
    code = dmobj->WaitKey(KEY_F8, 200);
    if (0 == code)
    {
        goto wait_f12;
    }
    
    (void)g_luamantaskmgr.killTask(dmobj);
    return ERRNO_SUCCESS;

wait_f12:
    code = dmobj->WaitKey(KEY_F12, 200);
    if (0 == code)
    {
        return ERRNO_SUCCESS;
    }
    
    code = g_luamantaskmgr.killTaskAll(dmobj);
    if (ERRNO_SUCCESS == code)
    {
        return ERRNO_SUCCESS;
    }
    return ERRNO_EXIT;
}

