#pragma once

#include "../GameInfo.h"

class CLine
{
	friend class CTileMap;

private:
	CLine();
	~CLine();

private:
	class CScene* m_Scene;
	Vector2	m_StartPos;
	Vector2	m_DestPos;
	float	m_Slope;


public:
	Vector2 GetStartPos() const
	{
		return m_StartPos;
	}

	Vector2 GetDestPos() const
	{
		return m_DestPos;
	}

	Vector2 GetLeftPoint() const
	{
		return m_StartPos.x < m_DestPos.x ? m_StartPos : m_DestPos;
	}

	Vector2 GetRightPoint() const
	{
		return m_StartPos.x < m_DestPos.x ? m_DestPos : m_StartPos;
	}

	float GetSlope() const
	{
		return m_Slope;
	}

	float GetX(float PointY) const
	{
		// y = m(x - a) + b
		// PointY = m(x - StartPoint.x) + StartPoint.y
		// PointY - StartPoint.y = m(x - StartPoint.x)
		// ((PointY - StartPoint.y) / m) + StartPoint.x = x
		return (PointY + m_Slope * m_StartPos.x - m_StartPos.y) / m_Slope;
	}

	float GetY(float PointX) const
	{
		// y = m(x - a) + b
		// y = m(PointX - StartPoint.x) + StartPoint.y
		return (m_Slope * (PointX - m_StartPos.x)) + m_StartPos.y;
	}

public:
	void SetLineInfo(const Vector2& StartPos, const Vector2& DestPos)
	{
		m_StartPos = StartPos;
		m_DestPos = DestPos;

		if(m_DestPos.x - m_StartPos.x != 0)
		m_Slope = (m_DestPos.y - m_StartPos.y) / (m_DestPos.x - m_StartPos.x);
	}

	void SetStartPoint(const Vector2& StartPos)
	{
		m_StartPos = StartPos;

		if (m_DestPos.x - m_StartPos.x != 0)
		m_Slope = (m_DestPos.y - m_StartPos.y) / (m_DestPos.x - m_StartPos.x);
	}

	void SetDestPoint(const Vector2& DestPos)
	{
		m_DestPos = DestPos;

		if (m_DestPos.x - m_StartPos.x != 0)
		m_Slope = (m_DestPos.y - m_StartPos.y) / (m_DestPos.x - m_StartPos.x);
	}

public:
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void PrevRender();
	void Render(HDC hDC);
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

