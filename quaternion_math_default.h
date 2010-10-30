// Credits:
// Code, Shawn Halayka
//	- shalayka@sasktel.net
// Configurable multiplication ("4D matrix and column vector concept") provided by Godwin Vickers
//	- ragnarok@hypercomplex.org  --  http://www.hypercomplex.org


#ifndef QUATERNION_MATH_DEFAULT_H
#define QUATERNION_MATH_DEFAULT_H


template <class T> class quaternion_math_default : public quaternion_math_base<T>
{
public:

	// no custom constructor required for this class

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void mul(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		// in case qA and qOut point to the same variable...
		temp_a_x = qA->x;
		temp_a_y = qA->y;
		temp_a_z = qA->z;
		temp_a_w = qA->w;

		temp_b_x = qB->x;
		temp_b_y = qB->y;
		temp_b_z = qB->z;
		temp_b_w = qB->w;

		// perform multiply
		qOut->x = temp_a_x*temp_b_x - temp_a_y*temp_b_y - temp_a_z*temp_b_z - temp_a_w*temp_b_w;
		qOut->y = temp_a_x*temp_b_y + temp_a_y*temp_b_x + temp_a_z*temp_b_w - temp_a_w*temp_b_z;
		qOut->z = temp_a_x*temp_b_z - temp_a_y*temp_b_w + temp_a_z*temp_b_x + temp_a_w*temp_b_y;
		qOut->w = temp_a_x*temp_b_w + temp_a_y*temp_b_z - temp_a_z*temp_b_y + temp_a_w*temp_b_x;
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
		else if(2 == pow_exponent)
		{
			// do fast multiply of identical quaternions
			temp_a_x = qA->x;
			temp_a_y = qA->y;
			temp_a_z = qA->z;
			temp_a_w = qA->w;

			qOut->x = temp_a_x*temp_a_x - temp_a_y*temp_a_y - temp_a_z*temp_a_z - temp_a_w*temp_a_w;
			qOut->y = temp_a_x*temp_a_y * 2;
			qOut->z = temp_a_x*temp_a_z * 2;
			qOut->w = temp_a_x*temp_a_w * 2;
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
		else if(2 == exp)
		{
			// do fast multiply of identical quaternions
			temp_a_x = qA->x;
			temp_a_y = qA->y;
			temp_a_z = qA->z;
			temp_a_w = qA->w;

			qOut->x = temp_a_x*temp_a_x - temp_a_y*temp_a_y - temp_a_z*temp_a_z - temp_a_w*temp_a_w;
			qOut->y = temp_a_x*temp_a_y * 2;
			qOut->z = temp_a_x*temp_a_z * 2;
			qOut->w = temp_a_x*temp_a_w * 2;
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
		// inv(b) = conjugate(b) / norm(b)
		// c = (conjugate(b) / norm(b)) * a

		T temp_b_norm = qB->x*qB->x + qB->y*qB->y + qB->z*qB->z + qB->w*qB->w;

		temp_b_x =  qB->x;
		temp_b_y = -qB->y;
		temp_b_z = -qB->z;
		temp_b_w = -qB->w;

		temp_b_x /= temp_b_norm;
		temp_b_y /= temp_b_norm;
		temp_b_z /= temp_b_norm;
		temp_b_w /= temp_b_norm;

		temp_a_x = qA->x;
		temp_a_y = qA->y;
		temp_a_z = qA->z;
		temp_a_w = qA->w;

		qOut->x = temp_b_x*temp_a_x - temp_b_y*temp_a_y - temp_b_z*temp_a_z - temp_b_w*temp_a_w;
		qOut->y = temp_b_x*temp_a_y + temp_b_y*temp_a_x + temp_b_z*temp_a_w - temp_b_w*temp_a_z;
		qOut->z = temp_b_x*temp_a_z - temp_b_y*temp_a_w + temp_b_z*temp_a_x + temp_b_w*temp_a_y;
		qOut->w = temp_b_x*temp_a_w + temp_b_y*temp_a_z - temp_b_z*temp_a_y + temp_b_w*temp_a_x;
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
};


#endif