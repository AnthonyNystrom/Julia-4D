// MainFrm.cpp : implementation of the CMainFrame class
//




#include "stdafx.h"
#include "utilities.h"
#include "julia4d2.h"
#include "about_dialog.h"
#include "splash_dialog.h"
#include "cache_settings_dialog.h"
#include "license_dialog.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <iostream>
#include <sstream>
#include <iomanip>
using std::endl;
using std::ostringstream;
using std::setprecision;


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)

	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_ACTIVATEAPP()
	ON_MESSAGE( WM_DISPLAYCHANGE, OnDisplayChange)
	ON_MESSAGE( WM_RECREATE_RENDERER, OnRecreateRenderer)
	ON_MESSAGE( WM_SET_GEN_SUCCESS, OnSetGenSuccess)
	ON_MESSAGE( WM_SET_GEN_FAILURE, OnSetGenFailure)
	ON_MESSAGE( WM_BOUNDING_GRID_CHANGED, OnBoundingGridChanged)


	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()


	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_VIEW_MAIN_SETTINGS, OnViewMainSettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAIN_SETTINGS, OnUpdateViewMainSettings)
	ON_COMMAND(ID_VIEW_EXTRA_SETTINGS, OnViewExtraSettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EXTRA_SETTINGS, OnUpdateViewExtraSettings)
	ON_COMMAND(ID_VIEW_MULTIPLICATION_SETTINGS, OnViewMultiplicationSettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MULTIPLICATION_SETTINGS, OnUpdateViewMultiplicationSettings)
	ON_COMMAND(ID_VIEW_RENDER_TO_SETTINGS, OnViewRenderToSettings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDER_TO_SETTINGS, OnUpdateViewRenderToSettings)
	ON_UPDATE_COMMAND_UI(IDC_FORMULA_MENU, OnUpdateViewFormulaMenuButton)
	ON_UPDATE_COMMAND_UI(IDC_GENERATE, OnUpdateViewGenerateButton)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_FORMULA_MENU, OnBnClickedFormulaMenu)


	ON_COMMAND(ID_FORMULA_CUSTOM, OnFormulaCustom)
	ON_UPDATE_COMMAND_UI(ID_FORMULA_CUSTOM, OnUpdateFormulaCustom)

	ON_COMMAND(ID_GENERAL_CLASSIC, OnGeneralClassic)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_CLASSIC, OnUpdateGeneralClassic)
	ON_COMMAND(ID_GENERAL_EXPONENTIAL, OnGeneralExponential)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_EXPONENTIAL, OnUpdateGeneralExponential)
	ON_COMMAND(ID_GENERAL_QUADRATIC, OnGeneralQuadratic)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_QUADRATIC, OnUpdateGeneralQuadratic)
	ON_COMMAND(ID_GENERAL_LAMBDA, OnGeneralLambda)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_LAMBDA, OnUpdateGeneralLambda)
	ON_COMMAND(ID_GENERAL_FUNKYBLOBS, OnGeneralFunkyBlobs)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_FUNKYBLOBS, OnUpdateGeneralFunkyBlobs)

	ON_COMMAND(ID_SINE_SINE, OnSineSine)
	ON_UPDATE_COMMAND_UI(ID_SINE_SINE, OnUpdateSineSine)
	ON_COMMAND(ID_SINE_SINEPLUS, OnSineSinePlus)
	ON_UPDATE_COMMAND_UI(ID_SINE_SINEPLUS, OnUpdateSineSinePlus)
	ON_COMMAND(ID_SINE_SINECLASSIC, OnSineSineClassic)
	ON_UPDATE_COMMAND_UI(ID_SINE_SINECLASSIC, OnUpdateSineSineClassic)
	ON_COMMAND(ID_COSINE_COSINE, OnCosineCosine)
	ON_UPDATE_COMMAND_UI(ID_COSINE_COSINE, OnUpdateCosineCosine)
	ON_COMMAND(ID_COSINE_COSINEPLUS, OnCosineCosinePlus)
	ON_UPDATE_COMMAND_UI(ID_COSINE_COSINEPLUS, OnUpdateCosineCosinePlus)
	ON_COMMAND(ID_COSINE_COSINECLASSIC, OnCosineCosineClassic)
	ON_UPDATE_COMMAND_UI(ID_COSINE_COSINECLASSIC, OnUpdateCosineCosineClassic)
	ON_COMMAND(ID_TANGENT_TANGENT, OnTangentTangent)
	ON_UPDATE_COMMAND_UI(ID_TANGENT_TANGENT, OnUpdateTangentTangent)
	ON_COMMAND(ID_TANGENT_TANGENTPLUS, OnTangentTangentPlus)
	ON_UPDATE_COMMAND_UI(ID_TANGENT_TANGENTPLUS, OnUpdateTangentTangentPlus)
	ON_COMMAND(ID_TANGENT_TANGENTCLASSIC, OnTangentTangentClassic)
	ON_UPDATE_COMMAND_UI(ID_TANGENT_TANGENTCLASSIC, OnUpdateTangentTangentClassic)
	ON_COMMAND(ID_SECANT_SECANT, OnSecantSecant)
	ON_UPDATE_COMMAND_UI(ID_SECANT_SECANT, OnUpdateSecantSecant)
	ON_COMMAND(ID_SECANT_SECANTPLUS, OnSecantSecantPlus)
	ON_UPDATE_COMMAND_UI(ID_SECANT_SECANTPLUS, OnUpdateSecantSecantPlus)
	ON_COMMAND(ID_SECANT_SECANTCLASSIC, OnSecantSecantClassic)
	ON_UPDATE_COMMAND_UI(ID_SECANT_SECANTCLASSIC, OnUpdateSecantSecantClassic)
	ON_COMMAND(ID_COSECANT_COSECANT, OnCosecantCosecant)
	ON_UPDATE_COMMAND_UI(ID_COSECANT_COSECANT, OnUpdateCosecantCosecant)
	ON_COMMAND(ID_COSECANT_COSECANTPLUS, OnCosecantCosecantPlus)
	ON_UPDATE_COMMAND_UI(ID_COSECANT_COSECANTPLUS, OnUpdateCosecantCosecantPlus)
	ON_COMMAND(ID_COSECANT_COSECANTCLASSIC, OnCosecantCosecantClassic)
	ON_UPDATE_COMMAND_UI(ID_COSECANT_COSECANTCLASSIC, OnUpdateCosecantCosecantClassic)
	ON_COMMAND(ID_COTANGENT_COTANGENT, OnCotangentCotangent)
	ON_UPDATE_COMMAND_UI(ID_COTANGENT_COTANGENT, OnUpdateCotangentCotangent)
	ON_COMMAND(ID_COTANGENT_COTANGENTPLUS, OnCotangentCotangentPlus)
	ON_UPDATE_COMMAND_UI(ID_COTANGENT_COTANGENTPLUS, OnUpdateCotangentCotangentPlus)
	ON_COMMAND(ID_COTANGENT_COTANGENTCLASSIC, OnCotangentCotangentClassic)
	ON_UPDATE_COMMAND_UI(ID_COTANGENT_COTANGENTCLASSIC, OnUpdateCotangentCotangentClassic)

	ON_COMMAND(ID_HYPERBOLICSINE_HYPERBOLICSINE, OnHyperbolicSineHyperbolicSine)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICSINE_HYPERBOLICSINE, OnUpdateHyperbolicSineHyperbolicSine)
	ON_COMMAND(ID_HYPERBOLICSINE_HYPERBOLICSINEPLUS, OnHyperbolicSineHyperbolicSinePlus)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICSINE_HYPERBOLICSINEPLUS, OnUpdateHyperbolicSineHyperbolicSinePlus)
	ON_COMMAND(ID_HYPERBOLICSINE_HYPERBOLICSINECLASSIC, OnHyperbolicSineHyperbolicSineClassic)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICSINE_HYPERBOLICSINECLASSIC, OnUpdateHyperbolicSineHyperbolicSineClassic)
	ON_COMMAND(ID_HYPERBOLICCOSINE_HYPERBOLICCOSINE, OnHyperbolicCosineHyperbolicCosine)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICCOSINE_HYPERBOLICCOSINE, OnUpdateHyperbolicCosineHyperbolicCosine)
	ON_COMMAND(ID_HYPERBOLICCOSINE_HYPERBOLICCOSINEPLUS, OnHyperbolicCosineHyperbolicCosinePlus)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICCOSINE_HYPERBOLICCOSINEPLUS, OnUpdateHyperbolicCosineHyperbolicCosinePlus)
	ON_COMMAND(ID_HYPERBOLICCOSINE_HYPERBOLICCOSINECLASSIC, OnHyperbolicCosineHyperbolicCosineClassic)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICCOSINE_HYPERBOLICCOSINECLASSIC, OnUpdateHyperbolicCosineHyperbolicCosineClassic)
	ON_COMMAND(ID_HYPERBOLICTANGENT_HYPERBOLICTANGENT, OnHyperbolicTangentHyperbolicTangent)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICTANGENT_HYPERBOLICTANGENT, OnUpdateHyperbolicTangentHyperbolicTangent)
	ON_COMMAND(ID_HYPERBOLICTANGENT_HYPERBOLICTANGENTPLUS, OnHyperbolicTangentHyperbolicTangentPlus)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICTANGENT_HYPERBOLICTANGENTPLUS, OnUpdateHyperbolicTangentHyperbolicTangentPlus)
	ON_COMMAND(ID_HYPERBOLICTANGENT_HYPERBOLICTANGENTCLASSIC, OnHyperbolicTangentHyperbolicTangentClassic)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICTANGENT_HYPERBOLICTANGENTCLASSIC, OnUpdateHyperbolicTangentHyperbolicTangentClassic)
	ON_COMMAND(ID_HYPERBOLICSECANT_HYPERBOLICSECANT, OnHyperbolicSecantHyperbolicSecant)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICSECANT_HYPERBOLICSECANT, OnUpdateHyperbolicSecantHyperbolicSecant)
	ON_COMMAND(ID_HYPERBOLICSECANT_HYPERBOLICSECANTPLUS, OnHyperbolicSecantHyperbolicSecantPlus)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICSECANT_HYPERBOLICSECANTPLUS, OnUpdateHyperbolicSecantHyperbolicSecantPlus)
	ON_COMMAND(ID_HYPERBOLICSECANT_HYPERBOLICSECANTCLASSIC, OnHyperbolicSecantHyperbolicSecantClassic)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICSECANT_HYPERBOLICSECANTCLASSIC, OnUpdateHyperbolicSecantHyperbolicSecantClassic)
	ON_COMMAND(ID_HYPERBOLICCOSECANT_HYPERBOLICCOSECANT, OnHyperbolicCosecantHyperbolicCosecant)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICCOSECANT_HYPERBOLICCOSECANT, OnUpdateHyperbolicCosecantHyperbolicCosecant)
	ON_COMMAND(ID_HYPERBOLICCOSECANT_HYPERBOLICCOSECANTPLUS, OnHyperbolicCosecantHyperbolicCosecantPlus)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICCOSECANT_HYPERBOLICCOSECANTPLUS, OnUpdateHyperbolicCosecantHyperbolicCosecantPlus)
	ON_COMMAND(ID_HYPERBOLICCOSECANT_HYPERBOLICCOSECANTCLASSIC, OnHyperbolicCosecantHyperbolicCosecantClassic)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICCOSECANT_HYPERBOLICCOSECANTCLASSIC, OnUpdateHyperbolicCosecantHyperbolicCosecantClassic)
	ON_COMMAND(ID_HYPERBOLICCOTANGENT_HYPERBOLICCOTANGENT, OnHyperbolicCotangentHyperbolicCotangent)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICCOTANGENT_HYPERBOLICCOTANGENT, OnUpdateHyperbolicCotangentHyperbolicCotangent)
	ON_COMMAND(ID_HYPERBOLICCOTANGENT_HYPERBOLICCOTANGENTPLUS, OnHyperbolicCotangentHyperbolicCotangentPlus)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICCOTANGENT_HYPERBOLICCOTANGENTPLUS, OnUpdateHyperbolicCotangentHyperbolicCotangentPlus)
	ON_COMMAND(ID_HYPERBOLICCOTANGENT_HYPERBOLICCOTANGENTCLASSIC, OnHyperbolicCotangentHyperbolicCotangentClassic)
	ON_UPDATE_COMMAND_UI(ID_HYPERBOLICCOTANGENT_HYPERBOLICCOTANGENTCLASSIC, OnUpdateHyperbolicCotangentHyperbolicCotangentClassic)



	ON_BN_CLICKED(IDC_GENERATE, OnBnClickedGenerate)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_BN_CLICKED(IDC_MUL_COLUMN_DEFAULT_VARIABLES_RADIO, OnBnClickedMulColumnDefaultVariablesRadio)
	ON_BN_CLICKED(IDC_MUL_COLUMN_CONSTANT_VALUES_RADIO, OnBnClickedMulColumnConstantValuesRadio)
	ON_BN_CLICKED(IDC_MUL_COLUMN_CUSTOM_VARIABLES_RADIO, OnBnClickedMulColumnCustomVariablesRadio)
	ON_BN_CLICKED(IDC_MUL_MATRIX_DEFAULT_VARIABLES_RADIO, OnBnClickedMulMatrixDefaultVariablesRadio)
	ON_BN_CLICKED(IDC_MUL_MATRIX_CUSTOM_VARIABLES_RADIO, OnBnClickedMulMatrixCustomVariablesRadio)

	ON_BN_CLICKED(IDC_CULL_NON_SURFACE_POINTS_CHECK, OnBnClickedCullNonSurfacePointsCheck)
	ON_BN_CLICKED(IDC_HOPF_MAP_CHECK, OnBnClickedHopfMapCheck)
	ON_BN_CLICKED(IDC_SCREEN_RADIO, OnBnClickedScreenRadio)
	ON_BN_CLICKED(IDC_MESH_RADIO, OnBnClickedMeshRadio)
	ON_BN_CLICKED(IDC_CSV_FILE_RADIO, OnBnClickedCsvFileRadio)
	ON_BN_CLICKED(IDC_VOXEL_RADIO, OnBnClickedVoxelRadio)

	ON_UPDATE_COMMAND_UI(IDC_LIGHT_BY_NORMALS_CHECK, OnUpdateViewLBN)
	ON_UPDATE_COMMAND_UI(IDC_SHADER_COMBO, OnUpdateViewShaderCombo)
	ON_UPDATE_COMMAND_UI(IDC_NORMAL_QUALITY_EDIT, OnUpdateViewNormalRadiusEdit)
	
	ON_UPDATE_COMMAND_UI(IDC_REVERSE_SET_CHECK, OnUpdateViewReverseSet)
	ON_UPDATE_COMMAND_UI(IDC_CULL_NON_SURFACE_POINTS_CHECK, OnUpdateViewCNSP)
	ON_UPDATE_COMMAND_UI(IDC_HOPF_MAP_CHECK, OnUpdateViewHopfMap)
	ON_UPDATE_COMMAND_UI(IDC_SCREEN_RADIO, OnUpdateViewScreenRadio)
	ON_UPDATE_COMMAND_UI(IDC_MESH_RADIO, OnUpdateViewMeshRadio)
	ON_UPDATE_COMMAND_UI(IDC_CSV_FILE_RADIO, OnUpdateViewCsvFileRadio)
	ON_UPDATE_COMMAND_UI(IDC_VOXEL_RADIO, OnUpdateViewVoxelRadio)


	ON_UPDATE_COMMAND_UI(IDC_Z_W_EDIT, OnUpdateViewZW)
	ON_UPDATE_COMMAND_UI(IDC_W_MIN_EDIT, OnUpdateViewWMin)
	ON_UPDATE_COMMAND_UI(IDC_W_MAX_EDIT, OnUpdateViewWMax)
	ON_UPDATE_COMMAND_UI(IDC_SAMPLES_EDIT, OnUpdateViewSamples)







	ON_COMMAND(ID_VIEW_HIDEALL, OnViewHideall)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HIDEALL, OnUpdateViewHideall)
	ON_COMMAND(ID_VIEW_SHOWALL, OnViewShowall)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWALL, OnUpdateViewShowall)



	ON_BN_CLICKED(IDC_SWAP_Z_AND_C_CHECK, OnBnClickedSwapZAndCCheck)
	ON_UPDATE_COMMAND_UI(IDC_SWAP_Z_AND_C_CHECK, OnUpdateViewSwapZAndCEdit)


	ON_BN_CLICKED(IDC_RANDOMIZE_C_CHECK, OnBnClickedRandomizeCCheck)
	ON_BN_CLICKED(IDC_REVERSE_SET_CHECK, OnBnClickedReverseSetCheck)

	ON_EN_CHANGE(IDC_SAMPLES_EDIT, OnEnChangeSamplesEdit)
	ON_EN_CHANGE(IDC_FORMULA_EDIT, OnEnChangeFormulaEdit)
	ON_EN_CHANGE(IDC_C_X_EDIT, OnEnChangeCXEdit)
	ON_EN_CHANGE(IDC_C_Y_EDIT, OnEnChangeCYEdit)
	ON_EN_CHANGE(IDC_C_Z_EDIT, OnEnChangeCZEdit)
	ON_EN_CHANGE(IDC_C_W_EDIT, OnEnChangeCWEdit)
	ON_EN_CHANGE(IDC_X_MIN_EDIT, OnEnChangeXMinEdit)
	ON_EN_CHANGE(IDC_Y_MIN_EDIT, OnEnChangeYMinEdit)
	ON_EN_CHANGE(IDC_Z_MIN_EDIT, OnEnChangeZMinEdit)
	ON_EN_CHANGE(IDC_W_MIN_EDIT, OnEnChangeWMinEdit)
	ON_EN_CHANGE(IDC_X_MAX_EDIT, OnEnChangeXMaxEdit)
	ON_EN_CHANGE(IDC_Y_MAX_EDIT, OnEnChangeYMaxEdit)
	ON_EN_CHANGE(IDC_Z_MAX_EDIT, OnEnChangeZMaxEdit)
	ON_EN_CHANGE(IDC_W_MAX_EDIT, OnEnChangeWMaxEdit)
	ON_EN_CHANGE(IDC_Z_W_EDIT, OnEnChangeZWEdit)
	ON_EN_CHANGE(IDC_A_EDIT, OnEnChangeAEdit)
	ON_EN_CHANGE(IDC_ITERATIONS_EDIT, OnEnChangeIterationsEdit)
	ON_EN_CHANGE(IDC_INFINITY_EDIT, OnEnChangeInfinityEdit)
	ON_EN_CHANGE(IDC_RESOLUTION_EDIT, OnEnChangeResolutionEdit)
	ON_EN_CHANGE(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT, OnEnChangeMulColumnConstantValuesXEdit)
	ON_EN_CHANGE(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT, OnEnChangeMulColumnConstantValuesYEdit)
	ON_EN_CHANGE(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT, OnEnChangeMulColumnConstantValuesZEdit)
	ON_EN_CHANGE(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT, OnEnChangeMulColumnConstantValuesWEdit)
	ON_EN_CHANGE(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT, OnEnChangeMulColumnCustomVariablesXEdit)
	ON_EN_CHANGE(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT, OnEnChangeMulColumnCustomVariablesYEdit)
	ON_EN_CHANGE(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT, OnEnChangeMulColumnCustomVariablesZEdit)
	ON_EN_CHANGE(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT, OnEnChangeMulColumnCustomVariablesWEdit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_11_EDIT, OnEnChangeMulMatrixCustom11Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_12_EDIT, OnEnChangeMulMatrixCustom12Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_13_EDIT, OnEnChangeMulMatrixCustom13Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_14_EDIT, OnEnChangeMulMatrixCustom14Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_21_EDIT, OnEnChangeMulMatrixCustom21Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_22_EDIT, OnEnChangeMulMatrixCustom22Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_23_EDIT, OnEnChangeMulMatrixCustom23Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_24_EDIT, OnEnChangeMulMatrixCustom24Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_31_EDIT, OnEnChangeMulMatrixCustom31Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_32_EDIT, OnEnChangeMulMatrixCustom32Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_33_EDIT, OnEnChangeMulMatrixCustom33Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_34_EDIT, OnEnChangeMulMatrixCustom34Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_41_EDIT, OnEnChangeMulMatrixCustom41Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_42_EDIT, OnEnChangeMulMatrixCustom42Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_43_EDIT, OnEnChangeMulMatrixCustom43Edit)
	ON_EN_CHANGE(IDC_MUL_MATRIX_CUSTOM_44_EDIT, OnEnChangeMulMatrixCustom44Edit)


	ON_COMMAND(ID_VIEW_RESTOREALL, OnViewRestoreall)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESTOREALL, OnUpdateViewRestoreall)
	ON_UPDATE_COMMAND_UI(ID_HELP_CONTENTS, OnUpdateHelpContents)

	ON_UPDATE_COMMAND_UI(ID_RENDERER_ZOOM_MINUS, OnUpdateRendererZoomMinus)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_POINTSIZE_PLUS, OnUpdateRendererPointsizePlus)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_POINTSIZE_MINUS, OnUpdateRendererPointsizeMinus)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_FOG, OnUpdateRendererFog)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_BOUNDINGGRID, OnUpdateRendererBoundinggrid)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_ZSLICE_PLUS, OnUpdateRendererZslicePlus)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_ZSLICE_MINUS, OnUpdateRendererZsliceMinus)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_SINGLEZSLICE, OnUpdateRendererSinglezslice)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_RESETZSLICING, OnUpdateRendererResetzslicing)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_RESETZTR, OnUpdateRendererResetztr)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)


	ON_COMMAND(ID_RENDERER_ZOOM_MINUS, OnRendererZoomMinus)


	ON_COMMAND(ID_RENDERER_POINTSIZE_PLUS, OnRendererPointsizePlus)
	ON_COMMAND(ID_RENDERER_POINTSIZE_MINUS, OnRendererPointsizeMinus)
	ON_COMMAND(ID_RENDERER_FOG, OnRendererFog)
	ON_COMMAND(ID_RENDERER_BOUNDINGGRID, OnRendererBoundinggrid)
	ON_COMMAND(ID_RENDERER_ZSLICE_PLUS, OnRendererZslicePlus)
	ON_COMMAND(ID_RENDERER_ZSLICE_MINUS, OnRendererZsliceMinus)
	ON_COMMAND(ID_RENDERER_SINGLEZSLICE, OnRendererSinglezslice)
	ON_COMMAND(ID_RENDERER_RESETZSLICING, OnRendererResetzslicing)
	ON_COMMAND(ID_RENDERER_RESETZTR, OnRendererResetztr)
	ON_COMMAND(ID_RENDERER_ZOOM_PLUS, OnRendererZoomPlus)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_ZOOM_PLUS, OnUpdateRendererZoomPlus)
	ON_COMMAND(ID_RENDERER_INTERFACETEXT, OnRendererInterfacetext)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_INTERFACETEXT, OnUpdateRendererInterfacetext)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_SETBACKGROUNDCOLOR, OnUpdateRendererSetbackgroundcolor)
	ON_COMMAND(ID_RENDERER_SETBACKGROUNDCOLOR, OnRendererSetbackgroundcolor)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_SETFOGCOLOR, OnUpdateRendererSetfogcolor)
	ON_COMMAND(ID_RENDERER_SETFOGCOLOR, OnRendererSetfogcolor)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_SETINTERFACETEXTCOLOUR, OnUpdateRendererSetinterfacetextcolour)
	ON_COMMAND(ID_RENDERER_SETINTERFACETEXTCOLOUR, OnRendererSetinterfacetextcolour)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_RESETBOUNDINGGRID, OnUpdateRendererResetboundinggrid)
	ON_COMMAND(ID_RENDERER_RESETBOUNDINGGRID, OnRendererResetboundinggrid)
	ON_UPDATE_COMMAND_UI(ID_RENDERER_RESTOREDEFAULTCOLOURS, OnUpdateRendererRestoredefaultcolours)
	ON_COMMAND(ID_RENDERER_RESTOREDEFAULTCOLOURS, OnRendererRestoredefaultcolours)
	ON_UPDATE_COMMAND_UI(ID_INTERFACEELEMENTS_AXISINDICATORS, OnUpdateInterfaceelementsAxisindicators)
	ON_COMMAND(ID_INTERFACEELEMENTS_AXISINDICATORS, OnInterfaceelementsAxisindicators)
	ON_UPDATE_COMMAND_UI(ID_FILE_GENERATE, OnUpdateFileGenerate)
	ON_COMMAND(ID_FILE_GENERATE, OnFileGenerate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CACHESETTINGS, OnUpdateEditCachesettings)
	ON_COMMAND(ID_EDIT_CACHESETTINGS, OnEditCachesettings)
	ON_BN_CLICKED(IDC_MUL_GEN_BUTTON, OnBnClickedMulGenButton)
	ON_COMMAND(ID_VOXEL_OPACITY_PLUS, OnVoxelOpacityPlus)
	ON_UPDATE_COMMAND_UI(ID_VOXEL_OPACITY_PLUS, OnUpdateVoxelOpacityPlus)
	ON_COMMAND(ID_VOXEL_OPACITY_MINUS, OnVoxelOpacityMinus)
	ON_UPDATE_COMMAND_UI(ID_VOXEL_OPACITY_MINUS, OnUpdateVoxelOpacityMinus)
	ON_COMMAND(ID_HELP_LICENSETERMS, OnHelpLicenseterms)
	ON_UPDATE_COMMAND_UI(ID_HELP_LICENSETERMS, OnUpdateHelpLicenseterms)
	ON_BN_CLICKED(IDC_RENDER_TO_GEN_BUTTON, OnBnClickedRenderToGenButton)
	ON_BN_CLICKED(IDC_EXTRA_GEN_BUTTON, OnBnClickedExtraGenButton)
	ON_CBN_SELCHANGE(IDC_SHADER_COMBO, OnCbnSelchangeShaderCombo)
	ON_COMMAND(ID_AMBIENCE_LEVEL_PLUS, OnAmbienceLevelPlus)
	ON_UPDATE_COMMAND_UI(ID_AMBIENCE_LEVEL_PLUS, OnUpdateAmbienceLevelPlus)
	ON_COMMAND(ID_AMBIENCE_LEVEL_MINUS, OnAmbienceLevelMins)
	ON_UPDATE_COMMAND_UI(ID_AMBIENCE_LEVEL_MINUS, OnUpdateAmbienceLevelMins)
	ON_EN_CHANGE(IDC_HOPF_FIELD_RESOLUTION_EDIT, OnEnChangeHopfFieldResolutionEdit)
	ON_UPDATE_COMMAND_UI(IDC_HOPF_FIELD_RESOLUTION_EDIT, OnUpdateViewHopfFieldResolutionEdit)
	ON_BN_CLICKED(IDC_LIGHT_BY_NORMALS_CHECK, OnBnClickedLightByNormalsCheck)
	ON_BN_CLICKED(IDC_PROP_RES_CHECK, OnBnClickedPropResCheck)
	ON_UPDATE_COMMAND_UI(IDC_PROP_RES_CHECK, OnUpdateViewPropResCheck)
	END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	srand(static_cast<unsigned int>(time(0)));
}

