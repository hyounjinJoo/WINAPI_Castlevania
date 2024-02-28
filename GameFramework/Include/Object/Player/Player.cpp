
#include "Player.h"
#include "../../Scene/Scene.h"
#include "../../Input.h"
#include "../../GameManager.h"
#include "../../Collision/ColliderBox.h"
#include "../../Collision/ColliderSphere.h"
#include "../../UI/UICharacterStateHUD.h"
#include "../../UI/ProgressBar.h"
#include "../../UI/UIText.h"
#include "../../Scene/SceneResource.h"
#include "../EffectHit.h"
#include "../LevelUpFont.h"
#include "../DamageFont.h"
#include "../Skill.h"

CPlayer::CPlayer()	:
	m_isLeft(false),
	m_isWalking(false),
	m_NoDamageTime(0.f),
	m_isFirstCreate(false),
	m_AnimState(EPlayerAnimState::Idle),
	m_RightAttack(false),
	m_LeftAttack(false),
	m_SkillCoolTime(0.f),
	m_RecoveryTime(1.f)
{
}

CPlayer::CPlayer(const CPlayer& obj)	:
	CCharacter(obj)
{
	m_isLeft = obj.m_isLeft;
	m_NoDamageTime = 0.f;
	m_AnimState = obj.m_AnimState;
	m_RightAttack = false;
	m_LeftAttack = false;
	m_SkillCoolTime = obj.m_SkillCoolTime;
	m_RecoveryTime = obj.m_RecoveryTime;
	m_CharacterInfo = obj.m_CharacterInfo;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Start()
{
	CCharacter::Start();

	CInput::GetInst()->SetCallback<CPlayer>("MoveUp", KeyState_Push,
		this, &CPlayer::MoveUp);

	CInput::GetInst()->SetCallback<CPlayer>("MoveDown", KeyState_Push,
		this, &CPlayer::MoveDown);

	CInput::GetInst()->SetCallback<CPlayer>("MoveLeft", KeyState_Push,
		this, &CPlayer::MoveLeft);

	CInput::GetInst()->SetCallback<CPlayer>("MoveRight", KeyState_Push,
		this, &CPlayer::MoveRight);

	CInput::GetInst()->SetCallback<CPlayer>("Fire", KeyState_Down,
		this, &CPlayer::BulletFire);

	CInput::GetInst()->SetCallback<CPlayer>("Pause", KeyState_Down,
		this, &CPlayer::Pause);

	CInput::GetInst()->SetCallback<CPlayer>("Resume", KeyState_Down,
		this, &CPlayer::Resume);

	CInput::GetInst()->SetCallback<CPlayer>("Jump", KeyState_Down,
		this, &CPlayer::JumpKey);

	CInput::GetInst()->SetCallback<CPlayer>("SuperJump", KeyState_Down,
		this, &CPlayer::SuperJumpKey);

	CInput::GetInst()->SetCallback<CPlayer>("RenderTileMap", KeyState_Down,
		this, &CPlayer::ChangeRenderTileMap);

	CInput::GetInst()->SetCallback<CPlayer>("RenderCollider", KeyState_Down,
		this, &CPlayer::ChangeRenderCollider);

	CInput::GetInst()->SetCallback<CPlayer>("RenderText", KeyState_Down,
		this, &CPlayer::ChangeRenderText);
}

bool CPlayer::Init()
{
	if (!CCharacter::Init())
		return false;

	SetPivot(0.5f, 1.f);

	CreateAnimation();
	AddAnimation("SomaRightIdle");
	AddAnimation("SomaRightWalkStart", false, 0.2f);
	AddAnimation("SomaRightWalking", true, 1.0f);
	AddAnimation("SomaRightAttack", false, 0.9f);
	AddAnimation("SomaRightDown", false, 0.4f);
	AddAnimation("SomaRightUp", false, 0.4f);
	AddAnimation("SomaRightSkillPos", false, 0.2f);

	SetAnimationEndNotify<CPlayer>("SomaRightWalkStart", this, &CPlayer::KeepMove);

	AddAnimationNotify<CPlayer>("SomaRightAttack", 2, this, &CPlayer::Fire);
	SetAnimationEndNotify<CPlayer>("SomaRightAttack", this, &CPlayer::AttackEnd);
	SetAnimationEndNotify<CPlayer>("SomaRightSkillPos", this, &CPlayer::SoulAttack);

	AddAnimation("SomaLeftIdle");
	AddAnimation("SomaLeftWalkStart", false, 0.2f);
	AddAnimation("SomaLeftWalking", true, 1.0f);
	AddAnimation("SomaLeftAttack", false, 0.9f);
	AddAnimation("SomaLeftDown", false, 0.4f);
	AddAnimation("SomaLeftUp", false, 0.4f);
	AddAnimation("SomaLeftSkillPos", false, 0.2f);

	SetAnimationEndNotify<CPlayer>("SomaLeftWalkStart", this, &CPlayer::KeepMove);

	AddAnimationNotify<CPlayer>("SomaLeftAttack", 2, this, &CPlayer::Fire);
	SetAnimationEndNotify<CPlayer>("SomaLeftAttack", this, &CPlayer::AttackEnd);
	SetAnimationEndNotify<CPlayer>("SomaLeftSkillPos", this, &CPlayer::SoulAttack);

	// Jump Part==============================================================================================
	AddAnimation("SomaRightJumpStart", true, 0.3f);
	AddAnimation("SomaRightJumpMax", false, 0.2f);
	AddAnimation("SomaRightJumpFalling", false, 0.5f);
	AddAnimation("SomaRightBeforeGround", false, 0.3f);
	AddAnimation("SomaRightDoubleJump", false, 0.05f);
	AddAnimation("SomaRightSuperJump", false, 2.0f);
	AddAnimation("SomaRightSuperJumpCeiling", false, 0.3f);

	AddAnimation("SomaLeftJumpStart", true, 0.3f);
	AddAnimation("SomaLeftJumpMax", false, 0.2f);
	AddAnimation("SomaLeftJumpFalling", false, 0.5f);
	AddAnimation("SomaLeftBeforeGround", false, 0.3f);
	AddAnimation("SomaLeftDoubleJump", false, 0.05f);
	AddAnimation("SomaLeftSuperJump", false, 2.0f);
	AddAnimation("SomaLeftSuperJumpCeiling", false, 0.3f);


	AddAnimationNotify<CPlayer>("SomaRightJumpStart", 1, this, &CPlayer::JumpSound);
	AddAnimationNotify<CPlayer>("SomaRightBeforeGround", 1, this, &CPlayer::JumpSound);
	AddAnimationNotify<CPlayer>("SomaRightDoubleJump", 2, this, &CPlayer::JumpSound);
	AddAnimationNotify<CPlayer>("SomaRightSuperJump", 0, this, &CPlayer::JumpSound);

	AddAnimationNotify<CPlayer>("SomaLeftJumpStart", 1, this, &CPlayer::JumpSound);
	AddAnimationNotify<CPlayer>("SomaLeftBeforeGround", 1, this, &CPlayer::JumpSound);
	AddAnimationNotify<CPlayer>("SomaLeftDoubleJump", 2, this, &CPlayer::JumpSound);
	AddAnimationNotify<CPlayer>("SomaLeftSuperJump", 0, this, &CPlayer::JumpSound);

	SetAnimationEndNotify<CPlayer>("SomaLeftBeforeGround", this, &CPlayer::CheckJumpEnd); 
	SetAnimationEndNotify<CPlayer>("SomaRightBeforeGround", this, &CPlayer::CheckJumpEnd);
	//========================================================================================================
	
	// Melee Attack===========================================================================================
	//01. Levatein
	//_Normal
	AddAnimation("SomaRightLevatein", false, 0.8f);
	AddAnimation("SomaLeftLevatein", false, 0.8f);

	AddAnimationNotify<CPlayer>("SomaRightLevatein", 2, this, &CPlayer::Fire);
	AddAnimationNotify<CPlayer>("SomaRightLevatein", 4, this, &CPlayer::DisableAttack);
	SetAnimationEndNotify<CPlayer>("SomaRightLevatein", this, &CPlayer::AttackEnd);
	AddAnimationNotify<CPlayer>("SomaLeftLevatein", 2, this, &CPlayer::Fire);
	AddAnimationNotify<CPlayer>("SomaLeftLevatein", 4, this, &CPlayer::DisableAttack);
	SetAnimationEndNotify<CPlayer>("SomaLeftLevatein", this, &CPlayer::AttackEnd);
	//_Jump===================================================================================================
	AddAnimation("SomaRightLevateinJump", false, 0.5f);
	AddAnimation("SomaLeftLevateinJump", false, 0.5f);

	AddAnimationNotify<CPlayer>("SomaRightLevateinJump", 2, this, &CPlayer::Fire);
	AddAnimationNotify<CPlayer>("SomaRightLevateinJump", 4, this, &CPlayer::DisableAttack);
	SetAnimationEndNotify<CPlayer>("SomaRightLevateinJump", this, &CPlayer::AttackEnd);
	AddAnimationNotify<CPlayer>("SomaLeftLevateinJump", 2, this, &CPlayer::Fire);
	AddAnimationNotify<CPlayer>("SomaLeftLevateinJump", 4, this, &CPlayer::DisableAttack);
	SetAnimationEndNotify<CPlayer>("SomaLeftLevateinJump", this, &CPlayer::AttackEnd);

	//_Down===================================================================================================
	AddAnimation("SomaRightLevateinDown", false, 1.0f);
	AddAnimation("SomaLeftLevateinDown", false, 1.0f);

	AddAnimationNotify<CPlayer>("SomaRightLevateinDown", 2, this, &CPlayer::Fire);
	AddAnimationNotify<CPlayer>("SomaRightLevateinDown", 4, this, &CPlayer::DisableAttack);
	SetAnimationEndNotify<CPlayer>("SomaRightLevateinDown", this, &CPlayer::AttackEnd);
	AddAnimationNotify<CPlayer>("SomaLeftLevateinDown", 2, this, &CPlayer::Fire);
	AddAnimationNotify<CPlayer>("SomaLeftLevateinDown", 4, this, &CPlayer::DisableAttack);
	SetAnimationEndNotify<CPlayer>("SomaLeftLevateinDown", this, &CPlayer::AttackEnd);
	//========================================================================================================


	CColliderBox* Body = AddCollider<CColliderBox>("PlayerBody");
	Body->SetExtent(50.f, 136.f);
	Body->SetOffset(0.f, -68.f);
	Body->SetCollisionProfile("Player");

	Body = AddCollider<CColliderBox>("RightMeleeAttack");
	Body->SetExtent(180.f, 60.f);
	Body->SetOffset(120, -m_Size.y + 20);
	Body->SetCollisionProfile("PlayerAttack");
	Body->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::RightCollisionBegin);

	Body = AddCollider<CColliderBox>("LeftMeleeAttack");
	Body->SetExtent(180.f, 60.f);
	Body->SetOffset(-120, -m_Size.y + 20);
	Body->SetCollisionProfile("PlayerAttack");
	Body->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::LeftCollisionBegin);

	m_MoveSpeed = 270.f;
	SetCharacterInfo(10, 6, 320, 80, 1, 0, 20, 0, 0.f, 0.f);

	//SetGravityAccel(30.f);
	SetPhysicsSimulate(true);
	SetJumpVelocity(60.f);
	SetSuperJumpVelocity(128.f);
	SetSideWallCheck(true);

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	if (m_NoDamageTime > 0.f)
	{
		m_NoDamageTime -= DeltaTime;
		if (m_NoDamageTime < 0.f)
			m_NoDamageTime = 0.f;
	}

	if (m_SkillCoolTime > 0.f)
	{
		m_SkillCoolTime -= DeltaTime;
		if (m_SkillCoolTime < 0.f)
			m_SkillCoolTime = 0.f;
	}

	if (m_RecoveryTime > 0.f)
	{
		m_RecoveryTime -= DeltaTime;
		if (m_RecoveryTime < 0.f)
		{
			m_RecoveryTime = 1.f;
			
			m_CharacterInfo.HP += 6;
			if (m_CharacterInfo.HP > m_CharacterInfo.HPMax)
				m_CharacterInfo.HP = m_CharacterInfo.HPMax;

			m_CharacterInfo.MP += 6;
			if (m_CharacterInfo.MP > m_CharacterInfo.MPMax)
				m_CharacterInfo.MP = m_CharacterInfo.MPMax;

			CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

			if (State)
				State->SetMPPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);
		}
	}

	float ResultX = m_PrevPos.x - m_Pos.x;
	if (ResultX > 0.f)
		m_isLeft = true;
	else if (ResultX < 0.f)
		m_isLeft = false;

	CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	if (State)
	{
		int StateHPNumber = State->GetHPNumber();

		if (StateHPNumber > 99)
			State->SetHPNumberPos(3);
		else if (StateHPNumber > 9)
			State->SetHPNumberPos(2);
		else
			State->SetHPNumberPos(1);

		if(StateHPNumber != m_CharacterInfo.HP)
			State->SetHPNumber(m_CharacterInfo.HP);
	}

	CheckExp();
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	switch (m_AnimState)
	{
	case EPlayerAnimState::JumpEnd:
	{
		if (CheckCurrentAnimationEnd())
			m_AnimState = Idle;
	}
	break;
	case EPlayerAnimState::Idle:
	case EPlayerAnimState::MoveStart:
	case EPlayerAnimState::Move:
	case EPlayerAnimState::Up:
	case EPlayerAnimState::Down:
	case EPlayerAnimState::AttackEnd:
	case EPlayerAnimState::DownAttackEnd:
	case EPlayerAnimState::SoulAttackEnd:
	// 애니메이션 종료 등으로 인한 Idle 상태로 전이
	{
		if (m_Velocity.Length() == 0.f
			&& !m_Jump && !m_DoubleJump && !m_SuperJump
			&& !CInput::GetInst()->GetUp()
			&& !CInput::GetInst()->GetDown())
		{
			{
				CColliderBox* Body = (CColliderBox*)FindCollider("PlayerBody");
				Body->SetExtent(50.f, 136.f);
				Body->SetOffset(0.f, -68.f);
				Body = (CColliderBox*)FindCollider("RightMeleeAttack");
				Body->SetOffset(120, -m_Size.y + 20);

				Body = (CColliderBox*)FindCollider("LeftMeleeAttack");
				Body->SetExtent(180.f, 60.f);
				Body->SetOffset(-120, -m_Size.y + 20);
			}

			if (m_isLeft)
			{
				m_isWalking = false;
				ChangeAnimation("SomaLeftIdle");
				m_AnimState = EPlayerAnimState::Idle;
			}

			else
			{
				m_isWalking = false;
				ChangeAnimation("SomaRightIdle");
				m_AnimState = EPlayerAnimState::Idle;
			}
		}
	}
		break;
	case EPlayerAnimState::Jump:
	case EPlayerAnimState::DoubleJump:
	case EPlayerAnimState::SuperJump:
	case EPlayerAnimState::DownJump:
	case EPlayerAnimState::JumpMax:
	case EPlayerAnimState::JumpStart:
	case EPlayerAnimState::Falling:
	case EPlayerAnimState::Ceilling:
	case EPlayerAnimState::JumpAttack:
	case EPlayerAnimState::JumpAttackEnd:
		CheckJump();
		break;
	default:
		break;
	}
}

