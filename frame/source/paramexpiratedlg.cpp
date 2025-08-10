// ParamExPirateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "luaman.h"
#include "config.h"
#include "ParamExPirateDlg.h"
#include "alarm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ParamExPirateDlg dialog


ParamExPirateDlg::ParamExPirateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ParamExPirateDlg::IDD, pParent), CParameters()
{
	//{{AFX_DATA_INIT(ParamExPirateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


#ifdef HAVE_PARAM_EXPIRATE_REIMPL

void ParamExPirateDlg::SetDirection(int direction)
{
    m_config.direction = direction;
    return;
}

void ParamExPirateDlg::SetRole(int role)
{
    m_config.role = (ROLE_E)role;
    return;
}

void ParamExPirateDlg::SetGroup(int grp)
{
    m_config.groupid = grp;
    return;
}

void ParamExPirateDlg::SetBlueMax(int bluemax)
{
    m_config.maxBlueFiled = bluemax;
    return;

}
void ParamExPirateDlg::SetBlueMin(int bluemin)
{
    m_config.minBlue = bluemin;
    return;
}
void ParamExPirateDlg::SetBlueThreshold(int bluethreshold)
{
    m_config.blueThreshold = bluethreshold;
    return;
}

void ParamExPirateDlg::SetServer(char* server)
{
    memcpy(m_config.alarmServer, server, strlen(server) + 1);
    return;
}
void ParamExPirateDlg::SetPort(int port)
{
    m_config.alarmPort = port;
    return;
}

void ParamExPirateDlg::SetHost(int host)
{
    m_config.alarmHostIndex = host;
    return;
}

void ParamExPirateDlg::SetLoopNum(unsigned int loopnum)
{
    m_config.loop_num = loopnum;
    return;
}
#endif


BOOL ParamExPirateDlg::OnInitDialog()
{
    char* dir[] = DIR_DESC_ARRAY();
    char* role[] = ROLE_DESC_ARRAY();
    char* group[] = GROUP_DESC_ARRAY();
    char* bluemax[] = BLUE_MAX_DESC_ARRAY();
    char* bluemin[] = BLUE_MIN_DESC_ARRAY();
    char* bluethreshold[] = BLUE_THRESHOLD_DESC_ARRAY();
    char* host[] = ALARM_HOST_DESC_ARRAY();
    int j = 0;
    char buff[128] = {0};
    
    CDialog::OnInitDialog();

    m_config.taskType = TASK_TYPE_EXPIRATE;

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
    
    return TRUE;
}


void ParamExPirateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ParamExPirateDlg)
	DDX_Control(pDX, IDC_EDIT_task_loop_num, m_edit_task_loop_num);
	DDX_Control(pDX, IDC_EDIT_alarm_server, m_edit_alarm_server);
	DDX_Control(pDX, IDC_EDIT_alarm_port, m_edit_alarm_port);
	DDX_Control(pDX, IDC_COMBO_role, m_combo_role);
	DDX_Control(pDX, IDC_COMBO_group, m_combo_group);
	DDX_Control(pDX, IDC_COMBO_direction, m_combo_direction);
	DDX_Control(pDX, IDC_COMBO_blue_threshold, m_combo_blue_threshold);
	DDX_Control(pDX, IDC_COMBO_blue_min, m_combo_blue_min);
	DDX_Control(pDX, IDC_COMBO_blue_max, m_combo_blue_max);
	DDX_Control(pDX, IDC_COMBO_alarm_host, m_combo_alarm_host);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ParamExPirateDlg, CDialog)
	//{{AFX_MSG_MAP(ParamExPirateDlg)
    ON_BN_CLICKED(IDC_BUTTON_alarm_test, OnBUTTONalarmtest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ParamExPirateDlg message handlers

void ParamExPirateDlg::OnOK() 
{
	// TODO: Add extra validation here
	int j = 0;
    char* bluemax[] = BLUE_MAX_DESC_ARRAY();
    char* bluemin[] = BLUE_MIN_DESC_ARRAY();
    char* bluethreshold[] = BLUE_THRESHOLD_DESC_ARRAY();
    char* host[] = ALARM_HOST_DESC_ARRAY();
    char buff[128] = {0};
    
	// TODO: Add extra validation here
    m_config.direction = m_combo_direction.GetCurSel();
    m_config.role = (ROLE_E)m_combo_role.GetCurSel();
    m_config.groupid = m_combo_group.GetCurSel();
    
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
    
	CDialog::OnOK();
}

void ParamExPirateDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void ParamExPirateDlg::OnBUTTONalarmtest() 
{
	// TODO: Add your control notification handler code here
	int status = 0;
    char alarmServer[32] = {0};
    int alarmPort = 0;
    int alarmHostIndex = 0;
    char buff[128] = {0};

    alarmHostIndex = m_combo_alarm_host.GetCurSel();
    
    m_edit_alarm_server.GetWindowText(buff, sizeof(buff));
    memcpy(alarmServer, buff, strlen(buff) + 1);

    memset(buff, 0, sizeof(buff));
    m_edit_alarm_port.GetWindowText(buff, sizeof(buff));
    alarmPort = atoi(buff);

    status = alloc_alarm_ctxt();
    if (status)
    {
        api_log_MsgDebug("alarm test: alloc_alarm_ctxt failed");
        return;
    }

    init_alarm_ctxt();
    if (check_alarm_timeout(0))
    {
        alarm_report(alarmServer, alarmPort, alarmHostIndex);
    }
    
    free_alarm_ctxt();
    return;
}