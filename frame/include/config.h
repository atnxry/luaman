#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <string>
//#include <vector>
//#include <algorithm>
#include "xcommon.h"
#include "xlist.h"
#include "xlog.h"
#include "dm.h"

//source insight编码为GB2312

//#define LUAMAN_TITLE()  "诺娃曼科技v6.0.2023.03.20.1"
#define LUAMAN_TITLE()  "诺娃曼科技v6.0."__DATE__" "__TIME__


typedef enum
{
    TASK_TYPE_UNKNOWN = -1,
    TASK_TYPE_164     = 0,
    TASK_TYPE_175     = 1,
    TASK_TYPE_1852    = 2,
    TASK_TYPE_1853    = 3,
    TASK_TYPE_195     = 4,
    TASK_TYPE_021     = 5,
    TASK_TYPE_000     = 6,
    TASK_TYPE_SIGNIN  = 7,
    TASK_TYPE_SAKURA  = 8,
    TASK_TYPE_DOTASK  = 9,
    TASK_TYPE_FIGHT   = 10,
    //TODO:
    TASK_TYPE_SIGNLE  = 11,
    TASK_TYPE_PIRATE  = 12,
    TASK_TYPE_FIRE    = 13,
    TASK_TYPE_SNAPSHOT= 14,
    TASK_TYPE_SIMPLE_FIGHT = 15,
    TASK_TYPE_REFUGEBOAT = 16,
    TASK_TYPE_EXSINGLE   = 17,
    TASK_TYPE_EXPIRATE = 18,
    TASK_TYPE_EX175    = 19,
    TASK_TYPE_TEAMGROUP = 20,
    
    TASK_TYPE_MAX        ,
}TASK_TYPE_E;

#define TASK_TYPE_NUM   (TASK_TYPE_MAX)

//混合任务
#define MIX_TASKS() {TASK_TYPE_000}
#define MIX_PARAMS_DIALOG_DECLARE() \
    CParam1853Dlg __dlg1853;\
    CParam195Dlg __dlg195;\
    CParam021Dlg __dlg021;\
    CParam000Dlg __dlg000;\
    CParamSigninDialog __dlgsignin;\
    CParamSakuraDlg __dlgsakura;\
    CParamDotaskDlg __dlgdotask; \
    CParamFightDlg __dlgfight;\
    CParamSignleDlg __dlgsignle; \
    CParamRefugeboatDlg __dlgrefugeboat;\
    CParamExsignleDlg __dlgexsignle; \
    CParamEx175Dlg __dlgex175;\
    CParamTeamGroupDlg __dlgTeamgroup;


#define MIX_PARAMS_ARRAY() {\
    NULL,NULL, NULL, \
    (CParameters*)&__dlg1853, \
    (CParameters*)&__dlg195, \
    (CParameters*)&__dlg021, \
    (CParameters*)&__dlg000, \
    (CParameters*)&__dlgsignin,\
    (CParameters*)&__dlgsakura,\
    (CParameters*)&__dlgdotask,\
    (CParameters*)&__dlgfight,\
    (CParameters*)&__dlgsignle,\
    NULL, NULL, NULL, NULL,\
    (CParameters*)& __dlgrefugeboat,\
    (CParameters*)& __dlgexsignle,\
    NULL,\
    (CParameters*)&__dlgex175,\
    (CParameters*)&__dlgTeamgroup,\
}

#define MIX_PARAMS_DIALOG_ARRAY() {\
    NULL,NULL, NULL, \
    (CDialog*)&__dlg1853, \
    (CDialog*)&__dlg195, \
    (CDialog*)&__dlg021, \
    (CDialog*)&__dlg000, \
    (CDialog*)&__dlgsignin,\
    (CDialog*)&__dlgsakura,\
    (CDialog*)&__dlgdotask,\
    (CDialog*)&__dlgfight,\
    (CDialog*)&__dlgsignle,\
    NULL, NULL, NULL, NULL,\
    (CDialog*)&__dlgrefugeboat,\
    (CDialog*)&__dlgexsignle,\
    NULL,\
    (CDialog*)&__dlgex175,\
    (CDialog*)&__dlgTeamgroup,\
}

