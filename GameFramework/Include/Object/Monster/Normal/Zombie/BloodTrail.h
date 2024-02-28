#pragma once
#include "../../../GameObject.h"
class CBloodTrail :
    public CGameObject
{
	friend class CScene;

protected:
	CBloodTrail();
	CBloodTrail(const CBloodTrail& obj);
	virtual ~CBloodTrail();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CBloodTrail* Clone();
};

