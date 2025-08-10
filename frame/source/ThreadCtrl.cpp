#include <stdio.h>
#include <stdlib.h>
#include <process.h>
//#include <windows.h>
#include "stdafx.h"
#include "ThreadCtrl.h"

#define THREAD_EXIT     (0)
#define THREAD_RUNNING  (1)
#define THREAD_INIT     (THREAD_EXIT)

static unsigned __stdcall threadRoutine(void* arg)
{
    THREAD_S* thread = (THREAD_S*)arg;
    DWORD code = WAIT_FAILED;
    DWORD processId = GetCurrentProcessId();
    DWORD taskId = GetCurrentThreadId();

    code = WaitForSingleObject(thread->lock, INFINITE);
    if (WAIT_OBJECT_0 != code)
    {
        thread->flag = THREAD_EXIT;
        return 0;
    }

    thread->flag = THREAD_RUNNING;
    if (0 == ReleaseMutex(thread->lock))
    {
        thread->flag = THREAD_EXIT;
        return 0;
    }

    thread->taskId = taskId;
    thread->routine(thread->arg);

    code = WaitForSingleObject(thread->lock, INFINITE);

    thread->flag = THREAD_EXIT;

    if (WAIT_OBJECT_0 == code)
    {
        (void)ReleaseMutex(thread->lock);
    }

    return 1;
}

int start_Thread(void* security, unsigned int stackSize, unsigned (*routine)(void* arg),
    void* arg, void (*exit)(void* arg, unsigned char), THREAD_S** thread)
{
    //uintptr_t handle = NULL;
    unsigned long handle = NULL;
    THREAD_S* pThread = NULL;
    DWORD code = WAIT_FAILED;
    unsigned taskId = 0;
    HANDLE lock = NULL;

    if (!routine)
    {
        return -1;
    }

    lock = CreateMutex(NULL, FALSE, NULL);
    if (!lock)
    {
        return -1;
    }

    pThread = (THREAD_S*)malloc(sizeof(THREAD_S));
    if (!pThread)
    {
        CloseHandle(lock);
        return -1;
    }

    memset(pThread, 0, sizeof(THREAD_S));
    pThread->arg = arg;
    pThread->routine = routine;
    pThread->flag = THREAD_INIT;
    pThread->exit = exit;
    pThread->lock = lock;

    /*
    * 工程属性-c/c++-预编译的头文件-不使用预补偿页眉,否则会报错找不到符号_beginthreadex
    */
    handle = _beginthreadex(security, stackSize, threadRoutine, pThread, CREATE_SUSPENDED, &taskId);
    if (!handle)
    {
        CloseHandle(lock);
        memset(pThread, 0, sizeof(THREAD_S));
        free(pThread);
        return -1;
    }

    pThread->taskHandle = (HANDLE)handle;
    pThread->taskId = taskId;
    
    code = ResumeThread((HANDLE)handle);
    if (-1 == code)
    {
        CloseHandle(lock);
        memset(pThread, 0, sizeof(THREAD_S));
        free(pThread);
        return -1;
    }

    if (thread)
    {
        *thread = pThread;
    }

    return 0;
}

void stop_Thread(THREAD_S** thread, unsigned char force)
{
    THREAD_S* pThread = NULL;
    DWORD code = WAIT_FAILED;
    DWORD taskId = 0;

    if (!thread || !(pThread = *thread))
    {
        return;
    }

    //code = WaitForSingleObject(pThread->lock, INFINITE);
    //if (WAIT_OBJECT_0 != code)
    //{
    //    return;
    //}

    taskId = pThread->taskId;

    if (THREAD_RUNNING == pThread->flag)
    {
        if (pThread->exit)
        {
            pThread->exit(pThread->arg, force);
        }

        pThread->arg = NULL;
    }

    //ReleaseMutex(pThread->lock);

    if (force)
    {
        return;
    }

    /*for (;;)
    {
        code = WaitForSingleObject(pThread->lock, INFINITE);
        if (WAIT_OBJECT_0 != code)
        {
            continue;
        }

        if (THREAD_EXIT == pThread->flag)
        {
            //(void)ReleaseMutex(pThread->lock);
            break;
        }

        (void)ReleaseMutex(pThread->lock);
    }*/

    (void)CloseHandle(pThread->lock);
    memset(pThread, 0, sizeof(THREAD_S));
    free(pThread);
    *thread = NULL;

    return;
}

