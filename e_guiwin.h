#pragma once


__forceinline bool IsInside(INT x, INT y, INT w, INT h)
{
	//return	x >= 0	&&
	//		y >= 0	&&
	//		x <= w	&&
	//		y <= h	;

	return (ULONG(x) <= ULONG(w)) && (ULONG(y) <= ULONG(h));
}



#pragma pack(push, 1)


class PROTOTYPE cGUIColor 
{
public:
	virtual VOID Release() = 0;
	virtual cGUIColor *CreateGroup() = 0;

	struct t_arg
	{
		XXCOLOR *pColor;	// 2
		PCSTR Name;			// 1
		enum e_act { color_set = (1), color_delptr = (2), color_add = (1|2), color_del = (1|4) } _act;
		__forceinline t_arg(const e_act a) : _act(a) {}
		ULONG Color;
	};
	virtual bool Do(const t_arg &) = 0;

	inline bool Add(PCSTR Name, XXCOLOR *pColor, const ULONG Color = 0)
	{
		t_arg arg(t_arg::color_add);
		arg.Name = Name;
		arg.pColor = pColor;
		arg.Color = Color;
		return Do(arg);
	}
	inline bool Set(PCSTR Name, ULONG Color)
	{
		t_arg arg(t_arg::color_set);
		arg.Name = Name;
		arg.Color = Color;
		return Do(arg);
	}
	inline bool Del(PCSTR Name)
	{
		t_arg arg(t_arg::color_del);
		arg.Name = Name;
		return Do(arg);
	}
	inline bool DelPtr(XXCOLOR *pColor)
	{
		t_arg arg(t_arg::color_delptr);
		arg.pColor = pColor;
		return Do(arg);
	}
};


class PROTOTYPE cGUIFontMan
{
public:
	virtual BOOL Register(PXFONT pFont, PCSTR Caption, XGUIFONTQUERY *Req = NULL) = 0;
	virtual BOOL Delete(PXFONT pFont) = 0;

	inline BOOL Register(PXFONT pFont, PCSTR Caption, PCSTR Name, const UCHAR Tall, const UINT Weight = 0, const UCHAR Blur = 0, const UCHAR Flags = 0)
	{
		XGUIFONTQUERY reg(Tall, Flags, Weight ? 1 : 0, Blur);
		_lstrncpy(reg.NameBuff, Name, ARRAY_COUNT(reg.NameBuff)-1 );
		return Register(pFont, Caption, &reg);
	}
};


//--------------------------------------------------------------


struct GUIRECT : public RECT
{
	__forceinline VOID Set(LONG c)
	{
		left = top = right = bottom = c;
	}

	__forceinline int DW() const { return left + right; }
	__forceinline int DH() const { return top + bottom; }

	__forceinline bool IsNull() const
	{
		return (*(PULONG64)&left == 0) && (*(PULONG64)&right == 0);		//	return !( left | top | right | bottom );
	}

	inline bool PtIn(LONG x, LONG y) const
	{
		return	x >= left &&
				y >= top &&
				x <  right &&
				y <  bottom;
	}
};


#define UNKNOWN_WINDOW  ((cGUIMainWindow *)~0)


class GUIBASE
{
public:
	UCHAR				m_Type;
	UCHAR				m_Focus;
	UCHAR				m_State;
	UCHAR				m_FontsChanged; // GUI notify

	ULONG				m_Style;

	cGUIWindow *		m_ParentWindow;

	LONG				m_X;
	LONG				m_Y;
	LONG				m_W;
	LONG				m_H;

	//GUIRECT				m_Margin;
	//GUIRECT				m_Border;
	GUIRECT				m_Padding;

	XFONT				m_Font;

	XSIZE				m_CaptionSize;
	cString *			m_Caption;
	cString *			m_Tip;

	GuiCallbackFn		m_Callback;
	PVOID				m_ARG;

