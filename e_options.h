#pragma once

//#pragma warning(disable: 4996)
//#pragma warning(disable: 4005) // macro redefinition
//#pragma warning(disable: 4244) // conversion from 'INT' to 'FLOAT', possible loss of data
//#pragma warning(disable: 4305) // 'argument' : truncation from 'double' to 'float'
//#pragma warning(disable: 4018) // '>' : signed/unsigned mismatch
//#pragma warning(disable: 4164) // 'memset' : intrinsic function not declared
//#pragma warning(disable: 4091) // 'typedef ': ignored on left of 
//#pragma warning(disable: 4311) // 'type cast': pointer truncation

#ifndef _NTSYSTEM_
#define	_NTSYSTEM_
#endif

#ifndef _WIN32_WINNT
#define	_WIN32_WINNT 0x0602
#endif

#ifndef WINVER
#define	WINVER 0x0602
#endif

#define _WOW64

#define VC_EXTRALEAN
#define _INC_MEMORY
#define _INC_STRING
#define _INC_STDDEF
#define _CRT_MEMORY_DEFINED // vcruntime_string.h
#define __STRALIGN_H_
#define _WINDOWS_SECURE_NO_DEPRECATE
//#define _SYS_GUID_OPERATORS_
#define _CPP_MEMCHR_DEFINED

#ifndef _NO_CRT_STDIO_INLINE
#define _NO_CRT_STDIO_INLINE
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#undef __STDC_WANT_SECURE_LIB__
#define __STDC_WANT_SECURE_LIB__ 0
#define _CRT_USE_WINAPI_FAMILY_DESKTOP_APP

#ifndef NULL
#define NULL nullptr
#endif

#if !defined(DRIVER) && !defined(_DRIVER)
	#include <intrin.h>
#else
	#include <i/intrin.h>
#endif


#undef memcpy
#undef memset
#undef memcmp

int _memcmp(const void *pvDst, const void *pvSrc, size_t cb);
void _memmove(void * _Dst, const void * _Src, size_t _Size);

#define memcpy(x,y,z) __movsb( (unsigned char *)(x), (const unsigned char *)(y), (z) )
#define memset(x,y,z) __stosb( (unsigned char *)(x), (unsigned char)(y), (z) )
#define memcmp _memcmp


__forceinline constexpr void _memcpy(void *dst, const void *src, const size_t count)
{
	if(count == 1)
		*(unsigned char *)dst = *(const unsigned char *)src;
	else
	if(count == 4)
		*(unsigned long *)dst = *(const unsigned long *)src;
	else
	if(count == 8)
		*(unsigned __int64 *)dst = *(const unsigned __int64 *)src;
	else
#ifdef _AMD64_
	if(count == 16)
	{
	//	__m128 t = _mm_loadu_ps((const float *)src);
	//	_mm_storeu_ps((float *)dst, t);
		unsigned __int64 *a = (unsigned __int64 *)dst;
		const unsigned __int64 *b = (const unsigned __int64 *)src;
		a[0] = b[0];
		a[1] = b[1];
	}
	else
	if(!(count & 7))
		__movsq( (unsigned __int64 *)dst, (const unsigned __int64 *)src, (count >> 3) );
	else
#endif

	if(!(count & 3))
		__movsd( (unsigned long *)dst, (const unsigned long *)src, (count >> 2) );
	else
		__movsb( (unsigned char *)dst, (const unsigned char *)src, count );
}


__forceinline constexpr void memset0(void *dst, const size_t count)
{
	if(count == 1)
		*(unsigned char *)dst = 0;
	else
	if(count == 2)
		*(unsigned short *)dst = 0;
	else
	if(count == 4)
		*(unsigned long *)dst = 0;
	else

	if(count == 8)
		*(unsigned __int64 *)dst = 0;
	else

#ifdef _AMD64_
	if(count == 16)
	{
		((void**)dst) [0] = 0;
		((void**)dst) [1] = 0;		//_mm_storeu_si128( (__m128i *)dst, _mm_setzero_si128() );
	}
	else
	//if(count == 32)
	//{
	//	__m128i i = _mm_setzero_si128();
	//	_mm_storeu_si128( (__m128i *)dst, i );
	//	_mm_storeu_si128( (__m128i *)dst + 1, i );
	//}
	//else
	if(!(count & 7))
		__stosq( (unsigned __int64 *)dst, 0, count >> 3 );
	else
#endif

	if(!(count & 3))
		__stosd( (unsigned long *)dst, 0, count >> 2 );
	else
		__stosb( (unsigned char *)dst, 0, count );
}


#define MEMSET0(x) memset0(x,sizeof(x));



#if !defined(DRIVER) && !defined(_DRIVER)

#include <windows.h>

#ifndef _NONTDLL
#include <nt/ntdll.h>
#endif

#else

