#pragma once


#define HUD_MAX_POINTS			(16)

#define HUD_STRING_LEFT			(0)
#define HUD_STRING_CENTERX		(1 << 1)
#define HUD_STRING_RIGHT		(1 << 2)
#define HUD_STRING_CENTERY		(1 << 3)
#define HUD_STRING_CENTER		(HUD_STRING_CENTERX|HUD_STRING_CENTERY)
#define HUD_STRING_UP			(1 << 4)
#define HUD_STRING_OPAQUE		(1 << 7)
#define HUD_STRING_OPAQUE_B		(HUD_STRING_OPAQUE | (1 << 6))
#define HUD_STRING_WORDWRAP		(1 << 8)

#define HUD_STRING_ALPHAMOD_0	(1 << 13)
#define HUD_ALPHA_SHIFT_0		(16) // Flags | (a << HUD_ALPHA_SHIFT) | HUD_STRING_ALPHAMOD
#define HUD_STRING_ALPHAMOD_BG	(1 << 14)
#define HUD_ALPHA_SHIFT_BG		(24) // Flags | (a << HUD_ALPHA_SHIFT) | HUD_STRING_ALPHAMOD

#define HUD_FONT_ITALIC			(1 << 0)
#define HUD_FONT_UNDERLINE		(1 << 1)
#define HUD_FONT_STRIKEOUT		(1 << 2)
#define HUD_FONT_SYMBOL			(1 << 3)
#define HUD_FONT_ANTIALIAS		(1 << 4)
#define HUD_FONT_DROPSHADOW		(1 << 5)
#define HUD_FONT_OUTLINE		(1 << 6)



typedef ULONG	XFONT, *PXFONT, **PPXFONT;

typedef const unsigned long CCOLOR;


//class Vector;
//struct Matrix4x4;

#include "e_math.h"



//User-defined types can be returned by value from global functions and static member functions.
//To return a user-defined type by value in RAX, it must have a length of 1, 2, 4, 8, 16, 32, or 64 bits.
//It must also have no user-defined constructor, destructor, or copy assignment operator;
//no private or protected non-static data members; no non-static data members of reference type;
//no base classes; no virtual functions; and no data members that do not also meet these requirements. 
struct XSIZE
{
	union
	{
		ULONG u;

		struct
		{
			USHORT cx;
			USHORT cy;
		};
	};

	__forceinline VOID Set(ULONG x, ULONG y) { u = x | (y << 16); } //	__forceinline VOID Set(ULONG x, ULONG y) { cx = (USHORT)x; cy = (USHORT)y; }
	__forceinline VOID Set(const RECT &rc) { Set(rc.right - rc.left, rc.bottom - rc.top); }
	__forceinline VOID Reset() { u = 0; }
};

struct XSIZE0 : public XSIZE
{
	__forceinline XSIZE0() {}
	__forceinline XSIZE0(const bool Nul)	{ if(Nul) Reset(); }
	__forceinline XSIZE0(ULONG x, ULONG y)	{ Set(x,y); }
};

typedef XSIZE *PXSIZE;


#pragma pack(push, 1)


typedef struct TEXCOORD
{
	FLOAT x0, y0;
	FLOAT x1, y1;

	TEXCOORD()
	{}
	explicit __forceinline TEXCOORD(FLOAT X0, FLOAT Y0, FLOAT X1, FLOAT Y1)
		: x0(X0), y0(Y0)
		, x1(X1), y1(Y1)
	{}
	__forceinline VOID SetI(INT X0, INT Y0, INT X1, INT Y1)
	{
		Vec4m mm;
		mm.init_i4f(X0, Y0, X1, Y1);
		mm.store4(&x0);
		////__m128i i = _mm_setr_epi32(X0,Y0,X1,Y1);
		////__m128 f = _mm_cvtepi32_ps(i);
		////_mm_storeu_ps(&x0, f);
	}
}
*PTEXCOORD;


