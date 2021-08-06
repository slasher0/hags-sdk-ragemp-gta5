#include "game.h"

//#pragma warning(disable: 4311 4312 4200)

INT		cEsp::esp_bar_max_dist;
FLOAT	cEsp::esp_dist_pickup;
FLOAT	cEsp::esp_dist_mine;
FLOAT	cEsp::esp_dist_nade;


/////////////////////////////////////////////////////////////////////////

int __fastcall View_Esp_Lazor(PObject pObject, PVOID Arg)
{
	//PPlayer pPlayer = (PPlayer)pObject;
	//g_Esp->DrawBeam(pPlayer);

	return 1;
}

/////////////////////////////////////////////////////////////////////////

//int __fastcall View_Esp_Name(PObject pObject, PVOID Arg)
//{
//	if(!pObject->Name[0])
//		return 0;
//
//	return g_GuiHud->DrawTextBuff(pObject->Font,
//			pObject->Screen,
//			pObject->Color,
//			pObject->Name,
//			pObject->Flags
//		).cy;
//}

int __fastcall View_Esp_Level(PObject pObject, PVOID Arg)
{
	//PPlayer pPlayer = (PPlayer)pObject;

	//if(!pPlayer->m_Level)
		return 0;

	//return g_GuiHud->DrawTextF(Font,
	//		XY[0],
	//		XY[1],
	//		pPlayer->Color,
	//		Flags,
	//		"%i", pPlayer->m_Level
	//	).cy;
}

int __fastcall View_Esp_Class(PObject pObject, PINT XY, XFONT Font, ULONG Flags, PVOID Arg)
{
	//PPlayer pPlayer = (PPlayer)pObject;

	//ULONG c = pPlayer->m_Class;
	//if(!c)
		return 0;

	//return g_GuiHud->DrawTextBuff(Font,
	//		XY[0],
	//		XY[1],
	//		pPlayer->Color,
	//		(PCSTR)&c,
	//		Flags,
	//		3
	//	).cy;
}

/////////////////////////////////////////////////////////////////////////

INT __fastcall View_Esp_Health(PObject pObject, PVOID Arg)
{
	if(pObject->iDist >= cEsp::esp_bar_max_dist)
		return 0;

	PPlayer pPlayer = (PPlayer)pObject;

	if(pPlayer->m_HealthMax <= 1)
		return 0;

	INT h = sdk._Esp->Bar_Draw(0, &pPlayer->m_Health, *pPlayer).cy;
	return h;
}

INT __fastcall View_Esp_Shield(PObject pObject, PVOID Arg)
{
	//if(pObject->iDist >= cEsp::esp_bar_max_dist)
		return 0;

	//PPlayer pPlayer = (PPlayer)pObject;

	//if(pPlayer->m_ShieldMax <= 1)
	//	return 0;

	//INT h = sdk._Esp->Bar_Draw(1, XY[0], XY[1], pPlayer->m_Shield, pPlayer->m_ShieldMax, pPlayer->fDist, Flags);
	//if(h)
	//	return h;

	//return g_GuiHud->DrawTextF(Font,
	//		XY,
	//		pPlayer->Color,
	//		Flags,
	//		"%u/%u", pPlayer->m_Shield, pPlayer->m_ShieldMax
	//	).cy;
}

/////////////////////////////////////////////////////////////////////////

//int __fastcall View_Esp_Dist(PObject pObject, PVOID Arg)
//{
//	return g_GuiHud->DrawTextF(
//		pObject->Font,
//		pObject->Screen,
//		pObject->Color,
//		pObject->Flags,
//		"%.02f",
//		pObject->fDist ).cy;
//}

/////////////////////////////////////////////////////////////////////////