void CPlayer::Collision(float DeltaTime)
{
	CCharacter::Collision(DeltaTime);
}

void CPlayer::Render(HDC hDC)
{
	CCharacter::Render(hDC);
}

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

float CPlayer::SetDamage(float Damage)
{
	if (m_NoDamageTime > 0.f)
		return 0.f;

	Damage = CCharacter::SetDamage(Damage);
	
	if (Damage <= 0.f)
	{
		return 1.f;
	}

	CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont",
		GetMiddleTop());

	DamageFont->SetDamageNumber((int)Damage);

	m_NoDamageTime = 2.f;

	CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	if (State)
		State->SetHPPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);

	return Damage;
}

void CPlayer::MoveUp(float DeltaTime)
{	
	if (!m_Jump && !m_DoubleJump && !m_SuperJump && CheckBeforeGroundEnd() && m_AnimState != EPlayerAnimState::SoulAttack)
	{
		m_isLeft ? ChangeAnimation("SomaLeftUp") : ChangeAnimation("SomaRightUp");
		m_AnimState = EPlayerAnimState::Up;
	}
}

void CPlayer::MoveDown(float DeltaTime)
{
	if (!m_Jump && !m_DoubleJump && !m_SuperJump && CheckBeforeGroundEnd()
		&& m_AnimState != EPlayerAnimState::DownAttack
		&& m_AnimState != EPlayerAnimState::Falling)
	{
		{
			CColliderBox* Body = (CColliderBox*)FindCollider("PlayerBody");
			Body->SetExtent(60.f, 80.f);
			Body->SetOffset(0.f, -40.f);
			Body = (CColliderBox*)FindCollider("RightMeleeAttack");
			Body->SetOffset(120, -m_Size.y + 60);
			Body = (CColliderBox*)FindCollider("LeftMeleeAttack");
			Body->SetOffset(-120, -m_Size.y + 60);
		}
		if ((m_AnimState == EPlayerAnimState::Down || m_AnimState == EPlayerAnimState::DownAttackEnd) && CheckCurrentAnimationEnd())
		{
			m_isLeft ? ChangeAnimationEnd("SomaLeftDown") : ChangeAnimationEnd("SomaRightDown");
		}
		else
		{
			m_isLeft ? ChangeAnimation("SomaLeftDown") : ChangeAnimation("SomaRightDown");
		}
		m_AnimState = EPlayerAnimState::Down;
	}
}

