#pragma comment(linker, "/MERGE:.rdata=.data")
#pragma comment(linker, "/MERGE:.text=.data")
#pragma comment(linker, "/MERGE:.bss=.data")
#pragma comment(linker, "/SECTION:.data,EWR")
#pragma comment(linker, "/ALIGN:128")

#pragma comment(linker, "/ENTRY:DllMain")
#pragma comment(linker, "/NODEFAULTLIB")

#include "game.h"


t_globals globals;
tH3ESDK sdk;
tMODULE GameModuleInfo;
IHagSdk *g_SDK;



#ifdef _DEV_

IPath* g_LogPath = 0;
PSTR g_LogBuff = 0;

void _cdecl Log(PCSTR fmt, ...)
{
	const ULONG Buff_size = 0x30000;

	PSTR Buff = g_LogBuff;
	if(!Buff)
		g_LogBuff = Buff = (PSTR)VirtualAlloc(0, Buff_size, MEM_COMMIT, PAGE_READWRITE);

	PVOID hFile = CreateFileW(g_LogPath->Get(), GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	SetFilePointer(hFile, 0, 0, FILE_END);

	va_list va_alist;
	va_start(va_alist, fmt);
	ULONG Len = g_Str->vsnprintf(Buff, Buff_size-32, fmt, va_alist);
	va_end(va_alist);
	_lstrcpy(&Buff[Len], "\r\n");

	WriteFile(hFile, Buff, Len+2, &Len, 0);
	CloseHandle(hFile);
}

#endif


#include "_sec_begin_i.h" // _init_game_begin.h

BOOL CGame::OnInit()
{
	LOG("Init:0 %s", GetCommandLineA());

	g_SDK = g_Interface->Interface_Query<IHagSdk *>(G_LIB_SDK);
	if(!g_SDK)
		__terminate3(0x20);

	g_SDK->OnInit(sdk);

	GameModuleInfo.init(0);

	LOG("Init:1");

	globals.Update_plug();

	LOG("Init:2");

	Init();

	LOG("Init:3");

	return 1;	//GameInit_2();
}

#include "_sec_end.h" // _init_game_end.h

///////////////////////////////////////////////////////////////

class CModuleInterface : public IModuleInterface
{
public:
	VOID Release()
	{
#ifdef _DEV_
		SAFE_RELEASE(Game);
		delete this;
		SAFE_RELEASE(g_LogPath);
		VirtualFree(g_LogBuff, 0, MEM_RELEASE);
#endif
	}

	BOOL Initialize()
	{
#ifdef _DEV_
		( g_LogPath = g_Interface->GetBasePath() )->Append("__log_.txt", 0);
#endif

		new CGame();

		return 1;
	}

	BOOL Shutdown()
	{
#ifdef _DEV_
		return 1;
#else
		return 0;
#endif
	}

	PVOID Query(ULONG Request)
	{
		if(Request == G_PLUG_GAME)
			return Game;

		return NULL;
	}
};

#include "_sec_begin_i.h" // _init_game_begin.h
#include "e_cpu.h"

BOOL NTAPI DllMain(HMODULE hModule, ULONG ulReason, IMainInterface *MainInterface)
{
	if(ulReason == DLL_PROCESS_ATTACH)
	{
		CPUID();

		globals.Init_base(MainInterface);

		MainInterface->Interface_Register(new CModuleInterface);
	}

	return 1;
}

#include "_sec_end.h" // _init_game_end.h

