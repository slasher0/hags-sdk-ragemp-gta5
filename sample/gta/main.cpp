#pragma comment(linker, "/MERGE:.rdata=.data")
#pragma comment(linker, "/MERGE:.text=.data")
#pragma comment(linker, "/MERGE:.bss=.data")
#pragma comment(linker, "/SECTION:.data,EWR")
#pragma comment(linker, "/ALIGN:128")

#pragma comment(linker, "/ENTRY:DllMain")
#pragma comment(linker, "/NODEFAULTLIB")

#include "plug.h"


t_globals globals;
tMODULE GameModuleInfo;

IHagSdk *g_SDK;

tH3ESDK sdk;

CPlug plug;


void _cdecl Log(PCSTR fmt, ...)
{
	const ULONG Buff_size = 0x30000;

	PSTR Buff = plug._LogBuff;
	if(!Buff)
		plug._LogBuff = Buff = (PSTR)VirtualAlloc(0, Buff_size, MEM_COMMIT, PAGE_READWRITE);

	PVOID hFile = CreateFileW(plug._LogPath->Get(), GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	SetFilePointer(hFile, 0, 0, FILE_END);

	va_list va_alist;
	va_start(va_alist, fmt);
	ULONG Len = g_Str->vsnprintf(Buff, Buff_size-32, fmt, va_alist);
	va_end(va_alist);
	_lstrcpy(&Buff[Len], "\r\n");

	WriteFile(hFile, Buff, Len+2, &Len, 0);
	CloseHandle(hFile);
}




class CModuleInterface : public IModuleInterface
{
public:
	VOID Release()
	{
#ifdef _DEV_
		plug.Unplug();
		delete this;
		g_Console->Printf("unload");
#endif
	}

	BOOL Initialize()
	{
		BOOL k = plug.Plug();
		g_Console->Printf("load:%i", k);
		return k;
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
		//if(Request == G_MODULE_)
		//	return this;

		return NULL;
	}
};



BOOL NTAPI DllMain(HMODULE hModule, ULONG ulReason, IMainInterface *MainInterface)
{
	if(ulReason == DLL_PROCESS_ATTACH)
	{
		globals.Init_plug(MainInterface);

		MainInterface->Interface_Register( new CModuleInterface );
	}

	return 1;
}



