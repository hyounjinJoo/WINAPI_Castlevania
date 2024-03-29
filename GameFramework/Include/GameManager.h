#pragma once

#include "GameInfo.h"

class CGameManager
{
private:
	CGameManager();
	~CGameManager();

private:
	static bool	m_Loop;
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	HDC			m_hDC;
	HDC			m_hBackDC;
	HBITMAP		m_hBackBmp;
	HBITMAP		m_hPrevBackBmp;
	Resolution	m_RS;
	class CTimer* m_Timer;
	float		m_TimeScale;
	HBRUSH		m_hGreenBrush;
	HBRUSH		m_hRedBrush;
	HBRUSH		m_hYellowBrush;
	HPEN		m_hGreenPen;
	HPEN		m_hRedPen;
	HPEN		m_hVioletPen;
	bool		m_EditorMode;
	bool		m_RenderTileMap;
	bool		m_RenderCollider;
	bool		m_RenderDebugText;

public:
	Resolution GetResolution()	const
	{
		return m_RS;
	}

	HPEN GetGreenPen()	const
	{
		return m_hGreenPen;
	}

	HPEN GetRedPen()	const
	{
		return m_hRedPen;
	}

	HPEN GetVioletPen()	const
	{
		return m_hVioletPen;
	}

	HBRUSH GetGreenBrush()	const
	{
		return m_hGreenBrush;
	}

	HBRUSH GetRedBrush()	const
	{
		return m_hRedBrush;
	}

	HBRUSH GetYellowBrush()	const
	{
		return m_hYellowBrush;
	}

	HDC GetWindowDC()	const
	{
		return m_hDC;
	}

	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance()	const
	{
		return m_hInst;
	}

	bool IsEditorMode()	const
	{
		return m_EditorMode;
	}

	bool IsRenderTileMap()	const
	{
		return m_RenderTileMap;
	}

	bool IsRenderCollider()	const
	{
		return m_RenderCollider;
	}

	bool IsRenderDebugText()	const
	{
		return m_RenderDebugText;
	}

public:
	void SetRenderTileMap(bool LikeDebug)
	{
		m_RenderTileMap = LikeDebug;
	}

	void SetRenderCollider(bool RenderCollider)
	{
		m_RenderCollider = RenderCollider;
	}

	void SetRenderText(bool RenderText)
	{
		m_RenderDebugText = RenderText;
	}

	void SetEditorMode(bool Mode)
	{
		m_EditorMode = Mode;
	}

	void SetTimeScale(float TimeScale)
	{
		m_TimeScale = TimeScale;
	}

public:
	float GetDeltaTime()	const;
	float GetFPS()	const;
	void Exit();

public:
	bool Init(HINSTANCE hInst);
	int Run();


private:
	void Logic();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Collision(float DeltaTime);
	void Render(float DeltaTime);


private:
	ATOM Register();
	BOOL Create();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:
	static CGameManager* m_Inst;

public:
	static CGameManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CGameManager;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

