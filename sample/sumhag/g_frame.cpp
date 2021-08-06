#include "game.h"





LONG CGame::Validate()
{
	LONG e;


	//CPed *pLocal = (CPed *) gReadPTR(&Viewport->ped);
	//if(!pLocal)
	//	return STAGE(10);


	//CObjUpd *upd = PreValidate(pLocal);
	//if(!upd)
	//{
	//	m_Local.m_Valid = 0;
	//	return STAGE(90000);
	//}


	//{
	//_MM_ALIGN16 Matrix4x4 proj = *(PMatrix4x4)&Viewport->Proj;
	//*(PULONG64)&proj._31 = 0;
	//const Matrix4x4 &wv = *(PMatrix4x4)&Viewport->WorldView;
	//Matrix4x4 wvp = wv * proj;
	//g_SDK->OnUpdateScreen(&wvp, 0, 0);
	//}

	m_ForceUpdate = !m_ForceUpdateDelay.Check_Restart(450);


	//m_ViewForward.vec() = *(Vector *)&View.Up.x * FLT(-1.0f);
	//m_ViewOrigin = *(VEC4*)&View.trans;
	//m_ViewForward.w = 0;
	//m_ViewOrigin.w = 0;


	e = Validate_LocalPlayer();
	if(e)
	{
		m_Local.m_Valid = 0;
		return STAGE(2000+e);
	}
	else
	{
		m_Local.m_Valid = 1;
	}


	//{
	//Vector &vRight = *(Vector *) &View.Right;//	Vector &vRight = *(Vector *) &Viewport->View.Right;
	//FLOAT Yaw = mm_atan2(vRight.y, vRight.x) + FLT(g_HALF_PI);
	//sdk._Radar->Update( m_Local.World, mm_neg(Yaw) );
	//}



/*
	{
	//Esp->ZEnable(1);
	//if(!Beam)
	//	Beam = g_SDK->CreateBeam();
	//for(int i = 0; i < Pcount; i++)
	//	Beam->Draw( *(Vector *)&CGame::m_ViewOrigin, _Points[i].A, _Points[i].B, 0.066f, _Points[i].c ? _Points[i].c : ~0);
	//Esp->ZEnable(0);
	for(int i = 0; i < Pcount; i++)
	{
		giW2S s;
		if(s.Project( _Points[i].A ))	g_GuiHud->DrawTextF(0,s.X,s.Y,~0,	HUD_STRING_CENTERX|HUD_STRING_CENTERY,"A:%i", i);
		if(s.Project( _Points[i].B ))	g_GuiHud->DrawTextF(0,s.X,s.Y,~0,	HUD_STRING_CENTERX|HUD_STRING_CENTERY,"B:%i", i);
	}

	}
	return 0;
//*/

	//g_GuiHud->DrawTextF(0,100,100,~0,HUD_STRING_OPAQUE_B,"GAME: p:%f y:%f", CGame::m_ViewPitch, CGame::m_ViewYaw);
	//g_GuiHud->DrawTextF(0,100,120,~0,HUD_STRING_OPAQUE_B,"GRAV: p:%f y:%f", m_Gravitator.Pitch, m_Gravitator.Yaw );



	//Vector xxx = *(Vector *)&Viewport->View.trans - *(Vector *)&Viewport->View.forward * 1.2;
	//giW2S s;
	//if(s.Project(xxx))
	//	g_GuiHud->DrawTextF(0,s.X,s.Y,~0,	HUD_STRING_CENTERX|HUD_STRING_CENTERY,"x");
	//*(Vector *)&m_ViewOrigin = xxx;
	//m_Local.m_WeaponOrigin = xxx;//&m_ViewOrigin.x;



#if 0

	//static VEC4 vTMP = {-1330.622192, 2406.865234, 307.299285, 0};
	static VEC4 vTMP = {-18.143432, 29.425292, 307.432342, 0};
	if(GetAsyncKeyState(VK_END))
	{
		//vTMP  camera->Pos);
		//*(Vector *)&vTMP = m_Local.World;
	//	*(Vector *)&vTMP = &local->tr.trans.x;
		*(Vector *)&vTMP = &local->PT.Pos_View.x;

		g_Console->Printf("%f %f %f", vTMP.x, vTMP.y, vTMP.z );

		//g_Console->Printf("T: %f %f %f", local->tr.trans.x, local->tr.trans.y, local->tr.trans.z );
		//g_Console->Printf("P: %f %f %f", local->PT.ViewPos.x, local->PT.ViewPos.y, local->PT.ViewPos.z );
	}

	_MM_ALIGN16 Matrix4x4 m, mm;
	if(!NT_SUCCESS(NtReadVirtualMemory(NtCurrentProcess(), &Viewport->WorldView, &m, sizeof(Matrix4x4), 0)))
		STAGE(222);

	PUCHAR base = (PUCHAR)Viewport;
	for(ULONG o = 1; o < 0x820/16; o++)
	{
		ULONG ofs = o*16;
		PUCHAR ptr = (PUCHAR)base + ofs;

		//if(ofs == 0x190 || ofs == 0x2d0 || ofs == 0x3d0)continue;

		if(!NT_SUCCESS(NtReadVirtualMemory(NtCurrentProcess(), ptr, &mm, sizeof(Matrix4x4), 0)))
			STAGE(222);

		//m.GetOrigin() = &Viewport->Pos.x;

		//mm.GetAxis(2).x = 0;
		//mm.GetAxis(2).y = 0;

		Matrix4x4 mmm = m * mm;
		g_SDK->OnUpdateScreen(&mmm, 0, 0);

		giW2S s;
		if(s.Project(&vTMP.x))
		{
			//if(o&1)s.X -= 20;
			g_GuiHud->DrawTextF(0,s.X,s.Y + (o % 25),~0,0,"%X", ofs );
		}
	}
#endif

	//local->tr;
	//		g_GuiHud->DrawTextF(0,s.X,s.Y + (o % 25),~0,0,"%X", ofs );

	//return 0;


	m_PlayerHolder.Frame(1000);
	m_VehicleHolder.Frame(1000);
	m_EntsHolder.Frame(1000);


// do smth


	m_PlayersCount = m_PlayerHolder._Count;
	m_VehiclesCount = m_VehicleHolder._Count;
	m_EntsCount = m_EntsHolder._Count;



	return 0;
}

