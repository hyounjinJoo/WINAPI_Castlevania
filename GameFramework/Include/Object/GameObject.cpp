
#include "GameObject.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneCollision.h"
#include "../Resource/AnimationSequence.h"
#include "../Scene/Camera.h"
#include "../Map/TileMap.h"

CGameObject::CGameObject() :
	m_Scene(nullptr),
	m_MoveSpeed(200.f),
	m_TimeScale(1.f),
	m_Animation(nullptr),
	m_CameraCull(false),
	m_Start(false),
	m_DamageEnable(true),
	m_ObjType(EObject_Type::GameObject),
	m_PhysicsSimulate(false),
	m_IsGround(true),
	m_FallTime(0.f),
	m_FallStartY(0.f),
	m_Jump(false),
	m_JumpVelocity(0.f),
	m_SuperJump(false),
	m_SuperJumpVelocity(0.f),
	m_DoubleJump(false),
	m_GravityAccel(14.f),
	m_LifeTime(0.f),
	m_FloorCheck(false),
	m_SideWallCheck(false),
	m_MaxJumpY(0.f),
	m_isCeiling(false),
	m_Exp(0)
{
	m_PrevPos.y = -1.f;
}

CGameObject::CGameObject(const CGameObject& obj)	:
	CRef(obj)
{
	m_PrevPos.y = -1.f;
	m_FloorCheck = obj.m_FloorCheck;
	m_GravityAccel = obj.m_GravityAccel;
	m_PhysicsSimulate = obj.m_PhysicsSimulate;
	m_IsGround = obj.m_IsGround;
	m_FallTime = obj.m_FallTime;
	m_FallStartY = obj.m_FallStartY;
	m_Jump = obj.m_Jump;
	m_JumpVelocity = obj.m_JumpVelocity;
	m_SuperJump = obj.m_SuperJump;
	m_SuperJumpVelocity = obj.m_SuperJumpVelocity;
	m_DoubleJump = obj.m_DoubleJump;
	m_isCeiling = obj.m_isCeiling;
	m_DamageEnable = obj.m_DamageEnable;
	m_ObjType = obj.m_ObjType;
	m_Exp = obj.m_Exp;

	m_Start = false;

	m_Scene = obj.m_Scene;

	if (obj.m_Animation)
		m_Animation = obj.m_Animation->Clone();

	m_Animation->m_Owner = this;

	m_ColliderList.clear();

	{
		auto	iter = obj.m_ColliderList.begin();
		auto	iterEnd = obj.m_ColliderList.end();

		for (; iter != iterEnd; ++iter)
		{
			CCollider* Collider = (*iter)->Clone();

			Collider->SetOwner(this);
			Collider->SetScene(m_Scene);

			m_ColliderList.push_back(Collider);
		}
	}
	
	m_WidgetComponentList.clear();

	{
		auto	iter = obj.m_WidgetComponentList.begin();
		auto	iterEnd = obj.m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			CWidgetComponent* Widget = (*iter)->Clone();
			Widget->SetOwner(this);
			Widget->SetScene(m_Scene);

			m_WidgetComponentList.push_back(Widget);
		}
	}

	m_PrevPos = obj.m_PrevPos;
	m_Pos = obj.m_Pos;
	m_Size = obj.m_Size;
	m_Pivot = obj.m_Pivot;
	m_Velocity = obj.m_Velocity;
	m_Offset = obj.m_Offset;
	m_MoveSpeed = obj.m_MoveSpeed;
	m_TimeScale = obj.m_TimeScale;
	m_Texture = obj.m_Texture;
	m_ImageStart = obj.m_ImageStart;
}

CGameObject::~CGameObject()
{
	SAFE_DELETE(m_Animation);

	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->ClearCollisionList();
	}
}

CCollider* CGameObject::FindCollider(const std::string& Name)
{
	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}

void CGameObject::CreateAnimation()
{
	if (!m_Animation)
	{
		m_Animation = new CAnimation;

		m_Animation->m_Owner = this;
		m_Animation->m_Scene = m_Scene;
	}
}

