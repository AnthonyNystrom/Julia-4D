#ifndef JULIA4D2_CACHE_SYSTEM_H
#define JULIA4D2_CACHE_SYSTEM_H

#include <string>
#include <set>
#include <sstream>
using std::string;
using std::basic_string;
using std::multiset;
using std::set;
using std::ostringstream;

#include "julia4d2Doc.h"
#include "defines.h"
#include "utilities.h"



struct file_info
{
	wstring file_name;
	LARGE_INTEGER file_size;
	LARGE_INTEGER last_access_time;
	LARGE_INTEGER creation_time;

	bool operator<(const file_info &rhs) const
	{
		// sort by extension...
		wstring f_ext = file_name;
		size_t f_ext_dot_pos = f_ext.find_first_of(L'.');

		if(f_ext_dot_pos != string::npos)
			f_ext.erase(0, f_ext_dot_pos + 1);
		else
			f_ext = L"";

		wstring rhs_f_ext = rhs.file_name;
		size_t rhs_f_ext_dot_pos = rhs_f_ext.find_first_of(L'.');

		if(rhs_f_ext_dot_pos != string::npos)
			rhs_f_ext.erase(0, rhs_f_ext_dot_pos + 1);
		else
			rhs_f_ext = L"";


		if(f_ext < rhs_f_ext)
			return true;

		if(file_size.QuadPart < rhs.file_size.QuadPart)
			return true;

		if(last_access_time.QuadPart < rhs.last_access_time.QuadPart)
			return true;

		return false;
	}
};



// cache file collection, sorted ascending by size, access time
struct cache_file_info_collection
{
	multiset<file_info> file_list;
	LARGE_INTEGER total_size_in_bytes;

	cache_file_info_collection(const wstring &src_path)
	{
		total_size_in_bytes.QuadPart = 0;

		if(0 == src_path.length())
			return;

		wstring base_file_path;
		wstring file_mask;

		base_file_path = src_path;

		if(base_file_path[base_file_path.length()] != L'\\')
			base_file_path += L'\\';

		file_mask = base_file_path;
		file_mask += L"*.";
		file_mask += J4D2_CACHE_FILE_EXTENSION; // delete only cache files
//		file_mask += J4D2_ALL_FILE_EXTENSION; // delete cache AND settings files

		WIN32_FIND_DATAW FileData;
		DWORD dwAttrs = 0;

		HANDLE hSearch = FindFirstFileW(file_mask.c_str(), &FileData);

		if(hSearch == INVALID_HANDLE_VALUE) // nothing found
		{
			return;
		}
		else // process first file
		{
			wstring file_name = base_file_path;
			file_name += FileData.cFileName;

			if(!(FILE_ATTRIBUTE_DIRECTORY & GetFileAttributesW(file_name.c_str())))
			{
				LARGE_INTEGER high;
				high.QuadPart = FileData.nFileSizeHigh;

				LARGE_INTEGER low;
				low.QuadPart = FileData.nFileSizeLow;

				LARGE_INTEGER max_dword;
				max_dword.QuadPart = MAXDWORD;
				max_dword.QuadPart += 1;

				file_info temp_file_info;
				temp_file_info.file_name = file_name;
				temp_file_info.file_size.QuadPart = (high.QuadPart * max_dword.QuadPart) + low.QuadPart;
				temp_file_info.last_access_time.LowPart = FileData.ftLastAccessTime.dwLowDateTime;
				temp_file_info.last_access_time.HighPart = FileData.ftLastAccessTime.dwHighDateTime;
				temp_file_info.creation_time.LowPart = FileData.ftCreationTime.dwLowDateTime;
				temp_file_info.creation_time.HighPart = FileData.ftCreationTime.dwHighDateTime;

				file_list.insert(temp_file_info);
			}
		}

		// process subsequent files
		while(FindNextFileW(hSearch, &FileData))
		{
			wstring file_name = base_file_path;
			file_name += FileData.cFileName;

			if(!(FILE_ATTRIBUTE_DIRECTORY & GetFileAttributesW(file_name.c_str())))
			{
				LARGE_INTEGER high;
				high.QuadPart = FileData.nFileSizeHigh;

				LARGE_INTEGER low;
				low.QuadPart = FileData.nFileSizeLow;

				LARGE_INTEGER max_dword;
				max_dword.QuadPart = MAXDWORD;
				max_dword.QuadPart += 1;

				file_info temp_file_info;
				temp_file_info.file_name = file_name;
				temp_file_info.file_size.QuadPart = (high.QuadPart * max_dword.QuadPart) + low.QuadPart;
				temp_file_info.last_access_time.LowPart = FileData.ftLastAccessTime.dwLowDateTime;
				temp_file_info.last_access_time.HighPart = FileData.ftLastAccessTime.dwHighDateTime;

				file_list.insert(temp_file_info);
			}
		}

		FindClose(hSearch);

		for(multiset<file_info>::const_iterator i = file_list.begin(); i != file_list.end(); i++)
			total_size_in_bytes.QuadPart += i->file_size.QuadPart;
	}

