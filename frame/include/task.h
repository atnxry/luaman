#ifndef __TASK_H__
#define __TASK_H__
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <time.h>
#include "xcommon.h"
#include "xlist.h"
#include "xlog.h"
#include "ThreadCtrl.h"
#include "config.h"
#include "dm.h"
#include <process.h>
#include "task195manager.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "snail_client_context.h"
#include "snail_client_manager.h"
#include "task1853_common.h"
#include "task1853_client_context.h"
#include "task1853_client_manager.h"
#include "taskFire.h"
#include "taskSnapshot.h"
#include "tasksimplefight.h"
#include "taskrefugeboat.h"
#include "taskTeamGroup.h"

#define FONT_FIND     (1) /* 结束条件找字 */
#define ADJUST_POS    (1) /* 归位 */
#define NO_FONT_FIND  (0) /* 结束条件不找字 */
#define NO_ADJUST_POS (0) /* 不归位 */

#define UFONT         (1)
#define NO_UFONT      (0)

struct deltaval
{
    int deltax;
    int deltay;
};

struct posxy_t
{
    long x;
    long y;
};

struct font_info_t
{
    char* word;
    char* font;
    char* color_offset;
};

struct font_info_of_fight_t
{
    int findfont;
    int timeout;
    int adjust_pos;
    struct font_info_t font;
};

struct font_info_of_fight_ex_t
{
    int findfont;
    int timeout;
    int adjust_pos;
    struct font_info_t font;
    int ufindfont;
    struct font_info_t ufont;
};

//单位:秒
#define FOR_CHECK_TIMEOUT_VAL   (60)

#define for_timeout_check_start(__luaman_task) {\
    time_t _check_start = time(NULL);\
    for (;;)

#define for_timeout_check(__luaman_task, _deltatime) do{\
    time_t _check_stop = time(NULL);\
    double deltaTime = difftime(time(NULL), _check_start);\
    if (deltaTime >= _deltatime)\
    {\
        api_log_MsgDebug("[%d --> %s]:循环:超时:%d --> %d秒:检查:触发", \
            __luaman_task->bindWnd, __luaman_task->user_name, \
            _check_start, _deltatime);\
    }\
}while(0)

#define for_timeout_check_end() }

int check_alarm(Idmsoft* dmobj, void* ctxt);

#define Check_Terminate_Thread_With_Alram(__task, __dmobj) do{\
    Check_Terminate_Thread(__task, __dmobj);\
    check_alarm(__dmobj, __task);\
}while(0)

