#include "game.h"

class CAnimations;

BOOL __fastcall __BonesUpdate(SDK_BONES::TBONEID *Arg, CAnimations *pAnim)
{
	//crSkeletonData *pSkel = pAnim->arr[0];
	//CSkelBone* bones = pSkel->m_bones->bones; 	// 0x028

	//for(ULONG i = 0; i < pSkel->size; i++)
	//{
	//	CSkelBone &bone = bones[i];

	//	//if(!pbid->ID[i])
	//	//	continue;
	//	//ULONG ID = pbid->ID[i];
	//	//if(!BoneMap(ID, pSkel))
	//	//	continue;

	//	sdk._Bones->OnUpdateBones(Arg, bone.name, i);
	//}

	return 1;
}


BOOL __fastcall __BonesViewUpdate(SDK_BONES::TBONEIDVIEW *Arg, CAnimations *pAnim)
{
	//crSkeletonData *pSkel = pAnim->arr[0];
	//CSkelBone* bones = pSkel->m_bones->bones;

	//for(ULONG i = 0; i < pSkel->size; i++)
	//{
	//	CSkelBone &bone = bones[i];

	//	sdk._Bones->OnUpdateBonesView(Arg, bone.name, i);
	//}

	return 1;
}











VOID CGame::UpdateBones(TPlayer &Player, bool ForceUpdate)
{
/*	if(!Player.m_anim)
		return;

	if(m_ForceUpdate || ForceUpdate)
		Player.m_BonesHolder.Clear();

	SDK_BONES::TBONEID *pbid = sdk._Bones->GetBones(0, Player.m_anim, &Player.m_BonesHolder, (SDK_BONES::tBonesUpdate)__BonesUpdate);
	if(	!pbid )
		return;

	//if(!pbid->Ready || m_ForceUpdate || ForceUpdate)
	//	pbid->Ready = __BonesUpdate(pbid, Player.m_skeleton);

	if(!pbid->Ready)
		return;

	CAnimations *pAnim = Player.m_anim;
	tBonePos *ppos = &pAnim->pos->arr[ pAnim->pos_offset ];
	crSkeletonData *pSkel = pAnim->arr[0];

	for(ULONG i = 0, ii = sdk._Bones->GetBonesCount(); i < ii; i++)
	{
		if(!pbid->ID[i])
			continue;

		ULONG ID = pbid->ID[i];

		if(!BoneMap(ID, pSkel))
			continue;

		Vec4 Pos;
		*(Vector *)&Pos = *(Vector *) &ppos[ ID ].Pos;
		*(PULONG)&Pos.w = FLD(1);
		Pos.z += FLT(0.0666f);
		TransformTransposeVec4(&Pos.x, *Player.m_BaseTransform, &Player.m_AimOrigin.x);

		break;
	}
*/
}

//////////////////////////////////////////////////////////





VOID CGame::RedrawBones(TPlayer &Player, INT Alpha)
{
/*	if(!Player.m_anim)
		return;

	g_GuiHud->DrawSetColor(COLOR_CHANGE_A(Player.Color, Alpha));


	SDK_BONES::TBONEIDVIEW *pbidv = sdk._Bones->GetBonesView(0, Player.m_anim, &Player.m_BonesHolder, (SDK_BONES::tBonesViewUpdate)__BonesViewUpdate);
	if(!pbidv)
		return;
	if(!pbidv->Ready)
		return;

	cGuiHud *pGuiHud = g_GuiHud;

	CAnimations *pAnim = Player.m_anim;
	tBonePos *ppos = &pAnim->pos->arr[ pAnim->pos_offset ];
	crSkeletonData *pSkel = pAnim->arr[0];

	Vec4 Pos;

	FLOAT pxy[ HUD_MAX_POINTS*2 ];

	for(ULONG iv = 0, ic = sdk._Bones->GetBonesCount(); iv < ic; iv++)
	{
		LONG PointsCount = 0;
		UINT vc = sdk._Bones->GetBonesViewCount(iv);

		for(ULONG ib = 0; ib < vc; ib++)
		{
			ULONG ID = pbidv->ID[iv][ib];

			if(!BoneMap(ID, pSkel))
				continue;

			*(Vector *)&Pos = *(Vector *) &ppos[ ID ].Pos;
			*(PULONG)&Pos.w = FLD(1);

			TransformTransposeVec4(&Pos.x, *Player.m_BaseTransform, &Pos.x);
			
			if(sdk.NF_ScreenProjectPoint(&Pos.x, &pxy[PointsCount]))
				PointsCount += 2;
		}

		if(!PointsCount)
			continue;

		pGuiHud->DrawPolyLine(pxy, PointsCount/2);
	}


#ifdef _DEV_
	if(!BotDebug)
		return;

	for(SIZE_T i = 0; i < pSkel->size; i++)
	{
		CSkelBone &bone = pSkel->m_bones->bones[i];

		if((bone.name[0] | 0x20) != 's')
			continue;
		if((bone.name[1] | 0x20) != 'k')
			continue;

		SIZE_T ii = pSkel->m_ids0[i];

		if(ii > pSkel->ids_count)
			continue;

		*(Vector *)&Pos = *(Vector *) &ppos[ ii ].Pos;
		*(PULONG)&Pos.w = FLD(1);

		TransformTransposeVec4(&Pos.x, *Player.m_BaseTransform, &Pos.x);

		niW2S ws;
		if(ws.Project( &Pos.x ))
			pGuiHud->DrawTextBuff(1, &ws.X, Player.Color|255, bone.name);
	}
#endif

*/
}

