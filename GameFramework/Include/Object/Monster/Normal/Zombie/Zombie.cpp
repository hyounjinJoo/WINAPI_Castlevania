
#include "Zombie.h"
#include "../../../../Scene/Scene.h"
#include "../../../../Collision/ColliderBox.h"
#include "../../../../Scene/SceneResource.h"
#include "../../../../UI/WidgetComponent.h"
#include "../../../../UI/NumberWidget.h"
#include "../../../DamageFont.h"
#include "../../../Effect/BloodEffect.h"
#include "../../../EffectHit.h"
#include "BloodTrail.h"

CZombie::CZombie() :
	m_AttackEnable(false),
	m_DeathTime(1.0f),
	m_CreatePos(Vector2(0.f, 0.f)),
	m_IsZombieAType(false),
	m_ReCreateTime(40.f),
	m_NoDamageTime(0.f)
{
	m_MonsterType = EMonsterType::NormalGround;
	m_PhysicsSimulate = true;
}

CZombie::CZombie(const CZombie& obj) :
	CMonster(obj)
{
	m_AttackEnable = obj.m_AttackEnable;
	m_DeathTime = 1.0f;
	m_CreatePos = Vector2(0.f, 0.f);
	m_ReCreateTime = 40.f;
	m_NoDamageTime = 0.f;
}

CZombie::~CZombie()
{
}

void CZombie::Start()
{
	CMonster::Start();

	AnmationNotifySetting();

	BodyCollisionSetting();

	m_CreatePos = m_Pos;

	float resultY = GetFloorTileY(m_CreatePos.y);
	resultY != 0.f ? m_Pos.y = resultY : m_Pos.y = m_Pos.y;
	m_FallStartY = m_Pos.y;
	m_PrevPos.y = m_Pos.y;

	m_CreatePos.y = resultY;

	m_ReCreateTime = 40.f;
}

bool CZombie::Init()
{
	if (!CMonster::Init())
		return false;

	SetPivot(0.5f, 1.0f);

	CreateAnimation();
	AddAnimation("ZombieLeftCreate", false, 1.5f);
	AddAnimation("ZombieALeftWalk", true, 2.f);
	AddAnimation("ZombieBLeftWalk", true, 2.f);

	AddAnimation("ZombieRightCreate", false, 1.5f);
	AddAnimation("ZombieARightWalk", true, 2.f);
	AddAnimation("ZombieBRightWalk", true, 2.f);

	AddAnimation("ZombieLeftDig", false, 1.5f);
	AddAnimation("ZombieRightDig", false, 1.5f);

	AddAnimation("ZombieLeftDeath", false, 0.6f);
	AddAnimation("ZombieRightDeath", false, 0.6f);

	m_CharacterInfo.HP = 18;
	m_CharacterInfo.HPMax = 18;
	m_CharacterInfo.MP = 10;
	m_CharacterInfo.MPMax = 10;
	m_CharacterInfo.Exp = 8;
	m_CharacterInfo.Attack = 9;
	m_CharacterInfo.Armor = 1;
	m_Size = Vector2(92.f, 144.f);
	m_MoveSpeed = 20.f;

	SetSideWallCheck(true);
	return true;
}

void CZombie::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	if (m_ReCreateTime > 0.f)
	{
		m_ReCreateTime -= DeltaTime;

		if (m_ReCreateTime < 0.f)
			m_ReCreateTime = 0.f;
	}

	if (m_CharacterInfo.HP <= 0)
	{
		m_AI = EMonsterAI::Death;
	}

	switch (m_AI)
	{
	case EMonsterAI::Idle:
		break;
	case EMonsterAI::Detect:
		break;
	case EMonsterAI::Trace:
		break;
	case EMonsterAI::Attack:
		break;
	case EMonsterAI::Death:
		AIDeath(DeltaTime);
		break;
	default:
		break;
	}

	if (m_Dir.x != 0.f)
	{
		if (abs(m_Pos.x - m_CreatePos.x) > 300.f
			|| m_ReCreateTime == 0.f)
		{
			m_ReCreateTime = 40.f;
			Dig();
		}
	}

	if (m_Scene->GetPlayer())
	{
		if (m_NoDamageTime <= 0.f)
		{
			m_NoDamageTime = 0.f;
		}
		else
		{
			m_NoDamageTime -= DeltaTime;
		}
	}
}

void CZombie::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CZombie::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CZombie::Render(HDC hDC)
{
	CMonster::Render(hDC);
}

CZombie* CZombie::Clone()
{
	return new CZombie(*this);
}

float CZombie::SetDamage(float Damage)
{
	Damage = CMonster::SetDamage(Damage);

	if (Damage <= 0.f)
	{
		return 0.f;
	}

	CEffectHit* Hit = m_Scene->CreateEffectObject<CEffectHit>("HitEffect", "HitEffect",
		GetMiddle());
	
	CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont",
		GetMiddleTop());

	DamageFont->SetDamageNumber((int)Damage);

	return Damage;
}

