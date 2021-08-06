#pragma once

#include "e_shared.h"//#include "e_container.h"
#include "e_math.h"
#include "e_sdk.h"


#define H3E_SDK_VERSION		MAKELONG(2,0)


#pragma pack(push, 1)


//----------------------------------------------------------


typedef struct TObject
{
	ULONG	Type;		//
	XXCOLOR	Color;		//
	ULONG	Frame;		//
	ULONG 	iDist;		// = fDist
	Vector	World;		// origin. conv XYZ->XZY
	FLOAT	fDist;		// dist to local
	INT		Screen[2];	// w2s
	ULONG	Flags;		// current callback text flags
	XFONT	Font;		// current callback text font
	FLOAT	Angle;		// angle to local. unused
	FLOAT	Height;		// +z
	CHAR	Name[52];	//

	__forceinline bool Screen_Valid() { return *(PULONG64)Screen != 0; }
	__forceinline void Screen_Clear() { *(PULONG64)Screen = 0; }

	VOID SetWorldPos(const float *pPos, const Vector &PosLocal, const bool swapZY = false);

	ULONG DrawTextBuff(LPCSTR Text) const;

} *PObject, **PPObject;

struct TPlayer;
typedef TPlayer *PPlayer, **PPPlayer;



#if defined(_DEV_)
	#define STAGE(error_code) error_code
#else
	#define STAGE(error_code) 1
#endif


//----------------------------------------------------------


class SDK_CROSSHAIR;
class SDK_BONES;
class SDK_LOCATIONS;
class SDK_NAMES;
class SDK_ESP;
class SDK_RADAR;
class SDK_ENTS;



struct tH3ESDK_base
{
	Vec4m (__vectorcall *_NF_ScreenProjectPoint)(FVec4m Pos3d); // native fw2s
	Vec4m (__vectorcall *_GF_ScreenProjectPoint)(FVec4m Pos3d); // game specific (swapZY)

	__forceinline bool NI_ScreenProjectPoint(FVec4m pos, PINT Pos2d)
	{
		Vec4m xy = _NF_ScreenProjectPoint(pos);
		if(xy.is_empty()) return 0;
		if(Pos2d) { xy.float_to_int(); xy.store2(Pos2d); }
		return 1;
	}
	__forceinline bool GI_ScreenProjectPoint(FVec4m pos, PINT Pos2d)
	{
		Vec4m xy = _GF_ScreenProjectPoint(pos);
		if(xy.is_empty()) return 0;
		if(Pos2d) { xy.float_to_int(); xy.store2(Pos2d); }
		return 1;
	}
	__forceinline bool NF_ScreenProjectPoint(FVec4m pos, PFLOAT Pos2d)
	{
		Vec4m xy = _NF_ScreenProjectPoint(pos);
		if(xy.is_empty()) return 0;
		if(Pos2d) xy.store2(Pos2d);
		return 1;
	}
	__forceinline bool GF_ScreenProjectPoint(FVec4m pos, PFLOAT Pos2d)
	{
		Vec4m xy = _GF_ScreenProjectPoint(pos);
		if(xy.is_empty()) return 0;
		if(Pos2d) xy.store2(Pos2d);
		return 1;
	}

	__forceinline BOOL NI_ScreenProjectPoint(const float *Pos3d, PINT Pos2d)
	{
		Vec4m pos; pos.load4(Pos3d);
		return NI_ScreenProjectPoint(pos, Pos2d);
	}
	__forceinline BOOL NF_ScreenProjectPoint(const float *Pos3d, PFLOAT Pos2d)
	{
		Vec4m pos; pos.load4(Pos3d);
		return NF_ScreenProjectPoint(pos, Pos2d);
	}
	__forceinline BOOL GI_ScreenProjectPoint(const float *Pos3d, PINT Pos2d)
	{
		Vec4m pos; pos.load4(Pos3d);
		return GI_ScreenProjectPoint(pos, Pos2d);
	}
	__forceinline BOOL GF_ScreenProjectPoint(const float *Pos3d, PFLOAT Pos2d)
	{
		Vec4m pos; pos.load4(Pos3d);
		return GF_ScreenProjectPoint(pos, Pos2d);
	}

