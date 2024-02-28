#pragma once
#include "GameObject.h"
class CLevelUpFont :
    public CGameObject
{
	friend class CScene;

protected:
	CLevelUpFont();
	CLevelUpFont(const CLevelUpFont& obj);
	virtual ~CLevelUpFont();

private:

	float	m_DirX;
	float	m_SpeedX;

public:

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CLevelUpFont* Clone();
};

