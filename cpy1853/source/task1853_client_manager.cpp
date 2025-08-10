#include <process.h>
#include <windows.h>
#include "datatype.h"
#include "task1853_errno.h"
#include "xlog.h"
#include "task1853_common.h"
#include "task1853_client_context.h"
#include "task1853_client_manager.h"

#define SNAIL_LOG_DEBUG api_log_MsgDebug
#define SNAIL_LOG_ERROR api_log_MsgError

typedef struct
{
    CRITICAL_SECTION lock;
    TZ1853_CLIENT_CONTEXT_S context_array[SNAIL_CONTEXT_NUM_MAX];
    INT32 count;
    INT32 free_count;
}SNAIL_CLIENT_MANAGER_S;

static SNAIL_CLIENT_MANAGER_S gSnailClientManager;

INT32 Tz1853_InitClientManager(INT32 count)
{
    SNAIL_CLIENT_MANAGER_S* manager = &gSnailClientManager;
    INT32 j = 0;
    TZ1853_CLIENT_CONTEXT_S* context = NULL;
    
    if (count < 1 || count > SNAIL_CONTEXT_NUM_MAX)
    {
        SNAIL_LOG_ERROR("Init client manager failed, count:%d.", count);
        return SNAIL_ERRNO_INVALID;
    }

    InitializeCriticalSection(&manager->lock);
    manager->count = count;
    manager->free_count = count;
    
    for (j = 0; j < SNAIL_CONTEXT_NUM_MAX; ++j)
    {
        context = &manager->context_array[j];
        context->hwnd = -1;
        context->taskId = -1;
        context->task_status = TASK1853_TASK_STATUS_UNKNOWN;
    }

    SNAIL_LOG_DEBUG("Init client manager success, count:%d.", count);
    return SNAIL_ERRNO_SUCCESS;
}

void Tz1853_ClearClientManager(void)
{
    SNAIL_CLIENT_MANAGER_S* manager = &gSnailClientManager;
    INT32 j = 0;
    TZ1853_CLIENT_CONTEXT_S* context = NULL;

    EnterCriticalSection(&manager->lock);
    
    manager->count = 0;
    manager->free_count = 0;
    
    for (j = 0; j < SNAIL_CONTEXT_NUM_MAX; ++j)
    {
        context = &manager->context_array[j];
        context->hwnd = -1;
        context->taskId = -1;
        context->task_status = TASK1853_TASK_STATUS_UNKNOWN;
    }

    LeaveCriticalSection(&manager->lock);
    
    SNAIL_LOG_DEBUG("Clear client manager success.");
    return;
}

INT32 Tz1853_AllocClientContext(INT32 hwnd)
{
    SNAIL_CLIENT_MANAGER_S* manager = &gSnailClientManager;
    INT32 j = 0;
    TZ1853_CLIENT_CONTEXT_S* context = NULL;
    INT32 empty_index = -1;
    INT32 free_count = 0;

    if (hwnd <= 0)
    {
        SNAIL_LOG_ERROR("Get client context failed: invalid argument, hwnd:%d.",
            hwnd);
        return SNAIL_ERRNO_INVALID;
    }
    
    EnterCriticalSection(&manager->lock);
    
    if (manager->free_count <= 0)
    {
        LeaveCriticalSection(&manager->lock);

        SNAIL_LOG_ERROR("Get client context failed: no space left, hwnd:%d.",
            hwnd);
        return SNAIL_ERRNO_NOSPACE;
    }

    for (j = 0; j < SNAIL_CONTEXT_NUM_MAX; ++j)
    {
        context = &manager->context_array[j];
        if (hwnd == context->hwnd)
        {
            LeaveCriticalSection(&manager->lock);

            SNAIL_LOG_ERROR("Get client context failed: exist, hwnd:%d.",
                hwnd);
            return SNAIL_ERRNO_EXIST;
        }

        if (-1 == context->hwnd)
        {
            if (-1 == empty_index)
            {
                empty_index = j;
            }
        }
    }

    context = &manager->context_array[empty_index];
    context->hwnd = hwnd;
    context->task_status = TASK1853_TASK_STATUS_START_PENDING;

    manager->free_count = manager->free_count - 1;
    free_count = manager->free_count;
    
    LeaveCriticalSection(&manager->lock);

    SNAIL_LOG_DEBUG("Get client context success, hwnd:%d, free_count:%d, empty_index:%d.",
        hwnd, free_count, empty_index);

    return SNAIL_ERRNO_SUCCESS;
}

