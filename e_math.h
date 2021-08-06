#ifndef __E_MATH__H_
#define __E_MATH__H_


#pragma optimize("gt", on)


#if __AVX__
#define _VEC_SSE4_
#endif

#include <e_float.h>	//#include <emmintrin.h>

#include <smmintrin.h>


#pragma warning(disable: 4244)

#pragma pack(push, 1)

#define	PITCH	0	// up/down
#define	YAW		1	// left/right
#define	ROLL	2	// fall over

#define FLT_EPSILON 1.192092896e-07F

#if 0
	const float g_PI 		= 3.1415926535897932384626433832795f;
	const float g_TWO_PI 	= 6.283185307179586476925286766559f;		//2PI 360
	const float g_HALF_PI 	= 1.5707963267948966192313216916398f;		//PI/2 90
	const float g_DEG2RAD 	= 0.017453292519943295769236907684886f;		//PI/180
	const float g_RAD2DEG 	= 57.295779513082320876798154814105f;		//180/PI
#else
	#define g_PI 		3.14159265358979f
	#define g_TWO_PI 	6.28318530717958f
	#define g_HALF_PI 	1.57079632679489f
	#define g_DEG2RAD 	0.01745329251994f
	#define g_RAD2DEG 	57.2957795130823f
#endif

#define D_PI 		3.1415926535897932384626433832795
#define D_TWO_PI 	6.283185307179586476925286766559		//2PI 360
#define D_HALF_PI 	1.5707963267948966192313216916398		//PI/2 90
#define D_DEG2RAD 	0.017453292519943295769236907684886		//PI/180
#define D_RAD2DEG 	57.295779513082320876798154814105		//180/PI

#define DEG2RAD(a) ( (a) * g_DEG2RAD )
#define RAD2DEG(a) ( (a) * g_RAD2DEG )

#define DEG2RADF(a) ( (a) * FLT( g_DEG2RAD ) )
#define RAD2DEGF(a) ( (a) * FLT( g_RAD2DEG ) )

#ifdef _AMD64_
#define FLT2XMM(x)	_mm_set_ss(x)
#else
#define FLT2XMM(x)	_mm_load_ss(&x)
#endif

#ifndef XM_PERMUTE_PS
#define XM_PERMUTE_PS( v, c ) _mm_shuffle_ps( v, v, c )
#endif

//-------------------------------------------------------------------



__forceinline bool FltCheck(const double * ptr) // != 0
{
	return *(PUSHORT(ULONG_PTR(ptr) + 2+4)) != 0;
}

__forceinline UCHAR FltCheck(const float * ptr) // != 0
{
	return *( ((PUCHAR)ptr) + 3 );
}

__forceinline bool FltCheckNeg(const float * ptr)
{
	return *( ((PCHAR)ptr) + 3 ) < 0;
}



//float	mm_sqrt(float);
//double	mm_sqrtd(double);
//float	mm_fabs(float);
//double	mm_fabsd(double);
//float	mm_exp(float);
//float	mm_log(float);
//float	mm_floor(float);
//double	mm_floord(double);
//int		mm_floori(float);
//float	mm_pow(float, float);
//float	mm_ceil(float);
//int		mm_ceili(float);
//double	mm_ceild(double);
//float	mm_round(float);
//double	mm_roundd(double);

ULONG mm_rand_U();
ULONG_PTR mm_rand_UU();



//__forceinline float __fastcall mm_float_set(ULONG x)
//{
//	__m128 f = _mm_castsi128_ps(_mm_cvtsi32_si128(x));
//	return _mm_cvtss_f32(f);
//}
////#define FLTD(x) mm_float_set(x)



#ifndef MATH_C_IMPL

extern "C" {
	void	_vectorcall __cossin(float *);
	double	_vectorcall __sincosd(double *S, double A);
	double	_vectorcall __sin(double);
	double	_vectorcall __cos(double);
	double	_vectorcall __asin(double);
	double	_vectorcall __acos(double);
	double	_vectorcall __tan(double);
	float	_vectorcall __tanf(float);
	double	_vectorcall __atan2d(double y, double x);
	float	_vectorcall __atan2p(const float *y, const float *x);
}

__forceinline double _vectorcall mm_sin(double s)
{
	return __sin(s);
}

__forceinline double _vectorcall mm_cos(double s)
{
	return __cos(s);
}

__forceinline void _vectorcall mm_sincos(float a, float* pSin, float* pCos)
{
	float cs[2];
	cs[0] = a;
	__cossin(cs);
	*pCos = cs[0];
	*pSin = cs[1];
}

__forceinline void _vectorcall mm_sincosd(double a, double* pSin, double* pCos)
{
	*pCos = __sincosd(pSin, a);
}

//inline float mm_asin(float x)
//{
//	return __asin(x);
//}
//
//inline float mm_acos(float x)
//{
//	return __acos(x);
//}

//---------------------------------------------------------

__forceinline double _vectorcall mm_tan(double a)
{
	return __tan(a);
}
__forceinline float _vectorcall mm_tanf(float a)
{
	return __tanf(a);
}

__forceinline double _vectorcall mm_atan2d(double y, double x)
{
	return __atan2d(y, x);
}

__forceinline float mm_atan2__inl(float y, float x)
{
	return __atan2p(&y, &x);
}

__forceinline float _vectorcall mm_atan2(const float &y, const float &x)
{
	return __atan2p(&y, &x);
}

#else

float _vectorcall mm_atan2(float y, float x);
float _vectorcall mm_atan2p(const float *y, const float *x);
void  _vectorcall mm_sincos(float a, float *pSin, float *pCos);

#endif

//---------------------------------------------------------

inline float __vectorcall mm_sqrt(float x)
{
	return _mm_cvtss_f32(_mm_sqrt_ss(FLT2XMM(x)));
}

inline float __vectorcall mm_rsqrt(float x)
{
	return _mm_cvtss_f32(_mm_rsqrt_ss(FLT2XMM(x)));
}

inline double __vectorcall mm_sqrtd(double x)
{
	__m128d d = _mm_set_sd(x);//	__m128d d = _mm_load_sd(&x);
	return _mm_cvtsd_f64(_mm_sqrt_sd(d, d));//	return _mm_cvtsd_f64(_mm_sqrt_sd(_mm_setzero_pd(), d));
}

//---------------------------------------------------------

inline float __vectorcall mm_asin(float x)
{
	return mm_atan2(x, mm_sqrt(FLT(1)-x*x));
}

inline float __vectorcall mm_acos(float x)
{
	return mm_atan2(mm_sqrt(FLT(1)-x*x), x);
}

//---------------------------------------------------------




/*
inline float mm_log(float s)
{
	return _mm_cvtss_f32(mm_log_128(FLT2XMM(s)));
}

inline float mm_exp(float s)
{
	return _mm_cvtss_f32(mm_exp_128(FLT2XMM(s)));
}

inline float mm_pow(float mant, float expo)
{
	unsigned int e;
	long double ret;

	// special cases 0^x
	if( mant == 0.0 )
	{
		if( expo > 0.0 )
			return 0.0;
		else
		if( expo == 0.0 )
			return 1.0;
		else
			return 1.0 / mant;
	}

	//special cases x^n with n is integer
	if( expo == (int) (e = (int) expo) )
	{
		if( (int)e < 0 )
		{
			e = -e;
			mant = 1.0 / mant;
		}

		ret = 1.0;

		for(;;)
		{
			if( e & 1 )
				ret *= mant;

			if( (e >>= 1) == 0 )
				break;

			mant *= mant;
		}

		return ret;
	}

	return _mm_cvtss_f32(mm_pow_128(FLT2XMM(mant), FLT2XMM(expo)));
}*/


//-------------------------------------------------------------------


#ifndef __INTEL_COMPILER

#undef _mm_undefined_ps
#undef _mm_undefined_pd

__forceinline __m128 _mm_undefined_ps()
{
	return _mm_castsi128_ps( _mm_setzero_si128() );
}

__forceinline __m128d _mm_undefined_pd()
{
	return _mm_castsi128_pd( _mm_setzero_si128() );
}

#endif


//-------------------------------------------------------------------

__forceinline __m128i __fastcall mm_int4_set(ULONG x)
{
	ULONG X = x;
	__m128i i = _mm_cvtsi32_si128(X);
	return _mm_shuffle_epi32(i,0);
}

__forceinline __m128 __fastcall mm_float4_set(ULONG x)
{
	return _mm_castsi128_ps( mm_int4_set(x) );
}


//-------------------------------------------------------------------


__forceinline __m128i __vectorcall _mm_make_FFFF()
{
#ifndef _PRELDREX_
	return mm_int4_set(~0);
#else
	return _mm_cmpeq_epi32(_mm_setzero_si128(),_mm_castps_si128(_mm_setzero_ps())); // opt fkup
#endif
}



__forceinline __m128i mm_int4_SL_1(const int L)//, __m128i n = _mm_make_FFFF())
{
	__m128i n = mm_int4_set(1);//	n = _mm_srli_epi32(n, 31);
	return _mm_slli_epi32(n, L);
}
__forceinline __m128i mm_int4_SIGN_mask()
{
	return mm_int4_SL_1(31);
}