int __fastcall View_Esp_Bounds(PObject pObject, PVOID Arg)
{
	IESP_VarView *View = (IESP_VarView *)Arg;
	IESP_VarView::IEspBox &b = View->m_Bounds;

	if(b.Alpha)
	if(	b.DistMax >= pObject->iDist &&
		b.DistMin <= pObject->iDist	)
	{
		PPlayer pPlayer = (PPlayer) pObject;
		pPlayer->RenderBounds(b.Alpha);
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////


int __fastcall View_Esp_Hitbox(PObject pObject, PVOID Arg)
{
	IESP_VarView *View = (IESP_VarView *)Arg;
	IESP_VarView::IEspBox &b = View->m_Hitbox;

	if(b.Alpha || b.Alpha2)
	if(	b.DistMax > pObject->iDist &&
		b.DistMin < pObject->iDist	)
	{
		PPlayer pPlayer = (PPlayer)pObject;
		Game->RedrawBones(*pPlayer, b.Alpha);
	}
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////
//
//void __fastcall View_Esp_Bounds_Veh(PObject pObject, PINT XY, XFONT Font, ULONG Flags, PVOID Arg)
//{
//	IESP_VarView *View = (IESP_VarView *)Arg;
//	SDK_ESP::IEspBox &b = *View->_Vars->Bounds;
//
//	if(!b.Alpha)
//		return;
//
//	//if(	b.DistMax > pObject->iDist &&
//	//	b.DistMin < pObject->iDist	)
//	//{
//	//	PVehicle pVeh = (PVehicle) pObject;
//	//	pVeh->RenderBounds(b.Alpha);
//	//}
//}

/////////////////////////////////////////////////////////////////////////

//void __fastcall View_Esp_Hitbox_Veh(PObject pObject, PINT XY, XFONT Font, ULONG Flags, PVOID Arg)
//{
//	IESP_VarView *View = (IESP_VarView *)Arg;
//	SDK_ESP::IEspBox &b = *View->_Vars->Hitbox;
//
//	if(!b.Alpha)
//		return;
//
//	//if(	b.DistMax > pObject->iDist &&
//	//	b.DistMin < pObject->iDist	)
//	//{
//	//	PVehicle pVeh = (PVehicle) pObject;
//	//	pVeh->RenderHitbox(b.Alpha);
//	//}
//}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


void cEsp::Esp_release()
{
	SAFE_RELEASE(Beam);
}

#include "_sec_begin_i.h" // _init_game_begin.h

void cEsp::Esp_init()
{
	Esp = sdk._Esp = g_SDK->Esp();
	
	Beam = g_SDK->CreateBeam();


	Esp->AddVar( TYPES_ARG(
		OBJECT_PC_NEUTRAL,
		OBJECT_PC_FRIEND,
		OBJECT_PC_ENEMY,
		OBJECT_PC_ENEMY_VISIBLE,
		OBJECT_NPC_NEUTRAL,
		OBJECT_NPC_FRIEND,
		OBJECT_NPC_ENEMY,
		OBJECT_NPC_ENEMY_VISIBLE, OBJECT_PICKUP, OBJECT_UNKNOWN).Ref() );

	Esp->SetFlags(IESP_VarView::FLAG__SCREEN_REAL_WORLD_POS | IESP_VarView::FLAG__SCREEN_REAL_WORLD_POS,
		TYPES_ARG(OBJECT_PICKUP, OBJECT_UNKNOWN).Ref() );



	Esp->AddVar("Name",	ESP_NAME_CB,//View_Esp_Name,
		TYPES_ARG(
			OBJECT_PC_FRIEND,		
			OBJECT_PC_NEUTRAL,		
			OBJECT_PC_ENEMY,		
			OBJECT_PC_ENEMY_VISIBLE,
			OBJECT_PICKUP,
			OBJECT_UNKNOWN ).Ref()
		);

	Esp->AddVar("Dist",	ESP_DIST_CB,//View_Esp_Dist,
		TYPES_ARG(
			OBJECT_PC_FRIEND,			
			OBJECT_PC_NEUTRAL,			
			OBJECT_PC_ENEMY,			
			OBJECT_PC_ENEMY_VISIBLE,	
			OBJECT_NPC_NEUTRAL,			
			OBJECT_NPC_FRIEND,			
			OBJECT_NPC_ENEMY,			
			OBJECT_NPC_ENEMY_VISIBLE,	
			OBJECT_PICKUP,				
			OBJECT_UNKNOWN ).Ref() );	

	Esp->AddVar("Health",	View_Esp_Health, TYPES_ARG(
		OBJECT_PC_FRIEND,			
		OBJECT_PC_NEUTRAL,			
		OBJECT_PC_ENEMY,			
		OBJECT_PC_ENEMY_VISIBLE,	
		OBJECT_NPC_NEUTRAL,			
		OBJECT_NPC_FRIEND,			
		OBJECT_NPC_ENEMY,			
		OBJECT_NPC_ENEMY_VISIBLE ).Ref() );



	const int d = 5000;


	Esp->AddVar_Box(View_Esp_Hitbox, View_Esp_Bounds, d, TYPES_ARG(
		OBJECT_PC_NEUTRAL,			
		OBJECT_PC_FRIEND,			
		OBJECT_PC_ENEMY,			
		OBJECT_PC_ENEMY_VISIBLE,	
		OBJECT_NPC_NEUTRAL,		
		OBJECT_NPC_FRIEND,			
		OBJECT_NPC_ENEMY,			
		OBJECT_NPC_ENEMY_VISIBLE) );


	Esp->AddVar_Box(0, View_Esp_Bounds, d, TYPES_ARG(
		OBJECT_PICKUP,
		OBJECT_UNKNOWN));


	//_lstrcpy(Tmp, "Skeleton"));

	//pPC_NEUTRAL	->Hitbox = Esp->Box_Register(OBJECT_PC_NEUTRAL,			Tmp, View_Esp_Hitbox, d);
	//pPC_FRIEND	->Hitbox = Esp->Box_Register(OBJECT_PC_FRIEND,			Tmp, View_Esp_Hitbox, d);
	//pPC_ENEMY	->Hitbox = Esp->Box_Register(OBJECT_PC_ENEMY, 			Tmp, View_Esp_Hitbox, d);
	//pPC_ENEMYVIS->Hitbox = Esp->Box_Register(OBJECT_PC_ENEMY_VISIBLE, 	Tmp, View_Esp_Hitbox, d);

	//pNPC_NEUTRAL->Hitbox = Esp->Box_Register(OBJECT_NPC_NEUTRAL,			Tmp, View_Esp_Hitbox, d);
	//pNPC_FRIEND	->Hitbox = Esp->Box_Register(OBJECT_NPC_FRIEND,			Tmp, View_Esp_Hitbox, d);
	//pNPC_ENEMY	->Hitbox = Esp->Box_Register(OBJECT_NPC_ENEMY, 			Tmp, View_Esp_Hitbox, d);
	//pNPC_ENEMYVIS->Hitbox = Esp->Box_Register(OBJECT_NPC_ENEMY_VISIBLE, 	Tmp, View_Esp_Hitbox, d);


	Esp->Bar_Register(0, "health");
	//Esp->Bar_Register(1, "shield");
	//Esp->Bar_Register(3, "mana");


	Esp->Register();


	sdk.Cvar->AddVarInt(STN("esp.bar_dist"), &esp_bar_max_dist, 50);

	//sdk.Cvar->AddVarFloat(STN("esp.lazor_scale"), &esp_lazor_scale, 0.04f);
	//sdk.Cvar->AddVarFloat(STN("esp.lazor_len"), &esp_lazor_len, 15);

	sdk.Cvar->AddVarFloatInit(STN("esp.dist_pickup"), &esp_dist_pickup, 20);
	sdk.Cvar->AddVarFloatInit(STN("esp.dist_mine"), &esp_dist_mine, 50);
	sdk.Cvar->AddVarFloatInit(STN("esp.dist_nade"), &esp_dist_nade, 20);



}

#include "_sec_end.h" // _init_game_end.h


void cEsp::Esp_Draw()
{
	if(!sdk.Valid)
		return;

	if(!Esp->Ready())
		return;

	//for(ULONG i = 0; i < CGame::m_VehiclesCount; i++)
	//	Draw1(CGame::m_Vehicles[i]);

	//for(ULONG i = 0; i < CGame::m_EntsCount; i++)
	//	Draw1(CGame::m_Ents[i]);

	for(ULONG i = 0; i < CGame::m_PlayersCount; i++)
	{
		PPlayer Player = CGame::m_Players[i];

		//if(esp_npc_filter && Player->Type == OBJECT_NPC_FRIEND && !Player->m_Interact)
		//	continue;


		//if(!CGame::TV::ents_npc.Value())
		//{
		//	if(	Player->Type >= OBJECT_NPC_NEUTRAL &&
		//		Player->Type <=	OBJECT_NPC_ENEMY_VISIBLE	)
		//	{
		//	}
		//	else
		//	{
		//		continue;
		//	}
		//}


		Esp_Draw1(Player);
	}

/*
	Esp->ZEnable(1);

	if(!Beam)
		Beam = sdk._Helpers->CreateBeam();

	for(ULONG i = 0; i < CGame::m_PlayersCount; i++)
		Draw2(CGame::m_Players[i]);

	Esp->ZEnable(0);
//*/
}

VOID cEsp::Esp_Draw1(PPlayer pObj)
{
	//if(!CGame::m_ObjFilter[pObj->m_Type])
	//	return;

	IESP_VarView *pView = Esp->PreDrawObject1(pObj);
	if(!pView)
		return;

	//giW2S ws;
	//if(ws.Project(pObj->World))
	//	g_GuiHud->DrawTextF(0,ws.X,ws.Y-16,pObj->Color|0xFF,0,"k");

	pView->Render1(pObj);
}

VOID cEsp::Esp_Draw2(PPlayer pObj)
{
	IESP_VarView *pView = Esp->PreDrawObject2(pObj);
	if(!pView)
	{
//#ifdef _DEV_
//		giW2S ws;
//		if(ws.Project(pObj->World))
//			g_GuiHud->DrawTextF(0,ws.X,ws.Y,pObj->Color|0xFF,0,"ERR");
//#endif

		return;
	}

	//giW2S ws;
	//if(ws.Project(pObj->World))
	//	g_GuiHud->DrawTextF(0,ws.X,ws.Y,pObj->Color|0xFF,0,"kk");

	pView->Render2(pObj);
}




VOID cEsp::Esp_DrawBeam(PPlayer Player)
{
	return;

	//LinearTransform *pTr = 0;

	//if(Player.m_Weapon && HIWORD(Player.m_Weapon->m_weapon))
	//{
	//	pTr = &Player.m_Weapon->m_weapon->m_shootSpace;
	//}
	//else
	//if(Player.m_Soldier && HIWORD(Player.m_Soldier->m_soldierWeaponsComponent))
	//{
	//	pTr = &Player.m_Soldier->m_soldierWeaponsComponent->m_weaponTransform;
	//}
	//else
	//{
	//	return;
	//}

	//Vector A( &pTr->trans.x );
	//Vector B = A + *(Vector *)&pTr->forward * Vars.beam_length;

	//m_Beam->Draw( *(Vector *)&CGame::m_ViewOrigin, A, B,
	//	Vars.beam_width * 0.01, COLOR_CHANGE_A(Player.Color, Vars.beam_alpha) );

//	Vector Angles;
	Vector Fwd;
	Vector A;
	Vector B;
	


	//A = Player->m_Transform.GetOrigin();// + Player->m_Eye;
	//B = A + Fwd * cEsp::esp_lazor_len;

	//Beam->Draw(
	//	*(Vector *)&CGame::m_ViewOrigin,
	//	B,
	//	A,
	//	cEsp::esp_lazor_scale, Player->Color);
}

