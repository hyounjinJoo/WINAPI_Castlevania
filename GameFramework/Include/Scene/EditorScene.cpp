
#include "EditorScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIEditor.h"
#include "../Input.h"
#include "EditorDlg.h"
#include "../Map/TileMap.h"
#include "../GameManager.h"

#include "../Object/Player/Player.h"
#include "../Object/Bullet.h"
#include "../Object/Monster/Monster.h"
#include "../Object/Monster/Normal/Zombie/Zombie.h"
#include "../Object/Effect/BloodEffect.h"
#include "../Object/Monster/Normal/Zombie/BloodTrail.h"

CEditorScene::CEditorScene() :
	m_Dlg(nullptr),
	m_Start(false),
	m_TileMap(nullptr),
	m_ScrollSpeed(500.f),
	m_LineStart(0, 0),
	m_LineDest(0, 0),
	m_MouseL(E_Mend)
{
	CGameManager::GetInst()->SetEditorMode(true);
}

CEditorScene::~CEditorScene()
{
	CGameManager::GetInst()->SetEditorMode(false);
	SAFE_DELETE(m_Dlg);
}

bool CEditorScene::Init()
{
	CScene::Init();

	LoadSound();

	LoadAnimationSequence();

	CreateProtoType();

	m_Dlg = new CEditorDlg;

	m_Dlg->SetScene(this);
	m_Dlg->Init(IDD_DIALOG_EDITOR);


	//CUIStart* StartWindow = CreateUIWindow<CUIStart>("StartWindow");
	CUIEditor* EditorWindow = CreateUIWindow<CUIEditor>("EditorWindow");

	return true;
}

bool CEditorScene::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);
	m_Dlg->Update();

	if (!m_Start)
	{
		m_Start = true;

		CInput::GetInst()->SetCallback<CEditorScene>("Editor", KeyState_Down,
			this, &CEditorScene::OnEditor);

		CInput::GetInst()->SetCallback<CEditorScene>("MoveUp", KeyState_Push,
			this, &CEditorScene::CameraMoveUp);

		CInput::GetInst()->SetCallback<CEditorScene>("MoveDown", KeyState_Push,
			this, &CEditorScene::CameraMoveDown);

		CInput::GetInst()->SetCallback<CEditorScene>("MoveLeft", KeyState_Push,
			this, &CEditorScene::CameraMoveLeft);

		CInput::GetInst()->SetCallback<CEditorScene>("MoveRight", KeyState_Push,
			this, &CEditorScene::CameraMoveRight);


		CInput::GetInst()->SetCallback<CEditorScene>("MouseLButton", KeyState_Push,
			this, &CEditorScene::MouseLButton);
		CInput::GetInst()->SetCallback<CEditorScene>("MouseRButton", KeyState_Push,
			this, &CEditorScene::MouseRButton);
	}

	RECT	rc = m_Dlg->GetRect();

	POINT	ptMouse;
	GetCursorPos(&ptMouse);

	if (rc.left <= ptMouse.x && ptMouse.x <= rc.right &&
		rc.top <= ptMouse.y && ptMouse.y <= rc.bottom)
	{
		if (!CInput::GetInst()->IsShowCursor())
		{
			CInput::GetInst()->SetShowCursor(true);
			ShowCursor(TRUE);
		}
	}

	else
	{
		if (CInput::GetInst()->IsShowCursor())
		{
			CInput::GetInst()->SetShowCursor(false);
			ShowCursor(FALSE);
		}
	}


	if (!m_TileMap)
		return false;

	Vector2	MousePos = CInput::GetInst()->GetMousePos();

	CCamera* Camera = GetCamera();

	if (MousePos.x < 0.f || MousePos.x > Camera->GetResolution().x ||
		MousePos.y < 0.f || MousePos.y > Camera->GetResolution().y)
		return false;

	MousePos += Camera->GetPos();

	EEditMode EditSelect = m_Dlg->GetEditSelection();
	ETileEditMode	EditMode = m_Dlg->GetTileEditMode();

	switch (EditSelect)
	{
	case EEditMode::TileMap:
		switch (EditMode)
		{
			case ETileEditMode::Line:
				if (CInput::GetInst()->GetMouseDown())
				{
					m_MouseL = E_Mdown;

					m_LineStart = m_TileMap->GetTile(MousePos)->GetPos();

					m_TileMap->CreateLine(m_LineStart, m_LineStart);
				}
				else if (CInput::GetInst()->GetMousePush())
				{
					m_MouseL = E_Mpush;

					m_LineDest = m_TileMap->GetTile(MousePos)->GetPos();

					m_TileMap->GetLastLine()->SetDestPoint(m_LineDest);
				}
				else if (CInput::GetInst()->GetMouseUp() && m_MouseL == E_Mpush)
				{
					m_MouseL = E_Mend;

					if (Distance(m_LineStart, m_LineDest) < 100.f)
					{
						m_TileMap->DeleteLastLine();
					}
					m_LineStart = Vector2(0.f, 0.f);
					m_LineDest = Vector2(0.f, 0.f);
				}
				break;
			case ETileEditMode::End:
				break;
			default:
				break;
		}
		break;
	case EEditMode::Spawner:
		{
			RECT SpawnerRect;
			Vector2 SpawnerPos;
			SpawnerPos = MousePos - GetCamera()->GetPos();
			SpawnerRect.left = (long)SpawnerPos.x;
			SpawnerRect.top = (long)SpawnerPos.y;
			SpawnerRect.bottom = (long)(SpawnerPos.y + 100.f);
			SpawnerRect.right = (long)(SpawnerPos.x + 100.f);

			FrameRect(CGameManager::GetInst()->GetWindowDC(), &SpawnerRect, CGameManager::GetInst()->GetGreenBrush());
		}
		break;
	case EEditMode::Door:
		{
			RECT DoorRect;
			Vector2 DoorPos;
			DoorPos = MousePos - GetCamera()->GetPos();
			DoorRect.left = (long)DoorPos.x;
			DoorRect.top = (long)DoorPos.y;
			DoorRect.bottom = (long)(DoorPos.y + 192.f);
			DoorRect.right = (long)(DoorPos.x + 128.f);

			FrameRect(CGameManager::GetInst()->GetWindowDC(), &DoorRect, CGameManager::GetInst()->GetGreenBrush());
		}
		break;
	case EEditMode::End:
		break;
	default:
		break;
	}

	return false;
}

