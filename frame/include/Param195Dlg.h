#if !defined(AFX_PARAM195DLG_H__8B7DFC1E_262A_4511_9A11_5C6ED351D5C2__INCLUDED_)
#define AFX_PARAM195DLG_H__8B7DFC1E_262A_4511_9A11_5C6ED351D5C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Param195Dlg.h : header file
//

#include "config.h"
#include "param.h"

/////////////////////////////////////////////////////////////////////////////
// CParam195Dlg dialog

class CParam195Dlg : public CDialog, public CParameters
{
public:
#ifdef HAVE_PARAM195_REIMPL
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
#endif

// Construction
public:
	CParam195Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParam195Dlg)
	enum { IDD = IDD_DIALOG_param_195 };
	CEdit	m_edit_task_loop_num;
	CEdit	m_edit_alarm_server;
	CEdit	m_edit_alarm_port;
	CComboBox	m_combo_role;
	CComboBox	m_combo_group;
	CComboBox	m_combo_direction;
	CComboBox	m_combo_blue_threshold;
	CComboBox	m_combo_blue_min;
	CComboBox	m_combo_blue_max;
	CComboBox	m_combo_alarm_host;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParam195Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParam195Dlg)
	virtual void OnOK();
	virtual void OnCancel();
    virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONalarmtest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAM195DLG_H__8B7DFC1E_262A_4511_9A11_5C6ED351D5C2__INCLUDED_)
