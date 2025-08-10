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
#include "taskSignleCpy1.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "Markup.h"

/*
source insight常用技能:
  1.搜索关键词:
    (1).双击选择要搜索的关键词
    (2).同时按住ctrl和/键，弹出搜索对话框
    (3).点击search执行搜索,如果有多个搜素结果,按下面方法逐个查看
      <a>.点击左侧的箭头,自动跳到相应的内容
      <b>.同时按住alt和逗号键,返回到搜索结果,继续查看下一个,依次类推
*/
////////////////////////////////////////////////////////////////////////////////
//副本7贵宾服务相对偏移值
static struct luaman_posxy deltaatpos_single_cpy7[] = 
{
    {-321, 310},
    {-314, 325},
    {-413, 324},
    {-410, 245},
    {-315, 242},
    {-364, 204},
    {-362, 185},
    {-363, 151},
    {-348, 67},
};

#define signlecpy7_fight_atpoint_1() {\
    pirate_fight_time(5, 12),\
}

#define signlecpy7_fight_atpoint_2() {\
    pirate_fight_time(2, 22),\
}

#define signlecpy7_fight_atpoint_3() {\
    pirate_fight_time(5, 12),\
}

#define signlecpy7_fight_atpoint_4() {\
    pirate_fight_time(2, 22),\
}

#define signlecpy7_fight_atpoint_5() {\
    pirate_fight_time(5, 12),\
}

#define signlecpy7_fight_atpoint_6() {\
    pirate_fight_time(2, 22),\
}

#define signlecpy7_fight_atpoint_7() {\
    pirate_fight_time(10, 12),\
}

#define signlecpy7_fight_atpoint_8() {\
    pirate_fight_time(2, 25),\
}

#define signlecpy7_fight_atpoint_9() {\
    pirate_fight_time(1, 5),\
}

#define signlecpy7_fight_atpoint_10() {\
    pirate_fight_time(1, 5),\
}

#define signlecpy7_fight_atpoint_11() {\
    pirate_fight_time(1, 27),\
}


//找到了点击黄色的入侵警报器(font-DuiHua4.txt)的相对位置
static struct luaman_posxy single_cpy7_ips_offset[] = 
{
    {31, 70},
    {31, 70},
    {31, 70},
    {31, 70},
};

//11，按-号，等3秒，按M，点击贵宾服务相对位置（7），按-，等1秒。再按M。找黄色的结界石(font-DuiHua5.txt)。
//    找到了点击黄色的结界石的相对位置。
static struct luaman_posxy single_cpy7_jjs_offset[] = 
{
    {20, 76},
};
//副本7配置结束
////////////////////////////////////////////////////////////////////////////////


//愚人节中的3个副本贵宾服务相对位置
static struct luaman_posxy deltaatpos_single_cpy_foolish[] = 
{
    {-563, 80},
    {-511, 126},
    {-446, 158},
};

// 3  进来后，坐标变成58:70.按-号键，等3秒，按M，点击贵宾服务相对位置（1）.
//   按0，等1秒。开始打怪1.（我自己配时间，等多少，打多少那个）。
// 时间: 等多少，打多少
#define signlecpy_foolish_fight_atpoint_1() {\
    pirate_fight_time(30, 210),\
}

//5  按-号键，等3秒，按M，点击贵宾服务相对位置（1），按0，等1秒。开始打怪，
//   一边打，一边找左上角的消灭等待(font-foolish2.txt)。
//   找到了就停止，按空格30下。
//消灭等待的颜色
#define signlecpy_foolish_diewait_font_color "fdb602-101010"

////////////////////////////////////////////////////////////////////////////////
/* 贵宾服务相对偏移值 */
static struct luaman_posxy deltaatpos_single_cpy6[] = 
{
    {-250, 324},
    {-12, -13},//6，领完卷，再按N，点对话的相对位置2（deltaatpos_single_cpy6 第2个）
    {-366, 258},
    {-373, 218},
};

//步骤7：等待固定时间(单位:秒)
static int single_cpy_wait_seconds = 30;

//8.按N对话，点击魔法阵（font-cpy6_npc6.txt）。然后打怪（按等几秒打几秒的方式，打几个回合）
#define signlecpy1_fight_atpoint_1() {\
    pirate_fight_time(35, 35),\
    pirate_fight_time(35, 35),\
    pirate_fight_time(30, 30),\
}


////////////////////////////////////////////////////////////////////////////////////////////////
#define FONTSIGNLECPY1_DIR "font\\signle\\"
#define BMPSIGNLECPY1_DIR "bmp\\signle\\"

static HANDLE g_signlecpy_criticalSection;
void signlecpy_InitCriticalSection()
{
    g_signlecpy_criticalSection = ::CreateMutex(NULL, FALSE, NULL);
    if (NULL == g_signlecpy_criticalSection)
    {
        api_log_MsgError("create signlecpy_InitCriticalSection lock failed");
        exit(1);
    }
    return;
}

static void signlecpyEnterCriticalSection()
{
    ::WaitForSingleObject(g_signlecpy_criticalSection, INFINITE);
    return;
}

static void signlecpyLeaveCriticalSection()
{
    (void)::ReleaseMutex(g_signlecpy_criticalSection);
    return;
}

static void signlecpyDeleteCriticalSection()
{
    (void)::CloseHandle(g_signlecpy_criticalSection);
    return;
}

static int signle_checkkeystatecpy1(Idmsoft* dmobj, void* ctxt)
{
    if (1 == dmobj->GetKeyState(65))
    {
        dmobj->KeyUp(65);
        LUAMAN_WAIT_MS(50);
    }
    if (1 == dmobj->GetKeyState(68))
    {
        dmobj->KeyUp(68);
        LUAMAN_WAIT_MS(50);
    }
    if (1 == dmobj->GetKeyState(83))
    {
        dmobj->KeyUp(83);
        LUAMAN_WAIT_MS(50);
    }
    if (1 == dmobj->GetKeyState(87))
    {
        dmobj->KeyUp(87);
        LUAMAN_WAIT_MS(50);
    }

    dmobj->KeyDownChar("a");
    LUAMAN_WAIT_MS(50);
    dmobj->KeyUpChar("a");
    LUAMAN_WAIT_MS(50);

    dmobj->KeyDownChar("s");
    LUAMAN_WAIT_MS(50);
    dmobj->KeyUpChar("s");
    LUAMAN_WAIT_MS(50);

    dmobj->KeyDownChar("w");
    LUAMAN_WAIT_MS(50);
    dmobj->KeyUpChar("w");
    LUAMAN_WAIT_MS(50);

    dmobj->KeyDownChar("d");
    LUAMAN_WAIT_MS(50);
    dmobj->KeyUpChar("d");
    LUAMAN_WAIT_MS(50);

    return ERRNO_SUCCESS;
}

static int signle_checklifecpy1(Idmsoft* dmobj, void* ctxt, int recusive)
{
    return 0;
}

static int signle_walkcpy1(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int dir = luaman_config->direction;
    ROLE_E role = luaman_config->role;
    int groupid = luaman_config->groupid;
    int code = 0;
    struct luamanwalkstate walkstate;
    CStringArray tgtPosXY;
    CStringArray tgtPosXYe;
    int tgtNum = 0;
    char* ptr = NULL;
    int index = 0;
    long tposX = 0, tposY = 0;
    int xyc = 0;
    int rdcltErrorCounter = 0;
    CString strPosXY;
    time_t startWalkTime = time(NULL);
    time_t curTime = 0;
    double walkTimeDelta = 0.0;
    int posX = 0;
    int posY = 0;
    CStringArray posXY;
    int keyPauseTime = 20;
    VARIANT pposX, pposY;
    //VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    //long lposX2 = 0, lposY2 = 0;

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    walkstate.xNeg = "w";
    walkstate.xPos = "s";
    walkstate.yNeg = "a";
    walkstate.yPos = "d";

    if (cdir != dir)
    {
        dir = cdir;
    }
    
    if (DIR_LEFT == dir)
    {
        walkstate.xNeg = "d";
        walkstate.xPos = "a";
        walkstate.yNeg = "w";
        walkstate.yPos = "s";
    }

    if (DIR_U == dir)
    {
        dmobj->MiddleClick();
        LUAMAN_WAIT_MS(200);
        dmobj->MiddleClick();
        LUAMAN_WAIT_MS(200);
    }

    tgtNum = SplitString(tgtPosXYs, '|', tgtPosXY);
    if (tgtNum < 0)
    {
        ptr = tgtPosXYs.GetBuffer(BUFFER_MAX);
        api_log_MsgError("路径:坐标:非法:寻路:失败,tgtNum:%d, tgtPosXYs:%s", tgtNum, ptr);
        
        tgtPosXYs.ReleaseBuffer();
        return SNAIL_ERRNO_INVALID;
    }
    
    //前进到每个坐标点
    for (index = 0; index < tgtNum; ++index)
    {
        xyc = SplitString(tgtPosXY.GetAt(index), ':', tgtPosXYe);
        if (xyc < 2)
        {
            ptr = tgtPosXYs.GetBuffer(BUFFER_MAX);
            
            api_log_MsgError("路径:坐标:参数:非法:寻路:失败,index:%d, tgtPosXYs:%s, xyc:%d",
                index, ptr, xyc);
        
            tgtPosXYs.ReleaseBuffer();
            return ERRNO_INVALID;
        }

        tposX = atol(tgtPosXYe.GetAt(0));
        tposY = atol(tgtPosXYe.GetAt(1));

        api_log_MsgDebug("坐标:解析:成功,index:%d, tposX:%ld, tposY:%ld", index, tposX, tposY);

        walkstate.yNegKeyDown = 0;
        walkstate.yPosKeyDown = 0;
        walkstate.xPosKeyDown = 0;
        walkstate.xNegKeyDown = 0;

        //Check_Terminate_Thread(luaman_task, dmobj);
        
        //前往下一目标坐标点
        for (;;)
        {
        WALK_LABEL_NEXT:
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            //////////////////////////////////////////////////////////////////////////////
            //@bug:检查寻路时间是否超过阈值120秒,如果超过则认为是被卡住了,退出 2019/4/20 10:09:39
            //bug:修改为40秒 2022/01/10 21:57
            curTime = time(NULL);
            walkTimeDelta = difftime(curTime, startWalkTime);
            if (walkTimeDelta > 30.0)
            {
                (void)signle_checkkeystatecpy1(dmobj, ctxt);

                api_log_MsgDebug("寻路:坐标:卡住,停止寻路:等待副本消失,下一坐标, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //等待地洞副本消失
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //先检查下血条
            code = signle_checklifecpy1(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //读取当前坐标
            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)signle_checkkeystatecpy1(dmobj, ctxt);
                
                walkstate.yNegKeyDown = 0;
                walkstate.yPosKeyDown = 0;
                walkstate.xPosKeyDown = 0;
                walkstate.xNegKeyDown = 0;

                api_log_MsgWarn("地图:坐标:读取:失败");
                goto WALK_LABEL_NEXT;
            }

            //解析当前坐标
            xyc = SplitString(strPosXY, ':', posXY);
            if (xyc < 2)
            {
                ptr = strPosXY.GetBuffer(BUFFER_MAX);
                
                api_log_MsgWarn("地图:坐标:解析:失败,strPosXY:%s, xyc:%d",
                    ptr, xyc);
            
                strPosXY.ReleaseBuffer();

                (void)signle_checkkeystatecpy1(dmobj, ctxt);
                
                walkstate.yNegKeyDown = 0;
                walkstate.yPosKeyDown = 0;
                walkstate.xPosKeyDown = 0;
                walkstate.xNegKeyDown = 0;
                
                goto WALK_LABEL_NEXT;
            }

            posX = atol(posXY.GetAt(0));
            posY = atol(posXY.GetAt(1));

            if (posX - tposX > 1)
            {
                if (posY - tposY > 1)
                {
                    /* 继续走 */
                    dmobj->KeyDownChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 1;
                    
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yNegKeyDown = 1;
                }
                else if (posY - tposY < -1)
                {
                    /* 继续走 */
                    dmobj->KeyDownChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 1;

                    dmobj->KeyDownChar(walkstate.yPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yPosKeyDown = 1;
                }
                else
                {
                    if (1 == walkstate.yNegKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yNeg);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yNegKeyDown = 0;
                    }
                    
                    if (1 == walkstate.yPosKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yPos);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yPosKeyDown = 0;
                    }

                    /* Y轴坐标不动,X轴移动 */
                    dmobj->KeyDownChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 1;
                }
            }
            else if (posX - tposX < -1)
            {
                if (posY - tposY > 1)
                {
                    /* 继续走 */
                    dmobj->KeyDownChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 1;
                    
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yNegKeyDown = 1;
                }
                else if (posY - tposY < -1)
                {
                    /* 继续走 */
                    dmobj->KeyDownChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 1;
                    
                    dmobj->KeyDownChar(walkstate.yPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yPosKeyDown = 1;
                }
                else
                {
                    if (1 == walkstate.yNegKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yNeg);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yNegKeyDown = 0;
                    }

                    if (1 == walkstate.yPosKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yPos);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yPosKeyDown = 0;
                    }

                    dmobj->KeyDownChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 1;
                }
            }
            else
            {
                if (1 == walkstate.xPosKeyDown)
                {
                    dmobj->KeyUpChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 0;
                }

                if (1 == walkstate.xNegKeyDown)
                {
                    dmobj->KeyUpChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 0;
                }

                /* X轴坐标不动,只判断Y轴 */
                if (posY - tposY > 1)
                {
                    /* 继续走 */
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yNegKeyDown = 1;
                }
                else if (posY - tposY < -1)
                {
                    /* 继续走 */
                    dmobj->KeyDownChar(walkstate.yPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yPosKeyDown = 1;
                }
                else
                {
                    if (1 == walkstate.yNegKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yNeg);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yNegKeyDown = 0;
                    }

                    if (1 == walkstate.yPosKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yPos);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yPosKeyDown = 0;
                    }

                    if (1 == walkstate.xPosKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.xPos);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.xPosKeyDown = 0;
                    }

                    if (1 == walkstate.xNegKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.xNeg);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.xNegKeyDown = 0;
                    }

                    LUAMAN_WAIT_MS(keyPauseTime);

                    //当前目标坐标点处理完毕
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            //WALK_LABEL_NEXT
        }//end:前往下一目标坐标点

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
    }

    //寻路过程完毕,顺利到达目标终点坐标点
    return SNAIL_ERRNO_SUCCESS;