CMainFrame::~CMainFrame()
{

}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);

	EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);




	CRect rect;

	if( !m_wndMultiplicationSettingsDialog.Create(this, IDD_MULTIPLICATION_SETTINGS, CBRS_LEFT | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY, IDD_MULTIPLICATION_SETTINGS) )
	{
		TRACE0(_T("Failed to create the Multiplication Settings dialogbar\n"));
		return -1;
	}
	else
	{
		m_wndMultiplicationSettingsDialog.SetWindowText("Multiplication Settings");
		m_wndMultiplicationSettingsDialog.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
		DockControlBar(&m_wndMultiplicationSettingsDialog, AFX_IDW_DOCKBAR_LEFT);
	}

	RecalcLayout();
	m_wndMultiplicationSettingsDialog.GetWindowRect(&rect);
	rect.OffsetRect(1, 0);


	if( !m_wndRenderToSettingsDialog.Create(this, IDD_RENDER_TO_SETTINGS, CBRS_LEFT | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY, IDD_RENDER_TO_SETTINGS) )
	{
		TRACE0(_T("Failed to create the RenderTo Settings dialogbar\n"));
		return -1;
	}
	else
	{
		m_wndRenderToSettingsDialog.SetWindowText("Render To Settings");
		m_wndRenderToSettingsDialog.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
		DockControlBar(&m_wndRenderToSettingsDialog, AFX_IDW_DOCKBAR_LEFT, &rect);
	}

	RecalcLayout();
	m_wndRenderToSettingsDialog.GetWindowRect(&rect);
	rect.OffsetRect(1, 0);

	if( !m_wndMainSettingsDialog.Create(this, IDD_MAIN_SETTINGS, CBRS_LEFT | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY, IDD_MAIN_SETTINGS) )
	{
		TRACE0(_T("Failed to create the Main Settings dialogbar\n"));
		return -1;
	}
	else
	{
		m_wndMainSettingsDialog.SetWindowText("Main Settings");
		m_wndMainSettingsDialog.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
		DockControlBar(&m_wndMainSettingsDialog, AFX_IDW_DOCKBAR_LEFT, &rect);
	}

	RecalcLayout();
	m_wndMainSettingsDialog.GetWindowRect(&rect);
	rect.OffsetRect(1, 0);

	if( !m_wndExtraSettingsDialog.Create(this, IDD_EXTRA_SETTINGS, CBRS_LEFT | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY, IDD_EXTRA_SETTINGS) )
	{
		TRACE0(_T("Failed to create the Extra Settings dialogbar\n"));
		return -1;
	}
	else
	{
		m_wndExtraSettingsDialog.SetWindowText("Extra Settings");
		m_wndExtraSettingsDialog.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
		DockControlBar(&m_wndExtraSettingsDialog, AFX_IDW_DOCKBAR_LEFT, &rect);
	}

	SaveBarState("Julia 4D 2 Default Settings");

	LoadBarState("Julia 4D 2 Settings");
	RecalcLayout();

	if(!set_gen_pkg.Init(this->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd))
	{
		ostringstream error;
		error << "Could not initialize set generation worker object.\n" << set_gen_pkg.GetLastError();
		::MessageBox(m_hWnd, error.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP); 
		return -1;
	}

	RECT frame_rect, erase_rect;
	this->GetDrawableClientRect(frame_rect, erase_rect);
	Renderer.SetDrawableRect(frame_rect);
	Renderer.SetScreenEraseRect(erase_rect);

	if(!Renderer.Init(this->m_hWnd))
	{
		ostringstream error;
		error << "Could not initialize Direct 3D renderer.\n" << Renderer.GetLastError();
		::MessageBox(m_hWnd, error.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP); 
		return -1;
	}

	string interface_header = APP_NAME;
	interface_header += " v";
	interface_header += APP_VERSION;
	interface_header += " - Copyright (c) 2004-2005 Shawn Halayka";
	interface_header += "\nSee Help > About for 3rd party contributor information";
	Renderer.SetInterfaceHeaderText(interface_header);
	Renderer.SetBoundingGrid(-1.5f, 1.5f, -1.5f, 1.5f, -1.5f, 1.5f);
	Renderer.SetAmbience(AfxGetApp()->GetProfileInt("Settings", "RendererAmbience", 0));
	Renderer.UseLitShader(true);
	m_wndExtraSettingsDialog.GetDlgItem(IDC_LIGHT_BY_NORMALS_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);

	long signed int R = 0;
	long signed int G = 0;
	long signed int B = 0;

	R = AfxGetApp()->GetProfileInt("Settings", "BG_R", 65535);
	G = AfxGetApp()->GetProfileInt("Settings", "BG_G", 65535);
	B = AfxGetApp()->GetProfileInt("Settings", "BG_B", 65535);

	if(R != 65535 && G != 65535 && B != 65535)
		Renderer.SetBackgroundColour(static_cast<unsigned char>(R), static_cast<unsigned char>(G), static_cast<unsigned char>(B));

	R = AfxGetApp()->GetProfileInt("Settings", "IT_R", 65535);
	G = AfxGetApp()->GetProfileInt("Settings", "IT_G", 65535);
	B = AfxGetApp()->GetProfileInt("Settings", "IT_B", 65535);

	if(R != 65535 && G != 65535 && B != 65535)
		Renderer.SetInterfaceTextColour(Renderer.GetInterfaceTextColourA(), static_cast<unsigned char>(R), static_cast<unsigned char>(G), static_cast<unsigned char>(B));

	R = AfxGetApp()->GetProfileInt("Settings", "F_R", 65535);
	G = AfxGetApp()->GetProfileInt("Settings", "F_G", 65535);
	B = AfxGetApp()->GetProfileInt("Settings", "F_B", 65535);

	if(R != 65535 && G != 65535 && B != 65535)
		Renderer.SetFogColour(static_cast<unsigned char>(R), static_cast<unsigned char>(G), static_cast<unsigned char>(B));


	if(!Renderer.Play())
	{
//		ostringstream error;
//		error << "Could not start Direct 3D renderer.\n" << Renderer.GetLastError();
//		::MessageBox(m_hWnd, error.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP); 
//		return -1;
	}

	creation_complete = true;
	generating_set = false;
	waiting_for_cancel = false;
	about_box_showing = false;

	renderer_reset_required = false;
	generation_start = 0.0;
	previous_set_completed = false;

	for(long unsigned int i = 0; i < 8; i++)
		acrCustClr[i] = RGB(Renderer.GetFogColourR(), Renderer.GetFogColourG(), Renderer.GetFogColourB());

	for(long unsigned int i = 8; i < 16; i++)
		acrCustClr[i] = RGB(Renderer.GetBackgroundColourR(), Renderer.GetBackgroundColourG(), Renderer.GetBackgroundColourB());

	//	ResetVars();

	//	RecalcLayout();



	m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"Distance");
	m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"Dot Product");
	m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"Perlin Cow");
	m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"Bronze");
	m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"Silver");
	m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"Gold");
	m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"Grey");
	m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"Red");
	m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"Green");
	m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"Blue");

	m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_SETCURSEL, 0, 0);


	time_t now;
	time(&now);
	char time_buf[128];
	if(0 ==	strftime(time_buf, 128, "%b %d %Y %H:%M:%S", localtime(&now)))
		m_wndMainSettingsDialog.SetDlgItemText(IDC_STATUS_EDIT, "Initialized...");
	else
		m_wndMainSettingsDialog.SetDlgItemText(IDC_STATUS_EDIT, time_buf);

	return 0;
}


