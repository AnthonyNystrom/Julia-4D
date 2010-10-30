// julia4d2Doc.cpp : implementation of the Cjulia4d2Doc class
//

#include "stdafx.h"
#include "julia4d2.h"

#include "julia4d2Doc.h"
#include ".\julia4d2doc.h"


#include "MainFrm.h"
#include "utilities.h"
#include "arc_ball.h"

#include <iomanip>
#include <sstream>
using std::setiosflags;
using std::setprecision;
using std::ostringstream;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cjulia4d2Doc

IMPLEMENT_DYNCREATE(Cjulia4d2Doc, CDocument)

BEGIN_MESSAGE_MAP(Cjulia4d2Doc, CDocument)
	ON_EN_UPDATE(IDC_C_X_EDIT, OnEnUpdateCXEdit)
	ON_EN_UPDATE(IDC_C_Y_EDIT, OnEnUpdateCYEdit)
	ON_EN_UPDATE(IDC_C_Z_EDIT, OnEnUpdateCZEdit)
	ON_EN_UPDATE(IDC_C_W_EDIT, OnEnUpdateCWEdit)

	ON_EN_UPDATE(IDC_X_MIN_EDIT, OnEnUpdateXMinEdit)
	ON_EN_UPDATE(IDC_X_MAX_EDIT, OnEnUpdateXMaxEdit)
	ON_EN_UPDATE(IDC_Y_MIN_EDIT, OnEnUpdateYMinEdit)
	ON_EN_UPDATE(IDC_Y_MAX_EDIT, OnEnUpdateYMaxEdit)
	ON_EN_UPDATE(IDC_Z_MIN_EDIT, OnEnUpdateZMinEdit)
	ON_EN_UPDATE(IDC_Z_MAX_EDIT, OnEnUpdateZMaxEdit)
	ON_EN_UPDATE(IDC_W_MIN_EDIT, OnEnUpdateWMinEdit)
	ON_EN_UPDATE(IDC_W_MAX_EDIT, OnEnUpdateWMaxEdit)

	ON_EN_UPDATE(IDC_Z_W_EDIT, OnEnUpdateZWEdit)
	ON_EN_UPDATE(IDC_A_EDIT, OnEnUpdateAEdit)
	ON_EN_UPDATE(IDC_INFINITY_EDIT, OnEnUpdateInfinityEdit)

	ON_EN_UPDATE(IDC_FORMULA_EDIT, OnEnUpdateFormulaEdit)
	ON_EN_UPDATE(IDC_ITERATIONS_EDIT, OnEnUpdateIterationsEdit)
	ON_EN_UPDATE(IDC_SAMPLES_EDIT, OnEnUpdateSamplesEdit)


	ON_COMMAND(ID_FORMULA_CUSTOM, OnFormulaCustom)

	ON_COMMAND(ID_GENERAL_CLASSIC, OnGeneralClassic)
	ON_COMMAND(ID_GENERAL_EXPONENTIAL, OnGeneralExponential)
	ON_COMMAND(ID_GENERAL_QUADRATIC, OnGeneralQuadratic)
	ON_COMMAND(ID_GENERAL_LAMBDA, OnGeneralLambda)
	ON_COMMAND(ID_GENERAL_FUNKYBLOBS, OnGeneralFunkyBlobs)

	ON_COMMAND(ID_SINE_SINE, OnSineSine)
	ON_COMMAND(ID_SINE_SINEPLUS, OnSineSinePlus)
	ON_COMMAND(ID_SINE_SINECLASSIC, OnSineSineClassic)

	ON_COMMAND(ID_COSINE_COSINE, OnCosineCosine)
	ON_COMMAND(ID_COSINE_COSINEPLUS, OnCosineCosinePlus)
	ON_COMMAND(ID_COSINE_COSINECLASSIC, OnCosineCosineClassic)

	ON_COMMAND(ID_TANGENT_TANGENT, OnTangentTangent)
	ON_COMMAND(ID_TANGENT_TANGENTPLUS, OnTangentTangentPlus)
	ON_COMMAND(ID_TANGENT_TANGENTCLASSIC, OnTangentTangentClassic)

	ON_COMMAND(ID_SECANT_SECANT, OnSecantSecant)
	ON_COMMAND(ID_SECANT_SECANTPLUS, OnSecantSecantPlus)
	ON_COMMAND(ID_SECANT_SECANTCLASSIC, OnSecantSecantClassic)

	ON_COMMAND(ID_COSECANT_COSECANT, OnCosecantCosecant)
	ON_COMMAND(ID_COSECANT_COSECANTPLUS, OnCosecantCosecantPlus)
	ON_COMMAND(ID_COSECANT_COSECANTCLASSIC, OnCosecantCosecantClassic)

	ON_COMMAND(ID_COTANGENT_COTANGENT, OnCotangentCotangent)
	ON_COMMAND(ID_COTANGENT_COTANGENTPLUS, OnCotangentCotangentPlus)
	ON_COMMAND(ID_COTANGENT_COTANGENTCLASSIC, OnCotangentCotangentClassic)

	ON_COMMAND(ID_HYPERBOLICSINE_HYPERBOLICSINE, OnHyperbolicSineHyperbolicSine)
	ON_COMMAND(ID_HYPERBOLICSINE_HYPERBOLICSINEPLUS, OnHyperbolicSineHyperbolicSinePlus)
	ON_COMMAND(ID_HYPERBOLICSINE_HYPERBOLICSINECLASSIC, OnHyperbolicSineHyperbolicSineClassic)

	ON_COMMAND(ID_HYPERBOLICCOSINE_HYPERBOLICCOSINE, OnHyperbolicCosineHyperbolicCosine)
	ON_COMMAND(ID_HYPERBOLICCOSINE_HYPERBOLICCOSINEPLUS, OnHyperbolicCosineHyperbolicCosinePlus)
	ON_COMMAND(ID_HYPERBOLICCOSINE_HYPERBOLICCOSINECLASSIC, OnHyperbolicCosineHyperbolicCosineClassic)

	ON_COMMAND(ID_HYPERBOLICTANGENT_HYPERBOLICTANGENT, OnHyperbolicTangentHyperbolicTangent)
	ON_COMMAND(ID_HYPERBOLICTANGENT_HYPERBOLICTANGENTPLUS, OnHyperbolicTangentHyperbolicTangentPlus)
	ON_COMMAND(ID_HYPERBOLICTANGENT_HYPERBOLICTANGENTCLASSIC, OnHyperbolicTangentHyperbolicTangentClassic)

	ON_COMMAND(ID_HYPERBOLICSECANT_HYPERBOLICSECANT, OnHyperbolicSecantHyperbolicSecant)
	ON_COMMAND(ID_HYPERBOLICSECANT_HYPERBOLICSECANTPLUS, OnHyperbolicSecantHyperbolicSecantPlus)
	ON_COMMAND(ID_HYPERBOLICSECANT_HYPERBOLICSECANTCLASSIC, OnHyperbolicSecantHyperbolicSecantClassic)

	ON_COMMAND(ID_HYPERBOLICCOSECANT_HYPERBOLICCOSECANT, OnHyperbolicCosecantHyperbolicCosecant)
	ON_COMMAND(ID_HYPERBOLICCOSECANT_HYPERBOLICCOSECANTPLUS, OnHyperbolicCosecantHyperbolicCosecantPlus)
	ON_COMMAND(ID_HYPERBOLICCOSECANT_HYPERBOLICCOSECANTCLASSIC, OnHyperbolicCosecantHyperbolicCosecantClassic)

	ON_COMMAND(ID_HYPERBOLICCOTANGENT_HYPERBOLICCOTANGENT, OnHyperbolicCotangentHyperbolicCotangent)
	ON_COMMAND(ID_HYPERBOLICCOTANGENT_HYPERBOLICCOTANGENTPLUS, OnHyperbolicCotangentHyperbolicCotangentPlus)
	ON_COMMAND(ID_HYPERBOLICCOTANGENT_HYPERBOLICCOTANGENTCLASSIC, OnHyperbolicCotangentHyperbolicCotangentClassic)


	ON_BN_CLICKED(IDC_SWAP_Z_AND_C_CHECK, OnBnClickedSwapZAndCCheck)
	ON_BN_CLICKED(IDC_REVERSE_SET_CHECK, OnBnClickedReverseSetCheck)
	ON_BN_CLICKED(IDC_HOPF_MAP_CHECK, OnBnClickedHopfMapCheck)
	ON_BN_CLICKED(IDC_CULL_NON_SURFACE_POINTS_CHECK, OnBnClickedCullNonSurfacePointsCheck)

	ON_BN_CLICKED(IDC_SCREEN_RADIO, OnBnClickedScreenRadio)
	ON_BN_CLICKED(IDC_CSV_FILE_RADIO, OnBnClickedCsvFileRadio)
	ON_BN_CLICKED(IDC_VOXEL_RADIO, OnBnClickedVoxelRadio)
	ON_BN_CLICKED(IDC_MESH_RADIO, OnBnClickedMeshRadio)
	ON_BN_CLICKED(IDC_MUL_COLUMN_DEFAULT_VARIABLES_RADIO, OnBnClickedMulColumnDefaultVariablesRadio)
	ON_BN_CLICKED(IDC_MUL_COLUMN_CONSTANT_VALUES_RADIO, OnBnClickedMulColumnConstantValuesRadio)
	ON_BN_CLICKED(IDC_MUL_COLUMN_CUSTOM_VARIABLES_RADIO, OnBnClickedMulColumnCustomVariablesRadio)
	ON_BN_CLICKED(IDC_MUL_MATRIX_DEFAULT_VARIABLES_RADIO, OnBnClickedMulMatrixDefaultVariablesRadio)
	ON_BN_CLICKED(IDC_MUL_MATRIX_CUSTOM_VARIABLES_RADIO, OnBnClickedMulMatrixCustomVariablesRadio)

	ON_EN_UPDATE(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT, OnEnUpdateMulColumnConstantValuesXEdit)
	ON_EN_UPDATE(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT, OnEnUpdateMulColumnConstantValuesYEdit)
	ON_EN_UPDATE(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT, OnEnUpdateMulColumnConstantValuesZEdit)
	ON_EN_UPDATE(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT, OnEnUpdateMulColumnConstantValuesWEdit)

	ON_EN_UPDATE(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT, OnEnUpdateMulColumnCustomVariablesXEdit)
	ON_EN_UPDATE(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT, OnEnUpdateMulColumnCustomVariablesYEdit)
	ON_EN_UPDATE(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT, OnEnUpdateMulColumnCustomVariablesZEdit)
	ON_EN_UPDATE(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT, OnEnUpdateMulColumnCustomVariablesWEdit)

	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_11_EDIT, OnEnUpdateMulMatrixCustom11Edit)
	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_12_EDIT, OnEnUpdateMulMatrixCustom12Edit)
	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_13_EDIT, OnEnUpdateMulMatrixCustom13Edit)
	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_14_EDIT, OnEnUpdateMulMatrixCustom14Edit)

	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_21_EDIT, OnEnUpdateMulMatrixCustom21Edit)
	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_22_EDIT, OnEnUpdateMulMatrixCustom22Edit)
	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_23_EDIT, OnEnUpdateMulMatrixCustom23Edit)
	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_24_EDIT, OnEnUpdateMulMatrixCustom24Edit)

	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_31_EDIT, OnEnUpdateMulMatrixCustom31Edit)
	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_32_EDIT, OnEnUpdateMulMatrixCustom32Edit)
	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_33_EDIT, OnEnUpdateMulMatrixCustom33Edit)
	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_34_EDIT, OnEnUpdateMulMatrixCustom34Edit)

	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_41_EDIT, OnEnUpdateMulMatrixCustom41Edit)
	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_42_EDIT, OnEnUpdateMulMatrixCustom42Edit)
	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_43_EDIT, OnEnUpdateMulMatrixCustom43Edit)
	ON_EN_UPDATE(IDC_MUL_MATRIX_CUSTOM_44_EDIT, OnEnUpdateMulMatrixCustom44Edit)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_EN_UPDATE(IDC_RESOLUTION_EDIT, OnEnUpdateResolutionEdit)
	ON_EN_UPDATE(IDC_HOPF_FIELD_RESOLUTION_EDIT, OnEnUpdateHopfFieldResolutionEdit)
	ON_EN_UPDATE(IDC_NORMAL_QUALITY_EDIT, OnEnUpdateNormalQualityEdit)
	ON_BN_CLICKED(IDC_PROP_RES_CHECK, OnBnClickedPropResCheck)
	END_MESSAGE_MAP()


