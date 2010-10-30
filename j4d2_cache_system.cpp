#include "stdafx.h"


#include "j4d2_cache_system.h"
#include "utilities.h"

#include <iostream>
#include <iomanip>
#include <fstream>
using std::ofstream;
using std::ifstream;
using std::ios_base;


//todo: do caching for other things, ie:
//todo: encode hopf 3d boundary as boolvect and cache that as its own type, etc.
//todo: when caching hopf maps, use the same identifier, but append the field resoltion to the filename. ie: *-400-surface-reverse.j4d2cache


j4d2_cache_system::j4d2_cache_system(void)
{
	long signed int EnableCacheReads = 0;
	long signed int EnableCacheWrites = 0;

	if(0 == AfxGetApp()->GetProfileInt("Cache", "EnableReads", 0))
		enable_cache_reads = false;
	else
		enable_cache_reads = true;

	if(0 == AfxGetApp()->GetProfileInt("Cache", "EnableWrites", 0))
		enable_cache_writes = false;
	else
		enable_cache_writes = true;

	if(0 == AfxGetApp()->GetProfileInt("Cache", "EnableSurfaceReads", 0))
		enable_cache_surface_reads = false;
	else
		enable_cache_surface_reads = true;

	if(0 == AfxGetApp()->GetProfileInt("Cache", "EnableSurfaceWrites", 0))
		enable_cache_surface_writes = false;
	else
		enable_cache_surface_writes = true;


	cache_path = get_module_path_wide();

	if(cache_path == L"")
	{
		enable_cache_reads = false;
		enable_cache_writes = false;
		return;
	}

	cache_path += L"\\";
	cache_path += J4D2_CACHE_FOLDER_NAME;

	if(enable_cache_reads || enable_cache_writes)
	{
		if(!CreateDirectoryW(cache_path.c_str(), 0))
		{
			DWORD LastError = ::GetLastError();

			if(ERROR_ALREADY_EXISTS != LastError)
			{
				enable_cache_reads = false;
				enable_cache_writes = false;
				cache_path = L"";
				return;
			}
		}
	}
}





bool j4d2_cache_system::PerformLimitTrim(void)
{
	if(cache_path == L"")
		return false;

	long signed int LimitDiskUsage = 0;
	long unsigned int *LimitSizeBuf = 0;
	long unsigned int LimitSize = 0;
	UINT LimitSizeBytesRead = 0;

	if(0 == AfxGetApp()->GetProfileInt("Cache", "LimitDiskUsage", 0))
		return false;

	if(!AfxGetApp()->GetProfileBinary("Cache", "LimitSize", (LPBYTE*)&LimitSizeBuf, &LimitSizeBytesRead) || LimitSizeBytesRead != sizeof(LimitSize))
	{
		if(0 != LimitSizeBuf)
			delete [] LimitSizeBuf;

		return false;
	}
	else
	{
		LimitSize = *LimitSizeBuf;
	}

	if(0 != LimitSizeBuf)
		delete [] LimitSizeBuf;


	cache_file_info_collection fcol(cache_path);

	if(fcol.total_size_in_bytes.QuadPart / BYTES_PER_MEGABYTE < LimitSize && LimitSize != 0)
		return true;
	else
	{
		while(fcol.total_size_in_bytes.QuadPart / BYTES_PER_MEGABYTE > LimitSize)
		{
			if(!fcol.DeleteFirst())
				break;
		}
	}

	return true;
}


