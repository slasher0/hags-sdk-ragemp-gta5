#include "game.h"



int __fastcall RadarCallback_Name(PObject pObject, PVOID Arg)
{
	PPlayer pPlayer = (PPlayer)pObject;

	//return pPlayer->DrawTextBuff(pPlayer->Name) & 0xFF;

	return g_GuiHud->DrawTextBuff(pPlayer->Font, pPlayer->Screen, pPlayer->Color, pPlayer->Name, HUD_STRING_CENTERY).cx;
}


static
void _fastcall RadarDraw()
{
	Game->Radar_Draw();
}

VOID cRadar::Radar_Draw()
{
	if(!sdk.Valid)
		return;

	if(!CGame::m_PlayersCount)
		return;

	Radar->DrawFrame();

	for(ULONG i = 0; i < CGame::m_PlayersCount; i++)
	{
		PPlayer Player = CGame::m_Players[i];

		if(	Player->Type >= OBJECT_PICKUP ) 
		{
			Radar->DrawObject(CGame::m_Players[i], 0);
		}
	}


	if(CGame::TV::ents_npc)
	{
		for(ULONG i = 0; i < CGame::m_PlayersCount; i++)
		{
			PPlayer Player = CGame::m_Players[i];

			if(	Player->Type >= OBJECT_NPC_NEUTRAL &&
				Player->Type <=	OBJECT_NPC_FRIEND	)
			{
				Radar->DrawObject(CGame::m_Players[i], 0);
			}
		}

		for(ULONG i = 0; i < CGame::m_PlayersCount; i++)
		{
			PPlayer Player = CGame::m_Players[i];

			if(	Player->Type >= OBJECT_NPC_ENEMY &&
				Player->Type <=	OBJECT_NPC_ENEMY_VISIBLE	)
			{
				Radar->DrawObject(CGame::m_Players[i], 0);
			}
		}
	}


	for(ULONG i = 0; i < CGame::m_PlayersCount; i++)
	{
		PPlayer Player = CGame::m_Players[i];

		if(	Player->Type == OBJECT_PC_FRIEND )
		{
			Radar->DrawObject(CGame::m_Players[i], 1);
		}
	}

	for(ULONG i = 0; i < CGame::m_PlayersCount; i++)
	{
		PPlayer Player = CGame::m_Players[i];

		if(	Player->Type == OBJECT_PC_NEUTRAL )
		{
			Radar->DrawObject(CGame::m_Players[i], 1);
		}
	}

	for(ULONG i = 0; i < CGame::m_PlayersCount; i++)
	{
		PPlayer Player = CGame::m_Players[i];

		if(	Player->Type == OBJECT_PC_ENEMY )
		{
			Radar->DrawObject(CGame::m_Players[i], 1);
		}
	}

	for(ULONG i = 0; i < CGame::m_PlayersCount; i++)
	{
		PPlayer Player = CGame::m_Players[i];

		if(	Player->Type == OBJECT_PC_ENEMY_VISIBLE )
		{
			Radar->DrawObject(CGame::m_Players[i], 1);
		}
	}

	//int xy[2];
	//Radar->CalcPointByOrigin(&CGame::m_ViewOrigin.x, xy);
	//Radar->DrawPoint(xy, ~0, 2, 0);
	//Vector t = *(Vector*)&CGame::m_ViewOrigin.x + CGame::m_ViewForward * 30;
	//Radar->CalcPointByOrigin(t, xy);
	//Radar->DrawPoint(xy, ~0, 2, 0);

	//for(ULONG i = 0; i < CGame::m_PlayersCount; i++)
	//{
	//	PPlayer Player = CGame::m_Players[i];

	//	if(	Player->Type == OBJECT_PC_ENEMY_VISIBLE )
	//		Radar->DrawObject(CGame::m_Players[i], 1);
	//}


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void cRadar::Radar_release()
{
}

#include "_sec_begin_i.h" // _init_game_begin.h

void cRadar::Radar_init()
{
	Radar = sdk._Radar = g_SDK->Radar();

	Radar->m_Swap = 1;

	Radar->AddView("name", RadarCallback_Name);

	Radar->Register( 5000, RadarDraw);

}

#include "_sec_end.h" // _init_game_end.h