	inline VOID SetCaption(PCSTR Caption)
	{
		if(!Caption)	Str_Release(&m_Caption);
		else			Str_Alloc(&m_Caption, Caption);
	}
	inline VOID SetTip(PCSTR Tip)
	{
		if(!Tip)	Str_Release(&m_Tip);
		else		Str_Alloc(&m_Tip, Tip);
	}

	__forceinline VOID Disable(const bool x)
	{
		if(x)	m_Style |= mmWinDisabled;
		else	m_Style &= ~mmWinDisabled;
	}
	__forceinline VOID Hide(const bool x)
	{
		if(x)	m_Style |= mmWinInvisible;
		else	m_Style &= ~mmWinInvisible;
	}

	__forceinline UCHAR QueryFocus() const
	{
		return m_Focus;
	}

	__forceinline VOID UpdateAsync()
	{
		m_FontsChanged = 1;
	}
	__forceinline bool Updated() const
	{
		return *(volatile UCHAR *)&m_FontsChanged == 0;
	}

};


//---------------------------------------------------------------
//---------------------------------------------------------------


class PROTOTYPE cGUIWindow : public GUIBASE
{
public:
	cGUIMainWindow *GetMainWindow();

	POINT __Pos() const noexcept;

	VOID ToScreen(INT &x, INT &y) const noexcept;
	VOID ToClient(INT &x, INT &y) const noexcept;

	XSIZE DrawCaption(XFONT Font, INT x, INT y, ULONG color, const ULONG Flags = HUD_STRING_CENTERY);

public:
	virtual VOID Release() = 0;
	virtual	VOID Draw() = 0;
	virtual	VOID SetFocus() = 0;
	virtual	VOID RemoveFocus() = 0;
	virtual	BOOL CheckClick(INT x, INT y) = 0;
	virtual	BOOL MouseMove(CPOS &) = 0;
	virtual	BOOL MousePress(CPOS &, UINT Mouse, UINT Down) = 0;
	virtual	BOOL KeyPress(CMSG &) = 0;
};



//--------------------------------------------------------------


class cGUITaskbar
{
public:
	UCHAR m_Position;
	UCHAR m_Dragging;
	UCHAR m_ReqUpdate;
	UCHAR __;
	XFONT m_Font;
};


//--------------------------------------------------------------


struct RESIZE
{
	enum e
	{
		X			= (1 <<  0), // use arg X
		Y			= (1 <<  1), // use arg Y
		W			= (1 <<  2), // use arg W
		H			= (1 <<  3), // use arg H
		RIGHT		= (1 <<  4), // x=X+W
		RIGHT_W		= (1 <<  5), // x=X+W-w
		LEFT		= (1 <<  6), // 
		TOP			= (1 <<  7), // top+h+step
		BOTTOM		= (1 <<  8), // 
		AUTO_W		= (1 << 10), // 
		AUTO_H		= (1 << 11), // 
		CONTROL_W	= (1 << 12), // w = wnd->m_W
		CONTROL_H	= (1 << 13), // h = wnd->m_H
		XY			= (X|Y),	 // 
		WH			= (W|H),	 // 
		ALL			= (X|Y|W|H), // 
		NONE		= 0
	};
};


class PROTOTYPE cGUIResizer
{
public:
	struct t_resizer // size: 24
	{
		cGUIWindow *Window;
		cGUIWindow *Near;
		ULONG	Flags;
		INT		Step; // D

		__forceinline ULONG GetWindowFlags() const { return Flags; }
		__forceinline ULONG GetNearFlags() const { return Flags >> 16; }

		__forceinline VOID SetFlags(const ULONG wndF, const ULONG nearF = 0)
		{
			Flags = wndF | (nearF << 16);
		}
	};
	struct t_resizer_arg : public t_resizer // tmp
	{
		INT		x, y;
		UINT	w, h;

		VOID Resize();
	};

	virtual VOID Add(t_resizer_arg &) = 0;
	virtual VOID Release() = 0;
	virtual VOID Begin(cGUIWindow *, ULONG Count) = 0;
	virtual VOID End(LONG D, LONG W, LONG H) = 0;
	virtual VOID Fix() = 0;

