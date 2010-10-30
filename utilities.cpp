#include "stdafx.h"


#include "utilities.h"
#include "md5.h"
#include "perlin.h"

#include <sstream>
#include <iomanip>
#include <fstream>
using std::ostringstream;
using std::setw;
using std::setfill;
using std::hex;
using std::ofstream;

bool MakeCubeOfVertices(POS_NORMAL_COLOUR_VERTEX *&vertices, long unsigned int &vertex_count)
{
	const long unsigned int z_res = 25;
	const long unsigned int y_res = 25;
	const long unsigned int x_res = 25;
	const float x_min = -100.0;
	const float y_min = -100.0;
	const float z_min = -100.0;
	const float x_max =  0.0;
	const float y_max =  0.0;
	const float z_max =  0.0;
	const float x_step_size = (x_max - x_min) / (x_res - 1);
	const float y_step_size = (y_max - y_min) / (y_res - 1);
	const float z_step_size = (z_max - z_min) / (z_res - 1);

	float x_pos = x_max;
	float y_pos = y_max;
	float z_pos = z_max;

	if(vertices != 0)
	{
		delete [] vertices;
		vertices = 0;
	}

	vertex_count = z_res * y_res * x_res;

	try
	{
		vertices = new POS_NORMAL_COLOUR_VERTEX[vertex_count];
	}
	catch(bad_alloc)
	{
		MessageBox(NULL, "Memory allocation error.", APP_NAME, MB_OK|MB_ICONSTOP);
		vertex_count = 0;
		return false;
	}

	for(long unsigned z = 0; z < z_res; z++, z_pos -= z_step_size, y_pos = y_max)
	{
		for(long unsigned y = 0; y < y_res; y++, y_pos -= y_step_size, x_pos = x_max)
		{
			for(long unsigned x = 0; x < x_res; x++, x_pos -= x_step_size)
			{
				long unsigned int index = z * y_res * x_res + y * x_res + x;

				vertices[index].z = z_pos;
				vertices[index].y = y_pos;
				vertices[index].x = x_pos;
				vertices[index].colour = 0xFF0000; // transparency, red, green, blue
			}
		}
	}

	return true;
}






void ColourVertices(const HWND &progressbar, const HWND &status_text, POS_NORMAL_COLOUR_VERTEX *const vertices, const long unsigned int &vertex_count, const bool random)
{
	if(0 == vertex_count || 0 == vertices)
		return;


	unsigned short int percent_done = 0;
	unsigned short int last_percent_done = 0;
	long unsigned int total_count = 0;
	long unsigned int num_steps_total = vertex_count;

	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	if(0 != status_text)
	{
		SetWindowText(status_text, "Measuring vertex lengths...");
		UpdateWindow(status_text);
	}


	float min_3d_length = FLT_MAX;
	float max_3d_length = FLT_MIN;

	// find min and max distances from origin
	for(long unsigned int minmax_find_counter = 0; minmax_find_counter < vertex_count; minmax_find_counter++)
	{
		float vertex_length = sqrt(vertices[minmax_find_counter].x * vertices[minmax_find_counter].x + vertices[minmax_find_counter].y * vertices[minmax_find_counter].y + vertices[minmax_find_counter].z * vertices[minmax_find_counter].z);

		if(vertex_length > max_3d_length)
			max_3d_length = vertex_length;

		if(vertex_length < min_3d_length)
			min_3d_length = vertex_length;


		if(0 != progressbar)
		{
			total_count++;

			percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

			if(percent_done > last_percent_done/* + 5*/ || percent_done == 100)
			{
				last_percent_done = percent_done;
				SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
				UpdateWindow(progressbar);
			}
		}
	}



	percent_done = 0;
	last_percent_done = 0;
	total_count = 0;
	num_steps_total = vertex_count;

	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	if(0 != status_text)
	{
		SetWindowText(status_text, "Colouring vertices...");
		UpdateWindow(status_text);
	}

	

	double max_rainbow = 360.0;
	double min_rainbow = 360.0;

	if(random)
	{
		max_rainbow = static_cast<double>(rand()%3601) / 10.0;
		min_rainbow = static_cast<double>(rand()%3601) / 10.0;
	}

	if(max_rainbow < min_rainbow)
	{
		double temp = max_rainbow;
		max_rainbow = min_rainbow;
		min_rainbow = temp;
	}

	// colour
	for(long unsigned int colouring_counter = 0; colouring_counter < vertex_count; colouring_counter++)
	{
		float vertex_length = sqrt(vertices[colouring_counter].x * vertices[colouring_counter].x + vertices[colouring_counter].y * vertices[colouring_counter].y + vertices[colouring_counter].z * vertices[colouring_counter].z) - min_3d_length;

		RGB rgb = HSBtoRGB(static_cast<unsigned short int>(
			max_rainbow - ((vertex_length / (max_3d_length - min_3d_length)) * min_rainbow)),
			static_cast<unsigned char>(50),
			static_cast<unsigned char>(100));

		vertices[colouring_counter].colour = D3DCOLOR_ARGB(255, rgb.r, rgb.g, rgb.b);


		if(0 != progressbar)
		{
			total_count++;

			percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

			if(percent_done > last_percent_done || percent_done == 100)
			{
				last_percent_done = percent_done;
				SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
				UpdateWindow(progressbar);
			}
		}
	}
}






