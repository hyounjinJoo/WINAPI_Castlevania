
#include "Monster.h"
#include "../../Scene/Scene.h"
#include "../../Collision/ColliderBox.h"

CMonster::CMonster()	:
	m_AI(EMonsterAI::Idle),
	m_DetectDistance(500.f),
	m_AttackDistance(200.f),
	m_AttackEnable(false),
	m_Dir(0.f, 0.f),
	m_BodyAttackTime(0.f)
{
	m_MonsterType = EMonsterType::NormalGround;
}

CMonster::CMonster(const CMonster& obj) :
	CCharacter(obj)
{
	m_Dir = obj.m_Dir;
	m_MonsterType = obj.m_MonsterType;
}

CMonster::~CMonster()
{
}

void CMonster::Start()
{
	CCharacter::Start();
}

bool CMonster::Init()
{
	if (!CCharacter::Init())
		return false;

	return true;
}

void CMonster::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	CGameObject* Player = m_Scene->GetPlayer();

	// 주변에 플레이어가 있는지 판단한다.
	// 플레이어와의 거리를 구한다.
	
	float	Dist = 0.f;
	if(Player)
		Dist = Distance(Player->GetPos(), m_Pos);

	if (Dist <= m_DetectDistance)
	{
		if (Dist <= m_AttackDistance)
		{
			m_AttackEnable = true;
		}

		else
		{
			if (!m_AttackEnable)
				m_AI = EMonsterAI::Trace;
		}
	}

	else
	{
		m_AI = EMonsterAI::Idle;
	}

	if (m_CharacterInfo.HP <= 0)
	{
		m_AI = EMonsterAI::Death;
	}
}

void CMonster::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);
}

void CMonster::Collision(float DeltaTime)
{
	CCharacter::Collision(DeltaTime);
}

void CMonster::Render(HDC hDC)
{
	CCharacter::Render(hDC);
}

float CMonster::SetDamage(float Damage)
{
	Damage = CCharacter::SetDamage(Damage);

	return Damage;
}

void CMonster::AttackEnd()
{
	m_AttackEnable = true;
}