LABEL_WALK_BLOCK_EXIT_CHECK:
    //检查地图是否退出到副本外面
    for (;;)
    {
        //先检查下血条
        code = signle_checklifecpy1(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:卡住:血条:检查:死亡:复活");
            return code;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "平静河谷", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("寻路:卡住:地图:平静河谷:退出,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        Delay(500);
        api_log_MsgDebug("寻路:卡住:地图:平静河谷:退出,正在退出中");
    }

    return SNAIL_ERRNO_BLOCKED;
}

static int beat_cpy_timeout(Idmsoft* dmobj, void* ctxt, int tottime, int enterspace)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > (tottime * 1.0))
        {
            api_log_MsgDebug("副本:%d秒:完成", tottime);
            break;
        }

        api_log_MsgDebug("副本:%d秒:继续", tottime);
    }

    if (enterspace)
    {
        for (idx = 0; idx < 30; idx++)
        {
            dmobj->KeyDownChar("space");
            Delay(20);
            dmobj->KeyUpChar("space");
            Delay(20);
        }
    }
    
    api_log_MsgDebug("副本:阶段:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}


static int beat_cpy1(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(20);
        dmobj->KeyUpChar("u");
        Delay(20);

        for (idx = 0; idx < 40; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        //找左上角的等待时间
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-waittime.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "等待时间:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("副本1:等待时间:读取:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("副本1:等待时间:读取:失败:继续:打怪");
    }

    lastChckTime = time(NULL);
    
    //打5秒
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(20);
        dmobj->KeyUpChar("u");
        Delay(20);

        for (idx = 0; idx < 40; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 5.0)
        {
            api_log_MsgDebug("副本1:打宝:5秒:完成");
            break;
        }

        api_log_MsgDebug("副本1:打宝:5秒:继续");
    }

    api_log_MsgDebug("副本1:按空格:15秒:开始");
    
    for (idx = 0; idx < 250; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);

        if (idx % 20)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
        }
    }

    api_log_MsgDebug("副本1:按空格:15秒:结束");

    //TODO:等待一个固定时间后出去 罗爱平 2023/1/8 23:06:11
    Delay_With_Terminate_Check(25, luaman_task, dmobj);

    dmobj->KeyDownChar("0");
    Delay(50);
    dmobj->KeyUpChar("0");
    Delay(50);

    //检查地图是不是变成大型竞技场广场
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "大型竞技场广场", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:大型竞技场广场:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:大型竞技场广场:查找:失败");
        Delay(1000);
    }

    api_log_MsgDebug("副本1:执行:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy2(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(20);
        dmobj->KeyUpChar("u");
        Delay(20);

        for (idx = 0; idx < 40; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        //找左上角的等待时间
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-waittime.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "等待时间:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("副本2:等待时间:读取:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("副本2:等待时间:读取:失败:继续:打怪");
    }

    lastChckTime = time(NULL);
    
    //打5秒
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(20);
        dmobj->KeyUpChar("u");
        Delay(20);

        for (idx = 0; idx < 40; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 5.0)
        {
            api_log_MsgDebug("副本2:打宝:5秒:完成");
            break;
        }

        api_log_MsgDebug("副本2:打宝:5秒:继续");
    }

    api_log_MsgDebug("副本2:按空格:15秒:开始");
    
    for (idx = 0; idx < 100; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("副本2:按空格:15秒:结束");

    //TODO:等待一个固定时间后出去
    Delay_With_Terminate_Check(10, luaman_task, dmobj);

    dmobj->KeyDownChar("0");
    Delay(50);
    dmobj->KeyUpChar("0");
    Delay(50);

    //检查地图是不是变成大型竞技场广场
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "大型竞技场广场", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:大型竞技场广场:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:大型竞技场广场:查找:失败");
        Delay(1000);
    }

    api_log_MsgDebug("副本2:执行:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy3_phase1(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-9-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "通往神殿接见室的路开启了", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("通往神殿接见室的路开启了:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (50 * 1.0))
        {
            api_log_MsgDebug("通往神殿接见室的路开启了:打怪:超时");
            break;
        }
        
        api_log_MsgDebug("通往神殿接见室的路开启了:查找:失败");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("副本3:阶段1:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy3_phase2(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-20-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "通往葵恩广场的路开启了", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("通往葵恩广场的路开启了:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (120 * 1.0))
        {
            api_log_MsgDebug("通往葵恩广场的路开启了:打怪:超时");
            break;
        }

        api_log_MsgDebug("通往葵恩广场的路开启了:查找:失败");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("副本3:阶段2:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy3_phase3_0(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    int istimeout = 0;

    dmobj->MoveTo(5, 5);
    Delay(200);

    api_log_MsgDebug("副本3:阶段3_0:打怪:开始");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "被洗脑的火之守护者", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("被洗脑的火之守护者:红色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "被洗脑的火之守护者", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("被洗脑的火之守护者:白色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "被洗脑的火之守护者", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("被洗脑的火之守护者:绿色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("被洗脑的火之守护者:查找:失败");
        Delay(500);
    }

    api_log_MsgDebug("被洗脑的火之守护者:打怪:开始");
    
    lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        if (0 == istimeout)
        {
            for (idx = 0; idx < 40; idx++)
            {
                dmobj->KeyDownChar("3");
                Delay(20);
                dmobj->KeyUpChar("3");
                Delay(20);
            }

            //如果打怪超过2分钟,就按一下~,然后一直检查怪是否打死 2023/3/28 22:40:11
            deltaTime = difftime(time(NULL), lastChckTime);
            if (deltaTime > (70 * 1.0))
            {
                api_log_MsgDebug("被洗脑的火之守护者:打怪:超时");
                
                istimeout = 1;
                
                dmobj->KeyDownChar("~");
                Delay(200);
                dmobj->KeyUpChar("~");
                Delay(200);
            }
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "被洗脑的火之守护者", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("被洗脑的火之守护者:红色:打怪:成功");
          
            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi6.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "被洗脑的火之守护者", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("被洗脑的火之守护者:白色:打怪:成功");

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi6.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "被洗脑的火之守护者", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("被洗脑的火之守护者:绿色:打怪:成功:结束");
                    break;
                }
                else
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;
                    
                    api_log_MsgDebug("被洗脑的火之守护者:绿色:打怪:继续, lposX:%ld, lposY:%ld", lposX, lposY);
                }
            }
            else
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("被洗脑的火之守护者:白色:打怪:继续, lposX:%ld, lposY:%ld", lposX, lposY);
            }
        }
        else
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("被洗脑的火之守护者:红色:打怪:继续, lposX:%ld, lposY:%ld", lposX, lposY);
        }
        
        Delay(500);
    }

    for (idx = 0; idx < 50; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("副本3:阶段3_0:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}


static int beat_cpy3_phase3(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-32-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "通往破碎的回廊与污染的庭院的路开启了", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("通往破碎的回廊与污染的庭院的路开启了:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (200 * 1.0))
        {
            api_log_MsgDebug("通往破碎的回廊与污染的庭院的路开启了:打怪:超时");
            break;
        }
        
        api_log_MsgDebug("通往破碎的回廊与污染的庭院的路开启了:查找:失败");
        Delay(500);
    }

    for (idx = 0; idx < 50; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("副本3:阶段3:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy3_phase4(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "污染的庭院的结界石生成了", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("污染的庭院的结界石生成了:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (180 * 1.0))
        {
            api_log_MsgDebug("污染的庭院的结界石生成了:打怪:超时");
            break;
        }
        
        api_log_MsgDebug("污染的庭院的结界石生成了:查找:失败");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("副本3:阶段4:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy3_phase5(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 40.0)
        {
            api_log_MsgDebug("副本3:阶段5:40秒:完成");
            break;
        }

        api_log_MsgDebug("副本3:阶段5:40秒:继续");
    }

    for (idx = 0; idx < 20; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("副本3:阶段5:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy3_phase6(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        //找左上角的等待时间
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-waittime.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "等待时间:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("副本3:阶段6:等待时间:读取:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("副本3:阶段6:等待时间:读取:失败:继续:打怪");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("副本3:阶段6:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy4_phase2(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);
        
        for (idx = 0; idx < 30; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(30);
            dmobj->KeyUpChar("3");
            Delay(30);
        }

        //找左上角的等待时间
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa127.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "奖励等待时间:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("副本1:阶段6:奖励等待时间:读取:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("副本1:阶段6:奖励等待时间:读取:失败:继续:打怪");
        Delay(500);
    }

    code = beat_cpy_timeout(dmobj, ctxt, 10, 0);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    for (idx = 0; idx < 120; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("副本3:阶段6:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy5_phase1(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);
        
        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        //找左上角的等待时间
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-waittime.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "等待时间:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("副本5:阶段1:等待时间:读取:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("副本5:阶段1:等待时间:读取:失败:继续:打怪");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("副本5:阶段1:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy5_phase2(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "深渊的黑龙艾德", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("深渊的黑龙艾德:红色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "深渊的黑龙艾德", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("深渊的黑龙艾德:白色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "深渊的黑龙艾德", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("深渊的黑龙艾德:绿色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("深渊的黑龙艾德:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        //找左上角的等待时间
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-waittime.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "等待时间:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("副本5:阶段2:等待时间:读取:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("副本5:阶段2:等待时间:读取:失败:继续:打怪");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("副本5:阶段2:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int cpy1_join2team(Idmsoft* dmobj, void* ctxt)
{
    int code = 0;
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int syncCountMax = 2/*luaman_config->syncCountMax*/;
    struct xyposition syncPosXYs[4] =
    {
        {1234, 266},
        {1272, 286},
        {1234, 298},
        {1272, 318},
    };
    time_t lastChckTime = time(NULL);
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    struct xyposition fellowPosXYs[2];
    time_t lastChckTimeAll;
    int i = 0, j = 0;
    long posX = 0, posY = 0;
    long posX1 = 0, posY1 = 0;
    long posX2 = 0, posY2 = 0;
    int syncCount = 0;
    
    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    lastChckTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("f");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f");
        LUAMAN_WAIT_MS(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-inviteheil2team-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "添加好友", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("组队:邀请:添加好友:找字:大范围:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 20.0)
        {
            api_log_MsgDebug("组队:邀请:添加好友:找字:大范围,失败:超时");
            return SNAIL_ERRNO_FAILED;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("组队:邀请:添加好友:找字:大范围,失败");
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(200);

    lastChckTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-inviteheil2team-1.txt");
        code = dmobj->FindStrFast(350, 35, 1279, 826, "频道", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("组队:邀请:频道:找字,成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 20.0)
        {
            api_log_MsgDebug("组队:邀请:频道:找字,失败,超时,退出");
            
            return SNAIL_ERRNO_OFFLINE;
        }

        api_log_MsgDebug("组队:邀请:频道:找字,失败");
        Delay(500);
    }

    fellowPosXYs[0].x = lposX2;
    fellowPosXYs[0].y = lposY2;
    fellowPosXYs[1].x = lposX2 - 120;
    fellowPosXYs[1].y = lposY2 + 20;

    lastChckTimeAll = time(NULL);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            posX = fellowPosXYs[i].x;
            posY = fellowPosXYs[i].y;
            
            api_log_MsgDebug("i:%d, posX:%ld, posY:%ld", i, posX, posY);

            lastChckTime = time(NULL);

            //点击队员名称,弹出队伍对话框
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                dmobj->MoveTo(posX, posY);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-inviteheil2team-1.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "队伍", "7d7d7d-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("组队:邀请:队员:队伍:找字,成功,lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                curTime = time(NULL);
                deltaTime = difftime(curTime, lastChckTime);
                if (deltaTime > 20.0)
                {
                    api_log_MsgDebug("组队:邀请:队员:队伍:找字,失败,超时,退出");
                    return SNAIL_ERRNO_OFFLINE;
                }

                api_log_MsgDebug("组队:邀请:队员:队伍:找字,失败");
                Delay(500);
            }

            //点击'队伍'邀请队友加入队伍
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
        }

        syncCount = 0;

        //@bug:稍等片刻,避免二次组队浪费时间2019/4/8 23:06:19
        Delay(2000);

        api_log_MsgDebug("同行:个数:检查");
        
        //检查'同行'个数
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            posX1 = syncPosXYs[2 * i].x;
            posY1 = syncPosXYs[2 * i].y;
            posX2 = syncPosXYs[2 * i + 1].x;
            posY2 = syncPosXYs[2 * i + 1].y;

            api_log_MsgDebug("(%ld, %ld) --> (%ld, %ld)", posX1, posY1, posX2, posY2);

            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-inviteheil2team-1.txt");
            code = dmobj->FindStrFast(posX1, posY1, posX2, posY2, "同行", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                syncCount++;

                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;

                api_log_MsgDebug("同行:检查:成功:%ld:%ld", lposX2, lposY2);
            }
        }

        if (syncCount == syncCountMax)
        {
            api_log_MsgDebug("组队:邀请:队员:所有,组队完成");
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTimeAll);
        if (deltaTime > 30.0)
        {
            api_log_MsgDebug("组队:邀请:队员:队伍:找字,失败,超时,退出");
            
            if (0 == syncCount)
            {
                return SNAIL_ERRNO_FAILED;
            }

            return SNAIL_ERRNO_AGAIN;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("组队:邀请:队员:所有,组队未完成,syncCount:%d", syncCount);
    }

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    api_log_MsgDebug("组队完成:成功:完成");
    return SNAIL_ERRNO_SUCCESS;
}


int signle_copy1(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    long copylefttimem = 0;
    CString strCopylefttime;
    int count = 0;
    CStringArray copylefttime;
    int rdcltErrorCounter = 0;
    char* ptr = NULL;
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "大型角斗场斗技场(组队)入场券发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放:查找:失败");
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "大型角斗场斗技场(组队)入场券发放(第", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放(第:找字:大范围,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放(第:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "大型角斗场斗技场(组队)入场券发放(第3次)", "00ff00-101010", 0.9, &pposX3, &pposY3);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX3 = pposX3.lVal;
            lposY3 = pposY3.lVal;

            dmobj->KeyPressChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);
            
            api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放(第3次):找字:大范围:成功:退出:副本, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
            return SNAIL_ERRNO_NOMORE;
        }

        api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放(第3次):查找:失败");
        Delay(500);

        break;
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "大型角斗场斗技场(组队)入场券发放(第", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放(第:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放(第:点击:大范围:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        
        Delay(500);
    }

    api_log_MsgDebug("临界区:进入:等待");
    signlecpyEnterCriticalSection();
    api_log_MsgDebug("临界区:进入:成功");
    
    code = cpy1_join2team(dmobj, ctxt);

    api_log_MsgDebug("临界区:退出:等待");
    signlecpyLeaveCriticalSection();
    api_log_MsgDebug("临界区:退出:成功");
    
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("副本1:组队:失败:%d", code);
        return code;
    }
    
LABEL_retalk_with_NPC_in_cpy1:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "大型角斗场斗技场(组队)入场券发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放:查找:失败");
        Delay(500);
    }

    //点第二行 53:497->53:461
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY - 36);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("npc1:对话:确认:找字:大范围,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc1:对话:确认:查找:副本地下城已满:重新对话");
            goto LABEL_retalk_with_NPC_in_cpy1;
        }
    }

    //点击确认
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

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc1:对话:确认:查找:副本地下城已满:重新对话");
            goto LABEL_retalk_with_NPC_in_cpy1;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("确认:点击:大范围:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "大型角斗场斗技场", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:大型角斗场斗技场:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:大型角斗场斗技场:查找:失败");
        Delay(1000);
    }

    pathofposxys = "265:235|274:240|278:247|278:255|272:256|265:256|256:256";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[256:256]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[256:256]:成功");

    //等待,当出现"副本进行时间"时开始行走
    for (;;)
    {
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本进行时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("副本进行时间:查找:成功,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("副本进行时间:查找:失败");
        LUAMAN_WAIT_MS(1000);
    }

    /*
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-copylefttime.txt");
        strCopylefttime = dmobj->Ocr(15, 172, 183, 194, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strCopylefttime != CString(""))
        {
            count = SplitStringWide(strCopylefttime, "分", copylefttime);
            if (count > 2)
            {
                copylefttimem = atol(copylefttime.GetAt(0));
                if (copylefttimem == 11)
                {
                    ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                    api_log_MsgDebug("副本:进行时间:不足:等待:副本:超时:%s", ptr);
                    strCopylefttime.ReleaseBuffer();
                    
                    goto beat_start;
                }

                ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                api_log_MsgDebug("副本:进行时间:%s", ptr);
                strCopylefttime.ReleaseBuffer();
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("副本:进行时间:继续:等待");
        LUAMAN_WAIT_MS(1000);
    }*/

beat_start:

    Delay_With_Terminate_Check(200, luaman_task, dmobj);

    //开始打怪
    status = beat_cpy1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS == status)
    {
        api_log_MsgError("副本1:打怪:失败:status:%d", status);
        return status;
    }

    api_log_MsgDebug("副本1:打怪:成功:继续:下一轮");
    return SNAIL_ERRNO_SUCCESS;
}

int signle_copy2(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    long copylefttimem = 0;
    CString strCopylefttime;
    int count = 0;
    CStringArray copylefttime;
    int rdcltErrorCounter = 0;
    char* ptr = NULL;

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "大型角斗场斗技场(组队)入场券发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放:查找:失败");
        Delay(500);
    }

    //53:497->53:479
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY - 18);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "大型角斗场斗技场入场券发放(第", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("大型角斗场斗技场入场券发放(第:找字:大范围,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("大型角斗场斗技场入场券发放(第:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "大型角斗场斗技场入场券发放(第3次)", "00ff00-101010", 0.9, &pposX3, &pposY3);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX3 = pposX3.lVal;
            lposY3 = pposY3.lVal;

            dmobj->KeyPressChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);
            
            api_log_MsgDebug("大型角斗场斗技场入场券发放(第3次):找字:大范围:成功:退出:副本, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
            return SNAIL_ERRNO_NOMORE;
        }

        api_log_MsgDebug("大型角斗场斗技场入场券发放(第3次):查找:失败");
        Delay(500);
        
        break;
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "大型角斗场斗技场入场券发放(第", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放(第:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放(第:点击:大范围:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        
        Delay(500);
    }

LABEL_retalk_with_NPC_in_cpy2:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "大型角斗场斗技场(组队)入场券发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("大型角斗场斗技场(组队)入场券发放:查找:失败");
        Delay(500);
    }

    //点第1行 53:497->53:443
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY - 50);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("npc1:对话:确认:找字:大范围,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc2:对话:确认:查找:副本地下城已满:重新对话");
            goto LABEL_retalk_with_NPC_in_cpy2;
        }
    }

    //点击确认
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

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc2:对话:确认:查找:副本地下城已满:重新对话");
            goto LABEL_retalk_with_NPC_in_cpy2;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("确认:点击:大范围:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "大型角斗场斗技场", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:大型角斗场斗技场:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:大型角斗场斗技场:查找:失败");
        Delay(1000);
    }

    Delay_With_Terminate_Check(130, luaman_task, dmobj);

    pathofposxys = "248:234";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[248:234]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[248:234]:成功");
    
    //等待,当出现"副本进行时间"时开始行走
    for (;;)
    {
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本进行时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("副本进行时间:查找:成功,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("副本进行时间:查找:失败");
        LUAMAN_WAIT_MS(1000);
    }
    
    /*for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-copylefttime.txt");
        strCopylefttime = dmobj->Ocr(15, 172, 183, 194, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strCopylefttime != CString(""))
        {
            count = SplitStringWide(strCopylefttime, "分", copylefttime);
            if (count > 2)
            {
                copylefttimem = atol(copylefttime.GetAt(0));
                if (copylefttimem == 11)
                {
                    ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                    api_log_MsgDebug("副本:进行时间:不足:等待:副本:超时:%s", ptr);
                    strCopylefttime.ReleaseBuffer();
                    
                    goto beat_start;
                }

                ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                api_log_MsgDebug("副本:进行时间:%s", ptr);
                strCopylefttime.ReleaseBuffer();
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("副本:进行时间:继续:等待");
        LUAMAN_WAIT_MS(1000);
    }*/

beat_start:

    Delay_With_Terminate_Check(80, luaman_task, dmobj);

    //开始打怪
    status = beat_cpy2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS == status)
    {
        api_log_MsgError("副本2:打怪:失败:status:%d", status);
        return status;
    }

    api_log_MsgDebug("副本2:打怪:成功:继续:下一轮");
    return SNAIL_ERRNO_SUCCESS;
}