	cCvarGroup *		Cvar;
	cCmdGroup *			CmdGroup;
	cGUIColor *			GuiColor;
	cSettingsAlloc *	Settings;

    ULONG_PTR			AddressLimit; // SYSTEM_BASIC_INFORMATION.MaximumUserModeAddress - 0x10000


	void (__stdcall *Box_native_calc)(const float *vMin, const float *vMax, const Matrix4x4 *Matrix, PVOID vec4x8cache ); // Box_Render_native2 CalcOnly 
	bool (__stdcall *Box_draw)(PVOID vec4x8cache); // ^ CalcOnly = -1

	bool (__stdcall *Box)(const float *vMin, const float *vMax);
	bool (__stdcall *Box_native)(const float *vMin, const float *vMax, const Matrix4x4 *Matrix, const float *vPos, PVOID vec4x8cache ); // calc & draw

	bool (__stdcall *Intersect)(const float *vStart, const float *vDelta, PVOID Vec4x8); // intersect ray with box. delta = end-start = direction*length

	__forceinline bool Box_Render_native(const float *vMin, const float *vMax, const Matrix4x4 *Matrix, const float *vPos = NULL, PVOID vec4x8cache = NULL) const
	{
		return Box_native(vMin, vMax, Matrix, vPos, vec4x8cache);
	}
	__forceinline bool Box_Render(const Vector &vMin, const Vector &vMax) const
	{
		return Box(&vMin.x, &vMax.x);
	}
	//virtual BOOL Box_Render_native2(const float *vMin, const float *vMax, const Matrix4x4 &Matrix, const float *vPos, PVOID Vec4x8, BOOL CalcOnly = 0) = 0;
	//virtual BOOL Box_Render2(const Vector &vMin, const Vector &vMax, PVOID Vec4x8, BOOL CalcOnly = 0) = 0;

};


struct tH3ESDK : public tH3ESDK_base
{
	IUnhooker *			Unhooker;	// OnInit
	SDK_CROSSHAIR *		_Crosshair;	// OnInit
	SDK_BONES *			_Bones;		//
	SDK_LOCATIONS *		_Loc;		//
	SDK_NAMES *			_Names;		//
	SDK_ESP *			_Esp;		//
	SDK_RADAR *			_Radar;		//
	SDK_ENTS *			_Ents;		//

	UCHAR				Valid;
};



#ifdef _ENGINEDLL
extern tH3ESDK_base sdk;
#else
extern tH3ESDK sdk;
#endif



//--------------------------------------------------------


struct tMODULE
{
public:
	PUCHAR	Base;
	PUCHAR	BaseMin;
	PUCHAR	BaseMax;
	//PUCHAR	CodeMin;
	//PUCHAR	CodeMax;
	//PUCHAR	DataMin;
	//PUCHAR	DataMax;

	bool init(PCSTR name);
};

extern tMODULE GameModuleInfo;


__forceinline bool IsValidPtr(const void *p)
{
	ULONG_PTR u = ULONG_PTR(p) - 0x10000;
	return u < sdk.AddressLimit;
}

__forceinline bool IsValidPtr_16(const void *p)	{ return !(ULONG_PTR(p) & 0xF) && IsValidPtr(p); }
__forceinline bool IsValidPtr_8	(const void *p)	{ return !(ULONG_PTR(p) & 0x7) && IsValidPtr(p); }
__forceinline bool IsValidPtr_4	(const void *p)	{ return !(ULONG_PTR(p) & 0x3) && IsValidPtr(p); }

__forceinline bool IsGamePtr	(const void *p)	{ return p && ((PUCHAR)p >= GameModuleInfo.BaseMin) && ((PUCHAR)p < GameModuleInfo.BaseMax); }
__forceinline bool IsGameVtbl	(const void *p)	{ return IsValidPtr(p) && IsGamePtr(*(PVOID*)p); }
__forceinline bool IsGameVtbl_16(const void *p)	{ return IsValidPtr_16(p) && IsGamePtr(*(PVOID*)p); }

