#include "stdafx.h"


#include "volumize_hopf_map.h"
#include "defines.h"
#include "quaternion_math.h"
#include "utilities.h"

#include <cfloat>


#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using std::ostringstream;
using std::ofstream;
using std::endl;
using std::setprecision;
using std::setiosflags;
using std::ios_base;




int volumize_hopf_map::GetDimensions(const Cjulia4d2Doc &internal_params, const vector<bool> src_vect, dimension_info &d_info, const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event)
{
	// find max dimensions of point cloud
	d_info.x_max = FLT_MIN;
	d_info.x_min = FLT_MAX;
	d_info.y_max = FLT_MIN;
	d_info.y_min = FLT_MAX;
	d_info.z_max = FLT_MIN;
	d_info.z_min = FLT_MAX;

	if(0 != status_text)
	{
		SetWindowText(status_text, "Finding 3D boundary...");
		UpdateWindow(status_text);
	}

	long unsigned int num_steps_total = internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution * internal_params.w_resolution;
	long unsigned int total_count = 0;
	short unsigned int last_percent_done = 0;
	short unsigned int percent_done = 0;


	const double w_step_size = (internal_params.w_max - internal_params.w_min) / (internal_params.w_resolution - 1);
	const double x_step_size = (internal_params.x_max - internal_params.x_min) / (internal_params.x_resolution - 1);
	const double y_step_size = (internal_params.y_max - internal_params.y_min) / (internal_params.y_resolution - 1);
	const double z_step_size = (internal_params.z_max - internal_params.z_min) / (internal_params.z_resolution - 1);


	quaternion_t<double> Z;

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

					if(true == src_vect[index])
					{
						POS_NORMAL_COLOUR_VERTEX temp_vertex;
						PerformHopfMapping(temp_vertex, Z);

						if(temp_vertex.x > d_info.x_max)
							d_info.x_max = temp_vertex.x;

						if(temp_vertex.x < d_info.x_min)
							d_info.x_min = temp_vertex.x;

						if(temp_vertex.y > d_info.y_max)
							d_info.y_max = temp_vertex.y;

						if(temp_vertex.y < d_info.y_min)
							d_info.y_min = temp_vertex.y;

						if(temp_vertex.z > d_info.z_max)
							d_info.z_max = temp_vertex.z;

						if(temp_vertex.z < d_info.z_min)
							d_info.z_min = temp_vertex.z;
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
						x = internal_params.x_resolution;
						y = internal_params.y_resolution;
						z = internal_params.z_resolution;
						w = internal_params.w_resolution;
						return 0;
					}
				}
			}
		}
	}








	// find span length across each dimension
	d_info.x_span = fabs(d_info.x_max - d_info.x_min);
	d_info.y_span = fabs(d_info.y_max - d_info.y_min);
	d_info.z_span = fabs(d_info.z_max - d_info.z_min);


	// find the maximum span length from all 3 dimensions
	float max_span = d_info.x_span;

	if(d_info.y_span > max_span)
		max_span = d_info.y_span;

	if(d_info.z_span > max_span)
		max_span = d_info.z_span;


	// if the maximum span is 0ish, make make it 1.0 by default
	if(max_span < 0.0001f)
		max_span = 1.0f;


	// how much do we need to expand each dimension
	// so that the point cloud bounding area will be a perfect cube
	// with sides that are the same length as the longest span
	float x_expand = max_span - d_info.x_span;
	float y_expand = max_span - d_info.y_span;
	float z_expand = max_span - d_info.z_span;


	// do any necessary expanding, the max dimension won't be expanded of course, unless all three were 0ish in span length
	d_info.x_min -= (x_expand / 2.0f);
	d_info.x_max += (x_expand / 2.0f);
	d_info.y_min -= (y_expand / 2.0f);
	d_info.y_max += (y_expand / 2.0f);
	d_info.z_min -= (z_expand / 2.0f);
	d_info.z_max += (z_expand / 2.0f);


	// set new span lengths
	d_info.x_span = fabs(d_info.x_max - d_info.x_min);
	d_info.y_span = fabs(d_info.y_max - d_info.y_min);
	d_info.z_span = fabs(d_info.z_max - d_info.z_min);

	return 1;
}





