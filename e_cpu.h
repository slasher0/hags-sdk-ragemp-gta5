#pragma once

#define X86_CPUID_VENDOR_INTEL_EBX      0x756e6547 /* Genu */
#define X86_CPUID_VENDOR_INTEL_ECX      0x6c65746e /* ntel */
#define X86_CPUID_VENDOR_INTEL_EDX      0x49656e69 /* ineI */
#define X86_CPUID_VENDOR_AMD_EBX        0x68747541 /* Auth */
#define X86_CPUID_VENDOR_AMD_ECX        0x444d4163 /* cAMD */
#define X86_CPUID_VENDOR_AMD_EDX        0x69746e65 /* enti */
#define X86_CPUID_VENDOR_VIA_EBX        0x746e6543
#define X86_CPUID_VENDOR_VIA_ECX        0x736c7561
#define X86_CPUID_VENDOR_VIA_EDX        0x48727561
#define X86_CPUID_FEATURE_ECX_SSE3      (1 << 0)
#define X86_CPUID_FEATURE_ECX_PCLMUL    (1 << 1)
#define X86_CPUID_FEATURE_ECX_DTES64    (1 << 2)
#define X86_CPUID_FEATURE_ECX_MONITOR   (1 << 3)
#define X86_CPUID_FEATURE_ECX_CPLDS     (1 << 4)
#define X86_CPUID_FEATURE_ECX_VMX       (1 << 5)
#define X86_CPUID_FEATURE_ECX_SMX       (1 << 6)
#define X86_CPUID_FEATURE_ECX_EST       (1 << 7)
#define X86_CPUID_FEATURE_ECX_TM2       (1 << 8)
#define X86_CPUID_FEATURE_ECX_SSSE3     (1 << 9)
#define X86_CPUID_FEATURE_ECX_CNTXID    (1 << 10)
#define X86_CPUID_FEATURE_ECX_FMA       (1 << 12)
#define X86_CPUID_FEATURE_ECX_CX16      (1 << 13)
#define X86_CPUID_FEATURE_ECX_TPRUPDATE (1 << 14)
#define X86_CPUID_FEATURE_ECX_PDCM      (1 << 15)
#define X86_CPUID_FEATURE_ECX_PCID      (1 << 17)
#define X86_CPUID_FEATURE_ECX_DCA       (1 << 18)
#define X86_CPUID_FEATURE_ECX_SSE4_1    (1 << 19)
#define X86_CPUID_FEATURE_ECX_SSE4_2    (1 << 20)
#define X86_CPUID_FEATURE_ECX_X2APIC    (1 << 21)
#define X86_CPUID_FEATURE_ECX_MOVBE     (1 << 22)
#define X86_CPUID_FEATURE_ECX_POPCNT    (1 << 23)
#define X86_CPUID_FEATURE_ECX_TSCDEADL  (1 << 24)
#define X86_CPUID_FEATURE_ECX_AES       (1 << 25)
#define X86_CPUID_FEATURE_ECX_XSAVE     (1 << 26)
#define X86_CPUID_FEATURE_ECX_OSXSAVE   (1 << 27)
#define X86_CPUID_FEATURE_ECX_AVX       (1 << 28)
#define X86_CPUID_FEATURE_ECX_RDRAND    (1 << 30) // leaf 1
#define X86_CPUID_FEATURE_ECX_HVP       (1 << 31)
#define X86_CPUID_FEATURE_EBX_RDSEED    (1 << 18) // leaf 7
#define X86_CPUID_FEATURE_EDX_FPU       (1 << 0)
#define X86_CPUID_FEATURE_EDX_VME       (1 << 1)
#define X86_CPUID_FEATURE_EDX_DE        (1 << 2)
#define X86_CPUID_FEATURE_EDX_PSE       (1 << 3)
#define X86_CPUID_FEATURE_EDX_TSC       (1 << 4)
#define X86_CPUID_FEATURE_EDX_MSR       (1 << 5)
#define X86_CPUID_FEATURE_EDX_PAE       (1 << 6)
#define X86_CPUID_FEATURE_EDX_MCE       (1 << 7)
#define X86_CPUID_FEATURE_EDX_CX8       (1 << 8)
#define X86_CPUID_FEATURE_EDX_APIC      (1 << 9)
#define X86_CPUID_FEATURE_EDX_SEP       (1 << 11)
#define X86_CPUID_FEATURE_EDX_MTRR      (1 << 12)
#define X86_CPUID_FEATURE_EDX_PGE       (1 << 13)
#define X86_CPUID_FEATURE_EDX_MCA       (1 << 14)
#define X86_CPUID_FEATURE_EDX_CMOV      (1 << 15)
#define X86_CPUID_FEATURE_EDX_PAT       (1 << 16)
#define X86_CPUID_FEATURE_EDX_PSE36     (1 << 17)
#define X86_CPUID_FEATURE_EDX_PSN       (1 << 18)
#define X86_CPUID_FEATURE_EDX_CLFSH     (1 << 19)
#define X86_CPUID_FEATURE_EDX_DS        (1 << 21)
#define X86_CPUID_FEATURE_EDX_ACPI      (1 << 22)
#define X86_CPUID_FEATURE_EDX_MMX       (1 << 23)
#define X86_CPUID_FEATURE_EDX_FXSR      (1 << 24)
#define X86_CPUID_FEATURE_EDX_SSE       (1 << 25)
#define X86_CPUID_FEATURE_EDX_SSE2      (1 << 26)
#define X86_CPUID_FEATURE_EDX_SS        (1 << 27)
#define X86_CPUID_FEATURE_EDX_HTT       (1 << 28)
#define X86_CPUID_FEATURE_EDX_TM        (1 << 29)
#define X86_CPUID_FEATURE_EDX_PBE       (1 << 31)
#define X86_CPUID_MWAIT_ECX_EXT				(1 << 0)
#define X86_CPUID_MWAIT_ECX_BREAKIRQIF0		(1 << 1)
#define X86_CPUID_EXT_FEATURE_ECX_LAHF_SAHF	(1 << 0)
#define X86_CPUID_EXT_FEATURE_EDX_SYSCALL	(1 << 11)
#define X86_CPUID_EXT_FEATURE_EDX_NX		(1 << 20)
#define X86_CPUID_EXT_FEATURE_EDX_PAGE1GB	(1 << 26)
#define X86_CPUID_EXT_FEATURE_EDX_RDTSCP	(1 << 27)
#define X86_CPUID_EXT_FEATURE_EDX_LONG_MODE	(1 << 29)
#define X86_CPUID_AMD_FEATURE_EDX_FPU       (1 << 0)
#define X86_CPUID_AMD_FEATURE_EDX_VME       (1 << 1)
#define X86_CPUID_AMD_FEATURE_EDX_DE        (1 << 2)
#define X86_CPUID_AMD_FEATURE_EDX_PSE       (1 << 3)
#define X86_CPUID_AMD_FEATURE_EDX_TSC       (1 << 4)
#define X86_CPUID_AMD_FEATURE_EDX_MSR       (1 << 5)
#define X86_CPUID_AMD_FEATURE_EDX_PAE       (1 << 6)
#define X86_CPUID_AMD_FEATURE_EDX_MCE       (1 << 7)
#define X86_CPUID_AMD_FEATURE_EDX_CX8       (1 << 8)
#define X86_CPUID_AMD_FEATURE_EDX_APIC      (1 << 9)
#define X86_CPUID_AMD_FEATURE_EDX_MTRR      (1 << 12)
#define X86_CPUID_AMD_FEATURE_EDX_PGE       (1 << 13)
#define X86_CPUID_AMD_FEATURE_EDX_MCA       (1 << 14)
#define X86_CPUID_AMD_FEATURE_EDX_CMOV      (1 << 15)
#define X86_CPUID_AMD_FEATURE_EDX_PAT       (1 << 16)
#define X86_CPUID_AMD_FEATURE_EDX_PSE36     (1 << 17)
#define X86_CPUID_AMD_FEATURE_EDX_AXMMX     (1 << 22)
#define X86_CPUID_AMD_FEATURE_EDX_MMX       (1 << 23)
#define X86_CPUID_AMD_FEATURE_EDX_FXSR      (1 << 24)
#define X86_CPUID_AMD_FEATURE_EDX_FFXSR     (1 << 25)
#define X86_CPUID_AMD_FEATURE_EDX_3DNOW_EX  (1 << 30)
#define X86_CPUID_AMD_FEATURE_EDX_3DNOW     (1 << 31)
#define X86_CPUID_AMD_FEATURE_ECX_CMPL      (1 << 1)
#define X86_CPUID_AMD_FEATURE_ECX_SVM       (1 << 2)
#define X86_CPUID_AMD_FEATURE_ECX_EXT_APIC  (1 << 3)
#define X86_CPUID_AMD_FEATURE_ECX_CR8L      (1 << 4)
#define X86_CPUID_AMD_FEATURE_ECX_ABM       (1 << 5)
#define X86_CPUID_AMD_FEATURE_ECX_SSE4A     (1 << 6)
#define X86_CPUID_AMD_FEATURE_ECX_MISALNSSE (1 << 7)
#define X86_CPUID_AMD_FEATURE_ECX_3DNOWPRF  (1 << 8)
#define X86_CPUID_AMD_FEATURE_ECX_OSVW      (1 << 9)
#define X86_CPUID_AMD_FEATURE_ECX_IBS       (1 << 10)
#define X86_CPUID_AMD_FEATURE_ECX_SSE5      (1 << 11)
#define X86_CPUID_AMD_FEATURE_ECX_SKINIT    (1 << 12)
#define X86_CPUID_AMD_FEATURE_ECX_WDT       (1 << 13)
#define X86_CPUID_AMD_ADVPOWER_EDX_TS       (1 << 0)
#define X86_CPUID_AMD_ADVPOWER_EDX_FID      (1 << 1)
#define X86_CPUID_AMD_ADVPOWER_EDX_VID      (1 << 2)
#define X86_CPUID_AMD_ADVPOWER_EDX_TTP      (1 << 3)
#define X86_CPUID_AMD_ADVPOWER_EDX_TM       (1 << 4)
#define X86_CPUID_AMD_ADVPOWER_EDX_STC      (1 << 5)
#define X86_CPUID_AMD_ADVPOWER_EDX_MC       (1 << 6)
#define X86_CPUID_AMD_ADVPOWER_EDX_HWPSTATE (1 << 7)
#define X86_CPUID_AMD_ADVPOWER_EDX_TSCINVAR (1 << 8)

