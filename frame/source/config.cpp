#include "stdafx.h"
#include "xrrno.h"
#include "xlog.h"
#include "xcommon.h"
#include "datatype.h"
#include "config.h"
#include "Markup.h"
#include "task.h"
#include "task175.h"
#include "task195.h"
#include "task021.h"
#include "task1853.h"
#include "task000.h"
#include "taskSignin.h"
#include "taskSakura.h"
#include "taskDotask.h"
#include "taskFight.h"
#include "taskSignle.h"
#include "taskPirate.h"
#include "taskExPirate.h"
#include "taskExSignle.h"
#include "taskEx175.h"
#include "taskTeamGroup.h"
#include <iostream>
#include <fstream>
#include <deque>

#define KEY_Root                     "Root"
#define KEY_Copy                     "Copy"
#define KEY_copy_name                "CopyName"
#define KEY_loop_num                 "loop_num"
#define KEY_taskType                 "taskType"
#define KEY_direction                "direction"
#define KEY_role                     "role"
#define KEY_groupid                  "groupid"
#define KEY_maxBlueFiled             "maxBlueFiled"
#define KEY_minBlue                  "minBlue"
#define KEY_blueThreshold            "blueThreshold"
#define KEY_alarmServer              "alarmServer"
#define KEY_alarmPort                "alarmPort"
#define KEY_alarmHostIndex           "alarmHostIndex"
#define KEY_playWndIndexVal          "playWndIndexVal"
#define KEY_Wait2Sleep               "Wait2Sleep"
#define KEY_exitGameCopyFull         "exitGameCopyFull"
#define KEY_cleanPackageWhenStart    "cleanPackageWhenStart"
#define KEY_callBabyDie              "callbabydie"
#define KEY_bluebloodWidth           "bluebloodWidth"
#define KEY_bluebloodLowLevel        "bluebloodLowLevel"
#define KEY_syncCountMax             "syncCountMax"
#define KEY_bluebloodChecktime       "bluebloodChecktime"
#define KEY_blockTimes               "blockTimes"
#define KEY_waitCopytime             "waitCopytime"
#define KEY_cleanPackageTime         "cleanPackageTime"
#define KEY_cleanPackageX            "cleanPackageX"
#define KEY_buddyName                "buddyName"
#define KEY_swjdIndex                "swjdIndex"
#define KEY_copyrolenr               "copyrolenr"

///////////////////////////////////////////////////////////////////////////////////////////
//组合脚本
#define KEY_BindInfo                 "BindInfo"
#define KEY_BindWnd                  "BindWnd"
#define KEY_Account                  "Account"
#define KEY_UserName                 "UserName"
#define KEY_Password                 "Password"
#define KEY_PasswordEx               "PasswordEx"
#define KEY_MixCopyLoopNum           "MixCopyLoopNum"
#define KEY_MixCopy                  "MixCopy"
#define KEY_BindHostIdx              "BindHostIdx"

////////////////////////////////////////////////////////////////////////////////////////


#define CHECK_FIELD_EQUAL(_f1_, _f2_) ((_f1_) == (_f2_))

static int createBindinfoNode(std::string& file/*CMarkup& xml*/, struct cluamanconfig* config)
{
    struct cluamanbindinfo* bpos = NULL, *bn = NULL;
    struct cluamanaccountinfo* apos = NULL, *an = NULL;
    TASK_TYPE_E type = TASK_TYPE_UNKNOWN;
    int flag = 0;
    int status = ERRNO_NOT_EXIST;
    TASK_TYPE_E mixtask[] = MIX_TASKS();
    int i = 0;
    int bExist = 0;

    CMarkup xml;

    //api_log_MsgDebug("========= createBindinfoNode ===========");
    //api_PrintConfig(config);
    
    if (!xml.Load(CString(file.c_str())))
    {
        api_log_MsgError("load %s failed when createBindinfoNode", file.c_str());
        return ERRNO_FAILED;
    }

    xml.ResetMainPos();
    if (!xml.FindElem(CString(KEY_Root)))
    {
        return ERRNO_FAILED;
    }
    
    //api_log_MsgDebug("---------- config->bindinfoNum:%d", config->bindinfoNum);
    
    if (config->bindinfoNum <= 0)
    {
        return ERRNO_SUCCESS;
    }

    xml.IntoElem();//ROOT

    while (xml.FindElem(CString(KEY_Copy)))
    {
        xml.IntoElem();//COPY
        
        while (xml.FindElem(NULL))
        {
            CString tag = xml.GetTagName();
            CString attr = xml.GetAttrib(CString(ATTR_VALUE));

            //if (CString(KEY_copy_name) == tag)
            if (CString(KEY_taskType) == tag)
            {
                char* cattr = attr.GetBuffer(BUFFER_MAX);
                type = (TASK_TYPE_E)atoi(cattr);

                //api_log_MsgDebug("cattr:%s, type:%d, taskType:%d, copyName:%s", cattr, type, config->taskType, config->copyName);
                attr.ReleaseBuffer();

                bExist = 0;
                for (i = 0; i < LUAMAN_DIM(mixtask); i++)
                {
                    if (type == mixtask[i])
                    {
                        bExist = 1;
                        break;
                    }
                }

                if (0 == bExist)
                {
                    break;
                }
                
                
                list_for_each_entry_safe(bpos, bn, &config->bindinfoList, node, struct cluamanbindinfo)
                {
                    ///////////////////////////////////////////////////////////
                    //新增记录
                    xml.AddElem(CString(KEY_BindInfo));
                    xml.IntoElem();
                    
                    ITEM_NEW_D(xml, KEY_BindWnd, bpos->bindWndnr);

                    if (bpos->accountNum <= 0)
                    {
                        xml.OutOfElem();
                        continue;
                    }
                    
                    list_for_each_entry_safe(apos, an, &bpos->accountList, node, struct cluamanaccountinfo)
                    {
                        xml.AddElem(CString(KEY_Account));
                        xml.IntoElem();
                        
                        ITEM_NEW(xml, KEY_UserName, apos->user_name);
                        ITEM_NEW(xml, KEY_Password, apos->passwd);
                        ITEM_NEW(xml, KEY_PasswordEx, apos->passwdex);

                        //api_log_MsgDebug("apos->user_name : %s", apos->user_name);
                        //api_log_MsgDebug("apos->passwd    : %s", apos->passwd);
                        //api_log_MsgDebug("apos->passwdex  : %s", apos->passwdex);
                        
                        xml.OutOfElem();
                    }

                    xml.OutOfElem();
                }

                ///////////////////////////////////////////////////////////
                flag = 1;
                status = ERRNO_SUCCESS;
                
                break;
            }

            if (flag)
            {
                break;
            }
         }

        xml.OutOfElem();//COPY

        if (flag)
        {
            break;
        }
    }
    
    xml.OutOfElem();//ROOT

    xml.Save(CString(file.c_str()));
    return status;
}

static int createMixcopyNode(std::string& file/*CMarkup& xml*/, struct cluamanconfig* config)
{
    struct cluamanconfig* cpos = NULL, *cn = NULL;
    TASK_TYPE_E type = TASK_TYPE_UNKNOWN;
    int flag = 0;
    int status = ERRNO_NOT_EXIST;
    TASK_TYPE_E mixtask[] = MIX_TASKS();
    int i = 0;
    int bExist = 0;
    CMarkup xml;
    
    if (config->mixConfigNum <= 0)
    {
        return ERRNO_SUCCESS;
    }

    if (!xml.Load(CString(file.c_str())))
    {
        api_log_MsgError("load %s failed when createMixcopyNode", file.c_str());
        return ERRNO_FAILED;
    }

    xml.ResetMainPos();
    if (!xml.FindElem(CString(KEY_Root)))
    {
        return ERRNO_FAILED;
    }
    
    xml.IntoElem();//ROOT

    while (xml.FindElem(CString(KEY_Copy)))
    {
        xml.IntoElem();//COPY
        
        while (xml.FindElem(NULL))
        {
            CString tag = xml.GetTagName();
            CString attr = xml.GetAttrib(CString(ATTR_VALUE));

            if (CString(KEY_taskType) == tag)
            {
                char* cattr = attr.GetBuffer(BUFFER_MAX);
                type = (TASK_TYPE_E)atoi(cattr);
                attr.ReleaseBuffer();

                bExist = 0;
                for (i = 0; i < LUAMAN_DIM(mixtask); i++)
                {
                    if (type == mixtask[i])
                    {
                        bExist = 1;
                        break;
                    }
                }

                if (0 == bExist)
                {
                    break;
                }

                list_for_each_entry_safe(cpos, cn, &config->mixConfigList, node, struct cluamanconfig)
                {
                    xml.AddElem(CString(KEY_MixCopy));
                    xml.IntoElem();
                
                    ITEM_NEW(xml, KEY_copy_name, cpos->copyName);
                    ITEM_NEW_D(xml, KEY_copyrolenr, cpos->copyrolenr);
                    ITEM_NEW_D(xml, KEY_loop_num, cpos->loop_num);
                    ITEM_NEW_D(xml, KEY_taskType, cpos->taskType);
                    ITEM_NEW_D(xml, KEY_direction, cpos->direction);
                    ITEM_NEW_D(xml, KEY_role, cpos->role);
                    ITEM_NEW_D(xml, KEY_groupid, cpos->groupid);
                    ITEM_NEW_D(xml, KEY_maxBlueFiled, cpos->maxBlueFiled);
                    ITEM_NEW_D(xml, KEY_minBlue, cpos->minBlue);
                    ITEM_NEW_D(xml, KEY_blueThreshold, cpos->blueThreshold);
                    ITEM_NEW(xml, KEY_alarmServer, cpos->alarmServer);
                    ITEM_NEW_D(xml, KEY_alarmPort, cpos->alarmPort);
                    ITEM_NEW_D(xml, KEY_alarmHostIndex, cpos->alarmHostIndex);
                    ITEM_NEW_D(xml, KEY_playWndIndexVal, cpos->playWndIndexVal);
                    ITEM_NEW_D(xml, KEY_Wait2Sleep, cpos->Wait2Sleep);
                    ITEM_NEW_D(xml, KEY_exitGameCopyFull, cpos->exitGameCopyFull);
                    ITEM_NEW_D(xml, KEY_cleanPackageWhenStart, cpos->cleanPackageWhenStart);
                    ITEM_NEW_D(xml, KEY_callBabyDie, cpos->callbabydie);
                    ITEM_NEW_D(xml, KEY_bluebloodWidth, cpos->bluebloodWidth);
                    ITEM_NEW_D(xml, KEY_bluebloodLowLevel, cpos->bluebloodLowLevel);
                    ITEM_NEW_D(xml, KEY_syncCountMax, cpos->syncCountMax);
                    ITEM_NEW_D(xml, KEY_bluebloodChecktime, cpos->bluebloodChecktime);
                    ITEM_NEW_D(xml, KEY_blockTimes, cpos->blockTimes);
                    ITEM_NEW_D(xml, KEY_waitCopytime, cpos->waitCopytime);
                    ITEM_NEW_D(xml, KEY_cleanPackageTime, cpos->cleanPackageTime);
                    ITEM_NEW_D(xml, KEY_cleanPackageX, cpos->cleanPackageX);
                    ITEM_NEW(xml, KEY_buddyName, cpos->buddyName);
                    ITEM_NEW_D(xml, KEY_swjdIndex, cpos->swjd_index);
                    ITEM_NEW_D(xml, KEY_BindHostIdx, cpos->curBindHostIdx);
                    
                    xml.OutOfElem();
                }

                flag = 1;
                status = ERRNO_SUCCESS;
                break;
            }

            if (flag)
            {
                break;
            }
        }

        xml.OutOfElem();//COPY

        if (flag)
        {
            break;
        }
    }
    
    xml.OutOfElem();//ROOT

    xml.Save(CString(file.c_str()));
    return status;
}

