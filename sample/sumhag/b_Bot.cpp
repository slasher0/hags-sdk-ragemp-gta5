#include "game.h"

#define _AIM_DBG_ // VK_DELETE


inline BOOL CGame::HandleAim(TPlayer *pTarget)//const Vector &To, const Vector *From)
{
	Vector v;

	//CDataIT *pObjData = pTarget->m_Data;
	//if(!pObjData || !pObjData->phys)
	//	return 0;
	//v = *(Vector*)&pTarget->m_AimOrigin.x + *(Vector*)&pObjData->phys->vel * Bot.pred;

	if(Bot.aim)
		return m_Gravitator.CalcAngles(v, NULL);

	if(Bot.view)
		return m_Gravitator.CalcDrop(v, NULL);

	return 0;
}


VOID CGame::AimFrame()
{
	if(!Bot.lock)
		Bot.target = 0;

	PPlayer pTarget = (Bot.aim || Bot.view || Esp->_target.Use)
		? FindTarget()
		: NULL;

	if(pTarget && Esp->_target.Use)
	{
		pTarget->Color = Esp->_target.Color;
	}


	if(	pTarget &&
		HandleAim(pTarget) )//		HandleAim(&pTarget->m_AimOrigin.x, 0) )
	{
		if(Bot.view)
		{
			Vector vAim = *(Vector *)&pTarget->m_AimOrigin;
			vAim.y += m_Gravitator.Drop;

			niW2S t2s;
			if(t2s.Project(vAim))
				g_GuiHud->DrawOutlinedRect(t2s.X-3, t2s.Y-3, 5, 5, 100, pTarget->Color|0xFF);
		}

		if(Bot.aim)
		{
// do smth

		}
	}

	if(!Bot.aim)
		Bot.target = 0;
	
}

//=================================================================//

__forceinline
LONG CGame::CheckAimErr(TPlayer &Player, INT MinDist, INT MaxDist)
{
	//if(Player.Type == OBJECT_PC_ENEMY_VISIBLE)
	//{
	//}
	//else
	//if(Player.Type == OBJECT_NPC_ENEMY_VISIBLE)
	//{
	//}
	if(Player.m_isEnemy && Player.m_isVisible)
	{
	}
	else
	{
		//if(!Player.m_isVisible)
		//	return STAGE(1);

		return STAGE(1000);
	}

	if(Player.iDist > MaxDist)
		return STAGE(2);

	if(Player.iDist < MinDist)
		return STAGE(3);

	Player.Angle = GetAngleToOrigin(&Player.m_AimOrigin.x);

	if(Player.Angle > Bot.fov)
		return STAGE(666);

	//if(Player._IsNub == 1)
	//	Player.Angle = 0;

	return 0;
}


__declspec(noinline)
PPlayer CGame::FindTarget()
{
	PPlayer pTarget = 0;

	if(Bot.target)
	{
		pTarget = FindPlayer(Bot.target);
		if(pTarget)
		{
			LONG err = CheckAimErr(*pTarget, Bot.dist_min, Bot.dist_max);
			if(err)
			{

#ifdef _AIM_DBG_
#ifdef _DEV_
			if(err < 1000 && GetAsyncKeyState(VK_DELETE))
				g_Console->Printf("AIM: %i %f", err, pTarget->Angle);
#endif
#endif

				return 0;
			}

			return pTarget;
		}
	}

	for(INT i = 0; i < m_PlayersCount; i++)
	{
		PPlayer Player = m_Players[i];
		if(!Player)
			break;

		LONG err = CheckAimErr(*Player, Bot.dist_min, Bot.dist_max);
		if(err)
		{

#ifdef _AIM_DBG_
#ifdef _DEV_
		if(err < 1000 && GetAsyncKeyState(VK_DELETE))
			g_Console->Printf("aim: %i %f", err, Player->Angle);
#endif
#endif

			continue;
		}

		if(Bot.mode)
		{
			if(!pTarget || pTarget->Angle > Player->Angle)
				pTarget = Player;
		}
		else
		{
			if(!pTarget || pTarget->fDist > Player->fDist)
				pTarget = Player;
		}
	}

	return pTarget;
}


PPlayer CGame::FindPlayer(PVOID p)
{
	PPlayer pPlayer = m_PlayerHolder.FindPrev(p);
	return pPlayer;
}


//=================================================================//


FLOAT CGame::GetAngle(const Vector &Diff)
{
	//FLOAT fDist = Diff.Length();
	//if(fDist < FLT(0.001f))
	//	return FLT(180);

	Vec4m diff;
	diff.load3(&Diff.x);

	FLOAT fAngle = Vec4m::VectorAngle(m_ViewForward.vec4(), diff);

//	FLOAT fAngle = RAD2DEGF( m_ViewForward.GetAngle(Diff) );

	//if(Bot.cyl && fDist > Bot.cyl)
	//	return fAngle * (fDist / Bot.cyl);

	return fAngle;
}


FLOAT CGame::GetAngleToOrigin(const Vector &Pos)
{
	Vec4m pos;
	pos.load3(&Pos.x);

	//Pos.GetAngle(Pos);
	//	FLOAT mm = Length() * b.Length();
	//	//if(mm < 0.000001f)
	//	//	return 180.0f;
	//	return mm_fabs( mm_acos( Dot(b) / mm ) );

	FLOAT fAngle = Vec4m::VectorAngle( m_ViewForward.vec4(), pos - m_ViewOrigin.vec4() );

	return fAngle;

//////	Vector vDiff = *(Vector*)&m_ViewOrigin - Pos;
	//Vector vDiff = Pos - *(Vector*)&m_ViewOrigin;
	//return GetAngle(vDiff);

}





////////////////////////////////////////////////////////////////


BOOL TGravitator::CalcDrop(const Vector &To, const Vector *From)
{
	if(Speed < FLT(0.01f) )
	{
		Drop = 0;
		return 0;
	}

	if(From)
	{
		Diff = To - *From;
	}
	else
	{
		//Diff = To - Game->m_Local.m_WeaponOrigin;
		Diff = To - *(Vector *)&CGame::m_ViewOrigin;
	}

	Dist = Diff.Length();

	if(Dist < FLT(0.01f) )
	{
		Drop = 0;
		return 0;
	}

	Time = Dist / Speed;
	Drop = Gravity * FLT(0.5f) * Time * Time;

	return 1;
}

BOOL TGravitator::CalcAngles(const Vector &To, const Vector *From)
{
	if(!CalcDrop(To, From))
		return 0;

	Diff.z += Drop;

	Diff.Normalize();

	Yaw = mm_atan2(Diff.y, Diff.x) - FLT(g_HALF_PI);

	Pitch = mm_atan2(Diff.z, mm_sqrt(Diff.x * Diff.x + Diff.y * Diff.y) );

	Yaw = QAngle::fNormalizeAngle(Yaw);
	Pitch = QAngle::fNormalizeAngle(Pitch);

	return 1;
}