	__forceinline VOID Add(cGUIWindow *Window, const ULONG WindowFlags, const INT X, const INT Y, const UINT W, const UINT H, const INT D = 0, cGUIWindow *Near = 0, const ULONG NearFlags = RESIZE::NONE)
	{
		t_resizer_arg arg;
		t_resizer_arg * p = (t_resizer_arg *)ptr_f_no_opt(&arg);
		p->x = X;
		p->y = Y;
		p->Window = Window;
		p->Near = Near;
		p->SetFlags(WindowFlags, NearFlags);
		p->Step = D;
		p->w = W;
		p->h = H;
		Add(arg);
	}
};


class PROTOTYPE cGUIMainWindow : public cGUIWindow
{
public:
	virtual	cGUIWindow *Add(eGuiWin) = 0;
	virtual	cGUIWindow *Find(cGUIWindow *) const = 0;

	virtual cGUIResizer *GetResizer() = 0;

	__forceinline UINT GetHeaderHeight() const { return UINT(m_CaptionSize.cy) + 5; }

	__forceinline cGUILabel *		Add_Label()		{ return (cGUILabel *)		Add(mmWinLabel);		}
	__forceinline cGUICheckBox *	Add_CheckBox()	{ return (cGUICheckBox *)	Add(mmWinCheckBox);		}
	__forceinline cGUIButton *		Add_Button()	{ return (cGUIButton *)		Add(mmWinButton);		}
	__forceinline cGUITextBox *		Add_TextBox()	{ return (cGUITextBox *)	Add(mmWinTextBox);		}
	__forceinline cGUIScroller *	Add_Scroller()	{ return (cGUIScroller *)	Add(mmWinScroller);		}
	__forceinline cGUIColorArea *	Add_ColorArea()	{ return (cGUIColorArea *)	Add(mmWinColorArea);	}
	__forceinline cGUIList *		Add_List()		{ return (cGUIList *)		Add(mmWinList);			}
	__forceinline cGUITab *			Add_Tab()		{ return (cGUITab *)		Add(mmWinTab);			}
	__forceinline cGUIRadio *		Add_Radio()		{ return (cGUIRadio *)		Add(mmWinRadio);		}
	__forceinline cGUISlider *		Add_Slider()	{ return (cGUISlider *)		Add(mmWinSlider);		}
	__forceinline cGUITree *		Add_Tree()		{ return (cGUITree *)		Add(mmWinTree);			}

	virtual BOOL		SetMenu(cGUIWindow *, cGUIMenu *) = 0;
	virtual cGUIMenu *	GetMenu(cGUIWindow *) = 0;
};


//--------------------------------------------------------------


class PROTOTYPE cGUIButton : public cGUIWindow
{
public:
	typedef VOID (__fastcall *tGUIButtonCb)(cGUIButton *, PVOID);

	tGUIButtonCb	m_OnClick;
	PVOID			m_OnClickArg;

	__forceinline VOID SetCallback(tGUIButtonCb p, PVOID a)
	{
		m_OnClick = p;
		m_OnClickArg = a;
	}
};



class PROTOTYPE cGUICheckBox : public cGUIWindow
{
public:
	virtual BOOL GetCheck() const = 0;
	virtual VOID SetCheck(BOOL) = 0;
	virtual	VOID SetValuePtr(PINT ptr, UINT Bits) = 0;
};



class PROTOTYPE cGUIColorArea	: public cGUIWindow
{
public:
	//virtual VOID GetColor_RGBA(PULONG) = 0;
	//virtual VOID GetColor_HSVA(PULONG) = 0;
	//virtual VOID SetColor_RGBA(PULONG, ULONG) = 0;
	//virtual VOID SetColor_HSVA(PULONG, ULONG) = 0;
};



class PROTOTYPE cGUILabel : public cGUIWindow
{
};



