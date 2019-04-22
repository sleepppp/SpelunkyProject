#pragma once
class EffectOption
{
public:
	virtual void Execute(class FrameEffect* pEffect) = 0; 
};

class EffectOptionScale : public EffectOption
{
private:
	float mScalingRatio;
public:
	EffectOptionScale()
		:mScalingRatio(0.5f) {}
	void SetScaleingRatio(const float& scalingRatio) { mScalingRatio = scalingRatio; }
	void Execute(class FrameEffect* pEffect)override;
};

class EffectOptionAlphaBlending : public EffectOption
{
public:
	virtual void Execute(class FrameEffect* pEffect)override;
};

class EffectOptionTranslate : public EffectOption
{
public:
	void Execute(class FrameEffect* pEffect)override; 
};