void CPlayer::MoveLeft(float DeltaTime)
{
	if (m_AnimState == EPlayerAnimState::Down)
	{
		m_isLeft = true;
		return;
	}

	if (m_AnimState == EPlayerAnimState::Attack
		|| m_AnimState == EPlayerAnimState::DownAttack)
		return;

	Move(Vector2(-1.f, 0.f));


	if (!m_Jump && !m_DoubleJump && !m_SuperJump && CheckBeforeGroundEnd())
	{
		if (m_isWalking)
		{
			ChangeAnimation("SomaLeftWalking");
			m_AnimState = EPlayerAnimState::Move;
		}
		else
		{
			ChangeAnimation("SomaLeftWalkStart");
			m_AnimState = EPlayerAnimState::MoveStart;		
		}
	}
	m_isLeft = true;
}

void CPlayer::MoveRight(float DeltaTime)
{
	if (m_AnimState == EPlayerAnimState::Down)
	{
		m_isLeft = false;
		return;
	}

	if (m_AnimState == EPlayerAnimState::Attack
		|| m_AnimState == EPlayerAnimState::DownAttack)
		return;

	Move(Vector2(1.f, 0.f));

	if (!m_Jump && !m_DoubleJump && !m_SuperJump && CheckBeforeGroundEnd())
	{
		if (m_isWalking)
		{
			ChangeAnimation("SomaRightWalking");
			m_AnimState = EPlayerAnimState::Move;		
		}
		else
		{
			ChangeAnimation("SomaRightWalkStart");
			m_AnimState = EPlayerAnimState::MoveStart;
		}
	}
	m_isLeft = false;
}

