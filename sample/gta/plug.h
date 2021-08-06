#include "e_sdk_game.h"
#include "../gta.h"


#define _DEV_ // unload


void _cdecl Log(PCSTR fmt, ...);

struct CPlug
{
	gta_shared * data;

	BOOL Plug();
	VOID Unplug();

	IPath* _LogPath;
	PSTR _LogBuff;


	VOID InitVars();
	VOID RemoveVars();

	cCvarGroup* vars;
	cCmdGroup * cmds;
	cGUIColor * colors;
	IUnhooker *	Unhooker;

	SDK_ESP *Esp;

	BOOL DrawObjects(SDK_ESP *);
	VOID Draw1(PObject);

	template<class T>
	__forceinline VOID DrawAll(T & h)
	{
		for(ULONG i = 0, ii = h._Count; i < ii; i++)
			Draw1( h._List[i] );
	}

	struct
	{
		TCvarBoolN ch;
		TCvarBool b;
		FLOAT flt;
		UCHAR c;
	}
	Var;
};

extern CPlug plug;





class giW2S
{
public:
	INT X, Y;
	inline BOOL Project(const Vector &Pos) { return sdk.GI_ScreenProjectPoint(&Pos.x, &X); }
};

class niW2S
{
public:
	INT X, Y;
	inline BOOL Project(const Vector &Pos) { return sdk.NI_ScreenProjectPoint(&Pos.x, &X); }
};



