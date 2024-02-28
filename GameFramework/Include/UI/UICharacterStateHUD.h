#pragma once

#include "UIWindow.h"
#include "ProgressBar.h"
#include "Button.h"
#include "UIImage.h"
#include "NumberWidget.h"

class CUICharacterStateHUD :
    public CUIWindow
{
	friend class CScene;

private:
	CUICharacterStateHUD();
	virtual ~CUICharacterStateHUD();

private:
	CSharedPtr<CProgressBar> m_HPBar;
	CSharedPtr<CProgressBar> m_MPBar;
	class CNumberWidget* m_HPWidget;

public:
	void SetHPPercent(float Percent)
	{
		m_HPBar->SetPercent(Percent);
	}
	void SetMPPercent(float Percent)
	{
		m_MPBar->SetPercent(Percent);
	}
	void SetHPNumber(int Number)
	{
		m_HPWidget->SetNumber(Number);
	}
	int GetHPNumber()
	{
		return m_HPWidget->GetNumber();
	}
	void SetHPNumberPos(int Number)
	{
		switch (Number)
		{
		case 3:
			m_HPWidget->SetPos(12.f, 25.f);
			break;
		case 2:
			m_HPWidget->SetPos(24.f, 25.f);
			break;
		case 1:
			m_HPWidget->SetPos(36.f, 25.f);
			break;
		default:
			break;
		}
	}


public:
	virtual bool Init();
};

