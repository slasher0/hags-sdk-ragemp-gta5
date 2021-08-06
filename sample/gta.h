#pragma once


#define MAX_PLAYERS		256
#define MAX_VEHICLES	256
#define MAX_HITBOX		16
#define MAX_ENTS		1024


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

#define OBJECT_INVISIBLE	14


class CAutomobile;
class CPhysicalEntity;
class CPed;
class CSkeleton;


struct TVehicle : public TObject
{
	CAutomobile *m_car;
	PVOID nul;
};


struct TEntity : public TObject
{
	CPhysicalEntity *m_Ent;
	PVOID nul;
};



#ifdef _NONTDLL

struct Vec4 // size: 0x010
{
	float x; 	// 0x000
	float y; 	// 0x004
	float z; 	// 0x008
	float w; 	// 0x00C
};

#endif


struct TPlayerBase : public TObject
{
	CPed *m_Ped; // same offs as TVehicle->m_car
	CAutomobile *m_car; // 2nd
	CSkeleton *m_skeleton;

	PMatrix4x4 m_BaseTransform;

	Vec4 m_AimOrigin; // native, bone

	INT	m_Health;
	INT	m_HealthMax;

	INT	m_Armor;
	INT	m_ArmorMax;

	UCHAR	m_isEnemy;
	UCHAR	m_isVisible;
	UCHAR	m_InCombat;
	UCHAR	m_IsNub;

	ULONG	m_Weapon_xxx;
	CHAR	m_Weapon_Name[32-1];

	UCHAR	m_detect_shit_count;

	SDK_BONES::BONESHOLDER m_BonesHolder;
};



enum
{
	cb_begin_update, // arg: local TPlayerBase
	cb_end_update, // arg: gta_shared
	cb_fail_update, // exc
	cb_esp_draw1, // arg:SDK_ESP, ret1 to skip
	cb_esp_end_frame, // arg:err
};

typedef BOOL (_vectorcall* t_gta_cb)(ULONG, PVOID);


struct gta_shared
{
	TEntMap<TPlayerBase, MAX_PLAYERS>	m_PlayerHolder;
	TEntMap<TVehicle, MAX_VEHICLES>		m_VehicleHolder;
	TEntMap<TEntity, MAX_ENTS>			m_EntsHolder;

	t_gta_cb cb;
};


#define G_PLUG_GTA	GLIBID(5,5)