void CMainFrame::ActivateFrame(int nCmdShow)
{
	// print one time real estate warning, if necessary
	if(GetSystemMetrics(SM_CXSCREEN) < 1280 || GetSystemMetrics(SM_CYSCREEN) < 1024)
	{
		long signed int show_real_estate_warning = 0;
		
		if(65535 == AfxGetApp()->GetProfileInt("Settings", "RealEstateWarn", 65535))
		{
			MessageBox("Please note:\n    Julia 4D 2 was designed for optimal use at 1280x1024 screen resolution, or better.\n    This message will not appear again.", APP_NAME, MB_OK|MB_ICONINFORMATION);
			AfxGetApp()->WriteProfileInt("Settings", "RealEstateWarn", 0);
		}
	}

	if (nCmdShow == -1)
	{
		if (!IsWindowVisible())
			nCmdShow = SW_SHOWNORMAL;
		else if (IsIconic())
			nCmdShow = SW_RESTORE;
	}

	BringToTop(nCmdShow);

	if(nCmdShow == -1)
		return;

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);

	wp.flags = AfxGetApp()->GetProfileInt("Settings", "wp_flags", 65535);
	wp.showCmd = AfxGetApp()->GetProfileInt("Settings", "wp_showCmd", 65535);

	wp.ptMinPosition.x = AfxGetApp()->GetProfileInt("Settings", "wp_ptMinPosition_x", 65535);
	wp.ptMinPosition.y = AfxGetApp()->GetProfileInt("Settings", "wp_ptMinPosition_y", 65535);

	wp.ptMaxPosition.x = AfxGetApp()->GetProfileInt("Settings", "wp_ptMaxPosition_x", 65535);
	wp.ptMaxPosition.y = AfxGetApp()->GetProfileInt("Settings", "wp_ptMaxPosition_y", 65535);

	wp.rcNormalPosition.bottom = AfxGetApp()->GetProfileInt("Settings", "wp_rcNormalPosition_bottom", 65535);
	wp.rcNormalPosition.left = AfxGetApp()->GetProfileInt("Settings", "wp_rcNormalPosition_left", 65535);
	wp.rcNormalPosition.right = AfxGetApp()->GetProfileInt("Settings", "wp_rcNormalPosition_right", 65535);
	wp.rcNormalPosition.top = AfxGetApp()->GetProfileInt("Settings", "wp_rcNormalPosition_top", 65535);

	if(wp.length != 65535 && 
		wp.flags != 65535 && 
		wp.showCmd != 65535 && 
		wp.ptMinPosition.x != 65535 && 
		wp.ptMinPosition.y != 65535 && 
		wp.ptMaxPosition.x != 65535 && 
		wp.ptMaxPosition.y != 65535 && 
		wp.rcNormalPosition.bottom != 65535 && 
		wp.rcNormalPosition.left != 65535 && 
		wp.rcNormalPosition.right != 65535 && 
		wp.rcNormalPosition.top != 65535 )
	{
		if(nCmdShow != SW_SHOWNORMAL)
			wp.showCmd = nCmdShow;

		SetWindowPlacement(&wp);
	}
	else
	{
		ShowWindow(SW_MAXIMIZE);
	}

	BringToTop(nCmdShow);


	// show one time splash screen
	if(65535 == AfxGetApp()->GetProfileInt("Settings", "EnableSplashScreen", 65535))
	{
		CSplashDlg splashDlg;
		splashDlg.DoModal();
		AfxGetApp()->WriteProfileInt("Settings", "EnableSplashScreen", 0);
	}

	return ;
}