////__forceinline PUCHAR __ReadPtr(void *Ptr) { return (Ptr && NT_SUCCESS(NtReadVirtualMemory(NtCurrentProcess(), Ptr, &Ptr, sizeof(Ptr), 0))) ? (PUCHAR)Ptr : NULL; }

#ifdef gReadPTR
__forceinline bool IsGameVtblPtr(const void *p)	{ return IsValidPtr(p) && IsGamePtr(gReadPTR(p)); }
#endif


//--------------------------------------------------------


class PROTOTYPE SDK_CROSSHAIR
{
public:
	struct t_data
	{
		ULONG	x,y;
		UCHAR	Enabled;
		UCHAR	Show;
		UCHAR	Alpha;
		UCHAR	U;

		inline bool ScreenCenter() const
		{
			return *(PULONG64)&x == 0;
		}
	} m_data;

	inline VOID Set(LONG x, LONG y, UCHAR a)
	{
		m_data.x = x;
		m_data.y = y;
		m_data.Alpha = a;
	}
	inline bool Show(bool a)
	{
		m_data.Show = a ? 1 : 0;
		return m_data.Enabled;
	}
};


//--------------------------------------------------------


class PROTOTYPE IESP_VarView
{
public:
	virtual VOID Render1(PObject) = 0;
	virtual VOID Render2(PObject) = 0;

	enum
	{
		FLAG__SCREEN_REAL_SCREEN_POS	= (1 << 1), // ignore esp settings
		FLAG__SCREEN_REAL_WORLD_POS		= (1 << 2), // ignore height & scale
		FLAG__SCREEN_ANY_POS			= (1 << 3),
		FLAG__REQ_VALID_SCREEN_POS		= (1 << 4), // callback req valid obj.Screen
	};

	ULONG	__Index; // _ObjectType;
	ULONG	_Flags;

	struct IEspBox
	{
		ULONG DistMin;
		ULONG DistMax;
		UCHAR Alpha;
		UCHAR Alpha2;
		UCHAR _pad[2];

		__forceinline bool CheckRange(const TObject *p) const
		{
			return (DistMax >= p->iDist) && (DistMin <= p->iDist);
		}
	};

	IEspBox m_Bounds; // 0
	IEspBox m_Hitbox; // 1
};


#define BOX_ALPHA2 0x80 // SDK_ESP::AddVar_box

#ifndef MAX_OBJ_TYPES
#define MAX_OBJ_TYPES 16
#endif

typedef INT  (_fastcall *TViewerCallback0)(PObject, PVOID);

#define ESP_NAME_CB (TViewerCallback0)0x11
#define ESP_DIST_CB (TViewerCallback0)0x12


struct TYPES_ARG
{
	UCHAR Types[MAX_OBJ_TYPES+4];

	__forceinline TYPES_ARG &Ref() 
	{
		return *this;
	}

	__forceinline TYPES_ARG(
		const UCHAR T00,
		const UCHAR T01 = 0, const UCHAR T02 = 0, const UCHAR T03 = 0, const UCHAR T04 = 0, const UCHAR T05 = 0,
		const UCHAR T06 = 0, const UCHAR T07 = 0, const UCHAR T08 = 0, const UCHAR T09 = 0, const UCHAR T10 = 0,
		const UCHAR T11 = 0, const UCHAR T12 = 0, const UCHAR T13 = 0, const UCHAR T14 = 0, const UCHAR T15 = 0	)
	{
		PUCHAR p = Types;
		if(T00) *p++ = T00;
		if(T01) *p++ = T01; if(T02) *p++ = T02; if(T03) *p++ = T03; if(T04) *p++ = T04; if(T05) *p++ = T05;
		if(T06) *p++ = T06; if(T07) *p++ = T07; if(T08) *p++ = T08; if(T09) *p++ = T09; if(T10) *p++ = T10;
		if(T11) *p++ = T11; if(T12) *p++ = T12; if(T13) *p++ = T13; if(T14) *p++ = T14; if(T15) *p++ = T15;
		*p = 0;
	}

