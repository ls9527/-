#pragma once
class CSelectHwndStatic:public CStatic
{
public:
	CSelectHwndStatic(void);
	~CSelectHwndStatic(void);


	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT MyMsgHandler(WPARAM,LPARAM);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	BOOL m_start;
public:
	HWND m_SelectHwnd;
};