void CGameObject::AddAnimation(const std::string& SequenceName,
	bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	if (!m_Animation)
	{
		m_Animation = new CAnimation;

		m_Animation->m_Owner = this;
		m_Animation->m_Scene = m_Scene;
	}

	m_Animation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CGameObject::SetAnimationPlayTime(const std::string& Name, 
	float PlayTime)
{
	if (!m_Animation)
		return;

	m_Animation->SetPlayTime(Name, PlayTime);
}

void CGameObject::SetAnimationPlayScale(const std::string& Name, 
	float PlayScale)
{
	if (!m_Animation)
		return;

	m_Animation->SetPlayScale(Name, PlayScale);
}

void CGameObject::SetCurrentAnimation(const std::string& Name)
{
	if (!m_Animation)
		return;

	m_Animation->SetCurrentAnimation(Name);
}

void CGameObject::ChangeAnimation(const std::string& Name)
{
	if (!m_Animation)
		return;

	m_Animation->ChangeAnimation(Name);
}

void CGameObject::ChangeAnimationEnd(const std::string& Name)
{
	if (!m_Animation)
		return;

	m_Animation->ChangeAnimationEnd(Name);
}

bool CGameObject::CheckCurrentAnimation(const std::string& Name)
{
	return m_Animation->CheckCurrentAnimation(Name);
}

bool CGameObject::CheckCurrentAnimationEnd()
{
	return m_Animation->CheckCurrentAnimationEnd();
}

void CGameObject::SetAnimationReverse(const std::string& Name, bool Reverse)
{
	if (!m_Animation)
		return;

	m_Animation->SetReverse(Name, Reverse);
}

void CGameObject::SetAnimationLoop(const std::string& Name, bool Loop)
{
	if (!m_Animation)
		return;

	m_Animation->SetLoop(Name, Loop);
}

float CGameObject::GetBottom() const
{
	Vector2	Size = m_Size;

	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Size = FrameData.Size;
	}

	return m_Pos.y + (1.f - m_Pivot.y) * Size.y + m_Offset.y;
}

void CGameObject::SetScene(CScene* Scene)
{
	m_Scene = Scene;

	if (m_Animation)
		m_Animation->m_Scene = Scene;

	if (m_ColliderList.size() > 0)
	{
		auto iter = m_ColliderList.begin();
		auto iterEnd = m_ColliderList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->SetScene(Scene);
		}
	}
}

void CGameObject::Move(const Vector2& Dir)
{
	Vector2	CurrentMove = Dir * m_MoveSpeed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Velocity	+= CurrentMove;
	m_Pos += CurrentMove;
}

void CGameObject::Move(const Vector2& Dir, float Speed)
{
	Vector2	CurrentMove = Dir * Speed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Velocity += CurrentMove;
	m_Pos += CurrentMove;
}

void CGameObject::SetTexture(const std::string& Name)
{
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName, 
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b, Index);
}

void CGameObject::Start()
{
	m_Start = true;

	// 나중에 땅에 닿으면 갱신한다.
	m_FallStartY = m_Pos.y;
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	if (!m_Start)
	{
		Start();
	}

	if (m_LifeTime > 0.f)
	{
		m_LifeTime -= DeltaTime;

		if (m_LifeTime <= 0.f)
		{
			Destroy();
			return;
		}
	}

	if (m_Animation)
		m_Animation->Update(DeltaTime);

	// 중력을 적용한다.
	if (!m_IsGround && m_PhysicsSimulate)
	{
		// 떨어지는 시간을 누적시켜준다.
		m_FallTime += DeltaTime * m_GravityAccel;

		// 9.8 m/s^2
		// t초 후 y값
		// V : 속도    A : 가속도    G : 중력
		// y = V * A - 0.5f * G * t * t
		// 0 = -0.5GA^2 VA - y
		// (-b +- 루트(b^2 - 4ac) / 2a
		float	Velocity = 0.f;

		if (m_Jump)
		{
			m_GravityAccel = 18.f;
			Velocity = m_JumpVelocity * m_FallTime;
		}
		if (m_SuperJump)
		{
			/*if (m_DoubleJump)
			{
				m_GravityAccel = 18.f;
				Velocity = m_JumpVelocity * m_FallTime;
			}
			else
			{
				Velocity = m_SuperJumpVelocity * m_FallTime;
				m_GravityAccel = 23.f;
			}*/
			Velocity = m_SuperJumpVelocity * m_FallTime;
			m_GravityAccel = 23.f;
		}
		if (m_isCeiling)
		{
			Velocity = -m_FallTime;
		}

		float	SaveY = m_Pos.y;

		m_Pos.y = m_FallStartY - (Velocity - 0.5f * GRAVITY * m_FallTime * m_FallTime);

		if (m_Pos.y - m_PrevPos.y >= 0.f)
			m_GravityAccel = 14.f;
	}


	{
		auto	iter = m_ColliderList.begin();
		auto	iterEnd = m_ColliderList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
				continue;
			}

			else if ((*iter)->GetEnable())
			{
				(*iter)->Update(DeltaTime);
			}

			++iter;
		}
	}

	{
		auto	iter = m_WidgetComponentList.begin();
		auto	iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Update(DeltaTime);
		}
	}
}