	__forceinline TYPES_ARG() {}

	__forceinline VOID Range(const ULONG First, const ULONG Last)
	{
		PUCHAR p = Types;
		for(ULONG i = First, j = 0; i <= Last && j < MAX_OBJ_TYPES; i++, j++)
			*p++ = (UCHAR)i;
		*p = 0;
	}
};


class PROTOTYPE SDK_ESP
{
public:
	TCvarBool		_Active;

	struct
	{
		TCvarBool	Use;
		XXCOLOR		Color;
	}
	_target;

public:
	virtual BOOL Ready() = 0;

	virtual IESP_VarView *PreDrawObject1(PObject Obj, Vector *RealWorldPos = NULL) = 0;
	virtual IESP_VarView *PreDrawObject2(PObject Obj) = 0;

	virtual VOID Register() = 0;
	virtual VOID Register_TargetColor() = 0;
	virtual VOID Release() = 0;


	struct tAddVarArg
	{
		enum eArgFlags
		{
			e_VAR		= (1 << 0), // tVar
			e_VAR_TYPE	= (1 << 1),
			e_VAR_FLAGS	= (1 << 2),
			e_BOX		= (1 << 4), // tBox
			e_INHERIT	= -2,
		};
		ULONG ArgFlags;

		struct tVar
		{
			ULONG ViewFlags; // e_VAR_FLAGS
			TViewerCallback0 Cb0;
			PCSTR Name;	// e_VAR_NAME
			Cvar_Types VarType; // e_VAR_TYPE
		};
		struct tBox
		{
			ULONG Range;
			TViewerCallback0 Cb0[2]; // bounds/hitbox
		};
		union
		{
			tVar	var;
			tBox	box;
		};
	};

	virtual VOID AddVarImpl(const tAddVarArg &, const TYPES_ARG &) = 0;


	__forceinline VOID AddVar_range(const ULONG First, const ULONG Last)
	{
		TYPES_ARG typ;
		typ.Range(First, Last);
		AddVar(typ);
	}

	__forceinline VOID AddVar_range(PCSTR Name, TViewerCallback0 Cb0, const ULONG First, const ULONG Last)
	{
		tAddVarArg arg;
		arg.ArgFlags = tAddVarArg::e_VAR;
		arg.var.Name = Name;
		arg.var.Cb0 = Cb0; // PreDrawCb if Name NULL
		TYPES_ARG typ;
		typ.Range(First, Last);
		AddVarImpl(arg,typ);
	}

	__forceinline VOID AddVar_Box_range(TViewerCallback0 hitbox, TViewerCallback0 bounds, UINT Range, const ULONG First, const ULONG Last)
	{
		tAddVarArg arg;
		arg.ArgFlags = tAddVarArg::e_BOX;
		arg.box.Cb0[0] = bounds;
		arg.box.Cb0[1] = hitbox;
		arg.box.Range = Range;
		TYPES_ARG typ;
		typ.Range(First, Last);
		AddVarImpl(arg,typ);
	}


	__forceinline VOID AddVar(const TYPES_ARG &typ)
	{
		tAddVarArg arg;
		arg.ArgFlags = 0;
		AddVarImpl(arg,typ);
	}

	__forceinline VOID AddVar(PCSTR Name, TViewerCallback0 Cb0, const TYPES_ARG &typ)
	{
		tAddVarArg arg;
		arg.ArgFlags = tAddVarArg::e_VAR;
		arg.var.Name = Name;
		arg.var.Cb0 = Cb0; // PreDrawCb if Name NULL
		AddVarImpl(arg,typ);
	}

	__forceinline VOID AddVar_View(PCSTR Name, ULONG Flags, TViewerCallback0 Cb0, const TYPES_ARG &typ)
	{
		tAddVarArg arg;
		arg.ArgFlags = tAddVarArg::e_VAR | tAddVarArg::e_VAR_FLAGS;
		arg.var.Name = Name;
		arg.var.Cb0 = Cb0;
		arg.var.ViewFlags = Flags;
		AddVarImpl(arg,typ);
	}

