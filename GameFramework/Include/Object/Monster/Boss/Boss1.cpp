#include "Boss1.h"
#include "../../../Collision/ColliderBox.h"
#include "../../../Scene/Scene.h"
#include "../../../Scene/SceneResource.h"
#include "../../DamageFont.h"
#include "../../Player/Player.h"
#include "../../Bullet.h"
#include "../../../Scene/StartScene.h"
#include "../../../Scene/SceneManager.h"
#include "../../EffectHit.h"

CBoss1::CBoss1() :
	m_DeathTime(1.f),
	m_NoDamageTime(0.f),
	m_FireCoolTime(0.f),
	m_SwingCoolTime(0.f),
	m_PhaseFireTime(3.f),
	m_PhaseSwingTime(4.f),
	m_AttackEnable(false),
	m_IsPhaseSecond(false),
	m_IsNeedtoSwing(false)
{
	m_MonsterType = EMonsterType::Boss;
	m_PhysicsSimulate = true;
}

CBoss1::CBoss1(const CBoss1& obj):
	CMonster(obj)
{
	m_AttackEnable = false;
	m_DeathTime = 1.f;
	m_NoDamageTime = 0.f;
	m_FireCoolTime = obj.m_FireCoolTime;
	m_SwingCoolTime = obj.m_SwingCoolTime;
	m_IsPhaseSecond = obj.m_IsPhaseSecond;
	m_IsNeedtoSwing = obj.m_IsNeedtoSwing;
	m_PhaseFireTime = obj.m_PhaseFireTime;
	m_PhaseSwingTime = obj.m_PhaseSwingTime;
}

CBoss1::~CBoss1()
{
}

void CBoss1::Start()
{
	CMonster::Start();

	m_CharacterInfo.HP = 600;
	m_CharacterInfo.HPMax = 600;
	m_CharacterInfo.MP = 600;
	m_CharacterInfo.MPMax = 600;
	m_CharacterInfo.Exp = 100;
	m_CharacterInfo.Attack = 30;
	m_CharacterInfo.Armor = 6;

	AnmationNotifySetting();

	BodyCollisionSetting();

	m_Size = Vector2(745.f, 560.f);
	float resultY = GetFloorTileY(m_Pos.y);
	resultY != 0.f ? m_Pos.y = resultY : m_Pos.y = m_Pos.y;
	m_FallStartY = m_Pos.y;
	m_PrevPos.y = m_Pos.y;
}

bool CBoss1::Init()
{
	if (!CMonster::Init())
		return false;

	SetPivot(0.5f, 1.f);

	CreateAnimation();
	AddAnimation("BossCrawling", true, 2.f);
	AddAnimation("FirePos", false, 2.5f);
	AddAnimation("Swing", false, 3.f);

	m_CharacterInfo.HP = 600;
	m_CharacterInfo.HPMax = 600;
	m_CharacterInfo.MP = 600;
	m_CharacterInfo.MPMax = 600;
	m_CharacterInfo.Exp = 100;
	m_CharacterInfo.Attack = 30;
	m_CharacterInfo.Armor = 6;
	m_Size = Vector2(745.f, 560.f);
	m_Pos = Vector2(1450.f, 600.f);
	m_PrevPos = Vector2(1450.f, 600.f);
	m_MoveSpeed = 0.f;

	SetSideWallCheck(true);
	return true;
}

void CBoss1::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	CPlayer* Player = (CPlayer*)m_Scene->GetPlayer();

	float DistPlayer = abs(m_Pos.x - Player->GetPos().x);

	if (DistPlayer > m_Size.x / 2.f)
	{
		m_IsNeedtoSwing = false;
	}
	else
	{
		m_IsNeedtoSwing = true;
	}

	if (m_FireCoolTime >= 0.f)
	{
		m_FireCoolTime -= DeltaTime;
		if (m_FireCoolTime <= 0.f)
			m_FireCoolTime = 0.f;
	}

	if (m_SwingCoolTime >= 0.f)
	{
		m_SwingCoolTime -= DeltaTime;
		if (m_SwingCoolTime <= 0.f)
			m_SwingCoolTime = 0.f;
	}

	if(m_IsNeedtoSwing && m_SwingCoolTime == 0.f && CheckCurrentAnimationEnd())
	{

		ChangeAnimation("Swing");
		m_SwingCoolTime = m_PhaseSwingTime;
	}
	else if (!m_IsNeedtoSwing && m_FireCoolTime == 0.f && CheckCurrentAnimationEnd())
	{
		ChangeAnimation("FirePos");
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

void CBoss1::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);

	if (m_CharacterInfo.HP <= 0.f)
	{
		Death();
	}

	if (m_CharacterInfo.HP <= (m_CharacterInfo.HPMax / 2.f))
	{
		if (!m_IsPhaseSecond)
		{
			m_IsPhaseSecond = true;
			m_CharacterInfo.Attack *= 2;
			m_CharacterInfo.Armor -= 3;
		}
			
	}

	if (m_IsPhaseSecond)
	{
		SetAnimationPlayTime("FirePos", 0.5f);
		SetAnimationPlayTime("Swing", 0.5f);

		m_PhaseFireTime = 1.f;
		m_PhaseSwingTime = 1.f;
	}
}

