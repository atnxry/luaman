#include <math.h>
#include <stdlib.h>
#include "stdafx.h"
#include "xrrno.h"
#include "xlog.h"
#include "xcommon.h"
#include "xlist.h"
#include "task.h"
#include "config.h"
#include "alarm.h"
#include "task.h"
#include "taskExSignle.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "Markup.h"
#include "curl.h"
#include "taskExSignle_Pub.h"
#include "taskSignlePri.h"

//点击右键后的时延值(单位:毫秒)
#define RIGHT_CLICK_DELAY_TIME  (500)

//仓库格子移动间隔时间(单位:毫秒)
#define KEY_MOVE_DELAY_TIME  (30)

//穿上衣服的时延(单位:毫秒)
#define COAT_ON_DELAY_TIME  (30)

//账号-角色-字库等信息
struct login_font_info_t lfi_pri[] =
{
    {"tzdk1601lmq", "A",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "我给无",        FONT_EXSIGNLE_DIR"font-1-ZhangHao4-B.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "C",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "规范五",        FONT_EXSIGNLE_DIR"font-1-ZhangHao4-D.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "E",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "F",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "G",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "H",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "I",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "J",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "K",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "发条无",        FONT_EXSIGNLE_DIR"font-1-ZhangHao4-L.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "M",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "N",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},



    {"tzdk1602lmq", "A",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "以撒地",        FONT_EXSIGNLE_DIR"font-1-ZhangHao2-B.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "C",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "股份一",        FONT_EXSIGNLE_DIR"font-1-ZhangHao2-D.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "E",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "F",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "G",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "H",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "I",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "J",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "K",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "沟通一",        FONT_EXSIGNLE_DIR"font-1-ZhangHao2-L.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "M",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "N",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},


    {"tzdk1603lmq", "A",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "沙发而",        FONT_EXSIGNLE_DIR"font-1-ZhangHao3-B.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "C",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "沟通儿",        FONT_EXSIGNLE_DIR"font-1-ZhangHao3-D.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "E",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "F",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "G",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "H",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "I",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "J",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "K",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "光和热",        FONT_EXSIGNLE_DIR"font-1-ZhangHao3-L.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "M",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "N",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},


    {"tzdk1604lmq", "A",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "多少散",        FONT_EXSIGNLE_DIR"font-1-ZhangHao1-B.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "C",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "关于三",        FONT_EXSIGNLE_DIR"font-1-ZhangHao1-D.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "E",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "F",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "G",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "H",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "I",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "J",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "K",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "十分三",        FONT_EXSIGNLE_DIR"font-1-ZhangHao1-L.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "M",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "N",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    


    {"tzdk1605lmq", "A",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "是的对方思",        FONT_EXSIGNLE_DIR"font-1-ZhangHao5-B.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "C",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "哥特人思",        FONT_EXSIGNLE_DIR"font-1-ZhangHao5-D.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "E",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "F",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "G",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "H",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "I",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "J",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "K",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "加好友思",        FONT_EXSIGNLE_DIR"font-1-ZhangHao5-L.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "M",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "N",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},


};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//以下是配置的容易,可以修改
/*
2 进入游戏之后，先按I，点击服饰。在背包的相对位置1（pkgpos_relative第一个），鼠标放上去，
  看看能找到紧身泳衣（颜色？）（font-zhuangbei1.txt）不，找到了，关掉背包。按I，找回城卷轴，然后按正常顺序走。
*/
//紧身泳衣颜色
#define SWIM_COAT_COLOR "00ff00-101010"

//相对位置
struct posxy_t pkgpos_relative[] =
{
    /*
    2 进入游戏之后，先按I，点击服饰。在背包的相对位置1（pkgpos_relative第一个），鼠标放上去，
      看看能找到紧身泳衣（颜色？）（font-zhuangbei1.txt）不，找到了，关掉背包。按I，找回城卷轴，然后按正常顺序走。
    */
    {-68, 166}, //背包相对位置
};

//找不到，就判断坐标。如果坐标是（pos_info）误伤范围大点。就直接按N对话。如果不是，就按I，找回城卷，然后回城，寻路
struct posxy_t pos_info[] =
{
    {304, 40},
};

/*
6 打完副本了，回到城里了，然后去卖垃圾349:129|353:119|355:114|357:107|356:97|351:91|345:86|338:81|332:78|325:75|319:71|315:66|312:61|308:54|301:52|292:49|293:46（路径要改，逻辑不变）。

卖完垃圾了，寻路292:49|301:52|308:54|305:48|304:43|304:40，然后先把装备拿下来。按I，在背包的相对位置（16个），
找到字库1和字库2，就点击右键。然后再点灰色的 服饰。再找背包的相对位置（1个）。到紧身泳衣（font-zhuangbei1.txt），点击右键。
*/
//16个相对于背包的相对偏移值
struct posxy_t coatoff_pos_info[] =
{
    //TODO:
    {-125, 155},
    {16,    93},
    {15,   185},
    {13,   279},
    {-128, 290},
    {157,  288},
    {-74,   71},
    {-56,  118},
    {87,   121},
    {-57,  221},
    {87,   220},
    {-55,  261},
    {86,   267},
    {-74,  361},
    {14,   360},
    {101,  361},

};

//再找背包的相对位置（1个）。到紧身泳衣（font-zhuangbei1.txt），点击右键。
//泳衣相对背包的位置
struct posxy_t swimcoatoff_pos_info[] =
{
    //TODO:
    {-68, 166},

};

/*
7 整理背包。然后按N对话，点击绿色的（00ff00）仓库存放。font-cunfang1.txt。然后点灰色的（929292）公用。然后背包里找一遍。
满足字库1的，就点击右键。满足字库2的（有四个）。点击左键，然后再把左键移动到对应的位置，再点左键。就把这四个也放仓库里了。
*/
//满足字库2的（有四个）相对于公用的位置
struct posxy_t mov2posIdx_pos_info[] =
{
    //TODO:
    {-117, 231},
    {-58,  228},
    {7,    230},
    {70,   231},

};

/*
2 进入游戏之后，先按I，点击服饰(font-FuShi.txt)(929292)。在背包的相对位置1（pkgpos_relative第一个），鼠标放上去，
  看看能找到紧身泳衣（颜色？）（font-zhuangbei1.txt）不，找到了，关掉背包。按I，找回城卷轴，然后按正常顺序走。

  找不到，就判断坐标。如果坐标是（pos_info）误伤范围大点。就直接按N对话。如果不是，就按I，找回城卷，然后回城，寻路。（）
*/
#define WALK_PATH_FRIST()   "349:129|353:119|355:114|357:107|356:97|351:91|345:86|338:81|332:78|325:75|319:71|315:66|312:61|308:54|305:48|304:43|304:40"

// 以下11个是字库1
struct font_info_t library_font_array_1[] =
{
    {"影之悲鸣",         FONT_EXSIGNLE_DIR"font-ziku1-Tou.txt",       "a335ee-101010"},
    {"紧身泳衣",         FONT_EXSIGNLE_DIR"font-ziku1-ShiZhuang.txt", "ffffff-101010"},
    {"长寿玉手环",        FONT_EXSIGNLE_DIR"font-ziku1-ChangShou.txt", "00ff00-101010"},
    {"石榴石耳环",        FONT_EXSIGNLE_DIR"font-ziku1-ShiLiu.txt",    "00ff00-101010"},
    {"手套",           FONT_EXSIGNLE_DIR"font-ziku1-ShouTao.txt",   "ffe08c-101010"},
    {"黑暗腰带",         FONT_EXSIGNLE_DIR"font-ziku1-YaoDai.txt",    "00ff00-101010"},
    {"翅膀",         FONT_EXSIGNLE_DIR"font-ziku1-YanWang.txt",   "ffe08c-101010"},
    {"印第安项链",        FONT_EXSIGNLE_DIR"font-ziku1-YinDiAn.txt",   "ffffff-101010"},
    {"海伯伦的耳环", FONT_EXSIGNLE_DIR"font-ziku1-HaiErHuan.txt", "a335ee-101010"},
    {"堕落深渊",         FONT_EXSIGNLE_DIR"font-ziku1-HaiJieZhi.txt", "a335ee-101010"},
    {"结婚戒指",         FONT_EXSIGNLE_DIR"font-ziku1-HunJie.txt",    "ffffff-101010"},

    //TODO:后面可以添加,依葫芦画瓢
    
};

//以下四个是字库2
struct font_info_t library_font_array_2[] =
{
    {"赫里恩双叶刀", FONT_EXSIGNLE_DIR"font-ziku2-ShuangYeDao.txt", "a335ee-101010"},
    {"赫里恩护腿",          FONT_EXSIGNLE_DIR"font-ziku2-HeLiEnTui.txt",   "a335ee-101010"},
    {"赫里恩战靴",          FONT_EXSIGNLE_DIR"font-ziku2-HeLiEnXue.txt",   "a335ee-101010"},
    {"赫里恩盔甲",          FONT_EXSIGNLE_DIR"font-ziku2-HeLiEnJia.txt",   "a335ee-101010"},

    //TODO:后面可以添加,依葫芦画瓢
    
};

struct font_info_t library_font_array_3[] =
{
    {"狂气碎片", FONT_EXSIGNLE_DIR"font-ziku3-kuangqi.txt", "ffffff-101010"},
    {"悲愤碎片",     FONT_EXSIGNLE_DIR"font-ziku3-beifen.txt",  "ffffff-101010"},

    //TODO:后面可以添加,依葫芦画瓢
    
};


//字库名字为:"font-exclrpkg2.txt"
static gabage_info usrdef_gabage_info[] = 
{
    //{"垃圾1", "00ff00-010101"}, //一行表示一种垃圾
    //{"垃圾2", "00ff00-010101"},
    //TODO:参考垃圾1、垃圾2写,'//'表示注释
    {"尊严腰带", "ffffff-010101"},
    {"尊严腰带", "c1c1ff-010101"},
    {"尊严腰带", "ffe08c-010101"},
    
    
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!! 以下内容不要修改 !!!!!!!!!!!!!!!!!!!
#define FONTSIGNLE_DIR "font\\exsignle\\"
#define BMPSIGNLE_DIR "bmp\\exsignle\\"

static int enter_with_role(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX0 = 0, lposY0 = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    char* user_name = luaman_config->user_name;
    int roleno = luaman_config->roleno;
    char* role_name = NULL;
    int i = 0, j = 0;
    int clicknr = 0;
    struct posxy_t posxys[] = {
        {1219, 608},
        {1219, 608},
        {1219, 608},
        {1105, 607},
        {1105, 607},
        {1105, 607},
    };
        
    /////////////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        dmobj->MoveTo(5, 5);
        Delay(500);

        for (i = 0; i < LUAMAN_DIM(lfi_pri); i++)
        {
            if (strcmp(lfi_pri[i].login, user_name))
            {
                api_log_MsgDebug("账号:%s:匹配:失败", user_name);
                continue;
            }

            role_name = lfi_pri[i + roleno].word;
            api_log_MsgDebug("账号:%s:角色:%s:匹配:成功", user_name, role_name);

            for (j = 0; j < LUAMAN_DIM(lfi_pri[i + roleno].color_offset); j++)
            {
                dmobj->SetDict(0, lfi_pri[i + roleno].font);
                code = dmobj->FindStrFast(0, 27, 1279, 826, role_name, lfi_pri[i + roleno].color_offset[j], 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("账号:%s:角色:%d:名称:%s:颜色:%s:查找:成功, lposX:%ld, lposY:%ld",
                        user_name, roleno, role_name, lfi_pri[i + roleno].color_offset[j],lposX, lposY);
                    goto label_click_role;
                }

                api_log_MsgDebug("账号:%s:角色:%d:名称:%s:字库:%s:颜色:%s:查找:失败",
                    user_name, roleno, role_name, lfi_pri[i + roleno].font,
                    lfi_pri[i + roleno].color_offset[j]);
            }

            api_log_MsgDebug("账号:%s:角色:%s:查找:失败", user_name, role_name);
            break;
        }

        //点击三角形箭头翻页再继续查找
        dmobj->MoveTo(posxys[clicknr].x, posxys[clicknr].y);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(500);
    
        clicknr = (clicknr + 1) % LUAMAN_DIM(posxys);
        
        api_log_MsgDebug("账号:%s:角色:%d:名称:%s:查找:失败:翻页", user_name, roleno, role_name);
        Delay(500);
    }

label_click_role:
    //dmobj->Capture(0, 27, 1279, 826, "cap\\sel_role0.bmp");
    
    //点击选择角色
    dmobj->MoveTo(lposX, lposY);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);

    //dmobj->Capture(0, 27, 1279, 826, "cap\\sel_role1.bmp");
    
    /////////////////////////////////////////////////////////////////////////////////
    //点击进入游戏
    dmobj->MoveTo(640, 670);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(200);

    return SNAIL_ERRNO_SUCCESS;
}

static int Buy_Blue(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = luaman_config->cleanPackageX;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX1 = 0, lposY1 = 0;
    long lposX2 = 0, lposY2 = 0;
    int posX1 = 0;
    int posY1 = 0;
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    long deltaX = 35;
    long  deltaY = 35;
    long  lineX = -1;
    long  lineY = -1;
    int index = 0;
    int jndex = 0;
    int lndex = 0;
    long posX = 0;
    long posY = 0;
    struct xyposition boxPosXYs[5 * 10];
    
    api_log_MsgDebug("买蓝:开始");

    dmobj->MoveTo(5, 5);
    Delay(500);
        
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //点击之后出现"商店"字样
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-shop.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "商店", "ffffff-101010", 0.9, &pposX1, &pposY1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX1 = pposX1.lVal;
            lposY1 = pposY1.lVal;

            api_log_MsgDebug("商店:商店:字样:找字:成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
            break;
        }
        
        api_log_MsgDebug("商店:商店:字样:找字:失败");
        Delay(500);
    }

    //498:204 -> 658:278
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX1 + 160, lposY1 + 74);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("商店:确认:字样:找字:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("商店:确认:字样:找字:失败");
        Delay(500);
    }

    //输入金额后点确认
    code = dmobj->SendString(hwnd, "300");
    if (1 != code)
    {
        api_log_MsgError("商店:金额:300:输入:失败");
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("密码:金额:300:输入:成功");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("商店:确认:字样:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("商店:确认:字样:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    api_log_MsgDebug("买蓝:完毕");
    return SNAIL_ERRNO_SUCCESS;
}

static int sell_gabage(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = 0x6;//luaman_config->cleanPackageX;//默认清理1,2包
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX1 = 0, lposY1 = 0;
    long lposX2 = 0, lposY2 = 0;
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    long deltaX = 35;
    long  deltaY = 35;
    long  lineX = -1;
    long  lineY = -1;
    int index = 0;
    int jndex = 0;
    int lndex = 0;
    long posX = 0;
    long posY = 0;
    struct xyposition boxPosXYs[5 * 10];
    int idx = 0;
    int findout = 0;
    
    api_log_MsgDebug("清包:开始");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("背包:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //bug:调整界面相对位置 2021/6/18 10:11:19
            //841:93->667:505 1037:687
            boxPosX0 = lposX2 - 174;
            boxPosY0 = lposY2 + 412;
            boxPosX1 = lposX2 + 196;
            boxPosY1 = lposY2 + 594;

            labelPosXY[0].x = lposX2 - 70;
            labelPosXY[0].y = lposY2 + 400;
            labelPosXY[1].x = lposX2 - 10;
            labelPosXY[1].y = lposY2 + 400;
            labelPosXY[2].x = lposX2 + 50;
            labelPosXY[2].y = lposY2 + 400;
            labelPosXY[3].x = lposX2 + 110;
            labelPosXY[3].y = lposY2 + 400;
            labelPosXY[4].x = lposX2 + 170;
            labelPosXY[4].y = lposY2 + 400;
            
            break;
        }

        api_log_MsgDebug("背包:查找:失败");
        Delay(500);
    }

    //点击整理
    do_click_tidy_package(luaman_task, dmobj, lposX2, lposY2);
    
    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;

    api_log_MsgDebug("背包:宝物柜, lineX:%ld, lineY:%ld", lineX, lineY);

    for (index = 0; index < 5; index++)
    {
        posY = lineY + index * deltaY;
        
        for (jndex = 0; jndex < 10; jndex++)
        {
            posX = lineX + jndex * deltaX;
            
            boxPosXYs[index * 10 + jndex].x = posX;
            boxPosXYs[index * 10 + jndex].y = posY;

            //api_log_MsgDebug("boxPosXYs[%d][%d] = [%ld, %ld]", index, jndex, posX, posY);
        }
    }

    for (lndex = 0; lndex < 5; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }

        //点开标签页
        dmobj->MoveTo(labelPosXY[lndex].x, labelPosXY[lndex].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        for (index = 0; index < 5; index++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            for (jndex = 0; jndex < 10; jndex++)
            {
                posX = boxPosXYs[index * 10 + jndex].x;
                posY = boxPosXYs[index * 10 + jndex].y;

                dmobj->MoveTo(posX, posY);
                Delay(250);

                findout = 0;
                
                for (idx = 0; idx < sizeof(usrdef_gabage_info)/sizeof(usrdef_gabage_info[0]); idx++)
                {
                    dmobj->SetDict(0, FONTSIGNLE_DIR"font-exclrpkg2.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, usrdef_gabage_info[idx].name, usrdef_gabage_info[idx].color_offset, 0.9, &pposX1, &pposY1);
                    dmobj->ClearDict(0);
                    if (0 != code)
                    {
                        continue;
                    }

                    Delay(200);
                    dmobj->RightClick();
                    Delay(300);
                    dmobj->KeyDownChar("enter");
                    Delay(300);
                    dmobj->KeyUpChar("enter");
                    Delay(300);

                    api_log_MsgDebug("清包:背包:%d:柜子:[%d:%d]:垃圾:%d:名称:%s:颜色:%s",
                        lndex, index, jndex, idx,
                        usrdef_gabage_info[idx].name, usrdef_gabage_info[idx].color_offset);

                    findout = 1;
                    break;
                }

                if (findout)
                {
                    continue;
                }

                dmobj->SetDict(0, FONTSIGNLE_DIR"font-ziku3-yan.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "炎", "ffffff-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (0 != code)
                {
                    continue;
                }

                api_log_MsgDebug("清包:背包:%d:柜子:[%d:%d]:垃圾:%d:炎:成功", lndex, index, jndex, idx);
                
                dmobj->SetDict(0, FONTSIGNLE_DIR"font-beasellman.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "180", "ffe08c-101010", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (0 != code)
                {
                    continue;
                }

                api_log_MsgDebug("清包:背包:%d:柜子:[%d:%d]:垃圾:%d:180:成功", lndex, index, jndex, idx);
                
                Delay(200);
                dmobj->RightClick();
                Delay(300);
                dmobj->KeyDownChar("enter");
                Delay(300);
                dmobj->KeyUpChar("enter");
                Delay(300);
            }
        }
    }

    //买蓝 2023/5/17 22:00:00
    (void)Buy_Blue(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("清包:完毕");
    return SNAIL_ERRNO_SUCCESS;
}

static int clean_package(Idmsoft* dmobj, void* ctxt)
{
    int status = 0;
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    
    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "349:129|353:119|355:114|357:107|356:97|351:91|345:86|338:81|332:78|325:75|319:71|315:66|312:61|308:54|301:52|292:49|283:46";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[293:46]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[293:46]:成功");
    
    //按n
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-QingBao1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "骑士(单手)", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("骑士(单手):查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("骑士(单手):查找:失败");
        Delay(500);
    }
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-QingBao1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "其他", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("其他:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            
            break;
        }

        api_log_MsgDebug("其他:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-QingBao1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "其他", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("其他:点击:成功");
            break;
        }
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("其他:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    return sell_gabage(dmobj, ctxt);
}

static int coatoff(Idmsoft* dmobj, void* ctxt)//脱衣服
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = 0x6;//luaman_config->cleanPackageX;//默认清理1,2包
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX1 = 0, lposY1 = 0;
    long lposX2 = 0, lposY2 = 0;
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    long deltaX = 35;
    long  deltaY = 35;
    long  lineX = -1;
    long  lineY = -1;
    int index = 0;
    int jndex = 0;
    int lndex = 0;
    long posX = 0;
    long posY = 0;
    struct xyposition boxPosXYs[5 * 10];
    int idx = 0;
    char* word = NULL;
    char* font = NULL;
    char* color_offset = NULL;
    int fintout = 0;
    
    api_log_MsgDebug("外套:脱下:开始");

    dmobj->MoveTo(5, 5);
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        dmobj->KeyDownChar("i");
        Delay(200);
        dmobj->KeyUpChar("i");
        Delay(200);
        
        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("背包:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("背包:查找:失败");
        Delay(500);
    }

    //点击整理
    do_click_tidy_package(luaman_task, dmobj, lposX2, lposY2);
    
    for (index = 0; index < LUAMAN_DIM(coatoff_pos_info); index++)
    {
        posX = lposX2 + coatoff_pos_info[index].x;
        posY = lposY2 + coatoff_pos_info[index].y;
        
        dmobj->MoveTo(posX, posY);
        Delay(RIGHT_CLICK_DELAY_TIME);

        dmobj->RightClick();
        Delay(RIGHT_CLICK_DELAY_TIME);
    }

    api_log_MsgDebug("外套:脱下:结束");
    return SNAIL_ERRNO_SUCCESS;
}

static int coaton(Idmsoft* dmobj, void* ctxt)//穿衣服
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = 0x6;//luaman_config->cleanPackageX;//默认清理1,2包
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX1 = 0, lposY1 = 0;
    long lposX2 = 0, lposY2 = 0;
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    long deltaX = 35;
    long  deltaY = 35;
    long  lineX = -1;
    long  lineY = -1;
    int index = 0;
    int jndex = 0;
    int lndex = 0;
    long posX = 0;
    long posY = 0;
    struct xyposition boxPosXYs[5 * 10];
    int idx = 0;
    char* word = NULL;
    char* font = NULL;
    char* color_offset = NULL;
    int fintout = 0;
    
    api_log_MsgDebug("外套:穿上:开始");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyDownChar("i");
        Delay(200);
        dmobj->KeyUpChar("i");
        Delay(200);
        
        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("背包:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //bug:调整界面相对位置 2021/6/18 10:11:19
            //841:93->667:505 1037:687
            boxPosX0 = lposX2 - 174;
            boxPosY0 = lposY2 + 412;
            boxPosX1 = lposX2 + 196;
            boxPosY1 = lposY2 + 594;

            labelPosXY[0].x = lposX2 - 70;
            labelPosXY[0].y = lposY2 + 400;
            labelPosXY[1].x = lposX2 - 10;
            labelPosXY[1].y = lposY2 + 400;
            labelPosXY[2].x = lposX2 + 50;
            labelPosXY[2].y = lposY2 + 400;
            labelPosXY[3].x = lposX2 + 110;
            labelPosXY[3].y = lposY2 + 400;
            labelPosXY[4].x = lposX2 + 170;
            labelPosXY[4].y = lposY2 + 400;
            
            break;
        }

        api_log_MsgDebug("背包:查找:失败");
        Delay(500);
    }

    //点击整理
    do_click_tidy_package(luaman_task, dmobj, lposX2, lposY2);
    
    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;

    api_log_MsgDebug("背包:宝物柜, lineX:%ld, lineY:%ld", lineX, lineY);

    for (index = 0; index < 5; index++)
    {
        posY = lineY + index * deltaY;
        
        for (jndex = 0; jndex < 10; jndex++)
        {
            posX = lineX + jndex * deltaX;
            
            boxPosXYs[index * 10 + jndex].x = posX;
            boxPosXYs[index * 10 + jndex].y = posY;

            //api_log_MsgDebug("boxPosXYs[%d][%d] = [%ld, %ld]", index, jndex, posX, posY);
        }
    }

    for (lndex = 0; lndex < 2; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        /*if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }*/

        //点开标签页
        dmobj->MoveTo(labelPosXY[lndex].x, labelPosXY[lndex].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        for (index = 0; index < 5; index++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            for (jndex = 0; jndex < 10; jndex++)
            {
                posX = boxPosXYs[index * 10 + jndex].x;
                posY = boxPosXYs[index * 10 + jndex].y;

                dmobj->MoveTo(posX, posY);
                Delay(COAT_ON_DELAY_TIME);

                fintout = 0;
                
                for (idx = 0; idx < LUAMAN_DIM(library_font_array_1); idx++)
                {
                    font = library_font_array_1[idx].font;
                    word = library_font_array_1[idx].word;
                    color_offset = library_font_array_1[idx].color_offset;
                
                    dmobj->SetDict(0, font);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("装备:单元格[%d:%d]:名称:%s:字库1:穿上:成功, lposX:%ld, lposY:%ld", index, jndex, word, lposX, lposY);

                        dmobj->MoveTo(posX, posY);
                        Delay(RIGHT_CLICK_DELAY_TIME);

                        dmobj->RightClick();
                        Delay(RIGHT_CLICK_DELAY_TIME);

                        fintout = 1;
                        break;
                    }
                }

                if (fintout)
                {
                    continue;
                }

                for (idx = 0; idx < LUAMAN_DIM(library_font_array_2); idx++)
                {
                    font = library_font_array_2[idx].font;
                    word = library_font_array_2[idx].word;
                    color_offset = library_font_array_2[idx].color_offset;
                
                    dmobj->SetDict(0, font);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("装备:单元格[%d:%d]:名称:%s:字库2:穿上:成功, lposX:%ld, lposY:%ld", index, jndex, word, lposX, lposY);

                        dmobj->MoveTo(posX, posY);
                        Delay(RIGHT_CLICK_DELAY_TIME);

                        dmobj->RightClick();
                        Delay(RIGHT_CLICK_DELAY_TIME);
                        
                        break;
                    }
                }
            }
        }
    }

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("外套:穿上:完毕");
    return SNAIL_ERRNO_SUCCESS;
}

//脱泳衣
static int swimcoatoff(Idmsoft* dmobj, void* ctxt)
{
    int status = 0;
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int i = 0, j = 0, k = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int result = 0;
    long posX0 = 0, posY0 = 0, posX1 = 0, posY1 = 0;
    long deltaX = 0, deltaY = 0;
    char* word = NULL;
    char* font = NULL;
    char* color_offset = NULL;
    int findout = 0;
    int index = 0;

    api_log_MsgDebug("泳衣:脱下:开始");
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //检查是否按成功
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("背包:找字:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("背包:找字:失败");
        Delay(500);
    }

    //点击整理
    do_click_tidy_package(luaman_task, dmobj, lposX2, lposY2);
    
    //点击服饰 880:121 -> 876:151
    dmobj->MoveTo(lposX2 - 4, lposY2 + 30);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    for (index = 0; index < LUAMAN_DIM(swimcoatoff_pos_info); index++)
    {
        posX = lposX2 + swimcoatoff_pos_info[index].x;
        posY = lposY2 + swimcoatoff_pos_info[index].y;
        
        dmobj->MoveTo(posX, posY);
        Delay(RIGHT_CLICK_DELAY_TIME);

        dmobj->RightClick();
        Delay(RIGHT_CLICK_DELAY_TIME);
    }

    api_log_MsgDebug("泳衣:脱下:结束");
    return SNAIL_ERRNO_SUCCESS;
}

static int back2library(Idmsoft* dmobj, void* ctxt)
{
    int status = 0;
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int i = 0, j = 0, k = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int result = 0;
    long posX0 = 0, posY0 = 0, posX1 = 0, posY1 = 0;
    long deltaX = 35;
    long deltaY = 35;
    char* word = NULL;
    char* font = NULL;
    char* color_offset = NULL;
    int findout = 0;
    int index = 0;
    long buildx = 0, buildy = 0;
    int lndex = 0;
    int jndex = 0;
    int fintout = 0;
    int idx = 0;
    struct xyposition boxPosXYs[5 * 10];
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    int mov2posIdx = 0;
    long  lineX = -1;
    long  lineY = -1;
    
    api_log_MsgDebug("归还:仓库:完毕");

    dmobj->MoveTo(5, 5);
    Delay(100);
        
    //按i弹出背包
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyDownChar("i");
        Delay(200);
        dmobj->KeyUpChar("i");
        Delay(200);

        //检查是否按成功
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("背包:找字,成功, lposX:%ld, lposY:%ld", lposX, lposY);

            lposX2 = lposX;
            lposY2 = lposY;
            
            //bug:调整界面相对位置 2021/6/18 10:11:19
            //841:93->667:505 1037:687
            boxPosX0 = lposX2 - 174;
            boxPosY0 = lposY2 + 412;
            boxPosX1 = lposX2 + 196;
            boxPosY1 = lposY2 + 594;

            labelPosXY[0].x = lposX2 - 70;
            labelPosXY[0].y = lposY2 + 400;
            labelPosXY[1].x = lposX2 - 10;
            labelPosXY[1].y = lposY2 + 400;
            labelPosXY[2].x = lposX2 + 50;
            labelPosXY[2].y = lposY2 + 400;
            labelPosXY[3].x = lposX2 + 110;
            labelPosXY[3].y = lposY2 + 400;
            labelPosXY[4].x = lposX2 + 170;
            labelPosXY[4].y = lposY2 + 400;
            break;
        }

        api_log_MsgDebug("背包:找字,失败");
        Delay(500);
    }

    //点击整理
    do_click_tidy_package(luaman_task, dmobj, lposX, lposY);
    
    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;

    api_log_MsgDebug("背包:宝物柜, lineX:%ld, lineY:%ld", lineX, lineY);

    for (index = 0; index < 5; index++)
    {
        posY = lineY + index * deltaY;
        
        for (jndex = 0; jndex < 10; jndex++)
        {
            posX = lineX + jndex * deltaX;
            
            boxPosXYs[index * 10 + jndex].x = posX;
            boxPosXYs[index * 10 + jndex].y = posY;
        }
    }
    
    //916,126->786,522
    {
        buildx = lposX - 130;
        buildy = lposY + 396;
        
        dmobj->MoveTo(buildx, buildy);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        api_log_MsgDebug("整理背包:完毕,按i退出, buildx:%ld, buildy:%ld", buildx, buildy);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-cunfang1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "仓库存放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("仓库存放:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("仓库存放:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //点击仓库存放
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-cunfang3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "公用", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("公用:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("公用:查找:失败");
        Delay(500);
    }

    dmobj->MoveTo(lposX2, lposY2);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (lndex = 0; lndex < 2; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        /*if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }*/

        //点开标签页
        dmobj->MoveTo(labelPosXY[lndex].x, labelPosXY[lndex].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        for (index = 0; index < 5; index++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            for (jndex = 0; jndex < 10; jndex++)
            {
                posX = boxPosXYs[index * 10 + jndex].x;
                posY = boxPosXYs[index * 10 + jndex].y;

                dmobj->MoveTo(posX, posY);
                Delay(50);

                fintout = 0;
                
                for (idx = 0; idx < LUAMAN_DIM(library_font_array_1); idx++)
                {
                    font = library_font_array_1[idx].font;
                    word = library_font_array_1[idx].word;
                    color_offset = library_font_array_1[idx].color_offset;
                
                    dmobj->SetDict(0, font);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("装备:单元格[%d:%d]:名称:%s:字库1:穿上:成功, lposX:%ld, lposY:%ld", index, jndex, word, lposX, lposY);

                        dmobj->MoveTo(posX, posY);
                        Delay(20);

                        dmobj->RightClick();
                        Delay(20);

                        fintout = 1;
                        break;
                    }
                }

                if (fintout)
                {
                    continue;
                }

                for (idx = 0; idx < LUAMAN_DIM(library_font_array_2); idx++)
                {
                    font = library_font_array_2[idx].font;
                    word = library_font_array_2[idx].word;
                    color_offset = library_font_array_2[idx].color_offset;
                
                    dmobj->SetDict(0, font);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("装备%d:单元格[%d:%d]:名称:%s:字库2:穿上:成功, lposX:%ld, lposY:%ld",
                            mov2posIdx, index, jndex, word, lposX, lposY);

                        dmobj->MoveTo(posX, posY);
                        Delay(20);

                        dmobj->LeftClick();
                        Delay(RIGHT_CLICK_DELAY_TIME);

                        //移到到相对于公用的偏移位置
                        lposX = lposX2 + mov2posIdx_pos_info[mov2posIdx].x;
                        lposY = lposY2 + mov2posIdx_pos_info[mov2posIdx].y;

                        dmobj->MoveTo(lposX, lposY);
                        Delay(20);

                        dmobj->LeftClick();
                        Delay(RIGHT_CLICK_DELAY_TIME);
                        
                        mov2posIdx++;
                        fintout = 1;
                        break;
                    }
                }

                if (fintout)
                {
                    continue;
                }
                
                for (idx = 0; idx < LUAMAN_DIM(library_font_array_3); idx++)
                {
                    font = library_font_array_3[idx].font;
                    word = library_font_array_3[idx].word;
                    color_offset = library_font_array_3[idx].color_offset;
                
                    dmobj->SetDict(0, font);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("装备:单元格[%d:%d]:名称:%s:字库3:穿上:成功, lposX:%ld, lposY:%ld", index, jndex, word, lposX, lposY);

                        dmobj->MoveTo(posX, posY);
                        Delay(20);

                        dmobj->RightClick();
                        Delay(20);
                        break;
                    }
                }
            }
        }
    }
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);
    
    api_log_MsgDebug("归还:仓库:完毕");
    return SNAIL_ERRNO_SUCCESS;
}

static int do_work(Idmsoft* dmobj, void* ctxt, int dotask_num)
{
    int status = 0;
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int i = 0, j = 0, k = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int result = 0;
    long posX0 = 0, posY0 = 0, posX1 = 0, posY1 = 0;
    long deltaX = 0, deltaY = 0;
    char* word = NULL;
    char* font = NULL;
    char* color_offset = NULL;
    int findout = 0;
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //检查是否按成功
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("背包:找字:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("背包:找字:失败");
        Delay(500);
    }

    //点击整理
    do_click_tidy_package(luaman_task, dmobj, lposX2, lposY2);
    
    //点击服饰 880:121 -> 876:151
    dmobj->MoveTo(lposX2 - 4, lposY2 + 30);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    
    //移动到背包相对位置点击
    dmobj->MoveTo(lposX2 + pkgpos_relative[0].x, lposY2 + pkgpos_relative[0].y);
    Delay(200);
    
    for (i = 0; i < 4; i++)
    {
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-ziku1-ShiZhuang.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "紧身泳衣", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            result = 1;
            
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("紧身泳衣:%s:查找:成功, lposX:%ld, lposY:%ld", SWIM_COAT_COLOR, lposX, lposY);
            break;
        }

        api_log_MsgDebug("紧身泳衣:%s:查找:失败", SWIM_COAT_COLOR);
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    Delay(200);
        
    //关掉背包
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyDownChar("i");
        Delay(300);
        dmobj->KeyUpChar("i");
        Delay(300);

        //检查是否按成功
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("背包:关闭:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("背包:关闭:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    if (result)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        status = signle_pri_clickhome(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != status)
        {
            api_log_MsgError("回城:失败:status:%d", status);
            return status;
        }

        api_log_MsgDebug("回城:成功");
        goto do_copy_work;
    }

    //查找坐标范围
    for (i = 0; i < 4; i++)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(300);

        //检查是否到达目标坐标
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pos_info[0].x) <= 3 && abs(posY - pos_info[0].y) <= 3)
                {
                    api_log_MsgDebug("坐标:%ld:%ld:查找:成功", pos_info[0].x, posY - pos_info[0].y);
                    goto do_dialog;
                }

                api_log_MsgDebug("坐标:%ld:%ld:查找:失败", pos_info[0].x, posY - pos_info[0].y);
            }
        }

        Delay(500);
    }

    status = signle_pri_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgError("回城:失败:status:%d", status);
        return status;
    }

    api_log_MsgDebug("回城:成功");

    //寻路
    pathofposxys = WALK_PATH_FRIST();
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[???]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[???]:成功");
    api_log_MsgDebug("开始:对话");
    
