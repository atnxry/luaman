#ifndef __XLOG_H__
#define __XLOG_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    LE_TRACE    = 0x1,
    LE_DEBUG    = 0x2,
    LE_INFO     = 0x4,
    LE_WARN     = 0x8,
    LE_ERROR    = 0x10,
    LE_ALL      = LE_TRACE | LE_DEBUG | LE_INFO | LE_WARN | LE_ERROR,
}LE_E;

int  __api_log_Init(const char* dir);
void __api_log_MsgTrace(const char* format, ...);
void __api_log_MsgDebug(const char* format, ...);
void __api_log_MsgInfo(const char* format, ...);
void __api_log_MsgWarn(const char* format, ...);
void __api_log_MsgError(const char* format, ...);
void __api_log_SetLevel(LE_E level, unsigned char status);
unsigned char __api_log_GetLevel(LE_E level);
void __api_log_MsgDebug_default(const char* format, ...);

#define api_log_Init        __api_log_Init
#define api_log_MsgTrace    __api_log_MsgTrace
#define api_log_MsgDebug    __api_log_MsgDebug
#define api_log_MsgInfo     __api_log_MsgInfo
#define api_log_MsgWarn     __api_log_MsgWarn
#define api_log_MsgError    __api_log_MsgError
#define api_log_SetLevel    __api_log_SetLevel
#define api_log_GetLevel    __api_log_GetLevel

#ifdef __cplusplus
}
#endif

#endif