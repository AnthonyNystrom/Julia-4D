#ifndef QGUI_set_parameters_H
#define QGUI_set_parameters_H


#include <string>
using std::string;


class set_parameters
{
public:

	set_parameters(void);
	bool operator==(const set_parameters &rhs);

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
	double isovalue_percent;
	double avg_percent;
	long unsigned int avg_passes;

	long unsigned int render_to_type;
};


#endif