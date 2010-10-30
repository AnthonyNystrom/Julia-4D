#include "stdafx.h"

#include "cache_settings_dialog.h"
#include "defines.h"
#include "utilities.h"


#include <sstream>
#include <string>
#include <iomanip>
using std::ostringstream;
using std::istringstream;
using std::string;
using std::setprecision;

#include "disk_settings_dialog.h"


#include ".\cache_settings_dialog.h"







CCacheSettingsDlg::CCacheSettingsDlg(CMainFrame *src_parent_frame) : CDialog(CCacheSettingsDlg::IDD)
{
	parent_frame = src_parent_frame;
	clearing_lists = false;
	skip_warn = false;
}

void CCacheSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BOOL CCacheSettingsDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog())
		return FALSE;

	FillListsFromCache();


	if(1 == AfxGetApp()->GetProfileInt("Settings", "AutoGenCachedSetsOnDblClick", 0))
		GetDlgItem(IDC_AUTO_GENCHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);

	if(1 == AfxGetApp()->GetProfileInt("Settings", "AutoCloseCacheSettingsOnDblClick", 0))
		GetDlgItem(IDC_AUTO_CLOSE_CACHE_SETTINGS_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);

	if(1 == AfxGetApp()->GetProfileInt("Settings", "WarnOnHistoryFileDelete", 1))
		GetDlgItem(IDC_DELETE_FILE_ITEM_WARN_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);

	if(1 == AfxGetApp()->GetProfileInt("Settings", "WarnOnHistoryResolutionDelete", 1))
		GetDlgItem(IDC_DELETE_RES_GROUP_WARN_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);

	if(1 == AfxGetApp()->GetProfileInt("Settings", "WarnOnHistoryFormulaDelete", 1))
		GetDlgItem(IDC_DELETE_FORMULA_GROUP_WARN_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);



	return TRUE;
}


void CCacheSettingsDlg::ClearLists(void)
{
	clearing_lists = true;

	GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_SETCURSEL, -1, 0);

	while(1)
	{
		if(CB_ERR == GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_DELETESTRING, 0, 0))
			break;
	}

	GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_SETCURSEL, -1, 0);

	while(1)
	{
		if(CB_ERR == GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_DELETESTRING, 0, 0))
			break;
	}

	GetDlgItem(IDC_FILE_LIST)->SendMessage(LB_SETCURSEL, -1, 0);

	while(1)
	{
		if(LB_ERR == GetDlgItem(IDC_FILE_LIST)->SendMessage(LB_DELETESTRING, 0, 0))
			break;
	}

	h_list.Clear();

	GetDlgItem(IDC_CACHED_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);

	clearing_lists = false;
}

void CCacheSettingsDlg::FillListsFromCache(void)
{
	ClearLists();

	wstring cache_path = get_module_path_wide();

	if(cache_path != L"")
	{
		cache_path += L"\\";
		cache_path += J4D2_CACHE_FOLDER_NAME;
	}

	if(cache_path != L"")
	{
		settings_file_info_collection sfic(cache_path);

		for(multiset<file_info>::iterator i = sfic.file_list.begin(); i != sfic.file_list.end(); i++)
			h_list.AddFile(i->file_name, i->creation_time);

		if(h_list.formulas.size() == 0)
		{
			GetDlgItem(IDC_FORMULA_GROUP_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_FILE_LIST)->EnableWindow(FALSE);

			GetDlgItem(IDC_DELETE_IDENTIFIER_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_DELETE_RES_GROUP_BUTTON)->EnableWindow(FALSE);
			GetDlgItem(IDC_DELETE_FORMULA_GROUP_BUTTON)->EnableWindow(FALSE);

			GetDlgItem(IDC_REVERSE_SET_CHECK)->EnableWindow(FALSE);
			GetDlgItem(IDC_CULL_CHECK)->EnableWindow(FALSE);
		}
		else
		{
			for(set<history_formula_group>::const_iterator i = h_list.formulas.begin(); i != h_list.formulas.end(); i++)
				GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_ADDSTRING, 0, (LPARAM)wsts(i->formula_text).c_str());

			GetDlgItem(IDC_FORMULA_GROUP_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_FILE_LIST)->EnableWindow(TRUE);

			GetDlgItem(IDC_DELETE_IDENTIFIER_BUTTON)->EnableWindow(TRUE);
			GetDlgItem(IDC_DELETE_RES_GROUP_BUTTON)->EnableWindow(TRUE);
			GetDlgItem(IDC_DELETE_FORMULA_GROUP_BUTTON)->EnableWindow(TRUE);

			GetDlgItem(IDC_REVERSE_SET_CHECK)->EnableWindow(TRUE);
			GetDlgItem(IDC_CULL_CHECK)->EnableWindow(TRUE);

			GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_SETCURSEL, 0, 0);
			OnCbnSelchangeFormulaGroupCombo();
		}
	}
}






