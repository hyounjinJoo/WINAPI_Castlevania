#include "Spawner.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Camera.h"
#include "../Object/Monster/Monster.h"
#include "../Scene/Scene.h"
#include "../GameManager.h"
#include "../Scene/EditorScene.h"

CSpawner::CSpawner() :
	m_IsCreateLeft(false),
	m_IsSpawn(true),
	m_SpawnTime(8.f),
	m_SpawnAfterTime(0.f),
	m_SpawnLimit(INT_MAX),
	m_CurrentSpawn(0),
	m_SpawnTarget(nullptr),
	m_isEditor(false)
{
	m_Size = Vector2(100.f, 100.f);
}

CSpawner::CSpawner(const CSpawner& Obj)
{
	m_IsCreateLeft = Obj.m_IsCreateLeft;
	m_IsSpawn = true;
	m_SpawnTime = Obj.m_SpawnTime;
	m_SpawnAfterTime = 0.f;

	m_SpawnMonsterName = Obj.m_SpawnMonsterName;
	m_CurrentSpawn = Obj.m_CurrentSpawn;
	m_Size = Vector2(100.f, 100.f);
}

CSpawner::~CSpawner()
{
	SAFE_RELEASE(m_SpawnTarget);
}

void CSpawner::SetSpawner(float PosX, float PosY, bool IsCreateLeft, const std::string& SpawnMonsterName, bool IsSpawn, int SpawnLimit)
{
	m_Pos = Vector2(PosX, PosY);
	m_PrevPos = Vector2(PosX, PosY);
	m_IsCreateLeft = IsCreateLeft;
	m_SpawnMonsterName = SpawnMonsterName;
	m_IsSpawn = IsSpawn;
	m_SpawnLimit = SpawnLimit;
}

void CSpawner::Start()
{
	CGameObject::Start();

	m_SpawnTime = static_cast<float>(rand() % 1);
}

bool CSpawner::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CSpawner::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (!m_IsSpawn)
	{
		m_IsCreateLeft = m_Pos.x - CSceneManager::GetInst()->GetScene()->GetCamera()->GetPos().x >= 0.f ? true : false;

		if (m_IsCreateLeft)
		{
			bool IsCreate = true;

			if (typeid(*m_Scene) == typeid(CEditorScene))
				IsCreate = false;

			if (m_CurrentSpawn >= m_SpawnLimit)
				IsCreate = false;

			if (IsCreate)
			{
				CMonster* Monster = m_Scene->CreateObject<CMonster>(m_SpawnMonsterName, m_SpawnMonsterName,
					m_Pos);
				Monster->SetScene(m_Scene);
				Monster->SetPos(m_Pos);
				Monster->SetPrevPos(m_Pos);

				m_IsSpawn = true;
				++m_CurrentSpawn;
			}
		}
	}
	else
	{
		m_SpawnAfterTime += DeltaTime;

		if (m_SpawnAfterTime >= m_SpawnTime)
		{
			m_SpawnAfterTime = 0.f;
			m_SpawnTime = static_cast<float>(rand() % 10);
			m_IsSpawn = false;
		}
	}
}

	

void CSpawner::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CSpawner::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CSpawner::PrevRender()
{
	CGameObject::PrevRender();
}

void CSpawner::Render(HDC hDC)
{
	CGameObject::Render(hDC);

	if (typeid(*m_Scene) != typeid(CEditorScene))
		return;

	CCamera* Camera = m_Scene->GetCamera();

	RectInfo	RenderInfo;

	RenderInfo.Left = m_Pos.x - Camera->GetPos().x;
	RenderInfo.Right = m_Pos.x + m_Size.x - Camera->GetPos().x;
	RenderInfo.Top = m_Pos.y - Camera->GetPos().y;
	RenderInfo.Bottom = m_Pos.y + m_Size.y - Camera->GetPos().y;

	RECT	rc = { (long)RenderInfo.Left, (long)RenderInfo.Top,
		(long)RenderInfo.Right, (long)RenderInfo.Bottom };

	FrameRect(hDC, &rc, CGameManager::GetInst()->GetRedBrush());
}

CSpawner* CSpawner::Clone() const
{
	return new CSpawner(*this);
}

void CSpawner::Save(FILE* pFile)
{
	fwrite(&m_IsCreateLeft, sizeof(bool), 1, pFile);

	int	Length = (int)m_SpawnMonsterName.length();

	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_SpawnMonsterName.c_str(), sizeof(char), Length, pFile);

	fwrite(&m_SpawnTime, sizeof(float), 1, pFile);
	fwrite(&m_SpawnLimit, sizeof(int), 1, pFile);
	fwrite(&m_SpawnTarget, sizeof(CGameObject*), 1, pFile);
	fwrite(&m_Pos, sizeof(Vector2), 1, pFile);
	fwrite(&m_PrevPos, sizeof(Vector2), 1, pFile);
}

void CSpawner::Load(FILE* pFile)
{
	fread(&m_IsCreateLeft, sizeof(bool), 1, pFile);

	int	Length;
	char	Name[256] = {};

	fread(&Length, sizeof(int), 1, pFile);
	fread(Name, sizeof(char), Length, pFile);

	m_SpawnMonsterName = Name;

	fread(&m_SpawnTime, sizeof(float), 1, pFile);
	fread(&m_SpawnLimit, sizeof(int), 1, pFile);
	fread(&m_SpawnTarget, sizeof(CGameObject*), 1, pFile);
	fread(&m_Pos, sizeof(Vector2), 1, pFile);
	fread(&m_PrevPos, sizeof(Vector2), 1, pFile);
}