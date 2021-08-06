#pragma once

#include "e_input.h"

enum eGuiWin
{
	mmWinMain		,
	mmWinLabel		,
	mmWinCheckBox	,
	mmWinLine		,
	mmWinButton		,
	mmWinTextBox	,
	mmWinScroller	,
	mmWinColorArea	,
	mmWinList		,
	mmWinMenu		,
	mmWinTab		,
	mmWinRadio		,
	mmWinSlider		,
	mmWinTree		,
};

class cGUIWindow;
class cGUIMainWindow;
class cGUITaskbar;
class cGUILabel;
class cGUICheckBox;
class cGUILine;
class cGUIButton;
class cGUITextBox;
class cGUIScroller;
class cGUIColorArea;
class cGUIList;
class cGUIMenu;
class cGUITab;
class cGUIRadio;
class cGUISlider;
class cGUITree;
class cGUIColor;
class CGuiColor;
class cGUIFontMan;
class cGUISkin;
class cGUIKey;

//---------------------------------------------------------------


#define	mmMOUSE_Move	(~0)
#define	mmMOUSE1		1
#define	mmMOUSE2		2
#define	mmMOUSE3		3
#define	mmMOUSE4		4
#define	mmMOUSE5		5

#define mmMOUSE_whUP	7
#define mmMOUSE_whDN	8
#define mmMOUSE_whR		9
#define mmMOUSE_whL		10

#define	mmMOUSE_MAX		10

//---------------------------------------------------------------

enum mmEvents
{
	mmEvent_None,

	mmEvent_MousePre,	// x, y, Mouse, Down - MousePress
	mmEvent_MousePost,
	mmEvent_MouseMove,	// x, y
	mmEvent_MouseClick,	// x, y - GUIMainWindow::CheckClick

	mmEvent_KeyUp,		// msg.ScanCode - KeyPress
	mmEvent_KeyDown,	//

	mmEvent_MainWindowMoveBegin,	// MousePress mmMOUSE1
	mmEvent_MainWindowMove,			// dx, dy
	mmEvent_MainWindowMoveEnd,

	mmEvent_MainWindowResize,	// post MouseMove
	mmEvent_MainWindowClose,	// MousePress

	mmEvent_LostFocus,
	mmEvent_SetFocus,

	mmEvent_PreDraw,	// mmEventDrawData_t
	mmEvent_PostDraw,

	mmEvent_PreDrawItem, // mmEvent_DrawItem // Event_PreDrawMenuitem
	mmEvent_PostDrawItem,

	mmEvent_MenuHide,
	mmEvent_MenuShow,
	mmEvent_MenuClick,

	mmEvent_PreChangeSelection, // Event_Change Event_ChangePtrs 
	mmEvent_PostChangeSelection,

	mmEvent_PreChangeValue,
	mmEvent_PostChangeValue,

	mmEvent_PreChangeTab,
	mmEvent_PostChangeTab,

	mmEvent_PreMoveItem,
	mmEvent_PostMoveItem,

	//mmEvent_PostDeleteItem,
};

struct mmEventData_t
{
	inline mmEventData_t &ref()
	{
		return *this;
	}
	inline mmEventData_t (cGUIWindow *p)
		: Window(p)
	{
	}
	inline mmEventData_t (cGUIWindow *p, INT arg0)
		: Window(p)
	{
		Args[0] = arg0;
	}
	inline mmEventData_t (cGUIWindow *p, INT arg0, INT arg1)
		: Window(p)
	{
		Args[0] = arg0;
		Args[1] = arg1;
	}
	inline mmEventData_t (cGUIWindow *p, INT arg0, INT arg1, INT arg2)
		: Window(p)
	{
		Args[0] = arg0;
		Args[1] = arg1;
		Args[2] = arg2;
	}
	inline mmEventData_t (cGUIWindow *p, INT arg0, INT arg1, INT arg2, INT arg3)
		: Window(p)
	{
		Args[0] = arg0;
		Args[1] = arg1;
		Args[2] = arg2;
		Args[3] = arg3;
	}
	inline mmEventData_t (cGUIWindow *p, INT arg0, INT arg1, INT arg2, INT arg3, INT arg4)
		: Window(p)
	{
		Args[0] = arg0;
		Args[1] = arg1;
		Args[2] = arg2;
		Args[3] = arg3;
		Args[4] = arg4;
	}
	inline mmEventData_t (cGUIWindow *p, INT arg0, INT arg1, INT arg2, INT arg3, INT arg4, INT arg5)
		: Window(p)
	{
		Args[0] = arg0;
		Args[1] = arg1;
		Args[2] = arg2;
		Args[3] = arg3;
		Args[4] = arg4;
		Args[5] = arg5;
	}

	inline mmEventData_t (cGUIWindow *p, PVOID arg0)
		: Window(p)
	{
		ArgP[0] = arg0;
	}
	inline mmEventData_t (cGUIWindow *p, PVOID arg0, PVOID arg1)
		: Window(p)
	{
		ArgP[0] = arg0;
		ArgP[1] = arg1;
	}
	inline mmEventData_t (cGUIWindow *p, PVOID arg0, PVOID arg1, PVOID arg2)
		: Window(p)
	{
		ArgP[0] = arg0;
		ArgP[1] = arg1;
		ArgP[2] = arg2;
	}