void CPlayer::BulletFire(float DeltaTime)
{
	if (m_Jump || m_DoubleJump || m_SuperJump)
	{
		m_isLeft ? ChangeAnimation("SomaLeftLevateinJump") : ChangeAnimation("SomaRightLevateinJump");
		m_AnimState = EPlayerAnimState::JumpAttack;
	}
	else if (m_AnimState == EPlayerAnimState::Down)
	{
		m_isLeft ? ChangeAnimation("SomaLeftLevateinDown") : ChangeAnimation("SomaRightLevateinDown");
		m_AnimState = EPlayerAnimState::DownAttack;
	}
	else if (m_AnimState == EPlayerAnimState::Up)
	{
		m_isLeft ? ChangeAnimation("SomaLeftSkillPos") : ChangeAnimation("SomaRightSkillPos");
		m_AnimState = EPlayerAnimState::SoulAttack;
	}
	else
	{
		m_isLeft ? ChangeAnimation("SomaLeftLevatein") : ChangeAnimation("SomaRightLevatein");
		m_AnimState = EPlayerAnimState::Attack;
	}
}

void CPlayer::Pause(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(0.f);
}

void CPlayer::Resume(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(1.f);
}

void CPlayer::JumpKey(float DeltaTime)
{
	if (!m_DoubleJump && m_Jump)
	{
		DoubleJump();
		if (m_AnimState != EPlayerAnimState::DoubleJump)
		{
			m_AnimState = EPlayerAnimState::DoubleJump;
		}
	}
	else
	{
		if (m_AnimState == EPlayerAnimState::Down
			|| m_AnimState == EPlayerAnimState::DownAttack)
		{
			if (!IsBottomWallNotSideColl())
			{
				m_Pos.y += 20.f;
				m_PrevPos.y += 20.f;
				
				m_AnimState = EPlayerAnimState::DownJump;
			}
		}
		else
		{
			Jump();

			if (m_AnimState != EPlayerAnimState::Jump)
			{
				m_AnimState = EPlayerAnimState::Jump;
			}
		}		
	}
}

