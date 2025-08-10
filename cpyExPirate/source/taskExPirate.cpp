#include <math.h>
#include <stdlib.h>
#include <process.h>
#include "stdafx.h"
//#include "xrrno.h"
#include "xlog.h"
#include "xcommon.h"
#include "xlist.h"
#include "task.h"
#include "config.h"
#include "alarm.h"
#include "task.h"
#include "taskExPirate.h"

#define FONT_EXPIRATE_DIR "font\\expirate\\"
#define BMP_EXPIRATE_DIR "bmp\\expirate\\"

//寻路坐标,共25个寻路坐标
static char* walk_path[] =
{
    "382:382|385:371|390:356|394:343|401:324|405:312", //寻路（1）
    "407:303|411:292|416:282", //寻路2
    "424:278|436:272|448:266|458:266|467:271|471:280|475:291|475:303|474:313|470:324|467:334|464:345|461:355", //寻路3
    "459:363|459:374|458:386|458:397|458:408|458:422", //寻路4
    "455:438|449:449|442:464|431:471|421:476|411:478|400:480|389:482|377:481|366:480|356:480|345:480|336:480|325:479|315:473|306:465|298:456|291:448|287:440", //寻路5
    "284:431|283:420|287:408|289:397|292:385|292:380", //寻路6
    "295:367|299:356|305:345|311:335|319:322|325:312|330:303|337:294|344:282|349:272|352:261|354:247|355:236|356:222|361:212|370:205|380:197|387:190", //寻路7
    "394:176|398:167|396:155|394:143|393:131|396:120|400:116", //寻路8
    "448:116|440:116|432:116|423:116|415:116|410:116", //寻路9
    "399:116|392:111|390:105|387:98|382:89", //寻路10
    "374:89|366:90|358:92|348:94|336:95", //寻路11
    "332:88|325:79|320:71|313:63|307:60|291:60|289:59|283:55|274:50|265:46|253:46|246:47|236:47|228:46|221:44|213:41|206:34|196:30|187:27", //寻路12
    "197:31|207:37|215:42|224:47|234:48|245:48|257:47|270:49|280:54|289:57|299:60|308:61|317:67|323:73|323:81|324:91|325:102|324:109|319:115|314:122|309:131|305:136|299:142|292:147|285:151|275:157|267:163|260:168|254:173|248:180|241:189|234:195|225:202", //寻路13
    "231:197|238:192|246:184|251:179|258:171|266:165|272:160|280:154|287:150|295:144|302:139|308:133|313:123|319:116|326:111|334:111|340:118|340:127|339:137|337:147|333:157|327:167|321:176|315:184|309:193|304:202|299:212|296:223|291:238", //寻路14
    "294:230|296:222|299:214|302:206|305:199|310:191|315:185|322:177|328:168|332:160|336:150|338:141|340:132|340:124|338:117|333:107|326:100|317:94", //寻路15
    "309:93|302:95|292:99|284:102|274:105|265:106|254:106|244:106|233:105|224:105|216:107|207:111|200:117", //寻路16
    "199:125|198:132|193:147|189:154|182:163|176:169", //寻路17
    "172:181|170:191|169:200|169:209|170:223|170:231|170:243", //寻路18
    "176:263|176:278|179:292|183:306|182:318", //寻路19
    "179:331|180:342|182:353|183:363", //寻路20
    "183:371|183:380|179:389|172:400|166:408|159:415|150:421|142:426|133:430|126:433|119:434", //寻路21
    "61:308|64:303|63:299|64:294|63:289|63:284|65:280|64:274|65:272|64:266|63:261", //寻路22
    "56:260|48:260|40:260|35:254|37:245|41:237|49:235|54:232|59:228", //寻路23
    "64:224|61:219|56:215|56:209|55:204|55:196|56:189|59:185|64:180|59:178|55:175|57:169|58:162|61:156|64:150", //寻路24
    "66:141|65:142|63:124|63:105", //寻路25
    "63:99|61:90|57:80|54:67|55:60|59:54|63:51|64:47|64:44|64:39|63:34|63:31", //寻路26
};