__forceinline __m128i mm_int4_SL_SR(const int L, const int R, __m128i n = _mm_make_FFFF())
{
	n = _mm_slli_epi32(n, L); // <<
	n = _mm_srli_epi32(n, R); // >>
	return n;
}

__forceinline __m128i mm_int4_SR_SL(const int R, const int L, __m128i n = _mm_make_FFFF())
{
	n = _mm_srli_epi32(n, R); // >>
	n = _mm_slli_epi32(n, L); // <<
	return n;
}



__forceinline __m128 __vectorcall mm_float4_mul_2(const __m128 v)
{
	__m128i n = _mm_add_epi32(_mm_castps_si128(v), mm_int4_SL_1(23) ); // 0x800000
	return _mm_castsi128_ps(n);
}

__forceinline __m128 __vectorcall mm_float4_div_2(const __m128 v)
{
	__m128i n = _mm_sub_epi32(_mm_castps_si128(v), mm_int4_SL_1(23) );
	return _mm_castsi128_ps(n);
}


__forceinline __m128i __vectorcall mm_int4_const_05()
{
#if 1
	return mm_int4_set( 0x3f000000 );
#else
	return mm_int4_SL_SR(26, 2);
#endif
}

__forceinline __m128 __vectorcall mm_float4_const_05()
{
	return _mm_castsi128_ps(mm_int4_const_05());
}


__forceinline __m128i __vectorcall mm_int4_const_1()
{
#if 1
	return mm_int4_set( 0x3f800000 );
#else
	return mm_int4_SL_SR(25, 2);
#endif
}

__forceinline __m128 __vectorcall mm_float4_const_1()
{
	return _mm_castsi128_ps(mm_int4_const_1());
}


__forceinline __m128 __vectorcall mm_float4_const_2()
{
#if 1
	return _mm_castsi128_ps(mm_int4_set(0x40000000));
#else
	return _mm_castsi128_ps(mm_int4_SL_SR(31, 1));
#endif
}


//-------------------------------------------------------------------

__forceinline float __vectorcall mm_int2float(int i)
{
	return (float)i;
	//__m128 f = _mm_cvtepi32_ps(_mm_cvtsi32_si128(i));	//__m128 f = _mm_cvt_si2ss(_mm_undefined_ps(), i);
	//return _mm_cvtss_f32(f);
}

__forceinline int __vectorcall mm_float2int(float x)
{
	////return (int)x;
	__m128 a = FLT2XMM(x); // _mm_set_ss
	return _mm_cvt_ss2si(a);
}

__forceinline int __vectorcall mm_float2int(float *p)
{
	return mm_float2int(*p);
}


#if defined(_M_X64) || defined(_AMD64_)

__forceinline double __vectorcall mm_long2dbl(__int64 i)
{
	__m128d f = _mm_undefined_pd();
	f = _mm_cvtsi64_sd(f, i);
	return _mm_cvtsd_f64(f);
}

__forceinline float __vectorcall mm_long2float(__int64 i)
{
	__m128 f = _mm_undefined_ps();
	f = _mm_cvtsi64_ss(f, i);
	return _mm_cvtss_f32(f);
}

__forceinline __int64 __vectorcall mm_dbl2long(double x)
{
	__m128d a = _mm_set_sd(x);
	return _mm_cvtsd_si64(a);
}

__forceinline __int64 __vectorcall mm_dbl2long(double *p)
{
	__m128d a = _mm_load_sd(p);
	return _mm_cvtsd_si64(a);
}

__forceinline __int64 __vectorcall mm_float2long(float x)
{
	__m128 a = FLT2XMM(x);
	return _mm_cvtss_si64(a);
}

__forceinline __int64 __vectorcall mm_float2long(float *p)
{
	__m128 a = _mm_load_ss(p);
	return _mm_cvtss_si64(a);
}

#endif


//-------------------------------------------------------------------


__forceinline bool __vectorcall mm_0to1f(float f)
{
	//struct IEEFLT
	//{
	//	ULONG mantissa : 23;
	//	ULONG exponent : 8;
	//	ULONG sign : 1;
	//};
	//((IEEFLT*)&f)->exponent;
//	__m128i ii = _mm_castps_si128(FLT2XMM(f));
	ULONG i = (ULONG)_mm_cvtsi128_si32(_mm_castps_si128(FLT2XMM(f)));
	return i < 0x3f800000;
	//return i > 0 && i < 0x3f800000;
}



__forceinline float __vectorcall mm_neg(float s)
{
	//return FLT(0) - s;
	float n = _mm_cvtss_f32(_mm_castsi128_ps(_mm_setzero_si128()));
	return (n - s);
////#if 1
//	register __m128 N = _mm_sub_ss(_mm_setzero_ps(), FLT2XMM(s));
//	return _mm_cvtss_f32(N);
////#else
////	__m128i i = _mm_xor_si128(_mm_castps_si128(FLT2XMM(s)), mm_int4_SIGN_mask());
////	return _mm_cvtss_f32(_mm_castsi128_ps(i));
////#endif
}


__forceinline double __vectorcall mm_negd(double s)
{
#if 1
	 __m128d N = _mm_sub_sd(_mm_setzero_pd(), _mm_set_sd(s));
	return _mm_cvtsd_f64(N);
#else
	__m128d f = _mm_set_sd(s);
	__m128i x = _mm_cvtsi32_si128(1);
	x = _mm_slli_epi64(x, 63);
	__m128i i = _mm_castpd_si128(f);
	i = _mm_xor_si128(i, x);
	return _mm_cvtsd_f64(_mm_castsi128_pd(i));
#endif
}


__forceinline __m128 __vectorcall xmm_fabs(__m128 s)
{
	__m128i i = _mm_castps_si128(s);
	i = _mm_slli_epi32(i, 1);
	i = _mm_srli_epi32(i, 1);
	return _mm_castsi128_ps(i);
}

__forceinline float __vectorcall mm_fabs(float s)
{
	return _mm_cvtss_f32(xmm_fabs(FLT2XMM(s)));
}


__forceinline double __vectorcall mm_fabsd(double s)
{
	__m128i i = _mm_castpd_si128(_mm_set_sd(s));
	i = _mm_slli_epi64(i, 1);
	i = _mm_srli_epi64(i, 1);
	return _mm_cvtsd_f64(_mm_castsi128_pd(i));
}


//-------------------------------------------------------------------


class dVec2
{
public:
	double x, y;
};

class dVec3
{
public:
	union
	{
		struct
		{
			double x, y, z;
		};
		struct
		{
			double pitch, yaw, roll;
		};
	};
};


//-------------------------------------------------------------------


class Vector;
typedef Vector QAngle;

class Vector
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		struct
		{
			float pitch, yaw, roll;
		};
	};