void CGameObject::PostUpdate(float DeltaTime)
{
	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		//m_Size = FrameData.Size;
	}

	{
		auto	iter = m_ColliderList.begin();
		auto	iterEnd = m_ColliderList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
				continue;
			}

			else if ((*iter)->GetEnable())
			{
				(*iter)->PostUpdate(DeltaTime);
			}

			++iter;
		}
	}



	{
		auto	iter = m_WidgetComponentList.begin();
		auto	iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->PostUpdate(DeltaTime);
		}
	}

	float	DirX = m_Pos.x - m_PrevPos.x;
	
	// 타일맵 가져옴
	CTileMap* TileMap = m_Scene->GetTileMap();
	// 사각형의 왼쪽 상단은 현재 위치 - 피봇*사이즈값.
	Vector2	LT = m_Pos - m_Pivot * m_Size;
	// 오른쪽 하단은 LT+물체의 크기만큼
	Vector2	RB = LT + m_Size;
	// 왼쪽 하단은 LT + m_Size의 y 크기만큼
	Vector2 LB = LT + Vector2(0.f, m_Size.y);
	// 오브젝트 하단 정가운데.
	Vector2 MB = (RB + LB) / 2.f;
	// 방향 벡터가 0이 아니다 && m_SideWallCheck인 타일인 경우
	if (DirX != 0.f && m_SideWallCheck)
	{
		// 이전 LT는 이전위치 - (피봇*물체크기)
		Vector2	PrevLT = m_PrevPos - m_Pivot * m_Size;
		// 이전 RB는 이전LT + 물체크기
		Vector2	PrevRB = PrevLT + m_Size;
		// 이전 LB는 이전LT + 물체크기의 y 크기만큼
		Vector2 PrevLB = PrevLT + Vector2(0.f, m_Size.y);		

		// LT 인덱스 ~ RB인덱스를 가져옴
		int	LTIndexX, LTIndexY, RBIndexX, RBIndexY;

		// 가장자리 충돌 false로 초기화
		bool	SideCollision = false;

		// 오른쪽으로 이동할때
		if (DirX > 0.f)
		{
			Vector2	resultLT, resultRB;

			resultLT = LT;
			resultRB = RB;

			resultLT.x = PrevRB.x;
			resultLT.y = LT.y < PrevLT.y ? LT.y : PrevLT.y;

			resultRB.y = RB.y > PrevRB.y ? RB.y : PrevRB.y;

			LTIndexX = TileMap->GetOriginTileIndexX(resultLT.x);
			LTIndexY = TileMap->GetOriginTileIndexY(resultLT.y);
			RBIndexX = TileMap->GetOriginTileIndexX(resultRB.x);
			RBIndexY = TileMap->GetOriginTileIndexY(resultRB.y);

			LTIndexX = LTIndexX < 0 ? 0 : LTIndexX;
			LTIndexY = LTIndexY < 0 ? 0 : LTIndexY;

			RBIndexX = RBIndexX > TileMap->GetTileCountX() - 1 ? TileMap->GetTileCountX() - 1 : RBIndexX;
			RBIndexY = RBIndexY > TileMap->GetTileCountY() - 1 ? TileMap->GetTileCountY() - 1 : RBIndexY;

			for (int i = LTIndexY; i <= RBIndexY; ++i)
			{
				for (int j = LTIndexX; j <= RBIndexX; ++j)
				{
					int	Index = i * TileMap->GetTileCountX() + j;

					if (TileMap->GetTile(Index)->GetTileOption() !=
						ETileOption::Wall ||
						!TileMap->GetTile(Index)->GetSideCollision())
						continue;

					Vector2	TilePos = TileMap->GetTile(Index)->GetPos();
					Vector2	TileSize = TileMap->GetTile(Index)->GetSize();

					if (TilePos.y - 0.001f <= RB.y && RB.y <= TilePos.y + 0.001f)
						continue;

					if (RB.x - TilePos.x > 8.f)
						continue;

					// 렉트충돌
					if (LT.x <= TilePos.x + TileSize.x &&
						LT.y <= TilePos.y + TileSize.y &&
						RB.x >= TilePos.x && RB.y >= TilePos.y)
					{
						SideCollision = true;

						float	MoveX = TilePos.x - RB.x - 0.001f;

						//Move(Vector2(MoveX, 0.f));
						m_Pos.x += MoveX;
						m_Velocity.x += MoveX;
					}
				}

				if (SideCollision)
					break;
			}			
		}

		// 왼쪽으로 이동할때
		else
		{
			Vector2	resultLT, resultRB;

			resultLT = LT;
			resultRB = RB;

			resultLT.y = LT.y < PrevLT.y ? LT.y : PrevLT.y;
			resultRB.y = RB.y > PrevRB.y ? RB.y : PrevRB.y;

			resultRB.x = PrevLT.x;

			LTIndexX = TileMap->GetOriginTileIndexX(resultLT.x);
			LTIndexY = TileMap->GetOriginTileIndexY(resultLT.y);
			RBIndexX = TileMap->GetOriginTileIndexX(resultRB.x);
			RBIndexY = TileMap->GetOriginTileIndexY(resultRB.y);

			LTIndexX = LTIndexX < 0 ? 0 : LTIndexX;
			LTIndexY = LTIndexY < 0 ? 0 : LTIndexY;

			RBIndexX = RBIndexX > TileMap->GetTileCountX() - 1 ? TileMap->GetTileCountX() - 1 : RBIndexX;
			RBIndexY = RBIndexY > TileMap->GetTileCountY() - 1 ? TileMap->GetTileCountY() - 1 : RBIndexY;

			for (int i = LTIndexY; i <= RBIndexY; ++i)
			{
				for (int j = RBIndexX; j >= LTIndexX; --j)
				{
					int	Index = i * TileMap->GetTileCountX() + j;

					if (TileMap->GetTile(Index)->GetTileOption() !=
						ETileOption::Wall ||
						!TileMap->GetTile(Index)->GetSideCollision())
						continue;

					Vector2	TilePos = TileMap->GetTile(Index)->GetPos();
					Vector2	TileSize = TileMap->GetTile(Index)->GetSize();

					if (TilePos.y - 0.001f <= RB.y && RB.y <= TilePos.y + 0.001f)
						continue;

					if (TilePos.x + TileSize.x - LT.x > 8.f)
						continue;

					// 렉트충돌
					if (LT.x <= TilePos.x + TileSize.x &&
						LT.y <= TilePos.y + TileSize.y &&
						RB.x >= TilePos.x && RB.y >= TilePos.y)
					{
						SideCollision = true;

						float	MoveX = TilePos.x + TileSize.x - LT.x + 0.001f;

						//Move(Vector2(MoveX, 0.f));
						m_Pos.x += MoveX;
						m_Velocity.x += MoveX;
					}
				}

				if (SideCollision)
					break;
			}			
		}		
	}
	if (m_PhysicsSimulate)
	{
		int LineCount = TileMap->GetLineCount();

		for (int Index = 0; Index < LineCount; ++Index)
		{
			// 라인의 왼쪽 점과 오른쪽 점 가져옴
			Vector2 LeftPoint = TileMap->GetLine(Index)->GetLeftPoint();
			Vector2 RightPoint = TileMap->GetLine(Index)->GetRightPoint();
			bool IsLeftUp = LeftPoint.y < RightPoint.y ? true : false;
			// 어떤 형태의 경사인지에 따라 물체의 좌측, 우측하단점을 검사할 지 결정
			Vector2 Target = IsLeftUp ? LB : RB/*MB*/;

			float LineTopY = IsLeftUp ? LeftPoint.y : RightPoint.y;
			float LineBottomY = IsLeftUp ? RightPoint.y : LeftPoint.y;

			//if(LeftPoint.x - m_Size.x <= Target.x  )
			if (LeftPoint.x <= Target.x && Target.x <= RightPoint.x
				&& LineTopY - 0.001f <= Target.y && Target.y <= LineBottomY + 0.001f)
			{
				float	OriginY = TileMap->GetLine(Index)->GetY(Target.x);
				float	MoveY = TileMap->GetLine(Index)->GetY(Target.x) - Target.y;

				if (m_Jump || m_SuperJump)
				{
					if (m_MaxJumpY >= Target.y)
					{
						m_MaxJumpY = Target.y;

						break;
					}
					else
					{
						if (OriginY - 3.0f <= Target.y
							&& Target.y <= OriginY + 3.0f)
						{
							m_Jump = false;
							m_SuperJump = false;
							m_DoubleJump = false;
							m_IsGround = true;
							m_FallTime = 0.f;
							m_Pos.y = OriginY;
						}
						else
						{
							break;
						}
					}
				}
				m_MaxJumpY = Target.y;

				// 오른쪽 이동인 경우
				if (DirX > 0.f)
				{
					// 좌상-우하단 경사인 경우
					if (IsLeftUp)
					{
						if (Target.y >= LineBottomY - 5.0f)
						{
							m_Pos.y = LineBottomY;
							m_Velocity.y = 0.f;
						}

						else
						{
							m_Pos.y += MoveY;
							m_Velocity.y += MoveY;
						}
					}
					// 좌하-우상단 경사인 경우
					else
					{
						if (Target.y <= LineTopY + 3.5f)
						{
							m_Pos.x += 5.f;
							m_Pos.y = RightPoint.y;
							m_Velocity.y = 0.f;
						}
						else
						{
							m_Pos.y += MoveY;
							m_Velocity.y += MoveY;
						}
					}

				}
				else if (DirX < 0.f)
				{
					// 좌상-우하단 경사인 경우
					if (IsLeftUp)
					{
						if (Target.y <= LineTopY + 3.5f)
						{
							m_Pos.x -= 5.f;
							m_Pos.y = LeftPoint.y;
							m_Velocity.y = 0.f;
						}
						else
						{
							m_Pos.y += MoveY;
							m_Velocity.y += MoveY;
						}
					}
					// 좌하-우상단 경사인 경우
					else
					{
						if (Target.y >= LineBottomY - 5.f)
						{
							m_Pos.y = LineBottomY;
							m_Velocity.y = 0.f;
						}

						else
						{
							m_Pos.y += MoveY;
							m_Velocity.y += MoveY;
						}
					}

				}

				else
				{
					m_Pos.y = OriginY;
					m_Velocity.y = 0.f;
				}

				m_IsGround = true;
			}
		}
	}

	// 천장에 대한 처리
	if (m_PhysicsSimulate && m_Pos.y - m_PrevPos.y < 0.f)
	{
		int HeadTopIndexX = TileMap->GetOriginTileIndexX(m_Pos.x) - 1;
		int HeadTopIndexY = TileMap->GetOriginTileIndexX(m_Pos.y - m_Size.y);

		if (HeadTopIndexX < 0)
			HeadTopIndexX = 0;

		if (HeadTopIndexY < 0)
			HeadTopIndexY = 0;

		if (HeadTopIndexX >= TileMap->GetTileCountX())
			HeadTopIndexX = TileMap->GetTileCountX() - 1;

		if (HeadTopIndexY >= TileMap->GetTileCountY())
			HeadTopIndexY = TileMap->GetTileCountY() - 1;

		bool	Check = false;
		for (int i = 0; i < 3; ++i)
		{
			if (TileMap->GetTile(HeadTopIndexX + i, HeadTopIndexY)->GetTileOption() == ETileOption::Wall)
			{
				if (TileMap->GetTile(HeadTopIndexX + i, HeadTopIndexY)->GetSideCollision())
				{
					m_IsGround = false;
					m_isCeiling = true;
					m_Pos.y = TileMap->GetTile(HeadTopIndexX + i, HeadTopIndexY)->GetPos().y/* + TileMap->GetTile(HeadTopIndexX, HeadTopIndexY)->GetSize().y*/ + m_Size.y;
					m_FallStartY = m_Pos.y;
					m_FallTime = 0.f;
				}
			}
		}
	}
	//if (!m_IsGround && m_Pos.y - m_PrevPos.y > 0.f)
	if (m_PhysicsSimulate && m_Pos.y - m_PrevPos.y >= 0.f)
	{
		// y값이 같을 경우는 땅 위를 걷고 있는것으로 판단한다.
		// 그러므로 이 경우에는 바닥을 체크하고 위로 올라가는 경우에는
		// 

		CTileMap* TileMap = m_Scene->GetTileMap();

		float	PrevBottom = m_PrevPos.y + (1.f - m_Pivot.y) * m_Size.y;
		float	CurBottom = m_Pos.y + (1.f - m_Pivot.y) * m_Size.y;
		float	PrevLeft = m_PrevPos.x - m_Pivot.x * m_Size.x;
		float	CurLeft = m_Pos.x - m_Pivot.x * m_Size.x;
		float	PrevRight = PrevLeft + m_Size.x;
		float	CurRight = CurLeft + m_Size.x;

		float	resultLeft = PrevLeft < CurLeft ? PrevLeft : CurLeft;
		float	resultRight = PrevRight > CurRight ? PrevRight : CurRight;
		float	resultTop = PrevBottom < CurBottom ? PrevBottom : CurBottom;
		float	resultBottom = PrevBottom > CurBottom ? PrevBottom : CurBottom;

		// 이전 위치와 현재 위치의 타일 인덱스를 구해온다.
		int	LeftIndex, TopIndex, RightIndex, BottomIndex;

		LeftIndex = TileMap->GetOriginTileIndexX(resultLeft);
		TopIndex = TileMap->GetOriginTileIndexY(resultTop);
		RightIndex = TileMap->GetOriginTileIndexX(resultRight);
		BottomIndex = TileMap->GetOriginTileIndexY(resultBottom);

		if (LeftIndex < 0)
			LeftIndex = 0;

		if (TopIndex < 0)
			TopIndex = 0;

		if (RightIndex >= TileMap->GetTileCountX())
			RightIndex = TileMap->GetTileCountX() - 1;

		if (BottomIndex >= TileMap->GetTileCountY())
			BottomIndex = TileMap->GetTileCountY() - 1;

		bool	Check = false;

		for (int i = TopIndex; i <= BottomIndex; ++i)
		{
			for (int j = LeftIndex; j <= RightIndex; ++j)
			{
				// 이전위치의 Bottom이 타일의 Top보다 클 경우 무시한다.
				if (TileMap->GetTile(j, i)->GetPos().y < PrevBottom)
					continue;

				if (TileMap->GetTile(j, i)->GetTileOption() == ETileOption::Wall)
				{
					Check = true;
					m_FallTime = 0.f;
					m_Pos.y = TileMap->GetTile(j, i)->GetPos().y - (1.f - m_Pivot.y) * m_Size.y;
					m_IsGround = true;
					m_Jump = false;
					m_SuperJump = false;
					m_DoubleJump = false;
					m_isCeiling = false;
					break;
				}
			}

			if (Check)
				break;
		}

		if (!Check)
		{
			if (m_IsGround)
			{
				m_FallTime = 0.f;
				m_FallStartY = m_Pos.y;
				m_isCeiling = false;
			}

			m_IsGround = false;
		}
	}
}

