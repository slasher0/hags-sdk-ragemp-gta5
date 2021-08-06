#include "game.h"





LONG CGame::Validate_Player()
{
	//Vector Pos = &pObj->tr_World.trans.x;

	//giW2S s;
	//if(!s.Project(Pos))
	//	return 1;

	//g_GuiHud->DrawTextF(1,s.X,s.Y+12,~0,0,"%X", pObj );

	return 0;

/*
	PPlayer pPlayer = m_PlayerHolder.Get(pObj);
	if(!pPlayer)
		return STAGE(100);
	TPlayer &Player = *pPlayer;

	bool ForceUpdatePed = (Player.m_Ped != pObj);
	bool ForceUpdate = ForceUpdatePed || m_ForceUpdate;

	Player.m_Health = pHealth->fCur;
	Player.m_HealthMax = pHealth->fMax;

	Player.World = &pObj->PT.Pos_View.x;

	float fDist = (m_Local.World - Player.World).Length();
	Player.fDist = fDist;
	Player.iDist = mm_float2int(fDist);

	if(ForceUpdate)
	{
		if(pInfo)
			_lstrncpy(Player.Name, pInfo->name, ARRAY_COUNT(Player.Name)-1);
		else
			Player.Name[0] = 0;

		ObjRelation RelWLocal;

		ULONG e;
		if(pInfo && sdk._Names && (e = sdk._Names->IsNub(Player.Name)) && e <= 2)
		{
			RelWLocal = (ObjRelation)e;
		}
		else
		{
			RelWLocal = m_Local.GetRelationWith(Player);

			// ally filter: 0 enemy, 1 ally, 2 both
			if(Bot.filter == 2)
			{
				Player.m_isEnemy = 1;

				RelWLocal = e_Enemy;
			}
			else
			{
				Player.m_isEnemy = (RelWLocal == e_Enemy);

				if(Bot.filter == 1)
					Player.m_isEnemy = !Player.m_isEnemy;
			}
		}

		Player.m_RelWLocal = RelWLocal;
	}


	if(ForceUpdate || Player.m_InCombatWith)
	{
		UCHAR c = m_Local.InCombatWith(Player);

		c |= Player.InCombatWith(m_Local);

		Player.m_InCombatWith = c;
	}


//	if(ForceUpdatePed)
//	{
//		LONG e = BonesUpdateIndexes(Player);
//#ifdef _DEV_
//		giW2S s;
//		if(s.Project(Player.World))
//			g_GuiHud->DrawTextF(1,s.X,s.Y-32,RGBA(255,0,0,255),0,"%i", e);
//#endif
//	}

	if(Player.m_InCombatWith)
	{
		Player.m_isVisible = Player.m_InCombatWith & 2;
		Player.m_isEnemy = 1;
		Player.m_RelWLocal = e_Enemy;
	}
	else
	{
		Player.m_isVisible = 1;
	}

	ObjRelation RelWLocal = Player.m_RelWLocal;

	if(pInfo)
	{
		if(RelWLocal == e_Enemy)
		{
			Player.Type = Player.m_isVisible ? OBJECT_PC_ENEMY_VISIBLE : OBJECT_PC_ENEMY;
		}
		else
		if(RelWLocal == e_Friend)
		{
			Player.Type = OBJECT_PC_FRIEND;
		}
		else
		{
			Player.Type = OBJECT_PC_NEUTRAL;
		}
	}
	else
	{
		if(RelWLocal == e_Enemy)
		{
			Player.Type = Player.m_isVisible ? OBJECT_NPC_ENEMY_VISIBLE : OBJECT_NPC_ENEMY;
		}
		else
		if(RelWLocal == e_Friend)
		{
			Player.Type = OBJECT_NPC_FRIEND;
		}
		else
		{
			Player.Type = OBJECT_NPC_NEUTRAL;
		}
	}


/*
	if(pSkeleton)
	{
		g_GuiHud->DrawSetColor(Player.Color | 0x255);

		LinearTransform *ptr = pSkeleton->trs->tr;

		for(int i = 0; i < pSkeleton->Count && i < 168; i++)
		{
			CSkelBone &bone = pSkeleton->m_data->m_bones->bones[i];

			if(!bone.name)
				continue;

			if(!_lstrstri(bone.name, "head"))
				continue;

			Vec4 Pos = ptr[i].trans;
			Pos.w = 1;

			TransformTransposeVec4(&Pos.x, *Player.m_BaseTransform, &Pos.x);

			_MM_ALIGN16 Matrix4x4 m;
			MatrixInitTranspose( (PFLOAT)&ptr[i], &Pos.x, (PFLOAT)&m );

			const float f = 0.04f;
			Vector m0(-f,-f,-f);
			Vector m1( f, f, f);

			sdk._Esp->Box_Render_native(&m0.x, &m1.x, m);

			//giW2S s;
			//if(s.Project(&Pos.x))
			//	g_GuiHud->DrawTextF(0,s.X,s.Y+15,~0,0,"%s", bone.name );
		}
	}
//*/


	//UpdateBones(Player, ForceUpdatePed);
	//RedrawBones(Player, 200);


#if 0
	giW2S s;
	if(s.Project(Player.World))
	{
		g_GuiHud->DrawTextF(0,s.X,s.Y,~0,0,"%f", GetAngleToOrigin(Player.World) );
	}
#endif

	//Player.Color = GetColor(Player.Type);
	return 0;
}




