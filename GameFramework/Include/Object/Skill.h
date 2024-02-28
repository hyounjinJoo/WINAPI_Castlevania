#pragma once
#include "GameObject.h"
class CSkill :
    public CGameObject
{
	friend class CScene;

protected:
	CSkill();
	CSkill(const CSkill& obj);
	virtual ~CSkill();

protected:
	Vector2		m_Dir;
	float		m_Damage;
	bool		m_Attack;

public:
	void SetSoulDamage(float Damage)
	{
		m_Damage = Damage;
	}

	void SetDir(float x, float y)
	{
		m_Dir.x = x;
		m_Dir.y = y;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CSkill* Clone();

public:
	void CollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime);

private:
	void AttackStart();
	void AttackEnd();
	void PlayVoice();
};

