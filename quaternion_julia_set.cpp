#include "stdafx.h"

#include <cmath>
#include <new>
#include <string>
#include <iostream>
using std::endl;
using std::sqrt;
using std::bad_alloc;
using std::nothrow;
using std::string;

#include <vector>
#include <set>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <string>
#include <algorithm>
#include <list>
using std::vector;
using std::ostringstream;
using std::sqrt;
using std::set;
using std::setprecision;
using std::string;
using std::ios_base;
using std::sort;
#include <fstream>
using std::ofstream;
using std::ifstream;
using std::list;

#include <windows.h>
#include <commctrl.h>

#include "quaternion_julia_set.h"
#include "iterators.h"

#include "utilities.h"
#include "volumize_hopf_map.h"

//todo: add a use temporary swap space on hard drive upon low memory condition?


quaternion_julia_set::quaternion_julia_set(const Cjulia4d2Doc &src_params)
{
	internal_params = src_params;
	setup_quaternion_math_class();

	set_iterator_ptr(iterator_ptr);

	bool resized = false;

	if(internal_params.hopf_map)
	{
		if(internal_params.resolution > 255) { internal_params.resolution = 255; resized = true; }
		if(internal_params.resolution < 3) { internal_params.resolution = 3; resized = true; }
		if(true == resized)
		{
			ostringstream out;
			out << "Note: The minimum Hopf map resolution is 3 and the maximum is 255.\nNew resolution setting: " << internal_params.resolution;
			MessageBox(NULL, out.str().c_str(), APP_NAME, MB_OK);
		}
	}
	else
	{
		if(internal_params.resolution > 1625) { internal_params.resolution = 1625; resized = true; }
		if(internal_params.resolution < 3) { internal_params.resolution = 3; resized = true; }
		if(true == resized)
		{
			ostringstream out;
			out << "Note: The minimum resolution is 3 and the maximum is 1625.\nNew resolution setting: " << internal_params.resolution;
			MessageBox(NULL, out.str().c_str(), APP_NAME, MB_OK);
		}
	}

	resized = false;

	if(internal_params.hopf_field_resolution > 1625) { internal_params.hopf_field_resolution = 1625; resized = true; }
	if(internal_params.hopf_field_resolution < 3) { internal_params.hopf_field_resolution = 3; resized = true; }
	if(true == resized)
	{
		ostringstream out;
		out << "Note: The minimum field resolution is 3 and the maximum is 1625.\nNew field resolution setting: " << internal_params.hopf_field_resolution;
		MessageBox(NULL, out.str().c_str(), APP_NAME, MB_OK);
	}

	resized = false;

	if(internal_params.normal_gen_radius > 1625) { internal_params.normal_gen_radius = 1625; resized = true; }
	if(internal_params.normal_gen_radius < 1) { internal_params.normal_gen_radius = 1; resized = true; }
	if(true == resized)
	{
		ostringstream out;
		out << "Note: The minimum normal radius is 1 and the maximum is 1625.\nNew normal radius setting: " << internal_params.normal_gen_radius;
		MessageBox(NULL, out.str().c_str(), APP_NAME, MB_OK);
	}


	// check for 0 length dimensions, adjust if so
	if(internal_params.x_min == internal_params.x_max)
	{
		internal_params.x_min -= 0.0001;
		internal_params.x_max += 0.0001;
	}

	if(internal_params.y_min == internal_params.y_max)
	{
		internal_params.y_min -= 0.0001;
		internal_params.y_max += 0.0001;
	}

	if(internal_params.z_min == internal_params.z_max)
	{
		internal_params.z_min -= 0.0001;
		internal_params.z_max += 0.0001;
	}




	// assume now that all dimension-related parameters contain valid values
	if(internal_params.proportional_res)
	{
		double x_diff = fabs(internal_params.x_max - internal_params.x_min);
		double y_diff = fabs(internal_params.y_max - internal_params.y_min);
		double z_diff = fabs(internal_params.z_max - internal_params.z_min);

		if(internal_params.hopf_map)
		{
			double w_diff = fabs(internal_params.w_max - internal_params.w_min);

			// x is largest dimension
			if(x_diff >= y_diff && x_diff >= z_diff && x_diff >= w_diff)
			{
				// don't bother with fancy rounding, just truncate the answer by casting it back to a long
				internal_params.x_resolution = internal_params.resolution;
				internal_params.y_resolution = static_cast<long unsigned int>((y_diff / x_diff) * static_cast<double>(internal_params.resolution));
				internal_params.z_resolution = static_cast<long unsigned int>((z_diff / x_diff) * static_cast<double>(internal_params.resolution));
				internal_params.w_resolution = static_cast<long unsigned int>((w_diff / x_diff) * static_cast<double>(internal_params.resolution));
			}
			else if(y_diff >= x_diff && y_diff >= z_diff && y_diff >= w_diff)
			{
				internal_params.y_resolution = internal_params.resolution;
				internal_params.x_resolution = static_cast<long unsigned int>((x_diff / y_diff) * static_cast<double>(internal_params.resolution));
				internal_params.z_resolution = static_cast<long unsigned int>((z_diff / y_diff) * static_cast<double>(internal_params.resolution));
				internal_params.w_resolution = static_cast<long unsigned int>((w_diff / y_diff) * static_cast<double>(internal_params.resolution));
			}
			else if(z_diff >= x_diff && z_diff >= y_diff && z_diff >= w_diff)
			{
				internal_params.z_resolution = internal_params.resolution;
				internal_params.x_resolution = static_cast<long unsigned int>((x_diff / z_diff) * static_cast<double>(internal_params.resolution));
				internal_params.y_resolution = static_cast<long unsigned int>((y_diff / z_diff) * static_cast<double>(internal_params.resolution));
				internal_params.w_resolution = static_cast<long unsigned int>((w_diff / z_diff) * static_cast<double>(internal_params.resolution));

			}
			else // w is largest dimension
			{
				internal_params.w_resolution = internal_params.resolution;
				internal_params.x_resolution = static_cast<long unsigned int>((x_diff / w_diff) * static_cast<double>(internal_params.resolution));
				internal_params.y_resolution = static_cast<long unsigned int>((y_diff / w_diff) * static_cast<double>(internal_params.resolution));
				internal_params.z_resolution = static_cast<long unsigned int>((z_diff / w_diff) * static_cast<double>(internal_params.resolution));
			}
		}
		else
		{
			// x is largest dimension
			if(x_diff >= y_diff && x_diff >= z_diff)
			{
				// don't bother with fancy rounding, just truncate the answer by casting it back to a long
				internal_params.x_resolution = internal_params.resolution;
				internal_params.y_resolution = static_cast<long unsigned int>((y_diff / x_diff) * static_cast<double>(internal_params.resolution));
				internal_params.z_resolution = static_cast<long unsigned int>((z_diff / x_diff) * static_cast<double>(internal_params.resolution));
			}
			// y is largest dimension
			else if(y_diff >= x_diff && y_diff >= z_diff)
			{
				internal_params.y_resolution = internal_params.resolution;
				internal_params.x_resolution = static_cast<long unsigned int>((x_diff / y_diff) * static_cast<double>(internal_params.resolution));
				internal_params.z_resolution = static_cast<long unsigned int>((z_diff / y_diff) * static_cast<double>(internal_params.resolution));
			}
			// z is largest dimension
			else
			{
				internal_params.z_resolution = internal_params.resolution;
				internal_params.x_resolution = static_cast<long unsigned int>((x_diff / z_diff) * static_cast<double>(internal_params.resolution));
				internal_params.y_resolution = static_cast<long unsigned int>((y_diff / z_diff) * static_cast<double>(internal_params.resolution));
			}
		}

		if(internal_params.x_resolution < 3)
			internal_params.x_resolution = 3;

		if(internal_params.y_resolution < 3)
			internal_params.y_resolution = 3;

		if(internal_params.z_resolution < 3)
			internal_params.z_resolution = 3;

		if(internal_params.w_resolution < 3)
			internal_params.w_resolution = 3;
	}
	else
	{
		internal_params.x_resolution = internal_params.resolution;
		internal_params.y_resolution = internal_params.resolution;
		internal_params.z_resolution = internal_params.resolution;
		internal_params.w_resolution = internal_params.resolution;
	}
}





string quaternion_julia_set::GetMulVarText(const int &mul_var)
{
	string var_string;

	switch(mul_var)
	{
	case MUL_PLUS_X:
		{
			var_string = "+x";
			break;
		}
	case MUL_PLUS_Y:
		{
			var_string = "+y";
			break;
		}
	case MUL_PLUS_Z:
		{
			var_string = "+z";
			break;
		}
	case MUL_PLUS_W:
		{
			var_string = "+w";
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
	}

	return var_string;
}


void quaternion_julia_set::setup_quaternion_math_class(void)
{
	if(internal_params.mul_column_type == MUL_COLUMN_DEFAULT)
	{
		if(internal_params.mul_matrix_type == MUL_MATRIX_DEFAULT)
		{
			// default, no setup required
			q_math = &q_math_default;
			return;

		}
		else if(internal_params.mul_matrix_type == MUL_MATRIX_CUSTOM)
		{
			// custom, setup using default column, custom matrix
			q_math = &q_math_custom;

			if(!q_math->setup_custom_column("+x", "+y", "+z", "+w"))
			{
				MessageBox(NULL, "Error setting up custom quaternion math library. Falling back to default library.", APP_NAME, MB_OK);
				q_math = &q_math_default;
				return;
			}

			if(!q_math->setup_custom_matrix(
				GetMulVarText(internal_params.mul_matrix_custom_11), GetMulVarText(internal_params.mul_matrix_custom_12), GetMulVarText(internal_params.mul_matrix_custom_13), GetMulVarText(internal_params.mul_matrix_custom_14),		// custom matrix row 1
				GetMulVarText(internal_params.mul_matrix_custom_21), GetMulVarText(internal_params.mul_matrix_custom_22), GetMulVarText(internal_params.mul_matrix_custom_23), GetMulVarText(internal_params.mul_matrix_custom_24),		// custom matrix row 2
				GetMulVarText(internal_params.mul_matrix_custom_31), GetMulVarText(internal_params.mul_matrix_custom_32), GetMulVarText(internal_params.mul_matrix_custom_33), GetMulVarText(internal_params.mul_matrix_custom_34),		// custom matrix row 3
				GetMulVarText(internal_params.mul_matrix_custom_41), GetMulVarText(internal_params.mul_matrix_custom_42), GetMulVarText(internal_params.mul_matrix_custom_43), GetMulVarText(internal_params.mul_matrix_custom_44)))	// custom matrix row 4 
			{
				MessageBox(NULL, "Error setting up custom quaternion math library. Falling back to default library.", APP_NAME, MB_OK);
				q_math = &q_math_default;
				return;
			}

			return;
		}
	}
	else if(internal_params.mul_column_type == MUL_COLUMN_CONST_VALS)
	{
		if(internal_params.mul_matrix_type == MUL_MATRIX_DEFAULT)
		{
			// constant_column
			q_math = &q_math_constant_column;

			q_math->setup_constant_column_values(
				internal_params.mul_column_const_vals_x, 
				internal_params.mul_column_const_vals_y, 
				internal_params.mul_column_const_vals_z, 
				internal_params.mul_column_const_vals_w);

			return;
		}
		else if(internal_params.mul_matrix_type == MUL_MATRIX_CUSTOM)
		{
			// custom_constant_column
			q_math = &q_math_custom_constant_column;
			q_math->setup_constant_column_values(
				internal_params.mul_column_const_vals_x, 
				internal_params.mul_column_const_vals_y, 
				internal_params.mul_column_const_vals_z, 
				internal_params.mul_column_const_vals_w);

			if(!q_math->setup_custom_matrix(
				GetMulVarText(internal_params.mul_matrix_custom_11), GetMulVarText(internal_params.mul_matrix_custom_12), GetMulVarText(internal_params.mul_matrix_custom_13), GetMulVarText(internal_params.mul_matrix_custom_14),		// custom matrix row 1
				GetMulVarText(internal_params.mul_matrix_custom_21), GetMulVarText(internal_params.mul_matrix_custom_22), GetMulVarText(internal_params.mul_matrix_custom_23), GetMulVarText(internal_params.mul_matrix_custom_24),		// custom matrix row 2
				GetMulVarText(internal_params.mul_matrix_custom_31), GetMulVarText(internal_params.mul_matrix_custom_32), GetMulVarText(internal_params.mul_matrix_custom_33), GetMulVarText(internal_params.mul_matrix_custom_34),		// custom matrix row 3
				GetMulVarText(internal_params.mul_matrix_custom_41), GetMulVarText(internal_params.mul_matrix_custom_42), GetMulVarText(internal_params.mul_matrix_custom_43), GetMulVarText(internal_params.mul_matrix_custom_44)))	// custom matrix row 4 
			{
				MessageBox(NULL, "Error setting up custom quaternion math library. Falling back to default library.", APP_NAME, MB_OK);
				q_math = &q_math_default;
				return;
			}

			return;
		}
	}
	else if(internal_params.mul_column_type == MUL_COLUMN_CUSTOM)
	{
		if(internal_params.mul_matrix_type == MUL_MATRIX_DEFAULT)
		{
			// custom
			q_math = &q_math_custom;

			if(!q_math->setup_custom_column(
				GetMulVarText(internal_params.mul_column_custom_x),
				GetMulVarText(internal_params.mul_column_custom_y),
				GetMulVarText(internal_params.mul_column_custom_z),
				GetMulVarText(internal_params.mul_column_custom_w)))
			{
				MessageBox(NULL, "Error setting up custom quaternion math library. Falling back to default library.", APP_NAME, MB_OK);
				q_math = &q_math_default;
				return;
			}

			if(!q_math->setup_custom_matrix(
				"+x", "-y", "-z", "-w",		// custom matrix row 1
				"+y", "+x", "+w", "-z",		// custom matrix row 2
				"+z", "-w", "+x", "+y",		// custom matrix row 3
				"+w", "+z", "-y", "+x"))	// custom matrix row 4 
			{
				MessageBox(NULL, "Error setting up custom quaternion math library. Falling back to default library.", APP_NAME, MB_OK);
				q_math = &q_math_default;
				return;
			}

			return;
		}
		else if(internal_params.mul_matrix_type == MUL_MATRIX_CUSTOM)
		{
			// custom
			q_math = &q_math_custom;

			if(!q_math->setup_custom_column(
				GetMulVarText(internal_params.mul_column_custom_x),
				GetMulVarText(internal_params.mul_column_custom_y),
				GetMulVarText(internal_params.mul_column_custom_z),
				GetMulVarText(internal_params.mul_column_custom_w)))
			{
				MessageBox(NULL, "Error setting up custom quaternion math library. Falling back to default library.", APP_NAME, MB_OK);
				q_math = &q_math_default;
				return;
			}

			if(!q_math->setup_custom_matrix(
				GetMulVarText(internal_params.mul_matrix_custom_11), GetMulVarText(internal_params.mul_matrix_custom_12), GetMulVarText(internal_params.mul_matrix_custom_13), GetMulVarText(internal_params.mul_matrix_custom_14),		// custom matrix row 1
				GetMulVarText(internal_params.mul_matrix_custom_21), GetMulVarText(internal_params.mul_matrix_custom_22), GetMulVarText(internal_params.mul_matrix_custom_23), GetMulVarText(internal_params.mul_matrix_custom_24),		// custom matrix row 2
				GetMulVarText(internal_params.mul_matrix_custom_31), GetMulVarText(internal_params.mul_matrix_custom_32), GetMulVarText(internal_params.mul_matrix_custom_33), GetMulVarText(internal_params.mul_matrix_custom_34),		// custom matrix row 3
				GetMulVarText(internal_params.mul_matrix_custom_41), GetMulVarText(internal_params.mul_matrix_custom_42), GetMulVarText(internal_params.mul_matrix_custom_43), GetMulVarText(internal_params.mul_matrix_custom_44)))	// custom matrix row 4 
			{
				MessageBox(NULL, "Error setting up custom quaternion math library. Falling back to default library.", APP_NAME, MB_OK);
				q_math = &q_math_default;
				return;
			}

			return;
		}
	}
}


void quaternion_julia_set::set_iterator_ptr(ip &iterator_ptr)
{
	switch(internal_params.formula)
	{
	case CUSTOM_ITERATOR:
		{
			// for now
			iterator_ptr = &quaternion_julia_set::custom_iter;
			break;
		}

	case CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::classic_iter;
			break;
		}

	case EXPONENTIAL_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::exponential_iter;
			break;
		}

	case QUADRATIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::quadratic_iter;
			break;
		}

	case LAMBDA_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::lambda_iter;
			break;
		}

	case FUNKY_BLOBS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::funky_blobs_iter;
			break;
		}

	case SINE_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::sine_iter;
			break;
		}

	case SINE_PLUS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::sine_plus_iter;
			break;
		}

	case SINE_CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::sine_classic_iter;
			break;
		}

	case COSINE_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::cosine_iter;
			break;
		}

	case COSINE_PLUS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::cosine_plus_iter;
			break;
		}

	case COSINE_CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::cosine_classic_iter;
			break;
		}

	case TANGENT_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::tangent_iter;
			break;
		}

	case TANGENT_PLUS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::tangent_plus_iter;
			break;
		}

	case TANGENT_CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::tangent_classic_iter;
			break;
		}

	case COSECANT_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::cosecant_iter;
			break;
		}

	case COSECANT_PLUS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::cosecant_plus_iter;
			break;
		}

	case COSECANT_CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::cosecant_classic_iter;
			break;
		}

	case SECANT_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::secant_iter;
			break;
		}

	case SECANT_PLUS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::secant_plus_iter;
			break;
		}

	case SECANT_CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::secant_classic_iter;
			break;
		}

	case COTANGENT_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::cotangent_iter;
			break;
		}

	case COTANGENT_PLUS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::cotangent_plus_iter;
			break;
		}

	case COTANGENT_CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::cotangent_classic_iter;
			break;
		}

	case HYPERBOLIC_SINE_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_sine_iter;
			break;
		}

	case HYPERBOLIC_SINE_PLUS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_sine_plus_iter;
			break;
		}

	case HYPERBOLIC_SINE_CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_sine_classic_iter;
			break;
		}

	case HYPERBOLIC_COSINE_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_cosine_iter;
			break;
		}

	case HYPERBOLIC_COSINE_PLUS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_cosine_plus_iter;
			break;
		}

	case HYPERBOLIC_COSINE_CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_cosine_classic_iter;
			break;
		}

	case HYPERBOLIC_TANGENT_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_tangent_iter;
			break;
		}

	case HYPERBOLIC_TANGENT_PLUS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_tangent_plus_iter;
			break;
		}

	case HYPERBOLIC_TANGENT_CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_tangent_classic_iter;
			break;
		}

	case HYPERBOLIC_COSECANT_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_cosecant_iter;
			break;
		}

	case HYPERBOLIC_COSECANT_PLUS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_cosecant_plus_iter;
			break;
		}

	case HYPERBOLIC_COSECANT_CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_cosecant_classic_iter;
			break;
		}

	case HYPERBOLIC_SECANT_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_secant_iter;
			break;
		}

	case HYPERBOLIC_SECANT_PLUS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_secant_plus_iter;
			break;
		}

	case HYPERBOLIC_SECANT_CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_secant_classic_iter;
			break;
		}

	case HYPERBOLIC_COTANGENT_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_cotangent_iter;
			break;
		}

	case HYPERBOLIC_COTANGENT_PLUS_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_cotangent_plus_iter;
			break;
		}

	case HYPERBOLIC_COTANGENT_CLASSIC_ITERATOR:
		{
			iterator_ptr = &quaternion_julia_set::hyperbolic_cotangent_classic_iter;
			break;
		}

	default:
		{
			iterator_ptr = &quaternion_julia_set::classic_iter;
			break;
		}
	}
}





