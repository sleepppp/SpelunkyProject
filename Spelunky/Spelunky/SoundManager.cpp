#include "stdafx.h"
#include "SoundManager.h"

#include "StringHelper.h"
#include "Path.h"
#include <io.h>

using namespace FMOD;


/**********************************************************************
## SoundManager() ##
FMOD 시스템 초기화
**********************************************************************/
SoundManager::SoundManager()
	:system(nullptr), channel(nullptr), sound(nullptr)
{
	//사운드 시스템 생성 
	System_Create(&this->system);
	//채널수 설정 및 초기화
	this->system->init(TOTAL_SOUND_CHANNEL, FMOD_INIT_NORMAL, 0);
	//채널과 사운드를 동적할당(메모리 버퍼 및 사운드 생성) 
	this->sound = new Sound*[TOTAL_SOUND_CHANNEL];
	this->channel = new Channel*[TOTAL_SOUND_CHANNEL];
	//초기화
	memset(this->sound, 0, sizeof(Sound*) * TOTAL_SOUND_CHANNEL);
	memset(this->channel, 0, sizeof(Channel*) * TOTAL_SOUND_CHANNEL);

	this->lastBgmTime = -60.0f;
	this->lastStreamBgm = "NONE";
	this->musicVolume = 0.35f;
	this->soundVolume = 1.0f;

}

/**********************************************************************
## ~SoundManager ##
FMOD 시스템 메모리 할당 해제
**********************************************************************/
SoundManager::~SoundManager()
{
	if (this->channel != nullptr || this->sound != nullptr)
	{
		for (int i = 0; i < TOTAL_SOUND_CHANNEL; ++i)
		{
			if (this->channel != nullptr)
			{
				if (this->channel[i])
					this->channel[i]->stop();
			}

			if (this->sound != nullptr)
			{
				if (this->sound[i] != nullptr)
					this->sound[i]->release();
			}
		}
	}

	//메모리 지우기
	SafeDeleteArray(this->channel);
	SafeDeleteArray(this->sound);

	this->totalSounds.clear();
	this->fadeoutList.clear();
}

/**********************************************************************
## Update ##
FMOD 시스템 업데이트
**********************************************************************/
void SoundManager::Update()
{
	//FMOD System을 지속적으로 업데이트
	this->system->update();

	//페이드 아웃 처리 
	for (size_t i = 0; i < fadeoutList.size(); ++i)
	{
		float volume = 1.0f;
		this->fadeoutList[i]->getVolume(&volume);
		if (volume > 0.001f)
		{
			this->fadeoutList[i]->setVolume(volume - 0.0015f);
		}
		else
		{
			this->fadeoutList[i]->stop();
			this->fadeoutList.erase(fadeoutList.begin() + i--);
			this->lastBgmTime = _TimeManager->DeltaTime() - 25.0f;
			this->lastStreamBgm = "NONE";
		}
	}
}

/**********************************************************************
## AddSound ##
FMOD에서 stream과 sound는 루프를 시키는지 아닌지로 나뉜다.
@@ string keyName : 이미지 키값
@@ string fileName : 파일 경로
@@ bool backGround : 배경음 불값
@@ bool loop : 루프 불값
**********************************************************************/
void SoundManager::AddSound(string keyName, string fileName, bool background, bool loop)
{
	ArrSoundsIter iter = totalSounds.find(keyName);
	if (iter != totalSounds.end())
		return;

	//루프라면
	if (loop)
	{
		//배경음 
		if (background)
		{
			this->system->createStream(fileName.c_str(), FMOD_LOOP_NORMAL,
				0, &this->sound[totalSounds.size()]);
		}
		//효과음
		else
		{
			this->system->createSound(fileName.c_str(), FMOD_DEFAULT,
				0, &this->sound[totalSounds.size()]);
		}
	}
	//루프가 아니라면
	else
	{
		//한번만 플레이
		this->system->createSound(fileName.c_str(), FMOD_DEFAULT,
			0, &this->sound[totalSounds.size()]);
	}

	this->totalSounds.insert(make_pair(keyName, &this->sound[totalSounds.size()]));
}

/**********************************************************************
## Play ##
효과음 재생
@@ string keyName : 사운드 키값
@@ float volume : 사운드 볼륨 비율
**********************************************************************/

void SoundManager::Play(string keyName, float volume)
{
	ArrSoundsIter iter = totalSounds.begin();
	int count = 0;

	for (; iter != this->totalSounds.end(); ++iter, ++count)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이 
			this->system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &this->channel[count]);
			//볼륨 설정
			this->channel[count]->setVolume(this->soundVolume * volume);
			break;
		}
	}
}