public:
	__forceinline Vector(float X, float Y, float Z)
	{
		x = X;	y = Y;	z = Z;
	}
	__forceinline Vector(float v)
	{
		x = v;	y = v;	z = v;
	}
	__forceinline Vector(const float *XYZ)
	{
		Init(XYZ);
	}
	__forceinline Vector()			
	{
	}
	__forceinline void Init()
	{
		x = y = z = 0;
	}
	__forceinline void Init(float X, float Y, float Z)
	{
		x = X;	y = Y;	z = Z;
	}
	__forceinline void Init(const Vector &v)
	{
		*this = v;
	}
	__forceinline void Init(const float *XYZ)
	{
		*this = *(const Vector *)XYZ;
	}
	__forceinline void InitXZY(const float *XZY)
	{
		const Vector &v = *(const Vector *)XZY;
		x = v.x;
		y = v.z;
		z = v.y;
	}
	__forceinline void SwapZY()
	{
		float t = y;
		y = z;
		z = t;
	}
	__forceinline Vector& operator = (const float *v)
	{
		Init(v);
		return *this; 
	}

	__forceinline float& operator[] (ULONG i)
	{
		return (&x)[i];
	}
	__forceinline float operator[] (ULONG i) const
	{
		return (&x)[i];
	}

	inline Vector& operator += (const Vector& v)
	{ 
		x += v.x;
		y += v.y;
		z += v.z;	
		return *this;
	}
	inline Vector& operator -= (const Vector& v)
	{ 
		x -= v.x;
		y -= v.y;
		z -= v.z;	
		return *this;
	}
	Vector& _vectorcall operator *= (float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}
	__forceinline Vector& operator *= (const Vector& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	Vector& _vectorcall operator /= (float fl)
	{
		fl = FLT(1) / fl;
		return this->operator *= (fl);
	}
	__forceinline Vector& operator /= (const Vector& v)
	{ 
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	__forceinline Vector operator - () const
	{
		return Vector(-x, -y, -z);
	}
	__forceinline Vector operator + (const Vector& v) const
	{
		return Vector( x + v.x, y + v.y, z + v.z );
	}
	__forceinline Vector operator - (const Vector& v) const
	{
		return Vector( x - v.x, y - v.y, z - v.z );
	}
	__forceinline Vector operator * (const Vector& v) const
	{
		return Vector( x * v.x, y * v.y, z * v.z );
	}
	__forceinline Vector operator / (const Vector& v) const
	{
		return Vector( x / v.x, y / v.y, z / v.z );
	}
	Vector _vectorcall operator * (float fl) const
	{
		return Vector(x * fl, y * fl, z * fl);
	}
	Vector _vectorcall operator / (float fl) const
	{
		fl = FLT(1) / fl;
		return this->operator * (fl);
	}
	float _vectorcall LengthSqr() const
	{
		return x*x + y*y + z*z;
	}
	float _vectorcall Length() const
	{
		return mm_sqrt( LengthSqr() );
	}
	float _vectorcall Length_R() const
	{
		return mm_rsqrt( LengthSqr() );
	}
	inline Vector& Normalize()
	{
		float fl = Length_R();
		return this->operator *= (fl);
    }

	static VOID NormalizeAngle(float & a)
	{
		if(a >= FLT( g_PI )) a -= FLT( g_TWO_PI );
		if(a <  FLT(-g_PI )) a += FLT( g_TWO_PI );
	}
	inline Vector& NormalizeAngle()
	{
		for(float* p = &x; p <= &z; p++) NormalizeAngle(*p);
		return *this;
    }

	static __forceinline float _vectorcall fNormalizeAngle(float a)
	{
		if(a >= FLT( g_PI )) a -= FLT( g_TWO_PI );
		if(a <  FLT(-g_PI )) a += FLT( g_TWO_PI );
		return a;
	}


	__forceinline float Length2D() const
	{
		return mm_sqrt(x*x + y*y);
	}
	__forceinline float Length2D_XZY() const
	{
		return mm_sqrt(x*x + z*z);
	}

	float _vectorcall Dot(const Vector& v) const
	{
		return x*v.x + y*v.y + z*v.z;
	}
	float _vectorcall operator | (const Vector& v) const
	{
		return Dot(v);
	}
	__forceinline Vector Cross(const Vector& v) const
	{
		return Vector( y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x ); 
	}
	__forceinline bool IsNull()
	{
		return !FltCheck(&x) && !FltCheck(&y) && !FltCheck(&z);
	}
//--------------------------------------------------------------------

	__forceinline Vector & RAD()
	{ 
		FLOAT m = FLT( g_DEG2RAD );
		return this->operator *= (m);
	}
	__forceinline Vector & DEG()
	{ 
		FLOAT m = FLT( g_RAD2DEG );
		return this->operator *= (m);
	}

	VOID GetTransform(Vector *pForward, Vector *pRight, Vector *pUp) const
	{
		Vector s;
		Vector c;

		mm_sincos(pitch, &s.pitch, &c.pitch);
		mm_sincos(yaw,   &s.yaw,   &c.yaw);

		if(pForward)
		{
			pForward->x = (c.pitch * c.yaw);
			pForward->y = (c.pitch * s.yaw);
			pForward->z = mm_neg( s.pitch );
		}

		if(pRight || pUp)
		{
			mm_sincos(roll,  &s.roll,  &c.roll);

			if(pRight)
			{
				float m1 = FLT(-1);
				pRight->x = (m1 * s.roll * s.pitch * c.yaw + c.roll * s.yaw);
				pRight->y = (m1 * s.roll * s.pitch * s.yaw - c.roll * c.yaw);
				pRight->z = (m1 * s.roll * c.pitch);
			}

			if(pUp)
			{
				pUp->x = (c.roll * s.pitch * c.yaw + s.roll * s.yaw);
				pUp->y = (c.roll * s.pitch * s.yaw - s.roll * c.yaw);
				pUp->z = (c.roll * c.pitch);
			}
		}
	}

	__forceinline Vector GetForward() const
	{
		Vector Forward;
		GetTransform(&Forward, 0, 0);
		return Forward;
	}
	//inline Vector MakeVector() const
	//{
	//	float sin_pitch, sin_yaw;
	//	float cos_pitch, cos_yaw;
	//	mm_sincos(x, &sin_pitch, &cos_pitch);
	//	mm_sincos(y, &sin_yaw, &cos_yaw);
	//	return Vector(-cos_pitch * -cos_yaw, cos_pitch * sin_yaw, -sin_pitch);
	//}
	Vector MakeAngles() const
	{
		float Yaw;
		float Pitch;

		if(	!FltCheck(&x)	&&
			!FltCheck(&y)	)
		{
			Yaw = 0;

			if(z > 0)	Pitch = FLT( g_HALF_PI );
			else		Pitch = FLT(-g_HALF_PI );
		}
		else
		{
			Yaw = mm_atan2(y, x);
			Pitch = mm_atan2(z, mm_sqrt( x*x + y*y ));
		}
		return Vector( mm_neg(Pitch), Yaw, 0 );
	}
	float _vectorcall GetAngle(const Vector &b)
	{
		FLOAT mm = Length() * b.Length();
		return mm_fabs( mm_acos( Dot(b) / mm ) );
	}
};


__forceinline Vector operator * (float fl, const Vector& v)	
{ 
	return v * fl; 
}


__forceinline Vector operator ^ (const Vector &v0, const Vector &v1)
{
	return v0.Cross(v1); //Vector(v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x ); 
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


class TPlane
{
public:
	union
	{
		struct
		{
			Vector	n;
			FLOAT	d;
		};
		struct
		{
			FLOAT	x,y,z,w;
		};
	};

	inline TPlane()	{}
	inline TPlane(const TPlane& P) = default;

	inline TPlane(const Vector& V, float D = 0.0f)
	{
		n = V;
		d = D;
	}
	inline TPlane(const Vector V, const Vector &N)
	{
		n = V;
		d = (V | N);
	}
	inline void	Set(const Vector &vNormal, const float fDist)
	{	
		n = vNormal; 
		d = fDist;
	}
	void SetPlane(const Vector &normal, const Vector &point)
	{ 
		n = normal; 
		d = -(point | normal); 
	}
	friend TPlane GetPlane(const Vector &normal, const Vector &point)
	{  
		return TPlane(normal, -(point|normal));
	}
	inline float operator | (const Vector &point) const
	{
		return (n | point) + d;
	}
	void Init(const Vector & v1, const Vector & v2, const Vector & v3)
	{
		Vector u, v, t;
		u = v2 - v1;
		v = v2 - v3;
		t = u ^ v;
		t.Normalize();
		n = t;
		d = (t * v1).Length();
	}
	inline float DistFromPlane(const Vector &vPoint) const
	{
		return (n * vPoint).Length() - d;
	}
	Vector MirrorVector(Vector& i)
	{
		return i - n * ((n|i) * FLT(2));
	}
	Vector MirrorPosition(Vector& i)
	{
		return i - n * (FLT(2) * ((n|i) - d));
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MM_ALIGN16
#define _MM_ALIGN16 __declspec(align(16))
#endif

#ifndef _MM_ALIGN8
#define _MM_ALIGN8 __declspec(align(8))
#endif



template<class MATR> 
inline MATR _vectorcall MatrixTranspose(const MATR M)
{
	MATR mResult;
# if 0
	__m128 m0 = M.r[0];
	__m128 m1 = M.r[1];
	__m128 m2 = M.r[2];
	__m128 m3 = M.r[3];
	__m128 m4 = m0;
	__m128 m6 = m2;

	m0 = _mm_unpackhi_ps(m0, m1);
	m2 = _mm_unpackhi_ps(m2, m3);

	m1 = _mm_unpacklo_ps(m1, m4);
	m3 = _mm_unpacklo_ps(m3, m6);

	m4 = m0;
	m0 = _mm_shuffle_ps(m0, m2, 0xEE);
	m4 = _mm_shuffle_ps(m4, m2, 0x44);

	m6 = m1;
	m1 = _mm_shuffle_ps(m1, m3, 0xBB);
	m6 = _mm_shuffle_ps(m6, m3, 0x11);

	mResult.r[0] = m6;
	mResult.r[1] = m1;
	mResult.r[2] = m4;
	mResult.r[3] = m0;
#else
    // x.x,x.y,y.x,y.y
    __m128 vTemp1 = _mm_shuffle_ps(M.r[0],M.r[1],_MM_SHUFFLE(1,0,1,0));
    // x.z,x.w,y.z,y.w
    __m128 vTemp3 = _mm_shuffle_ps(M.r[0],M.r[1],_MM_SHUFFLE(3,2,3,2));
    // z.x,z.y,w.x,w.y
    __m128 vTemp2 = _mm_shuffle_ps(M.r[2],M.r[3],_MM_SHUFFLE(1,0,1,0));
    // z.z,z.w,w.z,w.w
    __m128 vTemp4 = _mm_shuffle_ps(M.r[2],M.r[3],_MM_SHUFFLE(3,2,3,2));

    // x.x,y.x,z.x,w.x
    mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2,_MM_SHUFFLE(2,0,2,0));
    // x.y,y.y,z.y,w.y
    mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2,_MM_SHUFFLE(3,1,3,1));
    // x.z,y.z,z.z,w.z
    mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4,_MM_SHUFFLE(2,0,2,0));
    // x.w,y.w,z.w,w.w
    mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4,_MM_SHUFFLE(3,1,3,1));
#endif
	return mResult;
}