void CGameObject::Collision(float DeltaTime)
{
	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		m_Scene->GetSceneCollision()->AddCollider(*iter);
	}


	{
		auto	iter = m_WidgetComponentList.begin();
		auto	iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Collision(DeltaTime);
		}
	}
}

void CGameObject::PrevRender()
{
	CCamera* Camera = m_Scene->GetCamera();

	m_RenderPos = m_Pos - Camera->GetPos();

	Vector2	Size = m_Size;

	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Size = FrameData.Size;
	}

	Vector2	LT = m_RenderPos - m_Pivot * Size + m_Offset;

	m_CameraCull = false;

	if (LT.x + Size.x <= 0.f)
		m_CameraCull = true;

	else if (LT.x >= Camera->GetResolution().x)
		m_CameraCull = true;

	else if (LT.y + Size.y <= 0.f)
		m_CameraCull = true;

	else if (LT.y >= Camera->GetResolution().y)
		m_CameraCull = true;
}

void CGameObject::Render(HDC hDC)
{
	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Vector2	LT = m_RenderPos - m_Pivot * FrameData.Size + m_Offset;

		if (AnimInfo->Sequence->GetTextureType() == ETexture_Type::Atlas)
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size);
		}

		else
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size, AnimInfo->Frame);
		}
	}

	else
	{
		Vector2	LT = m_RenderPos - m_Pivot * m_Size + m_Offset;
		if (m_Texture)
		{
			// 이미지를 이용해서 출력한다.
			m_Texture->Render(hDC, LT, m_ImageStart, m_Size);
		}
	}


	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		else if ((*iter)->GetEnable())
		{
			(*iter)->Render(hDC);
		}

		++iter;
	}

	{
		auto	iter = m_WidgetComponentList.begin();
		auto	iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Render(hDC);
		}
	}

	m_PrevPos = m_Pos;
	m_Velocity = Vector2(0.f, 0.f);
}