int quaternion_julia_set::GetPointSet(const HWND &progressbar, const HWND &status_text, POS_NORMAL_COLOUR_VERTEX *&vertices, long unsigned int &vertex_count, const HANDLE &cancel_event)
{
	// automatically lose pointer to renderer owned memory, so we don't accidentally try to delete renderer owned memory later
	// ... the renderer will automatically delete the renderer owned memory on its own, when it no longer needs the memory for itself
	vertices = 0;

	SendMessage(progressbar, PBM_SETPOS, 0, 0);




	const double x_step_size = (internal_params.x_max - internal_params.x_min) / (internal_params.x_resolution - 1);
	const double y_step_size = (internal_params.y_max - internal_params.y_min) / (internal_params.y_resolution - 1);
	const double z_step_size = (internal_params.z_max - internal_params.z_min) / (internal_params.z_resolution - 1);

	long unsigned int num_steps_total = internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution;
	long unsigned int total_count = 0;
	unsigned short int percent_done = 0;
	unsigned short int last_percent_done = 0;

	quaternion_t<double> Z(internal_params.x_max, internal_params.y_max, internal_params.z_max, internal_params.z_w);
	quaternion_t<double> C(internal_params.c_x, internal_params.c_y, internal_params.c_z, internal_params.c_w);

	cache_data set_data;

	if(0 != status_text)
	{
		SetWindowText(status_text, "Checking cache...");
		UpdateWindow(status_text);
	}

	int ret = cache.GetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, status_text);

	if(ret != 1)
	{
		if(ret == -1)
			MessageBox(0, "Insufficient memory was available to read set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(ret == -2)
			MessageBox(0, "A file error occurred while reading set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		set_data.w_step_number = set_data.x_step_number = set_data.y_step_number = set_data.z_step_number = 0;
		try { set_data.data.resize(internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution); } catch(...) { return -1; }
	}
	else
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		for(long unsigned int temp_z_step_number = 0; temp_z_step_number < set_data.z_step_number; temp_z_step_number++)
			Z.z -= z_step_size;

		for(long unsigned int temp_y_step_number = 0; temp_y_step_number < set_data.y_step_number; temp_y_step_number++)
			Z.y -= y_step_size;

		for(long unsigned int temp_x_step_number = 0; temp_x_step_number < set_data.x_step_number; temp_x_step_number++)
			Z.x -= x_step_size;

		total_count = set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;
		percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);
	}

	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	double length = 0.0f;

	quaternion_t<double> *final_z;
	quaternion_t<double> *final_c;

	if(false == internal_params.swap_z_and_c)
	{
		final_z = &Z;
		final_c = &C;
	}
	else
	{
		final_z = &C;
		final_c = &Z;
	}

	bool cancelled = false;

	for(; set_data.z_step_number < internal_params.z_resolution; set_data.z_step_number++, Z.z -= z_step_size, Z.y = internal_params.y_max, set_data.y_step_number = 0)
	{
		for(; set_data.y_step_number < internal_params.y_resolution; set_data.y_step_number++, Z.y -= y_step_size, Z.x = internal_params.x_max, set_data.x_step_number = 0)
		{
			for(; set_data.x_step_number < internal_params.x_resolution; set_data.x_step_number++, Z.x -= x_step_size)
			{
				long unsigned int index = set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;

				if(internal_params.infinity > (length = (this->*iterator_ptr)(*final_z, *final_c, internal_params.iterations, internal_params.infinity, internal_params.a)))
					set_data.data[index] = true;
				else
					set_data.data[index] = false;

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

				if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
				{
					int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_Z_MAJOR_PLANE, status_text);

					if(ret != 1)
					{
						if(ret == -1)
							MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK);

						if(ret == -2)
							MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK);
					}

					set_data.z_step_number = internal_params.z_resolution;
					set_data.y_step_number = internal_params.y_resolution;
					set_data.x_step_number = internal_params.x_resolution;
					cancelled = true;
				}
			}
		}
	}


	if(!cancelled)
	{
		int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_Z_MAJOR_PLANE, status_text);

		if(ret != 1)
		{
			if(ret == -1)
				MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK);

			if(ret == -2)
				MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK);
		}
	}





	// reverse set if enabled
	if(!cancelled && internal_params.reverse_set)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Reversing set...");
			UpdateWindow(status_text);
		}

		num_steps_total = internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution;
		total_count = 0;
		last_percent_done = 0;
		percent_done = 0;

		for(long unsigned int z = 0; z < internal_params.z_resolution; z++)
		{
			for(long unsigned int y = 0; y < internal_params.y_resolution; y++)
			{
				for(long unsigned int x = 0; x < internal_params.x_resolution; x++)
				{
					long unsigned int index = z*internal_params.y_resolution*internal_params.x_resolution + y*internal_params.x_resolution + x;
					set_data.data[index] = !set_data.data[index];

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

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						z = internal_params.z_resolution;
						y = internal_params.y_resolution;
						x = internal_params.x_resolution;
						cancelled = true;
					}
				}
			}
		}
	}



