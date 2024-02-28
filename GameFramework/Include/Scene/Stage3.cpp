#include "Stage3.h"

#include "../Object/Player/Player.h"
#include "SceneResource.h"
#include "SceneManager.h"
#include "Camera.h"
#include "../UI/UIMain.h"
#include "../UI/UICharacterStateHUD.h"
#include "../Object/Monster/Boss/Boss1.h"

CStage3::CStage3()
{
}

CStage3::~CStage3()
{
}

bool CStage3::Init()
{
	if (!CMainScene::Init())
		return true;

	SetSoundBGM();

	CSceneManager::GetInst()->PrevSceneMiscAdd();

	SetPlayer("Player");

	CPlayer* Player = (CPlayer*)GetPlayer();

	Player->SetPos(100.f, 485.f);
	Player->SetPrevPos(100.f, 485.f);

	Player->SetScene(this);
	Player->SetStart(false);

	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	CTileMap* TileMap = CreateMap<CTileMap>("TileMap");
	TileMap->SetScene(this);

	TileMap->LoadFile("Stage3.map");
	TileMap->SetZOrder(0);

	if (TileMap)
	{
		int DoorCount = TileMap->GetDoorCount();

		EDoorStage Stage = EDoorStage::Stage1;
		Vector2 GetPos = Vector2(0.f, 0.f);
		bool IsDoorLeft = false;

		for (int i = 0; i < DoorCount; ++i)
		{
			Stage = TileMap->GetDoorStage(i);
			GetPos = TileMap->GetDoorPos(i);
			IsDoorLeft = TileMap->GetIsDoorLeft(i);

			CDoor* Door = CreateObject<CDoor>("Door");
			Door->SetDoor(Stage, GetPos, IsDoorLeft);
			Door->SetPrevPos(GetPos);
			Door->SetSize(128.f, 192.f);
		}
	}

#ifdef _DEBUG
	CUIMain* MainWindow = CreateUIWindow<CUIMain>("MainWindow");
#endif // DEBUG

	CUICharacterStateHUD* StateWindow = CreateUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	return true;
}

void CStage3::SetSoundBGM()
{
	if (GetSceneResource()->FindSound("CastleCorrider")->IsPlay())
		GetSceneResource()->SoundStop("CastleCorrider");
	if (!GetSceneResource()->FindSound("BossStart")->IsPlay() && !GetSceneResource()->FindSound("BossStart")->IsEnd())
		GetSceneResource()->SoundPlay("BossStart");
	if (!GetSceneResource()->FindSound("BossLoop")->IsPlay() && GetSceneResource()->FindSound("BossStart")->IsEnd())
		GetSceneResource()->SoundPlay("BossLoop");
}