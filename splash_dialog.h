#ifndef SPLASH_DLG_HEADER
#define SPLASH_DLG_HEADER


#include "resource.h"

// CSplashDlg dialog used for App About

class CSplashDlg : public CDialog
{
public:
	CSplashDlg();

// Dialog Data
	enum { IDD = IDD_SPLASHSCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL OnInitDialog();

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};


#endif