#ifndef __THREAD_CTRL_H__
#define __THREAD_CTRL_H__

enum
{
    KILL_THREAD_GRACEFULLY = 1,
    KILL_THREAD_FORCE      = 2,
};

typedef struct TAG_THREAD_S
{
    HANDLE taskHandle;
    DWORD taskId;
    unsigned char flag;
    HANDLE lock;
    unsigned (*routine)(void* arg);
    void (*exit)(void* arg, unsigned char force);
    void* arg;
}THREAD_S;

int start_Thread(void* security, unsigned int stackSize, unsigned (*routine)(void* arg),
    void* arg, void (*exit)(void* arg, unsigned char), THREAD_S** thread);
void stop_Thread(THREAD_S** thread, unsigned char force);

#endif