BEGIN_MESSAGE_MAP(CCacheSettingsDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_FORMULA_GROUP_COMBO, OnCbnSelchangeFormulaGroupCombo)
	ON_CBN_SELCHANGE(IDC_RESOLUTION_GROUP_COMBO, OnCbnSelchangeResolutionGroupCombo)
	ON_LBN_SELCHANGE(IDC_FILE_LIST, OnLbnSelchangeFileList)
	ON_LBN_DBLCLK(IDC_FILE_LIST, OnLbnDblclkFileList)
	ON_BN_CLICKED(IDC_DELETE_IDENTIFIER_BUTTON, OnBnClickedDeleteIdentifierButton)
	ON_BN_CLICKED(IDC_DELETE_RES_GROUP_BUTTON, OnBnClickedDeleteResGroupButton)
	ON_BN_CLICKED(IDC_DELETE_FORMULA_GROUP_BUTTON, OnBnClickedDeleteFormulaGroupButton)
	ON_BN_CLICKED(IDC_AUTO_GENCHECK, OnBnClickedAutoGencheck)
	ON_BN_CLICKED(IDC_AUTO_CLOSE_CACHE_SETTINGS_CHECK, OnBnClickedAutoCloseCacheSettingsCheck)
	ON_BN_CLICKED(IDC_DISK_SETTINGS_BUTTON, OnBnClickedDiskSettingsButton)
	ON_BN_CLICKED(IDC_DELETE_FILE_ITEM_WARN_CHECK, OnBnClickedDeleteFileItemWarnCheck)
	ON_BN_CLICKED(IDC_DELETE_RES_GROUP_WARN_CHECK, OnBnClickedDeleteResGroupWarnCheck)
	ON_BN_CLICKED(IDC_DELETE_FORMULA_GROUP_WARN_CHECK, OnBnClickedDeleteFormulaGroupWarnCheck)
END_MESSAGE_MAP()





