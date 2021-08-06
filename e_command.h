#pragma once


class cCvarGroup;
class cCmdGroup;
class cExprParser;
class cString;
class IPath;

typedef cCvarGroup* PCvarGroup;
typedef cCmdGroup* PCmdGroup;

typedef	struct IVar *PIVar;


#define	MAX_ARGS	16



enum Cvar_Types : unsigned char // TypeId & 1 - SIGNED!  bits = (Type - 1) >> 1;  bytes = (1 << bits);
{
	CVAR_NONE = 0,

	CVAR_INT8,
	CVAR_UINT8,
	CVAR_INT16,
	CVAR_UINT16,
	CVAR_INT32,
	CVAR_UINT32,
	CVAR_INT64,
	CVAR_UINT64,

	CVAR_INT_PTR,
	CVAR_UINT_PTR,

	CVAR_BOOL,		// bit 1: value, 2: reset on change

	CVAR_FLOAT,
	CVAR_DOUBLE,

	CVAR_PTR,
	CVAR_STRING,
	CVAR_DATA,

	CVAR_PSTR,
	CVAR_PWSTR,

#ifdef _ANAL_
	CVAR_CLASS,
	CVAR_UNION,

	CVAR_TEMPLATE,
	CVAR_TEMPLATE_ARG,
	CVAR_TEMPLATE_REF,

	CVAR_UNKNOWN_,
#else

	CVAR_TMP_CLAMP_PTR,	// tmp IVar*
	CVAR_VAR_CLAMP_PTR,
#endif

};


//========================================================

typedef	VOID (CALLBACK* TPartialCallback)(Cvar_Types, PVOID Arg, PCSTR Partial, PCSTR FullName);


#pragma pack(push, 1)


typedef struct TCvarBool
{
	union
	{
		UINT32	i;
		UCHAR	c;
	};
	__forceinline bool Value()	 const	{ return (c & 1); }
	__forceinline operator bool() const	{ return (c & 1); }

	__forceinline VOID Init(const bool v)	{ i = __trash() | (v ? 3 : 2); } // not Changed
	__forceinline VOID SetV(const bool v)	{ c = (c & 0xFC) | (v ? 1 : 0); } // Changed

	__forceinline VOID Set0_Submit()		{ c = 2; } // not Changed

private:
	__forceinline UINT32 __trash() { return (UINT32)( (~(UINT32)(ULONG_PTR)this) << 2 ); }

} *PCvarBool;


struct TCvarBoolN : public TCvarBool
{
	__forceinline bool Changed() const		{ return (c & 2) == 0; }
	__forceinline bool ChangedTo0() const	{ return (c & 3) == 0; } // just changed to 0
	__forceinline bool Any() const			{ return (c & 3) != 2; } // value or changed   (c & 1) || !(c & 2)
	__forceinline bool Value_Submit()		{ UCHAR r = c; c = (r | 2); return (r & 1); }
	__forceinline VOID Submit()				{ c |= 2; }
};




typedef struct TCvarData
{
	PUCHAR	Ptr;
	ULONG	Size;
} *PCvarData;

typedef	union
{
	ULONG_PTR		uptr;
	LONG_PTR		iptr;
	DOUBLE			d;
	FLOAT			f;
	INT8			i8;
	UINT8			u8;
	INT16			i16;
	UINT16			u16;
	INT32			i32;
	UINT32			u32;
	LONG64			i64;
	ULONG64			u64;
	TCvarBoolN		b;
	PSTR			pstr;
	PWSTR			pwstr;
	cString *		s;
	PIVar			pv;

	volatile float _f;
#ifdef _ANAL_
	TCvarData	Data;
#endif

} TCvar, *PCvar;


#pragma pack(pop) // 1


//========================================================
//========================================================


enum CVAR_FLAGS
{
	CVAR_FLAG_NOINIT	= (1 << 0), // no change on AddVar
	CVAR_FLAG_CLAMP		= (1 << 1), // have min/max
	CVAR_FLAG_NOSAVE	= (1 << 2), // 
	CVAR_FLAG_READONLY	= (1 << 3), // no save & no cvar->set
	CVAR_FLAG_WRITEONLY	= (1 << 4), // no save
	CVAR_FLAG_ENGINE	= (1 << 5), // ignore FindPartial
	CVAR_FLAG_FUNC		= (1 << 7),
};