// Cjulia4d2Doc construction/destruction

Cjulia4d2Doc::Cjulia4d2Doc()
{
	ResetVars();
}

Cjulia4d2Doc::~Cjulia4d2Doc()
{
}

BOOL Cjulia4d2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	ResetVars();
	SetParentVars();

	return TRUE;
}




// Cjulia4d2Doc serialization

void Cjulia4d2Doc::Serialize(CArchive& ar)
{
	char randomizer = 25;

	if (ar.IsStoring())
	{
		ar << J4D2_FILE_HDR4;

		ar << iterations;
		ar << infinity;
		ar << resolution;
		ar << formula;

		string temp_formula_text = formula_text;

		// disabled in file version 3
		/*
		for(size_t i = 0; i < temp_formula_text.length(); i++) // mix it up
		{
			if(i%2)
				temp_formula_text[i] = temp_formula_text[i] + randomizer + (char)i;
			else
				temp_formula_text[i] = temp_formula_text[i] - randomizer - (char)i;
		}
*/

		CString buf = temp_formula_text.c_str();
		ar << buf;

		ar << c_x;
		ar << c_y;
		ar << c_z;
		ar << c_w;
		ar << z_w;
		ar << a;

		ar << mul_column_type;

		ar << mul_column_const_vals_x;
		ar << mul_column_const_vals_y;
		ar << mul_column_const_vals_z;
		ar << mul_column_const_vals_w;

		ar << mul_column_custom_x;
		ar << mul_column_custom_y;
		ar << mul_column_custom_z;
		ar << mul_column_custom_w;

		ar << mul_matrix_type;

		ar << mul_matrix_custom_11;
		ar << mul_matrix_custom_12;
		ar << mul_matrix_custom_13;
		ar << mul_matrix_custom_14;

		ar << mul_matrix_custom_21;
		ar << mul_matrix_custom_22;
		ar << mul_matrix_custom_23;
		ar << mul_matrix_custom_24;

		ar << mul_matrix_custom_31;
		ar << mul_matrix_custom_32;
		ar << mul_matrix_custom_33;
		ar << mul_matrix_custom_34;

		ar << mul_matrix_custom_41;
		ar << mul_matrix_custom_42;
		ar << mul_matrix_custom_43;
		ar << mul_matrix_custom_44;

		ar << x_min;
		ar << x_max;
		ar << y_min;
		ar << y_max;
		ar << z_min;
		ar << z_max;
		ar << w_min;
		ar << w_max;

		ar << hopf_map;
		ar << swap_z_and_c;
		ar << reverse_set;
		ar << cull_non_surface_points;

		ar << samples;

		// store blank values for these deprecated items, for compatibility
		double temp_double = 0.0;
		long unsigned int temp_long = 0;

		ar << temp_double; //ar << isovalue_percent;
		ar << temp_double; //ar << avg_percent;
		ar << temp_long; //ar << avg_passes;

		ar << render_to_type;

		arc_ball arcball;

		if(!write_ident_arcball)
		{
			POSITION pos = GetFirstViewPosition();
			Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
			CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
			arcball = parent_frame->GetArcBall();
		}

		ar << arcball.rotation_radius;
		ar << arcball.translation_radius;
		ar << arcball.rotating;

		ar << arcball.down_quat.w;
		ar << arcball.down_quat.x;
		ar << arcball.down_quat.y;
		ar << arcball.down_quat.z;

		ar << arcball.now_quat.w;
		ar << arcball.now_quat.x;
		ar << arcball.now_quat.y;
		ar << arcball.now_quat.z;

		ar << arcball.rotation_matrix(0,0);
		ar << arcball.rotation_matrix(0,1);
		ar << arcball.rotation_matrix(0,2);
		ar << arcball.rotation_matrix(0,3);
		ar << arcball.rotation_matrix(1,0);
		ar << arcball.rotation_matrix(1,1);
		ar << arcball.rotation_matrix(1,2);
		ar << arcball.rotation_matrix(1,3);
		ar << arcball.rotation_matrix(2,0);
		ar << arcball.rotation_matrix(2,1);
		ar << arcball.rotation_matrix(2,2);
		ar << arcball.rotation_matrix(2,3);
		ar << arcball.rotation_matrix(3,0);
		ar << arcball.rotation_matrix(3,1);
		ar << arcball.rotation_matrix(3,2);
		ar << arcball.rotation_matrix(3,3);

		ar << arcball.translation_matrix(0,0);
		ar << arcball.translation_matrix(0,1);
		ar << arcball.translation_matrix(0,2);
		ar << arcball.translation_matrix(0,3);
		ar << arcball.translation_matrix(1,0);
		ar << arcball.translation_matrix(1,1);
		ar << arcball.translation_matrix(1,2);
		ar << arcball.translation_matrix(1,3);
		ar << arcball.translation_matrix(2,0);
		ar << arcball.translation_matrix(2,1);
		ar << arcball.translation_matrix(2,2);
		ar << arcball.translation_matrix(2,3);
		ar << arcball.translation_matrix(3,0);
		ar << arcball.translation_matrix(3,1);
		ar << arcball.translation_matrix(3,2);
		ar << arcball.translation_matrix(3,3);

		ar << arcball.translation_delta_matrix(0,0);
		ar << arcball.translation_delta_matrix(0,1);
		ar << arcball.translation_delta_matrix(0,2);
		ar << arcball.translation_delta_matrix(0,3);
		ar << arcball.translation_delta_matrix(1,0);
		ar << arcball.translation_delta_matrix(1,1);
		ar << arcball.translation_delta_matrix(1,2);
		ar << arcball.translation_delta_matrix(1,3);
		ar << arcball.translation_delta_matrix(2,0);
		ar << arcball.translation_delta_matrix(2,1);
		ar << arcball.translation_delta_matrix(2,2);
		ar << arcball.translation_delta_matrix(2,3);
		ar << arcball.translation_delta_matrix(3,0);
		ar << arcball.translation_delta_matrix(3,1);
		ar << arcball.translation_delta_matrix(3,2);
		ar << arcball.translation_delta_matrix(3,3);

		ar << arcball.down_vector.x;
		ar << arcball.down_vector.y;
		ar << arcball.down_vector.z;

		ar << proportional_res;
	}
	else
	{
		CString file_header;

		ar >> file_header;

		if(file_header != J4D2_FILE_HDR && file_header != J4D2_FILE_HDR3 && file_header != J4D2_FILE_HDR4)
		{
			CString error = "Reading of this file's format is not supported by Julia 4D 2:";
			error += "\n";

			CFile* fp = ar.GetFile();
			error += fp->GetFilePath();

			MessageBox(NULL, error.GetBuffer(), APP_NAME, MB_OK|MB_ICONSTOP);

			OnNewDocument();
			return;
		}



		ar >> iterations;
		ar >> infinity;
		ar >> resolution;
		ar >> formula;

		CString buf;
		ar >> buf;
		formula_text = buf.GetBuffer();

		if(file_header == J4D2_FILE_HDR)
		for(size_t i = 0; i < formula_text.length(); i++) // mix it up
		{
			if(i%2)
				formula_text[i] = formula_text[i] - randomizer - (char)i;
			else
				formula_text[i] = formula_text[i] + randomizer + (char)i;
		}


		ar >> c_x;
		ar >> c_y;
		ar >> c_z;
		ar >> c_w;
		ar >> z_w;
		ar >> a;

		ar >> mul_column_type;

		ar >> mul_column_const_vals_x;
		ar >> mul_column_const_vals_y;
		ar >> mul_column_const_vals_z;
		ar >> mul_column_const_vals_w;

		ar >> mul_column_custom_x;
		ar >> mul_column_custom_y;
		ar >> mul_column_custom_z;
		ar >> mul_column_custom_w;

		ar >> mul_matrix_type;

		ar >> mul_matrix_custom_11;
		ar >> mul_matrix_custom_12;
		ar >> mul_matrix_custom_13;
		ar >> mul_matrix_custom_14;

		ar >> mul_matrix_custom_21;
		ar >> mul_matrix_custom_22;
		ar >> mul_matrix_custom_23;
		ar >> mul_matrix_custom_24;

		ar >> mul_matrix_custom_31;
		ar >> mul_matrix_custom_32;
		ar >> mul_matrix_custom_33;
		ar >> mul_matrix_custom_34;

		ar >> mul_matrix_custom_41;
		ar >> mul_matrix_custom_42;
		ar >> mul_matrix_custom_43;
		ar >> mul_matrix_custom_44;

		ar >> x_min;
		ar >> x_max;
		ar >> y_min;
		ar >> y_max;
		ar >> z_min;
		ar >> z_max;
		ar >> w_min;
		ar >> w_max;

		ar >> hopf_map;
		ar >> swap_z_and_c;
		ar >> reverse_set;
		ar >> cull_non_surface_points;

		ar >> samples;

		// retrieve blank values for these deprecated items, for compatibility
		double temp_double = 0.0;
		long unsigned int temp_long = 0;
		ar >> temp_double;//isovalue_percent;
		ar >> temp_double;//avg_percent;
		ar >> temp_long;//avg_passes;

		ar >> render_to_type;


		arc_ball arcball;

		ar >> arcball.rotation_radius;
		ar >> arcball.translation_radius;
		ar >> arcball.rotating;

		ar >> arcball.down_quat.w;
		ar >> arcball.down_quat.x;
		ar >> arcball.down_quat.y;
		ar >> arcball.down_quat.z;

		ar >> arcball.now_quat.w;
		ar >> arcball.now_quat.x;
		ar >> arcball.now_quat.y;
		ar >> arcball.now_quat.z;

		ar >> arcball.rotation_matrix(0,0);
		ar >> arcball.rotation_matrix(0,1);
		ar >> arcball.rotation_matrix(0,2);
		ar >> arcball.rotation_matrix(0,3);
		ar >> arcball.rotation_matrix(1,0);
		ar >> arcball.rotation_matrix(1,1);
		ar >> arcball.rotation_matrix(1,2);
		ar >> arcball.rotation_matrix(1,3);
		ar >> arcball.rotation_matrix(2,0);
		ar >> arcball.rotation_matrix(2,1);
		ar >> arcball.rotation_matrix(2,2);
		ar >> arcball.rotation_matrix(2,3);
		ar >> arcball.rotation_matrix(3,0);
		ar >> arcball.rotation_matrix(3,1);
		ar >> arcball.rotation_matrix(3,2);
		ar >> arcball.rotation_matrix(3,3);

		ar >> arcball.translation_matrix(0,0);
		ar >> arcball.translation_matrix(0,1);
		ar >> arcball.translation_matrix(0,2);
		ar >> arcball.translation_matrix(0,3);
		ar >> arcball.translation_matrix(1,0);
		ar >> arcball.translation_matrix(1,1);
		ar >> arcball.translation_matrix(1,2);
		ar >> arcball.translation_matrix(1,3);
		ar >> arcball.translation_matrix(2,0);
		ar >> arcball.translation_matrix(2,1);
		ar >> arcball.translation_matrix(2,2);
		ar >> arcball.translation_matrix(2,3);
		ar >> arcball.translation_matrix(3,0);
		ar >> arcball.translation_matrix(3,1);
		ar >> arcball.translation_matrix(3,2);
		ar >> arcball.translation_matrix(3,3);

		ar >> arcball.translation_delta_matrix(0,0);
		ar >> arcball.translation_delta_matrix(0,1);
		ar >> arcball.translation_delta_matrix(0,2);
		ar >> arcball.translation_delta_matrix(0,3);
		ar >> arcball.translation_delta_matrix(1,0);
		ar >> arcball.translation_delta_matrix(1,1);
		ar >> arcball.translation_delta_matrix(1,2);
		ar >> arcball.translation_delta_matrix(1,3);
		ar >> arcball.translation_delta_matrix(2,0);
		ar >> arcball.translation_delta_matrix(2,1);
		ar >> arcball.translation_delta_matrix(2,2);
		ar >> arcball.translation_delta_matrix(2,3);
		ar >> arcball.translation_delta_matrix(3,0);
		ar >> arcball.translation_delta_matrix(3,1);
		ar >> arcball.translation_delta_matrix(3,2);
		ar >> arcball.translation_delta_matrix(3,3);

		ar >> arcball.down_vector.x;
		ar >> arcball.down_vector.y;
		ar >> arcball.down_vector.z;

		if(J4D2_FILE_HDR4 == file_header)
			ar >> proportional_res;

		if(set_parent_vars)
		{
			POSITION pos = GetFirstViewPosition();
			Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
			CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

			parent_frame->SetVars(*this);

			RECT client_rect;
			RECT erase_rect;

			parent_frame->GetDrawableClientRect(client_rect, erase_rect);

			arcball.window_width = client_rect.right - client_rect.left;
			arcball.window_height = client_rect.bottom - client_rect.top;

			parent_frame->SetArcBall(arcball);
		}
	}
}


