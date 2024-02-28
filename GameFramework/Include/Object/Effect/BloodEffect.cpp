#include "BloodEffect.h"

CBloodEffect::CBloodEffect()
{
	m_ObjType = EObject_Type::Effect;
	m_LifeTime = 0.15f;
	m_Dir.x = 0.f;
	m_Dir.y = 1.f;
	SetMoveSpeed(500.f);
}

CBloodEffect::CBloodEffect(const CBloodEffect& obj)	:
	CGameObject(obj)
{
	m_ObjType = obj.m_ObjType;
	m_LifeTime = 0.15f;
	m_Dir = obj.m_Dir;
}

CBloodEffect::~CBloodEffect()
{
}

void CBloodEffect::Start()
{
	CGameObject::Start();
}

bool CBloodEffect::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);

	CreateAnimation();
	AddAnimation("ZombieDropBlood", true);

	m_Size = Vector2(8.f, 16.f);

	return true;
}

void CBloodEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector2	Dir = m_Dir;
	Dir.Normalize();

	Move(Dir);
}

void CBloodEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBloodEffect::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CBloodEffect::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CBloodEffect* CBloodEffect::Clone()
{
	return new CBloodEffect(*this);
}