	__forceinline VOID AddVar_BOOL(PCSTR Name, TViewerCallback0 Cb0, const TYPES_ARG &typ)
	{
		tAddVarArg arg;
		arg.ArgFlags = tAddVarArg::e_VAR_TYPE | tAddVarArg::e_VAR;
		arg.var.Name = Name;
		arg.var.Cb0 = Cb0;
		arg.var.VarType = CVAR_BOOL;
		AddVarImpl(arg,typ);
	}

	__forceinline VOID AddVar_Box(TViewerCallback0 hitbox, TViewerCallback0 bounds, UINT Range, const TYPES_ARG &typ)
	{
		tAddVarArg arg;
		arg.ArgFlags = tAddVarArg::e_BOX;
		arg.box.Cb0[0] = bounds;
		arg.box.Cb0[1] = hitbox;
		arg.box.Range = Range;
		AddVarImpl(arg,typ);
	}

	__forceinline VOID SetFlags(ULONG Flags, const TYPES_ARG &typ) // IESP_VarView::FLAG__SCREEN_REAL_SCREEN_POS
	{
		tAddVarArg arg;
		arg.ArgFlags = tAddVarArg::e_VAR_FLAGS;
		arg.var.ViewFlags = Flags;
		AddVarImpl(arg,typ);
	}

	__forceinline VOID Inherit(const TYPES_ARG &typ)
	{
		tAddVarArg arg;
		arg.ArgFlags = tAddVarArg::e_INHERIT;
		AddVarImpl(arg,typ);
	}

public:

	struct IEspBar
	{
		XXCOLOR Color;
		XXCOLOR Background;
	};
	virtual IEspBar *Bar_Register(ULONG id, PCSTR Name) = 0;
	virtual ULONG _Bar_Draw(ULONG id, PINT pVar, const TObject &obj) const = 0;

	__forceinline XSIZE Bar_Draw(ULONG id, PINT pMinMax, const TObject &obj) const
	{
		XSIZE x;
		x.u = _Bar_Draw(id, pMinMax, obj);
		return x;
	}

public:
	virtual VOID ZEnable(ULONG) = 0; // ZFLG_*
};


//--------------------------------------------------------


class PROTOTYPE SDK_NAMES
{
public:
	virtual VOID Release() = 0;
	virtual LONG IsNub(PCSTR) = 0;

public:
	class PROTOTYPE Callback
	{
	public:
		class PROTOTYPE AddNew
		{
		public:
			virtual VOID NewNub(PCSTR) = 0;
		};
		virtual VOID OnUpdate(AddNew *) = 0; // while(names) AddNew.Add()
	};
};


//--------------------------------------------------------


class PROTOTYPE SDK_LOCATIONS
{
public:
	virtual void Release() = 0;

public:
	class PROTOTYPE Callback
	{
	public:
		virtual BOOL GetPos(Vector &) = 0;
		virtual VOID SetPos(const Vector &) = 0;
	};
};


//--------------------------------------------------------


class PROTOTYPE SDK_BONES
{
public:
	struct TBONEID
	{
		ULONG	Model;
		UCHAR	Ready;
		UCHAR	_[3];
		USHORT	ID[16];
	};
	struct TBONEIDVIEW
	{
		ULONG	Model;
		UCHAR	Ready;
		UCHAR	_[3];
		USHORT	ID[6][HUD_MAX_POINTS];
	};
	struct BONESHOLDER
	{
		SIZE_T	p2[2];
		SIZE_T	p4[4];

#ifndef _ENGINEDLL
		__forceinline VOID Clear() { sdk._Bones->Clear(this); }
#endif
	};

	typedef bool (__fastcall *tBonesUpdate)(TBONEID *Arg, PVOID ObjPtr);
	typedef bool (__fastcall *tBonesViewUpdate)(TBONEIDVIEW *Arg, PVOID ObjPtr);

public:
	virtual VOID Release() = 0;
	virtual VOID Clear(SDK_BONES::BONESHOLDER *) = 0;