struct IVar
{
	union
	{
		PCvar		_Var;
		PCvarData	_Data;
		cString *	_Str;
		PVOID		_Ptr;
		PVOID(NTAPI*_PtrFn)();
	};

	Cvar_Types	_Type;
	UCHAR		_Flags;

	UCHAR __eng[ sizeof(void*)-2 ];

	union
	{
		ULONG32 u;
		LONG32 i;
		FLOAT f;
	}
	_Min, _Max;

	__forceinline PCvar GetVarPtr() const
	{
		PCvar p = _Var;
		if(!p)
			return p;
		if((_Flags & CVAR_FLAG_FUNC) && !(p = (PCvar)_PtrFn()) )
			return 0;
		return p;
	}

	__forceinline IVar()
	{
		memset0(this,sizeof(*this));
	}

	__forceinline IVar &Ref()
	{
		return *this;
	}


	template<Cvar_Types Type, class TVAR>
		__forceinline VOID __init_var(TVAR* pVar, const TVAR vMin, const TVAR vMax, const TVAR vCur, const ULONG Flags)
		{
			_Type = Type;
			_Flags = (UCHAR)Flags | CVAR_FLAG_CLAMP;
			_Min.u = vMin;
			_Max.u = vMax;
			_Ptr = pVar;
			if(vCur < vMin) *pVar = vMin; else
			if(vCur > vMax) *pVar = vMax; else *pVar = vCur;
		}

	__forceinline VOID init_Int32(PINT pVar, const INT iMin = MININT32, const INT iMax = MAXINT32, const INT iCur = 0, const ULONG Flags = 0)
	{
		__init_var<CVAR_INT32,INT>(pVar, iMin, iMax, iCur, Flags);
	}
	__forceinline VOID init_UInt32(PULONG pVar, const ULONG uMin = 0, const ULONG uMax = MAXUINT32, const ULONG uCur = 0, const ULONG Flags = 0)
	{
		__init_var<CVAR_INT32,ULONG>(pVar, uMin, uMax, uCur, Flags);
	}
	__forceinline VOID init_UInt8(PUCHAR pVar, const UCHAR uMin = 0, const UCHAR uMax = MAXUINT8, const UCHAR uCur = 0, const ULONG Flags = 0)
	{
		__init_var<CVAR_UINT8,UCHAR>(pVar, uMin, uMax, uCur, Flags);
	}

	__forceinline VOID init_Bool(PCvarBool pVar, const bool bCur = 0, const ULONG Flags = 0)
	{
		_Type = CVAR_BOOL;
		_Flags = (UCHAR)Flags;
		_Ptr = pVar;
		_Var->b.Init(bCur);
	}

};


//========================================================

struct IVar_noinit
{
protected:
	UCHAR _data[ sizeof(IVar) ];
public:
	IVar &Ref()
	{
		return *(IVar*)_data;
	}
};

//========================================================

#include "e_float.h"

struct IVarFloat : public IVar_noinit
{
	__forceinline IVarFloat(float *pVar, const float fMin, const float fMax, const float fCur = 0, const ULONG Flags = 0)
	{
		IVar &Var = Ref();
		Var._Type = CVAR_FLOAT;
		Var._Flags = (UCHAR)Flags | CVAR_FLAG_CLAMP;
		Var._Ptr = pVar;
		//if(fCur < fMin) *pVar = FLTC(fMin); else
		//if(fCur > fMax) *pVar = FLTC(fMax); else
		FLTCp(pVar,fCur);
		IVar & p = Ref();
		FLTCp(&p._Min.f,fMin);
		FLTCp(&p._Max.f,fMax);
	}
};

//========================================================