void CMainFrame::OnClose()
{
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);

	if(this->GetWindowPlacement(&wp))
	{
		/*
		UINT length;
		UINT flags;
		UINT showCmd;
		POINT ptMinPosition;
		POINT ptMaxPosition;
		RECT rcNormalPosition;
		*/

		if(IsIconic())
			wp.showCmd = SW_SHOW ;

		if ((wp.flags & WPF_RESTORETOMAXIMIZED) != 0)
			wp.showCmd = SW_SHOWMAXIMIZED;

		AfxGetApp()->WriteProfileInt("Settings", "wp_flags", wp.flags);
		AfxGetApp()->WriteProfileInt("Settings", "wp_showCmd", wp.showCmd);

		AfxGetApp()->WriteProfileInt("Settings", "wp_ptMinPosition_x", wp.ptMinPosition.x);
		AfxGetApp()->WriteProfileInt("Settings", "wp_ptMinPosition_y", wp.ptMinPosition.y);

		AfxGetApp()->WriteProfileInt("Settings", "wp_ptMaxPosition_x", wp.ptMaxPosition.x);
		AfxGetApp()->WriteProfileInt("Settings", "wp_ptMaxPosition_y", wp.ptMaxPosition.y);

		AfxGetApp()->WriteProfileInt("Settings", "wp_rcNormalPosition_bottom", wp.rcNormalPosition.bottom);
		AfxGetApp()->WriteProfileInt("Settings", "wp_rcNormalPosition_left", wp.rcNormalPosition.left);
		AfxGetApp()->WriteProfileInt("Settings", "wp_rcNormalPosition_right", wp.rcNormalPosition.right);
		AfxGetApp()->WriteProfileInt("Settings", "wp_rcNormalPosition_top", wp.rcNormalPosition.top);
	}

	SaveBarState("Julia 4D 2 Settings");

	
	AfxGetApp()->WriteProfileInt("Settings", "RendererAmbience", Renderer.GetAmbience());

	// stop the renderer
	Renderer.Stop();

	// if generating, cancel
	if(generating_set)
	{
		if(IDNO == MessageBox("You are currently generating a set. Do you really want to quit?\n\nIf the cache has been enabled, any new set/surface calculations will be saved to the cache before the application completely exits.\nYou will also be asked to save any set parameter changes that may have occurred during your session.", APP_NAME, MB_YESNO|MB_ICONWARNING))
			return;

		OnBnClickedGenerate();
	}

	CFrameWnd::OnClose();
}

void CMainFrame::OnPaint()
{
	RecalcLayout();

	CWnd::OnPaint();

	if(renderer_reset_required)
		OnRecreateRenderer(0, 0);

	Renderer.Play();

//	if(!Renderer.Play())
//	{
//		ostringstream error;
//		error << "Could not restart Direct 3D renderer (WM_PAINT).\n" << Renderer.GetLastError();
//		::MessageBox(m_hWnd, error.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP); 
//		this->SendMessage(WM_CLOSE);
//	}

	if(generating_set)
	{
		::Sleep(100);
		Renderer.Stop();
	}

	// get rid of annoying flicker in direct3d's target region
	RECT drawable_rect, temp_rect;
	GetDrawableClientRect(drawable_rect, temp_rect); 
	this->ValidateRect(&drawable_rect);

	// let the base class handle the redraw of the dBocked windows and status bar
}


void CMainFrame::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	if(!bActive)
		Renderer.SetSnoozeFactor(500);
	else
	{
		if(renderer_reset_required)
		{
			if(!OnRecreateRenderer(0, 0))
				return;
		}

		Renderer.SetSnoozeFactor(0);
	}
}


void CMainFrame::OnHelp(void)
{
	CURL url;
	url.Open(_T("http://cwiki.org/index.php/Julia_4D_2_Help"));
}


long CMainFrame::OnDisplayChange( UINT nParam, LONG lParam)
{
	RecalcLayout();

	renderer_reset_required = true;
	return 1;
}



long CMainFrame::OnRecreateRenderer( UINT nParam, LONG lParam)
{
	this->ShowWindow(SW_MAXIMIZE);

	Renderer.Reset(m_hWnd);
	Renderer.Play();

	if(!Renderer.Reset(m_hWnd))
	{
		renderer_reset_required = true;
		return 0;

//		ostringstream error;
//		error << "Could not reset Direct 3D renderer.\n" << Renderer.GetLastError();
//		MessageBox(error.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP);
//		this->SendMessage(WM_CLOSE);
//		return 0;
	}
	else
	{
		renderer_reset_required = false;

		if(!Renderer.Play())
		{
//			ostringstream error;
//			error << "Could not start Direct 3D renderer.\n" << Renderer.GetLastError();
//			MessageBox(error.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP);
//			this->SendMessage(WM_CLOSE);
//			return 0;
		}
	}

	return 1;
}

long CMainFrame::OnSetGenSuccess( UINT nParam, LONG lParam)
{

#ifdef _DEBUG
	ostringstream debug_out;
	std::streambuf* coutBuf  = std::cout.rdbuf();

	std::cout.rdbuf(debug_out.rdbuf());
	_CrtDumpMemoryLeaks();
	std::cout.rdbuf(coutBuf);

	MessageBox(debug_out.str().c_str(), APP_NAME, MB_OK);
#endif


	if(!waiting_for_cancel)
	{
		if(set_gen_pkg.params.render_to_type == POINT_SET)
		{
			bool random_colour = false;

			::SetWindowText(m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd, "Filling vertex buffer...");
			::UpdateWindow(m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd);

			if(BST_CHECKED == m_wndExtraSettingsDialog.GetDlgItem(IDC_LIGHT_BY_NORMALS_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
				Renderer.UseLitShader(true);
			else
				Renderer.UseLitShader(false);

			Renderer.SetPoints(set_gen_pkg.vertices, set_gen_pkg.vertex_count, set_gen_pkg.z_slice_offsets);

			LRESULT shader_number = m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

			if(0 == shader_number || CB_ERR == shader_number)
				Renderer.RainbowColouring(m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd);  //ColourVertices(m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd, set_gen_pkg.vertices, set_gen_pkg.vertex_count, random_colour);
			else if(1 == shader_number)
				Renderer.DotProdColouring(m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd);  //ColourVertices(m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd, set_gen_pkg.vertices, set_gen_pkg.vertex_count, random_colour);
			else if(2 == shader_number)
				Renderer.CowColouring(m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd);
			else
				Renderer.FlatColouring(static_cast<DWORD>(shader_number), m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd);
		}

		::SetWindowText(m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd, "Done...");
	}
	else
	{
		m_wndExtraSettingsDialog.GetDlgItem(IDC_RANDOMIZE_C_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_MATRIX_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);

		::SetWindowText(m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd, "Cancelled...");
	}
		

	::UpdateWindow(m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd);


	generating_set = false;
	waiting_for_cancel = false;
	m_wndMainSettingsDialog.GetDlgItem(IDC_GENERATE)->SetWindowText("Generate");


	// notify user of completion
	if(this->GetActiveWindow() != this)
		this->FlashWindowEx(FLASHW_ALL, 2, 0);




	// begin the render loop
	Renderer.SetGeneratingFlag(false);
	if(!Renderer.Play())
	{
//		ostringstream error;
//		error << "Could not start Direct 3D renderer.\n" << Renderer.GetLastError();
//		MessageBox(error.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP);
//		this->SendMessage(WM_CLOSE);
//		return 0;
	}


	double duration = (double)(clock() - generation_start) / CLOCKS_PER_SEC;

	ostringstream out;
	out << "Main Settings: " << duration << "s";

	if(m_wndMainSettingsDialog.IsFloating())
		m_wndMainSettingsDialog.GetDockingFrame()->SetWindowText(out.str().c_str());
	else
		m_wndMainSettingsDialog.SetWindowText(out.str().c_str());

	return 1;
}


long CMainFrame::OnSetGenFailure( UINT nParam, LONG lParam)
{
	::SetWindowText(m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd, "Failed...");
	::UpdateWindow(m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd);

	generating_set = false;
	waiting_for_cancel = false;
	m_wndMainSettingsDialog.GetDlgItem(IDC_GENERATE)->SetWindowText("Generate");

	double duration = (double)(clock() - generation_start) / CLOCKS_PER_SEC;

	ostringstream out;
	out << "Main Settings: " << duration << "s";

	if(m_wndMainSettingsDialog.IsFloating())
		m_wndMainSettingsDialog.GetDockingFrame()->SetWindowText(out.str().c_str());
	else
		m_wndMainSettingsDialog.SetWindowText(out.str().c_str());


	Renderer.SetGeneratingFlag(false);
	if(!Renderer.Play())
	{
//		ostringstream error;
//		error << "Could not start Direct 3D renderer.\n" << Renderer.GetLastError();
//		MessageBox(error.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP);
//		this->SendMessage(WM_CLOSE);
//		return 0;
	}

	return 1;
}



long CMainFrame::OnBoundingGridChanged( UINT nParam, LONG lParam)
{
	BOUNDING_GRID_SETTINGS bgs;

	bgs = Renderer.GetBoundingGrid();

	ostringstream out;

	out << bgs.x_min;
	m_wndMainSettingsDialog.SetDlgItemText(IDC_X_MIN_EDIT, out.str().c_str());
	out.str("");

	out << bgs.x_max;
	m_wndMainSettingsDialog.SetDlgItemText(IDC_X_MAX_EDIT, out.str().c_str());
	out.str("");

	out << bgs.y_min;
	m_wndMainSettingsDialog.SetDlgItemText(IDC_Y_MIN_EDIT, out.str().c_str());
	out.str("");

	out << bgs.y_max;
	m_wndMainSettingsDialog.SetDlgItemText(IDC_Y_MAX_EDIT, out.str().c_str());
	out.str("");

	out << bgs.z_min;
	m_wndMainSettingsDialog.SetDlgItemText(IDC_Z_MIN_EDIT, out.str().c_str());
	out.str("");

	out << bgs.z_max;
	m_wndMainSettingsDialog.SetDlgItemText(IDC_Z_MAX_EDIT, out.str().c_str());
	out.str("");	

	return 1;
}




void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	Renderer.HandleInputMessages(this->m_hWnd, WM_MOUSEMOVE, nFlags, MAKELPARAM(point.x, point.y));
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	Renderer.HandleInputMessages(this->m_hWnd, WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
}

void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	Renderer.HandleInputMessages(this->m_hWnd, WM_LBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));
}

void CMainFrame::OnRButtonDown(UINT nFlags, CPoint point)
{
	Renderer.HandleInputMessages(this->m_hWnd, WM_RBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
}

void CMainFrame::OnRButtonUp(UINT nFlags, CPoint point)
{
	Renderer.HandleInputMessages(this->m_hWnd, WM_RBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));
}

void CMainFrame::OnMButtonDown(UINT nFlags, CPoint point)
{
	Renderer.HandleInputMessages(this->m_hWnd, WM_MBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));
}

void CMainFrame::OnMButtonUp(UINT nFlags, CPoint point)
{
	Renderer.HandleInputMessages(this->m_hWnd, WM_MBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	Renderer.HandleInputMessages(this->m_hWnd, WM_KEYDOWN, nChar, 0);
}

BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta > 0)
		Renderer.IncreaseZoom();
	else
		Renderer.DecreaseZoom();

	return TRUE;
}





BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION | FWS_ADDTOTITLE
		| WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), NULL, NULL);

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers


void CMainFrame::OnViewMainSettings()
{
	ShowControlBar( &m_wndMainSettingsDialog, !m_wndMainSettingsDialog.IsWindowVisible(), TRUE );
}

void CMainFrame::OnUpdateViewMainSettings(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
	pCmdUI->SetCheck( m_wndMainSettingsDialog.IsWindowVisible() );
}

void CMainFrame::OnViewExtraSettings()
{
	ShowControlBar( &m_wndExtraSettingsDialog, !m_wndExtraSettingsDialog.IsWindowVisible(), TRUE );
}

void CMainFrame::OnUpdateViewExtraSettings(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
	pCmdUI->SetCheck( m_wndExtraSettingsDialog.IsWindowVisible() );
}



void CMainFrame::OnViewMultiplicationSettings()
{
	ShowControlBar( &m_wndMultiplicationSettingsDialog, !m_wndMultiplicationSettingsDialog.IsWindowVisible(), TRUE );
}

void CMainFrame::OnUpdateViewMultiplicationSettings(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
	pCmdUI->SetCheck( m_wndMultiplicationSettingsDialog.IsWindowVisible() );
}

void CMainFrame::OnViewRenderToSettings()
{
	ShowControlBar( &m_wndRenderToSettingsDialog, !m_wndRenderToSettingsDialog.IsWindowVisible(), TRUE );
}

void CMainFrame::OnUpdateViewRenderToSettings(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
	pCmdUI->SetCheck( m_wndRenderToSettingsDialog.IsWindowVisible() );
}

