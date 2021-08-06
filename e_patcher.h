#pragma once

#ifndef NtCurrentProcess
#define NtCurrentProcess() ( (HANDLE)(LONG_PTR) -1 )  // ntddk wdm ntifs
#endif



typedef PVOID (_vectorcall *t_exc_IsValidPtr)(LPCVOID); //G_FN_CHKPTR
typedef CHAR (_vectorcall *t_exc_memcpy)(PVOID, LPCVOID, SIZE_T); //G_FN_MEMCPY

typedef PVOID (__fastcall *tExecPTR)(LPCVOID pfn, PVOID arg, LPCVOID exc); //G_FN_EXECPTR  pfn: fastcall 1 arg


struct t_sig
{
	PSTR SIG;
	ULARGE_INTEGER MSK;
	UCHAR opt;
};


//#include "e_sig.h"





class PROTOTYPE IUnhooker
{
public:
	virtual void Release() = 0;
	virtual void Data(PVOID, ULONG) = 0;
	virtual bool Restore(PVOID) = 0;
	virtual void Thread(HANDLE) = 0;
	virtual void Wow64NtHook(PVOID) = 0;

	inline bool RestoreAll()
	{
		return Restore(INVALID_HANDLE_VALUE);
	}

	struct tUnhooker
	{
		tUnhooker(IUnhooker *p)
			: _ptr(p)
		{}
		inline void Release()
		{
			if(this && _ptr)
				_ptr->Release();
		}
		inline void Data(PVOID p, ULONG i)
		{
			if(this && _ptr)
				_ptr->Data(p, i);
		}
		inline bool Restore(PVOID p)
		{
			if(this && _ptr)
				return _ptr->Restore(p);
			return 0;
		}
		inline void Thread(HANDLE p)
		{
			if(this && _ptr)
				_ptr->Thread(p);
		}
		IUnhooker *_ptr;
	};
};




#ifndef PATCHER_STATIC
	#define vPFN virtual
	#define PUREf = 0
	#define PATCHER_PROTOTYPE PROTOTYPE
#else
	#define vPFN
	#define PUREf
	#define PATCHER_PROTOTYPE
#endif


class PATCHER_PROTOTYPE IPatcher
{
public:
	enum ePOP
	{
		P_ERASE_PATTERNS = 1,	// erase mask & pattern on P_OP_CLEAR
		P_SCAN_STEP,			//
		P_MAX,					//
		P_OP_CLEAR = ~0,		// clear thread data & erase shits
		P_OP_CLEAR_ALL = ~2		// clear all shits
	};
	vPFN	VOID	Options(ePOP Op = P_OP_CLEAR, INT Val = 0) PUREf;

	vPFN	IUnhooker *Unhooker() PUREf;

public:
	vPFN 	PUCHAR	Real(PVOID pOpPtr) PUREf;
	vPFN 	PUCHAR	Wrap(PVOID pOpPtr, PVOID pOpData = NULL, PVOID pOpDst = NULL, ULONG MinSize = 5, PULONG RetSize = NULL) PUREf;
	vPFN 	PUCHAR	Wrap_Alloc(ULONG Size, PVOID At = NULL) PUREf;

	vPFN 	LONG	CopyInstruction(PUCHAR pOpSrc, PUCHAR pOpDst = NULL, PUCHAR pOpSrcEx = NULL, BOOL FixRelative = 1, struct InstructionInfo *pOp = NULL) PUREf;
	vPFN 	LONG	CopyRelative2(PUCHAR pOpSrc, PUCHAR pOpDst, PUCHAR pOpSrcEx = NULL) PUREf;

	vPFN 	LONG	CreateJump(PVOID JmpSrc, PVOID JmpDst, BOOL bProtect = 1) PUREf;

	__forceinline
	static
	PUCHAR GetRelativeOffset(PVOID _JmpSrc, const ULONG Offs = 1, const ULONG BytesAfterOffs = 0)
	{
		if(!_JmpSrc)
			return NULL;
		PUCHAR JmpSrc = (PUCHAR)_JmpSrc;
		PUCHAR JmpDst = (JmpSrc + 4 + Offs + BytesAfterOffs) + *(PLONG32)&JmpSrc[Offs];
		return JmpDst;
	}