int check_copyfull(Idmsoft* dmobj, void* ctxt)
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
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
        
    //检查副本中是否有大boss,如果是则会出现"因此无法入场"
    dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-reenter.txt");
    code = dmobj->FindStrFast(0, 27, 1279, 826, "因此无法入场。", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:副本中有大boss进不去, lposX:%ld, lposY:%ld", lposX, lposY);

        //查找"副本中有大boss进不去:确认"字样
        for (;;)
        {
            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-reenter.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;

                api_log_MsgDebug("副本中有大boss进不去:确认:查找:成功, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            LUAMAN_WAIT_MS(500);
            api_log_MsgDebug("副本中有大boss进不去:确认:查找:失败");
        }

        //点击"副本中有大boss进不去:确认"字样
        for (;;)
        {
            dmobj->MoveTo(lposX3, lposY3);
            LUAMAN_WAIT_MS(300);
            dmobj->LeftClick();
            LUAMAN_WAIT_MS(300);
            dmobj->MoveTo(5, 5);
            LUAMAN_WAIT_MS(300);

            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-reenter.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功");
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            LUAMAN_WAIT_MS(500);
            api_log_MsgDebug("副本中有大boss进不去:确认:点击:失败");
        }

        api_log_MsgDebug("副本中有大boss进不去:确认:点击:成功:重新对话");
        return SNAIL_ERRNO_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //没有进入副本,也没有出现"副本中有大boss进不去",则检查是否有"副本地下城满"
    dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-copyfull.txt");
    code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:副本地下城已满,lposX:%ld, lposY:%ld",
            lposX, lposY);

        for (;;)
        {
            dmobj->MoveTo(lposX + 55, lposY + 100);
            LUAMAN_WAIT_MS(500);
            dmobj->LeftClick();
            LUAMAN_WAIT_MS(500);
            dmobj->MoveTo(5, 5);
            LUAMAN_WAIT_MS(300);

            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-copyfull.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "副本地下城已满", "fff6f5-202020", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;
                
                api_log_MsgDebug("副本地下城已满:确认:点击:成功,lposX3:%ld, lposY3:%ld",
                    lposX3, lposY3);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            api_log_MsgDebug("副本地下城已满:确认:点击:失败");
            LUAMAN_WAIT_MS(500);
        }

        api_log_MsgDebug("副本地下城已满:确认:点击:成功:重新对话");
        return SNAIL_ERRNO_AGAIN;;
    }
    
    return SNAIL_ERRNO_SUCCESS;
}

