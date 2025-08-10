#ifndef __TASK1853_CLIENT_MANAGER_H__
#define __TASK1853_CLIENT_MANAGER_H__


INT32 Tz1853_InitClientManager(INT32 count);
void Tz1853_ClearClientManager(void);
INT32 Tz1853_AllocClientContext(INT32 hwnd);
void Tz1853_PutClientContext(INT32 hwnd);
void Tz1853_FreeClientContext(INT32 hwnd);
INT32 Tz1853_GetClientTaskId(INT32 hwnd, INT32* pTaskId);
INT32 Tz1853_UpdateClientTaskId(INT32 hwnd, INT32 taskId);
INT32 Tz1853_GetClientContextStatus(INT32 hwnd, TASK1853_TASK_STATUS_E* pTaskStatus);
INT32 Tz1853_UpdateClientContextStatus(INT32 hwnd, TASK1853_TASK_STATUS_E taskStatus);

void Tz1853_ShowClientManager(void);

#endif
