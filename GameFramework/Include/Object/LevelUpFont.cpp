#include "LevelUpFont.h"

#include "../Scene/Scene.h"

CLevelUpFont::CLevelUpFont()
{
	m_ObjType = EObject_Type::Effect;
}

CLevelUpFont::CLevelUpFont(const CLevelUpFont& obj) :
	CGameObject(obj)
{
}

CLevelUpFont::~CLevelUpFont()
{
}

void CLevelUpFont::Start()
{
	CGameObject::Start();

	m_SpeedX = (float)(rand() % 201);
}

bool CLevelUpFont::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);

	CreateAnimation();
	AddAnimation("LevelUp", true, 0.1f);

	SetGravityAccel(10.f);
	SetPhysicsSimulate(true);
	SetJumpVelocity(50.f);
	Jump();

	SetSize(128.f, 56.f);

	SetLifeTime(0.4f);

	return true;
}

void CLevelUpFont::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_LifeTime == 0.f)
		Destroy();
}

void CLevelUpFont::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CLevelUpFont::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CLevelUpFont::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CLevelUpFont* CLevelUpFont::Clone()
{
	return new CLevelUpFont(*this);
}