void DotProdColourVertices(const HWND &progressbar, const HWND &status_text, POS_NORMAL_COLOUR_VERTEX *const vertices, const long unsigned int &vertex_count, const bool &use_normals)
{
	if(0 == vertex_count || 0 == vertices)
		return;


	unsigned short int percent_done = 0;
	unsigned short int last_percent_done = 0;
	long unsigned int total_count = 0;
	long unsigned int num_steps_total = vertex_count;

	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	if(0 != status_text)
	{
		SetWindowText(status_text, "Colouring vertices...");
		UpdateWindow(status_text);
	}

	float orientor_x = 0.57735f;
	float orientor_y = 0.57735f;
	float orientor_z = -0.57735f;

	float temp_x = 0.0f;
	float temp_y = 0.0f;
	float temp_z = 0.0f;

	// colour
	for(long unsigned int colouring_counter = 0; colouring_counter < vertex_count; colouring_counter++)
	{
		if(true == use_normals)
		{
			temp_x = vertices[colouring_counter].nx;
			temp_y = vertices[colouring_counter].ny;
			temp_z = vertices[colouring_counter].nz;
		}
		else
		{
			temp_x = vertices[colouring_counter].x;
			temp_y = vertices[colouring_counter].y;
			temp_z = vertices[colouring_counter].z;
		}


		float len = sqrt(temp_x*temp_x + temp_y*temp_y + temp_z*temp_z);

		if(len != 1.0)
		{
			temp_x /= len;
			temp_y /= len;
			temp_z /= len;
		}

		float dot_prod = temp_x*orientor_x + temp_y*orientor_y + temp_z*orientor_z;

		dot_prod += 1.0f; // range 0 through 2
		dot_prod *= 180.0f; // gives range of 0 through 360

		RGB rgb = HSBtoRGB(static_cast<unsigned short int>(
			360.0f - dot_prod),
			static_cast<unsigned char>(50),
			static_cast<unsigned char>(100));

		vertices[colouring_counter].colour = D3DCOLOR_ARGB(255, rgb.r, rgb.g, rgb.b);


		if(0 != progressbar)
		{
			total_count++;

			percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

			if(percent_done > last_percent_done || percent_done == 100)
			{
				last_percent_done = percent_done;
				SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
				UpdateWindow(progressbar);
			}
		}
	}

}







void CowColourVertices(const HWND &progressbar, const HWND &status_text, POS_NORMAL_COLOUR_VERTEX *const vertices, const long unsigned int &vertex_count)
{
	if(0 == vertex_count || 0 == vertices)
		return;

	perlin perlin_noise;

	unsigned short int percent_done = 0;
	unsigned short int last_percent_done = 0;
	long unsigned int total_count = 0;
	long unsigned int num_steps_total = vertex_count;

	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	if(0 != status_text)
	{
		SetWindowText(status_text, "Colouring vertices...");
		UpdateWindow(status_text);
	}


	for(long unsigned int colouring_counter = 0; colouring_counter < vertex_count; colouring_counter++)
	{
		double noise = perlin_noise.PerlinNoise3D(	vertices[colouring_counter].x,
													vertices[colouring_counter].y, 
													vertices[colouring_counter].z,
													2.0, 2.0, 20);

		if(noise < 0.0)
            vertices[colouring_counter].colour = 0xFF6F6F6F;
		else
			vertices[colouring_counter].colour = 0xFFFFFFFF;

		if(0 != progressbar)
		{
			total_count++;

			percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

			if(percent_done > last_percent_done || percent_done == 100)
			{
				last_percent_done = percent_done;
				SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
				UpdateWindow(progressbar);
			}
		}
	}
}









void FlatColourVertices(const DWORD &colour, const HWND &progressbar, const HWND &status_text, POS_NORMAL_COLOUR_VERTEX *const vertices, const long unsigned int &vertex_count)
{
	if(0 == vertex_count || 0 == vertices)
		return;

	unsigned short int percent_done = 0;
	unsigned short int last_percent_done = 0;
	long unsigned int total_count = 0;
	long unsigned int num_steps_total = vertex_count;

	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	if(0 != status_text)
	{
		SetWindowText(status_text, "Colouring vertices...");
		UpdateWindow(status_text);
	}

						
	for(long unsigned int colouring_counter = 0; colouring_counter < vertex_count; colouring_counter++)
	{
		vertices[colouring_counter].colour = colour;

		if(0 != progressbar)
		{
			total_count++;

			percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

			if(percent_done > last_percent_done || percent_done == 100)
			{
				last_percent_done = percent_done;
				SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
				UpdateWindow(progressbar);
			}
		}
	}
}





