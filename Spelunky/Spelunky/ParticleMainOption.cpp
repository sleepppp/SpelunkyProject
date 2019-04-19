#include "stdafx.h"
#include "ParticleMainOption.h"

#include "BinaryFile.h"
ParticleMainOption::ParticleMainOption()
	:mIsLoop(true), mDurationTime(5.f), mDurationDelayTime(1.f), mCurrentTime(0.f), mParticleLiveTime(5.f),
	mParticleSpeed(300.f),mParticleStartSize(50,50), mDurationCount(0),mCurrentDelayTime(0.f), mIsRelative(true)
{
	
}


ParticleMainOption::~ParticleMainOption()
{
}

void ParticleMainOption::OnGui()
{
	if (ImGui::CollapsingHeader("MainOption"))
	{
		ImGui::Checkbox("Loop", &mIsLoop);
		ImGui::Checkbox("RelativeCamera", &mIsRelative);
		ImGui::SliderFloat("DurationTime", &mDurationTime, 0.1f, 10.f);
		ImGui::SliderFloat("DurationDelay", &mDurationDelayTime, 0.1f, 10.f);
		ImGui::SliderInt("DurationCount", &mDurationCount, 0, 100);
		ImGui::SliderFloat("LiveTime", &mParticleLiveTime, 0.1f, 10.f);
		ImGui::SliderFloat("Speed", &mParticleSpeed, 0.1f, 1000.f);
		ImGui::SliderFloat2("Size", &mParticleStartSize.x, 0.1f, 500.f);
		ImGui::Separator();
	}
}

void ParticleMainOption::SaveData(BinaryWriter * w)
{
	w->Bool(mIsLoop);
	w->Bool(mIsRelative);
	w->Float(mDurationTime);
	w->Float(mDurationDelayTime);
	w->Int(mDurationCount);
	w->Float(mParticleLiveTime);
	w->Float(mParticleSpeed);
	w->Vector2D(mParticleStartSize);
}

void ParticleMainOption::LoadData(BinaryReader * r)
{
	mIsLoop = r->Bool();
	mIsRelative = r->Bool();
	mDurationTime = r->Float();
	mDurationDelayTime = r->Float();
	mDurationCount = r->Int();
	mParticleLiveTime = r->Float();
	mParticleSpeed = r->Float();
	mParticleStartSize = r->Vector2D();
}
