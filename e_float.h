#pragma once


#include <emmintrin.h>


__forceinline static void* ptr_f_no_opt(const void *p) // use with care!
{
#if defined(_WIN64) || defined(_AMD64_)
	__m128i i = _mm_cvtsi64_si128((__int64)p);
	return (void*)_mm_cvtsi128_si64(i);
#else
	__m128i i = _mm_cvtsi32_si128((int)p);
	return (void*)_mm_cvtsi128_si32(i);
#endif
}


struct t_fltdata
{
	union
	{
		float _f;
		volatile float _ff;
		int i;
	};
	__forceinline static float __vectorcall flt(float f) noexcept
	{
		t_fltdata d;
		d._ff = f;
		return d._f;
	}
	__forceinline static void __vectorcall flt_p(float *dst, float f) noexcept
	{
		t_fltdata *p = (t_fltdata*) ptr_f_no_opt(dst);
		p->_f = f;

		//t_fltdata d;
		//d._ff = f;
		//*(int*)dst = d.i;
	}
};

#define FLTC(v) t_fltdata::flt(v)
#define FLTCp(p_FLT,v) t_fltdata::flt_p(p_FLT,v)


#ifndef FLT
#define FLT(v) t_fltdata::flt(v)
#endif



#ifndef FLT2XMM
#ifdef _AMD64_
#define FLT2XMM(x)	_mm_set_ss(x)
#else
#define FLT2XMM(x)	_mm_load_ss(&x)
#endif
#endif


#if defined(_WIN64) || defined(_AMD64_)

__forceinline int __vectorcall mm_float_cast(float f)
{
	return _mm_cvtsi128_si32(_mm_castps_si128(FLT2XMM(f)));
}

__forceinline float __vectorcall mm_float_set(int x)
{
	return _mm_cvtss_f32(_mm_castsi128_ps(_mm_cvtsi32_si128(x)));
}

#else

__forceinline int __vectorcall mm_float_cast(float f)
{
	t_fltdata r;
	r._f = f;
	return r.i; // *(int*)&f;
}

__forceinline float __vectorcall mm_float_set(int x)
{
	t_fltdata r;
	r.i = x;
	return r._f; //*(float*)&x;
}

#endif