RGB HSBtoRGB(unsigned short int hue_degree, unsigned char sat_percent, unsigned char bri_percent)
{
	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;

	if(hue_degree > 359)
		hue_degree = 359;

	if(sat_percent > 100)
		sat_percent = 100;

	if(bri_percent > 100)
		bri_percent = 100;

	float hue_pos = 6.0f - ((static_cast<float>(hue_degree) / 359.0f) * 6.0f);

	if(hue_pos >= 0.0f && hue_pos < 1.0f)
	{
		R = 255.0f;
		G = 0.0f;
		B = 255.0f * hue_pos;
	}
	else if(hue_pos >= 1.0f && hue_pos < 2.0f)
	{
		hue_pos -= 1.0f;

		R = 255.0f - (255.0f * hue_pos);
		G = 0.0f;
		B = 255.0f;
	}
	else if(hue_pos >= 2.0f && hue_pos < 3.0f)
	{
		hue_pos -= 2.0f;

		R = 0.0f;
		G = 255.0f * hue_pos;
		B = 255.0f;
	}
	else if(hue_pos >= 3.0f && hue_pos < 4.0f)
	{
		hue_pos -= 3.0f;

		R = 0.0f;
		G = 255.0f;
		B = 255.0f - (255.0f * hue_pos);
	}
	else if(hue_pos >= 4.0f && hue_pos < 5.0f)
	{
		hue_pos -= 4.0f;

		R = 255.0f * hue_pos;
		G = 255.0f;
		B = 0.0f;
	}
	else
	{
		hue_pos -= 5.0f;

		R = 255.0f;
		G = 255.0f - (255.0f * hue_pos);
		B = 0.0f;
	}

	if(100 != sat_percent)
	{
		if(0 == sat_percent)
		{
			R = 255.0f;
			G = 255.0f;
			B = 255.0f;
		}
		else
		{
			if(255.0f != R)
				R += ((255.0f - R) / 100.0f) * (100.0f - sat_percent);
			if(255.0f != G)
				G += ((255.0f - G) / 100.0f) * (100.0f - sat_percent);
			if(255.0f != B)
				B += ((255.0f - B) / 100.0f) * (100.0f - sat_percent);
		}
	}

	if(100 != bri_percent)
	{
		if(0 == bri_percent)
		{
			R = 0.0f;
			G = 0.0f;
			B = 0.0f;
		}
		else
		{
			if(0.0f != R)
				R *= static_cast<float>(bri_percent)/100.0f;
			if(0.0f != G)
				G *= static_cast<float>(bri_percent)/100.0f;
			if(0.0f != B)
				B *= static_cast<float>(bri_percent)/100.0f;
		}
	}

	if(R < 0.0f)
		R = 0.0f;
	else if(R > 255.0f)
		R = 255.0f;

	if(G < 0.0f)
		G = 0.0f;
	else if(G > 255.0f)
		G = 255.0f;

	if(B < 0.0f)
		B = 0.0f;
	else if(B > 255.0f)
		B = 255.0f;

	RGB rgb;

	rgb.r = static_cast<unsigned char>(R);
	rgb.g = static_cast<unsigned char>(G);
	rgb.b = static_cast<unsigned char>(B);

	return rgb;
}


void CentreAndScaleVertices(POS_NORMAL_COLOUR_VERTEX *&vertices, const long unsigned int &vertex_count, const float &max_half_dimension)
{
	if(0 == vertex_count || 0 == vertices)
		return;

	// find min and max distances from origin
	float x_max = FLT_MIN;
	float x_min = FLT_MAX;
	float y_max = FLT_MIN;
	float y_min = FLT_MAX;
	float z_max = FLT_MIN;
	float z_min = FLT_MAX;

	for(long unsigned int minmax_finder_counter = 0; minmax_finder_counter < vertex_count; minmax_finder_counter++)
	{
		if(vertices[minmax_finder_counter].x > x_max)
			x_max = vertices[minmax_finder_counter].x;
		else if(vertices[minmax_finder_counter].x < x_min)
			x_min = vertices[minmax_finder_counter].x;

		if(vertices[minmax_finder_counter].y > y_max)
			y_max = vertices[minmax_finder_counter].y;
		else if(vertices[minmax_finder_counter].y < y_min)
			y_min = vertices[minmax_finder_counter].y;

		if(vertices[minmax_finder_counter].z > z_max)
			z_max = vertices[minmax_finder_counter].z;
		else if(vertices[minmax_finder_counter].z < z_min)
			z_min = vertices[minmax_finder_counter].z;
	}

	// translate to around origin
	float x_offset = (x_max + x_min) / 2;
	float y_offset = (y_max + y_min) / 2;
	float z_offset = (z_max + z_min) / 2;

	for(long unsigned int translation_counter = 0; translation_counter < vertex_count; translation_counter++)
	{
		vertices[translation_counter].x -= x_offset;
		vertices[translation_counter].y -= y_offset;
		vertices[translation_counter].z -= z_offset;
	}

	// scale
	x_max = fabs(x_max - x_offset);
	x_min = fabs(x_min - x_offset);
	y_max = fabs(y_max - y_offset);
	y_min = fabs(y_min - y_offset);
	z_max = fabs(z_max - z_offset);
	z_min = fabs(z_min - z_offset);

	float x_div = 0.0f;
	float y_div = 0.0f;
	float z_div = 0.0f;
	float final_div = 0.0f;

	if(x_max > x_min)
		x_div = x_max;
	else
		x_div = x_min;

	if(y_max > y_min)
		y_div = y_max;
	else
		y_div = y_min;

	if(z_max > z_min)
		z_div = z_max;
	else
		z_div = z_min;

	if(x_div > y_div)
		final_div = x_div;
	else
		final_div = y_div;

	if(z_div > final_div)
		final_div = z_div;

	final_div /= max_half_dimension;

	for(long unsigned int scaling_counter = 0; scaling_counter < vertex_count; scaling_counter++)
	{
		vertices[scaling_counter].x /= final_div;
		vertices[scaling_counter].y /= final_div;
		vertices[scaling_counter].z /= final_div;
	}
}


string lower_string(const string &src_string)
{
	string temp = src_string;

	for(string::iterator i = temp.begin(); i != temp.end(); i++)
		*i = tolower(*i);

	return temp;
}

string upper_string(const string &src_string)
{
	string temp = src_string;

	for(string::iterator i = temp.begin(); i != temp.end(); i++)
		*i = toupper(*i);

	return temp;
}


string trim_whitespace_string(const string &src_string)
{
	string temp_string;

	for(size_t i = 0; i < src_string.length(); i++)
		if(!isspace(src_string[i]))
			temp_string += src_string[i];

	return temp_string;
}