/**********************************************************************
## PlayBGM ##
배경음 재생
@@string keyname : 키값
**********************************************************************/
void SoundManager::PlayBGM(string keyName)
{
	if (this->IsPlaySound(lastStreamBgm))
		return;

	ArrSoundsIter iter = totalSounds.begin();
	int count = 0;

	for (; iter != totalSounds.end(); ++iter, ++count)
	{
		if (keyName == iter->first)
		{
			this->system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &channel[count]);
			this->channel[count]->setVolume(musicVolume);
			this->lastStreamBgm = keyName;
			break;
		}
	}
}
/**********************************************************************
## SetVolume ##
볼륨 지정
@@ string keyname : 키값
@@ float volume : 볼륨 값
**********************************************************************/
void SoundManager::SetVolume(string keyName, float volume)
{
	FMOD::Channel* channel = FindChannel(keyName);
	if (channel)
	{
		channel->setVolume(volume);
	}
}
/**********************************************************************
## GetVolume ##
@@ string keyname : 키값
**********************************************************************/
float SoundManager::GetVolume(string keyName)
{
	FMOD::Channel* channel = FindChannel(keyName);
	float volume = 0.f;
	if (channel)
		channel->getVolume(&volume);

	return volume;
}
/**********************************************************************
## SetMusicVolume ##
지금 재생 중인 배경음 볼륨 설정
@@ float volume : 볼륨
**********************************************************************/
void SoundManager::SetMusicVolume(float volume)
{
	this->musicVolume = volume;
	this->SetVolume(this->lastStreamBgm, volume);
}

/**********************************************************************
## setLastBGMTime ##
lastBgmTime 갱신
**********************************************************************/
void SoundManager::SetLastBgmTime()
{
	this->lastBgmTime = _TimeManager->GetWorldTime();
}

/**********************************************************************
## Stop
**********************************************************************/
void SoundManager::Stop(string keyName)
{
	ArrSoundsIter iter = totalSounds.begin();
	int count = 0;
	for (; iter != totalSounds.end(); ++iter, ++count)
	{
		if (keyName == iter->first)
		{
			this->channel[count]->stop();
			break;
		}
	}
}
/**********************************************************************
## Pause ##
@@ string keyname : 키값
**********************************************************************/
void SoundManager::Pause(string keyName)
{
	ArrSoundsIter iter = totalSounds.begin();
	int count = 0;
	for (; iter != totalSounds.end(); ++iter, ++count)
	{
		if (keyName == iter->first)
		{
			this->channel[count]->setPaused(true);
			break;
		}
	}
}
/**********************************************************************
## FadeOutBGM ##
**********************************************************************/
void SoundManager::FadeoutBGM()
{
	if (this->fadeoutList.empty() == false)
		return;
	ArrSoundsIter iter = totalSounds.begin();
	int count = 0;

	for (; iter != totalSounds.end(); ++iter, ++count)
	{
		if (lastStreamBgm == iter->first)
		{
			this->fadeoutList.push_back(channel[count]);
			break;
		}
	}
}
/**********************************************************************
## Resume ##
@@ string keyName : 키값
**********************************************************************/
void SoundManager::Resume(string keyName)
{
	ArrSoundsIter iter = this->totalSounds.begin();
	int count = 0;
	for (; iter != totalSounds.end(); ++iter, ++count)
	{
		if (keyName == iter->first)
		{
			this->channel[count]->setPaused(false);
			break;
		}
	}
}

/**********************************************************************
## IsPauseSound ##
@@ string keyname : 키값
**********************************************************************/
bool SoundManager::IsPauseSound(string keyName)
{
	bool isPause = false;
	int count = 0;
	ArrSoundsIter iter = totalSounds.begin();
	for (; iter != totalSounds.end(); ++iter, ++count)
	{
		if (keyName == iter->first)
		{
			this->channel[count]->getPaused(&isPause);
			break;
		}
	}
	return isPause;
}

/**********************************************************************
## IsEndMusic ##
@@ string keyname : 키값
**********************************************************************/
bool SoundManager::IsEndMusic(string keyName)
{
	Channel* channel = FindChannel(keyName);
	Sound* currentSound;
	unsigned int position;
	unsigned int length;
	if (channel)
	{
		channel->getPosition(&position, FMOD_TIMEUNIT_MS);
		channel->getCurrentSound(&currentSound);

		currentSound->getLength(&length, FMOD_TIMEUNIT_MS);

		if (position >= length * 0.5f)
		{
			return true;
		}
	}
	return false;
}