// Cjulia4d2Doc diagnostics

#ifdef _DEBUG
void Cjulia4d2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cjulia4d2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cjulia4d2Doc commands

void Cjulia4d2Doc::ResetVars(void)
{
	iterations = 8;
	infinity = 4.0;
	resolution = 100;
	formula = CLASSIC_ITERATOR;
	formula_text = "Classic: Z = Z^a + C";

	c_x = 0.2866;
	c_y = 0.5133;
	c_z = 0.46;
	c_w = 0.2467;

	z_w = 0.0;

	a = 2.0;

	mul_column_type = MUL_COLUMN_DEFAULT;

	mul_column_const_vals_x = 1.0;
	mul_column_const_vals_y = 1.0;
	mul_column_const_vals_z = 1.0;
	mul_column_const_vals_w = 1.0;

	mul_column_custom_x = MUL_PLUS_X;
	mul_column_custom_y = MUL_PLUS_Y;
	mul_column_custom_z = MUL_PLUS_Z;
	mul_column_custom_w = MUL_PLUS_W;

	mul_matrix_type = MUL_MATRIX_DEFAULT;

	mul_matrix_custom_11 = MUL_PLUS_X;
	mul_matrix_custom_12 = MUL_MINUS_Y;
	mul_matrix_custom_13 = MUL_MINUS_Z;
	mul_matrix_custom_14 = MUL_MINUS_W;

	mul_matrix_custom_21 = MUL_PLUS_Y;
	mul_matrix_custom_22 = MUL_PLUS_X;
	mul_matrix_custom_23 = MUL_PLUS_W;
	mul_matrix_custom_24 = MUL_MINUS_Z;

	mul_matrix_custom_31 = MUL_PLUS_Z;
	mul_matrix_custom_32 = MUL_MINUS_W;
	mul_matrix_custom_33 = MUL_PLUS_X;
	mul_matrix_custom_34 = MUL_PLUS_Y;

	mul_matrix_custom_41 = MUL_PLUS_W;
	mul_matrix_custom_42 = MUL_PLUS_Z;
	mul_matrix_custom_43 = MUL_MINUS_Y;
	mul_matrix_custom_44 = MUL_PLUS_X;

	x_min = -1.5;
	x_max =  1.5;
	y_min = -1.5;
	y_max =  1.5;
	z_min = -1.5;
	z_max =  1.5;
	w_min = -1.5;
	w_max =  1.5;

	hopf_map = false;
	swap_z_and_c = false;
	reverse_set = false;
	cull_non_surface_points = false;

	samples = 0;

	render_to_type = POINT_SET;

	export_file_name = "";

	write_ident_arcball = false;
	set_parent_vars = true;

//	light_by_normals = true;

	hopf_field_resolution = 100;
	normal_gen_radius = 1;

	proportional_res = false;

	x_resolution = 100;
	y_resolution = 100;
	z_resolution = 100;
	w_resolution = 100;

	proportional_res = true;
}

void Cjulia4d2Doc::SetParentVars(void)
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->SetVars(*this);
}