class PROTOTYPE cGUILine : public cGUIWindow
{
};




class PROTOTYPE cGUIList : public cGUIWindow
{
public:

	inline LPCVOID GetData(ULONG Index) const
	{
		TListItemArg *pItem = Get(Index);
		return pItem ? pItem->Data : 0;
	}

	inline VOID SetData(ULONG Index, LPCVOID Data)
	{
		TListItemArg *pItem = Get(Index);
		if(pItem) pItem->Data = Data;
	}

	inline INT_PTR GetValue(ULONG Index) const
	{
		TListItemArg *pItem = Get(Index);
		return pItem ? pItem->Value : 0;
	}

	inline VOID SetValue(ULONG Index, INT_PTR Value)
	{
		TListItemArg *pItem = Get(Index);
		if(pItem) pItem->Value = Value;
	}

	inline ULONG GetFlags(ULONG Index) const
	{
		TListItemArg *pItem = Get(Index);
		return pItem ? pItem->Flags : 0;
	}

	inline VOID SetFlags(ULONG Index, ULONG Flags)
	{
		TListItemArg *pItem = Get(Index);
		if(pItem) pItem->Flags = Flags;
	}

	inline ULONG GetText(ULONG Index, PSTR Buff, ULONG Limit) const
	{
		TListItemArg *pItem = Get(Index);
		return pItem ? Str_Get((cString*)pItem->TextPtr, Buff, Limit) : 0;
	}

	inline VOID SetText(ULONG Index, PCSTR Text) const
	{
		TListItemArg *pItem = Get(Index);
		if(pItem) Str_Alloc((cString**)&pItem->TextPtr, Text);
	}

public:
	struct TListItemArg
	{
		LPCVOID		Data;
		LPCVOID		TextPtr; // cString* or char*
		INT_PTR		Value;
		ULONG		Color;
		ULONG		Background;
		XFONT		Font;
		ULONG		Flags;
	};
	virtual TListItemArg * Get(ULONG Index) const = 0;
	virtual cGUICheckBox * GetCheckBox(ULONG Index) const = 0;

	__forceinline ULONG Insert(ULONG Index, cString *Text, LPCVOID Data = NULL, LONG_PTR Value = 0, const ULONG Color = 0, const ULONG Bg = 0, const XFONT Font = 0, const PINT ValuePtr = 0, const INT ValueBit = 0)
	{
		TListItemArg Arg;
		Arg.Data = Data;
		Arg.TextPtr = Text;
		Arg.Value = Value;
		Arg.Color = Color;
		Arg.Background = Bg;
		Arg.Font = Font;
		Arg.Flags = 0;
		return Insert(Index, Arg, ValuePtr, ValueBit);
	}

	__forceinline ULONG	Add(cString *Text, LPCVOID Data, LONG_PTR Value = 0, const ULONG Color = 0, const ULONG Bg = 0, const XFONT Font = 0, const PINT ValuePtr = 0, const INT ValueBit = 0)
	{
		return Insert(~0, Text, Data, Value, Color, Bg, Font, ValuePtr, ValueBit);
	}

	enum e_Find
	{
		LIST_FIND_TEXT			= (1 << 0),
		LIST_FIND_DATA			= (1 << 1),
		LIST_FIND_VALUE			= (1 << 2),
		LIST_FIND_FROM_ID		= (1 << 3),
		LIST_FIND_TEXT_NOCASE	= (1 << 4),
		LIST_FIND_DELETE		= (1 << 5),
	};

	__forceinline ULONG Find(PCSTR pText, const bool bCaseSensitive = TRUE, const LPCVOID pData = NULL, const bool bDelete = FALSE, const ULONG From = 0)
	{
		TListItemArg Arg;
		ULONG Flags = (bDelete ? LIST_FIND_DELETE : 0);

		if(From)
		{
			Arg.Color = From;
			Flags |= LIST_FIND_FROM_ID;
		}

		if(pData)
		{
			Arg.Data = pData;
			Flags |= LIST_FIND_DATA;
		}

		if(pText)
		{
			Arg.TextPtr = pText;
			Flags |= LIST_FIND_TEXT | (bCaseSensitive ? 0 : LIST_FIND_TEXT_NOCASE);
		}

		Arg.Flags = Flags;

		return Find(Arg);
	}

