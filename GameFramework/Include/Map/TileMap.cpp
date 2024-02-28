
#include "TileMap.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"

CTileMap::CTileMap() :
	m_TileCountX(0),
	m_TileCountY(0),
	m_StartX(-1),
	m_StartY(-1),
	m_EndX(-1),
	m_EndY(-1)
{
}

CTileMap::~CTileMap()
{
	size_t	Size = m_vecTile.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecTile[i]);
	}

	m_vecTile.clear();

	Size = m_vecLine.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecLine[i]);
	}

	m_vecLine.clear();

	Size = m_vecSpawner.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecSpawner[i]);
	}

	m_vecSpawner.clear();

	Size = m_vecDoor.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecDoor[i]);
	}

	m_vecDoor.clear();
}

bool CTileMap::CreateTile(int CountX, int CountY, const Vector2& TileSize)
{
	m_TileCountX = CountX;
	m_TileCountY = CountY;
	m_TileSize = TileSize;

	{
		auto	iter = m_vecTile.begin();
		auto	iterEnd = m_vecTile.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE((*iter));
		}

		m_vecTile.clear();
	}

	m_Size = m_TileSize * Vector2((float)m_TileCountX, (float)m_TileCountY);

	CCamera* Camera = m_Scene->GetCamera();
	Camera->SetWorldResolution(m_Size);

	for (int i = 0; i < m_TileCountY; ++i)
	{
		for (int j = 0; j < m_TileCountX; ++j)
		{
			CTile* Tile = new CTile;
			Tile->m_Scene = m_Scene;

			Tile->Init();

			Vector2	Pos = Vector2((float)j, (float)i) * m_TileSize;

			Tile->SetTileInfo(Pos, m_TileSize, j, i, i * m_TileCountX + j,
				m_TileTexture);

			m_vecTile.push_back(Tile);
		}
	}


	for (int i = 0; i < (int)m_TileCountY; ++i)
	{
		for (int j = 0; j < (int)m_TileCountX; ++j)
		{
			m_vecTile[i * m_TileCountX + j]->SetTileTexture(m_TileTexture);
		}
	}

	return true;
}

bool CTileMap::CreateLine(const Vector2& SrcPos, const Vector2& DestPos)
{
	CLine* Line = new CLine;
	Line->m_Scene = m_Scene;

	Line->Init();

	Line->SetLineInfo(SrcPos, DestPos);

	m_vecLine.push_back(Line);

	return true;
}

bool CTileMap::SetTileTexture(CTexture* Texture)
{
	m_TileTexture = Texture;
	for (int i = 0; i < m_TileCountY; ++i)
	{
		for (int j = 0; j < m_TileCountX; ++j)
		{
			m_vecTile[i * m_TileCountX + j]->SetTileTexture(m_TileTexture);
		}
	}

	return true;
}

bool CTileMap::SetTileTexture(CTexture* Texture, int IndexX, int IndexY)
{
	m_TileTexture = Texture;
	m_vecTile[IndexY * m_TileCountX + IndexX]->SetTileTexture(m_TileTexture);

	return true;
}

void CTileMap::SetTileTexture(const Vector2& Pos, CTexture* Texture)
{
	CTile* Tile = GetTile(Pos);

	if (!Tile)
		return;

	Tile->SetTileTexture(Texture);
}

bool CTileMap::SetForeGroundTexture(CTexture* Texture)
{
	m_ForeGroundTexture = Texture;
	return true;
}

bool CTileMap::SetMidGroundTexture(CTexture* Texture)
{
	m_MidGroundTexture = Texture;
	return true;
}

bool CTileMap::SetBackGroundTexture(CTexture* Texture)
{
	m_BackGroundTexture = Texture;
	return true;
}

void CTileMap::ChangeTileOption(const Vector2& Pos, ETileOption Option)
{
	CTile* Tile = GetTile(Pos);

	if (!Tile)
		return;

	Tile->SetTileOption(Option);
}

void CTileMap::ChangeTileSideCollision(const Vector2& Pos, bool SideCollision)
{
	CTile* Tile = GetTile(Pos);

	if (!Tile)
		return;

	Tile->SetSideCollision(SideCollision);
}

