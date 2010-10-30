#ifndef DISK_SETTINGS_DLG_HEADER
#define DISK_SETTINGS_DLG_HEADER



#include "j4d2_cache_system.h"


#include "resource.h"





// CDiskSettingsDlg dialog used for App About

class CDiskSettingsDlg : public CDialog
{
public:
	CDiskSettingsDlg();

// Dialog Data
	enum { IDD = IDD_DISKSETTINGSBOX };

protected:
	j4d2_cache_system cache;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL OnInitDialog();

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedClearCacheButton();
	afx_msg void OnBnClickedLimitDiskUsageCheck();
	afx_msg void OnEnChangeCacheSizeEdit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancelButton();
	afx_msg void OnBnClickedEnableCacheReadsCheck();
	afx_msg void OnBnClickedEnableCacheWritesCheck();
};


#endif