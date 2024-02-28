
#include "UICharacterStateHUD.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"

CUICharacterStateHUD::CUICharacterStateHUD()
{
}

CUICharacterStateHUD::~CUICharacterStateHUD()
{
}

bool CUICharacterStateHUD::Init()
{
	SetPos(30.f, 20.f);

	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* Back = CreateWidget<CUIImage>("HPBarBack");

	Back->SetTexture("BarBack", TEXT("UI/BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);
	Back->SetZOrder(0);
	//Back->SetPos(30.f, 20.f);

	m_HPBar = CreateWidget<CProgressBar>("HPBar");

	m_HPBar->SetTexture("HPBar", TEXT("UI/HPBarDefault.bmp"));
	m_HPBar->SetZOrder(1);
	m_HPBar->SetPos(116.f, 24.f);

	m_MPBar = CreateWidget<CProgressBar>("MPBar");

	m_MPBar->SetTexture("MPBar", TEXT("UI/MPBarDefault.bmp"));
	m_MPBar->SetZOrder(1);
	m_MPBar->SetPos(116.f, 44.f);

	m_HPWidget = CreateWidget<CNumberWidget>("HPNumber");
	m_HPWidget->SetZOrder(2);
	m_HPWidget->SetPos(36.f, 25.f);

	std::vector<std::wstring>	vecNumberFileName;

	for (int i = 0; i < 10; ++i)
	{
		TCHAR	FileName[256] = {};

		wsprintf(FileName, TEXT("Number/Blue/%d.bmp"), i);

		vecNumberFileName.push_back(FileName);
	}

	m_HPWidget->SetTexture("HPNumber", vecNumberFileName);

	for (int i = 0; i < 10; ++i)
	{
		m_HPWidget->SetTextureColorKey(255, 0, 255, i);
	}

	m_HPWidget->SetSize(23.f, 32.f);

	return true;
}
