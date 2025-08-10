#ifndef __TASKSIGNLECPY1_PRI_H__
#define __TASKSIGNLECPY1_PRI_H__

#include "dm.h"

int check_pri_copyfull(Idmsoft* dmobj, void* ctxt);
int signle_pri_copy1(Idmsoft* dmobj, void* ctxt, int tasknr);
int signle_pri_copy2(Idmsoft* dmobj, void* ctxt, int tasknr);
int signle_pri_copy3(Idmsoft* dmobj, void* ctxt, int tasknr);
int signle_pri_copy4(Idmsoft* dmobj, void* ctxt, int tasknr);
int signle_pri_copy5(Idmsoft* dmobj, void* ctxt, int tasknr);
void signlecpy_pri_InitCriticalSection();
int signle_pri_copy6(Idmsoft* dmobj, void* ctxt, int tasknr);
int signle_pri_copy7(Idmsoft* dmobj, void* ctxt, int tasknr);

int signle_pri_copy_foolish(Idmsoft* dmobj, void* ctxt, int tasknr);


#endif