inline
void MatrixInitTranspose(_In_reads_(12) const float * rot, _In_reads_(4) const float * pos, float * out)
{
	__m128 m0 = _mm_loadu_ps( rot );		// &in._11
	__m128 m1 = _mm_loadu_ps( rot + 4 );	// &in._21
	__m128 m2 = _mm_loadu_ps( rot + 8 );	// &in._31
	__m128 m3 = _mm_loadu_ps( pos );		// &in._41
	__m128 m4 = m0;
	__m128 m6 = m2;

	m0 = _mm_unpackhi_ps(m0, m1);
	m2 = _mm_unpackhi_ps(m2, m3);

	m1 = _mm_unpacklo_ps(m1, m4);
	m3 = _mm_unpacklo_ps(m3, m6);

	m4 = m0;
	m0 = _mm_shuffle_ps(m0, m2, 0xEE);
	m4 = _mm_shuffle_ps(m4, m2, 0x44);

	m6 = m1;
	m1 = _mm_shuffle_ps(m1, m3, 0xBB);
	m6 = _mm_shuffle_ps(m6, m3, 0x11);

	_mm_storeu_ps(out     , m6);
	_mm_storeu_ps(out + 4 , m1);
	_mm_storeu_ps(out + 8 , m4);
	_mm_storeu_ps(out + 12, m0);
}




////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


//inline __m128 _mm_dp_ps_7F(__m128 a, __m128 b)
//{
//	//a = _mm_and_ps( a, _mm_castsi128_ps(_mm_set_epi32(0,-1,-1,-1)) ); // Clear input using the high bits of the mask
//	a = _mm_mul_ps( a, b );
//	a = _mm_hadd_ps( a, a );
//	a = _mm_hadd_ps( a, a );
//	return a;
//}

	//	__m128 v = Xmm(); LengthSqr
	//	return _mm_cvtss_f32(_mm_dp_ps_7F(v, v));
		//__m128 v = Xmm(); Length
		//return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps_7F(v, v)));
		//__m128 v = Xmm();Normalize
		//const float oob = _mm_cvtss_f32(_mm_rsqrt_ps(_mm_dp_ps_7F(v, v)));


struct Vec4m;
typedef const Vec4m FVec4m;

struct Vec4m
{
	__m128 _xyzw;

	Vec4m() = default;

	__forceinline Vec4m(float const *p) { load3(p); }
	__forceinline Vec4m(float f)	{ _xyzw = _mm_set_ps1(f); }

	__forceinline Vec4m(float x, float y, float z, float w)
	{
		_xyzw = _mm_setr_ps(x,y,z,w);
	}
	__forceinline Vec4m(__m128 v)
	{
		_xyzw = v;
	}
	__forceinline Vec4m(__m128i i)
	{
		_xyzw = _mm_castsi128_ps(i);
	}
    __forceinline operator __m128() const
	{
        return _xyzw;
    }

	template<int X, int Y, int Z, int W> inline __m128 shuffle() const // reversed  _MM_SHUFFLE
	{
		return _mm_shuffle_ps(_xyzw,_xyzw, _MM_SHUFFLE(W,Z,Y,X));
	}

	template<> inline __m128 shuffle<0,1,0,1>() const { return _mm_movelh_ps(_xyzw, _xyzw); }
	template<> inline __m128 shuffle<2,3,2,3>() const { return _mm_movehl_ps(_xyzw, _xyzw); }
	template<> inline __m128 shuffle<0,0,2,2>() const { return _mm_moveldup_ps(_xyzw); }
	template<> inline __m128 shuffle<1,1,3,3>() const { return _mm_movehdup_ps(_xyzw); }


	__forceinline __m128 dup_x() const { return shuffle<0,0,0,0>(); }
	__forceinline __m128 dup_y() const { return shuffle<1,1,1,1>(); }
	__forceinline __m128 dup_z() const { return shuffle<2,2,2,2>(); }
	__forceinline __m128 dup_w() const { return shuffle<3,3,3,3>(); }

	__forceinline __m128 dup_LO() const { return shuffle<0,1,0,1>(); }

	//__forceinline float get_y() { return _mm_cvtss_f32(_mm_castsi128_ps(_mm_srli_epi64(_mm_castps_si128(_xyzw), 32))); } //_mm_cvtss_f32(_mm_shuffle_ps(_xyzw,_xyzw, _MM_SHUFFLE2(0,1))); }
	//__forceinline float get_z() { return _mm_cvtss_f32(_mm_movehl_ps(_xyzw,_xyzw)); } //_mm_shuffle_ps(_xyzw,_xyzw, _MM_SHUFFLE2(1,0))); }
	//__forceinline float get_w() { return _mm_cvtss_f32(_mm_shuffle_ps(_xyzw,_xyzw, _MM_SHUFFLE2(1,1))); }

	__forceinline float get_x() const
	{
		return _mm_cvtss_f32(_xyzw);
	}
	__forceinline void set_x(float X)
	{
		_xyzw = _mm_move_ss(_xyzw, FLT2XMM(X));
	}

	__forceinline int get_x_bits() const
	{
		return _mm_cvtsi128_si32(_mm_castps_si128(_xyzw));
	}


	__forceinline float get_y() const { return _mm_cvtss_f32(dup_y()); }
	__forceinline float get_z() const { return _mm_cvtss_f32(dup_z()); }
	__forceinline float get_w() const { return _mm_cvtss_f32(dup_w()); }

	__forceinline void set_y_2d(float V) // xVy0
	{
		_xyzw = _mm_unpacklo_ps(_xyzw, FLT2XMM(V));
	}


#ifndef _VEC_SSE4_

	__forceinline void set_y(float Y)
	{
		__m128 v = _mm_shuffle_ps(_xyzw,_xyzw, _MM_SHUFFLE(3,2,0,1));
		v = _mm_move_ss(v, FLT2XMM(Y));
		_xyzw = _mm_shuffle_ps(v,v, _MM_SHUFFLE(3,2,0,1));
	}
	__forceinline void set_z(float Z)
	{
		__m128 v = _mm_shuffle_ps(_xyzw,_xyzw, _MM_SHUFFLE(3,0,1,2)); // zx
		v = _mm_move_ss(v, FLT2XMM(Z));
		_xyzw = _mm_shuffle_ps(v,v, _MM_SHUFFLE(3,0,1,2));
	}
	__forceinline void _set_w(float W)
	{
		__m128 v = _mm_shuffle_ps(_xyzw,_xyzw, _MM_SHUFFLE(0,2,1,3)); // wx
		v = _mm_move_ss(v, FLT2XMM(W));
		_xyzw = _mm_shuffle_ps(v,v, _MM_SHUFFLE(0,2,1,3));
	}

#else

	__forceinline void set_y(float Y)
	{
		_xyzw = _mm_insert_ps(_xyzw, FLT2XMM(Y), 1 << 4);
	}
	__forceinline void set_z(float Z)
	{
		_xyzw = _mm_insert_ps(_xyzw, FLT2XMM(Z), 2 << 4);
	}
	__forceinline void _set_w(float W)
	{
		_xyzw = _mm_insert_ps(_xyzw, FLT2XMM(W), 3 << 4);
	}

#endif



#ifndef _VEC_SSE4_

	__forceinline void set_w_0()
	{
		_set_w(FLT(0));
	}
	__forceinline void set_w_1()
	{
		_set_w(FLT(1));
	}

#else

	__forceinline void _set_w_i(const int i)
	{
		_xyzw = _mm_castsi128_ps(_mm_insert_epi32(_mm_castps_si128(_xyzw), i, 3));
	}
	__forceinline void set_w_0()
	{
		_set_w_i(0);
	}
	__forceinline void set_w_1()
	{
		_set_w_i(0x3f800000);
	}

#endif


	__forceinline void set_0011()
	{
#if 1
		__m128i i = _mm_cvtsi32_si128(0x3f800000);
		_xyzw = _mm_castsi128_ps(_mm_shuffle_epi32(i, _MM_SHUFFLE(0,0,1,1)));
#else
		__m128i _0011 = _mm_bslli_si128(mm_float4_const_1i(), 8);
		_xyzw = _mm_castsi128_ps(_0011);
#endif
	}
	__forceinline void set_0010()
	{
		__m128i i = _mm_cvtsi32_si128(0x3f800000);
		_xyzw = _mm_castsi128_ps(_mm_shuffle_epi32(i, _MM_SHUFFLE(1,0,1,1)));
	}


	__forceinline static __m128i _vectorcall __init_2i(ULONG I0, ULONG I1)
	{
#ifdef _AMD64_
		ULONG64 v = ((ULONG64)I0) | (((ULONG64)I1) << 32);
		return _mm_cvtsi64_si128(v);
#else
		return _mm_unpacklo_epi32(_mm_cvtsi32_si128(I0), _mm_cvtsi32_si128(I1));
#endif
	}

