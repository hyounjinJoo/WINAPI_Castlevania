#pragma once
#include "MainScene.h"
class CStage3 :
	public CMainScene
{
public:
	CStage3();
	virtual ~CStage3();

public:
	virtual bool Init();

	void SetSoundBGM();
};

