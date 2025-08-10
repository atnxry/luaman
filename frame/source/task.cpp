#include "stdafx.h"
#include "xrrno.h"
#include "xlog.h"
#include "xcommon.h"
#include "snail_client_context.h"
#include "snail_client_manager.h"
#include "config.h"
#include "alarm.h"
#include "task.h"
#include "task195manager.h"
#include <process.h>

#define BMP_EXCOMMON_DIR "bmp\\excommon\\"
#define FONT_EXCOMMON_DIR "font\\excommon\\"
#define EXCOMMON_ROLES_NR   (26)

//#define LUAMAN_DEBUG

clumantaskmgr g_luamantaskmgr;


static void taskEntryExit(void* luamantask, unsigned char force)
{
    struct clumantask* luaman_task = (struct clumantask*)luamantask;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    DWORD code = WAIT_FAILED;
    DWORD processId = GetCurrentProcessId();
    DWORD taskId = GetCurrentThreadId();
    unsigned char status = 0;
    unsigned int ref = 0;

    code = WaitForSingleObject(luaman_taskctxt->lock, INFINITE);
    if (WAIT_OBJECT_0 != code)
    {
        return;
    }

    luaman_taskctxt->stop = (force)? KILL_THREAD_FORCE : KILL_THREAD_GRACEFULLY;
    luaman_taskctxt->ref = (luaman_taskctxt->ref > 0)? luaman_taskctxt->ref - 1 : 0;
    ref = luaman_taskctxt->ref;

    (void)ReleaseMutex(luaman_taskctxt->lock);

    if (!ref)
    {
        if (force)
        {
            (void)CloseHandle(luaman_taskctxt->thread->lock);
            free(luaman_taskctxt->thread);
            luaman_taskctxt->thread = NULL;
        }
        
        //(void)CloseHandle(luaman_taskctxt->lock);
        //luaman_taskctxt->lock = NULL;
    }

    api_log_MsgInfo("task entry exit, ref:%u", ref);
    return;
}

#include "task000.h"