	inline mmEventData_t (cGUIWindow *p, CMSG &msg)
		: Window(p)
	{
		Event_Key.Key = msg.ScanCode;
		Event_Key.Msg = &msg;
	}


	union
	{
		cGUIWindow *	Window;
		cGUIMainWindow *MainWindow;
		cGUILabel *		Label;
		cGUICheckBox *	CheckBox;
		cGUIButton *	Button;
		cGUITextBox *	TextBox;
		cGUIScroller *	Scroller;
		cGUIList *		List;
		cGUIMenu *		Menu;
		cGUITab *		Tab;
		cGUIRadio *		Radio;
		cGUISlider *	Slider;
	};

	union
	{
		INT			Args[6];
		PVOID		ArgP[3];

		struct
		{
			INT	x, y;
			UINT Button;
			UINT __Down; //0/1/2

			__forceinline bool Up()			const { return __Down == 0; }
			__forceinline UINT Down()		const { return __Down; }
			__forceinline bool Up_M1()		const { return Up() && Button == mmMOUSE1; }
			__forceinline bool Up_M2()		const { return Up() && Button == mmMOUSE2; }
			__forceinline bool Down_M1()	const { return Down() && Button == mmMOUSE1; }
			__forceinline bool Down_M2()	const { return Down() && Button == mmMOUSE2; }
			__forceinline void Init(INT X, INT Y, UINT button, UINT down) { x=X; y=Y; Button=button; __Down=down; }
		} Event_Mouse;

		struct
		{
			UINT Key;
			UINT unused;
			CMSG * Msg;
		} Event_Key;

		struct
		{
			INT	Old, New;
		} Event_Change;

		struct
		{
			PVOID Old, New;
		} Event_ChangePtrs;

		struct
		{
			INT	X, Y, W, H;
		} Event_Draw;

		struct // GUIList
		{
			UINT	Index;	// 0
			UINT	Line;	// 1
			UINT	Height;	// 2
			INT		TextX;	// 3
			INT		X;		// 4
			INT		Y;		// 5
		} Event_DrawItem;

		struct
		{
			UINT Index;
			UINT Disabled; // Entry->Disabled GUIMenu
		} Event_PreDrawMenuitem;

		struct
		{
			INT	x, y;
		} Event_WindowMove;

		struct
		{
			UINT Target, Index;
		} Event_MoveItem;
	};
};


__forceinline mmEvents GetKeyEvent(const t_msg &msg) // msg.KeyDown ? mmEvent_KeyDown : mmEvent_KeyUp
{
	return mmEvents(mmEvent_KeyUp + msg.KeyDown);// & 1));
}



// callback fn
// ret error code
typedef
ULONG (CALLBACK *GuiCallbackFn)(mmEvents, mmEventData_t &);

//---------------------------------------------------------------

#define	mmWinInvisible				(1 << 1)
#define	mmWinNoSkin					(1 << 2)
#define	mmWinNoBackground			(1 << 3)
#define	mmWinDisabled				(1 << 4)
#define	mmWinContainer				(1 << 5)	// no bg & header

#define	mmWinResizable				(1 << 10)
#define	mmWinBorderOnly				(1 << 11)
#define	mmWinForceCheckSize			(1 << 12)
#define	mmWinAlwaysVisible			(1 << 13)
#define	mmWinAlwaysVisibleCaption	(1 << 14)
#define	mmWinNoClose				(1 << 15)

#define	mmTextBox_Readonly		(1 << 10)
#define	mmTextBox_Transparent	(1 << 11)
#define	mmTextBox_Border		(1 << 12)
#define	mmTextBox_Numeric		(1 << 13)
#define	mmTextBox_Hex			(1 << 14)
#define	mmTextBox_ColorTags		(1 << 15)
#define	mmTextBox_Multiline		(1 << 16)
#define	mmTextBox_Scroll		(1 << 17)
#define	mmTextBox_CursorVis		(1 << 18)
#define	mmTextBox_WordWrap		(1 << 19)

#define	mmMenu_Default		(1 << 10)
#define	mmMenu_Static		(1 << 11)

#define	mmScroller_Horiz		(1 << 10)
#define	mmScroller_Transparent	(1 << 11)

#define	mmList_Movable		(1 << 10)
#define	mmList_Dropdown		(1 << 11)
#define	mmList_Checkbox		(1 << 12)
#define	mmList_NoScroll		(1 << 13)
#define	mmList_NoBorder		(1 << 14)

#define	mmLabel_AutoSizeW	(1 << 10)
#define	mmLabel_AutoSizeH	(1 << 11)
#define	mmLabel_CenterW		(1 << 12)
#define	mmLabel_CenterH		(1 << 13)

//---------------------------------------------------------------

#pragma pack(push, 1)