void CPlayer::SuperJumpKey(float DeltaTime)
{	
	SuperJump();
	m_isCeiling = false;
	m_AnimState = EPlayerAnimState::SuperJump;
}

void CPlayer::CheckJump()
{
	if (m_AnimState == EPlayerAnimState::DownJump)
	{
		m_isLeft ? ChangeAnimation("SomaLeftJumpFalling") : ChangeAnimation("SomaRightJumpFalling");
		m_Pos = Vector2(m_Pos.x, m_Pos.y + 45.f);
		m_PrevPos = Vector2(m_PrevPos.x, m_PrevPos.y + 45.f);
		m_AnimState = EPlayerAnimState::Falling;
	}

	if (m_SuperJump && (m_AnimState != EPlayerAnimState::JumpAttack))
	{
		float MoveY = m_PrevPos.y - m_Pos.y;
	
		if (0.2f < MoveY)
		{
			m_isLeft ? ChangeAnimation("SomaLeftSuperJump") : ChangeAnimation("SomaRightSuperJump");
			m_AnimState = EPlayerAnimState::SuperJump;
		}

		else if (m_isCeiling)
		{
			m_isLeft ? ChangeAnimation("SomaLeftSuperJumpCeiling") : ChangeAnimation("SomaRightSuperJumpCeiling");
			m_AnimState = EPlayerAnimState::Ceilling;
		}

		else
		{
			m_isLeft ? ChangeAnimation("SomaLeftJumpFalling") : ChangeAnimation("SomaRightJumpFalling");
			m_AnimState = EPlayerAnimState::Falling;
		}
	}

	if (m_Jump && !m_DoubleJump && (m_AnimState != EPlayerAnimState::JumpAttack))
	{
		float MoveY = m_PrevPos.y - m_Pos.y;
		if (0.2f < MoveY)
		{
			m_isLeft ? ChangeAnimation("SomaLeftJumpStart") : ChangeAnimation("SomaRightJumpStart");
			m_AnimState = EPlayerAnimState::JumpStart;
		}

		else if (-0.2f <= MoveY && MoveY <= 0.2f)
		{
			m_isLeft ? ChangeAnimation("SomaLeftJumpMax") : ChangeAnimation("SomaRightJumpMax");
			m_AnimState = EPlayerAnimState::JumpMax;
		}
		else
		{
			m_isLeft ? ChangeAnimation("SomaLeftJumpFalling") : ChangeAnimation("SomaRightJumpFalling");
			m_AnimState = EPlayerAnimState::Falling;
		}
	}

	if (m_DoubleJump && (m_AnimState != EPlayerAnimState::JumpAttack))
	{
		float MoveY = m_PrevPos.y - m_Pos.y;
		if (0.2f < MoveY)
		{
			m_isLeft ? ChangeAnimation("SomaLeftDoubleJump") : ChangeAnimation("SomaRightDoubleJump");
			m_AnimState = EPlayerAnimState::DoubleJump;
		}

		else if (-0.2f <= MoveY)
		{
			m_isLeft ? ChangeAnimation("SomaLeftJumpFalling") : ChangeAnimation("SomaRightJumpFalling");
			m_AnimState = EPlayerAnimState::Falling;
		}
	}	

	if (m_Jump || m_DoubleJump || m_SuperJump 
		|| (CheckCurrentAnimation("SomaLeftJumpFalling") || CheckCurrentAnimation("SomaRightJumpFalling")))
	{
		if (m_PrevPos.y - m_Pos.y < 0.2f)
		{
			if (IsBottomWall())
			{
				m_isLeft ? ChangeAnimation("SomaLeftBeforeGround") : ChangeAnimation("SomaRightBeforeGround");
				m_AnimState = EPlayerAnimState::JumpEnd;
			}
		}
	}

	// this is ceiling
	if (!m_Jump && !m_DoubleJump && !m_SuperJump && !(CheckCurrentAnimation("SomaLeftJumpFalling")) && !(CheckCurrentAnimation("SomaRightJumpFalling")))
	{
		if (CheckCurrentAnimation("SomaLeftJumpFalling") || CheckCurrentAnimation("SomaRightJumpFalling")
			|| CheckCurrentAnimation("SomaLeftSuperJumpCeiling") || CheckCurrentAnimation("SomaRightSuperJumpCeiling")
			|| CheckCurrentAnimation("SomaLeftLevateinJump") || CheckCurrentAnimation("SomaRightLevateinJump"))
		{
			m_isLeft ? ChangeAnimation("SomaLeftBeforeGround") : ChangeAnimation("SomaRightBeforeGround");
			m_AnimState = EPlayerAnimState::JumpEnd;
		}
	}
}