typedef enum  {
	CPUID_GETVENDORSTRING,			//0
	CPUID_GETFEATURES,				//1
	CPUID_GETTLB,					//2
	CPUID_GETSERIAL,				//3
} cpuid_requests;

#define	CPUID_INTELEXTENDED			0x80000000
#define	CPUID_INTELFEATURES			0x80000001
#define	CPUID_INTELBRANDSTRING		0x80000002
#define	CPUID_INTELBRANDSTRINGMORE	0x80000003
#define	CPUID_INTELBRANDSTRINGEND	0x80000004


//#pragma pack (push, 1)
//union t_cpuid
//{
//	INT32 i[4];
//	struct
//	{
//		ULONG32 EAX, EBX, ECX, EDX;
//	};
//	INT8 c[16];
//};
//#pragma pack (pop)

#ifndef _AMD64_
__forceinline ULONG GetEcx()
{
	_asm mov eax, ecx
}
__forceinline ULONG GetEbx()
{
	_asm mov eax, ebx
}
#endif

//extern bool _SSE41;
//extern bool _SSE42;
//extern bool _RDRND;

__forceinline void CPUID()
{

#ifdef _VEC_SSE4_

#if 1//def _AMD64_
	struct
	{
		int EAX, EBX, ECX, EDX, ext[4];
	}
	cpu;

	__cpuidex( &cpu.EAX, CPUID_GETFEATURES, 0 );

	BOOL bSSE41 = ( cpu.ECX & X86_CPUID_FEATURE_ECX_SSE4_1 );// ? 1 : 0; // ECX bit 19 (SSE 4.1)
	BOOL bSSE42 = ( cpu.ECX & X86_CPUID_FEATURE_ECX_SSE4_2 );// ? 1 : 0; // SSE 4.2

	//_RDRND = ( cpu.ECX & X86_CPUID_FEATURE_ECX_RDRAND ) ? 1 : 0; // RDRAND
#else
	//_asm push esi
	//_asm push edi
	_asm mov eax, CPUID_GETFEATURES
	_asm cpuid
	bool _SSE41 = ( GetEcx() & X86_CPUID_FEATURE_ECX_SSE4_1 ) ? 1 : 0; // ECX bit 19 (SSE 4.1)
	bool _SSE42 = ( GetEcx() & X86_CPUID_FEATURE_ECX_SSE4_2 ) ? 1 : 0; // SSE 4.2
	_RDRND = ( GetEcx() & X86_CPUID_FEATURE_ECX_RDRAND ) ? 1 : 0; // RDRAND
	//_asm pop edi
	//_asm pop esi
#endif

#ifdef TERM_NO_SSE4
	if(!bSSE41)
		__terminate3(0x41);
#endif
#endif

}


