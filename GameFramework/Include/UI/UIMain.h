#pragma once
#include "UIWindow.h"
class CUIMain :
    public CUIWindow
{
	friend class CScene;

private:
	CUIMain();
	virtual ~CUIMain();

private:
	class CUIText* m_Text;
	class CUIText* m_PlayerPosText;
	class CUIText* m_PlayerSizeText;
	class CUIText* m_FPSTime;
	class CUIText* m_MousePosition;
	class CUIText* m_TileIndex;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

