#pragma once
#include "UIWindow.h"
class CUIStart :
    public CUIWindow
{
	friend class CScene;

private:
	CUIStart();
	virtual ~CUIStart();
	void Update(float DeltaTime);

	bool IsOpeningEnd;

public:
	virtual bool Init();

public:
	void StartClick();
	void ExitClick();
	void EditorClick();
};

