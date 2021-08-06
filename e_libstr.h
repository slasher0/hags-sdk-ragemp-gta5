#pragma once

#define strcpy _lstrcpy
#define strlen _lstrlen

#define LIBF __fastcall

bool LIBF _lisnumeric(PCSTR); // +- _lisdigit
bool LIBF _lisnumeric(PCSTR, UINT);
bool LIBF _lisnumeric16(PCSTR); // _lisxdigit
bool LIBF _lisnumeric16(PCSTR, UINT);

PSTR  LIBF _ltolower(PSTR dst, PCSTR src, UINT len);
PSTR  LIBF _ltolower(PSTR dst, PCWSTR src);
PSTR  LIBF _ltoupper(PSTR dst, PCSTR src, UINT len);
PSTR  LIBF _lstrchrCC(PCSTR src, char c0, char c1);
PSTR  LIBF _lstrchrCB(PCSTR src, bool(*)(const char&) );
PSTR  LIBF _lstrchr2C(PCSTR src, ULONG c); // xx
PSTR  LIBF _lstrnchr(PCSTR src, UINT len, INT c);
PSTR  LIBF _lstrchr(PCSTR src, INT c);
PWSTR LIBF _lstrchr(PCWSTR src, INT c);
PSTR  LIBF _lstrrchr(PCSTR src, INT c);
PSTR  LIBF _lstrnrchr(PCSTR src, UINT len, INT c);
PWSTR LIBF _lstrrchr(PCWSTR src, INT c);
PWSTR LIBF _lstrnrchr(PCWSTR src, UINT len, INT c);
UINT  LIBF _lstrlen(PCSTR src);
UINT  LIBF _lstrlen(PCWSTR src);
PSTR  LIBF _lstrcpy(PSTR dst, PCSTR src);
PSTR  _cdecl _lstrcpy_m(PSTR dst, PCSTR src0, ...);
PWSTR _cdecl _lstrcpyW_m(PWSTR dst, PCWSTR src0, ...);
PWSTR _cdecl _lstrcpyA_m(PWSTR dst, PCSTR src0, ...);
PWSTR LIBF _lstrcpy(PWSTR dst, PCWSTR src);
PWSTR LIBF _lstrcpy(PWSTR dst, PCSTR src);
PSTR  LIBF _lstrcpy(PSTR dst, PCWSTR src);
PSTR  LIBF _lstrncpy(PSTR dst, PCSTR src, UINT len);
PWSTR LIBF _lstrncpy(PWSTR dst, PCWSTR src, UINT len);
PWSTR LIBF _lstrncpy(PWSTR dst, PCSTR src, UINT len);
PSTR  LIBF _lstrncpy(PSTR dst, PCWSTR src, UINT len);
PSTR  LIBF _lstrncat(PSTR dst, PCSTR src, UINT len);
PSTR  LIBF _lstrdup(PCSTR c, UINT AllocChars = 0);
PWSTR LIBF _lstrdup(PCWSTR c, UINT AllocChars = 0);
PWSTR LIBF _lstrdupW(PCSTR c, UINT AllocChars = 0);
PSTR  LIBF _lstrdupA(PCWSTR c, UINT AllocChars = 0);
PSTR  LIBF _lstrcut(PCSTR c, UINT r);
PWSTR LIBF _lstrcut(PCWSTR w, UINT r);
int LIBF _lstrcmp(PCSTR dst, PCSTR src);
int LIBF _lstrcmp(PCWSTR dst, PCSTR src);
int LIBF _lstrcmp(PCWSTR dst, PCWSTR src);
int LIBF _lstricmp(PCSTR dst, PCSTR src);
int LIBF _lstricmp(PCWSTR dst, PCSTR src);
int LIBF _lstricmp(PCWSTR dst, PCWSTR src);
int LIBF _lstrncmp(PCSTR dst, PCSTR src, UINT i);
int LIBF _lstrncmp(PCWSTR dst, PCSTR src, UINT i);
int LIBF _lstrncmp(PCWSTR dst, PCWSTR src, UINT i);
int LIBF _lstrncmpi(PCSTR dst, PCSTR src, UINT i);
int LIBF _lstrncmpi(PCWSTR dst, PCSTR src, UINT i);
int LIBF _lstrncmpi(PCWSTR dst, PCWSTR src, UINT i);
PSTR  LIBF _lstrstr(PCSTR str, PCSTR fnd);
PWSTR LIBF _lstrstr(PCWSTR str, PCSTR fnd);
PSTR  LIBF _lstrstri(PCSTR str, PCSTR fnd);
PWSTR LIBF _lstrstri(PCWSTR str, PCWSTR fnd);
PWSTR LIBF _lstrstri(PCWSTR str, PCSTR fnd);

