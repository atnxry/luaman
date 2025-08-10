#if !defined(AFX_PARAM021DLG_H__9E3531F8_5513_4EC7_9111_A199D6DED2C4__INCLUDED_)
#define AFX_PARAM021DLG_H__9E3531F8_5513_4EC7_9111_A199D6DED2C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Param021Dlg.h : header file
#include "config.h"
#include "param.h"

/////////////////////////////////////////////////////////////////////////////
// CParam021Dlg dialog

class CParam021Dlg : public CDialog, public CParameters
{
public:
#ifdef HAVE_PARAM021_REIMPL
    //struct cluamanconfig m_config;
    //struct cluamanconfig* GetConfig(){return &m_config;}
    void SetDirection(int direction);
    void SetRole(int role);
    void SetGroup(int grp);
    void SetBlueMax(int bluemax);
    void SetBlueMin(int bluemin);
    void SetBlueThreshold(int bluethreshold);
    void SetServer(char* server);
    void SetPort(int port);
    void SetHost(int host);
    void SetLoopNum(unsigned int loopnum);
    void SetPalyWndIndex(unsigned int playWndIndexVal);
    void SetTransWait(int Wait2Sleep);
    void SetExitGameCopyFull(int exitGameCopyFull);
    void SetCleanPackageWhenStart(int cleanPackageWhenStart);
#endif

// Construction
public:
	CParam021Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParam021Dlg)
	enum { IDD = IDD_DIALOG_param_021 };
	CComboBox	m_combo_wait2sleep;
	CComboBox	m_combo_role;
	CComboBox	m_combo_playWndIndexVal;
	CEdit	m_edit_task_loop_num;
	CEdit	m_edit_alarm_server;
	CEdit	m_edit_alarm_port;
	CComboBox	m_combo_group;
	CComboBox	m_combo_direction;
	CComboBox	m_combo_blue_threshold;
	CComboBox	m_combo_blue_min;
	CComboBox	m_combo_blue_max;
	CComboBox	m_combo_alarm_host;
	CButton	m_btn_exit_game;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParam021Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParam021Dlg)
	virtual void OnOK();
	virtual void OnCancel();
    virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAM021DLG_H__9E3531F8_5513_4EC7_9111_A199D6DED2C4__INCLUDED_)
