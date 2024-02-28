
#include "UIStart.h"
#include "Button.h"
#include "UIImage.h"
#include "../GameManager.h"
#include "../Scene/Stage1.h"
#include "../Scene/EditorScene.h"
#include "../Scene/SceneManager.h"
#include "UIText.h"
#include "../Input.h"

CUIStart::CUIStart() :
	IsOpeningEnd(false)
{
}

CUIStart::~CUIStart()
{
}

void CUIStart::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);

	CUIImage* Opening = FindWidget<CUIImage>("Opening");

	if (CInput::GetInst()->GetMouseDown())
		Opening->SetIsEnd(true);

	if (Opening->GetIsEnd() && !IsOpeningEnd)
	{
		IsOpeningEnd = true;

		Resolution	RS = CGameManager::GetInst()->GetResolution();

		CUIImage* Back = CreateWidget<CUIImage>("Back");

		Back->SetTexture("GameBack", TEXT("UI/GameBack.bmp"));

		CButton* Button = CreateWidget<CButton>("StartButton");

		Button->SetTexture("StartButton", TEXT("UI/GameStart.bmp"));
		Button->SetTextureColorKey(255, 0, 255);
		Button->SetPos(RS.Width / 10.f * 4.f + 5.f, RS.Height - 200.f);
		Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(325.f, 38.f));
		Button->SetFrameData(EButton_State::MouseOn, Vector2(325.f, 0.f), Vector2(325.f, 38.f));
		Button->SetFrameData(EButton_State::Click, Vector2(325.f, 0.f), Vector2(325.f, 38.f));
		Button->SetFrameData(EButton_State::Disable, Vector2(325.f, 0.f), Vector2(325.f, 38.f));
		Button->SetMouseOnSound("ButtonMouseOn");
		Button->SetClickSound("ButtonClick");
		Button->SetZOrder(1);

		Button->SetClickCallback<CUIStart>(this, &CUIStart::StartClick);
		//============================================================================================
		Button = CreateWidget<CButton>("EditorButton");

		Button->SetTexture("EditorButton", TEXT("UI/Editor.bmp"));
		Button->SetTextureColorKey(255, 0, 255);
		Button->SetPos(RS.Width / 10.f * 4.f, RS.Height - 150.f);
		Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(325.f, 38.f));
		Button->SetFrameData(EButton_State::MouseOn, Vector2(325.f, 0.f), Vector2(325.f, 38.f));
		Button->SetFrameData(EButton_State::Click, Vector2(325.f, 0.f), Vector2(325.f, 38.f));
		Button->SetFrameData(EButton_State::Disable, Vector2(325.f, 0.f), Vector2(325.f, 38.f));
		Button->SetMouseOnSound("ButtonMouseOn");
		Button->SetClickSound("ButtonClick");
		Button->SetZOrder(1);

		Button->SetClickCallback<CUIStart>(this, &CUIStart::EditorClick);

		//============================================================================================
		Button = CreateWidget<CButton>("ExitButton");

		Button->SetTexture("ExitButton", TEXT("UI/Exit.bmp"));
		Button->SetTextureColorKey(255, 0, 255);
		Button->SetPos(RS.Width / 10.f * 4.f + 97.5f, RS.Height - 100.f);
		Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(130.f, 38.f));
		Button->SetFrameData(EButton_State::MouseOn, Vector2(130.f, 0.f), Vector2(130.f, 38.f));
		Button->SetFrameData(EButton_State::Click, Vector2(130.f, 0.f), Vector2(130.f, 38.f));
		Button->SetFrameData(EButton_State::Disable, Vector2(130.f, 0.f), Vector2(130.f, 38.f));
		Button->SetMouseOnSound("ButtonMouseOn");
		Button->SetClickSound("ButtonClick");
		Button->SetZOrder(1);

		Button->SetClickCallback<CUIStart>(this, &CUIStart::ExitClick);
	}
}

bool CUIStart::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* Opening = CreateWidget<CUIImage>("Opening");

	Opening->SetLoop(false);

	Opening->SetTexture("StartSceneOpening", TEXT("UI/StartSceneOpening.bmp"));

	Opening->SetPlayTime(15.f);

	for (int i = 0; i < 37; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			Opening->AddFrameData(Vector2(j * 1280.f, i * 720.f), Vector2(1280.f, 720.f));
		}
	}

	return true;
}

void CUIStart::StartClick()
{
	CSceneManager::GetInst()->CreateScene<CStage1>();
}

void CUIStart::ExitClick()
{
	CGameManager::GetInst()->Exit();
}

void CUIStart::EditorClick()
{
	CSceneManager::GetInst()->CreateScene<CEditorScene>();
}
