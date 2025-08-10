#ifndef __TASK_SNAPSHOT_H__
#define __TASK_SNAPSHOT_H__

#include "dm.h"

int api_TaskSnapshotMain(Idmsoft* dmobj, void* ctxt);
int api_TaskSnapshotInit(Idmsoft* dmobj, void* ctxt);
int api_TaskSnapshotExit(Idmsoft* dmobj, void* ctxt);


#endif
