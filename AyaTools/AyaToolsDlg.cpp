
// AyaToolsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AyaTools.h"
#include "AyaToolsDlg.h"
#include "afxdialogex.h"
#include <atlconv.h>
#include <string>
#include "FeatureCode.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAyaToolsDlg 对话框



CAyaToolsDlg::CAyaToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAyaToolsDlg::IDD, pParent)
	, m_nOffset(0)
	, m_addrLen(20)
	, m_szText(_T(""))
	, m_szFeatureCode(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAyaToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SRC2, m_nOffset);
	DDX_Text(pDX, IDC_EDIT_SRC3, m_addrLen);
	DDX_Text(pDX, IDC_EDIT_DESC, m_szText);
	DDX_Text(pDX, IDC_EDIT_SRC, m_szFeatureCode);
}

BEGIN_MESSAGE_MAP(CAyaToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CALL, &CAyaToolsDlg::OnBnClickedButtonCall)
	
	ON_COMMAND(ID_TEST_T1, &CAyaToolsDlg::OnTestT1)
END_MESSAGE_MAP()


// CAyaToolsDlg 消息处理程序

BOOL CAyaToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAyaToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAyaToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAyaToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CAyaToolsDlg::OnBnClickedButtonCall()
{
	UpdateData();

	
	//m_addrLen = 2;

	HWND hwnd =::FindWindow(NULL,_T("计算器"));
	DWORD processId;
	::GetWindowThreadProcessId(hwnd,&processId);
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS,NULL,processId)	;
	ASSERT(hProcess);

	FeatureCode fc;

	int nFeatureCode = m_szFeatureCode.GetLength();
	TCHAR * featureCode = m_szFeatureCode .GetBuffer(nFeatureCode);
	CW2A fCodeAscii(featureCode);
	int feaLen = nFeatureCode/3 +(nFeatureCode%3!=0);
	char * feaCode = new char[feaLen];
	fc.convertAsciiToCharPoint(fCodeAscii,nFeatureCode,feaCode,feaLen);

	TCHAR * buff = new TCHAR[4096];
	memset(buff,0,4096);
	DWORD callAddr;
	fc.GetDataAndBaseAddr(hProcess,feaCode,feaLen,m_nOffset,m_addrLen,buff,callAddr);


	CloseHandle(hProcess);
	
	m_szText = buff;
	delete buff;	
	delete feaCode;

	CString baseAddrText;
	baseAddrText.Format(_T("%08x"),callAddr);
	SetDlgItemText(IDC_EDIT_BASE_ADDR,baseAddrText);
	UpdateData(FALSE);
}



void CAyaToolsDlg::OnTestT1()
{
	// TODO: 在此添加命令处理程序代码
	
}