#define EXSINGLE_COPYROLENR_MAX (16)

typedef enum
{
    ROLE_U  = 0,
    ROLE_M  = 1,
    ROLE_S0 = 2,
    ROLE_S1 = 3,
    ROLE_AUX= 4,
}ROLE_E;

typedef enum
{
    DIR_U       = 0,
    DIR_LEFT    = 1,
    DIR_RIGHT   = 2,
}DIR_E;

#define SELECTED_FONT()             {"字库1", "字库2", "字库3", "字库4", "字库5", "字库6", "字库7", "字库8", "字库9", "字库10"}
#define TASK_TYPE_DESC_ARRAY()      {"164", "175", "1852", "1853", "195", "021", "000", "signin", "sakura", "dotask", "fight", "signle", "pirate", "fire", "snapshot", "simple_fight", "refuge_boat", "ExSingle", "ExPirate", "Ex175", "TeamGroup"}
#define ROLE_DESC_ARRAY()           {"-----", "主号", "副号", "丛号", "借组号"}
#define DIR_DESC_ARRAY()            {"向上视角", "向左视角", "向右视角"}
#define GROUP_DESC_ARRAY()          {"组1", "组2", "组3", "组4", "组5", "组6", "组7", "组8", "组9", "组10", "组11", "组12"}
#define BLUE_MAX_DESC_ARRAY()       {"4"}
#define BLUE_MIN_DESC_ARRAY()       {"4"}
#define BLUE_THRESHOLD_DESC_ARRAY() {"1000", "2000", "3000", "4000"}
#define ALARM_HOST_DESC_ARRAY()     {"主机1", "主机2", "主机3", "主机4", "主机5", "主机6", "主机7", "主机8", "主机9", "主机10", "主机11", "主机12"}
#define PALY_WND_INDEX_TIME_ARRAY() {"0", "1", "2", "3", "4", "5", "6"}
#define TRANS_WAIT_TIME_ARRAY()     {"0", "5", "10", "15", "20", "25", "30", "35", "40", "45", "50", "55", "60", "65", "70", "75", "80", "85", "90", "95", "100", "105", "110", "115", "120"}

#define BUDDY_NAME_ARRAY()          {"玩玩而已1", "玩玩而已2", "玩玩而已3", "玩玩而已4", "玩玩而已5"}
#define SWJD_INDEX_ARRAY()          {"1", "2", "3", "4", "5", "6"}

#define ATTR_VALUE                   "value"

//字符串属性
#define ITEM_NEW(_xml_, _k_, _v_) do{\
    _xml_.AddElem(CString(_k_));\
    _xml_.AddAttrib(CString(ATTR_VALUE), CString(_v_));\
}while(0)

//数值属性
#define ITEM_NEW_D(_xml_, _k_, _v_) do{\
    _xml_.AddElem(CString(_k_));\
    _xml_.AddAttrib(CString(ATTR_VALUE), _v_);\
}while(0)

#define ITEM_UPDATE(_xml_, _v_) do{\
    _xml_.SetAttrib(CString(ATTR_VALUE), CString(_v_));\
}while(0)

#define ITEM_UPDATE_D(_xml_, _v_) do{\
    _xml_.SetAttrib(CString(ATTR_VALUE), _v_);\
}while(0)

#define READ_ATTR(_attr_, _cfg_, _kyn_, _fld_) do{\
    if (CString(_kyn_) == tag)\
    {\
        char* _cattr_ = _attr_.GetBuffer(BUFFER_MAX);\
        _cfg_->_fld_ = atoi(_cattr_);\
        attr.ReleaseBuffer();\
        //api_log_MsgDebug(##_fld_":%d", _cfg_->_fld_);\
        continue;\
    }\
}while(0)