void Tz1853_PutClientContext(INT32 hwnd)
{
    SNAIL_CLIENT_MANAGER_S* manager = &gSnailClientManager;
    INT32 j = 0;
    TZ1853_CLIENT_CONTEXT_S* context = NULL;

    if (hwnd <= 0)
    {
        SNAIL_LOG_ERROR("Put client context failed: invalid argument, hwnd:%d.",
            hwnd);
        return;
    }
    
    EnterCriticalSection(&manager->lock);
    for (j = 0; j < SNAIL_CONTEXT_NUM_MAX; ++j)
    {
        context = &manager->context_array[j];
        if (hwnd == context->hwnd)
        {
            if (TASK1853_TASK_STATUS_RUNNING == context->task_status)
            {
                context->task_status = TASK1853_TASK_STATUS_STOP_PENDING;
            }
            break;
        }
    }
    LeaveCriticalSection(&manager->lock);
    
    return;
}

void Tz1853_FreeClientContext(INT32 hwnd)
{
    SNAIL_CLIENT_MANAGER_S* manager = &gSnailClientManager;
    INT32 j = 0;
    TZ1853_CLIENT_CONTEXT_S* context = NULL;

    if (hwnd <= 0)
    {
        SNAIL_LOG_ERROR("Free client context failed: invalid argument, hwnd:%d.",
            hwnd);
        return;
    }
    
    EnterCriticalSection(&manager->lock);
    for (j = 0; j < SNAIL_CONTEXT_NUM_MAX; ++j)
    {
        context = &manager->context_array[j];
        if (hwnd == context->hwnd)
        {
            SNAIL_LOG_DEBUG("Free client context success, hwnd:%d, taskId:%d, task_status:%d",
                context->hwnd, context->taskId, context->task_status);
            
            context->hwnd = -1;
            context->taskId = -1;
            context->task_status = TASK1853_TASK_STATUS_UNKNOWN;

            manager->free_count = manager->free_count + 1;
            break;
        }
    }
    LeaveCriticalSection(&manager->lock);
    
    return;
}

INT32 Tz1853_GetClientContextStatus(INT32 hwnd, TASK1853_TASK_STATUS_E* pTaskStatus)
{
    SNAIL_CLIENT_MANAGER_S* manager = &gSnailClientManager;
    INT32 j = 0;
    TZ1853_CLIENT_CONTEXT_S* context = NULL;
    TASK1853_TASK_STATUS_E status = TASK1853_TASK_STATUS_UNKNOWN;
    INT32 flag = 0;
    
    if (hwnd <= 0)
    {
        SNAIL_LOG_ERROR("Get client context status failed: invalid argument, hwnd:%d.",
            hwnd);
        return SNAIL_ERRNO_INVALID;
    }
    
    EnterCriticalSection(&manager->lock);
    for (j = 0; j < SNAIL_CONTEXT_NUM_MAX; ++j)
    {
        context = &manager->context_array[j];
        if (hwnd == context->hwnd)
        {
            status = context->task_status;
            flag = 1;
            break;
        }
    }
    LeaveCriticalSection(&manager->lock);

    if (0 == flag)
    {
        SNAIL_LOG_ERROR("Get client context status failed: not exist, hwnd:%d.",
            hwnd);
        
        return SNAIL_ERRNO_NOT_EXIST;
    }
    
    *pTaskStatus = status;
    
    return SNAIL_ERRNO_SUCCESS;
}

