// Credits:
// Code, Shawn Halayka
//	- shalayka@sasktel.net
// Configurable multiplication ("4D matrix and column vector concept") provided by Godwin Vickers
//	- ragnarok@hypercomplex.org  --  http://www.hypercomplex.org


#ifndef QUATERNION_MATH_CUSTOM_H
#define QUATERNION_MATH_CUSTOM_H


template <class T> class quaternion_math_custom : public quaternion_math_base<T>
{
public:

	quaternion_math_custom(const bool &init_everything = true)
	{
		if(init_everything)
		{
			mul_column_cp[0] = 0;
			mul_column_cp[1] = 0;
			mul_column_cp[2] = 0;
			mul_column_cp[3] = 0;

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

			// initial setup of custom multiplication function
			// using default multiplication column and matrix
			setup_custom_column("+x", "+y", "+z", "+w");	// custom multiplication column
			setup_custom_matrix(
				"+x", "-y", "-z", "-w",		// custom matrix row 1
				"+y", "+x", "+w", "-z",		// custom matrix row 2
				"+z", "-w", "+x", "+y",		// custom matrix row 3
				"+w", "+z", "-y", "+x");	// custom matrix row 4 
		}
	}

	bool setup_custom_column(const std::string &mc1, const std::string &mc2, const std::string &mc3, const std::string &mc4)
	{
		// setup multiplication column
		if(!_mul_custom_column_cell_setup(mc1, 0))
			return false;

		if(!_mul_custom_column_cell_setup(mc2, 1))
			return false;

		if(!_mul_custom_column_cell_setup(mc3, 2))
			return false;

		if(!_mul_custom_column_cell_setup(mc4, 3))
			return false;

		return true;
	}

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
		static quaternion_math_custom temp_q_math(false);

		((temp_q_math).*(mul_column_cp[0])) (*qA, &temp_a_x);
		((temp_q_math).*(mul_column_cp[1])) (*qA, &temp_a_y);
		((temp_q_math).*(mul_column_cp[2])) (*qA, &temp_a_z);
		((temp_q_math).*(mul_column_cp[3])) (*qA, &temp_a_w);

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
		qOut->x = temp_a_x*qB_final_matrix_cells[0]  - temp_a_y*qB_final_matrix_cells[1]  - temp_a_z*qB_final_matrix_cells[2]  - temp_a_w*qB_final_matrix_cells[3];
		qOut->y = temp_a_x*qB_final_matrix_cells[4]  + temp_a_y*qB_final_matrix_cells[5]  + temp_a_z*qB_final_matrix_cells[6]  - temp_a_w*qB_final_matrix_cells[7];
		qOut->z = temp_a_x*qB_final_matrix_cells[8]  - temp_a_y*qB_final_matrix_cells[9]  + temp_a_z*qB_final_matrix_cells[10] + temp_a_w*qB_final_matrix_cells[11];
		qOut->w = temp_a_x*qB_final_matrix_cells[12] + temp_a_y*qB_final_matrix_cells[13] - temp_a_z*qB_final_matrix_cells[14] + temp_a_w*qB_final_matrix_cells[15];
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
			temp_quat = *qOut = *qA;

			for(long unsigned int i = 1; i < pow_exponent; i++)
			{
				mul(qOut, &temp_quat, qOut);
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
			temp_quat = *qOut = *qA;

			for(long unsigned int i = 1; i < exp; i++)
			{
				mul(qOut, &temp_quat, qOut);
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
		// c = a/b
		// c = inv(b) * a
		//     inv(b) = conjugate(b) / norm(b)
		// c = (conjugate(b) / norm(b)) * a

		quaternion_t<T> final_b_quat;

		T temp_b_norm = qB->x*qB->x + qB->y*qB->y + qB->z*qB->z + qB->w*qB->w;

		temp_b_x =  qB->x;
		temp_b_y = -qB->y;
		temp_b_z = -qB->z;
		temp_b_w = -qB->w;

		//temp_b_x /= temp_b_norm;
		//temp_b_y /= temp_b_norm;
		//temp_b_z /= temp_b_norm;
		//temp_b_w /= temp_b_norm;
		final_b_quat.x = temp_b_x / temp_b_norm;
		final_b_quat.y = temp_b_y / temp_b_norm;
		final_b_quat.z = temp_b_z / temp_b_norm;
		final_b_quat.w = temp_b_w / temp_b_norm;

		//temp_a_x = qA->x;
		//temp_a_y = qA->y;
		//temp_a_z = qA->z;
		//temp_a_w = qA->w;
		//qOut->x = temp_b_x*temp_a_x - temp_b_y*temp_a_y - temp_b_z*temp_a_z - temp_b_w*temp_a_w;
		//qOut->y = temp_b_x*temp_a_y + temp_b_y*temp_a_x + temp_b_z*temp_a_w - temp_b_w*temp_a_z;
		//qOut->z = temp_b_x*temp_a_z - temp_b_y*temp_a_w + temp_b_z*temp_a_x + temp_b_w*temp_a_y;
		//qOut->w = temp_b_x*temp_a_w + temp_b_y*temp_a_z - temp_b_z*temp_a_y + temp_b_w*temp_a_x;
		mul(&final_b_quat, qA, qOut);
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
			div(&sin_quat, qA, qOut);
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
			div(&sinh_quat, qA, qOut);
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
		quaternion_t<T> sin_quat;

		sin(qA, 0, &sin_quat);
		div(&one_quat, &sin_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void csch(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> sinh_quat;

		sinh(qA, 0, &sinh_quat);
		div(&one_quat, &sinh_quat, qOut);
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

		csc(qA, 0, &csc_quat);
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

		csch(qA, 0, &csch_quat);
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

		csc(qA, 0, &csc_quat);
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

		csch(qA, 0, &csch_quat);
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

			csc(qA, 0, &csc_quat);
			div(&csc_quat, qA, qOut);
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

			csch(qA, 0, &csch_quat);
			div(&csch_quat, qA, qOut);
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
			div(&cos_quat, qA, qOut);
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
			div(&cosh_quat, qA, qOut);
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
		quaternion_t<T> cos_quat;

		cos(qA, 0, &cos_quat);
		div(&one_quat, &cos_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void sech(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> cosh_quat;

		cosh(qA, 0, &cosh_quat);
		div(&one_quat, &cosh_quat, qOut);
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

		sec(qA, 0, &sec_quat);
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

		sech(qA, 0, &sech_quat);
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

		sec(qA, 0, &sec_quat);
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

		sech(qA, 0, &sech_quat);
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

			sec(qA, 0, &sec_quat);
			div(&sec_quat, qA, qOut);
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

			sech(qA, 0, &sech_quat);
			div(&sech_quat, qA, qOut);
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
		quaternion_t<T> cos_quat;

		sin(qA, 0, &sin_quat);
		cos(qA, 0, &cos_quat);

		div(&sin_quat, &cos_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void tanh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		quaternion_t<T> sinh_quat;
		quaternion_t<T> cosh_quat;

		sinh(qA, 0, &sinh_quat);
		cosh(qA, 0, &cosh_quat);

		div(&sinh_quat, &cosh_quat, qOut);
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
			div(&tan_quat, qA, qOut);
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
			div(&tanh_quat, qA, qOut);
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
		quaternion_t<T> tan_quat;

		tan(qA, 0, &tan_quat);
		div(&one_quat, &tan_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void coth(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> tanh_quat;

		tanh(qA, 0, &tanh_quat);
		div(&one_quat, &tanh_quat, qOut);
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

		cot(qA, 0, &cot_quat);
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

		coth(qA, 0, &coth_quat);
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

		cot(qA, 0, &cot_quat);
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

		coth(qA, 0, &coth_quat);
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

			cot(qA, 0, &cot_quat);
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

			coth(qA, 0, &coth_quat);
			div(&coth_quat, qA, qOut);
		}
	}

protected:

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	bool _mul_custom_column_cell_setup(const std::string &cell, const short unsigned int &index)
	{
		std::string temp_string = cell;

		for(std::string::iterator string_iter = temp_string.begin(); string_iter != temp_string.end(); string_iter++)
			*string_iter = tolower(*string_iter);

		if(temp_string == "+x" || temp_string == "x")
		{
			mul_column_cp[index] = &quaternion_math_custom::_copy_x;
			return true;
		}
		else if(temp_string == "-x")
		{
			mul_column_cp[index] = &quaternion_math_custom::_copy_neg_x;
			return true;
		}

		else if(temp_string == "+y" || temp_string == "y")
		{
			mul_column_cp[index] = &quaternion_math_custom::_copy_y;
			return true;
		}
		else if(temp_string == "-y")
		{
			mul_column_cp[index] = &quaternion_math_custom::_copy_neg_y;
			return true;
		}

		else if(temp_string == "+z" || temp_string == "z")
		{
			mul_column_cp[index] = &quaternion_math_custom::_copy_z;
			return true;
		}
		else if(temp_string == "-z")
		{
			mul_column_cp[index] = &quaternion_math_custom::_copy_neg_z;
			return true;
		}

		else if(temp_string == "+w" || temp_string == "w")
		{
			mul_column_cp[index] = &quaternion_math_custom::_copy_w;
			return true;
		}
		else if(temp_string == "-w")
		{
			mul_column_cp[index] = &quaternion_math_custom::_copy_neg_w;
			return true;
		}

		return false;
	}

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
				matrix_cp[index] = &quaternion_math_custom::_copy_x;
			else
				matrix_cp[index] = &quaternion_math_custom::_copy_neg_x;

			return true;
		}
		else if(temp_string == "-x")
		{
			if(default_sign_negative[index] == true)
				matrix_cp[index] = &quaternion_math_custom::_copy_x;
			else
				matrix_cp[index] = &quaternion_math_custom::_copy_neg_x;

			return true;
		}

		else if(temp_string == "+y" || temp_string == "y")
		{
			if(default_sign_negative[index] == false)
				matrix_cp[index] = &quaternion_math_custom::_copy_y;
			else
				matrix_cp[index] = &quaternion_math_custom::_copy_neg_y;

			return true;
		}
		else if(temp_string == "-y")
		{
			if(default_sign_negative[index] == true)
				matrix_cp[index] = &quaternion_math_custom::_copy_y;
			else
				matrix_cp[index] = &quaternion_math_custom::_copy_neg_y;

			return true;
		}

		else if(temp_string == "+z" || temp_string == "z")
		{
			if(default_sign_negative[index] == false)
				matrix_cp[index] = &quaternion_math_custom::_copy_z;
			else
				matrix_cp[index] = &quaternion_math_custom::_copy_neg_z;

			return true;
		}
		else if(temp_string == "-z")
		{
			if(default_sign_negative[index] == true)
				matrix_cp[index] = &quaternion_math_custom::_copy_z;
			else
				matrix_cp[index] = &quaternion_math_custom::_copy_neg_z;

			return true;
		}

		else if(temp_string == "+w" || temp_string == "w")
		{
			if(default_sign_negative[index] == false)
				matrix_cp[index] = &quaternion_math_custom::_copy_w;
			else
				matrix_cp[index] = &quaternion_math_custom::_copy_neg_w;

			return true;
		}
		else if(temp_string == "-w")
		{
			if(default_sign_negative[index] == true)
				matrix_cp[index] = &quaternion_math_custom::_copy_w;
			else
				matrix_cp[index] = &quaternion_math_custom::_copy_neg_w;

			return true;
		}

		return false;
	}

	void _copy_x(const quaternion_t<T> &qIn, T *dest) { *dest = qIn.x; }
	void _copy_neg_x(const quaternion_t<T> &qIn, T *dest) { *dest = -qIn.x; }
	void _copy_y(const quaternion_t<T> &qIn, T *dest) { *dest = qIn.y; }
	void _copy_neg_y(const quaternion_t<T> &qIn, T *dest) { *dest = -qIn.y; }
	void _copy_z(const quaternion_t<T> &qIn, T *dest) { *dest = qIn.z; }
	void _copy_neg_z(const quaternion_t<T> &qIn, T *dest) { *dest = -qIn.z; }
	void _copy_w(const quaternion_t<T> &qIn, T *dest) { *dest = qIn.w; }
	void _copy_neg_w(const quaternion_t<T> &qIn, T *dest) { *dest = -qIn.w; }

	// function pointers for copying data during custom mul
	typedef void (quaternion_math_custom::*copy_ptr)(const quaternion_t<T> &, T *);

	// custom multiplication column pointers for non-constant
	copy_ptr mul_column_cp[4];
	
	// custom matrix
	copy_ptr matrix_cp[16];
	T qB_final_matrix_cells[16];
	bool default_sign_negative[16];
};


#endif