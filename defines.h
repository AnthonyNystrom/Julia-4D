// 1.92 fixes: fixed user pointer rendering code to properly handle very large primitive counts on older/less adequate hardware
// 1.92 added: proportional resolution. the resolution option is used for the longest dimension of the bounding box. the resolution in the
// other two dimensions are then scaled down proportionately by dimension length. minimum number of slices per dimension is three,
// regardless of bounding box dimension length (required for surface reconstruction purposes)
// - added: changed j4d2 voxel file format slightly

// can alter cache format to add network rendering support so that chunks can be set even if chunks before them are not
// (a solution with incomplete holes in it)
// otherwise there is no way to determine how to finish calculating the set if cancellation occurs while a remote computer
// is still calculating a chunk
// ... provide option to keep app alive until all network data is retrieved and connections are closed
// ... the remote computers will be notified at the time of cancellation and will automatically return whatever they've calculated up until
// that point in time. should data be RLE compressed before transmission back to the main computer?



#ifndef J4D2_DEFINES_H
#define J4D2_DEFINES_H


#include "stdafx.h"



#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif



#include <windows.h>





#define APP_NAME "Julia 4D 2"
#define APP_VERSION "1.92"
#define J4D2_FILE_HDR (CString("JULIA4D002"))
#define J4D2_FILE_HDR3 (CString("JULIA4D003"))
#define J4D2_FILE_HDR4 (CString("JULIA4D004"))

#define J4D2_CACHE_FILE_VERSION 1
#define J4D2_CACHE_FILE_HDR "JULIA4D012"
#define J4D2_CACHE_FILE_HDR_LEN (strlen(J4D2_CACHE_FILE_HDR))

#define J4D2_VOXEL_FILE_HDR "JULIA4D902"
#define J4D2_VOXEL_FILE_HDR_LEN (strlen(J4D2_VOXEL_FILE_HDR))


#define J4D2_CACHE_FOLDER_NAME (L"julia4d2cache")
#define J4D2_CACHE_FILE_EXTENSION (L"j4d2cache")
#define J4D2_ALL_FILE_EXTENSION (L"j4d2*")
#define J4D2_SETTINGS_FILE_EXTENSION (L"j4d2")

#define J4D2_CACHE_NONSAMPLEDTYPE			0
#define J4D2_CACHE_SURFACE_NONSAMPLEDTYPE	1

#define J4D2_CACHE_Z_MAJOR_PLANE	0
#define J4D2_CACHE_W_MAJOR_PLANE	1

#define BYTES_PER_MEGABYTE (1048576)

#define DEFAULT_ARCBALL_ROT_RADIUSf 0.75f
#define DEFAULT_ARCBALL_TRANS_RADIUSf 1.5f



#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2



#define WM_RECREATE_RENDERER		(WM_USER+101)
#define WM_SET_GEN_SUCCESS			(WM_USER+102)
#define WM_SET_GEN_FAILURE			(WM_USER+103)
#define WM_BOUNDING_GRID_CHANGED	(WM_USER+104)


#define POINT_SET 0
#define VOXEL_FILE 1
#define MESH_FILE 2
#define CSV_FILE 3



#define CUSTOM_ITERATOR 0

#define CLASSIC_ITERATOR 1
#define EXPONENTIAL_ITERATOR 2
#define QUADRATIC_ITERATOR 3
#define LAMBDA_ITERATOR 4
#define FUNKY_BLOBS_ITERATOR 5

#define SINE_ITERATOR 6
#define SINE_PLUS_ITERATOR 7
#define SINE_CLASSIC_ITERATOR 8

#define COSINE_ITERATOR 9
#define COSINE_PLUS_ITERATOR 10
#define COSINE_CLASSIC_ITERATOR 11

#define TANGENT_ITERATOR 12
#define TANGENT_PLUS_ITERATOR 13
#define TANGENT_CLASSIC_ITERATOR 14

