
#include "UIMain.h"
#include "Button.h"
#include "UIImage.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "UIText.h"
#include "NumberWidget.h"
#include "../Input.h"
#include "../Scene/Camera.h"

CUIMain::CUIMain()
{
}

CUIMain::~CUIMain()
{
}

bool CUIMain::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	Vector2 UITextPos = Vector2(20.f, 130.f);

	m_PlayerPosText = CreateWidget<CUIText>("PlayerPos");

	m_PlayerPosText->SetPos(UITextPos);
	UITextPos.y += 20.f;
	m_PlayerPosText->SetTextColor(255, 0, 255);
	m_PlayerPosText->SetZOrder(1);

	m_PlayerSizeText = CreateWidget<CUIText>("PlayerSize");

	m_PlayerSizeText->SetPos(UITextPos);
	UITextPos.y += 20.f;
	m_PlayerSizeText->SetTextColor(255, 0, 255);
	m_PlayerSizeText->SetZOrder(1);

	m_FPSTime = CreateWidget<CUIText>("CurrentFPS");

	m_FPSTime->SetPos(UITextPos);
	UITextPos.y += 20.f;
	m_FPSTime->SetTextColor(255, 0, 255);
	m_FPSTime->SetZOrder(1);

	m_MousePosition = CreateWidget<CUIText>("CurrentMousePos");

	m_MousePosition->SetPos(UITextPos);
	UITextPos.y += 20.f;
	m_MousePosition->SetTextColor(255, 0, 255);
	m_MousePosition->SetZOrder(1);

	m_TileIndex = CreateWidget<CUIText>("TileIndex");

	m_TileIndex->SetPos(UITextPos);
	UITextPos.y += 20.f;
	m_TileIndex->SetTextColor(255, 0, 255);
	m_TileIndex->SetZOrder(1);

	return true;
}

void CUIMain::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
	
	bool CurrentVisibility = CGameManager::GetInst()->IsRenderDebugText();
	m_PlayerPosText->SetVisibility(CurrentVisibility);
	m_PlayerSizeText->SetVisibility(CurrentVisibility);
	m_FPSTime->SetVisibility(CurrentVisibility);
	m_MousePosition->SetVisibility(CurrentVisibility);
	m_TileIndex->SetVisibility(CurrentVisibility);

	if (!CurrentVisibility)
		return;

	if (m_Scene->GetPlayer())
	{
		char	Buff[128] = {};
		TCHAR	BuffText[128] = {};
		sprintf_s(Buff, "플레이어 위치 X : %8.2f\t Y : %8.2f", m_Scene->GetPlayer()->GetPos().x, m_Scene->GetPlayer()->GetPos().y);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Buff, (int)strlen(Buff), BuffText, 128);

		m_PlayerPosText->SetText(BuffText);

		memset(Buff, 0, sizeof(char) * 128);
		memset(BuffText, 0, sizeof(TCHAR) * 128);

		sprintf_s(Buff, "플레이어 크기 X : %8.2f\t Y : %8.2f", m_Scene->GetPlayer()->GetSize().x, m_Scene->GetPlayer()->GetSize().y);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Buff, (int)strlen(Buff), BuffText, 128);

		m_PlayerSizeText->SetText(BuffText);

		memset(Buff, 0, sizeof(char) * 128);
		memset(BuffText, 0, sizeof(TCHAR) * 128);

		sprintf_s(Buff, "현재 프레임 : %8.2f", CGameManager::GetInst()->GetFPS());
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Buff, (int)strlen(Buff), BuffText, 128);

		m_FPSTime->SetText(BuffText);

		memset(Buff, 0, sizeof(char) * 128);
		memset(BuffText, 0, sizeof(TCHAR) * 128);

		Vector2 MousePos = CInput::GetInst()->GetMousePos();

		CCamera* Camera = m_Scene->GetCamera();

		if (MousePos.x < 0.f || MousePos.x > Camera->GetResolution().x ||
			MousePos.y < 0.f || MousePos.y > Camera->GetResolution().y)
			MousePos = Vector2(0.f, 0.f);

		MousePos += Camera->GetPos();

		sprintf_s(Buff, "마우스 위치 X : %8.2f\t Y : %8.2f", MousePos.x, MousePos.y);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Buff, (int)strlen(Buff), BuffText, 128);

		m_MousePosition->SetText(BuffText);

		memset(Buff, 0, sizeof(char) * 128);
		memset(BuffText, 0, sizeof(TCHAR) * 128);

		int TileIndexX = m_Scene->GetTileMap()->GetTileIndexX(MousePos.x);
		int TileIndexY = m_Scene->GetTileMap()->GetTileIndexY(MousePos.y);
		sprintf_s(Buff, "타일 인덱스 X : %d\t Y : %d", TileIndexX, TileIndexY);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Buff, (int)strlen(Buff), BuffText, 128);

		m_TileIndex->SetText(BuffText);
	}
}
