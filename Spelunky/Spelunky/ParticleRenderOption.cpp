#include "stdafx.h"
#include "ParticleRenderOption.h"


ParticleRenderOption::ParticleRenderOption()
	:mRenderType(Particle::RenderType::Rectangle)
{
}


ParticleRenderOption::~ParticleRenderOption()
{
}

void ParticleRenderOption::OnGui()
{
	if (ImGui::CollapsingHeader("Render"))
	{
		if (ImGui::ColorEdit4("FontColor", &mColor.arr[0],
			ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreviewHalf)) {}
		
		ImGui::Separator();
	}
}