int volumize_hopf_map::FillBoolField(const Cjulia4d2Doc &internal_params, const vector<bool> src_vect, vector<bool> &bool_field, const dimension_info &d_info, const bool &add_single_border, const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event)
{
	unsigned char border_add = 0;
	unsigned char border_offset = 0;

	if(true == add_single_border)
	{
		border_add = 2;
		border_offset = 1;
	}



	if(0 != status_text)
	{
		SetWindowText(status_text, "Allocating memory...");
		UpdateWindow(status_text);
	}

	bool_field.clear();
	try { bool_field.resize((internal_params.hopf_field_resolution+border_add) * (internal_params.hopf_field_resolution+border_add) * (internal_params.hopf_field_resolution+border_add), false); } catch(...) { return -1; }

	const float x_step_size = (d_info.x_max - d_info.x_min) / static_cast<float>(internal_params.hopf_field_resolution - 1);
	const float y_step_size = (d_info.y_max - d_info.y_min) / static_cast<float>(internal_params.hopf_field_resolution - 1);
	const float z_step_size = (d_info.z_max - d_info.z_min) / static_cast<float>(internal_params.hopf_field_resolution - 1);

	const float x_half_step_size = x_step_size / 2.0f;
	const float y_half_step_size = y_step_size / 2.0f;
	const float z_half_step_size = z_step_size / 2.0f;


	if(0 != status_text)
	{
		SetWindowText(status_text, "Snapping to grid...");
		UpdateWindow(status_text);
	}

	long unsigned int num_steps_total = internal_params.x_resolution * internal_params.y_resolution * internal_params.z_resolution * internal_params.w_resolution;
	long unsigned int total_count = 0;
	short unsigned int last_percent_done = 0;
	short unsigned int percent_done = 0;


	const double orig_w_step_size = (internal_params.w_max - internal_params.w_min) / (internal_params.w_resolution - 1);
	const double orig_x_step_size = (internal_params.x_max - internal_params.x_min) / (internal_params.x_resolution - 1);
	const double orig_y_step_size = (internal_params.y_max - internal_params.y_min) / (internal_params.y_resolution - 1);
	const double orig_z_step_size = (internal_params.z_max - internal_params.z_min) / (internal_params.z_resolution - 1);


	quaternion_t<double> Z;

	Z.w = internal_params.w_max;
	Z.x = internal_params.x_max;
	Z.y = internal_params.y_max;
	Z.z = internal_params.z_max;

	for(long unsigned int w = 0; w < internal_params.w_resolution; w++, Z.w -= orig_w_step_size, Z.z = internal_params.z_max)
	{
		for(long unsigned int z = 0; z < internal_params.z_resolution; z++, Z.z -= orig_z_step_size, Z.y = internal_params.y_max)
		{
			for(long unsigned int y = 0; y < internal_params.y_resolution; y++, Z.y -= orig_y_step_size, Z.x = internal_params.x_max)
			{
				for(long unsigned int x = 0; x < internal_params.x_resolution; x++, Z.x -= orig_x_step_size)
				{
					long unsigned int index = w*internal_params.z_resolution*internal_params.y_resolution*internal_params.x_resolution + z*internal_params.y_resolution*internal_params.x_resolution + y*internal_params.x_resolution + x;

					if(true == src_vect[index])
					{
						POS_NORMAL_COLOUR_VERTEX temp_vertex;
						PerformHopfMapping(temp_vertex, Z);

						long unsigned int x = 0;
						float x_pos = d_info.x_max - x_half_step_size;

						for(long unsigned int x_index = 0; x_index < internal_params.hopf_field_resolution; x_index++)
						{
							x = x_index;

							if(temp_vertex.x >= x_pos)
								break;

							x_pos -= x_step_size;
						}

						long unsigned int y = 0;
						float y_pos = d_info.y_max - y_half_step_size;

						for(long unsigned int y_index = 0; y_index < internal_params.hopf_field_resolution; y_index++)
						{
							y = y_index;

							if(temp_vertex.y >= y_pos)
								break;

							y_pos -= y_step_size;
						}

						long unsigned int z = 0;
						float z_pos = d_info.z_max - z_half_step_size;

						for(long unsigned int z_index = 0; z_index < internal_params.hopf_field_resolution; z_index++)
						{
							z = z_index;

							if(temp_vertex.z >= z_pos)
								break;

							z_pos -= z_step_size;
						}


						size_t bool_field_index = (z+border_offset)*(internal_params.hopf_field_resolution+border_add)*(internal_params.hopf_field_resolution+border_add) + (y+border_offset)*(internal_params.hopf_field_resolution+border_add) + (x+border_offset);
						bool_field[bool_field_index] = true;
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
						x = internal_params.x_resolution;
						y = internal_params.y_resolution;
						z = internal_params.z_resolution;
						w = internal_params.w_resolution;

						return 0;
					}
				}
			}
		}
	}



	if(internal_params.reverse_set)
	{
		num_steps_total = internal_params.hopf_field_resolution*internal_params.hopf_field_resolution*internal_params.hopf_field_resolution;
		total_count = 0;
		percent_done = 0;
		last_percent_done = 0;

		SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

		if(0 != status_text)
		{
			SetWindowText(status_text, "Reversing set...");
			UpdateWindow(status_text);
		}

		for(long unsigned int z = 0; z < internal_params.hopf_field_resolution; z++)
		{
			for(long unsigned int y = 0; y < internal_params.hopf_field_resolution; y++)
			{
				for(long unsigned int x = 0; x < internal_params.hopf_field_resolution; x++)
				{
					size_t bool_field_index = (z+border_offset)*(internal_params.hopf_field_resolution+border_add)*(internal_params.hopf_field_resolution+border_add) + (y+border_offset)*(internal_params.hopf_field_resolution+border_add) + (x+border_offset);

					bool_field[bool_field_index] = !bool_field[bool_field_index];


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

	return 1;	
}







int volumize_hopf_map::PolygoniseHopfMap(const Cjulia4d2Doc &internal_params, const vector<bool> src_vect, list<mc_tri> &triangles, const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event)
{
	bool cancelled = false;

	triangles.clear();

	dimension_info d_info;
	int ret = GetDimensions(internal_params, src_vect, d_info, progressbar, status_text, cancel_event);
	if(ret != 1)
		return ret;

	vector<bool> bool_field;
	ret = FillBoolField(internal_params, src_vect, bool_field, d_info, true, progressbar, status_text, cancel_event);
	if(ret != 1)
		return ret;



	if(!cancelled)
	{
		long unsigned int num_steps_total = static_cast<long unsigned int>(bool_field.size());
		long unsigned int total_count = 0;
		short unsigned int percent_done = 0;
		short unsigned int last_percent_done = 0;

		SendMessage(progressbar, PBM_SETPOS, percent_done, 0);



		double isovalue = 0.5;


		if(0 != status_text)
		{
			SetWindowText(status_text, "Generating mesh data...");
			UpdateWindow(status_text);
		}

		num_steps_total = (internal_params.hopf_field_resolution+1) * (internal_params.hopf_field_resolution+1) * (internal_params.hopf_field_resolution+1);
		total_count = 0;
		percent_done = 0;
		last_percent_done = 0;


		SendMessage(progressbar, PBM_SETPOS, percent_done, 0);

		// temp variables
		mc_gridcell temp_cell;
		mc_tri temp_tris[5];


		const float x_step_size = (d_info.x_max - d_info.x_min) / static_cast<float>(internal_params.hopf_field_resolution - 1);
		const float y_step_size = (d_info.y_max - d_info.y_min) / static_cast<float>(internal_params.hopf_field_resolution - 1);
		const float z_step_size = (d_info.z_max - d_info.z_min) / static_cast<float>(internal_params.hopf_field_resolution - 1);

		// starting positions (taking into account blank border)
		double temp_x_max = d_info.x_max + x_step_size;
		double temp_y_max = d_info.y_max + y_step_size;
		double temp_z_max = d_info.z_max + z_step_size;

		for(long unsigned int z = 0; z < internal_params.hopf_field_resolution+1; z++)
		{
			for(long unsigned int y = 0; y < internal_params.hopf_field_resolution+1; y++)
			{
				for(long unsigned int x = 0; x < internal_params.hopf_field_resolution+1; x++)
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

					index = (z+z_offset)*(internal_params.hopf_field_resolution+2)*(internal_params.hopf_field_resolution+2) + (y+y_offset)*(internal_params.hopf_field_resolution+2) + (x+x_offset);
					temp_cell.val[0] = bool_field[index];


					// cube vertex 1
					z_offset = 1;
					y_offset = 0;
					x_offset = 1;
					temp_cell.p[1].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[1].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[1].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.hopf_field_resolution+2)*(internal_params.hopf_field_resolution+2) + (y+y_offset)*(internal_params.hopf_field_resolution+2) + (x+x_offset);
					temp_cell.val[1] = bool_field[index];


					// cube vertex 2
					z_offset = 0;
					y_offset = 0;
					x_offset = 1;
					temp_cell.p[2].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[2].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[2].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.hopf_field_resolution+2)*(internal_params.hopf_field_resolution+2) + (y+y_offset)*(internal_params.hopf_field_resolution+2) + (x+x_offset);
					temp_cell.val[2] = bool_field[index];


					// cube vertex 3
					z_offset = 0;
					y_offset = 0;
					x_offset = 0;
					temp_cell.p[3].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[3].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[3].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.hopf_field_resolution+2)*(internal_params.hopf_field_resolution+2) + (y+y_offset)*(internal_params.hopf_field_resolution+2) + (x+x_offset);
					temp_cell.val[3] = bool_field[index];


					// cube vertex 4
					z_offset = 1;
					y_offset = 1;
					x_offset = 0;
					temp_cell.p[4].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[4].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[4].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.hopf_field_resolution+2)*(internal_params.hopf_field_resolution+2) + (y+y_offset)*(internal_params.hopf_field_resolution+2) + (x+x_offset);
					temp_cell.val[4] = bool_field[index];


					// cube vertex 5
					z_offset = 1;
					y_offset = 1;
					x_offset = 1;
					temp_cell.p[5].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[5].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[5].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.hopf_field_resolution+2)*(internal_params.hopf_field_resolution+2) + (y+y_offset)*(internal_params.hopf_field_resolution+2) + (x+x_offset);
					temp_cell.val[5] = bool_field[index];


					// cube vertex 6
					z_offset = 0;
					y_offset = 1;
					x_offset = 1;
					temp_cell.p[6].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[6].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[6].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.hopf_field_resolution+2)*(internal_params.hopf_field_resolution+2) + (y+y_offset)*(internal_params.hopf_field_resolution+2) + (x+x_offset);
					temp_cell.val[6] = bool_field[index];


					// cube vertex 7
					z_offset = 0;
					y_offset = 1;
					x_offset = 0;
					temp_cell.p[7].z = temp_z_max - ((z+z_offset) * z_step_size);
					temp_cell.p[7].y = temp_y_max - ((y+y_offset) * y_step_size);
					temp_cell.p[7].x = temp_x_max - ((x+x_offset) * x_step_size);

					index = (z+z_offset)*(internal_params.hopf_field_resolution+2)*(internal_params.hopf_field_resolution+2) + (y+y_offset)*(internal_params.hopf_field_resolution+2) + (x+x_offset);
					temp_cell.val[7] = bool_field[index];



					int num_tris = 0;

					num_tris = GenericPolygonise(isovalue, temp_cell, temp_tris);

					try
					{
						for(unsigned short int i = 0; i < num_tris; i++)
							triangles.push_back(temp_tris[i]);
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

	if(cancelled)
		return 0;
	else
		return 1;
}


int volumize_hopf_map::VoxelizeHopfMap(const Cjulia4d2Doc &internal_params, const vector<bool> src_vect, vector<bool> &voxels, dimension_info &d_info, const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event)
{
	if(0 != status_text)
	{
		SetWindowText(status_text, "Allocating memory...");
		UpdateWindow(status_text);
	}

	int ret = GetDimensions(internal_params, src_vect, d_info, progressbar, status_text, cancel_event);
	if(ret != 1)
		return ret;

	ret = FillBoolField(internal_params, src_vect, voxels, d_info, false, progressbar, status_text, cancel_event);
	if(ret != 1)
		return ret;

	return 1;
}


