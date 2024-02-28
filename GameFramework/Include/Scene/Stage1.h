#pragma once
#include "MainScene.h"
class CStage1 :
    public CMainScene
{
public:
	CStage1();
	virtual ~CStage1();

public:
	virtual bool Init();

	void SetSoundBGM();
};

