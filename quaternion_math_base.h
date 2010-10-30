// Credits:
// Code, Shawn Halayka
//	- shalayka@sasktel.net
// Configurable multiplication ("4D matrix and column vector concept") provided by Godwin Vickers
//	- ragnarok@hypercomplex.org  --  http://www.hypercomplex.org


#ifndef QUATERNION_MATH_BASE_H
#define QUATERNION_MATH_BASE_H

#include <string>
#include <cmath>

template <class T> class quaternion_t
{
public:

// if using msvc++, use the __forceinline keyword to try to guarantee inlining of these functions
// else, just suggest it and hope for the best

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	quaternion_t(void) : x(0.0), y(0.0), z(0.0), w(0.0){}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	quaternion_t(const T &src_x, const T &src_y, const T &src_z, const T &src_w) : x(src_x), y(src_y), z(src_z), w(src_w){}

	T x, y, z, w;
};



template <class T> class quaternion_math_base
{
public:

	quaternion_math_base(const bool &init_everything = true)
	{
		if(init_everything)
		{
			temp_a_x = temp_a_y = temp_a_z = temp_a_w = 0;
			temp_b_x = temp_b_y = temp_b_z = temp_b_w = 0;
			temp_c_x = temp_c_y = temp_c_z = temp_c_w = 0;
			pow_exponent = 1;
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void setup_pow_exponent(const T &src_pow_exponent)
	{
		pow_exponent = static_cast<long unsigned int>(src_pow_exponent);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	virtual bool setup_constant_column_values(const T &mc1, const T &mc2, const T &mc3, const T &mc4)
	{
		// do nothing, the appropriate derived classes will override this
		return false;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	virtual bool setup_constant_column_values(const quaternion_t<T> *const qA)
	{
		// do nothing, the appropriate derived classes will override this
		return false;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	virtual bool setup_custom_column(const std::string &mc1, const std::string &mc2, const std::string &mc3, const std::string &mc4)
	{
		// do nothing, the appropriate derived classes will override this
		return false;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	virtual bool setup_custom_matrix(
		const std::string &_11, const std::string &_12, const std::string &_13, const std::string &_14,
		const std::string &_21, const std::string &_22, const std::string &_23, const std::string &_24,
		const std::string &_31, const std::string &_32, const std::string &_33, const std::string &_34,
		const std::string &_41, const std::string &_42, const std::string &_43, const std::string &_44)
	{
		// do nothing, the appropriate derived classes will override this
		return false;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void sin(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		//|V| = sqrt(v.x^2 + v.y^2 + v.z^2)
		//sin(q) = sin(t) * cosh(|V|), cos(t) * sinh(|V|) * V / |V|

		T mag_vector = std::sqrt(qA->y*qA->y + qA->z*qA->z + qA->w*qA->w);

		temp_a_x = qA->x;

		qOut->x = std::sin(temp_a_x) * std::cosh(mag_vector);
		qOut->y = std::cos(temp_a_x) * std::sinh(mag_vector) * qA->y / mag_vector;
		qOut->z = std::cos(temp_a_x) * std::sinh(mag_vector) * qA->z / mag_vector;
		qOut->w = std::cos(temp_a_x) * std::sinh(mag_vector) * qA->w / mag_vector;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void sinh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		//|V| = sqrt(v.x^2 + v.y^2 + v.z^2)
		//sin(q) = sin(t) * cosh(|V|), cos(t) * sinh(|V|) * V / |V|

		T mag_vector = std::sqrt(qA->y*qA->y + qA->z*qA->z + qA->w*qA->w);

		temp_a_x = qA->x;

		qOut->x = std::sinh(temp_a_x) * std::cos(mag_vector);
		qOut->y = std::cosh(temp_a_x) * std::sin(mag_vector) * qA->y / mag_vector;
		qOut->z = std::cosh(temp_a_x) * std::sin(mag_vector) * qA->z / mag_vector;
		qOut->w = std::cosh(temp_a_x) * std::sin(mag_vector) * qA->w / mag_vector;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void exsin(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> sin_quat;

		sin(qA, 0, &sin_quat);
		sub(&sin_quat, &one_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void exsinh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> sinh_quat;

		sinh(qA, 0, &sinh_quat);
		sub(&sinh_quat, &one_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void coversin(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> sin_quat;

		sin(qA, 0, &sin_quat);
		sub(&one_quat, &sin_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void coversinh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> sinh_quat;

		sinh(qA, 0, &sinh_quat);
		sub(&one_quat, &sinh_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void cos(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		//|V| = sqrt(v.x^2 + v.y^2 + v.z^2)
		//cos(q) = cos(t) * cosh(|V|), -sin(t) * sinh(|V|) * V / |V|

		T mag_vector = std::sqrt(qA->y*qA->y + qA->z*qA->z + qA->w*qA->w);

		temp_a_x = qA->x;

		qOut->x =  std::cos(temp_a_x) * std::cosh(mag_vector);
		qOut->y = -std::sin(temp_a_x) * std::sinh(mag_vector) * qA->y / mag_vector;
		qOut->z = -std::sin(temp_a_x) * std::sinh(mag_vector) * qA->z / mag_vector;
		qOut->w = -std::sin(temp_a_x) * std::sinh(mag_vector) * qA->w / mag_vector;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void cosh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		//|V| = sqrt(v.x^2 + v.y^2 + v.z^2)
		//cos(q) = cos(t) * cosh(|V|), -sin(t) * sinh(|V|) * V / |V|

		T mag_vector = std::sqrt(qA->y*qA->y + qA->z*qA->z + qA->w*qA->w);

		temp_a_x = qA->x;

		qOut->x = std::cosh(temp_a_x) * std::cos(mag_vector);
		qOut->y = std::sinh(temp_a_x) * std::sin(mag_vector) * qA->y / mag_vector;
		qOut->z = std::sinh(temp_a_x) * std::sin(mag_vector) * qA->z / mag_vector;
		qOut->w = std::sinh(temp_a_x) * std::sin(mag_vector) * qA->w / mag_vector;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void excos(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> cos_quat;

		cos(qA, 0, &cos_quat);
		sub(&cos_quat, &one_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void excosh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> cosh_quat;

		cosh(qA, 0, &cosh_quat);
		sub(&cosh_quat, &one_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void versin(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> cos_quat;

		cos(qA, 0, &cos_quat);
		sub(&one_quat, &cos_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void versinh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		static quaternion_t<T> one_quat(1, 0, 0, 0);
		quaternion_t<T> cosh_quat;

		cosh(qA, 0, &cosh_quat);
		sub(&one_quat, &cosh_quat, qOut);
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void add(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		temp_a_x = qA->x;
		temp_a_y = qA->y;
		temp_a_z = qA->z;
		temp_a_w = qA->w;

		temp_b_x = qB->x;
		temp_b_y = qB->y;
		temp_b_z = qB->z;
		temp_b_w = qB->w;

		qOut->x = temp_a_x + temp_b_x;
		qOut->y = temp_a_y + temp_b_y;
		qOut->z = temp_a_z + temp_b_z;
		qOut->w = temp_a_w + temp_b_w;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void sub(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		temp_a_x = qA->x;
		temp_a_y = qA->y;
		temp_a_z = qA->z;
		temp_a_w = qA->w;

		temp_b_x = qB->x;
		temp_b_y = qB->y;
		temp_b_z = qB->z;
		temp_b_w = qB->w;

		qOut->x = temp_a_x - temp_b_x;
		qOut->y = temp_a_y - temp_b_y;
		qOut->z = temp_a_z - temp_b_z;
		qOut->w = temp_a_w - temp_b_w;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void ln(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		temp_a_x = qA->x;
		temp_a_y = qA->y;
		temp_a_z = qA->z;
		temp_a_w = qA->w;

		T quat_length = std::sqrt(temp_a_x*temp_a_x + temp_a_y*temp_a_y + temp_a_z*temp_a_z + temp_a_w*temp_a_w);

		// make into unit quaternion_t if necessary
		if(1 != quat_length)
		{
			temp_a_x /= quat_length;
			temp_a_y /= quat_length;
			temp_a_z /= quat_length;
			temp_a_w /= quat_length;
		}

		//ln(q) = 0.5 * ln(t^2 + V.V), atan2(|V|, t) * V / |V|
		T vector_dot_prod = temp_a_y*temp_a_y + temp_a_z*temp_a_z + temp_a_w*temp_a_w;

		T vector_length = std::sqrt(vector_dot_prod);

		qOut->x = 0.5 * std::log(temp_a_x*temp_a_x + vector_dot_prod);
		qOut->y = (std::atan2(vector_length, temp_a_x) * temp_a_y) / vector_length;
		qOut->z = (std::atan2(vector_length, temp_a_x) * temp_a_z) / vector_length;
		qOut->w = (std::atan2(vector_length, temp_a_x) * temp_a_w) / vector_length;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void exp(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		//exp(q) = exp(t) * cos(|V|), exp(t) * sin(|V|) * V / |V|

		T mag_vector = std::sqrt(qA->y*qA->y + qA->z*qA->z + qA->w*qA->w);

		temp_a_x = qA->x;

		qOut->x = std::exp(temp_a_x) * std::cos(mag_vector);
		qOut->y = std::exp(temp_a_x) * std::sin(mag_vector) * qA->y / mag_vector;
		qOut->z = std::exp(temp_a_x) * std::sin(mag_vector) * qA->z / mag_vector;
		qOut->w = std::exp(temp_a_x) * std::sin(mag_vector) * qA->w / mag_vector;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void sqrt(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		if(qA->y == 0 && qA->z == 0 && qA->w == 0)
		{
			if(qA->x >= 0)
			{
				qOut->x = std::sqrt(qA->x);
				qOut->y = 0;
				qOut->z = 0;
				qOut->w = 0;
			}
			else
			{
				qOut->x = std::sqrt(-qA->x); //0;
				qOut->y = 0; //std::sqrt(-qA->x);
				qOut->z = 0;
				qOut->w = 0;
			}
		}
		else
		{
			temp_a_x = std::sqrt(qA->y*qA->y + qA->z*qA->z + qA->w*qA->w);

			if(qA->x >= 0)
			{
				T m = std::sqrt(0.5 * (std::sqrt(qA->x*qA->x + temp_a_x*temp_a_x) + qA->x));
				T l = temp_a_x / (2 * m);
				T t = l / temp_a_x;

				qOut->x = m;
				qOut->y = qA->y * t;
				qOut->z = qA->z * t;
				qOut->w = qA->w * t;
			}
			else
			{
				T l = std::sqrt(0.5 * (std::sqrt(qA->x*qA->x + temp_a_x*temp_a_x) - qA->x));
				T m = temp_a_x / (2 * l);
				T t = l / temp_a_x;

				qOut->x = m;
				qOut->y = qA->y * t;
				qOut->z = qA->z * t;
				qOut->w = qA->w * t;
			}
		}
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void inverse(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		// inv(a) = conjugate(a) / norm(a)

		T temp_a_norm = qA->x*qA->x + qA->y*qA->y + qA->z*qA->z + qA->w*qA->w;

		temp_a_x =  qA->x;
		temp_a_y = -qA->y;
		temp_a_z = -qA->z;
		temp_a_w = -qA->w;

		qOut->x = temp_a_x / temp_a_norm;
		qOut->y = temp_a_y / temp_a_norm;
		qOut->z = temp_a_z / temp_a_norm;
		qOut->w = temp_a_w / temp_a_norm;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void conjugate(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		qOut->x =  qA->x;
		qOut->y = -qA->y;
		qOut->z = -qA->z;
		qOut->w = -qA->w;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void copy(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		qOut->x = qA->x;
		qOut->y = qA->y;
		qOut->z = qA->z;
		qOut->w = qA->w;
	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void copy_masked(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		temp_a_x = qA->x;
		temp_a_y = qA->y;
		temp_a_z = qA->z;
		temp_a_w = qA->w;

		if(qB->x != 0.0)
		{
			if(qB->x == 1.0)
				qOut->x = temp_a_x;
			else if(qB->x == -1.0)
				qOut->x = -temp_a_x;
			else if(qB->x == 2.0)
				qOut->x = temp_a_y;
			else if(qB->x == -2.0)
				qOut->x = -temp_a_y;
			else if(qB->x == 3.0)
				qOut->x = temp_a_z;
			else if(qB->x == -3.0)
				qOut->x = -temp_a_z;
			else if(qB->x == 4.0)
				qOut->x = temp_a_w;
			else if(qB->x == -4.0)
				qOut->x = -temp_a_w;
		}

		if(qB->y != 0.0)
		{
			if(qB->y == 1.0)
				qOut->y = temp_a_x;
			else if(qB->y == -1.0)
				qOut->y = -temp_a_x;
			else if(qB->y == 2.0)
				qOut->y = temp_a_y;
			else if(qB->y == -2.0)
				qOut->y = -temp_a_y;
			else if(qB->y == 3.0)
				qOut->y = temp_a_z;
			else if(qB->y == -3.0)
				qOut->y = -temp_a_z;
			else if(qB->y == 4.0)
				qOut->y = temp_a_w;
			else if(qB->y == -4.0)
				qOut->y = -temp_a_w;
		}

		if(qB->z != 0.0)
		{
			if(qB->z == 1.0)
				qOut->z = temp_a_x;
			else if(qB->z == -1.0)
				qOut->z = -temp_a_x;
			else if(qB->z == 2.0)
				qOut->z = temp_a_y;
			else if(qB->z == -2.0)
				qOut->z = -temp_a_y;
			else if(qB->z == 3.0)
				qOut->z = temp_a_z;
			else if(qB->z == -3.0)
				qOut->z = -temp_a_z;
			else if(qB->z == 4.0)
				qOut->z = temp_a_w;
			else if(qB->z == -4.0)
				qOut->z = -temp_a_w;
		}

		if(qB->w != 0.0)
		{
			if(qB->w == 1.0)
				qOut->w = temp_a_x;
			else if(qB->w == -1.0)
				qOut->w = -temp_a_x;
			else if(qB->w == 2.0)
				qOut->w = temp_a_y;
			else if(qB->w == -2.0)
				qOut->w = -temp_a_y;
			else if(qB->w == 3.0)
				qOut->w = temp_a_z;
			else if(qB->w == -3.0)
				qOut->w = -temp_a_z;
			else if(qB->w == 4.0)
				qOut->w = temp_a_w;
			else if(qB->w == -4.0)
				qOut->w = -temp_a_w;
		}

	}

#ifdef _MSC_VER
	__forceinline
#else
	inline
#endif
	void swizzle(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut)
	{
		temp_a_x = qA->x;
		temp_a_y = qA->y;
		temp_a_z = qA->z;
		temp_a_w = qA->w;

		if(qB->x == 1.0)
			qOut->x = temp_a_x;
		else if(qB->x == 2.0)
			qOut->x = temp_a_y;
		else if(qB->x == 3.0)
			qOut->x = temp_a_z;
		else
			qOut->x = temp_a_w;

		if(qB->y == 1.0)
			qOut->y = temp_a_x;
		else if(qB->y == 2.0)
			qOut->y = temp_a_y;
		else if(qB->y == 3.0)
			qOut->y = temp_a_z;
		else
			qOut->y = temp_a_w;

		if(qB->z == 1.0)
			qOut->z = temp_a_x;
		else if(qB->z == 2.0)
			qOut->z = temp_a_y;
		else if(qB->z == 3.0)
			qOut->z = temp_a_z;
		else
			qOut->z = temp_a_w;

		if(qB->w == 1.0)
			qOut->w = temp_a_x;
		else if(qB->w == 2.0)
			qOut->w = temp_a_y;
		else if(qB->w == 3.0)
			qOut->w = temp_a_z;
		else
			qOut->w = temp_a_w;
	}


	// derived classes must override these pure virtual functions
	virtual void mul(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void pow(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void pow_dynamic(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void div(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void sinc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void sinhc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void csc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void csch(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void excsc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void excsch(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void covercsc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void covercsch(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void cscc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void cschc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void cosc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void coshc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void sec(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void sech(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void exsec(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void exsech(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void vercsc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void vercsch(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void secc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void sechc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void tan(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void tanh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void extan(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void extanh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void covertan(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void covertanh(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void tanc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void tanhc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void cot(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void coth(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void excot(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void excoth(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void covercot(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void covercoth(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void cotc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;
	virtual void cothc(const quaternion_t<T> *const qA, const quaternion_t<T> *const qB, quaternion_t<T> *const qOut) = 0;

protected:
	T temp_a_x, temp_a_y, temp_a_z, temp_a_w;
	T temp_b_x, temp_b_y, temp_b_z, temp_b_w;
	T temp_c_x, temp_c_y, temp_c_z, temp_c_w;
	quaternion_t<T> temp_quat;

	long unsigned int pow_exponent;
};



#endif