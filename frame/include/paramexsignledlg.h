#if !defined(AFX_PARAMEXSIGNLEDLG_H__3FB372A0_730E_41B5_9C23_19FA3784773E__INCLUDED_)
#define AFX_PARAMEXSIGNLEDLG_H__3FB372A0_730E_41B5_9C23_19FA3784773E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamExsignleDlg.h : header file
//
#include "param.h"

/////////////////////////////////////////////////////////////////////////////
// CParamExsignleDlg dialog

class CParamExsignleDlg : public CDialog, public CParameters
{
// Construction
public:
	CParamExsignleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParamExsignleDlg)
	enum { IDD = IDD_DIALOG_param_exsingle };
	CEdit	m_edit_task_loop_num;
	CEdit	m_edit_alarm_server;
	CEdit	m_edit_alarm_port;
	CComboBox	m_combo_wait2sleep;
	CComboBox	m_combo_swjdIndex;
	CComboBox	m_combo_role;
	CComboBox	m_combo_playWndIndexVal;
	CComboBox	m_combo_group;
	CComboBox	m_combo_direction;
	CComboBox	m_combo_copyrolenr;
	CComboBox	m_combo_buddyName;
	CComboBox	m_combo_blue_threshold;
	CComboBox	m_combo_blue_min;
	CComboBox	m_combo_blue_max;
	CComboBox	m_combo_alarm_host;
	CButton	m_btn_exit_game;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParamExsignleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParamExsignleDlg)
	virtual void OnOK();
	virtual void OnCancel();
    virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMEXSIGNLEDLG_H__3FB372A0_730E_41B5_9C23_19FA3784773E__INCLUDED_)
