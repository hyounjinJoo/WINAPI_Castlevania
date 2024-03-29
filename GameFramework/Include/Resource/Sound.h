#pragma once

#include "../Ref.h"

class CSound :
	public CRef
{
	friend class CResourceManager;

private:
	CSound();
	~CSound();

private:
	std::string	m_Name;
	FMOD::System* m_System;
	FMOD::Sound* m_Sound;
	FMOD::ChannelGroup* m_Group;
	FMOD::Channel* m_Channel;
	bool			m_Play;
	bool			m_IsPlayed;
	bool			m_End;
	bool			m_Loop;

public:
	bool IsPlay()	const
	{
		return m_Play;
	}

	bool IsEnd() const
	{
		return m_End;
	}

	bool IsLoop()	const
	{
		return m_Loop;
	}

public:
	bool LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group,
		bool Loop, const std::string& Name, const char* FileName,
		const std::string& PathName = SOUND_PATH);
	void Play();
	void Stop();
	void Pause();
	void Resume();
	void Update();
};

