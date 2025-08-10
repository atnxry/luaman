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
#include "taskEx175.h"

#define CONFIG_EX175_XML  "conf\\ex175.xml"
#define FONTEX175_DIR "font\\ex175\\"
#define BMPEX175_DIR "bmp\\ex175\\"

static struct luaman_posxy deltaatpos175[] = 
{
    {0,-15}, //4 按N，点击绿色的圣魔之爪.(找入场卷发放的相对位置，因为有两个绿色的圣魔之爪）
};

//结束条件是否找字,超时时间(单位:秒),是否归位,打怪结束字样,字库,颜色偏移值
//找字结束:
//  {FONT_FIND, 20, ADJUST_POS, {"门已开启",     FONT_EXPIRATE_DIR"font-xxx.txt", "ffffff-101010"}}
//如果结束条件不找字,则第4~最后字段不用填写,如:
//  {NO_FONT_FIND, 20, ADJUST_POS}
//一共有7个
static struct font_info_of_fight_t fif175_info[] =
{
    {FONT_FIND,    30, ADJUST_POS,   {"清除所有警备兵",     FONTEX175_DIR"font-175tishi-1.txt", "ffffff-101010"}}, //打怪（1）
    {FONT_FIND,    120, ADJUST_POS,   {"我的朋友",     FONTEX175_DIR"font-175tishi-2.txt", "00ff00-101010"}}, //打怪2
    {NO_FONT_FIND, 25, ADJUST_POS,  },
    //{NO_FONT_FIND, 5, ADJUST_POS,  },
    {NO_FONT_FIND, 25, ADJUST_POS,  },
    {NO_FONT_FIND, 20, ADJUST_POS,  },
    {FONT_FIND,    220, ADJUST_POS,   {"我看见刀",     FONTEX175_DIR"font-175tishi-3.txt", "00ff00-101010"}},
};

//寻路坐标,共12个寻路坐标
static char* walk175_path[] =
{
    "466:239|461:239|456:239", //寻路（1）
    "450:239|444:238|437:237|432:237|426:237|420:238|415:238|411:238",
    "405:239|401:244|398:249|392:252|386:250|383:245|379:239|370:239|361:239|352:239|343:239|334:239|327:239|321:239",
    "315:239|309:239|302:243|296:245|289:244|283:240|275:237",
    //TODO:再当前行的下面加一行
    "271:237|264:237|257:237|250:237|244:237|238:238",
    "229:237|223:233",
    "223:233|218:225|214:218|213:212|213:205|212:198|212:191|212:184|212:175|212:168|213:162|213:158",
    "213:150|214:141|221:138|223:133",
    "216:137|215:146|213:153|213:159|213:167|213:176|213:184|213:193|213:201|213:208|213:215|213:222|213:231|213:242|213:250|212:257|212:264|212:272|212:279|212:284|212:290|213:296",
    "213:300|215:307|216:325|223:326",
    "215:316|213:309|213:302|213:295|213:287|213:279|213:272|212:264|212:257|209:252|205:245|201:239|195:236",
    "191:239|184:236|177:235|169:236|160:236|154:235",
    "142:237|136:236|129:239|125:241|118:238|112:236|106:234|100:234|95:235|88:235|82:235|75:234|69:234|63:234|57:234",
};

struct deltaval deltaatpos175_u[] =
{
    {24, 82}, //7 寻路7，找到黄色的 气体操纵杆。点击气体操纵杆的相对位置1.
    {24, 82}, //9 寻路9，找到黄色的 气体操纵杆。点击气体操纵杆的相对位置2.
};


static int ex175Checkkeystate(Idmsoft* dmobj, void* ctxt)
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