typedef struct TEXCOORD_4i
{
	INT x0, y0; // 0 1
	INT x1, y1; // 2 3

	TEXCOORD_4i()
	{}
	explicit __forceinline TEXCOORD_4i(INT X0, INT Y0, INT X1, INT Y1)
		: x0(X0), y0(Y0)
		, x1(X1), y1(Y1)
	{}
	__forceinline VOID init(INT X0, INT Y0, INT X1, INT Y1)
	{
		x0 = X0; y0 = Y0;
		x1 = X1; y1 = Y1; 
	}
}
*PTEXCOORD_4i;


struct XVIEWSIZE16
{
	SHORT x;
	SHORT y;
	USHORT w;
	USHORT h;

	__forceinline XVIEWSIZE16(const bool z)
	{
		if(z) *(PULONG64)this = 0;
	}
	__forceinline XVIEWSIZE16(PINT xywh)
	{
		pack(xywh);
	}

	__forceinline XVIEWSIZE16& operator = (const XVIEWSIZE16 &v)
	{
		*(PULONG64)this = *(PULONG64)&v;
		return *this; 
	}

	__forceinline VOID get_RECT(RECT &rc)
	{
		for(SIZE_T i = 0; i < 2; i++)
		{
			XVIEWSIZE16 *src = (XVIEWSIZE16*) &((&x)[i]);
			LPRECT dst = (LPRECT) &((&rc.left)[i]);
			LONG X = src->x;
			dst->left = X;
			dst->right = X + (ULONG)src->w;
		}
	}

	__forceinline VOID set(UINT X, UINT Y, UINT W, UINT H)
	{
		*(PULONG)&x = X | (Y << 16);  // (X & 0xFFFF)
		*(PULONG)&w = W | (H << 16);  // (W & 0xFFFF)
	}

	__forceinline VOID pack(PINT xywh)
	{
#ifndef _AMD64_
		for(SIZE_T i = 0; i < 4; i++)
			(&x)[i] = (SHORT)xywh[i];
#else
		__m128i j = _mm_packs_epi32(_mm_loadu_si128((__m128i *)xywh), _mm_setzero_si128());
		_mm_storel_epi64((__m128i *)&x, j);
#endif
	}

	__forceinline VOID unpack(PINT xywh) const
	{
		for(SIZE_T i = 0; i < 4; i++)
			xywh[i] = (&x)[i];
	}

	__forceinline bool CheckClick(LONG X, LONG Y) const
	{
		if(!h)
			return 0;
		return	(X > x) && (X < x + w) &&
				(Y > y) && (Y < y + h);
	}

};

typedef XVIEWSIZE16 XVIEWSIZE;
typedef XVIEWSIZE16 *PXVIEWSIZE;
typedef XVIEWSIZE16 *PXVIEWSIZE16;




typedef struct XVIEWSIZE32
{
	LONG	x;
	LONG	y;
	ULONG	w;
	ULONG	h;

	__forceinline XVIEWSIZE32(LONG X, LONG Y, ULONG W, ULONG H)
		: x(X), y(Y), w(W), h(H)
	{}
	__forceinline XVIEWSIZE32()
	{
		memset0(this,sizeof(*this));
	}
	__forceinline VOID get_RECT(RECT &rc) const
	{
		rc.left = x;
		rc.right = x + w;
		rc.top = y;
		rc.bottom = y + h;
	}
	__forceinline VOID set(INT X, INT Y, UINT W, UINT H)
	{
		x = X;
		y = Y;
		w = W;
		h = H;
	}
	__forceinline VOID set(PINT xywh)
	{
		_memcpy(&x, xywh, 16);
	}

} *PXVIEWSIZE32;


//--------------------------------------------------------------