struct IVarUInt32 : public IVar_noinit
{
	__forceinline IVarUInt32(PULONG pVar, const UINT uMin = 0, const UINT uMax = MAXUINT32, const UINT uCur = 0, const ULONG Flags = 0)
	{
		Ref().init_UInt32(pVar, uMin, uMax, uCur, Flags);
	}
};


struct IVarInt32 : public IVar_noinit
{
	__forceinline IVarInt32(PINT pVar, const INT iMin = MININT32, const INT iMax = MAXINT32, const INT iCur = 0, const ULONG Flags = 0)
	{
		Ref().init_Int32(pVar, iMin, iMax, iCur, Flags);
	}
};


struct IVarUInt8 : public IVar_noinit
{
	__forceinline IVarUInt8(PUCHAR pVar, const UCHAR uMin = 0, const UCHAR uMax = MAXUINT8, const UCHAR uCur = 0, const ULONG Flags = 0)
	{
		Ref().init_UInt8(pVar, uMin, uMax, uCur, Flags);
	}
};


struct IVarBool : public IVar_noinit
{
	__forceinline IVarBool(PCvarBool pVar, const bool bCur, const ULONG Flags = 0)
	{
		Ref().init_Bool(pVar, bCur, Flags);
	}
};



//========================================================


class PROTOTYPE cCvarGroup
{
public:
	virtual VOID	Release() = 0;

public:
	virtual PCvar	Find(PCSTR Name, Cvar_Types *pType) = 0;
	virtual BOOL	Del(PCSTR) = 0;
	virtual BOOL	Set(PCSTR, PCSTR) = 0;
	virtual ULONG	GetString(PCSTR, PSTR) = 0;
	virtual PIVar	AddIVar(cString *Name, const IVar &) = 0;

private:
	__forceinline VOID AddVar(cString *Name, const Cvar_Types Type, const PVOID Ptr, const ULONG Flags)
	{
		IVar_noinit t;
		IVar &v = t.Ref();
		v._Type = Type;
		v._Ptr = Ptr;
		v._Flags = (UCHAR)Flags;
		AddIVar(Name, v);
	}

public:
	__forceinline VOID AddVarFn(cString *Name, Cvar_Types Type, PVOID Fn)
	{
		AddVar(Name, Type, Fn, CVAR_FLAG_FUNC);
	}

	__forceinline VOID AddVarULong(cString *Name, PULONG Ptr, const ULONG Value, const ULONG Flags = 0)
	{
		if(!(Flags & CVAR_FLAG_NOINIT))
			*Ptr = Value;
		AddVar(Name, CVAR_UINT32, Ptr, Flags);
	}
	__forceinline VOID AddVarInt(cString *Name, PINT Ptr, const INT Value, const ULONG Flags = 0)
	{
		if(!(Flags & CVAR_FLAG_NOINIT))
			*Ptr = Value;
		AddVar(Name, CVAR_INT32, Ptr, Flags);
	}
	__forceinline VOID AddVarBOOL(cString *Name, TCvarBool *Ptr, const bool Value, const ULONG Flags = 0)
	{
		if(!(Flags & CVAR_FLAG_NOINIT))
			Ptr->Init(Value);
		AddVar(Name, CVAR_BOOL, Ptr, Flags);
	}
	__forceinline VOID AddVarUInt8(cString *Name, PUCHAR Ptr, const UCHAR Value, const ULONG Flags = 0)
	{
		if(!(Flags & CVAR_FLAG_NOINIT))
			*Ptr = Value;
		AddVar(Name, CVAR_UINT8, Ptr, Flags);
	}

	__forceinline VOID AddVarFloat(cString *Name, float *Ptr)
	{
		AddVar(Name, CVAR_FLOAT, Ptr, 0);
	}
	__forceinline VOID AddVarFloatInit(cString *Name, float *Ptr, float v, const ULONG Flags = 0)
	{
		FLTCp(Ptr,v);
		AddVar(Name, CVAR_FLOAT, Ptr, Flags);
	}