void CMainFrame::OnUpdateViewFormulaMenuButton(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateViewGenerateButton(CCmdUI *pCmdUI)
{
	if(waiting_for_cancel)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	Renderer.Stop();

	//RecalcLayout(); // this caused a bug where the docking windows would see the window as very short
	// and dock them selves side by side every time the app was min then maximized
	RECT frame_rect, erase_rect;
	this->GetDrawableClientRect(frame_rect, erase_rect);
	Renderer.SetDrawableRect(frame_rect);
	Renderer.SetScreenEraseRect(erase_rect);



	CWnd::OnPaint();

	if(creation_complete)
	{
		if(renderer_reset_required)
		{
			if(!OnRecreateRenderer(0, 0))
				return;
		}

		Renderer.Play();
//		if(!Renderer.Play())
//		{
//			ostringstream error;
//			error << "Could not start Direct 3D renderer (WM_SIZE).\n" << Renderer.GetLastError();
//			MessageBox(error.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP);
//			this->SendMessage(WM_CLOSE);
//			return;
//		}
	}
}


void CMainFrame::OnBnClickedFormulaMenu()
{
	HMENU hFormulaMenu = GetSubMenu(LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_FORMULA_MENU)), 0);	

	if(0 != hFormulaMenu)
	{
		POINT mousepos;
		GetCursorPos(&mousepos);

		TrackPopupMenuEx(hFormulaMenu, NULL, mousepos.x, mousepos.y, this->m_hWnd, NULL);
		DestroyMenu(hFormulaMenu);
	}
	else
	{
		MessageBox("Error while loading formula menu.", APP_NAME, MB_OK);
	}
}





void CMainFrame::GetDrawableClientRect(RECT &src_frame_rect, RECT &src_erase_rect)
{
	this->GetClientRect(&src_frame_rect);
	src_erase_rect = src_frame_rect;
	this->ClientToScreen(&src_erase_rect);

	src_erase_rect.bottom += src_erase_rect.top;
	src_erase_rect.top = 0;

	if(creation_complete)
	{
		// obtain docking and status bar sizes
		long signed int left_bar_width = 0;
		long signed int right_bar_width = 0;
		long signed int status_bar_height = 0;

		CControlBar* pDockL; 
		CControlBar* pDockR;
		CControlBar *pStatusBar;

		pDockL = GetControlBar(AFX_IDW_DOCKBAR_LEFT);
		pDockR = GetControlBar(AFX_IDW_DOCKBAR_RIGHT);

		pStatusBar = GetControlBar(AFX_IDW_STATUS_BAR);

		RECT crect;

		pDockL->GetClientRect(&crect);

		left_bar_width = crect.right - crect.left;

		if(left_bar_width < 0)
			left_bar_width = 0;

		pDockR->GetClientRect(&crect);

		right_bar_width = crect.right - crect.left;

		if(right_bar_width < 0)
			right_bar_width = 0;

		if(pStatusBar->IsVisible())
		{
			RECT MainWindowRect;
			RECT StatusBarRect;

			pStatusBar->GetWindowRect(&StatusBarRect);
			this->GetWindowRect(&MainWindowRect);

			if(StatusBarRect.top < MainWindowRect.bottom + 3)
				status_bar_height = MainWindowRect.bottom - StatusBarRect.top - 3;
		}

		// trim frame client rect
		if(left_bar_width != 0)
		{
			src_frame_rect.left += left_bar_width;
			//			src_erase_rect.left += left_bar_width;
		}

		if(right_bar_width != 0)
		{
			src_frame_rect.right -= right_bar_width;
			//			src_erase_rect.right -= right_bar_width;
		}

		if(status_bar_height != 0)
		{
			src_frame_rect.bottom -= status_bar_height;
			//			src_erase_rect.bottom -= status_bar_height;
		}
	}
}





void CMainFrame::OnBnClickedGenerate()
{
	if(!generating_set)
	{
		static Cjulia4d2Doc previous_params;
		PerformRandomizations();
		Cjulia4d2Doc *params = (Cjulia4d2Doc *)GetActiveDocument();

//		if(	BST_CHECKED == m_wndRenderToSettingsDialog.GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_GETCHECK, 0, 0) &&
//			BST_CHECKED == m_wndExtraSettingsDialog.GetDlgItem(IDC_LIGHT_BY_NORMALS_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
//		{
//			params->light_by_normals = true;
//		}
//		else
//		{
//			params->light_by_normals = false;
//		}

/*
		static first_generation = true;

		if(first_generation)
		{
			first_generation = false;
		}
		else
		{
			if((params->render_to_type == POINT_SET && previous_params.render_to_type == POINT_SET ) &&
				!params->hopf_map &&
				*params == previous_params && previous_set_completed )
				//			if(*params == previous_params && previous_set_completed && (params->render_to_type == POINT_SET && !params->hopf_map))
				return;
		}
*/
		previous_params = *params;

//		MessageBox(params->formula_text.c_str(), " " ,MB_OK);

		if(params->render_to_type == CSV_FILE)
		{
			CFileDialog file_dlg(FALSE, "csv", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "CSV File (*.csv)|*.csv||");
			file_dlg.m_ofn.lpstrTitle = "Export to CSV File...";

			if(file_dlg.DoModal() == IDOK)
				params->export_file_name = file_dlg.GetPathName();
			else
				return;
		}
		else if(params->render_to_type == VOXEL_FILE)
		{
			CFileDialog file_dlg(FALSE, "j4d2vox", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Julia 4D 2 Voxel File (*.j4d2vox)|*.j4d2vox||");
			file_dlg.m_ofn.lpstrTitle = "Export to J4D2VOX File...";

			if(file_dlg.DoModal() == IDOK)
				params->export_file_name = file_dlg.GetPathName();
			else
				return;
		}
		else if(params->render_to_type == MESH_FILE)
		{
			CFileDialog file_dlg(FALSE, "obj", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Wavefront File (*.obj)|*.obj||");
			file_dlg.m_ofn.lpstrTitle = "Export to OBJ File...";

			if(file_dlg.DoModal() == IDOK)
				params->export_file_name = file_dlg.GetPathName();
			else
				return;
		}

		generation_start = clock();

		if(set_gen_pkg.Generate(*params))
		{
			POS_NORMAL_COLOUR_VERTEX *temp_vertices = 0;
			long unsigned int temp_vertex_count = 0;
			vector<long unsigned int> temp_z_slices;


			Renderer.SetPoints(temp_vertices, temp_vertex_count, temp_z_slices);
			Renderer.SetGeneratingFlag(true);
			Renderer.UseLitShader(false);
			if(!Renderer.Play())
			{
//				ostringstream error;
//				error << "Could not start Direct 3D renderer.\n" << Renderer.GetLastError();
//				MessageBox(error.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP);
//				this->SendMessage(WM_CLOSE);
//				return;
			}

			::Sleep(100);

			Renderer.Stop();

			generating_set = true;
			waiting_for_cancel = false;
			previous_set_completed = true;

			m_wndMainSettingsDialog.GetDlgItem(IDC_GENERATE)->SetWindowText("Cancel");
		}
		else
		{
			ostringstream error;
			error << "Could not begin set generation.\n" << set_gen_pkg.GetLastError();
			::MessageBox(m_hWnd, error.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP);  
			return;
		}
	}
	else
	{
		if(!set_gen_pkg.Cancel())
		{
			ostringstream error;
			error << "Could not cancel set generation.\n" << set_gen_pkg.GetLastError();
			::MessageBox(m_hWnd, error.str().c_str(), APP_NAME, MB_OK|MB_ICONSTOP);  
			return;
		}

		generating_set = false;
		waiting_for_cancel = true;
		previous_set_completed = false;

		m_wndMainSettingsDialog.GetDlgItem(IDC_GENERATE)->SetWindowText("Cancelling...");
	}
}



void CMainFrame::OnAppAbout()
{
	if(!about_box_showing)
	{
		about_box_showing = true;
		CAboutDlg aboutDlg;
		aboutDlg.DoModal();
		about_box_showing = false;
	}
}

void CMainFrame::OnBnClickedMulColumnDefaultVariablesRadio()
{
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK)->EnableWindow(FALSE);

	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT)->EnableWindow(FALSE);

	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnBnClickedMulColumnConstantValuesRadio()
{
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK)->EnableWindow(TRUE);

	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT)->EnableWindow(FALSE);

	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnBnClickedMulColumnCustomVariablesRadio()
{
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK)->EnableWindow(TRUE);

	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT)->EnableWindow(TRUE);

	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnBnClickedMulMatrixDefaultVariablesRadio()
{
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_MATRIX_CHECK)->EnableWindow(FALSE);

	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_11_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_12_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_13_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_14_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_21_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_22_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_23_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_24_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_31_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_32_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_33_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_34_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_41_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_42_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_43_EDIT)->EnableWindow(FALSE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_44_EDIT)->EnableWindow(FALSE);

	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnBnClickedMulMatrixCustomVariablesRadio()
{
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_MATRIX_CHECK)->EnableWindow(TRUE);

	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_11_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_12_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_13_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_14_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_21_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_22_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_23_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_24_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_31_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_32_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_33_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_34_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_41_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_42_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_43_EDIT)->EnableWindow(TRUE);
	m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_44_EDIT)->EnableWindow(TRUE);

	UncheckRandomizeCheckboxes();
}





void CMainFrame::OnBnClickedCullNonSurfacePointsCheck()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnBnClickedHopfMapCheck()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnBnClickedScreenRadio()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnBnClickedMeshRadio()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnBnClickedCsvFileRadio()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnBnClickedVoxelRadio()
{
	UncheckRandomizeCheckboxes();
}


void CMainFrame::OnUpdateViewShaderCombo(CCmdUI *pCmdUI)
{
	if(BST_CHECKED == m_wndRenderToSettingsDialog.GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_GETCHECK, 0, 0))
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CMainFrame::OnUpdateViewNormalRadiusEdit(CCmdUI *pCmdUI)
{
	if(BST_CHECKED == m_wndRenderToSettingsDialog.GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_GETCHECK, 0, 0))
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}




void CMainFrame::OnUpdateViewLBN(CCmdUI *pCmdUI)
{
	LRESULT CNSPChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_CULL_NON_SURFACE_POINTS_CHECK)->SendMessage(BM_GETCHECK, 0, 0);
	LRESULT HopfMapChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_GETCHECK, 0, 0);
	LRESULT RenderToScreenChecked = m_wndRenderToSettingsDialog.GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_UNCHECKED == RenderToScreenChecked)
	{
//		m_wndExtraSettingsDialog.GetDlgItem(IDC_LIGHT_BY_NORMALS_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		pCmdUI->Enable( FALSE );
		return;
	}

	pCmdUI->Enable( TRUE );
}



void CMainFrame::OnUpdateViewReverseSet(CCmdUI *pCmdUI)
{
	LRESULT CSVChecked = m_wndRenderToSettingsDialog.GetDlgItem(IDC_CSV_FILE_RADIO)->SendMessage(BM_GETCHECK, 0, 0);
	LRESULT HopfMapChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_CHECKED == HopfMapChecked && BST_CHECKED == CSVChecked)
	{
		m_wndExtraSettingsDialog.GetDlgItem(IDC_REVERSE_SET_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		pCmdUI->Enable( FALSE );
		return;
	}
	else
	{
		pCmdUI->Enable( TRUE );
	}
}



void CMainFrame::OnUpdateViewCNSP(CCmdUI *pCmdUI)
{
	LRESULT MeshChecked = m_wndRenderToSettingsDialog.GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_GETCHECK, 0, 0);
	LRESULT VoxelChecked = m_wndRenderToSettingsDialog.GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_GETCHECK, 0, 0);
	LRESULT HopfMapChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_GETCHECK, 0, 0);
	LRESULT CSVChecked = m_wndRenderToSettingsDialog.GetDlgItem(IDC_CSV_FILE_RADIO)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_CHECKED == HopfMapChecked && BST_CHECKED == CSVChecked)
	{
		m_wndExtraSettingsDialog.GetDlgItem(IDC_CULL_NON_SURFACE_POINTS_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		pCmdUI->Enable( FALSE );
		return;
	}

	if(BST_CHECKED == MeshChecked || BST_CHECKED == VoxelChecked)
	{
		m_wndExtraSettingsDialog.GetDlgItem(IDC_CULL_NON_SURFACE_POINTS_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
		pCmdUI->Enable( FALSE );
	}
	else
	{
		pCmdUI->Enable( TRUE );
	}
}


void CMainFrame::OnUpdateViewHopfMap(CCmdUI *pCmdUI)
{
//	LRESULT MeshChecked = m_wndRenderToSettingsDialog.GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_GETCHECK, 0, 0);
//	LRESULT VoxelChecked = m_wndRenderToSettingsDialog.GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_GETCHECK, 0, 0);
//	LRESULT CNSPChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_CULL_NON_SURFACE_POINTS_CHECK)->SendMessage(BM_GETCHECK, 0, 0);

//	if(/*BST_CHECKED == MeshChecked ||*/ BST_CHECKED == VoxelChecked /*|| BST_CHECKED == CNSPChecked*/)
//	{
//		pCmdUI->Enable( FALSE );
//	}
//	else
//	{
		pCmdUI->Enable( TRUE );
//	}
}



void CMainFrame::OnUpdateViewScreenRadio(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}


void CMainFrame::OnUpdateViewMeshRadio(CCmdUI *pCmdUI)
{
	LRESULT HopfMapChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_GETCHECK, 0, 0);
	LRESULT CNSPChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_CULL_NON_SURFACE_POINTS_CHECK)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_CHECKED == CNSPChecked)
	{
		pCmdUI->Enable( FALSE );

		if(BST_CHECKED == m_wndRenderToSettingsDialog.GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_GETCHECK, 0, 0))
		{
			m_wndRenderToSettingsDialog.GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
			m_wndRenderToSettingsDialog.GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
		}
	}
	else
	{
		pCmdUI->Enable( TRUE );
	}
}


