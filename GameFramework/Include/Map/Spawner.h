#pragma once
#include "../Object/GameObject.h"
class CSpawner :
    public CGameObject
{
    friend class CGameObject;
    friend class CTileMap;
    friend class CMainScene;

protected:
    CSpawner();
    CSpawner(const CSpawner& Obj);
    virtual ~CSpawner();
    

private:
    bool        m_IsCreateLeft;

    std::string m_SpawnMonsterName;

    bool        m_IsSpawn;

    float       m_SpawnTime;
    float       m_SpawnAfterTime;
    int         m_SpawnLimit;
    int         m_CurrentSpawn;
    bool        m_isEditor;

    class CGameObject* m_SpawnTarget;

public:
    void    SetSpawner(float PosX, float PosY, bool IsCreateLeft,
        const std::string& SpawnMonsterName, bool IsSpawn = false, int SpawnLimit = INT_MAX);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Collision(float DeltaTime);
    virtual void PrevRender();
    virtual void Render(HDC hDC);
    virtual CSpawner* Clone() const;
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

    //virtual void AddAnimationName();


};

