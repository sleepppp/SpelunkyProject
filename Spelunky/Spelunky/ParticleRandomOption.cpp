#include "stdafx.h"
#include "ParticleRandomOption.h"

#include "BinaryFile.h"

ParticleRandomOption::ParticleRandomOption()
	:mUseRandomLiveTime(false),mUseRandomSpeed(false),mUseRandomSize(false),mMinLiveTime(0.f),mMaxLiveTime(0.f),
	mMinSpeed(0.f),mMaxSpeed(0.f),mUseRandomAngleAccelation(false),mUseRandomSpeedAccelation(false),
	mMinSpeedAccelation(0.f),mMaxSpeedAccelation(500.f),mMinAngleAccelation(0.f),mMaxAngleAccelation(360.f)
{

}


ParticleRandomOption::~ParticleRandomOption()
{
}

void ParticleRandomOption::OnGui()
{
	if (ImGui::CollapsingHeader("Random"))
	{
		ImGui::Checkbox("RandomLiveTime", &mUseRandomLiveTime);
		ImGui::SliderFloat("MinLiveTime", &mMinLiveTime, 0.1f, 10.f);
		ImGui::SliderFloat("MaxLiveTime", &mMaxLiveTime, 0.1f, 10.f);
		ImGui::Checkbox("RandomSpeed", &mUseRandomSpeed);
		ImGui::SliderFloat("MinSpeed", &mMinSpeed, 1.f, 1000.f);
		ImGui::SliderFloat("MaxSpeed", &mMaxSpeed, 1.f, 1000.f);
		ImGui::Checkbox("RandomSize", &mUseRandomSize);
		ImGui::SliderFloat2("MinSize", &mMinSize.x, 1.f, 300.f);
		ImGui::SliderFloat2("MaxSize", &mMaxSize.x, 1.f, 300.f);
		ImGui::Checkbox("SpeedAccel", &mUseRandomSpeedAccelation);
		ImGui::SliderFloat("MinSpeedAccel", &mMinSpeedAccelation, 0.f, 500.f);
		ImGui::SliderFloat("MaxSpeedAccel", &mMaxSpeedAccelation, 0.f, 500.f);
		ImGui::Checkbox("AngleAccel", &mUseRandomAngleAccelation);
		ImGui::SliderFloat("MinAngleAccel", &mMinAngleAccelation, 0.f, 360.f);
		ImGui::SliderFloat("MaxAngleAccel", &mMaxAngleAccelation, 0.f, 360.f);
		ImGui::Separator();
	}
}

void ParticleRandomOption::SaveData(BinaryWriter * w)
{
	w->Bool(mUseRandomLiveTime);
	w->Float(mMinLiveTime);
	w->Float(mMaxLiveTime);

	w->Bool(mUseRandomSpeed);
	w->Float(mMinSpeed);
	w->Float(mMaxSpeed);

	w->Bool(mUseRandomSize);
	w->Vector2D(mMinSize);
	w->Vector2D(mMaxSize);

	w->Bool(mUseRandomSpeedAccelation);
	w->Float(mMinSpeedAccelation);
	w->Float(mMaxSpeedAccelation);

	w->Bool(mUseRandomAngleAccelation);
	w->Float(mMinAngleAccelation);
	w->Float(mMaxAngleAccelation);
}

void ParticleRandomOption::LoadData(BinaryReader * r)
{
	mUseRandomLiveTime = r->Bool();
	mMinLiveTime = r->Float();
	mMaxLiveTime = r->Float();

	mUseRandomSpeed = r->Bool();
	mMinSpeed = r->Float();
	mMaxSpeed = r->Float();

	mUseRandomSize = r->Bool();
	mMinSize = r->Vector2D();
	mMaxSize = r->Vector2D();

	mUseRandomSpeedAccelation = r->Bool();
	mMinSpeedAccelation = r->Float();
	mMaxSpeedAccelation = r->Float();

	mUseRandomAngleAccelation = r->Bool();
	mMinAngleAccelation = r->Float();
	mMaxAngleAccelation = r->Float();
}