extern "C"
{
#include <ntifs.h>
#include <ntddk.h>
#include <wdm.h>
#include <ntimage.h>
#include <portcls.h>
}

#endif



#undef ZeroMemory
#define ZeroMemory memset0

#undef RtlZeroMemory
#define RtlZeroMemory memset0

#define PROTOTYPE __declspec(novtable)



#define ARRAY_COUNT( Array ) ( sizeof(Array) / sizeof((Array)[0]) )

#define STRUCT_OFFSET( Struc, Member ) ( (ULONG_PTR)&((Struc*)0)->Member )



#ifndef SAFE_DELETE
#	define SAFE_DELETE(p)			{ if(p) { delete (p);		(p)=0; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#	define SAFE_DELETE_ARRAY(p)		{ if(p) { delete[] (p);		(p)=0; } }
#endif

#ifndef SAFE_RELEASE
#	define SAFE_RELEASE(p)			{ if(p) { (p)->Release();	(p)=0; } }
#endif

#ifndef SAFE_RELEASE_ARG
#	define SAFE_RELEASE_ARG(p,a)	{ if(p) { (p)->Release(a);	(p)=0; } }
#endif

#ifndef SAFE_RELEASE_ARRAY
#	define SAFE_RELEASE_ARRAY(p,c)	{ for(int i = 0; i < (c); i++) SAFE_RELEASE(p[i]); }
#endif


#define TYPEOF(_FNP) 	decltype(_FNP)


struct optfk
{
	optfk() { }
	inline ~optfk() { volatile char c = 0; }
};


//-------------------------------------------------------


//#pragma warning (disable: 4577)
//#include <type_traits>

template< class T >
	__forceinline T Abs( T A )
	{
		//static_assert( !std::is_same<T, float>() , "FLOAT");
		return (A>=(T)0) ? A : -A;
	}

#if !defined(DRIVER) && !defined(_DRIVER)
	__forceinline FLOAT __vectorcall Abs( FLOAT F )
	{
		__m128i i = _mm_castps_si128(_mm_set_ss(F));
		i = _mm_slli_epi32(i, 1);
		i = _mm_srli_epi32(i, 1);
		return _mm_cvtss_f32(_mm_castsi128_ps(i));
	}
#endif

template< class T, class R >
	__forceinline R Abs2( T A )
	{
		return (R)( (A>=(T)0) ? A : -A );
	}


template< class T >
	__forceinline T Min(T A, T B)
	{
		return (A<=B) ? A : B;
	}
template< class T >
	__forceinline T Max(T A, T B)
	{
		return (A>=B) ? A : B;
	}
template< class T >
	__forceinline T Max3(T A, T B, T C)
	{
		return Max( Max( A, B ), C );
	}
template< class T >
	__forceinline T Min3(T A, T B, T C)
	{
		return Min( Min( A, B ), C );
	}

template< class T >
	inline T Clamp(T X, T lower, T upper)
	{
		return (X<=lower) ? lower : ((X<=upper) ? X : upper);
	}

template< class T >
	__forceinline T AlignSize( const T Size, const SIZE_T Alignment )
	{
		SIZE_T R = (Size % Alignment);
		if(R)
			return Size + (T)(Alignment - R);
		return Size;
	}

template< class T >
	__forceinline T Align( const T Ptr, const SIZE_T Alignment )
	{
		return (T)(((ULONG_PTR)Ptr + (Alignment-1)) & ~(Alignment-1));
	}

template< class T >
	__forceinline T Align( LPCVOID Ptr, const SIZE_T Alignment )
	{
		return (T)(((ULONG_PTR)Ptr + (Alignment-1)) & ~(Alignment-1));
	}


template <class T> 
	__forceinline void Swap(T& A, T& B)
	{
		const T Temp = A;
		A = B;
		B = Temp;
	}


//------------------------------------------------------------------------


inline ULONG CountLeadingZeros(ULONG x)
{
	ULONG firstBit;
	if( _BitScanReverse(&firstBit, x) )
		return 31 - firstBit;
	return 32;
}

inline ULONG CountBits(ULONG x)
{
	ULONG firstBit;
	if( _BitScanReverse(&firstBit, x) )
		return firstBit + 1;
	return 0;
}

inline ULONG CountTrailingZeros(ULONG elem)
{
	ULONG out;
	if( _BitScanForward(&out, elem) )
		return out;
	return 32;
}

#ifdef _AMD64_

inline ULONG CountTrailingZeros64(ULONG64 i) // 1 << x
{
	ULONG out;
	if( _BitScanForward64(&out, i) )
		return out;
	return 64;
}

inline ULONG CountLeadingZeros64(ULONG64 x) // >>>
{
	ULONG firstBit;
	if( _BitScanReverse64(&firstBit, x) )
		return 63 - firstBit;
	return 64;
}

#endif


#ifndef _NONTDLL
#include "e_options2.h"
#endif

