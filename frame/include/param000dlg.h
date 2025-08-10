#if !defined(AFX_PARAM000DLG_H__760C063A_C741_4323_95CF_97E338B0DAA1__INCLUDED_)
#define AFX_PARAM000DLG_H__760C063A_C741_4323_95CF_97E338B0DAA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Param000Dlg.h : header file
//
#include "config.h"
#include "param.h"

/////////////////////////////////////////////////////////////////////////////
// CParam000Dlg dialog

class CParam000Dlg : public CDialog, public CParameters
{
public:
#ifdef HAVE_PARAM000_REIMPL
    virtual int SetConfig(struct cluamanconfig* config);
#endif

// Construction
public:
	CParam000Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParam000Dlg)
	enum { IDD = IDD_DIALOG_param_000 };
	CComboBox	m_combo_bindHostIdx;
	CButton	m_btn_waitEmail;
	CListBox	m_lstbox_TaskTypeList;
	CEdit	m_edit_TaskLoopNum;
	CComboBox	m_combo_WndList;
	CComboBox	m_combo_TaskList;
	CButton	m_btn_UpTask;
	CButton	m_btn_DownTask;
	CButton	m_btn_SetTaskConf;
	CButton	m_btn_SaveConf;
	CButton	m_btn_DelTaskType;
	CButton	m_btn_AddTaskType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParam000Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParam000Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONaddtasktype();
	afx_msg void OnBUTTONdeltasktype();
	afx_msg void OnBUTTONsettaskconf();
	afx_msg void OnBUTTONsaveconf();
	afx_msg void OnBUTTONtasktypeup();
	afx_msg void OnBUTTONtasktypedn();
	afx_msg void OnSelchangeCOMBObindwnd();
	afx_msg void OnCheckWaitEmail();
	afx_msg void OnBUTTONimportaccounts();
	afx_msg void OnSelchangeCOMBObindhostidx();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAM000DLG_H__760C063A_C741_4323_95CF_97E338B0DAA1__INCLUDED_)