CGameObject* CGameObject::Clone()
{
	return nullptr;
}

float CGameObject::SetDamage(float Damage)
{
	if (!m_DamageEnable)
		return 0.f;

	return Damage;
}

float CGameObject::GetFloorTileY()
{
	if (!m_Scene->GetTileMap())
		return 0.f;

	CTileMap* TileMap = m_Scene->GetTileMap();

	Vector2	LB = m_Pos;

	Vector2	RB = LB + m_Size.x / 2.f;

	int LBIndexX;
	int LBIndexY;
	int RBIndexX;
	int RBIndexY;

	

	LBIndexX = TileMap->GetOriginTileIndexX(LB.x);
	LBIndexY = TileMap->GetOriginTileIndexY(LB.y);
	RBIndexX = TileMap->GetOriginTileIndexX(RB.x);
	RBIndexY = TileMap->GetOriginTileIndexY(RB.y);


	LBIndexX = LBIndexX > TileMap->GetTileCountX() - 1 ? TileMap->GetTileCountX() - 1 : LBIndexX;
	LBIndexY = LBIndexY > TileMap->GetTileCountY() - 1 ? TileMap->GetTileCountY() - 1 : LBIndexY;

	RBIndexX = RBIndexX > TileMap->GetTileCountX() - 1 ? TileMap->GetTileCountX() - 1 : RBIndexX;

	for (; LBIndexX <= RBIndexX; ++LBIndexX)
	{
		for (int i = 0; i < 20; ++i)
		{
			if (TileMap->GetTile(LBIndexX, LBIndexY + i)->GetTileOption() == ETileOption::Wall)
				return TileMap->GetTile(LBIndexX, LBIndexY + i)->GetPos().y - TileMap->GetTile(LBIndexX, LBIndexY + i)->GetSize().y / 2.f;
		}
	}

	return 0.f;
}