	__forceinline static __m128i _vectorcall __init_4i(ULONG I0, ULONG I1, ULONG I2, ULONG I3)
	{
		return _mm_unpacklo_epi64(__init_2i(I0, I1), __init_2i(I2, I3));		//return _mm_setr_epi32(I0,I1,I2,I3);
	}


	__forceinline VOID init_i4f(int I0, int I1, int I2, int I3)
	{
		__m128i i = __init_4i(I0,I1,I2,I3);
		_xyzw = _mm_cvtepi32_ps(i);
	}

	__forceinline VOID init_i2f(int I0, int I1)
	{
		__m128i i = __init_2i(I0,I1);
		_xyzw = _mm_cvtepi32_ps(i);
	}

	__forceinline VOID init_p4f(const int *p)
	{
		__m128i i = _mm_loadu_si128( (const __m128i *)p ); //		__m128i i = _mm_lddqu_si128((const __m128i *)p);
		_xyzw = _mm_cvtepi32_ps(i);
	}
	__forceinline VOID init_0()
	{
		_xyzw = _mm_setzero_ps();
	}

	__forceinline void int_to_float()
	{
		_xyzw = _mm_cvtepi32_ps( _mm_castps_si128(_xyzw) );
	}
	__forceinline void float_to_int()
	{
		_xyzw = _mm_castsi128_ps( to_int() );
	}
	__forceinline __m128i to_int() const
	{
		return _mm_cvtps_epi32(_xyzw); // req _MM_SET_ROUNDING_MODE		///return _mm_cvttps_epi32(_xyzw);
	}

	__forceinline void set2_HI(FVec4m v)
	{
		_xyzw = _mm_movelh_ps(_xyzw, v);
	}
	__forceinline void set2_LO_from_HI(FVec4m v)
	{
		_xyzw = _mm_movehl_ps(_xyzw, v);
	}

	__forceinline void load2_HI(void const *p)
	{
		_xyzw = _mm_loadh_pi(_xyzw, (const __m64*)p);
	}

	__forceinline void load2_DUP_F(void const *p)
	{
		_xyzw = _mm_cvtepi32_ps( _mm_castpd_si128( _mm_loaddup_pd((const double*)p) ) );
	}
	__forceinline void load2_DUP(void const *p) // xy xy
	{
		_xyzw = _mm_castpd_ps( _mm_loaddup_pd((const double*)p) );
	}

	__forceinline static __m128 _load2(void const *p)
	{
	//	return _mm_castsi128_ps( _mm_loadl_epi64((const __m128i*)p) );
	//	return _mm_loadl_pi(_mm_undefined_ps(), (const __m64*)p);
		return _mm_castpd_ps(_mm_load_sd((double const*)p));
	}

	__forceinline VOID load2(void const *p)
	{
		_xyzw = _load2(p);
	}
	__forceinline VOID load3(void const *p)
	{
		__m128 xy = _load2(p);
		__m128 z0 = _mm_load_ss( ((const float *)p) + 2 );
		_xyzw = _mm_movelh_ps(xy, z0);
	}
	__forceinline VOID load4(void const *p) // does not need to be aligned on any particular boundary.
	{
		_xyzw = _mm_loadu_ps((const float *)p);
	}
	__forceinline VOID load4a(void const *p)
	{
		_xyzw = _mm_load_ps((const float *)p);
	}

	__forceinline VOID load4_FLT_INT(void const *p, const bool Int)
	{
		if(Int)	init_p4f((const int *)p);
		else	load4((const float *)p);
	}

	__forceinline void store4(void *p) const
	{
		_mm_storeu_ps((float*)p, _xyzw);
	}
	__forceinline void store4a(void *p) const
	{
		_mm_store_ps((float*)p, _xyzw);
	}
	__forceinline void store4nt(void *p) const
	{
		_mm_stream_ps((float*)p, _xyzw);
	}

	__forceinline void store2_HI(void *p) const
	{
		_mm_storeh_pi((__m64*)p, _xyzw);
		////_mm_storeh_pd((double*)p, _mm_castps_pd(_xyzw));
		//__m128 hi = _mm_movehl_ps(_xyzw,_xyzw);
		//_mm_store_sd((double*)p, _mm_castps_pd(hi));
	}
	__forceinline void store2(void *p) const
	{
		//_mm_storel_pi((__m64 *)p, _xyzw);
		_mm_store_sd((double*)p, _mm_castps_pd(_xyzw));
	}
	__forceinline void store3(void *p) const
	{
		store2(p);
		_mm_store_ss(&((float*)p)[2], _mm_movehl_ps(_xyzw,_xyzw));
	}

