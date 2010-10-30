// Credits:
// Code, Shawn Halayka
//	- shalayka@sasktel.net
// Configurable multiplication ("4D matrix and column vector concept") provided by Godwin Vickers
//	- ragnarok@hypercomplex.org  --  http://www.hypercomplex.org


#ifndef QUATERNION_MATH_CUSTOM_CONSTANT_COLUMN_H
#define QUATERNION_MATH_CUSTOM_CONSTANT_COLUMN_H


template <class T> class quaternion_math_custom_constant_column : public quaternion_math_base<T>
{
public:

	quaternion_math_custom_constant_column(const bool &init_everything = true)
	{
		if(init_everything)
		{
			for(short unsigned int i = 0; i < 16; i++)
			{
				// similar to C's 'matrix_cp[i] = NULL;'
				matrix_cp[i] = 0;
				qB_final_matrix_cells[i] = 0;
			}

			// default signage for multiplication comes from this table...
			//+x -y -z -w
			//+y +x +w -z
			//+z -w +x +y
			//+w +z -y +x
			default_sign_negative[0]  = false;
			default_sign_negative[1]  = true;
			default_sign_negative[2]  = true;
			default_sign_negative[3]  = true;

			default_sign_negative[4]  = false;
			default_sign_negative[5]  = false;
			default_sign_negative[6]  = false;
			default_sign_negative[7]  = true;

			default_sign_negative[8]  = false;
			default_sign_negative[9]  = true;
			default_sign_negative[10] = false;
			default_sign_negative[11] = false;

			default_sign_negative[12] = false;
			default_sign_negative[13] = false;
			default_sign_negative[14] = true;
			default_sign_negative[15] = false;

			setup_constant_column_values(1, 1, 1, 1);
			setup_custom_matrix(
				"+x", "-y", "-z", "-w",		// custom matrix row 1
				"+y", "+x", "+w", "-z",		// custom matrix row 2
				"+z", "-w", "+x", "+y",		// custom matrix row 3
				"+w", "+z", "-y", "+x");	// custom matrix row 4 

		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	bool setup_constant_column_values(const T &mc1, const T &mc2, const T &mc3, const T &mc4)
	{
		// setup multiplication column constant values
		constant_column[0] = mc1;
		constant_column[1] = mc2;
		constant_column[2] = mc3;
		constant_column[3] = mc4;

		return true;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	bool setup_constant_column_values(const quaternion_t<T> *const qA)
	{
		// setup multiplication column constant values
		constant_column[0] = qA->x;
		constant_column[1] = qA->y;
		constant_column[2] = qA->z;
		constant_column[3] = qA->w;

		return true;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	bool setup_custom_matrix(
		const std::string &_11, const std::string &_12, const std::string &_13, const std::string &_14,
		const std::string &_21, const std::string &_22, const std::string &_23, const std::string &_24,
		const std::string &_31, const std::string &_32, const std::string &_33, const std::string &_34,
		const std::string &_41, const std::string &_42, const std::string &_43, const std::string &_44)
	{
		// setup matrix
		if(!_mul_custom_matrix_cell_setup(_11, 0))
			return false;

		if(!_mul_custom_matrix_cell_setup(_12, 1))
			return false;

		if(!_mul_custom_matrix_cell_setup(_13, 2))
			return false;

		if(!_mul_custom_matrix_cell_setup(_14, 3))
			return false;

		if(!_mul_custom_matrix_cell_setup(_21, 4))
			return false;

		if(!_mul_custom_matrix_cell_setup(_22, 5))
			return false;

		if(!_mul_custom_matrix_cell_setup(_23, 6))
			return false;

		if(!_mul_custom_matrix_cell_setup(_24, 7))
			return false;

		if(!_mul_custom_matrix_cell_setup(_31, 8))
			return false;

		if(!_mul_custom_matrix_cell_setup(_32, 9))
			return false;

		if(!_mul_custom_matrix_cell_setup(_33, 10))
			return false;

		if(!_mul_custom_matrix_cell_setup(_34, 11))
			return false;

		if(!_mul_custom_matrix_cell_setup(_41, 12))
			return false;

		if(!_mul_custom_matrix_cell_setup(_42, 13))
			return false;

		if(!_mul_custom_matrix_cell_setup(_43, 14))
			return false;

		if(!_mul_custom_matrix_cell_setup(_44, 15))
			return false;

		return true;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void mul(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		// rearrange matrix values
		static quaternion_math_custom_constant_column temp_q_math(false);

		((temp_q_math).*(matrix_cp[0]))  (*qB, &qB_final_matrix_cells[0]);
		((temp_q_math).*(matrix_cp[1]))  (*qB, &qB_final_matrix_cells[1]);
		((temp_q_math).*(matrix_cp[2]))  (*qB, &qB_final_matrix_cells[2]);
		((temp_q_math).*(matrix_cp[3]))  (*qB, &qB_final_matrix_cells[3]);
		((temp_q_math).*(matrix_cp[4]))  (*qB, &qB_final_matrix_cells[4]);
		((temp_q_math).*(matrix_cp[5]))  (*qB, &qB_final_matrix_cells[5]);
		((temp_q_math).*(matrix_cp[6]))  (*qB, &qB_final_matrix_cells[6]);
		((temp_q_math).*(matrix_cp[7]))  (*qB, &qB_final_matrix_cells[7]);
		((temp_q_math).*(matrix_cp[8]))  (*qB, &qB_final_matrix_cells[8]);
		((temp_q_math).*(matrix_cp[9]))  (*qB, &qB_final_matrix_cells[9]);
		((temp_q_math).*(matrix_cp[10])) (*qB, &qB_final_matrix_cells[10]);
		((temp_q_math).*(matrix_cp[11])) (*qB, &qB_final_matrix_cells[11]);
		((temp_q_math).*(matrix_cp[12])) (*qB, &qB_final_matrix_cells[12]);
		((temp_q_math).*(matrix_cp[13])) (*qB, &qB_final_matrix_cells[13]);
		((temp_q_math).*(matrix_cp[14])) (*qB, &qB_final_matrix_cells[14]);
		((temp_q_math).*(matrix_cp[15])) (*qB, &qB_final_matrix_cells[15]);

		// perform multiply
		qOut->x = constant_column[0]*qB_final_matrix_cells[0]  - constant_column[1]*qB_final_matrix_cells[1]  - constant_column[2]*qB_final_matrix_cells[2]  - constant_column[3]*qB_final_matrix_cells[3];
		qOut->y = constant_column[0]*qB_final_matrix_cells[4]  + constant_column[1]*qB_final_matrix_cells[5]  + constant_column[2]*qB_final_matrix_cells[6]  - constant_column[3]*qB_final_matrix_cells[7];
		qOut->z = constant_column[0]*qB_final_matrix_cells[8]  - constant_column[1]*qB_final_matrix_cells[9]  + constant_column[2]*qB_final_matrix_cells[10] + constant_column[3]*qB_final_matrix_cells[11];
		qOut->w = constant_column[0]*qB_final_matrix_cells[12] + constant_column[1]*qB_final_matrix_cells[13] - constant_column[2]*qB_final_matrix_cells[14] + constant_column[3]*qB_final_matrix_cells[15];
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void pow(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(0 == pow_exponent)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else if(1 == pow_exponent)
		{
			qOut->x = qA->x;
			qOut->y = qA->y;
			qOut->z = qA->z;
			qOut->w = qA->w;
		}
		else
		{
			*qOut = *qA;

			for(long unsigned int i = 1; i < pow_exponent; i++)
			{
				mul(0, qOut, qOut);
			}
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void pow_dynamic(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		long unsigned int exp = static_cast<long unsigned int>(fabs(qB->x));

		if(0 == exp)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else if(1 == exp)
		{
			qOut->x = qA->x;
			qOut->y = qA->y;
			qOut->z = qA->z;
			qOut->w = qA->w;
		}
		else
		{
			*qOut = *qA;

			for(long unsigned int i = 1; i < exp; i++)
			{
				mul(0, qOut, qOut);
			}
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void div(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		// since the mul column is constant, we don't need to figure out much of anything
		mul(0, qA, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void sinc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->x == 0)// && qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else
		{
			quaternion_t<T> sin_quat;

			sin(qA, 0, &sin_quat);
			div(&sin_quat, 0, qOut);
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void sinhc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->x == 0)// && qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else
		{
			quaternion_t<T> sinh_quat;

			sinh(qA, 0, &sinh_quat);
			div(&sinh_quat, 0, qOut);
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void csc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		div(&one_quat, 0, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void csch(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		div(&one_quat, 0, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void excsc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> csc_quat;

		csc(0, 0, &csc_quat);
		sub(&csc_quat, &one_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void excsch(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> csch_quat;

		csch(0, 0, &csch_quat);
		sub(&csch_quat, &one_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void covercsc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> csc_quat;

		csc(0, 0, &csc_quat);
		sub(&one_quat, &csc_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void covercsch(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> csch_quat;

		csch(0, 0, &csch_quat);
		sub(&one_quat, &csch_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void cscc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->x == 0)// && qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else
		{
			quaternion_t<T> csc_quat;

			csc(0, 0, &csc_quat);
			div(&csc_quat, 0, qOut);
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void cschc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->x == 0)// && qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else
		{
			quaternion_t<T> csch_quat;

			csch(0, 0, &csch_quat);
			div(&csch_quat, 0, qOut);
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void cosc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->x == 0)// && qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else
		{
			quaternion_t<T> cos_quat;

			cos(qA, 0, &cos_quat);
			div(&cos_quat, 0, qOut);
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void coshc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->x == 0)// && qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else
		{
			quaternion_t<T> cosh_quat;

			cosh(qA, 0, &cosh_quat);
			div(&cosh_quat, 0, qOut);
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void sec(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		div(&one_quat, 0, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void sech(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		div(&one_quat, 0, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void exsec(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> sec_quat;

		sec(0, 0, &sec_quat);
		sub(&sec_quat, &one_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void exsech(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> sech_quat;

		sech(0, 0, &sech_quat);
		sub(&sech_quat, &one_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void vercsc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> sec_quat;

		sec(0, 0, &sec_quat);
		sub(&one_quat, &sec_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void vercsch(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> sech_quat;

		sech(0, 0, &sech_quat);
		sub(&one_quat, &sech_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void secc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->x == 0)// && qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else
		{
			quaternion_t<T> sec_quat;

			sec(0, 0, &sec_quat);
			div(&sec_quat, 0, qOut);
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void sechc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->x == 0)// && qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else
		{
			quaternion_t<T> sech_quat;

			sech(0, 0, &sech_quat);
			div(&sech_quat, 0, qOut);
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void tan(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		quaternion_t<T> sin_quat;
		sin(qA, 0, &sin_quat);
		div(&sin_quat, 0, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void tanh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		quaternion_t<T> sinh_quat;
		sinh(qA, 0, &sinh_quat);
		div(&sinh_quat, 0, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void extan(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> tan_quat;

		tan(qA, 0, &tan_quat);
		sub(&tan_quat, &one_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void extanh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> tanh_quat;

		tanh(qA, 0, &tanh_quat);
		sub(&tanh_quat, &one_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void covertan(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> tan_quat;

		tan(qA, 0, &tan_quat);
		sub(&one_quat, &tan_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void covertanh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> tanh_quat;

		tanh(qA, 0, &tanh_quat);
		sub(&one_quat, &tanh_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void tanc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->x == 0)// && qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else
		{
			quaternion_t<T> tan_quat;

			tan(qA, 0, &tan_quat);
			div(&tan_quat, 0, qOut);
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void tanhc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->x == 0)// && qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else
		{
			quaternion_t<T> tanh_quat;

			tanh(qA, 0, &tanh_quat);
			div(&tanh_quat, 0, qOut);
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void cot(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		div(&one_quat, 0, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void coth(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		div(&one_quat, 0, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void excot(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> cot_quat;

		cot(0, 0, &cot_quat);
		sub(&cot_quat, &one_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void excoth(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> coth_quat;

		coth(0, 0, &coth_quat);
		sub(&coth_quat, &one_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void covercot(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> cot_quat;

		cot(0, 0, &cot_quat);
		sub(&one_quat, &cot_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void covercoth(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> coth_quat;

		coth(0, 0, &coth_quat);
		sub(&one_quat, &coth_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void cotc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->x == 0)// && qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else
		{
			quaternion_t<T> cot_quat;

			cot(0, 0, &cot_quat);
			div(&cot_quat, qA, qOut);
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void cothc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->x == 0)// && qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			qOut->x = 1;
			qOut->y = 0;
			qOut->z = 0;
			qOut->w = 0;
		}
		else
		{
			quaternion_t<T> coth_quat;

			coth(0, 0, &coth_quat);
			div(&coth_quat, qA, qOut);
		}
	}

protected:

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	bool _mul_custom_matrix_cell_setup(const std::string &cell, const short unsigned int &index)
	{
		std::string temp_string = cell;

		for(std::string::iterator string_iter = temp_string.begin(); string_iter != temp_string.end(); string_iter++)
			*string_iter = tolower(*string_iter);

		if(temp_string == "+x" || temp_string == "x")
		{
			if(default_sign_negative[index] == false)
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_x;
			else
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_neg_x;

			return true;
		}
		else if(temp_string == "-x")
		{
			if(default_sign_negative[index] == true)
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_x;
			else
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_neg_x;

			return true;
		}

		else if(temp_string == "+y" || temp_string == "y")
		{
			if(default_sign_negative[index] == false)
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_y;
			else
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_neg_y;

			return true;
		}
		else if(temp_string == "-y")
		{
			if(default_sign_negative[index] == true)
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_y;
			else
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_neg_y;

			return true;
		}

		else if(temp_string == "+z" || temp_string == "z")
		{
			if(default_sign_negative[index] == false)
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_z;
			else
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_neg_z;

			return true;
		}
		else if(temp_string == "-z")
		{
			if(default_sign_negative[index] == true)
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_z;
			else
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_neg_z;

			return true;
		}

		else if(temp_string == "+w" || temp_string == "w")
		{
			if(default_sign_negative[index] == false)
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_w;
			else
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_neg_w;

			return true;
		}
		else if(temp_string == "-w")
		{
			if(default_sign_negative[index] == true)
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_w;
			else
				matrix_cp[index] = &quaternion_math_custom_constant_column::_copy_neg_w;

			return true;
		}

		return false;
	}

	// constant column values
	T constant_column[4];

	void _copy_x(const quaternion_t<T> &qIn, T *dest) { *dest = qIn.x; }
	void _copy_neg_x(const quaternion_t<T> &qIn, T *dest) { *dest = -qIn.x; }
	void _copy_y(const quaternion_t<T> &qIn, T *dest) { *dest = qIn.y; }
	void _copy_neg_y(const quaternion_t<T> &qIn, T *dest) { *dest = -qIn.y; }
	void _copy_z(const quaternion_t<T> &qIn, T *dest) { *dest = qIn.z; }
	void _copy_neg_z(const quaternion_t<T> &qIn, T *dest) { *dest = -qIn.z; }
	void _copy_w(const quaternion_t<T> &qIn, T *dest) { *dest = qIn.w; }
	void _copy_neg_w(const quaternion_t<T> &qIn, T *dest) { *dest = -qIn.w; }

	// function pointers for copying data during custom mul
	typedef void (quaternion_math_custom_constant_column::*copy_ptr)(const quaternion_t<T> &, T *);

	// custom matrix
	copy_ptr matrix_cp[16];
	T qB_final_matrix_cells[16];
	bool default_sign_negative[16];
};


#endif