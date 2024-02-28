#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture.h"

struct TileFrameData
{
	Vector2	Start;
	Vector2	End;
};

struct TileTextureFrameData
{
	std::vector<TileFrameData>	vecData;
};

class CEditorDlg
{
public:
	CEditorDlg();
	~CEditorDlg();

private:
	HWND	m_hDlg;
	int		m_ID;
	bool	m_Open;
	class CEditorScene* m_Scene;
	HWND	m_TextureListBox;
	int		m_SelectTextureListIndex;
	TCHAR	m_SelectTextureListText[256];
	CSharedPtr<CTexture>	m_SelectTileTexture;
	std::vector<TileTextureFrameData>	m_vecTextureFrameData;
	CSharedPtr<CTexture>	m_ForeGroundTexture;
	CSharedPtr<CTexture>	m_MidGroundTexture;
	CSharedPtr<CTexture>	m_BackGroundTexture;

	HWND	m_EditSelectCombo;
	EEditMode	m_EditMode;

	HWND	m_SpawnerEditCombo;
	ESpawnerEditMode m_SpawnerEditMode;

	HWND	m_SpawnerDirCombo;
	ESpawnerEditDir m_SpawnerEditDir;

	HWND	m_SpawnerObjCombo;
	ESpawnerEditOBJ m_SpawnerEditObj;

	HWND	m_EditModeCombo;
	ETileEditMode	m_TileEditMode;

	HWND	m_TileOptionCombo;
	ETileOption	m_TileOption;

	HWND		m_DoorStageCombo;
	EDoorStage	m_DoorStage;

	HWND		m_DoorFormCombo;
	EDoorForm	m_DoorForm;

	HWND	m_FrameListBox;
	int		m_SelectFrameIndex;

	HWND	m_SideCollisionCheckHandle;
	bool	m_SideCollision;

public:
	bool GetSideCollision()	const
	{
		return m_SideCollision;
	}

	ETileOption GetTileOption()	const
	{
		return (ETileOption)SendMessage(m_TileOptionCombo, CB_GETCURSEL, 0, 0);
	}

	TileFrameData GetTileFrameData()	const
	{
		if (m_SelectTextureListIndex == -1 ||
			m_SelectFrameIndex == -1)
			return TileFrameData();

		return m_vecTextureFrameData[m_SelectTextureListIndex].vecData[m_SelectFrameIndex];
	}

	EEditMode GetEditSelection() const
	{
		return (EEditMode)SendMessage(m_EditSelectCombo, CB_GETCURSEL, 0, 0);
	}

	ETileEditMode GetTileEditMode()	const
	{
		return (ETileEditMode)SendMessage(m_EditModeCombo, CB_GETCURSEL, 0, 0);
	}

	ESpawnerEditOBJ GetSpawnObj() const
	{
		return (ESpawnerEditOBJ)SendMessage(m_SpawnerObjCombo, CB_GETCURSEL, 0, 0);
	}

	ESpawnerEditDir GetSpawnDir() const
	{
		return (ESpawnerEditDir)SendMessage(m_SpawnerDirCombo, CB_GETCURSEL, 0, 0);
	}

	EDoorStage GetDoorStage() const
	{
		return (EDoorStage)SendMessage(m_DoorStageCombo, CB_GETCURSEL, 0, 0);
	}

	EDoorForm GetDoorForm() const
	{
		return (EDoorForm)SendMessage(m_DoorFormCombo, CB_GETCURSEL, 0, 0);
	}

	int GetSpawnLimit() const
	{
		BOOL	Transfer = FALSE;
		return GetDlgItemInt(m_hDlg, IDC_EDIT_SPAWNERCREATELIMIT, &Transfer, TRUE);
	}

	bool IsOpen()	const
	{
		return m_Open;
	}

	RECT GetRect()	const
	{
		RECT	rc;
		GetWindowRect(m_hDlg, &rc);
		return rc;
	}

	void SetScene(class CEditorScene* Scene)
	{
		m_Scene = Scene;
	}

public:
	bool Init(int ID);
	void Show();
	void CreateMap();
	void LoadTileTexture();
	void SelectTexture();
	void SelectList();
	void AddFrame();
	void DeleteFrame();
	void ModifyFrame();
	void ChangeFrame();
	void Save();
	void Load();
	void TileImageAllClear();
	void SideCollisionCheck();
	void LoadForeMap();
	void LoadMidMap();
	void LoadBackMap();
	void Update();
	void ChangeEditMode();
	void ReturnToStartScene();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
