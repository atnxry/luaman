// ParamFireDlg.cpp : implementation file
//

#include "stdafx.h"
#include "luaman.h"
#include "ParamFireDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamFireDlg dialog
#ifdef HAVE_PARAM_FIRE_REIMPL
void CParam175Dlg::SetDirection(int direction)
{
    m_config.direction = direction;
    return;
}

void CParam175Dlg::SetRole(int role)
{
    m_config.role = (ROLE_E)role;
    return;
}

void CParam175Dlg::SetGroup(int grp)
{
    m_config.groupid = grp;
    return;
}

void CParam175Dlg::SetBlueMax(int bluemax)
{
    m_config.maxBlueFiled = bluemax;
    return;

}
void CParam175Dlg::SetBlueMin(int bluemin)
{
    m_config.minBlue = bluemin;
    return;
}
void CParam175Dlg::SetBlueThreshold(int bluethreshold)
{
    m_config.blueThreshold = bluethreshold;
    return;
}

void CParam175Dlg::SetServer(char* server)
{
    memcpy(m_config.alarmServer, server, strlen(server) + 1);
    return;
}
void CParam175Dlg::SetPort(int port)
{
    m_config.alarmPort = port;
    return;
}

void CParam175Dlg::SetHost(int host)
{
    m_config.alarmHostIndex = host;
    return;
}

void CParam175Dlg::SetLoopNum(unsigned int loopnum)
{
    m_config.loop_num = loopnum;
    return;
}

void CParam175Dlg::SetPalyWndIndex(unsigned int playWndIndexVal)
{
    m_config.playWndIndexVal = playWndIndexVal;
    return;
}

void CParam175Dlg::SetTransWait(int Wait2Sleep)
{
    m_config.Wait2Sleep = Wait2Sleep;
    return;
}

void CParam175Dlg::SetExitGameCopyFull(int exitGameCopyFull)
{
    m_config.exitGameCopyFull = exitGameCopyFull;
    return;
}

void CParam175Dlg::SetCallBabyDie(int callbabydie)
{
    m_config.callbabydie = callbabydie;
    return;
}

void CParam175Dlg::SetCleanPackageTime(int cleanPackageTime)
{
    m_config.cleanPackageTime = cleanPackageTime;
}

void CParam175Dlg::SetCleanPackageWhenStart(int cleanPackageWhenStart)
{
    m_config.cleanPackageWhenStart = cleanPackageWhenStart;
    return;
}

void CParam175Dlg::SetCleanPackage1(int cleanPackage1)
{
    if (cleanPackage1)
    {
        CLEAN_PACKAGE_1(m_config.cleanPackageX);
        return;
    }

    NO_CLEAN_PACKAGE_1(m_config.cleanPackageX);
    return;
}

void CParam175Dlg::SetCleanPackage2(int cleanPackage2)
{
    if (cleanPackage2)
    {
        CLEAN_PACKAGE_2(m_config.cleanPackageX);
        return;
    }

    NO_CLEAN_PACKAGE_2(m_config.cleanPackageX);
    return;
}

void CParam175Dlg::SetCleanPackage3(int cleanPackage3)
{
    if (cleanPackage3)
    {
        CLEAN_PACKAGE_3(m_config.cleanPackageX);
        return;
    }

    NO_CLEAN_PACKAGE_3(m_config.cleanPackageX);
    return;
}

void CParam175Dlg::SetCleanPackage4(int cleanPackage4)
{
    if (cleanPackage4)
    {
        CLEAN_PACKAGE_4(m_config.cleanPackageX);
        return;
    }

    NO_CLEAN_PACKAGE_4(m_config.cleanPackageX);
    return;
}

void CParam175Dlg::SetCleanPackage5(int cleanPackage5)
{
    if (cleanPackage5)
    {
        CLEAN_PACKAGE_5(m_config.cleanPackageX);
        return;
    }

    NO_CLEAN_PACKAGE_5(m_config.cleanPackageX);
    return;
}

void CParam175Dlg::SetCleanPackage(int cleanPackage)
{
    m_config.cleanPackageX = cleanPackage;
}

void CParam175Dlg::SetWaitCopytime(int waitCopytime)
{
    m_config.waitCopytime = waitCopytime;
}

