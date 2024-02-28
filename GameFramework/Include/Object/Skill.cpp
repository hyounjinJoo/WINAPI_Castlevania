#include "Skill.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderBox.h"
#include "../Scene/SceneResource.h"

CSkill::CSkill() :
	m_Dir(Vector2(0.f, 0.f)),
	m_Damage(0.f),
	m_Attack(false)
{
	m_ObjType = EObject_Type::Bullet;
}

CSkill::CSkill(const CSkill& obj) :
	CGameObject(obj)
{
	m_Dir = obj.m_Dir;
	m_Damage = obj.m_Damage;
	m_Attack = false;
}

CSkill::~CSkill()
{
}

void CSkill::Start()
{
	CGameObject::Start();

	CColliderBox* Body = AddCollider<CColliderBox>("Body");

	Body->SetCollisionProfile("PlayerAttack");
	Body->SetCollisionBeginFunction<CSkill>(this, &CSkill::CollisionBegin);

	AddAnimationNotify<CSkill>("LeftSkill", 0, this, &CSkill::PlayVoice);
	AddAnimationNotify<CSkill>("RightSkill", 0, this, &CSkill::PlayVoice);

	AddAnimationNotify<CSkill>("LeftSkill", 2, this, &CSkill::AttackStart);
	AddAnimationNotify<CSkill>("RightSkill", 2, this, &CSkill::AttackStart);

	SetAnimationEndNotify<CSkill>("LeftSkill", this, &CSkill::AttackEnd);
	SetAnimationEndNotify<CSkill>("RightSkill", this, &CSkill::AttackEnd);
}

bool CSkill::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);

	CreateAnimation();
	AddAnimation("LeftSkill", false, 0.6f);
	AddAnimation("RightSkill", false, 0.6f);
	

	return true;
}

void CSkill::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CSkill::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	CColliderBox* Body = (CColliderBox*)FindCollider("Body");

	if (m_Dir.x > 0.f)
	{
		ChangeAnimation("RightSkill");
		Body->SetExtent(178.f, 436.f);
		Body->SetOffset(89.f, 0.f);
	}
	else if (m_Dir.x < 0.f)
	{
		ChangeAnimation("LeftSkill");
		Body->SetExtent(178.f, 436.f);
		Body->SetOffset(-89.f, 0.f);
	}
}

void CSkill::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CSkill::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CSkill* CSkill::Clone()
{
	return new CSkill(*this);
}

void CSkill::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (!m_Attack)
		return;
	Dest->GetOwner()->SetDamage(m_Damage);
}

void CSkill::AttackStart()
{
	m_Attack = true;
	CCollider* Body = FindCollider("Body");
	if (Body)
		Body->ClearCollisionList();
}

void CSkill::AttackEnd()
{
	Destroy();
}

void CSkill::PlayVoice()
{
	m_Scene->GetSceneResource()->SoundPlay("Skill");
	m_Scene->GetSceneResource()->SoundPlay("Skill2");
}