struct cluamanoperations
{
    TASK_TYPE_E type;
    int (*init)(Idmsoft* dmobj, void* ctxt);//ctxt:clumantask
    int (*main)(Idmsoft* dmobj, void* ctxt);
    int (*exit)(Idmsoft* dmobj, void* ctxt);
};

#define LUAMAN_ALARM_SERVER "127.0.0.1"

#define USRNAME_SIZE   (32)
#define PASSWORD_SIZE   (32)

struct cluamanaccountinfo
{
    struct list_head node;
    char user_name[USRNAME_SIZE];
    char passwd[PASSWORD_SIZE];
    char passwdex[PASSWORD_SIZE];
};

struct cluamanbindinfo
{
    struct list_head node;
    int bindWndnr;
    struct list_head accountList;
    int accountNum;
};

struct cluamanconfig
{
    struct list_head node;
    unsigned int loop_num;
    TASK_TYPE_E taskType;
    int selected_front_index;
    int direction;
    ROLE_E role;
    int groupid;
    int maxBlueFiled;
    int minBlue;
    int blueThreshold;
    char alarmServer[32];
    int alarmPort;
    int alarmHostIndex;

    /* 021副本配置 */
    unsigned int playWndIndexVal;
    int Wait2Sleep;

    int exitGameCopyFull;//连续出现副本地下城已满退出游戏标志 1853也有
    int cleanPackageWhenStart;//1853也有

    /* 1853副本配置*/
    int bluebloodWidth;//maxBlueFiled
    int bluebloodLowLevel;//blueThreshold
    //int forwardDirection;//direction 未用
    int syncCountMax;//TODO:新增,默认为2
    int bluebloodChecktime;//TODO:新增
    int callbabydie;

    int blockTimes;//TODO:新增 组队失败后等待时间/分钟
    int waitCopytime;//TODO:新增 副本结束后等待时间/秒
    int cleanPackageTime;//TODO:新增 清包周期3小时
    int cleanPackageX;//TODO:新增

    ///////////////////////////////////////////////////////////////////
    //新增组合副本的配置
    char copyName[32];
    struct list_head mixConfigList;//组合副本各任务的配置 struct cluamanconfig
    int mixConfigNum;
    struct list_head bindinfoList;//绑定窗口信息
    int bindinfoNum;
    int mixLoopNum;//组合副本任务次数

    //该参数有param000dlg中设置
    int curBindWnd;//内存配置,设置下拉框中的索引,用于组合副本启动时获取对应绑定组的用户信息,不存到xml
    int waitEmail;//没有邮件时是否一直等待,不存在xml文件里
    //绑定电脑
    int curBindHostIdx;
    ///////////////////////////////////////////////////////////////////

    //做任务的伙伴ID
    char buddyName[128];
    int swjd_index;//死亡绝地几个坐标的索引
    
    int copyrolenr;//单人副本中账号角色的个数
    struct cluamanoperations ops;

    //中间传递的临时上下文数据,不需要存储到xml
    char* user_name;
    int roleno;
    int (*enter_with_role)(Idmsoft* dmobj, void* ctxt);
};

#define NO_CLEAN_PACKAGE_X(_x_, _p_) ((_x_) &= (~(1 << (_p_))))
#define NO_CLEAN_PACKAGE_1(_x_) NO_CLEAN_PACKAGE_X((_x_), 1)
#define NO_CLEAN_PACKAGE_2(_x_) NO_CLEAN_PACKAGE_X((_x_), 2)
#define NO_CLEAN_PACKAGE_3(_x_) NO_CLEAN_PACKAGE_X((_x_), 3)
#define NO_CLEAN_PACKAGE_4(_x_) NO_CLEAN_PACKAGE_X((_x_), 4)
#define NO_CLEAN_PACKAGE_5(_x_) NO_CLEAN_PACKAGE_X((_x_), 5)

