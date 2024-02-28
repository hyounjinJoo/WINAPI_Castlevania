#include "BloodTrail.h"

CBloodTrail::CBloodTrail()
{
	m_ObjType = EObject_Type::Effect;
	m_LifeTime = 3.f;
}

CBloodTrail::CBloodTrail(const CBloodTrail& obj) :
	CGameObject(obj)
{
	m_ObjType = obj.m_ObjType;
	m_LifeTime = 3.f;
}

CBloodTrail::~CBloodTrail()
{
}

void CBloodTrail::Start()
{
}

bool CBloodTrail::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);

	CreateAnimation();
	AddAnimation("ZombieBloodTrail", false);

	m_PrevPos = m_Pos;
	m_Size = Vector2(124.f, 4.f);

	return true;
}

void CBloodTrail::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBloodTrail::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBloodTrail::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CBloodTrail::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CBloodTrail* CBloodTrail::Clone()
{
	return new CBloodTrail(*this);
}