void CParam175Dlg::SetBluebloodChecktime(int bluebloodChecktime)
{
    m_config.bluebloodChecktime = bluebloodChecktime;
}

void CParam175Dlg::SetBlockTimes(int blockTimes)
{
    m_config.blockTimes = blockTimes;
}


#endif

BOOL CParamFireDlg::OnInitDialog()
{
    char* dir[] = DIR_DESC_ARRAY();
    char* role[] = ROLE_DESC_ARRAY();
    char* group[] = GROUP_DESC_ARRAY();
    char* bluemax[] = BLUE_MAX_DESC_ARRAY();
    char* bluemin[] = BLUE_MIN_DESC_ARRAY();
    char* bluethreshold[] = BLUE_THRESHOLD_DESC_ARRAY();
    char* host[] = ALARM_HOST_DESC_ARRAY();
    char* trans_wait_time[] = TRANS_WAIT_TIME_ARRAY();
    char* paly_wnd_index_time[] = PALY_WND_INDEX_TIME_ARRAY();
    int j = 0;
    char buff[128] = {0};
    
    CDialog::OnInitDialog();

    m_config.taskType = TASK_TYPE_FIRE;

    m_combo_wait2sleep.Clear();
    for (j = 0; j < sizeof(trans_wait_time)/sizeof(trans_wait_time[0]); ++j)
    {
        m_combo_wait2sleep.InsertString(j, trans_wait_time[j]);
        if (m_config.Wait2Sleep == atoi(trans_wait_time[j]))
        {
            m_combo_wait2sleep.SetCurSel(j);
        }
    }

    m_combo_playWndIndexVal.Clear();
    for (j = 0; j < sizeof(paly_wnd_index_time)/sizeof(paly_wnd_index_time[0]); ++j)
    {
        m_combo_playWndIndexVal.InsertString(j, paly_wnd_index_time[j]);
        if (m_config.playWndIndexVal == atoi(paly_wnd_index_time[j]))
        {
            m_combo_playWndIndexVal.SetCurSel(j);
        }
    }
    
    m_combo_direction.Clear();
    for (j = 0; j < sizeof(dir)/sizeof(dir[0]); ++j)
    {
        m_combo_direction.InsertString(j, dir[j]);
    }
    
    m_combo_role.Clear();
    for (j = 0; j < sizeof(role)/sizeof(role[0]); ++j)
    {
        m_combo_role.InsertString(j, role[j]);
    }

    m_combo_group.Clear();
    for (j = 0; j < sizeof(group)/sizeof(group[0]); ++j)
    {
        m_combo_group.InsertString(j, group[j]);
    }

    m_combo_blue_max.Clear();
    for (j = 0; j < sizeof(bluemax)/sizeof(bluemax[0]); ++j)
    {
        m_combo_blue_max.InsertString(j, bluemax[j]);
        if (atoi(bluemax[j]) == m_config.maxBlueFiled)
        {
            m_combo_blue_max.SetCurSel(j);
        }
    }

    m_combo_blue_min.Clear();
    for (j = 0; j < sizeof(bluemin)/sizeof(bluemin[0]); ++j)
    {
        m_combo_blue_min.InsertString(j, bluemin[j]);
        if (atoi(bluemin[j]) == m_config.minBlue)
        {
            m_combo_blue_min.SetCurSel(j);
        }
    }
    
    m_combo_blue_threshold.Clear();
    for (j = 0; j < sizeof(bluethreshold)/sizeof(bluethreshold[0]); ++j)
    {
        m_combo_blue_threshold.InsertString(j, bluethreshold[j]);
        if (atoi(bluethreshold[j]) == m_config.blueThreshold)
        {
            m_combo_blue_threshold.SetCurSel(j);
        }
    }

    m_combo_alarm_host.Clear();
    for (j = 0; j < sizeof(host)/sizeof(host[0]); ++j)
    {
        m_combo_alarm_host.InsertString(j, host[j]);
    }

    m_combo_direction.SetCurSel(m_config.direction);
    m_combo_role.SetCurSel(m_config.role);
    m_combo_group.SetCurSel(m_config.groupid);
    m_combo_alarm_host.SetCurSel(m_config.alarmHostIndex);
    
    m_edit_alarm_server.SetWindowText(m_config.alarmServer);
    
    (void)_snprintf(buff, sizeof(buff), "%d", m_config.alarmPort);
    m_edit_alarm_port.SetWindowText(buff);

    (void)_snprintf(buff, sizeof(buff), "%u", m_config.loop_num);
    m_edit_task_loop_num.SetWindowText(buff);

    m_btn_exit_game.SetCheck(m_config.exitGameCopyFull);

    //新增
    //清包时间,小时
    char* cleanPackageTime[] = {"1", "2", "3", "4", "6", "8", "10", "12", "120"};
    m_combo_cleanPackageTime.Clear();
    for (j = 0; j < sizeof(cleanPackageTime)/sizeof(cleanPackageTime[0]); ++j)
    {
        m_combo_cleanPackageTime.InsertString(j, cleanPackageTime[j]);
        if (m_config.cleanPackageTime == atoi(cleanPackageTime[j]))
        {
            m_combo_cleanPackageTime.SetCurSel(j);
        }
    }

    m_btn_cleanPackageWhenStart.SetCheck(m_config.cleanPackageWhenStart?1:0);
    m_btn_callBabyDie.SetCheck(m_config.callbabydie?1:0);
    m_btn_cleanPackage1.SetCheck(CHECK_CLEAN_PACKAGE_1(m_config.cleanPackageX)?1:0);
    m_btn_cleanPackage2.SetCheck(CHECK_CLEAN_PACKAGE_2(m_config.cleanPackageX)?1:0);
    m_btn_cleanPackage3.SetCheck(CHECK_CLEAN_PACKAGE_3(m_config.cleanPackageX)?1:0);
    m_btn_cleanPackage4.SetCheck(CHECK_CLEAN_PACKAGE_4(m_config.cleanPackageX)?1:0);
    m_btn_cleanPackage5.SetCheck(CHECK_CLEAN_PACKAGE_5(m_config.cleanPackageX)?1:0);
    
    char* waitCopytime[] = {"18", "19", "20", "21", "22", "23", "24", "25", "26", "27",
        "28", "29", "30", "31", "32"};
    m_combo_waitCopytime.Clear();
    for (j = 0; j < sizeof(waitCopytime)/sizeof(waitCopytime[0]); ++j)
    {
        m_combo_waitCopytime.InsertString(j, waitCopytime[j]);
        if (m_config.waitCopytime == atoi(waitCopytime[j]))
        {
            m_combo_waitCopytime.SetCurSel(j);
        }
    }
    
    char* bluebloodChecktime[] = {"1", "2", "3", "4", "5", "6", "7",
        "8", "9", "10"};
    m_combo_bluebloodChecktime.Clear();
    for (j = 0; j < sizeof(bluebloodChecktime)/sizeof(bluebloodChecktime[0]); ++j)
    {
        m_combo_bluebloodChecktime.InsertString(j, bluebloodChecktime[j]);
        if (m_config.bluebloodChecktime == atoi(bluebloodChecktime[j]))
        {
            m_combo_bluebloodChecktime.SetCurSel(j);
        }
    }

    char* blockTimes[] = {"1", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "15", "20", "25", "30"};
    m_combo_blockTimes.Clear();
    for (j = 0; j < sizeof(blockTimes)/sizeof(blockTimes[0]); ++j)
    {
        m_combo_blockTimes.InsertString(j, blockTimes[j]);
        if (m_config.blockTimes == atoi(blockTimes[j]))
        {
            m_combo_blockTimes.SetCurSel(j);
        }
    }
    
    return TRUE;
}