bool CEditorScene::PostUpdate(float DeltaTime)
{
	CScene::PostUpdate(DeltaTime);

	return false;
}

void CEditorScene::LoadAnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("ZombieLeftCreate",
		"ZombieLeftCreate", TEXT("Monster/Zombie/Left/ZombieLeftCreate.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieLeftCreate",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("ZombieLeftCreate",
			i * 92.f, 0.f, 92.f, 144.f);
	}

	GetSceneResource()->CreateAnimationSequence("ZombieLeftDig",
		"ZombieLeftDig", TEXT("Monster/Zombie/Left/ZombieLeftDig.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieLeftDig",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("ZombieLeftDig",
			i * 92.f, 0.f, 92.f, 144.f);
	}

	GetSceneResource()->CreateAnimationSequence("ZombieLeftDeath",
		"ZombieLeftDeath", TEXT("Monster/Zombie/Left/ZombieLeftDeath.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieLeftDeath",
		255, 0, 255);

	for (int i = 0; i < 1; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("ZombieLeftDeath",
			i * 92.f, 0.f, 92.f, 144.f);
	}

	GetSceneResource()->CreateAnimationSequence("ZombieALeftWalk",
		"ZombieALeftWalk", TEXT("Monster/Zombie/Left/ZombieALeftWalk.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieALeftWalk",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("ZombieALeftWalk",
			i * 92.f, 0.f, 92.f, 144.f);
	}

	GetSceneResource()->CreateAnimationSequence("ZombieBLeftWalk",
		"ZombieBLeftWalk", TEXT("Monster/Zombie/Left/ZombieBLeftWalk.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieBLeftWalk",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("ZombieBLeftWalk",
			i * 88.f, 0.f, 88.f, 144.f);
	}

	GetSceneResource()->CreateAnimationSequence("ZombieRightCreate",
		"ZombieRightCreate", TEXT("Monster/Zombie/Right/ZombieRightCreate.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieRightCreate",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("ZombieRightCreate",
			i * 92.f, 0.f, 92.f, 144.f);
	}

	GetSceneResource()->CreateAnimationSequence("ZombieRightDig",
		"ZombieRightDig", TEXT("Monster/Zombie/Right/ZombieRightDig.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieRightDig",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("ZombieRightDig",
			i * 92.f, 0.f, 92.f, 144.f);
	}

	GetSceneResource()->CreateAnimationSequence("ZombieRightDeath",
		"ZombieRightDeath", TEXT("Monster/Zombie/Right/ZombieRightDeath.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieRightDeath",
		255, 0, 255);

	for (int i = 0; i < 1; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("ZombieRightDeath",
			i * 92.f, 0.f, 92.f, 144.f);
	}

	GetSceneResource()->CreateAnimationSequence("ZombieARightWalk",
		"ZombieARightWalk", TEXT("Monster/Zombie/Right/ZombieARightWalk.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieARightWalk",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("ZombieARightWalk",
			i * 92.f, 0.f, 92.f, 144.f);
	}

	GetSceneResource()->CreateAnimationSequence("ZombieBRightWalk",
		"ZombieBRightWalk", TEXT("Monster/Zombie/Right/ZombieBRightWalk.bmp"));

	GetSceneResource()->SetTextureColorKey("ZombieBRightWalk",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("ZombieBRightWalk",
			i * 88.f, 0.f, 88.f, 144.f);
	}

	GetSceneResource()->CreateAnimationSequence("LeftOpenDoor",
		"LeftOpenDoor", TEXT("Object/Door/LeftOpenDoor.bmp"));

	GetSceneResource()->SetTextureColorKey("LeftOpenDoor",
		255, 0, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			GetSceneResource()->AddAnimationFrameData("LeftOpenDoor",
				j * 128.f, i * 192.f, 128.f, 192.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("LeftIdleDoor",
		"LeftIdleDoor", TEXT("Object/Door/LeftIdleDoor.bmp"));

	GetSceneResource()->SetTextureColorKey("LeftIdleDoor",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LeftIdleDoor",
			i * 128.f, 0.f, 128.f, 192.f);
	}

	GetSceneResource()->CreateAnimationSequence("RightOpenDoor",
		"RightOpenDoor", TEXT("Object/Door/RightOpenDoor.bmp"));

	GetSceneResource()->SetTextureColorKey("RightOpenDoor",
		255, 0, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			GetSceneResource()->AddAnimationFrameData("RightOpenDoor",
				j * 128.f, i * 192.f, 128.f, 192.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("RightIdleDoor",
		"RightIdleDoor", TEXT("Object/Door/RightIdleDoor.bmp"));

	GetSceneResource()->SetTextureColorKey("RightIdleDoor",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("RightIdleDoor",
			i * 128.f, 0.f, 128.f, 192.f);
	}
}

void CEditorScene::LoadSound()
{
	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "TeemoSmile.mp3");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "TeemoStartClicck.mp3");

	GetSceneResource()->SetVolume("UI", 30);
}

void CEditorScene::CreateProtoType()
{

	CZombie* ZombieMonster = CreatePrototype<CZombie>("Zombie");
	CCollider* Collider = ZombieMonster->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("Monster");

	CDoor* Door = CreatePrototype<CDoor>("Door");
	Collider = Door->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("Door");
}

void CEditorScene::OnEditor(float DeltaTime)
{
}

void CEditorScene::CreateTileMap()
{
	if (m_TileMap)
		return;

	m_TileMap = CreateMap<CTileMap>("TileMap");
	m_TileMap->SetZOrder(1);
}

void CEditorScene::SetTileInfo(int CountX, int CountY, int SizeX, int SizeY)
{
	m_TileMap->CreateTile(CountX, CountY, Vector2((float)SizeX, (float)SizeY));
}

void CEditorScene::SetTileTexture(CTexture* Texture)
{
	m_TileMap->SetTileTexture(Texture);
}

void CEditorScene::CameraMoveUp(float DeltaTime)
{
	CCamera* Camera = GetCamera();

	Camera->Move(Vector2(0.f, -1.f) * m_ScrollSpeed * DeltaTime);
}

void CEditorScene::CameraMoveDown(float DeltaTime)
{
	CCamera* Camera = GetCamera();

	Camera->Move(Vector2(0.f, 1.f) * m_ScrollSpeed * DeltaTime);
}

void CEditorScene::CameraMoveLeft(float DeltaTime)
{
	CCamera* Camera = GetCamera();

	Camera->Move(Vector2(-1.f, 0.f) * m_ScrollSpeed * DeltaTime);
}

void CEditorScene::CameraMoveRight(float DeltaTime)
{
	CCamera* Camera = GetCamera();

	Camera->Move(Vector2(1.f, 0.f) * m_ScrollSpeed * DeltaTime);
}

void CEditorScene::MouseLButton(float DeltaTime)
{
	if (!m_TileMap)
		return;

	Vector2	MousePos = CInput::GetInst()->GetMousePos();

	CCamera* Camera = GetCamera();

	if(MousePos.x < 0.f || MousePos.x > Camera->GetResolution().x ||
		MousePos.y < 0.f || MousePos.y > Camera->GetResolution().y)
		return;


	EEditMode EditSelect = m_Dlg->GetEditSelection();
	ETileEditMode	EditMode = m_Dlg->GetTileEditMode();

	MousePos += Camera->GetPos();
	switch (EditSelect)
	{
	case EEditMode::TileMap:
		switch (EditMode)
		{
			case ETileEditMode::Option:
			{
				ETileOption	Option = m_Dlg->GetTileOption();

				m_TileMap->ChangeTileOption(MousePos, Option);

				m_TileMap->ChangeTileSideCollision(MousePos, m_Dlg->GetSideCollision());
			}
			break;
			case ETileEditMode::Image:
			{
				TileFrameData	FrameData = m_Dlg->GetTileFrameData();

				m_TileMap->SetTileFrame(MousePos, FrameData.Start, FrameData.End);
			}
			break;
		}
		break;
	case EEditMode::Spawner:
		if (CInput::GetInst()->GetMouseDown())
		{
			ESpawnerEditOBJ SpawnObj = m_Dlg->GetSpawnObj();
			bool IsSpawnDirLeft = m_Dlg->GetSpawnDir() == ESpawnerEditDir::Left ? true : false;
			int SpawnLimit = m_Dlg->GetSpawnLimit();

			switch (SpawnObj)
			{
			case ESpawnerEditOBJ::Zombie:
				m_TileMap->AddSpawner("ZombieSpawner", MousePos.x, MousePos.y, IsSpawnDirLeft, "Zombie", false, SpawnLimit);
				break;
			case ESpawnerEditOBJ::Boss:
				m_TileMap->AddSpawner("BossSpawner", MousePos.x, MousePos.y, IsSpawnDirLeft, "Boss", false, 1);
				break;
			case ESpawnerEditOBJ::End:
			default:
				break;
			}
		}
		break;
	case EEditMode::Door:
		if (CInput::GetInst()->GetMouseDown())
		{
			EDoorStage Door = m_Dlg->GetDoorStage();
			bool IsDoorFormLeft = m_Dlg->GetDoorForm() == EDoorForm::Left ? true : false;

			m_TileMap->AddDoor(Door, MousePos, IsDoorFormLeft);
		}
		break;
	case EEditMode::End:
		break;
	default:
		break;
	}
	
}

void CEditorScene::MouseRButton(float DeltaTime)
{
	if (!m_TileMap)
		return;

	Vector2	MousePos = CInput::GetInst()->GetMousePos();

	CCamera* Camera = GetCamera();

	if (MousePos.x < 0.f || MousePos.x > Camera->GetResolution().x ||
		MousePos.y < 0.f || MousePos.y > Camera->GetResolution().y)
		return;

	EEditMode EditSelect = m_Dlg->GetEditSelection();
	ETileEditMode	EditMode = m_Dlg->GetTileEditMode();

	MousePos += Camera->GetPos();
	switch (EditSelect)
	{
	case EEditMode::TileMap:
		switch (EditMode)
		{
			case ETileEditMode::Option:
			{
				m_TileMap->ChangeTileOption(MousePos, ETileOption::Normal);
			}
			break;
			case ETileEditMode::Image:
			{
				m_TileMap->SetTileTexture(MousePos, nullptr);
			}
			break;
			case ETileEditMode::Line:
			{
				m_TileMap->ClearLine(MousePos);
			}
			break;
		}
		break;
	case EEditMode::Spawner:
		m_TileMap->ClearSpawner(MousePos);
		break;
	case EEditMode::Door:
		m_TileMap->ClearDoor(MousePos);
		break;
	case EEditMode::End:
		break;
	default:
		break;
	}	
}

void CEditorScene::Save(const char* FullPath)
{
	if (!m_TileMap)
	{
		MessageBox(0, TEXT("맵을 생성하세요"), TEXT("Error"), MB_OK);
		return;
	}

	m_TileMap->SaveFullPath(FullPath);
}

void CEditorScene::Load(const char* FullPath)
{
	if (m_TileMap)
	{
		DeleteMap(m_TileMap->GetName());
		m_TileMap = nullptr;
	}
	if (!m_TileMap)
		m_TileMap = CreateMap<CTileMap>("TileMap");

	m_TileMap->LoadFullPath(FullPath);
}

void CEditorScene::SetBackGroundTexture(CTexture* Texture)
{
	m_TileMap->SetBackGroundTexture(Texture);
}

void CEditorScene::SetMidGroundTexture(CTexture* Texture)
{
	m_TileMap->SetMidGroundTexture(Texture);
}

void CEditorScene::SetForeGroundTexture(CTexture* Texture)
{
	m_TileMap->SetForeGroundTexture(Texture);
}

void CEditorScene::TileImageAllClear()
{
	if (!m_TileMap)
		return;

	m_TileMap->TileImageAllClear();
}
