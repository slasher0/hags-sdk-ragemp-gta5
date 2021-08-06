#include "game.h"

//--------------------------------------------

TCvarBool CGame::TV::ents_npc;

BOOL	GameInitialized = 0;
CGame * Game = 0;

_MM_ALIGN16 VEC4	CGame::m_ViewOrigin;
_MM_ALIGN16 VEC4	CGame::m_ViewForward;

FLOAT		CGame::m_ViewPitch;
FLOAT		CGame::m_ViewYaw;

PPPlayer	CGame::m_Players;
ULONG		CGame::m_PlayersCount = 0;

UINT	 	CGame::m_VehiclesCount = 0;
TVehicle **	CGame::m_Vehicles;

TEntity **	CGame::m_Ents;
UINT	 	CGame::m_EntsCount = 0;

INT		CGame::BotDebug = 0;

bool	TLocal::m_Valid = 0;

ULONG	CGame::m_Frame = 0;
bool	CGame::m_ForceUpdate = 0;

//--------------------------------------------

void CGame::OnHalt()
{
	m_FailTimer.ms(1000);
	m_Error = 665;
	sdk.Valid = 0;
	m_Local.m_Valid = 0;
	m_PlayersCount = 0;
}




inline
VOID CGame::InputUnLock()
{
	//if(m_OrigWnd.LowPart)
	//{
	//	PULARGE_INTEGER phwnd = (PULARGE_INTEGER)(PVOID)m_Engine.m_HWNDptr;
	//	if(phwnd)
	//	{
	//		m_OrigWnd.LowPart = 0;
	//		phwnd->HighPart = m_OrigWnd.HighPart;

	//		if(m_OrigCursor)
	//			SetCursor(m_OrigCursor);
	//	}
	//}
}


inline
VOID CGame::InputLock()
{
	//PULARGE_INTEGER phwnd = (PULARGE_INTEGER)(PVOID)m_Engine.m_HWNDptr;
	//if(phwnd)
	//{
	//	if(g_GUI->IsVisible())
	//	{
	//		if(!m_OrigWnd.QuadPart)
	//		{
	//			m_OrigWnd.QuadPart = phwnd->QuadPart;
	//			phwnd->HighPart = 0x1010;

	//			m_OrigCursor = SetCursor(0);
	//		}
	//	}
	//	else
	//	if(m_OrigWnd.LowPart)
	//	{
	//		m_OrigWnd.LowPart = 0;
	//		phwnd->HighPart = m_OrigWnd.HighPart;

	//		if(m_OrigCursor)
	//			SetCursor(m_OrigCursor);
	//	}
	//}
}



BOOL CGame::OnDraw()
{
	if(!GameInitialized)
		return 0;

	InputLock();

	if(m_FailTimer.Check())
	{
		sdk.Valid = 0;
	}
	else
	{
		m_Error = Validate();
		sdk.Valid = !m_Error;
	}

	if(sdk.Valid)
	{
		AimFrame();
		Esp_Draw();
	}

#ifdef _DEV_
	g_GuiHud->SetColor(RGBA(0,0,0,200));
	if(sdk.Valid)
		g_GuiHud->DrawTextF(0, 10, 10, RGBA(0,255,0,210), HUD_STRING_OPAQUE, "OK");
	else
		g_GuiHud->DrawTextF(0, 10, 10, RGBA(255,0,0,210), HUD_STRING_OPAQUE, "%02u", m_Error);
#endif

	return sdk.Valid;
}

//-----------------------------------------------------------