CParamFireDlg::CParamFireDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParamFireDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParamFireDlg)
	//}}AFX_DATA_INIT
}


void CParamFireDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParamFireDlg)
	DDX_Control(pDX, IDC_EDIT_task_loop_num, m_edit_task_loop_num);
	DDX_Control(pDX, IDC_EDIT_alarm_server, m_edit_alarm_server);
	DDX_Control(pDX, IDC_EDIT_alarm_port, m_edit_alarm_port);
	DDX_Control(pDX, IDC_COMBO_waitCopytime, m_combo_waitCopytime);
	DDX_Control(pDX, IDC_COMBO_Wait2Sleep, m_combo_wait2sleep);
	DDX_Control(pDX, IDC_COMBO_role, m_combo_role);
	DDX_Control(pDX, IDC_COMBO_playWndIndexVal, m_combo_playWndIndexVal);
	DDX_Control(pDX, IDC_COMBO_group, m_combo_group);
	DDX_Control(pDX, IDC_COMBO_direction, m_combo_direction);
	DDX_Control(pDX, IDC_COMBO_cleanPackageTime, m_combo_cleanPackageTime);
	DDX_Control(pDX, IDC_COMBO_bluebloodChecktime, m_combo_bluebloodChecktime);
	DDX_Control(pDX, IDC_COMBO_blue_threshold, m_combo_blue_threshold);
	DDX_Control(pDX, IDC_COMBO_blue_min, m_combo_blue_min);
	DDX_Control(pDX, IDC_COMBO_blue_max, m_combo_blue_max);
	DDX_Control(pDX, IDC_COMBO_blockTimes, m_combo_blockTimes);
	DDX_Control(pDX, IDC_COMBO_alarm_host, m_combo_alarm_host);
	DDX_Control(pDX, IDC_CHECK_cleanPackageWhenStart, m_btn_cleanPackageWhenStart);
	DDX_Control(pDX, IDC_CHECK_cleanPackage5, m_btn_cleanPackage5);
	DDX_Control(pDX, IDC_CHECK_cleanPackage4, m_btn_cleanPackage4);
	DDX_Control(pDX, IDC_CHECK_cleanPackage3, m_btn_cleanPackage3);
	DDX_Control(pDX, IDC_CHECK_cleanPackage2, m_btn_cleanPackage2);
	DDX_Control(pDX, IDC_CHECK_cleanPackage1, m_btn_cleanPackage1);
	DDX_Control(pDX, IDC_CHECK_callBabyDie, m_btn_callBabyDie);
	DDX_Control(pDX, IDC_CHECK_exit_game, m_btn_exit_game);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParamFireDlg, CDialog)
	//{{AFX_MSG_MAP(CParamFireDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParamFireDlg message handlers

