
#include "UIEditor.h"
#include "Button.h"
#include "UIImage.h"
#include "../GameManager.h"
#include "../Scene/EditorScene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "UIText.h"
#include "NumberWidget.h"
#include "../Input.h"
#include "../Scene/Camera.h"

CUIEditor::CUIEditor()
{
}

CUIEditor::~CUIEditor()
{
}

bool CUIEditor::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	Vector2 UITextPos = Vector2(20.f, 130.f);

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

void CUIEditor::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);

	if (m_Scene->GetTileMap())
	{
		Vector2 MousePos = CInput::GetInst()->GetMousePos();

		CCamera* Camera = m_Scene->GetCamera();

		if (MousePos.x < 0.f || MousePos.x > Camera->GetResolution().x ||
			MousePos.y < 0.f || MousePos.y > Camera->GetResolution().y)
			MousePos = Vector2(0.f, 0.f);

		MousePos += Camera->GetPos();

		char	Buff[128] = {};
		TCHAR	BuffText[128] = {};
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

		memset(Buff, 0, sizeof(char) * 128);
		memset(BuffText, 0, sizeof(TCHAR) * 128);

		sprintf_s(Buff, "현재 프레임 : %8.2f", CGameManager::GetInst()->GetFPS());
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Buff, (int)strlen(Buff), BuffText, 128);

		m_FPSTime->SetText(BuffText);
	}
}