void CCacheSettingsDlg::SetVars(Cjulia4d2Doc &doc)
{
	ostringstream out;
	size_t last_not_zero_pos = 0;
	out << setiosflags(std::ios_base::fixed);
	out << setprecision(14);

	if(doc.swap_z_and_c)
		GetDlgItem(IDC_SWAP_Z_AND_C_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED);
	else
		GetDlgItem(IDC_SWAP_Z_AND_C_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

	if(doc.hopf_map)
	{
		GetDlgItem(IDC_W_MIN_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_W_MAX_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_Z_W_EDIT)->EnableWindow(FALSE);

//		GetDlgItem(IDC_CULL_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
		GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED);
	}
	else
	{
		GetDlgItem(IDC_W_MIN_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_W_MAX_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_Z_W_EDIT)->EnableWindow(TRUE);

		GetDlgItem(IDC_HOPF_MAP_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
	}


	if(doc.proportional_res)
	{
		GetDlgItem(IDC_PROP_RES_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED);
	}
	else
	{
		GetDlgItem(IDC_PROP_RES_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
	}





	//mesh - done
	out.str("");
	out << doc.samples;
	SetDlgItemText(IDC_SAMPLES_EDIT, out.str().c_str());
	out.str("");






	//mul - done
	switch(doc.mul_column_type)
	{
	case MUL_COLUMN_DEFAULT:
		{
			GetDlgItem(IDC_MUL_COLUMN_DEFAULT_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);


			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT)->EnableWindow(FALSE);

			break;
		}
	case MUL_COLUMN_CONST_VALS:
		{
			GetDlgItem(IDC_MUL_COLUMN_DEFAULT_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT)->EnableWindow(TRUE);

			break;
		}
	case MUL_COLUMN_CUSTOM:
		{
			GetDlgItem(IDC_MUL_COLUMN_DEFAULT_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);

			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT)->EnableWindow(FALSE);

			break;
		}
	}

	out.str("");

	out << doc.mul_column_const_vals_x;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_MUL_COLUMN_CONSTANT_VALUES_X_EDIT, out.str().c_str());
	out.str("");

	out << doc.mul_column_const_vals_y;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_MUL_COLUMN_CONSTANT_VALUES_Y_EDIT, out.str().c_str());
	out.str("");

	out << doc.mul_column_const_vals_z;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_MUL_COLUMN_CONSTANT_VALUES_Z_EDIT, out.str().c_str());
	out.str("");

	out << doc.mul_column_const_vals_w;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_MUL_COLUMN_CONSTANT_VALUES_W_EDIT, out.str().c_str());
	out.str("");

	SetDlgItemText(IDC_MUL_COLUMN_CUSTOM_VARIABLES_X_EDIT, get_mul_variable_string(doc.mul_column_custom_x).c_str());
	SetDlgItemText(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Y_EDIT, get_mul_variable_string(doc.mul_column_custom_y).c_str());
	SetDlgItemText(IDC_MUL_COLUMN_CUSTOM_VARIABLES_Z_EDIT, get_mul_variable_string(doc.mul_column_custom_z).c_str());
	SetDlgItemText(IDC_MUL_COLUMN_CUSTOM_VARIABLES_W_EDIT, get_mul_variable_string(doc.mul_column_custom_w).c_str());


	switch(doc.mul_matrix_type)
	{
	case MUL_MATRIX_DEFAULT:
		{
			GetDlgItem(IDC_MUL_MATRIX_DEFAULT_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_11_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_12_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_13_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_14_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_21_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_22_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_23_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_24_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_31_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_32_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_33_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_34_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_41_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_42_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_43_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_44_EDIT)->EnableWindow(FALSE);
			break;
		}
	case MUL_MATRIX_CUSTOM:
		{
			GetDlgItem(IDC_MUL_MATRIX_DEFAULT_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_VARIABLES_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);

			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_11_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_12_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_13_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_14_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_21_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_22_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_23_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_24_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_31_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_32_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_33_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_34_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_41_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_42_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_43_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_MUL_MATRIX_CUSTOM_44_EDIT)->EnableWindow(TRUE);
			break;
		}
	}

	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_11_EDIT, get_mul_variable_string(doc.mul_matrix_custom_11).c_str());
	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_12_EDIT, get_mul_variable_string(doc.mul_matrix_custom_12).c_str());
	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_13_EDIT, get_mul_variable_string(doc.mul_matrix_custom_13).c_str());
	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_14_EDIT, get_mul_variable_string(doc.mul_matrix_custom_14).c_str());

	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_21_EDIT, get_mul_variable_string(doc.mul_matrix_custom_21).c_str());
	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_22_EDIT, get_mul_variable_string(doc.mul_matrix_custom_22).c_str());
	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_23_EDIT, get_mul_variable_string(doc.mul_matrix_custom_23).c_str());
	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_24_EDIT, get_mul_variable_string(doc.mul_matrix_custom_24).c_str());

	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_31_EDIT, get_mul_variable_string(doc.mul_matrix_custom_31).c_str());
	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_32_EDIT, get_mul_variable_string(doc.mul_matrix_custom_32).c_str());
	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_33_EDIT, get_mul_variable_string(doc.mul_matrix_custom_33).c_str());
	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_34_EDIT, get_mul_variable_string(doc.mul_matrix_custom_34).c_str());

	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_41_EDIT, get_mul_variable_string(doc.mul_matrix_custom_41).c_str());
	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_42_EDIT, get_mul_variable_string(doc.mul_matrix_custom_42).c_str());
	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_43_EDIT, get_mul_variable_string(doc.mul_matrix_custom_43).c_str());
	SetDlgItemText(IDC_MUL_MATRIX_CUSTOM_44_EDIT, get_mul_variable_string(doc.mul_matrix_custom_44).c_str());







	//renderto - done
	switch(doc.render_to_type)
	{
	case POINT_SET:
		{
			GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
			GetDlgItem(IDC_CSV_FILE_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

			GetDlgItem(IDC_SAMPLES_EDIT)->EnableWindow(FALSE);
			break;
		}
	case CSV_FILE:
		{
			GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_CSV_FILE_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
			GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

			GetDlgItem(IDC_SAMPLES_EDIT)->EnableWindow(FALSE);
			break;
		}
	case VOXEL_FILE:
		{
			GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_CSV_FILE_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);
			GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);

			GetDlgItem(IDC_SAMPLES_EDIT)->EnableWindow(FALSE);
			break;
		}
	case MESH_FILE:
		{
			GetDlgItem(IDC_SCREEN_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_CSV_FILE_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_VOXEL_RADIO)->SendMessage(BM_SETCHECK, BST_UNCHECKED);
			GetDlgItem(IDC_MESH_RADIO)->SendMessage(BM_SETCHECK, BST_CHECKED);

			GetDlgItem(IDC_SAMPLES_EDIT)->EnableWindow(TRUE);
			break;
		}
	}






	SetDlgItemText(IDC_FORMULA_EDIT, doc.formula_text.c_str());




	out.str("");

	out << doc.iterations;
	SetDlgItemText(IDC_ITERATIONS_EDIT, out.str().c_str());
	out.str("");


	out << doc.infinity;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_INFINITY_EDIT, out.str().c_str());
	out.str("");


	out << doc.resolution;
	SetDlgItemText(IDC_RESOLUTION_EDIT, out.str().c_str());
	out.str("");



	out << doc.c_x;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_C_X_EDIT, out.str().c_str());
	out.str("");

	out << doc.c_y;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_C_Y_EDIT, out.str().c_str());
	out.str("");

	out << doc.c_z;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_C_Z_EDIT, out.str().c_str());
	out.str("");

	out << doc.c_w;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_C_W_EDIT, out.str().c_str());
	out.str("");

	out << doc.z_w;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_Z_W_EDIT, out.str().c_str());
	out.str("");

	out << doc.a;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_A_EDIT, out.str().c_str());
	out.str("");



	out << doc.x_min;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_X_MIN_EDIT, out.str().c_str());
	out.str("");

	out << doc.x_max;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_X_MAX_EDIT, out.str().c_str());
	out.str("");

	out << doc.y_min;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_Y_MIN_EDIT, out.str().c_str());
	out.str("");

	out << doc.y_max;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_Y_MAX_EDIT, out.str().c_str());
	out.str("");

	out << doc.z_min;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_Z_MIN_EDIT, out.str().c_str());
	out.str("");

	out << doc.z_max;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_Z_MAX_EDIT, out.str().c_str());
	out.str("");

	out << doc.w_min;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_W_MIN_EDIT, out.str().c_str());
	out.str("");

	out << doc.w_max;
	if(string::npos != (last_not_zero_pos = out.str().find_last_not_of('0'))) { out.str(out.str().substr(0, last_not_zero_pos + 1)); if(out.str().length() > 0) if(out.str()[out.str().length() - 1] == '.') out.str(out.str() + "0"); }

	SetDlgItemText(IDC_W_MAX_EDIT, out.str().c_str());
	out.str("");
}

