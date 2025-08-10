#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <stdio.h>

#include "xrrno.h"
#include "xcommon.h"
#include "xlog.h"

#ifdef __cplusplus
extern "C" {
#endif

static char __message[256] = {0};
static char buffer[2048] = {0};
static CRITICAL_SECTION lock;

//#define HAVE_DEBUG
#ifndef HAVE_DEBUG
    #define MAX_LOG_FILE_SIZE   (1024 * 1024 * 256)
#else
    #define MAX_LOG_FILE_SIZE   (32)
#endif

static unsigned int level = LE_DEBUG | LE_WARN | LE_ERROR;

static void __checksize(const char* file)
{
    struct _stat st;
    int status = 0;
    
    memset(&st, 0, sizeof(struct _stat));
    status = _stat(file, &st);
    if (status)
    {
        return;
    }

    if (st.st_size > MAX_LOG_FILE_SIZE)
    {
        try
        {
            CFile::Remove(CString(file));
        }catch(CFileException e)
        {
            
        }
    }
    
    return;
}

int __api_log_Init(const char* dir)
{
    int offset = 0;
    
    InitializeCriticalSection(&lock);
    offset = _snprintf(__message, sizeof(__message), "%s\\message.log", dir);
    RETURN_IF_CONDITION_TRUE(offset <= 0 || sizeof(__message) == offset, ERRNO_IO);
    
	return ERRNO_SUCCESS;
}

void __api_log_MsgTrace(const char* format, ...)
{
	int len = 0;
    int offset = 0;
	char* buffer = (char*)malloc(2048);
	SYSTEMTIME curTimeStamp;
	FILE* fp = NULL;
    DWORD taskId = GetCurrentThreadId();

    if (LE_TRACE != (level & LE_TRACE))
    {
        return;
    }

	if (!buffer)
	{
		return;
	}

	memset(buffer, 0, 2048);

	GetLocalTime(&curTimeStamp);
	offset = _snprintf(buffer, 2046, "[%04d-%02d-%02d %02d:%02d:%02d][TRACE][%ld]:",
		curTimeStamp.wYear, curTimeStamp.wMonth, curTimeStamp.wDay,
		curTimeStamp.wHour, curTimeStamp.wMinute, curTimeStamp.wSecond, taskId);
	if (offset <= 0 || 2046 == offset)
	{
		free(buffer);
		return;
	}

	len += offset;

	va_list marker;
    va_start(marker, format);

	offset = vsprintf(buffer + offset, format, marker);
    //offset = _snprintf(buffer + offset, 2046 - len, format, marker);
	//if (offset <= 0 || 2046 - len == offset)
	if (offset <= 0)
	{
		free(buffer);
		return;
	}

    va_end(marker);

	len += offset;
    buffer[len] = '\n';
    buffer[len + 1] = '\0';

	EnterCriticalSection(&lock);
    __checksize(__message);
    
	fp = fopen(__message, "a+");
	if (!fp)
	{
		goto back;
	}

	(void)fwrite(buffer, len + 1, 1, fp);
	fclose(fp);

back:
	LeaveCriticalSection(&lock);
	free(buffer);

	return;
}

void __api_log_MsgDebug(const char* format, ...)
{
	int len = 0;
    int offset = 0;
	char* buffer = (char*)malloc(2048);
	SYSTEMTIME curTimeStamp;
	FILE* fp = NULL;
    DWORD taskId = GetCurrentThreadId();

    if (LE_DEBUG != (level & LE_DEBUG))
    {
        return;
    }
    
	if (!buffer)
	{
		return;
	}

	memset(buffer, 0, 2048);

	GetLocalTime(&curTimeStamp);
	offset = _snprintf(buffer, 2046, "[%04d-%02d-%02d %02d:%02d:%02d][DEBUG][%ld]:",
		curTimeStamp.wYear, curTimeStamp.wMonth, curTimeStamp.wDay,
		curTimeStamp.wHour, curTimeStamp.wMinute, curTimeStamp.wSecond, taskId);
	if (offset <= 0 || 2046 == offset)
	{
		free(buffer);
		return;
	}

	len += offset;

	va_list marker;
    va_start(marker, format);

	offset = vsprintf(buffer + offset, format, marker);
    //offset = _snprintf(buffer + offset, 2046 - len, format, marker);
	//if (offset <= 0 || 2046 - len == offset)
	if (offset <= 0)
	{
		free(buffer);
		return;
	}

    va_end(marker);

	len += offset;
    buffer[len] = '\n';
    buffer[len + 1] = '\0';

	EnterCriticalSection(&lock);
    __checksize(__message);
    
	fp = fopen(__message, "a+");
	if (!fp)
	{
		goto back;
	}

	(void)fwrite(buffer, len + 1, 1, fp);
	fclose(fp);

back:
	LeaveCriticalSection(&lock);
	free(buffer);

	return;
}

void __api_log_MsgInfo(const char* format, ...)
{
	int len = 0;
    int offset = 0;
	char* buffer = (char*)malloc(2048);
	SYSTEMTIME curTimeStamp;
	FILE* fp = NULL;
    DWORD taskId = GetCurrentThreadId();

    if (LE_INFO != (level & LE_INFO))
    {
        return;
    }
    
	if (!buffer)
	{
		return;
	}

	memset(buffer, 0, 2048);

	GetLocalTime(&curTimeStamp);
	offset = _snprintf(buffer, 2046, "[%04d-%02d-%02d %02d:%02d:%02d][INFO][%ld]:",
		curTimeStamp.wYear, curTimeStamp.wMonth, curTimeStamp.wDay,
		curTimeStamp.wHour, curTimeStamp.wMinute, curTimeStamp.wSecond, taskId);
	if (offset <= 0 || 2046 == offset)
	{
		free(buffer);
		return;
	}

	len += offset;

	va_list marker;
    va_start(marker, format);

	offset = vsprintf(buffer + offset, format, marker);
    //offset = _snprintf(buffer + offset, 2046 - len, format, marker);
	//if (offset <= 0 || 2046 - len == offset)
	if (offset <= 0)
	{
		free(buffer);
		return;
	}

    va_end(marker);

	len += offset;
    buffer[len] = '\n';
    buffer[len + 1] = '\0';

	EnterCriticalSection(&lock);
    __checksize(__message);
    
	fp = fopen(__message, "a+");
	if (!fp)
	{
		goto back;
	}

	(void)fwrite(buffer, len + 1, 1, fp);
	fclose(fp);

back:
	LeaveCriticalSection(&lock);
	free(buffer);

	return;
}

void __api_log_MsgWarn(const char* format, ...)
{
	int len = 0;
    int offset = 0;
	char* buffer = (char*)malloc(2048);
	SYSTEMTIME curTimeStamp;
	FILE* fp = NULL;
    DWORD taskId = GetCurrentThreadId();

    if (LE_WARN != (level & LE_WARN))
    {
        return;
    }
    
	if (!buffer)
	{
		return;
	}

	memset(buffer, 0, 2048);

	GetLocalTime(&curTimeStamp);
	offset = _snprintf(buffer, 2046, "[%04d-%02d-%02d %02d:%02d:%02d][WARN][%ld]:",
		curTimeStamp.wYear, curTimeStamp.wMonth, curTimeStamp.wDay,
		curTimeStamp.wHour, curTimeStamp.wMinute, curTimeStamp.wSecond, taskId);
	if (offset <= 0 || 2046 == offset)
	{
		free(buffer);
		return;
	}

	len += offset;

	va_list marker;
    va_start(marker, format);

	offset = vsprintf(buffer + offset, format, marker);
    //offset = _snprintf(buffer + offset, 2046 - len, format, marker);
	//if (offset <= 0 || 2046 - len == offset)
	if (offset <= 0)
	{
		free(buffer);
		return;
	}

    va_end(marker);

	len += offset;
    buffer[len] = '\n';
    buffer[len + 1] = '\0';

	EnterCriticalSection(&lock);
    __checksize(__message);
    
	fp = fopen(__message, "a+");
	if (!fp)
	{
		goto back;
	}

	(void)fwrite(buffer, len + 1, 1, fp);
	fclose(fp);

back:
	LeaveCriticalSection(&lock);
	free(buffer);

	return;
}

void __api_log_MsgError(const char* format, ...)
{
	int len = 0;
    int offset = 0;
	char* buffer = (char*)malloc(2048);
	SYSTEMTIME curTimeStamp;
	FILE* fp = NULL;
    DWORD taskId = GetCurrentThreadId();

    if (LE_ERROR != (level & LE_ERROR))
    {
        return;
    }
    
	if (!buffer)
	{
		return;
	}

	memset(buffer, 0, 2048);

	GetLocalTime(&curTimeStamp);
	offset = _snprintf(buffer, 2046, "[%04d-%02d-%02d %02d:%02d:%02d][ERROR][%ld]:",
		curTimeStamp.wYear, curTimeStamp.wMonth, curTimeStamp.wDay,
		curTimeStamp.wHour, curTimeStamp.wMinute, curTimeStamp.wSecond, taskId);
	if (offset <= 0 || 2046 == offset)
	{
		free(buffer);
		return;
	}

	len += offset;

	va_list marker;
    va_start(marker, format);

	offset = vsprintf(buffer + offset, format, marker);
    //offset = _snprintf(buffer + offset, 2046 - len, format, marker);
	//if (offset <= 0 || 2046 - len == offset)
	if (offset <= 0)
	{
		free(buffer);
		return;
	}

    va_end(marker);

	len += offset;
    buffer[len] = '\n';
    buffer[len + 1] = '\0';

	EnterCriticalSection(&lock);
    __checksize(__message);
    
	fp = fopen(__message, "a+");
	if (!fp)
	{
		goto back;
	}

	(void)fwrite(buffer, len + 1, 1, fp);
	fclose(fp);

back:
	LeaveCriticalSection(&lock);
	free(buffer);

	return;
}

void __api_log_MsgDebug_default(const char* format, ...)
{
    return;
}

void __api_log_SetLevel(LE_E lev, unsigned char status)
{
    if (status)
    {
        level |= lev;
        return;
    }

    level &= (~lev);
    return;
}

unsigned char __api_log_GetLevel(LE_E lev)
{
    if (((unsigned int)lev) == (((unsigned int)lev) & level))
    {
        return 1;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