void CBoss1::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CBoss1::Render(HDC hDC)
{
	CMonster::Render(hDC);
}

CBoss1* CBoss1::Clone()
{
	return new CBoss1(*this);
}

float CBoss1::SetDamage(float Damage)
{
	Damage = CMonster::SetDamage(Damage);

	if (Damage <= 0.f)
	{
		return 0.f;
	}

	CEffectHit* Hit = m_Scene->CreateEffectObject<CEffectHit>("HitEffect", "HitEffect",
		GetMiddle() + Vector2(0.f, m_Size.y / 2.f));

	CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont",
		GetMiddleTop()+Vector2(0.f, m_Size.y / 2.f));

	DamageFont->SetDamageNumber((int)Damage);

	return Damage;
}

void CBoss1::Death()
{
	Destroy();
}

void CBoss1::AnmationNotifySetting()
{
	SetAnimationEndNotify<CBoss1>("Swing", this, &CBoss1::ChangeCrawling);
	SetAnimationEndNotify<CBoss1>("FirePos", this, &CBoss1::ChangeCrawling);

	AddAnimationNotify<CBoss1>("Swing", 58, this, &CBoss1::SetAttackEnable);
	AddAnimationNotify<CBoss1>("Swing", 61, this, &CBoss1::SetAttackDisable);
	AddAnimationNotify<CBoss1>("FirePos", 23, this, &CBoss1::Fire);

}

void CBoss1::BodyCollisionSetting()
{
	CColliderBox* Attack = AddCollider<CColliderBox>("BossAttack");
	Attack->SetExtent(372.f, 91.f);
	Attack->SetOffset(-186.f, 0.f);
	Attack->SetCollisionBeginFunction<CBoss1>(this, &CBoss1::AttackCollisionBegin);
	Attack->SetCollisionProfile("MonsterAttack");

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(147.f, 163.f);
	Body->SetOffset(0.f, -225.f);
	Body->SetCollisionBeginFunction<CBoss1>(this, &CBoss1::BodyCollisionBegin);
	Body->SetCollisionWhileFunction<CBoss1>(this, &CBoss1::BodyCollisionWhile);
	Body->SetCollisionProfile("Monster");
}

void CBoss1::ChangeCrawling()
{
	ChangeAnimation("BossCrawling");
}

void CBoss1::SetAttackEnable()
{
	m_Scene->GetSceneResource()->SoundPlay("Swing");

	CCollider* Body = FindCollider("BossAttack");
	if (Body)
		Body->ClearCollisionList();
	m_AttackEnable = true;
}

void CBoss1::SetAttackDisable()
{
	m_AttackEnable = false;
}

void CBoss1::Fire()
{
	CSharedPtr<CBullet> Bullet = m_Scene->CreateEffectObject<CBullet>("Bullet",
		"BossFire",
		Vector2(m_Pos.x, m_Pos.y - 125.f),
		Vector2(160.f, 116.f));
	Bullet->SetBulletDamage(static_cast<float>(m_CharacterInfo.Attack));
	CGameObject* Player = m_Scene->GetPlayer();

	float	Angle = GetAngle(Bullet->GetPos(), Player->GetPos());
	Bullet->SetDir(Angle);

	m_Scene->GetSceneResource()->SoundPlay("Fire");
	m_FireCoolTime = m_PhaseFireTime;
}

void CBoss1::AIIdle(float DeltaTime)
{
}

void CBoss1::AIDetect(float DeltaTime)
{
}

void CBoss1::AITrace(float DeltaTime)
{
}

void CBoss1::AIAttack(float DeltaTime)
{
}

void CBoss1::AIDeath(float DeltaTime)
{
}

void CBoss1::BodyCollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (Dest->GetOwner()->GetName() != "Player")
		return;

	if (Dest->GetProfile()->Name != "Player")
		return;

	if (m_NoDamageTime <= 0.f)
	{
		Dest->GetOwner()->SetDamage((float)m_CharacterInfo.Armor * 5);
		m_NoDamageTime = 1.f;
	}
}

void CBoss1::BodyCollisionWhile(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (Dest->GetOwner()->GetName() != "Player")
		return;

	if (Dest->GetProfile()->Name != "Player")
		return;

	if (m_NoDamageTime <= 0.f)
	{
		Dest->GetOwner()->SetDamage((float)m_CharacterInfo.Armor * 5);
		m_NoDamageTime = 1.f;
	}
}

void CBoss1::AttackCollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (Dest->GetOwner()->GetName() != "Player")
		return;

	if (Dest->GetProfile()->Name != "Player")
		return;

	if (!m_AttackEnable)
		return;

	if (m_NoDamageTime <= 0.f)
	{
		Dest->GetOwner()->SetDamage((float)m_CharacterInfo.Attack * 1.5f);
		m_NoDamageTime = 1.f;
	}
}