void CCacheSettingsDlg::OnCbnSelchangeFormulaGroupCombo()
{
	if(clearing_lists)
		return;

	while(1)
	{
		if(CB_ERR == GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_DELETESTRING, 0, 0))
			break;
	}

	while(1)
	{
		if(LB_ERR == GetDlgItem(IDC_FILE_LIST)->SendMessage(LB_DELETESTRING, 0, 0))
			break;
	}


	LRESULT formula_sel = GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(CB_ERR == formula_sel)
		return;

	size_t iter_count = 0;

    for(set<history_formula_group>::const_iterator i = h_list.formulas.begin(); i != h_list.formulas.end(); i++)
	{
		if(iter_count == formula_sel)
		{
			for(set<history_resolution_group>::const_iterator resi = i->resolutions.begin(); resi != i->resolutions.end(); resi++)
			{
				ostringstream out;
				out << resi->resolution;

				GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_ADDSTRING, 0, (LPARAM) out.str().c_str());
			}

			break;
		}

		iter_count++;
	}


	GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_SETCURSEL, 0, 0);
	OnCbnSelchangeResolutionGroupCombo();
}

void CCacheSettingsDlg::OnCbnSelchangeResolutionGroupCombo()
{
	if(clearing_lists)
		return;

	while(1)
	{
		if(LB_ERR == GetDlgItem(IDC_FILE_LIST)->SendMessage(LB_DELETESTRING, 0, 0))
			break;
	}

	LRESULT formula_sel = GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(CB_ERR == formula_sel)
		return;

	LRESULT res_sel = GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(CB_ERR == res_sel)
		return;


	size_t formula_iter_count = 0;

    for(set<history_formula_group>::const_iterator fi = h_list.formulas.begin(); fi != h_list.formulas.end(); fi++)
	{
		if(formula_iter_count == formula_sel)
		{
			size_t resolution_iter_count = 0;

			for(set<history_resolution_group>::const_iterator resi = fi->resolutions.begin(); resi != fi->resolutions.end(); resi++)
			{
				if(resolution_iter_count == res_sel)
				{
					for(multiset<history_file_info>::const_iterator hfii = resi->cache_files.begin(); hfii != resi->cache_files.end(); hfii++)
						GetDlgItem(IDC_FILE_LIST)->SendMessage(LB_ADDSTRING, 0, (LPARAM) wsts(hfii->MD5_hash_string).c_str());

					break;
				}

				resolution_iter_count++;
			}

			break;
		}

		formula_iter_count++;
	}


	GetDlgItem(IDC_FILE_LIST)->SendMessage(LB_SETCURSEL, 0, 0);
	OnLbnSelchangeFileList();
}