	__forceinline Vec4m Cross(FVec4m tb) const // v0.y * v1.z - v0.z * v1.y,  v0.z * v1.x - v0.x * v1.z,  v0.x * v1.y - v0.y * v1.x
	{
		__m128 tr = _mm_sub_ps(
			_mm_mul_ps(_mm_shuffle_ps(_xyzw, _xyzw, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(tb, tb, _MM_SHUFFLE(3, 1, 0, 2))),
			_mm_mul_ps(_mm_shuffle_ps(_xyzw, _xyzw, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(tb, tb, _MM_SHUFFLE(3, 0, 2, 1)))
		);
		return tr; // W = 0
	}

	__forceinline __m128 Summ4() const
	{
#if 1
		__m128 t1 = _mm_movehl_ps(_xyzw, _xyzw);
		__m128 t2 = _mm_add_ps(_xyzw, t1);
		__m128 t3 = _mm_shuffle_ps(t2, t2, 1);
		return _mm_add_ss(t2, t3);

		//__m128 shuf = _mm_movehdup_ps(_xyzw); // broadcast elements 3,1 to 2,0
		//__m128 sums = _mm_add_ps(_xyzw, shuf);
		//shuf = _mm_movehl_ps(shuf, sums); // high half -> low half
		//sums = _mm_add_ss(sums, shuf);
		//return sums;
#else
		__m128 v = _mm_hadd_ps(_xyzw,_xyzw);
		v = _mm_hadd_ps(v,v);
		return v;
#endif
	}

	__forceinline Vec4m Dot4(FVec4m ov) const	// return x*v.x + y*v.y + z*v.z;
	{
#ifdef _VEC_SSE4_
		return _mm_dp_ps(_xyzw, ov, 0xFF);
#else
		Vec4m v = _mm_mul_ps(_xyzw, ov);
		return v.Summ4();
#endif
	}

	__forceinline float Dot(FVec4m ov) const	// return x*v.x + y*v.y + z*v.z;
	{
		return _mm_cvtss_f32( Dot4(ov) );
	}


	__forceinline void shift_left(const int bits)
	{
		__m128i i = _mm_castps_si128(_xyzw);
		i = _mm_slli_epi32(i, bits);
		_xyzw = _mm_castsi128_ps(i);
	}

	__forceinline void shift_right_signed(const int bits)
	{
		__m128i i = _mm_srai_epi32(_mm_castps_si128(_xyzw), bits);
		_xyzw = _mm_castsi128_ps(i);
	}
	__forceinline void shift_right(const int bits)
	{
		__m128i i = _mm_srli_epi32(_mm_castps_si128(_xyzw), bits);
		_xyzw = _mm_castsi128_ps(i);
	}


	template<int bytes>
	__forceinline void shift_bytes_right() // trunc: 1234-234-34..
	{
		__m128i i = _mm_srli_si128(_mm_castps_si128(_xyzw), bytes);
		_xyzw = _mm_castsi128_ps(i);
	}

	template<int bytes>
	__forceinline void shift_bytes_left() // insert
	{
		__m128i i = _mm_slli_si128(_mm_castps_si128(_xyzw), bytes);
		_xyzw = _mm_castsi128_ps(i);
	}


	__forceinline __m128 LengthSqrImpl() const
	{
		return Dot4( _xyzw );
		//Vec4m v = _mm_mul_ps(_xyzw,_xyzw);
		//return v.sum4();
		////////return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(_xyzw, _xyzw, 0x71)));
	}

	__forceinline float LengthSqr() const
	{
		return _mm_cvtss_f32( LengthSqrImpl() );
	}

	__forceinline float Length() const
	{
		return _mm_cvtss_f32( _mm_sqrt_ss( LengthSqrImpl() ) );
	}


	__forceinline __m128 NormalizeImpl( __m128 len ) const
	{
		__m128 L = _mm_rsqrt_ss( len );
		L = _mm_shuffle_ps(L,L,0);
		return _mm_mul_ps(_xyzw, L);
	}

	__forceinline __m128 NormalizeImpl() const
	{
		return NormalizeImpl( LengthSqrImpl() );
	}

	__forceinline VOID Normalize()
	{
		_xyzw = NormalizeImpl();
	}


	__forceinline __m128 ReciprocalEst() const // 1/v
	{
		return _mm_rcp_ps(_xyzw);
	}



	__forceinline Vec4m operator + (FVec4m v) const	{ return _mm_add_ps(_xyzw, v); }
	__forceinline Vec4m operator - (FVec4m v) const	{ return _mm_sub_ps(_xyzw, v); }
	__forceinline Vec4m operator * (FVec4m v) const	{ return _mm_mul_ps(_xyzw, v); }
	__forceinline Vec4m operator / (FVec4m v) const	{ return _mm_div_ps(_xyzw, v); }

	__forceinline Vec4m& operator += (FVec4m v)	{ _xyzw = _mm_add_ps(_xyzw, v);  return *this; }
	__forceinline Vec4m& operator -= (FVec4m v)	{ _xyzw = _mm_sub_ps(_xyzw, v);  return *this; }
	__forceinline Vec4m& operator *= (FVec4m v)	{ _xyzw = _mm_mul_ps(_xyzw, v);  return *this; }
	__forceinline Vec4m& operator /= (FVec4m v)	{ _xyzw = _mm_div_ps(_xyzw, v);  return *this; }

	__forceinline Vec4m operator * (float f) const	{ __m128 mm = _mm_set_ps1(f); return _mm_mul_ps(_xyzw, mm); }
	__forceinline Vec4m operator / (float f) const	{ __m128 mm = _mm_set_ps1(f); return _mm_div_ps(_xyzw, mm); }

	__forceinline Vec4m& operator *= (float f)		{ _xyzw = this->operator* (f);  return *this; }
	__forceinline Vec4m& operator /= (float f)		{ _xyzw = this->operator/ (f);  return *this; }

	////__forceinline Vec4m& operator = (float f)		{ _xyzw = FLT2XMM(f);  return *this; }


	__forceinline __m128 NegateImpl() const
	{
		return _mm_sub_ps(_mm_setzero_ps(), _xyzw);
	}

	__forceinline VOID Negate()
	{
		_xyzw = NegateImpl();
	}

	__forceinline void Abs()
	{
		_xyzw = xmm_fabs(_xyzw);
	}


	__forceinline VOID ADD_4i(__m128i i)
	{
		__m128 v = _mm_castsi128_ps(_mm_add_epi32(_mm_castps_si128(_xyzw), i));
		_xyzw = v;
	}
	__forceinline VOID SUB_4i(__m128i i)
	{
		__m128 v = _mm_castsi128_ps(_mm_sub_epi32(_mm_castps_si128(_xyzw), i));
		_xyzw = v;
	}

	__forceinline VOID XOR_4(__m128 i)
	{
		_xyzw = _mm_xor_ps(_xyzw, i);
	}
	__forceinline VOID OR_4(__m128 i)
	{
		_xyzw = _mm_or_ps(_xyzw, i);
	}

	__forceinline VOID swap_YZ()
	{
		_xyzw = _mm_shuffle_ps(_xyzw,_xyzw, _MM_SHUFFLE(0,2,1,3));
	}

	__forceinline bool is_empty() const
	{
#ifdef _VEC_SSE4_
		return _mm_testz_si128(_mm_castps_si128(_xyzw), _mm_castps_si128(_xyzw));
#else
		return _mm_movemask_epi8(_mm_cmpeq_epi32(_mm_castps_si128(_xyzw), _mm_setzero_si128())) == 0xFFFF;
#endif
	}

	__forceinline __m128i cast_int() const
	{
		return _mm_castps_si128(_xyzw);
	}
	__forceinline __m128d cast_double() const
	{
		return _mm_castps_pd(_xyzw);
	}

	static FLOAT __vectorcall VectorAngle(FVec4m a, FVec4m b, FLOAT * B_Len = NULL)
	{
		FLOAT lB = b.Length();
		FLOAT lA = a.Length();

		if(B_Len)
			*B_Len = lB;

		FLOAT _Fmin = FLT(0.001f);
		if(	lB < _Fmin ||
			lA < _Fmin )
			return FLT(180);

		return mm_fabs(mm_acos(a.Dot(b) / (lA * lB))) * FLT( g_RAD2DEG );
	}

};



template<int x, int y, int z, int w> inline Vec4m mix0011(Vec4m a, Vec4m b) { return _mm_shuffle_ps(a, b, _MM_SHUFFLE(w, z, y, x)); }
template<int x, int y, int z, int w> Vec4m mix0101(Vec4m a, Vec4m b);  //!< Default configurations unimplemented

template<> inline Vec4m mix0011<0, 1, 0, 1>(Vec4m a, Vec4m b) { return _mm_movelh_ps(a, b); }
template<> inline Vec4m mix0011<2, 3, 2, 3>(Vec4m a, Vec4m b) { return _mm_movehl_ps(b, a); }
template<> inline Vec4m mix0101<0, 0, 1, 1>(Vec4m a, Vec4m b) { return _mm_unpacklo_ps(a, b); }
template<> inline Vec4m mix0101<2, 2, 3, 3>(Vec4m a, Vec4m b) { return _mm_unpackhi_ps(a, b); }


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


struct Matrix4x4m;
typedef const Matrix4x4m FMatrix4x4m;


typedef struct Matrix4x4
{
public:
	union
	{
		__m128 r[4];

		struct
		{
			__m128 m1, m2, m3, m4;
		};

		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};

		// 00 01 02 03
		// 10 11 12 13
		// 20 21 22 23
		// 30 31 32 33
		float m[4][4];

		// 00 01 02 03
		// 04 05 06 07
		// 08 09 10 11
		// 12 13 14 15
		float mm[16];
	};
	inline Vec4m xmm(const ULONG i) const
	{
		Vec4m r;
		r.load4(m[i]);
		return r;
	}
	inline void SetIdentity()
	{
		_11=1; _12=0; _13=0; _14=0;
		_21=0; _22=1; _23=0; _24=0;
		_31=0; _32=0; _33=1; _34=0;
		_41=0; _42=0; _43=0; _44=1;
	}
	inline void SetR2L()
	{
		_11=1; _12=0; _13=0; _14=0;
		_21=0; _22=0; _23=1; _24=0;
		_31=0; _32=1; _33=0; _34=0;
		_41=0; _42=0; _43=0; _44=1;
	}
	inline float* operator [] (const ULONG index)
	{
		return &mm[index*4];
	}
	inline const float* operator [] (const ULONG index) const
	{
		return &mm[index*4];
	}
	inline float* Base()
	{
		return &_11;
	}
	
	Matrix4x4 _vectorcall operator*(const Matrix4x4& M2) const;

	//inline TPlane TransformPlane(const TPlane &P) const
	//{
	//	TPlane Result;
	//	Result.n.x = P.n.x * m[0][0] + P.n.y * m[1][0] + P.n.z * m[2][0] + P.d * m[3][0];
	//	Result.n.y = P.n.x * m[0][1] + P.n.y * m[1][1] + P.n.z * m[2][1] + P.d * m[3][1];
	//	Result.n.z = P.n.x * m[0][2] + P.n.y * m[1][2] + P.n.z * m[2][2] + P.d * m[3][2];
	//	Result.d   = P.n.x * m[0][3] + P.n.y * m[1][3] + P.n.z * m[2][3] + P.d * m[3][3];
	//	return Result;
	//}

/*	static __forceinline __m128 __vectorcall __XMVectorMultiplyAdd(__m128 V1, __m128 V2, __m128 V3)
	{
		//XMVECTOR Result;
		//Result.vector4_f32[0] = V1.vector4_f32[0] * V2.vector4_f32[0] + V3.vector4_f32[0];
		//Result.vector4_f32[1] = V1.vector4_f32[1] * V2.vector4_f32[1] + V3.vector4_f32[1];
		//Result.vector4_f32[2] = V1.vector4_f32[2] * V2.vector4_f32[2] + V3.vector4_f32[2];
		//Result.vector4_f32[3] = V1.vector4_f32[3] * V2.vector4_f32[3] + V3.vector4_f32[3];
		//return Result;
		__m128 vResult = _mm_mul_ps( V1, V2 );
		return _mm_add_ps(vResult, V3);
	}
	inline Vector TransformVector(const Vector &v) const
	{
		__m128 V = _mm_loadu_ps( reinterpret_cast<const float*>( &v.x ) );

		__m128 Z = XM_PERMUTE_PS( V, _MM_SHUFFLE(2, 2, 2, 2) ); // f[0]=f[1]=f[2]=f[3] = V[2]
		__m128 Result = __XMVectorMultiplyAdd(Z, r[2], r[3]);
		__m128 Y = XM_PERMUTE_PS( V, _MM_SHUFFLE(1, 1, 1, 1) );
		Result = __XMVectorMultiplyAdd(Y, r[1], Result);
		__m128 X = XM_PERMUTE_PS( V, _MM_SHUFFLE(0, 0, 0, 0) );
		Result = __XMVectorMultiplyAdd(X, r[0], Result);

		t_m128 r;
		_mm_store_ps(r.f,Result);

		return *(Vector *)&r;
		//return TransformPlane(TPlane(V,1.f)).n;
	}*/
	inline Vector &GetOrigin()
	{
		return *(Vector *) &_41;//  (m[3][0], m[3][1], m[3][2]);
	}
	inline Vector &GetAxis(const ULONG index) // xyz: right/forward/up
	{
		return *(Vector *) m[index];
	}
	inline Vector &GetAxis(const ULONG index) const
	{
		return *(Vector *) m[index];
	}
	inline TPlane &GetPlane(const ULONG index)
	{
		return *(TPlane *) m[index];
	}

	inline void Angle(QAngle &angles, Vector &position)
	{
		struct
		{
			float pitch, yaw, roll;
		}
		s, c;

		mm_sincos(angles.pitch, &s.pitch, &c.pitch);
		mm_sincos(angles.yaw,   &s.yaw,   &c.yaw);
		mm_sincos(angles.roll,  &s.roll,  &c.roll);

		FLOAT m1 = FLT(-1.0);

		// fwd
		m[0][0] = (c.pitch * c.yaw);
		m[1][0] = (c.pitch * s.yaw);
		m[2][0] = s.pitch * m1;

		// right
		m[0][1] = (m1 * s.roll * s.pitch * c.yaw + c.roll * s.yaw);
		m[1][1] = (m1 * s.roll * s.pitch * s.yaw - c.roll * c.yaw);
		m[2][1] = (m1 * s.roll * c.pitch);
		//m[0][1] = sr * sp * cy + cr * -sy;
		//m[1][1] = sr * sp * sy + cr * cy;
		//m[2][1] = sr * cp;

		// up
		m[0][2] = (c.roll * s.pitch * c.yaw + s.roll * s.yaw);
		m[1][2] = (c.roll * s.pitch * s.yaw - s.roll * c.yaw);
		m[2][2] = (c.roll * c.pitch);

		m[0][3] = position.x;
		m[1][3] = position.y;
		m[2][3] = position.z;

		*(PULONG)&m[3][0] = 0;
		*(PULONG)&m[3][1] = 0;
		*(PULONG)&m[3][2] = 0;
		*(PULONG)&m[3][3] = 0x3f800000; // 1
	}

} *PMatrix4x4;



__forceinline
void MatrixPrefetch(Matrix4x4 &m)
{
	_mm_prefetch( (PCHAR)&m._11, _MM_HINT_NTA );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

// Fix-up for (2nd+) Matrix4x4m parameters to pass by reference
typedef const Matrix4x4m& CMatrix4x4m;


__declspec(align(16))
struct Matrix4x4m
{
	Vec4m r[4];

	Matrix4x4m() = default;
	Matrix4x4m(const Matrix4x4m&) = default;

#if defined(_MSC_VER) && (_MSC_FULL_VER < 191426431)
	Matrix4x4m& operator= (const Matrix4x4m& M) noexcept { r[0] = M.r[0]; r[1] = M.r[1]; r[2] = M.r[2]; r[3] = M.r[3]; return *this; }
#else
	Matrix4x4m& operator=(const Matrix4x4m&) = default;

	Matrix4x4m(Matrix4x4m&&) = default;
	Matrix4x4m& operator=(Matrix4x4m&&) = default;
#endif

	__forceinline Matrix4x4m(FVec4m R0, FVec4m R1, FVec4m R2, FVec4m R3) : r{ R0,R1,R2,R3 } {}
	
	//__forceinline Matrix4x4m
	//(
	//	float m00, float m01, float m02, float m03,
	//	float m10, float m11, float m12, float m13,
	//	float m20, float m21, float m22, float m23,
	//	float m30, float m31, float m32, float m33
	//)
	//{
	//	r[0].init(m00, m01, m02, m03);
	//	r[1].init(m10, m11, m12, m13);
	//	r[2].init(m20, m21, m22, m23);
	//	r[3].init(m30, m31, m32, m33);
	//}

	explicit Matrix4x4m(_In_reads_(16) const float *);

	__forceinline Matrix4x4m _vectorcall operator+ () const
	{
		return *this;
	}

	Matrix4x4m _vectorcall operator- () const
	{
		Matrix4x4m R;
		R.r[0] = r[0].NegateImpl();
		R.r[1] = r[1].NegateImpl();
		R.r[2] = r[2].NegateImpl();
		R.r[3] = r[3].NegateImpl();
		return R;
	}

	Matrix4x4m& _vectorcall operator+= (FMatrix4x4m M)
	{
		r[0] += M.r[0];
		r[1] += M.r[1];
		r[2] += M.r[2];
		r[3] += M.r[3];
		return *this;
	}
	Matrix4x4m& _vectorcall operator-= (FMatrix4x4m M)
	{
		r[0] -= M.r[0];
		r[1] -= M.r[1];
		r[2] -= M.r[2];
		r[3] -= M.r[3];
		return *this;
	}

	Matrix4x4m& _vectorcall operator*= (float S)
	{
		Vec4m v(S);
		r[0] *= v;
		r[1] *= v;
		r[2] *= v;
		r[3] *= v;
		return *this;
	}
	Matrix4x4m& _vectorcall operator/= (float S)
	{
		Vec4m v(S);
		r[0] /= v;
		r[1] /= v;
		r[2] /= v;
		r[3] /= v;
		return *this;
	}

	Matrix4x4m _vectorcall operator+ (FMatrix4x4m M) const
	{
		Matrix4x4m R;
		R.r[0] = r[0] + M.r[0];
		R.r[1] = r[1] + M.r[1];
		R.r[2] = r[2] + M.r[2];
		R.r[3] = r[3] + M.r[3];
		return R;
	}
	Matrix4x4m _vectorcall operator- (FMatrix4x4m M) const
	{
		Matrix4x4m R;
		R.r[0] = r[0] - M.r[0];
		R.r[1] = r[1] - M.r[1];
		R.r[2] = r[2] - M.r[2];
		R.r[3] = r[3] - M.r[3];
		return R;
	}
	Matrix4x4m _vectorcall operator* (float S) const
	{
		Vec4m v(S);
		Matrix4x4m R;
		R.r[0] = r[0] * v;
		R.r[1] = r[1] * v;
		R.r[2] = r[2] * v;
		R.r[3] = r[3] * v;
		return R;
	}
	Matrix4x4m _vectorcall operator/ (float S) const
	{
		Vec4m v(S);
		Matrix4x4m R;
		R.r[0] = r[0] / v;
		R.r[1] = r[1] / v;
		R.r[2] = r[2] / v;
		R.r[3] = r[3] / v;
		return R;
	}

	Matrix4x4m _vectorcall operator* (FMatrix4x4m M) const;
	Matrix4x4m& _vectorcall operator*= (FMatrix4x4m M);

	Matrix4x4m _vectorcall operator* (const Matrix4x4& M) const;
	Matrix4x4m& _vectorcall operator*= (const Matrix4x4& M);

	friend Matrix4x4m _vectorcall operator* (float S, FMatrix4x4m M)
	{
		Matrix4x4m R;
		R.r[0] = M.r[0] * S;
		R.r[1] = M.r[1] * S;
		R.r[2] = M.r[2] * S;
		R.r[3] = M.r[3] * S;
		return R;
	}

	__forceinline void Store(Matrix4x4* p) const
	{
		r[0].store4(&p->_11);
		r[1].store4(&p->_21);
		r[2].store4(&p->_31);
		r[3].store4(&p->_41);
	}
	__forceinline void Load(const Matrix4x4* p)
	{
		r[0].load4(&p->_11);
		r[1].load4(&p->_21);
		r[2].load4(&p->_31);
		r[3].load4(&p->_41);
	}
	__forceinline Matrix4x4m(const Matrix4x4 &M)
	{
		Load(&M);
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////


__forceinline Matrix4x4m::Matrix4x4m(_In_reads_(16) const float * pArray)
{
	r[0].load4(pArray);
	r[1].load4(pArray+4);
	r[2].load4(pArray+8);
	r[3].load4(pArray+12);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////


template<class MATR>
inline MATR _vectorcall MatrixMultiply(const MATR & M1, const MATR & M2)//inline Matrix4x4m _vectorcall MatrixMultiply(FMatrix4x4m M1, CMatrix4x4m M2)
{
    //Matrix4x4m mResult;
    //// Cache the invariants in registers
    //float x = M1.m[0][0];
    //float y = M1.m[0][1];
    //float z = M1.m[0][2];
    //float w = M1.m[0][3];
    //// Perform the operation on the first row
    //mResult.m[0][0] = (M2.m[0][0]*x)+(M2.m[1][0]*y)+(M2.m[2][0]*z)+(M2.m[3][0]*w);
    //mResult.m[0][1] = (M2.m[0][1]*x)+(M2.m[1][1]*y)+(M2.m[2][1]*z)+(M2.m[3][1]*w);
    //mResult.m[0][2] = (M2.m[0][2]*x)+(M2.m[1][2]*y)+(M2.m[2][2]*z)+(M2.m[3][2]*w);
    //mResult.m[0][3] = (M2.m[0][3]*x)+(M2.m[1][3]*y)+(M2.m[2][3]*z)+(M2.m[3][3]*w);
    //// Repeat for all the other rows
    //x = M1.m[1][0];
    //y = M1.m[1][1];
    //z = M1.m[1][2];
    //w = M1.m[1][3];
    //mResult.m[1][0] = (M2.m[0][0]*x)+(M2.m[1][0]*y)+(M2.m[2][0]*z)+(M2.m[3][0]*w);
    //mResult.m[1][1] = (M2.m[0][1]*x)+(M2.m[1][1]*y)+(M2.m[2][1]*z)+(M2.m[3][1]*w);
    //mResult.m[1][2] = (M2.m[0][2]*x)+(M2.m[1][2]*y)+(M2.m[2][2]*z)+(M2.m[3][2]*w);
    //mResult.m[1][3] = (M2.m[0][3]*x)+(M2.m[1][3]*y)+(M2.m[2][3]*z)+(M2.m[3][3]*w);
    //x = M1.m[2][0];
    //y = M1.m[2][1];
    //z = M1.m[2][2];
    //w = M1.m[2][3];
    //mResult.m[2][0] = (M2.m[0][0]*x)+(M2.m[1][0]*y)+(M2.m[2][0]*z)+(M2.m[3][0]*w);
    //mResult.m[2][1] = (M2.m[0][1]*x)+(M2.m[1][1]*y)+(M2.m[2][1]*z)+(M2.m[3][1]*w);
    //mResult.m[2][2] = (M2.m[0][2]*x)+(M2.m[1][2]*y)+(M2.m[2][2]*z)+(M2.m[3][2]*w);
    //mResult.m[2][3] = (M2.m[0][3]*x)+(M2.m[1][3]*y)+(M2.m[2][3]*z)+(M2.m[3][3]*w);
    //x = M1.m[3][0];
    //y = M1.m[3][1];
    //z = M1.m[3][2];
    //w = M1.m[3][3];
    //mResult.m[3][0] = (M2.m[0][0]*x)+(M2.m[1][0]*y)+(M2.m[2][0]*z)+(M2.m[3][0]*w);
    //mResult.m[3][1] = (M2.m[0][1]*x)+(M2.m[1][1]*y)+(M2.m[2][1]*z)+(M2.m[3][1]*w);
    //mResult.m[3][2] = (M2.m[0][2]*x)+(M2.m[1][2]*y)+(M2.m[2][2]*z)+(M2.m[3][2]*w);
    //mResult.m[3][3] = (M2.m[0][3]*x)+(M2.m[1][3]*y)+(M2.m[2][3]*z)+(M2.m[3][3]*w);
    //return mResult;

//	Matrix4x4m mResult;
	MATR mResult;
    // Splat the component X,Y,Z then W
#if defined(_XM_AVX_INTRINSICS_)
    Vec4m vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 0);
    Vec4m vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 1);
    Vec4m vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 2);
    Vec4m vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 3);
#else
    // Use vW to hold the original row
    Vec4m vW = M1.r[0];
    Vec4m vX = XM_PERMUTE_PS(vW,_MM_SHUFFLE(0,0,0,0));
    Vec4m vY = XM_PERMUTE_PS(vW,_MM_SHUFFLE(1,1,1,1));
    Vec4m vZ = XM_PERMUTE_PS(vW,_MM_SHUFFLE(2,2,2,2));
    vW = XM_PERMUTE_PS(vW,_MM_SHUFFLE(3,3,3,3));
#endif
    // Perform the operation on the first row
    vX = _mm_mul_ps(vX,M2.r[0]);
    vY = _mm_mul_ps(vY,M2.r[1]);
    vZ = _mm_mul_ps(vZ,M2.r[2]);
    vW = _mm_mul_ps(vW,M2.r[3]);
    // Perform a binary add to reduce cumulative errors
    vX = _mm_add_ps(vX,vZ);
    vY = _mm_add_ps(vY,vW);
    vX = _mm_add_ps(vX,vY);
    mResult.r[0] = vX;
    // Repeat for the other 3 rows
#if defined(_XM_AVX_INTRINSICS_)
    vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 0);
    vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 1);
    vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 2);
    vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 3);