bool Cjulia4d2Doc::operator==(const Cjulia4d2Doc &rhs)
{
	if(iterations != rhs.iterations)
		return false;

	if(infinity != rhs.infinity)
		return false;

	if(resolution != rhs.resolution)
		return false;

	if(formula != rhs.formula)
		return false;

	if(lower_string(formula_text) != lower_string(rhs.formula_text))
		return false;


	if(c_x != rhs.c_x)
		return false;

	if(c_y != rhs.c_y)
		return false;

	if(c_z != rhs.c_z)
		return false;

	if(c_w != rhs.c_w)
		return false;


	if(z_w != rhs.z_w)
		return false;


	if(a != rhs.a)
		return false;


	if(mul_column_type != rhs.mul_column_type)
		return false;


	if(mul_column_const_vals_x != rhs.mul_column_const_vals_x)
		return false;

	if(mul_column_const_vals_y != rhs.mul_column_const_vals_y)
		return false;

	if(mul_column_const_vals_z != rhs.mul_column_const_vals_z)
		return false;

	if(mul_column_const_vals_w != rhs.mul_column_const_vals_w)
		return false;


	if(mul_column_custom_x != rhs.mul_column_custom_x)
		return false;

	if(mul_column_custom_y != rhs.mul_column_custom_y)
		return false;

	if(mul_column_custom_z != rhs.mul_column_custom_z)
		return false;

	if(mul_column_custom_w != rhs.mul_column_custom_w)
		return false;


	if(mul_matrix_type != rhs.mul_matrix_type)
		return false;


	if(mul_matrix_custom_11 != rhs.mul_matrix_custom_11)
		return false;

	if(mul_matrix_custom_12 != rhs.mul_matrix_custom_12)
		return false;

	if(mul_matrix_custom_13 != rhs.mul_matrix_custom_13)
		return false;

	if(mul_matrix_custom_14 != rhs.mul_matrix_custom_14)
		return false;


	if(mul_matrix_custom_21 != rhs.mul_matrix_custom_21)
		return false;

	if(mul_matrix_custom_22 != rhs.mul_matrix_custom_22)
		return false;

	if(mul_matrix_custom_23 != rhs.mul_matrix_custom_23)
		return false;

	if(mul_matrix_custom_24 != rhs.mul_matrix_custom_24)
		return false;


	if(mul_matrix_custom_31 != rhs.mul_matrix_custom_31)
		return false;

	if(mul_matrix_custom_32 != rhs.mul_matrix_custom_32)
		return false;

	if(mul_matrix_custom_33 != rhs.mul_matrix_custom_33)
		return false;

	if(mul_matrix_custom_34 != rhs.mul_matrix_custom_34)
		return false;


	if(mul_matrix_custom_41 != rhs.mul_matrix_custom_41)
		return false;

	if(mul_matrix_custom_42 != rhs.mul_matrix_custom_42)
		return false;

	if(mul_matrix_custom_43 != rhs.mul_matrix_custom_43)
		return false;

	if(mul_matrix_custom_44 != rhs.mul_matrix_custom_44)
		return false;


	if(x_min != rhs.x_min)
		return false;

	if(x_max != rhs.x_max)
		return false;

	if(y_min != rhs.y_min)
		return false;

	if(y_max != rhs.y_max)
		return false;

	if(z_min != rhs.z_min)
		return false;

	if(z_max != rhs.z_max)
		return false;

	if(w_min != rhs.w_min)
		return false;

	if(w_max != rhs.w_max)
		return false;


	if(hopf_map != rhs.hopf_map)
		return false;

	if(swap_z_and_c != rhs.swap_z_and_c)
		return false;

	if(reverse_set != rhs.reverse_set)
		return false;

	if(cull_non_surface_points != rhs.cull_non_surface_points)
		return false;

	if(proportional_res != rhs.proportional_res)
		return false;

	if(samples != rhs.samples)
		return false;


	// NOTE: DO NOT USE INDIVIDUAL RESOLUTION PARAMS HERE!!!


	//	if(render_to_type != rhs.render_to_type)
	//		return false;

	//	if(export_file_name != rhs.export_file_name)
	//		return false;


	return true;
}



void Cjulia4d2Doc::operator=(const Cjulia4d2Doc &rhs)
{
	iterations = rhs.iterations;


	infinity = rhs.infinity;


	resolution = rhs.resolution;


	formula = rhs.formula;


	formula_text = rhs.formula_text;



	c_x = rhs.c_x;


	c_y = rhs.c_y;


	c_z = rhs.c_z;


	c_w = rhs.c_w;



	z_w = rhs.z_w;



	a = rhs.a;



	mul_column_type = rhs.mul_column_type;



	mul_column_const_vals_x = rhs.mul_column_const_vals_x;


	mul_column_const_vals_y = rhs.mul_column_const_vals_y;


	mul_column_const_vals_z = rhs.mul_column_const_vals_z;


	mul_column_const_vals_w = rhs.mul_column_const_vals_w;



	mul_column_custom_x = rhs.mul_column_custom_x;


	mul_column_custom_y = rhs.mul_column_custom_y;


	mul_column_custom_z = rhs.mul_column_custom_z;


	mul_column_custom_w = rhs.mul_column_custom_w;



	mul_matrix_type = rhs.mul_matrix_type;



	mul_matrix_custom_11 = rhs.mul_matrix_custom_11;


	mul_matrix_custom_12 = rhs.mul_matrix_custom_12;


	mul_matrix_custom_13 = rhs.mul_matrix_custom_13;


	mul_matrix_custom_14 = rhs.mul_matrix_custom_14;



	mul_matrix_custom_21 = rhs.mul_matrix_custom_21;


	mul_matrix_custom_22 = rhs.mul_matrix_custom_22;


	mul_matrix_custom_23 = rhs.mul_matrix_custom_23;


	mul_matrix_custom_24 = rhs.mul_matrix_custom_24;



	mul_matrix_custom_31 = rhs.mul_matrix_custom_31;


	mul_matrix_custom_32 = rhs.mul_matrix_custom_32;


	mul_matrix_custom_33 = rhs.mul_matrix_custom_33;


	mul_matrix_custom_34 = rhs.mul_matrix_custom_34;



	mul_matrix_custom_41 = rhs.mul_matrix_custom_41;


	mul_matrix_custom_42 = rhs.mul_matrix_custom_42;


	mul_matrix_custom_43 = rhs.mul_matrix_custom_43;


	mul_matrix_custom_44 = rhs.mul_matrix_custom_44;



	x_min = rhs.x_min;


	x_max = rhs.x_max;


	y_min = rhs.y_min;


	y_max = rhs.y_max;


	z_min = rhs.z_min;


	z_max = rhs.z_max;


	w_min = rhs.w_min;


	w_max = rhs.w_max;



	hopf_map = rhs.hopf_map;


	swap_z_and_c = rhs.swap_z_and_c;


	reverse_set = rhs.reverse_set;


	cull_non_surface_points = rhs.cull_non_surface_points;


	proportional_res = rhs.proportional_res;


	samples = rhs.samples;



	render_to_type = rhs.render_to_type;


	export_file_name = rhs.export_file_name;


//	light_by_normals = rhs.light_by_normals;


	hopf_field_resolution = rhs.hopf_field_resolution;
	normal_gen_radius = rhs.normal_gen_radius;


	// not really necessary
	x_resolution = rhs.x_resolution;
	y_resolution = rhs.y_resolution;
	z_resolution = rhs.z_resolution;
	w_resolution = rhs.w_resolution;
}



void Cjulia4d2Doc::OnEnUpdateCXEdit() { ProcessFloatingPointMainSettingsEdit(IDC_C_X_EDIT, c_x);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeCXEdit();
}

void Cjulia4d2Doc::OnEnUpdateCYEdit() { ProcessFloatingPointMainSettingsEdit(IDC_C_Y_EDIT, c_y);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeCYEdit();
}

void Cjulia4d2Doc::OnEnUpdateCZEdit() { ProcessFloatingPointMainSettingsEdit(IDC_C_Z_EDIT, c_z);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeCZEdit();
}

void Cjulia4d2Doc::OnEnUpdateCWEdit() { ProcessFloatingPointMainSettingsEdit(IDC_C_W_EDIT, c_w);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeCWEdit();
}


void Cjulia4d2Doc::OnEnUpdateXMinEdit() { ProcessFloatingPointMainSettingsEdit(IDC_X_MIN_EDIT, x_min);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeXMinEdit();
}

void Cjulia4d2Doc::OnEnUpdateXMaxEdit() { ProcessFloatingPointMainSettingsEdit(IDC_X_MAX_EDIT, x_max);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeXMaxEdit();
}

void Cjulia4d2Doc::OnEnUpdateYMinEdit() { ProcessFloatingPointMainSettingsEdit(IDC_Y_MIN_EDIT, y_min);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeYMinEdit();
}

void Cjulia4d2Doc::OnEnUpdateYMaxEdit() { ProcessFloatingPointMainSettingsEdit(IDC_Y_MAX_EDIT, y_max);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeYMaxEdit();
}

void Cjulia4d2Doc::OnEnUpdateZMinEdit() { ProcessFloatingPointMainSettingsEdit(IDC_Z_MIN_EDIT, z_min);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeZMinEdit();
}

void Cjulia4d2Doc::OnEnUpdateZMaxEdit() { ProcessFloatingPointMainSettingsEdit(IDC_Z_MAX_EDIT, z_max);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeZMaxEdit();
}

void Cjulia4d2Doc::OnEnUpdateWMinEdit() { ProcessFloatingPointMainSettingsEdit(IDC_W_MIN_EDIT, w_min);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeWMinEdit();
}

void Cjulia4d2Doc::OnEnUpdateWMaxEdit() { ProcessFloatingPointMainSettingsEdit(IDC_W_MAX_EDIT, w_max);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeWMaxEdit();
}


void Cjulia4d2Doc::OnEnUpdateZWEdit() { ProcessFloatingPointMainSettingsEdit(IDC_Z_W_EDIT, z_w);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeZWEdit();
}

