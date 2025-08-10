#ifndef __XRRNO_H__
#define __XRRNO_H__

typedef enum
{
    ERRNO_SUCCESS     = 0,
    ERRNO_FAILED      = -1000,
    ERRNO_INVALID     = -999,
    ERRNO_NOMEM       = -998,
    ERRNO_NOSPACE     = -997,
    ERRNO_NETWORK     = -996,
    ERRNO_NOT_EXIST   = -995,
    ERRNO_EXIST       = -994,
    ERRNO_EXIT        = -993,
    ERRNO_EXCEPTION   = -992,
    ERRNO_AGAIN       = -991,
    ERRNO_IO          = -990,
    ERRNO_NOT_SUPPORT = -989,
    ERRNO_CONFLICT    = -988,
    ERRNO_BINDWND     = -987,
    ERRNO_REALIVE     = -986,
    ERRNO_BLOCK       = -985,
    ERRNO_TIMEOUT     = -984,
    ERRNO_IGNORE      = -983,
    ERRNO_BLOCKED     = -982,
    ERRNO_NEXTCOPY    = -981,
    ERRNO_REALIVE_X1  = -980,
    ERRNO_OFFLINE     = -979,
    ERRNO_NOMORE      = -978,
    ERRNO_ALARM       = -977,
}ERRNO_E;

#endif