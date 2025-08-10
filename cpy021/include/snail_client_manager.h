#ifndef __SNAIL_CLIENT_MANAGER_H__
#define __SNAIL_CLIENT_MANAGER_H__


INT32 snail_InitClientManager(INT32 count);
void snail_ClearClientManager(void);
INT32 snail_AllocClientContext(INT32 hwnd);
void snail_PutClientContext(INT32 hwnd);
void snail_FreeClientContext(INT32 hwnd);
INT32 snail_GetClientTaskId(INT32 hwnd, INT32* pTaskId);
INT32 snail_UpdateClientTaskId(INT32 hwnd, INT32 taskId);
INT32 snail_GetClientContextStatus(INT32 hwnd, SNAIL_TASK_STATUS_E* pTaskStatus);
INT32 snail_UpdateClientContextStatus(INT32 hwnd, SNAIL_TASK_STATUS_E taskStatus);

void snail_ShowClientManager(void);

#endif