void CZombie::WalkBegin()
{
	m_CreatePos = m_Pos;

	m_IsZombieAType = rand() % 2 == 0 ? true : false;

	if (CheckCurrentAnimation("ZombieLeftCreate"))
	{
		m_Dir = Vector2(-250, 0);
		m_IsZombieAType ? ChangeAnimation("ZombieALeftWalk") : ChangeAnimation("ZombieBLeftWalk");
	}
	else if(CheckCurrentAnimation("ZombieRightCreate"))
	{
		m_Dir = Vector2(250, 0);
		m_IsZombieAType ? ChangeAnimation("ZombieARightWalk") : ChangeAnimation("ZombieBRightWalk");
	}
}

void CZombie::Death()
{
	m_Scene->GetPlayer()->SetExp(m_CharacterInfo.Exp);
	Destroy();
}

void CZombie::Walk()
{
	bool IsLeftWalk = m_Dir.x < 0.f ? true : false;

	if (IsTileWallExist(IsLeftWalk))
	{
	}
	else 
	{
		Move(m_Dir);
	}
}

void CZombie::ReCreate()
{
	m_Pos.x = m_CreatePos.x;
	float resultY = GetFloorTileY(m_CreatePos.y);
	resultY != 0.f ? m_Pos.y = resultY : m_Pos.y = m_Pos.y;
	m_FallStartY = m_Pos.y;
	m_PrevPos.y = m_Pos.y;

	float	PlayerDist = 0.f;

	if (m_Scene->GetPlayer())
		PlayerDist = m_Scene->GetPlayer()->GetPos().x - m_Pos.x;

	if (PlayerDist > 0.f)
	{
		m_Dir.x = 250.f;

		ChangeAnimation("ZombieRightCreate");
	}
	else
	{
		m_Dir.x = -250.f;

		ChangeAnimation("ZombieLeftCreate");
	}
}

void CZombie::Dig()
{
	if (m_Dir.x > 0.f)
	{
		ChangeAnimation("ZombieRightDig");
	}
	else
	{
		ChangeAnimation("ZombieLeftDig");
	}
}

void CZombie::BloodEffect()
{
	BloodTrail();
	BloodDrop();
}

void CZombie::BloodTrail()
{
	Vector2 TrailPos = m_Pos - Vector2(m_Size.x / 2.f, m_Size.y);;

	if (m_Dir.x > 0.f)
		TrailPos = m_Pos - Vector2(20.f, 2.f);
	else
		TrailPos = m_Pos - Vector2(-20.f, 2.f);

	CSharedPtr<CBloodTrail> Blood = m_Scene->CreateEffectObject<CBloodTrail>("BloodTrailEffect",
		"BloodTrailEffect",
		TrailPos,
		Vector2(124.f, 4.f));
}

void CZombie::BloodDrop()
{
	Vector2 BloodDropPos = m_Pos - Vector2(m_Size.x /2.f, m_Size.y + 20.f);

	if (CheckCurrentAnimation("ZombieALeftWalk"))
		BloodDropPos += Vector2(80.0f, 27.f);
	else if (CheckCurrentAnimation("ZombieBLeftWalk"))
		BloodDropPos += Vector2(12.0f, 60.f);
	else if (CheckCurrentAnimation("ZombieARightWalk"))
		BloodDropPos += Vector2(8.0f, 27.f);
	else
		BloodDropPos += Vector2(64.0f, 60.f);

	CSharedPtr<CBloodEffect> Blood = m_Scene->CreateObject<CBloodEffect>("BloodEffect",
		"BloodDropEffect",
		BloodDropPos,
		Vector2(8.f, 16.f));
}

void CZombie::AnmationNotifySetting()
{
	SetAnimationEndNotify<CZombie>("ZombieLeftCreate", this, &CZombie::WalkBegin);
	SetAnimationEndNotify<CZombie>("ZombieRightCreate", this, &CZombie::WalkBegin);
	SetAnimationEndNotify<CZombie>("ZombieLeftDig", this, &CZombie::ReCreate);
	SetAnimationEndNotify<CZombie>("ZombieRightDig", this, &CZombie::ReCreate);
	SetAnimationEndNotify<CZombie>("ZombieLeftDeath", this, &CZombie::Death);
	SetAnimationEndNotify<CZombie>("ZombieRightDeath", this, &CZombie::Death);

	AddAnimationNotify<CZombie>("ZombieALeftWalk", 0, this, &CZombie::BloodEffect);
	AddAnimationNotify<CZombie>("ZombieBLeftWalk", 0, this, &CZombie::BloodEffect);
	AddAnimationNotify<CZombie>("ZombieALeftWalk", 1, this, &CZombie::Walk);
	AddAnimationNotify<CZombie>("ZombieBLeftWalk", 1, this, &CZombie::Walk);
	AddAnimationNotify<CZombie>("ZombieALeftWalk", 2, this, &CZombie::Walk);
	AddAnimationNotify<CZombie>("ZombieBLeftWalk", 2, this, &CZombie::Walk);
	AddAnimationNotify<CZombie>("ZombieARightWalk", 0, this, &CZombie::Walk);
	AddAnimationNotify<CZombie>("ZombieBRightWalk", 0, this, &CZombie::Walk);
	AddAnimationNotify<CZombie>("ZombieARightWalk", 1, this, &CZombie::Walk);
	AddAnimationNotify<CZombie>("ZombieBRightWalk", 1, this, &CZombie::Walk);
	AddAnimationNotify<CZombie>("ZombieARightWalk", 2, this, &CZombie::BloodEffect);
	AddAnimationNotify<CZombie>("ZombieBRightWalk", 2, this, &CZombie::BloodEffect);
}

