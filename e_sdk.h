#pragma once

#include "e_shared.h"
#include "e_guid.h"
#include "e_libstr.h"
#include "e_list.h"

#ifndef _NONTDLL
#include "e_lib_sync.h"
#include "e_lib_xstr.h"
#endif

#include "e_guihud.h"
#include "e_gui.h"
#include "e_guiwin.h"

#include "e_patcher.h"
#include "e_command.h"
#include "e_console.h"
#include "e_input.h"
#include "e_settings.h"

#include "e_file.h"
#include "e_plugin.h" // main

#include "e_float.h" // ptr_no_opt




class PROTOTYPE ISTRF
{
public:
	int (__cdecl *vsnprintf)(PSTR, UINT_PTR, PCSTR, va_list);
	int (__cdecl *vsprintf)(PSTR, PCSTR, va_list);
	int (__cdecl *sprintf)(PSTR, PCSTR, ...);

	PSTR (__vectorcall *__ftostr)(PSTR dst, UINT prec, FLOAT f);
	FLOAT (__vectorcall *_AToF)(PCSTR);

	__forceinline ULONG __vectorcall _FToA(FLOAT Value, PSTR Buffer) const
	{
		PSTR p = __ftostr(Buffer, 3, Value);
		return ULONG(p - Buffer);
	}

	UINT (__stdcall *__lconv_wc2mb)(PCWSTR src, UINT src_len, PSTR  dst, UINT dst_len); // len=chars
	UINT (__stdcall *__lconv_mb2wc)(PCSTR  src, UINT src_len, PWSTR dst, UINT dst_len);
	UINT (__stdcall *__lconv_wc2utf8)(PCWSTR src, UINT src_len, PSTR  dst, UINT dst_len);
	UINT (__stdcall *__lconv_utf82wc)(PCSTR  src, UINT src_len, PWSTR dst, UINT dst_len);
	UINT (__stdcall *__lconv_utf82cp)(PCSTR src, UINT src_len, LPSTR dst, UINT dst_len);

	cString*(__stdcall *__Str_Alloc)(PCSTR);
	VOID	(__stdcall *__Str_Set)(cString *, PCSTR);
	UINT	(__stdcall *__Str_Get)(cString *, PSTR dst, UINT dst_len);
	VOID	(__stdcall *__Str_Release)(cString *);

	inline PSTR VecToStr(PSTR str, const FLOAT *pVec) const
	{
		for(ULONG i = 0; ; i++)
		{
			str = __ftostr(str, 3, pVec[i]);
			if(i >= 2) return str;
			*str++ = ' ';
		}
	}


	struct t_crypt_arg
	{
		PVOID dataSrc, dataDst;
		LPCVOID key;
		ULONG keyLen, dataLen;
	};
	void (__stdcall *_crypt)(t_crypt_arg &);

	__forceinline VOID rc4crypt(PVOID data, const ULONG len, LPCVOID key, const ULONG klen) const
	{
		t_crypt_arg arg { data, NULL, key, klen, len };
		_crypt(arg);
	}

	void (__vectorcall *_cossin)(float *);
	float (_vectorcall *_atan2p)(const float *y, const float *x);
};



class PROTOTYPE IImageHelper // e_guihud.h
{
public:
	virtual BOOL	ImageCopyBits(const XIMAGE &src, const XVIEWSIZE *srcRect, XIMAGE &dst, const XVIEWSIZE *dstRect) = 0;
	virtual BOOL	ImageFromMemory(PVOID, ULONG, XIMAGE &, BOOL fixA) = 0;
	virtual BOOL	ImageFromFile(IPath *, XIMAGE &, BOOL fixA) = 0;
	virtual VOID	ImageFill(const XIMAGE &, UINT X, UINT Y, UINT W, UINT H, ULONG Color) = 0;
	virtual VOID	DrawImage(cTexture *pTexture, INT X, INT Y, UINT insideW = 0, UINT insideH = 0, PXVIEWSIZE pRet = 0, UCHAR Repeat = 0) = 0; //1:x 2:y
	virtual LONG	FontsHelper(PVOID, UINT) = 0;
};


class PROTOTYPE IMemoryManager
{
public:
	virtual PVOID	Alloc(ULONG) = 0;
	virtual VOID	Free(PVOID) = 0;
	virtual PVOID	ReAlloc(PVOID, ULONG) = 0;
	virtual ULONG	Size(PVOID) = 0;
};



class PROTOTYPE IWINAPI
{
public:

	struct t_arg
	{
		union
		{
			PCSTR ModuleName;
			PVOID ModuleBase;
			ULONG Module_ID;
		};

		union
		{
			PCSTR FunctionName;
			ULONG OrdinalNumber;
			ULONG ModuleSize;	// ret with base
		};

		union
		{
			PCSTR FunctionName_REF;	// ret gpa
			ULONG Hint;
			PCSTR ImportModuleName; // in: IAT fnd
		};

