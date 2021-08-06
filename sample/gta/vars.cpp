#include "plug.h"




static void _fastcall Cmd_kek(PVOID arg)
{
	g_Console->Printf("-%s-", arg);

	for(ULONG i = 0, ii = g_Cmd->Argc(); i < ii; i++)
	{
		PCSTR p = g_Cmd->ArgV(i);
		g_Console->Printf("arg %i = '%s'", i, p);
	}
}



#define STN Str_Alloc


VOID CPlug::InitVars()
{
	vars = g_Cmd->GetVars()->CreateGroup();
	cmds = g_Cmd->CreateGroup();
	colors = g_GUI->ColorInterface()->CreateGroup();

	Unhooker = g_Patcher->Unhooker();


	cmds->Add( "kek", Cmd_kek, "kek" );

	vars->AddVarBOOL(STN("kek.change"), &Var.ch, 0);
	vars->AddVarBOOL(STN("kek.bool"), &Var.b, 0);

	vars->AddVarFloatInit(STN("kek.float"), &Var.flt, 0.666);

}



VOID CPlug::RemoveVars()
{
	SAFE_RELEASE(Unhooker);

	SAFE_RELEASE(vars);
	SAFE_RELEASE(cmds);
	SAFE_RELEASE(colors);

}

