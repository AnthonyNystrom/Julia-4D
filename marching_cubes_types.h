#ifndef MARCHING_CUBES_TYPES_HEADER
#define MARCHING_CUBES_TYPES_HEADER








class mc_xyz
{
public:
   double x, y, z;

	bool operator<(const mc_xyz &right) const
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

	bool operator==(const mc_xyz &right) const
	{
		if(right.x == x && right.y == y && right.z == z)
			return true;
		else
			return false;
	}

	mc_xyz& operator=(const mc_xyz &right)
	{
		x = right.x;
		y = right.y;
		z = right.z;

		return *this;
	}

	long unsigned int index;
};

struct mc_tri
{
   mc_xyz p[3];
   mc_xyz normal;
};

struct mc_gridcell
{
	mc_xyz p[8];
	double val[8];
};







#endif