//结束条件是否找字,超时时间(单位:秒),是否归位,打怪结束字样,字库,颜色偏移值
//找字结束:
//  {FONT_FIND, 20, ADJUST_POS, {"门已开启",     FONT_EXPIRATE_DIR"font-xxx.txt", "ffffff-101010"}}
//如果结束条件不找字,则第4~最后字段不用填写,如:
//  {NO_FONT_FIND, 20, ADJUST_POS}
//一共有19个
static struct font_info_of_fight_t fif_info[] =
{
    {NO_FONT_FIND, 20, ADJUST_POS, }, //打怪（1）
    {FONT_FIND,    50, ADJUST_POS,   {"出现了一条路",     FONT_EXPIRATE_DIR"font-HaiDaoChuan-1.txt", "ffffff-101010"}}, //打怪2
    {NO_FONT_FIND, 25, ADJUST_POS, }, //打怪（3）
    {FONT_FIND,    50, ADJUST_POS,   {"出现了一条路",     FONT_EXPIRATE_DIR"font-HaiDaoChuan-1.txt", "ffffff-101010"}},
    {NO_FONT_FIND, 40, ADJUST_POS, }, //打怪（5）
    {FONT_FIND,    70, ADJUST_POS,   {"出现了一条路",     FONT_EXPIRATE_DIR"font-HaiDaoChuan-1.txt", "ffffff-101010"}},
    {NO_FONT_FIND, 25, ADJUST_POS, }, //打怪（7）
    {FONT_FIND,    30, ADJUST_POS,   {"前往分岔地区",     FONT_EXPIRATE_DIR"font-HaiDaoChuan-4.txt", "ffffff-101010"}},
    {FONT_FIND,    70, ADJUST_POS,   {"德拉辛艾恩",     FONT_EXPIRATE_DIR"font-HaiDaoChuan-7.txt", "ffffff-101010"}},
    {FONT_FIND,    60, ADJUST_POS,   {"德拉辛兹巴伊",     FONT_EXPIRATE_DIR"font-HaiDaoChuan-5.txt", "ffffff-101010"}},
    {FONT_FIND,    60, ADJUST_POS,   {"通往海边的路",     FONT_EXPIRATE_DIR"font-HaiDaoChuan-6.txt", "ffffff-101010"}},
    {NO_FONT_FIND, 30, ADJUST_POS, }, //打怪（12）
    {NO_FONT_FIND, 30, ADJUST_POS, }, //打怪（13）
    {NO_FONT_FIND, 30, ADJUST_POS, }, //打怪（14）
    {NO_FONT_FIND, 30, ADJUST_POS, }, //打怪（15）
    {FONT_FIND,    30, ADJUST_POS,   {"特洛斯出现了",     FONT_EXPIRATE_DIR"font-HaiDaoChuan-8.txt", "ffffff-101010"}},
    {NO_FONT_FIND, 20, ADJUST_POS, }, //打怪（17）
    {NO_FONT_FIND, 20, ADJUST_POS, }, //打怪（18）
    {FONT_FIND,    30, ADJUST_POS,   {"消灭时间",     FONT_EXPIRATE_DIR"font-HaiDaoChuan-11.txt", "fdb602-101010"}},
    
};

//打boss的u怪、打怪结束条件等字库配置,一共有3个boss
static struct font_info_of_fight_ex_t fif_info_ex[] =
{
    //一个boss打怪u、结束条件
    {
        //结束条件是否找字,超时时间(单位:秒),是否归位,结束条件,是否u怪,u怪字库
        FONT_FIND, 60, ADJUST_POS,
        {"不再召唤怪物",       FONT_EXPIRATE_DIR"font-HaiDaoChuan-3.txt", "ffffff-101010"}, //打怪结束的字样
        UFONT, 
        {"召唤的祭品",        FONT_EXPIRATE_DIR"font-HaiDaoChuan-2.txt", "00ff00-101010"}, //u怪的字样
    },

    //第二个boss,依葫芦画瓢
        {
        //是否u怪,结束条件是否找字,超时时间(单位:秒),是否归位
        FONT_FIND, 60, ADJUST_POS,
        {"恭喜你",       FONT_EXPIRATE_DIR"font-HaiDaoChuan-10.txt", "ffffff-101010"}, //打怪结束的字样
        UFONT, 
        {"地狱特罗斯",        FONT_EXPIRATE_DIR"font-HaiDaoChuan-9.txt", "00ff00-101010"}, //u怪的字样
    },

    //第3个boss
        {
        //是否u怪,结束条件是否找字,超时时间(单位:秒),是否归位
        FONT_FIND, 600, ADJUST_POS,
        {"离开时间",       FONT_EXPIRATE_DIR"font-HaiDaoChuan-13.txt", "fdb602-101010"}, //打怪结束的字样
        UFONT, 
        {"愤怒至极的",        FONT_EXPIRATE_DIR"font-HaiDaoChuan-12.txt", "00ff00-101010"}, //u怪的字样
    },
    
    
};

//6   寻路25.鼠标右键，点一下固定位置
static struct luaman_posxy right_click_pos[] =
{
    {511, 739},
};