void CMainFrame::OnUpdateViewCsvFileRadio(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}


void CMainFrame::OnUpdateViewVoxelRadio(CCmdUI *pCmdUI)
{
	LRESULT HopfMapChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_GETCHECK, 0, 0);
	LRESULT CNSPChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_CULL_NON_SURFACE_POINTS_CHECK)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_CHECKED == CNSPChecked)
	{
		pCmdUI->Enable( FALSE );

		if(BST_CHECKED == m_wndRenderToSettingsDialog.GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_GETCHECK, 0, 0))
		{
			m_wndRenderToSettingsDialog.GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
			m_wndRenderToSettingsDialog.GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
		}
	}
	else
	{
		pCmdUI->Enable( TRUE );
	}
}




void CMainFrame::OnUpdateViewZW(CCmdUI *pCmdUI)
{
	LRESULT HopfMapChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_CHECKED == HopfMapChecked)
	{
		pCmdUI->Enable( FALSE );
	}
	else
	{
		pCmdUI->Enable( TRUE );
	}

}

void CMainFrame::OnUpdateViewWMin(CCmdUI *pCmdUI)
{
	LRESULT HopfMapChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_CHECKED == HopfMapChecked)
	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}

void CMainFrame::OnUpdateViewWMax(CCmdUI *pCmdUI)
{
	LRESULT HopfMapChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_CHECKED == HopfMapChecked)
	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}

void CMainFrame::OnUpdateViewSamples(CCmdUI *pCmdUI)
{
	LRESULT MeshChecked = m_wndRenderToSettingsDialog.GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_GETCHECK, 0, 0);
	LRESULT HopfMapChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_CHECKED == MeshChecked && BST_UNCHECKED == HopfMapChecked)
	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}

void CMainFrame::OnUpdateViewIsovaluePercent(CCmdUI *pCmdUI)
{
	LRESULT MeshChecked = m_wndRenderToSettingsDialog.GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_CHECKED == MeshChecked)
	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}

void CMainFrame::OnUpdateViewAvg(CCmdUI *pCmdUI)
{
	LRESULT MeshChecked = m_wndRenderToSettingsDialog.GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_CHECKED == MeshChecked)
	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}

void CMainFrame::OnUpdateViewAvgPercent(CCmdUI *pCmdUI)
{
	LRESULT MeshChecked = m_wndRenderToSettingsDialog.GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_CHECKED == MeshChecked)
	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}













void CMainFrame::OnViewHideall()
{
	ShowControlBar( &m_wndMainSettingsDialog, FALSE, TRUE );
	ShowControlBar( &m_wndExtraSettingsDialog, FALSE, TRUE );
	ShowControlBar( &m_wndMultiplicationSettingsDialog, FALSE, TRUE );
	ShowControlBar( &m_wndRenderToSettingsDialog, FALSE, TRUE );
	ShowControlBar( &m_wndStatusBar, FALSE, TRUE );

	RecalcLayout();
}

void CMainFrame::OnUpdateViewHideall(CCmdUI *pCmdUI)
{
	if( m_wndMainSettingsDialog.IsVisible() ||
		m_wndExtraSettingsDialog.IsVisible() ||
		m_wndMultiplicationSettingsDialog.IsVisible() ||
		m_wndRenderToSettingsDialog.IsVisible() ||
		m_wndStatusBar.IsVisible())
	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}

void CMainFrame::OnViewShowall()
{
	ShowControlBar( &m_wndMainSettingsDialog, TRUE, TRUE );
	ShowControlBar( &m_wndExtraSettingsDialog, TRUE, TRUE );
	ShowControlBar( &m_wndMultiplicationSettingsDialog, TRUE, TRUE );
	ShowControlBar( &m_wndRenderToSettingsDialog, TRUE, TRUE );
	ShowControlBar( &m_wndStatusBar, TRUE, TRUE );

	RecalcLayout();
}

void CMainFrame::OnUpdateViewShowall(CCmdUI *pCmdUI)
{
	if( m_wndMainSettingsDialog.IsVisible() &&
		m_wndExtraSettingsDialog.IsVisible() &&
		m_wndMultiplicationSettingsDialog.IsVisible() &&
		m_wndRenderToSettingsDialog.IsVisible() &&
		m_wndStatusBar.IsVisible())
	{
		pCmdUI->Enable( FALSE );
	}
	else
	{
		pCmdUI->Enable( TRUE );
	}
}





void CMainFrame::UncheckRandomizeCheckboxes(void)
{
	CWnd *CheckBox;

	CheckBox = m_wndExtraSettingsDialog.GetDlgItem(IDC_RANDOMIZE_C_CHECK);

	if(0 != CheckBox)
		CheckBox->SendMessage(BM_SETCHECK, BST_UNCHECKED);

	CheckBox = m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK);

	if(0 != CheckBox)
		CheckBox->SendMessage(BM_SETCHECK, BST_UNCHECKED);

	CheckBox = m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_MATRIX_CHECK);

	if(0 != CheckBox)
		CheckBox->SendMessage(BM_SETCHECK, BST_UNCHECKED);
}



