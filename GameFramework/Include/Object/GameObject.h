#pragma once

#include "../Ref.h"
#include "../Resource/Texture.h"
#include "../Animation/Animation.h"
#include "../Collision/Collider.h"
#include "../UI/WidgetComponent.h"

class CGameObject :
	public CRef
{
	friend class CScene;

public:
	CGameObject();
	CGameObject(const CGameObject& obj);
	virtual ~CGameObject();

protected:
	class CScene* m_Scene;
	Vector2		m_PrevPos;
	Vector2		m_Pos;
	Vector2		m_RenderPos;
	Vector2		m_Size;
	Vector2		m_Pivot;
	Vector2		m_Velocity;
	Vector2		m_Offset;
	float		m_MoveSpeed;
	float		m_TimeScale;
	float		m_LifeTime;
	CSharedPtr<CTexture>	m_Texture;
	Vector2		m_ImageStart;
	std::list<CSharedPtr<CCollider>>	m_ColliderList;
	bool		m_CameraCull;
	bool		m_Start;
	EObject_Type	m_ObjType;
	bool			m_DamageEnable;
	std::list<CSharedPtr<CWidgetComponent>>	m_WidgetComponentList;
	bool			m_PhysicsSimulate;
	float			m_GravityAccel;
	bool			m_IsGround;
	float			m_FallTime;
	float			m_FallStartY;
	bool			m_Jump;
	bool			m_DoubleJump;
	float			m_JumpVelocity;
	bool			m_SuperJump;
	float			m_SuperJumpVelocity;
	bool			m_FloorCheck;
	bool			m_SideWallCheck;
	float			m_MaxJumpY;
	float			m_JumpKeyTime;
	bool			m_isCeiling;
	int				m_Exp;

public:
	void SetStart(bool Start)
	{
		m_Start = Start;
	}

	void SetSideWallCheck(bool Check)
	{
		m_SideWallCheck = Check;
	}

	void SetFloorCheck(bool Check)
	{
		m_FloorCheck = Check;
	}

	void SetLifeTime(float Time)
	{
		m_LifeTime = Time;
	}

	void SetGravityAccel(float Accel)
	{
		m_GravityAccel = Accel;
	}

	void SetPhysicsSimulate(bool Physics)
	{
		m_PhysicsSimulate = Physics;
	}

	void SetJumpVelocity(float JumpVelocity)
	{
		m_JumpVelocity = JumpVelocity;
	}

	void SetSuperJumpVelocity(float SuperJumpVelocity)
	{
		m_SuperJumpVelocity = SuperJumpVelocity;
	}

	void Jump()
	{		
		if (!m_Jump && !m_SuperJump)
		{
			m_Jump = true;
			m_IsGround = false;

			m_FallTime = 0.f;
			m_FallStartY = m_Pos.y;
		}
	}

	void SuperJump()
	{
		m_SuperJump = true;
		m_DoubleJump = false;
		m_Jump = false;
		m_IsGround = false;

		m_FallTime = 0.f;
		m_FallStartY = m_Pos.y;
	}

	void DoubleJump()
	{
		if (!m_DoubleJump)
		{
			if ((m_Jump || m_SuperJump))
			{
				m_Jump = true;
				m_DoubleJump = true;
				m_IsGround = false;

				if (!m_isCeiling)
				{
					m_FallTime = 0.f;
					m_FallStartY = m_Pos.y;
				}
			}

		}
	}

	void DamageEnable(bool Enable)
	{
		m_DamageEnable = Enable;
	}

	EObject_Type GetObjType()	const
	{
		return m_ObjType;
	}

	bool IsCull()	const
	{
		return m_CameraCull;
	}

	bool IsJumped()	const
	{
		if (m_Jump)
			return true;
		if (m_DoubleJump)
			return true;
		if (m_SuperJump)
			return true;

		return false;
	}

	void SetExp(int Exp)
	{
		m_Exp += Exp;
	}

public:
	CCollider* FindCollider(const std::string& Name);

protected:
	CAnimation* m_Animation;

public:
	void CreateAnimation();
	void AddAnimation(const std::string& SequenceName,
		bool Loop = true, float PlayTime = 1.f, float PlayScale = 1.f,
		bool Reverse = false);
	void SetAnimationPlayTime(const std::string& Name, float PlayTime);
	void SetAnimationPlayScale(const std::string& Name, float PlayScale);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	void ChangeAnimationEnd(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);
	bool CheckCurrentAnimationEnd();
	
	void SetAnimationReverse(const std::string& Name, bool Reverse);
	void SetAnimationLoop(const std::string& Name, bool Loop);

public:
	Vector2 GetOffset()	const
	{
		return m_Offset;
	}

	Vector2 GetPos()	const
	{
		return m_Pos;
	}

	Vector2 GetSize()	const
	{
		return m_Size;
	}

	Vector2 GetPivot()	const
	{
		return m_Pivot;
	}

	Vector2 GetVelocity()	const
	{
		return m_Velocity;
	}

	float GetMoveSpeed()	const
	{
		return m_MoveSpeed;
	}

	float GetMoveSpeedFrame()	const
	{
		return m_Velocity.Length();
	}

	float GetBottom()	const;


	Vector2 GetMiddleTop()	const
	{
		return m_Pos - m_Pivot * m_Size + Vector2(m_Size.x / 2.f, 0.f);
	}

	Vector2 GetMiddle() const
	{
		return m_Pos - Vector2(0.f,  3 * m_Size.y / 4.f);
	}

public:
	void SetScene(class CScene* Scene);

	CScene* GetScene()
	{
		return m_Scene;
	}

	void SetOffset(float x, float y)
	{
		m_Offset.x = x;
		m_Offset.y = y;
	}

	void SetOffset(const Vector2& Offset)
	{
		m_Offset = Offset;
	}

	void SetPos(float x, float y)
	{
		m_Pos.x = x;
		m_Pos.y = y;
	}

	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPrevPos(float x, float y)
	{
		m_PrevPos.x = x;
		m_PrevPos.y = y;
	}

	void SetPrevPos(const Vector2& Pos)
	{
		m_PrevPos = Pos;
	}

	void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}

	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}

	void SetPivot(float x, float y)
	{
		m_Pivot.x = x;
		m_Pivot.y = y;
	}

	void SetPivot(const Vector2& Pivot)
	{
		m_Pivot = Pivot;
	}

	void SetMoveSpeed(float Speed)
	{
		m_MoveSpeed = Speed;
	}

	void SetTimeScale(float TimeScale)
	{
		m_TimeScale = TimeScale;
	}

	void Move(const Vector2& Dir);
	void Move(const Vector2& Dir, float Speed);

	void SetImageStart(float x, float y)
	{
		m_ImageStart.x = x;
		m_ImageStart.y = y;
	}

	void SetImageStart(const Vector2& ImageStart)
	{
		m_ImageStart = ImageStart;
	}

