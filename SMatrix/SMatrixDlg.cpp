// SMatrixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMatrix.h"
#include "SMatrixDlg.h"
#include "SMCom.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI SMatrix_Implement_Threaad(LPVOID pParam);
DWORD WINAPI SMatrix_Responce_Threaad(LPVOID pParam);
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSMatrixDlg dialog




CSMatrixDlg::CSMatrixDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMatrixDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSMatrixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_REditDiscourse);
	DDX_Control(pDX, IDC_RICHEDIT22, m_REditResponse);
}

BEGIN_MESSAGE_MAP(CSMatrixDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(WM_SRESPONCE_MESSAGE, OnResponceDisplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CSMatrixDlg message handlers
LRESULT CSMatrixDlg::OnResponceDisplay(WPARAM wParam, LPARAM lParam)
{
	static string Prestr = "";
	m_Response = (char*)wParam;

	m_REditResponse.SetWindowTextW(m_Response);
	return 1;
}

BOOL CSMatrixDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SMatrix_Busy_Flag = false;
	msgThread = CreateThread(NULL, 0, SMatrix_Responce_Threaad, (LPVOID)this, 0, &msgphreadId);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
DWORD WINAPI SMatrix_Responce_Threaad(LPVOID pParam)
{
	CSMatrixDlg& dlgRef = *(CSMatrixDlg*)pParam;
	char CurputChar[MAX_SENTENCE];
	while(true){
		if(dlgRef.SMatrix_Busy_Flag){
			Sleep(3000);
			continue;
			strcpy_s(CurputChar, MAX_SENTENCE, "SMatrix is busy...");
		}
		else{
			strcpy_s(CurputChar, MAX_SENTENCE, "SMatrix is Waiting...");
		}
		::SendMessage(dlgRef.m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)CurputChar, 1);
		Sleep(1500);
		strcpy_s(CurputChar, MAX_SENTENCE, "");
		::SendMessage(dlgRef.m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)CurputChar, 1);
		Sleep(2000);
	}
	ExitThread(0);
}

void CSMatrixDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSMatrixDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSMatrixDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSMatrixDlg::PreTranslateMessage(MSG* pMsg)  
{  
	if(WM_KEYDOWN == pMsg->message )  //keyboard buttondown
	{  
		UINT nKey = (int) pMsg->wParam;
		if( VK_RETURN == nKey || VK_ESCAPE == nKey )
		{
			//-----------------------------------------------
			m_REditDiscourse.GetWindowText(m_Discourse);
			
			m_REditDiscourse.SetWindowTextW(_T(""));
			Call_SMatrix(NLPOP::CString2string(m_Discourse).c_str());
			m_REditResponse.SetWindowTextW(m_Response);

			return TRUE ; 
			//----------------------------------------------------
		}
	}
	return CDialog::PreTranslateMessage(pMsg);  
}

DWORD WINAPI SMatrix_Implement_Threaad(LPVOID pParam)
{
	CSMatrixDlg& dlgRef = *(CSMatrixDlg*)pParam;
	dlgRef.m_CWSA.Segmentation_Matrix_Port(dlgRef.m_Inputstr.c_str());
	dlgRef.SMatrix_Busy_Flag = false;
	ExitThread(0);
}
void CSMatrixDlg::Call_SMatrix(const char* inputchar)
{
	if(SMatrix_Busy_Flag){
		m_REditResponse.SetWindowTextW(_T("SMatrix is Busy..."));
		return;
	}
	m_Inputstr = inputchar;
	SMatrix_Busy_Flag = true;
	m_REditResponse.SetWindowTextW(_T("SMatrix is Started..."));

	ImpThread = CreateThread(NULL, 0, SMatrix_Implement_Threaad, (LPVOID)this, 0, &ImpphreadId);
}

