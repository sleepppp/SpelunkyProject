#include "stdafx.h"
#include "ParticleRandomOption.h"


ParticleRandomOption::ParticleRandomOption()
	:mUseRandomLiveTime(false),mUseRandomSpeed(false),mUseRandomSize(false),mMinLiveTime(0.f),mMaxLiveTime(0.f),
	mMinSpeed(0.f),mMaxSpeed(0.f),mRenderType(Particle::RenderType::Rectangle),mUseRandomAngleAccelation(false),mUseRandomSpeedAccelation(false),
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