bool j4d2_cache_system::MakeRoomForFile(const long unsigned int &file_size_in_mb)
{
	if(cache_path == L"")
		return false;

	long signed int LimitDiskUsage = 0;
	long unsigned int *LimitSizeBuf = 0;
	long unsigned int LimitSize = 0;
	UINT LimitSizeBytesRead = 0;

	if(0 == AfxGetApp()->GetProfileInt("Cache", "LimitDiskUsage", 0))
		return true;

	if(!AfxGetApp()->GetProfileBinary("Cache", "LimitSize", (LPBYTE*)&LimitSizeBuf, &LimitSizeBytesRead) || LimitSizeBytesRead != sizeof(LimitSize))
	{
		if(0 != LimitSizeBuf)
			delete [] LimitSizeBuf;

		return false;
	}
	else
	{
		LimitSize = *LimitSizeBuf;
	}

	if(0 != LimitSizeBuf)
		delete [] LimitSizeBuf;


	if(LimitSize < file_size_in_mb)
		return false;


	cache_file_info_collection fcol(cache_path);

	LARGE_INTEGER target_size;
	target_size.QuadPart = LimitSize;
	target_size.QuadPart -= file_size_in_mb;

	while(fcol.total_size_in_bytes.QuadPart / BYTES_PER_MEGABYTE >= target_size.QuadPart )
	{
		if(!fcol.DeleteFirst())
			break;
	}

	return true;
}



bool j4d2_cache_system::CanClear(void)
{
	if(cache_path == L"")
		return false;

	wstring base_file_path;
	wstring file_mask;

	base_file_path = cache_path;
	base_file_path += L"\\";

	file_mask = base_file_path;
	file_mask += L"*.";
//	file_mask += J4D2_CACHE_FILE_EXTENSION;
	file_mask += J4D2_ALL_FILE_EXTENSION;

	bool found_file = false;

	WIN32_FIND_DATAW FileData;
	DWORD dwAttrs = 0;

	HANDLE hSearch = FindFirstFileW(file_mask.c_str(), &FileData);

	if(hSearch != INVALID_HANDLE_VALUE)
	{
		wstring file_name = base_file_path;
		file_name += FileData.cFileName;

		if(!(FILE_ATTRIBUTE_DIRECTORY & GetFileAttributesW(file_name.c_str())))
			found_file = true;

		while(!found_file && FindNextFileW(hSearch, &FileData))
		{
			wstring file_name = base_file_path;
			file_name += FileData.cFileName;

			if(!(FILE_ATTRIBUTE_DIRECTORY & GetFileAttributesW(file_name.c_str())))
				found_file = true;
		}

		FindClose(hSearch);
	}

	return found_file;
}

void j4d2_cache_system::PerformClear(void)
{
	if(cache_path == L"")
		return;

	wstring base_file_path;
	wstring file_mask;

	base_file_path = cache_path;
	base_file_path += L"\\";

	file_mask = base_file_path;
	file_mask += L"*.";
//	file_mask += J4D2_CACHE_FILE_EXTENSION;
	file_mask += J4D2_ALL_FILE_EXTENSION;

	WIN32_FIND_DATAW FileData;
	DWORD dwAttrs = 0;

	HANDLE hSearch = FindFirstFileW(file_mask.c_str(), &FileData);

	if(hSearch == INVALID_HANDLE_VALUE) // nothing found
	{
		RemoveDirectoryW(cache_path.c_str());
		return;
	}
	else
	{
		wstring file_name = base_file_path;
		file_name += FileData.cFileName;

		if(!(FILE_ATTRIBUTE_DIRECTORY & GetFileAttributesW(file_name.c_str())))
			DeleteFileW(file_name.c_str());
	}

	while(FindNextFileW(hSearch, &FileData))
	{
		wstring file_name = base_file_path;
		file_name += FileData.cFileName;

		if(!(FILE_ATTRIBUTE_DIRECTORY & GetFileAttributesW(file_name.c_str())))
			DeleteFileW(file_name.c_str());
	} 

	FindClose(hSearch);
	RemoveDirectoryW(cache_path.c_str());
}



wstring j4d2_cache_system::GetFilePathForData(const Cjulia4d2Doc &params, const long unsigned int &cache_file_type)
{
	if(cache_path == L"")
		return L"";

	wstring file_name = cache_path;
	file_name += L'\\';
	file_name += MakeHexFromMD5HashFromJ4D2Doc(params, cache_file_type);

	if(J4D2_CACHE_SURFACE_NONSAMPLEDTYPE == cache_file_type)
	{
		file_name += L"-surface";

		if(params.reverse_set)
			file_name += L"-reverse";
	}

	file_name += L".";
	file_name += J4D2_CACHE_FILE_EXTENSION;

	return file_name;
}



