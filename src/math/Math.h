#pragma once

/*
Some (2D) math library functions, some faster and/or less accurate than the standard functions
Author: bim.overbohm@googlemail.com
*/
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#ifdef Success
    //Both Eigen and X11 define a symbol "Success". Undef it otherwise compilation fails.
    #undef Success
#endif

#include <Eigen/Eigen>
#include <Eigen/StdVector> //this needs to be included if fixed-size Eigen values are being used in std containers

typedef Eigen::Vector2f vec2;
typedef Eigen::Vector3f vec3;
typedef Eigen::Vector4f vec4;
typedef Eigen::Vector4f col4;
typedef Eigen::Matrix2f mat2;
typedef Eigen::Matrix3f mat3;
typedef Eigen::Matrix4f mat4;
typedef Eigen::Quaternionf quat;

EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(vec4)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(mat2)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(mat4)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(quat)

#include "half.h"
#include "half2.h"
#include "half3.h"
#include "half4.h"


#define USE_LIBRARY_FUNCTIONS //define this to use standard library functions

//Standard math.h defines
//#define M_E        2.71828182845904523536
//#define M_LOG2E    1.44269504088896340736
//#define M_LOG10E   0.434294481903251827651
//#define M_LN2      0.693147180559945309417
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI2      6.28318530717958647692 //2*PI
#define M_PIPI     9.86960440108935861883 //PI*PI
#define M_PI_2     1.57079632679489661923 //PI/2
//#define M_PI_4     0.785398163397448309616 //PI/4
//#define M_1_PI     0.318309886183790671538 //1/PI
//#define M_2_PI     0.636619772367581343076 //2/PI
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880 //sqrt(2)
//#define M_SQRT1_2  0.707106781186547524401 //sqrt(1/2)

//Standard math.h functions
namespace Math
{
    const float epsilon = 0.00001f;

	float acos(float x);
	float asin(float x);
	float atan(float x);
	float atan2(float y, float x);
	float cos(float x);
	float sin(float x);
	float tan(float x);

	float cosh(float x);
	float sinh(float x);
	float tanh(float x);

	float exp(float x);
	float log(float x);
	float log10(float x);
	float modf(float x, float * y);

	float pow(float x, float y);
	float sqrt(float x);
	float invsqrt(float x);

	float abs(float x);
	float ceil(float x);
	int ceili(float x);
	float floor(float x);
	int floori(float x);
	float trunc(float x);
	int trunci(float x);
	float fmod(float x, float y);

	float rand(int * seed); //returns value between -1 and 1.
};

//----- some conversion helpers ----------------------------------------------------------------------------------------

//Convert float color to RGBA8888
inline void convert(unsigned int * destination, const col4 * source, const size_t n)
{
    const col4 s255(255.0f, 255.0f, 255.0f, 255.0f);
	for (size_t i = 0; i < n; ++i) {
        unsigned char * dest = (unsigned char *)destination;
        col4 col = source[i].cwiseProduct(s255);
        dest[0] = (unsigned char)col(0);
        dest[1] = (unsigned char)col(1);
        dest[2] = (unsigned char)col(2);
        dest[3] = (unsigned char)col(3);
    }
}

//----- some utility functions/templates -------------------------------------------------------------------------------
#define VERTEX_TOLERANCE 0.0001f

template<typename Derived>
bool is_not_nan(Derived x)
{
	return x == x;
}

template<typename Derived>
bool is_finite(Derived x)
{
	return is_not_nan(x - x);
}

template<typename Derived>
bool is_valid(Derived x)
{
	return (x == x) && ((x - x) == (x - x));
}

template <typename Derived>
bool is_approx_absolute(Derived x, Derived y, float epsilon = VERTEX_TOLERANCE)
{
    return ((x - y) * (x - y)) < (epsilon * epsilon);
}

template <typename Derived>
bool is_approx_relative(Derived x, Derived y, float epsilon = VERTEX_TOLERANCE)
{
    return y != 0 && abs(abs(x / y) - 1) < epsilon;
}

//---- PLEASE NOTE! ----
//EIGEN IS COLUMN-MAJOR BY DEFAULT!
//---- PLEASE NOTE! ----

template<typename Derived>
inline bool is_finite(const Eigen::MatrixBase<Derived> & x)
{
	return ( (x - x).array() == (x - x).array()).all();
}

template<typename Derived>
inline bool is_not_nan(const Eigen::MatrixBase<Derived> & x)
{
	return ((x.array() == x.array())).all();
}

template<typename Derived>
inline bool is_valid(const Eigen::MatrixBase<Derived> & x)
{
	return ((x.array() == x.array())).all() && ((x - x).array() == (x - x).array()).all();
}

template <typename Derived>
bool is_approx_absolute(const Eigen::MatrixBase<Derived> & x, const Eigen::MatrixBase<Derived> & y, float epsilon = VERTEX_TOLERANCE)
{
    return Derived(x - y).squaredNorm() < (epsilon * epsilon);
}

template <typename Derived>
bool is_approx_relative(const Eigen::MatrixBase<Derived> & x, const Eigen::MatrixBase<Derived> & y, float epsilon = VERTEX_TOLERANCE)
{
    return y != 0 && Derived(Derived(x.array() / y.array()).cwiseAbs() - 1).cwiseAbs() < epsilon;
}