typedef struct XIMAGE
{
	enum XFORMAT
	{
		RGBA32 = 0,
		ABGR32 = 1, // DXGI_FORMAT_R8G8B8A8
		ARGB32 = 2,
		//BGRA32 = 3,
		TEXPTR = 4
	};

	PUCHAR	pBits;
	UINT	Width;
	UINT	Height;
	XFORMAT	Format;

	XIMAGE &ref()
	{
		return *this;
	}
	XIMAGE()
		: Format(RGBA32)
		, pBits(0)
	{}
	XIMAGE(PVOID Data, UINT W, UINT H, XFORMAT Fmt = RGBA32)
		: Format(Fmt)
		, pBits((PUCHAR)Data)
		, Width(W)
		, Height(H)
	{}
	XIMAGE(UINT W, UINT H, XFORMAT Fmt = RGBA32)
		: Format(Fmt)
		, Width(W)
		, Height(H)
	{
		Alloc(W*H);
	}
	inline VOID Dup(const XIMAGE *Src)
	{
		ULONG newWH = Src->Width * Src->Height;

		PULONG p = (PULONG)pBits;
		if(!p)
		{
			p = Alloc(newWH);
		}
		else
		if(newWH > (Width * Height) )
		{
			delete p;
			p = Alloc(newWH);
		}

		Width  = Src->Width;
		Height = Src->Height;
		Format = Src->Format;

		__movsd( p, (PULONG)Src->pBits, newWH );
	}
	inline PULONG Alloc(ULONG WH)
	{
		PULONG p = new ULONG[ WH ];
		pBits = (PUCHAR)p;
		return p;
	}
	void Release()
	{
		if(pBits) 
			delete [] pBits;
		delete this;
	}
} *PXIMAGE;

//--------------------------------------------------------------

struct VEC2
{
	FLOAT x, y;
};

struct VEC3
{
	FLOAT x, y, z;

	inline Vector &vec() { return *(Vector*)&x; }
};

struct VEC4
{
	FLOAT x, y, z, w;

	inline Vector& vec() { return *(Vector*)&x; }
	inline Vec4m& vec4() { return *(Vec4m*)&x; }
};


//--------------------------------------------------------------


struct ABC_t
{
	union
	{
		struct
		{
			INT8  a; // space before character (can be negative)
			UINT8 b; // width of character
			UINT8 _; // width overflow
			INT8  c; // space after character
		};
		ULONG abc;
	};
	ULONG w;

	__forceinline VOID Set(INT A, UINT B, INT C)
	{
		w = B + A + C;
		abc = (A & 0xFF) | (B << 8) | (C << 24);
	}

	__forceinline ABC_t& operator = (const ABC_t &v)
	{
		*(PULONG64)&abc = *(PULONG64)&v.abc;
		return *this; 
	}
};


//--------------------------------------------------------------


#define	COLORR(C)	( UCHAR( ULONG(C) >> 24) )
#define	COLORG(C)	( UCHAR( ULONG(C) >> 16) )
#define	COLORB(C)	( UCHAR( ULONG(C) >>  8) )
#define	COLORA(C)	( UCHAR( C ) )

#define RGBA(r, g, b, a) \
	(	(ULONG(r) << 24) | \
		(ULONG(g) << 16) | \
		(ULONG(b) <<  8) | ULONG(a) )

////#define RGBATOARGB(a) \
////	(	(UCHAR(a) << 24) | \
////		((a) >> 8)	)
////
////#define ARGBTORGBA(a) \
////	(	UCHAR((a) >> 24) | \
////		((a) << 8)	)

//#define ARGBTOABGR(a) \
//	(	(((a) & 0xFF00FF00)      ) | \
//		(((a) & 0x000000FF) << 16) | \
//		(((a) & 0x00FF0000) >> 16) )

//#define RGBATOABGR(a) \
//	(	(((a) >> 24) & 0x000000FF) | \
//		(((a) >>  8) & 0x0000FF00) | \
//		(((a) <<  8) & 0x00FF0000) | \
//		(((a) << 24) & 0xFF000000) )

