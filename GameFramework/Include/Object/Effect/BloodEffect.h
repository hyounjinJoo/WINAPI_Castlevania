#pragma once
#include "../GameObject.h"
class CBloodEffect :
    public CGameObject
{
	friend class CScene;

protected:
	CBloodEffect();
	CBloodEffect(const CBloodEffect& obj);
	virtual ~CBloodEffect();

	float m_CreateY;
	Vector2		m_Dir;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CBloodEffect* Clone();
};

