/*
Julia4D - Real-time quaternion_t<double>-based fractal generator / renderer for Win32.
Written in C++, uses the Microsoft DirectX 9 library for graphics display.

Copyright (C) 2004 Shawn Halayka

email: shalayka@sasktel.net

This program is free software; you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details. 

You should have received a copy of the GNU General Public License along with this program;
if not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#ifndef QUATERNION_JULIA_SET_H
#define QUATERNION_JULIA_SET_H


#include "defines.h"
#include "quaternion_math.h"
#include "julia4d2Doc.h"
#include "j4d2_cache_system.h"
#include "marching_cubes_types.h"
#include "eqparse.h"




extern int MC_EdgeTable[256];
extern int MC_TriTable[256][16];



#define MC_TOTALLY_FILLED	200000.0
#define MC_ISOVALUE			(MC_TOTALLY_FILLED / 2.0)
#define MC_TOTALLY_UNFILLED	0.0



class quaternion_julia_set
{
public:
	quaternion_julia_set(const Cjulia4d2Doc &src_params);

	// rendered to screen 
	int GetPointSet(const HWND &progressbar, const HWND &status_text, POS_NORMAL_COLOUR_VERTEX *&vertices, long unsigned int &vertex_count, const HANDLE &cancel_event);
	int GetHopfMap(const HWND &progressbar, const HWND &status_text, POS_NORMAL_COLOUR_VERTEX *&vertices, long unsigned int &vertex_count, const HANDLE &cancel_event);

	// saved to file
	int SavePointSetCSV(const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event);
	int SaveHopfMapCSV(const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event);
	int SaveJ4D2VOX(const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event);
	int SaveHopfMapJ4D2VOX(const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event);
	int SaveOBJ(const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event);
	int SaveHopfMapOBJ(const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event);

	// custom formula initializer
	bool SetupCustomFormulaText(const string &src_formula_text, string &error_string)
	{
		quaternion_t<double> C(internal_params.c_x, internal_params.c_y, internal_params.c_z, internal_params.c_w);

		if(eqparser.Setup(src_formula_text, error_string, q_math, C, internal_params.a))
		{
			internal_params.formula_text = eqparser.GetUniqueFormulaString();
			return true;
		}
		else
			return false;
	}

protected:
	string GetMulVarText(const int &mul_var);
	void setup_quaternion_math_class(void);

	typedef double (quaternion_julia_set::*ip)(const quaternion_t<double> &, const quaternion_t<double> &, const long unsigned int &, const double &, const double &) throw();  
	void set_iterator_ptr(ip &iterator_ptr);

	// inlining isn't done because these are always accessed through a pointer
	// i will probably get rid of the __forceinline keyword in the future when i have time to test it thoroughly
	__forceinline double custom_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double exponential_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double quadratic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double lambda_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double funky_blobs_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);

	__forceinline double sine_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double sine_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double sine_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double cosine_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double cosine_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double cosine_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double tangent_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double tangent_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double tangent_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);

	__forceinline double hyperbolic_sine_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_sine_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_sine_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_cosine_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_cosine_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_cosine_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_tangent_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_tangent_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_tangent_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);

	__forceinline double cosecant_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double cosecant_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double cosecant_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double secant_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double secant_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double secant_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double cotangent_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double cotangent_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double cotangent_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	
	__forceinline double hyperbolic_cosecant_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_cosecant_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_cosecant_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_secant_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_secant_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_secant_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_cotangent_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_cotangent_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);
	__forceinline double hyperbolic_cotangent_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a);

	Cjulia4d2Doc internal_params;

	quaternion_math_base<double> *q_math;
	quaternion_math_default<double> q_math_default;
	quaternion_math_constant_column<double> q_math_constant_column;
	quaternion_math_custom<double> q_math_custom;
	quaternion_math_custom_constant_column<double> q_math_custom_constant_column;

	j4d2_cache_system cache;
	ip iterator_ptr;


	quaternion_julia_set_equation_parser eqparser;



	__forceinline mc_xyz QuatFractalLinearInterp(mc_xyz p1, mc_xyz p2, double valp1, double valp2)
	{
		// if sample rate is 0, then we've already done all the sampling they want
		// is this really a good place to put this check? probably not
		if(internal_params.samples == 0)
		{
			if(valp1 == MC_TOTALLY_FILLED)
				return p1;
			else
				return p2;
		}

		double x_step_size = (p2.x - p1.x) * (1.0 / static_cast<double>(internal_params.samples+1) );
		double y_step_size = (p2.y - p1.y) * (1.0 / static_cast<double>(internal_params.samples+1) );
		double z_step_size = (p2.z - p1.z) * (1.0 / static_cast<double>(internal_params.samples+1) );

		long unsigned int samples_on = 0;

		quaternion_t<double> Z(p1.x, p1.y, p1.z, internal_params.z_w);
		quaternion_t<double> C(internal_params.c_x, internal_params.c_y, internal_params.c_z, internal_params.c_w);


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

		Z.x += x_step_size;
		Z.y += y_step_size;
		Z.z += z_step_size;

		for(long unsigned int i = 0; i < internal_params.samples; i++)
		{
			bool include = false;

			if(internal_params.infinity > (length = (this->*iterator_ptr)(*final_z, *final_c, internal_params.iterations, internal_params.infinity, internal_params.a)))
				include = true;
			else
				include = false;

			if(true == internal_params.reverse_set)
				include = !include;

			if(true == include)
				samples_on++;

			Z.x += x_step_size;
			Z.y += y_step_size;
			Z.z += z_step_size;
		}

		// now that we have the number of samples filled, we need to walk that many steps away from the vertex that is MT_TOTALLY_FILLED
		// ie: if 2 of 2 samples is filled, then we need to interpolate to 2/3 of the way away from the MT_TOTALLY_FILLED
		// ie: samples_on / (samples+1)
		double distance_multiplier = static_cast<double>(samples_on) / static_cast<double>(internal_params.samples + 1);
	
		// two of these will be 0s for all vertex interps, but whatever
		double x_surface_distance = fabs(p2.x - p1.x) * distance_multiplier;
		double y_surface_distance = fabs(p2.y - p1.y) * distance_multiplier;
		double z_surface_distance = fabs(p2.z - p1.z) * distance_multiplier;

		mc_xyz final_p;

		if(valp1 == MC_TOTALLY_FILLED)
		{
			if(p1.x <= p2.x)
				final_p.x = p1.x + x_surface_distance;
			else
				final_p.x = p1.x - x_surface_distance;

			if(p1.y <= p2.y)
				final_p.y = p1.y + y_surface_distance;
			else
				final_p.y = p1.y - y_surface_distance;

			if(p1.z <= p2.z)
				final_p.z = p1.z + z_surface_distance;
			else
				final_p.z = p1.z - z_surface_distance;
		}
		else
		{
			if(p2.x <= p1.x)
				final_p.x = p2.x + x_surface_distance;
			else
				final_p.x = p2.x - x_surface_distance;

			if(p2.y <= p1.y)
				final_p.y = p2.y + y_surface_distance;
			else
				final_p.y = p2.y - y_surface_distance;

			if(p2.z <= p1.z)
				final_p.z = p2.z + z_surface_distance;
			else
				final_p.z = p2.z - z_surface_distance;
		}

		return final_p;
	}


	__forceinline int QuatFractalPolygonise(mc_gridcell grid, mc_tri *triangles)
	{
		int i,ntriang;
		int cubeindex;
		mc_xyz vertlist[12];

		// Determine the index into the edge table which
		// tells us which vertices are inside of the surface

		cubeindex = 0;
		if (grid.val[0] < MC_ISOVALUE) cubeindex |= 1;
		if (grid.val[1] < MC_ISOVALUE) cubeindex |= 2;
		if (grid.val[2] < MC_ISOVALUE) cubeindex |= 4;
		if (grid.val[3] < MC_ISOVALUE) cubeindex |= 8;
		if (grid.val[4] < MC_ISOVALUE) cubeindex |= 16;
		if (grid.val[5] < MC_ISOVALUE) cubeindex |= 32;
		if (grid.val[6] < MC_ISOVALUE) cubeindex |= 64;
		if (grid.val[7] < MC_ISOVALUE) cubeindex |= 128;

		/* Cube is entirely in/out of the surface */
		if (MC_EdgeTable[cubeindex] == 0)
			return(0);


		if (MC_EdgeTable[cubeindex] & 1)
			vertlist[0] = QuatFractalLinearInterp(grid.p[0], grid.p[1], grid.val[0], grid.val[1]);

		if (MC_EdgeTable[cubeindex] & 2)
			vertlist[1] = QuatFractalLinearInterp(grid.p[1], grid.p[2], grid.val[1], grid.val[2]);

		if (MC_EdgeTable[cubeindex] & 4)
			vertlist[2] = QuatFractalLinearInterp(grid.p[2], grid.p[3], grid.val[2], grid.val[3]);

		if (MC_EdgeTable[cubeindex] & 8)
			vertlist[3] = QuatFractalLinearInterp(grid.p[3], grid.p[0], grid.val[3], grid.val[0]);

		if (MC_EdgeTable[cubeindex] & 16)
			vertlist[4] = QuatFractalLinearInterp(grid.p[4], grid.p[5], grid.val[4], grid.val[5]);

		if (MC_EdgeTable[cubeindex] & 32)
			vertlist[5] = QuatFractalLinearInterp(grid.p[5], grid.p[6], grid.val[5], grid.val[6]);

		if (MC_EdgeTable[cubeindex] & 64)
			vertlist[6] = QuatFractalLinearInterp(grid.p[6], grid.p[7], grid.val[6], grid.val[7]);

		if (MC_EdgeTable[cubeindex] & 128)
			vertlist[7] = QuatFractalLinearInterp(grid.p[7], grid.p[4], grid.val[7], grid.val[4]);

		if (MC_EdgeTable[cubeindex] & 256)
			vertlist[8] = QuatFractalLinearInterp(grid.p[0], grid.p[4], grid.val[0], grid.val[4]);

		if (MC_EdgeTable[cubeindex] & 512)
			vertlist[9] = QuatFractalLinearInterp(grid.p[1], grid.p[5], grid.val[1], grid.val[5]);

		if (MC_EdgeTable[cubeindex] & 1024)
			vertlist[10] = QuatFractalLinearInterp(grid.p[2], grid.p[6], grid.val[2], grid.val[6]);

		if (MC_EdgeTable[cubeindex] & 2048)
			vertlist[11] = QuatFractalLinearInterp(grid.p[3], grid.p[7], grid.val[3], grid.val[7]);


		/* Create the mc_tri */
		ntriang = 0;
		for (i=0;MC_TriTable[cubeindex][i]!=-1;i+=3) {
			triangles[ntriang].p[0] = vertlist[MC_TriTable[cubeindex][i  ]];
			triangles[ntriang].p[1] = vertlist[MC_TriTable[cubeindex][i+1]];
			triangles[ntriang].p[2] = vertlist[MC_TriTable[cubeindex][i+2]];
			ntriang++;
		}

		return(ntriang);
	}
};




#endif


