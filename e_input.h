#pragma once


#define RAW_IGNORE	1
#define RAW_USE		4

#define BIND_GLOBAL	(1 << 1)
#define BIND_NOTIFY	(1 << 2)
#define BIND_REPEAT	(1 << 3)

// \public\oak\inc\kbd.h

#define mmKey_til	0x29
#define mmKey_1		0x02
#define mmKey_2		0x03
#define mmKey_3		0x04
#define mmKey_4		0x05
#define mmKey_5		0x06
#define mmKey_6		0x07
#define mmKey_7		0x08
#define mmKey_8		0x09
#define mmKey_9		0x0A
#define mmKey_0		0x0B
#define mmKey_q		0x10
#define mmKey_w		0x11
#define mmKey_e		0x12
#define mmKey_r		0x13
#define mmKey_t		0x14
#define mmKey_y		0x15
#define mmKey_u		0x16
#define mmKey_i		0x17
#define mmKey_o		0x18
#define mmKey_p		0x19
#define mmKey_a		0x1E
#define mmKey_s		0x1F
#define mmKey_d		0x20
#define mmKey_f		0x21
#define mmKey_g		0x22
#define mmKey_h		0x23
#define mmKey_j		0x24
#define mmKey_k		0x25
#define mmKey_l		0x26
#define mmKey_z		0x2C
#define mmKey_x		0x2D
#define mmKey_c		0x2E
#define mmKey_v		0x2F
#define mmKey_b		0x30
#define mmKey_n		0x31
#define mmKey_m		0x32

#define mmKey_insert		(0x52+100)
#define mmKey_delete		(0x53+100)
#define mmKey_home			(0x47+100)
#define mmKey_end			(0x4f+100)
#define mmKey_pageup		(0x49+100)
#define mmKey_pagedown		(0x51+100)
#define mmKey_leftarrow		(0x4b+100)
#define mmKey_rightarrow	(0x4d+100)
#define mmKey_uparrow		(0x48+100)
#define mmKey_downarrow		(0x50+100)
#define mmKey_ctrl			(0x1d)		// SCANCODE_CTRL
#define mmKey_rctrl			(0x1d+100)
#define mmKey_alt			(0x38)		// SCANCODE_ALT
#define mmKey_ralt			(0x38+100)
#define mmKey_lshift		(0x2A)		// SCANCODE_LSHIFT
#define mmKey_rshift		(0x36)		// SCANCODE_RSHIFT
#define mmKey_esc			(0x01)
#define mmKey_tab			(0x0F)
#define mmKey_capslock		(0x3A)
#define mmKey_enter			(0x1C)
#define mmKey_backspace		(0x0E)
#define mmKey_space			(0x39)
#define mmKey_f1			(0x3b)
#define mmKey_f2			(0x3c)
#define mmKey_f3			(0x3d)
#define mmKey_f4			(0x3e)
#define mmKey_f5			(0x3f)
#define mmKey_f6			(0x40)
#define mmKey_f7			(0x41)
#define mmKey_f8			(0x42)
#define mmKey_f9			(0x43)
#define mmKey_f10			(0x44)
#define mmKey_f11			(0x57)
#define mmKey_f12			(0x58)
#define mmKey_printscreen	(0x37+100)
#define mmKey_numlock		(0x45+100)
#define mmKey_pause			(0x45)
#define mmKey_break			(0x46+100) // 46 scrolllock

#define mmKey_lWin	(0x5B+100)
#define mmKey_rWin	(0x5C+100)

#define mmKey_kp_home		(0x47)
#define mmKey_kp_end		(0x4f)
#define mmKey_kp_leftarrow	(0x4b)
#define mmKey_kp_rightarrow	(0x4d)
#define mmKey_kp_uparrow	(0x48)
#define mmKey_kp_downarrow	(0x50)
#define mmKey_kp_pageup		(0x49)
#define mmKey_kp_pagedown	(0x51)
#define mmKey_kp_insert		(0x52)
#define mmKey_kp_enter		(0x1C+100)
#define mmKey_kp_slash		(0x35+100)

