#pragma once
#include "../../Monster.h"

class CZombie :
    public CMonster
{
	friend class CScene;

protected:
	CZombie();
	CZombie(const CZombie& obj);
	virtual ~CZombie();

private:
	bool	m_AttackEnable;
	float	m_DeathTime;
	Vector2	m_CreatePos;
	bool	m_IsZombieAType;
	float	m_ReCreateTime;
	float	m_NoDamageTime;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CZombie* Clone();
	virtual float SetDamage(float Damage);
	virtual void WalkBegin();
	virtual void Death();
	virtual void Walk();

private:
	void	ReCreate();
	void	Dig();
	void	BloodEffect();
	void	BloodTrail();
	void	BloodDrop();
	void	AnmationNotifySetting();
	void	BodyCollisionSetting();
	void	DirectionSetting();

public:
	virtual void AIIdle(float DeltaTime);
	virtual void AIDetect(float DeltaTime);
	virtual void AITrace(float DeltaTime);
	virtual void AIAttack(float DeltaTime);
	virtual void AIDeath(float DeltaTime);
	virtual void BodyCollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime);
	virtual void BodyCollisionWhile(class CCollider* Src, class CCollider* Dest, float DeltaTime);

private:
	bool	IsTileWallExist(bool IsLeftMove);
};