		enum
		{
			e_GPA_DllName_FnORD		,
			e_GPA_DllName_FnName	,
			e_GPA_DllBase_FnORD		,
			e_GPA_DllBase_FnName	,
			e_BASE_DllName				= 4,
			e_BASE_DllName_SystemOnly,
			e_IAT_DllName_FnORD			= 8,
			e_IAT_DllName_FnName,
			//e_GPA_WRAP_DllName_FnORD	= 0x10,
			//e_GPA_WRAP_DllName_FnName,
			//e_GPA_WRAP_DllBase_FnORD,
			//e_GPA_WRAP_DllBase_FnName,
		};
	};
	virtual PVOID FUNC_0(t_arg &, UCHAR) = 0;

	PVOID GPA(PCSTR dll, PCSTR func, ULONG hint = 0, PCSTR *ppRef = NULL)
	{
		t_arg arg;
		arg.ModuleName = dll;
		arg.FunctionName = func;
		arg.Hint = hint;
		PVOID ret = FUNC_0(arg, t_arg::e_GPA_DllName_FnName);
		if(ppRef) *ppRef = arg.FunctionName_REF;
		return ret;
	}

	PVOID GPA(PVOID dll, PCSTR func, ULONG hint = 0, PCSTR *ppRef = NULL)
	{
		t_arg arg;
		arg.ModuleBase = dll;
		arg.FunctionName = func;
		arg.Hint = hint;
		PVOID ret = FUNC_0(arg, t_arg::e_GPA_DllBase_FnName);
		if(ppRef) *ppRef = arg.FunctionName_REF;
		return ret;
	}

	PVOID GPA(PCSTR dll, ULONG hash_or_ordinal)
	{
		t_arg arg;
		arg.ModuleName = dll;
		arg.OrdinalNumber = hash_or_ordinal;
		arg.Hint = 0;
		return FUNC_0(arg, t_arg::e_GPA_DllName_FnORD);
	}

	PVOID GPA_Hash(PVOID dll, ULONG x) // XHASH() #include "e_hash32.h" "e_hash.h"
	{
		t_arg arg;
		arg.ModuleBase = dll;
		arg.OrdinalNumber = x;
		arg.Hint = 0;
		return FUNC_0(arg, t_arg::e_GPA_DllBase_FnORD);
	}

	PBYTE GetBase(PCSTR dll, PULONG size)
	{
		t_arg arg;
		arg.ModuleName = dll;
		PVOID ret = FUNC_0(arg, t_arg::e_BASE_DllName);
		if(size) *size = arg.ModuleSize;
		return (PBYTE)ret;
	}

	PBYTE GetBase_SystemOnly(PCSTR dll, PULONG size)
	{
		t_arg arg;
		arg.ModuleName = dll;
		PVOID ret = FUNC_0(arg, t_arg::e_BASE_DllName_SystemOnly);
		if(size) *size = arg.ModuleSize;
		return (PBYTE)ret;
	}

	//PVOID GPA_Wrap(PVOID dll, PCSTR func)
	//{
	//	t_arg arg;
	//	arg.ArgType = t_arg::e_GPA_WRAP_DllBase_FnName;
	//	arg.ModuleBase = dll;
	//	arg.FunctionName = func;
	//	arg.Hint = 0;
	//	return FUNC_0(arg);
	//}

	//PVOID GPA_Wrap_Hash(PVOID dll, ULONG x)
	//{
	//	t_arg arg;
	//	arg.ArgType = t_arg::e_GPA_WRAP_DllBase_FnORD;
	//	arg.ModuleBase = dll;
	//	arg.OrdinalNumber = x;
	//	arg.Hint = 0;
	//	return FUNC_0(arg);
	//}

	PVOID GetBase_NTDLL(PULONG size)	{ return GetBase_SystemOnly(PCSTR(1),size); }
	PVOID GetBase_KERNEL32(PULONG size)	{ return GetBase_SystemOnly(PCSTR(2),size); }
	PVOID GetBase_USER32(PULONG size)	{ return GetBase_SystemOnly(PCSTR(3),size); }

	PVOID IAT(PVOID base, PCSTR ModuleName, PCSTR FuncName)
	{
		t_arg arg;
		arg.ModuleBase = base;
		arg.FunctionName = FuncName;
		arg.ImportModuleName = ModuleName;
		return FUNC_0(arg, t_arg::e_IAT_DllName_FnName);
	}

	PVOID IAT_Hash(PVOID base, PCSTR ModuleName, ULONG xFuncName)
	{
		t_arg arg;
		arg.ModuleBase = base;
		arg.OrdinalNumber = xFuncName;
		arg.ImportModuleName = ModuleName;
		return FUNC_0(arg, t_arg::e_IAT_DllName_FnORD);
	}
};



//----------------------------------------------------------------


