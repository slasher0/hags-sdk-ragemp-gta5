#include "plug.h"
#include "e_sig.h"



VOID CPlug::Draw1(PObject pObj)
{
	IESP_VarView *pView = Esp->PreDrawObject1(pObj);
	if(!pView)
		return;

	giW2S ws;
	if(ws.Project(pObj->World))
		g_GuiHud->DrawTextF(0,ws.X,ws.Y-16,pObj->Color|0xFF,0,"o");

	pView->Render1(pObj);
}


BOOL CPlug::DrawObjects(SDK_ESP *esp)
{
	if(Var.ch.Changed())
	{
		Var.ch.Submit();
		g_Console->Printf("changed to %i", Var.ch.Value() );
	}

	Esp = esp;

	DrawAll(data->m_PlayerHolder);
	DrawAll(data->m_VehicleHolder);
	DrawAll(data->m_EntsHolder);

	return 1;
}


//
//	cb_begin_update, // arg: local TPlayerBase
//	cb_end_update, // arg: gta_shared
//	cb_fail_update, // exc
//	cb_esp_draw1, // arg:SDK_ESP, ret1 to skip
//	cb_esp_end_frame, // arg:err

BOOL _vectorcall _gta_cb(ULONG wot, PVOID arg)
{
	if(wot == cb_esp_draw1)
		return plug.DrawObjects( (SDK_ESP*)arg );

	if(wot == cb_begin_update)
	{
		TPlayerBase *local = (TPlayerBase *)arg;

		return 0;
	}
	else
	if(wot == cb_end_update)
	{
		gta_shared *ptrs = (gta_shared *)arg;

		return 0;
	}
	else
	if(wot == cb_esp_end_frame)
	{
		ULONG err = (ULONG)(size_t)arg;

		g_GuiHud->SetColor(err ? RGBA(50,0,0,250) : RGBA(0,0,0,250) ); // HUD_STRING_OPAQUE bg
		g_GuiHud->DrawTextF(0, 20,20, RGBA(0,255,0,210), HUD_STRING_OPAQUE, "%02u", err);
	}

	return 0;
}



BOOL CPlug::Plug()
{
	IMainInterface *pInt = g_Interface;

	if(!(data = pInt->Interface_Query<gta_shared*>(G_PLUG_GTA)))
		return 0;

	if(!(g_SDK = pInt->Interface_Query<IHagSdk*>(G_LIB_SDK)))
		return 0;

	if(!g_SDK->OnInit(sdk)) // copy
		return 0;

	if(!sdk._Esp || !sdk._Radar)
		return 0;

	//globals.Update_plug();

	( _LogPath = pInt->GetBasePath() )->Append("__plug.txt", 0);
	Log("plug init %p", data->cb);


	//g_Patcher->Options(IPatcher::P_ERASE_PATTERNS, 1);
	g_Patcher->Options(IPatcher::P_SCAN_STEP, 0);


	GameModuleInfo.init(0);


	tSIG sig;
	PUCHAR pFnd;
	
	
	sig.Init("48 8D 0D ?? ?? ?? ?? 89 44 24 30 E8 ?? ?? ?? ?? 48 83 C4 28 C3 48 8B 05");
	pFnd = g_Patcher->FindPattern(GameModuleInfo.BaseMin, GameModuleInfo.BaseMax, sig);

	if(pFnd)
		pFnd = IPatcher::GetRelativeOffset(pFnd,3);

	Log("m_replay_interface %p", pFnd);



	InitVars();

	_InterlockedExchangePointer( (void**)&data->cb, _gta_cb );
	return 1;
}






VOID CPlug::Unplug()
{
	RemoveVars();

	_InterlockedExchangePointer( (void**)&data->cb, NULL );

	Log("Unplug");

	VirtualFree(_LogBuff, 0, MEM_RELEASE);
	SAFE_RELEASE(_LogPath);
}