//	vector<bool> is_surface;
	cache_data is_surface;

	// cull non-surface points if enabled
	if(!cancelled && internal_params.cull_non_surface_points)
	{
		if(0 != progressbar)
			SendMessage(progressbar, PBM_SETPOS, 0, 0);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Checking cache...");
			UpdateWindow(status_text);
		}

		num_steps_total = internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution;
		total_count = 0;
		last_percent_done = 0;
		percent_done = 0;

		int ret = cache.GetCacheData(internal_params, is_surface, J4D2_CACHE_SURFACE_NONSAMPLEDTYPE, status_text);

		if(ret != 1)
		{
			if(ret == -1)
				MessageBox(0, "Insufficient memory was available to read surface data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

			if(ret == -2)
				MessageBox(0, "A file error occurred while reading surface data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

			if(0 != status_text)
			{
				SetWindowText(status_text, "Tracing 3D surface...");
				UpdateWindow(status_text);
			}

			is_surface.w_step_number = is_surface.x_step_number = is_surface.y_step_number = is_surface.z_step_number = 0;
			try { is_surface.data.resize(internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution); } catch(...) { return -1; }
		}
		else
		{
			if(0 != status_text)
			{
				SetWindowText(status_text, "Tracing 3D surface...");
				UpdateWindow(status_text);
			}

			total_count = is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + is_surface.x_step_number;
			percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);
		}

		for(; is_surface.z_step_number < internal_params.z_resolution; is_surface.z_step_number++, is_surface.y_step_number = 0)
		{
			for(; is_surface.y_step_number < internal_params.y_resolution; is_surface.y_step_number++, is_surface.x_step_number = 0)
			{
				for(; is_surface.x_step_number < internal_params.x_resolution; is_surface.x_step_number++)
				{
					// these indices were completely backwards (don't take back medication when programming...)
					long unsigned int index = is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + is_surface.x_step_number;

					// if it's on, test if all 6 immediate neighbours are also on (if it's not on the edge)
					if(true == set_data.data[index])
					{
						if( is_surface.z_step_number != 0 && is_surface.z_step_number != internal_params.z_resolution - 1 &&
							is_surface.y_step_number != 0 && is_surface.y_step_number != internal_params.y_resolution - 1 &&
							is_surface.x_step_number != 0 && is_surface.x_step_number != internal_params.x_resolution - 1 &&
							true == set_data.data[(is_surface.z_step_number-1)*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + is_surface.x_step_number] && 
							true == set_data.data[(is_surface.z_step_number+1)*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + is_surface.x_step_number] && 
							true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + (is_surface.y_step_number-1)*internal_params.x_resolution + is_surface.x_step_number] && 
							true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + (is_surface.y_step_number+1)*internal_params.x_resolution + is_surface.x_step_number] && 
							true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + (is_surface.x_step_number-1)] && 
							true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + (is_surface.x_step_number+1)])

						{
							is_surface.data[index] = false;
						}
						else
						{
							// don't include voxels with no neighbours
							//							if(internal_params.light_by_normals)
							//							{
							unsigned int neighbour_count = 0;

							if(is_surface.z_step_number != 0)
								if(true == set_data.data[(is_surface.z_step_number-1)*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + is_surface.x_step_number])
									neighbour_count++;

							if(is_surface.z_step_number != internal_params.z_resolution-1)
								if(true == set_data.data[(is_surface.z_step_number+1)*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + is_surface.x_step_number])
									neighbour_count++;

							if(is_surface.y_step_number != 0)
								if(true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + (is_surface.y_step_number-1)*internal_params.x_resolution + is_surface.x_step_number])
									neighbour_count++;

							if(is_surface.y_step_number != internal_params.y_resolution-1)
								if(true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + (is_surface.y_step_number+1)*internal_params.x_resolution + is_surface.x_step_number])
									neighbour_count++;

							if(is_surface.x_step_number != 0)
								if(true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + (is_surface.x_step_number-1)])
									neighbour_count++;

							if(is_surface.x_step_number != internal_params.x_resolution-1)
								if(true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + (is_surface.x_step_number+1)])
									neighbour_count++;

							if(neighbour_count == 0)
								is_surface.data[index] = false;
							else
								is_surface.data[index] = true;
						}
					}
					else
					{
						is_surface.data[index] = false;
					}

					total_count++;

					if(0 != progressbar)
					{
						percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

						if(percent_done > last_percent_done || percent_done == 100)
						{
							last_percent_done = percent_done;
							SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
							//						Sleep(1);
						}
					}

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						int ret = cache.SetCacheData(internal_params, is_surface, J4D2_CACHE_SURFACE_NONSAMPLEDTYPE, J4D2_CACHE_Z_MAJOR_PLANE, status_text);

						if(ret != 1)
						{
							if(ret == -1)
								MessageBox(0, "Insufficient memory was available to write surface data to the cache.", APP_NAME, MB_OK);

							if(ret == -2)
								MessageBox(0, "A file error occurred while writing surface data to the cache.", APP_NAME, MB_OK);
						}

						is_surface.z_step_number = internal_params.z_resolution;
						is_surface.y_step_number = internal_params.y_resolution;
						is_surface.x_step_number = internal_params.x_resolution;
						cancelled = true;
					}
				}
			}
		}

		if(!cancelled)
		{
			int ret = cache.SetCacheData(internal_params, is_surface, J4D2_CACHE_SURFACE_NONSAMPLEDTYPE, J4D2_CACHE_Z_MAJOR_PLANE, status_text);

			if(ret != 1)
			{
				if(ret == -1)
					MessageBox(0, "Insufficient memory was available to write surface data to the cache.", APP_NAME, MB_OK);

				if(ret == -2)
					MessageBox(0, "A file error occurred while writing surface data to the cache.", APP_NAME, MB_OK);
			}
		}
	}


	vector<bool> *data_ptr = 0;

	if(false == internal_params.cull_non_surface_points)
		data_ptr = &set_data.data;
	else
		data_ptr = &is_surface.data;

	ofstream of("fluid.txt");

	of << internal_params.x_resolution << endl;
	of << internal_params.y_resolution << endl;
	of << internal_params.z_resolution << endl;

	if(!cancelled)
	{
		if(0 != progressbar)
			SendMessage(progressbar, PBM_SETPOS, 0, 0);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Generating vertices...");
			UpdateWindow(status_text);
		}

		vertex_count = 0;

		for(vector<bool>::const_iterator i = data_ptr->begin(); i != data_ptr->end(); i++)
		{
			if(*i == true)
			{
				of << 1.0f << endl;
				vertex_count++;
			}
			else
			{
				of << 0.0f << endl;
			}
		}

		if(0 == vertex_count)
		{
			if(0 != vertices)
			{
				delete [] vertices;
				vertices = 0;
			}
		}
		else
		{
			try
			{
				vertices = new POS_NORMAL_COLOUR_VERTEX[vertex_count];
			}
			catch(...)
			{
				vertex_count = 0;

				if(vertices != 0)
				{
					delete [] vertices;
					vertices = 0;
				}

				return -1;
			}

			long unsigned int vertex_index = 0;


			quaternion_t<double> Z(internal_params.x_max, internal_params.y_max, internal_params.z_max, internal_params.z_w);

			total_count = 0;
			percent_done = 0;
			last_percent_done = 0;

			for(long unsigned int z_step_number = 0; z_step_number < internal_params.z_resolution; z_step_number++, Z.z -= z_step_size, Z.y = internal_params.y_max)
			{
				for(long unsigned int y_step_number = 0; y_step_number < internal_params.y_resolution; y_step_number++, Z.y -= y_step_size, Z.x = internal_params.x_max)
				{
					for(long unsigned int x_step_number = 0; x_step_number < internal_params.x_resolution; x_step_number++, Z.x -= x_step_size)
					{
						long unsigned int index = z_step_number*internal_params.y_resolution*internal_params.x_resolution + y_step_number*internal_params.x_resolution + x_step_number;

						if(true == (*data_ptr)[index])
						{
							vertices[vertex_index].x = static_cast<float>(Z.x);
							vertices[vertex_index].y = static_cast<float>(Z.y);
							vertices[vertex_index].z = static_cast<float>(Z.z);

							vertices[vertex_index].nx = 0.0f;
							vertices[vertex_index].ny = 0.0f;
							vertices[vertex_index].nz = 0.0f;

							signed int x_min_radius = 0;
							signed int x_max_radius = 0;
							signed int y_min_radius = 0;
							signed int y_max_radius = 0;
							signed int z_min_radius = 0;
							signed int z_max_radius = 0;

							if(x_step_number < internal_params.normal_gen_radius)
								x_min_radius = -static_cast<signed int>(x_step_number);
							else
								x_min_radius = -static_cast<signed int>(internal_params.normal_gen_radius);

							if(internal_params.x_resolution - x_step_number - 1 < internal_params.normal_gen_radius)
								x_max_radius = static_cast<signed int>(internal_params.x_resolution - x_step_number - 1);
							else
								x_max_radius = internal_params.normal_gen_radius;

							if(y_step_number < internal_params.normal_gen_radius)
								y_min_radius = -static_cast<signed int>(y_step_number);
							else
								y_min_radius = -static_cast<signed int>(internal_params.normal_gen_radius);

							if(internal_params.y_resolution - y_step_number - 1 < internal_params.normal_gen_radius)
								y_max_radius = static_cast<signed int>(internal_params.y_resolution - y_step_number - 1);
							else
								y_max_radius = internal_params.normal_gen_radius;

							if(z_step_number < internal_params.normal_gen_radius)
								z_min_radius = -static_cast<signed int>(z_step_number);
							else
								z_min_radius = -static_cast<signed int>(internal_params.normal_gen_radius);

							if(internal_params.z_resolution - z_step_number - 1 < internal_params.normal_gen_radius)
								z_max_radius = static_cast<signed int>(internal_params.z_resolution - z_step_number - 1);
							else
								z_max_radius = internal_params.normal_gen_radius;


							for(signed int z = z_min_radius; z <= z_max_radius; z++)
							{
								for(signed int y = y_min_radius; y <= y_max_radius; y++)
								{
									for(signed int x = x_min_radius; x <= x_max_radius; x++)
									{
										long unsigned int temp_x = x_step_number;
										long unsigned int temp_y = y_step_number;
										long unsigned int temp_z = z_step_number;

										if(x < 0)
											temp_x -= abs(x);
										else
											temp_x += x;

										if(y < 0)
											temp_y -= abs(y);
										else
											temp_y += y;

										if(z < 0)
											temp_z -= abs(z);
										else
											temp_z += z;

										if(temp_x == x_step_number &&
											temp_y == y_step_number &&
											temp_z == z_step_number)
											continue;


										if(true == set_data.data[temp_z*internal_params.y_resolution*internal_params.x_resolution + temp_y*internal_params.x_resolution + temp_x])
										{
											float local_x = static_cast<float>(x);
											float local_y = static_cast<float>(y);
											float local_z = static_cast<float>(z);
											float len = sqrt(local_x*local_x + local_y*local_y + local_z*local_z);

											if(len != 1.0)
											{
												local_x /= len;
												local_y /= len;
												local_z /= len;
											}

											vertices[vertex_index].nx += local_x;
											vertices[vertex_index].ny += local_y;
											vertices[vertex_index].nz += local_z;
										}
									}
								}
							}



							float len = sqrt(vertices[vertex_index].nx*vertices[vertex_index].nx + vertices[vertex_index].ny*vertices[vertex_index].ny + vertices[vertex_index].nz*vertices[vertex_index].nz);

							if(len <= 0.1)
							{
								vertices[vertex_index].nx = 0.0f;
								vertices[vertex_index].ny = 0.0f;
								vertices[vertex_index].nz = -1.0f;
							}
							else if(len != 1.0)
							{
								vertices[vertex_index].nx /= len;
								vertices[vertex_index].ny /= len;
								vertices[vertex_index].nz /= len;
							}

							vertex_index++;
						}

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

						if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
						{
							z_step_number = internal_params.z_resolution;
							y_step_number = internal_params.y_resolution;
							x_step_number = internal_params.x_resolution;
							cancelled = true;
						}
					}
				}
			}

		}
	}

	if(cancelled == true)
	{
		if(0 != vertices)
			delete [] vertices;

		vertex_count = 0;

		return 0;
	}
	else
	{
		return 1;
	}
}