	virtual TBONEID *GetBones(ULONG Model, BONESHOLDER *, tBonesUpdate, PVOID ObjPtr) = 0;
	virtual UINT GetBonesCount() = 0;

	virtual VOID OnUpdateBones(TBONEID *Arg, PCSTR BoneName, ULONG BoneID) = 0;
	virtual TBONEIDVIEW *GetBonesView(ULONG Model, BONESHOLDER *, tBonesViewUpdate, PVOID ObjPtr) = 0;
	virtual UINT GetBonesViewCount(UINT) = 0;
	virtual VOID OnUpdateBonesView(TBONEIDVIEW *Arg, PCSTR BoneName, ULONG BoneID) = 0;
};


//--------------------------------------------------------


class PROTOTYPE SDK_RADAR
{
public:
	cGUIMainWindow *m_Window; // window m_arg:	TCommandFn m_Draw;
	cGUIMainWindow *m_Window2;

	XXCOLOR	m_Background, m_Border;
	Vector	m_Origin;
	FLOAT	m_Range;
	ULONG	m_Size;
	UCHAR	m_Swap;
	UCHAR	m_NoClamp;
	UCHAR	__pad;

	enum e_Args
	{
		e_ptr_iXY		= (1 << 0),
		e_ptr_Vec3		= (1 << 1),
		e_ptr_Object	= (1 << 2),
		e_Color		= (1 << 3),
		e_Alpha		= (1 << 4),
		e_Size		= (1 << 5),
		e_Outline	= (1 << 6),
		e_RetXY		= (1 << 7),
	};
	struct t_Arg
	{
		union {
			const INT *pXY;
			const Vector *pVec3;
			TObject *pObject;
		};
		union {
			ULONG Color;
			UCHAR Alpha;
		};
		ULONG Size;
		PVOID Ret;
	};

public:
	virtual BOOL ShouldDraw(ULONG TypeId, OUT XFONT *Font = NULL) = 0;
	virtual VOID DrawFrame() = 0;
	virtual VOID _vectorcall Update(const Vector &, FLOAT Yaw) = 0;

	virtual VOID Req(ULONG, LPCVOID) const = 0;

	__forceinline VOID DrawObject(PObject pObj, const bool Outline = 0, const UCHAR Alpha = 0) const
	{
		if(Alpha)
		{
			t_Arg arg;
			arg.pObject = pObj;
			arg.Alpha = Alpha;
			ULONG e = e_ptr_Object | e_Alpha;
			if(Outline) e |= e_Outline;
			Req(e,&arg);
		}
		else
		{
			ULONG e = e_ptr_Object;
			if(Outline) e |= e_Outline;
			Req(e,pObj);
		}
	}

	__forceinline VOID DrawPoint(const INT *xy, ULONG Color, const ULONG Size = 0, const UCHAR Outline = 0) const
	{
		t_Arg arg;
		arg.pXY = xy;
		arg.Color = Color;
		ULONG e = e_ptr_iXY | e_Color;
		if(Size) { arg.Size = Size; e |= e_Size; }
		if(Outline) { e |= e_Outline; }
		Req(e,&arg);
	}

	__forceinline VOID DrawPoint(const Vector &Vec, ULONG Color, const ULONG Size = 0, const UCHAR Outline = 0) const
	{
		t_Arg arg;
		arg.pVec3 = &Vec;
		arg.Color = Color;
		ULONG e = e_ptr_Vec3 | e_Color;
		if(Size) { arg.Size = Size; e |= e_Size; }
		if(Outline) { e |= e_Outline; };
		Req(e,&arg);
	}

	__forceinline VOID CalcPointByOrigin(const Vector &Vec, PVOID pXY) const
	{
		t_Arg arg;
		arg.pVec3 = &Vec;
		arg.Ret = pXY;
		Req(e_ptr_Vec3|e_RetXY, &arg);
	}

	virtual VOID AddView(PCSTR Name, TViewerCallback0 Cb, Cvar_Types Ct = CVAR_NONE, PCvar pV = NULL) = 0;
	virtual VOID Register(ULONG range, TCommandFn DrawFn) = 0;
	virtual VOID Release() = 0;
};