// ret codes
// -2 file error
// -1 bad alloc
// 0 cancelled/no cache data
// 1 OK

/*


// rle compressed versions of Get/Set
int j4d2_cache_system::GetCacheData(const Cjulia4d2Doc &params, cache_data &hit, const long unsigned int &cache_file_type, const HWND &status_text)
{
	if(cache_path == L"" || !enable_cache_reads)
		return 0;

	if(cache_file_type == J4D2_CACHE_SURFACE_NONSAMPLEDTYPE && !enable_cache_surface_reads)
		return 0;


	wstring in_filename;

	if(L"" == (in_filename = GetFilePathForData(params, cache_file_type)))
		return 0;

	ifstream in(wsts(in_filename).c_str(), ios_base::binary);

	if(in.fail())
		return 0;

	// read header
	char *header_buf = 0;

	try
	{
		header_buf = new char[J4D2_CACHE_FILE_HDR_LEN + 1];
	}
	catch(...)
	{
		last_error.str("Header buffer allocation failure.");
		return -1;
	}

	// validate header
	in.read(header_buf, static_cast<long unsigned int>(J4D2_CACHE_FILE_HDR_LEN));
	header_buf[J4D2_CACHE_FILE_HDR_LEN] = '\0';
	string header_string = header_buf;
	delete [] header_buf;
	if( lower_string(J4D2_CACHE_FILE_HDR) != lower_string(header_string) ||
		in.fail() ||
		in.eof())
	{
		last_error.str("Header format mismatch.");
		return -2;
	}

	// read cache version number
	long unsigned int cache_version = 0;
	in.read((char *)&cache_version, sizeof(long unsigned int));
	if(in.fail() ||	in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	if(cache_version != J4D2_CACHE_FILE_VERSION)
		return 0;



	// read w_step_number
	in.read((char *)&hit.w_step_number, sizeof(long unsigned int));
	if(in.fail() ||	in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	// read x_step_number
	in.read((char *)&hit.x_step_number, sizeof(long unsigned int));
	if(in.fail() ||	in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	// read y_step_number
	in.read((char *)&hit.y_step_number, sizeof(long unsigned int));
	if(in.fail() ||	in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	// read z_step_number
	in.read((char *)&hit.z_step_number, sizeof(long unsigned int));
	if(in.fail() ||	in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	if(cache_file_type == J4D2_CACHE_NONSAMPLEDTYPE)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Loading set from cache...");
			UpdateWindow(status_text);
		}
	}
	else if(cache_file_type == J4D2_CACHE_SURFACE_NONSAMPLEDTYPE)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Loading surface from cache...");
			UpdateWindow(status_text);
		}
	}


	// read uncompressed data length
	long unsigned int uncompressed_len = 0;
	in.read((char *)&uncompressed_len, sizeof(long unsigned int));
	if( uncompressed_len == 0 ||
		in.fail() ||
		in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	// read compressed length
	size_t compressed_len = 0;
	in.read((char *)&compressed_len, sizeof(long unsigned int));
	if( compressed_len == 0 ||
		in.fail() ||
		in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	// make buffers for uncompressed and compressed data
	unsigned char *uncompressed_data = 0;
	unsigned char *compressed_data = 0;

	try
	{
		uncompressed_data = new unsigned char[uncompressed_len];
	}
	catch(...)
	{
		if(0 != compressed_data)
			delete [] compressed_data;

		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		last_error.str("Uncompressed array allocation failure.");

		return -1;
	}

	try
	{
		compressed_data = new unsigned char[compressed_len];
	}
	catch(...)
	{
		if(0 != compressed_data)
			delete [] compressed_data;

		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		last_error.str("Compressed array allocation failure.");

		return -1;
	}


	// if either memory allocation failed (this is not really necessary now
	if(0 == uncompressed_data || 0 == compressed_data)
	{
		if(0 != compressed_data)
			delete [] compressed_data;

		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		return -1;
	}

	// read compressed data
	in.read((char *)compressed_data, static_cast<long unsigned int>(compressed_len));
	if(in.fail() && !in.eof())
	{
		if(0 != compressed_data)
			delete [] compressed_data;

		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		last_error.str("File read error.");

		return -2;
	}

	in.close();


	// uncompress data
	int ret = RLEDecompress(compressed_data, compressed_len, uncompressed_data);

	if(ret != 1)
	{
		if(0 != compressed_data)
			delete [] compressed_data;

		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		last_error.str("RLEDecompress error.");

		return ret;
	}

	// decode data
	ret = DecodeCharToBoolVect(hit.data, uncompressed_data, uncompressed_len);

	if(ret != 1)
		last_error.str("DecodeCharToBoolVect error.");

	if(0 != compressed_data)
		delete [] compressed_data;

	if(0 != uncompressed_data)
		delete [] uncompressed_data;

	return ret;
}







int j4d2_cache_system::SetCacheData(const Cjulia4d2Doc &params, cache_data &hit, const long unsigned int &cache_file_type, const long unsigned int &major_plane, const HWND &status_text)
{
clock_t start = clock();

	if(cache_path == L"" || !enable_cache_writes)
		return 0;

	if(cache_file_type == J4D2_CACHE_SURFACE_NONSAMPLEDTYPE && !enable_cache_surface_writes)
		return 0;



	wstring out_filename;

	if(L"" == (out_filename = GetFilePathForData(params, cache_file_type)))
		return 0;


	// if cache file already exists, ensure that the major plane isn't already at the max
	// ie: the existing cache file is already a complete set, so don't bother overwriting
	ifstream in(wsts(out_filename).c_str(), ios_base::binary);

	if(in.fail())
	{
		in.close();
	}
	else
	{
		// read header
		char *header_buf = new(std::nothrow) char[J4D2_CACHE_FILE_HDR_LEN + 1];

		if(0 != header_buf)
		{
			// validate header
			in.read(header_buf, static_cast<long unsigned int>(J4D2_CACHE_FILE_HDR_LEN));
			header_buf[J4D2_CACHE_FILE_HDR_LEN] = '\0';
			string header_string = header_buf;
			delete [] header_buf;

			if(! ( lower_string(J4D2_CACHE_FILE_HDR) != lower_string(header_string) ||
				in.fail() ||
				in.eof()) )
			{
				long unsigned int temp_w_step_number = 0;
				long unsigned int temp_x_step_number = 0;
				long unsigned int temp_y_step_number = 0;
				long unsigned int temp_z_step_number = 0;

				long unsigned int cache_version = 0;
				in.read((char *)&cache_version, sizeof(long unsigned int));
				in.read((char *)&temp_w_step_number, sizeof(long unsigned int));
				in.read((char *)&temp_x_step_number, sizeof(long unsigned int));
				in.read((char *)&temp_y_step_number, sizeof(long unsigned int));
				in.read((char *)&temp_z_step_number, sizeof(long unsigned int));

				if(!in.fail() && !in.eof())
				{
					// if cache file same or newer version as this one, check to see if we can skip overwrite
					if(cache_version >= J4D2_CACHE_FILE_VERSION)
					{
						if(major_plane == J4D2_CACHE_W_MAJOR_PLANE)
							if(temp_w_step_number == params.resolution)
								return true;

						if(major_plane == J4D2_CACHE_Z_MAJOR_PLANE)
							if(temp_z_step_number == params.resolution)
								return true;
					}
				}
			}
		}

		in.close();
	}



	if(cache_file_type == J4D2_CACHE_NONSAMPLEDTYPE)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Saving set to cache...");
			UpdateWindow(status_text);
		}
	}
	else if(cache_file_type == J4D2_CACHE_SURFACE_NONSAMPLEDTYPE)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Saving surface to cache...");
			UpdateWindow(status_text);
		}
	}




	// encode data
	size_t uncompressed_len = 0;
	unsigned char *uncompressed_data = 0;
	int ret = EncodeBoolVectToChar(uncompressed_data, hit.data, uncompressed_len);

	if(0 == uncompressed_data || 0 == uncompressed_len || ret != 1)
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		last_error.str("EncodeBoolVectToChar error.");

		return -1;
	}


	size_t compressed_len = 0;

	// test to find out how much memory to allocate for compressed buffer
	ret = RLECompress(uncompressed_data, uncompressed_len, 0, compressed_len);

	if(ret != 1)
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		last_error.str("RLECompress error.");

		return -1;
	}

	unsigned char *compressed_data = 0;
	
	try
	{
		compressed_data = new unsigned char[compressed_len];
	}
	catch(...)
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		last_error.str("Compressed array memory allocation failure.");

		return -1;
	}

	if(0 == compressed_data)
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		return -1;
	}

	// actually compress data
	ret = RLECompress(uncompressed_data, uncompressed_len, compressed_data, compressed_len);



	if(ret != 1)
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		last_error.str("RLECompress error.");

		return ret;
	}


	// file header string length + xyzw step numbers + size in bytes of actual rle compressed array
	long unsigned int file_header_info_size = 0;
	file_header_info_size += static_cast<long unsigned int>(J4D2_CACHE_FILE_HDR_LEN);
	file_header_info_size += (5 * static_cast<long unsigned int>(sizeof(long unsigned int)));


	if(!MakeRoomForFile( (file_header_info_size + compressed_len) / BYTES_PER_MEGABYTE ))
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		return 0;
	}



	//write cache
	ofstream out(wsts(out_filename).c_str(), ios_base::binary);

	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write header
	out.write(J4D2_CACHE_FILE_HDR, static_cast<long unsigned int>(J4D2_CACHE_FILE_HDR_LEN));
	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}


	// write cache_version
	long unsigned int cache_version = J4D2_CACHE_FILE_VERSION;
	out.write((const char *)&cache_version, sizeof(long unsigned int));
	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}


	// write w_step_number
	out.write((const char *)&hit.w_step_number, sizeof(long unsigned int));
	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write x_step_number
	out.write((const char *)&hit.x_step_number, sizeof(long unsigned int));
	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write y_step_number
	out.write((const char *)&hit.y_step_number, sizeof(long unsigned int));
	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write z_step_number
	out.write((const char *)&hit.z_step_number, sizeof(long unsigned int));
	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write uncompressed data length
	out.write((const char *)&uncompressed_len, sizeof(long unsigned int));
	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write compressed data length
	out.write((const char *)&compressed_len, sizeof(long unsigned int));
	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write compressed data
	out.write((const char *)compressed_data, static_cast<long unsigned int>(compressed_len));
	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	delete [] uncompressed_data;
	delete [] compressed_data;

	clock_t finish = clock();
	double duration = (double)(finish - start) / CLOCKS_PER_SEC;

	ostringstream debug;
	debug << duration;
	MessageBox(0, debug.str().c_str(), APP_NAME, MB_OK);

	out.close();

	if(cache_file_type != J4D2_CACHE_SURFACE_NONSAMPLEDTYPE)
	{
		// write j4d2 file as well
		wstring settings_filename = out_filename;
		settings_filename += L".j4d2";

		Cjulia4d2Doc temp_params;
		temp_params = params;
		temp_params.write_ident_arcball = true;

		temp_params.cull_non_surface_points = false;
		temp_params.reverse_set = false;

	//	if(temp_params.render_to_type != MESH_FILE)
			temp_params.render_to_type = POINT_SET;

		temp_params.OnSaveDocument(wsts(settings_filename).c_str());
	}

	return 1;
}



*/










































