int quaternion_julia_set::GetHopfMap(const HWND &progressbar, const HWND &status_text, POS_NORMAL_COLOUR_VERTEX *&vertices, long unsigned int &vertex_count, const HANDLE &cancel_event)
{
	// automatically lose pointer to renderer owned memory, so we don't accidentally try to delete renderer owned memory later
	vertices = 0;

	SendMessage(progressbar, PBM_SETPOS, 0, 0);



	const double x_step_size = (internal_params.x_max - internal_params.x_min) / (internal_params.x_resolution - 1);
	const double y_step_size = (internal_params.y_max - internal_params.y_min) / (internal_params.y_resolution - 1);
	const double z_step_size = (internal_params.z_max - internal_params.z_min) / (internal_params.z_resolution - 1);
	const double w_step_size = (internal_params.w_max - internal_params.w_min) / (internal_params.w_resolution - 1);

	long unsigned int num_steps_total = internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution * internal_params.w_resolution;
	long unsigned int total_count = 0;
	unsigned short int percent_done = 0;
	unsigned short int last_percent_done = 0;

	quaternion_t<double> Z(internal_params.x_max, internal_params.y_max, internal_params.z_max, internal_params.w_max);
	quaternion_t<double> C(internal_params.c_x, internal_params.c_y, internal_params.c_z, internal_params.c_w);

	cache_data set_data;

	if(0 != status_text)
	{
		SetWindowText(status_text, "Checking cache...");
		UpdateWindow(status_text);
	}


	int ret = cache.GetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, status_text);

	if(ret != 1)
	{
		if(ret == -1)
			MessageBox(0, "Insufficient memory was available to read set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(ret == -2)
			MessageBox(0, "A file error occurred while reading set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		set_data.w_step_number = set_data.x_step_number = set_data.y_step_number = set_data.z_step_number = 0;
		try { set_data.data.resize(internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution * internal_params.w_resolution); } catch(...) { return -1; }
	}
	else
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		for(long unsigned int temp_w_step_number = 0; temp_w_step_number < set_data.w_step_number; temp_w_step_number++)
			Z.w -= w_step_size;

		for(long unsigned int temp_z_step_number = 0; temp_z_step_number < set_data.z_step_number; temp_z_step_number++)
			Z.z -= z_step_size;

		for(long unsigned int temp_y_step_number = 0; temp_y_step_number < set_data.y_step_number; temp_y_step_number++)
			Z.y -= y_step_size;

		for(long unsigned int temp_x_step_number = 0; temp_x_step_number < set_data.x_step_number; temp_x_step_number++)
			Z.x -= x_step_size;

		total_count = set_data.w_step_number*internal_params.z_resolution*internal_params.y_resolution*internal_params.x_resolution + set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;
		percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);
	}


	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	double length = 0.0f;

	quaternion_t<double> *final_z;
	quaternion_t<double> *final_c;

	if(false == internal_params.swap_z_and_c)
	{
		final_z = &Z;
		final_c = &C;
	}
	else
	{
		final_z = &C;
		final_c = &Z;
	}

	bool cancelled = false;

	for(; set_data.w_step_number < internal_params.w_resolution; set_data.w_step_number++, Z.w -= w_step_size, Z.z = internal_params.z_max, set_data.z_step_number = 0)
	{
		for(; set_data.z_step_number < internal_params.z_resolution; set_data.z_step_number++, Z.z -= z_step_size, Z.y = internal_params.y_max, set_data.y_step_number = 0)
		{
			for(; set_data.y_step_number < internal_params.y_resolution; set_data.y_step_number++, Z.y -= y_step_size, Z.x = internal_params.x_max, set_data.x_step_number = 0)
			{
				for(; set_data.x_step_number < internal_params.x_resolution; set_data.x_step_number++, Z.x -= x_step_size)
				{
					long unsigned int index = set_data.w_step_number*internal_params.z_resolution*internal_params.y_resolution*internal_params.x_resolution + set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;

					if(internal_params.infinity > (length = (this->*iterator_ptr)(*final_z, *final_c, internal_params.iterations, internal_params.infinity, internal_params.a)))
						set_data.data[index] = true;
					else
						set_data.data[index] = false;

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

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_W_MAJOR_PLANE, status_text);

						if(ret != 1)
						{
							if(ret == -1)
								MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

							if(ret == -2)
								MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
						}


						set_data.z_step_number = internal_params.z_resolution;
						set_data.y_step_number = internal_params.y_resolution;
						set_data.x_step_number = internal_params.x_resolution;
						set_data.w_step_number = internal_params.w_resolution;
						cancelled = true;
					}
				}
			}
		}
	}


	if(!cancelled)
	{
		int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_W_MAJOR_PLANE, status_text);

		if(ret != 1)
		{
			if(ret == -1)
				MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

			if(ret == -2)
				MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
		}

	}






	vector<bool> voxels;
	dimension_info d_info;

	if(!cancelled)
	{
		volumize_hopf_map vhm;
		int ret = vhm.VoxelizeHopfMap(internal_params, set_data.data, voxels, d_info, progressbar, status_text, cancel_event);
		if(ret != 1)
			return ret;
	}


	// clear out the bool vector that we don't need anymore
	if(!cancelled)
	{
		vector<bool> temp_data;
		set_data.data = temp_data;
	}




	vector<bool> is_surface;

	// cull non-surface points if enabled
	if(!cancelled && internal_params.cull_non_surface_points)
	{
		if(0 != progressbar)
			SendMessage(progressbar, PBM_SETPOS, 0, 0);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Tracing 3D surface...");
			UpdateWindow(status_text);
		}

		num_steps_total = internal_params.hopf_field_resolution * internal_params.hopf_field_resolution * internal_params.hopf_field_resolution;
		total_count = 0;
		last_percent_done = 0;
		percent_done = 0;


		try { is_surface.resize(num_steps_total); } catch(...) { return -1; }

		for(long unsigned int z = 0; z < internal_params.hopf_field_resolution; z++)
		{
			for(long unsigned int y = 0; y < internal_params.hopf_field_resolution; y++)
			{
				for(long unsigned int x = 0; x < internal_params.hopf_field_resolution; x++)
				{
					// these indices were completely backwards (don't take back medication when programming...)
					//long unsigned int index = z + y*internal_params.hopf_field_resolution + x*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution;
					long unsigned int index = z*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + y*internal_params.hopf_field_resolution + x;

					// if it's on, test if all 6 immediate neighbours are also on (if it's not on the edge)
					if(true == voxels[index])
					{
						if( z != 0 && z != internal_params.hopf_field_resolution - 1 &&
							y != 0 && y != internal_params.hopf_field_resolution - 1 &&
							x != 0 && x != internal_params.hopf_field_resolution - 1 &&
							true == voxels[(z-1)*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + y*internal_params.hopf_field_resolution + x] && 
							true == voxels[(z+1)*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + y*internal_params.hopf_field_resolution + x] && 
							true == voxels[z*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + (y-1)*internal_params.hopf_field_resolution + x] && 
							true == voxels[z*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + (y+1)*internal_params.hopf_field_resolution + x] && 
							true == voxels[z*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + y*internal_params.hopf_field_resolution + (x-1)] && 
							true == voxels[z*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + y*internal_params.hopf_field_resolution + (x+1)])

						{
							is_surface[index] = false;
						}
						else
						{
							// don't include voxels with no neighbours
							//							if(internal_params.light_by_normals)
							//							{
							unsigned int neighbour_count = 0;

							if(z != 0)
								if(true == voxels[(z-1)*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + y*internal_params.hopf_field_resolution + x])
									neighbour_count++;

							if(z != internal_params.hopf_field_resolution-1)
								if(true == voxels[(z+1)*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + y*internal_params.hopf_field_resolution + x])
									neighbour_count++;

							if(y != 0)
								if(true == voxels[z*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + (y-1)*internal_params.hopf_field_resolution + x])
									neighbour_count++;

							if(y != internal_params.hopf_field_resolution-1)
								if(true == voxels[z*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + (y+1)*internal_params.hopf_field_resolution + x])
									neighbour_count++;

							if(x != 0)
								if(true == voxels[z*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + y*internal_params.hopf_field_resolution + (x-1)])
									neighbour_count++;

							if(x != internal_params.hopf_field_resolution-1)
								if(true == voxels[z*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + y*internal_params.hopf_field_resolution + (x+1)])
									neighbour_count++;

							if(neighbour_count == 0)
								is_surface[index] = false;
							else
								is_surface[index] = true;
							//							}
							//							else
							//							{
							//								is_surface[index] = true;
							//							}
						}
					}
					else
					{
						is_surface[index] = false;
					}

					total_count++;

					if(0 != progressbar)
					{
						percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

						if(percent_done > last_percent_done || percent_done == 100)
						{
							last_percent_done = percent_done;
							SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
							//						Sleep(1);
						}
					}

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						z = y = x = internal_params.hopf_field_resolution;
						cancelled = true;
					}
				}
			}
		}
	}


	vector<bool> *data_ptr = 0;

	if(false == internal_params.cull_non_surface_points)
		data_ptr = &voxels;
	else
		data_ptr = &is_surface;


	if(!cancelled)
	{
		if(0 != progressbar)
			SendMessage(progressbar, PBM_SETPOS, 0, 0);


		SetWindowText(status_text, "Generating vertices...");
		UpdateWindow(status_text);

		vertex_count = 0;

		for(vector<bool>::const_iterator i = data_ptr->begin(); i != data_ptr->end(); i++)
			if(*i == true)
				vertex_count++;

		if(0 == vertex_count)
		{ 
			if(0 != vertices)
			{
				delete [] vertices;
				vertices = 0;
			}
		}
		else
		{
			vertices = new(std::nothrow) POS_NORMAL_COLOUR_VERTEX[vertex_count];

			long unsigned int vertex_index = 0;

			// if we managed to scrounge up a bit more memory
			if(0 != vertices)
			{
				quaternion_t<double> Z(d_info.x_max, d_info.y_max, d_info.z_max, internal_params.z_w);

				num_steps_total = internal_params.hopf_field_resolution * internal_params.hopf_field_resolution * internal_params.hopf_field_resolution;
				total_count = 0;
				percent_done = 0;
				last_percent_done = 0;

				const double x_step_size = (d_info.x_max - d_info.x_min) / (internal_params.hopf_field_resolution - 1);
				const double y_step_size = (d_info.y_max - d_info.y_min) / (internal_params.hopf_field_resolution - 1);
				const double z_step_size = (d_info.z_max - d_info.z_min) / (internal_params.hopf_field_resolution - 1);

				for(long unsigned int z_step_number = 0; z_step_number < internal_params.hopf_field_resolution; z_step_number++, Z.z -= z_step_size, Z.y = d_info.y_max)
				{
					for(long unsigned int y_step_number = 0; y_step_number < internal_params.hopf_field_resolution; y_step_number++, Z.y -= y_step_size, Z.x = d_info.x_max)
					{
						for(long unsigned int x_step_number = 0; x_step_number < internal_params.hopf_field_resolution; x_step_number++, Z.x -= x_step_size)
						{
							long unsigned int index = z_step_number*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + y_step_number*internal_params.hopf_field_resolution + x_step_number;

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

							if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
							{
								z_step_number = y_step_number = x_step_number = internal_params.hopf_field_resolution;
								cancelled = true;
							}

							if(true == (*data_ptr)[index])
							{
								vertices[vertex_index].x = static_cast<float>(Z.x);
								vertices[vertex_index].y = static_cast<float>(Z.y);
								vertices[vertex_index].z = static_cast<float>(Z.z);


								vertices[vertex_index].nx = 0.0f;
								vertices[vertex_index].ny = 0.0f;
								vertices[vertex_index].nz = 0.0f;

								signed int x_min_radius = 0;
								signed int x_max_radius = 0;
								signed int y_min_radius = 0;
								signed int y_max_radius = 0;
								signed int z_min_radius = 0;
								signed int z_max_radius = 0;

								if(x_step_number < internal_params.normal_gen_radius)
									x_min_radius = -static_cast<signed int>(x_step_number);
								else
									x_min_radius = -static_cast<signed int>(internal_params.normal_gen_radius);

								if(internal_params.hopf_field_resolution - x_step_number - 1 < internal_params.normal_gen_radius)
									x_max_radius = static_cast<signed int>(internal_params.hopf_field_resolution - x_step_number - 1);
								else
									x_max_radius = internal_params.normal_gen_radius;

								if(y_step_number < internal_params.normal_gen_radius)
									y_min_radius = -static_cast<signed int>(y_step_number);
								else
									y_min_radius = -static_cast<signed int>(internal_params.normal_gen_radius);

								if(internal_params.hopf_field_resolution - y_step_number - 1 < internal_params.normal_gen_radius)
									y_max_radius = static_cast<signed int>(internal_params.hopf_field_resolution - y_step_number - 1);
								else
									y_max_radius = internal_params.normal_gen_radius;

								if(z_step_number < internal_params.normal_gen_radius)
									z_min_radius = -static_cast<signed int>(z_step_number);
								else
									z_min_radius = -static_cast<signed int>(internal_params.normal_gen_radius);

								if(internal_params.hopf_field_resolution - z_step_number - 1 < internal_params.normal_gen_radius)
									z_max_radius = static_cast<signed int>(internal_params.hopf_field_resolution - z_step_number - 1);
								else
									z_max_radius = internal_params.normal_gen_radius;



								for(signed int z = z_min_radius; z <= z_max_radius; z++)
								{
									for(signed int y = y_min_radius; y <= y_max_radius; y++)
									{
										for(signed int x = x_min_radius; x <= x_max_radius; x++)
										{
											long unsigned int temp_x = x_step_number;
											long unsigned int temp_y = y_step_number;
											long unsigned int temp_z = z_step_number;

											if(x < 0)
												temp_x -= abs(x);
											else
												temp_x += x;

											if(y < 0)
												temp_y -= abs(y);
											else
												temp_y += y;

											if(z < 0)
												temp_z -= abs(z);
											else
												temp_z += z;

											if(temp_x == x_step_number &&
												temp_y == y_step_number &&
												temp_z == z_step_number)
												continue;


											if(true == voxels[temp_z*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution + temp_y*internal_params.hopf_field_resolution + temp_x])
											{
												float local_x = static_cast<float>(x);
												float local_y = static_cast<float>(y);
												float local_z = static_cast<float>(z);
												float len = sqrt(local_x*local_x + local_y*local_y + local_z*local_z);

												if(len != 1.0)
												{
													local_x /= len;
													local_y /= len;
													local_z /= len;
												}

												vertices[vertex_index].nx += local_x;
												vertices[vertex_index].ny += local_y;
												vertices[vertex_index].nz += local_z;
											}
										}
									}
								}

								float len = sqrt(vertices[vertex_index].nx*vertices[vertex_index].nx + vertices[vertex_index].ny*vertices[vertex_index].ny + vertices[vertex_index].nz*vertices[vertex_index].nz);

								if(len <= 0.1)
								{
									vertices[vertex_index].nx = 0.0f;
									vertices[vertex_index].ny = 0.0f;
									vertices[vertex_index].nz = -1.0f;
								}
								else if(len != 1.0)
								{
									vertices[vertex_index].nx /= len;
									vertices[vertex_index].ny /= len;
									vertices[vertex_index].nz /= len;
								}

								vertex_index++;
							}
						}
					}
				}
			}
		}
	}

	if(cancelled == true)
	{
		if(0 != vertices)
			delete [] vertices;

		vertex_count = 0;

		return 0;
	}
	else
	{
		return 1;
	}
}






int quaternion_julia_set::SavePointSetCSV(const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event)
{
	SendMessage(progressbar, PBM_SETPOS, 0, 0);




	const double x_step_size = (internal_params.x_max - internal_params.x_min) / (internal_params.x_resolution - 1);
	const double y_step_size = (internal_params.y_max - internal_params.y_min) / (internal_params.y_resolution - 1);
	const double z_step_size = (internal_params.z_max - internal_params.z_min) / (internal_params.z_resolution - 1);

	long unsigned int num_steps_total = internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution;
	long unsigned int total_count = 0;
	unsigned short int percent_done = 0;
	unsigned short int last_percent_done = 0;

	quaternion_t<double> Z(internal_params.x_max, internal_params.y_max, internal_params.z_max, internal_params.z_w);
	quaternion_t<double> C(internal_params.c_x, internal_params.c_y, internal_params.c_z, internal_params.c_w);

	cache_data set_data;

	if(0 != status_text)
	{
		SetWindowText(status_text, "Checking cache...");
		UpdateWindow(status_text);
	}

	int ret = cache.GetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, status_text);

	if(ret != 1)
	{
		if(ret == -1)
			MessageBox(0, "Insufficient memory was available to read set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(ret == -2)
			MessageBox(0, "A file error occurred while reading set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		set_data.w_step_number = set_data.x_step_number = set_data.y_step_number = set_data.z_step_number = 0;
		try { set_data.data.resize(internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution); } catch(...) { return -1; }
	}
	else
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		for(long unsigned int temp_z_step_number = 0; temp_z_step_number < set_data.z_step_number; temp_z_step_number++)
			Z.z -= z_step_size;

		for(long unsigned int temp_y_step_number = 0; temp_y_step_number < set_data.y_step_number; temp_y_step_number++)
			Z.y -= y_step_size;

		for(long unsigned int temp_x_step_number = 0; temp_x_step_number < set_data.x_step_number; temp_x_step_number++)
			Z.x -= x_step_size;

		total_count = set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;
		percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);
	}


	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	double length = 0.0f;

	quaternion_t<double> *final_z;
	quaternion_t<double> *final_c;

	if(false == internal_params.swap_z_and_c)
	{
		final_z = &Z;
		final_c = &C;
	}
	else
	{
		final_z = &C;
		final_c = &Z;
	}

	bool cancelled = false;

	for(; set_data.z_step_number < internal_params.z_resolution; set_data.z_step_number++, Z.z -= z_step_size, Z.y = internal_params.y_max, 		set_data.y_step_number = 0)
	{
		for(; set_data.y_step_number < internal_params.y_resolution; set_data.y_step_number++, Z.y -= y_step_size, Z.x = internal_params.x_max, 			set_data.x_step_number = 0)
		{
			for(; set_data.x_step_number < internal_params.x_resolution; set_data.x_step_number++, Z.x -= x_step_size)
			{
				long unsigned int index = set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;

				if(internal_params.infinity > (length = (this->*iterator_ptr)(*final_z, *final_c, internal_params.iterations, internal_params.infinity, internal_params.a)))
					set_data.data[index] = true;
				else
					set_data.data[index] = false;

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

				if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
				{
					int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_Z_MAJOR_PLANE, status_text);

					if(ret != 1)
					{
						if(ret == -1)
							MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

						if(ret == -2)
							MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
					}

					set_data.z_step_number = internal_params.z_resolution;
					set_data.y_step_number = internal_params.y_resolution;
					set_data.x_step_number = internal_params.x_resolution;
					cancelled = true;
				}
			}
		}
	}


	if(!cancelled)
	{
		int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_Z_MAJOR_PLANE, status_text);

		if(ret != 1)
		{
			if(ret == -1)
				MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

			if(ret == -2)
				MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
		}
	}






	// reverse set if enabled
	if(!cancelled && internal_params.reverse_set)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Reversing set...");
			UpdateWindow(status_text);
		}

		num_steps_total = internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution;
		total_count = 0;
		last_percent_done = 0;
		percent_done = 0;

		for(long unsigned int z = 0; z < internal_params.z_resolution; z++)
		{
			for(long unsigned int y = 0; y < internal_params.y_resolution; y++)
			{
				for(long unsigned int x = 0; x < internal_params.x_resolution; x++)
				{
					long unsigned int index = z*internal_params.y_resolution*internal_params.x_resolution + y*internal_params.x_resolution + x;
					set_data.data[index] = !set_data.data[index];

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

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						z = internal_params.z_resolution;
						y = internal_params.y_resolution;
						x = internal_params.x_resolution;
						cancelled = true;
					}
				}
			}
		}
	}