static int expirateBuyBlue(Idmsoft* dmobj, void* ctxt)
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

    posX1 = 1069;
    posY1 = 48;

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        //点击之后出现"仓库"字样
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-surebox.txt");
        code = dmobj->FindStrFast(590, 500, 687, 530, "仓库", "ffffff-101010", 0.9, &pposX1, &pposY1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX1 = pposX1.lVal;
            lposY1 = pposY1.lVal;

            api_log_MsgDebug("商店:仓库:字样:找字:成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
            break;
        }
        
        api_log_MsgDebug("商店:仓库:字样:找字:失败");
        Delay(500);
    }

    //点击商店 633:510->633:380
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX1, lposY1 - 130);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("商店:背包:字样:找字:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("商店:背包:字样:找字:失败");
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    Delay(500);
        
    for (;;)
    {
        //点击之后出现"商店"字样
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-shop.txt");
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
        dmobj->MoveTo(lposX1 + 160, lposY1 + 74);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-okcancel.txt");
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
    code = dmobj->SendString(hwnd, "22");
    if (1 != code)
    {
        api_log_MsgError("商店:金额:60:输入:失败");
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("密码:金额:60:输入:成功");

    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-okcancel.txt");
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

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    api_log_MsgDebug("买蓝:完毕");
    return SNAIL_ERRNO_SUCCESS;
}


static int expirateSaveXstrong2Library(Idmsoft* dmobj, void* ctxt)
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
    int findxstrong = 0;

    api_log_MsgDebug("cleanPackageX:0x%x", cleanPackageX);
    
    //查找背包标签
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        //检查是否按成功
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("仓库:背包:字样:找字,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

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

        api_log_MsgDebug("仓库:背包:字样:找字,成功");
        Delay(500);
    }

    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;
    
    api_log_MsgDebug("仓库:背包:宝物柜, lineX:%ld, lineY:%ld", lineX, lineY);

    for (index = 0; index < 5; index++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        posY = lineY + index * deltaY;
        
        for (jndex = 0; jndex < 10; jndex++)
        {
            posX = lineX + jndex * deltaX;
            
            boxPosXYs[index * 10 + jndex].x = posX;
            boxPosXYs[index * 10 + jndex].y = posY;

            api_log_MsgDebug("boxPosXYs[%d][%d] = [%ld, %ld]", index, jndex, posX, posY);
        }
    }

    api_log_MsgDebug("仓库:背包:宝物柜,准备处理");

    //处理每个背包的所有宝物柜
    int pkgnum = 2;
    for (lndex = 0; lndex < pkgnum; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }

        //点开标签页
        dmobj->MoveTo(labelPosXY[lndex].x, labelPosXY[lndex].y);
        Delay(200);
        dmobj->LeftDown();
        Delay(200);
        dmobj->LeftUp();
        Delay(200);

        for (index = 0; index < 5; index++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            for (jndex = 0; jndex < 10; jndex++)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                posX = boxPosXYs[index * 10 + jndex].x;
                posY = boxPosXYs[index * 10 + jndex].y;

                dmobj->MoveTo(posX, posY);
                Delay(200);

                api_log_MsgDebug("处理:格尼亚罗:背包:%d:柜子:[%d,%d]:坐标:[%ld, %ld]",
                    lndex, index, jndex, posX, posY);
                
                findxstrong = 0;

                //查找'小强|中强|高强'字样
                dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-MingZi7.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "格尼亚罗", "ffffff-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    findxstrong = 1;

                    lposX1 = pposX1.lVal;
                    lposY1 = pposY1.lVal;

                    Delay(200);
                    dmobj->RightClick();
                    Delay(200);

                    api_log_MsgDebug("宝物柜:格尼亚罗:存储,成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                    //break;
                }

                if (0 == findxstrong)
                {
                    dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-MingZi7.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "海伯伦戒指", "a335ee-101010", 0.9, &pposX1, &pposY1);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        findxstrong = 2;

                        lposX1 = pposX1.lVal;
                        lposY1 = pposY1.lVal;

                        Delay(200);
                        dmobj->RightClick();
                        Delay(200);

                        api_log_MsgDebug("宝物柜:海伯伦戒指:存储,成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                        //break;
                    }
                }
            }
        }
    }

    return SNAIL_ERRNO_SUCCESS;
}


static int expirateCleanPackage(Idmsoft* dmobj, void* ctxt)
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
    CString strArrangePackPosXYs;
    CStringArray arrangePackPosXYs;
    int tgtNum = 0;
    int tgtNum2 = 0;
    long posX = 0;
    long posY = 0;
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    int index = 0;
    int jndex = 0;
    CString strPosXYtgt;
    CStringArray posXYtgt;
    CString strPosXY;
    CStringArray posXY;
    long posX1 = 0, posY1 = 0;
    long buildx = 0, buildy = 0;

    //按i弹出背包
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(200);
        dmobj->KeyUpChar("i");
        Delay(200);

        //检查是否按成功
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("清理背包:背包:找字,成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("清理背包:背包:找字,失败");
        Delay(500);
    }

    api_log_MsgDebug("清包:准备:点击");
    
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
    
    //重新按i,并记录背包I/II/III/IV/V的坐标位置
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //检查是否按成功
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("背包:字样:找字,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

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

        api_log_MsgDebug("背包:字样:找字,失败");
        Delay(500);
    }

    api_log_MsgDebug("准备查找仓库图标");
    
    posX1 = 1069;
    posY1 = 48;

    //点击图标
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);

        //点击之后出现"仓库"字样
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-surebox.txt");
        code = dmobj->FindStrFast(590, 500, 687, 530, "仓库", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("仓库:字样:找字,成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("仓库:字样:找字,失败");
        Delay(500);
    }

    api_log_MsgDebug("准备点击仓库按钮, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
    
    //点击'仓库'字样
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);

        //查找出现背包则表明点击成功
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "背包", "ffffff-101010", 0.9, &pposX1, &pposY1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX1 = pposX1.lVal;
            lposY1 = pposY1.lVal;

            api_log_MsgDebug("仓库:背包:字样:找字,成功, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
            break;
        }

        api_log_MsgDebug("仓库:背包:字样:找字,失败");
        Delay(500);
    }

    api_log_MsgDebug("准备:清理:背包");

    //处理背包中的东西
    expirateSaveXstrong2Library(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    return SNAIL_ERRNO_SUCCESS;
}