float CGameObject::GetFloorTileY(float PosY)
{
	if (!m_Scene->GetTileMap())
		return 0.f;

	CTileMap* TileMap = m_Scene->GetTileMap();

	Vector2	LB = m_Pos;

	Vector2	RB = LB + m_Size.x / 2.f;

	int LBIndexX;
	int LBIndexY;
	int RBIndexX;
	int RBIndexY;



	LBIndexX = TileMap->GetOriginTileIndexX(LB.x);
	LBIndexY = TileMap->GetOriginTileIndexY(PosY);
	RBIndexX = TileMap->GetOriginTileIndexX(RB.x);
	RBIndexY = TileMap->GetOriginTileIndexY(PosY);


	LBIndexX = LBIndexX > TileMap->GetTileCountX() - 1 ? TileMap->GetTileCountX() - 1 : LBIndexX;
	LBIndexY = LBIndexY > TileMap->GetTileCountY() - 1 ? TileMap->GetTileCountY() - 1 : LBIndexY;

	RBIndexX = RBIndexX > TileMap->GetTileCountX() - 1 ? TileMap->GetTileCountX() - 1 : RBIndexX;
	RBIndexY = RBIndexY > TileMap->GetTileCountY() - 1 ? TileMap->GetTileCountY() - 1 : RBIndexY;

	int MaxDownCount = TileMap->GetTileCountY();

	MaxDownCount = LBIndexY >= RBIndexY ? MaxDownCount - LBIndexY : MaxDownCount - RBIndexY;

	for (; LBIndexX <= RBIndexX; ++LBIndexX)
	{
		for (int i = 0; i < MaxDownCount; ++i)
		{
			if (TileMap->GetTile(LBIndexX, LBIndexY + i)->GetTileOption() == ETileOption::Wall)
				return TileMap->GetTile(LBIndexX, LBIndexY + i)->GetPos().y/* - TileMap->GetTile(LBIndexX, LBIndexY + i)->GetSize().y / 2.f*/;
		}
	}

	return 0.f;
}
