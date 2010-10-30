#include "stdafx.h"

#include "set_parameters.h"
#include "defines.h"


set_parameters::set_parameters(void)
{
	iterations = 8;
	infinity = 4.0;
	resolution = 100;
	formula = 1;
	formula_text = "Classic: Z = Z^a + C";

	c_x = 0.0;
	c_y = 0.0;
	c_z = 0.0;
	c_w = 0.0;

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

	samples = 1;
	isovalue_percent = 50.0;
	avg_percent = 0.0;
	avg_passes = 1;

	render_to_type = POINT_SET;
};


bool set_parameters::operator==(const set_parameters &rhs)
{
	if(iterations != rhs.iterations)
		return false;

	if(infinity != rhs.infinity)
		return false;

	if(resolution != rhs.resolution)
		return false;

	if(formula != rhs.formula)
		return false;

	if(formula_text != rhs.formula_text)
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


	if(samples != rhs.samples)
		return false;

	if(isovalue_percent != rhs.isovalue_percent)
		return false;

	if(avg_percent != rhs.avg_percent)
		return false;

	if(avg_passes != rhs.avg_passes)
		return false;

	if(render_to_type != rhs.render_to_type)
		return false;


	return true;
}