PVOID LIBF _ldup(LPCVOID, ULONG);

PSTR LIBF _ltohex(LPCVOID, ULONG, PSTR);
UINT LIBF _lfromhex(PCSTR, PVOID, ULONG); // ret null bytes count at the end



PSTR LIBF _lstrstri_back(PCSTR begin, PCSTR end, PCSTR fnd);

PSTR __fastcall _ltoa(PSTR Out, LONG Value);
PSTR __fastcall _ultoa(PSTR Out, ULONG Value);

PSTR __fastcall _itoa(PSTR Out, INT Value);
PSTR __fastcall _uitoa(PSTR Out, UINT Value);

PSTR __fastcall _l64toa(PSTR Out, INT64 Value);
PSTR __fastcall _ul64toa(PSTR Out, UINT64 Value);
PSTR __fastcall _i64toa(PSTR Out, INT64 Value);
PSTR __fastcall _ui64toa(PSTR Out, UINT64 Value);


INT		LIBF _latoi(PCSTR);
UINT	LIBF _latoui(PCSTR);
LONG	LIBF _latol(PCSTR);
ULONG	LIBF _latoul(PCSTR);


#if defined(_AMD64_) || defined(_ANAL_)

INT64	_latoi64(PCSTR);
UINT64	_latoui64(PCSTR);
LONG64	_latol64(PCSTR);
ULONG64	_latoul64(PCSTR);

inline LONG64	AToL64(PCSTR s) { return _latol64(s);	}
inline INT64	AToI64(PCSTR s) { return _latoi64(s);	}
inline ULONG64	AToUL64(PCSTR s){ return _latoul64(s);	}
inline UINT64	AToUI64(PCSTR s){ return _latoui64(s);	}

#endif



PSTR __vectorcall _ftostr(PSTR dst, UINT prec, FLOAT f);

ULONG	__vectorcall FToA(FLOAT, PSTR);
FLOAT	__vectorcall AToF(PCSTR);



inline INT  AToI(PCSTR s)	{	return _latoi(s);	}
inline UINT AToUI(PCSTR s)	{	return _latoui(s);	}
inline INT  AToL(PCSTR s)	{	return _latol(s);	}
inline UINT AToUL(PCSTR s)	{	return _latoul(s);	}

inline UINT IToA(int i, PSTR s)
{
	PSTR p = _itoa(s, i);
	return UINT(p - s);
}

inline UINT UIToA(UINT i, PSTR s)
{
	PSTR p = _uitoa(s, i);
	return UINT(p - s);
}

inline UINT LToA(LONG i, PSTR s)
{
	PSTR p = _ltoa(s, i);
	return UINT(p - s);
}

inline UINT ULToA(ULONG i, PSTR s)
{
	PSTR p = _ultoa(s, i);
	return UINT(p - s);
}

inline UINT UIToW(UINT i, PWSTR s)
{
	CHAR t[32];
	UINT r = UIToA(i, t);
	_lstrcpy(s, t);
	return r;
}

inline UINT ULToW(ULONG x, PWSTR s)
{
	CHAR t[32];
	UINT r = ULToA(x, t);
	_lstrcpy(s, t);
	return r;
}




UINT __stdcall _lconv_wc2mb(PCWSTR pSrcStr, UINT cchSrc, PSTR  pDestStr, UINT cchDest);
UINT __stdcall _lconv_mb2wc(PCSTR  pSrcStr, UINT cchSrc, PWSTR pDestStr, UINT cchDest);

UINT __stdcall _lconv_wc2utf8(PCWSTR lpSrcStr, UINT cchSrc, PSTR  lpDestStr, UINT cchDest);
UINT __stdcall _lconv_utf82wc(PCSTR  lpSrcStr, UINT cchSrc, PWSTR lpDestStr, UINT cchDest);

UINT __stdcall _lconv_cp2utf8(PCSTR lpSrcStr, UINT cchSrc, PSTR lpDestStr, UINT cchDest);
UINT __stdcall _lconv_utf82cp(PCSTR lpSrcStr, UINT cchSrc, PSTR lpDestStr, UINT cchDest);


int __cdecl __vsnprintf(PSTR p, UINT_PTR i, PCSTR c, va_list a);



#include "__ascii.h"


class cString;

cString *	Str_Alloc(PCSTR);
cString *	Str_Alloc(cString **, PCSTR);
VOID		Str_Set(cString *, PCSTR);
UINT		Str_Get(cString *, PSTR, UINT);
VOID		Str_Release(cString **);
