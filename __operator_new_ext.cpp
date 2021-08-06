#include "e_sdk.h"

int __cdecl _purecall()
{
	__halt();
	__debugbreak();
	_mm_pause();
	return 0;
}

__declspec(noinline)
void * __cdecl operator new (size_t size)
{
	return g_Memory->Alloc(size);
}

__declspec(noinline)
void * __cdecl operator new[] (size_t size)
{
	return g_Memory->Alloc(size);
}

__declspec(noinline)
void __cdecl operator delete (void *ptr)
{
	g_Memory->Free(ptr);
}

__declspec(noinline)
void __cdecl operator delete[] (void *ptr)
{
	g_Memory->Free(ptr);
}

//------------------------------------------------

__declspec(noinline)
void __CRTDECL operator delete(
	void*  _Block,
	size_t _Size
	) throw()
{
	g_Memory->Free(_Block);
}

__declspec(noinline)
void __CRTDECL operator delete[](
	void* _Block,
	size_t _Size
	) throw()
{
	g_Memory->Free(_Block);
}