int signle_copy3(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int index = 0;
    int retrytimes = 0;
    
    //按p先退出队伍
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(500);
        dmobj->KeyUpChar("p");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("退出队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("退出队伍:查找:失败");
        Delay(500);
    }

    //点击
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }

    //点击确认
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //重新组队
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "创建队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("创建队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("创建队伍:查找:失败");
        Delay(500);
    }

    //点击创建队伍
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }

    //点击确认
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //按掉p
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("退出队伍:关闭:成功");
                break;
            }
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("退出队伍:关闭:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100001.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "[遗弃的葵恩神殿]入场券发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("[遗弃的葵恩神殿]入场券发放:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("[遗弃的葵恩神殿]入场券发放:查找:失败");
        Delay(500);
    }

    //点击
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100001.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "遗弃的葵恩神殿入场券发放", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("遗弃的葵恩神殿入场券发放:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("明天再来。:查找:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //关掉明日再来
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
        
                dmobj->MoveTo(lposX2, lposY2);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("明天再来。:点击:成功");

                    return SNAIL_ERRNO_NOMORE;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("明天再来。:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            
            return SNAIL_ERRNO_NOMORE;
        }
        
        api_log_MsgDebug("遗弃的葵恩神殿入场券发放:查找:失败");
        Delay(500);
    }

    //点击
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("明天再来。:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
            return SNAIL_ERRNO_NOMORE;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100001.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "遗弃的葵恩神殿入场券发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿入场券发放:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("遗弃的葵恩神殿入场券发放:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

LABEL_retalk_with_NPC3:
    
    //进副本对话
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        LUAMAN_WAIT_MS(300);
        dmobj->KeyUpChar("n");
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-2-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "遗弃的葵恩神殿", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("遗弃的葵恩神殿:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("遗弃的葵恩神殿:查找:失败");
        LUAMAN_WAIT_MS(500);
    }
    
    //点击"遗弃的葵恩神殿"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-enter-ack.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("遗弃的葵恩神殿:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:点击:副本满:检查:重试");
            goto LABEL_retalk_with_NPC3;
        }
        
        api_log_MsgDebug("遗弃的葵恩神殿:点击:失败");
        LUAMAN_WAIT_MS(500);
    }

    //点击"确认"进入副本
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "遗弃的葵恩神殿", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)//条件成立
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:遗弃的葵恩神殿:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        //检查地下副本满
        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("遗弃的葵恩神殿:进入:副本满:检查:重试");
            goto LABEL_retalk_with_NPC3;
        }
        
        api_log_MsgDebug("遗弃的葵恩神殿:确认:点击:成功:副本:努力加载中");
        LUAMAN_WAIT_MS(500);
    }

    //等待,当出现"副本进行时间"时开始行走
    for (;;)
    {
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本进行时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("副本进行时间:查找:成功,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("副本进行时间:查找:失败");
        LUAMAN_WAIT_MS(1000);
    }
    
    pathofposxys = "108:415|117:415|128:415|140:414|151:414|154:421|157:428|167:429|176:429";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[176:429]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[176:429]:成功");

    status = beat_cpy3_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本3:阶段1:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "184:429|192:429|198:422";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[198:422]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[198:422]:成功");

Label_redialog_cpy3_phase1:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-15-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至神殿接见室", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至神殿接见室:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("移动至神殿接见室:查找:失败");
        Delay(500);
    }

    //点击"移动至神殿接见室"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-15-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至神殿接见室", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至神殿接见室:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("移动至神殿接见室:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    retrytimes = 0;
    
    //检查当前坐标,如果坐标不是254:415则表明对话失败,第一关还没有打过
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 254) <= 1 && abs(posY - 415) <= 1)
                {
                    api_log_MsgDebug("移动至神殿接见室:对话:成功");
                    break;
                }
                else if (abs(posX - 197) <= 1 && abs(posY - 421) <= 1)
                {
                    
                }
            }
        }

        retrytimes++;
        if (retrytimes > 10)
        {
            beat_cpy_timeout(dmobj, ctxt, 10, 0);
            
            api_log_MsgDebug("移动至神殿接见室:对话:坐标:获取:失败:%d:重新对话", retrytimes);
            goto Label_redialog_cpy3_phase1;
        }
        
        api_log_MsgDebug("移动至神殿接见室:对话:坐标:获取:失败:%d", retrytimes);
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = "263:415|274:417|273:419";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[273:419]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[273:419]:成功");

    Delay_With_Terminate_Check(50, luaman_task, dmobj);

    //打怪
    status = beat_cpy3_phase2(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本3:阶段2:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    //步骤16
    pathofposxys = "281:424|291:426|300:427|309:427|319:427|329:427|348:428|356:429";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[356:429]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[356:429]:成功");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-27-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至葵恩广场", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至葵恩广场:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("移动至葵恩广场:查找:失败");
        Delay(500);
    }

    //点击"移动至葵恩广场"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-27-1.txt");
        code = dmobj->FindStrFast(380, 27, 1279, 826, "移动至葵恩广场", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至葵恩广场:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("移动至葵恩广场:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 374) <= 1 && abs(posY - 428) <= 1)
                {
                    api_log_MsgDebug("移动至葵恩广场:对话:成功");
                    break;
                }
                else if (abs(posX - 357) <= 1 && abs(posY - 429) <= 1)
                {
                    
                }
            }
        }

        api_log_MsgDebug("移动至葵恩广场:对话:坐标:获取:失败");
        Delay(500);
    }

    //pathofposxys = "383:422|387:412";
    pathofposxys = "383:422|383:412";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[383:412]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[383:412]:成功");

    //打怪
    status = beat_cpy3_phase3_0(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本3:阶段3_0:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "383:412";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:归位:坐标:[383:412]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:归位:坐标:[383:412]:成功");
    
    //打怪
    status = beat_cpy3_phase3(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本3:阶段3:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    //17步
    pathofposxys = "382:397|380:390|375:387";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[375:387]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[375:387]:成功");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-34-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至下一区域", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至下一区域:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("移动至下一区域:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-35-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至污染的庭院", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("移动至污染的庭院:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("移动至污染的庭院:查找:失败");
        Delay(500);
    }
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-27-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至污染的庭院", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至污染的庭院:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("移动至污染的庭院:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 386) <= 1 && abs(posY - 312) <= 1)
                {
                    api_log_MsgDebug("移动至污染的庭院:对话:成功");
                    break;
                }
                else if (abs(posX - 375) <= 1 && abs(posY - 388) <= 1)
                {
                    
                }
            }
        }

        api_log_MsgDebug("移动至污染的庭院:对话:坐标:获取:失败");
        Delay(500);
    }

    pathofposxys = "385:303|383:291|382:280";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[382:280]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[382:280]:成功");

    Delay_With_Terminate_Check(50, luaman_task, dmobj);

    //打怪
    status = beat_cpy3_phase4(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本3:阶段4:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    //18步
    pathofposxys = "384:265|385:251|386:238|387:228|387:218|382:212";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[382:212]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[382:212]:成功");

    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);
        
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结界石", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("结界石:绿色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结界石", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("结界石:白色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结界石", "ff0000-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("结界石:红色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("结界石:查找:失败");
        Delay(500);*/
    }

    //按`键
    dmobj->KeyDown(192);
    Delay(200);
    dmobj->KeyUp(192);
    Delay(200);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    //步骤18:按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    // 858:158 -> 631:560
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 227, lposY + 402);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //检查是否到达目标坐标
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pjhg_tgtpos[pjhg_tgtpos_index].posX) <= 3 && abs(posY - pjhg_tgtpos[pjhg_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("目标:坐标:%ld:%ld:到达:成功",
                        pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("前往:目标:坐标:%ld:%ld:当前:坐标:%ld:%ld",
                    pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);*/
    }
    

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    pathofposxys = "376:389";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[376:389]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[376:389]:成功");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-84-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至下一区域", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动至下一区域:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("移动至下一区域:查找:失败");
        Delay(500);
    }

    //点击"移动至下一区域"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-85-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至破碎的回廊", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("移动至破碎的回廊:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("移动至破碎的回廊:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-85-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至破碎的回廊", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至破碎的回廊:点击:成功");
            break;
        }

        api_log_MsgDebug("移动至破碎的回廊:点击:失败");
        Delay(500);
    }

    //19步
    pathofposxys = "347:395|339:386|334:376|330:366|323:360|316:354|308:347|302:341|298:336|291:334";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[291:334]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[291:334]:成功");

    dmobj->MoveTo(5, 5);
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-94-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入破碎的回廊", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("进入破碎的回廊:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("进入破碎的回廊:查找:失败");
        Delay(500);
    }

    //点击"进入破碎的回廊"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-94-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入破碎的回廊", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("进入破碎的回廊:点击:成功");
            break;
        }

        api_log_MsgDebug("进入破碎的回廊:点击:失败");
        Delay(500);
    }

    //20步
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //TODO: 步骤20:按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    // 858:158 -> 520:358
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 338, lposY + 200);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //检查是否到达目标坐标
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pjhg_tgtpos[pjhg_tgtpos_index].posX) <= 3 && abs(posY - pjhg_tgtpos[pjhg_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("目标:坐标:%ld:%ld:到达:成功",
                        pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("前往:目标:坐标:%ld:%ld:当前:坐标:%ld:%ld",
                    pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);*/
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    pathofposxys = "177:290";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[177:290]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[177:290]:成功");

    status = beat_cpy3_phase5(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本3:阶段5:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }
    
    //22步
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //TODO: 步骤22:按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    // 858:158 -> 595:338
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 263, lposY + 180);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //检查是否到达目标坐标
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pjhg_tgtpos[pjhg_tgtpos_index].posX) <= 3 && abs(posY - pjhg_tgtpos[pjhg_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("目标:坐标:%ld:%ld:到达:成功",
                        pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("前往:目标:坐标:%ld:%ld:当前:坐标:%ld:%ld",
                    pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);*/
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "161:355";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[161:355]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[161:355]:成功");

    status = beat_cpy3_phase5(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本3:阶段5:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    //23步
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //TODO: 步骤23:按M键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    // 858:158 -> 528:338
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 330, lposY + 180);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //检查是否到达目标坐标
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pjhg_tgtpos[pjhg_tgtpos_index].posX) <= 3 && abs(posY - pjhg_tgtpos[pjhg_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("目标:坐标:%ld:%ld:到达:成功",
                        pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("前往:目标:坐标:%ld:%ld:当前:坐标:%ld:%ld",
                    pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);*/
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "158:281";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[158:281]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[158:281]:成功");
    
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);
        
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结界石", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("结界石:绿色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结界石", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("结界石:白色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结界石", "ff0000-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("结界石:红色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("结界石:查找:失败");
        Delay(500);*/
    }

    //按`键
    dmobj->KeyDown(192);
    Delay(200);
    dmobj->KeyUp(192);
    Delay(200);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //TODO:步骤23:按m键
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    // 858:158 -> 344:425
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 514, lposY + 267);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //检查是否到达目标坐标
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pjhg_tgtpos[pjhg_tgtpos_index].posX) <= 3 && abs(posY - pjhg_tgtpos[pjhg_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("目标:坐标:%ld:%ld:到达:成功",
                        pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("前往:目标:坐标:%ld:%ld:当前:坐标:%ld:%ld",
                    pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);*/
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "244:109";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[244:109]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[244:109]:成功");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-59-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入礼堂", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("进入礼堂:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("进入礼堂:查找:失败");
        Delay(500);
    }
    
    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-59-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入礼堂", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("进入礼堂:点击:成功");
            break;
        }

        api_log_MsgDebug("进入礼堂:点击:失败");
        Delay(500);
    }
    
    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    //检查当前坐标,如果坐标不是 244:151 则表明对话失败,第一关还没有打过
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 244) <= 1 && abs(posY - 151) <= 1)
                {
                    api_log_MsgDebug("进入礼堂:对话:成功");
                    break;
                }
                else if (abs(posX - 244) <= 1 && abs(posY - 113) <= 1)
                {
                    
                }
            }
        }

        api_log_MsgDebug("进入礼堂:对话:坐标:获取:失败");
        Delay(500);
    }
    
    //24步
    dmobj->KeyPressChar("9");
    Delay(500);
    dmobj->KeyUpChar("9");
    Delay(500);

    Delay_With_Terminate_Check(10, luaman_task, dmobj);
    
    for (code = 0; code < 5; code++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(200);
        dmobj->KeyUpChar("u");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        api_log_MsgDebug("西勒诺斯:u:次数:%d", i);
        
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-61-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "西勒诺斯", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("西勒诺斯:红色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-61-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "西勒诺斯", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("西勒诺斯:绿色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-61-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "西勒诺斯", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("西勒诺斯:白色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("西勒诺斯:查找:失败");
        Delay(500);*/
    }

    status = beat_cpy3_phase6(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本3:阶段6:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }
    
    //25步
    pathofposxys = "244:159|244:166|244:172";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[244:172]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[244:172]:成功");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aaa-4-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("移动奎恩的秘密书房:对话:查找:成功, lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        api_log_MsgDebug("移动奎恩的秘密书房:对话:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aaa-4-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至葵恩的秘密书房", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("移动至葵恩的秘密书房:查找:成功,lposX2:%ld, lposY2:%ld",
                lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("移动至葵恩的秘密书房:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aaa-4-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至葵恩的秘密书房", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至葵恩的秘密书房:点击:成功");
            break;
        }

        Delay(500);
        api_log_MsgDebug("移动至葵恩的秘密书房:点击:失败");
    }

    dmobj->MoveTo(5, 5);
    Delay(300);
        
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc1000.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "隐藏", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("隐藏的西勒诺斯宝物箱子:查找:成功, lposX:%ld, lposY:%d", lposX, lposY);
            break;
        }

        api_log_MsgDebug("隐藏的西勒诺斯宝物箱子:查找:失败");
        Delay(500);
    }

    //点击宝箱 573:429 ->611:467~611:502~670:466~670:499
    //573+38/97, 429+38/70
    lposX = lposX + 38 + time(NULL) % 50;
    lposY = lposY + 38 + time(NULL) % 25;

    api_log_MsgDebug("宝箱:随机:点击:位置:lposX:%ld, lposY:%ld", lposX, lposY);
    
    dmobj->MoveTo(lposX, lposY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);

    Delay_With_Terminate_Check(10, luaman_task, dmobj);
    
    //按20下空格
    for (index = 0; index < 40; ++index)
    {
        dmobj->KeyDown(32);
        Delay(50);
        dmobj->KeyUp(32);
        Delay(50);
    }
    
    dmobj->KeyPressChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    //对话,出副本
    dmobj->MoveTo(5, 5);
    Delay(300);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aaa-12-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("对话:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("对话:查找:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aaa-12-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确定离开副本", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确定离开副本:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("确定离开副本:查找:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        ///////////////////////////////////////////////////////////////////////////////////////    
        //bug:点完第一次之后,又弹出来一次 2019/12/22 21:37:22
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aaa-12-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确定离开副本", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确定离开副本:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("确定离开副本:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    api_log_MsgDebug("副本3:打怪:成功:继续:下一轮");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy4_phase1(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    api_log_MsgDebug("副本4:阶段1:打怪:开始");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa57.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "隐藏之房显现了。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("隐藏之房显现了。:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (120 * 1.0))
        {
            api_log_MsgDebug("隐藏之房显现了。:打怪:超时");
            break;
        }

        api_log_MsgDebug("隐藏之房显现了。:查找:失败");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("副本4:阶段1:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}


int signle_copy4(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int index = 0;
    int findOkFailedCount = 0;
    
    //按p先退出队伍
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(500);
        dmobj->KeyUpChar("p");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("退出队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("退出队伍:查找:失败");
        Delay(500);
    }

    //点击
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }

    //点击确认
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //重新组队
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "创建队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("创建队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("创建队伍:查找:失败");
        Delay(500);
    }

    //点击创建队伍
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }

    //点击确认
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //按掉p
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("退出队伍:关闭:成功");
                break;
            }
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("退出队伍:关闭:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //29步
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
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[卡伦的火炉]入场券发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("[卡伦的火炉]入场券发放:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("[卡伦的火炉]入场券发放:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
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
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "卡伦的火炉入场券发放", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("卡伦的火炉入场券发放:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("明天再来。:查找:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //关掉明日再来
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
        
                dmobj->MoveTo(lposX2, lposY2);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("明天再来。:点击:成功");

                    return SNAIL_ERRNO_NOMORE;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("明天再来。:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            
            return SNAIL_ERRNO_NOMORE;
        }

        api_log_MsgDebug("卡伦的火炉入场券发放:查找:失败");
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
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "卡伦的火炉入场券发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("卡伦的火炉入场券发放:点击:成功");
            break;
        }

        api_log_MsgDebug("卡伦的火炉入场券发放:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

LABEL_retalk_with_NPC_in_cpy4:
    //按n对话进入副本
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[入场]卡伦的火炉", "00ff00-000000", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:[入场]卡伦的火炉:找字:大范围,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:对话:[入场]卡伦的火炉:找字:大范围,失败");
        Delay(500);
    }

    //点击'[入场]卡伦的火炉'
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

        //检查是否出现'确认'字样的对话框
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("npc1:对话:确认:找字:大范围,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }
        
        api_log_MsgDebug("npc1:对话:确认:找字:大范围,失败");
        Delay(500);
    }

    //点击'确认'字样对话框
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "卡伦的火炉", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("npc1:对话:卡伦的火炉:进入,成功:lposX:%ld,lposY:%ld", lposX, lposY);
            break;
        }

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc1:对话:卡伦的火炉:确认:查找:副本地下城已满:重新对话");
            goto LABEL_retalk_with_NPC_in_cpy4;
        }
        
        api_log_MsgDebug("npc1:对话:卡伦的火炉,正在进入");
        Delay(500);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //增加遗漏的步骤 2023/1/15 11:06:11
    pathofposxys = "470:372|462:379|455:385";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[455:385]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[455:385]:成功");

    //@bug:检查副本进行时间,有就不用再对话4/8/2019 23:43:55
    dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-copyrunning.txt");
    code = dmobj->FindStrFast(0, 27, 1279, 826, "副本进行时间:", "fdb602-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 == code)
    {
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            //按n对话
            dmobj->KeyPressChar("n");
            Delay(500);
            dmobj->KeyUpChar("n");
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->MoveTo(5, 5);
            Delay(200);

            //查找'知道了。(开始)'字样
            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc2.txt");
            code = dmobj->FindStrFast(388, 115, 984, 667, "知道了。(开始)", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("npc2:对话:知道了。(开始):找字,成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }
            

            api_log_MsgDebug("npc2:对话:知道了。(开始):找字,失败");
            Delay(500);
        }
        

        findOkFailedCount = 0;
        
        //点击'知道了。(开始)'字样
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

            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc2.txt");
            code = dmobj->FindStrFast(388, 115, 984, 667, "知道了。(开始)", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("npc2:对话:知道了。(开始):点击,成功");
                break;
            }

            findOkFailedCount++;
            if (findOkFailedCount > 10)
            {
                api_log_MsgDebug("npc2:对话:知道了。(开始):点击:失败:超时,继续:后续流程");
                goto LABEL_find_failed_after_npc_talk;
            }

            api_log_MsgDebug("npc2:对话:好的:找字,失败");
            Delay(500);
        }
    }

LABEL_find_failed_after_npc_talk:
    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //30步
    pathofposxys = "449:383|440:383|431:383|420:383|410:382|399:382|393:382";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[393:382]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[393:382]:成功");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    code = beat_cpy_timeout(dmobj, ctxt, 10, 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "381:384|372:384|362:384|353:384|346:384|340:383|333:383|325:383|318:383|309:382";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[309:382]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[309:382]:成功");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    code = beat_cpy_timeout(dmobj, ctxt, 10, 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "306:372|304:362|304:355|303:349|302:343|299:338|294:333|292:326|291:318|292:307|293:298";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[293:298]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[293:298]:成功");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    //code = beat_cpy_timeout(dmobj, ctxt, 40, 1);
    code = beat_cpy4_phase1(dmobj, ctxt);//改为一直打直到找到标志 2023/5/17 21:46:12
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "291:288|289:279|286:271|286:263|286:254";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[286:254]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[286:254]:成功");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    code = beat_cpy_timeout(dmobj, ctxt, 20, 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "286:254|286:249|286:241|286:232|286:223|286:213|285:205|280:197|272:197|263:197";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[263:197]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[263:197]:成功");
    
    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    code = beat_cpy_timeout(dmobj, ctxt, 10, 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }
    
    pathofposxys = "261:204|258:215|256:224|251:233|246:239|241:240|235:240|228:242|222:242|215:242|212:240|202:239";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[202:239]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[202:239]:成功");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    code = beat_cpy_timeout(dmobj, ctxt, 10, 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "192:236|180:232|170:229|162:224|160:215|161:206|166:197|174:196";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[174:196]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[174:196]:成功");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    code = beat_cpy_timeout(dmobj, ctxt, 10, 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "186:193|194:189|199:183|199:175|199:167|199:159|199:152|199:145";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[199:145]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[199:145]:成功");

    //31步
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa119.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入里面。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("对话:进入里面。:找字:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("对话:进入里面。:找字:失败");
        Delay(500);
    }

    //点击'进入里面。'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa120.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "是，我要进去。", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("对话:是，我要进去。:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("对话:是，我要进去。:查找:失败");
        Delay(500);
    }

    //点击'是，我要进去。'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa120.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "是，我要进去。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("对话:是，我要进去。:点击:成功");
            break;
        }

        api_log_MsgDebug("对话:是，我要进去。:点击:失败");
        Delay(500);
    }

    pathofposxys = "199:132|199:123|199:115";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[199:115]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[199:115]:成功");

    Delay_With_Terminate_Check(15, luaman_task, dmobj);
    
    code = beat_cpy4_phase2(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    //31步
    pathofposxys = "200:99|199:92|200:89";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[200:89]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[200:89]:成功");

    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);

    Delay_With_Terminate_Check(2, luaman_task, dmobj);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa130.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "离开副本。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("对话:离开副本。:找字:成功:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("对话:离开副本。:找字:成功:失败");
        Delay(500);
    }

    //点击'进入里面。'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa130.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "离开副本。", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("对话:离开副本。:点击:成功");
            break;
        }

        api_log_MsgDebug("对话:离开副本。:点击:失败");
        Delay(500);
    }

    api_log_MsgDebug("副本4:成功:继续:下一轮");
    return SNAIL_ERRNO_SUCCESS;
}