/**********************************************************************
## IsPlaySound ##
@@ string keyName : 키값
**********************************************************************/
bool SoundManager::IsPlaySound(string keyName)
{
	bool isPlay = false;
	int count = 0;

	ArrSoundsIter iter = totalSounds.begin();
	for (; iter != totalSounds.end(); ++iter, ++count)
	{
		if (keyName == iter->first)
		{
			this->channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

/**********************************************************************
## IsPlayBGM ##
현재 BGM이 플레이중인지 반환
**********************************************************************/
bool SoundManager::IsPlayBGM()
{
	if (this->lastStreamBgm == "NONE")
		return false;
	return IsPlaySound(lastStreamBgm);
}
/**********************************************************************
## FindCannel ##
@@ string keyname : 키값
**********************************************************************/
Channel * SoundManager::FindChannel(string keyName)
{
	ArrSoundsIter iter = totalSounds.begin();
	int count = 0;
	for (; iter != totalSounds.end(); ++iter, ++count)
	{
		if (keyName == iter->first)
			return channel[count];
	}
	return nullptr;
}

/**********************************************************************
## IsEndMusic ##
재생되는 사운드의 현재 플레이 타임을 밀리세컨드 단위로 반환
@@ string keyname : 키값
**********************************************************************/
unsigned int SoundManager::GetPosition(string keyName)
{
	ArrSoundsIter iter = totalSounds.begin();
	int count = 0;
	UINT soundPos;

	for (; iter != totalSounds.end(); ++iter, ++count)
	{
		if (keyName == iter->first)
		{
			this->channel[count]->getPosition(&soundPos, FMOD_TIMEUNIT_MS);
			return soundPos;
		}
	}

	return 0;
}

/**********************************************************************
## GetLength ##
해당 사운드의 총 재생시간을 밀리세컨드 단위로 반환
@@ string keyname : 키값
**********************************************************************/
unsigned int SoundManager::GetLength(string keyName)
{
	ArrSoundsIter iter = totalSounds.begin();
	int count = 0;
	UINT soundLength;
	for (; iter != totalSounds.end(); ++iter, ++count)
	{
		if (keyName == iter->first)
		{
			this->sound[count]->getLength(&soundLength, FMOD_TIMEUNIT_MS);
			return static_cast<UINT>(soundLength);
		}
	}
	return 0;
}

/**********************************************************************
## SetPosition ##
해당 사운드를 정한 시간때로 재생
@@ string keyname : 키값
@@ UINT time : 재생할 시간
**********************************************************************/
void SoundManager::SetPosition(string keyName, unsigned int time)
{
	ArrSoundsIter iter = totalSounds.begin();
	int count = 0;
	for (; iter != totalSounds.end(); ++iter, ++count)
	{
		if (keyName == iter->first)
		{
			this->channel[count]->setPosition(time, FMOD_TIMEUNIT_MS);
			break;
		}
	}
}

void SoundManager::LoadAllSound()
{
	this->AddSound("ButtonOnMouse", "../Resources/Sound/ButtonOnMouse.wav", false, false);
	this->AddSound("MenuOpen", "../Resources/Sound/MenuOpen.wav", false, false);
	this->AddSound("zone2", "../Resources/Sound/zone2.ogg", true, true);
	this->AddSound("ShotGun", "../Resources/Sound/shotgun.wav", false, false);
	this->AddSound("ShotGunPump", "../Resources/Sound/shotgunpump.wav", false, false);
	this->AddSound("bomb_timer", "../Resources/Sound/bomb_timer.wav", false, false);
	this->AddSound("kaboom", "../Resources/Sound/kaboom.wav", false, false);
	this->AddSound("ThrowItem", "../Resources/Sound/throw_item.wav", false, false);
	this->AddSound("BatFlap", "../Resources/Sound/batoneflap.wav", false, false);
	this->AddSound("bone_shatter", "../Resources/Sound/bone_shatter.wav", false, false);
	this->AddSound("batAttack", "../Resources/Sound/batAttack.ogg", false, false);
	this->AddSound("batDeath", "../Resources/Sound/batDeath.ogg", false, false);
	this->AddSound("splat", "../Resources/Sound/splat.wav", false, false);
	this->AddSound("snakebite", "../Resources/Sound/snakebite.wav", false, false);
	this->AddSound("hit", "../Resources/Sound/hit.wav", false, false);
	this->AddSound("rubble", "../Resources/Sound/rubble.wav", false, false);
	this->AddSound("zapper", "../Resources/Sound/zapper.wav", false, false);
	this->AddSound("shootemup", "../Resources/Sound/shoot em up.wav", false, false);
	this->AddSound("cocked", "../Resources/Sound/cocked.wav", false, false);
	this->AddSound("playerHurt", "../Resources/Sound/playerHurt.ogg", false, false);
	this->AddSound("frog2", "../Resources/Sound/frog2.wav", false, false);
	this->AddSound("frogDeath", "../Resources/Sound/slimeHit.ogg", false, false);
	this->AddSound("bounce", "../Resources/Sound/bounce.wav", false, false);
	this->AddSound("yeah", "../Resources/Sound/yeah.ogg", false, false);
	this->AddSound("AWPShot", "../Resources/Sound/AWPShot.mp3", false, false);
	this->AddSound("FrogHouling", "../Resources/Sound/FrogHouling.ogg", false, false);
	
}


