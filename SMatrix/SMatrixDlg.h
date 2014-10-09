// SMatrixDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "SMatrix\\CWSA.h"

// CSMatrixDlg dialog
class CSMatrixDlg : public CDialog
{
// Construction
public:
	CSMatrixDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SMATRIX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnResponceDisplay( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

	void Call_SMatrix(const char* inputchar);

public:
	CRichEditCtrl m_REditDiscourse;
	CRichEditCtrl m_REditResponse;
	CString m_Discourse;
	CString m_Response;

	CWSA m_CWSA;
	HANDLE ImpThread;
	DWORD ImpphreadId;
	HANDLE msgThread;
	DWORD msgphreadId;
	bool SMatrix_Busy_Flag;
	string m_Inputstr;
};