void CZombie::BodyCollisionSetting()
{
	CColliderBox* Body = AddCollider<CColliderBox>("ZombieBody");
	Body->SetExtent(92.f, 144.f);
	Body->SetOffset(0.f, -72.f);
	Body->SetCollisionProfile("Monster");
	Body->SetCollisionBeginFunction<CZombie>(this, &CZombie::BodyCollisionBegin);
	Body->SetCollisionWhileFunction<CZombie>(this, &CZombie::BodyCollisionWhile);
}

void CZombie::DirectionSetting()
{
	CGameObject* Player = m_Scene->GetPlayer();

	// 주변에 플레이어가 있는지 판단한다.
	// 플레이어와의 거리를 구한다.
	//float	Dist = Distance(Player->GetPos(), m_Pos);
	float	Dist = 0.f;
	if (Player)
		Dist = Player->GetPos().x - m_Pos.x;

	if (Dist <= 0.f)
	{
		SetCurrentAnimation("ZombieLeftCreate");
		m_Dir = Vector2(-250.f, 0.f);
	}
	else
	{
		SetCurrentAnimation("ZombieRightCreate");
		m_Dir = Vector2(250.f, 0.f);
	}
}

void CZombie::AIIdle(float DeltaTime)
{
}

void CZombie::AIDetect(float DeltaTime)
{
}

void CZombie::AITrace(float DeltaTime)
{
}

void CZombie::AIAttack(float DeltaTime)
{
}

void CZombie::AIDeath(float DeltaTime)
{
	m_CharacterInfo.Attack = 0;
	if (m_Dir.x > 0.f)
	{
		m_Dir.x = 0.f;
		ChangeAnimation("ZombieRightDeath");

	}
	else if(m_Dir.x < 0.f)
	{
		m_Dir.x = 0.f;
		ChangeAnimation("ZombieLeftDeath");
	}
}

void CZombie::BodyCollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (Dest->GetOwner()->GetName() != "Player")
		return;

	if (Dest->GetProfile()->Name != "Player")
		return;

	if (m_NoDamageTime <= 0.f)
	{
		Dest->GetOwner()->SetDamage((float)m_CharacterInfo.Attack);
		m_NoDamageTime = 1.f;
	}
}

void CZombie::BodyCollisionWhile(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (Dest->GetOwner()->GetName() != "Player")
		return;

	if (Dest->GetProfile()->Name != "Player")
		return;

	if (m_NoDamageTime <= 0.f)
	{
		Dest->GetOwner()->SetDamage((float)m_CharacterInfo.Attack);
		m_NoDamageTime = 1.f;
	}
}

bool CZombie::IsTileWallExist(bool IsLeftMove)
{
	if (!m_Scene->GetTileMap())
		return 0.f;

	CTileMap* TileMap = m_Scene->GetTileMap();

	Vector2	LB = Vector2(m_Pos.x - m_Size.x / 2.f, m_Pos.y - m_Size.y /2.f);

	Vector2	RB = LB + Vector2(m_Size.x, 0.f);

	int LBIndexX;
	int LBIndexY;
	int RBIndexX;
	int RBIndexY;

	LBIndexX = TileMap->GetOriginTileIndexX(LB.x);
	LBIndexY = TileMap->GetOriginTileIndexY(LB.y);
	RBIndexX = TileMap->GetOriginTileIndexX(RB.x);
	RBIndexY = TileMap->GetOriginTileIndexY(RB.y);


	LBIndexX = LBIndexX - 1 < 0 ? 0 : LBIndexX - 1;
	LBIndexY = LBIndexY > TileMap->GetTileCountY() - 1 ? TileMap->GetTileCountY() - 1 : LBIndexY;

	RBIndexX = RBIndexX + 1 > TileMap->GetTileCountX() - 1 ? TileMap->GetTileCountX() - 1 : RBIndexX;
	RBIndexY = RBIndexY > TileMap->GetTileCountY() - 1 ? TileMap->GetTileCountY() - 1 : RBIndexY;

	if (IsLeftMove)
	{
		if (TileMap->GetTile(LBIndexX - 1, LBIndexY)->GetTileOption() == ETileOption::Wall)
			return true;
	}

	else
	{
		if (TileMap->GetTile(RBIndexX + 1, RBIndexY)->GetTileOption() == ETileOption::Wall)
			return true;
	}

	return false;
}

