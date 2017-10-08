// CopyFileTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CopyFileTest.h"
#include "CopyFileTestDlg.h"

#include <Shlwapi.h>

#include "..\CopyFileWrapper\CopyFileWrapper.h"
#include "..\CopyFileDriver\User.h"

#ifdef _DEBUG
#pragma comment(lib, "CopyFileWrapper.lib")
#pragma message("###############Debug")
#else
#pragma comment(lib, "CopyFileWrapper.lib")
#pragma message("###############Release")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern int logger(CString logstr);

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


// CCopyFileTestDlg dialog




CCopyFileTestDlg::CCopyFileTestDlg(CString szCommandLine, CWnd* pParent /*=NULL*/)
	: CDialog(CCopyFileTestDlg::IDD, pParent), m_szCommandLine(szCommandLine)
	, m_SourceFile(_T(""))
	, m_DestFile(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_SourceFile = _T("C:\\WINDOWS\\system32\\config\\software");
	m_DestFile = _T("C:\\software_regedit");
}

void CCopyFileTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SOURCE_FILE, m_SourceFile);
	DDX_Text(pDX, IDC_DEST_FILE, m_DestFile);
}

BEGIN_MESSAGE_MAP(CCopyFileTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_INSTALL_DRIVER, OnInstallDriver)
	ON_BN_CLICKED(IDC_UNINSTALL_DRIVER, OnUninstallDriver)
	ON_BN_CLICKED(IDC_BROWSE_SOURCE, OnBrowseSource)
	ON_BN_CLICKED(IDC_BROWSE_DEST, OnBrowseDest)
	ON_BN_CLICKED(IDC_COPY_FILE, OnCopyFile)
	ON_BN_CLICKED(IDC_READ_FILE, OnReadFile)
	ON_BN_CLICKED(IDCANCEL, OnExit)
	ON_BN_CLICKED(IDC_GET_FILE_SIZE, OnGetFileSize)
END_MESSAGE_MAP()

typedef BOOL (__stdcall* FNKernelGetFileSize)(IN HANDLE FileHandle,
											  IN OUT LONGLONG *pFileSize);

typedef HANDLE (__stdcall* FNKernelOpenFile)(IN LPCSTR FileName);

FNKernelGetFileSize g_fnKernelGetFileSize = NULL;
FNKernelOpenFile g_fnKernelOpenFile = NULL;

void GetRealBackupFuncAddress()
{
	DWORD		dwError;	
	HMODULE		hModule = NULL;
	CHAR		szFilePath[MAX_PATH] = {0};
	CString logstr;

	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	PathRemoveFileSpec(szFilePath);
	PathAppend(szFilePath, "CopyFileWrapper.dll");
	hModule = LoadLibrary(szFilePath);
	if (hModule == NULL)
	{
		dwError = GetLastError();
		TRACE("dwError = %d\n", dwError);
		AfxMessageBox(_T("Load CopyFileWrapper.dll failed!"));
		TerminateProcess(GetCurrentProcess(), 0);
	}
	else
	{
		logstr.Format("注意: %s存在, 加载成功" , "CopyFileWrapper.dll");
		logger(logstr);
	}

 	g_fnKernelGetFileSize = (FNKernelGetFileSize) GetProcAddress(hModule, "KernelGetFileSize");
	g_fnKernelOpenFile = (FNKernelOpenFile) GetProcAddress(hModule, "KernelOpenFile");

	if(g_fnKernelOpenFile==NULL)
	{
		logstr.Format("错误: g_fnKernelOpenFile 函数指针为空");
	}

	if(g_fnKernelGetFileSize==NULL)
	{
		logstr.Format("错误: g_fnKernelGetFileSize 函数指针为空");
	}


}

//////////////////////////////////////////////////////////////////////////
// Add new by David at 20110518.

//------------------------------------------------------------------------
// Custom Function OutputDebugString, used to print debug information.
//------------------------------------------------------------------------
int My_OutputDebugString(CHAR * Format, ...)
{
	va_list		pArgList = NULL;	
	int			Length = 0;
	CHAR		Buffer[2048] = {0}; 
	size_t		MaxCount = sizeof(Buffer);
	size_t		SizeInBytes = MaxCount/sizeof(CHAR);

	if (Format == NULL) 
		return -1;

	va_start(pArgList, Format); // pArgList point to first parameter after Format.

	//Length = _vsnprintf_s(Buffer, SizeInBytes, MaxCount, Format, pArgList);
	Length = _vsnprintf(Buffer, SizeInBytes,  Format, pArgList);

	//Length = _vsnprintf_s(Buffer, SizeInBytes, MaxCount, Format, pArgList);

	//cb = _vsnprintf(buffer, sizeof(buffer), Format, arglist);

	va_end(pArgList);

	if (Length <= 0) 
		return Length;

	OutputDebugString(Buffer);
	return Length;
}