void CTileMap::SetTileFrame(const Vector2& Pos,
	const Vector2& Start, const Vector2& End)
{
	CTile* Tile = GetTile(Pos);

	if (!Tile)
		return;

	Tile->SetTileTexture(m_TileTexture);
	Tile->SetStartFrame(Start);
	Tile->SetEndFrame(End);
}

CTile* CTileMap::GetTile(const Vector2& Pos)
{
	int	IndexX = (int)(Pos.x / m_TileSize.x);
	int	IndexY = (int)(Pos.y / m_TileSize.y);

	if (IndexX < 0 || IndexX >= m_TileCountX)
		return nullptr;

	if (IndexY < 0 || IndexY >= m_TileCountY)
		return nullptr;

	return m_vecTile[IndexY * m_TileCountX + IndexX];
}

CTile* CTileMap::GetTile(int Index)
{
	return m_vecTile[Index];
}

CTile* CTileMap::GetTile(int IndexX, int IndexY)
{
	return m_vecTile[IndexY * m_TileCountX + IndexX];
}

int CTileMap::GetTileIndexX(float PosX)
{
	int	IndexX = (int)(PosX / m_TileSize.x);

	if (IndexX < 0 || IndexX >= m_TileCountX)
		return -1;

	return IndexX;
}

int CTileMap::GetTileIndexY(float PosY)
{
	int	IndexY = (int)(PosY / m_TileSize.y);

	if (IndexY < 0 || IndexY >= m_TileCountY)
		return -1;

	return IndexY;
}

int CTileMap::GetTileIndex(const Vector2& Pos)
{
	int	IndexX = GetTileIndexX(Pos.x);
	int	IndexY = GetTileIndexY(Pos.y);

	if (IndexX == -1 || IndexY == -1)
		return -1;

	return IndexY * m_TileCountX + IndexX;
}

int CTileMap::GetOriginTileIndexX(float PosX)
{
	return (int)(PosX / m_TileSize.x);
}

int CTileMap::GetOriginTileIndexY(float PosY)
{
	return (int)(PosY / m_TileSize.y);
}

CLine* CTileMap::GetLine(int Index)
{
	return m_vecLine[Index];
}

int CTileMap::GetLineCount()
{
	return (int)(m_vecLine.size());
}

CLine* CTileMap::GetLastLine()
{
	return m_vecLine.back();
}

void CTileMap::DeleteLastLine()
{
	m_vecLine.pop_back();

	return;
}

void CTileMap::TileImageAllClear()
{
	size_t	Size = m_vecTile.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTile[i]->SetTileTexture(nullptr);
	}
}

void CTileMap::LineAllClear()
{
	size_t Size = m_vecLine.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecLine[i]);
	}

	m_vecLine.clear();
}

void CTileMap::ClearLine(const Vector2& Pos)
{
	if (m_vecLine.empty())
		return;

	size_t Size = m_vecLine.size();
	size_t target = -1;
	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecLine.empty())
			return;

		float MidleValue = m_vecLine[i]->GetX(Pos.y);
		if (MidleValue - 10.f <= Pos.x && Pos.x <= MidleValue + 10.f)
		{
			MidleValue = m_vecLine[i]->GetY(Pos.x);
			if (MidleValue - 10.f <= Pos.y && Pos.y <= MidleValue + 10.f)
			{
				target = i;
			}
		}
	}
	
	if (target != -1)
	m_vecLine.erase(m_vecLine.begin() + target);
}

CLine* CTileMap::GetLine(const Vector2& Pos)
{
	return nullptr;
}

void CTileMap::AddSpawner(const std::string& SpawnerName, float PosX, float PosY, bool SpawnDir, const std::string& SpawnMonsterName, bool IsSpawn, int SpawnLimit)
{
	CSpawner* Spawner = new CSpawner;

	Spawner->SetName(SpawnerName);
	Spawner->SetSpawner(PosX, PosY, SpawnDir, SpawnMonsterName, IsSpawn, SpawnLimit);
	Spawner->SetPrevPos(PosX, PosY);
	Spawner->SetSize(100.f, 100.f);
	Spawner->m_Scene = m_Scene;

	m_vecSpawner.push_back(Spawner);
}