	__forceinline
	static
	PUCHAR GetRelativeOffset2(PVOID _JmpSrc)
	{
		PUCHAR JmpSrc = (PUCHAR)_JmpSrc;
		PUCHAR JmpDst = (JmpSrc + 2) + *(PCHAR)&JmpSrc[1];
		return JmpDst;
	}

	__forceinline
	static
	VOID SetRelativeOffset(PVOID _JmpSrc, PVOID _JmpDst, const ULONG Offs = 1)
	{
		PUCHAR JmpSrc = (PUCHAR)_JmpSrc;
		PUCHAR JmpDst = (PUCHAR)_JmpDst;
		_InterlockedExchange( (PULONG32)&JmpSrc[Offs], LONG32( JmpDst - (JmpSrc + 4 + Offs) ) );	//*(PLONG32)&JmpSrc[Offs] = LONG32( JmpDst - (JmpSrc + 4 + Offs) );
	}

	__forceinline
	VOID SetRelativeOffset(PVOID _JmpSrc, PVOID _JmpDst, ULONG Offset, const bool bProtect, PVOID ppOrig = NULL)
	{
		ULONG Attrib;
		if(bProtect && !(Attrib = Protect(_JmpSrc, 4+Offset, PAGE_EXECUTE_READWRITE)) )
			return;

		if(ppOrig)
			*(PVOID*)ppOrig = IPatcher::GetRelativeOffset(_JmpSrc, Offset);

		IPatcher::SetRelativeOffset(_JmpSrc, _JmpDst, Offset);

		if(bProtect)
			Protect(_JmpSrc, 4+Offset, Attrib);
	}

	inline PVOID AllocateRW(SIZE_T sz) // PAGE_READWRITE
	{
		return Allocate(sz, NULL); 
	}

	vPFN 	DWORD	Protect(PVOID, SIZE_T, ULONG Attrib = PAGE_EXECUTE_READWRITE, HANDLE hProc = NtCurrentProcess() ) PUREf; // 
	vPFN 	DWORD	ProtectEx2(PVOID Ptr, SIZE_T Size, ULONG fAdd, ULONG fDel = 0, HANDLE hProc = NtCurrentProcess() ) PUREf; // add/remove prot flags
	vPFN 	PVOID	Allocate(SIZE_T, HANDLE hProc = NtCurrentProcess() ) PUREf; // PAGE_EXECUTE_READWRITE
	vPFN 	VOID	Release(PVOID, HANDLE hProc = NtCurrentProcess() ) PUREf;
	vPFN 	SIZE_T	Query(PVOID, PMEMORY_BASIC_INFORMATION, ULONG_PTR) PUREf;

	vPFN 	ULONG_PTR	Read(HANDLE, PVOID, PVOID, ULONG_PTR) PUREf;
	vPFN 	ULONG_PTR	Write(HANDLE, PVOID, PVOID, ULONG_PTR) PUREf;
	vPFN 	ULONG_PTR	Write_Protect(HANDLE, PVOID, PVOID, ULONG_PTR) PUREf;

	vPFN 	BOOL	MemCpy(PVOID, LPCVOID, SIZE_T) PUREf;
	vPFN 	BOOL	MemSet(PVOID, UCHAR, SIZE_T) PUREf;

	vPFN 	PUCHAR	FindPattern(PVOID Start, PVOID End, t_sig &) PUREf;
	vPFN 	PUCHAR	FindPatternBack(PVOID From, ULONG Max, t_sig &) PUREf;
	vPFN 	PUCHAR	FindRelative(PVOID Start, PVOID End, t_sig &, PVOID pPtr, ULONG RelOpPos, ULONG RelOpSize = 0) PUREf;
	vPFN 	PUCHAR	FindPushPtr(PVOID From, PVOID To, PVOID Ptr) PUREf;
	vPFN 	PUCHAR	FindEpilogFrom(PVOID From, ULONG Max) PUREf;
	vPFN	PUCHAR	FindHotpatchSpace(PVOID Ptr, PVOID HookAt = 0) PUREf; // x86 only