static int expirateCheckcopyfull(Idmsoft* dmobj, void* ctxt)
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
    dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-reenter.txt");
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
            dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-reenter.txt");
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

            dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-reenter.txt");
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
    dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-copyfull.txt");
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

            dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-copyfull.txt");
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

static int expirateCheckkeystate(Idmsoft* dmobj, void* ctxt)
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

static int expirateChecklife(Idmsoft* dmobj, void* ctxt, int recusive)
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

    dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-blood.txt");
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
    }
    else
    {
        strBBloodLen = strBBlood.GetLength();
        bBloodAva = atol(strBBlood.Mid(1, strBBloodLen - maxBlueFiled));
        bBloodTot = atol(strBBlood.Mid(strBBloodLen - maxBlueFiled + 1, strBBloodLen));

        api_log_MsgDebug("血条:解析:成功,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
            hwnd, bBloodAva, bBloodTot);
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

    expirateCheckkeystate(dmobj, ctxt);

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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-chcklife.txt");
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-chcklife.txt");
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
        code = expirateChecklife(dmobj, ctxt, 1);
        if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("复活:站立:完成");
            break;
        }

        Delay_With_Terminate_Check(1, luaman_task, dmobj);
    }

    return SNAIL_ERRNO_REALIVE;
}


static int expirateWalk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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
                (void)expirateCheckkeystate(dmobj, ctxt);

                api_log_MsgDebug("寻路:坐标:卡住,停止寻路:等待副本消失,下一坐标, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //等待地洞副本消失
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //先检查下血条
            code = expirateChecklife(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //读取当前坐标
            dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)expirateCheckkeystate(dmobj, ctxt);
                
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

                (void)expirateCheckkeystate(dmobj, ctxt);
                
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
        code = expirateChecklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:卡住:血条:检查:死亡:复活");
            return code;
        }
        
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-map.txt");
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

static int expirateFightAutoStop(Idmsoft* dmobj, void* ctxt)
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

static int expirateFightAutoStart(Idmsoft* dmobj, void* ctxt, int seqnr)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    
    dmobj->MoveTo(5, 5);
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(105, 143);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开始", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
                
            api_log_MsgDebug("战斗:辅助:打开:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        code = expirateChecklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点:打怪:血条:死亡,复活");
            return code;
        }
    }

    lposX3 = lposX;
    lposY3 = lposY;
    
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-HaiDaoChuan-16.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "战斗辅助", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
                
            api_log_MsgDebug("战斗辅助:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = expirateChecklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点:打怪:血条:死亡,复活");
            return code;
        }

        api_log_MsgDebug("战斗辅助:查找:失败");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2 + 228, lposY2 + 29);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        if (1 == seqnr)
        {
            dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-HaiDaoChuan-14.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "1号设置", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                    
                api_log_MsgDebug("1号设置:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }
        }
        else if (2 == seqnr)
        {
            dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-HaiDaoChuan-15.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "2号设置", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                    
                api_log_MsgDebug("2号设置:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }
        }
        else
        {
            api_log_MsgDebug("unknown seqnr %d", seqnr);
        }

        code = expirateChecklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点:打怪:血条:死亡,复活");
            return code;
        }
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

        if (1 == seqnr)
        {
            dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-HaiDaoChuan-14.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "1号设置", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("1号设置:点击:成功");
                break;
            }

            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
                
            api_log_MsgDebug("1号设置:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        }
        else if (2 == seqnr)
        {
            dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-HaiDaoChuan-15.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "2号设置", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("2号设置:点击:成功");
                break;
            }

            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
                
            api_log_MsgDebug("2号设置:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        }
        else
        {
            api_log_MsgDebug("unknown seqnr %d", seqnr);
        }

        code = expirateChecklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点:打怪:血条:死亡,复活");
            return code;
        }
    }

    lposX = lposX3;
    lposY = lposY3;
    
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "开始", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("战斗:辅助:开始");
            break;
        }

        code = expirateChecklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("定点:打怪:血条:死亡,复活");
            return code;
        }
    }

    return SNAIL_ERRNO_SUCCESS;
}


