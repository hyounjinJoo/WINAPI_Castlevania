#pragma once
#include "Scene.h"
class CMainScene :
    public CScene
{
	friend class CSceneManager;

public:
	CMainScene();
	virtual ~CMainScene();

public:
	virtual bool Init();

private:
	void LoadAnimationSequence();
	void LoadPlayerAnimationSequence();
	void LoadPlayerAttackSequence();
	void LoadMonsterAnimationSequence();
	void LoadObjectAnimationSequence();
	void LoadEffectAnimationSequence();

	void LoadSound();
	void CreatePrototypes();

	//Monster Animation Sequence
	void ZombieAnimationSequence();
	void Boss1AnimationSequence();

	//Object Animation Sequence
	void DoorAnimationSequence();
};