	inline ULONG FindByData(LPCVOID pData)
	{
		TListItemArg Arg;
		Arg.Data = pData;
		Arg.Flags = LIST_FIND_DATA;
		return Find(Arg);
	}

	inline ULONG FindByValue(INT_PTR Value)
	{
		TListItemArg Arg;
		Arg.Value = Value;
		Arg.Flags = LIST_FIND_VALUE;
		return Find(Arg);
	}

public:
	virtual VOID	Clear() = 0;
	virtual ULONG	Size() const = 0;
	virtual ULONG	Selected() const = 0;
	virtual ULONG	Hover() const = 0;
	virtual BOOL	Select(ULONG) = 0;
	virtual ULONG	Insert(ULONG Index, TListItemArg &, PINT ValuePtr, UINT ValueBits) = 0; // ValuePtr/ValueBits for checkbox
	virtual ULONG	Find(const TListItemArg &) = 0; // ~0 on error
	virtual BOOL	Move(ULONG From, ULONG To) = 0;
	virtual ULONG	Del(ULONG Index) = 0;

	inline BOOL GetCheck(ULONG Index) const
	{
		cGUICheckBox *pCheck = GetCheckBox(Index);
		return pCheck ? pCheck->GetCheck() : 0;
	}

	inline VOID SetCheck(ULONG Index, BOOL Check) const
	{
		cGUICheckBox *pCheck = GetCheckBox(Index);
		if(pCheck) pCheck->SetCheck(Check);
	}

	inline VOID ValuePtr_Checkbox(ULONG Index, PINT Check, UINT Bits) const
	{
		cGUICheckBox *pCheck = GetCheckBox(Index);
		if(pCheck) pCheck->SetValuePtr(Check, Bits);
	}
};





//--------------------------------------------------------------


#define TREE_ELEM_HIDDEN	(1 << 1)

class cGuiTreeElem;


class PROTOTYPE cGUITree : public cGUIWindow
{
public:
	virtual	cGuiTreeElem *	GetRootElem() const = 0;
	virtual	cGuiTreeElem *	GetSelectedElem() const = 0;
	virtual	cGuiTreeElem *	GetElemAt(POINT &) const = 0;
	virtual	cGuiTreeElem *	FindData(LPCVOID) const = 0;

	virtual	VOID	ScrollTo(cGuiTreeElem *) = 0;
	virtual	VOID	SelectElem(cGuiTreeElem *) = 0;

	virtual	VOID	Clear() = 0;
	virtual	UINT	Count(BOOL Force) = 0;
	virtual	VOID	RecalcSize() = 0;
	virtual	VOID	GetSize(PULONG) = 0;

	virtual VOID	SetMultiSelect(bool) = 0;
	virtual bool	GetMultiSelect() const = 0;
};


class PROTOTYPE cGuiTreeElemCB
{
public:
	virtual VOID OnRelease(cGuiTreeElem *)								= 0;
	virtual BOOL OnCalcRect(cGuiTreeElem *, SIZE &)						= 0;
	virtual BOOL OnPaint(cGuiTreeElem *, RECT &)						= 0;
	virtual BOOL OnMouse(cGuiTreeElem *, CPOS &, UINT Mouse, UINT Down)	= 0;
	virtual BOOL OnExpand(cGuiTreeElem *, bool NewState)				= 0;
	virtual VOID OnContextMenu(cGuiTreeElem *, CPOS &)					= 0;
};


class PROTOTYPE cGuiTreeElem
{
public:
	SIZE				Size;
	GUIRECT				Rect;
	cGuiTreeElemCB *	Callback;
	PVOID				Data;
	ULONG				Flags;
	ULONG				Level;
	cGuiTreeElem *		Parent;
	cGUITree *			_Ctl;

