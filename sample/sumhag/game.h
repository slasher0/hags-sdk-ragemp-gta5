

#define _DEV_



#ifndef _DEV_
	#define NO_LOG
#endif

#ifndef _DEV_
#endif



#ifdef _DEV_
	#define STAGE(x) x

	void _cdecl Log(const char * fmt, ...);
#	define LOG Log

#else
	#define STAGE(x) 1
#endif



#include "e_sdk_game.h"
//#include "e_sdk_objects.h"
//#include "e_sdk_helpers.h"


//------------------------------------------------------------------

#pragma pack(push, 1)
#include "game_engine.h"
#pragma pack(pop)




#define MAX_PLAYERS		128
#define MAX_VEHICLES	256
#define MAX_HITBOX		16
#define MAX_ENTS		512


#define OBJECT_PC_NEUTRAL		1
#define OBJECT_PC_FRIEND		2
#define OBJECT_PC_ENEMY			3
#define OBJECT_PC_ENEMY_VISIBLE	4

#define OBJECT_NPC_NEUTRAL			5
#define OBJECT_NPC_FRIEND			6
#define OBJECT_NPC_ENEMY			7
#define OBJECT_NPC_ENEMY_VISIBLE	8

#define OBJECT_VEHICLE_NEUTRAL		9
#define OBJECT_VEHICLE_FRIEND		10
#define OBJECT_VEHICLE_ENEMY		11

#define OBJECT_PICKUP		12
#define OBJECT_UNKNOWN		13



inline
PCSTR utf2acp(PVOID In, LONG_PTR InLen, PSTR Buff, ULONG Size)
{
	if(!In)
		return "<0>";
	ULONG Len;
	WCHAR Tmp[512];
	Len = MultiByteToWideChar(CP_UTF8, 0, (PCSTR)In, InLen, Tmp, ARRAY_COUNT(Tmp));
	Len = WideCharToMultiByte(CP_ACP, 0, Tmp, Len, Buff, Size, 0, 0);
	Buff[Len] = 0;
	return Buff;
}

inline
PCSTR uni2acp(PVOID In, LONG_PTR InLen, PSTR Buff, ULONG Size)
{
	if(!In)
		return "<0>";
	ULONG Len = WideCharToMultiByte(CP_ACP, 0, (PCWSTR)In, InLen, Buff, Size, 0, 0);
	Buff[Len] = 0;
	return Buff;
}


#include "radar.h"
#include "esp.h"
#include "client.h"


#ifndef STF
#	define STF ;
#	define STN(S) Str_Alloc(S)
#endif