__forceinline ULONG RGBATOARGB(ULONG C) { return _rotr(C, 8);	}
__forceinline ULONG ARGBTORGBA(ULONG C) { return _rotl(C, 8);	}
__forceinline ULONG RGBATOABGR(ULONG C) { return _byteswap_ulong(C);	}

__forceinline ULONG COLOR_CHANGE_R(ULONG c, ULONG r) { return ((c) & 0x00FFFFFF) | (((r)       ) << 24); }
__forceinline ULONG COLOR_CHANGE_G(ULONG c, ULONG g) { return ((c) & 0xFF00FFFF) | (((g) & 0xFF) << 16); }
__forceinline ULONG COLOR_CHANGE_B(ULONG c, ULONG b) { return ((c) & 0xFFFF00FF) | (((b) & 0xFF) << 8 ); }
__forceinline ULONG COLOR_CHANGE_A(ULONG c, ULONG a) { return ((c) & 0xFFFFFF00) | (((a) & 0xFF)      ); }


struct XGUICOLOR
{
	union
	{
		struct
		{
			UCHAR a, b, g, r;
		};
		UCHAR c[4];
		ULONG rgba;
	};
};



//--------------------------------------------------------------


struct XGUIFONTQUERY
{
	__forceinline XGUIFONTQUERY()
	{}

	__forceinline XGUIFONTQUERY(const UCHAR height, const UCHAR flags = 0, const UCHAR w = 0, const UCHAR blur = 0)
		: Flags(flags)
		, Tall(height)
		, Weight(w)
		, Blur(blur)
	{}

	UCHAR	Flags; // TFontHldr order
	UCHAR	Tall;
	UCHAR	Weight;
	UCHAR	Blur;

	CHAR	NameBuff[28];

	__forceinline ULONG GetWeight() const
	{
		return ULONG(Weight + 5) * 100; // FW_NORMAL:400 FW_SEMIBOLD:600 FW_BOLD:700
	}
};

//--------------------------------------------------------------


struct tFontQueryArg
{
	enum { e_Create, e_Query, e_Release } _act;
	XFONT _Font;
};


struct tFillRectArg
{
	struct { INT x0, y0, x1, y1; } // rect[1] used by eng
		rect[2];

	union
	{
		struct { ULONG    Bg, Top, Bottom, Left, Right;	}	color;
		struct { PXXCOLOR Bg, Top, Bottom, Left, Right;	}	color_ptr;

		ULONG		_u[1];
		PXXCOLOR	_p[1];
	};

	__forceinline tFillRectArg(INT x0, INT y0, INT x1, INT y1)
	{
		rect[0].x0 = x0;
		rect[0].y0 = y0;
		rect[0].x1 = x1;
		rect[0].y1 = y1;
	}

	tFillRectArg(const INT* xywh)
	{
		rect[0].x0 = xywh[0];
		rect[0].y0 = xywh[1];
		rect[0].x1 = xywh[0] + xywh[2];
		rect[0].y1 = xywh[1] + xywh[3];
	}

	inline ULONG Fix(LONG Count)
	{
		if(Count >= 0)
			return Count;
		ULONG ii = ULONG( -Count ), i = 0;
		do { _u[i] = _p[i] ? (*_p[i]) : 0; } while(++i < ii);
		return ii;
	}
};


struct tRectState
{
	INT x,y,w,h;
	tRectState * pXYZW;
	UCHAR saved;

	__forceinline tRectState(PINT ptr) : pXYZW( (tRectState*)ptr ), saved(0)
	{}

	__forceinline tRectState(INT X, INT Y, INT W, INT H) : x(X),y(Y),w(W),h(H), pXYZW(NULL), saved(0)
	{}

	__forceinline const tRectState *XYZW() const
	{
		return (pXYZW && !saved) ? pXYZW : this;
	}

	__forceinline VOID get_RECT(RECT &rc) const
	{
		const tRectState *p = XYZW();
		rc.left = p->x;
		rc.top = p->y;
		rc.right = p->x + p->w;
		rc.bottom = p->y + p->h;
	}
};



