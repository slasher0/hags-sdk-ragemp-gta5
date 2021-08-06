#pragma once

#include "e_patcher.h"

//#pragma optimize( "gs", on )
//#pragma pack(push, 1)


// TODO: constexpr bs


	//__forceinline constexpr static char HEXC(const char ch) { return (ch >= '0' && ch <= '9') ? (ch & 0xF) : ((ch & 0xF) + 9); }

	//template <size_t N>
	//constexpr tSIG( const char(&SRC)[N] )
	//{
	//	SIG = p;
	//	opt = 0;

	//	ULONG MM = 0;
	//	ULONG ii = 0, pos = 0;

	//	for( ; ii < N; ii++)
	//	{
	//		if( SRC[ii*3] != '?' )
	//		{
	//			MM |= (1 << ii);
	//			const UCHAR cL = SRC[ii*3+1];
	//			const UCHAR cH = SRC[ii*3];
	//			const UCHAR c = HEXC(cL) | ( HEXC(cH) << 4);
	//			m_buff[ pos++ ] = c;
	//		}
	//	}
	//	MSK.LowPart = MM;
	//	MSK.HighPart = 0;
	//}



struct tSIG : public t_sig
{
	//static_assert( !( ((unsigned)(ch - '0') < 10u) || ((unsigned)((ch | 0x20) - 'a') < 6u)), "FAIL STR"  ); 

#define HEXC(ch) (((unsigned)(ch - '0') < 10u) ? (ch & 0xF) : ((ch & 0xF) + 9))

#define CH(ii) \
	if(ii >= (Size)/3) { MSK.LowPart = m; MSK.HighPart = 0; return; } \
	if( SRC[ii*3] != '?' ) { \
		m |= (1 << ii); \
		const UCHAR cL = SRC[ii*3+1]; \
		const UCHAR cH = SRC[ii*3]; \
		const UCHAR c = HEXC(cL) | (HEXC(cH) << 4); \
		p[ pos++ ] = c; }

	CHAR m_buff[31];

	template <size_t Size>
	__forceinline constexpr void Init(char const(&SRC)[Size])
	{
		SIG = m_buff;
		opt = 0;
		SIZE_T pos = 0;
		ULONG m = 0;
		PSTR p = (PSTR)ptr_f_no_opt(m_buff);
		CH( 0)
		CH( 1)
		CH( 2)
		CH( 3)
		CH( 4)
		CH( 5)
		CH( 6)
		CH( 7)
		CH( 8)
		CH( 9)
		CH(10)
		CH(11)
		CH(12)
		CH(13)
		CH(14)
		CH(15)
		CH(16)
		CH(17)
		CH(18)
		CH(19)
		CH(20)
		CH(21)
		CH(22)
		CH(23)
		CH(24)
		CH(25)
		CH(26)
		CH(27)
		CH(28)
		CH(29)
		CH(30)

		MessageBoxA(0,"OVERFLOWED",SRC,0);
		__assume(0);
	}
};


#undef CH
#undef HEXC

//#pragma pack(pop)
//#pragma optimize( "", on ) 

