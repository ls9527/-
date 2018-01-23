
// AyaToolsDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CAyaToolsDlg �Ի���



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


// CAyaToolsDlg ��Ϣ�������

BOOL CAyaToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAyaToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAyaToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CAyaToolsDlg::OnBnClickedButtonCall()
{
	UpdateData();

	
	//m_addrLen = 2;

	HWND hwnd =::FindWindow(NULL,_T("������"));
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
	// TODO: �ڴ���������������
	
}