	__forceinline BOOL IsHidden() const
	{
		return (Flags & TREE_ELEM_HIDDEN);
	}

	__forceinline VOID Hide(const bool b)
	{
		if(b)	Flags |= TREE_ELEM_HIDDEN;
		else	Flags &= ~TREE_ELEM_HIDDEN;
	}

	__forceinline VOID SetSize(const SIZE &s)
	{
		Size = s;
		Update();
	}

	__forceinline VOID Update() const
	{
		_Ctl->UpdateAsync();
	}

	__forceinline cGUITree *GetCtl() const
	{
		return _Ctl;
	}

public:
	virtual VOID	Release() = 0;

	virtual VOID	ClearList() = 0;	// delete list

	virtual cGuiTreeElem *	Insert(cGuiTreeElem *InsertBefore = NULL) = 0;
	virtual cGuiTreeElem *	GetLast() const = 0;
	virtual cGuiTreeElem *	GetFirst() const = 0;
	virtual cGuiTreeElem *	GetNext(cGuiTreeElem *) const = 0;
	virtual cGuiTreeElem *	GetPrev(cGuiTreeElem *) const = 0;

	virtual VOID	SetText(PCSTR, ULONG Len = 0) = 0;
	virtual ULONG	GetText(PSTR, ULONG) const = 0;

	virtual VOID	Expand(bool) = 0;	// alloc list
	virtual bool	IsExpanded() const = 0;
	virtual bool	IsSelected() const = 0;

	virtual bool	IsList(PSIZE_T elems = NULL) const = 0;
};


//--------------------------------------------------------------


class PROTOTYPE cGUIScroller : public cGUIWindow
{
public:
	virtual VOID ValueFromScroll() = 0;
	virtual BOOL ScrollFromValue() = 0;

	struct tScrollData
	{
		LONG	_Value;	// pos
		LONG	_NobPos;
		LONG	_Range; // 1 page size
		LONG	_Length; // H
		LONG	_NobSize;
		LONG	_Min;
		LONG	_Max;
		LONG	__;
	};
	tScrollData Data;

	inline LONG GetValue() const
	{
		return Data._Value;
	}

	__forceinline void SetValueImpl(LONG v)
	{
		Data._Value = Clamp<LONG>(v, Data._Min, Data._Max);
	}
	inline void SetValue(LONG v)
	{
		SetValueImpl(v);
		ScrollFromValue();
	}

	inline void GetRange(LONG *p) const
	{
		p[0] = Data._Min;
		p[1] = Data._Max;
	}
	inline void SetRange(LONG iMin, LONG iMax)
	{
		Data._Max = Max<LONG>(iMin, iMax);
		Data._Min = Min<LONG>(iMin, iMax);
		ScrollFromValue();
	}
};



class PROTOTYPE cGUITextBox : public cGUIWindow
{
public:
	virtual ULONG	Add(PCSTR) = 0;
	virtual UINT	Get(ULONG line, PSTR, UINT sz) const = 0;
	virtual UINT	Set(ULONG line, PCSTR) = 0;
	virtual UINT	Set(ULONG line, cString *) = 0; //
	virtual ULONG	GetLines() const = 0;
	virtual ULONG	GetLine() const = 0;
	virtual VOID	SetLine(ULONG) = 0;
	virtual VOID	Clear() = 0;
	virtual VOID	SetLimit(UINT sz) = 0;
	virtual UINT	GetLimit() const = 0;
};