	vPFN	PUCHAR	FindOpcode(PVOID Start, ULONG OpCount, ULONG Op0, ULONG Op1 = 0, BOOL BreakAtRet = 1) PUREf; // dasm


	__forceinline PUCHAR FindPattern(PVOID Start, PVOID End, PVOID pSig, const ULONG Len)
	{
		t_sig sig;
		sig.SIG = (PSTR)pSig;
		sig.MSK.LowPart = (Len >= 32) ? ~0 : ((1 << Len) - 1);
		sig.MSK.HighPart = (Len <= 32) ? 0 : ((1 << (Len - 32)) - 1);
		sig.opt = 0;
		return FindPattern(Start, End, sig);
	}


	__forceinline PUCHAR FindStringPtr(PVOID From, PVOID To, LPCSTR Str, const bool FullString)
	{
		t_sig sig;
		sig.SIG = (PSTR)Str;
		sig.MSK.LowPart = 0x1230; // magic val
		sig.opt = FullString ? 1 : 0;
		return FindPattern(From, To, sig);
	}
	__forceinline PUCHAR FindStringPtr(PVOID From, PVOID To, LPCWSTR Str, const bool FullString)
	{
		t_sig sig;
		sig.SIG = (PSTR)Str;
		sig.MSK.LowPart = 0x1230;
		sig.opt = FullString ? 3 : 2;
		return FindPattern(From, To, sig);
	}

#ifndef _AMD64_

	template<class CH>
	__forceinline PUCHAR FindOpStringPtr(PVOID _CodeFrom, PVOID _CodeTo, const UCHAR Op, PVOID _StrFrom, PVOID _StrTo, const CH* Str, const bool FullString = 0, const UCHAR Op5 = 0, const UCHAR Op6 = 0)
	{
		if(!_StrFrom)	_StrFrom = _CodeFrom;
		if(!_StrTo)		_StrTo = _CodeTo;
		PUCHAR pStrPtr = FindStringPtr(_StrFrom, _StrTo, Str, FullString);
		return pStrPtr ? FindOpPtr(_CodeFrom, _CodeTo, Op, 0, 0, pStrPtr, Op5, Op6) : pStrPtr;
	}

	template<class CH>
	__forceinline PUCHAR FindPushStringPtr(PVOID _CodeFrom, PVOID _CodeTo, PVOID _StrFrom, PVOID _StrTo, const CH* Str, const bool FullString = 0)
	{
		return FindOpStringPtr(_CodeFrom, _CodeTo, 0x68, _StrFrom, _StrTo, Str, FullString);
	}

	__forceinline PUCHAR FindOpPtr(PVOID _Start, PVOID _End, UCHAR Op0, UCHAR Op1, UCHAR Op2, PVOID Ptr, UCHAR Post0 = 0, UCHAR Post1 = 0)
	{
		ULONG Len = 0;
		UCHAR SIG[16];

		if(Op0)		SIG[Len++] = Op0;
		if(Op1)		SIG[Len++] = Op1;
		if(Op2)		SIG[Len++] = Op2;

		*(PULONG32)&SIG[Len] = (ULONG32)Ptr;
		Len += 4;

		if(Post0)	SIG[Len++] = Post0;
		if(Post1)	SIG[Len++] = Post1;

		return FindPattern(_Start, _End, SIG, Len);
	}

#else

