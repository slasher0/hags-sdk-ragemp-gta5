#pragma once


//---------------------------------------------------------


struct TGravitator
{
	TGravitator()
		: Gravity(0)
		, Speed(4000.0f)
		, Drop(0)
		, Dist(0)
		, Time(0)
		, MaxSpeed(0)
		, Accel(0)
		, Range(0)
	{
	}
	Vector	Diff;
	BOOL	CalcAngles(const Vector &To, const Vector *From = 0);
	BOOL	CalcDrop(const Vector &To, const Vector *From = 0);
	float	Gravity;
	float	Speed;
	float	Drop;
	float	Dist;
	float	Time;
	float	Yaw;
	float	Pitch;
	float	MaxSpeed;
	float	Accel;
	int		Range;
};

//---------------------------------------------------------

enum ObjRelation
{
	e_Unknown = -1,
	e_Passive,
	e_Enemy,
	e_Friend,
};

typedef struct TVehicle : public TObject
{

};

typedef struct TEntity : public TObject
{

};

typedef struct TPlayer : public TObject
{

	PMatrix4x4 m_BaseTransform;

	Vec4	m_AimOrigin; // native, bone

	VOID	RenderBounds(INT Alpha1);

	UCHAR m_isEnemy;
	UCHAR m_isVisible;
	
	UCHAR __fsdgh[2];

	ObjRelation m_RelWLocal;

	INT	m_Health;
	INT	m_HealthMax;

	SDK_BONES::BONESHOLDER m_BonesHolder;

	UCHAR InCombatWith(TPlayer &);

};


typedef struct TLocal : public TPlayer
{
	static bool m_Valid;

	ObjRelation GetRelationWith(TPlayer &);

	VOID Teleport(const Vector &);
	BOOL	m_TP;
	Vector	m_TPos;
};


//---------------------------------------------------------

class CGame : public IPluginInterface, public cEsp, public cRadar
{
public:

	struct
	{
		TCvarBool god;

	} shit;


public:
	CGame();

public:
	BOOL OnInit();
	BOOL OnDraw();
	void OnHalt();

public:
	void Init();
	void Init_OBJ();

	void Release();
	void Redraw();

	LONG Validate();

	LONG m_Stage;
	LONG m_Error;

	static
	ULONG	m_Frame;

	static 
	bool	m_ForceUpdate;
	cTimer	m_ForceUpdateDelay, m_FailTimer;

public:
	VOID	RedrawBones(TPlayer &, INT);
	VOID	UpdateBones(TPlayer &, bool);

public:
	static PPPlayer	m_Players;
	static ULONG	m_PlayersCount;

	static TVehicle **	m_Vehicles;
	static UINT	 		m_VehiclesCount;

	static TEntity **	m_Ents;
	static UINT	 		m_EntsCount;

	TGravitator m_Gravitator;
	BOOL		m_HaveValidWeapon;

	static VEC4		m_ViewOrigin;
	static VEC4		m_ViewForward;

	static FLOAT	m_ViewPitch;
	static FLOAT	m_ViewYaw;

	Vector		m_Forward;

public:
	LONG Validate_LocalPlayer();
	LONG Validate_Player();

public:

	struct
	{
		//XVAR<CViewportPtr *> m_ViewportPtr;
		//XVAR<CCameraPtr *> m_CamPtr;
		//XVAR<PUCHAR> m_HWNDptr;
		//XVAR<CDataHldrPtr *> m_data;
		//XVAR<PVOID> m_blip;
	} m_Engine;


	struct TV
	{
		TCvarBool ents_pickup;
		TCvarBool ents_veh;
		TCvarBool ents_veh_neutral;

		TCvarBool check_menu;

		static TCvarBool ents_npc;
		TCvarBool ents_dead;

		TCvarBool nospread;

	} Vars;

public:
	TLocal		m_Local;

public:

	BOOL HandleAim(TPlayer *);
	VOID AimFrame();
	PPlayer FindTarget();
	PPlayer FindPlayer(PVOID);
	FLOAT GetAngleToOrigin(const Vector &);
	FLOAT GetAngle(const Vector &);

	LONG CheckAimErr(TPlayer &Player, INT MinDist, INT MaxDist);


	struct
	{
		int   lock;
		int   aim, view;
		int   filter; // 0 enemy, 1 ally, 2 both
		int   mode;
		float fov;
		float pred;
		int   dist_min;
		int   dist_max;
		PVOID target; // obj ptr
	} Bot;

	static int   BotDebug;

public:

	PUCHAR	m_PlayerData; //16b aligned mem

public:

	TEntMap<TPlayer, MAX_PLAYERS>		m_PlayerHolder;
	TEntMap<TVehicle, MAX_VEHICLES>		m_VehicleHolder;
	TEntMap<TEntity, MAX_ENTS>			m_EntsHolder;

	VOID InputLock();
	VOID InputUnLock();
	HCURSOR m_OrigCursor;
	ULARGE_INTEGER m_OrigWnd;
};

extern BOOL		GameInitialized;
extern CGame *	Game;



void goto_fail();
void goto_print(const Vector &pos);
