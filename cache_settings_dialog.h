#ifndef CACHE_SETTINGS_DLG_HEADER
#define CACHE_SETTINGS_DLG_HEADER


#include "resource.h"
#include "j4d2_cache_system.h"
#include "MainFrm.h"



#include "windows.h"


#include <utility>
using std::pair;



// CCacheSettingsDlg dialog used for App About
struct history_file_info
{
	wstring file_name;
	wstring MD5_hash_string;
	LARGE_INTEGER creation_time;

	Cjulia4d2Doc params;
	bool cached;

	history_file_info(void)
	{
		file_name = L"";
		MD5_hash_string = L"";
		cached = false;
		creation_time.QuadPart = 0;
	}

	history_file_info(const history_file_info &rhs)
	{
		file_name = rhs.file_name;
		MD5_hash_string = rhs.MD5_hash_string;
		params = rhs.params;
		cached = rhs.cached;
	}

	// sort by render to type, then by reversed creation time
	bool operator<(const history_file_info &rhs) const
	{
		if(params.render_to_type < rhs.params.render_to_type)
			return true;

		if(creation_time.QuadPart > rhs.creation_time.QuadPart)
			return true;


		return false;
	}

	void Delete(void)
	{
		DeleteFileW(file_name.c_str());

		wstring cache_file_name = file_name;

		if(cache_file_name.length() > 5)
		{
			// delete cache file
			cache_file_name = cache_file_name.substr(0, cache_file_name.length() - 5);
			DeleteFileW(cache_file_name.c_str());

			if(cache_file_name.length() > 10)
			{
				// delete surface and reverse surface cache files
				cache_file_name = cache_file_name.substr(0, cache_file_name.length() - 10);

				wstring surface_file_name = cache_file_name;
				surface_file_name += L"-surface";

				wstring surface_reverse_file_name = surface_file_name;
				surface_reverse_file_name += L"-reverse";

				surface_file_name += L".";
				surface_file_name += J4D2_CACHE_FILE_EXTENSION;

				surface_reverse_file_name += L".";
				surface_reverse_file_name += J4D2_CACHE_FILE_EXTENSION;

				DeleteFileW(surface_file_name.c_str());
				DeleteFileW(surface_reverse_file_name.c_str());
			}
		}
	}
};


struct history_resolution_group
{
	long unsigned int resolution;
	multiset<history_file_info> cache_files;

	void AddFile(const wstring &src_file_name, const Cjulia4d2Doc &temp_params, LARGE_INTEGER &src_creation_time)
	{
		history_file_info temp_hfi;

		temp_hfi.file_name = src_file_name;
		temp_hfi.params = temp_params;
		temp_hfi.creation_time = src_creation_time;

		wstring cache_file_name = src_file_name;

		if(cache_file_name.length() <= 5)
		{
			temp_hfi.cached = false;
		}
		else
		{
			cache_file_name = cache_file_name.substr(0, cache_file_name.length() - 5);

			WIN32_FIND_DATAW FileData;
			DWORD dwAttrs = 0;

			HANDLE hSearch = FindFirstFileW(cache_file_name.c_str(), &FileData);

			if(hSearch == INVALID_HANDLE_VALUE) // nothing found
			{
				temp_hfi.cached = false;
			}
			else
			{
				if(FILE_ATTRIBUTE_DIRECTORY & GetFileAttributesW(cache_file_name.c_str()))
				{
					temp_hfi.cached = false;
				}
				else
				{
					temp_hfi.cached = true;
				}
			}

			FindClose(hSearch);
		}

		temp_hfi.MD5_hash_string = src_file_name;

		size_t pos = temp_hfi.MD5_hash_string.find_last_of(L'\\');

		if(pos != wstring::npos)
		{
			temp_hfi.MD5_hash_string = temp_hfi.MD5_hash_string.substr(pos + 1, temp_hfi.MD5_hash_string.length() - pos);
		}

		// trim .j4d2cache.j4d2
		if(temp_hfi.MD5_hash_string.length() > 15)
			temp_hfi.MD5_hash_string = temp_hfi.MD5_hash_string.substr(0, temp_hfi.MD5_hash_string.length() - 15);

		cache_files.insert(temp_hfi);
	}

	bool operator<(const history_resolution_group &rhs) const
	{
		if(resolution < rhs.resolution)
			return true;

		return false;
	}

	void Delete(void)
	{
		for(multiset<history_file_info>::iterator i = cache_files.begin(); i != cache_files.end(); i++)
		{
			i->Delete();
		}

		cache_files.clear();
	}

	void DeleteSingleFile(const long unsigned int &file_num)
	{
		if(file_num >= cache_files.size())
			return;

		long unsigned int iter_count = 0;

		for(multiset<history_file_info>::iterator i = cache_files.begin(); i != cache_files.end(); i++)
		{
			if(iter_count == file_num)
			{
				i->Delete();
				cache_files.erase(i);
				break;
			}

			iter_count++;
		}
	}
};


struct history_formula_group
{
	wstring formula_text;
	set<history_resolution_group> resolutions;

