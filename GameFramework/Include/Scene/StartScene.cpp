
#include "StartScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIStart.h"

CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

bool CStartScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	CUIStart* StartWindow = CreateUIWindow<CUIStart>("StartWindow");

	return true;
}

void CStartScene::LoadAnimationSequence()
{
}

void CStartScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", false, "StartBGM", "StageSound/MainMenuBGM.wav");
	GetSceneResource()->SetVolume("BGM", 30);
	GetSceneResource()->SoundPlay("StartBGM");

	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "UI/UIMove.wav");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "UI/UISelect.wav");

	GetSceneResource()->SetVolume("UI", 100);
}