do_dialog:
    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
        dmobj->KeyDownChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);

        dmobj->SetDict( 0, FONT_EXSIGNLE_DIR"font-cunfang1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "仓库存放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("仓库存放:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Delay(500);
        api_log_MsgDebug("仓库存放:查找:失败");
    }

    //点击仓库存放
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-cunfang3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "公用", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("公用:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("公用:查找:失败");
        Delay(500);
    }

    //仓库 630:146 -> 公用 620:170
    dmobj->MoveTo(lposX2, lposY2);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->MoveTo(5, 5);
    Delay(200);

    //遍历窗口格子 620:170 -> 480:190 -> 800:510
    posX0 = lposX2 - 140;
    posY0 = lposY2 + 20;
    posX1 = lposX2 + 180;
    posY1 = lposY2 + 340;
    deltaX = (posX1 - posX0) / 10;
    deltaY = (posY1 - posY0) / 10;

    posX0 = posX0 + deltaX / 2;
    posY0 = posY0 + deltaY / 2;

    api_log_MsgDebug("deltaX  : %ld", deltaX);
    api_log_MsgDebug("deltaY  : %ld", deltaY);
    api_log_MsgDebug("posX0   : %ld", posX0);
    api_log_MsgDebug("posY0   : %ld", posY0);
    
    for (i = 0; i < 10; i++)
    {
        if (i > 2 && i != 6)
        {
            continue;
        }
        
        posY = posY0 + i * deltaY;

        for (j = 0; j < 10; j++)
        {
            posX = posX0 + j * deltaX;

            dmobj->MoveTo(posX, posY);
            Delay(KEY_MOVE_DELAY_TIME);

            findout = 0;
            
            for (k = 0; k < LUAMAN_DIM(library_font_array_1); k++)
            {
                font = library_font_array_1[k].font;
                word = library_font_array_1[k].word;
                color_offset = library_font_array_1[k].color_offset;
            
                dmobj->SetDict(0, font);
                code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("装备:单元格[%d:%d]:名称:%s:查找:字库1:成功, lposX:%ld, lposY:%ld", i, j, word, lposX, lposY);

                    dmobj->MoveTo(posX, posY);
                    Delay(KEY_MOVE_DELAY_TIME);

                    dmobj->RightClick();
                    Delay(KEY_MOVE_DELAY_TIME);

                    findout = 1;
                    break;
                }
            }

            if (findout)
            {
                continue;//不用继续查找了
            }
            
            for (k = 0; k < LUAMAN_DIM(library_font_array_2); k++)
            {
                font = library_font_array_2[k].font;
                word = library_font_array_2[k].word;
                color_offset = library_font_array_2[k].color_offset;
            
                dmobj->SetDict(0, font);
                code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("装备:单元格[%d:%d]:名称:%s:字库2:查找:成功, lposX:%ld, lposY:%ld", i, j, word, lposX, lposY);

                    dmobj->MoveTo(posX, posY);
                    Delay(KEY_MOVE_DELAY_TIME);

                    dmobj->RightClick();
                    Delay(KEY_MOVE_DELAY_TIME);
                    
                    break;
                }
            }
        }
    }

    //按2下ESC
    dmobj->KeyDownChar("ESC");
    Delay(200);
    dmobj->KeyUpChar("ESC");
    Delay(200);
    dmobj->KeyDownChar("ESC");
    Delay(200);
    dmobj->KeyUpChar("ESC");
    Delay(200);

    status = signle_pri_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgError("回城:失败:status:%d", status);
        return status;
    }

    api_log_MsgDebug("回城:成功");
    
    status = coaton(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgDebug("外套:穿上:失败, status:%d", status);
        return status;
    }

    api_log_MsgDebug("外套:穿上:成功");