typedef struct _GUICOLORS
{
	struct t_window
	{
		XXCOLOR text				;
		XXCOLOR background			;
		XXCOLOR border_highlight	;
		XXCOLOR border_shadow		;

		VOID Register(UCHAR, PCSTR, CGuiColor *);
	}
	window, taskbar, tip, button;

	struct t_text : public t_window
	{
		XXCOLOR selected_text		;
		XXCOLOR selected_background	;

		VOID Register(UCHAR, PCSTR, CGuiColor *);
	}
	text_edit, text_readonly;

	struct t_scrollbar : public t_window
	{
		XXCOLOR button_background	;
		XXCOLOR button_pressed		;

		VOID Register(UCHAR, PCSTR, CGuiColor *);
	}
	scrollbar;

	struct t_list : public t_window
	{
		XXCOLOR selected_text			;
		XXCOLOR selected_background		;

		VOID Register(UCHAR, PCSTR, CGuiColor *);
	}
	list;

	struct t_tree : public t_list
	{
		XXCOLOR line;

		VOID Register(UCHAR, PCSTR, CGuiColor *);
	}
	tree;

	struct t_menu : public t_list
	{
		XXCOLOR grayed;

		VOID Register(UCHAR, PCSTR, CGuiColor *);
	}
	menu;

	struct t_colorarea
	{
		XXCOLOR border_highlight	;
		XXCOLOR border_shadow		;

		VOID Register(UCHAR, PCSTR, CGuiColor *);
	}
	colorarea;

	struct t_header
	{
		XXCOLOR active_text		;
		XXCOLOR active_1		;
		XXCOLOR active_2		;
		XXCOLOR passive_text	;
		XXCOLOR passive_1		;
		XXCOLOR passive_2		;

		VOID Register(UCHAR, PCSTR, CGuiColor *);
	}
	header;

	struct t_tab : public t_window
	{
		XXCOLOR passive_text		;
		XXCOLOR passive_background	;

		VOID Register(UCHAR, PCSTR, CGuiColor *);
	}
	tab;

	struct t_slider : public t_window
	{
		XXCOLOR nob_passive	;
		XXCOLOR nob_active	;

		VOID Register(UCHAR, PCSTR, CGuiColor *);
	}
	slider;

} GUICOLORS, *PGUICOLORS;

//---------------------------------------

typedef struct _GUIFONTS
{
	XFONT Window	;
	XFONT Label		;
	XFONT CheckBox	;
	XFONT Button	;
	XFONT TextBox	;
	XFONT List		;
	XFONT Tree		;
	XFONT Menu		;
	XFONT Tab		;
	XFONT Radio		;
	XFONT Slider	;
	XFONT Taskbar	;
	XFONT Tip		;
} GUIFONTS, *PGUIFONTS;

#pragma pack(pop)


//---------------------------------------------------------------

typedef const POINT CPOS;

class PROTOTYPE cGUI
{
public:
	POINT	m_CursorPosition;
	UCHAR	m_CursorDraw;
	UCHAR	m_Resizing; // cursor#
	UCHAR	m_ScreenResized; // fix bar
	UCHAR	m_FontsChanged; // notify all windows
	UCHAR	m_GuiVis;
	UCHAR	m_DisableBar;
	UCHAR	__pad_[2];

public:
	virtual BOOL	Redraw() = 0;

public:
	virtual VOID	MousePress(UINT mmMOUSE, UINT Down) = 0;
	virtual VOID	MouseMove(CPOS &) = 0;
	virtual VOID	KeyPress(CMSG &) = 0;

public:
	virtual VOID ShowHide(BOOL Vis) = 0;

	inline bool IsVisible() const	{ return m_GuiVis; }
	inline void SetVisible(BOOL v)	{ ShowHide(v); }

public:
	virtual cGUIMainWindow * Window_Add() = 0;
	virtual cGUIMainWindow * Window_Find(PCSTR) = 0;

	virtual cGUIMainWindow * SetTopWindow(cGUIMainWindow *) = 0;

	virtual cGUIMenu *	CreateMenu() = 0;

public:
	enum e_GuiPtr : ULONG
	{
		e_Fonts,
		e_Colors,
		e_ColorInterface,
		e_FontInterface,
		e_TaskbarInterface,
	};
	virtual PVOID GuiPtr(e_GuiPtr) = 0;

	inline PGUIFONTS	Fonts()		{ return (PGUIFONTS)GuiPtr(e_Fonts); }
	inline PGUICOLORS	Colors()	{ return (PGUICOLORS)GuiPtr(e_Colors); }

	inline cGUIColor *	ColorInterface()	{ return (cGUIColor  *)GuiPtr(e_ColorInterface); }
	inline cGUIFontMan*	FontInterface()		{ return (cGUIFontMan*)GuiPtr(e_FontInterface);	}
	inline cGUITaskbar* TaskbarInterface()	{ return (cGUITaskbar*)GuiPtr(e_TaskbarInterface); }

	inline cGUIMainWindow *Window_Add_no_bar()
	{
		m_DisableBar = 1;
		cGUIMainWindow *pWnd = Window_Add();
		m_DisableBar = 0;
		return pWnd;
	}

	inline cGUIMainWindow * GetTopWindow()
	{
		return SetTopWindow(NULL);
	}
};