#define CLEAN_PACKAGE_X(_x_, _p_) ((_x_) |= (1 << (_p_)))
#define CLEAN_PACKAGE_1(_x_) CLEAN_PACKAGE_X((_x_), 1)
#define CLEAN_PACKAGE_2(_x_) CLEAN_PACKAGE_X((_x_), 2)
#define CLEAN_PACKAGE_3(_x_) CLEAN_PACKAGE_X((_x_), 3)
#define CLEAN_PACKAGE_4(_x_) CLEAN_PACKAGE_X((_x_), 4)
#define CLEAN_PACKAGE_5(_x_) CLEAN_PACKAGE_X((_x_), 5)
#define CLEAN_PACKAGE_ALL(_x_) (CLEAN_PACKAGE_1(_x_) | CLEAN_PACKAGE_2(_x_) | CLEAN_PACKAGE_3(_x_) | CLEAN_PACKAGE_4(_x_) | CLEAN_PACKAGE_5(_x_))

#define CHECK_CLEAN_PACKAGE_X(_x_, _p_) ((_x_) & (1 << (_p_)))
#define CHECK_CLEAN_PACKAGE_1(_x_) CHECK_CLEAN_PACKAGE_X((_x_), 1)
#define CHECK_CLEAN_PACKAGE_2(_x_) CHECK_CLEAN_PACKAGE_X((_x_), 2)
#define CHECK_CLEAN_PACKAGE_3(_x_) CHECK_CLEAN_PACKAGE_X((_x_), 3)
#define CHECK_CLEAN_PACKAGE_4(_x_) CHECK_CLEAN_PACKAGE_X((_x_), 4)
#define CHECK_CLEAN_PACKAGE_5(_x_) CHECK_CLEAN_PACKAGE_X((_x_), 5)

struct cluamanconfiglist
{
    HANDLE lock;
    struct list_head head;
    unsigned int count;
};

struct xyposition
{
    long x;
    long y;
};

#define CONFIG_XML  "./conf/config.xml"

void api_UpdateConfig(struct cluamanconfig* oconfig, struct cluamanconfig* nconfig, int method);
void api_PrintConfig(struct cluamanconfig* config);

int api_LoadXML(std::string& file, char* tasktype, struct cluamanconfig* config);
int api_SaveXML(std::string& file, struct cluamanconfig* config);


int api_CloneConfig(struct cluamanconfig* oconfig, struct cluamanconfig* nconfig);
int api_CloneConfigAppendBindinfo(struct cluamanconfig* oconfig, struct cluamanconfig* nconfig);
void api_ClearMixConfig(struct cluamanconfig* config);
int api_UpdateBindinfo(struct cluamanconfig* config, struct cluamanbindinfo* bindinfo);

//////////////////////////////////////////////////////////////////////////////////////////

struct exCommonConfig
{
    int bindwnd;
    int last_nr;
    int last_roleno;
    char user_name[USRNAME_SIZE];
    char passwd[PASSWORD_SIZE];
    char passwdex[PASSWORD_SIZE];
};

#define KEY_exCommon_Root         "Root"
#define KEY_exCommon_Account      "Account"
#define KEY_exCommon_Bindwnd      "Bindwnd"
#define KEY_exCommon_Lastnr       "Lastnr"
#define KEY_exCommon_Lastroleno   "Lastroleno"
#define KEY_exCommon_Username     "Username"
#define KEY_exCommon_Password     "Password"
#define KEY_exCommon_Passwordex   "Passwordex"

int init_exCommonLock(void);//全局锁
bool load_exCommonConfig(int bindwnd, std::string& file, struct exCommonConfig* config);
void print_exCommonConfig(struct exCommonConfig* config);
int save_exCommonConfig(std::string& file, struct exCommonConfig* config);

struct login_font_info_t
{
    char* login;
    char* word;
    char* font;
    char* color_offset[2];
};


#endif
