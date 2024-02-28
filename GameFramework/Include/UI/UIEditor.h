#pragma once
#include "UIWindow.h"
class CUIEditor :
	public CUIWindow
{
	friend class CScene;

private:
	CUIEditor();
	virtual ~CUIEditor();

private:
	class CUIText* m_MousePosition;
	class CUIText* m_FPSTime;
	class CUIText* m_TileIndex;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

