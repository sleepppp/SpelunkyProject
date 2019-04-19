#include "stdafx.h"
#include "particleShapeOption.h"

#include "BinaryFile.h"
ParticleShapeOption::ParticleShapeOption()
	:mShape(Shape::Corn),mRadius(100.f), mDirectionOffset(Math::PI / 4.f), mParticleStartDirection(0.f)
	,mUsePhysics(false), mMass(500.f)
{

}


ParticleShapeOption::~ParticleShapeOption()
{
}

void ParticleShapeOption::OnGui()
{
	if (ImGui::CollapsingHeader("ShapeOption"))
	{
		static int selectShape = 1;
		if (ImGui::Combo("Shape", &selectShape, "Circle\0Corn"))
			this->mShape = (Shape)selectShape;
		ImGui::SliderFloat("Radius", &mRadius, 0.f, 1000.f);
		ImGui::SliderFloat("Angle", &mParticleStartDirection, 0.0f, Math::PI * 2.f);
		ImGui::SliderFloat("AnglerCorn", &mDirectionOffset, 0.f, Math::PI);
		ImGui::Separator();
		ImGui::Checkbox("UsePhysics", &mUsePhysics);
		ImGui::SliderFloat("Mass", &mMass, 0.f, CastingFloat(_WinSizeX) / 2.f);
		ImGui::Separator();
	}
}

void ParticleShapeOption::SaveData(BinaryWriter * w)
{
	w->Int((int)mShape);
	w->Float(mRadius);
	w->Float(mDirectionOffset);
	w->Float(mParticleStartDirection);
	w->Float(mMass);
	w->Bool(mUsePhysics);
}

void ParticleShapeOption::LoadData(BinaryReader * r)
{
	mShape = (Shape)r->Int();
	mRadius = r->Float();
	mDirectionOffset = r->Float();
	mParticleStartDirection = r->Float();
	mMass = r->Float();
	mUsePhysics = r->Bool();
}