int signle_copy5(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int index = 0;
    time_t start_tmstmp;
    time_t end_tmstmp;
    double time_delta = 0.0;
    
    //按p先退出队伍
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(500);
        dmobj->KeyUpChar("p");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("退出队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("退出队伍:查找:失败");
        Delay(500);
    }

    //点击
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }

    //点击确认
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //重新组队
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "创建队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("创建队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("创建队伍:查找:失败");
        Delay(500);
    }

    //点击创建队伍
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }

    //点击确认
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //按掉p
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("退出队伍:关闭:成功");
                break;
            }
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("退出队伍:关闭:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
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
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-talk5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "[西泽神殿:无限对战]入场", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("[西泽神殿:无限对战]入场:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        
        api_log_MsgDebug("[西泽神殿:无限对战]入场:查找:失败");
        Delay(500);
    }

    //点击
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "西泽神殿:无限对战入场", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("西泽神殿:无限对战入场:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("明天再来。:查找:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //关掉明日再来
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
        
                dmobj->MoveTo(lposX2, lposY2);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("明天再来。:点击:成功");

                    

                    return SNAIL_ERRNO_NOMORE;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("明天再来。:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            
            return SNAIL_ERRNO_NOMORE;
        }
        
        api_log_MsgDebug("西泽神殿:无限对战入场:查找:失败");
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
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "西泽神殿:无限对战入场", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("西泽神殿:无限对战入场:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("西泽神殿:无限对战入场:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

LABEL_retalk_with_NPC_in_cpy5:
    //重新对话
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
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-talk5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "[西泽神殿:无限对战]入场", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("[西泽神殿:无限对战]入场:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("[西泽神殿:无限对战]入场:查找:失败");
        Delay(500);
    }

    //点击 50:444 -> 50:426
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY - 18);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("npc1:对话:确认:找字:大范围,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("西泽神殿:无限对战入场:查找:失败");
        Delay(500);
    }

    //点击确认
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("确认:点击:副本地下城已满:重新对话");
            goto LABEL_retalk_with_NPC_in_cpy5;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("确认:点击:大范围:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    Delay_With_Terminate_Check(10, luaman_task, dmobj);
    
    pathofposxys = "260:231|254:232|249:234";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[249:234]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[249:234]:成功");

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
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-talk6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开启副本。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("开启副本。:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("开启副本。:查找:失败");
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
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开启副本。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("开启副本。:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("开启副本。:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    start_tmstmp = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开启副本。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("开启副本。:查找2:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        end_tmstmp = time(NULL);
        time_delta = difftime(end_tmstmp, start_tmstmp);
        if (time_delta >= (10 * 1.0))
        {
            api_log_MsgDebug("开启副本。:查找2:10秒:超时:跳过");
            goto Label_skip_opencpy2;
        }

        api_log_MsgDebug("开启副本。:查找2:失败");
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
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开启副本。", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("开启副本。:点击2:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("开启副本。:点击2:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

Label_skip_opencpy2:
    pathofposxys = "255:233|260:233|264:238|264:244";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[264:244]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[264:244]:成功");

    //5-1
    Delay_With_Terminate_Check(40, luaman_task, dmobj);

    status = beat_cpy5_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本5:阶段1:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 298) <= 1 && abs(posY - 277) <= 1)
                {
                    api_log_MsgDebug("副本5:阶段1:298:277:传送:成功");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("副本5:阶段1:传送:298:277:坐标:获取:失败");
        Delay(500);
    }

    pathofposxys = "292:282|286:289|281:295|274:302";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[274:302]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[274:302]:成功");
    
    //5-2
    Delay_With_Terminate_Check(60, luaman_task, dmobj);

    status = beat_cpy5_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本5:阶段1:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 226) <= 1 && abs(posY - 277) <= 1)
                {
                    api_log_MsgDebug("副本5:阶段2:传送:226:277:成功");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("副本5:阶段2:传送:226:277:坐标:获取:失败");
        Delay(500);
    }

    pathofposxys = "221:271|216:266|212:262|205:256";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[205:256]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[205:256]:成功");

    //5-3
    status = beat_cpy5_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本5:阶段3:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 230) <= 1 && abs(posY - 206) <= 1)
                {
                    api_log_MsgDebug("副本5:阶段3:传送:230:206:成功");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("副本5:阶段3:传送:230:206:坐标:获取:失败");
        Delay(500);
    }

    pathofposxys = "236:202|245:197|251:193|258:192";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[258:192]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[258:192]:成功");

    //5-4
    Delay_With_Terminate_Check(20, luaman_task, dmobj);

    status = beat_cpy5_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本5:阶段4:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 289) <= 1 && abs(posY - 205) <= 1)
                {
                    api_log_MsgDebug("副本5:阶段4:传送:289:205:成功");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("副本5:阶段3:传送:289:205:坐标:获取:失败");
        Delay(500);
    }

    pathofposxys = "296:213|303:219";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[303:219]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[303:219]:成功");

    //5-5
    status = beat_cpy5_phase2(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("副本5:阶段5:打怪:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    //TODO:时间小于10秒时按0
    Delay_With_Terminate_Check(20, luaman_task, dmobj);
    
    //等待传出去
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->KeyDownChar("0");
        Delay(200);
        dmobj->KeyUpChar("0");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "米特拉", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:米特拉:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Delay(1000);
        api_log_MsgDebug("地图:米特拉:查找:失败");
    }

    pathofposxys = "460:460";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[460:460]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[460:460]:成功");

    api_log_MsgDebug("副本5:成功:继续:下一轮");
    return SNAIL_ERRNO_SUCCESS;
}

