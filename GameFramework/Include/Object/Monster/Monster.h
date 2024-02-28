#pragma once
#include "../Character.h"

class CMonster :
    public CCharacter
{
	friend class CScene;

protected:
	CMonster();
	CMonster(const CMonster& obj);
	virtual ~CMonster();

protected:
	Vector2	m_Dir;
	float	m_DetectDistance;
	float	m_AttackDistance;
	bool	m_AttackEnable;
	float	m_BodyAttackTime;
	EMonsterType m_MonsterType;
	EMonsterAI	m_AI;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual float SetDamage(float Damage);
	
public:
	void SetAttackDistance(float Distance)
	{
		m_AttackDistance = Distance;
	}
	void SetDetectDistance(float Distance)
	{
		m_DetectDistance = Distance;
	}

	float GetAttackDistance()
	{
		return m_AttackDistance;
	}
	float GetDetectDistance()
	{
		return m_DetectDistance;
	}

public:
	virtual void AIIdle(float DeltaTime) = 0;
	virtual void AIDetect(float DeltaTime) = 0;
	virtual void AITrace(float DeltaTime) = 0;
	virtual void AIAttack(float DeltaTime) = 0;
	virtual void AIDeath(float DeltaTime) = 0;
	virtual void BodyCollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime) = 0;

public:
	void AttackEnd();
};

