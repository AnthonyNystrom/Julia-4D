#include "stdafx.h"

#include "disk_settings_dialog.h"
#include "defines.h"

#include ".\disk_settings_dialog.h"

#include <string>
#include <vector>
#include <sstream>

using std::string;
using std::vector;
using std::istringstream;

CDiskSettingsDlg::CDiskSettingsDlg() : CDialog(CDiskSettingsDlg::IDD)
{
}

void CDiskSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BOOL CDiskSettingsDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog())
		return FALSE;

	if(cache.CanClear())
		GetDlgItem(IDC_CLEAR_CACHE_BUTTON)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_CLEAR_CACHE_BUTTON)->EnableWindow(FALSE);

	long signed int EnableReads = 0;
	long signed int EnableWrites = 0;
	long signed int EnableSurfaceReads = 0;
	long signed int EnableSurfaceWrites = 0;
	long signed int LimitDiskUsage = 0;
	long unsigned int *SizeBuf = 0;
	long unsigned int Size = 0;
	UINT SizeBytesRead = 0;

	EnableReads = AfxGetApp()->GetProfileInt("Cache", "EnableReads", 0);
	EnableWrites = AfxGetApp()->GetProfileInt("Cache", "EnableWrites", 0);
	EnableSurfaceReads = AfxGetApp()->GetProfileInt("Cache", "EnableSurfaceReads", 0);
	EnableSurfaceWrites = AfxGetApp()->GetProfileInt("Cache", "EnableSurfaceWrites", 0);
	LimitDiskUsage = AfxGetApp()->GetProfileInt("Cache", "LimitDiskUsage", 0);

	if(!AfxGetApp()->GetProfileBinary("Cache", "LimitSize", (LPBYTE*)&SizeBuf, &SizeBytesRead) || SizeBytesRead != sizeof(Size))
	{
		Size = 0;
	}
	else
	{
		Size = *SizeBuf;
	}

	if(0 != SizeBuf)
		delete [] SizeBuf;



	if(EnableReads == 1)
	{
		GetDlgItem(IDC_ENABLE_CACHE_READS_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);

		if(EnableSurfaceReads == 1)
			GetDlgItem(IDC_ENABLE_CACHE_SURFACE_READS_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
		else
			GetDlgItem(IDC_ENABLE_CACHE_SURFACE_READS_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
	}
	else
	{
		GetDlgItem(IDC_ENABLE_CACHE_READS_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		GetDlgItem(IDC_ENABLE_CACHE_SURFACE_READS_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		GetDlgItem(IDC_ENABLE_CACHE_SURFACE_READS_CHECK)->EnableWindow(FALSE);
	}

	if(EnableWrites == 1)
	{
		GetDlgItem(IDC_ENABLE_CACHE_WRITES_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);

		if(EnableSurfaceWrites == 1)
			GetDlgItem(IDC_ENABLE_CACHE_SURFACE_WRITES_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
		else
			GetDlgItem(IDC_ENABLE_CACHE_SURFACE_WRITES_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
	}
	else
	{
		GetDlgItem(IDC_ENABLE_CACHE_WRITES_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		GetDlgItem(IDC_ENABLE_CACHE_SURFACE_WRITES_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		GetDlgItem(IDC_ENABLE_CACHE_SURFACE_WRITES_CHECK)->EnableWindow(FALSE);
	}








	if(LimitDiskUsage == 1)
	{
		GetDlgItem(IDC_CACHE_SIZE_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIMIT_DISK_USAGE_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);

		ostringstream out;
		out << Size;

		SetDlgItemText(IDC_CACHE_SIZE_EDIT, out.str().c_str());	
	}
	else
	{
		wstring cache_path = get_module_path_wide();

		if(cache_path != L"")
		{
			cache_path += L"\\";
			cache_path += J4D2_CACHE_FOLDER_NAME;
		}

		if(cache_path != L"")
		{
			cache_file_info_collection fcol(cache_path);

			LARGE_INTEGER file_size = fcol.total_size_in_bytes;
			file_size.QuadPart /= BYTES_PER_MEGABYTE;

			ostringstream out;
			out << file_size.LowPart;

			SetDlgItemText(IDC_CACHE_SIZE_EDIT, out.str().c_str());

			GetDlgItem(IDC_CACHE_SIZE_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_LIMIT_DISK_USAGE_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		}
	}

	return TRUE;
}


BEGIN_MESSAGE_MAP(CDiskSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_CLEAR_CACHE_BUTTON, OnBnClickedClearCacheButton)
	ON_BN_CLICKED(IDC_LIMIT_DISK_USAGE_CHECK, OnBnClickedLimitDiskUsageCheck)
	ON_EN_CHANGE(IDC_CACHE_SIZE_EDIT, OnEnChangeCacheSizeEdit)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL_BUTTON, OnBnClickedCancelButton)
	ON_BN_CLICKED(IDC_ENABLE_CACHE_READS_CHECK, OnBnClickedEnableCacheReadsCheck)
	ON_BN_CLICKED(IDC_ENABLE_CACHE_WRITES_CHECK, OnBnClickedEnableCacheWritesCheck)
END_MESSAGE_MAP()


void CDiskSettingsDlg::OnBnClickedClearCacheButton()
{
	MessageBeep(MB_ICONINFORMATION);
	if(IDNO == MessageBox("Are you sure that you want to clear the entire History & Cache?\n\nThis will delete all cached settings and calculations that have been saved up to this point.", APP_NAME, MB_YESNO|MB_ICONQUESTION))
		return;

	if(cache.CanClear())
	{
		cache.PerformClear();
	}

	GetDlgItem(IDC_CLEAR_CACHE_BUTTON)->EnableWindow(FALSE);
}

void CDiskSettingsDlg::OnBnClickedLimitDiskUsageCheck()
{
	if(GetDlgItem(IDC_LIMIT_DISK_USAGE_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		GetDlgItem(IDC_CACHE_SIZE_EDIT)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_CACHE_SIZE_EDIT)->EnableWindow(FALSE);
}


void CDiskSettingsDlg::OnEnChangeCacheSizeEdit()
{
	CString buf;
	GetDlgItemText(IDC_CACHE_SIZE_EDIT, buf);

	if(buf.IsEmpty())
		buf = "0";

	string final_buf = buf.GetBuffer();

	if(!is_long_unsigned_int(final_buf))
	{
		long unsigned int *SizeBuf = 0;
		long unsigned int Size = 0;
		UINT SizeBytesRead = 0;

		if(!AfxGetApp()->GetProfileBinary("Cache", "LimitSize", (LPBYTE*)&SizeBuf, &SizeBytesRead) || SizeBytesRead != sizeof(Size))
		{
			Size = 0;
		}
		else
		{
			Size = *SizeBuf;
		}

		if(0 != SizeBuf)
			delete [] SizeBuf;

		ostringstream out;
		out << Size;

		SetDlgItemText(IDC_CACHE_SIZE_EDIT, out.str().c_str());

		DWORD SelStart = 0;
		DWORD SelEnd = 0;
		GetDlgItem(IDC_CACHE_SIZE_EDIT)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
		GetDlgItem(IDC_CACHE_SIZE_EDIT)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

		return;
	}
}

void CDiskSettingsDlg::OnBnClickedOk()
{
	if(GetDlgItem(IDC_ENABLE_CACHE_READS_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		AfxGetApp()->WriteProfileInt("Cache", "EnableReads", 1);
	else
		AfxGetApp()->WriteProfileInt("Cache", "EnableReads", 0);

	if(GetDlgItem(IDC_ENABLE_CACHE_WRITES_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		AfxGetApp()->WriteProfileInt("Cache", "EnableWrites", 1);
	else
		AfxGetApp()->WriteProfileInt("Cache", "EnableWrites", 0);

	if(GetDlgItem(IDC_ENABLE_CACHE_SURFACE_READS_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		AfxGetApp()->WriteProfileInt("Cache", "EnableSurfaceReads", 1);
	else
		AfxGetApp()->WriteProfileInt("Cache", "EnableSurfaceReads", 0);

	if(GetDlgItem(IDC_ENABLE_CACHE_SURFACE_WRITES_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		AfxGetApp()->WriteProfileInt("Cache", "EnableSurfaceWrites", 1);
	else
		AfxGetApp()->WriteProfileInt("Cache", "EnableSurfaceWrites", 0);



	if(GetDlgItem(IDC_LIMIT_DISK_USAGE_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
	{
		AfxGetApp()->WriteProfileInt("Cache", "LimitDiskUsage", 1);

		CString buf;
		GetDlgItemText(IDC_CACHE_SIZE_EDIT, buf);

		if(buf.IsEmpty())
			buf = "0";

		string final_buf = buf.GetBuffer();

		istringstream in;
		in.str(final_buf.c_str());
		long unsigned int Size = 0;
		in >> Size;

		AfxGetApp()->WriteProfileBinary("Cache", "LimitSize", (LPBYTE)&Size, sizeof(Size));
	}
	else
	{
		AfxGetApp()->WriteProfileInt("Cache", "LimitDiskUsage", 0);
	}

	cache.PerformLimitTrim();

	OnOK();
}

void CDiskSettingsDlg::OnBnClickedCancelButton()
{
	OnOK();
}

void CDiskSettingsDlg::OnBnClickedEnableCacheReadsCheck()
{
	if(BST_CHECKED == GetDlgItem(IDC_ENABLE_CACHE_READS_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
	{
		GetDlgItem(IDC_ENABLE_CACHE_SURFACE_READS_CHECK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ENABLE_CACHE_SURFACE_READS_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		GetDlgItem(IDC_ENABLE_CACHE_SURFACE_READS_CHECK)->EnableWindow(FALSE);
	}
}

void CDiskSettingsDlg::OnBnClickedEnableCacheWritesCheck()
{
	if(BST_CHECKED == GetDlgItem(IDC_ENABLE_CACHE_WRITES_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
	{
		GetDlgItem(IDC_ENABLE_CACHE_SURFACE_WRITES_CHECK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ENABLE_CACHE_SURFACE_WRITES_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		GetDlgItem(IDC_ENABLE_CACHE_SURFACE_WRITES_CHECK)->EnableWindow(FALSE);
	}
}