static unsigned __cdecl taskEntry(void* luamantask)
{
    struct clumantask* luaman_task = (struct clumantask*)luamantask;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfiglist* luaman_config_list = luaman_taskctxt->config;
    struct cluamanconfig* luaman_config = NULL;
    struct cluamanconfig* pos = NULL;
    struct cluamanconfig* n = NULL;
    DWORD code = WAIT_FAILED;
    DWORD processId = GetCurrentProcessId();
    DWORD taskId = GetCurrentThreadId();
    unsigned char stop = 0;
    unsigned int ref = 0;
    unsigned int size = 0;
    char* ptr = NULL;
    int status = 0;
    Idmsoft* dmobj = luaman_task->dmobj;
    HRESULT hr = ::CoInitialize(NULL);//初始化线程com库
    //dmobj.CreateDispatch("dm.dmsoft");
    dmobj->CreateDispatch("dm.dmsoft");
    unsigned int loop_num = 0;
    unsigned int max_loop_num = 0;
    char* tasktips[] = TASK_TYPE_DESC_ARRAY();
    SYSTEMTIME lastTimeStamp;
    SYSTEMTIME curTimeStamp;
    int lastTimeday = 0;
    int curTimeday = 0;
    VARIANT pposX2, pposY2;
    long lposX2 = 0, lposY2 = 0;
    
    luaman_task->taskid = taskId;
    luaman_task->pid = processId;
    
    api_log_MsgInfo("task running success, hwnd:%ld, pid:%ld, taskid:%ld, luaman_config_list:%p",
        luaman_task->hwnd, processId, taskId, luaman_config_list);

    list_for_each_entry_safe(pos, n, &luaman_config_list->head, node, struct cluamanconfig)
    {
        luaman_config = pos;
        luaman_taskctxt->context = luaman_config;

        api_log_MsgInfo("+++++++ ready execute task %s +++++++++++", tasktips[luaman_config->taskType]);
        api_PrintConfig(luaman_config);

        if (luaman_config->ops.init)
        {
            status = luaman_config->ops.init(dmobj, luaman_task);
            if (status)
            {
                api_log_MsgWarn("init failed, goto next task, status:%d", status);
                continue;
            }
        }

        GetLocalTime(&lastTimeStamp);
        lastTimeday = lastTimeStamp.wYear * 10000 +
            lastTimeStamp.wMonth * 100 + lastTimeStamp.wDay;

        max_loop_num = luaman_config->loop_num;

        api_log_MsgDebug("max_loop_num:%u", max_loop_num);
        
        for (;;)
        {
            //指定任务次数时完成后结束任务,不指定为0时死循环执行
            if (max_loop_num)
            {
                if (loop_num > max_loop_num)
                {
                    break;
                }
                
                loop_num++;
            }
            
            code = WaitForSingleObject(luaman_taskctxt->lock, INFINITE);
            if (WAIT_OBJECT_0 != code)
            {
                break;
            }

            stop = luaman_taskctxt->stop;
            (void)ReleaseMutex(luaman_taskctxt->lock);

            if (stop)
            {
                api_log_MsgInfo("task (%ld) ordered terminate", taskId);
                goto back;
            }

            if (luaman_config->ops.main)
            {
                if (luaman_config->ops.main(dmobj, luaman_task))
                {
                    api_log_MsgInfo("task (%ld) callback finish", taskId);
                    break;
                }
            }

            //每完成一次任务时检查是否是第二天,如果是则继续;不是则等到第二天再继续
            //只有000副本才检查
            if (TASK_TYPE_000 == luaman_config->taskType)
            {
                for (;;)
                {
                    GetLocalTime(&curTimeStamp);
                    curTimeday = curTimeStamp.wYear * 10000 +
                        curTimeStamp.wMonth * 100 + curTimeStamp.wDay;
                    
                    if (curTimeday > lastTimeday)
                    {
                        lastTimeday = curTimeday;
                        break;
                    }

                    for_timeout_check_start(luaman_task)
                    {
                        Check_Terminate_Thread(luaman_task, dmobj);
                        //check_alarm(dmobj, luaman_task);
                        
                        dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-login.txt");
                        code = dmobj->FindStrFast(0, 27, 1279, 826, "帐号", "ffffff-101010", 0.9, &pposX2, &pposY2);
                        dmobj->ClearDict(0);
                        if (-1 != code)
                        {
                            lposX2 = pposX2.lVal;
                            lposY2 = pposY2.lVal;
                                
                            api_log_MsgDebug("帐号:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                        }

                        break;
                    }
                    for_timeout_check_end();
                    
                    Delay_With_Terminate_Check(2, luaman_task, dmobj);
                }
            }
        
            Sleep(1000);
        }

        if (luaman_config->ops.exit)
        {
            status = luaman_config->ops.exit(dmobj, luaman_task);
            if (status)
            {
                api_log_MsgWarn("exit not expected, code:%d", status);
            }
        }
    }

back:
    code = WaitForSingleObject(luaman_taskctxt->lock, INFINITE);
    if (WAIT_OBJECT_0 != code)
    {
        api_log_MsgInfo("terminate task (%ld) NOT gracefully", taskId);
        return 0;
    }

    luaman_taskctxt->ref = (luaman_taskctxt->ref > 0)? luaman_taskctxt->ref - 1 : 0;
    ref = luaman_taskctxt->ref;

    api_log_MsgInfo("task entry (%ld) reference (%u)", taskId, ref);
    (void)ReleaseMutex(luaman_taskctxt->lock);

    if (!ref)
    {
        if (KILL_THREAD_FORCE == stop)
        {
            (void)CloseHandle(luaman_taskctxt->thread->lock);
            free(luaman_taskctxt->thread);
            luaman_taskctxt->thread = NULL;
        }
        
        (void)CloseHandle(luaman_taskctxt->lock);
        luaman_taskctxt->lock = NULL;

        //TODO: 释放config资源
        
        api_log_MsgInfo("release task entry (%ld) resource", taskId);
    }

    api_log_MsgInfo("terminate task entry (%ld) gracefully", taskId);
    return 0;
}


/////////////////////////////////////////////////////////////////////////////
clumantaskmgr::clumantaskmgr()
{
    int j = 0;

    memset(&m_configlist, 0, sizeof(struct cluamanconfiglist));
    INIT_LIST_HEAD(&m_configlist.head);
    m_configlist.lock = CreateMutex(NULL, FALSE, NULL);
    if (!m_configlist.lock)
    {
        exit(1);
    }
    
    memset(&m_lstTask, 0, sizeof(struct clumantasklist));
    INIT_LIST_HEAD(&m_lstTask.head);
    m_lstTask.lock = CreateMutex(NULL, FALSE, NULL);
    if (!m_lstTask.lock)
    {
        CloseHandle(m_configlist.lock);
        exit(1);
    }

    for (j = 0; j < DMOBJ_MAX_NUM; j++)
    {
        dmobj_flag[j] = 0;
    }
}

clumantaskmgr::~clumantaskmgr()
{
    struct clumantask* pos = NULL;
    struct clumantask* n = NULL;

    clearCurConfig(&m_configlist);
    if (m_configlist.lock)
    {
        CloseHandle(m_configlist.lock);
        m_configlist.lock = NULL;
    }
    
    LUAMAN_LOCK(m_lstTask.lock);
    list_for_each_entry_safe(pos, n, &m_lstTask.head, node, struct clumantask)
    {
        api_log_MsgDebug("del clumantask, hwnd:%ld, taskid:%lu",
            pos->hwnd, pos->taskid);
        
        list_del_init(&pos->node);
        LUAMAN_FREE(pos);
    }
    LUAMAN_UNLOCK(m_lstTask.lock);
    
    if (m_lstTask.lock)
    {
        CloseHandle(m_lstTask.lock);
        m_lstTask.lock = NULL;
    }
}

void clumantaskmgr::clearCurConfig(struct cluamanconfiglist* list)
{
    struct cluamanconfig* pos = NULL;
    struct cluamanconfig* n = NULL;

    LUAMAN_LOCK(m_configlist.lock);
    list_for_each_entry_safe(pos, n, &list->head, node, struct cluamanconfig)
    {
        //api_log_MsgDebug("clear cluamanconfig, taskType:%ld, role:%lu", pos->taskType, pos->role);
        
        list_del_init(&pos->node);
        api_ClearMixConfig(pos);
        
        LUAMAN_FREE(pos);
        
        m_configlist.count = m_configlist.count - 1;
        LUAMAN_FREE(pos);
    }
    LUAMAN_UNLOCK(m_configlist.lock);
    
    return;
}

void clumantaskmgr::setCurConfig(struct cluamanconfiglist* list)
{
    struct cluamanconfig* pos = NULL;
    struct cluamanconfig* n = NULL;

    clearCurConfig(&m_configlist);
    
    LUAMAN_LOCK(m_configlist.lock);
    list_for_each_entry_safe(pos, n, &list->head, node, struct cluamanconfig)
    {
        list_del_init(&pos->node);
        list_add_tail(&pos->node, &m_configlist.head);
        m_configlist.count = m_configlist.count + 1;

        //api_log_MsgDebug("set valid cluamanconfig, taskType:%ld, role:%lu, count:%u", pos->taskType, pos->role, m_configlist.count);
    }
    LUAMAN_UNLOCK(m_configlist.lock);

    //api_log_MsgDebug("set cluamanconfig finish");
    return;
}

struct cluamanconfiglist* clumantaskmgr::dupCurConfig()
{
    struct cluamanconfiglist* list = NULL;
    struct cluamanconfig* pos = NULL;
    struct cluamanconfig* n = NULL;
    struct cluamanconfig* node = NULL;

    LUAMAN_LOCK(m_configlist.lock);
    if (0 == m_configlist.count)
    {
        LUAMAN_UNLOCK(m_configlist.lock);
        api_log_MsgDebug("dupCurConfig failed, m_configlist.count is 0");
        return NULL;
    }

    list = (struct cluamanconfiglist*)LUAMAN_ALLOC(sizeof(struct cluamanconfiglist));
    if (NULL == list)
    {
        LUAMAN_UNLOCK(m_configlist.lock);

        api_log_MsgDebug("dupCurConfig failed, memory short");
        return NULL;
    }

    LUAMAN_MEMSET(list, 0, sizeof(struct cluamanconfiglist));
    INIT_LIST_HEAD(&list->head);
    
    list_for_each_entry_safe(pos, n, &m_configlist.head, node, struct cluamanconfig)
    {
        node = (struct cluamanconfig*)LUAMAN_ALLOC(sizeof(struct cluamanconfig));
        if (NULL == node)
        {
            LUAMAN_UNLOCK(m_configlist.lock);
            
            api_log_MsgDebug("dupCurConfig failed, memory short");
            goto failed;
        }

        LUAMAN_MEMSET(node, 0, sizeof(struct cluamanconfig));
        INIT_LIST_HEAD(&node->node);
        INIT_LIST_HEAD(&node->mixConfigList);
        INIT_LIST_HEAD(&node->bindinfoList);
        api_UpdateConfig(node, pos, 1);
        
        list_add_tail(&node->node, &list->head);
        list->count = list->count + 1;
    }

    api_log_MsgDebug("dupCurConfig success, count:%u", list->count);
    
    LUAMAN_UNLOCK(m_configlist.lock);
    return list;

failed:
    list_for_each_entry_safe(pos, n, &list->head, node, struct cluamanconfig)
    {
        list_del_init(&pos->node);
        list->count = list->count - 1;
        LUAMAN_FREE(pos);
    }
    
    LUAMAN_FREE(list);
    return NULL;
}

int clumantaskmgr::startTask(Idmsoft* dmobj)
{
    struct clumantask* pos = NULL;
    struct clumantask* n = NULL;
    struct clumantask* node = NULL;
    long hwnd = dmobj->GetMousePointWindow();
    CString strWndCls = dmobj->GetWindowClass(hwnd);
    int status = 0;
    struct cluamanconfiglist* config = dupCurConfig();
    int code = 0;
    int j = 0;
    int dmobj_index = -1;

#ifndef LUAMAN_DEBUG
    if (strWndCls != CString("Renderer") && strWndCls != CString("#32770"))
    {
        AfxMessageBox("请选择游戏窗口");
        return ERRNO_NOT_SUPPORT;
    }
#endif

    api_log_MsgDebug("start task, hwnd:%ld", hwnd);

    LUAMAN_LOCK(m_lstTask.lock);
    list_for_each_entry_safe(pos, n, &m_lstTask.head, node, struct clumantask)
    {
        if (pos->hwnd == hwnd)
        {
            api_log_MsgWarn("task is aready running, hwnd:%ld, pid:%lu, taskid:%lu",
                hwnd, pos->pid, pos->taskid);
            
            LUAMAN_UNLOCK(m_lstTask.lock);
            AfxMessageBox("重复启动游戏窗口");

            return ERRNO_EXIST;
        }
    }
    LUAMAN_UNLOCK(m_lstTask.lock);

    for (j = 1; j < DMOBJ_MAX_NUM; j++)//0给taskEntry用
    {
        if (0 == dmobj_flag[j])
        {
            dmobj_index = j;
            break;
        }
    }

    if (-1 == dmobj_index)
    {
        api_log_MsgError("dmobj exhausted, hwnd:%ld", hwnd);
        AfxMessageBox("系统规格不支持,启动失败");
        return ERRNO_NOT_SUPPORT;
    }

    node = (struct clumantask*)LUAMAN_ALLOC(sizeof(struct clumantask));
    if (NULL == node)
    {
        api_log_MsgError("start task failed, hwnd:%ld", hwnd);
        AfxMessageBox("内存不足,游戏窗口开启失败");
        
        return ERRNO_NOMEM;
    }

    LUAMAN_MEMSET(node, 0, sizeof(struct clumantask));
    node->hwnd = hwnd;
    INIT_LIST_HEAD(&node->node);
    node->config = config;
    node->ctxt.config = node->config;
    
    node->ctxt.lock = CreateMutex(NULL, FALSE, NULL);;
    if (NULL == node->ctxt.lock)
    {
        LUAMAN_FREE(node);

        api_log_MsgError("start task failed for CreateMutex, hwnd:%ld", hwnd);
        AfxMessageBox("内部错误,游戏窗口开启失败");
        
        return ERRNO_IO;
    }

    /* 启动线程 */
    status = start_Thread(NULL, 0, taskEntry, node, taskEntryExit, &node->ctxt.thread);
    if (status)
    {
        api_log_MsgError("start task failed, status:%d", status);
        return ERRNO_FAILED;
    }

    LUAMAN_LOCK(m_lstTask.lock);
    list_add_tail(&node->node, &m_lstTask.head);
    m_lstTask.count = m_lstTask.count + 1;

    dmobj_flag[dmobj_index] = 1;
    node->dmobj_index = dmobj_index;
    node->dmobj = &dmobj[dmobj_index];
    
    api_log_MsgInfo("start task success, count:%u, pid:%ld, taskid:%ld, dmobj_index:%d",
        m_lstTask.count, node->pid, node->taskid, dmobj_index);
    
    LUAMAN_UNLOCK(m_lstTask.lock);

    AfxMessageBox("游戏窗口启动成功");
    return ERRNO_SUCCESS;
}

#define KILL_GRACEFULLY

int clumantaskmgr::killTask(Idmsoft* dmobj)
{
    struct clumantask* pos = NULL;
    struct clumantask* n = NULL;
    struct clumantask* node = NULL;
    long hwnd = dmobj->GetMousePointWindow();
    CString strWndCls = dmobj->GetWindowClass(hwnd);
    int status = 0;
    DWORD code = 0;
    int maxtime = 5;
    
#ifndef LUAMAN_DEBUG
    if (strWndCls != CString("Renderer"))
    {
        AfxMessageBox("请选择游戏窗口");
        return ERRNO_NOT_SUPPORT;
    }
#endif

    api_log_MsgDebug("stop task, hwnd:%ld", hwnd);

    LUAMAN_LOCK(m_lstTask.lock);
    list_for_each_entry_safe(pos, n, &m_lstTask.head, node, struct clumantask)
    {
        if (pos->hwnd != hwnd)
        {
            continue;
        }
        
        list_del_init(&pos->node);
        m_lstTask.count = m_lstTask.count - 1;
        
        LUAMAN_UNLOCK(m_lstTask.lock);

    #ifdef KILL_GRACEFULLY
        AfxMessageBox("准备停止游戏窗口");

        stop_Thread(&pos->ctxt.thread, 0);

        dmobj_flag[pos->dmobj_index] = 0;
        
        AfxMessageBox("停止游戏窗口完成");
        api_log_MsgDebug("stop task success, hwnd:%ld", hwnd);
        
    #else
        //强制杀掉进程
        for (;;)
        {
            if (!TerminateThread(pos->ctxt.thread->taskHandle, 1))
            {
                if (!maxtime)
                {
                    api_log_MsgWarn("TerminateThread timeout, hwnd:%ld, taskHandle:%ld",
                        hwnd, pos->ctxt.thread->taskHandle);
                    AfxMessageBox("停止游戏窗口超时");
                    break;
                }

                maxtime--;
                Sleep(1000);

                api_log_MsgWarn("TerminateThread exception, hwnd:%ld, taskHandle:%ld, count:%d",
                    hwnd, pos->ctxt.thread->taskHandle, maxtime);
                
                continue;
            }

            if (!GetExitCodeThread(pos->ctxt.thread->taskHandle, &code))
            {
                if (!maxtime)
                {
                    api_log_MsgWarn("GetExitCodeThread timeout, hwnd:%ld, taskHandle:%ld",
                        hwnd, pos->ctxt.thread->taskHandle);
                    AfxMessageBox("停止游戏窗口超时");
                    break;
                }

                maxtime--;
                Sleep(1000);
                
                api_log_MsgWarn("GetExitCodeThread exception, hwnd:%ld, taskHandle:%ld, count:%d",
                    hwnd, pos->ctxt.thread->taskHandle, maxtime);
                continue;
            }

            if (1 == code)
            {
                api_log_MsgDebug("stop task success, hwnd:%ld, taskHandle:%ld, count:%d",
                    hwnd, pos->ctxt.thread->taskHandle, maxtime);
                AfxMessageBox("停止游戏窗口成功");
                break;
            }

            if (!maxtime)
            {
                api_log_MsgWarn("stop task timeout, hwnd:%ld, taskHandle:%ld",
                    hwnd, pos->ctxt.thread->taskHandle);
                AfxMessageBox("停止游戏窗口超时");
                break;
            }
            
            maxtime--;
            Sleep(1000);
        }
        
        (void)CloseHandle(pos->ctxt.thread->lock);
        pos->ctxt.thread->lock = NULL;

        api_log_MsgDebug("unbind window check, dmobj_index:%d", pos->dmobj_index);

        pos->dmobj->LockInput(0);
        if (pos->dmobj->IsBind(pos->hwnd))
        {
            api_log_MsgDebug("unbind window, hwnd:%ld, dmobj_index:%d", pos->hwnd, pos->dmobj_index);
        }
        else
        {
            api_log_MsgDebug("window has been unbinded, dmobj_index:%d", pos->dmobj_index);
        }

        pos->dmobj->UnBindWindow();
        
        /* 线程结束后要关闭线程句柄,否则线程资源不会被释放,导致窗口未解绑 */
        (void)CloseHandle(pos->ctxt.thread->taskHandle);
        pos->ctxt.thread->taskHandle = NULL;
        
        dmobj_flag[pos->dmobj_index] = 0;
        pos->dmobj = NULL;
        pos->dmobj_index = -1;
        
        //(void)PutContext(pos->config.groupid, hwnd);

        //021释放资源
        snail_FreeClientContext(hwnd);
    #endif

        LUAMAN_FREE(pos);
        pos = NULL;
        
        return ERRNO_SUCCESS;
    }
    LUAMAN_UNLOCK(m_lstTask.lock);

    AfxMessageBox("未启动游戏窗口");
    api_log_MsgWarn("stop thread failed, not exist, hwnd:%ld", hwnd);
    return ERRNO_EXIST;
}

int clumantaskmgr::killTaskAll(Idmsoft* dmobj)
{
    api_log_MsgInfo("stop all thread success");
    return ERRNO_SUCCESS;
}

int SplitString(CString& str, char split, CStringArray& strArray)
{
    strArray.RemoveAll();
    CString strTemp = str;
    int iIndex = 0;
    while (1)
    {
        iIndex = strTemp.Find(split);
        if(iIndex >= 0)
        {
            strArray.Add(strTemp.Left(iIndex));
            strTemp = strTemp.Right(strTemp.GetLength()-iIndex-1);
        }
        else
        {
            break;
        }
    }
    strArray.Add(strTemp);

    return strArray.GetSize();
}

int SplitStringWide(CString& str, char* split, CStringArray& strArray)
{
    strArray.RemoveAll();
    CString strTemp = str;
    int iIndex = 0;
    while (1)
    {
        iIndex = strTemp.Find(split);
        if(iIndex >= 0)
        {
            strArray.Add(strTemp.Left(iIndex));
            strTemp = strTemp.Right(strTemp.GetLength()-iIndex-1);
        }
        else
        {
            break;
        }
    }
    strArray.Add(strTemp);

    return strArray.GetSize();
}

int CheckKeyState(Idmsoft* dmobj, void* ctxt)
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

//#define HAVE_ALARM_LOG

int check_alarm(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int code = 0;
    char* music[3] = {"wav\\alarm_1.wav", "wav\\alarm_2.wav", "wav\alarm_3.wav"};
    VARIANT pposXe1, pposYe1;
    VARIANT pposXe2, pposYe2;
    VARIANT pposXe3, pposYe3;
    long lposXe1 = 0, lposYe1 = 0;
    long lposXe2 = 0, lposYe2 = 0;
    long lposXe3 = 0, lposYe3 = 0;
    char bmpFileName[256] = {0};
    char* alarmServer = &luaman_config->alarmServer[0];
    int alarmPort = luaman_config->alarmPort;
    int alarmHostIndex = luaman_config->alarmHostIndex;
    int nr = 0;
    
    Check_Terminate_Thread(luaman_task, dmobj);
    
    code = check_alarm_timeout(30);
    if (!code)
    {
        return ERRNO_SUCCESS;
    }

    LUAMAN_WAIT_MS(200);
    dmobj->MoveTo(5, 5);

    for (;;)
    {
        dmobj->SetDict(0, "font\\font-alarm.txt");
        code = dmobj->FindStrFast(1, 693, 385, 730, ";组队", "929292-101010", 0.9, &pposXe1, &pposYe1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposXe1 = pposXe1.lVal;
            lposYe1 = pposYe1.lVal;

        #ifdef HAVE_ALARM_LOG
            api_log_MsgDebug("私信:悄悄话:组队:查找:成功, groupid:%d, hwnd:%ld, lposXe1:%ld, lposYe1:%ld",
                luaman_config->groupid, hwnd, lposXe1, lposYe1);
        #endif
        
            dmobj->MoveTo(lposXe1, lposYe1);
            LUAMAN_WAIT_MS(100);
            dmobj->LeftClick();
            LUAMAN_WAIT_MS(400);
            dmobj->MoveTo(5, 5);
            LUAMAN_WAIT_MS(100);
        }

        dmobj->SetDict(0, "font\\font-alarm.txt");
        code = dmobj->FindStrFast(1, 693, 385, 730, ";组队", "ffffff-101010", 0.9, &pposXe2, &pposYe2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposXe2 = pposXe2.lVal;
            lposYe2 = pposYe2.lVal;

        #ifdef HAVE_ALARM_LOG
            api_log_MsgDebug("私信:悄悄话:组队:点击:成功, groupid:%d, hwnd:%ld, lposXe2:%ld, lposYe2:%ld",
                luaman_config->groupid, hwnd, lposXe2, lposYe2);
        #endif
            break;
        }

        nr++;
        if (nr > 3)
        {
            return ERRNO_SUCCESS;
        }
        
        Check_Terminate_Thread(luaman_task, dmobj);
    }

    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(200);

    //查找是否有'From'字样,有就报警
    {
        dmobj->SetDict(0, "font\\font-alarm.txt");
        //code = dmobj->FindStrFast(6, 597, 385, 700, "From", "ff6060-101010", 0.9, &pposXe3, &pposYe3);
        code = dmobj->FindStrFast(6, 456, 385, 700, "From", "ff6060-101010", 0.9, &pposXe3, &pposYe3);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposXe3 = pposXe3.lVal;
            lposYe3 = pposYe3.lVal;

            alarm_report(alarmServer, alarmPort, alarmHostIndex);
            //dmobj->Capture(0, 27, 1279, 826, bmpFileName);

        #ifdef HAVE_ALARM_LOG
            api_log_MsgDebug("私信:悄悄话:管理员:消息:接收:查找:成功, groupid:%d, hwnd:%ld, lposXe3:%ld, lposYe3:%ld",
                luaman_config->groupid, hwnd, lposXe3, lposYe3);
        #endif
            
            return ERRNO_ALARM;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
    }
    
    return ERRNO_SUCCESS;
}

int check_connection_status(Idmsoft* dmobj, void* ctxt,
    int (*callback)(Idmsoft* dmobj, void* ctxt),
    void (*lockonoff)(int flag))
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int waitCopytime = luaman_config->waitCopytime;
    int code = 0;
    long cpid = dmobj->GetWindowProcessId(hwnd);
    CString hwnds = dmobj->EnumWindowByProcess("dekaron.exe", "", "#32770", 27);
    CStringArray hwndArray;
    char* strhwnds = hwnds.GetBuffer(BUFFER_MAX);
    int count = 0;
    int idx = 0;
    int ihwnd = 0;
    long ipid = 0;
    VARIANT pposX, pposY;
    long lposX = 0, lposY = 0;
    int status = 0;

    return ERRNO_SUCCESS;//取消功能
    
    api_log_MsgDebug("检查:窗口:掉线:开始, hwnd:%ld, cpid:%ld", hwnd, cpid);
    api_log_MsgDebug("游戏:窗口:枚举, cpid:%ld, strhwnds:\"%s\"", cpid, strhwnds);
    hwnds.ReleaseBuffer();

    if (hwnds == CString(""))
    {
        api_log_MsgDebug("窗口:未掉线");
        return ERRNO_SUCCESS;
    }
    
    count = SplitStringWide(hwnds, ",", hwndArray);
    if (count < 1)
    {
        api_log_MsgDebug("窗口:分割:错误, count:%d", count);
        return ERRNO_SUCCESS;
    }
    
    api_log_MsgDebug("窗口:分割:成功, count:%d", count);
    
    for (idx = 0; idx < count; idx++)
    {
        ihwnd = atol(hwndArray.GetAt(idx));
        ipid = dmobj->GetWindowProcessId(ihwnd);

        api_log_MsgDebug(" ");
        api_log_MsgDebug("idx          : %d", idx);
        api_log_MsgDebug("ihwnd        : %ld", ihwnd);
        api_log_MsgDebug("ipid         : %ld", ipid);
        api_log_MsgDebug("cpid         : %ld", cpid);
        
        if (cpid != ipid)
        {
            continue;
        }

        api_log_MsgDebug("当前:窗口:掉线");

        if (lockonoff)
        {
            lockonoff(1);
        }
        
        //解绑现有的窗口
        dmobj->LockInput(0);
        dmobj->UnBindWindow();

        api_log_MsgDebug("当前:窗口:后台:解绑");

        //窗口置顶并激活
        ::SetWindowPos((HWND)ihwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREPOSITION );
        ::ShowWindow((HWND)ihwnd, SW_RESTORE | SW_SHOW);
        
        api_log_MsgDebug("当前:窗口:置顶");
        
        //前台绑定弹窗
        code = dmobj->BindWindow(ihwnd, "normal", "normal", "normal", 0);
        if (1 == code)
        {
            api_log_MsgDebug("当前:窗口:前台:绑定:成功, ihwnd:%ld", ihwnd);

        //#define FRONT_DEBUG
        #ifdef FRONT_DEBUG
            VARIANT posX1, posY1;
            VARIANT posX2, posY2;
            VARIANT width, height;
            long lposX1 = 0, lposY1 = 0;
            long lposX2 = 0, lposY2 = 0;
            long lwidth = 0, lheight = 0;
            
            code = dmobj->GetClientRect(ihwnd, &posX1, &posY1, &posX2, &posY2);
            if (code)
            {
                lposX1 = posX1.lVal;
                lposY1 = posY1.lVal;
                lposX2 = posX2.lVal;
                lposY2 = posY2.lVal;

                api_log_MsgDebug("GetClientRect:成功");
                api_log_MsgDebug("lposX1:%ld, lposY1:%ld, lposX2:%ld, lposY2:%ld",
                    lposX1, lposY1, lposX2, lposY2);
                
                code = dmobj->GetClientSize(ihwnd, &width, &height);
                if (code)
                {
                    lwidth = width.lVal;
                    lheight = height.lVal;

                    api_log_MsgDebug("GetClientSize:成功");
                    api_log_MsgDebug("lwidth:%ld, lheight:%ld", lwidth, lheight);
                }
                else
                {
                    api_log_MsgError("GetClientSize:失败");
                }
            }
            else
            {
                api_log_MsgError("GetClientRect:失败");
            }
        #endif

            api_log_MsgDebug("确定:查找:开始");
            
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                dmobj->MoveTo(5, 5);
                LUAMAN_WAIT_MS(200);
    
                dmobj->SetDict(0, "font\\font-chckconn.txt");
                code = dmobj->FindStrFast(0, 0, 1279, 799, "确定", "000000-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("确定:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                api_log_MsgDebug("确定:查找:失败");
                Delay(500);
            }

            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);

            dmobj->UnBindWindow();
            api_log_MsgDebug("当前:窗口:回车:成功, ihwnd:%ld", ihwnd);
        }
        else
        {
            api_log_MsgError("当前:窗口:前台:绑定:失败, ihwnd:%ld", ihwnd);
        }

        //重新绑定
        if (callback)
        {
            code = callback(dmobj, ctxt);
            if (ERRNO_SUCCESS != code)
            {
                api_log_MsgError("当前:窗口:恢复:后台:绑定:失败, code:%d", code);
            }
            else
            {
                api_log_MsgDebug("当前:窗口:恢复:后台:绑定:成功, hwnd:%ld", hwnd);
            }
        }

        if (lockonoff)
        {
            lockonoff(0);
        }
        
        status = ERRNO_OFFLINE;
        break;
    }
    
    api_log_MsgDebug("检查:窗口:掉线:完成, status:%d", status);
    return status;
}

