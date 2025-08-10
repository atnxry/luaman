// ParamSigninDialog.cpp : implementation file
//

#include "stdafx.h"
#include "luaman.h"
#include "ParamSigninDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamSigninDialog dialog


CParamSigninDialog::CParamSigninDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CParamSigninDialog::IDD, pParent), CParameters()
{
	//{{AFX_DATA_INIT(CParamSigninDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_config.taskType = TASK_TYPE_SIGNIN;
}

BOOL CParamSigninDialog::OnInitDialog()
{
    m_config.taskType = TASK_TYPE_SIGNIN;
    return TRUE;
}


void CParamSigninDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParamSigninDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParamSigninDialog, CDialog)
	//{{AFX_MSG_MAP(CParamSigninDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParamSigninDialog message handlers
