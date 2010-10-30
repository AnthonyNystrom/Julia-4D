#include "stdafx.h"

#include "about_dialog.h"
#include "defines.h"

#include <string>
#include <vector>
using std::string;
using std::vector;


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BOOL CAboutDlg::OnInitDialog()
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
	compile_info += "Copyright © 2004-2005 Shawn Halayka";
	compile_info += "\n";
	compile_info += "--------------------------------";
	compile_info += "\n";
	compile_info += "Shawn Halayka: Design and code";
	compile_info += "\n-\n";
	compile_info += "Lisa Halayka: Testing and feedback";
	compile_info += "\n-\n";
	compile_info += "Campbell Boucher-Burnet: Z Slicing idea";
	compile_info += "\n-\n";
	compile_info += "Godwin Vickers: Hopf map idea, configurable multiplication algorithm; \"4D matrix and column vector concept\"";
	compile_info += "\n-\n";
	compile_info += "Paul Bourke, Cory Gene Bloyd: OBJ tesselation algorithm, original code; \"Polygonising a Scalar Field\"";
	compile_info += "\n-\n";
	compile_info += "Caching system filename algorithm and code \"derived from the RSA Data Security, Inc. MD5 Message Digest Algorithm\"";
	compile_info += "\n-\n";
	compile_info += "Tony Ray: Jan 2001, Dominik Reichl, Sept 2002; \"Modified MD5 C++ code\"";
	compile_info += "\n-\n";
	compile_info += "Microsoft: Original arcball code";
	compile_info += "\n-\n";
	compile_info += "Ken Perlin: Perlin noise algorithm, original Perlin noise C source code";
	compile_info += "\n";
	compile_info += "--------------------------------";
	compile_info += "\n";
	compile_info += "All 3rd party provided source code is Copyright © 2004-2005 by their respective authors.";

	this->SetDlgItemText(IDC_COMPILE_INFO_STATIC, compile_info.c_str());


	vector<string> titles;
	titles.push_back("Hello World!");
	titles.push_back("New-and-Improved-Special-Hyper CRAZY COW EDITION(tm)!!");
	titles.push_back("Do you need a new hobby? Have you ever thought of trying SMASH?");
	titles.push_back("OINK-01 The Robotic Pig was here...");
	titles.push_back("I am Robot-Cat. I will eat your brain!");

	this->SetWindowText(titles[rand()%titles.size()].c_str());

	return TRUE;
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