// rle compressed versions of Get/Set
int j4d2_cache_system::GetCacheData(const Cjulia4d2Doc &params, cache_data &hit, const long unsigned int &cache_file_type, const HWND &status_text)
{
	if(cache_path == L"" || !enable_cache_reads)
		return 0;

	if(cache_file_type == J4D2_CACHE_SURFACE_NONSAMPLEDTYPE && !enable_cache_surface_reads)
		return 0;


	wstring in_filename;

	if(L"" == (in_filename = GetFilePathForData(params, cache_file_type)))
		return 0;

	ifstream in(wsts(in_filename).c_str(), ios_base::binary);

	if(in.fail())
		return 0;

	// read header
	char *header_buf = 0;

	try
	{
		header_buf = new char[J4D2_CACHE_FILE_HDR_LEN + 1];
	}
	catch(...)
	{
		last_error.str("Header buffer allocation failure.");
		return -1;
	}

	// validate header
	in.read(header_buf, static_cast<long unsigned int>(J4D2_CACHE_FILE_HDR_LEN));
	header_buf[J4D2_CACHE_FILE_HDR_LEN] = '\0';
	string header_string = header_buf;
	delete [] header_buf;
	if( lower_string(J4D2_CACHE_FILE_HDR) != lower_string(header_string) ||
		in.fail() ||
		in.eof())
	{
		last_error.str("Header format mismatch.");
		return -2;
	}

	// read cache version number
	long unsigned int cache_version = 0;
	in.read((char *)&cache_version, sizeof(long unsigned int));
	if(in.fail() ||	in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	if(cache_version != J4D2_CACHE_FILE_VERSION)
		return 0;



	// read w_step_number
	in.read((char *)&hit.w_step_number, sizeof(long unsigned int));
	if(in.fail() ||	in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	// read x_step_number
	in.read((char *)&hit.x_step_number, sizeof(long unsigned int));
	if(in.fail() ||	in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	// read y_step_number
	in.read((char *)&hit.y_step_number, sizeof(long unsigned int));
	if(in.fail() ||	in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	// read z_step_number
	in.read((char *)&hit.z_step_number, sizeof(long unsigned int));
	if(in.fail() ||	in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	if(cache_file_type == J4D2_CACHE_NONSAMPLEDTYPE)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Loading set from cache...");
			UpdateWindow(status_text);
		}
	}
	else if(cache_file_type == J4D2_CACHE_SURFACE_NONSAMPLEDTYPE)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Loading surface from cache...");
			UpdateWindow(status_text);
		}
	}


	// read uncompressed data length
	size_t uncompressed_length = 0;
	in.read((char *)&uncompressed_length, sizeof(long unsigned int));
	if( uncompressed_length == 0 ||
		in.fail() ||
		in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}

	// read compressed length
	size_t compressed_length = 0;
	in.read((char *)&compressed_length, sizeof(long unsigned int));
	if( compressed_length == 0 ||
		in.fail() ||
		in.eof())
	{
		last_error.str("File read error.");
		return -2;
	}



	int ret = ReadFromFile(hit.data, in, uncompressed_length, compressed_length);

	if(ret != 1)
		return ret;

	in.close();

	return ret;
}








int j4d2_cache_system::SetCacheData(const Cjulia4d2Doc &params, cache_data &hit, const long unsigned int &cache_file_type, const long unsigned int &major_plane, const HWND &status_text)
{
	if(cache_path == L"" || !enable_cache_writes)
		return 0;

	if(cache_file_type == J4D2_CACHE_SURFACE_NONSAMPLEDTYPE && !enable_cache_surface_writes)
		return 0;



	wstring out_filename;

	if(L"" == (out_filename = GetFilePathForData(params, cache_file_type)))
		return 0;


	// if cache file already exists, ensure that the major plane isn't already at the max
	// ie: the existing cache file is already a complete set, so don't bother overwriting
	ifstream in(wsts(out_filename).c_str(), ios_base::binary);

	if(in.fail())
	{
		in.close();
	}
	else
	{
		// read header
		char *header_buf = new(std::nothrow) char[J4D2_CACHE_FILE_HDR_LEN + 1];

		if(0 != header_buf)
		{
			// validate header
			in.read(header_buf, static_cast<long unsigned int>(J4D2_CACHE_FILE_HDR_LEN));
			header_buf[J4D2_CACHE_FILE_HDR_LEN] = '\0';
			string header_string = header_buf;
			delete [] header_buf;

			if(! ( lower_string(J4D2_CACHE_FILE_HDR) != lower_string(header_string) ||
				in.fail() ||
				in.eof()) )
			{
				long unsigned int temp_w_step_number = 0;
				long unsigned int temp_x_step_number = 0;
				long unsigned int temp_y_step_number = 0;
				long unsigned int temp_z_step_number = 0;

				long unsigned int cache_version = 0;
				in.read((char *)&cache_version, sizeof(long unsigned int));
				in.read((char *)&temp_w_step_number, sizeof(long unsigned int));
				in.read((char *)&temp_x_step_number, sizeof(long unsigned int));
				in.read((char *)&temp_y_step_number, sizeof(long unsigned int));
				in.read((char *)&temp_z_step_number, sizeof(long unsigned int));

				if(!in.fail() && !in.eof())
				{
					// if cache file same or newer version as this one, check to see if we can skip overwrite
					if(cache_version >= J4D2_CACHE_FILE_VERSION)
					{
						if(major_plane == J4D2_CACHE_W_MAJOR_PLANE)
							if(temp_w_step_number == params.resolution)
								return true;

						if(major_plane == J4D2_CACHE_Z_MAJOR_PLANE)
							if(temp_z_step_number == params.resolution)
								return true;
					}
				}
			}
		}

		in.close();
	}



	if(cache_file_type == J4D2_CACHE_NONSAMPLEDTYPE)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Saving set to cache...");
			UpdateWindow(status_text);
		}
	}
	else if(cache_file_type == J4D2_CACHE_SURFACE_NONSAMPLEDTYPE)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Saving surface to cache...");
			UpdateWindow(status_text);
		}
	}
	
	size_t rle_length = 0;
	size_t data_length = 0;
	int ret = PadAndGetRLELength(hit.data, rle_length);
	if(ret != 1)
		return ret;

	data_length = hit.data.size()/8;



	// file header string length + xyzw step numbers + size in bytes of actual rle compressed array
	long unsigned int file_header_info_size = 0;
	file_header_info_size += static_cast<long unsigned int>(J4D2_CACHE_FILE_HDR_LEN);
	file_header_info_size += (5 * static_cast<long unsigned int>(sizeof(long unsigned int)));


	if(!MakeRoomForFile((file_header_info_size + rle_length) / BYTES_PER_MEGABYTE ))
		return 0;


	//write cache
	ofstream out(wsts(out_filename).c_str(), ios_base::binary);

	if(out.fail())
	{
		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write header
	out.write(J4D2_CACHE_FILE_HDR, static_cast<long unsigned int>(J4D2_CACHE_FILE_HDR_LEN));
	if(out.fail())
	{
		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}


	// write cache_version
	long unsigned int cache_version = J4D2_CACHE_FILE_VERSION;
	out.write((const char *)&cache_version, sizeof(long unsigned int));
	if(out.fail())
	{
		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}


	// write w_step_number
	out.write((const char *)&hit.w_step_number, sizeof(long unsigned int));
	if(out.fail())
	{
		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write x_step_number
	out.write((const char *)&hit.x_step_number, sizeof(long unsigned int));
	if(out.fail())
	{
		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write y_step_number
	out.write((const char *)&hit.y_step_number, sizeof(long unsigned int));
	if(out.fail())
	{
		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write z_step_number
	out.write((const char *)&hit.z_step_number, sizeof(long unsigned int));
	if(out.fail())
	{
		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write uncompressed data length
	out.write((const char *)&data_length, sizeof(long unsigned int));
	if(out.fail())
	{
		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}

	// write compressed data length
	out.write((const char *)&rle_length, sizeof(long unsigned int));
	if(out.fail())
	{
		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return -2;
	}


	// write compressed data
	ret = PadAndWriteToFile(hit.data, out);

	if(ret != 1)
	{
		last_error.str("File write error.");

		out.close();
		DeleteFileW(out_filename.c_str());

		return ret;
	}




	out.close();

	if(cache_file_type != J4D2_CACHE_SURFACE_NONSAMPLEDTYPE)
	{
		// write j4d2 file as well
		wstring settings_filename = out_filename;
		settings_filename += L".j4d2";

		Cjulia4d2Doc temp_params;
		temp_params = params;
		temp_params.write_ident_arcball = true;

		temp_params.cull_non_surface_points = false;
		temp_params.reverse_set = false;

	//	if(temp_params.render_to_type != MESH_FILE)
			temp_params.render_to_type = POINT_SET;

		temp_params.OnSaveDocument(wsts(settings_filename).c_str());
	}

	return 1;
}