class PROTOTYPE cGUIMenu : public cGUIWindow
{
public:
	struct TEntry
	{
		__forceinline TEntry() {}
		__forceinline TEntry(cString *str) : Text(str) { *(PULONG)&Disabled = 0; }
		cString *	Text;
		cString *	Tip;
		PVOID		Data;
		UCHAR		Disabled;
		UCHAR		Hidden;
		UCHAR		_PAD[2];
	};
	__forceinline ULONG	Insert(ULONG Index, cString *Text, cString *const Tip = NULL, const PVOID Data = NULL)
	{
		TEntry Arg(Text);
		*(volatile void**)&Arg.Data = Data;
		Arg.Tip = Tip;
		return Insert(Index, Arg);
	}
	__forceinline ULONG	Add(cString *Text, cString *Tip = NULL, const PVOID Data = NULL)
	{
		return Insert(~0, Text, Tip, Data);
	}

public:
	virtual VOID 	Clear() = 0;
	virtual ULONG	Size() const = 0;
	virtual ULONG	Selected() const = 0;
	virtual ULONG	Insert(ULONG Index, TEntry &) = 0; // insert before
	virtual ULONG	Del(ULONG Index) = 0;
	virtual TEntry *Get(ULONG Index) const = 0;

	
	VOID Disable(ULONG Index, const UCHAR v)
	{
		TEntry *pItem = Get(Index);
		if(!pItem)
			return;
		if(v>2)	pItem->Hidden = (v & 1);
		else	pItem->Disabled = (v);
		UpdateAsync();
	}

	__forceinline PVOID GetData(ULONG Index)
	{
		TEntry *pItem = Get(Index);
		return pItem ? pItem->Data : NULL;
	}
	//inline VOID SetData(ULONG Index, PVOID Data)
	//{
	//	TEntry *pItem = Get(Index);
	//	if(pItem) pItem->Data = Data;
	//}

	//inline ULONG GetText(ULONG Index, PSTR Buff, ULONG Limit)
	//{
	//	TEntry *pItem = Get(Index);
	//	return pItem ? Str_Get(pItem->Text, Buff, Limit) : 0;
	//}
	//inline VOID SetText(ULONG Index, PCSTR Text)
	//{
	//	TEntry *pItem = Get(Index);
	//	if(pItem) Str_Alloc(&pItem->Text, Text);
	//}

	//inline ULONG GetTip(ULONG Index, PSTR Buff, ULONG Limit)
	//{
	//	TEntry *pItem = Get(Index);
	//	return pItem ? Str_Get(pItem->Tip, Buff, Limit) : 0;
	//}
	//inline VOID SetTip(ULONG Index, PCSTR Text)
	//{
	//	TEntry *pItem = Get(Index);
	//	if(pItem) Str_Alloc(&pItem->Tip, Text);
	//}

};



class PROTOTYPE cGUITab : public cGUIWindow
{
public:
	virtual	cGUIMainWindow *Add() = 0;
	virtual bool Del(cGUIMainWindow *) = 0;

	virtual cGUIMainWindow *SetCurrent(cGUIMainWindow *pSwitchTo) = 0;

	virtual cGUIMainWindow *Find(PCSTR Text, PVOID Data = NULL) const = 0;
	virtual	cGUIMainWindow *GetNext(cGUIMainWindow *pPrev) const = 0;

	virtual ULONG TabsCount() const = 0;
	virtual ULONG TabsHeight() const = 0;

	virtual VOID Clear() = 0;


	__forceinline cGUIMainWindow *GetFirst() const
	{
		return GetNext(NULL);
	}

	__forceinline cGUIMainWindow *GetCurrent()
	{
		return SetCurrent(NULL);
	}

	cGUIMainWindow *Insert(PCSTR Caption, PVOID Data)
	{
		cGUIMainWindow *pwnd = Add();
		if(!pwnd)
			return pwnd;
		pwnd->m_ARG = Data;
		if(Caption)
			pwnd->SetCaption(Caption);
		return pwnd;
	}
};


//--------------------------------------------------------------

class PROTOTYPE cGUIRadio : public cGUIWindow
{
	LONG _Value;

public:
	virtual BOOL	GetCheck() const = 0;
	virtual VOID	SetCheck() = 0;
	virtual	LONG 	GroupValue() const = 0;
	virtual	VOID	Group(cGUIRadio *) = 0;

