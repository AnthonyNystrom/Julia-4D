#ifndef PERLIN_H
#define PERLIN_H


	#define perlin_B 0x100
	#define BM 0xff
	#define N 0x1000
	#define NP 12   /* 2^N */
	#define NM 0xfff

	#define s_curve(t) ( t * t * (3. - 2. * t) )
	#define lerp(t, a, b) ( a + t * (b - a) )
	#define setup(i,b0,b1,r0,r1)\
			t = vec[i] + N;\
			b0 = ((int)t) & BM;\
			b1 = (b0+1) & BM;\
			r0 = t - (int)t;\
			r1 = r0 - 1.;
	#define at2(rx,ry) ( rx * q[0] + ry * q[1] )
	#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )


struct perlin
{
	perlin(void);
	double noise1(double);
	double noise2(double *);
	double noise3(double *);
	void normalize3(double *);
	void normalize2(double *);

	double PerlinNoise1D(double,double,double,int);
	double PerlinNoise2D(double,double,double,double,int);
	double PerlinNoise3D(double,double,double,double,double,int);

	int p[perlin_B + perlin_B + 2];
	double g3[perlin_B + perlin_B + 2][3];
	double g2[perlin_B + perlin_B + 2][2];
	double g1[perlin_B + perlin_B + 2];
};


#endif