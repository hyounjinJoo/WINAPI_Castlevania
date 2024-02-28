#include "Line.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "../GameManager.h"

CLine::CLine() :
	m_Scene(nullptr),
	m_StartPos(0,0),
	m_DestPos(0,0),
	m_Slope(0.f)
{
}

CLine::~CLine()
{
}

bool CLine::Init()
{
	return true;
}

void CLine::Update(float DeltaTime)
{
	if (m_DestPos.x - m_StartPos.x != 0)
		m_Slope = (m_DestPos.y - m_StartPos.y) / (m_DestPos.x - m_StartPos.x);
}

void CLine::PostUpdate(float DeltaTime)
{
}

void CLine::PrevRender()
{
}

void CLine::Render(HDC hDC)
{
	if (CGameManager::GetInst()->IsEditorMode() || CGameManager::GetInst()->IsRenderTileMap())
	{
		HPEN	Pen = CGameManager::GetInst()->GetVioletPen();

		CCamera* Camera = CSceneManager::GetInst()->GetScene()->GetCamera();

		Vector2	CameraPos = Camera->GetPos();

		Vector2 StartPos;
		Vector2 EndPos;

		StartPos.x = m_StartPos.x - CameraPos.x;
		StartPos.y = m_StartPos.y - CameraPos.y;
		EndPos.x = m_DestPos.x - CameraPos.x;
		EndPos.y = m_DestPos.y - CameraPos.y;

		HGDIOBJ	Prev = SelectObject(hDC, Pen);

		MoveToEx(hDC, (int)StartPos.x, (int)StartPos.y, NULL);

		LineTo(hDC, (int)EndPos.x, (int)EndPos.y);

		SelectObject(hDC, Prev);
	}
}

void CLine::Save(FILE* pFile)
{
	fwrite(&m_StartPos, sizeof(Vector2), 1, pFile);
	fwrite(&m_DestPos, sizeof(Vector2), 1, pFile);
}

void CLine::Load(FILE* pFile)
{
	fread(&m_StartPos, sizeof(Vector2), 1, pFile);
	fread(&m_DestPos, sizeof(Vector2), 1, pFile);
}
