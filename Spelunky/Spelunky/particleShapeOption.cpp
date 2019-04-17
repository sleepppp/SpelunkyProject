#include "stdafx.h"
#include "particleShapeOption.h"


ParticleShapeOption::ParticleShapeOption()
	:mShape(Shape::Corn),mRadius(100.f), mDirectionOffset(Math::PI / 2.f), mParticleStartDirection(0.f)
{

}


ParticleShapeOption::~ParticleShapeOption()
{
}

void ParticleShapeOption::OnGui()
{
	if (ImGui::CollapsingHeader("Shape"))
	{
		ImGui::SliderFloat("Radius", &mRadius, 0.f, 500.f);
		ImGui::SliderFloat("Angle", &mParticleStartDirection, 0.0f, Math::PI * 2.f);
		ImGui::SliderFloat("AnglerCorn", &mDirectionOffset, 0.f, Math::PI);
		ImGui::Separator();
	}
}