void CCacheSettingsDlg::OnLbnSelchangeFileList()
{
	if(clearing_lists)
		return;

	LRESULT formula_sel = GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(CB_ERR == formula_sel)
		return;

	LRESULT res_sel = GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(CB_ERR == res_sel)
		return;

	LRESULT file_list_sel = GetDlgItem(IDC_FILE_LIST)->SendMessage(LB_GETCURSEL, 0, 0);

	if(LB_ERR == file_list_sel)
		return;

	size_t formula_iter_count = 0;

    for(set<history_formula_group>::const_iterator fi = h_list.formulas.begin(); fi != h_list.formulas.end(); fi++)
	{
		if(formula_iter_count == formula_sel)
		{
			size_t resolution_iter_count = 0;

			for(set<history_resolution_group>::const_iterator resi = fi->resolutions.begin(); resi != fi->resolutions.end(); resi++)
			{
				if(resolution_iter_count == res_sel)
				{
					size_t file_list_iter_count = 0;

					for(multiset<history_file_info>::const_iterator hfii = resi->cache_files.begin(); hfii != resi->cache_files.end(); hfii++)
					{
						if(file_list_iter_count == file_list_sel)
						{
							Cjulia4d2Doc temp_params;
							temp_params.set_parent_vars = false;
							if(temp_params.OnOpenDocument(wsts(hfii->file_name).c_str()))
							{
								SetVars(temp_params);

								if(/*temp_params.hopf_map ||*/ temp_params.render_to_type == MESH_FILE)
									GetDlgItem(IDC_CULL_CHECK)->EnableWindow(FALSE);
								else
									GetDlgItem(IDC_CULL_CHECK)->EnableWindow(TRUE);

								if(hfii->cached)
									GetDlgItem(IDC_CACHED_CHECK)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
								else
									GetDlgItem(IDC_CACHED_CHECK)->SendMessage(BM_SETCHECK, BST_UNCHECKED, 0);
							}

							break;
						}

						file_list_iter_count++;
					}

					break;
				}

				resolution_iter_count++;
			}

			break;
		}

		formula_iter_count++;
	}
	
}