void Cjulia4d2Doc::OnEnUpdateAEdit() { ProcessFloatingPointMainSettingsEdit(IDC_A_EDIT, a);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeAEdit();
}

void Cjulia4d2Doc::OnEnUpdateInfinityEdit() { ProcessFloatingPointMainSettingsEdit(IDC_INFINITY_EDIT, infinity);

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeInfinityEdit();
}

void Cjulia4d2Doc::OnEnUpdateResolutionEdit()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *main_settings_dialog = &parent_frame->m_wndMainSettingsDialog;

	CString buf;
	main_settings_dialog->GetDlgItemText(IDC_RESOLUTION_EDIT, buf);

	double temp_double = atof(buf.GetBuffer());
/*	
	if(temp_double < 3.0)
	{
		temp_double = 3.0;
		main_settings_dialog->SetDlgItemText(IDC_RESOLUTION_EDIT, "3");
	}
	else if(temp_double > 1625.0)
	{
		temp_double = 1625.0;
		main_settings_dialog->SetDlgItemText(IDC_RESOLUTION_EDIT, "1625");
	}
*/

	resolution = static_cast<long unsigned int>(temp_double);

	this->SetModifiedFlag(TRUE);

	parent_frame->OnEnChangeResolutionEdit();
}




void Cjulia4d2Doc::OnEnUpdateFormulaEdit()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *main_settings_dialog = &parent_frame->m_wndMainSettingsDialog;

	CString buf;
	main_settings_dialog->GetDlgItemText(IDC_FORMULA_EDIT, buf);

	formula_text = buf.GetBuffer();

	this->SetModifiedFlag(TRUE);

	parent_frame->OnEnChangeFormulaEdit();
}


void Cjulia4d2Doc::OnEnUpdateIterationsEdit()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *main_settings_dialog = &parent_frame->m_wndMainSettingsDialog;

	CString buf;
	main_settings_dialog->GetDlgItemText(IDC_ITERATIONS_EDIT, buf);

	iterations = atol(buf.GetBuffer());

	this->SetModifiedFlag(TRUE);

	parent_frame->OnEnChangeIterationsEdit();
}





void Cjulia4d2Doc::OnEnUpdateSamplesEdit()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *main_settings_dialog = &parent_frame->m_wndMainSettingsDialog;

	CString buf;
	main_settings_dialog->GetDlgItemText(IDC_SAMPLES_EDIT, buf);


	double temp_double = atof(buf.GetBuffer());

	if(temp_double > 4294967295.0)
	{
		temp_double = 4294967295;
		main_settings_dialog->SetDlgItemText(IDC_SAMPLES_EDIT, "4294967295");
	}

	samples = static_cast<long unsigned int>(temp_double);
	
	this->SetModifiedFlag(TRUE);

	parent_frame->OnEnChangeSamplesEdit();
}












void Cjulia4d2Doc::OnFormulaCustom()
{
	formula = CUSTOM_ITERATOR;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnFormulaCustom();
}



void Cjulia4d2Doc::OnGeneralClassic()
{
	formula = CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnGeneralClassic();
}


void Cjulia4d2Doc::OnGeneralExponential()
{
	formula = EXPONENTIAL_ITERATOR;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnGeneralExponential();
}


void Cjulia4d2Doc::OnGeneralQuadratic()
{
	formula = QUADRATIC_ITERATOR;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnGeneralQuadratic();
}


void Cjulia4d2Doc::OnGeneralLambda()
{
	formula = LAMBDA_ITERATOR;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnGeneralLambda();
}

void Cjulia4d2Doc::OnGeneralFunkyBlobs()
{
	formula = FUNKY_BLOBS_ITERATOR;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnGeneralFunkyBlobs();
}



void Cjulia4d2Doc::OnSineSine()
{
	formula = SINE_ITERATOR;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnSineSine();
}

void Cjulia4d2Doc::OnSineSinePlus()
{
	formula = SINE_PLUS_ITERATOR;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnSineSinePlus();
}


void Cjulia4d2Doc::OnSineSineClassic()
{
	formula = SINE_CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnSineSineClassic();
}


void Cjulia4d2Doc::OnCosineCosine()
{
	formula = COSINE_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnCosineCosine();
}

void Cjulia4d2Doc::OnCosineCosinePlus()
{
	formula = COSINE_PLUS_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnCosineCosinePlus();
}

void Cjulia4d2Doc::OnCosineCosineClassic()
{
	formula = COSINE_CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnCosineCosineClassic();
}



void Cjulia4d2Doc::OnTangentTangent()
{
	formula = TANGENT_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnTangentTangent();
}


void Cjulia4d2Doc::OnTangentTangentPlus()
{
	formula = TANGENT_PLUS_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnTangentTangentPlus();
}


void Cjulia4d2Doc::OnTangentTangentClassic()
{
	formula = TANGENT_CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnTangentTangentClassic();
}



void Cjulia4d2Doc::OnSecantSecant()
{
	formula = SECANT_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnSecantSecant();
}


void Cjulia4d2Doc::OnSecantSecantPlus()
{
	formula = SECANT_PLUS_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnSecantSecantPlus();
}


void Cjulia4d2Doc::OnSecantSecantClassic()
{
	formula = SECANT_CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnSecantSecantClassic();
}



void Cjulia4d2Doc::OnCosecantCosecant()
{
	formula = COSECANT_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnCosecantCosecant();
}


void Cjulia4d2Doc::OnCosecantCosecantPlus()
{
	formula = COSECANT_PLUS_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnCosecantCosecantPlus();
}


void Cjulia4d2Doc::OnCosecantCosecantClassic()
{
	formula = COSECANT_CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnCosecantCosecantClassic();
}

void Cjulia4d2Doc::OnCotangentCotangent()
{
	formula = COTANGENT_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnCotangentCotangent();
}


void Cjulia4d2Doc::OnCotangentCotangentPlus()
{
	formula = COTANGENT_PLUS_ITERATOR;
	this->SetModifiedFlag(TRUE);



	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnCotangentCotangentPlus();
}

void Cjulia4d2Doc::OnCotangentCotangentClassic()
{
	formula = COTANGENT_CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnCotangentCotangentClassic();
}














void Cjulia4d2Doc::OnHyperbolicSineHyperbolicSine()
{
	formula = HYPERBOLIC_SINE_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicSineHyperbolicSine();
}


void Cjulia4d2Doc::OnHyperbolicSineHyperbolicSinePlus()
{
	formula = HYPERBOLIC_SINE_PLUS_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicSineHyperbolicSinePlus();
}


void Cjulia4d2Doc::OnHyperbolicSineHyperbolicSineClassic()
{
	formula = HYPERBOLIC_SINE_CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicSineHyperbolicSineClassic();
}



void Cjulia4d2Doc::OnHyperbolicCosineHyperbolicCosine()
{
	formula = HYPERBOLIC_COSINE_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicCosineHyperbolicCosine();
}


void Cjulia4d2Doc::OnHyperbolicCosineHyperbolicCosinePlus()
{
	formula = HYPERBOLIC_COSINE_PLUS_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicCosineHyperbolicCosinePlus();
}


void Cjulia4d2Doc::OnHyperbolicCosineHyperbolicCosineClassic()
{
	formula = HYPERBOLIC_COSINE_CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicCosineHyperbolicCosineClassic();
}




void Cjulia4d2Doc::OnHyperbolicTangentHyperbolicTangent()
{
	formula = HYPERBOLIC_TANGENT_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicTangentHyperbolicTangent();
}


void Cjulia4d2Doc::OnHyperbolicTangentHyperbolicTangentPlus()
{
	formula = HYPERBOLIC_TANGENT_PLUS_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicTangentHyperbolicTangentPlus();
}


void Cjulia4d2Doc::OnHyperbolicTangentHyperbolicTangentClassic()
{
	formula = HYPERBOLIC_TANGENT_CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicTangentHyperbolicTangentClassic();
}


void Cjulia4d2Doc::OnHyperbolicSecantHyperbolicSecant()
{
	formula = HYPERBOLIC_SECANT_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicSecantHyperbolicSecant();
}


void Cjulia4d2Doc::OnHyperbolicSecantHyperbolicSecantPlus()
{
	formula = HYPERBOLIC_SECANT_PLUS_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicSecantHyperbolicSecantPlus();
}


void Cjulia4d2Doc::OnHyperbolicSecantHyperbolicSecantClassic()
{
	formula = HYPERBOLIC_SECANT_CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicSecantHyperbolicSecantClassic();
}



void Cjulia4d2Doc::OnHyperbolicCosecantHyperbolicCosecant()
{
	formula = HYPERBOLIC_COSECANT_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicCosecantHyperbolicCosecant();
}


void Cjulia4d2Doc::OnHyperbolicCosecantHyperbolicCosecantPlus()
{
	formula = HYPERBOLIC_COSECANT_PLUS_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicCosecantHyperbolicCosecantPlus();
}


void Cjulia4d2Doc::OnHyperbolicCosecantHyperbolicCosecantClassic()
{
	formula = HYPERBOLIC_COSECANT_CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicCosecantHyperbolicCosecantClassic();
}


void Cjulia4d2Doc::OnHyperbolicCotangentHyperbolicCotangent()
{
	formula = HYPERBOLIC_COTANGENT_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicCotangentHyperbolicCotangent();
}


void Cjulia4d2Doc::OnHyperbolicCotangentHyperbolicCotangentPlus()
{
	formula = HYPERBOLIC_COTANGENT_PLUS_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicCotangentHyperbolicCotangentPlus();
}


