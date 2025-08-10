#if !defined(AFX_PARAMFIGHTDLG_H__8EFD18E3_069C_497D_9315_F18A3618A34A__INCLUDED_)
#define AFX_PARAMFIGHTDLG_H__8EFD18E3_069C_497D_9315_F18A3618A34A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamFightDlg.h : header file
//
#include "config.h"
#include "param.h"

/////////////////////////////////////////////////////////////////////////////
// CParamFightDlg dialog

class CParamFightDlg : public CDialog, public CParameters
{
// Construction
public:
	CParamFightDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParamFightDlg)
	enum { IDD = IDD_DIALOG_param_fight };
	CEdit	m_edit_task_loop_num;
	CEdit	m_edit_alarm_port;
	CEdit	m_edit_alarm_server;
	CComboBox	m_combo_wait2sleep;
	CComboBox	m_combo_role;
	CComboBox	m_combo_playWndIndexVal;
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
	//{{AFX_VIRTUAL(CParamFightDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParamFightDlg)
	virtual void OnOK();
	virtual void OnCancel();
    virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMFIGHTDLG_H__8EFD18E3_069C_497D_9315_F18A3618A34A__INCLUDED_)
