#ifndef __TASK_SIGNLE_PRI_H__
#define __TASK_SIGNLE_PRI_H__

#include "dm.h"

int signle_pri_clickhome(Idmsoft* dmobj, void* ctxt);
int signle_pri_walk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir);
int signle_pri_copy(Idmsoft* dmobj, void* ctxt, int tasknr);
int signle_pri_postwalk(Idmsoft* dmobj, void* ctxt);

#endif
