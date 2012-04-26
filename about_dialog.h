#ifndef ABOUT_DLG_HEADER
#define ABOUT_DLG_HEADER


#include "resource.h"

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL OnInitDialog();

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};


#endif