INT32 Tz1853_GetClientTaskId(INT32 hwnd, INT32* pTaskId)
{
    SNAIL_CLIENT_MANAGER_S* manager = &gSnailClientManager;
    INT32 j = 0;
    TZ1853_CLIENT_CONTEXT_S* context = NULL;
    INT32 taskId = 0;
    INT32 flag = 0;
    
    if (hwnd <= 0)
    {
        SNAIL_LOG_ERROR("Get client context taskid failed: invalid argument, hwnd:%d.",
            hwnd);
        return SNAIL_ERRNO_INVALID;
    }
    
    EnterCriticalSection(&manager->lock);
    for (j = 0; j < SNAIL_CONTEXT_NUM_MAX; ++j)
    {
        context = &manager->context_array[j];
        if (hwnd == context->hwnd)
        {
            taskId = context->taskId;
            flag = 1;
            break;
        }
    }
    LeaveCriticalSection(&manager->lock);

    if (0 == flag)
    {
        SNAIL_LOG_ERROR("Get client context taskid failed: not exist, hwnd:%d.",
            hwnd);
        
        return SNAIL_ERRNO_NOT_EXIST;
    }
    
    *pTaskId = taskId;
    
    return SNAIL_ERRNO_SUCCESS;
}


INT32 Tz1853_UpdateClientTaskId(INT32 hwnd, INT32 taskId)
{
    SNAIL_CLIENT_MANAGER_S* manager = &gSnailClientManager;
    INT32 j = 0;
    TZ1853_CLIENT_CONTEXT_S* context = NULL;
    TASK1853_TASK_STATUS_E status = TASK1853_TASK_STATUS_UNKNOWN;
    INT32 flag = 0;
    
    if (hwnd <= 0)
    {
        SNAIL_LOG_ERROR("Update client context taskid failed: invalid argument, hwnd:%d.",
            hwnd);
        return SNAIL_ERRNO_INVALID;
    }
    
    EnterCriticalSection(&manager->lock);
    for (j = 0; j < SNAIL_CONTEXT_NUM_MAX; ++j)
    {
        context = &manager->context_array[j];
        if (hwnd == context->hwnd)
        {
            context->taskId = taskId;
            flag = 1;
            break;
        }
    }
    LeaveCriticalSection(&manager->lock);

    return (0 == flag)? SNAIL_ERRNO_NOT_EXIST : SNAIL_ERRNO_SUCCESS;
}

INT32 Tz1853_UpdateClientContextStatus(INT32 hwnd, TASK1853_TASK_STATUS_E taskStatus)
{
    SNAIL_CLIENT_MANAGER_S* manager = &gSnailClientManager;
    INT32 j = 0;
    TZ1853_CLIENT_CONTEXT_S* context = NULL;
    TASK1853_TASK_STATUS_E status = TASK1853_TASK_STATUS_UNKNOWN;
    INT32 flag = 0;
    
    if (hwnd <= 0)
    {
        SNAIL_LOG_ERROR("Update client context status failed: invalid argument, hwnd:%d.",
            hwnd);
        return SNAIL_ERRNO_INVALID;
    }
    
    EnterCriticalSection(&manager->lock);
    for (j = 0; j < SNAIL_CONTEXT_NUM_MAX; ++j)
    {
        context = &manager->context_array[j];
        if (hwnd == context->hwnd)
        {
            context->task_status = taskStatus;
            flag = 1;
            break;
        }
    }
    LeaveCriticalSection(&manager->lock);

    return (0 == flag)? SNAIL_ERRNO_NOT_EXIST : SNAIL_ERRNO_SUCCESS;
}

void Tz1853_ShowClientManager(void)
{
    SNAIL_CLIENT_MANAGER_S* manager = &gSnailClientManager;
    INT32 j = 0;
    TZ1853_CLIENT_CONTEXT_S* context = NULL;

    EnterCriticalSection(&manager->lock);

    SNAIL_LOG_DEBUG("manager->count                   : %d", manager->count);
    SNAIL_LOG_DEBUG("manager->free_count              : %d", manager->free_count);
    
    for (j = 0; j < SNAIL_CONTEXT_NUM_MAX; ++j)
    {
        context = &manager->context_array[j];

        SNAIL_LOG_DEBUG("context[%d]->hwnd                : %d", j, context->hwnd);
        SNAIL_LOG_DEBUG("context[%d]->task_status         : %d", j, context->task_status);
    }
    LeaveCriticalSection(&manager->lock);
    
    return;
}


