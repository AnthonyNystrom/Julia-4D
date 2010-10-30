// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "d3dpointsetrenderer.h"
#include "set_generation_package.h"
#include "defines.h"

#include <new>
using std::bad_alloc;






class CURL
{
private:
	// The default browser
	CString m_strBrowser;
public:
	// Open a URL
	void Open(LPCTSTR lpszURL, bool bNewWindow = true)
	{
		if (bNewWindow)
			::ShellExecute(NULL, NULL, GetBrowser(), lpszURL, NULL, SW_SHOWNORMAL);
		else
			::ShellExecute(NULL, NULL, lpszURL, NULL, NULL, SW_SHOWNORMAL);
	}

	// Get the default browser from the registry
	LPCTSTR GetBrowser(void)
	{
		// Have we already got the browser?
		if (m_strBrowser.IsEmpty())
		{
			// Get the default browser from HKCR\http\shell\open\command
			HKEY hKey = NULL;
			// Open the registry
			if (::RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("http\\shell\\open\\command"),
				0, KEY_READ, &hKey) == ERROR_SUCCESS)
			{
				// Data size
				DWORD cbData = 0;
				// Get the default value
				if (::RegQueryValueEx(hKey, NULL, NULL, NULL, NULL, &cbData) 
					== ERROR_SUCCESS && cbData > 0)
				{
					// Allocate a suitable buffer
					TCHAR* psz = new TCHAR [cbData];
					// Success?
					if (psz != NULL)
					{
						if (::RegQueryValueEx(hKey, NULL, NULL,
							NULL, (LPBYTE)psz, &cbData) ==
							ERROR_SUCCESS)
						{
							// Success!
							m_strBrowser = psz;
						}
						delete [] psz;
					}
				}
				::RegCloseKey(hKey);
			}
			// Do we have the browser?
			if (m_strBrowser.GetLength() > 0)
			{
				// Strip the full path from the string
				int nStart = m_strBrowser.Find('"');
				int nEnd = m_strBrowser.ReverseFind('"');
				// Do we have either quote?
				// If so, then the path contains spaces
				if (nStart >= 0 && nEnd >= 0)
				{
					// Are they the same?
					if (nStart != nEnd)
					{   
						// Get the full path
						m_strBrowser = m_strBrowser.Mid(nStart + 1, nEnd - nStart - 1);
					}
				}
				else
				{
					// We may have a pathname with spaces but
					// no quotes (Netscape), e.g.:
					//   C:\PROGRAM FILES\NETSCAPE\COMMUNICATOR\PROGRAM\NETSCAPE.EXE -h "%1"
					// Look for the last backslash
					int nIndex = m_strBrowser.ReverseFind('\\');
					// Success?
					if (nIndex > 0)
					{
						// Look for the next space after the final
						// backslash
						int nSpace = m_strBrowser.Find(' ', nIndex);
						// Do we have a space?
						if (nSpace > 0)
							m_strBrowser = m_strBrowser.Left(nSpace);    
					}
				}
			}
		}
		// Done
		return m_strBrowser;
	}
};













class CMainFrame : public CFrameWnd
{
	friend class CCacheSettingsDlg;

protected:
	D3DPointSetRenderer Renderer;
	bool generating_set;
	bool waiting_for_cancel;
	bool previous_set_completed;

	set_generation_package set_gen_pkg;

	bool renderer_reset_required;
	double generation_start;
	bool first_renderer_init;
	COLORREF acrCustClr[16];

	void ActivateFrame(int nCmdShow);

	CMenu *main_menu;

	void PerformRandomizations(void);
	long OnDisplayChange( UINT nParam, LONG lParam);
	long OnRecreateRenderer( UINT nParam, LONG lParam);
	long OnSetGenSuccess( UINT nParam, LONG lParam);
	long OnSetGenFailure( UINT nParam, LONG lParam);
	long OnBoundingGridChanged( UINT nParam, LONG lParam);

	void UpdateBoundingGrid(void);


protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// Attributes
public:

