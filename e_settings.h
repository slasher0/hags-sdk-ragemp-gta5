#pragma once


enum eSETTING : UCHAR
{
	SETTING_CUSTOM = 0,
	SETTING_LINE,
	SETTING_LIST,
	SETTING_BIND,
	SETTING_LABEL,
	SETTING_TAB,
	SETTING_BUTTON,
	SETTING_CHECK,
	SETTING_INT8,
	SETTING_INT32,
	SETTING_FLOAT,
	SETTING_BREAK,// = 0xFF,
};


class CTABS;
class cSettingsPage;


class PROTOTYPE cTABS
{
public:
	cGUITab * m_Tab;

	__forceinline cGUITab * Wnd() const
	{
		return m_Tab;
	}

public:
	virtual VOID	Release() = 0;
	virtual cSettingsPage *	Tab(PCSTR) = 0;
};


typedef struct TSetting
{
	cSettingsPage *		Parent;

	union
	{
		LONG_PTR		LongPtr;
		LONG			Long;
		cGUIWindow *	Window;
		cTABS *			Tabs;
		CTABS *			TabsImpl;
		cGUICheckBox *	Check;
		cGUILabel *		Label;
		cGUIButton *	Button;
		cGUISlider *	Slider;
		cGUIList *		List;
		cGUILine *		Line;
	} Wnd;

	cGUILabel *	Label;
	LONG		LabelWidth;
	eSETTING	Type;
	UCHAR		UserConfig;

	__forceinline cTABS *Tabs()
	{
		return (Type == SETTING_TAB) ? Wnd.Tabs : NULL;
	}
	__forceinline CTABS *TabsImpl()
	{
		return (Type == SETTING_TAB) ? Wnd.TabsImpl : NULL;
	}

	~TSetting();

} *PSetting;



class PROTOTYPE cSettingsPage
{
public:
	cGUIMainWindow *m_wnd; // C_TabPageControl

	__forceinline cGUIMainWindow * Wnd() const
	{
		return m_wnd;
	}

	virtual VOID Release() = 0;

public:
	struct tArg
	{
		__forceinline VOID Init0()
		{
			memset0(this,sizeof(*this));
		}

		eSETTING	_Type;
		LONG		TextWidth;
		PCSTR		Text;
		PCSTR		Tip;
		LONG		Width, Height;

		struct
		{
			union
			{
				PUCHAR		pVar_BYTE;
				PINT		pVar_INT;
				PULONG		pVar_UINT;
				PFLOAT		pVar_FLOAT;
				PCvarBool	pVar_BOOL;
				PCSTR		pVar_STR; // tmp
			};
			union
			{
				struct { INT	iMin, iMax; };
				struct { ULONG	uMin, uMax; };
				struct { FLOAT	fMin, fMax; };
			};
			union
			{
				ULONG	Bits; // SETTING_BOOL
				eGuiWin	CustomType;
			};
		};
	};

	virtual PSetting Add(const tArg &) = 0;

	__forceinline PSetting Custom(eGuiWin Type)
	{
		tArg arg;
		arg._Type = SETTING_CUSTOM;
		arg.CustomType = Type;
		return Add(arg);
	}

	__forceinline PSetting Break(const LONG Height = 0)
	{
		tArg arg;
		arg._Type = SETTING_BREAK;
		arg.Height = Height;
		return Add(arg);
	}

	__forceinline PSetting Line(const LONG Height = 0)
	{
		tArg arg;
		arg._Type = SETTING_LINE;
		arg.Height = Height;
		return Add(arg);
	}

	__forceinline PSetting Label(PCSTR Text, LONG TextWidth, PCSTR Tip)
	{
		tArg arg;
		arg._Type = SETTING_LABEL;
		arg.Text = Text;
		arg.TextWidth = TextWidth;
		arg.Tip = Tip;
		return Add(arg);
	}

	__forceinline PSetting Check_Impl(PCSTR Text, PCSTR Tip, PVOID pVar, const ULONG Bits)
	{
		tArg arg;
		arg._Type = SETTING_CHECK;
		arg.Text = Text;
		arg.Tip = Tip;
		arg.pVar_INT = (PINT)pVar;
		arg.Bits = Bits;
		return Add(arg);
	}
	__forceinline PSetting Check(PCSTR Text, PCSTR Tip, PUCHAR pVar)
	{
		return Check_Impl(Text, Tip, pVar, 8); // 2-8:BYTE
	}
	__forceinline PSetting Check(PCSTR Text, PCSTR Tip, PCvarBool pVar)
	{
		return Check_Impl(Text, Tip, pVar, 1); // 1:varBool
	}
	__forceinline PSetting Check(PCSTR Text, PCSTR Tip, PINT pVar)
	{
		return Check_Impl(Text, Tip, pVar, 0);
	}

	__forceinline PSetting List(PCSTR Text, LONG TextWidth, PCSTR Tip, PINT pVar, LONG Width)
	{
		tArg arg;
		arg._Type = SETTING_LIST;
		arg.Text = Text;
		arg.TextWidth = TextWidth;
		arg.Tip = Tip;
		arg.pVar_INT = pVar;
		arg.Width = Width;
		return Add(arg);
	}

	__forceinline PSetting Button(PCSTR Text, PCSTR Exec, LONG Width, LONG Height, PCSTR Tip)
	{
		tArg arg;
		arg._Type = SETTING_BUTTON;
		arg.Text = Text;
		arg.pVar_STR = Exec;
		arg.Width = Width;
		arg.Height = Height;
		arg.Tip = Tip;
		return Add(arg);
	}

	__forceinline PSetting Bind(PCSTR Alias, LONG Width, LONG Height, PCSTR Tip, ULONG Flags)
	{
		tArg arg;
		arg._Type = SETTING_BIND;
		arg.Text = Alias;
		arg.Width = Width;
		arg.Height = Height;
		arg.Tip = Tip;
		arg.Bits = Flags;
		return Add(arg);
	}

};




class PROTOTYPE cSettings
{
public:
	virtual VOID	Release() = 0;
	virtual cTABS *	Tabs() const = 0;

	inline cGUITab *Tab() const { return Tabs()->m_Tab;	}
};


class PROTOTYPE cSettingsAlloc
{
public:
	virtual VOID		Release() = 0;
	virtual cSettings *	Get() = 0;
	virtual PSetting	Add(PCSTR) = 0;
	virtual PSetting	Add(cString *) = 0;
	virtual VOID		LoadConfig() = 0;
};