//_declspec(dllexport)
void _cdecl Cmd_qqq()
{
	//NtTerminateProcess(NtCurrentProcess(), 0);

	//PVOID (NTAPI *fn)(PVOID);
	//*(PVOID*)&fn = (PVOID)0x140EADE38ULL;
	//PVOID p = fn(NULL);
	//g_Console->Printf("%p", p);

	//PVOID (NTAPI *fn)();//PVOID,PVOID,PVOID);
	//*(PVOID*)&fn = (PVOID)0x0140E5B184;

	//PVOID p = fn();

	//g_Console->Printf("o: %p", p);
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------



VOID TLocal::Teleport(const Vector &v)
{
	m_TP = 0;
	if(!m_Valid)
		return;
	m_TPos = v;
	m_TP = 1;
	g_GUI->SetVisible(0);
}


void goto_print(const Vector &pos)
{
	g_Console->Printf("goto %f %f %f", pos.x, pos.y, pos.z);
}


void goto_fail()
{
	g_Console->Printf("fail");
}


void Cmd_goto()
{
	CGame *pg = Game;
	cCmd *Cmd = g_Cmd;


	PCSTR arg1 = Cmd->ArgV(1);
	if(	arg1 &&
		CmpI(&arg1[0],'w') &&
		CmpI(&arg1[1],'p') )
	{
		FLOAT zz = FLT(-202);

		if(Cmd->ArgV(2))
		{
			float z = Cmd->ArgF(2);
			if( z > FLT(10) )
				zz = z;
		}

		pg->m_Local.m_TPos.z = zz;
		pg->m_Local.m_TP = 2;
		return;
	}


	if(Cmd->Argc() < 3)
	{
		PCSTR p = Cmd->ArgV(1);
		if(p && _ltolower(*p) == 't')
		{
			pg->m_Local.m_TP = 3;
			return;
		}

		Vector &pos = pg->m_Local.World;
		goto_print(pos);
	}
	else
	{
		Vector Pos(Cmd->ArgF(1), Cmd->ArgF(2), Cmd->ArgF(3));
		pg->m_Local.Teleport(Pos);
	}
}

class CLocCB : public SDK_LOCATIONS::Callback
{
public:
	BOOL GetPos(Vector &Pos)
	{
		Pos = Game->m_Local.World;
		return sdk.Valid;
	}
	VOID SetPos(const Vector &Pos)
	{
		Game->m_Local.Teleport(Pos);
	}
};


//---------------------------------------------------------------------


class CNubCB : public SDK_NAMES::Callback
{
public:
	VOID OnUpdate(AddNew *cb)
	{
		for(ULONG i = 0; i < CGame::m_PlayersCount; i++)
		{
			PPlayer Player = CGame::m_Players[i];

			if(	Player->Type >= OBJECT_PC_NEUTRAL && Player->Type <= OBJECT_PC_ENEMY_VISIBLE ) 
			{
				cb->NewNub( Player->Name );
			}
		}
	}
};


//---------------------------------------------------------------------


#include "_sec_begin_i.h" // _init_game_begin.h


VOID __fastcall __Terminate(t_term &t)
{
	g_Interface->Terminate(t);
}


void CGame::Init_OBJ()
{
	g_SDK->RegisterObjectType(OBJECT_PC_NEUTRAL,		"pc_neutral");
	g_SDK->RegisterObjectType(OBJECT_PC_FRIEND,			"pc_friend");
	g_SDK->RegisterObjectType(OBJECT_PC_ENEMY,			"pc_enemy");
	g_SDK->RegisterObjectType(OBJECT_PC_ENEMY_VISIBLE,	"pc_enemy_visible");

	g_SDK->RegisterObjectType(OBJECT_NPC_NEUTRAL,		"npc_neutral");
	g_SDK->RegisterObjectType(OBJECT_NPC_FRIEND,		"npc_friend");
	g_SDK->RegisterObjectType(OBJECT_NPC_ENEMY,			"npc_enemy");
	g_SDK->RegisterObjectType(OBJECT_NPC_ENEMY_VISIBLE,	"npc_enemy_visible");

	g_SDK->RegisterObjectType(OBJECT_PICKUP,		"obj_pickup");
	g_SDK->RegisterObjectType(OBJECT_UNKNOWN,		"obj_unknown");
}


#include "e_sig.h"


void CGame::Init()
{
	g_Input->RawInputFlags = RAW_USE;


	memset0(&m_Engine, sizeof(m_Engine));
	//memset0(&m_EngFuncs, sizeof(m_EngFuncs));

	Init_OBJ();

	STF;

	g_Patcher->Options(IPatcher::P_ERASE_PATTERNS, 1);
	g_Patcher->Options(IPatcher::P_SCAN_STEP, 0);

 

	PUCHAR pFnd;



	//pFnd = g_Patcher->FindPattern(GameModuleInfo.BaseMin, GameModuleInfo.BaseMax,
	//	XSIG("48 8B 05 ?? ?? ?? ?? 8B CE 48 8B 3C C8 48 85 FF 74 ?? F6 87 ?? ?? 00 00 01"));

	//if(pFnd)
	//	pFnd = g_Patcher->GetRelativeOffset(pFnd, 3);
	//LOG("CViewportPtr = %p\r\n", pFnd);

	//m_Engine.m_ViewportPtr = (CViewportPtr *)pFnd;



	/*
	pFnd = g_Patcher->FindPattern(GameModuleInfo.BaseMin, GameModuleInfo.BaseMax,
		XSIG("CC AF FB 5A E8"));
	LOG("BLIP_0 = %p", pFnd);

	if(pFnd)
	{
		pFnd = g_Patcher->FindPattern(pFnd, pFnd+126, 
			XSIG("4C 8B 05 ?? ?? ?? ?? 48 C1 C9 03 48"));
		LOG("BLIP_1 = %p", pFnd);
	}

	if(pFnd)
	{
		pFnd = g_Patcher->GetRelativeOffset(pFnd, 3);
		m_Engine.m_blip = pFnd;
		LOG("BLIP_p = %p\r\n", pFnd);
	}
*/








	g_Patcher->Options();






	sdk.Cvar->AddVarBOOL(STN("wtf.god"), &shit.god, 0);


	sdk.CmdGroup->Add("qqq", Cmd_qqq);

	sdk.Cvar->AddVarBOOL(STN("engine.ents_npc"), &Vars.ents_npc, 1);
	sdk.Cvar->AddVarBOOL(STN("engine.ents_veh"), &Vars.ents_veh, 1);
	sdk.Cvar->AddVarBOOL(STN("engine.ents_dead"), &Vars.ents_dead, 1);
	sdk.Cvar->AddVarBOOL(STN("engine.ents_veh_neutral"), &Vars.ents_veh_neutral, 1);
	sdk.Cvar->AddVarBOOL(STN("engine.ents_pickup"), &Vars.ents_pickup, 1);
	sdk.Cvar->AddVarBOOL(STN("engine.check_menu"), &Vars.check_menu, 0);

	sdk.Cvar->AddVarBOOL(STN("bot.nospread"), &Vars.nospread, 0);

	sdk.Cvar->AddVarInt(STN("bot.debug"), &BotDebug, 0, CVAR_FLAG_NOSAVE);
	sdk.Cvar->AddVarInt(STN("bot.aim"), &Bot.aim, 0);//, CVAR_FLAG_NOSAVE, 0);

	sdk.Cvar->AddVarInt(STN("bot.filter"), &Bot.filter, 0);
	sdk.Cvar->AddVarInt(STN("bot.lock"), &Bot.lock, 1);
	sdk.Cvar->AddVarInt(STN("bot.mode"), &Bot.mode, 0);
	sdk.Cvar->AddVarInt(STN("bot.view"), &Bot.view, 1);

	sdk.Cvar->AddVarFloatInit(STN("bot.fov"), &Bot.fov, 10);
	sdk.Cvar->AddVarFloatInit(STN("bot.pred"), &Bot.pred, 0.033333f);
	//sdk.Cvar->AddVarFloat(STN("bot.cyl"), &Bot.cyl, 0);

	sdk.Cvar->AddVarInt(STN("bot.dist_min"), &Bot.dist_min, 0);
	sdk.Cvar->AddVarInt(STN("bot.dist_max"), &Bot.dist_max, 1000);




	m_PlayerHolder.Init();
	m_Players = m_PlayerHolder._List;
	m_PlayersCount = 0;

	m_VehicleHolder.Init();
	m_Vehicles = m_VehicleHolder._List;
	m_VehiclesCount = 0;

	m_EntsHolder.Init();
	m_Ents = m_EntsHolder._List;
	m_EntsCount = 0;

	//InitObjFilter();
	//Targets_Init();

	Esp_init();
	Radar_init();


	sdk._Bones = g_SDK->Bones();

	sdk.Settings->LoadConfig();

	sdk.CmdGroup->Add("goto", Cmd_goto);
	sdk._Loc = g_SDK->Locations( new CLocCB );
	sdk._Names = g_SDK->Names( new CNubCB );


	m_PlayerData = 0;

	SIZE_T trSize = sizeof(Matrix4x4) * MAX_PLAYERS;
	m_PlayerData = (PUCHAR) g_Patcher->AllocateRW(trSize);	//NtAllocateVirtualMemory(NtCurrentProcess(), (PPVOID)&m_PlayerData, 0, &trSize, MEM_COMMIT, PAGE_READWRITE);


	Vec4 *VecPtr = (Vec4 *)m_PlayerData;

	for(ULONG i = 0; i < MAX_PLAYERS; i++)
	{
		m_PlayerHolder._Data[i].m_BaseTransform = (PMatrix4x4)VecPtr;
		VecPtr += 4;
	}


	LOG("Initialized.");

	GameInitialized = 1;
}


CGame::CGame()
{
	Game = this;
}


#include "_sec_end.h" // _init_game_end.h


void CGame::Release()
{
#ifdef _DEV_
	GameInitialized = 0;
	sdk.Valid = 0;

	InputUnLock();

	//Targets_Release();

	g_SDK->OnRelease(sdk, 0);

	Radar_release();
	Esp_release();

	g_Patcher->Release(m_PlayerData);

	g_SDK->OnRelease(sdk, 1);

	delete this;
#endif
}