/*
string trim_formula_comment(const string &src_string)
{
	string temp_string = src_string;

	// find colon, delete up to and including it, this allows the user to put a comment at the beginning of the formula
	size_t pos = temp_string.find_first_of(':');

	if(pos != string::npos)
		temp_string.erase(0, pos + 1);	

	return temp_string;
}
*/

/*
//	todo: make sure that custom formula text parser trims excess whitespace, ( ), etc from formula_text before r/w from/to the cache
string get_unique_formula_string(const string &src_string)
{
	string temp_string = lower_string(trim_formula_comment(src_string));

	MessageBox(0, temp_string.c_str(), "lala", MB_OK);

	return src_string;
}
*/

bool is_signed_int(const string &src_string)
{
	if(src_string == "" || src_string.size() > 5)
		return false;

	string temp = lower_string(src_string);

	for(size_t i = 0; i < temp.size(); i++)
	{
		if(temp[i] == '-' && i != 0)
			return false;

		if(!isdigit(temp[i]) && temp[i] != '-')
			return false;
	}

	return true;
}

bool is_long_unsigned_int(const string &src_string)
{
	if(src_string == "" || src_string.length() > 10)
		return false;

	string temp = lower_string(src_string);

	for(size_t i = 0; i < temp.size(); i++)
	{
		if(!isdigit(temp[i]))
			return false;
	}

	double num = atof(temp.c_str());

	if(num > 4294967295.0)
		return false;

	return true;
}

bool is_real_number(const string &src_string)
{
	//ie: 
	//1
	//-23e4
	//1.E2
	//-2.717
	//.31415e1
	//-7.53e-9
	//7.53e+9

	if(src_string == "")
		return false;

	string temp = lower_string(src_string);

	bool found_dot = false;
	bool found_e = false;
	bool found_digit = false;

	for(size_t i = 0; i < temp.size(); i++)
	{
		if(isdigit(temp[i]))
		{
			if(found_digit == false)
				found_digit = true;	
		}
		else if(temp[i] == 'e')
		{
			if(found_e == true || found_digit == false || i == temp.size() - 1)
				return false;
			else
				found_e = true;
		}
		else if(temp[i] == '-' || temp[i] == '+')
		{
			if(!(i == 0 || temp[i-1] == 'e') || i == temp.size() - 1)
				return false;
		}
		else if(temp[i] == '.')
		{
			if(found_dot == true || (i != 0 && temp[i-1] == 'e'))
				return false;
			else
				found_dot = true;
		}
		else
		{
			return false;
		}
	}

	return true;
}




bool is_valid_mul_variable(const string &mul_variable)
{
	string temp = lower_string(mul_variable);

	if( mul_variable == "x" || mul_variable == "+x" || mul_variable == "-x" ||
		mul_variable == "y" || mul_variable == "+y" || mul_variable == "-y" ||
		mul_variable == "z" || mul_variable == "+z" || mul_variable == "-z" ||
		mul_variable == "w" || mul_variable == "+w" || mul_variable == "-w" )
	{
		return true;
	}

	return false;
}

int get_mul_variable_type(const string &mul_variable)
{
	string temp = lower_string(mul_variable);

	if(mul_variable == "x" || mul_variable == "+x")
		return MUL_PLUS_X;
	else if(mul_variable == "-x")
		return MUL_MINUS_X;
	else if(mul_variable == "y" || mul_variable == "+y")
		return MUL_PLUS_Y;
	else if(mul_variable == "-y")
		return MUL_MINUS_Y;
	else if(mul_variable == "z" || mul_variable == "+z")
		return MUL_PLUS_Z;
	else if(mul_variable == "-z")
		return MUL_MINUS_Z;
	else if(mul_variable == "w" || mul_variable == "+w")
		return MUL_PLUS_W;
	else if(mul_variable == "-w")
		return MUL_MINUS_W;
	else
		return -1;
}





string get_mul_variable_string(const long unsigned int &mul_var)
{
	string var_string;

	switch(mul_var)
	{
	case MUL_PLUS_X:
		{
			var_string = "x";
			break;
		}
	case MUL_PLUS_Y:
		{
			var_string = "y";
			break;
		}
	case MUL_PLUS_Z:
		{
			var_string = "z";
			break;
		}
	case MUL_PLUS_W:
		{
			var_string = "w";
			break;
		}
	case MUL_MINUS_X:
		{
			var_string = "-x";
			break;
		}
	case MUL_MINUS_Y:
		{
			var_string = "-y";
			break;
		}
	case MUL_MINUS_Z:
		{
			var_string = "-z";
			break;
		}
	case MUL_MINUS_W:
		{
			var_string = "-w";
			break;
		}
	default:
		{
			var_string = "";
			MessageBox(NULL, "Error while generating random multiplication variable string.", APP_NAME, MB_OK);
			break;
		}
	}

	return var_string;
}








string GetRandomCustomMulVariable(void)
{
	int var = rand()%MUL_MINUS_W+1;

	string var_string;

	switch(var)
	{
	case MUL_PLUS_X:
		{
			var_string = "x";
			break;
		}
	case MUL_PLUS_Y:
		{
			var_string = "y";
			break;
		}
	case MUL_PLUS_Z:
		{
			var_string = "z";
			break;
		}
	case MUL_PLUS_W:
		{
			var_string = "w";
			break;
		}
	case MUL_MINUS_X:
		{
			var_string = "-x";
			break;
		}
	case MUL_MINUS_Y:
		{
			var_string = "-y";
			break;
		}
	case MUL_MINUS_Z:
		{
			var_string = "-z";
			break;
		}
	case MUL_MINUS_W:
		{
			var_string = "-w";
			break;
		}
	default:
		{
			var_string = "";
			MessageBox(NULL, "Error while generating random multiplication variable string.", APP_NAME, MB_OK);
			break;
		}
	}

	return var_string;
}