	void AddFile(const wstring &src_file_name, const Cjulia4d2Doc &temp_params, LARGE_INTEGER &src_creation_time)
	{
		history_resolution_group temp_hrg;

		temp_hrg.resolution = temp_params.resolution;

		// try to insert new hfg
		// if exists already, will get iterator to existing, otherwise, will get iterator to new
		pair< set<history_resolution_group>::iterator, bool > pr;
		pr = resolutions.insert(temp_hrg);
		pr.first->AddFile(src_file_name, temp_params, src_creation_time);
	}

	bool operator<(const history_formula_group &rhs) const
	{
		if(formula_text < rhs.formula_text)
			return true;

		return false;
	}

	void Delete(void)
	{
		for(set<history_resolution_group>::iterator i = resolutions.begin(); i != resolutions.end(); i++)
		{
			i->Delete();
		}

		resolutions.clear();
	}

	void DeleteResolution(const long unsigned int &resolution_group_num)
	{
		if(resolution_group_num >= resolutions.size())
			return;

		long unsigned int iter_count = 0;

		for(set<history_resolution_group>::iterator i = resolutions.begin(); i != resolutions.end(); i++)
		{
			if(iter_count == resolution_group_num)
			{
				i->Delete();
				resolutions.erase(i);
				break;
			}

			iter_count++;
		}
	}

	void DeleteSingleFile(const long unsigned int &resolution_group_num, const long unsigned int &file_num)
	{
		if(resolution_group_num >= resolutions.size())
			return;

		long unsigned int iter_count = 0;

		for(set<history_resolution_group>::iterator i = resolutions.begin(); i != resolutions.end(); i++)
		{
			if(iter_count == resolution_group_num)
			{
				i->DeleteSingleFile(file_num);
				break;
			}

			iter_count++;
		}
	}

};


struct history_list
{
	set<history_formula_group> formulas;

	bool AddFile(const wstring &src_file_name, LARGE_INTEGER &src_creation_time)
	{
		Cjulia4d2Doc temp_params;
		temp_params.set_parent_vars = false;

		if(FALSE == temp_params.OnOpenDocument(wsts(src_file_name).c_str()))
			return false;

		history_formula_group temp_hfg;

		temp_hfg.formula_text = stws(temp_params.formula_text);

		// try to insert new hfg
		// if exists already, will get iterator to existing, otherwise, will get iterator to new
		pair< set<history_formula_group>::iterator, bool > pr;
		pr = formulas.insert(temp_hfg);
		pr.first->AddFile(src_file_name, temp_params, src_creation_time);

		return true;
	}

	void Clear(void)
	{
		formulas.clear();
	}

	void DeleteFormula(const long unsigned int &formula_num)
	{
		if(formula_num >= formulas.size())
			return;

		long unsigned int iter_count = 0;

		for(set<history_formula_group>::iterator i = formulas.begin(); i != formulas.end(); i++)
		{
			if(iter_count == formula_num)
			{
				i->Delete();
				formulas.erase(i);
				break;
			}

			iter_count++;
		}
	}

	// gotta add a bunch of these so that it will auto delete stuff
	void DeleteResolution(const long unsigned int &formula_num, const long unsigned int &resolution_num)
	{
		if(formula_num >= formulas.size())
			return;

		long unsigned int iter_count = 0;

		for(set<history_formula_group>::iterator i = formulas.begin(); i != formulas.end(); i++)
		{
			if(iter_count == formula_num)
			{
				i->DeleteResolution(resolution_num);
				break;
			}

			iter_count++;
		}
	}

	// gotta add a bunch of these so that it will auto delete stuff
	void DeleteSingleFile(const long unsigned int &formula_num, const long unsigned int &resolution_num, const long unsigned int &file_num)
	{
		if(formula_num >= formulas.size())
			return;

		long unsigned int iter_count = 0;

		for(set<history_formula_group>::iterator i = formulas.begin(); i != formulas.end(); i++)
		{
			if(iter_count == formula_num)
			{
				i->DeleteSingleFile(resolution_num, file_num);
				break;
			}

			iter_count++;
		}
	}
};


class CCacheSettingsDlg : public CDialog
{
public:
	CCacheSettingsDlg(CMainFrame *src_parent_frame);

	// Dialog Data
	enum { IDD = IDD_CACHESETTINGSBOX };


protected:

	CMainFrame *parent_frame;
	history_list h_list;



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL OnInitDialog();
	void SetVars(Cjulia4d2Doc &doc);

	void ClearLists(void);

	bool clearing_lists;
	bool skip_warn;

	void FillListsFromCache(void);


	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeFormulaGroupCombo();
	afx_msg void OnCbnSelchangeResolutionGroupCombo();
	afx_msg void OnLbnSelchangeFileList();
	afx_msg void OnLbnDblclkFileList();
	afx_msg void OnBnClickedDeleteIdentifierButton();
	afx_msg void OnBnClickedDeleteResGroupButton();
	afx_msg void OnBnClickedDeleteFormulaGroupButton();
	afx_msg void OnBnClickedAutoGencheck();
	afx_msg void OnBnClickedAutoCloseCacheSettingsCheck();
	afx_msg void OnBnClickedDiskSettingsButton();
	afx_msg void OnBnClickedDeleteFileItemWarnCheck();
	afx_msg void OnBnClickedDeleteResGroupWarnCheck();
	afx_msg void OnBnClickedDeleteFormulaGroupWarnCheck();
};


#endif