static int expirateFightTimeoutAuto(Idmsoft* dmobj, void* ctxt, struct font_info_of_fight_t* fif, int seqnr)
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

    code = expirateFightAutoStart(dmobj, ctxt, seqnr);
    if (SNAIL_ERRNO_REALIVE == code)
    {
        api_log_MsgDebug("自动:打怪:开始:血条:死亡,复活");
        return code;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        code = expirateChecklife(dmobj, ctxt, 0);
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
    expirateFightAutoStop(dmobj, ctxt);
    
    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("自动:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}


static int expirateFightTimeout(Idmsoft* dmobj, void* ctxt, int withu, struct font_info_of_fight_t* fif)
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
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        code = expirateChecklife(dmobj, ctxt, 0);
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
            code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
            if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
            {
                api_log_MsgDebug("打怪:归位:坐标:死亡:复活");
                return SNAIL_ERRNO_REALIVE;
            }

            api_log_MsgDebug("打怪:归位:坐标:成功");
        }
        api_log_MsgDebug("查找:失败");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int expirateBossFightTimeout(Idmsoft* dmobj, void* ctxt, struct font_info_of_fight_ex_t* fif_ex)
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
    int ufindfont = fif_ex->ufindfont;
    int findfont = fif_ex->findfont;
    int timeout = fif_ex->timeout;
    int adjust_pos = fif_ex->adjust_pos;
    struct font_info_t* ufont = &fif_ex->ufont;
    struct font_info_t* fontinfo = &fif_ex->font;
    int uflag = 0;
    
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
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        code = expirateChecklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:血条:检查,死亡并复活");
            return code;
        }
        
        if (ufindfont && (0 == uflag))
        {
            dmobj->KeyDownChar("u");
            Delay(50);
            dmobj->KeyUpChar("u");
            Delay(50);
            dmobj->SetDict(0, ufont->font);
            code = dmobj->FindStrFast(0, 27, 1279, 826, ufont->word, ufont->color_offset, 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                uflag = 1;
                api_log_MsgDebug("%s:u怪:成功, lposX:%ld, lposY:%ld", ufont->word, lposX, lposY);
            }
            else
            {
                api_log_MsgDebug("%s:u怪:失败", ufont->word);
            }
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
            code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
            if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
            {
                api_log_MsgDebug("打怪:归位:坐标:死亡:复活");
                return SNAIL_ERRNO_REALIVE;
            }

            api_log_MsgDebug("打怪:归位:坐标:成功");
        }
        api_log_MsgDebug("查找:失败");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}

