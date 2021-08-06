#ifndef _LSTR_STD_H__
#define _LSTR_STD_H__


#pragma pack(push, 1)

struct STDSTRINGA_0
{
	union
	{
		unsigned char  Buf[16];
		unsigned char *Ptr;
	} B;
	size_t Size;
	size_t Reserved;
};

struct STDSTRINGW_0
{
	union
	{
		wchar_t  Buf[8];
		wchar_t *Ptr;
	} B;
	size_t Size;
	size_t Reserved;
};

struct STDSTRINGA
{
	int Wtf;
	union
	{
		unsigned char  Buf[16];
		unsigned char *Ptr;
	} B;
	size_t Size;
	size_t Reserved;
};

struct STDSTRINGW
{
	size_t Wtf;
	union
	{
		wchar_t  Buf[8];
		wchar_t *Ptr;
	} B;
	size_t Size;
	size_t Reserved;
};

#pragma pack(pop)


const char *_lstringA(const void *ptr, char *Buff, size_t Size);
const char *_lstringA(const void *ptr);
const char *_lstringA(const void *ptr, size_t *pSize);
const char *_lstringA_0(const void *ptr, size_t *pSize);
const wchar_t *_lstringW(const void *ptr, char *Buff, size_t Size);
const wchar_t *_lstringW(const void *ptr);
const wchar_t *_lstringW(const void *ptr, size_t *pSize);
const wchar_t *_lstringW_0(const void *ptr, size_t *pSize);


#endif