//--------------------------------------------------------


class PROTOTYPE SDK_ENTS
{
public:
	virtual VOID	Release() = 0;
	virtual VOID	Frame(ULONG _Handle, ULONG Delay) = 0;
	virtual PVOID	Get(ULONG _Handle, PVOID Key) = 0;
	virtual PVOID	Find(ULONG _Handle, PVOID Key, ULONG Delay) = 0;
	virtual PVOID	Init(ULONG Count, ULONG SizeOfEnt, PULONG OUT _Handle) = 0;
};


template<class T, ULONG MAXc>
struct TEntMap
{
	ULONG	_Handle;
	ULONG	_Count;
	T*		_Data;
	T*		_Prev;
	T*		_List[MAXc];
	ULONG	_Frame;
	ULONG	_Trash;

	__forceinline void Init()
	{
		SDK_ENTS *ents = sdk._Ents;
		if(!ents)
			ents = sdk._Ents = g_SDK->Ents();
		_Frame = 1;
		_Data = (T*)ents->Init(MAXc, sizeof(T), &_Handle);
	}

	inline T* GetImpl(PVOID pKey, bool* IsDuplicate)
	{
		if(_Count >= MAXc)
			return NULL;

		T* Ptr = (T*)sdk._Ents->Get(_Handle, pKey);
		if(!Ptr)
			return Ptr;

		if(Ptr->Frame == _Frame)
		{
			if(!IsDuplicate)
				return NULL;
			*IsDuplicate = 1;
		}
		else
		{
			Ptr->Frame = _Frame;
			_List[_Count++] = Ptr;
		}

		return Ptr;
	}

	inline T* Get(PVOID pKey, bool* IsDuplicate = NULL)
	{
		return _Prev = GetImpl(pKey, IsDuplicate);
	}

	inline T* FindPrev(PVOID pKey, const ULONG Delay = 200) const
	{
		return (T*)sdk._Ents->Find(_Handle, pKey, Delay);
	}
	inline T* Find(PVOID pKey) const
	{
		T* p = FindPrev(pKey);
		return (p && (p->Frame == _Frame)) ? p : NULL;
	}

	inline VOID	Frame(const ULONG Delay)
	{
		sdk._Ents->Frame(_Handle, Delay);
		_Prev = 0;
		_Count = 0;
		_Frame += 1;
	}
};


//--------------------------------------------------------


class PROTOTYPE IHagSdk
{
public:
	virtual BOOL OnInit(tH3ESDK &sdk, ULONG Version = H3E_SDK_VERSION) = 0; // init tH3ESDK_base, fail on wrong version
	virtual VOID OnRelease(tH3ESDK &sdk, LONG Step) = 0;
	virtual VOID OnUpdateScreen(const Matrix4x4 *Matrix, const Matrix4x4 *MatrixTranspose, const BOOL swapZY = 0) = 0;

	virtual BOOL RegisterObjectType(ULONG TypeId, PCSTR TypeName) = 0;
	virtual XXCOLOR* FindObjectType(ULONG TypeId) = 0;

	virtual SDK_RADAR *Radar() = 0;
	virtual SDK_ESP *Esp() = 0;
	virtual SDK_ENTS *Ents() = 0;
	virtual SDK_BONES *Bones() = 0;
	virtual SDK_NAMES *Names(SDK_NAMES::Callback *) = 0;
	virtual SDK_LOCATIONS *Locations(SDK_LOCATIONS::Callback *) = 0;

	virtual cTexture *CreateBeam() = 0;
};

extern IHagSdk *g_SDK;


inline ULONG GetColor(const ULONG i)
{
	XXCOLOR *p = g_SDK->FindObjectType(i);
	return p ? (*p) : ~0;
}

inline VOID SetColor(const ULONG i, const ULONG c)
{
	XXCOLOR *p = g_SDK->FindObjectType(i);
	if(p) *p = c;
}



//////////////////////////////////

#pragma pack(pop)
