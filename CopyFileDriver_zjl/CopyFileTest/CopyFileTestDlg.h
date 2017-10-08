// CopyFileTestDlg.h : header file
//

#pragma once


// CCopyFileTestDlg dialog
class CCopyFileTestDlg : public CDialog
{
// Construction
public:
	CCopyFileTestDlg(CString szCommandLine, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_COPYFILETEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CString m_szCommandLine;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnInstallDriver();
	afx_msg void OnUninstallDriver();
	CString m_SourceFile;
	CString m_DestFile;
	afx_msg void OnBrowseSource();
	afx_msg void OnBrowseDest();
	afx_msg void OnCopyFile();
	afx_msg void OnReadFile();
	afx_msg void OnExit();
	afx_msg void OnGetFileSize();
};
