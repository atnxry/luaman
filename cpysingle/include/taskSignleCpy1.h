#ifndef __TASKSIGNLECPY1_H__
#define __TASKSIGNLECPY1_H__

#include "dm.h"

int check_copyfull(Idmsoft* dmobj, void* ctxt);
int signle_copy1(Idmsoft* dmobj, void* ctxt, int tasknr);
int signle_copy2(Idmsoft* dmobj, void* ctxt, int tasknr);
int signle_copy3(Idmsoft* dmobj, void* ctxt, int tasknr);
int signle_copy4(Idmsoft* dmobj, void* ctxt, int tasknr);
int signle_copy5(Idmsoft* dmobj, void* ctxt, int tasknr);
void signlecpy_InitCriticalSection();
int signle_copy6(Idmsoft* dmobj, void* ctxt, int tasknr);
int signle_copy7(Idmsoft* dmobj, void* ctxt, int tasknr);

int signle_copy_foolish(Idmsoft* dmobj, void* ctxt, int tasknr);


#endif
