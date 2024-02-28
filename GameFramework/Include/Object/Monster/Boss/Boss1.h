#pragma once
#include "../Monster.h"

class CBoss1 :
    public CMonster
{
	friend class CScene;

protected:
	CBoss1();
	CBoss1(const CBoss1& obj);
	virtual ~CBoss1();

private:
	bool	m_AttackEnable;
	float	m_DeathTime;
	float	m_NoDamageTime;
	
	float	m_FireCoolTime;
	float	m_SwingCoolTime;
	bool	m_IsNeedtoSwing;
	bool	m_IsPhaseSecond;
	float	m_PhaseSwingTime;
	float   m_PhaseFireTime;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CBoss1* Clone();
	virtual float SetDamage(float Damage);
	virtual void Death();

private:
	void	AnmationNotifySetting();
	void	BodyCollisionSetting();
	void	ChangeCrawling();
	void	SetAttackEnable();
	void	SetAttackDisable();
	void    Fire();

public:
	virtual void AIIdle(float DeltaTime);
	virtual void AIDetect(float DeltaTime);
	virtual void AITrace(float DeltaTime);
	virtual void AIAttack(float DeltaTime);
	virtual void AIDeath(float DeltaTime);

	virtual void BodyCollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime);
	virtual void BodyCollisionWhile(class CCollider* Src, class CCollider* Dest, float DeltaTime);
	virtual void AttackCollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime);
};