//	vector<bool> is_surface;
	cache_data is_surface;

	// cull non-surface points if enabled
	if(!cancelled && internal_params.cull_non_surface_points)
	{
		if(0 != progressbar)
			SendMessage(progressbar, PBM_SETPOS, 0, 0);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Checking cache...");
			UpdateWindow(status_text);
		}

		num_steps_total = internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution;
		total_count = 0;
		last_percent_done = 0;
		percent_done = 0;

		int ret = cache.GetCacheData(internal_params, is_surface, J4D2_CACHE_SURFACE_NONSAMPLEDTYPE, status_text);

		if(ret != 1)
		{
			if(ret == -1)
				MessageBox(0, "Insufficient memory was available to read surface data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

			if(ret == -2)
				MessageBox(0, "A file error occurred while reading surface data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

			if(0 != status_text)
			{
				SetWindowText(status_text, "Tracing 3D surface...");
				UpdateWindow(status_text);
			}

			is_surface.w_step_number = is_surface.x_step_number = is_surface.y_step_number = is_surface.z_step_number = 0;
			try { is_surface.data.resize(internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution); } catch(...) { return -1; }
		}
		else
		{
			if(0 != status_text)
			{
				SetWindowText(status_text, "Tracing 3D surface...");
				UpdateWindow(status_text);
			}

			total_count = is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + is_surface.x_step_number;
			percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);
		}

		for(; is_surface.z_step_number < internal_params.z_resolution; is_surface.z_step_number++, is_surface.y_step_number = 0)
		{
			for(; is_surface.y_step_number < internal_params.y_resolution; is_surface.y_step_number++, is_surface.x_step_number = 0)
			{
				for(; is_surface.x_step_number < internal_params.x_resolution; is_surface.x_step_number++)
				{
					// these indices were completely backwards (don't take back medication when programming...)
					long unsigned int index = is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + is_surface.x_step_number;

					// if it's on, test if all 6 immediate neighbours are also on (if it's not on the edge)
					if(true == set_data.data[index])
					{
						if( is_surface.z_step_number != 0 && is_surface.z_step_number != internal_params.z_resolution - 1 &&
							is_surface.y_step_number != 0 && is_surface.y_step_number != internal_params.y_resolution - 1 &&
							is_surface.x_step_number != 0 && is_surface.x_step_number != internal_params.x_resolution - 1 &&
							true == set_data.data[(is_surface.z_step_number-1)*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + is_surface.x_step_number] && 
							true == set_data.data[(is_surface.z_step_number+1)*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + is_surface.x_step_number] && 
							true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + (is_surface.y_step_number-1)*internal_params.x_resolution + is_surface.x_step_number] && 
							true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + (is_surface.y_step_number+1)*internal_params.x_resolution + is_surface.x_step_number] && 
							true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + (is_surface.x_step_number-1)] && 
							true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + (is_surface.x_step_number+1)])

						{
							is_surface.data[index] = false;
						}
						else
						{
							// don't include voxels with no neighbours
							//							if(internal_params.light_by_normals)
							//							{
							unsigned int neighbour_count = 0;

							if(is_surface.z_step_number != 0)
								if(true == set_data.data[(is_surface.z_step_number-1)*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + is_surface.x_step_number])
									neighbour_count++;

							if(is_surface.z_step_number != internal_params.z_resolution-1)
								if(true == set_data.data[(is_surface.z_step_number+1)*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + is_surface.x_step_number])
									neighbour_count++;

							if(is_surface.y_step_number != 0)
								if(true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + (is_surface.y_step_number-1)*internal_params.x_resolution + is_surface.x_step_number])
									neighbour_count++;

							if(is_surface.y_step_number != internal_params.y_resolution-1)
								if(true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + (is_surface.y_step_number+1)*internal_params.x_resolution + is_surface.x_step_number])
									neighbour_count++;

							if(is_surface.x_step_number != 0)
								if(true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + (is_surface.x_step_number-1)])
									neighbour_count++;

							if(is_surface.x_step_number != internal_params.x_resolution-1)
								if(true == set_data.data[is_surface.z_step_number*internal_params.y_resolution*internal_params.x_resolution + is_surface.y_step_number*internal_params.x_resolution + (is_surface.x_step_number+1)])
									neighbour_count++;

							if(neighbour_count == 0)
								is_surface.data[index] = false;
							else
								is_surface.data[index] = true;
						}
					}
					else
					{
						is_surface.data[index] = false;
					}

					total_count++;

					if(0 != progressbar)
					{
						percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

						if(percent_done > last_percent_done || percent_done == 100)
						{
							last_percent_done = percent_done;
							SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
							//						Sleep(1);
						}
					}

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						int ret = cache.SetCacheData(internal_params, is_surface, J4D2_CACHE_SURFACE_NONSAMPLEDTYPE, J4D2_CACHE_Z_MAJOR_PLANE, status_text);

						if(ret != 1)
						{
							if(ret == -1)
								MessageBox(0, "Insufficient memory was available to write surface data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

							if(ret == -2)
								MessageBox(0, "A file error occurred while writing surface data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
						}

						is_surface.z_step_number = internal_params.z_resolution;
						is_surface.y_step_number = internal_params.y_resolution;
						is_surface.x_step_number = internal_params.x_resolution;
						cancelled = true;
					}
				}
			}
		}

		if(!cancelled)
		{
			int ret = cache.SetCacheData(internal_params, is_surface, J4D2_CACHE_SURFACE_NONSAMPLEDTYPE, J4D2_CACHE_Z_MAJOR_PLANE, status_text);

			if(ret != 1)
			{
				if(ret == -1)
					MessageBox(0, "Insufficient memory was available to write surface data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

				if(ret == -2)
					MessageBox(0, "A file error occurred while writing surface data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
			}
		}
	}


	vector<bool> *data_ptr = 0;

	if(false == internal_params.cull_non_surface_points)
		data_ptr = &set_data.data;
	else
		data_ptr = &is_surface.data;

	if(!cancelled && 0 != status_text)
	{
		SetWindowText(status_text, "Writing to CSV file...");
		UpdateWindow(status_text);
	}


	if(!cancelled)
	{
		ofstream out(internal_params.export_file_name.GetBuffer());

		if(out.fail())
			return -2;

		out << setiosflags(ios_base::fixed);
		out << setprecision(18);
		out << "x,y,z" << endl;

		quaternion_t<double> Z(internal_params.x_max, internal_params.y_max, internal_params.z_max, internal_params.z_w);

		total_count = 0;
		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution;

		for(long unsigned int z_step_number = 0; z_step_number < internal_params.z_resolution; z_step_number++, Z.z -= z_step_size, Z.y = internal_params.y_max)
		{
			for(long unsigned int y_step_number = 0; y_step_number < internal_params.y_resolution; y_step_number++, Z.y -= y_step_size, Z.x = internal_params.x_max)
			{
				for(long unsigned int x_step_number = 0; x_step_number < internal_params.x_resolution; x_step_number++, Z.x -= x_step_size)
				{
					long unsigned int index = z_step_number*internal_params.y_resolution*internal_params.x_resolution + y_step_number*internal_params.x_resolution + x_step_number;

					if(true == (*data_ptr)[index])
						out << static_cast<float>(Z.x) << ',' << static_cast<float>(Z.y) << ',' << static_cast<float>(Z.z) << endl;

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

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						z_step_number = internal_params.z_resolution;
						y_step_number = internal_params.y_resolution;
						x_step_number = internal_params.x_resolution;
						cancelled = true;
					}
				}
			}
		}
	}


	if(cancelled)
		return 0;
	else
		return 1;
}



int quaternion_julia_set::SaveHopfMapCSV(const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event)
{
	SendMessage(progressbar, PBM_SETPOS, 0, 0);



	const double w_step_size = (internal_params.w_max - internal_params.w_min) / (internal_params.w_resolution - 1);
	const double x_step_size = (internal_params.x_max - internal_params.x_min) / (internal_params.x_resolution - 1);
	const double y_step_size = (internal_params.y_max - internal_params.y_min) / (internal_params.y_resolution - 1);
	const double z_step_size = (internal_params.z_max - internal_params.z_min) / (internal_params.z_resolution - 1);

	long unsigned int num_steps_total = internal_params.w_resolution * internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution;
	long unsigned int total_count = 0;
	unsigned short int percent_done = 0;
	unsigned short int last_percent_done = 0;

	quaternion_t<double> Z(internal_params.x_max, internal_params.y_max, internal_params.z_max, internal_params.w_max);
	quaternion_t<double> C(internal_params.c_x, internal_params.c_y, internal_params.c_z, internal_params.c_w);

	cache_data set_data;

	if(0 != status_text)
	{
		SetWindowText(status_text, "Checking cache...");
		UpdateWindow(status_text);
	}

	int ret = cache.GetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, status_text);

	if(ret != 1)
	{
		if(ret == -1)
			MessageBox(0, "Insufficient memory was available to read set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(ret == -2)
			MessageBox(0, "A file error occurred while reading set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		set_data.w_step_number = set_data.x_step_number = set_data.y_step_number = set_data.z_step_number = 0;
		try { set_data.data.resize(internal_params.w_resolution * internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution); } catch(...) { return -1; }
	}
	else
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		for(long unsigned int temp_w_step_number = 0; temp_w_step_number < set_data.w_step_number; temp_w_step_number++)
			Z.w -= w_step_size;

		for(long unsigned int temp_z_step_number = 0; temp_z_step_number < set_data.z_step_number; temp_z_step_number++)
			Z.z -= z_step_size;

		for(long unsigned int temp_y_step_number = 0; temp_y_step_number < set_data.y_step_number; temp_y_step_number++)
			Z.y -= y_step_size;

		for(long unsigned int temp_x_step_number = 0; temp_x_step_number < set_data.x_step_number; temp_x_step_number++)
			Z.x -= x_step_size;

		total_count = set_data.w_step_number*internal_params.z_resolution*internal_params.y_resolution*internal_params.x_resolution + set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;
		percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);
	}


	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	double length = 0.0f;

	quaternion_t<double> *final_z;
	quaternion_t<double> *final_c;

	if(false == internal_params.swap_z_and_c)
	{
		final_z = &Z;
		final_c = &C;
	}
	else
	{
		final_z = &C;
		final_c = &Z;
	}

	bool cancelled = false;

	for(; set_data.w_step_number < internal_params.w_resolution; set_data.w_step_number++, Z.w -= w_step_size, Z.z = internal_params.z_max, set_data.z_step_number = 0)
	{
		for(; set_data.z_step_number < internal_params.z_resolution; set_data.z_step_number++, Z.z -= z_step_size, Z.y = internal_params.y_max, set_data.y_step_number = 0)
		{
			for(; set_data.y_step_number < internal_params.y_resolution; set_data.y_step_number++, Z.y -= y_step_size, Z.x = internal_params.x_max, set_data.x_step_number = 0)
			{
				for(; set_data.x_step_number < internal_params.x_resolution; set_data.x_step_number++, Z.x -= x_step_size)
				{
					long unsigned int index = set_data.w_step_number*internal_params.z_resolution*internal_params.y_resolution*internal_params.x_resolution + set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;

					if(internal_params.infinity > (length = (this->*iterator_ptr)(*final_z, *final_c, internal_params.iterations, internal_params.infinity, internal_params.a)))
						set_data.data[index] = true;
					else
						set_data.data[index] = false;

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

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_W_MAJOR_PLANE, status_text);

						if(ret != 1)
						{
							if(ret == -1)
								MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

							if(ret == -2)
								MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
						}

						set_data.z_step_number = internal_params.z_resolution;
						set_data.y_step_number = internal_params.y_resolution;
						set_data.x_step_number = internal_params.x_resolution;
						set_data.w_step_number = internal_params.w_resolution;
						cancelled = true;
					}
				}
			}
		}
	}


	if(!cancelled)
	{
		int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_W_MAJOR_PLANE, status_text);

		if(ret != 1)
		{
			if(ret == -1)
				MessageBox(0, "Insufficient memory was available to write surface data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

			if(ret == -2)
				MessageBox(0, "A file error occurred while writing surface data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
		}

	}



	// project from 4d to 3d
	if(!cancelled)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Writing to CSV file...");
			UpdateWindow(status_text);
		}

		ofstream out(internal_params.export_file_name.GetBuffer());

		if(out.fail())
			return -2;

		out << setiosflags(ios_base::fixed);
		out << setprecision(18);
		out << "x,y,z" << endl;

		num_steps_total = internal_params.w_resolution * internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution;
		total_count = 0;
		last_percent_done = 0;
		percent_done = 0;

		Z.w = internal_params.w_max;
		Z.x = internal_params.x_max;
		Z.y = internal_params.y_max;
		Z.z = internal_params.z_max;

		for(long unsigned int w = 0; w < internal_params.w_resolution; w++, Z.w -= w_step_size, Z.z = internal_params.z_max)
		{
			for(long unsigned int z = 0; z < internal_params.z_resolution; z++, Z.z -= z_step_size, Z.y = internal_params.y_max)
			{
				for(long unsigned int y = 0; y < internal_params.y_resolution; y++, Z.y -= y_step_size, Z.x = internal_params.x_max)
				{
					for(long unsigned int x = 0; x < internal_params.x_resolution; x++, Z.x -= x_step_size)
					{
						long unsigned int index = w*internal_params.z_resolution*internal_params.y_resolution*internal_params.x_resolution + z*internal_params.y_resolution*internal_params.x_resolution + y*internal_params.x_resolution + x;

						if(true == set_data.data[index])
						{
							POS_NORMAL_COLOUR_VERTEX temp_vertex;
							PerformHopfMapping(temp_vertex, Z);

							out << static_cast<float>(temp_vertex.x) << ',' << static_cast<float>(temp_vertex.y) << ',' << static_cast<float>(temp_vertex.z) << endl;
						}

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

						if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
						{
							w = internal_params.w_resolution;
							x = internal_params.x_resolution;
							y = internal_params.y_resolution;
							z = internal_params.z_resolution;
							cancelled = true;
						}
					}
				}
			}
		}
	}




	if(cancelled)
		return 0;
	else
		return 1;
}