struct tDrawTextArg_base
{
	union
	{
		struct { FLOAT fX, fY, minX, minY, maxX, maxY; };
		struct { LONG x, y; };
	};
	ULONG MaxLength;
	ULONG Color;
};

struct tDrawTextArg : public tDrawTextArg_base
{
	ULONG Flags;
	ULONG MaxWidth;

	PVOID u[3];

	__forceinline
	tDrawTextArg(const LONG X, const LONG Y, const ULONG color, const ULONG Length = 0, const ULONG W = 0) : MaxWidth(W)
	{
		x = X;
		y = Y;
		Color = color;
		MaxLength = Length;
	}
};


#pragma pack(pop)


//--------------------------------------------------------------
// Hud->ZEnable
//
#define ZFLG_ZENABLE		(1 << 1)
#define ZFLG_ZWRITEENABLE	(1 << 2)
#define ZFLG_EX_FRAME		(1 << 8)
//#define ZFLG_EX_STATE0		(1 << 9)


//--------------------------------------------------------------


#if defined(_VMPROT_) || defined(PATCHER_STATIC)
#include "../guihud2.h"
#else


#define Pure = 0


class PROTOTYPE cTexture
{
public:
	virtual VOID Release() Pure;
	virtual VOID SetRGBA(const XIMAGE &) Pure;

	virtual LONG __vectorcall Draw(PINT xywh, const VEC3 *pYawCosSinScale, const TEXCOORD *, ULONG Color) Pure;
	virtual LONG __vectorcall Draw(const Vector &, const Vector &, const Vector &, FLOAT W, ULONG Color) Pure;

public:
	INT		m_PosX;
	INT		m_PosY;
	UINT	m_Width;
	UINT	m_Height;

	__forceinline VOID GetOffset(INT &x, INT &y) const
	{
		x = m_PosX;
		y = m_PosY;
	}
	__forceinline VOID SetOffset(INT x, INT y)
	{
		m_PosX = x;
		m_PosY = y;
	}
	__forceinline ULONG GetWide() const
	{
		return m_Width;
	}
	__forceinline ULONG GetTall() const
	{
		return m_Height;
	}
	__forceinline LONG Draw(INT x, INT y, INT w = 0, INT h = 0)
	{
		INT xywh[] = { x,y,w,h };
		return Draw(xywh, NULL, NULL, ~0);
	}

	__forceinline LONG Draw2_center(PINT XY, const UCHAR Alpha)
	{
		INT x = XY[0] - INT(GetWide()/2);
		INT y = XY[1] - INT(GetTall()/2);
		INT xywh[] = { x,y,0,0 };
		return Draw(xywh, NULL, NULL, Alpha ? (~0xFF | Alpha) : ~0);
	}
	__forceinline LONG Draw2(PINT XY, const UCHAR Alpha)
	{
		INT xywh[] = { XY[0],XY[1],0,0 };
		return Draw(xywh, NULL, NULL, Alpha ? (~0xFF | Alpha) : ~0);
	}

	__forceinline LONG __vectorcall Draw(INT x, INT y, INT w, INT h, FLOAT yaw, FLOAT scale)
	{
		INT xywh[] = { x,y,w,h };
		VEC3 CosSinScale;
		CosSinScale.z = scale;
		mm_sincos(yaw, &CosSinScale.y, &CosSinScale.x);
		return Draw(xywh, &CosSinScale, NULL, ~0 );
	}

	inline bool fix_xywh(PINT xywh)
	{
		for(PINT dst = xywh, src = &m_PosX; ; )
		{
			INT w = dst[2] ? dst[2] : src[2];	// fix WH if 0
			INT x = dst[0] + src[0];			// fix XY offset
			w += x;
			if(w < 0)
				return 0;
			dst[0] = x;
			dst[2] = w;
			dst++;
			src++;
			if(src > &m_PosY)
				break;
		}
		return 1;
	}
};