wstring get_module_path_wide(void)
{
	wchar_t wide_buf[65536];
	wstring module_name;

	if(!::GetModuleFileNameW(GetModuleHandle(0), wide_buf, 65535))
		return module_name;

	wide_buf[65535] = L'\0';

	module_name = wide_buf;

	size_t pos = module_name.find_last_of(L'\\');

	if(pos != string::npos)
		module_name = module_name.substr(0, pos);

	return module_name;
}

string wsts(const wstring &src_string)
{
	size_t src_len = src_string.length();

	if(src_len == 0)
		return "";

	char *buf = new(std::nothrow) char[src_len + 1];

	if(buf == 0)
		return "";

	wcstombs(buf, src_string.c_str(), src_len);
	buf[src_len] = '\0';

	string final_string = buf;

	if(buf != 0)
		delete [] buf;

	return final_string;
}

wstring stws(const string &src_string)
{
	size_t src_len = src_string.length();

	if(src_len == 0)
		return L"";

	wchar_t *buf = new(std::nothrow) wchar_t[src_len + 1];

	if(buf == 0)
		return L"";

	mbstowcs(buf, src_string.c_str(), src_len);
	buf[src_len] = L'\0';

	wstring final_string = buf;

	if(buf != 0)
		delete [] buf;

	return final_string;
}


wstring MakeHexFromData(const void *const data, const size_t &len_in_bytes)
{
	char *byte_sized_data = (char *)data;

	ostringstream out;
	out << (hex);

	for(size_t i = 0; i < len_in_bytes; i++)
		out << static_cast<int>(byte_sized_data[i]);

	return stws(out.str());
}

wstring MakeHexFromMD5HashFromVoidData(const void *const data, const size_t &len_in_bytes)
{
	MD5_CTX m_md5;
	MD5Init(&m_md5, 0);
	MD5Update(&m_md5, (unsigned char *)data, static_cast<long unsigned int>(len_in_bytes));
	MD5Final(&m_md5);

	char *byte_sized_data = (char *)&m_md5.digest;

	ostringstream out;
	out << setw(2) << setfill('0') << hex;

	for(size_t i = 0; i < 16; i++)
	{
		if(i == 4 || i == 8 || i == 12)
			out << '-';

		out << static_cast<unsigned int>(m_md5.digest[i]);
	}

	return stws(upper_string(out.str()));
}






wstring MakeHexFromMD5HashFromJ4D2Doc(const Cjulia4d2Doc &params, const long unsigned int &cache_file_type)
{
	MD5_CTX m_md5;
	MD5Init(&m_md5, 0);

	MD5Update(&m_md5, (unsigned char *)&params.iterations, sizeof(long unsigned int));
	MD5Update(&m_md5, (unsigned char *)&params.infinity, sizeof(double));
	MD5Update(&m_md5, (unsigned char *)&params.resolution, sizeof(long unsigned int));
	MD5Update(&m_md5, (unsigned char *)&params.formula, sizeof(long unsigned int));


	if(params.formula == CUSTOM_ITERATOR)
		MD5Update(&m_md5, (unsigned char *)params.formula_text.c_str(), sizeof(char) * static_cast<long unsigned int>(params.formula_text.length()));


	MD5Update(&m_md5, (unsigned char *)&params.c_x, sizeof(double));
	MD5Update(&m_md5, (unsigned char *)&params.c_y, sizeof(double));
	MD5Update(&m_md5, (unsigned char *)&params.c_z, sizeof(double));
	MD5Update(&m_md5, (unsigned char *)&params.c_w, sizeof(double));

	MD5Update(&m_md5, (unsigned char *)&params.z_w, sizeof(double));

	MD5Update(&m_md5, (unsigned char *)&params.a, sizeof(double));

	MD5Update(&m_md5, (unsigned char *)&params.mul_column_type, sizeof(long unsigned int));

	if(params.mul_column_type == MUL_COLUMN_CONST_VALS)
	{
		MD5Update(&m_md5, (unsigned char *)&params.mul_column_const_vals_x, sizeof(double));
		MD5Update(&m_md5, (unsigned char *)&params.mul_column_const_vals_y, sizeof(double));
		MD5Update(&m_md5, (unsigned char *)&params.mul_column_const_vals_z, sizeof(double));
		MD5Update(&m_md5, (unsigned char *)&params.mul_column_const_vals_w, sizeof(double));
	}

	if(params.mul_column_type == MUL_COLUMN_CUSTOM)
	{
		MD5Update(&m_md5, (unsigned char *)&params.mul_column_custom_x, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_column_custom_y, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_column_custom_z, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_column_custom_w, sizeof(long unsigned int));
	}

	MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_type, sizeof(long unsigned int));

	if(params.mul_matrix_type == MUL_MATRIX_CUSTOM)
	{
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_11, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_12, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_13, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_14, sizeof(long unsigned int));

		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_21, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_22, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_23, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_24, sizeof(long unsigned int));

		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_31, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_32, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_33, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_34, sizeof(long unsigned int));

		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_41, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_42, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_43, sizeof(long unsigned int));
		MD5Update(&m_md5, (unsigned char *)&params.mul_matrix_custom_44, sizeof(long unsigned int));
	}

	MD5Update(&m_md5, (unsigned char *)&params.x_min, sizeof(double));
	MD5Update(&m_md5, (unsigned char *)&params.x_max, sizeof(double));
	MD5Update(&m_md5, (unsigned char *)&params.y_min, sizeof(double));
	MD5Update(&m_md5, (unsigned char *)&params.y_max, sizeof(double));
	MD5Update(&m_md5, (unsigned char *)&params.z_min, sizeof(double));
	MD5Update(&m_md5, (unsigned char *)&params.z_max, sizeof(double));

	if(params.hopf_map)
	{
		MD5Update(&m_md5, (unsigned char *)&params.w_min, sizeof(double));
		MD5Update(&m_md5, (unsigned char *)&params.w_max, sizeof(double));
	}


	MD5Update(&m_md5, (unsigned char *)&params.hopf_map, sizeof(bool));

	MD5Update(&m_md5, (unsigned char *)&params.swap_z_and_c, sizeof(bool));


	// if all dimension resolutions are the same, don't bother caching as proportional
	if(params.proportional_res)
	{
		if(params.hopf_map)
		{
			if(params.x_resolution != params.y_resolution || params.x_resolution != params.z_resolution || params.x_resolution != params.w_resolution || params.y_resolution != params.z_resolution || params.y_resolution != params.w_resolution || params.z_resolution != params.w_resolution)
				MD5Update(&m_md5, (unsigned char *)&params.proportional_res, sizeof(bool));
		}
		else
		{
			if(params.x_resolution != params.y_resolution || params.x_resolution != params.z_resolution || params.y_resolution != params.z_resolution)
				MD5Update(&m_md5, (unsigned char *)&params.proportional_res, sizeof(bool));
		}
	}



	MD5Final(&m_md5);

	char *byte_sized_data = (char *)&m_md5.digest;

	ostringstream out;

	for(size_t i = 0; i < 16; i++)
	{
		if(i == 4 || i == 8 || i == 12)
			out << '-';
		
		out << setw(2) << setfill('0') << hex << static_cast<unsigned short int>(m_md5.digest[i]);
	}

	return stws(upper_string(out.str()));

	return L"";
}