int quaternion_julia_set::SaveJ4D2VOX(const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event)
{
	SendMessage(progressbar, PBM_SETPOS, 0, 0);




	const double x_step_size = (internal_params.x_max - internal_params.x_min) / (internal_params.x_resolution - 1);
	const double y_step_size = (internal_params.y_max - internal_params.y_min) / (internal_params.y_resolution - 1);
	const double z_step_size = (internal_params.z_max - internal_params.z_min) / (internal_params.z_resolution - 1);

	long unsigned int num_steps_total = internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution;
	long unsigned int total_count = 0;
	unsigned short int percent_done = 0;
	unsigned short int last_percent_done = 0;

	quaternion_t<double> Z(internal_params.x_max, internal_params.y_max, internal_params.z_max, internal_params.z_w);
	quaternion_t<double> C(internal_params.c_x, internal_params.c_y, internal_params.c_z, internal_params.c_w);

	cache_data set_data;

	if(0 != status_text)
	{
		SetWindowText(status_text, "Checking cache...");
		UpdateWindow(status_text);
	}

	int ret = cache.GetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, status_text);

	if(ret != 1)
	{
		if(ret == -1)
			MessageBox(0, "Insufficient memory was available to read set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(ret == -2)
			MessageBox(0, "A file error occurred while reading set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		set_data.w_step_number = set_data.x_step_number = set_data.y_step_number = set_data.z_step_number = 0;
		try { set_data.data.resize(internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution); } catch(...) { return -1; }
	}
	else
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		for(long unsigned int temp_z_step_number = 0; temp_z_step_number < set_data.z_step_number; temp_z_step_number++)
			Z.z -= z_step_size;

		for(long unsigned int temp_y_step_number = 0; temp_y_step_number < set_data.y_step_number; temp_y_step_number++)
			Z.y -= y_step_size;

		for(long unsigned int temp_x_step_number = 0; temp_x_step_number < set_data.x_step_number; temp_x_step_number++)
			Z.x -= x_step_size;

		total_count = set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;
		percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);
	}


	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	double length = 0.0f;

	quaternion_t<double> *final_z;
	quaternion_t<double> *final_c;

	if(false == internal_params.swap_z_and_c)
	{
		final_z = &Z;
		final_c = &C;
	}
	else
	{
		final_z = &C;
		final_c = &Z;
	}

	bool cancelled = false;

	for(; set_data.z_step_number < internal_params.z_resolution; set_data.z_step_number++, Z.z -= z_step_size, Z.y = internal_params.y_max, set_data.y_step_number = 0)
	{
		for(; set_data.y_step_number < internal_params.y_resolution; set_data.y_step_number++, Z.y -= y_step_size, Z.x = internal_params.x_max,	set_data.x_step_number = 0)
		{
			for(; set_data.x_step_number < internal_params.x_resolution; set_data.x_step_number++, Z.x -= x_step_size)
			{
				long unsigned int index = set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;

				if(internal_params.infinity > (length = (this->*iterator_ptr)(*final_z, *final_c, internal_params.iterations, internal_params.infinity, internal_params.a)))
					set_data.data[index] = true;
				else
					set_data.data[index] = false;

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

				if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
				{
					int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_Z_MAJOR_PLANE, status_text);

					if(ret != 1)
					{
						if(ret == -1)
							MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

						if(ret == -2)
							MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
					}

					set_data.z_step_number = internal_params.z_resolution;
					set_data.y_step_number = internal_params.y_resolution;
					set_data.x_step_number = internal_params.x_resolution;
					cancelled = true;
				}
			}
		}
	}



	if(!cancelled)
	{
		int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_Z_MAJOR_PLANE, status_text);

		if(ret != 1)
		{
			if(ret == -1)
				MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

			if(ret == -2)
				MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
		}
	}


	// reverse set if enabled
	if(!cancelled && internal_params.reverse_set)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Reversing set...");
			UpdateWindow(status_text);
		}

		num_steps_total = internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution;
		total_count = 0;
		last_percent_done = 0;
		percent_done = 0;

		for(long unsigned int z = 0; z < internal_params.z_resolution; z++)
		{
			for(long unsigned int y = 0; y < internal_params.y_resolution; y++)
			{
				for(long unsigned int x = 0; x < internal_params.x_resolution; x++)
				{
					long unsigned int index = z*internal_params.y_resolution*internal_params.x_resolution + y*internal_params.x_resolution + x;
					set_data.data[index] = !set_data.data[index];

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

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						z = internal_params.z_resolution;
						y = internal_params.y_resolution;
						x = internal_params.x_resolution;
						cancelled = true;
					}
				}
			}
		}
	}


	if(cancelled)
		return 0;


	if(0 != status_text)
	{
		SetWindowText(status_text, "Writing to J4D2VOX file...");
		UpdateWindow(status_text);
	}

	return WriteJ4D2VoxelFile(
		set_data.data,
		internal_params.export_file_name.GetBuffer(),
		static_cast<float>(internal_params.x_min),
		static_cast<float>(internal_params.x_max),
		static_cast<float>(internal_params.y_min),
		static_cast<float>(internal_params.y_max),
		static_cast<float>(internal_params.z_min),
		static_cast<float>(internal_params.z_max),
		internal_params.x_resolution,
		internal_params.y_resolution,
		internal_params.z_resolution);
}








int quaternion_julia_set::SaveOBJ(const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event)
{
	SendMessage(progressbar, PBM_SETPOS, 0, 0);



	const double x_step_size = (internal_params.x_max - internal_params.x_min) / (internal_params.x_resolution - 1);
	const double y_step_size = (internal_params.y_max - internal_params.y_min) / (internal_params.y_resolution - 1);
	const double z_step_size = (internal_params.z_max - internal_params.z_min) / (internal_params.z_resolution - 1);

	long unsigned int num_steps_total = internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution;
	long unsigned int total_count = 0;
	unsigned short int percent_done = 0;
	unsigned short int last_percent_done = 0;

	quaternion_t<double> Z(internal_params.x_max, internal_params.y_max, internal_params.z_max, internal_params.z_w);
	quaternion_t<double> C(internal_params.c_x, internal_params.c_y, internal_params.c_z, internal_params.c_w);

	cache_data set_data;

	if(0 != status_text)
	{
		SetWindowText(status_text, "Checking cache...");
		UpdateWindow(status_text);
	}

	int ret = cache.GetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, status_text);

	if(ret != 1)
	{
		if(ret == -1)
			MessageBox(0, "Insufficient memory was available to read set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(ret == -2)
			MessageBox(0, "A file error occurred while reading set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		set_data.w_step_number = set_data.x_step_number = set_data.y_step_number = set_data.z_step_number = 0;
		try { set_data.data.resize(internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution); } catch(...) { return -1; }
	}
	else
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		for(long unsigned int temp_z_step_number = 0; temp_z_step_number < set_data.z_step_number; temp_z_step_number++)
			Z.z -= z_step_size;

		for(long unsigned int temp_y_step_number = 0; temp_y_step_number < set_data.y_step_number; temp_y_step_number++)
			Z.y -= y_step_size;

		for(long unsigned int temp_x_step_number = 0; temp_x_step_number < set_data.x_step_number; temp_x_step_number++)
			Z.x -= x_step_size;

		total_count = set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;
		percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);
	}


	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	double length = 0.0f;

	quaternion_t<double> *final_z;
	quaternion_t<double> *final_c;

	if(false == internal_params.swap_z_and_c)
	{
		final_z = &Z;
		final_c = &C;
	}
	else
	{
		final_z = &C;
		final_c = &Z;
	}

	bool cancelled = false;

	for(; set_data.z_step_number < internal_params.z_resolution; set_data.z_step_number++, Z.z -= z_step_size, Z.y = internal_params.y_max, set_data.y_step_number = 0)
	{
		for(; set_data.y_step_number < internal_params.y_resolution; set_data.y_step_number++, Z.y -= y_step_size, Z.x = internal_params.x_max, set_data.x_step_number = 0)
		{
			for(; set_data.x_step_number < internal_params.x_resolution; set_data.x_step_number++, Z.x -= x_step_size)
			{
				long unsigned int index = set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;

				if(internal_params.infinity > (length = (this->*iterator_ptr)(*final_z, *final_c, internal_params.iterations, internal_params.infinity, internal_params.a)))
					set_data.data[index] = true;
				else
					set_data.data[index] = false;

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

				if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
				{
					int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_Z_MAJOR_PLANE, status_text);

					if(ret != 1)
					{
						if(ret == -1)
							MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

						if(ret == -2)
							MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
					}

					set_data.z_step_number = internal_params.z_resolution;
					set_data.y_step_number = internal_params.y_resolution;
					set_data.x_step_number = internal_params.x_resolution;
					cancelled = true;
				}
			}
		}
	}

	if(!cancelled)
	{
		int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_Z_MAJOR_PLANE, status_text);

		if(ret != 1)
		{
			if(ret == -1)
				MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

			if(ret == -2)
				MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
		}
	}




	// reverse set if enabled
	if(!cancelled && internal_params.reverse_set)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Reversing set...");
			UpdateWindow(status_text);
		}

		num_steps_total = internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution;
		total_count = 0;
		last_percent_done = 0;
		percent_done = 0;

		for(long unsigned int z = 0; z < internal_params.z_resolution; z++)
		{
			for(long unsigned int y = 0; y < internal_params.y_resolution; y++)
			{
				for(long unsigned int x = 0; x < internal_params.x_resolution; x++)
				{
					long unsigned int index = z*internal_params.y_resolution*internal_params.x_resolution + y*internal_params.x_resolution + x;
					set_data.data[index] = !set_data.data[index];

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

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						z = internal_params.z_resolution;
						y = internal_params.y_resolution;
						x = internal_params.x_resolution;
						cancelled = true;
					}
				}
			}
		}
	}




	vector<bool> final_grid;

	if(!cancelled)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Allocating memory...");
			UpdateWindow(status_text);
		}

		try
		{
			final_grid.resize((internal_params.z_resolution+2) * (internal_params.y_resolution+2) * (internal_params.x_resolution+2));
		}
		catch(...)
		{
			return -1;
		}


		if(0 != status_text)
		{
			SetWindowText(status_text, "Adding border...");
			UpdateWindow(status_text);
		}

		num_steps_total = (internal_params.z_resolution) * (internal_params.y_resolution) * (internal_params.x_resolution);
		percent_done = 0;
		last_percent_done = 0;
		total_count = 0;

		SendMessage(progressbar, PBM_SETPOS, percent_done, 0);


		for(long unsigned int z = 0; z < internal_params.z_resolution; z++)
		{
			for(long unsigned int y = 0; y < internal_params.y_resolution; y++)
			{
				for(long unsigned int x = 0; x < internal_params.x_resolution; x++)
				{
					size_t final_grid_index = (z+1)*(internal_params.y_resolution+2)*(internal_params.x_resolution+2) + (y+1)*(internal_params.x_resolution+2) + (x+1);
					long unsigned int set_data_index = (z)*(internal_params.y_resolution)*(internal_params.x_resolution) + (y)*(internal_params.x_resolution) + (x);

					final_grid[final_grid_index] = set_data.data[set_data_index];


					total_count++;

					if(0 != progressbar)
					{
						percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

						if(percent_done > last_percent_done || percent_done == 100)
						{
							last_percent_done = percent_done;
							SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
						}
					}

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						return 0;
					}
				}
			}
		}
	}

	// force a memory delete on unnecessary data to make room for other stuff
	if(!cancelled)
	{
		vector<bool> temp_data;
		set_data.data = temp_data;
	}




	list<mc_tri> final_tris;

	if(!cancelled)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Generating mesh data...");
			UpdateWindow(status_text);
		}


		num_steps_total = (internal_params.z_resolution+1) * (internal_params.y_resolution+1) * (internal_params.x_resolution+1);
		percent_done = 0;
		last_percent_done = 0;
		total_count = 0;

		SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

		// temp variables
		mc_gridcell temp_cell;
		mc_tri temp_tris[5];

		// starting positions (taking into account blank border)
		double temp_x_max = internal_params.x_max + x_step_size;
		double temp_y_max = internal_params.y_max + y_step_size;
		double temp_z_max = internal_params.z_max + z_step_size;

		for(long unsigned int z = 0; z < internal_params.z_resolution+1; z++)
		{
			for(long unsigned int y = 0; y < internal_params.y_resolution+1; y++)
			{
				for(long unsigned int x = 0; x < internal_params.x_resolution+1; x++)
				{
					// cube vertex 0
					long unsigned int z_offset = 0;
					long unsigned int y_offset = 0;
					long unsigned int x_offset = 0;

					long unsigned int index = 0;

					z_offset = 1;
					y_offset = 0;
					x_offset = 0;
					temp_cell.p[0].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[0].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[0].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.y_resolution+2)*(internal_params.x_resolution+2) + (y+y_offset)*(internal_params.x_resolution+2) + (x+x_offset);

					if(true == final_grid[index])
						temp_cell.val[0] = MC_TOTALLY_FILLED;
					else
						temp_cell.val[0] = MC_TOTALLY_UNFILLED;


					// cube vertex 1
					z_offset = 1;
					y_offset = 0;
					x_offset = 1;
					temp_cell.p[1].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[1].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[1].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.y_resolution+2)*(internal_params.x_resolution+2) + (y+y_offset)*(internal_params.x_resolution+2) + (x+x_offset);

					if(true == final_grid[index])
						temp_cell.val[1] = MC_TOTALLY_FILLED;
					else
						temp_cell.val[1] = MC_TOTALLY_UNFILLED;


					// cube vertex 2
					z_offset = 0;
					y_offset = 0;
					x_offset = 1;
					temp_cell.p[2].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[2].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[2].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.y_resolution+2)*(internal_params.x_resolution+2) + (y+y_offset)*(internal_params.x_resolution+2) + (x+x_offset);

					if(true == final_grid[index])
						temp_cell.val[2] = MC_TOTALLY_FILLED;
					else
						temp_cell.val[2] = MC_TOTALLY_UNFILLED;


					// cube vertex 3
					z_offset = 0;
					y_offset = 0;
					x_offset = 0;
					temp_cell.p[3].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[3].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[3].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.y_resolution+2)*(internal_params.x_resolution+2) + (y+y_offset)*(internal_params.x_resolution+2) + (x+x_offset);

					if(true == final_grid[index])
						temp_cell.val[3] = MC_TOTALLY_FILLED;
					else
						temp_cell.val[3] = MC_TOTALLY_UNFILLED;


					// cube vertex 4
					z_offset = 1;
					y_offset = 1;
					x_offset = 0;
					temp_cell.p[4].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[4].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[4].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.y_resolution+2)*(internal_params.x_resolution+2) + (y+y_offset)*(internal_params.x_resolution+2) + (x+x_offset);

					if(true == final_grid[index])
						temp_cell.val[4] = MC_TOTALLY_FILLED;
					else
						temp_cell.val[4] = MC_TOTALLY_UNFILLED;


					// cube vertex 5
					z_offset = 1;
					y_offset = 1;
					x_offset = 1;
					temp_cell.p[5].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[5].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[5].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.y_resolution+2)*(internal_params.x_resolution+2) + (y+y_offset)*(internal_params.x_resolution+2) + (x+x_offset);

					if(true == final_grid[index])
						temp_cell.val[5] = MC_TOTALLY_FILLED;
					else
						temp_cell.val[5] = MC_TOTALLY_UNFILLED;


					// cube vertex 6
					z_offset = 0;
					y_offset = 1;
					x_offset = 1;
					temp_cell.p[6].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[6].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[6].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.y_resolution+2)*(internal_params.x_resolution+2) + (y+y_offset)*(internal_params.x_resolution+2) + (x+x_offset);

					if(true == final_grid[index])
						temp_cell.val[6] = MC_TOTALLY_FILLED;
					else
						temp_cell.val[6] = MC_TOTALLY_UNFILLED;


					// cube vertex 7
					z_offset = 0;
					y_offset = 1;
					x_offset = 0;
					temp_cell.p[7].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[7].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[7].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.y_resolution+2)*(internal_params.x_resolution+2) + (y+y_offset)*(internal_params.x_resolution+2) + (x+x_offset);

					if(true == final_grid[index])
						temp_cell.val[7] = MC_TOTALLY_FILLED;
					else
						temp_cell.val[7] = MC_TOTALLY_UNFILLED;



					int num_tris = 0;

					num_tris = QuatFractalPolygonise(temp_cell, temp_tris);

					try
					{
						for(unsigned short int i = 0; i < num_tris; i++)
							final_tris.push_back(temp_tris[i]);
					}
					catch(...)
					{
						return -1;
					}



					total_count++;

					if(0 != progressbar)
					{
						percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

						if(percent_done > last_percent_done || percent_done == 100)
						{
							last_percent_done = percent_done;
							SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
						}
					}

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						return 0;
					}
				}
			}
		}
	}

	if(!cancelled && 0 == final_tris.size())
	{
		MessageBox(NULL, "No triangles were generated. The file will not be saved.", APP_NAME, MB_OK);
		return 0;
	}




	// force a memory delete on unnecessary data to make room for other stuff
	if(!cancelled)
	{
		vector<bool> temp_data;
		final_grid = temp_data;
	}




	if(!cancelled)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Cleaning mesh...");
			UpdateWindow(status_text);
		}

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_tris.size());
		total_count = 0;

		long unsigned int cull_count = 0;


		for(list<mc_tri>::iterator i = final_tris.begin(); i != final_tris.end();)
		{
			if( (*i).p[0] == (*i).p[1] ||
				(*i).p[0] == (*i).p[2] ||
				(*i).p[1] == (*i).p[2] )
			{
				i = final_tris.erase(i);
			}
			else
			{
				i++;
			}


			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
					//						Sleep(1);
				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}




		if(0 != status_text)
		{
			SetWindowText(status_text, "Generating normals...");
			UpdateWindow(status_text);
		}

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_tris.size());
		total_count = 0;


		// generate face normals, do not normalize them yet
		for(list<mc_tri>::iterator i = final_tris.begin(); i != final_tris.end(); i++)
		{
			// calculate face normal via cross product of two planes of the triangle
			double x1 = (*i).p[0].x - (*i).p[1].x;
			double y1 = (*i).p[0].y - (*i).p[1].y;
			double z1 = (*i).p[0].z - (*i).p[1].z;

			double x2 = (*i).p[1].x - (*i).p[2].x;
			double y2 = (*i).p[1].y - (*i).p[2].y;
			double z2 = (*i).p[1].z - (*i).p[2].z;

			(*i).normal.x = ( y1 * z2 ) - ( z1 * y2 );
			(*i).normal.y = ( z1 * x2 ) - ( x1 * z2 );
			(*i).normal.z = ( x1 * y2 ) - ( y1 * x2 );


			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
					//						Sleep(1);
				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}




		if(0 != status_text)
		{
			SetWindowText(status_text, "Creating welds...");
			UpdateWindow(status_text);
		}

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_tris.size());
		total_count = 0;

		// insert unique vertices into set
		set<mc_xyz> final_vertices;

		for(list<mc_tri>::const_iterator i = final_tris.begin(); i != final_tris.end(); i++)
		{
			try
			{
				final_vertices.insert((*i).p[0]);
				final_vertices.insert((*i).p[1]);
				final_vertices.insert((*i).p[2]);
			}
			catch(...)
			{
				return -1;
			}


			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}




		// add indexes to the vertices, helps speed up triangle / vertex lookup
		long unsigned int index = 0;

		for(set<mc_xyz>::iterator i = final_vertices.begin(); i != final_vertices.end(); i++)
			(*i).index = index++;

		// find 3 vertices for each triangle, adding the index to the triangle
		set<mc_xyz>::const_iterator find_iter;

		vector< vector< mc_xyz > > vertex_normals;

		try
		{
			vertex_normals.resize(final_vertices.size());
		}
		catch(...)
		{
			return -1;
		}

		if(0 != status_text)
		{
			SetWindowText(status_text, "Assigning welds...");
			UpdateWindow(status_text);
		}

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_tris.size());
		total_count = 0;

		for(list<mc_tri>::iterator i = final_tris.begin(); i != final_tris.end(); i++)
		{
			try
			{
				find_iter = final_vertices.find((*i).p[0]);

				if(find_iter == final_vertices.end())
				{
					MessageBox(NULL, "Error processing vertices. This is a bug, please report this behaviour.", APP_NAME, MB_OK);
					return -3;
				}

				(*i).p[0].index = (*find_iter).index;
				vertex_normals[(*find_iter).index].push_back((*i).normal);


				find_iter = final_vertices.find((*i).p[1]);

				if(find_iter == final_vertices.end())
				{
					MessageBox(NULL, "Error processing vertices. This is a bug, please report this behaviour.", APP_NAME, MB_OK);
					return -3;
				}

				(*i).p[1].index = (*find_iter).index;
				vertex_normals[(*find_iter).index].push_back((*i).normal);


				find_iter = final_vertices.find((*i).p[2]);

				if(find_iter == final_vertices.end())
				{
					MessageBox(NULL, "Error processing vertices. This is a bug, please report this behaviour.", APP_NAME, MB_OK);
					return -3;
				}

				(*i).p[2].index = (*find_iter).index;
				vertex_normals[(*find_iter).index].push_back((*i).normal);
			}
			catch(...)
			{
				return -1;
			}


			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}




		// write to file
		ofstream out(internal_params.export_file_name.GetBuffer());

		if(out.fail())
			return -2;




		if(0 != status_text)
		{
			SetWindowText(status_text, "Writing vertices...");
			UpdateWindow(status_text);
		}

		out << setiosflags(ios_base::fixed);
		out << setprecision(18);

		out << "# Exported from Julia 4D 2 - Copyright (c) 2004-2005 Shawn Halayka -- shalayka@sasktel.net" << endl;
		out << "#" << endl;
		out << "# OBJ tesselation algorithm:" << endl;
		out << "# Polygonising a scalar field - http://astronomy.swin.edu.au/~pbourke/modelling/polygonise/" << endl;
		out << "# Also known as: \"3D Contouring\", \"Marching Cubes\", \"Surface Reconstruction\"" << endl;
		out << "#" << endl;
		out << "# Written by Paul Bourke" << endl;
		out << "# May 1997" << endl;
		out << "#" << endl;
		out << "# Based on tables by Cory Gene Bloyd along with" << endl;
		out << "# additional example source code marchingsource.cpp" << endl;
		out << "#" << endl;
		out << "# All 3rd party provided source code is Copyright (c) 2004-2005 by their respective authors." << endl << endl;

		out << "o julia4d2_fractal" << endl;
		out << "g julia4d2_fractal" << endl << endl;

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_vertices.size());
		total_count = 0;

		out << "# " << num_steps_total << " vertices" << "\n";

		for(set<mc_xyz>::const_iterator i = final_vertices.begin(); i != final_vertices.end(); i++)
		{
			out << "v " << (*i).x << " " << (*i).y << " " << (*i).z << "\n";


			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}




		if(0 != status_text)
		{
			SetWindowText(status_text, "Writing vertex normals...");
			UpdateWindow(status_text);
		}

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_vertices.size());
		total_count = 0;

		out << "\n# " << num_steps_total << " vertex normals" << "\n";

		long unsigned int vertex_index = 0;

		for(set<mc_xyz>::const_iterator i = final_vertices.begin(); i != final_vertices.end(); i++, vertex_index++)
		{
			// normalized smooth
			double temp_x = 0.0;
			double temp_y = 0.0;
			double temp_z = 0.0;

			// add up all face normals associated with this vertex
			for(size_t i = 0; i < vertex_normals[vertex_index].size(); i++)
			{
				double local_temp_x = vertex_normals[vertex_index][i].x;
				double local_temp_y = vertex_normals[vertex_index][i].y;
				double local_temp_z = vertex_normals[vertex_index][i].z;

				double local_len = sqrt(local_temp_x*local_temp_x + local_temp_y*local_temp_y + local_temp_z*local_temp_z);

				if(local_len != 1.0f)
				{
					local_temp_x /= local_len;
					local_temp_y /= local_len;
					local_temp_z /= local_len;
				}

				temp_x += local_temp_x;
				temp_y += local_temp_y;
				temp_z += local_temp_z;
			}

			// average them using a flat linear
			temp_x /= vertex_normals[vertex_index].size();
			temp_y /= vertex_normals[vertex_index].size();
			temp_z /= vertex_normals[vertex_index].size();

			// normalize the final result
			double len = sqrt(temp_x*temp_x + temp_y*temp_y + temp_z*temp_z);

			if(len != 1.0)
			{
				temp_x /= len;
				temp_y /= len;
				temp_z /= len;
			}

			out << "vn " << -temp_x << " " << -temp_y << " " << -temp_z << "\n";


			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}




		if(0 != status_text)
		{
			SetWindowText(status_text, "Writing triangles...");
			UpdateWindow(status_text);
		}

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_tris.size());
		total_count = 0;

		out << "\n# " << num_steps_total << " triangle(s)\n";

		for(list<mc_tri>::const_iterator i = final_tris.begin(); i != final_tris.end(); i++)
		{
			out << "f "
				<< (*i).p[2].index + 1 << "//" << (*i).p[2].index + 1 << " "
				<< (*i).p[1].index + 1 << "//" << (*i).p[1].index + 1 << " "
				<< (*i).p[0].index + 1 << "//" << (*i).p[0].index + 1
				<< "\n";


			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}

		out.close();
	}




	if(cancelled)
		return 0;
	else
		return 1;
}






