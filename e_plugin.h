#pragma once
#include "e_shared.h"


#pragma pack(push, 1)

// A3E 000<info> 00<term code>

struct t_term
{
	enum eTerm
	{
		HERR_CLIENT_DATA = 1, // netdata:0, shaders:1, 3:wow
		HERR_CLIENT_NETDATA, // 1:!offs, 2:lo size, 3:hi size, 4:crc, 0x33:version, 5&6: !read
		HERR_CLIENT_PATCHER, // 0:driver, 1:hw wow threads, 5&6: hw wow exc+check, 
		HERR_CLIENT_CONFIGS = 4,
		HERR_DIRECTX, // 0:notfound, 1:both, 2:nothing, 0x10+E:E:unpack shaders.
		HERR_MODULE_HUD,
		HERR_MODULE_ALL, // !Input|0x100 !Bind|0x010 !Cmd|0x001
	//	HERR_PUGBUST = 0x80,
		HERR_INJ = 0x55,	// 0xA3E00055
		HERR_ADV = 0x66,
		HERR_EXC = 0x77,
	};

	ULONG code;
	ULONG e[4];

	__forceinline t_term(const ULONG sig, const ULONG A, const ULONG B = 0, const ULONG x0 = 0, const ULONG x1 = 0, const ULONG x2 = 0)
	{ 
		code = sig | ((B & 0xFFF) << 8) | (A & 0xFF);
		PULONG p = e;
		if(x0) *p++ = x0;
		if(x1) *p++ = x1;
		if(x2) *p++ = x2;
		*p = 0;
	}

	t_term & Ref()
	{
		return *this;
	}
};



class PROTOTYPE IPluginInterface
{
public:
	virtual BOOL OnDraw() = 0;	// loop
	virtual BOOL OnInit() = 0;	//
	virtual VOID OnHalt() = 0;	// exc

	XVAR<IPluginInterface *> m_ChildPlug; // sdk->game->plugs
};


class PROTOTYPE IModuleInterface
{
public:
	virtual PVOID	Query(ULONG) = 0;	// 
	virtual VOID	Release() = 0;		// after Shutdown()
	virtual BOOL	Shutdown() = 0;		// return 0 to prevent unload/release
	virtual BOOL	Initialize() = 0;	// 
};



class IPath
{
public:
	ULONG (__fastcall *__Append)(IPath*, LPCVOID, UCHAR);
	USHORT __Len;
	WCHAR __Buff[276-1];

	__forceinline ULONG Len() const { return __Len; }
	__forceinline PCWSTR Get() const { return __Buff; }

	__forceinline ULONG Append(LPCSTR str, const bool chk) { return __Append(this, str, chk?2:0 ); }
	__forceinline ULONG Append(PCWSTR str, const bool chk) { return __Append(this, str, chk?3:1 ); }

	__forceinline VOID Release() { __Append(this,0,8); }
};


typedef VOID (_fastcall * TCommandFn)();
typedef BOOL (_fastcall * TScreenRedrawFn)();

struct tScreenInfo;


class PROTOTYPE IMainInterface : public TSharedInterfaces
{
public:
	ULONG m_TickCount;
	ULONG m_FPS;

	struct tWSize
	{
		ULONG	iX, iY, iCenterX, iCenterY;
		FLOAT	fX, fY, fCenterX, fCenterY;
	}
	m_WindowSize,		// GetWindowPlacement: rcNormalPosition w/h
	m_ScreenSize,		// dx rect
	m_DeviceSize;		// monitor

	H32<HWND> m_WindowHandle;

	ULONG _pad_0[3];

	struct
	{
		ULONG_PTR x;
		ULONG_PTR u[10];
		__forceinline ULONG_PTR operator [] (const ULONG i) const { ULONG_PTR y = u[i]; return (y ^ x);	}
	} xHldr;

	typedef SIZE_T	(__fastcall *fn_Rand)();
	typedef VOID	(__fastcall *fn_int_Register)(IModuleInterface *);
	typedef PVOID	(__fastcall *fn_int_Query)(ULONG);
	typedef IPath *	(__fastcall *fn_GetPath)(ULONG);
	typedef IPath *	(__fastcall *fn_GetPathFile)(PCSTR);
	typedef LONG	(__fastcall *fn_ThreadCreate)(HANDLE &, PVOID, PVOID);
	typedef VOID	(__fastcall *fn_Terminate)(t_term &);
	typedef BOOL	(__fastcall *fn_NullzorCall)(size_t, size_t);

	__forceinline SIZE_T Rand() const									{ return fn_Rand(xHldr[0])();	}
	__forceinline VOID	 Interface_Register(IModuleInterface *p)		{ return fn_int_Register(xHldr[1])(p);	}

	template<class T>
	__forceinline T 	 Interface_Query(ULONG u) const					{ return (T)(fn_int_Query(xHldr[2])(u));	}

	__forceinline IPath* GetConfigPath()								{ return fn_GetPath(xHldr[3])(1);	}
	__forceinline IPath* GetBasePath()									{ return fn_GetPath(xHldr[3])(0);	}

	__forceinline LONG	ThreadCreate(HANDLE &h, PVOID fn, PVOID arg)	{ return fn_ThreadCreate(xHldr[4])(h,fn,arg); }
	__forceinline VOID	Terminate(t_term &t) const						{ return fn_Terminate(xHldr[5])(t);		}

	__forceinline BOOL	NullzorCall(size_t a, size_t b) const	{ return fn_NullzorCall(xHldr[6])(a,b);	}

	__forceinline tScreenInfo * Screen() const				{ return (tScreenInfo*)xHldr[7]; }

	__forceinline IPath* FileNameArg(PCSTR Def)	{ return fn_GetPathFile(xHldr[8])(Def);	} // append to cofig path: default or valid cmd->argv(1)
	__forceinline cEngFILE* NewFile()			{ return Interface_Query<cEngFILE*>(G_LIB_FILE);	}

	ULONG_PTR _pad_1[2];
};



#pragma pack(pop)

