#if !defined(AFX_PARAMSIGNINDIALOG_H__1C9211A0_3AC3_4A26_872E_BBB1DA72C31D__INCLUDED_)
#define AFX_PARAMSIGNINDIALOG_H__1C9211A0_3AC3_4A26_872E_BBB1DA72C31D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamSigninDialog.h : header file
//
#include "param.h"

/////////////////////////////////////////////////////////////////////////////
// CParamSigninDialog dialog

class CParamSigninDialog : public CDialog, public CParameters
{
// Construction
public:
	CParamSigninDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParamSigninDialog)
	enum { IDD = IDD_DIALOG_param_signin };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParamSigninDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParamSigninDialog)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMSIGNINDIALOG_H__1C9211A0_3AC3_4A26_872E_BBB1DA72C31D__INCLUDED_)
