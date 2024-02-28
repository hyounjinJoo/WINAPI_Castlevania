#pragma once
#include "GameObject.h"
class CDoor :
    public CGameObject
{
    friend class CGameObject;
    friend class CTileMap;
    friend class CMainScene;

public:
    CDoor();
    CDoor(const CDoor& Obj);
    ~CDoor();

private:
    bool m_IsDoorFormLeft;
    bool m_IsOpened;
    bool m_IsMoveEnd;
    float  m_PlayerMoveCount;
    EDoorStage m_DoorStage;

private:
    void DoorCollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime);
    void StageCreate();
    void PlayerMove(float DeltaTime);
    void OpenIsEnd();
    void ChangeDoor();

public:
    void SetDoorStage(int Stage)
    {
        m_DoorStage = (EDoorStage)Stage;
    }

    void SetDoorStage(EDoorStage Stage)
    {
        m_DoorStage = Stage;
    }

    void SetLeft(bool Left)
    {
        m_IsDoorFormLeft = Left;
    }

    void SetOpened(bool Opened)
    {
        m_IsOpened = Opened;
    }

    void SetDoor(EDoorStage Stage, Vector2 Pos, bool IsDoorFormLeft)
    {
        m_DoorStage = Stage;
        m_Pos = Pos;
        m_IsDoorFormLeft = IsDoorFormLeft;
    }

    EDoorStage GetDoorStage()
    {
        return m_DoorStage;
    }

    bool GetIsDoorLeft()
    {
        return m_IsDoorFormLeft;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void Render(HDC hDC);
    virtual void PrevRender();
    virtual CDoor* Clone();
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

