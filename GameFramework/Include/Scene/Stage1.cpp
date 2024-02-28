#include "Stage1.h"
#include "../Object/Player/Player.h"
#include "SceneResource.h"
#include "Camera.h"
#include "../UI/UIMain.h"
#include "../UI/UICharacterStateHUD.h"

CStage1::CStage1()
{
}

CStage1::~CStage1()
{
}

bool CStage1::Init()
{
	if (!CMainScene::Init())
		return true;

	SetSoundBGM();

	CPlayer* Player = CreateObject<CPlayer>("Player");
	
	Player->SetPos(433.f, 2624.f);
	Player->SetPrevPos(433.f, 2624.f);
	
	SetPlayer(Player);

	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	CTileMap* TileMap = CreateMap<CTileMap>("TileMap");

	TileMap->LoadFile("Stage1.map");
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

	Player->SetSize(76.f, 136.f);

#ifdef _DEBUG
	CUIMain* MainWindow = CreateUIWindow<CUIMain>("MainWindow");
#endif // DEBUG

	CUICharacterStateHUD* StateWindow = CreateUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	return true;
}

void CStage1::SetSoundBGM()
{
	if (!GetSceneResource()->FindSound("CastleCorrider")->IsPlay())
		GetSceneResource()->SoundPlay("CastleCorrider");
}