do_copy_work:
    return signle_pri_copy(dmobj, ctxt, dotask_num);
}

int start_work(struct cluamanaccountinfo* accountinfo, int roleno, Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX0 = 0, lposY0 = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    int i = 0;
    struct passwd2{char ch;char* path;} bmps[]=
    {
        {'0', BMP_EXSIGNLE_DIR"0.bmp"},
        {'1', BMP_EXSIGNLE_DIR"1.bmp"},
        {'2', BMP_EXSIGNLE_DIR"2.bmp"},
        {'3', BMP_EXSIGNLE_DIR"3.bmp"},
        {'4', BMP_EXSIGNLE_DIR"4.bmp"},
        {'5', BMP_EXSIGNLE_DIR"5.bmp"},
        {'6', BMP_EXSIGNLE_DIR"6.bmp"},
        {'7', BMP_EXSIGNLE_DIR"7.bmp"},
        {'8', BMP_EXSIGNLE_DIR"8.bmp"},
        {'9', BMP_EXSIGNLE_DIR"9.bmp"},
    };
    int j = 0;
    char* path = NULL;
    int dotask_num = 0;
    time_t check_conn_start;
    time_t check_conn_stop;
    double check_conn_deltaTime = 0.0;
    //角色名字数组={A?, B?, C?,...,}
    //char* role_names[] = EXSIGNLE_ROLES();
    char* role_name = NULL;
    struct posxy_t posxys[] = {
        {1219, 608},
        {1219, 608},
        {1219, 608},
        {1105, 607},
        {1105, 607},
        {1105, 607},
    };

    luaman_config->roleno = roleno;
    luaman_config->user_name = accountinfo->user_name;
    luaman_config->enter_with_role = enter_with_role;
    
    if (roleno >= EXSIGNLE_ROLES_NR || roleno < 0)
    {
        api_log_MsgError("任务:副本:角色:%d:参数:错误", roleno);
        return SNAIL_ERRNO_INVALID;
    }

    //role_name = role_names[roleno];
    api_log_MsgDebug("副本:角色:任务:执行:开始");
    api_log_MsgDebug("user       : %s", accountinfo->user_name);
    api_log_MsgDebug("passwd     : %s", accountinfo->passwd);
    api_log_MsgDebug("passwdex   : %s", accountinfo->passwdex);
    api_log_MsgDebug("roleno     : %d", roleno);
    //api_log_MsgDebug("role_name  : %s", role_name);
    
label_login:
    dmobj->MoveTo(5, 5);
    Delay(200);

    check_conn_start = time(NULL);
    
    //查找“帐号”
    //for (;;)
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-login.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "帐号", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
                
            api_log_MsgDebug("帐号:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        api_log_MsgDebug("帐号:查找:失败");
        Delay(500);

        check_conn_stop = time(NULL);
        check_conn_deltaTime = difftime(check_conn_stop, check_conn_start);
        
    }
    for_timeout_check_end()
    
    //帐号 490:648 -> 600:655
    //dmobj->MoveTo(603, 647);
    dmobj->MoveTo(lposX2 + 110, lposY2 + 7);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    code = dmobj->SendString(hwnd, accountinfo->user_name);
    if (1 != code)
    {
        api_log_MsgError("帐号:%s:输入:失败", accountinfo->user_name);
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("帐号:%s:输入:成功", accountinfo->user_name);

    //密码 490:648 -> 600:680
    //dmobj->MoveTo(603, 672);
    dmobj->MoveTo(lposX2 + 110, lposY2 + 32);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    code = dmobj->SendString(hwnd, accountinfo->passwd);
    if (1 != code)
    {
        api_log_MsgError("密码:%s:输入:失败", accountinfo->passwd);
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("密码:%s:输入:成功", accountinfo->passwd);

    //进入游戏 490:648 -> 530:720
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        //dmobj->MoveTo(509, 730);
        dmobj->MoveTo(lposX2 + 50, lposY2 + 72);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-login.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "帐号", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (0 != code)
        {
            api_log_MsgDebug("账户:%s:登录:成功", accountinfo->user_name);
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
            
        api_log_MsgDebug("账户:%s:登录:失败, lposX:%ld, pposY:%ld", accountinfo->user_name, lposX, pposY);
        Delay(1000);
    }

    check_conn_start = time(NULL);
    
    //检查二级密码
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-security.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "请谨慎输入二级密码", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            lposX0 = lposX;
            lposY0 = lposY;
            
            api_log_MsgDebug("二级:密码:查找:成功, lposX:%ld, pposY:%ld", lposX, lposY);
            break;
        }

        //检查是否出现错误
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-loginerr.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "发生错误，请重新登录。", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("发生错误，请重新登录。:查找:成功, lposX:%ld, pposY:%ld", lposX, lposY);

            //点击“发生错误，请重新登录。” 523:242 -> 640:533
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                dmobj->MoveTo(lposX + 117, lposY + 291);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-loginerr.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "发生错误，请重新登录。", "ffffff-101010", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("发生错误，请重新登录。:点击:成功");
                    goto label_login;
                }

                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;

                api_log_MsgDebug("发生错误，请重新登录。:点击:失败, lposX2:%ld, pposY2:%ld", lposX2, lposY2);
            }
        }

        check_conn_stop = time(NULL);
        check_conn_deltaTime = difftime(check_conn_stop, check_conn_start);
        
        
        api_log_MsgDebug("二级:密码:查找:失败");
        Delay(1000);
    }

    for (i = 0; i < strlen(accountinfo->passwdex); i++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        path = NULL;
        for (j = 0; j < sizeof(bmps)/sizeof(bmps[0]); j++)
        {
            if (bmps[j].ch == accountinfo->passwdex[i])
            {
                path = bmps[j].path;
                break;
            }
        }

        if (NULL == path)
        {
            api_log_MsgError("二次密码:非数字:不支持");
            return ERRNO_NOT_SUPPORT;
        }
        
        api_log_MsgDebug("输入:二次:密码:%s", path);

        dmobj->MoveTo(5, 5);
        Delay(200);
        
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-security.txt");
            code = dmobj->FindPic(496, 310, 781, 417, path, "101010", 0.9, 0, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("二次:密码:%s:查找:成功, lposX:%ld, lposY:%ld", path, lposX, lposY);
                break;
            }

            api_log_MsgDebug("二次:密码:%s:查找:失败", path);
            Delay(1000);
        }

        //点击二次密码
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        api_log_MsgDebug("点击:二次:密码:%s:成功", path);
    }

    check_conn_start = time(NULL);
    
    //点击确认,请谨慎输入二级密码: 586:266 -> 686:430
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX0 + 100, lposY0 + 164);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-security.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "请谨慎输入二级密码", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("二级:密码:确认:成功");
            break;
        }

        check_conn_stop = time(NULL);
        check_conn_deltaTime = difftime(check_conn_stop, check_conn_start);
        
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;
                
        api_log_MsgDebug("二级:密码:确认:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(2000);
    }
    
    api_log_MsgDebug("进入:游戏:成功:准备:开始");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);

    int clicknr = 0;
    
    /////////////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        dmobj->MoveTo(5, 5);
        Delay(500);

        for (i = 0; i < LUAMAN_DIM(lfi_pri); i++)
        {
            if (strcmp(lfi_pri[i].login, accountinfo->user_name))
            {
                api_log_MsgDebug("账号:%s:匹配:失败", accountinfo->user_name);
                continue;
            }

            role_name = lfi_pri[i + roleno].word;
            
            api_log_MsgDebug("账号:%s:角色:%s:匹配:成功", accountinfo->user_name, role_name);

            for (j = 0; j < LUAMAN_DIM(lfi_pri[i + roleno].color_offset); j++)
            {
                dmobj->SetDict(0, lfi_pri[i + roleno].font);
                code = dmobj->FindStrFast(0, 27, 1279, 826, role_name, lfi_pri[i + roleno].color_offset[j], 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("账号:%s:角色:%d:名称:%s:颜色:%s:查找:成功, lposX:%ld, lposY:%ld",
                        accountinfo->user_name, roleno, role_name, lfi_pri[i + roleno].color_offset[j],lposX, lposY);
                    goto label_click_role;
                }

                api_log_MsgDebug("账号:%s:角色:%d:名称:%s:字库:%s:颜色:%s:查找:失败",
                    accountinfo->user_name, roleno, role_name, lfi_pri[i + roleno].font,
                    lfi_pri[i + roleno].color_offset[j]);
            }

            api_log_MsgDebug("账号:%s:角色:%s:查找:失败", accountinfo->user_name, role_name);
            break;
        }

        //点击三角形箭头翻页再继续查找
        dmobj->MoveTo(posxys[clicknr].x, posxys[clicknr].y);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(500);
    
        clicknr = (clicknr + 1) % LUAMAN_DIM(posxys);
        
        api_log_MsgDebug("账号:%s:角色:%d:名称:%s:查找:失败:翻页", accountinfo->user_name, roleno, role_name);
        Delay(500);
    }

