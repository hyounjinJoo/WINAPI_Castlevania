
#include "MainScene.h"

#include "../Object/Player/Player.h"
#include "../Object/Monster/Monster.h"
#include "../Object/Monster/Normal/Zombie/Zombie.h"
#include "../Object/Monster/Boss/Boss1.h"
#include "../Object/Effect/BloodEffect.h"
#include "../Object/Bullet.h"
#include "../Object/Skill.h"
#include "../Object/Monster/Normal/Zombie/BloodTrail.h"
#include "../Object/Door.h"
#include "../Object/EffectHit.h"
#include "../Object/LevelUpFont.h"
#include "../Input.h"

#include "SceneResource.h"
#include "Camera.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	LoadPlayerAttackSequence();

	GetCamera()->SetWorldResolution(1024.f, 1342.f);
	
	CreatePrototypes();

	return true;
}

void CMainScene::LoadAnimationSequence()
{
	LoadPlayerAnimationSequence();

	LoadEffectAnimationSequence();

	LoadMonsterAnimationSequence();

	LoadObjectAnimationSequence();
}

void CMainScene::LoadPlayerAnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("SomaRightIdle",
		"SomaRightIdle", TEXT("Player/Right/RightIdle.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightIdle",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightIdle")->GetFrameCount() == 0) 
	{
		for (int i = 0; i < 4; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightIdle",
				i * 136.f, 0.f, 136.f, 136.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaRightWalkStart",
		"SomaRightWalkStart", TEXT("Player/Right/RightWalkStart.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightWalkStart",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightWalkStart")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightWalkStart",
				i * 136.f, 0.f, 136.f, 136.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaRightWalking",
		"SomaRightWalking", TEXT("Player/Right/RightWalking.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightWalking",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightWalking")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 18; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightWalking",
				i * 136.f, 0.f, 136.f, 136.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaRightAttack",
		"SomaRightAttack", TEXT("Player/Right/RightAttack.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightAttack",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightAttack")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 10; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightAttack",
				i * 136.f, 0.f, 136.f, 136.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftIdle",
		"SomaLeftIdle", TEXT("Player/Left/LeftIdle.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftIdle",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftIdle")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftIdle",
				i * 136.f, 0.f, 136.f, 136.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftWalkStart",
		"SomaLeftWalkStart", TEXT("Player/Left/LeftWalkStart.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftWalkStart",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftWalkStart")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftWalkStart",
				i * 136.f, 0.f, 136.f, 136.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftWalking",
		"SomaLeftWalking", TEXT("Player/Left/LeftWalking.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftWalking",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftWalking")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 18; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftWalking",
				i * 136.f, 0.f, 136.f, 136.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftAttack",
		"SomaLeftAttack", TEXT("Player/Left/LeftAttack.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftAttack",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftAttack")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 10; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftAttack",
				i * 136.f, 0.f, 136.f, 136.f);
		}
	}

	//Jump Start====================================================================
	GetSceneResource()->CreateAnimationSequence("SomaRightJumpStart",
		"SomaRightJumpStart", TEXT("Player/Right/JumpStart.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightJumpStart",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightJumpStart")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightJumpStart",
				i * 76.f, 0.f, 76.f, 140.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftJumpStart",
		"SomaLeftJumpStart", TEXT("Player/Left/JumpStart.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftJumpStart",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftJumpStart")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftJumpStart",
				i * 76.f, 0.f, 76.f, 140.f);
		}
	}

	//Jump To Max====================================================================
	GetSceneResource()->CreateAnimationSequence("SomaRightJumpMax",
		"SomaRightJumpMax", TEXT("Player/Right/JumpMax.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightJumpMax",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightJumpMax")->GetFrameCount() == 0)
	{
		GetSceneResource()->AddAnimationFrameData("SomaRightJumpMax",
			0.f, 0.f, 108.f, 140.f);
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftJumpMax",
		"SomaLeftJumpMax", TEXT("Player/Left/JumpMax.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftJumpMax",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftJumpMax")->GetFrameCount() == 0)
	{
		GetSceneResource()->AddAnimationFrameData("SomaLeftJumpMax",
			0.f, 0.f, 108.f, 140.f);
	}

	//Jump Falling====================================================================
	GetSceneResource()->CreateAnimationSequence("SomaRightJumpFalling",
		"SomaRightJumpFalling", TEXT("Player/Right/JumpFalling.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightJumpFalling",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightJumpFalling")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightJumpFalling",
				i * 108.f, 0.f, 108.f, 140.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftJumpFalling",
		"SomaLeftJumpFalling", TEXT("Player/Left/JumpFalling.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftJumpFalling",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftJumpFalling")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftJumpFalling",
				i * 108.f, 0.f, 108.f, 140.f);
		}
	}

	//Jump - Before Ground====================================================================
	GetSceneResource()->CreateAnimationSequence("SomaRightBeforeGround",
		"SomaRightBeforeGround", TEXT("Player/Right/BeforeGround.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightBeforeGround",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightBeforeGround")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightBeforeGround",
				i * 112.f, 0.f, 112.f, 140.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftBeforeGround",
		"SomaLeftBeforeGround", TEXT("Player/Left/BeforeGround.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftBeforeGround",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftBeforeGround")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftBeforeGround",
				i * 112.f, 0.f, 112.f, 140.f);
		}
	}

	//Double Jump ====================================================================
	GetSceneResource()->CreateAnimationSequence("SomaRightDoubleJump",
		"SomaRightDoubleJump", TEXT("Player/Right/DoubleJump.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightDoubleJump",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightDoubleJump")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightDoubleJump",
				i * 112.f, 0.f, 112.f, 140.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftDoubleJump",
		"SomaLeftDoubleJump", TEXT("Player/Left/DoubleJump.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftDoubleJump",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftDoubleJump")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftDoubleJump",
				i * 112.f, 0.f, 112.f, 140.f);
		}
	}

	//Super Jump ====================================================================
	GetSceneResource()->CreateAnimationSequence("SomaRightSuperJump",
		"SomaRightSuperJump", TEXT("Player/Right/RightSuperJump.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightSuperJump",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightSuperJump")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightSuperJump",
				i * 60.f, 0.f, 60.f, 140.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftSuperJump",
		"SomaLeftSuperJump", TEXT("Player/Left/LeftSuperJump.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftSuperJump",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftSuperJump")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftSuperJump",
				i * 60.f, 0.f, 60.f, 140.f);
		}
	}

	//Super Jump Ceiling============================================================
	GetSceneResource()->CreateAnimationSequence("SomaRightSuperJumpCeiling",
		"SomaRightSuperJumpCeiling", TEXT("Player/Right/SuperJumpCeiling.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightSuperJumpCeiling",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightSuperJumpCeiling")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightSuperJumpCeiling",
				i * 88.f, 0.f, 88.f, 124.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftSuperJumpCeiling",
		"SomaLeftSuperJumpCeiling", TEXT("Player/Left/SuperJumpCeiling.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftSuperJumpCeiling",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftSuperJumpCeiling")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftSuperJumpCeiling",
				i * 88.f, 0.f, 88.f, 124.f);
		}
	}

	//Soma Up=====================================================================
	GetSceneResource()->CreateAnimationSequence("SomaLeftUp",
		"SomaLeftUp", TEXT("Player/Left/Up.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftUp",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftUp")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftUp",
				i * 96.f, 0.f, 96.f, 132.f);
		}
	}


	GetSceneResource()->CreateAnimationSequence("SomaRightUp",
		"SomaRightUp", TEXT("Player/Right/Up.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightUp",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightUp")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightUp",
				i * 96.f, 0.f, 96.f, 132.f);
		}
	}
	//Soma Down===================================================================
	GetSceneResource()->CreateAnimationSequence("SomaLeftDown",
		"SomaLeftDown", TEXT("Player/Left/Down.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftDown",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftDown")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftDown",
				i * 124.f, 0.f, 124.f, 96.f);
		}
	}


	GetSceneResource()->CreateAnimationSequence("SomaRightDown",
		"SomaRightDown", TEXT("Player/Right/Down.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightDown",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightDown")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightDown",
				i * 124.f, 0.f, 124.f, 96.f);
		}
	}
	//Soma Skill Pos===================================================================
	GetSceneResource()->CreateAnimationSequence("SomaLeftSkillPos",
		"SomaLeftSkillPos", TEXT("Player/Left/SkillAttackPos.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftSkillPos",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftSkillPos")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftSkillPos",
				i * 136.f, 0.f, 136.f, 132.f);
		}
	}


	GetSceneResource()->CreateAnimationSequence("SomaRightSkillPos",
		"SomaRightSkillPos", TEXT("Player/Right/SkillAttackPos.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightSkillPos",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightSkillPos")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightSkillPos",
				i * 136.f, 0.f, 136.f, 132.f);
		}
	}
}

void CMainScene::LoadPlayerAttackSequence()
{
	//Levatein_NORMAL===============================================================
	GetSceneResource()->CreateAnimationSequence("SomaRightLevatein",
		"SomaRightLevatein", TEXT("Player/Right/Levatein.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightLevatein",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightLevatein")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 12; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightLevatein",
				i * 412.f, 0.f, 412.f, 205.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftLevatein",
		"SomaLeftLevatein", TEXT("Player/Left/Levatein.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftLevatein",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftLevatein")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 12; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftLevatein",
				i * 412.f, 0.f, 412.f, 205.f);
		}
	}

	//Levatein_Jump================================================================
	GetSceneResource()->CreateAnimationSequence("SomaRightLevateinJump",
		"SomaRightLevateinJump", TEXT("Player/Right/LevateinJump.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightLevateinJump",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightLevateinJump")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaRightLevateinJump",
				i * 424.f, 0.f, 424.f, 205.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftLevateinJump",
		"SomaLeftLevateinJump", TEXT("Player/Left/LevateinJump.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftLevateinJump",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftLevateinJump")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("SomaLeftLevateinJump",
				i * 424.f, 0.f, 424.f, 205.f);
		}
	}


	////Levatein_Down================================================================
	GetSceneResource()->CreateAnimationSequence("SomaRightLevateinDown",
		"SomaRightLevateinDown", TEXT("Player/Right/LevateinDown.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaRightLevateinDown",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaRightLevateinDown")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				GetSceneResource()->AddAnimationFrameData("SomaRightLevateinDown",
					j * 424.f, i * 174.f, 424.f, 174.f);
			}
		}
	}

	GetSceneResource()->CreateAnimationSequence("SomaLeftLevateinDown",
		"SomaLeftLevateinDown", TEXT("Player/Left/LevateinDown.bmp"));

	GetSceneResource()->SetTextureColorKey("SomaLeftLevateinDown",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("SomaLeftLevateinDown")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				GetSceneResource()->AddAnimationFrameData("SomaLeftLevateinDown",
					j * 424.f, i * 174.f, 424.f, 174.f);
			}
		}
	}
}

void CMainScene::LoadMonsterAnimationSequence()
{
	ZombieAnimationSequence();
	Boss1AnimationSequence();
}

void CMainScene::LoadObjectAnimationSequence()
{
	DoorAnimationSequence();	
}

void CMainScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", true, "CastleCorrider", "StageSound/CastleCorrider.wav");

	GetSceneResource()->LoadSound("BGM", false, "BossStart", "StageSound/BossStart.wav");
	GetSceneResource()->LoadSound("BGM", true, "BossLoop", "StageSound/BossLoop.wav");

	GetSceneResource()->LoadSound("Effect", false, "Jump", "Player/Jump/Jump.wav");
	GetSceneResource()->LoadSound("Effect", false, "SuperJump", "Player/Jump/SuperJump.wav");
	GetSceneResource()->LoadSound("Effect", false, "BeforeGround", "Player/Jump/BeforeGround.wav");
	GetSceneResource()->LoadSound("Effect", false, "DoorOpen", "StageSound/DoorOpen.wav");

	GetSceneResource()->LoadSound("Effect", false, "Levatein", "Player/Attack/Levatein.wav");

	GetSceneResource()->LoadSound("Effect", false, "Skill", "Soul/Skill.wav");
	GetSceneResource()->LoadSound("Effect", false, "Skill", "Soul/Skill2.wav");

	GetSceneResource()->LoadSound("Effect", false, "SomaAttack1", "Player/Voice/Voice1.wav");
	GetSceneResource()->LoadSound("Effect", false, "SomaAttack2", "Player/Voice/Voice2.wav");
	GetSceneResource()->LoadSound("Effect", false, "SomaAttack3", "Player/Voice/Voice3.wav");
	GetSceneResource()->LoadSound("Effect", false, "SomaAttack4", "Player/Voice/Voice4.wav");
	GetSceneResource()->LoadSound("Effect", false, "SomaAttack5", "Player/Voice/Voice5.wav");

	GetSceneResource()->LoadSound("Effect", false, "Swing", "Effect/Swing.wav");
	GetSceneResource()->LoadSound("Effect", false, "Fire", "Effect/Fire.wav");
	GetSceneResource()->LoadSound("Effect", false, "LevelUp", "Effect/LevelUp.wav");
}