int EncodeBoolVectToChar(unsigned char *&dest_buf, vector<bool> &data, size_t &dest_buf_len)
{
	if(dest_buf != 0)
	{
		delete [] dest_buf;
		dest_buf = 0;
	}

	dest_buf_len = 0;

	if(0 == data.size())
		return -1;

	try
	{
		// make vector size divisible by 8 by adding elements if necessary (aka padding)
		size_t num_elems_to_add = 8 - (data.size() % 8);

		if(num_elems_to_add < 8)
			for(size_t i = 0; i < num_elems_to_add; i++)
				data.push_back(false);

		// allocate memory to char* pointer
		dest_buf_len = data.size() / 8;
		dest_buf = new unsigned char[dest_buf_len];

		// encode through bit shifting.
		// ie: ttftttft would end up being a char with val of 221dec (11011101 binary)
		for(size_t i = 0; i < dest_buf_len; i++)
		{
			dest_buf[i]  = (data.at(i*8 + 0) << 7);
			dest_buf[i] += (data.at(i*8 + 1) << 6);
			dest_buf[i] += (data.at(i*8 + 2) << 5);
			dest_buf[i] += (data.at(i*8 + 3) << 4);
			dest_buf[i] += (data.at(i*8 + 4) << 3);
			dest_buf[i] += (data.at(i*8 + 5) << 2);
			dest_buf[i] += (data.at(i*8 + 6) << 1);
			dest_buf[i] += (data.at(i*8 + 7));
		}
	}
	catch(...)
	{
		if(dest_buf != 0)
		{
			delete [] dest_buf;
			dest_buf = 0;
		}

		dest_buf_len = 0;

		return -1;
	}

	return 1;
}


int DecodeCharToBoolVect(vector<bool> &dest_vect, unsigned char *data, const size_t &src_buf_len)
{
	if(0 == data || 0 == src_buf_len)
	{
		dest_vect.clear();
		dest_vect.reserve(0);
		return -1;
	}

	// create vector<bool> with enough elements to hold the data encoded in the char* buffer
	try
	{
		dest_vect.reserve(src_buf_len * 8);
		if(dest_vect.capacity() < src_buf_len * 8)
		{
			dest_vect.clear();
			dest_vect.reserve(0);
			return -1;
		}

		dest_vect.resize(src_buf_len * 8, false);
		if(dest_vect.size() < src_buf_len * 8)
		{
			dest_vect.clear();
			dest_vect.reserve(0);
			return -1;
		}
	}
	catch(...)
	{
		return -1;
	}

	try
	{
		// decode through bitshifting
		for(size_t i = 0; i < src_buf_len; i++)
		{
			if(data[i] & (1<<7))
				dest_vect.at(i*8 + 0) = true;

			if(data[i] & (1<<6))
				dest_vect.at(i*8 + 1) = true;

			if(data[i] & (1<<5))
				dest_vect.at(i*8 + 2) = true;

			if(data[i] & (1<<4))
				dest_vect.at(i*8 + 3) = true;

			if(data[i] & (1<<3))
				dest_vect.at(i*8 + 4) = true;

			if(data[i] & (1<<2))
				dest_vect.at(i*8 + 5) = true;

			if(data[i] & (1<<1))
				dest_vect.at(i*8 + 6) = true;

			if(data[i] & 1)
				dest_vect.at(i*8 + 7) = true;
		}
	}
	catch(...)
	{
		dest_vect.clear();
		dest_vect.reserve(0);
		return -1;
	}

	return 1;
}