#define INTERFACE_ALLOC(C) \
	public: \
		static void *operator new(size_t sz) { \
			static size_t t[ 1+ sizeof(C)/sizeof(size_t) ]; \
			return t; \
		} \
		static void operator delete(void*) {}


//----------------------------------------------------------------

struct t_globals
{
public:
	t_exc_IsValidPtr	ReadPTR;

	XXXVAR<IMainInterface *	>	_Interface;

	XXXVAR<IPatcher *		>	_Patcher;
	XXXVAR<IMemoryManager *	>	_Memory;
	XXXVAR<IWINAPI *		>	_Api;
	XXXVAR<ISTRF *			>	_Str;
	XXXVAR<IImageHelper *	>	_Image;

	XXXVAR<cCmd *			>	_Cmd;
	XXXVAR<cBind *			>	_Bind;
	XXXVAR<cConsole *		>	_Console;

	XXXVAR<cGuiHud *		>	_GuiHud;
	XXXVAR<cGUI *			>	_GUI;
	XXXVAR<cInput *			>	_Input;

private:

	__forceinline static void cpyint(PVOID p_dst, LPCVOID p_src, const ULONG count)
	{
		XXXVAR<PVOID> *dst = (XXXVAR<PVOID> *)p_dst;
		const XVAR<PVOID> *src = (const XVAR<PVOID> *)p_src;

		for(ULONG i = 0; i < count; i++)
			dst[i].put( src[i].value() );
	}

	__forceinline static void Update__main(t_globals *dst, const TSharedInterfaces *p)
	{
		cpyint( &dst->_Patcher, &p->Patcher, 5 );
	}

	__forceinline static void Update__plug(t_globals *_dst, const TSharedInterfaces *p)
	{
		t_globals *dst = (t_globals*)ptr_f_no_opt(_dst);

		dst->_GuiHud	.put( p->GuiHud	);
		dst->_GUI		.put( p->GUI	);
		dst->_Input		.put( p->Input	);

		cpyint( &dst->_Cmd, &p->Cmd, 3 );
	}

public:
	__forceinline TSharedInterfaces *Init_base(IMainInterface *pMainInterface)
	{
		t_globals *dst = (t_globals*)ptr_f_no_opt(this); // no static ref
		dst->_Interface.put(pMainInterface);
		dst->ReadPTR = pMainInterface->Interface_Query<t_exc_IsValidPtr>(G_FN_CHKPTR);
		TSharedInterfaces *pg = pMainInterface;
		Update__main(dst, pg);
		return pg;
	}
	__forceinline void Init_plug(IMainInterface *pMainInterface) // dllmain
	{
		TSharedInterfaces *pg = Init_base(pMainInterface);
		Update__plug(this, pg);
	}
	__forceinline void Update_plug() // CModuleInterface::Initialize
	{
		TSharedInterfaces *pg = _Interface;
		Update__plug(this, pg);
	}
};


//----------------------------------------------------------------

#if defined(PATCHER_STATIC)

extern IMainInterface * g_Interface;
extern ISTRF * g_Str;

#elif !defined(_BASEDLL)

extern t_globals globals;

#define gReadPTR	globals.ReadPTR

#define g_Interface	globals._Interface 
#define g_Shared	globals._Interface

#define g_Patcher	globals._Patcher 
#define g_Memory	globals._Memory	
#define g_Api		globals._Api	
#define g_Str		globals._Str	
#define g_Image		globals._Image	
#define g_GuiHud	globals._GuiHud	 
#define g_GUI		globals._GUI	 
#define g_Input		globals._Input	 
#define g_Cmd		globals._Cmd	 
#define g_Bind		globals._Bind	 
#define g_Console	globals._Console 

#define g_GuiFont	globals._GUI->FontInterface()

#define g_Screen	globals._Interface->Screen()

#endif // !_BASEDLL



//__declspec(noreturn) 
VOID __fastcall __Terminate(t_term &t);

__forceinline
VOID __terminate3(const ULONG A, const ULONG B = 0, const ULONG x0 = 0, const ULONG x1 = 0, const ULONG x2 = 0)
{
	__Terminate( t_term(0xBAD00000, A, B, x0,x1,x2).Ref() );
}

__forceinline
VOID __terminate_DX(const ULONG A, const ULONG B = 0, const ULONG x0 = 0, const ULONG x1 = 0, const ULONG x2 = 0)
{
	__Terminate( t_term(0xD3000000, A, B, x0,x1,x2).Ref() );
}


//----------------------------------------------------------------


#ifdef g_Str

__forceinline int __cdecl __vsnprintf(PSTR p, UINT_PTR i, PCSTR c, va_list a)
{
	int l = g_Str->vsnprintf(p, i, c, a);
	if(l <= 0) p[0] = 0;
	return l;
}

#endif


//----------------------------------------------------------------


#include "e_timer.h" // req g_Interface


