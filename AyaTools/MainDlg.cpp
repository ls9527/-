// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AyaTools.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "json.h"
#include "fstream"
#include <afx.h>
#include "iconv.h"
#include "FeatureCode.h"
using namespace std;
// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
{
	//最大512KB配置文件
}

CMainDlg::~CMainDlg()
{

}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
	DDX_Control(pDX, IDC_STATIC_SEL, m_dragToProgress);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_READ_CONFIG, &CMainDlg::OnBnClickedButtonReadConfig)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_INI, &CMainDlg::OnBnClickedButtonExportIni)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

static int Utf8ToUnicode(const char* utf8, wchar_t*unicode,int len)
{
	//int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	memcpy(unicode, wstr, len);
	
	if(wstr) delete[] wstr;
	return len;
}


bool utf8_to_unicode (char *inbuf, size_t *inlen, char *outbuf, size_t *outlen)
{

  /* 目的编码, TRANSLIT：遇到无法转换的字符就找相近字符替换
   *           IGNORE ：遇到无法转换字符跳过*/
  char *encTo = "gb2312//IGNORE";
  /* 源编码 */
  char *encFrom = "UTF-8";

  /* 获得转换句柄
   *@param encTo 目标编码方式
   *@param encFrom 源编码方式
   *
   * */
  iconv_t cd = iconv_open (encTo, encFrom);
  if (cd == (iconv_t)-1)
  {
      perror ("iconv_open");
	  return false;
  }

  
  /* 由于iconv()函数会修改指针，所以要保存源指针 */
  char *tmpin = inbuf;
  char *tmpout = outbuf;
  size_t insize = *inlen;
  size_t outsize = *outlen;

  /* 进行转换
   *@param cd iconv_open()产生的句柄
   *@param srcstart 需要转换的字符串
   *@param inlen 存放还有多少字符没有转换
   *@param tempoutbuf 存放转换后的字符串
   *@param outlen 存放转换后,tempoutbuf剩余的空间
   *
   * */
  size_t ret = iconv (cd, &tmpin, inlen, &tmpout, outlen);
  if (ret == -1)
  {
     perror ("iconv");
  }


  /* 关闭句柄 */
  iconv_close (cd);

  return 0;
}

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	m_listData.InsertColumn(0,_T("节区"),LVCFMT_LEFT,80);
	m_listData.InsertColumn(1,_T("名称描述"),LVCFMT_LEFT,150);
	m_listData.InsertColumn(2,_T("类型"),LVCFMT_LEFT,80);
	m_listData.InsertColumn(3,_T("内容"),LVCFMT_LEFT,80);
	m_listData.SetExtendedStyle(LVS_EX_FULLROWSELECT );
	InitialConfigData();
	
	int index = 0;
	Json::Value feaArray = m_jsonRoot["feaArray"];
	for(auto iter = feaArray.begin();iter!=feaArray.end();iter++){
		Json::Value iterValue = (*iter);

		m_listData.InsertItem(index,CA2W(iterValue["key"].asCString()),0);
		m_listData.SetItemText(index,1,CA2W(iterValue["description"].asCString()));
		m_listData.SetItemText(index,2,CA2W(iterValue["type"].asCString()));

		index++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMainDlg::InitialConfigData()
{
	
	CFile file(_T("D:/github_work/-/AyaTools/config.json"),CFile::modeRead);

	char *buff = new char[CACHE_SIZE];//最大512KB配置文件
	memset(buff,0,CACHE_SIZE);
	UINT realRead = file.Read(buff,CACHE_SIZE);

	char *outbuff = new char[CACHE_SIZE];
	size_t outSize = realRead*2;
	utf8_to_unicode(buff,&realRead,outbuff,&outSize);  
	file.Close();
	
	Json::Reader reader;  

	if ((!reader.parse(outbuff, m_jsonRoot, false)))  
	{  
		//TODO json失败处理
	}  
	delete []outbuff;
	delete [] buff;
}

void CMainDlg::OnBnClickedButtonReadConfig()
{
	if(m_dragToProgress.m_SelectHwnd==NULL){
		MessageBox(_T("请选择进程!"));
		return;
	}
	InitialConfigData();
	
	DWORD processId;
	::GetWindowThreadProcessId(m_dragToProgress.m_SelectHwnd,&processId);
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS,NULL,processId)	;
	ASSERT(hProcess);


	
	// TODO: 在此添加控件通知处理程序代码
	m_listData.DeleteAllItems();
	int index = 0;
	FeatureCode fc;
	Json::Value feaArray = m_jsonRoot["feaArray"];
	for(auto iter = feaArray.begin();iter!=feaArray.end();iter++){
		Json::Value iterValue = (*iter);

		m_listData.InsertItem(index,CA2W(iterValue["key"].asCString()),0);
		m_listData.SetItemText(index,1,CA2W(iterValue["description"].asCString()));
		m_listData.SetItemText(index,2,CA2W(iterValue["type"].asCString()));

		const char* feaCodeStr = iterValue["feaCode"].asCString();
		const char* offset = iterValue["distance"].asCString();
		
		const char* type = iterValue["type"].asCString();
		UINT codeSize = 0;
		if(strcmp(type,"BASEADDR")==0||strcmp(type,"DWORD")==0){
			codeSize = 4;
		}
		else if(strcmp(type,"WORD")==0){
			codeSize = 2;
		}
		else if(strcmp(type,"BYTE")==0){
			codeSize = 1;
		}else {
			continue;
		}
		int offsetI = atoi(offset);
		
		CString strFeatureCode(feaCodeStr);
		int nFeatureCode = strFeatureCode.GetLength();
		TCHAR * featureCode = strFeatureCode .GetBuffer(nFeatureCode);
		CW2A fCodeAscii(featureCode);
		int feaLen = nFeatureCode/3 +(nFeatureCode%3!=0);
		char * feaCode = new char[feaLen];
		fc.convertAsciiToCharPoint(fCodeAscii,nFeatureCode,feaCode,feaLen);		
		DWORD callAddr;
		DWORD readDWORD = fc.GetDataAndBaseAddr(hProcess,feaCode,feaLen,offsetI,codeSize,NULL,callAddr);

		
		delete[] feaCode;

		CString strValue;
		if(strcmp(type,"BASEADDR")==0){
			CString strAddr;
			strAddr.Format(_T("%x"),callAddr);
			m_listData.SetItemText(index,3,strAddr);
		}else{
			CString strReadNumber;
			if(codeSize==4){
				strReadNumber.Format(_T("%x"),readDWORD);
			}else if(codeSize==2){
				strReadNumber.Format(_T("%x"),readDWORD&0xFFFF);
			}else if(codeSize==1){
				strReadNumber.Format(_T("%x"),readDWORD&0xFF);
			}
			m_listData.SetItemText(index,3,strReadNumber);
		}
		
		index++;
	}


	
	

	CloseHandle(hProcess);
}


void CMainDlg::OnBnClickedButtonExportIni()
{
	// TODO: 在此添加控件通知处理程序代码

}