int RLECompress(const unsigned char * const src_buf, const size_t &src_len, unsigned char * const dest_buf, size_t &dest_len)
{
	// compression scheme:
	// 4 bytes repeat value (size_t) -- use bit shifting to encode to 4 chars
	// 1 byte data (0 or 1 value)
	// repeat as necessary

	if(0 == src_buf || 0 == src_len)
	{
		dest_len = 0;

		if(0 != dest_buf)
			delete [] dest_buf;

		return 0;
	}

	size_t src_buf_index = 0;
	size_t dest_buf_index = 0;

	while(src_buf_index < src_len)
	{
		unsigned char cur_data_item = src_buf[src_buf_index];
		src_buf_index++;

		size_t repeat_count = 0;

		while(src_buf_index < src_len && src_buf[src_buf_index] == cur_data_item)
		{
			src_buf_index++;
			repeat_count++;
		}

		if(dest_buf)
		{
			dest_buf[dest_buf_index + 0] = static_cast<unsigned char>(repeat_count >> 24);
			dest_buf[dest_buf_index + 1] = static_cast<unsigned char>(repeat_count >> 16);
			dest_buf[dest_buf_index + 2] = static_cast<unsigned char>(repeat_count >> 8);
			dest_buf[dest_buf_index + 3] = static_cast<unsigned char>(repeat_count);
			dest_buf[dest_buf_index + 4] = static_cast<unsigned char>(cur_data_item);
		}

		dest_buf_index += 5;
	}

	dest_len = dest_buf_index;

	return 1;
}





int RLEDecompress(const unsigned char * const src_buf, const size_t &src_len, unsigned char * const dest_buf)
{
	// compression scheme:
	// 1 byte data (0 or 1 value)
	// 4 bytes repeat value (size_t) -- use bit shifting to encode to 4 chars
	// repeat as necessary

	if(0 == src_buf || 0 == src_len || 0 == dest_buf)
	{
		if(0 != dest_buf)
			delete [] dest_buf;

		return 0;
	}

	if(src_len % 5 != 0)
	{
		if(0 != dest_buf)
			delete [] dest_buf;

		return 0;
	}

	size_t src_buf_index = 0;
	size_t dest_buf_index = 0;

	while(src_buf_index < src_len)
	{
		size_t repeat_count = 0;

		repeat_count  = (src_buf[src_buf_index + 0] << 24);
		repeat_count += (src_buf[src_buf_index + 1] << 16);
		repeat_count += (src_buf[src_buf_index + 2] << 8);
		repeat_count += (src_buf[src_buf_index + 3]);

		for(size_t i = 0; i < repeat_count+1; i++)
		{
			dest_buf[dest_buf_index] = src_buf[src_buf_index + 4];
			dest_buf_index++;
		}

		src_buf_index += 5;
	}

	return 1;
}




float radians_to_degrees(const float &radians)
{
	return radians * (180.0f / 3.1415926535897932384626433832795f);
}









// todo: change this to use the same bufferless method as the cache system
int WriteJ4D2VoxelFile(vector<bool> &data, const char *const file_name, const float &x_min, const float &x_max, const float &y_min, const float &y_max, const float &z_min, const float &z_max, const long unsigned int &x_resolution, const long unsigned int &y_resolution, const long unsigned int &z_resolution)
{
	// sanity check, though this situation can't ever happen in Julia 4D 2
	if(data.size() == 0 || x_resolution == 0 || y_resolution == 0 || z_resolution == 0)
		return -1;

	// compress data
	size_t uncompressed_len = 0;
	unsigned char *uncompressed_data = 0;
	
	int ret = EncodeBoolVectToChar(uncompressed_data, data, uncompressed_len);

	if(0 == uncompressed_data || 0 == uncompressed_len || ret != 1)
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		return -1;
	}

	// remove padding added by EncodeBoolVectToChar, if necessary (ie: if res*res*res was not divisible by 8)
	if(data.size() != z_resolution*y_resolution*x_resolution)
		data.resize(z_resolution*y_resolution*x_resolution);

	size_t compressed_len = 0;

	// test to find out how much memory to allocate for compressed buffer
	ret = RLECompress(uncompressed_data, uncompressed_len, 0, compressed_len);

	if(ret != 1)
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

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

		return -1;
	}


	//open voxel file
	ofstream out(file_name, std::ios::binary);

	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		return -2;
	}

	// write header
	out.write((const char *)J4D2_VOXEL_FILE_HDR, static_cast<long unsigned int>(J4D2_VOXEL_FILE_HDR_LEN));
	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

		return -2;
	}

	// write voxel set dimensions and stepsize
	out.write((const char *)&x_min, sizeof(float));
	if(out.fail())
		return -2;

	out.write((const char *)&x_max, sizeof(float));
	if(out.fail())
		return -2;

	out.write((const char *)&y_min, sizeof(float));
	if(out.fail())
		return -2;

	out.write((const char *)&y_max, sizeof(float));
	if(out.fail())
		return -2;

	out.write((const char *)&z_min, sizeof(float));
	if(out.fail())
		return -2;

	out.write((const char *)&z_max, sizeof(float));
	if(out.fail())
		return -2;

	out.write((const char *)&x_resolution, sizeof(long unsigned int));
	if(out.fail())
		return -2;

	out.write((const char *)&y_resolution, sizeof(long unsigned int));
	if(out.fail())
		return -2;

	out.write((const char *)&z_resolution, sizeof(long unsigned int));
	if(out.fail())
		return -2;

	// write uncompressed data length
	out.write((const char *)&uncompressed_len, sizeof(long unsigned int));
	if(out.fail())
	{
		if(0 != uncompressed_data)
			delete [] uncompressed_data;

		if(0 != compressed_data)
			delete [] compressed_data;

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

		return -2;
	}

	delete [] uncompressed_data;
	delete [] compressed_data;




	out.close();

	return 1;
}