#define COSECANT_ITERATOR 15
#define COSECANT_PLUS_ITERATOR 16
#define COSECANT_CLASSIC_ITERATOR 17

#define SECANT_ITERATOR 18
#define SECANT_PLUS_ITERATOR 19
#define SECANT_CLASSIC_ITERATOR 20

#define COTANGENT_ITERATOR 21
#define COTANGENT_PLUS_ITERATOR 22
#define COTANGENT_CLASSIC_ITERATOR 23

#define HYPERBOLIC_SINE_ITERATOR 24
#define HYPERBOLIC_SINE_PLUS_ITERATOR 25
#define HYPERBOLIC_SINE_CLASSIC_ITERATOR 26

#define HYPERBOLIC_COSINE_ITERATOR 27
#define HYPERBOLIC_COSINE_PLUS_ITERATOR 28
#define HYPERBOLIC_COSINE_CLASSIC_ITERATOR 29

#define HYPERBOLIC_TANGENT_ITERATOR 30
#define HYPERBOLIC_TANGENT_PLUS_ITERATOR 31
#define HYPERBOLIC_TANGENT_CLASSIC_ITERATOR 32

#define HYPERBOLIC_COSECANT_ITERATOR 33
#define HYPERBOLIC_COSECANT_PLUS_ITERATOR 34
#define HYPERBOLIC_COSECANT_CLASSIC_ITERATOR 35

#define HYPERBOLIC_SECANT_ITERATOR 36
#define HYPERBOLIC_SECANT_PLUS_ITERATOR 37
#define HYPERBOLIC_SECANT_CLASSIC_ITERATOR 38

#define HYPERBOLIC_COTANGENT_ITERATOR 39
#define HYPERBOLIC_COTANGENT_PLUS_ITERATOR 40
#define HYPERBOLIC_COTANGENT_CLASSIC_ITERATOR 41




#define MUL_COLUMN_DEFAULT		0
#define MUL_COLUMN_CONST_VALS	1
#define MUL_COLUMN_CUSTOM		2

#define MUL_MATRIX_DEFAULT		0
#define MUL_MATRIX_CUSTOM		1

#define MUL_PLUS_X				0
#define MUL_PLUS_Y				1
#define MUL_PLUS_Z				2
#define MUL_PLUS_W				3
#define MUL_MINUS_X				4
#define MUL_MINUS_Y				5
#define MUL_MINUS_Z				6
#define MUL_MINUS_W				7





#define SHADER_COLOUR_RAINBOW 0
#define SHADER_COLOUR_DOTPROD 1
#define SHADER_COLOUR_COW 2
#define SHADER_COLOUR_BRONZE 3
#define SHADER_COLOUR_SILVER 4
#define SHADER_COLOUR_GOLD 5
#define SHADER_COLOUR_GREY 6
#define SHADER_COLOUR_RED 7
#define SHADER_COLOUR_GREEN 8
#define SHADER_COLOUR_BLUE 9





#define D3DFVF_POS_NORMAL_COLOUR_VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE)

struct POS_NORMAL_COLOUR_VERTEX
{
	float x, y, z;
	float nx, ny, nz;
	DWORD colour;

	bool operator<(const POS_NORMAL_COLOUR_VERTEX &right) const
	{
		if(right.x > x)
			return true;
		else if(right.x < x)
			return false;

		if(right.y > y)
			return true;
		else if(right.y < y)
			return false;

		if(right.z > z)
			return true;
		else if(right.z < z)
			return false;

		return false;
	}
};

struct BOUNDING_GRID_SETTINGS
{
	float x_min;
	float x_max;
	float y_min;
	float y_max;
	float z_min;
	float z_max;
};

struct RGB
{
	unsigned char r, g, b;
};

struct xyz
{
	xyz(void)
	{
		x = y = z = 0.0f;
	}

	float x, y, z;
};

struct dimension_info
{
	float x_max, x_min, x_span, y_min, y_max, y_span, z_min, z_max, z_span;
};




#endif


