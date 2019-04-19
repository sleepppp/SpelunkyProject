#include "stdafx.h"
#include "ParticleRenderOption.h"

#include "BinaryFile.h"

ParticleRenderOption::ParticleRenderOption()
	:mRenderType(Particle::RenderType::Rectangle)
{
	mImageList[RubbleType::Leaf].emplace_back(_ImageManager->FindImage("rubble"), 3, 2);
	mImageList[RubbleType::Stage1Rock].emplace_back(_ImageManager->FindImage("rubble"), 0,0);
	mImageList[RubbleType::Stage2Rock].emplace_back(_ImageManager->FindImage("rubble"), 0, 1);
	mImageList[RubbleType::Stage3Rock].emplace_back(_ImageManager->FindImage("rubble"), 6, 1);
}


ParticleRenderOption::~ParticleRenderOption()
{
	mImageList.clear();
}

void ParticleRenderOption::OnGui()
{
	if (ImGui::CollapsingHeader("Render"))
	{
		static int selectRenderType = 0;
		static int selectRubbleType = 0;

		if (ImGui::Combo("RenderType", &selectRenderType, "Rectangle\0Image"))
			this->mRenderType = (Particle::RenderType)selectRenderType;
		if (ImGui::Combo("RubbleType", &selectRubbleType, "Stage1Rock\0Stage2Rock\0Stage3Rock\0Leaf"))
			this->mRubbleType = (RubbleType)selectRubbleType;
		if (ImGui::ColorEdit4("FontColor", &mColor.arr[0],
			ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreviewHalf)) {}
		
		ImGui::Separator();
	}
}

void ParticleRenderOption::SaveData(BinaryWriter * w)
{
	w->Int((int)mRenderType);
	w->Int((int)mRubbleType);
	w->Float(mColor.r);
	w->Float(mColor.g);
	w->Float(mColor.b);
	w->Float(mColor.a);
}

void ParticleRenderOption::LoadData(BinaryReader * r)
{
	mRenderType = (Particle::RenderType)r->Int();
	mRubbleType = (RubbleType)r->Int();
	mColor.r = r->Float();
	mColor.g = r->Float();
	mColor.b = r->Float();
	mColor.a = r->Float();
}
