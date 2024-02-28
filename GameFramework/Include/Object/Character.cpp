
#include "Character.h"

CCharacter::CCharacter()	:
	m_CharacterInfo{},
	m_NoVisibleTime(0.f),
	m_NoVisibleAdd(0.f),
	m_IsVisible(true)
{
	m_ObjType = EObject_Type::Character;
}

CCharacter::CCharacter(const CCharacter& obj) :
	CGameObject(obj)
{
	m_CharacterInfo = obj.m_CharacterInfo;
	m_NoVisibleTime = 0.f;
	m_NoVisibleAdd = 0.f;
	m_IsVisible = true;
}

CCharacter::~CCharacter()
{
}

void CCharacter::Start()
{
	CGameObject::Start();
}

bool CCharacter::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CCharacter::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);


	m_NoVisibleAdd += DeltaTime;
	if (m_NoVisibleAdd > 0.05f)
	{
		m_NoVisibleAdd = 0.05f;

		m_NoVisibleTime -= m_NoVisibleAdd;

		m_IsVisible = !m_IsVisible;

		m_NoVisibleAdd = 0.f;

	}

	if (m_NoVisibleTime <= 0.f)
	{
		m_NoVisibleTime = 0.f;
		m_IsVisible = true;
	}
}

void CCharacter::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CCharacter::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CCharacter::Render(HDC hDC)
{
	if (m_IsVisible)
		CGameObject::Render(hDC);
}

CCharacter* CCharacter::Clone()
{
	return nullptr;
}

float CCharacter::SetDamage(float Damage)
{
	Damage = CGameObject::SetDamage(Damage);

	Damage -= m_CharacterInfo.Armor;

	if (Damage > 0.f && m_NoVisibleTime == 0.f)
	{
		m_NoVisibleTime = 0.6f;

		m_CharacterInfo.HP -= (int)Damage;
	}
	
	return Damage;
}