void Cjulia4d2Doc::OnHyperbolicCotangentHyperbolicCotangentClassic()
{
	formula = HYPERBOLIC_COTANGENT_CLASSIC_ITERATOR;
	this->SetModifiedFlag(TRUE);


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnHyperbolicCotangentHyperbolicCotangentClassic();
}























bool Cjulia4d2Doc::ProcessXYZWMinMaxEdits(const DWORD &ID, double &target_var)
{
	if( ID != IDC_X_MIN_EDIT && ID != IDC_X_MAX_EDIT &&
		ID != IDC_Y_MIN_EDIT && ID != IDC_Y_MAX_EDIT &&
		ID != IDC_Z_MIN_EDIT && ID != IDC_Z_MAX_EDIT &&
		ID != IDC_W_MIN_EDIT && ID != IDC_W_MAX_EDIT )
	{
		return false;
	}


	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *main_settings_dialog = &parent_frame->m_wndMainSettingsDialog;

	CString buf;
	main_settings_dialog->GetDlgItemText(ID, buf);

	if(buf.IsEmpty() || buf == "-")
	{
		double temp_var = 0.0;

		if(ID == IDC_X_MIN_EDIT)
		{
			if(temp_var >= x_max)
			{
				MessageBeep(MB_ICONEXCLAMATION);

				ostringstream out;
				out << setiosflags(std::ios_base::fixed);
				out << setprecision(14);

				out << target_var;

				size_t last_not_zero_pos = 0;

				if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
				{
					out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str() += "0";
				}

				main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

				DWORD SelStart = 0;
				DWORD SelEnd = 0;
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

				return true;
			}
			else
			{
				target_var = temp_var;
				this->SetModifiedFlag(TRUE);
				return true;
			}
		}
		else if(ID == IDC_X_MAX_EDIT)
		{
			if(temp_var <= x_min)
			{
				MessageBeep(MB_ICONEXCLAMATION);

				ostringstream out;
				out << setiosflags(std::ios_base::fixed);
				out << setprecision(14);

				out << target_var;

				size_t last_not_zero_pos = 0;

				if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
				{
					out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
				}

				main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

				DWORD SelStart = 0;
				DWORD SelEnd = 0;
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

				return true;
			}
			else
			{
				target_var = temp_var;
				this->SetModifiedFlag(TRUE);
				return true;
			}
		}
		else if(ID == IDC_Y_MIN_EDIT)
		{
			if(temp_var >= y_max)
			{
				MessageBeep(MB_ICONEXCLAMATION);

				ostringstream out;
				out << setiosflags(std::ios_base::fixed);
				out << setprecision(14);

				out << target_var;

				size_t last_not_zero_pos = 0;

				if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
				{
					out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
				}

				main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

				DWORD SelStart = 0;
				DWORD SelEnd = 0;
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

				return true;
			}
			else
			{
				target_var = temp_var;
				this->SetModifiedFlag(TRUE);
				return true;
			}
		}
		else if(ID == IDC_Y_MAX_EDIT)
		{
			if(temp_var <= y_min)
			{
				MessageBeep(MB_ICONEXCLAMATION);

				ostringstream out;
				out << setiosflags(std::ios_base::fixed);
				out << setprecision(14);

				out << target_var;

				size_t last_not_zero_pos = 0;

				if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
				{
					out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
				}

				main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

				DWORD SelStart = 0;
				DWORD SelEnd = 0;
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

				return true;
			}
			else
			{
				target_var = temp_var;
				this->SetModifiedFlag(TRUE);
				return true;
			}
		}
		else if(ID == IDC_Z_MIN_EDIT)
		{
			if(temp_var >= z_max)
			{
				MessageBeep(MB_ICONEXCLAMATION);

				ostringstream out;
				out << setiosflags(std::ios_base::fixed);
				out << setprecision(14);

				out << target_var;

				size_t last_not_zero_pos = 0;

				if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
				{
					out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
				}

				main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

				DWORD SelStart = 0;
				DWORD SelEnd = 0;
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

				return true;
			}
			else
			{
				target_var = temp_var;
				this->SetModifiedFlag(TRUE);
				return true;
			}
		}
		else if(ID == IDC_Z_MAX_EDIT)
		{
			if(temp_var <= z_min)
			{
				MessageBeep(MB_ICONEXCLAMATION);

				ostringstream out;
				out << setiosflags(std::ios_base::fixed);
				out << setprecision(14);

				out << target_var;

				size_t last_not_zero_pos = 0;

				if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
				{
					out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
				}

				main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

				DWORD SelStart = 0;
				DWORD SelEnd = 0;
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

				return true;
			}
			else
			{
				target_var = temp_var;
				this->SetModifiedFlag(TRUE);
				return true;
			}
		}
		else if(ID == IDC_W_MIN_EDIT)
		{
			if(temp_var >= w_max)
			{
				MessageBeep(MB_ICONEXCLAMATION);

				ostringstream out;
				out << setiosflags(std::ios_base::fixed);
				out << setprecision(14);

				out << target_var;

				size_t last_not_zero_pos = 0;

				if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
				{
					out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
				}

				main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

				DWORD SelStart = 0;
				DWORD SelEnd = 0;
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

				return true;
			}
			else
			{
				target_var = temp_var;
				this->SetModifiedFlag(TRUE);
				return true;
			}
		}
		else if(ID == IDC_W_MAX_EDIT)
		{
			if(temp_var <= w_min)
			{
				MessageBeep(MB_ICONEXCLAMATION);

				ostringstream out;
				out << setiosflags(std::ios_base::fixed);
				out << setprecision(14);

				out << target_var;

				size_t last_not_zero_pos = 0;

				if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
				{
					out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
				}

				main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

				DWORD SelStart = 0;
				DWORD SelEnd = 0;
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
				main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

				return true;
			}
			else
			{
				target_var = temp_var;
				this->SetModifiedFlag(TRUE);
				return true;
			}
		}

	}
	else if(!is_real_number(buf.GetBuffer()))
	{
		MessageBeep(-1);

		ostringstream out;
		out << setiosflags(std::ios_base::fixed);
		out << setprecision(14);

		if(target_var == 0.0)
		{
			out << "0.0";
		}
		else
		{
			out << target_var;

			size_t last_not_zero_pos = 0;

			if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
			{
				out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
			}
		}

		main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

		DWORD SelStart = 0;
		DWORD SelEnd = 0;
		main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
		main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

		return true;
	}


	double temp_var = atof(buf.GetBuffer());


	if(ID == IDC_X_MIN_EDIT)
	{
		if(temp_var >= x_max)
		{
			MessageBeep(-1);

			ostringstream out;
			out << setiosflags(std::ios_base::fixed);
			out << setprecision(14);

			out << target_var;

			size_t last_not_zero_pos = 0;

			if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
			{
				out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
			}

			main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

			DWORD SelStart = 0;
			DWORD SelEnd = 0;
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

			return true;
		}
		else
		{
			target_var = temp_var;
			this->SetModifiedFlag(TRUE);
			return true;
		}
	}
	else if(ID == IDC_X_MAX_EDIT)
	{
		if(temp_var <= x_min)
		{
			MessageBeep(-1);

			ostringstream out;
			out << setiosflags(std::ios_base::fixed);
			out << setprecision(14);

			out << target_var;

			size_t last_not_zero_pos = 0;

			if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
			{
				out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
			}

			main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

			DWORD SelStart = 0;
			DWORD SelEnd = 0;
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

			return true;
		}
		else
		{
			target_var = temp_var;
			this->SetModifiedFlag(TRUE);
			return true;
		}
	}
	else if(ID == IDC_Y_MIN_EDIT)
	{
		if(temp_var >= y_max)
		{
			MessageBeep(-1);

			ostringstream out;
			out << setiosflags(std::ios_base::fixed);
			out << setprecision(14);

			out << target_var;

			size_t last_not_zero_pos = 0;

			if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
			{
				out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
			}

			main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

			DWORD SelStart = 0;
			DWORD SelEnd = 0;
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

			return true;
		}
		else
		{
			target_var = temp_var;
			this->SetModifiedFlag(TRUE);
			return true;
		}
	}
	else if(ID == IDC_Y_MAX_EDIT)
	{
		if(temp_var <= y_min)
		{
			MessageBeep(-1);

			ostringstream out;
			out << setiosflags(std::ios_base::fixed);
			out << setprecision(14);

			out << target_var;

			size_t last_not_zero_pos = 0;

			if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
			{
				out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
			}

			main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

			DWORD SelStart = 0;
			DWORD SelEnd = 0;
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

			return true;
		}
		else
		{
			target_var = temp_var;
			this->SetModifiedFlag(TRUE);
			return true;
		}
	}
	else if(ID == IDC_Z_MIN_EDIT)
	{
		if(temp_var >= z_max)
		{
			MessageBeep(-1);

			ostringstream out;
			out << setiosflags(std::ios_base::fixed);
			out << setprecision(14);

			out << target_var;

			size_t last_not_zero_pos = 0;

			if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
			{
				out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
			}

			main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

			DWORD SelStart = 0;
			DWORD SelEnd = 0;
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

			return true;
		}
		else
		{
			target_var = temp_var;
			this->SetModifiedFlag(TRUE);
			return true;
		}
	}
	else if(ID == IDC_Z_MAX_EDIT)
	{
		if(temp_var <= z_min)
		{
			MessageBeep(-1);

			ostringstream out;
			out << setiosflags(std::ios_base::fixed);
			out << setprecision(14);

			out << target_var;

			size_t last_not_zero_pos = 0;

			if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
			{
				out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
			}

			main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

			DWORD SelStart = 0;
			DWORD SelEnd = 0;
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

			return true;
		}
		else
		{
			target_var = temp_var;
			this->SetModifiedFlag(TRUE);
			return true;
		}
	}
	else if(ID == IDC_W_MIN_EDIT)
	{
		if(temp_var >= w_max)
		{
			MessageBeep(-1);

			ostringstream out;
			out << setiosflags(std::ios_base::fixed);
			out << setprecision(14);

			out << target_var;

			size_t last_not_zero_pos = 0;

			if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
			{
				out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
			}

			main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

			DWORD SelStart = 0;
			DWORD SelEnd = 0;
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

			return true;
		}
		else
		{
			target_var = temp_var;
			this->SetModifiedFlag(TRUE);
			return true;
		}
	}
	else if(ID == IDC_W_MAX_EDIT)
	{
		if(temp_var <= w_min)
		{
			MessageBeep(-1);

			ostringstream out;
			out << setiosflags(std::ios_base::fixed);
			out << setprecision(14);

			out << target_var;

			size_t last_not_zero_pos = 0;

			if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
			{
				out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
			}

			main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

			DWORD SelStart = 0;
			DWORD SelEnd = 0;
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
			main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

			return true;
		}
		else
		{
			target_var = temp_var;
			this->SetModifiedFlag(TRUE);
			return true;
		}
	}

	return true;
}



