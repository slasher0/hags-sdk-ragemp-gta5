#pragma once



template<class T> class XXXVAR
{
private:
	ULONG_PTR x;

	__forceinline T get() const
	{
		return (T)x;
	}

public:
	__forceinline VOID put(T v)
	{
		x = ULONG_PTR(v);	//_InterlockedExchangePointer((PPVOID)&x, (PVOID)v);
	}

	__forceinline XXXVAR & operator = (T v)
	{
		put(v);
		return *this;
	}

	__forceinline operator bool () const { return x != 0; }
	__forceinline operator T	() const { return get();	}
	__forceinline T operator -> () const { return get();	}
};


