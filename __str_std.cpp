
#include "e_lib_stdstr.h"


const char *_lstringA(const void *ptr, char *Buff, size_t Size)
{
	if(Buff)
		*Buff = 0;

	STDSTRINGA *p = (STDSTRINGA *)ptr;

	if(!p->Size || !p->Reserved || (p->Reserved >> 18) || p->Size > p->Reserved)
		return 0;

	unsigned char *pos = (p->Reserved >= 16) ? p->B.Ptr : p->B.Buf;

	if(!(((size_t)pos) >> 16))
		return 0;

	void *ret = pos;

	if(Buff)
	{
		unsigned char *end = &pos[ p->Size ];
		unsigned char *dst = (unsigned char *)Buff;

		while(end > pos && Size)
		{
			if(!(*dst++ = *pos++))
				break;
			Size--;
		}

		*dst = 0;
	}

	return (const char *)ret;//dst - (PBYTE)Buff;
}

const char *_lstringA(const void *ptr)
{
	return _lstringA(ptr, 0, 0);
}

const char *_lstringA(const void *ptr, size_t *pSize)
{
	STDSTRINGA *p = (STDSTRINGA *)ptr;

	if(!p->Size || !p->Reserved || (p->Reserved >> 18) || p->Size > p->Reserved)
		return 0;

	unsigned char *pos = (p->Reserved >= 16) ? p->B.Ptr : p->B.Buf;

	if(!(((size_t)pos) >> 16))
		return 0;

	if(pSize)
		*pSize = p->Size;

	return (const char *)pos;
}

const char *_lstringA_0(const void *ptr, size_t *pSize)
{
	STDSTRINGA_0 *p = (STDSTRINGA_0 *)ptr;

	if(!p->Size || !p->Reserved || (p->Reserved >> 18) || p->Size > p->Reserved)
		return 0;

	unsigned char *pos = (p->Reserved >= 16) ? p->B.Ptr : p->B.Buf;

	if(!(((size_t)pos) >> 16))
		return 0;

	if(pSize)
		*pSize = p->Size;

	return (const char *)pos;
}

//------------------------------

const wchar_t *_lstringW(const void *ptr, char *Buff, size_t Size)
{
	if(Buff)
		*Buff = 0;

	STDSTRINGW *p = (STDSTRINGW *)ptr;

	if(!p->Size || !p->Reserved || (p->Reserved >> 18) || p->Size > p->Reserved)
		return 0;

	wchar_t *pos = (p->Reserved >= 8) ? p->B.Ptr : p->B.Buf;

	if(!(((size_t)pos) >> 16))
		return 0;

	void *ret = pos;

	if(Buff)
	{
		wchar_t *end = &pos[ p->Size ];
		unsigned char *dst = (unsigned char *)Buff;

		while(end > pos && Size)
		{
			if(!(*dst++ = *(unsigned char *)pos++))
				break;
			Size--;
		}

		*dst = 0;
	}

	return (const wchar_t *)ret;//dst - (PBYTE)Buff;
}

const wchar_t *_lstringW(const void *ptr)
{
	return _lstringW(ptr, 0, 0);
}

const wchar_t *_lstringW(const void *ptr, size_t *pSize)
{
	STDSTRINGW *p = (STDSTRINGW *)ptr;

	if(!p || !p->Size || !p->Reserved || (p->Reserved >> 18) || p->Size > p->Reserved)
		return 0;

	wchar_t *pos = (p->Reserved >= 8)
		? p->B.Ptr
		: p->B.Buf;

	if(!(((size_t)pos) >> 16))
		return 0;

	if(pSize)
		*pSize = p->Size;

	return (const wchar_t *)pos;
}

const wchar_t *_lstringW_0(const void *ptr, size_t *pSize)
{
	STDSTRINGW_0 *p = (STDSTRINGW_0 *)ptr;

	if(!p || !p->Size || !p->Reserved || (p->Reserved >> 18) || p->Size > p->Reserved)
		return 0;

	wchar_t *pos = (p->Reserved >= 8)
		? p->B.Ptr
		: p->B.Buf;

	if(!(((size_t)pos) >> 16))
		return 0;

	if(pSize)
		*pSize = p->Size;

	return (const wchar_t *)pos;
}

