// julia4d2.h : main header file for the julia4d2 application
//
#ifndef JULIA4D2_H
#define JULIA4D2_H






#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif





#include "resource.h"       // main symbols
#include "defines.h"

// Cjulia4d2App:
// See julia4d2.cpp for the implementation of this class
//

class Cjulia4d2App : public CWinApp
{
public:
	Cjulia4d2App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	DECLARE_MESSAGE_MAP()
};

extern Cjulia4d2App theApp;



#endif