static int expirateFightDYHLDtimeout(Idmsoft* dmobj, void* ctxt, int seqnr)
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
    int uflag = 0;
    int failrnr = 0;
    int i = 0;

    api_log_MsgDebug("海德罗:打怪:开始");

    code = expirateFightAutoStart(dmobj, ctxt, seqnr);
    if (SNAIL_ERRNO_REALIVE == code)
    {
        api_log_MsgDebug("自动:打怪:开始:血条:死亡,复活");
        return code;
    }

    api_log_MsgDebug("地狱海罗德:攻击:发起");
    
    lastChckTime = time(NULL);
    
    for (;;)
    {
    Label_FightatPoint16_start:

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        code = expirateChecklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("寻路:血条:检查,死亡并复活");
            return code;
        }

        dmobj->MoveTo(5, 5);
        Delay(100);
        
       //检查有没有打死
       for (;;)
       {
			Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
			for(i = 0; i < 2; i++)
			{
				Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
				dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-MingZi3.txt");
				code = dmobj->FindStrFast(0, 27, 1279, 826, "地狱海罗德", "ffffff-101010", 0.9, &pposX, &pposY);
				dmobj->ClearDict(0);
				if (-1 != code)
				{
					lposX = pposX.lVal;
					lposY = pposY.lVal;
                    failrnr = 0;
                    
					api_log_MsgDebug("海德罗:白色:攻击:继续, lposX:%ld, lposY:%ld", lposX, lposY);
					goto Label_FightatPoint16_start;
				}

				dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-MingZi3.txt");
				code = dmobj->FindStrFast(0, 27, 1279, 826, "地狱海罗德", "ff3300-101010", 0.9, &pposX, &pposY);
				dmobj->ClearDict(0);
				if (-1 != code)
				{
					lposX = pposX.lVal;
					lposY = pposY.lVal;
                    failrnr = 0;
                    
					api_log_MsgDebug("海德罗:红色:攻击:继续, lposX:%ld, lposY:%ld", lposX, lposY);
					goto Label_FightatPoint16_start;
				 }

                dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-MingZi3.txt");
				code = dmobj->FindStrFast(0, 27, 1279, 826, "地狱海罗德", "00ff00-101010", 0.9, &pposX, &pposY);
				dmobj->ClearDict(0);
				if (-1 != code)
				{
					lposX = pposX.lVal;
					lposY = pposY.lVal;
                    failrnr = 0;
                    
					api_log_MsgDebug("海德罗:绿色:攻击:继续, lposX:%ld, lposY:%ld", lposX, lposY);
					goto Label_FightatPoint16_start;
				 }
                
                code = expirateChecklife(dmobj, ctxt, 0);
                if (SNAIL_ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                    return code;
                }
			}

            //找一下诅咒时间
            dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-TiShi1.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "化身发动", "fdb602-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                failrnr = 0;
                
                api_log_MsgDebug("海德罗:攻击:化身发动:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            code = expirateChecklife(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }

            failrnr++;
            if (failrnr > 3)
            {
                expirateFightAutoStop(dmobj, ctxt);
                api_log_MsgDebug("海德罗:打怪:完成");
            }
            
            return SNAIL_ERRNO_SUCCESS;
       }
    }

    expirateFightAutoStop(dmobj, ctxt);
    api_log_MsgDebug("海德罗:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}


static int expirateFightDYHLD(Idmsoft* dmobj, void* ctxt)
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
    int uflag = 0;
    int failrnr = 0;
    int i = 0;

    api_log_MsgDebug("海德罗:打怪:开始");
    
    for (;;)
    {
    Label_FightatPoint16_start:

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        if (!uflag || 2 == uflag)
        {
            dmobj->MoveTo(5, 5);
            Delay(100);

            failrnr = 0;
            
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                dmobj->KeyDownChar("u");
                Delay(20);
                dmobj->KeyUpChar("u");
                Delay(200);

                dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-MingZi3.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "地狱海罗德", "ffffff-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uflag = 1;
                    
                    api_log_MsgDebug("地狱海罗德:白色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

				dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-MingZi3.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "地狱海罗德", "ff3300-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uflag = 1;
                    
                    api_log_MsgDebug("地狱海罗德:红色:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                failrnr++;
                if (2 == uflag && failrnr > 5)
                {
                    api_log_MsgDebug("海德罗:打怪:超时:退出");
                    return SNAIL_ERRNO_SUCCESS;
                }

                code = expirateChecklife(dmobj, ctxt, 0);
                if (SNAIL_ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                    return code;
                }
                
                api_log_MsgDebug("地狱海罗德:查找:失败");
                Delay(200);
            }
        }

        api_log_MsgDebug("地狱海罗德:攻击:发起");
        
        lastChckTime = time(NULL);
        
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            for (idx = 0; idx < 20; idx++)
            {
                dmobj->KeyDownChar("3");
                Delay(15);
                dmobj->KeyUpChar("3");
                Delay(15);
            }

            curTime = time(NULL);
            deltaTime = difftime(curTime, lastChckTime);
            if (deltaTime > 3.0)
            {
                api_log_MsgDebug("地狱海罗德:攻击:暂停");
                break;
            }

            code = expirateChecklife(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }
       }

        dmobj->MoveTo(5, 5);
        Delay(100);
        
       //检查有没有打死
       for (;;)
       {
			Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
			for(i = 0; i < 2; i++)
			{
				Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
				dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-MingZi3.txt");
				code = dmobj->FindStrFast(0, 27, 1279, 826, "地狱海罗德", "ffffff-101010", 0.9, &pposX, &pposY);
				dmobj->ClearDict(0);
				if (-1 != code)
				{
					lposX = pposX.lVal;
					lposY = pposY.lVal;
                
					api_log_MsgDebug("海德罗:白色:攻击:继续, lposX:%ld, lposY:%ld", lposX, lposY);
					goto Label_FightatPoint16_start;
				}

				dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-MingZi3.txt");
				code = dmobj->FindStrFast(0, 27, 1279, 826, "地狱海罗德", "ff3300-101010", 0.9, &pposX, &pposY);
				dmobj->ClearDict(0);
				if (-1 != code)
				{
					lposX = pposX.lVal;
					lposY = pposY.lVal;
                
					api_log_MsgDebug("海德罗:红色:攻击:继续, lposX:%ld, lposY:%ld", lposX, lposY);
					goto Label_FightatPoint16_start;
				 }

                code = expirateChecklife(dmobj, ctxt, 0);
                if (SNAIL_ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                    return code;
                }
			}

            //找一下诅咒时间
            dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-TiShi1.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "化身发动", "fdb602-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                uflag = 2;
                failrnr = 0;
                
                api_log_MsgDebug("海德罗:攻击:化身发动:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            code = expirateChecklife(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("寻路:血条:检查,死亡并复活");
                return code;
            }
            
            api_log_MsgDebug("海德罗:打怪:完成");
            return SNAIL_ERRNO_SUCCESS;
       }
    }
    
    api_log_MsgDebug("海德罗:打怪:完成");
    return SNAIL_ERRNO_SUCCESS;
}


static int expirateTask(Idmsoft* dmobj, void* ctxt)
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
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int idx = 0;
    int retry_npc_num = 0;
    
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤1
    /////////////////////////////////////////////////////////////////////////////////
