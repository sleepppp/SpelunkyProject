/*************************************************************************
## Fmode Lib  ##
**************************************************************************/
#pragma once
#include "SingletonBase.h"
//�������� ä�� ���� ����(����)
#define EXTRA_SOUND_CHANNEL 36
#define SOUNDBUFFER 36

//�� ���� ����
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
	//���� �߰�
	void AddSound(std::string keyName, std::string fileName, bool background = false, bool loop = false);
	//���
	void Play(std::string keyName, float volume = 1.0f);
	void PlayBGM(std::string keyName);
	//Volume
	void SetVolume(std::string keyName, float volume);
	float GetVolume(std::string keyName);
	void SetMusicVolume(float set);
	void SetLastBgmTime();
	//����
	void Stop(std::string keyName);
	//����
	void Pause(std::string keyName);
	void FadeoutBGM();
	//�ٽ� ���
	void Resume(std::string keyName);
	//�����ִ°�
	bool IsPauseSound(std::string keyName);
	//�ش� ���尡 �����ִ��� 
	bool IsEndMusic(std::string keyName);
	//������ΰ�
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

