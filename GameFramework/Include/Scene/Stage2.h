#pragma once
#include "MainScene.h"
class CStage2 :
    public CMainScene
{
public:
	CStage2();
	virtual ~CStage2();

public:
	virtual bool Init();

	void SetSoundBGM();
};