void Cjulia4d2Doc::ProcessFloatingPointMainSettingsEdit(const DWORD &ID, double &target_var)
{
	if(ProcessXYZWMinMaxEdits(ID, target_var))
		return;

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *main_settings_dialog = &parent_frame->m_wndMainSettingsDialog;

	CString buf;
	main_settings_dialog->GetDlgItemText(ID, buf);

	if(buf.IsEmpty() || buf == "-")
	{
		target_var = 0.0;
		this->SetModifiedFlag(TRUE);

		return;
	}
	else if(!is_real_number(buf.GetBuffer()))
	{
		MessageBeep(-1);

		ostringstream out;
		out << setiosflags(std::ios_base::fixed);
		out << setprecision(14);

		if(target_var == 0.0)
		{
			out << "0.0";
		}
		else
		{
			out << target_var;

			size_t last_not_zero_pos = 0;

			if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
			{
				out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
			}
		}

		main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

		DWORD SelStart = 0;
		DWORD SelEnd = 0;
		main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
		main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

		return;
	}

	target_var = atof(buf.GetBuffer());
	this->SetModifiedFlag(TRUE);
}



void Cjulia4d2Doc::ProcessFloatingPointMeshSettingsEdit(const DWORD &ID, double &target_var)
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *main_settings_dialog = &parent_frame->m_wndMainSettingsDialog;

	CString buf;
	main_settings_dialog->GetDlgItemText(ID, buf);

	if(buf.IsEmpty())
	{
		target_var = 0.0;
		this->SetModifiedFlag(TRUE);

		return;
	}
	else if(!is_real_number(buf.GetBuffer()))
	{
		MessageBeep(-1);

		ostringstream out;
		out << setiosflags(std::ios_base::fixed);
		out << setprecision(14);

		if(target_var == 0.0)
		{
			out << "0.0";
		}
		else
		{
			out << target_var;

			size_t last_not_zero_pos = 0;

			if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
			{
				out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
			}
		}

		main_settings_dialog->SetDlgItemText(ID, out.str().c_str());

		DWORD SelStart = 0;
		DWORD SelEnd = 0;
		main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
		main_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

		return;
	}

	target_var = atof(buf.GetBuffer());
	this->SetModifiedFlag(TRUE);
}


void Cjulia4d2Doc::ProcessFloatingPointMultiplicationSettingsEdit(const DWORD &ID, double &target_var)
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *mul_settings_dialog = &parent_frame->m_wndMultiplicationSettingsDialog;

	CString buf;
	mul_settings_dialog->GetDlgItemText(ID, buf);

	if(buf.IsEmpty())
	{
		target_var = 0.0;
		this->SetModifiedFlag(TRUE);

		return;
	}
	else if(!is_real_number(buf.GetBuffer()))
	{
		MessageBeep(-1);

		ostringstream out;
		out << setiosflags(std::ios_base::fixed);
		out << setprecision(14);

		if(target_var == 0.0)
		{
			out << "0.0";
		}
		else
		{
			out << target_var;

			size_t last_not_zero_pos = 0;

			if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0')))
			{
				out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0");
			}
		}

		mul_settings_dialog->SetDlgItemText(ID, out.str().c_str());

		DWORD SelStart = 0;
		DWORD SelEnd = 0;
		mul_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
		mul_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

		return;
	}

	target_var = atof(buf.GetBuffer());
	this->SetModifiedFlag(TRUE);
}















void Cjulia4d2Doc::OnBnClickedSwapZAndCCheck()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *extra_settings_dialog = &parent_frame->m_wndExtraSettingsDialog;

	// extra settings
	if(extra_settings_dialog->GetDlgItem(IDC_SWAP_Z_AND_C_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		swap_z_and_c = true;
	else
		swap_z_and_c = false;

	this->SetModifiedFlag(TRUE);

	parent_frame->OnBnClickedSwapZAndCCheck();
}

void Cjulia4d2Doc::OnBnClickedReverseSetCheck()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *extra_settings_dialog = &parent_frame->m_wndExtraSettingsDialog;

	// extra settings
	if(extra_settings_dialog->GetDlgItem(IDC_REVERSE_SET_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		reverse_set = true;
	else
		reverse_set = false;

	this->SetModifiedFlag(TRUE);

	parent_frame->OnBnClickedReverseSetCheck();
}

void Cjulia4d2Doc::OnBnClickedHopfMapCheck()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *extra_settings_dialog = &parent_frame->m_wndExtraSettingsDialog;

	// extra settings
	if(extra_settings_dialog->GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		hopf_map = true;
	else
		hopf_map = false;

	this->SetModifiedFlag(TRUE);

	parent_frame->OnBnClickedHopfMapCheck();
}

void Cjulia4d2Doc::OnBnClickedCullNonSurfacePointsCheck()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *extra_settings_dialog = &parent_frame->m_wndExtraSettingsDialog;

	// extra settings
	if(extra_settings_dialog->GetDlgItem(IDC_CULL_NON_SURFACE_POINTS_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		cull_non_surface_points = true;
	else
		cull_non_surface_points = false;

	this->SetModifiedFlag(TRUE);

	parent_frame->OnBnClickedCullNonSurfacePointsCheck();
}

void Cjulia4d2Doc::OnBnClickedScreenRadio()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *render_to_settings_dialog = &parent_frame->m_wndRenderToSettingsDialog;

	// RenderTo settings
	if(render_to_settings_dialog->GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_GETCHECK, 0, 0))
		render_to_type = POINT_SET;

	this->SetModifiedFlag(TRUE);

	parent_frame->OnBnClickedScreenRadio();
}

void Cjulia4d2Doc::OnBnClickedCsvFileRadio()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *render_to_settings_dialog = &parent_frame->m_wndRenderToSettingsDialog;

	// RenderTo settings
	if(render_to_settings_dialog->GetDlgItem(IDC_CSV_FILE_RADIO)->SendMessage(BM_GETCHECK, 0, 0))
		render_to_type = CSV_FILE;

	this->SetModifiedFlag(TRUE);

	parent_frame->OnBnClickedCsvFileRadio();
}

void Cjulia4d2Doc::OnBnClickedVoxelRadio()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *render_to_settings_dialog = &parent_frame->m_wndRenderToSettingsDialog;

	// RenderTo settings
	if(render_to_settings_dialog->GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_GETCHECK, 0, 0))
		render_to_type = VOXEL_FILE;

	this->SetModifiedFlag(TRUE);

	parent_frame->OnBnClickedVoxelRadio();
}

void Cjulia4d2Doc::OnBnClickedMeshRadio()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *render_to_settings_dialog = &parent_frame->m_wndRenderToSettingsDialog;

	// RenderTo settings
	if(render_to_settings_dialog->GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_GETCHECK, 0, 0))
		render_to_type = MESH_FILE;

	this->SetModifiedFlag(TRUE);

	parent_frame->OnBnClickedMeshRadio();
}










void Cjulia4d2Doc::OnBnClickedMulColumnDefaultVariablesRadio()
{
	mul_column_type = MUL_COLUMN_DEFAULT;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnBnClickedMulColumnDefaultVariablesRadio();
}

void Cjulia4d2Doc::OnBnClickedMulColumnConstantValuesRadio()
{
	mul_column_type = MUL_COLUMN_CONST_VALS;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnBnClickedMulColumnConstantValuesRadio();
}

void Cjulia4d2Doc::OnBnClickedMulColumnCustomVariablesRadio()
{
	mul_column_type = MUL_COLUMN_CUSTOM;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnBnClickedMulColumnCustomVariablesRadio();
}

void Cjulia4d2Doc::OnBnClickedMulMatrixDefaultVariablesRadio()
{
	mul_matrix_type = MUL_MATRIX_DEFAULT;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnBnClickedMulMatrixDefaultVariablesRadio();
}

void Cjulia4d2Doc::OnBnClickedMulMatrixCustomVariablesRadio()
{
	mul_matrix_type = MUL_MATRIX_CUSTOM;
	this->SetModifiedFlag(TRUE);

	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

	parent_frame->OnBnClickedMulMatrixCustomVariablesRadio();
}





void Cjulia4d2Doc::OnEnUpdateMulColumnConstantValuesXEdit() { ProcessFloatingPointMultiplicationSettingsEdit(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT, mul_column_const_vals_x); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulColumnConstantValuesXEdit();
}

