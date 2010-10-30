#ifndef LICENSE_DLG_HEADER
#define LICENSE_DLG_HEADER




#include "resource.h"





// CLicenseDlg dialog used for App About

class CLicenseDlg : public CDialog
{
public:
	CLicenseDlg();

// Dialog Data
	enum { IDD = IDD_LICENSEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

#endif