void CTileMap::ClearSpawner(const Vector2& Pos)
{
	if (m_vecSpawner.empty())
		return;

	CCamera* Camera = m_Scene->GetCamera();

	RectInfo	Info;

	size_t Size = m_vecSpawner.size();
	size_t target = -1;
	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecSpawner.empty())
			return;

		Info.Left = m_vecSpawner[i]->m_Pos.x;
		Info.Right = m_vecSpawner[i]->m_Pos.x + m_vecSpawner[i]->m_Size.x;
		Info.Top = m_vecSpawner[i]->m_Pos.y;
		Info.Bottom = m_vecSpawner[i]->m_Pos.y + m_Size.y;

		if (Info.Left <= Pos.x && Pos.x <= Info.Right
			&& Info.Top <= Pos.y && Pos.y <= Info.Bottom)
		{
			target = i;
			break;
		}
	}

	if (target != -1)
		m_vecSpawner.erase(m_vecSpawner.begin() + target);
}

void CTileMap::AddDoor(const EDoorStage Stage, const Vector2& Pos, bool IsDoorFormLeft)
{
	CDoor* Door = m_Scene->CreateObject<CDoor>("Door", "Door", Pos);

	Door->SetName("Door");
	Door->SetDoor(Stage, Pos, IsDoorFormLeft);
	Door->SetPrevPos(Pos);
	Door->SetSize(128.f, 192.f);
	Door->m_Scene = m_Scene;

	//m_vecDoor.push_back(Door);
}

void CTileMap::ClearDoor(const Vector2& Pos)
{
	if (m_vecDoor.empty())
		return;

	CCamera* Camera = m_Scene->GetCamera();

	RectInfo	Info;

	size_t Size = m_vecDoor.size();
	// 씬에서 오브젝트도 가져와서 삭제할 것.
	int target = -1;
	for (int i = 0; i < Size; ++i)
	{
		if (m_vecDoor.empty())
			return;

		Info.Left = m_vecDoor[i]->m_Pos.x;
		Info.Right = m_vecDoor[i]->m_Pos.x + m_vecDoor[i]->m_Size.x;
		Info.Top = m_vecDoor[i]->m_Pos.y;
		Info.Bottom = m_vecDoor[i]->m_Pos.y + m_Size.y;

		if (Info.Left <= Pos.x && Pos.x <= Info.Right
			&& Info.Top <= Pos.y && Pos.y <= Info.Bottom)
		{
			target = i;
			break;
		}
	}

	if (target != -1)
	{
		m_vecDoor.erase(m_vecDoor.begin() + target);			
	}
}

void CTileMap::SetSoundBGM()
{
	m_Scene->SetSoundBGM();
}

void CTileMap::Start()
{
	CMapBase::Start();
}

bool CTileMap::Init()
{
	CMapBase::Init();

	return true;
}

void CTileMap::Update(float DeltaTime)
{
	CMapBase::Update(DeltaTime);

	if (!m_vecTile.empty())
	{
		CCamera* Camera = m_Scene->GetCamera();

		Vector2	CameraPos = Camera->GetPos();
		Vector2	Resolution = Camera->GetResolution();

		m_StartX = (int)(CameraPos.x / m_TileSize.x);
		m_StartY = (int)(CameraPos.y / m_TileSize.y);
		m_EndX = (int)((CameraPos.x + Resolution.x) / m_TileSize.x);
		m_EndY = (int)((CameraPos.y + Resolution.y) / m_TileSize.y);

		m_StartX = m_StartX < 0 ? 0 : m_StartX;
		m_StartY = m_StartY < 0 ? 0 : m_StartY;
		m_EndX = m_EndX >= m_TileCountX ? m_TileCountX - 1 : m_EndX;
		m_EndY = m_EndY >= m_TileCountY ? m_TileCountY - 1 : m_EndY;

		for (int i = m_StartY; i <= m_EndY; ++i)
		{
			for (int j = m_StartX; j <= m_EndX; ++j)
			{
				m_vecTile[i * m_TileCountX + j]->Update(DeltaTime);
			}
		}
	}

	if (!m_vecLine.empty())
	{
		size_t Size = m_vecLine.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecLine[i]->Update(DeltaTime);
		}
	}

	if (!m_vecSpawner.empty())
	{
		size_t Size = m_vecSpawner.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecSpawner[i]->Update(DeltaTime);
		}
	}

	if (!m_vecDoor.empty())
	{
		size_t Size = m_vecDoor.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecDoor[i]->Update(DeltaTime);
		}
	}
}