static int ex175Checklife(Idmsoft* dmobj, void* ctxt, int recusive)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int blueThreshold = luaman_config->blueThreshold;
    int maxBlueFiled = luaman_config->maxBlueFiled;
    int minBlue = luaman_config->minBlue;
    int code = 0;
    int bCopyoutPre = 1;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    CString strBBlood;
    CStringArray bBlood;
    char* cstr = NULL;
    int sz = 0;
    long bBloodAva = 0;
    long bBloodTot = 0;
    int strBBloodLen = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    
    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    dmobj->SetDict(0, FONTEX175_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("蓝血条:读取:异常, groupid:%d, hwnd:%ld", luaman_config->groupid, hwnd);
        return SNAIL_ERRNO_INGORE;
    }

    cstr = strBBlood.GetBuffer(BUFFER_MAX);
    strBBlood.ReleaseBuffer();

    sz = SplitString(strBBlood, '/', bBlood);
    if (2 == sz)
    {
        bBloodAva = atol(bBlood.GetAt(0));
        bBloodTot = atol(bBlood.GetAt(1));

        //api_log_MsgDebug("血条:解析,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
        //    hwnd, bBloodAva, bBloodTot);
    }
    else
    {
        strBBloodLen = strBBlood.GetLength();
        bBloodAva = atol(strBBlood.Mid(1, strBBloodLen - maxBlueFiled));
        bBloodTot = atol(strBBlood.Mid(strBBloodLen - maxBlueFiled + 1, strBBloodLen));

        //api_log_MsgDebug("血条:解析:成功,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
        //    hwnd, bBloodAva, bBloodTot);
    }

    if (bBloodAva > blueThreshold)
    {
        return SNAIL_ERRNO_SUCCESS;
    }

    if (bBloodAva > 0)
    {
        dmobj->KeyDownChar("f3");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f3");
        LUAMAN_WAIT_MS(200);

        return SNAIL_ERRNO_SUCCESS;
    }

    api_log_MsgDebug("血条:检查:死亡,等待出现接受字样");

    ex175Checkkeystate(dmobj, ctxt);

    if (recusive)
    {
        return SNAIL_ERRNO_AGAIN;
    }
    
    //等待出现死亡字样
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTEX175_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("死亡:复活:接受:找字:成功,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("血条:检查:死亡:接受:找字:失败");
        Delay(500);
    }
    
    //点击接受
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTEX175_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "接受", "929292-000000", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("血条:检查:死亡:接受:点击,成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("血条:检查:死亡:接受:点击,失败,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //等待复活站起来
    for (;;)
    {
        code = ex175Checklife(dmobj, ctxt, 1);
        if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("复活:站立:完成");
            break;
        }

        Delay_With_Terminate_Check(1, luaman_task, dmobj);
    }

    code = common_exitgroup(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("退组:失败:code:%d", code);
    }
    
    return SNAIL_ERRNO_REALIVE;
}


