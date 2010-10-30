// Credits:
// Code, Shawn Halayka
//	- shalayka@sasktel.net
// Configurable multiplication ("4D matrix and column vector concept") provided by Godwin Vickers
//	- ragnarok@hypercomplex.org  --  http://www.hypercomplex.org


#ifndef QUATERNION_MATH_H
#define QUATERNION_MATH_H


#define QUATERNION_MATH_LIB_VERSION 1.0


#include "quaternion_math_base.h"
#include "quaternion_math_default.h"
#include "quaternion_math_constant_column.h"
#include "quaternion_math_custom.h"
#include "quaternion_math_custom_constant_column.h"


#endif

/*
functions that do not rely on multiplication setup (included in quaternion_math_base):
	sin
	sinh
	exsin
	exsinh
	coversin
	coversinh
	cos
	cosh
	excos
	excosh
	versin
	versinh
	add
	sub
	ln
	exp
	sqrt
	inverse
	conjugate

functions that rely on multiplication setup (unique version included in each of the derived classes:
	mul
	pow
	pow_dynamic
	div
	sinc
	sinhc
	csc
	csch
	excsc
	excsch
	covercsc
	covercsch
	cscc
	cschc
	cosc
	coshc
	sec
	sech
	exsec
	exsech
	vercsc
	vercsch
	secc
	sechc
	tan
	tanh
	extan
	extanh
	covertan
	covertanh
	tanc
	tanhc
	cot
	coth
	excot
	excoth
	covercot
	covercoth
	cotc
	cothc
*/