void CCacheSettingsDlg::OnLbnDblclkFileList()
{
	if(clearing_lists)
		return;

	LRESULT formula_sel = GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(CB_ERR == formula_sel)
		return;

	LRESULT res_sel = GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(CB_ERR == res_sel)
		return;

	LRESULT file_list_sel = GetDlgItem(IDC_FILE_LIST)->SendMessage(LB_GETCURSEL, 0, 0);

	if(LB_ERR == file_list_sel)
		return;

	size_t formula_iter_count = 0;

    for(set<history_formula_group>::const_iterator fi = h_list.formulas.begin(); fi != h_list.formulas.end(); fi++)
	{
		if(formula_iter_count == formula_sel)
		{
			size_t resolution_iter_count = 0;

			for(set<history_resolution_group>::const_iterator resi = fi->resolutions.begin(); resi != fi->resolutions.end(); resi++)
			{
				if(resolution_iter_count == res_sel)
				{
					size_t file_list_iter_count = 0;

					for(multiset<history_file_info>::const_iterator hfii = resi->cache_files.begin(); hfii != resi->cache_files.end(); hfii++)
					{
						if(file_list_iter_count == file_list_sel)
						{
							Cjulia4d2Doc *ActiveDoc = (Cjulia4d2Doc *)parent_frame->GetActiveDocument();
							arc_ball temp_arcball = parent_frame->GetArcBall();

							// does this really help?
							ActiveDoc->set_parent_vars = true;

							if(ActiveDoc->SaveModified())
							{
								if(ActiveDoc->OnOpenDocument(wsts(hfii->file_name).c_str()))
								{
									if(BST_CHECKED == GetDlgItem(IDC_REVERSE_SET_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
										ActiveDoc->reverse_set = true;

									if(BST_CHECKED == GetDlgItem(IDC_CULL_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
										ActiveDoc->cull_non_surface_points = true;

									parent_frame->SetVars(*ActiveDoc);

									parent_frame->SetArcBall(temp_arcball);

									ActiveDoc->SetModifiedFlag(FALSE);


									if(1 == AfxGetApp()->GetProfileInt("Settings", "AutoGenCachedSetsOnDblClick", 0))
									{
										if(/*hfii->cached && */!parent_frame->generating_set)
											parent_frame->OnBnClickedGenerate();
									}
									
									if(1 == AfxGetApp()->GetProfileInt("Settings", "AutoCloseCacheSettingsOnDblClick", 0))
									{
										OnOK();
									}
								}
							}

							break;
						}

						file_list_iter_count++;
					}

					break;
				}

				resolution_iter_count++;
			}

			break;
		}

		formula_iter_count++;
	}
}


void CCacheSettingsDlg::OnBnClickedDeleteIdentifierButton()
{
	if(BST_CHECKED == GetDlgItem(IDC_DELETE_FILE_ITEM_WARN_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
	{
		int ret = 0;

		MessageBeep(MB_ICONINFORMATION);
		ret = MessageBox("Are you sure you want to delete this Identifier's history and cache files?", APP_NAME, MB_YESNO|MB_ICONQUESTION);

		if(IDNO == ret)
			return;
	}


	LRESULT formula_sel = GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(CB_ERR == formula_sel)
		return;

	LRESULT res_sel = GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(CB_ERR == res_sel)
		return;

	LRESULT file_list_sel = GetDlgItem(IDC_FILE_LIST)->SendMessage(LB_GETCURSEL, 0, 0);

	if(LB_ERR == file_list_sel)
		return;

	h_list.DeleteSingleFile(static_cast<long unsigned int>(formula_sel), static_cast<long unsigned int>(res_sel), static_cast<long unsigned int>(file_list_sel));

    LRESULT new_file_count = GetDlgItem(IDC_FILE_LIST)->SendMessage(LB_DELETESTRING, file_list_sel, 0);

	if(0 == new_file_count)
	{
		skip_warn = true;
		OnBnClickedDeleteResGroupButton();
		skip_warn = false;
	}
	else if(new_file_count != LB_ERR)
	{
		LRESULT new_file_list_sel = file_list_sel;

		if(new_file_list_sel > new_file_count - 1)
			new_file_list_sel = new_file_count - 1;

		GetDlgItem(IDC_FILE_LIST)->SendMessage(LB_SETCURSEL, new_file_list_sel, 0);
		OnLbnSelchangeFileList();
	}
}

void CCacheSettingsDlg::OnBnClickedDeleteResGroupButton()
{
	if(!skip_warn && BST_CHECKED == GetDlgItem(IDC_DELETE_RES_GROUP_WARN_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
	{
		int ret = 0;

		MessageBeep(MB_ICONINFORMATION);
		ret = MessageBox("Are you sure that you want to delete all of the history and cache files for this formula's selected resolution?", APP_NAME, MB_YESNO|MB_ICONQUESTION);

		if(IDNO == ret)
			return;
	}

	LRESULT formula_sel = GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(CB_ERR == formula_sel)
		return;

	LRESULT res_sel = GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(CB_ERR == res_sel)
		return;

	h_list.DeleteResolution(static_cast<long unsigned int>(formula_sel), static_cast<long unsigned int>(res_sel));

    LRESULT new_res_group_count = GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_DELETESTRING, res_sel, 0);

	if(0 == new_res_group_count)
	{
		skip_warn = true;
		OnBnClickedDeleteFormulaGroupButton();
		skip_warn = false;
	}
	else if(new_res_group_count != CB_ERR)
	{
		GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_SETCURSEL, 0, 0);
		OnCbnSelchangeResolutionGroupCombo();
	}
}

void CCacheSettingsDlg::OnBnClickedDeleteFormulaGroupButton()
{
	if(!skip_warn && BST_CHECKED == GetDlgItem(IDC_DELETE_FORMULA_GROUP_WARN_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
	{
		int ret = 0;

		MessageBeep(MB_ICONINFORMATION);
		ret = MessageBox("Are you sure that you want to delete all of the history and cache files for this formula?", APP_NAME, MB_YESNO|MB_ICONQUESTION);

		if(IDNO == ret)
			return;
	}

	LRESULT formula_sel = GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_GETCURSEL, 0, 0);

	if(CB_ERR == formula_sel)
		return;

	h_list.DeleteFormula(static_cast<long unsigned int>(formula_sel));

    LRESULT new_formula_group_count = GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_DELETESTRING, formula_sel, 0);

	if(0 == new_formula_group_count)
	{
		while(1)
		{
			if(LB_ERR == GetDlgItem(IDC_FILE_LIST)->SendMessage(LB_DELETESTRING, 0, 0))
				break;
		}

		while(1)
		{
			if(CB_ERR == GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->SendMessage(CB_DELETESTRING, 0, 0))
				break;
		}

		GetDlgItem(IDC_FORMULA_GROUP_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RESOLUTION_GROUP_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_FILE_LIST)->EnableWindow(FALSE);

		GetDlgItem(IDC_DELETE_IDENTIFIER_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DELETE_RES_GROUP_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DELETE_FORMULA_GROUP_BUTTON)->EnableWindow(FALSE);

		GetDlgItem(IDC_REVERSE_SET_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CULL_CHECK)->EnableWindow(FALSE);
	}
	else if(new_formula_group_count != CB_ERR)
	{
		GetDlgItem(IDC_FORMULA_GROUP_COMBO)->SendMessage(CB_SETCURSEL, 0, 0);
		OnCbnSelchangeFormulaGroupCombo();
	}
}



void CCacheSettingsDlg::OnBnClickedAutoGencheck()
{
	if(BST_CHECKED == GetDlgItem(IDC_AUTO_GENCHECK)->SendMessage(BM_GETCHECK, 0, 0))
		AfxGetApp()->WriteProfileInt("Settings", "AutoGenCachedSetsOnDblClick", 1);
	else
		AfxGetApp()->WriteProfileInt("Settings", "AutoGenCachedSetsOnDblClick", 0);
}

void CCacheSettingsDlg::OnBnClickedAutoCloseCacheSettingsCheck()
{
	if(BST_CHECKED == GetDlgItem(IDC_AUTO_CLOSE_CACHE_SETTINGS_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		AfxGetApp()->WriteProfileInt("Settings", "AutoCloseCacheSettingsOnDblClick", 1);
	else
		AfxGetApp()->WriteProfileInt("Settings", "AutoCloseCacheSettingsOnDblClick", 0);
}

void CCacheSettingsDlg::OnBnClickedDiskSettingsButton()
{
	CDiskSettingsDlg DiskSettingsDlg;

	DiskSettingsDlg.DoModal();
	FillListsFromCache();
}

void CCacheSettingsDlg::OnBnClickedDeleteFileItemWarnCheck()
{
	if(BST_CHECKED == GetDlgItem(IDC_DELETE_FILE_ITEM_WARN_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		AfxGetApp()->WriteProfileInt("Settings", "WarnOnHistoryFileDelete", 1);
	else
		AfxGetApp()->WriteProfileInt("Settings", "WarnOnHistoryFileDelete", 0);
}

void CCacheSettingsDlg::OnBnClickedDeleteResGroupWarnCheck()
{
	if(BST_CHECKED == GetDlgItem(IDC_DELETE_RES_GROUP_WARN_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		AfxGetApp()->WriteProfileInt("Settings", "WarnOnHistoryResolutionDelete", 1);
	else
		AfxGetApp()->WriteProfileInt("Settings", "WarnOnHistoryResolutionDelete", 0);
}

void CCacheSettingsDlg::OnBnClickedDeleteFormulaGroupWarnCheck()
{
	if(BST_CHECKED == GetDlgItem(IDC_DELETE_FORMULA_GROUP_WARN_CHECK)->SendMessage(BM_GETCHECK, 0, 0))
		AfxGetApp()->WriteProfileInt("Settings", "WarnOnHistoryFormulaDelete", 1);
	else
		AfxGetApp()->WriteProfileInt("Settings", "WarnOnHistoryFormulaDelete", 0);
}


