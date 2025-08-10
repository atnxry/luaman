#ifndef __XCOMMON_H__
#define __XCOMMON_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum
{
    KEY_F7 = 118,
    KEY_F8 = 119,
    KEY_F9 = 120,
    KEY_F10 = 121,
    KEY_F12 = 123,
};

#define BUFFER_MAX  512

#define LUAMAN_LOCK(_lock_) (void)WaitForSingleObject((_lock_), INFINITE);
#define LUAMAN_UNLOCK(_lock_) ReleaseMutex((_lock_));
#define LUAMAN_ALLOC(_sz_) (char*)malloc(sizeof(char) * (_sz_))
#define LUAMAN_FREE(_ptr_) if ((_ptr_)){free((_ptr_));(_ptr_)=NULL;}
#define LUAMAN_MEMSET(_m_, _c_, _sz_) memset((_m_), (_c_), (_sz_))
#define LUAMAN_MEMCPY(_d_, _s_, _sz_) memcpy((_d_), (_s_), (_sz_))
#define LUAMAN_DELETE(_ptr_) if ((_ptr_)){delete (_ptr_);(_ptr_)=NULL;}
#define LUAMAN_FCLOSE(_fp_) if (_fp_){fclose(_fp_);(_fp_)=NULL;}

#define RETURN_WITH_IF_PTR_NULL(_p_, _c_) if(!(_p_))return (_c_);
#define RETURN_IF_PTR_NULL(_p_) if(!(_p_))return;
#define RETURN_IF_CONDITION_TRUE(_c_, _r_) {if(_c_)return (_r_);}
#define JUMP_IF_PTR_NULL(_p_, _l_) if(!(_p_)) goto _l_;

#define LUAMAN_WAIT_MS(_ms_)  Sleep((_ms_))
#define Delay(_ms_)   Sleep(_ms_)

#endif