int GetCurPosXY(Idmsoft* dmobj, void* ctxt, long* lposX, long* lposY, CString* strPos)
{
    CString strPosXY;
    CStringArray posXY;
    int xyc = 0;
    
    dmobj->SetDict(0, "font\\font-posxy.txt");
    strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (CString("") == strPosXY)
    {
        return ERRNO_FAILED;
    }

    xyc = SplitString(strPosXY, ':', posXY);
    if (xyc < 2)
    {
        return ERRNO_FAILED;
    }
    
    if (lposX)*lposX = atol(posXY.GetAt(0));
    if (lposY)*lposY = atol(posXY.GetAt(1));
    if (strPos)*strPos = strPosXY;
    
    return ERRNO_SUCCESS;
}

static int common_startwork(struct cluamanaccountinfo* accountinfo, int roleno, struct common_loop_ctxt_t* clctxt)
{
    Idmsoft* dmobj = clctxt->dmobj;
    void* ctxt = clctxt->ctxt;
    int status = 0;
    
    if (clctxt->pre_work)
    {
        status = clctxt->pre_work(accountinfo, roleno, clctxt);
        if (status)
        {
            return status;
        }
    }

    if (clctxt->do_work)
    {
        status = clctxt->do_work(accountinfo, roleno, clctxt);
        if (status)
        {
            return status;
        }
    }
    
    if (clctxt->post_work)
    {
        status = clctxt->post_work(accountinfo, roleno, clctxt);
        if (status)
        {
            return status;
        }
    }

    return 0;
}