int Delete(PWCHAR FileName)
{
	CHAR szFrom[MAX_PATH] = {0};
	SHFILEOPSTRUCT FileOp = {0};

	CHAR szFileName[MAX_PATH] = {0};
	int iCount = 0;
	int iOk = 0;

	iCount = WideCharToMultiByte(CP_ACP, 0, FileName, wcslen(FileName), szFileName, sizeof(szFileName), NULL, NULL);

	lstrcpy(szFrom, szFileName);
	szFrom[lstrlen(szFrom) + 1] = 0;

	ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
	FileOp.wFunc = FO_DELETE;
	FileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;// | FOF_FILESONLY;

	FileOp.pFrom = szFrom;

	iOk =  SHFileOperation(&FileOp);

	if (iOk == 0)
	{
		My_OutputDebugString("Delete: szFrom = %s\n", szFrom);
	}

	if (iOk != 0)
	{
		My_OutputDebugString("3 szFrom = %s, iOk = 0x%X\n", szFrom, iOk);
	}

	return iOk;
}


// CCopyFileTestDlg message handlers

BOOL CCopyFileTestDlg::OnInitDialog()
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

	GetRealBackupFuncAddress();

	if( !m_szCommandLine.CompareNoCase("-InstallDriver") )
	{
		SendMessage(WM_COMMAND, IDC_INSTALL_DRIVER);
		PostMessage(WM_COMMAND, IDCANCEL);
	}
	else if( !m_szCommandLine.CompareNoCase("-UnInstallDriver") )
	{
		SendMessage(WM_COMMAND, IDC_UNINSTALL_DRIVER);
		PostMessage(WM_COMMAND, IDCANCEL);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCopyFileTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCopyFileTestDlg::OnPaint()
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
HCURSOR CCopyFileTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCopyFileTestDlg::OnInstallDriver()
{
	BOOL	bRet = FALSE;
	TCHAR	ExeFileName[MAX_PATH] = {0};
	TCHAR	DestDriverPath[MAX_PATH] = {0};
	TCHAR	SourceDriverPath[MAX_PATH] = {0};
	UINT	uiRet = 0;
	BOOL	bCopyRet = FALSE;
	DWORD	dwRet = 0;

	TCHAR	drive[_MAX_DRIVE] = {0};
	TCHAR	dir[_MAX_DIR] = {0};
	TCHAR	fname[_MAX_FNAME] = {0};
	TCHAR	ext[_MAX_EXT] = {0};
	//errno_t err = -1;
	int err = -1;

	// Construct sys file path.
	dwRet = GetModuleFileName(NULL, ExeFileName, MAX_PATH);
	//err = _splitpath_s(ExeFileName, drive, dir, fname, ext);
	//err = _makepath_s(SourceDriverPath, MAX_PATH, drive, dir, COPYDEVICE, SYS);
	//err = 
		_splitpath(ExeFileName, drive, dir, fname, ext);
	//err = 
		_makepath(SourceDriverPath,  drive, dir, COPYDEVICE, SYS);

	uiRet = GetSystemDirectory(DestDriverPath, MAX_PATH);
	PathAppend(DestDriverPath, DRIVERS);
	PathAppend(DestDriverPath, COPYDEVICE_SYS);

	// SourceDriverPath --> DestDriverpath.
	bCopyRet = CopyFile( SourceDriverPath, DestDriverPath, FALSE );
// 	dwError = GetLastError();
// 	if ( dwError != ERROR_SUCCESS )
// 	{
// 		AfxMessageBox( _T("安装CopyFileDriver失败！") );
// 		return;
// 	}

	if (!bCopyRet)
	{
		CString str;
		str.Format(_T("复制CopyFileDrivers.sys文件失败！\n  源文件：%s\n目标文件：%s"), SourceDriverPath, DestDriverPath);
		AfxMessageBox( str );
		return;
	}

	bRet = InstallDriver(COPYDEVICE, DestDriverPath);
	if (!bRet)
		AfxMessageBox(_T("安装CopyFileDriver失败！"));
}

void CCopyFileTestDlg::OnUninstallDriver()
{
	BOOL bRet = UninstallDriver(COPYDEVICE);
	TRACE("CCopyFileTestDlg::OnUninstallDriver() UninstallDriver return:%u\n", bRet);
	// Add new by David blow at 2011-05-19.
	DWORD dwError = GetLastError();
	if ( dwError == ERROR_SERVICE_DOES_NOT_EXIST )
	{
		return;
	}
	// above

	//My_OutputDebugString( "OnUninstallDriver dwError = %ld\n", dwError );

	//if (!bRet)
		//AfxMessageBox(_T("卸载CopyFileDriver失败！"));
}

void CCopyFileTestDlg::OnBrowseSource()
{
	CString	strPathName = _T("");
	CString	strFileName = _T("");

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("所有文件(*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		strPathName = dlg.GetPathName();
		strFileName = dlg.GetFileName();
		m_SourceFile = strPathName;
	}
	UpdateData(FALSE);
}

void CCopyFileTestDlg::OnBrowseDest()
{
	char		szDir[MAX_PATH] = {0};
	BROWSEINFO	bi = {0};
	ITEMIDLIST	*pidl = NULL;
	char		szFileName[MAX_PATH] = {0};

	bi.hwndOwner = this->m_hWnd;
	bi.pszDisplayName = szDir;
	bi.lpszTitle = "选择要添加的文件夹";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN;

	pidl = SHBrowseForFolder(&bi);
	if (pidl == NULL) return;
	SHGetPathFromIDList(pidl,szDir);

	lstrcpy(szFileName, strrchr(m_SourceFile, '\\') + 1);
	PathAppend(szDir, szFileName);
	m_DestFile = szDir;
	UpdateData(FALSE);
}

void CCopyFileTestDlg::OnCopyFile()
{
	BOOL bOk = FALSE;
	bOk = KernelCopyFile(m_SourceFile, m_DestFile);
	CString logstr;
	logstr.Format("%s 拷贝到 %s ", m_SourceFile, m_DestFile);
	if (!bOk)
	{
		logstr+="失败\r\n";
		AfxMessageBox(logstr);
		logger(logstr);
	}
	else
	{
		logstr+="成功\r\n";
		AfxMessageBox(logstr);
		logger(logstr);
	}
	return;
}

void CCopyFileTestDlg::OnReadFile()
{
	HANDLE		FileHandle = INVALID_HANDLE_VALUE;
	BOOL		bOk = FALSE;
	LONGLONG	ReadOffset = 0;
	#define		BUFFER_SIZE 512
	BYTE		Buffer[BUFFER_SIZE] = {0};
	DWORD		Length = BUFFER_SIZE;
	DWORD		BytesReturned = 0;
	CString		logstr;

	FileHandle = KernelOpenFile(m_SourceFile);
	if (FileHandle == INVALID_HANDLE_VALUE)
	{
		logstr.Format("%s 打开失败\r\n", m_SourceFile);
		AfxMessageBox(logstr);
		return;
	}

	bOk = KernelReadFile(FileHandle, Buffer,Length, ReadOffset, &BytesReturned);
	if (!bOk)
	{
		logstr.Format("%s 读取文件文件失败\r\n", m_SourceFile);
		AfxMessageBox(logstr);
		logger(logstr);
		if (FileHandle != INVALID_HANDLE_VALUE)
			KernelCloseHandle(FileHandle);
		return;
	}	

	if (FileHandle != INVALID_HANDLE_VALUE)
		KernelCloseHandle(FileHandle);
	
	return;
}

void CCopyFileTestDlg::OnExit()
{
	OnCancel();
}

void CCopyFileTestDlg::OnGetFileSize()
{
	HANDLE			FileHandle = INVALID_HANDLE_VALUE;
	BOOL			bOk = FALSE;
	LONGLONG		FileSize = 0;
	CString logstr;
	// FileHandle = KernelOpenFile(m_SourceFile);
	if(g_fnKernelOpenFile==NULL || g_fnKernelGetFileSize ==NULL )
	{
		logstr.Format("g_fnKernelOpenFile || g_fnKernelGetFileSize 函数指针为空\r\r");
		logger(logstr);
		return ;
	}
	FileHandle = g_fnKernelOpenFile(m_SourceFile);
	if (FileHandle == INVALID_HANDLE_VALUE)
	{
		logstr.Format("%s 打开文件失败！", m_SourceFile);
		AfxMessageBox(logstr);
		return;
	}

	// bOk = KernelGetFileSize(FileHandle, &FileSize);
	bOk = g_fnKernelGetFileSize(FileHandle, &FileSize);
	if (!bOk)
	{
		AfxMessageBox(_T("获取文件大小失败！"));
		if (FileHandle != INVALID_HANDLE_VALUE)
			KernelCloseHandle(FileHandle);
		return;
	}	

	if (FileHandle != INVALID_HANDLE_VALUE)
		KernelCloseHandle(FileHandle);

	TRACE("FileSize = %ld\n", FileSize);
	CString str = _T("");
	str.Format("FileSize = %ld\n", FileSize);
	AfxMessageBox(str);

	return;
}
