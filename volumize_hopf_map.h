#ifndef VOLUMIZE_HOPF_MAP_HEADER
#define VOLUMIZE_HOPF_MAP_HEADER


#include <set>
#include <list>
#include <vector>
using std::set;
using std::list;
using std::vector;

#include "defines.h"
#include "marching_cubes_types.h"
#include "julia4d2Doc.h"


extern int MC_EdgeTable[256];
extern int MC_TriTable[256][16];








class volumize_hopf_map
{
public:
	int PolygoniseHopfMap(const Cjulia4d2Doc &internal_params, const vector<bool> src_vect, list<mc_tri> &triangles, const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event);
	int VoxelizeHopfMap(const Cjulia4d2Doc &internal_params, const vector<bool> src_vect, vector<bool> &voxels, dimension_info &d_info, const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event);

protected:

	int GetDimensions(const Cjulia4d2Doc &internal_params, const vector<bool> src_vect, dimension_info &d_info, const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event);
	int FillBoolField(const Cjulia4d2Doc &internal_params, const vector<bool> src_vect, vector<bool> &bool_field, const dimension_info &d_info, const bool &add_single_border, const HWND &progressbar, const HWND &status_text, const HANDLE &cancel_event);


	__forceinline mc_xyz GenericVertexInterp(const double &isolevel, mc_xyz p1, mc_xyz p2, double valp1, double valp2)
	{
		double mu;
		mc_xyz p;

		if (fabs(isolevel - valp1) < 0.00001)
			return(p1);

		if (fabs(isolevel - valp2) < 0.00001)
			return(p2);

		if (fabs(valp1 - valp2) < 0.00001) // was 0.00001
			return(p1);

		mu = (isolevel - valp1) / (valp2 - valp1);
		p.x = p1.x + mu * (p2.x - p1.x);
		p.y = p1.y + mu * (p2.y - p1.y);
		p.z = p1.z + mu * (p2.z - p1.z);

		return(p);
	}

	__forceinline int GenericPolygonise(const double &isovalue, mc_gridcell grid, mc_tri *triangles)
	{
		int i,ntriang;
		int cubeindex;
		mc_xyz vertlist[12];

		// Determine the index into the edge table which
		// tells us which vertices are inside of the surface

		cubeindex = 0;
		if (grid.val[0] < isovalue) cubeindex |= 1;
		if (grid.val[1] < isovalue) cubeindex |= 2;
		if (grid.val[2] < isovalue) cubeindex |= 4;
		if (grid.val[3] < isovalue) cubeindex |= 8;
		if (grid.val[4] < isovalue) cubeindex |= 16;
		if (grid.val[5] < isovalue) cubeindex |= 32;
		if (grid.val[6] < isovalue) cubeindex |= 64;
		if (grid.val[7] < isovalue) cubeindex |= 128;

		/* Cube is entirely in/out of the surface */
		if (MC_EdgeTable[cubeindex] == 0)
			return(0);


		if (MC_EdgeTable[cubeindex] & 1)
			vertlist[0] = GenericVertexInterp(isovalue, grid.p[0], grid.p[1], grid.val[0], grid.val[1]);

		if (MC_EdgeTable[cubeindex] & 2)
			vertlist[1] = GenericVertexInterp(isovalue, grid.p[1], grid.p[2], grid.val[1], grid.val[2]);

		if (MC_EdgeTable[cubeindex] & 4)
			vertlist[2] = GenericVertexInterp(isovalue, grid.p[2], grid.p[3], grid.val[2], grid.val[3]);

		if (MC_EdgeTable[cubeindex] & 8)
			vertlist[3] = GenericVertexInterp(isovalue, grid.p[3], grid.p[0], grid.val[3], grid.val[0]);

		if (MC_EdgeTable[cubeindex] & 16)
			vertlist[4] = GenericVertexInterp(isovalue, grid.p[4], grid.p[5], grid.val[4], grid.val[5]);

		if (MC_EdgeTable[cubeindex] & 32)
			vertlist[5] = GenericVertexInterp(isovalue, grid.p[5], grid.p[6], grid.val[5], grid.val[6]);

		if (MC_EdgeTable[cubeindex] & 64)
			vertlist[6] = GenericVertexInterp(isovalue, grid.p[6], grid.p[7], grid.val[6], grid.val[7]);

		if (MC_EdgeTable[cubeindex] & 128)
			vertlist[7] = GenericVertexInterp(isovalue, grid.p[7], grid.p[4], grid.val[7], grid.val[4]);

		if (MC_EdgeTable[cubeindex] & 256)
			vertlist[8] = GenericVertexInterp(isovalue, grid.p[0], grid.p[4], grid.val[0], grid.val[4]);

		if (MC_EdgeTable[cubeindex] & 512)
			vertlist[9] = GenericVertexInterp(isovalue, grid.p[1], grid.p[5], grid.val[1], grid.val[5]);

		if (MC_EdgeTable[cubeindex] & 1024)
			vertlist[10] = GenericVertexInterp(isovalue, grid.p[2], grid.p[6], grid.val[2], grid.val[6]);

		if (MC_EdgeTable[cubeindex] & 2048)
			vertlist[11] = GenericVertexInterp(isovalue, grid.p[3], grid.p[7], grid.val[3], grid.val[7]);


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