#else
    vW = M1.r[1];
    vX = XM_PERMUTE_PS(vW,_MM_SHUFFLE(0,0,0,0));
    vY = XM_PERMUTE_PS(vW,_MM_SHUFFLE(1,1,1,1));
    vZ = XM_PERMUTE_PS(vW,_MM_SHUFFLE(2,2,2,2));
    vW = XM_PERMUTE_PS(vW,_MM_SHUFFLE(3,3,3,3));
#endif
    vX = _mm_mul_ps(vX,M2.r[0]);
    vY = _mm_mul_ps(vY,M2.r[1]);
    vZ = _mm_mul_ps(vZ,M2.r[2]);
    vW = _mm_mul_ps(vW,M2.r[3]);
    vX = _mm_add_ps(vX,vZ);
    vY = _mm_add_ps(vY,vW);
    vX = _mm_add_ps(vX,vY);
    mResult.r[1] = vX;
#if defined(_XM_AVX_INTRINSICS_)
    vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 0);
    vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 1);
    vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 2);
    vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 3);
#else
    vW = M1.r[2];
    vX = XM_PERMUTE_PS(vW,_MM_SHUFFLE(0,0,0,0));
    vY = XM_PERMUTE_PS(vW,_MM_SHUFFLE(1,1,1,1));
    vZ = XM_PERMUTE_PS(vW,_MM_SHUFFLE(2,2,2,2));
    vW = XM_PERMUTE_PS(vW,_MM_SHUFFLE(3,3,3,3));
