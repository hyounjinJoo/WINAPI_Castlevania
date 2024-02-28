#pragma once

#include "MapBase.h"
#include "../Resource/Texture.h"
#include "Tile.h"
#include "Line.h"
#include "Spawner.h"
#include "ScrollMap.h"
#include "../Object/Door.h"

class CTileMap :
    public CMapBase
{
	friend class CScene;

protected:
	CTileMap();
	virtual ~CTileMap();

protected:
	std::vector<CTile*>	m_vecTile;
	std::vector<CLine*> m_vecLine;
	std::vector<CSpawner*> m_vecSpawner;
	std::vector<CDoor*>		m_vecDoor;
	CSharedPtr<CTexture>	m_TileTexture; 
	CSharedPtr<CTexture>	m_ForeGroundTexture;
	CSharedPtr<CTexture>	m_MidGroundTexture;
	CSharedPtr<CTexture>	m_BackGroundTexture;
	int		m_TileCountX;
	int		m_TileCountY;
	Vector2	m_TileSize;

	int		m_StartX;
	int		m_StartY;
	int		m_EndX;
	int		m_EndY;

public:
	int GetTileCountX()	const
	{
		return m_TileCountX;
	}

	int GetTileCountY()	const
	{
		return m_TileCountY;
	}

	CTexture* GetForeGround() const
	{
		return m_ForeGroundTexture;
	}

	CTexture* GetMidGround() const
	{
		return m_MidGroundTexture;
	}

	CTexture* GetBackGround() const
	{
		return m_BackGroundTexture;
	}

	CDoor* GetDoor(int Index) const
	{
		return m_vecDoor[Index];
	}

	int GetDoorCount() const
	{
		return (int)(m_vecDoor.size());
	}

	EDoorStage GetDoorStage(int Index) const
	{
		return m_vecDoor[Index]->GetDoorStage();
	}

	Vector2 GetDoorPos(int Index) const
	{
		return m_vecDoor[Index]->GetPos();
	}

	bool GetIsDoorLeft(int Index) const
	{
		return m_vecDoor[Index]->GetIsDoorLeft();
	}

public:
	bool CreateTile(int CountX, int CountY, const Vector2& TileSize);
	bool CreateLine(const Vector2& SrcPos, const Vector2& DestPos);
	bool SetTileTexture(CTexture* Texture);
	bool SetTileTexture(CTexture* Texture, int IndexX, int IndexY);
	void SetTileTexture(const Vector2& Pos, CTexture* Texture);
	bool SetForeGroundTexture(CTexture* Texture);
	bool SetMidGroundTexture(CTexture* Texture);
	bool SetBackGroundTexture(CTexture* Texture);
	void ChangeTileOption(const Vector2& Pos, ETileOption Option);
	void ChangeTileSideCollision(const Vector2& Pos, bool SideCollision);
	void SetTileFrame(const Vector2& Pos, const Vector2& Start, const Vector2& End);
	CTile* GetTile(const Vector2& Pos);
	CTile* GetTile(int Index);
	CTile* GetTile(int IndexX, int IndexY);
	int GetTileIndexX(float PosX);
	int GetTileIndexY(float PosY);
	int GetTileIndex(const Vector2& Pos);
	int GetOriginTileIndexX(float PosX);
	int GetOriginTileIndexY(float PosY);
	CLine* GetLine(int Index);
	int	GetLineCount();
	CLine* GetLastLine();
	void DeleteLastLine();
	void TileImageAllClear();
	void LineAllClear();
	void ClearLine(const Vector2& Pos);
	CLine* GetLine(const Vector2& Pos);
	void AddSpawner(const std::string& SpawnerName, float PosX, float PosY,
		bool SpawnDir, const std::string& SpawnMonsterName, bool IsSpawn = false, int SpawnLimit = 5);
	void ClearSpawner(const Vector2& Pos);
	void AddDoor(const EDoorStage Stage, const Vector2& Pos, bool IsDoorFormLeft);
	void ClearDoor(const Vector2& Pos);
	void SetSoundBGM();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	void RenderMidMap(HDC hDC);
};