void CParamFireDlg::OnOK() 
{
	// TODO: Add extra validation here
	int j = 0;
    char* bluemax[] = BLUE_MAX_DESC_ARRAY();
    char* bluemin[] = BLUE_MIN_DESC_ARRAY();
    char* bluethreshold[] = BLUE_THRESHOLD_DESC_ARRAY();
    char* host[] = ALARM_HOST_DESC_ARRAY();
    char* trans_wait_time[] = TRANS_WAIT_TIME_ARRAY();
    char* paly_wnd_index_time[] = PALY_WND_INDEX_TIME_ARRAY();
    char buff[128] = {0};
    
	// TODO: Add extra validation here
    m_config.direction = m_combo_direction.GetCurSel();
    m_config.role = (ROLE_E)m_combo_role.GetCurSel();
    m_config.groupid = m_combo_group.GetCurSel();

    for (j = 0; j < sizeof(trans_wait_time)/sizeof(trans_wait_time[0]); ++j)
    {
        if (j == m_combo_wait2sleep.GetCurSel())
        {
            m_config.Wait2Sleep = atoi(trans_wait_time[j]);
            break;
        }
    }

    for (j = 0; j < sizeof(paly_wnd_index_time)/sizeof(paly_wnd_index_time[0]); ++j)
    {
        if (j == m_combo_playWndIndexVal.GetCurSel())
        {
            m_config.playWndIndexVal = atoi(paly_wnd_index_time[j]);
            break;
        }
    }
    
    for (j = 0; j < sizeof(bluemax)/sizeof(bluemax[0]); ++j)
    {
        if (j == m_combo_blue_max.GetCurSel())
        {
            m_config.maxBlueFiled = atoi(bluemax[j]);
            break;
        }
    }

    for (j = 0; j < sizeof(bluemin)/sizeof(bluemin[0]); ++j)
    {
        if (j == m_combo_blue_max.GetCurSel())
        {
            m_config.minBlue = atoi(bluemin[j]);
            break;
        }
    }

    for (j = 0; j < sizeof(bluethreshold)/sizeof(bluethreshold[0]); ++j)
    {
        if (j == m_combo_blue_threshold.GetCurSel())
        {
            m_config.blueThreshold = atoi(bluethreshold[j]);
            break;
        }
    }
    
    m_config.alarmHostIndex = m_combo_alarm_host.GetCurSel();
    
    m_edit_alarm_server.GetWindowText(buff, sizeof(buff));
    memcpy(m_config.alarmServer, buff, strlen(buff) + 1);

    memset(buff, 0, sizeof(buff));
    m_edit_alarm_port.GetWindowText(buff, sizeof(buff));
    m_config.alarmPort = atoi(buff);

    memset(buff, 0, sizeof(buff));
    m_edit_task_loop_num.GetWindowText(buff, sizeof(buff));
    m_config.loop_num = atoi(buff);

    m_config.exitGameCopyFull = m_btn_exit_game.GetCheck();

    //新增
    char* cleanPackageTime[] = {"1", "2", "3", "4", "6", "8", "10", "12", "120"};
    for (j = 0; j < sizeof(cleanPackageTime)/sizeof(cleanPackageTime[0]); ++j)
    {
        if (j == m_combo_cleanPackageTime.GetCurSel())
        {
            m_config.cleanPackageTime = atoi(cleanPackageTime[j]);
            break;
        }
    }

    m_config.cleanPackageWhenStart = m_btn_cleanPackageWhenStart.GetCheck();
    m_config.callbabydie = m_btn_callBabyDie.GetCheck();
    if (m_btn_cleanPackage1.GetCheck())
    {
        CLEAN_PACKAGE_1(m_config.cleanPackageX);
    }
    else
    {
        NO_CLEAN_PACKAGE_1(m_config.cleanPackageX);
    }

    if (m_btn_cleanPackage2.GetCheck())
    {
        CLEAN_PACKAGE_2(m_config.cleanPackageX);
    }
    else
    {
        NO_CLEAN_PACKAGE_2(m_config.cleanPackageX);
    }

    if (m_btn_cleanPackage3.GetCheck())
    {
        CLEAN_PACKAGE_3(m_config.cleanPackageX);
    }
    else
    {
        NO_CLEAN_PACKAGE_3(m_config.cleanPackageX);
    }

    if (m_btn_cleanPackage4.GetCheck())
    {
        CLEAN_PACKAGE_4(m_config.cleanPackageX);
    }
    else
    {
        NO_CLEAN_PACKAGE_4(m_config.cleanPackageX);
    }

    if (m_btn_cleanPackage5.GetCheck())
    {
        CLEAN_PACKAGE_5(m_config.cleanPackageX);
    }
    else
    {
        NO_CLEAN_PACKAGE_5(m_config.cleanPackageX);
    }

    char* waitCopytime[] = {"18", "19", "20", "21", "22", "23", "24", "25", "26", "27",
        "28", "29", "30", "31", "32"};
    for (j = 0; j < sizeof(waitCopytime)/sizeof(waitCopytime[0]); ++j)
    {
        if (j == m_combo_waitCopytime.GetCurSel())
        {
            m_config.waitCopytime = atoi(waitCopytime[j]);
            break;
        }
    }

    char* bluebloodChecktime[] = {"1", "2", "3", "4", "5", "6", "7",
        "8", "9", "10"};
    for (j = 0; j < sizeof(bluebloodChecktime)/sizeof(bluebloodChecktime[0]); ++j)
    {
        if (j == m_combo_bluebloodChecktime.GetCurSel())
        {
            m_config.bluebloodChecktime = atoi(bluebloodChecktime[j]);
            break;
        }
    }

    char* blockTimes[] = {"1", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "15", "20", "25", "30"};
    for (j = 0; j < sizeof(blockTimes)/sizeof(blockTimes[0]); ++j)
    {
        if (j == m_combo_blockTimes.GetCurSel())
        {
            m_config.blockTimes = atoi(blockTimes[j]);
            break;
        }
    }
    
	CDialog::OnOK();
}

void CParamFireDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