class PROTOTYPE cGuiHud
{
public:
	virtual BOOL Ready() Pure;
	virtual BOOL Begin(PVOID) Pure;
	virtual VOID End() Pure;

public:
	virtual VOID SetColor(ULONG) Pure;
	virtual VOID SetFont(XFONT) Pure;

	virtual ULONG DrawTextBuff(tDrawTextArg &, LPCSTR, XFONT) Pure; // ret XSIZE::u

	virtual VOID DrawRect(tFillRectArg &arg, LONG Count) Pure;

	virtual VOID DrawPolyLine(PINT pXY, ULONG Count, PUCHAR pIndexes = NULL) Pure;
	virtual VOID DrawPolyLine(PFLOAT pXY, ULONG Count, PUCHAR pIndexes = NULL) Pure;

	__forceinline VOID DrawLine(INT x0, INT y0, INT x1, INT y1)
	{
		INT xy[] = { x0, y0, x1, y1 };
		DrawPolyLine(xy, 2);
	}

	inline VOID DrawFilledRect(INT x0, INT y0, INT x1, INT y1, CCOLOR Bg)
	{
		if(!Bg) return;
		tFillRectArg arg(x0, y0, x1, y1);
		arg.color.Bg = Bg;
		DrawRect(arg, 1);
	}

	inline VOID DrawOutlinedRect(INT x, INT y, INT w, INT h, CCOLOR Bg)
	{
		if(!Bg) return;
		tFillRectArg arg(x, y, x+w, y+h);
		arg.color.Bg = Bg;
		DrawRect(arg, 1);
	}

	inline VOID DrawOutlinedRect(INT x, INT y, INT w, INT h, CCOLOR Bg, CCOLOR Line)
	{
		tFillRectArg arg(x, y, x+w, y+h);
		arg.color.Bg = Bg;
		arg.color.Top = Line;
		DrawRect(arg, 2);
	}
	inline VOID DrawOutlinedRect(INT x, INT y, INT w, INT h, CXXCOLOR& Bg, CXXCOLOR& Line)
	{
		tFillRectArg arg(x, y, x+w, y+h);
		arg.color_ptr.Bg = &Bg;
		arg.color_ptr.Top = &Line;
		DrawRect(arg, -2);
	}

	inline VOID DrawOutlinedRect(INT x, INT y, INT w, INT h, CCOLOR Bg, CCOLOR Top, CCOLOR Bottom, CCOLOR Left, CCOLOR Right)
	{
		tFillRectArg arg(x, y, x+w, y+h);
		arg.color.Bg = Bg;
		arg.color.Top = Top;
		arg.color.Bottom = Bottom;
		arg.color.Left = Left;
		arg.color.Right = Right;
		DrawRect(arg, 5);
	}
	inline VOID DrawOutlinedRect(INT x, INT y, INT w, INT h, CXXCOLOR& Bg, CXXCOLOR& Top, CXXCOLOR& Bottom, CXXCOLOR& Left, CXXCOLOR& Right)
	{
		tFillRectArg arg(x, y, x+w, y+h);
		arg.color_ptr.Bg = &Bg;
		arg.color_ptr.Top = &Top;
		arg.color_ptr.Bottom = &Bottom;
		arg.color_ptr.Left = &Left;
		arg.color_ptr.Right = &Right;
		DrawRect(arg, -5);
	}

	inline VOID DrawOutlinedRect(PINT xywh, CXXCOLOR& Bg)
	{
		if(!Bg) return;
		tFillRectArg arg(xywh);
		arg.color_ptr.Bg = &Bg;
		DrawRect(arg, -1);
	}
	inline VOID DrawOutlinedRect(PINT xywh, CXXCOLOR& Bg, CXXCOLOR& Line)
	{
		tFillRectArg arg(xywh);
		arg.color_ptr.Bg = &Bg;
		arg.color_ptr.Top = &Line;
		DrawRect(arg, -2);
	}
	inline VOID DrawOutlinedRect(PINT xywh, CXXCOLOR& Bg, CXXCOLOR& Top, CXXCOLOR& Bottom, CXXCOLOR& Left, CXXCOLOR& Right)
	{
		tFillRectArg arg(xywh);
		arg.color_ptr.Bg = &Bg;
		arg.color_ptr.Top = &Top;
		arg.color_ptr.Bottom = &Bottom;
		arg.color_ptr.Left = &Left;
		arg.color_ptr.Right = &Right;
		DrawRect(arg, -5);
	}