int api_CreatXML(std::string& file, struct cluamanconfig* config)
{
    CMarkup xml;
    char* tasktype_tips[] = TASK_TYPE_DESC_ARRAY();

    xml.SetDoc(CString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"));
    xml.AddElem(CString(KEY_Root));

    xml.IntoElem();
    xml.AddElem(CString(KEY_Copy));
    xml.IntoElem();
    
    ITEM_NEW(xml, KEY_copy_name, tasktype_tips[config->taskType]);
    ITEM_NEW_D(xml, KEY_loop_num, config->loop_num);
    ITEM_NEW_D(xml, KEY_taskType, config->taskType);
    ITEM_NEW_D(xml, KEY_direction, config->direction);
    ITEM_NEW_D(xml, KEY_role, config->role);
    ITEM_NEW_D(xml, KEY_groupid, config->groupid);
    ITEM_NEW_D(xml, KEY_maxBlueFiled, config->maxBlueFiled);
    ITEM_NEW_D(xml, KEY_minBlue, config->minBlue);
    ITEM_NEW_D(xml, KEY_blueThreshold, config->blueThreshold);
    ITEM_NEW(xml, KEY_alarmServer, config->alarmServer);
    ITEM_NEW_D(xml, KEY_alarmPort, config->alarmPort);
    ITEM_NEW_D(xml, KEY_alarmHostIndex, config->alarmHostIndex);
    ITEM_NEW_D(xml, KEY_playWndIndexVal, config->playWndIndexVal);
    ITEM_NEW_D(xml, KEY_Wait2Sleep, config->Wait2Sleep);
    ITEM_NEW_D(xml, KEY_exitGameCopyFull, config->exitGameCopyFull);
    ITEM_NEW_D(xml, KEY_cleanPackageWhenStart, config->cleanPackageWhenStart);
    ITEM_NEW_D(xml, KEY_callBabyDie, config->callbabydie);
    ITEM_NEW_D(xml, KEY_bluebloodWidth, config->bluebloodWidth);
    ITEM_NEW_D(xml, KEY_bluebloodLowLevel, config->bluebloodLowLevel);
    ITEM_NEW_D(xml, KEY_syncCountMax, config->syncCountMax);
    ITEM_NEW_D(xml, KEY_bluebloodChecktime, config->bluebloodChecktime);
    ITEM_NEW_D(xml, KEY_blockTimes, config->blockTimes);
    ITEM_NEW_D(xml, KEY_waitCopytime, config->waitCopytime);
    ITEM_NEW_D(xml, KEY_cleanPackageTime, config->cleanPackageTime);
    ITEM_NEW_D(xml, KEY_cleanPackageX, config->cleanPackageX);

    ITEM_NEW_D(xml, KEY_MixCopyLoopNum, config->mixConfigNum);
    ITEM_NEW_D(xml, KEY_BindHostIdx, config->curBindHostIdx);
    
    xml.OutOfElem();
    xml.OutOfElem();

    xml.Save(CString(file.c_str()));

    //(void)createBindinfoNode(file, config);
    //(void)createMixcopyNode(file, config);
        
    return ERRNO_SUCCESS;
}

static bool LoadCopyConfig(char* tasktype, CMarkup& xml, struct cluamanconfig* config)
{
    bool bok = false;
    
    while (xml.FindElem(NULL))
    {
        CString tag = xml.GetTagName();
        CString attr = xml.GetAttrib(CString(ATTR_VALUE));

        if (CString(KEY_copy_name) == tag)
        {
            if (NULL != tasktype)
            {
                if (CString(tasktype) != attr)
                {
                    break;
                }

                continue;
            }

            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->copyName, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            //api_log_MsgDebug("copyName:%s", config->copyName);

            bok = true;
            continue;
        }

        bok = true;

        if (CString(KEY_copyrolenr) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->copyrolenr = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("copyrolenr:%d", config->copyrolenr);
            continue;
        }
        
        if (CString(KEY_loop_num) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->loop_num = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("loop_num:%d", config->loop_num);
            continue;
        }
        if (CString(KEY_taskType) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->taskType = (TASK_TYPE_E)atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("taskType:%d", config->taskType);
            continue;
        }
        
        if (CString(KEY_direction) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->direction = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("direction:%d", config->direction);
            continue;
        }

        if (CString(KEY_role) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->role = (ROLE_E)atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("role:%d", (int)config->role);
            continue;
        }

        if (CString(KEY_groupid) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->groupid = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("groupid:%d", config->groupid);
            continue;
        }

        if (CString(KEY_maxBlueFiled) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->maxBlueFiled = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("maxBlueFiled:%d", config->maxBlueFiled);
            continue;
        }

        if (CString(KEY_minBlue) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->minBlue = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("minBlue:%d", config->minBlue);
            continue;
        }

        if (CString(KEY_blueThreshold) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->blueThreshold = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("blueThreshold:%d", config->blueThreshold);
            continue;
        }

        if (CString(KEY_buddyName) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->buddyName, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("buddyName:%s", config->buddyName);
            continue;
        }
        
        if (CString(KEY_alarmServer) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->alarmServer, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("alarmServer:%s", config->alarmServer);
            continue;
        }

        if (CString(KEY_swjdIndex) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->swjd_index = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("swjd_index:%d", config->swjd_index);
            continue;
        }
        
        if (CString(KEY_alarmPort) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->alarmPort = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("alarmPort:%d", config->alarmPort);
            continue;
        }

        if (CString(KEY_alarmHostIndex) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->alarmHostIndex = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("alarmHostIndex:%d", config->alarmHostIndex);
            continue;
        }

        if (CString(KEY_playWndIndexVal) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->playWndIndexVal = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("playWndIndexVal:%d", config->playWndIndexVal);
            continue;
        }

        if (CString(KEY_Wait2Sleep) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->Wait2Sleep = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("Wait2Sleep:%d", config->Wait2Sleep);
            continue;
        }

        if (CString(KEY_exitGameCopyFull) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->exitGameCopyFull = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("exitGameCopyFull:%d", config->exitGameCopyFull);
            continue;
        }

        if (CString(KEY_cleanPackageWhenStart) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->cleanPackageWhenStart = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("cleanPackageWhenStart:%d", config->cleanPackageWhenStart);
            continue;
        }

        if (CString(KEY_callBabyDie) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->callbabydie = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("callbabydie:%d", config->callbabydie);
            continue;
        }
        
        if (CString(KEY_bluebloodWidth) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->bluebloodWidth = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("bluebloodWidth:%d", config->bluebloodWidth);
            continue;
        }

        if (CString(KEY_bluebloodLowLevel) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->bluebloodLowLevel = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("bluebloodLowLevel:%d", config->bluebloodLowLevel);
            continue;
        }

        if (CString(KEY_syncCountMax) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->syncCountMax = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("syncCountMax:%d", config->syncCountMax);
            continue;
        }

        if (CString(KEY_bluebloodChecktime) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->bluebloodChecktime = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("bluebloodChecktime:%d", config->bluebloodChecktime);
            continue;
        }

        if (CString(KEY_blockTimes) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->blockTimes = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("blockTimes:%d", config->blockTimes);
            continue;
        }

        if (CString(KEY_waitCopytime) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->waitCopytime = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("waitCopytime:%d", config->waitCopytime);
            continue;
        }

        if (CString(KEY_cleanPackageTime) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->cleanPackageTime = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("cleanPackageTime:%d", config->cleanPackageTime);
            continue;
        }

        if (CString(KEY_cleanPackageX) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->cleanPackageX = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("cleanPackageX:%d", config->cleanPackageX);
            continue;
        }

        ////////////////////////////////////////////////////////////////////////////////
        //组合脚本
        if (CString(KEY_BindInfo) == tag)//解析BindInfo
        {
            struct cluamanbindinfo* bindinfo_node = NULL;
            
            xml.IntoElem();

            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            //解析BindInfo下各个标签
            while (xml.FindElem(NULL))
            {
                CString itag = xml.GetTagName();
                CString iattr = xml.GetAttrib(CString(ATTR_VALUE));

                if (CString(KEY_BindWnd) == itag)
                {
                    char* cattr = iattr.GetBuffer(BUFFER_MAX);

                    if (NULL == bindinfo_node)
                    {
                        bindinfo_node = (struct cluamanbindinfo*)LUAMAN_ALLOC(sizeof(struct cluamanbindinfo));
                        if (NULL == bindinfo_node)
                        {
                            AfxMessageBox("内存不足,程序即将退出");
                            exit(1);
                        }

                        memset(bindinfo_node, 0, sizeof(struct cluamanbindinfo));
                        INIT_LIST_HEAD(&bindinfo_node->node);
                        INIT_LIST_HEAD(&bindinfo_node->accountList);
                    }

                    bindinfo_node->bindWndnr = atoi(cattr);
                    attr.ReleaseBuffer();
                    
                    api_log_MsgInfo("bindWndnr:%d", bindinfo_node->bindWndnr);
                    continue;
                }

                //***********************************************************************
                //解析Account
                if (CString(KEY_Account) == itag)
                {
                    struct cluamanaccountinfo* account_node = NULL;
                    
                    xml.IntoElem();
                    //解析Account下各个标签
                    while (xml.FindElem(NULL))
                    {
                        CString iitag = xml.GetTagName();
                        CString iiattr = xml.GetAttrib(CString(ATTR_VALUE));

                        if (CString(KEY_UserName) == iitag)
                        {
                            char* cattr = iiattr.GetBuffer(BUFFER_MAX);

                            if (NULL == account_node)
                            {
                                account_node = (struct cluamanaccountinfo*)malloc(sizeof(struct cluamanaccountinfo));
                                if (NULL == account_node)
                                {
                                    AfxMessageBox("内存不足,程序即将退出");
                                    exit(1);
                                }
                                memset(account_node, 0, sizeof(struct cluamanaccountinfo));
                                INIT_LIST_HEAD(&account_node->node);
                            }

                            memcpy(account_node->user_name, cattr, strlen(cattr));
                            attr.ReleaseBuffer();

                            api_log_MsgInfo("UserName:%s", account_node->user_name);
                            continue;
                        }

                        if (CString(KEY_Password) == iitag)
                        {
                            char* cattr = iiattr.GetBuffer(BUFFER_MAX);

                            if (NULL == account_node)
                            {
                                account_node = (struct cluamanaccountinfo*)malloc(sizeof(struct cluamanaccountinfo));
                                if (NULL == account_node)
                                {
                                    AfxMessageBox("内存不足,程序即将退出");
                                    exit(1);
                                }
                                memset(account_node, 0, sizeof(struct cluamanaccountinfo));
                                INIT_LIST_HEAD(&account_node->node);
                            }

                            memcpy(account_node->passwd, cattr, strlen(cattr));
                            attr.ReleaseBuffer();

                            api_log_MsgInfo("passwd:%s", account_node->passwd);
                            continue;
                        }

                        if (CString(KEY_PasswordEx) == iitag)
                        {
                            char* cattr = iiattr.GetBuffer(BUFFER_MAX);

                            if (NULL == account_node)
                            {
                                account_node = (struct cluamanaccountinfo*)malloc(sizeof(struct cluamanaccountinfo));
                                if (NULL == account_node)
                                {
                                    AfxMessageBox("内存不足,程序即将退出");
                                    exit(1);
                                }
                                memset(account_node, 0, sizeof(struct cluamanaccountinfo));
                                INIT_LIST_HEAD(&account_node->node);
                            }

                            memcpy(account_node->passwdex, cattr, strlen(cattr));
                            attr.ReleaseBuffer();

                            api_log_MsgInfo("passwdex:%s", account_node->passwdex);
                            continue;
                        }
                    }
                    xml.OutOfElem();

                    if (account_node)
                    {
                        if (NULL == bindinfo_node)
                        {
                            bindinfo_node = (struct cluamanbindinfo*)LUAMAN_ALLOC(sizeof(struct cluamanbindinfo));
                            if (NULL == bindinfo_node)
                            {
                                LUAMAN_FREE(account_node);
                                
                                AfxMessageBox("内存不足,程序即将退出");
                                exit(1);
                            }

                            memset(bindinfo_node, 0, sizeof(struct cluamanbindinfo));
                            INIT_LIST_HEAD(&bindinfo_node->node);
                            INIT_LIST_HEAD(&bindinfo_node->accountList);
                        }
                        
                        list_add_tail(&account_node->node, &bindinfo_node->accountList);
                        bindinfo_node->accountNum++;

                        api_log_MsgInfo("accountNum:%d", bindinfo_node->accountNum);
                    }
                }
                //***********************************************************************
            }
            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

            if (NULL != bindinfo_node)
            {
                list_add_tail(&bindinfo_node->node, &config->bindinfoList);
                config->bindinfoNum++;
            }
            
            xml.OutOfElem();
        }

        if (CString(KEY_MixCopyLoopNum) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->mixLoopNum = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("mixLoopNum:%d", config->mixLoopNum);
            continue;
        }

        if (CString(KEY_BindHostIdx) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->curBindHostIdx = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgInfo("curBindHostIdx:%d", config->curBindHostIdx);
            continue;
        }
        
        if (CString(KEY_MixCopy) == tag)
        {
            struct cluamanconfig* config_node = (struct cluamanconfig*)malloc(sizeof(struct cluamanconfig));
            if (NULL == config_node)
            {
                AfxMessageBox("内存不足,程序即将退出");
                exit(1);
            }
            memset(config_node, 0, sizeof(struct cluamanconfig));
            INIT_LIST_HEAD(&config_node->node);
            //INIT_LIST_HEAD(&config_node->accountList);
            INIT_LIST_HEAD(&config_node->bindinfoList);
            INIT_LIST_HEAD(&config_node->mixConfigList);
            
            xml.IntoElem();
            bok = LoadCopyConfig(NULL, xml, config_node);
            xml.OutOfElem();
            if (true != bok)
            {
                LUAMAN_FREE(config_node);
                
                api_log_MsgError("load mixcoy config failed");
                return bok;
            }

            list_add_tail(&config_node->node, &config->mixConfigList);
            config->mixConfigNum++;

            api_log_MsgInfo("mixConfigNum:%d", config->mixConfigNum);
        }
        ////////////////////////////////////////////////////////////////////////////////
    }

    return bok;
}

int api_LoadXML(std::string& file, char* tasktype, struct cluamanconfig* config)
{
    CMarkup xml;
    bool bok = false;

    if (!xml.Load(CString(file.c_str())))
    {
        api_log_MsgError("LoadXML %s failed", tasktype);
        return ERRNO_FAILED;
    }

    xml.ResetMainPos();
    if (!xml.FindElem(CString(KEY_Root)))
    {
        api_log_MsgError("LoadXML %s failed, KEY_Root:%s", tasktype, KEY_Root);
        return ERRNO_FAILED;
    }

    xml.IntoElem();
    while (xml.FindElem(CString(KEY_Copy)))
    {
        xml.IntoElem();
        bok = LoadCopyConfig(tasktype, xml, config);
        xml.OutOfElem();
        
        if (bok)
        {
            break;
        }
    }
    xml.OutOfElem();

    if (true != bok)
    {
        api_log_MsgDebug("LoadXML %s failed", tasktype);
        return ERRNO_FAILED;
    }

    api_log_MsgDebug("LoadXML %s success", tasktype);

    //api_PrintConfig(config);

    
    return ERRNO_SUCCESS;
}

static int SaveXML(std::string& file, struct cluamanconfig* config)
{
    CMarkup xml;
    bool bok = false;
    char* tasktype[] = TASK_TYPE_DESC_ARRAY();
    
    if (!xml.Load(CString(file.c_str())))
    {
        api_log_MsgError("load %s failed when saveXML, create it", file.c_str());
        return api_CreatXML(file, config);
    }

    xml.ResetMainPos();
    if (!xml.FindElem(CString(KEY_Root)))
    {
        return ERRNO_FAILED;
    }
    
    xml.IntoElem();
    while (xml.FindElem(CString(KEY_Copy)))
    {
        xml.IntoElem();
        while (xml.FindElem(NULL))
        {
            CString tag = xml.GetTagName();
            CString attr = xml.GetAttrib(CString(ATTR_VALUE));

            //bok = true;
            
            if (CString(KEY_copy_name) == tag)
            {
                char* cattr = attr.GetBuffer(BUFFER_MAX);
                //api_log_MsgInfo("update, cattr:%s, tasktype:%s", cattr, tasktype[config->taskType]);
                attr.ReleaseBuffer();

                if (CString(tasktype[config->taskType]) != attr)
                {
                    break;
                }

                bok = true;
                
                ITEM_UPDATE(xml, config->copyName);
                continue;
            }

            bok = true;

            if (CString(KEY_copyrolenr) == tag)
            {
                ITEM_UPDATE_D(xml, config->copyrolenr);
                continue;
            }
            
            if (CString(KEY_loop_num) == tag)
            {
                ITEM_UPDATE_D(xml, config->loop_num);
                continue;
            }

            if (CString(KEY_taskType) == tag)
            {
                ITEM_UPDATE_D(xml, config->taskType);
                continue;
            }

            if (CString(KEY_direction) == tag)
            {
                ITEM_UPDATE_D(xml, config->direction);
                continue;
            }

            if (CString(KEY_role) == tag)
            {
                ITEM_UPDATE_D(xml, config->role);
                continue;
            }

            if (CString(KEY_groupid) == tag)
            {
                ITEM_UPDATE_D(xml, config->groupid);
                continue;
            }

            if (CString(KEY_maxBlueFiled) == tag)
            {
                ITEM_UPDATE_D(xml, config->maxBlueFiled);
                continue;
            }

            if (CString(KEY_minBlue) == tag)
            {
                ITEM_UPDATE_D(xml, config->minBlue);
                continue;
            }

            if (CString(KEY_blueThreshold) == tag)
            {
                ITEM_UPDATE_D(xml, config->blueThreshold);
                continue;
            }

            if (CString(KEY_buddyName) == tag)
            {
                ITEM_UPDATE(xml, config->buddyName);
                continue;
            }
            
            if (CString(KEY_alarmServer) == tag)
            {
                ITEM_UPDATE(xml, config->alarmServer);
                continue;
            }

            if (CString(KEY_swjdIndex) == tag)
            {
                ITEM_UPDATE_D(xml, config->swjd_index);
                continue;
            }
            
            if (CString(KEY_alarmPort) == tag)
            {
                ITEM_UPDATE_D(xml, config->alarmPort);
                continue;
            }

            if (CString(KEY_alarmHostIndex) == tag)
            {
                ITEM_UPDATE_D(xml, config->alarmHostIndex);
                continue;
            }

            if (CString(KEY_playWndIndexVal) == tag)
            {
                ITEM_UPDATE_D(xml, config->playWndIndexVal);
                continue;
            }

            if (CString(KEY_Wait2Sleep) == tag)
            {
                ITEM_UPDATE_D(xml, config->Wait2Sleep);
                continue;
            }

            if (CString(KEY_exitGameCopyFull) == tag)
            {
                ITEM_UPDATE_D(xml, config->exitGameCopyFull);
                continue;
            }

            if (CString(KEY_cleanPackageWhenStart) == tag)
            {
                ITEM_UPDATE_D(xml, config->cleanPackageWhenStart);
                continue;
            }

            if (CString(KEY_callBabyDie) == tag)
            {
                ITEM_UPDATE_D(xml, config->callbabydie);
                continue;
            }
            
            if (CString(KEY_bluebloodWidth) == tag)
            {
                ITEM_UPDATE_D(xml, config->bluebloodWidth);
                continue;
            }

            if (CString(KEY_bluebloodLowLevel) == tag)
            {
                ITEM_UPDATE_D(xml, config->bluebloodLowLevel);
                continue;
            }

            if (CString(KEY_syncCountMax) == tag)
            {
                ITEM_UPDATE_D(xml, config->syncCountMax);
                continue;
            }

            if (CString(KEY_bluebloodChecktime) == tag)
            {
                ITEM_UPDATE_D(xml, config->bluebloodChecktime);
                continue;
            }

            if (CString(KEY_blockTimes) == tag)
            {
                ITEM_UPDATE_D(xml, config->blockTimes);
                continue;
            }

            if (CString(KEY_waitCopytime) == tag)
            {
                ITEM_UPDATE_D(xml, config->waitCopytime);
                continue;
            }

            if (CString(KEY_cleanPackageTime) == tag)
            {
                ITEM_UPDATE_D(xml, config->cleanPackageTime);
                continue;
            }

            if (CString(KEY_cleanPackageX) == tag)
            {
                ITEM_UPDATE_D(xml, config->cleanPackageX);
                continue;
            }

            ////////////////////////////////////////////////////////////////////
            //bindinfo
            if (CString(KEY_BindInfo) == tag)
            {
                xml.RemoveNode();
                
                /*
                xml.IntoElem();

                //解析BindInfo下各个标签
                while (xml.FindElem(NULL))
                {
                    CString itag = xml.GetTagName();
                    CString iattr = xml.GetAttrib(CString(ATTR_VALUE));

                    if (CString(KEY_BindWnd) == itag)
                    {
                        char* cattr = iattr.GetBuffer(BUFFER_MAX);

                        ITEM_UPDATE_D(xml, config->bindinfoNum);
                        attr.ReleaseBuffer();
                        continue;
                    }

                    
                }
                
                xml.OutOfElem();
                */
                
                continue;
            }

            if (CString(KEY_MixCopy) == tag)
            {
                xml.RemoveNode();
                continue;
            }
            
            //继续 MixCopyLoopNum
            if (CString(KEY_MixCopyLoopNum) == tag)
            {
                ITEM_UPDATE_D(xml, config->mixLoopNum);
                continue;
            }

            if (CString(KEY_BindHostIdx) == tag)
            {
                ITEM_UPDATE_D(xml, config->curBindHostIdx);
                continue;
            }
            ////////////////////////////////////////////////////////////////////
        }

        xml.OutOfElem();
        if (bok)
        {
            break;
        }
    }
    xml.OutOfElem();
    if (bok)
    {
        xml.Save(CString(file.c_str()));
        
        //(void)createBindinfoNode(file, config);
        //(void)createMixcopyNode(file, config);
        
        //api_log_MsgDebug("======= update XML finish");
        return ERRNO_SUCCESS;
    }

    //api_log_MsgDebug("------- create %s XML -------", file.c_str());
    //api_PrintConfig(config);

    {
        xml.IntoElem();
        xml.AddElem(CString(KEY_Copy));
        xml.IntoElem();
        
        ITEM_NEW(xml, KEY_copy_name, tasktype[config->taskType]);
        ITEM_NEW_D(xml, KEY_loop_num, config->loop_num);
        ITEM_NEW_D(xml, KEY_taskType, config->taskType);
        ITEM_NEW_D(xml, KEY_direction, config->direction);
        ITEM_NEW_D(xml, KEY_role, config->role);
        ITEM_NEW_D(xml, KEY_groupid, config->groupid);
        ITEM_NEW_D(xml, KEY_maxBlueFiled, config->maxBlueFiled);
        ITEM_NEW_D(xml, KEY_minBlue, config->minBlue);
        ITEM_NEW_D(xml, KEY_blueThreshold, config->blueThreshold);
        ITEM_NEW(xml, KEY_alarmServer, config->alarmServer);
        ITEM_NEW_D(xml, KEY_alarmPort, config->alarmPort);
        ITEM_NEW_D(xml, KEY_alarmHostIndex, config->alarmHostIndex);
        ITEM_NEW_D(xml, KEY_playWndIndexVal, config->playWndIndexVal);
        ITEM_NEW_D(xml, KEY_Wait2Sleep, config->Wait2Sleep);
        ITEM_NEW_D(xml, KEY_exitGameCopyFull, config->exitGameCopyFull);
        ITEM_NEW_D(xml, KEY_cleanPackageWhenStart, config->cleanPackageWhenStart);
        ITEM_NEW_D(xml, KEY_callBabyDie, config->callbabydie);
        ITEM_NEW_D(xml, KEY_bluebloodWidth, config->bluebloodWidth);
        ITEM_NEW_D(xml, KEY_bluebloodLowLevel, config->bluebloodLowLevel);
        ITEM_NEW_D(xml, KEY_syncCountMax, config->syncCountMax);
        ITEM_NEW_D(xml, KEY_bluebloodChecktime, config->bluebloodChecktime);
        ITEM_NEW_D(xml, KEY_blockTimes, config->blockTimes);
        ITEM_NEW_D(xml, KEY_waitCopytime, config->waitCopytime);
        ITEM_NEW_D(xml, KEY_cleanPackageTime, config->cleanPackageTime);
        ITEM_NEW_D(xml, KEY_cleanPackageX, config->cleanPackageX);
        ITEM_NEW_D(xml, KEY_BindHostIdx, config->curBindHostIdx);
        
        xml.OutOfElem();
        xml.OutOfElem();

        xml.Save(CString(file.c_str()));

        //(void)createBindinfoNode(file, config);
        //(void)createMixcopyNode(file, config);
    }

    return ERRNO_SUCCESS;
}

int api_SaveXML(std::string& file, struct cluamanconfig* config)
{
    int status = 0;

    //!!!!!减小作用域,xml完成析构后新建节点时可以打开xml文件
    {
        status = SaveXML(file, config);
        if (ERRNO_SUCCESS != status)
        {
            api_log_MsgError("SaveXML failed, status:%d", status);
            return status;
        }
    }
    
    //api_log_MsgDebug("api_SaveXML");
    {
        (void)createBindinfoNode(file, config);
        (void)createMixcopyNode(file, config);
    }

    //!!!!!!去掉多余的空行
    std::fstream targetFile(file.c_str(), std::fstream::in | std::fstream::out);
    std::string line;
    std::string temp;
    std::deque<std::string> noBlankLineQueue;
    auto StartPos = targetFile.tellp();
    
    while(std::getline(targetFile,line))
    {
        if(!line.empty())
        {
            noBlankLineQueue.push_back(line);
        }
    }
    
    targetFile.close();

    std::ofstream emptyFile(file.c_str());
    emptyFile.close();

    std::deque<std::string>::iterator begin = noBlankLineQueue.begin();
    std::deque<std::string>::iterator end = noBlankLineQueue.end();
    
    std::fstream target(file.c_str(), std::fstream::out | std::fstream::in);
    if(!target)
    {
        //api_log_MsgError("std::fstream target %s failed", file.c_str());
        goto finish;
    }

    while(begin != end)
    {
        temp = *begin;
        target << temp << std::endl;
        begin++;
    }

    target.close();

finish:
    return ERRNO_SUCCESS;
}

int api_EqualConfig(struct cluamanconfig* oconfig, struct cluamanconfig* nconfig)
{
    struct cluamanbindinfo* bpos = NULL, *bn = NULL;
    struct cluamanbindinfo* bposx = NULL, *bnx = NULL;
    struct cluamanaccountinfo* apos = NULL, *an = NULL;
    struct cluamanaccountinfo* aposx = NULL, *anx = NULL;
    struct cluamanconfig* cpos = NULL, *cn = NULL;
    struct cluamanconfig* cposx = NULL, *cnx = NULL;
    int i = 0, j = 0;
    int p = 0, q = 0;
    
    if (!(
        CHECK_FIELD_EQUAL(oconfig->copyrolenr   ,  nconfig->copyrolenr)  &&
        CHECK_FIELD_EQUAL(oconfig->taskType   ,  nconfig->taskType)  &&
        CHECK_FIELD_EQUAL(oconfig->selected_front_index  ,  nconfig->selected_front_index)  &&
        CHECK_FIELD_EQUAL(oconfig->direction  ,  nconfig->direction)  &&
        CHECK_FIELD_EQUAL(oconfig->role       ,  nconfig->role)  &&
        CHECK_FIELD_EQUAL(oconfig->groupid    ,  nconfig->groupid)  &&
        CHECK_FIELD_EQUAL(oconfig->maxBlueFiled  ,  nconfig->maxBlueFiled)  &&
        CHECK_FIELD_EQUAL(oconfig->minBlue       ,  nconfig->minBlue)  &&
        CHECK_FIELD_EQUAL(oconfig->blueThreshold ,  nconfig->blueThreshold)  &&
        CHECK_FIELD_EQUAL(oconfig->alarmPort       ,  nconfig->alarmPort)  &&
        CHECK_FIELD_EQUAL(oconfig->alarmHostIndex ,  nconfig->alarmHostIndex)  &&
        CHECK_FIELD_EQUAL(oconfig->loop_num   ,  nconfig->loop_num)  &&
        CHECK_FIELD_EQUAL(oconfig->playWndIndexVal ,  nconfig->playWndIndexVal)  &&
        CHECK_FIELD_EQUAL(oconfig->exitGameCopyFull ,  nconfig->exitGameCopyFull)  &&
        CHECK_FIELD_EQUAL(oconfig->Wait2Sleep ,  nconfig->Wait2Sleep)  &&
        CHECK_FIELD_EQUAL(oconfig->cleanPackageWhenStart ,  nconfig->cleanPackageWhenStart)  &&
        CHECK_FIELD_EQUAL(oconfig->callbabydie ,  nconfig->callbabydie)  &&
        CHECK_FIELD_EQUAL(oconfig->bluebloodWidth ,  nconfig->maxBlueFiled)  &&
        CHECK_FIELD_EQUAL(oconfig->bluebloodLowLevel ,  nconfig->blueThreshold)  &&
        CHECK_FIELD_EQUAL(oconfig->syncCountMax ,  nconfig->syncCountMax) &&
        CHECK_FIELD_EQUAL(oconfig->blockTimes ,  nconfig->blockTimes)  &&
        CHECK_FIELD_EQUAL(oconfig->waitCopytime ,  nconfig->waitCopytime)  &&
        CHECK_FIELD_EQUAL(oconfig->cleanPackageTime ,  nconfig->cleanPackageTime)  &&
        CHECK_FIELD_EQUAL(oconfig->cleanPackageX ,  nconfig->cleanPackageX)  &&
        CHECK_FIELD_EQUAL(oconfig->bluebloodChecktime ,  nconfig->bluebloodChecktime)  &&
        !strcmp(oconfig->alarmServer, nconfig->alarmServer)
    ))
    {
        return 0;
    }

    if (strcmp(oconfig->buddyName, nconfig->buddyName))
    {
        return 0;
    }
    
    if (strcmp(oconfig->copyName, nconfig->copyName))
    {
        return 0;
    }

    if (oconfig->swjd_index != nconfig->swjd_index)
    {
        return 0;
    }
    
    if (oconfig->bindinfoNum != nconfig->bindinfoNum)
    {
        return 0;
    }

    if (oconfig->mixConfigNum != nconfig->mixConfigNum)
    {
        return 0;
    }

    if (oconfig->mixLoopNum != nconfig->mixLoopNum)
    {
        return 0;
    }

    if (oconfig->curBindHostIdx != nconfig->curBindHostIdx)
    {
        return 0;
    }
    
    i = 0;
    
    list_for_each_entry_safe(bpos, bn, &oconfig->bindinfoList, node, struct cluamanbindinfo)
    {
        j = 0;
        
        list_for_each_entry_safe(bposx, bnx, &nconfig->bindinfoList, node, struct cluamanbindinfo)
        {
            if (i != j)
            {
                j++;
                continue;
            }

            if (bpos->bindWndnr != bposx->bindWndnr)
            {
                return 0;
            }

            if (bpos->accountNum != bposx->accountNum)
            {
                return 0;
            }

            p = 0;
            q = 0;
            
            //比较用户信息
            list_for_each_entry_safe(apos, an, &bpos->accountList, node, struct cluamanaccountinfo)
            {
                q = 0;
                
                list_for_each_entry_safe(aposx, anx, &bposx->accountList, node, struct cluamanaccountinfo)
                {
                    if (p != q)
                    {
                        q++;
                        continue;
                    }
                    if (!
                        (!strcmp(apos->user_name, aposx->user_name) &&
                            !strcmp(apos->passwd, aposx->passwd) &&
                            !strcmp(apos->passwdex, aposx->passwdex)
                        )
                    )
                    {
                        return 0;
                    }

                    break;
                }

                p++;
            }
            
            break;
        }
        
        i++;
    }

    i = 0;
    
    list_for_each_entry_safe(cpos, cn, &oconfig->mixConfigList, node, struct cluamanconfig)
    {
        j = 0;

        list_for_each_entry_safe(cposx, cnx, &nconfig->mixConfigList, node, struct cluamanconfig)
        {
            if (i != j)
            {
                j++;
                continue;
            }

            if (!api_EqualConfig(cpos, cposx))
            {
                return 0;
            }

            break;
        }

        i++;
    }

    return 1;
}

void api_UpdateConfig(struct cluamanconfig* oconfig, struct cluamanconfig* nconfig, int method)
{
    RETURN_IF_PTR_NULL(oconfig);
    RETURN_IF_PTR_NULL(nconfig);

    if (api_EqualConfig(oconfig, nconfig))
    {
        api_log_MsgInfo("config not change");
        return;
    }

    oconfig->copyrolenr   = nconfig->copyrolenr;
    oconfig->selected_front_index   = nconfig->selected_front_index;
    //195
    oconfig->taskType   = nconfig->taskType;
    oconfig->direction  = nconfig->direction;
    oconfig->role       = nconfig->role;
    oconfig->groupid    = nconfig->groupid;
    oconfig->maxBlueFiled  = nconfig->maxBlueFiled;
    oconfig->minBlue       = nconfig->minBlue;
    oconfig->blueThreshold = nconfig->blueThreshold;
    LUAMAN_MEMCPY(oconfig->alarmServer, nconfig->alarmServer, sizeof(nconfig->alarmServer));
    oconfig->alarmPort       = nconfig->alarmPort;
    oconfig->alarmHostIndex = nconfig->alarmHostIndex;
    oconfig->loop_num   = nconfig->loop_num;

    //021
    oconfig->playWndIndexVal = nconfig->playWndIndexVal;
    oconfig->exitGameCopyFull = nconfig->exitGameCopyFull;
    oconfig->Wait2Sleep = nconfig->Wait2Sleep;
    oconfig->cleanPackageWhenStart = nconfig->cleanPackageWhenStart;

    //1853配置
    oconfig->bluebloodWidth = nconfig->maxBlueFiled;
    oconfig->bluebloodLowLevel = nconfig->blueThreshold;
    oconfig->syncCountMax = nconfig->syncCountMax = 2;//默认为2
    oconfig->callbabydie = nconfig->callbabydie;
    
    oconfig->blockTimes = nconfig->blockTimes;
    oconfig->waitCopytime = nconfig->waitCopytime;
    oconfig->cleanPackageTime = nconfig->cleanPackageTime;
    oconfig->cleanPackageX = nconfig->cleanPackageX;
    oconfig->bluebloodChecktime = nconfig->bluebloodChecktime;

    oconfig->curBindHostIdx = nconfig->curBindHostIdx;
    
    if (method)
    {
        oconfig->ops.type       = nconfig->ops.type;
        oconfig->ops.init       = nconfig->ops.init;
        oconfig->ops.main       = nconfig->ops.main;
        oconfig->ops.exit       = nconfig->ops.exit;
    }

    if (ERRNO_SUCCESS != api_CloneConfig(oconfig, nconfig))
    {
        api_log_MsgError("update clone config failed");
        
        api_PrintConfig(oconfig);
        api_PrintConfig(nconfig);

        api_SaveXML(std::string(CONFIG_XML), nconfig);
        return;
    }
    
    api_SaveXML(std::string(CONFIG_XML), oconfig);

    //__api_log_MsgDebug("+++++++ update config finish");
    //api_PrintConfig(oconfig);
    
    return;
}

void api_PrintConfig(struct cluamanconfig* config)
{
    char* task[] = TASK_TYPE_DESC_ARRAY();
    char* dir[] = DIR_DESC_ARRAY();
    char* role[] = ROLE_DESC_ARRAY();
    int mix_task[] = MIX_TASKS();
    int i = 0;
    struct cluamanbindinfo* bpos = NULL, *bn = NULL, *bnode = NULL;
    struct cluamanaccountinfo* apos = NULL, *an = NULL;
    struct cluamanconfig* cpos = NULL, *cn = NULL;

    RETURN_IF_PTR_NULL(config);

    api_log_MsgInfo("++++++++++ Dump Config Parameters ++++++++++");
    api_log_MsgInfo("taskType       : %d : %s", config->taskType, task[config->taskType]);
    api_log_MsgInfo("selected_front_index: %u", config->selected_front_index);
    api_log_MsgInfo("loop_num       : %u", config->loop_num);
    api_log_MsgInfo("copyrolenr     : %d", config->copyrolenr);
    api_log_MsgInfo("direction      : %s", dir[config->direction]);
    api_log_MsgInfo("role           : %s", role[config->role]);
    api_log_MsgInfo("groupid        : %d", config->groupid);
    api_log_MsgInfo("maxBlueFiled   : %d", config->maxBlueFiled);
    api_log_MsgInfo("minBlue        : %d", config->minBlue);
    api_log_MsgInfo("blueThreshold  : %d", config->blueThreshold);
    api_log_MsgInfo("alarmServer    : %s", config->alarmServer);
    api_log_MsgInfo("alarmPort      : %d", config->alarmPort);
    api_log_MsgInfo("alarmHostIndex : %d", config->alarmHostIndex);

    api_log_MsgInfo("exitGameCopyFull      : %d", config->exitGameCopyFull);
    api_log_MsgInfo("cleanPackageWhenStart : %d", config->cleanPackageWhenStart);
    api_log_MsgInfo("callbabydie           : %d", config->callbabydie);
    api_log_MsgInfo("bluebloodLowLevel     : %d", config->bluebloodLowLevel);
    api_log_MsgInfo("syncCountMax          : %d", config->syncCountMax);
    api_log_MsgInfo("bluebloodChecktime    : %d", config->bluebloodChecktime);
    api_log_MsgInfo("blockTimes            : %d", config->blockTimes);
    api_log_MsgInfo("waitCopytime          : %d", config->waitCopytime);
    api_log_MsgInfo("cleanPackageTime      : %d", config->cleanPackageTime);
    api_log_MsgInfo("cleanPackageX         : 0x%x", config->cleanPackageX);

    ////////////////////////////////////////////////////////////////////////////
    //组合任务
    api_log_MsgInfo("copyName       : %s", config->copyName);
    api_log_MsgInfo("curBindWnd     : %d", config->curBindWnd);
    api_log_MsgInfo("curBindHostIdx : %d", config->curBindHostIdx);
    api_log_MsgInfo("waitEmail      : %d", config->waitEmail);
    api_log_MsgInfo("buddyName      : %s", config->buddyName);
    api_log_MsgInfo("swjd_index     : %d", config->swjd_index);
    
    api_log_MsgInfo("init           : %p", config->ops.init);
    api_log_MsgInfo("main           : %p", config->ops.main);
    api_log_MsgInfo("exit           : %p", config->ops.exit);

    list_for_each_entry_safe(bpos, bn, &config->bindinfoList, node, struct cluamanbindinfo)
    {
        api_log_MsgInfo(" bindWndnr      : %d", bpos->bindWndnr);
        api_log_MsgInfo(" accountNum     : %d", bpos->accountNum);
        
        list_for_each_entry_safe(apos, an, &bpos->accountList, node, struct cluamanaccountinfo)
        {
            api_log_MsgInfo("  UserName     : %s", apos->user_name);
            api_log_MsgInfo("  passwd       : %s", apos->passwd);
            api_log_MsgInfo("  passwdex     : %s", apos->passwdex);
        }
    }
        
    for (i = 0; i < sizeof(mix_task)/sizeof(mix_task[0]); i++)
    {
        if (mix_task[i] != config->taskType)
        {
            continue;
        }
        
        api_log_MsgInfo("mixConfigNum   : %d", config->mixConfigNum);
        api_log_MsgInfo("mixLoopNum     : %d", config->mixLoopNum);

        list_for_each_entry_safe(cpos, cn, &config->mixConfigList, node, struct cluamanconfig)
        {
            api_PrintConfig(cpos);
        }
    }
    
    return;
}

int api_UpdateBindinfo(struct cluamanconfig* config, struct cluamanbindinfo* bindinfo)
{
    struct cluamanbindinfo* bpos = NULL, *bn = NULL, *bnode = NULL;
    struct cluamanaccountinfo* apos = NULL, *an = NULL, *anode = NULL;
    struct cluamanconfig* cpos = NULL, *cn = NULL, *cnode = NULL;
    
    RETURN_WITH_IF_PTR_NULL(config, ERRNO_INVALID);

    list_for_each_entry_safe(bpos, bn, &config->bindinfoList, node, struct cluamanbindinfo)
    {
        if (bpos->bindWndnr != bindinfo->bindWndnr)
        {
            continue;
        }

        api_log_MsgDebug("update bindwdr bindinfo:%d", bindinfo->bindWndnr);
        
        //先删掉原有的账户信息
        list_for_each_entry_safe(apos, an, &bpos->accountList, node, struct cluamanaccountinfo)
        {
            bpos->accountNum--;
            list_del(&apos->node);
            LUAMAN_FREE(apos);
        }

        //新增新的账户信息
        list_for_each_entry_safe(apos, an, &bindinfo->accountList, node, struct cluamanaccountinfo)
        {
            bindinfo->accountNum--;
            list_del(&apos->node);

            bpos->accountNum++;
            list_add_tail(&apos->node, &bpos->accountList);
        }

        return ERRNO_SUCCESS;
    }

    //如果没有找到则新建bind节点
    bnode = (struct cluamanbindinfo*)LUAMAN_ALLOC(sizeof(struct cluamanbindinfo));
    if (NULL == bnode)
    {
        api_log_MsgError("alloc cluamanbindinfo failed when api_UpdateBindinfo");
        return ERRNO_NOMEM;
    }

    LUAMAN_MEMSET(bnode, 0, sizeof(struct cluamanbindinfo));
    INIT_LIST_HEAD(&bnode->node);
    INIT_LIST_HEAD(&bnode->accountList);

    bnode->bindWndnr = bindinfo->bindWndnr;

    api_log_MsgDebug("add bindwdr bindinfo:%d, accountNum:%d", bindinfo->bindWndnr, bindinfo->accountNum);
    
    //新增新的账户信息
    list_for_each_entry_safe(apos, an, &bindinfo->accountList, node, struct cluamanaccountinfo)
    {
        bindinfo->accountNum--;
        list_del(&apos->node);

        bnode->accountNum++;
        list_add_tail(&apos->node, &bnode->accountList);

        api_log_MsgDebug("user_name  : %s", apos->user_name);
        api_log_MsgDebug("passwd     : %s", apos->passwd);
        api_log_MsgDebug("passwdex   : %s", apos->passwdex);
    }

    list_add_tail(&bnode->node, &config->bindinfoList);
    config->bindinfoNum++;
    
    return ERRNO_SUCCESS;
}

void api_ClearMixConfig(struct cluamanconfig* config)
{
    struct cluamanbindinfo* bpos = NULL, *bn = NULL, *bnode = NULL;
    struct cluamanaccountinfo* apos = NULL, *an = NULL, *anode = NULL;
    struct cluamanconfig* cpos = NULL, *cn = NULL, *cnode = NULL;
    
    RETURN_IF_PTR_NULL(config);

    list_for_each_entry_safe(bpos, bn, &config->bindinfoList, node, struct cluamanbindinfo)
    {
        list_del(&bpos->node);
        
        list_for_each_entry_safe(apos, an, &bpos->accountList, node, struct cluamanaccountinfo)
        {
            bpos->accountNum--;
            list_del(&apos->node);
            LUAMAN_FREE(apos);
        }

        LUAMAN_FREE(bpos);
        config->bindinfoNum--;
    }
    
    list_for_each_entry_safe(cpos, cn, &config->mixConfigList, node, struct cluamanconfig)
    {
        config->mixConfigNum--;
        list_del(&cpos->node);
        api_ClearMixConfig(cpos);
    }
    
    return;
}

int api_CloneConfigAppendBindinfo(struct cluamanconfig* oconfig, struct cluamanconfig* nconfig)
{
    int status = 0;
    struct cluamanbindinfo* bpos = NULL, *bn = NULL, *bnode = NULL;
    struct cluamanaccountinfo* apos = NULL, *an = NULL, *anode = NULL;
    struct cluamanconfig* cpos = NULL, *cn = NULL, *cnode = NULL;
    struct list_head bindinfoList;
    char* tasktype[] = TASK_TYPE_DESC_ARRAY();
    struct cluamanoperations ops[] = LUAMAN_TASK_OPERATIONS();
    int i = 0;
    
    RETURN_WITH_IF_PTR_NULL(oconfig, ERRNO_INVALID);
    RETURN_WITH_IF_PTR_NULL(nconfig, ERRNO_INVALID);

    //////////////////////////////////////////////////////////////////////////////
    //组合任务
    INIT_LIST_HEAD(&bindinfoList);

    oconfig->bindinfoNum = nconfig->bindinfoNum;
    oconfig->curBindWnd = nconfig->curBindWnd;
    oconfig->curBindHostIdx = nconfig->curBindHostIdx;
    
    list_for_each_entry_safe(bpos, bn, &nconfig->bindinfoList, node, struct cluamanbindinfo)
    {
        bnode = (struct cluamanbindinfo*)LUAMAN_ALLOC(sizeof(struct cluamanbindinfo));
        if (NULL == bnode)
        {
            api_log_MsgError("clone cluamanbindinfo failed for no memory");
            goto failed;
        }

        memset(bnode, 0, sizeof(struct cluamanbindinfo));
        INIT_LIST_HEAD(&bnode->node);
        INIT_LIST_HEAD(&bnode->accountList);
        bnode->bindWndnr = bpos->bindWndnr;
        
        list_for_each_entry_safe(apos, an, &bpos->accountList, node, struct cluamanaccountinfo)
        {
            anode = (struct cluamanaccountinfo*)LUAMAN_ALLOC(sizeof(struct cluamanaccountinfo));
            if (NULL == anode)
            {
                api_log_MsgError("clone cluamanaccountinfo failed for no memory");
                goto failed;
            }

            memset(anode, 0, sizeof(struct cluamanaccountinfo));
            INIT_LIST_HEAD(&anode->node);
            
            memcpy(anode->user_name, apos->user_name, strlen(apos->user_name) + 1);
            memcpy(anode->passwd, apos->passwd, strlen(apos->passwd) + 1);
            memcpy(anode->passwdex, apos->passwdex, strlen(apos->passwdex) + 1);
            
            list_add_tail(&anode->node, &bnode->accountList);
            bnode->accountNum++;
        }

        list_add_tail(&bnode->node, &bindinfoList);
    }

    list_for_each_entry_safe(bpos, bn, &oconfig->bindinfoList, node, struct cluamanbindinfo)
    {
        list_del(&bpos->node);
        
        list_for_each_entry_safe(apos, an, &bpos->accountList, node, struct cluamanaccountinfo)
        {
            bpos->accountNum--;
            list_del(&apos->node);
            LUAMAN_FREE(apos);
        }

        LUAMAN_FREE(bpos);
        oconfig->bindinfoNum--;
    }
    
    list_for_each_entry_safe(bpos, bn, &bindinfoList, node, struct cluamanbindinfo)
    {
        list_del(&bpos->node);
        list_add_tail(&bpos->node, &oconfig->bindinfoList);
        oconfig->bindinfoNum++;
    }

    return ERRNO_SUCCESS;
    
failed:
    list_for_each_entry_safe(bpos, bn, &bindinfoList, node, struct cluamanbindinfo)
    {
        list_del(&bpos->node);
        list_for_each_entry_safe(apos, an, &bpos->accountList, node, struct cluamanaccountinfo)
        {
            bpos->accountNum--;
            list_del(&apos->node);
            LUAMAN_FREE(apos);
        }
        LUAMAN_FREE(bpos);
    }
    
    return ERRNO_NOMEM;
}


int api_CloneConfig(struct cluamanconfig* oconfig, struct cluamanconfig* nconfig)
{
    int status = 0;
    struct cluamanbindinfo* bpos = NULL, *bn = NULL, *bnode = NULL;
    struct cluamanaccountinfo* apos = NULL, *an = NULL, *anode = NULL;
    struct cluamanconfig* cpos = NULL, *cn = NULL, *cnode = NULL;
    struct list_head bindinfoList;
    //struct list_head accountList;
    struct list_head mixConfigList;
    char* tasktype[] = TASK_TYPE_DESC_ARRAY();
    struct cluamanoperations ops[] = LUAMAN_TASK_OPERATIONS();
    int i = 0;
    
    RETURN_WITH_IF_PTR_NULL(oconfig, ERRNO_INVALID);
    RETURN_WITH_IF_PTR_NULL(nconfig, ERRNO_INVALID);

    oconfig->copyrolenr   = nconfig->copyrolenr;
    oconfig->selected_front_index   = nconfig->selected_front_index;
    //195
    oconfig->taskType   = nconfig->taskType;
    oconfig->direction  = nconfig->direction;
    oconfig->role       = nconfig->role;
    oconfig->groupid    = nconfig->groupid;
    oconfig->maxBlueFiled  = nconfig->maxBlueFiled;
    oconfig->minBlue       = nconfig->minBlue;
    oconfig->blueThreshold = nconfig->blueThreshold;
    LUAMAN_MEMCPY(oconfig->alarmServer, nconfig->alarmServer, sizeof(nconfig->alarmServer));
    oconfig->alarmPort       = nconfig->alarmPort;
    oconfig->alarmHostIndex = nconfig->alarmHostIndex;
    oconfig->loop_num   = nconfig->loop_num;

    //021
    oconfig->playWndIndexVal = nconfig->playWndIndexVal;
    oconfig->exitGameCopyFull = nconfig->exitGameCopyFull;
    oconfig->Wait2Sleep = nconfig->Wait2Sleep;
    oconfig->cleanPackageWhenStart = nconfig->cleanPackageWhenStart;

    //1853配置
    oconfig->bluebloodWidth = nconfig->maxBlueFiled;
    oconfig->bluebloodLowLevel = nconfig->blueThreshold;
    oconfig->syncCountMax = nconfig->syncCountMax = 2;//默认为2
    oconfig->callbabydie = nconfig->callbabydie;
    
    oconfig->blockTimes = nconfig->blockTimes;
    oconfig->waitCopytime = nconfig->waitCopytime;
    oconfig->cleanPackageTime = nconfig->cleanPackageTime;
    oconfig->cleanPackageX = nconfig->cleanPackageX;
    oconfig->bluebloodChecktime = nconfig->bluebloodChecktime;

    for (i = 0; i < LUAMAN_DIM(ops); i++)
    {
        if (oconfig->taskType != ops[i].type)
        {
            continue;
        }
        
        oconfig->ops.init = ops[i].init;
        oconfig->ops.main = ops[i].main;
        oconfig->ops.exit = ops[i].exit;
    }
    
    //////////////////////////////////////////////////////////////////////////////
    //组合任务
    INIT_LIST_HEAD(&bindinfoList);
    //INIT_LIST_HEAD(&accountList);
    INIT_LIST_HEAD(&mixConfigList);

    memcpy(oconfig->buddyName, nconfig->buddyName, strlen(nconfig->buddyName) + 1);
    memcpy(oconfig->copyName, tasktype[oconfig->taskType], strlen(tasktype[oconfig->taskType]) + 1);
    oconfig->swjd_index = nconfig->swjd_index;
    oconfig->mixConfigNum = nconfig->mixConfigNum;
    oconfig->bindinfoNum = nconfig->bindinfoNum;
    oconfig->mixLoopNum = nconfig->mixLoopNum;

    oconfig->curBindWnd = nconfig->curBindWnd;
    oconfig->waitEmail = nconfig->waitEmail;
    oconfig->curBindHostIdx = nconfig->curBindHostIdx;
    
    list_for_each_entry_safe(bpos, bn, &nconfig->bindinfoList, node, struct cluamanbindinfo)
    {
        bnode = (struct cluamanbindinfo*)LUAMAN_ALLOC(sizeof(struct cluamanbindinfo));
        if (NULL == bnode)
        {
            api_log_MsgError("clone cluamanbindinfo failed for no memory");
            goto failed;
        }

        memset(bnode, 0, sizeof(struct cluamanbindinfo));
        INIT_LIST_HEAD(&bnode->node);
        INIT_LIST_HEAD(&bnode->accountList);
        bnode->bindWndnr = bpos->bindWndnr;
        
        list_for_each_entry_safe(apos, an, &bpos->accountList, node, struct cluamanaccountinfo)
        {
            anode = (struct cluamanaccountinfo*)LUAMAN_ALLOC(sizeof(struct cluamanaccountinfo));
            if (NULL == anode)
            {
                api_log_MsgError("clone cluamanaccountinfo failed for no memory");
                goto failed;
            }

            memset(anode, 0, sizeof(struct cluamanaccountinfo));
            INIT_LIST_HEAD(&anode->node);
            
            memcpy(anode->user_name, apos->user_name, strlen(apos->user_name) + 1);
            memcpy(anode->passwd, apos->passwd, strlen(apos->passwd) + 1);
            memcpy(anode->passwdex, apos->passwdex, strlen(apos->passwdex) + 1);
            
            list_add_tail(&anode->node, &bnode->accountList);
            bnode->accountNum++;
        }

        list_add_tail(&bnode->node, &bindinfoList);
    }
    
    list_for_each_entry_safe(cpos, cn, &nconfig->mixConfigList, node, struct cluamanconfig)
    {
        cnode = (struct cluamanconfig*)LUAMAN_ALLOC(sizeof(struct cluamanconfig));
        if (NULL == cnode)
        {
            api_log_MsgError("clone cluamanconfig failed for no memory");
            goto failed;
        }

        memset(cnode, 0, sizeof(struct cluamanconfig));
        INIT_LIST_HEAD(&cnode->node);
        //INIT_LIST_HEAD(&cnode->accountList);
        INIT_LIST_HEAD(&cnode->bindinfoList);
        INIT_LIST_HEAD(&cnode->mixConfigList);
        
        status = api_CloneConfig(cnode, cpos);
        if (ERRNO_SUCCESS != status)
        {
            api_log_MsgError("clone config failed, status:%d", status);
            goto failed;
        }

        list_add_tail(&cnode->node, &mixConfigList);
    }
    
    //清空组合任务信息
    api_ClearMixConfig(oconfig);

    list_for_each_entry_safe(bpos, bn, &bindinfoList, node, struct cluamanbindinfo)
    {
        list_del(&bpos->node);
        list_add_tail(&bpos->node, &oconfig->bindinfoList);
        oconfig->bindinfoNum++;
    }

    list_for_each_entry_safe(cpos, cn, &mixConfigList, node, struct cluamanconfig)
    {
        list_add_tail(&cpos->node, &oconfig->mixConfigList);
        oconfig->mixConfigNum++;
    }
    
    return ERRNO_SUCCESS;
    
failed:
    list_for_each_entry_safe(bpos, bn, &bindinfoList, node, struct cluamanbindinfo)
    {
        list_del(&bpos->node);
        list_for_each_entry_safe(apos, an, &bpos->accountList, node, struct cluamanaccountinfo)
        {
            bpos->accountNum--;
            list_del(&apos->node);
            LUAMAN_FREE(apos);
        }
        LUAMAN_FREE(bpos);
    }

    list_for_each_entry_safe(cpos, cn, &mixConfigList, node, struct cluamanconfig)
    {
        list_del(&cpos->node);
        api_ClearMixConfig(cpos);
        LUAMAN_FREE(cpos);
    }
    
    return ERRNO_NOMEM;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
static HANDLE exCommonlock;
int init_exCommonLock(void)
{
    exCommonlock = CreateMutex(NULL, FALSE, NULL);
    if (!exCommonlock)
    {
        api_log_MsgError("init_exCommonLock failed");
        return ERRNO_FAILED;
    }

    api_log_MsgDebug("init_exCommonLock success");
    return 0;
}

static bool __load_exCommonConfig(int bindwnd, CMarkup& xml, struct exCommonConfig* config)
{
    bool bok = false;
    
    while (xml.FindElem(NULL))
    {
        CString tag = xml.GetTagName();
        CString attr = xml.GetAttrib(CString(ATTR_VALUE));

        /*char* ctag = tag.GetBuffer(BUFFER_MAX);
        api_log_MsgDebug("ctag   : %s", ctag);
        tag.ReleaseBuffer();
        
        char* ccattr = attr.GetBuffer(BUFFER_MAX);
        api_log_MsgDebug("ccattr : %s", ccattr);
        attr.ReleaseBuffer();*/
        
        if (CString(KEY_exCommon_Bindwnd) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            if (bindwnd != atoi(cattr))
            {
                attr.ReleaseBuffer();
                break;
            }

            config->bindwnd = bindwnd;
            attr.ReleaseBuffer();
            continue;
        }

        bok = true;
        if (CString(KEY_exCommon_Lastnr) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->last_nr = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->last_nr:%d", config->last_nr);
            continue;
        }

        if (CString(KEY_exCommon_Lastroleno) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            config->last_roleno = atoi(cattr);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->last_roleno:%d", config->last_roleno);
            continue;
        }

        if (CString(KEY_exCommon_Username) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->user_name, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->user_name:%s", config->user_name);
            continue;
        }

        if (CString(KEY_exCommon_Password) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->passwd, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->passwd:%s", config->passwd);
            continue;
        }

        if (CString(KEY_exCommon_Passwordex) == tag)
        {
            char* cattr = attr.GetBuffer(BUFFER_MAX);
            memcpy(config->passwdex, cattr, strlen(cattr) + 1);
            attr.ReleaseBuffer();
        
            api_log_MsgDebug("config->passwdex:%s", config->passwdex);
            continue;
        }
    }

    return bok;
}

static bool _load_exCommonConfig(int bindwnd, std::string& file, struct exCommonConfig* config)
{
    bool bok = false;
    CMarkup xml;
    
    if (!xml.Load(CString(file.c_str())))
    {
        api_log_MsgError("LoadXML %s failed", file.c_str());
        return false;
    }

    xml.ResetMainPos();
    if (!xml.FindElem(CString(KEY_exCommon_Root)))
    {
        api_log_MsgError("LoadXML %s failed, KEY_Root:%s", file.c_str(), KEY_exCommon_Root);
        return false;
    }

    xml.IntoElem();
    while (xml.FindElem(CString(KEY_exCommon_Account)))
    {
        xml.IntoElem();
        bok = __load_exCommonConfig(bindwnd, xml, config);
        xml.OutOfElem();
        
        if (bok)
        {
            break;
        }
    }
    xml.OutOfElem();

    if (true != bok)
    {
        api_log_MsgDebug("LoadXML %s failed", file.c_str());
        return false;
    }

    api_log_MsgDebug("LoadXML %s success", file.c_str());
    return true;
}

bool load_exCommonConfig(int bindwnd, std::string& file, struct exCommonConfig* config)
{
    bool bok = false;

    memset(config, 0, sizeof(struct exCommonConfig));
    
    LUAMAN_LOCK(exCommonlock);
    bok = _load_exCommonConfig(bindwnd, file, config);
    LUAMAN_UNLOCK(exCommonlock);
    
    return bok;
}

void print_exCommonConfig(struct exCommonConfig* config)
{
    api_log_MsgDebug("bindwnd     : %d", config->bindwnd);
    api_log_MsgDebug("last_nr     : %d", config->last_nr);
    api_log_MsgDebug("last_roleno : %d", config->last_roleno);
    api_log_MsgDebug("user_name   : %s", config->user_name);
    api_log_MsgDebug("passwd      : %s", config->passwd);
    api_log_MsgDebug("passwdex    : %s", config->passwdex);
    
    return;
}

static int __creatXML(std::string& file, struct exCommonConfig* config)
{
    CMarkup xml;
    xml.SetDoc(CString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"));
    xml.AddElem(CString(KEY_exCommon_Root));

    xml.IntoElem();
    xml.AddElem(CString(KEY_exCommon_Account));
    xml.IntoElem();
    
    ITEM_NEW_D(xml, KEY_exCommon_Bindwnd, config->bindwnd);
    ITEM_NEW_D(xml, KEY_exCommon_Lastnr, config->last_nr);
    ITEM_NEW_D(xml, KEY_exCommon_Lastroleno, config->last_roleno);
    
    ITEM_NEW(xml, KEY_exCommon_Username, config->user_name);
    ITEM_NEW(xml, KEY_exCommon_Password, config->passwd);
    ITEM_NEW(xml, KEY_exCommon_Passwordex, config->passwdex);
    
    xml.OutOfElem();
    xml.OutOfElem();

    xml.Save(CString(file.c_str()));
    return ERRNO_SUCCESS;
}

static int __save_exCommonConfig(std::string& file, struct exCommonConfig* config)
{
    CMarkup xml;
    bool bok = false;
    int bindwnd = -1;
    
    if (!xml.Load(CString(file.c_str())))
    {
        api_log_MsgError("load %s failed when saveXML", file.c_str());
        return __creatXML(file, config);
    }

    xml.ResetMainPos();
    if (!xml.FindElem(CString(KEY_exCommon_Root)))
    {
        return ERRNO_FAILED;
    }

    xml.IntoElem();
    while (xml.FindElem(CString(KEY_exCommon_Account)))
    {
        xml.IntoElem();
        while (xml.FindElem(NULL))
        {
            CString tag = xml.GetTagName();
            CString attr = xml.GetAttrib(CString(ATTR_VALUE));

            if (CString(KEY_exCommon_Bindwnd) == tag)
            {
                char* cbindwnd = attr.GetBuffer(BUFFER_MAX);
                bindwnd = atoi(cbindwnd);
                attr.ReleaseBuffer();

                if (bindwnd != config->bindwnd)
                {
                    break;
                }
                
                continue;
            }

            bok = true;
            
            if (CString(KEY_exCommon_Lastnr) == tag)
            {
                ITEM_UPDATE_D(xml, config->last_nr);
                continue;
            }

            if (CString(KEY_exCommon_Lastroleno) == tag)
            {
                ITEM_UPDATE_D(xml, config->last_roleno);
                continue;
            }

            if (CString(KEY_exCommon_Username) == tag)
            {
                ITEM_UPDATE(xml, config->user_name);
                continue;
            }

            if (CString(KEY_exCommon_Password) == tag)
            {
                ITEM_UPDATE(xml, config->passwd);
                continue;
            }

            if (CString(KEY_exCommon_Passwordex) == tag)
            {
                ITEM_UPDATE(xml, config->passwdex);
                continue;
            }
        }

        xml.OutOfElem();
        if (bok)
        {
            break;
        }
    }

    xml.OutOfElem();
    if (bok)
    {
        xml.Save(CString(file.c_str()));
        return ERRNO_SUCCESS;
    }

    {
        xml.IntoElem();
        xml.AddElem(CString(KEY_exCommon_Account));
        xml.IntoElem();
        
        ITEM_NEW_D(xml, KEY_exCommon_Bindwnd, config->bindwnd);
        ITEM_NEW_D(xml, KEY_exCommon_Lastnr, config->last_nr);
        ITEM_NEW_D(xml, KEY_exCommon_Lastroleno, config->last_roleno);
        ITEM_NEW(xml, KEY_exCommon_Username, config->user_name);
        ITEM_NEW(xml, KEY_exCommon_Password, config->passwd);
        ITEM_NEW(xml, KEY_exCommon_Passwordex, config->passwdex);
        
        xml.OutOfElem();
        xml.OutOfElem();

        xml.Save(CString(file.c_str()));
    }

    return ERRNO_SUCCESS;
}

int save_exCommonConfig(std::string& file, struct exCommonConfig* config)
{
    int bok = 0;

    LUAMAN_LOCK(exCommonlock);
    bok = __save_exCommonConfig(file, config);
    LUAMAN_UNLOCK(exCommonlock);

    api_log_MsgDebug("save_exCommonConfig %s finish, bok:%d", file.c_str(), bok);
    print_exCommonConfig(config);
    
    return bok;
}

