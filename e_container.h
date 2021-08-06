#pragma once

#include "e_options.h"
#include "e_libstr.h"

#include <e_xxptr.h>


ULONG mm_rand_U();
ULONG_PTR mm_rand_UU();



#ifndef DRIVER
//c:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Tools\MSVC\14.13.26128\include\xtr1common
#include <xtr1common>

template<class T>
class XVAR
{
private:
	using U_PTR = typename std::conditional<sizeof(T) == 4, ULONG, ULONG64>::type;
	U_PTR x;
	U_PTR y;

	__forceinline VOID _put(T v)
	{
		ULONG_PTR r = mm_rand_UU();
		U_PTR xx = x + (U_PTR)r;
		x = xx;
		y = xx ^ U_PTR(v);
	}
	__forceinline const T _get() const
	{
		return T( x ^ y );
	}
	__declspec(property(get = _get, put = _put))
		T _value;

public:
	__forceinline XVAR()			{ set_null(); }
	__forceinline XVAR(const T &v)	{ _put(v); }
	__forceinline XVAR(const XVAR &v)	{ x = v.y; y = v.x; }

	__forceinline VOID set_null()	{	x = y;	}

	__forceinline bool is_null() const 	{ return x == y;	}
	__forceinline operator bool() const { return x != y;	}

	__forceinline operator T () const			{ return _value;	}
	__forceinline T value() const				{ return _value;	}
	__forceinline T operator -> () const		{ return _value;	}
	__forceinline const T operator () () const	{ return _value;	}

	//__forceinline XVAR& operator = (T v)
	//{
	//	_value = v;
	//	return *this;
	//}
	__forceinline XVAR& operator = (LPCVOID v)
	{
#if DBG
		if(!v) __debugbreak();
#endif
		_value = (T)v;
		return *this;
	}

	static_assert(sizeof(T) >= 4, "wtf");
};

typedef XVAR<PUCHAR> XPTR;

#endif // DRIVER


template<class T> struct H32
{
private:
	LONG handle;
public:
	__forceinline operator T() const
	{
		return (T)LongToPtr(handle);
	}
	__forceinline H32& operator = (T k)
	{
		handle = PtrToLong(k);
		return *this;
	}
	__forceinline H32& operator = (const H32 &o)
	{
		handle = o.handle;
		return *this; 
	}
	__forceinline operator bool () const
	{
		return handle != 0;
	}
	__forceinline bool operator == (const T k) const
	{
		return handle == PtrToLong(k);
	}
	__forceinline bool operator != (const T k) const
	{
		return handle != PtrToLong(k);
	}
	__forceinline bool valid() const
	{
		return handle != 0;
	}

	static_assert(sizeof(T) == sizeof(void*), "wtf");

	__forceinline LONG get_long() const
	{
		return handle;
	}
};



struct XXCOLOR
{
private:
	UINT _u;
	__forceinline UINT key() const { return (UINT)(SIZE_T)this; }

	__forceinline UINT get() const { return _u ^ key(); }
	__forceinline VOID set(UINT i) { _u = i ^ key(); }
	
public:
	__forceinline XXCOLOR& operator= (ULONG i)				{ set(i);			return *this; }
	__forceinline XXCOLOR& operator= (const XXCOLOR &src)	{ set(src.get());	return *this; }
	__forceinline operator ULONG() const { return get(); }
};

typedef const XXCOLOR CXXCOLOR;
typedef const XXCOLOR*PXXCOLOR;

#define NO_COLOR (*(XXCOLOR*)NULL)







