// todo: when custom formula parser is working, try out some reciprocals, ie: Z = (1/Z)^a + C


__forceinline double quaternion_julia_set::custom_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	return eqparser.Iterate(Z, max_iterations, infinite_length);
}


// Z = Z^2 + C;
__forceinline double quaternion_julia_set::classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	// move this into _asm block?
	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}




//Z = exp(Z^a) + C;
__forceinline double quaternion_julia_set::exponential_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	// move this into _asm block?
	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->exp(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = Z^a + Z + C
__forceinline double quaternion_julia_set::quadratic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	// move this into _asm block?
	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> z_quat_copy = z_quat;

		q_math->pow(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &z_quat_copy, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = C*Z*(a-Z)
__forceinline double quaternion_julia_set::lambda_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> sub_quat(a, 0.0, 0.0, 0.0);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//(C*Z)
		quaternion_t<double> cz_quat(C.x, C.y, C.z, C.w);
		q_math->mul(&cz_quat, &z_quat, &cz_quat);

		//(a-Z)
		q_math->sub(&sub_quat, &z_quat, &z_quat);

		//(C*Z) * (a-Z)
		q_math->mul(&cz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = C*Z*(C-Z)
__forceinline double quaternion_julia_set::funky_blobs_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//(C*Z)
		quaternion_t<double> cz_quat(C.x, C.y, C.z, C.w);
		q_math->mul(&cz_quat, &z_quat, &cz_quat);

		//(C-Z)
		q_math->sub(&c_quat, &z_quat, &z_quat);

		//(C*Z) * (C-Z)
		q_math->mul(&cz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}



// Z = C * sin(Z)^a
__forceinline double quaternion_julia_set::sine_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//sin(Z)^a
		q_math->sin(&z_quat, 0, &z_quat);
		q_math->pow(&z_quat, 0, &z_quat);

		//C * sin(Z)^a
		q_math->mul(&c_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = sin(Z)^a + C * sin(Z)^a
__forceinline double quaternion_julia_set::sine_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> sinz_quat(z_quat.x, z_quat.y, z_quat.z, z_quat.w);

		q_math->sin(&z_quat, 0, &sinz_quat);
		q_math->pow(&sinz_quat, 0, &sinz_quat);

		q_math->mul(&c_quat, &sinz_quat, &z_quat);
		q_math->add(&sinz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = sin(Z^a) + C;
__forceinline double quaternion_julia_set::sine_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->sin(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = C * cos(Z)^a
__forceinline double quaternion_julia_set::cosine_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//cos(Z)^a
		q_math->cos(&z_quat, 0, &z_quat);
		q_math->pow(&z_quat, 0, &z_quat);

		//C * cos(Z)^a
		q_math->mul(&c_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = cos(Z)^a + C * cos(Z)^a
__forceinline double quaternion_julia_set::cosine_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> cosz_quat(z_quat.x, z_quat.y, z_quat.z, z_quat.w);

		q_math->cos(&z_quat, 0, &cosz_quat);
		q_math->pow(&cosz_quat, 0, &cosz_quat);

		q_math->mul(&c_quat, &cosz_quat, &z_quat);
		q_math->add(&cosz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = cos(Z^a) + C;
__forceinline double quaternion_julia_set::cosine_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->cos(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = C * tan(Z)^a
__forceinline double quaternion_julia_set::tangent_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//tan(Z)^a
		q_math->tan(&z_quat, 0, &z_quat);
		q_math->pow(&z_quat, 0, &z_quat);

		//C * tan(Z)^a
		q_math->mul(&c_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = tan(Z)^a + C * tan(Z)^a
__forceinline double quaternion_julia_set::tangent_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> tanz_quat(z_quat.x, z_quat.y, z_quat.z, z_quat.w);

		q_math->tan(&z_quat, 0, &tanz_quat);
		q_math->pow(&tanz_quat, 0, &tanz_quat);

		q_math->mul(&c_quat, &tanz_quat, &z_quat);
		q_math->add(&tanz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = tan(Z^a) + C;
__forceinline double quaternion_julia_set::tangent_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->tan(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = C * sinh(Z)^a
__forceinline double quaternion_julia_set::hyperbolic_sine_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//sinh(Z)^a
		q_math->sinh(&z_quat, 0, &z_quat);
		q_math->pow(&z_quat, 0, &z_quat);

		//C * sinh(Z)^a
		q_math->mul(&c_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = sinh(Z)^a + C * sinh(Z)^a
__forceinline double quaternion_julia_set::hyperbolic_sine_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> sinhz_quat(z_quat.x, z_quat.y, z_quat.z, z_quat.w);

		q_math->sinh(&z_quat, 0, &sinhz_quat);
		q_math->pow(&sinhz_quat, 0, &sinhz_quat);

		q_math->mul(&c_quat, &sinhz_quat, &z_quat);
		q_math->add(&sinhz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = sinh(Z^a) + C;
__forceinline double quaternion_julia_set::hyperbolic_sine_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->sinh(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = C * cosh(Z)^a
__forceinline double quaternion_julia_set::hyperbolic_cosine_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//cosh(Z)^a
		q_math->cosh(&z_quat, 0, &z_quat);
		q_math->pow(&z_quat, 0, &z_quat);

		//C * cosh(Z)^a
		q_math->mul(&c_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = cosh(Z)^a + C * cosh(Z)^a
__forceinline double quaternion_julia_set::hyperbolic_cosine_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> coshz_quat(z_quat.x, z_quat.y, z_quat.z, z_quat.w);

		q_math->cosh(&z_quat, 0, &coshz_quat);
		q_math->pow(&coshz_quat, 0, &coshz_quat);

		q_math->mul(&c_quat, &coshz_quat, &z_quat);
		q_math->add(&coshz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = cosh(Z^a) + C;
__forceinline double quaternion_julia_set::hyperbolic_cosine_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->cosh(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = C * tanh(Z)^a
__forceinline double quaternion_julia_set::hyperbolic_tangent_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//tanh(Z)^a
		q_math->tanh(&z_quat, 0, &z_quat);
		q_math->pow(&z_quat, 0, &z_quat);

		//C * tanh(Z)^a
		q_math->mul(&c_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = tanh(Z)^a + C * tanh(Z)^a
__forceinline double quaternion_julia_set::hyperbolic_tangent_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> tanhz_quat(z_quat.x, z_quat.y, z_quat.z, z_quat.w);

		q_math->tanh(&z_quat, 0, &tanhz_quat);
		q_math->pow(&tanhz_quat, 0, &tanhz_quat);

		q_math->mul(&c_quat, &tanhz_quat, &z_quat);
		q_math->add(&tanhz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = tanh(Z^a) + C;
__forceinline double quaternion_julia_set::hyperbolic_tangent_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->tanh(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}



// Z = C * csc(Z)^a
__forceinline double quaternion_julia_set::cosecant_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//csc(Z)^a
		q_math->csc(&z_quat, 0, &z_quat);
		q_math->pow(&z_quat, 0, &z_quat);

		//C * csc(Z)^a
		q_math->mul(&c_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = csc(Z)^a + C * csc(Z)^a
__forceinline double quaternion_julia_set::cosecant_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> cscz_quat(z_quat.x, z_quat.y, z_quat.z, z_quat.w);

		q_math->csc(&z_quat, 0, &cscz_quat);
		q_math->pow(&cscz_quat, 0, &cscz_quat);

		q_math->mul(&c_quat, &cscz_quat, &z_quat);
		q_math->add(&cscz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = csc(Z^a) + C;
__forceinline double quaternion_julia_set::cosecant_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->csc(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = C * sec(Z)^a
__forceinline double quaternion_julia_set::secant_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//sec(Z)^a
		q_math->sec(&z_quat, 0, &z_quat);
		q_math->pow(&z_quat, 0, &z_quat);

		//C * sec(Z)^a
		q_math->mul(&c_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = sec(Z)^a + C * sec(Z)^a
__forceinline double quaternion_julia_set::secant_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> secz_quat(z_quat.x, z_quat.y, z_quat.z, z_quat.w);

		q_math->sec(&z_quat, 0, &secz_quat);
		q_math->pow(&secz_quat, 0, &secz_quat);

		q_math->mul(&c_quat, &secz_quat, &z_quat);
		q_math->add(&secz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = sec(Z^a) + C;
__forceinline double quaternion_julia_set::secant_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->sec(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = C * cot(Z)^a
__forceinline double quaternion_julia_set::cotangent_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//cot(Z)^a
		q_math->cot(&z_quat, 0, &z_quat);
		q_math->pow(&z_quat, 0, &z_quat);

		//C * cot(Z)^a
		q_math->mul(&c_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = cot(Z)^a + C * cot(Z)^a
__forceinline double quaternion_julia_set::cotangent_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> cotz_quat(z_quat.x, z_quat.y, z_quat.z, z_quat.w);

		q_math->cot(&z_quat, 0, &cotz_quat);
		q_math->pow(&cotz_quat, 0, &cotz_quat);

		q_math->mul(&c_quat, &cotz_quat, &z_quat);
		q_math->add(&cotz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = cot(Z^a) + C;
__forceinline double quaternion_julia_set::cotangent_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->cot(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}



// Z = C * csch(Z)^a
__forceinline double quaternion_julia_set::hyperbolic_cosecant_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//csch(Z)^a
		q_math->csch(&z_quat, 0, &z_quat);
		q_math->pow(&z_quat, 0, &z_quat);

		//C * csch(Z)^a
		q_math->mul(&c_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = csch(Z)^a + C * csch(Z)^a
__forceinline double quaternion_julia_set::hyperbolic_cosecant_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> cschz_quat(z_quat.x, z_quat.y, z_quat.z, z_quat.w);

		q_math->csch(&z_quat, 0, &cschz_quat);
		q_math->pow(&cschz_quat, 0, &cschz_quat);

		q_math->mul(&c_quat, &cschz_quat, &z_quat);
		q_math->add(&cschz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = csch(Z^a) + C;
__forceinline double quaternion_julia_set::hyperbolic_cosecant_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->csch(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = C * sech(Z)^a
__forceinline double quaternion_julia_set::hyperbolic_secant_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//sech(Z)^a
		q_math->sech(&z_quat, 0, &z_quat);
		q_math->pow(&z_quat, 0, &z_quat);

		//C * sech(Z)^a
		q_math->mul(&c_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = sech(Z)^a + C * sech(Z)^a
__forceinline double quaternion_julia_set::hyperbolic_secant_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> sechz_quat(z_quat.x, z_quat.y, z_quat.z, z_quat.w);

		q_math->sech(&z_quat, 0, &sechz_quat);
		q_math->pow(&sechz_quat, 0, &sechz_quat);

		q_math->mul(&c_quat, &sechz_quat, &z_quat);
		q_math->add(&sechz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = sech(Z^a) + C;
__forceinline double quaternion_julia_set::hyperbolic_secant_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->sech(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = C * coth(Z)^a
__forceinline double quaternion_julia_set::hyperbolic_cotangent_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		//coth(Z)^a
		q_math->coth(&z_quat, 0, &z_quat);
		q_math->pow(&z_quat, 0, &z_quat);

		//C * coth(Z)^a
		q_math->mul(&c_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

// Z = coth(Z)^a + C * coth(Z)^a
__forceinline double quaternion_julia_set::hyperbolic_cotangent_plus_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		quaternion_t<double> cothz_quat(z_quat.x, z_quat.y, z_quat.z, z_quat.w);

		q_math->coth(&z_quat, 0, &cothz_quat);
		q_math->pow(&cothz_quat, 0, &cothz_quat);

		q_math->mul(&c_quat, &cothz_quat, &z_quat);
		q_math->add(&cothz_quat, &z_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

//Z = coth(Z^a) + C;
__forceinline double quaternion_julia_set::hyperbolic_cotangent_classic_iter(const quaternion_t<double> &Z, const quaternion_t<double> &C, const long unsigned int &max_iterations, const double &infinite_length, const double &a) throw()
{
	q_math->setup_pow_exponent(a);

	quaternion_t<double> z_quat(Z.x, Z.y, Z.z, Z.w);
	quaternion_t<double> c_quat(C.x, C.y, C.z, C.w);

	double length = 0.0;

	for(long unsigned int i = 0; i < max_iterations; i++)
	{
		q_math->pow(&z_quat, 0, &z_quat);
		q_math->coth(&z_quat, 0, &z_quat);
		q_math->add(&z_quat, &c_quat, &z_quat);

		if((length = (z_quat.x*z_quat.x + z_quat.y*z_quat.y + z_quat.z*z_quat.z + z_quat.w*z_quat.w)) >= infinite_length*infinite_length)
			break;
	}

	return sqrt(length);
}

