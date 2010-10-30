#ifndef JULIA4D2_UTILITIES_H
#define JULIA4D2_UTILITIES_H

#include "defines.h"
//#include "marching_x.h"
#include "julia4d2Doc.h"
#include "quaternion_math.h"

#include <windows.h>
#include <d3d9.h>

#include <vector>
#include <string>
#include <new>
#include <cmath>
#include <string>
#include <fstream>
using std::vector;
using std::string;
using std::wstring;
using std::bad_alloc;
using std::sqrt;
using std::basic_string;
using std::ofstream;
using std::ifstream;


// misc functions
bool MakeCubeOfVertices(POS_NORMAL_COLOUR_VERTEX *&vertices, long unsigned int &vertex_count);
RGB HSBtoRGB(unsigned short int hue_degree, unsigned char sat_percent, unsigned char bri_percent);

void ColourVertices(const HWND &progressbar, const HWND &status_text, POS_NORMAL_COLOUR_VERTEX *const vertices, const long unsigned int &vertex_count, const bool random);
void DotProdColourVertices(const HWND &progressbar, const HWND &status_text, POS_NORMAL_COLOUR_VERTEX *const vertices, const long unsigned int &vertex_count, const bool &use_normals);
void CowColourVertices(const HWND &progressbar, const HWND &status_text, POS_NORMAL_COLOUR_VERTEX *const vertices, const long unsigned int &vertex_count);

void FlatColourVertices(const DWORD &colour, const HWND &progressbar, const HWND &status_text, POS_NORMAL_COLOUR_VERTEX *const vertices, const long unsigned int &vertex_count);
void CentreAndScaleVertices(POS_NORMAL_COLOUR_VERTEX *&vertices, const long unsigned int &vertex_count, const float &max_half_dimension);
string lower_string(const string &src_string);
string upper_string(const string &src_string);
string trim_whitespace_string(const string &src_string);

/*
string trim_formula_comment(const string &src_string);
string get_unique_formula_string(const string &src_string);
*/

bool is_signed_int(const string &src_string);
bool is_long_unsigned_int(const string &src_string);
bool is_real_number(const string &src_string);
bool is_valid_mul_variable(const string &mul_variable);
int get_mul_variable_type(const string &mul_variable);
string get_mul_variable_string(const long unsigned int &mul_var);
string GetRandomCustomMulVariable(void);



wstring get_module_path_wide(void);

string wsts(const wstring &src_string);
wstring stws(const string &src_string);

wstring MakeHexFromData(const void *const data, const size_t &len_in_bytes);
wstring MakeHexFromMD5HashFromVoidData(const void *const data, const size_t &len_in_bytes);
wstring MakeHexFromMD5HashFromJ4D2Doc(const Cjulia4d2Doc &params, const long unsigned int &cache_file_type);


int EncodeBoolVectToChar(unsigned char *&dest_buf, vector<bool> &data, size_t &dest_buf_len);
int DecodeCharToBoolVect(vector<bool> &dest_vect, unsigned char *data, const size_t &src_buf_len);

int RLECompress(const unsigned char * const src_buf, const size_t &src_len, unsigned char * const dest_buf, size_t &dest_len);
int RLEDecompress(const unsigned char * const src_buf, const size_t &src_len, unsigned char * const dest_buf);


float radians_to_degrees(const float &radians);

int WriteJ4D2VoxelFile(vector<bool> &data, const char *const file_name, const float &x_min, const float &x_max, const float &y_min, const float &y_max, const float &z_min, const float &z_max, const long unsigned int &x_resolution, const long unsigned int &y_resolution, const long unsigned int &z_resolution);




__forceinline void PerformHopfMapping(POS_NORMAL_COLOUR_VERTEX &dest_vertex, const quaternion_t<double> &Z)
{
	// hopf mapping:
	// x3d = x* x + y* y + z*-z + w*-w
	// y3d = x* w + y* z + z* y + w* x
	// z3d = x*-z + y* w + z*-x + w* y
	// which boils down to...
	// x3d = xx + yy - zz - ww
	// y3d = 2(xw + yz)
	// z3d = 2(yw - xz)

	//		dest_vertex.x = static_cast<float>(Z.x* Z.x + Z.y* Z.y + Z.z*-Z.z + Z.w*-Z.w);
	//		dest_vertex.y = static_cast<float>(Z.x* Z.w + Z.y* Z.z + Z.z* Z.y + Z.w* Z.x);
	//		dest_vertex.z = static_cast<float>(Z.x*-Z.z + Z.y* Z.w + Z.z*-Z.x + Z.w* Z.y);

	dest_vertex.x = static_cast<float>(Z.x*Z.x + Z.y*Z.y - Z.z*Z.z - Z.w*Z.w);
	dest_vertex.y = static_cast<float>(2.0 * (Z.x*Z.w + Z.y*Z.z));
	dest_vertex.z = static_cast<float>(2.0 * (Z.y*Z.w - Z.x*Z.z));
}




__forceinline unsigned char EncodeToUnsignedChar(vector<bool> &data, const size_t &offset)
{
	unsigned char data_item = 0;
	data_item  = (data.at(offset + 0) << 7);
	data_item += (data.at(offset + 1) << 6);
	data_item += (data.at(offset + 2) << 5);
	data_item += (data.at(offset + 3) << 4);
	data_item += (data.at(offset + 4) << 3);
	data_item += (data.at(offset + 5) << 2);
	data_item += (data.at(offset + 6) << 1);
	data_item += (data.at(offset + 7));

	return data_item;
}






int PadAndGetRLELength(vector<bool> &data, size_t &length);
int PadAndWriteToFile(vector<bool> &data, ofstream &out);
int ReadFromFile(vector<bool> &data, ifstream &in, const size_t &uncompressed_length, const size_t &compressed_length);



// function that takes a token (space character for instance
// and a source string, then chops up the string into chunks based on the token
// and then puts each individual chunk one by one into a vector
// it returns the number of chunks found
__forceinline vector<string> STLStrTok(string token, const string &SrcString)
{
	vector<string> DestVector;

	size_t PreviousPosition = 0;
	// set the current position to wherever we find the first token at
	// 0 says start at the beginning of the string
	size_t CurrentPosition = SrcString.find(token, 0);

	// if we've reached the end of the string, and no token was found
	// add the entire string as one giant chunk
	// and then return 1
	if(CurrentPosition==string::npos)
	{
		DestVector.push_back(SrcString);
		return DestVector;
	}

	// else, if the string isn't blank, push the substring into the vector
	if(SrcString.substr(PreviousPosition, CurrentPosition-PreviousPosition)!="")
		DestVector.push_back(SrcString.substr(PreviousPosition, CurrentPosition-PreviousPosition));

	// keep doing it until we hit beyond the end of the source string
	while(CurrentPosition!=string::npos)
	{
		PreviousPosition = CurrentPosition+token.length();
		CurrentPosition = SrcString.find(token, PreviousPosition);

		if(SrcString.substr(PreviousPosition, CurrentPosition-PreviousPosition)!="")
			DestVector.push_back(SrcString.substr(PreviousPosition, CurrentPosition-PreviousPosition));
	}

	return DestVector;
}



#endif