	virtual VOID DrawTriangle(PFLOAT pxy, ULONG bg) Pure;

public:

	__forceinline XSIZE DrawTextBuff(XFONT font, INT x, INT y, ULONG Color, LPCSTR Text, ULONG Flags = 0, UINT MaxLength = 0, UINT MaxWidth = 0)
	{
		tDrawTextArg arg(x,y,Color,MaxLength,MaxWidth);
		arg.Flags = Flags;
		register XSIZE ret;
		ret.u = DrawTextBuff(arg, Text, font);
		return ret;
	}

	__forceinline XSIZE DrawTextBuff(INT x, INT y, ULONG Color, LPCSTR Text, ULONG Flags = 0, UINT MaxLength = 0, UINT MaxWidth = 0)
	{
		return DrawTextBuff(0, x, y, Color, Text, Flags, MaxLength, MaxWidth);
	}

	virtual ULONG GetFittableStringLength(XFONT, LPCSTR Text, UINT Length, UINT MaxWidth, PXSIZE size = NULL) Pure;

public:
	virtual cTexture *CreateTexture() Pure;

	virtual VOID SetShaderI(ULONG) Pure;
	virtual VOID ZEnable(ULONG flags, Matrix4x4* viewProjectionMatrixTranspose = NULL) Pure;

public:
	virtual UINT FontQuery(tFontQueryArg &, XGUIFONTQUERY *) Pure;

	__forceinline XFONT Font_Create(XGUIFONTQUERY &font)
	{
		tFontQueryArg arg;
		arg._act = tFontQueryArg::e_Create;
		return FontQuery(arg, &font) ? arg._Font : 0;
	}
	__forceinline UINT Font_Query(XFONT hFont, XGUIFONTQUERY *pnfo = NULL)
	{
		tFontQueryArg arg = { tFontQueryArg::e_Query, hFont };
		return FontQuery(arg, pnfo);
	}
	__forceinline VOID Font_Release(XFONT hFont)
	{
		tFontQueryArg arg = { tFontQueryArg::e_Release, hFont };
		FontQuery(arg, NULL);
	}

public:
	virtual VOID SetRect(tRectState &) Pure;

	virtual VOID GetScreenSize(PUINT) Pure;

public:
	virtual LONG Initialize(PVOID dev, LPCVOID info) Pure;
	virtual VOID Reset(BOOL) Pure;
	virtual UINT Version() Pure;

public:

	__forceinline XSIZE GetTextExtent(XFONT hFont)
	{
		XSIZE sz;
		GetFittableStringLength(hFont, NULL, 0, 0, &sz);
		return sz;
	}

	__forceinline XSIZE GetTextExtent(XFONT hFont, PCSTR pText, const UINT Length)
	{
		XSIZE sz;
		GetFittableStringLength(hFont, pText, Length, 0x1000, &sz);
		return sz;
	}


	__forceinline XSIZE GetTextExtent(XFONT hf, PUCHAR Text, const UINT Length, const bool Unicode)
	{
		return GetTextExtent(hf, (PCSTR)Text, Length);
	}

	__forceinline ULONG GetFittableStringLength(XFONT hf, PUCHAR Text, UINT Length, UINT MaxWidth, PXSIZE size, const bool Unicode)
	{
		return GetFittableStringLength(hf, (PCSTR)Text, Length, MaxWidth, size);
	}


