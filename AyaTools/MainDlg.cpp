// MainDlg.cpp : ʵ���ļ�
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
// CMainDlg �Ի���

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
{
	//���512KB�����ļ�
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


// CMainDlg ��Ϣ�������

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

  /* Ŀ�ı���, TRANSLIT�������޷�ת�����ַ���������ַ��滻
   *           IGNORE �������޷�ת���ַ�����*/
  char *encTo = "gb2312//IGNORE";
  /* Դ���� */
  char *encFrom = "UTF-8";

  /* ���ת�����
   *@param encTo Ŀ����뷽ʽ
   *@param encFrom Դ���뷽ʽ
   *
   * */
  iconv_t cd = iconv_open (encTo, encFrom);
  if (cd == (iconv_t)-1)
  {
      perror ("iconv_open");
	  return false;
  }

  
  /* ����iconv()�������޸�ָ�룬����Ҫ����Դָ�� */
  char *tmpin = inbuf;
  char *tmpout = outbuf;
  size_t insize = *inlen;
  size_t outsize = *outlen;

  /* ����ת��
   *@param cd iconv_open()�����ľ��
   *@param srcstart ��Ҫת�����ַ���
   *@param inlen ��Ż��ж����ַ�û��ת��
   *@param tempoutbuf ���ת������ַ���
   *@param outlen ���ת����,tempoutbufʣ��Ŀռ�
   *
   * */
  size_t ret = iconv (cd, &tmpin, inlen, &tmpout, outlen);
  if (ret == -1)
  {
     perror ("iconv");
  }


  /* �رվ�� */
  iconv_close (cd);

  return 0;
}

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	m_listData.InsertColumn(0,_T("����"),LVCFMT_LEFT,80);
	m_listData.InsertColumn(1,_T("��������"),LVCFMT_LEFT,150);
	m_listData.InsertColumn(2,_T("����"),LVCFMT_LEFT,80);
	m_listData.InsertColumn(3,_T("����"),LVCFMT_LEFT,80);
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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CMainDlg::InitialConfigData()
{
	
	CFile file(_T("D:/github_work/-/AyaTools/config.json"),CFile::modeRead);

	char *buff = new char[CACHE_SIZE];//���512KB�����ļ�
	memset(buff,0,CACHE_SIZE);
	UINT realRead = file.Read(buff,CACHE_SIZE);

	char *outbuff = new char[CACHE_SIZE];
	size_t outSize = realRead*2;
	utf8_to_unicode(buff,&realRead,outbuff,&outSize);  
	file.Close();
	
	Json::Reader reader;  

	if ((!reader.parse(outbuff, m_jsonRoot, false)))  
	{  
		//TODO jsonʧ�ܴ���
	}  
	delete []outbuff;
	delete [] buff;
}

void CMainDlg::OnBnClickedButtonReadConfig()
{
	if(m_dragToProgress.m_SelectHwnd==NULL){
		MessageBox(_T("��ѡ�����!"));
		return;
	}
	InitialConfigData();
	
	DWORD processId;
	::GetWindowThreadProcessId(m_dragToProgress.m_SelectHwnd,&processId);
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS,NULL,processId)	;
	ASSERT(hProcess);


	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}