	__forceinline bool DeleteFirst(void)
	{
		if(file_list.size() != 0)
		{
//			ostringstream out;
//			out << "Cache delete:\n";
//			out << wsts(file_list.begin()->file_name.c_str());
//			MessageBox(0, out.str().c_str(), APP_NAME, MB_OK);
			
			LARGE_INTEGER temp_file_size;
			temp_file_size.QuadPart = file_list.begin()->file_size.QuadPart;

			if(DeleteFileW(file_list.begin()->file_name.c_str()))
			{
				file_list.erase(file_list.begin());
				total_size_in_bytes.QuadPart -= temp_file_size.QuadPart;
				return true;
			}
			
		}

		return false;
	}
};







struct settings_file_info_collection
{
	multiset<file_info> file_list;
	LARGE_INTEGER total_size_in_bytes;

	settings_file_info_collection(const wstring &src_path)
	{
		total_size_in_bytes.QuadPart = 0;

		if(0 == src_path.length())
			return;

		wstring base_file_path;
		wstring file_mask;

		base_file_path = src_path;

		if(base_file_path[base_file_path.length()] != L'\\')
			base_file_path += L'\\';

		file_mask = base_file_path;
		file_mask += L"*.";
		file_mask += J4D2_SETTINGS_FILE_EXTENSION;

		WIN32_FIND_DATAW FileData;
		DWORD dwAttrs = 0;

		HANDLE hSearch = FindFirstFileW(file_mask.c_str(), &FileData);

		if(hSearch == INVALID_HANDLE_VALUE) // nothing found
		{
			return;
		}
		else // process first file
		{
			wstring file_name = base_file_path;
			file_name += FileData.cFileName;

			if(!(FILE_ATTRIBUTE_DIRECTORY & GetFileAttributesW(file_name.c_str())))
			{
				LARGE_INTEGER high;
				high.QuadPart = FileData.nFileSizeHigh;

				LARGE_INTEGER low;
				low.QuadPart = FileData.nFileSizeLow;

				LARGE_INTEGER max_dword;
				max_dword.QuadPart = MAXDWORD;
				max_dword.QuadPart += 1;

				file_info temp_file_info;
				temp_file_info.file_name = file_name;
				temp_file_info.file_size.QuadPart = (high.QuadPart * max_dword.QuadPart) + low.QuadPart;
				temp_file_info.last_access_time.LowPart = FileData.ftLastAccessTime.dwLowDateTime;
				temp_file_info.last_access_time.HighPart = FileData.ftLastAccessTime.dwHighDateTime;

				file_list.insert(temp_file_info);
			}
		}

		// process subsequent files
		while(FindNextFileW(hSearch, &FileData))
		{
			wstring file_name = base_file_path;
			file_name += FileData.cFileName;

			if(!(FILE_ATTRIBUTE_DIRECTORY & GetFileAttributesW(file_name.c_str())))
			{
				LARGE_INTEGER high;
				high.QuadPart = FileData.nFileSizeHigh;

				LARGE_INTEGER low;
				low.QuadPart = FileData.nFileSizeLow;

				LARGE_INTEGER max_dword;
				max_dword.QuadPart = MAXDWORD;
				max_dword.QuadPart += 1;

				file_info temp_file_info;
				temp_file_info.file_name = file_name;
				temp_file_info.file_size.QuadPart = (high.QuadPart * max_dword.QuadPart) + low.QuadPart;
				temp_file_info.last_access_time.LowPart = FileData.ftLastAccessTime.dwLowDateTime;
				temp_file_info.last_access_time.HighPart = FileData.ftLastAccessTime.dwHighDateTime;

				file_list.insert(temp_file_info);
			}
		}

		FindClose(hSearch);

		for(multiset<file_info>::const_iterator i = file_list.begin(); i != file_list.end(); i++)
			total_size_in_bytes.QuadPart += i->file_size.QuadPart;
	}
};








struct cache_data
{
	cache_data(void)
	{
		w_step_number = 0;
		z_step_number = 0;
		y_step_number = 0;
		x_step_number = 0;
	}

	vector<bool> data;

	long unsigned int w_step_number;
	long unsigned int z_step_number;
	long unsigned int y_step_number;
	long unsigned int x_step_number;
};










class j4d2_cache_system
{
public:

	j4d2_cache_system(void);
	bool CanClear(void);
	void PerformClear(void);
	bool PerformLimitTrim(void);

	int GetCacheData(const Cjulia4d2Doc &params, cache_data &hit, const long unsigned int &cache_file_type, const HWND &status_text);
	int SetCacheData(const Cjulia4d2Doc &params, cache_data &hit, const long unsigned int &cache_file_type, const long unsigned int &major_plane, const HWND &status_text);

	string GetLastError(void)
	{
		return last_error.str();
	}


	bool MakeRoomForFile(const long unsigned int &file_size_in_mb);
	basic_string <wchar_t> GetFilePathForData(const Cjulia4d2Doc &params, const long unsigned int &cache_file_type);

protected:
	bool enable_cache_reads;
	bool enable_cache_writes;
	wstring cache_path;

	bool enable_cache_surface_reads;
	bool enable_cache_surface_writes;

	ostringstream last_error;
};


#endif