bool CPlayer::IsBottomWall()
{
	if (!m_Scene->GetTileMap())
		return false;

	CTileMap* TileMap = m_Scene->GetTileMap();

	int BottomIndexX = m_Scene->GetTileMap()->GetOriginTileIndexX(m_Pos.x) - 2;
	int BottomIndexY = m_Scene->GetTileMap()->GetOriginTileIndexX(m_Pos.y) + 1;

	if (BottomIndexX < 0)
		BottomIndexX = 0;

	if (BottomIndexY < 0)
		BottomIndexY = 0;

	if (BottomIndexX >= m_Scene->GetTileMap()->GetTileCountX())
		BottomIndexX = m_Scene->GetTileMap()->GetTileCountX() - 1;

	if (BottomIndexY >= m_Scene->GetTileMap()->GetTileCountY())
		BottomIndexY = m_Scene->GetTileMap()->GetTileCountY() - 1;

	for (int i = 0; i < 6; ++i)
	{
		if (m_Scene->GetTileMap()->GetTile(BottomIndexX + i, BottomIndexY)->GetTileOption() == ETileOption::Wall)
			return true;
	}

	return false;
}

bool CPlayer::IsBottomWallNotSideColl()
{
	if (!m_Scene->GetTileMap())
		return false;

	CTileMap* TileMap = m_Scene->GetTileMap();

	int BottomIndexX = m_Scene->GetTileMap()->GetOriginTileIndexX(m_Pos.x);
	int BottomIndexY = m_Scene->GetTileMap()->GetOriginTileIndexX(m_Pos.y) + 1;

	if (BottomIndexX < 0)
		BottomIndexX = 0;

	if (BottomIndexY < 0)
		BottomIndexY = 0;

	if (BottomIndexX >= m_Scene->GetTileMap()->GetTileCountX())
		BottomIndexX = m_Scene->GetTileMap()->GetTileCountX() - 1;

	if (BottomIndexY >= m_Scene->GetTileMap()->GetTileCountY())
		BottomIndexY = m_Scene->GetTileMap()->GetTileCountY() - 1;

	if (TileMap->GetTile(BottomIndexX, BottomIndexY)->GetTileOption() == ETileOption::Wall)
		if(TileMap->GetTile(BottomIndexX, BottomIndexY)->GetSideCollision())
			return true;

	return false;
}

