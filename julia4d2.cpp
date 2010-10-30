// julia4d2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "julia4d2.h"
#include "about_dialog.h"
#include "MainFrm.h"

#include "julia4d2Doc.h"
#include "julia4d2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cjulia4d2App

BEGIN_MESSAGE_MAP(Cjulia4d2App, CWinApp)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// Cjulia4d2App construction

Cjulia4d2App::Cjulia4d2App()
{
}


// The one and only Cjulia4d2App object

Cjulia4d2App theApp;

// Cjulia4d2App initialization

BOOL Cjulia4d2App::InitInstance()
{
	if(GetSystemMetrics(SM_CXSCREEN) < 1024 || GetSystemMetrics(SM_CYSCREEN) < 768)
	{
		MessageBox(0, "Julia 4D 2 requires 1024x768 screen resolution, or better.\n\nThe application will now exit.", APP_NAME, MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	SetRegistryKey(_T("Shawn Halayka"));

	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views

	CSingleDocTemplate* pDocTemplate;

	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Cjulia4d2Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(Cjulia4d2View));

	if (!pDocTemplate)
		return FALSE;

	AddDocTemplate(pDocTemplate);
	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;



	//MessageBox(NULL, cmdInfo.m_strFileName.GetBuffer(), APP_NAME, MB_OK);




	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW/*MAXIMIZED*/);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}





// Cjulia4d2App message handlers

