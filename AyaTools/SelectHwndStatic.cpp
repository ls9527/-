#include "stdafx.h"
#include "SelectHwndStatic.h"
#include "SelHwnd.h"





CSelectHwndStatic::CSelectHwndStatic(void):m_start(FALSE)
{
}


CSelectHwndStatic::~CSelectHwndStatic(void)
{
}


BEGIN_MESSAGE_MAP(CSelectHwndStatic, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

END_MESSAGE_MAP()


void CSelectHwndStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_start = TRUE;
	DWORD threadId = GetWindowThreadProcessId(m_hWnd,NULL);
	::SetHwndHook(threadId,m_hWnd);
	CStatic::OnLButtonDown(nFlags, point);
}


void CSelectHwndStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonUp(nFlags, point);
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_start = FALSE;
	::UnHwndHook();
	
}


void CSelectHwndStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	CStatic::OnMouseMove(nFlags, point);
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_start){
		m_SelectHwnd= ::WindowFromPoint(point);
		CString str;		
		::GetWindowText(m_SelectHwnd,str.GetBuffer(100),100);
		str.ReleaseBuffer();
		//str.Format(_T("x:%ld,y:%ld"),point.x,point.y);
		SetWindowText(str);
	}
	
	
}
