
#include "Bullet.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "EffectHit.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../UI/WidgetComponent.h"
#include "../UI/NumberWidget.h"
#include "DamageFont.h"

CBullet::CBullet()
{
	m_ObjType = EObject_Type::Bullet;
	m_Dir.x = 1.f;
	m_Dir.y = 0.f;

	m_Damage = 6.f;

	m_Distance = 900.f;

	SetMoveSpeed(450.f);
}

CBullet::CBullet(const CBullet& obj)	:
	CGameObject(obj)
{
	m_Damage = obj.m_Damage;
	m_Dir = obj.m_Dir;
	m_Distance = obj.m_Distance;
}

CBullet::~CBullet()
{
}

void CBullet::Start()
{
	CGameObject::Start();

	CCollider* Body = FindCollider("Body");

	Body->SetCollisionBeginFunction<CBullet>(this, &CBullet::CollisionBegin);

	SetAnimationEndNotify<CBullet>("FireBallStart", this, &CBullet::ChangeMiddle);
}

bool CBullet::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);

	CreateAnimation();
	AddAnimation("FireBallStart", false, 0.5f);
	AddAnimation("FireBallMiddle", true, 0.8f);


	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(25.f);
	Body->SetOffset(0.f, 0.f);

	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector2	Dir = m_Dir;
	Dir.Normalize();

	Move(Dir);

	m_Distance -= GetMoveSpeedFrame();

	if (m_Distance <= 0.f)
		Destroy();
}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBullet::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CBullet::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::ChangeMiddle()
{
	ChangeAnimation("FireBallMiddle");
}

void CBullet::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	Destroy();

	Dest->GetOwner()->SetDamage(m_Damage);

	CEffectHit* Hit = m_Scene->CreateEffectObject<CEffectHit>("HitEffect", "HitEffect",
		Dest->GetOwner()->GetMiddleTop());
	
	m_Scene->GetSceneResource()->SoundPlay("Fire");
}