	inline VOID SetValue(LONG Value)
	{
		_Value = Value;
	}
	inline LONG GetValue() const
	{
		return _Value;
	}
};

//--------------------------------------------------------------

class PROTOTYPE cGUISlider : public cGUIWindow
{
public:
	virtual LONG	GetTicks() const = 0;
	virtual VOID	SetTicks(ULONG) = 0;

	enum eTypeM
	{
		e_INT32, e_FLOAT, e_PTR, e_BYTE = 4
	};
	virtual	eTypeM	GetValue(PVOID, UINT nobId = 0) const = 0;
	virtual	VOID	SetValue(PVOID, eTypeM, UINT nobId = 0) = 0;

	virtual eTypeM	GetRange(PVOID) const = 0; // minmax[2]
	virtual VOID	SetRange(PVOID, eTypeM) = 0;

	__forceinline VOID SetValue(LONG i, const UINT NobId = 0)
	{
		SetValue(&i, e_INT32, NobId);
	}
	__forceinline VOID __vectorcall SetFloatValue(FLOAT f, const UINT NobId = 0)
	{
		SetValue(&f, e_FLOAT, NobId);
	}
	__forceinline VOID SetValuePtr(PVOID ptr, const eTypeM Type = e_INT32, const UINT nobId = 0)
	{
		SetValue(ptr, eTypeM(Type|e_PTR), nobId);
	}

	union ifl
	{
		INT		i;
		UINT	u;
		FLOAT	f;
		UCHAR	u8;
	};

	__forceinline LONG GetValue(const UINT NobId = 0) const
	{
		ifl v; GetValue(&v, NobId);
		return v.i;
	}
	__forceinline FLOAT __vectorcall GetFloatValue(const UINT NobId = 0) const
	{
		ifl v; GetValue(&v, NobId);
		return v.f;
	}
	__forceinline VOID SetRange(LONG a, LONG b)
	{
		ifl r[2]; r[0].i = a; r[1].i = b;
		SetRange(r, e_INT32);
	}
	__forceinline VOID __vectorcall SetFloatRange(FLOAT a, FLOAT b)
	{
		ifl r[2]; r[0].f = a; r[1].f = b;
		SetRange(r, e_FLOAT);
	}
};

#pragma pack(pop)// 1

//--------------------------------------------------------------
//--------------------------------------------------------------

//TODO:UNKNOWN_WINDOW check

__forceinline cGUIMainWindow *cGUIWindow::GetMainWindow()
{
	for(cGUIWindow *Parent = m_ParentWindow;
		Parent;
		Parent = Parent->m_ParentWindow )
	{
		if(Parent->m_Type == mmWinMain)
			return (cGUIMainWindow *)Parent;
	}

	return NULL;
}


//--------------------------------------------------------------


__forceinline POINT cGUIWindow::__Pos() const noexcept
{
	LONG x = 0;
	LONG y = 0;

	const cGUIWindow *p = this;
	do
	{
		x += p->m_X;
		y += p->m_Y;
		p = p->m_ParentWindow;
	}
	while(p);

	return {x,y};
}


__forceinline VOID cGUIWindow::ToScreen(INT &px, INT &py) const noexcept
{
	POINT pt = __Pos();
	px += pt.x;
	py += pt.y;
}


__forceinline VOID cGUIWindow::ToClient(INT &px, INT &py) const noexcept
{
	POINT pt = __Pos();
	px -= pt.x;
	py -= pt.y;
}


//--------------------------------------------------------------

struct mmEventDrawData_t : public mmEventData_t
{
	inline mmEventDrawData_t(cGUIWindow *p) : mmEventData_t(p)
	{
		Event_Draw.W = p->m_W;// - p->m_Padding.right;
		Event_Draw.H = p->m_H;// - p->m_Padding.bottom;
		POINT pt = p->__Pos(); // ToScreen
		Event_Draw.X = pt.x;
		Event_Draw.Y = pt.y;
	}
};


//--------------------------------------------------------------