static int singlecpy1_Fighttimeout(Idmsoft* dmobj, void* ctxt, int waittime, int fighttime)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    api_log_MsgDebug("打怪:等待:%d秒:开始", waittime);
    
    if (waittime > 0)
    {
        Delay_With_Terminate_Check(waittime, luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
    }

    api_log_MsgDebug("打怪:等待:%d秒:结束", waittime);

    if (fighttime <= 0)
    {
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("打怪:%d秒:跳过", fighttime);
        return SNAIL_ERRNO_SUCCESS;
    }
    
    api_log_MsgDebug("打怪:%d秒:开始", fighttime);
    
    lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("u");
        Delay(20);
        dmobj->KeyUpChar("u");
        Delay(20);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(15);
            dmobj->KeyUpChar("3");
            Delay(15);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > (fighttime * 1.0))
        {
            api_log_MsgDebug("打怪:%d秒:完成", fighttime);
            break;
        }

        api_log_MsgDebug("打怪:%d秒:继续", fighttime);
    }

    for (idx = 0; idx < 60; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    check_alarm(dmobj, ctxt);
    
    api_log_MsgDebug("打怪:%d秒:结束", fighttime);
    return SNAIL_ERRNO_SUCCESS;
}


static int beat_cpy6_phase1(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy1_fight_atpoint_1();

    api_log_MsgDebug("点1:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点1::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy6_phase2(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_cove.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "请前往洞穴的深", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("请前往洞穴的深:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (120 * 1.0))
        {
            api_log_MsgDebug("请前往洞穴的深:打怪:超时");
            break;
        }
        
        api_log_MsgDebug("请前往洞穴的深:查找:失败");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("副本6:阶段2:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy6_phase3(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc8.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "拉比林托斯", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("拉比林托斯:红色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc8.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "拉比林托斯", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("拉比林托斯:白色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc8.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "拉比林托斯", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("拉比林托斯:绿色:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("拉比林托斯:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        //找左上角的等待时间
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-TiShi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结束时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("副本6:阶段3:等待时间:读取:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("副本6:阶段3:等待时间:读取:失败:继续:打怪");
        Delay(500);
    }

    for (idx = 0; idx < 60; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("副本6:阶段3:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

int signle_copy6(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int index = 0;
    time_t start_tmstmp;
    time_t end_tmstmp;
    double time_delta = 0.0;
    int deltaX = deltaatpos_single_cpy6[0].x;
    int deltaY = deltaatpos_single_cpy6[0].y;

    if (tasknr)
    {
        goto Label_next_cpy_task;
    }
    
    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);
    
    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    pathofposxys = "347:123|346:116";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[346:116]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[346:116]:成功");

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "想传送", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("想传送:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("想传送:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "荒凉谷地", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("荒凉谷地:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("荒凉谷地:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "荒凉谷地", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("荒凉谷地:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("荒凉谷地:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //等待地图
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "荒凉谷地", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:荒凉谷地:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:荒凉谷地:查找:失败");
        Delay(1000);
    }

    pathofposxys = "102:118|103:125|103:132|102:139|101:147|99:153|96:158|92:160";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[92:160]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[92:160]:成功");

    //////////////////////////////////////////////////////////////////////////////////////////////
    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动地区", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("移动地区:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("移动地区:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "艾伦恩卫城", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("艾伦恩卫城:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("艾伦恩卫城:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "艾伦恩卫城", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("艾伦恩卫城:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("艾伦恩卫城:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //等待地图
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "艾伦恩卫城", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:艾伦恩卫城:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:艾伦恩卫城:查找:失败");
        Delay(1000);
    }

    pathofposxys = "474:43|477:54|478:65|478:78|478:87|477:97|478:108|480:117|482:126|483:134";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[483:134]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[483:134]:成功");

    //////////////////////////////////////////////////////////////////////////////////////////////
    dmobj->MoveTo(5, 5);
    Delay(200);

    //等待10秒
    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos_single_cpy6[0].x;
    deltaY = deltaatpos_single_cpy6[0].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

Label_next_cpy_task:
    pathofposxys = (0 == tasknr)? "315:379|321:383|327:394|335:385|341:382|346:379|350:375|355:373|360:372|365:372" :
        "327:394|335:385|341:382|346:379|350:375|355:373|360:372|365:372";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);
    //////////////////////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "入场卷发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("入场卷发放:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("入场卷发放:查找:失败");
        Delay(500);
    }

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    
    //点击
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "入场卷发放", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("入场卷发放:点击:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("明天再来。:查找:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //关掉明日再来
            for (;;)
            {
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
                dmobj->MoveTo(lposX2, lposY2);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("明天再来。:点击:成功");
                    return SNAIL_ERRNO_NOMORE;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("明天再来。:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            
            return SNAIL_ERRNO_NOMORE;
        }
        
        api_log_MsgDebug("入场卷发放:点击::失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "入场卷发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("入场卷发放:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("入场卷发放:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    dmobj->MoveTo(5, 5);
    Delay(200);

Label_NPC_next_retry:
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "入场卷发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("入场卷发放:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("入场卷发放:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos_single_cpy6[1].x;
    deltaY = deltaatpos_single_cpy6[1].y;
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("确认:查找:副本满:检查:重试");
            goto Label_NPC_next_retry;
        }
        
        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }

    //点击确认
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("确认:点击:副本满:检查:重试");
            goto Label_NPC_next_retry;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //等待固定时间
    Delay_With_Terminate_Check_Alram(single_cpy_wait_seconds, luaman_task, dmobj);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    //按M，找贵宾服务的相对位置2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }
    
    deltaX = deltaatpos_single_cpy6[2].x;
    deltaY = deltaatpos_single_cpy6[2].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    //////////////////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "尝试轻微触碰魔", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("尝试轻微触碰魔:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("尝试轻微触碰魔:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc6.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "尝试轻微触碰魔", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("尝试轻微触碰魔:点击:成功");
            break;
        }
        
        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("尝试轻微触碰魔:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    //打怪
    code = beat_cpy6_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点1:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点1:成功");

    code = beat_cpy6_phase2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点2:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点2:成功");

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    //按M，找贵宾服务的相对位置2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }
    
    deltaX = deltaatpos_single_cpy6[3].x;
    deltaY = deltaatpos_single_cpy6[3].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    dmobj->MoveTo(5, 5);
    Delay(200);
        
    //////////////////////////////////////////////////////////////////////////////////////
    //按N对话，点击进入洞穴（）
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc7.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入洞穴", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("进入洞穴:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("进入洞穴:查找:失败");
        Delay(500);
    }
    
    //点击
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc7.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入洞穴", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("进入洞穴:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("进入洞穴:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //检查当前坐标,如果坐标不是109:246则表明对话失败,第一关还没有打过
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 109) <= 2 && abs(posY - 246) <= 2)
                {
                    api_log_MsgDebug("坐标:109:246:到达:成功");
                    break;
                }
            }
        }

        api_log_MsgDebug("坐标:109:246:到达:失败");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = "97:244";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[97:244]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[97:244]:成功");

    //打怪
    code = beat_cpy6_phase3(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        return code;
    }

    api_log_MsgDebug("等待:15秒:开始");
    
    Delay_With_Terminate_Check_Alram(15, luaman_task, dmobj);
    
    api_log_MsgDebug("等待:15秒:结束");

    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);
        
    //等待地图
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "艾伦恩卫城", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:艾伦恩卫城:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:艾伦恩卫城:查找:失败");
        Delay(1000);
    }

    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase1(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_1();

    api_log_MsgDebug("点1:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点1::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase2(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_2();

    api_log_MsgDebug("点2:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点2::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase3(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_3();

    api_log_MsgDebug("点3:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点3::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase4(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_4();

    api_log_MsgDebug("点4:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点4::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase5(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_5();

    api_log_MsgDebug("点5:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点5::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase6(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_6();

    api_log_MsgDebug("点6:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点6::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase7(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_7();

    api_log_MsgDebug("点7:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点7::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase8(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_8();

    api_log_MsgDebug("点8:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点8::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase9(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_9();

    api_log_MsgDebug("点9:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点9::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase10(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_10();

    api_log_MsgDebug("点10:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点10::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase11(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_11();

    api_log_MsgDebug("点11:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("点11::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase12(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    const char* fontcolors[] = {"ffffff-101010", "00ff00-101010", "ff3300-101010"};
    const char* fontcolor = fontcolors[0];
    int flag = 0;
    
    api_log_MsgDebug("点12::打怪:开始");

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "冰封心脏的", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[0];
            
            api_log_MsgDebug("冰封心脏的:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "冰封心脏的", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[1];
            
            api_log_MsgDebug("冰封心脏的:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "冰封心脏的", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[2];
            
            api_log_MsgDebug("冰封心脏的:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("冰封心脏的:查找:失败");
        Delay(500);
    }

    flag = 0;
    
    //开始打怪
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        for (idx = 0; idx < 50; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(15);
            dmobj->KeyUpChar("3");
            Delay(15);
        }

        if (0 == flag)
        {
            code = dmobj->FindPic(535, 49, 768, 160, BMPSIGNLECPY1_DIR"hiborun1.bmp", "101010", 0.9, 0, &pposX, &pposY);
            if (-1 == code)
            {
                dmobj->KeyDown(192);
                Delay(200);
                dmobj->KeyUp(192);
                Delay(200);
                
                dmobj->KeyDownChar("~");
                Delay(200);
                dmobj->KeyUpChar("~");
                Delay(200);

                Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

                dmobj->KeyDownChar("w");
                Delay(200);
                dmobj->KeyUpChar("w");
                Delay(200);

                flag = 1;
            }
        }
        
        if (flag)
        {
            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-TiShi2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "消灭时间", "fdb602-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("冰封心脏的:消灭时间:红色:打怪:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("冰封心脏的:消灭时间:查找:打怪:继续");
            Delay(500);
        }
    }

    for (idx = 0; idx < 60; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }
    
    api_log_MsgDebug("点12::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase13(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    const char* fontcolors[] = {"ffffff-101010", "00ff00-101010", "ff3300-101010"};
    const char* fontcolor = fontcolors[0];
    int flag = 0;
    
    api_log_MsgDebug("点13::打怪:开始");

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "堕落的维纳", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[0];
            
            api_log_MsgDebug("堕落的维纳:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "堕落的维纳", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[1];
            
            api_log_MsgDebug("堕落的维纳:绿色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "堕落的维纳", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[2];
            
            api_log_MsgDebug("堕落的维纳:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("堕落的维纳:查找:失败");
        Delay(500);
    }

    //开始打怪
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(15);
            dmobj->KeyUpChar("3");
            Delay(15);
        }

        //找左上角的等待时间
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-TiShi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结束时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("副本7:阶段13:等待时间:读取:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("副本7:阶段13:等待时间:读取:失败:继续:打怪");
        Delay(500);
    }

    for (idx = 0; idx < 60; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }
    
    api_log_MsgDebug("点13::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

int signle_copy7(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int index = 0;
    time_t start_tmstmp;
    time_t end_tmstmp;
    double time_delta = 0.0;
    char* randtime = NULL;
    int deltaX = deltaatpos_single_cpy7[0].x;
    int deltaY = deltaatpos_single_cpy7[0].y;
    int retrytimes = 0;
    int flags = 0;
    int reclick = 1;

    if (tasknr)
    {
        goto Label_next_cpy7_task;
    }
    
    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);
    
    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    pathofposxys = "347:123|346:116";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[346:116]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[346:116]:成功");

    dmobj->MoveTo(5, 5);
    Delay(200);

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "想传送", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("想传送:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("想传送:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "冰湖山地", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("冰湖山地:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("冰湖山地:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "荒凉谷地", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("荒凉谷地:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("荒凉谷地:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 130) <= 1 && abs(posY - 304) <= 1)
                {
                    api_log_MsgDebug("副本7:传送:130:304:成功");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("副本7:传送:130:304:坐标:获取:失败");
        Delay(500);
    }

    pathofposxys = "126:297|123:289|122:284|125:278|131:270|139:265|143:259|143:251|141:245";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[141:245]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[141:245]:成功");

    reclick = 1;
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[DAS]移动到DK服务器", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("[DAS]移动到DK服务器:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("[DAS]移动到DK服务器:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        if (reclick)
        {
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
        }
        
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DiTUMingZhi2.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "冰湖山地的深渊", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("地图:冰湖山地的深渊:进入:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }
        
        randtime = ((time(NULL)) % 2)? "DK服务器1" : "DK服务器2";
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, randtime, "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("%s:点击:成功", randtime);

            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk3.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "[DAS]移动到DK服务器", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                reclick = 1;
                
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("[DAS]移动到DK服务器:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            else
            {
                reclick = 0;
                api_log_MsgDebug("[DAS]移动到DK服务器:查找:失败");
            }
            
            continue;
        }

        reclick = 1;
        
        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        lposX = lposX2;
        lposY = lposY2;
        
        api_log_MsgDebug("%s:点击:失败, lposX2:%ld, lposY2:%ld", randtime, lposX2, lposY2);
        
        Delay(500);
    }
    
    //重新组队
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "创建队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("创建队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("创建队伍:查找:失败");
        Delay(500);
    }

    //点击创建队伍
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }

    //点击确认
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //按掉p
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("退出队伍:关闭:成功");
                break;
            }
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("退出队伍:关闭:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);
    
    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos_single_cpy7[0].x;
    deltaY = deltaatpos_single_cpy7[0].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    pathofposxys = "291:284|293:270|294:262|294:255";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[294:255]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[294:255]:成功");

Label_next_cpy7_task:
    //按p先退出队伍
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(500);
        dmobj->KeyUpChar("p");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("退出队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("退出队伍:查找:失败");
        Delay(500);
    }

    //点击
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }

    //点击确认
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //重新组队
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "创建队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("创建队伍:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("创建队伍:查找:失败");
        Delay(500);
    }

    //点击创建队伍
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }

    //点击确认
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("确认:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //按掉p
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("退出队伍:关闭:成功");
                break;
            }
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("退出队伍:关闭:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "难]入场卷发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("难]入场卷发放:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("难]入场卷发放:查找:失败");
        Delay(500);
    }

    retrytimes = 0;
    flags = 1;
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        if (flags)
        {
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(200);
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "难]入场卷发放", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX2.lVal;
            lposY = pposY2.lVal;
            flags = 1;
            
            api_log_MsgDebug("难]入场卷发放:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
            continue;
        }

        flags = 0;
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("明天再来。:查找:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //关掉明日再来
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
        
                dmobj->MoveTo(lposX2, lposY2);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来。", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("明天再来。:点击:成功");

                    return SNAIL_ERRNO_NOMORE;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("明天再来。:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            
            return SNAIL_ERRNO_NOMORE;
        }
        
        api_log_MsgDebug("难]入场卷发放:点击:查找:再试:%d", retrytimes);
        Delay(500);

        retrytimes++;
        if (retrytimes > 2)
        {
            api_log_MsgDebug("难]入场卷发放:点击:成功");
            break;
        }
    }
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua3.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "片(困难)", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("片(困难):查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("片(困难):查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("确认:查找:失败");
        Delay(500);
    }
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("确认:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    /*for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 343) <= 1 && abs(posY - 303) <= 1)
                {
                    api_log_MsgDebug("副本7:传送:343:303:成功");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("副本7:传送:343:303:坐标:获取:失败");
        Delay(500);
    }*/
    //等待地图
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DiTUMingZhi3.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "卡伦的碎片", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:卡伦的碎片:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:卡伦的碎片:查找:失败");
        Delay(1000);
    }

    dmobj->MiddleClick();
    LUAMAN_WAIT_MS(200);
    dmobj->MiddleClick();
    LUAMAN_WAIT_MS(200);
        
    Delay_With_Terminate_Check_Alram(10, luaman_task, dmobj);
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }
    
    deltaX = deltaatpos_single_cpy7[1].x;
    deltaY = deltaatpos_single_cpy7[1].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    //打怪
    code = beat_cpy7_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点1:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点1:成功");
    
    //找黄色的入侵警报器。找到了点击黄色的入侵警报器(font-DuiHua4.txt)的相对位置
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "入侵警报器", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("入侵警报器:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("入侵警报器:查找:失败");
        Delay(500);
    }

    //点击入侵警报器(font-DuiHua4.txt)的相对位置
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + single_cpy7_ips_offset[0].x, lposY + single_cpy7_ips_offset[0].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "入侵警报器", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("入侵警报器:点击:成功");
            break;
        }

        lposX = pposX2.lVal;
        lposY = pposY2.lVal;
    
        api_log_MsgDebug("入侵警报器:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    code = beat_cpy7_phase2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点2:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点2:成功");

    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos_single_cpy7[2].x;
    deltaY = deltaatpos_single_cpy7[2].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    code = beat_cpy7_phase3(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点3:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点3:成功");
    
    //找黄色的入侵警报器。找到了点击黄色的入侵警报器(font-DuiHua4.txt)的相对位置
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "入侵警报器", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("入侵警报器:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("入侵警报器:查找:失败");
        Delay(500);
    }

    //点击入侵警报器(font-DuiHua4.txt)的相对位置
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + single_cpy7_ips_offset[1].x, lposY + single_cpy7_ips_offset[1].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "入侵警报器", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("入侵警报器:点击:成功");
            break;
        }

        lposX = pposX2.lVal;
        lposY = pposY2.lVal;
    
        api_log_MsgDebug("入侵警报器:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    code = beat_cpy7_phase4(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点4:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点4:成功");
    
    //////////////////////////////////////////////////////////////////////////////
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos_single_cpy7[3].x;
    deltaY = deltaatpos_single_cpy7[3].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    code = beat_cpy7_phase5(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点5:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点5:成功");
    
    //找黄色的入侵警报器。找到了点击黄色的入侵警报器(font-DuiHua4.txt)的相对位置
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "入侵警报器", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("入侵警报器:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("入侵警报器:查找:失败");
        Delay(500);
    }

    //点击入侵警报器(font-DuiHua4.txt)的相对位置
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + single_cpy7_ips_offset[2].x, lposY + single_cpy7_ips_offset[2].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "入侵警报器", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("入侵警报器:点击:成功");
            break;
        }

        lposX = pposX2.lVal;
        lposY = pposY2.lVal;
    
        api_log_MsgDebug("入侵警报器:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    code = beat_cpy7_phase6(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点6:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点6:成功");
    
    //////////////////////////////////////////////////////////////////////////////
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos_single_cpy7[4].x;
    deltaY = deltaatpos_single_cpy7[4].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    code = beat_cpy7_phase7(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点7:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点7:成功");

    pathofposxys = "221:303";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[221:303]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[221:303]:成功");
    
    //找黄色的入侵警报器。找到了点击黄色的入侵警报器(font-DuiHua4.txt)的相对位置
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "入侵警报器", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("入侵警报器:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("入侵警报器:查找:失败");
        Delay(500);
    }

    //点击入侵警报器(font-DuiHua4.txt)的相对位置
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + single_cpy7_ips_offset[3].x, lposY + single_cpy7_ips_offset[3].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "入侵警报器", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("入侵警报器:点击:成功");
            break;
        }

        lposX = pposX2.lVal;
        lposY = pposY2.lVal;
    
        api_log_MsgDebug("入侵警报器:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    code = beat_cpy7_phase8(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点8:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点8:成功");

    ////////////////////////////////////////////////////////////////////////////////////////
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos_single_cpy7[5].x;
    deltaY = deltaatpos_single_cpy7[5].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    code = beat_cpy7_phase9(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点9:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点9:成功");

    pathofposxys = "183:248";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[183:248]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }
    
    code = beat_cpy7_phase10(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点10:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点10:成功");

    pathofposxys = "182:256|183:264";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[183:264]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    code = beat_cpy7_phase11(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点11:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点11:成功");
    
    ////////////////////////////////////////////////////////////////////////////////////
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos_single_cpy7[6].x;
    deltaY = deltaatpos_single_cpy7[6].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    //找黄色的结界石
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结界石", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("结界石:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("结界石:查找:失败");
        Delay(500);
    }

    //点击入侵警报器(font-DuiHua4.txt)的相对位置
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + single_cpy7_jjs_offset[0].x, lposY + single_cpy7_jjs_offset[0].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "结界石", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("结界石:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("结界石:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos_single_cpy7[7].x;
    deltaY = deltaatpos_single_cpy7[7].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //打怪
    code = beat_cpy7_phase12(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点12:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点12:成功");

    ////////////////////////////////////////////////////////////////////////////////////////////
    //按一下减号键，等三秒
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //按M，找贵宾服务的相对位置2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos_single_cpy7[8].x;
    deltaY = deltaatpos_single_cpy7[8].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //打怪
    code = beat_cpy7_phase13(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点13:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("打怪:点13:成功");
    
    ///////////////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("对话:找字:成功:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("对话:找字:成功:失败");
        Delay(500);
    }

    //点击'进入里面。'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出副本", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("退出副本:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("退出副本:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出副本", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("退出副本:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("退出副本:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //等待坐标变成292:269
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 292) <= 1 && abs(posY - 269) <= 1)
                {
                    api_log_MsgDebug("副本7:退出:292:269:成功");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("副本7:退出:292:269:坐标:获取:失败");
        Delay(500);
    }

    pathofposxys = "294:255";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[294:255]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[294:255]:成功");
    
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy_foolish_phase1(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = signlecpy_foolish_fight_atpoint_1();
    char bmpFileName[255] = {0};
    SYSTEMTIME curTimeStamp;
    
    api_log_MsgDebug("点1:打怪:开始");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    GetLocalTime(&curTimeStamp);
    _snprintf(bmpFileName, sizeof(bmpFileName) - 1, "cap\\single\\%04d%02d%02d-%02d%02d%02d.bmp",
        curTimeStamp.wYear, curTimeStamp.wMonth, curTimeStamp.wDay,
        curTimeStamp.wHour, curTimeStamp.wMinute, curTimeStamp.wSecond);
    dmobj->Capture(0, 27, 1279, 826, bmpFileName);
    
    api_log_MsgDebug("点1::打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy_foolish_phase2(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    char bmpFileName[255] = {0};
    SYSTEMTIME curTimeStamp;
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);
        
        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        //找左上角的等待时间
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-foolish2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "消灭等待", signlecpy_foolish_diewait_font_color, 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("阶段2:消灭等待:读取:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("阶段2:消灭等待:读取:失败:继续:打怪");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    GetLocalTime(&curTimeStamp);
    _snprintf(bmpFileName, sizeof(bmpFileName) - 1, "cap\\single\\%04d%02d%02d-%02d%02d%02d.bmp",
        curTimeStamp.wYear, curTimeStamp.wMonth, curTimeStamp.wDay,
        curTimeStamp.wHour, curTimeStamp.wMinute, curTimeStamp.wSecond);
    dmobj->Capture(0, 27, 1279, 826, bmpFileName);
    
    api_log_MsgDebug("阶段2:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}


int signle_copy_foolish(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int index = 0;
    time_t start_tmstmp;
    time_t end_tmstmp;
    double time_delta = 0.0;
    int deltaX = 0;
    int deltaY = 0;
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);
    
    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    //按M，找贵宾服务的相对位置2
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }
    for_timeout_check_end();
    
    deltaX = deltaatpos_single_cpy_foolish[0].x;
    deltaY = deltaatpos_single_cpy_foolish[0].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "入场卷发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("入场卷发放:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("入场卷发放:查找:失败");
        Delay(500);
    }
    for_timeout_check_end();
    
    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    
    //点击
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "入场卷发放", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("入场卷发放:点击:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("明天再来:查找:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //关掉明日再来
            for_timeout_check_start(luaman_task)
            {
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
                dmobj->MoveTo(lposX2, lposY2);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("明天再来:点击:成功");
                    return SNAIL_ERRNO_NOMORE;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;

                for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
                
                api_log_MsgDebug("明天再来:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            for_timeout_check_end();
            
            return SNAIL_ERRNO_NOMORE;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("入场卷发放:点击:s失败");
        Delay(500);
    }
    for_timeout_check_end();
    
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "入场卷发放", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("入场卷发放:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("入场卷发放:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    for_timeout_check_end();
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    dmobj->MoveTo(5, 5);
    Delay(200);

Label_next_retry:
    
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-foolish1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "?????", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("?????:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("?????:查找:失败");
        Delay(500);
    }
    for_timeout_check_end();
    
    //点击 50:444 -> 50:426
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("对话:确认:找字:大范围,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("?????:查找:失败");
        Delay(500);
    }
    for_timeout_check_end();
    
    //点击确认
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("确认:点击:副本地下城已满:重新对话");
            goto Label_next_retry;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("确认:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("确认:点击:大范围:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    for_timeout_check_end();

    //等待坐标
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 58) <= 2 && abs(posY - 70) <= 2)
                {
                    api_log_MsgDebug("58:70:达到:成功");
                    break;
                }
            }
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("58:70:坐标:获取:失败");
        Delay(500);
    }
    for_timeout_check_end();

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);
    
    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    //按M，找贵宾服务的相对位置2
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }
    for_timeout_check_end();
    
    deltaX = deltaatpos_single_cpy_foolish[1].x;
    deltaY = deltaatpos_single_cpy_foolish[1].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

    //开始打怪
    code = beat_cpy_foolish_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点1:失败, code:%d", code);
        return code;
    }
    //////////////////////////////////////////////////////////////////////////

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);
    
    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    //按M，找贵宾服务的相对位置2
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "贵宾服务", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("贵宾服务:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("贵宾服务:查找:失败");
        Delay(500);
    }
    for_timeout_check_end();
    
    deltaX = deltaatpos_single_cpy_foolish[2].x;
    deltaY = deltaatpos_single_cpy_foolish[2].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

    //打怪
    code = beat_cpy_foolish_phase2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("打怪:点2:失败, code:%d", code);
        return code;
    }

    api_log_MsgDebug("愚人节:副本:完成");
    return SNAIL_ERRNO_NOMORE;
}

static void callend(){}
