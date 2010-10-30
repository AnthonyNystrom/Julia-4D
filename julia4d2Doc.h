// julia4d2Doc.h : interface of the Cjulia4d2Doc class
//
#ifndef JULIA4D2_DOC_H
#define JULIA4D2_DOC_H






#include <string>
using std::string;

#include "julia4d2View.h"


class Cjulia4d2View;


class Cjulia4d2Doc : public CDocument
{
public: // create from serialization only
	Cjulia4d2Doc();
	DECLARE_DYNCREATE(Cjulia4d2Doc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~Cjulia4d2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	bool operator==(const Cjulia4d2Doc &rhs);
	void operator=(const Cjulia4d2Doc &rhs);

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	void ResetVars(void);
	void SetParentVars(void);
	bool ProcessXYZWMinMaxEdits(const DWORD &ID, double &target_var);
	void ProcessFloatingPointMainSettingsEdit(const DWORD &ID, double &target_var);
	void ProcessFloatingPointMeshSettingsEdit(const DWORD &ID, double &target_var);
	void ProcessFloatingPointMultiplicationSettingsEdit(const DWORD &ID, double &target_var);


public:

	long unsigned int iterations;
	double infinity;
	long unsigned int resolution;
	long unsigned int formula;
	string formula_text;

	double c_x;
	double c_y;
	double c_z;
	double c_w;
	double z_w;
	double a;

	long unsigned int mul_column_type;

	double mul_column_const_vals_x;
	double mul_column_const_vals_y;
	double mul_column_const_vals_z;
	double mul_column_const_vals_w;

	long unsigned int mul_column_custom_x;
	long unsigned int mul_column_custom_y;
	long unsigned int mul_column_custom_z;
	long unsigned int mul_column_custom_w;

	long unsigned int mul_matrix_type;

	long unsigned int mul_matrix_custom_11;
	long unsigned int mul_matrix_custom_12;
	long unsigned int mul_matrix_custom_13;
	long unsigned int mul_matrix_custom_14;

	long unsigned int mul_matrix_custom_21;
	long unsigned int mul_matrix_custom_22;
	long unsigned int mul_matrix_custom_23;
	long unsigned int mul_matrix_custom_24;

	long unsigned int mul_matrix_custom_31;
	long unsigned int mul_matrix_custom_32;
	long unsigned int mul_matrix_custom_33;
	long unsigned int mul_matrix_custom_34;

	long unsigned int mul_matrix_custom_41;
	long unsigned int mul_matrix_custom_42;
	long unsigned int mul_matrix_custom_43;
	long unsigned int mul_matrix_custom_44;

	double x_min;
	double x_max;
	double y_min;
	double y_max;
	double z_min;
	double z_max;
	double w_min;
	double w_max;

	bool hopf_map;
	bool swap_z_and_c;
	bool reverse_set;
	bool cull_non_surface_points;

	long unsigned int samples;


	long unsigned int render_to_type;

	CString export_file_name;

//	bool light_by_normals;

	long unsigned int hopf_field_resolution;

	long unsigned int normal_gen_radius;

	bool proportional_res;

	// NOTE: THESE ARE NOT USED ANYWHERE *BUT* SET GENERATION!!!
	// NOTE: DO NOT USE THESE INDIVIDUALLY WHEN SAVING OR CACHING!!!
	long unsigned int x_resolution;
	long unsigned int y_resolution;
	long unsigned int z_resolution;
	long unsigned int w_resolution;

public:

	bool write_ident_arcball;
	bool set_parent_vars;

	afx_msg void OnEnUpdateCXEdit();
	afx_msg void OnEnUpdateCYEdit(void);
	afx_msg void OnEnUpdateCZEdit(void);
	afx_msg void OnEnUpdateCWEdit(void);

	afx_msg void OnEnUpdateXMinEdit(void);
	afx_msg void OnEnUpdateXMaxEdit(void);
	afx_msg void OnEnUpdateYMinEdit(void);
	afx_msg void OnEnUpdateYMaxEdit(void);
	afx_msg void OnEnUpdateZMinEdit(void);
	afx_msg void OnEnUpdateZMaxEdit(void);
	afx_msg void OnEnUpdateWMinEdit(void);
	afx_msg void OnEnUpdateWMaxEdit(void);

	afx_msg void OnEnUpdateZWEdit(void);
	afx_msg void OnEnUpdateAEdit(void);
	afx_msg void OnEnUpdateInfinityEdit(void);

	afx_msg void OnEnUpdateFormulaEdit(void);
	afx_msg void OnEnUpdateIterationsEdit(void);
	afx_msg void OnEnUpdateSamplesEdit(void);


	afx_msg void OnFormulaCustom();

	afx_msg void OnGeneralClassic();
	afx_msg void OnGeneralExponential();
	afx_msg void OnGeneralQuadratic();
	afx_msg void OnGeneralLambda();
	afx_msg void OnGeneralFunkyBlobs();

	afx_msg void OnSineSine();
	afx_msg void OnSineSinePlus();
	afx_msg void OnSineSineClassic();

	afx_msg void OnCosineCosine();
	afx_msg void OnCosineCosinePlus();
	afx_msg void OnCosineCosineClassic();

	afx_msg void OnTangentTangent();
	afx_msg void OnTangentTangentPlus();
	afx_msg void OnTangentTangentClassic();

	afx_msg void OnSecantSecant();
	afx_msg void OnSecantSecantPlus();
	afx_msg void OnSecantSecantClassic();

	afx_msg void OnCosecantCosecant();
	afx_msg void OnCosecantCosecantPlus();
	afx_msg void OnCosecantCosecantClassic();

	afx_msg void OnCotangentCotangent();
	afx_msg void OnCotangentCotangentPlus();
	afx_msg void OnCotangentCotangentClassic();

	afx_msg void OnHyperbolicSineHyperbolicSine();
	afx_msg void OnHyperbolicSineHyperbolicSinePlus();
	afx_msg void OnHyperbolicSineHyperbolicSineClassic();

	afx_msg void OnHyperbolicCosineHyperbolicCosine();
	afx_msg void OnHyperbolicCosineHyperbolicCosinePlus();
	afx_msg void OnHyperbolicCosineHyperbolicCosineClassic();

	afx_msg void OnHyperbolicTangentHyperbolicTangent();
	afx_msg void OnHyperbolicTangentHyperbolicTangentPlus();
	afx_msg void OnHyperbolicTangentHyperbolicTangentClassic();

	afx_msg void OnHyperbolicSecantHyperbolicSecant();
	afx_msg void OnHyperbolicSecantHyperbolicSecantPlus();
	afx_msg void OnHyperbolicSecantHyperbolicSecantClassic();

	afx_msg void OnHyperbolicCosecantHyperbolicCosecant();
	afx_msg void OnHyperbolicCosecantHyperbolicCosecantPlus();
	afx_msg void OnHyperbolicCosecantHyperbolicCosecantClassic();

	afx_msg void OnHyperbolicCotangentHyperbolicCotangent();
	afx_msg void OnHyperbolicCotangentHyperbolicCotangentPlus();
	afx_msg void OnHyperbolicCotangentHyperbolicCotangentClassic();

	afx_msg void OnBnClickedSwapZAndCCheck();
	afx_msg void OnBnClickedReverseSetCheck();
	afx_msg void OnBnClickedHopfMapCheck();
	afx_msg void OnBnClickedCullNonSurfacePointsCheck();

	afx_msg void OnBnClickedScreenRadio();
	afx_msg void OnBnClickedCsvFileRadio();
	afx_msg void OnBnClickedVoxelRadio();
	afx_msg void OnBnClickedMeshRadio();




	afx_msg void OnBnClickedMulColumnDefaultVariablesRadio();
	afx_msg void OnBnClickedMulColumnConstantValuesRadio();
	afx_msg void OnBnClickedMulColumnCustomVariablesRadio();
	afx_msg void OnBnClickedMulMatrixDefaultVariablesRadio();
	afx_msg void OnBnClickedMulMatrixCustomVariablesRadio();

	afx_msg void OnEnUpdateMulColumnConstantValuesXEdit();
	afx_msg void OnEnUpdateMulColumnConstantValuesYEdit();
	afx_msg void OnEnUpdateMulColumnConstantValuesZEdit();
	afx_msg void OnEnUpdateMulColumnConstantValuesWEdit();

	afx_msg void OnEnUpdateMulColumnCustomVariablesXEdit();
	afx_msg void OnEnUpdateMulColumnCustomVariablesYEdit();
	afx_msg void OnEnUpdateMulColumnCustomVariablesZEdit();
	afx_msg void OnEnUpdateMulColumnCustomVariablesWEdit();

	afx_msg void OnEnUpdateMulMatrixCustom11Edit();
	afx_msg void OnEnUpdateMulMatrixCustom12Edit();
	afx_msg void OnEnUpdateMulMatrixCustom13Edit();
	afx_msg void OnEnUpdateMulMatrixCustom14Edit();

	afx_msg void OnEnUpdateMulMatrixCustom21Edit();
	afx_msg void OnEnUpdateMulMatrixCustom22Edit();
	afx_msg void OnEnUpdateMulMatrixCustom23Edit();
	afx_msg void OnEnUpdateMulMatrixCustom24Edit();

	afx_msg void OnEnUpdateMulMatrixCustom31Edit();
	afx_msg void OnEnUpdateMulMatrixCustom32Edit();
	afx_msg void OnEnUpdateMulMatrixCustom33Edit();
	afx_msg void OnEnUpdateMulMatrixCustom34Edit();

	afx_msg void OnEnUpdateMulMatrixCustom41Edit();
	afx_msg void OnEnUpdateMulMatrixCustom42Edit();
	afx_msg void OnEnUpdateMulMatrixCustom43Edit();
	afx_msg void OnEnUpdateMulMatrixCustom44Edit();


	void ProcessMulVarEdit(const DWORD &ID, long unsigned int &target_var);

	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnEnUpdateResolutionEdit();
	afx_msg void OnEnUpdateHopfFieldResolutionEdit();
	afx_msg void OnEnUpdateNormalQualityEdit();
	afx_msg void OnBnClickedPropResCheck();
};


#endif