void CPlayer::CheckJumpEnd()
{
	if (m_isLeft)
	{
		m_isWalking = false;
		ChangeAnimation("SomaLeftIdle");
		m_AnimState = EPlayerAnimState::Idle;
	}

	else
	{
		m_isWalking = false;
		ChangeAnimation("SomaRightIdle");
		m_AnimState = EPlayerAnimState::Idle;
	}
}

bool CPlayer::CheckBeforeGroundEnd()
{
	if (CheckCurrentAnimation("SomaLeftBeforeGround") || CheckCurrentAnimation("SomaRightBeforeGround"))
	{
		if (CheckCurrentAnimationEnd())
			return true;
		else
			return false;
	}
	return true;
}

void CPlayer::JumpSound()
{
	if (CheckCurrentAnimation("SomaLeftBeforeGround") || CheckCurrentAnimation("SomaRightBeforeGround"))
		m_Scene->GetSceneResource()->SoundPlay("BeforeGround");
	else if(CheckCurrentAnimation("SomaLeftDoubleJump") || CheckCurrentAnimation("SomaRightDoubleJump"))
		m_Scene->GetSceneResource()->SoundPlay("Jump");
	else if (CheckCurrentAnimation("SomaLeftJumpStart") || CheckCurrentAnimation("SomaRightJumpStart"))
		m_Scene->GetSceneResource()->SoundPlay("Jump");
	else if (CheckCurrentAnimation("SomaLeftSuperJump") || CheckCurrentAnimation("SomaRightSuperJump"))
		m_Scene->GetSceneResource()->SoundPlay("SuperJump");
}

void CPlayer::LeftCollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (!m_LeftAttack)
		return;

	if (Dest->GetProfile()->Name == "Player")
		return;

	Dest->GetOwner()->SetDamage((float)m_CharacterInfo.Attack + 8.f);
}

void CPlayer::RightCollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (!m_RightAttack)
		return;

	if (Dest->GetProfile()->Name == "Player")
		return;

	Dest->GetOwner()->SetDamage((float)m_CharacterInfo.Attack + 8.f);
}