void CTileMap::PostUpdate(float DeltaTime)
{
	CMapBase::PostUpdate(DeltaTime);
	SetSoundBGM();
	if (!m_vecTile.empty())
	{
		if (m_StartY == -1 || m_StartX == -1 || m_EndX == -1 || m_EndY == -1)
			return;

		for (int i = m_StartY; i <= m_EndY; ++i)
		{
			for (int j = m_StartX; j <= m_EndX; ++j)
			{
				m_vecTile[i * m_TileCountX + j]->PostUpdate(DeltaTime);
			}
		}
	}

	if (!m_vecLine.empty())
	{
		size_t Size = m_vecLine.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecLine[i]->PostUpdate(DeltaTime);
		}
	}

	if (!m_vecSpawner.empty())
	{
		size_t Size = m_vecSpawner.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecSpawner[i]->PostUpdate(DeltaTime);
		}
	}

	if (!m_vecDoor.empty())
	{
		size_t Size = m_vecDoor.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecDoor[i]->PostUpdate(DeltaTime);
		}
	}
}

void CTileMap::PrevRender()
{
	CMapBase::PrevRender();


	if (!m_vecTile.empty())
	{
		if (m_StartY == -1 || m_StartX == -1 || m_EndX == -1 || m_EndY == -1)
			return;

		for (int i = m_StartY; i <= m_EndY; ++i)
		{
			for (int j = m_StartX; j <= m_EndX; ++j)
			{
				m_vecTile[i * m_TileCountX + j]->PrevRender();
			}
		}
	}

	if (!m_vecLine.empty())
	{
		size_t Size = m_vecLine.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecLine[i]->PrevRender();
		}
	}

	if (!m_vecSpawner.empty())
	{
		size_t Size = m_vecSpawner.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecSpawner[i]->PrevRender();
		}
	}

	if (!m_vecDoor.empty())
	{
		size_t Size = m_vecDoor.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecDoor[i]->PrevRender();
		}
	}
}

void CTileMap::Render(HDC hDC)
{
	CMapBase::Render(hDC);

	if (m_BackGroundTexture)
	{
		m_BackGroundTexture->Render(hDC, m_Pos, Vector2(0.f, 0.f), Vector2((float)m_BackGroundTexture->GetWidth(), (float)m_BackGroundTexture->GetHeight()));
	}

	if (m_MidGroundTexture)
	{
		RenderMidMap(hDC);
	}

	if (m_ForeGroundTexture)
	{
		CCamera* Camera = m_Scene->GetCamera();

		Vector2	CameraPos = Camera->GetPos();

		m_ForeGroundTexture->Render(hDC, m_Pos - CameraPos, Vector2(0.f, 0.f), Vector2((float)m_ForeGroundTexture->GetWidth(), (float)m_ForeGroundTexture->GetHeight()));
	}

	if (!m_vecTile.empty())
	{
		if (m_StartY == -1 || m_StartX == -1 || m_EndX == -1 || m_EndY == -1)
			return;

		for (int i = m_StartY; i <= m_EndY; ++i)
		{
			for (int j = m_StartX; j <= m_EndX; ++j)
			{
				m_vecTile[i * m_TileCountX + j]->Render(hDC);
			}
		}
	}

	if (!m_vecLine.empty())
	{
		size_t Size = m_vecLine.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecLine[i]->Render(hDC);
		}
	}

	if (!m_vecSpawner.empty())
	{
		size_t Size = m_vecSpawner.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecSpawner[i]->Render(hDC);
		}
	}

	if (!m_vecDoor.empty())
	{
		size_t Size = m_vecDoor.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecDoor[i]->Render(hDC);
		}
	}
}