void CMainScene::CreatePrototypes()
{
	CBloodTrail* BloodTrail = CreatePrototype<CBloodTrail>("BloodTrailEffect");

	CBloodEffect* Blood = CreatePrototype<CBloodEffect>("BloodDropEffect");

	CEffectHit* EffectPrototype = CreatePrototype<CEffectHit>("HitEffect");

	CSkill* Skill = CreatePrototype<CSkill>("PlayerSkill");

	CZombie* ZombieProto = CreatePrototype<CZombie>("Zombie");

	CCollider* Collider = ZombieProto->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("Monster");

	CBoss1* Boss = CreatePrototype<CBoss1>("Boss");

	Collider = Boss->FindCollider("Body");
	if(Collider)
		Collider->SetCollisionProfile("Monster");

	Collider = Boss->FindCollider("BossAttack");
	if (Collider)
		Collider->SetCollisionProfile("MonsterAttack");

	CBullet* BossFire = CreatePrototype<CBullet>("BossFire");

	Collider = BossFire->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("MonsterAttack");
}

void CMainScene::LoadEffectAnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("HitBloodEffect",
		"HitBloodEffect", TEXT("Effect/HitBloodEffect.bmp"));

	GetSceneResource()->SetTextureColorKey("HitBloodEffect",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("HitBloodEffect")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 5; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("HitBloodEffect",
				i * 116.f, 0.f, 116.f, 124.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("LeftSkill",
		"LeftSkill", TEXT("Player/Left/SkillAttack.bmp"));

	GetSceneResource()->SetTextureColorKey("LeftSkill",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("LeftSkill")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 8; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("LeftSkill",
				i * 356.f, 0.f, 356.f, 436.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("RightSkill",
		"RightSkill", TEXT("Player/Right/SkillAttack.bmp"));

	GetSceneResource()->SetTextureColorKey("RightSkill",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("RightSkill")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 8; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("RightSkill",
				i * 356.f, 0.f, 356.f, 436.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("FireBallStart",
		"FireBallStart", TEXT("Monster/Boss1/FireBallStart.bmp"));

	GetSceneResource()->SetTextureColorKey("FireBallStart",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("FireBallStart")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("FireBallStart",
				i * 160.f, 0.f, 160.f, 116.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("FireBallMiddle",
		"FireBallMiddle", TEXT("Monster/Boss1/FireBallMiddle.bmp"));

	GetSceneResource()->SetTextureColorKey("FireBallMiddle",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("FireBallMiddle")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 11; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("FireBallMiddle",
				i * 160.f, 0.f, 160.f, 116.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("LevelUp",
		"LevelUp", TEXT("Effect/LevelUp.bmp"));

	GetSceneResource()->SetTextureColorKey("LevelUp",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("LevelUp")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 2; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("LevelUp",
				i * 128.f, 0.f, 128.f, 56.f);
		}
	}
}

void CMainScene::ZombieAnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("ZombieDropBlood",
		"ZombieDropBlood", TEXT("Monster/Zombie/ZombieDropBlood.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieDropBlood",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("ZombieDropBlood")->GetFrameCount() == 0)
	{
		GetSceneResource()->AddAnimationFrameData("ZombieDropBlood",
			0.f, 0.f, 8.f, 16.f);
	}

	GetSceneResource()->CreateAnimationSequence("ZombieBloodTrail",
		"ZombieBloodTrail", TEXT("Monster/Zombie/ZombieBloodTrail.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieBloodTrail",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("ZombieBloodTrail")->GetFrameCount() == 0)
	{
		GetSceneResource()->AddAnimationFrameData("ZombieBloodTrail",
			0.f, 0.f, 124.f, 4.f);
	}

	GetSceneResource()->CreateAnimationSequence("ZombieLeftCreate",
		"ZombieLeftCreate", TEXT("Monster/Zombie/Left/ZombieLeftCreate.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieLeftCreate",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("ZombieLeftCreate")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("ZombieLeftCreate",
				i * 92.f, 0.f, 92.f, 144.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("ZombieLeftDig",
		"ZombieLeftDig", TEXT("Monster/Zombie/Left/ZombieLeftDig.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieLeftDig",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("ZombieLeftDig")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("ZombieLeftDig",
				i * 92.f, 0.f, 92.f, 144.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("ZombieLeftDeath",
		"ZombieLeftDeath", TEXT("Monster/Zombie/Left/ZombieLeftDeath.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieLeftDeath",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("ZombieLeftDeath")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 1; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("ZombieLeftDeath",
				i * 120.f, 0.f, 120.f, 166.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("ZombieALeftWalk",
		"ZombieALeftWalk", TEXT("Monster/Zombie/Left/ZombieALeftWalk.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieALeftWalk",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("ZombieALeftWalk")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("ZombieALeftWalk",
				i * 92.f, 0.f, 92.f, 144.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("ZombieBLeftWalk",
		"ZombieBLeftWalk", TEXT("Monster/Zombie/Left/ZombieBLeftWalk.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieBLeftWalk",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("ZombieBLeftWalk")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("ZombieBLeftWalk",
				i * 88.f, 0.f, 88.f, 144.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("ZombieRightCreate",
		"ZombieRightCreate", TEXT("Monster/Zombie/Right/ZombieRightCreate.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieRightCreate",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("ZombieRightCreate")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("ZombieRightCreate",
				i * 92.f, 0.f, 92.f, 144.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("ZombieRightDig",
		"ZombieRightDig", TEXT("Monster/Zombie/Right/ZombieRightDig.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieRightDig",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("ZombieRightDig")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("ZombieRightDig",
				i * 92.f, 0.f, 92.f, 144.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("ZombieRightDeath",
		"ZombieRightDeath", TEXT("Monster/Zombie/Right/ZombieRightDeath.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieRightDeath",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("ZombieRightDeath")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 1; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("ZombieRightDeath",
				i * 120.f, 0.f, 120.f, 166.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("ZombieARightWalk",
		"ZombieARightWalk", TEXT("Monster/Zombie/Right/ZombieARightWalk.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieARightWalk",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("ZombieARightWalk")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("ZombieARightWalk",
				i * 92.f, 0.f, 92.f, 144.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("ZombieBRightWalk",
		"ZombieBRightWalk", TEXT("Monster/Zombie/Right/ZombieBRightWalk.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieBRightWalk",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("ZombieBRightWalk")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("ZombieBRightWalk",
				i * 88.f, 0.f, 88.f, 144.f);
		}
	}
}

void CMainScene::Boss1AnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("BossCrawling",
		"BossCrawling", TEXT("Monster/Boss1/Crawling.bmp"));

	GetSceneResource()->SetTextureColorKey("BossCrawling",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("BossCrawling")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 7; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				GetSceneResource()->AddAnimationFrameData("BossCrawling",
					j * 745.f, i * 560.f, 745.f, 560.f);
			}
		}
	}

	GetSceneResource()->CreateAnimationSequence("FirePos",
		"FirePos", TEXT("Monster/Boss1/FirePos.bmp"));

	GetSceneResource()->SetTextureColorKey("FirePos",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("FirePos")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				if (i >= 3 && j >= 3)
					break;

				GetSceneResource()->AddAnimationFrameData("FirePos",
					j * 745.f, i * 560.f, 745.f, 560.f);

			}
		}
	}

	GetSceneResource()->CreateAnimationSequence("Swing",
		"Swing", TEXT("Monster/Boss1/Swing.bmp"));

	GetSceneResource()->SetTextureColorKey("Swing",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("Swing")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				if (i >= 7 && j >= 4)
					break;

				GetSceneResource()->AddAnimationFrameData("Swing",
					j * 745.f, i * 560.f, 745.f, 560.f);
			}
		}
	}
}

void CMainScene::DoorAnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("LeftOpenDoor",
		"LeftOpenDoor", TEXT("Object/Door/LeftOpenDoor.bmp"));

	GetSceneResource()->SetTextureColorKey("LeftOpenDoor",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("LeftOpenDoor")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				GetSceneResource()->AddAnimationFrameData("LeftOpenDoor",
					j * 128.f, i * 192.f, 128.f, 192.f);
			}
		}
	}

	GetSceneResource()->CreateAnimationSequence("LeftIdleDoor",
		"LeftIdleDoor", TEXT("Object/Door/LeftIdleDoor.bmp"));

	GetSceneResource()->SetTextureColorKey("LeftIdleDoor",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("LeftIdleDoor")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("LeftIdleDoor",
				i * 128.f, 0.f, 128.f, 192.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("RightOpenDoor",
		"RightOpenDoor", TEXT("Object/Door/RightOpenDoor.bmp"));

	GetSceneResource()->SetTextureColorKey("RightOpenDoor",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("RightOpenDoor")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				GetSceneResource()->AddAnimationFrameData("RightOpenDoor",
					j * 128.f, i * 192.f, 128.f, 192.f);
			}
		}
	}

	GetSceneResource()->CreateAnimationSequence("RightIdleDoor",
		"RightIdleDoor", TEXT("Object/Door/RightIdleDoor.bmp"));

	GetSceneResource()->SetTextureColorKey("RightIdleDoor",
		255, 0, 255);

	if (GetSceneResource()->FindAnimationSequence("RightIdleDoor")->GetFrameCount() == 0)
	{
		for (int i = 0; i < 3; ++i)
		{
			GetSceneResource()->AddAnimationFrameData("RightIdleDoor",
				i * 128.f, 0.f, 128.f, 192.f);
		}
	}
}
