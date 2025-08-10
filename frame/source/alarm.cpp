#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h> 
#include "xlog.h"
#include "alarm.h"
#pragma comment(lib, "Winmm.lib")

struct alarm_args
{
    char filename[MAX_PATH];
    int duration;
    char ipaddr[MAX_PATH];
    int port;
    int host_index;
};

typedef enum
{
    SNAIL_ERRNO_SUCCESS     = 0     ,
    SNAIL_ERRNO_FAILED      = -100  ,
    SNAIL_ERRNO_INVALID             ,
    SNAIL_ERRNO_NOMEM               ,
    SNAIL_ERRNO_NOSPACE             ,
    SNAIL_ERRNO_NETWORK             ,
    SNAIL_ERRNO_NOT_EXIST           ,
    SNAIL_ERRNO_EXIST               ,
    SNAIL_ERRNO_EXIT                ,
    SNAIL_ERRNO_EXCEPTION           ,
    SNAIL_ERRNO_AGAIN               ,
}SNAIL_ERRNO_E;

typedef unsigned int      FD_T;
#define SNAIL_INVALID_FD      (-1)

typedef struct
{
    int type;
    int len;
    char value[0];
}SNAIL_MESSAGE_S;

#define SNAIL_H2NL(n) (n) = (htonl(n))
#define SNAIL_N2HL(n) (n) = (ntohl(n))

#define SNAIL_MESSAGE_H2NL(msg) do { \
    SNAIL_H2NL((msg)->type); \
    SNAIL_H2NL((msg)->len); \
} while (0);

#define SNAIL_MESSAGE_N2HL(msg) do { \
    SNAIL_N2HL((msg)->type); \
    SNAIL_N2HL((msg)->len); \
} while (0);

static int sendAlarm(char* ipaddr, int port, int type)
{
    int code = SNAIL_ERRNO_FAILED;
    sockaddr_in servAddr;
    FD_T connFd = SNAIL_INVALID_FD;
    WSADATA wsaData;
    char* message = NULL;
    int msgLen = 0;
    int flag = 1;
    SNAIL_MESSAGE_S msg;

    api_log_MsgDebug("sendAlarm, ipaddr:%s, port:%d, type:%d", ipaddr, port, type);
    
    code = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (NO_ERROR != code)
    {
        api_log_MsgDebug("WSAStartup failed, code:%d", code);
        return SNAIL_ERRNO_FAILED;
    }

    connFd = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == connFd)
    {
        WSACleanup();

        api_log_MsgDebug("socket failed");
        return SNAIL_ERRNO_NETWORK;
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ipaddr);
    servAddr.sin_port = htons(port);

    code = connect(connFd, (SOCKADDR*)&servAddr, sizeof(servAddr));
    if (SOCKET_ERROR == code)
    {
        closesocket(connFd);
        WSACleanup();

        api_log_MsgDebug("connect failed");
        return SNAIL_ERRNO_NETWORK;
    }

    memset(&msg, 0, sizeof(SNAIL_MESSAGE_S));
    msg.type = type;
    msg.len = sizeof(SNAIL_MESSAGE_S);

    SNAIL_MESSAGE_H2NL(&msg);
    message = (CHAR*)&msg;
    msgLen = sizeof(SNAIL_MESSAGE_S);
    
    code = send(connFd, message, msgLen, 0);
    if (code != msgLen)
    {
        closesocket(connFd);
        WSACleanup();

        api_log_MsgDebug("send failed, msgLen:%d, code:%d", msgLen, code);
        return SNAIL_ERRNO_NETWORK;
    }
    
    memset(&msg, 0, sizeof(SNAIL_MESSAGE_S));
    (void)recv(connFd, (char*)&msg, sizeof(SNAIL_MESSAGE_S), 0);
    SNAIL_MESSAGE_N2HL(&msg);

    closesocket(connFd);
    WSACleanup();

    return SNAIL_ERRNO_SUCCESS;
}


void thread_entry(void* ctxt)
{
    struct alarm_args* pArgs = (struct alarm_args*)ctxt;
    char* filename = pArgs->filename;
    int duration = pArgs->duration;

    PlaySound(filename, NULL, SND_LOOP |SND_ASYNC | SND_FILENAME);
    while (duration > 0)
    {
        Sleep(1000);
        duration--;
    }
    PlaySound(NULL, NULL, SND_LOOP |SND_ASYNC | SND_FILENAME);
    free(ctxt);
    return;
}

int alarm(char* filename, int duration)
{
    struct alarm_args* ctxt = NULL;
    int len = 0;

    if (NULL == filename)
    {
        return -2;
    }

    len = strlen(filename);
    if (len <= 0 || len >= MAX_PATH)
    {
        return -3;
    }

    if (duration <= 0)
    {
        duration = 10;
    }

    if (duration >= 600)
    {
        duration = 600;
    }

    ctxt = (struct alarm_args*)malloc(sizeof(struct alarm_args));
    if (!ctxt)
    {
        return -1;
    }

    memset(ctxt, 0, sizeof(struct alarm_args));
    memcpy(ctxt->filename, filename, len);
    ctxt->duration = duration;

    if (-1 == _beginthread(thread_entry, 0, ctxt))
    {
        free(ctxt);
        return -4;
    }

    return 0;
}

void thread_entry_report(void* ctxt)
{
    struct alarm_args* pArgs = (struct alarm_args*)ctxt;
    char* ipaddr = pArgs->ipaddr;
    int port = pArgs->port;
    int host_index = pArgs->host_index;

    sendAlarm(ipaddr, port, host_index);
    
    free(ctxt);
    return;
}

int alarm_report(char* ipaddr, int port, int host_index)
{
    struct alarm_args* ctxt = NULL;
    int len = 0;

    if (NULL == ipaddr)
    {
        return -2;
    }

    len = strlen(ipaddr);
    if (len <= 0 || len >= MAX_PATH)
    {
        return -3;
    }

    ctxt = (struct alarm_args*)malloc(sizeof(struct alarm_args));
    if (!ctxt)
    {
        return -1;
    }

    memset(ctxt, 0, sizeof(struct alarm_args));
    memcpy(ctxt->ipaddr, ipaddr, len);
    ctxt->port = port;
    ctxt->host_index = host_index;
    
    if (-1 == _beginthread(thread_entry_report, 0, ctxt))
    {
        free(ctxt);
        return -4;
    }

    return 0;
}

#include <time.h>
time_t g_last_check_time;

int alloc_alarm_ctxt(void)
{
    g_last_check_time = TlsAlloc();
    if (-1 == g_last_check_time)
    {
        return -1;
    }

    return 0;
}

void free_alarm_ctxt(void)
{
    TlsFree(g_last_check_time);
    return;
}

void init_alarm_ctxt(void)
{
    time_t start;
    start = time(NULL);  

    TlsSetValue(g_last_check_time, (LPVOID)start);
    return;
}

int check_alarm_timeout(int duration)
{
    time_t end;
    int offset = 0;

    end = time(NULL);
    offset = end - (time_t)TlsGetValue(g_last_check_time);

    if (offset >= duration)
    {
        TlsSetValue(g_last_check_time, (LPVOID)end);    
        return 1;
    }

    return 0;
}