int common_prework(struct cluamanaccountinfo* accountinfo, int roleno, struct common_loop_ctxt_t* clctxt)
{
    Idmsoft* dmobj = clctxt->dmobj;
    void* ctxt = clctxt->ctxt;
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
        {'0', BMP_EXCOMMON_DIR"0.bmp"},
        {'1', BMP_EXCOMMON_DIR"1.bmp"},
        {'2', BMP_EXCOMMON_DIR"2.bmp"},
        {'3', BMP_EXCOMMON_DIR"3.bmp"},
        {'4', BMP_EXCOMMON_DIR"4.bmp"},
        {'5', BMP_EXCOMMON_DIR"5.bmp"},
        {'6', BMP_EXCOMMON_DIR"6.bmp"},
        {'7', BMP_EXCOMMON_DIR"7.bmp"},
        {'8', BMP_EXCOMMON_DIR"8.bmp"},
        {'9', BMP_EXCOMMON_DIR"9.bmp"},
    };
    int j = 0;
    char* path = NULL;
    int dotask_num = 0;
    time_t check_conn_start;
    time_t check_conn_stop;
    double check_conn_deltaTime = 0.0;
    //角色名字数组={A?, B?, C?,...,}
    //char* role_names[] = EXSIGNLE_ROLES();
    char* role_name = NULL;
    struct posxy_t posxys[] = {
        {1219, 608},
        {1219, 608},
        {1219, 608},
        {1105, 607},
        {1105, 607},
        {1105, 607},
    };

    luaman_config->roleno = roleno;
    luaman_config->user_name = accountinfo->user_name;
    //luaman_config->enter_with_role = enter_with_role;
    
    if (roleno >= EXCOMMON_ROLES_NR || roleno < 0)
    {
        api_log_MsgError("任务:副本:角色:%d:参数:错误", roleno);
        return SNAIL_ERRNO_INVALID;
    }

    api_log_MsgDebug("副本:角色:任务:执行:开始");
    api_log_MsgDebug("user       : %s", accountinfo->user_name);
    api_log_MsgDebug("passwd     : %s", accountinfo->passwd);
    api_log_MsgDebug("passwdex   : %s", accountinfo->passwdex);
    api_log_MsgDebug("roleno     : %d", roleno);

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
        
        dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-login.txt");
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
    
        dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-login.txt");
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
        
        dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-security.txt");
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
        dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-loginerr.txt");
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

                dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-loginerr.txt");
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
            
            dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-security.txt");
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
    
        dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-security.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "请谨慎输入二级密码", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("二级:密码:确认:成功");
            break;
        }

        check_conn_stop = time(NULL);
        check_conn_deltaTime = difftime(check_conn_stop, check_conn_start);
        
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;
                
        api_log_MsgDebug("二级:密码:确认:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(2000);
    }
    
    api_log_MsgDebug("进入:游戏:成功:准备:开始");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);

    int clicknr = 0;
    
    /////////////////////////////////////////////////////////////////////////////////
    if (roleno > 0 && clctxt->lfi_pri_size > 0 && NULL != clctxt->lfi_pri)
    {
        //参考taskExSignle_dowork.cpp::start_work
        struct login_font_info_t* lfi_pri = clctxt->lfi_pri;
        int lfi_pri_size = clctxt->lfi_pri_size;
        
        for (;;)
        {
            Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            dmobj->MoveTo(5, 5);
            Delay(500);

            for (i = 0; i < lfi_pri_size; i++)
            {
                if (strcmp(lfi_pri[i].login, accountinfo->user_name))
                {
                    api_log_MsgDebug("账号:%s:匹配:失败", accountinfo->user_name);
                    continue;
                }

                role_name = lfi_pri[i + roleno].word;
                
                api_log_MsgDebug("账号:%s:角色:%s:匹配:成功", accountinfo->user_name, role_name);

                for (j = 0; j < LUAMAN_DIM(lfi_pri[i + roleno].color_offset); j++)
                {
                    dmobj->SetDict(0, lfi_pri[i + roleno].font);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, role_name, lfi_pri[i + roleno].color_offset[j], 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("账号:%s:角色:%d:名称:%s:颜色:%s:查找:成功, lposX:%ld, lposY:%ld",
                            accountinfo->user_name, roleno, role_name, lfi_pri[i + roleno].color_offset[j],lposX, lposY);
                        goto label_click_role;
                    }

                    api_log_MsgDebug("账号:%s:角色:%d:名称:%s:字库:%s:颜色:%s:查找:失败",
                        accountinfo->user_name, roleno, role_name, lfi_pri[i + roleno].font,
                        lfi_pri[i + roleno].color_offset[j]);
                }

                api_log_MsgDebug("账号:%s:角色:%s:查找:失败", accountinfo->user_name, role_name);
                break;
            }

            //点击三角形箭头翻页再继续查找
            dmobj->MoveTo(posxys[clicknr].x, posxys[clicknr].y);
            Delay(300);
            dmobj->LeftClick();
            Delay(300);
            dmobj->MoveTo(5, 5);
            Delay(500);
        
            clicknr = (clicknr + 1) % LUAMAN_DIM(posxys);
            
            api_log_MsgDebug("账号:%s:角色:%d:名称:%s:查找:失败:翻页", accountinfo->user_name, roleno, role_name);
            Delay(500);
        }

    label_click_role:
        //dmobj->Capture(0, 27, 1279, 826, "cap\\sel_role0.bmp");
        
        //点击选择角色
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
    }

    /////////////////////////////////////////////////////////////////////////////////
    //点击进入游戏
    dmobj->MoveTo(640, 670);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(200);

    i = 0;

    api_log_MsgDebug("点击:进入游戏:成功");
    return 0;
}