	__forceinline VOID AddVarData(cString *Name, PCvarData Ptr, const ULONG Flags = 0)
	{
		AddVar(Name, CVAR_DATA, Ptr, Flags);
	}

#ifdef _CSTRING_H_
	__forceinline VOID AddVarString(cString *Name, cString *Ptr, PCSTR Value, const ULONG Flags = 0)
	{
		if(Value && !(Flags & CVAR_FLAG_NOINIT))
			Ptr->Set(Value);
		AddVar(Name, CVAR_STRING, Ptr, Flags);
	}
#endif

};


class PROTOTYPE cCvar
{
public:
	virtual PCvar		Find(LPCSTR Name, Cvar_Types *pType = NULL) = 0;
	virtual VOID		FindPartial(PCSTR, TPartialCallback, PVOID) = 0;
	virtual BOOL		Set(PCSTR, PCSTR) = 0;
	virtual ULONG		GetString(PCSTR, PSTR) = 0;
	virtual VOID		Save(PCWSTR) = 0;
	virtual PCvarGroup	CreateGroup() = 0;
};


class PROTOTYPE cCmdGroup
{
public:
	virtual VOID	Release() = 0;
	virtual VOID	Add(PCSTR, PVOID Fn, PVOID pArg = NULL) = 0;
	virtual bool	Del(PCSTR) = 0;
};


class PROTOTYPE cCmd
{
public:
	virtual VOID	FindPartial(PCSTR, TPartialCallback, PVOID) = 0;

	virtual UINT	Argc() const = 0;		// count
	virtual PCSTR	ArgV(UINT) const = 0;	// 0:name 1:value..
	virtual PCSTR	ArgV_Pointer(UINT) const = 0;
	virtual INT		ArgI(UINT) const = 0;
	virtual ULONG	ArgU(UINT) const = 0;	// atoui for 123 & atoul for 0x123
	virtual FLOAT _vectorcall ArgF(UINT) const = 0;
	virtual INT64	ArgI64(UINT) const = 0;
	virtual ULONG64	ArgU64(UINT) const = 0;

	virtual PCmdGroup	DefaultGroup() = 0;
	virtual PCmdGroup	CreateGroup() = 0;

	virtual cCvar *		GetVars() = 0;

	virtual cString *	AliasQuery(PCSTR) = 0;
	virtual VOID		AliasAdd(PCSTR, PCSTR) = 0;
	virtual bool		AliasDel(PCSTR) = 0;


	enum f_exec
	{
		e_STRING = (1 << 0),	// cString
		e_PATH_X = (1 << 1),	// IPath*
		e_PATH_W = (1 << 2),	// wchar*
		e_KEY_UP = (1 << 4),	// change + to -
		e_BOOL_N = (1 << 5),	// SubmitBools
	};
	struct t_exec_arg
	{
		LPCVOID Data;
		__forceinline t_exec_arg(LPCVOID p) : Data(p) {}
		ULONG Len;
		UCHAR Flags;
		mutable UCHAR SubmitBools;
	};
	virtual VOID Execute(const t_exec_arg &) = 0;

	__forceinline VOID Execute(PCSTR str, ULONG len)
	{
		t_exec_arg arg(str);
		arg.Len = len;
		arg.Flags = 0;
		Execute(arg);
	}
	__forceinline VOID Execute(const cString *str)
	{
		t_exec_arg arg(str);
		arg.Flags = e_STRING;
		Execute(arg);
	}
	__forceinline VOID Execute_UP(const cString *str)
	{
		t_exec_arg arg(str);
		arg.Flags = e_STRING | e_KEY_UP;
		Execute(arg);
	}

	__forceinline VOID ExecConfig(const IPath *path)
	{
		t_exec_arg arg(path);
		arg.Flags = e_PATH_X;
		Execute(arg);
	}

	__forceinline VOID ExecConfig_Init(const IPath *path)
	{
		t_exec_arg arg(path);
		arg.Flags = e_PATH_X | e_BOOL_N;
		arg.SubmitBools = 1;
		Execute(arg);
	}

	__forceinline bool __InitStage(const bool bSubmitBools)
	{
		t_exec_arg arg(0);
		arg.Flags = e_BOOL_N;
		arg.SubmitBools = bSubmitBools;
		Execute(arg);
		return arg.SubmitBools;
	}
};


