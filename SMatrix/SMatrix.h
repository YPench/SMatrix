// SMatrix.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "SMatrixDlg.h"

// CSMatrixApp:
// See SMatrix.cpp for the implementation of this class
//

class CSMatrixApp : public CWinApp
{
public:
	CSMatrixApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
public:
	CSMatrixDlg*  pdlg;
};

extern CSMatrixApp theApp;