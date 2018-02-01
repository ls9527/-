#pragma once
#include "afxcmn.h"
#include "writer.h"
#include "selecthwndstatic.h"

#define CACHE_SIZE 0x8000
// CMainDlg 对话框

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void InitialConfigData();
	
	
	CListCtrl m_listData;
	afx_msg void OnBnClickedButtonReadConfig();
private:
	
	Json::Value m_jsonRoot;
public:
	afx_msg void OnBnClickedButtonExportIni();
	CSelectHwndStatic m_dragToProgress;
};