label_click_role:
    //dmobj->Capture(0, 27, 1279, 826, "cap\\sel_role0.bmp");
    
    //点击选择角色
    dmobj->MoveTo(lposX, lposY);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);

    //dmobj->Capture(0, 27, 1279, 826, "cap\\sel_role1.bmp");
    
    /////////////////////////////////////////////////////////////////////////////////
    //点击进入游戏
    dmobj->MoveTo(640, 670);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(200);

    i = 0;
    
next:
    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dotask_num = 0;
    
    //执行“任务副本”
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        code = do_work(dmobj, ctxt, dotask_num);
        if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("角色:任务:执行:完成,dotask_num:%d, code:%d",
                dotask_num + 1, code);
            break;
        }

        dotask_num++;
        api_log_MsgDebug("副本:任务:继续,dotask_num:%d", dotask_num);

        if (1 == dotask_num)
        {
            api_log_MsgDebug("副本:任务:完成:继续:下一个:账户");
            break;
        }
    }

    api_log_MsgDebug("副本:任务:完成,dotask_num:%d", dotask_num);

    //清包
    (void)clean_package(dmobj, ctxt);

    //寻路
    (void)signle_pri_postwalk(dmobj, ctxt);

    //脱衣服
    (void)coatoff(dmobj, ctxt);

    //脱泳衣
    (void)swimcoatoff(dmobj, ctxt);

    (void)back2library(dmobj, ctxt);

    //退出副本
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        code = check_alarm(dmobj, ctxt);
        if (ERRNO_ALARM == code)
        {
            Delay(1000);
            api_log_MsgDebug("准备:重新登录:报警:检测:成功");
            continue;
        }
        
        //点击 1262:747
        dmobj->MoveTo(1262, 747);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-logout.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "重新登录", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("重新登录:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("重新登录:查找:失败");
        Delay(500);
    }

    //点击"重新登录"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-saveconf.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "游戏信息保存", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("游戏信息保存:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("游戏信息保存:查找:失败");
        Delay(500);
    }

    //等待“游戏信息保存”消失
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-saveconf.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "游戏信息保存", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("游戏信息保存:完成");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
            
        api_log_MsgDebug("游戏信息保存:进行, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    return ERRNO_SUCCESS;
}

static void __this_null(void){}