VOID TPlayer::RenderBounds(INT Alpha1)
{
	return;
/*
	if(! m_Ped->m_model )
		return;

	_MM_ALIGN16 Matrix4x4 m;

	_memcpy(&m, &m_Ped->tr_World, sizeof(Vec4)*3 );
	//m.SetIdentity();
	_memcpy(&m._41, &m_Ped->PT.Pos_View, 3*4);//sizeof(Vec4) );
	m._44 = 1;

	MatrixTranspose(m, m);

	//const float f = 1;
	//Vector m0 = World + Vector(f,f,f);//&m_AvatarGM->m_creatureCollision->m0.x;
	//Vector m1 = World - Vector(f,f,f);//&m_AvatarGM->m_creatureCollision->m1.x;
	////Vector m0 = World + *(Vector *) &m_Ped->m_model->m0;
	////Vector m1 = World + *(Vector *) &m_Ped->m_model->m1;

	//sdk._Esp->Box_Render(
	//	m0,
	//	m1);

	Vector mi = *(Vector *)&m_Ped->m_model->aabbMin * FLT(0.95f);
	Vector ma = *(Vector *)&m_Ped->m_model->aabbMax;

	//ma.z -= 0.0666f;
	//float d = m_Ped->m_model->aabbUnk.z - ma.z;
	//if(d > 0)
	//	mi.z += d;

	sdk._Esp->Box_Render_native(
		&mi.x,
		&ma.x,
		m,
		0);*/
}




/*
	if(!CGame::BotDebug)
		return 0;


	ColorHid = COLOR_CHANGE_A(Color, 60);
	ColorVis = COLOR_CHANGE_A(Color, 180);
	

	if(CGame::BotDebug == 1)
	{
		for(ULONG i = 0; i < CGame::m_HitBoxCount; i++)
		{
			CGame::THitBox &hb = CGame::m_HitBox[i];

			{
			ULONG t = m_BoneIndexes[i];

			CSkelBone &bone = m_skeleton->m_data->m_bones->bones[t];
			if(!bone.name)
				continue;
			Vec4 Pos = ptr[t].trans;
			Pos.w = 1;
			TransformTransposeVec4(&Pos.x, *m_BaseTransform, &Pos.x);
			_MM_ALIGN16 Matrix4x4 mm;
			MatrixInitTranspose( (PFLOAT)&ptr[t], &Pos.x, (PFLOAT)&mm );


			ULONG c;
			if(pVis0[t] || pVis1[t])
				c = COLOR_CHANGE_A(~0, 150);
			else
				c = COLOR_CHANGE_A(~0, 50);
			g_GuiHud->DrawSetColor(c);

			sdk._Esp->Box_Render_native(
				&VMin0.x,
				&VMax0.x, mm );

			niW2S ws;
			if(ws.Project( &Pos.x ))
			{
				g_GuiHud->DrawTextBuff(1, ws.X, ws.Y, c, bone.name);
			}
			}

		}
	}
	else
	{

		for(INT i = 0; i < m_skeleton->Count; i++)
		{
			CSkelBone &bone = m_skeleton->m_data->m_bones->bones[i];

			Vec4 Pos = ptr[i].trans;
			Pos.w = 1;
			TransformTransposeVec4(&Pos.x, *m_BaseTransform, &Pos.x);
			//_MM_ALIGN16 Matrix4x4 m;
			//MatrixInitTranspose( (PFLOAT)&ptr[t], &Pos.x, (PFLOAT)&m );

			niW2S ws;
			if(ws.Project( &Pos.x ))
				g_GuiHud->DrawTextF(1,ws.X,ws.Y,Color|255,0,"%i.%s",i,bone.name);
		}
	}



	return 0;
}
*/




