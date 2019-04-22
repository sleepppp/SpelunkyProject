#include "stdafx.h"
#include "EffectOption.h"
#include "FrameEffect.h"
#include "Animation.h"
#include "Transform.h"
void EffectOptionScale::Execute(FrameEffect * pEffect)
{
	pEffect->mScale += mScalingRatio * _TimeManager->DeltaTime();
}

void EffectOptionAlphaBlending::Execute(FrameEffect * pEffect)
{
	pEffect->mAlpha = 1.f - pEffect->mAnimation->GetTotalCurrentTime() / pEffect->mAnimation->GetTotalFrameTime();
}

void EffectOptionTranslate::Execute(FrameEffect * pEffect)
{
	pEffect->GetTransform()->Translate(Vector2(0.f, -1.f) * 300.f * _TimeManager->DeltaTime());
}