int common_postwork(struct cluamanaccountinfo* accountinfo, int roleno, struct common_loop_ctxt_t* clctxt)
{
    Idmsoft* dmobj = clctxt->dmobj;
    void* ctxt = clctxt->ctxt;
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

    //退出副本
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        code = check_alarm(dmobj, ctxt);
        if (ERRNO_ALARM == code)
        {
            Delay(1000);
            api_log_MsgDebug("准备:重新登录:报警:检测:成功");
            continue;
        }
        
        //点击 1262:747
        dmobj->MoveTo(1262, 747);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-logout.txt");
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

        dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-saveconf.txt");
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
        
        dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-saveconf.txt");
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

int common_mainloop(struct common_loop_ctxt_t* clctxt)
{
    struct list_head* accountList = clctxt->accountList;
    Idmsoft* dmobj = clctxt->dmobj;
    void* ctxt = clctxt->ctxt;
    char* configXML = clctxt->configXML;
    
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
    
    struct exCommonConfig dotask_config;
    int lastnr = 0;
    int maxroleno = luaman_config->copyrolenr + 1;
    int roleno = 0;
    int bok = 0;
    int exitflag = 0;

    api_log_MsgDebug("mainloop:开始, maxroleno:%d", maxroleno);
    
    bok = load_exCommonConfig(bindWnd, std::string(configXML), &dotask_config);
    
    //拷贝进来,便于副本里面循环检查时输出用户名信息
    luaman_task->bindWnd = bindWnd;
    
    print_exCommonConfig(&dotask_config);

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
            
            status = common_startwork(apos, roleno, clctxt);
            if (ERRNO_SUCCESS != status)
            {
                dotask_config.last_roleno = roleno % maxroleno;
                exitflag = 1;
                
                api_log_MsgError("任务:%s:角色:%d:失败, status:%d", apos->user_name, roleno, status);
                break;
            }

            dotask_config.last_roleno = (roleno + 1) % maxroleno;

            //每个角色完了也要记录
            (void)save_exCommonConfig(std::string(configXML), &dotask_config);
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
        (void)save_exCommonConfig(std::string(configXML), &dotask_config);
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
    save_exCommonConfig(std::string(configXML), &dotask_config);

    api_log_MsgDebug("任务:%s:窗口%d:主机%d:任务:账号:%d:角色:%d:完成",
        apos->user_name, bindWnd + 1, curBindHostIdx + 1, dotask_config.last_nr, dotask_config.last_roleno);
    
    return ERRNO_SUCCESS;
}

int common_checkcopyfull(Idmsoft* dmobj, void* ctxt)
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
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
        
    //检查副本中是否有大boss,如果是则会出现"因此无法入场"
    dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-reenter.txt");
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
            dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-reenter.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;

                api_log_MsgDebug("副本中有大boss进不去:确认:查找:成功, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
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

            dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-reenter.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功");
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            LUAMAN_WAIT_MS(500);
            api_log_MsgDebug("副本中有大boss进不去:确认:点击:失败");
        }

        api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功:重新对话");
        return SNAIL_ERRNO_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //没有进入副本,也没有出现"副本中有大boss进不去",则检查是否有"副本地下城满"
    dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-copyfull.txt");
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

            dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-copyfull.txt");
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
            check_alarm(dmobj, ctxt);
            api_log_MsgDebug("副本地下城已满:确认:点击:失败");
            LUAMAN_WAIT_MS(500);
        }

        api_log_MsgDebug("副本地下城已满:确认:点击:成功:重新对话");
        return SNAIL_ERRNO_AGAIN;;
    }
    
    return SNAIL_ERRNO_SUCCESS;
}