LABEL_retalk_with_NPC:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        (void)check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("对话:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("对话:查找:失败");
        Delay(500);
    }

    //对话:    638:108 -> 638:494
    //delta: 0:386
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY + 386);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        code = expirateCheckcopyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("确认:查找:副本满:检查:重试");
            goto LABEL_retalk_with_NPC;
        }
        
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-okcancel.txt");
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

        code = expirateCheckcopyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("确认:点击:副本满:检查:重试");
            goto LABEL_retalk_with_NPC;
        }
        
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "确认", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 ==code)
        {
            api_log_MsgDebug("确认:点击:成功");
            
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("确认:查找:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    double cleanPackageTimeDelta = 0.0;
    time_t curTime = time(NULL);
    int cleanPackageTime =2;// luaman_config->cleanPackageTime;
    time_t cleanPackageTimeLast = time(NULL);

    //海盗船副本 启动时清包，1:启动时清包, 0:启动时不清包
    //罗爱平 2023/1/22 22:03:11
    int cleanPackageWhenStart = 1; //luaman_config->cleanPackageWhenStart;

    if (1 == cleanPackageWhenStart)
    {
        cleanPackageTimeLast = 1;
    }
    
    api_log_MsgDebug("副本:开始");

    //新增功能:增加清包处理逻辑
    if (0 == cleanPackageTimeLast)
    {
        expirateCleanPackage(dmobj, ctxt);
        cleanPackageTimeLast = time(NULL);
        expirateBuyBlue(dmobj, ctxt);
    }
    else
    {
        curTime = time(NULL);
        cleanPackageTimeDelta = difftime(curTime, cleanPackageTimeLast);
        if (cleanPackageTimeDelta >= (cleanPackageTime * 3600 * 1.0))
        {
            expirateCleanPackage(dmobj, ctxt);
            cleanPackageTimeLast = time(NULL);
            expirateBuyBlue(dmobj, ctxt);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤2
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "381:407|381:394";
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[381:394]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[381:394]:成功");
    
    //等待,当出现"副本进行时间"时开始行走
    for (;;)
    {
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "副本进行时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("副本进行时间:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("副本进行时间:查找:失败");
        LUAMAN_WAIT_MS(1000);
    }

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤3
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = walk_path[0];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[0]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[0], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }
    
    pathofposxys = walk_path[1];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[1]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[1], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[2];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[2]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[2], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[3];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[3]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[3], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[4];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[4]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[4], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[5];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:%s:成功", pathofposxys);

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[5]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[5], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }
    
    pathofposxys = walk_path[6];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }
    
    dmobj->KeyDownChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay(500);
    
    pathofposxys = walk_path[7];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }
    
    //按n对话
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
        
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "查看", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("查看:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("查看:查找:失败");
        Delay(500);
    }

    //点击查看
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至召唤", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("移动至召唤:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("移动至召唤:查找:失败");
        Delay(500);
    }

    //点击移动至召唤
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "移动至召唤", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("移动至召唤:点击:成功");
            break;
        }
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("移动至召唤:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //code = expirateBossFightTimeout(dmobj, ctxt, &fif_info_ex[0]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, (struct font_info_of_fight_t*)&fif_info_ex[0], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[8];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }
    
    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[6]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[6], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[9];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[7]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[7], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[10];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }
    
    //按n对话
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
        
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "查看", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("查看:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("查看:查找:失败");
        Delay(500);
    }

    //点击查看
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "查看", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("查看:点击:成功");
            break;
        }
        
        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("查看:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    dmobj->KeyPressChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay(500);

    pathofposxys = walk_path[11];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[8]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[8], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[12];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[9]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[9], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    dmobj->KeyDownChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay(500);

    pathofposxys = walk_path[13];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[10]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[10], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[14];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }
    
    dmobj->KeyDownChar("u");
    Delay(500);
    dmobj->KeyUpChar("u");
    Delay(500);

    dmobj->KeyDownChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);

    pathofposxys = walk_path[15];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[11]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[11], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[16];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[12]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[12], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[17];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[13]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[13], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[18];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[14]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[14], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[19];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[15]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[15], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[20];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateBossFightTimeout(dmobj, ctxt, &fif_info_ex[1]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, (struct font_info_of_fight_t*)&fif_info_ex[1], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    //按n对话
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
        
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua9.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("对话:查找:绿色:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        
        api_log_MsgDebug("对话:查找:失败");
        Delay(500);
    }
    
    //点击对话
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "我要移动", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("我要移动:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("我要移动:查找:失败");
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "我要移动", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("我要移动:点击:成功");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("我要移动:点击:失败, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤18
    /////////////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-MingZi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "操纵杆", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("操纵杆:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("操纵杆:查找:失败");
        Delay(500);
    }

    //866:31 -> 869:85
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX + 3, lposY + 54);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    }

    Delay_With_Terminate_Check(2, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = walk_path[21];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[16]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[16], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[22];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[17]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[17], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[23];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //code = expirateFightTimeout(dmobj, ctxt, 1, &fif_info[18]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, &fif_info[18], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = walk_path[24];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    //寻路25.鼠标右键，点一下固定位置
    dmobj->MoveTo(right_click_pos[0].x, right_click_pos[0].y);
    Delay(500);
    dmobj->RightClick();
    Delay(500);

    code = expirateFightDYHLDtimeout(dmobj, ctxt, 2);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgError("海罗德:打怪:失败:code:%d", code);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("海罗德:打怪:成功");
    
    //两种可能性

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤21
    /////////////////////////////////////////////////////////////////////////////////
    api_log_MsgDebug("海罗德:按空格:30下:开始");
    
    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("海罗德:按空格:30下:结束");

    int retry_Label_leave_time = 0;
    
    //找副本离开时间或者消灭时间
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-TiShi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "离开时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("离开时间:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            goto Label_leave_time;
        }

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-TiShi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "消灭时间", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("消灭时间:查找:成功,lposX:%ld, lposY:%ld", lposX, lposY);
            goto Label_die_time;
        }
        

        api_log_MsgDebug("离开时间:消灭时间:查找:失败");
        Delay(500);
    }
    
    //不可能走到这里
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("死循环:不可能:到这里");
        Delay(1000);
    }