	__forceinline PUCHAR FindOpPtr(	PVOID _Start, PVOID _End,
			const UCHAR Op0, const UCHAR Op1, const UCHAR Op2, PVOID Ptr,
			const UCHAR Post0 = 0, const UCHAR Post1 = 0,
			const UCHAR Op_retry = 0 )
	{
		ULONG Len = 0;
		ULONG MSK = 0;
		UCHAR SIG[16];

		if(Op0)		{  MSK |= (1 << Len);	SIG[Len] = Op0;		Len++; }
		if(Op1)		{  MSK |= (1 << Len);	SIG[Len] = Op1;		Len++; }
		if(Op2)		{  MSK |= (1 << Len);	SIG[Len] = Op2;		Len++; }

		ULONG RelOpPos = Len;
		Len += 4;

		if(Post0)	{  MSK |= (1 << Len);	SIG[Len-4] = Post0;	Len++; }
		if(Post1)	{  MSK |= (1 << Len);	SIG[Len-4] = Post1;	Len++; }

		t_sig sig;
		sig.SIG = (PSTR)SIG;
		sig.MSK.QuadPart = MSK;
		sig.opt = 0;

		PUCHAR found = FindRelative(_Start, _End, sig, Ptr, RelOpPos, RelOpPos + 4);

		if(Op_retry && !found)
		{
			SIG[ RelOpPos-1 ] = Op_retry;
			found = FindRelative(_Start, _End, sig, Ptr, RelOpPos, RelOpPos + 4);
		}

		return found;
	}

	__forceinline PUCHAR FindOpStringPtr(
			PVOID _CodeFrom, PVOID _CodeTo,
			const UCHAR Op0, const UCHAR Op1, const UCHAR Op2,
			PVOID _StrFrom, PVOID _StrTo,
			LPCSTR Str, const bool bFullString = 0,
			const UCHAR Op5 = 0, const UCHAR Op6 = 0,
			const UCHAR Op_retry = 0 )
	{
		if(!_StrFrom)	_StrFrom = _CodeFrom;
		if(!_StrTo)		_StrTo = _CodeTo;
		PUCHAR pStrPtr = FindStringPtr(_StrFrom, _StrTo, Str, bFullString);
		return pStrPtr ? FindOpPtr(_CodeFrom, _CodeTo, Op0, Op1, Op2, pStrPtr, Op5, Op6, Op_retry) : pStrPtr;
	}

#endif

	vPFN 	BOOL HookProt(PVOID Ptr, PVOID HookPtr, PVOID ppWrapPtr = NULL, IUnhooker *Unhooker = NULL, ULONG HookTag = 0) PUREf;
	vPFN 	BOOL HookProt_Remove(PVOID Ptr, ULONG HookTag = 0) PUREf;
	vPFN 	VOID HookProt_UpdateHooks() PUREf;
};


#undef vPFN
#undef PUREf

IPatcher *BuildPatcher();


/////////////////////////////////////////////////
/////////////////////////////////////////////////

// 48 8B 01             mov   rax,qword ptr [rcx]  
// FF 20                jmp   qword ptr [rax]  
// FF 60 00             jmp   qword ptr [rax+b]  
// FF A0 00 00 00 00    jmp   qword ptr [rax+d]  

template <class T>
ULONG omgpvfIndex(T func)
{
	union {
    	T p;
    	PUCHAR pb;
	};
	p = func;
	ULONG_PTR Offs = ~1;

#ifdef _AMD64_
	if(pb[0] == 0x48)
		pb += 1;
	else
		return ~0;
#endif

	if(pb[0] == 0x8B && pb[1] == 0x01)
    	pb += 2;
	else
		return ~0;

   	if(pb[0] != 0xFF)
		return ~0;
	pb += 1;

    if( pb[0] == 0x20 )
    	return 0;
	else
    if( pb[0] == 0x60 )
    	Offs = pb[1];
	else
	if( pb[0] == 0xA0 )
    	Offs = *(PULONG)&pb[1];

	if(Offs & (sizeof(PVOID)-1))
		return ~0;
	return Offs / sizeof(PVOID);
}



inline bool IsRet(const unsigned char *pOp)
{
	if(*pOp == 0xF3 || *pOp == 0xF2)
		pOp++;
	if(*pOp == 0xC3)
		return 1;
	if(*pOp == 0xC2)// && *(PUSHORT)&pOp[1] == 0)
		return 1;
	return 0;
}



