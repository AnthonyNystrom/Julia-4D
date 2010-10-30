// julia4d2View.cpp : implementation of the Cjulia4d2View class
//

#include "stdafx.h"
#include "julia4d2.h"

#include "julia4d2Doc.h"
#include "julia4d2View.h"
#include ".\julia4d2view.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cjulia4d2View

IMPLEMENT_DYNCREATE(Cjulia4d2View, CView)

BEGIN_MESSAGE_MAP(Cjulia4d2View, CView)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Cjulia4d2View construction/destruction

Cjulia4d2View::Cjulia4d2View()
{
}

Cjulia4d2View::~Cjulia4d2View()
{
}

BOOL Cjulia4d2View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// Cjulia4d2View drawing

void Cjulia4d2View::OnDraw(CDC* /*pDC*/)
{
	Cjulia4d2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}


// Cjulia4d2View diagnostics

#ifdef _DEBUG
void Cjulia4d2View::AssertValid() const
{
	CView::AssertValid();
}

void Cjulia4d2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cjulia4d2Doc* Cjulia4d2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cjulia4d2Doc)));
	return (Cjulia4d2Doc*)m_pDocument;
}
#endif //_DEBUG



void Cjulia4d2View::OnSize(UINT nType, int cx, int cy)
{
	CWnd *pParent = GetParent();

	if(0 != pParent)
		pParent->PostMessage(WM_SIZE, 0, 0);
}


void Cjulia4d2View::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if(0 != pParent)
		pParent->SendNotifyMessage(WM_MOUSEMOVE, nFlags, MAKELPARAM(point.x, point.y));
}

void Cjulia4d2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if(0 != pParent)
		pParent->SendNotifyMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
}

void Cjulia4d2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if(0 != pParent)
		pParent->SendNotifyMessage(WM_LBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));
}

void Cjulia4d2View::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if(0 != pParent)
		pParent->SendNotifyMessage(WM_RBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
}

void Cjulia4d2View::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if(0 != pParent)
		pParent->SendNotifyMessage(WM_RBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));
}

void Cjulia4d2View::OnMButtonDown(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if(0 != pParent)
		pParent->SendNotifyMessage(WM_MBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
}

void Cjulia4d2View::OnMButtonUp(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if(0 != pParent)
		pParent->SendNotifyMessage(WM_MBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));
}

void Cjulia4d2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd *pParent = GetParent();

	if(0 != pParent)
		pParent->SendNotifyMessage(WM_KEYDOWN, nChar, 0);
}