int quaternion_julia_set::SaveHopfMapOBJ(const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event)
{
	SendMessage(progressbar, PBM_SETPOS, 0, 0);



	const double w_step_size = (internal_params.w_max - internal_params.w_min) / (internal_params.w_resolution - 1);
	const double x_step_size = (internal_params.x_max - internal_params.x_min) / (internal_params.x_resolution - 1);
	const double y_step_size = (internal_params.y_max - internal_params.y_min) / (internal_params.y_resolution - 1);
	const double z_step_size = (internal_params.z_max - internal_params.z_min) / (internal_params.z_resolution - 1);

	long unsigned int num_steps_total = internal_params.w_resolution * internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution;
	long unsigned int total_count = 0;
	unsigned short int percent_done = 0;
	unsigned short int last_percent_done = 0;

	quaternion_t<double> Z(internal_params.x_max, internal_params.y_max, internal_params.z_max, internal_params.w_max);
	quaternion_t<double> C(internal_params.c_x, internal_params.c_y, internal_params.c_z, internal_params.c_w);

	cache_data set_data;

	if(0 != status_text)
	{
		SetWindowText(status_text, "Checking cache...");
		UpdateWindow(status_text);
	}

	int ret = cache.GetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, status_text);

	if(ret != 1)
	{
		if(ret == -1)
			MessageBox(0, "Insufficient memory was available to read set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(ret == -2)
			MessageBox(0, "A file error occurred while reading set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		set_data.w_step_number = set_data.x_step_number = set_data.y_step_number = set_data.z_step_number = 0;
		try { set_data.data.resize(internal_params.w_resolution * internal_params.z_resolution * internal_params.y_resolution * internal_params.x_resolution); } catch(...) { return -1; }
	}
	else
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		for(long unsigned int temp_w_step_number = 0; temp_w_step_number < set_data.w_step_number; temp_w_step_number++)
			Z.w -= w_step_size;

		for(long unsigned int temp_z_step_number = 0; temp_z_step_number < set_data.z_step_number; temp_z_step_number++)
			Z.z -= z_step_size;

		for(long unsigned int temp_y_step_number = 0; temp_y_step_number < set_data.y_step_number; temp_y_step_number++)
			Z.y -= y_step_size;

		for(long unsigned int temp_x_step_number = 0; temp_x_step_number < set_data.x_step_number; temp_x_step_number++)
			Z.x -= x_step_size;

		total_count = set_data.w_step_number*internal_params.z_resolution*internal_params.y_resolution*internal_params.x_resolution + set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;
		percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);
	}


	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	double length = 0.0f;

	quaternion_t<double> *final_z;
	quaternion_t<double> *final_c;

	if(false == internal_params.swap_z_and_c)
	{
		final_z = &Z;
		final_c = &C;
	}
	else
	{
		final_z = &C;
		final_c = &Z;
	}

	bool cancelled = false;

	for(; set_data.w_step_number < internal_params.w_resolution; set_data.w_step_number++, Z.w -= w_step_size, Z.z = internal_params.z_max, set_data.z_step_number = 0)
	{
		for(; set_data.z_step_number < internal_params.z_resolution; set_data.z_step_number++, Z.z -= z_step_size, Z.y = internal_params.y_max, set_data.y_step_number = 0)
		{
			for(; set_data.y_step_number < internal_params.y_resolution; set_data.y_step_number++, Z.y -= y_step_size, Z.x = internal_params.x_max, set_data.x_step_number = 0)
			{
				for(; set_data.x_step_number < internal_params.x_resolution; set_data.x_step_number++, Z.x -= x_step_size)
				{
					long unsigned int index = set_data.w_step_number*internal_params.z_resolution*internal_params.y_resolution*internal_params.x_resolution + set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;

					if(internal_params.infinity > (length = (this->*iterator_ptr)(*final_z, *final_c, internal_params.iterations, internal_params.infinity, internal_params.a)))
						set_data.data[index] = true;
					else
						set_data.data[index] = false;

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

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_W_MAJOR_PLANE, status_text);

						if(ret != 1)
						{
							if(ret == -1)
								MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

							if(ret == -2)
								MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
						}

						set_data.w_step_number = internal_params.w_resolution;
						set_data.z_step_number = internal_params.z_resolution;
						set_data.y_step_number = internal_params.y_resolution;
						set_data.x_step_number = internal_params.x_resolution;

						cancelled = true;
					}
				}
			}
		}
	}


	if(!cancelled)
	{
		int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_W_MAJOR_PLANE, status_text);

		if(ret != 1)
		{
			if(ret == -1)
				MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

			if(ret == -2)
				MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
		}

	}




	list<mc_tri> final_tris;

	if(!cancelled)
	{
		volumize_hopf_map vhm;
		int ret = vhm.PolygoniseHopfMap(internal_params, set_data.data, final_tris, progressbar, status_text, cancel_event);
		if(ret != 1)
			return ret;
	}

	if(!cancelled && 0 == final_tris.size())
	{
		MessageBox(NULL, "No triangles were generated. The file will not be saved.", APP_NAME, MB_OK);
		return 0;
	}

	// clear out the bool vector that we don't need anymore
	if(!cancelled)
	{
		vector<bool> temp_data;
		set_data.data = temp_data;
	}











	if(!cancelled)
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Cleaning mesh...");
			UpdateWindow(status_text);
		}

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_tris.size());
		total_count = 0;

		long unsigned int cull_count = 0;


		for(list<mc_tri>::iterator i = final_tris.begin(); i != final_tris.end();)
		{
			if( (*i).p[0] == (*i).p[1] ||
				(*i).p[0] == (*i).p[2] ||
				(*i).p[1] == (*i).p[2] )
			{
				i = final_tris.erase(i);
			}
			else
			{
				i++;
			}


			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
					//						Sleep(1);
				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}




		if(0 != status_text)
		{
			SetWindowText(status_text, "Generating normals...");
			UpdateWindow(status_text);
		}

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_tris.size());
		total_count = 0;


		// generate face normals, do not normalize them yet
		for(list<mc_tri>::iterator i = final_tris.begin(); i != final_tris.end(); i++)
		{
			// calculate face normal via cross product of two planes of the triangle
			double x1 = (*i).p[0].x - (*i).p[1].x;
			double y1 = (*i).p[0].y - (*i).p[1].y;
			double z1 = (*i).p[0].z - (*i).p[1].z;

			double x2 = (*i).p[1].x - (*i).p[2].x;
			double y2 = (*i).p[1].y - (*i).p[2].y;
			double z2 = (*i).p[1].z - (*i).p[2].z;

			(*i).normal.x = ( y1 * z2 ) - ( z1 * y2 );
			(*i).normal.y = ( z1 * x2 ) - ( x1 * z2 );
			(*i).normal.z = ( x1 * y2 ) - ( y1 * x2 );


			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
					//						Sleep(1);
				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}




		if(0 != status_text)
		{
			SetWindowText(status_text, "Creating welds...");
			UpdateWindow(status_text);
		}

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_tris.size());
		total_count = 0;

		// insert unique vertices into set
		set<mc_xyz> final_vertices;

		for(list<mc_tri>::const_iterator i = final_tris.begin(); i != final_tris.end(); i++)
		{
			try
			{
				final_vertices.insert((*i).p[0]);
				final_vertices.insert((*i).p[1]);
				final_vertices.insert((*i).p[2]);
			}
			catch(...)
			{
				return -1;
			}

			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);
				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}




		// add indexes to the vertices, helps speed up triangle / vertex lookup
		long unsigned int index = 0;

		for(set<mc_xyz>::iterator i = final_vertices.begin(); i != final_vertices.end(); i++)
			(*i).index = index++;

		// find 3 vertices for each triangle, adding the index to the triangle
		set<mc_xyz>::const_iterator find_iter;

		vector< vector< mc_xyz > > vertex_normals;

		try
		{
			vertex_normals.resize(final_vertices.size());
		}
		catch(...)
		{
			return -1;
		}

		if(0 != status_text)
		{
			SetWindowText(status_text, "Assigning welds...");
			UpdateWindow(status_text);
		}

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_tris.size());
		total_count = 0;

		for(list<mc_tri>::iterator i = final_tris.begin(); i != final_tris.end(); i++)
		{
			try
			{
				find_iter = final_vertices.find((*i).p[0]);

				if(find_iter == final_vertices.end())
				{
					MessageBox(NULL, "Error processing vertices. This is a bug, please report this behaviour.", APP_NAME, MB_OK);
					return -3;
				}

				(*i).p[0].index = (*find_iter).index;
				vertex_normals[(*find_iter).index].push_back((*i).normal);


				find_iter = final_vertices.find((*i).p[1]);

				if(find_iter == final_vertices.end())
				{
					MessageBox(NULL, "Error processing vertices. This is a bug, please report this behaviour.", APP_NAME, MB_OK);
					return -3;
				}

				(*i).p[1].index = (*find_iter).index;
				vertex_normals[(*find_iter).index].push_back((*i).normal);


				find_iter = final_vertices.find((*i).p[2]);

				if(find_iter == final_vertices.end())
				{
					MessageBox(NULL, "Error processing vertices. This is a bug, please report this behaviour.", APP_NAME, MB_OK);
					return -3;
				}

				(*i).p[2].index = (*find_iter).index;
				vertex_normals[(*find_iter).index].push_back((*i).normal);
			}
			catch(...)
			{
				return -1;
			}


			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}




		// write to file
		ofstream out(internal_params.export_file_name.GetBuffer());

		if(out.fail())
			return -2;




		if(0 != status_text)
		{
			SetWindowText(status_text, "Writing vertices...");
			UpdateWindow(status_text);
		}

		out << setiosflags(ios_base::fixed);
		out << setprecision(18);

		out << "# Exported from Julia 4D 2 - Copyright (c) 2004-2005 Shawn Halayka -- shalayka@sasktel.net" << endl;
		out << "#" << endl;
		out << "# OBJ tesselation algorithm:" << endl;
		out << "# Polygonising a scalar field - http://astronomy.swin.edu.au/~pbourke/modelling/polygonise/" << endl;
		out << "# Also known as: \"3D Contouring\", \"Marching Cubes\", \"Surface Reconstruction\"" << endl;
		out << "#" << endl;
		out << "# Written by Paul Bourke" << endl;
		out << "# May 1997" << endl;
		out << "#" << endl;
		out << "# Based on tables by Cory Gene Bloyd along with" << endl;
		out << "# additional example source code marchingsource.cpp" << endl;
		out << "#" << endl;
		out << "# All 3rd party provided source code is Copyright (c) 2004-2005 by their respective authors." << endl << endl;

		out << "o julia4d2_fractal" << endl;
		out << "g julia4d2_fractal" << endl << endl;

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_vertices.size());
		total_count = 0;

		out << "# " << num_steps_total << " vertices" << "\n";

		for(set<mc_xyz>::const_iterator i = final_vertices.begin(); i != final_vertices.end(); i++)
		{
			out << "v " << (*i).x << " " << (*i).y << " " << (*i).z << "\n";

			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}




		if(0 != status_text)
		{
			SetWindowText(status_text, "Writing vertex normals...");
			UpdateWindow(status_text);
		}

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_vertices.size());
		total_count = 0;

		out << "\n# " << num_steps_total << " vertex normals" << "\n";

		long unsigned int vertex_index = 0;

		for(set<mc_xyz>::const_iterator i = final_vertices.begin(); i != final_vertices.end(); i++, vertex_index++)
		{
			// normalized smooth
			double temp_x = 0.0;
			double temp_y = 0.0;
			double temp_z = 0.0;

			// add up all face normals associated with this vertex
			for(size_t i = 0; i < vertex_normals[vertex_index].size(); i++)
			{
				double local_temp_x = vertex_normals[vertex_index][i].x;
				double local_temp_y = vertex_normals[vertex_index][i].y;
				double local_temp_z = vertex_normals[vertex_index][i].z;

				double local_len = sqrt(local_temp_x*local_temp_x + local_temp_y*local_temp_y + local_temp_z*local_temp_z);

				if(local_len != 1.0f)
				{
					local_temp_x /= local_len;
					local_temp_y /= local_len;
					local_temp_z /= local_len;
				}

				temp_x += local_temp_x;
				temp_y += local_temp_y;
				temp_z += local_temp_z;
			}

			// average them using a flat linear
			temp_x /= vertex_normals[vertex_index].size();
			temp_y /= vertex_normals[vertex_index].size();
			temp_z /= vertex_normals[vertex_index].size();

			// normalize the final result
			double len = sqrt(temp_x*temp_x + temp_y*temp_y + temp_z*temp_z);

			if(len != 1.0)
			{
				temp_x /= len;
				temp_y /= len;
				temp_z /= len;
			}

			out << "vn " << -temp_x << " " << -temp_y << " " << -temp_z << "\n";


			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}




		if(0 != status_text)
		{
			SetWindowText(status_text, "Writing triangles...");
			UpdateWindow(status_text);
		}

		percent_done = 0;
		last_percent_done = 0;
		num_steps_total = static_cast<long unsigned int>(final_tris.size());
		total_count = 0;

		out << "\n# " << num_steps_total << " triangle(s)\n";

		for(list<mc_tri>::const_iterator i = final_tris.begin(); i != final_tris.end(); i++)
		{
			out << "f "
				<< (*i).p[2].index + 1 << "//" << (*i).p[2].index + 1 << " "
				<< (*i).p[1].index + 1 << "//" << (*i).p[1].index + 1 << " "
				<< (*i).p[0].index + 1 << "//" << (*i).p[0].index + 1
				<< "\n";


			total_count++;

			if(0 != progressbar)
			{
				percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);

				if(percent_done > last_percent_done || percent_done == 100)
				{
					last_percent_done = percent_done;
					SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

				}
			}

			if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
			{
				return 0;
			}
		}

		out.close();
	}

	if(cancelled)
		return 0;
	else
		return 1;
}