Label_leave_time:
    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = walk_path[25];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    retry_npc_num = 0;
    
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
        
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua9.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("对话:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        if (retry_Label_leave_time > 1)
        {
            api_log_MsgDebug("对话:查找:失败:超时:跳转:等待:传出:副本:%d", retry_Label_leave_time);
            goto Label_term_copy;
        }

        retry_npc_num++;
        if (retry_npc_num > 5)
        {
            api_log_MsgDebug("对话:查找:失败:超时:重试:%d", retry_Label_leave_time);
            
            retry_Label_leave_time++;
            goto Label_leave_time;
        }
        
        api_log_MsgDebug("对话:查找:失败");
        Delay(500);
    }

    //点击对话
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "想要召唤隐藏", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("想要召唤隐藏:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("想要召唤隐藏:查找:失败");
        Delay(500);
    }

    //点击
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "想要和隐藏", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("想要和隐藏:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("想要和隐藏:查找:失败");
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "想要和隐藏", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("想要和隐藏:点击:成功");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("想要和隐藏:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    /////////////////////////////////////////////////////////////////////////////////
    // 步骤22
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "64:34";
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[64:34]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[64:34]:成功");

    //code = expirateBossFightTimeout(dmobj, ctxt, &fif_info_ex[2]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, (struct font_info_of_fight_t*)&fif_info_ex[2], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("打怪:超时:死亡:复活");
    goto Label_continue_Next_Cpy;
    
Label_die_time:
    //code = expirateBossFightTimeout(dmobj, ctxt, &fif_info_ex[2]);
    code = expirateFightTimeoutAuto(dmobj, ctxt, (struct font_info_of_fight_t*)&fif_info_ex[2], 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("打怪:超时:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("打怪:超时:死亡:复活");

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = walk_path[25];
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:%s:死亡:复活", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

Label_continue_Next_Cpy:
    /////////////////////////////////////////////////////////////////////////////////
    // 步骤24
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "64:31";
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[64:31]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[64:31]:成功");

    dmobj->KeyPressChar("9");
    Delay(500);
    dmobj->KeyUpChar("9");
    Delay(500);

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
        
        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua9.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "对话", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("对话:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("对话:查找:失败");
        Delay(500);
    }

    //点击查看
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-DuiHua7.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "只是做了该做的", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("只是做了该做的:查找:成功, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("只是做了该做的:查找:失败");
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "谢谢", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("谢谢:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("谢谢:查找:失败");
        Delay(500);
    }

    //点击谢谢
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

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "谢谢", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("谢谢:点击:成功");
            break;
        }
        
        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("谢谢:点击:失败, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

Label_term_copy:
    dmobj->MoveTo(5, 5);
    Delay(200);
        
    //出来到外头了
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->SetDict(0, FONT_EXPIRATE_DIR"font-MingZi1.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "古林群岛", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("古林群岛:查找:成功, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("古林群岛:查找:失败");
        Delay(500);
    }

    api_log_MsgDebug("副本:结束");
    api_log_MsgDebug("副本:结束:等待:27秒:开始");
    
    Delay_With_Terminate_Check(27, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    api_log_MsgDebug("副本:结束:等待:27秒:结束");
    
    pathofposxys = "152:326";
    pathOfPosXYs = CString(pathofposxys);
    code = expirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("寻路:前往:坐标:[152:326]:死亡:复活");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("寻路:前往:坐标:[152:326]:成功");
    return ERRNO_SUCCESS;
}



int api_TaskExPirateInit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    time_t start = time(NULL);
    
    api_log_MsgDebug("副本:ExPirate:初始化: hwnd:%ld, role:%d", hwnd, (int)role);
    api_PrintConfig(luaman_config);

    /* 绑定窗口 */
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("bind ExPirate hwnd failed, hwnd:%ld", hwnd);

        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("bind ExPirate hwnd success, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("lock ExPirate hwnd failed, hwnd:%ld", hwnd);

        return ERRNO_BINDWND;
    }

    /* 初始化告警时间初始值 */
    init_alarm_ctxt();
    
    api_log_MsgDebug("副本:ExPirate:初始化:完成, hwnd:%ld", hwnd);   
    return ERRNO_SUCCESS;
}

int api_TaskExPirateMain(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    int j = 0;
    
    api_log_MsgDebug("副本:ExPirate:主循环: hwnd:%ld, role:%d", hwnd, (int)role);

    for (j = 0; j < luaman_config->loop_num; j++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        api_log_MsgDebug("执行副本,j:%ld, loop_num:%d", j, luaman_config->loop_num);
        
        code = expirateTask(dmobj, ctxt);
        if (code)
        {
            api_log_MsgDebug("执行副本完毕,j:%ld, loop_num:%d", j, luaman_config->loop_num);
            break;
        }
    }

    api_log_MsgDebug("副本:ExPirate:主循环:结束, hwnd:%ld, role:%d", hwnd, (int)role);
    return ERRNO_SUCCESS;
}

int api_TaskExPirateExit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;

    api_log_MsgDebug("副本:ExPirate:结束: hwnd:%ld, role:%d", hwnd, (int)role);
    return ERRNO_SUCCESS;
}