void CMainFrame::OnFormulaCustom()
{
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, FALSE, 0);
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnUpdateFormulaCustom(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnGeneralClassic()
{
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Classic: Z = Z^a + C");
	UncheckRandomizeCheckboxes();

}
void CMainFrame::OnUpdateGeneralClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnGeneralExponential()
{
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Exponential: Z = exp(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateGeneralExponential(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnGeneralQuadratic()
{
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Quadratic: Z = Z^a + Z + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateGeneralQuadratic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnGeneralLambda()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Lambda: Z = C*Z*(a - Z)");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateGeneralLambda(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnGeneralFunkyBlobs()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Funky Blobs: Z = C*Z*(C - Z)");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateGeneralFunkyBlobs(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnSineSine()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Sine: Z = C*sin(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateSineSine(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnSineSinePlus()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Sine Plus: Z = sin(Z)^a + C*sin(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateSineSinePlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnSineSineClassic()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Sine Classic: Z = sin(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateSineSineClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}





void CMainFrame::OnCosineCosine()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Cosine: Z = C*cos(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateCosineCosine(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnCosineCosinePlus()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Cosine Plus: Z = cos(Z)^a + C*cos(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateCosineCosinePlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnCosineCosineClassic()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Cosine Classic: Z = cos(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateCosineCosineClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}



void CMainFrame::OnTangentTangent()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Tangent: Z = C*tan(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateTangentTangent(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnTangentTangentPlus()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Tangent Plus: Z = tan(Z)^a + C*tan(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateTangentTangentPlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnTangentTangentClassic()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Tangent Classic: Z = tan(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateTangentTangentClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}



void CMainFrame::OnSecantSecant()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Secant: Z = C*sec(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateSecantSecant(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnSecantSecantPlus()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Secant Plus: Z = sec(Z)^a + C*sec(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateSecantSecantPlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnSecantSecantClassic()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Secant Classic: Z = sec(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateSecantSecantClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnCosecantCosecant()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Cosecant: Z = C*csc(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateCosecantCosecant(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnCosecantCosecantPlus()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Cosecant Plus: Z = csc(Z)^a + C*csc(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateCosecantCosecantPlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnCosecantCosecantClassic()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Cosecant Classic: Z = csc(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateCosecantCosecantClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnCotangentCotangent()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Cotangent: Z = C*cot(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateCotangentCotangent(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnCotangentCotangentPlus()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Cotangent Plus: Z = cot(Z)^a + C*cot(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateCotangentCotangentPlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnCotangentCotangentClassic()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Cotangent Classic: Z = cot(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateCotangentCotangentClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}















void CMainFrame::OnHyperbolicSineHyperbolicSine()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Sine: Z = C*sinh(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicSineHyperbolicSine(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicSineHyperbolicSinePlus()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Sine Plus: Z = sinh(Z)^a + C*sinh(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicSineHyperbolicSinePlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicSineHyperbolicSineClassic()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Sine Classic: Z = sinh(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicSineHyperbolicSineClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}





void CMainFrame::OnHyperbolicCosineHyperbolicCosine()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Cosine: Z = C*cosh(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicCosineHyperbolicCosine(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicCosineHyperbolicCosinePlus()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Cosine Plus: Z = cosh(Z)^a + C*cosh(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicCosineHyperbolicCosinePlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicCosineHyperbolicCosineClassic()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Cosine Classic: Z = cosh(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicCosineHyperbolicCosineClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}



void CMainFrame::OnHyperbolicTangentHyperbolicTangent()
{
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Tangent: Z = C*tanh(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicTangentHyperbolicTangent(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicTangentHyperbolicTangentPlus()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Tangent Plus: Z = tanh(Z)^a + C*tanh(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicTangentHyperbolicTangentPlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicTangentHyperbolicTangentClassic()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Tangent Classic: Z = tanh(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicTangentHyperbolicTangentClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}



void CMainFrame::OnHyperbolicSecantHyperbolicSecant()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Secant: Z = C*sech(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicSecantHyperbolicSecant(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicSecantHyperbolicSecantPlus()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Secant Plus: Z = sech(Z)^a + C*sech(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicSecantHyperbolicSecantPlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicSecantHyperbolicSecantClassic()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Secant Classic: Z = sech(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicSecantHyperbolicSecantClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnHyperbolicCosecantHyperbolicCosecant()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Cosecant: Z = C*csch(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicCosecantHyperbolicCosecant(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicCosecantHyperbolicCosecantPlus()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Cosecant Plus: Z = csch(Z)^a + C*csch(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicCosecantHyperbolicCosecantPlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicCosecantHyperbolicCosecantClassic()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Cosecant Classic: Z = csch(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicCosecantHyperbolicCosecantClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicCotangentHyperbolicCotangent()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Cotangent: Z = C*coth(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicCotangentHyperbolicCotangent(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicCotangentHyperbolicCotangentPlus()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Cotangent Plus: Z = coth(Z)^a + C*coth(Z)^a");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicCotangentHyperbolicCotangentPlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHyperbolicCotangentHyperbolicCotangentClassic()
{

	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);
	m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SetWindowText("Hyperbolic Cotangent Classic: Z = coth(Z^a) + C");
	UncheckRandomizeCheckboxes();
}
void CMainFrame::OnUpdateHyperbolicCotangentHyperbolicCotangentClassic(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}



void CMainFrame::OnBnClickedSwapZAndCCheck()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnBnClickedRandomizeCCheck()
{
	// nothing so far :) - don't want to uncheck itself upon check :)
}

void CMainFrame::OnBnClickedReverseSetCheck()
{
	UncheckRandomizeCheckboxes();
}


void CMainFrame::OnEnChangeSamplesEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeAvgPercentEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeIsovaluePercentEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeAvgPassesEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeFormulaEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeCXEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeCYEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeCZEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeCWEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeXMinEdit()
{
	UpdateBoundingGrid();
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeYMinEdit()
{
	UpdateBoundingGrid();
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeZMinEdit()
{
	UpdateBoundingGrid();
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeWMinEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeXMaxEdit()
{
	UpdateBoundingGrid();
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeYMaxEdit()
{
	UpdateBoundingGrid();
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeZMaxEdit()
{
	UpdateBoundingGrid();
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeWMaxEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeZWEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeAEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeIterationsEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeInfinityEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeResolutionEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulColumnConstantValuesXEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulColumnConstantValuesYEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulColumnConstantValuesZEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulColumnConstantValuesWEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulColumnCustomVariablesXEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulColumnCustomVariablesYEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulColumnCustomVariablesZEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulColumnCustomVariablesWEdit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom11Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom12Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom13Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom14Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom21Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom22Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom23Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom24Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom31Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom32Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom33Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom34Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom41Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom42Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom43Edit()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnEnChangeMulMatrixCustom44Edit()
{
	UncheckRandomizeCheckboxes();
}





void CMainFrame::PerformRandomizations()
{
	LRESULT randomize_c = m_wndExtraSettingsDialog.GetDlgItem(IDC_RANDOMIZE_C_CHECK)->SendMessage(BM_GETCHECK, 0, 0);
	LRESULT randomize_mul_column = m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK)->SendMessage(BM_GETCHECK, 0, 0);
	LRESULT randomize_mul_matrix = m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_MATRIX_CHECK)->SendMessage(BM_GETCHECK, 0, 0);

	if(randomize_c)
	{
		double C_x = static_cast<double>(rand()%10001) / 10000.0;
		double C_y = static_cast<double>(rand()%10001) / 10000.0;
		double C_z = static_cast<double>(rand()%10001) / 10000.0;
		double C_w = static_cast<double>(rand()%10001) / 10000.0;

		if(rand()%2)
			C_x = -C_x;

		if(rand()%2)
			C_y = -C_y;

		if(rand()%2)
			C_z = -C_z;

		if(rand()%2)
			C_w = -C_w;

		ostringstream out("");

		out << C_x;
		m_wndMainSettingsDialog.SetDlgItemText(IDC_C_X_EDIT, out.str().c_str());
		out.str("");

		out << C_y;
		m_wndMainSettingsDialog.SetDlgItemText(IDC_C_Y_EDIT, out.str().c_str());
		out.str("");

		out << C_z;
		m_wndMainSettingsDialog.SetDlgItemText(IDC_C_Z_EDIT, out.str().c_str());
		out.str("");

		out << C_w;
		m_wndMainSettingsDialog.SetDlgItemText(IDC_C_W_EDIT, out.str().c_str());
		out.str("");
	}


	if(randomize_mul_column)
	{
		if(BST_CHECKED == m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_RADIO)->SendMessage(BM_GETCHECK, 0, 0))
		{
			double val_1 = static_cast<double>(rand()%1001) / 1000.0;
			double val_2 = static_cast<double>(rand()%1001) / 1000.0;
			double val_3 = static_cast<double>(rand()%1001) / 1000.0;
			double val_4 = static_cast<double>(rand()%1001) / 1000.0;

			if(rand()%2)
				val_1 = -val_1;

			if(rand()%2)
				val_2 = -val_2;

			if(rand()%2)
				val_3 = -val_3;

			if(rand()%2)
				val_4 = -val_4;

			ostringstream out("");

			out << val_1;
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT, out.str().c_str());
			out.str("");

			out << val_2;
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT, out.str().c_str());
			out.str("");

			out << val_3;
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT, out.str().c_str());
			out.str("");

			out << val_4;
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT, out.str().c_str());
			out.str("");
		}
		if(BST_CHECKED == m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_RADIO)->SendMessage(BM_GETCHECK, 0, 0))
		{
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT, GetRandomCustomMulVariable().c_str());
		}
	}


	if(randomize_mul_matrix)
	{
		if(BST_CHECKED == m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_VARIABLES_RADIO)->SendMessage(BM_GETCHECK, 0, 0))
		{
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_11_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_12_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_13_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_14_EDIT, GetRandomCustomMulVariable().c_str());

			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_21_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_22_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_23_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_24_EDIT, GetRandomCustomMulVariable().c_str());

			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_31_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_32_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_33_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_34_EDIT, GetRandomCustomMulVariable().c_str());

			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_41_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_42_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_43_EDIT, GetRandomCustomMulVariable().c_str());
			m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_44_EDIT, GetRandomCustomMulVariable().c_str());
		}
	}

	if(randomize_c)
		m_wndExtraSettingsDialog.GetDlgItem(IDC_RANDOMIZE_C_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);

	if(randomize_mul_column)
		m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);

	if(randomize_mul_matrix)
		m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_MATRIX_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
}




void CMainFrame::UpdateBoundingGrid(void)
{
	CString buf;
	double x_min = 0.0;
	double x_max = 0.0;
	double y_min = 0.0;
	double y_max = 0.0;
	double z_min = 0.0;
	double z_max = 0.0;


	m_wndMainSettingsDialog.GetDlgItemText(IDC_X_MIN_EDIT, buf);

	if(buf.IsEmpty() || buf == "-")
	{
		x_min = 0.0;
	}
	else if(is_real_number(buf.GetBuffer()))
	{
		x_min = atof(buf.GetBuffer());
	}
	else
	{
		return;
	}

	m_wndMainSettingsDialog.GetDlgItemText(IDC_X_MAX_EDIT, buf);

	if(buf.IsEmpty() || buf == "-")
	{
		x_max = 0.0;
	}
	else if(is_real_number(buf.GetBuffer()))
	{
		x_max = atof(buf.GetBuffer());
	}
	else
	{
		return;
	}


	m_wndMainSettingsDialog.GetDlgItemText(IDC_Y_MIN_EDIT, buf);

	if(buf.IsEmpty() || buf == "-")
	{
		y_min = 0.0;
	}
	else if(is_real_number(buf.GetBuffer()))
	{
		y_min = atof(buf.GetBuffer());
	}
	else
	{
		return;
	}

	m_wndMainSettingsDialog.GetDlgItemText(IDC_Y_MAX_EDIT, buf);

	if(buf.IsEmpty() || buf == "-")
	{
		y_max = 0.0;
	}
	else if(is_real_number(buf.GetBuffer()))
	{
		y_max = atof(buf.GetBuffer());
	}
	else
	{
		return;
	}


	m_wndMainSettingsDialog.GetDlgItemText(IDC_Z_MIN_EDIT, buf);

	if(buf.IsEmpty() || buf == "-")
	{
		z_min = 0.0;
	}
	else if(is_real_number(buf.GetBuffer()))
	{
		z_min = atof(buf.GetBuffer());
	}
	else
	{
		return;
	}

	m_wndMainSettingsDialog.GetDlgItemText(IDC_Z_MAX_EDIT, buf);

	if(buf.IsEmpty() || buf == "-")
	{
		z_max = 0.0;
	}
	else if(is_real_number(buf.GetBuffer()))
	{
		z_max = atof(buf.GetBuffer());
	}
	else
	{
		return;
	}


	Renderer.SetBoundingGrid(
		static_cast<float>(x_min),
		static_cast<float>(x_max),
		static_cast<float>(y_min),
		static_cast<float>(y_max),
		static_cast<float>(z_min),
		static_cast<float>(z_max) );
}



void CMainFrame::SetVars(Cjulia4d2Doc &doc)
{
	ostringstream out;
	size_t last_not_zero_pos = 0;
	out << setiosflags(std::ios_base::fixed);
	out << setprecision(14);


	if(creation_complete)
	{
		// extra - done
		if(doc.swap_z_and_c)
			m_wndExtraSettingsDialog.GetDlgItem(IDC_SWAP_Z_AND_C_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED);
		else
			m_wndExtraSettingsDialog.GetDlgItem(IDC_SWAP_Z_AND_C_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

		if(doc.reverse_set)
			m_wndExtraSettingsDialog.GetDlgItem(IDC_REVERSE_SET_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED);
		else
			m_wndExtraSettingsDialog.GetDlgItem(IDC_REVERSE_SET_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

		if(doc.hopf_map)
			m_wndExtraSettingsDialog.GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED);
		else
			m_wndExtraSettingsDialog.GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

		if(doc.cull_non_surface_points)
			m_wndExtraSettingsDialog.GetDlgItem(IDC_CULL_NON_SURFACE_POINTS_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED);
		else
			m_wndExtraSettingsDialog.GetDlgItem(IDC_CULL_NON_SURFACE_POINTS_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

		if(doc.proportional_res)
			m_wndMainSettingsDialog.GetDlgItem(IDC_PROP_RES_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED);
		else
			m_wndMainSettingsDialog.GetDlgItem(IDC_PROP_RES_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);


		out.str("");
		out << doc.normal_gen_radius;
		m_wndExtraSettingsDialog.SetDlgItemText(IDC_NORMAL_QUALITY_EDIT, out.str().c_str());


		m_wndExtraSettingsDialog.GetDlgItem(IDC_RANDOMIZE_C_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);




		//mesh - done
		out.str("");
		out << doc.samples;
		m_wndMainSettingsDialog.SetDlgItemText(IDC_SAMPLES_EDIT, out.str().c_str());
		out.str("");





		//mul - done
		switch(doc.mul_column_type)
		{
		case MUL_COLUMN_DEFAULT:
			{
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_DEFAULT_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK)->EnableWindow(FALSE);

				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT)->EnableWindow(FALSE);

				break;
			}
		case MUL_COLUMN_CONST_VALS:
			{
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_DEFAULT_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK)->EnableWindow(TRUE);

				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT)->EnableWindow(TRUE);

				break;
			}
		case MUL_COLUMN_CUSTOM:
			{
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_DEFAULT_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_COLUMN_CHECK)->EnableWindow(TRUE);

				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT)->EnableWindow(FALSE);

				break;
			}
		}

		out.str("");

		out << doc.mul_column_const_vals_x;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT, out.str().c_str());
		out.str("");

		out << doc.mul_column_const_vals_y;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT, out.str().c_str());
		out.str("");

		out << doc.mul_column_const_vals_z;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT, out.str().c_str());
		out.str("");

		out << doc.mul_column_const_vals_w;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT, out.str().c_str());
		out.str("");

		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT, get_mul_variable_string(doc.mul_column_custom_x).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT, get_mul_variable_string(doc.mul_column_custom_y).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT, get_mul_variable_string(doc.mul_column_custom_z).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT, get_mul_variable_string(doc.mul_column_custom_w).c_str());


		switch(doc.mul_matrix_type)
		{
		case MUL_MATRIX_DEFAULT:
			{
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_DEFAULT_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_MATRIX_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_MATRIX_CHECK)->EnableWindow(FALSE);

				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_11_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_12_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_13_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_14_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_21_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_22_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_23_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_24_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_31_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_32_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_33_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_34_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_41_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_42_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_43_EDIT)->EnableWindow(FALSE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_44_EDIT)->EnableWindow(FALSE);
				break;
			}
		case MUL_MATRIX_CUSTOM:
			{
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_DEFAULT_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_MATRIX_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_RANDOMIZE_MUL_MATRIX_CHECK)->EnableWindow(TRUE);

				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_11_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_12_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_13_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_14_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_21_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_22_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_23_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_24_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_31_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_32_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_33_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_34_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_41_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_42_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_43_EDIT)->EnableWindow(TRUE);
				m_wndMultiplicationSettingsDialog.GetDlgItem(IDC_MUL_MATRIX_CUSTOM_44_EDIT)->EnableWindow(TRUE);
				break;
			}
		}

		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_11_EDIT, get_mul_variable_string(doc.mul_matrix_custom_11).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_12_EDIT, get_mul_variable_string(doc.mul_matrix_custom_12).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_13_EDIT, get_mul_variable_string(doc.mul_matrix_custom_13).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_14_EDIT, get_mul_variable_string(doc.mul_matrix_custom_14).c_str());

		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_21_EDIT, get_mul_variable_string(doc.mul_matrix_custom_21).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_22_EDIT, get_mul_variable_string(doc.mul_matrix_custom_22).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_23_EDIT, get_mul_variable_string(doc.mul_matrix_custom_23).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_24_EDIT, get_mul_variable_string(doc.mul_matrix_custom_24).c_str());

		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_31_EDIT, get_mul_variable_string(doc.mul_matrix_custom_31).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_32_EDIT, get_mul_variable_string(doc.mul_matrix_custom_32).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_33_EDIT, get_mul_variable_string(doc.mul_matrix_custom_33).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_34_EDIT, get_mul_variable_string(doc.mul_matrix_custom_34).c_str());

		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_41_EDIT, get_mul_variable_string(doc.mul_matrix_custom_41).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_42_EDIT, get_mul_variable_string(doc.mul_matrix_custom_42).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_43_EDIT, get_mul_variable_string(doc.mul_matrix_custom_43).c_str());
		m_wndMultiplicationSettingsDialog.SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_44_EDIT, get_mul_variable_string(doc.mul_matrix_custom_44).c_str());







		//renderto - done
		switch(doc.render_to_type)
		{
		case POINT_SET:
			{
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_CSV_FILE_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

				break;
			}
		case CSV_FILE:
			{
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_CSV_FILE_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

				break;
			}
		case VOXEL_FILE:
			{
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_CSV_FILE_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

				break;
			}
		case MESH_FILE:
			{
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_CSV_FILE_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
				m_wndRenderToSettingsDialog.GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);

				break;
			}
		}





		//main
		if(doc.formula == CUSTOM_ITERATOR)
			m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, FALSE, 0);
		else
			m_wndMainSettingsDialog.GetDlgItem(IDC_FORMULA_EDIT)->SendMessage(EM_SETREADONLY, TRUE, 0);

		m_wndMainSettingsDialog.SetDlgItemText(IDC_FORMULA_EDIT, doc.formula_text.c_str());




		out.str("");

		out << doc.iterations;
		m_wndMainSettingsDialog.SetDlgItemText(IDC_ITERATIONS_EDIT, out.str().c_str());
		out.str("");


		out << doc.infinity;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_INFINITY_EDIT, out.str().c_str());
		out.str("");


		out << doc.resolution;
		m_wndMainSettingsDialog.SetDlgItemText(IDC_RESOLUTION_EDIT, out.str().c_str());
		out.str("");

		out << doc.hopf_field_resolution;
		m_wndMainSettingsDialog.SetDlgItemText(IDC_HOPF_FIELD_RESOLUTION_EDIT, out.str().c_str());
		out.str("");

		out << doc.c_x;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_C_X_EDIT, out.str().c_str());
		out.str("");

		out << doc.c_y;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_C_Y_EDIT, out.str().c_str());
		out.str("");

		out << doc.c_z;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_C_Z_EDIT, out.str().c_str());
		out.str("");

		out << doc.c_w;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_C_W_EDIT, out.str().c_str());
		out.str("");

		out << doc.z_w;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_Z_W_EDIT, out.str().c_str());
		out.str("");

		out << doc.a;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_A_EDIT, out.str().c_str());
		out.str("");



		out << doc.x_min;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_X_MIN_EDIT, out.str().c_str());
		out.str("");

		out << doc.x_max;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_X_MAX_EDIT, out.str().c_str());
		out.str("");

		out << doc.y_min;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_Y_MIN_EDIT, out.str().c_str());
		out.str("");

		out << doc.y_max;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_Y_MAX_EDIT, out.str().c_str());
		out.str("");

		out << doc.z_min;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_Z_MIN_EDIT, out.str().c_str());
		out.str("");

		out << doc.z_max;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_Z_MAX_EDIT, out.str().c_str());
		out.str("");

		out << doc.w_min;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_W_MIN_EDIT, out.str().c_str());
		out.str("");

		out << doc.w_max;
		if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

		m_wndMainSettingsDialog.SetDlgItemText(IDC_W_MAX_EDIT, out.str().c_str());
		out.str("");


	}
}