int PadAndGetRLELength(vector<bool> &data, size_t &length)
{
	length = 0;

	if(data.size() == 0)
		return -1;

	size_t num_elems_to_add = 8 - (data.size() % 8);

	try
	{
		if(num_elems_to_add < 8)
			for(size_t i = 0; i < num_elems_to_add; i++)
				data.push_back(false);

		size_t data_iter = 0;

		unsigned char cur_data_item = 0;
		cur_data_item  = (data.at(data_iter + 0) << 7);
		cur_data_item += (data.at(data_iter + 1) << 6);
		cur_data_item += (data.at(data_iter + 2) << 5);
		cur_data_item += (data.at(data_iter + 3) << 4);
		cur_data_item += (data.at(data_iter + 4) << 3);
		cur_data_item += (data.at(data_iter + 5) << 2);
		cur_data_item += (data.at(data_iter + 6) << 1);
		cur_data_item += (data.at(data_iter + 7));
		data_iter += 8;

		while(data_iter < data.size())
		{
			size_t repeat_count = 0;

			while(data_iter < data.size())
			{
				unsigned char next_data_item = 0;
				next_data_item  = (data.at(data_iter + 0) << 7);
				next_data_item += (data.at(data_iter + 1) << 6);
				next_data_item += (data.at(data_iter + 2) << 5);
				next_data_item += (data.at(data_iter + 3) << 4);
				next_data_item += (data.at(data_iter + 4) << 3);
				next_data_item += (data.at(data_iter + 5) << 2);
				next_data_item += (data.at(data_iter + 6) << 1);
				next_data_item += (data.at(data_iter + 7));
				data_iter += 8;

				if(cur_data_item == next_data_item)
				{
					repeat_count++;
				}
				else
				{
					cur_data_item = next_data_item;
					break;
				}
			}

			length += 5;
		}
	}
	catch(...)
	{
		return -1;
	}

	return 1;
}








int PadAndWriteToFile(vector<bool> &data, ofstream &out)
{
	if(data.size() == 0)
		return -1;

	size_t num_elems_to_add = 8 - (data.size() % 8);

	try
	{
		if(num_elems_to_add < 8)
			for(size_t i = 0; i < num_elems_to_add; i++)
				data.push_back(false);

		size_t data_iter = 0;

		while(data_iter < data.size())
		{
			unsigned char cur_data_item = EncodeToUnsignedChar(data, data_iter);
			data_iter += 8;

			size_t repeat_count = 0;

			while(data_iter < data.size() && EncodeToUnsignedChar(data, data_iter) == cur_data_item)
			{
				data_iter += 8;
				repeat_count++;
			}

			unsigned char dest_buf[5];

			dest_buf[0] = static_cast<unsigned char>(repeat_count >> 24);
			dest_buf[1] = static_cast<unsigned char>(repeat_count >> 16);
			dest_buf[2] = static_cast<unsigned char>(repeat_count >> 8);
			dest_buf[3] = static_cast<unsigned char>(repeat_count);
			dest_buf[4] = static_cast<unsigned char>(cur_data_item);

			out.write((const char *)dest_buf, sizeof(unsigned char)*5);
		}
	}
	catch(...)
	{
		return -1;
	}

	return 1;
}







int ReadFromFile(vector<bool> &data, ifstream &in, const size_t &uncompressed_length, const size_t &compressed_length)
{
	if(compressed_length % 5 != 0)
		return 0;

	try
	{
		data.reserve(uncompressed_length * 8);
		if(data.capacity() < uncompressed_length * 8)
		{
			data.clear();
			data.reserve(0);
			return -1;
		}

		data.resize(uncompressed_length * 8, false);
		if(data.size() < uncompressed_length * 8)
		{
			data.clear();
			data.reserve(0);
			return -1;
		}
	}
	catch(...)
	{
		return -1;
	}

	try
	{
		long unsigned int compressed_data_pos = 0;
		long unsigned int data_pos = 0;
		long unsigned int vect_size = static_cast<long unsigned int>(data.size());

		while(compressed_data_pos < compressed_length)
		{
			unsigned char dest_buf[5];
			in.read((char *)dest_buf, sizeof(unsigned char) * 5);
			if(in.fail() && !in.eof())
				return -2;

			compressed_data_pos += 5;

			long unsigned int repeat_count  = (dest_buf[0] << 24);
			repeat_count += (dest_buf[1] << 16);
			repeat_count += (dest_buf[2] << 8);
			repeat_count += (dest_buf[3]);

			unsigned char cur_data = dest_buf[4];

			for(long unsigned int i = 0; i < repeat_count+1; i++)
			{
				if(cur_data & (1<<7))
					data.at(data_pos + 0) = true;

				if(cur_data & (1<<6))
					data.at(data_pos + 1) = true;

				if(cur_data & (1<<5))
					data.at(data_pos + 2) = true;

				if(cur_data & (1<<4))
					data.at(data_pos + 3) = true;

				if(cur_data & (1<<3))
					data.at(data_pos + 4) = true;

				if(cur_data & (1<<2))
					data.at(data_pos + 5) = true;

				if(cur_data & (1<<1))
					data.at(data_pos + 6) = true;

				if(cur_data & 1)
					data.at(data_pos + 7) = true;

				data_pos += 8;
			}
		}
	}
	catch(...)
	{
		return -1;
	}

	return 1;
}






