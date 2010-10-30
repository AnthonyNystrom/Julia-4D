#include "stdafx.h"

#include "splash_dialog.h"
#include "defines.h"

#include <string>
#include <vector>
using std::string;
using std::vector;


CSplashDlg::CSplashDlg() : CDialog(CSplashDlg::IDD)
{
}

void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BOOL CSplashDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog())
		return FALSE;

	string compile_title;
	compile_title += APP_NAME;
	compile_title += " ";
	compile_title += "v";
	compile_title += APP_VERSION;
	compile_title += " - ";
	compile_title += __DATE__;
	compile_title += " ";
	compile_title += __TIME__;

	this->SetDlgItemText(IDC_COMPILE_TITLE_STATIC, compile_title.c_str());

	string compile_info;
	compile_info += "Thank you for using Julia 4D 2!";
	compile_info += "\n\n";
	compile_info += "Please feel free to leave feedback, or report bugs to shalayka@sasktel.net. Your participation is greatly appreciated.";
	compile_info += "\n\n";
	compile_info += "Please see Help > Online Help for detailed information on how to use Julia 4D 2...";

	this->SetDlgItemText(IDC_WELCOME_INFO_STATIC, compile_info.c_str());

	return TRUE;
}


BEGIN_MESSAGE_MAP(CSplashDlg, CDialog)
END_MESSAGE_MAP()