void Cjulia4d2Doc::OnEnUpdateMulColumnConstantValuesYEdit() { ProcessFloatingPointMultiplicationSettingsEdit(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT, mul_column_const_vals_y); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulColumnConstantValuesYEdit();
}

void Cjulia4d2Doc::OnEnUpdateMulColumnConstantValuesZEdit() { ProcessFloatingPointMultiplicationSettingsEdit(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT, mul_column_const_vals_z); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulColumnConstantValuesZEdit();
}

void Cjulia4d2Doc::OnEnUpdateMulColumnConstantValuesWEdit() { ProcessFloatingPointMultiplicationSettingsEdit(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT, mul_column_const_vals_w); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulColumnConstantValuesWEdit();
}

void Cjulia4d2Doc::OnEnUpdateMulColumnCustomVariablesXEdit() { ProcessMulVarEdit(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT, mul_column_custom_x); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulColumnCustomVariablesXEdit();
}

void Cjulia4d2Doc::OnEnUpdateMulColumnCustomVariablesYEdit() { ProcessMulVarEdit(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT, mul_column_custom_y); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulColumnCustomVariablesYEdit();
}

void Cjulia4d2Doc::OnEnUpdateMulColumnCustomVariablesZEdit() { ProcessMulVarEdit(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT, mul_column_custom_z); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulColumnCustomVariablesZEdit();
}

void Cjulia4d2Doc::OnEnUpdateMulColumnCustomVariablesWEdit() { ProcessMulVarEdit(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT, mul_column_custom_w); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulColumnCustomVariablesWEdit();
}


void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom11Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_11_EDIT, mul_matrix_custom_11); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom11Edit();
}

void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom12Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_12_EDIT, mul_matrix_custom_12); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom12Edit();
}

void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom13Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_13_EDIT, mul_matrix_custom_13); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom13Edit();
}

void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom14Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_14_EDIT, mul_matrix_custom_14); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom14Edit();
}


void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom21Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_21_EDIT, mul_matrix_custom_21); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom21Edit();
}

void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom22Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_22_EDIT, mul_matrix_custom_22); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom22Edit();
}

void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom23Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_23_EDIT, mul_matrix_custom_23); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom23Edit();
}

void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom24Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_24_EDIT, mul_matrix_custom_24); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom24Edit();
}


void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom31Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_31_EDIT, mul_matrix_custom_31); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom31Edit();
}

void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom32Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_32_EDIT, mul_matrix_custom_32); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom32Edit();
}

void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom33Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_33_EDIT, mul_matrix_custom_33); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom33Edit();
}

void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom34Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_34_EDIT, mul_matrix_custom_34); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom34Edit();
}


void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom41Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_41_EDIT, mul_matrix_custom_41); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom41Edit();
}

void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom42Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_42_EDIT, mul_matrix_custom_42); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom42Edit();
}

void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom43Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_43_EDIT, mul_matrix_custom_43); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom43Edit();
}

void Cjulia4d2Doc::OnEnUpdateMulMatrixCustom44Edit() { ProcessMulVarEdit(IDC_MUL_MATRIX_CUSTOM_44_EDIT, mul_matrix_custom_44); 

POSITION pos = GetFirstViewPosition();
Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();

parent_frame->OnEnChangeMulMatrixCustom44Edit();
}








void Cjulia4d2Doc::ProcessMulVarEdit(const DWORD &ID, long unsigned int &target_var)
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *mul_settings_dialog = &parent_frame->m_wndMultiplicationSettingsDialog;

	CString buf;
	mul_settings_dialog->GetDlgItemText(ID, buf);

	if(buf.IsEmpty())
	{
		switch(ID)
		{
		case IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT:
			{
				target_var = MUL_PLUS_X;
				break;
			}

		case IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT:
			{
				target_var = MUL_PLUS_Y;
				break;
			}

		case IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT:
			{
				target_var = MUL_PLUS_Z;
				break;
			}

		case IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT:
			{
				target_var = MUL_PLUS_W;
				break;
			}

		case IDC_MUL_MATRIX_CUSTOM_11_EDIT:
			{	
				target_var = MUL_PLUS_X;
				break;
			}
		case IDC_MUL_MATRIX_CUSTOM_12_EDIT:
			{	
				target_var = MUL_MINUS_Y;
				break;
			}
		case IDC_MUL_MATRIX_CUSTOM_13_EDIT:
			{	
				target_var = MUL_MINUS_Z;
				break;
			}
		case IDC_MUL_MATRIX_CUSTOM_14_EDIT:
			{	
				target_var = MUL_MINUS_W;
				break;
			}

		case IDC_MUL_MATRIX_CUSTOM_21_EDIT:
			{	
				target_var = MUL_PLUS_Y;
				break;
			}
		case IDC_MUL_MATRIX_CUSTOM_22_EDIT:
			{	
				target_var = MUL_PLUS_X;
				break;
			}
		case IDC_MUL_MATRIX_CUSTOM_23_EDIT:
			{	
				target_var = MUL_PLUS_W;
				break;
			}
		case IDC_MUL_MATRIX_CUSTOM_24_EDIT:
			{	
				target_var = MUL_MINUS_Z;
				break;
			}

		case IDC_MUL_MATRIX_CUSTOM_31_EDIT:
			{	
				target_var = MUL_PLUS_Z;
				break;
			}
		case IDC_MUL_MATRIX_CUSTOM_32_EDIT:
			{	
				target_var = MUL_MINUS_W;
				break;
			}
		case IDC_MUL_MATRIX_CUSTOM_33_EDIT:
			{	
				target_var = MUL_PLUS_X;
				break;
			}
		case IDC_MUL_MATRIX_CUSTOM_34_EDIT:
			{	
				target_var = MUL_PLUS_Y;
				break;
			}

		case IDC_MUL_MATRIX_CUSTOM_41_EDIT:
			{	
				target_var = MUL_PLUS_W;
				break;
			}
		case IDC_MUL_MATRIX_CUSTOM_42_EDIT:
			{	
				target_var = MUL_PLUS_Z;
				break;
			}
		case IDC_MUL_MATRIX_CUSTOM_43_EDIT:
			{	
				target_var = MUL_MINUS_Y;
				break;
			}
		case IDC_MUL_MATRIX_CUSTOM_44_EDIT:
			{	
				target_var = MUL_PLUS_X;
				break;
			}

		default:
			target_var = -1;
			break;
		}

		this->SetModifiedFlag(TRUE);

		return;
	}
	else if(!is_valid_mul_variable(buf.GetBuffer()))
	{
		mul_settings_dialog->SetDlgItemText(ID, get_mul_variable_string(target_var).c_str());

		DWORD SelStart = 0;
		DWORD SelEnd = 0;
		mul_settings_dialog->GetDlgItem(ID)->SendMessage(EM_GETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);
		mul_settings_dialog->GetDlgItem(ID)->SendMessage(EM_SETSEL,(WPARAM)&SelStart,(LPARAM)&SelEnd);

		return;
	}

	target_var = get_mul_variable_type(buf.GetBuffer());

	this->SetModifiedFlag(TRUE);
}



void Cjulia4d2Doc::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	//	if(this->IsModified())
	pCmdUI->Enable(TRUE);
	//	else
	//		pCmdUI->Enable(FALSE);
}




void Cjulia4d2Doc::OnEnUpdateHopfFieldResolutionEdit()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *main_settings_dialog = &parent_frame->m_wndMainSettingsDialog;

	CString buf;
	main_settings_dialog->GetDlgItemText(IDC_HOPF_FIELD_RESOLUTION_EDIT, buf);

	double temp_double = atof(buf.GetBuffer());
/*
	if(temp_double < 3.0)
	{
		temp_double = 3.0;
		main_settings_dialog->SetDlgItemText(IDC_HOPF_FIELD_RESOLUTION_EDIT, "3");
	}
	else if(temp_double > 1625.0)
	{
		temp_double = 1625.0;
		main_settings_dialog->SetDlgItemText(IDC_HOPF_FIELD_RESOLUTION_EDIT, "1625");
	}
*/

	hopf_field_resolution = static_cast<long unsigned int>(temp_double);

	this->SetModifiedFlag(TRUE);

	parent_frame->OnEnChangeHopfFieldResolutionEdit();
}

void Cjulia4d2Doc::OnEnUpdateNormalQualityEdit()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *extra_settings_dialog = &parent_frame->m_wndExtraSettingsDialog;

	CString buf;
	extra_settings_dialog->GetDlgItemText(IDC_NORMAL_QUALITY_EDIT, buf);

	double temp_double = atof(buf.GetBuffer());

/*	if(temp_double < 1.0)
	{
		temp_double = 1.0;
		extra_settings_dialog->SetDlgItemText(IDC_NORMAL_QUALITY_EDIT, "1");
	}
	else if(temp_double > 1625.0)
	{
		temp_double = 1625.0;
		extra_settings_dialog->SetDlgItemText(IDC_NORMAL_QUALITY_EDIT, "1625");
	}
*/
	normal_gen_radius = static_cast<long unsigned int>(temp_double);
}

void Cjulia4d2Doc::OnBnClickedPropResCheck()
{
	POSITION pos = GetFirstViewPosition();
	Cjulia4d2View *parent_view = (Cjulia4d2View *)GetNextView( pos );
	CMainFrame *parent_frame = (CMainFrame *)parent_view->GetParentFrame();
	CDialogBar *main_settings_dialog = &parent_frame->m_wndMainSettingsDialog;

	// main settings
	if(main_settings_dialog->GetDlgItem(IDC_PROP_RES_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		proportional_res = true;
	else
		proportional_res = false;

	this->SetModifiedFlag(TRUE);

	parent_frame->OnBnClickedPropResCheck();
}