arc_ball CMainFrame::GetArcBall(void)
{
	return Renderer.GetArcBall();
}

void CMainFrame::SetArcBall(const arc_ball &src_arcball)
{
	Renderer.SetArcBall(src_arcball);
}



void CMainFrame::OnViewRestoreall()
{
	LoadBarState("Julia 4D 2 Default Settings");
	RecalcLayout();
}

void CMainFrame::OnUpdateViewRestoreall(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}


void CMainFrame::OnUpdateHelpContents(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateRendererZoomPlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}


void CMainFrame::OnUpdateRendererZoomMinus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateRendererPointsizePlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateRendererPointsizeMinus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateRendererFog(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( Renderer.IsFogEnabled() );
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateRendererBoundinggrid(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( Renderer.IsBoundingGridEnabled() );
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateRendererZslicePlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateRendererZsliceMinus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateRendererSinglezslice(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
	pCmdUI->SetCheck( Renderer.IsSingleZSliceModeEnabled() );
}

void CMainFrame::OnUpdateRendererResetzslicing(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateRendererResetztr(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}


void CMainFrame::OnHelpContents()
{
	OnHelp();
}

void CMainFrame::OnRendererZoomPlus()
{
	Renderer.IncreaseZoom();
}


void CMainFrame::OnRendererZoomMinus()
{
	Renderer.DecreaseZoom();
}

void CMainFrame::OnRendererPointsizePlus()
{
	Renderer.IncreasePointSize();
}

void CMainFrame::OnRendererPointsizeMinus()
{
	Renderer.DecreasePointSize();
}

void CMainFrame::OnRendererFog()
{
	Renderer.ToggleFog();
}

void CMainFrame::OnRendererBoundinggrid()
{
	Renderer.ToggleBoundingGrid();
}

void CMainFrame::OnRendererZslicePlus()
{
	Renderer.ZSlicePlus();
}

void CMainFrame::OnRendererZsliceMinus()
{
	Renderer.ZSliceMinus();
}

void CMainFrame::OnRendererSinglezslice()
{
	Renderer.ToggleSingleZSlice();
}

void CMainFrame::OnRendererResetzslicing()
{
	Renderer.ResetZSlicing();
}

void CMainFrame::OnRendererResetztr()
{
	Renderer.ResetTranslationAndRotation();
}


void CMainFrame::OnRendererInterfacetext()
{
	Renderer.ToggleInterfaceText();
}

void CMainFrame::OnUpdateRendererInterfacetext(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( Renderer.IsInterfaceTextEnabled() );
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateRendererSetbackgroundcolor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnRendererSetbackgroundcolor()
{
	COLORREF curr_bg_colour = RGB(Renderer.GetBackgroundColourR(), Renderer.GetBackgroundColourG(), Renderer.GetBackgroundColourB());

	CColorDialog dlg(curr_bg_colour, 0, this);

	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;  // opens custom color selector
	dlg.m_cc.lpCustColors = acrCustClr;

	dlg.DoModal();  // open color dialog

	COLORREF new_bg_colour = dlg.GetColor();  // set current color to the one chosen

	Renderer.SetBackgroundColour(GetRValue(new_bg_colour), GetGValue(new_bg_colour), GetBValue(new_bg_colour));

	AfxGetApp()->WriteProfileInt("Settings", "BG_R", GetRValue(new_bg_colour));
	AfxGetApp()->WriteProfileInt("Settings", "BG_G", GetGValue(new_bg_colour));
	AfxGetApp()->WriteProfileInt("Settings", "BG_B", GetBValue(new_bg_colour));
}




void CMainFrame::OnUpdateRendererSetfogcolor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnRendererSetfogcolor()
{
	COLORREF curr_fog_colour = RGB(Renderer.GetFogColourR(), Renderer.GetFogColourG(), Renderer.GetFogColourB());

	CColorDialog dlg(curr_fog_colour, 0, this);

	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;  // opens custom color selector
	dlg.m_cc.lpCustColors = acrCustClr;

	dlg.DoModal();  // open color dialog

	COLORREF new_fog_colour = dlg.GetColor();  // set current color to the one chosen

	Renderer.SetFogColour(GetRValue(new_fog_colour), GetGValue(new_fog_colour), GetBValue(new_fog_colour));

	AfxGetApp()->WriteProfileInt("Settings", "F_R", GetRValue(new_fog_colour));
	AfxGetApp()->WriteProfileInt("Settings", "F_G", GetGValue(new_fog_colour));
	AfxGetApp()->WriteProfileInt("Settings", "F_B", GetBValue(new_fog_colour));
}

void CMainFrame::OnUpdateRendererSetinterfacetextcolour(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnRendererSetinterfacetextcolour()
{
	COLORREF curr_text_colour = RGB(Renderer.GetInterfaceTextColourR(), Renderer.GetInterfaceTextColourG(), Renderer.GetInterfaceTextColourB());

	CColorDialog dlg(curr_text_colour, 0, this);

	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;  // opens custom color selector
	dlg.m_cc.lpCustColors = acrCustClr;

	dlg.DoModal();  // open color dialog

	COLORREF new_text_colour = dlg.GetColor();  // set current color to the one chosen

	Renderer.SetInterfaceTextColour(Renderer.GetInterfaceTextColourA(), GetRValue(new_text_colour), GetGValue(new_text_colour), GetBValue(new_text_colour));

	AfxGetApp()->WriteProfileInt("Settings", "IT_R", GetRValue(new_text_colour));
	AfxGetApp()->WriteProfileInt("Settings", "IT_G", GetGValue(new_text_colour));
	AfxGetApp()->WriteProfileInt("Settings", "IT_B", GetBValue(new_text_colour));
}

void CMainFrame::OnUpdateRendererResetboundinggrid(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnRendererResetboundinggrid()
{
	m_wndMainSettingsDialog.SetDlgItemText(IDC_X_MIN_EDIT, "-1.5");
	m_wndMainSettingsDialog.SetDlgItemText(IDC_X_MAX_EDIT, "1.5");
	m_wndMainSettingsDialog.SetDlgItemText(IDC_Y_MIN_EDIT, "-1.5");
	m_wndMainSettingsDialog.SetDlgItemText(IDC_Y_MAX_EDIT, "1.5");
	m_wndMainSettingsDialog.SetDlgItemText(IDC_Z_MIN_EDIT, "-1.5");
	m_wndMainSettingsDialog.SetDlgItemText(IDC_Z_MAX_EDIT, "1.5");
	m_wndMainSettingsDialog.SetDlgItemText(IDC_W_MIN_EDIT, "-1.5");
	m_wndMainSettingsDialog.SetDlgItemText(IDC_W_MAX_EDIT, "1.5");

}

void CMainFrame::OnUpdateRendererRestoredefaultcolours(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnRendererRestoredefaultcolours()
{
	Renderer.RestoreDefaultColours();

	AfxGetApp()->WriteProfileInt("Settings", "BG_R", Renderer.GetBackgroundColourR());
	AfxGetApp()->WriteProfileInt("Settings", "BG_G", Renderer.GetBackgroundColourG());
	AfxGetApp()->WriteProfileInt("Settings", "BG_B", Renderer.GetBackgroundColourB());

	AfxGetApp()->WriteProfileInt("Settings", "F_R", Renderer.GetFogColourR());
	AfxGetApp()->WriteProfileInt("Settings", "F_G", Renderer.GetFogColourG());
	AfxGetApp()->WriteProfileInt("Settings", "F_B", Renderer.GetFogColourB());

	AfxGetApp()->WriteProfileInt("Settings", "IT_R", Renderer.GetInterfaceTextColourR());
	AfxGetApp()->WriteProfileInt("Settings", "IT_G", Renderer.GetInterfaceTextColourG());
	AfxGetApp()->WriteProfileInt("Settings", "IT_B", Renderer.GetInterfaceTextColourB());
}


void CMainFrame::OnUpdateInterfaceelementsAxisindicators(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
	pCmdUI->SetCheck( Renderer.IsAxisIndicatorsEnabled() );
}

void CMainFrame::OnInterfaceelementsAxisindicators()
{
	Renderer.ToggleAxisIndicators();
}

void CMainFrame::OnUpdateFileGenerate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnFileGenerate()
{
	OnBnClickedGenerate();
}






void CMainFrame::OnUpdateEditCachesettings(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnEditCachesettings()
{
	CCacheSettingsDlg csDlg(this);
	csDlg.DoModal();
}

void CMainFrame::OnBnClickedMulGenButton()
{
	OnBnClickedGenerate();
}

void CMainFrame::OnVoxelOpacityPlus()
{
	Renderer.IncreaseOpacity();
}

void CMainFrame::OnUpdateVoxelOpacityPlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnVoxelOpacityMinus()
{
	Renderer.DecreaseOpacity();
}

void CMainFrame::OnUpdateVoxelOpacityMinus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHelpLicenseterms()
{
	CLicenseDlg licenseDlg;
	licenseDlg.DoModal();
}

void CMainFrame::OnUpdateHelpLicenseterms(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnBnClickedRenderToGenButton()
{
	OnBnClickedGenerate();
}

void CMainFrame::OnBnClickedExtraGenButton()
{
	OnBnClickedGenerate();
}






void CMainFrame::OnCbnSelchangeShaderCombo()
{
	LRESULT shader_number = m_wndExtraSettingsDialog.GetDlgItem(IDC_SHADER_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(0 == shader_number || CB_ERR == shader_number)
		Renderer.RainbowColouring(m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd);  //ColourVertices(m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd, set_gen_pkg.vertices, set_gen_pkg.vertex_count, random_colour);
	else if(1 == shader_number)
		Renderer.DotProdColouring(m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd);  //ColourVertices(m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd, set_gen_pkg.vertices, set_gen_pkg.vertex_count, random_colour);
	else if(2 == shader_number)
		Renderer.CowColouring(m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd);
	else
		Renderer.FlatColouring(static_cast<DWORD>(shader_number), m_wndMainSettingsDialog.GetDlgItem(IDC_PROGRESS_BAR)->m_hWnd, m_wndMainSettingsDialog.GetDlgItem(IDC_STATUS_EDIT)->m_hWnd);
}

void CMainFrame::OnAmbienceLevelPlus()
{
	Renderer.AmbiencePlus();
}

void CMainFrame::OnUpdateAmbienceLevelPlus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnAmbienceLevelMins()
{
	Renderer.AmbienceMinus();
}

void CMainFrame::OnUpdateAmbienceLevelMins(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnEnChangeHopfFieldResolutionEdit()
{
	UncheckRandomizeCheckboxes();
}


void CMainFrame::OnUpdateViewHopfFieldResolutionEdit(CCmdUI *pCmdUI)
{
	LRESULT CSVChecked = m_wndRenderToSettingsDialog.GetDlgItem(IDC_CSV_FILE_RADIO)->SendMessage(BM_GETCHECK, 0, 0);
	LRESULT HopfMapChecked = m_wndExtraSettingsDialog.GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_GETCHECK, 0, 0);

	if(BST_CHECKED == HopfMapChecked && BST_UNCHECKED == CSVChecked)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}







void CMainFrame::OnBnClickedLightByNormalsCheck()
{
	if(BST_CHECKED == m_wndExtraSettingsDialog.GetDlgItem(IDC_LIGHT_BY_NORMALS_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		Renderer.UseLitShader(true);
	else
		Renderer.UseLitShader(false);
}


void CMainFrame::OnUpdateViewSwapZAndCEdit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}



void CMainFrame::OnBnClickedPropResCheck()
{
	UncheckRandomizeCheckboxes();
}

void CMainFrame::OnUpdateViewPropResCheck(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