int common_check_copy_access(Idmsoft* dmobj, void* ctxt)
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

    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-CAS.txt");
    code = dmobj->FindStrFast(0, 27, 1279, 826, "才能入场", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("才能入场:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);

        for (;;)
        {
            //点击确认
            dmobj->MoveTo(lposX + 8, lposY + 86);// 罗爱平 : 修改确认相对于才能入场的位置偏移 2023/9/10 22:25:11
            Delay(300);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(300);
            
            dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-CAS.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "才能入场", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("才能入场:点击:成功:重新对话");
                return ERRNO_AGAIN;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            api_log_MsgDebug("才能入场:点击:失败");
            Delay(500);
        }
    }
    
    return SNAIL_ERRNO_SUCCESS;
}

int common_check_people_noenough(Idmsoft* dmobj, void* ctxt)
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

    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-peopleleak.txt");
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
            
            dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-peopleleak.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "人员不足", "ffffff-202020", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("人员不足:点击:成功:重新对话");
                return ERRNO_AGAIN;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            api_log_MsgDebug("人员不足:点击:失败");
            Delay(500);
        }
    }
    
    return SNAIL_ERRNO_SUCCESS;
}

int common_exitgroup(Idmsoft* dmobj, void* ctxt)
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
        
        dmobj->SetDict( 0, FONT_EXCOMMON_DIR"font-auxiend.txt");
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
        if (retrynr > 5)
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
        
        dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-auxiend.txt");
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
        
        dmobj->SetDict(0, FONT_EXCOMMON_DIR"font-auxiend.txt");
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
        
        dmobj->SetDict( 0, FONT_EXCOMMON_DIR"font-auxiend.txt");
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