#define mmKey_kp_1		(0x4f)
#define	mmKey_kp_2		(0x50)
#define	mmKey_kp_3		(0x51)
#define	mmKey_kp_4		(0x4b)
#define	mmKey_kp_5		(0x4c)
#define	mmKey_kp2_5		(0x4c+100)
#define	mmKey_kp_6		(0x4d)
#define	mmKey_kp_7		(0x47)
#define	mmKey_kp_8		(0x48)
#define	mmKey_kp_9		(0x49)
#define	mmKey_kp_0		(0x52)
#define	mmKey_kp_minus	(0x4a)
#define	mmKey_kp_plus	(0x4e)
#define	mmKey_kp_delete	(0x53)


#define mmKey_mouse1		200
#define mmKey_mouse2		201
#define mmKey_mouse3		202
#define mmKey_mouse4		203
#define mmKey_mouse5		204

#define mmKey_mwheelup		206
#define mmKey_mwheeldown	207
#define mmKey_mwheelright	208
#define mmKey_mwheelleft	209


struct t_msg
{
	ULONG	ScanCode;
	UCHAR	KeyDown;
	UCHAR	KeyWasDown;
	UCHAR	Raw;
	UCHAR	Repeat;
	WPARAM	wParam;
	LPARAM	lParam;
	POINT	Pos;
	UCHAR	Flags;
	UCHAR	_pad[3];

	__forceinline t_msg(WPARAM wp, LPARAM lp)
		: wParam(wp)
		, lParam(lp) { *(PULONG64)this = 0; }

	__forceinline bool IsKey() const { return Flags & 1; }
	__forceinline bool IsMouse() const { return Flags & 2; }
	__forceinline bool IsMWheel() const { return Flags & 8; } // down only
	__forceinline bool IsMDblClc() const { return Flags & 4; }

	UCHAR Init(UINT);
};

typedef const struct t_msg CMSG;


class PROTOTYPE cInput
{
public:
	virtual VOID	Notify(CMSG &) = 0;
	virtual ULONG	QueryScanCode(IN PCSTR key) const = 0;
	virtual BOOL	IsChar(ULONG sc) const = 0;
	virtual UINT	QueryChar(CMSG &) const = 0;
	virtual ULONG	QueryName(ULONG sc, OUT PSTR key) const = 0;

	UCHAR	ShiftState;
	UCHAR	CtrlState;
	UCHAR	AltState;
	UCHAR	CapsState;

	UCHAR	Mouse1State;
	UCHAR	Mouse2State;
	UCHAR	Mouse3State;
	UCHAR	Mouse4State;
	UCHAR	Mouse5State;

	UCHAR	___unused[2];

	UCHAR	RawInputFlags;
};


class cString;

class PROTOTYPE cBind
{
public:
	virtual VOID Notify(t_msg *) = 0;
	virtual ULONG GetFlags(ULONG Key) const = 0;
	virtual const cString* Get(ULONG Key) const = 0;
	virtual UINT Add(PUCHAR Keys, PCSTR cmd, ULONG Flags) = 0;
	virtual UINT Del(PUCHAR Keys) = 0;

	inline BOOL IsGlobal(ULONG Key) const { return GetFlags(Key) & BIND_GLOBAL; }
	inline BOOL IsNotify(ULONG Key) const { return GetFlags(Key) & BIND_NOTIFY; }
	inline BOOL IsRepeat(ULONG Key) const { return GetFlags(Key) & BIND_REPEAT; }

	virtual UINT QueryK(const cString *cmd, OUT PUCHAR keys) const = 0; // block all binds; QueryN(0,0) unblock
	virtual PSTR QueryN(const cString *cmd, OUT PSTR names) const = 0; // ret[1]=flags

	virtual VOID DelAll(const cString *cmd) = 0;
};




__forceinline UCHAR t_msg::Init(UINT Message)
{
	ULONG m = (Message >= WM_KEYFIRST && Message <= WM_KEYLAST);

	if(Message >= WM_MOUSEFIRST && Message <= WM_MOUSELAST)
	{
		m |= 2;

		if(	Message == WM_LBUTTONDBLCLK ||
			Message == WM_RBUTTONDBLCLK ||
			Message == WM_MBUTTONDBLCLK ||
			Message == WM_XBUTTONDBLCLK )
			m |= 4;
		else
		if(	Message == WM_MOUSEWHEEL ||
			Message == WM_MOUSEHWHEEL )
			m |= 8;
	}

	return Flags = UCHAR(m);
}