public:
	void SetTexture(CTexture* Texture)
	{
		m_Texture = Texture;

		if (m_Texture)
		{
			m_Size.x = (float)m_Texture->GetWidth();
			m_Size.y = (float)m_Texture->GetHeight();
		}
	}

	void SetTexture(const std::string& Name);
	void SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureColorKey(unsigned char r,
		unsigned char g, unsigned char b, int Index = 0);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual CGameObject* Clone();
	virtual float SetDamage(float Damage);
	virtual float GetFloorTileY();
	virtual float GetFloorTileY(float PosY);

public:
	template <typename T>
	void SetAnimationEndNotify(const std::string& SequenceName, 
		T* Obj, void (T::* Func)())
	{
		m_Animation->SetEndNotify<T>(SequenceName, Obj, Func);
	}

	template <typename T>
	void AddAnimationNotify(const std::string& SequenceName, int Frame,
		T* Obj, void (T::* Func)())
	{
		m_Animation->AddNotify<T>(SequenceName, Frame, Obj, Func);
	}

public:
	template <typename T>
	T* AddCollider(const std::string& Name)
	{
		T* Collider = new T;

		Collider->SetName(Name);
		Collider->SetOwner(this);
		Collider->SetScene(m_Scene);

		if (!Collider->Init())
		{
			SAFE_DELETE(Collider);
			return nullptr;
		}

		m_ColliderList.push_back(Collider);

		return Collider;
	}

	CWidgetComponent* FindWidgetComponent(const std::string& Name)
	{
		auto	iter = m_WidgetComponentList.begin();
		auto	iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return *iter;
		}

		return nullptr;
	}

	CWidgetComponent* CreateWidgetComponent(const std::string& Name)
	{
		CWidgetComponent* Widget = FindWidgetComponent(Name);

		if (Widget)
			return nullptr;

		Widget = new CWidgetComponent;

		Widget->SetName(Name);
		Widget->SetOwner(this);
		Widget->SetScene(m_Scene);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		m_WidgetComponentList.push_back(Widget);

		return Widget;
	}
};