static int ex175Walk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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
            if (walkTimeDelta > 300.0)
            {
                (void)ex175Checkkeystate(dmobj, ctxt);

                api_log_MsgDebug("寻路:坐标:卡住,停止寻路:等待副本消失,下一坐标, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //等待地洞副本消失
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //先检查下血条
            code = ex175Checklife(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //读取当前坐标
            dmobj->SetDict(0, FONTEX175_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)ex175Checkkeystate(dmobj, ctxt);
                
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

                (void)ex175Checkkeystate(dmobj, ctxt);
                
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
        code = ex175Checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:卡住:血条:检查:死亡:复活");
            return code;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-map.txt");
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

static int ex175FightTimeout(Idmsoft* dmobj, void* ctxt, int withu, struct font_info_of_fight_t* fif)
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
    time_t lastAdjposTime = time(NULL);
    CString pathOfPosXYs;
    int findfont = fif->findfont;
    int timeout = fif->timeout;
    int adjust_pos = fif->adjust_pos;
    struct font_info_t* fontinfo = &fif->font;
    
    api_log_MsgDebug("打怪:开始, adjust_pos:%d", adjust_pos);

    if (adjust_pos)
    {
        code = GetCurPosXY(dmobj, ctxt, NULL, NULL, &pathOfPosXYs);
        if (ERRNO_SUCCESS != code)
        {
            adjust_pos = 0;
            api_log_MsgDebug("GetCurPosXY failed, adjust_pos:%d, code:%d", adjust_pos, code);
        }
    }

    lastAdjposTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        code = ex175Checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:血条:检查,死亡并复活");
            return code;
        }
        
        if (withu)
        {
            dmobj->KeyDownChar("u");
            Delay(50);
            dmobj->KeyUpChar("u");
            Delay(50);
        }
        
        for (idx = 0; idx < 8; ++idx)
        {
            dmobj->KeyDown(48 + idx + 1);
            Delay(50);
            dmobj->KeyUp(48 + idx + 1);
            Delay(50);
        }

        if (findfont)
        {
            dmobj->SetDict(0, fontinfo->font);
            code = dmobj->FindStrFast(0, 27, 1279, 826, fontinfo->word, fontinfo->color_offset, 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("%s:查找:成功, lposX:%ld, lposY:%ld", fontinfo->word, lposX, lposY);
                break;
            }
        }

        if (timeout > 0)
        {
            deltaTime = difftime(time(NULL), lastChckTime);
            if (deltaTime > (timeout * 1.0))
            {
                api_log_MsgDebug("查找:超时:%d秒", timeout);
                break;
            }

            api_log_MsgDebug("查找:没有:超时:%d秒:%d秒", timeout, deltaTime);
        }

        if (adjust_pos)
        {
            deltaTime = difftime(time(NULL), lastAdjposTime);
            if (deltaTime < 15.0)
            {
                continue;
            }

            api_log_MsgDebug("打怪:归位");
            
            code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
            if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
            {
                api_log_MsgDebug("打怪:归位:坐标:死亡:复活");
                return SNAIL_ERRNO_REALIVE;
            }

            api_log_MsgDebug("打怪:归位:坐标:成功");

            lastAdjposTime = time(NULL); //更新归位时间
        }
        api_log_MsgDebug("查找:失败");
        Delay(500);
    }

    for (idx = 0; idx < 80; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int ex175FightAutoStop(Idmsoft* dmobj, void* ctxt)
{
    dmobj->KeyDownChar("w");
    Delay(25);
    dmobj->KeyUpChar("w");
    Delay(25);
    
    dmobj->KeyDownChar("s");
    Delay(25);
    dmobj->KeyUpChar("s");
    Delay(25);

    return SNAIL_ERRNO_SUCCESS;
}

static int ex175FightAutoStart(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    int code = 0;
    VARIANT pposX, pposY;
    long lposX = 0, lposY = 0;
    VARIANT pposX2, pposY2;
    long lposX2 = 0, lposY2 = 0;
    
    for (;;)
    {
        dmobj->MoveTo(105, 143);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTEX175_DIR"font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开始", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("战斗:辅助:打开:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("战斗:辅助:打开:失败");
        LUAMAN_WAIT_MS(500);

        code = ex175Checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("战斗:辅助:打开:死亡:复活");
            return code;
        }
    }

    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(500);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(500);

        dmobj->SetDict(0, FONTEX175_DIR"font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开始", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("战斗:辅助:开始:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("战斗:辅助:开始:失败,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        LUAMAN_WAIT_MS(500);

        code = ex175Checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("战斗:辅助:开始:死亡:复活");
            return code;
        }
    }

    return SNAIL_ERRNO_SUCCESS;
}

static int ex175FightTimeoutAuto(Idmsoft* dmobj, void* ctxt, struct font_info_of_fight_t* fif)
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
    CString pathOfPosXYs;
    int findfont = fif->findfont;
    int timeout = fif->timeout;
    int adjust_pos = fif->adjust_pos;
    struct font_info_t* fontinfo = &fif->font;
    
    api_log_MsgDebug("自动:打怪:开始");

    code = ex175FightAutoStart(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code)
    {
        api_log_MsgDebug("自动:打怪:开始:血条:死亡,复活");
        return code;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        code = ex175Checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:血条:检查,死亡并复活");
            return code;
        }
        
        if (findfont)
        {
            dmobj->SetDict(0, fontinfo->font);
            code = dmobj->FindStrFast(0, 27, 1279, 826, fontinfo->word, fontinfo->color_offset, 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("%s:查找:成功, lposX:%ld, lposY:%ld", fontinfo->word, lposX, lposY);
                break;
            }
        }

        if (timeout > 0)
        {
            deltaTime = difftime(time(NULL), lastChckTime);
            if (deltaTime > (timeout * 1.0))
            {
                api_log_MsgDebug("查找:超时:%d秒", timeout);
                break;
            }

            api_log_MsgDebug("查找:没有:超时:%d秒:%d秒", timeout, deltaTime);
        }

        api_log_MsgDebug("查找:失败");
        Delay(500);
    }

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    ex175FightAutoStop(dmobj, ctxt);
    
    for (idx = 0; idx < 70; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("自动:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int __doworkex175(Idmsoft* dmobj, void* ctxt, int dotask_num)
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
    VARIANT pposX5, pposY5;
    VARIANT pposX7, pposY7;
    VARIANT pposX8, pposY8;
    long posX = 0, posY = 0;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX5 = 0, lposY5 = 0;
    long lposX7 = 0, lposY7 = 0;
    long lposX8 = 0, lposY8 = 0;
    long lposX0 = 0, lposY0 = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    int i = 0;
    int j = 0;
    char* path = NULL;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int deltaX = 0;
    int deltaY = 0;
    char* randtime = NULL;
    int status = 0;
    int auto_walk_index = 0;
    int auto_fight_index = 0;

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
        
        dmobj->SetDict( 0, FONTEX175_DIR"font-auxiend.txt");
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
    
        dmobj->SetDict(0, FONTEX175_DIR"font-auxiend.txt");
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
    
        dmobj->SetDict(0, FONTEX175_DIR"font-auxiend.txt");
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
        
        dmobj->SetDict( 0, FONTEX175_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "退出队伍", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTEX175_DIR"font-auxiend.txt");
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
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTEX175_DIR"font-cpy6_npc5.txt");
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

        dmobj->SetDict(0, FONTEX175_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "入场卷发放", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("入场卷发放:点击:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONTEX175_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来", "00ff00-101010", 0.9, &pposX2, &pposY2);
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

                dmobj->SetDict(0, FONTEX175_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "明天再来", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("明天再来:点击:成功");
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

        dmobj->SetDict(0, FONTEX175_DIR"font-cpy6_npc5.txt");
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
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTEX175_DIR"font-cpy6_npc5.txt");
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

    deltaX = deltaatpos175[0].x;
    deltaY = deltaatpos175[0].y;
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTEX175_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("确认:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = common_checkcopyfull(dmobj, ctxt);
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

        code = common_checkcopyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("确认:点击:副本满:检查:重试");
            goto Label_NPC_next_retry;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-okcancel.txt");
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

    dmobj->KeyPressChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay(500);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    dmobj->KeyDownChar("shift");
    Delay(200);
    dmobj->KeyDownChar("e");
    Delay(200);
    dmobj->KeyUpChar("shift");
    Delay(200);
    dmobj->KeyUpChar("e");
    Delay(200);
    
    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:对话:树藤:查找:失败");
        Delay(500);
    }

    dmobj->KeyDownChar("5");
    Delay(500);
    dmobj->KeyUpChar("5");
    Delay(500);

    //code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    code = ex175FightTimeoutAuto(dmobj, ctxt, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("打怪1:成功");
    
    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:对话:树藤:查找:失败");
        Delay(500);
    }

    dmobj->KeyDownChar("5");
    Delay(500);
    dmobj->KeyUpChar("5");
    Delay(500);

    //code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    code = ex175FightTimeoutAuto(dmobj, ctxt, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("打怪2:成功");
    
    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);
    
    code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("打怪3:成功");

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    /*
    code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("打怪4:成功");
    */
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:对话:树藤:查找:失败");
        Delay(500);
    }

    dmobj->KeyDownChar("5");
    Delay(500);
    dmobj->KeyUpChar("5");
    Delay(500);
    
    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("7");
        Delay(500);
        dmobj->KeyUpChar("7");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:对话:树藤:查找:失败");
        Delay(500);
    }

    dmobj->KeyDownChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);
    
    code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("打怪5:成功");

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:对话:树藤:查找:失败");
        Delay(500);
    }

    dmobj->KeyDownChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    dmobj->KeyDownChar("a");
    Delay(1000);
    dmobj->KeyUpChar("a");
    Delay(500);

    //找黄色的气体操纵杆
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "气体操纵杆", "ffff00-101010", 0.9, &pposX5, &pposY5);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX5 = pposX5.lVal;
            lposY5 = pposY5.lVal;

            api_log_MsgDebug("气体操纵杆:查找:成功, lposX:%ld, lposY:%ld", lposX5, lposY5);
            break;
        }

        api_log_MsgDebug("气体操纵杆:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos175_u[0].deltax;
    deltaY = deltaatpos175_u[0].deltay;
    
    //点击黄色的气体操纵杆
    int idx = 0;
    for (idx = 0; idx < 1; idx++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX5 + deltaX, lposY5 + deltaY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "气体操纵杆", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("气体操纵杆:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("气体操纵杆:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:对话:树藤:查找:失败");
        Delay(500);
    }

    dmobj->KeyDownChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    dmobj->KeyDownChar("s");
    Delay(1000);
    dmobj->KeyUpChar("s");
    Delay(500);
    
    //找黄色的气体操纵杆
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "气体操纵杆", "ffff00-101010", 0.9, &pposX5, &pposY5);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX5 = pposX5.lVal;
            lposY5 = pposY5.lVal;

            api_log_MsgDebug("气体操纵杆:查找:成功, lposX:%ld, lposY:%ld", lposX5, lposY5);
            break;
        }

        api_log_MsgDebug("气体操纵杆:查找:失败");
        Delay(500);
    }

    deltaX = deltaatpos175_u[1].deltax;
    deltaY = deltaatpos175_u[1].deltay;
    
    //点击黄色的气体操纵杆
    for (idx = 0; idx < 1; idx++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX5 + deltaX, lposY5 + deltaY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "气体操纵杆", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("气体操纵杆:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("气体操纵杆:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "树藤", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:对话:树藤:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:对话:树藤:查找:失败");
        Delay(500);
    }

    dmobj->KeyDownChar("5");
    Delay(500);
    dmobj->KeyUpChar("5");
    Delay(500);

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    //找绿色进入管制室
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175DuiHua3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入管制室", "00ff00-101010", 0.9, &pposX7, &pposY7);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX7 = pposX7.lVal;
            lposY7 = pposY7.lVal;

            api_log_MsgDebug("对话:进入管制室:查找:成功, lposX:%ld, lposY:%ld", lposX7, lposY7);
            break;
        }

        api_log_MsgDebug("对话:进入管制室:查找:失败");
        Delay(500);
    }

    //点击绿色进入管制室
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX7, lposY7);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTEX175_DIR"font-175DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "进入管制室", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("进入管制室:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("进入管制室:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("打怪6:成功");

    //等待坐标变成 142:237
    for (;;)
    {
        dmobj->SetDict(0, FONTEX175_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                //bug:误差太大可能就在目标点附近,实际并没有飞拢 2022/01/08 15:43:16
                if (abs(posX - 142) <= 1 && abs(posY - 237) <= 1)
                {
                    api_log_MsgDebug("坐标:142:237:匹配");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("坐标:142:237:未匹配");
        Delay(1000);
    }
    
    //code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    code = ex175FightTimeoutAuto(dmobj, ctxt, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("打怪7:成功");

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    dmobj->KeyDownChar("9");
    Delay(500);
    dmobj->KeyUpChar("9");
    Delay(500);
    
    //找绿色脱离副本
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTEX175_DIR"font-175DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "脱离副本", "00ff00-101010", 0.9, &pposX8, &pposY8);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX8 = pposX8.lVal;
            lposY8 = pposY8.lVal;

            api_log_MsgDebug("对话:脱离副本:查找:成功, lposX8:%ld, lposY8:%ld", lposX8, lposY8);
            break;
        }

        api_log_MsgDebug("对话:脱离副本:查找:失败");
        Delay(500);
    }

    //点击绿色脱离副本
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX8, lposY8);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTEX175_DIR"font-175DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "脱离副本", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("对话:脱离副本:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("对话:脱离副本:找字:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    Delay(100);

    status = common_exitgroup(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgError("退组:失败:status:%d", status);
        return status;
    }

    api_log_MsgDebug("退队:成功");
    return 0;
}