	__forceinline XSIZE DrawTextBuff(XFONT hf, const INT *xy, CCOLOR Color, PCSTR Text, const ULONG Flags = 0, const UINT MaxLength = 0, const UINT MaxWidth = 0)
	{
		return DrawTextBuff(hf, xy[0], xy[1], Color, Text, Flags, MaxLength, MaxWidth);
	}

	__forceinline XSIZE DrawTextBuff(XFONT hf, INT x, INT y, CCOLOR Color, PUCHAR Text, const ULONG Flags, const UINT MaxLength, const UINT MaxWidth, const bool Unicode)
	{
		return DrawTextBuff(hf, x, y, Color, (PCSTR)Text, Flags, MaxLength, MaxWidth);
	}


	inline XSIZE DrawTextF(XFONT hFont, INT x, INT y, CCOLOR Color, const ULONG Flags, PCSTR Text, ...)
	{
		va_list	argptr;
		va_start(argptr, Text);
		CHAR Buff[512];
		__vsnprintf(Buff, ARRAY_COUNT(Buff), Text, argptr);
		va_end(argptr);
		return DrawTextBuff(hFont, x, y, Color, Buff, Flags, 0, 0);
	}

	inline XSIZE DrawTextF(XFONT hFont, INT x, INT y, CCOLOR Color, const ULONG Flags, UINT MaxWidth, PCSTR Text, ...)
	{
		va_list	argptr;
		va_start(argptr, Text);
		CHAR Buff[512];
		__vsnprintf(Buff, ARRAY_COUNT(Buff), Text, argptr);
		va_end(argptr);
		return DrawTextBuff(hFont, x, y, Color, Buff, Flags, 0, MaxWidth);
	}

	inline XSIZE DrawTextF(XFONT hFont, PINT xy, CCOLOR Color, const ULONG Flags, PCSTR Text, ...)
	{
		va_list	argptr;
		va_start(argptr, Text);
		CHAR Buff[512];
		__vsnprintf(Buff, ARRAY_COUNT(Buff), Text, argptr);
		va_end(argptr);
		return DrawTextBuff(hFont, xy, Color, Buff, Flags, 0, 0);
	}

};


#undef Pure

#endif //#if defined(_VMPROT_) || defined(PATCHER_STATIC)





struct DXFLAG
{
	enum Flag
	{
	//	eDXH_STRETCH = 1,
	//	eDXH_SHITTER,
		eDXH_DEEPAH = 3,
	//	eDXH_POSTPROC = 4,
		eDXH_FULLSCREEN = 5,
	};
	enum Arg
	{
		Screen_GetDevice,
		Screen_Initialize,
		Screen_Blocks,
	};
};


struct tScreenInfo
{
	typedef SIZE_T (__fastcall *fn_ScreenInfo)(tScreenInfo*);

	DXFLAG::Arg id;

	union
	{
		struct { UINT x, y; H32<HWND> h; } init;
		struct { UINT v; } device;
		struct { DXFLAG::Flag Flg; SIZE_T Arg; } blocks;
	};

	__forceinline PVOID Screen_GetDevice(const UINT V)
	{
		tScreenInfo arg;
		arg.id = DXFLAG::Screen_GetDevice;
		arg.device.v = V;
		return PVOID(fn_ScreenInfo(this)(&arg));
	}
	__forceinline VOID Screen_Initialize(UINT x, UINT y, HWND hWnd)
	{
		tScreenInfo arg;
		arg.id = DXFLAG::Screen_Initialize;
		arg.init.x = x;
		arg.init.y = y;
		arg.init.h = hWnd;
		fn_ScreenInfo(this)(&arg);
	}
	__forceinline SIZE_T Screen_Blocks(const DXFLAG::Flag flg, const SIZE_T pArg)
	{
		tScreenInfo arg;
		arg.id = DXFLAG::Screen_Blocks;
		arg.blocks.Flg = flg;
		arg.blocks.Arg = pArg;
		return fn_ScreenInfo(this)(&arg);
	}
};



