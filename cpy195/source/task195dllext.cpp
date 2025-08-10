#include <stdlib.h>
#include <stdio.h>
#include "stdafx.h"
#include "config.h"
#include "task195dllext.h"

struct sync_counter
{
    int master;
    int slave;
};

#define MAX_GROUP   (32)
static struct sync_counter sync_count[MAX_GROUP];

int InitMutex(void)
{
    int j = 0;
    for (j = 0; j < MAX_GROUP; ++j)
    {
        sync_count[j].master = 0;
        sync_count[j].slave = 0;
    }
    return 0;
}

int FlagMark(int group, int role)
{
    if (group < 0 || group >= MAX_GROUP)
    {
        return -2;
    }

    if (ROLE_M == role)
    {
        if (1 != sync_count[group].master)
        {
            sync_count[group].master = 1;
        }
        return 0;
    }

    if (ROLE_S0 == role)
    {
        if (1 != sync_count[group].slave)
        {
            sync_count[group].slave = 1;
        }
        return 0;
    }

    return -1;
}

int FlagCheck(int group)
{    
    if (group < 0 || group >= MAX_GROUP)
    {
        return -1;
    }

    if (1 != sync_count[group].master)
    {
        return -2;
    }

    if (1 != sync_count[group].slave)
    {
        return -3;
    }
    
    return 0;
}

int FlagReset(int group)
{    
    if (group < 0 || group >= MAX_GROUP)
    {
        return -1;
    }
        
    sync_count[group].master = 0;
    sync_count[group].slave = 0;

    return 0;
}
