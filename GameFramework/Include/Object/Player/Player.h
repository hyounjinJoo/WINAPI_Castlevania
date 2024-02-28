#pragma once
#include "../Character.h"
class CPlayer :
    public CCharacter
{
	friend class CScene;
	friend class CDoor;

protected:
	CPlayer();
	CPlayer(const CPlayer& obj);
	virtual ~CPlayer();

protected:
	bool	m_isWalking;
	float	m_NoDamageTime;
	bool	m_isLeft;
	float	m_SkillCoolTime;
	float	m_RecoveryTime;
	bool	m_isFirstCreate;
	bool	m_RightAttack;
	bool	m_LeftAttack;
	EPlayerAnimState m_AnimState;

public:
	virtual void SetAttackSpeed(float Speed)
	{
		CCharacter::SetAttackSpeed(Speed);

		SetAnimationPlayScale("SomaRightAttack", Speed);
		SetAnimationPlayScale("SomaLeftAttack", Speed);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CPlayer* Clone();
	virtual float SetDamage(float Damage);

private:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
public:
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
private:
	void BulletFire(float DeltaTime);
	void Pause(float DeltaTime);
	void Resume(float DeltaTime);
	void JumpKey(float DeltaTime);
	void SuperJumpKey(float DeltaTime);
	void CheckJump();
	bool IsBottomWall();
	bool IsBottomWallNotSideColl();
	void CheckJumpEnd();
	bool CheckBeforeGroundEnd();
	void JumpSound();

	void LeftCollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime);
	void RightCollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime);

	void CheckLevelUp();

	void ChangeRenderTileMap(float DeltaTime);
	void ChangeRenderCollider(float DeltaTime);
	void ChangeRenderText(float DeltaTime);
	void DisableAttack();

public:
	void AttackEnd();
	void SoulAttack();
	void Fire();
	void KeepMove();
	void SetFirstCreate(bool isFirst)
	{
		m_isFirstCreate = isFirst;
	}

private:
	void CheckExp()
	{
		if (m_Exp != 0)
		{
			m_CharacterInfo.Exp += m_Exp;
			m_Exp = 0;
			CheckLevelUp();
		}
	}

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