static int _doworkex175(Idmsoft* dmobj, void* ctxt, int dotask_num)
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
    VARIANT pposX5, pposY5;
    VARIANT pposX7, pposY7;
    VARIANT pposX8, pposY8;
    long posX = 0, posY = 0;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX5 = 0, lposY5 = 0;
    long lposX7 = 0, lposY7 = 0;
    long lposX8 = 0, lposY8 = 0;
    long lposX0 = 0, lposY0 = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    int i = 0;
    int j = 0;
    char* path = NULL;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int deltaX = 0;
    int deltaY = 0;
    char* randtime = NULL;
    int status = 0;
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi6.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "德拉比斯", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:德拉比斯:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:德拉比斯:查找:失败");
        Delay(1000);
    }

    code = common_exitgroup(dmobj, ctxt);
    if (ERRNO_SUCCESS != code)
    {
        return code;
    }
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTEX175_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动到DK服务器", "00ff00-101010", 0.9, &pposX, &pposY);
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
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);

        //地图变成香格里拉就对了
        dmobj->SetDict(0, FONTEX175_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "德拉比斯野外", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("地图:德拉比斯野外:进入:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }
        
        randtime = ((time(NULL)) % 2)? "DK服务器1" : "DK服务器2";
        
        dmobj->SetDict(0, FONTEX175_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, randtime, "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("%s:点击:成功", randtime);
            continue;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        lposX = lposX2;
        lposY = lposY2;
        
        api_log_MsgDebug("%s:点击:失败, lposX2:%ld, lposY2:%ld", randtime, lposX2, lposY2);
        
        Delay(500);
    }

    //等待进入地图
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "德拉比斯野外", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("地图:德拉比斯野外:进入:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("地图:德拉比斯野外:查找:失败");
        Delay(1000);
    }

    //按M，找贵宾服务的相对位置2
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

        dmobj->SetDict(0, FONTEX175_DIR"font-npc1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "圣魔之爪", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("圣魔之爪:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("圣魔之爪:查找:失败");
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
    
        dmobj->SetDict(0, FONTEX175_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("其他:移动:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            
            break;
        }

        api_log_MsgDebug("其他:移动:失败");
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
    
        dmobj->SetDict(0, FONTEX175_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动:点击:成功");
            
            break;
        }

        
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("其移动:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    //等待坐标变成 16:12
    for (;;)
    {
        dmobj->SetDict(0, FONTEX175_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                //bug:误差太大可能就在目标点附近,实际并没有飞拢 2022/01/08 15:43:16
                if (abs(posX - 16) <= 1 && abs(posY - 12) <= 1)
                {
                    api_log_MsgDebug("坐标:16:12:匹配");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("坐标:16:12:未匹配");
        Delay(1000);
    }

    for (;;)
    {
        status = __doworkex175(dmobj, ctxt, dotask_num);
        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("副本:ex175:循环:结束,dotask_num:%d", dotask_num);
            break;
        }

        api_log_MsgDebug("副本:ex175:循环:继续,dotask_num:%d", dotask_num);
    }
    
    return 0;
}


static int doworkex175(struct cluamanaccountinfo* accountinfo, int roleno, struct common_loop_ctxt_t* clctxt)
{
    Idmsoft* dmobj = clctxt->dmobj;
    void* ctxt = clctxt->ctxt;
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
    int j = 0;
    char* path = NULL;
    int dotask_num = 0;
    time_t check_conn_start;
    time_t check_conn_stop;
    double check_conn_deltaTime = 0.0;
    
    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dotask_num = 0;
    
    //执行“任务副本”
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        code = _doworkex175(dmobj, ctxt, dotask_num);
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
    
    return 0;
}

int api_TaskEx175Init(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int bindWnd = luaman_config->curBindWnd;
    int curBindHostIdx = luaman_config->curBindHostIdx;
    
    api_log_MsgDebug("副本:Ex175:初始化");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);
    api_log_MsgDebug("curBindHostIdx  : %d", curBindHostIdx);
    
    api_PrintConfig(luaman_config);
    
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("窗口:绑定:失败, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("窗口:绑定:成功, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("窗口:鼠标:锁定:失败, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("窗口:鼠标:锁定:成功, hwnd:%ld", hwnd);

    /* 初始化告警时间初始值 */
    init_alarm_ctxt();
    
    return ERRNO_SUCCESS;
}

int api_TaskEx175Main(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int bindWnd = luaman_config->curBindWnd;
    struct cluamanbindinfo* bpos = NULL, *bn = NULL, *bnode = NULL;
    
    api_log_MsgDebug("副本:Ex175:主循环");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    list_for_each_entry_safe(bpos, bn, &luaman_config->bindinfoList, node, struct cluamanbindinfo)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        if (bindWnd != bpos->bindWndnr)
        {
            api_log_MsgDebug("ExSignleMain bindwnd unexpected, bindWnd:%ld. bindWndnr:%ld",
                bindWnd, bpos->bindWndnr);
            continue;
        }

        //return mainloop(&bpos->accountList, dmobj, ctxt);
        struct common_loop_ctxt_t clctxt;
        LUAMAN_MEMSET(&clctxt, 0, sizeof(struct common_loop_ctxt_t));
        clctxt.accountList = &bpos->accountList;
        clctxt.dmobj = dmobj;
        clctxt.ctxt = ctxt;
        clctxt.fontdir = FONTEX175_DIR;
        clctxt.bmpdir = BMPEX175_DIR;
        clctxt.configXML = CONFIG_EX175_XML;
        clctxt.pre_work = common_prework;
        clctxt.post_work = common_postwork;
        clctxt.do_work = doworkex175;
        
        return common_mainloop(&clctxt);
    }

    return ERRNO_SUCCESS;
}

int api_TaskEx175Exit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int bindWnd = luaman_config->curBindWnd;
    
    api_log_MsgDebug("副本:Ex175:退出");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
    
    return ERRNO_SUCCESS;
}