void CTileMap::Save(FILE* pFile)
{
	CMapBase::Save(pFile);

	fwrite(&m_TileCountX, sizeof(int), 1, pFile);
	fwrite(&m_TileCountY, sizeof(int), 1, pFile);
	fwrite(&m_TileSize, sizeof(Vector2), 1, pFile);
	fwrite(&m_StartX, sizeof(int), 1, pFile);
	fwrite(&m_StartY, sizeof(int), 1, pFile);
	fwrite(&m_EndX, sizeof(int), 1, pFile);
	fwrite(&m_EndY, sizeof(int), 1, pFile);

	if (m_TileTexture)
	{
		bool	Tex = true;
		fwrite(&Tex, sizeof(bool), 1, pFile);

		m_TileTexture->Save(pFile);
	}

	else
	{
		bool	Tex = false;
		fwrite(&Tex, sizeof(bool), 1, pFile);
	}

	int	TileCount = (int)m_vecTile.size();
	fwrite(&TileCount, sizeof(int), 1, pFile);

	for (int i = 0; i < TileCount; ++i)
	{
		m_vecTile[i]->Save(pFile);
	}

	int	LineCount = (int)m_vecLine.size();
	fwrite(&LineCount, sizeof(int), 1, pFile);

	for (int i = 0; i < LineCount; ++i)
	{
		m_vecLine[i]->Save(pFile);
	}

	int	SpawnerCount = (int)m_vecSpawner.size();
	fwrite(&SpawnerCount, sizeof(int), 1, pFile);

	for (int i = 0; i < SpawnerCount; ++i)
	{
		m_vecSpawner[i]->Save(pFile);
	}

	int	DoorCount = (int)m_vecDoor.size();
	fwrite(&DoorCount, sizeof(int), 1, pFile);

	for (int i = 0; i < DoorCount; ++i)
	{
		m_vecDoor[i]->Save(pFile);
	}

	if (m_BackGroundTexture)
	{
		bool	Tex = true;
		fwrite(&Tex, sizeof(bool), 1, pFile);

		m_BackGroundTexture->Save(pFile);
	}

	else
	{
		bool	Tex = false;
		fwrite(&Tex, sizeof(bool), 1, pFile);
	}

	if (m_MidGroundTexture)
	{
		bool	Tex = true;
		fwrite(&Tex, sizeof(bool), 1, pFile);

		m_MidGroundTexture->Save(pFile);
	}

	else
	{
		bool	Tex = false;
		fwrite(&Tex, sizeof(bool), 1, pFile);
	}

	if (m_ForeGroundTexture)
	{
		bool	Tex = true;
		fwrite(&Tex, sizeof(bool), 1, pFile);

		m_ForeGroundTexture->Save(pFile);
	}

	else
	{
		bool	Tex = false;
		fwrite(&Tex, sizeof(bool), 1, pFile);
	}
}

