#include "GameObject.h"
class PointLight : public GameObject
{
protected:
	GameColor mColor;
	float mOriginRange;
	float mIntensity;
public:
	PointLight(Vector2 pos, float range = 460.f, float intensity = 1.25f, GameColor color = GameColor(1.f, 1.f, 1.f, 1.f));
	virtual ~PointLight();

	void Init()override;
	void Release()override;
	void Update()override;
	void OnGui()override; 

	GameColor GetColor()const { return this->mColor; }
	float GetOriginRange()const { return this->mOriginRange; }
	float GetIntensity()const { return this->mIntensity; }
	void SetColor(const GameColor color) { this->mColor = color; }
	void SetRange(const float range) { this->mOriginRange = range; }
	void SetIntensity(const float intensity) { this->mIntensity = intensity; }
};
