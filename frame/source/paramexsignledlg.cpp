// ParamExsignleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "luaman.h"
#include "ParamExsignleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamExsignleDlg dialog
BOOL CParamExsignleDlg::OnInitDialog()
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
    char* buddyName[] = BUDDY_NAME_ARRAY();
    char* swjdIndex[] = SWJD_INDEX_ARRAY();
    
    CDialog::OnInitDialog();

    m_config.taskType = TASK_TYPE_EXSINGLE;

    m_combo_copyrolenr.Clear();
    for (j = 0; j < EXSINGLE_COPYROLENR_MAX; j++)
    {
        char strnr[64] = {0};
        (void)_snprintf(strnr, sizeof(strnr) - 1, "%d", j + 1);
        m_combo_copyrolenr.InsertString(j, strnr);
    }

    m_combo_buddyName.Clear();
    for (j = 0; j < sizeof(buddyName)/sizeof(buddyName[0]); ++j)
    {
        m_combo_buddyName.InsertString(j, buddyName[j]);
        if (!strcmp(m_config.buddyName, buddyName[j]))
        {
            m_combo_buddyName.SetCurSel(j);
        }
    }

    m_combo_swjdIndex.Clear();
    for (j = 0; j < sizeof(swjdIndex)/sizeof(swjdIndex[0]); ++j)
    {
        m_combo_swjdIndex.InsertString(j, swjdIndex[j]);
        if (m_config.swjd_index + 1 == atoi(swjdIndex[j]))
        {
            m_combo_swjdIndex.SetCurSel(j);
        }
    }
    
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

    m_combo_copyrolenr.SetCurSel(m_config.copyrolenr);
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
    
    return TRUE;
}


CParamExsignleDlg::CParamExsignleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParamExsignleDlg::IDD, pParent), CParameters()
{
	//{{AFX_DATA_INIT(CParamExsignleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CParamExsignleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParamExsignleDlg)
	DDX_Control(pDX, IDC_EDIT_task_loop_num, m_edit_task_loop_num);
	DDX_Control(pDX, IDC_EDIT_alarm_server, m_edit_alarm_server);
	DDX_Control(pDX, IDC_EDIT_alarm_port, m_edit_alarm_port);
	DDX_Control(pDX, IDC_COMBO_Wait2Sleep, m_combo_wait2sleep);
	DDX_Control(pDX, IDC_COMBO_swjdIndex, m_combo_swjdIndex);
	DDX_Control(pDX, IDC_COMBO_role, m_combo_role);
	DDX_Control(pDX, IDC_COMBO_playWndIndexVal, m_combo_playWndIndexVal);
	DDX_Control(pDX, IDC_COMBO_group, m_combo_group);
	DDX_Control(pDX, IDC_COMBO_direction, m_combo_direction);
	DDX_Control(pDX, IDC_COMBO_copyrolenr, m_combo_copyrolenr);
	DDX_Control(pDX, IDC_COMBO_buddyName, m_combo_buddyName);
	DDX_Control(pDX, IDC_COMBO_blue_threshold, m_combo_blue_threshold);
	DDX_Control(pDX, IDC_COMBO_blue_min, m_combo_blue_min);
	DDX_Control(pDX, IDC_COMBO_blue_max, m_combo_blue_max);
	DDX_Control(pDX, IDC_COMBO_alarm_host, m_combo_alarm_host);
	DDX_Control(pDX, IDC_CHECK_exit_game, m_btn_exit_game);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParamExsignleDlg, CDialog)
	//{{AFX_MSG_MAP(CParamExsignleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParamExsignleDlg message handlers

void CParamExsignleDlg::OnOK() 
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
    char* buddyName[] = BUDDY_NAME_ARRAY();
    
	// TODO: Add extra validation here
	m_config.copyrolenr = m_combo_copyrolenr.GetCurSel();;
    m_config.direction = m_combo_direction.GetCurSel();
    m_config.role = (ROLE_E)m_combo_role.GetCurSel();
    m_config.groupid = m_combo_group.GetCurSel();

    m_config.swjd_index = m_combo_swjdIndex.GetCurSel();
    memcpy(m_config.buddyName, buddyName[m_combo_buddyName.GetCurSel()], strlen(buddyName[m_combo_buddyName.GetCurSel()]) + 1);

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
    
	CDialog::OnOK();
}


void CParamExsignleDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