#define Check_Terminate_Thread(__task, __dmobj) do{\
    struct cluamantaskctxt* __luaman_taskctxt = &(__task)->ctxt;\
    struct cluamanconfig* __luaman_config = (struct cluamanconfig*)(__luaman_taskctxt->context);\
    if (__luaman_taskctxt->stop)\
    {\
        if (TASK_TYPE_000 == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("task000 terminated, line:%d", __LINE__);\
            _endthreadex(0);\
        }\
        if (TASK_TYPE_SIGNIN == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("tasksignin terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_SAKURA == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("tasksakura terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_DOTASK == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("taskdotask terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_SIGNLE == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("tasksignle terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_175 == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("tasks175 terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_FIRE == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("tasksfire terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_021 == __luaman_config->taskType)\
        {\
            snail_FreeClientContext((__task)->hwnd);\
            api_log_MsgDebug("task021 terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_TEAMGROUP == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("taskTeamGroup terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_195 == __luaman_config->taskType)\
        {\
            (void)PutContext(__luaman_config->groupid, (__task)->hwnd);\
            api_log_MsgDebug("task195 terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_1853 == __luaman_config->taskType)\
        {\
            Tz1853_FreeClientContext((__task)->hwnd);\
            api_log_MsgDebug("task1853 terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_REFUGEBOAT == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("taskRefugeboat terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_PIRATE == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("taskPirate terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_EXPIRATE == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("taskExPirate terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_SNAPSHOT == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("taskSnapshot terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_SIMPLE_FIGHT == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("taskSimpleFight terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_EXSINGLE == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("taskExSignle terminated, line:%d", __LINE__);\
        }\
        if (TASK_TYPE_EX175 == __luaman_config->taskType)\
        {\
            api_log_MsgDebug("taskEx175 terminated, line:%d", __LINE__);\
        }\
        (__dmobj)->LockInput(0);\
        (__dmobj)->UnBindWindow();\
        _endthreadex(0);\
    }\
}while(0)

#define Delay_With_Terminate_Check(_sec_, __task, __dmobj) do{\
    int __i = 0;\
    for (__i = 0; __i < (_sec_); ++__i)\
    {\
        Delay(1000);\
        Check_Terminate_Thread(__task, __dmobj);\
    }\
}while(0)


#define Delay_With_Terminate_Check_Alram(_sec_, __task, __dmobj) do{\
    Delay_With_Terminate_Check(_sec_, __task, __dmobj);\
    check_alarm(__dmobj, __task);\
}while(0)


#define LUAMAN_TASK_OPERATIONS() \
{ \
    { \
        TASK_TYPE_REFUGEBOAT, \
        api_TaskRefugeboatInit, \
        api_TaskRefugeboatMain, \
        api_TaskRefugeboatExit, \
    }, \
    { \
        TASK_TYPE_SIMPLE_FIGHT, \
        api_TaskSimpleFightInit, \
        api_TaskSimpleFightMain, \
        api_TaskSimpleFightExit, \
    }, \
    { \
        TASK_TYPE_SNAPSHOT, \
        api_TaskSnapshotInit, \
        api_TaskSnapshotMain, \
        api_TaskSnapshotExit, \
    }, \
    { \
        TASK_TYPE_175, \
        api_Task175Init, \
        api_Task175Main, \
        api_Task175Exit, \
    }, \
    { \
        TASK_TYPE_FIRE, \
        api_TaskFireInit, \
        api_TaskFireMain, \
        api_TaskFireExit, \
    }, \
    { \
        TASK_TYPE_195, \
        api_Task195Init, \
        api_Task195Main, \
        api_Task195Exit, \
    }, \
    { \
        TASK_TYPE_021, \
        api_Task021Init, \
        api_Task021Main, \
        api_Task021Exit, \
    }, \
    { \
        TASK_TYPE_TEAMGROUP, \
        api_TaskTeamGroupInit, \
        api_TaskTeamGroupMain, \
        api_TaskTeamGroupExit, \
    }, \
    { \
        TASK_TYPE_1853, \
        api_Task1853Init, \
        api_Task1853Main, \
        api_Task1853Exit, \
    }, \
    { \
        TASK_TYPE_000, \
        api_Task000Init, \
        api_Task000Main, \
        api_Task000Exit, \
    }, \
    { \
        TASK_TYPE_SIGNIN, \
        api_TaskSigninInit, \
        api_TaskSigninMain, \
        api_TaskSigninExit, \
    }, \
    { \
        TASK_TYPE_SAKURA, \
        api_TaskSakuraInit, \
        api_TaskSakuraMain, \
        api_TaskSakuraExit, \
    }, \
    { \
        TASK_TYPE_DOTASK, \
        api_TaskDotaskInit, \
        api_TaskDotaskMain, \
        api_TaskDotaskExit, \
    }, \
    { \
        TASK_TYPE_FIGHT, \
        api_TaskFightInit, \
        api_TaskFightMain, \
        api_TaskFightExit, \
    }, \
    { \
        TASK_TYPE_SIGNLE, \
        api_TaskSignleInit, \
        api_TaskSignleMain, \
        api_TaskSignleExit, \
    }, \
    { \
        TASK_TYPE_EXSINGLE, \
        api_TaskExSignleInit, \
        api_TaskExSignleMain, \
        api_TaskExSignleExit, \
    }, \
    { \
        TASK_TYPE_EX175, \
        api_TaskEx175Init, \
        api_TaskEx175Main, \
        api_TaskEx175Exit, \
    }, \
    { \
        TASK_TYPE_PIRATE, \
        api_TaskPirateInit, \
        api_TaskPirateMain, \
        api_TaskPirateExit, \
    }, \
    { \
        TASK_TYPE_EXPIRATE, \
        api_TaskExPirateInit, \
        api_TaskExPirateMain, \
        api_TaskExPirateExit, \
    }, \
};


struct cluamantaskctxt
{
    HANDLE lock;
    unsigned int ref;
    unsigned char stop;
    THREAD_S* thread;
    struct cluamanconfiglist* config;
    void* context;//指向clumantask.ctxt
};

struct clumantask
{
    struct list_head node;
    //struct cluamanconfig config;
    struct cluamanconfiglist* config;
    char user_name[USRNAME_SIZE];
    int bindWnd;
    long hwnd;
    unsigned long taskid;
    unsigned long pid;
    struct cluamantaskctxt ctxt;
    Idmsoft* dmobj;
    int dmobj_index;
};

struct clumantasklist
{
    HANDLE lock;
    struct list_head head;
    unsigned int count;
};

class clumantaskmgr
{
public:
    clumantaskmgr();
    ~clumantaskmgr();

public:
    void setCurConfig(struct cluamanconfiglist* list);
    struct cluamanconfiglist* dupCurConfig();
    void clearCurConfig(struct cluamanconfiglist* list);
    
public:
    int startTask(Idmsoft* dmobj);
    int killTask(Idmsoft* dmobj);
    int killTaskAll(Idmsoft* dmobj);

public:
    struct clumantasklist m_lstTask;
    struct cluamanconfiglist m_configlist;
    unsigned char dmobj_flag[DMOBJ_MAX_NUM];
};

extern clumantaskmgr g_luamantaskmgr;

struct luamanwalkstate
{
    int yNegKeyDown;
    int yPosKeyDown;
    int xPosKeyDown;
    int xNegKeyDown;
    char* xNeg;
    char* xPos;
    char* yNeg;
    char* yPos;
};

struct luaman_posxy
{
    long x;
    long y;
};

struct fighttimeinfo
{
    int wait_time;
    int fight_time;
};

#define pirate_fight_time(_w_, _f_) {(_w_), (_f_)}
#define pirate_dim(_a_) (sizeof((_a_))/sizeof((_a_)[0]))

int SplitString(CString& str, char split, CStringArray& strArray);
int SplitStringWide(CString& str, char* split, CStringArray& strArray);
int CheckKeyState(Idmsoft* dmobj, void* ctxt);
int check_connection_status(Idmsoft* dmobj, void* ctxt,
    int (*callback)(Idmsoft* dmobj, void* ctxt),
    void (*lockonoff)(int flag));

int GetCurPosXY(Idmsoft* dmobj, void* ctxt, long* lposX, long* lposY, CString* strPos);

///////////////////////////////////////////////////////////////////////////////////////////////////////

struct common_loop_ctxt_t
{
    int (*pre_work)(struct cluamanaccountinfo* accountinfo, int roleno, struct common_loop_ctxt_t* clctxt);
    int (*do_work)(struct cluamanaccountinfo* accountinfo, int roleno, struct common_loop_ctxt_t* clctxt);
    int (*post_work)(struct cluamanaccountinfo* accountinfo, int roleno, struct common_loop_ctxt_t* clctxt);
    struct list_head* accountList;
    Idmsoft* dmobj;
    void* ctxt;
    char* fontdir;
    char* bmpdir;
    char* configXML;

    struct login_font_info_t* lfi_pri;
    int lfi_pri_size;
};

int common_mainloop(struct common_loop_ctxt_t* clctxt);
int common_prework(struct cluamanaccountinfo* accountinfo, int roleno, struct common_loop_ctxt_t* clctxt);
int common_postwork(struct cluamanaccountinfo* accountinfo, int roleno, struct common_loop_ctxt_t* clctxt);
int common_checkcopyfull(Idmsoft* dmobj, void* ctxt);
int common_exitgroup(Idmsoft* dmobj, void* ctxt);
int common_check_copy_access(Idmsoft* dmobj, void* ctxt);
int common_check_people_noenough(Idmsoft* dmobj, void* ctxt);



#endif