#endif
    vX = _mm_mul_ps(vX,M2.r[0]);
    vY = _mm_mul_ps(vY,M2.r[1]);
    vZ = _mm_mul_ps(vZ,M2.r[2]);
    vW = _mm_mul_ps(vW,M2.r[3]);
    vX = _mm_add_ps(vX,vZ);
    vY = _mm_add_ps(vY,vW);
    vX = _mm_add_ps(vX,vY);
    mResult.r[2] = vX;
#if defined(_XM_AVX_INTRINSICS_)
    vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 0);
    vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 1);
    vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 2);
    vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 3);
#else
    vW = M1.r[3];
    vX = XM_PERMUTE_PS(vW,_MM_SHUFFLE(0,0,0,0));
    vY = XM_PERMUTE_PS(vW,_MM_SHUFFLE(1,1,1,1));
    vZ = XM_PERMUTE_PS(vW,_MM_SHUFFLE(2,2,2,2));
    vW = XM_PERMUTE_PS(vW,_MM_SHUFFLE(3,3,3,3));
#endif
    vX = _mm_mul_ps(vX,M2.r[0]);
    vY = _mm_mul_ps(vY,M2.r[1]);
    vZ = _mm_mul_ps(vZ,M2.r[2]);
    vW = _mm_mul_ps(vW,M2.r[3]);
    vX = _mm_add_ps(vX,vZ);
    vY = _mm_add_ps(vY,vW);
    vX = _mm_add_ps(vX,vY);
    mResult.r[3] = vX;
    return mResult;
}


inline Matrix4x4m& _vectorcall Matrix4x4m::operator*= (FMatrix4x4m M)
{
	*this = MatrixMultiply(*this, M);
	return *this;
}

inline Matrix4x4m _vectorcall Matrix4x4m::operator* (FMatrix4x4m M) const
{
	return MatrixMultiply(*this, M);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////


__forceinline Matrix4x4m& _vectorcall Matrix4x4m::operator*= (const Matrix4x4 &M)
{
	Matrix4x4m M2 = M;
	*this = MatrixMultiply(*this, M2);
	return *this;
}

__forceinline Matrix4x4m _vectorcall Matrix4x4m::operator* (const Matrix4x4 &M) const
{
	Matrix4x4m M2 = M;
	return MatrixMultiply(*this, M2);
}


__forceinline Matrix4x4 _vectorcall Matrix4x4::operator*(const Matrix4x4& M2) const
{
	return MatrixMultiply(*this, M2);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////


__forceinline VOID MatrixTranspose(const Matrix4x4 & M, Matrix4x4 & mResult)
{
	Matrix4x4m tr = MatrixTranspose(M);
	tr.Store(&mResult);
}

__forceinline VOID _vectorcall MatrixTranspose(FMatrix4x4m M, Matrix4x4 & mResult)
{
	Matrix4x4m tr = MatrixTranspose(M);
	tr.Store(&mResult);
}


__forceinline VOID MatrixTransposeU(const Matrix4x4 &in, Matrix4x4 &out)
{
	MatrixInitTranspose(&in._11, &in._41, &out._11);
}


Vec4m __vectorcall TransformTransposeVec4(const Vec4m In, const Matrix4x4 &MatrixTranspose);

__forceinline
void TransformTransposeVec4(_In_reads_(4) const float *In4, const Matrix4x4 &Matrix, float *Out4)
{
	Vec4m v;
	v.load4(In4);
	Vec4m r = TransformTransposeVec4(v, Matrix);
	r.store4(Out4);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma warning(default: 4244)

#pragma pack(pop)


#ifdef _GUITEST_
#	pragma optimize("gst", off)
#else
#	pragma optimize("", on)
#endif


#endif