int quaternion_julia_set::SaveHopfMapJ4D2VOX(const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event)
{
	SendMessage(progressbar, PBM_SETPOS, 0, 0);



	const double w_step_size = (internal_params.w_max - internal_params.w_min) / (internal_params.w_resolution - 1);
	const double x_step_size = (internal_params.x_max - internal_params.x_min) / (internal_params.x_resolution - 1);
	const double y_step_size = (internal_params.y_max - internal_params.y_min) / (internal_params.y_resolution - 1);
	const double z_step_size = (internal_params.z_max - internal_params.z_min) / (internal_params.z_resolution - 1);

	long unsigned int num_steps_total = internal_params.w_resolution * internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution;
	long unsigned int total_count = 0;
	unsigned short int percent_done = 0;
	unsigned short int last_percent_done = 0;

	quaternion_t<double> Z(internal_params.x_max, internal_params.y_max, internal_params.z_max, internal_params.w_max);
	quaternion_t<double> C(internal_params.c_x, internal_params.c_y, internal_params.c_z, internal_params.c_w);

	cache_data set_data;

	if(0 != status_text)
	{
		SetWindowText(status_text, "Checking cache...");
		UpdateWindow(status_text);
	}

	int ret = cache.GetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, status_text);

	if(ret != 1)
	{
		if(ret == -1)
			MessageBox(0, "Insufficient memory was available to read set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(ret == -2)
			MessageBox(0, "A file error occurred while reading set data from the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		set_data.w_step_number = set_data.x_step_number = set_data.y_step_number = set_data.z_step_number = 0;
		try { set_data.data.resize(internal_params.w_resolution * internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution); } catch(...) { return -1; }
	}
	else
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Calculating set...");
			UpdateWindow(status_text);
		}

		for(long unsigned int temp_w_step_number = 0; temp_w_step_number < set_data.w_step_number; temp_w_step_number++)
			Z.w -= w_step_size;

		for(long unsigned int temp_z_step_number = 0; temp_z_step_number < set_data.z_step_number; temp_z_step_number++)
			Z.z -= z_step_size;

		for(long unsigned int temp_y_step_number = 0; temp_y_step_number < set_data.y_step_number; temp_y_step_number++)
			Z.y -= y_step_size;

		for(long unsigned int temp_x_step_number = 0; temp_x_step_number < set_data.x_step_number; temp_x_step_number++)
			Z.x -= x_step_size;

		total_count = set_data.w_step_number*internal_params.z_resolution*internal_params.y_resolution*internal_params.x_resolution + set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;
		percent_done = unsigned short(double((double)total_count / (double)num_steps_total) * (double)100.0f);
	}


	SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

	double length = 0.0f;

	quaternion_t<double> *final_z;
	quaternion_t<double> *final_c;

	if(false == internal_params.swap_z_and_c)
	{
		final_z = &Z;
		final_c = &C;
	}
	else
	{
		final_z = &C;
		final_c = &Z;
	}

	bool cancelled = false;

	for(; set_data.w_step_number < internal_params.w_resolution; set_data.w_step_number++, Z.w -= w_step_size, Z.z = internal_params.z_max, set_data.z_step_number = 0)
	{
		for(; set_data.z_step_number < internal_params.z_resolution; set_data.z_step_number++, Z.z -= z_step_size, Z.y = internal_params.y_max, set_data.y_step_number = 0)
		{
			for(; set_data.y_step_number < internal_params.y_resolution; set_data.y_step_number++, Z.y -= y_step_size, Z.x = internal_params.x_max, set_data.x_step_number = 0)
			{
				for(; set_data.x_step_number < internal_params.x_resolution; set_data.x_step_number++, Z.x -= x_step_size)
				{
					long unsigned int index = set_data.w_step_number*internal_params.z_resolution*internal_params.y_resolution*internal_params.x_resolution + set_data.z_step_number*internal_params.y_resolution*internal_params.x_resolution + set_data.y_step_number*internal_params.x_resolution + set_data.x_step_number;

					if(internal_params.infinity > (length = (this->*iterator_ptr)(*final_z, *final_c, internal_params.iterations, internal_params.infinity, internal_params.a)))
						set_data.data[index] = true;
					else
						set_data.data[index] = false;

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

					if(WAIT_OBJECT_0 == WaitForSingleObject(cancel_event, 0))
					{
						int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_W_MAJOR_PLANE, status_text);

						if(ret != 1)
						{
							if(ret == -1)
								MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

							if(ret == -2)
								MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
						}

						set_data.z_step_number = internal_params.z_resolution;
						set_data.y_step_number = internal_params.y_resolution;
						set_data.x_step_number = internal_params.x_resolution;
						set_data.w_step_number = internal_params.w_resolution;
						cancelled = true;
					}
				}
			}
		}
	}


	if(!cancelled)
	{
		int ret = cache.SetCacheData(internal_params, set_data, J4D2_CACHE_NONSAMPLEDTYPE, J4D2_CACHE_W_MAJOR_PLANE, status_text);

		if(ret != 1)
		{
			if(ret == -1)
				MessageBox(0, "Insufficient memory was available to write set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);

			if(ret == -2)
				MessageBox(0, "A file error occurred while writing set data to the cache.", APP_NAME, MB_OK|MB_ICONSTOP);
		}
	}





	vector<bool> voxels;
	dimension_info d_info;

	if(!cancelled)
	{
		volumize_hopf_map vhm;
		int ret = vhm.VoxelizeHopfMap(internal_params, set_data.data, voxels, d_info, progressbar, status_text, cancel_event);
		if(ret != 1)
			return ret;
	}


	// clear out the bool vector that we don't need anymore
	if(!cancelled)
	{
		vector<bool> temp_data;
		set_data.data = temp_data;
	}




	if(cancelled)
	{
		return 0;
	}
	else
	{
		if(0 != status_text)
		{
			SetWindowText(status_text, "Writing to J4D2VOX file...");
			UpdateWindow(status_text);
		}

		return WriteJ4D2VoxelFile(
			voxels,
			internal_params.export_file_name.GetBuffer(),
			static_cast<float>(d_info.x_min),
			static_cast<float>(d_info.x_max),
			static_cast<float>(d_info.y_min),
			static_cast<float>(d_info.y_max),
			static_cast<float>(d_info.z_min),
			static_cast<float>(internal_params.z_max),
			internal_params.hopf_field_resolution,
			internal_params.hopf_field_resolution,
			internal_params.hopf_field_resolution);
	}
}