	// Operations
public:

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


public:
	CDialogBar m_wndMainSettingsDialog;
	CDialogBar m_wndExtraSettingsDialog;
	CDialogBar m_wndMultiplicationSettingsDialog;
	CDialogBar m_wndRenderToSettingsDialog;


	void GetDrawableClientRect(RECT &src_frame_rect, RECT &src_erase_rect);
	void SetVars(Cjulia4d2Doc &doc);
	arc_ball GetArcBall(void);
	void SetArcBall(const arc_ball &src_arcball);

protected:  // control bar embedded members
	CStatusBar m_wndStatusBar;

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose(void);
	afx_msg void OnPaint(void);
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	afx_msg void OnHelp(void);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewMainSettings();
	afx_msg void OnUpdateViewMainSettings(CCmdUI *pCmdUI);

	afx_msg void OnViewExtraSettings();
	afx_msg void OnUpdateViewExtraSettings(CCmdUI *pCmdUI);


	afx_msg void OnUpdateViewMultiplicationSettings(CCmdUI *pCmdUI);
	afx_msg void OnViewMultiplicationSettings();

	afx_msg void OnUpdateViewRenderToSettings(CCmdUI *pCmdUI);
	afx_msg void OnViewRenderToSettings();

	afx_msg void OnUpdateViewFormulaMenuButton(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewGenerateButton(CCmdUI *pCmdUI);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedFormulaMenu();

	bool creation_complete;
	afx_msg void OnBnClickedGenerate();
	afx_msg void OnAppAbout();

	bool about_box_showing;
	afx_msg void OnBnClickedMulColumnDefaultVariablesRadio();
	afx_msg void OnBnClickedMulColumnConstantValuesRadio();
	afx_msg void OnBnClickedMulColumnCustomVariablesRadio();
	afx_msg void OnBnClickedMulMatrixDefaultVariablesRadio();
	afx_msg void OnBnClickedMulMatrixCustomVariablesRadio();



	afx_msg void OnBnClickedCullNonSurfacePointsCheck();
	afx_msg void OnBnClickedHopfMapCheck();
	afx_msg void OnBnClickedScreenRadio();
	afx_msg void OnBnClickedMeshRadio();
	afx_msg void OnBnClickedCsvFileRadio();
	afx_msg void OnBnClickedVoxelRadio();

	afx_msg void OnUpdateViewLBN(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewShaderCombo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewNormalRadiusEdit(CCmdUI *pCmdUI);

	afx_msg void OnUpdateViewReverseSet(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewCNSP(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewHopfMap(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewScreenRadio(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewMeshRadio(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewCsvFileRadio(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewVoxelRadio(CCmdUI *pCmdUI);


	afx_msg void OnUpdateViewZW(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewWMin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewWMax(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewSamples(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewIsovaluePercent(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewAvg(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewAvgPercent(CCmdUI *pCmdUI);



	afx_msg void OnViewHideall();
	afx_msg void OnUpdateViewHideall(CCmdUI *pCmdUI);
	afx_msg void OnViewShowall();
	afx_msg void OnUpdateViewShowall(CCmdUI *pCmdUI);


	void UncheckRandomizeCheckboxes(void);

	afx_msg void OnFormulaCustom();
	afx_msg void OnUpdateFormulaCustom(CCmdUI *pCmdUI);

	afx_msg void OnGeneralClassic();
	afx_msg void OnUpdateGeneralClassic(CCmdUI *pCmdUI);
	afx_msg void OnGeneralExponential();
	afx_msg void OnUpdateGeneralExponential(CCmdUI *pCmdUI);
	afx_msg void OnGeneralQuadratic();
	afx_msg void OnUpdateGeneralQuadratic(CCmdUI *pCmdUI);
	afx_msg void OnGeneralLambda();
	afx_msg void OnUpdateGeneralLambda(CCmdUI *pCmdUI);
	afx_msg void OnGeneralFunkyBlobs();
	afx_msg void OnUpdateGeneralFunkyBlobs(CCmdUI *pCmdUI);

	afx_msg void OnSineSine();
	afx_msg void OnUpdateSineSine(CCmdUI *pCmdUI);
	afx_msg void OnSineSinePlus();
	afx_msg void OnUpdateSineSinePlus(CCmdUI *pCmdUI);
	afx_msg void OnSineSineClassic();
	afx_msg void OnUpdateSineSineClassic(CCmdUI *pCmdUI);
	afx_msg void OnCosineCosine();
	afx_msg void OnUpdateCosineCosine(CCmdUI *pCmdUI);
	afx_msg void OnCosineCosinePlus();
	afx_msg void OnUpdateCosineCosinePlus(CCmdUI *pCmdUI);
	afx_msg void OnCosineCosineClassic();
	afx_msg void OnUpdateCosineCosineClassic(CCmdUI *pCmdUI);
	afx_msg void OnTangentTangent();
	afx_msg void OnUpdateTangentTangent(CCmdUI *pCmdUI);
	afx_msg void OnTangentTangentPlus();
	afx_msg void OnUpdateTangentTangentPlus(CCmdUI *pCmdUI);
	afx_msg void OnTangentTangentClassic();
	afx_msg void OnUpdateTangentTangentClassic(CCmdUI *pCmdUI);
	afx_msg void OnSecantSecant();
	afx_msg void OnUpdateSecantSecant(CCmdUI *pCmdUI);
	afx_msg void OnSecantSecantPlus();
	afx_msg void OnUpdateSecantSecantPlus(CCmdUI *pCmdUI);
	afx_msg void OnSecantSecantClassic();
	afx_msg void OnUpdateSecantSecantClassic(CCmdUI *pCmdUI);
	afx_msg void OnCosecantCosecant();
	afx_msg void OnUpdateCosecantCosecant(CCmdUI *pCmdUI);
	afx_msg void OnCosecantCosecantPlus();
	afx_msg void OnUpdateCosecantCosecantPlus(CCmdUI *pCmdUI);
	afx_msg void OnCosecantCosecantClassic();
	afx_msg void OnUpdateCosecantCosecantClassic(CCmdUI *pCmdUI);
	afx_msg void OnCotangentCotangent();
	afx_msg void OnUpdateCotangentCotangent(CCmdUI *pCmdUI);
	afx_msg void OnCotangentCotangentPlus();
	afx_msg void OnUpdateCotangentCotangentPlus(CCmdUI *pCmdUI);
	afx_msg void OnCotangentCotangentClassic();
	afx_msg void OnUpdateCotangentCotangentClassic(CCmdUI *pCmdUI);

	afx_msg void OnHyperbolicSineHyperbolicSine();
	afx_msg void OnUpdateHyperbolicSineHyperbolicSine(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicSineHyperbolicSinePlus();
	afx_msg void OnUpdateHyperbolicSineHyperbolicSinePlus(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicSineHyperbolicSineClassic();
	afx_msg void OnUpdateHyperbolicSineHyperbolicSineClassic(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicCosineHyperbolicCosine();
	afx_msg void OnUpdateHyperbolicCosineHyperbolicCosine(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicCosineHyperbolicCosinePlus();
	afx_msg void OnUpdateHyperbolicCosineHyperbolicCosinePlus(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicCosineHyperbolicCosineClassic();
	afx_msg void OnUpdateHyperbolicCosineHyperbolicCosineClassic(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicTangentHyperbolicTangent();
	afx_msg void OnUpdateHyperbolicTangentHyperbolicTangent(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicTangentHyperbolicTangentPlus();
	afx_msg void OnUpdateHyperbolicTangentHyperbolicTangentPlus(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicTangentHyperbolicTangentClassic();
	afx_msg void OnUpdateHyperbolicTangentHyperbolicTangentClassic(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicSecantHyperbolicSecant();
	afx_msg void OnUpdateHyperbolicSecantHyperbolicSecant(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicSecantHyperbolicSecantPlus();
	afx_msg void OnUpdateHyperbolicSecantHyperbolicSecantPlus(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicSecantHyperbolicSecantClassic();
	afx_msg void OnUpdateHyperbolicSecantHyperbolicSecantClassic(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicCosecantHyperbolicCosecant();
	afx_msg void OnUpdateHyperbolicCosecantHyperbolicCosecant(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicCosecantHyperbolicCosecantPlus();
	afx_msg void OnUpdateHyperbolicCosecantHyperbolicCosecantPlus(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicCosecantHyperbolicCosecantClassic();
	afx_msg void OnUpdateHyperbolicCosecantHyperbolicCosecantClassic(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicCotangentHyperbolicCotangent();
	afx_msg void OnUpdateHyperbolicCotangentHyperbolicCotangent(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicCotangentHyperbolicCotangentPlus();
	afx_msg void OnUpdateHyperbolicCotangentHyperbolicCotangentPlus(CCmdUI *pCmdUI);
	afx_msg void OnHyperbolicCotangentHyperbolicCotangentClassic();
	afx_msg void OnUpdateHyperbolicCotangentHyperbolicCotangentClassic(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedSwapZAndCCheck();
	afx_msg void OnBnClickedRandomizeCCheck();
	afx_msg void OnBnClickedReverseSetCheck();

	afx_msg void OnEnChangeSamplesEdit();
	afx_msg void OnEnChangeAvgPercentEdit();
	afx_msg void OnEnChangeIsovaluePercentEdit();
	afx_msg void OnEnChangeAvgPassesEdit();
	afx_msg void OnEnChangeFormulaEdit();
	afx_msg void OnEnChangeCXEdit();
	afx_msg void OnEnChangeCYEdit();
	afx_msg void OnEnChangeCZEdit();
	afx_msg void OnEnChangeCWEdit();
	afx_msg void OnEnChangeXMinEdit();
	afx_msg void OnEnChangeYMinEdit();
	afx_msg void OnEnChangeZMinEdit();
	afx_msg void OnEnChangeWMinEdit();
	afx_msg void OnEnChangeXMaxEdit();
	afx_msg void OnEnChangeYMaxEdit();
	afx_msg void OnEnChangeZMaxEdit();
	afx_msg void OnEnChangeWMaxEdit();
	afx_msg void OnEnChangeZWEdit();
	afx_msg void OnEnChangeAEdit();
	afx_msg void OnEnChangeIterationsEdit();
	afx_msg void OnEnChangeInfinityEdit();
	afx_msg void OnEnChangeResolutionEdit();
	afx_msg void OnEnChangeMulColumnConstantValuesXEdit();
	afx_msg void OnEnChangeMulColumnConstantValuesYEdit();
	afx_msg void OnEnChangeMulColumnConstantValuesZEdit();
	afx_msg void OnEnChangeMulColumnConstantValuesWEdit();
	afx_msg void OnEnChangeMulColumnCustomVariablesXEdit();
	afx_msg void OnEnChangeMulColumnCustomVariablesYEdit();
	afx_msg void OnEnChangeMulColumnCustomVariablesZEdit();
	afx_msg void OnEnChangeMulColumnCustomVariablesWEdit();
	afx_msg void OnEnChangeMulMatrixCustom11Edit();
	afx_msg void OnEnChangeMulMatrixCustom12Edit();
	afx_msg void OnEnChangeMulMatrixCustom13Edit();
	afx_msg void OnEnChangeMulMatrixCustom14Edit();
	afx_msg void OnEnChangeMulMatrixCustom21Edit();
	afx_msg void OnEnChangeMulMatrixCustom22Edit();
	afx_msg void OnEnChangeMulMatrixCustom23Edit();
	afx_msg void OnEnChangeMulMatrixCustom24Edit();
	afx_msg void OnEnChangeMulMatrixCustom31Edit();
	afx_msg void OnEnChangeMulMatrixCustom32Edit();
	afx_msg void OnEnChangeMulMatrixCustom33Edit();
	afx_msg void OnEnChangeMulMatrixCustom34Edit();
	afx_msg void OnEnChangeMulMatrixCustom41Edit();
	afx_msg void OnEnChangeMulMatrixCustom42Edit();
	afx_msg void OnEnChangeMulMatrixCustom43Edit();
	afx_msg void OnEnChangeMulMatrixCustom44Edit();

	afx_msg void OnViewRestoreall();
	afx_msg void OnUpdateViewRestoreall(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHelpContents(CCmdUI *pCmdUI);

	afx_msg void OnUpdateRendererZoomMinus(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRendererPointsizePlus(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRendererPointsizeMinus(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRendererFog(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRendererBoundinggrid(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRendererZslicePlus(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRendererZsliceMinus(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRendererSinglezslice(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRendererResetzslicing(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRendererResetztr(CCmdUI *pCmdUI);
	afx_msg void OnHelpContents();

	afx_msg void OnRendererZoomMinus();
	afx_msg void OnRendererPointsizePlus();
	afx_msg void OnRendererPointsizeMinus();
	afx_msg void OnRendererFog();
	afx_msg void OnRendererBoundinggrid();
	afx_msg void OnRendererZslicePlus();
	afx_msg void OnRendererZsliceMinus();
	afx_msg void OnRendererSinglezslice();
	afx_msg void OnRendererResetzslicing();
	afx_msg void OnRendererResetztr();
	afx_msg void OnRendererZoomPlus();
	afx_msg void OnUpdateRendererZoomPlus(CCmdUI *pCmdUI);
	afx_msg void OnRendererInterfacetext();
	afx_msg void OnUpdateRendererInterfacetext(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRendererSetbackgroundcolor(CCmdUI *pCmdUI);
	afx_msg void OnRendererSetbackgroundcolor();
	afx_msg void OnUpdateRendererSetfogcolor(CCmdUI *pCmdUI);
	afx_msg void OnRendererSetfogcolor();
	afx_msg void OnUpdateRendererSetinterfacetextcolour(CCmdUI *pCmdUI);
	afx_msg void OnRendererSetinterfacetextcolour();
	afx_msg void OnUpdateRendererResetboundinggrid(CCmdUI *pCmdUI);
	afx_msg void OnRendererResetboundinggrid();
	afx_msg void OnUpdateRendererRestoredefaultcolours(CCmdUI *pCmdUI);
	afx_msg void OnRendererRestoredefaultcolours();
	afx_msg void OnUpdateInterfaceelementsAxisindicators(CCmdUI *pCmdUI);
	afx_msg void OnInterfaceelementsAxisindicators();
	afx_msg void OnUpdateFileGenerate(CCmdUI *pCmdUI);
	afx_msg void OnFileGenerate();
	afx_msg void OnUpdateEditCachesettings(CCmdUI *pCmdUI);
	afx_msg void OnEditCachesettings();

	afx_msg void OnBnClickedMulGenButton();
	afx_msg void OnVoxelOpacityPlus();
	afx_msg void OnUpdateVoxelOpacityPlus(CCmdUI *pCmdUI);
	afx_msg void OnVoxelOpacityMinus();
	afx_msg void OnUpdateVoxelOpacityMinus(CCmdUI *pCmdUI);
	afx_msg void OnHelpLicenseterms();
	afx_msg void OnUpdateHelpLicenseterms(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRenderToGenButton();
	afx_msg void OnBnClickedExtraGenButton();
	afx_msg void OnCbnSelchangeShaderCombo();
	afx_msg void OnAmbienceLevelPlus();
	afx_msg void OnUpdateAmbienceLevelPlus(CCmdUI *pCmdUI);
	afx_msg void OnAmbienceLevelMins();
	afx_msg void OnUpdateAmbienceLevelMins(CCmdUI *pCmdUI);

	afx_msg void OnEnChangeHopfFieldResolutionEdit();
	afx_msg void OnUpdateViewHopfFieldResolutionEdit(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedLightByNormalsCheck();
	afx_msg void OnUpdateViewSwapZAndCEdit(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedPropResCheck();
	afx_msg void OnUpdateViewPropResCheck(CCmdUI *pCmdUI);
};