void CTileMap::Load(FILE* pFile)
{
	CMapBase::Load(pFile);

	fread(&m_TileCountX, sizeof(int), 1, pFile);
	fread(&m_TileCountY, sizeof(int), 1, pFile);
	fread(&m_TileSize, sizeof(Vector2), 1, pFile);
	fread(&m_StartX, sizeof(int), 1, pFile);
	fread(&m_StartY, sizeof(int), 1, pFile);
	fread(&m_EndX, sizeof(int), 1, pFile);
	fread(&m_EndY, sizeof(int), 1, pFile);

	bool	Tex = true;
	fread(&Tex, sizeof(bool), 1, pFile);

	if (Tex)
	{
		m_TileTexture = CTexture::LoadStatic(pFile, m_Scene);
	}

	int	TileCount = 0;
	fread(&TileCount, sizeof(int), 1, pFile);

	size_t	TileSize = m_vecTile.size();

	for (size_t i = 0; i < TileSize; ++i)
	{
		SAFE_DELETE(m_vecTile[i]);
	}

	m_vecTile.clear();

	for (int i = 0; i < TileCount; ++i)
	{
		CTile* Tile = new CTile;
		Tile->m_Scene = m_Scene;

		m_vecTile.push_back(Tile);

		m_vecTile[i]->Load(pFile);
	}

	int	LineCount = 0;
	fread(&LineCount, sizeof(int), 1, pFile);

	size_t	LineSize = m_vecLine.size();

	for (size_t i = 0; i < LineSize; ++i)
	{
		SAFE_DELETE(m_vecLine[i]);
	}

	m_vecLine.clear();

	for (int i = 0; i < LineCount; ++i)
	{
		CLine* Line = new CLine;
		Line->m_Scene = m_Scene;

		m_vecLine.push_back(Line);

		m_vecLine[i]->Load(pFile);
	}

	int	SpawnerCount = 0;
	fread(&SpawnerCount, sizeof(int), 1, pFile);

	size_t	SpawnerSize = m_vecSpawner.size();

	for (size_t i = 0; i < SpawnerSize; ++i)
	{
		SAFE_DELETE(m_vecSpawner[i]);
	}

	m_vecSpawner.clear();

	for (int i = 0; i < SpawnerCount; ++i)
	{
		CSpawner* Spawner = new CSpawner;
		Spawner->m_Scene = m_Scene;

		m_vecSpawner.push_back(Spawner);

		m_vecSpawner[i]->Load(pFile);
	}
	
	int	DoorCount = 0;
	fread(&DoorCount, sizeof(int), 1, pFile);

	size_t	DoorSize = m_vecDoor.size();

	for (size_t i = 0; i < DoorSize; ++i)
	{
		SAFE_DELETE(m_vecDoor[i]);
	}

	m_vecDoor.clear();

	for (int i = 0; i < DoorCount; ++i)
	{
		CDoor* Door = new CDoor;
		Door->m_Scene = m_Scene;
		m_vecDoor.push_back(Door);

		m_vecDoor[i]->Load(pFile);
		m_vecDoor[i]->SetPos(m_vecDoor[i]->GetPos() - 1.f);
	}

	Tex = true;
	fread(&Tex, sizeof(bool), 1, pFile);

	if (Tex)
	{
		m_BackGroundTexture = CTexture::LoadStatic(pFile, m_Scene);
	}

	Tex = true;
	fread(&Tex, sizeof(bool), 1, pFile);

	if (Tex)
	{
		m_MidGroundTexture = CTexture::LoadStatic(pFile, m_Scene);
	}

	Tex = true;
	fread(&Tex, sizeof(bool), 1, pFile);

	if (Tex)
	{
		m_ForeGroundTexture = CTexture::LoadStatic(pFile, m_Scene);
	}


	m_Size = m_TileSize * Vector2((float)m_TileCountX, (float)m_TileCountY);

	CCamera* Camera = m_Scene->GetCamera();
	Camera->SetWorldResolution(m_Size);
}

void CTileMap::RenderMidMap(HDC hDC)
{
	if (m_MidGroundTexture)
	{
		CCamera* Camera = m_Scene->GetCamera();
	
		Vector2 Size = Vector2(1280.f, 720.f);

		float	ScrollWidth = m_MidGroundTexture->GetWidth() - Camera->GetResolution().x;
		float	ScrollHeight = m_MidGroundTexture->GetHeight() - Camera->GetResolution().y;

		float	TileMapWidth = m_Size.x - Camera->GetResolution().x;
		float	TileMapHeight = m_Size.y - Camera->GetResolution().y;

		float RatioX = TileMapWidth == 0.f ? 0.f : ScrollWidth / TileMapWidth;
		float RatioY = TileMapHeight == 0.f ? 0.f : ScrollHeight / TileMapHeight;

		Vector2 ScrollRatio = Vector2(RatioX, RatioY);

		Vector2 Resolution = Camera->GetResolution();

		Vector2	ImagePos = Camera->GetPos() * ScrollRatio;

		unsigned int	Width = m_MidGroundTexture->GetWidth();
		unsigned int	Height = m_MidGroundTexture->GetHeight();

		ImagePos.x = ImagePos.x < 0.f ? 0.f : ImagePos.x;
		ImagePos.x = ImagePos.x + Resolution.x > Width ? Width - Resolution.x : ImagePos.x;

		ImagePos.y = ImagePos.y < 0.f ? 0.f : ImagePos.y;
		ImagePos.y = ImagePos.y + Resolution.y > Height ? Height - Resolution.y : ImagePos.y;

		m_MidGroundTexture->Render(hDC, m_Pos, ImagePos, Size);
	}
}