void CPlayer::CheckLevelUp()
{
	if (m_CharacterInfo.Exp >= m_CharacterInfo.NeedLvUpExp)
	{
		CLevelUpFont* LevelUp = m_Scene->CreateEffectObject<CLevelUpFont>("LevelUp",
			GetMiddle());

		LevelUp->SetPrevPos(GetMiddle());

		m_Scene->GetSceneResource()->SoundPlay("LevelUp");

		m_CharacterInfo.Attack++;
		m_CharacterInfo.Armor += m_CharacterInfo.Level % 2 == 0 ? 1 : 0;
		m_CharacterInfo.HPMax += 12;
		m_CharacterInfo.HP += 12;
		m_CharacterInfo.MPMax += 5;
		m_CharacterInfo.MP += 5;
		m_CharacterInfo.NeedLvUpExp += 5;
		m_CharacterInfo.Exp = 0;
		m_CharacterInfo.Level++;
	}
}

void CPlayer::ChangeRenderTileMap(float DeltaTime)
{
	bool CurrentRenderLike = CGameManager::GetInst()->IsRenderTileMap();

	CGameManager::GetInst()->SetRenderTileMap(!CurrentRenderLike);
}

void CPlayer::ChangeRenderCollider(float DeltaTime)
{
	bool CurrentRender = CGameManager::GetInst()->IsRenderCollider();

	CGameManager::GetInst()->SetRenderCollider(!CurrentRender);
}

void CPlayer::ChangeRenderText(float DeltaTime)
{
	bool CurrentRender = CGameManager::GetInst()->IsRenderDebugText();

	CGameManager::GetInst()->SetRenderText(!CurrentRender);
}

void CPlayer::DisableAttack()
{
	m_LeftAttack = false;
	m_RightAttack = false;
}

void CPlayer::AttackEnd()
{
	switch (m_AnimState)
	{
	case JumpAttack:
		m_AnimState = EPlayerAnimState::JumpAttackEnd;
		break;
	case DownAttack:
		m_AnimState = EPlayerAnimState::DownAttackEnd;
		break;
	case Attack:
		m_AnimState = EPlayerAnimState::AttackEnd;
		break;
	default:
		break;
	}
}

void CPlayer::SoulAttack()
{
	if (m_SkillCoolTime > 0.f)
		return;

	m_SkillCoolTime = 0.5f;

	if (m_CharacterInfo.MP < 10.f)
		return;

	if (m_isLeft)
	{
		CSharedPtr<CSkill> Skill = m_Scene->CreateObject<CSkill>("Skill",
			"PlayerSkill",
			Vector2(m_Pos + Vector2(-m_Size.x / 2.f, -m_Size.y) + Vector2(-120.f, -60.f)),
			Vector2(356.f, 436.f));
		Skill->SetDir(-1.f, 0.f);
		Skill->SetSoulDamage(30.f + m_CharacterInfo.Attack * 2);
	}
	else
	{
		CSharedPtr<CSkill> Skill = m_Scene->CreateObject<CSkill>("Skill",
			"PlayerSkill",
			Vector2(m_Pos + Vector2(m_Size.x / 2.f, -m_Size.y) + Vector2(120.f, -60.f)),
			Vector2(356.f, 436.f));
		Skill->SetDir(1.f, 0.f);
		Skill->SetSoulDamage(30.f + m_CharacterInfo.Attack * 2);
	}

	m_CharacterInfo.MP -= 25;

	CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	if (State)
		State->SetMPPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);


	m_AnimState = EPlayerAnimState::SoulAttackEnd;
}

void CPlayer::Fire()
{
	int Voice = rand() % 5;

	switch (Voice)
	{
	case 0:
		m_Scene->GetSceneResource()->SoundPlay("SomaAttack1");
		break;
	case 1:
		m_Scene->GetSceneResource()->SoundPlay("SomaAttack2");
		break;
	case 2:
		m_Scene->GetSceneResource()->SoundPlay("SomaAttack3");
		break;
	case 3:
		m_Scene->GetSceneResource()->SoundPlay("SomaAttack4");
		break;
	case 4:
		m_Scene->GetSceneResource()->SoundPlay("SomaAttack5");
		break;
	default:
		break;
	}
	m_Scene->GetSceneResource()->SoundPlay("Levatein");
	if (m_isLeft)
	{
		m_LeftAttack = true;
		CCollider* Body = FindCollider("LeftMeleeAttack");
		if(Body)
			Body->ClearCollisionList();
	}
	else
	{
		m_RightAttack = true;
		CCollider* Body = FindCollider("RightMeleeAttack");
		if (Body)
			Body->ClearCollisionList();
	}
}

void CPlayer::KeepMove()
{
	m_isWalking = true;
}

void CPlayer::Save(FILE* pFile)
{
}

void CPlayer::Load(FILE* pFile)
{
}
