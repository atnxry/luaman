#include "stdafx.h"
#include "xrrno.h"
//#include "luaman.h"
#include "param.h"


CParameters::CParameters()
{
    LUAMAN_MEMSET(&m_config, 0, sizeof(struct cluamanconfig));
    INIT_LIST_HEAD(&m_config.node);
    INIT_LIST_HEAD(&m_config.bindinfoList);
    INIT_LIST_HEAD(&m_config.mixConfigList);
}

CParameters::~CParameters()
{
    
}

int CParameters::SetConfig(struct cluamanconfig* config)
{
    int status = api_CloneConfig(&m_config, config);
    if (ERRNO_SUCCESS == status)
    {
        //api_PrintConfig(&m_config);
    }

    return status;
}

void CParameters::SetSelectedFrontIndex(int selected_front_index)
{
    m_config.selected_front_index = selected_front_index;
    return;
}

void CParameters::SetDirection(int direction)
{
    m_config.direction = direction;
    return;
}

void CParameters::SetRole(int role)
{
    m_config.role = (ROLE_E)role;
    return;
}

void CParameters::SetGroup(int grp)
{
    m_config.groupid = grp;
    return;
}

void CParameters::SetBlueMax(int bluemax)
{
    m_config.maxBlueFiled = bluemax;
    return;

}
void CParameters::SetBlueMin(int bluemin)
{
    m_config.minBlue = bluemin;
    return;
}
void CParameters::SetBlueThreshold(int bluethreshold)
{
    m_config.blueThreshold = bluethreshold;
    return;
}

void CParameters::SetServer(char* server)
{
    memcpy(m_config.alarmServer, server, strlen(server) + 1);
    return;
}
void CParameters::SetPort(int port)
{
    m_config.alarmPort = port;
    return;
}

void CParameters::SetHost(int host)
{
    m_config.alarmHostIndex = host;
    return;
}

void CParameters::SetLoopNum(unsigned int loopnum)
{
    m_config.loop_num = loopnum;
    return;
}

void CParameters::SetPalyWndIndex(unsigned int playWndIndexVal)
{
    m_config.playWndIndexVal = playWndIndexVal;
    return;
}

void CParameters::SetTransWait(int Wait2Sleep)
{
    m_config.Wait2Sleep = Wait2Sleep;
    return;
}

void CParameters::SetExitGameCopyFull(int exitGameCopyFull)
{
    m_config.exitGameCopyFull = exitGameCopyFull;
    return;
}

void CParameters::SetCleanPackageTime(int cleanPackageTime)
{
    m_config.cleanPackageTime = cleanPackageTime;
}

void CParameters::SetCleanPackageWhenStart(int cleanPackageWhenStart)
{
    m_config.cleanPackageWhenStart = cleanPackageWhenStart;
    return;
}

void CParameters::SetCallBabyDie(int callbabydie)
{
    m_config.callbabydie = callbabydie;
    return;
}

void CParameters::SetCleanPackage1(int cleanPackage1)
{
    if (cleanPackage1)
    {
        CLEAN_PACKAGE_1(m_config.cleanPackageX);
        return;
    }

    NO_CLEAN_PACKAGE_1(m_config.cleanPackageX);
    return;
}

void CParameters::SetCleanPackage2(int cleanPackage2)
{
    if (cleanPackage2)
    {
        CLEAN_PACKAGE_2(m_config.cleanPackageX);
        return;
    }

    NO_CLEAN_PACKAGE_2(m_config.cleanPackageX);
    return;
}

void CParameters::SetCleanPackage3(int cleanPackage3)
{
    if (cleanPackage3)
    {
        CLEAN_PACKAGE_3(m_config.cleanPackageX);
        return;
    }

    NO_CLEAN_PACKAGE_3(m_config.cleanPackageX);
    return;
}

void CParameters::SetCleanPackage4(int cleanPackage4)
{
    if (cleanPackage4)
    {
        CLEAN_PACKAGE_4(m_config.cleanPackageX);
        return;
    }

    NO_CLEAN_PACKAGE_4(m_config.cleanPackageX);
    return;
}

void CParameters::SetCleanPackage5(int cleanPackage5)
{
    if (cleanPackage5)
    {
        CLEAN_PACKAGE_5(m_config.cleanPackageX);
        return;
    }

    NO_CLEAN_PACKAGE_5(m_config.cleanPackageX);
    return;
}

void CParameters::SetCleanPackage(int cleanPackage)
{
    m_config.cleanPackageX = cleanPackage;
}

void CParameters::SetWaitCopytime(int waitCopytime)
{
    m_config.waitCopytime = waitCopytime;
}

void CParameters::SetBluebloodChecktime(int bluebloodChecktime)
{
    m_config.bluebloodChecktime = bluebloodChecktime;
}

void CParameters::SetBlockTimes(int blockTimes)
{
    m_config.blockTimes = blockTimes;
}

