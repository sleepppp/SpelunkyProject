/*************************************************************************
## Fmode Lib  ##
**************************************************************************/
#pragma once
#include "SingletonBase.h"
//여유분의 채널 갯수 설정(버퍼)
#define EXTRA_SOUND_CHANNEL 36
#define SOUNDBUFFER 36

//총 사운드 갯수
#define TOTAL_SOUND_CHANNEL (SOUNDBUFFER + EXTRA_SOUND_CHANNEL)

class SoundManager : public SingletonBase<SoundManager>
{
private:
	friend class SingletonBase<SoundManager>;
	SoundManager();
	~SoundManager();
private:
	typedef map<string, FMOD::Sound**> ArrSounds;
	typedef map<string, FMOD::Sound**>::iterator ArrSoundsIter;
	//typedef map<string, FMOD::Channel**> ArrChannels;
	//typedef map<string, FMOD::Channel**>::iterator ArrChannelIter;
private:
	FMOD::System* system;
	FMOD::Sound** sound;
	FMOD::Channel** channel;

	ArrSounds totalSounds;

	float	lastBgmTime;
	std::string  lastStreamBgm;

	float  soundVolume;
	float  musicVolume;

	vector<FMOD::Channel*> fadeoutList;
public:
	void Update();
	//사운드 추가
	void AddSound(std::string keyName, std::string fileName, bool background = false, bool loop = false);
	//재생
	void Play(std::string keyName, float volume = 1.0f);
	void PlayBGM(std::string keyName);
	//Volume
	void SetVolume(std::string keyName, float volume);
	float GetVolume(std::string keyName);
	void SetMusicVolume(float set);
	void SetLastBgmTime();
	//종료
	void Stop(std::string keyName);
	//정지
	void Pause(std::string keyName);
	void FadeoutBGM();
	//다시 재생
	void Resume(std::string keyName);
	//멈춰있는가
	bool IsPauseSound(std::string keyName);
	//해당 사운드가 꺼져있는지 
	bool IsEndMusic(std::string keyName);
	//재생중인가
	bool IsPlaySound(std::string keyName);
	bool IsPlayBGM();

	FMOD::Channel* FindChannel(std::string keyName);
	unsigned int GetPosition(std::string keyName);
	unsigned int GetLength(std::string keyName);
	void SetPosition(std::string keyName, unsigned int time);

	void SetSoundVolume(float set) { soundVolume = set; }
	float GetLastBgmTime() { return lastBgmTime; }
	std::string GetPlayingBGM() { return lastStreamBgm; }
	float GetMusicVolume() { return musicVolume; }
	float GetSoundVolume() { return soundVolume; }

	void LoadAllSound();
};

#define _SoundManager SoundManager::Get()

