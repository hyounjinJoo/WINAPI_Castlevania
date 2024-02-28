#include "Door.h"
#include "../Scene/SceneManager.h"
#include "../Collision/ColliderBox.h"
#include "../Scene/StartScene.h"
#include "../Scene/Stage1.h"
#include "../Scene/Stage2.h"
#include "../Scene/Stage3.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"
#include "Player/Player.h"

CDoor::CDoor()	:
	m_IsDoorFormLeft(false),
	m_IsOpened(false),
	m_IsMoveEnd(false),
	m_PlayerMoveCount(1.f)
{
	m_DoorStage = EDoorStage::Stage1;
}

CDoor::CDoor(const CDoor& Obj)
	:CGameObject(Obj)
{
	m_IsDoorFormLeft = Obj.m_IsDoorFormLeft;
	m_IsOpened = Obj.m_IsOpened;
	m_DoorStage = Obj.m_DoorStage;
	m_IsMoveEnd = false;
	m_PlayerMoveCount = 0.8f;
}

CDoor::~CDoor()
{
}

void CDoor::DoorCollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	if (Dest->GetOwner()->GetName() != "Player")
		return;

	if (Dest->GetProfile()->Name != "Player")
		return;

	CInput::GetInst()->ClearCallback();
	m_IsDoorFormLeft ? ChangeAnimation("LeftOpenDoor") : ChangeAnimation("RightOpenDoor");
	m_Scene->GetSceneResource()->SoundPlay("DoorOpen");
}

void CDoor::StageCreate()
{
	switch (m_DoorStage)
	{
	case EDoorStage::Stage1:
		CSceneManager::GetInst()->PrevSceneMiscSave();
		CSceneManager::GetInst()->CreateScene<CStage1>();
		break;
	case EDoorStage::Stage2:
		CSceneManager::GetInst()->PrevSceneMiscSave();
		CSceneManager::GetInst()->CreateScene<CStage2>();
		break;
	case EDoorStage::Stage3:
		CSceneManager::GetInst()->PrevSceneMiscSave();
		CSceneManager::GetInst()->CreateScene<CStage3>();
		break;
	case EDoorStage::Ending:
		CSceneManager::GetInst()->CreateScene<CStartScene>();
		break;
	default:
		break;
	}
}

void CDoor::PlayerMove(float DeltaTime)
{
	CPlayer* Player = (CPlayer*)m_Scene->GetPlayer();
	if (m_PlayerMoveCount < 0.f)
	{
		m_IsMoveEnd = true;
		Player->SetPhysicsSimulate(true);
	}
	else
	{
		Player->SetPhysicsSimulate(false);
		Player->MoveRight(DeltaTime);
		m_PlayerMoveCount -= DeltaTime;
	}	
}

void CDoor::OpenIsEnd()
{
	m_IsOpened = true;
}

void CDoor::ChangeDoor()
{
	m_IsDoorFormLeft ? ChangeAnimation("LeftIdleDoor") : ChangeAnimation("RightIdleDoor");
}

void CDoor::Start()
{
	CGameObject::Start();

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(30.f, 192.f);
	Body->SetOffset(90.f, 100.f);
	Body->SetCollisionProfile("Door");
	Body->SetCollisionBeginFunction<CDoor>(this, &CDoor::DoorCollisionBegin);
}

bool CDoor::Init()
{
	if (!CGameObject::Init())
		return false;

	CreateAnimation();

	AddAnimation("LeftIdleDoor");
	AddAnimation("LeftOpenDoor",false, 1.5f);
	AddAnimation("RightIdleDoor");
	AddAnimation("RightOpenDoor", false, 1.5f);

	AddAnimationNotify<CDoor>("LeftIdleDoor", 0, this, &CDoor::ChangeDoor);
	AddAnimationNotify<CDoor>("RightIdleDoor", 0, this, &CDoor::ChangeDoor);
	SetAnimationEndNotify<CDoor>("LeftOpenDoor", this, &CDoor::OpenIsEnd);
	SetAnimationEndNotify<CDoor>("RightOpenDoor", this, &CDoor::OpenIsEnd);

	return true;
}

void CDoor::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_IsMoveEnd)
		StageCreate();

	if (m_IsOpened)
		PlayerMove(DeltaTime);
}

void CDoor::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CDoor::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CDoor::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

void CDoor::PrevRender()
{
	CGameObject::PrevRender();
}

CDoor* CDoor::Clone()
{
	return new CDoor(*this);
}

void CDoor::Save(FILE* pFile)
{
	fwrite(&m_IsDoorFormLeft, sizeof(bool), 1, pFile);
	fwrite(&m_DoorStage, sizeof(EDoorStage), 1, pFile);
	fwrite(&m_Pos, sizeof(Vector2), 1, pFile);
	fwrite(&m_PrevPos, sizeof(Vector2), 1, pFile);
}

void CDoor::Load(FILE* pFile)
{
	fread(&m_IsDoorFormLeft, sizeof(bool), 1, pFile);
	fread(&m_DoorStage, sizeof(EDoorStage), 1, pFile);
	fread(&m_Pos, sizeof(Vector2), 1, pFile);
	fread(&m_PrevPos, sizeof(Vector2), 1, pFile);
}