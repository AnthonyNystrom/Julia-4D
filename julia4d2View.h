// julia4d2View.h : interface of the Cjulia4d2View class
//
#ifndef JULIA4D2_VIEW_H
#define JULIA4D2_VIEW_H




#include "julia4d2Doc.h"


class Cjulia4d2Doc;

class Cjulia4d2View : public CView
{
protected: // create from serialization only
	Cjulia4d2View();
	DECLARE_DYNCREATE(Cjulia4d2View)

// Attributes
public:
	Cjulia4d2Doc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~Cjulia4d2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);


};

#ifndef _DEBUG  // debug version in julia4d2View.cpp
inline Cjulia4d2Doc* Cjulia4d2View::GetDocument() const
   { return reinterpret_cast<Cjulia4d2Doc*>(m_pDocument); }
#endif







#endif